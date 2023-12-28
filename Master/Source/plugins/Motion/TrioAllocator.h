#pragma once

#include "TrioDriver.h"
#include <QMap>

namespace Motion {

class TrioDriver;

class TrioAllocator : public QObject
{
    Q_OBJECT

public:
    explicit TrioAllocator(int totalSize, TrioDriver *parent = nullptr);
    int Alloc(int size);
    int Free(int addr);

private:
    void AddFreeEntry(int offset, int size);
    void RemoveFreeEntry(int offset, int size);

private:
    using MemMap = QMap<int, int>;
    using MemMapReversed = QMultiMap<int, int>;
    MemMap freeMap;
    MemMapReversed freeMapSize;
    MemMap allocMap;
};

}   /* namespace Motion */
