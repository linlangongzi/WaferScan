#include "TrioQueue.h"
#include "TrioAllocator.h"
#include "TrioDriver.h"
#include "TrioStream.h"
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <Motion/TrioDriver.h>
#include <QLoggingCategory>
#include <QRegularExpression>
#include <QStringList>
#include <QTimer>
#include <QtMath>
#include <allrising/Util.h>

using namespace Motion;
static Q_LOGGING_CATEGORY(debug, "trio.queue");

using s = QString;

static const QString programName("QUEUE");              /*!< MC中运行的程序名 */
static const QString lineStart("LS");                   /*!< 状态报告: 扫描线开始 */
static const QString lineEnd("LE");                     /*!< 状态报告: 扫描线结束 */
int TrioQueue::handle = 0;
static const int tableInstructionLength = 4;
static const int tableSize = 128000;

enum VrOffset {
    VrStartIndex,
    VrIsReverse,
    VrSize,
};

TrioQueue::TrioQueue(TrioDriver *parent) :
    QObject(parent),
    p(parent)
{
    p->addGlobalStatus(s("PROC_STATUS PROC(%1)").arg(QUEUE_PROCESS_NUMBER));
    vrAddr = p->m_vrAlloc->Alloc(VrSize);
    camBase = p->m_tableAlloc->Alloc(tableSize);
    UpdateState(STATE_UNAVAILABLE, -1, -1);
    Reset();
    connect(p, &TrioDriver::tokenReceived, this, &TrioQueue::OnTokenReceived);
    connect(p->m_stream, &TrioStream::commandSent, this, &TrioQueue::OnCommandSent);
    connect(parent, &TrioDriver::disconnected, this, &TrioQueue::onDisconnected);
    qCInfo(debug, "Using VR addr %d, CAM table addr %d", vrAddr, camBase);
}

/*! 发送queue >=0:  当前句柄
 *           -1：  当前非IDLE状态
 *           -2：  程序结构错误
*/
int TrioQueue::Send()
{
    qCInfo(debug) << "Starting queue";
    if (GetState() != STATE_IDLE) {
        qCInfo(debug) << "Start failed: wrong state" << GetState();
        return -1;
    }
    if (!callStack.isEmpty()) {
        qCInfo(debug) << "Start failed: bad program structure";
        return -2;
    }

    baseCommandId = p->m_stream->execute("Stop QUEUE", ChannelProject, true);
    finalCommandId = p->setProgram(programName, queue);
    UpdateState(STATE_SENDING, -1, -1);
    return ++handle;
}

/*!
 *  启动queue,需要验证当前句柄 0:启动校验通过，发送启动指令成功
 *                         -1：句柄错误
 *                         -2：当前状态非IDLE
 */
int TrioQueue::Start(int queueHandle, int index, bool isReverse)
{
    if (queueHandle != handle) {
        return -1;
    }
    if (GetState() != STATE_IDLE && GetState() != STATE_STARTING) {
        qCInfo(debug) << "Start failed: wrong state" << GetState();
        return -2;
    }
    // 启动之前，需要设置vr中的起始序号
    p->setVr(vrAddr + VrStartIndex, index);
    p->setVr(vrAddr + VrIsReverse, isReverse ? 1 : 0);
    // 启动程序
    p->runProgram(programName, QUEUE_PROCESS_NUMBER);
    QTimer::singleShot(1000, this, &TrioQueue::OnStartTimeout);
    return 0;
}

int TrioQueue::Stop()
{
    p->m_stream->cancel(baseCommandId, finalCommandId);
    baseCommandId = 0;
    finalCommandId = 0;
    p->stopProgram(programName, QUEUE_PROCESS_NUMBER);
    UpdateState(STATE_IDLE, -1, total);
    return SUCCESS;
}

int TrioQueue::Continue()
{
    if (GetState() != STATE_PAUSED) {
        qCInfo(debug) << "Continue failed: wrong state" << GetState();
        return ERR_WRONG_STATE;
    }
    UpdateState(STATE_RUNNING, -1, total);
    p->execute(s("TrOff %1, %2").arg(programName).arg(QUEUE_PROCESS_NUMBER));
    return SUCCESS;
}

int TrioQueue::Reset()
{
    if (GetState() != STATE_IDLE) {
        return ERR_WRONG_STATE;
    }
    queue.clear();
    base.clear();
    callStack.clear();
    isVirtualConfigured = false;
    progress = -1;
    total = -1;
    baseCommandId = 0;
    finalCommandId = 0;
    UpdateState(STATE_IDLE, -1, -1);
    return SUCCESS;
}

TrioQueue::State TrioQueue::GetState() const
{
    if (p->state() == QTcpSocket::ConnectedState) {
        return state;
    }
    return STATE_UNAVAILABLE;
}

/*! 根据motionpath更新程序 */
void TrioQueue::UpdateProgram(const MotionPath &path)
{
    AddBreakPoint();
    MotionPath newPath = path.expandMultiChannel();
    bool isReverse = false;
    for (int index = 0; index < newPath.prerequisites.count(); ++index) {
        MotionElement firstElement = newPath.prerequisites.value(index);
        MotionElement secondElement = newPath.prerequisites.value(index + 1);
        isReverse = false;
        int startIndex = index == 0 ? 0 : newPath.moveCountTo(index - 1, isReverse);
        MoveElement(firstElement, secondElement, startIndex, index == 0 ? isReverse : !isReverse, newPath.rotations);
    }

    // 扫描路径下发
    isReverse = false;
    total = newPath.moveCountTo(-1, isReverse);
    AddParameter("isReverse", s("VR(%1) > 0").arg(vrAddr + VrIsReverse));
    for (int index = 0; index < newPath.elements.count(); ++index) {
        MotionElement firstElement = newPath.elements.value(index);
        MotionElement secondElement = newPath.elements.value(index + 1);
        isReverse = false;
        int startIndex = index == 0 ? 0 : newPath.moveCountTo(index - 1, isReverse);
        MoveElement(firstElement, secondElement, startIndex, index == 0 ? isReverse : !isReverse, newPath.rotations, newPath.prerequisites);
    }
    WaitConditions(newPath.elements.value(newPath.elements.count() - 1).conditions);
}

/*! 由一个element移到另一个element */
void TrioQueue::MoveElement(const MotionElement &prev, const MotionElement &next, int startIndex, bool isReverse, const QList<RotationInfo> &rotations, const QList<MotionElement> &prerequisities)
{
    if (prev.type == MotionElement::None) {
//        addReportPoint(startIndex);
        return;
    }
    if (prev.type == MotionElement::MoveLine) {
        MotionElement::InterpolatePolicy policy = prev.interPolicy;
        if (policy != MotionElement::InterpolateInvalid) {
            if (!prev.canInterpolate(next)) {
                policy = MotionElement::InterpolateNone;
            }
        } else {
            return;
        }
        if (policy == MotionElement::InterpolateNone) {
            If(s("VR(%1) <= %2").arg(vrAddr + VrStartIndex).arg(startIndex));
                WaitConditions(prev.conditions);
                ResetPrerequisityAxes(prerequisities);
                reportMoveStart(startIndex);
                If("isReverse");
                    MovePoseList(prev, QList<MotionPose>(), "0", true, prerequisities, rotations);
                ElseIf();
                    MovePoseList(prev, QList<MotionPose>(), "0", false, prerequisities, rotations);
                Endif();
                reportMoveEnd(startIndex);
                reverseBoolean("isReverse");
            Endif();
        } else {
            int stepCount = prev.interpolateCount(next);
            MotionElement interval = prev.calcInterval(next);
            if (policy == MotionElement::InterpolateUnidirectional) {
                isReverse ? AddParameter("SingleStep", "1") : AddParameter("SingleStep", "0");
                int lastIndex = isReverse ? startIndex + stepCount * 2 - 1 : startIndex + stepCount * 2;
                If(s("VR(%1) <= %2").arg(vrAddr + VrStartIndex).arg(startIndex));
                    AddParameter("index", "0");
                ElseIf(s("VR(%1) <= %2").arg(vrAddr + VrStartIndex).arg(lastIndex));
                    isReverse ? AddParameter("index", s("INT((VR(%1) - %2) / 2.0 + 0.5)").arg(vrAddr + VrStartIndex).arg(startIndex)) :
                                AddParameter("index", s("INT((VR(%1) - %2) / 2.0)").arg(vrAddr + VrStartIndex).arg(startIndex));
                ElseIf();
                    AddParameter("index", QString::number(stepCount));
                Endif();
            } else {
                int lastIndex = startIndex + stepCount;
                If(s("VR(%1) <= %2").arg(vrAddr + VrStartIndex).arg(startIndex));
                    AddParameter("index", "0");
                ElseIf(s("VR(%1) <= %2").arg(vrAddr + VrStartIndex).arg(lastIndex));
                    AddParameter("index", s("VR(%1) - %2").arg(vrAddr + VrStartIndex).arg(startIndex));
                ElseIf();
                    AddParameter("index", QString::number(stepCount));
                Endif();
            }
            While(s("index <= %1").arg(stepCount - 1));
                WaitConditions(prev.conditions);
                ResetPrerequisityAxes(prerequisities);
                if (policy == MotionElement::InterpolateUnidirectional) {
                    If("isReverse");
                        reportMoveStart(s("%1 + index * 2").arg(startIndex));
                        MovePoseList(prev, interval.pos, "index", true, prerequisities, rotations);
                        reportMoveEnd(s("%1 + index * 2").arg(startIndex));
                    ElseIf();
                        reportMoveStart(s("%1 + index * 2 - SingleStep").arg(startIndex));
                        MovePoseList(prev, interval.pos, "index", false, prerequisities, rotations);
                        reportMoveEnd(s("%1 + index * 2 - SingleStep").arg(startIndex));
                        reverseBoolean("isReverse");
                        reportMoveStart(s("%1 + index * 2 - SingleStep + 1").arg(startIndex));
                        MovePoseList(prev, interval.pos, "index", true, prerequisities, rotations);
                        reportMoveEnd(s("%1 + index * 2 - SingleStep + 1").arg(startIndex));
                    Endif();
                } else {
                    const QString index = s("%1 + index").arg(startIndex);
                    reportMoveStart(index);
                    If("isReverse");
                        MovePoseList(prev, interval.pos, "index", true, prerequisities, rotations);
                    ElseIf();
                        MovePoseList(prev, interval.pos, "index", false, prerequisities, rotations);
                    Endif();
                    reportMoveEnd(index);
                }
                reverseBoolean("isReverse");
                AddParameter("index", "index + 1");
            Wend();
        }
    } else if (prev.type == MotionElement::Stop) {
        foreach (int axis, prev.axisList().toList()) {
            Cancel(axis);
        }
        WaitIdle();
    } else if (prev.type == MotionElement::Home) {

    } else if (prev.type == MotionElement::Locate) {
        if (prev.pos.count() > 0 && !prev.axis.isEmpty()) {
            MotionPose dest = prev.pos.first();
            int axisZ = 2;
            qreal safeHeight = 0;
            If(s("DPOS AXIS(%1) > %2").arg(axisZ).arg(safeHeight));
                MoveAbs(safeHeight, axisZ);
                WaitIdle(axisZ);
                CheckAxisPosition(safeHeight, axisZ, ">");

                MotionAxisList axis = prev.axis;
                axis.setAxis(axisZ, false);
                MoveAbs(dest, axis);
            Endif();
            MoveAbs(dest, prev.axis);
            WaitIdle(prev.axis.first());
//            AddBreakPoint();
        }
    } else if (prev.type == MotionElement::MoveRel) {

    } else if (prev.type == MotionElement::MoveCircle) {
        if (prev.axis.count() < 3 || prev.pos.count() < 2) {
            return;
        }

        MotionPose annulus = prev.pos[0];
        MotionPose angle = prev.pos[1];
        if (annulus.count() < 4 || angle.count() < 3) {
            return;
        }

        const QList<int> axes = prev.axis.toList();
        int axisX = axes[0];
        int axisY = axes[1];
        int axisZ = axes[2];
        int axisC = prev.mainAxis;

        QPointF center(annulus[0], annulus[1]);
        qreal radius = annulus[2];
        qreal depth = annulus[3];

        qreal angleStart = angle[0];
        qreal angleEnd = angle[1];
        qreal angleOffset = angle[2];

        qreal radian = qDegreesToRadians(angleStart + angleOffset);
        Motion pos;
        pos[axisX] = center.x() + radius * qCos(radian);
        pos[axisY] = center.y() + radius * qSin(radian);
        pos[axisZ] = depth;

        queue << s("' Move to start");
        MoveAbs(pos);

        SetSpeed(axisC, prev.speed);
        MoveCircle(axisX, axisY, axisC, center, radius, angleStart, angleOffset, angleEnd);
//        AddReportPoint(startIndex);
    } else if (prev.type == MotionElement::Continuous) {
        for (int axis: prev.axisList().toList()) {
            RotationInfo rotation = RotationInfo::byAxisIndex(rotations, axis);
            SetSpeed(axis, prev.speed);
            ResetAxisPos(0, axis);
            MoveContinuous(axis, rotation.direction == RotationInfo::Reverse);
        }
    } else if (prev.type == MotionElement::Cam) {
        MoveCam(prev, prev.speed, prev.speed);
    }
}

/*! 移动整条element */
void TrioQueue::MovePoseList(const MotionElement &element, const QList<MotionPose> &intervals, const QString &i, bool isReversed,
                         const QList<MotionElement> &prerequisities, const QList<RotationInfo>
                          &rotations)
{
    if (isReversed) {
        for (int index = element.pos.count() - 1; index >= 0; --index) {
            qreal realSpeed = index == element.pos.count() - 1 ? element.stepSpeed : element.speed;
            int mainAxis = index == element.pos.count() - 1 ? element.stepAxis : element.mainAxis;
//            bool needCancelLink = index == element.pos.count() - 1 || index == 0;
            MovePose(element.axis, realSpeed, mainAxis, element.pos.value(index), intervals.value(index), i, prerequisities, rotations/*, needCancelLink*/);
        }
    } else {
        for (int index = 0; index < element.pos.count(); ++index) {
            qreal realSpeed = index == 0 ? element.stepSpeed : element.speed;
            int mainAxis = index == 0 ? element.stepAxis : element.mainAxis;
//            bool needCancelLink = index == element.pos.count() - 1 || index == 0;
            MovePose(element.axis, realSpeed, mainAxis, element.pos.value(index), intervals.value(index), i, prerequisities, rotations/*, needCancelLink*/);
        }
    }
}

/*! 完成一个MotionPose的步进，a+b*i */
void TrioQueue::MovePose(const MotionAxisList &axes, qreal speed, int mainAxis, const MotionPose &a, const MotionPose &b, const QString &i, const QList<MotionElement> &prerequisities, const QList<RotationInfo> &rotations/*, bool cancelLink*/)
{
    foreach (const MotionElement &element, prerequisities) {
        if (element.type != MotionElement::Continuous) {
            continue;
        }
        foreach (int axisIndex, element.axis.toList()) {
            Axis *axis = MotionManager::axis(axisIndex);
            if (!axis) {
                continue;
            }
            RotationInfo info = RotationInfo::byAxisIndex(rotations, axisIndex);
            foreach (const RotationInfo &rotation, rotations) {
                if (rotation.axis == axisIndex) {
                    info = rotation;
                    break;
                }
            }
            if (info.speedType == RotationInfo::LimitAngularSpeed) {
                QVector3D vector = axis->baseDirection();
                QVector3D offset = axis->baseOffset();
                if (vector.isNull() || offset.isNull()) {
                    continue;
                }
                AddParameter("x", s("%1 + %2 * %3").arg(a.value(0), 0, 'f', MotionManager::displayPrecision()).arg(b.value(0), 0, 'f', MotionManager::displayPrecision()).arg(i));
                AddParameter("y", s("%1 + %2 * %3").arg(a.value(1), 0, 'f', MotionManager::displayPrecision()).arg(b.value(1), 0, 'f', MotionManager::displayPrecision()).arg(i));
                AddParameter("z", s("%1 + %2 * %3").arg(a.value(2), 0, 'f', MotionManager::displayPrecision()).arg(b.value(2), 0, 'f', MotionManager::displayPrecision()).arg(i));

                AddParameter("dotX", s("((x - %1) * %2) * %2").arg(offset.x()).arg(vector.x()));
                AddParameter("dotY", s("((y - %1) * %2) * %2").arg(offset.y()).arg(vector.y()));
                AddParameter("dotZ", s("((z - %1) * %2) * %2").arg(offset.z()).arg(vector.z()));

                AddParameter("pX", s("x - dotX - %1").arg(offset.x()));
                AddParameter("pY", s("y - dotY - %1").arg(offset.y()));
                AddParameter("pZ", s("z - dotZ - %1").arg(offset.z()));

                AddParameter("realRadius", s("SQR(pX * pX + pY * pY + pZ * pZ)"));

                If(s("realRadius > 0"));
                    AddParameter("w", s("%1 / realRadius * 180 / PI").arg(element.speed));
                    If(s("w <= %1").arg(info.maxAngularSpeed));
                        SetSpeed(axisIndex, "w");
                    ElseIf();
                        SetSpeed(axisIndex, info.maxAngularSpeed);
                    Endif();
                Endif();
            }
        }
    }
    QList<int> list = axes.toList();
    static const QString destPos("%1 + %2 * %3");
    QString linkDestPos =  QString("ABS(%1 - DPOS AXIS(%2))").arg(destPos.arg(a.value(mainAxis)).arg(b.value(mainAxis)).arg(i)).arg(mainAxis);
    int virtualAxis = p->virtualAxisId();
    AddParameter("linkDest", linkDestPos);
    If(s("linkDest >= %1").arg(driverResolution));
        AddParameter(QString("UNITS AXIS(%1)").arg(virtualAxis), QString("UNITS AXIS(%1)").arg(mainAxis));
        AddParameter(QString("ACCEL AXIS(%1)").arg(virtualAxis), QString("ACCEL AXIS(%1)").arg(mainAxis));
        AddParameter(QString("DECEL AXIS(%1)").arg(virtualAxis), QString("DECEL AXIS(%1)").arg(mainAxis));
        SetSpeed(virtualAxis, speed);
        for (const auto &axis: list) {
            QString currentDestPos = destPos.arg(a.value(axis)).arg(b.value(axis)).arg(i);
            MoveLink(currentDestPos, "linkDest", axis, virtualAxis);
        }
        Move("linkDest", virtualAxis);
        WaitIdle(mainAxis);
        ResetAxisPos(0, virtualAxis);
    Endif();
}

/*! 执行一个轴关联移动 */
void TrioQueue::MoveLink(const QString &destPos, const QString &linkDestPos, int currentAxis, int linkAxis)
{
    queue << s("MOVELINK(%1 - DPOS AXIS(%2), %3, 0, 0, %4) AXIS(%2)").arg(destPos).arg(currentAxis).arg(linkDestPos).arg(linkAxis);
}

void TrioQueue::AddParameter(const QString &parameter, const QString &value)
{
    queue << QString("%1 = %2").arg(parameter, value);
}

void TrioQueue::reverseBoolean(const QString &parameter)
{
    queue << QString("%1 = NOT %1").arg(parameter);
}

void TrioQueue::While(const QString &text)
{
    queue << QString("WHILE %1").arg(text);
    callStack << "While_" + text;
}

void TrioQueue::Wend()
{
    queue << s("WEND");
    if (callStack.isEmpty() || !callStack.takeLast().startsWith("While_")) {
        qCCritical(debug) << "Wend expecting while";
    }
}

void TrioQueue::If(const QString &text)
{
    queue << QString("IF %1 THEN").arg(text);
    callStack << "If_" + text;
}

void TrioQueue::ElseIf(const QString &text)
{
    const QString conditions = text.isEmpty() ? "ELSE" : QString("ELSEIF %1 THEN").arg(text);
    queue << conditions;
    if (callStack.isEmpty() || !callStack.last().startsWith("If_")) {
        qCCritical(debug) << "Endif expecting if";
    }
}

void TrioQueue::Endif()
{
    queue << QString("ENDIF");
    if (callStack.isEmpty() || !callStack.takeLast().startsWith("If_")) {
        qCCritical(debug) << "Endif expecting if";
    }
}

void TrioQueue::WaitConditions(const QList<MotionCondition> &conditions)
{
    if (conditions.isEmpty()) {
        return;
    }

    struct Condition {
        MotionCondition condition;
        QString name;
        QString state;
    };

    QList<Condition> list;
    for (int index = 0; index < conditions.count(); ++index) {
        Condition item;
        item.condition = conditions.value(index);
        item.name = QString("Condition%1").arg(index);
        item.state = QString("State%1").arg(index);
        int value = qRound(item.condition.arg);
        list << item;
        queue << s("%1 = 0").arg(item.name);
        switch (item.condition.type) {
        case MotionCondition::ConditionDelay:
            queue << s("TICKS = (%1 * 1000 / SERVO_PERIOD)").arg(value);
            break;
        case MotionCondition::ConditionIoFalling:
        case MotionCondition::ConditionIoRising:
            queue << s("%1 = (%2 AND IN)").arg(item.state).arg(1 << value);
            break;
        default:
            break;
        }
    }
    Repeat();
    QStringList resultTextList;
    QStringList resultText;
    for (const auto &condition: list) {
        CheckCondition(condition.name, condition.condition, condition.state);
        resultText << condition.name;
        if (resultText.count() > 4) {
            resultTextList << resultText.join(" AND ");
            resultText.clear();
        }
    }
    if (!resultText.isEmpty()) {
        resultTextList << resultText;
    }
    QString firstResult = resultTextList.count() == 1 ? resultTextList.value(0) : resultTextList.value(0) + " _";
    Until(firstResult);
    for (int index = 1; index < resultTextList.count(); ++index) {
        QString format = index == resultTextList.count() - 1 ? s("AND %1") : s("AND %1 _");
        queue << format.arg(resultTextList.value(index));
    }

    OutputIo(15, false);
    Wait(1);
    OutputIo(15, true);
}

void TrioQueue::CheckCondition(const QString &result, const MotionCondition &condition, const QString &state)
{
    int value = qRound(condition.arg);
    switch (condition.type) {
    case MotionCondition::ConditionNone:
        queue << s("%1 = 1").arg(result);
        break;
    case MotionCondition::ConditionDelay:
        If(s("%1 <> 1").arg(result));
            If(s("TICKS <= 0"));
                queue << s("%1 = 1").arg(result);
            Endif();
        Endif();
        break;
    case MotionCondition::ConditionIoRising:
        If(s("%1 <> 1").arg(result));
            queue << s("state = %1 AND IN").arg(1 << value);
            If(s("%1 <> state").arg(state));
                If(s("state"));
                    queue << s("%1 = 1").arg(result);
                    queue << s("DEFPOS(0) AXIS(%1)").arg(condition.axis);
                ElseIf();
                    queue << s("%1 = state").arg(state);
                Endif();
            Endif();
        Endif();
        break;
    case MotionCondition::ConditionIoFalling:
        If(s("%1 <> 1").arg(result));
            queue << s("state = %1 AND IN").arg(1 << value);
            If(s("%1 <> state").arg(state));
                If(s("state"));
                    queue << s("%1 = state").arg(state);
                ElseIf();
                    queue << s("%1 = 1").arg(result);
                    queue << s("DEFPOS(0) AXIS(%1)").arg(condition.axis);
                Endif();
            Endif();
        Endif();
        break;
    default:
        break;
    }
}

/*! 重置前置条件的旋转轴 */
void TrioQueue::ResetPrerequisityAxes(const QList<MotionElement> &elements)
{
    MotionAxisList list;
    foreach (const MotionElement &element, elements) {
        if (element.type != MotionElement::Continuous) {
            continue;
        }
        list.combine(element.axis);
    }
    if (list.axisCount() > 0) {
        if (UpdateBase(list)) {
            queue << s("DEFPOS(0)");
        }
    }
}

void TrioQueue::Repeat()
{
    queue << s("REPEAT");
}

void TrioQueue::Until(const QString &condition)
{
    queue << QString("UNTIL %1").arg(condition);
}

void TrioQueue::CancelAxesLink(const MotionAxisList &axes, int mainAxis)
{
    QString option = s("REP_OPTION AXIS(%1)");
    foreach (int axis, axes.toList()) {
        if (axis == mainAxis) {
            continue;
        }
        queue << s("%1 = %1 OR 2").arg(option.arg(axis));
//        Cancel(axis);
    }
}

void TrioQueue::Quit()
{
    queue << ("STOP " + programName);
}

void TrioQueue::SetAxisParameter(int axis, const QString &key, double value)
{
    SetAxisParameter(axis, key, QString::number(value));
}

void TrioQueue::SetAxisParameter(int axis, const QString &key, const QString &value)
{
    static const QString format("%1 Axis(%2) = %3");
    queue << format.arg(key).arg(axis).arg(value);
}

void TrioQueue::ResetAxisPos(qreal pos, int axis)
{
    queue << QString("DEFPOS(%1) AXIS(%2)").arg(pos).arg(axis);
}

void TrioQueue::SetSpeed(int axis, double speed)
{
    SetAxisParameter(axis, "Speed", speed);
}

void TrioQueue::SetSpeed(int axis, const QString &speed)
{
    SetAxisParameter(axis, "Speed", speed);
}

void TrioQueue::Cancel(int axis)
{
    queue << s("Cancel Axis(%1)").arg(axis);
}

void TrioQueue::MoveContinuous(int axis, bool isReversed)
{
    UpdateBase(AxisList() << axis);
    if (isReversed) {
        queue << "Reverse";
    } else {
        queue << "Forward";
    }
}

void TrioQueue::Move(const TrioQueue::Motion &displacement)
{
    Motion m = CheckAxisRange(displacement);
    if (!m.isEmpty()) {
        UpdateBase(m.keys());
        queue << "Move(" + JoinNumber(m.values()) + ")";
    }
}

void TrioQueue::Move(const QString &pos, int axis)
{
    queue << s("MOVE(%1) AXIS(%2)").arg(pos).arg(axis);
}

void TrioQueue::MoveAbs(const TrioQueue::Motion &pos)
{
    Motion m = CheckAxisRange(pos);
    if (!m.isEmpty()) {
        UpdateBase(m.keys());
        queue << "MoveAbs(" + JoinNumber(m.values()) + ")";
    }
}

void TrioQueue::MoveAbs(qreal pos, int axis)
{
    queue << s("MOVEABS(%1) AXIS(%2)").arg(pos).arg(axis);
}

void TrioQueue::MoveAbs(const MotionPose &pos, const MotionAxisList &axis)
{
    if (!pos.isEmpty()) {
        UpdateBase(axis);
        queue << s("MOVEABS(%1)").arg(JoinNumber(pos));
    }
}

void TrioQueue::MoveCam(const MotionElement &element, double speed, double returnSpeed)
{
    static const float linkDistance = 1e5;

    if (element.pos.isEmpty()) {
        return;
    }

    MotionPose firstPos = element.pos.first();
    MoveAbs(firstPos, element.axis);
    WaitIdle(element.axis.first());

    const auto axisList = element.axis.toList();
    int virtualAxis = p->virtualAxisId();
    int poseLength = element.pos.count();

    /* 填充和配置凸轮表, 凸轮表为轴主序 */
    QVector<qreal> table(poseLength * axisList.count());
    for (int poseIndex = 0; poseIndex < poseLength; ++poseIndex) {
        const MotionPose &pose = element.pos[poseIndex];
        int offset = poseIndex;
        for (int axisIndex = 0; axisIndex < axisList.count(); ++axisIndex) {
            table[offset] = pose.value(axisIndex) - firstPos.value(axisIndex);
            offset += poseLength;
        }
    }
    SetTable(camBase, table);

    for (int index = 0; index < axisList.count(); ++index) {
        const int axis = axisList[index];
        const int offset = camBase + poseLength * index;
        queue << s("CamBox(%1, %2, Units Axis(%3), %4, %5, 4) Axis(%3)")
                 .arg(offset).arg(offset + poseLength - 1).arg(axis).arg(linkDistance).arg(virtualAxis);
    }

    if (isVirtualConfigured) {
        SetAxisParameter(virtualAxis, "Units", 100000);
        SetAxisParameter(virtualAxis, "Accel", 100000);
        SetAxisParameter(virtualAxis, "Decel", 100000);
        isVirtualConfigured = true;
    }
    SetAxisParameter(virtualAxis, "Speed", speed);
    queue << s("Move(%2) Axis(%1)").arg(virtualAxis).arg(linkDistance);
    WaitIdle(virtualAxis);
    SetAxisParameter(virtualAxis, "Speed", returnSpeed);
    queue << s("Move(%2) Axis(%1)").arg(virtualAxis).arg(-linkDistance);
    WaitIdle(virtualAxis);
    for (int index = 0; index < axisList.count(); ++index) {
        const int axis = axisList[index];
        Cancel(axis);
    }
}
template <typename T>
static inline T mod(T x, T y)
{
    T ret = std::fmod(x, y);
    if (ret < 0) {
        ret += y;
    }
    return ret;
}

void TrioQueue::MoveCircle(int axisX, int axisY, int axisC, const QPointF &center, qreal radius, qreal startAngle, qreal offsetAngle, qreal moveAngle)
{
    queue << s("' Circ c(%1, %2) r=%3 theta %4 -> %5").arg(center.x()).arg(center.y()).arg(radius).arg(startAngle).arg(moveAngle);

    MoveAbs(startAngle, axisC);
    WaitIdle(axisC);

    qreal startAngleRadian = qDegreesToRadians(startAngle + offsetAngle);
    Motion pos;
    pos[axisX] = center.x() + radius * qCos(startAngleRadian);
    pos[axisY] = center.y() + radius * qSin(startAngleRadian);
    MoveAbs(pos);
    WaitIdle(axisX);

    int tableEnd = p->m_sineTable + p->m_sineTableSize - 1;
    CamBox(p->m_sineTable, tableEnd, axisX, radius, axisC, 360, mod(offsetAngle + startAngle + 90, 360.0));   /* X = r * cos(angle), 即sin(angle + 90) */
    CamBox(p->m_sineTable, tableEnd, axisY, radius, axisC, 360, mod(offsetAngle + startAngle, 360.0));    /* Y = r * sin(angle) */
    MoveAbs(moveAngle, axisC);
    WaitIdle(axisC);
    Cancel(axisX);
    Cancel(axisY);
    WaitIdle(axisX);
    WaitIdle(axisY);
}

void TrioQueue::CamBox(int tableStart, int tableEnd, int axis, qreal linkScale, int linkAxis, qreal linkDistance, qreal linkOffset)
{
    int flags = 4 + ((qFuzzyIsNull(linkOffset) || qFuzzyCompare(linkOffset, 360.0)) ? 0 : 128); /* Flags含义参见CamBox文档. 当offset=0时, flag 128将报错 */
    queue << s("CamBox(%1, %2, %3 * Units Axis(%8), %4, %5, %6, 0, %7) Axis(%8)")
             .arg(tableStart)   /* %1 */
             .arg(tableEnd)     /* %2 */
             .arg(linkScale)    /* %3: Scale */
             .arg(linkDistance) /* %4: Link distance */
             .arg(linkAxis)     /* %5: Link axis (master axis) */
             .arg(flags)        /* %6 */
             .arg(linkOffset)   /* %7 */
             .arg(axis);        /* %8 */
}

void TrioQueue::Home(int axis, double speed, double creep, int datumIn, bool isReversed)
{
    if (axis < 0 || datumIn < 0) {
        return;
    }
    SetAxisParameter(axis, "Speed", speed);
    SetAxisParameter(axis, "Creep", creep);
    SetAxisParameter(axis, "Datum_In", datumIn);
    queue << s("Datum(%2) Axis(%1)").arg(axis).arg(isReversed ? 3 : 4);
}

void TrioQueue::WaitIdle(int axis)
{
    if (axis < 0) {
        queue << "Wait Idle";
    } else {
        queue << s("Wait Idle Axis(%1)").arg(axis);
    }
}

void TrioQueue::WaitInput(int inputIo, bool waitForOff)
{
    if (inputIo >= 0) {
        queue << s("Wait Until In(%1)=%2").arg(inputIo).arg(waitForOff ? "OFF" : "ON");
    }
}

void TrioQueue::reportMoveStart(int id)
{
    addReport(lineStart, QString::number(id));
}

void TrioQueue::reportMoveStart(const QString &id)
{
    addReport(lineStart, id);
}

void TrioQueue::reportMoveEnd(int id)
{
    addReport(lineEnd, QString::number(id));
}

void TrioQueue::reportMoveEnd(const QString &id)
{
    addReport(lineEnd, id);
}

void TrioQueue::reportError(const QString &error)
{
    const QString format(R"(Print "@%1 %2";"%")");
    queue << format.arg("ERROR", error);
}

void TrioQueue::addReport(const QString &token, const QString &id)
{
    const QString format(R"(Print "@%1 ";Int(%2);"%")");
    queue << format.arg(token, id);
}

void TrioQueue::onDisconnected()
{
    if (state != STATE_IDLE) {
        qCInfo(debug) << "Driver disconnected, stopping";
        Quit();
        UpdateState(STATE_IDLE, -1, -1);
    }
    m_programStatus = PROGRAM_UNKNOWN;
}

void TrioQueue::OnTokenReceived(const QString &token)
{
    static const QRegularExpression pattern("(\\w+) (\\d+)");
    QRegularExpressionMatch match = pattern.match(token);
    if (match.hasMatch()) {
        bool ok = false;
        int progress = match.captured(2).toInt(&ok);
        if (ok && (match.captured(1) == lineStart || match.captured(1) == lineEnd)) {
            qCDebug(debug) << "received report" << progress << total;
            return UpdateState(GetState(), progress, total);
        }
    }
}

void TrioQueue::OnStartTimeout()
{
    if (GetState() == STATE_STARTING) {
        errorString = "Failed starting command queue: Trio didn't respond";
        qCInfo(debug) << errorString;
        UpdateState(STATE_ERROR, -1, -1);
    }
}

void TrioQueue::setProgramStatus(int status)
{
    if (m_programStatus == status) {
        return;
    }
    m_programStatus = status;
    switch (status) {
    case PROGRAM_STOPPING:
    case PROGRAM_STOPPED:
        UpdateState(STATE_IDLE, -1, total);
        break;
    case PROGRAM_STEPPING:
    case PROGRAM_PAUSING:
    case PROGRAM_PAUSED:
        UpdateState(STATE_PAUSED, progress, total);
        break;
    case PROGRAM_RUNNING:
    case PROGRAM_SUSPEND:
    case PROGRAM_SUSPENDED:
    case PROGRAM_SLEEP:
        UpdateState(STATE_RUNNING, progress, total);
        break;
    default:
        qCCritical(debug) << "Unknown program status" << status;
    }
    qCDebug(debug) << "Program status" << ProgramStatus(status);
}

void TrioQueue::OnCommandSent(int id)
{
//    qCDebug(debug) << "command sent" << id << baseCommandId << finalCommandId;
    if (GetState() == STATE_SENDING && id <= finalCommandId && id >= baseCommandId) {
        const int progress = id - baseCommandId;
        const int notifyTotal = finalCommandId - baseCommandId;
        UpdateState(STATE_SENDING, qreal(progress) / notifyTotal * total, total);

        if (progress == notifyTotal) {
            qCDebug(debug) << "Send progress" << progress << notifyTotal;
            UpdateState(STATE_STARTING, -1, total);
            if (Start(handle) < 0) {
                UpdateState(STATE_ERROR, -1, total);
            }
        }
    }
}

void TrioQueue::UpdateState(TrioQueue::State newState, int newProgress, int newTotal)
{
    if (state == newState && progress == newProgress && total == newTotal) {
        return;
    }
    stateTime = QTime::currentTime();
    state = newState;
    progress = newProgress;
    if (state != STATE_SENDING) {
        total = newTotal;
    }
    qCDebug(debug) << "progress" << GetState() << progress << total;
    emit StateUpdated(GetState(), progress, total);
}

TrioQueue::Motion TrioQueue::CheckAxisRange(const Motion &table)
{
    Motion filtered = table;
    for (auto it = table.cbegin(); it != table.cend(); ++it) {
        if (p->m_axisIndex.contains(it.key())) {
            filtered[it.key()] = it.value();
        }
    }
    return filtered;
}

bool TrioQueue::UpdateBase(const MotionAxisList &newBase)
{
    if (newBase != base) {
        base = newBase;
        QStringList axisTextList;
        foreach (int axis, base.toList()) {
            axisTextList << QString::number(axis);
        }
        queue << "Base(" + axisTextList.join(",") + ")";
        return true;
    }
    return false;
}

void TrioQueue::AddBreakPoint()
{
    queue << "TrOn";
}

void TrioQueue::CheckAxisPosition(qreal pos, int axis, const QString &op)
{
    If(s("DPOS AXIS(%1) %3 %2").arg(axis).arg(pos).arg(op));
        reportError("AxisPositionMismatch");
        Quit();
    Endif();
}

void TrioQueue::OutputIo(int outputIo, bool value)
{
    if (outputIo >= 0) {
        queue << s("Op(%1, %2)").arg(outputIo).arg(value ? "ON" : "OFF");
    }
}

void TrioQueue::Wait(int msec)
{
    queue << s("Wa(%1)").arg(msec);
}

void TrioQueue::SetTable(int offset, const QVector<qreal> &data)
{
    static const QString tableFormat("Table(%1, %2)");
    for (int index = 0; index < data.count(); index += tableInstructionLength) {
        QList<qreal> list;
        int lastIndex = qMin(index + tableInstructionLength, data.count());
        for (int numberIndex = index; numberIndex < lastIndex; ++numberIndex) {
            list << data[numberIndex];
        }
        queue << tableFormat.arg(offset + index).arg(JoinNumber(list));
    }
}

void TrioQueue::For(const QString &key, int from, int to, int step)
{
    queue << s("For %1 = %2 To %3 Step %4").arg(key).arg(from).arg(to).arg(step);
//    callStack << "For_" + key;
}

void TrioQueue::For(const QString &key, const QString &from, const QString &to, int step)
{
    queue << s("For %1 = %2 To %3 Step %4").arg(key, from, to).arg(step);
    callStack << "For_" + key;
}

int TrioQueue::Next(const QString &key)
{
    if (callStack.isEmpty() || callStack.takeLast() != "For_" + key) {
        return ERR_BAD_STRUCTURE;
    }
    queue << s("Next %1").arg(key);
    return SUCCESS;
}
