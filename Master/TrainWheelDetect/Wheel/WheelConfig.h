#pragma once

#include <QObject>
#include <allrising/Util.h>

class ChannelGroup
{
    Q_GADGET

    Q_PROPERTY(int start MEMBER start)
    Q_PROPERTY(int end MEMBER end)
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(qreal interval MEMBER interval)
    Q_PROPERTY(int view MEMBER view)

public:
    DECLARE_COMPARE_OPERATORS(ChannelGroup)

    int start = 0;
    int end = 0;
    QString name;
    qreal interval = 1;
    int view = -1;
};

Q_DECLARE_METATYPE(ChannelGroup)

class FeatureFilterConfig
{
    Q_GADGET

    Q_PROPERTY(FeatureType featureType MEMBER featureType)
    Q_PROPERTY(qreal min MEMBER min)
    Q_PROPERTY(qreal max MEMBER max)

public:
    enum FeatureType {
        GateITime,
        GateIAmp,
        GateATime,
        GateAAmp,
        GateBTime,
        GateBAmp,
        GateI2Time,
        GateI2Amp
    };

    Q_ENUM(FeatureType)
    DECLARE_COMPARE_OPERATORS(FeatureFilterConfig)

    static FeatureFilterConfig fromString(const QString &string);

    FeatureType featureType = GateITime;
    qreal min = -1;
    qreal max = -1;
};

Q_DECLARE_METATYPE(FeatureFilterConfig)

class ChannelConfig
{
    Q_GADGET

    Q_PROPERTY(int index MEMBER index)
    Q_PROPERTY(int preBox MEMBER preBox)
    Q_PROPERTY(int channel MEMBER channel)
    Q_PROPERTY(int sensorId MEMBER sensorId)
    Q_PROPERTY(int transfer MEMBER transfer)
    Q_PROPERTY(int location MEMBER location)
    Q_PROPERTY(int aecCardId MEMBER aecCardId)
    Q_PROPERTY(int aecChannel MEMBER aecChannel)
    Q_PROPERTY(int averageSize MEMBER averageSize)
    Q_PROPERTY(int frameNumber MEMBER frameNumber)
    Q_PROPERTY(int softChannel MEMBER softChannel)
    Q_PROPERTY(int instrumentId MEMBER instrumentId)
    Q_PROPERTY(int removeAfterSwitch MEMBER removeAfterSwitch)
    Q_PROPERTY(qreal position MEMBER position)
    Q_PROPERTY(qreal removeBack MEMBER removeBack)
    Q_PROPERTY(qreal removeFront MEMBER removeFront)
    Q_PROPERTY(QString ip MEMBER ip)
    Q_PROPERTY(QString channelGroup MEMBER channelGroup)
    Q_PROPERTY(MergeType merge MEMBER merge)
    Q_PROPERTY(QList<FeatureFilterConfig> judgments MEMBER judgments)
    Q_PROPERTY(QList<FeatureFilterConfig> featureFilters MEMBER featureFilters)
    Q_PROPERTY(FeatureFilterConfig::FeatureType featureType MEMBER featureType)

public:
    DECLARE_COMPARE_OPERATORS(ChannelConfig)

    enum MergeType {
        Min,
        Max
    };
    Q_ENUM(MergeType)

    int index = 0;                                                              // 序号
    int preBox = 0;                                                             // 前置机箱
    int channel = -1;                                                           // 通道
    int transfer = 1;                                                           // 转接板
    int location = 0;                                                           // 位置号
    int sensorId = -1;                                                          // 传感器
    int aecCardId = -1;                                                         // aec卡序号
    int aecChannel = -1;                                                        // aec卡通道
    int averageSize = 1;                                                        // 移动平均次数
    int frameNumber = 0;                                                        // 最低伤时帧数半径
    int softChannel = -1;                                                       // 软通道号
    int instrumentId = 0;                                                       // 仪器Id
    int removeAfterSwitch = 0;                                                  // 触感器切换时的降噪数
    qreal position = 0;                                                         // 实际安装位置 mm
    qreal removeBack = 0;                                                       // 后端过滤比例
    qreal removeFront = 0;                                                      // 前端过滤比例
    QString ip;                                                                 // 仪器Ip
    QString channelGroup;
    MergeType merge = Max;
    QList<FeatureFilterConfig> judgments;
    QList<FeatureFilterConfig> featureFilters;
    FeatureFilterConfig::FeatureType featureType = FeatureFilterConfig::FeatureType::GateAAmp;
};

Q_DECLARE_METATYPE(ChannelConfig)

class WheelSetLayout
{
    Q_GADGET

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(QString pattern MEMBER pattern)
    Q_PROPERTY(QString trainType MEMBER trainType)
    Q_PROPERTY(QString setupPath MEMBER setupPath)
    Q_PROPERTY(QList<int> wheelSetLayout MEMBER wheelSetLayout)
    Q_PROPERTY(QList<QString> wheelName MEMBER wheelName)

public:
    DECLARE_COMPARE_OPERATORS(WheelSetLayout)

    QString name;
    QString pattern;
    QString trainType;
    QString setupPath;
    QList<int> wheelSetLayout;
    QList<QString> wheelName;
};

Q_DECLARE_METATYPE(WheelSetLayout)

class WheelConfig
{
    Q_GADGET

    Q_PROPERTY(int rate MEMBER rate)
    Q_PROPERTY(int radius MEMBER radius)
    Q_PROPERTY(int average MEMBER average)
    Q_PROPERTY(int standard MEMBER standard)

public:
    DECLARE_COMPARE_OPERATORS(WheelConfig)

    int rate = 10;                                                              // 加、减分倍率
    int radius = 30;                                                            // 位置范围半径
    int average = 20;                                                           // 计算轮组平均位置的窗口大小
    int standard = 50;                                                          // 达到此分数认定为真实的轮

};
Q_DECLARE_METATYPE(WheelConfig)

class GlobalConfigOptions
{
    Q_GADGET

    Q_PROPERTY(bool plcOpen MEMBER plcOpen)
    Q_PROPERTY(bool judgeOpen MEMBER judgeOpen)
    Q_PROPERTY(bool autoReport MEMBER autoReport)
    Q_PROPERTY(QString trainType MEMBER trainType)
    Q_PROPERTY(QString documentPath MEMBER documentPath)
    Q_PROPERTY(QString templatePath MEMBER templatePath)
    Q_PROPERTY(WheelConfig wheelConfig MEMBER wheelConfig)
    Q_PROPERTY(QList<WheelSetLayout> wheelSetLayouts MEMBER wheelSetLayouts)
    Q_PROPERTY(QList<ChannelConfig> channelConfig MEMBER channelConfig)
    Q_PROPERTY(QList<ChannelGroup> channelGroups MEMBER channelGroups)

public:
    DECLARE_COMPARE_OPERATORS(GlobalConfigOptions)

    bool plcOpen = false;
    bool judgeOpen = true;
    bool autoReport = true;
    QString trainType;
    QString documentPath;
    QString templatePath;
    WheelConfig wheelConfig;
    QList<WheelSetLayout> wheelSetLayouts;
    QList<ChannelConfig> channelConfig;
    QList<ChannelGroup> channelGroups;
};
Q_DECLARE_METATYPE(GlobalConfigOptions)

class TrainInfo
{
    Q_GADGET

    Q_PROPERTY(int wheelSetCount MEMBER wheelSetCount)
    Q_PROPERTY(qreal speed MEMBER speed)
    Q_PROPERTY(qreal diameter MEMBER diameter)
    Q_PROPERTY(qreal frequency MEMBER frequency)
    Q_PROPERTY(qreal sensitivity MEMBER sensitivity)
    Q_PROPERTY(qreal wheelDiameter MEMBER wheelDiameter)
    Q_PROPERTY(QString trainId MEMBER trainId)
    Q_PROPERTY(QString couplant MEMBER couplant)
    Q_PROPERTY(QString trainType MEMBER trainType)
    Q_PROPERTY(QString entrusted MEMBER entrusted)
    Q_PROPERTY(QString equipment MEMBER equipment)

public:
    DECLARE_COMPARE_OPERATORS(TrainInfo)

    int wheelSetCount = 0;
    qreal speed = 0;
    qreal diameter = 0;
    qreal frequency = 0;
    qreal sensitivity = 0;
    qreal wheelDiameter = 0;
    QString trainId;
    QString couplant = QObject::tr("水");
    QString trainType;
    QString entrusted = QObject::tr("中车集团");
    QString equipment = QObject::tr("奥瑞视车轮检测系统");
};
Q_DECLARE_METATYPE(TrainInfo)
