#include "TcpServerBase.h"
#include <QDebug>

TcpServerBase::TcpServerBase(QObject *parent, qint32 nPort, qint32 nMax) :
    QTcpServer(parent),
    m_nPort(nPort),
    m_strIp("192.168.1.100"),
    m_MaxConnect(nMax)
{
    Init();
}

void TcpServerBase::incomingConnection(qintptr socketDescriptor)
{
    Q_UNUSED(socketDescriptor);
/*    qDebug() << "TcpServerBase incomingConnection";

    SocketWorkBase* socketWork = new SocketWorkBase(0, socketDescriptor, m_nPort);
    sharedSocketWork pWork = sharedSocketWork(socketWork);
    ThreadController* threadController = new ThreadController(this, pWork.objectCast<ThreadWork>());
    connect(socketWork->GetSocket(), SIGNAL(disconnected()), threadController, SLOT(Quit()));*/
    return;
}

void TcpServerBase::Init()
{
    setMaxPendingConnections(m_MaxConnect);
    if(!listen(QHostAddress::Any, m_nPort))
    {
        qDebug() << "port:"<< m_nPort << "listen failed";
    }
    else
    {
        qDebug() << "start listen port:"<< m_nPort;
    }

    return;
}
