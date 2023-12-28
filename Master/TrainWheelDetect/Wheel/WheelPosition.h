#ifndef WHEELPOSITION_H
#define WHEELPOSITION_H

#include "WheelDataset/WheelDataset.h"

class WheelPosition
{
public:
    bool isValid(int position) const;
    bool add(int time, int position);
    void addConfidence(int num);

    int confidence = 0;                                                         // 评分,用于判定轮消失或发现新轮
    bool isReal = false;                                                        // 被评定为真实的车轮
    bool hasFinished = false;                                                   // 轮消失标志
    qreal currentPosition = 0;                                                  // 移动平均得出的当前物理位置
    MovingAverage window;                                                       // 移动平均基础数据表
    QMap<int, qreal> positions;                                                   // <timestamp, position>
};

#endif // WHEELPOSITION_H
