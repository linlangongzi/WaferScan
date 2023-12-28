#include "TcpServer.h"
#include <QDebug>
#include "SocketWork.h"
#include "ThreadController/ThreadController.h"

TcpServer::TcpServer(QObject *parent, qint32 nPort, qint32 nMax) :
    TcpServerBase(parent, nPort, nMax)
{
    Init();
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << "TcpServer incomingConnection";
    SocketWork* socketWork = new SocketWork(0, socketDescriptor, m_nPort);
    sharedSocketWork pWork = sharedSocketWork(socketWork);
    ThreadController* threadController = new ThreadController(this, pWork.objectCast<ThreadWork>());
    connect(socketWork->GetSocket(), SIGNAL(disconnected()), threadController, SLOT(Quit()));
}
