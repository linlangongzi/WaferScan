#include "CardRunningWork.h"
#include <QThread>
#include <QDebug>

CardRunningWork::CardRunningWork(QObject *parent, int nSleep) :
    ThreadWork(parent),
    m_nSleep(nSleep)
{
    ConnectTo();
}

void CardRunningWork::ConnectTo()
{
    return;
}

void CardRunningWork::Run()
{
    while(!m_bFinish)
    {
        emit Runing();
        QThread::msleep(m_nSleep);
    }

    return;
}

void CardRunningWork::OnFinish()
{
    qDebug() << "CardRunningWork::OnFinish()";
    ThreadWork::OnFinish();
    return;
}
