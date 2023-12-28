#pragma once

#include <QTcpSocket>
#include <QJsonArray>
#include "Ars1000/PipeParser.h"
#include "Ars1000/PipeDataDef.h"
#include "Ars1000/PipeIncomingBaseSocket.h"

namespace PipeDetect {

class PipeParameterSocket : public PipeIncomingBaseSocket
{
    Q_OBJECT

public:
    enum E_InitResult
    {
        SUCCESS = 1,
        ERROR_INIT,         // 初始化失败
        ERROR_FIND,         // 找不到卡
        ERROR_SELTEST,      // 自检失败不对
        ERROR_AEC_CARD_NUM  // 卡数量不对
    };

public:
    explicit PipeParameterSocket(ParserList parsers, QObject *parent = nullptr);
    void Reset();

signals:
    void dataArrived(UTDataSegment data);
    void StartFlagArrived(quint16 flag);

public slots:
    void SetParameter(const UTSetup &bottomWaveSetup, const UTSetup &tableFlaw, int channelID, int index);
    void setTCG(const QJsonArray TCGArray, int channel, bool enabled);
    void SendData(const QByteArray &data);
    void SetSimulationData(UTSharedDataSimulation simulation);
    void ReqPLXStatus();
    void SetScanStatus(qint32 nStatus);
    void SetWaveTypeMark(quint16 mark);
    void SetPRF(quint16 prf);
    void SetCoderMark(quint16 mark);

private slots:
    void OnReceiveData();

private:
    virtual void FetchData(const PacketHead &head);

private:
    ParserList parsers;
    QByteArray ReceiveData;

};

}                                                                               /* namespace PipeDetect */
