#pragma once

#include <QObject>
#include <QThread>
#include "ThreadWork.h"

class ThreadController : public QObject
{
    Q_OBJECT
public:
    explicit ThreadController(QObject *parent = 0, sharedWork work = sharedWork::create());
    ~ThreadController();

private:
    QThread m_thread;
    sharedWork m_work;

signals:
    void Finish();

public slots:
    void Quit();

};
