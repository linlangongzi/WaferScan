#include "Ars1000/PipeAScanSocket.h"

using namespace PipeDetect;

PipeAScanSocket::PipeAScanSocket(ParserList parsers, QObject *parent) :
    PipeIncomingBaseSocket(parsers, parent)
{
    Q_ASSERT(parent);
    packetLength = BYTES_PACKET_ASCAN;
    packetBodyLength = BYTES_PACKET_BODY_ASCAN;
    packetCommand = CMD_ASCAN_RET_DATA_STORE;
    requestCmd = CMD_ASCAN_DATA_STORE;
    requestChannel = 0;
    maskDataLevel = IInstrumentDriver::DataLevel::MASK_ASCAN;
}

/*!
 * \brief 重载的回调函数，从缓冲区中读取一个A扫描数据包体，将其存入一个共享数据片段，并抛出信号
 * \param head 数据包头
 */
void PipeAScanSocket::FetchData(const PacketHead &head)
{
    UTDataSegment segment;
    segment.reserve(A_SCAN_NUM_FRAMES_PER_PACKET);

    for (int index = A_SCAN_NUM_FRAMES_PER_PACKET; index; --index)
    {
        UTDataFrame frame = UTDataFrame();
        ReadADataFrame(frame, head.nChannelIndex);
        ReadCDataFrame(frame, head.nChannelIndex, FeatureDataType::FEATURE_CDATA);
        frame.Context.SetChannelID(head.nChannelIndex);
        segment.append(frame);
    }
    Q_ASSERT(segment.size() == A_SCAN_NUM_FRAMES_PER_PACKET);
    if (disabled)   // 如果为true,说明datalevel改变使得抛弃此数据
    {
        return;
    }
    emit dataArrived(segment);
    //    qDebug() << segment->at(0)->Pose[0].raw << segment->at(0)->Pose[1].raw << segment->at(0)->Pose[2].raw << segment->at(0)->Pose[3].raw;
}

bool PipeAScanSocket::IsInvalidPacket(const PacketHead &head)
{
    return (head.lCommand != packetCommand) || (head.lSize != BYTES_PACKET_BODY_ASCAN && head.lSize != BYTES_PACKET_BODY_ASCAN_COMPRESSED);
}
