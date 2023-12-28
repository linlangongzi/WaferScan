#pragma once

#include "MotionGlobal.h"
#include <QCoreApplication>
#include "MotionAxisList.h"
#include "MotionCondition.h"
#include "MotionPose.h"
#include "RotationInfo.h"

class MOTION_EXPORT MotionElement
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(MotionElement)

    Q_PROPERTY(Type type MEMBER type)
    Q_PROPERTY(InterpolatePolicy interPolicy MEMBER interPolicy)
    Q_PROPERTY(int mainAxis MEMBER mainAxis)
    Q_PROPERTY(int stepAxis MEMBER stepAxis)
    Q_PROPERTY(qreal stepInterval MEMBER stepInterval)
    Q_PROPERTY(qreal speed MEMBER speed)
    Q_PROPERTY(qreal stepSpeed MEMBER stepSpeed)
    Q_PROPERTY(QList<MotionPose> pos MEMBER pos)

public:
    enum Type {
        None,
        Stop,
        Home,
        Locate,
        MoveLine,
        MoveRel,
        MoveCircle,
        Continuous,
        Cam,
    };
    Q_ENUM(Type)

    enum InterpolatePolicy {
        InterpolateInvalid,
        InterpolateNone,
        InterpolateUnidirectional,
        InterpolateBidiretional,
    };
    Q_ENUM(InterpolatePolicy)

    MotionElement() {}
    MotionElement(const MotionElement &other);
    MotionElement(const MotionAxisList &axisList, const MotionPose &pose);
    MotionElement(const MotionAxisList &axisList, const MotionPose &from, const MotionPose &to);

    MotionAxisList axisList() const {return axis;}
    void setAxisList(const MotionAxisList &list);

    qreal length(int axisIndex = -1) const;
    qreal mainStep(const MotionElement &next) const;
    int stepCount(const MotionElement *next) const;
    QList<MotionPose> movePose(int moveIndex, MotionElement *next) const;
    int interpolateCount(const MotionElement &next) const;
    MotionElement &reverse(bool reverse = true);
    bool canInterpolate(const MotionElement &next, int index = 0) const;
    MotionElement calcInterval(const MotionElement &next) const;
    MotionElement interpolate(const MotionElement &next, int index) const;
    void addOffset(qreal offset, int axis);
    MotionElement modifyRadius(qreal radius);
    MotionElement modifyDepth(qreal depth);

    int moveCount(const MotionElement *next) const;
    QString description() const;
    QJsonValue toJson() const;
    static MotionElement fromJson(const QJsonValue &json);

    bool operator ==(const MotionElement &other) const;
    inline bool operator !=(const MotionElement &other) const {return !(*this == other);}
    MotionElement &operator *=(qreal multiplier);
    MotionElement operator *(qreal multiplier);
    MotionElement &operator +=(const MotionElement &other);
    MotionElement &operator +=(const MotionPose &other);
    MotionElement operator +(const MotionElement &other) const;
    MotionElement &operator -=(const MotionElement &other);
    MotionElement operator -(const MotionElement &other) const;

//private:
    InterpolatePolicy interPolicy = InterpolateNone;
    QList<MotionPose> pos;

    /*! 当 type 为 MoveCircle 时
     * \brief mainAxis: X轴
     * \brief stepAxis: Y轴
     * \brief axis:     C轴 0位置
     */
    Type type = None;
    int mainAxis = -1;
    int stepAxis = -1;
    MotionAxisList axis;

    qreal speed = 0;
    qreal stepSpeed = 0;
    qreal stepInterval = 0;
    QList<MotionCondition> conditions;
};
