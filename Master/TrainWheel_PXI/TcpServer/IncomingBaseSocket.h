#ifndef INCOMINGBASESOCKET_H
#define INCOMINGBASESOCKET_H

#include <QTcpSocket>
#include "DataDef.h"

namespace NS_PXI100 {

class IncomingBaseSocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit IncomingBaseSocket(QObject *parent = 0);

protected:
    virtual void FetchData(const PacketHead &head) = 0;

private slots:
    void OnReceiveData();
    virtual void OnDisconnected();

protected:
    qint32 nIdentification;         // 识别码
};

}

#endif // INCOMINGBASESOCKET_H
