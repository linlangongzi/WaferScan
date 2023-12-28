#ifndef EYESSTATUSSOCKET_H
#define EYESSTATUSSOCKET_H

#include <QTcpSocket>
#include "TcpServer/DataDef.h"
#include "IncomingBaseSocket.h"
#include "CardWork/HardwareAddress.h"

namespace NS_PXI100 {

class EyesStatusSocket : public IncomingBaseSocket
{
    Q_OBJECT

public:
    enum eEyesSocketType
    {
        AEC = 0,
        MFC
    };

public:
    explicit EyesStatusSocket(QObject *parent = 0, eEyesSocketType type = AEC);
    void SendData(UTSharedDataSimulation simulation);
    void FetchData(const PacketHead &head);

private:
    eEyesSocketType m_type;

signals:
    void EyesReadFlagChanged(bool isOpen);

public slots:
    void OnDataArrived(UTSharedDataSimulation simulation);
    void onMfcDataArrived(QByteArray byteArray);

private slots:
    void OnDisconnected();

private:
    void Connect();
    void Disconnect();

};

}       // namespace NS_PXI100

#endif  // EYESSTATUSSOCKET_H
