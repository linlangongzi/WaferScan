#include "GamepadButtonSettings.h"
#include "ui_GamepadButtonSettings.h"

GamepadButtonSettings::GamepadButtonSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamepadButtonSettings),
    m_handler(this, &GamepadButtonSettings::option, &GamepadButtonSettings::setOption)
{
    ui->setupUi(this);

    ui->action->addItem(tr("无动作"), GamepadButtonOptions::NoAction);
    ui->action->addItem(tr("正向移动"), GamepadButtonOptions::Forward);
    ui->action->addItem(tr("反向移动"), GamepadButtonOptions::Reverse);
    ui->action->addItem(tr("保护"), GamepadButtonOptions::Protect);
    ui->action->addItem(tr("全部停止"), GamepadButtonOptions::StopAll);

    m_handler.addHandler(ui->action, &EnumComboBox::setCurrentValue, &EnumComboBox::currentValueChanged, &GamepadButtonOptions::action);
    m_handler.addHandler(ui->axis, &AxisSelector::setCurrentUuid, &AxisSelector::currentAxisUuidChanged, &GamepadButtonOptions::axis);
}

GamepadButtonSettings::~GamepadButtonSettings()
{
    delete ui;
}

GamepadButtonOptions GamepadButtonSettings::option() const
{
    return m_option;
}

bool GamepadButtonSettings::isPressed() const
{
    return m_pressed;
}

void GamepadButtonSettings::setOption(const GamepadButtonOptions &option)
{
    if (m_option == option) {
        return;
    }

    m_option = option;

    m_handler.setControls(option);
    ui->axis->setEnabled(option.action == GamepadButtonOptions::Forward || option.action == GamepadButtonOptions::Reverse);

    emit optionChanged(m_option);
}

void GamepadButtonSettings::setPressed(bool pressed)
{
    m_pressed = pressed;
    if (pressed) {
        ui->status->setPixmap(tr(":/fatcow/16x16/bullet_red.png"));
    } else {
        ui->status->setPixmap(tr(":/fatcow/16x16/bullet_green.png"));
    }
}
