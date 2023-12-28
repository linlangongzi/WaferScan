#include "GamepadController.h"
#include <Motion/AxisControl.h>
#include <Motion/MotionManager.h>
#include <Motion/PanelAxisControl.h>
#include <Motion/TrioDriver.h>
#include <QLoggingCategory>
#include <allrising/ThrottleUpdate.h>
#include <utils/algorithm.h>

static Q_LOGGING_CATEGORY(debug, "gamepad.control");

static const qint64 DELAY = 50;
static GamepadController *m_instance = nullptr;
static const qreal DEADZONE = 0.2;
REGISTER_JSON_CONVERTER(GamepadOptions)

Q_DECL_UNUSED static const bool buttonOptionsRegistered = GamepadButtonOptions::registerMetaType();
Q_DECL_UNUSED static const bool axisOptionsRegistered = GamepadAxisOption::registerMetaType();

bool GamepadButtonOptions::registerMetaType()
{
    registerJsonConverter<GamepadButtonOptions>();

    MetaType meta(qMetaTypeId<GamepadButtonOptions>());

    MetaType::setTr(NoAction, tr("无动作"));
    MetaType::setTr(Forward, tr("向前移动"));
    MetaType::setTr(Reverse, tr("向后移动"));

    MetaType::registerMeta(meta);

    return true;
}

bool GamepadAxisOption::registerMetaType()
{
    registerJsonConverter<GamepadAxisOption>();
    MetaType meta(qMetaTypeId<GamepadAxisOption>());
    MetaType::setTr(NoAction, tr("无动作"));
    MetaType::setTr(Forward, tr("向前移动"));
    MetaType::setTr(Reverse, tr("向后移动"));
    MetaType::registerMeta(meta);
    return true;
}

static AxisControl *findControl(const GamepadButtonOptions &options)
{
    Axis *axis = MotionManager::axis(options.axis);
    return Utils::findOrDefault(PanelAxisControl::axisControls(), [=] (auto *c) {
        return c->axis() == axis;
    });
}


GamepadController::GamepadController(QObject *parent)
    : QObject(parent)
{
    m_instance = this;

    connect(QGamepadManager::instance(), &QGamepadManager::gamepadAxisEvent, this, &GamepadController::onAxisEvent);
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonPressEvent, this, &GamepadController::onButtonPressEvent);
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonReleaseEvent, this, &GamepadController::onButtonReleaseEvent);
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadDisconnected, this, &GamepadController::onGamepadDisconnected);
}

GamepadController::~GamepadController()
{
    m_instance = nullptr;
}

GamepadController *GamepadController::instance()
{
    return m_instance;
}

GamepadOptions GamepadController::options() const
{
    return m_options;
}

void GamepadController::setOptions(const GamepadOptions &options)
{
    if (m_options == options) {
        return;
    }

    for (auto &option: m_options.buttons) {
        auto *control = findControl(option);
        if (control) {
            control->stop();
        }
    }

    for (const auto &axisOption: m_options.axes) {
        Axis *axis = MotionManager::axis(axisOption.axis);
        if (axis != nullptr) {
            axis->cancelMove();
        }
    }
    m_current.clear();
    m_options = options;
    emit optionsChanged(m_options);
}

void GamepadController::onAxisEvent(int deviceId, QGamepadManager::GamepadAxis axis, double value)
{
    Q_UNUSED(deviceId);
    Motion::TrioDriver::MotionMode currentMode = MotionManager::driver()->mode();
    if (currentMode == Motion::TrioDriver::ModeUninitialized ||
            currentMode == Motion::TrioDriver::ModeManual ||
            currentMode == Motion::TrioDriver::ModeRemote)
    {
        if (qAbs(value) < DEADZONE) {
            value = 0;
        }
        m_axisStates[axis] = value;
        updateDecision();
    }
}

void GamepadController::onButtonPressEvent(int deviceId, QGamepadManager::GamepadButton button, double value)
{
    if (value <= buttonThreshold) {
        return onButtonReleaseEvent(deviceId, button);
    }
    m_buttonState[button] = true;
    updateDecision();
}

void GamepadController::onButtonReleaseEvent(int deviceId, QGamepadManager::GamepadButton button)
{
    Q_UNUSED(deviceId);
    m_buttonState[button] = false;
    updateDecision();
}

void GamepadController::onGamepadDisconnected(int deviceId)
{
    Q_UNUSED(deviceId);
    MotionManager::driver()->cancel();
}

void GamepadController::updateDecision()
{
    bool isAllProtectPressed = true;
    for (int index = 0; index < m_buttonState.count(); ++index) {
        if (m_options.buttons.value(index).action == GamepadButtonOptions::Protect && !m_buttonState[index]) {
            isAllProtectPressed = false;
        }
    }

    QHash<QUuid, double> expectedMove;
    for (int index = 0; index < m_buttonState.count(); ++index) {
        GamepadButtonOptions buttonOptions = m_options.buttons.value(index);
        if (m_buttonState[index]) {
            if (buttonOptions.action == GamepadButtonOptions::Forward && isAllProtectPressed) {
                expectedMove[buttonOptions.axis] = 1;
            }
            if (buttonOptions.action == GamepadButtonOptions::Reverse && isAllProtectPressed) {
                expectedMove[buttonOptions.axis] = -1;
            }
            if (buttonOptions.action == GamepadButtonOptions::Protect) {
                expectedMove[buttonOptions.axis] = 0;
            }
        }
    }

    for (int index = 0; index < m_axisStates.count(); ++index) {
        GamepadAxisOption axesOptions = m_options.axes.value(index);
        if (isAllProtectPressed) {
            if (axesOptions.state == GamepadAxisOption::Forward) {
                expectedMove[axesOptions.axis] = m_axisStates[index];
            } else if (axesOptions.state == GamepadAxisOption::Reverse) {
                expectedMove[axesOptions.axis] = -m_axisStates[index];
            }
        }
    }

    foreach (const auto &axis, expectedMove.keys() + m_current.keys()) {
        Axis *localAxis = MotionManager::axis(axis);
        if (localAxis == nullptr) {
            continue;
        }
        for (int index = 0; index < m_buttonState.count(); ++index) {
            GamepadButtonOptions buttonOptions = m_options.buttons.value(index);
            if (m_buttonState[index] && buttonOptions.action == GamepadButtonOptions::StopAll) {
                expectedMove.clear();
            }
        }
        qreal expect = expectedMove[axis];
        qreal current = m_current[axis];
        if (qFuzzyCompare(expect, current)) {
            continue;
        }

        // Currently moving, but expected to stop or move to another direction. Stop first.
        if (expect * current  < 0 || qFuzzyCompare(expect, 0)) {
            qCDebug(debug) << "Cancel" << localAxis->name();
            localAxis->cancelMove();
        }

        if (!qFuzzyIsNull(expect)) {
            if (m_timer.elapsed() > DELAY || !m_timer.isValid()) {
                // Axis must be stopped by now. Move it.
                if (expect > 0 && current <= 0) {
                    qCDebug(debug) << "Forward" << localAxis->name();
                    localAxis->MoveForward();
                }
                if (expect < 0 && current >= 0) {
                    qCDebug(debug) << "Reverse" << localAxis->name();
                    localAxis->MoveReverse();
                }
                localAxis->SetSpeed(localAxis->speedJog() * qAbs(expect));
                m_timer.start();
            } else {
                ThrottleUpdate::call(this, &GamepadController::updateDecision);
                return;
            }
        }
        m_current[axis] = expectedMove[axis];
    }
}
