#ifndef CSCANSOCKET_H
#define CSCANSOCKET_H

#include <QTcpSocket>
#include "DataDef.h"

namespace NS_PXI100 {
class CScanSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit CScanSocket(QObject *parent = 0);

public:
    void SendData(UTSharedFeatureSegment data, qint32 nFrameNum);
    void FetchData(const PacketHead &head);

private:
    QByteArray ReceiveData;
    qint32 packetHeadLength = BYTES_PACKET_HEAD;
    qint32 packetLength = BYTES_PACKET_CSCAN;             /* 标准数据包长度，只有缓冲区中数据不低于这个长度时，才会尝试解析数据。 */
    qint32 packetBodyLength = BYTES_PACKET_BODY_CSCAN;    /* 标准数据包体长度，包头中的size需要与此数值相等，才被认为是有效的数据包。 */
    const CommandInt sendCommand = CMD_CSCAN_DATA;          // 发送命令
    bool m_bSendFlag;

private:
    void ConnectTo();

public slots:
    void OnDataArrived(UTSharedFeatureSegment data, qint32 nFrameNum);

private slots:
    void OnReceiveData();
    void OnDisconnected();

};
}

#endif // CSCANSOCKET_H
