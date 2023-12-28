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
	int nPointNum;		// ���ߵ�����
	CURVEPOINT_DATA_t stPointData[4][20];	//���ߵ�����
}DAC_CURVE_t;

typedef struct tagTCG_CURVE
{
	bool bSwitchFlag;
	float fFirstGain;
	int nPointNum;		// ���ߵ�����
	CURVEPOINT_DATA_t stPointData[20];	//���ߵ�����
}TCG_CURVE_t;

// Aɨ��������
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
	WORD wGateEn;		// ��ͨ���ײ�բ��ʹ�ܼĴ���
	WORD wGateStart;	// ��ͨ���ײ�բ��բ����ʼֵ
	WORD wGateWidth;	// ��ͨ���ײ�բ��բ�ſ��
	WORD wGateHeight;	// ��ͨ���ײ�բ��բ�ŷ���
}GATE_INFO;

typedef struct
{
	WORD wAPlus;	// ��ͨ��ͨ��A����
	WORD wBPlus;	// ��ͨ��ͨ��B����
	WORD wCPlus;	// ��ͨ��ͨ��C����
	WORD wDPlus;	// ��ͨ��ͨ��D����
}PLUS;

typedef struct
{
	GATE_INFO stGate[4];	// բ��I, բ��A, բ��B, բ��I2
	WORD wDelay;		// ��ʱ
	WORD wMiddleValue;	// ��ֵ
	WORD wDetectMode;	// �첨ģʽ
	WORD wSuppression;	// ����
}CH_SUB_CONFIG;

typedef struct
{
	PLUS stPlus;	// ����
	WORD wBottomFlawPlusDiff;	// ��ͨ��ͨ���˲��͵ײ������
	WORD wTCGEn;	// ��ͨ�� TCGʹ�ܼĴ���
	CH_SUB_CONFIG	stWaveConfigure[2];	// �ײ�ͨ�����á��˲�ͨ������
}CH_CONFIG;

typedef struct
{
	WORD wData[256];	// TCG����ֵ
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
	WORD wXLow;		// ����low
	WORD wXHigh;	// ����High
	WORD wSoftChannelIndex;	// ��ͨ����
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
	BOOL bRunFlag;			// ����: 0, ����: 1	
	BOOL bStarted;
	WORD wSetOneChannel;
	WORD wSelChannel;
	CH_CONFIG* pAllConfig;	// Doc��allCH_CONFIG�����ַ
	CPLXCommunication* pPLXCommunication;
	QObject* qObject;
	WD_PCI_SLOT deviceSlot;	// Slot���
	int nFlawOrBottom;	//�˲��ײ��л�����
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
	WORD wXLow16;					//X�����16λ
	WORD wXHigh16;					//X�����16λ
	WORD wGate1SyncTime;			//բ��1ͬ��ʱ��  1
	WORD wGate1MPAmp	;			//բ��1����󲨷�ֵ����   1
	WORD wGate2MPTime;				//բ��2����󲨷�ֵʱ��  1
	WORD wGate2MPAmp	;			//բ��2����󲨷�ֵ����  1
	WORD wGate3MPTime;				//բ��3����󲨷�ֵʱ��
	WORD wGate3MPAmp	;			//բ��3����󲨷�ֵ����
	WORD wGate4SyncTime;			//բ��4ͬ��ʱ��
	WORD wGate4MPAmp	;			//բ��4����󲨷�ֵ����
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
	int m_nCount;	// ��ͬ���Ӵ����� ������ƽ��ֵʱʹ��
	float **m_pfFeatures;
};

enum PROBE_FREQ_e{TYPE_1M = 0, TYPE_05M, TYPE_2M, TYPE_WIDE, TYPE_5M, TYPE_205M};
enum DETECT_OSCILLATION_MODE_e{PLUS_HWAVE = 0, MINUS_HWAVE, FULL_WAVE, RF};
enum VOLTAGE_TYPE_e {TYPE_50 = 0, TYPE_400};
enum IMPEDANCE_e {TYPE_0 = 0, TYPE_1 = 1};
enum LVBO_TYPE_e {HIGH_LVBO = 0, LOWER_LVBO, BAND_LVBO};

// ̽ͷ��Ϣ
class CProbeInfo
{
public:
	PROBE_FREQ_e m_eProbeFreq;  // ̽ͷƵ��	
	float m_fRefAngle;			//�����     ����-2013-8-6
	VOLTAGE_TYPE_e m_eInterruptVoltage;		// ������ѹ
	WORD m_wReFreq;				// �ظ�Ƶ��
	WORD m_wPlusWidth;			// ������
	DETECT_OSCILLATION_MODE_e m_eDisplayMode;	// �첨ģʽ
	IMPEDANCE_e m_eImpedance;	// �迹
	float m_fLvBoHigh;			// ��ͨ�˲�ֵ
	float m_fLvBoLow;			// ��ͨ�˲�ֵ
	LVBO_TYPE_e m_eLvBoType;	// �˲�����

public:
	CProbeInfo()
	{
		m_eProbeFreq = TYPE_1M;
		m_fRefAngle = 0.0f;					// ����-2013-8-6
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
// բ����
class CGateInfo
{
public:
	BOOL m_bOpen;				// ������־
	double m_fFrontGate;		// ǰ��
	double m_fBackGate;		// ���
	double m_fPeakGate;		// ��ֵ
	COLORREF m_colorGate;	//բ����ɫ
	int m_nMtpMode;//ͬ��ģʽ
	double m_fMTPPeak;//ͬ����ֵ
	int m_nJudgeMode;		//����ģʽ				����-2013-8-6
	int m_nStaNoise;		//������					����-2013-8-6
public:
	CGateInfo()
	{
		m_bOpen = FALSE;			//����
		m_fFrontGate = 0.0f;		// ǰ��
		m_fBackGate = 10.0f;		// ���
		m_fPeakGate = 50;			//��ֵ
		m_colorGate = RGB(0,255,0);//բ����ɫ
		m_nMtpMode = FALSE;	
		m_fMTPPeak = 0.0f;			//ͬ����ֵ
		m_nJudgeMode = 2;			//����ģʽ		����-2013-8-6
		m_nStaNoise = 0;			//������			����-2013-8-6
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
	CGateInfo m_GateInfo[4];	//բ����Ϣ
	float m_fChannelPlus;		//����
	float m_fChannelCPlus;		//�ο�����		����-2013-8-6 
	float m_fChannelDelay;		//�ӳ�
	float m_fChannelRange;		//��Χ
	int m_nChSI;				//�������
	int m_nChSavg;				//����ƽ����
	float m_fZeroPoint;			//���
	float m_fRestrain;			//����

	CProbeInfo m_ProbeInfo;		// ̽ͷ�������
	int m_nWaveSpeed;			// ����
	int m_nWaterWaveSpeed;		//ˮ����			����-2013-8-6
	BOOL m_bOpen;				// ���ر�־
public:
	CChannelInfo()
	{
		m_fChannelPlus = 0.0f;
		m_fChannelCPlus = 0.0f;	//�ο�����		����-2013-8-6					
		m_fChannelDelay = 0.0f;
		m_fChannelRange = 30.0f;
		m_nChSI = 0;
		m_nChSavg = 0;
		m_fZeroPoint = 0.0f;
		m_fRestrain = 0.0f;
		m_nWaveSpeed = WAVESPEED_STEEL;
		m_bOpen = TRUE;
		m_nWaterWaveSpeed = 340;//ˮ����			����-2013-8-6
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
