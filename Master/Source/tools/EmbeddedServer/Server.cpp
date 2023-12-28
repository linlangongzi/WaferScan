#include "Server.h"
#include "Client.h"
#include <QLoggingCategory>
#include <QTcpSocket>
#include <QTimerEvent>
#include <../../app/app_version.h>

static int serverPort = 6666;

static Q_LOGGING_CATEGORY(debug, "server");

static Server *instance = nullptr;

Server::Server(QObject *parent) :
    QObject(parent),
    server(new QTcpServer(this))
{
    instance = this;
    devices = Device::enumerate();
    for (Device *dev: devices) {
        dev->setParent(this);
    }
    qCDebug(debug) << "Devices found:" << devices.count();
    reportTimer = startTimer(1000);
    dataTimer = startTimer(0);
}

Server::~Server()
{
    qCDebug(debug) << "Stopping server";
    instance = nullptr;
}

Device *Server::device(int index)
{
    return instance->devices.value(index);
}

int Server::deviceCount()
{
    return instance->devices.count();
}

bool Server::start()
{
    if (!server->listen(QHostAddress::Any, serverPort)) {
        qCCritical(debug) << "Failed listening on socket with error" << server->errorString();
        return false;
    }

    connect(server, &QTcpServer::newConnection, this, &Server::onNewConnection);
    qCDebug(debug) << "Server listening on" << serverPort;
    return true;
}

void Server::onNewConnection()
{
    while (server->hasPendingConnections()) {
        QTcpSocket *socket = server->nextPendingConnection();
        if (!socket) {
            qCCritical(debug) << "Unexpected null incoming socket";
            continue;
        }
        connect(socket, &QTcpSocket::disconnected, this, &Server::onSocketDisconnected);
        qCDebug(debug) << "Incoming connection from" << socket->peerAddress();
        if (client) {
            socket->write("ERR_BUSY ");
            socket->write(qPrintable(client->socket->peerAddress().toString()));
            socket->disconnectFromHost();
        } else {
            qCDebug(debug) << "Creating client for socket" << socket->socketDescriptor();
            client = new Client(socket, this);
            connect(socket, &QTcpSocket::readyRead, client, &Client::onSocketReadyRead);
            for (Device *dev: devices) {
                dev->reset();
            }
        }
        qCDebug(debug, "We have %d clients now", Client::engines.size());
    }
}

void Server::onSocketDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket) {
        qCDebug(debug) << "Client" << socket->peerAddress().toString() << "disconnected";
        if (client->socket == socket) {
            qCDebug(debug) << "Removing client";
            client = nullptr;
        }
        socket->deleteLater();
    }
}

void Server::timerEvent(QTimerEvent *event)
{
    if (client) {
        if (event->timerId() == reportTimer) {
            client->report();
        } else {
            for (Device *dev: devices) {
                volatile quint16 *led = reinterpret_cast<quint16 *>(dev->base + 0x50);
                *led = !(*led);
                for (int channel = 0; channel < 2; ++channel) {
                    for (int level = 0; level < 3; ++level) {
                        dev->checkLevel(channel, level);
                    }
                }
            }
        }
    }
}
