#ifndef PIPEDATADEF
#define PIPEDATADEF

#include <QMap>
#include <QList>
#include <QByteArray>
#include <QJsonObject>
#include <QSharedPointer>

#define GATE_NUM 4
#define SUB_CONFIG_NUM 2

namespace PipeDetect {

enum StandardType
{
    SteelPlate,
    SteelBar,
    SteelPipe
};

enum SteelPlateStandard
{
    GBT_2970,
    BS_EN_ISO_10893
};

enum SteelPipeStandard
{
    GBT_5777_2008,
    GBT_31925_2015
};

struct ThickInfo
{
    bool        isOpen              = 0;                // 测厚开关
    qreal       calibrationMax      = 0;                // 增厚校准
    qreal       calibrationMin      = 0;                // 减薄校准
    qint32      thickMaxAverage     = 1;                // 增厚平均数
    qint32      thickMinAverage     = 1;                // 减薄平均数
};

struct ProcessInfo
{
    bool        isMark              = 0;                // 喷标开关
    int         paintInterval       = 10;               // 喷标间隔 mm 用于除数，取整
};

struct OrderInfo
{
    qreal       materialDiameter    = 300;              // 材料直径 mm
    qreal       materialLength      = 10000;            // 材料长度 mm
    qreal       materialThick       = 10;               // 材料壁厚 mm
    qreal       thickShowMax        = 12.5;             // 壁厚显示最大值
    qreal       thickShowMin        = 7.5;              // 壁厚显示最小值
    qreal       thickMax            = 11;               // 允许的最大壁厚
    qreal       thickMin            = 9;                // 允许的最小壁厚
    qreal       showStart           = 0;                // 显示的范围最小值
    qreal       showEnd             = 10000;            // 显示的范围最大值
    qint32      orderCount          = 0;                // 订单数量
    QString     orderNumber         = "";               // 订单编号
    QString     workerName          = "";               // 工人姓名
    QString     workerClasses       = "";               // 工人班次
    QString     materialNumber      = "";               // 材料编号
    QString     materialLevel       = "";               // 材料级别
    QString     testStandard        = "GBT_5777_2008";  // 检测标准
    QString     testLevel           = "III";            // 检测等级
};

struct AllParameterInfo
{
    bool        isOver              = 0;                // 一次检测完成标志,用于断电续扫权限判定
    OrderInfo   orderInfo;                              // 订单参数
    ProcessInfo processInfo;                            // 工艺参数
};

struct ChannelInfo
{
    qint32  index = 0;
    qint32  probeBoxIndex = 0;
    qint32  probeNo = 0;
    qint32  channelNo = 0;
    qint32  hardChannel = 0;
    qint32  softChannel = 0;
    qint32  txCardNo = 0;
    qint32  txChannelIn = 0;
    qint32  txChannelOut = 0;
    qreal   angleCorrect = 0;
    QString name = "";
    QString probeName = "";
    bool    isFlaw = false;
};
typedef QMap<qint32, ChannelInfo> ChannelInfoMap;

typedef qint32 CommandInt;
typedef QJsonObject ParameterTable;
typedef QVector<float> TCGData;

// 指令集
enum Command
{
    CMD_CURRENT_CHANNEL        = 0x0001,                                    // 当前通道
    CMD_SCAN_STATUS            = 0x0007,                                    // 扫查状态
    CMD_SIMULATE_SET           = 0x0008,                                    // 电气模拟设置
    CMD_WORKMODE               = 0x0011,                                    // 工作模式
    CMD_ALL_PARAM_SET          = 0x0601,                                    // 上位机设置全部参数
    CMD_RESET                  = 0x0701,                                    // 上位机复位请求
    CMD_ALL_PARAM_REQ          = 0x1001,                                    // 上位机获取全部参数请求
    CMD_ASCAN                  = 0x1002,                                    // A扫实时数据请求 特征数据请求（A扫(3333端口)和C扫（4444端口）使用的端口不同，能够区分开来），必须在程序开始时分别向两个端口发送该命令
    CMD_VERSION_REQ            = 0x1003,                                    // 版本号请求
    CMD_WORK_MODE_SET          = 0x1004,                                    // 设置工作模式  从1开始：单1;单2;双单;双晶反射;双晶对射;单晶关联;
    CMD_TCG_SWITCH             = 0x1005,                                    // 设置TCG曲线
    CMD_ASCAN_DATA_STORE       = 0x1006,                                    // A扫存储数据请求
    CMD_ASCAN_STOP             = 0x1007,
    CMD_TEST_START_FLAG        = 0x1008,                                    // 探伤开始标志
    CMD_CODER_MARK             = 0X1009,                                    // 编码器切换标志
    CMD_SET_PRF                = 0x5000,                                    // 设置PRF
    CMD_ASCAN_RET_WAVE         = 0x2001,                                    // A扫实时数据上传 特征数据上传（A扫(3333端口)和C扫（4444端口）使用的端口不同，能够区分开来）
    CMD_CSCAN_DATA             = 0x2002,                                    // C扫数据
    CMD_ASCAN_RET_DATA_STORE   = 0x2006,                                    // A扫存储数据上传
    CMD_SET_PARAM_RET          = 0xff01,                                    // 设置参数返回指令
    CMD_LOWER_PLX_REQ          = 0x3001,                                    // 请求下位机PLX状态
    CMD_EYES_STATUS_REQ        = 0x3002,                                    // 给下位机设置请求传感器状态的上发的开关
    CMD_LOWER_PLX_ANSWER       = 0x4001,                                    // 下位机状态应答
    CMD_EYES_STATUS_ANSWER     = 0x4002,                                    // 下位机传输当前传感器触发状态
    CMD_HEART                  = 0x1111,                                    // 心跳包
    CMD_FLAW_OR_BOTTOM         = 0x1200                                     // 1号AEC卡8个硬通道伤波底波切换标志
};

struct GATE_INFO
{
    quint16 GateEnable;         // 软通道底波闸门使能寄存器
    float GateStart;            // 软通道底波闸门闸门起始值
    float GateWidth;            // 软通道底波闸门闸门宽度
    float GateAmp;              // 软通道底波闸门闸门幅度
};

struct CH_SUB_CONFIG
{
    GATE_INFO stGate[GATE_NUM];     // 闸门I, 闸门A, 闸门B, 闸门I2
    float Delay;                    // 延时
    float Suppression;              // 抑制
    quint16 MiddleValue;            // 中值
    quint16 DetectMode;             // 检波模式
};

struct CH_CONFIG
{
    float Plus;                                     // 增益
    float CPlus;                                    // 参考增益
    float Range;                                    // 声程
    qint32 Velocity;                                // 声速
    quint16 TCGEnable;                              // 软通道 TCG使能寄存器
    CH_SUB_CONFIG stWaveConfigure[SUB_CONFIG_NUM];  // 0底波通道设置、1伤波通道设置
    quint16 ProbeNo;                                // 探头编号
};

typedef struct
{
    quint16 wFlawTime;
    quint16 wFlawHeight;
}GATE_FEATURE_DATA;

typedef struct
{
    GATE_FEATURE_DATA stGateFeature[4];
}CH_FEATURE_DATA;

typedef struct
{
    CH_FEATURE_DATA stFlawFeature;
    CH_FEATURE_DATA stSurFeature;
}CH_L_FEATURE_DATA;

struct GateFeature
{
    quint16 time;
    quint16 amp;
};

struct CH_FEATURE_DATA_1To6
{
    GateFeature flawFeature[4];
};

struct CH_FEATURE_DATA_7To8
{
    GateFeature surfaceFeature[4];
    GateFeature flawFeature[4];
};

struct ChannelFeature
{
    GateFeature feature[4];
};

struct CH_C_FEATURE_DATA
{
    ChannelFeature flaw1;       // wCh1ProbeNo1
    ChannelFeature flaw2;       // wCh1ProbeNo2
    ChannelFeature flaw3;       // wCh1ProbeNo3
    ChannelFeature flaw4;       // wCh1ProbeNo4
    ChannelFeature flaw5;       // wCh1ProbeNo5
    ChannelFeature flaw6;       // wCh1ProbeNo6
    ChannelFeature surface7;    // wCh1ProbeNo7 + 5
    ChannelFeature flaw7;       // wCh1ProbeNo7
    ChannelFeature surface8;    // wCh1ProbeNo8 + 5
    ChannelFeature flaw8;       // wCh1ProbeNo8
};

typedef struct
{
//    quint16 coderXLow;
//    quint16 coderXHigh;
    qint32 coderX;
    quint16 softChannelNum;         // 软通道号
    quint16 pxiIdx;                 // 数据标签位 用做PXI编号
    CH_C_FEATURE_DATA featureData;
    quint16 wCh1ProbeNo;
    quint16 wCh2ProbeNo;
    quint16 wCh3ProbeNo;
    quint16 wCh4ProbeNo;
    quint16 wCh5ProbeNo;
    quint16 wCh6ProbeNo;
    quint16 wCh7ProbeNo;
    quint16 wCh8ProbeNo;
//    quint16 coderRLow;
//    quint16 coderRHigh;
    qint32 coderR;
}FEATURE_DATA;

static const int C_SCAN_NUM_FRAMES_PER_PACKET = 128;                            ///< C扫数据通道，每数据包中的帧数

typedef struct
{
    FEATURE_DATA featurs[C_SCAN_NUM_FRAMES_PER_PACKET];
}FEATURE_DATA_S;

static const qint32 LIGHT_EYE_VAR_NUM = 9;
typedef struct tagSIMULATION_DATA
{
    quint16 sEnable;                        // 模拟使能
}SIMULATION_DATA_t;       // 模拟数据


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

// 精简版特征数据
struct FeatureData
{
    quint16 gateIMPTime;                                                // 闸门1内最大波峰值时间高
    quint16 gateIMPAmp;                                                 // 闸门1内最大波峰值幅度
    quint16 gateAMPTime;                                                // 闸门2内最大波峰值时间
    quint16 gateAMPAmp;                                                 // 闸门2内最大波峰值幅度
    quint16 gateBMPTime;                                                // 闸门3内最大波峰值时间
    quint16 gateBMPAmp;                                                 // 闸门3内最大波峰值幅
    quint16 gateI2MPTime;                                               // 闸门I2内最大波峰值时间高
    quint16 gateI2MPAmp;                                                // 闸门I2内最大波峰值幅度
    quint16 probeNo;                                                    // 探头号
    quint16 coderXLow;
    quint16 coderXHigh;
    quint16 coderRLow;
    quint16 coderRHigh;
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

static const int NUM_GATES = 4;                                                 ///< 仪器的闸门数
static const int NUM_AXES = 2;                                                  ///< 仪器上发的机械轴数量
static const int NUM_FEATURES = 8;                                             ///< 仪器上发的特征值数量
static const int NUM_FLAW_CHANNELS = 25;
static const int NUM_SURFACE_CHANNELS = 5;
static const int NUM_CHANNELS = NUM_FLAW_CHANNELS + NUM_SURFACE_CHANNELS;       // 仪器的通道数
static const int NUM_FEATURES_ADATA = 13;                                        // A扫特征值个数
static const int NUM_FEATURES_CDATA = 94;                                       // 特征值个数为94个



//! 监视数据
struct FullData
{
    qint16 nPicData[A_SCAN_NUM_SAMPLES_PER_FRAME];
    FeatureData stFeatureData;
};

typedef qint16 ADataValueRaw;
typedef qint16 CDataValueRaw;
typedef qint32 PoseValueRaw;
typedef qint32 UTPoseValueRaw;

static const int BYTES_FRAME_POSE = sizeof(PoseValueRaw) * NUM_AXES;
static const int BYTES_PACKET_BODY_MONITOR = sizeof(FullData);
static const int BYTES_PACKET_MONITOR = sizeof(PacketHead) + sizeof(FullData);

static const int BYTES_PACKET_BODY_ASCAN_COMPRESSED = (sizeof(qint8) * A_SCAN_NUM_SAMPLES_PER_FRAME + sizeof(FeatureData)) * A_SCAN_NUM_FRAMES_PER_PACKET_COMPRESSED;
static const int BYTES_PACKET_ASCAN_COMPRESSED = sizeof(PacketHead) + BYTES_PACKET_BODY_ASCAN_COMPRESSED;

static const int BYTES_FRAME_ASCAN = sizeof(ADataValueRaw) * A_SCAN_NUM_SAMPLES_PER_FRAME;
static const int BYTES_FRAME_FEATURE_ADATA = sizeof(CDataValueRaw) * NUM_FEATURES_ADATA;
static const int BYTES_FRAME_FEATURE_CDATA = sizeof(CDataValueRaw) * NUM_FEATURES_CDATA;
static const int BYTES_FRAME_CDATA = sizeof(FeatureData);

static const int BYTES_PACKET_BODY_ASCAN = sizeof(FullData) * A_SCAN_NUM_FRAMES_PER_PACKET;
static const int BYTES_PACKET_ASCAN = sizeof(PacketHead) + BYTES_PACKET_BODY_ASCAN;

static const int BYTES_PACKET_BODY_CSCAN = sizeof(FEATURE_DATA) * C_SCAN_NUM_FRAMES_PER_PACKET;
static const int BYTES_PACKET_CSCAN = sizeof(PacketHead) + BYTES_PACKET_BODY_CSCAN;

static const int BYTES_PACKET_BODY_PARAMETER = sizeof(int);
static const int BYTES_PACKET_PARAMETER = sizeof(PacketHead) + BYTES_PACKET_BODY_PARAMETER;

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

typedef QSharedPointer<PipeDetect::SIMULATION_DATA_t> UTSharedDataSimulation;

#endif // PIPEDATADEF
