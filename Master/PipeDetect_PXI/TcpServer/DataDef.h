#ifndef DATADEF_H
#define DATADEF_H

#include <QByteArray>
#include "ARS3D/UTData.h"
#include "UTInstrumentDriver.h"
#include "const.h"

#define PROBE_SENSOR 1
#define SUB_CONFIG_NUM 2
#define WAVE_SURFACE 0
#define WAVE_FLAW 1
#define ASCAN_PORT       2222
#define MONITOR_PORT     3333
#define CSCAN_PORT       4444
#define PARAM_PORT       5555
#define EYES_STATUS_PORT 6666

namespace NS_PXI100 {
    typedef UTInstrumentDriver::DataLevel DataLevel;
    typedef qint32 CommandInt;
    typedef UTInstrumentDriver::ParameterTable ParameterTable;
    typedef QVector<float> TCGData;

    // 指令集
    enum Command
    {
        CMD_CURRENT_CHANNEL         = 0x0001,                                   // 当前通道
        CMD_SCAN_STATUS             = 0x0007,                                   // 扫查状态
        CMD_SIMULATE_SET            = 0x0008,                                   // 电气模拟设置
        CMD_WORKMODE                = 0x0011,                                   // 工作模式
        CMD_ALL_PARAM_SET           = 0x0601,                                   // 上位机设置全部参数
        CMD_RESET                   = 0x0701,                                   // 上位机复位请求
        CMD_ALL_PARAM_REQ           = 0x1001,                                   // 上位机获取全部参数请求
        CMD_ASCAN                   = 0x1002,                                   // A扫实时数据请求 特征数据请求（A扫(3333端口)和C扫（4444端口）使用的端口不同，能够区分开来），必须在程序开始时分别向两个端口发送该命令
        CMD_VERSION_REQ             = 0x1003,                                   // 版本号请求
        CMD_WORK_MODE_SET           = 0x1004,                                   // 设置工作模式  从1开始：单1;单2;双单;双晶反射;双晶对射;单晶关联;
        CMD_TCG_SWITCH              = 0x1005,                                   // 设置TCG曲线
        CMD_ASCAN_DATA_STORE        = 0x1006,                                   // A扫存储数据请求
        CMD_ASCN_STOP               = 0x1007,                                   // A扫实时数据 特征数据 暂停
        CMD_TEST_START_FLAG         = 0x1008,                                   // 探伤开始标志
        CMD_CODER_MARK              = 0X1009,                                   // 编码器切换标志
        CMD_SET_PRF                 = 0x5000,                                   // PRF设置
        CMD_ASCAN_RET_WAVE          = 0x2001,                                   // A扫实时数据上传 特征数据上传（A扫(3333端口)和C扫（4444端口）使用的端口不同，能够区分开来）
        CMD_CSCAN_DATA              = 0x2002,                                   // C扫数据
        CMD_ASCAN_RET_DATA_STORE    = 0x2006,                                   // A扫存储数据上传
        CMD_SET_PARAM_RET           = 0xff01,                                   // 设置参数返回指令
        CMD_LOWER_PLX_REQ           = 0x3001,                                   // 请求下位机PLX状态
        CMD_EYES_STATUS_REQ         = 0x3002,                                   // 给下位机设置请求传感器状态的上发的开关
        CMD_LOWER_PLX_ANSWER        = 0x4001,                                   // 下位机状态应答
        CMD_EYES_STATUS_ANSWER      = 0x4002,                                   // 下位机传输当前传感器触发状态
        CMD_HEART                   = 0x1111,                                   // 心跳包
        CMD_FLAW_OR_BOTTOM          = 0x1200                                    // 1号AEC卡8个硬通道伤波底波切换标志
    };

    // 网络数据自定义包头
    struct PacketHead
    {
        CommandInt lCommand;                                                    // 命令
        qint16 nChannelIndex;                                                   // 通道号
        qint16 nGateIndex;                                                      // 闸门号
        qint32 lSize;                                                           // 该命令大小
    };

    // TCG包头
    struct TCGCommuData
    {
        bool bOpenFlag;
        int nSramNum;
        qint16 nSamplingInterval;//采样间隔  yachang.huang add on 2014-06-18
    };

    typedef struct tagSIMULATION_DATA
    {
        quint16 usEnable;                         // 模拟使能
    }SIMULATION_DATA_t;       // 模拟数据

    struct GATE_INFO
    {
        quint16 GateEnable;         // 软通道底波闸门使能寄存器
        float GateStart;            // 软通道底波闸门闸门起始值
        float GateWidth;            // 软通道底波闸门闸门宽度
        float GateAmp;              // 软通道底波闸门闸门幅度
    };

    struct CH_SUB_CONFIG
    {
        GATE_INFO stGate[GATE_NUM]; // 闸门I, 闸门A, 闸门B, 闸门I2
        float Delay;                // 延时
        float Suppression;          // 抑制
        quint16 MiddleValue;        // 中值
        quint16 DetectMode;         // 检波模式
    };

    struct CH_CONFIG
    {
        float Plus;                                     // 增益
        float CPlus;                                    // 参考增益
        float Range;                                    // 声程
        qint32 Velocity;                                // 声速
        quint16 TCGEnable;                              // 软通道 TCG使能寄存器
        CH_SUB_CONFIG stWaveConfigure[SUB_CONFIG_NUM];	// 0底波通道设置、1伤波通道设置
        quint16 ProbeNo;                                // 探头编号
    };

    struct PLUS
    {
        quint16 wAPlus;     // 软通道通道A增益
        quint16 wBPlus;     // 软通道通道B增益
        quint16 wCPlus;     // 软通道通道C增益
        quint16 wDPlus;     // 软通道通道D增益
    };

    struct GATE_INFO_HARD
    {
        quint16 GateEnable;      // 软通道底波闸门使能寄存器
        quint16 GateStart;       // 软通道底波闸门闸门起始值
        quint16 GateWidth;       // 软通道底波闸门闸门宽度
        quint16 GateAmp;         // 软通道底波闸门闸门幅度
    };

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
        GATE_INFO_HARD stGate[GATE_NUM];    // 闸门I, 闸门A, 闸门B, 闸门I2
        quint16 delay;                      // 延迟
        quint16 middleValue;                // 中值
        quint16 detectMode;                 // 检波模式
        quint16 suppression;                // 抑制
    }CH_SUB_CONFIG_HARD;

    typedef struct
    {
        PLUS stPlus;                            // 增益
        quint16 wBottomFlawPlusDiff;            // 软通道通道伤波和底波增益差
        quint16 wTCGEn;                         // 软通道 TCG使能寄存器
        CH_SUB_CONFIG_HARD stWaveConfigure[2];	// 底波通道设置、伤波通道设置
        quint16 probeNo;                        // 探头编号
    }CH_CONFIG_HARD;

    ///////////////////////////////

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

    static const int A_SCAN_NUM_SAMPLES_PER_FRAME = 512;
    static const int A_SCAN_NUM_FRAMES_PER_PACKET = 30;
    static const int C_SCAN_NUM_FRAMES_PER_PACKET = 128;

    static const int NUM_FLAW_CHANNELS = 25;
    static const int NUM_HARD_CHANNELS = 8;                                     // 所有AEC卡硬通道数量总和，本项目只有一个AEC卡
    static const int NUM_SURFACE_CHANNELS = 5;
    static const int NUM_CHANNELS = NUM_FLAW_CHANNELS + NUM_SURFACE_CHANNELS;   // 仪器有30个通道

    // 监视数据
    struct FullData
    {
        quint16 nPicData[A_SCAN_NUM_SAMPLES_PER_FRAME];
        FeatureData stFeatureData;
    };

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

    struct CH_L_FEATURE_DATA
    {
        CH_FEATURE_DATA_1To6 data1To6[6];
        CH_FEATURE_DATA_7To8 data7To8[2];
    };

    typedef struct
    {
        quint16 coderXLow;
        quint16 coderXHigh;
        quint16 softChannelNum;         // 软通道号
        quint16 pxiIdx;                 // 数据标签位 用做PXI编号
        CH_L_FEATURE_DATA featureData;
        quint16 wCh1ProbeNo;
        quint16 wCh2ProbeNo;
        quint16 wCh3ProbeNo;
        quint16 wCh4ProbeNo;
        quint16 wCh5ProbeNo;
        quint16 wCh6ProbeNo;
        quint16 wCh7ProbeNo;
        quint16 wCh8ProbeNo;
        quint16 coderRLow;
        quint16 coderRHigh;
    }FEATURE_DATA;

    typedef struct
    {
        FEATURE_DATA featurs[C_SCAN_NUM_FRAMES_PER_PACKET];
    }FEATURE_DATA_S;

    typedef qint16 ADataValueRaw;
    typedef qint16 CDataValueRaw;
    static const int BYTES_PACKET_HEAD = sizeof(PacketHead);
    static const int BYTES_FRAME_ASCAN = sizeof(ADataValueRaw) * A_SCAN_NUM_SAMPLES_PER_FRAME;
    static const int BYTES_FRAME_CDATA = sizeof(FeatureData);

    static const int BYTES_PACKET_BODY_EYES_STATUS = sizeof(SIMULATION_DATA_t);
    static const int BYTES_PACKET_EYES_STATUS = sizeof(PacketHead) + BYTES_PACKET_BODY_EYES_STATUS;

    static const int BYTES_PACKET_BODY_MONITOR = sizeof(FullData);
    static const int BYTES_PACKET_MONITOR = sizeof(PacketHead) + sizeof(FullData);

    static const int BYTES_PACKET_BODY_ASCAN = sizeof(FullData) * A_SCAN_NUM_FRAMES_PER_PACKET;
    static const int BYTES_PACKET_ASCAN = sizeof(PacketHead) + BYTES_PACKET_BODY_ASCAN;

    static const int BYTES_PACKET_BODY_CSCAN = sizeof(FEATURE_DATA) * C_SCAN_NUM_FRAMES_PER_PACKET;
    static const int CSCAN_DATA_DMA_SIZE = BYTES_PACKET_BODY_CSCAN / sizeof(WORD) * 2;
    static const int BYTES_PACKET_CSCAN = sizeof(PacketHead) + BYTES_PACKET_BODY_CSCAN;

    static const int BYTES_PACKET_BODY_PARAM = sizeof(CH_CONFIG);
    static const int BYTES_PACKET_PARAM = sizeof(PacketHead) + sizeof(CH_CONFIG);

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
typedef QSharedPointer<NS_PXI100::FullData> UTSharedMonitorFrame;
typedef QSharedPointer<NS_PXI100::FEATURE_DATA> UTSharedFeatureFrame;
typedef QList<UTSharedFeatureFrame> UTFeatureList;
typedef QSharedPointer<NS_PXI100::FEATURE_DATA_S> UTSharedFeatureSegment;
#endif // DATADEF_H
