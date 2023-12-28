#pragma once

#include "Ars1000/PipeAScanSocket.h"
#include "Ars1000/PipeCScanSocket.h"
#include "Ars1000/PipeDataDef.h"
#include "Ars1000/PipeMonitorSocket.h"
#include "Ars1000/PipeParser.h"
#include "Ars1000/PipePrameterSocket.h"
#include <QDateTime>
#include <QHostAddress>
#include <QJsonObject>
#include <QMutex>
#include <QTcpSocket>
#include <Ultrasonic/Benchmark.h>
#include <Ultrasonic/IInstrumentDriver.h>
#include <Ultrasonic/UTInstrument.h>

namespace PipeDetect {

/*!
 * \brief PipeInstrumentDriver 是 PipeDetect 仪器的驱动类，在 PipeInstrumentDriver 接口类的基础上，
 * 为 PipeDetect 的实现提供控制和数据交换对象。
 *
 * \sa PipeInstrumentDriver
 */
class PipeInstrumentDriver : public IInstrumentDriver
{
    Q_OBJECT

public:
    explicit PipeInstrumentDriver(const QJsonArray &config, QObject *parent = nullptr);

public slots:
    void ResetChannelCount(int channelCount);
    InstrumentInfo instrumentInfo() const override;

public:
    static CDataTypeList GetCDataType();
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
    void setActiveChannel(int channelIndex) override;
    void setPRF(quint16 prf) override;
    void sendRawMessage(int type, const QVariant &message) override;

protected:
    QVector<UTSetup> localParameter;                                     // 本地参数表

private:
    void InitSockets();
    void timerEvent(QTimerEvent *event) override;

private slots:
    void OnAScanConnected();
    void OnMonitorConnected();
    void OnCScanConnected();
    void OnParamConnected();

    void OnAScanReceived(UTDataSegment dataSegment);
    void OnMonitorReceived(UTDataFrame frame);
    void OnCScanReceived(UTDataSegment dataSegment);
    void OnParamReceived(UTDataSegment dataSegment);

    void OnAScanDisConnected();
    void OnMonitorDisConnected();
    void OnCScanDisConnected();
    void OnParamDisConnected();

    void OnConnected(int mask);
    void OnDisconnected(int mask);

private:
    void UpdateWorkMode();
    bool UpdateParameters(int channel, bool forceUpdate = false);
    bool UpdateTCG(int channel);

private:
    static const int NUM_GATES = 4;
    static const QString version;
    static const QString deviceType;
    InstrumentStatus status;

    PipeAScanSocket *m_AScanSocket;
    PipeMonitorSocket *m_MonitorSocket;
    PipeCScanSocket *m_CScanSocket;
    PipeParameterSocket *m_ParameterSocket;

    static const int ASCAN_PORT       = 2222;
    static const int MONITOR_PORT     = 3333;
    static const int CSCAN_PORT       = 4444;
    static const int PARAM_PORT       = 5555;

    QHostAddress IPAddr;
    int nConnectedStatus;
    qint32 dataLevel;
    ParserList parsers;   // wangwei add
    quint16 selChannelMark;
    QJsonArray tcgArray;
    bool hasNewTCG = false;
    int tcgChannel = 0;
    QTime lastTime = QTime::currentTime();
    QVector<int> averageCount;
    QVector<CDataFrame> sum;
    QVector<int> sumCount;
    InstrumentInfo m_info;
};

}                                                                               /* namespace PipeDetect */

class PipeInstrumentDriverFactory : public IInstrumentDriverFactory
{
    Q_OBJECT

public:
    PipeInstrumentDriverFactory(QObject *parent) : IInstrumentDriverFactory(parent) {}
    QString name() const override {return "PipeDetect";}
    IInstrumentDriver *create() const override {return new PipeDetect::PipeInstrumentDriver(QJsonArray());}
};
