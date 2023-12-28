#include "MonitorSocket.h"
#include "Platform/Platform.h"
#include "CardWork/AECWork.h"

using namespace NS_PXI100;

MonitorSocket::MonitorSocket(QObject *parent) :
    IncomingBaseSocket(parent),
    m_bSendFlag(false)
{
    ConnectTo();
}

void MonitorSocket::ConnectTo()
{
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
    stPacketHead.nChannelIndex = nIdx;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
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

void MonitorSocket::FetchData(const PacketHead &head)
{
    if(head.lCommand == CMD_ASCAN)  // 监视数据请求
    {
        m_bSendFlag = true;
        qDebug() << "receive monitor data request";
    }
    else if(head.lCommand == CMD_ASCN_STOP) // 监视数据暂停
    {
        qDebug() << "receive monitor data stop";
        m_bSendFlag = false;
    }
    else
    {
        ;//
    }
}

void MonitorSocket::OnDisconnected()
{
    return;
}

void MonitorSocket::OnDataArrived(UTSharedMonitorFrame data, qint32 nIdx)
{
//    qDebug()<<"MonitorSocket::SendData() thread: "<<QThread::currentThreadId();
    if(m_bSendFlag)
    {
        SendData(data, nIdx);
    }

    return;
}
