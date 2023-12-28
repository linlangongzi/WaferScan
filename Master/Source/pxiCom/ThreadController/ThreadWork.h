#pragma once

#include <QObject>
#include <QMutex>
#include <QSharedPointer>

class ThreadWork : public QObject
{
    Q_OBJECT
public:
    explicit ThreadWork(QObject *parent = 0);

protected:
    QMutex mutex;
    bool m_bFinish;

public:
    virtual void Run();

signals:

public slots:
    virtual void OnFinish();

};

typedef QSharedPointer<ThreadWork> sharedWork;
