#ifndef _DATA_DEFINE_H_
#define _DATA_DEFINE_H_

class CMainFrame;

// 卡线程传递参数
// Songchenguang modified start on 2011-08-17
typedef struct
{
	int index;
	P9054_HANDLE hDev;
	WORD nMain2AECThreadCommand; 
	WORD nAEC2MainThreadFlag;
	BOOL bRunFlag;			// 挂起: 0, 运行: 1	
	BOOL bStarted;
	WORD nSetOneChannel;
	WORD nSelChannel;
	CSCAN_ARS200FPGA_SETONE * pAllConfig;// modify by songchenguang 2012-01-16
	HWND hWnd;				// A扫视图句柄
	HWND hWndFeatureInfo;	// 特征窗口句柄
	HWND hWndCScan;			// C扫窗口句柄  wangbingfu add at 2011-9-27
	WD_PCI_SLOT deviceSlot;	// Slot编号
	CMainFrame* pFrame;
}AECTHREAD_PARAM;
// Songchenguang modified end on 2011-08-17

// A扫波形数据
class CPicData
{
public:
	CPicData()
	{
		::ZeroMemory(m_wData, sizeof(m_wData));
		m_fX = 0.0f;
		m_fY = 0.0f;
		m_fZ = 0.0f;
		// machao modify start on 2013-11-26
		m_fA = 0.0f;
		m_fB = 0.0f;
		m_fW = 0.0f;
		m_fR = 0.0f;
		// machao modify end on 2013-11-26

		int nSize = MONITOR_DATA_SIZE/AD_SIZE;
		for (int i=0; i<nSize; i++)
		{
			m_wData[i]= 10;
		}
	}

	~CPicData()
	{
	}
	inline CPicData& operator=(const CPicData& rhs)
	{
		if(this == &rhs)
			return * this;

		m_fX = rhs.m_fX;
		m_fY = rhs.m_fY;
		m_fZ = rhs.m_fZ;
		// machao modify start on 2013-11-26
		m_fA = rhs.m_fA;
		m_fB = rhs.m_fB;
		m_fW = rhs.m_fW;
		m_fR = rhs.m_fR;
		// machao modify end on 2013-11-26
		memcpy(m_wData,rhs.m_wData, sizeof(m_wData));

		return *this;
	}
public:
	WORD m_wData[MONITOR_DATA_SIZE/AD_SIZE];
	float m_fX;
	float m_fY;
	float m_fZ;
	// machao modify start on 2013-11-26
	float m_fA;
	float m_fB;
	float m_fW;
	float m_fR;
	// machao modify end on 2013-11-26
};


// 下发通道数据
class CH_CONFIG_HD
{
public:
	WORD CH_DEM_MODE;   // 检波模式  全波0，正半波1，负半波2
	WORD MIDDLE_VALUE;  // AD中值
	WORD CH_FRONTTIME;	//闸门前沿	mm			
	WORD CH_REARTIME;   //闸门后沿  mm          
	WORD CH_MAXBACKTIME;	//最大底波位置 mm
	WORD CH_BEMINDATA;		//底波阈值
	//	WORD CH_TGC_DA_START;
	// Song Chenguang add start on 2008-11-11
	WORD CH_GAIN_DIFF;		// 伤波通道与底波通道增益差值
	// Song Chenguang add end on 2008-11-11
	WORD CH_GAIN_A;			
	WORD CH_GAIN_B;
	WORD CH_GAIN_C;
	WORD CH_GAIN_D;			//增益值		dB
	WORD CH_REJECT_VALUE;	//抑制
	WORD CH_ZEROCORRECT;	//延迟		mm
	WORD CH_FLAW_GAIN_MUX;	//伤波衰减档	0dB：0；-6dB：1；-12dB:2，....-42dB：7
	WORD CH_BE_GAIN_MUX;	//底波衰减档	0dB：0；-6dB：1；-12dB:2，....-42dB：7
	WORD CH_TGC_DAC_EN;		//DAC开启		关闭：0；开启：1	
	WORD CH_FLAW_SURF_DISTANCE;				//上盲区		mm
	WORD CH_FLAW_BE_DISTANCE;				//下盲区		mm
	WORD CH_EN;								//通道开启 关闭：0；开启：1
	WORD AUTO_GAIN_EN;						//自动增益 关闭：0；开启：1
	WORD AUTO_GATE_EN;						//自动闸门 关闭：0；开启：1
	WORD ALARM_EN;							//报警开关 关闭：0；开启：1
	WORD CH_REF_NO;							//起始通道号
	WORD CH_FLAW_DEF;						
	WORD CH_SELFCHECK;						//自检开关 关闭：0；开启：1
	WORD CH_MONITOR_RANGE;					
	WORD CH_NO;								//通道号
	// Song Chenguang add start on 2011-08-10
	WORD wGate1Enable;			//闸门1使能	
	WORD wGate1SyncMode;		//闸门1同步模式
	WORD wGate1SyncThreshold;	//闸门1同步阈值
	WORD wGate1Start;			//闸门1起点			
	WORD wGate1Width;           //闸门1宽度 
	WORD wGate1Amp;				//闸门1幅值
	WORD wGate2Enable;			//闸门2使能
	WORD wGate2SyncMode;		//闸门2同步模式
	WORD wGate2SyncThreshold;	//闸门2同步阈值
	WORD wGate2Start;			//闸门2起点			
	WORD wGate2Width;           //闸门2宽度 
	WORD wGate2Amp;				//闸门2幅值
	WORD wGate3Enable;			//闸门3使能
	WORD wGate3SyncMode;		//闸门3同步模式
	WORD wGate3SyncThreshold;	//闸门3同步阈值
	WORD wGate3Start;			//闸门3起点			
	WORD wGate3Width;           //闸门3宽度 
	WORD wGate3Amp;				//闸门3幅值
	WORD wGate4Enable;			//闸门4使能
	WORD wGate4SyncMode;		//闸门4同步模式
	WORD wGate4SyncThreshold;	//闸门4同步阈值
	WORD wGate4Start;			//闸门4起点			
	WORD wGate4Width;           //闸门4宽度 
	WORD wGate4Amp;				//闸门4幅值	
	WORD wSimpleInterval;		//通道采样间隔
	WORD wSimpleAverage;		//通道1采样平均数	
	WORD wDelay;				//通道延迟时间
	WORD wProbeFreq;			// 探头频率
	WORD wTemp;
	// Song Chenguang add end on 2011-08-10
	CH_CONFIG_HD()
	{
		CH_DEM_MODE = 0;
		MIDDLE_VALUE = 0;
		CH_FRONTTIME = 0;
		CH_REARTIME = 0;
		CH_MAXBACKTIME = 0;
		CH_BEMINDATA = 0;
		CH_GAIN_DIFF = 0;
		CH_GAIN_A = 0;			
		CH_GAIN_B = 0;
		CH_GAIN_C = 0;
		CH_GAIN_D = 0;			
		CH_REJECT_VALUE = 0;
		CH_ZEROCORRECT = 0;
		CH_FLAW_GAIN_MUX = 0;
		CH_BE_GAIN_MUX = 0;
		CH_TGC_DAC_EN = 0;
		CH_FLAW_SURF_DISTANCE = 0;
		CH_FLAW_BE_DISTANCE = 0;
		CH_EN = 0;
		AUTO_GAIN_EN = 0;
		AUTO_GATE_EN = 0;
		ALARM_EN = 0;
		CH_REF_NO = 0;
		CH_FLAW_DEF = 0;						
		CH_SELFCHECK = 0;
		CH_MONITOR_RANGE = 0;					
		CH_NO = 0;
		wGate1Enable = 0;
		wGate1SyncMode = 0;
		wGate1SyncThreshold = 0;
		wGate1Start = 0;
		wGate1Width = 0;
		wGate1Amp = 0;
		wGate2Enable = 0;
		wGate2SyncMode = 0;
		wGate2SyncThreshold = 0;
		wGate2Start = 0;
		wGate2Width = 0;
		wGate2Amp = 0;
		wGate3Enable = 0;
		wGate3SyncMode = 0;
		wGate3SyncThreshold = 0;
		wGate3Start = 0;
		wGate3Width = 0;
		wGate3Amp = 0;
		wGate4Enable = 0;
		wGate4SyncMode = 0;
		wGate4SyncThreshold = 0;
		wGate4Start = 0;
		wGate4Width = 0;
		wGate4Amp = 0;
		wSimpleInterval = 0;
		wSimpleAverage = 0;
		wDelay = 0;
		wProbeFreq = 2;			// 探头频率
		wTemp = 0;
	}

	~CH_CONFIG_HD()
	{
	}
} ;


// 闸门类
class CGateInfo
{
public:
	BOOL m_bOpen;				// 开启标志
	double m_fFrontGate;		// 前沿
	double m_fBackGate;		// 宽度
	double m_fPeakGate;		// 峰值
	COLORREF m_colorGate;	//闸门颜色
	int m_nMtpMode;//同步模式
	double m_fMTPPeak;//同步阈值
public:
	CGateInfo()
	{
		m_bOpen = FALSE;			//开关
		m_fFrontGate = 0.0f;		// 前沿
		m_fBackGate = 10.0f;		// 宽度
		m_fPeakGate = 50;			//幅值
		m_colorGate = RGB(0,255,0);//闸门颜色
		m_nMtpMode = FALSE;	
		m_fMTPPeak = 0.0f;			//同步阈值
	}

	~CGateInfo()
	{
	}

	inline CGateInfo& operator=(const CGateInfo& rhs)
	{
		if(this == &rhs)
			return * this;
		m_bOpen = rhs.m_bOpen;
		m_fFrontGate = rhs.m_fFrontGate;
		m_fBackGate = rhs.m_fBackGate;
		m_fPeakGate = rhs.m_fPeakGate;
		m_colorGate = rhs.m_colorGate;
		m_nMtpMode = rhs.m_nMtpMode;
		m_fMTPPeak = rhs.m_fMTPPeak;

		return *this;
	}
};

enum PROBE_FREQ_e{TYPE_1M = 0, TYPE_2M,TYPE_205M,TYPE_5M,  TYPE_WIDE, TYPE_05M, TYPE_2MHz,TYPE_1MHz};
enum DETECT_OSCILLATION_MODE_e{PLUS_HWAVE = 0, MINUS_HWAVE, FULL_WAVE, RF};
enum VOLTAGE_TYPE_e {TYPE_50 = 0, TYPE_400, TYPE_ERROR};
enum IMPEDANCE_e {TYPE_0 = 0, TYPE_1 = 1, TYPE_2 = 2};
enum LVBO_TYPE_e {HIGH_LVBO = 0, LOWER_LVBO, BAND_LVBO};
enum ENERGY_e {LOW_ENERGY = 0, HIGH_ENERGY = 1, DEFAULT_ENERGY = 2};//yachang.huang add on 2013-11-25
enum RevAndSendMode_e {OneSend_OneRev = 0, SelfSend_SelfRev = 1, Default_Mode = 2};//yachang.huang add on 2013-11-25
enum SPI_ADDR_e{TYPE_RDA11 = 0,TYPE_RDA12,TYPE_RDA22,TYPE_RDA21,TYPE_FIL_DA2, TYPE_FIL_DA1, TYPE_DA_AMP1,TYPE_DA_AMP2};//yachang.huang add on 2013-11-25
// 探头信息
class CProbeInfo
{
public:
	PROBE_FREQ_e m_eProbeFreq;  // 探头频率	
	VOLTAGE_TYPE_e m_eInterruptVoltage;		// 激发电压
	WORD m_wReFreq;				// 重复频率
	WORD m_wPlusWidth;			// 脉冲宽度
	DETECT_OSCILLATION_MODE_e m_eDisplayMode;	// 检波模式
	IMPEDANCE_e m_eImpedance;	// 阻抗
	ENERGY_e m_eEnergy;//yachang.huang add on 2013-11-25
	RevAndSendMode_e m_eRevAndSendMode;//yachang.huang add on 2013-11-25
	SPI_ADDR_e m_eSpiAddr;//yachang.huang add on 2013-11-25
	int m_nSpiData;//yachang.huang add on 2013-11-25
	float m_fLvBoHigh;			// 高通滤波值
	float m_fLvBoLow;			// 高通滤波值
	LVBO_TYPE_e m_eLvBoType;	// 滤波类型
	WORD m_wWorkMode;			// 工作模式
	float m_RefPlus;			// 探伤灵敏度

public:
	CProbeInfo()
	{
		m_eProbeFreq = TYPE_1M;
		m_wReFreq = MIN_RE_FREQ;
		m_eInterruptVoltage = TYPE_400;
		m_wPlusWidth = MIN_MAIKUAN;
		m_eDisplayMode = FULL_WAVE;
		m_eImpedance = TYPE_0;
		m_fLvBoHigh = MAX_LVBO;
		m_fLvBoLow = MIN_LVBO;
		m_eLvBoType = HIGH_LVBO;
		m_wWorkMode = 1;
		m_RefPlus = 0.0f;
		m_eEnergy = HIGH_ENERGY;//yachang.huang add on 2013-11-25
		m_eRevAndSendMode = SelfSend_SelfRev;//yachang.huang add on 2013-11-25
		m_eSpiAddr = TYPE_RDA11;//yachang.huang add on 2013-11-25
	}

	~CProbeInfo()
	{

	}

	inline CProbeInfo& operator=(const CProbeInfo& rhs)
	{
		if(this == &rhs)
			return * this;

		m_eProbeFreq = rhs.m_eProbeFreq;
		m_wReFreq = rhs.m_wReFreq;
		m_eInterruptVoltage = rhs.m_eInterruptVoltage;
		m_wPlusWidth = rhs.m_wPlusWidth;
		m_eDisplayMode = rhs.m_eDisplayMode;
		m_eImpedance = rhs.m_eImpedance;
		m_fLvBoHigh = rhs.m_fLvBoHigh;
		m_fLvBoLow = rhs.m_fLvBoLow;
		m_eLvBoType = rhs.m_eLvBoType;
		m_wWorkMode = rhs.m_wWorkMode;
		m_RefPlus = rhs.m_RefPlus;
		m_eEnergy = rhs.m_eEnergy;
		m_eRevAndSendMode = rhs.m_eRevAndSendMode;
		m_eSpiAddr = rhs.m_eSpiAddr;
		return *this;
	}

};



// 通道信息
class CChannelInfo
{
public:
	CGateInfo m_GateInfo[GATE_NUM];	// 闸门信息
	float m_fChannelPlus;		// 增益
	float m_fChannelRefPlus;	// 参考增益		// Song Chenguang add on 2012-07-25
	float m_fChannelDelay;		// 延迟
	float m_fChannelRange;		// 范围
	int m_nChSI;				// 采样间隔
	int m_nChSavg;				// 采样平均数
	float m_fZeroPoint;			// 零点
	float m_fRestrain;			// 抑制
	
	CProbeInfo m_ProbeInfo;		// 探头相关数据
	int m_nWaveSpeed;			// 声速
	BOOL m_bOpen;				// 开关标志
//	int m_nMidValue;            // 中值  //yachang.huang add on 2013-11-27


public:
	CChannelInfo()
	{
		m_fChannelPlus = 0.0f;
		m_fChannelRefPlus = 0.0f;	// Song Chenguang add on 2012-07-25
		m_fChannelDelay = 0.0f;
		m_fChannelRange = 30.0f;
		m_nChSI = 0;
		m_nChSavg = 0;
		m_fZeroPoint = 0.0f;
		m_fRestrain = 0.0f;
		m_nWaveSpeed = WAVESPEED_STEEL;
		m_bOpen = TRUE;
//		m_nMidValue = 504;//yachang.huang add on 2013-11-27
	}

	~CChannelInfo()
	{

	}
	inline CChannelInfo& operator=(const CChannelInfo& rhs)
	{
		if(this == &rhs)
			return * this;
		m_GateInfo[0] = rhs.m_GateInfo[0];
		m_GateInfo[1] = rhs.m_GateInfo[1];
		m_GateInfo[2] = rhs.m_GateInfo[2];
		m_GateInfo[3] = rhs.m_GateInfo[3];
		m_fChannelPlus = rhs.m_fChannelPlus;
		m_fChannelRefPlus = rhs.m_fChannelRefPlus;		// Song Chenguang add on 2012-07-25
		m_fChannelDelay = rhs.m_fChannelDelay;
		m_fChannelRange = rhs.m_fChannelRange;
		m_nChSI = rhs.m_nChSI;
		m_nChSavg = rhs.m_nChSavg;
		m_fZeroPoint = rhs.m_fZeroPoint;
		m_nWaveSpeed = rhs.m_nWaveSpeed;
		m_ProbeInfo = rhs.m_ProbeInfo;
		m_fRestrain = rhs.m_fRestrain;
		m_bOpen = rhs.m_bOpen;
//		m_nMidValue = rhs.m_nMidValue;   //yachang.huang add on 2013-11-27

		return *this;
	}

};

// 闸门特征值
typedef struct{
	WORD wXLow16;					//X方向低16位
	WORD wXHigh16;					//X方向高16位
	WORD wYLow16;					//Y方向低16位
	WORD wYHigh16;					//Y方向高16位
	WORD wZLow16;					//z方向低16位
	WORD wZHigh16;					//z方向高16位
	WORD wW1Low16;					//W1方向低16位
	WORD wW1High16;					//W1方向高16位
	WORD wALow16;					//A方向低16位
	WORD wAHigh16;					//A方向高16位
	WORD wBLow16;					//B方向低16位
	WORD wBHigh16;					//B方向高16位
	WORD wW2Low16;					//W2方向低16位
	WORD wW2High16;					//W2方向高16位
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
	WORD wGate3FPTime;				//闸门3内第一个波峰波峰值时间
	WORD wGate3FPAmp;				//闸门3内第一个波峰波峰值幅度
	WORD wGate4SyncTime;			//闸门4同步时间
	WORD wGate4MPFirstTime;			//闸门4内最大波峰过阈值时间
	WORD wGate4MPTime;				//闸门4内最大波峰值时间
	WORD wGate4MPAmp	;			//闸门4内最大波峰值幅度
	WORD wGate4FPFirstTime;			//闸门4内第一个波峰过阈值时间
	WORD wGate4FPTime;				//闸门4内第一个波峰波峰值时间
	WORD wGate4FPAmp;				//闸门4内第一个波峰波峰值幅度
} CH_FLAWDATA_CSCAN;

// 精简版特征数据
typedef struct{
	WORD wXLow16;					//X方向低16位
	WORD wXHigh16;					//X方向高16位
	WORD wYLow16;					//Y方向低16位
	WORD wYHigh16;					//Y方向高16位
	WORD wZLow16;					//z方向低16位
	WORD wZHigh16;					//z方向高16位
	WORD wW1Low16;					//W1方向低16位
	WORD wW1High16;					//W1方向高16位
	WORD wALow16;					//A方向低16位
	WORD wAHigh16;					//A方向高16位
	WORD wBLow16;					//B方向低16位
	WORD wBHigh16;					//B方向高16位
	WORD wW2Low16;					//W2方向低16位
	WORD wW2High16;					//W2方向高16位
	WORD wGate1SyncTime;			//闸门1同步时间
	WORD wGate1MPTime;				//闸门1内最大波峰值时间
	WORD wGate1MPAmp	;			//闸门1内最大波峰值幅度
	WORD wGate2MPTime;				//闸门2内最大波峰值时间
	WORD wGate2MPAmp	;			//闸门2内最大波峰值幅度
	WORD wGate2FPFirstTime;			//闸门2内第一个波峰过阈值时间
	WORD wGate2FPTime;				//闸门2内第一个波峰波峰值时间
	WORD wGate2FPAmp;				//闸门2内第一个波峰波峰值幅度
	WORD wGate3MPTime;				//闸门3内最大波峰值时间
	WORD wGate3MPAmp	;			//闸门3内最大波峰值幅度
	WORD wGate3FPFirstTime;			//闸门3内第一个波峰过阈值时间
	WORD wGate3FPTime;				//闸门3内第一个波峰波峰值时间
	WORD wGate3FPAmp;				//闸门3内第一个波峰波峰值幅度
	WORD wGate4MPTime;				//闸门4内最大波峰值时间
	WORD wGate4MPAmp	;			//闸门4内最大波峰值幅度
	WORD wGate4FPFirstTime;			//闸门4内第一个波峰过阈值时间
	WORD wGate4FPTime;				//闸门4内第一个波峰波峰值时间
	WORD wGate4FPAmp;				//闸门4内第一个波峰波峰值幅度
} CH_FLAWDATA_CSCAN_S;
// Song Chenguang add start on 2013-12-18
// A扫数据加强版
class CPicData_L
{
public:
	CPicData_L()
	{
		::ZeroMemory(m_wData, sizeof(m_wData));
		m_fX = 0.0f;
		m_fY = 0.0f;
		m_fZ = 0.0f;
		m_fA = 0.0f;
		m_fB = 0.0f;
		m_fW = 0.0f;
		m_fR = 0.0f;
		::ZeroMemory(&m_stFlawData, sizeof(CH_FLAWDATA_CSCAN_S));
		m_nCardIndex = 0;
		m_nChannelIndex = 0;

		int nSize = MONITOR_DATA_SIZE/AD_SIZE;
		for (int i=0; i<nSize; i++)
		{
			m_wData[i]= 10;
		}
	}

	~CPicData_L()
	{
	}
	inline CPicData_L& operator=(const CPicData_L& rhs)
	{
		if(this == &rhs)
			return * this;

		m_fX = rhs.m_fX;
		m_fY = rhs.m_fY;
		m_fZ = rhs.m_fZ;
		m_fA = rhs.m_fA;
		m_fB = rhs.m_fB;
		m_fW = rhs.m_fW;
		m_fR = rhs.m_fR;
		memcpy(m_wData,rhs.m_wData, sizeof(m_wData));
		m_stFlawData = rhs.m_stFlawData;
		m_nCardIndex = rhs.m_nCardIndex;
		m_nChannelIndex = rhs.m_nChannelIndex;

		return *this;
	}
public:
	WORD m_wData[MONITOR_DATA_SIZE/AD_SIZE];
	float m_fX;
	float m_fY;
	float m_fZ;
	float m_fA;
	float m_fB;
	float m_fW;
	float m_fR;
	CH_FLAWDATA_CSCAN_S m_stFlawData;
	int m_nCardIndex;
	int m_nChannelIndex;
};
// Song Chenguang add end on 2013-12-18
// 监视数据
typedef struct tagCSCAN_CH_SEND_MONITOR_DATA{
	WORD wPicData[DATA_SIZE];
	CH_FLAWDATA_CSCAN_S stFeatureData;
}CSCAN_CH_SEND_MONITOR_DATA_t;
class CFeatureInfo
{
public:
	CFeatureInfo()
	{
		m_dX = 0.0;
		m_dY = 0.0;
		m_dZ = 0.0;
		m_dR = 0.0;
		m_dA = 0.0;
		m_dB = 0.0;
		m_dW = 0.0;

		m_nGateNum = 1;
		m_nFeatureNum = 1;

		m_nCount = 0;

		m_pfFeatures = new float*[m_nGateNum];
		if (NULL != m_pfFeatures) //add by wangqianfeng on 20121106
		{
			for (int j =0; j<m_nGateNum; j++)
			{
				m_pfFeatures[j] = NULL;
			}
		}
		for (int i = 0; i< m_nGateNum; i++)
		{
			m_pfFeatures[i] = new float[m_nFeatureNum];
			if (NULL != m_pfFeatures[i]) //add by wangqianfeng on 20121106
			{
				::ZeroMemory(m_pfFeatures[i], sizeof(float) * m_nFeatureNum);
			}
		}
	}

	CFeatureInfo(int nGateNum, int nFeatureNum)
	{
		m_dX = 0.0;
		m_dY = 0.0;
		m_dZ = 0.0;
		m_dR = 0.0;
		m_dA = 0.0;
		m_dB = 0.0;
		m_dW = 0.0;

		if (nGateNum < 1)
			nGateNum = 1;
		if (nFeatureNum < 1)
			nFeatureNum = 1;

		m_nCount = 0;

		m_pfFeatures = new float*[nGateNum];
		if (NULL != m_pfFeatures) //add by wangqianfeng on 20121106
		{
				for (int j =0; j<nGateNum; j++)
				{
					m_pfFeatures[j] = NULL;
				}
		}
		for (int i = 0; i< nGateNum; i++)
		{
			m_pfFeatures[i] = new float[nFeatureNum];
			if (NULL != m_pfFeatures[i]) //add by wangqianfeng on 20121106
			{
				::ZeroMemory(m_pfFeatures[i], sizeof(float) * nFeatureNum);
			}
		}

		m_nGateNum = nGateNum;
		m_nFeatureNum = nFeatureNum;
	}

	~CFeatureInfo()
	{
		if (m_pfFeatures != NULL)
		{
 			for(int i =0; i<m_nGateNum; i++)
 			{
				delete [](m_pfFeatures[i]);
 				m_pfFeatures[i] = NULL;
 			}
			delete []m_pfFeatures;
			m_pfFeatures = NULL;
		}
	}
	inline CFeatureInfo& operator = (const CFeatureInfo& rhs)
	{
		if (this == &rhs)
			return *this;
		
		m_dX = rhs.m_dX;
		m_dY = rhs.m_dY;
		m_dZ = rhs.m_dZ;
		m_dR = rhs.m_dR;
		m_dA = rhs.m_dA;
		m_dB = rhs.m_dB;
		m_dW = rhs.m_dW;

		if (m_pfFeatures != NULL)
		{
			for(int i =0; i<m_nGateNum; i++)
			{
				delete [](m_pfFeatures[i]);
				m_pfFeatures[i] = NULL;
			}
			delete []m_pfFeatures;
			m_pfFeatures = NULL;
		}

		m_nGateNum = rhs.m_nGateNum;
		m_nFeatureNum = rhs.m_nFeatureNum;

		m_nCount = rhs.m_nCount;
		
		if (m_nGateNum > 0)
		{
			m_pfFeatures = new float*[m_nGateNum];
			if (NULL != m_pfFeatures) //add by wangqianfeng on 20121106
			{
				for (int j =0; j<m_nGateNum; j++)
				{
					m_pfFeatures[j] = NULL;
				}
			}
			for (int i = 0; i< m_nGateNum; i++)
			{
				m_pfFeatures[i] = new float[m_nFeatureNum];
				if (NULL != m_pfFeatures[i]) //add by wangqianfeng on 20121106
				{
					memcpy(m_pfFeatures[i], rhs.m_pfFeatures[i], sizeof(float) * m_nFeatureNum);
				}
			}
		}		
	
		return *this;
	}

	CFeatureInfo(const CFeatureInfo& rhs)
	{
		m_dX = rhs.m_dX;
		m_dY = rhs.m_dY;
		m_dZ = rhs.m_dZ;
		m_dR = rhs.m_dR;
		m_dA = rhs.m_dA;
		m_dB = rhs.m_dB;
		m_dW = rhs.m_dW;

		m_nGateNum = rhs.m_nGateNum;
		m_nFeatureNum = rhs.m_nFeatureNum;

		m_nCount = rhs.m_nCount;

		if (m_nGateNum > 0)
		{
			m_pfFeatures = new float*[m_nGateNum];
			if (NULL != m_pfFeatures) //add by wangqianfeng on 20121106
			{
				for (int j =0; j<m_nGateNum; j++)
				{
					m_pfFeatures[j] = NULL;
				}
			}
			for (int i = 0; i< m_nGateNum; i++)
			{
				m_pfFeatures[i] = new float[m_nFeatureNum];
				if (NULL != m_pfFeatures[i]) //add by wangqianfeng on 20121106
				{
					memcpy(m_pfFeatures[i], rhs.m_pfFeatures[i], sizeof(float) * m_nFeatureNum);
				}
			}
		}
	}
public:
	double m_dX;
	double m_dY;
	double m_dZ;
	double m_dR;
	double m_dA;
	double m_dB;
	double m_dW;
	int m_nGateNum;
	int m_nFeatureNum;
	int m_nCount;	// 相同叠加次数， 网格化算平均值时使用
	float **m_pfFeatures;	// 特征值二级指针
	//特征信号,4个门，每个门7个特征，共4*7=28个
	//	1、触发到界面（第一个回波）检测
	//	2、门内第一个峰值前的阈值时间
	//	3、第一个峰值（门内）时间
	//	4、门内最大峰值前的阈值时间
	//	5、最大峰值（门内）时间 MPMT
	//	6、门内第一个峰值幅度 FPMA
	//	7、门内最大峰值幅度 MPMA
};


//C扫成像参数
typedef struct{
	int nColor;				// 颜色选择
	int nDrawMode;			// 成像模式
	int nValueMode;			// 取值模式
	int nGateIndex;			// 闸门选择
	int nFeatureAmp;		// 幅值特征选择
	int nFeatureTime;		// 时间特征选择
	int nGate1Threshold;	// 闸门1阈值
	int nGate2Threshold;	// 闸门2阈值
	int nGate3Threshold;	// 闸门3阈值
	int nGate4Threshold;	// 闸门4阈值
	//Machao add start on 2013-06-27
	int nSizeThreshold;		// 高于（低于）阈值选择
	int nTubeBarYMode;		// 管棒材Y轴模式
	int nImageMode;			// 成像模式
	int nWorkpieceThickness;	// 工件厚度
	int nMinAcreage;		// 最小面积
	int nMinDisplayRange;	// 显示范围上限
	int nMaxDisplayRange;	// 显示范围下限
	int nProbePosition;		// 显示探头位置
	BOOL nCBroomMap;			// C扫图同步
	BOOL nChangeOrigin;		// 变更坐标原点
	//Machao add end on 2013-06-27
}CSCAN_PARAM_t;

// C扫数据文件头
class CCScanFileHeader
{
public:
	CCScanFileHeader()
	{
		m_fRangeX = 1200;
		m_fRangeY = 800;
		m_bMPFTimeDisplay = FALSE;
		m_bMPMTimeDisplay = TRUE;
		m_bMPMAltitudeDisplay = TRUE;
		m_bFPFTimeDisplay = FALSE;
		m_bFPMTimeDisplay = FALSE;
		m_bFPMAltitudeDisplay = FALSE;
		m_fThick = 20.0f;
		m_fGridX = 0.5;
		m_fGridY = 0.5;
		m_wWaveSpeed = 5980;
		m_wChannelNum = 1;
		m_dXStart = 0;
		m_dYStart = 0;
		m_dZStart = 0;
		m_dXEnd = 0;
		m_dYEnd = 0;
		m_dZEnd = 0;
		m_nAxisSelect = 0;
		m_nProbeNum = 1;		// 探头个数
		m_nScanType = 0;		// 扫查方式：并行或深度
		m_nFillType = 0;		// 填充方式：纵向或横向
		m_fProbeDistance = 0;	// 探头间距
		m_fChongDie  = 0;		// 重叠长度
		m_nWorkMode = 0;		//工作模式
	}

	~	CCScanFileHeader()
	{
	}
public:
	float m_fRangeX;	// 扫查架X范围
	float m_fRangeY;	// 扫查架Y范围
	CGateInfo m_GateInfo[CHANNEL_NUM][4];	// 闸门数据
	BOOL m_bMPFTimeDisplay;		// 最高波过阈值时间
	BOOL m_bMPMTimeDisplay;		// 最高波过伤时
	BOOL m_bMPMAltitudeDisplay;	// 最高波高度
	BOOL m_bFPFTimeDisplay;		// 一次波过阈值时间
	BOOL m_bFPMTimeDisplay;		// 一次波伤时
	BOOL m_bFPMAltitudeDisplay;	// 一次波高度
	float m_fThick;				// 工件厚度
	float m_fGridX;				// X网格间隔
	float m_fGridY;				// Y网格间隔
	WORD m_wWaveSpeed;			// 声速
	WORD m_wChannelNum;			// 通道个数
	float m_dXStart;
	float m_dYStart;
	float m_dZStart;
	float m_dXEnd;
	float m_dYEnd;
	float m_dZEnd;
	int m_nAxisSelect;
	double m_dPipeDiameter;
	int m_nProbeNum;			// 探头个数
	int m_nScanType;			// 扫查方式：并行或深度
	int m_nFillType;			// 填充方式：纵向或横向
	int m_nTwoProbeIndex;		// 两个探头组合方式
	float m_fProbeDistance;		// 探头间距
	float m_fChongDie;			// 重叠长度
	int m_nWorkMode;			//工作模式
};
typedef struct tagCURVEPOINT_DATA
{
	float fData;
	float fRange;
}CURVEPOINT_DATA_t;

typedef struct
{
	BOOL bSwitchFlag;
	float fPingDing;
	float fDingLiang;
	float fPanFei;
	float fFirstGain;
	int nPointNum;		// 曲线点数量
	CURVEPOINT_DATA_t stPointData[DAC_CURVE_NUM][DAC_CURVE_POINT_NUM];	//曲线点数据
}DAC_CURVE_t;

typedef struct tagTCG_CURVE
{
	BOOL bSwitchFlag;
	float fFirstGain;
	int nPointNum;		// 曲线点数量
	CURVEPOINT_DATA_t stPointData[TCG_CURVE_POINT_NUM];	//曲线点数据
}TCG_CURVE_t;

struct LOGICAL_POINT_t 
{
	float x;
	float y;
};
// Song Chenguang add start on 2013-06-20
class LOGICAL_REGION
{
public:
	LOGICAL_REGION()
	{
		m_stFrom.x = 0;
		m_stFrom.y = 0;
		m_stTo.x = 0;
		m_stTo.y = 0;
		m_fCenterDB = 0;
		m_fEdgeDB = 0;
		m_nInstallEdgeDB = 0;	// machao add on 2013-09-18
	}
	~LOGICAL_REGION()
	{
	}
public:
	LOGICAL_POINT_t m_stFrom;		// 左上点
	LOGICAL_POINT_t m_stTo;			// 右下点
	float m_fCenterDB;				// 中心点DB
	float m_fEdgeDB;				// 边缘点DB
	int m_nInstallEdgeDB;			// 设置边缘点	// machao add on 2013-09-18
};
// Song Chenguang add end on 2013-06-20
// 详细结果列表存储节点
class FlawNode
{
public:
	FlawNode()
	{
		m_fMinX = 0.0f;
		m_fMinY = 0.0f;
		m_fMaxX = 0.0f;
		m_fMaxY = 0.0f;
		m_fZ = 0.0f;
		m_fArea = 0.0f;
		m_strAscan = _T("");
		m_strPosition = _T("");
		m_strResult = _T("");
		m_strProc = _T("");
		m_strMemo = _T("");
		m_strSave = _T("");
		m_aScan = new CPicData;
		m_boolHasAscan = false;
		m_strAscanPath = _T("");
		m_fFuzhi = 0.0f;
	}
public:
	float m_fMinX;
	float m_fMinY;
	float m_fMaxX;
	float m_fMaxY;
	float m_fZ;
	float m_fArea;
	CString m_strAscan;
	CString m_strSave;
	CString m_strPosition;
	CString m_strResult;
	CString m_strProc;
	CString m_strMemo;
	CPicData *m_aScan;
	bool m_boolHasAscan;
	CString m_strAscanPath;
	float m_fFuzhi;
};

#pragma pack(push,1)
struct HPOINT
{
	double  x;
	double  y;
	double  z;
	char  type;
	double p1;
	double p2;
	double p3;
	double p4;
	double p5;
};

struct PlaneFlawHeader
{
	double min_x;
	double min_y;
	double max_x;
	double max_y;
	double z;
	int type;
	double area;
	int EdgeOrBody;
	int PointsCount;
	// yachang.huang add start on 2012-11-26
	float fShangBoFuZhiMax;
	float fShangBoFuZhiMin;
	float fDiBoFuZhiMax;
	float fDiBoFuZhiMin;
	// yachang.huang add end on 2012-11-26
	int nColorType; //每个伤的填充颜色种类,暂定1,2,3 //add by wangqianfeng 2012-11-30
	BOOL bDisplay;
};
#pragma pack(pop)

typedef struct tagPACKET_HEAD
{
	DWORD dwCommand;		//命令
	WORD wChannelIndex;		//通道号
	WORD wGateIndex;		//闸门号
	DWORD dwSize;				//该命令大小
} PACKET_HEAD_t;//网络数据自定义包头

enum WORKMODE{SingleChannelA = 1, SingleChannelB, DoubleChannels,
			  OneSendOneReceive, DanJingGuanLian, ShuangJingGuanLian};
/*
class SEND_PARAM_DATA
{
public:
	WORD nPress;	// 电压
	WORD nMaiKuan; // 脉宽
	DWORD nRefrainFrequency; // 重复频率
	float nAmend; // 修正
	float fQianYan;
public:
	SEND_PARAM_DATA()
	{
		nPress = MAX_PRESSURE;	// 电压
		nMaiKuan = 100; // 脉宽
		nRefrainFrequency = 50; // 重复频率
		nAmend = MIN_AMEND;
		fQianYan = 0;
	}

	inline SEND_PARAM_DATA& operator=(const SEND_PARAM_DATA& rhs)
	{
		if(this == &rhs)
			return * this;
		nPress = rhs.nPress;
		nMaiKuan = rhs.nMaiKuan;
		nRefrainFrequency = rhs.nRefrainFrequency;
		nAmend = rhs.nAmend;
		fQianYan = rhs.fQianYan;

		return *this;
	}	
};


class RECEIVE_PARAM_DATA
{
public:
	WORD nProbeFreq;	// 探头频率
	WORD nJianBoMode;	// 检波模式
	float fPlus;		// 基准增益
	float fRefPlus;		// 参考增益
	float fDelay;		// 延时
	WORD nLvBo;		// 滤波频带
	WORD wImpedance;		//单通道阻抗
public:
	RECEIVE_PARAM_DATA()
	{
		nProbeFreq = 5;	// 探头频率
		nJianBoMode = 2;	// 检波模式
		fPlus = 0.0f;			// 基准增益
		fRefPlus = 0.0f;		// 参考增益
		fDelay = 0.0f;		// 延时
		nLvBo = 255;		// 滤波频带
		wImpedance = 0;		//单通道阻抗
	}

	inline RECEIVE_PARAM_DATA& operator=(const RECEIVE_PARAM_DATA& rhs)
	{
		if(this == &rhs)
			return * this;
		nProbeFreq = rhs.nProbeFreq;
		nJianBoMode = rhs.nJianBoMode;
		fPlus = rhs.fPlus;
		fRefPlus = rhs.fRefPlus;
		fDelay = rhs.fDelay;
		nLvBo = rhs.nLvBo;
		wImpedance = rhs.wImpedance;

		return *this;
	}
};

class DISPLAY_SET_DLG_DATA
{
public:
	BOOL bSFlag;
	float fRange;
	WORD nWaveSpeed;
	float fZheSheJiao;
	WORD nBate;
public:
	DISPLAY_SET_DLG_DATA()
	{
		bSFlag = TRUE;
		fRange = MIN_RANGE;
		nWaveSpeed = VSOUND;
		fZheSheJiao = 0.0f;
		nBate = 0;
	}

	inline DISPLAY_SET_DLG_DATA& operator=(const DISPLAY_SET_DLG_DATA& rhs)
	{
		if(this == &rhs)
			return * this;

		bSFlag = rhs.bSFlag;
		fRange = rhs.fRange;
		nWaveSpeed = rhs.nWaveSpeed;
		fZheSheJiao = rhs.fZheSheJiao;
		nBate = rhs.nBate;

		return *this;
	}
};

enum alarm_modes{ SuperHigh = 0, LostWave };

class GATE_INFO
{
public:
	alarm_modes alarmMode;	// 报警模式
	BOOL bOpen;				// 开启标志
	float fFrontGate;		// 前沿
	float fBackGate;		// 后沿
	float fPeakGate;		// 峰值
public:
	GATE_INFO()
	{
		alarmMode = SuperHigh;
		bOpen = FALSE;

		fFrontGate = 0.0f;		// 前沿
		fBackGate = 10.0f;		// 后沿
		fPeakGate = 50;
	}

	inline GATE_INFO& operator=(const GATE_INFO& rhs)
	{
		if(this == &rhs)
			return * this;

		alarmMode = rhs.alarmMode;
		bOpen = rhs.bOpen;
		fFrontGate = rhs.fFrontGate;
		fBackGate = rhs.fBackGate;
		fPeakGate = rhs.fPeakGate;

		return *this;
	}
};

typedef struct tagOTHER_PARAM
{
	int nColor;	//颜色
	int nLCD;	// 背光
	int nVol;	//音量
	int nStoreType;	//存储介质

	tagOTHER_PARAM()
	{
		nColor = 0;
		nLCD = 5;
		nVol = 3;
		nStoreType = 0;
	}
} OTHER_PARAM_t;


typedef struct tagWIRELESS_ALL_PARAM
{
	WORD wCurChannel;	// 当前通道
	WORKMODE wWorkMode;	// 工作模式
	RECEIVE_PARAM_DATA stReceiveParam[2];
	DISPLAY_SET_DLG_DATA stDisplayParam[2];
	SEND_PARAM_DATA stSendParam[2];
	GATE_INFO stGatParam[2][2];
	OTHER_PARAM_t stOtherParam;
} WIRELESS_ALL_PARAM_t;//网络数据自定义全部参数

typedef struct tag_PEAK_INFO{
	double wDefectTime;
	double wDefectAmp;

	tag_PEAK_INFO()
	{
		wDefectTime = 0.0;
		wDefectAmp = 0.0;
	}
}PEAK_INFO;

typedef struct tagREAL_SEND_DATA
{
	WORD wPicData[CHANNEL_NUM][DATA_SIZE];
	PEAK_INFO stPeakInfo[CHANNEL_NUM][GATE_NUM - 1];
} REAL_SEND_DATA_t;//下位机实时发送数据
*/
// 小仪器端版本信息
typedef struct tagCSCAN_VERSION_DATA{
	float wHardWareVer;		// 硬件版本
	float wSoftWareVer;		// 软件版本
}CSCAN_VERSION_DATA_t;

struct CARD_INFO_t
{
	int nCardIndex;
	int nChannelIndex;
	int nStartPoint;
	int nNum;
};

typedef struct tagTCG_COMMU_DATA 
{
//	TCG_CURVE_t stTcgCurve;
	BOOL bOpenFlag;
	int nSramNum;
	WORD wSamplingInterval;//采样间隔  yachang.huang add on 2014-06-18
	float fSramGain[TCG_SRAM_NUM];
}TCG_COMMU_DATA_t;
// Song Chenguang add start on 2013-07-12
// copy from Machao
// 调色板色值
struct PLATTE
{
	COLORREF color;		// 颜色
	float fMaxPercent;	// 百分比上限
	float fMinPercent;	// 百分比下限
};

// Song Chenguang add end on 2013-07-12
#endif