#include "AECWork.h"
#include <QThread>
#include "p9054_lib.h"
#include "plx_lib_x.h"
#include "HardwareAddress.h"
#include "const.h"
#include <QMessageBox>
#include "CardWork/PLXCommunication.h"
#include <QTime>
#include "ProbeConnectManager/ProbeConnectManager.h"
#include "TcpServer/UTSetupSetOne.h"
#include "time.h"
#include "ConfigObject/JsonFile.h"

using namespace NS_PXI100;

const int AECWork::m_frameBytes = sizeof(FEATURE_DATA) /  sizeof(WORD) * 2;
QVector<int> AECWork::vecSet;
AECWork::AECWork(QObject *parent) :
    CardWorkBase(parent),
    m_usEyeArea(0),
    m_curEyeArea(0),
    m_usSensor(0),
    m_bMaxFrameFlag(false),
    m_bEyesRead(false),
    m_usSoftIdx(-1),
    m_usSoftIdx1(-1),
    m_bDeleteFrames(true),
    m_nDeleteNum(2)
{
    m_pParam = SharedCardParam::create();
    connect(platform()->GetUTSetupSetOne(), SIGNAL(SetOne(UTSharedDataParam, qint32)),
            this, SLOT(OnSetOne(UTSharedDataParam, qint32)));

    // 保证内存不在线程外被自动删掉
    m_probeNoManager = platform()->GetProbeNoManager();
    m_frameMax = UTSharedFeatureFrame::create();
    m_frameMax1 = UTSharedFeatureFrame::create();
    for (auto &value : m_curEyeAreaSingle)
    {
        value = 0;
    }
    for (auto &value : m_usSoftIdxSingle)
    {
        value = -1;
    }
    for (auto &value : m_usSoftIdxSingle1)
    {
        value = -1;
    }
    for (auto &value : m_usSensorSingle)
    {
        value = 0;
    }
    m_nDeleteNum = platform()->GetDeleteFrameNum();
    m_avgCount = platform()->Get("MaxFrameAvgCount").toInt();
    m_sensorCardNum = platform()->GetSensorCardNum();
    m_straightFlangeCardNum = platform()->GetStraightFlangeCardNum();
    m_21DegreeCardNum = platform()->Get21DegreeCardNum();
    m_isStraightFlangeTwoSensor = platform()->IsStraightFlangeTwoSensor();
    m_is21DegreeTwoSensor = platform()->Is21DegreeTwoSensor();
    if (isTwoSensorWork())  // 直探头轮缘卡为单区激励,正负21度为双区激励
    {
       m_avgCount = m_avgCount * 2;
    }
}

/*!
 * \brief AECWork::初始化AEC卡
 * \param pParam
 * \return
 */
bool AECWork::InitCard()
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return false;
    }

    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, SYS_RESET_ADDRESS, 1);
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, SYS_RESET_ADDRESS, 0);

    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_ADDRESS, 1);
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_ADDRESS, 0);

    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, 1);
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, 0);

    OnSetScanStutas(1);
    OnSetSurfaceTrigger(true);
    int nRpf = 30e6 / 900;
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, REPEAT_FREQUENCE_ADDR, nRpf);
    int sensorCardNum = platform()->GetSensorCardNum();
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0,
                    CARD_NO_ADDRESS, m_pParam->index - sensorCardNum);
#if FEATURE_SINGLE_CHANNEL
    InitHardChn();
#endif
    return true;
}

void AECWork::InitHardChn()
{
    sharedProbeNoManager pProbeNoManager = platform()->GetProbeNoManager();
    if(!pProbeNoManager)
    {
        qDebug() << "AECWork::InitHardChn() pProbeNoManager is null";
        return;
    }

    JsonFile jsonFile;
    jsonFile.setFileName(pProbeNoManager->GetProbeConnectFileName());
    jsonFile.open();
    QJsonArray array = jsonFile.Get("Connection").toArray();
    foreach (QJsonValue value, array)   // 遍历所有AEC卡
    {
        QJsonObject obj = value.toObject();
        qint32 nAecCardNo = obj["AecCardNo"].toInt();
        if (m_pParam.isNull() || !m_pParam->hDev) {
            continue;
        }

        if(nAecCardNo == m_pParam->index)
        {
            QJsonArray arrayAecChn = obj["AecChannel"].toArray();
            foreach (QJsonValue value, arrayAecChn)     // 遍历所有AEC硬通道
            {
                QJsonObject obj = value.toObject();
                qint32 nAecChnNo = obj["AecChannelNo"].toInt();
                QJsonArray arrayTxChn = obj["TxChannel"].toArray();
                QJsonObject objTxChn = arrayTxChn.first().toObject();  // 取第一个计算就可以
                qint32 nProbeNo = objTxChn["first"].toInt();
                qint32 nEyeArea = pProbeNoManager->ProbeNo2EyeArea(nProbeNo) / 16;
                P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0,
                                ADDR_CHN_EYE_AREA_START + (2 * nAecChnNo), nEyeArea);
            }
        }
    }

    return;
}

/*!
 * \brief AECWork::AECCardStart
 * \param pParam
 * \return 成功返回true,否则返回false
 */
bool AECWork::CardStart()
{
    try
    {
        unsigned short nInterSpace[FLAW_DOT_MAX_NO];
        RangeProcess(WAVESPEED_STEEL, 300, nInterSpace);
        SetMonitorRange(nInterSpace);

        return true;
    }
    catch(QString exception)
    {
        QMessageBox::about(NULL,"AECCardStart异常",exception);
        return false;
    }
}

/*!
 * \brief AECWork::读取AEC卡数据，监视通道和C扫特征数据
 * \param pParam
 * \return
 */
bool AECWork::CardReading()
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return false;
    }

    qint32 iCardIndex = m_pParam->index;
    qint32 nSelectChn = m_pParam->wSelChannel;

    if(SelectChnInCard(nSelectChn)) {
        UTSharedMonitorFrame monitorframe = UTSharedMonitorFrame::create();

        bool bRet = ReadMonitorData(monitorframe->nPicData);
        if (bRet) {
            monitorframe->stFeatureData.nGateIMPTimeLo =
                P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS);
            monitorframe->stFeatureData.nGateIMPTimeHi =
                P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0x2);
            monitorframe->stFeatureData.nGateIMPAmp =
                P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0x4);
            monitorframe->stFeatureData.nGateAMPTime =
                P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0x6);
            monitorframe->stFeatureData.nGateAMPAmp =
                P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0x8);
            monitorframe->stFeatureData.nGateBMPTime =
                P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0xA);
            monitorframe->stFeatureData.nGateBMPAmp =
                P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0xC);
            monitorframe->stFeatureData.nGateI2MPTimeLo =
                    P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0xE);
            monitorframe->stFeatureData.nGateI2MPTimeHi =
                    P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0x10);
            monitorframe->stFeatureData.nGateI2MPAmp =
                P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0x12);

            quint16 nMonitorChnNo = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0,
                                                   MONITOR_FEATURE_DATA_ADDRESS + 0x16);
            monitorframe->stFeatureData.nProbeNo = nMonitorChnNo;

            // B闸门内波形宽度
#if FLANGE_ANALYSIS
            quint16 nGateBWaveWidth = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, 0x4A);
            monitorframe->stFeatureData.nGateBWaveWidth = nGateBWaveWidth;
#endif
            emit MonitorDataRead(monitorframe, nMonitorChnNo);
        }

        // 通道数据清零
        WORD wValue = PCI_SAMPLE_RESET_ON;
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);

        wValue = PCI_SAMPLE_RESET_OFF;
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);

        wValue = PCI_SAMPLE_RESET_ON;
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_ADDRESS, wValue);

        wValue = PCI_SAMPLE_RESET_OFF;
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_ADDRESS, wValue);
    }

    /* 第一个探伤aec卡读取所有的传感器监视状态 */
    if (iCardIndex == platform()->GetSensorCardNum()) {
        emit EyesDataRead(ReadEyesStatus());
    }

    /* 读取C扫特征数据,包括前3个传感器卡和后8个探伤卡 */
    ReadFlawResult();

    return 0;
}

/*!
 * \brief AECWork::读取C扫特征数据
 * \param pParam
 * \param nCardIndex
 * \return
 */
bool AECWork::ReadFlawResult()
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return false;
    }

    quint16 wIndex = 0;
    quint32 dwAddress = 0;   // 缓冲区地址
    quint16 wIsValid = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_EN_FLAG_ADDRESS);
    quint16 wTemp = wIsValid & 0x1;
    if (wTemp == 1) {
        wIndex = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_INDEX_ADDRESS);
        wIndex &= 0x3;
    } else {
        return false;
    }

    if (wIndex == 1) {
        dwAddress = FEATURE_BUFFER1_ADDRESS;
    } else if (wIndex == 2) {
        dwAddress = FEATURE_BUFFER2_ADDRESS;
    } else {
        return false;
    }

    UTSharedFeatureSegment segment = UTSharedFeatureSegment::create();
    qint32 nSize = m_frameBytes * C_SCAN_NUM_FRAMES_PER_PACKET;
    BOOL bRet = P9054_DMAReadWriteBlock(m_pParam->hDev, dwAddress, segment.data(), nSize, TRUE, P9054_MODE_WORD, P9054_DMA_CHANNEL_0);
    if(!bRet) {
        qDebug() << "DMARead cscan data failed";
        return false;
    }

# if 0
    int nSize = sizeof(FEATURE_DATA) /  sizeof(WORD);
    for (int j = 0; j < nFrameNum; j++)
    {
        UTSharedFeatureFrame frame = UTSharedFeatureFrame::create();

        quint16* pwData = reinterpret_cast<quint16 *>(frame.data());
     /*   clock_t time = clock();
        P9054_ReadBlock (m_pParam->hDev, dwAddress, pwData,
                           nSize, P9054_ADDR_SPACE0, P9054_MODE_WORD);
        qDebug() << "reakblock: " << clock() - time;
        dwAddress = dwAddress + nSize;*/    // 效率跟点读一样
       for (qint32 i = 0; i < nSize; i++)
        {
            *pwData = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, dwAddress);
            pwData++;
            dwAddress += 2;
        }

   //     segment->append(frame);
        memcpy(&(segment->featurs[j]), frame.data(), sizeof(FEATURE_DATA));
    }
#endif

    for (int i = 0; i < C_SCAN_NUM_FRAMES_PER_PACKET; ++i) {
        segment->featurs[i].wPxiIdx = m_pParam->index;
    }

    emit CscanDataRead(segment, C_SCAN_NUM_FRAMES_PER_PACKET);

    quint16 wValue = PCI_SAMPLE_RESET_ON;
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_FLAG_ADDRESS, wValue);

    wValue = PCI_SAMPLE_RESET_OFF;
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_FLAG_ADDRESS, wValue);

    return true;
}

/*!
 * \brief AECWork::通道选择
 * \param nChnNo 探头号
 * \param nSurOrFlaw 伤波或底波
 */
void AECWork::SetSelectChannel(qint32 nChnNo, qint32 nSurOrFlaw)
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return;
    }

    m_pParam->wSelChannel = nChnNo;
    m_pParam->nFlawOrBottom = nSurOrFlaw;

    if(SelectChnInCard(nChnNo)) {
        qint32 nCardChnNo = platform()->GetProbeNoManager()->ProbNo2CardChnNo(nChnNo);
        qint32 nSoftChnNo = platform()->GetProbeNoManager()->ProbNo2SoftChnNo(nChnNo, ProbeConnectManager::DOUBLE);
        int sensorCardNum = platform()->GetSensorCardNum();

        if(sensorCardNum && m_pParam->index < sensorCardNum) {
            nSoftChnNo = platform()->GetProbeNoManager()->ProbNo2SoftChnNo(nChnNo, ProbeConnectManager::SINGLE);
        }

        if((nCardChnNo == -1) || (nSoftChnNo == -1)) {
            return;
        }

        quint16 wValueH = static_cast<quint16>(nCardChnNo * 2 + nSurOrFlaw);
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_CHANNEL_H_ADDRESS, wValueH);

        quint16 wValueS = static_cast<quint16>(nSoftChnNo);  //  软通道号
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_CHANNEL_S_ADDRESS, wValueS);

        //A扫数据清零
        quint16 wValue = PCI_SAMPLE_RESET_ON;
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);

        wValue = PCI_SAMPLE_RESET_OFF;
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);

        wValue = PCI_SAMPLE_RESET_ON;
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_ADDRESS, wValue);

        wValue = PCI_SAMPLE_RESET_OFF;
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_ADDRESS, wValue);
    }

    return;
}
/*!
 * \brief 设置指定通道号参数
 * \param param
 * \param nChnNo
 */
void AECWork::OnSetOne(UTSharedDataParam param, qint32 nChnNo)
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return;
    }

    if(SelectChnInCard(nChnNo)) {
        WORD nInterSpace[FLAW_DOT_MAX_NO];
        RangeProcess(param->Velocity, param->Range, nInterSpace);
        SetMonitorRange(nInterSpace);
        CH_CONFIG_HARD stConfig;
        memset(&stConfig, 0, sizeof(stConfig));
        Params2HardParam(param, stConfig);

        P9054_HANDLE hDev = m_pParam->hDev;

        qint32 nCardChnNo = platform()->GetProbeNoManager()->ProbNo2CardChnNo(nChnNo);
        qint32 nSoftChnNo = GetSoftChnNo(nChnNo);
        if((nCardChnNo == -1) || (nSoftChnNo == -1))
        {
            return;
        }

        quint16 wValueH = static_cast<quint16>(nCardChnNo + 1);  // 8个硬通道0x01000,0x02000,0x03000,0x04000,0x05000,0x06000,0x07000,0x08000
        //quint16 wValueS = 8 * nTxChn0or1 + nTxChnNo;  // 16个软通道（8（前置通道）*2（探头）），1-8第一排探头，9-16第二排探头
        quint16 wValueS = static_cast<quint16>(nSoftChnNo);

        quint32 dwAddrStart = wValueH * SOFT_CHANNEL_CONFIG_PARARM_ADDRESS + wValueS * 0x80;

        quint32 dwAddr = dwAddrStart;
        quint16 *pData = reinterpret_cast<quint16*>(&stConfig);

        int nEndAddr = sizeof(CH_CONFIG_HARD) - 1;
        qint32 i = 0;
        while(dwAddr < dwAddrStart + nEndAddr)  // 参数个数
        {
            P9054_WriteWord(hDev, P9054_ADDR_SPACE0, dwAddr, pData[i]);
            dwAddr += 2;
            i++;
        }
    }
}

void AECWork::SetEyesStatusReadFlag(bool isOpen)
{
    m_bEyesRead = isOpen;
}

/*!
 * \brief AECWork::通道参数转换成配置硬件的参数
 * \param param
 * \param stConfig
 */
void AECWork::Params2HardParam(UTSharedDataParam param, CH_CONFIG_HARD &stConfig)
{
    stConfig.stPlus.wAPlus = Gain2Data(20);
    stConfig.stPlus.wBPlus = Gain2Data(20);
    stConfig.stPlus.wCPlus = Gain2Data(param->Plus / 2);
    stConfig.stPlus.wDPlus = Gain2Data(param->Plus / 2);
    stConfig.wBottomFlawPlusDiff = Gain2Data(param->CPlus);
    stConfig.wTCGEn = param->TCGEnable;
    stConfig.ProbeNo = param->ProbeNo;
//    stConfig.eyeArea = param->eyeArea;

    quint32 nDelay = Range2Dot(param->Velocity, param->Delay);
    for(qint32 i = 0; i < SUB_CONFIG_NUM; i++)
    {
        stConfig.stWaveConfigure[i].DelayLo = static_cast<quint16>(nDelay & 0xFFFF);
        stConfig.stWaveConfigure[i].DelayHi = static_cast<quint16>(nDelay >> 16);
        stConfig.stWaveConfigure[i].Suppression = param->stWaveConfigure[i].Suppression / 100 * MAX_HEIGHT;
        stConfig.stWaveConfigure[i].DetectMode = param->stWaveConfigure[i].DetectMode;
        stConfig.stWaveConfigure[i].MiddleValue = param->stWaveConfigure[i].MiddleValue;

        // 闸门I1
        stConfig.stWaveConfigure[i].stGateI1.GateEnable =
                param->stWaveConfigure[i].stGate[0].GateEnable;
        quint32 nGtatStart = Range2Dot(param->Velocity, param->stWaveConfigure[i].stGate[0].GateStart);
        nGtatStart = nGtatStart - nDelay;
        stConfig.stWaveConfigure[i].stGateI1.GateStartLo = static_cast<quint16>(nGtatStart & 0xFFFF);
        stConfig.stWaveConfigure[i].stGateI1.GateStartHi = static_cast<quint16>(nGtatStart >> 16);
        stConfig.stWaveConfigure[i].stGateI1.GateWidth =
                 static_cast<quint16>(Range2Dot(param->Velocity, param->stWaveConfigure[i].stGate[0].GateWidth));
        stConfig.stWaveConfigure[i].stGateI1.GateAmp =
                param->stWaveConfigure[i].stGate[0].GateAmp / 100 * MAX_HEIGHT;
        //闸门A
        stConfig.stWaveConfigure[i].stGateA.GateEnable =
                param->stWaveConfigure[i].stGate[1].GateEnable;
        nGtatStart = Range2Dot(param->Velocity, param->stWaveConfigure[i].stGate[1].GateStart);
        stConfig.stWaveConfigure[i].stGateA.GateStart = nGtatStart;
        stConfig.stWaveConfigure[i].stGateA.GateWidth =
                 static_cast<quint16>(Range2Dot(param->Velocity, param->stWaveConfigure[i].stGate[1].GateWidth));
        stConfig.stWaveConfigure[i].stGateA.GateAmp =
                param->stWaveConfigure[i].stGate[1].GateAmp / 100 * MAX_HEIGHT;
        //闸门B
        stConfig.stWaveConfigure[i].stGateB.GateEnable =
                param->stWaveConfigure[i].stGate[2].GateEnable;
        nGtatStart = Range2Dot(param->Velocity, param->stWaveConfigure[i].stGate[2].GateStart);
        stConfig.stWaveConfigure[i].stGateB.GateStart = nGtatStart;
        stConfig.stWaveConfigure[i].stGateB.GateWidth =
                 static_cast<quint16>(Range2Dot(param->Velocity, param->stWaveConfigure[i].stGate[2].GateWidth));
        stConfig.stWaveConfigure[i].stGateB.GateAmp =
                param->stWaveConfigure[i].stGate[2].GateAmp / 100 * MAX_HEIGHT;
        // 闸门I2
        stConfig.stWaveConfigure[i].stGateI2.GateEnable =
                param->stWaveConfigure[i].stGate[3].GateEnable;
        nGtatStart = Range2Dot(param->Velocity, param->stWaveConfigure[i].stGate[3].GateStart);
        nGtatStart = nGtatStart - nDelay;
        stConfig.stWaveConfigure[i].stGateI2.GateStartLo = static_cast<quint16>(nGtatStart & 0xFFFF);
        stConfig.stWaveConfigure[i].stGateI2.GateStartHi = static_cast<quint16>(nGtatStart >> 16);
        stConfig.stWaveConfigure[i].stGateI2.GateWidth =
                 static_cast<quint16>(Range2Dot(param->Velocity, param->stWaveConfigure[i].stGate[3].GateWidth));
        stConfig.stWaveConfigure[i].stGateI2.GateAmp =
                param->stWaveConfigure[i].stGate[3].GateAmp / 100 * MAX_HEIGHT;
    }

    return;
}

void AECWork::SendAllFeatureFrame(UTSharedFeatureSegment  segmentIn, qint32 nFrameNum)
{
#if FLANGE_ANALYSIS
    // 上传全部轮缘数据
    // 轮缘数据不取最大帧
    {
#if FEATURE_SINGLE_CHANNEL
        UTSharedCommuSegment convertSegment = UTSharedCommuSegment::create();
        int chnSize = sizeof(CH_L_FEATURE_DATA);
        for (int i = 0; i < nFrameNum; ++i) {
            for (int j = 0; j < AEC_CHN_NUM; ++j) {
                if (segmentIn->featurs[i].wProbeNo[j] != 0) {
                    UTSharedFeatureCommu feature = UTSharedFeatureCommu::create();
                    memcpy(&feature->stData, &segmentIn->featurs[j], chnSize);
                    feature->wLightEye = GetEyeLowOrHi(segmentIn->featurs[i].wLightEye[j / 2], j);
                    feature->wProbeNo = segmentIn->featurs[i].wProbeNo[j];
                    feature->wSoftChannelIndex = GetSoftChnIdx(segmentIn->featurs[i].wSoftChannelIndex[j / 4], j);
    #if FLANGE_ANALYSIS
                    feature->stGateBWaveWidth = segmentIn->featurs[i].stGateBWaveWidth[j];
                    feature->wTimeStampL = segmentIn->featurs[i].wTimeStampL;
                    feature->wTimeStampH = segmentIn->featurs[i].wTimeStampH;
    #endif
                    convertSegment->append(feature);
                }
            }
        }
        emit CscanDataRead(convertSegment);
#else
        for (int i = 0; i < C_SCAN_NUM_FRAMES_PER_PACKET; ++i) {
            if (m_pParam.isNull() || !m_pParam->hDev) {
                continue;
            }

            segmentIn->featurs[i].wPxiIdx = m_pParam->index;
        }

        emit CscanDataRead(segmentIn, nFrameNum);
#endif
    }
#endif
}

// 第16个光眼区缺用来判断其结束的标志
/*!
 * \brief AECWork::取最大帧
 * \param segmentIn 需取最大帧的数据段
 * \param nFrameNum 数据段的帧数
 * \param nFrameRead   已经读过的帧数
 * \param frameMax   最大帖
 * \return
 */
#if FEATURE_SINGLE_CHANNEL
// 每个硬通道对应一个传感器信号
bool AECWork::GetMaxFeatureFrame(UTSharedFeatureSegment  segmentIn, qint32 nFrameNum,
                                 qint32 &nFrameRead, UTSharedFeatureFrame  frameMax)
{
    for(int i = nFrameRead; i < nFrameNum; i++)
    {
        for(int j = 0; j < AEC_HARD_CHN_NUM; j++)
        {
            if(segmentIn.data()->featurs[i].wProbeNo[j] != 0) // 有效探头号，无激发的探头号为0
            {
                quint16 usEyeArea = GetEyeLowOrHi(segmentIn.data()->featurs[i].wLightEye[j / 2], j);
                /*if(m_curEyeAreaSingle[j] == -1)
                {
                    if(GetEyeLowOrHi(usEye, j) == 0)
                    {
                        memcpy(&frameMax.data()->stData[j], &segmentIn.data()->featurs[i].stData[j], sizeof(CH_L_FEATURE_DATA));
                        m_curEyeAreaSingle[j] = GetEyeLowOrHi(usEye, j);
                    }
                }
                else*/
                {
                    if(m_curEyeAreaSingle[j] == usEyeArea)
                    {
                        // 当前光眼区域内有效的探头数（即一块AEC卡内有接探头的硬通道数）
                        //QList<qint32> listProbe = platform()->GetProbeNoManager()->EyeArea2ProbeNoList(m_usEyeArea * 16 + m_curEyeArea);

                        // 闸门A的最大幅值
                        quint16 usHeightA = segmentIn.data()->featurs[i].stData[j].stFlawFeature.stGateA.wFlawHeight;
                        quint16 usHeightMaxA = frameMax.data()->stData[j].stFlawFeature.stGateA.wFlawHeight;

                        if(usHeightA > usHeightMaxA)
                        {
                            frameMax.data()->stData[j].stFlawFeature.stGateA.wFlawHeight = usHeightA;
                            quint16 usFlawTime = segmentIn.data()->featurs[i].stData[j].stFlawFeature.stGateA.wFlawTime;
                            frameMax.data()->stData[j].stFlawFeature.stGateA.wFlawTime = usFlawTime;
                            //memcpy(&frameMax.data()->stData[j], &segmentIn.data()->featurs[i].stData[j], sizeof(CH_L_FEATURE_DATA));
                        }

                        // 闸门B的最大幅值
                        quint16 usHeightB = segmentIn.data()->featurs[i].stData[j].stFlawFeature.stGateB.wFlawHeight;
                        quint16 usHeightMaxB = frameMax.data()->stData[j].stFlawFeature.stGateB.wFlawHeight;

                        if(usHeightB > usHeightMaxB)
                        {
                            frameMax.data()->stData[j].stFlawFeature.stGateB.wFlawHeight = usHeightB;
                            quint16 usFlawTime = segmentIn.data()->featurs[i].stData[j].stFlawFeature.stGateB.wFlawTime;
                            frameMax.data()->stData[j].stFlawFeature.stGateB.wFlawTime = usFlawTime;
                        }
                        // 通道号
                        frameMax.data()->wProbeNo[j] = segmentIn.data()->featurs[i].wProbeNo[j];
                    }
                    else
                    {
                        UTSharedFeatureCommu send = UTSharedFeatureCommu::create();
                        memcpy(&send.data()->stData, &m_frameMax.data()->stData[j], sizeof(CH_L_FEATURE_DATA));
                        send.data()->wProbeNo = frameMax.data()->wProbeNo[j];
                        send.data()->wLightEye = m_curEyeAreaSingle[j];
                        UTSharedCommuSegment segment = UTSharedCommuSegment::create();
                        segment->append(send);
                        emit CscanDataRead(segment);

                        m_curEyeAreaSingle[j] = usEyeArea;
                        memset(&m_frameMax.data()->stData[j], 0, sizeof(CH_L_FEATURE_DATA));

                        if(nFrameRead < nFrameNum) // 没有判断完（数据段跨两个光眼以上）
                        {
                            GetMaxFeatureFrame(segmentIn, nFrameNum, nFrameRead, m_frameMax);
                        }
                    }
                }
            }
        }

        nFrameRead++;

        if(nFrameRead >= nFrameNum)
        {
            return false;
        }
    }

    return false;
}

//#if SENSOR_TWO_WORK
/*!
 * \brief AECWork::CalcMaxFrameTwoSensor
 * 双区激励计算最大帧
 */
void AECWork::CalcMaxFrameTwoSensor(UTSharedFeatureSegment segmentIn, int idx, int chnIdx,
                                    UTSharedFeatureFrame frameMax, UTSharedFeatureFrame frameMax1)
{
    FEATURE_DATA feature;
    memcpy(&feature, &segmentIn.data()->featurs[idx], sizeof(FEATURE_DATA));
    UTSharedFeatureFrame frameTemp = UTSharedFeatureFrame::create();

    quint16 usSoftIdx = GetSoftChnIdx(feature.wSoftChannelIndex[chnIdx / 4], chnIdx);
    if(m_usSoftIdxSingle[chnIdx] == usSoftIdx)
    {
        frameTemp = frameMax;
        m_usSoftIdxSingle[chnIdx] = usSoftIdx;
    }
    else if(m_usSoftIdxSingle1[chnIdx] == usSoftIdx)
    {
        frameTemp = frameMax1;
        m_usSoftIdxSingle1[chnIdx] = usSoftIdx;
    }
    else
    {
        if(m_usSoftIdxSingle[chnIdx] == -1)
        {
            frameTemp = frameMax;
            m_usSoftIdxSingle[chnIdx] = usSoftIdx;
        }
        else if(m_usSoftIdxSingle1[chnIdx] == -1)
        {
            frameTemp = frameMax1;
            m_usSoftIdxSingle1[chnIdx] = usSoftIdx;
        }
        else
        {
            ;//
        }
    }

    // 闸门A的最大幅值
    quint16 usHeightA = feature.stData[chnIdx].stFlawFeature.stGateA.wFlawHeight;
    quint16 usHeightMaxA = frameTemp.data()->stData[chnIdx].stFlawFeature.stGateA.wFlawHeight;

    if(usHeightA > usHeightMaxA)
    {
        frameTemp.data()->stData[chnIdx].stFlawFeature.stGateA.wFlawHeight = usHeightA;
        quint16 usFlawTime = feature.stData[chnIdx].stFlawFeature.stGateA.wFlawTime;
        frameTemp.data()->stData[chnIdx].stFlawFeature.stGateA.wFlawTime = usFlawTime;
        //memcpy(&frameMax.data()->stData[j], &feature.stData[j], sizeof(CH_L_FEATURE_DATA));
    }

    // 闸门B的最大幅值
    quint16 usHeightB = feature.stData[chnIdx].stFlawFeature.stGateB.wFlawHeight;
    quint16 usHeightMaxB = frameTemp.data()->stData[chnIdx].stFlawFeature.stGateB.wFlawHeight;

    if(usHeightB > usHeightMaxB)
    {
        frameTemp.data()->stData[chnIdx].stFlawFeature.stGateB.wFlawHeight = usHeightB;
        quint16 usFlawTime = feature.stData[chnIdx].stFlawFeature.stGateB.wFlawTime;
        frameTemp.data()->stData[chnIdx].stFlawFeature.stGateB.wFlawTime = usFlawTime;
    }

    frameTemp.data()->wProbeNo[chnIdx] = feature.wProbeNo[chnIdx];
    quint16 usEye = feature.wLightEye[chnIdx / 2];
    if(chnIdx % 2 == 0) // 设置低8位
    {
        usEye = (usEye & 0xFF00) | GetEyeLowOrHi(usEye, chnIdx);

    }
    else    // 设置高8位
    {
        usEye = (usEye & 0xFF) | (GetEyeLowOrHi(usEye, chnIdx) << 8);
    }
    frameTemp.data()->wLightEye[chnIdx / 2] = usEye;

    quint16 usSoftChnNum = feature.wSoftChannelIndex[chnIdx / 4];
    switch (chnIdx % 4)
    {
        case 0:
            usSoftChnNum = (usSoftChnNum & 0xFFF0) | GetSoftChnIdx(usSoftChnNum, chnIdx);    // 设置低4位
            break;
        case 1:
            usSoftChnNum = (usSoftChnNum & 0xFF0F) | (GetSoftChnIdx(usSoftChnNum, chnIdx) << 4);    // 设置次低4位
            break;
        case 2:
            usSoftChnNum = (usSoftChnNum & 0xF0FF) | (GetSoftChnIdx(usSoftChnNum, chnIdx) << 8);    // 设置次高4位
            break;
        case 3:
            usSoftChnNum = (usSoftChnNum & 0xFFF) | (GetSoftChnIdx(usSoftChnNum, chnIdx) << 12);    // 设置次高4位
            break;
        default:
            break;
    }
    frameTemp.data()->wSoftChannelIndex[chnIdx / 4] = usSoftChnNum;

    return;
}

/*!
 * \brief AECWork::CalcMaxFrameTwoSensor
 * 双区激励计算完最大帧后发送最大帧
 * \param nFrameRead:读取第几帧
 * \param nFrameNum:总共帧数
 */
void AECWork::EmitMaxFrameTwoSensor(UTSharedFeatureSegment segmentIn, int idx,  int chnIdx, int nFrameRead, int nFrameNum,
                                    UTSharedFeatureFrame frameMax, UTSharedFeatureFrame frameMax1)
{
    UTSharedFeatureFrame frameTemp = UTSharedFeatureFrame::create();
    if(m_usSoftIdxSingle[chnIdx] > m_usSoftIdxSingle1[chnIdx])
    {
        frameTemp = frameMax1;
        m_usSoftIdxSingle1[chnIdx] = -1;
    }
    else
    {
        frameTemp = frameMax;
        m_usSoftIdxSingle[chnIdx] = -1;
    }

    UTSharedFeatureCommu send = UTSharedFeatureCommu::create();
    memcpy(&send.data()->stData, &frameTemp.data()->stData[chnIdx], sizeof(CH_L_FEATURE_DATA));
    send.data()->wProbeNo = frameTemp.data()->wProbeNo[chnIdx];
    send.data()->wLightEye = GetEyeLowOrHi(frameTemp.data()->wLightEye[chnIdx / 2], chnIdx);
    send.data()->wSoftChannelIndex = GetSoftChnIdx(frameTemp.data()->wSoftChannelIndex[chnIdx / 4], chnIdx);
    UTSharedCommuSegment segment = UTSharedCommuSegment::create();
    segment->append(send);
    emit CscanDataRead(segment);

    m_curEyeAreaSingle[chnIdx] = GetEyeLowOrHi(segmentIn.data()->featurs[idx].wLightEye[chnIdx / 2], chnIdx);
    memset(&frameTemp.data()->stData[idx], 0, sizeof(CH_L_FEATURE_DATA));

    if(nFrameRead < nFrameNum) // 没有判断完（数据段跨两个光眼以上）
    {
        GetMaxFeatureFrame(segmentIn, nFrameNum, nFrameRead, frameMax, frameMax1);
    }

    return;
}

bool AECWork::GetMaxFeatureFrame(UTSharedFeatureSegment segmentIn,
                                 qint32 nFrameNum, qint32 &nFrameRead,
                                 UTSharedFeatureFrame frameMax, UTSharedFeatureFrame frameMax1)
{
    for(int i = nFrameRead; i < nFrameNum; i++)
    {
        for(int j = 0; j < AEC_HARD_CHN_NUM; j++)
        {
            if(segmentIn.data()->featurs[i].wProbeNo[j] != 0)
            {
                quint16 usEyeArea = GetEyeLowOrHi(segmentIn.data()->featurs[i].wLightEye[j / 2], j);
#if CSCAN_DELETE_FRAMS
                if(usEyeArea == 0)
                {
                    m_usSensorSingle[j] = 0;
                }
                if(m_usSensorSingle[j] == usEyeArea)
                {
                    if(m_bDeleteFrames)
                    {
                        if(m_nDeleteNum > 0)
                        {
                            m_nDeleteNum--;
                            nFrameRead++;
                            break;
                        }
                        else
                        {
                            m_bDeleteFrames = false;
                        }
                    }
                }
                else
                {
                    m_bDeleteFrames = true;
                    m_nDeleteNum = platform()->GetDeleteFrameNum();
                    m_usSensorSingle[j] = usEyeArea;
                }
#endif
                if(m_curEyeAreaSingle[j] == 0)
                {
                    if(m_curEyeAreaSingle[j] == usEyeArea
                            || m_curEyeAreaSingle[j] + 1 == usEyeArea)
                    {
                        CalcMaxFrameTwoSensor(segmentIn, i, j, frameMax, frameMax1);
                    }
                    else
                    {
                        EmitMaxFrameTwoSensor(segmentIn, i, j, nFrameRead, nFrameNum, frameMax, frameMax1);
                    }
                }
                else
                {
                    if(m_curEyeAreaSingle[j] == usEyeArea)
                    {
                        CalcMaxFrameTwoSensor(segmentIn, i, j, frameMax, frameMax1);
                    }
                    else
                    {
                        EmitMaxFrameTwoSensor(segmentIn, i, j, nFrameRead, nFrameNum, frameMax, frameMax1);
                    }
                }
            }
        }

        nFrameRead++;

        if(nFrameRead >= nFrameNum)
        {
            return false;
        }
    }

    return true;
}
//#endif
#else
//#if SENSOR_TWO_WORK
/*!
 * \brief AECWork::CalcMaxFrameTwoSensor
 * 双区激励计算最大帧
 */
void AECWork::CalcMaxFrameTwoSensor(UTSharedFeatureSegment segmentIn, int idx,
                                    UTSharedFeatureFrame frameMax, UTSharedFeatureFrame frameMax1)
{
    FEATURE_DATA feature;
    memcpy(&feature, &segmentIn.data()->featurs[idx], sizeof(FEATURE_DATA));
    UTSharedFeatureFrame frameTemp = UTSharedFeatureFrame::create();
    if(m_usSoftIdx == feature.wSoftChannelIndex)
    {
        frameTemp = frameMax;
        m_usSoftIdx = feature.wSoftChannelIndex;
    }
    else if(m_usSoftIdx1 == feature.wSoftChannelIndex)
    {
        frameTemp = frameMax1;
        m_usSoftIdx1 = feature.wSoftChannelIndex;
    }
    else
    {
        if(m_usSoftIdx == -1)
        {
            frameTemp = frameMax;
            m_usSoftIdx = feature.wSoftChannelIndex;
        }
        else if(m_usSoftIdx1 == -1)
        {
            frameTemp = frameMax1;
            m_usSoftIdx1 = feature.wSoftChannelIndex;
        }
        else
        {
            ;//
        }
    }

    // 当前光眼区域内有效的探头数（即一块AEC卡内有接探头的硬通道数）
    //QList<qint32> listProbe = platform()->GetProbeNoManager()->EyeArea2ProbeNoList(m_usEyeArea * 16 + m_curEyeArea);
    for(int j = 0; j < 8; j++)
    {
        // 闸门A的最大幅值
        quint16 usHeightA = feature.stData[j].stFlawFeature.stGateA.wFlawHeight;
        quint16 usHeightMaxA = frameTemp.data()->stData[j].stFlawFeature.stGateA.wFlawHeight;

        if(usHeightA > usHeightMaxA)
        {
            frameTemp.data()->stData[j].stFlawFeature.stGateA.wFlawHeight = usHeightA;
            quint16 usFlawTime = feature.stData[j].stFlawFeature.stGateA.wFlawTime;
            frameTemp.data()->stData[j].stFlawFeature.stGateA.wFlawTime = usFlawTime;
            //memcpy(&frameMax.data()->stData[j], &feature.stData[j], sizeof(CH_L_FEATURE_DATA));
        }

        // 闸门B的最大幅值
        quint16 usHeightB = feature.stData[j].stFlawFeature.stGateB.wFlawHeight;
        quint16 usHeightMaxB = frameTemp.data()->stData[j].stFlawFeature.stGateB.wFlawHeight;

        if(usHeightB > usHeightMaxB)
        {
            frameTemp.data()->stData[j].stFlawFeature.stGateB.wFlawHeight = usHeightB;
            quint16 usFlawTime = feature.stData[j].stFlawFeature.stGateB.wFlawTime;
            frameTemp.data()->stData[j].stFlawFeature.stGateB.wFlawTime = usFlawTime;
        }
    }

    memcpy(&frameTemp.data()->wProbeNo[0], &feature.wProbeNo[0], 8 * sizeof(quint16));
    frameTemp.data()->wLightEye = feature.wLightEye;
    frameTemp.data()->wSoftChannelIndex = feature.wSoftChannelIndex;

    return;
}

void AECWork::CalcAvgFrameTwoSensor(UTSharedFeatureSegment segmentIn, int idx,
                                    UTSharedFeatureFrame frameAvg, UTSharedFeatureFrame frameAvg1)
{
    QVector<FEATURE_DATA> vecFeature;
    FEATURE_DATA feature;
    memcpy(&feature, &segmentIn.data()->featurs[idx], sizeof(FEATURE_DATA));
    vecFeature.append(feature);

    for(int i = idx + 1; i < m_avgCount; ++i)
    {
        if(i < C_SCAN_NUM_FRAMES_PER_PACKET)
        {
            FEATURE_DATA featureNext;
            memcpy(&featureNext, &segmentIn->featurs[i], sizeof(FEATURE_DATA));
            if(feature.wLightEye == featureNext.wLightEye)
            {
                if (feature.wSoftChannelIndex == featureNext.wSoftChannelIndex)
                {
                    vecFeature.append(featureNext);
                }
            }
            else
            {
                break;
            }
        }
    }

    UTSharedFeatureFrame frameTemp = UTSharedFeatureFrame::create();
    if(m_usSoftIdx == feature.wSoftChannelIndex)
    {
        frameTemp = frameAvg;
        m_usSoftIdx = feature.wSoftChannelIndex;
    }
    else if(m_usSoftIdx1 == feature.wSoftChannelIndex)
    {
        frameTemp = frameAvg1;
        m_usSoftIdx1 = feature.wSoftChannelIndex;
    }
    else
    {
        if(m_usSoftIdx == -1)
        {
            frameTemp = frameAvg;
            m_usSoftIdx = feature.wSoftChannelIndex;
        }
        else if(m_usSoftIdx1 == -1)
        {
            frameTemp = frameAvg1;
            m_usSoftIdx1 = feature.wSoftChannelIndex;
        }
        else
        {
            ;//
        }
    }

    int count = vecFeature.size();

    for (int j = 0; j < AEC_CHN_NUM; j++)
    {
        quint16 avgHeightA = 0;
        quint16 avgHeightB = 0;

        foreach (FEATURE_DATA feature, vecFeature)
        {
            avgHeightA += feature.stData[j].stFlawFeature.stGateA.wFlawHeight;
            avgHeightB += feature.stData[j].stFlawFeature.stGateB.wFlawHeight;
        }

        avgHeightA = avgHeightA / count;
        avgHeightB = avgHeightB / count;

        quint16 curHeightA = frameTemp->stData[j].stFlawFeature.stGateA.wFlawHeight;
        if (avgHeightA > curHeightA)
        {
            frameTemp.data()->stData[j].stFlawFeature.stGateA.wFlawHeight = avgHeightA;
            quint16 usFlawTime = feature.stData[j].stFlawFeature.stGateA.wFlawTime;
            frameTemp.data()->stData[j].stFlawFeature.stGateA.wFlawTime = usFlawTime;
        }

        quint16 curHeightB = frameTemp->stData[j].stFlawFeature.stGateB.wFlawHeight;
        if(avgHeightB > curHeightB)
        {
            frameTemp.data()->stData[j].stFlawFeature.stGateB.wFlawHeight = avgHeightB;
            quint16 usFlawTime = feature.stData[j].stFlawFeature.stGateB.wFlawTime;
            frameTemp.data()->stData[j].stFlawFeature.stGateB.wFlawTime = usFlawTime;
        }
    }

    memcpy(&frameTemp.data()->wProbeNo[0], &feature.wProbeNo[0], 8 * sizeof(quint16));
    frameTemp.data()->wLightEye = feature.wLightEye;
    frameTemp.data()->wSoftChannelIndex = feature.wSoftChannelIndex;

    return;
}

/*!
 * \brief AECWork::CalcMaxFrameTwoSensor
 * 双区激励计算完最大帧后发送最大帧
 * \param nFrameRead:读取第几帧
 * \param nFrameNum:总共帧数
 */
void AECWork::EmitMaxFrameTwoSensor(UTSharedFeatureSegment segmentIn, int idx, int nFrameRead, int nFrameNum,
                                    UTSharedFeatureFrame frameMax, UTSharedFeatureFrame frameMax1)
{
    UTSharedFeatureFrame frameTemp = UTSharedFeatureFrame::create();
    m_curEyeArea = segmentIn.data()->featurs[idx].wLightEye;
    if (isTwoSensorWork())  // 直探头轮缘卡为单区激励,正负21度为双区激励
    {
        if(m_usSoftIdx > m_usSoftIdx1)
        {
            frameTemp = frameMax1;
            m_usSoftIdx1 = -1;
        }
        else
        {
            frameTemp = frameMax;
            m_usSoftIdx = -1;
        }

        if(m_curEyeArea == 17)  // 2米结束
        {
            m_curEyeArea = 0;
        }
    }
    else
    {
        frameTemp = frameMax;
        m_usSoftIdx = -1;
        if(m_curEyeArea == 16)  // 2米结束
        {
            m_curEyeArea = 0;
        }
    }

    UTSharedFeatureSegment  segmentSend = UTSharedFeatureSegment::create();
    memcpy(&segmentSend.data()->featurs[0], frameTemp.data(), sizeof(FEATURE_DATA));
    memset(frameTemp.data(), 0, sizeof(FEATURE_DATA));
    emit CscanDataRead(segmentSend, 1);
    qDebug() << "EmitMaxFrameTwoSensor: probe: " << segmentSend->featurs[0].wProbeNo[0];

    nFrameRead++;
    if(nFrameRead < nFrameNum) // 没有判断完（数据段跨两个光眼以上）
    {
        return GetMaxFeatureFrame(segmentIn, nFrameNum, nFrameRead, frameMax, frameMax1);
    }

    return;
}

void AECWork::GetMaxFeatureFrame(UTSharedFeatureSegment segmentIn,
                                 qint32 nFrameNum, qint32 &nFrameRead,
                                 UTSharedFeatureFrame frameMax, UTSharedFeatureFrame frameMax1)
{
    for(int i = nFrameRead; i < nFrameNum; i++)
    {
        /*if(m_curEyeArea == -1)
        {
            if(segmentIn.data()->featurs[i].wLightEye == 0)
            {
                memcpy(frameMax.data(), &segmentIn.data()->featurs[i], sizeof(FEATURE_DATA));
                m_curEyeArea = segmentIn.data()->featurs[i].wLightEye;
            }
        }
        else*/
        {
            // 传感器切换时（继电器切换），去掉头几帧, 配置文件配为0时，则不去帧
            if(segmentIn.data()->featurs[i].wLightEye == 0)
            {
                m_usSensor = 0;
            }
            if(m_usSensor == segmentIn.data()->featurs[i].wLightEye)
            {
                if(m_bDeleteFrames)
                {
                    if(m_nDeleteNum > 0)
                    {
                        m_nDeleteNum--;
                        nFrameRead++;
                        continue;
                    }
                    else
                    {
                        m_bDeleteFrames = false;
                    }
                }
            }
            else
            {
                m_bDeleteFrames = true;
                m_nDeleteNum = platform()->GetDeleteFrameNum();
                m_usSensor = segmentIn.data()->featurs[i].wLightEye;
            }

            /*qDebug() << "i: " << i  << "m_usSensor: " << m_usSensor
                     << "wLightEye: " << segmentIn->featurs[i].wLightEye
                       << "wSoftChannelIndex: " << segmentIn->featurs[i].wSoftChannelIndex
                       << "probe: " << segmentIn->featurs[i].wProbeNo[0]
                        << "nFrameRead: " << nFrameRead
                        <<  "nFrameNum: " << nFrameNum
                        << "m_nDeleteNum:" << m_nDeleteNum;*/
            if (isTwoSensorWork())  // 直探头轮缘卡为单区激励,正负21度为双区激励
            {
                if(m_curEyeArea == 0)
                {
                    if(m_curEyeArea == segmentIn.data()->featurs[i].wLightEye
                            || m_curEyeArea + 1 == segmentIn.data()->featurs[i].wLightEye)
                    {
                        CalcMaxFrameTwoSensor(segmentIn, i, frameMax, frameMax1);
                        //CalcAvgFrameTwoSensor(segmentIn, i, frameMax, frameMax1);
                    }
                    else
                    {
                        return EmitMaxFrameTwoSensor(segmentIn, i, nFrameRead, nFrameNum, frameMax, frameMax1);
                    }
                }
            }

            if(m_curEyeArea == segmentIn.data()->featurs[i].wLightEye)
            {
                CalcMaxFrameTwoSensor(segmentIn, i, frameMax, frameMax1);
                //CalcAvgFrameTwoSensor(segmentIn, i, frameMax, frameMax1);
            }
            else
            {
                return EmitMaxFrameTwoSensor(segmentIn, i, nFrameRead, nFrameNum, frameMax, frameMax1);
            }
        }

        nFrameRead++;

        if(nFrameRead >= nFrameNum)
        {
            qDebug() << "nFrameRead >= nFrameNum: " << nFrameRead;
            return;
        }
    }

    return;
}
//#else
bool AECWork::GetMaxFeatureFrame(UTSharedFeatureSegment  segmentIn,
                                 qint32 nFrameNum, qint32 &nFrameRead,
                                 UTSharedFeatureFrame  frameMax)
{
    for(int i = nFrameRead; i < nFrameNum; ++i)
    {
        /*if(m_curEyeArea == -1)
        {
            if(segmentIn.data()->featurs[i].wLightEye == 0)
            {
                memcpy(frameMax.data(), &segmentIn.data()->featurs[i], sizeof(FEATURE_DATA));
                m_curEyeArea = segmentIn.data()->featurs[i].wLightEye;
            }
        }
        else*/
        {
            if(m_curEyeArea == segmentIn.data()->featurs[i].wLightEye)
            {
                // 当前光眼区域内有效的探头数（即一块AEC卡内有接探头的硬通道数）
                //QList<qint32> listProbe = platform()->GetProbeNoManager()->EyeArea2ProbeNoList(m_usEyeArea * 16 + m_curEyeArea);
                if(m_bDeleteFrames)
                {
                    if(m_nDeleteNum > 0)
                    {
                        m_nDeleteNum--;
                        nFrameRead++;
                        break;
                    }
                    else
                    {
                        m_bDeleteFrames = false;
                    }
                }

                for(int j = 0; j < 8; ++j)
                {
                    // 闸门A的最大幅值
                    quint16 usHeightA = segmentIn.data()->featurs[i].stData[j].stFlawFeature.stGateA.wFlawHeight;
                    quint16 usHeightMaxA = frameMax.data()->stData[j].stFlawFeature.stGateA.wFlawHeight;

                    if(usHeightA > usHeightMaxA)
                    {
                        frameMax.data()->stData[j].stFlawFeature.stGateA.wFlawHeight = usHeightA;
                        quint16 usFlawTime = segmentIn.data()->featurs[i].stData[j].stFlawFeature.stGateA.wFlawTime;
                        frameMax.data()->stData[j].stFlawFeature.stGateA.wFlawTime = usFlawTime;
                        //memcpy(&frameMax.data()->stData[j], &segmentIn.data()->featurs[i].stData[j], sizeof(CH_L_FEATURE_DATA));
                    }

                    // 闸门B的最大幅值
                    quint16 usHeightB = segmentIn.data()->featurs[i].stData[j].stFlawFeature.stGateB.wFlawHeight;
                    quint16 usHeightMaxB = frameMax.data()->stData[j].stFlawFeature.stGateB.wFlawHeight;

                    if(usHeightB > usHeightMaxB)
                    {
                        frameMax.data()->stData[j].stFlawFeature.stGateB.wFlawHeight = usHeightB;
                        quint16 usFlawTime = segmentIn.data()->featurs[i].stData[j].stFlawFeature.stGateB.wFlawTime;
                        frameMax.data()->stData[j].stFlawFeature.stGateB.wFlawTime = usFlawTime;
                    }
                }

                memcpy(&frameMax.data()->wProbeNo[0], &segmentIn.data()->featurs[i].wProbeNo[0],  8 * sizeof(quint16));
                frameMax.data()->wLightEye = segmentIn.data()->featurs[i].wLightEye;
#if FLANGE_ANALYSIS
                // B闸门波宽不取，只有轮缘数据才有用，也不取时间戳，取最大帧时间戳为0与直探头I2触发上传区分开（时间戳从1计起）
                //frameMax.data()->wTimeStampL = segmentIn.data()->featurs[i].wTimeStampL;
                //frameMax.data()->wTimeStampH = segmentIn.data()->featurs[i].wTimeStampH;
#endif
            }
            else
            {
                m_bDeleteFrames = true;
                m_nDeleteNum = platform()->GetDeleteFrameNum();

                m_curEyeArea = segmentIn.data()->featurs[i].wLightEye;
                UTSharedFeatureSegment  segmentSend = UTSharedFeatureSegment::create();
                memcpy(&segmentSend.data()->featurs[0], frameMax.data(), sizeof(FEATURE_DATA));
                memset(frameMax.data(), 0, sizeof(FEATURE_DATA));
                emit CscanDataRead(segmentSend, 1);

                if(nFrameRead < nFrameNum) // 没有判断完（数据段跨两个光眼以上）
                {
                    GetMaxFeatureFrame(segmentIn, nFrameNum, nFrameRead, frameMax);
                }
            }
        }

        nFrameRead++;

        if(nFrameRead >= nFrameNum)
        {
            return false;
        }
    }

    return true;
}
//#endif
#endif

quint16 AECWork::GetEyeLowOrHi(quint16 usEye, int nChnNum)
{
    quint16 usResult = 0;
    if((nChnNum % 2) == 0)    // 取低8位
    {
         usResult = usEye && 0xFF;
    }
    else    // 取高8位
    {
        usResult = usEye >> 8;
    }

    return usResult;
}

quint16 AECWork::GetSoftChnIdx(quint16 usSoftIdx, int nChnNum)
{
    quint16 usResult = 0;
    switch (nChnNum % 4)
    {
        case 0:
            usResult = usSoftIdx && 0xF;    // 取低4位
            break;
        case 1:
            usResult = (usSoftIdx && 0xF0) >> 4;
            break;
        case 2:
            usResult = (usSoftIdx && 0xF00) >> 8;
            break;
        case 3:
            usResult = (usSoftIdx && 0xF000) >> 12;
            break;
        default:
            break;
    }

    return usResult;
}

void AECWork::SetMaxFrameFlag(bool bFlag)
{
    if(m_bMaxFrameFlag != bFlag)
    {
        m_bMaxFrameFlag = bFlag;
        if(m_bMaxFrameFlag) // 开启取最大帧时，当前光眼信号应置为-1
        {
            m_curEyeArea = 0;
            m_usSoftIdx = -1;
            m_usSoftIdx1 = -1;
            for (auto &value : m_curEyeAreaSingle)
            {
                value = 0;
            }
            for (auto &value : m_usSoftIdxSingle)
            {
                value = -1;
            }
            for (auto &value : m_usSoftIdxSingle1)
            {
                value = -1;
            }
            memset(m_frameMax.data(), 0, sizeof(FEATURE_DATA));
            memset(m_frameMax1.data(), 0, sizeof(FEATURE_DATA));
        }
    }

    return;
}

/*!
 * \brief AECWork::ReadEyesStatus
 * \param pParam
 * \return: 返回从当前AEC卡中读取的所有传感器激发状态
 */
UTSharedDataSimulation AECWork::ReadEyesStatus()
{
    UTSharedDataSimulation simulation = UTSharedDataSimulation::create();
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return simulation;
    }

    simulation->usEnable = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, SIMULATE_ENABL_ADDR);

    for (int i = 0; i < LIGHT_EYE_VAR_NUM; ++i) {
        simulation->stLightEye.usLightEye[i] = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, EYE_LIGHT_ADDRESS + i * 2);
    }

    if (8 < LIGHT_EYE_VAR_NUM) { // 8：usLightEye中第9个quint16，对应传感器129，130，131，132+的状态
        simulation->stLightEye.usLightEye[8] = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, 0x58);     // 新加的4个传感器的地址
    }

    return simulation;
}

void AECWork::OnSetScanStutas(qint32 nStataus)
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return;
    }

    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, SOFT_CHANNEL_SELECT_MODE_ADDRESS, nStataus);

    if(nStataus == 0) {
        SetMaxFrameFlag(true);
        unsigned int timeStampResetAddress = TIMESTAMP_RESET;
        if (IsSensorCard()) {
            timeStampResetAddress = SENSOR_TIME_STAMP_RESET;
        }

        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, timeStampResetAddress, 1);
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, timeStampResetAddress, 0);
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, WHEEL_NO_RESET, 1);
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, WHEEL_NO_RESET, 0);
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, SYS_RESET_ADDRESS, 1);
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, SYS_RESET_ADDRESS, 0);
    } else if(nStataus == 1) {
        SetMaxFrameFlag(false);
    }
}

void AECWork::OnSetRepeatFrequence(qint32 nValue)
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return;
    }

    int nRpf = 30e6 / nValue;
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, REPEAT_FREQUENCE_ADDR, nRpf);
}

void AECWork::OnSetSensorRoll(quint16 nValue)
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return;
    }

    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, SENSOR_ROLL_ENABLE_ADDR, nValue);
}


bool AECWork::ReadMonitorData(quint16* pData)
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return false;
    }

    return m_pParam->pPLX->ReadMonitorData(m_pParam->hDev, pData, MONITOR_CHANNEL_DATA_EN, MONITOR_WAVE_DATA_ADDRESS);
}

void AECWork::SetMonitorRange(quint16 *pInterSpace)
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return;
    }

    m_pParam->pPLX->SetMonitorRange(m_pParam->hDev, pInterSpace, MONITOR_RANGE_ADDRESS);
}


void AECWork::OnSetSurfaceTrigger(bool bFlag)
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return;
    }

    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, 0x3c, bFlag);
}

/*!
 * \brief 设置模拟数据MPB_SIMULATE_DATA_ADDR
 */
void AECWork::SetSimulationData(UTSharedDataSimulation simulation)
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return;
    }

    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, SIMULATE_ENABL_ADDR, simulation->usEnable);

//    // 普通方式写入
//    for (int i = 0; i < 4; i++) {
//        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, SIMULATE_DATA_ADDR + i * 2, simulation->stLightEye.usLightEye[i + addressOffset]);
//    }

    /* DMA方式写入,可以避免普通写入所带来的传感器状态混乱的问题 */
    int addressOffset = m_pParam->index * 4;
    P9054_DMAReadWriteBlock(m_pParam->hDev, SIMULATE_DATA_ADDR, simulation->stLightEye.usLightEye + addressOffset, 8, FALSE, P9054_MODE_WORD, P9054_DMA_CHANNEL_0);
}

/*!
 * \brief AECWork::IsStraightFlangeCard 是否是直探头轮缘卡
 * \return bool
 */
bool AECWork::IsStraightFlangeCard()
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return false;
    }

    if (m_pParam->index >= m_sensorCardNum
            && m_pParam->index < (m_sensorCardNum + m_straightFlangeCardNum)) {
        return true;
    } else {
        return false;
    }
}

/*!
 * \brief AECWork::IsStraightFlangeCard 是否是21度卡
 * \return bool
 */
bool AECWork::Is21DegreeCard()
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return false;
    }

    if (m_pParam->index >= (m_sensorCardNum + m_straightFlangeCardNum)
            && m_pParam->index < (m_sensorCardNum
                                  + m_straightFlangeCardNum + m_is21DegreeTwoSensor)) {
        return true;
    } else {
        return false;
    }
}

/*!
 * \brief AECWork::IsSensorCard 是否是传感器卡
 * \return bool
 */
bool AECWork::IsSensorCard()
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return false;
    }

    if (m_pParam->index < m_sensorCardNum) {
        return true;
    } else {
        return false;
    }
}

/*!
 * \brief AECWork::isTwoSensorWork 是否双区激励
 * \return bool
 */
bool AECWork::isTwoSensorWork()
{
    if (IsStraightFlangeCard())  // 直探头轮缘卡为单区激励,正负21度为双区激励
    {
        if (m_isStraightFlangeTwoSensor)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        if (m_is21DegreeTwoSensor)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

void AECWork::SetTCG(qint32 chnNo, TCGCommuData data, TCGData tcgData)
{
    Q_UNUSED(data);

    if (SelectChnInCard(chnNo))
    {
        quint32 addressStart = GetTCGAddress(chnNo);
        if (addressStart)
        {
            quint32 addressCur = 0;
            for (int i = 0; i < TCG_RAM_SIZE; ++i)
            {
                addressCur = addressStart + 0x2 * i;
                if (m_pParam.isNull() || !m_pParam->hDev) {
                    continue;
                }

                P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0,
                                addressCur, Gain2Data(tcgData.value(i)));
            }
        }
    }
}

quint32 AECWork::GetTCGAddress(qint32 chnNo)
{
    qint32 hardChnNo = platform()->GetProbeNoManager()->ProbNo2CardChnNo(chnNo);
    quint32 address = CHANNEL1_TCG_DATA_ADDRESS + hardChnNo * 0x1000;
/*    qint32 softChnNo = GetSoftChnNo(chnNo);
    if ((hardChnNo == -1) || (softChnNo == -1))
    {
        return 0;
    }

    quint32 address = CHANNEL1_TCG_DATA_ADDRESS + hardChnNo * 0x1000 + softChnNo * 0x80;
*/
    return address;
}

qint32 AECWork::GetSoftChnNo(qint32 chnNo)
{
    if (m_pParam.isNull() || !m_pParam->hDev) {
        return 0;
    }

    if (m_pParam->index < platform()->GetSensorCardNum()) {
        return platform()->GetProbeNoManager()->ProbNo2SoftChnNo(chnNo, ProbeConnectManager::SINGLE);
    } else {
        return platform()->GetProbeNoManager()->ProbNo2SoftChnNo(chnNo, ProbeConnectManager::DOUBLE);
    }
}
