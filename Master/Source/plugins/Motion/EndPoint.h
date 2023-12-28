#pragma once

#include "Axis.h"
#include "MotionPose.h"
#include <QDateTime>
#include <QTimer>
#include <QUuid>
#include <Motion/MotionAxisList.h>

class Axis;

/*!
 *  描述了设备上某一端点及其下属级的各运动副（Axis class)信息
 *      设备的各运动副为串联关系
 *      从设备原点起，各运动副串联，可构成多个EndPoint
 */
class MOTION_EXPORT EndPoint : public QObject
{
    Q_OBJECT

    Q_PROPERTY(EndPointType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QUuid uuid READ uuid)
    Q_PROPERTY(QList<Axis *> axisList READ axisList WRITE setAxisList NOTIFY axisListChanged)
    Q_PROPERTY(QVector3D offset READ offset WRITE setOffset)

public:
    enum EndPointType
    {
        None,
        Cartesian                                                               // X、Y、Z、B、A
    };
    Q_ENUM(EndPointType)

    explicit EndPoint(QObject *parent = nullptr, EndPointType type = None);
    void setAxes(const MotionAxisList &axes);
    int axisIndex(const QUuid &uuid) const;
    QUuid axisUuid(int index) const;
    Axis *axis(int index) const;
    Axis *axis(const QUuid &id) const;
    int axisCount() const;
    MotionPose motionPose(const QVector3D &position);
    MotionPose motionPose(const QVector3D &position, const QVector3D &direction);
    MotionPose motionPose(const QVector3D &position, qreal angleFirst, qreal angleSecond);
    QVector3D position(const MotionPose &pose);
    QVector3D direction(const MotionPose &pose, Qt::Axis axis = Qt::ZAxis);

    static QPair<qreal, qreal> directionBA(const QVector3D &direction);
    static QPair<qreal, qreal> directionCB(const QVector3D &direction);
    static QVector3D directionFromBA(qreal b, qreal a);
    static QVector3D directionFromCB(qreal c, qreal b);

    bool isValid() const;

    static QList<Axis *> axesOfType(EndPointType type);

public:
    void loadJson(const QJsonValue &value);
    QJsonValue toJson() const;

    QString description() const;                                                /* 获取自由度汇总 */
    MotionPose currentPos(const MotionAxisList &axes = MotionAxisList()) const; /* 获取Endpoint中所有轴位置 */
    QHash<QUuid, qreal> currentPose();
    QString currentPosText() const;
    bool isIdle();
    void AddTransform(QMatrix4x4 &m, const MotionPose &pose) const;             /* 添加变换矩阵 */
    void AddTransformChain(QList<QMatrix4x4> &chain, QMatrix4x4 &m, const MotionPose &pose) const; /* 追加变换矩阵链 */
    QMatrix4x4 GetTransform(const MotionPose &pose) const;                      /* 获取变换矩阵 */
    QHash<QUuid, qreal> filterPose(const QHash<QUuid, qreal> &pose) const;

    EndPointType type() const;
    QString name() const;
    QUuid uuid() const;
    QList<Axis *> axisList() const;
    QVector3D offset() const;
    QVector3D offsetFromBA(qreal angleB, qreal angleA);

public slots:
    void MoveAbs(const QHash<QUuid, qreal> &pose); /* 移动到位置position */
    void Cancel();                                                              /* 取消运动 */
    void Locate(const MotionPose &position, const MotionAxisList &axes = MotionAxisList()); /* 空走,专门用于定位 */
    void Locate(const QHash<QUuid, qreal> &pos);
    void setType(EndPointType type);
    void setName(const QString &name);
    void setAxisList(const QList<Axis *> &axisList);
    void setOffset(const QVector3D offset);

signals:
    void typeChanged(EndPointType type);
    void PositionUpdated(const MotionPose &position);                           /* 当Endpoint发生位置变化,发送最新位置 */
    void axisListChanged(QList<Axis *> axisList);
    void EndPointIdle();

private slots:
    void setUuid(const QUuid &uuid);                                            /* 仅允许MotionManager配置UUID */
    void onAxisStatusChanged();
    void updateAxisList();

private:
    void timerEvent(QTimerEvent *event) override;

private:
    QString m_name;
    QUuid m_uuid;
    EndPointType m_type;
    QList<QUuid> m_axisUuidList;                                                /* 与轴对象列表的顺序对应的Uuid列表 */
    QList<Axis *> m_axisList;                                                   /* 轴对象列表 */
    QList<int> m_axisIdList;                                                    /* trio中轴ID列表 */
    QList<int> m_axisIndexList;                                                 /* 与轴对象列表的顺序对应的index列表 */
    bool prevIdle = false;
    QDateTime prevIdleTime;
    QVector3D m_offset;

    friend class MotionManager;
};
