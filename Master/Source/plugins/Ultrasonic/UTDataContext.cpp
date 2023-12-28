#include "UTDataContext.h"

UTDataContext::UTDataContext(const UTDataContext &other) :
    UTDataContextBase(other)
{}

/*!
 * \brief 检测并确保Vector尺寸不小于CONTEXT_SIZE
 */
void UTDataContext::CheckSize()
{
    if (size() < CONTEXT_SIZE) {
        resize(CONTEXT_SIZE);
        squeeze();
    }
}

void UTDataContext::SetChannelID(int id)
{
    CheckSize();
    UTDataContextBase::operator [](CONTEXT_CHANNEL) = id;
}

int UTDataContext::GetChannelID() const
{
    Q_ASSERT(CONTEXT_CHANNEL >= 0);                                             /* 确保不会越界 */
    return qRound( value(CONTEXT_CHANNEL) );
}

union floatIntConvert                                                           /* 在int和float之间进行安全的位转换的集合 */
{
    int i;                                                                      /* reinterpret_cast在某些编译器上会导致strict-aliasing警告 */
    UTDataContext::Direction dir;
    UTDataContextData f;
};

void UTDataContext::SetDirections(Direction directions)                         /* 方向信息按位存储 */
{
    CheckSize();
    floatIntConvert c;
    c.dir = directions;
    UTDataContextBase::operator [](CONTEXT_DIRECTION) = c.f;
}

UTDataContext::Direction UTDataContext::GetDirections() const
{
    floatIntConvert c;
    c.f = value(CONTEXT_DIRECTION);
    return c.dir;
}

bool UTDataContext::IsReverse(int axis) const
{
    return GetDirections() & (1 << axis);
}

void UTDataContext::setDelay(float delay)
{
    CheckSize();
    UTDataContextBase::operator [](CONTEXT_DELAY) = delay;
}

float UTDataContext::GetDelay() const
{
    Q_ASSERT(CONTEXT_DELAY >= 0);
    return value(CONTEXT_DELAY);
}

void UTDataContext::SetRange(float range)
{
    CheckSize();
    UTDataContextBase::operator [](CONTEXT_RANGE) = range;
}

float UTDataContext::GetRange() const
{
    Q_ASSERT(CONTEXT_RANGE >= 0);
    return value(CONTEXT_RANGE);
}
