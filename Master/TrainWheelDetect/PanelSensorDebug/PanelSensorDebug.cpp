#include "PanelSensorDebug.h"
#include "ui_PanelSensorDebug.h"
#include <arsCore/Platform.h>
#include <Wheel/ControlPanel.h>
#include <Core/ICore.h>
#include <WheelDataset/WheelDataset.h>
#include <Ultrasonic/UTInstrument.h>
#include <ProbeNoManager/ProbeNoManager.h>
#include <PanelSensorStatus/PanelSensorStatus.h>

PanelSensorDebug::PanelSensorDebug(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelSensorDebug),
    m_type(STOP),
    m_proTime(clock()),
    m_index(0)
{
    ui->setupUi(this);
    p_action = UTSharedDataSimulation::create();
    m_timer = new QTimer(this);
    InitUI();
    Connect();
    ChangeEnable(0);
    m_timer->start(50);
    m_timeSpace = ui->doubleSpinBox->value() * 1000;
    int sensorMinIdx = ProbeNoManager::instance()->GetSensorMinIdx();
    int sensorMaxIdx = ProbeNoManager::instance()->GetSensorMaxIdx();
    ui->spinBoxSensorStart->setMinimum(sensorMinIdx);
    ui->spinBoxSensorStart->setMaximum(sensorMaxIdx);
    ui->spinBoxSensorEnd->setMinimum(sensorMinIdx);
    ui->spinBoxSensorEnd->setMaximum(sensorMaxIdx);
    m_sensorStart = ui->spinBoxSensorStart->value();
    m_sensorEnd = ui->spinBoxSensorEnd->value();
}

PanelSensorDebug::~PanelSensorDebug()
{
    delete ui;
}

void PanelSensorDebug::ActionChanged(int index, bool active)
{
    int integer = index / 16;
    int remainder = index % 16;
    qint16 data = p_action.data()->sLightEye[integer];

    int integerBefore = integer;
    int remainderBefore = remainder;
    qint16 dataBefore = data;
    if (index > 0 && index < m_radioButtonList.size()) {
        if (index % 16 == 0) {

        }
        int indexBefore = index - 1;
        integerBefore = indexBefore / 16;
        remainderBefore = indexBefore % 16;
        dataBefore = p_action.data()->sLightEye[integerBefore];
    }

    if (active) {
        data |= (1 << remainder);
        dataBefore |= (1 << remainderBefore);
        if (integer == integerBefore) {
            p_action.data()->sLightEye[integer] = data | dataBefore;
        } else {
            p_action.data()->sLightEye[integer] = data;
            p_action.data()->sLightEye[integerBefore] = dataBefore;
        }
    } else {
        data &= !(1 << remainder);
        dataBefore &= !(1 << remainderBefore);
        if (integer == integerBefore) {
            p_action.data()->sLightEye[integer] = data & dataBefore;
        } else {
            p_action.data()->sLightEye[integer] = data;
            p_action.data()->sLightEye[integerBefore] = dataBefore;
        }
    }

    for (auto instrument: platform()->Instruments()->GetRegistry<UTInstrument>()) {
        if (active) {
            instrument->sendRawMessage(IInstrumentDriver::MessageTrainSensor, QVariant::fromValue(p_action));
        }
    }
}

void PanelSensorDebug::ChannelChanged(qint32 channelID, qint16 surOrFlaw)
{
    Q_UNUSED(surOrFlaw);
    ProbeNoManager *manager = ProbeNoManager::instance();
    if(manager->IsProbeSensor(channelID)) {
       return;
    }

    if (ui->debugSwitch->isChecked() && ui->radioButtonHand->isChecked()) {
        qint32 eyeIndex = ProbeNoManager::instance()->ProbeNo2EyeArea(channelID);
        Clear();

        if (eyeIndex >= m_radioButtonList.count()) {
            qDebug() << "PanelSensorDebug::ChannelChanged(): eyeIndex over range";
            return;
        }

        if (eyeIndex < 0) {
            return;
        }

        m_radioButtonList.value(eyeIndex)->setChecked(true);
    }
}

void PanelSensorDebug::OnSelfTestConfirm(int sensorNo)
{
    if (ui->debugSwitch->isChecked() && ui->radioButtonHand->isChecked()) {
        Clear();

        if (sensorNo >= m_radioButtonList.count() || sensorNo < 0) {
            qDebug() << "PanelSensorDebug::ChannelChanged(): sensorNo over range " << sensorNo;
            return;
        }

        m_radioButtonList.value(sensorNo)->setChecked(true);
    }
}

/*!
 * \brief PanelSensorDebug::SelfTestStateChanged
 * \param selfTest
 * 根据自检模块的开启状态切换调试状态
 * 特别提醒：只要有自检模块状态变化，传感器调试都切换为关闭状态
 */
void PanelSensorDebug::SelfTestStateChanged(bool selfTest)
{
    if (selfTest) {
        bool isEnable = !selfTest;
        ui->debugSwitch->setChecked(isEnable);
    }
}

/*!
 * \brief PanelSensorDebug::ConnectionChanged
 * \param isConnect
 * 仪器连接成功后，将目前的调试状态下发到下位机使同步
 */
void PanelSensorDebug::ConnectionChanged()
{
    for (auto instrument: platform()->Instruments()->GetRegistry<UTInstrument>()) {
        instrument->sendRawMessage(IInstrumentDriver::MessageTrainSensor, QVariant::fromValue(p_action));
    }
}

void PanelSensorDebug::ChangeEnable(bool checked)
{
    int count = m_radioButtonList.count();
    p_action.data()->sEnable = checked;
    SetSensorSwitch(checked);

    if (checked)
    {
        ui->radioButtonHand->setEnabled(true);
        ui->radioButtonSelf->setEnabled(true);

        if (ui->radioButtonHand->isChecked())
        {
            for (int index = 0; index < count; ++index)
            {
                m_radioButtonList.value(index)->setEnabled(true);
            }
        }
        else if (ui->radioButtonSelf->isChecked())
        {
            ui->pushButtonFront->setEnabled(true);
            ui->pushButtonBack->setEnabled(true);
            ui->pushButtonOver->setEnabled(true);
            ui->doubleSpinBox->setEnabled(true);
        }
    }
    else
    {
        for (int index = 0; index < count; ++index)
        {
            m_radioButtonList.value(index)->setEnabled(false);
        }

        ui->radioButtonHand->setEnabled(false);
        ui->radioButtonSelf->setEnabled(false);
        ui->pushButtonFront->setEnabled(false);
        ui->pushButtonBack->setEnabled(false);
        ui->pushButtonOver->setEnabled(false);
        ui->doubleSpinBox->setEnabled(false);
    }
}

void PanelSensorDebug::ChangeModle()
{
    int count = m_radioButtonList.count();

    if (ui->radioButtonHand->isChecked())
    {
        ui->pushButtonFront->setEnabled(false);
        ui->pushButtonBack->setEnabled(false);
        ui->pushButtonOver->setEnabled(false);
        ui->doubleSpinBox->setEnabled(false);

        for (int index = 0; index < count; ++index)
        {
            m_radioButtonList.value(index)->setEnabled(true);
        }
    }
    else
    {
        ui->pushButtonFront->setEnabled(true);
        ui->pushButtonBack->setEnabled(true);
        ui->pushButtonOver->setEnabled(true);
        ui->doubleSpinBox->setEnabled(true);

        for (int index = 0; index < count; ++index)
        {
            m_radioButtonList.value(index)->setEnabled(false);
        }
    }
}

void PanelSensorDebug::GoFront()
{
    m_proTime = clock();
    ui->pushButtonBack->setEnabled(false);
    ui->radioButtonHand->setEnabled(false);
    ui->debugSwitch->setEnabled(false);

    switch (m_type)
    {
    case STOP:
        m_type = FRONT_RUN;
        ui->pushButtonFront->setText(tr("Pause"));   break;

    case FRONT_RUN:
        m_type = FRONT_PAUSE;
        ui->pushButtonFront->setText(tr("Go"));   break;

    case FRONT_PAUSE:
        m_type = FRONT_RUN;
        ui->pushButtonFront->setText(tr("Pause"));   break;

    default: break;
    }
}

void PanelSensorDebug::GoBack()
{
    m_proTime = clock();
    ui->pushButtonFront->setEnabled(false);
    ui->radioButtonHand->setEnabled(false);
    ui->debugSwitch->setEnabled(false);

    if (m_index == 0)
    {
        m_index = m_radioButtonList.count() - 1;
    }

    switch (m_type)
    {
    case STOP:
        m_type = BACK_RUN;
        ui->pushButtonBack->setText(tr("Pause")); break;

    case BACK_RUN:
        m_type = BACK_PAUSE;
        ui->pushButtonBack->setText(tr("Go")); break;

    case BACK_PAUSE:
        m_type = BACK_RUN;
        ui->pushButtonBack->setText(tr("Pause")); break;

    default:
        ui->pushButtonBack->setText(tr("GoBack")); break;
    }
}

void PanelSensorDebug::Stop()
{
    m_type = STOP;
    m_index = 0;
    ui->pushButtonBack->setText(tr("GoBack"));
    ui->pushButtonFront->setText(tr("GoPosition"));
    ui->pushButtonBack->setEnabled(true);
    ui->pushButtonFront->setEnabled(true);
    ui->radioButtonHand->setEnabled(true);
    ui->debugSwitch->setEnabled(true);
    Clear();
}

void PanelSensorDebug::SetRunType(PanelSensorDebug::RunType type)
{
    m_type = type;
}

void PanelSensorDebug::SetTimeSpace(qreal timeSpace)
{
    m_timeSpace = timeSpace * 1000;
}

void PanelSensorDebug::TimeEvent()
{
    time_t time = clock();
    if (time >= m_proTime + m_timeSpace)
    {
        m_proTime = time;

        switch (m_type)
        {
        case FRONT_RUN:
            if (m_index < m_sensorStart || m_index > m_sensorEnd) {
                m_index = m_sensorStart;
            }

            Clear();
            if (m_index >= 1) {
                m_radioButtonList.value(m_index - 1)->setChecked(true);
            }

            m_index++;
            break;

        case BACK_RUN:
            if (m_index < m_sensorStart || m_index > m_sensorEnd) {
                m_index = m_sensorEnd;
            }

            Clear();
            if (m_index >= 1) {
                m_radioButtonList.value(m_index - 1)->setChecked(true);
            }

            m_index--;
            break;

        default:
            break;
        }
    }
}

void PanelSensorDebug::InitUI()
{
    // 将4个辅助UI添加到List中，用于实现128个radiobutton可以处于不选中状态
    m_radioButtonHide.push_back(ui->Sensor_133);
    m_radioButtonHide.push_back(ui->Sensor_134);
    m_radioButtonHide.push_back(ui->Sensor_135);
    // 隐藏不需要显示的UI控件
    for (int index = 0; index < m_radioButtonHide.count(); ++index)
    {
        m_radioButtonHide.value(index)->hide();
    }

    // 将128个手动传感器UI控件添加到List中便于操作
    AddButtonToList(ui->groupBox);
    AddButtonToList(ui->groupBox_2);
    AddButtonToList(ui->groupBox_3);

    // 将调试默认模式设置为手动调试
    ui->radioButtonHand->setChecked(true);
}

void PanelSensorDebug::AddButtonToList(QWidget *widget)
{
    foreach (QObject *obj, widget->children()) {
        QRadioButton *button = qobject_cast<QRadioButton *>(obj);
        if (!button) {
            continue;
        }
        if (!button->isHidden()) {
            m_radioButtonList << button;
        }
    }
}

void PanelSensorDebug::Connect()
{
    int count = m_radioButtonList.count();

    for (int index = 0; index < count; ++index) {
        connect(m_radioButtonList[index], &QRadioButton::toggled, [=](bool checked) {
            ActionChanged(index, checked);
        });
    }

    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimeEvent()));
    connect(ui->pushButtonOver, SIGNAL(clicked()), this, SLOT(Stop()));
    connect(ui->pushButtonBack, SIGNAL(clicked()), this, SLOT(GoBack()));
    connect(ui->pushButtonFront, SIGNAL(clicked()), this, SLOT(GoFront()));
    connect(ui->radioButtonHand, SIGNAL(clicked()), this, SLOT(ChangeModle()));
    connect(ui->radioButtonSelf, SIGNAL(clicked()), this, SLOT(ChangeModle()));
    connect(ui->debugSwitch, SIGNAL(clicked(bool)), this, SLOT(ChangeEnable(bool)));
    connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(SetTimeSpace(qreal)));

    for (auto obj: platform()->Instruments()->GetRegistry<UTInstrument>()) {
        connect(obj, &UTInstrument::ConnectionChanged, this, &PanelSensorDebug::ConnectionChanged);
    }
}

void PanelSensorDebug::Clear()
{
    int count = m_radioButtonHide.count();

    for (int index = 0; index < count; ++index)
    {
        m_radioButtonHide.value(index)->setChecked(true);
    }
}

void PanelSensorDebug::SetSensorSwitch(bool isOpen)
{
    emit SimulationEnableChanged(isOpen);

    Clear();
    p_action.data()->sEnable = isOpen;

    for (auto instrument: platform()->Instruments()->GetRegistry<UTInstrument>()) {
        instrument->sendRawMessage(IInstrumentDriver::MessageTrainSensor, QVariant::fromValue(p_action));
    }
}

void PanelSensorDebug::onChannelChanged(int channel)
{
    auto config = controlPanel()->getChannelConfigs().value(channel);
    int sensorId = config.sensorId % 1000 - 1;

    if (sensorId < m_radioButtonList.count() && sensorId >= 0) {
        m_radioButtonList.value(sensorId)->click();
    }
}

void PanelSensorDebug::OnScanStart()
{
    ui->debugSwitch->setChecked(false);
    ChangeEnable(false);
}

void PanelSensorDebug::on_spinBoxSensorStart_valueChanged(int value)
{
    m_sensorStart = value;
    ui->spinBoxSensorEnd->setMinimum(m_sensorStart + 1);
    if (m_sensorStart >= m_sensorEnd) {
        m_sensorEnd = m_sensorStart + 1;
        ui->spinBoxSensorEnd->setValue(m_sensorEnd);
    }

}

void PanelSensorDebug::on_spinBoxSensorEnd_valueChanged(int value)
{
    m_sensorEnd = value;
    ui->spinBoxSensorStart->setMaximum(m_sensorEnd - 1);
    if (m_sensorStart >= m_sensorEnd) {
        m_sensorStart = m_sensorEnd - 1;
        ui->spinBoxSensorStart->setValue(m_sensorStart);
    }
}
