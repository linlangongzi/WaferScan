#ifndef PARAMETERSOCKET_H
#define PARAMETERSOCKET_H

#include "Parser.h"
#include "DataDef.h"
#include "IncomingBaseSocket.h"
#include <QTcpSocket>
#include <QJsonArray>
#include <QSharedPointer>

namespace NS_PXI100 {

class ParameterSocket : public IncomingBaseSocket
{
    Q_OBJECT

public:
    enum E_InitResult
    {
        SUCCESS = 1,
        ERROR_INIT,             // 初始化失败
        ERROR_FIND,             // 找不到卡
        ERROR_SELTEST,          // 自检失败不对
        ERROR_AEC_CARD_NUM,     // 卡数量不对
        ERROR_PXI_VERSION       // 下位机软件版本号
    };

public:
    explicit ParameterSocket(ParserList parsers, QObject *parent = 0);
    void Reset();

signals:
    void sendData(const QByteArray &data);
    void VersionLowerRead(sharedVersionLower version);
    void PxiInitResultRead(int result);

public slots:
    void SetParameter(const ParameterTable &table, int channelID, int index);
    void SetTCG(const QJsonArray TCGArray, int channelID, int index, bool enabled);
    void SendData(const QByteArray &data);
    void SetSelectChn(qint32 nChannel, qint16 sSurOrFlaw);  // TODO
    void SetSimulationData(UTSharedDataSimulation simulation);
    void ReqPLXStatus();
    void SetScanStatus(qint32 nStatus);
    void SetRepeatFrequence(qint32 nValue);
    void VersionLowerRequest();

private slots:
    void OnReceiveData();

private:
    virtual void FetchData(const PacketHead &head);

private:
    ParserList parsers;
    QByteArray ReceiveData;
};

}   /* namespace NS_PXI100 */

#endif // PARAMETERSOCKET_H
