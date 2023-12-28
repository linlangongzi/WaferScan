#ifndef ASCANSOCKET_H
#define ASCANSOCKET_H

#include "IncomingBaseSocket.h"

namespace NS_PXI100 {

/*!
 * \brief AScanSocket 是与A扫描全波形上传接口相关的Socket类，继承IncomingBaseSocket。
 * \inmodule PXI100
 * 关于此类中涉及的接口，请先参阅 IncomingBaseSocket 文档。
 * \sa IncomingBaseSocket
 */
class AScanSocket : public IncomingBaseSocket
{
    Q_OBJECT

public:
    explicit AScanSocket(ParserList parsers, QObject *parent = 0);

signals:
    void DataArrived(UTSharedDataSegment frame);

private:
    virtual void FetchData(const PacketHead &head);
};

}   /* namespace NS_PXI100 */

#endif // ASCANSOCKET_H
