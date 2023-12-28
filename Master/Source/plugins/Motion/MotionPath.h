#pragma once

#include "ChannelList.h"
#include "Motion/MotionElement.h"
#include "Motion/RotationInfo.h"
#include "MotionElement.h"
#include "RotationInfo.h"
#include "SafetyBarrierOptions.h"
#include <QCoreApplication>
#include <QList>
#include <QMetaType>

/*!
 * \brief MotionPath 是一个完整的扫查路径
 */
class MOTION_EXPORT MotionPath
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(MotionPath)

    Q_PROPERTY(QList<MotionElement> prerequisites MEMBER prerequisites)
    Q_PROPERTY(QList<MotionElement> elements MEMBER elements)
    Q_PROPERTY(QList<RotationInfo> rotations MEMBER rotations)
    Q_PROPERTY(qreal interval MEMBER interval)
    Q_PROPERTY(bool multiChannelEnable MEMBER multiChannelEnable)
    Q_PROPERTY(QList<ChannelList> multiChannels MEMBER multiChannels)
    Q_PROPERTY(qreal channelOffset MEMBER channelOffset)
    Q_PROPERTY(SafetyBarrierOptions safetyBarrier MEMBER safetyBarrier)


public:
    DECLARE_COMPARE_OPERATORS(MotionPath)

    void addElement(const MotionElement &element);
    void addPrerequisite(const MotionElement &elements);
    void addRotation(const RotationInfo &rotaion);
//    void addLocate(const QMap<int, qreal> &move);
    int moveCount(int elementIndex) const;
    int moveCountTo(int elementIndex, bool &isReverse) const;
    MotionElement elementByMove(int index, bool &isReverse) const;
    int canInterpolate(int elementIndex) const;

    MotionAxisList allAxes() const;
    MotionAxisList prerequisitesAxes() const;
    MotionAxisList ptpAxes() const;

    qreal timeExpected(int index = 0) const;
    qreal timeRemains(int index, const MotionPose &pos) const;
    qreal moveTime(int elementIndex, int moveIndex) const;
    qreal moveTime(int elementIndex) const;
    qreal conditionTime(const MotionElement &element, bool &isRotation) const;
    qreal posePercent(int move, const MotionPose &pose) const;
    Q_REQUIRED_RESULT MotionPath expandMultiChannel() const;
    Q_REQUIRED_RESULT MotionPath expandAll() const;

    QJsonValue toJson() const;
    static MotionPath fromJson(const QJsonValue &json);
    static bool isInbounds(qreal start, qreal value, qreal end);

    struct CheckResult {
        QStringList warning;
        QStringList error;
        QList<MotionElement> path;
    };
    CheckResult GeneratePath(bool dryRun = false) const;

public:
    QList<MotionElement> prerequisites;
    QList<MotionElement> elements;
    QList<RotationInfo> rotations;
    qreal interval = 0;                                         // TODO 此interval 与 MotionElement 中的stepInterval 重复, 不指导运动控制, 待删除
    bool multiChannelEnable = false;
    QList<ChannelList> multiChannels;
    qreal channelOffset = 0;
    SafetyBarrierOptions safetyBarrier;
};
