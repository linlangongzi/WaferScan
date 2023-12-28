#pragma once

#include <QTcpSocket>
#include "Parser.h"

namespace Ars500 {

/*!
 * \class IncomingBaseSocket
 * \brief ARS500 设备接口中，所有接收数据包的接口类的基类。
 *
 * IncomingBaseSocket 为所有 ARS500 设备的接收接口类，提供发送数据请求、接收过滤、接收包头
 * 解析、包头验证、数据包长度验证等基本的协议服务。
 * 派生类应当在构造时设置这个基类的受保护参数，完成对基类的配置。不当的配置会导致数据包读取失败。
 *
 * 这个类应当被 AScanSocket 、 CScanSocket 、 MonitorSocket 继承。
 *
 * 当接收到有效的数据包时， IncomingBaseSocket 会调用派生类的 FetchData 虚函数成员，由派生
 * 类决定如何解析数据包。 IncomingBaseSocket 也提供 ReadADataFrame 和 ReadCDataFrame
 * 来帮助派生类快速组织输出数据。
 *
 * 这个类不会抛出信号，完成解析数据包后，派生类应当自行抛出信号。
 */
class IncomingBaseSocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit IncomingBaseSocket(const ParserList &parsers, QObject *parent = nullptr);
    void RequestData();
    void Reset();

protected:
    /*!
     * \brief 当数据包可用时的回调函数。该回调函数被调用时，Socket 缓冲区中即为数据包体。
     * 派生类应当使用 read() 从缓冲区中读取数据，并保证读取的总字节数等于 packetBodyLength
     * 纯虚函数，必须被派生类重载。
     * \param head ARS500 数据包的包头
     */
    virtual void FetchData(const PacketHead &head) = 0;
    virtual bool IsInvalidPacket(const PacketHead &head);

    void ReadADataFrame(UTDataFrame &frame);
    void ReadCompressedADataFrame(UTDataFrame &frame);
    void ReadCDataFrame(UTDataFrame &frame, int channel);

public slots:
    void SetDataLevel(int level, int channel);

private slots:
    void OnDataReady();

signals:
    void DataRequestSent();

protected:
    qint32 packetLength;                                                        /*!< 标准数据包长度，只有缓冲区中数据不低于这个长度时，才会尝试解析数据。 */
    qint32 packetBodyLength;                                                    /*!< 标准数据包体长度，包头中的 size 需要与此数值相等，才被认为是有效的数据包。 */
    CommandInt packetCommand;                                                   /*!< 标准数据包指令，包头中的 command 需要与此指令相同，才被认为是有效的数据包。 */
    CommandInt requestCmd;                                                      /*!< 发送数据请求时使用的指令。此指令将在 RequestData() 时写入数据包中。 */
    int requestChannel;                                                         /*!< 发送数据请求时使用的通道号。此指令将在 RequestData() 时写入数据包中 */
    bool disabled[NUM_CHANNELS];                                                /*!< Socket 禁用标识。抛弃收到的所有有效数据包。 */
    int maskDataLevel;                                                          /*!< 过滤层级的掩模，此掩模将在 SetDataLevel 时与输入的过滤层级进行逻辑与(&)，并以此决定是否禁用整个 Socket 。 */

private:
    ParserList parsers;                                                         /*!< Parser 的指针列表 */
};

}                                                                               /* namespace ARS500 */
