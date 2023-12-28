#pragma once

#include <Ultrasonic/Benchmark.h>
#include <Ultrasonic/IInstrumentDriver.h>
#include "AScanSocket.h"
#include "CScanSocket.h"
#include "DataDef.h"
#include "MonitorSocket.h"
#include "ParameterSocket.h"
#include "Parser.h"
#include <QDateTime>
#include <QHostAddress>
#include <QJsonObject>
#include <QTcpSocket>

namespace Ars500 {

/*!
 * \brief UTDriverARS500 是 ARS500 仪器的驱动类，在 IInstrumentDriver 接口类的基础上，
 * 为 ARS500 的实现提供控制和数据交换对象。
 *
 * \sa IInstrumentDriver
 */
class UTDriverARS500 : public IInstrumentDriver
{
    Q_OBJECT

public:
    explicit UTDriverARS500(const QJsonArray &config, QObject *parent = nullptr);

public slots:
    void setActiveChannel(int channel) override;
    InstrumentInfo instrumentInfo() const override;

public:
    static UTDataHeader InitDataHeader();

signals:
    void DataLevelChanged(int level, int channel);

public slots:
    void setHostAddr(const QString &ip) override;
    void Connect() override;
    void Disconnect() override;
    void setChannelSetup(int channel, const UTSetup &setup) override;
    void setDataLevel(int level, int channel) override;
    void resetPose() override;                                                   // 编码器复位
    void setPoseOffset(const UTPose &offset) override;

private:
    void InitSockets();
    void timerEvent(QTimerEvent *event) override;

private slots:
    void OnAScanReceived(UTDataSegment dataSegment);
    void OnMonitorReceived(UTDataFrame dataFrame);
    void OnCScanReceived(UTDataSegment dataSegment);
    void OnParamReceived(UTDataSegment dataSegment);
    void OnConnected();
    void OnVersionUpdate(const QString &newVersion);
    void onSocketStateChanged();

private:
    void UpdateWorkMode();
    bool UpdateParameters(int channel, bool forceUpdate = false);
    bool UpdateTCG(int channel);

private:
    QString version;
    static const QString deviceType;

    AScanSocket *ASocket;
    MonitorSocket *monitorSocket;
    CScanSocket *CSocket;
    ParameterSocket *parameterSocket;

    QList<QTcpSocket *> sockets;

    static const UTDataHeader header;

    QHostAddress IPAddr;
    ParserList parsers;

    QVector<UTSetup> localParameter;                                     // 本地参数表
    int currentChannel;
    int remoteChannel;
    int currentWorkMode;
    bool isTcgChanged[NUM_CHANNELS];
    bool isParameterChanged[NUM_CHANNELS];
    QMap<int, int> dataLevel;
    InstrumentInfo m_info;
};

}                                                                               /* namespace ARS500 */

class Ars500Factory : public IInstrumentDriverFactory
{
    Q_OBJECT

public:
    Ars500Factory(QObject *parent) : IInstrumentDriverFactory(parent) {}
    QString name() const override {return "ARS500";}
    IInstrumentDriver *create() const override {return new Ars500::UTDriverARS500(QJsonArray());}
};
