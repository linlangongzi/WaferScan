#include "DialogMaterial.h"
#include "DialogProbeList.h"
#include "PanelAScan.h"
#include "PanelUTSetup.h"
#include "ui_DialogUTSetupTimebase.h"
#include "ui_PanelAScan.h"
#include <Core/ICore.h>
#include <QApplication>
#include <QClipboard>
#include <QDateTime>
#include <QFileDialog>
#include <QInputDialog>
#include <QLoggingCategory>
#include <QMainWindow>
#include <QMatrix4x4>
#include <QMenu>
#include <QMessageBox>
#include <QProgressDialog>
#include <QWindow>
#include <Ultrasonic/ChannelModel.h>
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UTInstrument.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/QtExtend.h>
#include <allrising/Util.h>
#include <utils/algorithm.h>
#ifdef QT_CONCURRENT_LIB
#   include <QtConcurrent>
#   include <functional>
#endif

using namespace Core;
static Q_LOGGING_CATEGORY(debug, "wave.ascan")

PanelAScan::PanelAScan(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PanelAScan),
    handler(this, &PanelAScan::setup, &PanelAScan::setSetup)
{
    ui->setupUi(this);

    channelModel = new ChannelModel(this);
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(channelModel);
    proxyModel->setFilterKeyColumn(-1);

    ui->channelList->setModel(proxyModel);
    ui->channelList->setModelColumn(ChannelModel::ColName);
    ui->channels->setModel(proxyModel);
    ui->channels->setModelColumn(ChannelModel::ColName);

    connect(ui->channelFilter, &QLineEdit::textChanged, proxyModel, &QSortFilterProxyModel::setFilterFixedString);
    connect(channelModel, &QAbstractItemModel::modelReset, this, [this]() {
        int index = ui->channelList->findData(m_channelId, ChannelModel::ChannelIdRole);
        if (index < 0 && ui->channelList->count() > 0) {
            index = 0;
        }
        ui->channelList->setCurrentIndex(index);
        QModelIndex modelIndex = proxyModel->index(index, 1, QModelIndex());
        if (modelIndex.isValid()) {
            ui->channels->selectionModel()->setCurrentIndex(modelIndex, QItemSelectionModel::SelectCurrent);
        }
    });

    addActions(findChildren<QAction *>());

    UiInit();                                                                   // UI初始化
    InitAction();
    // 与AScanGraphicsView绑定
    connect(ui->actionFeature, &QAction::toggled, ui->view, &AScanGraphicsView::setFeatureMarkVisible);
    ui->view->setFeatureMarkVisible(ui->actionFeature->isChecked());
    connect(ui->actionCursor, &QAction::toggled, ui->view, &AScanGraphicsView::setCursorEnabled);
    ui->view->setCursorEnabled(ui->actionCursor->isChecked());
    connect(ui->actionFillWaveform, &QAction::triggered, ui->view, &AScanGraphicsView::setWaveformFill);
    ui->view->setWaveformFill(ui->actionFillWaveform->isChecked());

    connect(ui->view, &AScanGraphicsView::onHorizontalRulerMouseDoubleClicked, this, &PanelAScan::openMaterialDialog);
    connect(ui->view, &AScanGraphicsView::setupChanged, this, &PanelAScan::setSetup);

    ui->material->installEventFilter(this);

    ICore::onInternalVersion(this, [=](bool internal) {
        ui->position->setVisible(internal);
        ui->actionUseDataSegment->setVisible(internal);
//        ui->updateFeature->setVisible(internal);

        ui->actionExportCsv->setVisible(internal);
        ui->actionBladeTest->setVisible(internal);
        ui->actionBladeSplit->setVisible(internal);
        ui->actionBladeSplitSlice->setVisible(internal);
    });

    ui->alarm->slotAlarmHide();
}

PanelAScan::~PanelAScan()
{
    delete ui;
}

QJsonObject PanelAScan::saveSettings() const
{
    QJsonObject obj;

    QJsonObject actions;
    for (QAction *action: findChildren<QAction *>(QString(), Qt::FindDirectChildrenOnly)) {
        if (action->isCheckable()) {
            actions[action->objectName()] = action->isChecked();
        }
    }
    obj["Actions"] = actions;

    return obj;
}

void PanelAScan::loadSettings(const QJsonObject &layout)
{
    const QJsonObject actions = layout["Actions"].toObject();
    for (QAction *action: findChildren<QAction *>(QString(), Qt::FindDirectChildrenOnly)) {
        const QString name = action->objectName();
        if (action->isCheckable() && actions.contains(name)) {
            action->setChecked(actions[name].toBool());
        }
    }
}

void PanelAScan::SetToolBarVisible(bool enabled)
{
    ui->statusBarContainer->setVisible(enabled);
}

void PanelAScan::SetFrozenData(const UTDataFrame &frame)
{
    SetMonitorData(frame);
    ui->actionFreeze->setChecked(true);
}

void PanelAScan::setSetup(const UTSetup &setup)
{
    if (m_setup == setup) {
        return;
    }
    m_setup = setup;

    handler.setControls(setup);

    ui->view->setSetup(setup);
    ui->material->setText( tr("%1 %2 m/s").arg(setup.materialName()).arg(setup.materialSpeed(), 4, 'f', 0) );

    QSignalBlocker blocker(this);
    ui->gain->setValue(setup.gain());
    ui->gainSlider->setValue(setup.gain() * 10);
    ui->gainRef->setValue(setup.gainRef());
    ui->gainRefSlider->setValue(setup.gainRef() * 10);
    blocker.unblock();

    emit setupChanged(setup);
}

QPixmap PanelAScan::RenderPixmap()
{
    QPixmap pixmap(size());
    render(&pixmap);
    return pixmap;
}

void PanelAScan::ProjectCustomizeUI(QString projectName)
{
    if (projectName == "PIPE_DETECT") {
        ui->line_6->hide();
        ui->line_7->hide();
        ui->alarm->hide();
        ui->line_9->hide();
        ui->material->hide();
        ui->waterPath->hide();
        ui->statusBarContainer->setMinimumWidth(600);
    }
}

UTSetup PanelAScan::setup() const
{
    return m_setup;
}

void PanelAScan::setSetupLocked(bool locked)
{
    ui->view->setSetupLocked(locked);
    ui->gain->setDisabled(locked);
    ui->gainRef->setDisabled(locked);
    ui->gainSlider->setDisabled(locked);
    ui->gainRefSlider->setDisabled(locked);
    ui->gainPlus->setDisabled(locked);
    ui->gainMinus->setDisabled(locked);
}

bool PanelAScan::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonDblClick) {
        if (obj == ui->material) {
            openMaterialDlg();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void PanelAScan::setInstrumentInfo(const InstrumentInfo &info)
{
    ui->gain->setMaximum(info.maxGain * 10);
    ui->gainSlider->setMaximum(info.maxGain * 10);
    m_info = info;
    if (curChannel != nullptr) {
        m_info.supportAlterSource &= curChannel->instrumentIndex() != 0;
    }
    panelUTSetup->setInstrumentInfo(m_info);
}

void PanelAScan::setChannelId(int channelId)
{
    UTChannelPointer channel = UltrasonicManager::channel(channelId);
    if (channel.isNull()) {
        qCDebug(debug, "Null channelId: %d", channelId);
        m_channelId = -1;
        return;
    } else if (channelId == m_channelId) {
        return;
    }

    m_channelId = channelId;
    int index = ui->channelList->findData(channelId, ChannelModel::ChannelIdRole);
    ui->channelList->setCurrentIndex(index);
    QModelIndex modelIndex = proxyModel->index(index, 1, QModelIndex());
    if (modelIndex.isValid()) {
        ui->channels->selectionModel()->setCurrentIndex(modelIndex, QItemSelectionModel::SelectCurrent);
    }

    if (curChannel) {
        disconnect(curChannel, nullptr, this, nullptr);
        disconnect(this, nullptr, curChannel, nullptr);
        if (curChannel->instrument()) {
            disconnect(curChannel->instrument(), nullptr, this, nullptr);
        }
    }

    curChannel = channel;
    if (curChannel == nullptr) {
        return;
    }
    UTInstrument *curInstrument = curChannel->instrument();
    if (curInstrument == nullptr) {
        return;
    }
    connect(curInstrument, &UTInstrument::instrumentInfoChanged, this, &PanelAScan::setInstrumentInfo);
    setInstrumentInfo(curInstrument->instrumentInfo());
    curInstrument->setActiveChannel(curChannel->instrumentIndex());                       // 通知Instrument当前通道已经变更
    if ( curChannel.isNull() ) {
        qWarning() << "PanelAScan::ChannelChanged instrumentID got by combobox is not valid";
        return;
    }

    setWindowTitle(tr("A扫描 - %1").arg(curChannel->name()));
    connect(curChannel, &UTChannel::MonitorDataArrived, this, &PanelAScan::OnMonitorDataArrived);// 绑定当前通道，接收当前通道AData数据
    connect(curChannel, &UTChannel::dataArrived, this, &PanelAScan::OnDataArrived);

    connect(curChannel, &UTChannel::setupChanged, this, &PanelAScan::setSetup);
    connect(this, &PanelAScan::setupChanged, curChannel, &UTChannel::setSetup);

    connect(curChannel, &UTChannel::probeChanged, this, [&] (const Probe &probe) {
        ui->actionSelectProbe->setText(tr("选择探头(%1)").arg(probe.name));
    });

    setSetup(curChannel->setup());
    handler.setControls(curChannel->setup());

    panelUTSetup->setChannel(curChannel);
    // 0通道不显示关联检测
    panelUTSetup->hideAlterSourceCheckbox(channelId == 0);

    onInstrumentConnectChanged(InstrumentStatus());
    UTInstrument *instrument = curChannel->instrument();
    if (instrument) {
        connect(instrument, &UTInstrument::instrumentStatusChanged, this, &PanelAScan::onInstrumentConnectChanged);
        instrument->setActiveChannel(curChannel->channelId());
    }

    SetMonitorData(UTDataFrame());                                        // 切换通道后需要清空原来通道的波形

    ui->view->setChannel(curChannel);
}

void PanelAScan::openMaterialDialog()
{
    QDialog dialog(this);
    Ui::DialogUTSetupTimebase ui;
    ui.setupUi(&dialog);

    UTSetup setup = m_setup;
    ui.timebase->setTimebase(setup.timebase());
    connect(ui.timebase, &PanelUTSetupTimebase::timebaseChanged, this, [=](const UTSetupTimebase &timebase) {
        handler.set(&UTSetup::setTimebase, timebase);
    });

    /* 用户选择取消时, 恢复旧的时序 */
    if (dialog.exec() != QDialog::Accepted) {
        setSetup(setup);
    }
}

void PanelAScan::openMaterialDlg()
{
    DialogMaterial dialog;
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    handler.set(&UTSetup::setMaterial, dialog.currentMaterial());
}

/*! 仪器连接状态改变 */
void PanelAScan::onInstrumentConnectChanged(const InstrumentStatus &status)
{
    Q_UNUSED(status)
    UTInstrument *instrument = curChannel.isNull() ? nullptr : curChannel->instrument();
    ui->connect->setVisible(instrument);
    if (instrument == nullptr) {
        ui->connectionStatus->setText(tr("仪器不可用"));
    } else {
        ui->instrumentContainer->setVisible(instrument->instrumentStatus().state != InstrumentStatus::Connected);
        ui->connectionStatus->setText(instrument->instrumentStatus().text());
    }
}

void PanelAScan::OnOpenDAC()
{
    QWindow *window = setupWindow->windowHandle();
    if (window) {
        window->alert(1);
    }
    panelUTSetup->setTabPage("tabDac");
}

void PanelAScan::SetMonitorData(UTDataFrame data)                         // A扫描收到数据并告知PlotterData更新数据并画图
{
    lastFrame = data;

    qreal waterPath = data.waterPathLength();
    ui->waterPath->setText(tr("水距: %1 mm").arg(waterPath, 0, 'f', 2));

    if (data.isGateTriggered(gateIndex)) {  //默认打开闸门2的报警
        ui->alarm->alarmShow();
    }
#ifdef PIPE_DETECT
    ui->position->setText(tr("X:%1 mm--R:%2").arg(data->GetPose(0), 0, 'f', 2).arg(data->GetPose(1), 0, 'f', 2));
#endif

    panelUTSetup->setDataFrame(data);
    ui->view->setMonitorData(data);
}

void PanelAScan::OnMonitorDataArrived(UTDataFrame frame)
{
    if (!ui->actionFreeze->isChecked() && !ui->actionUseDataSegment->isChecked()) {
        SetMonitorData(frame);
    }
}

void PanelAScan::OnDataArrived(UTDataSegment segment)
{
    if (segment.isEmpty()) {
        return;
    }
    if (!ui->actionFreeze->isChecked() && ui->actionUseDataSegment->isChecked()) {
        SetMonitorData(segment.first());
    }
}

void PanelAScan::UiInit()                                                       // UI初始化
{
    //设定UI中各个控件的尺寸
    ui->gainShow->setChecked(false);
    // 创建setup界面
    setupWindow = new QDialog(this, Qt::Tool);
    setupWindow->setLayout(new QVBoxLayout(setupWindow));
    setupWindow->setObjectName(objectName() + "UTSetup");
    setupWindow->hide();

    panelUTSetup = new PanelUTSetup(setupWindow);
    ui->mainLayout->addWidget(panelUTSetup);

    connect(this, &PanelAScan::windowTitleChanged, [this](QString title){
        setupWindow->setWindowTitle(title + tr(" - 工艺") );
    });
    connect(setupWindow, &QDialog::finished, this, [this] () {
        setupWindow->layout()->removeWidget(panelUTSetup);
        ui->mainLayout->addWidget(panelUTSetup);
    });
    connect(panelUTSetup, &PanelUTSetup::requestPopup, this, [this]() {
        ui->mainLayout->removeWidget(panelUTSetup);
        setupWindow->layout()->addWidget(panelUTSetup);
        setupWindow->show();
        setupWindow->windowHandle()->alert(0);
    });
    connect(panelUTSetup, &PanelUTSetup::readonlyChanged, this, &PanelAScan::setSetupLocked);
}

void PanelAScan::InitAction()
{
    auto *menu = new QMenu(this);

    menu->addSection(tr("工艺"));
    menu->addAction(ui->actionSetup);
    menu->addAction(tr("DAC"), this, SLOT(OnOpenDAC()));
    menu->addAction(ui->actionSelectProbe);

    menu->addSection(tr("视图"));
    menu->addAction(ui->actionFeature);
    menu->addAction(ui->actionCursor);
    menu->addAction(ui->actionFreeze);
    menu->addAction(ui->actionUseDataSegment);
    menu->addAction(ui->actionFillWaveform);

    menu->addSection(tr("工具"));
    menu->addAction(ui->actionCopyScreenshots);
    menu->addAction(ui->actionScreenshot);
    menu->addAction(ui->actionGateAlarm);
    menu->addAction(ui->actionExportCsv);
    menu->addAction(ui->actionBladeTest);
    menu->addAction(ui->actionBladeSplit);
    menu->addAction(ui->actionBladeSplitSlice);

    ui->toolOptions->setMenu(menu);
}

/*! 将波形导出为csv */
void PanelAScan::ExportWaveform()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出数据文件"), QString(), tr("Lock Config(*.csv)"));
    if (fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if ( !file.open(QIODevice::WriteOnly) ) {
        QMessageBox::warning(this, QString(), tr("文件写入失败"));
        return;
    }
    if (!lastFrame.isEmpty()) {
        file.write(lastFrame.exportCsv(UTDataFrame::AScanDataFrame).toLocal8Bit());
    }
    file.close();
}

void PanelAScan::on_connect_clicked()
{
    if (curChannel && curChannel->instrument()) {
        curChannel->instrument()->Connect();
    }
}

void PanelAScan::on_actionScreenshot_triggered()
{
    QString defaultName(QDateTime::currentDateTime().toString("yyMMdd-hhmmss"));
    QString fileName = QFileDialog::getSaveFileName(this, tr("A扫截图"), defaultName, tr("PNG File(*.png);;JPG File(*.jpg);;BMP File(*.bmp)"));
    if (fileName.isEmpty()) {
        return;
    }
    QPixmap pixmap = RenderPixmap();
    pixmap.save(fileName);
}

void PanelAScan::on_actionCopyScreenshots_triggered()
{
    QPixmap pixmap = RenderPixmap();
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setPixmap(pixmap);
}

void PanelAScan::on_actionSetup_triggered()
{
//    setupWindow->Flash(1);
}

void PanelAScan::on_actionExportCsv_triggered()
{
    if (lastFrame.hasWaveform()) {
        ExportWaveform();
    }
}

void PanelAScan::on_actionFreeze_toggled(bool checked)
{
    ui->view->setBackgroundColor(checked ? QColor(0x47bad5) : Qt::white);
}

void PanelAScan::on_actionGateAlarm_triggered()
{
    QStringList gateNames = {tr("禁用")};
    gateNames << Utils::transform(m_setup.gates(), &UTSetupGate::name);
    bool ok = false;
    QString selected = QInputDialog::getItem(this, QString(), tr("过闸门报警设置"), gateNames, gateIndex + 1, false, &ok);
    if (ok) {
        gateIndex = gateNames.indexOf(selected) - 1;
        ui->alarm->slotAlarmHide();
    }
}

void PanelAScan::on_channelList_currentIndexChanged(int index)
{
    if (index < 0) {
        return;
    }
    setChannelId(ui->channelList->currentData(ChannelModel::ChannelIdRole).toInt());
}

void PanelAScan::on_gainShow_toggled(bool checked)
{
    ui->gainContainer->setVisible(checked);
}

void PanelAScan::on_channels_clicked(const QModelIndex &index)
{
    setChannelId(index.data(ChannelModel::ChannelIdRole).toInt());
}

void PanelAScan::on_gainPlus_clicked()
{
    ui->gain->setValue(ui->gain->value() + 6.0);
}

void PanelAScan::on_gainMinus_clicked()
{
    ui->gain->setValue(ui->gain->value() - 6.0);
}

void PanelAScan::on_gainSlider_valueChanged(int value)
{
    if (m_setup.gain() == double(value) / 10) {
        return;
    }
    handler.set(&UTSetup::setGain, double(value) / 10);
    ui->gain->setValue(m_setup.gain());
}

void PanelAScan::on_gain_valueChanged(double arg1)
{
    if (m_setup.gain() == arg1) {
        return;
    }
    handler.set(&UTSetup::setGain, arg1);
    ui->gainSlider->setValue(m_setup.gain() * 10);
}

void PanelAScan::on_gainRefSlider_valueChanged(int value)
{
    if (m_setup.gainRef() == double(value) / 10) {
        return;
    }
    handler.set(&UTSetup::setGainRef, double(value) / 10);
    ui->gainRef->setValue(m_setup.gainRef());
}

void PanelAScan::on_gainRef_valueChanged(double arg1)
{
    if (m_setup.gainRef() == arg1) {
        return;
    }
    handler.set(&UTSetup::setGainRef, arg1);
    ui->gainRefSlider->setValue(m_setup.gainRef() * 10);
}

void PanelAScan::on_actionSelectProbe_triggered()
{
    DialogProbeList dialog(this);
    dialog.setEditable(false);
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    Probe curProbe = dialog.currentProbe();
    curChannel->setProbe(curProbe);
    ui->view->refreshFocalPos();
}
