#pragma once

#include "MotionGlobal.h"
#include <QMetaType>

class MOTION_EXPORT MotionCondition
{
    Q_GADGET

public:
    enum Type {
        ConditionNone,
        ConditionIoRising,
        ConditionIoFalling,
        ConditionDelay,
    };
    Q_ENUM(Type)

    Q_PROPERTY(Type type MEMBER type)
    Q_PROPERTY(int axis MEMBER axis)
    Q_PROPERTY(qreal arg MEMBER arg)
    Q_PROPERTY(int repCount MEMBER repCount)

    QJsonValue toJson() const;
    static MotionCondition fromJson(const QJsonValue &json);

    bool operator ==(const MotionCondition &other) const;
    inline bool operator !=(const MotionCondition &other) const {return !(*this == other);}

    Type type = ConditionNone;
    qreal arg = 0;
    int repCount = 1;
    int axis = -1;
};
