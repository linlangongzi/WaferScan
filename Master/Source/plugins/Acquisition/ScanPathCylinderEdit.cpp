#include "ScanPathCylinderEdit.h"
#include "ui_ScanPathCylinderEdit.h"
#include <Motion/EndPoint.h>
#include <Motion/MotionManager.h>
#include <QInputDialog>
#include <QMessageBox>
#include <Ultrasonic/UltrasonicManager.h>
#include <Waveform/DialogUTSetup.h>
#include <utils/algorithm.h>
#include <QtMath>
#include <Core/LicenseManager.h>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "acquisition.cylinderEdit")

ScanPathCylinderEdit::ScanPathCylinderEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanPathCylinderEdit)
{
    ui->setupUi(this);

    ui->scanAxis->setFilter([](Axis *axis){
        return (axis->axisType() == AxisConfig::Rotation);
    });
    ui->linearSpeed->setVisible(false);
    ui->rotatorAngularitySpeed->setVisible(false);
    ui->cylinderAngularitySpeed->setVisible(false);

    for (UTChannel *channel: UltrasonicManager::channels()) {
        ui->channelList->addItem(channel->name(), channel->channelId());
    }
    ui->calculationMode->addItems({tr("全自动计算"), tr("指定线速度"), tr("指定承载轮角速度"), tr("指定工件角速度")});
    for (auto *control: {ui->linearSpeed, ui->rotatorAngularitySpeed, ui->cylinderAngularitySpeed, ui->cylinderDiameter, ui->scanInterval}) {
        connect(control, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ScanPathCylinderEdit::refreshControls);
    }
    connect(ui->calculationMode, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ScanPathCylinderEdit::refreshControls);
    connect(ui->scanAxis, QOverload<int>::of(&AxisSelector::currentIndexChanged), this, &ScanPathCylinderEdit::refreshControls);

    for (UTChannel *channel: UltrasonicManager::channels()) {
        connect(channel, &UTChannel::setupChanged, this, [&]() {
            ui->linearSpeed->setValue(options().effectiveLinearSpeed());
            ui->linearSpeedText->setText(tr("%1 mm/s").arg(options().effectiveLinearSpeed(), 0, 'f', MotionManager::displayPrecision()));
        });
    }
}

ScanPathCylinderEdit::~ScanPathCylinderEdit()
{
    delete ui;
}

ScanPathCylinderOptions ScanPathCylinderEdit::options() const
{
    using Options = ScanPathCylinderOptions;

    Options options;
    options.endPointId = ui->endPointAxis->currentId();
    options.scanAxis = ui->scanAxis->currentAxisUuid();
    options.scanInterval = ui->scanInterval->value();
    options.mode = ScanPathCylinderOptions::CalculationMode(ui->calculationMode->currentIndex());

    options.linearSpeed = ui->linearSpeed->value();
    options.stepInterval = ui->stepInterval->value();
    options.rotatorAngularSpeed = ui->rotatorAngularitySpeed->value();
    options.cylinderDiameter = ui->cylinderDiameter->value();
    options.cylinderAngularSpeed = ui->cylinderAngularitySpeed->value();
    options.posList.insert(0, ui->start->nativePos());
    options.posList.insert(options.posList.count(), ui->end->nativePos());

    options.setupEnable = ui->setup->isChecked();
    options.setup = m_setup;
    options.channelSelect = ui->singleChannel->isChecked()? Options::Single : Options::Parallel;
    options.channel = ui->channelList->currentData(Qt::UserRole).toInt();
    options.channels = channelsInListWidget();
    options.alterSourceEnable = ui->alterSource->isChecked();
    options.gainOffset = ui->gain->value();
    options.probeInterval = ui->probeDistance->value();

    return options;
}

void ScanPathCylinderEdit::setOptions(const ScanPathCylinderOptions &options)
{
    using Options = ScanPathCylinderOptions;

    ui->endPointAxis->setCurrentUuid(options.endPointId);
    ui->scanAxis->setCurrentUuid(options.scanAxis);
    ui->scanInterval->setValue(options.scanInterval);
    ui->calculationMode->setCurrentIndex(options.mode);

    ui->linearSpeed->setValue(options.effectiveLinearSpeed());
    ui->stepInterval->setValue(options.stepInterval);
    ui->rotatorAngularitySpeed->setValue(options.effectiveRotatorAngularSpeed());
    ui->cylinderDiameter->setValue(options.cylinderDiameter);
    ui->cylinderAngularitySpeed->setValue(options.effectiveCylinderAngularSpeed());

    m_setup = options.setup;

    ui->singleChannel->setChecked(options.channelSelect == Options::Single);
    ui->multiChannel->setChecked(options.channelSelect == Options::Parallel);
    foreach (auto channel, options.channels) {
        addChannel(channel);
    }
    ui->channelList->setCurrentIndex(ui->channelList->findData(options.channel));
    ui->setup->setChecked(options.setupEnable);
    ui->editSetup->setEnabled(options.setupEnable);
    ui->start->setNativePos(options.posList.isEmpty() ? QVector3D() : options.posList.first());
    ui->end->setNativePos(options.posList.isEmpty() ? QVector3D() : options.posList.last());
    ui->alterSource->setChecked(options.alterSourceEnable);
    ui->probeDistance->setValue(options.probeInterval);
    ui->gain->setValue(options.gainOffset);

    Axis *axis = MotionManager::axis(options.scanAxis);
    if (axis) {
        ui->rotatorDiameter->setValue(axis->config().axleDiameter);
    }
}

void ScanPathCylinderEdit::refreshControls()
{
    auto option = options();
    ui->linearSpeed->setVisible(option.mode == ScanPathCylinderOptions::SpecifyLinearSpeed);
    ui->linearSpeedText->setVisible(option.mode != ScanPathCylinderOptions::SpecifyLinearSpeed);
    ui->rotatorAngularitySpeed->setVisible(option.mode == ScanPathCylinderOptions::SpecifyRotatorAngularSpeed);
    ui->rotatorAngularitySpeedText->setVisible(option.mode != ScanPathCylinderOptions::SpecifyRotatorAngularSpeed);
    ui->cylinderAngularitySpeed->setVisible(option.mode == ScanPathCylinderOptions::SpecifyCylinderAngularSpeed);
    ui->cylinderAngularSpeedText->setVisible(option.mode != ScanPathCylinderOptions::SpecifyCylinderAngularSpeed);

    ui->linearSpeedText->setText(tr("%1 mm/s").arg(option.advicedLinearSpeed(), 0, 'f', MotionManager::displayPrecision()));
    ui->rotatorAngularitySpeedText->setText(tr("%1 °/s").arg(option.advicedRotatorAngularSpeed(), 0, 'f', MotionManager::displayPrecision()));
    ui->cylinderAngularSpeedText->setText(tr("%1 °/s").arg(option.advicedCylinderAngularSpeed(), 0, 'f', MotionManager::displayPrecision()));

    ui->linearSpeed->setStyleSheet(option.linearSpeed > option.advicedLinearSpeed() ? QString("color: red;") : QString());
    ui->rotatorAngularitySpeed->setStyleSheet(option.rotatorAngularSpeed > option.advicedRotatorAngularSpeed() ? QString("color: red;") : QString());
    ui->cylinderAngularitySpeed->setStyleSheet(option.cylinderAngularSpeed > option.advicedCylinderAngularSpeed() ? QString("color: red;") : QString());
}

void ScanPathCylinderEdit::on_scanAxis_currentIndexChanged(int index)
{
    Axis *axis = MotionManager::axis(index);
    if (axis != nullptr) {
        ui->rotatorDiameter->setValue(axis->config().axleDiameter);
    }
}

UTSetup ScanPathCylinderEdit::setup() const
{
    const auto channels = UltrasonicManager::channels();
    if (channels.isEmpty()) {
        return {};
    }
    return channels.first()->setup();
}

void ScanPathCylinderEdit::on_editSetup_clicked()
{
    DialogUTSetup dialog(this);
    UTSetup setup = this->setup();
    dialog.setSetup(setup);
    if (dialog.exec() == QDialog::Accepted) {
        m_setup = setup;
        refreshControls();
    }
}

QList<int> ScanPathCylinderEdit::channelsInListWidget() const
{
    QList<int> channels;
    for (int row = 0; row < ui->multiChannelList->count(); ++row) {
        auto *item = ui->multiChannelList->item(row);
        channels << item->data(Qt::UserRole).toInt();
    }
    return channels;
}

void ScanPathCylinderEdit::onLicenseUpdated()
{
    bool develop = LicenseManager::instance()->hasDevelopLicense();
    ui->gain->setVisible(develop);
    ui->gainTitle->setVisible(develop);
}

void ScanPathCylinderEdit::addChannel(int channelId)
{
    UTChannel *channel = UltrasonicManager::channel(channelId);
    if (channel == nullptr) {
        return;
    }
    auto *item = new QListWidgetItem;
    item->setText(channel->name());
    item->setData(Qt::UserRole, channelId);
    ui->multiChannelList->addItem(item);
}

void ScanPathCylinderEdit::on_addChannel_clicked()
{
    QList<int> channels = channelsInListWidget();
    QList<UTChannel *> channelsFiltered = Utils::filtered(UltrasonicManager::channels(), [&](UTChannel *channel){
        return !channels.contains(channel->channelId());
    });
    if (channelsFiltered.isEmpty()) {
        QMessageBox::information(this, QString(), tr("没有可以添加的多余通道"));
        return;
    }
    QStringList names = Utils::transform(channelsFiltered, &UTChannel::name);
    bool ok = false;
    QString name = QInputDialog::getItem(this, QString(), tr("通道选择"), names, 0, false, &ok);
    if (!ok) {
        return;
    }
    addChannel(channelsFiltered[names.indexOf(name)]->channelId());
}

void ScanPathCylinderEdit::on_removeChannel_clicked()
{
    delete ui->multiChannelList->takeItem(ui->multiChannelList->currentRow());
}

void ScanPathCylinderEdit::on_setup_toggled(bool checked)
{
    ui->editSetup->setEnabled(checked);
}

void ScanPathCylinderEdit::on_multiChannel_toggled(bool checked)
{
    ui->channelList->setEnabled(!checked);
    ui->multiChannelGroup->setEnabled(checked);
}

