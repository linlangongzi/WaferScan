#include "CScanSocket.h"
#include "Platform/Platform.h"
#include "CardWork/AECWork.h"

using namespace NS_PXI100;

CScanSocket::CScanSocket(QObject *parent) :
    IncomingBaseSocket(parent),
    m_bSendFlag(false)
{
    packetHeadLength = sizeof(PacketHead);//BYTES_PACKET_HEAD;用BYTES_PACKET_HEAD改变数据结构得重新编译
    packetBodyLength = sizeof(FEATURE_DATA) * C_SCAN_NUM_FRAMES_PER_PACKET;//BYTES_PACKET_BODY_CSCAN;
    packetLength = packetHeadLength + packetBodyLength;//BYTES_PACKET_CSCAN;
    ConnectTo();
}

void CScanSocket::ConnectTo()
{
    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();

    for(AecWorkVector::iterator it = pAecWorkVec->begin(); it != pAecWorkVec->end(); ++it)
    {
        sharedAecWork aecWork = *it;
#if FEATURE_SINGLE_CHANNEL
        connect(aecWork.data(), SIGNAL(CscanDataRead(UTSharedCommuSegment)),
            this, SLOT(OnDataArrived(UTSharedCommuSegment)));
#else
        connect(aecWork.data(), SIGNAL(CscanDataRead(UTSharedFeatureSegment, qint32)),
            this, SLOT(OnDataArrived(UTSharedFeatureSegment, qint32)));
#endif
    }

    return;
}

/*!
 * \brief CScanSocket::发送特征数据
 * \param data 指向特征数据的共享指针
 */
void CScanSocket::SendData(UTSharedFeatureSegment data, qint32 nFrameNum)
{  
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = sendCommand;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = sizeof(FEATURE_DATA) * nFrameNum;

    // 拼接数据包
    QByteArray byteArray;
    int packetLength = packetHeadLength + stPacketHead.lSize;
    byteArray.resize(packetLength);
    memcpy(byteArray.data(), &stPacketHead, packetHeadLength);
    memcpy(byteArray.data() + packetHeadLength, data.data(), stPacketHead.lSize);

    // 发送
    write(byteArray);
    flush();
}

void CScanSocket::SendData(UTSharedCommuSegment segment)
{
    int count = segment->size();
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = sendCommand;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = sizeof(FEATURE_COMMU) * count;
    // 拼接数据包
    QByteArray byteArray;
    int packetLength = packetHeadLength + stPacketHead.lSize;
    byteArray.resize(packetLength);
    memcpy(byteArray.data(), &stPacketHead, packetHeadLength);
    int nSize = sizeof(FEATURE_COMMU);
    for (int i = 0; i < count; ++i) {
        memcpy(byteArray.data() + packetHeadLength + i * nSize, segment.data()->value(i).data(), nSize);
    }
    // 发送
    write(byteArray);
    flush();

    return;
}

void CScanSocket::FetchData(const PacketHead &head)
{
    if(head.lCommand == CMD_ASCAN)      // 特征数据请求
    {
        m_bSendFlag = true;
        qDebug() << "receive cscan data request";
    }
    else if(head.lCommand == CMD_ASCN_STOP)     // 特征数据暂停
    {
        qDebug() << "receive cscan data stop";
        m_bSendFlag = false;
    }
    else
    {
        ;//
    }
}

void CScanSocket::OnDisconnected()
{
    return;
}

void CScanSocket::OnDataArrived(UTSharedFeatureSegment data, qint32 nFrameNum)
{
    if(m_bSendFlag)
    {
        SendData(data, nFrameNum);
    }

    return;
}

void CScanSocket::OnDataArrived(UTSharedCommuSegment segment)
{
    if(m_bSendFlag)
    {
        SendData(segment);
    }

    return;
}
