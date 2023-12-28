#ifndef UTDRIVERPXI100_H
#define UTDRIVERPXI100_H

#include <QMutex>
#include <QDateTime>
#include <QTcpSocket>
#include <QHostAddress>
#include "Parser.h"
#include "DataDef.h"
#include "ParameterSocket.h"
#include <Ultrasonic/IInstrumentDriver.h>

namespace NS_PXI100 {
class CScanSocket;
class MonitorSocket;
class EyesStatusSocket;
/*!
 * \brief UTDriverPXI100 是 PXI100 仪器的驱动类，在 UTInstrumentDriver 接口类的基础上，
 * 为 PXI100 的实现提供控制和数据交换对象。
 *
 * \sa UTInstrumentDriver
 */
class UTDriverPXI100 : public IInstrumentDriver
{
    Q_OBJECT

public:
    typedef IInstrumentDriver::ConnectionStatus ConnectionStatus;
    typedef IInstrumentDriver::DataLevel DataLevel;

public:
//    explicit UTDriverPXI100(QVector<QJsonObject> obj, int ObjectIndex = 0, QObject *parent = 0);
    explicit UTDriverPXI100(const QJsonArray &config, QObject *parent = 0);

public:
    virtual IInstrumentDriver::ConnectionStatus GetConnectionStatus();
    virtual int GetNumChannels();
    virtual int GetNumAxes();
    virtual int GetNumGates();
    virtual const QString &Version();
    virtual const QString &DeviceType();
    virtual void SetLocalParamterTable(QVector<ParameterTable> objList);
    virtual void UpdateLocalParamteratable(int channel, const ParameterTable &table);
    static QStringList GetCDataDescription();
    static CDataTypeList GetCDataType();

public slots:
    virtual void SetHostAddr(const QHostAddress &ip);
    virtual void SetHostAddr(const QString &ip);
    virtual void Connect();
    virtual void Disconnect();
    virtual void Reset();
    virtual void SetParameter(const QString &key, const QJsonValue &value);
    virtual void SetChannelParameter(int index, int channel, const QString &key = QString(), const QJsonValue &value = QJsonValue());
    virtual void SetCurChannel(qint32 channelIndex, qint16 surOrFlaw);
    virtual void SetRepeatFrequence(qint32 nValue);
    virtual void sendRawMessage(int type, const QVariant &message);

protected:
    QVector<ParameterTable> localParameter;      // 本地参数表

private:
    void InitSockets();

private slots:
    void OnMonitorReceived(UTSharedDataFrame dataSegment);
    void OnCScanReceived(UTSharedDataSegment dataSegment);
    void onMfcDataArrived(const QByteArray &data);

    void OnConnected();
    void OnDisconnected();

private:
    static const int NUM_GATES = 4;
    static const QString version;
    static const QString deviceType;
    IInstrumentDriver::ConnectionStatus status;

    MonitorSocket *m_MonitorSocket;
    CScanSocket *m_CScanSocket;
    ParameterSocket *m_ParameterSocket;
    EyesStatusSocket *m_EyesStatusSocket;
    EyesStatusSocket *m_MfcEyesStatusSocket;

    static const int MONITOR_PORT     = 3333;
    static const int CSCAN_PORT       = 4444;
    static const int PARAM_PORT       = 5555;
    static const int EYES_STATUS_PORT = 6666;
    static const int MFC_EYES_STATUS_PORT = 7777;

    static const QStringList CDataDescription;

    QHostAddress hostName;
    ParserList parsers;   // wangwei add
};

}   /* namespace NS_PXI100 */

#endif // UTDRIVERPXI100_H
