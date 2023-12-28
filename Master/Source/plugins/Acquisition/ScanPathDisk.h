#pragma once

#include <QObject>
#include "ScanPathFactory.h"
#include <allrising/MetaType/MetaType.h>

class ScanPathDiskOptions
{
    Q_GADGET

    Q_PROPERTY(QUuid endPointId MEMBER endPointId)
    Q_PROPERTY(int channel MEMBER channel)
    Q_PROPERTY(QUuid scanAxis MEMBER scanAxis)
    Q_PROPERTY(qreal interval MEMBER interval)
    Q_PROPERTY(qreal angularSpeed MEMBER angularSpeed)
    Q_PROPERTY(bool setupEnable MEMBER setupEnable)
    Q_PROPERTY(UTSetup setup MEMBER setup)
    Q_PROPERTY(QVector3D start MEMBER start)
    Q_PROPERTY(QVector3D end MEMBER end)
    Q_PROPERTY(CoordinateSystem coordinateSystem MEMBER coordinateSystem)
    Q_PROPERTY(bool axisRotationEnable MEMBER axisRotationEnable)
    Q_PROPERTY(QUuid rotationAxis MEMBER rotationAxis)
    Q_PROPERTY(qreal angleStart MEMBER angleStart)
    Q_PROPERTY(qreal angleEnd MEMBER angleEnd)
    Q_PROPERTY(bool rectangleEnable MEMBER rectangleEnable)

public:
    DECLARE_COMPARE_OPERATORS(ScanPathDiskOptions)

    int stepAxis() const;

    enum CoordinateSystem
    {
        Equipment,                                                              // 机械坐标系
        Workpiece                                                               // 工件坐标系
    };
    Q_ENUM(CoordinateSystem)

    QUuid endPointId;                                                           // 机械臂Id
    int channel = 0;                                                            // 扫描通道
    QUuid scanAxis;                                                             // 扫描轴Id
    qreal interval = 0.1;                                                       // 扫描间隔
    qreal angularSpeed = 0;                                                     // 扫描角速度
    bool setupEnable = false;                                                   // 工艺开关
    UTSetup setup;                                                              // 通道工艺
    QVector3D start;                                                            // 扫描起点
    QVector3D end;                                                              // 扫描终点
    CoordinateSystem coordinateSystem = Equipment;                              // 坐标系类型
    bool axisRotationEnable = false;                                            // 有限旋转轴开关
    QUuid rotationAxis;                                                         // 有限旋转轴
    qreal angleStart = 0;                                                       // 有限旋转轴 起始角度
    qreal angleEnd = 0;                                                         // 有限旋转轴 终止角度
    bool rectangleEnable;                                                       // 矩形成像
};

class ScanPathDisk : public ScanPathFactory
{
    Q_OBJECT

public:
    ScanPathDisk(QObject *parent = nullptr);

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
    void setWidgetEnabled(QWidget *widget, bool enable) const override;
};
