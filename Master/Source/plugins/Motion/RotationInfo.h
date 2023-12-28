#pragma once

#include "MotionGlobal.h"
#include <QMetaType>
#include <QCoreApplication>

class MOTION_EXPORT RotationInfo
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(RotationInfo)

public:
    enum Type {
        None,
        SurfaceContact,
        AxisContact,
    };
    Q_ENUM(Type)

    enum SpeedType {
        Uniform,
        LimitAngularSpeed
    };
    Q_ENUM(SpeedType)

    enum DirectionType {
        Forward,
        Reverse
    };
    Q_ENUM(DirectionType)

    Q_PROPERTY(int axis MEMBER axis)
    Q_PROPERTY(Type type MEMBER type)
    Q_PROPERTY(DirectionType direction MEMBER direction)
    Q_PROPERTY(qreal axisDiameter MEMBER axisDiameter)
    Q_PROPERTY(qreal partDiameter MEMBER partDiameter)
    Q_PROPERTY(SpeedType speedType MEMBER speedType)
    Q_PROPERTY(qreal maxAngularSpeed MEMBER maxAngularSpeed)

    RotationInfo(Type t = None, qreal axisD = 0, qreal partD = 0);

    bool isValid() const;
    QString description() const;

    qreal nativeSpeed(qreal surfaceSpeed) const;
    qreal surfaceSpeed(qreal nativeSpeed) const;
    qreal nativeDegrees(qreal partDegrees) const;

    QJsonValue toJson() const;
    static RotationInfo fromJson(const QJsonValue &json);
    static RotationInfo byAxisIndex(const QList<RotationInfo> &rotations, int axis);

    bool operator ==(const RotationInfo &other) const;
    bool operator !=(const RotationInfo &other) const;

    int axis = -1;
    Type type = None;
    DirectionType direction = Forward;
    qreal axisDiameter = 0;
    qreal partDiameter = 0;
    SpeedType speedType = Uniform;
    qreal maxAngularSpeed = 300;
};
