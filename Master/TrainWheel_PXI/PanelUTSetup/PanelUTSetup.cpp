#include "PanelUTSetup.h"
#include "ui_PanelUTSetup.h"
#include "Platform/Platform.h"
#include "ConfigControl/ConfigCombox.h"
#include <QTextCodec>

const int PanelUTSetup::MIN_PULSE_WIDTH = 2;
const int PanelUTSetup::MAX_PULSE_WIDTH = 50;
const int PanelUTSetup::MIN_PRF = 100;
const int PanelUTSetup::MAX_PRF = 999999;
const int PanelUTSetup::tableViewWidth = 155;
const double PanelUTSetup::MAX_RANGE = 999999.0;
const double PanelUTSetup::MIN_RANGE = 1.0;
const double PanelUTSetup::MAX_DELAY = 999999.0;   // 估计值
const double PanelUTSetup::MIN_DELAY = 0.0;
const double PanelUTSetup::MAX_HEIGH = 1023.0;
const double PanelUTSetup::MIN_HEIGH = 0.0;
const double PanelUTSetup::MAX_GAIN = 120.0;

PanelUTSetup::PanelUTSetup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelUTSetup)
{
    ui->setupUi(this);
    InitUI();
    InitControlValue();
    ConnectTo();
}

PanelUTSetup::~PanelUTSetup()
{
    delete ui;
}

void PanelUTSetup::ConnectTo()
{
    connect(ui->widget_ChannelMgr, SIGNAL(SendChannelNo(int, WaveType)), this, SLOT(SetChannelNo(int, WaveType)));
}

/**
 * 绑定超声工艺
 * @brief PanelUTSetup::BindUTSetup
 * @param setup 绑定的UTSetup指针
 *
 */
void PanelUTSetup::BindUTSetup(UTSetup *setup)
{
    if (setup != NULL)
    {
        m_pUTSetup = setup;
        /* 底层设置相应的界面进行变化 */
        foreach (ConfigRadioButton *rBtn, rectifierModeRadioButtons)
        {
            rBtn->ConnectTo(m_pUTSetup);
        }

        ui->gainSpinBox->ConnectTo(m_pUTSetup);
        ui->gainRefSpinBox->ConnectTo(m_pUTSetup);
        ui->PRFSpinBox->ConnectTo(m_pUTSetup);
        ui->suppressSpinBox->ConnectTo(m_pUTSetup);
        ui->widgetTimebase->ConnectTo(m_pUTSetup);

        panelTCG = new PanelTCG(this);
        panelTCG->ConnectTo(m_pUTSetup);
        ui->tabWidgetControl->insertTab(2, panelTCG, tr("TCG"));
        panelUTGate = new PanelUTGate(m_pUTSetup, this);
        ui->tabWidgetControl->insertTab(3, panelUTGate, tr("UTGate"));
    }
}

void PanelUTSetup::UnbindUTSetup()
{
    if (m_pUTSetup != NULL)
    {
        /* 底层设置相应的界面进行变化 */
        foreach (ConfigRadioButton *rBtn, rectifierModeRadioButtons)
        {
            rBtn->Disconnect(m_pUTSetup);
        }

        ui->gainSpinBox->Disconnect(m_pUTSetup);
        ui->gainRefSpinBox->Disconnect(m_pUTSetup);
        ui->PRFSpinBox->Disconnect(m_pUTSetup);
        ui->suppressSpinBox->Disconnect(m_pUTSetup);
        ui->widgetTimebase->Disconnect(m_pUTSetup);

        panelTCG->Disconnect(m_pUTSetup);
        delete panelTCG;
        panelTCG = NULL;
        delete panelUTGate;
        panelUTGate = NULL;
        ui->tabWidgetControl->removeTab(2);
        ui->tabWidgetControl->removeTab(3);
    }
}
/*!
 * \brief PanelUTSetup::SetChannelNo 槽函数，接收到通道号时在Edit控件中显示
 * \param nChannelNo
 */
void PanelUTSetup::SetChannelNo(int nChannelNo, WaveType type)
{
    QString strNo("%1/%2");
//    QTextCodec *gbkCode = QTextCodec::codecForName("GB2312");
    UTSetup *setup = dynamic_cast<UTSetup *>(platform()->Setups()->GetByID(nChannelNo));
    setup->SetWaveType(type);

    switch (type)
    {
    case WaveType::WaveFlaw:
        ui->lineEdit_ChannelNo->setText(strNo.arg("伤波").arg(nChannelNo));
        break;

    case WaveType::WaveSurface:
        ui->lineEdit_ChannelNo->setText(strNo.arg("界面波").arg(nChannelNo));
        break;

    default:
        break;
    }

    emit SetupChanged(setup);
    emit ChannelChanged(nChannelNo);
}

void PanelUTSetup::GainDialToSpinBox(int value)
{
    int gainRef = ui->gainRefDial->value();
    if (gainRef + value > MAX_GAIN * 100)
    {
        ui->gainDial->setValue(MAX_GAIN * 100 - gainRef);
        double dValue = static_cast<double>(MAX_GAIN * 100 - gainRef) / 100.0;
        ui->gainSpinBox->setValue(dValue);
        return;
    }

    double dValue = static_cast<double>(value) / 100.0;
    ui->gainSpinBox->setValue(dValue);
}

void PanelUTSetup::GainSpinBoxToDial(double value)
{
    double gainRef = ui->gainRefSpinBox->value();
    if (gainRef + value > MAX_GAIN)
    {
        return;
    }

    int nValue = static_cast<int>(value * 100.0);
    ui->gainDial->setValue(nValue);
}

void PanelUTSetup::GainRefDialToSpinBox(int value)
{
    int gain = ui->gainDial->value();
    if (gain + value > MAX_GAIN * 100)
    {
        ui->gainRefDial->setValue(MAX_GAIN * 100 - gain);
        double dValue = static_cast<double>(MAX_GAIN * 100 - gain) / 100.0;
        ui->gainRefSpinBox->setValue(dValue);
        return;
    }

    double dValue = static_cast<double>(value) / 100.0;
    ui->gainRefSpinBox->setValue(dValue);
}

void PanelUTSetup::GainRefSpinBoxToDial(double value)
{
    double gain = ui->gainSpinBox->value();
    if (gain + value > MAX_GAIN)
    {
        return;
    }

    int nValue = static_cast<int>(value * 100.0);
    ui->gainRefDial->setValue(nValue);
}

/**
 * 初始化UI界面
 * @brief PanelUTSetup::InitUI
 */
void PanelUTSetup::InitUI()
{
    ui->PRFSpinBox->setRange(MIN_PRF, MAX_PRF);
    ui->lineEdit_ChannelNo->setEnabled(false);
    ui->tabWidgetControl->removeTab(1);
    GetWidgetChildList(ui->rectifierModeGroup, rectifierModeRadioButtons);
    connect(ui->gainDial, SIGNAL(valueChanged(int)), this, SLOT(GainDialToSpinBox(int)));
    connect(ui->gainSpinBox, SIGNAL(valueChanged(double)), this, SLOT(GainSpinBoxToDial(double)));
    connect(ui->gainRefDial, SIGNAL(valueChanged(int)), this, SLOT(GainRefDialToSpinBox(int)));
    connect(ui->gainRefSpinBox, SIGNAL(valueChanged(double)), this, SLOT(GainRefSpinBoxToDial(double)));
}

/**
 * 获取Widget中的控件
 * @brief PanelUTSetup::GetWidgetChildList
 * @param widget  // 需要读取的Widget的指针
 * @param rBtnList  // 读取到的控件指针储存到该List中
 */
void PanelUTSetup::GetWidgetChildList(QWidget *widget, RadioButtonList &rBtnList)
{
    QObjectList list = widget->children();   // 获取GroupBox中的项
    ConfigRadioButtonPointer radioBtn;
    foreach (QObject *obj, list)
    {
        if (strcmp(obj->metaObject()->className(), "ConfigRadioButton") == 0)  // 判断是否为GroupBox,从而获取其中的子项
        {
            radioBtn = dynamic_cast<ConfigRadioButton *>(obj);
            if (radioBtn)
            {
                rBtnList.append(radioBtn);
            }
        }
    }
}

void PanelUTSetup::InitControlValue()
{
    /* 第二个参数的值为默认情况下设置的值 */
    ui->PRFSpinBox->SetParameterKey("PRF");
    ui->gainSpinBox->SetParameterKey("Gain");
    ui->gainRefSpinBox->SetParameterKey("GainRef");
    ui->suppressSpinBox->SetParameterKey("Suppress");

    int index = 0;
    foreach (ConfigRadioButton *rBtn, rectifierModeRadioButtons)        // RectifierMode
    {
        rBtn->SetParameterKey("RectifierMode", index);
        index++;
    }
}

void PanelUTSetup::Disconnect(QWidget *widget, UTSetup *setup)
{
    QObjectList list = widget->children();   // 获取不到GroupBox中的项
    foreach (QObject *obj, list)
    {
        DisconnectWidgetChildren(obj, setup);
        if (QString(obj->metaObject()->className()) == "QGroupBox")  // 判断是否为GroupBox,从而获取其中的子项
        {
            foreach (QObject *objChild, obj->children())
            {
                DisconnectWidgetChildren(objChild, setup);
            }
        }
    }
}

void PanelUTSetup::DisconnectWidgetChildren(QObject *obj,  UTSetup *setup)
{
    ConfigRadioButton *radioButton = NULL;
    ConfigDoubleSpinBox *spinBox = NULL;
    ConfigCombox  *combox = NULL;
    if (QString(obj->metaObject()->className()) == "ConfigRadioButton")
    {
        radioButton = qobject_cast<ConfigRadioButton *>(obj);
        if (radioButton)
        {
            radioButton->Disconnect(setup);
        }
    }
    if (QString(obj->metaObject()->className()) == "ConfigSpinBox" )
    {
        spinBox = qobject_cast<ConfigDoubleSpinBox *>(obj);
        if (spinBox)
        {
            spinBox->Disconnect(setup);
        }
    }
    if (QString(obj->metaObject()->className()) == "ConfigCombox")
    {
        combox = qobject_cast<ConfigCombox *>(obj);
        if (combox)
        {
            combox->Disconnect(setup);
        }
    }
}

void PanelUTSetup::CopyChannelData()
{
    int nSourceNo = ui->lineEdit_Source->text().toInt();
    int nDestStartNo = ui->lineEdit_DestStart->text().toInt();
    int nDestEndNo = ui->lineEdit_DestEnd->text().toInt();

    Q_UNUSED(nSourceNo);
    Q_UNUSED(nDestStartNo);
    Q_UNUSED(nDestEndNo);
}
