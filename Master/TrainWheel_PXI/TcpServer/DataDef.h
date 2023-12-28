#ifndef DATADEF_H
#define DATADEF_H

#include <QByteArray>
#include "ARS3D/UTData.h"
#include "UTInstrumentDriver.h"
#include "const.h"

#define FEATURE_SINGLE_CHANNEL 0    //1:1 C扫特征值是否只含单个通道
#define FLANGE_ANALYSIS   1         // 轮缘统计分析
//#define CSCAN_DELETE_FRAMS 1        // 传感器切换时（继电器切换），去掉头几帧
//#define GATE_NUM 4
#define SUB_CONFIG_NUM 2
#define AEC_CHN_NUM 8
#define ASCAN_PORT              2222
#define MONITOR_PORT            3333
#define CSCAN_PORT              4444
#define PARAM_PORT              5555
#define EYES_STATUS_PORT        6666
#define MFC_EYES_STATUS_PORT    7777
#define LUA_PORT                8888

typedef enum
{
    SURFACE = 0,    // 界面波通道
    FLAW            // 伤波通道
}SURFACE_FLAW_e;

typedef enum
{
    I1 = 0,
    A,
    B,
    I2
}GATE_e; //

namespace NS_PXI100 {
    typedef UTInstrumentDriver::DataLevel DataLevel;
    typedef qint32 CommandInt;
    typedef UTInstrumentDriver::ParameterTable ParameterTable;
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
        CMD_ASCN_STOP              = 0x1007,                                    // A扫实时数据 特征数据 暂停
        CMD_RPF_SET                = 0x1100,                                    // 重复频率
        CMD_SENSOR_ROLL_SET        = 0x1101,                                    // 传感器轮循使能设置
        CMD_ASCAN_RET_WAVE         = 0x2001,                                    // A扫实时数据上传 特征数据上传（A扫(3333端口)和C扫（4444端口）使用的端口不同，能够区分开来）
        CMD_ASCAN_RET_DATA_STORE   = 0x2006,                                    // A扫存储数据上传
        CMD_SET_PARAM_RET          = 0xff01,                                    // 设置参数返回指令
        CMD_LOWER_PLX_REQ          = 0x3001,                                    // 请求下位机PLX状态
        CMD_EYES_STATUS_REQ        = 0x3002,                                    // 给下位机设置请求传感器状态的上发的开关
        CMD_LOWER_PLX_ANSWER       = 0x4001,                                    // 下位机状态应答
        CMD_EYES_STATUS_ANSWER     = 0x4002,                                    // 下位机传输当前传感器触发状态
        CMD_VERSION_ANSWER         = 0x4003,                                    // 版本信息应答
        CMD_MFC_DATA               = 0x4004,                                    // MFC卡数据
        CMD_HEART                  = 0x1111                                     // 心跳包
    };

    // 网络数据自定义包头
    struct PacketHead
    {
        CommandInt lCommand;                                                    // 命令
        qint16 nChannelIndex;                                                   // 通道号
        qint16 nGateIndex;                                                      // 闸门号
        qint32 nIdentification;                                                 // 识别码
        qint32 lSize;                                                           // 该命令大小
    };

    // TCG包头
    struct TCGCommuData
    {
        bool bOpenFlag;
        int nSramNum;
        qint16 nSamplingInterval;//采样间隔
//        TCGData vecSramGain;
    };

    static const qint32 LIGHT_EYE_VAR_NUM = 12;
    typedef struct tagLIGHT_EYE
    {
        // 15-0 31-16 47-17 63-48 79-64 79-64 95-80 111-96 127-112
        quint16 usLightEye[LIGHT_EYE_VAR_NUM];
    }LIGHT_EYE_t;   // 电气光眼信号 有128个I/O中,由8个16位来表示

    struct SIMULATION_DATA_t
    {
        SIMULATION_DATA_t(){};
        ~SIMULATION_DATA_t(){};
        quint16 usEnable;                         // 模拟使能
        LIGHT_EYE_t stLightEye;                 // 电气光眼信号数据
    };       // 模拟数据


    struct GATE_INFO
    {
        quint16 GateEnable;         // 软通道底波闸门使能寄存器
        float GateStart;            // 软通道底波闸门闸门起始值
        float GateWidth;            // 软通道底波闸门闸门宽度
        float GateAmp;              // 软通道底波闸门闸门幅度
    };

    struct CH_SUB_CONFIG
    {
        GATE_INFO stGate[GATE_NUM];    // 闸门I, 闸门A, 闸门B, 闸门I2
        float Delay;           // 延时
        float Suppression;     // 抑制
        quint16 MiddleValue;     // 中值
        quint16 DetectMode;      // 检波模式
    };

    struct CH_CONFIG
    {
        float Plus;                                     // 增益
        float CPlus;                                    // 参考增益
        float Range;                                    // 声程
        float Delay;
        qint32 Velocity;                                // 声速
        quint16 TCGEnable;                              // 软通道 TCG使能寄存器
        CH_SUB_CONFIG stWaveConfigure[SUB_CONFIG_NUM];	// 0底波通道设置、1伤波通道设置
        quint16 ProbeNo;                                // 探头编号
//        quint16 eyeArea;                                //光眼区域0-7(16个光眼一个区域)
    };

    typedef struct
    {
        quint16 wAPlus;	// 软通道通道A增益
        quint16 wBPlus;	// 软通道通道B增益
        quint16 wCPlus;	// 软通道通道C增益
        quint16 wDPlus;	// 软通道通道D增益
    } PLUS;

    typedef struct
    {
        quint16 GateEnable;      // 软通道底波闸门使能寄存器
        quint16 GateStart;       // 软通道底波闸门闸门起始值
        quint16 GateWidth;       // 软通道底波闸门闸门宽度
        quint16 GateAmp;         // 软通道底波闸门闸门幅度
    }GATE_INFO_HARD;

    typedef struct
    {
        quint16 GateEnable;         // 软通道底波闸门使能寄存器
        quint16 GateStartLo;        // 软通道底波闸门闸门起始值低16位
        quint16 GateStartHi;        // 软通道底波闸门闸门起始值高16位
        quint16 GateWidth;          // 软通道底波闸门闸门宽度
        quint16 GateAmp;            // 软通道底波闸门闸门幅度
    }GATE_I_INFO_HARD;

    typedef struct
    {
     //   GATE_INFO_HARD stGate[GATE_NUM];    // 闸门I, 闸门A, 闸门B, 闸门I2
        GATE_I_INFO_HARD stGateI1;  // 闸门I1
        GATE_INFO_HARD stGateA;     // 闸门A
        GATE_INFO_HARD stGateB;     // 闸门B
        GATE_I_INFO_HARD stGateI2;  // 闸门I2
        quint16 DelayLo;           // 延时低16位
        quint16 DelayHi;           // 延时低16位
        quint16 MiddleValue;     // 中值
        quint16 DetectMode;      // 检波模式
        quint16 Suppression;     // 抑制
    }CH_SUB_CONFIG_HARD;

    typedef struct
    {
        PLUS stPlus;                                            // 增益
        quint16 wBottomFlawPlusDiff;                            // 软通道通道伤波和底波增益差
        quint16 wTCGEn;                                         // 软通道 TCG使能寄存器
        CH_SUB_CONFIG_HARD	stWaveConfigure[SUB_CONFIG_NUM];	// 底波通道设置、伤波通道设置
        quint16 ProbeNo;                                        // 探头编号
//        quint16 eyeArea;                                      //光眼区域0-7(16个光眼一个区域)
    }CH_CONFIG_HARD;

    ///////////////////////////////

    // 精简版特征数据
    struct FeatureData
    {
        quint16 nGateIMPTimeLo;                                                  // 闸门1内最大波峰值时间低
        quint16 nGateIMPTimeHi;                                                  // 闸门1内最大波峰值时间高
        quint16 nGateIMPAmp;                                                     // 闸门1内最大波峰值幅度
        quint16 nGateAMPTime;                                                    // 闸门2内最大波峰值时间
        quint16 nGateAMPAmp;                                                     // 闸门2内最大波峰值幅度
        quint16 nGateBMPTime;                                                    // 闸门3内最大波峰值时间
        quint16 nGateBMPAmp;                                                     // 闸门3内最大波峰值幅
        quint16 nGateI2MPTimeLo;                                                 // 闸门4内最大波峰值时间低
        quint16 nGateI2MPTimeHi;                                                 // 闸门4内最大波峰值时间高
        quint16 nGateI2MPAmp;                                                    // 闸门4内最大波峰值幅度
        quint16 nProbeNo;                                                        // 探头号
#if FLANGE_ANALYSIS
        quint16 nGateBWaveWidth;                                                 // B闸门内波形宽度
#endif
        //quint16 nSoftIdx;                                                        // 软通道号
    };

    // 下位机版本信息
    typedef struct VersionLower
    {
        VersionLower():
            aecCardNum(0),
            sensorCardNum(0),
            isFeatureSingleChannel(false),
            isFlangeAnalysis(false),
            deleteFrames(0),
            version(0),
            svnVersion(0),
            fpgaVersion(0)
        {
        };
        ~VersionLower(){};
        int aecCardNum;                 // AEC卡数量
        int sensorCardNum;              // 传感器卡数量
        bool isFeatureSingleChannel;    // 1:1 C扫特征值是否只含单个通道
        bool isFlangeAnalysis;          // 轮缘统计分析
        int  deleteFrames;              // 传感器切换时（继电器切换），去掉头几帧
        float version;                  // 版本号
        int svnVersion;                 // svn版本
        quint16 fpgaVersion;            // fpgaVersion
    }VersionLower;

    static const int A_SCAN_NUM_SAMPLES_PER_FRAME = 512;
    static const int A_SCAN_NUM_FRAMES_PER_PACKET = 30;
    static const int C_SCAN_NUM_FRAMES_PER_PACKET = 128;

    static const int NUM_AXES = 7;                                              // 仪器有7个控制轴
    static const int NUM_FEATURES = 18;                                         // 特征值个数为18个
    static const int NUM_CHANNELS = 824;                                        // 仪器有824个通道


    // 监视数据
    struct FullData
    {
        quint16 nPicData[A_SCAN_NUM_SAMPLES_PER_FRAME];
        FeatureData stFeatureData;
    };

    typedef struct
    {
        quint16 wFlawTime;
        quint16 wFlawHeight;
    }GATE_FEATURE_DATA;

    typedef struct
    {
        quint16 wFlawTimeLo;
        quint16 wFlawTimeHi;
        quint16 wFlawHeight;
    }GATE_FEATURE_DATA_32;

    typedef struct
    {
        GATE_FEATURE_DATA_32 stGateI1;
        GATE_FEATURE_DATA stGateA;
        GATE_FEATURE_DATA stGateB;
        GATE_FEATURE_DATA_32 stGateI2;
    }CH_FEATURE_DATA;

    typedef struct
    {
        CH_FEATURE_DATA stFlawFeature;
        CH_FEATURE_DATA stSurFeature;
    }CH_L_FEATURE_DATA;

#if FLANGE_ANALYSIS
    typedef struct
    {
        quint16 wSurface;   // 界面波
        quint16 wFlaw;      // 伤波
    }CH_GATEB_WAVE_WIDTH;   // 通道B闸门内波形宽度
#endif

#if FEATURE_SINGLE_CHANNEL
    typedef struct
    {
        quint16 wSoftChannelIndex[2];   // 软通道号
        CH_L_FEATURE_DATA stData[AEC_CHN_NUM];
        quint16 wProbeNo[AEC_CHN_NUM];
        quint16 wLightEye[4];
#if FLANGE_ANALYSIS
        CH_GATEB_WAVE_WIDTH stGateBWaveWidth[AEC_CHN_NUM];
        quint16 wTimeStampL;        // 时间戳低16位
        quint16 wTimeStampH;        // 时间戳高16位
#endif
    }FEATURE_DATA;
#else
    typedef struct
    {
        quint16 wSoftChannelIndex;      // 软通道号
        quint16 wPxiIdx;                // 数据标签位 用做AEC卡序号 0~7
        CH_L_FEATURE_DATA stData[AEC_CHN_NUM];
        quint16 wProbeNo[AEC_CHN_NUM];
        quint16 wLightEye;
#if FLANGE_ANALYSIS
        CH_GATEB_WAVE_WIDTH stGateBWaveWidth[AEC_CHN_NUM];
        quint16 wTimeStampL;            // 时间戳低16位
        quint16 wTimeStampH;            // 时间戳高16位
#endif
        quint16 wWheelNo;               // 轮组序号 1~n
    }FEATURE_DATA;
#endif

    typedef struct
    {
        quint16 wSoftChannelIndex;	// 软通道号
        CH_L_FEATURE_DATA stData;
        quint16 wProbeNo;
        quint16 wLightEye;
#if FLANGE_ANALYSIS
        CH_GATEB_WAVE_WIDTH stGateBWaveWidth;   // B
        quint16 wTimeStampL;        // 时间戳低16位
        quint16 wTimeStampH;        // 时间戳高16位
#endif
    }FEATURE_COMMU; // 1：1通讯数据

    typedef struct
    {
        FEATURE_DATA featurs[C_SCAN_NUM_FRAMES_PER_PACKET];
    }FEATURE_DATA_S;

    typedef qint16 ADataValueRaw;
    typedef qint16 CDataValueRaw;
    static const int BYTES_PACKET_HEAD = sizeof(PacketHead);
    static const int BYTES_FRAME_ASCAN = sizeof(ADataValueRaw) * A_SCAN_NUM_SAMPLES_PER_FRAME;
    static const int BYTES_FRAME_POSE = sizeof(UTPoseValueRaw) * NUM_AXES;
    static const int BYTES_FRAME_FEATURE = sizeof(CDataValueRaw) * NUM_FEATURES;
    static const int BYTES_FRAME_CDATA = sizeof(FeatureData);

    static const int BYTES_PACKET_BODY_EYES_STATUS = sizeof(SIMULATION_DATA_t);
    static const int BYTES_PACKET_EYES_STATUS = sizeof(PacketHead) + BYTES_PACKET_BODY_EYES_STATUS;

    static const int BYTES_PACKET_BODY_MONITOR = sizeof(FullData);
    static const int BYTES_PACKET_MONITOR = sizeof(PacketHead) + sizeof(FullData);

    static const int BYTES_PACKET_BODY_ASCAN = sizeof(FullData) * A_SCAN_NUM_FRAMES_PER_PACKET;
    static const int BYTES_PACKET_ASCAN = sizeof(PacketHead) + BYTES_PACKET_BODY_ASCAN;

    static const int BYTES_PACKET_BODY_CSCAN = sizeof(FEATURE_DATA) * C_SCAN_NUM_FRAMES_PER_PACKET;
    static const int BYTES_PACKET_CSCAN = sizeof(PacketHead) + BYTES_PACKET_BODY_CSCAN;

    static const int BYTES_PACKET_BODY_PARAM = sizeof(CH_CONFIG);
    static const int BYTES_PACKET_PARAM = sizeof(PacketHead) + sizeof(CH_CONFIG);

    const int TCG_RAM_SIZE = 1024;                                                                    // TCG ram 地址块大小 1024 / 16个软通道

    enum SocketType
    {
        AScan = 0,
        Monitor,
        CScan,
        Parameter
    };
}   /* namespace NS_PXI100 */

//不能在命名空间内
typedef QSharedPointer<NS_PXI100::CH_CONFIG> UTSharedDataParam;
//Q_DECLARE_METATYPE(UTSharedDataParam)         // 别名不需要

typedef QSharedPointer<NS_PXI100::FullData> UTSharedMonitorFrame;
//Q_DECLARE_METATYPE(UTSharedMonitorFrame)      // 别名不需要

typedef QSharedPointer<NS_PXI100::SIMULATION_DATA_t> UTSharedDataSimulation;
//Q_DECLARE_METATYPE(UTSharedDataSimulation)    // 别名不需要


typedef QSharedPointer<NS_PXI100::FEATURE_DATA> UTSharedFeatureFrame;
typedef QList<UTSharedFeatureFrame> UTFeatureList;
//typedef QSharedPointer<UTFeatureList> UTSharedFeatureSegment;
typedef QSharedPointer<NS_PXI100::FEATURE_DATA_S> UTSharedFeatureSegment;
typedef QSharedPointer<NS_PXI100::FEATURE_COMMU> UTSharedFeatureCommu;
typedef QList<UTSharedFeatureCommu> UTCommuList;
typedef QSharedPointer<UTCommuList> UTSharedCommuSegment;
typedef QSharedPointer<NS_PXI100::VersionLower> sharedVersionLower;
#endif // DATADEF_H
