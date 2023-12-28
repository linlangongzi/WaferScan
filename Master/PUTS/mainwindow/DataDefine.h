/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 数据结构体定义
* Filename: DataDefine.h
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2013-06-06
*/


#ifndef _DATA_DEFINE_H_
#define _DATA_DEFINE_H_

#include <QString>
#include <QObject>
#include <QPoint>
#include <QRect>
#include "..\pci9054_dll\plx_lib_x.h"	// plx_dll.dll PCI卡操作动态链接库
#include "..\pci9054_dll\p9054_lib.h"
#include "windows.h"
#include "const.h"

class CPLXCommunication;
#define  CALCULATE_WHOLEPOINT 1
#define  CALCULATE_FRONTPOINT 3
#define  CALCULATE_SIDEPOINT  4
#define  CALCULATE_THICKPOINT 5
#define  CALCULATE_XPOINT     6
#define  CALCULATE_YPOINT     7


// 发布时应注意设置的地方
#define PUT_RELEASE 1

#if PUT_RELEASE
#	define HAVE_OPC 1
#	define PLC_DEBUG 0
#	define HAVE_PLX 1
#	define HAVE_DATA_BASE 1 
#	define HAVE_DOG 1
#else 
#	define HAVE_OPC 0
#	define PLC_DEBUG 1
#	define HAVE_PLX 0
#	define HAVE_DATA_BASE 0 
#	define HAVE_DOG 1
#endif
// <<--end


enum zero_pos{leftTop = 1,leftBottom,rightTop,rightBottom};

struct GuoLvQueXian
{
	bool IsFilter;				// 过滤开关标志
	bool IsTestPlate;			// 试板开关标志
	bool IsConformStandard;		// 符合规格标志
	bool IsCompareX1;
	bool IsCompareX2;
	bool IsCompareY1;
	bool IsCompareY2;
	double DifferenceX1;
	double DifferenceX2;
	double DifferenceY1;
	double DifferenceY2;
	double PointStartX1;
	double PointStartY1;
	double PointEndX1;
	double PointEndY1;
	double PointStartX2;
	double PointStartY2;
	double PointEndX2;
	double PointEndY2;
	double StandardLength;
	double StandardWidth;
	double StandardHeight;
	double StandardOffsetLength;
	double StandardOffsetHeight;
};

class CPlateInfo
{
public:
	QString m_strSteelNumber;		// 钢板号
	QString m_strSteelGrade;		// 钢板等级
	QString m_strFurnNumber;		// 炉号
	QString m_strTime;				// 时间
	float m_nPlateLength;			// 长度
	float m_nPlateWidth;			// 宽度
	float m_nPlateThickness;		// 厚度
	QString m_Memo1;				// 备注
	QString  m_Memo2;
	QString m_strContractNum;	    // 合同号 
	QString m_strBodyLevel;		    // 本体级别
	QString m_strEdgeLevel;		    // 边缘级别 
	QString m_strDetectStandard;	// 探伤标准
	QString m_strArtName;           // 工艺名称
	QString m_strProbeType;			// 探头类型

	int flag;                       //试板判断 0为普通板 1为试板 chenyuzhu add on 2014.5.13
 
	CPlateInfo()
	{

		m_nPlateLength = 10;
		m_nPlateWidth = 10;
		m_nPlateThickness = 10;
	}
};

typedef struct
{
	int index;	//
	P9054_HANDLE hDev;//卡句柄
	WORD nMain2AECThreadCommand; 
	WORD nAEC2MainThreadFlag;//没使用
	BOOL bRunFlag;			// 挂起: 0, 运行: 1	
	BOOL bStarted;	 //没使用
	WORD nSetOneChannel; //下发数据通道号
	WORD wSelChannel;   //切换
	CH_CONFIG* pAllConfig;	// CH_CONFIG数组地址
	HWND hWnd;				// A扫视图句柄
	CPLXCommunication* pPLXCommunication;
	HWND hWndFeatureInfo;	// 特征窗口句柄
	HWND hWndCScan;			// C扫窗口句柄
	WD_PCI_SLOT deviceSlot;	// Slot编号
	QObject* qObject;
	int nFlawOrBottom;	//伤波底波切换变量
}AECTHREAD_PARAM;

struct OtherParameters
{
	int edgeCar1BoxCount;
	int	edgeCar2BoxCount;
	int	insideCar1BoxCount;
	int	insideCar2BoxCount;
	int	edgeCarEachBoxChannelCount;
	int	insideCarEachBoxChannelCount;
	int	edgeCar1PhysicalChannelStart;
	int	edgeCar2PhysicalChannelStart;
	int	insideCar1PhysicalChannelStart;
	int	insideCar2PhysicalChannelStart;
	int	edgeCar1LogicalChannelStart;
	int	edgeCar2LogicalChannelStart;
	int	insideCar1LogicalChannelStart;
	int	insideCar2LogicalChannelStart;
	QString	chineseReportTitle;
	QString	englishReportTitle;
};

class CPicData
{
public:
	CPicData()
	{
		::ZeroMemory(m_wData, sizeof(m_wData));
		m_fX = 0.0f;
		m_fY = 0.0f;
		m_fZ = 0.0f;
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
		memcpy(m_wData,rhs.m_wData, sizeof(m_wData));
		return *this;
	}
public:
	WORD m_wData[MONITOR_DATA_SIZE/AD_SIZE];
	float m_fX;
	float m_fY;
	float m_fZ;
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



// 通道信息
class CChannelInfo
{
public:
	CGateInfo m_GateInfo[4];	//闸门信息
	float m_fChannelPlus;		//增益
	float m_fChannelDelay;		//延迟
	float m_fChannelRange;		//范围
	int m_nChSI;				//采样间隔
	int m_nChSavg;				//采样平均数
	float m_fZeroPoint;			//零点
	float m_fRestrain;			//抑制

	CProbeInfo m_ProbeInfo;		// 探头相关数据
	int m_nWaveSpeed;			// 声速
	BOOL m_bOpen;				// 开关标志
public:
	CChannelInfo()
	{
		m_fChannelPlus = 0.0f;
		m_fChannelDelay = 0.0f;
		m_fChannelRange = 30.0f;
		m_nChSI = 0;
		m_nChSavg = 0;
		m_fZeroPoint = 0.0f;
		m_fRestrain = 0.0f;
		m_nWaveSpeed = WAVESPEED_STEEL;
		m_bOpen = TRUE;
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
		m_fChannelDelay = rhs.m_fChannelDelay;
		m_fChannelRange = rhs.m_fChannelRange;
		m_nChSI = rhs.m_nChSI;
		m_nChSavg = rhs.m_nChSavg;
		m_fZeroPoint = rhs.m_fZeroPoint;
		m_nWaveSpeed = rhs.m_nWaveSpeed;
		m_ProbeInfo = rhs.m_ProbeInfo;
		m_fRestrain = rhs.m_fRestrain;
		m_bOpen = rhs.m_bOpen;

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

// 闸门特征值上位机
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
	WORD wChannelNo;
	WORD wCardNum;
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

//仅保留特征信号的数据点
struct	FeaturesData
{
	//坐标位置
	double	x;
	double	y;
	double	z;
	//数据结果
	//波形
	//特征信号,4个门，每个门7个特征，共4*7=28个
	//	1、触发到界面（第一个回波）检测
	//	2、门内第一个峰值前的阈值时间
	//	3、第一个峰值（门内）时间
	//	4、门内最大峰值前的阈值时间
	//	5、最大峰值（门内）时间
	//	6、门内第一个峰值幅度
	//	7、门内最大峰值幅度
	unsigned long features[4][7];
};


typedef struct 
{
	float length;
	float width;
	float height;			// 实际钢板厚度
	float fReportHeight;	// 报表需用的钢板厚度
	QString plateno;
	QString Num;
	float fRealLength;	
}PLATE_INFO;

#pragma pack(push,1)//对齐
// 点伤结构
struct stDianShang
{
	quint32 nCheckId;
	quint16 nDianShangTeZheng;
	float fX;
	float fY;
	float fZ;
	float fShangBoFuZhi;
	float fDiBoZ;
	float fDiBoFuZhi;
	float fShangBoZengYi;
	float fDiBoZengYi;
};

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
	float fShangBoFuZhiMax;
	float fShangBoFuZhiMin;
	float fDiBoFuZhiMax;
	float fDiBoFuZhiMin;
	int nColorType; //每个伤的填充颜色种类,暂定1,2,3 //add by wangqianfeng 2012-11-30
	int bDisplay; 
};

struct  PlaneFlawHeader2
{
	PlaneFlawHeader2()
	{
		min_x = 0.0;
		min_y = 0.0;
		max_x = 0.0;
		max_y = 0.0;
		z = 0.0;
		type = 0;
		area = 0.0;
		EdgeOrBody = 0;
		PointsCount = 0;
		bCalculated = FALSE;
		nMemeryPos = 0;
		bDisPlay = TRUE;
	}

	double min_x;
	double min_y;
	double max_x;
	double max_y;
	double z;
	int type;
	double area;
	int EdgeOrBody;
	int PointsCount;
	bool bCalculated;
	int nMemeryPos;
	bool bDisPlay;
	//POINT[]
};
#pragma pack(pop)

struct detectInfo
{
	float fShangBoFuZhi;
	float fDiBoZ;
	float fDiBoFuZhi;
	float fShangBoZengYi;
	float fDiBoZengYi;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//定义通道具体信息结构体

typedef struct 
{
	short CH_NO;         // 逻辑通道号
	short CH_FLAW_TIME;  // z 坐标
	short CH_FLAW_PEAK;	// 伤波幅值
	short CH_BE_TIME;	// 底波时间
	short CH_BE_PEAK;	// 底波幅度
	short CH_HEIGHT;    // 钢板厚度
	short CHX_GAIN;		// 增益
	short CH_FRONTTIME;	// 前沿
	short CH_REARTIME;	// 后沿
	short DETECT_STATE;	// 0失波，1 缺陷，2：超厚, 3:失耦；4：报警
	long CH_X;         // 编码器值 //chenyuzhu modify on 2014-8-9
}CH_FLAWDATA;

class DeltaX
{
public:
	DeltaX()
	{
		m_fBianTan1DeltaX = 0.0f;
		m_fBianTan2DeltaX = 0.0f;
		m_fNeiTan1DeltaX = 0.0f;
		m_fNeiTan2DeltaX = 0.0f;
		m_fNeiTan3DeltaX = 0.0f;
		m_fNeiTan4DeltaX = 0.0f;
		m_fNeitan1_Coder1_Distance = 0.0f;
		m_fNeitan2_Coder1_Distance = 0.0f;
		m_fNeitan3_Coder1_Distance = 0.0f;
		m_fNeitan4_Coder1_Distance = 0.0f;
		m_fCar1_Coder1_Distance = 0.0f;
		m_fCar2_Coder1_Distance = 0.0f;
	}

	~DeltaX(){}
public:
	float m_fBianTan1DeltaX;
	float m_fBianTan2DeltaX;
	float m_fNeiTan1DeltaX;
	float m_fNeiTan2DeltaX;
	float m_fNeiTan3DeltaX;
	float m_fNeiTan4DeltaX;
	float m_fNeitan1_Coder1_Distance;
	float m_fNeitan2_Coder1_Distance;
	float m_fNeitan3_Coder1_Distance;
	float m_fNeitan4_Coder1_Distance;
	float m_fCar1_Coder1_Distance;
	float m_fCar2_Coder1_Distance;
};


struct GlobalItemValue
{
	BYTE PLCSelfResult;
	BYTE dwPLC2WSCommand;
	WORD wBiantan1Status;
	WORD wBiantan2Status;

	ULONG ulPlateLength;
	ULONG ulPlateWidth;
	ULONG ulPlateHeight;
	ULONG ulTestMode;
	// Neitan1标定结果
	BYTE byNeitan1Box1Status;
	BYTE byNeitan1Box2Status;
	BYTE byNeitan1Box3Status;
	BYTE byNeitan1Box4Status;
	BYTE byNeitan1Box5Status;
	BYTE byNeitan1Box6Status;
	BYTE byNeitan1Box7Status;
	BYTE byNeitan1Box8Status;
	BYTE byNeitan1Box9Status;
	BYTE byNeitan1Box10Status;
	BYTE byNeitan1Box11Status;
	BYTE byNeitan1Box12Status;

	// Neitan2标定结果
	BYTE byNeitan2Box1Status;
	BYTE byNeitan2Box2Status;
	BYTE byNeitan2Box3Status;
	BYTE byNeitan2Box4Status;
	BYTE byNeitan2Box5Status;
	BYTE byNeitan2Box6Status;
	BYTE byNeitan2Box7Status;
	BYTE byNeitan2Box8Status;
	BYTE byNeitan2Box9Status;
	BYTE byNeitan2Box10Status;
	BYTE byNeitan2Box11Status;
	BYTE byNeitan2Box12Status;

	// Neitan3标定结果
	BYTE byNeitan3Box1Status;
	BYTE byNeitan3Box2Status;
	BYTE byNeitan3Box3Status;
	BYTE byNeitan3Box4Status;
	BYTE byNeitan3Box5Status;
	BYTE byNeitan3Box6Status;
	BYTE byNeitan3Box7Status;
	BYTE byNeitan3Box8Status;
	BYTE byNeitan3Box9Status;
	BYTE byNeitan3Box10Status;
	BYTE byNeitan3Box11Status;
	BYTE byNeitan3Box12Status;

	// Neitan4标定结果
	BYTE byNeitan4Box1Status;
	BYTE byNeitan4Box2Status;
	BYTE byNeitan4Box3Status;
	BYTE byNeitan4Box4Status;
	BYTE byNeitan4Box5Status;
	BYTE byNeitan4Box6Status;
	BYTE byNeitan4Box7Status;
	BYTE byNeitan4Box8Status;
	BYTE byNeitan4Box9Status;
	BYTE byNeitan4Box10Status;
	BYTE byNeitan4Box11Status;
	BYTE byNeitan4Box12Status;

	// 内探落下状态
	BYTE byA;
	BYTE byB;
	BYTE byC;
	BYTE byD;
	BYTE byE;
	BYTE byF;

	// 内探落下状态2
	// 	WORD wLiLun_NT1_A;
	// 	WORD wLiLun_NT1_B;
	// 	WORD wLiLun_NT2_A;
	// 	WORD wLiLun_NT2_B;
	DWORD dwLiLun_NT1;
	DWORD dwLiLun_NT2;


	// 探架与板边距离
	float fRefBody_Start;
	float fFreeBody_Start;

	// 探伤结果
	BYTE byPlateTestResult;

	GlobalItemValue()
	{
		PLCSelfResult = 0;
		dwPLC2WSCommand = 0;
		wBiantan1Status = 0;
		wBiantan2Status = 0;
		ulPlateLength = 0;
		ulPlateWidth = 0;
		ulPlateHeight = 0;
		ulTestMode = 0;

		// Neitan1标定结果
		byNeitan1Box1Status = 0;
		byNeitan1Box2Status = 0;
		byNeitan1Box3Status = 0;
		byNeitan1Box4Status = 0;
		byNeitan1Box5Status = 0;
		byNeitan1Box6Status = 0;
		byNeitan1Box7Status = 0;
		byNeitan1Box8Status = 0;
		byNeitan1Box9Status = 0;
		byNeitan1Box10Status = 0;
		byNeitan1Box11Status = 0;
		byNeitan1Box12Status = 0;

		// Neitan2标定结果
		byNeitan2Box1Status = 0;
		byNeitan2Box2Status = 0;
		byNeitan2Box3Status = 0;
		byNeitan2Box4Status = 0;
		byNeitan2Box5Status = 0;
		byNeitan2Box6Status = 0;
		byNeitan2Box7Status = 0;
		byNeitan2Box8Status = 0;
		byNeitan2Box9Status = 0;
		byNeitan2Box10Status = 0;
		byNeitan2Box11Status = 0;
		byNeitan2Box12Status = 0;

		// Neitan3标定结果
		byNeitan3Box1Status = 0;
		byNeitan3Box2Status = 0;
		byNeitan3Box3Status = 0;
		byNeitan3Box4Status = 0;
		byNeitan3Box5Status = 0;
		byNeitan3Box6Status = 0;
		byNeitan3Box7Status = 0;
		byNeitan3Box8Status = 0;
		byNeitan3Box9Status = 0;
		byNeitan3Box10Status = 0;
		byNeitan3Box11Status = 0;
		byNeitan3Box12Status = 0;

		// Neitan4标定结果
		byNeitan4Box1Status = 0;
		byNeitan4Box2Status = 0;
		byNeitan4Box3Status = 0;
		byNeitan4Box4Status = 0;
		byNeitan4Box5Status = 0;
		byNeitan4Box6Status = 0;
		byNeitan4Box7Status = 0;
		byNeitan4Box8Status = 0;
		byNeitan4Box9Status = 0;
		byNeitan4Box10Status = 0;
		byNeitan4Box11Status = 0;
		byNeitan4Box12Status = 0;

		// 内探落下状态
		byA = 0;
		byB = 0;
		byC = 0;
		byD = 0;
		byE = 0;
		byF = 0;

		// 探架与板边距离
		fRefBody_Start = 0.0f;
		fFreeBody_Start = 0.0f;
		byPlateTestResult = 0;
	}
};


class NeiTanBoxStartNum
{
public:
	NeiTanBoxStartNum()
	{
		m_nNeitan1StartNum = 0;
		m_nNeitan2StartNum = 0;
		m_nNeitan3StartNum = 0;
		m_nNeitan4StartNum = 0;
	}
public:
	unsigned short m_nNeitan1StartNum;
	unsigned short m_nNeitan2StartNum;
	unsigned short m_nNeitan3StartNum;
	unsigned short m_nNeitan4StartNum;
};


struct Flaw_Coordinate_Info
{
	QPoint point;
	unsigned short DETECT_STATE;
};


typedef struct
{
	UINT PhysicalNo;//物理通道号
	UINT LogicNo;//逻辑通道号
	BOOL isSelect;//是否选中	
}ST_CHANEL;


struct ThickOffset
{
	int x;
	int y;
	float value;
};

struct ZhiShiAreaRegion
{
	ZhiShiAreaRegion()
	{
		min_x = 0.0;
		min_y = 0.0;
		max_x = 0.0;
		max_y = 0.0;
	}

	double min_x;
	double min_y;
	double max_x;
	double max_y;
};

struct DetailInfo
{
public:
	DetailInfo()
	{
		m_BodyMaxZhiShiLength = 0.0f;
		m_BodyMaxArea = 0.0f;
		m_BodyMaxLength = 0.0f;
		m_BodyMaxWidth = 0.0f;
		m_EdgeMaxArea = 0.0f;
		m_EdgeMaxZhiShiLength = 0.0f;
		m_EdgeMaxLength = 0.0f;
		m_EdgeMaxWidth = 0.0f;
		m_BodyAvgQueXianLv = 0.0f;
		m_EdgeAvgQueXianLv = 0.0f;
		m_AllAvgQueXianLv = 0.0f;
		m_BodyMaxQueXianLv = 0.0f;
		m_EdgeMaxQueXianLv = 0.0f;
		m_AllMaxQueXianLv = 0.0f;
		m_perBodyMaxQueXianCount = 0.0f;
		m_perEdgeMaxQueXianCount = 0.0f;
		m_perAllMaxQueXianCount = 0.0f;
	}

	double m_BodyMaxZhiShiLength;
	double m_BodyMaxArea;
	double m_BodyMaxLength;
	double m_BodyMaxWidth;
	double m_EdgeMaxArea;
	double m_EdgeMaxZhiShiLength;
	double m_EdgeMaxLength;
	double m_EdgeMaxWidth;
	double m_BodyAvgQueXianLv;
	double m_EdgeAvgQueXianLv;
	double m_AllAvgQueXianLv;
	double m_BodyMaxQueXianLv;
	double m_EdgeMaxQueXianLv;
	double m_AllMaxQueXianLv;
	double m_perBodyMaxQueXianCount;
	double m_perEdgeMaxQueXianCount;
	double m_perAllMaxQueXianCount;
};
struct SurfaceFlawInfo
{
	int nMinX;
	int nMaxX;
	int nMinY;
	int nMaxY;
	double fArea;
	float fShangLength;
};

struct DLPointInfo  
{
	double minX; //长
	double maxX; //长
	double minY; //宽
	double maxY; //宽
	double z; //深
	double zhishiLength; //指示长度
	double mianShangArea; //面伤面积
	QRect  drawingPointRect; //伤点矩形
	QRect  PrintDrawRect; //报表伤点矩形//chenyuzhu add on 2014-7-31
};
// dong qi add start on 2013-11-22
enum StandarConditiondType
{
	STANDAR_GBT2970_AREA = 1,
	STANDAR_GBT2970_LENGTH = 2,
	STANDAR_GBT2970_DEFECTRATE = 3,
	STANDAR_GBT2970_EDGEMAXFLAW = 4,
	STANDAR_ASTM435_MAXDIAMETER,
	STANDAR_JISG0801_BODYDEFECTNUM,
	STANDAR_JISG0801_EDGEDEFECTNUM,
	STANDAR_JISG0801_BODYMAXFLAWLENGTH,
	STANDAR_JISG0801_PERMAXFLAWNUM,
	STANDAR_JISG0801_MAXBODYDEFECTRATE,
	STANDAR_JISG0801_EDGEMAXFLAWLENGTH,
	STANDAR_JISG0801_PERMAXEDGEFLAWNUM,
	STANDAR_JISG0801_MAXEDGEDEFECTRATE,
	STANDAR_JISG0901_AVERDEFECTRATE,
	STANDAR_JISG0901_MAXDEFECTRATE,
	STANDAR_ISO12094_MAX_BODY_AREA,
	STANDAR_ISO12094_AVE_FLAW_RATE,
	STANDAR_ISO12094_MAX_FLAW_RATE,
	STANDAR_ISO12094_MAX_EDGE_AREA,
	STANDAR_ISO12094_MAX_EDGE_LENGTH,
	STANDAR_ISO12094_MAX_EDGE_FLAWNUM_PERMETER,
	STANDAR_EU10160_MAX_BODY_AREA,
	STANDAR_EU10160_MAX_BODY_FLAWNUM_PERMETER,
	STANDAR_EU10160_MAX_EDGE_AREA,
	STANDAR_EU10160_MAX_EDGE_LENGTH,
	STANDAR_EU10160_MAX_EDGE_FLAWNUM_PERMETER,
	STANDAR_ASTM578_MAXDIAMETER
};

struct ZhiShiAreaRegionAndType
{
	ZhiShiAreaRegionAndType()
	{
		AreaRegion_t.min_x = 0.0;
		AreaRegion_t.min_y = 0.0;
		AreaRegion_t.max_x = 0.0;
		AreaRegion_t.max_y = 0.0;
		type = STANDAR_GBT2970_AREA;
		nGeneralOrMaxType = 0;
	}
	ZhiShiAreaRegion AreaRegion_t;
	StandarConditiondType type;
	int nGeneralOrMaxType;//0:一般伤 1：最大伤
};

struct ResultRectAndType
{
	QRect rect;
	StandarConditiondType type;
};

// *设置指示面积类型
// 1 = 为实际面积
// 2 = 所围矩形面积
const int AREA_REAL = 1;
const int AREA_RECTANGLE = 2;

// *设置指示长度类型
// 1 = 对角线长度为指示长度
// 2 = 以伤X方向值为指示长度
// 3 = 以伤Y方向值为指示长度
// 4 = 以x和y之间的较大值为指示长度
const int ZHISHI_LENGTH_DIAGONAL = 1;
const int ZHISHI_LENGTH_X = 2;
const int ZHISHI_LENGTH_Y = 3;
const int ZHISHI_LENGTH_MAX_XY = 4;

// *设置相邻伤合并时，判断距离的类型。
// 1 = 以指示长度小的为判断标准
// 2 = 以指示长度大的为判断标准
// 3 = 不合并
// 4 = 设定一个最大值，小于该值则合并
const int SHANG_MERGE_MIN_DISANCE = 1;
const int SHANG_MERGE_MAX_DISANCE = 2;
const int SHANG_NO_MERGE = 3;
const int SHANG_MERGE_SET_DISANCE = 4;

// *设置伤宽度类型
// 1 = 以伤X方向值为伤宽度
// 2 = 以伤Y方向值为伤宽度
// 3 = x和y之间的较小值
const int SHANG_WIDTH_X = 1;
const int SHANG_WIDTH_Y = 2;
const int SHANG_WIDTH_MIN_XY = 3;
// dong qi add end on 2013-11-22

#endif