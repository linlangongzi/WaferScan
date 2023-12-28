#include "PipeParser.h"
#include "PipeDataDef.h"
#include <Core/Platform.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <Ultrasonic/UTSetup.h>

using namespace PipeDetect;

PipeParser::PipeParser(const QJsonObject &config, QObject *parent) :
    QObject(parent)
{
    memset(&data, 0, sizeof(data));
    FPGA_Frequency = config.value("FPGA_Frequency").toInt(100);             // FPGA核心频率
    AD_Frequency = config.value("AD_Frequency").toInt(100);                 // ADC等效频率
    DA_Frequency = 2;                                                       // DAC等效频率  AEC2.0板卡使用2.2727272
    TCG_RAM_Size = config.value("TCG_RAM_Size").toInt(256);                 // TCG的RAM大小
    FPGA_RAM_Size = config.value("FPGA_RAM_Size").toInt(16384);             // FPGA与ARM
    nSampleInterval = 1;    // 默认采样间隔，管探采样间隔是定值，无需计算

    for(qint32 i = 0; i < 128; ++i) {
        DataType << TIME_DATA
                 << AMP_DATA;
    }
}

void PipeParser::ReadADataFrame(QTcpSocket *socket, UTDataFrame &frame)
{
    Q_ASSERT(BYTES_FRAME_ASCAN == sizeof(ADataValue) * A_SCAN_NUM_SAMPLES_PER_FRAME);
    frame.AData.resize(A_SCAN_NUM_SAMPLES_PER_FRAME);
    socket->read(reinterpret_cast<char *>(frame.AData.data()), BYTES_FRAME_ASCAN);
}

void PipeParser::ReadCDataFrame(QTcpSocket *socket, UTDataFrame &frame, FeatureDataType dataType)
{
    if (dataType == FEATURE_ADATA) {
        quint16 buffer[NUM_FEATURES_ADATA];
        Q_ASSERT_X(sizeof(buffer) == BYTES_FRAME_FEATURE_ADATA, "Parser::ReadCDataFrame", "Feature data length mismatch");
        socket->read(reinterpret_cast<char *>(&buffer), BYTES_FRAME_FEATURE_ADATA);       /* 此处使用预先算好的包长度，避免重复运算 */
        frame.CData.resize(NUM_FEATURES_ADATA);

        bool isSurface = false;
        if (buffer[8] > 24) {
            isSurface = true;
        }

        auto maxAmp = static_cast<qreal>(data.stWaveConfigure[isSurface].MiddleValue);

        for (int index = 0; index < 8; ++index) {
            switch (DataType[index]) {
            case TIME_DATA: frame.CData[index] = float(buffer[index]) / AD_Frequency;  break;
            case AMP_DATA:
                buffer[index] = qMin(qreal(buffer[index]), maxAmp);
            default: frame.CData[index] = float(buffer[index]) / maxAmp * 100; break;
            }
        }

        qint32 coders[2] = {(buffer[10] << 16) + buffer[9], (buffer[12] << 16) + buffer[11]};
        frame.Pose = MotionManager::fillPose(coders, 2, {}, nullptr);
        frame.Context.SetRange(data.Range);
    }
}

TCGData PipeParser::CalcTCG(const QJsonArray TCGFromUTSetup)
{
    int nSetupTCGCount = TCGFromUTSetup.count();    // setup中TCG点数 单位：个
    int nSetupIndex = -1;   // 虚拟一个点,位置在第一个点的左侧,故index为-1
    double dIntervalTime = static_cast<double>(nSampleInterval) / DA_Frequency;  // 采样间隔时间 单位：μs

    TCGData vecTCGCurve; // TCG曲线gain列表
    vecTCGCurve.resize(TCG_RAM_Size);   // 将容器初始化为256大小
    QPointF p1(0, TCGFromUTSetup[0].toObject().value("Gain").toDouble());   // 画布左侧边缘添加一个虚拟点
    QPointF p2(TCGFromUTSetup[0].toObject().value("Time").toDouble(), TCGFromUTSetup[0].toObject().value("Gain").toDouble()); // Setup中第一个TCG点

    for (int nIndex = 0; nIndex < TCG_RAM_Size; nIndex++) { // 逐个计算并设置vecTCGCurve[nIndex]
        double dX = nIndex * dIntervalTime; // 当前点的声程 单位：μs
        while( !( p1.x() <= dX && dX < p2.x() ) ) {  // 找出dX所处两点的区间,包含形式为："[ )";
            ++nSetupIndex;
            if (nSetupIndex >= 0 && nSetupIndex + 1 < nSetupTCGCount) { // 移动后的点没有超出最后一个点
                p1.setX(TCGFromUTSetup[nSetupIndex].toObject().value("Time").toDouble());
                p1.setY(TCGFromUTSetup[nSetupIndex].toObject().value("Gain").toDouble());
                p2.setX(TCGFromUTSetup[nSetupIndex + 1].toObject().value("Time").toDouble());
                p2.setY(TCGFromUTSetup[nSetupIndex + 1].toObject().value("Gain").toDouble());
            }
            else { // 如果超出最后一个点，虚拟一个永远不会超出的点来作为p2
                p1 = p2;
                p2.setX(p2.x() + data.Range);
            }
        }

        if (qFuzzyCompare(dX, p1.x())) { // dX点和p1点刚好重合
            vecTCGCurve[nIndex] = p1.y();
            continue;
        }

        double dScale = (dX - p1.x()) / (p2.x() - p1.x());
        QPointF TCGPoint = (p2 - p1) * dScale + p1;
        vecTCGCurve[nIndex] = TCGPoint.y();
    }

    return vecTCGCurve;
}

QByteArray PipeParser::fromStruct(const UTSetup &objSurface, const UTSetup &objFlaw, int channel)
{
    if (channel > 24) {
        data.Velocity = objSurface.material().longitudinalVelocity;          // 北方重工管探下位机目前要声速没有用处
        data.Plus = objSurface.gain();
        data.CPlus = objSurface.gainRef();               // 参考增益
        data.TCGEnable = static_cast<quint16>(objSurface.dacMode() == UTSetup::DacTcg);     // TCG使能
        data.Range = static_cast<float>(qMax(1.0, objSurface.range()));    // 声程
    } else {
        data.Velocity = objFlaw.material().longitudinalVelocity;
        data.Plus = objFlaw.gain();
        data.CPlus = objFlaw.gainRef();                   // 参考增益
        data.TCGEnable = static_cast<quint16>(objFlaw.dacMode() == UTSetup::DacTcg);         // TCG使能
        data.Range = static_cast<float>(qMax(1.0, objFlaw.range()));        // 声程
    }

    data.stWaveConfigure[0].MiddleValue = 490;   // 检波中值
    data.stWaveConfigure[1].MiddleValue = 490;      // 检波中值

    data.stWaveConfigure[0].stGate[0].GateEnable = static_cast<quint16>(objSurface.gate(0).isEnabled()); // 闸门1使能
    data.stWaveConfigure[0].stGate[1].GateEnable = static_cast<quint16>(objSurface.gate(1).isEnabled()); // 闸门2使能
    data.stWaveConfigure[0].stGate[2].GateEnable = static_cast<quint16>(objSurface.gate(2).isEnabled()); // 闸门3使能
    data.stWaveConfigure[0].stGate[3].GateEnable = static_cast<quint16>(objSurface.gate(3).isEnabled()); // 闸门4使能
    data.stWaveConfigure[1].stGate[0].GateEnable = static_cast<quint16>(objFlaw.gate(0).isEnabled());    // 闸门1使能
    data.stWaveConfigure[1].stGate[1].GateEnable = static_cast<quint16>(objFlaw.gate(1).isEnabled());    // 闸门2使能
    data.stWaveConfigure[1].stGate[2].GateEnable = static_cast<quint16>(objFlaw.gate(2).isEnabled());    // 闸门3使能
    data.stWaveConfigure[1].stGate[3].GateEnable = static_cast<quint16>(objFlaw.gate(3).isEnabled());    // 闸门4使能

    data.stWaveConfigure[0].stGate[0].GateAmp = static_cast<float>(objSurface.gate(0).threshold());      // 闸门1幅值
    data.stWaveConfigure[0].stGate[1].GateAmp = static_cast<float>(objSurface.gate(1).threshold());      // 闸门2幅值
    data.stWaveConfigure[0].stGate[2].GateAmp = static_cast<float>(objSurface.gate(2).threshold());      // 闸门3幅值
    data.stWaveConfigure[0].stGate[3].GateAmp = static_cast<float>(objSurface.gate(3).threshold());      // 闸门4幅值
    data.stWaveConfigure[1].stGate[0].GateAmp = static_cast<float>(objFlaw.gate(0).threshold());         // 闸门1幅值
    data.stWaveConfigure[1].stGate[1].GateAmp = static_cast<float>(objFlaw.gate(1).threshold());         // 闸门2幅值
    data.stWaveConfigure[1].stGate[2].GateAmp = static_cast<float>(objFlaw.gate(2).threshold());         // 闸门3幅值
    data.stWaveConfigure[1].stGate[3].GateAmp = static_cast<float>(objFlaw.gate(3).threshold());         // 闸门4幅值

    data.stWaveConfigure[0].stGate[0].GateWidth = static_cast<float>(objSurface.gate(0).width());  // 闸门1宽度
    data.stWaveConfigure[0].stGate[1].GateWidth = static_cast<float>(objSurface.gate(1).width());  // 闸门1宽度
    data.stWaveConfigure[0].stGate[2].GateWidth = static_cast<float>(objSurface.gate(2).width());  // 闸门1宽度
    data.stWaveConfigure[0].stGate[3].GateWidth = static_cast<float>(objSurface.gate(3).width());  // 闸门1宽度
    data.stWaveConfigure[1].stGate[0].GateWidth = static_cast<float>(objFlaw.gate(0).width());     // 闸门1宽度
    data.stWaveConfigure[1].stGate[1].GateWidth = static_cast<float>(objFlaw.gate(1).width());     // 闸门1宽度
    data.stWaveConfigure[1].stGate[2].GateWidth = static_cast<float>(objFlaw.gate(2).width());     // 闸门1宽度
    data.stWaveConfigure[1].stGate[3].GateWidth = static_cast<float>(objFlaw.gate(3).width());     // 闸门1宽度

    data.stWaveConfigure[0].stGate[0].GateStart = static_cast<float>(qMax(objSurface.gate(0).start(), 0.0));  // 闸门1起点
    data.stWaveConfigure[0].stGate[1].GateStart = static_cast<float>(qMax(objSurface.gate(1).start(), 0.0));  // 闸门2起点
    data.stWaveConfigure[0].stGate[2].GateStart = static_cast<float>(qMax(objSurface.gate(2).start(), 0.0));  // 闸门3起点
    data.stWaveConfigure[0].stGate[3].GateStart = static_cast<float>(qMax(objSurface.gate(3).start(), 0.0));  // 闸门4起点
    data.stWaveConfigure[1].stGate[0].GateStart = static_cast<float>(qMax(objFlaw.gate(0).start(), 0.0));     // 闸门1起点
    data.stWaveConfigure[1].stGate[1].GateStart = static_cast<float>(qMax(objFlaw.gate(1).start(), 0.0));     // 闸门2起点
    data.stWaveConfigure[1].stGate[2].GateStart = static_cast<float>(qMax(objFlaw.gate(2).start(), 0.0));     // 闸门3起点
    data.stWaveConfigure[1].stGate[3].GateStart = static_cast<float>(qMax(objFlaw.gate(3).start(), 0.0));     // 闸门4起点

    data.stWaveConfigure[0].Delay = static_cast<float>(objSurface.delay());            // 通道延迟时间
    data.stWaveConfigure[1].Delay = static_cast<float>(objFlaw.delay());               // 通道延迟时间
    data.stWaveConfigure[0].DetectMode = 0;  // 检波模式。0-正半波 1-负半波 2-全波 3-射频
    data.stWaveConfigure[1].DetectMode = 0;     // 检波模式。0-正半波 1-负半波 2-全波 3-射频
    data.stWaveConfigure[0].Suppression = static_cast<float>(objSurface.suppress());     // 抑制
    data.stWaveConfigure[1].Suppression = static_cast<float>(objFlaw.suppress());        // 抑制
    data.ProbeNo = static_cast<quint16>(channel);                                                           // 探头编号

    QByteArray array;
    array.resize(sizeof(data));
    memcpy(array.data(), &data, sizeof(data));
    return array;
}

//ParameterTable PipeParser::toStruct(QByteArray& array, int channel)
//{
//    Q_UNUSED(channel);
//    CH_CONFIG data;
//    memcpy(&data, array.data(), sizeof(data));
//    ParameterTable obj;
//    bool isFlaw = channel < NUM_FLAW_CHANNELS ? true : false;

//    obj.insert("Gain", data.Plus);              // 增益
//    obj.insert("GainRef", data.CPlus);          // 参考增益
//    obj.insert("TcgEnable", data.TCGEnable);    // TCG使能
//    obj.insert("IsFlaw", isFlaw);

//    obj.insert("Gate1Enable", data.stWaveConfigure[isFlaw].stGate[0].GateEnable);   // 闸门1使能
//    obj.insert("Gate2Enable", data.stWaveConfigure[isFlaw].stGate[1].GateEnable);   // 闸门2使能
//    obj.insert("Gate3Enable", data.stWaveConfigure[isFlaw].stGate[2].GateEnable);   // 闸门3使能
//    obj.insert("Gate4Enable", data.stWaveConfigure[isFlaw].stGate[3].GateEnable);   // 闸门4使能

//    obj.insert("Gate1Start", data.stWaveConfigure[isFlaw].stGate[0].GateStart);     // 闸门1起点
//    obj.insert("Gate2Start", data.stWaveConfigure[isFlaw].stGate[1].GateStart);     // 闸门2起点
//    obj.insert("Gate3Start", data.stWaveConfigure[isFlaw].stGate[2].GateStart);     // 闸门3起点
//    obj.insert("Gate4Start", data.stWaveConfigure[isFlaw].stGate[3].GateStart);     // 闸门4起点

//    obj.insert("Gate1Width", data.stWaveConfigure[isFlaw].stGate[0].GateWidth);     // 闸门1宽度
//    obj.insert("Gate2Width", data.stWaveConfigure[isFlaw].stGate[1].GateWidth);     // 闸门2宽度
//    obj.insert("Gate3Width", data.stWaveConfigure[isFlaw].stGate[2].GateWidth);     // 闸门3宽度
//    obj.insert("Gate4Width", data.stWaveConfigure[isFlaw].stGate[3].GateWidth);     // 闸门4宽度

//    obj.insert("Gate1Amp", data.stWaveConfigure[isFlaw].stGate[0].GateAmp);         // 闸门1幅值
//    obj.insert("Gate2Amp", data.stWaveConfigure[isFlaw].stGate[1].GateAmp);         // 闸门2幅值
//    obj.insert("Gate3Amp", data.stWaveConfigure[isFlaw].stGate[2].GateAmp);         // 闸门3幅值
//    obj.insert("Gate4Amp", data.stWaveConfigure[isFlaw].stGate[3].GateAmp);         // 闸门4幅值

//    obj.insert("Delay", data.stWaveConfigure[isFlaw].Delay);                        // 通道延迟时间
//    obj.insert("RectifierZero", data.stWaveConfigure[isFlaw].MiddleValue);          // 中值
//    obj.insert("RectifierMode", data.stWaveConfigure[isFlaw].DetectMode);           // 检波模式
//    obj.insert("Suppress", data.stWaveConfigure[isFlaw].Suppression);               // 底波抑制
//    obj.insert("Range", data.Range);                                                // 声程
//    obj.insert("Velocity", data.Velocity);                                          // 声速

//    return obj;
//}

int PipeParser::GetSampleInterval()
{
    return nSampleInterval;
}

int PipeParser::GettcgRamSize()
{
    return TCG_RAM_Size;
}

TCGData PipeParser::EmptyTCG()
{
    return TCGData(TCG_RAM_Size, 0);
}

int PipeParser::GetTcgRamSize()
{
    return TCG_RAM_Size;
}
