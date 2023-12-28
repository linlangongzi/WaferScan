#pragma once

#include <QObject>
#include <QList>
#include <QUuid>
#include <allrising/Util.h>
#include <QGamepadManager>
#include <QCoreApplication>
#include <QVector>
#include <QElapsedTimer>

class GamepadButtonOptions
{
    Q_GADGET
    QT_TR_FUNCTIONS

    Q_PROPERTY(QUuid axis MEMBER axis)
    Q_PROPERTY(State action MEMBER action)

public:
    enum State {
        NoAction,
        Forward,
        Reverse,
        Protect,
        StopAll,
    };
    Q_ENUM(State)

    DECLARE_COMPARE_OPERATORS(GamepadButtonOptions)

    static bool registerMetaType();

    QUuid axis;
    State action = NoAction;
};

class GamepadAxisOption
{
    Q_GADGET
    QT_TR_FUNCTIONS

    Q_PROPERTY(State state MEMBER state)
    Q_PROPERTY(QUuid axis MEMBER axis)

public:
    enum State {
        NoAction,
        Forward,
        Reverse,
    };
    Q_ENUM(State)

    DECLARE_COMPARE_OPERATORS(GamepadAxisOption)
    static bool registerMetaType();

    QUuid axis;
    State state = NoAction;
};

class GamepadOptions
{
    Q_GADGET

    Q_PROPERTY(int deviceId MEMBER deviceId)
    Q_PROPERTY(QList<GamepadAxisOption> axes MEMBER axes)
    Q_PROPERTY(QList<GamepadButtonOptions> buttons MEMBER buttons)

public:
    DECLARE_COMPARE_OPERATORS(GamepadOptions)

    int deviceId = 0;
    QList<GamepadButtonOptions> buttons;
    QList<GamepadAxisOption> axes;
};

class GamepadController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(GamepadOptions options READ options WRITE setOptions NOTIFY optionsChanged)

public:
    enum State {
        NoAction,
        Forward,
        Reverse,
        Stop,
    };

    explicit GamepadController(QObject *parent = nullptr);
    ~GamepadController() override;

    static GamepadController *instance();
    static constexpr qreal buttonThreshold = 0.5;
    GamepadOptions options() const;

signals:
    void optionsChanged(GamepadOptions options);
    void buttonStateChanged(QVector<GamepadButtonOptions::State> buttonState);

public slots:
    void setOptions(const GamepadOptions &options);

private:
    void onAxisEvent(int deviceId, QGamepadManager::GamepadAxis axis, double value);
    void onButtonPressEvent(int deviceId, QGamepadManager::GamepadButton button, double value);
    void onButtonReleaseEvent(int deviceId, QGamepadManager::GamepadButton button);
    void onGamepadDisconnected(int deviceId);
    void updateDecision();

private:
    GamepadOptions m_options;
    QVector<qreal> m_axisStates = {0, 0, 0, 0};
    QVector<bool> m_buttonState = QVector<bool>(18, false);
    QHash<QUuid, qreal> m_current;
    QElapsedTimer m_timer;
};
