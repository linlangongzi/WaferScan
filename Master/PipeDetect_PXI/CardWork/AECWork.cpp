#include "time.h"
#include "const.h"
#include "AECWork.h"
#include "windows.h"
#include "p9054_lib.h"
#include "plx_lib_x.h"
#include "HardwareAddress.h"
#include "Platform/Platform.h"
#include "TcpServer/UTSetupSetOne.h"
#include "ConfigObject/UTInstrument.h"
#include "CardWork/PLXCommunication.h"
#include "ProbeConnectManager/ProbeConnectManager.h"
#include <QTime>
#include <QFile>
#include <QThread>
#include <QJsonArray>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>

using namespace NS_PXI100;

AECWork::AECWork(QObject *parent) :
    CardWorkBase(parent),
    m_mark(0),
    setAllCount(-1)
{
    InitChannelMap();
    m_pParam = SharedCardParam::create();
    m_probeNoManager = platform()->GetProbeNoManager();
    m_frameMax = UTSharedFeatureFrame::create();
    m_vectorMonitorframe.clear();

    for (int i = 0; i < MONITOR_BUFFER_COUNT; i++)
    {
        m_vectorMonitorframe.push_back(UTSharedMonitorFrame::create());
    }
}

/*!
 * \brief AECWork::HardChannelAddress 根据监视数据缓冲区序号计算缓冲区地址
 * \param hardIndex 缓冲区序号
 * \return 缓冲区地址
 */
quint32 AECWork::HardChannelDataAddress(quint32 hardIndex)
{
    switch (hardIndex)
    {
    case 0:     return MONITOR_WAVE_DATA_ADDRESS_1;
    case 1:     return MONITOR_WAVE_DATA_ADDRESS_2;
    case 2:     return MONITOR_WAVE_DATA_ADDRESS_3;
    case 3:     return MONITOR_WAVE_DATA_ADDRESS_4;
    case 4:     return MONITOR_WAVE_DATA_ADDRESS_5;
    case 5:     return MONITOR_WAVE_DATA_ADDRESS_6;
    case 6:     return MONITOR_WAVE_DATA_ADDRESS_7;
    case 7:     return MONITOR_WAVE_DATA_ADDRESS_8;
    default:    return MONITOR_WAVE_DATA_ADDRESS_1;
    }
}

quint32 AECWork::HardChannelRangeAddress(quint32 hardIndex)
{
    switch (hardIndex)
    {
    case 0:     return MONITOR_RANGE_ADDRESS_1;
    case 1:     return MONITOR_RANGE_ADDRESS_2;
    case 2:     return MONITOR_RANGE_ADDRESS_3;
    case 3:     return MONITOR_RANGE_ADDRESS_4;
    case 4:     return MONITOR_RANGE_ADDRESS_5;
    case 5:     return MONITOR_RANGE_ADDRESS_6;
    case 6:     return MONITOR_RANGE_ADDRESS_7;
    case 7:     return MONITOR_RANGE_ADDRESS_8;
    default:    return MONITOR_RANGE_ADDRESS_1;
    }
}

/*!
 * \brief AECWork::GateEigenvalueAddressStart
 * \param bufferIndex
 * \return 闸门特征值缓冲区起始地址
 */
quint32 AECWork::GateEigenvalueAddressStart(quint32 bufferIndex)
{
    switch (bufferIndex)
    {
    case 0:     return MONITOR_FEATURE_DATA1_ADDRESS;
    case 1:     return MONITOR_FEATURE_DATA2_ADDRESS;
    case 2:     return MONITOR_FEATURE_DATA3_ADDRESS;
    case 3:     return MONITOR_FEATURE_DATA4_ADDRESS;
    case 4:     return MONITOR_FEATURE_DATA5_ADDRESS;
    case 5:     return MONITOR_FEATURE_DATA6_ADDRESS;
    case 6:     return MONITOR_FEATURE_DATA7_ADDRESS;
    case 7:     return MONITOR_FEATURE_DATA8_ADDRESS;
    default:    return MONITOR_FEATURE_DATA1_ADDRESS;
    }
}

quint32 AECWork::GetTCGAddress(quint32 channel)
{
    switch (channel)
    {
    case 0: return SOFT_CHANNEL1_TCG_DATA_ADDRESS;
    case 1: return SOFT_CHANNEL2_TCG_DATA_ADDRESS;
    case 2: return SOFT_CHANNEL3_TCG_DATA_ADDRESS;
    case 3: return SOFT_CHANNEL4_TCG_DATA_ADDRESS;
    case 4: return SOFT_CHANNEL5_TCG_DATA_ADDRESS;
    case 5: return SOFT_CHANNEL6_TCG_DATA_ADDRESS;
    case 6: return SOFT_CHANNEL7_TCG_DATA_ADDRESS;
    case 7: return SOFT_CHANNEL8_TCG_DATA_ADDRESS;
    case 8: return SOFT_CHANNEL9_TCG_DATA_ADDRESS;
    case 9: return SOFT_CHANNEL10_TCG_DATA_ADDRESS;
    case 10: return SOFT_CHANNEL11_TCG_DATA_ADDRESS;
    case 11: return SOFT_CHANNEL12_TCG_DATA_ADDRESS;
    case 12: return SOFT_CHANNEL13_TCG_DATA_ADDRESS;
    case 13: return SOFT_CHANNEL14_TCG_DATA_ADDRESS;
    case 14: return SOFT_CHANNEL15_TCG_DATA_ADDRESS;
    case 15: return SOFT_CHANNEL16_TCG_DATA_ADDRESS;
    case 16: return SOFT_CHANNEL17_TCG_DATA_ADDRESS;
    case 17: return SOFT_CHANNEL18_TCG_DATA_ADDRESS;
    case 18: return SOFT_CHANNEL19_TCG_DATA_ADDRESS;
    case 19: return SOFT_CHANNEL20_TCG_DATA_ADDRESS;
    case 20: return SOFT_CHANNEL21_TCG_DATA_ADDRESS;
    case 21: return SOFT_CHANNEL22_TCG_DATA_ADDRESS;
    case 22: return SOFT_CHANNEL23_TCG_DATA_ADDRESS;
    case 23: return SOFT_CHANNEL24_TCG_DATA_ADDRESS;
    case 24: return SOFT_CHANNEL25_TCG_DATA_ADDRESS;
    case 25: return SOFT_CHANNEL26_TCG_DATA_ADDRESS;
    case 26: return SOFT_CHANNEL27_TCG_DATA_ADDRESS;
    case 27: return SOFT_CHANNEL28_TCG_DATA_ADDRESS;
    case 28: return SOFT_CHANNEL29_TCG_DATA_ADDRESS;
    case 29: return SOFT_CHANNEL30_TCG_DATA_ADDRESS;
    case 30: return SOFT_CHANNEL31_TCG_DATA_ADDRESS;
    case 31: return SOFT_CHANNEL32_TCG_DATA_ADDRESS;
    default: return SOFT_CHANNEL1_TCG_DATA_ADDRESS;
    }
}

void AECWork::InitChannelInfo()
{
    quint32 addressOffset = 0;
    for (int hardChannel = 0; hardChannel < 8; ++hardChannel) {
        addressOffset = (hardChannel + 1) * 0x1000;
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, addressOffset + 0x25C, hardChannel * 4 + 0);
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, addressOffset + 0x2DC, hardChannel * 4 + 1);
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, addressOffset + 0x35C, hardChannel * 4 + 2);
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, addressOffset + 0x3DC, hardChannel * 4 + 3);
    }
}

/*!
 * \brief AECWork::初始化AEC卡
 * \param pParam
 * \return
 */
bool AECWork::InitCard()
{
    if (m_pParam->hDev != NULL) {
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, SYS_RESET_ADDRESS, 1);
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, SYS_RESET_ADDRESS, 0);
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, 0xFFFF);
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, 0);

        return true;
    }

    return false;
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

        for (int bufferIndex = 0; bufferIndex < 8; ++bufferIndex)
        {
            SetMonitorRange(nInterSpace, bufferIndex);
        }

        return true;
    }
    catch(QString exception)
    {
        QMessageBox::about(NULL,"AECCardStart异常",exception);
        return false;
    }
}

/*!
 * \brief AECWork::读取AEC监视通道数据和闸门监视数据
 * \param pParam
 * \return
 */
bool AECWork::CardReading()
{
    if (isSetOne) {
        SetOne();
        isSetOne = false;
    }

    quint32 vectorSize = m_vectorMonitorframe.size();

    for (quint16 hardChannel = 0; hardChannel < vectorSize; ++hardChannel) {   // 轮寻 8个监视数据缓冲区
        if (ReadMonitorData(m_vectorMonitorframe[hardChannel]->nPicData, HardChannelDataAddress(hardChannel), hardChannel)) {
            quint32 address = GateEigenvalueAddressStart(hardChannel);

            m_vectorMonitorframe[hardChannel]->stFeatureData.gateIMPTime = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, address);
            m_vectorMonitorframe[hardChannel]->stFeatureData.gateIMPAmp = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, address + 0x2);
            m_vectorMonitorframe[hardChannel]->stFeatureData.gateAMPTime = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, address + 0x4);
            m_vectorMonitorframe[hardChannel]->stFeatureData.gateAMPAmp = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, address + 0x6);
            m_vectorMonitorframe[hardChannel]->stFeatureData.gateBMPTime = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, address + 0x8);
            m_vectorMonitorframe[hardChannel]->stFeatureData.gateBMPAmp = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, address + 0xa);
            m_vectorMonitorframe[hardChannel]->stFeatureData.gateI2MPTime = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, address + 0xc);
            m_vectorMonitorframe[hardChannel]->stFeatureData.gateI2MPAmp = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, address + 0xe);

            quint16 channelIndex = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, address + 0x10);
            m_vectorMonitorframe[hardChannel]->stFeatureData.probeNo = channelIndex;
            m_vectorMonitorframe[hardChannel]->stFeatureData.coderXLow = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, 0x22);
            m_vectorMonitorframe[hardChannel]->stFeatureData.coderXHigh = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, 0x24);
            m_vectorMonitorframe[hardChannel]->stFeatureData.coderRLow = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, 0x34);
            m_vectorMonitorframe[hardChannel]->stFeatureData.coderRHigh = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, 0x36);

            CalChannelIndex(channelIndex, hardChannel);

            if (channelIndex < 30) {
                emit MonitorDataRead(m_vectorMonitorframe[hardChannel], channelIndex);
            }
        }
    }

    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, 0xFF);
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, 0);

    // 读取伤数据
    ReadFlawResult();
    return true;
}

/*!
 * \brief AECWork::读取C扫特征数据
 * \param pParam
 * \param nCardIndex
 * \return
 */
bool AECWork::ReadFlawResult()
{
    quint32 dwAddress = 0x0;                              // 缓冲区地址
    quint16 addressIndex = 0x0;                           // 缓冲区序号

    if ( (P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_EN_FLAG_ADDRESS) & 0x1) == 1) { // 有效
        addressIndex = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_INDEX_ADDRESS);    // 读取buffer序号
        addressIndex &= 0x3;
    } else {
        return false;
    }

    if (addressIndex == 1) {
        dwAddress = FEATURE_BUFFER1_ADDRESS; // 1号缓冲区
    } else if (addressIndex == 2) {
        dwAddress = FEATURE_BUFFER2_ADDRESS; // 2号缓冲区
    } else {
        return false;
    }

    UTSharedFeatureSegment segment = UTSharedFeatureSegment::create();

#ifdef PIPE_DETECT
//    quint16 size = sizeof(FEATURE_DATA) / sizeof(WORD);
//    for (int i = 0; i < 128; ++i) {
//        FEATURE_DATA data;
//        WORD *wData = (WORD*)(&data);
//        for (int j = 0; j < size; ++j) {
//            *wData = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, dwAddress);
//            wData++;
//            dwAddress += 0x2;
//        }

//        memcpy(&(segment->featurs[i]), &data, sizeof(FEATURE_DATA));
//    }

    BOOL bRet = P9054_DMAReadWriteBlock(m_pParam->hDev, dwAddress, segment->featurs, CSCAN_DATA_DMA_SIZE, TRUE, P9054_MODE_WORD, P9054_DMA_CHANNEL_0);
#else
    BOOL bRet = P9054_DMAReadWriteBlock(m_pParam->hDev, dwAddress, segment->featurs, CSCAN_DATA_DMA_SIZE, TRUE, P9054_MODE_WORD, P9054_DMA_CHANNEL_1);
    if(!bRet) {
        qWarning() << "DMARead CScan data failed";
        return false;
    }
#endif

    // 缓冲区复位
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_FLAG_ADDRESS, PCI_SAMPLE_RESET_ON);
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_FLAG_ADDRESS, PCI_SAMPLE_RESET_OFF);

    for (int i = 0; i < C_SCAN_NUM_FRAMES_PER_PACKET; ++i) {
        CalChannelIndexForCData(segment->featurs[i].wCh1ProbeNo, 0);
        CalChannelIndexForCData(segment->featurs[i].wCh2ProbeNo, 1);
        CalChannelIndexForCData(segment->featurs[i].wCh3ProbeNo, 2);
        CalChannelIndexForCData(segment->featurs[i].wCh4ProbeNo, 3);
        CalChannelIndexForCData(segment->featurs[i].wCh5ProbeNo, 4);
        CalChannelIndexForCData(segment->featurs[i].wCh6ProbeNo, 5);
        CalChannelIndexForCData(segment->featurs[i].wCh7ProbeNo, 6);
        CalChannelIndexForCData(segment->featurs[i].wCh8ProbeNo, 7);
    }

    emit CscanDataRead(segment, C_SCAN_NUM_FRAMES_PER_PACKET);  // 发送给CSCanSocket
    return true;
}

/*!
 * \brief AECWork::通道选择
 * \param nChnNo 探头号
 * \param nSurOrFlaw 伤波或底波
 */
void AECWork::SetSelectChannel(qint32 nChnNo, qint32 nSurOrFlaw)
{
    quint16 probeNo = mapChannel2Probe.value(nChnNo);
    m_pParam->wSelChannel = probeNo;
    m_pParam->nFlawOrBottom = nSurOrFlaw;
    qint32 nCardChnNo = platform()->GetProbeNoManager()->ProbNo2CardChnNo(probeNo);
    qint32 nSoftChnNo = platform()->GetProbeNoManager()->ProbNo2SoftChnNo(probeNo, ProbeConnectManager::DOUBLE);

    if((nCardChnNo == -1) || (nSoftChnNo == -1))    // 卡号或软通道号有误
    {
        return;
    }

    quint16 wValueH = static_cast<quint16>((probeNo - 1) / 4 * 2 + nSurOrFlaw);
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_CHANNEL_H_ADDRESS, wValueH);

    quint16 wValueS = static_cast<quint16>(nSoftChnNo);  //  软通道号
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_CHANNEL_S_ADDRESS, wValueS);

    //A扫数据清零
    quint16 wValue = PCI_SAMPLE_RESET_ON;
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);

    wValue = PCI_SAMPLE_RESET_OFF;
    P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);

    return;
}

/*!
 * \brief 设置指定通道号参数
 * \param param
 * \param nChnNo
 */
void AECWork::OnSetOne(UTSharedDataParam param, qint32 nChnNo)
{
    config = *param.data();
    channel = nChnNo;
    isSetOne = true;

    if (setAllCount > 0) {
        setAllCount--;
        SetOne();
    }
}

void AECWork::SetOne()
{
    quint16 nInterSpace[FLAW_DOT_MAX_NO];
    quint16 probeNo = mapChannel2Probe.value(channel);
    qint32 hardChnNo = platform()->GetProbeNoManager()->ProbNo2CardChnNo(probeNo);
    qint32 softChnNo = platform()->GetProbeNoManager()->ProbNo2SoftChnNo(probeNo, ProbeConnectManager::HALF_SINGLE);  // 4个软通道（两个前置卡串联）

    RangeProcess(config.Velocity, config.Range, nInterSpace);   // 根据声速、声程计算通道采样间隔
    SetMonitorRange(nInterSpace, hardChnNo);                               // 设置采样间隔

    CH_CONFIG_HARD stConfig;
    memset(&stConfig, 0, sizeof(stConfig));
    Params2HardParam(stConfig, probeNo);                          // 将上位机设置的参数结构改型为下发给仪器的数据结构

    if(hardChnNo == -1 || softChnNo == -1) {
        return;
    }

    // 8个硬通道0x01000, 0x02000, 0x03000, 0x04000, 0x05000, 0x06000, 0x07000, 0x08000
    quint16 wValueH = static_cast<quint16>(hardChnNo);  // 0开始
    quint16 wValueS = static_cast<quint16>(softChnNo);  // 0开始
    quint32 dwAddrStart = SOFT_CHANNEL1_CONFIG_PARARM_ADDRESS + wValueH * 0x01000 + wValueS * 0x80;

    qint32 i = 0;
    quint32 dwAddr = dwAddrStart;
    int nEndAddr = sizeof(CH_CONFIG_HARD) - 1;
    quint16 *pData = reinterpret_cast<quint16*>(&stConfig);

    while(dwAddr < dwAddrStart + nEndAddr) {  // 参数个数
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, dwAddr, pData[i]);
        dwAddr += 2;
        i++;
    }

    //    P9054_DMAReadWriteBlock(m_pParam->hDev, dwAddrStart, pData, sizeof(CH_CONFIG_HARD) / sizeof(WORD) * 2, FALSE, P9054_MODE_WORD, P9054_DMA_CHANNEL_0);
}

/*!
 * \brief AECWork::SetWaveType 更改硬通道6和7的伤/底波设置
 * \param hard6硬通道6
 * \param hard7硬通道7
 */
void AECWork::SetWaveType(quint16 mark)
{
    if (m_pParam->hDev) {
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, MONITOR_CHANNEL_H_ADDRESS, mark);
    } else {
        qDebug() << "AECWork::SetWaveType(): m_pParam->hDev is null";
    }
}

void AECWork::SetProbeBoxSimulationData(bool isOpen)
{
    if (isOpen) {
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, PROBE_BOX_DEBUG_ENABLE, 1);
//        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, PROBE_BOX_DEBUG_DISENABLE, 0);
    } else {
//        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, PROBE_BOX_DEBUG_ENABLE, 0);
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, PROBE_BOX_DEBUG_DISENABLE, 0);
    }
}

void AECWork::SetTCG(quint32 channel, TCGCommuData data1, TCGData data2)
{
    quint16 probeNo = mapChannel2Probe.value(channel);
    if (probeNo > 31) {
        return;
    }

    quint32 addressStart = GetTCGAddress(probeNo);
    quint32 addressCur = 0;

    for (int index = 0; index < 256; index++)
    {
        addressCur = addressStart + 0x2 * index;
        P9054_WriteWord(m_pParam->hDev, P9054_ADDR_SPACE0, addressCur, Gain2Data(data2.value(index)));
    }
}

void AECWork::OnConnectionChanged(bool isConnect)
{
    if (isConnect) {
        setAllCount = 32; // 一个AEC卡，算伤波底波共32个通道
    }
}

/*!
 * \brief AECWork::通道参数转换成配置硬件的参数
 * \param param
 * \param stConfig
 */
void AECWork::Params2HardParam(CH_CONFIG_HARD &stConfig, quint16 probeNo)
{
    stConfig.stPlus.wAPlus = GAIN_AB_CONST;
    stConfig.stPlus.wBPlus = GAIN_AB_CONST;
#ifdef PIPE_DETECT
    if (config.Plus >= GAIN_MAX) {
        stConfig.stPlus.wCPlus = Gain2Data(GAIN_MAX);
        stConfig.stPlus.wDPlus = Gain2Data(config.Plus - GAIN_MAX);
    } else {
        stConfig.stPlus.wCPlus = Gain2Data(config.Plus);
        stConfig.stPlus.wDPlus = Gain2Data(GAIN_MIN);
    }
#else
    stConfig.stPlus.wCPlus = Gain2Data(config.Plus / 2);
    stConfig.stPlus.wDPlus = Gain2Data(config.Plus / 2);
#endif
    stConfig.wBottomFlawPlusDiff = Gain2Data(config.CPlus);
    stConfig.wTCGEn = static_cast<quint16>(config.TCGEnable);
    stConfig.probeNo = static_cast<quint16>(probeNo);

    for(qint32 i = 0; i < SUB_CONFIG_NUM; i++)
    {
        quint32 nDelay = config.stWaveConfigure[i].Delay * ADFREQUENCY;
        stConfig.stWaveConfigure[i].delay = static_cast<quint16>(nDelay & 0xFFFF);
        stConfig.stWaveConfigure[i].suppression = static_cast<quint16>(config.stWaveConfigure[i].Suppression * 0.01 * MAX_HEIGHT);
        stConfig.stWaveConfigure[i].detectMode = static_cast<quint16>(config.stWaveConfigure[i].DetectMode);
        stConfig.stWaveConfigure[i].middleValue = static_cast<quint16>(config.stWaveConfigure[i].MiddleValue);

        for (quint16 gateIndex = 0; gateIndex < 4; gateIndex++) // 4个闸门参数设置
        {
            quint32 nGtatStart = config.stWaveConfigure[i].stGate[gateIndex].GateStart * ADFREQUENCY;

            if (gateIndex == 0 || gateIndex == 3) {
                nGtatStart = nGtatStart - nDelay;
            }

            stConfig.stWaveConfigure[i].stGate[gateIndex].GateEnable =
                    static_cast<quint16>(config.stWaveConfigure[i].stGate[gateIndex].GateEnable);
            stConfig.stWaveConfigure[i].stGate[gateIndex].GateStart =
                    static_cast<quint16>(nGtatStart & 0xFFFF);
            stConfig.stWaveConfigure[i].stGate[gateIndex].GateWidth =
                    static_cast<quint16>(config.stWaveConfigure[i].stGate[gateIndex].GateWidth * ADFREQUENCY);
            stConfig.stWaveConfigure[i].stGate[gateIndex].GateAmp =
                    static_cast<quint16>(config.stWaveConfigure[i].stGate[gateIndex].GateAmp * 0.01 * MAX_HEIGHT);
        }
    }
}

/*!
 * \brief AECWork::ReadMonitorData
 * \param pData
 * \param address
 * \return
 */
bool AECWork::ReadMonitorData(quint16* pData, unsigned int address, quint32 bufferIndex)
{
    return m_pParam->pPLX->ReadMonitorData(m_pParam->hDev, pData, MONITOR_CHANNEL_DATA_EN, address, bufferIndex);
}

/*!
 * \brief AECWork::SetMonitorRange
 * \param pInterSpace
 * \param bufferIndex 缓冲区序号0~7
 */
void AECWork::SetMonitorRange(quint16 *pInterSpace, quint32 bufferIndex)
{
    m_pParam->pPLX->SetMonitorRange(m_pParam->hDev, pInterSpace, HardChannelRangeAddress(bufferIndex));
}

/*!
 * \brief AECWork::ValidSoftChannelVector
 * \param vector
 * \param mark
 * 根据软通道标志位获得8个硬通道各自的软通道号vector
 */
void AECWork::ValidSoftChannelVector(AECWork::VectorValidSoftChannel &vector, quint16 mark)
{
    vector.clear();
    for (quint16 byte = 0; byte < 8; ++byte) {
        vector.push_back((0x3 << byte) & mark);
    }
}

bool AECWork::InitChannelMap()
{
    mapProbe2Channel.clear();
    mapChannel2Probe.clear();
    UTInstrument *instrument = dynamic_cast<UTInstrument*>(platform()->Instruments()->GetByID(1));
    if (!instrument) {
        return false;
    }

    QString fileName = QString("./%1%2").arg(instrument->Get("ChannelListFile").toString()).arg(".json");
    QFile file;
    file.setFileName(fileName);

    if (file.open(QIODevice::ReadWrite)) {
        if (file.size() <= 0) {
            file.close();
            return false;
        }

        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll(), &err);

        if (err.error == QJsonParseError::NoError) {
            if (jsonDoc.isObject()) {
                QJsonObject objChannel = jsonDoc.object();
                QJsonArray arrayBox = objChannel["ProbeBoxList"].toArray();
                int boxCount = arrayBox.count();

                for (int boxIndex = 0; boxIndex < boxCount; ++boxIndex) {
                    QJsonObject objBox = arrayBox.at(boxIndex).toObject();
                    QJsonArray arrayProbe = objBox["ProbeList"].toArray();
                    int probeCount = arrayProbe.count();

                    for (int probeIndex = 0; probeIndex < probeCount; probeIndex++ ) {
                        QJsonObject objProbe = arrayProbe.at(probeIndex).toObject();
                        mapChannel2Probe.insert(objProbe["Index"].toInt(), objProbe["ProbeNo"].toInt());
                        mapProbe2Channel.insert(objProbe["ProbeNo"].toInt(), objProbe["Index"].toInt());
                    }
                }
            }

            file.close();
            return true;
        }
    }

    return false;
}

void AECWork::CalChannelIndex(quint16 &probeNo, quint16 hardChannel)
{
    if (mapProbe2Channel.contains(probeNo)) {
        probeNo = mapProbe2Channel.value(probeNo);
    } else {
        probeNo = 45;
    }

    quint16 isFlawMark = P9054_ReadWord(m_pParam->hDev, P9054_ADDR_SPACE0, 0x8);
    bool isFlaw = (isFlawMark >> hardChannel) & 0x1;

    if ((probeNo > 24) && isFlaw) {
        probeNo -= 5;
    }
}

void AECWork::CalChannelIndexForCData(quint16 &probeNo, quint16 hardChannel)
{
    if (mapProbe2Channel.contains(probeNo)) {
        probeNo = mapProbe2Channel.value(probeNo);
    } else {
        probeNo = 45;
    }

    if (probeNo > 24) {
        probeNo -= 5;
    }
}
