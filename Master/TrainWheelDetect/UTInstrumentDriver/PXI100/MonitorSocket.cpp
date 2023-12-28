#include "MonitorSocket.h"

using namespace NS_PXI100;

MonitorSocket::MonitorSocket(ParserList parsers, QObject *parent) :
    IncomingBaseSocket(parsers, parent)
{
    Q_ASSERT(parent);
    packetLength = BYTES_PACKET_MONITOR;
    packetBodyLength = BYTES_PACKET_BODY_MONITOR;
    packetCommand = CMD_ASCAN_RET_WAVE;
    requestCmd = CMD_ASCAN;
    requestChannel = 0;
    maskDataLevel = IInstrumentDriver::DataLevel::MASK_MONITOR;
}

void MonitorSocket::FetchData(const PacketHead &head)
{
    UTSharedDataFrame frame(new UTDataFrame);
    ReadADataFrame(frame);
//    int nChnIdx = dynamic_cast<UTInstrument*>(parent()->parent())->GetChannelMap()->value(head.nChannelIndex);
    int nChnIdx = head.nChannelIndex; // FIXME: 此处应使用逻辑通道号.
    ReadCDataFrame(frame, nChnIdx, FeatureDataType::FEATURE_ADATA);
    frame->Context.SetChannelID(head.nChannelIndex);
    if (disabled)   // 如果为true,说明datalevel改变使得抛弃此数据
    {
        return;
    }
    emit DataArrived(frame);
//    qDebug() << "Gate1SyncTime" << frame->CData[0] << "Gate2MPTime" << frame->CData[3];
}
