#include "DialogImageOptions.h"
#include "ui_DialogImageOptions.h"
#include "DialogChannelOffset.h"
#include "Ars3000/Ars3000.h"
#include "Ars1000/PipeInstrumentDriver.h"
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <QMessageBox>
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/Util.h>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "image.options")

using PipeDetect::PipeInstrumentDriver;

DialogImageOptions::DialogImageOptions(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogImageOptions),
    isRefreshing(false)
{
    ui->setupUi(this);

    QStringList names = MotionManager::axisNames();
    for (int index = 0; index < names.count(); ++index) {
        ui->axisH->addItem(names[index], index);
        ui->axisV->addItem(names[index], index);
    }

    for (UTChannel *channel: UltrasonicManager::channels()) {
        QAction *a = channelMenu.addAction(channel->name(), this, SLOT(RefreshData()));
        a->setCheckable(true);
    }

    for (QComboBox *control: findChildren<QComboBox *>()) {
        connect(control, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DialogImageOptions::refreshDataWithoutSwap);
    }
    for (QDoubleSpinBox *control: findChildren<QDoubleSpinBox *>()) {
        connect(control, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &DialogImageOptions::refreshDataWithoutSwap);
    }

    ui->channels->setMenu(&channelMenu);

    ui->channelOffsetList->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->channelOffsetList->addAction(ui->actionAdd);
    ui->channelOffsetList->addAction(ui->actionEdit);
    ui->channelOffsetList->addAction(ui->actionDelete);
    connect(ui->channelOffset, &QGroupBox::toggled, this, &DialogImageOptions::OnChannelOffsetSelectedChanged);
    connect(ui->channelOffsetList, &QTreeWidget::itemSelectionChanged, this, &DialogImageOptions::OnChannelOffsetSelectedChanged);
    connect(ui->channelOffsetList, &QTreeWidget::itemDoubleClicked, this, &DialogImageOptions::OnEditOffset);
    connect(ui->addOffset, &QToolButton::clicked, this, &DialogImageOptions::OnAddOffset);
    connect(ui->editOffset, &QToolButton::clicked, this, &DialogImageOptions::OnEditOffset);
    connect(ui->deleteOffset, &QToolButton::clicked, this, &DialogImageOptions::OnDeleteOffset);
    OnChannelOffsetSelectedChanged();

    RefreshUi();
    resize(minimumSize());
}

DialogImageOptions::~DialogImageOptions()
{
    delete ui;
}

ImageOptions DialogImageOptions::GetDefaultOptions() const
{
    return defaultOptions;
}

void DialogImageOptions::SetOptions(const ImageOptions &newOptions)
{
    if (options == newOptions) {
        return;
    }
    options = newOptions;
    RefreshUi();
}

void DialogImageOptions::SetDefaultOptions(const ImageOptions &newDefaults)
{
    defaultOptions = newDefaults;
    auto buttons = ui->buttonBox->standardButtons();
    if (newDefaults.IsValid()) {
        ui->buttonBox->setStandardButtons(buttons | QDialogButtonBox::RestoreDefaults);
    } else {
        ui->buttonBox->setStandardButtons(buttons ^ QDialogButtonBox::RestoreDefaults);
    }
}

void DialogImageOptions::resetFeatrueComboBox(int featureIndex)
{
    UTDataHeader dataHeader;
#ifdef PIPE_DETECT
    dataHeader = PipeInstrumentDriver::InitDataHeader();
#else
    dataHeader = ARS3000::InitDataHeader();
#endif

    ui->gateFeature->clear();
    int numFeature = dataHeader.GetNumFeatures();
    for (int index = 0; index < numFeature; ++index) {
        QString name = "";
        QString dataHeaderName = dataHeader.GetName(index);
        if (index < 3) {
            name = tr("1闸门 %1").arg(dataHeaderName);
        } else {
            name = tr("%1闸门 %2").arg((index - 3) / 5 + 2).arg(dataHeaderName);
        }
        ui->gateFeature->addItem(name);
    }

    if (featureIndex < numFeature) {
        ui->gateFeature->setCurrentIndex(featureIndex);
    } else if (numFeature > 0) {
        ui->gateFeature->setCurrentIndex(0);
    }
}

void DialogImageOptions::RefreshUi()
{
    isRefreshing = true;
    ui->axisH->setCurrentIndex(ui->axisH->findData(options.axisX()));
    Axis *axisX = MotionManager::axis(options.axisX());
    const QString unitX = axisX ? axisX->axisUnit() : QString();
    foreach (QDoubleSpinBox *box, ui->optionsH->findChildren<QDoubleSpinBox *>()) {
        box->setSuffix(unitX);
    }

    ui->axisV->setCurrentIndex(ui->axisV->findData(options.axisY()));
    Axis *axisY = MotionManager::axis(options.axisY());
    const QString unitY = axisY ? axisY->axisUnit() : QString();
    foreach (QDoubleSpinBox *box, ui->optionsV->findChildren<QDoubleSpinBox *>()) {
        box->setSuffix(unitY);
    }

    const QRectF &source = options.sourceRect();
    ui->sourceFromH->setValue(source.left());
    ui->sourceToH->setValue(source.right());
    ui->sourceWidthH->setText(QString::number(source.width()));

    ui->sourceFromV->setValue(source.top());
    ui->sourceToV->setValue(source.bottom());
    ui->sourceWidthV->setText(QString::number(source.height()));

    const QSizeF &grid = options.gridSize();
    ui->gridH->setValue(grid.width());
    ui->gridV->setValue(grid.height());

    const QSize &target = options.targetRect();
    if (target.isValid()) {
        ui->pixelH->setText(QString::number(target.width()));
        ui->pixelV->setText(QString::number(target.height()));
    } else {
        ui->pixelH->setText(tr("N/A"));
        ui->pixelV->setText(tr("N/A"));
    }

    QPointF backlash = options.backlash();
    ui->backlashH->setValue(backlash.x());
    ui->backlashV->setValue(backlash.y());

    QSizeF scale = options.scale();
    scale = scale.isValid() ? scale : QSizeF(1.00, 1.00);
    ui->scaleH->setValue(scale.width());
    ui->scaleV->setValue(scale.height());

    ui->coordinateType->setCurrentIndex(options.coordinateType());
    ui->cyclularCenter->setValue(options.cyclularCenter());
    ui->cyclularCenter->setEnabled(options.coordinateType() == ImageOptions::CoordThetaR);
    ui->cyclularCenterTitle->setEnabled(options.coordinateType() == ImageOptions::CoordThetaR);

    /* 成像通道 */
    auto actions = channelMenu.actions();
    QStringList channelNames;
    for (int index = 0; index < actions.count(); ++index) {
        bool enabled = options.isChannelEnabled(index);
        actions[index]->setChecked(enabled);
        if (enabled) {
            channelNames << actions[index]->text();
        }
    }

    if (options.channels().toList().isEmpty()) {    //没有加载文件
        ui->channels->setText(tr("未选择"));
    } else {
        if (channelNames.isEmpty()) {       //添加默认的通道号，只加界面，不加工艺
            m_defaultChannel = options.channels().toList().last();
            channelNames << tr("通道 %1").arg(m_defaultChannel + 1);
            QAction *a = channelMenu.addAction(channelNames.last(), this, SLOT(RefreshData()));
            a->setCheckable(true);
            a->setChecked(true);
        }
        ui->channels->setText(channelNames.join('/'));
    }

    /* 特征值 */
    resetFeatrueComboBox(options.featureIndex());

    /* 归并 */
    ui->reduce->setCurrentIndex(options.reducePolicy());
    ui->directionFilter->setCurrentIndex(options.directionFilter());

    /* 通道偏移 */
    ui->channelOffsetList->clear();
    QVector<QVector<float> > channelOffset = options.channelOffset();
    for (int channel = 0; channel < channelOffset.count(); ++channel) {
        UTChannel *utchannel = UltrasonicManager::channel(channel);
        if (utchannel != nullptr) {
            QStringList offsetText;
            foreach (float value, channelOffset.value(channel)) {
                offsetText << QString::number(value);
            }
            if (offsetText.isEmpty()) {
                continue;
            }
            auto *item = new QTreeWidgetItem(ui->channelOffsetList);
            item->setText(0, utchannel->name());
            item->setText(1, offsetText.join(","));
            item->setData(0, Qt::UserRole, utchannel->channelId());
        }
    }
    ui->channelOffsetList->resizeColumnToContents(0);

    isRefreshing = false;
}

void DialogImageOptions::RefreshData(bool swap)
{
    if (isRefreshing) {
        return;
    }
    qreal left = ui->sourceFromH->value();
    qreal top = ui->sourceFromV->value();
    qreal width = ui->sourceToH->value() - left;
    qreal height = ui->sourceToV->value() - top;
    int axisX = ui->axisH->currentData().toInt();
    int axisY = ui->axisV->currentData().toInt();

    QRectF source(left, top, width, height);
    QSizeF grid(ui->gridH->value(), ui->gridV->value());

    if (swap) {
        qSwap(axisX, axisY);
        source = QRectF(top, left, height, width);
        qSwap(grid.rwidth(), grid.rheight());
    }

    auto type = ImageOptions::CoordinateType(ui->coordinateType->currentIndex());
    float center = ui->cyclularCenter->value();

    ImageOptions newOptions(source, grid);
    newOptions.setAxisX(axisX);
    newOptions.setAxisY(axisY);
    newOptions.setScale(QSizeF(ui->scaleH->value(), ui->scaleV->value()));
    newOptions.setBacklash(QPointF(ui->backlashH->value(), ui->backlashV->value()));
    newOptions.setCoordinateType(type);
    newOptions.setCyclularCenter(center);
    MotionAxisList channels;
    auto actions = channelMenu.actions();
    for (int index = actions.count() - 1; index >= 0; --index) {
        channels.setAxis(index, actions[index]->isChecked());
    }

    if (channels.toList().isEmpty()) {
        channels.setAxis(m_defaultChannel, true);
    }

    newOptions.setChannels(channels);

    newOptions.setFeatureIndex(ui->gateFeature->currentIndex());
    newOptions.setReducePolicy(ImageOptions::ReducePolicy(ui->reduce->currentIndex()));
    newOptions.setDirectionFilter(ImageOptions::DirectionFilter(ui->directionFilter->currentIndex()));

    QVector<QVector<float> > channelOffset;
    int channelCount = UltrasonicManager::channels().count();
    channelOffset.resize(UltrasonicManager::channels().count());
    for (int row = 0; row < ui->channelOffsetList->topLevelItemCount(); ++row) {
        QTreeWidgetItem *item = ui->channelOffsetList->topLevelItem(row);
        if (!item) {
            continue;
        }
        int channelIndex = item->data(0, Qt::UserRole).toInt();
        if (channelIndex >= 0 && channelIndex < channelCount) {
            QStringList offsetText = item->text(1).split(",");
            QVector<float> offsetList;
            foreach (QString text, offsetText) {
                offsetList << text.toFloat();
            }
            channelOffset.replace(channelIndex, offsetList);
        }
    }
    newOptions.setChannelOffset(channelOffset);
    SetOptions(newOptions);
}

void DialogImageOptions::refreshDataWithoutSwap()
{
    RefreshData();
}

void DialogImageOptions::on_swap_clicked()
{
    RefreshData(true);
}

void DialogImageOptions::on_buttonBox_clicked(QAbstractButton *button)
{
    if (ui->buttonBox->standardButton(button) == QDialogButtonBox::RestoreDefaults) {
        options = defaultOptions;
        RefreshUi();
    }
}

void DialogImageOptions::OnChannelOffsetSelectedChanged()
{
    bool selected = ui->channelOffsetList->selectedItems().count() > 0;
    ui->editOffset->setEnabled(selected);
    ui->actionEdit->setEnabled(selected);
    ui->deleteOffset->setEnabled(selected);
    ui->actionDelete->setEnabled(selected);
}

void DialogImageOptions::OnAddOffset()
{
    DialogChannelOffset dialogOffset;
    if (dialogOffset.exec() == QDialog::Rejected) {
        return;
    }
    QPair<int, QVector<float> > offset = dialogOffset.channelOffset();
    for (int row = 0; row < ui->channelOffsetList->topLevelItemCount(); ++row) {
        QTreeWidgetItem *item = ui->channelOffsetList->topLevelItem(row);
        if (!item) {
            continue;
        }
        if (item->data(0, Qt::UserRole).toInt() == offset.first) {
            QMessageBox::warning(this, QString(), tr("当前通道偏移已经存在"));
            return;
        }
    }
    UTChannel *channel = UltrasonicManager::channel(offset.first);
    if (channel) {
        auto *item = new QTreeWidgetItem(ui->channelOffsetList);
        item->setData(0, Qt::UserRole, offset.first);
        item->setText(0, channel->name());
        item->setText(1, JoinNumber(offset.second));
    }
    RefreshData();
}

void DialogImageOptions::OnEditOffset()
{
    QTreeWidgetItem *item = ui->channelOffsetList->selectedItems().value(0);
    if (!item) {
        return;
    }
    int channel = item->data(0, Qt::UserRole).toInt();
    QVector<float> vector = SplitNumber<float>(item->text(1)).toVector();
    DialogChannelOffset dialogOffset;
    dialogOffset.setChannelOffset(qMakePair(channel, vector));
    if (dialogOffset.exec() == QDialog::Accepted) {
        QPair<int, QVector<float> > offset = dialogOffset.channelOffset();
        item->setText(1, JoinNumber(offset.second));
        RefreshData();
    }
}

void DialogImageOptions::OnDeleteOffset()
{
    QList<QTreeWidgetItem *> items = ui->channelOffsetList->selectedItems();
    if (items.count() <= 0) {
        return;
    }
    if (QMessageBox::question(this, QString(), tr("确定要删除被选中的通道偏移吗"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        foreach (QTreeWidgetItem *item, items) {
            delete item;
        }
        RefreshData();
    }
}
