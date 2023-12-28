#include "IncomingBaseSocket.h"
#include "Platform/Platform.h"

using namespace NS_PXI100;

/*!
 * \brief
 * \param parent QObject父对象
 */
IncomingBaseSocket::IncomingBaseSocket(QObject *parent) :
    QTcpSocket(parent)
{
    nIdentification = platform()->GetCommunicationId();
    connect(this, SIGNAL(readyRead()), this, SLOT(OnReceiveData()));
    connect(this, SIGNAL(disconnected()), this, SLOT(OnDisconnected()));
}

/*!
 * \brief 槽函数，当TcpSocket接收到数据包时触发。
 * 此槽函数将尝试读取和检测包头，如果检测到有效的包，则调用派生类的回调函数进行解包。
 * 包头的有效性，将检查识别码
 * 当遇到无效包头时，会尝试逐字节向后搜索，直到找到正确的包头。
 */
void IncomingBaseSocket::OnReceiveData()
{
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

        read(reinterpret_cast<char *>(&head), sizeof(PacketHead));              /* 将包头从Socket缓冲区中弹出 */
        FetchData(head);                                                        /* 现在Socket缓冲区中应当是包体，调用派生类的回调函数读取。 */
    }
}

void IncomingBaseSocket::OnDisconnected()
{
    return;
}
