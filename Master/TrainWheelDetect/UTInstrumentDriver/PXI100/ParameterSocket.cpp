#include "ParameterSocket.h"
#include <QDebug>
#include <QMessageBox>

using namespace NS_PXI100;

ParameterSocket::ParameterSocket(ParserList parsers, QObject *parent) :
    IncomingBaseSocket(parsers, parent),
    parsers(parsers)
{
    Q_ASSERT(parent);
    packetLength = BYTES_PACKET_PARAMETER;
    packetBodyLength = BYTES_PACKET_BODY_PARAMETER;
    packetCommand = CMD_LOWER_PLX_ANSWER;
    requestCmd = CMD_LOWER_PLX_REQ;
    requestChannel = 0;
    maskDataLevel = IInstrumentDriver::CSCAN_ONLY;
    connect(this, SIGNAL(readyRead()), this, SLOT(OnReceiveData()));
}

void ParameterSocket::Reset()
{
    ReceiveData.clear();
}

void ParameterSocket::SetParameter(const ParameterTable &table, int channelID, int index)
{
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_ALL_PARAM_SET;
    stPacketHead.nChannelIndex = channelID;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = sizeof(CH_CONFIG);

    // 拼接数据包
    QByteArray Parameter;
    Parameter.resize(sizeof(stPacketHead) + sizeof(CH_CONFIG));
    memcpy(Parameter.data(), &stPacketHead, sizeof(stPacketHead));

    Q_ASSERT(parsers.count() > index);
    memcpy(Parameter.data() + sizeof(stPacketHead), parsers[index]->fromStruct(table, channelID).data(), sizeof(CH_CONFIG));

    // 发送
    SendData(Parameter);
}

void ParameterSocket::SetTCG(const QJsonArray TCGArray, int channelID, int index, bool enabled)
{
    TCGData tcg = enabled ? parsers[index]->CalcTCG(TCGArray) : parsers[index]->EmptyTCG();
    int bytesTCG = sizeof(TCGData::value_type) * tcg.count();
    int tcgCommuDataSize = sizeof(TCGCommuData);
    PacketHead head;
    head.lCommand = CMD_TCG_SWITCH;
    head.nChannelIndex = channelID;
    head.nGateIndex = 0;
    head.nIdentification = nIdentification;
    head.lSize = tcgCommuDataSize + bytesTCG;

    TCGCommuData tcgCommuData;
    tcgCommuData.bOpenFlag = true;    //需修改
    tcgCommuData.nSamplingInterval = parsers[index]->GetSampleInterval();   /* 下位机的SampleInterval是时钟周期的倍数，1为80MHz，2为40MHz，以此类推 */
    tcgCommuData.nSramNum = parsers[index]->GettcgRamSize();

    QByteArray send;
    int headSize = sizeof(head);
    send.resize(headSize + head.lSize);
    memcpy(send.data(), &head, headSize);
    memcpy(send.data() + headSize, &tcgCommuData, tcgCommuDataSize);
    memcpy(send.data() + headSize + tcgCommuDataSize, tcg.data(), bytesTCG);
    SendData(send);
}

void ParameterSocket::SendData(const QByteArray &data)
{
    if (state() != ConnectedState) {
        return;
    }
    write(data);
    flush();
}

void ParameterSocket::OnReceiveData()
{
    ReceiveData.clear();
}

void ParameterSocket::FetchData(const PacketHead &head)
{
    if (head.lCommand == CMD_LOWER_PLX_ANSWER) {
        int status;
        read(reinterpret_cast<char *>(&status), sizeof(status));
        E_InitResult plxResult = static_cast<E_InitResult>(status);
        emit PxiInitResultRead(plxResult);
    } else if ((head.lCommand == CMD_VERSION_ANSWER)) {
        sharedVersionLower version = sharedVersionLower::create();
        read(reinterpret_cast<char *>(version.data()), sizeof(VersionLower));
        if (version->svnVersion < NS_PXI100::PXI_VERSION_MINIMUM) {
            emit PxiInitResultRead(E_InitResult::ERROR_PXI_VERSION);
        }
        emit VersionLowerRead(version);
    }
}

/*!
 * \brief 设置通道选择
 */
void ParameterSocket::SetSelectChn(qint32 nChannel, qint16 sSurOrFlaw)
{
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_CURRENT_CHANNEL;
    stPacketHead.nChannelIndex = static_cast<qint16>(nChannel);
    stPacketHead.nGateIndex = sSurOrFlaw;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = 0;

//    write(reinterpret_cast<char *>(&stPacketHead), sizeof(stPacketHead));
//    flush();
    QByteArray sendData;
    sendData.resize(sizeof(stPacketHead));
    memcpy(sendData.data(), &stPacketHead, sizeof(stPacketHead));
    SendData(sendData);

    return;
}

/*! 设置模拟数据（光眼信号） */
void ParameterSocket::SetSimulationData(UTSharedDataSimulation simulation)
{
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_SIMULATE_SET;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = sizeof(SIMULATION_DATA_t);

    QByteArray sendData;
    sendData.resize(sizeof(stPacketHead) + sizeof(SIMULATION_DATA_t));
    memcpy(sendData.data(), &stPacketHead, sizeof(stPacketHead));
    memcpy(sendData.data() + sizeof(stPacketHead), simulation.data(), sizeof(SIMULATION_DATA_t));

    SendData(sendData);

    return;
}

/*! 请求下位机状态 */
void ParameterSocket::ReqPLXStatus()
{
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_LOWER_PLX_REQ;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = 0;

    // 拼接数据包
    QByteArray send;
    send.resize(sizeof(stPacketHead) + stPacketHead.lSize);
    memcpy(send.data(), &stPacketHead, sizeof(stPacketHead));

    // 发送
    SendData(send);
}

/*!
 * \brief 设置扫查状态
 */
void ParameterSocket::SetScanStatus(qint32 nStatus)
{
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_SCAN_STATUS;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = sizeof(qint32);

    QByteArray sendData;
    sendData.resize(sizeof(stPacketHead) + stPacketHead.lSize);
    memcpy(sendData.data(), &stPacketHead, sizeof(stPacketHead));
    memcpy(sendData.data() + sizeof(stPacketHead), &nStatus, stPacketHead.lSize);
    SendData(sendData);

    return;
}

/*!
 * \brief 设置重复频率
 */
void ParameterSocket::SetRepeatFrequence(qint32 nValue)
{
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_RPF_SET;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = sizeof(qint32);

    QByteArray sendData;
    sendData.resize(sizeof(stPacketHead) + stPacketHead.lSize);
    memcpy(sendData.data(), &stPacketHead, sizeof(stPacketHead));
    memcpy(sendData.data() + sizeof(stPacketHead), &nValue, stPacketHead.lSize);
    SendData(sendData);

    return;
}

/*!
 * \brief ParameterSocket::VersionLowerRequest
 * \brief 下位机版本请求
 */
void ParameterSocket::VersionLowerRequest()
{
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_VERSION_REQ;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = 0;

    QByteArray sendData;
    sendData.resize(sizeof(stPacketHead) + stPacketHead.lSize);
    memcpy(sendData.data(), &stPacketHead, sizeof(stPacketHead));
    SendData(sendData);
}
