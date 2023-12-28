#ifndef PARAMETERSOCKET_H
#define PARAMETERSOCKET_H

#include <QTcpSocket>
#include <QSharedPointer>
#include "DataDef.h"
#include <QJsonArray>
#include "IncomingBaseSocket.h"

namespace NS_PXI100 {

class ParameterSocket : public IncomingBaseSocket
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
    void SimulationArrived(UTSharedDataSimulation simulation);
    void PlxStatusReq();
    void VersionReq();
    void ScanStutas(qint32 nStatus);
    void RepeatFrequence(qint32 nValue);
    void SensorRoll(quint16);
    void SendTCG(qint32 chnNo, TCGCommuData data, TCGData tcgData);

public slots:
    void SendData(const QByteArray &data);
    void PlxStatusAnswer(int nValue);
    void VersionAnswer(sharedVersionLower version);

private:
    QByteArray ReceiveData;
    qint32 packetLength = BYTES_PACKET_PARAM;                                                       /* 标准数据包长度，只有缓冲区中数据不低于这个长度时，才会尝试解析数据。 */
    const qint32 packetBodyLength = BYTES_PACKET_BODY_PARAM;                                        /* 标准数据包体长度，包头中的size需要与此数值相等，才被认为是有效的数据包。 */
    const CommandInt packetCommand = CMD_ALL_PARAM_SET;                                             /* 标准数据包指令，包头中的command需要与此指令相同，才被认为是有效的数据包。 */
//    QVector<UTSharedDataParam> vecParm;
//    QVector<PacketHead> vecParmHead;
//    qint64 curTime;
//    clock_t createTime;
};

}   /* namespace NS_PXI100 */

#endif // PARAMETERSOCKET_H
