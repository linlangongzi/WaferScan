#pragma once

class CPLXCommunication;
typedef struct tagCURVEPOINT_DATA
{
	float fData;
	float fRange;
}CURVEPOINT_DATA_t;

typedef struct
{
	bool bSwitchFlag;
	float fPingDing;
	float fDingLiang;
	float fPanFei;
	float fFirstGain;
	int nPointNum;		// 曲线点数量
	CURVEPOINT_DATA_t stPointData[4][20];	//曲线点数据
}DAC_CURVE_t;

typedef struct tagTCG_CURVE
{
	bool bSwitchFlag;
	float fFirstGain;
	int nPointNum;		// 曲线点数量
	CURVEPOINT_DATA_t stPointData[20];	//曲线点数据
}TCG_CURVE_t;

// A扫波形数据
class CPicData
{
public:
	WORD m_wData[MONITOR_SIZE];
	//	BOOL m_bDrawed;
	float m_fX;
	float m_fY;
	float m_fZ;

public:
	CPicData() {
		::memset(m_wData, 10, sizeof(m_wData));

		//		m_bDrawed = FALSE;
		m_fX = 0.0f;
		m_fY = 0.0f;
		m_fZ = 0.0f;
	}

	~CPicData() {}

	inline CPicData& operator=(const CPicData& rhs) {
		if(this == &rhs)
			return * this;

		m_fX = rhs.m_fX;
		m_fY = rhs.m_fY;
		m_fZ = rhs.m_fZ;
		memcpy(m_wData,rhs.m_wData, sizeof(m_wData));

		return *this;
	}
};


typedef struct
{
	WORD wGateEn;		// 软通道底波闸门使能寄存器
	WORD wGateStart;	// 软通道底波闸门闸门起始值
	WORD wGateWidth;	// 软通道底波闸门闸门宽度
	WORD wGateHeight;	// 软通道底波闸门闸门幅度
}GATE_INFO;

typedef struct
{
	WORD wAPlus;	// 软通道通道A增益
	WORD wBPlus;	// 软通道通道B增益
	WORD wCPlus;	// 软通道通道C增益
	WORD wDPlus;	// 软通道通道D增益
}PLUS;

typedef struct
{
	GATE_INFO stGate[4];	// 闸门I, 闸门A, 闸门B, 闸门I2
	WORD wDelay;		// 延时
	WORD wMiddleValue;	// 中值
	WORD wDetectMode;	// 检波模式
	WORD wSuppression;	// 抑制
}CH_SUB_CONFIG;

typedef struct
{
	PLUS stPlus;	// 增益
	WORD wBottomFlawPlusDiff;	// 软通道通道伤波和底波增益差
	WORD wTCGEn;	// 软通道 TCG使能寄存器
	CH_SUB_CONFIG	stWaveConfigure[2];	// 底波通道设置、伤波通道设置
}CH_CONFIG;

typedef struct
{
	WORD wData[256];	// TCG曲线值
}TCG_DATA;

typedef struct
{
	WORD wFlawTime;
	WORD wFlawHeight;
}GATE_FEATURE_DATA;

typedef struct  
{
	GATE_FEATURE_DATA stGateFeature[4];
}CH_FEATURE_DATA;

typedef struct  
{
	CH_FEATURE_DATA stBeFeature;
	CH_FEATURE_DATA stFlawFeature;
}CH_L_FEATURE_DATA;

typedef struct
{
	WORD wXLow;		// 长度low
	WORD wXHigh;	// 长度High
	WORD wSoftChannelIndex;	// 软通道号
	WORD wTemp1;
	CH_FEATURE_DATA  stData1To6[6];
	CH_L_FEATURE_DATA stData7To8[2];
	WORD wTemp2;
	WORD wTemp3;
	WORD wTemp4;
	WORD wTemp5;
	WORD wTemp6;
	WORD wTemp7;
}FEATURE_DATA;

typedef struct
{
	int index;
	P9054_HANDLE hDev;
	WORD wMain2AECThreadCommand; 
	WORD wAEC2MainThreadFlag;
	BOOL bRunFlag;			// 挂起: 0, 运行: 1	
	BOOL bStarted;
	WORD wSetOneChannel;
	WORD wSelChannel;
	CH_CONFIG* pAllConfig;	// Doc的allCH_CONFIG数组地址
	CPLXCommunication* pPLXCommunication;
	QObject* qObject;
	WD_PCI_SLOT deviceSlot;	// Slot编号
	int nFlawOrBottom;	//伤波底波切换变量
}AECTHREAD_PARAM;

typedef struct
{
	QString strOrderNumber;
	int nSteelNumber;
	int nSteelCount;
	int nSteelDia;
	int nSteelLength;
	float fSteelPly;
	QString strSteelRank;
}ORDER_PARAMETER;

typedef struct
{
	float fShowMaxLength;
	float fShowToleranceMin;
	float fShowToleranceMax;
	float fShowLimitsMin;
	float fShowLimitsMax;
	float fShowPly;
}SHOW_PARAMETER;

typedef struct
{
	int nMarkYOut;
	int nMarkYIn;
	int nMarkXOut;
	int nMarkXIn;
	int nMarkPlyCut;
	int nMarkPlyAdd;
	int nMarkLayering;
	int nYesCouplingNumber;
	int nYesCouplingLength;
	int nYesFaultNumber;
	int nYesFaultLength;
	int nNoCouplingNumber;
	int nNoCouplingLength;
	int nNoFaultNumber;
	int nNoFaultLength;
	int nCouplingNumber;
	int nCouplingLength;
	bool bSoundOpen;
	bool bLightOpen;
}JADGE_PARAMETER;

typedef struct{
	WORD wXLow16;					//X方向低16位
	WORD wXHigh16;					//X方向高16位
	WORD wGate1SyncTime;			//闸门1同步时间  1
	WORD wGate1MPAmp	;			//闸门1内最大波峰值幅度   1
	WORD wGate2MPTime;				//闸门2内最大波峰值时间  1
	WORD wGate2MPAmp	;			//闸门2内最大波峰值幅度  1
	WORD wGate3MPTime;				//闸门3内最大波峰值时间
	WORD wGate3MPAmp	;			//闸门3内最大波峰值幅度
	WORD wGate4SyncTime;			//闸门4同步时间
	WORD wGate4MPAmp	;			//闸门4内最大波峰值幅度
} CH_FLAWDATA_CSCAN_D;

class CFeatureInfo
{
public:
	CFeatureInfo()
	{
		m_dX = 0.0;
		m_dY = 0.0;
		m_dZ = 0.0;
		m_dW1 = 0.0;
		m_dA = 0.0;
		m_dB = 0.0;
		m_dW2 = 0.0;

		m_nGateNum = 1;
		m_nFeatureNum = 1;

		m_nCount = 0;

		m_pfFeatures = new float*[m_nGateNum];
		for (int j =0; j<m_nGateNum; j++)
		{
			m_pfFeatures[j] = NULL;
		}
		for (int i = 0; i< m_nGateNum; i++)
		{
			m_pfFeatures[i] = new float[m_nFeatureNum];
			::ZeroMemory(m_pfFeatures[i], sizeof(float) * m_nFeatureNum);
		}
	}

	CFeatureInfo(int nGateNum, int nFeatureNum)
	{
		m_dX = 0.0;
		m_dY = 0.0;
		m_dZ = 0.0;
		m_dW1 = 0.0;
		m_dA = 0.0;
		m_dB = 0.0;
		m_dW2 = 0.0;

		if (nGateNum < 1)
			nGateNum = 1;
		if (nFeatureNum < 1)
			nFeatureNum = 1;

		m_nCount = 0;

		m_pfFeatures = new float*[nGateNum];
		for (int j =0; j<nGateNum; j++)
		{
			m_pfFeatures[j] = NULL;
		}
		for (int i = 0; i< nGateNum; i++)
		{
			m_pfFeatures[i] = new float[nFeatureNum];
			::ZeroMemory(m_pfFeatures[i], sizeof(float) * nFeatureNum);
		}
//		::ZeroMemory(m_pfFeatures, sizeof(float) * nGateNum * nFeatureNum);

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
		m_dW1 = rhs.m_dW1;
		m_dA = rhs.m_dA;
		m_dB = rhs.m_dB;
		m_dW2 = rhs.m_dW2;

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
			for (int j =0; j<m_nGateNum; j++)
			{
				m_pfFeatures[j] = NULL;
			}
			for (int i = 0; i< m_nGateNum; i++)
			{
				m_pfFeatures[i] = new float[m_nFeatureNum];
				memcpy(m_pfFeatures[i], rhs.m_pfFeatures[i], sizeof(float) * m_nFeatureNum);
			}
		}		
	
		return *this;
	}

	CFeatureInfo(const CFeatureInfo& rhs)
	{
		m_dX = rhs.m_dX;
		m_dY = rhs.m_dY;
		m_dZ = rhs.m_dZ;
		m_dW1 = rhs.m_dW1;
		m_dA = rhs.m_dA;
		m_dB = rhs.m_dB;
		m_dW2 = rhs.m_dW2;
/*
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
*/
		m_nGateNum = rhs.m_nGateNum;
		m_nFeatureNum = rhs.m_nFeatureNum;

		m_nCount = rhs.m_nCount;

		if (m_nGateNum > 0)
		{
			m_pfFeatures = new float*[m_nGateNum];
			for (int j =0; j<m_nGateNum; j++)
			{
				m_pfFeatures[j] = NULL;
			}
			for (int i = 0; i< m_nGateNum; i++)
			{
				m_pfFeatures[i] = new float[m_nFeatureNum];
				memcpy(m_pfFeatures[i], rhs.m_pfFeatures[i], sizeof(float) * m_nFeatureNum);
			}
		}
	}
public:
	double m_dX;
	double m_dY;
	double m_dZ;
	double m_dW1;
	double m_dA;
	double m_dB;
	double m_dW2;
	int m_nGateNum;
	int m_nFeatureNum;
	int m_nCount;	// 相同叠加次数， 网格化算平均值时使用
	float **m_pfFeatures;
};

enum PROBE_FREQ_e{TYPE_1M = 0, TYPE_05M, TYPE_2M, TYPE_WIDE, TYPE_5M, TYPE_205M};
enum DETECT_OSCILLATION_MODE_e{PLUS_HWAVE = 0, MINUS_HWAVE, FULL_WAVE, RF};
enum VOLTAGE_TYPE_e {TYPE_50 = 0, TYPE_400};
enum IMPEDANCE_e {TYPE_0 = 0, TYPE_1 = 1};
enum LVBO_TYPE_e {HIGH_LVBO = 0, LOWER_LVBO, BAND_LVBO};

// 探头信息
class CProbeInfo
{
public:
	PROBE_FREQ_e m_eProbeFreq;  // 探头频率	
	float m_fRefAngle;			//折射角     王威-2013-8-6
	VOLTAGE_TYPE_e m_eInterruptVoltage;		// 激发电压
	WORD m_wReFreq;				// 重复频率
	WORD m_wPlusWidth;			// 脉冲宽度
	DETECT_OSCILLATION_MODE_e m_eDisplayMode;	// 检波模式
	IMPEDANCE_e m_eImpedance;	// 阻抗
	float m_fLvBoHigh;			// 高通滤波值
	float m_fLvBoLow;			// 高通滤波值
	LVBO_TYPE_e m_eLvBoType;	// 滤波类型

public:
	CProbeInfo()
	{
		m_eProbeFreq = TYPE_1M;
		m_fRefAngle = 0.0f;					// 王威-2013-8-6
		m_wReFreq = MIN_RE_FREQ;
		m_eInterruptVoltage = TYPE_50;
		m_wPlusWidth = MIN_MAIKUAN;
		m_eDisplayMode = FULL_WAVE;
		m_eImpedance = TYPE_0;
		m_fLvBoHigh = MAX_LVBO;
		m_fLvBoLow = MIN_LVBO;
		m_eLvBoType = HIGH_LVBO;
	}

	~CProbeInfo()
	{

	}

	inline CProbeInfo& operator=(const CProbeInfo& rhs)
	{
		if(this == &rhs)
			return * this;

		m_eProbeFreq = rhs.m_eProbeFreq;
		m_fRefAngle = rhs.m_fRefAngle;
		m_wReFreq = rhs.m_wReFreq;
		m_eInterruptVoltage = rhs.m_eInterruptVoltage;
		m_wPlusWidth = rhs.m_wPlusWidth;
		m_eDisplayMode = rhs.m_eDisplayMode;
		m_eImpedance = rhs.m_eImpedance;
		m_fLvBoHigh = rhs.m_fLvBoHigh;
		m_fLvBoLow = rhs.m_fLvBoLow;
		m_eLvBoType = rhs.m_eLvBoType;

		return *this;
	}

};
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
	int m_nJudgeMode;		//评判模式				王威-2013-8-6
	int m_nStaNoise;		//降噪数					王威-2013-8-6
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
		m_nJudgeMode = 2;			//评判模式		王威-2013-8-6
		m_nStaNoise = 0;			//降噪数			王威-2013-8-6
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
		m_nJudgeMode = rhs.m_nJudgeMode;
        m_nStaNoise = rhs.m_nStaNoise;

		return *this;
	}
};


class CChannelInfo
{
public:
	CGateInfo m_GateInfo[4];	//闸门信息
	float m_fChannelPlus;		//增益
	float m_fChannelCPlus;		//参考增益		王威-2013-8-6 
	float m_fChannelDelay;		//延迟
	float m_fChannelRange;		//范围
	int m_nChSI;				//采样间隔
	int m_nChSavg;				//采样平均数
	float m_fZeroPoint;			//零点
	float m_fRestrain;			//抑制

	CProbeInfo m_ProbeInfo;		// 探头相关数据
	int m_nWaveSpeed;			// 声速
	int m_nWaterWaveSpeed;		//水声速			王威-2013-8-6
	BOOL m_bOpen;				// 开关标志
public:
	CChannelInfo()
	{
		m_fChannelPlus = 0.0f;
		m_fChannelCPlus = 0.0f;	//参考增益		王威-2013-8-6					
		m_fChannelDelay = 0.0f;
		m_fChannelRange = 30.0f;
		m_nChSI = 0;
		m_nChSavg = 0;
		m_fZeroPoint = 0.0f;
		m_fRestrain = 0.0f;
		m_nWaveSpeed = WAVESPEED_STEEL;
		m_bOpen = TRUE;
		m_nWaterWaveSpeed = 340;//水声速			王威-2013-8-6
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
		m_fChannelCPlus = rhs.m_fChannelCPlus;
		m_fChannelDelay = rhs.m_fChannelDelay;
		m_fChannelRange = rhs.m_fChannelRange;
		m_nChSI = rhs.m_nChSI;
		m_nChSavg = rhs.m_nChSavg;
		m_fZeroPoint = rhs.m_fZeroPoint;
		m_nWaveSpeed = rhs.m_nWaveSpeed;
		m_ProbeInfo = rhs.m_ProbeInfo;
		m_fRestrain = rhs.m_fRestrain;
		m_nWaterWaveSpeed = rhs.m_nWaterWaveSpeed;
		m_bOpen = rhs.m_bOpen;

		return *this;
	}

};

struct GlobalItemValue
{
	BYTE PLCState;
	ULONG PipeLength; 
	BYTE ProbeInFlawFlag1;
	BYTE ProbeInFlawFlag2;
	BYTE ProbeInFlawFlag3;
	BYTE ProbeInFlawFlag4;
	BYTE ProbeInFlawFlag5;
	BYTE ProbeOutFlawFlag1;
	BYTE ProbeOutFlawFlag2;
	BYTE ProbeOutFlawFlag3;
	BYTE ProbeOutFlawFlag4;
	BYTE ProbeOutFlawFlag5;
	BYTE ProbeWallFlawFlag1;
	BYTE ProbeWallFlawFlag2;
	BYTE ProbeWallFlawFlag3;
	BYTE ProbeWallFlawFlag4;
	BYTE ProbeWallFlawFlag5;
	GlobalItemValue()
	{
		PLCState = 0;
	}
};
