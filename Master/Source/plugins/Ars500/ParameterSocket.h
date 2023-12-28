#pragma once

#include <QTcpSocket>
#include "Parser.h"
#include "DataDef.h"
#include <QJsonArray>

namespace Ars500 {

class ParameterSocket : public QTcpSocket
{
    Q_OBJECT

public:
//    explicit UTDriverARS500_ParameterSocket(const QJsonArray &config, int numChannels, QObject *parent = nullptr);
    explicit ParameterSocket(ParserList parsers, QObject *parent = nullptr);

signals:
    void dataArrived(UTDataSegment data);
    void VersionUpdate(const QString &ver);

public slots:
    void SendParameter(const QVector<UTSetup> &parameters, int channel);
    void setTCG(const UTSetup &TCGArray, int channel, bool enabled);
    void ResetPose();
    void SendData(const QByteArray &data);
    void RequestVersion();

private slots:
    void OnReceiveData();

private:
    void AnalyzeData(const PacketHead &head);
    bool IsValidHead(const PacketHead &head);

private:
    ParserList parsers;
};

}                                                                               /* namespace ARS500 */
