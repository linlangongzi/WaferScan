/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 复合材料检测窗口源文件
 * Filename: fnFuHeCaiLiaoJianCeWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
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
#include "recvParamWin.h"
#include "gateWin.h"
#include "craftParamWin.h"
#include "modeWin.h"

HWND hWndFuHeCaiLiaoJianCe = NULL;				//复合材料检测窗口句柄
HWND hBtnFuHeCaiLiaoJianCeThickness = NULL;		//钢板厚度按钮句柄
HWND hBtnFuHeCaiLiaoJianCeUpBlindArea = NULL;	//上盲区按钮句柄
HWND hBtnFuHeCaiLiaoJianCeDownBlindArea = NULL;	//下盲区按钮句柄
HWND hBtnFuHeCaiLiaoJianCeSensitive = NULL;		//检测灵敏度按钮句柄
HWND hBtnFuHeCaiLiaoJianCeSpeedChaoCha = NULL;			//声速超差百分比按钮句柄
HWND hBtnFuHeCaiLiaoJianCeWaveSpeed = NULL;		//声速按钮句柄


HWND FuHeCaiLiaoJianCeBtnList[6] = {NULL,NULL,NULL,NULL,NULL,NULL};							//复合材料检测窗口的按钮表
WNDPROC OldBtnFuHeCaiLiaoJianCeThicknessProc = NULL;		//钢板厚度按钮默认处理过程


WNDPROC OldBtnFuHeCaiLiaoJianCeSpeedChaoChaProc = NULL;			//声速超差百分比按钮默认处理过程
WNDPROC OldBtnFuHeCaiLiaoJianCeUpBlindAreaProc = NULL;	//上盲区按钮默认处理过程
WNDPROC OldBtnFuHeCaiLiaoJianCeDownBlindAreaProc = NULL;	//下盲区按钮默认处理过程
WNDPROC OldBtnFuHeCaiLiaoJianCeSensitiveProc = NULL;		//检测灵敏度按钮默认处理过程
WNDPROC OldBtnFuHeCaiLiaoJianCeWaveSpeedProc = NULL;		//声速按钮默认处理过程


 
HWND currentFuHeCaiLiaoJianCeBtn = NULL;					//复合材料检测窗中当前选中的按钮

static const int buttonNum = 6;		//按钮控件数
static float fDelayStep = 0.1;//AMENDSTEPS[0];
static WORD nDelayStepIndex = 0;			// 当前延时步长序号


ATOM registerFuHeCaiLiaoJianCeChild(HINSTANCE hInstance);
extern void displayDelay(HWND hWnd, float fDelay, const WORD& nType);
static LRESULT CALLBACK fuHeCaiLiaoJianCeChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int fuHeCaiLiaoJianCe_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnFuHeCaiLiaoJianCeThicknessProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnFuHeCaiLiaoJianCeSpeedChaoChaProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnFuHeCaiLiaoJianCeUpBlindAreaProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnFuHeCaiLiaoJianCeDownBlindAreaProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnFuHeCaiLiaoJianCeSensitiveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnFuHeCaiLiaoJianCeWaveSpeedProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

extern HINSTANCE hInst;		//程序实例句柄

extern HWND hWndMain;
extern HWND hWndSpecialFunction;
extern HWND currentSpecialFunctionBtn;
extern HWND gCurrentFocusBtn;

static WORD wFuHeCaiLiaoShengSu = 0;	//复合材料声速
static float fSpeedChaoCha = 0;	//声速超差
// songchenguang add start 2010-06-30
//static float fPlateHeight = 10.0f;
static float fUpBlindValue = 1.0f;
static float fDownBlindValue = 1.0f;
static float fPlateHeightStep = 1.0f;
//static float fBlindValueStep = 0.1f;
static float fUpBlindValueStep = 0.1f;	//上盲区步长
static WORD wUpBlindValueStepIndex = 1;	//上盲区步长序号
static float fDownBlindValueStep = 0.1f; //下盲区步长
static WORD wDownBlindValueStepIndex = 1; //下盲区步长序号
static WORD wPlateHeightStepIndex = 1;
static float fSpeedChaoChaStep = 1;	//声速超差步长
static WORD wSpeedChaoChaStepIndex = 1;	//声速超差步长序号

static TCHAR DACFileName[MAX_PATH];
extern TCHAR FileDirectory[MAX_PATH];
extern ARS200FPGA_CH_DAC_REG *chCurrent_dac_reg;
extern WORD *chCurrent_tgc;
extern HWND hStatusBar;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
extern ARS200FPGA_CH_GATE_REG *ch2_gate_reg;
extern WORD CurrentChannelNo;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern SEND_PARAM_DATA SendParam_ad[2];
extern float fPlusStep;
extern WORD nPlusStepIndex;
extern GATE_INFO GateInfos[2][3];
extern BOOL gbGuanLian;

extern float gCh1Plus;
extern float gCh2Plus;
extern ARS200FPGA_CH_PLUS_REG* ch1_plus_reg;
extern ARS200FPGA_CH_PLUS_REG* ch2_plus_reg;
extern wchar_t gszTextRefPlus[10];
extern ARS200FPGA_SYS_REG* sys_reg;
extern SYS_INFO gSysInfo;
extern CRITICAL_SECTION csSysInfo;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//当前显示的子窗口
extern wchar_t lpDefaultParamFile[MAX_PATH];

extern ARS200FPGA_CH_DATA_REG *chCurrent_data_reg;
extern ARS200FPGA_CH_CON_REG *chCurrent_con_reg;
extern ARS200FPGA_CH_PLUS_REG* chCurrent_plus_reg;

extern WORD *chCurrent_interval;
extern WORD *chCurrent_data;

extern WORD *chCurrent_gate1curve;
extern WORD *chCurrent_gate2curve;
extern WORD *chCurrent_gate3curve;
extern WORD *ch1_gate1curve;
extern WORD *ch1_gate2curve;
extern WORD *ch1_gate3curve;
extern wchar_t gszTextChannel[10];
extern wchar_t gszTextPlus[10];
extern ARS200FPGA_CH_DAC_REG* ch1_dac_reg;

extern WORD *ch1_interval;
extern WORD *ch1_data;
extern ARS200FPGA_CH_DATA_REG *ch1_data_reg;
extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
extern ARS200FPGA_CH_CON_REG *ch1_con_reg;
extern CRITICAL_SECTION csGate;
// songchenguang add end 2010-06-30
extern wchar_t gszTextStep[10];
extern BOOL gbHotRange;	// 快捷键声程
extern BOOL gbHotDB;		// 快捷键增益
extern float MIN_DELAY[2];
extern float MAX_DELAY;
extern PEAK_INFO gPeakDefectInfos[2][2];	//保存闸门峰点信息
extern CRITICAL_SECTION csPeakDefectInfos;
extern float gfPlateHeight;			//工件厚度
extern BOOL gbAlarmFlag;
extern BOOL gbChannelAlarmFlag[CHANNEL_NUM];			//闸门报警标志
extern CRITICAL_SECTION csAlarmFlag;
// songchenguang add end 2011-06-22
extern WORD MAX_PLUS;
// songchenguang add end 2011-06-22

extern void SetFnRefPlusLableAndExecute(HWND hWnd);

static void SetPlateHeight(HWND hWnd, const float& fHeight)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fHeight);
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	
}
static void SetBlindValue(HWND hWnd, const float& fValue)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fValue);

	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);	
}

/**************************
* 注册复合材料检测窗口
* @FuHeCaiLiaoJianCe    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerFuHeCaiLiaoJianCeChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) fuHeCaiLiaoJianCeChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szFuHeCaiLiaoJianCeChild;

	return RegisterClass(&wc);
}
//声速超差显示
static void displaySpeedChaoCha(HWND hWnd,float fChaoCha)
{
	wchar_t strText[10];
	memset(strText,0,sizeof(strText));
	wsprintf(strText,_T("%.1f%%"),fChaoCha);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	
}

//声速显示
static void displayShengSu(HWND hWnd,WORD wShengSu)
{
	wchar_t strText[10];
	memset(strText,0,sizeof(strText));
	wsprintf(strText,_T("%d"),wShengSu);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	
}
/** 
 * 采集点转换为时间
 * @param    dwGateDefectTime 点的值.
 * @return    float: Time.
 * @since     1.00
 */
float Dot2Time(DWORD dwGateDefectTime)
{
	float temp;
	temp = (float)(dwGateDefectTime/2/float(ADFREQUENCY)/1000000.0);
	return temp;
}
//复合材料声速定时器消息响应函数
void CALLBACK FuHeCaiLiaoTimerProc( HWND hwnd, 
							  UINT uMsg, 
							  UINT idEvent, 
							  DWORD dwTime 
							)
{
	PEAK_INFO peakDefectInfos[2][2];
	InitializeCriticalSection(&csPeakDefectInfos);
	EnterCriticalSection(&csPeakDefectInfos);
	for(int i = 0 ;i < CHANNEL_NUM; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			peakDefectInfos[i][j] = gPeakDefectInfos[i][j];
		}
	}
	LeaveCriticalSection(&csPeakDefectInfos);
	DeleteCriticalSection(&csPeakDefectInfos);
	wFuHeCaiLiaoShengSu = (WORD)((gfPlateHeight  / Dot2Time((DWORD)peakDefectInfos[CurrentChannelNo][1].wDefectTime)) / 1000);
						
	displayShengSu(hBtnFuHeCaiLiaoJianCeWaveSpeed, wFuHeCaiLiaoShengSu);

	BOOL bChannelAlarmFlag[CHANNEL_NUM];
	InitializeCriticalSection(&csAlarmFlag);
	EnterCriticalSection(&csAlarmFlag);
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		bChannelAlarmFlag[i] = gbChannelAlarmFlag[i];
	}
	LeaveCriticalSection(&csAlarmFlag);
	DeleteCriticalSection(&csAlarmFlag);

	if(wFuHeCaiLiaoShengSu > DisplaySet_ad[CurrentChannelNo].nWaveSpeed)//大于声速超差报警
	{
		if((((float)wFuHeCaiLiaoShengSu - DisplaySet_ad[CurrentChannelNo].nWaveSpeed) * 100 / DisplaySet_ad[CurrentChannelNo].nWaveSpeed) > fSpeedChaoCha)
		{
			if(gbAlarmFlag)
			{
				if(!bChannelAlarmFlag[0] && !bChannelAlarmFlag[1])	//无闸门报警时
				{
					sys_reg->wLedAlarm |= 0x1;
					//MessageBeep(0xFFFFFFFF);
					PlaySound(_T("SystemStart"),NULL,SND_ALIAS|SND_ASYNC); 
				}
			}
			else
			{
				sys_reg->wLedAlarm &= 0xFFFE;			
			}
		}
		else
		{
			if(!bChannelAlarmFlag[0] && !bChannelAlarmFlag[1])	//无闸门报警时
			{
				sys_reg->wLedAlarm &= 0xFFFE;
			}
		}
	}	
}

/**************************
*复合材料检测窗口处理过程
* @FuHeCaiLiaoJianCe    hInstance[in]:
* @FuHeCaiLiaoJianCe		message[in]:
* @FuHeCaiLiaoJianCe		wParam[in]:
* @FuHeCaiLiaoJianCe		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK fuHeCaiLiaoJianCeChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		{
			fPlateHeightStep = AMENDSTEPS[wPlateHeightStepIndex];
			fPlusStep = PLUSSTEPS[nPlusStepIndex];
			SetTimer(hWnd, TIMER_FUHECAILIAO_DISPLAY, 100, FuHeCaiLiaoTimerProc);
			//画按钮控件			
			hBtnFuHeCaiLiaoJianCeThickness = CreateWindow(_T("BUTTON"),_T("thicknessFuHeCaiLiaoJianCe"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_FUHECAILIAOJIANCE_THICKNESS,hInst,NULL);			
			hBtnFuHeCaiLiaoJianCeUpBlindArea = CreateWindow(_T("BUTTON"),_T("upBlindAreaFuHeCaiLiaoJianCe"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_FUHECAILIAOJIANCE_UPBLINDAREA,hInst,NULL);
			hBtnFuHeCaiLiaoJianCeDownBlindArea = CreateWindow(_T("BUTTON"),_T("downBlindAreaFuHeCaiLiaoJianCe"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_FUHECAILIAOJIANCE_DOWNBLINDAREA,hInst,NULL);
			hBtnFuHeCaiLiaoJianCeSensitive = CreateWindow(_T("BUTTON"),_T("upBlindAreaFuHeCaiLiaoJianCe"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_FUHECAILIAOJIANCE_SENSITIVE,hInst,NULL);
			hBtnFuHeCaiLiaoJianCeSpeedChaoCha = CreateWindow(_T("BUTTON"),_T("speedChaoChaFuHeCaiLiaoJianCe"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_FUHECAILIAOJIANCE_SPEEDCHAOCHA,hInst,NULL);
			hBtnFuHeCaiLiaoJianCeWaveSpeed= CreateWindow(_T("BUTTON"),_T("upBlindAreaFuHeCaiLiaoJianCe"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 5*BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_FUHECAILIAOJIANCE_WAVESPEED,hInst,NULL);

			FuHeCaiLiaoJianCeBtnList[0] = hBtnFuHeCaiLiaoJianCeThickness;
			FuHeCaiLiaoJianCeBtnList[1] = hBtnFuHeCaiLiaoJianCeUpBlindArea;
			FuHeCaiLiaoJianCeBtnList[2] = hBtnFuHeCaiLiaoJianCeDownBlindArea;
			FuHeCaiLiaoJianCeBtnList[3] = hBtnFuHeCaiLiaoJianCeSensitive;
			FuHeCaiLiaoJianCeBtnList[4] = hBtnFuHeCaiLiaoJianCeSpeedChaoCha;
			FuHeCaiLiaoJianCeBtnList[5] = hBtnFuHeCaiLiaoJianCeWaveSpeed;
			
			//按钮控件的自定义处理过程
			OldBtnFuHeCaiLiaoJianCeThicknessProc = (WNDPROC) GetWindowLong(hBtnFuHeCaiLiaoJianCeThickness,GWL_WNDPROC);
			SetWindowLong(hBtnFuHeCaiLiaoJianCeThickness,GWL_WNDPROC,(LONG)btnFuHeCaiLiaoJianCeThicknessProc);
			
			OldBtnFuHeCaiLiaoJianCeSpeedChaoChaProc = (WNDPROC) GetWindowLong(hBtnFuHeCaiLiaoJianCeSpeedChaoCha,GWL_WNDPROC);
			SetWindowLong(hBtnFuHeCaiLiaoJianCeSpeedChaoCha,GWL_WNDPROC,(LONG)btnFuHeCaiLiaoJianCeSpeedChaoChaProc);
			
			OldBtnFuHeCaiLiaoJianCeUpBlindAreaProc = (WNDPROC) GetWindowLong(hBtnFuHeCaiLiaoJianCeUpBlindArea, GWL_WNDPROC);
			SetWindowLong(hBtnFuHeCaiLiaoJianCeUpBlindArea,GWL_WNDPROC,(LONG)btnFuHeCaiLiaoJianCeUpBlindAreaProc);

			OldBtnFuHeCaiLiaoJianCeDownBlindAreaProc = (WNDPROC) GetWindowLong(hBtnFuHeCaiLiaoJianCeDownBlindArea, GWL_WNDPROC);
			SetWindowLong(hBtnFuHeCaiLiaoJianCeDownBlindArea,GWL_WNDPROC,(LONG)btnFuHeCaiLiaoJianCeDownBlindAreaProc);

			OldBtnFuHeCaiLiaoJianCeSensitiveProc = (WNDPROC) GetWindowLong(hBtnFuHeCaiLiaoJianCeSensitive, GWL_WNDPROC);
			SetWindowLong(hBtnFuHeCaiLiaoJianCeSensitive,GWL_WNDPROC,(LONG)btnFuHeCaiLiaoJianCeSensitiveProc);
			
			OldBtnFuHeCaiLiaoJianCeWaveSpeedProc = (WNDPROC) GetWindowLong(hBtnFuHeCaiLiaoJianCeWaveSpeed, GWL_WNDPROC);
			SetWindowLong(hBtnFuHeCaiLiaoJianCeWaveSpeed,GWL_WNDPROC,(LONG)btnFuHeCaiLiaoJianCeWaveSpeedProc);
			
			currentFuHeCaiLiaoJianCeBtn = FuHeCaiLiaoJianCeBtnList[0];//默认当前选中钢板厚度按钮
			SetFocus(currentFuHeCaiLiaoJianCeBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentFuHeCaiLiaoJianCeBtn;

			
			SetOpenFlag(NULL, TRUE, 1);//打开闸门1
			SetAlarmMode(NULL, 1, SuperHigh);
			SetPeakGate(NULL, 1, 50.0f, FALSE);
			// 设定闸门前沿 
			SetQianYan(NULL, 1, fUpBlindValue, FALSE);
			// 设定后沿
			SetHouYan(NULL, 1, (gfPlateHeight - fDownBlindValue), FALSE);

			SetOpenFlag(NULL, TRUE, 2);
			SetAlarmMode(NULL, 2, LostWave);
			SetPeakGate(NULL, 2, 50.0f, FALSE);
			// 设定闸门前沿 
			SetQianYan(NULL, 2, (gfPlateHeight - fDownBlindValue), FALSE);
			// 设定后沿
			SetHouYan(NULL, 2, (gfPlateHeight - fDownBlindValue + 10), FALSE);

			// 灵敏度设为10
			float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
			ReceiveParam_ad[CurrentChannelNo].fRefPlus = 10;
			SetFnRefPlusLableAndExecute(NULL);
			refGainChangeCurve(fOldRefPlus, TRUE);//wangbingfu add at 2011-03-09
			// 保存配置到默认文件
			SaveParamFile(lpDefaultParamFile);
		
			break;
		}
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			fuHeCaiLiaoJianCe_DrawButtonBitmap(hInst,pdis);

			// songchenguang add start 2010-06-18
			if(pdis->hwndItem == hBtnFuHeCaiLiaoJianCeThickness)
				SetPlateHeight(pdis->hwndItem, gfPlateHeight );
			else if(pdis->hwndItem == hBtnFuHeCaiLiaoJianCeSpeedChaoCha)
			{
				displaySpeedChaoCha(pdis->hwndItem, fSpeedChaoCha);
			}
			else if(pdis->hwndItem == hBtnFuHeCaiLiaoJianCeUpBlindArea)
				SetBlindValue(pdis->hwndItem,fUpBlindValue);
			else if(pdis->hwndItem == hBtnFuHeCaiLiaoJianCeDownBlindArea)
				SetBlindValue(pdis->hwndItem,fDownBlindValue);
			else if(pdis->hwndItem == hBtnFuHeCaiLiaoJianCeSensitive)
				SetFnRefPlusLableAndExecute(pdis->hwndItem);
			else if(pdis->hwndItem == hBtnFuHeCaiLiaoJianCeWaveSpeed)
				displayShengSu(pdis->hwndItem,wFuHeCaiLiaoShengSu);

			// songchenguang add end 2010-06-18

			// 检测按钮句柄，显示相应的步长
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentFuHeCaiLiaoJianCeBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnFuHeCaiLiaoJianCeThickness)
						swprintf(gszTextStep, _T("%.1f"), fPlateHeightStep);
					else if(pdis->hwndItem == hBtnFuHeCaiLiaoJianCeSpeedChaoCha)
						swprintf(gszTextStep, _T("%.1f"), fSpeedChaoChaStep);
					else if(pdis->hwndItem == hBtnFuHeCaiLiaoJianCeUpBlindArea)
						swprintf(gszTextStep, _T("%.1f"), fUpBlindValueStep);
					else if(pdis->hwndItem == hBtnFuHeCaiLiaoJianCeDownBlindArea)
						swprintf(gszTextStep, _T("%.1f"), fDownBlindValueStep);
					else
						swprintf(gszTextStep, _T(""));
					
					PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}
			break;
/*		case WM_CHAR:			
			if( wParam == '\t')
			{
				// 更新按钮项数据
				if(CurrentChannelNo == 0)
				{
					displayDelay(hBtnFuHeCaiLiaoJianCeDelay, ReceiveParam_ad[1].fDelay, 6);
				}
				else
				{
					displayDelay(hBtnFuHeCaiLiaoJianCeDelay, ReceiveParam_ad[0].fDelay, 6);
				}
			}
			SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			break;*/
		case WM_CLOSE:
		
			break;
		case WM_DESTROY:
			//wangbingfu add at 2010-09-07
//			GateInfos[0][0].bOpen = OFF;//关闭闸门线
//			GateInfos[1][0].bOpen = OFF;//关闭闸门线

			KillTimer(hWnd,TIMER_FUHECAILIAO_DISPLAY);
			sys_reg->wLedAlarm &= 0xFFFE; //2010-1-17
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载复合材料检测窗口按钮图片
* @FuHeCaiLiaoJianCe    hInstance[in]:
* @FuHeCaiLiaoJianCe		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int fuHeCaiLiaoJianCe_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;

	const BTN_BMP_INFO tbl_fuHeCaiLiaoJianCeBtnBitmap[]=
	{
		{IDC_FUHECAILIAOJIANCE_THICKNESS,		IDB_BITMAP_GANGBANHOUDU6_NOSEL_R,  	IDB_BITMAP_GANGBANHOUDU6_SELECT_R		},
		{IDC_FUHECAILIAOJIANCE_SPEEDCHAOCHA,	IDB_BITMAP_SHENGSUCHAOCHA_NOSEL_R ,  	IDB_BITMAP_SHENGSUCHAOCHA_SEL_R		},
		{IDC_FUHECAILIAOJIANCE_UPBLINDAREA,		IDB_BITMAP_SHANGMANGQU6_NOSEL_R ,  	IDB_BITMAP_SHANGMANGQU6_SELECT_R	},
		{IDC_FUHECAILIAOJIANCE_DOWNBLINDAREA,	IDB_BITMAP_XIAMANGQU6_NOSEL_R , 		IDB_BITMAP_XIAMANGQU6_SELECT_R	},
		{IDC_FUHECAILIAOJIANCE_SENSITIVE,		IDB_BITMAP_JIANCELINGMINDU6_NOSEL_R ,  IDB_BITMAP_JIANCELINGMINDU6_SELECT_R	},
		{IDC_FUHECAILIAOJIANCE_WAVESPEED,		IDB_BITMAP_DANGQIANSHENGSU_NOSEL_R ,  IDB_BITMAP_DANGQIANSHENGSU_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 
	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_fuHeCaiLiaoJianCeBtnBitmap);
	return ReturnVal;
}



/**************************
*钢板厚度按钮自定义处理过程
* @FuHeCaiLiaoJianCe    hInstance[in]:
* @FuHeCaiLiaoJianCe		message[in]:
* @FuHeCaiLiaoJianCe		wParam[in]:
* @FuHeCaiLiaoJianCe		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnFuHeCaiLiaoJianCeThicknessProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	/*	case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), fPlateHeightStep);
			PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			break;
		case WM_KILLFOCUS:
			swprintf(gszTextStep, _T(""));
			PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			break;
			*/
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndFuHeCaiLiaoJianCe);
					hWndFuHeCaiLiaoJianCe = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}			
				else if(wParam == VK_UP)
				{
					while(FuHeCaiLiaoJianCeBtnList[i] != currentFuHeCaiLiaoJianCeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFuHeCaiLiaoJianCeBtn = FuHeCaiLiaoJianCeBtnList[i];
					SetFocus(currentFuHeCaiLiaoJianCeBtn);
					gCurrentFocusBtn = currentFuHeCaiLiaoJianCeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(FuHeCaiLiaoJianCeBtnList[i] != currentFuHeCaiLiaoJianCeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFuHeCaiLiaoJianCeBtn = FuHeCaiLiaoJianCeBtnList[i];
					SetFocus(currentFuHeCaiLiaoJianCeBtn);
					gCurrentFocusBtn = currentFuHeCaiLiaoJianCeBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					float fCha = gfPlateHeight  - 5.0f;
					if(gfPlateHeight  > 5.0f && fCha > fPlateHeightStep)
						gfPlateHeight  -= fPlateHeightStep;
					else
						gfPlateHeight  = 5.0f;

					SetPlateHeight(hWnd, gfPlateHeight );

					// 设定闸门前沿 
					SetQianYan(NULL, 1, fUpBlindValue, FALSE);

					////////////////////////////////////////////////////////
					// 可以提取出来 songchenguang
					InitializeCriticalSection(&csGate);
					EnterCriticalSection(&csGate);
					float fLength = GateInfos[0][1].fBackGate - GateInfos[0][1].fFrontGate;
					float fG12Distance = GateInfos[0][1].fFrontGate - GateInfos[0][0].fBackGate;
					LeaveCriticalSection(&csGate);
					DeleteCriticalSection(&csGate);

					// 设定后沿
					SetHouYan(NULL, 1, (gfPlateHeight - fDownBlindValue), FALSE);					
					
					SetQianYan(NULL, 2, (gfPlateHeight - fDownBlindValue + fG12Distance), FALSE);
					SetHouYan(NULL, 2, (gfPlateHeight - fDownBlindValue  + fG12Distance + fLength), TRUE);
					///////////////////////////////////////////////////////
				}
				else if(wParam == VK_RIGHT)
				{
					float fCha = MAX_fPlateHeight - gfPlateHeight ;
					if(gfPlateHeight  < MAX_fPlateHeight && fCha > fPlateHeightStep)
						gfPlateHeight  += fPlateHeightStep;
					else
						gfPlateHeight  = MAX_fPlateHeight;

					SetPlateHeight(hWnd, gfPlateHeight );

					// 设定闸门前沿 
					SetQianYan(NULL, 1, fUpBlindValue, FALSE);

					////////////////////////////////////////////////////////
					// 可以提取出来 songchenguang
					InitializeCriticalSection(&csGate);
					EnterCriticalSection(&csGate);
					float fLength = GateInfos[0][1].fBackGate - GateInfos[0][1].fFrontGate;
					float fG12Distance = GateInfos[0][1].fFrontGate - GateInfos[0][0].fBackGate;
					LeaveCriticalSection(&csGate);
					DeleteCriticalSection(&csGate);

					// 设定后沿
					SetHouYan(NULL, 1, (gfPlateHeight - fDownBlindValue), FALSE);					
					
					SetQianYan(NULL, 2, (gfPlateHeight - fDownBlindValue + fG12Distance), FALSE);
					SetHouYan(NULL, 2, (gfPlateHeight - fDownBlindValue  + fG12Distance + fLength), TRUE);
					///////////////////////////////////////////////////////
				}
				else if(wParam == 'F')
				{
					if(wPlateHeightStepIndex < 3)
						wPlateHeightStepIndex++;
					else
						wPlateHeightStepIndex = 0;

					fPlateHeightStep = AMENDSTEPS[wPlateHeightStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fPlateHeightStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnFuHeCaiLiaoJianCeThicknessProc,hWnd,Msg,wParam,lParam);
}

/**************************
*声速超差百分比按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnFuHeCaiLiaoJianCeSpeedChaoChaProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			fSpeedChaoChaStep = RATESTEPS[wSpeedChaoChaStepIndex];
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndFuHeCaiLiaoJianCe);
					hWndFuHeCaiLiaoJianCe = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}
			
				else if(wParam == VK_UP)
				{
					while(FuHeCaiLiaoJianCeBtnList[i] != currentFuHeCaiLiaoJianCeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFuHeCaiLiaoJianCeBtn = FuHeCaiLiaoJianCeBtnList[i];
					SetFocus(currentFuHeCaiLiaoJianCeBtn);
					gCurrentFocusBtn = currentFuHeCaiLiaoJianCeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(FuHeCaiLiaoJianCeBtnList[i] != currentFuHeCaiLiaoJianCeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFuHeCaiLiaoJianCeBtn = FuHeCaiLiaoJianCeBtnList[i];
					SetFocus(currentFuHeCaiLiaoJianCeBtn);
					gCurrentFocusBtn = currentFuHeCaiLiaoJianCeBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(fSpeedChaoCha - fSpeedChaoChaStep > 0)
					{
						fSpeedChaoCha = fSpeedChaoCha - fSpeedChaoChaStep;
					}
					else
					{
						fSpeedChaoCha = 0;
					}

					displaySpeedChaoCha(hBtnFuHeCaiLiaoJianCeSpeedChaoCha, fSpeedChaoCha);
				}
				else if(wParam == VK_RIGHT)
				{
					if(fSpeedChaoCha + fSpeedChaoChaStep < 100)
					{
						fSpeedChaoCha = fSpeedChaoCha + fSpeedChaoChaStep;
					}
					else
					{
						fSpeedChaoCha = 100;
					}

					displaySpeedChaoCha(hBtnFuHeCaiLiaoJianCeSpeedChaoCha, fSpeedChaoCha);
				}
				else if(wParam == 'F')
				{
					if(wSpeedChaoChaStepIndex < 2)
						wSpeedChaoChaStepIndex++;
					else
						wSpeedChaoChaStepIndex = 0;

					fSpeedChaoChaStep = AMENDSTEPS[wSpeedChaoChaStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fSpeedChaoChaStep);
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
				else if(wParam == 'B')	// 冻结
				{
					PostMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				//songchenguang add end 2010-06-01
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnFuHeCaiLiaoJianCeSpeedChaoChaProc,Wnd,Msg,wParam,lParam);
}

/**************************
* 上盲区自定义处理过程
* @FuHeCaiLiaoJianCe    hInstance[in]:
* @FuHeCaiLiaoJianCe		message[in]:
* @FuHeCaiLiaoJianCe		wParam[in]:
* @FuHeCaiLiaoJianCe		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnFuHeCaiLiaoJianCeUpBlindAreaProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndFuHeCaiLiaoJianCe);
					hWndFuHeCaiLiaoJianCe = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}
			
				else if(wParam == VK_UP)
				{
					while(FuHeCaiLiaoJianCeBtnList[i] != currentFuHeCaiLiaoJianCeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFuHeCaiLiaoJianCeBtn = FuHeCaiLiaoJianCeBtnList[i];
					SetFocus(currentFuHeCaiLiaoJianCeBtn);
					gCurrentFocusBtn = currentFuHeCaiLiaoJianCeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(FuHeCaiLiaoJianCeBtnList[i] != currentFuHeCaiLiaoJianCeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFuHeCaiLiaoJianCeBtn = FuHeCaiLiaoJianCeBtnList[i];
					SetFocus(currentFuHeCaiLiaoJianCeBtn);
					gCurrentFocusBtn = currentFuHeCaiLiaoJianCeBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(fUpBlindValue > fUpBlindValueStep)
						fUpBlindValue -= fUpBlindValueStep;
					else
						fUpBlindValue = 0.0f;

					SetBlindValue(hWnd, fUpBlindValue);

					// 设定闸门前沿
					SetQianYan(NULL, 1, fUpBlindValue, TRUE);
				}
			
				else if(wParam == VK_RIGHT)
				{
					if(fUpBlindValue < (gfPlateHeight  - fDownBlindValue -fUpBlindValueStep))
						fUpBlindValue += fUpBlindValueStep;
					else
						fUpBlindValue = gfPlateHeight  - fDownBlindValue;

					SetBlindValue(hWnd, fUpBlindValue);

					// 设定闸门前沿 
					SetQianYan(NULL, 1, fUpBlindValue, TRUE);
				}

				else if(wParam == 'F')
				{
					if(wUpBlindValueStepIndex < 3)
						wUpBlindValueStepIndex ++;
					else
						wUpBlindValueStepIndex = 0;
					fUpBlindValueStep = AMENDSTEPS[wUpBlindValueStepIndex];
						
					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fUpBlindValueStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnFuHeCaiLiaoJianCeUpBlindAreaProc,hWnd,Msg,wParam,lParam);
}

/**************************
* 下盲区自定义处理过程
* @FuHeCaiLiaoJianCe    hInstance[in]:
* @FuHeCaiLiaoJianCe		message[in]:
* @FuHeCaiLiaoJianCe		wParam[in]:
* @FuHeCaiLiaoJianCe		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnFuHeCaiLiaoJianCeDownBlindAreaProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndFuHeCaiLiaoJianCe);
					hWndFuHeCaiLiaoJianCe = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}
			
				else if(wParam == VK_UP)
				{
					while(FuHeCaiLiaoJianCeBtnList[i] != currentFuHeCaiLiaoJianCeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFuHeCaiLiaoJianCeBtn = FuHeCaiLiaoJianCeBtnList[i];
					SetFocus(currentFuHeCaiLiaoJianCeBtn);
					gCurrentFocusBtn = currentFuHeCaiLiaoJianCeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(FuHeCaiLiaoJianCeBtnList[i] != currentFuHeCaiLiaoJianCeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFuHeCaiLiaoJianCeBtn = FuHeCaiLiaoJianCeBtnList[i];
					SetFocus(currentFuHeCaiLiaoJianCeBtn);
					gCurrentFocusBtn = currentFuHeCaiLiaoJianCeBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(fDownBlindValue > fDownBlindValueStep)
						fDownBlindValue -= fDownBlindValueStep;
					else
						fDownBlindValue = 0.0f;

					SetBlindValue(hWnd, fDownBlindValue);

					InitializeCriticalSection(&csGate);
					EnterCriticalSection(&csGate);
					float fLength = GateInfos[0][1].fBackGate - GateInfos[0][1].fFrontGate;
					float fG12Distance = GateInfos[0][1].fFrontGate - GateInfos[0][0].fBackGate;
					LeaveCriticalSection(&csGate);
					DeleteCriticalSection(&csGate);

					// 设定后沿
					SetHouYan(NULL, 1, (gfPlateHeight  - fDownBlindValue), FALSE);					
					
					SetQianYan(NULL, 2, (gfPlateHeight  - fDownBlindValue + fG12Distance), FALSE);
					SetHouYan(NULL, 2, (gfPlateHeight  - fDownBlindValue  + fG12Distance + fLength), TRUE);

				}
				else if(wParam == VK_RIGHT)
				{
					if(fDownBlindValue < (gfPlateHeight  - fUpBlindValue - fDownBlindValueStep))
						fDownBlindValue += fDownBlindValueStep;
					else
						fDownBlindValue = gfPlateHeight  - fUpBlindValue;

					SetBlindValue(hWnd, fDownBlindValue);

					InitializeCriticalSection(&csGate);
					EnterCriticalSection(&csGate);
					float fLength = GateInfos[0][1].fBackGate - GateInfos[0][1].fFrontGate;
					float fG12Distance = GateInfos[0][1].fFrontGate - GateInfos[0][0].fBackGate;
					LeaveCriticalSection(&csGate);
					DeleteCriticalSection(&csGate);

					// 设定后沿
					SetHouYan(NULL, 1, (gfPlateHeight  - fDownBlindValue), FALSE);					

					SetQianYan(NULL, 2, (gfPlateHeight  - fDownBlindValue + fG12Distance), FALSE);
					SetHouYan(NULL, 2, (gfPlateHeight  - fDownBlindValue  + fG12Distance + fLength), TRUE);
				}
				else if(wParam == 'F')
				{
					if(wDownBlindValueStepIndex < 3)
						wDownBlindValueStepIndex ++;
					else
						wDownBlindValueStepIndex = 0;
					fDownBlindValueStep = AMENDSTEPS[wDownBlindValueStepIndex];
						
					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fDownBlindValueStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnFuHeCaiLiaoJianCeDownBlindAreaProc, hWnd,Msg,wParam,lParam);
}

/**************************
* 探伤灵敏度自定义处理过程
* @FuHeCaiLiaoJianCe    hInstance[in]:
* @FuHeCaiLiaoJianCe		message[in]:
* @FuHeCaiLiaoJianCe		wParam[in]:
* @FuHeCaiLiaoJianCe		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnFuHeCaiLiaoJianCeSensitiveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndFuHeCaiLiaoJianCe);
					hWndFuHeCaiLiaoJianCe = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}
			
				else if(wParam == VK_UP)
				{
					while(FuHeCaiLiaoJianCeBtnList[i] != currentFuHeCaiLiaoJianCeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFuHeCaiLiaoJianCeBtn = FuHeCaiLiaoJianCeBtnList[i];
					SetFocus(currentFuHeCaiLiaoJianCeBtn);
					gCurrentFocusBtn = currentFuHeCaiLiaoJianCeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(FuHeCaiLiaoJianCeBtnList[i] != currentFuHeCaiLiaoJianCeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFuHeCaiLiaoJianCeBtn = FuHeCaiLiaoJianCeBtnList[i];
					SetFocus(currentFuHeCaiLiaoJianCeBtn);
					gCurrentFocusBtn = currentFuHeCaiLiaoJianCeBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
			
					// songchenguang add start 2010-04-22
					if(ReceiveParam_ad[CurrentChannelNo].fRefPlus > MIN_PLUS && 
						ReceiveParam_ad[CurrentChannelNo].fRefPlus > fPlusStep)
						ReceiveParam_ad[CurrentChannelNo].fRefPlus -= fPlusStep;
					else
						ReceiveParam_ad[CurrentChannelNo].fRefPlus = MIN_PLUS;

					SetFnRefPlusLableAndExecute(Wnd);
					// songchenguang add end 2010-04-22
					refGainChangeCurve(fOldRefPlus, TRUE);//wangbingfu add at 2011-03-09
				}
				else if(wParam == VK_RIGHT)
				{
					float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
					// songchenguang add start 2010-04-22
					if(ReceiveParam_ad[CurrentChannelNo].fRefPlus < (MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus))
						ReceiveParam_ad[CurrentChannelNo].fRefPlus += fPlusStep;
					else
						ReceiveParam_ad[CurrentChannelNo].fRefPlus = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus;

					SetFnRefPlusLableAndExecute(Wnd);
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
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnFuHeCaiLiaoJianCeSensitiveProc,Wnd,Msg,wParam,lParam);
}
/**************************
* 声速自定义处理过程
* @FuHeCaiLiaoJianCe    hInstance[in]:
* @FuHeCaiLiaoJianCe		message[in]:
* @FuHeCaiLiaoJianCe		wParam[in]:
* @FuHeCaiLiaoJianCe		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnFuHeCaiLiaoJianCeWaveSpeedProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndFuHeCaiLiaoJianCe);
					hWndFuHeCaiLiaoJianCe = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}
			
				else if(wParam == VK_UP)
				{
					while(FuHeCaiLiaoJianCeBtnList[i] != currentFuHeCaiLiaoJianCeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFuHeCaiLiaoJianCeBtn = FuHeCaiLiaoJianCeBtnList[i];
					SetFocus(currentFuHeCaiLiaoJianCeBtn);
					gCurrentFocusBtn = currentFuHeCaiLiaoJianCeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(FuHeCaiLiaoJianCeBtnList[i] != currentFuHeCaiLiaoJianCeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFuHeCaiLiaoJianCeBtn = FuHeCaiLiaoJianCeBtnList[i];
					SetFocus(currentFuHeCaiLiaoJianCeBtn);
					gCurrentFocusBtn = currentFuHeCaiLiaoJianCeBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnFuHeCaiLiaoJianCeWaveSpeedProc,Wnd,Msg,wParam,lParam);
}

