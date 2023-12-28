#ifndef UTINSTRUMENT_H
#define UTINSTRUMENT_H

#include "ConfigObject/Config.h"
#include "UTChannel.h"
#include <QMap>

//#define START 0    // 起始编号
// 不同缺陷类型的探头序号范围
#define ARM_RIM_AXIAL_TREAD_START 0         // 轮辋轮辐轴向踏面
#define FLANGE_RADIAL_START 1000            // 轮缘径向
#define RIM_RADIAL_START 2000               // 轮辋径向
#define ARM_RADIAL_START 3000               // 轮幅径向
// 不同缺陷类型的探头序号范围
// 轮辋轮辐轴向踏面
#define ARM_RIM_AXIAL_MAX (ARM_RIM_AXIAL_TREAD_START + 191)
#define ARM_RIM_AXIAL_MIN (ARM_RIM_AXIAL_TREAD_START)

// 轮缘径向
#define FLANGE_RADIAL_MAX (FLANGE_RADIAL_START + 191)
#define FLANGE_RADIAL_MIN (FLANGE_RADIAL_START)

// 轮辋径向
#define RIM_RADIAL_MAX (RIM_RADIAL_START + 253)
#define RIM_RADIAL_MIN (RIM_RADIAL_START)

// 轮幅径向
#define ARM_RADIAL_MAX (ARM_RADIAL_START + 191)
#define ARM_RADIAL_MIN (ARM_RADIAL_START)

class UTInstrument : public Config
{
    Q_OBJECT
    CONFIG_OBJECT

public:
    typedef QSharedPointer<UTChannel> ChannelPointer;
    typedef QVector<ChannelPointer> ChannelList;
    typedef QMap<int, int> ChannelMap;

protected:
    explicit UTInstrument(QObject *parent = 0);
    ConfigObject *New(QObject *parent) const;

public:
    virtual void Set(const QString &key, const QJsonValue &value);
    int GetNumChannels();
    UTChannel *GetChannel(int index);
    void ConnectReceivedData(UTChannel *channel);
    void DisconnectReceivedData(UTChannel *channel);
    ChannelList GetChannels();
    static ChannelList GetAllChannels();
    const QStringList GetCDataDescription();
    ChannelMap *GetChannelMap();

signals:
    void DataArrived(UTSharedDataSegment sharedata);
    void MonitorDataArrived(UTSharedDataFrame sharedata);
    void DataLevelChanged();

private:
    void InitInstrument();
    void BindSetupToChannel(int channel, ConfigID id);
    void InitChannelMap();
    void InitProbeNo();
    void InitChannelList();

private slots:
    virtual void LoadCompleted();

private:
    ChannelList channels;
    bool bInitCompleted;
    ChannelMap m_channelMap;
};

#endif // UTINSTRUMENT_H
