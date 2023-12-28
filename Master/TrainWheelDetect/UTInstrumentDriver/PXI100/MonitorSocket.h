#ifndef MONITORSOCKET_H
#define MONITORSOCKET_H

#include "IncomingBaseSocket.h"

namespace NS_PXI100 {

/*!
 * \brief MonitorSocket 是与监视波形上传接口相关的Socket类，继承IncomingBaseSocket。
 * \inmodule PXI100
 * 关于此类中涉及的接口，请先参阅 IncomingBaseSocket 文档。
 * \sa IncomingBaseSocket
 */
class MonitorSocket : public IncomingBaseSocket
{
    Q_OBJECT

public:
    explicit MonitorSocket(ParserList parsers, QObject *parent = 0);

signals:
    void DataArrived(UTSharedDataFrame frame);

private:
    virtual void FetchData(const PacketHead &head);
};

}   /* namespace NS_PXI100 */

#endif // MONITORSOCKET_H
