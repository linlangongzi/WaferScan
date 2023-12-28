#pragma once

#include <QObject>
#include <QKeySequence>
#include "AxisKinematics.h"
#include "MotionGlobal.h"

class MOTION_EXPORT AxisConfig
{
    Q_GADGET

    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(int axisId MEMBER axisId)
    Q_PROPERTY(AxisType axisType MEMBER axisType)
    Q_PROPERTY(qreal units MEMBER units)
    Q_PROPERTY(qreal unitsInstrument MEMBER unitsInstrument)
    Q_PROPERTY(qreal repeatDistance MEMBER repeatDistance)

    Q_PROPERTY(qreal backlash MEMBER backlash)
    Q_PROPERTY(int encoderId MEMBER encoderId)
    Q_PROPERTY(bool isLocateMaster MEMBER isLocateMaster)
    Q_PROPERTY(QMap<QString, int> sensors MEMBER sensors)
    Q_PROPERTY(qreal speedMax MEMBER speedMax)
    Q_PROPERTY(qreal accelLimit MEMBER accelLimit)
    Q_PROPERTY(qreal decelLimit MEMBER decelLimit)
    Q_PROPERTY(qreal sRamp MEMBER sRamp)
    Q_PROPERTY(qreal limitReverse MEMBER limitReverse)
    Q_PROPERTY(qreal limitForward MEMBER limitForward)
    Q_PROPERTY(int limitReverseInput MEMBER limitReverseInput)
    Q_PROPERTY(int limitForwardInput MEMBER limitForwardInput)
    Q_PROPERTY(int datumInput MEMBER datumInput)
    Q_PROPERTY(bool datumReversed MEMBER datumReversed)
    Q_PROPERTY(qreal datumSpeed MEMBER datumSpeed)
    Q_PROPERTY(qreal datumCreep MEMBER datumCreep)
    Q_PROPERTY(qreal datumPosition MEMBER datumPosition)
    Q_PROPERTY(int datumSequence MEMBER datumSequence)
    Q_PROPERTY(qreal axleDiameter MEMBER axleDiameter)

    Q_PROPERTY(int jogForwardInput MEMBER jogForwardInput)
    Q_PROPERTY(int jogReverseInput MEMBER jogReverseInput)

    Q_PROPERTY(bool hidden MEMBER hidden)
    Q_PROPERTY(QVector3D basePoint MEMBER basePoint)
    Q_PROPERTY(QVector3D baseDirection MEMBER baseDirection)
    Q_PROPERTY(QList<AxisKinematics> kinematics MEMBER kinematics)

public:
    enum AxisType {
        Prismatic,
        Rotation,
        RotationLimited
    };
    Q_ENUM(AxisType)

    QString name;
    int axisId = -1;
//    DriveType driveType = StepDirection;
    AxisType axisType = Prismatic;
    qreal units = 1;
    qreal unitsInstrument = 1;
    qreal repeatDistance = Q_SNAN;
    qreal backlash = 0;
    int encoderId = -1;
    bool isLocateMaster = false;
    QMap<QString, int> sensors;

    qreal speedMax = 1;
    qreal accelLimit = 1;
    qreal decelLimit = 1;
    qreal sRamp = 0;

    qreal limitReverse = Q_SNAN;
    qreal limitForward = Q_SNAN;
    int limitReverseInput = -1;
    int limitForwardInput = -1;

    qreal axleDiameter = 0;
    int datumInput = -1;
    bool datumReversed = false;
    qreal datumSpeed = 1;
    qreal datumCreep = 1;
    qreal datumPosition = 0;
    int datumSequence = 0;

    int jogForwardInput = -1;
    int jogReverseInput = -1;

    bool hidden = false;

    QVector3D basePoint;
    QVector3D baseDirection;
    QList<AxisKinematics> kinematics;

    DECLARE_COMPARE_OPERATORS(AxisConfig)
};

class MOTION_EXPORT AxisSetup
{
    Q_GADGET

    Q_PROPERTY(qreal jogStep MEMBER jogStep)
    Q_PROPERTY(qreal speedJog MEMBER speedJog)
    Q_PROPERTY(qreal speedLocate MEMBER speedLocate)
    Q_PROPERTY(qreal accel MEMBER accel)
    Q_PROPERTY(qreal decel MEMBER decel)
    Q_PROPERTY(bool disabled MEMBER disabled)
    Q_PROPERTY(QKeySequence forwardKey MEMBER forwardKey)
    Q_PROPERTY(QKeySequence reverseKey MEMBER reverseKey)

public:
    DECLARE_COMPARE_OPERATORS(AxisSetup)

    qreal jogStep = 1;
    qreal speedJog = 1;
    qreal speedLocate = 1;
    qreal accel = 1;
    qreal decel = 1;

    bool disabled = false;

    QKeySequence forwardKey;
    QKeySequence reverseKey;
};

class MOTION_EXPORT AxisStatus
{
    Q_GADGET

    Q_PROPERTY(int axisType MEMBER axisType)
    Q_PROPERTY(qreal position MEMBER position)
    Q_PROPERTY(MoveType moveType MEMBER moveType)
    Q_PROPERTY(bool datumDone MEMBER datumDone)
    Q_PROPERTY(bool limitedForwardInput MEMBER limitedForwardInput)
    Q_PROPERTY(bool limitedReverseInput MEMBER limitedReverseInput)
    Q_PROPERTY(bool limitedForward MEMBER limitedForward)
    Q_PROPERTY(bool limitedReverse MEMBER limitedReverse)

public:
    enum MoveType {
        Idle = 0,
        MOVE = 1,
        MOVEABS = 2,
        MHELICAL = 3,
        MOVECIRC = 4,
        MOVEMODIFY = 5,
        MOVESP = 6,
        MOVEABSSP = 7,
        MOVECIRCSP = 8,
        MHELICALSP = 9,
        FORWARD = 10,
        REVERSE = 11,
        DATUM = 12,
        CAM = 13,
        FWD_JOG = 14,
        REV_JOG = 15,
        CAMBOX = 20,
        CONNECT = 21,
        MOVELINK = 22,
        CONNPATH = 23,
        FLEXLINK  = 24,
        MOVETANG = 30,
        MSPHERICAL = 31,
    };
    Q_ENUM(MoveType)

    DECLARE_COMPARE_OPERATORS(AxisStatus)

    inline bool forwardLimited() const
    {
        return (limitedForwardInput || limitedForward);
    }
    inline bool reverseLimited() const
    {
        return (limitedReverseInput || limitedReverse);
    }

    int axisType = 0;
    qreal position = 0;
    MoveType moveType = Idle;
    bool datumDone = false;
    bool limitedForwardInput = false;
    bool limitedReverseInput = false;
    bool limitedForward = false;
    bool limitedReverse = false;
};
