#pragma once

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
    UTDataContext() = default;
    UTDataContext(const UTDataContext &other);

    enum ContextIndex
    {
//        CONTEXT_INSTRUMENT = 0,
        CONTEXT_CHANNEL = 0,                                                    /* 仪器通道ID */
        CONTEXT_DELAY,                                                          /* 时间延迟 */
        CONTEXT_RANGE,                                                          /* 声程 */
        CONTEXT_DIRECTION,                                                      /* 轴运动方向信息 */
        ContextSyncOffset,                                                          /*      */
        CONTEXT_SIZE,                                                           /* 上下文变量的数量 */
        CONTEXT_LAST_INDEX = CONTEXT_SIZE - 1,
    };
    typedef quint32 Direction;

    void CheckSize();                                                           /* 确保QVector的尺寸不小于上下文的尺寸CONTEXT_SIZE */
    void SetChannelID(int id);                                                  /* 设置仪器通道ID */
    int GetChannelID() const;                                                   /* 获取仪器通道ID */

    void SetDirections(Direction directions);                                   /* 设置各运动轴的运动方向，方向信息按位存储 */
    Direction GetDirections() const;                                            /* 获取各运动轴的运动方向 */
    bool IsReverse(int axis) const;                                             /* 判断一个轴是否反向运动 */

    void setDelay(float delay);                                                 /* 设置时间延迟 */
    float GetDelay() const;                                                     /* 获取时间延迟 */

    void SetRange(float range);                                                 /* 设置声程 */
    float GetRange() const;                                                     /* 获取声程 */
};
