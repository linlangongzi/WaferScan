#include "TrioAllocator.h"

using namespace Motion;

TrioAllocator::TrioAllocator(int totalSize, TrioDriver *parent) :
    QObject(parent)
{
    AddFreeEntry(0, totalSize);
}

int TrioAllocator::Alloc(int size)
{
    Q_ASSERT(size > 0);
    if (size <= 0) {
        return -1;
    }
    /* Find next suitable memort chunk */
    MemMap::iterator nextFree = freeMapSize.lowerBound(size);
    if (nextFree == freeMapSize.end()) {
        return -1;
    }

    /* Fetch chunk offset and size, since the iterator will be removed soon */
    int availableSize = nextFree.key();
    int offset = nextFree.value();
    Q_ASSERT(availableSize >= size);
    Q_ASSERT(!allocMap.contains(offset));

    /* Update free memory table */
    RemoveFreeEntry(offset, availableSize);
    if (availableSize > size) {
        AddFreeEntry(offset + size, availableSize - size);
    }

    /* Update allocated memory table */
    allocMap.insert(offset, size);
    return offset;
}

int TrioAllocator::Free(int addr)
{
    /* Sanity check, and fetch size of chunk */
    if (!allocMap.contains(addr)) {
        return addr;
    }
    int size = allocMap.take(addr);
    int end = addr + size;

    /* Find next free chunk */
    MemMap::iterator nextFree = freeMap.lowerBound(addr);
    /* Check if this chunk could be merged with previous free chunk */
    if (nextFree != freeMap.begin()) {
        MemMap::iterator prevFree = nextFree - 1;
        int prevAddr = prevFree.key();
        int prevSize = prevFree.value();
        Q_ASSERT(prevAddr + prevSize <= addr);
        if (prevAddr + prevSize == addr) {
            RemoveFreeEntry(prevAddr, prevSize);
            size += prevSize;
            addr = prevAddr;
        }
    }

    /* Check if this chunk could be merged with next free chunk */
    /* This operation will make iterator "nextFree" invalid */
    if (nextFree != freeMap.end()) {
        int nextAddr = nextFree.key();
        int nextSize = nextFree.value();
        Q_ASSERT(nextAddr >= end);
        if (nextAddr == end) {
            RemoveFreeEntry(nextAddr, nextSize);
            size += nextSize;
        }
    }

    AddFreeEntry(addr, size);
    return -1;
}

void TrioAllocator::AddFreeEntry(int offset, int size)
{
    freeMap.insert(offset, size);
    freeMapSize.insertMulti(size, offset);
}

void TrioAllocator::RemoveFreeEntry(int offset, int size)
{
    freeMap.remove(offset);
    freeMapSize.remove(size, offset);
}
