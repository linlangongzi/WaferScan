#ifndef SOCKETWORK_H
#define SOCKETWORK_H

#include <QObject>
#include <QSharedPointer>
#include "TcpServer/SocketWorkBase.h"

class SocketWork : public SocketWorkBase
{
    Q_OBJECT

public:
    explicit SocketWork(QObject *parent = 0, qint32 socketDescriptor = 0, qint32 nPort = 2222);

signals:
    void ConnectionChanged(bool isConnect);

private:
    virtual void Init();
    void CheckSender();

public slots:
    virtual void OnFinish();

private:
    bool hasSend = false;
};

typedef QSharedPointer<SocketWork> sharedSocketWork;


#endif // SOCKETWORK_H
