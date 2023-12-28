#ifndef _ARS200FPGA_H_
#define _ARS200FPGA_H_

//#pragma pack(1)

typedef struct _ARS200FPGA_SYS_REG
{

	WORD wSoftReset;	//系统软复位
	WORD wRelay;		//所有继电器控制bit10-0
	WORD wSPIReset;     //SPI复位
	WORD wSPIChannel;	//SPI选择
	WORD wSPIStart;		//SPI传输开始
	WORD wSPIModeSelect;	//SPI模式选择
	WORD wSPIAddr;	//SPI地址
	WORD wSPIData;   //SPI数据
	WORD wNULL1;   //
	WORD wNULL2;   //
	WORD wNULL3;   //
	WORD wNULL4;   //
	WORD wNULL5;   //	
	WORD wSPIReady;		//SPI就绪
	WORD wLedAlarm;		//LED报警
	WORD wLedBrightness;	//LED背光亮度
	WORD wPowerSupply;	// 电源指示灯
	WORD wWorkMode;		// 工作模式
} ARS200FPGA_SYS_REG;

// songchenguang add start 2010-10-13
// 硬件版本
typedef struct _ARS200VER
{
	WORD wHardwareVer;
}ARS200VER;

// 软件功能定义
typedef struct _ARS200FUN_DEFINE
{
	WORD wLowFunctionDefine;
	WORD wHighFunctionDefine;
}ARS200FUN_DEFINE;
// songchenguang add end 2010-10-13
/*
// wangbingfu add start 2010-11-11
//设备型号
typedef struct _ARS200DEVICETYPE
{
	WORD wDeviceType;
}ARS200DEVICETYPE;
// wangbingfu add start 2010-11-11
*/

/*  发现没用处，注掉 yachang.huang add on 2013-12-2
typedef struct _ARS200FPGA_CH_CON_REG
{
	WORD wReset;		//通道软复位
	WORD wEnable;		//使能
	WORD wSuspend;		//抑制
	WORD wRepeatCounter;	//重复频率计数器
	WORD wDetectMode;	//检波模式
	WORD wValueMiddle;	//中值
	WORD wMaiKuan;		//脉宽
} ARS200FPGA_CH_CON_REG;
*/

typedef struct _ARS200FPGA_CH_PLUS_REG
{
	WORD wdBControl;	//增益转换开关
	WORD wdBRef;		//增益控制基准
	WORD wdBLevel1;		//1级增益 AD603
	WORD wdBLevel2;		//2级增益 AD600-1
	WORD wdBLevel3;		//3级增益 AD600-2
} ARS200FPGA_CH_PLUS_REG;

typedef struct _ARS200FPGA_CH_DAC_REG
{
	WORD wDACEnable;	//DAC使能开关
	WORD wDACFront;		//DAC前闸门
	WORD wDACRear;		//DAC后闸门
} ARS200FPGA_CH_DAC_REG;

typedef struct _ARS200FPGA_CH_GATE_REG
{
	WORD wGate1Enable;	//闸门1使能
	WORD wGate1Alarm;	//闸门1报警类型设置
	WORD wGate1Front;	//闸门1前沿
	WORD wGate1Rear;	//闸门1后沿
	WORD wGate2Enable;	//闸门2使能
	WORD wGate2Alarm;	//闸门2报警类型设置
	WORD wGate2Front;	//闸门2前沿
	WORD wGate2Rear;	//闸门2后沿
	WORD wGate3Enable;	//闸门3使能
	WORD wGate3Alarm;	//闸门3报警类型设置
	WORD wGate3Front;	//闸门3前沿
	WORD wGate3Rear;	//闸门3后沿
	WORD wAlarmFlag;	//报警标志位
	WORD wAlarmReg;		//报警向量
	WORD wGateDone;		//闸门处理结束

	WORD wGate1DefectAmp;	//闸门1最大伤幅
	WORD wGate1DefectTime;	//闸门1最大伤时间

	WORD wGate2DefectAmp;	//闸门2最大伤幅
	WORD wGate2DefectTime;	//闸门2最大伤时间

	WORD wGate3DefectAmp;	//闸门3最大伤幅
	WORD wGate3DefectTime;	//闸门3最大伤时间

	WORD wGate1Peak;		//闸门1幅度值
	WORD wGate2Peak;		//闸门2幅度值
} ARS200FPGA_CH_GATE_REG;

typedef struct _ARS200FPGA_CH_DATA_REG
{
	WORD wMonitorReset;	//监视通道复位
	WORD wMonitorZeroCounter;	//零点修正监视器
	WORD wMonitorFlag;	//数据有效标志
	WORD wMonitorDelayCounter;	// 监视通道延迟计数器
} ARS200FPGA_CH_DATA_REG;

//	cscan wangbingfu add start at 2012-1-13
//编码器
typedef struct _CSCAN_ARS200FPGA_CODE_REG
{
	WORD wEnable;		//编码器使能
	WORD wReset;		//编码器复位
	WORD wXLow;			//X轴编码器低16位
	WORD wXHigh;		//X轴编码器高16位
	WORD wYLow;			//Y轴编码器低16位
	WORD wYHigh;		//Y轴编码器高16位
	WORD wZLow;			//Z轴编码器低16位
	WORD wZHigh;		//Z轴编码器高16位
	WORD wW1Low;		//W1轴编码器低16位
	WORD wW1High;		//W1轴编码器高16位
	WORD wALow;			//A轴编码器低16位
	WORD wAHigh;		//A轴编码器高16位
	WORD wBLow;			//B轴编码器低16位
	WORD wBHigh;		//B轴编码器高16位
	WORD wW2Low;		//W2轴编码器低16位
	WORD wW2High;		//W2轴编码器高16位
} CSCAN_ARS200FPGA_CODE_REG;

typedef struct _CSCAN_ARS200FPGA_CH_CON_REG
{
	WORD wReset;		//通道软复位
	WORD wRepeatCounter;	//重复频率计数器
	WORD wMaiKuan;		//脉宽
} CSCAN_ARS200FPGA_CH_CON_REG;

typedef struct _CSCAN_ARS200FPGA_CH_CONFIG_REG
{
	WORD wDetectMode;	//检波模式
	WORD wValueMiddle;	//中值
	WORD wdBControl;	//增益转换开关
	WORD wdBRef;		//增益控制基准
	WORD wdBLevel1;		//1级增益 AD603
	WORD wdBLevel2;		//2级增益 AD600-1
	WORD wdBLevel3;		//3级增益 AD600-2
	WORD wSuspend;		//抑制
	WORD wMonitorZeroCounter;	//零点修正监视器
	WORD wDACEnable;	//DAC使能开关
} CSCAN_ARS200FPGA_CH_CONFIG_REG;

typedef struct _CSCAN_ARS200FPGA_CH_GATE_REG
{
	WORD wGate1Enable;					//闸门1使能
	WORD wGate1SyncMode;				//闸门1同步模式
   	WORD wGate1SyncThreshold;			//闸门1同步阈值
   	WORD wGate1Start;					//闸门1起点			
  	WORD wGate1Width;              		//闸门1宽度 
   	WORD wGate1Amp;						//闸门1幅值
   	WORD wGate2Enable;					//闸门2使能
   	WORD wGate2SyncMode;				//闸门2同步模式
   	WORD wGate2SyncThreshold;			//闸门2同步阈值
   	WORD wGate2Start;					//闸门2起点			
   	WORD wGate2Width;              		//闸门2宽度 
   	WORD wGate2Amp;						//闸门2幅值
   	WORD wGate3Enable;					//闸门3使能
   	WORD wGate3SyncMode;				//闸门3同步模式
   	WORD wGate3SyncThreshold;			//闸门3同步阈值
   	WORD wGate3Start;					//闸门3起点			
   	WORD wGate3Width;              		//闸门3宽度 
   	WORD wGate3Amp;						//闸门3幅值
   	WORD wGate4Enable;					//闸门4使能
   	WORD wGate4SyncMode;				//闸门4同步模式
   	WORD wGate4SyncThreshold;			//闸门4同步阈值
   	WORD wGate4Start;					//闸门4起点			
   	WORD wGate4Width;              		//闸门4宽度 
   	WORD wGate4Amp;						//闸门4幅值
   	WORD wSimpleInterval;				//通道采样间隔
   	WORD wSimpleAverage;				//通道采样平均数
   	WORD wDelay;						//通道延迟时间
}CSCAN_ARS200FPGA_CH_GATE_REG;

typedef struct _CSCAN_ARS200FPGA_SETONE
{
	CSCAN_ARS200FPGA_CH_CON_REG stCon;
	CSCAN_ARS200FPGA_CH_CONFIG_REG stConfig;
	CSCAN_ARS200FPGA_CH_GATE_REG stGate;
	float fRange;			//声程
	int nVelocity;			//声速
//	WORD wWorkMode;			//工作模式
	WORD wProbeBand;		//探头频率
	WORD wVoltage;			//电压
	WORD wImpedance;		//阻抗
	float fPlus;			//增益
	float fRefPlus;			//参考增益
	WORD wEnerge;           //高低能量切换，0为低能量，1为高能量,分通道   yachang.huang add on 2013-11-22
	WORD wRevAndSendMode;   //收发模式切换，0为一发一收模式，1为自发自收模式 yachang.huang add on 2013-11-22
	WORD wSPIAddr;          //SPI地址选择 串行D/A 8路通道选择 数字1-8代表   yachang.huang add on 2013-11-22
	WORD wSPIData;          //0x0000~0x3FFF：其中0x0000为-6V，0x3FFF为+6V   yachang.huang add on 2013-11-22
	float fPlusAddX11;      //增益补偿 X11   yachang.huang add on 2014-04-16
	float fPlusAddX12;      //增益补偿 X12   yachang.huang add on 2014-04-16
	float fPlusAddX21;      //增益补偿 X21   yachang.huang add on 2014-04-16
	float fPlusAddX22;      //增益补偿 X22   yachang.huang add on 2014-04-16
}CSCAN_ARS200FPGA_SETONE;

typedef struct _ARS200FPGA_CH_FEATURE_REG{
	WORD wGate1SyncTime;			//闸门1同步时间
   	WORD wGate1MPFirstTime;			//闸门1内最大波峰过阈值时间
  	WORD wGate1MPTime;				//闸门1内最大波峰值时间
    WORD wGate1MPAmp	;			//闸门1内最大波峰值幅度
    WORD wGate1FPFirstTime;			//闸门1内第一个波峰过阈值时间
    WORD wGate1FPTime;				//闸门1内第一个波峰波峰值时间
    WORD wGate1FPAmp;				//闸门1内第一个波峰波峰值幅度
    WORD wGate2SyncTime;			//闸门2同步时间
   	WORD wGate2MPFirstTime;			//闸门2内最大波峰过阈值时间
  	WORD wGate2MPTime;				//闸门2内最大波峰值时间
    WORD wGate2MPAmp	;			//闸门2内最大波峰值幅度
    WORD wGate2FPFirstTime;			//闸门2内第一个波峰过阈值时间
    WORD wGate2FPTime;				//闸门2内第一个波峰波峰值时间
    WORD wGate2FPAmp;				//闸门2内第一个波峰波峰值幅度
    WORD wGate3SyncTime;			//闸门3同步时间
   	WORD wGate3MPFirstTime;			//闸门3内最大波峰过阈值时间
  	WORD wGate3MPTime;				//闸门3内最大波峰值时间
    WORD wGate3MPAmp	;			//闸门3内最大波峰值幅度
    WORD wGate3FPFirstTime;			//闸门3内第一个波峰过阈值时间
    WORD wsGate3FPTime;				//闸门3内第一个波峰波峰值时间
    WORD wGate3FPAmp;				//闸门3内第一个波峰波峰值幅度
    WORD wGate4SyncTime;			//闸门4同步时间
   	WORD wGate4MPFirstTime;			//闸门4内最大波峰过阈值时间
  	WORD wGate4MPTime;				//闸门4内最大波峰值时间
    WORD wGate4MPAmp	;			//闸门4内最大波峰值幅度
    WORD wGate4FPFirstTime;			//闸门4内第一个波峰过阈值时间
    WORD wGate4FPTime;				//闸门4内第一个波峰波峰值时间
    WORD wGate4FPAmp;				//闸门4内第一个波峰波峰值幅度
} CSCAN_ARS200FPGA_CH_FEATURE_REG;

typedef struct _CSCAN_ARS200FPGA_CH_DATA_REG
{
	WORD wMonitorReset;		//监视通道复位
	WORD wMonitorMReset;	//监视通道手动复位
	WORD wFeatureReadReset;	//闸门内特征数据缓冲区可读标志复位
	WORD wMonitorFlag;		//数据有效标志
	WORD wFeatureReadFlag;	//闸门内特征数据缓冲区可读标志
} CSCAN_ARS200FPGA_CH_DATA_REG;

//	cscan wangbingfu add end at 2012-1-13
//#pragma pack()

BOOL ars200fpga_alloc_io();
BOOL ars200fpga_unalloc_io();

BOOL cscan_ars200fpga_alloc_io();
BOOL cscan_ars200fpga_unalloc_io();

void send_max500(WORD cword);
void send_mcp41010(WORD cword);

#endif
