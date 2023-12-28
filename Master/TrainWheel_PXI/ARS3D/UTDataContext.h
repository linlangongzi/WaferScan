#ifndef UTDATACONTEXT_H
#define UTDATACONTEXT_H

#include <QVector>

/* 前置声明 */
class UTChannel;
class UTInstrument;
class UTSetup;

typedef float UTDataContextData;
typedef QVector<UTDataContextData> UTDataContextBase;

/*!
 * \class UTDataContext
 * \brief UTDataContext是一份超声检测数据帧的上下文记录，其中包含了该次扫描的参考信息，包括
 * 扫描所用的仪器、通道号、超声工艺延迟和声程等。
 * UTDataContext基于一个QVector，其中值的映射定义在ContextIndex中。
 * 类中提供了一系列辅助函数，用于快速访问上下文的内容，查询仪器、通道、工艺信息。
 */
struct UTDataContext : public UTDataContextBase
{
public:
    UTDataContext();
    UTDataContext(const UTDataContext &other);

    enum ContextIndex
    {
        CONTEXT_INSTRUMENT = 0,
        CONTEXT_CHANNEL,
        CONTEXT_DELAY,
        CONTEXT_RANGE,

        CONTEXT_SIZE,                                                           /* 上下文变量的数量 */
        CONTEXT_LAST_INDEX = CONTEXT_SIZE - 1
    };

    void CheckSize();

    void SetInstrumentID(int id);
    int GetInstrumentID();

    void SetChannelID(int id);
    int GetChannelID();

#ifdef ARS_ARS3D
    UTInstrument *GetInstrument();
    UTChannel *GetChannel();
    UTSetup *GetUTSetup();
#endif

    void SetDelay(float delay);
    float GetDelay();

    void SetRange(float range);
    float GetRange();
};

#endif // UTDATACONTEXT_H
