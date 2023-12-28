#include "PanelAxisControl.h"
#include "ui_PanelAxisControl.h"
#include "AxisControl.h"
#include "DialogNamedPositionList.h"
#include "DialogSafetyBarrierOptions.h"
#include "MotionConstants.h"
#include "MotionManager.h"
#include "TrioDriver.h"
#include <Core/ICore.h>
#include <Core/LicenseManager.h>
#include <Core/Platform.h>
#include <Core/actionmanager/actioncontainer.h>
#include <Core/actionmanager/actionmanager.h>
#include <Motion/Axis.h>
#include <Motion/EndPoint.h>
#include <Motion/WizardDatum.h>
#include <QDir>
#include <QInputDialog>
#include <QLabel>
#include <QLoggingCategory>
#include <QMenu>
#include <QMessageBox>
#include <QStatusBar>
#include <QtMath>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/Util.h>
#include <utils/algorithm.h>

PanelAxisControl *PanelAxisControl::m_instance = nullptr;
QList<AxisControl *> PanelAxisControl::axisControlList;

using namespace Core;
using namespace Motion;

static Q_LOGGING_CATEGORY(debug, "motion.control");

PanelAxisControl::PanelAxisControl(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PanelAxisControl)
{
    Q_ASSERT(m_instance == nullptr);
    m_instance = this;
    ui->setupUi(this);
    ui->actionHotkeyEnabled->setChecked(false);
    updateCurrentPage();

    ui->bindingChannel->clear();
    for (UTChannel *channel: UltrasonicManager::channels()) {
        ui->bindingChannel->addItem(channel->name(), channel->channelId());
    }

    ui->ptpContainerX->setVisible(false);
    ui->ptpContainerY->setVisible(false);
    ui->ptpContainerZ->setVisible(false);
    ui->ptpContainerB->setVisible(false);
    ui->ptpContainerA->setVisible(false);

    QFont font = ui->coordX->font();
    font.setPixelSize(25);
    for (QLabel *coord: tcpCoords()) {
        coord->setFont(font);
    }

    /* 创建"运动控制"主菜单 */
    ActionContainer *menu = ActionManager::createMenu(G_MOTION);
    menu->setOnAllDisabledBehavior(ActionContainer::Hide);
    menu->menu()->setTitle(tr("运动控制"));
    menu->menu()->setEnabled(LicenseManager::instance()->hasProductLicense());
    ActionContainer *menubar = ActionManager::actionContainer(Core::Constants::MENU_BAR);
    menubar->insertGroup(Core::Constants::G_HELP, G_MOTION);
    menubar->addMenu(menu, G_MOTION);

    Command *cmd = nullptr;

    /* 注册"连接/断开连接"菜单项 */
    menu->appendGroup(G_CONNECTION);
    cmd = ActionManager::registerAction(ui->actionConnect, CONNECT);
    menu->addAction(cmd, G_CONNECTION);
    cmd = ActionManager::registerAction(ui->actionDisconnect, DISCONNECT);
    menu->addAction(cmd, G_CONNECTION);

    menu->addSeparator(G_CONNECTION);

    /* 注册"手工控制"菜单项 */
    menu->appendGroup(G_MANUAL_CONTROL);
    cmd = ActionManager::registerAction(ui->actionSafetyBarrier, SAFETY_BARRIER);
    menu->addAction(cmd, G_MANUAL_CONTROL);
    cmd = ActionManager::registerAction(ui->actionEnforceStep, ENFORCE_STEP);
    menu->addAction(cmd, G_MANUAL_CONTROL);
    cmd = ActionManager::registerAction(ui->actionHotkeyEnabled, HOTKEY_ENABLE);
    menu->addAction(cmd, G_MANUAL_CONTROL);

    menu->addSeparator(G_MANUAL_CONTROL);

    /* 注册"运动模式"菜单项 */
    menu->appendGroup(G_MOTION_MODE);
    cmd = ActionManager::registerAction(ui->actionManualMode, MANUAL_MODE);
    menu->addAction(cmd, G_MOTION_MODE);
    cmd = ActionManager::registerAction(ui->actionRemoteMode, REMOTE_MODE);
    menu->addAction(cmd, G_MOTION_MODE);
//    cmd = ActionManager::registerAction(ui->actionCircularMode, CIRCULAR_MODE);
//    menu->addAction(cmd, G_MOTION_MODE);
    cmd = ActionManager::registerAction(ui->actionTcpMode, TCP_MODE);
    menu->addAction(cmd, G_MOTION_MODE);

    /* 配置"选项"菜单 */
    auto *optionsMenu = new QMenu(ui->options);
    optionsMenu->addAction(ui->actionSafetyBarrier);
    optionsMenu->addAction(ui->actionMotionOptions);
    optionsMenu->addAction(ui->actionEnforceStep);
    optionsMenu->addAction(ui->actionHotkeyEnabled);
    ui->options->setMenu(optionsMenu);

    /* 配置"模式"菜单 */
    auto *modeMenu = new QMenu(ui->mode);
    modeMenu->addAction(ui->actionManualMode);
    modeMenu->addAction(ui->actionRemoteMode);
//    modeMenu->addAction(ui->actionCircularMode);
    modeMenu->addAction(ui->actionTcpMode);
    ui->mode->setMenu(modeMenu);

    /* 向主窗口底部状态条增加"连接/断开"按钮 */
    QStatusBar *bar = Core::ICore::mainWindow()->statusBar();
    statusButton = new QToolButton(bar);
    statusButton->setAutoRaise(true);
    statusButton->setPopupMode(QToolButton::InstantPopup);
    statusButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    bar->insertPermanentWidget(0, statusButton);

    auto *connectionMenu = new QMenu(statusButton);
    connectionMenu->addAction(ui->actionConnect);
    connectionMenu->addAction(ui->actionDisconnect);
    statusButton->setMenu(connectionMenu);

    auto *driver = MotionManager::driver();
    connect(driver, &TrioDriver::statusChanged, this, &PanelAxisControl::refreshConnectionStatus);
    connect(driver, &TrioDriver::modeChanged, this, &PanelAxisControl::refreshConnectionStatus);
    connect(driver, &TrioDriver::globalStatusChanged, this, &PanelAxisControl::onParameterChanged);
    connect(ui->actionConnect, &QAction::triggered, driver, &TrioDriver::connectToController);
    connect(ui->actionDisconnect, &QAction::triggered, driver, &TrioDriver::disconnectFromController);
    connect(ui->connectButton, &QAbstractButton::clicked, driver, &TrioDriver::connectToController);

    connect(MotionManager::instance(), &MotionManager::axisListChanged, this, &PanelAxisControl::refreshAxisList);
    refreshAxisList();
    connect(MotionManager::instance(), &MotionManager::endPointsChanged, this, &PanelAxisControl::refreshEndPointList);
    refreshEndPointList();
}

PanelAxisControl::~PanelAxisControl()
{
    delete ui;
}

PanelAxisControl *PanelAxisControl::instance()
{
    return m_instance;
}

void PanelAxisControl::setPose(const MotionPose &pose, const MotionAxisList &axisList)
{
    for (int index = 0; index < axisControlList.size(); ++index) {
        auto *control = axisControlList[index];
        Axis *axis = control->axis();
        if (!axis) {
            continue;
        }
        int axisId = axis->axisId();
        AxisSetup setup = axis->setup();
        setup.disabled = !axisList.hasAxis(axisId);
        axis->setSetup(setup);
        control->setToPointValue(pose.value(index));
    }
}

QList<AxisControl *> PanelAxisControl::axisControls()
{
    return axisControlList;
}

void PanelAxisControl::startMove()
{
    on_SafeGo_clicked();
}

void PanelAxisControl::refreshTcpPose()
{
    const QList<QLabel *> coords = tcpCoords();
    EndPoint *endPoint = ui->endPoints->currentEndPoint();
    if (endPoint) {
        const int decimalPlaces = MotionManager::displayPrecision();
        for (int index = 0; index < coords.count(); ++index) {
            Axis *axis = endPoint->axis(index);
            if (axis) {
                coords[index]->setText(tr("%1%2").arg(axis->position(), 0, 'f', decimalPlaces).arg(axis->axisUnit()));
            } else {
                coords[index]->setText("SNAN");
            }
        }
    } else {
        for (QLabel *coord: coords) {
            coord->setText("SNAN");
        }
    }
}

void PanelAxisControl::refreshTcpStatus()
{
    QList<QLabel *> labels = {ui->statusX, ui->statusY, ui->statusZ, ui->statusB, ui->statusA};
    EndPoint *endPoint = ui->endPoints->currentEndPoint();
    if (endPoint) {
        for (int index = 0; index < labels.count(); ++index) {
            Axis *axis = endPoint->axis(index);
            if (axis) {
                labels[index]->setText(axis->moveTypeText());
            } else {
                labels[index]->setText("SNAN");
            }
        }
    } else {
        for (QLabel *label: labels) {
            label->setText("SNAN");
        }
    }
}

void PanelAxisControl::axesStatusReconnect()
{
    for (Axis *axis: MotionManager::axisList()) {
        disconnect(axis, &Axis::statusChanged, this, &PanelAxisControl::refreshTcpStatus);
    }

    EndPoint *endPoint = ui->endPoints->currentEndPoint();
    if (!endPoint) {
        return;
    }
    for (Axis *axis: endPoint->axisList()) {
        connect(axis, &Axis::statusChanged, this, &PanelAxisControl::refreshTcpStatus);
    }
}

void PanelAxisControl::onParameterChanged(const QString &key, double newValue)
{
    if (key == "WDOG") {
        wdog = newValue;
        refreshConnectionStatus();
    }
}

void PanelAxisControl::refreshAxisList()
{
    qDeleteAll(axisControlList);
    axisControlList.clear();

    auto axisList = MotionManager::axisList();
    for (int index = 0; index < axisList.count(); ++index) {
        Axis *axis = axisList[index];
        auto *control = new AxisControl(ui->axisListContainer);
        ui->axisListLayout->addWidget(control, 0, index);
        axisControlList.append(control);
        control->setAxis(axis);
        control->setHotKeyEnable(false);
        connect(ui->actionEnforceStep, &QAction::triggered, control, &AxisControl::setEnforceStep);
        connect(ui->actionHotkeyEnabled, &QAction::triggered, control, &AxisControl::setHotKeyEnable);
    }

    ui->importAxis->setVisible(axisControlList.isEmpty());
    refreshConnectionStatus();
}

void PanelAxisControl::refreshEndPointList()
{
    disconnect(ui->stopAll, &QAbstractButton::clicked, nullptr, nullptr);
    EndPoint *endpoint = MotionManager::endPoint(0);
    if (endpoint) {
        connect(ui->stopAll, &QAbstractButton::clicked, endpoint, &EndPoint::Cancel);
    }
}

void PanelAxisControl::refreshConnectionStatus()
{
    ControllerStatus status = MotionManager::driver()->status();
    auto state = status.state;
    bool isConnected = (state == Normal);

    auto mode = MotionManager::driver()->mode();
    QAction *currentMode = nullptr;
    if (mode == TrioDriver::ModeManual) {
        currentMode = ui->actionManualMode;
    } else if (mode == TrioDriver::ModeRemote) {
        currentMode = ui->actionRemoteMode;
//    } else if (mode == TrioDriver::ModeCircular) {
//        currentMode = ui->actionCircularMode;
    } else if (mode == TrioDriver::ModeProgram) {
        currentMode = ui->actionProgramMode;
    } else if (mode == TrioDriver::ModeTcp) {
        currentMode = ui->actionTcpMode;
    }
    ui->mode->setDefaultAction(currentMode);
    ui->mode->setEnabled(currentMode);
    updateCurrentPage();

    bool enabled = axisControlList.isEmpty() || (isConnected && wdog);

    ui->actionConnect->setEnabled(state == Disconnected);
    ui->actionDisconnect->setEnabled(state != Disconnected);

    ui->connectButton->setVisible(!isConnected);
    ui->connectButton->setEnabled(state == Disconnected);
    statusButton->setIcon(isConnected ? ui->actionConnect->icon() : ui->actionDisconnect->icon());
    statusButton->setText(tr("运动控制: %1").arg(status.text()));
    statusButton->setEnabled(LicenseManager::instance()->hasProductLicense());

    if (state == Connecting) {
        ui->connectButton->setText(tr("正在连接..."));
    } else if (state == Disconnected) {
        ui->connectButton->setText(tr("连接"));
    } else {
        ui->connectButton->setText(status.text());
    }
    ui->connectProgress->setRange(0, status.total);
    ui->connectProgress->setValue(status.progress);
    ui->connectProgress->setVisible(state != Normal && state != Disconnected);

    ui->SafeGo->setEnabled(mode == TrioDriver::ModeManual);
    ui->normalPage->setEnabled(enabled && mode != TrioDriver::ModeProgram);
    ui->actionHotkeyEnabled->setEnabled(enabled);
    for (auto *control: axisControlList) {
        control->setEnabled(enabled && (mode == TrioDriver::ModeManual));
    }
}

void PanelAxisControl::on_SafeGo_clicked()
{
    /*! 有效的 AxisControl 列表 */
    QList<AxisControl *> controlList = Utils::filtered(axisControlList, [] (AxisControl *control) {
        return control->axis() && !control->axis()->setup().disabled;
    });
    QList<AxisControl *> needDatum = Utils::filtered(controlList, [] (AxisControl *control) {
        return !control->axis()->status().datumDone && control->axis()->config().datumInput >= 0;
    });
    if (!platform()->Get("MotionDisableDatumStatus").toBool() && !needDatum.isEmpty()) {
        QStringList needDatumNames = Utils::transform(needDatum, [] (AxisControl *control) {
                return control->axis()->name();
        });
        QMessageBox::warning(this, QString(), tr("无法移动: 下列轴未复位 %1").arg(needDatumNames.join(" / ")));
        return;
    }

    QStringList nameList;
    for (AxisControl *control: controlList) {
        if (!qFuzzyCompare(control->axis()->position(), control->ptpDestination())) {
            nameList.append(QString(" %1  %2").arg(control->axis()->name()).arg(control->ptpDestination()));
        }
    }
    if (nameList.isEmpty()) {
        return;
    }
    QMessageBox message(this);
    message.setText(tr("是否移动下列的轴:"));
    message.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    message.setDefaultButton(QMessageBox::Ok);
    message.setIcon(QMessageBox::Question);
    message.setInformativeText(nameList.join("\n"));
    if (message.exec() != QMessageBox::Ok) {
        return;
    }
    QHash<QUuid, qreal> pose;
    for (AxisControl *control: controlList) {
        Axis *axis = control->axis();
        if (axis->isRotation()) {                                               /* 旋转轴不参与联动 */
            axis->SetSpeed(axis->speedLocate());
            axis->MoveAbs(control->ptpDestination());
        } else {                                                                /* 轴号重复的不参与联动 */
            pose[axis->uuid()] = control->ptpDestination();
        }
    }
    EndPoint *endPoint = MotionManager::endPoint(0);
    if (endPoint && !pose.isEmpty()) {
        endPoint->Locate(pose);
    }
}

void PanelAxisControl::on_homeAll_clicked()
{
    WizardDatum wizard(this);
    wizard.exec();
}

/*! 设置安全区的槽函数 */
void PanelAxisControl::on_actionSafetyBarrier_triggered()
{
    SafetyBarrierOptions options = MotionManager::safetyBarrier()->options();
    DialogSafetyBarrierOptions dialog(this);
    dialog.setOptions(options);
    if (dialog.exec() == QDialog::Accepted) {
        MotionManager::safetyBarrier()->setOptions(dialog.options());
    }
}

void PanelAxisControl::on_actionMotionOptions_triggered()
{
    Core::ICore::mainWindow()->showOptionsDialog(Motion::SETTINGS_ID_MOTION);
}

void PanelAxisControl::on_globalPositions_clicked()
{
    DialogNamedPositionList list;

    list.setSelectionMode(false);
    list.exec();
}

void PanelAxisControl::on_importAxis_clicked()
{
    using Core::ICore;
    QDir dir(ICore::resourcePath() + "/AxisProfiles");
    QStringList files = dir.entryList(QStringList(), QDir::Files);
    bool ok = false;
    QString name = QInputDialog::getItem(this, QString(), tr("选择一个默认的轴配置文件"), files, 0, false, &ok);
    if (ok) {
        QJsonValue json = ReadFileAsJson(dir.filePath(name));
        MotionManager::instance()->loadAxisList(json.toArray());
    }
}

void PanelAxisControl::on_actionManualMode_triggered()
{
    MotionManager::driver()->setMode(TrioDriver::ModeManual);
}

void PanelAxisControl::on_actionRemoteMode_triggered()
{
    MotionManager::driver()->setMode(TrioDriver::ModeRemote);
}

void PanelAxisControl::on_actionCircularMode_triggered()
{
    MotionManager::driver()->setMode(TrioDriver::ModeCircular);
}

void PanelAxisControl::on_actionTcpMode_triggered()
{
    MotionManager::driver()->setMode(TrioDriver::ModeTcp);
}

void PanelAxisControl::updateCurrentPage()
{
    auto mode = MotionManager::driver()->mode();
    if (mode == TrioDriver::ModeTcp) {
        ui->stackedWidget->setCurrentIndex(1);
        ui->endPoints->setFilter([&](EndPoint *endPoint){
            return endPoint->type() == EndPoint::Cartesian;
        });
    } else {
        ui->stackedWidget->setCurrentIndex(0);
    }
}

void PanelAxisControl::stopReceiveMonitorData()
{
    if (m_channelPointer) {
        disconnect(m_channelPointer, &UTChannel::MonitorDataArrived, this, &PanelAxisControl::setMonitorData);
        m_channelPointer = nullptr;
    }
}

/*!
 * \brief PanelAxisControl::tcpJogStep
 * \param vectors 三维矢量
 * \param direction 前进、后退
 */
void PanelAxisControl::tcpJogStep(Qt::Axis axis, int direction)
{
    EndPoint *endPoint = ui->endPoints->currentEndPoint();
    if (!endPoint || !endPoint->isIdle() || endPoint->type() != EndPoint::Cartesian) {
        return;
    }

    const MotionPose pos = endPoint->currentPos();
    const QVector3D vector = endPoint->direction(pos, axis).normalized();
    const QVector3D position = endPoint->position(pos) + direction * ui->moveStep->value() * vector;
    const MotionPose destination = endPoint->motionPose(position, vector);

    QHash<QUuid, qreal> pose;
    for (int index = 0; index < 3; ++index) {
        pose[endPoint->axisUuid(index)] = destination.value(index);
    }
    endPoint->MoveAbs(pose);
}

/*!
 * \brief PanelAxisControl::tcpJogDegree
 * \param axis 矩阵旋转所绕的轴
 * \param direction 角度步进的增加或减少标志 1或-1
 */
void PanelAxisControl::tcpJogDegree(Qt::Axis axis, int direction)
{
    EndPoint *endPoint = ui->endPoints->currentEndPoint();
    if (!endPoint || !endPoint->isIdle() || endPoint->type() != EndPoint::Cartesian) {
        return;
    }

    const MotionPose endPointPose = endPoint->currentPos();                             // 机械臂末端 pose
    const QVector3D endPointPosition = endPoint->position(endPointPose);                // 机械臂末端 向量
    const QVector3D vectorZ = endPoint->direction(endPointPose, Qt::ZAxis);             // 探头方向  单位向量
    const qreal offsetLength = ui->probeLength->value() + ui->waterDistance->value();   // 探头长度 + 水距

    const qreal angleB = endPointPose.value(3);
    const qreal angleA = endPointPose.value(4);
    QVector3D offsetVector = endPoint->offsetFromBA(angleB, angleA);
    QVector3D positionProbeOfRoot = endPointPosition + offsetVector;

    const qreal angleOffset = ui->rotateStep->value() * direction;
    const QVector3D focusPosition = positionProbeOfRoot + offsetLength * vectorZ;       // 探头焦点 向量

    QMatrix4x4 matrix;
    matrix.translate(focusPosition);
    QVector3D vectorRotate(0, 1, 0);                                                    // 转B轴时所绕的向量
    qreal angleTargetB = qBound(-180.0, angleB + angleOffset, 180.0);
    qreal angleTargetA = angleA;
    qreal realAngleOffset = angleTargetB - angleB;
    offsetVector = endPoint->offsetFromBA(angleTargetB, angleTargetA);
    if (axis == Qt::XAxis) {
        angleTargetB = angleB;
        angleTargetA = qBound(-90.0, angleA + angleOffset, 90.0);
        realAngleOffset = angleTargetA - angleA;
        vectorRotate = endPoint->direction(endPointPose, axis);                         // 转A轴时所绕的向量
        offsetVector = endPoint->offsetFromBA(angleB, angleTargetA);
    }
    matrix.rotate(realAngleOffset, vectorRotate);
    matrix.translate(-focusPosition);

    const QVector3D rotatedPosition = matrix.map(positionProbeOfRoot);
    QVector3D rotatedDirection = (matrix.map(positionProbeOfRoot + vectorZ) - rotatedPosition).normalized();
    if (!qFuzzyCompare(rotatedDirection.z(), 0)) {
        rotatedDirection = rotatedDirection * QVector3D(1, 1, -1);
    }

    const MotionPose targetPose = endPoint->motionPose(rotatedPosition - offsetVector, angleTargetB, angleTargetA);

    QHash<QUuid, qreal> pose;
    for (int index = 0; index < targetPose.count(); ++index) {
        pose[endPoint->axisUuid(index)] = targetPose.value(index);
    }
    endPoint->MoveAbs(pose);
}

QList<QLabel *> PanelAxisControl::tcpCoords() const
{
    return QList<QLabel *>{ui->coordX, ui->coordY, ui->coordZ, ui->coordB, ui->coordA};
}

void PanelAxisControl::on_bindingChannel_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    stopReceiveMonitorData();

    int channelIndex = ui->bindingChannel->currentData(Qt::UserRole).toInt();
    UTChannel *channel = UltrasonicManager::channel(channelIndex);
    if (!channel) {
        return;
    }

    m_channelPointer = channel;
    connect(channel, &UTChannel::MonitorDataArrived, this, &PanelAxisControl::setMonitorData);
}

void PanelAxisControl::setMonitorData(UTDataFrame frame)
{
    if (!ui->autoWaterDistance->isChecked()) {
        return;
    }

    qreal distance = frame.waterPathLength();
    if (qIsNaN(distance) || qIsInf(distance)) {
        distance = 0;
    }
    ui->waterDistance->setValue(distance);
}

void PanelAxisControl::on_endPoints_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    for (auto *endPointOld: MotionManager::endPoints()) {
        disconnect(endPointOld, &EndPoint::PositionUpdated, this, &PanelAxisControl::refreshTcpPose);
        disconnect(endPointOld, &EndPoint::axisListChanged, this, &PanelAxisControl::axesStatusReconnect);
    }

    EndPoint *endPointNew = ui->endPoints->currentEndPoint();
    if (!endPointNew) {
        return;
    }
    connect(endPointNew, &EndPoint::PositionUpdated, this, &PanelAxisControl::refreshTcpPose);
    connect(endPointNew, &EndPoint::axisListChanged, this, &PanelAxisControl::axesStatusReconnect);
    axesStatusReconnect();
    refreshTcpPose();
}

void PanelAxisControl::on_forwardX_clicked()
{
    tcpJogStep(Qt::XAxis, 1);
}

void PanelAxisControl::on_reverseX_clicked()
{
    tcpJogStep(Qt::XAxis, -1);
}

void PanelAxisControl::on_forwardY_clicked()
{
    tcpJogStep(Qt::YAxis, 1);
}

void PanelAxisControl::on_reverseY_clicked()
{
    tcpJogStep(Qt::YAxis, -1);
}

void PanelAxisControl::on_forwardZ_clicked()
{
    tcpJogStep(Qt::ZAxis, 1);
}

void PanelAxisControl::on_reverseZ_clicked()
{
    tcpJogStep(Qt::ZAxis, -1);
}

void PanelAxisControl::on_forwardB_clicked()
{
    tcpJogDegree(Qt::YAxis, 1);
}

void PanelAxisControl::on_reverseB_clicked()
{
    tcpJogDegree(Qt::YAxis, -1);
}

void PanelAxisControl::on_forwardA_clicked()
{
    tcpJogDegree(Qt::XAxis, 1);
}

void PanelAxisControl::on_reverseA_clicked()
{
    tcpJogDegree(Qt::XAxis, -1);
}

void PanelAxisControl::on_load_clicked()
{
    EndPoint *endPoint = MotionManager::endPoint(0);
    if (endPoint == nullptr) {
        return;
    }
    auto *item =  MotionManager::positionModel()->loadingPosition();
    if (item == nullptr) {
        return;
    }
    endPoint->Locate(item->pos, item->axis);
}

void PanelAxisControl::on_stopAll_pressed()
{
    EndPoint *endPoint = MotionManager::endPoint(0);
    endPoint->Cancel();
}
