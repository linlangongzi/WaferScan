#pragma once

#include "MotionGlobal.h"
#include <QBitArray>
#include <QMetaType>
#include <QDebug>

class QJsonValue;

class MOTION_EXPORT MotionAxisList
{
    Q_GADGET

public:
    inline MotionAxisList() = default;
    inline MotionAxisList(const QBitArray &bits) : data(bits) {}
    MotionAxisList(const std::initializer_list<int> &init) : MotionAxisList(QList<int>(init)) {}
    MotionAxisList(const QList<int> &axisList);
    MotionAxisList(const QSet<int> &axisList);

    inline bool hasAxis(int axis) const;
    inline void setAxis(int axis);
    inline void setAxis(int axis, bool enabled);
    inline bool isEmpty() const;
    inline int axisCount() const;
    inline int count() const;
    inline void clear();
    void append(bool enabled);
    int first() const;
    int last() const;
    void combine(const MotionAxisList &other);
    void intersect(const MotionAxisList &other);

    QList<int> toList() const;
    QSet<int> toSet() const;
    int indexOf(int index) const;

    QJsonValue toJson() const;
    static MotionAxisList fromJson(const QJsonValue &json);

    inline bool operator ==(const MotionAxisList &other) const {return data == other.data;}
    inline bool operator !=(const MotionAxisList &other) const {return data != other.data;}
    inline operator QBitArray() const {return data;}

private:
    inline void ensureSize(int size);
    QBitArray data;
};

MOTION_EXPORT QDebug operator <<(QDebug debug, const MotionAxisList list);

bool MotionAxisList::hasAxis(int axis) const
{
    if (uint(axis) < uint(data.size())) {
        return data.testBit(axis);
    }
    return false;
}

void MotionAxisList::setAxis(int axis)
{
    if (axis >= 0) {
        ensureSize(axis + 1);
        data.setBit(axis);
    }
}

void MotionAxisList::setAxis(int axis, bool enabled)
{
    if (axis >= 0) {
        ensureSize(axis + 1);
        data.setBit(axis, enabled);
    }
}

bool MotionAxisList::isEmpty() const
{
    return data.isEmpty();
}

int MotionAxisList::axisCount() const
{
    return data.count(true);
}

int MotionAxisList::count() const
{
    return data.count();
}

void MotionAxisList::clear()
{
    data.clear();
}

void MotionAxisList::ensureSize(int size)
{
    if (size > data.size()) {
        data.resize(size);
    }
}
