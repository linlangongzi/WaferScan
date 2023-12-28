#pragma once

#include "Ars1000/PipeIncomingBaseSocket.h"

namespace PipeDetect {

/*!
 * \brief PipeCScanSocket 是与C扫描上传接口相关的Socket类，继承PipeIncomingBaseSocket。
 * 关于此类中涉及的接口，请先参阅 PipeIncomingBaseSocket 文档。
 * \sa PipeIncomingBaseSocket
 */
class PipeCScanSocket : public PipeIncomingBaseSocket
{
    Q_OBJECT

public:
    explicit PipeCScanSocket(ParserList parsers, QObject *parent = nullptr);

    void ReadFeatures(QVector<UTDataSegment> &segments, const ChannelFeature &features, int channel, const UTPose &pose) const;

signals:
    void dataArrived(UTDataSegment frame);

private:
    virtual void FetchData(const PacketHead &head);
    virtual bool IsInvalidPacket(const PacketHead &head);
};

}                                                                               /* namespace PipeDetect */
