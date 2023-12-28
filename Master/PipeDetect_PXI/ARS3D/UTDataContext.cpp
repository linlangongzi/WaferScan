#include "UTDataContext.h"

#ifdef ARS_ARS3D
#include "Platform/Platform.h"
#include "ConfigObject/UTInstrument.h"
#endif

UTDataContext::UTDataContext() :
    UTDataContextBase()
{
}

UTDataContext::UTDataContext(const UTDataContext &other) :
    UTDataContextBase(other)
{
}

/*!
 * \brief 检测并确保Vector尺寸不小于CONTEXT_SIZE
 */
void UTDataContext::CheckSize()
{
    if (size() < CONTEXT_SIZE)
    {
        resize(CONTEXT_SIZE);
    }
}

void UTDataContext::SetInstrumentID(int id)
{
    CheckSize();
    UTDataContextBase::operator [](CONTEXT_INSTRUMENT) = id;
}

/*!
 * \brief 获取仪器ID
 * \return 仪器ID，不保证该ID有效
 */
int UTDataContext::GetInstrumentID()
{
    Q_ASSERT(CONTEXT_INSTRUMENT >= 0);
    return qRound(value(CONTEXT_INSTRUMENT));
}

void UTDataContext::SetChannelID(int id)
{
    CheckSize();
    UTDataContextBase::operator [](CONTEXT_CHANNEL) = id;
}

int UTDataContext::GetChannelID()
{
    Q_ASSERT(CONTEXT_CHANNEL >= 0);                                             /* 确保不会越界 */
    return qRound(value(CONTEXT_CHANNEL));
}

#ifdef ARS_ARS3D
UTInstrument *UTDataContext::GetInstrument()
{
    return platform()->Instruments()->GetByID<UTInstrument>(GetInstrumentID());
}

/*!
 * \brief 获取当前数据帧的通道对象
 * \return 通道对象的指针，若当前数据帧的仪器或通道号无效，可能返回NULL
 */
UTChannel *UTDataContext::GetChannel()
{
    UTInstrument *instrument = GetInstrument();
    return instrument ? instrument->GetChannel(GetChannelID()) : NULL;
}

/*!
 * \brief 获取当前数据帧的工艺对象
 * \return 工艺对象(UTSetup)的指针，若数据指向的仪器或通道号无效，或指定的通道没有绑定工艺，
 * 则返回NULL
 */
UTSetup *UTDataContext::GetUTSetup()
{
    UTChannel *channel = GetChannel();
    return channel ? channel->GetBindUTSetup() : NULL;
}
#endif

void UTDataContext::SetDelay(float delay)
{
    CheckSize();
    UTDataContextBase::operator [](CONTEXT_DELAY) = delay;
}

float UTDataContext::GetDelay()
{
    Q_ASSERT(CONTEXT_DELAY >= 0);
    Q_ASSERT(CONTEXT_DELAY < size());
    return value(CONTEXT_DELAY);
}

void UTDataContext::SetRange(float range)
{
    CheckSize();
    UTDataContextBase::operator [](CONTEXT_RANGE) = range;
}

float UTDataContext::GetRange()
{
    Q_ASSERT(CONTEXT_RANGE >= 0);
    return value(CONTEXT_RANGE);
}
