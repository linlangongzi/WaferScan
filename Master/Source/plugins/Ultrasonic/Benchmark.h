#pragma once

#include <QObject>
#include <QQueue>
#include <QMap>
#include <QHash>
#include <QPoint>
#include <time.h>
#include <allrising/QtExtend.h>

class Benchmark : public QObject
{
    Q_OBJECT

public:
    Benchmark(QObject *parent = nullptr);

    void Add(int col, int row, qreal value);
    void SetColName(int col, const QString &name);
    int SetRowName(int row, int parent, const QString &name);
    QMap<int, QString> GetColName() const {return cols;}
    QMap<int, QString> GetRowName() const {return rows;}
    QMap<int, int> GetRowParent() const {return rowParent;}
    QString Render();
    QHash<QPoint, qreal> Snapshot();

public slots:
    void SetSplitInterval(int newSplitInterval);
    void SetRollingDepth(int newRollingDepth);

private:
    typedef QHash<QPoint, qreal> DataCol;
    struct Record {
        DataCol data;
        clock_t timestamp;
    };
    typedef QQueue<Record> Records;
    typedef QMap<int, QString> HeaderMap;

private:
    void RemoveExpired(clock_t time);

private:
    int timerId;
    Records records;
    HeaderMap cols;
    HeaderMap rows;
    QMap<int, int> rowParent;
    int splitInterval;
    int rollingDepth;
};
