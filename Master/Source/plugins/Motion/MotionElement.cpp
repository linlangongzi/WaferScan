#include "MotionElement.h"
#include <allrising/Util.h>
#include <QJsonObject>
#include <QtMath>

REGISTER_JSON_CONVERTER(MotionElement);

MotionElement::MotionElement(const MotionElement &other)
{
    type = other.type;
    interPolicy = other.interPolicy;
    pos = other.pos;
    axis = other.axis;
    mainAxis = other.mainAxis;
    stepAxis = other.stepAxis;
    stepInterval = other.stepInterval;
    speed = other.speed;
    conditions = other.conditions;
    stepSpeed = other.stepSpeed;
}

MotionElement::MotionElement(const MotionAxisList &axisList, const MotionPose &pose) :
    axis(axisList)
{
    pos << pose;
}

MotionElement::MotionElement(const MotionAxisList &axisList, const MotionPose &from, const MotionPose &to) :
    axis(axisList)
{
    pos << from << to;
}

QString MotionElement::description() const
{
    return tr("扫描线");
}

void MotionElement::setAxisList(const MotionAxisList &list)
{
    axis = list;
}

qreal MotionElement::length(int axisIndex) const
{
    if (axisIndex < 0) {
        axisIndex = axis.indexOf(mainAxis);
    }
    qreal length = 0;
    for (int index = 1; index < pos.count(); ++index) {                         /* 跳过首个元素 */
        const MotionPose &currentPos = pos[index];
        const MotionPose &prePose = pos[index - 1];
        length += qAbs(currentPos.value(axisIndex) - prePose.value(axisIndex));
    }
    return length;
}

/*! 获取当前element与下一个element之间，在主步进方向上的总位移量 */
qreal MotionElement::mainStep(const MotionElement &next) const
{
    if (pos.isEmpty() || next.pos.isEmpty()) {
        return 0;
    }
    qreal start = pos.first().value(stepAxis);
    qreal end = next.pos.first().value(stepAxis);
    return end - start;
}

int MotionElement::stepCount(const MotionElement *next) const
{
    if (pos.isEmpty() || !next || pos.size() != next->pos.size() || qFuzzyIsNull(stepInterval)) {
        return 1;
    }
    if (interPolicy == InterpolateNone || interPolicy == InterpolateInvalid) {
        return 1;
    }
    qreal step = qAbs(mainStep(*next) / stepInterval);
    return qMax(1, qFloor(step));
}

QList<MotionPose> MotionElement::movePose(int moveIndex, MotionElement *next) const
{
    int step = stepCount(next);
    if (moveIndex <= 0 || step <= 1) {
        return pos;
    }
    if (next && moveIndex >= step) {
        return next->pos;
    }

    QList<MotionPose> pose;
    Q_ASSERT(next);
    Q_ASSERT(pos.count() == next->pos.count());
    qreal scale = stepInterval / mainStep(*next);
    for (int index = 0; index < pos.count(); ++index) {
        pose << pos[index] + (next->pos[index] - pos[index]) * scale;
    }
    return pose;
}

int MotionElement::interpolateCount(const MotionElement &next) const
{
    if (interPolicy == InterpolateNone) {
        return 1;
    }
    qreal step = mainStep(next);
    if (qFuzzyIsNull(step)) {
        return 1;
    }
    return qCeil(qAbs(step) / stepInterval);
}

MotionElement &MotionElement::reverse(bool reverse)
{
    if (reverse) {
        int max = pos.count() / 2;
        for (int index = 0; index <= max; ++index) {                            /* 将第0个Pose与最后一个交换，第1个与倒数第二个交换，以此类推 */
            pos.swap(index, pos.count() - index);
        }
    }
    return *this;
}

bool MotionElement::canInterpolate(const MotionElement &next, int index) const
{
    if (axis != next.axis || pos.isEmpty() || pos.count() != next.pos.count()) {
        return false;
    }
    qreal mainMove = qAbs(mainStep(next));
    qreal stepCount = mainMove / stepInterval;
    return (uint(index) < uint(stepCount) && !qFuzzyIsNull(mainMove));
}

MotionElement MotionElement::calcInterval(const MotionElement &next) const
{
    if (!canInterpolate(next)) {
        return MotionElement();
    }
    qreal mainMove = mainStep(next);
    qreal stepCount = qAbs(mainMove / stepInterval);
    QList<MotionPose> poseList;
    for (int index = 0; index < pos.count(); ++index) {
        poseList << (next.pos.value(index) - pos[index]) / stepCount;
    }
    MotionElement element = *this;
    element.pos = poseList;
    return element;
}

MotionElement MotionElement::interpolate(const MotionElement &next, int index) const
{
    Q_ASSERT(canInterpolate(next, index));
    MotionElement interval = calcInterval(next);
    interval *= index;
    interval += *this;
    return interval;
}

void MotionElement::addOffset(qreal offset, int axis)
{
    for (MotionPose &pose: pos) {
        pose.addOffset(offset, axis);
    }
}

MotionElement MotionElement::modifyRadius(qreal radius)
{
    if (pos.count() < 2 || pos[0].count() < 3 || pos[1].count() < 2) {
        return *this;
    }
    pos[0][2] = radius;
    qSwap(pos[1][0], pos[1][1]);                                                // 起始角度、目标角度 互换
    return *this;
}

MotionElement MotionElement::modifyDepth(qreal depth)
{
    if (pos.count() < 2 || pos[0].count() < 4 || pos[1].count() < 2) {
        return *this;
    }
    pos[0][3] = depth;
    qSwap(pos[1][0], pos[1][1]);
    return *this;
}

int MotionElement::moveCount(const MotionElement *next) const
{
    switch (type) {
    case None:          return 0;
    case Stop:          return 0;
    case Home:          return 0;
    case Locate:        return 1;
    case MoveLine:      return stepCount(next);
    case MoveRel:       return 1;
    case Continuous:    return 1;
    case Cam:           return 1;
    default:            return 0;
    }
}

QJsonValue MotionElement::toJson() const
{
    QJsonObject obj;
    obj["Type"] = EnumToKey(type);
    obj["Axis"] = axis.toJson();
    obj["Conditions"] = convertToJson(conditions);
    obj["Speed"] = speed;
    obj["ScanAxis"] = mainAxis;
    obj["StepAxis"] = stepAxis;
    obj["InterpolatePolicy"] = EnumToKey(interPolicy);
    obj["StepInterval"] = stepInterval;
    obj["StepSpeed"] = stepSpeed;
    obj["Pos"] = convertToJson(pos);

    return obj;
}

MotionElement MotionElement::fromJson(const QJsonValue &json)
{
    const QJsonObject obj = json.toObject();
    MotionElement element;

    element.type = KeyToEnum(obj["Type"].toString(), None);
    element.axis = MotionAxisList::fromJson(obj["Axis"]);
    element.conditions = convertFromJson<QList<MotionCondition>>(obj["Conditions"]);
    element.interPolicy = KeyToEnum(obj["InterpolatePolicy"].toString(), InterpolateNone);
    element.mainAxis = obj["ScanAxis"].toInt();
    element.stepAxis = obj["StepAxis"].toInt();
    element.stepInterval = obj["StepInterval"].toDouble();
    element.stepSpeed = obj["StepSpeed"].toDouble();
    element.pos = convertFromJson<QList<MotionPose>>(obj["Pos"]);
    element.speed = obj["Speed"].toDouble();
    return element;
}

bool MotionElement::operator ==(const MotionElement &other) const
{
    return (type == other.type)
            && (pos == other.pos)
            && (axis == other.axis)
            && (mainAxis == other.mainAxis)
            && (stepAxis == other.stepAxis)
            && qFuzzyCompare(stepInterval, other.stepInterval)
            && qFuzzyCompare(speed, other.speed)
            && qFuzzyCompare(stepSpeed, other.speed)
            && (interPolicy == other.interPolicy)
            && (conditions == other.conditions);
}

MotionElement &MotionElement::operator *=(qreal multiplier)
{
    for (auto &pose : pos) {
        pose *= multiplier;
    }
    return *this;
}

MotionElement MotionElement::operator *(qreal multiplier)
{
    MotionElement element(*this);
    element *= multiplier;
    return element;
}

MotionElement &MotionElement::operator +=(const MotionElement &other)
{
    for (int index = 0; index < pos.count(); ++index) {
        pos[index] += other.pos.value(index);
    }
    return *this;
}

MotionElement &MotionElement::operator +=(const MotionPose &other)
{
    for (auto &pose: pos) {
        pose += other;
    }
    return *this;
}

MotionElement MotionElement::operator +(const MotionElement &other) const
{
    MotionElement element(*this);
    element += other;
    return element;
}

MotionElement &MotionElement::operator -=(const MotionElement &other)
{
    for (int index = 0; index < pos.count(); ++index) {
        pos[index] -= other.pos.value(index);
    }
    return *this;
}

MotionElement MotionElement::operator -(const MotionElement &other) const
{
    MotionElement element(*this);
    element -= other;
    return element;
}
