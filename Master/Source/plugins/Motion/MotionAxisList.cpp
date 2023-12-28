#include "MotionAxisList.h"
#include <QList>
#include <QSet>
#include <QJsonArray>
#include <allrising/Util.h>

REGISTER_JSON_CONVERTER(MotionAxisList);

MotionAxisList::MotionAxisList(const QList<int> &axisList)
{
    foreach (int axis, axisList) {
        setAxis(axis);
    }
}

MotionAxisList::MotionAxisList(const QSet<int> &axisList)
{
    foreach (int axis, axisList) {
        setAxis(axis);
    }
}

void MotionAxisList::append(bool enabled)
{
    data.resize(data.count() + 1);
    data.setBit(data.count() - 1, enabled);
}

int MotionAxisList::first() const
{
    for (int index = 0; index < data.count(); ++index) {
        if (data.testBit(index)) {
            return index;
        }
    }
    return -1;
}

int MotionAxisList::last() const
{
    for (int index = data.count() - 1; index >= 0; --index) {
        if (data.testBit(index)) {
            return index;
        }
    }
    return -1;
}

QList<int> MotionAxisList::toList() const
{
    QList<int> list;
    for (int index = 0; index < data.count(); ++index) {
        if (data.testBit(index)) {
            list << index;
        }
    }
    return list;
}

QSet<int> MotionAxisList::toSet() const
{
    QSet<int> set;
    for (int index = 0; index < data.count(); ++index) {
        if (data.testBit(index)) {
            set << index;
        }
    }
    return set;
}

/*! 等价于ToList().indexOf(index) */
int MotionAxisList::indexOf(int index) const
{
    if (index >= data.size()) {
        return -1;
    }
    int i = -1;
    for (; index >= 0; --index) {
        if (data.testBit(index)) {
            ++i;
        }
    }
    return i;
}

QJsonValue MotionAxisList::toJson() const
{
    QJsonArray array;
    for (int index = 0; index < data.count(); ++index) {
        if (data.testBit(index)) {
            array << index;
        }
    }
    return array;
}

MotionAxisList MotionAxisList::fromJson(const QJsonValue &json)
{
    MotionAxisList list;
    const QJsonArray array = json.toArray();
    foreach (const QJsonValue &value, array) {
        list.setAxis(value.toInt(-1));
    }
    return list;
}

void MotionAxisList::combine(const MotionAxisList &other)
{
    for (int index = other.data.count() - 1; index >= 0; --index) {
        if (other.data.testBit(index)) {
            setAxis(index);
        }
    }
}

void MotionAxisList::intersect(const MotionAxisList &other)
{
    for (int index = qMin(data.count(), other.data.count()) - 1; index >= 0; --index) {
        if (!other.data.testBit(index)) {
            data.clearBit(index);
        }
    }
}

QDebug operator <<(QDebug debug, const MotionAxisList list)
{
    QDebugStateSaver state(debug);
    Q_UNUSED(state);
    debug.nospace() << "List(";
    debug.setAutoInsertSpaces(true);
    for (int index = 0; index < list.count(); ++index) {
        if (list.hasAxis(index)) {
            debug << index;
        }
    }
    return debug.nospace() << ")";
}
