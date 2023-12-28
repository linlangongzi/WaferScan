#include "AxisKinematics.h"
#include <allrising/Util.h>

REGISTER_JSON_CONVERTER(AxisKinematics);

void AxisKinematics::addTransform(QMatrix4x4 &m, qreal pos) const
{
    switch (type) {
    case TRANSLATE: m.translate(multiplier * pos + constant); break;            /* 固定偏移对应的multiplier=[0,0,0] */
    case ROTATE:    m.rotate(multiplier.x() * pos + constant.x(), axis); break; /* 旋转轴下的constant一般为[0,0,0] */
    default:        break;
    }
}

QMatrix4x4 AxisKinematics::transform(qreal pos)
{
    QMatrix4x4 m;
    addTransform(m, pos);
    return m;
}

AxisKinematics AxisKinematics::fromJson(const QJsonValue &json)
{
    AxisKinematics k;
    const QJsonObject obj = json.toObject();

    k.type = KeyToEnum(obj["Type"].toString(), NONE);
    k.multiplier = convertFromJson<QVector3D>(obj["Multiplier"]);
    k.constant = convertFromJson<QVector3D>(obj["Constant"]);
    k.axis = convertFromJson<QVector3D>(obj["Axis"]);

    return k;
}

QJsonValue AxisKinematics::toJson() const
{
    QJsonObject obj;
    obj["Multiplier"] = convertToJson(multiplier);
    obj["Constant"] = convertToJson(constant);
    obj["Axis"] = convertToJson(axis);
    obj["Type"] = EnumToKey(type);
    return obj;
}
