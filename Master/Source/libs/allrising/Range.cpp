#include "Range.h"

/*!
 * \brief 默认构造一个上下限均为0的范围
 */
Range::Range() :
    m_bottom(0),
    m_top(0)
{}

/*!
 * \brief 使用指定的上下限构造一个范围
 * \param min 下限
 * \param max 上限
 */
Range::Range(qreal min, qreal max)
{
    setRange(min, max);
}

Range::Range(const QPair<qreal, qreal> &range)
{
    setRange(range.first, range.second);
}

Range::Range(const Range &other) = default;

Range::Range(const QRectF &rect, Qt::Orientation orientation)
{
    switch (orientation)
    {
    case Qt::Horizontal:
        setRange( rect.left(), rect.right() ); break;
    case Qt::Vertical:
    default:
        setRange( rect.top(), rect.bottom() );
    }
}

qreal Range::min() const
{
    return m_bottom;
}

qreal Range::max() const
{
    return m_top;
}

qreal Range::length() const
{
    return m_top - m_bottom;
}

qreal Range::center() const
{
    return (m_top + m_bottom) * 0.5;                                                /* 浮点乘法比除法更快 */
}

/*!
 * \brief 计算目标Range到当前对象之间的偏移。两个Range的宽度必须相同
 * \param other 目标Range
 * \return 距离(other - this)。如果宽度不同，则返回QNaN
 */
qreal Range::diff(const Range &other) const
{
    if ( fuzzyCompare( length(), other.length() ) )
    {
        return other.m_bottom - m_bottom;
    }
    return qQNaN();
}

/*!
 * \brief 设置范围的下限，请注意，下限不允许超过当前的上限。
 * 如果需要同时设置上下限，请使用SetRange
 * \param min 下限值
 * \return 当前对象的引用
 */
Range &Range::setMin(qreal min)
{
    m_bottom = qMin(min, m_top);
    return *this;
}

/*!
 * \brief 设置范围的上限，请注意，上限不允许低于当前的下限。
 * 如果需要同时设置上下限，请使用SetRange
 * \param max 上限值
 * \return 当前对象的引用
 */
Range &Range::setMax(qreal max)
{
    m_top = qMax(max, m_bottom);
    return *this;
}

/*!
 * \brief 同时设置范围的上下限，如果上限低于下限，则将以下限为准
 * \param min 下限值
 * \param max 上限值
 * \return当前对象的引用
 */
Range &Range::setRange(qreal min, qreal max)
{
    m_bottom = min;
    setMax(max);
    return *this;
}

/*!
 * \brief 设置宽度。下限将保持不变
 * \param width 新的宽度。负的宽度将按0处理。
 * \return 当前对象的引用
 */
Range &Range::setWidth(qreal width)
{
    m_top = m_bottom + qMax<qreal>(width, 0);
    return *this;
}

Range &Range::setWidth(qreal width, const Range &constraint)
{
    return setWidth(width).constrainTo(constraint);
}

/*!
 * \brief 平移指定的比例，即平移(n * length)。
 * 例如scale = 1则向上平移1倍当前宽度，scale = -0.5则向下平移当前宽度的一半。
 * \param scale 平移距离倍数
 * \return 当前对象的引用
 */
Range &Range::shiftBy(qreal scale)
{
    moveBy( scale * length() );
    return *this;
}

Range Range::shiftedBy(qreal scale) const
{
    return Range(*this).shiftBy(scale);
}

/*!
 * \brief 平移指定的距离，维持宽度不变
 * \param offset 位移
 * \return 当前对象的引用
 */
Range &Range::moveBy(qreal offset)
{
    m_bottom += offset;
    m_top += offset;
    return *this;
}

Range Range::movedBy(qreal offset) const
{
    return Range(*this).moveBy(offset);                                         /* 使用复制构造体复制一份，再进行移动 */
}

Range &Range::expandTo(qreal value)
{
    m_bottom = qMin(value, m_bottom);
    m_top = qMax(value, m_top);
    return *this;
}

///*!
// * \brief 以范围中心为基准，进行缩放
// * \param scale 缩放比例
// * \return 当前对象的引用
// */
//Range &Range::ScaleOnCenter(qreal scale)
//{
//    return ScaleOnCenter(scale, center());
//}

/*!
 * \brief 以指定位置为基准，进行缩放
 * \param scale 缩放比例，1为不变，大于1的值为放大，小于1为缩小。小于0的值等价于0。
 * \param center 中心位置，当值为NaN时使用当前范围的中心。
 * \return 当前对象的引用
 */
Range &Range::scaleOnCenter(qreal scale, qreal pos)
{
    pos = qIsFinite(pos) ? center() : pos;
//    center = boundValue(center);
    scale = qMax<qreal>(scale, 0) - 1;                                          /* 将scale限制为非负值，并-1 */
    m_bottom += (pos - m_bottom) * scale;
    m_top -= (m_top - pos) * scale;
    return *this;
}

//Range Range::ScaledOnCenter(qreal scale) const
//{
//    return ScaledOnCenter(scale, center());
//}

/*!
 * \brief 以指定位置为基准，进行缩放，但不修改当前对象。
 * \param scale 缩放比例
 * \param center 中心位置
 * \return 缩放后的范围
 */
Range Range::scaledOnCenter(qreal scale, qreal center) const
{
    return Range(*this).scaleOnCenter(scale, center);
}

/*! 将当前range比例放大 */
Range &Range::scale(qreal scale)
{
    m_bottom *= scale;
    m_top *= scale;
    return *this;
}

/*! 不修改当前对象的比例放大 */
Range Range::scaled(qreal scale) const
{
    return Range(*this).scale(scale);
}

/*!
 * \brief 将当前范围限制在一个约束范围之内。
 * \param constraint 约束范围
 * \param mode 约束模式，参见 ConstrainMode
 * \return 当前对象的引用
 */
Range &Range::constrainTo(const Range &constraint, ConstrainMode mode)
{
    qreal width = length();
    switch (mode)
    {
    case KeepWidthExpandUp:
        m_bottom = qMax(m_bottom, constraint.m_bottom);                         /* 首先确定一个理想的下限 */
        m_top = qMin(m_bottom + width, constraint.m_top);                       /* 尝试向上扩展，可能碰到约束上限，导致宽度不够 */
        m_bottom = qMax(m_top - width, constraint.m_bottom);                    /* 如果没有碰到约束上限，则top - width == bottom。如果碰到了约束上限，则尝试向下扩展保持宽度 */
        break;
    case KeepWidthExpandDown:
        m_top = qMin(m_top, constraint.m_top);
        m_bottom = qMax(m_top - width, constraint.m_bottom);
        m_top = qMin(m_top + width, constraint.m_top);
        break;
    case LimitOnly:
        m_top = qMin(m_top, constraint.m_top);
        m_bottom = qMax(m_bottom, constraint.m_bottom);
        break;
    case StrictlyKeepWidthExpandUp:
        m_bottom = qMax(m_bottom, constraint.m_bottom);                         /* 确定一个理想的下限 */
        m_top = m_bottom + width;                                               /* 强制保持宽度，忽略约束下限 */
        break;
    case StrictlyKeeyWidthExpandDown:
        m_top = qMin(m_top, constraint.m_top);
        m_bottom = m_top - width;
    }

    return *this;
}

Range Range::constrainedTo(const Range &constraint, ConstrainMode mode) const
{
    return Range(*this).constrainTo(constraint, mode);
}

Range &Range::combine(const Range &other)
{
    m_bottom = qMin(m_bottom, other.m_bottom);
    m_top = qMax(m_top, other.m_top);
    return *this;
}

Range Range::combined(const Range &other) const
{
    return Range(*this).combine(other);
}

Range &Range::centerAt(qreal center)
{
    qreal oldCenter = (m_top + m_bottom) / 2;
    m_bottom += center - oldCenter;
    m_top += center - oldCenter;
    return *this;
}

Range Range::centeredAt(qreal center) const
{
    return Range(*this).centerAt(center);
}

/*!
 * \brief 将一个值限制在目标范围内
 * \param value 被限制的值
 * \return 限制后的值
 */
qreal Range::boundValue(qreal value) const
{
    return qBound(m_bottom, value, m_top);
}

/*!
 * \brief 将一个值从当前范围缩放到目标范围。缩放后的值可能会超出目标范围。
 * \param value 被缩放的值
 * \param target 目标范围
 * \return 缩放后的值
 */
qreal Range::scaleValue(qreal value, const Range &target) const
{
    qreal len = length();
    if (qFuzzyIsNull(len)) {                                                    /* 初数为零，返回下限 */
        return target.min();
    }
    return value / len * target.length() + target.min();
}

/*!
 * \brief 将一个值从当前范围映射到目标范围
 * \param value 被映射的值
 * \param target 目标范围
 * \return 重映射后的值
 */
qreal Range::mapValue(qreal value, const Range &target) const
{
    return target.m_bottom + scaleValue(value - m_bottom, target);
}

/*!
 * \brief 判断一个点是否落在当前范围内
 * \param value 待测值
 * \return true落在范围内，false在范围之外
 */
bool Range::isInside(qreal value) const
{
    return value >= m_bottom && value <= m_top;
}

bool Range::isEqual(const Range &other) const
{
    return fuzzyCompare(m_bottom, other.m_bottom) && fuzzyCompare(m_top, other.m_top);
}

bool Range::operator ==(const Range &other) const
{
    return isEqual(other);
}

bool Range::operator !=(const Range &other) const
{
    return !isEqual(other);
}

Range Range::fromCenterWidth(qreal center, qreal width)
{
    width *= 0.5;
    return Range(center - width, center + width);
}

QRectF Range::toRect(const Range &horizontal, const Range &vertical)
{
    return QRectF( horizontal.min(), vertical.min(), horizontal.length(), vertical.length() );
}

/*! 尝试将一个矩形限制在另一个矩形范围内，尽量保持其尺寸不变 */
QRectF Range::constrainRect(const QRectF &rect, const QRectF &boundingRect)
{
    Range rangeH(rect, Qt::Horizontal);
    Range rangeV(rect, Qt::Vertical);
    const Range boundingH(boundingRect, Qt::Horizontal);
    const Range boundingV(boundingRect, Qt::Vertical);
    rangeH.constrainTo(boundingH);
    rangeV.constrainTo(boundingV);
    return toRect(rangeH, rangeV);
}

Range Range::infinite()
{
    return Range( -qInf(), qInf() );
}

bool Range::fuzzyCompare(qreal v1, qreal v2)
{
//    return qFuzzyCompare(v1, v2);
    return qAbs(v1 - v2) < 1e-3;
}

QDebug operator <<(QDebug debug, const Range &range)
{
    return debug << qPrintable( QString("Range(%1,%2)").arg( range.min() ).arg( range.max() ) );
}
