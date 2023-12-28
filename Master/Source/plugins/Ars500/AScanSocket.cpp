#include "AScanSocket.h"
#ifdef ARS_DUMP_CSV
#   include <Core/Platform.h>
#endif

using namespace Ars500;

/*!
 * \brief 构造函数，需要 Parser 指针的列表。
 * \param parsers
 * \param parent
 */
AScanSocket::AScanSocket(ParserList parsers, QObject *parent) :
    IncomingBaseSocket(parsers, parent)
{
    Q_ASSERT(parent);
    packetLength = qMax(BYTES_PACKET_ASCAN, BYTES_PACKET_ASCAN_COMPRESSED);
    packetBodyLength =  qMax(BYTES_PACKET_BODY_ASCAN, BYTES_PACKET_BODY_ASCAN_COMPRESSED);
    packetCommand = CMD_ASCAN_RET_DATA_STORE;
    requestCmd = CMD_ASCAN_DATA_STORE;
    requestChannel = 0;
    maskDataLevel = IInstrumentDriver::MASK_ASCAN;
#ifdef ARS_BENCHMARK_INSTRUMENT
    startTimer(1000);
#endif
}

void AScanSocket::timerEvent(QTimerEvent *)
{
    qDebug() << "A1:" << segmentCount[0] << "segments" << frameCount[0] << "frames;" << "A2:" << segmentCount[1] << "segments" << frameCount[1] << "frames";
    segmentCount[0] = 0;
    frameCount[0] = 0;
    segmentCount[1] = 0;
    frameCount[1] = 0;
}

bool AScanSocket::IsInvalidPacket(const PacketHead &head)
{
    return (head.lCommand != packetCommand) || (head.lSize != BYTES_PACKET_BODY_ASCAN && head.lSize != BYTES_PACKET_BODY_ASCAN_COMPRESSED);
}

/*!
 * \brief 重载的回调函数，从缓冲区中读取一个A扫描数据包体，将其存入一个共享数据片段，并抛出信号
 * \param head 数据包头
 */
void AScanSocket::FetchData(const PacketHead &head)
{
    UTDataSegment segment;
    if (head.lSize == BYTES_PACKET_BODY_ASCAN_COMPRESSED)                   /* 新版本的仪器数据进行了压缩*/

    {
        segment.reserve(A_SCAN_NUM_FRAMES_PER_PACKET_COMPRESSED);
        for (int index = A_SCAN_NUM_FRAMES_PER_PACKET_COMPRESSED; index; --index)
        {
            UTDataFrame frame = UTDataFrame();
            frame.Context.SetChannelID(head.nChannelIndex);
            ReadCompressedADataFrame(frame);
            ReadCDataFrame(frame, head.nChannelIndex);
            segment.append(frame);
        }
    }
    else
    {
        segment.reserve(A_SCAN_NUM_FRAMES_PER_PACKET);
        for (int index = A_SCAN_NUM_FRAMES_PER_PACKET; index; --index)
        {
            UTDataFrame frame = UTDataFrame();
            frame.Context.SetChannelID(head.nChannelIndex);
            ReadADataFrame(frame);
            ReadCDataFrame(frame, head.nChannelIndex);
            segment.append(frame);
        }
    }
    Q_ASSERT((segment.size() == A_SCAN_NUM_FRAMES_PER_PACKET || segment.size() == A_SCAN_NUM_FRAMES_PER_PACKET_COMPRESSED) && head.nChannelIndex < NUM_CHANNELS && head.nChannelIndex >= 0);
    segmentCount[head.nChannelIndex]++;
    frameCount[head.nChannelIndex] += segment.count();
#ifdef ARS_DUMP_CSV
    if (platform()->Get("DumpCsv").toBool())
    {
        DumpSegment(QString("AScan%1.csv").arg(head.nChannelIndex), segment);
    }
#endif
    if (disabled[head.nChannelIndex])                                           // 如果为true,说明datalevel改变使得抛弃此数据
    {
        return;
    }
    emit dataArrived(segment);
//    qDebug() << segment->at(0)->Pose[0].raw << segment->at(0)->Pose[1].raw << segment->at(0)->Pose[2].raw << segment->at(0)->Pose[3].raw;
}
