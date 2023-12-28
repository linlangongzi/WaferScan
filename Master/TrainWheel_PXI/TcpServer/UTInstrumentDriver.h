#ifndef UTINSTRUMENTDRIVER_H
#define UTINSTRUMENTDRIVER_H

#include <QThread>
#include <QJsonObject>
#include <QVector>
#include <QHostAddress>

class UTInstrumentDriver : public QObject
{
    Q_OBJECT

public:
    typedef QJsonObject ParameterTable;

    enum ConnectionStatus
    {
        CONNECTED,
        DISCONNECTED,
        SYNC_ERROR,
        INITIALIZING
    };

    enum DataLevel
    {
        NONE = 0,        // 无
        MASK_MONITOR = 1 << 0,
        MASK_CSCAN = 1 << 1,
        MASK_ASCAN = 1 << 2,
        MONITOR_ONLY = MASK_MONITOR,    // 只有监视
        CSCAN_ONLY = MASK_MONITOR | MASK_CSCAN,     // 监视与C扫
        ASCAN_ONLY = MASK_MONITOR | MASK_ASCAN           // 监视与A扫
    };

public:
    explicit UTInstrumentDriver(QObject *parent = 0);

public:
    virtual ConnectionStatus GetConnectionStatus() = 0;
    virtual int GetNumChannels() = 0;
    virtual int GetNumAxes() = 0;
    virtual int GetNumGates() = 0;
    virtual const QString &Version() = 0;
    virtual const QString &DeviceType() = 0;
    virtual const QJsonValue GetParameters(int channel, const QString &key) = 0;
    virtual DataLevel GetDataLevel() = 0;
    virtual void SetHostAddr(const QHostAddress &ip) = 0;
    virtual void SetHostAddr(const QString &ip) = 0;
    virtual void SetLocalParamterTable(QVector<ParameterTable> objList) = 0;
    virtual void UpdateLocalParamteratable(int channel, const ParameterTable &table) = 0;
    virtual const QStringList GetCDataDescription() = 0;

signals:   // 信号在派生类中

public slots:
    virtual void Connect() = 0;
    virtual void Disconnect() = 0;
    virtual void Reset() = 0;
    virtual void SendCommand(QByteArray CmdArray) = 0;
    virtual void SetParameter(const QString &key, const QJsonValue &value) = 0;
    virtual void SetChannelParameter(int channel, const QString &key = QString(), const QJsonValue &value = QJsonValue()) = 0;
    virtual void SetDataLevel(DataLevel level) = 0;
    virtual void ResetPose() = 0;  // 编码器复位
};

#endif // UTINSTRUMENTDRIVER_H
