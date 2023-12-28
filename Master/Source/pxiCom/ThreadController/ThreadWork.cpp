#include "ThreadWork.h"

ThreadWork::ThreadWork(QObject *parent) :
    QObject(parent),
    m_bFinish(false)
{
}

void ThreadWork::Run()
{
    return;
}

void ThreadWork::OnFinish()
{
    QMutexLocker lock(&mutex);
    m_bFinish = true;
    return;
}
