/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * �����������Դ�ļ�
 * Filename: sendParamWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "sendParamWin.h"
#include "interfaceCmdID.h"

#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "const.h"
#include "win_com.h"
#include "craftParamWin.h"

HWND hWndSendParam;				//����������ھ��
HWND hBtnSendParamVoltage;		//��ѹ��ť���
HWND hBtnSendParamMaiKuan;		//����ʽ��ť���
HWND hBtnSendParamRepeatFreq;	//�ظ�Ƶ�ʰ�ť���
HWND hBtnSendParamModify;		//������ť���
HWND hBtnSendParamHeadQianYan;	//̽ͷǰ�ذ�ť���


HWND sendParamBtnList[5];						//������������еİ�ť��
WNDPROC OldBtnSendParamVoltageProc = NULL;		//��ѹ��ťĬ�ϴ������
WNDPROC OldBtnSendParamMaiKuanProc = NULL;		//����ťĬ�ϴ������
WNDPROC OldBtnSendParamRepeatFreqProc = NULL;	//�ظ�Ƶ�ʰ�ťĬ�ϴ������
WNDPROC OldBtnSendParamModifyProc = NULL;		//������ťĬ�ϴ������
WNDPROC OldBtnSendParamHeadQianYanProc = NULL;	//̽ͷǰ�ذ�ťĬ�ϴ������


HWND currentSendParamBtn;	//����������е�ǰѡ�еİ�ť

static WORD buttonNum = 5;			//��ť�ؼ���
extern HWND gCurrentFocusBtn;
// songchenguang add start 2010-04-21
// ���Ͳ����Ի������
SEND_PARAM_DATA SendParam_ad[2];

const WORD nPressStep = 10;
const WORD nMaiKuanStep = 10;
const WORD nRepeatFreqStep = 10;
static float fAmendStep = 10;
static float fUSStep = 0.01;
static WORD nUSStepIndex = 0;

static WORD nAmendStepIndex = 0;				// ��ǰ�������
WORD wVoltageType = 0;			//��ѹ����  0������ 1���� 2���� 3����
	
extern ARS200FPGA_CH_CON_REG *chCurrent_con_reg;
extern ARS200FPGA_CH_CON_REG *ch1_con_reg;
extern ARS200FPGA_CH_CON_REG *ch2_con_reg;
extern ARS200FPGA_CH_DATA_REG *chCurrent_data_reg;
extern ARS200FPGA_CH_DATA_REG *ch1_data_reg;
extern ARS200FPGA_CH_DATA_REG *ch2_data_reg;

extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];

extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern HWND hStatusBar;
extern wchar_t gszTextStep[10];
extern HWND hWndMain;
extern BOOL gbGuanLian;
// songchenguang add end 2010-04-21 

//static HBRUSH hStaticBrush = NULL;	//��ˢ
extern HWND hWndParam;				//�������ھ��
extern HWND currentParamBtn;		//�������е�ǰѡ�еİ�ť
extern HINSTANCE hInst;				//����ʵ�����
extern WORD CurrentChannelNo;
extern SYS_INFO gSysInfo;
extern CRITICAL_SECTION csSysInfo;

extern WORD gwDeviceType;	//�豸���� 0:̽����	1:�����
extern HWND hWndChParam;			//���������ھ��
extern HWND currentChParamBtn;		//���������е�ǰѡ�еİ�ť
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���
extern BOOL gbHotDB;			// ��ݼ�����
extern BOOL gbHotRange;			// ��ݼ�����

extern float fRangeStart[2];
extern float fRangeEnd[2];

extern CRITICAL_SECTION csRange;
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern WORD gwDeviceType;	//�豸���� 0:̽����	1:�����
extern ARS200FPGA_SYS_REG* sys_reg;
extern WORD wTanTouLeiXing[CHANNEL_NUM];	//̽ͷ����

extern CSCAN_ARS200FPGA_CH_CON_REG *cscan_ch1_con_reg;
extern CSCAN_ARS200FPGA_CH_CON_REG *cscan_ch2_con_reg;
extern CSCAN_ARS200FPGA_CH_CONFIG_REG *cscan_ch1_config_reg;
extern CSCAN_ARS200FPGA_CH_CONFIG_REG *cscan_ch2_config_reg;

ATOM registerSendParamChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) sendParamChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szSendParamChild;

	return RegisterClass(&wc);
}
static void displayHeadQianYan(HWND hwnd, float fQianYan)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	if(DisplaySet_ad[CurrentChannelNo].fZheSheJiao > 0)	//б̽ͷ
	{
		wTanTouLeiXing[CurrentChannelNo] = 0;
	}
	else
	{
		wTanTouLeiXing[CurrentChannelNo] = 1;
	}

	swprintf(strText, _T("%.1f"), fQianYan);
	if(wTanTouLeiXing[CurrentChannelNo] == 0)
	{
		DrawLableText(hwnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	}
	else
	{
		DrawLableText(hwnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, RGB(219,207,202), LABEL_START_X, LABEL_5_START_Y);
	}
}
WORD typeToVoltage(WORD wType)
{
	WORD wVoltage;
	
	if(wType == 0)
	{
		wVoltage = 50;
	}
	else if(wType == 1)
	{
		wVoltage = 100;
	}
	else if(wType == 2)
	{
		wVoltage = 200;
	}
	else if(wType == 3)
	{
		wVoltage = 400;
	}
	
	return wVoltage;
}
WORD VoltageToType(WORD wVoltage)
{
	WORD wVoltageType;

	if(wVoltage == 50)
	{
		wVoltageType = 0;
	}
	else if(wVoltage == 100)
	{
		wVoltageType = 1;
	}
	else if(wVoltage == 200)
	{
		wVoltageType = 2;
	}
	else if(wVoltage == 400)
	{
		wVoltageType = 3;
	}
	
	return wVoltageType;
}
void displayVoltage(HWND hWnd, WORD wVoltageType)
{
	if(wVoltageType == 0)
		DrawLableText(hWnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	if(wVoltageType == 1)
		DrawLableText(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	if(wVoltageType == 2)
		DrawLableText(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	if(wVoltageType == 3)
		DrawLableText(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}
void UpdateSendMenuInfo(const WORD& chNo)
{
	// ��ѹ
	wchar_t strText[20];
/*	memset(strText, 0, sizeof(strText));
	swprintf(strText, _T("%d"), SendParam_ad[chNo].nPress);
	DrawLableText(hBtnSendParamVoltage, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
*/
	displayVoltage(hBtnSendParamVoltage, wVoltageType);

	// ����
	memset(strText, 0, sizeof(strText));
	swprintf(strText, _T("%d"), SendParam_ad[chNo].nMaiKuan);
	DrawLableText(hBtnSendParamMaiKuan, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

//	memset(strText, 0, sizeof(strText));
//	swprintf(strText, _T("%d"), SendParam_ad[chNo].nRefrainFrequency);
//	DrawLableText(hBtnSendParamRepeatFreq, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

	// ���
	memset(strText, 0, sizeof(strText));
	swprintf(strText, _T("%.2f"), SendParam_ad[chNo].nAmend);
	DrawLableText(hBtnSendParamModify, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

	// ǰ��
	displayHeadQianYan(hBtnSendParamHeadQianYan, SendParam_ad[chNo].fQianYan);
}

LRESULT CALLBACK sendParamChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;
	WORD nReFreq = 50;
//	LPDRAWITEMSTRUCT pdis;
//	TCHAR szHello[MAX_LOADSTRING];
//	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	switch (message) 
	{
		case WM_CREATE:
			wVoltageType = VoltageToType(SendParam_ad[CurrentChannelNo].nPress);
		//	if(hStaticBrush == NULL)	
		//		hStaticBrush = CreateSolidBrush(BK_COLOR_WHITE);			
			//����ť�ؼ�
			if(gwDeviceType == 0 || gwDeviceType == 2) //̽�˻���·
			{
				buttonNum = 5;
				hBtnSendParamVoltage = CreateWindow(_T("BUTTON"),_T("voltage"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_SENDPARAM_VOLTAGE,hInst,NULL);
				hBtnSendParamMaiKuan = CreateWindow(_T("BUTTON"),_T("maikuan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 +BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_SENDPARAM_MAIKUAN,hInst,NULL);
				hBtnSendParamRepeatFreq = CreateWindow(_T("BUTTON"),_T("repeatFreq"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_SENDPARAM_REPEATFREQ,hInst,NULL);
				hBtnSendParamModify = CreateWindow(_T("BUTTON"),_T("modify"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_SENDPARAM_MODIFY,hInst,NULL);
				hBtnSendParamHeadQianYan= CreateWindow(_T("BUTTON"),_T("headQianYan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT-4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_SENDPARAM_HEADQIANYAN,hInst,NULL);
	
				sendParamBtnList[0] = hBtnSendParamVoltage;
				sendParamBtnList[1] = hBtnSendParamMaiKuan;
				sendParamBtnList[2] = hBtnSendParamRepeatFreq;
				sendParamBtnList[3] = hBtnSendParamModify;
				sendParamBtnList[4] = hBtnSendParamHeadQianYan;

				//��ť�ؼ����Զ��崦�����
				OldBtnSendParamVoltageProc = (WNDPROC) GetWindowLong(hBtnSendParamVoltage,GWL_WNDPROC);
				SetWindowLong(hBtnSendParamVoltage,GWL_WNDPROC,(LONG)btnSendParamVoltageProc);
				
				OldBtnSendParamMaiKuanProc = (WNDPROC) GetWindowLong(hBtnSendParamMaiKuan,GWL_WNDPROC);
				SetWindowLong(hBtnSendParamMaiKuan,GWL_WNDPROC,(LONG)btnSendParamMaiKuanProc);
				
				OldBtnSendParamRepeatFreqProc = (WNDPROC) GetWindowLong(hBtnSendParamRepeatFreq,GWL_WNDPROC);
			    SetWindowLong(hBtnSendParamRepeatFreq,GWL_WNDPROC,(LONG)btnSendParamRepeatFreqProc);
				
				OldBtnSendParamModifyProc = (WNDPROC) GetWindowLong(hBtnSendParamModify,GWL_WNDPROC);
				SetWindowLong(hBtnSendParamModify,GWL_WNDPROC,(LONG)btnSendParamModifyProc);

				OldBtnSendParamHeadQianYanProc = (WNDPROC) GetWindowLong(hBtnSendParamHeadQianYan,GWL_WNDPROC);
				SetWindowLong(hBtnSendParamHeadQianYan,GWL_WNDPROC,(LONG)btnSendParamHeadQianYanProc);
			}
			else if(gwDeviceType == 1)
			{
				buttonNum = 4;
				hBtnSendParamVoltage = CreateWindow(_T("BUTTON"),_T("voltage"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_SENDPARAM_VOLTAGE,hInst,NULL);
				hBtnSendParamMaiKuan = CreateWindow(_T("BUTTON"),_T("maikuan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 +BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_SENDPARAM_MAIKUAN,hInst,NULL);
				hBtnSendParamRepeatFreq = CreateWindow(_T("BUTTON"),_T("repeatFreq"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_SENDPARAM_REPEATFREQ,hInst,NULL);
				hBtnSendParamModify = CreateWindow(_T("BUTTON"),_T("modify"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_SENDPARAM_MODIFY,hInst,NULL);
	
				sendParamBtnList[0] = hBtnSendParamVoltage;
				sendParamBtnList[1] = hBtnSendParamMaiKuan;
				sendParamBtnList[2] = hBtnSendParamRepeatFreq;
				sendParamBtnList[3] = hBtnSendParamModify;

				//��ť�ؼ����Զ��崦�����
				OldBtnSendParamVoltageProc = (WNDPROC) GetWindowLong(hBtnSendParamVoltage,GWL_WNDPROC);
				SetWindowLong(hBtnSendParamVoltage,GWL_WNDPROC,(LONG)btnSendParamVoltageProc);
				
				OldBtnSendParamMaiKuanProc = (WNDPROC) GetWindowLong(hBtnSendParamMaiKuan,GWL_WNDPROC);
				SetWindowLong(hBtnSendParamMaiKuan,GWL_WNDPROC,(LONG)btnSendParamMaiKuanProc);
				
				OldBtnSendParamRepeatFreqProc = (WNDPROC) GetWindowLong(hBtnSendParamRepeatFreq,GWL_WNDPROC);
			    SetWindowLong(hBtnSendParamRepeatFreq,GWL_WNDPROC,(LONG)btnSendParamRepeatFreqProc);
				
				OldBtnSendParamModifyProc = (WNDPROC) GetWindowLong(hBtnSendParamModify,GWL_WNDPROC);
				SetWindowLong(hBtnSendParamModify,GWL_WNDPROC,(LONG)btnSendParamModifyProc);
			}
			else
			{
				;//
			}
			
			currentSendParamBtn = sendParamBtnList[0];//Ĭ�ϵ�ǰѡ�е�ѹ��ť
			SetFocus(currentSendParamBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentSendParamBtn;
			////////////////////////////////////////////////////
			// ��ʱ���ظ�Ƶ����Ϊ50, songchenguang 2010-08-24
			// ���·�ֵ
		/*
		if(!gbGuanLian)
		{
			chCurrent_con_reg->wRepeatCounter = 200000 / nReFreq;
			SendParam_ad[CurrentChannelNo].nRefrainFrequency = nReFreq;
		}
		else
		{
			ch1_con_reg->wRepeatCounter = 200000 / nReFreq;
			ch2_con_reg->wRepeatCounter = 200000 / nReFreq;
	
			SendParam_ad[0].nRefrainFrequency = nReFreq;
			SendParam_ad[1].nRefrainFrequency = nReFreq;
		}*/
		/////////////////////////////////////////////////////////
	
			break;

		case WM_PAINT:

			hdc = BeginPaint(hWndSendParam, &ps);
			
			
			EndPaint(hWndSendParam, &ps);
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

				DestroyWindow(hWndSendParam);
				hWndSendParam = 0;
			}
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM://�ػ���ť
			wchar_t strText[20];
			memset(strText, 0, sizeof(strText));

			pdis=(LPDRAWITEMSTRUCT)lParam;
			sendParam_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnSendParamVoltage)
			{
				displayVoltage(pdis->hwndItem, wVoltageType);
			}
			if(pdis->hwndItem == hBtnSendParamMaiKuan)
			{
				swprintf(strText, _T("%d"), SendParam_ad[CurrentChannelNo].nMaiKuan);
				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
			}
			if(pdis->hwndItem == hBtnSendParamRepeatFreq)
			{
//				swprintf(strText, _T("%d"), SendParam_ad[CurrentChannelNo].nRefrainFrequency);
/*
				if(SendParam_ad[CurrentChannelNo].nRefrainFrequency != MAX_RE_FREQ)
					swprintf(strText, _T("%d"), SendParam_ad[CurrentChannelNo].nRefrainFrequency);
				else
				{
					swprintf(strText, _T("�Զ�"));
				}
*/
				// ��ʱ songchenguang 2010-08-24
				swprintf(strText, _T("�Զ�"));
				
				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
			}
			if(pdis->hwndItem == hBtnSendParamModify)
			{
				swprintf(strText, _T("%.2f"), SendParam_ad[CurrentChannelNo].nAmend);
				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
			}
			if(pdis->hwndItem == hBtnSendParamHeadQianYan)
			{
				displayHeadQianYan(pdis->hwndItem, SendParam_ad[CurrentChannelNo].fQianYan);
			}
			
			// ��ⰴť�������ʾ��Ӧ�Ĳ���
			memset(gszTextStep, 0, sizeof(gszTextStep));
			
			if(pdis->hwndItem == currentSendParamBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					//if(pdis->hwndItem == hBtnSendParamVoltage)
					//	swprintf(gszTextStep, _T("%d"), nPressStep);
					if(pdis->hwndItem == hBtnSendParamMaiKuan)
						swprintf(gszTextStep, _T("%d"), nMaiKuanStep);
					else if(pdis->hwndItem == hBtnSendParamRepeatFreq)
						swprintf(gszTextStep, _T("%d"), nRepeatFreqStep);
					else if(pdis->hwndItem == hBtnSendParamModify)
						swprintf(gszTextStep, _T("%.2f"), fUSStep);
					else if(pdis->hwndItem == hBtnSendParamHeadQianYan)
						swprintf(gszTextStep, _T("%.1f"),fAmendStep);
					else
						swprintf(gszTextStep, _T(""));

					PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}

			
			break;
		case WM_CLOSE:
		
			break;
		case WM_DESTROY:
		//	if(hStaticBrush)
		//	{
		//		DeleteObject(hStaticBrush);
		//		hStaticBrush = NULL;
		//	}
			break;
		case WM_CHAR:
			InitializeCriticalSection(&csSysInfo);
			EnterCriticalSection(&csSysInfo);
			if( wParam == '\t' && gSysInfo.wWorkMode != OneSendOneReceive)
			{
				// ���°�ť������
				if(CurrentChannelNo == 0)
				{
					UpdateSendMenuInfo(1);
				}
				else
				{
					UpdateSendMenuInfo(0);
				}
			}

			LeaveCriticalSection(&csSysInfo);
			DeleteCriticalSection(&csSysInfo);

			SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

int sendParam_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_sendParamBtnBitmap[]=
	{
		{IDC_SENDPARAM_VOLTAGE	,IDB_BITMAP_DIANYA_NOSEL_R	 ,  IDB_BITMAP_DIANYA_SELECT_R		},
		{IDC_SENDPARAM_MAIKUAN	,IDB_BITMAP_MAIKUAN_NOSEL_R ,  IDB_BITMAP_MAIKUAN_SELECT_R		},
		{IDC_SENDPARAM_REPEATFREQ	,IDB_BITMAP_CHONGFUPINLV_NOSEL_R ,  IDB_BITMAP_CHONGFUPINLV_SELECT_R	},
		{IDC_SENDPARAM_MODIFY	,IDB_BITMAP_LINGDIAN_NOSEL_R	 ,  IDB_BITMAP_LINGDIAN_SELECT_R		},
		{IDC_SENDPARAM_HEADQIANYAN	,IDB_BITMAP_TANTOUQIANYAN_NOSEL_R	 ,  IDB_BITMAP_TANTOUQIANYAN_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_sendParamBtnBitmap);

	return ReturnVal;
}

// songchenguang add start 2010-04-21
void SetPressLableAndExecute(HWND hWnd)
{
	// ���ñ�ǩ
	if(hWnd)
	{	
		WORD wVoltageType = VoltageToType(SendParam_ad[CurrentChannelNo].nPress);
		displayVoltage(hWnd, wVoltageType);
	}
	// ���·�ֵ
	WORD nPress = WORD(SendParam_ad[CurrentChannelNo].nPress/400.0f*255.0f);

	send_mcp41010(nPress);
/*	
	if(gbGuanLian)
	{
		SendParam_ad[0].nPress = SendParam_ad[CurrentChannelNo].nPress;
		SendParam_ad[1].nPress = SendParam_ad[CurrentChannelNo].nPress;
	}
*/
	// �������õ�Ĭ���ļ�...
	if(hWnd)
		SaveParamFile(lpDefaultParamFile);
}
// songchenguang add end 2010-04-21

LRESULT CALLBACK btnSendParamVoltageProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:			
			break;
		case WM_PAINT:
			// songchenguang add start 2010-04-21
			wchar_t strText[20];

			memset(strText, 0, sizeof(strText));
			swprintf(strText, _T("%d"), SendParam_ad[CurrentChannelNo].nPress);
			//SetWindowText(hLableVoltage, strText);
			DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
			// songchenguang add end 2010-04-21
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
					while(sendParamBtnList[i] != currentSendParamBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSendParamBtn = sendParamBtnList[i];
					SetFocus(currentSendParamBtn);
					gCurrentFocusBtn = currentSendParamBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(sendParamBtnList[i] != currentSendParamBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSendParamBtn = sendParamBtnList[i];
					SetFocus(currentSendParamBtn);
					gCurrentFocusBtn = currentSendParamBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
				/*	
					// songchenguang add start 2010-04-21
					if(SendParam_ad[CurrentChannelNo].nPress > MIN_PRESSURE && 
						SendParam_ad[CurrentChannelNo].nPress > nPressStep)
						SendParam_ad[CurrentChannelNo].nPress -= nPressStep;
					else
						SendParam_ad[CurrentChannelNo].nPress = MIN_PRESSURE;

					SetPressLableAndExecute(Wnd);
					// songchenguang add end 2010-04-21
					*/
					if(wVoltageType == 0)
					{
						wVoltageType = 3;
					}
					else
						wVoltageType--;

				//	displayVoltage(Wnd, wVoltageType);
					
					SendParam_ad[0].nPress = typeToVoltage(wVoltageType);
					SendParam_ad[1].nPress = typeToVoltage(wVoltageType);
					SetPressLableAndExecute(Wnd);
				}
				else if(wParam == VK_RIGHT)
				{
				/*	// songchenguang add start 2010-04-21
					if(SendParam_ad[CurrentChannelNo].nPress < MAX_PRESSURE)
						SendParam_ad[CurrentChannelNo].nPress += nPressStep;
					else
						SendParam_ad[CurrentChannelNo].nPress = MAX_PRESSURE;
					
					SetPressLableAndExecute(Wnd);
					// songchenguang add end 2010-04-21
					*/
					if(wVoltageType == 3)
					{
						wVoltageType = 0;
					}
					else
						wVoltageType++;

				//	displayVoltage(Wnd, wVoltageType);
					
					SendParam_ad[0].nPress = typeToVoltage(wVoltageType);
					SendParam_ad[1].nPress = typeToVoltage(wVoltageType);					
					SetPressLableAndExecute(Wnd);
				}
				else if(wParam == '\t')
				{
//					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == 'D')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnSendParamVoltageProc,Wnd,Msg,wParam,lParam);
}

// songchenguang add start 2010-04-21
void SetMaiKuanLableAndExecute(HWND hWnd)
{
	// ���ñ�ǩ
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	swprintf(strText, _T("%d"), SendParam_ad[CurrentChannelNo].nMaiKuan);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

	// ���·�ֵ
	if(!gbGuanLian)
	{
/*		if(sys_reg->wWorkMode == OneSendOneReceive) //���ʱһ��һ�ղ�����
		{
			ch1_con_reg->wMaiKuan = int(float(SendParam_ad[CurrentChannelNo].nMaiKuan)/20.0 +0.5);
			ch2_con_reg->wMaiKuan = int(float(SendParam_ad[CurrentChannelNo].nMaiKuan)/20.0 +0.5);

			SendParam_ad[0].nMaiKuan = SendParam_ad[CurrentChannelNo].nMaiKuan;
			SendParam_ad[1].nMaiKuan = SendParam_ad[CurrentChannelNo].nMaiKuan;
		}
		else*/
		//	chCurrent_con_reg->wMaiKuan = int(float(SendParam_ad[CurrentChannelNo].nMaiKuan)/20.0 +0.5);
		if(CurrentChannelNo == 0)
		{
			cscan_ch1_con_reg->wMaiKuan = int(float(SendParam_ad[CurrentChannelNo].nMaiKuan)/(1000.0 / ADFREQUENCY) + 0.5);
		}
		else if(CurrentChannelNo == 1)
		{
			cscan_ch2_con_reg->wMaiKuan = int(float(SendParam_ad[CurrentChannelNo].nMaiKuan)/(1000.0 / ADFREQUENCY) +0.5);
		}
	}
	else
	{
		cscan_ch1_con_reg->wMaiKuan = int(float(SendParam_ad[CurrentChannelNo].nMaiKuan)/(1000.0 / ADFREQUENCY) +0.5);
		cscan_ch2_con_reg->wMaiKuan = int(float(SendParam_ad[CurrentChannelNo].nMaiKuan)/(1000.0 / ADFREQUENCY) +0.5);

		SendParam_ad[0].nMaiKuan = SendParam_ad[CurrentChannelNo].nMaiKuan;
		SendParam_ad[1].nMaiKuan = SendParam_ad[CurrentChannelNo].nMaiKuan;
	}

	// �������õ�Ĭ���ļ�...
	SaveParamFile(lpDefaultParamFile);
}
// songchenguang add end 2010-04-21

LRESULT CALLBACK btnSendParamMaiKuanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_PAINT:
		// songchenguang add start 2010-04-21
		wchar_t strText[20];

		memset(strText, 0, sizeof(strText));
		swprintf(strText, _T("%d"), SendParam_ad[CurrentChannelNo].nMaiKuan);
		//SetWindowText(hLableMaiKuan, strText);
		DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
		// songchenguang add end 2010-04-21
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
				while(sendParamBtnList[i] != currentSendParamBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentSendParamBtn = sendParamBtnList[i];
				SetFocus(currentSendParamBtn);
				gCurrentFocusBtn = currentSendParamBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(sendParamBtnList[i] != currentSendParamBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentSendParamBtn = sendParamBtnList[i];
				SetFocus(currentSendParamBtn);
				gCurrentFocusBtn = currentSendParamBtn;
			}
			else if(wParam == VK_RETURN)
			{
			}
			else if(wParam == VK_LEFT)
			{
				// songchenguang add start 2010-04-21
				if(SendParam_ad[CurrentChannelNo].nMaiKuan > MIN_MAIKUAN && 
					SendParam_ad[CurrentChannelNo].nMaiKuan > nMaiKuanStep)
					SendParam_ad[CurrentChannelNo].nMaiKuan -= nMaiKuanStep;
				else
					SendParam_ad[CurrentChannelNo].nMaiKuan = MIN_MAIKUAN;

				SetMaiKuanLableAndExecute(Wnd);
				// songchenguang add end 2010-04-21
			}
			else if(wParam == VK_RIGHT)
			{
				// songchenguang add start 2010-04-21
				if(SendParam_ad[CurrentChannelNo].nMaiKuan < MAX_MAIKUAN)
					SendParam_ad[CurrentChannelNo].nMaiKuan += nMaiKuanStep;
				else
					SendParam_ad[CurrentChannelNo].nMaiKuan = MAX_MAIKUAN;
				
				SetMaiKuanLableAndExecute(Wnd);
				// songchenguang add end 2010-04-21
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
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == 'D')
			{
				SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'E')
			{
				SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'B')	// ����
			{
				SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
		}
		break;
//	case WM_CTLCOLORSTATIC:
//		SetBkMode((HDC)wParam,TRANSPARENT);
//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
//		return (LRESULT)hStaticBrush;
	default:
		break;
	}
	return CallWindowProc(OldBtnSendParamMaiKuanProc,Wnd,Msg,wParam,lParam);
}

// songchenguang add start 2010-04-21
void SetReFreqLableAndExecute(HWND hWnd)
{
	DWORD nReFreq = SendParam_ad[CurrentChannelNo].nRefrainFrequency;

	if(nReFreq != MIN_RE_FREQ)
	{
		nReFreq = DWORD(DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1000/(6*DisplaySet_ad[CurrentChannelNo].fRange));	// 

		if(nReFreq > 1000)
			nReFreq = 1000;
		else if(nReFreq < 50)
			nReFreq = 50;
		//wangbingfu add start at 2010-3-15
		//ȡƵ��С��ֵ
		if(SendParam_ad[CurrentChannelNo].nRefrainFrequency < nReFreq)
			nReFreq = SendParam_ad[CurrentChannelNo].nRefrainFrequency;
		//wangbingfu add end at 2010-3-15
	}

	// ���ñ�ǩ
	if(hWnd)
	{
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		if(nReFreq == MIN_RE_FREQ)
			swprintf(strText, _T("%d"), nReFreq);
		else
		{
			swprintf(strText, _T("�Զ�"));
		}

		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

		// �������õ�Ĭ���ļ�...
		SaveParamFile(lpDefaultParamFile);
	}

	// ���·�ֵ
	if(!gbGuanLian)
		
	{
	//	chCurrent_con_reg->wRepeatCounter = WORD(200000 / nReFreq);
		if(CurrentChannelNo == 0)
		{
			cscan_ch1_con_reg->wRepeatCounter = 1950;//WORD(200000 / nReFreq);
		}
		else if(CurrentChannelNo == 1)
		{
			cscan_ch2_con_reg->wRepeatCounter = 1950;//WORD(200000 / nReFreq);
		}
		
		SendParam_ad[CurrentChannelNo].nRefrainFrequency = nReFreq;
	}
	else
	{
		cscan_ch1_con_reg->wRepeatCounter = WORD(200000 / nReFreq);
		cscan_ch2_con_reg->wRepeatCounter = WORD(200000 / nReFreq);

		SendParam_ad[0].nRefrainFrequency = SendParam_ad[CurrentChannelNo].nRefrainFrequency;
		SendParam_ad[1].nRefrainFrequency = SendParam_ad[CurrentChannelNo].nRefrainFrequency;
	}

	
}
// songchenguang add end 2010-04-21

LRESULT CALLBACK btnSendParamRepeatFreqProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_PAINT:
		// songchenguang add start 2010-04-21
		wchar_t strText[20];

		memset(strText, 0, sizeof(strText));
		swprintf(strText, _T("%d"), SendParam_ad[CurrentChannelNo].nRefrainFrequency);
		//SetWindowText(hLableRepeatFreq, strText);
		DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
		// songchenguang add end 2010-04-21
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
				while(sendParamBtnList[i] != currentSendParamBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentSendParamBtn = sendParamBtnList[i];
				SetFocus(currentSendParamBtn);
				gCurrentFocusBtn = currentSendParamBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(sendParamBtnList[i] != currentSendParamBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentSendParamBtn = sendParamBtnList[i];
				SetFocus(currentSendParamBtn);
				gCurrentFocusBtn = currentSendParamBtn;
			}
			/*  ��ʱ
			else if(wParam == VK_LEFT)
			{
				// songchenguang add start 2010-04-21
//				if(SendParam_ad[CurrentChannelNo].nRefrainFrequency > MIN_RE_FREQ && 
//					SendParam_ad[CurrentChannelNo].nRefrainFrequency > nRepeatFreqStep)
//					SendParam_ad[CurrentChannelNo].nRefrainFrequency -= nRepeatFreqStep;
//				else
					SendParam_ad[CurrentChannelNo].nRefrainFrequency = MIN_RE_FREQ;

				SetReFreqLableAndExecute(Wnd);
				// songchenguang add end 2010-04-21
			}
			else if(wParam == VK_RIGHT)
			{
				// songchenguang add start 2010-04-21
//				if(SendParam_ad[CurrentChannelNo].nRefrainFrequency < MAX_RE_FREQ)
//					SendParam_ad[CurrentChannelNo].nRefrainFrequency += nRepeatFreqStep;
//				else
					SendParam_ad[CurrentChannelNo].nRefrainFrequency = MAX_RE_FREQ;
				
				SetReFreqLableAndExecute(Wnd);
				// songchenguang add end 2010-04-21
			}
			*/
			else if(wParam == '\t')
			{
//				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				HWND hParentWnd = GetParent(Wnd);

				if(hParentWnd != NULL)
					SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '1')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == 'D')
			{
				SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'E')
			{
				SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'B')	// ����
			{
				SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
		}
		break;
//	case WM_CTLCOLORSTATIC:
//		SetBkMode((HDC)wParam,TRANSPARENT);
//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
//		return (LRESULT)hStaticBrush;
	default:
		break;
	}
	return CallWindowProc(OldBtnSendParamRepeatFreqProc,Wnd,Msg,wParam,lParam);
}

// songchenguang add start 2010-04-21
void SetAmendLableAndExecute(HWND hWnd)
{
	// ���ñ�ǩ
	if(hWnd)
	{
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		swprintf(strText, _T("%.2f"), SendParam_ad[CurrentChannelNo].nAmend);

		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	}

	// ���·�ֵ
	float fTemp = SendParam_ad[CurrentChannelNo].nAmend;

	if(!gbGuanLian)
		//chCurrent_data_reg->wMonitorZeroCounter = WORD(MicroSecond2Dot(fTemp));//WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fTemp));
	{
		if(CurrentChannelNo == 0)
		{
			cscan_ch1_config_reg->wMonitorZeroCounter = WORD(MicroSecond2Dot(fTemp));
		}
		else if(CurrentChannelNo == 1)
		{
			cscan_ch1_config_reg->wMonitorZeroCounter = WORD(MicroSecond2Dot(fTemp));
		}
	}
	else
	{
		cscan_ch1_config_reg->wMonitorZeroCounter = WORD(MicroSecond2Dot(fTemp));//WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fTemp));
		cscan_ch2_config_reg->wMonitorZeroCounter = WORD(MicroSecond2Dot(fTemp));//WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fTemp));

		SendParam_ad[0].nAmend = SendParam_ad[CurrentChannelNo].nAmend;
		SendParam_ad[1].nAmend = SendParam_ad[CurrentChannelNo].nAmend;
	}

	// �������õ�Ĭ���ļ�
	if(hWnd)
	{
		SaveParamFile(lpDefaultParamFile);
	}
}
// songchenguang add end 2010-04-21

LRESULT CALLBACK btnSendParamModifyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_CREATE:
		fUSStep = USSTEPS[nUSStepIndex];
		break;
	case WM_PAINT:
		// songchenguang add start 2010-04-21
		wchar_t strText[20];

		memset(strText, 0, sizeof(strText));
		swprintf(strText, _T("%.1f"), SendParam_ad[CurrentChannelNo].nAmend);
		//SetWindowText(hLableModify, strText);
		DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
		// songchenguang add end 2010-04-21
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
				while(sendParamBtnList[i] != currentSendParamBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentSendParamBtn = sendParamBtnList[i];
				SetFocus(currentSendParamBtn);
				gCurrentFocusBtn = currentSendParamBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(sendParamBtnList[i] != currentSendParamBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentSendParamBtn = sendParamBtnList[i];
				SetFocus(currentSendParamBtn);
				gCurrentFocusBtn = currentSendParamBtn;
			}
			else if(wParam == VK_LEFT)
			{
				// songchenguang add start 2010-04-21
				if((SendParam_ad[CurrentChannelNo].nAmend - fUSStep) > MIN_AMEND )
					SendParam_ad[CurrentChannelNo].nAmend -= fUSStep;
				else
					SendParam_ad[CurrentChannelNo].nAmend = MIN_AMEND;

				SetAmendLableAndExecute(Wnd);
				// songchenguang add end 2010-04-21
			}
			else if(wParam == VK_RIGHT)
			{
				// songchenguang add start 2010-04-21
				if((SendParam_ad[CurrentChannelNo].nAmend + fUSStep)< MAX_AMEND)
					SendParam_ad[CurrentChannelNo].nAmend += fUSStep;
				else
					SendParam_ad[CurrentChannelNo].nAmend = MAX_AMEND;
				
				SetAmendLableAndExecute(Wnd);
				// songchenguang add end 2010-04-21
			}
			else if(wParam == 'F') // ����
			{
				if(nUSStepIndex < 2)
					nUSStepIndex++;
				else
					nUSStepIndex = 0;

				fUSStep = USSTEPS[nUSStepIndex];

				memset(gszTextStep, 0, sizeof(gszTextStep));
				swprintf(gszTextStep, _T("%.2f"), fUSStep);
				SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == 'D')
			{
				SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'E')
			{
				SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
			else if(wParam == 'B')	// ����
			{
				SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
		}
		break;
//	case WM_CTLCOLORSTATIC:
//		SetBkMode((HDC)wParam,TRANSPARENT);
//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
//		return (LRESULT)hStaticBrush;
	default:
		break;
	}
	return CallWindowProc(OldBtnSendParamModifyProc,Wnd,Msg,wParam,lParam);
}

void SetObliqueProbeQianYan(HWND hWnd)
{
	if(hWnd)
	{
		displayHeadQianYan(hWnd, SendParam_ad[CurrentChannelNo].fQianYan);
		// �������õ�Ĭ���ļ�
		SaveParamFile(lpDefaultParamFile);
	}

	float fAngle = float(DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI);
	// ��ΪX״̬��
	InitializeCriticalSection(&csRange);
	EnterCriticalSection(&csRange);

	if(!gbGuanLian)
	{
		if(gwDeviceType == 0)
		{
	 		if(!DisplaySet_ad[CurrentChannelNo].bSFlag)
	 		{
	 			fRangeStart[CurrentChannelNo] = 
	 				float((ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3)*sin(fAngle) - SendParam_ad[CurrentChannelNo].fQianYan); // ��ʱ
	 			fRangeEnd[CurrentChannelNo] =
	 				float((ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3 + DisplaySet_ad[CurrentChannelNo].fRange)*sin(fAngle) - SendParam_ad[CurrentChannelNo].fQianYan);  // ��ʱ+����
	 		}
		}
		else
		{
	 		fRangeStart[CurrentChannelNo] = 
	 			float((ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3)*sin(fAngle) - SendParam_ad[CurrentChannelNo].fQianYan); // ��ʱ
	 		fRangeEnd[CurrentChannelNo] =
	 			float((ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3 + DisplaySet_ad[CurrentChannelNo].fRange)*sin(fAngle) - SendParam_ad[CurrentChannelNo].fQianYan);  // ��ʱ+����
		}
	}
	else
	{
		if(gwDeviceType == 0)
	 	{
			if(!DisplaySet_ad[CurrentChannelNo].bSFlag)
			{
				fRangeStart[0] = 
					float((ReceiveParam_ad[CurrentChannelNo].fDelay / 2 * DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3)*sin(fAngle) - SendParam_ad[CurrentChannelNo].fQianYan); // ��ʱ
				fRangeEnd[0] = 
					float((ReceiveParam_ad[CurrentChannelNo].fDelay / 2 * DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3 + DisplaySet_ad[CurrentChannelNo].fRange)*sin(fAngle) - SendParam_ad[CurrentChannelNo].fQianYan);  // ��ʱ+����

				fRangeStart[1] = fRangeStart[0];
				fRangeEnd[1] = fRangeEnd[0];
			}
		}
		else
		{
			fRangeStart[0] = 
				float((ReceiveParam_ad[CurrentChannelNo].fDelay / 2 * DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3)*sin(fAngle) - SendParam_ad[CurrentChannelNo].fQianYan); // ��ʱ
			fRangeEnd[0] = 
				float((ReceiveParam_ad[CurrentChannelNo].fDelay / 2 * DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3 + DisplaySet_ad[CurrentChannelNo].fRange)*sin(fAngle) - SendParam_ad[CurrentChannelNo].fQianYan);  // ��ʱ+����

			fRangeStart[1] = fRangeStart[0];
			fRangeEnd[1] = fRangeEnd[0];
		}

		SendParam_ad[0].fQianYan = SendParam_ad[CurrentChannelNo].fQianYan;
		SendParam_ad[1].fQianYan = SendParam_ad[CurrentChannelNo].fQianYan;
	}

	LeaveCriticalSection(&csRange);
	DeleteCriticalSection(&csRange);
	
}

LRESULT CALLBACK btnSendParamHeadQianYanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			fAmendStep = AMENDSTEPS[nAmendStepIndex];
			break;
		case WM_PAINT:
			// songchenguang add start 2010-04-21
			wchar_t strText[20];

			memset(strText, 0, sizeof(strText));
			swprintf(strText, _T("%.2f"), SendParam_ad[CurrentChannelNo].nAmend);
			//SetWindowText(hLableModify, strText);
			DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
			// songchenguang add end 2010-04-21
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
					while(sendParamBtnList[i] != currentSendParamBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSendParamBtn = sendParamBtnList[i];
					SetFocus(currentSendParamBtn);
					gCurrentFocusBtn = currentSendParamBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(sendParamBtnList[i] != currentSendParamBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSendParamBtn = sendParamBtnList[i];
					SetFocus(currentSendParamBtn);
					gCurrentFocusBtn = currentSendParamBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(wTanTouLeiXing[CurrentChannelNo] == 0)
					{
						if(SendParam_ad[CurrentChannelNo].fQianYan > fAmendStep)
							SendParam_ad[CurrentChannelNo].fQianYan -= fAmendStep;
						else
							SendParam_ad[CurrentChannelNo].fQianYan = 0.0f;

						SetObliqueProbeQianYan(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(wTanTouLeiXing[CurrentChannelNo] == 0)
					{
						if((SendParam_ad[CurrentChannelNo].fQianYan + fAmendStep) < 100)
							SendParam_ad[CurrentChannelNo].fQianYan += fAmendStep;
						else
							SendParam_ad[CurrentChannelNo].fQianYan = 100;

						SetObliqueProbeQianYan(Wnd);
					}
				}
				else if(wParam == 'F') // ����
				{
					if(nAmendStepIndex < 2)
						nAmendStepIndex++;
					else
						nAmendStepIndex = 0;

					fAmendStep = AMENDSTEPS[nAmendStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fAmendStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == 'D')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnSendParamHeadQianYanProc,Wnd,Msg,wParam,lParam);
}

