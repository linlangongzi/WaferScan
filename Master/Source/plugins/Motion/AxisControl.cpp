#include "AxisControl.h"
#include "ui_AxisControl.h"
#include "Axis.h"
#include "MotionConstants.h"
#include "MotionManager.h"
#include "TrioDriver.h"
#include <Core/ICore.h>
#include <Core/Platform.h>
#include <QContextMenuEvent>
#include <QLoggingCategory>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#include <allrising/HotkeyListener.h>
#include <allrising/Util.h>

static const int KEY_HOLD_TIME = 200;                                           /* 按键被识别为连续移动时，需要按住的时间 */
static const qreal TIME_THRESHOLD = 0.5;

static Q_LOGGING_CATEGORY(debug, "motion.axis.control", QtWarningMsg);

AxisControl::AxisControl(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AxisControl),
    m_hotkeyForward(new HotkeyListener(this)),
    m_hotkeyReverse(new HotkeyListener(this))
{
    ui->setupUi(this);

    connect(m_hotkeyForward, &HotkeyListener::keyPressed, this, &AxisControl::on_moveUp_pressed);
    connect(m_hotkeyForward, &HotkeyListener::keyReleased, this, &AxisControl::on_moveUp_released);
    connect(m_hotkeyReverse, &HotkeyListener::keyPressed, this, &AxisControl::on_moveDown_pressed);
    connect(m_hotkeyReverse, &HotkeyListener::keyReleased, this, &AxisControl::on_moveDown_released);

    m_keyStatus[0] = 0;
    m_keyStatus[1] = 0;

    startTimer(50);
}

AxisControl::~AxisControl()
{
    delete ui;
}

double AxisControl::ptpDestination()
{
    return ui->ptp->value();
}

void AxisControl::stop()
{
    updateButtonStatus(Forward, false);
    updateButtonStatus(Reverse, false);
}

void AxisControl::setDisabled(bool enabled)
{
    ui->axis->setChecked(enabled);
    updateDisableStatus();
}

bool AxisControl::isDisabled() const
{
    return ui->axis->isChecked();
}

void AxisControl::setToPointValue(double value)
{
    ui->ptp->setValue(value);
}

Axis *AxisControl::axis() const
{
    return m_axis;
}

void AxisControl::setEnforceStep(bool enabled)
{
    m_forceStep = enabled;
}

void AxisControl::setHotKeyEnable(bool arg)
{
    m_hotkeyForward->setEnabled(arg);
    m_hotkeyReverse->setEnabled(arg);
    stop();
}

void AxisControl::jogStep(int direction)
{
    if (!m_axis || m_axis->status().moveType != AxisStatus::Idle) {
        return;
    }
    qreal step = m_axis->setup().jogStep;
    qreal speed = ui->speed->value();

    if ((qAbs(step / speed)) > TIME_THRESHOLD) {
        AxisConfig config = m_axis->config();
        qreal destination = m_axis->position() + direction * step;
        const int precision = MotionManager::displayPrecision();

        QMessageBox message(this);
        message.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        message.setText(tr("确定将 %1 移动至 %2%3?").arg(m_axis->name()).arg(destination, 0, 'f', precision).arg(m_axis->axisUnit()));
        message.setIcon(QMessageBox::Question);

        bool bypassDatum = platform()->Get("MotionDisableDatumStatus").toBool();
        if (!m_axis->status().datumDone && !bypassDatum) {
            message.setInformativeText(tr("%1 未复位, 请谨慎操作").arg(m_axis->name()));
            message.setIcon(QMessageBox::Warning);
        } else if (!Range(config.limitReverse, config.limitForward).isInside(destination)) {
            message.setInformativeText(tr("目标位置超出限位 (%1 - %2), 请谨慎操作")
                                       .arg(config.limitReverse, 0, 'f', precision)
                                       .arg(config.limitForward, 0, 'f', precision));
            message.setIcon(QMessageBox::Warning);
        }

        if (message.exec() != QMessageBox::Ok) {
            return;
        }
    }

    m_axis->SetSpeed(m_axis->speedJog());
    m_axis->MoveRel(step * direction);
}

void AxisControl::cancelMove()
{
    if (m_axis) {
        m_axis->cancelMove();
    }
}

void AxisControl::continuousForward()
{
    if (m_axis) {
        m_axis->cancelMove();
        m_axis->SetSpeed(m_axis->speedJog());
        m_axis->MoveForward();
    }
}

void AxisControl::continuousReverse()
{
    if (!m_axis) {
        return;
    }
    m_axis->cancelMove();
    m_axis->SetSpeed(m_axis->speedJog());
    m_axis->MoveReverse();
}

void AxisControl::on_moveUp_pressed()
{
    if ( ui->moveUp->isEnabled() ) {                                            // 此处判断是针对键盘快捷键
        updateButtonStatus(Forward, true);
    }
}

void AxisControl::on_moveDown_pressed()
{
    if ( ui->moveDown->isEnabled() ) {
        updateButtonStatus(Reverse, true);
    }
}

void AxisControl::on_moveUp_released()
{
    if ( ui->moveUp->isEnabled() ) {                                            // 此处判断是针对键盘快捷键
        updateButtonStatus(Forward, false);
    }
}

void AxisControl::on_moveDown_released()
{
    if ( ui->moveDown->isEnabled() ) {
        updateButtonStatus(Reverse, false);
    }
}

void AxisControl::updateButtonStatus(JogDirection direction, bool pressed)                // direction 0:forward  1:back
{
    m_keyStatus[direction] = pressed;
    int status = m_keyStatus[Forward] - m_keyStatus[Reverse];
    qCDebug(debug) << "Update button status" << m_keyStatus[Forward] << m_keyStatus[Reverse] << status;
    setExpectedDirection(status);
}

void AxisControl::setExpectedDirection(int direction)
{
    if (m_expectedDirection == direction) {
        return;
    }
    qCDebug(debug) << "Expected dir" << m_expectedDirection << "->" << direction << "(current=" << m_currentDirection << ")";
    if (direction != 0) {   // 转到"按下"
        if (m_forceStep) {
            jogStep(direction);
        } else {
            m_keyDownTime.start();
        }
    } else {    // 转到"弹起"
        bool directionMatch = (m_currentDirection == m_expectedDirection);
        m_currentDirection = 0;
        m_keyDownTime.invalidate();
        if (!directionMatch && !m_forceStep) {
            jogStep(m_expectedDirection);
        } else {
            cancelMove();
        }
    }
    m_expectedDirection = direction;
}

void AxisControl::updateDisableStatus()
{
    bool disabled = ui->axis->isChecked();
    ui->moveUp->setDisabled(disabled || m_status.forwardLimited());
    ui->moveDown->setDisabled(disabled || m_status.reverseLimited());

    ui->stepContainer->setDisabled(disabled);
    ui->status->setDisabled(disabled);
}

void AxisControl::openSettings()
{
    Core::ICore::mainWindow()->showOptionsDialog(Motion::SETTINGS_ID_MOTION);
}

/*! 右键菜单事件 */
void AxisControl::setAxis(Axis *axis)
{
    if (axis == m_axis) {
        return;
    }
    unsetAxis();
    m_axis = axis;
    if (axis) {
        connect(axis, &Axis::configChanged, this, &AxisControl::setAxisConfig);
        connect(axis, &Axis::setupChanged, this, &AxisControl::setAxisSetup);
        connect(axis, &Axis::statusChanged, this, &AxisControl::setAxisStatus);
        setAxisConfig(axis->config());
        setAxisSetup(axis->setup());
        setAxisStatus(axis->status());

        const auto unit = axis->axisUnit();
        ui->suffix->setText(unit);
        ui->step->setSuffix(unit);
        ui->ptp->setSuffix(unit);
        ui->speed->setSuffix(axis->axisUnit(Axis::UNIT_SPEED));
    }
}

void AxisControl::unsetAxis()
{
    if (m_axis) {
        disconnect(m_axis, nullptr, this, nullptr);
    }
    m_axis = nullptr;
}

/*! 右键菜单事件 */
void AxisControl::contextMenuEvent(QContextMenuEvent *event)
{
    cancelMove();
    QMenu menu(this);
    menu.addAction(tr("轴参数设置"), this, &AxisControl::openSettings);
    menu.exec(event->globalPos());
}

void AxisControl::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    if (m_forceStep) {
        return;
    }
    if (m_expectedDirection && m_expectedDirection != m_currentDirection && m_keyDownTime.isValid() && m_keyDownTime.hasExpired(KEY_HOLD_TIME)) {
        if (m_expectedDirection >= 1) {
            continuousForward();
        } else if (m_expectedDirection <= -1) {
            continuousReverse();
        }
        m_currentDirection = m_expectedDirection;
    }
}

void AxisControl::setAxisConfig(const AxisConfig &config)
{
    ui->axis->setText(config.name);
    ui->speed->setMaximum(config.speedMax);
}

void AxisControl::setAxisSetup(const AxisSetup &setup)
{
    ui->speed->setValue(setup.speedJog);
    ui->step->setValue(setup.jogStep);
    m_hotkeyForward->setKey(setup.forwardKey);
    m_hotkeyReverse->setKey(setup.reverseKey);
    setDisabled(setup.disabled);
}

void AxisControl::setAxisStatus(const AxisStatus &status)
{
    const int displayPrecision = MotionManager::displayPrecision();
    ui->status->setText(EnumToKey(status.moveType));
    ui->coord->setText(QString::number(status.position, 'f', displayPrecision));

    bool datumDone = platform()->Get("MotionDisableDatumStatus").toBool() || status.datumDone;
    if (m_axis && m_axis->config().datumInput < 0) {
        datumDone = true;
    }
    ui->loadPos->setVisible(datumDone);
    ui->ptp->setVisible(datumDone);
    ui->needDatum->setVisible(!datumDone);
    m_status = status;
    updateDisableStatus();
    if (status.forwardLimited()) {
        updateButtonStatus(Forward, false);
    }
    if (status.reverseLimited()) {
        updateButtonStatus(Reverse, false);
    }

    setToolTip(tr("轴类型: %1\n位置: %2\n当前运动: %3\n已复位: %4\n正限位传感器: %5\n负限位传感器: %6\n正逻辑限位: %7\n负逻辑限位: %8")
        .arg(status.axisType)
        .arg(status.position, 0, 'f', displayPrecision)
        .arg(status.moveType)
        .arg(status.datumDone)
        .arg(status.limitedForwardInput)
        .arg(status.limitedReverseInput)
        .arg(status.limitedForward)
        .arg(status.limitedReverse)
    );
}

void AxisControl::on_loadPos_clicked()
{
    if (!m_axis) {
        return;
    }
    double dest = ui->ptp->value();
    if (qFuzzyCompare(dest, m_axis->status().position)) {
        return;
    }

    AxisConfig config = m_axis->config();
    const int displayPrecision = MotionManager::displayPrecision();
    QMessageBox message(this);
    message.setText(tr("确定要将 %1 轴移动到 %2%3 吗?").arg(m_axis->name()).arg(dest, 0, 'f', displayPrecision).arg(ui->suffix->text()));
    message.setIcon(QMessageBox::Question);
    message.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    if (qBound(config.limitReverse, dest, config.limitForward) != dest) {
        message.setIcon(QMessageBox::Warning);
        message.setInformativeText(tr("目标位置超出限位 (%1 - %2), 请谨慎操作")
                                   .arg(config.limitReverse, 0, 'f', displayPrecision)
                                   .arg(config.limitForward, 0, 'f', displayPrecision));
    }
    if (message.exec() == QMessageBox::Ok) {
        m_axis->SetSpeed(m_axis->speedLocate());
        m_axis->MoveAbs(dest);
    }
}

void AxisControl::on_step_editingFinished()
{
    if (m_axis) {
        AxisSetup setup = m_axis->setup();
        setup.jogStep = ui->step->value();
        m_axis->setSetup(setup);
    }
}

void AxisControl::on_speed_editingFinished()
{
    if (m_axis) {
        AxisSetup setup = m_axis->setup();
        setup.speedJog = ui->speed->value();
        m_axis->setSetup(setup);
    }
}

void AxisControl::on_axis_toggled(bool checked)
{
    if (m_axis) {
        AxisSetup setup = m_axis->setup();
        setup.disabled = checked;
        m_axis->setSetup(setup);
    }
}
