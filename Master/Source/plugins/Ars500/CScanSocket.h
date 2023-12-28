#pragma once

#include "IncomingBaseSocket.h"

namespace Ars500 {

/*!
 * \brief CScanSocket 是与C扫描上传接口相关的Socket类，继承IncomingBaseSocket。
 * 关于此类中涉及的接口，请先参阅 IncomingBaseSocket 文档。
 * \sa IncomingBaseSocket
 */
class CScanSocket : public IncomingBaseSocket
{
    Q_OBJECT

public:
    explicit CScanSocket(ParserList parsers, QObject *parent = nullptr);

signals:
    void dataArrived(UTDataSegment frame);

private:
    virtual void timerEvent(QTimerEvent *);
    virtual void FetchData(const PacketHead &head);
    int segmentCount[2];
    int frameCount[2];
};

}                                                                               /* namespace ARS500 */
