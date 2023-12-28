#include "MotionPath.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QtMath>
#include "TrioDriver.h"
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "motion.path");
REGISTER_JSON_CONVERTER(MotionPath);

void MotionPath::addElement(const MotionElement &element)
{
    elements.append(element);
}

void MotionPath::addPrerequisite(const MotionElement &elements)
{
    prerequisites.append(elements);
}

void MotionPath::addRotation(const RotationInfo &rotaion)
{
    rotations.append(rotaion);
}

int MotionPath::moveCount(int elementIndex) const
{
    if (uint(elementIndex) >= uint(elements.size())) {
        return 0;
    }
    const MotionElement &element = elements[elementIndex];
    if (canInterpolate(elementIndex)) {   // 可以插值.
        return element.moveCount(&elements[elementIndex + 1]);
    } else {
        return element.moveCount(nullptr);
    }
}

/*! 获取第index条element插值之后实际移动的element的数量 */
int MotionPath::moveCountTo(int elementIndex, bool &isReverse) const
{
    int count = 0;
    int elementCount = elementIndex < 0 ? elements.count() : elementIndex + 1;
    for (int index = 0; index < elementCount; ++index) {
        if (index != 0) {
            isReverse = !isReverse;                                             // 除了第一条方向由外界设定之外，其余的方向由上一条的方向反向得来
        }
        auto type = MotionElement::None;
        if (index < elements.count()) {
            type = elements[index].type;
        }
        if (type == MotionElement::None) {
            count += 1;
            continue;
        } else if (type == MotionElement::Stop) {
            continue;
        } else if (type == MotionElement::Home) {
            count += 1;
        } else if (type == MotionElement::Locate) {
            count += 1;
        } else if (type == MotionElement::MoveRel) {
            count += 1;
        } else if (type == MotionElement::Continuous) {
            continue;
        } else if (type == MotionElement::MoveLine) {
            MotionElement firstElement = elements.value(index);
            MotionElement secondElement = elements.value(index + 1);
            const int stepCount = firstElement.interpolateCount(secondElement);
            auto policy = firstElement.interPolicy;
            if (policy != MotionElement::InterpolateInvalid) {
                if (!firstElement.canInterpolate(secondElement)) {
                    policy = MotionElement::InterpolateNone;
                }
            }
            switch (policy) {
            case MotionElement::InterpolateNone:
                count += 1;
                break;
            case MotionElement::InterpolateUnidirectional:
                count += isReverse ? 2 * stepCount - 1 : 2 * stepCount;         // 单向插值，暂时是把整在起始点步进
                isReverse = true;
                break;
            case MotionElement::InterpolateBidiretional:
                count += stepCount;
                isReverse = stepCount % 2 != 0 ? isReverse : !isReverse;
                break;
            default:
                break;
            }
        } else if (type == MotionElement::Cam) {
            count += 1;
        } else if (type == MotionElement::MoveCircle) {
            count += 1;
        }
    }
    return count;
}

/*! 根据给定的index获取当前正在执行的element  isReverse:当前element是否是反向运行 */
MotionElement MotionPath::elementByMove(int index, bool &isReverse) const
{
    bool reverse = isReverse;
    if (index == 0) {
//        isReverse = isReverse;
        return elements.value(0);
    } else if (index >= moveCountTo(-1, reverse)) {
        return MotionElement();
    }
    int elementIndex = 0;
    for (; elementIndex < elements.count(); ++elementIndex) {
        reverse = isReverse;
        if (moveCountTo(elementIndex, reverse) > index) {
            break;
        }
    }
    bool preEndIsReversed = isReverse;
    int preCount = elementIndex == 0 ? 0 : moveCountTo(elementIndex - 1, preEndIsReversed);
    reverse = isReverse;
    int curCount = moveCountTo(elementIndex, reverse);
    if (elementIndex >= elements.count()) {
        return MotionElement();
    }
    MotionElement firstElement = elements.value(elementIndex);
    if (firstElement.type == MotionElement::None) {
        isReverse = preEndIsReversed;
        return MotionElement();
    } else if (firstElement.type != MotionElement::MoveLine) {                          // 只有直线平移才需要插值
        isReverse = !preEndIsReversed;
        return firstElement;
    }
    MotionElement secondElemnt = elements.value(elementIndex + 1);
    int stepCount = firstElement.interpolateCount(secondElemnt);
    MotionElement::InterpolatePolicy policy = firstElement.interPolicy;
    if (policy != MotionElement::InterpolateInvalid) {
        if (!firstElement.canInterpolate(secondElemnt)) {
            policy = MotionElement::InterpolateNone;
        }
    }
    switch (policy) {
    case MotionElement::InterpolateNone:
        isReverse = !preEndIsReversed;
        return firstElement;
    case MotionElement::InterpolateUnidirectional: {
        int count = index - preCount;
        if (2 * stepCount == (curCount - preCount)) {                           // 每条element都执行了双向扫描
            isReverse = count % 2 == 0 ? false : true;
            return firstElement + (firstElement.calcInterval(secondElemnt)) * qFloor(count / 2.0);
        } else {   // 首条element只执行了单向扫描
            isReverse = count % 2 == 0 ? true : false;
            return firstElement + firstElement.calcInterval(secondElemnt) * qCeil(count / 2.0);
        }
    }
    case MotionElement::InterpolateBidiretional: {
        int count = index - preCount;
        if (preCount == 0) {
            isReverse = count % 2 == 0 ? preEndIsReversed : !preEndIsReversed;
        } else {
            isReverse = count % 2 == 0 ? !preEndIsReversed : preEndIsReversed;
        }
        return firstElement + firstElement.calcInterval(secondElemnt) * count;
    }
    default:
        break;
    }
    return MotionElement();
}

int MotionPath::canInterpolate(int elementIndex) const
{
    if (uint(elementIndex + 1) >= uint(elements.size())) {  // 没有下一个element
        return false;
    }
    return elements[elementIndex].canInterpolate(elements[elementIndex + 1]);
}

MotionAxisList MotionPath::allAxes() const
{
    MotionAxisList list;
    list.combine(ptpAxes());
    list.combine(prerequisitesAxes());
    return list;
}

/*! 获取准备工作中的轴列表 */
MotionAxisList MotionPath::prerequisitesAxes() const
{
    MotionAxisList list;
    foreach (const MotionElement &element, prerequisites) {
        list.combine(element.axis);
    }
    return list;
}

/*! 获取步进过程中使用到的轴 */
MotionAxisList MotionPath::ptpAxes() const
{
    MotionAxisList list;
    foreach (const MotionElement &element, elements) {
        list.combine(element.axis);
    }
    return list;
}

/*! 计算实际扫描所需时间 */
qreal MotionPath::timeExpected(int index) const
{
    MotionPath newPath = expandMultiChannel();
    index = qMax(0, index);
    qreal total = 0;
    bool isReverse = false;
    for (int elementIndex = 0; elementIndex < newPath.elements.count(); ++elementIndex) {
        MotionElement firstElement = newPath.elements.value(elementIndex);
        isReverse = false;
        int start = elementIndex == 0 ? 0 : newPath.moveCountTo(elementIndex - 1, isReverse);
        isReverse = false;
        int end = newPath.moveCountTo(elementIndex, isReverse);
        if (end <= index) {
            continue;
        }

        int realStart = qMax(start, index);
        for (int i = realStart; i < end; ++i) {
            isReverse = false;
            MotionElement element = newPath.elementByMove(i, isReverse);
            bool isRotation = false;
            qreal cycle = newPath.conditionTime(firstElement, isRotation);
            if (isRotation) {                                                   // 是旋转扫描
                total += cycle;
            } else if (element.type == MotionElement::MoveCircle && element.speed > 0) {
                total += qAbs(element.pos.value(1).value(1) - element.pos.value(1).value(0)) / element.speed;
            } else if (element.type == MotionElement::MoveLine) {                   // 平动扫描
                total += element.length() / element.speed;                      // 扫描一条线时间
                total += cycle;                                                 // 假如存在延时，还需要加上延时时间
                if (index < i) {                                                // 需要加上上一条线的末尾位置到当前线的起始位置的步进时间
                    MotionPose currentPose = isReverse ? element.pos.value(0) : element.pos.value(element.pos.count() - 1);
                    isReverse = false;
                    MotionElement preElement = newPath.elementByMove(i - 1, isReverse);
                    MotionPose prePose = isReverse ? preElement.pos.value(0) : preElement.pos.value(preElement.pos.count() - 1);
                    int axisIndex = element.axis.toList().indexOf(element.stepAxis);
                    total += currentPose.distanceOnAxis(prePose, axisIndex) / element.stepSpeed;
                }
            }
        }
    }
    return total;
}

/*! 计算剩余时间 */
qreal MotionPath::timeRemains(int index, const MotionPose &pos) const
{
    MotionPath newPath = expandMultiChannel();
    if (index < 0) {
        return newPath.timeExpected(0);
    }
    qreal total = 0.00/* = newPath.timeExpected(index)*/;
    bool isReverse = false;
    MotionElement element = newPath.elementByMove(index, isReverse);
    RotationInfo mainRotation = RotationInfo::byAxisIndex(rotations, element.mainAxis);
    bool isRotation = false;
    qreal cycle = newPath.conditionTime(element, isRotation);                        // 获取conditions周期
    MotionPose destPos = isReverse ? element.pos.value(0) : element.pos.value(element.pos.count() - 1);
    int stepAxisIndex = element.axis.toList().indexOf(element.stepAxis);
    qreal offset = qAbs(destPos.value(stepAxisIndex) - pos.value(stepAxisIndex));
    if (offset < driverResolution || qAbs(offset - element.stepInterval) < driverResolution) { // 在同一条扫描线上
        if (qAbs(offset - element.stepInterval) < driverResolution) {
            index += 1;
            element = newPath.elementByMove(index, isReverse);
            destPos = isReverse ? element.pos.value(0) : element.pos.value(element.pos.count() - 1);
        }
        int mainAxisIndex = element.axis.toList().indexOf(element.mainAxis);
        qreal scanTime = 0;
        if (isRotation) {
            MotionElement prerequisity;
            foreach (const MotionElement &prerequisityElement, prerequisites) {
                if (prerequisityElement.mainAxis == element.mainAxis) {
                    prerequisity = prerequisityElement;
                    break;
                }
            }
            qreal scanLength = isReverse ? mainRotation.nativeDegrees(360) - pos.value(mainAxisIndex) : pos.value(mainAxisIndex);
            if (prerequisity.speed > 0) {
                scanTime = qMin(cycle, qAbs(scanLength / prerequisity.speed));
            }
        } else {
            qreal scanLength = isReverse ? element.pos.value(element.pos.count() - 1).value(mainAxisIndex) - pos.value(mainAxisIndex) :
                                           pos.value(mainAxisIndex) - element.pos.value(0).value(mainAxisIndex);
            if (element.speed > 0) {
                scanTime = qAbs(scanLength / element.speed);
            }
        }
        total -= scanTime;
    } else {   // 在步进过程中
        qreal stepLength = isReverse ? qAbs(element.pos.value(element.pos.count() - 1).value(stepAxisIndex) - pos.value(stepAxisIndex)) :
                                       qAbs(element.pos.value(0).value(stepAxisIndex) - pos.value(stepAxisIndex));
        if (element.stepSpeed > 0) {
            total += stepLength / element.stepSpeed;
        }
    }
    total += newPath.timeExpected(index);
    return qMax(total, 0.0);
}

qreal MotionPath::conditionTime(const MotionElement &element, bool &isRotation) const
{
    qreal total = 0.0;
    isRotation = false;
    RotationInfo mainRotation = RotationInfo::byAxisIndex(rotations, element.mainAxis);
    foreach (const MotionCondition &condition, element.conditions) {
        if (condition.type == MotionCondition::ConditionNone) {
            continue;
        }
        if (condition.type == MotionCondition::ConditionIoFalling || condition.type == MotionCondition::ConditionIoRising) {
            RotationInfo rotation = RotationInfo::byAxisIndex(rotations, condition.axis);
            if (rotation.type == RotationInfo::SurfaceContact || rotation.type == RotationInfo::AxisContact) {
                if (prerequisites.count() > 0) {
                    isRotation = true;
                }
                foreach (const MotionElement &e, prerequisites) {
                    if (e.type == MotionElement::Continuous && e.mainAxis == condition.axis) {       // 找到前置条件的扫描线
                        if (rotation.speedType == RotationInfo::Uniform) {       // 匀速
                            total = qMax(rotation.nativeDegrees(360) / e.speed * condition.repCount, total);
                        } else if (rotation.speedType == RotationInfo::LimitAngularSpeed) { // 最大角速度限制
                            qreal realDegrees = qMin(e.speed, mainRotation.maxAngularSpeed);
                            total = qMax(rotation.nativeDegrees(360) / realDegrees * condition.repCount, total);
                        }
                    }
                }
            }
        } else if (condition.type == MotionCondition::ConditionDelay) {    // 延时触发
            total = qMax(total, condition.arg * condition.repCount);
        }
    }
    return total;
}

qreal MotionPath::posePercent(int move, const MotionPose &pose) const
{
    Q_UNUSED(pose);
//    MotionElement prev = elements.value(move);
//    MotionElement next = elements.value(move + 1);
    return move / (elements.count() - 1);
}

/*! 利用多通道展开成一个新的motionpath */
MotionPath MotionPath::expandMultiChannel() const
{
    if (multiChannelEnable == false || multiChannels.count() <= 1) {
        return *this;
    }

    MotionPath newPath;
    newPath.interval = interval;
    newPath.multiChannelEnable = false;
    newPath.multiChannels = multiChannels;
    newPath.channelOffset = channelOffset;
    newPath.prerequisites = prerequisites;
    newPath.rotations = rotations;


    qCDebug(debug) << "Simple scan plan";
    for (int elementIndex = 0; elementIndex < elements.count(); ++elementIndex) {
        qCDebug(debug) << "Expanding element" << elementIndex;
        if (elementIndex == elements.count() - 1) {
            continue;
        }
        MotionElement currentElement = elements.value(elementIndex);
        if (currentElement.interPolicy == MotionElement::InterpolateInvalid) {
            continue;
        }
        MotionElement nextElement = elements.value(elementIndex + 1);
        if (currentElement.interPolicy == MotionElement::InterpolateNone || !currentElement.canInterpolate(nextElement)) {
            newPath.addElement(currentElement);
            continue;
        }

        const qreal mainStep = currentElement.mainStep(nextElement);
        const int stepDirection = mainStep / qAbs(mainStep);
        qCDebug(debug) << "Total step:" << mainStep;
        const qreal patchSpan = channelOffset * multiChannels.count() * stepDirection;              /*!< 一个扫描块能够覆盖的步进距离 */
        qCDebug(debug) << "Patch span:" << patchSpan;
        const int patchCount = qFloor(mainStep / patchSpan);                    /*!< 扫描需要的完整块数 */
        qCDebug(debug) << "Patch count:" << patchCount;
        const qreal patchLast = mainStep - patchCount * patchSpan;              /*!< 完整块之后, 最后一个扫描块的尺寸 */
        qCDebug(debug) << "Last patch:" << patchLast;

        if (patchCount == 0) {
            qreal realStep = (qAbs(patchLast) > channelOffset ? channelOffset : patchLast);
            qCDebug(debug) << "No full patch. Single step size" << realStep;

            newPath.addElement(currentElement);

            MotionElement endElement = currentElement;
            endElement.addOffset(realStep, currentElement.stepAxis);
            endElement.interPolicy = MotionElement::InterpolateNone;
            newPath.addElement(endElement);
        } else {
            qCDebug(debug) << "Processing " << patchCount << "full patches, and last patch length" << patchLast;
            for (int patchIndex = 0; patchIndex < patchCount; ++patchIndex) {
                MotionElement startElement = currentElement;
                startElement.addOffset(patchIndex * patchSpan, currentElement.stepAxis);
                newPath.addElement(startElement);

                MotionElement endElement = currentElement;
                endElement.addOffset(patchIndex * patchSpan + channelOffset * stepDirection, currentElement.stepAxis);
                endElement.interPolicy = MotionElement::InterpolateNone;
                newPath.addElement(endElement);
                qCDebug(debug) << "Adding patch #" << patchIndex << "with offset" << patchIndex * patchSpan << "~" << patchIndex * patchSpan + channelOffset;
            }
            if (qFuzzyIsNull(patchLast)) {
                /* 不存在不完整的块, 跳过 */
            } else if (qAbs(patchLast) <= channelOffset) {                            /* 剩余的部分不足1块, 将最后一个完整路径延长 */
                newPath.elements.last().addOffset(patchLast, currentElement.stepAxis);
                qCDebug(debug) << "Adding final patch: extending last patch by" << patchLast;
            } else if (qAbs(patchLast) > channelOffset) {                             /* 剩余的部分超过一块, 但不足n块, 令最后一个探头指向结尾 */
                const qreal start = (patchCount - 1) * patchSpan + patchLast;
                MotionElement startElement = currentElement;
                startElement.addOffset(start, currentElement.stepAxis);
                newPath.addElement(startElement);

                MotionElement endElement = currentElement;
                endElement.addOffset(start + channelOffset * stepDirection, currentElement.stepAxis);
                endElement.interPolicy = MotionElement::InterpolateNone;
                newPath.addElement(endElement);
                qCDebug(debug) << "Adding final patch with offset" << start << "~" << start + channelOffset;
            }
        }
    }

    return newPath;
}

MotionPath MotionPath::expandAll() const
{
    return *this;
}

QJsonValue MotionPath::toJson() const
{
    QJsonObject obj;
    QJsonArray elementsArray;
    foreach (const MotionElement &element, elements) {
        elementsArray << element.toJson();
    }
    obj["Elements"] = elementsArray;

    QJsonArray prerequisitesArray;
    foreach (const MotionElement &element, prerequisites) {
        prerequisitesArray << element.toJson();
    }
    obj["Prerequisites"] = prerequisitesArray;

    obj["Rotations"] = convertToJson(rotations);
    obj["ScanInterval"] = interval;
    obj["MultiChannelEnable"] = multiChannelEnable;
    QJsonArray channels;
    for (const ChannelList &channelList: multiChannels) {
        channels.append(convertToJson(channelList));
    }
    obj["MultiChannels"] = channels;
    obj["ChannelOffset"] = channelOffset;
    return obj;
}

MotionPath MotionPath::fromJson(const QJsonValue &json)
{
    const QJsonObject obj = json.toObject();
    MotionPath path;

    foreach (QJsonValue value, obj["Elements"].toArray()) {
        path.elements << MotionElement::fromJson(value);
    }
    foreach (QJsonValue value, obj["Prerequisites"].toArray()) {
        path.prerequisites << MotionElement::fromJson(value);
    }
    foreach (QJsonValue value, obj["Rotations"].toArray()) {
        path.rotations << RotationInfo::fromJson(value);
    }
    path.interval = obj["ScanInterval"].toDouble();
    path.channelOffset = obj["ChannelOffset"].toDouble();
    path.multiChannelEnable = obj["MultiChannelEnable"].toBool();
    foreach (const QJsonValue &value, obj["MultiChannels"].toArray()) {
        path.multiChannels.append(convertFromJson<QList<int> >(value));
    }
    return path;
}

bool MotionPath::isInbounds(qreal start, qreal value, qreal end)
{
    const qreal min = qMin(start, end);
    const qreal max = qMax(start, end);
    const qreal afterBound = qBound(min, value, max);
    return qFuzzyCompare(afterBound, value);
}

MotionPath::CheckResult MotionPath::GeneratePath(bool dryRun) const
{
    CheckResult r;
    if (allAxes().axisCount() == 0) {
        r.error << tr("未定义任何运动轴");
    }
//    if (interPolicy == InterpolateNone) {
//        r.error << tr("无效的插值方式: %1").arg(EnumToKey(interPolicy));
//    }
    if (elements.count() < 2) {
        r.error << tr("扫描路径欠定义: 扫描线数量过少");
    }
//    if (stepInterval <= 0) {
//        r.error << tr("步进距离不正确");
//    }
    for (int index = 0; index < elements.count(); ++index) {
        const MotionElement &element = elements[index];
        if (element.type == MotionElement::None) {
            r.error << tr("扫描路径欠定义: 第 %1 个路径定义无效").arg(index + 1);
        }
    }
//    if (rotationType == RotationNone) {
        foreach (const MotionElement &element, elements) {
            if (element.type != MotionElement::MoveLine) {
                r.error << tr("扫描路径过定义: 平面扫描含有非法运动");
            }
            if (element.type == MotionElement::Continuous) {
                r.error << tr("扫描路径过定义: 非旋转扫描含有无限旋转运动");
            }
            if (element.pos.count() < 2) {
                r.error << tr("扫描路径欠定义: 缺少扫描线定义");
            }
        }
//    }

    bool isReversed = false;
    auto it = elements.cbegin();                                                /*!< 指向起始扫描线的迭代器 */
    auto itNext = it + 1;                                                       /*!< 指向终止扫描线的迭代器 */
    while (itNext != elements.cend()) {
        const MotionElement &p1 = *it;
        const MotionElement &p2 = *itNext;
        int lineCount = dryRun ? 0 : p1.interpolateCount(p2);

        for (int lineIndex = 0; lineIndex < lineCount; ++lineIndex) {
            if (p1.interPolicy == MotionElement::InterpolateBidiretional) {
                isReversed = !isReversed;
            }
            r.path << p1.interpolate(p2, lineIndex).reverse(isReversed);
        }
        it++;
        itNext++;
    }

    if (!r.error.isEmpty()) {
        return r;
    }
    if (dryRun) {
        return r;
    }
    return r;
}
