#pragma once

#include <QObject>
#include "Material.h"
#include <allrising/Util.h>

class ULTRASONIC_EXPORT UTSetupSurfaceTracing
{
    Q_GADGET

    Q_PROPERTY(bool enable MEMBER enable)
    Q_PROPERTY(qreal offset MEMBER offset)
    Q_PROPERTY(qreal threshold MEMBER threshold)
    Q_PROPERTY(qreal earlist MEMBER earlist)
    Q_PROPERTY(qreal latest MEMBER latest)

public:
    DECLARE_COMPARE_OPERATORS(UTSetupSurfaceTracing)

public:
    bool enable = false;
    qreal offset = 0;
    qreal threshold = 0;
    qreal earlist = 0;
    qreal latest = 0;
};

class ULTRASONIC_EXPORT UTSetupTimebase
{
    Q_GADGET

    Q_PROPERTY(qreal delay READ delay WRITE setDelay)
    Q_PROPERTY(qreal range READ range WRITE setRange)
    Q_PROPERTY(qreal zero READ zero WRITE setZero)
    Q_PROPERTY(TimebaseUnit timebaseUnit READ timebaseUnit WRITE setTimebaseUnit)
    Q_PROPERTY(Material material READ material WRITE setMaterial)
    Q_PROPERTY(UTSetupSurfaceTracing surfaceTracing READ surfaceTracing WRITE setSurfaceTracing)

public:
    enum TimebaseUnit {
        Time,
        Distance,
    };
    Q_ENUM(TimebaseUnit)

public:
    qreal delay() const;
    qreal range() const;
    qreal zero() const;
    TimebaseUnit timebaseUnit() const;
    Material material() const;
    UTSetupSurfaceTracing surfaceTracing() const;

public:
    void setDelay(qreal delay);
    void setRange(qreal range);
    void setZero(qreal zero);
    void setTimebaseUnit(TimebaseUnit timebaseUnit);
    void setMaterial(const Material &material);
    void setSurfaceTracing(const UTSetupSurfaceTracing &surfaceTracing);

    QString timebaseSuffix() const;
    qreal end() const;
    qreal toTime(qreal distance) const;
    qreal toDistance(qreal time) const;
    qreal toTimebaseUnit(qreal time) const;
    qreal fromTimebaseUnit(qreal timebase) const;
    QString timebaseText(qreal time) const;
    static int timebasePrecision();
    static void setTimebasePrecision(int precision);

    DECLARE_COMPARE_OPERATORS(UTSetupTimebase)

private:
    qreal m_delay = 0;
    qreal m_range = 100;
    qreal m_zero = 0;
    TimebaseUnit m_timebaseUnit = Distance;
    Material m_material;
    UTSetupSurfaceTracing m_surfaceTracing;
};
