#pragma once

#include <QByteArray>
#include <QList>

namespace Ars500 {

typedef qint32 CommandInt;
typedef QVector<float> TCGData;

//! 指令集
enum Command
{
    CMD_CURRENT_CHANNEL        = 0x0001,                                        ///< 当前通道
    CMD_WORKMODE               = 0x0011,                                        ///< 工作模式
    CMD_ALL_PARAM_SET          = 0x0601,                                        ///< 上位机设置全部参数
    CMD_RESET                  = 0x0701,                                        ///< 上位机复位请求
    CMD_ALL_PARAM_REQ          = 0x1001,                                        ///< 上位机获取全部参数请求
    CMD_ASCAN                  = 0x1002,                                        ///< A扫实时数据请求 特征数据请求（A扫(3333端口)和C扫（4444端口）使用的端口不同，能够区分开来），必须在程序开始时分别向两个端口发送该命令
    CMD_VERSION_REQ            = 0x1003,                                        ///< 版本号请求
    CMD_WORK_MODE_SET          = 0x1004,                                        ///< 设置工作模式  从1开始：单1;单2;双单;双晶反射;双晶对射;单晶关联;
    CMD_TCG_SWITCH             = 0x1005,                                        ///< 设置TCG曲线
    CMD_ASCAN_DATA_STORE       = 0x1006,                                        ///< A扫存储数据请求
    CMD_ASCAN_RET_WAVE         = 0x2001,                                        ///< A扫实时数据上传 特征数据上传（A扫(3333端口)和C扫（4444端口）使用的端口不同，能够区分开来）
    CMD_ASCAN_RET_DATA_STORE   = 0x2006,                                        ///< A扫存储数据上传
    CMD_SET_PARAM_RET          = 0xff01                                         ///< 设置参数返回指令
};

//! 网络数据自定义包头
struct PacketHead
{
    CommandInt lCommand;                                                        ///< 命令
    qint16 nChannelIndex;                                                       ///< 通道号
    qint16 nGateIndex;                                                          ///< 闸门号
    qint32 lSize;                                                               ///< 该命令大小
};

//! 版本信息
struct VersionData
{
    float wHardWareVer;                                                         // 硬件版本
    float wSoftWareVer;                                                         // 软件版本
};

//! TCG包头
struct TCGCommuData
{
    int bOpenFlag;
    int nSramNum;
    qint16 nSamplingInterval;                                                   ///<采样间隔
//        TCGData vecSramGain;
};

//! 通道基本参数
struct ChannelConfig
{
    qint16 nReset;                                                              ///< 通道软复位
    qint16 nRepeatCounter;                                                      ///< 重复频率计数器
    qint16 nMaiKuan;                                                            ///< 脉宽
};

//! 通道配置参数
struct ChannelConfigReg
{
    qint16 nDetectMode;                                                         ///< 检波模式
    qint16 nValueMiddle;                                                        ///< 中值
    qint16 ndBControl;                                                          ///< 增益转换开关
    qint16 ndBRef;                                                              ///< 增益控制基准
    qint16 ndBLevel1;                                                           ///< 1级增益 AD603
    qint16 ndBLevel2;                                                           ///< 2级增益 AD600-1
    qint16 ndBLevel3;                                                           ///< 3级增益 AD600-2
    qint16 nSuspend;                                                            ///< 抑制
    qint16 nMonitorZeroCounter;                                                 ///< 零点修正监视器
    qint16 nTcgEnable;                                                          ///< DAC使能开关
};

//! 闸门参数
struct ChannelGateConfig
{
    qint16 nGate1Enable;                                                        ///< 闸门1使能
    qint16 nGate1SyncMode;                                                      ///< 闸门1同步模式
    qint16 nGate1SyncThreshold;                                                 ///< 闸门1同步阈值
    qint16 nGate1Start;                                                         ///< 闸门1起点
    qint16 nGate1Width;                                                         ///< 闸门1宽度
    qint16 nGate1Amp;                                                           ///< 闸门1幅值
    qint16 nGate2Enable;                                                        ///< 闸门2使能
    qint16 nGate2SyncMode;                                                      ///< 闸门2同步模式
    qint16 nGate2SyncThreshold;                                                 ///< 闸门2同步阈值
    qint16 nGate2Start;                                                         ///< 闸门2起点
    qint16 nGate2Width;                                                         ///< 闸门2宽度
    qint16 nGate2Amp;                                                           ///< 闸门2幅值
    qint16 nGate3Enable;                                                        ///< 闸门3使能
    qint16 nGate3SyncMode;                                                      ///< 闸门3同步模式
    qint16 nGate3SyncThreshold;                                                 ///< 闸门3同步阈值
    qint16 nGate3Start;                                                         ///< 闸门3起点
    qint16 nGate3Width;                                                         ///< 闸门3宽度
    qint16 nGate3Amp;                                                           ///< 闸门3幅值
    qint16 nGate4Enable;                                                        ///< 闸门4使能
    qint16 nGate4SyncMode;                                                      ///< 闸门4同步模式
    qint16 nGate4SyncThreshold;                                                 ///< 闸门4同步阈值
    qint16 nGate4Start;                                                         ///< 闸门4起点
    qint16 nGate4Width;                                                         ///< 闸门4宽度
    qint16 nGate4Amp;                                                           ///< 闸门4幅值
    qint16 nSimpleInterval;                                                     ///< 通道采样间隔
    qint16 nSimpleAverage;                                                      ///< 通道采样平均数
    qint16 nDelay;                                                              ///< 通道延迟时间
};

/*! 仪器参数 */
struct Config
{
    ChannelConfig stCon;
    ChannelConfigReg stConfig;
    ChannelGateConfig stGate;
    float fRange;                                                               ///< 声程
    int nVelocity;                                                              ///< 声速
    // qint16 wWorkMode;                                                       // 工作模式
    qint16 nProbeBand;                                                          ///< 探头频率
    qint16 nVoltage;                                                            ///< 电压
    qint16 nImpedance;                                                          ///< 阻抗
    float fPlus;                                                                ///< 增益
    float fRefPlus;                                                             ///< 参考增益
    qint16 nEnerge;                                                             ///< 高低能量切换，0为低能量，1为高能量,分通道
    qint16 nRevAndSendMode;                                                     ///< 收发模式切换，0为一发一收模式，1为自发自收模式
    qint16 nSPIAddr;                                                            ///< SPI地址选择 串行D/A 8路通道选择 数字1-8代表
    qint16 nSPIData;                                                            ///< 0x0000~0x3FFF：其中0x0000为-6V，0x3FFF为+6V
    float fPlusAddX11;                                                          ///< 增益补偿 X11
    float fPlusAddX12;                                                          ///< 增益补偿 X12
    float fPlusAddX21;                                                          ///< 增益补偿 X21
    float fPlusAddX22;                                                          ///< 增益补偿 X22
};

/*! 特征值数据 */
struct FeatureData
{
//        qint32 nX;
//        qint32 nY;
//        qint32 nZ;
//        qint32 nW1;
//        qint32 nA;
//        qint32 nB;
//        qint32 nW2;
    qint16 nXLow16;                                                             ///< X方向低16位
    qint16 nXHigh16;                                                            ///< X方向高16位
    qint16 nYLow16;                                                             ///< Y方向低16位
    qint16 nYHigh16;                                                            ///< Y方向高16位
    qint16 nZLow16;                                                             ///< z方向低16位
    qint16 nZHigh16;                                                            ///< z方向高16位
    qint16 nW1Low16;                                                            ///< W1方向低16位
    qint16 nW1High16;                                                           ///< W1方向高16位
    qint16 nALow16;                                                             ///< A方向低16位
    qint16 nAHigh16;                                                            ///< A方向高16位
    qint16 nBLow16;                                                             ///< B方向低16位
    qint16 nBHigh16;                                                            ///< B方向高16位
    qint16 nW2Low16;                                                            ///< W2方向低16位
    qint16 nW2High16;                                                           ///< W2方向高16位
    qint16 nGate1SyncTime;                                                      ///< 闸门1同步时间
    qint16 nGate1MPTime;                                                        ///< 闸门1内最大波峰值时间
    qint16 nGate1MPAmp;                                                         ///< 闸门1内最大波峰值幅度
    qint16 nGate2MPTime;                                                        ///< 闸门2内最大波峰值时间
    qint16 nGate2MPAmp;                                                         ///< 闸门2内最大波峰值幅度
    qint16 nGate2FPFirstTime;                                                   ///< 闸门2内第一个波峰过阈值时间
    qint16 nGate2FPTime;                                                        ///< 闸门2内第一个波峰波峰值时间
    qint16 nGate2FPAmp;                                                         ///< 闸门2内第一个波峰波峰值幅度
    qint16 nGate3MPTime;                                                        ///< 闸门3内最大波峰值时间
    qint16 nGate3MPAmp;                                                         ///< 闸门3内最大波峰值幅度
    qint16 nGate3FPFirstTime;                                                   ///< 闸门3内第一个波峰过阈值时间
    qint16 nGate3FPTime;                                                        ///< 闸门3内第一个波峰波峰值时间
    qint16 nGate3FPAmp;                                                         ///< 闸门3内第一个波峰波峰值幅度
    qint16 nGate4MPTime;                                                        ///< 闸门4内最大波峰值时间
    qint16 nGate4MPAmp;                                                         ///< 闸门4内最大波峰值幅度
    qint16 nGate4FPFirstTime;                                                   ///< 闸门4内第一个波峰过阈值时间
    qint16 nGate4FPTime;                                                        ///< 闸门4内第一个波峰波峰值时间
    qint16 nGate4FPAmp;                                                         ///< 闸门4内第一个波峰波峰值幅度
};

static const int ASCAN_PORT = 2222;                                             ///< A扫数据通道的端口号
static const int MONITOR_PORT = 3333;                                           ///< 监视通道的端口号
static const int CSCAN_PORT = 4444;                                             ///< C扫数据通道的端口号
static const int PARAM_PORT = 5555;                                             ///< 参数下发通道的端口号
static const int GENERAL_PORT = 6666;
static const char PACKET_SIGNATURE[] = "\033\000ARS";
static const size_t GENERAL_MIN_PACKET_SIZE = sizeof(PACKET_SIGNATURE) + sizeof(quint32) - 1;

static const int A_SCAN_NUM_SAMPLES_PER_FRAME = 512;                            ///< 每帧A扫数据的幅值样本数
static const int A_SCAN_NUM_FRAMES_PER_PACKET = 30;                             ///< A扫数据通道，每数据包中的帧数
static const int A_SCAN_NUM_FRAMES_PER_PACKET_COMPRESSED = 60;                  ///< 被压缩的A扫数据通道，没数据包中的帧数
static const int C_SCAN_NUM_FRAMES_PER_PACKET = 128;                            ///< C扫数据通道，每数据包中的帧数

static const int NUM_GATES = 4;                                                 ///< 仪器的闸门数
static const int NUM_AXES = 7;                                                  ///< 仪器上发的机械轴数量
static const int NUM_FEATURES = 18;                                             ///< 仪器上发的特征值数量
static const int NUM_CHANNELS = 2;                                              ///< 仪器的通道数

//! 监视数据
struct FullData
{
    qint16 nPicData[A_SCAN_NUM_SAMPLES_PER_FRAME];
    FeatureData stFeatureData;
};

typedef qint16 ADataValueRaw;
typedef qint16 CDataValueRaw;
typedef qint32 PoseValueRaw;
static const int BYTES_FRAME_ASCAN = sizeof(ADataValueRaw) * A_SCAN_NUM_SAMPLES_PER_FRAME;
static const int BYTES_FRAME_POSE = sizeof(PoseValueRaw) * NUM_AXES;
static const int BYTES_FRAME_FEATURE = sizeof(CDataValueRaw) * NUM_FEATURES;
static const int BYTES_FRAME_CDATA = sizeof(FeatureData);

static const int BYTES_PACKET_BODY_MONITOR = sizeof(FullData);
static const int BYTES_PACKET_MONITOR = sizeof(PacketHead) + sizeof(FullData);

static const int BYTES_PACKET_BODY_ASCAN = sizeof(FullData) * A_SCAN_NUM_FRAMES_PER_PACKET;
static const int BYTES_PACKET_ASCAN = sizeof(PacketHead) + BYTES_PACKET_BODY_ASCAN;
static const int BYTES_PACKET_BODY_ASCAN_COMPRESSED = (sizeof(qint8) * A_SCAN_NUM_SAMPLES_PER_FRAME + sizeof(FeatureData)) * A_SCAN_NUM_FRAMES_PER_PACKET_COMPRESSED;
static const int BYTES_PACKET_ASCAN_COMPRESSED = sizeof(PacketHead) + BYTES_PACKET_BODY_ASCAN_COMPRESSED;

static const int BYTES_PACKET_BODY_CSCAN = sizeof(FeatureData) * C_SCAN_NUM_FRAMES_PER_PACKET;
static const int BYTES_PACKET_CSCAN = sizeof(PacketHead) + BYTES_PACKET_BODY_CSCAN;

enum SocketType
{
    AScan = 0,
    Monitor,
    CScan,
    Parameter
};

/*! C扫特征值的类型，标识特征值的单位 */
enum CDataTypeEnum
{
    DATA_TYPE_TIME,                                                             ///< 时间 (μs)
    DATA_TYPE_AMP                                                               ///< 幅值 (%)
};
typedef QList<CDataTypeEnum> CDataTypeList;
static const CDataTypeList CDataType = {                                        ///< C扫描特征值的物理量类型
    DATA_TYPE_TIME,                                                             // 0    Gate1SyncTime
    DATA_TYPE_TIME,                                                             // 1    Gate1MPTime
    DATA_TYPE_AMP,                                                              // 2    Gate1MPAmp
    DATA_TYPE_TIME,                                                             // 3    Gate2MPTime
    DATA_TYPE_AMP,                                                              // 4    Gate2MPAmp
    DATA_TYPE_TIME,                                                             // 5    Gate2FPFirstTime
    DATA_TYPE_TIME,                                                             // 6    Gate2FPTime
    DATA_TYPE_AMP,                                                              // 7    Gate2FPAmp
    DATA_TYPE_TIME,                                                             // 8    Gate3MPTime
    DATA_TYPE_AMP,                                                              // 9    Gate3MPAmp
    DATA_TYPE_TIME,                                                             // 10   Gate3FPFirstTime
    DATA_TYPE_TIME,                                                             // 11   Gate3FPTime
    DATA_TYPE_AMP,                                                              // 12   Gate3FPAmp
    DATA_TYPE_TIME,                                                             // 13   Gate4MPTime
    DATA_TYPE_AMP,                                                              // 14   Gate4MPAmp
    DATA_TYPE_TIME,                                                             // 15   Gate4FPFirstTime
    DATA_TYPE_TIME,                                                             // 16   Gate4FPTime
    DATA_TYPE_AMP                                                               // 17   Gate4FPAmp
};

}                                                                               /* namespace ARS500 */
