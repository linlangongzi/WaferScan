#pragma once

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include "ThreadController/ThreadWork.h"

class SocketWorkBase : public ThreadWork
{
    Q_OBJECT
 //   typedef QScopedPointer<QTcpSocket> scopedSocket;
 //   typedef QSharedPointer<QTcpSocket> sharedSocket;
public:
    explicit SocketWorkBase(QObject *parent = 0, qint32 socketDescriptor = 0, qint32 nPort = 2222);

protected:
    qint32 m_socketDescriptor;
    //sharedSocket m_pSocket;
    QTcpSocket* m_pSocket;
    qint32 m_nPort;
    bool m_bFinish;

public slots:
    virtual void Run();

public:
    QTcpSocket* GetSocket() const;

protected:
    virtual void Init();

public slots:
    virtual void OnFinish();
};
