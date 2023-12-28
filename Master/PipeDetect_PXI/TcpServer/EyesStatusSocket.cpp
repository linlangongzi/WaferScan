#include "EyesStatusSocket.h"
#include "CardWork/AECWork.h"
#include "Platform/Platform.h"

using namespace NS_PXI100;

EyesStatusSocket::EyesStatusSocket(QObject *parent) :
    QTcpSocket(parent)
{
    Connect();
}

/*!
 * \brief NS_PXI100::EyesStatusSocket::SendData
 * \param data:传感器状态结构体
 * 向上位机发送最新传感器状态
 */
void NS_PXI100::EyesStatusSocket::SendData(SIMULATION_DATA_t data)
{
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_EYES_STATUS_ANSWER;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.lSize = sizeof(SIMULATION_DATA_t);

    // 拼接数据包
    QByteArray byteArray;
    byteArray.resize(BYTES_PACKET_EYES_STATUS);
    memcpy(byteArray.data(), &stPacketHead, BYTES_PACKET_HEAD);
    memcpy(byteArray.data() + BYTES_PACKET_HEAD, &data, stPacketHead.lSize);

    // 发送
    write(byteArray);
    flush();
}

/*!
 * \brief NS_PXI100::EyesStatusSocket::FetchData
 * \param head
 * 接收上位机发来的设置读取传感器状态的标志
 */
void NS_PXI100::EyesStatusSocket::FetchData(const NS_PXI100::PacketHead &head)
{
    if (head.lCommand == CMD_EYES_STATUS_REQ)
    {
        bool isOpen = false;
        read(reinterpret_cast<char *>(&isOpen), sizeof(bool));
        emit EyesReadFlagChanged(isOpen);
    }
}

void NS_PXI100::EyesStatusSocket::OnDataArrived(SIMULATION_DATA_t data)
{
    SendData(data);
}

void NS_PXI100::EyesStatusSocket::OnReceiveData()
{
    PacketHead head;
    read(reinterpret_cast<char *>(&head), BYTES_PACKET_HEAD);           // 将包头从Socket缓冲区中弹出
    FetchData(head);                                                    // 现在Socket缓冲区中应当是包体，调用派生类的回调函数读取
}

void NS_PXI100::EyesStatusSocket::OnDisconnected()
{
    Disconnect();
}

void NS_PXI100::EyesStatusSocket::Connect()
{
    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();

    sharedAecWork aecWork = *(pAecWorkVec->begin());
    if (!aecWork.isNull())
    {
        connect(aecWork.data(), SIGNAL(EyesDataRead(SIMULATION_DATA_t)), this, SLOT(OnDataArrived(SIMULATION_DATA_t)));
        connect(this, SIGNAL(EyesReadFlagChanged(bool)), aecWork.data(), SLOT(SetEyesStatusReadFlag(bool)));
    }

    connect(this, SIGNAL(readyRead()), this, SLOT(OnReceiveData()));
    connect(this, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
}

void EyesStatusSocket::Disconnect()
{
    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();

    sharedAecWork aecWork = *(pAecWorkVec->begin());
    if (!aecWork.isNull())
    {
        disconnect(aecWork.data(), SIGNAL(EyesDataRead(SIMULATION_DATA_t)), this, SLOT(OnDataArrived(SIMULATION_DATA_t)));
        disconnect(this, SIGNAL(EyesReadFlagChanged(bool)), aecWork.data(), SLOT(SetEyesStatusReadFlag(bool)));
    }

    disconnect(this, SIGNAL(readyRead()), this, SLOT(OnReceiveData()));
    disconnect(this, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
}
