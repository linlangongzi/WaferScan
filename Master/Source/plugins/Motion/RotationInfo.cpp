#include "RotationInfo.h"
#include <QJsonValue>
#include <allrising/Util.h>
#include <QtMath>
#include <QMetaType>

Q_DECL_UNUSED static const bool registerDebug = QMetaType::registerDebugStreamOperator<RotationInfo>();

RotationInfo::RotationInfo(Type t, qreal axisD, qreal partD) :
    type(t),
    axisDiameter(axisD),
    partDiameter(partD)
{
}

bool RotationInfo::isValid() const
{
    return type == None
            || (type == AxisContact && axisDiameter > 0)
            || (type == SurfaceContact && axisDiameter > 0 && partDiameter > 0);
}

QString RotationInfo::description() const
{
    return tr("轴旋转属性");
}

/*! 根据线速度获取本地速度 */
qreal RotationInfo::nativeSpeed(qreal surfaceSpeed) const
{
    qreal speed = surfaceSpeed;
    if (type == SurfaceContact) {
        if (axisDiameter > 0) {
            speed = qRadiansToDegrees(2 * surfaceSpeed / axisDiameter);
        }
    } else if (type == AxisContact) {
        if (partDiameter > 0) {
            speed = qRadiansToDegrees(2 * surfaceSpeed / partDiameter);
        }
    }
    if (speedType == LimitAngularSpeed) {
        speed = qMin(speed, maxAngularSpeed);
    }
    return speed;
}

/*! 根据本地速度获取线速度 */
qreal RotationInfo::surfaceSpeed(qreal nativeSpeed) const
{
    if (type == SurfaceContact) {
        if (axisDiameter > 0) {
            return qDegreesToRadians(nativeSpeed) * axisDiameter / 2;
        }
    } else if (type == AxisContact) {
        if (partDiameter > 0) {
            return qDegreesToRadians(nativeSpeed) * partDiameter / 2;
        }
    }
    return nativeSpeed;
}

/*! 由工件角度获取本地角度 */
qreal RotationInfo::nativeDegrees(qreal partDegrees) const
{
    if (type == SurfaceContact) {
        if (partDiameter > 0 && axisDiameter > 0) {
            return partDiameter / axisDiameter * partDegrees;
        }
    }
    return partDegrees;
}

QJsonValue RotationInfo::toJson() const
{
    QJsonObject obj;
    obj["Axis"] = axis;
    obj["Type"] = EnumToKey(type);
    obj["Direction"] = EnumToKey(direction);
    obj["AxisDiameter"] = axisDiameter;
    obj["PartDiameter"] = partDiameter;
    obj["SpeedType"] = EnumToKey(speedType);
    obj["MaxAngularSpeed"] = maxAngularSpeed;
    return obj;
}

RotationInfo RotationInfo::fromJson(const QJsonValue &json)
{
    const QJsonObject obj = json.toObject();
    RotationInfo r;
    r.axis = obj["Axis"].toInt();
    r.type = KeyToEnum(obj["Type"].toString(), None);
    r.direction = KeyToEnum(obj["Direction"].toString(), Forward);
    r.axisDiameter = obj["AxisDiameter"].toDouble();
    r.partDiameter = obj["PartDiameter"].toDouble();
    r.speedType = KeyToEnum(obj["SpeedType"].toString(), Uniform);
    r.maxAngularSpeed = obj["MaxAngularSpeed"].toDouble();
    return r;
}

RotationInfo RotationInfo::byAxisIndex(const QList<RotationInfo> &rotations, int axis)
{
    foreach (const RotationInfo &info, rotations) {
        if (info.axis == axis) {
            return info;
        }
    }
    return RotationInfo();
}

bool RotationInfo::operator ==(const RotationInfo &other) const
{
    return (axis == other.axis)&& (type == other.type) && (
        (type == None) ||
        (type == AxisContact && qFuzzyCompare(axisDiameter, other.axisDiameter)) ||
        (type == SurfaceContact && qFuzzyCompare(axisDiameter, other.axisDiameter) && qFuzzyCompare(partDiameter, other.partDiameter))
    ) && (((speedType == Uniform) && (speedType == other.speedType)) ||
          ((speedType == LimitAngularSpeed) && qFuzzyCompare(maxAngularSpeed, other.maxAngularSpeed)));
}

bool RotationInfo::operator !=(const RotationInfo &other) const
{
    return !(*this == other);
}
