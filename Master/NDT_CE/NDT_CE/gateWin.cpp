/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 闸门设定窗口源文件
 * Filename: gateWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "gateWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "recvParamWin.h"
#include "sendParamWin.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "craftParamWin.h"

HWND hWndGate = NULL;				// 闸门设定窗口句柄
HWND hBtnGateSelect = NULL;		// 闸门设定选择按钮句柄
HWND hBtnGateAlarmMode = NULL;		// 报警模式选择按钮句柄
HWND hBtnGateSwitch = NULL;		// 开关按钮句柄
HWND hBtnGateQiDian = NULL;		// 起点按钮句柄
HWND hBtnGateKuanDu = NULL;		// 宽度按钮句柄
HWND hBtnGateFuZhi = NULL;			// 幅值按钮句柄
HWND gateBtnList[6] = {NULL,NULL,NULL,NULL,NULL,NULL};		// 闸门设定窗口的按钮表
WNDPROC OldBtnGateSelectProc = NULL;		// 闸门设定选择按钮默认处理过程
WNDPROC OldBtnGateAlarmModeProc = NULL;		// 报警模式选择按钮默认处理过程
WNDPROC OldBtnGateSwitchProc = NULL;		// 开关按钮默认处理过程
WNDPROC OldBtnGateQiDianProc = NULL;		// 起点按钮默认处理过程
WNDPROC OldBtnGateKuanDuProc = NULL;		// 宽度按钮默认处理过程
WNDPROC OldBtnGateFuZhiProc = NULL;			// 幅值按钮默认处理过程
HWND currentGateBtn = NULL;			// 闸门设定窗中当前选中的按钮

static const int buttonNum = 6;		// 按钮控件数
//static HBRUSH hStaticBrush = NULL;	// 画刷
extern HWND hWndGateDisplay;					//闸门窗口句柄
extern HWND currentGateDisplayBtn;				//闸门窗中当前选中的按钮
extern HINSTANCE hInst;				// 程序实例句柄
//const TCHAR szLableTest[] = TEXT("0.4-0.6");
extern HWND gCurrentFocusBtn;
// songchenguang add start 2010-05-12
GATE_INFO GateInfos[2][3];
GATE_INFO stOldGateInfos[2][3];

CRITICAL_SECTION csGate;

WORD GateIndex = 1;	
extern WORD CurrentChannelNo;				
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
extern ARS200FPGA_CH_GATE_REG *ch2_gate_reg;
extern float fRangeStart[2];
extern float fRangeEnd[2];

float fGateStep = 1.0f;
static WORD nGateStepIndex = 0;				// 当前步长序号
const WORD nPeakStep = 1;

extern CRITICAL_SECTION csRange;
extern WORD *chCurrent_gate1curve;
extern WORD *chCurrent_gate2curve;
extern WORD *chCurrent_gate3curve;

extern WORD *ch1_gate1curve;
extern WORD *ch1_gate2curve;
extern WORD *ch1_gate3curve;

extern WORD *ch2_gate1curve;
extern WORD *ch2_gate2curve;
extern WORD *ch2_gate3curve;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern HWND hWndMain;
extern BOOL gbGuanLian;
extern SYS_INFO gSysInfo;
extern CRITICAL_SECTION csSysInfo;
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;
extern BOOL bFreezeUp;

// songchenguang add end 2010-05-12
extern BOOL  gbHotGateFlag;	//是否打开闸门快捷方式
extern HWND gCurrentWnd;		//当前显示的子窗口

extern ARS200FPGA_SYS_REG* sys_reg;
extern ARS200FPGA_CH_CON_REG *ch1_con_reg;
extern ARS200FPGA_CH_CON_REG *ch2_con_reg;
extern BOOL gbHotDB;			// 快捷键增益
extern BOOL gbHotRange;			// 快捷键声程
extern wchar_t gszTextPeak[20];
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern WORD gwDeviceType;


extern CSCAN_ARS200FPGA_CH_GATE_REG *cscan_ch1_gate_reg;
extern CSCAN_ARS200FPGA_CH_GATE_REG *cscan_ch2_gate_reg;

/**************************
*注册闸门设定窗口
* @gate    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerGateChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) gateChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szGateChild;

	return RegisterClass(&wc);
}

void UpdateGateMenuInfo(const WORD& chNo)
{
	// 闸门设定序号
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%d"), GateIndex);

	DrawLableText(hBtnGateSelect, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	// 报警类型
	memset(strText, 0, sizeof(strText));

	if(GateInfos[chNo][GateIndex-1].alarmMode == SuperHigh)
	{
		wsprintf(strText, _T("进波"));	
	}
	else if(GateInfos[chNo][GateIndex-1].alarmMode == LostWave)
	{
		wsprintf(strText, _T("失波"));
	}
	
	DrawLableText(hBtnGateAlarmMode, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	// 闸门设定开关状态
	memset(strText, 0, sizeof(strText));
	if(GateInfos[chNo][GateIndex-1].bOpen)
	{
		wsprintf(strText, _T("开"));	
	}
	else 
	{
		wsprintf(strText, _T("关"));
	}
	
	DrawLableText(hBtnGateSwitch, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	// 闸门设定起点
	memset(strText, 0, sizeof(strText));
					
	wsprintf(strText, _T("%.2f"), GateInfos[chNo][GateIndex-1].fFrontGate);

	DrawLableText(hBtnGateQiDian, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	// 闸门设定宽度
	memset(strText, 0, sizeof(strText));
					
	wsprintf(strText, _T("%.2f"), (GateInfos[chNo][GateIndex-1].fBackGate - GateInfos[chNo][GateIndex-1].fFrontGate));

	DrawLableText(hBtnGateKuanDu, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	// 闸门设定峰沿
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.2f"), GateInfos[chNo][GateIndex-1].fPeakGate);

	DrawLableText(hBtnGateFuZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}
/**************************
*闸门设定窗口处理过程
* @gate    hInstance[in]:
* @gate		message[in]:
* @gate		wParam[in]:
* @gate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK gateChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			if(gwDeviceType == 1)//测厚
			{
				memcpy(GateInfos, stOldGateInfos, sizeof(GATE_INFO)*CHANNEL_NUM*GATE_NUM);	//读取保存的闸门信息 测厚
			}
		//	if(hStaticBrush == NULL)	
		//		hStaticBrush = CreateSolidBrush(BK_COLOR_WHITE);
						//画按钮控件
			hBtnGateSelect = CreateWindow(_T("BUTTON"),_T("gateSelect"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_GATE_SELECT,hInst,NULL);

			hBtnGateAlarmMode = CreateWindow(_T("BUTTON"),_T("gateAlarmMode"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_GATE_ALARMMODE,hInst,NULL);

			hBtnGateSwitch = CreateWindow(_T("BUTTON"),_T("gateSwitch"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_GATE_SWITCH,hInst,NULL);

			hBtnGateQiDian = CreateWindow(_T("BUTTON"),_T("gateQiDian"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_GATE_QIANYAN,hInst,NULL);

			hBtnGateKuanDu = CreateWindow(_T("BUTTON"),_T("gateKuanDu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_GATE_HOUYAN,hInst,NULL);

			hBtnGateFuZhi = CreateWindow(_T("BUTTON"),_T("gateFuZhi"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 5*BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_GATE_FUZHI,hInst,NULL);
					
			gateBtnList[0] = hBtnGateSelect;
			gateBtnList[1] = hBtnGateAlarmMode;
			gateBtnList[2] = hBtnGateSwitch;
			gateBtnList[3] = hBtnGateQiDian;
			gateBtnList[4] = hBtnGateKuanDu;
			gateBtnList[5] = hBtnGateFuZhi;

			//按钮控件的自定义处理过程
			OldBtnGateSelectProc = (WNDPROC) GetWindowLong(hBtnGateSelect,GWL_WNDPROC);
			SetWindowLong(hBtnGateSelect,GWL_WNDPROC,(LONG)btnGateSelectProc);

			OldBtnGateAlarmModeProc = (WNDPROC) GetWindowLong(hBtnGateAlarmMode,GWL_WNDPROC);
			SetWindowLong(hBtnGateAlarmMode,GWL_WNDPROC,(LONG)btnGateAlarmModeProc);

			OldBtnGateSwitchProc = (WNDPROC) GetWindowLong(hBtnGateSwitch,GWL_WNDPROC);
			SetWindowLong(hBtnGateSwitch,GWL_WNDPROC,(LONG)btnGateSwitchProc);

			OldBtnGateQiDianProc = (WNDPROC) GetWindowLong(hBtnGateQiDian,GWL_WNDPROC);
			SetWindowLong(hBtnGateQiDian,GWL_WNDPROC,(LONG)btnGateQiDianProc);

			OldBtnGateKuanDuProc = (WNDPROC) GetWindowLong(hBtnGateKuanDu,GWL_WNDPROC);
			SetWindowLong(hBtnGateKuanDu,GWL_WNDPROC,(LONG)btnGateKuanDuProc);

			OldBtnGateFuZhiProc = (WNDPROC) GetWindowLong(hBtnGateFuZhi,GWL_WNDPROC);
			SetWindowLong(hBtnGateFuZhi,GWL_WNDPROC,(LONG)btnGateFuZhiProc);

			currentGateBtn = gateBtnList[0];//默认当前选中闸门设定选择按钮
			SetFocus(currentGateBtn);//闸门设定选择按钮获得输入焦点
			if(!gbHotGateFlag)
				gCurrentFocusBtn = currentGateBtn;

			//  songchenguang add start 2010-05-25
			// 显示当前闸门设定信息
			DisplayGateIndex();
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndGate, &ps);


			//  songchenguang add end 2010-05-25
			EndPaint(hWndGate, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				memcpy(stOldGateInfos, GateInfos, sizeof(GATE_INFO)*CHANNEL_NUM*GATE_NUM);	//保存闸门信息
				if(!gbHotGateFlag)
				{
					ShowWindow(hWndGateDisplay,SW_SHOW);
					SetFocus(currentGateDisplayBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndGate);
					hWndGate = 0;					
					
					gCurrentFocusBtn = currentGateDisplayBtn;

					gCurrentWnd = hWndGateDisplay;
				}
				else
				{
					if(gCurrentWnd)//有子菜单窗口
					{
						ShowWindow(gCurrentWnd, SW_SHOW);
						SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
					}

					DestroyWindow(hWndGate);
					hWndGate = 0;
					if(!gCurrentWnd)//没有子菜单窗口
					{
						SetMenuStateFlag(FALSE);
						SetFocus(hWndMain);//主窗口获得输入焦点
					}
					
					gbHotGateFlag = FALSE;

				/*	memset(gszTextPeak, 0, sizeof(gszTextPeak));

					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);*/

				}
			}
			break;
		
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			{
				WORD nAlarmMode = WORD(GateInfos[CurrentChannelNo][GateIndex-1].alarmMode);
				BOOL bOpen = GateInfos[CurrentChannelNo][GateIndex-1].bOpen;
				float fFront = GateInfos[CurrentChannelNo][GateIndex-1].fFrontGate;
				float fBack = GateInfos[CurrentChannelNo][GateIndex-1].fBackGate;
				float fPeakGate = GateInfos[CurrentChannelNo][GateIndex-1].fPeakGate;
				pdis=(LPDRAWITEMSTRUCT)lParam;
				gate_DrawButtonBitmap(hInst,pdis);

				wchar_t strText[20];
				InitializeCriticalSection(&csGate);
				EnterCriticalSection(&csGate);
	
				if(pdis->hwndItem == hBtnGateSelect)
				{
					memset(strText, 0, sizeof(strText));
					wsprintf(strText, _T("%d"), GateIndex);

					DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else if(pdis->hwndItem == hBtnGateAlarmMode)
				{
					memset(strText, 0, sizeof(strText));

					if(GateInfos[CurrentChannelNo][GateIndex-1].alarmMode == SuperHigh)
					{
						wsprintf(strText, _T("进波"));	
					}
					else if(GateInfos[CurrentChannelNo][GateIndex-1].alarmMode == LostWave)
					{
						wsprintf(strText, _T("失波"));
					}
					
					DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else if(pdis->hwndItem == hBtnGateSwitch)
				{
					memset(strText, 0, sizeof(strText));
					if(GateInfos[CurrentChannelNo][GateIndex-1].bOpen)
					{
						wsprintf(strText, _T("开"));	
					}
					else 
					{
						wsprintf(strText, _T("关"));
					}
					
					DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else if(pdis->hwndItem == hBtnGateQiDian)
				{
					memset(strText, 0, sizeof(strText));
					
					wsprintf(strText, _T("%.1f"), GateInfos[CurrentChannelNo][GateIndex-1].fFrontGate);

					DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else if(pdis->hwndItem == hBtnGateKuanDu)
				{
					memset(strText, 0, sizeof(strText));
					
					wsprintf(strText, _T("%.1f"), (GateInfos[CurrentChannelNo][GateIndex-1].fBackGate - GateInfos[CurrentChannelNo][GateIndex-1].fFrontGate));

					DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else if(pdis->hwndItem == hBtnGateFuZhi)
				{
					memset(strText, 0, sizeof(strText));
	
					wsprintf(strText, _T("%.0f"), GateInfos[CurrentChannelNo][GateIndex-1].fPeakGate);

					DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				
				LeaveCriticalSection(&csGate);
				DeleteCriticalSection(&csGate);

				// 检测按钮句柄，显示相应的步长
				memset(gszTextStep, 0, sizeof(gszTextStep));

				if(pdis->hwndItem == currentGateBtn)
				{
					if(!gbHotDB && !gbHotRange)
					{
						if(pdis->hwndItem == hBtnGateQiDian || pdis->hwndItem == hBtnGateKuanDu)
							swprintf(gszTextStep, _T("%.1f"), fGateStep);
						else
							swprintf(gszTextStep, _T(""));

						PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
					}
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
				// 更新按钮项数据
				if(CurrentChannelNo == 0)
				{
					UpdateGateMenuInfo(1);
				}
				else
				{
					UpdateGateMenuInfo(0);
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
*加载闸门设定窗口按钮图片
* @gate    hInstance[in]:
* @gate		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int gate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO gate_gateBtnBitmap[]=
	{
		{IDC_GATE_SELECT	,IDB_BITMAP_ZHAMENXUANZE_NOSEL_R	 ,  IDB_BITMAP_ZHAMENXUANZE_SELECT_R		},
		{IDC_GATE_ALARMMODE	,IDB_BITMAP_BAOJINGMOSHI_NOSEL_R	 ,  IDB_BITMAP_BAOJINGMOSHI_SELECT_R		},		
		{IDC_GATE_SWITCH	,IDB_BITMAP_KAIQIGUANBI6_NOSEL_R	 ,  IDB_BITMAP_KAIQIGUANBI6_SELECT_R		},
		{IDC_GATE_QIANYAN	,IDB_BITMAP_QIANYAN_NOSEL_R ,  IDB_BITMAP_QIANYAN_SELECT_R	},
		{IDC_GATE_HOUYAN	,IDB_BITMAP_HOUYAN_NOSEL_R ,  IDB_BITMAP_HOUYAN_SELECT_R	},
		{IDC_GATE_FUZHI		,IDB_BITMAP_FUZHI_NOSEL_R ,  IDB_BITMAP_FUZHI_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,gate_gateBtnBitmap);
	return ReturnVal;
}

void DisplayGateIndex()
{
	// 闸门设定号
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	wsprintf(strText, _T("%d"), GateIndex);

	DrawLableText(hBtnGateSelect, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	
	WORD nAlarmMode = WORD(GateInfos[CurrentChannelNo][GateIndex-1].alarmMode);
	BOOL bOpen = GateInfos[CurrentChannelNo][GateIndex-1].bOpen;
	float fFront = GateInfos[CurrentChannelNo][GateIndex-1].fFrontGate;
	float fBack = GateInfos[CurrentChannelNo][GateIndex-1].fBackGate;
	float fPeakGate = GateInfos[CurrentChannelNo][GateIndex-1].fPeakGate;
	float fKuanDu = fBack - fFront;
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);

	// 报警模式
	SetAlarmMode(hBtnGateAlarmMode, GateIndex, nAlarmMode);

	// 开启标志
	SetOpenFlag(hBtnGateSwitch, bOpen, GateIndex);

	// 起点
	SetQiDian(hBtnGateQiDian, GateIndex, fFront, fBack, FALSE);
	// 宽度
	SetKuanDu(hBtnGateKuanDu, GateIndex, fFront, fKuanDu, FALSE);
	// 峰沿
	SetPeakGate(hBtnGateFuZhi, GateIndex, fPeakGate, TRUE);
}
/**************************
*闸门设定选择按钮自定义处理过程
* @gate    hInstance[in]:
* @gate		message[in]:
* @gate		wParam[in]:
* @gate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnGateSelectProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(gateBtnList[i] != currentGateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentGateBtn = gateBtnList[i];
					SetFocus(currentGateBtn);
					if(!gbHotGateFlag)
						gCurrentFocusBtn = currentGateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(gateBtnList[i] != currentGateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentGateBtn = gateBtnList[i];
					SetFocus(currentGateBtn);
					if(!gbHotGateFlag)
						gCurrentFocusBtn = currentGateBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					// songchenguang add start 2010-05-12
					if(GateIndex > 1)
						GateIndex--;
					else
						GateIndex = 2;

					DisplayGateIndex();
					// songchenguang add end 2010-05-12
				}
				else if(wParam == VK_RIGHT)
				{
					// songchenguang add start 2010-05-12
					if(GateIndex < 2)
						GateIndex++;
					else
						GateIndex = 1;

					DisplayGateIndex();
					// songchenguang add end 2010-05-12
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					if(gbHotGateFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(!gbHotGateFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(!gbHotGateFlag)
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
				else if(wParam == 'B')	// 冻结
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
	return CallWindowProc(OldBtnGateSelectProc,Wnd,Msg,wParam,lParam);
}

void SetAlarmMode(HWND hWnd, const WORD& nGateIndex, const WORD& nIndex)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(nIndex == SuperHigh)
	{
		wsprintf(strText, _T("进波"));	
	}
	else if(nIndex == LostWave)
	{
		wsprintf(strText, _T("失波"));
	}
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	// 传值
	if(!gbGuanLian)
	{
	/*	if(nGateIndex == 1)
			chCurrent_gate_reg->wGate1Alarm = nIndex;
		else if(nGateIndex == 2)
			chCurrent_gate_reg->wGate2Alarm = nIndex;
		else if(nGateIndex == 3)
			chCurrent_gate_reg->wGate3Alarm = nIndex;*/
		if(CurrentChannelNo == 0)
		{
			if(nGateIndex == 1)
				cscan_ch1_gate_reg->wGate1SyncMode = nIndex;
			else if(nGateIndex == 2)
				cscan_ch1_gate_reg->wGate2SyncMode = nIndex;
			
		}
		else if(CurrentChannelNo == 1)
		{
			if(nGateIndex == 1)
				cscan_ch1_gate_reg->wGate1SyncMode = nIndex;
			else if(nGateIndex == 2)
				cscan_ch1_gate_reg->wGate2SyncMode = nIndex;
		}
	}
	else
	{
	/*	if(nGateIndex == 1)
		{
			ch1_gate_reg->wGate1Alarm = nIndex;
			ch2_gate_reg->wGate1Alarm = nIndex;
		}
		else if(nGateIndex == 2)
		{
			ch1_gate_reg->wGate2Alarm = nIndex;
			ch2_gate_reg->wGate2Alarm = nIndex;
		}
		else if(nGateIndex == 3)
		{
			ch1_gate_reg->wGate3Alarm = nIndex;
			ch2_gate_reg->wGate3Alarm = nIndex;
		}*/
		if(nGateIndex == 1)
		{
			cscan_ch1_gate_reg->wGate1SyncMode = nIndex;
			cscan_ch1_gate_reg->wGate1SyncMode = nIndex;
		}
		else if(nGateIndex == 2)
		{
			cscan_ch1_gate_reg->wGate2SyncMode = nIndex;
			cscan_ch2_gate_reg->wGate2SyncMode = nIndex;
		}
	}
	
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	if(!gbGuanLian)
	{
		GateInfos[CurrentChannelNo][nGateIndex-1].alarmMode = (alarm_modes)nIndex;
	}
	else
	{
		GateInfos[0][nGateIndex-1].alarmMode = (alarm_modes)nIndex;
		GateInfos[1][nGateIndex-1].alarmMode = (alarm_modes)nIndex;
	}
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
	
	// songchenguang add start 2010-06-13
	// 保存配置到默认文件
	SaveParamFile(lpDefaultParamFile);
	// songchenguang add end 2010-06-13
}

/**************************
*报警模式选择按钮自定义处理过程
* @gate    hInstance[in]:
* @gate		message[in]:
* @gate		wParam[in]:
* @gate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnGateAlarmModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);

	WORD mode = (WORD)GateInfos[CurrentChannelNo][GateIndex-1].alarmMode;
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);

	
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					if(!gbHotGateFlag)
					{
						HWND hParentWnd = GetParent(Wnd);

						if(hParentWnd != NULL)
							SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
					}
					else
					{
						if(gCurrentWnd)//有子菜单窗口
						{
							ShowWindow(gCurrentWnd, SW_SHOW);
							SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
						}

						DestroyWindow(hWndGate);
						hWndGate = 0;
						if(!gCurrentWnd)//没有子菜单窗口
						{
							SetMenuStateFlag(FALSE);
							SetFocus(hWndMain);//主窗口获得输入焦点
						}
						
						gbHotGateFlag = FALSE;

					/*	memset(gszTextPeak, 0, sizeof(gszTextPeak));

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);*/
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(gateBtnList[i] != currentGateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentGateBtn = gateBtnList[i];
					SetFocus(currentGateBtn);
					if(!gbHotGateFlag)
						gCurrentFocusBtn = currentGateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(gateBtnList[i] != currentGateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentGateBtn = gateBtnList[i];
					SetFocus(currentGateBtn);
					if(!gbHotGateFlag)
						gCurrentFocusBtn = currentGateBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					//wangbingf add start at 2010-8-12
					//先把前一个报警模式的报警关闭
/*					sys_reg->wLedAlarm &= 0xFFFE;
					if(CurrentChannelNo == 0)
					{
						ch1_con_reg->wReset |= 0x1;
						ch1_con_reg->wReset &= 0xFFFE;
					}
					else if(CurrentChannelNo == 1)
					{
						ch2_con_reg->wReset |= 0x1;
						ch2_con_reg->wReset &= 0xFFFE;
					}
*/					//wangbingf add end at 2010-8-12
					// songchenguang add start 2010-05-12
					if(mode > SuperHigh)
						mode --;
					else
						mode = LostWave;

					SetAlarmMode(Wnd, GateIndex, mode);

					InitializeCriticalSection(&csGate);
					EnterCriticalSection(&csGate);
					if(!gbGuanLian)
					{
						GateInfos[CurrentChannelNo][GateIndex-1].alarmMode = (alarm_modes)mode;
					}
					else
					{
						GateInfos[0][GateIndex-1].alarmMode = (alarm_modes)mode;
						GateInfos[1][GateIndex-1].alarmMode = (alarm_modes)mode;
					}
					LeaveCriticalSection(&csGate);
					DeleteCriticalSection(&csGate);
					// songchenguang add end 2010-05-12
				}
				else if(wParam == VK_RIGHT)
				{
					//wangbingf add start at 2010-8-12
					//先把前一个报警模式的报警关闭
/*					sys_reg->wLedAlarm &= 0xFFFE;
					if(CurrentChannelNo == 0)
					{
						ch1_con_reg->wReset |= 0x1;
						ch1_con_reg->wReset &= 0xFFFE;
					}
					else if(CurrentChannelNo == 1)
					{
						ch2_con_reg->wReset |= 0x1;
						ch2_con_reg->wReset &= 0xFFFE;
					}
*/					//wangbingf add end at 2010-8-12
					if(mode < LostWave)
						mode ++;
					else
						mode = SuperHigh;

					SetAlarmMode(Wnd, GateIndex, mode);

					InitializeCriticalSection(&csGate);
					EnterCriticalSection(&csGate);
					if(!gbGuanLian)
					{
						GateInfos[CurrentChannelNo][GateIndex-1].alarmMode = (alarm_modes)mode;
					}
					else
					{
						GateInfos[0][GateIndex-1].alarmMode = (alarm_modes)mode;
						GateInfos[1][GateIndex-1].alarmMode = (alarm_modes)mode;
					}
					LeaveCriticalSection(&csGate);
					DeleteCriticalSection(&csGate);
					// songchenguang add end 2010-05-12
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					if(gbHotGateFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(!gbHotGateFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(!gbHotGateFlag)
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
				else if(wParam == 'B')	// 冻结
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
	return CallWindowProc(OldBtnGateAlarmModeProc,Wnd,Msg,wParam,lParam);
}

void SetOpenFlag(HWND hWnd, const BOOL& bOpen, const WORD& nGateIndex)
{
	if(hWnd)
	{
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		if(bOpen)
		{
			wsprintf(strText, _T("开"));	
		}
		else 
		{
			wsprintf(strText, _T("关"));
		}
		
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}

	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	if(!gbGuanLian)
	{
		GateInfos[CurrentChannelNo][nGateIndex-1].bOpen = bOpen;

//		if(bOpen == OFF)
//			sys_reg->wLedAlarm &= 0xFFFE;
	}
	else
	{
		GateInfos[0][nGateIndex-1].bOpen = bOpen;
		GateInfos[1][nGateIndex-1].bOpen = bOpen;

//		if(bOpen == OFF)
//			sys_reg->wLedAlarm &= 0xFFFE;
	}
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
					
	if(!gbGuanLian)
	{
	//	if(nGateIndex == 1)
	//		chCurrent_gate_reg->wGate1Enable = (WORD)bOpen;
	//	else if(nGateIndex == 2)
	//		chCurrent_gate_reg->wGate2Enable = (WORD)bOpen;
	//	else if(nGateIndex == 3)
	//		chCurrent_gate_reg->wGate3Enable = (WORD)bOpen;
		if(CurrentChannelNo == 0)
		{
			if(nGateIndex == 1)
				cscan_ch1_gate_reg->wGate1Enable = (WORD)bOpen;
			else if(nGateIndex == 2)
				cscan_ch1_gate_reg->wGate2Enable = (WORD)bOpen;
			
		}
		else if(CurrentChannelNo == 1)
		{
			if(nGateIndex == 1)
				cscan_ch2_gate_reg->wGate1Enable = (WORD)bOpen;
			else if(nGateIndex == 2)
				cscan_ch2_gate_reg->wGate2Enable = (WORD)bOpen;
		}
		
	}
	else
	{
		if(nGateIndex == 1)
		{
			cscan_ch1_gate_reg->wGate1Enable = (WORD)bOpen;
			cscan_ch2_gate_reg->wGate1Enable = (WORD)bOpen;
		}
		else if(nGateIndex == 2)
		{
			cscan_ch1_gate_reg->wGate2Enable = (WORD)bOpen;
			cscan_ch2_gate_reg->wGate2Enable = (WORD)bOpen;
		}
	//	else if(nGateIndex == 3)
	//	{
	//		ch1_gate_reg->wGate3Enable = (WORD)bOpen;
	//		ch2_gate_reg->wGate3Enable = (WORD)bOpen;
	//	}
	}

	// songchenguang add start 2010-06-13
	// 保存配置到默认文件
//	if(hWnd)
		SaveParamFile(lpDefaultParamFile);
	// songchenguang add end 2010-06-13

	SetRedrawFlag(TRUE);
}
/**************************
*开关按钮自定义处理过程
* @gate    hInstance[in]:
* @gate		message[in]:
* @gate		wParam[in]:
* @gate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnGateSwitchProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	BOOL bOpen = GateInfos[CurrentChannelNo][GateIndex-1].bOpen;
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
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
					while(gateBtnList[i] != currentGateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentGateBtn = gateBtnList[i];
					SetFocus(currentGateBtn);
					if(!gbHotGateFlag)
						gCurrentFocusBtn = currentGateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(gateBtnList[i] != currentGateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentGateBtn = gateBtnList[i];
					SetFocus(currentGateBtn);
					if(!gbHotGateFlag)
						gCurrentFocusBtn = currentGateBtn;
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					// songchenguang add start 2010-05-12
					bOpen = !bOpen;

					// 传值
					SetOpenFlag(Wnd, bOpen, GateIndex);
					
					// songchenguang add end 2010-05-12
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					if(gbHotGateFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(!gbHotGateFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(!gbHotGateFlag)
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
				else if(wParam == 'B')	// 冻结
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
	return CallWindowProc(OldBtnGateSwitchProc,Wnd,Msg,wParam,lParam);
}

void SetQianYan(HWND hWnd, const WORD& nGateIndex, const float& fFront, const BOOL& bSave)
{
	if(hWnd)
	{
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		
		wsprintf(strText, _T("%.2f"), fFront);

		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}

	if(!gbGuanLian)
	{
	/*	if(nGateIndex == 1)
			chCurrent_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
		else if(nGateIndex == 2)
			chCurrent_gate_reg->wGate2Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
		else if(nGateIndex == 3)
			chCurrent_gate_reg->wGate3Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));*/
		if(CurrentChannelNo == 0)
		{
			if(nGateIndex == 1)
				cscan_ch1_gate_reg->wGate1Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			else if(nGateIndex == 2)
				cscan_ch1_gate_reg->wGate1Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			
		}
		else if(CurrentChannelNo == 1)
		{
			if(nGateIndex == 1)
				cscan_ch2_gate_reg->wGate1Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			else if(nGateIndex == 2)
				cscan_ch2_gate_reg->wGate1Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
		}
	}
	else
	{
		if(nGateIndex == 1)
		{
			cscan_ch1_gate_reg->wGate1Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			cscan_ch2_gate_reg->wGate1Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
		}
		else if(nGateIndex == 2)
		{
			cscan_ch1_gate_reg->wGate1Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			cscan_ch2_gate_reg->wGate1Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
		}
	}

	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	if(!gbGuanLian)
	{
		GateInfos[CurrentChannelNo][nGateIndex-1].fFrontGate = fFront;
	}
	else
	{
		GateInfos[0][nGateIndex-1].fFrontGate = fFront;
		GateInfos[1][nGateIndex-1].fFrontGate = fFront;
	}
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
	// songchenguang add start 2010-06-13
	// 保存配置到默认文件
	if(bSave)
		SaveParamFile(lpDefaultParamFile);
	// songchenguang add end 2010-06-13
}


void SetQiDian(HWND hWnd, const WORD& nGateIndex, const float& fFront, const float& fBack, BOOL bSave)
{
	if(hWnd)
	{
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		
		wsprintf(strText, _T("%.1f"), fFront);

		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}

	if(!gbGuanLian)
	{
	/*	if(nGateIndex == 1)
		{
			chCurrent_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
		
			chCurrent_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
		}
		else if(nGateIndex == 2)
		{
			chCurrent_gate_reg->wGate2Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
		
			chCurrent_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
		}
		else if(nGateIndex == 3)
		{
			chCurrent_gate_reg->wGate3Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
		
			chCurrent_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
		}*/
		if(CurrentChannelNo == 0)
		{
			if(nGateIndex == 1)
			{
				cscan_ch1_gate_reg->wGate1Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));		
				cscan_ch1_gate_reg->wGate1Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack - fFront));
			}
			else if(nGateIndex == 2)
			{
				cscan_ch1_gate_reg->wGate2Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));			
				cscan_ch1_gate_reg->wGate2Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack - fFront));
			}	
		}
		else if(CurrentChannelNo == 1)
		{
			if(nGateIndex == 1)
			{
				cscan_ch2_gate_reg->wGate1Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));		
				cscan_ch2_gate_reg->wGate1Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack - fFront));
			}
			else if(nGateIndex == 2)
			{
				cscan_ch2_gate_reg->wGate2Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));			
				cscan_ch2_gate_reg->wGate2Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack - fFront));
			}
		}
	}
	else
	{
	/*	if(nGateIndex == 1)
		{
			ch1_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			ch2_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			
			ch1_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
			ch2_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
		}
		else if(nGateIndex == 2)
		{
			ch1_gate_reg->wGate2Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			ch2_gate_reg->wGate2Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));

			ch1_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
			ch2_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
		}
		else if(nGateIndex == 3)
		{
			ch1_gate_reg->wGate3Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			ch2_gate_reg->wGate3Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));

			ch1_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
			ch2_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
		}*/
		if(nGateIndex == 1)
		{
			cscan_ch1_gate_reg->wGate1Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			cscan_ch2_gate_reg->wGate1Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			
			cscan_ch1_gate_reg->wGate1Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack - fFront));
			cscan_ch2_gate_reg->wGate1Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack - fFront));
		}
		else if(nGateIndex == 2)
		{
			cscan_ch1_gate_reg->wGate2Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			cscan_ch2_gate_reg->wGate2Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));

			cscan_ch1_gate_reg->wGate2Start = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack - fFront));
			cscan_ch2_gate_reg->wGate2Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack - fFront));
		}
	}

	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	if(!gbGuanLian)
	{
		GateInfos[CurrentChannelNo][nGateIndex-1].fFrontGate = fFront;
		
		GateInfos[CurrentChannelNo][nGateIndex-1].fBackGate= fBack;
	}
	else
	{
		GateInfos[0][nGateIndex-1].fFrontGate = fFront;
		GateInfos[1][nGateIndex-1].fFrontGate = fFront;

		GateInfos[0][nGateIndex-1].fBackGate = fBack;
		GateInfos[1][nGateIndex-1].fBackGate = fBack;
	}
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
	// songchenguang add start 2010-06-13
	// 保存配置到默认文件
	if(bSave)
		SaveParamFile(lpDefaultParamFile);
	// songchenguang add end 2010-06-13
}

/**************************
*起点按钮自定义处理过程
* @gate    hInstance[in]:
* @gate		message[in]:
* @gate		wParam[in]:
* @gate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnGateQiDianProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	// songchenguang add start 2010-05-12
	// 获取范围最小值和最大值
	InitializeCriticalSection(&csRange);
	EnterCriticalSection(&csRange);

	float fStart = fRangeStart[CurrentChannelNo];
	float fEnd = fRangeEnd[CurrentChannelNo];

	LeaveCriticalSection(&csRange);
	DeleteCriticalSection(&csRange);
	
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	float fFront = GateInfos[CurrentChannelNo][GateIndex-1].fFrontGate;
	float fBack = GateInfos[CurrentChannelNo][GateIndex-1].fBackGate;
	float fKuanDu = fBack - fFront;
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
	// songchenguang add end 2010-05-12

	switch(Msg)
	{
	case WM_CREATE:
		fGateStep = AMENDSTEPS[nGateStepIndex];
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
					while(gateBtnList[i] != currentGateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentGateBtn = gateBtnList[i];
					SetFocus(currentGateBtn);
					if(!gbHotGateFlag)
						gCurrentFocusBtn = currentGateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(gateBtnList[i] != currentGateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentGateBtn = gateBtnList[i];
					SetFocus(currentGateBtn);
					if(!gbHotGateFlag)
						gCurrentFocusBtn = currentGateBtn;
				}
				else if(wParam == VK_LEFT)
				{
					// songchenguang add start 2010-05-12
					if(fFront > 0)
					{
						fFront -= fGateStep;
						if(fFront < 0)
							fFront = 0;
						fBack = fFront + fKuanDu;
					}

					SetQiDian(Wnd, GateIndex, fFront, fBack, TRUE);
					
					// songchenguang add end 2010-05-12
				}
				else if(wParam == VK_RIGHT)
				{
					// songchenguang add start 2010-05-12
					if(fFront < (MAX_RANGE - fKuanDu))
					{
						fFront += fGateStep;
						if( fFront > (MAX_RANGE - fKuanDu))
							fFront = MAX_RANGE - fKuanDu;
						fBack = fFront + fKuanDu;
						if(fBack > MAX_RANGE)
							fBack = MAX_RANGE;
					}

					SetQiDian(Wnd, GateIndex, fFront, fBack, TRUE);
					// songchenguang add end 2010-05-12
				}
				else if(wParam == 'F') // 步长
				{
					if(nGateStepIndex < 3)
						nGateStepIndex++;
					else
						nGateStepIndex = 0;

					fGateStep = AMENDSTEPS[nGateStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fGateStep);
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
					if(gbHotGateFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(!gbHotGateFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(!gbHotGateFlag)
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
				else if(wParam == 'B')	// 冻结
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
	return CallWindowProc(OldBtnGateQiDianProc,Wnd,Msg,wParam,lParam);
}

void SetHouYan(HWND hWnd, const WORD& nGateIndex, const float& fBack, const BOOL& bSave)
{
	if(hWnd)
	{
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		
		wsprintf(strText, _T("%.2f"), fBack);
		
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}

	if(!gbGuanLian)
	{
		if(nGateIndex == 1)
			chCurrent_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
		else if(nGateIndex == 2)
			chCurrent_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
		else if(nGateIndex == 3)
			chCurrent_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));					
	}
	else
	{
		if(nGateIndex == 1)
		{
			ch1_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
			ch2_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
		}
		else if(nGateIndex == 2)
		{
			ch1_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
			ch2_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
		}
		else if(nGateIndex == 3)
		{
			ch1_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
			ch2_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
		}
	}

	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	if(!gbGuanLian)
	{
		GateInfos[CurrentChannelNo][nGateIndex-1].fBackGate = fBack;
	}
	else
	{
		GateInfos[0][nGateIndex-1].fBackGate = fBack;
		GateInfos[1][nGateIndex-1].fBackGate = fBack;
	}
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
	// songchenguang add start 2010-06-13
	// 保存配置到默认文件
	if(bSave)
		SaveParamFile(lpDefaultParamFile);
	// songchenguang add end 2010-06-13
}

void SetKuanDu(HWND hWnd, const WORD& nGateIndex, const float& fFront, const float& fKuanDu, BOOL bSave)
{
	if(hWnd)
	{
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		
		wsprintf(strText, _T("%.1f"), fKuanDu);
		
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}

	if(!gbGuanLian)
	{
	/*	if(nGateIndex == 1)
			chCurrent_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (fFront + fKuanDu)));
		else if(nGateIndex == 2)
			chCurrent_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (fFront + fKuanDu)));
		else if(nGateIndex == 3)
			chCurrent_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (fFront + fKuanDu)));
		*/
		if(CurrentChannelNo == 0)
		{
			if(nGateIndex == 1)
				cscan_ch1_gate_reg->wGate1Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fKuanDu));
			else if(nGateIndex == 2)
				cscan_ch1_gate_reg->wGate2Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fKuanDu));
		}
		else if(CurrentChannelNo == 1)
		{
			if(nGateIndex == 1)
				cscan_ch2_gate_reg->wGate1Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fKuanDu));
			else if(nGateIndex == 2)
				cscan_ch2_gate_reg->wGate2Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fKuanDu));
		}
	}
	else
	{
	/*	if(nGateIndex == 1)
		{
			ch1_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (fFront + fKuanDu)));
			ch2_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (fFront + fKuanDu)));
		}
		else if(nGateIndex == 2)
		{
			ch1_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (fFront + fKuanDu)));
			ch2_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (fFront + fKuanDu)));
		}
		else if(nGateIndex == 3)
		{
			ch1_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (fFront + fKuanDu)));
			ch2_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (fFront + fKuanDu)));
		}*/
		if(nGateIndex == 1)
		{
			cscan_ch1_gate_reg->wGate1Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fKuanDu));
			cscan_ch2_gate_reg->wGate1Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fKuanDu));
		}
		else if(nGateIndex == 2)
		{
			cscan_ch1_gate_reg->wGate2Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fKuanDu));
			cscan_ch2_gate_reg->wGate2Width = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fKuanDu));
		}
	}

	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	if(!gbGuanLian)
	{
		GateInfos[CurrentChannelNo][nGateIndex-1].fBackGate = fFront + fKuanDu;
	}
	else
	{
		GateInfos[0][nGateIndex-1].fBackGate = fFront + fKuanDu;
		GateInfos[1][nGateIndex-1].fBackGate = fFront + fKuanDu;
	}
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
	// songchenguang add start 2010-06-13
	// 保存配置到默认文件
	if(bSave)
		SaveParamFile(lpDefaultParamFile);
	// songchenguang add end 2010-06-13
}
/**************************
*后沿按钮自定义处理过程
* @gate    hInstance[in]:
* @gate		message[in]:
* @gate		wParam[in]:
* @gate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnGateKuanDuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	// songchenguang add start 2010-05-12
	// 获取范围最小值和最大值
	InitializeCriticalSection(&csRange);
	EnterCriticalSection(&csRange);

	float fStart = fRangeStart[CurrentChannelNo];
	float fEnd = fRangeEnd[CurrentChannelNo];

	LeaveCriticalSection(&csRange);
	DeleteCriticalSection(&csRange);	

	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	float fFront = GateInfos[CurrentChannelNo][GateIndex-1].fFrontGate;
	float fBack = GateInfos[CurrentChannelNo][GateIndex-1].fBackGate;
	float fKuanDu = fBack - fFront;	//闸门宽度
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
	// songchenguang add end 2010-05-12

	switch(Msg)
	{
	case WM_CREATE:
		fGateStep = AMENDSTEPS[nGateStepIndex];
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
					while(gateBtnList[i] != currentGateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentGateBtn = gateBtnList[i];
					SetFocus(currentGateBtn);
					if(!gbHotGateFlag)
						gCurrentFocusBtn = currentGateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(gateBtnList[i] != currentGateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentGateBtn = gateBtnList[i];
					SetFocus(currentGateBtn);
					if(!gbHotGateFlag)
						gCurrentFocusBtn = currentGateBtn;
				}
			/*	else if(wParam == VK_LEFT)
				{
					// songchenguang add start 2010-05-12
					if(fBack > fFront)
						fBack -= fGateStep;

					SetHouYan(Wnd, GateIndex, fBack);

					// songchenguang add end 2010-05-12
				}
				else if(wParam == VK_RIGHT)
				{
					// songchenguang add start 2010-05-12
					if(GateInfos[CurrentChannelNo][GateIndex-1].fBackGate < fEnd)
						GateInfos[CurrentChannelNo][GateIndex-1].fBackGate += fGateStep;

					float fBack = GateInfos[CurrentChannelNo][GateIndex-1].fBackGate;
					SetHouYan(Wnd, GateIndex, fBack);

					// songchenguang add end 2010-05-12
				}*/
				else if(wParam == VK_LEFT)
				{
					if(fKuanDu > 0)
					{
						fKuanDu = fKuanDu - fGateStep;
						if(fKuanDu < 0)
							fKuanDu = 0;
					}

					SetKuanDu(Wnd, GateIndex, fFront, fKuanDu, TRUE);
				}
				else if(wParam == VK_RIGHT)
				{
					if(fKuanDu + fFront < MAX_RANGE)
					{
						fKuanDu = fKuanDu + fGateStep;
						if(fKuanDu + fFront >  MAX_RANGE)
							fKuanDu = MAX_RANGE - fFront;
					}
					
					SetKuanDu(Wnd, GateIndex, fFront, fKuanDu, TRUE);
				}
				else if(wParam == 'F') // 步长
				{
					if(nGateStepIndex < 3)
						nGateStepIndex++;
					else
						nGateStepIndex = 0;

					fGateStep = AMENDSTEPS[nGateStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fGateStep);
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
					if(gbHotGateFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(!gbHotGateFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(!gbHotGateFlag)
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
				else if(wParam == 'B')	// 冻结
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
	return CallWindowProc(OldBtnGateKuanDuProc,Wnd,Msg,wParam,lParam);
}
/*
void SetGateCurve(WORD* nCurves, 
				  const WORD& nRange,
				  const float& fFuzhi)
{
	if(nRange >= 4096 || nCurves == NULL)
		return;

	WORD *p = nCurves;
	for(int i=0; i<= nRange; i++)
	{
		*p = WORD(MAX_HEIGHT * fFuzhi / 100.0f);
		p++;
	}	
}
*/

void SetPeakGate(HWND hWnd, const WORD& nGateIndex, const float& fFuzhi, const BOOL& bSave)
{
	if(hWnd)
	{
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));		
		wsprintf(strText, _T("%.0f"), fFuzhi);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}

	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	float fFront = GateInfos[CurrentChannelNo][nGateIndex-1].fFrontGate;
	float fBack = GateInfos[CurrentChannelNo][nGateIndex-1].fBackGate;
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);

	WORD nFront = (WORD)Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront);
	WORD nBack = (WORD)Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack);
	WORD nRange = nBack - nFront;

	if(!gbGuanLian)
	{
	/*	if(nGateIndex == 1)
		{
			//SetGateCurve(chCurrent_gate1curve, nRange, fFuzhi);
			chCurrent_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * fFuzhi / 100.0f);			
		}
		else if(nGateIndex == 2)
		{
			//SetGateCurve(chCurrent_gate2curve, nRange, fFuzhi);
			chCurrent_gate_reg->wGate2Peak = WORD(MAX_HEIGHT * fFuzhi / 100.0f);
		}
	//	else if(nGateIndex == 3)
	//	{
	//		SetGateCurve(chCurrent_gate3curve, nRange, fFuzhi);
	//	}*/
		if(CurrentChannelNo == 0)
		{
			if(nGateIndex == 1)
				cscan_ch1_gate_reg->wGate1Amp = WORD(MAX_HEIGHT * fFuzhi / 100.0f);
			else if(nGateIndex == 2)
				cscan_ch1_gate_reg->wGate2Amp = WORD(MAX_HEIGHT * fFuzhi / 100.0f);
		}
		else if(CurrentChannelNo == 1)
		{
			if(nGateIndex == 1)
				cscan_ch1_gate_reg->wGate1Amp = WORD(MAX_HEIGHT * fFuzhi / 100.0f);
			else if(nGateIndex == 2)
				cscan_ch1_gate_reg->wGate2Amp = WORD(MAX_HEIGHT * fFuzhi / 100.0f);
		}
		InitializeCriticalSection(&csGate);
		EnterCriticalSection(&csGate);
		GateInfos[CurrentChannelNo][nGateIndex-1].fPeakGate = fFuzhi;
		LeaveCriticalSection(&csGate);
		DeleteCriticalSection(&csGate);
	}
	else
	{
		if(nGateIndex == 1)
		{
		//	SetGateCurve(ch1_gate1curve, nRange, fFuzhi);
		//	SetGateCurve(ch2_gate1curve, nRange, fFuzhi);
			cscan_ch1_gate_reg->wGate1Amp = WORD(MAX_HEIGHT * fFuzhi / 100.0f);
			cscan_ch2_gate_reg->wGate1Amp = WORD(MAX_HEIGHT * fFuzhi / 100.0f);
		}
		else if(nGateIndex == 2)
		{
		//	SetGateCurve(ch1_gate2curve, nRange, fFuzhi);
		//	SetGateCurve(ch2_gate2curve, nRange, fFuzhi);
			cscan_ch1_gate_reg->wGate2Amp = WORD(MAX_HEIGHT * fFuzhi / 100.0f);
			cscan_ch2_gate_reg->wGate2Amp = WORD(MAX_HEIGHT * fFuzhi / 100.0f);
		}
		
	//	else if(nGateIndex == 3)
	//	{
	//		SetGateCurve(ch1_gate3curve, nRange, fFuzhi);
	//		SetGateCurve(ch2_gate3curve, nRange, fFuzhi);
	//	}
		InitializeCriticalSection(&csGate);
		EnterCriticalSection(&csGate);
		GateInfos[0][nGateIndex-1].fPeakGate = fFuzhi;
		GateInfos[1][nGateIndex-1].fPeakGate = fFuzhi;
		LeaveCriticalSection(&csGate);
		DeleteCriticalSection(&csGate);

	}

	// songchenguang add start 2010-06-13
	// 保存配置到默认文件	
	if(bSave)
		SaveParamFile(lpDefaultParamFile);
	// songchenguang add end 2010-06-13
}
/**************************
*幅值按钮自定义处理过程
* @gate    hInstance[in]:
* @gate		message[in]:
* @gate		wParam[in]:
* @gate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnGateFuZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	// songchenguang add start 2010-05-12
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	float fPeakGate = GateInfos[CurrentChannelNo][GateIndex-1].fPeakGate;
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
	// songchenguang add start 2010-05-12

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
					while(gateBtnList[i] != currentGateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentGateBtn = gateBtnList[i];
					SetFocus(currentGateBtn);
					if(!gbHotGateFlag)
						gCurrentFocusBtn = currentGateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(gateBtnList[i] != currentGateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentGateBtn = gateBtnList[i];
					SetFocus(currentGateBtn);
					if(!gbHotGateFlag)
						gCurrentFocusBtn = currentGateBtn;
				}
				else if(wParam == VK_LEFT)
				{
					// songchenguang add start 2010-05-12
					if(fPeakGate > MIN_PEAK)
						fPeakGate -= nPeakStep;

					// 传值
					SetPeakGate(Wnd, GateIndex, fPeakGate, TRUE);
					
				/*	InitializeCriticalSection(&csGate);
					EnterCriticalSection(&csGate);
					if(!gbGuanLian)
					{
						GateInfos[CurrentChannelNo][GateIndex-1].fPeakGate = fPeakGate;
					}
					else
					{
						GateInfos[0][GateIndex-1].fPeakGate = fPeakGate;
						GateInfos[1][GateIndex-1].fPeakGate = fPeakGate;
					}
					LeaveCriticalSection(&csGate);
					DeleteCriticalSection(&csGate);
					*/
					// songchenguang add end 2010-05-12
				}
				else if(wParam == VK_RIGHT)
				{
					// songchenguang add start 2010-05-12
					if(fPeakGate < MAX_PEAK)
						fPeakGate += nPeakStep;

					SetPeakGate(Wnd, GateIndex, fPeakGate, TRUE);

				/*	InitializeCriticalSection(&csGate);
					EnterCriticalSection(&csGate);
					if(!gbGuanLian)
					{
						GateInfos[CurrentChannelNo][GateIndex-1].fPeakGate = fPeakGate;
					}
					else
					{
						GateInfos[0][GateIndex-1].fPeakGate = fPeakGate;
						GateInfos[1][GateIndex-1].fPeakGate = fPeakGate;
					}
					LeaveCriticalSection(&csGate);
					DeleteCriticalSection(&csGate);
					*/
					// songchenguang add end 2010-05-12
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					if(gbHotGateFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(!gbHotGateFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(!gbHotGateFlag)
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
				else if(wParam == 'B')	// 冻结
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
	return CallWindowProc(OldBtnGateFuZhiProc,Wnd,Msg,wParam,lParam);
}
