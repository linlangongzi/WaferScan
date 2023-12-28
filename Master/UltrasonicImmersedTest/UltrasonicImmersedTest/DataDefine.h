#ifndef _DATA_DEFINE_H_
#define _DATA_DEFINE_H_

class CMainFrame;

// ���̴߳��ݲ���
// Songchenguang modified start on 2011-08-17
typedef struct
{
	int index;
	P9054_HANDLE hDev;
	WORD nMain2AECThreadCommand; 
	WORD nAEC2MainThreadFlag;
	BOOL bRunFlag;			// ����: 0, ����: 1	
	BOOL bStarted;
	WORD nSetOneChannel;
	WORD nSelChannel;
	CSCAN_ARS200FPGA_SETONE * pAllConfig;// modify by songchenguang 2012-01-16
	HWND hWnd;				// Aɨ��ͼ���
	HWND hWndFeatureInfo;	// �������ھ��
	HWND hWndCScan;			// Cɨ���ھ��  wangbingfu add at 2011-9-27
	WD_PCI_SLOT deviceSlot;	// Slot���
	CMainFrame* pFrame;
}AECTHREAD_PARAM;
// Songchenguang modified end on 2011-08-17

// Aɨ��������
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


// �·�ͨ������
class CH_CONFIG_HD
{
public:
	WORD CH_DEM_MODE;   // �첨ģʽ  ȫ��0�����벨1�����벨2
	WORD MIDDLE_VALUE;  // AD��ֵ
	WORD CH_FRONTTIME;	//բ��ǰ��	mm			
	WORD CH_REARTIME;   //բ�ź���  mm          
	WORD CH_MAXBACKTIME;	//���ײ�λ�� mm
	WORD CH_BEMINDATA;		//�ײ���ֵ
	//	WORD CH_TGC_DA_START;
	// Song Chenguang add start on 2008-11-11
	WORD CH_GAIN_DIFF;		// �˲�ͨ����ײ�ͨ�������ֵ
	// Song Chenguang add end on 2008-11-11
	WORD CH_GAIN_A;			
	WORD CH_GAIN_B;
	WORD CH_GAIN_C;
	WORD CH_GAIN_D;			//����ֵ		dB
	WORD CH_REJECT_VALUE;	//����
	WORD CH_ZEROCORRECT;	//�ӳ�		mm
	WORD CH_FLAW_GAIN_MUX;	//�˲�˥����	0dB��0��-6dB��1��-12dB:2��....-42dB��7
	WORD CH_BE_GAIN_MUX;	//�ײ�˥����	0dB��0��-6dB��1��-12dB:2��....-42dB��7
	WORD CH_TGC_DAC_EN;		//DAC����		�رգ�0��������1	
	WORD CH_FLAW_SURF_DISTANCE;				//��ä��		mm
	WORD CH_FLAW_BE_DISTANCE;				//��ä��		mm
	WORD CH_EN;								//ͨ������ �رգ�0��������1
	WORD AUTO_GAIN_EN;						//�Զ����� �رգ�0��������1
	WORD AUTO_GATE_EN;						//�Զ�բ�� �رգ�0��������1
	WORD ALARM_EN;							//�������� �رգ�0��������1
	WORD CH_REF_NO;							//��ʼͨ����
	WORD CH_FLAW_DEF;						
	WORD CH_SELFCHECK;						//�Լ쿪�� �رգ�0��������1
	WORD CH_MONITOR_RANGE;					
	WORD CH_NO;								//ͨ����
	// Song Chenguang add start on 2011-08-10
	WORD wGate1Enable;			//բ��1ʹ��	
	WORD wGate1SyncMode;		//բ��1ͬ��ģʽ
	WORD wGate1SyncThreshold;	//բ��1ͬ����ֵ
	WORD wGate1Start;			//բ��1���			
	WORD wGate1Width;           //բ��1��� 
	WORD wGate1Amp;				//բ��1��ֵ
	WORD wGate2Enable;			//բ��2ʹ��
	WORD wGate2SyncMode;		//բ��2ͬ��ģʽ
	WORD wGate2SyncThreshold;	//բ��2ͬ����ֵ
	WORD wGate2Start;			//բ��2���			
	WORD wGate2Width;           //բ��2��� 
	WORD wGate2Amp;				//բ��2��ֵ
	WORD wGate3Enable;			//բ��3ʹ��
	WORD wGate3SyncMode;		//բ��3ͬ��ģʽ
	WORD wGate3SyncThreshold;	//բ��3ͬ����ֵ
	WORD wGate3Start;			//բ��3���			
	WORD wGate3Width;           //բ��3��� 
	WORD wGate3Amp;				//բ��3��ֵ
	WORD wGate4Enable;			//բ��4ʹ��
	WORD wGate4SyncMode;		//բ��4ͬ��ģʽ
	WORD wGate4SyncThreshold;	//բ��4ͬ����ֵ
	WORD wGate4Start;			//բ��4���			
	WORD wGate4Width;           //բ��4��� 
	WORD wGate4Amp;				//բ��4��ֵ	
	WORD wSimpleInterval;		//ͨ���������
	WORD wSimpleAverage;		//ͨ��1����ƽ����	
	WORD wDelay;				//ͨ���ӳ�ʱ��
	WORD wProbeFreq;			// ̽ͷƵ��
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
		wProbeFreq = 2;			// ̽ͷƵ��
		wTemp = 0;
	}

	~CH_CONFIG_HD()
	{
	}
} ;


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
// ̽ͷ��Ϣ
class CProbeInfo
{
public:
	PROBE_FREQ_e m_eProbeFreq;  // ̽ͷƵ��	
	VOLTAGE_TYPE_e m_eInterruptVoltage;		// ������ѹ
	WORD m_wReFreq;				// �ظ�Ƶ��
	WORD m_wPlusWidth;			// ������
	DETECT_OSCILLATION_MODE_e m_eDisplayMode;	// �첨ģʽ
	IMPEDANCE_e m_eImpedance;	// �迹
	ENERGY_e m_eEnergy;//yachang.huang add on 2013-11-25
	RevAndSendMode_e m_eRevAndSendMode;//yachang.huang add on 2013-11-25
	SPI_ADDR_e m_eSpiAddr;//yachang.huang add on 2013-11-25
	int m_nSpiData;//yachang.huang add on 2013-11-25
	float m_fLvBoHigh;			// ��ͨ�˲�ֵ
	float m_fLvBoLow;			// ��ͨ�˲�ֵ
	LVBO_TYPE_e m_eLvBoType;	// �˲�����
	WORD m_wWorkMode;			// ����ģʽ
	float m_RefPlus;			// ̽��������

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



// ͨ����Ϣ
class CChannelInfo
{
public:
	CGateInfo m_GateInfo[GATE_NUM];	// բ����Ϣ
	float m_fChannelPlus;		// ����
	float m_fChannelRefPlus;	// �ο�����		// Song Chenguang add on 2012-07-25
	float m_fChannelDelay;		// �ӳ�
	float m_fChannelRange;		// ��Χ
	int m_nChSI;				// �������
	int m_nChSavg;				// ����ƽ����
	float m_fZeroPoint;			// ���
	float m_fRestrain;			// ����
	
	CProbeInfo m_ProbeInfo;		// ̽ͷ�������
	int m_nWaveSpeed;			// ����
	BOOL m_bOpen;				// ���ر�־
//	int m_nMidValue;            // ��ֵ  //yachang.huang add on 2013-11-27


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

// բ������ֵ
typedef struct{
	WORD wXLow16;					//X�����16λ
	WORD wXHigh16;					//X�����16λ
	WORD wYLow16;					//Y�����16λ
	WORD wYHigh16;					//Y�����16λ
	WORD wZLow16;					//z�����16λ
	WORD wZHigh16;					//z�����16λ
	WORD wW1Low16;					//W1�����16λ
	WORD wW1High16;					//W1�����16λ
	WORD wALow16;					//A�����16λ
	WORD wAHigh16;					//A�����16λ
	WORD wBLow16;					//B�����16λ
	WORD wBHigh16;					//B�����16λ
	WORD wW2Low16;					//W2�����16λ
	WORD wW2High16;					//W2�����16λ
	WORD wGate1SyncTime;			//բ��1ͬ��ʱ��
	WORD wGate1MPFirstTime;			//բ��1����󲨷����ֵʱ��
	WORD wGate1MPTime;				//բ��1����󲨷�ֵʱ��
	WORD wGate1MPAmp	;			//բ��1����󲨷�ֵ����
	WORD wGate1FPFirstTime;			//բ��1�ڵ�һ���������ֵʱ��
	WORD wGate1FPTime;				//բ��1�ڵ�һ�����岨��ֵʱ��
	WORD wGate1FPAmp;				//բ��1�ڵ�һ�����岨��ֵ����
	WORD wGate2SyncTime;			//բ��2ͬ��ʱ��
	WORD wGate2MPFirstTime;			//բ��2����󲨷����ֵʱ��
	WORD wGate2MPTime;				//բ��2����󲨷�ֵʱ��
	WORD wGate2MPAmp	;			//բ��2����󲨷�ֵ����
	WORD wGate2FPFirstTime;			//բ��2�ڵ�һ���������ֵʱ��
	WORD wGate2FPTime;				//բ��2�ڵ�һ�����岨��ֵʱ��
	WORD wGate2FPAmp;				//բ��2�ڵ�һ�����岨��ֵ����
	WORD wGate3SyncTime;			//բ��3ͬ��ʱ��
	WORD wGate3MPFirstTime;			//բ��3����󲨷����ֵʱ��
	WORD wGate3MPTime;				//բ��3����󲨷�ֵʱ��
	WORD wGate3MPAmp	;			//բ��3����󲨷�ֵ����
	WORD wGate3FPFirstTime;			//բ��3�ڵ�һ���������ֵʱ��
	WORD wGate3FPTime;				//բ��3�ڵ�һ�����岨��ֵʱ��
	WORD wGate3FPAmp;				//բ��3�ڵ�һ�����岨��ֵ����
	WORD wGate4SyncTime;			//բ��4ͬ��ʱ��
	WORD wGate4MPFirstTime;			//բ��4����󲨷����ֵʱ��
	WORD wGate4MPTime;				//բ��4����󲨷�ֵʱ��
	WORD wGate4MPAmp	;			//բ��4����󲨷�ֵ����
	WORD wGate4FPFirstTime;			//բ��4�ڵ�һ���������ֵʱ��
	WORD wGate4FPTime;				//բ��4�ڵ�һ�����岨��ֵʱ��
	WORD wGate4FPAmp;				//բ��4�ڵ�һ�����岨��ֵ����
} CH_FLAWDATA_CSCAN;

// �������������
typedef struct{
	WORD wXLow16;					//X�����16λ
	WORD wXHigh16;					//X�����16λ
	WORD wYLow16;					//Y�����16λ
	WORD wYHigh16;					//Y�����16λ
	WORD wZLow16;					//z�����16λ
	WORD wZHigh16;					//z�����16λ
	WORD wW1Low16;					//W1�����16λ
	WORD wW1High16;					//W1�����16λ
	WORD wALow16;					//A�����16λ
	WORD wAHigh16;					//A�����16λ
	WORD wBLow16;					//B�����16λ
	WORD wBHigh16;					//B�����16λ
	WORD wW2Low16;					//W2�����16λ
	WORD wW2High16;					//W2�����16λ
	WORD wGate1SyncTime;			//բ��1ͬ��ʱ��
	WORD wGate1MPTime;				//բ��1����󲨷�ֵʱ��
	WORD wGate1MPAmp	;			//բ��1����󲨷�ֵ����
	WORD wGate2MPTime;				//բ��2����󲨷�ֵʱ��
	WORD wGate2MPAmp	;			//բ��2����󲨷�ֵ����
	WORD wGate2FPFirstTime;			//բ��2�ڵ�һ���������ֵʱ��
	WORD wGate2FPTime;				//բ��2�ڵ�һ�����岨��ֵʱ��
	WORD wGate2FPAmp;				//բ��2�ڵ�һ�����岨��ֵ����
	WORD wGate3MPTime;				//բ��3����󲨷�ֵʱ��
	WORD wGate3MPAmp	;			//բ��3����󲨷�ֵ����
	WORD wGate3FPFirstTime;			//բ��3�ڵ�һ���������ֵʱ��
	WORD wGate3FPTime;				//բ��3�ڵ�һ�����岨��ֵʱ��
	WORD wGate3FPAmp;				//բ��3�ڵ�һ�����岨��ֵ����
	WORD wGate4MPTime;				//բ��4����󲨷�ֵʱ��
	WORD wGate4MPAmp	;			//բ��4����󲨷�ֵ����
	WORD wGate4FPFirstTime;			//բ��4�ڵ�һ���������ֵʱ��
	WORD wGate4FPTime;				//բ��4�ڵ�һ�����岨��ֵʱ��
	WORD wGate4FPAmp;				//բ��4�ڵ�һ�����岨��ֵ����
} CH_FLAWDATA_CSCAN_S;
// Song Chenguang add start on 2013-12-18
// Aɨ���ݼ�ǿ��
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
// ��������
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
	int m_nCount;	// ��ͬ���Ӵ����� ������ƽ��ֵʱʹ��
	float **m_pfFeatures;	// ����ֵ����ָ��
	//�����ź�,4���ţ�ÿ����7����������4*7=28��
	//	1�����������棨��һ���ز������
	//	2�����ڵ�һ����ֵǰ����ֵʱ��
	//	3����һ����ֵ�����ڣ�ʱ��
	//	4����������ֵǰ����ֵʱ��
	//	5������ֵ�����ڣ�ʱ�� MPMT
	//	6�����ڵ�һ����ֵ���� FPMA
	//	7����������ֵ���� MPMA
};


//Cɨ�������
typedef struct{
	int nColor;				// ��ɫѡ��
	int nDrawMode;			// ����ģʽ
	int nValueMode;			// ȡֵģʽ
	int nGateIndex;			// բ��ѡ��
	int nFeatureAmp;		// ��ֵ����ѡ��
	int nFeatureTime;		// ʱ������ѡ��
	int nGate1Threshold;	// բ��1��ֵ
	int nGate2Threshold;	// բ��2��ֵ
	int nGate3Threshold;	// բ��3��ֵ
	int nGate4Threshold;	// բ��4��ֵ
	//Machao add start on 2013-06-27
	int nSizeThreshold;		// ���ڣ����ڣ���ֵѡ��
	int nTubeBarYMode;		// �ܰ���Y��ģʽ
	int nImageMode;			// ����ģʽ
	int nWorkpieceThickness;	// �������
	int nMinAcreage;		// ��С���
	int nMinDisplayRange;	// ��ʾ��Χ����
	int nMaxDisplayRange;	// ��ʾ��Χ����
	int nProbePosition;		// ��ʾ̽ͷλ��
	BOOL nCBroomMap;			// Cɨͼͬ��
	BOOL nChangeOrigin;		// �������ԭ��
	//Machao add end on 2013-06-27
}CSCAN_PARAM_t;

// Cɨ�����ļ�ͷ
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
		m_nProbeNum = 1;		// ̽ͷ����
		m_nScanType = 0;		// ɨ�鷽ʽ�����л����
		m_nFillType = 0;		// ��䷽ʽ����������
		m_fProbeDistance = 0;	// ̽ͷ���
		m_fChongDie  = 0;		// �ص�����
		m_nWorkMode = 0;		//����ģʽ
	}

	~	CCScanFileHeader()
	{
	}
public:
	float m_fRangeX;	// ɨ���X��Χ
	float m_fRangeY;	// ɨ���Y��Χ
	CGateInfo m_GateInfo[CHANNEL_NUM][4];	// բ������
	BOOL m_bMPFTimeDisplay;		// ��߲�����ֵʱ��
	BOOL m_bMPMTimeDisplay;		// ��߲�����ʱ
	BOOL m_bMPMAltitudeDisplay;	// ��߲��߶�
	BOOL m_bFPFTimeDisplay;		// һ�β�����ֵʱ��
	BOOL m_bFPMTimeDisplay;		// һ�β���ʱ
	BOOL m_bFPMAltitudeDisplay;	// һ�β��߶�
	float m_fThick;				// �������
	float m_fGridX;				// X������
	float m_fGridY;				// Y������
	WORD m_wWaveSpeed;			// ����
	WORD m_wChannelNum;			// ͨ������
	float m_dXStart;
	float m_dYStart;
	float m_dZStart;
	float m_dXEnd;
	float m_dYEnd;
	float m_dZEnd;
	int m_nAxisSelect;
	double m_dPipeDiameter;
	int m_nProbeNum;			// ̽ͷ����
	int m_nScanType;			// ɨ�鷽ʽ�����л����
	int m_nFillType;			// ��䷽ʽ����������
	int m_nTwoProbeIndex;		// ����̽ͷ��Ϸ�ʽ
	float m_fProbeDistance;		// ̽ͷ���
	float m_fChongDie;			// �ص�����
	int m_nWorkMode;			//����ģʽ
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
	int nPointNum;		// ���ߵ�����
	CURVEPOINT_DATA_t stPointData[DAC_CURVE_NUM][DAC_CURVE_POINT_NUM];	//���ߵ�����
}DAC_CURVE_t;

typedef struct tagTCG_CURVE
{
	BOOL bSwitchFlag;
	float fFirstGain;
	int nPointNum;		// ���ߵ�����
	CURVEPOINT_DATA_t stPointData[TCG_CURVE_POINT_NUM];	//���ߵ�����
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
	LOGICAL_POINT_t m_stFrom;		// ���ϵ�
	LOGICAL_POINT_t m_stTo;			// ���µ�
	float m_fCenterDB;				// ���ĵ�DB
	float m_fEdgeDB;				// ��Ե��DB
	int m_nInstallEdgeDB;			// ���ñ�Ե��	// machao add on 2013-09-18
};
// Song Chenguang add end on 2013-06-20
// ��ϸ����б�洢�ڵ�
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
	int nColorType; //ÿ���˵������ɫ����,�ݶ�1,2,3 //add by wangqianfeng 2012-11-30
	BOOL bDisplay;
};
#pragma pack(pop)

typedef struct tagPACKET_HEAD
{
	DWORD dwCommand;		//����
	WORD wChannelIndex;		//ͨ����
	WORD wGateIndex;		//բ�ź�
	DWORD dwSize;				//�������С
} PACKET_HEAD_t;//���������Զ����ͷ

enum WORKMODE{SingleChannelA = 1, SingleChannelB, DoubleChannels,
			  OneSendOneReceive, DanJingGuanLian, ShuangJingGuanLian};
/*
class SEND_PARAM_DATA
{
public:
	WORD nPress;	// ��ѹ
	WORD nMaiKuan; // ����
	DWORD nRefrainFrequency; // �ظ�Ƶ��
	float nAmend; // ����
	float fQianYan;
public:
	SEND_PARAM_DATA()
	{
		nPress = MAX_PRESSURE;	// ��ѹ
		nMaiKuan = 100; // ����
		nRefrainFrequency = 50; // �ظ�Ƶ��
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
	WORD nProbeFreq;	// ̽ͷƵ��
	WORD nJianBoMode;	// �첨ģʽ
	float fPlus;		// ��׼����
	float fRefPlus;		// �ο�����
	float fDelay;		// ��ʱ
	WORD nLvBo;		// �˲�Ƶ��
	WORD wImpedance;		//��ͨ���迹
public:
	RECEIVE_PARAM_DATA()
	{
		nProbeFreq = 5;	// ̽ͷƵ��
		nJianBoMode = 2;	// �첨ģʽ
		fPlus = 0.0f;			// ��׼����
		fRefPlus = 0.0f;		// �ο�����
		fDelay = 0.0f;		// ��ʱ
		nLvBo = 255;		// �˲�Ƶ��
		wImpedance = 0;		//��ͨ���迹
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
	alarm_modes alarmMode;	// ����ģʽ
	BOOL bOpen;				// ������־
	float fFrontGate;		// ǰ��
	float fBackGate;		// ����
	float fPeakGate;		// ��ֵ
public:
	GATE_INFO()
	{
		alarmMode = SuperHigh;
		bOpen = FALSE;

		fFrontGate = 0.0f;		// ǰ��
		fBackGate = 10.0f;		// ����
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
	int nColor;	//��ɫ
	int nLCD;	// ����
	int nVol;	//����
	int nStoreType;	//�洢����

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
	WORD wCurChannel;	// ��ǰͨ��
	WORKMODE wWorkMode;	// ����ģʽ
	RECEIVE_PARAM_DATA stReceiveParam[2];
	DISPLAY_SET_DLG_DATA stDisplayParam[2];
	SEND_PARAM_DATA stSendParam[2];
	GATE_INFO stGatParam[2][2];
	OTHER_PARAM_t stOtherParam;
} WIRELESS_ALL_PARAM_t;//���������Զ���ȫ������

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
} REAL_SEND_DATA_t;//��λ��ʵʱ��������
*/
// С�����˰汾��Ϣ
typedef struct tagCSCAN_VERSION_DATA{
	float wHardWareVer;		// Ӳ���汾
	float wSoftWareVer;		// ����汾
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
	WORD wSamplingInterval;//�������  yachang.huang add on 2014-06-18
	float fSramGain[TCG_SRAM_NUM];
}TCG_COMMU_DATA_t;
// Song Chenguang add start on 2013-07-12
// copy from Machao
// ��ɫ��ɫֵ
struct PLATTE
{
	COLORREF color;		// ��ɫ
	float fMaxPercent;	// �ٷֱ�����
	float fMinPercent;	// �ٷֱ�����
};

// Song Chenguang add end on 2013-07-12
#endif