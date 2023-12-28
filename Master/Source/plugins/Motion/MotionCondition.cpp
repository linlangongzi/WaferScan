#include "MotionCondition.h"
#include <QJsonObject>
#include <allrising/Util.h>

REGISTER_JSON_CONVERTER(MotionCondition);

QJsonValue MotionCondition::toJson() const
{
    QJsonObject obj;
    obj["Type"] = EnumToKey(type);
    obj["Arg"] = arg;
    obj["RepeatCount"] = repCount;
    obj["Axis"] = axis;
    return obj;
}

MotionCondition MotionCondition::fromJson(const QJsonValue &json)
{
    MotionCondition cond;
    const QJsonObject obj = json.toObject();
    cond.type = KeyToEnum(obj["Type"].toString(), ConditionNone);
    cond.arg = obj["Arg"].toDouble();
    cond.repCount = obj["RepeatCount"].toInt(1);
    cond.axis = obj["Axis"].toInt(-1);
    return cond;
}

bool MotionCondition::operator ==(const MotionCondition &other) const
{
    return type == other.type && qFuzzyCompare(arg, other.arg) && repCount == other.repCount && axis == other.axis;
}
