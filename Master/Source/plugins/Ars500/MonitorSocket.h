#pragma once

#include "IncomingBaseSocket.h"

namespace Ars500 {

/*!
 * \class MonitorSocket
 * \brief ARS500 仪器接收监视波形上传的实现，继承 IncomingBaseSocket
 * \sa IncomingBaseSocket
 */
class MonitorSocket : public IncomingBaseSocket
{
    Q_OBJECT

public:
    explicit MonitorSocket(ParserList parsers, QObject *parent = nullptr);

signals:
    void dataArrived(UTDataFrame frame);

private:
    virtual void FetchData(const PacketHead &head);
};

}                                                                               /* namespace ARS500 */
