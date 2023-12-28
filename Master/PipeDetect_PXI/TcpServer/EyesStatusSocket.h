#ifndef EYESSTATUSSOCKET_H
#define EYESSTATUSSOCKET_H

#include <QTcpSocket>
#include "TcpServer/DataDef.h"

namespace NS_PXI100 {

class EyesStatusSocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit EyesStatusSocket(QObject *parent = 0);
    void SendData(SIMULATION_DATA_t data);
    void FetchData(const PacketHead &head);

signals:
    void EyesReadFlagChanged(bool isOpen);

public slots:
    void OnDataArrived(SIMULATION_DATA_t data);

private slots:
    void OnReceiveData();
    void OnDisconnected();

private:
    void Connect();
    void Disconnect();

};

}       // namespace NS_PXI100

#endif  // EYESSTATUSSOCKET_H
