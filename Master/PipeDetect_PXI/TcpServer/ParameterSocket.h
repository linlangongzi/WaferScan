#ifndef PARAMETERSOCKET_H
#define PARAMETERSOCKET_H

#include <QTcpSocket>
#include <QSharedPointer>
#include "DataDef.h"
#include <QJsonArray>

namespace NS_PXI100 {

class ParameterSocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit ParameterSocket(QObject *parent = 0);

public:
    void Reset();

private:
    void FetchData(const PacketHead &head);
    void ConnectTo();

signals:
    void sendData(const QByteArray &data);
    void DataArrived(UTSharedDataParam param, int channel);
    void Disconnected();
    void ChannelSelect(qint32 nSelectNo, qint32 nSurOrFlaw);
    void PlxStatusReq();
    void ScanStutas(qint32 nStatus);
    void WaveTypeChanged(quint16 mark);
    void SendTCG(quint32 channel, NS_PXI100::TCGCommuData data1, NS_PXI100::TCGData data);
    void SendPRF(quint16 prf);
    void SendCoderMark(quint16 mark);

public slots:
    void SendData(const QByteArray &data);
    void PlxStatusAnswer(int nValue);
    void SendStartFlag(quint16 flag);

private slots:
    void OnReceiveData();

private:
    QByteArray ReceiveData;
    qint32 packetLength = BYTES_PACKET_PARAM;                 /* 标准数据包长度，只有缓冲区中数据不低于这个长度时，才会尝试解析数据。 */
    const qint32 packetBodyLength = BYTES_PACKET_BODY_PARAM;        /* 标准数据包体长度，包头中的size需要与此数值相等，才被认为是有效的数据包。 */
    const CommandInt packetCommand = CMD_ALL_PARAM_SET;       /* 标准数据包指令，包头中的command需要与此指令相同，才被认为是有效的数据包。 */
    static const int TCG_RAM_Size = 256;                        // TCG ram 地址块大小 1024 / 4个软通道

};

}   /* namespace NS_PXI100 */

#endif // PARAMETERSOCKET_H
