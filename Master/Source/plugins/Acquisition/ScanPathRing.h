#pragma once

#include "ScanPathFactory.h"

#include <allrising/Range.h>

class ScanPathRingOptions
{
    Q_GADGET

    Q_PROPERTY(int scanAxis MEMBER scanAxis)
    Q_PROPERTY(qreal linearSpeed MEMBER linearSpeed)
    Q_PROPERTY(qreal stepInterval MEMBER stepInterval)
    Q_PROPERTY(qreal angleStart MEMBER angleStart)
    Q_PROPERTY(qreal angleEnd MEMBER angleEnd)
    Q_PROPERTY(qreal pressingAngle MEMBER pressingAngle)

    Q_PROPERTY(StepDirection direction MEMBER direction)
    Q_PROPERTY(CenterType centerType MEMBER centerType)
    Q_PROPERTY(QPointF center MEMBER center)
    Q_PROPERTY(qreal radius MEMBER radius)
    Q_PROPERTY(qreal innerRadius MEMBER innerRadius)
    Q_PROPERTY(qreal depth MEMBER depth)
    Q_PROPERTY(qreal height MEMBER height)

    Q_PROPERTY(QList<ChannelList> multiChannels MEMBER multiChannels)
    Q_PROPERTY(qreal probeInterval MEMBER probeInterval)

public:
    enum CenterType {
        DirectInput,
        Calculate
    };
    Q_ENUM(CenterType)

    enum StepDirection {
        Above,
        Inside,
        Outside
    };
    Q_ENUM(StepDirection)

    DECLARE_COMPARE_OPERATORS(ScanPathRingOptions)

    int scanAxis = 3;                                                           // 扫描轴
    qreal linearSpeed = 10;                                                     // 扫描线速度
    qreal stepInterval = 1;                                                     // 步进间隔
    qreal angleStart = 0;                                                       // 起始角度
    qreal angleEnd = 360;                                                       // 终止角度
    qreal pressingAngle = 45;                                                   // 下压角度

    StepDirection direction = Above;                                            // 步进方向
    CenterType centerType = DirectInput;                                        // 圆心确定方式
    QPointF center;                                                             // 工件圆心
    qreal radius = 0;                                                           // 工件半径
    qreal innerRadius = 0;                                                      // 工件内径
    qreal depth = 0;                                                            // 工件上表面深度
    qreal height = 0;                                                           // 工件高度

    QList<ChannelList> multiChannels;                                           // 通道列表
    qreal probeInterval = 0;                                                    // 探头间距
};

class ScanPathRing : public ScanPathFactory
{
    Q_OBJECT

public:
    explicit ScanPathRing(QObject *parent = nullptr);

    QString name() const override;
    QString displayName() const override;
    QIcon icon() const override;
    MotionPath motionPath(const QVariant &options) const override;
    QHash<int, UTSetup> setups(const QVariant &options) const override;
    QList<ImageOptions> imageOptions(const QVariant &options) const override;
    QPair<qreal, qreal> boundingRange(const QVariant &options, int axis) const override;
//    QList<RecordConfig> recordOptions(const QVariant &options) const override;
//    SafetyBarrierOptions safetyBarrier(const QVariant &options) const override;
    QWidget *widget(const QVariant &options) const override;
    QVariant widgetOptions(QWidget *widget) const override;

private:
    ImageOptions imageOption(const ScanPathRingOptions &option, int index, int axisX, int axisY) const;
    QPair<int, int> imageAxes(const ScanPathRingOptions &option) const;

    QList<MotionElement> expandByDirection(MotionElement &base, const ScanPathRingOptions &option, qreal start, qreal end) const;
};
