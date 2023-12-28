#pragma once

#include "MotionGlobal.h"
#include <allrising/Util.h>
#include <allrising/Range.h>
#include "AxisConfig.h"

namespace Motion {
class TrioDriver;
}

/*!
 *  描述一个设备的一个运动副
 *      其中包括一个运动轴（直线轴或者旋转轴）、若干固定偏移量
 *      运动轴和固定偏移量按一定顺序配置构成一个运动副
 *      运动轴和固定偏移量都可以用平移和旋转来描述，即AxisKinematics class
 */
class MOTION_EXPORT Axis : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUuid uuid READ uuid)
    Q_PROPERTY(AxisConfig config READ config WRITE setConfig NOTIFY configChanged)
    Q_PROPERTY(AxisSetup setup READ setup WRITE setSetup NOTIFY setupChanged)
    Q_PROPERTY(AxisStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)

public:
    enum UnitType {
        UNIT_NONE,
        UNIT_DISTANCE,
        UNIT_SPEED,
        UNIT_ACCEL
    };

public:
    explicit Axis(QObject *parent = nullptr);

    QString name() const;                                                       /* 获取轴名称 */
    int index() const;                                                          /* 获取轴列表中的序号 */
    int axisId() const;                                                         /* 获取trio中的轴号 */
    AxisConfig::AxisType axisType() const;                                      /* 获取轴类型 */
    int moveType() const;
    double position() const;                                                    /* 获取轴位置 */
    QString positionText() const;
    bool isRotation() const;                                                    /* 判断是否为旋转轴 */
    bool isInfiniteRotating() const;                                            /* 判断是否为有限旋转轴 */
    bool isIdle();                                                              /* 获取最新idle状态:判断空闲 */
    QString axisUnit(UnitType type = UNIT_DISTANCE) const;                      /* 获取轴单位文字 */
    double speedJog() const;                                                    /* 获取利用键盘手动控制速度 */
    double speedLocate() const;                                                 /* 获取定位速度，即点到点的速度*/
    void AddTransform(QMatrix4x4 &m, qreal position) const;                     /* 增加变换矩阵链，postion为当前轴坐标 */
    void AddTransformChain(QList<QMatrix4x4> &chain, QMatrix4x4 &m, qreal position) const;  /* 增加变换矩阵链 */
    QMap<QString, int> GetSensorMap();                                          /* 获取传感器列表 */
    void fromJson(const QJsonValue &value);                                     /* 从json表中读取运动轴配置 */
    QJsonObject toJson() const;                                                 /* 将运动轴配置写入json表 */
    QVector3D baseDirection() const;
    QVector3D baseOffset() const;
    bool datumDone() const;
    bool withinLimit(qreal pos);

    static QList<Axis *> GetAxesSortByAxisID();                                 /* 获取以AxisID排序好的Axis集合 */
    static int GetMaxAxisID();                                                  /* 获取轴ID的最大值 */
    QUuid uuid() const;

    AxisConfig config() const;
    AxisSetup setup() const;
    AxisStatus status() const;
    QString moveTypeText() const;

public slots:
    void SetSpeed(double speed);                                                /* 设置-速度 已过滤 */
    void SetFSLimit(double fsLimit);                                            /* 设置-前向行走-最大值 */
    void SetRSLimit(double rsLimit);                                            /* 设置-反向行走-最小值 */

    void cancelMove();                                                          /* 取消运动 */
    void MoveForward();                                                         /* 正向连续运动 */
    void MoveReverse();                                                         /* 反向连续运动 */
    void MoveAbs(double pos);                                                   /* 移动到绝对位置 */
    void MoveRel(double pos);                                                   /* 增量式移动 */

    void setConfig(const AxisConfig &config);
    void setSetup(const AxisSetup &setup);
    void setIndex(int index);

signals:
    void configChanged(AxisConfig config);
    void setupChanged(AxisSetup setup);
    void statusChanged(AxisStatus status);
    void indexChanged(int index);

private:
    void setUuid(const QUuid &uuid);                                            /* UUID仅允许MotionManager分配 */
    void setStatus(const AxisStatus &status);
    void SetAxisParameter(const QString &key, qreal value);           /* 设置轴参数 */
    void InitParameters();

private:
    QUuid m_uuid;
    AxisConfig m_config;
    AxisSetup m_setup;
    AxisStatus m_status;
    int m_index = -1;

    friend class MotionManager;
    friend class Motion::TrioDriver;
};
