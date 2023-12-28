#include "SocketWorkBase.h"
#include <QDebug>

SocketWorkBase::SocketWorkBase(QObject *parent, qint32 socketDescriptor, qint32 nPort) :
    ThreadWork(parent),
    m_socketDescriptor(socketDescriptor),
//    m_pSocket(0),
    m_nPort(nPort),
    m_bFinish(false)
{
    Init();
}

void SocketWorkBase::Run()
{
//    Init();
    ThreadWork::Run();
    return;
}

QTcpSocket* SocketWorkBase::GetSocket() const
{
    return m_pSocket;
}

void SocketWorkBase::OnFinish()
{
    qDebug() << "SocketWorkBase::OnFinish()";
    return;
}

void SocketWorkBase::Init()
{
    return;
}
