#include "EyesStatusSocket.h"

using namespace NS_PXI100;

/*!
 * \brief 构造函数，需要 Parser 指针的列表。
 * \param parsers
 * \param parent
 */
EyesStatusSocket::EyesStatusSocket(ParserList parsers, QObject *parent) :
    IncomingBaseSocket(parsers, parent)
{
    Q_ASSERT(parent);
    packetLength = BYTES_PACKET_EYES_STATUS;
    packetBodyLength = BYTES_PACKET_BODY_EYES_STATUS;
    packetCommand = CMD_EYES_STATUS_ANSWER;
    requestCmd = CMD_EYES_STATUS_REQ;
    requestChannel = 0;
    maskDataLevel = IInstrumentDriver::DataLevel::MASK_MONITOR;
}

void EyesStatusSocket::SendData(const QByteArray &data)
{
    write(data);
    flush();
    emit sendData(data);
}

void EyesStatusSocket::SetReqEyesStatusMark(bool isOpen)
{
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_EYES_STATUS_REQ;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = sizeof(bool);

    // 拼接数据包
    QByteArray send;
    send.resize(sizeof(stPacketHead) + stPacketHead.lSize);
    memcpy(send.data(), &stPacketHead, sizeof(stPacketHead));
    memcpy(send.data() + sizeof(stPacketHead), &isOpen, sizeof(bool));

    // 发送
    SendData(send);
}

/*!
 * \brief 重载的回调函数，从缓冲区中读取一个传感器状态数据包体，将其存入一个共享数据片段，并抛出信号
 * \param head 数据包头
 */
void EyesStatusSocket::FetchData(const PacketHead &head)
{
    if (head.lCommand == CMD_EYES_STATUS_ANSWER) {
        UTSharedDataSimulation simulation = UTSharedDataSimulation::create();
        read(reinterpret_cast<char *>(simulation.data()), sizeof(SIMULATION_DATA_t));
        emit DataArrived(simulation);
    } else if (head.lCommand == CMD_MFC_DATA) {
        emit mfcDataArrived(read(head.lSize));
    }
}

