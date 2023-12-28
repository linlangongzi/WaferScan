#pragma once

#include <QSharedPointer>
#include "ThreadController/ThreadWork.h"

class CardRunningWork : public ThreadWork
{
    Q_OBJECT
public:
    explicit CardRunningWork(QObject *parent = 0, int nSleep = 25);

protected:
    int m_nSleep;

protected:
    virtual void ConnectTo();

signals:
    void Runing();

private slots:
    void Run();

public slots:
    virtual void OnFinish();

};

typedef QSharedPointer<CardRunningWork> sharedCardRunningWork;
