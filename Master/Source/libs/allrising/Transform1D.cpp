#include "Transform1D.h"
#include <QTransform>
#include "Range.h"

Transform1D::Transform1D(qreal scale, qreal offset) :
    a(scale),
    b(offset)
{}

Transform1D::Transform1D(const QTransform &transform, Qt::Orientation orientation)
{
    if (orientation == Qt::Horizontal) {
        a = transform.m11();
        b = transform.m31();
    } else {
        a = transform.m22();
        b = transform.m32();
    }
}

void Transform1D::setScale(qreal scale)
{
    a = scale;
}

void Transform1D::setOffsetBefore(qreal offset)
{
    b = offset / a;
}

void Transform1D::setOffset(qreal offset)
{
    b = offset;
}

qreal Transform1D::scale() const
{
    return a;
}

qreal Transform1D::offsetBefore() const
{
    return -b / a;
}

qreal Transform1D::offsetAfter() const
{
    return b;
}

qreal Transform1D::map(qreal value) const
{
    return a * value + b;
}

Range Transform1D::map(const Range &value) const
{
    qreal min = map(value.min());
    qreal max = map(value.max());
    if (max < min) {
        qSwap(min, max);
    }
    return Range(min, max);
}

qreal Transform1D::mapInverted(qreal value) const
{
    return value / a - b;
}

Range Transform1D::mapInverted(const Range &value) const
{
    qreal min = mapInverted(value.min());
    qreal max = mapInverted(value.max());
    if (max < min) {
        qSwap(min, max);
    }
    return Range(min, max);
}

Transform1D Transform1D::operator *(const Transform1D &other) const
{
    return Transform1D(a * other.a, b * other.a + other.b);                     /* t * t' => a'(ax + b) + b' */
}

void Transform1D::operator *=(const Transform1D &other)
{
    b = b * other.a + other.b;
    a *= other.a;
}

bool Transform1D::operator ==(const Transform1D &other) const
{
    return qFuzzyCompare(a, other.a) && qFuzzyCompare(b, other.b);
}

QTransform Transform1D::to2D(const Transform1D &horizontal, const Transform1D &vertical)
{
    return QTransform::fromScale(horizontal.a, vertical.a).translate(horizontal.b, vertical.b);
}

QDebug operator <<(QDebug debug, const Transform1D &transform)
{
    QDebugStateSaver state(debug);
    Q_UNUSED(state);
    debug.nospace() << "Transform1D(" << transform.scale() << "x + " << transform.offsetAfter() << ")";
    return debug;
}
