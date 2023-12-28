#include "ParameterSocket.h"
#include "Platform/Platform.h"
#include "CardWork/AECWork.h"
#include "CardWork/MFCWork.h"
#include "ConfigObject/UTSetup.h"
#include "TcpServer/UTSetupSetOne.h"
#include <QDebug>

using namespace NS_PXI100;

ParameterSocket::ParameterSocket(QObject *parent) :
    IncomingBaseSocket(parent)
//    curTime(0),
//    createTime(GetTickCount())
{
    ConnectTo();
}

void ParameterSocket::ConnectTo()
{
    connect(this,  SIGNAL(DataArrived(UTSharedDataParam, int)),
            platform()->GetUTSetupSetOne(), SLOT(ReceiveSetOneData(UTSharedDataParam, int)));
    connect(this, SIGNAL(PlxStatusReq()), platform(), SLOT(GetPlxInitResult()));
    connect(platform(), SIGNAL(PlxInitResult(int)),
            this, SLOT(PlxStatusAnswer(int)));
    connect(this, SIGNAL(VersionReq()), platform(), SLOT(GetVersion()));
    connect(platform(), SIGNAL(VersionRead(sharedVersionLower)),
            this, SLOT(VersionAnswer(sharedVersionLower)));

    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();

    int sensorCardNum = platform()->GetSensorCardNum();
    for (int i = 0; i < sensorCardNum; ++i)
    {
        connect(this, SIGNAL(SimulationArrived(UTSharedDataSimulation)),
                pAecWorkVec->at(i).data(), SLOT(SetSimulationData(UTSharedDataSimulation)));
    }

    for(AecWorkVector::iterator it = pAecWorkVec->begin(); it != pAecWorkVec->end(); ++it)
    {
        sharedAecWork aecWork = *it;
        connect(this, SIGNAL(ChannelSelect(qint32, qint32)),
                aecWork.data(), SLOT(SetSelectChannel(qint32, qint32)));
        connect(this, SIGNAL(ScanStutas(qint32)),
                aecWork.data(), SLOT(OnSetScanStutas(qint32))); // 扫查状态
        connect(this, SIGNAL(RepeatFrequence(qint32)),
                aecWork.data(), SLOT(OnSetRepeatFrequence(qint32))); // 重复频率
        connect(this, SIGNAL(SensorRoll(quint16)),
                aecWork.data(), SLOT(OnSetSensorRoll(quint16))); // 传感器轮循使能
        connect(this, SIGNAL(SendTCG(qint32, TCGCommuData, TCGData)),
                aecWork.data(), SLOT(SetTCG(qint32, TCGCommuData, TCGData)));
    }

    return;
}

void ParameterSocket::Reset()
{
    ReceiveData.clear();
}

void ParameterSocket::SendData(const QByteArray &data)
{
    write(data);
    flush();
    emit sendData(data);
}

void ParameterSocket::FetchData(const PacketHead &head)
{
    if(head.lCommand == CMD_ALL_PARAM_SET)
    {
#if ARS3D_SHARED_POINTER_LOCAL_ALLOCATION  /* 编译器性能优化，参见此宏开关定义 */
        UTSharedDataParam param = UTSharedDataParam::create();
#else
        UTSharedDataParam param(new CH_CONFIG);
#endif

        read(reinterpret_cast<char *>(param.data()), head.lSize);
        //qDebug() << "set0ne ChnNo: " << head.nChannelIndex;
        emit DataArrived(param, head.nChannelIndex);

 /*       if(GetTickCount() - createTime > 10000)
        {
  //          if(curTime != 0)
            {
                qint64 nTime = head.nGateIndex - curTime;
                if(nTime < 100)
                {
                    vecParm.append(param);
                    vecParmHead.append(head);
                }
                curTime = head.nGateIndex;
            }

            int nSize = vecParmHead.count();
            if(nSize > 5)
            {
                qint64 nTime = 0;
                int nMaxIdx = 0;
                for(int i = 0; i < nSize; ++i)
                {
                    qint64 nValue = vecParmHead.at(i).nGateIndex;
                    if(nValue > nTime)
                    {
                        nTime = nValue;
                        nMaxIdx = i;
                    }
                }
                emit DataArrived(vecParm.at(nMaxIdx), head.nChannelIndex);
                vecParm.clear();
                vecParmHead.clear();
            }
        }*/
    }
    else if(head.lCommand == CMD_CURRENT_CHANNEL)
    {
        // 用nGateIndex来表示界面波或伤波通道 0：界面波 1：伤波
        qint32 nSurOrFlaw = static_cast<qint32>(head.nGateIndex);
        qint32 nSelectChn = static_cast<qint32>(head.nChannelIndex);    // 选择的通道
        emit ChannelSelect(nSelectChn, nSurOrFlaw);
    }
    else if(head.lCommand == CMD_SIMULATE_SET)  // 模拟光眼信号
    {
        UTSharedDataSimulation simulation = UTSharedDataSimulation::create();
        read(reinterpret_cast<char *>(simulation.data()), head.lSize);
        qDebug() << "size" << head.lSize;
        emit SimulationArrived(simulation);
    }
    else if(head.lCommand == CMD_LOWER_PLX_REQ)  // 请求下位机状态
    {
        emit PlxStatusReq();
    }
    else if(head.lCommand == CMD_VERSION_REQ)  // 请求下位机版本信息
    {
        emit VersionReq();
    }
    else if(head.lCommand == CMD_SCAN_STATUS)  // 扫查状态
    {
        qint32 nScanStatus = 0;
        read(reinterpret_cast<char *>(&nScanStatus), head.lSize);
        emit ScanStutas(nScanStatus);
    }
    else if(head.lCommand == CMD_TCG_SWITCH)
    {
        TCGCommuData data;
        read(reinterpret_cast<char *>(&data), sizeof(TCGCommuData));
        TCGData tcgData;
        tcgData.resize(TCG_RAM_SIZE);
        int nSize = sizeof(float) * TCG_RAM_SIZE;
        read(reinterpret_cast<char *>(tcgData.data()), nSize);
        emit SendTCG(head.nChannelIndex, data, tcgData);
    }
    else if(head.lCommand == CMD_RPF_SET)  // 复重频率
    {
        qint32 nRpf = 0;
        read(reinterpret_cast<char *>(&nRpf), head.lSize);
        emit RepeatFrequence(nRpf);
    }
    else if(head.lCommand == CMD_SENSOR_ROLL_SET)  // 传感器轮循使能
    {
        quint16 value = 0;
        read(reinterpret_cast<char *>(&value), head.lSize);
        emit SensorRoll(value);
    }
    else
    {
        ;//
    }
}
/*
void ParameterSocket::OnDisconnected()
{
    emit Disconnected();
    return;
}
*/

void ParameterSocket::PlxStatusAnswer(int nValue)
{
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_LOWER_PLX_ANSWER;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = sizeof(int);
    // 拼接数据包
    QByteArray send;
    send.resize(sizeof(stPacketHead) + stPacketHead.lSize);
    memcpy(send.data(), &stPacketHead, sizeof(stPacketHead));

    memcpy(send.data() + sizeof(stPacketHead), &nValue, stPacketHead.lSize);
    // 发送
    SendData(send);
}

void ParameterSocket::VersionAnswer(sharedVersionLower version)
{
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_VERSION_ANSWER;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = sizeof(VersionLower);
    // 拼接数据包
    QByteArray send;
    send.resize(sizeof(stPacketHead) + stPacketHead.lSize);
    memcpy(send.data(), &stPacketHead, sizeof(stPacketHead));

    memcpy(send.data() + sizeof(stPacketHead), version.data(), stPacketHead.lSize);
    // 发送
    SendData(send);
}
