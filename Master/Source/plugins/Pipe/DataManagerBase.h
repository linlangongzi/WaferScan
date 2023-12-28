#pragma once

#include "Ars1000/PipeDataDef.h"
#include "PipeGlobalConfig.h"
#include <Imaging/DialogImageOptions.h>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QObject>
#include <QPolygonF>
#include <Ultrasonic/UTData.h>

class DataManagerBase : public QObject
{
    Q_OBJECT

public:
    typedef QVector<int> ProbeBoxIndexVector;
    typedef QList<int> ChannelListOfMode;

public:
    explicit DataManagerBase(QObject *parent = nullptr);
    virtual void Connect();
    virtual void Disconnect();
    void SetProbeBoxStatus(int status);
    QVector<QVector<float>> GetChannelOffsetList();
    QList<qreal> channelOffset(int featureIndex);
    int GetProbeBoxStatus() const;

signals:
    void SegmentArrived(UTDataSegment segment);

public slots:
    virtual void dataArrived(const UTDataSegment &segment);
    void OnStartFlagArrived(quint16 flag);
    void setConfig(const PipeGlobalConfig &globalConfig);

protected:
    ProbeBoxIndexVector probeBoxIndexVector;
    quint16 startFlag = 0;
    int probeBoxStatus;                     // 使用的探头盒状态
    PipeGlobalConfig config;
};
