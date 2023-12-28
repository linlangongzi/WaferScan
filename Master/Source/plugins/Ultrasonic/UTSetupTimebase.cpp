#include "UTSetupTimebase.h"
#include <allrising/Util.h>

REGISTER_JSON_CONVERTER(UTSetupTimebase)
REGISTER_JSON_CONVERTER(UTSetupSurfaceTracing)

static QAtomicInt m_timebasePrecision = 2;

qreal UTSetupTimebase::delay() const
{
    return m_delay;
}

qreal UTSetupTimebase::range() const
{
    return m_range;
}

qreal UTSetupTimebase::zero() const
{
    return m_zero;
}

UTSetupTimebase::TimebaseUnit UTSetupTimebase::timebaseUnit() const
{
    return m_timebaseUnit;
}

Material UTSetupTimebase::material() const
{
    return m_material;
}

UTSetupSurfaceTracing UTSetupTimebase::surfaceTracing() const
{
    return m_surfaceTracing;
}

void UTSetupTimebase::setDelay(qreal delay)
{
    m_delay = delay;
}

void UTSetupTimebase::setRange(qreal range)
{
    m_range = range;
}

void UTSetupTimebase::setZero(qreal zero)
{
    m_zero = zero;
}

void UTSetupTimebase::setTimebaseUnit(UTSetupTimebase::TimebaseUnit timebaseUnit)
{
    m_timebaseUnit = timebaseUnit;
}

void UTSetupTimebase::setMaterial(const Material &material)
{
    m_material = material;
}

void UTSetupTimebase::setSurfaceTracing(const UTSetupSurfaceTracing &surfaceTracing)
{
    m_surfaceTracing = surfaceTracing;
}

QString UTSetupTimebase::timebaseSuffix() const
{
    return m_timebaseUnit == Distance ? "mm" : "us";
}

qreal UTSetupTimebase::end() const
{
    return m_delay + m_range;
}

qreal UTSetupTimebase::toTime(qreal distance) const
{
    return distance * 2 / m_material.longitudinalVelocity * 1000;
}

qreal UTSetupTimebase::toDistance(qreal time) const
{
    return time * m_material.longitudinalVelocity / 1000 / 2;
}

qreal UTSetupTimebase::toTimebaseUnit(qreal time) const
{
    if (m_timebaseUnit == Distance) {
        return toDistance(time);
    } else {
        return time;
    }
}

qreal UTSetupTimebase::fromTimebaseUnit(qreal timebase) const
{
    if (m_timebaseUnit == Distance) {
        return toTime(timebase);
    } else {
        return timebase;
    }
}

QString UTSetupTimebase::timebaseText(qreal time) const
{
    return QString("%1 %2").arg(toTimebaseUnit(time), 0, 'f', m_timebasePrecision.load()).arg(timebaseSuffix());
}

int UTSetupTimebase::timebasePrecision()
{
    return m_timebasePrecision;
}

void UTSetupTimebase::setTimebasePrecision(int precision)
{
    m_timebasePrecision = precision;
}
