#pragma once

#include "ScanPathFactory.h"
#include <allrising/MetaType/MetaType.h>

class ScanPathCylinderOptions
{
    Q_GADGET

    Q_PROPERTY(QUuid endPointId MEMBER endPointId)
    Q_PROPERTY(QUuid scanAxis MEMBER scanAxis)
    Q_PROPERTY(qreal scanInterval MEMBER scanInterval)
    Q_PROPERTY(qreal linearSpeed MEMBER linearSpeed)
    Q_PROPERTY(qreal rotatorAngularSpeed MEMBER rotatorAngularSpeed)
    Q_PROPERTY(qreal cylinderAngularSpeed MEMBER cylinderAngularSpeed)
    Q_PROPERTY(qreal cylinderDiameter MEMBER cylinderDiameter)
    Q_PROPERTY(qreal stepInterval MEMBER stepInterval)
    Q_PROPERTY(bool setupEnable MEMBER setupEnable)
    Q_PROPERTY(UTSetup setup MEMBER setup)
    Q_PROPERTY(int channel MEMBER channel)
    Q_PROPERTY(QList<int> channels MEMBER channels)
    Q_PROPERTY(WaveformSavePolicy waveformSave MEMBER waveformSave)
    Q_PROPERTY(ChannelSelectPolicy channelSelect MEMBER channelSelect)
    Q_PROPERTY(FeatureSavePolicy featureSave MEMBER featureSave)
    Q_PROPERTY(bool alterSourceEnable MEMBER alterSourceEnable)
    Q_PROPERTY(qreal gainOffset MEMBER gainOffset)
    Q_PROPERTY(qreal probeInterval MEMBER probeInterval)
    Q_PROPERTY(QList<QVector3D> posList MEMBER posList)
    Q_PROPERTY(CalculationMode mode MEMBER mode)

public:
    DECLARE_COMPARE_OPERATORS(ScanPathCylinderOptions)

    int axisIndex(const QUuid &axisId) const;
    QList<int> primaryChannelIdList() const;
    QList<int> secondaryChannelIdList() const;
    qreal advicedLinearSpeed() const;
    qreal advicedRotatorAngularSpeed() const;
    qreal advicedCylinderAngularSpeed() const;
    qreal axleDiameter() const;
    qreal effectiveLinearSpeed() const;
    qreal effectiveRotatorAngularSpeed() const;
    qreal effectiveCylinderAngularSpeed() const;
    int stepAxis() const;
    RotationInfo pathRotation() const;
    MotionElement requisityElement() const;
    MotionCondition elementCondition() const;
    QSizeF unitScale() const;
    QRectF adjustedSourceRect() const;
    QHash<int, QVector<float>> channelOffset() const;
    MotionAxisList elementAxisList() const;
    MotionPose elementMotionPose(const QVector3D &vector) const;
    void addPatch(QList<MotionElement> &elements, const QVector3D &startOffset, const QVector3D &endOffset, const MotionElement &element) const;
    QList<MotionElement> motionElements(const MotionElement &element) const;

    enum CalculationMode
    {
        AutoCalculation,
        SpecifyLinearSpeed,
        SpecifyRotatorAngularSpeed,
        SpecifyCylinderAngularSpeed
    };
    Q_ENUM(CalculationMode)

    enum ChannelSelectPolicy
    {
        Single,
        Parallel
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

    QUuid endPointId;                                                       // 机械臂Id
    QUuid scanAxis;                                                         // 扫秒轴Id
    qreal scanInterval = 0;                                                 // 扫查间隔
    qreal linearSpeed = 0;                                                  // 扫描线速度
    qreal rotatorAngularSpeed = 0;                                          // 承载轮角速度
    qreal cylinderAngularSpeed = 0;                                         // 工件角速度
    qreal stepInterval = 0;                                                 // 步进间隔
    qreal probeInterval = 0;                                               // 探头间距
    qreal cylinderDiameter = 0;                                             // 工件直径
    bool setupEnable = false;                                               // 通道工艺开关
    UTSetup setup;                                                      // 通道工艺
    int channel = 0;                                                        // 单通道工作时的通道
    QList<int> channels;                                                    // 多通道工作时通道列表
    WaveformSavePolicy waveformSave = WaveformSaveNone;                     // 波形数据保存模式
    ChannelSelectPolicy channelSelect = Single;                             // 通道选择模式
    FeatureSavePolicy featureSave = FeatureSaveAll;                         // 特征值保存模式
    bool alterSourceEnable = false;                                         // 关联检测开关
    qreal gainOffset = 0;                                                   // 增益补偿
    QList<QVector3D> posList;                                               // 取点列表
    CalculationMode mode;                                                   // 用户指定计算模式
};

class ScanPathCylinder : public ScanPathFactory
{
    Q_OBJECT

public:
    ScanPathCylinder(QObject *parent = nullptr);
    QString name() const override;
    QString displayName() const override;
    QIcon icon() const override;
    MotionPath motionPath(const QVariant &options) const override;
    QHash<int, UTSetup> setups(const QVariant &options) const override;
    QList<ImageOptions> imageOptions(const QVariant &options) const override;
    QPair<qreal, qreal> boundingRange(const QVariant &options, int axis) const override;
    CheckResult checkOptions(const QVariant &options) const override;
    QWidget *widget(const QVariant &options) const override;
    QVariant widgetOptions(QWidget *widget) const override;
};
