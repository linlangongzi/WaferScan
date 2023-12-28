#include "Ars1000/PipeMonitorSocket.h"

using namespace PipeDetect;

PipeMonitorSocket::PipeMonitorSocket(ParserList parsers, QObject *parent) :
    PipeIncomingBaseSocket(parsers, parent)
{
    Q_ASSERT(parent);
    packetLength = BYTES_PACKET_MONITOR;
    packetBodyLength = BYTES_PACKET_BODY_MONITOR;
    packetCommand = CMD_ASCAN_RET_WAVE;
    requestCmd = CMD_ASCAN;
    requestChannel = 0;
    maskDataLevel = IInstrumentDriver::DataLevel::MASK_MONITOR;
}

void PipeMonitorSocket::FetchData(const PacketHead &head)
{
    qint16 channelIndex = head.nChannelIndex;
    UTDataFrame frame;

    ReadADataFrame(frame, channelIndex);
    ReadCDataFrame(frame, channelIndex, FeatureDataType::FEATURE_ADATA);
    frame.Context.SetChannelID(channelIndex);

    if (channelIndex < NUM_CHANNELS && channelIndex >= 0) {
        emit dataArrived(frame);
    }
}
