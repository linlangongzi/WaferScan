#pragma once

#include "ChannelConfig.h"
#include "Probe.h"
#include "UTSetup.h"
#include "UltrasonicGlobal.h"
#include <QJsonValue>
#include <QPointer>
#include <QTransform>
#include <Ultrasonic/UTData.h>

class Probe;
class UTSetup;
/*!
 *  描述了仪器通道信息：绑定的探头、检测工艺等
 */
class ULTRASONIC_EXPORT UTChannel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ChannelConfig config READ config WRITE setConfig NOTIFY configChanged)
    Q_PROPERTY(UTSetup setup READ setup WRITE setSetup NOTIFY setupChanged)
    Q_PROPERTY(Probe probe READ probe WRITE setProbe NOTIFY probeChanged)

    friend class UTInstrument;
    friend class UltrasonicManager;
    friend class Probe;

public:
    explicit UTChannel(QObject *parent = nullptr);

    QString name() const;                                                       /* 获取通道名称 */
    QString description() const;
    UTInstrument *instrument() const;                                           /* 获取通道所有的仪器 */
    void setSetup(const UTSetup &setup);                                        /* 设置工艺 */
    Probe probe() const;                                                        /* 获取绑定的探头 */
    void addTransform(QMatrix4x4 &matrix, const UTPose &pose, qreal waterPath); /* 添加变换矩阵 */
    QList<QMatrix4x4> transformChain(const UTPose &pose, qreal waterPath) const;/* 获取变换矩阵链 */
    QUuid probeUuid() const;                                                    /* 获取绑定的探头ID */
    UTSetup setup() const;                                                      /* 获取绑定的工艺设置 */
    ChannelConfig config() const;
    int instrumentIndex() const;                                                /* 获取通道的索引号 */
    int channelId() const;                                                      /* 获取全局索引 */
    UTDataFrame lastFrame() const;

    void setName(const QString &name);                                          /* 设置通道名称 */
    void setDataLevel(int level);                                               /* 设置保存数据模式 */

    void fromJson(const QJsonObject &json);
    QJsonValue toJson() const;

public slots:
    void setChannelId(int id);
    void setConfig(const ChannelConfig &config);
    void setDescription(const QString &description);
    void setProbe(const Probe &probe);

signals:
    void probeChanged(const Probe &probe);                                      /* 绑定的探头发生改变 */
    void dataArrived(UTDataSegment sharedata);                                  /* 数据包到达 */
    void MonitorDataArrived(UTDataFrame sharedata);                             /* 数据帧到达 */
    void configChanged(ChannelConfig config);
    void setupChanged(UTSetup setup);

private slots:
    void refreshInstrumentBinding();

private:
    Probe m_probe;                                                              /* 绑定的探头 */
    UTSetup m_setup;                                                            /* 绑定的工艺 */
    ChannelConfig m_config;
    QString m_description;
    UTDataFrame m_lastFrame;                                                    /* 超声数据帧指针 */
    QPointer<UTInstrument> m_instrument;
};
