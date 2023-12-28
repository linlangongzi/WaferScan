#include "SocketWork.h"
#include <QDebug>
#include "DataDef.h"
#include "CScanSocket.h"
#include "MonitorSocket.h"
#include "ParameterSocket.h"
#include "TcpServer/EyesStatusSocket.h"
#include "pxiCom.hpp"
#include "LuaEngine.h"

void Log(const char *message, ...);

using namespace NS_PXI100;

SocketWork::SocketWork(QObject *parent, qint32 socketDescriptor, qint32 nPort) :
    SocketWorkBase(parent, socketDescriptor, nPort)
{
    Init();
}

void SocketWork::Init()
{
//   m_pSocket = new QTcpSocket(this);
//    QTcpSocket* m_pSocket;
    qDebug() << "SocketWork::Init()" << "prot:" << m_nPort;

    if(m_nPort == PARAM_PORT)
    {
        //m_pSocket = new ParameterSocket(parsers, 0);
        m_pSocket = new ParameterSocket(this);
       //m_pSocket.reset(new ParameterSocket(parsers, 0));
        //QTcpSocket* socket = new ParameterSocket(parsers, 0); // 不能为智能指针,底层由于接收到操作系统的信号而停止
        //m_pSocket = sharedSocket(socket);
    }
    else if(m_nPort == MONITOR_PORT)
    {
        //m_pSocket = new MonitorSocket(0);
        m_pSocket = new MonitorSocket(this);
        //m_pSocket.reset(new MonitorSocket(0));
        //QTcpSocket* socket = new MonitorSocket(0);
        //m_pSocket = sharedSocket(socket);
    }
    else if(m_nPort == CSCAN_PORT)
    {
        //m_pSocket = new CScanSocket(0);
        m_pSocket = new CScanSocket(this);
        //m_pSocket.reset(new CScanSocket(0));
        //QTcpSocket* socket = new CScanSocket(0);
        //m_pSocket = sharedSocket(socket);
    }
    else if(m_nPort == EYES_STATUS_PORT)
    {
        //m_pSocket = new EyesStatusSocket(0);
        m_pSocket = new EyesStatusSocket(this);
        //m_pSocket.reset(new EyesStatusSocket(0));
        //QTcpSocket* socket = new EyesStatusSocket(0);
        //m_pSocket = sharedSocket(socket);
    }
    else if(m_nPort == MFC_EYES_STATUS_PORT)
    {
        m_pSocket = new EyesStatusSocket(this, EyesStatusSocket::MFC);
    }
    else if(m_nPort == LUA_PORT)
    {
        m_pSocket = new QTcpSocket(this);
        ARS_CHECK(m_pSocket);
        new LuaEngine(m_pSocket);
        Log("We have %d clients now", LuaEngine::engines.size());
    }
    else
    {
         //m_pSocket = new QTcpSocket(0);
         m_pSocket = new QTcpSocket(this);
         //m_pSocket.reset(new QTcpSocket(0));
         //QTcpSocket* socket = new QTcpSocket(0);
         //m_pSocket = sharedSocket(socket);
    }

    if (!m_pSocket->setSocketDescriptor(m_socketDescriptor))
    {
        qDebug() << "SocketTask：setSocketDescriptor error:"<< m_pSocket->error();
    }

//    connect(m_pSocket, SIGNAL(Disconnected()), this, SLOT(OnDisconnected()));
    return;
}


void SocketWork::OnFinish()
{
    qDebug() << "SocketWork::OnFinish()" << "prot:" << m_nPort;
    //m_bFinish = TRUE;
/*    if(m_pSocket)
    {
      m_pSocket->disconnectFromHost();
      delete m_pSocket;
      m_pSocket = NULL;
    }*/
    if(m_nPort == LUA_PORT)
    {
        while (LuaEngine::engines.size() > 0) {
            delete LuaEngine::engines[0];
        }
    }
    return;
}
