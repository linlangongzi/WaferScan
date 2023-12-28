/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ��ʾ����Դ�ļ�
 * Filename: displayWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "displayWin.h"
#include "interfaceCmdID.h"

#include "resource.h"
#include "recvParamWin.h"
#include "sendParamWin.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "const.h"
#include "win_com.h"
#include "craftParamWin.h"
#include "gateWin.h"

HWND hWndDisplay;									//��ʾ���ھ��
HWND hBtnDisplayRefractMode;						//�����ģʽ
HWND hBtnDisplaySoundDistance;						//���̰�ť���
HWND hBtnDisplaySoundSpeed;							//���ٰ�ť���
HWND hBtnDisplayRefractAngle;						//����ǰ�ť���
HWND hBtnDisplayRestraint;							//���ư�ť���
HWND displayBtnList[5];								//��ʾ���ڵİ�ť��
WNDPROC OldBtnDisplayRefractModeProc = NULL;		//���̰�ťĬ�ϴ������
WNDPROC OldBtnDisplaySoundDistanceProc = NULL;		//���̰�ťĬ�ϴ������
WNDPROC OldBtnDisplaySoundSpeedProc = NULL;			//���ٰ�ťĬ�ϴ������
WNDPROC OldBtnDisplayRefractAngleProc = NULL;		//����ǰ�ťĬ�ϴ������
WNDPROC OldBtnDisplayRestraintProc = NULL;			//���ư�ťĬ�ϴ������
HWND currentDisplayBtn;								//��ʾ���е�ǰѡ�еİ�ť

static  int buttonNum;						//��ť�ؼ���
//static HBRUSH hStaticBrush = NULL;				//��ˢ
extern HWND hWndParam;								//��ʾ���ھ��
extern HWND currentParamBtn;						//��ʾ���е�ǰѡ�еİ�ť
extern HWND hWndWaveSpeed;							//���ٴ��ھ��
extern HWND hWndRefractAngle;						//���ٴ��ھ��
extern HINSTANCE hInst;								//����ʵ�����
extern HWND gCurrentWnd;							//��ǰ��ʾ���Ӵ���

// songchenguang add start 2010-04-22
extern HWND hStatusBar;
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
//extern SEND_PARAM_DATA SendParam_ad[2];
extern float gCh1Range;
extern float gCh2Range;
extern WORD CurrentChannelNo;
extern float fRangeStart[2];
extern float fRangeEnd[2];
extern WORD *chCurrent_interval;
extern WORD *ch1_interval;
extern WORD *ch2_interval;
extern WORD WaveSpeed;
extern ARS200FPGA_CH_CON_REG *chCurrent_con_reg;
extern ARS200FPGA_CH_CON_REG *ch1_con_reg;
extern ARS200FPGA_CH_CON_REG *ch2_con_reg;
extern CRITICAL_SECTION csRange;
extern LOGFONT glogFont;
extern wchar_t gszTextStep[10];
extern HWND hWndMain;
extern BOOL gbGuanLian;
extern HWND gCurrentFocusBtn;
//extern GATE_INFO GateInfos[2][3];
//extern CRITICAL_SECTION csGate;
//extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
//extern ARS200FPGA_CH_GATE_REG *ch2_gate_reg;
//extern ARS200FPGA_CH_DATA_REG *ch1_data_reg;
//extern ARS200FPGA_CH_DATA_REG *ch2_data_reg;
extern WORD *ch1_gate1curve;
extern WORD *ch2_gate1curve;
extern WORD *ch1_gate2curve;
extern WORD *ch2_gate2curve;
extern WORD *ch1_gate3curve;
extern WORD *ch2_gate3curve;
//extern CURVEPOINT_DATA gSelectedDacPointData[CHANNEL_NUM][MAX_CURVE_POINT_NUM];//tcg
//extern DWORD gSelectedDacPointDataNum[CHANNEL_NUM];//tcg

extern CRITICAL_SECTION csRefWave;
extern BOOL g_bDisplayRefWave;
extern SEND_PARAM_DATA SendParam_ad[2];

DISPLAY_SET_DLG_DATA DisplaySet_ad[2];

float fRangeStep = 1;
//static WORD nWaveSpeedStep = 100;
//float fZheSheAngleStep = 1;
const WORD nBateStep = 1;
static BOOL bSFlag = TRUE;

WORD nRangeStepIndex = 0;				// ��ǰ���沽�����
//static WORD nWaveSpeedStepIndex = 0;			// ��ǰ��ʱ�������
//static WORD nZheSheAngleStepIndex = 0;

extern SYS_INFO gSysInfo;
extern CRITICAL_SECTION csSysInfo;
//extern float gfMinRange[2];
//extern CRITICAL_SECTION csMinRange;
// songchenguang add end 2010-04-22
extern WORD gwDeviceType;	//�豸���� 0:̽����	1:�����
extern HWND hWndChParam;			//���������ھ��
extern HWND currentChParamBtn;		//���������е�ǰѡ�еİ�ť
extern BOOL gDacChangeFlag; //DAC���Ƿ���Ҫ�ı�
extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];//��ѡ�е�DAC������
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];		//DACͨ������
extern float gfOldDacRange[CHANNEL_NUM];				//�������DACʱ������
extern short gwOldDacSerial[GATE_NUM][MAX_CURVE_POINT_NUM];	//�������DACʱ��ÿ�����λ��
extern wchar_t gszTextPeak[20];
extern BOOL gbHotDB;			// ��ݼ�����
extern BOOL gbHotRange;			// ��ݼ�����
extern wchar_t lpDefaultParamFile[MAX_PATH];

extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG���߿��ر�־
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//��ѡ�е�AVG������
extern BOOL bgAvgChangeFlag;
extern float gfOldAvgRange[CHANNEL_NUM];				//�������AVGʱ������
extern short gwOldAvgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������AVGʱ��ÿ�����λ��

extern float gfTlRange[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];			//����׼����λ����
extern WORD gwStandard[CHANNEL_NUM];	//����׼
extern WORD gwPosition[CHANNEL_NUM][TL_STANDARD_NUM];	//��ⲿλ

extern WORD *cscan_ch1_interval;
extern WORD *cscan_ch2_interval;

/**************************
*ע����ʾ����
* @display    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerDisplayChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) displayChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szDisplayChild;

	return RegisterClass(&wc);
}

void UpdateDisplayMenuInfo(const WORD& ChNo)
{
	wchar_t strText[20];

	// �Ƕ�ģʽ
	memset(strText, 0, sizeof(strText));

	if(DisplaySet_ad[ChNo].bSFlag)
	{
		swprintf(strText, _T("S"));
	}
	else
	{
		swprintf(strText, _T("X"));
	}
	DrawLableText(hBtnDisplayRefractMode, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

	// ����
	memset(strText, 0, sizeof(strText));

	swprintf(strText, _T("%.1f"), DisplaySet_ad[ChNo].fRange);
	DrawLableText(hBtnDisplaySoundDistance, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

	// ����
//	memset(strText, 0, sizeof(strText));
//	swprintf(strText, _T("%d"), DisplaySet_ad[ChNo].nWaveSpeed);
//	DrawLableText(hBtnDisplaySoundSpeed, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	// �Ƕ�
//	memset(strText, 0, sizeof(strText));
//	swprintf(strText, _T("%.1f"), DisplaySet_ad[ChNo].fZheSheJiao);
//	DrawLableText(hBtnDisplayRefractAngle, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

	// ����
	memset(strText, 0, sizeof(strText));
	swprintf(strText, _T("%d"), DisplaySet_ad[ChNo].nBate);
	DrawLableText(hBtnDisplayRestraint, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}
/**************************
*��ʾ���ڴ������
* @display    hInstance[in]:
* @display		message[in]:
* @display		wDisplay[in]:
* @display		lDisplay[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK displayChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			//if(hStaticBrush == NULL)	
			//	hStaticBrush = CreateSolidBrush(BK_COLOR_WHITE);
			//����ť�ؼ�
			if(gwDeviceType == 0 || gwDeviceType == 2) //̽�˻���·
			{
				buttonNum = 5;
				
				hBtnDisplayRefractMode = CreateWindow(_T("BUTTON"),_T("refractMode"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_DISPLAY_REFRACTMODE,hInst,NULL);
				hBtnDisplaySoundDistance = CreateWindow(_T("BUTTON"),_T("soundDistance"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_DISPLAY_SOUNDDISTANCE,hInst,NULL);
				hBtnDisplaySoundSpeed = CreateWindow(_T("BUTTON"),_T("soundSpeed"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_DISPLAY_SOUNDSPEED,hInst,NULL);
				hBtnDisplayRefractAngle = CreateWindow(_T("BUTTON"),_T("refractAngle"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 +3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_DISPLAY_REFRACTANGLE,hInst,NULL);
				hBtnDisplayRestraint = CreateWindow(_T("BUTTON"),_T("restraint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_DISPLAY_RESTRAINT,hInst,NULL);
						
				displayBtnList[0] = hBtnDisplayRefractMode;
				displayBtnList[1] = hBtnDisplaySoundDistance;
				displayBtnList[2] = hBtnDisplaySoundSpeed;
				displayBtnList[3] = hBtnDisplayRefractAngle;
				displayBtnList[4] = hBtnDisplayRestraint;
				//��ť�ؼ����Զ��崦�����
				OldBtnDisplayRefractModeProc = (WNDPROC) GetWindowLong(hBtnDisplayRefractMode,GWL_WNDPROC);
				SetWindowLong(hBtnDisplayRefractMode,GWL_WNDPROC,(LONG)btnDisplayRefractModeProc);
				OldBtnDisplaySoundDistanceProc = (WNDPROC) GetWindowLong(hBtnDisplaySoundDistance,GWL_WNDPROC);
				SetWindowLong(hBtnDisplaySoundDistance,GWL_WNDPROC,(LONG)btnDisplaySoundDistanceProc);
				OldBtnDisplaySoundSpeedProc = (WNDPROC) GetWindowLong(hBtnDisplaySoundSpeed,GWL_WNDPROC);
				SetWindowLong(hBtnDisplaySoundSpeed,GWL_WNDPROC,(LONG)btnDisplaySoundSpeedProc);
				OldBtnDisplayRefractAngleProc = (WNDPROC) GetWindowLong(hBtnDisplayRefractAngle,GWL_WNDPROC);
				SetWindowLong(hBtnDisplayRefractAngle,GWL_WNDPROC,(LONG)btnDisplayRefractAngleProc);
				OldBtnDisplayRestraintProc = (WNDPROC) GetWindowLong(hBtnDisplayRestraint,GWL_WNDPROC);
				SetWindowLong(hBtnDisplayRestraint,GWL_WNDPROC,(LONG)btnDisplayRestraintProc);

				currentDisplayBtn = displayBtnList[0];//Ĭ�ϵ�ǰѡ�������ģʽ��ť
			}
			else if(gwDeviceType == 1)//���
			{
				buttonNum = 2;
				
				hBtnDisplaySoundDistance = CreateWindow(_T("BUTTON"),_T("soundDistance"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_DISPLAY_SOUNDDISTANCE,hInst,NULL);

			//	hBtnDisplaySoundSpeed = CreateWindow(_T("BUTTON"),_T("soundSpeed"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
			//			0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_DISPLAY_SOUNDSPEED,hInst,NULL);

				hBtnDisplayRestraint = CreateWindow(_T("BUTTON"),_T("restraint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_DISPLAY_RESTRAINT,hInst,NULL);
						
				displayBtnList[0] = hBtnDisplaySoundDistance;
			//	displayBtnList[1] = hBtnDisplaySoundSpeed;
				displayBtnList[1] = hBtnDisplayRestraint;
			
				//��ť�ؼ����Զ��崦�����
				OldBtnDisplaySoundDistanceProc = (WNDPROC) GetWindowLong(hBtnDisplaySoundDistance,GWL_WNDPROC);
				SetWindowLong(hBtnDisplaySoundDistance,GWL_WNDPROC,(LONG)btnDisplaySoundDistanceProc);
				
			//	OldBtnDisplaySoundSpeedProc = (WNDPROC) GetWindowLong(hBtnDisplaySoundSpeed,GWL_WNDPROC);
			//	SetWindowLong(hBtnDisplaySoundSpeed,GWL_WNDPROC,(LONG)btnDisplaySoundSpeedProc);

				OldBtnDisplayRestraintProc = (WNDPROC) GetWindowLong(hBtnDisplayRestraint,GWL_WNDPROC);
				SetWindowLong(hBtnDisplayRestraint,GWL_WNDPROC,(LONG)btnDisplayRestraintProc);

				currentDisplayBtn = displayBtnList[0];//Ĭ�ϵ�ǰѡ�����ٰ�ť
			}
			SetFocus(currentDisplayBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentDisplayBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndDisplay, &ps);
			
			EndPaint(hWndDisplay, &ps);
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
				
				DestroyWindow(hWndDisplay);
				hWndDisplay = 0;
			}
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			display_DrawButtonBitmap(hInst,pdis);
			
			wchar_t strText[20];
			if(pdis->hwndItem == hBtnDisplayRefractMode)
			{				
				memset(strText, 0, sizeof(strText));

				if(DisplaySet_ad[CurrentChannelNo].bSFlag)
				{
					swprintf(strText, _T("S"));
				}
				else
				{
					swprintf(strText, _T("X"));
				}
				DrawLableText(hBtnDisplayRefractMode, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
			}
			else if(pdis->hwndItem == hBtnDisplaySoundDistance)
			{
				memset(strText, 0, sizeof(strText));
				swprintf(strText, _T("%.1f"), DisplaySet_ad[CurrentChannelNo].fRange);
				
				DrawLableText(hBtnDisplaySoundDistance, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
			}
		/*	else if(pdis->hwndItem == hBtnDisplaySoundSpeed)
			{
				memset(strText, 0, sizeof(strText));
				swprintf(strText, _T("%d"), DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
				DrawLableText(hBtnDisplaySoundSpeed, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
			}
			else if(pdis->hwndItem == hBtnDisplayRefractAngle)
			{
				memset(strText, 0, sizeof(strText));
				swprintf(strText, _T("%.1f"), DisplaySet_ad[CurrentChannelNo].fZheSheJiao);
				DrawLableText(hBtnDisplayRefractAngle, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
			}
			*/
			else if(pdis->hwndItem == hBtnDisplayRestraint)
			{
				memset(strText, 0, sizeof(strText));
				swprintf(strText, _T("%d"), DisplaySet_ad[CurrentChannelNo].nBate);
				DrawLableText(hBtnDisplayRestraint, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
			}

			// ��ⰴť�������ʾ��Ӧ�Ĳ���
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentDisplayBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnDisplaySoundDistance)
						swprintf(gszTextStep, _T("%.1f"), fRangeStep);
				//	else if(pdis->hwndItem == hBtnDisplaySoundSpeed)
				//		swprintf(gszTextStep, _T("%d"), nWaveSpeedStep);
				//	else if(pdis->hwndItem == hBtnDisplayRefractAngle)
				//		swprintf(gszTextStep, _T("%.1f"), fZheSheAngleStep);
					else if(pdis->hwndItem == hBtnDisplayRestraint)
						swprintf(gszTextStep, _T("%d"), nBateStep);
					else
						swprintf(gszTextStep, _T(""));

					PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}
			
			break;
		case WM_CLOSE:
		
			break;
		case WM_CHAR:			
			InitializeCriticalSection(&csSysInfo);
			EnterCriticalSection(&csSysInfo);
			if( wParam == '\t' && gSysInfo.wWorkMode != OneSendOneReceive)
			{
				// ���°�ť������
				if(CurrentChannelNo == 0)
				{
					UpdateDisplayMenuInfo(1);
				}
				else
				{
					UpdateDisplayMenuInfo(0);
				}
			}

			LeaveCriticalSection(&csSysInfo);
			DeleteCriticalSection(&csSysInfo);

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
*������ʾ���ڰ�ťͼƬ
* @display    hInstance[in]:
* @display		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int display_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	if(gwDeviceType == 0 || gwDeviceType == 2) //̽�˻���·
	{
		const BTN_BMP_INFO display_displayBtnBitmap[]=
		{
			{IDC_DISPLAY_REFRACTMODE	,IDB_BITMAP_SXZHUANHUAN_NOSEL_R	 ,  IDB_BITMAP_SXZHUANHUAN_SELECT_R		},
			{IDC_DISPLAY_SOUNDDISTANCE	,IDB_BITMAP_SHENGCHENG_NOSEL_R	 ,  IDB_BITMAP_SHENGCHENG_SELECT_R		},
			{IDC_DISPLAY_SOUNDSPEED	,IDB_BITMAP_SHENGSU_NOSEL_R	 ,  IDB_BITMAP_SHENGSU_SELECT_R		},		
			{IDC_DISPLAY_REFRACTANGLE	,IDB_BITMAP_ZHESHEJIAO_NOSEL_R	 ,  IDB_BITMAP_ZHESHEJIAO_SELECT_R		},
			{IDC_DISPLAY_RESTRAINT	,IDB_BITMAP_YIZI_NOSEL_R ,  IDB_BITMAP_YIZI_SELECT_R	},
			{	-1			,	-1					 ,	-1							}    
		} ;
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,display_displayBtnBitmap);
	}
	
	else if(gwDeviceType == 1)//���
	{
		const BTN_BMP_INFO display_displayBtnBitmap[]=
		{
			{IDC_DISPLAY_SOUNDDISTANCE	,IDB_BITMAP_SHENGCHENG_NOSEL_R	 ,  IDB_BITMAP_SHENGCHENG_SELECT_R		},
			{IDC_DISPLAY_SOUNDSPEED	,IDB_BITMAP_SHENGSU_NOSEL_R	 ,  IDB_BITMAP_SHENGSU_SELECT_R		},
			{IDC_DISPLAY_RESTRAINT	,IDB_BITMAP_YIZI_NOSEL_R ,  IDB_BITMAP_YIZI_SELECT_R	},
			{	-1			,	-1					 ,	-1							}    
		} ;
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,display_displayBtnBitmap);
	}

	return ReturnVal;
}

void SetAngleMode(HWND hWnd, BOOL bFlag)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	float fAngle = float(DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI);
	InitializeCriticalSection(&csRange);
	EnterCriticalSection(&csRange);

	if(!gbGuanLian)
	{
		if(bFlag)
		{
			swprintf(strText, _T("S"));
			if(sin(fAngle) > 0)
			{
				fRangeStart[CurrentChannelNo] = (fRangeStart[CurrentChannelNo] + SendParam_ad[CurrentChannelNo].fQianYan) / float(sin(fAngle));
				fRangeEnd[CurrentChannelNo] = (fRangeEnd[CurrentChannelNo] + SendParam_ad[CurrentChannelNo].fQianYan) / float(sin(fAngle));
			}
			else	//�����Ϊ��ʱ�����̲����任
			{
				;//
			}
		}
		else
		{
			swprintf(strText, _T("X"));
			if(sin(fAngle) > 0)
			{
				fRangeStart[CurrentChannelNo] = fRangeStart[CurrentChannelNo] * float(sin(fAngle)) - SendParam_ad[CurrentChannelNo].fQianYan;
				fRangeEnd[CurrentChannelNo] = fRangeEnd[CurrentChannelNo] * float(sin(fAngle)) - SendParam_ad[CurrentChannelNo].fQianYan;
			}
			else	//�����Ϊ��ʱ�����̲����任
			{
				;//
			}
		}
	}
	else
	{
		if(bFlag)
		{
			swprintf(strText, _T("S"));

			if(sin(fAngle) > 0)
			{
				fRangeStart[0] = (fRangeStart[CurrentChannelNo] + SendParam_ad[CurrentChannelNo].fQianYan) / float(sin(fAngle));
				fRangeEnd[0] = (fRangeEnd[CurrentChannelNo] + SendParam_ad[CurrentChannelNo].fQianYan) / float(sin(fAngle));
			}
			else	//�����Ϊ��ʱ�����̲����任
			{
				;//
			}

			fRangeStart[1] = fRangeStart[0];
			fRangeEnd[1] = fRangeEnd[0];
		}
		else
		{
			swprintf(strText, _T("X"));
			if(sin(fAngle) > 0)
			{
				fRangeStart[0] = fRangeStart[CurrentChannelNo] * float(sin(fAngle)) - SendParam_ad[CurrentChannelNo].fQianYan;
				fRangeEnd[0] = fRangeEnd[CurrentChannelNo] * float(sin(fAngle)) - SendParam_ad[CurrentChannelNo].fQianYan;

				fRangeStart[1] = fRangeStart[CurrentChannelNo] * float(sin(fAngle)) - SendParam_ad[CurrentChannelNo].fQianYan;
				fRangeEnd[1] = fRangeEnd[CurrentChannelNo] * float(sin(fAngle)) - SendParam_ad[CurrentChannelNo].fQianYan;
			}
			else	//�����Ϊ��ʱ�����̲����任
			{
				;//
			}
		}

		DisplaySet_ad[0].bSFlag = bFlag;
		DisplaySet_ad[1].bSFlag = bFlag;
	}

	LeaveCriticalSection(&csRange);
	DeleteCriticalSection(&csRange);

	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);	

	// �������õ�Ĭ���ļ�
	SaveParamFile(lpDefaultParamFile);
}
/**************************
*�����ģʽ��ť�Զ��崦�����
* @display    hInstance[in]:
* @display		message[in]:
* @display		wDisplay[in]:
* @display		lDisplay[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnDisplayRefractModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_PAINT:
			wchar_t strText[20];
			memset(strText, 0, sizeof(strText));

			if(DisplaySet_ad[CurrentChannelNo].bSFlag)
			{
				swprintf(strText, _T("S"));
			}
			else
			{
				swprintf(strText, _T("X"));
			}
			DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
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
				while(displayBtnList[i] != currentDisplayBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentDisplayBtn = displayBtnList[i];
				SetFocus(currentDisplayBtn);
				gCurrentFocusBtn = currentDisplayBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(displayBtnList[i] != currentDisplayBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentDisplayBtn = displayBtnList[i];
				SetFocus(currentDisplayBtn);
				gCurrentFocusBtn = currentDisplayBtn;
			}
			else if(wParam == VK_RETURN)
			{
			}
			else if(wParam == VK_LEFT || wParam == VK_RIGHT)
			{
				DisplaySet_ad[CurrentChannelNo].bSFlag = !DisplaySet_ad[CurrentChannelNo].bSFlag;

				SetAngleMode(Wnd,DisplaySet_ad[CurrentChannelNo].bSFlag);
			}
			else if(wParam == '\t')
			{
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
	//	SetBkMode((HDC)wParam,TRANSPARENT);
	//	SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//	return (LRESULT)hStaticBrush;
	default:
		break;
	}
	return CallWindowProc(OldBtnDisplayRefractModeProc,Wnd,Msg,wParam,lParam);
}

void SetRangeLableAndExecute(HWND hWnd, const float& fRange, const BOOL& bSave)
{
	// ����
	if(hWnd)
	{
		wchar_t strText[20];

		memset(strText, 0, sizeof(strText));
		swprintf(strText, _T("%.1f"), fRange);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	}
	// ��ֵ
	WORD wInterval[DATA_SIZE];
	memset(wInterval, 0, sizeof(wInterval));

//	float fRange = DisplaySet_ad[CurrentChannelNo].fRange;
	RangeProcess(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fRange, wInterval);
	float fAngle = float(DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI);

	if(!gbGuanLian)
	{
		//memcpy(chCurrent_interval, wInterval, sizeof(wInterval));
		if(CurrentChannelNo == 0)
		{
			memcpy(cscan_ch1_interval, wInterval, sizeof(wInterval));
		}
		else if(CurrentChannelNo == 1)
		{
			memcpy(cscan_ch2_interval, wInterval, sizeof(wInterval));
		}
		
		InitializeCriticalSection(&csRange);
		EnterCriticalSection(&csRange);

		if(DisplaySet_ad[CurrentChannelNo].bSFlag)
		{
			fRangeStart[CurrentChannelNo] = float(ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3);
			fRangeEnd[CurrentChannelNo] = fRangeStart[CurrentChannelNo] + fRange;
		}
		else
		{
			if(sin(fAngle) > 0)//�����ΪOʱ��X�����̲���
			{
				fRangeStart[CurrentChannelNo] = float(ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3 * sin(fAngle) - 
					SendParam_ad[CurrentChannelNo].fQianYan);
				fRangeEnd[CurrentChannelNo] = float((ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3 + fRange) * sin(fAngle) -
					SendParam_ad[CurrentChannelNo].fQianYan);
			}
			else
			{
				fRangeStart[CurrentChannelNo] = float(ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3);
				fRangeEnd[CurrentChannelNo] = fRangeStart[CurrentChannelNo] + fRange;
			}
		}

		LeaveCriticalSection(&csRange);
		DeleteCriticalSection(&csRange);

		if(CurrentChannelNo == 0)
		{
			gCh1Range = fRange;
		}
		else if(CurrentChannelNo == 1)
		{
			gCh2Range = fRange;
		}
	}
	else
	{
		memcpy(cscan_ch1_interval, wInterval, sizeof(wInterval));
		memcpy(cscan_ch2_interval, wInterval, sizeof(wInterval));
		
		InitializeCriticalSection(&csRange);
		EnterCriticalSection(&csRange);

		if(DisplaySet_ad[CurrentChannelNo].bSFlag)
		{
			fRangeStart[0] = float(ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3);
			fRangeEnd[0] = fRangeStart[CurrentChannelNo] + fRange;

			fRangeStart[1] = fRangeStart[0];	//ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3;
			fRangeEnd[1] = fRangeEnd[0];	//fRangeStart[CurrentChannelNo] + fRange;
		}
		else
		{
			if(sin(fAngle) > 0)//�����ΪOʱ��X�����̲���
			{
				fRangeStart[0] = float(ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3 * sin(fAngle) - 
					SendParam_ad[CurrentChannelNo].fQianYan);
				fRangeEnd[0] = float((ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3 + fRange) * sin(fAngle) - 
					SendParam_ad[CurrentChannelNo].fQianYan);

				fRangeStart[1] = fRangeStart[0];	//ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3 * cos(fAngle);
				fRangeEnd[1] = fRangeEnd[0];	//(fRangeStart[CurrentChannelNo] + fRange) * cos(fAngle);
			}
			else
			{
				fRangeStart[0] = float(ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3);
				fRangeEnd[0] = fRangeStart[CurrentChannelNo] + fRange;

				fRangeStart[1] = fRangeStart[0];	//ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3;
				fRangeEnd[1] = fRangeEnd[0];	//fRangeStart[CurrentChannelNo] + fRange;
			}
		}

		LeaveCriticalSection(&csRange);
		DeleteCriticalSection(&csRange);

		gCh1Range = fRange;		
		gCh2Range = fRange;

		DisplaySet_ad[0].fRange = DisplaySet_ad[CurrentChannelNo].fRange;
		DisplaySet_ad[1].fRange = DisplaySet_ad[CurrentChannelNo].fRange;		
	}

	InitializeCriticalSection(&csRefWave);
	EnterCriticalSection(&csRefWave);
	
	BOOL bDisplayRefWave = g_bDisplayRefWave;
	LeaveCriticalSection(&csRefWave);
	DeleteCriticalSection(&csRefWave);

	// �޸��ظ�Ƶ��
	SetReFreqLableAndExecute(NULL);

	if(bDisplayRefWave)
		SetRedrawFlag(TRUE);

	if(gwDeviceType == 2)//��·ר��ʱ������������ο�����һ��  wangbingfu add at 2011-05-27
	{
		gfTlRange[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]] = DisplaySet_ad[CurrentChannelNo].fRange;
	}
	// �������õ�Ĭ���ļ�
	if(bSave)
		SaveParamFile(lpDefaultParamFile);

	SetRedrawFlag(TRUE);
}

/**************************
*���̰�ť�Զ��崦�����
* @display    hInstance[in]:
* @display		message[in]:
* @display		wDisplay[in]:
* @display		lDisplay[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnDisplaySoundDistanceProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_CREATE:
		fRangeStep = AMENDSTEPS[nRangeStepIndex];
		break;
	case WM_PAINT:
		// songchenguang add start 2010-04-22
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		swprintf(strText, _T("%.1f"), DisplaySet_ad[CurrentChannelNo].fRange);
		DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
		//SetWindowText(hLableSoundDistance, strText);			
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
				while(displayBtnList[i] != currentDisplayBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentDisplayBtn = displayBtnList[i];
				SetFocus(currentDisplayBtn);
				gCurrentFocusBtn = currentDisplayBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(displayBtnList[i] != currentDisplayBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentDisplayBtn = displayBtnList[i];
				SetFocus(currentDisplayBtn);
				gCurrentFocusBtn = currentDisplayBtn;
			}
			else if(wParam == VK_RETURN)
			{
			}
			else if(wParam == VK_LEFT)
			{
				float fOldRange = DisplaySet_ad[CurrentChannelNo].fRange;
				
				// songchenguang add start 2010-04-22
				if(DisplaySet_ad[CurrentChannelNo].fRange > 1 && 
					DisplaySet_ad[CurrentChannelNo].fRange > fRangeStep)
					DisplaySet_ad[CurrentChannelNo].fRange -= fRangeStep;
				else
					DisplaySet_ad[CurrentChannelNo].fRange = 1;

				SetRangeLableAndExecute(Wnd, DisplaySet_ad[CurrentChannelNo].fRange, TRUE);
				// songchenguang add end 2010-04-22
				rangeChangeCurve(fOldRange, TRUE);
			}
			else if(wParam == VK_RIGHT)
			{
				float fOldRange = DisplaySet_ad[CurrentChannelNo].fRange;
				
				// songchenguang add start 2010-04-22
				if(DisplaySet_ad[CurrentChannelNo].fRange < MAX_RANGE)
					DisplaySet_ad[CurrentChannelNo].fRange += fRangeStep;
				else
					DisplaySet_ad[CurrentChannelNo].fRange = MAX_RANGE;

				SetRangeLableAndExecute(Wnd, DisplaySet_ad[CurrentChannelNo].fRange, TRUE);
				// songchenguang add end 2010-04-22

				rangeChangeCurve(fOldRange, TRUE);		
			}
			else if(wParam == 'F')
			{
				if(nRangeStepIndex < 3)
					nRangeStepIndex++;
				else
					nRangeStepIndex = 0;

				fRangeStep = AMENDSTEPS[nRangeStepIndex];

				memset(gszTextStep, 0, sizeof(gszTextStep));
				swprintf(gszTextStep, _T("%.1f"), fRangeStep);
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
	return CallWindowProc(OldBtnDisplaySoundDistanceProc,Wnd,Msg,wParam,lParam);
}
#if 0
void ChangeWaveSpeed(const WORD& chNo)
{
	// ����
	SetRangeLableAndExecute(NULL, DisplaySet_ad[chNo].fRange);

	// բ��
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	if(chNo == 0)
	{
		// ǰ��
		ch1_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][0].fFrontGate));
		ch1_gate_reg->wGate2Front = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][1].fFrontGate));
		ch1_gate_reg->wGate3Front = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][2].fFrontGate));
		// ����
		ch1_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][0].fBackGate));
		ch1_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][1].fBackGate));
		ch1_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][2].fBackGate));
		// ����
		WORD nFront = (WORD)Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][0].fFrontGate);
		WORD nBack = (WORD)Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][0].fBackGate);
		WORD nRange = nBack - nFront;

		//SetGateCurve(ch1_gate1curve, nRange, GateInfos[chNo][0].fPeakGate);
		ch1_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * (GateInfos[chNo][0].fPeakGate) / 100.0f);

		nFront = (WORD)Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][1].fFrontGate);
		nBack = (WORD)Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][1].fBackGate);
		nRange = nBack - nFront;

		//SetGateCurve(ch1_gate2curve, nRange, GateInfos[chNo][1].fPeakGate);
		ch1_gate_reg->wGate2Peak = WORD(MAX_HEIGHT * (GateInfos[chNo][1].fPeakGate) / 100.0f);
	/*
		nFront = (WORD)Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][2].fFrontGate);
		nBack = (WORD)Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][2].fBackGate);
		nRange = nBack - nFront;

		SetGateCurve(ch1_gate3curve, nRange, GateInfos[chNo][2].fPeakGate);
		*/
	}
	else if(chNo == 1)
	{
		// ǰ��
		ch2_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][0].fFrontGate));
		ch2_gate_reg->wGate2Front = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][1].fFrontGate));
		ch2_gate_reg->wGate3Front = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][2].fFrontGate));
		// ����
		ch2_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][0].fBackGate));
		ch2_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][1].fBackGate));
		ch2_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][2].fBackGate));
		// ����
		WORD nFront = (WORD)Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][0].fFrontGate);
		WORD nBack = (WORD)Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][0].fBackGate);
		WORD nRange = nBack - nFront;

		//SetGateCurve(ch2_gate1curve, nRange, GateInfos[chNo][0].fPeakGate);
		ch2_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * (GateInfos[chNo][0].fPeakGate) / 100.0f);

		nFront = (WORD)Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][1].fFrontGate);
		nBack = (WORD)Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][1].fBackGate);
		nRange = nBack - nFront;

		//SetGateCurve(ch2_gate2curve, nRange, GateInfos[chNo][1].fPeakGate);
		ch2_gate_reg->wGate2Peak = WORD(MAX_HEIGHT * (GateInfos[chNo][1].fPeakGate) / 100.0f);
	/*
		nFront = (WORD)Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][2].fFrontGate);
		nBack = (WORD)Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][2].fBackGate);
		nRange = nBack - nFront;

		SetGateCurve(ch2_gate3curve, nRange, GateInfos[chNo][2].fPeakGate);
		*/
	}
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);

	
	// ���� + ��ʱ
	float fTemp = SendParam_ad[chNo].nAmend + ReceiveParam_ad[chNo].fDelay;
	if(chNo == 0)
		ch1_data_reg->wMonitorZeroCounter = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, fTemp));
	else if(chNo == 1)
	{
		ch2_data_reg->wMonitorZeroCounter = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, fTemp));
	}

	InitializeCriticalSection(&csRange);
	EnterCriticalSection(&csRange);
	float fRange = fRangeEnd[chNo] - fRangeStart[chNo];

	fRangeStart[chNo] = fTemp;
	fRangeEnd[chNo] = fRangeStart[chNo] + fRange;	

	LeaveCriticalSection(&csRange);
	DeleteCriticalSection(&csRange);

	// TCGǰ��ֵ
	getGain2TgcSram(gSelectedDacPointData[chNo], gSelectedDacPointDataNum[chNo], chNo);
	//DAC
	for(int i = 0; i < 3; i++)
	{
		if(gSelectedDgsPointDataNum[CurrentChannelNo][i] >= 2)
		{
			write2GateSram(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], CurrentChannelNo, i);
		}
	}

	// ������С����ֵ
	InitializeCriticalSection(&csMinRange);
	EnterCriticalSection(&csMinRange);

	gfMinRange[chNo] = Dot2Range(DisplaySet_ad[chNo].nWaveSpeed, MONITOR_SIZE);

	LeaveCriticalSection(&csMinRange);
	DeleteCriticalSection(&csMinRange);
}

void SetWaveSpeedLabelAndExecute(HWND hWnd)
{
	// ����
	wchar_t strText[20];

	memset(strText, 0, sizeof(strText));
	swprintf(strText, _T("%d"), DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	
	// ��ֵ(���к�������صĶ�Ҫ����Ӧ�仯)...
	if(gbGuanLian)
	{
		DisplaySet_ad[0].nWaveSpeed = DisplaySet_ad[CurrentChannelNo].nWaveSpeed;
		DisplaySet_ad[1].nWaveSpeed = DisplaySet_ad[CurrentChannelNo].nWaveSpeed;

		for(WORD i=0; i<2; i++)
			ChangeWaveSpeed(i);
	}
	else
	{
		ChangeWaveSpeed(CurrentChannelNo);
	}

	// �������õ�Ĭ���ļ�
	SaveParamFile(lpDefaultParamFile);
}
#endif
/**************************
*���ٰ�ť�Զ��崦�����
* @display    hInstance[in]:
* @display		message[in]:
* @display		wDisplay[in]:
* @display		lDisplay[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnDisplaySoundSpeedProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	/*case WM_CREATE:
		nWaveSpeedStep = WAVESPEEDSTEPS[nWaveSpeedStepIndex];
		break;
		*/
	/*case WM_PAINT:
		// songchenguang add start 2010-04-22
		wchar_t strText[20];

		memset(strText, 0, sizeof(strText));
		swprintf(strText, _T("%d"), DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
		DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
		//SetWindowText(hLableSoundSpeed, strText);
		// songchenguang add end 2010-04-22
		
		break;*/
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
				while(displayBtnList[i] != currentDisplayBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentDisplayBtn = displayBtnList[i];
				SetFocus(currentDisplayBtn);
				gCurrentFocusBtn = currentDisplayBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(displayBtnList[i] != currentDisplayBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentDisplayBtn = displayBtnList[i];
				SetFocus(currentDisplayBtn);
				gCurrentFocusBtn = currentDisplayBtn;
			}
			else if(wParam == VK_RETURN)
			{
				hWndWaveSpeed = CreateWindow(szWaveSpeedChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
				ShowWindow(hWndDisplay,SW_HIDE);
				//SetFocus(hWndWaveSpeed);
				ShowWindow(hWndWaveSpeed,SW_SHOW);
				UpdateWindow(hWndWaveSpeed);

				gCurrentWnd = hWndWaveSpeed;
			}
		/*	else if(wParam == VK_LEFT)
			{
				// songchenguang add start 2010-04-22
				if(DisplaySet_ad[CurrentChannelNo].nWaveSpeed > MIN_WAVE_SPEED && 
					DisplaySet_ad[CurrentChannelNo].nWaveSpeed > nWaveSpeedStep)
				{
					DisplaySet_ad[CurrentChannelNo].nWaveSpeed -= nWaveSpeedStep;
				}
				else
					DisplaySet_ad[CurrentChannelNo].nWaveSpeed = MIN_WAVE_SPEED;

				SetWaveSpeedLabelAndExecute(Wnd);
				// songchenguang add end 2010-04-22
			}
			else if(wParam == VK_RIGHT)
			{
				// songchenguang add start 2010-04-22
				if(DisplaySet_ad[CurrentChannelNo].nWaveSpeed < MAX_WAVE_SPEED)
				{
					DisplaySet_ad[CurrentChannelNo].nWaveSpeed += nWaveSpeedStep;
				}
				else
					DisplaySet_ad[CurrentChannelNo].nWaveSpeed = MAX_WAVE_SPEED;

				SetWaveSpeedLabelAndExecute(Wnd);
				// songchenguang add end 2010-04-22
			}
			else if(wParam == 'F')
			{
				if(nWaveSpeedStepIndex < 2)
					nWaveSpeedStepIndex++;
				else
					nWaveSpeedStepIndex = 0;

				nWaveSpeedStep = WAVESPEEDSTEPS[nWaveSpeedStepIndex];

				memset(gszTextStep, 0, sizeof(gszTextStep));
				swprintf(gszTextStep, _T("%d"), nWaveSpeedStep);
				SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			}
			*/
			else if(wParam == '\t')
			{
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
	return CallWindowProc(OldBtnDisplaySoundSpeedProc,Wnd,Msg,wParam,lParam);
}
/*
void SetAngleLabelAndExecute(HWND hWnd)
{
	wchar_t strText[20];

	memset(strText, 0, sizeof(strText));
	swprintf(strText, _T("%.1f"), DisplaySet_ad[CurrentChannelNo].fZheSheJiao);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

	if(gbGuanLian)
	{
		DisplaySet_ad[0].fZheSheJiao = DisplaySet_ad[CurrentChannelNo].fZheSheJiao;
		DisplaySet_ad[1].fZheSheJiao = DisplaySet_ad[CurrentChannelNo].fZheSheJiao;
	}

	// ���������ֵ������ֵ��Ӧ���仯...
	float fRange = DisplaySet_ad[CurrentChannelNo].fRange;
	float fAngle = float(DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI);
	InitializeCriticalSection(&csRange);
	EnterCriticalSection(&csRange);

	if(!gbGuanLian)
	{
		if(DisplaySet_ad[CurrentChannelNo].bSFlag)
		{
			fRangeStart[CurrentChannelNo] = ReceiveParam_ad[CurrentChannelNo].fDelay;
			fRangeEnd[CurrentChannelNo] = ReceiveParam_ad[CurrentChannelNo].fDelay + fRange;
		}
		else
		{
			fRangeStart[CurrentChannelNo] = float(ReceiveParam_ad[CurrentChannelNo].fDelay * sin(fAngle));
			fRangeEnd[CurrentChannelNo] = float((ReceiveParam_ad[CurrentChannelNo].fDelay + fRange) * sin(fAngle));
		}
	}
	else
	{
		if(DisplaySet_ad[CurrentChannelNo].bSFlag)
		{
			fRangeStart[0] = ReceiveParam_ad[CurrentChannelNo].fDelay;
			fRangeEnd[0] = ReceiveParam_ad[CurrentChannelNo].fDelay + fRange;

			fRangeStart[1] = fRangeStart[0];
			fRangeEnd[1] = fRangeEnd[0];
		}
		else
		{
			fRangeStart[0] = float(ReceiveParam_ad[CurrentChannelNo].fDelay * sin(fAngle));
			fRangeEnd[0] = float((ReceiveParam_ad[CurrentChannelNo].fDelay + fRange) * sin(fAngle));

			fRangeStart[1] = fRangeStart[0];
			fRangeEnd[1] = fRangeEnd[1];
		}

	}

	LeaveCriticalSection(&csRange);
	DeleteCriticalSection(&csRange);

	// �������õ�Ĭ���ļ�
	SaveParamFile(lpDefaultParamFile);
}
*/
/**************************
*����ǰ�ť�Զ��崦�����
* @display    hInstance[in]:
* @display		message[in]:
* @display		wDisplay[in]:
* @display		lDisplay[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnDisplayRefractAngleProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{

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
				while(displayBtnList[i] != currentDisplayBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentDisplayBtn = displayBtnList[i];
				SetFocus(currentDisplayBtn);
				gCurrentFocusBtn = currentDisplayBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(displayBtnList[i] != currentDisplayBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentDisplayBtn = displayBtnList[i];
				SetFocus(currentDisplayBtn);
				gCurrentFocusBtn = currentDisplayBtn;
			}
		/*	else if(wParam == VK_LEFT)
			{
				// songchenguang add start 2010-04-22
				if(DisplaySet_ad[CurrentChannelNo].fZheSheJiao > MIN_ANGLE && 
					DisplaySet_ad[CurrentChannelNo].fZheSheJiao > fZheSheAngleStep)
				{
					DisplaySet_ad[CurrentChannelNo].fZheSheJiao -= fZheSheAngleStep;
				}
				else
					DisplaySet_ad[CurrentChannelNo].fZheSheJiao = MIN_ANGLE;

				SetAngleLabelAndExecute(Wnd);
				// songchenguang add end 2010-04-22
			}
			else if(wParam == VK_RIGHT)
			{
				// songchenguang add start 2010-04-22
				if(DisplaySet_ad[CurrentChannelNo].fZheSheJiao < MAX_ANGLE)
				{
					DisplaySet_ad[CurrentChannelNo].fZheSheJiao += fZheSheAngleStep;
				}
				else
					DisplaySet_ad[CurrentChannelNo].fZheSheJiao = MAX_ANGLE;

				SetAngleLabelAndExecute(Wnd);
				// songchenguang add end 2010-04-22
			}
			else if(wParam == 'F')
			{
				if(nZheSheAngleStepIndex < 1)
					nZheSheAngleStepIndex++;
				else
					nZheSheAngleStepIndex = 0;

				fZheSheAngleStep = WAVESPEEDSTEPS[nZheSheAngleStepIndex];

				memset(gszTextStep, 0, sizeof(gszTextStep));
				swprintf(gszTextStep, _T("%.1f"), fZheSheAngleStep);
				SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			}*/
			else if(wParam == VK_RETURN)
			{
				hWndRefractAngle = CreateWindow(szRefractAngleChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
				ShowWindow(hWndDisplay,SW_HIDE);
				//SetFocus(hWndRefractAngle);
				ShowWindow(hWndRefractAngle,SW_SHOW);
				UpdateWindow(hWndRefractAngle);

				gCurrentWnd = hWndRefractAngle;
			}
			else if(wParam == '\t')
			{
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
	return CallWindowProc(OldBtnDisplayRefractAngleProc,Wnd,Msg,wParam,lParam);
}

void SetBateLabelAndExecute(HWND hWnd)
{
	// ����
	if(hWnd)
	{
		wchar_t strText[20];

		memset(strText, 0, sizeof(strText));
		swprintf(strText, _T("%d"), DisplaySet_ad[CurrentChannelNo].nBate);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	}

	// ��ֵx
	if(!gbGuanLian)
	{
		//yachang.huang delete on 2013-12-10
		//chCurrent_con_reg->wSuspend = WORD((DisplaySet_ad[CurrentChannelNo].nBate/100.0f)*MAX_HEIGHT);
	}
	else
	{
		//yachang.huang delete on 2013-12-10
		//ch1_con_reg->wSuspend = WORD((DisplaySet_ad[CurrentChannelNo].nBate/100.0f)*MAX_HEIGHT);
		//ch2_con_reg->wSuspend = WORD((DisplaySet_ad[CurrentChannelNo].nBate/100.0f)*MAX_HEIGHT);

		DisplaySet_ad[0].nBate = DisplaySet_ad[CurrentChannelNo].nBate;
		DisplaySet_ad[1].nBate = DisplaySet_ad[CurrentChannelNo].nBate;
	}

	// �������õ�Ĭ���ļ�
	SaveParamFile(lpDefaultParamFile);

	memset(gszTextPeak, 0, sizeof(gszTextPeak));

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
}

/**************************
*���ư�ť�Զ��崦�����
* @display    hInstance[in]:
* @display		message[in]:
* @display		wDisplay[in]:
* @display		lDisplay[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnDisplayRestraintProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_PAINT:
		// songchenguang add start 2010-04-22
		wchar_t strText[20];

		memset(strText, 0, sizeof(strText));
		swprintf(strText, _T("%d"), DisplaySet_ad[CurrentChannelNo].nBate);
		DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
		//SetWindowText(hLableRestraint, strText);			
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
				while(displayBtnList[i] != currentDisplayBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentDisplayBtn = displayBtnList[i];
				SetFocus(currentDisplayBtn);
				gCurrentFocusBtn = currentDisplayBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(displayBtnList[i] != currentDisplayBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentDisplayBtn = displayBtnList[i];
				SetFocus(currentDisplayBtn);
				gCurrentFocusBtn = currentDisplayBtn;
			}
			else if(wParam == VK_LEFT)
			{
				// songchenguang add start 2010-04-22
				if(DisplaySet_ad[CurrentChannelNo].nBate > MIN_BATE && 
					DisplaySet_ad[CurrentChannelNo].nBate > nBateStep)
					DisplaySet_ad[CurrentChannelNo].nBate -= nBateStep;
				else
					DisplaySet_ad[CurrentChannelNo].nBate = MIN_BATE;

				SetBateLabelAndExecute(Wnd);
				// songchenguang add end 2010-04-22
			}
			else if(wParam == VK_RIGHT)
			{
				// songchenguang add start 2010-04-22
				if(DisplaySet_ad[CurrentChannelNo].nBate < MAX_BATE)
					DisplaySet_ad[CurrentChannelNo].nBate += nBateStep;
				else
					DisplaySet_ad[CurrentChannelNo].nBate = MAX_BATE;

				SetBateLabelAndExecute(Wnd);
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
	return CallWindowProc(OldBtnDisplayRestraintProc,Wnd,Msg,wParam,lParam);
}
