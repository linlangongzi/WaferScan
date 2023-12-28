#include "CScanSocket.h"

#ifdef ARS_DUMP_CSV
#include <Core/Platform.h>
#endif

using namespace Ars500;

//#define ARS_BENCHMARK_INSTRUMENT

CScanSocket::CScanSocket(ParserList parsers, QObject *parent) :
    IncomingBaseSocket(parsers, parent)
{
    Q_ASSERT(parent);
    packetLength = BYTES_PACKET_CSCAN;
    packetBodyLength = BYTES_PACKET_BODY_CSCAN;
    packetCommand = CMD_ASCAN_RET_WAVE;
    requestCmd = CMD_ASCAN;
    requestChannel = 1;
    maskDataLevel = IInstrumentDriver::MASK_CSCAN;
#ifdef ARS_BENCHMARK_INSTRUMENT
    startTimer(1000);
#endif
}

void CScanSocket::timerEvent(QTimerEvent *)
{
    qDebug() << "C1:" << segmentCount[0] << "segments" << frameCount[0] << "frames;" << "C2:" << segmentCount[1] << "segments" << frameCount[1] << "frames";
    segmentCount[0] = 0;
    frameCount[0] = 0;
    segmentCount[1] = 0;
    frameCount[1] = 0;
}

void CScanSocket::FetchData(const PacketHead &head)
{
    UTDataSegment segment;
    segment.reserve(C_SCAN_NUM_FRAMES_PER_PACKET);

    for (int index = C_SCAN_NUM_FRAMES_PER_PACKET; index; --index)
    {
        UTDataFrame frame = UTDataFrame();
        frame.Context.SetChannelID(head.nChannelIndex);
        ReadCDataFrame(frame, head.nChannelIndex);
        segment.append(frame);
    }
    Q_ASSERT(segment.size() == C_SCAN_NUM_FRAMES_PER_PACKET && head.nChannelIndex < NUM_CHANNELS && head.nChannelIndex >= 0);
//    ARS_CHECK_WARNING(head.nChannelIndex)
    segmentCount[head.nChannelIndex]++;
    frameCount[head.nChannelIndex] += segment.count();
#ifdef ARS_DUMP_CSV
    if (platform()->Get("DumpCsv").toBool())
    {
        DumpSegment(QString("CScan%1.csv").arg(head.nChannelIndex), segment);
    }
#endif
    if (disabled[head.nChannelIndex])                                           // 如果为true,说明datalevel改变使得抛弃此数据
    {
        return;
    }
    emit dataArrived(segment);
}
