#pragma once

#include "Ars1000/PipeIncomingBaseSocket.h"

namespace PipeDetect {

/*!
 * \class PipeMonitorSocket
 * \brief PipeDetect 仪器接收监视波形上传的实现，继承 PipeIncomingBaseSocket
 * \sa PipeIncomingBaseSocket
 */
class PipeMonitorSocket : public PipeIncomingBaseSocket
{
    Q_OBJECT

public:
    explicit PipeMonitorSocket(ParserList parsers, QObject *parent = nullptr);

signals:
    void dataArrived(UTDataFrame frame);

private:
    virtual void FetchData(const PacketHead &head);
};

}                                                                               /* namespace PipeDetect */
