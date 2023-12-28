#ifndef MONITORSOCKET_H
#define MONITORSOCKET_H

#include <QTcpSocket>
#include "DataDef.h"
#include "IncomingBaseSocket.h"

namespace NS_PXI100 {
class MonitorSocket : public IncomingBaseSocket
{
    Q_OBJECT
public:
    explicit MonitorSocket(QObject *parent = 0);

public:
    void SendData(UTSharedMonitorFrame data, qint32 nIdx);
    void FetchData(const PacketHead &head);

private:
    QByteArray ReceiveData;
    const qint32 packetHeadLength = BYTES_PACKET_HEAD;
    const qint32 packetLength = BYTES_PACKET_MONITOR;           /* 标准数据包长度，只有缓冲区中数据不低于这个长度时，才会尝试解析数据。 */
    const qint32 packetBodyLength = BYTES_PACKET_BODY_MONITOR;  /* 标准数据包体长度，包头中的size需要与此数值相等，才被认为是有效的数据包。 */
    const CommandInt sendCommand = CMD_ASCAN_RET_WAVE;          // 发送命令
//    const CommandInt requestCommand = CMD_ASCAN;                // 请求命令
    bool m_bSendFlag;                                           // 实时数据发送标志

private:
    void ConnectTo();

public slots:
    void OnDataArrived(UTSharedMonitorFrame data, qint32 nIdx);

private slots:
    void OnDisconnected();
};
}

#endif // MONITORSOCKET_H
