#pragma once

#include <QObject>
#include <QTcpServer>

class TcpServerBase : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServerBase(QObject *parent = 0, qint32 nPort = 3333, qint32 nMax = 10);

protected:
    qint32 m_nPort;
    QString m_strIp;
    qint32 m_MaxConnect;

protected:
    virtual void incomingConnection(qintptr socketDescriptor);
    void Init();
};
