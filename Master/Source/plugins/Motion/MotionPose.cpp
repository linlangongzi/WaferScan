#include "MotionPose.h"
#include <QtMath>
#include <QVector3D>
#include <allrising/MetaType/MetaType.h>

REGISTER_JSON_CONVERTER(MotionPose)

MotionPose::MotionPose(int size) :
    d(size)
{
}

MotionPose::MotionPose(const QVector<float> &vector) :
    d(vector)
{
}

MotionPose MotionPose::fromJson(const QJsonValue &json)
{
    return convertFromJson<Base>(json);
}

QJsonValue MotionPose::toJson() const
{
    return convertToJson(d);
}

QString MotionPose::toString() const
{
    QStringList text;
    for (auto value: *this) {
        text << QString::number(value);
    }
    return tr("(%1)").arg(text.join(","));
}

MotionPose MotionPose::fromString(const QString &text)
{
    MotionPose pose;
    if (text.startsWith("(") && text.endsWith(")")) {
        QStringRef posText = text.midRef(1, text.length() - 1);
        for (const QStringRef &value: posText.split(",")) {
            pose << value.toDouble();
        }
    }
    return pose;
}

/*! 使用轴列表过滤pose中不需要的位置 */
QList<qreal> MotionPose::filterPose(const MotionAxisList &axes) const
{
    QList<qreal> poseList;
    for (int index = 0; index < d.size(); ++index) {
        if (axes.hasAxis(index)) {
            poseList << d.at(index);
        }
    }
    return poseList;
}

/*! 在轴向的距离 */
qreal MotionPose::distanceOnAxis(const MotionPose &other, int axis)
{
    return qAbs(value(axis) - other.value(axis));
}

void MotionPose::addOffset(qreal offset, int axis)
{
    if (uint(axis) >= uint(d.count())) {
        return;
    }
    d[axis] += offset;
}

void MotionPose::ensureSize(int size)
{
    while (d.size() < size) {
        d.append(0);
    }
}

bool MotionPose::isEqualTo(const MotionAxisList &list, const MotionPose &other, double limit) const
{
    for (int index = 0; index < list.count(); ++index) {
        if (list.hasAxis(index)) {
            if (qAbs(other.value(index) - value(index)) > limit) {
                return false;
            }
        }
    }
    return true;
}

MotionPose &MotionPose::operator +=(const MotionPose &other)
{
    while (d.size() < other.d.size()) {
        d.append(0);
    }
    for (int index = 0; index < other.d.size(); ++index) {                        /* other.size <= size */
        d[index] += other.d[index];
    }
    return *this;
}

MotionPose &MotionPose::operator +=(qreal c)
{
    for (auto &v: *this) {
        v += c;
    }
    return *this;
}

MotionPose &MotionPose::operator -=(const MotionPose &other)
{
    while (d.size() < other.d.size()) {
        d.append(0);
    }
    for (int index = 0; index < other.d.size(); ++index) {
        d[index] -= other[index];
    }
    return *this;
}

MotionPose &MotionPose::operator -=(qreal c)
{
    for (auto &v: *this) {
        v -= c;
    }
    return *this;
}

MotionPose &MotionPose::operator *=(qreal c)
{
    for (auto &v: *this) {
        v *= c;
    }
    return *this;
}

MotionPose &MotionPose::operator /=(qreal c)
{
    for (auto &v: *this) {
        v /= c;
    }
    return *this;
}

bool MotionPose::operator ==(const MotionPose &other) const
{
    int maxCount = qMax(d.count(), other.d.count());
    for (int index = 0; index < maxCount; ++index) {
        if (!qFuzzyCompare(value(index), other.value(index))) {
            return false;
        }
    }
    return true;
}

MotionPose MotionPose::operator -() const
{
    MotionPose copy;
    copy.d.reserve(d.size());
    for (auto v: *this) {
        copy << -v;
    }
    return copy;
}
