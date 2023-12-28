#include "CScanSocket.h"
#include "Platform/Platform.h"
#include "CardWork/AECWork.h"

using namespace NS_PXI100;

CScanSocket::CScanSocket(QObject *parent) :
    QTcpSocket(parent),
    m_bSendFlag(false)
{
    packetHeadLength = sizeof(PacketHead);              // BYTES_PACKET_HEAD;用BYTES_PACKET_HEAD改变数据结构得重新编译
    packetBodyLength = sizeof(FEATURE_DATA_S);          // BYTES_PACKET_BODY_CSCAN;
    packetLength = packetHeadLength + packetBodyLength; // BYTES_PACKET_CSCAN;
    ConnectTo();
}

void CScanSocket::ConnectTo()
{
    connect(this, SIGNAL(readyRead()), this, SLOT(OnReceiveData()));
    connect(this, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));

    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();

    for(AecWorkVector::iterator it = pAecWorkVec->begin(); it != pAecWorkVec->end(); ++it) {
        sharedAecWork aecWork = *it;
        connect(aecWork.data(), SIGNAL(CscanDataRead(UTSharedFeatureSegment, qint32)),
            this, SLOT(OnDataArrived(UTSharedFeatureSegment, qint32)));
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
    stPacketHead.lSize = packetBodyLength;//packetBodyLength;

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

void CScanSocket::OnReceiveData()
{
    PacketHead head;
    read(reinterpret_cast<char *>(&head), sizeof(PacketHead));              // 将包头从Socket缓冲区中弹出
    FetchData(head);                                                        // 现在Socket缓冲区中应当是包体，调用派生类的回调函数读取
}

void CScanSocket::FetchData(const PacketHead &head)
{
    if(head.lCommand == CMD_ASCAN) {            // 特征数据请求
        m_bSendFlag = true;
        qDebug() << "receive cscan data request";
    }
    else if(head.lCommand == CMD_ASCN_STOP) {   // 特征数据暂停
        qDebug() << "receive cscan data stop";
        m_bSendFlag = false;
    }
    else {
    }
}

void CScanSocket::OnDisconnected()
{
    return;
}

void CScanSocket::OnDataArrived(UTSharedFeatureSegment data, qint32 nFrameNum)
{
    if(m_bSendFlag) {
        SendData(data, nFrameNum);
    }

    return;
}
