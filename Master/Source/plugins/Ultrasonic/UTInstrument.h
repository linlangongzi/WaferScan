#pragma once

#include <Ultrasonic/IInstrumentDriver.h>
#include "UTChannel.h"
#include <QTextDocument>

/*!
 * UTInstrument 是一个超声仪器的包装类, 对外提供一个超声仪器的统一接口, 提供仪器初始化,
 * 属性配置, 数据交换等服务.
 *
 * UTInstrument 不与仪器进行直接数据交换, 而是依赖 IInstrumentDriver 与仪器进行通讯.
 *
 * 注意, IInstrumentDriver 与 UTInstrument 不在同一个线程中, 在线程初始化完成后, 不得
 * 进行任何直接调用.
 */
class UTInstrument : public QObject
{
    Q_OBJECT
    friend class UTChannel;
    friend class UltrasonicManager;

    Q_PROPERTY(InstrumentConfig config READ config WRITE setConfig NOTIFY configChanged)
    Q_PROPERTY(InstrumentStatus instrumentStatus READ instrumentStatus NOTIFY instrumentStatusChanged)
    Q_PROPERTY(InstrumentInfo instrumentInfo READ instrumentInfo NOTIFY instrumentInfoChanged)
    Q_PROPERTY(InstrumentSetup setup READ setup WRITE setSetup NOTIFY setupChanged)

public:
    explicit UTInstrument(QObject *parent = nullptr);
    ~UTInstrument();

public:
    QString name() const;
    int channelCount() const;
    UTChannel *channel(int index) const;
    QList<UTChannel *> channels() const;

    InstrumentConfig config() const;
    InstrumentStatus instrumentStatus() const;
    InstrumentInfo instrumentInfo() const;
    InstrumentSetup setup() const;

    QTextDocument *messageLog() const;
    void loadJson(const QJsonObject &json);
    QJsonObject toJson() const ;

    void SetStandardPos(const UTPose &pos);
    IInstrumentDriver *instrumentDriver() const;

public slots:
    void setConfig(const InstrumentConfig &config);
    void setSetup(const InstrumentSetup &setup);
    void Connect();                                                             /* 连接到远程仪器。检查是否存在Driver，如果不存在，先InitDriver()，然后连接 */
    void Disconnect();
    void setActiveChannel(int channel);                                         //  当前使用通道改变
    void resetPose();
    void sendCommand(const QString &command);
    void sendRawMessage(int type, const QVariant &message);

signals:
    void dataArrived(UTDataSegment sharedata);
    void MonitorDataArrived(UTDataFrame sharedata);
    void rawMessageArrived(int type, QVariant message);
    void configChanged(const InstrumentConfig &config);
    void instrumentStatusChanged(InstrumentStatus status);
    void instrumentInfoChanged(InstrumentInfo info);
    void setupChanged(InstrumentSetup setup);

private:
    void initDriver();
    void destroyDriver();

private slots:
    void setInstrumentStatus(const InstrumentStatus &status);
    void setInstrumentInfo(const InstrumentInfo &info);
    void onMonitorDataArrived(UTDataFrame frame);
    void onDataArrived(UTDataSegment segment);
    void setChannelSetup(int channel, const UTSetup &setup);
    void onMessageReceived(const QString &text);

private:
    QList<UTChannel *> m_channels;
    IInstrumentDriver *driver = nullptr;
    QThread driverThread;
    InstrumentConfig m_config;
    InstrumentStatus m_instrumentStatus;
    InstrumentInfo m_info;
    InstrumentSetup m_setup;
    QTextDocument *m_messageLog = nullptr;
    UTDataFrame m_lastFrame;
    int m_index = -1;
};
