#include "Parser.h"
#include <Core/Platform.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <Ultrasonic/UTSetup.h>

using namespace Ars500;

Parser::Parser(const QJsonObject &config, QObject *parent) :
    QObject(parent)
{
    memset( &data, 0, sizeof(data) );
    data.stConfig.ndBLevel1 = config.value("L1Amp").toInt(7789);                // 1级增益 AD603
    data.stConfig.ndBLevel2 = config.value("L2Amp").toInt(1453);                // 2级增益 AD600-1
    data.stConfig.ndBLevel3 = config.value("L3Amp").toInt(7789);                // 3级增益 AD600-2
    data.fPlusAddX11 = config.value("GainCompensation11").toDouble(0);          // 增益补偿 X11
    data.fPlusAddX12 = config.value("GainCompensation12").toDouble(0);          // 增益补偿 X12
    data.fPlusAddX21 = config.value("GainCompensation21").toDouble(0);          // 增益补偿 X21
    data.fPlusAddX22 = config.value("GainCompensation22").toDouble(0);          // 增益补偿 X22
    data.stConfig.nValueMiddle = config.value("RectifierZero").toInt(511);      // 检波中值
    FPGA_Frequency = config.value("FPGA_Frequency").toInt(100);                 // FPGA核心频率
    AD_Frequency = config.value("AD_Frequency").toInt(400);                     // ADC等效频率
    DA_Frequency = config.value("DA_Frequency").toInt(80);                      // DAC等效频率
    TCG_RAM_Size = config.value("TCG_RAM_Size").toInt(1024);                    // TCG的RAM大小
    FPGA_RAM_Size = config.value("FPGA_RAM_Size").toInt(16384);                 // FPGA与ARM进行波形数据交换的缓冲区中的点数 */
    nSampleInterval = 1;                                                        // 默认采样间隔
}

qint64 Parser::ReadADataFrame(QTcpSocket *socket, UTDataFrame &frame)
{
    Q_ASSERT(BYTES_FRAME_ASCAN == sizeof(ADataValue) * A_SCAN_NUM_SAMPLES_PER_FRAME);
    frame.AData.resize(A_SCAN_NUM_SAMPLES_PER_FRAME);
    char dummy[sizeof(ADataValue)];
    qint64 bytesRead = socket->read(dummy, sizeof(dummy));
    bytesRead += socket->read(reinterpret_cast<char *>(frame.AData.data()), BYTES_FRAME_ASCAN - sizeof(dummy));
    frame.AData.squeeze();
    Q_ASSERT(bytesRead == 1024);
    return bytesRead;
}

qint64 Parser::ReadCompressedADataFrame(QTcpSocket *socket, UTDataFrame &frame)
{
    QVarLengthArray<uchar, 512> buffer(A_SCAN_NUM_SAMPLES_PER_FRAME);
    qint64 bytesRead = socket->read(reinterpret_cast<char *>(buffer.data()), A_SCAN_NUM_SAMPLES_PER_FRAME);
    ADataFrame &AData = frame.AData;
    AData.resize(A_SCAN_NUM_SAMPLES_PER_FRAME);
    AData.squeeze();
    for (int index = A_SCAN_NUM_SAMPLES_PER_FRAME - 1; index; --index)              /* 将读出来的512个点映射到A扫数据中的0-510 */
    {
        AData[index - 1] = ADataValue(buffer[index]) << 2;
    }
    return bytesRead;
}

qint64 Parser::ReadCDataFrame(QTcpSocket *socket, UTDataFrame &frame)
{
    int sampleInterval = data.fRange * AD_Frequency / FPGA_RAM_Size;            // 取样间隔
    return ReadCDataFrame(socket, frame, sampleInterval + 1);
}

qint64 Parser::ReadCDataFrame(QTcpSocket *socket, UTDataFrame &frame, int timeMultiplier)
{
    Q_ASSERT(BYTES_FRAME_FEATURE + BYTES_FRAME_POSE == BYTES_FRAME_CDATA);
    /* 读取位姿 */
    qint32 buf[NUM_AXES];
    qint64 bytesRead = socket->read(reinterpret_cast<char *>(buf), BYTES_FRAME_POSE);
    quint32 direction = 0;
    frame.Pose = MotionManager::fillPose(buf, NUM_AXES, poseOffset, &direction);
//    qDebug() << buf[0] << buf[1] << buf[2] << buf[3];

    /* 读取特征值 */
    /* 由于QIODevice::read本质是memcpy，涉及指针传递和检查，用于逐个处理数据并不理想，所以使
     * 用一个固定数量特征值缓冲区，此缓冲区在栈上分配更高效。 */
    CDataValueRaw buffer[NUM_FEATURES];
    Q_ASSERT_X(sizeof(buffer) == BYTES_FRAME_FEATURE, "Parser::ReadCDataFrame", "Feature data length mismatch");
    bytesRead += socket->read(reinterpret_cast<char *>(&buffer), BYTES_FRAME_FEATURE);       /* 此处使用预先算好的包长度，避免重复运算 */
    frame.CData.resize(NUM_FEATURES);
    /* 因为需要同时访问DataType表，下标比迭代器更快 */
    for (int index = NUM_FEATURES - 1; index >= 0; --index)
    {
        switch (Ars500::CDataType[index])
        {
        case DATA_TYPE_TIME:
            frame.CData[index] = float(buffer[index]) * (timeMultiplier) / AD_Frequency; break;
        case DATA_TYPE_AMP:
        default:
            frame.CData[index] = float(buffer[index]) / 1023.0 * 100;
        }
    }
    frame.CData.squeeze();

    frame.Context.SetRange(data.fRange);
    frame.Context.setDelay(delayTime);
    frame.Context.SetDirections(direction);

    return bytesRead;
}

TCGData Parser::CalcTCG(const QJsonArray TCGFromUTSetup)
{
    int nSetupTCGCount = TCGFromUTSetup.count();                                // setup中TCG点数 单位：个
    int nSetupIndex = -1;                                                       // 虚拟一个点,位置在第一个点的左侧,故index为-1

    double dClockCycleNum = data.fRange * DA_Frequency;                         // range声程内的时钟周期数 单位：个
    nSampleInterval = ceil(dClockCycleNum / TCG_RAM_Size);                      // 采样间隔 单位：个
    if (nSampleInterval <= 0)
    {
        nSampleInterval = 1;
    }
    double dIntervalTime = double(nSampleInterval) / DA_Frequency;              // 采样间隔时间 单位：μs

    TCGData vecTCGCurve;                                                        // TCG曲线gain列表
    vecTCGCurve.resize(TCG_RAM_Size);                                           // 将容器初始化为1024大小

    /* 硬件TCG精细调试，使用全0的TCG表，令其中的3项为50，检查TCG生成电路的时间分辨率 (Lin Xinbo) */
//#define TCG_CALIBERATION
#ifdef TCG_CALIBERATION
    vecTCGCurve.fill(0);
    int pos = qMax(TCGFromUTSetup[0].toObject().value("Time").toDouble(), 1);
    vecTCGCurve[pos - 1] = 50;
    vecTCGCurve[pos] = 50;
    vecTCGCurve[pos + 1] = 50;
    nSampleInterval = TCGFromUTSetup[0].toObject().value("Gain").toDouble();
    return vecTCGCurve;
#endif

    QPointF p1( 0, TCGFromUTSetup[0].toObject().value("Gain").toDouble() );     // 画布左侧边缘添加一个虚拟点
    QPointF p2( TCGFromUTSetup[0].toObject().value("Time").toDouble(), TCGFromUTSetup[0].toObject().value("Gain").toDouble() ); // Setup中第一个TCG点

    for (int nIndex = 0; nIndex < TCG_RAM_Size; nIndex++)                       // 逐个计算并设置vecTCGCurve[nIndex]
    {
        double dX = nIndex * dIntervalTime;                                     // 当前点的声程 单位：μs

        while ( !( p1.x() <= dX && dX < p2.x() ) )                              // 找出dX所处两点的区间,包含形式为："[ )";
        {
            ++nSetupIndex;

            if ( (nSetupIndex >= 0) && (nSetupIndex + 1 < nSetupTCGCount) )     // 移动后的点没有超出最后一个点
            {
                p1.setX( TCGFromUTSetup[nSetupIndex].toObject().value("Time").toDouble() );
                p1.setY( TCGFromUTSetup[nSetupIndex].toObject().value("Gain").toDouble() );
                p2.setX( TCGFromUTSetup[nSetupIndex + 1].toObject().value("Time").toDouble() );
                p2.setY( TCGFromUTSetup[nSetupIndex + 1].toObject().value("Gain").toDouble() );
            }
            else                                                                // 如果超出最后一个点，虚拟一个永远不会超出的点来作为p2
            {
                p1 = p2;
                p2.setX(p2.x() + data.fRange);
            }
        }

        if ( qFuzzyCompare( dX, p1.x() ) )                                      // dX点和p1点刚好重合
        {
            vecTCGCurve[nIndex] = p1.y();
            continue;
        }

        double dScale = ( dX - p1.x() ) / ( p2.x() - p1.x() );
        QPointF TCGPoint = (p2 - p1) * dScale + p1;
        vecTCGCurve[nIndex] = TCGPoint.y();
//        qDebug() << nIndex * dIntervalTime << TCGPoint.y();
    }

    return vecTCGCurve;
}

TCGData Parser::EmptyTCG()
{
    return TCGData(TCG_RAM_Size, 0);
}

void Parser::UpdateStruct(const UTSetup &obj, int channel)
{
//    ParameterStruct data;
    data.stCon.nReset = 0;                                                      //通道软复位，暂不使用
    data.stCon.nMaiKuan = 2;                                                    // 脉宽，在ARS500中无效
    data.nSPIAddr = 0;                                                          //SPI地址选择 串行D/A 8路通道选择 数字1-8代表
    data.nSPIData = 0;                                                          //0x0000~0x3FFF：其中0x0000为-6V，0x3FFF为+6V
    data.nVelocity = WAVE_SPEED;                                                //声速

//    data.stConfig.ndBLevel1 = obj.value("L1Amp").toInt(7789);      //1级增益 AD603
//    data.stConfig.ndBLevel2 = obj.value("L2Amp").toInt(1453);      //2级增益 AD600-1
//    data.stConfig.ndBLevel3 = obj.value("L3Amp").toInt(7789);      //3级增益 AD600-2
//    data.fPlusAddX11 = obj.value("GainCompensation11").toDouble(0);          //增益补偿 X11
//    data.fPlusAddX12 = obj.value("GainCompensation12").toDouble(0);          //增益补偿 X12
//    data.fPlusAddX21 = obj.value("GainCompensation21").toDouble(0);          //增益补偿 X21
//    data.fPlusAddX22 = obj.value("GainCompensation22").toDouble(0);          //增益补偿 X22
//    data.stConfig.nValueMiddle = obj.value("RectifierZero").toInt(511);   //检波中值

    data.stConfig.ndBControl = 1;                                               //增益转换开关
    data.stConfig.nTcgEnable = obj.dacMode() == UTSetup::DacTcg;            //DAC使能开关
//    qDebug() << "TcgEnable: " << obj.value("TcgEnable") << obj.value("TcgEnable").toBool(false) << data.stConfig.nTcgEnable;
    data.stConfig.ndBRef = obj.gainRef();              //增益控制基准
    data.stCon.nMaiKuan = obj.pulseWidth();                // 脉宽

    data.stCon.nRepeatCounter = FPGA_Frequency * 1.0E6 / 256 / obj.prf();//重复频率计数器，单位是FPGA时钟周期, 右移8bit
    data.stConfig.nSuspend = obj.suppress() / 100 * 1024;    //抑制

    switch (obj.rectifierMode()) {                    // 下位机的检波模式为：0-全波 1-正半波 2-负半波 3-射频
    case UTSetup::RectifierPositive:     data.stConfig.nDetectMode = 0; break;                           /* 1: 正半波，对应ARM程序0 */
    case UTSetup::RectifierNegative:     data.stConfig.nDetectMode = 1; break;                           /* 2: 负半波，对应ARM程序1 */
    case UTSetup::RectifierRf:     data.stConfig.nDetectMode = 3; break;                           /* 3: 射频，对应ARM程序3 */
    case UTSetup::RectifierFull:
    default:    data.stConfig.nDetectMode = 2; break;                           /* 0: 全波，对应ARM程序2 (默认) */
    }

    /* 在上层软件架构中，声程全部使用时间(μs)，但下位机仍保留了距离(mm)的计算。
     * 下发的声程(mm)将被下位机根据声速(m/s)，处理为FPGA的时钟周期数。
     * 此处尝试使用一个特殊的声速1000m/s，即1mm/μs，使距离和时间的数值相等。
     * 由于下位机计算公式中会将声程时间*2，所以使用2倍的声速，即2000m/s来抵消。 */
    data.fRange = qMax( 1.0, obj.range());

    /* SampleInterval是FPGA配置寄存器，由声程计算得来，由于下位机暂不计算此数值，需要由上位机提供计算。 */
    int nDotNum = qRound(data.fRange * 2 / (WAVE_SPEED * 1000) * AD_Frequency * 1E6);
    data.stGate.nSimpleInterval = nDotNum / FPGA_RAM_Size;

    delayTime = obj.delay();
//    double zero = obj.value("Zero").toDouble();                                 // modify by luwei 20150331 延迟的下发需要加上零点
    double zero = 0;
    data.stGate.nDelay = (delayTime + zero) * FPGA_Frequency;                   //通道延迟时间
    data.stConfig.nMonitorZeroCounter = obj.zero() * FPGA_Frequency;  //零点修正监视器
    data.stGate.nSimpleAverage = obj.sampleAverage();//通道采样平均数, 为pow(2, n)
//    qDebug() << "Parser: SampleAverage = " << data.stGate.nSimpleAverage;

//    0 ---0.2~30MHz
//    1 ---10~30MHz
//    2 ---15MHz
//    3 ---1~10MHz
//    4 ---10MHz
//    5 ---4MHz
//    6 ---2MHz
//    7 ---1MHz
    data.nProbeBand = obj.probeBand();                //探头频率
    data.nVoltage = obj.transmitVoltage();

    switch (channel)                                                            // 通道对外满足 1-高 0-低
    {
    case 0: data.nImpedance = obj.impedance()== UTSetup::ImpedanceLow ? 0 : 1; break; // 下位机 通道1: 0-高 1-低
    case 1: data.nImpedance = obj.impedance()== UTSetup::ImpedanceHigh ? 2 : 0; break; // 下位机 通道2: 2-高 0-低
    default: data.nImpedance = 0;
    }

    data.fPlus = obj.gain();                                 //增益
    data.fRefPlus = obj.gainRef();                           //参考增益
    data.nEnerge = obj.impedance();
    data.nRevAndSendMode = obj.impedance()== UTSetup::ImpedanceHigh ? 1 : 0;    //收发模式切换 0-一发一收模式 1-自发自收

    data.stGate.nGate1Amp = obj.gate(0).threshold() / 100.0 * 1023.0;//闸门1幅值
    data.stGate.nGate2Amp = obj.gate(1).threshold() / 100.0 * 1023.0;//闸门2幅值
    data.stGate.nGate3Amp = obj.gate(2).threshold() / 100.0 * 1023.0;//闸门3幅值
    data.stGate.nGate4Amp = obj.gate(3).threshold() / 100.0 * 1023.0;//闸门4幅值
    data.stGate.nGate1Enable = obj.gate(0).isEnabled();           //闸门1使能
    data.stGate.nGate2Enable = obj.gate(1).isEnabled();           //闸门2使能
    data.stGate.nGate3Enable = obj.gate(2).isEnabled();           //闸门3使能
    data.stGate.nGate4Enable = obj.gate(3).isEnabled();           //闸门4使能
    data.stGate.nGate1Start = qMax(obj.gate(0).start() - delayTime, 0.0) * AD_Frequency / (data.stGate.nSimpleInterval + 1);//闸门1起点
    data.stGate.nGate2Start = qMax(qMax(obj.gate(1).start(), 0.0) * AD_Frequency / (data.stGate.nSimpleInterval + 1), 1.0); //闸门2起点
    data.stGate.nGate3Start = qMax(qMax(obj.gate(2).start(), 0.0) * AD_Frequency / (data.stGate.nSimpleInterval + 1), 1.0); //闸门3起点
    data.stGate.nGate4Start = qMax(qMax(obj.gate(3).start(), 0.0) * AD_Frequency / (data.stGate.nSimpleInterval + 1), 1.0); //闸门4起点
    /* 以下参数在FPGA上无效 */
    /*
     *  data.stGate.nGate1SyncMode = obj.value("Gate1SyncMode").toInt(0);                   //闸门1同步模式
     *  data.stGate.nGate2SyncMode = obj.value("Gate2SyncMode").toInt(1);                   //闸门2同步模式
     *  data.stGate.nGate3SyncMode = obj.value("Gate3SyncMode").toInt(1);                   //闸门3同步模式
     *  data.stGate.nGate4SyncMode = obj.value("Gate4SyncMode").toInt(1);                   //闸门4同步模式
     *  data.stGate.nGate1SyncThreshold = obj.value("Gate1SyncThreshold").toDouble(400);    //闸门1同步阈值
     *  data.stGate.nGate2SyncThreshold = obj.value("Gate2SyncThreshold").toDouble(400);    //闸门2同步阈值
     *  data.stGate.nGate3SyncThreshold = obj.value("Gate3SyncThreshold").toDouble(400);    //闸门3同步阈值
     *  data.stGate.nGate4SyncThreshold = obj.value("Gate4SyncThreshold").toDouble(400);    //闸门4同步阈值
     * */
    data.stGate.nGate1Width = obj.gate(0).width() * AD_Frequency / (data.stGate.nSimpleInterval + 1);//闸门1宽度
    data.stGate.nGate2Width = obj.gate(1).width() * AD_Frequency / (data.stGate.nSimpleInterval + 1);//闸门2宽度
    data.stGate.nGate3Width = obj.gate(2).width() * AD_Frequency / (data.stGate.nSimpleInterval + 1);//闸门3宽度
    data.stGate.nGate4Width = obj.gate(3).width() * AD_Frequency / (data.stGate.nSimpleInterval + 1);//闸门4宽度
}

//ParameterTable Parser::toStruct(const QByteArray &array, int channel)
//{
//    Config data;
//    memcpy( &data, array.data(), sizeof(data) );
//    ParameterTable obj;
//    obj.insert("Reset", data.stCon.nReset);                                     //通道软复位
//    obj.insert("PRF", data.stCon.nRepeatCounter);                               //重复频率计数器
//    obj.insert("PulseWidth", data.stCon.nMaiKuan);                              //脉宽

//    obj.insert("RectifierMode", data.stConfig.nDetectMode);                     //检波模式
//    obj.insert("RectifierZero", data.stConfig.nValueMiddle);                    //中值
//    obj.insert("GainEnable", data.stConfig.ndBControl);                         //增益转换开关
//    obj.insert("GainRefEnable", data.stConfig.ndBRef);                          //增益控制基准
//    obj.insert("L1Amp", data.stConfig.ndBLevel1);                               //1级增益 AD603
//    obj.insert("L2Amp", data.stConfig.ndBLevel2);                               //2级增益 AD600-1
//    obj.insert("L3Amp", data.stConfig.ndBLevel3);                               //3级增益 AD600-2
//    obj.insert("Suppress", data.stConfig.nSuspend);                             //抑制
//    obj.insert("ZeroOffset", data.stConfig.nMonitorZeroCounter);                //零点修正监视器
//    obj.insert("TcgEnable", bool(data.stConfig.nTcgEnable));                    //DAC使能开关

//    obj.insert("Gate1Enable", data.stGate.nGate1Enable);                        //闸门1使能
//    obj.insert("Gate1SyncMode", data.stGate.nGate1SyncMode);                    //闸门1同步模式
//    obj.insert("Gate1SyncThreshold", data.stGate.nGate1SyncThreshold);          //闸门1同步阈值
//    obj.insert("Gate1Start", data.stGate.nGate1Start);                          //闸门1起点
//    obj.insert("Gate1Width", data.stGate.nGate1Width);                          //闸门1宽度
//    obj.insert("Gate1Amp", data.stGate.nGate1Amp);                              //闸门1幅值
//    obj.insert("Gate2Enable", data.stGate.nGate2Enable);                        //闸门2使能
//    obj.insert("Gate2SyncMode", data.stGate.nGate2SyncMode);                    //闸门2同步模式
//    obj.insert("Gate2SyncThreshold", data.stGate.nGate2SyncThreshold);          //闸门2同步阈值
//    obj.insert("Gate2Start", data.stGate.nGate2Start);                          //闸门2起点
//    obj.insert("Gate2Width", data.stGate.nGate2Width);                          //闸门2宽度
//    obj.insert("Gate2Amp", data.stGate.nGate2Amp);                              //闸门2幅值
//    obj.insert("Gate3Enable", data.stGate.nGate3Enable);                        //闸门3使能
//    obj.insert("Gate3SyncMode", data.stGate.nGate3SyncMode);                    //闸门3同步模式
//    obj.insert("Gate3SyncThreshold", data.stGate.nGate3SyncThreshold);          //闸门3同步阈值
//    obj.insert("Gate3Start", data.stGate.nGate3Start);                          //闸门3起点
//    obj.insert("Gate3Width", data.stGate.nGate3Width);                          //闸门3宽度
//    obj.insert("Gate3Amp", data.stGate.nGate3Amp);                              //闸门3幅值
//    obj.insert("Gate4Enable", data.stGate.nGate4Enable);                        //闸门4使能
//    obj.insert("Gate4SyncMode", data.stGate.nGate4SyncMode);                    //闸门4同步模式
//    obj.insert("Gate4SyncThreshold", data.stGate.nGate4SyncThreshold);          //闸门4同步阈值
//    obj.insert("Gate4Start", data.stGate.nGate4Start);                          //闸门4起点
//    obj.insert("Gate4Width", data.stGate.nGate4Width);                          //闸门4宽度
//    obj.insert("Gate4Amp", data.stGate.nGate4Amp);                              //闸门4幅值
//    obj.insert("SampleInterval", data.stGate.nSimpleInterval);                  //通道采样间隔
//    obj.insert("SampleAverage", data.stGate.nSimpleAverage);                    //通道采样平均数
//    obj.insert("Delay", data.stGate.nDelay);                                    //通道延迟时间

//    obj.insert("Range", data.fRange);                                           //声程
//    obj.insert("Velocity", data.nVelocity);                                     //声速
//    obj.insert("ProbeBand", data.nProbeBand);                                   //探头频率
//    obj.insert("TransmitVoltage", data.nVoltage);                               //电压
//    if (channel == 0)
//    {
//        if (data.nImpedance == 0)
//        {
//            obj.insert("Impedance", 1);
//        }
//        else
//        {
//            obj.insert("Impedance", 0);
//        }
//    }
//    else
//    {
//        if (data.nImpedance == 0)
//        {
//            obj.insert("Impedance", 0);
//        }
//        else
//        {
//            obj.insert("Impedance", 1);
//        }
//    }
//    obj.insert("Gain", data.fPlus);                                             //增益
//    obj.insert("GainRef", data.fRefPlus);                                       //参考增益
//    obj.insert("TransmitEnergy", data.nEnerge);                                 //高低能量切换，0为低能量，1为高能量,分通道
//    obj.insert("TransmitMode", data.nRevAndSendMode);                           //收发模式切换，0为一发一收模式，1为自发自收模式
//    obj.insert("GainCompensation11", data.fPlusAddX11);                         //增益补偿 X11
//    obj.insert("GainCompensation12", data.fPlusAddX12);                         //增益补偿 X12
//    obj.insert("GainCompensation21", data.fPlusAddX21);                         //增益补偿 X21
//    obj.insert("GainCompensation22", data.fPlusAddX22);                         //增益补偿 X22

//    return obj;
//}

int Parser::GetSampleInterval()
{
    return nSampleInterval;
}

int Parser::GetTcgRamSize()
{
    return TCG_RAM_Size;
}
/*!
 * \brief Parser::UpdateChannelOffset 更新通道偏移
 * \param offset
 */
void Parser::UpdateChannelOffset(const UTPose &offset)
{
    poseOffset.resize(qMax(offset.size(), poseOffset.size()));
    for (int index = 0; index < poseOffset.count(); ++index) {
        poseOffset[index] += offset.value(index);
    }
}
