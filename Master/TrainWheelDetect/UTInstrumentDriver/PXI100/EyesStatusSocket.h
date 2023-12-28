#ifndef EYESSTATUSSOCKET_H
#define EYESSTATUSSOCKET_H

#include <QObject>
#include "IncomingBaseSocket.h"

namespace NS_PXI100 {

class EyesStatusSocket : public IncomingBaseSocket
{
    Q_OBJECT

public:
    explicit EyesStatusSocket(ParserList parsers, QObject *parent = 0);

signals:
    void sendData(const QByteArray &data);
    void DataArrived(UTSharedDataSimulation simulation);
    void mfcDataArrived(QByteArray mfcData);

public slots:
    void SendData(const QByteArray &data);
    void SetReqEyesStatusMark(bool isOpen);

private:
    virtual void FetchData(const PacketHead &head);

};

}/* namespace NS_PXI100 */

#endif // EYESSTATUSSOCKET_H
