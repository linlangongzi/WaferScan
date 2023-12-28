#include "MonitorSocket.h"
#include "Platform/Platform.h"
#include "CardWork/AECWork.h"

using namespace NS_PXI100;

MonitorSocket::MonitorSocket(QObject *parent) :
    QTcpSocket(parent),
    m_bSendFlag(false)
{
    ConnectTo();
}

void MonitorSocket::ConnectTo()
{
    connect(this, SIGNAL(readyRead()), this, SLOT(OnReceiveData()));
    connect(this, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));

    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();

    for(AecWorkVector::iterator it = pAecWorkVec->begin(); it != pAecWorkVec->end(); ++it)
    {
        sharedAecWork aecWork = *it;
        connect(aecWork.data(), SIGNAL(MonitorDataRead(UTSharedMonitorFrame, qint32)),
            this, SLOT(OnDataArrived(UTSharedMonitorFrame, qint32)));
    }

    return;
}
void MonitorSocket::SendData(UTSharedMonitorFrame data, qint32 nIdx)
{
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = sendCommand;
    stPacketHead.nChannelIndex = static_cast<qint16>(nIdx);
    stPacketHead.nGateIndex = 0;
    stPacketHead.lSize = packetBodyLength;
    // 拼接数据包
    QByteArray byteArray;
    byteArray.resize(packetLength);
    memcpy(byteArray.data(), &stPacketHead, packetHeadLength);
    memcpy(byteArray.data() + packetHeadLength, data.data(), packetBodyLength);
    // 发送
    write(byteArray);
    flush();

    return;
}

void MonitorSocket::OnReceiveData()
{
    PacketHead head;
    read(reinterpret_cast<char *>(&head), sizeof(PacketHead));              // 将包头从Socket缓冲区中弹出
    FetchData(head);                                                        // 现在Socket缓冲区中应当是包体，调用派生类的回调函数读取
    readAll();
}

void MonitorSocket::FetchData(const PacketHead &head)
{
    if(head.lCommand == CMD_ASCAN) {            // 监视数据请求
        m_bSendFlag = true;
        qDebug() << "receive monitor data request";
    }
    else if(head.lCommand == CMD_ASCN_STOP) {   // 监视数据暂停
        qDebug() << "receive monitor data stop";
        m_bSendFlag = false;
    }
    else {
    }
}

void MonitorSocket::OnDisconnected()
{
    return;
}

void MonitorSocket::OnDataArrived(UTSharedMonitorFrame data, qint32 nIdx)
{
    if(m_bSendFlag) {
        SendData(data, nIdx);
    }

    return;
}
