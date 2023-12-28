#pragma once

#include <Ultrasonic/Benchmark.h>
#include <Ultrasonic/IInstrumentDriver.h>

#include <QTcpSocket>

class Ars1000 : public IInstrumentDriver
{
    Q_OBJECT

public:
    enum LinkName {
        LinkMonitor,
        LinkAquisition1,
        LinkAquisition2,
        LinkParameter,
    };

    enum LinkState {
        Uninitialized,

        Normal,
    };

    explicit Ars1000(QObject *parent = 0);

private slots:
    void onConnected();
    void onDataArrived();

private:
    void sendRawPacket(int command, LinkName link, const QByteArray &data);
    void consumePacket(int command, const QByteArray &data);

private:
    struct Link {
        QTcpSocket *socket = NULL;
        QDataStream *stream = NULL;
        LinkState state = Uninitialized;
    };

    QList<Link> links;
};
