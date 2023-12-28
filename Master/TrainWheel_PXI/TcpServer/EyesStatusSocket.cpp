#include "EyesStatusSocket.h"
#include "CardWork/AECWork.h"
#include "Platform/Platform.h"
#include "CardWork/MFCWork.h"

using namespace NS_PXI100;

EyesStatusSocket::EyesStatusSocket(QObject *parent, eEyesSocketType type) :
    IncomingBaseSocket(parent),
    m_type(type)
{
    Connect();
}

/*!
 * \brief NS_PXI100::EyesStatusSocket::SendData
 * \param data:传感器状态结构体
 * 向上位机发送最新传感器状态
 */
void NS_PXI100::EyesStatusSocket::SendData(UTSharedDataSimulation simulation)
{
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_EYES_STATUS_ANSWER;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = sizeof(SIMULATION_DATA_t);

    // 拼接数据包
    QByteArray byteArray;
    byteArray.resize(BYTES_PACKET_EYES_STATUS);
    memcpy(byteArray.data(), &stPacketHead, BYTES_PACKET_HEAD);
    memcpy(byteArray.data() + BYTES_PACKET_HEAD, simulation.data(), stPacketHead.lSize);

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

void NS_PXI100::EyesStatusSocket::OnDataArrived(UTSharedDataSimulation simulation)
{
    SendData(simulation);
}

void EyesStatusSocket::onMfcDataArrived(QByteArray byteArray)
{
    PacketHead mfcDataHeader;
    mfcDataHeader.lCommand = CMD_MFC_DATA;
    mfcDataHeader.nChannelIndex = 0;
    mfcDataHeader.nGateIndex = 0;
    mfcDataHeader.nIdentification = nIdentification;
    mfcDataHeader.lSize = byteArray.size();

    write(reinterpret_cast<char *>(&mfcDataHeader), BYTES_PACKET_HEAD);
    write(byteArray, byteArray.size());
    flush();
}

void NS_PXI100::EyesStatusSocket::OnDisconnected()
{
    Disconnect();
}

void NS_PXI100::EyesStatusSocket::Connect()
{
    if (m_type == AEC) {
        pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
        pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();
        int sensorCardNum = platform()->GetSensorCardNum();
        if (pAecWorkVec->size() > sensorCardNum) {
            sharedAecWork aecWork = *(pAecWorkVec->begin() + sensorCardNum);
            if (!aecWork.isNull()) {
                connect(aecWork.data(), SIGNAL(EyesDataRead(UTSharedDataSimulation)),
                        this, SLOT(OnDataArrived(UTSharedDataSimulation)));
                connect(this, SIGNAL(EyesReadFlagChanged(bool)),
                        aecWork.data(), SLOT(SetEyesStatusReadFlag(bool)));
            }
        }
    } else {
        MFCWork* pMfcWork = platform()->GetMfcWork<MFCWork>();
        if (pMfcWork) {
            connect(pMfcWork, SIGNAL(sendMfcData(QByteArray)), this, SLOT(onMfcDataArrived(QByteArray)));
        }
    }
}

void EyesStatusSocket::Disconnect()
{
    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();
    int sensorCardNum = platform()->GetSensorCardNum();
    if (pAecWorkVec->size() > sensorCardNum) {
        sharedAecWork aecWork = *(pAecWorkVec->begin() + sensorCardNum);
        if (!aecWork.isNull()) {
            disconnect(aecWork.data(), SIGNAL(EyesDataRead(UTSharedDataSimulation)),
                       this, SLOT(OnDataArrived(UTSharedDataSimulation)));
            disconnect(this, SIGNAL(EyesReadFlagChanged(bool)),
                       aecWork.data(), SLOT(SetEyesStatusReadFlag(bool)));
        }
    }

    disconnect(this, SIGNAL(readyRead()), this, SLOT(OnReceiveData()));
    disconnect(this, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
}
