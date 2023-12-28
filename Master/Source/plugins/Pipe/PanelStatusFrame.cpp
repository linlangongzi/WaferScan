#include "PanelStatusFrame.h"
#include "ui_PanelStatusFrame.h"
#include <Core/Platform.h>
#include <Pipe/PipeManager.h>
#include <QMutex>
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <Waveform/WaveformManager.h>

#ifdef Q_CC_MSVC
extern QMutex g_ItemCS;
extern GlobalItemValue g_ItemValue;
#endif

PanelStatusFrame::PanelStatusFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PanelStatusFrame),
    m_TIMER_REFRESH_OPC_DATA(0)
{
    ui->setupUi(this);
    Init();

    m_TIMER_REFRESH_OPC_DATA = startTimer(500);
    UpdateLocked(0);
}

PanelStatusFrame::~PanelStatusFrame()
{
    delete ui;

    if (m_TIMER_REFRESH_OPC_DATA != 0) {
        killTimer(m_TIMER_REFRESH_OPC_DATA);
    }
}

#ifdef Q_CC_MSVC
void PanelStatusFrame::UpdatePLCStatus(GlobalItemValue &itemNew)
{
    QString styleBlack = QString();
    QString styleGreen = QString("color: rgb(0, 170, 0);;");

    // 电气连接状态
    if (itemNew.PLCState != m_itemValue.PLCState) {
        m_itemValue.PLCState = itemNew.PLCState;
        if (m_itemValue.PLCState) {
            ui->labelPLC->setText(tr("电气: 已连接"));
            ui->labelPLC->setStyleSheet(styleGreen);
        } else {
            ui->labelPLC->setText(tr("电气: 已断开"));
            ui->labelPLC->setStyleSheet(styleBlack);
        }
    }

    if (itemNew.IsInstrumentReady != m_itemValue.IsInstrumentReady) {
        m_itemValue.IsInstrumentReady = itemNew.IsInstrumentReady;
        if (m_itemValue.IsInstrumentReady) {
            ui->labelInstrumentReady->setText(tr("仪器状态：已就绪"));
            ui->labelInstrumentReady->setStyleSheet(styleGreen);
        } else {
            ui->labelInstrumentReady->setText(tr("仪器状态：未就绪"));
            ui->labelInstrumentReady->setStyleSheet(styleBlack);
        }
    }

    if (itemNew.IsLocked != m_itemValue.IsLocked) {
        m_itemValue.IsLocked = itemNew.IsLocked;
        LockedSetups(m_itemValue.IsLocked);
        if (itemNew.IsLocked) {
            ui->labelParamLock->setText(tr("参数锁定：开启"));
            ui->labelParamLock->setStyleSheet(styleGreen);
        } else {
            ui->labelParamLock->setText(tr("参数锁定：关闭"));
            ui->labelParamLock->setStyleSheet(styleBlack);
        }
    }

    // 检测模式
    if (itemNew.SWTestMode != m_itemValue.SWTestMode) {
        m_itemValue.SWTestMode = itemNew.SWTestMode;
        Mode mode = Mode::FinalPipe;

        switch (m_itemValue.SWTestMode) {
        case 1: mode = Mode::FinalPipe; break;
        case 2: mode = Mode::FinalBar; break;
        case 3: mode = Mode::SamplePipe; break;
        case 4: mode = Mode::SampleBar; break;
        default: mode = Mode::InvalidMode; break;
        }
        ui->labelTestMode->setText(tr("检测模式: %1").arg(PipeManager::ModeName(mode)));

        emit TestModeChanged(mode);
    }

    // 断电续扫
    if (itemNew.KeepTesting != m_itemValue.KeepTesting) {
        m_itemValue.KeepTesting = itemNew.KeepTesting;

        if (m_itemValue.KeepTesting) {
            ui->labelkeepTesting->setText(tr("电气续扫标志：开启"));
            ui->labelkeepTesting->setStyleSheet(styleGreen);
        } else {
            ui->labelkeepTesting->setText(tr("电气续扫标志：关闭"));
            ui->labelkeepTesting->setStyleSheet(styleBlack);
        }

        emit KeepTestingChanged(m_itemValue.KeepTesting);
    }

    // 缺陷回位
    if (itemNew.ReturnMark != m_itemValue.ReturnMark) {
        m_itemValue.ReturnMark = itemNew.ReturnMark;

        if (m_itemValue.ReturnMark) {
            ui->labelReturn->setText(tr("缺陷回位：开启"));
            ui->labelReturn->setStyleSheet(styleGreen);
        } else {
            ui->labelReturn->setText(tr("缺陷回位：关闭"));
            ui->labelReturn->setStyleSheet(styleBlack);
        }
    }

    // 探伤使能
    if (itemNew.TestStartFlag != m_itemValue.TestStartFlag) {
        m_itemValue.TestStartFlag = itemNew.TestStartFlag;

        if (m_itemValue.TestStartFlag) {
            ui->labelStartFlag->setText(tr("探伤使能：开启"));
            ui->labelStartFlag->setStyleSheet(styleGreen);
        } else {
            ui->labelStartFlag->setText(tr("探伤使能：停止"));
            ui->labelStartFlag->setStyleSheet(styleBlack);
        }

        emit TestStartFlagArrived(m_itemValue.TestStartFlag);
    }

    // 探头盒使用序列
    if (itemNew.ProbeBoxStatus != m_itemValue.ProbeBoxStatus) {
        m_itemValue.ProbeBoxStatus = itemNew.ProbeBoxStatus;
        int updateBytes = 5;
        for (int byte = 0; byte < updateBytes; byte++) {
            quint16 mark = m_itemValue.ProbeBoxStatus;
            if ( (mark >> byte) & 0x1 ) {
                m_boxsLabelVector.value(byte)->setStyleSheet(styleGreen);
            } else {
                m_boxsLabelVector.value(byte)->setStyleSheet(styleBlack);
            }
        }

        emit ProbeBoxStatusChanged(m_itemValue.ProbeBoxStatus);
    }
}
#endif

void PanelStatusFrame::UpdatePLXConnectionStatus(InstrumentStatus status)
{
    switch (status.state) {
    case InstrumentStatus::Connected:   ui->labelPLX->setText(tr("下位机: 已连接"));  break;
    default:                            ui->labelPLX->setText(tr("下位机: 已断开"));  break;
    }
}

void PanelStatusFrame::OnResultChanged(const TestResultType &result)
{
    switch (result) {
    case TestResultType::Accept:
        ui->labelResult->setText(tr("判伤结果：合格品"));
        ui->labelResult->setStyleSheet(QString("color: rgb(0, 170, 0);;"));
        break;
    case TestResultType::Reject:
        ui->labelResult->setText(tr("判伤结果：不合格品"));
        ui->labelResult->setStyleSheet(QString("color: rgb(255, 0, 0);;"));
        break;
    case TestResultType::Decouple:
        ui->labelResult->setText(tr("判伤结果：耦合不良品"));
        ui->labelResult->setStyleSheet(QString("color: rgb(255, 0, 255);;"));
        break;
    default:
        ui->labelResult->setText(tr("判伤结果：未评判"));
        ui->labelResult->setStyleSheet(QString(""));
        break;
    }
}

void PanelStatusFrame::timerEvent(QTimerEvent *event)
{
#ifdef Q_CC_MSVC
    if(event->timerId() == m_TIMER_REFRESH_OPC_DATA) {
        g_ItemCS.lock();
        GlobalItemValue itemNew = g_ItemValue;
        g_ItemCS.unlock();
        UpdatePLCStatus(itemNew);
    }
#else
    Q_UNUSED(event);
#endif // Q_CC_MSVC
}

void PanelStatusFrame::Init()
{
    m_boxsLabelVector.clear();
    m_boxsLabelVector.push_back(ui->labelProbeBox_1);
    m_boxsLabelVector.push_back(ui->labelProbeBox_2);
    m_boxsLabelVector.push_back(ui->labelProbeBox_3);
    m_boxsLabelVector.push_back(ui->labelProbeBox_4);
    m_boxsLabelVector.push_back(ui->labelProbeBox_5);
}

/*!
 * \brief PanelStatusFrame::LockedSetups
 * \param isLocked
 * 锁定、解锁工艺参数设置功能
 */
void PanelStatusFrame::LockedSetups(bool isLocked)
{
//    for (UTSetup *setup: UltrasonicManager::setups()->GetRegistry<UTSetup>()) {
//        setup->SetLock(isLocked, true);
//    }
    WaveformManager::setSetupLock(isLocked);
}

/*!
 * \brief PanelStatusFrame::UpdateLocked
 * \param setupIndex
 * 将指定序号的setup中的Locked状态更新给m_itemValue.IsLocked
 */
void PanelStatusFrame::UpdateLocked(int setupIndex)
{
    if (setupIndex >= UltrasonicManager::setups().count()) {
        return;
    }

#ifdef Q_CC_MSVC
    UTSetup setup = UltrasonicManager::setups().value(setupIndex);
//    if (setup) {
//        m_itemValue.IsLocked = setup->Get("Locked").toBool();
//    }
//    m_itemValue.IsLocked = setup
    // TODO: m_item. isLocked
#endif
}
