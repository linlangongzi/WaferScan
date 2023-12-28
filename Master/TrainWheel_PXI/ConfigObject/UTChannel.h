#ifndef UTCHANNEL_H
#define UTCHANNEL_H

#include "ARS3D/UTData.h"
#include "ConfigObject/ConfigObject.h"
#include <QJsonValue>
#include <QPointer>

class Probe;
class UTSetup;

class UTChannel : public QObject
{
    Q_OBJECT

    friend class UTInstrument;

    typedef ConfigObject::ConfigID ConfigID;
    typedef QPointer<UTSetup> SetupPointer;

protected:
    explicit UTChannel(QObject *parent = 0);

public:
    void BindUTSetup(ConfigID id);
    void BindUTSetup(UTSetup *setup);
    void UnbindUTSetup();
    virtual void Set(const QString &key, const QJsonValue &value);
    UTSetup *GetBindUTSetup();
    ConfigID GetBindUTSetupID();
    int GetIndex();
    int GetGlobalIndex();

public slots:
    // 工艺修改通道参数（只有工艺可以修改通道参数）
    void SetChannelParamter(const QString& key, const QJsonValue& value);
    // 向下位机请求回来的整个参数表
    QJsonObject GetChannelParamter();

    void OnDriverDataArrived(UTSharedDataSegment sharedata);
    void OnDriverMonitorArrived(UTSharedDataFrame sharedata);

signals:
    void BindUTSetupChanged(int channel, ConfigID id);
    void ChannelParamterChanged(int channel, const QString &key, const QJsonValue &value);
    void DataArrived(UTSharedDataSegment sharedata);
    void MonitorDataArrived(UTSharedDataFrame sharedata);

protected:
    void SetIndex(int channel, int instrument);    // 设置通道号
    void SetGlobalIndex(int index);

private:
    void connectReceivedData();
    void disconnectReceivedData();
    void ConnectSignalOnSetup();
    void DisonnectSignalOnSetup();

private:
    int instrumentIndex;
    int channelIndex;
    SetupPointer bindSetup;
    int nGlobaIndex;
};

#endif // UTCHANNEL_H
