#include <Ultrasonic/IInstrumentDriver.h>
#include <Motion/MotionManager.h>
#include "Ars1000/PipeCScanSocket.h"

using namespace PipeDetect;

PipeCScanSocket::PipeCScanSocket(ParserList parsers, QObject *parent) :
    PipeIncomingBaseSocket(parsers, parent)
{
    Q_ASSERT(parent);
    packetBodyLength = sizeof(FEATURE_DATA_S);              //
    packetLength = packetBodyLength + sizeof(PacketHead);   //BYTES_PACKET_CSCAN;
    packetCommand = CMD_CSCAN_DATA;
    requestCmd = CMD_ASCAN;
    requestChannel = 1;
    maskDataLevel = IInstrumentDriver::DataLevel::MASK_CSCAN;
}

void PipeCScanSocket::ReadFeatures(QVector<UTDataSegment> &segments, const ChannelFeature &features, int channel, const UTPose &pose) const
{
    if (uint(channel) >= uint(parsers.count()) || uint(channel) >= uint(segments.count())) {
        return;
    }
    if (features.feature[0].time == 0 && features.feature[3].time == 0) {
        return;
    }
    UTDataFrame frame = UTDataFrame();
    frame.Pose = pose;

    const quint16 midValue = parsers[channel]->data.stWaveConfigure[0].MiddleValue;
    const qreal scale = qreal(100) / midValue;
    for (auto feature : features.feature) {
        frame.CData << feature.time * 0.01;
        quint16 ampTemp = feature.amp;
        ampTemp = qMin(ampTemp, midValue);
        frame.CData << ampTemp * scale;
    }

    frame.Context.SetChannelID(channel);

    segments[channel] << frame;
}

void PipeCScanSocket::FetchData(const PacketHead &head)
{
    Q_UNUSED(head);
    QVector<UTDataSegment> segments(30);

    for (int index = 0; index < C_SCAN_NUM_FRAMES_PER_PACKET; ++index) {
        FEATURE_DATA data;
        read(reinterpret_cast<char *>(&data), sizeof(FEATURE_DATA));       /* 此处使用预先算好的包长度，避免重复运算 */

        qint32 coders[2] = {data.coderX, data.coderR};
        UTPose pose = MotionManager::fillPose(coders, 2, {}, nullptr);

        ReadFeatures(segments, data.featureData.flaw1, data.wCh1ProbeNo, pose);
        ReadFeatures(segments, data.featureData.flaw2, data.wCh2ProbeNo, pose);
        ReadFeatures(segments, data.featureData.flaw3, data.wCh3ProbeNo, pose);
        ReadFeatures(segments, data.featureData.flaw4, data.wCh4ProbeNo, pose);
        ReadFeatures(segments, data.featureData.flaw5, data.wCh5ProbeNo, pose);
        ReadFeatures(segments, data.featureData.flaw6, data.wCh6ProbeNo, pose);
        ReadFeatures(segments, data.featureData.surface7, data.wCh7ProbeNo + 5, pose);
        ReadFeatures(segments, data.featureData.flaw7, data.wCh7ProbeNo, pose);
        ReadFeatures(segments, data.featureData.surface8, data.wCh8ProbeNo + 5, pose);
        ReadFeatures(segments, data.featureData.flaw8, data.wCh8ProbeNo, pose);
    }

    for (const auto &segment: segments) {
        if (!segment.isEmpty()) {
            emit dataArrived(segment);
        }
    }
}

bool PipeCScanSocket::IsInvalidPacket(const PacketHead &head)
{
    return (head.lCommand != packetCommand) || (head.lSize != BYTES_PACKET_BODY_CSCAN);
}
