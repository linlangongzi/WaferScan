#pragma once

#include "GamepadButtonSettings.h"
#include "GamepadController.h"
#include <QGamepadManager>
#include <QPointer>
#include <QProgressBar>
#include <QWidget>
#include <Core/IOptionsPage.h>

class GamepadAxisSettings;

namespace Ui {
class GamepadSettings;
}

class GamepadSettings : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(GamepadOptions options READ options WRITE setOptions NOTIFY optionsChanged)

public:
    explicit GamepadSettings(QWidget *parent = nullptr);
    ~GamepadSettings() override;

    void refreshGamepadList();
    void onAxisEvent(int deviceId, QGamepadManager::GamepadAxis axis, double value);
    void onButtonPressEvent(int deviceId, QGamepadManager::GamepadButton button, double value);
    void onButtonReleaseEvent(int deviceId, QGamepadManager::GamepadButton button);

    void apply();

    GamepadOptions options() const;

public slots:
    void setOptions(const GamepadOptions &options);

signals:
    void optionsChanged(GamepadOptions options);

private:
    Ui::GamepadSettings *ui;
    QList<GamepadButtonSettings *> m_buttons;
    GamepadOptions m_options;
    QList<GamepadAxisSettings *> m_axes;
};

class GamepadSettingsPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    GamepadSettingsPage(QObject *parent = nullptr);

    QWidget *widget() override;
    void apply() override;
    void finish() override;

    QPointer<GamepadSettings> m_widget;
};
