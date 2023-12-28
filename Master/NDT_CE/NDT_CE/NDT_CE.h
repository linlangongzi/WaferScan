
#if !defined(AFX_NDT_CE_H__C4456DAA_1483_4EAE_B426_16BCB76BA261__INCLUDED_)
#define AFX_NDT_CE_H__C4456DAA_1483_4EAE_B426_16BCB76BA261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "Winsock2.h" // wireless 2011-06-02
#pragma comment(lib,"ws2.lib") 

#include <winsock.h>

#define MAIN2AECTHREADCOMMAND_INIT			0
#define MAIN2AECTHREADCOMMAND_START			1
#define MAIN2AECTHREADCOMMAND_STOP			2
#define MAIN2AECTHREADCOMMAND_THREAD_STOP	3
#define MAIN2AECTHREADCOMMAND_IDLE			4
#define MAIN2AECTHREADCOMMAND_SETONE		5
#define MAIN2AECTHREADCOMMAND_NONE			6

#define WM_SOCKET  WM_USER + 1	//wireless wangbingfu add at 2011-5-26
#define MAX_SOCKETS 20			//wireless wangbingfu add at 2011-05-30
#define COMMUNICATION_MONITOR_PORT 3333	//wireless wangbingfu add at 2011-05-30
#define MAX_BACKLOG 5			//wireless wangbingfu add at 2011-05-30
#define NETHEAD_SIZE sizeof(PACKET_HEAD_t)
#define MAX_SIZE_BUF ( NETHEAD_SIZE + DATA_SIZE * 2 )	//wireless wangbingfu add at 2011-05-30
#define CHAN_NUM	20

struct decodeUINT{
	UINT Code;
	LRESULT (*Fxn)(HWND, UINT, WPARAM, LPARAM);
};

struct decodeCMD{
	UINT Code;
	LRESULT (*Fxn)(HWND, WORD, HWND, WORD);
};
struct allParamConfigWithTime
{
	WORD year;//��
	WORD month;//��
	WORD day;//��
	WORD hour;//ʱ
	WORD minute;//��
	WORD second;//��
};


typedef struct
{
	HWND hWnd;
	HDC hDC;
	HDC hMemDC;
}DRAWTHREAD_PARAM;

typedef struct
{
	HWND hWnd;
	float fValue;
}THREAD_PARAM;

typedef struct
{
	HDC hDC;
	HDC hMemDC;
	RECT rect;
}BitBltTHREAD_PARAM;

typedef struct
{
	WORD nMain2AECThreadCommand;
/*
	int index;
	WDC_DEVICE_HANDLE hDev;
	WORD nMain2AECThreadCommand; 
//	WORD *pMain2AECThreadCommand_LOCK;
	WORD nAEC2MainThreadFlag;
	BOOL bRunFlag;	// ����: 0, ����: 1	
	BOOL bStarted;
	BOOL b_ProcessingStop;
//	WORD *pAEC2MainThreadFlag_LOCK;
	WORD *pAECCardParameters;
	WORD nActiveChannel;
	WORD nSetOneChannel;
	WORD nSelChannel;
	BOOL bFlawDataRdy;
	// Song Chenguang add start on 2009-02-05
	// Doc��allCH_CONFIG�����ַ
	CH_CONFIG* pAllConfig;
	CH_CONFIG_GLOBAL* pConfigGlobal;
	// Song Chenguang add end on 2009-02-05
	BOOL m_bThreadStopEn;
*/
}AECTHREAD_PARAM;


typedef struct
{
	WORD data[DATA_SIZE];
	BOOL bDrawed;
}PIC_DATA;


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

// songchenguang add start 2010-05-12
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
// songchenguang add end 2010-05-12
//wangbingfu add start at 2011-2-22
class RAILROAD_PARAM_DATA{
	public:
		WORD wStandard;		//����׼
		WORD wPosition;		//��ⲿλ
		float fBuChangGain[TL_STANDARD_NUM][TL_POSITION_NUM];	//��������
		float fRange[TL_STANDARD_NUM][TL_POSITION_NUM];	//����׼����λ����
		float fPlus[TL_STANDARD_NUM][TL_POSITION_NUM];	//����׼����λ����
		BOOL  bFlag[TL_STANDARD_NUM][TL_POSITION_NUM];	//����׼����λ�趨��־
	public:
		RAILROAD_PARAM_DATA()
		{
			wStandard = 0;		//����׼
			wPosition = 0;		//��ⲿλ
			for(int i = 0; i < TL_STANDARD_NUM; i++)
			{
				for(int j = 0; j < TL_POSITION_NUM; j++)
				{
					fBuChangGain[i][j] = 0.0f;					
				}
			}
			for(int i = 0; i < TL_STANDARD_NUM; i++)
			{
				for(int j = 0; j < TL_POSITION_NUM; j++)
				{
					fRange[i][j] = 50.0f;					
				}
			}
			for(int i = 0; i < TL_STANDARD_NUM; i++)
			{
				for(int j = 0; j < TL_POSITION_NUM; j++)
				{
					fPlus[i][j] = 30.0f;					
				}
			}
			for(int i = 0; i < TL_STANDARD_NUM; i++)
			{
				for(int j = 0; j < TL_POSITION_NUM; j++)
				{
					bFlag[i][j] = FALSE;					
				}
			}
		}

		inline RAILROAD_PARAM_DATA& operator=(const RAILROAD_PARAM_DATA& rhs)
		{
			if(this == &rhs)
				return * this;
			wStandard = rhs.wStandard;
			wPosition = rhs.wPosition;
			memcpy(fBuChangGain, rhs.fBuChangGain, sizeof(fBuChangGain));
			memcpy(fRange, rhs.fRange, sizeof(fRange));
			memcpy(fPlus, rhs.fPlus, sizeof(fPlus));
			memcpy(bFlag, rhs.bFlag, sizeof(bFlag));
			
			return *this;
		}	
};
//wangbingfu add end at 2011-2-22

class CRAFT_PARAM
{
	public:
		SEND_PARAM_DATA SendParam;
		RECEIVE_PARAM_DATA ReceiveParam;
		DISPLAY_SET_DLG_DATA DisplayParam;
		
		GATE_INFO GateInfos[3];
		// TCG�ɼ�����
		DWORD SelectedDacPointDataNum;
		float fTcgFirstGain;
		float fOldTcgRang;
		short wOldTcgSerial[MAX_CURVE_POINT_NUM];
		// TCG����
		CURVEPOINT_DATA SelectedDacPointData[MAX_CURVE_POINT_NUM];
		//DAC����
		DGS_CURVE_S dgsCurve;
		//AVG����
		AVG_CURVE_S avgCurve;
		//��·
		RAILROAD_PARAM_DATA RailRoadParam;
	public:
		CRAFT_PARAM()
		{
			SelectedDacPointDataNum = 0;
			fTcgFirstGain = 0;
			fOldTcgRang = 0;
			memset(wOldTcgSerial, 0, sizeof(WORD)*MAX_CURVE_POINT_NUM);
			memset(SelectedDacPointData, 0, sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
		}

		inline CRAFT_PARAM& operator=(const CRAFT_PARAM& rhs)
		{
			if(this == &rhs)
				return * this;
			SendParam = rhs.SendParam;
			ReceiveParam = rhs.ReceiveParam;
			DisplayParam = rhs.DisplayParam;
			RailRoadParam = rhs.RailRoadParam;
			for(int i = 0; i < GATE_NUM; i++)
			{
				GateInfos[i] = rhs.GateInfos[i];
			}
			SelectedDacPointDataNum = rhs.SelectedDacPointDataNum;
			fTcgFirstGain = rhs.fTcgFirstGain;
			fOldTcgRang = rhs.fOldTcgRang;
			memcpy(wOldTcgSerial, rhs.wOldTcgSerial, sizeof(WORD)*MAX_CURVE_POINT_NUM);
			memcpy(SelectedDacPointData, rhs.SelectedDacPointData, sizeof(WORD)*MAX_CURVE_POINT_NUM);
			dgsCurve = rhs.dgsCurve;
			avgCurve = rhs.avgCurve;
			
			return *this;
		}
		
};

enum WORKMODE{SingleChannelA = 1, SingleChannelB, DoubleChannels,
				OneSendOneReceive, ShuangJingGuanLian,DanJingGuanLian};

class SYS_INFO
{
public:
	WORKMODE wWorkMode;	// ����ģʽ
	WORD wRelay;		// �̵���
	WORD wCurrentChNo;
	WORD wDeviceType;	//�豸�ͺ�
public:
	SYS_INFO()
	{
		wWorkMode = SingleChannelA;
		wRelay = 0;
		wCurrentChNo = 0;
		wDeviceType = 0;
	}

	inline SYS_INFO& operator=(const SYS_INFO& rhs)
	{
		if(this == &rhs)
			return * this;

		wWorkMode = rhs.wWorkMode;
		wRelay = rhs.wRelay;
		wCurrentChNo = rhs.wCurrentChNo;
		wDeviceType = rhs.wDeviceType;

		return *this;
	}
};

typedef struct _DB_ITEM
{
	WORD wDB_A;
	WORD wDB_B;
	WORD wDB_C;
	WORD wDB_D;
} DB_ITEM;

struct PEAK_INFO{
	double wDefectTime;
	double wDefectAmp;

	PEAK_INFO()
	{
		wDefectTime = 0.0;
		wDefectAmp = 0.0;
	}
};

struct SYS_COLOR_INFO{
	COLORREF clrScaleBack;	// ����߱���ɫ
	COLORREF clrWaveBack;	// �ǻز�ģʽ�²������򱳾�ɫ
	COLORREF clrCh1Wave;	// ͨ��1������ɫ
	COLORREF clrCh1WaveLike;	// ͨ��1������ɫ���ɫ
	COLORREF clrCh2Wave;	// ͨ��2������ɫ	
	COLORREF clrCh2WaveLike;	// ͨ��2������ɫ���ɫ

	SYS_COLOR_INFO()
	{
		clrScaleBack = RGB(255,255,255);
		clrWaveBack = RGB(0,0,0);

		clrCh1Wave = RGB(255,255,0);
		clrCh1WaveLike = RGB(255,255,31);
		clrCh2Wave = RGB(0,255,0);
		clrCh2WaveLike = RGB(0,255,31);
	}
};

//wireless 2011-05--31

typedef struct tagPACKET_HEAD
{
	DWORD dwCommand;		//����
	WORD wChannelIndex;		//ͨ����
	WORD wGateIndex;		//բ�ź�
	DWORD dwSize;				//�������С
} PACKET_HEAD_t;//���������Զ����ͷ
/*
typedef struct tagPACKET_HEAD
{
	WORD wCommand;		//����
	WORD dwSize;				//�������С
} PACKET_HEAD_t;//���������Զ����ͷ
*/
typedef struct tagOTHER_PARAM
{
	int nColor;	//��ɫ
	int nLCD;	// ����
	int nVol;	//����
	int nStoreType;	//�洢����
} OTHER_PARAM_t;

typedef struct tagWIRELESS_ALL_PARAM
{
	WORD wCurChannel;	// ��ǰͨ��
	WORKMODE wWorkMode;	// ����ģʽ
	SEND_PARAM_DATA stSendParam[2];
	RECEIVE_PARAM_DATA stReceiveParam[2];
	DISPLAY_SET_DLG_DATA stDisplayParam[2];	
	GATE_INFO stGatParam[2][2];
	OTHER_PARAM_t stOtherParam;
} WIRELESS_ALL_PARAM_t;//���������Զ����ͷ

typedef struct tagREAL_SEND_DATA
{
	WORD wPicData[CHANNEL_NUM][DATA_SIZE];
	PEAK_INFO stPeakInfo[CHANNEL_NUM][GATE_NUM - 1];
} REAL_SEND_DATA_t;//��λ��ʵʱ��������

typedef struct tagCH_FEATURE_DATA{
	WORD wXLow16;					//X�����16λ
	WORD wXHigh16;					//X�����16λ
	WORD wYLow16;					//Y�����16λ
	WORD wYHigh16;					//Y�����16λ
	WORD wZLow16;					//z�����16λ
	WORD wZHigh16;					//z�����16λ
	WORD wW1Low16;					//W1�����16λ
	WORD wW1High16;				//W1�����16λ
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
} CH_FEATURE_DATA_t;

typedef struct tagCH_FEATURE_BUFFER_DATA{
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
} CH_FEATURE_BUFFER_DATA_t;

typedef struct tagCH_CONFIG{
	CSCAN_ARS200FPGA_CH_CON_REG stCh_con;
	CSCAN_ARS200FPGA_CH_CONFIG_REG stCh_config;
	CSCAN_ARS200FPGA_CH_GATE_REG stCh_gate;
}CH_CONFIG_t;

typedef struct tagCSCAN_CH_SEND_MONITOR_DATA{
	WORD wPicData[DATA_SIZE];
//	CH_FEATURE_DATA_t stFeatureData;
	CH_FEATURE_BUFFER_DATA_t stFeatureData; //�򻯰���������
}CSCAN_CH_SEND_MONITOR_DATA_t;

typedef struct tagCSCAN_SEND_DATA{
	CSCAN_ARS200FPGA_CH_CON_REG stCh_con;
	CSCAN_ARS200FPGA_CH_CONFIG_REG stCh_config;
	CSCAN_ARS200FPGA_CH_GATE_REG stCh_gate;
}CSCAN_SEND_DATA_t;

typedef struct tagCSCAN_VERSION_DATA{
	float fHardWareVer;
	float fSoftWareVer;
	char versionStr[20];
}CSCAN_VERSION_DATA_t;

typedef struct tagCURVEPOINT_DATA
{
	float fData;
	float fRange;
}CURVEPOINT_DATA_t;

typedef struct tagTCG_CURVE
{
	BOOL bSwitchFlag;
	float fFirstGain;
	int nPointNum;		// ���ߵ�����
	CURVEPOINT_DATA_t stPointData[TCG_CURVE_POINT_NUM];	//���ߵ�����
}TCG_CURVE_t;

typedef struct tagTCG_COMMU_DATA 
{
//	TCG_CURVE_t stTcgCurve;
	BOOL bOpenFlag;
	int nSramNum;
	WORD wSamplingInterval;//�������  yachang.huang add on 2014-06-18
	float fSramGain[1024];
}TCG_COMMU_DATA_t;

#include <sstream>
struct lua_State;	// ǰ������
typedef struct tagSOCKET_PROC_PARAM
{
	SOCKET sockClient;
	int nIndex;
	lua_State *L;
	std::string buffer;
}SOCKET_PROC_PARAM_t;

DWORD WINAPI DrawThreadProc(LPVOID lpParameter);
DWORD WINAPI AECThreadProc(LPVOID lpParameter);
DWORD WINAPI BitBltThreadProc(LPVOID lpParameter);
DWORD WINAPI SocketThreadProc(LPVOID lpParameter);//wireless 2011-05--31
DWORD WINAPI SocketThreadProc2(LPVOID lpParameter);
DWORD WINAPI SocketThreadProc3(LPVOID lpParameter);//�����·�socket�̵߳����ó��� yachang.huang 2013-11-13
DWORD WINAPI SocketThreadProc4(LPVOID lpParameter);//����Aɨ�洢���� yachang.huang 2013-12-19
DWORD WINAPI SocketParamStringThreadProc( LPVOID lpParameter );
DWORD WINAPI AECThreadProc1(LPVOID lpParameter);
void AnalyseRecvData(char *pData, SOCKET sockClient);	//wireless 2011-05--31

#define IDC_CMDBAR	1
#define IDC_MAIN_CMDBAR 2
#define ID_STATUS_BAR 3
// Message handlers
LRESULT DoCreateMain(HWND, UINT, WPARAM, LPARAM);
LRESULT DoPaintMain(HWND, UINT, WPARAM, LPARAM);
LRESULT DoCommandMain(HWND, UINT, WPARAM, LPARAM);
LRESULT DoDestoryMain(HWND, UINT, WPARAM, LPARAM);
LRESULT DoKeysMain(HWND, UINT, WPARAM, LPARAM);
LRESULT DoMouseMain(HWND, UINT, WPARAM, LPARAM);
LRESULT DoDrawItem(HWND, UINT, WPARAM, LPARAM);
LRESULT DoHotKey(HWND, UINT, WPARAM, LPARAM); //wangbingfu add at2010-11-24

LRESULT OnParamSet(HWND,WORD,HWND,WORD);
LRESULT OnParamSet2(HWND,WORD,HWND,WORD);

LRESULT OnStart(HWND,WORD,HWND,WORD);
LRESULT OnStop(HWND,WORD,HWND,WORD);
LRESULT OnAlarmOpen(HWND,WORD,HWND,WORD);
LRESULT OnAlarmClose(HWND,WORD,HWND,WORD);
LRESULT OnSendSPIDelay(HWND,WORD,HWND,WORD);
//LRESULT OnSendParam(HWND,WORD,HWND,WORD);
LRESULT OnReceiveParam(HWND,WORD,HWND,WORD);
LRESULT OnDisplaySet(HWND,WORD,HWND,WORD);

int AECThreadIdle(AECTHREAD_PARAM * pThreadParam, int& nCount, int& nFrames);
int AECThreadIdle1(AECTHREAD_PARAM * pThreadParam, int& nCount, int& nFrames);

BOOL ReadFlawResult(AECTHREAD_PARAM * pThreadParam);

void PutsReadDrawData(PIC_DATA& picData, 
					  CSCAN_ARS200FPGA_CH_DATA_REG * ch_data_reg,
					  WORD *ch_data,
					  const WORD& chNo, 
					  PEAK_INFO PeakSum[2]);

BOOL AECThreadInit(AECTHREAD_PARAM * pThreadParam);
BOOL AECThreadStart(AECTHREAD_PARAM * pThreadParam);
BOOL AECThreadStop(AECTHREAD_PARAM * pThreadParam);

////////////////////////////////////////////////////
// ת������
//int Gain2Data( float Gain );
//int Gain2DataPart( float Gain );
WORD RangeProcess(float VSound, float Range, WORD* pInterSpace);
WORD Range2Dot(float VSound, float Range);
int Range2DotNew(float VSound, float Range);

float Dot2Range(float VSound, int DotNo);
void dBProcess(const float& db, 
			   WORD& db_0_data,
			   WORD& db_1_data,
			   WORD& db_2_data,
			   WORD& db_3_data);
WORD DACdBProcess(const float& db);
////////////////////////////////////////////////////

void CALLBACK TimerProcessProc(
  HWND hwnd, 
  UINT uMsg, 
  UINT idEvent, 
  DWORD dwTime 
);

void CALLBACK TimerPressProc(
  HWND hwnd, 
  UINT uMsg, 
  UINT idEvent, 
  DWORD dwTime 
);

void CALLBACK TimerClockProc(
  HWND hwnd, 
  UINT uMsg, 
  UINT idEvent, 
  DWORD dwTime 
);

void ErroWindowMessage();

// ��ʼ�����˺ͱ༭�ؼ�
void InitSldAndEit(HWND hSld, 
				   HWND hEdit, 
				   WORD nMaxRange, 
				   WORD nMinRange, 
				   WORD nPageSize,
				   WORD nPos);

// ������Ӧ
VOID WINAPI TBNotifications( WPARAM wParam, // wParam of WM_HSCROLL message
							HWND hwndTrack, // handle of trackbar window
							UINT iSelMin, // minimum value of trackbar selection
							UINT iSelMax,
							HWND hEdit);

BOOL InitParam();
void PlusChangeTurn(const WORD& chNo);

HWND DoCreateStatusBar(HWND hwndParent, int nStatusID,
					   HINSTANCE hinst, int nParts);

HBITMAP GetPressBmp(WORD nPress);

HBITMAP GetModeBmp(const WORKMODE& nMode);

// �޸Ĳ˵���ʾ��־
void SetMenuStateFlag(BOOL bValue);
// �޸��ػ���־
void SetRedrawFlag(BOOL bValue);
// ����
void Line(const int& xStart, const int& yStart, const int& xEnd, const int& yEnd, const WORD& nColor);
// ����������
void Line3(const int& xStart, const int& yStart, const int& xEnd, const int& yEnd, const WORD& nColor);
// ������������
void EraseLine3(const int& xStart, const int& yStart, const int& xEnd, const int& yEnd, const WORD& nBackColor, const WORD& nLineColor);



// ������
void DashLine(const int& xStart, const int& yStart, const int& xEnd, const int& yEnd, const WORD& nColor, const WORD& nColorBk);

// ���·����̲���
//void RangeExecute(const float& fRange);
// ���·��������
void PlusExecute(const float& fPlus);

void CopyBmp(HDC hDC, const WORD& wBmpID, const RECT& rect);

void GuanLianDBProcess(const float& fBaseDB, 
					   const float& fRefDB,
					   DB_ITEM& dbCh1,
					   DB_ITEM& dbCh2);

void GetDisplayRect(const RECT& RctFull,
					BOOL bMenuShown,
					RECT& RctWave, 
					RECT& RctXScale, 
					RECT& RctYScale,
					RECT& RctRight);

BOOL InitParamDetail();
void alarmProcess(WORKMODE eWorkMode);

int MicroSecond2Dot(const float& fMicroSecond);
void InitializeSysColor(SYS_COLOR_INFO* ColorArr, const int& nSize);
void DrawRectDashLine(const RECT& RctWave, const WORD& nColorBk);

void gainChangeCurve(float fOldGain, BOOL bSave);
void rangeChangeCurve(float fOldRange, BOOL bSave);
void refGainChangeCurve(float fOldRefGain, BOOL bSave);

void curve100To110Display();
void curve110To100Display();

void delayChangeCurve(float fRangeStartChange, float fRange);//�������ӳٸı�
void wait400000();
BOOL CheckRecvParams(RECEIVE_PARAM_DATA recvParams);
void SetRecvParams();
void RequestRespond(SOCKET sockClient, char *pData,DWORD dwDataSize, BOOL bFlag);
void ChangeChannel(BOOL bSaveFlag, int nChnNo);
void SetOne(CSCAN_ARS200FPGA_SETONE stSetOne , WORD wChannelNo);
void SetGain(CSCAN_ARS200FPGA_SETONE stSetOne , WORD wChannelNo);
void delay(int nDelayclock);
void SetInit();
WORD Gain2Data(float fGain);
void ReadCodeAndFeature(CSCAN_CH_SEND_MONITOR_DATA_t *pstMonitorData, int nChNo);
void SetProbeBand(WORD wBand, WORD wChannelNo);
void SetImpedance(WORD wImpedance, WORD wChannelNo);
void SetJianBoMode(WORD wJianBoMode, WORD wChannelNo);
BOOL SetIpAddress(LPCTSTR strDevice, LPCTSTR strIp,LPCTSTR strMask,LPCTSTR strGateWay);
DWORD WINAPI ReadFeatureThreadProc(LPVOID lpParameter);
BOOL SendTo(SOCKET sockClient, char szSendData[], DWORD dwSize);
void SetVoltage(WORD wVoltage);//���õ�ѹ yachang.huang add on 2013-11-22
void SetEnerge(WORD wEnerge,WORD wChannelNo);//�л��ߵ����� yachang.huang add on 2013-11-22
void SetRevAndSendMode(WORD wRevAndSendMode,WORD wChannelNo);//�л��շ�ģʽ yachang.huang add on 2013-11-22
void SetSPI(WORD wSPIAddr,WORD wSPIData,WORD wChannelNo);//��ѡ���SPI��ַ�·�SPI���� yachang.huang add on 2013-11-22

BOOL ArmSelfTest();
void GetCurPath(WCHAR* strFilePath);
DWORD GetWorkingFolder(LPTSTR lpszName, DWORD nCapacity);
void GetParamFormIni();
#endif // !defined(AFX_NDT_CE_H__C4456DAA_1483_4EAE_B426_16BCB76BA261__INCLUDED_)
