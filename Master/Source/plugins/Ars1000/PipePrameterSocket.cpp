#include <QJsonObject>
#include <QMessageBox>
#include <Ultrasonic/IInstrumentDriver.h>
#include "Ars1000/PipePrameterSocket.h"

using namespace PipeDetect;

PipeParameterSocket::PipeParameterSocket(ParserList parsers, QObject *parent) :
    PipeIncomingBaseSocket(parsers, parent),
    parsers(parsers)
{
    Q_ASSERT(parent);
    packetLength = BYTES_PACKET_PARAMETER;
    packetBodyLength = BYTES_PACKET_BODY_PARAMETER;
    packetCommand = CMD_LOWER_PLX_ANSWER;
    requestCmd = CMD_LOWER_PLX_REQ;
    requestChannel = 0;
    disabled = false;
    connect(this, SIGNAL(readyRead()), this, SLOT(OnReceiveData()));
}

void PipeParameterSocket::Reset()
{
    ReceiveData.clear();
}

void PipeParameterSocket::SetParameter(const UTSetup &bottomWaveSetup, const UTSetup &tableFlaw, int channelID, int index)
{
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_ALL_PARAM_SET;
    stPacketHead.nChannelIndex = channelID; // 探头编号
    stPacketHead.nGateIndex = 0;
    stPacketHead.lSize = sizeof(CH_CONFIG);
    // 拼接数据包
    QByteArray Parameter;
    Parameter.resize(sizeof(stPacketHead) + sizeof(CH_CONFIG));
    memcpy(Parameter.data(), &stPacketHead, sizeof(stPacketHead));

    Q_ASSERT(parsers.count() > index);
    memcpy(Parameter.data() + sizeof(stPacketHead), parsers[index]->fromStruct(bottomWaveSetup, tableFlaw, channelID).data(), sizeof(CH_CONFIG));
    // 发送
    SendData(Parameter);
}

void PipeParameterSocket::setTCG(const QJsonArray TCGArray, int channel, bool enabled)
{
    if (channel >= parsers.count()) {
        return;
    }

    TCGData tcg = enabled ? parsers[channel]->CalcTCG(TCGArray) : parsers[channel]->EmptyTCG();
    int bytesTCG = sizeof(TCGData::value_type) * tcg.count();
    PacketHead head;
    head.lCommand = CMD_TCG_SWITCH;
    head.nChannelIndex = channel;
    head.nGateIndex = 0;
    head.lSize = sizeof(TCGCommuData) + bytesTCG;

    TCGCommuData stTCGCommuData;
    stTCGCommuData.bOpenFlag = enabled;                                         //需修改
    stTCGCommuData.nSamplingInterval = parsers[channel]->GetSampleInterval();   /* 下位机的SampleInterval是时钟周期的倍数，1为80MHz，2为40MHz，以此类推 */
    stTCGCommuData.nSramNum = parsers[channel]->GetTcgRamSize();

    QByteArray data;
    data.resize(sizeof(PacketHead) + head.lSize);
    memcpy(data.data(), &head, sizeof(head));
    memcpy(data.data() + sizeof(PacketHead), &stTCGCommuData, sizeof(TCGCommuData));
    memcpy(data.data() + sizeof(PacketHead) + sizeof(TCGCommuData), tcg.data(), bytesTCG);

    SendData(data);
}

void PipeParameterSocket::SendData(const QByteArray &data)
{
    write(data);
    flush();
}

void PipeParameterSocket::OnReceiveData()
{
    ReceiveData.clear();
}

void PipeParameterSocket::FetchData(const PacketHead &head)
{
    if (head.lCommand == CMD_LOWER_PLX_ANSWER) {
        int status;
        read(reinterpret_cast<char *>(&status), sizeof(status));
        auto plxResult = static_cast<E_InitResult>(status);
        switch (plxResult)
        {
        case 1:
            break;
        case ERROR_INIT:
            QMessageBox::warning(nullptr, tr("error"), tr("PLX init failed!"), QMessageBox::Yes);
            break;
        case ERROR_FIND:
            QMessageBox::warning(nullptr, tr("error"), tr("PLX Bus、Slot find failed!"), QMessageBox::Yes);
            break;
        case ERROR_SELTEST:
            QMessageBox::warning(nullptr, tr("error"), tr("PLX card selfTest failed!"), QMessageBox::Yes);
            break;
        case ERROR_AEC_CARD_NUM:
            QMessageBox::warning(nullptr, tr("error"), tr("PLX aec card num is not enough, please \
                                   ensure insert enough aec card or one of them is not read!"), QMessageBox::Yes);
            break;
        default:
            //QMessageBox::warning(NULL, tr("error"), tr("PLX STATUS_ERROR"), QMessageBox::Yes);
            break;
        }
    } else if (head.lCommand == CMD_TEST_START_FLAG) {
        quint16 flag = 0;
        read(reinterpret_cast<char *>(&flag), sizeof(flag));
        emit StartFlagArrived(flag);
    }
}

/*!
 * \brief 设置模拟数据（光眼信号）
 */
void PipeParameterSocket::SetSimulationData(UTSharedDataSimulation simulation)
{
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_SIMULATE_SET;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.lSize = sizeof(SIMULATION_DATA_t);

    QByteArray sendData;
    sendData.resize(sizeof(stPacketHead) + sizeof(SIMULATION_DATA_t));
    memcpy(sendData.data(), &stPacketHead, sizeof(stPacketHead));
    memcpy(sendData.data() + sizeof(stPacketHead), simulation.data(), sizeof(SIMULATION_DATA_t));
    SendData(sendData);

    return;
}

/*!
 * \brief ParameterSocket::ReqPLXStatus: 请求下位机状态
 */
void PipeParameterSocket::ReqPLXStatus()
{
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_LOWER_PLX_REQ;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
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
void PipeParameterSocket::SetScanStatus(qint32 nStatus)
{
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_SCAN_STATUS;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.lSize = sizeof(qint32);

    QByteArray sendData;
    sendData.resize(sizeof(stPacketHead) + stPacketHead.lSize);
    memcpy(sendData.data(), &stPacketHead, sizeof(stPacketHead));
    memcpy(sendData.data() + sizeof(stPacketHead), &nStatus, stPacketHead.lSize);
    SendData(sendData);

    return;
}

/*!
 * \brief PipeParameterSocket::SetWaveTypeMark
 * \param mark伤波底波切换标志
 */
void PipeParameterSocket::SetWaveTypeMark(quint16 mark)
{
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_FLAW_OR_BOTTOM;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.lSize = sizeof(quint16);

    QByteArray sendData;
    sendData.resize(sizeof(stPacketHead) + stPacketHead.lSize);
    memcpy(sendData.data(), &stPacketHead, sizeof(stPacketHead));
    memcpy(sendData.data() + sizeof(stPacketHead), &mark, stPacketHead.lSize);
    SendData(sendData);

    return;
}

void PipeParameterSocket::SetPRF(quint16 prf)
{
    PacketHead stPacketHead;
    stPacketHead.lCommand = Command::CMD_SET_PRF;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.lSize = sizeof(quint16);

    QByteArray sendData;
    sendData.resize(sizeof(stPacketHead) + stPacketHead.lSize);
    memcpy(sendData.data(), &stPacketHead, sizeof(stPacketHead));
    memcpy(sendData.data() + sizeof(stPacketHead), &prf, stPacketHead.lSize);
    SendData(sendData);

    return;
}

void PipeParameterSocket::SetCoderMark(quint16 mark)
{
    PacketHead stPacketHead;
    stPacketHead.lCommand = Command::CMD_CODER_MARK;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.lSize = sizeof(quint16);

    QByteArray sendData;
    sendData.resize(sizeof(stPacketHead) + stPacketHead.lSize);
    memcpy(sendData.data(), &stPacketHead, sizeof(stPacketHead));
    memcpy(sendData.data() + sizeof(stPacketHead), &mark, stPacketHead.lSize);
    SendData(sendData);

    return;
}

