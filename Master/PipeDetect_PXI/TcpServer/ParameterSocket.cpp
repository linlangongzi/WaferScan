#include "ParameterSocket.h"
#include "Platform/Platform.h"
#include "CardWork/AECWork.h"
#include "CardWork/MFCWork.h"
#include "ConfigObject/UTSetup.h"
#include "TcpServer/UTSetupSetOne.h"
#include <QDebug>

using namespace NS_PXI100;

ParameterSocket::ParameterSocket(QObject *parent) :
    QTcpSocket(parent)
{
    ConnectTo();
}

void ParameterSocket::ConnectTo()
{
    connect(this, &QIODevice::readyRead, this, &ParameterSocket::OnReceiveData);
    connect(this, &ParameterSocket::PlxStatusReq, platform(), &PlatformBase::GetPlxInitResult);
    connect(platform(), &PlatformBase::PlxInitResult, this, &ParameterSocket::PlxStatusAnswer);
    connect(this, &ParameterSocket::SendPRF, platform()->GetMfcWork().data(), &CardWorkBase::SetPRF);
    connect(this, &ParameterSocket::SendCoderMark, platform()->GetMfcWork().data(), &CardWorkBase::SetCoderMark);
    connect(platform()->GetMfcWork().data(), &CardWorkBase::SendStartFlag, this, &ParameterSocket::SendStartFlag);
//    connect(this, SIGNAL(DataArrived(UTSharedDataParam, int)), platform()->GetUTSetupSetOne(), SLOT(ReceiveSetOneData(UTSharedDataParam, int)));

    qRegisterMetaType<NS_PXI100::TCGCommuData>("NS_PXI100::TCGCommuData");
    qRegisterMetaType<NS_PXI100::TCGData>("NS_PXI100::TCGData");
    pointerAecWorkVec pAecWorkVec = pointerAecWorkVec::create();
    pAecWorkVec = platform()->GetAecWorkByType<pointerAecWorkVec, sharedAecWork, AECWork>();

    for(AecWorkVector::iterator it = pAecWorkVec->begin(); it != pAecWorkVec->end(); ++it)
    {
        sharedAecWork aecWork = *it;
        connect(this, SIGNAL(DataArrived(UTSharedDataParam, int)), aecWork.data(), SLOT(OnSetOne(UTSharedDataParam, int)));
        connect(this, SIGNAL(WaveTypeChanged(quint16)), aecWork.data(), SLOT(SetWaveType(quint16)));
        connect(this, SIGNAL(SendTCG(quint32,NS_PXI100::TCGCommuData,NS_PXI100::TCGData)), aecWork.data(), SLOT(SetTCG(quint32,NS_PXI100::TCGCommuData,NS_PXI100::TCGData)));
    }
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

void ParameterSocket::OnReceiveData()
{
#if 1
    /* 合理性检查 */
    Q_ASSERT(packetLength > 0);
    Q_ASSERT(packetBodyLength > 0);
    PacketHead head;

    /* 需要尝试读取多次，直到缓冲区耗竭 */
    int remaining = 1;                                                          /* 可供尝试的多余字节数。当可用字节数等于标准包长度时，remaining为1。由于尚未进行检查，此处置1。 */
    while (remaining)                                                           /* 也可以使用do ... while循环，但出于栈分配的考虑，这样反而降低执行效率。 */
    {
        /* 包头检查时，不使用read读取数据，因为read会将数据从缓冲区中弹出，在包头未对齐时将导
         * 致难以逐字节向后搜索包头。此处使用peek，将包头读出，但不影响缓冲区。 */
        peek(reinterpret_cast<char *>(&head), sizeof(PacketHead));              /* 此处可用长度可能不够，但此时由于remaining < 0，head将不被读取，不会造成异常。 */
        packetLength = BYTES_PACKET_PARAM;

        if(head.lCommand == CMD_ALL_PARAM_SET) {
            packetLength = BYTES_PACKET_PARAM;
        } else if(head.lCommand == CMD_CURRENT_CHANNEL || head.lCommand == CMD_LOWER_PLX_REQ) {
            packetLength = sizeof(PacketHead);
        } else if(head.lCommand == CMD_SIMULATE_SET) {
            packetLength = sizeof(PacketHead) + sizeof(sizeof(SIMULATION_DATA_t));
        } else if(head.lCommand == CMD_SCAN_STATUS) {
            packetLength = sizeof(PacketHead) + sizeof(qint32);
        } else if(head.lCommand == CMD_TCG_SWITCH) {
            packetLength = sizeof(PacketHead) + sizeof(TCGCommuData) + sizeof(float) * TCG_RAM_Size;
        } else if(head.lCommand == CMD_FLAW_OR_BOTTOM
                  || head.lCommand == CMD_SET_PRF
                  || head.lCommand == CMD_CODER_MARK) {
            packetLength = sizeof(PacketHead) + sizeof(quint16);
        }

        for (remaining = bytesAvailable() - packetLength + 1;                   /* 更新remaining字节数 */
             remaining > 0 && (head.lCommand != CMD_ALL_PARAM_SET
                               && head.lCommand != CMD_CURRENT_CHANNEL
                               && head.lCommand != CMD_SIMULATE_SET
                               && head.lCommand != CMD_SCAN_STATUS
                               && head.lCommand != CMD_LOWER_PLX_REQ
                               && head.lCommand != CMD_TCG_SWITCH
                               && head.lCommand != CMD_FLAW_OR_BOTTOM
                               && head.lCommand != CMD_SET_PRF
                               && head.lCommand != CMD_CODER_MARK);   /* 如果包长度足够，并且peek到的不是有效包头，则向后逐字节搜索有效包头。 */
             --remaining)
        {
            char dummy;
            read(&dummy, 1);                                                    /* 从Socket缓冲区中弹出一个字节。这个操作效率比较低，但是不经常执行。 */
            peek(reinterpret_cast<char *>(&head), sizeof(PacketHead));          /* 更新包头数据 */
        }

        if (Q_UNLIKELY(remaining <= 0))                                         /* 包长度不足，结束所有处理 */
        {
            return;
        }

        read(reinterpret_cast<char *>(&head), sizeof(PacketHead));              /* 将包头从Socket缓冲区中弹出 */
        FetchData(head);                                                        /* 现在Socket缓冲区中应当是包体，调用派生类的回调函数读取。 */
    }
#endif
}

void ParameterSocket::FetchData(const PacketHead &head)
{
    if(head.lCommand == CMD_ALL_PARAM_SET) {
#if ARS3D_SHARED_POINTER_LOCAL_ALLOCATION  /* 编译器性能优化，参见此宏开关定义 */
        UTSharedDataParam param = UTSharedDataParam::create();
#else
        UTSharedDataParam param(new CH_CONFIG);
#endif

        read(reinterpret_cast<char *>(param.data()), packetBodyLength);
        emit DataArrived(param, head.nChannelIndex);
    } else if (head.lCommand == CMD_CURRENT_CHANNEL) {
        // 用nGateIndex来表示界面波或伤波通道 0：界面波 1：伤波
        qint32 nSurOrFlaw = static_cast<qint32>(head.nGateIndex);
        qint32 nSelectChn = static_cast<qint32>(head.nChannelIndex);    // 选择的通道
        emit ChannelSelect(nSelectChn, nSurOrFlaw);
    } else if (head.lCommand == CMD_LOWER_PLX_REQ) {  // 请求下位机状态
        emit PlxStatusReq();
    } else if (head.lCommand == CMD_SCAN_STATUS) {  // 扫查状态
        qint32 nScanStatus = 0;
        read(reinterpret_cast<char *>(&nScanStatus), sizeof(nScanStatus));
        emit ScanStutas(nScanStatus);
    } else if (head.lCommand == CMD_TCG_SWITCH) {
        TCGData tcgData;
        TCGCommuData data;
        tcgData.resize(TCG_RAM_Size);
        int nSize = sizeof(float) * TCG_RAM_Size;
        read(reinterpret_cast<char *>(&data), sizeof(TCGCommuData));
        read(reinterpret_cast<char *>(tcgData.data()), nSize);
        emit SendTCG(head.nChannelIndex, data, tcgData);
    } else if (head.lCommand == CMD_FLAW_OR_BOTTOM) {
        quint16 mark = 0;
        read(reinterpret_cast<char *>(&mark), sizeof(quint16));
        emit WaveTypeChanged(mark);
    } else if (head.lCommand == CMD_SET_PRF) {
        quint16 prf = 0;
        read(reinterpret_cast<char *>(&prf), sizeof(quint16));
        emit SendPRF(prf);
    } else if (head.lCommand == CMD_CODER_MARK) {
        quint16 mark = 0;
        read(reinterpret_cast<char *>(&mark), sizeof(quint16));
        emit SendCoderMark(mark);
    }
}

void ParameterSocket::PlxStatusAnswer(int nValue)
{
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_LOWER_PLX_ANSWER;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.lSize = sizeof(int);

    // 拼接数据包
    QByteArray send;
    send.resize(sizeof(stPacketHead) + stPacketHead.lSize);
    memcpy(send.data(), &stPacketHead, sizeof(stPacketHead));
    memcpy(send.data() + sizeof(stPacketHead), &nValue, stPacketHead.lSize);

    // 发送
    SendData(send);
}

void ParameterSocket::SendStartFlag(quint16 flag)
{
    // 组包头
    PacketHead stPacketHead;
    stPacketHead.lCommand = CMD_TEST_START_FLAG;
    stPacketHead.nChannelIndex = 0;
    stPacketHead.nGateIndex = 0;
    stPacketHead.lSize = sizeof(quint16);

    // 拼接数据包
    QByteArray send;
    send.resize(sizeof(stPacketHead) + stPacketHead.lSize);
    memcpy(send.data(), &stPacketHead, sizeof(stPacketHead));
    memcpy(send.data() + sizeof(stPacketHead), &flag, stPacketHead.lSize);

    // 发送
    SendData(send);
}
