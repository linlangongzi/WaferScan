#pragma once

#include "Ars1000/PipeIncomingBaseSocket.h"

namespace PipeDetect {

/*!
 * \brief PipeAScanSocket 是与A扫描全波形上传接口相关的Socket类，继承PipeIncomingBaseSocket。
 * 关于此类中涉及的接口，请先参阅 PipeIncomingBaseSocket 文档。
 * \sa PipeIncomingBaseSocket
 */
class PipeAScanSocket : public PipeIncomingBaseSocket
{
    Q_OBJECT

public:
    explicit PipeAScanSocket(ParserList parsers, QObject *parent = nullptr);

signals:
    void dataArrived(UTDataSegment frame);

private:
    virtual void FetchData(const PacketHead &head);
    virtual bool IsInvalidPacket(const PacketHead &head);
};

}                                                                               /* namespace PipeDetect */
