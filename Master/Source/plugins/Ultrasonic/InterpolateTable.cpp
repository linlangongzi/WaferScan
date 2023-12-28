#include "InterpolateTable.h"
#include <QDebug>
#include <allrising/Util.h>

REGISTER_JSON_CONVERTER(InterpolateTable)

// 线性插值
double InterpolateTable::linearInterpolate(double key) const
{
    if (isEmpty()) {
        return 0;
    }
    const_iterator it = lowerBound(key);
    if (qFuzzyCompare(it.key(), key) || it == begin()) {                    // 在点上, 或在起始位置之前
        return it.value();
    } else if (it == end()) {                                                 // 没有找到
        return last();
    } else {                                                                    // 在两个点之间
        const_iterator before = it - 1;
        return (it.value() - before.value()) / (it.key() - before.key()) * (key - before.key()) + before.value();
    }
}

// 移除范围内的插值表
void InterpolateTable::removeInRange(double min, double max)
{
    for (iterator start = lowerBound(min); start != end() && start.key() < max; start = lowerBound(min)) {
        remove(start.key());
    }
}

int InterpolateTable::countInRange(double min, double max) const
{
    int count = 0;
    for (auto start = lowerBound(min); start != cend() && start.key() < max; ++start) {
        count++;
    }
    return count;
}

QJsonValue InterpolateTable::toJson() const
{
    QJsonArray array;
    for (auto iter = cbegin(); iter != cend(); ++iter) {
        QJsonObject obj;
        obj["Time"] = iter.key();
        obj["Gain"] = iter.value();
        array.append(obj);
    }
    return array;
}

InterpolateTable InterpolateTable::fromJson(const QJsonValue &json)
{
    InterpolateTable t;
    const QJsonArray array = json.toArray();
    for (const QJsonValue &value: array) {
        QJsonObject obj = value.toObject();
        if (obj.isEmpty()) {
            continue;
        }
        t.insert(obj["Time"].toDouble(), obj["Gain"].toDouble());
    }
    return t;
}
