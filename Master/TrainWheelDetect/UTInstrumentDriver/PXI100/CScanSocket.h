#ifndef CSCANSOCKET_H
#define CSCANSOCKET_H

#include "IncomingBaseSocket.h"

namespace NS_PXI100 {

/*!
 * \brief CScanSocket 是与C扫描上传接口相关的Socket类，继承IncomingBaseSocket。
 * \inmodule PXI100
 * 关于此类中涉及的接口，请先参阅 IncomingBaseSocket 文档。
 * \sa IncomingBaseSocket
 */
class CScanSocket : public IncomingBaseSocket
{
    Q_OBJECT

public:
    explicit CScanSocket(ParserList parsers, QObject *parent = 0);

private:
    int nPerFrameSize;

signals:
    void DataArrived(UTSharedDataSegment frame);

private:
    virtual void FetchData(const PacketHead &head);
};

}   /* namespace NS_PXI100 */

#endif // CSCANSOCKET_H
