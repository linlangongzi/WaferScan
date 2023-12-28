#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include "TcpServer/TcpServerBase.h"

class TcpServer : public TcpServerBase
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0, qint32 nPort = 3333, qint32 nMax = 10);

protected:
    virtual void incomingConnection(qintptr socketDescriptor);
};

#endif // TCPSERVER_H
