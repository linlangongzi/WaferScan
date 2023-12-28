#pragma once

#include <QMetaType>
#include <QDebug>
#include "allrising_global.h"

class Range;
class QTransform;

/*! \class Transform1D
 * 一维线性坐标变换 y = ax + b
 */
class ALLRISING_UTILS_EXPORT Transform1D
{
public:
    Transform1D(qreal scale = 1, qreal offset = 0);
    Transform1D(const QTransform &transform, Qt::Orientation orientation);

    void setScale(qreal scale);
    void setOffsetBefore(qreal offset);
    void setOffset(qreal offset);

    bool hasOffset() const;
    bool hasScale() const;
    bool isReversed() const;
    Q_REQUIRED_RESULT Transform1D inverted(bool *ok = nullptr) const;
    bool isInvertable() const;
    Transform1D scaled(qreal scale) const;
    Transform1D translated(qreal offset) const;

    qreal scale() const;
    qreal offsetBefore() const;
    qreal offsetAfter() const;
    qreal map(qreal value) const;
    Range map(const Range &value) const;
    qreal mapInverted(qreal value) const;
    Range mapInverted(const Range &value) const;

    Transform1D operator *(const Transform1D &other) const;
    void operator *=(const Transform1D &other);
    bool operator ==(const Transform1D &other) const;

    Q_REQUIRED_RESULT static QTransform to2D(const Transform1D &horizontal, const Transform1D &vertical);

private:
    qreal a;
    qreal b;
};

Q_DECLARE_METATYPE(Transform1D)
Q_DECLARE_TYPEINFO(Transform1D, Q_MOVABLE_TYPE);

QDebug operator <<(QDebug debug, const Transform1D &transform);
