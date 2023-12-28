#include "Benchmark.h"
#include <QStringList>
#include <QSet>
#include <allrising/QtExtend.h>

const QPoint timestampKey(INT_MAX, INT_MAX);

Benchmark::Benchmark(QObject *parent) :
    QObject(parent),
    splitInterval(500),
    rollingDepth(10)
{}

void Benchmark::Add(int col, int row, qreal value)
{
    clock_t time = clock();
    RemoveExpired(time);
    if (records.isEmpty() || int(time - records.last().timestamp) > splitInterval) {
        Record r;
        r.timestamp = time;
        records.append(r);
    }
    Record &r = records.last();
    r.data[QPoint(col, row)] += value;
}

void Benchmark::SetColName(int col, const QString &name)
{
    cols[col] = name;
}

int Benchmark::SetRowName(int row, int parent, const QString &name)
{
    rows[row] = name;
    if (rows.contains(parent)) {
        rowParent[row] = parent;
    }
    return row;
}

void Benchmark::SetSplitInterval(int newSplitInterval)
{
    splitInterval = newSplitInterval;
    RemoveExpired(clock());
}

void Benchmark::SetRollingDepth(int newRollingDepth)
{
    rollingDepth = newRollingDepth;
    RemoveExpired(clock());
}

void Benchmark::RemoveExpired(clock_t time)
{
    while (!records.isEmpty()
           && int(time - records.first().timestamp) > (splitInterval * rollingDepth)) {
        records.removeFirst();
    }
}

QHash<QPoint, qreal> Benchmark::Snapshot()
{
    clock_t now = clock();
    RemoveExpired(now);
    DataCol r;
//    r[timestampKey] = 0;
    if (records.isEmpty()) {
        return r;
    }
    foreach (Record record, records) {
        for (auto it = record.data.cbegin(); it != record.data.cend(); ++it) {
            r[it.key()] += it.value();
        }
    }
    r[timestampKey] = now - records.first().timestamp;
    return r;
}
