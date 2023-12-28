#include "IncomingBaseSocket.h"
#include <QMessageBox>
#include <arsCore/Platform.h>

using namespace NS_PXI100;

/*!
 * \brief 默认构造体需要一个Parser列表，其中包含各通道的Parser对象的安全指针。
 * \param parsers Parser对象的指针列表
 * \param parent QObject父对象
 */
IncomingBaseSocket::IncomingBaseSocket(const ParserList &parsers, QObject *parent) :
    QTcpSocket(parent),
    parsers(parsers)
{
    nIdentification = platform()->Get("CommunicationId").toInt();
    connect(this, SIGNAL(readyRead()), this, SLOT(OnDataReady()));
}

/*!
 * \brief 设置接口的过滤等级。接口的过滤等级将与派生类设置的Mask相比较，启用或屏蔽接口。
 * \param level 过滤等级
 */
void IncomingBaseSocket::SetDataLevel(const IInstrumentDriver::DataLevel &level)
{
    disabled = !(level & maskDataLevel);
}

/*!
 * \brief 向下位机发送一个请求数据包，请求其发送上传数据。当接口被屏蔽时不生效。
 * 生成请求数据包的参数，由派生类设置。如果派生类设置不当，请求可能无效。
 */
void IncomingBaseSocket::RequestData()
{
//    if (disabled)
//    {
//        return;
//    }
    qDebug() << "IncomingBaseSocket::RequestData" << this;
    PacketHead stPacketHead;
    stPacketHead.lCommand = requestCmd;
    stPacketHead.nChannelIndex = requestChannel;
    stPacketHead.nGateIndex = 0;
    stPacketHead.nIdentification = nIdentification;
    stPacketHead.lSize = 0;
    write(reinterpret_cast<char *>(&stPacketHead), sizeof(PacketHead));
}

/*!
 * \brief 从TcpSocket缓冲区中读取一个A扫描帧，将其存入指定的共享帧中。
 * TcpSocket缓冲区中的A扫描二进制数据数据，将被从缓冲区中弹出。
 * 这个函数将从缓冲区中读取并弹出 BYTES_FRAME_ASCAN 字节。
 * \param frame 共享的数据帧
 */
void IncomingBaseSocket::ReadADataFrame(UTSharedDataFrame &frame)
{
    /* 合理性检查 */
    Q_ASSERT(BYTES_FRAME_ASCAN == sizeof(ADataValue) * A_SCAN_NUM_SAMPLES_PER_FRAME);
    frame->AData.resize(A_SCAN_NUM_SAMPLES_PER_FRAME);
    read(reinterpret_cast<char *>(frame->AData.data()), BYTES_FRAME_ASCAN);     /* 将缓冲区直接复制到QVector内存空间中 */
}

/*!
 * \brief 从TcpSocket缓冲区中读取一个C扫描帧，将其存入指定的共享帧中。
 * TcpSocket缓冲区中的A扫描二进制数据数据，将被从缓冲区中弹出。
 * 这个函数将从缓冲区中读取并弹出 BYTES_FRAME_CDATA 字节。
 * \param frame 共享的数据帧
 * \param channel 扫描数据的通道号。该通道号将用于索引Parser列表，按照指定通道的配置处理数据。
 */
void IncomingBaseSocket::ReadCDataFrame(UTSharedDataFrame &frame, int channel, FeatureDataType dataType)
{
    /* 边界检查 */
    if (Q_LIKELY(channel >= 0 && channel < parsers.count() && parsers[channel])) {
        parsers[channel]->ReadCDataFrame(this, frame, dataType);
    }
}

/*!
 * \brief 槽函数，当TcpSocket接收到数据包时触发。
 * 此槽函数将尝试读取和检测包头，如果检测到有效的包，则调用派生类的回调函数进行解包。
 * 包头的有效性，将检查指令参数和长度参数，这两个参数的正确值由派生类指定。注意，派生类如果指定错
 * 误的值，会导致包头解析失败。
 * 当遇到无效包头时，会尝试逐字节向后搜索，直到找到正确的包头。
 */
void IncomingBaseSocket::OnDataReady()
{
    /* 合理性检查 */
//    Q_ASSERT(packetLength > 0);
//    Q_ASSERT(packetBodyLength > 0);
//    Q_ASSERT(packetBodyLength + qint32(sizeof(PacketHead)) == packetLength);    /* 包长度应当等于包头加正文 */
    PacketHead head;
    int nHeadSize = sizeof(PacketHead);
    /* 需要尝试读取多次，直到缓冲区耗竭 */
    int remaining = 1;                                                          /* 可供尝试的多余字节数。当可用字节数等于标准包长度时，remaining为1。由于尚未进行检查，此处置1。 */
    while (remaining)                                                           /* 也可以使用do ... while循环，但出于栈分配的考虑，这样反而降低执行效率。 */
    {
        /* 包头检查时，不使用read读取数据，因为read会将数据从缓冲区中弹出，在包头未对齐时将导
         * 致难以逐字节向后搜索包头。此处使用peek，将包头读出，但不影响缓冲区。 */
        peek(reinterpret_cast<char *>(&head), nHeadSize);              /* 此处可用长度可能不够，但此时由于remaining < 0，head将不被读取，不会造成异常。 */
        for (remaining = bytesAvailable();                   /* 更新remaining字节数 */
             remaining > 0 && (head.nIdentification != nIdentification);        /* 如果包长度足够，并且peek到的不是有效包头，则向后逐字节搜索有效包头。 */
             --remaining)
        {
//            qDebug() << "IncomingBaseSocket::OnDataReady: got invalid header" << head.lCommand << head.lSize << head.nChannelIndex << head.nGateIndex;
            char dummy;
            read(&dummy, 1);                                                    /* 从Socket缓冲区中弹出一个字节。这个操作效率比较低，但是不经常执行。 */
            peek(reinterpret_cast<char *>(&head), sizeof(PacketHead));          /* 更新包头数据 */
        }

        if(head.nIdentification == nIdentification) // 包头验证正确，在缓冲区的数据不足数据长度时返回
        {
            if(remaining < nHeadSize + head.lSize)
            {
                return;
            }
        }
        if (Q_UNLIKELY(remaining <= 0))                                         /* 包长度不足，结束所有处理 */
        {
            return;
        }
        if (Q_UNLIKELY(disabled))                                               /* 如果Socket已经被禁用，则不需要解析 */
        {
            read(head.lSize);                                                 /* 但需要将缓冲区中的数据包弹出 */
            continue;                                                           /* 继续循环。此处若return，则可能有数据堆积在缓冲区中，由于Socket缓冲区无限大，长时间运行将耗尽内存。 */
        }
        read(reinterpret_cast<char *>(&head), sizeof(PacketHead));              /* 将包头从Socket缓冲区中弹出 */
        FetchData(head);                                                        /* 现在Socket缓冲区中应当是包体，调用派生类的回调函数读取。 */
    }
}
