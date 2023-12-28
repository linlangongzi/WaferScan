#pragma once

#include <QtGlobal>
#include <QMetaType>
#include <QDebug>
#include <QRectF>
#include "allrising_global.h"

class ALLRISING_UTILS_EXPORT Range
{
public:
    /*!
     * \brief 范围限制模式
     */
    enum ConstrainMode
    {
        KeepWidthExpandUp,                                                      /* 尝试保持宽度和下限，优先向上移动，如果向上移动失败将向下移动，不会超出约束范围 */
        KeepWidthExpandDown,                                                    /* 尝试保持宽度和上限，优先向下移动，如果向下移动失败将向上移动，不会超出约束范围 */
        LimitOnly,                                                              /* 不尝试保持宽度，仅去除约束范围之外的部分 */
        StrictlyKeepWidthExpandUp,                                              /* 优先向上移动，严格保持宽度，忽略约束上限 */
        StrictlyKeeyWidthExpandDown                                             /* 优先向下移动，严格保持宽度，忽略约束下限 */
    };

    Range();
    Range(qreal min, qreal max);
    Range(const QPair<qreal, qreal> &range);
    Range(const Range &other);
    Range(const QRectF &rect, Qt::Orientation orientation = Qt::Horizontal);

    qreal min() const;
    qreal max() const;
    qreal length() const;
    qreal center() const;
    qreal diff(const Range &other) const;

    Range &setMin(qreal min);
    Range &setMax(qreal max);
    Range &setRange(qreal min, qreal max);
    Range &setWidth(qreal width);
    Range &setWidth(qreal width, const Range &constraint);

    Range &shiftBy(qreal scale);
    Range shiftedBy(qreal scale) const;

    Range &moveBy(qreal offset);
    Range movedBy(qreal offset) const;

    Range &expandTo(qreal value);

//    Range &ScaleOnCenter(qreal scale);
    Range &scaleOnCenter(qreal scale, qreal pos = Q_SNAN);
//    Range ScaledOnCenter(qreal scale) const;
    Range scaledOnCenter(qreal scale, qreal center = Q_SNAN) const;

    Range &scale(qreal scale);
    Range scaled(qreal scale) const;

    Range &constrainTo(const Range &constraint, ConstrainMode mode = KeepWidthExpandUp);
    Range constrainedTo(const Range &constraint, ConstrainMode mode = KeepWidthExpandUp) const;

    Range &combine(const Range &other);
    Range combined(const Range &other) const;

    Range &centerAt(qreal center);
    Range centeredAt(qreal center) const;

    qreal boundValue(qreal value) const;
    qreal scaleValue(qreal value, const Range &target) const;
    qreal mapValue(qreal value, const Range &target) const;
    bool isInside(qreal value) const;
    bool isEqual(const Range &other) const;
    bool operator == (const Range &other) const;
    bool operator !=(const Range &other) const;

    static Range fromCenterWidth(qreal center, qreal width);
    static QRectF toRect(const Range &horizontal, const Range &vertical);
    static QRectF constrainRect(const QRectF &rect, const QRectF &boundingRect);
    static Range infinite();

private:
    static bool fuzzyCompare(qreal v1, qreal v2);

private:
    qreal m_bottom;
    qreal m_top;
};

Q_DECLARE_METATYPE(Range)
Q_DECLARE_TYPEINFO(Range, Q_MOVABLE_TYPE);

ALLRISING_UTILS_EXPORT QDebug operator <<(QDebug debug, const Range &range);
