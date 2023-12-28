#pragma once

#include "IncomingBaseSocket.h"

namespace Ars500 {

/*!
 * \brief AScanSocket 是与A扫描全波形上传接口相关的Socket类，继承IncomingBaseSocket。
 * 关于此类中涉及的接口，请先参阅 IncomingBaseSocket 文档。
 * \sa IncomingBaseSocket
 */
class AScanSocket : public IncomingBaseSocket
{
    Q_OBJECT

public:
    explicit AScanSocket(ParserList parsers, QObject *parent = nullptr);

signals:
    void dataArrived(UTDataSegment frame);

private:
    virtual void timerEvent(QTimerEvent *);
    virtual bool IsInvalidPacket(const PacketHead &head);
    virtual void FetchData(const PacketHead &head);
    int segmentCount[2];
    int frameCount[2];
};

}                                                                               /* namespace ARS500 */
