#ifndef DATADEF_H
#define DATADEF_H

#include <QByteArray>
#include <QJsonObject>
#include <Ultrasonic/UTData.h>

#define GATE_NUM 4
#define SUB_CONFIG_NUM 2
#define AEC_CHN_NUM 8

typedef enum{
    SURFACE = 0,    // 界面波通道
    FLAW            // 伤波通道
}SURFACE_FLAW_e;

typedef enum{
    I1 = 0,
    A,
    B,
    I2
}GATE_e; //

enum WaveType
{
    WaveSurface,
    WaveFlaw
};

namespace NS_PXI100 {
    typedef qint32 CommandInt;
    typedef QJsonObject ParameterTable;
    typedef QVector<float> TCGData;

    enum DataLevel
    {
        NONE = 0,        // 无
        MASK_MONITOR = 1 << 0,
        MASK_CSCAN = 1 << 1,
        MASK_ASCAN = 1 << 2,
        MASK_PARAMETER = 1 << 3,
        MONITOR_ONLY = MASK_MONITOR,    // 只有监视
        CSCAN_ONLY = MASK_MONITOR | MASK_CSCAN,     // 监视与C扫
        ASCAN_ONLY = MASK_MONITOR | MASK_ASCAN           // 监视与A扫
    };

    enum CDataTypeEnum
    {
        DATA_TYPE_TIME,
        DATA_TYPE_AMP
    };
    typedef QList<CDataTypeEnum> CDataTypeList;
    static const CDataTypeList CDataType = {DATA_TYPE_TIME,   // 0    Gate1SyncTime
                                            DATA_TYPE_TIME,   // 1    Gate1MPTime
                                            DATA_TYPE_AMP,    // 2    Gate1MPAmp
                                            DATA_TYPE_TIME,   // 3    Gate2MPTime
                                            DATA_TYPE_AMP,    // 4    Gate2MPAmp
                                            DATA_TYPE_TIME,   // 5    Gate2FPFirstTime
                                            DATA_TYPE_TIME,   // 6    Gate2FPTime
                                            DATA_TYPE_AMP,    // 7    Gate2FPAmp
                                            DATA_TYPE_TIME,   // 8    Gate3MPTime
                                            DATA_TYPE_AMP,    // 9    Gate3MPAmp
                                            DATA_TYPE_TIME,   // 10   Gate3FPFirstTime
                                            DATA_TYPE_TIME,   // 11   Gate3FPTime
                                            DATA_TYPE_AMP,    // 12   Gate3FPAmp
                                            DATA_TYPE_TIME,   // 13   Gate4MPTime
                                            DATA_TYPE_AMP,    // 14   Gate4MPAmp
                                            DATA_TYPE_TIME,   // 15   Gate4FPFirstTime
                                            DATA_TYPE_TIME,   // 16   Gate4FPTime
                                            DATA_TYPE_AMP};   // 17   Gate4FPAmp;

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
        qint16 nSamplingInterval;//采样间隔  yachang.huang add on 2014-06-18
//        TCGData vecSramGain;
    };

    static const qint32 LIGHT_EYE_VAR_NUM = 12;
    struct SIMULATION_DATA_t
    {
        SIMULATION_DATA_t(){
            sEnable = 0;
            memset(sLightEye, 0, sizeof(sLightEye));
        };
        ~SIMULATION_DATA_t(){};
        quint16 sEnable;                        // 模拟使能
        quint16 sLightEye[LIGHT_EYE_VAR_NUM];   // 电气光眼信号数据
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
        GATE_INFO stGate[GATE_NUM];   // 闸门I, 闸门A, 闸门B, 闸门I2
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
        float Delay;
        qint32 Velocity;                                // 声速
        quint16 TCGEnable;                              // 软通道 TCG使能寄存器
        CH_SUB_CONFIG stWaveConfigure[SUB_CONFIG_NUM];	// 0底波通道设置、1伤波通道设置
        quint16 ProbeNo;                                // 探头编号
//        quint16 eyeArea;                                //光眼区域0-7(16个光眼一个区域)
    };

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
        quint16 nGateWaveWidth;                                                  //B闸门内波形宽度
        //quint16 nSoftIdx;                                                        // 软通道号
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

    typedef struct
    {
        quint16 wSurface;   // 界面波
        quint16 wFlaw;      // 伤波
    }CH_GATEB_WAVE_WIDTH;   // 通道B闸门内波形宽度

    typedef struct
    {
        quint16 wSoftChannelIndex;	// 软通道号
        quint16 wPxiIdx;             // 数据标签位 用做PXI编号
        CH_L_FEATURE_DATA stData[AEC_CHN_NUM];
        quint16 wProbeNo[AEC_CHN_NUM];
        quint16 wLightEye;
        CH_GATEB_WAVE_WIDTH stGateBWaveWidth[AEC_CHN_NUM];
        quint16 wTimeStampL;
        quint16 wTimeStampH;
        quint16 wWheelNo;
    }FEATURE_DATA;

    typedef struct
    {
        quint16 wSoftChannelIndex;	// 软通道号
        CH_L_FEATURE_DATA stData;
        quint16 wProbeNo;
        quint16 wLightEye;
        CH_GATEB_WAVE_WIDTH stGateBWaveWidth;   // B
        quint16 wTimeStampL;        // 时间戳低16位
        quint16 wTimeStampH;        // 时间戳高16位
    }FEATURE_COMMU; // 1：1通讯数据

    // 下位机版本信息
    typedef struct VersionLower
    {
        VersionLower():
            aecCardNum(0),
            sensorCardNum(0),
            isFeatureSingleChannel(false),
            isFlangeAnalysis(false),
            deleteFrames(false),
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
        int deleteFrames;               // 传感器切换时（继电器切换），去掉头几帧
        float version;                  // 版本号
        int svnVersion;                 // svn版本
        quint16 fpgaVersion;            // fpgaVersion
    }VersionLower;

    static const int PXI_VERSION_MINIMUM = 10641;
    static const int A_SCAN_NUM_SAMPLES_PER_FRAME = 512;
    static const int A_SCAN_NUM_FRAMES_PER_PACKET = 30;
    static const int C_SCAN_NUM_FRAMES_PER_PACKET = 1;//128;PXI端最大一帧发送上来

    static const int NUM_AXES = 7;                                              // 仪器有7个控制轴
    static const int NUM_FEATURES_ADATA = 13;
    static const int NUM_MONITOR_CDATA = NUM_FEATURES_ADATA - 2;                 // I1和I2由高低位组成
    static const int NUM_FEATURES_CDATA = sizeof(FEATURE_DATA) / 2;                 // 特征值个数为190个
    static const int NUM_FRAME_CDATA = NUM_FEATURES_CDATA - 33;                 // 特征值个数为157个

    // 监视数据
    struct FullData
    {
        qint16 nPicData[A_SCAN_NUM_SAMPLES_PER_FRAME];
        FeatureData stFeatureData;
    };
    typedef qint16 ADataValueRaw;
    typedef quint16 CDataValueRaw;  // wangwei
    static const int BYTES_PACKET_HEAD = sizeof(PacketHead);
    static const int BYTES_FRAME_ASCAN = sizeof(ADataValueRaw) * A_SCAN_NUM_SAMPLES_PER_FRAME;
    static const int BYTES_FRAME_POSE = sizeof(qint32) * NUM_AXES;
    static const int BYTES_FRAME_FEATURE_ADATA = sizeof(CDataValueRaw) * NUM_FEATURES_ADATA;
    static const int BYTES_FRAME_FEATURE_CDATA = sizeof(CDataValueRaw) * NUM_FEATURES_CDATA;
    static const int BYTES_FRAME_CDATA = sizeof(FeatureData);

    static const int BYTES_PACKET_BODY_MONITOR = sizeof(FullData);
    static const int BYTES_PACKET_MONITOR = BYTES_PACKET_HEAD + sizeof(FullData);

    static const int BYTES_PACKET_BODY_EYES_STATUS = sizeof(SIMULATION_DATA_t);
    static const int BYTES_PACKET_EYES_STATUS = BYTES_PACKET_HEAD + BYTES_PACKET_BODY_EYES_STATUS;

    static const int BYTES_PACKET_BODY_ASCAN = sizeof(FullData) * A_SCAN_NUM_FRAMES_PER_PACKET;
    static const int BYTES_PACKET_ASCAN = BYTES_PACKET_HEAD + BYTES_PACKET_BODY_ASCAN;

    static const int BYTES_PACKET_BODY_CSCAN = sizeof(FEATURE_DATA) * C_SCAN_NUM_FRAMES_PER_PACKET;
    static const int BYTES_PACKET_CSCAN = BYTES_PACKET_HEAD + BYTES_PACKET_BODY_CSCAN;

    static const int BYTES_PACKET_BODY_PARAMETER = sizeof(int);
    static const int BYTES_PACKET_PARAMETER = BYTES_PACKET_HEAD + BYTES_PACKET_BODY_PARAMETER;

    static const int BYTES_PACKET_BODY_SERVER_PARAM = sizeof(CH_CONFIG);
    static const int BYTES_PACKET_SERVER_PARAM = BYTES_PACKET_HEAD + BYTES_PACKET_BODY_SERVER_PARAM;

    enum SocketType
    {
        AScan = 0,
        Monitor,
        CScan,
        Parameter
    };

    // 组建数据请求数据包
//    QByteArray MakeDataRequestPacket(CommandInt cmd, int nchannel);
    // 组建编码器复位（坐标复位）数据包
//    QByteArray MakePoseResetPacket();
    // 由CH_FLAWDATA_CSCAN数据提取坐标
//    qint32 GetCScanDataCoordinate(const char *data);

}   /* namespace NS_PXI100 */

typedef QSharedPointer<NS_PXI100::SIMULATION_DATA_t> UTSharedDataSimulation;
Q_DECLARE_METATYPE(UTSharedDataSimulation)
typedef QSharedPointer<NS_PXI100::VersionLower> sharedVersionLower;
#endif // DATADEF_H
