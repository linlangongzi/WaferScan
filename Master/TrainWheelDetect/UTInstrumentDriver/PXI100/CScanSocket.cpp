#include "CScanSocket.h"

using namespace NS_PXI100;

CScanSocket::CScanSocket(ParserList parsers, QObject *parent) :
    IncomingBaseSocket(parsers, parent)
{
    Q_ASSERT(parent);
    nPerFrameSize = sizeof(FEATURE_DATA);
    packetCommand = CMD_ASCAN_RET_WAVE;
    requestCmd = CMD_ASCAN;
    requestChannel = 1;
    maskDataLevel = IInstrumentDriver::DataLevel::MASK_CSCAN;
}

void CScanSocket::FetchData(const PacketHead &head)
{
    UTSharedDataSegment segment;
    int nFrames = head.lSize / nPerFrameSize;
    segment.reserve(nFrames);

    for (int index = nFrames; index; --index) {
        UTSharedDataFrame frame = UTSharedDataFrame::create();

        ReadCDataFrame(frame, head.nChannelIndex, FeatureDataType::FEATURE_CDATA);
        frame->Context.SetChannelID(head.nChannelIndex);
        segment.append(frame);
    }
    Q_ASSERT(segment.size() == nFrames);
    if (disabled) {  // 如果为true,说明datalevel改变使得抛弃此数据
        return;
    }
    emit DataArrived(segment);
}
