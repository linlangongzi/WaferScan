#include "Parser.h"
#include <QJsonArray>
#include <arsCore/Platform.h>
#include "ProbeNoManager/ProbeNoManager.h"
//#include "PanelAScan/const.h"
#include <Ultrasonic/UTSetup.h>
#include <allrising/Util.h>

using namespace NS_PXI100;

static const int WAVE_MAX_DATA = 995;

Parser::Parser(const QJsonObject &config, QObject *parent) :
    QObject(parent)
{
    memset(&data, 0, sizeof(data));
    data.stWaveConfigure[SURFACE].MiddleValue = quint16(config.value("RectifierZero").toInt(511));  // 检波中值
    data.stWaveConfigure[FLAW].MiddleValue = quint16(config.value("RectifierZero").toInt(511));  // 检波中值
    FPGA_Frequency = config.value("FPGA_Frequency").toInt(100);             // FPGA核心频率
    AD_Frequency = config.value("AD_Frequency").toInt(100);                 // ADC等效频率
    DA_Frequency = config.value("DA_Frequency").toInt(80);                  // DAC等效频率
    TCG_RAM_Size = config.value("TCG_RAM_Size").toInt(64);                // TCG的RAM大小
    FPGA_RAM_Size = config.value("FPGA_RAM_Size").toInt(16384);             // FPGA与ARM
    nSampleInterval = 1;    // 默认采样间隔

//    DataType << TIME_DATA   // 0    Gate1SyncTime
//             << TIME_DATA   // 1    Gate1MPTime
//             << AMP_DATA    // 2    Gate1MPAmp
//             << TIME_DATA   // 3    Gate2MPTime
//             << AMP_DATA    // 4    Gate2MPAmp
//             << TIME_DATA   // 5    Gate2FPFirstTime
//             << TIME_DATA   // 6    Gate2FPTime
//             << AMP_DATA    // 7    Gate2FPAmp
//             << TIME_DATA   // 8    Gate3MPTime
//             << AMP_DATA    // 9    Gate3MPAmp
//             << TIME_DATA   // 10   Gate3FPFirstTime
//             << TIME_DATA   // 11   Gate3FPTime
//             << AMP_DATA    // 12   Gate3FPAmp
//             << TIME_DATA   // 13   Gate4MPTime
//             << AMP_DATA    // 14   Gate4MPAmp
//             << TIME_DATA   // 15   Gate4FPFirstTime
//             << TIME_DATA   // 16   Gate4FPTime
//             << AMP_DATA;   // 17   Gate4FPAmp
    for(qint32 i = 0; i < 128; ++i)
    {
        DataType << TIME_DATA
                 << AMP_DATA;
    }
}

void Parser::ReadADataFrame(QTcpSocket *socket, UTSharedDataFrame &frame)
{
    Q_ASSERT(BYTES_FRAME_ASCAN == sizeof(ADataValue) * A_SCAN_NUM_SAMPLES_PER_FRAME);
    frame->AData.resize(A_SCAN_NUM_SAMPLES_PER_FRAME);
    socket->read(reinterpret_cast<char *>(frame->AData.data()), BYTES_FRAME_ASCAN);
}

void Parser::ReadCDataFrame(QTcpSocket *socket, UTSharedDataFrame &frame, FeatureDataType dataType)
{
    //读取位姿
//    frame->Pose.resize(NUM_AXES);
//    socket->read(reinterpret_cast<char *>(frame->Pose.data()), BYTES_FRAME_POSE);
//    int mod = frame->Pose[0].raw % 2;
//    if (mod) {
//        qWarning("Even");
//    }
//    qDebug() << "Pose" << frame->Pose[0].raw << frame->Pose[1].raw << frame->Pose[2].raw << frame->Pose[3].raw;

    /* 读取特征值 */
    /* 由于QIODevice::read本质是memcpy，涉及指针传递和检查，用于逐个处理数据并不理想，所以使
     * 用一个固定数量特征值缓冲区，此缓冲区在栈上分配更高效。 */

    switch (dataType)
    {
        case FEATURE_ADATA:
        {
//            Q_ASSERT(BYTES_FRAME_FEATURE_CDATA + BYTES_FRAME_POSE == BYTES_FRAME_CDATA);
            CDataValueRaw buffer[NUM_FEATURES_ADATA];
            Q_ASSERT_X(sizeof(buffer) == BYTES_FRAME_FEATURE_ADATA, "Parser::ReadCDataFrame", "Feature data length mismatch");
            socket->read(reinterpret_cast<char *>(&buffer), BYTES_FRAME_FEATURE_ADATA);       /* 此处使用预先算好的包长度，避免重复运算 */
            int nCDataNum = NUM_MONITOR_CDATA; // I1和I2由高低位组成
            frame->CData.resize(nCDataNum);
            int nLastFeature = nCDataNum - 3;
            for (int index = nLastFeature; index >= 0; --index)
            {
                switch (DataType[index])
                {
                case TIME_DATA:
                    if(index == 0)
                    {
                        quint32 data = 0;
                        memcpy(&data, &buffer[index], 4);
                        frame->CData[index] = float(data) / AD_Frequency;
                    }
                    else if(index == 6)
                    {
                        quint32 data = 0;
                        memcpy(&data, &buffer[index + 1], 4);
                        frame->CData[index] = float(data) / AD_Frequency;
                    }
                    else
                    {
                        frame->CData[index] = float(buffer[index + 1]) / AD_Frequency;
                    }

                    break;
                case AMP_DATA:
                default:
                    if(index < 7)
                    {
                       frame->CData[index] = float(buffer[index + 1]) / WAVE_MAX_DATA * 100;
                    }
                    else
                    {
                        frame->CData[index] = float(buffer[index + 2]) / WAVE_MAX_DATA * 100;
                    }
                    break;
                }
            }
            frame->CData[nCDataNum - 2] = float(buffer[NUM_FEATURES_ADATA - 2]);
            frame->CData[nCDataNum - 1] = float(buffer[NUM_FEATURES_ADATA - 1]) / AD_Frequency;
            break;
        }
        case FEATURE_CDATA:
        default:
        {
            CDataValueRaw buffer[NUM_FEATURES_CDATA];
            Q_ASSERT_X(sizeof(buffer) == BYTES_FRAME_FEATURE_CDATA,
                       "Parser::ReadCDataFrame", "Feature data length mismatch");
            /* 此处使用预先算好的包长度，避免重复运算 */
            socket->read(reinterpret_cast<char *>(&buffer), BYTES_FRAME_FEATURE_CDATA);
            int nCDataNum = NUM_FRAME_CDATA;
            frame->CData.resize(nCDataNum);
            int nOffset = 0;
            int nLastFeature = nCDataNum - 27;
            /* 因为需要同时访问DataType表，下标比迭代器更快 */
            for (int index = 0; index < nCDataNum; ++index) {
                // 无高低位
                // 1: 0~1不取值
                // 2: 2~129为8个硬通道的闸门特征值，共 8*2*8 = 128
                // 3: 130~137为8个硬通道当前对应的探头编号
                // 4: 138为当前一帧的光眼状态0~127
                // 有高低位
                // 1: 0~1不取值
                // 2: 2~161为8个硬通道的闸门特征值
                // 3: 162~169为8个硬通道当前对应的探头编号
                // 4: 170为当前一帧的光眼状态0~127
                if(index < 2) {
                    frame->CData[index] = float(buffer[index]);
                } else if (index > 1 && index < nLastFeature) {          // 2
                    switch (DataType[index - 2])
                    {
                        case TIME_DATA:
                            // 2 8 10 16|18 24 26 32|34 40 42 48|50 56 58 64
                            // 66 72 74 80|82 88 90 96|98 104 106 112|114 120 122 128由高低位组成
                            if((index % 16 == 2) || (index % 16 == 8)
                                    || (index % 16 == 10) || (index % 16 == 0)) {
                                quint32 data = 0;
                                memcpy(&data, &buffer[index + nOffset], 4);
                                data = data / AD_Frequency;
                                frame->CData[index] = data;
                                nOffset++;
                            } else {
                                frame->CData[index] = float(buffer[index + nOffset]) / AD_Frequency;
                            }
                            break;
                        case AMP_DATA:
                        default:
                            frame->CData[index] = float(buffer[index + nOffset]) / WAVE_MAX_DATA * 100;
                            break;
                    }
                } else if ((index >= nLastFeature && index < nLastFeature + 9)            // 探头号
                         || index > nLastFeature + 25) {                                // 轮号
                    frame->CData[index] = float(buffer[index + nOffset]);
                } else if (index >= nLastFeature + 9 && index < nLastFeature + 25) {    // B闸门波形宽度
                    frame->CData[index] = float(buffer[index + nOffset]) / AD_Frequency;
                } else if (index == nLastFeature + 25) {                                // 时间戳
                    quint32 data = 0;
                    memcpy(&data, &buffer[index + nOffset], 4);
                    frame->CData[index] = data;
                    nOffset++;
                }
            }
            break;
        }
    }

    frame->Context.SetRange(data.Range);
    frame->Context.SetDelay(delayTimeFlaw);
}


void Parser::MonitorCData2Raw(UTSharedDataFrame frame, CDataValueRaw* buffer)
{
//    CDataValueRaw buffer[NUM_FEATURES_ADATA];
    buffer[NUM_FEATURES_ADATA - 1] = static_cast<CDataValueRaw>(frame->CData[NUM_MONITOR_CDATA - 1]);
    int nOffset = 0;
//    QJsonObject obj;
//    Parser parser(obj);
    for (int index = 0; index < NUM_MONITOR_CDATA - 1; ++index)
    {
        switch (DataType[index])
        {
        case TIME_DATA:
            if(index == 0 || index == 6)
            {
                quint32 value = static_cast<quint32>(frame->CData[index] * AD_Frequency);
                buffer[index + nOffset] = static_cast<CDataValueRaw>(value);
                buffer[index + nOffset + 1] = static_cast<CDataValueRaw>(value >> 16);
                nOffset++;
            }
            else
            {
                buffer[index+ nOffset] = static_cast<CDataValueRaw>(frame->CData[index] * AD_Frequency);
            }

            break;
        case AMP_DATA:
        default:
            buffer[index+ nOffset] = static_cast<CDataValueRaw>(frame->CData[index] * WAVE_MAX_DATA / 100);
            break;
        }
    }
}

TCGData Parser::CalcTCG(const QJsonArray TCGFromUTSetup)
{
    int nSetupTCGCount = TCGFromUTSetup.count();                                // setup中TCG点数 单位：个
    int nSetupIndex = -1;                                                       // 虚拟一个点,位置在第一个点的左侧,故index为-1
    double dIntervalTime = double(nSampleInterval) / DA_Frequency;              // 采样间隔时间 单位：us

    TCGData vecTCGCurve;                                                        // TCG曲线gain列表
    vecTCGCurve.resize(TCG_RAM_Size);                                           // 将容器初始化为1024大小

    QPointF p1(0, TCGFromUTSetup[0].toObject().value("Gain").toDouble());       // 画布左侧边缘添加一个虚拟点
    QPointF p2(TCGFromUTSetup[0].toObject().value("Time").toDouble(), TCGFromUTSetup[0].toObject().value("Gain").toDouble());   // Setup中第一个TCG点

    for (int nIndex = 0; nIndex < TCG_RAM_Size; nIndex++) {                     // 逐个计算并设置vecTCGCurve[nIndex]
        double dX = nIndex * dIntervalTime;                                     // 当前点的声程 单位：us
        while( !( p1.x() <= dX && dX < p2.x() ) ) {                             // 找出dX所处两点的区间,包含形式为："[ )";
            ++nSetupIndex;
            if (nSetupIndex >= 0 && nSetupIndex + 1 < nSetupTCGCount) {         // 移动后的点没有超出最后一个点
                p1.setX(TCGFromUTSetup[nSetupIndex].toObject().value("Time").toDouble());
                p1.setY(TCGFromUTSetup[nSetupIndex].toObject().value("Gain").toDouble());
                p2.setX(TCGFromUTSetup[nSetupIndex + 1].toObject().value("Time").toDouble());
                p2.setY(TCGFromUTSetup[nSetupIndex + 1].toObject().value("Gain").toDouble());
            } else {                                                            // 如果超出最后一个点，虚拟一个永远不会超出的点来作为p2
                p1 = p2;
                p2.setX(p2.x() + data.Range);
            }
        }

        if (qFuzzyCompare(dX, p1.x())) {                                        // dX点和p1点刚好重合
            vecTCGCurve[nIndex] = p1.y();
            continue;
        }

        double dScale = (dX - p1.x()) / (p2.x() - p1.x());
        QPointF TCGPoint = (p2 - p1) * dScale + p1;
        vecTCGCurve[nIndex] = TCGPoint.y();
    }

    return vecTCGCurve;
}

QByteArray Parser::fromStruct(const ParameterTable &obj, int channel)
{
    data.Velocity = qint32(obj.value("Velocity").toInt(1000));
    data.Plus = obj.value("Gain").toDouble(10) + obj.value("GainDelta").toDouble(0);

    data.CPlus = obj.value("GainRef").toDouble(0);                      // 参考增益
    data.TCGEnable = quint16(obj.value("TcgEnable").toBool(false));            // TCG使能
    data.Range = qMax(1.0, obj.value("Range").toDouble(100));           // 声程
    data.Delay = obj.value("Delay").toDouble(0);                        // 延迟

    data.stWaveConfigure[SURFACE].stGate[0].GateEnable = quint16(obj.value("SurfaceGate1Enable").toInt(1));   // 闸门1使能
    data.stWaveConfigure[SURFACE].stGate[1].GateEnable = quint16(obj.value("SurfaceGate2Enable").toInt(1));   // 闸门2使能
    data.stWaveConfigure[SURFACE].stGate[2].GateEnable = quint16(obj.value("SurfaceGate3Enable").toInt(1));   // 闸门3使能
    data.stWaveConfigure[SURFACE].stGate[3].GateEnable = quint16(obj.value("SurfaceGate4Enable").toInt(1));   // 闸门4使能
    data.stWaveConfigure[FLAW].stGate[0].GateEnable = quint16(obj.value("FlawGate1Enable").toInt(1));         // 闸门1使能
    data.stWaveConfigure[FLAW].stGate[1].GateEnable = quint16(obj.value("FlawGate2Enable").toInt(1));         // 闸门2使能
    data.stWaveConfigure[FLAW].stGate[2].GateEnable = quint16(obj.value("FlawGate3Enable").toInt(1));         // 闸门3使能
    data.stWaveConfigure[FLAW].stGate[3].GateEnable = quint16(obj.value("FlawGate4Enable").toInt(1));         // 闸门4使能

    data.stWaveConfigure[SURFACE].stGate[0].GateAmp = obj.value("SurfaceGate1Amp").toDouble(30);       // 闸门1幅值
    data.stWaveConfigure[SURFACE].stGate[1].GateAmp = obj.value("SurfaceGate2Amp").toDouble(30);       // 闸门2幅值
    data.stWaveConfigure[SURFACE].stGate[2].GateAmp = obj.value("SurfaceGate3Amp").toDouble(30);       // 闸门3幅值
    data.stWaveConfigure[SURFACE].stGate[3].GateAmp = obj.value("SurfaceGate4Amp").toDouble(30);       // 闸门4幅值
    data.stWaveConfigure[FLAW].stGate[0].GateAmp = obj.value("FlawGate1Amp").toDouble(30);             // 闸门1幅值
    data.stWaveConfigure[FLAW].stGate[1].GateAmp = obj.value("FlawGate2Amp").toDouble(30);             // 闸门2幅值
    data.stWaveConfigure[FLAW].stGate[2].GateAmp = obj.value("FlawGate3Amp").toDouble(30);             // 闸门3幅值
    data.stWaveConfigure[FLAW].stGate[3].GateAmp = obj.value("FlawGate4Amp").toDouble(30);             // 闸门4幅值

    data.stWaveConfigure[SURFACE].stGate[0].GateWidth = obj.value("SurfaceGate1Width").toDouble(20);   // 闸门1宽度
    data.stWaveConfigure[SURFACE].stGate[1].GateWidth = obj.value("SurfaceGate2Width").toDouble(20);   // 闸门1宽度
    data.stWaveConfigure[SURFACE].stGate[2].GateWidth = obj.value("SurfaceGate3Width").toDouble(20);   // 闸门1宽度
    data.stWaveConfigure[SURFACE].stGate[3].GateWidth = obj.value("SurfaceGate4Width").toDouble(20);   // 闸门1宽度
    data.stWaveConfigure[FLAW].stGate[0].GateWidth = obj.value("FlawGate1Width").toDouble(20);         // 闸门1宽度
    data.stWaveConfigure[FLAW].stGate[1].GateWidth = obj.value("FlawGate2Width").toDouble(20);         // 闸门1宽度
    data.stWaveConfigure[FLAW].stGate[2].GateWidth = obj.value("FlawGate3Width").toDouble(20);         // 闸门1宽度
    data.stWaveConfigure[FLAW].stGate[3].GateWidth = obj.value("FlawGate4Width").toDouble(20);         // 闸门1宽度

    delayTimeFlaw = obj.value("FlawDelay").toDouble();
    delayTimeSurface = obj.value("SurfaceDelay").toDouble();
    data.stWaveConfigure[SURFACE].stGate[0].GateStart = qMax(obj.value("SurfaceGate1Start").toDouble(0.0), 0.0);   // 闸门1起点
    data.stWaveConfigure[SURFACE].stGate[1].GateStart = qMax(obj.value("SurfaceGate2Start").toDouble(0.0), 0.0);   // 闸门2起点
    data.stWaveConfigure[SURFACE].stGate[2].GateStart = qMax(obj.value("SurfaceGate3Start").toDouble(0.0), 0.0);   // 闸门3起点
    data.stWaveConfigure[SURFACE].stGate[3].GateStart = qMax(obj.value("SurfaceGate4Start").toDouble(0.0), 0.0);   // 闸门4起点
    data.stWaveConfigure[FLAW].stGate[0].GateStart = qMax(obj.value("FlawGate1Start").toDouble(0.0), 0.0);         // 闸门1起点
    data.stWaveConfigure[FLAW].stGate[1].GateStart = qMax(obj.value("FlawGate2Start").toDouble(0.0), 0.0);         // 闸门2起点
    data.stWaveConfigure[FLAW].stGate[2].GateStart = qMax(obj.value("FlawGate3Start").toDouble(0.0), 0.0);         // 闸门3起点
    data.stWaveConfigure[FLAW].stGate[3].GateStart = qMax(obj.value("FlawGate4Start").toDouble(0.0), 0.0);         // 闸门4起点

    data.stWaveConfigure[SURFACE].Delay = delayTimeSurface;        // 通道延迟时间
    data.stWaveConfigure[FLAW].Delay = delayTimeFlaw;              // 通道延迟时间

    data.stWaveConfigure[SURFACE].DetectMode = quint16(obj.value("SurfaceRectifierMode").toInt(2));   // 检波模式。0-正半波 1-负半波 2-全波 3-射频
    data.stWaveConfigure[FLAW].DetectMode = quint16(obj.value("FlawRectifierMode").toInt(2));         // 检波模式。0-正半波 1-负半波 2-全波 3-射频

    data.stWaveConfigure[SURFACE].Suppression = obj.value("SurfaceSuppress").toDouble(0);      // 抑制
    data.stWaveConfigure[FLAW].Suppression = obj.value("FlawSuppress").toDouble(0);            // 抑制

    data.ProbeNo = quint16(channel);                                            // 探头编号

    QByteArray array;
    array.resize(sizeof(data));
    memcpy(array.data(), &data, sizeof(data));
    return array;
}

ParameterTable Parser::toStruct(QByteArray& array, int channel)
{
    Q_UNUSED(channel);
    CH_CONFIG data;
    memcpy(&data, array.data(), sizeof(data));
    ParameterTable obj;

    obj.insert("Gain", data.Plus);              // 增益
    obj.insert("GainRef", data.CPlus);          // 参考增益
    obj.insert("TCGEnable", data.TCGEnable);    // TCG使能

    obj.insert("SurfaceGate1Enable", data.stWaveConfigure[SURFACE].stGate[0].GateEnable); // 闸门1使能
    obj.insert("SurfaceGate2Enable", data.stWaveConfigure[SURFACE].stGate[1].GateEnable); // 闸门2使能
    obj.insert("SurfaceGate3Enable", data.stWaveConfigure[SURFACE].stGate[2].GateEnable); // 闸门3使能
    obj.insert("SurfaceGate4Enable", data.stWaveConfigure[SURFACE].stGate[3].GateEnable); // 闸门4使能
    obj.insert("FlawGate1Enable", data.stWaveConfigure[FLAW].stGate[0].GateEnable);    // 闸门1使能
    obj.insert("FlawGate2Enable", data.stWaveConfigure[FLAW].stGate[1].GateEnable);    // 闸门2使能
    obj.insert("FlawGate3Enable", data.stWaveConfigure[FLAW].stGate[2].GateEnable);    // 闸门3使能
    obj.insert("FlawGate4Enable", data.stWaveConfigure[FLAW].stGate[3].GateEnable);    // 闸门4使能

    obj.insert("SurfaceGate1Start", data.stWaveConfigure[SURFACE].stGate[0].GateStart);  // 闸门1起点
    obj.insert("SurfaceGate2Start", data.stWaveConfigure[SURFACE].stGate[1].GateStart);  // 闸门2起点
    obj.insert("SurfaceGate3Start", data.stWaveConfigure[SURFACE].stGate[2].GateStart);  // 闸门3起点
    obj.insert("SurfaceGate4Start", data.stWaveConfigure[SURFACE].stGate[3].GateStart);  // 闸门4起点
    obj.insert("FlawGate1Start", data.stWaveConfigure[FLAW].stGate[0].GateStart);     // 闸门1起点
    obj.insert("FlawGate2Start", data.stWaveConfigure[FLAW].stGate[1].GateStart);     // 闸门2起点
    obj.insert("FlawGate3Start", data.stWaveConfigure[FLAW].stGate[2].GateStart);     // 闸门3起点
    obj.insert("FlawGate4Start", data.stWaveConfigure[FLAW].stGate[3].GateStart);     // 闸门4起点

    obj.insert("SurfaceGate1Width", data.stWaveConfigure[SURFACE].stGate[0].GateWidth);  // 闸门1宽度
    obj.insert("SurfaceGate2Width", data.stWaveConfigure[SURFACE].stGate[1].GateWidth);  // 闸门2宽度
    obj.insert("SurfaceGate3Width", data.stWaveConfigure[SURFACE].stGate[2].GateWidth);  // 闸门3宽度
    obj.insert("SurfaceGate4Width", data.stWaveConfigure[SURFACE].stGate[3].GateWidth);  // 闸门4宽度
    obj.insert("FlawGate1Width", data.stWaveConfigure[FLAW].stGate[0].GateWidth);     // 闸门1宽度
    obj.insert("FlawGate2Width", data.stWaveConfigure[FLAW].stGate[1].GateWidth);     // 闸门2宽度
    obj.insert("FlawGate3Width", data.stWaveConfigure[FLAW].stGate[2].GateWidth);     // 闸门3宽度
    obj.insert("FlawGate4Width", data.stWaveConfigure[FLAW].stGate[3].GateWidth);     // 闸门4宽度

    obj.insert("SurfaceGate1Amp", data.stWaveConfigure[SURFACE].stGate[0].GateAmp);   // 闸门1幅值
    obj.insert("SurfaceGate2Amp", data.stWaveConfigure[SURFACE].stGate[1].GateAmp);   // 闸门2幅值
    obj.insert("SurfaceGate3Amp", data.stWaveConfigure[SURFACE].stGate[2].GateAmp);   // 闸门3幅值
    obj.insert("SurfaceGate4Amp", data.stWaveConfigure[SURFACE].stGate[3].GateAmp);   // 闸门4幅值
    obj.insert("FlawGate1Amp", data.stWaveConfigure[FLAW].stGate[0].GateAmp);      // 闸门1幅值
    obj.insert("FlawGate2Amp", data.stWaveConfigure[FLAW].stGate[1].GateAmp);      // 闸门2幅值
    obj.insert("FlawGate3Amp", data.stWaveConfigure[FLAW].stGate[2].GateAmp);      // 闸门3幅值
    obj.insert("FlawGate4Amp", data.stWaveConfigure[FLAW].stGate[3].GateAmp);      // 闸门4幅值

    obj.insert("SurfaceDelay", data.stWaveConfigure[SURFACE].Delay);                    // 通道延迟时间
    obj.insert("FlawDelay", data.stWaveConfigure[FLAW].Delay);                          // 通道延迟时间

    obj.insert("SurfaceRectifierZero", data.stWaveConfigure[SURFACE].MiddleValue);      // 中值
    obj.insert("FlawRectifierZero", data.stWaveConfigure[FLAW].MiddleValue);            // 中值

    obj.insert("SurfaceRectifierMode", data.stWaveConfigure[SURFACE].DetectMode);       // 检波模式
    obj.insert("FlawRectifierMode", data.stWaveConfigure[FLAW].DetectMode);             // 检波模式

    obj.insert("SurfaceSuppress", data.stWaveConfigure[SURFACE].Suppression);           // 底波抑制
    obj.insert("FlawSuppress", data.stWaveConfigure[FLAW].Suppression);                 // 伤波抑制

    obj.insert("Range", data.Range);                                                    // 声程
    obj.insert("Velocity", data.Velocity);                                              // 声速
    obj.insert("Delay", data.Delay);

    return obj;
}

int Parser::GetSampleInterval()
{
    return nSampleInterval;
}

int Parser::GettcgRamSize()
{
    return TCG_RAM_Size;
}

TCGData Parser::EmptyTCG()
{
    return TCGData(TCG_RAM_Size, 0);
}

qint16 Parser::Gain2Data(double data)
{
    qint16 nData;

    if(data < 0.0) {
        nData = 0;
    } else if( data > 130.0) {
        nData = 130;
    } else {
        nData = static_cast<qint16>((data * 20.0 / 3.0 * AEC_AMP_LINEAR_PARA_A) + 0.5);
    }

    return nData;
}
