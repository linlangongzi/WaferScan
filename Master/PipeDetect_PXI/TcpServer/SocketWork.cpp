#include "DataDef.h"
#include "SocketWork.h"
#include "CScanSocket.h"
#include "MonitorSocket.h"
#include "ParameterSocket.h"
#include "Platform/Platform.h"
#include <QDebug>

using namespace NS_PXI100;

SocketWork::SocketWork(QObject *parent, qint32 socketDescriptor, qint32 nPort) :
    SocketWorkBase(parent, socketDescriptor, nPort)
{
    if (platform()->GetAecWork().data()->value(0).data()) {
        connect(this, SIGNAL(ConnectionChanged(bool)), platform()->GetAecWork().data()->value(0).data(), SLOT(OnConnectionChanged(bool)));
    }
    Init();
}

void SocketWork::Init()
{
    qDebug() << "SocketWork::Init()" << "prot:" << m_nPort;

    if(m_nPort == MONITOR_PORT) {
        CheckSender();
        m_pSocket = new MonitorSocket(this);
    }
    else if(m_nPort == CSCAN_PORT) {
        CheckSender();
        m_pSocket = new CScanSocket(this);
    }
    else if(m_nPort == PARAM_PORT) {
        CheckSender();
        m_pSocket = new ParameterSocket(this);
    }
    else {
        CheckSender();
        m_pSocket = new QTcpSocket(this);
    }

    if (!m_pSocket->setSocketDescriptor(m_socketDescriptor)) {
        qDebug() << "SocketTask：setSocketDescriptor error:"<< m_pSocket->error();
    }

    return;
}

void SocketWork::CheckSender()
{
    if (!hasSend) {
        emit ConnectionChanged(true);
        hasSend = true;
    }
}


void SocketWork::OnFinish()
{
    qDebug() << "SocketWork::OnFinish()" << "prot:" << m_nPort;
    hasSend = false;
    return;
}
