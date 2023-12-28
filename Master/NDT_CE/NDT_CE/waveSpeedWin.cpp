/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 声速窗口源文件
 * Filename: waveSpeedWin.cpp
 *
 * Spec: Trace Tag write here. If has not, ScreenCopy this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "displayWin.h"
#include "craftParamWin.h"
#include "sendParamWin.h"

HWND hWndWaveSpeed;//声速窗口句柄
HWND hBtnWaveSpeedType;//声速类型按钮句柄
HWND hBtnWaveSpeedValue;//声速值按钮句柄

HWND waveSpeedBtnList[2];//声速窗口的按钮表
WNDPROC OldBtnWaveSpeedTypeProc = NULL;//声速类型按钮默认处理过程
WNDPROC OldBtnWaveSpeedValueProc = NULL;//声速值按钮默认处理过程
HWND currentWaveSpeedBtn;//声速窗中当前选中的按钮

ATOM registerWaveSpeedChild(HINSTANCE hInstance);
static LRESULT CALLBACK waveSpeedChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int waveSpeed_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnWaveSpeedTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnWaveSpeedValueProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static void displayMaterialType(HWND hWnd, WORD wMaterialType);
static void displayWaveSpeed(HWND hWnd, WORD wWaveSpeed);
void ChangeWaveSpeed(const WORD& chNo);
//void SetWaveSpeedLabelAndExecute(HWND hWnd);
WORD gwMaterialType = 0;				//材料类型

static const int buttonNum = 2;			//按钮控件数
static WORD wWaveSpeed;					//声速
static WORD nWaveSpeedStep = 1;			//声速步长
static WORD nWaveSpeedStepIndex = 0;	//声速步长索引
WORD gwUserDefineWaveSpeed = 6000;  		//自定义声速	
extern HWND hWndDisplay;						//显示窗口句柄
extern HWND currentDisplayBtn;					//显示窗中当前选中的按钮
extern HINSTANCE hInst;							//程序实例句柄
extern HWND hWndMain;							//主窗口
extern HWND gCurrentFocusBtn;

extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern BOOL gbGuanLian;
extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//添加的样点数据队列
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][GATE_NUM];//已选中的DAC点数量
extern CURVEPOINT_DATA gSelectedDacPointData[CHANNEL_NUM][MAX_CURVE_POINT_NUM];//tcg
extern DWORD gSelectedDacPointDataNum[CHANNEL_NUM];//tcg
extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
extern ARS200FPGA_CH_GATE_REG *ch2_gate_reg;
extern ARS200FPGA_CH_DATA_REG *ch1_data_reg;
extern ARS200FPGA_CH_DATA_REG *ch2_data_reg;
extern CRITICAL_SECTION csGate;
extern GATE_INFO GateInfos[2][3];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern SEND_PARAM_DATA SendParam_ad[2];
extern CRITICAL_SECTION csRange;
extern float fRangeStart[2];
extern float fRangeEnd[2];
extern WORD CurrentChannelNo;
extern CRITICAL_SECTION csMinRange;
extern float gfMinRange[2];
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;
extern CRITICAL_SECTION csSysInfo;
extern SYS_INFO gSysInfo;

extern HWND gCurrentWnd;		//当前显示的子窗口
extern BOOL gbHotDB;			// 快捷键增益
extern BOOL gbHotRange;			// 快捷键声程
extern wchar_t lpDefaultParamFile[MAX_PATH];

extern HWND hWndChParam;			//测厚参数窗口句柄
extern HWND currentChParamBtn;		//测厚参数窗中当前选中的按钮
extern WORD gwDeviceType;	//设备类型 0:探伤仪	1:测厚仪
extern WORD wTanTouLeiXing[CHANNEL_NUM];//探头类型
extern CRITICAL_SECTION csProbeType;
/**************************
*注册声速窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerWaveSpeedChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) waveSpeedChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szWaveSpeedChild;

	return RegisterClass(&wc);
}
static void displayMaterialType(HWND hWnd, WORD wMaterialType)
{
	switch(wMaterialType)
	{
		case 1:
			DrawLableText(hWnd, _T("自定义"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			break;
		case 2:
			DrawLableText(hWnd, _T("钢"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			break;
		case 3:
			DrawLableText(hWnd, _T("铁"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			break;
		case 4:
			DrawLableText(hWnd, _T("铜"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			break;
		case 5:
			DrawLableText(hWnd, _T("陶瓷"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			break;
		case 6:
			DrawLableText(hWnd, _T("不锈钢"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			break;
		case 7:
			DrawLableText(hWnd, _T("铝"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			break;
		case 8:
			DrawLableText(hWnd, _T("黄铜"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			break;
		case 9:
			DrawLableText(hWnd, _T("锌"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			break;
		case 10:
			DrawLableText(hWnd, _T("环氧树脂"), 25, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			break;
		case 11:
			DrawLableText(hWnd, _T("聚氯乙烯"), 25, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			break;
		default:
			break;						
	}
}

static void displayWaveSpeed(HWND hWnd, WORD wWaveSpeed)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%d"), wWaveSpeed);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

void ChangeWaveSpeed(const WORD& chNo)
{
	// 声程
	SetRangeLableAndExecute(NULL, DisplaySet_ad[chNo].fRange, TRUE);

	// 闸门
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	if(chNo == 0)
	{
		// 前沿
		ch1_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][0].fFrontGate));
		ch1_gate_reg->wGate2Front = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][1].fFrontGate));
		ch1_gate_reg->wGate3Front = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][2].fFrontGate));
		// 后沿
		ch1_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][0].fBackGate));
		ch1_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][1].fBackGate));
		ch1_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][2].fBackGate));		
	}
	else if(chNo == 1)
	{
		// 前沿
		ch2_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][0].fFrontGate));
		ch2_gate_reg->wGate2Front = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][1].fFrontGate));
		ch2_gate_reg->wGate3Front = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][2].fFrontGate));
		// 后沿
		ch2_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][0].fBackGate));
		ch2_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][1].fBackGate));
		ch2_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, GateInfos[chNo][2].fBackGate));		
	}
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
/*	
	// 修正 + 延时
	float fTemp = SendParam_ad[chNo].nAmend;
	if(chNo == 0)
		ch1_data_reg->wMonitorZeroCounter = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, fTemp));
	else if(chNo == 1)
	{
		ch2_data_reg->wMonitorZeroCounter = WORD(Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, fTemp));
	}

	fTemp = ReceiveParam_ad[chNo].fDelay;
	if(chNo == 0)
		ch1_data_reg->wMonitorDelayCounter = WORD(500 + Range2DotNew(DisplaySet_ad[chNo].nWaveSpeed, fTemp));
	else if(chNo == 1)
	{
		ch2_data_reg->wMonitorDelayCounter = WORD(500 + Range2DotNew(DisplaySet_ad[chNo].nWaveSpeed, fTemp));
	}
	
	InitializeCriticalSection(&csRange);
	EnterCriticalSection(&csRange);
	float fRange = fRangeEnd[chNo] - fRangeStart[chNo];

	fRangeStart[chNo] = fTemp;
	fRangeEnd[chNo] = fRangeStart[chNo] + fRange;	

	LeaveCriticalSection(&csRange);
	DeleteCriticalSection(&csRange);
*/
	// TCG前后值
	getGain2TgcSram(gSelectedDacPointData[chNo], gSelectedDacPointDataNum[chNo], chNo);
/*
	//DAC
	for(int i = 0; i < 3; i++)
	{
		if(gSelectedDgsPointDataNum[CurrentChannelNo][i] >= 2)
		{
			write2GateSram(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], CurrentChannelNo, i);
		}
	}
*/
	// 更新最小声程值
	InitializeCriticalSection(&csMinRange);
	EnterCriticalSection(&csMinRange);

	gfMinRange[chNo] = Dot2Range(DisplaySet_ad[chNo].nWaveSpeed, MONITOR_SIZE);

	LeaveCriticalSection(&csMinRange);
	DeleteCriticalSection(&csMinRange);
}
WORD waveSpeedToType(WORD wWaveSpeed)
{
    WORD wMaterialType = 0;

	InitializeCriticalSection(&csProbeType);
	EnterCriticalSection(&csProbeType);

	if(wTanTouLeiXing[CurrentChannelNo] == 0)	//斜探头
	{
		if(wWaveSpeed == 3230)
		{
			wMaterialType = 2;	//钢
		}
		else if(wWaveSpeed == 3230)
		{
			wMaterialType = 3;	//
		}
		else if(wWaveSpeed == 2260)
		{
			wMaterialType = 4;	//
		}
		else if(wWaveSpeed == 3150)
		{
			wMaterialType = 5;	//
		}
		else if(wWaveSpeed == 3110)
		{
			wMaterialType = 6;	//
		}
		else if(wWaveSpeed == 3080)
		{
			wMaterialType = 7;	//
		}
		else if(wWaveSpeed == 2110)
		{
			wMaterialType = 8;	//
		}
		else if(wWaveSpeed == 2410)
		{
			wMaterialType = 9;	//
		}
		else if(wWaveSpeed == 1100)
		{
			wMaterialType = 10;	//
		}
		else if(wWaveSpeed == 980)
		{
			wMaterialType = 11;	//
		}
		else
		{
			wMaterialType = 1;	//自定义
		}
	}
	else if(wTanTouLeiXing[CurrentChannelNo] == 1)	//直探头
	{
		if(wWaveSpeed == 5920)
		{
			wMaterialType = 2;	//钢
		}
		else if(wWaveSpeed == 5930)
		{
			wMaterialType = 3;	//
		}
		else if(wWaveSpeed == 4720)
		{
			wMaterialType = 4;	//
		}
		else if(wWaveSpeed == 5842)
		{
			wMaterialType = 5;	//
		}
		else if(wWaveSpeed == 5740)
		{
			wMaterialType = 6;	//
		}
		else if(wWaveSpeed == 6260)
		{
			wMaterialType = 7;	//
		}
		else if(wWaveSpeed == 4399)
		{
			wMaterialType = 8;	//
		}
		else if(wWaveSpeed == 4170)
		{
			wMaterialType = 9;	//
		}
		else if(wWaveSpeed == 2540)
		{
			wMaterialType = 10;	//
		}
		else if(wWaveSpeed == 2388)
		{
			wMaterialType = 11;	//
		}
		else
		{
			wMaterialType = 1;	//自定义
		}
	}

	LeaveCriticalSection(&csProbeType);
	DeleteCriticalSection(&csProbeType);
	return wMaterialType;
}

WORD typeToWaveSpeed(WORD wType)
{
	WORD wWaveSpeed = 0;
	InitializeCriticalSection(&csProbeType);
	EnterCriticalSection(&csProbeType);

	if(wTanTouLeiXing[CurrentChannelNo] == 0)	//斜探头
	{
		if(wType == 2)//钢
		{
			wWaveSpeed = 3230;			
		}
		else if(wType == 3)
		{
			wWaveSpeed = 3230;
		}
		else if(wType == 4)
		{
			wWaveSpeed = 2260;
		}
		else if(wType == 5)
		{
			wWaveSpeed = 3150;
		}
		else if(wType == 6)
		{
			wWaveSpeed = 3110;
		}
		else if(wType == 7)
		{
			wWaveSpeed = 3080;
		}
		else if(wType == 8)
		{
			wWaveSpeed = 2110;
		}
		else if(wType == 9)
		{
			wWaveSpeed = 2410;
		}
		else if(wType == 10)
		{
			wWaveSpeed = 1100;
		}
		else if(wType == 11)
		{
			wWaveSpeed = 980;
		}
		else if(wType == 1)
		{
			wWaveSpeed = gwUserDefineWaveSpeed;
		}
	}
	else if(wTanTouLeiXing[CurrentChannelNo] == 1)	//直探头
	{
		if(wType == 2)//钢
		{
			wWaveSpeed = 5920;			
		}
		else if(wType == 3)
		{
			wWaveSpeed = 5930;
		}
		else if(wType == 4)
		{
			wWaveSpeed = 4720;
		}
		else if(wType == 5)
		{
			wWaveSpeed = 5842;
		}
		else if(wType == 6)
		{
			wWaveSpeed = 5740;
		}
		else if(wType == 7)
		{
			wWaveSpeed = 6260;
		}
		else if(wType == 8)
		{
			wWaveSpeed = 4399;
		}
		else if(wType == 9)
		{
			wWaveSpeed = 4170;
		}
		else if(wType == 10)
		{
			wWaveSpeed = 2540;
		}
		else if(wType == 11)
		{
			wWaveSpeed = 2388;
		}
		else if(wType == 1)
		{
			wWaveSpeed = gwUserDefineWaveSpeed;
		}
	}

	LeaveCriticalSection(&csProbeType);
	DeleteCriticalSection(&csProbeType);
	return wWaveSpeed;
}


void SetWaveSpeedLabelAndExecute(HWND hWnd)
{
	// 更新
	if(hWnd == hBtnWaveSpeedType)
	{
		displayMaterialType(hWnd, gwMaterialType);
			
		DisplaySet_ad[CurrentChannelNo].nWaveSpeed = typeToWaveSpeed(gwMaterialType);
		displayWaveSpeed(hBtnWaveSpeedValue, DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
	}
	else
	{
		displayWaveSpeed(hWnd, DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
		gwMaterialType = waveSpeedToType(DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
		displayMaterialType(hBtnWaveSpeedType, gwMaterialType);
							
		if(gwMaterialType == 1)
			gwUserDefineWaveSpeed = DisplaySet_ad[CurrentChannelNo].nWaveSpeed;
	}
	// 传值(所有和声速相关的都要做相应变化)...
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

	// 修改重复频率
	SetReFreqLableAndExecute(NULL);
	
	// 保存配置到默认文件
//	SaveParamFile(lpDefaultParamFile);
}
/**************************
*声速窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK waveSpeedChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			gwMaterialType = waveSpeedToType(DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
			if(gwMaterialType == 1)
				gwUserDefineWaveSpeed = DisplaySet_ad[CurrentChannelNo].nWaveSpeed;
			//画按钮控件
			hBtnWaveSpeedType = CreateWindow(_T("BUTTON"),_T("Type"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_WAVESPEED_TYPE,hInst,NULL);
			hBtnWaveSpeedValue = CreateWindow(_T("BUTTON"),_T("Value"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_WAVESPEED_VALUE,hInst,NULL);
			
			waveSpeedBtnList[0] = hBtnWaveSpeedType;
			waveSpeedBtnList[1] = hBtnWaveSpeedValue;
			//按钮控件的自定义处理过程
			OldBtnWaveSpeedTypeProc = (WNDPROC) GetWindowLong(hBtnWaveSpeedType,GWL_WNDPROC);
			SetWindowLong(hBtnWaveSpeedType,GWL_WNDPROC,(LONG)btnWaveSpeedTypeProc);
			OldBtnWaveSpeedValueProc = (WNDPROC) GetWindowLong(hBtnWaveSpeedValue,GWL_WNDPROC);
			SetWindowLong(hBtnWaveSpeedValue,GWL_WNDPROC,(LONG)btnWaveSpeedValueProc);

			currentWaveSpeedBtn = waveSpeedBtnList[0];//默认当前选中声速类型按钮
			SetFocus(currentWaveSpeedBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentWaveSpeedBtn;

			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndWaveSpeed, &ps);
			
			EndPaint(hWndWaveSpeed, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDC_WAVESPEED_TYPE)
			{
			/*	currentWaveSpeedBtn = waveSpeedBtnList[0]; 
				SetFocus(currentWaveSpeedBtn);
				gCurrentFocusBtn = currentWaveSpeedBtn;
				hWndType = CreateWindow(szTypeChild, TEXT(""), WS_CHILD|WS_VISIBLE,
						MENU_TYPE_X, MENU_TYPE_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
				ShowWindow(hWndWaveSpeed,SW_HIDE);
			//	SetFocus(hWndType);
				ShowWindow(hWndType,SW_SHOW);
				UpdateWindow(hWndType);
				*/
			}
			if(LOWORD(wParam) == IDC_WAVESPEED_VALUE)
			{
			/*	currentWaveSpeedBtn = waveSpeedBtnList[0]; 
				SetFocus(currentWaveSpeedBtn);
				gCurrentFocusBtn = currentWaveSpeedBtn;
				hWndValue = CreateWindow(szValueChild, TEXT(""), WS_CHILD|WS_VISIBLE,
						MENU_TYPE_X, MENU_TYPE_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
				ShowWindow(hWndWaveSpeed,SW_HIDE);
			//	SetFocus(hWndValue);
				ShowWindow(hWndValue,SW_SHOW);
				UpdateWindow(hWndValue);
				*/
			}
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			waveSpeed_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnWaveSpeedType)
			{
				gwMaterialType = waveSpeedToType(DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
				displayMaterialType(pdis->hwndItem, gwMaterialType);
			}
			else if(pdis->hwndItem == hBtnWaveSpeedValue)
			{
				displayWaveSpeed(pdis->hwndItem, DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
			}
			else
			{
				;//
			}

			// 检测按钮句柄，显示相应的步长
			memset(gszTextStep, 0, sizeof(gszTextStep));
			if(pdis->hwndItem == currentWaveSpeedBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnWaveSpeedValue)
						swprintf(gszTextStep, _T("%d"), nWaveSpeedStep);
					else
						swprintf(gszTextStep, _T(""));
				
					PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}
			break;
		case WM_CHAR:			
			InitializeCriticalSection(&csSysInfo);
			EnterCriticalSection(&csSysInfo);
			if( wParam == '\t' && gSysInfo.wWorkMode != OneSendOneReceive)
			{
				// 更新按钮项数据
				if(CurrentChannelNo == 0)
				{
					displayWaveSpeed(hBtnWaveSpeedValue, DisplaySet_ad[1].nWaveSpeed);
					gwMaterialType = waveSpeedToType(DisplaySet_ad[1].nWaveSpeed);
					displayMaterialType(hBtnWaveSpeedType, gwMaterialType);
				}
				else
				{
					displayWaveSpeed(hBtnWaveSpeedValue, DisplaySet_ad[0].nWaveSpeed);
					gwMaterialType = waveSpeedToType(DisplaySet_ad[0].nWaveSpeed);
					displayMaterialType(hBtnWaveSpeedType, gwMaterialType);
				}
			}

			LeaveCriticalSection(&csSysInfo);
			DeleteCriticalSection(&csSysInfo);

			SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			break;
	//	case WM_CTLCOLORSTATIC:
	//		return (LRESULT)hStaticBrush;
		case WM_CLOSE:		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载声速窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int waveSpeed_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_waveSpeedBtnBitmap[]=
	{
		{IDC_WAVESPEED_TYPE	,IDB_BITMAP_CAILIAO_NOSEL_R	 ,  IDB_BITMAP_CAILIAO_SELECT_R		},
		{IDC_WAVESPEED_VALUE	,IDB_BITMAP_SHENGSUZHI_NOSEL_R	 ,  IDB_BITMAP_SHENGSUZHI_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_waveSpeedBtnBitmap);
	return ReturnVal;
}
/**************************
*声速类型按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnWaveSpeedTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{	
					if(gwDeviceType == 0 || gwDeviceType == 2) //探伤或铁路
					{
						ShowWindow(hWndDisplay,SW_SHOW);
						SetFocus(currentDisplayBtn);//上级菜单最后选中的按钮获得输入焦点

						DestroyWindow(hWndWaveSpeed);
						hWndWaveSpeed = 0;

						gCurrentFocusBtn = currentDisplayBtn;

						gCurrentWnd = hWndDisplay;
					}
					else if(gwDeviceType == 1)
					{
						ShowWindow(hWndChParam,SW_SHOW);
						
						gCurrentFocusBtn = currentChParamBtn;
						SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点

						DestroyWindow(hWndWaveSpeed);

						gCurrentWnd = hWndChParam;
					}
					else
					{
						;//
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(waveSpeedBtnList[i] != currentWaveSpeedBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentWaveSpeedBtn = waveSpeedBtnList[i];
					SetFocus(currentWaveSpeedBtn);
					gCurrentFocusBtn = currentWaveSpeedBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(waveSpeedBtnList[i] != currentWaveSpeedBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentWaveSpeedBtn = waveSpeedBtnList[i];
					SetFocus(currentWaveSpeedBtn);
					gCurrentFocusBtn = currentWaveSpeedBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					gwMaterialType--;
					if(gwMaterialType == 0)
						gwMaterialType = 11;

					SetWaveSpeedLabelAndExecute(Wnd);
				}
				else if(wParam == VK_RIGHT)
				{
					gwMaterialType++;
					if(gwMaterialType == 12)
						gwMaterialType = 1;

					displayMaterialType(Wnd, gwMaterialType);
			
					DisplaySet_ad[CurrentChannelNo].nWaveSpeed = typeToWaveSpeed(gwMaterialType);
					displayWaveSpeed(hBtnWaveSpeedValue, DisplaySet_ad[CurrentChannelNo].nWaveSpeed);

					SetWaveSpeedLabelAndExecute(Wnd);
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
				else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnWaveSpeedTypeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*声速值按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnWaveSpeedValueProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			nWaveSpeedStep = WAVESPEEDSTEPS[nWaveSpeedStepIndex];
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					if(gwDeviceType == 0 || gwDeviceType == 2) //探伤或铁路
					{
						ShowWindow(hWndDisplay,SW_SHOW);
						SetFocus(currentDisplayBtn);//上级菜单最后选中的按钮获得输入焦点

						DestroyWindow(hWndWaveSpeed);
						hWndWaveSpeed = 0;

						gCurrentFocusBtn = currentDisplayBtn;

						gCurrentWnd = hWndDisplay;
					}
					else if(gwDeviceType == 1)
					{
						ShowWindow(hWndChParam,SW_SHOW);
						
						gCurrentFocusBtn = currentChParamBtn;
						SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点

						DestroyWindow(hWndWaveSpeed);
						hWndWaveSpeed = 0;

						gCurrentWnd = hWndChParam;
					}
					else
					{
						;//
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(waveSpeedBtnList[i] != currentWaveSpeedBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentWaveSpeedBtn = waveSpeedBtnList[i];
					SetFocus(currentWaveSpeedBtn);
					gCurrentFocusBtn = currentWaveSpeedBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(waveSpeedBtnList[i] != currentWaveSpeedBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentWaveSpeedBtn = waveSpeedBtnList[i];
					SetFocus(currentWaveSpeedBtn);
					gCurrentFocusBtn = currentWaveSpeedBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
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
				else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnWaveSpeedValueProc,Wnd,Msg,wParam,lParam);
}
