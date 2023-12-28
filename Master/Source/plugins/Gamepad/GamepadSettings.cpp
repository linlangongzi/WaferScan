#include "GamepadSettings.h"
#include "ui_GamepadSettings.h"
#include "GamepadAxisSettings.h"
#include "GamepadButtonSettings.h"
#include "GamepadConstants.h"
#include <QGamepad>
#include <utils/algorithm.h>

GamepadSettings::GamepadSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamepadSettings)
{
    ui->setupUi(this);
    ui->groupTBD->hide();
    QList<QPair<QGamepadManager::GamepadButton, QString>> buttons = {
        {QGamepadManager::ButtonA,       tr("A")},
        {QGamepadManager::ButtonB,       tr("B")},
        {QGamepadManager::ButtonX,       tr("X")},
        {QGamepadManager::ButtonY,       tr("Y")},
        {QGamepadManager::ButtonL1,      tr("LB")},
        {QGamepadManager::ButtonR1,      tr("RB")},
        {QGamepadManager::ButtonL2,      tr("LT")},
        {QGamepadManager::ButtonR2,      tr("RT")},
        {QGamepadManager::ButtonSelect,  tr("Select")},
        {QGamepadManager::ButtonStart,   tr("Start")},
        {QGamepadManager::ButtonL3,      tr("LAxis")},
        {QGamepadManager::ButtonR3,      tr("RAxis")},
        {QGamepadManager::ButtonUp,      tr("Up")},
        {QGamepadManager::ButtonDown,    tr("Down")},
        {QGamepadManager::ButtonRight,   tr("Right")},
        {QGamepadManager::ButtonLeft,    tr("Left")},
        {QGamepadManager::ButtonCenter,  tr("Center")},
        {QGamepadManager::ButtonGuide,   tr("Guide")}
    };
    QList<QPair<QGamepadManager::GamepadAxis, QString>> axes = {
        {QGamepadManager::AxisLeftX,    tr("LX")},
        {QGamepadManager::AxisLeftY,    tr("LY")},
        {QGamepadManager::AxisRightX,   tr("RX")},
        {QGamepadManager::AxisRightY,   tr("RY")}
    };

    for (const auto &axis: axes) {
        auto *setting = new GamepadAxisSettings(ui->groupAxes);
        ui->axesLayout->addRow(axis.second, setting);
        m_axes << setting;
    }

    for (const auto &button: buttons) {
        auto *editor = new GamepadButtonSettings(ui->groupButtons);
        ui->buttonsLayout->addRow(button.second, editor);
        m_buttons << editor;
    }

    connect(QGamepadManager::instance(), &QGamepadManager::gamepadAxisEvent, this, &GamepadSettings::onAxisEvent);
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonPressEvent, this, &GamepadSettings::onButtonPressEvent);
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonReleaseEvent, this, &GamepadSettings::onButtonReleaseEvent);
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadConnected, this, &GamepadSettings::refreshGamepadList);
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadDisconnected, this, &GamepadSettings::refreshGamepadList);
    refreshGamepadList();

    setOptions(GamepadController::instance()->options());
}

GamepadSettings::~GamepadSettings()
{
    delete ui;
}

void GamepadSettings::refreshGamepadList()
{
    QStringList names = Utils::transform(QGamepadManager::instance()->connectedGamepads(), [] (int id) {
        return tr("手柄 %1 %2").arg(id).arg(QGamepad(id).name());
    });
    ui->gamepad->clear();
    ui->gamepad->addItems(names);
}

void GamepadSettings::onAxisEvent(int deviceId, QGamepadManager::GamepadAxis axis, double value)
{
    Q_UNUSED(deviceId);

    GamepadAxisSettings *editor = m_axes.value(axis);
    if (editor != nullptr) {
        editor->setProgress(value);
    }
}

void GamepadSettings::onButtonPressEvent(int deviceId, QGamepadManager::GamepadButton button, double value)
{
    Q_UNUSED(deviceId);
    auto *editor = m_buttons.value(button);
    if (editor) {
        editor->setPressed(value > GamepadController::buttonThreshold);
    }
}

void GamepadSettings::onButtonReleaseEvent(int deviceId, QGamepadManager::GamepadButton button)
{
    Q_UNUSED(deviceId);
    auto *editor = m_buttons.value(button);
    if (editor) {
        editor->setPressed(false);
    }
}

void GamepadSettings::apply()
{
    GamepadController::instance()->setOptions(options());
}

GamepadOptions GamepadSettings::options() const
{
    auto options = m_options;
    options.buttons = Utils::transform(m_buttons, &GamepadButtonSettings::option);
    options.axes = Utils::transform(m_axes, &GamepadAxisSettings::option);
    return options;
}

void GamepadSettings::setOptions(const GamepadOptions &options)
{
    if (m_options == options) {
        return;
    }

    m_options = options;

    for (int index = 0; index < m_buttons.count(); ++index) {
        auto *editor = m_buttons[index];
        if (editor) {
            editor->setOption(options.buttons.value(index));
        }
    }

    for (int index = 0; index < m_axes.count(); ++index) {
        GamepadAxisSettings *edit = m_axes[index];
        if (edit != nullptr) {
            edit->setOption(m_options.axes.value(index));
        }
    }
    emit optionsChanged(m_options);
}

////////////////////////////////////////////////////////////////////////////////

GamepadSettingsPage::GamepadSettingsPage(QObject *parent) :
    IOptionsPage(parent)
{
    using namespace Gamepad;

    setId(SETTINGS_ID_GAMEPAD_BUTTONS);
    setDisplayName(tr("按钮"));
    setCategory(SETTINGS_CATEGORY_GAMEPAD);
    setDisplayCategory(tr("手柄控制"));
    setCategoryIcon(QIcon(SETTINGS_ICON_GAMEPAD));
}

QWidget *GamepadSettingsPage::widget()
{
    if (!m_widget) {
        m_widget = new GamepadSettings;
    }
    return m_widget;
}

void GamepadSettingsPage::apply()
{
    if (m_widget) {
        m_widget->apply();
    }
}

void GamepadSettingsPage::finish()
{
    delete m_widget;
}
