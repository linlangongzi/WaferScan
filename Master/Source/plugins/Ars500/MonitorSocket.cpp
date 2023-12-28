#include "MonitorSocket.h"

using namespace Ars500;

MonitorSocket::MonitorSocket(ParserList parsers, QObject *parent) :
    IncomingBaseSocket(parsers, parent)
{
    Q_ASSERT(parent);
    packetLength = BYTES_PACKET_MONITOR;
    packetBodyLength = BYTES_PACKET_BODY_MONITOR;
    packetCommand = CMD_ASCAN_RET_WAVE;
    requestCmd = CMD_ASCAN;
    requestChannel = 0;
    maskDataLevel = IInstrumentDriver::MASK_MONITOR;
}

void MonitorSocket::FetchData(const PacketHead &head)
{
    UTDataFrame frame;
    frame.Context.SetChannelID(head.nChannelIndex);
    ReadADataFrame(frame);
    ReadCDataFrame(frame, head.nChannelIndex);
    Q_ASSERT(head.nChannelIndex < NUM_CHANNELS && head.nChannelIndex >= 0);
    if (disabled[head.nChannelIndex])                                           // 如果为true,说明datalevel改变使得抛弃此数据
    {
        return;
    }
    emit dataArrived(frame);
//    qDebug() << "Gate1SyncTime" << frame.CData[0] << "Gate2MPTime" << frame.CData[3];
}
