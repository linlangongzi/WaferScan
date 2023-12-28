#pragma once

#include "UTSetup.h"
#include <QThread>
#include <QVector>
#include <Ultrasonic/InstrumentSetup.h>
#include <Ultrasonic/UTData.h>
#include <Ultrasonic/ChannelConfig.h>

class QJsonValue;
class Benchmark;

enum DataHeaders {
    HeaderArs3000,
    HeaderPipe,
    HeaderTrain,
    HeaderMdo,
};

class IInstrumentDriver : public QObject
{
    Q_OBJECT

public:
    enum DataLevel {
        NONE = 0,                                                               // 无
        MASK_MONITOR = 1 << 0,
        MASK_CSCAN = 1 << 1,
        MASK_ASCAN = 1 << 2,
        NUM_LEVELS = 3,
        MONITOR_ONLY = MASK_MONITOR,                                            // 只有监视
        CSCAN_ONLY = MASK_MONITOR | MASK_CSCAN,                                 // 监视与C扫
        ASCAN_ONLY = MASK_MONITOR | MASK_ASCAN                                  // 监视与A扫
    };

    enum RawMessageType {
        MessageDigitalIo,
        MessageError,
        MessageEncoder,
        MessageTrainScanStatus,
        MessageTrainSensor,
        MessagePipeStartFlag,
    };

    enum PxiStatus
    {
        Succeed,
        ErrorInitialize,                                                        // 初始化失败
        ErrorFind,                                                              // 找不到卡
        ErrorSelfTest,                                                          // 自检失败不对
        ErrorCardCount,                                                         // 卡数量不对
        ErrorVersion                                                            // 下位机软件版本号
    };
    Q_ENUM(PxiStatus)

public:
    explicit IInstrumentDriver(QObject *parent = nullptr);

public slots:
    virtual InstrumentInfo instrumentInfo() const;
    virtual void setHostAddr(const QString &ip) = 0;
    virtual void setActiveChannel(int channel);
    virtual void sendCommand(const QString &command);
    virtual QVariantMap getVariables() const;
    virtual void setVariables(const QVariantMap &var);
    virtual Benchmark *benchmark() {return nullptr;}
    virtual QVariant supportedSetup(const UTSetup &setup, const QString &key) const;

signals:                                                                        // 信号在派生类中
    void messageReceived(QString message);
    void dataArrived(UTDataSegment data);
    void monitorDataArrived(UTDataFrame data);
    void rawMessageArrived(int type, QVariant data);
    void instrumentInfoChanged(InstrumentInfo info);
    void instrumentStatusChanged(InstrumentStatus status);

public slots:
    virtual void Connect() = 0;
    virtual void Disconnect() = 0;
    virtual void setChannelSetup(int channel, const UTSetup &setup);
    virtual void setInstrumentSetup(const InstrumentSetup &setup);
    virtual void sendRawMessage(int type, const QVariant &message);
    virtual void setIdentifier(const QHash<int, ChannelIdentifier> &identifier);

    virtual void setDataLevel(int level, int channel) = 0;
    virtual void resetPose() {}                                                 // 编码器复位
    virtual void setPoseOffset(const UTPose &offset);
    virtual void setActiveChannel(quint32 channelIndex) {Q_UNUSED(channelIndex)}
    virtual void setPRF(quint16 prf) {Q_UNUSED(prf)}

protected:
    void setInstrumentStatus(InstrumentStatus::State state, const QString &errorString = QString());
    InstrumentStatus instrumentStatus() const;

private:
    InstrumentInfo info;
    InstrumentStatus status;
};

class IInstrumentDriverFactory : public QObject
{
    Q_OBJECT

public:
    IInstrumentDriverFactory(QObject *parent = nullptr);
    ~IInstrumentDriverFactory() override;

    static QList<IInstrumentDriverFactory *> allFactories();

    virtual QString name() const = 0;
    virtual IInstrumentDriver *create() const = 0;
};
