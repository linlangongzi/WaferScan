/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ���ղ�������Դ�ļ�
 * Filename: recvParamWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "recvParamWin.h"
#include "interfaceCmdID.h"

#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"
#include "const.h"
#include "craftParamWin.h"

HWND hWndRecvParam;						// ���ղ������ھ��
HWND hBtnRecvParamWaveMode;				// �첨��ʽ��ť���
HWND hBtnRecvParamGain;					// ���水ť���
HWND hBtnRecvParamDelay;				// �ӳٰ�ť���
HWND hBtnRecvParamWaveBand;				// �˲�Ƶ����ť���
HWND hBtnRecvParamReferenceGain;		// �ο����水ť���
HWND hBtnRecvParamImpedance;			// �迹��ť���

HWND recvParamBtnList[6];				// ���ղ��������еİ�ť��

WNDPROC OldBtnRecvParamWaveModeProc = NULL;		// �첨��ʽ��ťĬ�ϴ������
WNDPROC OldBtnRecvParamGainProc = NULL;			// ���水ťĬ�ϴ������
WNDPROC OldBtnRecvParamDelayProc = NULL;		// �ӳٰ�ťĬ�ϴ������
WNDPROC OldBtnRecvParamWaveBandProc = NULL;		// �˲�Ƶ����ťĬ�ϴ������
WNDPROC OldBtnRecvParamReferenceGainProc = NULL;			// �ο����水ťĬ�ϴ������
WNDPROC OldBtnRecvParamImpedanceProc = NULL;				// �迹��ťĬ�ϴ������

HWND currentRecvParamBtn;							// ���ղ������е�ǰѡ�еİ�ť

// songchenguang add start 2011-01-10
extern void displayDelay(HWND hWnd, float fDelay, const WORD& nType);
// songchenguang add end 2011-01-10

static int buttonNum;							// ��ť�ؼ���
//static HBRUSH hStaticBrush = NULL;				// ��ˢ
extern HWND hWndParam;								// �������ھ��
extern HWND currentParamBtn;						// �������е�ǰѡ�еİ�ť
extern HINSTANCE hInst;								// ����ʵ�����

// songchenguang add start 2010-04-22
RECEIVE_PARAM_DATA ReceiveParam_ad[2];
float fPlusStep = 0.1;//PLUSSTEPS[0];
static float fDelayStep = 0.1;//AMENDSTEPS[0];
static WORD nLvBoStep = 1;//LVBOIMPEDANCE[0];

WORD nPlusStepIndex = 0;				// ��ǰ���沽�����
static WORD nDelayStepIndex = 0;			// ��ǰ��ʱ�������
static WORD nImpedanceStepIndex = 0;		// ��ǰ��ʱ�������


extern ARS200FPGA_CH_CON_REG *chCurrent_con_reg;
extern ARS200FPGA_CH_CON_REG *ch1_con_reg;
extern ARS200FPGA_CH_CON_REG *ch2_con_reg;
extern ARS200FPGA_SYS_REG* sys_reg;
extern WORD CurrentChannelNo;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern SEND_PARAM_DATA SendParam_ad[2];
extern ARS200FPGA_CH_DATA_REG *chCurrent_data_reg;
extern ARS200FPGA_CH_DATA_REG *ch1_data_reg;
extern ARS200FPGA_CH_DATA_REG *ch2_data_reg;
extern ARS200FPGA_CH_PLUS_REG* chCurrent_plus_reg;
extern ARS200FPGA_CH_PLUS_REG* ch1_plus_reg;
extern ARS200FPGA_CH_PLUS_REG* ch2_plus_reg;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern HWND hStatusBar;
extern float gCh1Plus;
extern float gCh2Plus;
extern wchar_t gszTextPlus[10];
extern wchar_t gszTextRefPlus[10];
extern float fRangeStart[2];
extern float fRangeEnd[2];
extern CRITICAL_SECTION csRange;
extern wchar_t gszTextStep[10];
extern HWND hWndMain;
extern SYS_INFO gSysInfo;
extern CRITICAL_SECTION csSysInfo;
extern BOOL gbGuanLian;
extern HWND gCurrentFocusBtn;
// songchenguang add end 2010-04-22
extern WORD gwDeviceType;	//�豸���� 0:̽����	1:�����
extern HWND hWndChParam;			//���������ھ��
extern HWND currentChParamBtn;		//���������е�ǰѡ�еİ�ť
extern BOOL gDacChangeFlag; //DAC���Ƿ���Ҫ�ı�
extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];//��ѡ�е�DAC������
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];		//DACͨ������
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���
extern BOOL gbHotDB;			// ��ݼ�����
extern BOOL gbHotRange;			// ��ݼ�����
extern wchar_t lpDefaultParamFile[MAX_PATH];

// songchenguang add start 2010-08-13
//int MIN_DELAY[2] = {-9*DisplaySet_ad[0].nWaveSpeed*(1E-3)/2.0, -9*DisplaySet_ad[1].nWaveSpeed*(1E-3)/2};
//int MAX_DELAY = 12000;
float MIN_DELAY[2] = {0.0f, 0.0f};
float MAX_DELAY = 999;
// songchenguang add end 2010-08-13
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG���߿��ر�־
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//��ѡ�е�AVG������
extern BOOL bgAvgChangeFlag;
extern SYS_COLOR_INFO gstSysColor[SYS_COLOR_NUM];
extern int gCurrentSysColorIndex;

extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];		//��ӵ��������ݶ���
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];											//��ѡ�е�DGS������
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];	//��ӵ��������ݶ���
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];													//��ѡ�е�AVG������
//short gwOldDacSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������DACʱ��ÿ�����λ��
//short gwOldAvgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������AVGʱ��ÿ�����λ��
extern float gfTlPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];				//����׼����λ����
extern float gfTlBuChangPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//����׼����λ��������
extern WORD gwStandard[CHANNEL_NUM];	//����׼
extern WORD gwPosition[CHANNEL_NUM][TL_STANDARD_NUM];	//��ⲿλ
// songchenguang add end 2011-06-22
extern WORD MAX_PLUS;
// songchenguang add end 2011-06-22

extern CSCAN_ARS200FPGA_CH_CONFIG_REG *cscan_ch1_config_reg;
extern CSCAN_ARS200FPGA_CH_CONFIG_REG *cscan_ch2_config_reg;
extern CSCAN_ARS200FPGA_CH_GATE_REG *cscan_ch1_gate_reg;
extern CSCAN_ARS200FPGA_CH_GATE_REG *cscan_ch2_gate_reg;


/**************************
*ע����ղ�������
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerRecvParamChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) recvParamChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szRecvParamChild;

	return RegisterClass(&wc);
}

void UpdateReceiveMenuInfo(const WORD& chNo)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	if(ReceiveParam_ad[chNo].nJianBoMode == 0)
	{
		swprintf(strText, _T("���벨"));
	}
	else if(ReceiveParam_ad[chNo].nJianBoMode == 1)
	{
		swprintf(strText, _T("���벨"));
	}
	else if(ReceiveParam_ad[chNo].nJianBoMode == 2)
	{
		swprintf(strText, _T("ȫ��"));
	}
	else if(ReceiveParam_ad[chNo].nJianBoMode == 3)
	{
		swprintf(strText, _T("��Ƶ"));
	}
	DrawLableText(hBtnRecvParamWaveMode, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	wsprintf(strText, _T("%.1f"), ReceiveParam_ad[chNo].fPlus);
	DrawLableText(hBtnRecvParamGain, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	wsprintf(strText, _T("%.1f"), ReceiveParam_ad[chNo].fDelay);
	DrawLableText(hBtnRecvParamDelay, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	if(ReceiveParam_ad[chNo].nProbeFreq == 0)
		wsprintf(strText, _T("1M"));
	else if(ReceiveParam_ad[chNo].nProbeFreq == 1)
		wsprintf(strText, _T("0.5M"));
	else if(ReceiveParam_ad[chNo].nProbeFreq == 2)
		wsprintf(strText, _T("2M"));
	else if(ReceiveParam_ad[chNo].nProbeFreq == 3)
		wsprintf(strText, _T("��Ƶ��"));
	else if(ReceiveParam_ad[chNo].nProbeFreq == 4)
		wsprintf(strText, _T("5M"));
	else if(ReceiveParam_ad[chNo].nProbeFreq == 5) //2.5M խ��	
		wsprintf(strText, _T("2.5M"));

	DrawLableText(hBtnRecvParamWaveBand, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	wsprintf(strText, _T("%.1f"), ReceiveParam_ad[chNo].fRefPlus);
	DrawLableText(hBtnRecvParamReferenceGain, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

//	wsprintf(strText, _T("%d"), ReceiveParam_ad[chNo].nLvBo);
//	DrawLableText(hBtnRecvParamImpedance, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}
/**************************
*���ղ������ڴ������
* @param    hInstance[in]:
* @param	message[in]:
* @param	wParam[in]:
* @param	lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK recvParamChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;
//	LPDRAWITEMSTRUCT pdis;
//	TCHAR szHello[MAX_LOADSTRING];
//	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	switch (message) 
	{
		case WM_CREATE:
//			if(hStaticBrush == NULL)	
//				hStaticBrush = CreateSolidBrush(BK_COLOR_WHITE);
			if(gSysInfo.wWorkMode == SingleChannelA|| gSysInfo.wWorkMode == SingleChannelB)
			{
				buttonNum = 6;
			}
			else
			{
				buttonNum = 5;
			}
			//����ť�ؼ�
			hBtnRecvParamWaveMode = CreateWindow(_T("BUTTON"), _T("waveMode"), WS_VISIBLE | WS_CHILD|BS_OWNERDRAW, 0,
					0, MENU_WIDTH,BUTTON_6_EACH_HEIGHT, hWnd, (HMENU)IDC_RECVPARAM_WAVEMODE, hInst, NULL);
			hBtnRecvParamGain = CreateWindow(_T("BUTTON"),_T("gain"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_RECVPARAM_GAIN,hInst,NULL);
			hBtnRecvParamReferenceGain = CreateWindow(_T("BUTTON"),_T("referenceGain"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_RECVPARAM_REFERENCEGAIN,hInst,NULL);
			hBtnRecvParamDelay = CreateWindow(_T("BUTTON"),_T("delay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_RECVPARAM_DELAY,hInst,NULL);
			hBtnRecvParamWaveBand = CreateWindow(_T("BUTTON"),_T("waveBand"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_RECVPARAM_WAVEBAND,hInst,NULL);
			hBtnRecvParamImpedance = CreateWindow(_T("BUTTON"),_T("impedance"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_6_EACH_HEIGHT, MENU_WIDTH, MENU_HEIGHT - 5*BUTTON_6_EACH_HEIGHT,
					hWnd, (HMENU)IDC_RECVPARAM_LITTLE_MODIFY, hInst, NULL);

			recvParamBtnList[0] = hBtnRecvParamWaveMode;
			recvParamBtnList[1] = hBtnRecvParamGain;
			recvParamBtnList[2] = hBtnRecvParamReferenceGain;
			recvParamBtnList[3] = hBtnRecvParamDelay;
			recvParamBtnList[4] = hBtnRecvParamWaveBand;
			recvParamBtnList[5] = hBtnRecvParamImpedance;

			//��ť�ؼ����Զ��崦�����
			OldBtnRecvParamWaveModeProc = (WNDPROC) GetWindowLong(hBtnRecvParamWaveMode,GWL_WNDPROC);
			SetWindowLong(hBtnRecvParamWaveMode,GWL_WNDPROC,(LONG)btnRecvParamWaveModeProc);
			OldBtnRecvParamGainProc = (WNDPROC) GetWindowLong(hBtnRecvParamGain,GWL_WNDPROC);
		    SetWindowLong(hBtnRecvParamGain,GWL_WNDPROC,(LONG)btnRecvParamGainProc);
			OldBtnRecvParamDelayProc = (WNDPROC) GetWindowLong(hBtnRecvParamDelay,GWL_WNDPROC);
			SetWindowLong(hBtnRecvParamDelay,GWL_WNDPROC,(LONG)btnRecvParamDelayProc);
			OldBtnRecvParamWaveBandProc = (WNDPROC) GetWindowLong(hBtnRecvParamWaveBand,GWL_WNDPROC);
			SetWindowLong(hBtnRecvParamWaveBand,GWL_WNDPROC,(LONG)btnRecvParamWaveBandProc);
			OldBtnRecvParamReferenceGainProc = (WNDPROC) GetWindowLong(hBtnRecvParamReferenceGain,GWL_WNDPROC);
		    SetWindowLong(hBtnRecvParamReferenceGain,GWL_WNDPROC,(LONG)btnRecvParamReferenceGainProc);
			OldBtnRecvParamImpedanceProc = (WNDPROC) GetWindowLong(hBtnRecvParamImpedance, GWL_WNDPROC);
		    SetWindowLong(hBtnRecvParamImpedance, GWL_WNDPROC,(LONG)btnRecvParamImpedanceProc);

			currentRecvParamBtn = recvParamBtnList[0];//Ĭ�ϵ�ǰѡ���˲���ʽ
			SetFocus(currentRecvParamBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentRecvParamBtn;
				
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndRecvParam, &ps);
			
			EndPaint(hWndRecvParam, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				if(gwDeviceType == 0 || gwDeviceType == 2) //̽�˻���·
				{
					ShowWindow(hWndParam,SW_SHOW);
					SetFocus(currentParamBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					gCurrentFocusBtn = currentParamBtn;

					gCurrentWnd = hWndParam;
				}
				else if(gwDeviceType == 1)//���
				{
					ShowWindow(hWndChParam,SW_SHOW);
					SetFocus(currentChParamBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					gCurrentFocusBtn = currentChParamBtn;
					
					gCurrentWnd = hWndChParam;
				}

				DestroyWindow(hWndRecvParam);
				hWndRecvParam = 0;
			}
			break;
		case WM_DRAWITEM://�ػ���ťwch
			wchar_t strText[20];
			memset(strText, 0, sizeof(strText));
			pdis=(LPDRAWITEMSTRUCT)lParam;
			recvParam_DrawButtonBitmap(hInst,pdis);
			if(pdis->hwndItem == hBtnRecvParamWaveMode)	
			{
				if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode == 0)
				{
					swprintf(strText, _T("���벨"));
				}
				else if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode == 1)
				{
					swprintf(strText, _T("���벨"));
				}
				else if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode == 2)
				{
					swprintf(strText, _T("ȫ��"));
				}
				else if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode == 3)
				{
					swprintf(strText, _T("��Ƶ"));
				}
				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			}
			else if(pdis->hwndItem == hBtnRecvParamGain)
			{
				wsprintf(strText, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);
				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			}
			else if(pdis->hwndItem == hBtnRecvParamDelay)
			{
				wsprintf(strText, _T("%.2f"), ReceiveParam_ad[CurrentChannelNo].fDelay);
				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			}
			else if(pdis->hwndItem == hBtnRecvParamWaveBand)
			{
				if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 0)
					wsprintf(strText, _T("1M"));
				else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 1)
					wsprintf(strText, _T("0.5M"));
				else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 2)
					wsprintf(strText, _T("2M"));
				else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 3)
					wsprintf(strText, _T("��Ƶ��"));
				else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 4)
					wsprintf(strText, _T("5M"));
				else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 5) //2.5M խ��	
					wsprintf(strText, _T("2.5M"));
				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			}
			else if(pdis->hwndItem == hBtnRecvParamReferenceGain)
			{
				wsprintf(strText, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);
				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			}
			if(pdis->hwndItem == hBtnRecvParamImpedance)
			{
				if(gSysInfo.wWorkMode == SingleChannelA|| gSysInfo.wWorkMode == SingleChannelB)
				{
					displayImpedance(pdis->hwndItem, CurrentChannelNo, ReceiveParam_ad[CurrentChannelNo].wImpedance);
				}
			}

			// ��ⰴť�������ʾ��Ӧ�Ĳ���
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentRecvParamBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnRecvParamGain || pdis->hwndItem == hBtnRecvParamReferenceGain)
						swprintf(gszTextStep, _T("%.1f"), fPlusStep);
					else if(pdis->hwndItem == hBtnRecvParamDelay)
						swprintf(gszTextStep, _T("%.2f"), fDelayStep);
					else
						swprintf(gszTextStep, _T(""));

					PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}			

			break;
		case WM_COMMAND:
			break;
		case WM_CLOSE:
		
			break;
		case WM_CHAR:			
			if( wParam == '\t')
			{
				// ���°�ť������
				if(CurrentChannelNo == 0)
				{
					UpdateReceiveMenuInfo(1);
				}
				else
				{
					UpdateReceiveMenuInfo(0);
				}
			}
			SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			break;
		case WM_DESTROY:
		//	if(hStaticBrush)
		//	{
		//		DeleteObject(hStaticBrush);
		//		hStaticBrush = NULL;
		//	}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*���ؽ��ղ������ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int recvParam_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	if(gSysInfo.wWorkMode == SingleChannelA|| gSysInfo.wWorkMode == SingleChannelB)
	{
		const BTN_BMP_INFO tbl_recvParamBtnBitmap[]=
		{
			{IDC_RECVPARAM_WAVEMODE	,IDB_BITMAP_JIANBOFANGSHI_NOSEL_R	 ,  IDB_BITMAP_JIANBOFANGSHI_SELECT_R		},
			{IDC_RECVPARAM_GAIN	,IDB_BITMAP_ZENGYI_NOSEL_R	 ,  IDB_BITMAP_ZENGYI_SELECT_R		},
			{IDC_RECVPARAM_DELAY	,IDB_BITMAP_YANCHI_NOSEL_R ,  IDB_BITMAP_YANCHI_SELECT_R	},
			{IDC_RECVPARAM_WAVEBAND	,IDB_BITMAP_LVBOPINDAI_NOSEL_R	 ,  IDB_BITMAP_LVBOPINDAI_SELECT_R		},
			{IDC_RECVPARAM_REFERENCEGAIN, IDB_BITMAP_CANKAOZENGYI_NOSEL_R	 ,  IDB_BITMAP_CANKAOZENGYI_SELECT_R		},
			{IDC_RECVPARAM_LITTLE_MODIFY, IDB_BITMAP_ZUKANG_NOSEL_R	 ,  IDB_BITMAP_ZUKANG_SELECT_R },
			{	-1			,	-1					 ,	-1							}    
		};
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_recvParamBtnBitmap);
	}
	else
	{
		const BTN_BMP_INFO tbl_recvParamBtnBitmap[]=
		{
			{IDC_RECVPARAM_WAVEMODE	,IDB_BITMAP_JIANBOFANGSHI_NOSEL_R	 ,  IDB_BITMAP_JIANBOFANGSHI_SELECT_R		},
			{IDC_RECVPARAM_GAIN	,IDB_BITMAP_ZENGYI_NOSEL_R	 ,  IDB_BITMAP_ZENGYI_SELECT_R		},
			{IDC_RECVPARAM_DELAY	,IDB_BITMAP_YANCHI_NOSEL_R ,  IDB_BITMAP_YANCHI_SELECT_R	},
			{IDC_RECVPARAM_WAVEBAND	,IDB_BITMAP_LVBOPINDAI_NOSEL_R	 ,  IDB_BITMAP_LVBOPINDAI_SELECT_R		},
			{IDC_RECVPARAM_REFERENCEGAIN, IDB_BITMAP_CANKAOZENGYI_NOSEL_R	 ,  IDB_BITMAP_CANKAOZENGYI_SELECT_R		},
			{	-1			,	-1					 ,	-1							}    
		};
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_recvParamBtnBitmap);
	}
	
	return ReturnVal;
}

void SetJianBoModeLableAndExecute(HWND hWnd)
{
	// ���ñ�ǩ
	wchar_t strText[20];

	memset(strText, 0, sizeof(strText));
	if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode == 0)
	{
		swprintf(strText, _T("���벨"));
	}
	else if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode == 1)
	{
		swprintf(strText, _T("���벨"));
	}
	else if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode == 2)
	{
		swprintf(strText, _T("ȫ��"));
	}
	else if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode == 3)
	{
		swprintf(strText, _T("��Ƶ"));
	}

//	SetWindowText(hLableWaveMode, strText);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	
	// ���·�ֵ
	if(!gbGuanLian)
	{
		if(CurrentChannelNo == 0)
		{
			cscan_ch1_config_reg->wDetectMode = ReceiveParam_ad[CurrentChannelNo].nJianBoMode;
		}
		else if(CurrentChannelNo == 1)
		{
			cscan_ch2_config_reg->wDetectMode = ReceiveParam_ad[CurrentChannelNo].nJianBoMode;
		}
	}
	else
	{
		cscan_ch1_config_reg->wDetectMode = ReceiveParam_ad[CurrentChannelNo].nJianBoMode;
		cscan_ch2_config_reg->wDetectMode = ReceiveParam_ad[CurrentChannelNo].nJianBoMode;

		ReceiveParam_ad[0].nJianBoMode = ReceiveParam_ad[CurrentChannelNo].nJianBoMode;
		ReceiveParam_ad[1].nJianBoMode = ReceiveParam_ad[CurrentChannelNo].nJianBoMode;
	}

	InitializeCriticalSection(&csSysInfo);
	EnterCriticalSection(&csSysInfo);

	if(!gbGuanLian)
	{
		switch(ReceiveParam_ad[CurrentChannelNo].nJianBoMode)
		{
		case 0:	// ���벨
			if(CurrentChannelNo == 0)
			{
				gSysInfo.wRelay |= 0x1C;
			}
			else if(CurrentChannelNo == 1)
			{
				gSysInfo.wRelay |= 0xE00;
			}
			else
			{
			}
			break;
		case 1: // ���벨
			if(CurrentChannelNo == 0)
			{
				gSysInfo.wRelay |= 0x14;
				gSysInfo.wRelay &= 0xFFF7;
			}
			else if(CurrentChannelNo == 1)
			{
				gSysInfo.wRelay |= 0xA00;
				gSysInfo.wRelay &= 0xFBFF;
			}
			else
			{
			}
			break;
		case 2: // ȫ��
			if(CurrentChannelNo == 0)
			{
				gSysInfo.wRelay |= 0x10;
				gSysInfo.wRelay &= 0xFFF3;
			}
			else if(CurrentChannelNo == 1)
			{
				gSysInfo.wRelay |= 0x800;
				gSysInfo.wRelay &= 0xF9FF;
			}
			else
			{
			}
			break;
		case 3: // ��Ƶ
			if(CurrentChannelNo == 0)
			{
				gSysInfo.wRelay &= 0xFFEF;
			}
			else if(CurrentChannelNo == 1)
			{
				gSysInfo.wRelay &= 0xF7FF;
			}
			else
			{
			}
			break;
		}
	}
	else
	{
		switch(ReceiveParam_ad[CurrentChannelNo].nJianBoMode)
		{
		case 0:	// ���벨
			gSysInfo.wRelay |= 0x1C;

			gSysInfo.wRelay |= 0xE00;
			break;
		case 1: // ���벨
			gSysInfo.wRelay |= 0x14;
			gSysInfo.wRelay &= 0xFFF7;
		
			gSysInfo.wRelay |= 0xA00;
			gSysInfo.wRelay &= 0xFBFF;
			
			break;
		case 2: // ȫ��
			gSysInfo.wRelay |= 0x10;
			gSysInfo.wRelay &= 0xFFF3;
		
			gSysInfo.wRelay |= 0x800;
			gSysInfo.wRelay &= 0xF9FF;
			
			break;
		case 3: // ��Ƶ
			gSysInfo.wRelay &= 0xFFEF;
		
			gSysInfo.wRelay &= 0xF7FF;
			
			break;
		}
	}
	//yachang.huang rum on 2013-11-19
	//sys_reg->wRelay = gSysInfo.wRelay;

	LeaveCriticalSection(&csSysInfo);
	DeleteCriticalSection(&csSysInfo);

	// �������õ�Ĭ���ļ�
	SaveParamFile(lpDefaultParamFile);
}

/**************************
*�첨��ʽ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnRecvParamWaveModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			break;
		case WM_PAINT:
			// songchenguang add start 2010-04-22
			wchar_t strText[20];

			memset(strText, 0, sizeof(strText));
			if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode == 0)
			{
				swprintf(strText, _T("���벨"));
			}
			else if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode == 1)
			{
				swprintf(strText, _T("���벨"));
			}
			else if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode == 2)
			{
				swprintf(strText, _T("ȫ��"));
			}
			else if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode == 3)
			{
				swprintf(strText, _T("��Ƶ"));
			}

			//SetWindowText(hLableWaveMode, strText);
			DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			// songchenguang add end 2010-04-22
			break;
		case WM_KEYDOWN:
		{
			int i = 0;
			if(wParam == VK_ESCAPE)
			{
				HWND hParentWnd = GetParent(Wnd);

				if(hParentWnd != NULL)
					SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
			}
		
			else if(wParam == VK_UP)
			{
				while(recvParamBtnList[i] != currentRecvParamBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentRecvParamBtn = recvParamBtnList[i];
				SetFocus(currentRecvParamBtn);
				gCurrentFocusBtn = currentRecvParamBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(recvParamBtnList[i] != currentRecvParamBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentRecvParamBtn = recvParamBtnList[i];
				SetFocus(currentRecvParamBtn);
				gCurrentFocusBtn = currentRecvParamBtn;
//				PostMessage(Wnd,WM_PAINT,(WPARAM)0,(LPARAM)0);
			}
			else if(wParam == VK_RETURN)
			{
			}
			else if(wParam == VK_LEFT)
			{
				// songchenguang add start 2010-04-22
				if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode > 0)
					ReceiveParam_ad[CurrentChannelNo].nJianBoMode -- ;
				else
					ReceiveParam_ad[CurrentChannelNo].nJianBoMode = 0;

				SetJianBoModeLableAndExecute(Wnd);
				PostMessage(Wnd,WM_PAINT,(WPARAM)0,(LPARAM)0);
				// songchenguang add end 2010-04-22
			}
			else if(wParam == VK_RIGHT)
			{
				// songchenguang add start 2010-04-22
				if(ReceiveParam_ad[CurrentChannelNo].nJianBoMode < 3)
					ReceiveParam_ad[CurrentChannelNo].nJianBoMode ++ ;
				else
					ReceiveParam_ad[CurrentChannelNo].nJianBoMode = 3;

				SetJianBoModeLableAndExecute(Wnd);
				// songchenguang add end 2010-04-22
			}
			else if(wParam == '\t')
			{
//				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				HWND hParentWnd = GetParent(Wnd);

				if(hParentWnd != NULL)
					SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '1')
			{
				PostMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '2')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '3')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '4')
			{
				PostMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == 'D')
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'E')
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'B')	// ����
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
		}
			break;
	//	case WM_CTLCOLORSTATIC:
	//		PostMessage(Wnd,WM_PAINT,(WPARAM)0,(LPARAM)0);
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);	
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnRecvParamWaveModeProc,Wnd,Msg,wParam,lParam);
}

void SetPlusLableAndExecute(HWND hWnd, const float& fBasePlus, const float& fRefPlus, const BOOL& bSave, const WORD& wChNo )
{
	// ����
	if(hWnd)
	{
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		wsprintf(strText, _T("%.1f"), fBasePlus);

		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}

	// ��ֵ
	if(!gbGuanLian)
	{
		//ARS200FPGA_CH_PLUS_REG* ch_plus_reg = NULL;
		CSCAN_ARS200FPGA_CH_CONFIG_REG* ch_plus_reg = NULL;
		if(wChNo == 0)
		{
			ch_plus_reg = cscan_ch1_config_reg;
		}
		else if(wChNo == 1)
		{
			ch_plus_reg = cscan_ch2_config_reg;
		}
		else
			return;

		WORD wTemp0 = 0;
		WORD wTemp1 = 0;
		WORD wTemp2 = 0;
		WORD wTemp3 = 0;

		float fTotalPlus = fBasePlus + fRefPlus;
		dBProcess(fTotalPlus, wTemp0, wTemp1, wTemp2, wTemp3);

		ch_plus_reg->wdBRef = wTemp0;
		PlusChangeTurn(wChNo);	

		ch_plus_reg->wdBLevel1 = wTemp1;
		PlusChangeTurn(wChNo);

		ch_plus_reg->wdBLevel2 = wTemp2;
		PlusChangeTurn(wChNo);

		ch_plus_reg->wdBLevel3 = wTemp3;
		PlusChangeTurn(wChNo);

		if(wChNo == 0)
			gCh1Plus = fTotalPlus;
		else if(wChNo == 1)
			gCh2Plus = fTotalPlus;
	}
	else
	{
		DB_ITEM dB1;
		memset(&dB1, 0, sizeof(DB_ITEM));
		DB_ITEM dB2;
		memset(&dB2, 0, sizeof(DB_ITEM));

		GuanLianDBProcess(fBasePlus,
						  fRefPlus,
						  dB1,
						  dB2);

		cscan_ch1_config_reg->wdBRef = dB1.wDB_A;
		PlusChangeTurn(0);	

		cscan_ch1_config_reg->wdBLevel1 = dB1.wDB_B;
		PlusChangeTurn(0);

		cscan_ch1_config_reg->wdBLevel2 = dB1.wDB_C;
		PlusChangeTurn(0);

		cscan_ch1_config_reg->wdBLevel3 = dB1.wDB_D;
		PlusChangeTurn(0);

		cscan_ch2_config_reg->wdBRef = dB2.wDB_A;
		PlusChangeTurn(1);	

		cscan_ch2_config_reg->wdBLevel1 = dB2.wDB_B;
		PlusChangeTurn(1);

		cscan_ch2_config_reg->wdBLevel2 = dB2.wDB_C;
		PlusChangeTurn(1);

		cscan_ch2_config_reg->wdBLevel3 = dB2.wDB_D;
		PlusChangeTurn(1);

		ReceiveParam_ad[0].fPlus = fBasePlus;
		ReceiveParam_ad[1].fPlus = fBasePlus;
	}

	memset(gszTextPlus, 0, sizeof(gszTextPlus));
	swprintf(gszTextPlus, _T("%.1f"), fBasePlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

	if(gwDeviceType == 2)//��·ר��ʱ������������ο�����һ��  wangbingfu add at 2011-05-27
	{
		gfTlPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]] = ReceiveParam_ad[CurrentChannelNo].fPlus;
	}

	// �������õ�Ĭ���ļ�
	if(bSave)
		SaveParamFile(lpDefaultParamFile);
}
/**************************
*���水ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnRecvParamGainProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_CREATE:
		fPlusStep = PLUSSTEPS[nPlusStepIndex];
		break;
	case WM_PAINT:
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		wsprintf(strText, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

		//SetWindowText(hLableGain, strText);
		DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
		break;
	case WM_KEYDOWN:
		{
			int i = 0;
			if(wParam == VK_ESCAPE)
			{
				HWND hParentWnd = GetParent(Wnd);

				if(hParentWnd != NULL)
					SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == VK_UP)
			{
				while(recvParamBtnList[i] != currentRecvParamBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentRecvParamBtn = recvParamBtnList[i];
				SetFocus(currentRecvParamBtn);
				gCurrentFocusBtn = currentRecvParamBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(recvParamBtnList[i] != currentRecvParamBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentRecvParamBtn = recvParamBtnList[i];
				SetFocus(currentRecvParamBtn);
				gCurrentFocusBtn = currentRecvParamBtn;
			}
			else if(wParam == VK_LEFT)
			{
				float fOldPlus = ReceiveParam_ad[CurrentChannelNo].fPlus;
				/*CURVEPOINT_DATA tmpSelectedDgsPointData[GATE_NUM][MAX_CURVE_POINT_NUM];
				memset(tmpSelectedDgsPointData, 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM * GATE_NUM);
				for(int j = 0; j < 3; j++)
				{
					for(DWORD k = 0; k < gSelectedDgsPointDataNum[CurrentChannelNo][j]; k++)
					{
						tmpSelectedDgsPointData[j][k] = gSelectedDgsPointData[CurrentChannelNo][j][k];
						if(tmpSelectedDgsPointData[j][k].data > (WORD)MAX_HEIGHT)
							tmpSelectedDgsPointData[j][k].data = (WORD)MAX_HEIGHT;
					}
				}*/
				// songchenguang add start 2010-04-22
				if((ReceiveParam_ad[CurrentChannelNo].fPlus - fPlusStep) > MIN_PLUS )
					ReceiveParam_ad[CurrentChannelNo].fPlus -= fPlusStep;
				else
					ReceiveParam_ad[CurrentChannelNo].fPlus = MIN_PLUS;

				SetPlusLableAndExecute(Wnd, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);
				// songchenguang add end 2010-04-22
				gainChangeCurve(fOldPlus, TRUE);
			}
			else if(wParam == VK_RIGHT)
			{
				float fOldPlus = ReceiveParam_ad[CurrentChannelNo].fPlus;
				
				// songchenguang add start 2010-04-22
				if((ReceiveParam_ad[CurrentChannelNo].fPlus + fPlusStep) < (MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fRefPlus))
					ReceiveParam_ad[CurrentChannelNo].fPlus += fPlusStep;
				else
					ReceiveParam_ad[CurrentChannelNo].fPlus = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fRefPlus;

				SetPlusLableAndExecute(Wnd, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);
				// songchenguang add end 2010-04-22
				gainChangeCurve(fOldPlus, TRUE);
			}
			else if(wParam == 'F')
			{
				if(nPlusStepIndex < 3)
					nPlusStepIndex++;
				else
					nPlusStepIndex = 0;

				fPlusStep = PLUSSTEPS[nPlusStepIndex];

				memset(gszTextStep, 0, sizeof(gszTextStep));
				swprintf(gszTextStep, _T("%.1f"), fPlusStep);
				SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			}
			else if(wParam == '\t')
			{
				HWND hParentWnd = GetParent(Wnd);

				if(hParentWnd != NULL)
					SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '1')
			{
				PostMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '2')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '3')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '4')
			{
				PostMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == 'D')
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'E')
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'B')	// ����
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
		}
		break;
//	case WM_CTLCOLORSTATIC:
//		PostMessage(Wnd,WM_PAINT,(WPARAM)0,(LPARAM)0);
//		SetBkMode((HDC)wParam,TRANSPARENT);
//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
//		return (LRESULT)hStaticBrush;
	default:
		break;
	}
	return CallWindowProc(OldBtnRecvParamGainProc,Wnd,Msg,wParam,lParam);
}

void SetDelayLableAndExecute(HWND hWnd, const WORD& nChNo)
{
	float fOldRangeStart = fRangeStart[nChNo]; //����ɵ���ʼ����
	// ����
	if(hWnd)
	{
		// songchenguang modify start 2011-01-10
		displayDelay(hWnd, ReceiveParam_ad[nChNo].fDelay, 6);
		// songchenguang modify end 2011-01-10
	}
	// ��ֵ
	float fTemp = ReceiveParam_ad[nChNo].fDelay;
	float fAngle = float(DisplaySet_ad[nChNo].fZheSheJiao/180.0f * PI);
	ARS200FPGA_CH_DATA_REG *ch_data_reg = NULL;

/*	if(nChNo == 0)
		ch_data_reg = ch1_data_reg;
	else if(nChNo == 1)
		ch_data_reg = ch2_data_reg;
*/
	if(!gbGuanLian)
//		chCurrent_data_reg->wMonitorDelayCounter = WORD(500 + Range2DotNew(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fTemp));
//		ch_data_reg->wMonitorDelayCounter = WORD(500 + MicroSecond2Dot(fTemp));
	{
		if(nChNo == 0)
		{
			cscan_ch1_gate_reg->wDelay = WORD(MicroSecond2Dot(fTemp));
		}
		else if(nChNo == 1)
		{
			cscan_ch2_gate_reg->wDelay = WORD(MicroSecond2Dot(fTemp));
		}
	}
	else
	{
		cscan_ch1_gate_reg->wDelay = WORD(500 + MicroSecond2Dot(fTemp));
		cscan_ch2_gate_reg->wDelay = WORD(500 + MicroSecond2Dot(fTemp));

		ReceiveParam_ad[0].fDelay = fTemp;
		ReceiveParam_ad[1].fDelay = fTemp;
	}

	InitializeCriticalSection(&csRange);
	EnterCriticalSection(&csRange);
	float fRange = fRangeEnd[nChNo] - fRangeStart[nChNo];
	
	if(!gbGuanLian)
	{
		if(!DisplaySet_ad[nChNo].bSFlag)
		{
			if(fAngle > 0)
			{
				fRangeStart[nChNo] = 
					float((fTemp / 2 * DisplaySet_ad[nChNo].nWaveSpeed*1E-3)*sin(fAngle) - SendParam_ad[nChNo].fQianYan); // ��ʱ
				fRangeEnd[nChNo] =
					float((fTemp / 2 * DisplaySet_ad[nChNo].nWaveSpeed*1E-3 + DisplaySet_ad[nChNo].fRange)*sin(fAngle) - SendParam_ad[nChNo].fQianYan);  // ��ʱ+����
			}
		}
		else
		{
			fRangeStart[nChNo] = float(fTemp/2*DisplaySet_ad[nChNo].nWaveSpeed*1E-3);
			fRangeEnd[nChNo] = fRangeStart[nChNo] + fRange;
		}
	}
	else
	{
		if(!DisplaySet_ad[nChNo].bSFlag)
		{
			if(fAngle > 0)
			{
				fRangeStart[0] = 
				float((fTemp / 2 * DisplaySet_ad[nChNo].nWaveSpeed*1E-3)*sin(fAngle) - SendParam_ad[nChNo].fQianYan); // ��ʱ
				fRangeEnd[0] = 
					float((fTemp / 2 * DisplaySet_ad[nChNo].nWaveSpeed*1E-3 + DisplaySet_ad[nChNo].fRange)*sin(fAngle) - SendParam_ad[nChNo].fQianYan);  // ��ʱ+����

				fRangeStart[1] = fRangeStart[0];
				fRangeEnd[1] = fRangeEnd[0];
			}
		}
		else
		{
			fRangeStart[0] = float(fTemp/2*DisplaySet_ad[nChNo].nWaveSpeed*1E-3);
			fRangeEnd[0] = fRangeStart[0] + fRange;

			fRangeStart[1] = fRangeStart[0];
			fRangeEnd[1] = fRangeEnd[0];
		}
	}

	LeaveCriticalSection(&csRange);
	DeleteCriticalSection(&csRange);
	
	// wangbingfu add start 2011-3-4
	float fRangeStartChange = fRangeStart[nChNo] - fOldRangeStart;
	delayChangeCurve(fRangeStartChange, fRange);
	// wangbingfu add end 2011-3-4

	// �������õ�Ĭ���ļ�
	SaveParamFile(lpDefaultParamFile);

	SetRedrawFlag(TRUE);
}

/**************************
*�ӳٰ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnRecvParamDelayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		//songchenguang add start 2010-06-01
	case WM_CREATE:
		fDelayStep = USSTEPS[nDelayStepIndex];
		break;
		//songchenguang add end 2010-06-01
	case WM_PAINT:
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		wsprintf(strText, _T("%.2f"), ReceiveParam_ad[CurrentChannelNo].fDelay);

		//SetWindowText(hLableDelay, strText);
		DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
		break;
	case WM_KEYDOWN:
		{
			int i = 0;
			if(wParam == VK_ESCAPE)
			{
				HWND hParentWnd = GetParent(Wnd);

				if(hParentWnd != NULL)
					SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
			}
		
			else if(wParam == VK_UP)
			{
				while(recvParamBtnList[i] != currentRecvParamBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentRecvParamBtn = recvParamBtnList[i];
				SetFocus(currentRecvParamBtn);
				gCurrentFocusBtn = currentRecvParamBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(recvParamBtnList[i] != currentRecvParamBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentRecvParamBtn = recvParamBtnList[i];
				SetFocus(currentRecvParamBtn);
				gCurrentFocusBtn = currentRecvParamBtn;
			}
			else if(wParam == VK_LEFT)
			{
				// songchenguang add start 2010-04-22
				if(ReceiveParam_ad[CurrentChannelNo].fDelay > MIN_DELAY[CurrentChannelNo] && 
					ReceiveParam_ad[CurrentChannelNo].fDelay > (MIN_DELAY[CurrentChannelNo] + fDelayStep))
					ReceiveParam_ad[CurrentChannelNo].fDelay -= fDelayStep;
				else
					ReceiveParam_ad[CurrentChannelNo].fDelay = MIN_DELAY[CurrentChannelNo];

				SetDelayLableAndExecute(Wnd, CurrentChannelNo);
				// songchenguang add end 2010-04-22
			}
			else if(wParam == VK_RIGHT)
			{
				// songchenguang add start 2010-04-22
				if(ReceiveParam_ad[CurrentChannelNo].fDelay < MAX_DELAY)
					ReceiveParam_ad[CurrentChannelNo].fDelay += fDelayStep;
				else
					ReceiveParam_ad[CurrentChannelNo].fDelay = MAX_DELAY;

				SetDelayLableAndExecute(Wnd, CurrentChannelNo);
				// songchenguang add end 2010-04-22
			}
			//songchenguang add start 2010-06-01
			else if(wParam == 'F')
			{
				if(nDelayStepIndex < 2)
					nDelayStepIndex++;
				else
					nDelayStepIndex = 0;

				fDelayStep = USSTEPS[nDelayStepIndex];

				memset(gszTextStep, 0, sizeof(gszTextStep));
				swprintf(gszTextStep, _T("%.2f"), fDelayStep);
				SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			}
			else if(wParam == '\t')
			{
				HWND hParentWnd = GetParent(Wnd);

				if(hParentWnd != NULL)
					SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '1')
			{
				PostMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '2')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '3')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '4')
			{
				PostMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == 'D')
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'E')
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'B')	// ����
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			//songchenguang add end 2010-06-01
		}
		break;
//	case WM_CTLCOLORSTATIC:
//		PostMessage(Wnd,WM_PAINT,(WPARAM)0,(LPARAM)0);
//		SetBkMode((HDC)wParam,TRANSPARENT);
//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
//		return (LRESULT)hStaticBrush;
	default:
		break;
	}
	return CallWindowProc(OldBtnRecvParamDelayProc,Wnd,Msg,wParam,lParam);
}

void SetLvBoLableAndExecute(HWND hWnd, const WORD& chNo)
{
	// ����
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	InitializeCriticalSection(&csSysInfo);
	EnterCriticalSection(&csSysInfo);

	WORD nMax500Value = 0;
	if(!gbGuanLian)
	{
		if(ReceiveParam_ad[chNo].nProbeFreq == 0)
		{
			wsprintf(strText, _T("1M"));
			if(chNo == 0)
			{
				gSysInfo.wRelay &= 0xFE5F;
			}
			else if(chNo == 1)
			{
				gSysInfo.wRelay &= 0x2FFF;
			}

			// �迹
			ReceiveParam_ad[chNo].nLvBo = 50;
		}
		else if(ReceiveParam_ad[chNo].nProbeFreq == 1)
		{
			wsprintf(strText, _T("0.5M"));
			if(chNo == 0)
			{
				gSysInfo.wRelay |= 0x80;
				gSysInfo.wRelay &= 0xFEDF;
			}
			else if(chNo == 1)
			{
				gSysInfo.wRelay |= 0x4000;
				gSysInfo.wRelay &= 0x6FFF;
			}

			// �迹
			ReceiveParam_ad[chNo].nLvBo = 0;
		}
		else if(ReceiveParam_ad[chNo].nProbeFreq == 2)
		{
			wsprintf(strText, _T("2M"));
			if(chNo == 0)
			{
				gSysInfo.wRelay |= 0x100;
				gSysInfo.wRelay &= 0xFF5F;
			}
			else if(chNo == 1)
			{
				gSysInfo.wRelay |= 0x8000;
				gSysInfo.wRelay &= 0xAFFF;
			}

			// �迹
			ReceiveParam_ad[chNo].nLvBo = 100;
		}
		else if(ReceiveParam_ad[chNo].nProbeFreq == 3)
		{
			wsprintf(strText, _T("��Ƶ��"));
			if(chNo == 0)
			{
				gSysInfo.wRelay |= 0x180;
				gSysInfo.wRelay &= 0xFFDF;
			}
			else if(chNo == 1)
			{
				gSysInfo.wRelay |= 0xC000;
				gSysInfo.wRelay &= 0xEFFF;
			}

			// �迹
			ReceiveParam_ad[chNo].nLvBo = 125;
		}
		else if(ReceiveParam_ad[chNo].nProbeFreq == 4)
		{
			wsprintf(strText, _T("5M"));
			if(chNo == 0)
			{
				gSysInfo.wRelay |= 0x120;
				gSysInfo.wRelay &= 0xFFBF;
			}
			else if(chNo == 1)
			{
				gSysInfo.wRelay |= 0x9000;
				gSysInfo.wRelay &= 0xDFFF;
			}

			// �迹
			ReceiveParam_ad[chNo].nLvBo = 200;
		}
		else if(ReceiveParam_ad[chNo].nProbeFreq == 5) //2.5M խ��	
		{
			wsprintf(strText, _T("2.5M"));
			if(chNo == 0)
			{
				gSysInfo.wRelay |= 0x160;	
			}
			else if(chNo == 1)
			{
				gSysInfo.wRelay |= 0xB000;
			}

			// �迹
			ReceiveParam_ad[chNo].nLvBo = 150;
		}	

		if(chNo == 0)
		{
			nMax500Value = (2<<8) + ReceiveParam_ad[chNo].nLvBo;
			send_max500(nMax500Value);
		}
		else if(chNo == 1)
		{
			nMax500Value = (3<<8) + ReceiveParam_ad[chNo].nLvBo;
			send_max500(nMax500Value);
		}
	}
	else
	{
		if(ReceiveParam_ad[chNo].nProbeFreq == 0)
		{
			wsprintf(strText, _T("1M"));
			
			gSysInfo.wRelay &= 0xFE5F;
		
			gSysInfo.wRelay &= 0x2FFF;
		
			ReceiveParam_ad[chNo].nLvBo = 50;
		}
		else if(ReceiveParam_ad[chNo].nProbeFreq == 1)
		{
			wsprintf(strText, _T("0.5M"));
			
			gSysInfo.wRelay |= 0x80;
			gSysInfo.wRelay &= 0xFEDF;
		
			gSysInfo.wRelay |= 0x4000;
			gSysInfo.wRelay &= 0x6FFF;
		
			ReceiveParam_ad[chNo].nLvBo = 0;
		}
		else if(ReceiveParam_ad[chNo].nProbeFreq == 2)
		{
			wsprintf(strText, _T("2M"));
			
			gSysInfo.wRelay |= 0x100;
			gSysInfo.wRelay &= 0xFF5F;
		
			gSysInfo.wRelay |= 0x8000;
			gSysInfo.wRelay &= 0xAFFF;
		
			ReceiveParam_ad[chNo].nLvBo = 100;
		}
		else if(ReceiveParam_ad[chNo].nProbeFreq == 3)
		{
			wsprintf(strText, _T("��Ƶ��"));
			
			gSysInfo.wRelay |= 0x180;
			gSysInfo.wRelay &= 0xFFDF;
		
			gSysInfo.wRelay |= 0xC000;
			gSysInfo.wRelay &= 0xEFFF;
		
			ReceiveParam_ad[chNo].nLvBo = 125;
		}
		else if(ReceiveParam_ad[chNo].nProbeFreq == 4)
		{
			wsprintf(strText, _T("5M"));
			
			gSysInfo.wRelay |= 0x120;
			gSysInfo.wRelay &= 0xFFBF;
		
			gSysInfo.wRelay |= 0x9000;
			gSysInfo.wRelay &= 0xDFFF;
		
			ReceiveParam_ad[chNo].nLvBo = 200;
		}
		else if(ReceiveParam_ad[chNo].nProbeFreq == 5) //2.5M խ��	
		{
			wsprintf(strText, _T("2.5M"));
			
			gSysInfo.wRelay |= 0x160;	
		
			gSysInfo.wRelay |= 0xB000;		

			ReceiveParam_ad[chNo].nLvBo = 150;
		}

		ReceiveParam_ad[0].nProbeFreq = ReceiveParam_ad[chNo].nProbeFreq;
		ReceiveParam_ad[1].nProbeFreq = ReceiveParam_ad[chNo].nProbeFreq;

		ReceiveParam_ad[0].nLvBo = ReceiveParam_ad[chNo].nLvBo;
		ReceiveParam_ad[1].nLvBo = ReceiveParam_ad[chNo].nLvBo;

		nMax500Value = (2<<8) + ReceiveParam_ad[chNo].nLvBo;
		send_max500(nMax500Value);
	
		Sleep(1);
		nMax500Value = (3<<8) + ReceiveParam_ad[chNo].nLvBo;
		send_max500(nMax500Value);
	}
	//yachang.huang rum on 2013-11-19
	//sys_reg->wRelay = gSysInfo.wRelay;
	LeaveCriticalSection(&csSysInfo);
	DeleteCriticalSection(&csSysInfo);

	if(hWnd)
	{
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

		// �����迹ֵ
	//	wsprintf(strText, _T("%d"), ReceiveParam_ad[chNo].nLvBo);
	//	DrawLableText(hBtnRecvParamImpedance, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

		// �������õ�Ĭ���ļ�
		SaveParamFile(lpDefaultParamFile);
	}
}

/**************************
*�˲�Ƶ����ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnRecvParamWaveBandProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_PAINT:
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 0)
		{
			wsprintf(strText, _T("1M"));			
		}
		else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 1)
		{
			wsprintf(strText, _T("0.5M"));			
		}
		else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 2)
		{
			wsprintf(strText, _T("2M"));			
		}
		else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 3)
		{
			wsprintf(strText, _T("10M"));
		}
		else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 4)
		{
			wsprintf(strText, _T("5M"));
		}
		else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 5) //2.5M խ��	
		{
			wsprintf(strText, _T("2.5M"));
		}
		DrawLableText(Wnd, strText, 24, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

		break;
	case WM_KEYDOWN:
		{
			int i = 0;
			if(wParam == VK_ESCAPE)
			{
				HWND hParentWnd = GetParent(Wnd);

				if(hParentWnd != NULL)
					SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
			}
		
			else if(wParam == VK_UP)
			{
				while(recvParamBtnList[i] != currentRecvParamBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentRecvParamBtn = recvParamBtnList[i];
				SetFocus(currentRecvParamBtn);
				gCurrentFocusBtn = currentRecvParamBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(recvParamBtnList[i] != currentRecvParamBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentRecvParamBtn = recvParamBtnList[i];
				SetFocus(currentRecvParamBtn);
				gCurrentFocusBtn = currentRecvParamBtn;
			}
			else if(wParam == VK_LEFT)
			{
				// songchenguang add start 2010-04-22
				if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq > MIN_LVBO_TYPE)
					ReceiveParam_ad[CurrentChannelNo].nProbeFreq --;
				else
					ReceiveParam_ad[CurrentChannelNo].nProbeFreq = MAX_LVBO_TYPE;

				SetLvBoLableAndExecute(Wnd, CurrentChannelNo);
				// songchenguang add end 2010-04-22
			}
			else if(wParam == VK_RIGHT)
			{
				// songchenguang add start 2010-04-22
				if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq < MAX_LVBO_TYPE)
					ReceiveParam_ad[CurrentChannelNo].nProbeFreq ++;
				else
					ReceiveParam_ad[CurrentChannelNo].nProbeFreq = MIN_LVBO_TYPE;

				SetLvBoLableAndExecute(Wnd, CurrentChannelNo);
				// songchenguang add end 2010-04-22
			}
			else if(wParam == '\t')
			{
				HWND hParentWnd = GetParent(Wnd);

				if(hParentWnd != NULL)
					SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '1')
			{
				PostMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '2')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '3')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '4')
			{
				PostMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == 'D')
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'E')
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'B')	// ����
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
		}
		break;
//	case WM_CTLCOLORSTATIC:
//		PostMessage(Wnd,WM_PAINT,(WPARAM)0,(LPARAM)0);
//		SetBkMode((HDC)wParam,TRANSPARENT);
//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
//		return (LRESULT)hStaticBrush;
	default:
		break;
	}
	return CallWindowProc(OldBtnRecvParamWaveBandProc,Wnd,Msg,wParam,lParam);
}

void SetRefPlusLableAndExecute(HWND hWnd, const float& fBasePlus, const float& fRefPlus, const BOOL& bSave, const WORD& wChNo)
{
	// ����
	if(hWnd)
	{
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		wsprintf(strText, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}

	// ��ֵ
	if(!gbGuanLian)
	{
		//ARS200FPGA_CH_PLUS_REG* ch_plus_reg = NULL;
		CSCAN_ARS200FPGA_CH_CONFIG_REG* ch_plus_reg = NULL;
		if(wChNo == 0)
		{
			ch_plus_reg = cscan_ch1_config_reg;
		}
		else if(wChNo == 1)
		{
			ch_plus_reg = cscan_ch2_config_reg;
		}
		else
			return;
		WORD wTemp0 = 0;
		WORD wTemp1 = 0;
		WORD wTemp2 = 0;
		WORD wTemp3 = 0;

		float fTotalPlus = fBasePlus + fRefPlus;
		dBProcess(fTotalPlus, wTemp0, wTemp1, wTemp2, wTemp3);

		ch_plus_reg->wdBRef = wTemp0;
		PlusChangeTurn(wChNo);

		ch_plus_reg->wdBLevel1 = wTemp1;
		PlusChangeTurn(wChNo);

		ch_plus_reg->wdBLevel2 = wTemp2;
		PlusChangeTurn(wChNo);

		ch_plus_reg->wdBLevel3 = wTemp3;
		PlusChangeTurn(wChNo);

		if(wChNo == 0)
			gCh1Plus = fTotalPlus;
		else if(wChNo == 1)
			gCh2Plus = fTotalPlus;
	}
	else
	{
		DB_ITEM dB1;
		memset(&dB1, 0, sizeof(DB_ITEM));
		DB_ITEM dB2;
		memset(&dB2, 0, sizeof(DB_ITEM));

		GuanLianDBProcess(fBasePlus,
						  fRefPlus,
						  dB1,
						  dB2);

		cscan_ch1_config_reg->wdBRef = dB1.wDB_A;
		PlusChangeTurn(0);	

		cscan_ch1_config_reg->wdBLevel1 = dB1.wDB_B;
		PlusChangeTurn(0);

		cscan_ch1_config_reg->wdBLevel2 = dB1.wDB_C;
		PlusChangeTurn(0);

		cscan_ch1_config_reg->wdBLevel3 = dB1.wDB_D;
		PlusChangeTurn(0);

		cscan_ch2_config_reg->wdBRef = dB2.wDB_A;
		PlusChangeTurn(1);	

		cscan_ch2_config_reg->wdBLevel1 = dB2.wDB_B;
		PlusChangeTurn(1);

		cscan_ch2_config_reg->wdBLevel2 = dB2.wDB_C;
		PlusChangeTurn(1);

		cscan_ch2_config_reg->wdBLevel3 = dB2.wDB_D;
		PlusChangeTurn(1);

		ReceiveParam_ad[0].fRefPlus = fRefPlus;
		ReceiveParam_ad[1].fRefPlus = fRefPlus;
	}

	memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
	swprintf(gszTextRefPlus, _T("%.1f"), fRefPlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
	
	if(gwDeviceType == 2)//��·ר��ʱ������������ο�����һ�� 2011-05-27 wangbingfu 
	{
		gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]] = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
	}
	// �������õ�Ĭ���ļ�
	if(bSave)
		SaveParamFile(lpDefaultParamFile);
}


LRESULT CALLBACK btnRecvParamReferenceGainProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			fPlusStep = PLUSSTEPS[nPlusStepIndex];
			break;
		case WM_PAINT:
			break;
		case WM_KEYDOWN:
		{
			int i = 0;
			if(wParam == VK_ESCAPE)
			{				
				HWND hParentWnd = GetParent(Wnd);

				if(hParentWnd != NULL)
					SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
			}
		
			else if(wParam == VK_UP)
			{
				while(recvParamBtnList[i] != currentRecvParamBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentRecvParamBtn = recvParamBtnList[i];
				SetFocus(currentRecvParamBtn);
				gCurrentFocusBtn = currentRecvParamBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(recvParamBtnList[i] != currentRecvParamBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentRecvParamBtn = recvParamBtnList[i];
				SetFocus(currentRecvParamBtn);
				gCurrentFocusBtn = currentRecvParamBtn;
			}
			else if(wParam == VK_LEFT)
			{
				float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;

				// songchenguang add start 2010-04-22
				if(ReceiveParam_ad[CurrentChannelNo].fRefPlus > (MIN_PLUS + fPlusStep))
					ReceiveParam_ad[CurrentChannelNo].fRefPlus -= fPlusStep;
				else
					ReceiveParam_ad[CurrentChannelNo].fRefPlus = MIN_PLUS;

				SetRefPlusLableAndExecute(Wnd, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus,TRUE, CurrentChannelNo);
				// songchenguang add end 2010-04-22
				refGainChangeCurve(fOldRefPlus, TRUE);//wangbingfu add at 2011-03-09
			}
			else if(wParam == VK_RIGHT)
			{
				float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;

				// songchenguang add start 2010-04-22
				float fPlusRange = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus;
				float fMaxValue = fPlusRange > 80.0f ? 80.0f : fPlusRange;

				if(ReceiveParam_ad[CurrentChannelNo].fRefPlus < (fMaxValue - fPlusStep))
					ReceiveParam_ad[CurrentChannelNo].fRefPlus += fPlusStep;
				else
					ReceiveParam_ad[CurrentChannelNo].fRefPlus = fMaxValue;

				SetRefPlusLableAndExecute(Wnd, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus,TRUE, CurrentChannelNo);
				// songchenguang add end 2010-04-22
				refGainChangeCurve(fOldRefPlus, TRUE);//wangbingfu add at 2011-03-09
			}
			else if(wParam == 'F')
			{
				if(nPlusStepIndex < 3)
					nPlusStepIndex++;
				else
					nPlusStepIndex = 0;

				fPlusStep = PLUSSTEPS[nPlusStepIndex];

				memset(gszTextStep, 0, sizeof(gszTextStep));
				swprintf(gszTextStep, _T("%.1f"), fPlusStep);
				SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			}
			else if(wParam == '\t')
			{
				HWND hParentWnd = GetParent(Wnd);

				if(hParentWnd != NULL)
					SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '1')
			{
				PostMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '2')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '3')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '4')
			{
				PostMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == 'D')
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'E')
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'B')	// ����
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
		}
		break;
//	case WM_CTLCOLORSTATIC:
//		PostMessage(Wnd,WM_PAINT,(WPARAM)0,(LPARAM)0);
//		SetBkMode((HDC)wParam,TRANSPARENT);
//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
//		return (LRESULT)hStaticBrush;
	default:
		break;
	}
	return CallWindowProc(OldBtnRecvParamReferenceGainProc,Wnd,Msg,wParam,lParam);
}
/*
void SetLittleModifyLableAndExecute(HWND hWnd)
{
	// ����
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%d"), ReceiveParam_ad[CurrentChannelNo].nLvBo);

	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	// ��ֵ
	WORD nMax500Value = 0;

	if(!gbGuanLian)
	{
		if(CurrentChannelNo == 0)
		{
			nMax500Value = (2<<8) + ReceiveParam_ad[CurrentChannelNo].nLvBo;
		}
		else if(CurrentChannelNo == 1)
		{
			nMax500Value = (3<<8) + ReceiveParam_ad[CurrentChannelNo].nLvBo;
		}

		send_max500(nMax500Value);
	}
	else
	{
		nMax500Value = (2<<8) + ReceiveParam_ad[CurrentChannelNo].nLvBo;
		send_max500(nMax500Value);

		Sleep(1);
		nMax500Value = (3<<8) + ReceiveParam_ad[CurrentChannelNo].nLvBo;
		send_max500(nMax500Value);

		ReceiveParam_ad[0].nLvBo = ReceiveParam_ad[CurrentChannelNo].nLvBo;
		ReceiveParam_ad[1].nLvBo = ReceiveParam_ad[CurrentChannelNo].nLvBo;
	}

	// �������õ�Ĭ���ļ�
	SaveParamFile(lpDefaultParamFile);
}
*/
void displayImpedance(HWND hWnd,WORD wChNo, WORD wImpedance)
{
	if(wChNo == 0)
	{
		if(wImpedance == 0)
			DrawLableText(hBtnRecvParamImpedance, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
		else if(wImpedance == 1)
			DrawLableText(hBtnRecvParamImpedance, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}
	else if(wChNo == 1)
	{
		if(wImpedance == 2)
			DrawLableText(hBtnRecvParamImpedance, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
		else if(wImpedance == 0)
			DrawLableText(hBtnRecvParamImpedance, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}
}
void SetImpedanceLableAndExecute(HWND hWnd, WORD wImpedeance, WORD wChNo, BOOL bSaveFlag)
{
	if(hWnd)
	{
		displayImpedance(hWnd, wChNo, wImpedeance);
	}
	
	if(wChNo == 0)
	{
		if(ReceiveParam_ad[wChNo].wImpedance == 0) //A��
		{
			gSysInfo.wRelay &= 0xFFFC;
			//yachang.huang rum on 2013-11-19
			//sys_reg->wRelay = gSysInfo.wRelay;						
		}
		else if(ReceiveParam_ad[wChNo].wImpedance == 1)//A��
		{			
			gSysInfo.wRelay &= 0xFFFC;
			gSysInfo.wRelay |= 0x1;
			//yachang.huang rum on 2013-11-19
			//sys_reg->wRelay = gSysInfo.wRelay;
		}
	}
	else if(wChNo == 1)
	{
		if(ReceiveParam_ad[wChNo].wImpedance == 2) //B��
		{
			gSysInfo.wRelay &= 0xFFFC;
			gSysInfo.wRelay |= 0x2;
			//yachang.huang rum on 2013-11-19
			//sys_reg->wRelay = gSysInfo.wRelay;
		}
		else if(ReceiveParam_ad[wChNo].wImpedance == 0)//B��
		{
			gSysInfo.wRelay &= 0xFFFC;
			//yachang.huang rum on 2013-11-19
			//sys_reg->wRelay = gSysInfo.wRelay;
		}
	}

	if(bSaveFlag)
	{
		SaveParamFile(lpDefaultParamFile);
	}
}
LRESULT CALLBACK btnRecvParamImpedanceProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		break;
	case WM_KEYDOWN:
		{
			int i = 0;
			if(wParam == VK_ESCAPE)
			{
				HWND hParentWnd = GetParent(Wnd);

				if(hParentWnd != NULL)
					SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
			}
		
			else if(wParam == VK_UP)
			{
				while(recvParamBtnList[i] != currentRecvParamBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentRecvParamBtn = recvParamBtnList[i];
				SetFocus(currentRecvParamBtn);
				gCurrentFocusBtn = currentRecvParamBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(recvParamBtnList[i] != currentRecvParamBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentRecvParamBtn = recvParamBtnList[i];
				SetFocus(currentRecvParamBtn);
				gCurrentFocusBtn = currentRecvParamBtn;
			}
			else if(wParam == VK_LEFT || wParam == VK_RIGHT)
			{
				if(CurrentChannelNo == 0)
				{
					if(ReceiveParam_ad[CurrentChannelNo].wImpedance == 0) //A�� ����A�� ����λΪ00 �鿴�̵�������˵��
					{
						gSysInfo.wRelay &= 0xFFFC;
						gSysInfo.wRelay |= 0x1;
						//yachang.huang rum on 2013-11-19
						//sys_reg->wRelay = gSysInfo.wRelay;
						ReceiveParam_ad[CurrentChannelNo].wImpedance = 1;
					}
					else if(ReceiveParam_ad[CurrentChannelNo].wImpedance == 1)//A�� ����A�� ����λΪ01 �鿴�̵�������˵��
					{
						gSysInfo.wRelay &= 0xFFFC;
						//yachang.huang rum on 2013-11-19
						//sys_reg->wRelay = gSysInfo.wRelay;
						ReceiveParam_ad[CurrentChannelNo].wImpedance = 0;
					}
				}
				else if(CurrentChannelNo == 1)
				{
					if(ReceiveParam_ad[CurrentChannelNo].wImpedance == 2) //B�� ����B�� ����λΪ10 �鿴�̵�������˵��
					{
						gSysInfo.wRelay &= 0xFFFC;
						//yachang.huang rum on 2013-11-19
						//sys_reg->wRelay = gSysInfo.wRelay;
						ReceiveParam_ad[CurrentChannelNo].wImpedance = 0;
					}
					else if(ReceiveParam_ad[CurrentChannelNo].wImpedance == 0)//B�� ����B�� ����λΪ00 �鿴�̵�������˵��
					{
						gSysInfo.wRelay &= 0xFFFC;
						gSysInfo.wRelay |= 0x2;
						//yachang.huang rum on 2013-11-19
						//sys_reg->wRelay = gSysInfo.wRelay;
						ReceiveParam_ad[CurrentChannelNo].wImpedance = 2;
					}
				}
				displayImpedance(Wnd, CurrentChannelNo, ReceiveParam_ad[CurrentChannelNo].wImpedance);
				// �������õ�Ĭ���ļ�
				SaveParamFile(lpDefaultParamFile);
			}
			else if(wParam == '\t')
			{
				HWND hParentWnd = GetParent(Wnd);

				if(hParentWnd != NULL)
					SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '1')
			{
				PostMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '2')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '3')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '4')
			{
				PostMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == 'D')
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'E')
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'B')	// ����
			{
				PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
		}
		break;
//	case WM_CTLCOLORSTATIC:
//		PostMessage(Wnd,WM_PAINT,(WPARAM)0,(LPARAM)0);
//		SetBkMode((HDC)wParam,TRANSPARENT);
//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
//		return (LRESULT)hStaticBrush;
	default:
		break;
	}
	return CallWindowProc(OldBtnRecvParamImpedanceProc,Wnd,Msg,wParam,lParam);
}
