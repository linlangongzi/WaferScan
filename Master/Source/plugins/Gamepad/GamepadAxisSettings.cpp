#include "GamepadAxisSettings.h"
#include "ui_GamepadAxisSettings.h"

GamepadAxisSettings::GamepadAxisSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamepadAxisSettings),
    m_handler(this, &GamepadAxisSettings::option, &GamepadAxisSettings::setOption)
{
    ui->setupUi(this);
    ui->control->addItem(tr("无动作"), GamepadAxisOption::NoAction);
    ui->control->addItem(tr("正向移动"), GamepadAxisOption::Forward);
    ui->control->addItem(tr("反向移动"), GamepadAxisOption::Reverse);

    m_handler.addHandler(ui->control, &EnumComboBox::setCurrentValue, &EnumComboBox::currentValueChanged, &GamepadAxisOption::state);
    m_handler.addHandler(ui->axis, &AxisSelector::setCurrentUuid, &AxisSelector::currentAxisUuidChanged, &GamepadAxisOption::axis);

    setProgress(0);
 }

GamepadAxisSettings::~GamepadAxisSettings()
{
    delete ui;
}

GamepadAxisOption GamepadAxisSettings::option() const
{
    return m_option;
}

void GamepadAxisSettings::setOption(const GamepadAxisOption &option)
{
    if (m_option == option) {
        return;
    }
    m_option = option;
    m_handler.setControls(m_option);
    ui->axis->setEnabled(m_option.state == GamepadAxisOption::Forward || m_option.state == GamepadAxisOption::Reverse);
    emit OptionChanged(m_option);
}

void GamepadAxisSettings::setProgress(qreal value)
{
    int progress = ui->statusPositive->maximum() * value;
    ui->statusPositive->setValue(progress);
    ui->statusNegative->setValue(-progress);
}
