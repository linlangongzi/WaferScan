#pragma once

#include "Device.h"
#include <QObject>
#include <QTcpServer>

class Client;

class Server : public QObject
{
    Q_OBJECT

public:
    explicit Server(QObject *parent = nullptr);
    ~Server();

    static Device *device(int index);
    static int deviceCount();

signals:

public slots:
    bool start();

private slots:
    void onNewConnection();
    void onSocketDisconnected();

private:
    void timerEvent(QTimerEvent *event) override;

private:
    QTcpServer *server = nullptr;
    Client *client = nullptr;
    int reportTimer = 0;
    int dataTimer = 0;
    QList<Device *> devices;
};
