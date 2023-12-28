#pragma once

#include <Ultrasonic/Benchmark.h>
#include <Ultrasonic/IInstrumentDriver.h>
#include <QTcpSocket>

class ARS3000 : public IInstrumentDriver
{
    Q_OBJECT

public:
    ARS3000(QObject *parent = nullptr);

public:
    static UTDataHeader InitDataHeader();

public slots:
    Benchmark *benchmark() override;
    QVariantMap getVariables() const override;
    InstrumentInfo instrumentInfo() const override;
    void setVariables(const QVariantMap &var) override;
    void setIdentifier(const QHash<int, ChannelIdentifier> &identifier) override;

public slots:
    void Connect() override;
    void Disconnect() override;
    void setHostAddr(const QString &ip) override;
    void setChannelSetup(int channel, const UTSetup &setup) override;
    void setDataLevel(int level, int channel) override;
    void setPoseOffset(const UTPose &offset) override;
    void sendCommand(const QString &command) override;
    void sendRawMessage(int type, const QVariant &message) override;
    void resetPose() override;                                                   // 编码器复位

signals:
    void DataLevelChanged(int level, int channel);

private slots:
    void onConnected();
    void onDataArrived();
    void onSocketStateChanged();
    void updateVariables();
    void updateInstrumentInfo(const QJsonObject &obj);
    void sendParameters();

private:
    qint64 parseBinaryData(qint64 size);
    qint64 readADataFrame(UTDataFrame &frame);
    qint64 readCompressedADataFrame(UTDataFrame &frame);
    qint64 readCDataFrame(UTDataFrame &frame, int timeMultiplier);
    ADataValue mapRawAmplitude(ADataValue amp);

private:
    bool alternativeCoder = false;
    QString hostIp;
    QVariantMap variables;
    QTcpSocket socket;
    QMap<int, int> dataLevel;
    QVector<int> rawAmp;
    UTPose poseOffset;
    InstrumentInfo m_info;
    Benchmark m_benchmark;
    QVector<qint32> lastEncoder;
    QVector<qint32> encoderOffset;
    QHash<int, ChannelIdentifier> identifierFront;
    QVector<int> identifierBack;
    QVector<UTSetup> m_setups;                                     // 本地参数表
};

class Ars3000Factory : public IInstrumentDriverFactory
{
    Q_OBJECT

public:
    Ars3000Factory(QObject *parent) : IInstrumentDriverFactory(parent) {}
    QString name() const override {return "ARS3000";}
    IInstrumentDriver *create() const override {return new ARS3000;}
};
