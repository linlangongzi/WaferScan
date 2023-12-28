#pragma once

#include <QWidget>
#include "GamepadController.h"
#include <allrising/ConnectionHelper.h>

namespace Ui {
class GamepadButtonSettings;
}

class GamepadButtonSettings : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(GamepadButtonOptions option READ option WRITE setOption NOTIFY optionChanged)
    Q_PROPERTY(bool pressed READ isPressed WRITE setPressed)

public:
    explicit GamepadButtonSettings(QWidget *parent = nullptr);
    ~GamepadButtonSettings();

    GamepadButtonOptions option() const;

    bool isPressed() const;

public slots:
    void setOption(const GamepadButtonOptions &option);
    void setPressed(bool pressed);

signals:
    void optionChanged(GamepadButtonOptions option);

private:
    Ui::GamepadButtonSettings *ui;
    GamepadButtonOptions m_option;
    bool m_pressed = false;
    ConnectionHelper<GamepadButtonSettings, GamepadButtonOptions> m_handler;
};
