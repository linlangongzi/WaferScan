#pragma once

#include "ScanPathFactory.h"

class ScanPathRectangleOptions
{
    Q_GADGET

    Q_PROPERTY(bool unidirection MEMBER unidirection)
    Q_PROPERTY(QUuid endPointId MEMBER endPointId)
    Q_PROPERTY(QUuid scanAxisId MEMBER scanAxisId)
    Q_PROPERTY(qreal scanInterval MEMBER scanInterval)
    Q_PROPERTY(bool userLinearSpeedEnable MEMBER userLinearSpeedEnable)
    Q_PROPERTY(qreal linearSpeed MEMBER linearSpeed)
    Q_PROPERTY(qreal stepInterval MEMBER stepInterval)
    Q_PROPERTY(qreal probeInterval MEMBER probeInterval)
    Q_PROPERTY(QList<qreal> probeIntervalList MEMBER probeIntervalList)
    Q_PROPERTY(QVector3D start MEMBER start)
    Q_PROPERTY(QVector3D end MEMBER end)
    Q_PROPERTY(int channel MEMBER channel)
    Q_PROPERTY(QList<int> channels MEMBER channels)
    Q_PROPERTY(ChannelSelectPolicy channelSelect MEMBER channelSelect)
    Q_PROPERTY(UTSetup setup MEMBER setup)
    Q_PROPERTY(WaveformSavePolicy waveformSave MEMBER waveformSave)
    Q_PROPERTY(FeatureSavePolicy featureSave MEMBER featureSave)
    Q_PROPERTY(bool alterSourceEnable MEMBER alterSourceEnable)
    Q_PROPERTY(bool setupEnable MEMBER setupEnable)
    Q_PROPERTY(qreal gainOffset MEMBER gainOffset)

public:
    DECLARE_COMPARE_OPERATORS(ScanPathRectangleOptions)

    int axisIndex(const QUuid &axisId) const;
    QList<int> primaryChannelIdList() const;
    QList<int> secondaryChannelIdList() const;
    qreal calculatedAdvicedSpeed() const;
    qreal effectiveLinearSpeed() const;
    MotionAxisList elementAxisList() const;
    int stepAxis() const;
    QVector3D topRight() const;
    QVector3D stepVector() const ;
    QVector3D scanDirection() const;
    MotionPose elementMotionPose(const QVector3D &vector) const;
    qreal multiProbePatchSpan() const;
    qreal multiProbePatchStep() const;
    QList<MotionElement> motionElements() const;
    void addPatch(QList<MotionElement> &elements, const QVector3D &startOffset, const QVector3D &endOffset) const;

    enum ChannelSelectPolicy
    {
        Single,
        Parallel,
        ParallelList
    };
    Q_ENUM(ChannelSelectPolicy)

    enum WaveformSavePolicy
    {
        WaveformSaveNone,
        WaveformSaveAll,
        WaveformSavePart
    };
    Q_ENUM(WaveformSavePolicy)

    enum FeatureSavePolicy
    {
        FeatureSaveAll,
        FeatureSavePart
    };
    Q_ENUM(FeatureSavePolicy)

    bool unidirection = false;                                                  // 单向扫描开关
    QUuid endPointId;                                                           // 机械臂Id
    QUuid scanAxisId;                                                           // 扫查轴Id
    qreal scanInterval = 0;                                                     // 扫查间隔
    bool userLinearSpeedEnable = false;                                         // 用户指定扫描线速度
    qreal linearSpeed = 0;                                                      // 扫查速度
    qreal stepInterval = 0;                                                     // 步进间隔
    qreal probeInterval = 0;                                                    // 探头间距
    QList<qreal> probeIntervalList;                                             // 不同间距探头的间距列表
    QVector3D start;                                                            // 起始点
    QVector3D end;                                                              // 终点
    int channel = 0;                                                            // 单通道工作时的通道
    QList<int> channels;                                                        // 多通道工作时通道列表
    ChannelSelectPolicy channelSelect = Single;                                 // 通道选择模式
    bool setupEnable = false;                                                   //通道工艺开关
    UTSetup setup;                                                              // 通道工艺
    WaveformSavePolicy waveformSave = WaveformSaveNone;                         // 波形数据保存模式
    FeatureSavePolicy featureSave = FeatureSaveAll;                             // 特征值保存模式
    bool alterSourceEnable = false;                                             // 关联检测开关
    qreal gainOffset = 0;                                                       // 增益补偿
};

class ScanPathRectangle : public ScanPathFactory
{
    Q_OBJECT

public:
    ScanPathRectangle(QObject *parent = nullptr);

    QString name() const override;
    QString displayName() const override;
    QIcon icon() const override;
    MotionPath motionPath(const QVariant &options) const override;
    QHash<int, UTSetup> setups(const QVariant &options) const override;
//    QList<RecordConfig> recordOptions(const QVariant &options) const override;
//    SafetyBarrierOptions safetyBarrier(const QVariant &options) const override;
    QList<ImageOptions> imageOptions(const QVariant &options) const override;
    QPair<qreal, qreal> boundingRange(const QVariant &options, int axis) const override;
    CheckResult checkOptions(const QVariant &options) const override;
    QWidget *widget(const QVariant &options) const override;
    QVariant widgetOptions(QWidget *widget) const override;

private:
    QHash<int, QVector<float> > channelOffset(const ScanPathRectangleOptions &o) const;
    QList<MotionElement> motionElements(const ScanPathRectangleOptions &options) const;
    void addPatch(QList<MotionElement> &elements, MotionPose startOffSet, MotionPose endOffset, const MotionElement &element) const;

};
