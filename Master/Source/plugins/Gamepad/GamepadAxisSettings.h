#pragma once

#include <QWidget>
#include <Gamepad/GamepadController.h>
#include <allrising/ConnectionHelper.h>

class GamepadAxisOption;

namespace Ui {
class GamepadAxisSettings;
}

class GamepadAxisSettings : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(GamepadAxisOption option READ option WRITE setOption NOTIFY OptionChanged)

public:
    explicit GamepadAxisSettings(QWidget *parent = 0);
    ~GamepadAxisSettings();

    GamepadAxisOption option() const;

public slots:
    void setOption(const GamepadAxisOption &option);
    void setProgress(qreal value);

signals:
    void OptionChanged(GamepadAxisOption option);

private:
    Ui::GamepadAxisSettings *ui;
    GamepadAxisOption m_option;
    ConnectionHelper<GamepadAxisSettings, GamepadAxisOption> m_handler;
};
