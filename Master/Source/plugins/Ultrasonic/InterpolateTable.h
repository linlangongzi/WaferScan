#pragma once

#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QMetaType>
#include "UltrasonicGlobal.h"

class ULTRASONIC_EXPORT InterpolateTable : public QMap<double, double>
{
    Q_GADGET

public:
    double linearInterpolate(double key) const;                                 /* 线性插值 */
    void removeInRange(double min, double max);                                 /* 移除区间内的插值表 */
    int countInRange(double min, double max) const;                             /* 查找范围内有多少数据 */
    QJsonValue toJson() const;
    static InterpolateTable fromJson(const QJsonValue &json);
};
