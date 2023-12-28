/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 铸件探伤窗口源文件
 * Filename: ZhuJianWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnZhuJianTanShangWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "gateWin.h"
#include "recvParamWin.h"
#include "NDT_CE.h"
#include "const.h"
#include "craftParamWin.h"
#include "modeWin.h"
#include "fnZhongHouBanTanShangWin.h"



HWND hWndZhuJianTanShang = NULL;						//铸件探伤窗口句柄
HWND hBtnZhuJianHouDu = NULL;	//工件厚度按钮句柄
HWND hBtnZhuJianUpBlindArea = NULL;			//上盲区按钮句柄
HWND hBtnZhuJianDownBlindArea = NULL;			//下盲区按钮句柄
HWND hBtnZhuJianSensitive = NULL;				//检测灵敏度按钮句柄
HWND hBtnZhuJianDefectZhi = NULL;				//Φ值按钮句柄

HWND zhuJianTanShangBtnList[5] = {NULL,NULL,NULL,NULL,NULL};					//铸件探伤窗口的按钮表
WNDPROC OldBtnZhuJianHouDuProc = NULL;//工件厚度按钮默认处理过程
WNDPROC OldBtnZhuJianUpBlindAreaProc = NULL;		//上盲区按钮默认处理过程
WNDPROC OldBtnZhuJianDownBlindAreaProc = NULL;		//下盲区按钮默认处理过程
WNDPROC OldBtnZhuJianSensitiveProc = NULL;			//检测灵敏度按钮默认处理过程
WNDPROC OldBtnZhuJianDefectZhiProc = NULL;			//Φ值按钮默认处理过程
HWND currentZhuJianBtn = NULL;					//铸件探伤窗中当前选中的按钮

static WORD buttonNum = 5;					//按钮控件数
static WORD wZhuJianPlateHeightStepIndex = 1;
static WORD wZhuJianSensitiveStepIndex = 1;
static float fUpBlindValueStep = 0.1f;	//上盲区步长
static WORD wUpBlindValueStepIndex = 1;	//上盲区步长序号
static float fDownBlindValueStep = 0.1f; //下盲区步长
static WORD wDownBlindValueStepIndex = 1; //下盲区步长序号


float gfZhuJianPlateHeight = 10.0f;
static float fZhuJianPlateHeightStep = 1.0f;
static float fZhuJianBlindValueStep = 0.1f;
static float fZhuJianSensitiveStep = 0.1f;

//float gfUpBlindValue = 1.0f;
//float gfDownBlindValue = 1.0f;
float gfZhuJianDefectZhiValue = 1.0f;


extern float gfUpBlindValue;//上盲区按钮显示值
extern float gfDownBlindValue;//下盲区按钮显示值
// songchenguang add end 2011-06-22
extern WORD MAX_PLUS;
// songchenguang add end 2011-06-22
extern HINSTANCE hInst;					//程序实例句柄
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;	
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;				//当前显示的子窗口	
extern HWND hWndSpecialFunction;		//专用功能窗口句柄
extern HWND currentSpecialFunctionBtn;	//专用功能窗中当前选中的按钮

extern float gCh1Plus;
extern float gCh2Plus;
extern BOOL gbGuanLian;
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];

extern WORD CurrentChannelNo;
extern ARS200FPGA_CH_PLUS_REG* ch1_plus_reg;
extern ARS200FPGA_CH_PLUS_REG* ch2_plus_reg;
extern HWND hStatusBar;
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern float fPlusStep;
extern ARS200FPGA_SYS_REG* sys_reg;
extern SYS_INFO gSysInfo;
extern CRITICAL_SECTION csSysInfo;

extern wchar_t gszTextStep[10];
extern BOOL gbHotRange;	// 快捷键声程
extern BOOL gbHotDB;		// 快捷键增益
extern GATE_INFO GateInfos[2][3];
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern PEAK_INFO gPeakDefectInfos[2][2];	//保存闸门峰值点信息

extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//添加的样点数据队列
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//已选中的AVG点数量
extern CRITICAL_SECTION csPeakDefectInfos;

extern ARS200FPGA_CH_DATA_REG *chCurrent_data_reg;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern ARS200FPGA_CH_CON_REG *chCurrent_con_reg;
extern ARS200FPGA_CH_PLUS_REG* chCurrent_plus_reg;
extern ARS200FPGA_CH_DAC_REG* chCurrent_dac_reg;
extern WORD *chCurrent_gate1curve;
extern WORD *chCurrent_gate2curve;
extern WORD *chCurrent_gate3curve;
extern wchar_t gszTextChannel[10];
extern wchar_t gszTextPlus[10];
extern wchar_t gszTextRefPlus[10];

extern WORD *ch2_gate1curve;
extern WORD *ch2_gate2curve;
extern WORD *ch2_gate3curve;

extern ARS200FPGA_CH_CON_REG *ch2_con_reg;
extern ARS200FPGA_CH_DAC_REG* ch2_dac_reg;
extern ARS200FPGA_CH_GATE_REG *ch2_gate_reg;
extern ARS200FPGA_CH_DATA_REG *ch2_data_reg;
extern WORD *ch2_interval;
extern WORD *ch1_data;
extern WORD *ch2_data;

extern WORD *chCurrent_interval;
extern WORD *chCurrent_data;
extern float gfPlateHeight;		//工件厚度

extern BOOL gbAlarmFlag;
extern BOOL gbChannelAlarmFlag[CHANNEL_NUM];			//闸门报警标志
extern CRITICAL_SECTION csAlarmFlag;

/**************************
*注册铸件探伤窗口
* @ZhuJian    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerZhuJianChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) zhuJianChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szZhuJianChild;

	return RegisterClass(&wc);
}
void ZhuJianFaiZhiValue(HWND hWnd,float fFaiZhi)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fFaiZhi);

	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

	
}

void CALLBACK ZhuJianFaiZhiTimerProc( HWND hwnd, 
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
	gfZhuJianDefectZhiValue = calculateFaiZhi(WORD(peakDefectInfos[CurrentChannelNo][0].wDefectAmp), DWORD(peakDefectInfos[CurrentChannelNo][0].wDefectTime),
						gSelectedAvgPointData[CurrentChannelNo][0], WORD(gSelectedAvgPointDataNum[CurrentChannelNo]));

	ZhuJianFaiZhiValue(hBtnZhuJianDefectZhi,gfZhuJianDefectZhiValue);

	BOOL bChannelAlarmFlag[CHANNEL_NUM];
	InitializeCriticalSection(&csAlarmFlag);
	EnterCriticalSection(&csAlarmFlag);
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		bChannelAlarmFlag[i] = gbChannelAlarmFlag[i];
	}
	LeaveCriticalSection(&csAlarmFlag);
	DeleteCriticalSection(&csAlarmFlag);
	DWORD dwAmpPlus12DB = (DWORD)(peakDefectInfos[CurrentChannelNo][0].wDefectAmp / pow(10, (double)12 / (-20))); //底波增加12DB
	if((dwAmpPlus12DB / MAX_HEIGHT) < GateInfos[CurrentChannelNo][0].fPeakGate) //比设定值低12DB报警
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

void ZhuJianSetPlateHeight(HWND hWnd, const float& fHeight)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fHeight);
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);	
}
void ZhuJianSetBlindValue(HWND hWnd, const float& fValue)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fValue);

	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

	
}
void ZhuJianSetFnRefPlusLableAndExecute(HWND hWnd)
{
	if(hWnd)
	{
		// 更新
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		wsprintf(strText, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	}
	// 传值
	if(!gbGuanLian)
	{
		WORD wTemp0 = 0;
		WORD wTemp1 = 0;
		WORD wTemp2 = 0;
		WORD wTemp3 = 0;

		float fTotalPlus = ReceiveParam_ad[CurrentChannelNo].fPlus + ReceiveParam_ad[CurrentChannelNo].fRefPlus;
		dBProcess(fTotalPlus, wTemp0, wTemp1, wTemp2, wTemp3);

		chCurrent_plus_reg->wdBRef = wTemp0;
		PlusChangeTurn(CurrentChannelNo);

		chCurrent_plus_reg->wdBLevel1 = wTemp1;
		PlusChangeTurn(CurrentChannelNo);

		chCurrent_plus_reg->wdBLevel2 = wTemp2;
		PlusChangeTurn(CurrentChannelNo);

		chCurrent_plus_reg->wdBLevel3 = wTemp3;
		PlusChangeTurn(CurrentChannelNo);

		if(CurrentChannelNo == 0)
			gCh1Plus = fTotalPlus;
		else if(CurrentChannelNo == 1)
			gCh2Plus = fTotalPlus;
	}
	else
	{
		DB_ITEM dB1;
		memset(&dB1, 0, sizeof(DB_ITEM));
		DB_ITEM dB2;
		memset(&dB2, 0, sizeof(DB_ITEM));

		GuanLianDBProcess(ReceiveParam_ad[CurrentChannelNo].fPlus,
						  ReceiveParam_ad[CurrentChannelNo].fRefPlus,
						  dB1,
						  dB2);

		ch1_plus_reg->wdBRef = dB1.wDB_A;
		PlusChangeTurn(0);	

		ch1_plus_reg->wdBLevel1 = dB1.wDB_B;
		PlusChangeTurn(0);

		ch1_plus_reg->wdBLevel2 = dB1.wDB_C;
		PlusChangeTurn(0);

		ch1_plus_reg->wdBLevel3 = dB1.wDB_D;
		PlusChangeTurn(0);

		ch2_plus_reg->wdBRef = dB2.wDB_A;
		PlusChangeTurn(1);	

		ch2_plus_reg->wdBLevel1 = dB2.wDB_B;
		PlusChangeTurn(1);

		ch2_plus_reg->wdBLevel2 = dB2.wDB_C;
		PlusChangeTurn(1);

		ch2_plus_reg->wdBLevel3 = dB2.wDB_D;
		PlusChangeTurn(1);

		ReceiveParam_ad[0].fRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
		ReceiveParam_ad[1].fRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
	}

	memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
	swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);

	// 保存配置到默认文件
	SaveParamFile(lpDefaultParamFile);
}



/**************************
*铸件探伤窗口处理过程
* @ZhuJian    hInstance[in]:
* @ZhuJian		message[in]:
* @ZhuJian		wParam[in]:
* @ZhuJian		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK zhuJianChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

//			closeAllGate();//关闭所有闸门
//			GateInfos[0][0].bOpen = ON; //画通道1闸门1线
//			GateInfos[1][0].bOpen = ON; //画通道2闸门线
			SetTimer(hWnd, TIMER_ZHUJIAN_FAIZHI_DISPLAY, 30, ZhuJianFaiZhiTimerProc);
			
			hBtnZhuJianHouDu = CreateWindow(_T("BUTTON"),_T("houDuZhuJian"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHUJIAN_HOUDU,hInst,NULL);
			hBtnZhuJianUpBlindArea = CreateWindow(_T("BUTTON"),_T("upBlindAreaZhuJian"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHUJIAN_UPBLINDAREA,hInst,NULL);
			hBtnZhuJianDownBlindArea = CreateWindow(_T("BUTTON"),_T("downBlindAreaZhuJian"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHUJIAN_DOWNBLINDAREA,hInst,NULL);
			hBtnZhuJianSensitive = CreateWindow(_T("BUTTON"),_T("sensitiveZhuJian"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHUJIAN_SENSITIVE,hInst,NULL);
			hBtnZhuJianDefectZhi = CreateWindow(_T("BUTTON"),_T("defectZhiZhuJian"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHUJIAN_DEFECTZHI,hInst,NULL);
				
			zhuJianTanShangBtnList[0] = hBtnZhuJianHouDu;
			zhuJianTanShangBtnList[1] = hBtnZhuJianUpBlindArea;
			zhuJianTanShangBtnList[2] = hBtnZhuJianDownBlindArea;
			zhuJianTanShangBtnList[3] = hBtnZhuJianSensitive;
			zhuJianTanShangBtnList[4] = hBtnZhuJianDefectZhi;
			//按钮控件的自定义处理过程
			OldBtnZhuJianHouDuProc = (WNDPROC) GetWindowLong(hBtnZhuJianHouDu,GWL_WNDPROC);
			SetWindowLong(hBtnZhuJianHouDu,GWL_WNDPROC,(LONG)btnZhuJianHouDuProc);
			
			OldBtnZhuJianUpBlindAreaProc = (WNDPROC) GetWindowLong(hBtnZhuJianUpBlindArea,GWL_WNDPROC);
			SetWindowLong(hBtnZhuJianUpBlindArea,GWL_WNDPROC,(LONG)btnZhuJianUpBlindAreaProc);
			
			OldBtnZhuJianDownBlindAreaProc = (WNDPROC) GetWindowLong(hBtnZhuJianDownBlindArea,GWL_WNDPROC);
			SetWindowLong(hBtnZhuJianDownBlindArea,GWL_WNDPROC,(LONG)btnZhuJianDownBlindAreaProc);
			
			OldBtnZhuJianSensitiveProc = (WNDPROC) GetWindowLong(hBtnZhuJianSensitive,GWL_WNDPROC);
			SetWindowLong(hBtnZhuJianSensitive,GWL_WNDPROC,(LONG)btnZhuJianSensitiveProc);

			OldBtnZhuJianDefectZhiProc = (WNDPROC) GetWindowLong(hBtnZhuJianDefectZhi,GWL_WNDPROC);
			SetWindowLong(hBtnZhuJianDefectZhi,GWL_WNDPROC,(LONG)btnZhuJianDefectZhiProc);
		
			currentZhuJianBtn = zhuJianTanShangBtnList[0];//默认当前选中工件厚度按钮			
			SetFocus(currentZhuJianBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentZhuJianBtn;

			SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)DanJingGuanLian);
					
			sys_reg->wWorkMode = DanJingGuanLian;

			InitializeCriticalSection(&csSysInfo);
			EnterCriticalSection(&csSysInfo);

			gSysInfo.wRelay &= 0xFFFE;
			gSysInfo.wRelay |= 0x2;
			//yachang.huang rum on 2013-11-19
			//sys_reg->wRelay = gSysInfo.wRelay;

			gSysInfo.wWorkMode = DanJingGuanLian;
			LeaveCriticalSection(&csSysInfo);
			DeleteCriticalSection(&csSysInfo);
			
			SetRedrawFlag(TRUE);
			gbGuanLian = TRUE;

//					Switch2Ch1();
			////////////					
			CurrentChannelNo = 1;
			chCurrent_data_reg = ch2_data_reg;
			chCurrent_gate_reg = ch2_gate_reg;
			chCurrent_con_reg = ch2_con_reg;
			chCurrent_interval = ch2_interval;
			chCurrent_data = ch2_data;

			chCurrent_plus_reg = ch2_plus_reg;
			chCurrent_dac_reg = ch2_dac_reg;

			chCurrent_gate1curve = ch2_gate1curve;
			chCurrent_gate2curve = ch2_gate2curve;
			chCurrent_gate3curve = ch2_gate3curve;

			memset(gszTextChannel, 0, sizeof(gszTextChannel));

			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

			memset(gszTextPlus, 0, sizeof(gszTextPlus));
			swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

			memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
			swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
			////

			Ch1Data2Ch2Data();
			InitParamDetail();
			
			// 设定闸门前沿 
			SetQianYan(NULL, 1, gfUpBlindValue, FALSE);
			// 设定后沿
			SetHouYan(NULL, 1, (gfPlateHeight - gfDownBlindValue), FALSE);

			// 设定幅值
			SetPeakGate(NULL, 1, 50.0f, TRUE);

			SetOpenFlag(NULL, TRUE, 1);

			// 设定闸门前沿 
			SetQianYan(NULL, 2, (gfPlateHeight - gfDownBlindValue), FALSE);
			// 设定后沿
			SetHouYan(NULL, 2, (gfPlateHeight + 10), FALSE);

			// 设定幅值
			SetPeakGate(NULL, 2, 50.0f, TRUE);

			SetOpenFlag(NULL, TRUE, 2);

			// 保存配置到默认文件
//			SaveParamFile(lpDefaultParamFile);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndSpecialFunction,SW_SHOW);
				SetFocus(currentSpecialFunctionBtn);//上级菜单最后选中的按钮获得输入焦点

				DestroyWindow(hWndZhuJianTanShang);
				hWndZhuJianTanShang = 0;
				
				gCurrentFocusBtn = currentSpecialFunctionBtn;

				gCurrentWnd = hWndSpecialFunction;

				KillTimer(hWnd, TIMER_ZHUJIAN_FAIZHI_DISPLAY);
			}
			break;
		case WM_COMMAND:
				
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			zhuJian_DrawButtonBitmap(hInst,pdis);

		
			if(pdis->hwndItem == hBtnZhuJianHouDu)
				ZhuJianSetPlateHeight(pdis->hwndItem, gfPlateHeight);
	
			else if(pdis->hwndItem == hBtnZhuJianUpBlindArea)
				ZhuJianSetBlindValue(pdis->hwndItem,gfUpBlindValue);
			else if(pdis->hwndItem == hBtnZhuJianDownBlindArea)
				ZhuJianSetBlindValue(pdis->hwndItem,gfDownBlindValue);
			else if(pdis->hwndItem == hBtnZhuJianSensitive)
				ZhuJianSetFnRefPlusLableAndExecute(pdis->hwndItem);
			else if(pdis->hwndItem == hBtnZhuJianDefectZhi)
				ZhuJianFaiZhiValue(pdis->hwndItem,gfZhuJianDefectZhiValue);

			//显示步长
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentZhuJianBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnZhuJianHouDu)
						swprintf(gszTextStep, _T("%.1f"), fZhuJianPlateHeightStep);
					else if(pdis->hwndItem == hBtnZhuJianSensitive)
						swprintf(gszTextStep, _T("%.1f"), fZhuJianSensitiveStep);
					else if(pdis->hwndItem == hBtnZhuJianUpBlindArea)
						swprintf(gszTextStep, _T("%.1f"), fUpBlindValueStep);
					else if(pdis->hwndItem == hBtnZhuJianDownBlindArea)
						swprintf(gszTextStep, _T("%.1f"), fDownBlindValueStep);
					else
						swprintf(gszTextStep, _T(""));
					
					PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}
			break;
		case WM_CLOSE:
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载铸件探伤窗口按钮图片
* @ZhuJian    hInstance[in]:
* @ZhuJian		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int zhuJian_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;

	BTN_BMP_INFO tbl_ZhuJianBtnBitmap[]=
	{
		{IDC_ZHUJIAN_HOUDU	,IDB_BITMAP_GANGBANHOUDU5_NOSEL_R	 ,  IDB_BITMAP_GANGBANHOUDU5_SELECT_R		},
		{IDC_ZHUJIAN_UPBLINDAREA	,IDB_BITMAP_SHANGMANGQU_NOSEL_R	 ,  IDB_BITMAP_SHANGMANGQU_SELECT_R		},
		{IDC_ZHUJIAN_DOWNBLINDAREA	,IDB_BITMAP_XIAMANGQU_NOSEL_R ,  IDB_BITMAP_XIAMANGQU_SELECT_R	},
		{IDC_ZHUJIAN_SENSITIVE	,IDB_BITMAP_JIANCELINGMINDU_NOSEL_R ,  IDB_BITMAP_JIANCELINGMINDU_SELECT_R	},
		{IDC_ZHUJIAN_DEFECTZHI	,IDB_BITMAP_FAIZHI_NOSEL_R ,IDB_BITMAP_FAIZHI_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	};
		
	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_ZhuJianBtnBitmap);
		
	return ReturnVal;
}
/**************************
*工件厚度按钮自定义处理过程
* @ZhuJian    hInstance[in]:
* @ZhuJian		message[in]:
* @ZhuJian		wParam[in]:
* @ZhuJian		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnZhuJianHouDuProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(hWnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(zhuJianTanShangBtnList[i] != currentZhuJianBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentZhuJianBtn = zhuJianTanShangBtnList[i];
					SetFocus(currentZhuJianBtn);
					gCurrentFocusBtn = currentZhuJianBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(zhuJianTanShangBtnList[i] != currentZhuJianBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentZhuJianBtn = zhuJianTanShangBtnList[i];
					SetFocus(currentZhuJianBtn);
					gCurrentFocusBtn = currentZhuJianBtn;
				}
				else if(wParam == VK_LEFT)
				{
					float fCha = gfPlateHeight - 5.0f;
					if(gfPlateHeight > 5.0f && fCha > fZhuJianPlateHeightStep)
						gfPlateHeight -= fZhuJianPlateHeightStep;
					else
						gfPlateHeight = 5.0f;

					ZhuJianSetPlateHeight(hWnd, gfPlateHeight);
					// 设定闸门前沿 
					SetQianYan(NULL, 1, gfUpBlindValue, FALSE);
					
					// 设定下盲区改变后闸门变化
					SetGuanLianDownBlindAreaGate();
				}
				else if(wParam == VK_RIGHT)
				{
					float fCha = MAX_fPlateHeight - gfPlateHeight;
					if(gfPlateHeight < MAX_fPlateHeight && fCha > fZhuJianPlateHeightStep)
						gfPlateHeight += fZhuJianPlateHeightStep;
					else
						gfPlateHeight = MAX_fPlateHeight;

					ZhuJianSetPlateHeight(hWnd, gfPlateHeight);

					// 设定闸门前沿 
					SetQianYan(NULL, 1, gfUpBlindValue, FALSE);

					// 设定下盲区改变后闸门变化
					SetGuanLianDownBlindAreaGate();
				}
				else if(wParam == 'F')
				{
					if(wZhuJianPlateHeightStepIndex < 3)
						wZhuJianPlateHeightStepIndex++;
					else
						wZhuJianPlateHeightStepIndex = 0;

					fZhuJianPlateHeightStep = AMENDSTEPS[wZhuJianPlateHeightStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fZhuJianPlateHeightStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnZhuJianHouDuProc,hWnd,Msg,wParam,lParam);
}
/**************************
*上盲区按钮自定义处理过程
* @ZhuJian    hInstance[in]:
* @ZhuJian		message[in]:
* @ZhuJian		wParam[in]:
* @ZhuJian		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnZhuJianUpBlindAreaProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(hWnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(zhuJianTanShangBtnList[i] != currentZhuJianBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentZhuJianBtn = zhuJianTanShangBtnList[i];
					SetFocus(currentZhuJianBtn);
					gCurrentFocusBtn = currentZhuJianBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(zhuJianTanShangBtnList[i] != currentZhuJianBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentZhuJianBtn = zhuJianTanShangBtnList[i];
					SetFocus(currentZhuJianBtn);
					gCurrentFocusBtn = currentZhuJianBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if( gfUpBlindValue > fUpBlindValueStep)
						gfUpBlindValue -= fUpBlindValueStep;
					else 
						gfUpBlindValue = 0.0f;

					ZhuJianSetBlindValue(hWnd, gfUpBlindValue);

					// 设定闸门前沿 
					SetQianYan(NULL, 1, gfUpBlindValue, TRUE);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfUpBlindValue < (gfPlateHeight - gfDownBlindValue - fUpBlindValueStep))
						gfUpBlindValue += fUpBlindValueStep;
					else
						gfUpBlindValue = gfPlateHeight - gfDownBlindValue;

					ZhuJianSetBlindValue(hWnd, gfUpBlindValue);

					// 设定闸门前沿 
					SetQianYan(NULL, 1, gfUpBlindValue, TRUE);
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
	return CallWindowProc(OldBtnZhuJianUpBlindAreaProc,hWnd,Msg,wParam,lParam);
}
/**************************
*下盲区按钮自定义处理过程
* @ZhuJian    hInstance[in]:
* @ZhuJian		message[in]:
* @ZhuJian		wParam[in]:
* @ZhuJian		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnZhuJianDownBlindAreaProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(hWnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(zhuJianTanShangBtnList[i] != currentZhuJianBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentZhuJianBtn = zhuJianTanShangBtnList[i];
					SetFocus(currentZhuJianBtn);
					gCurrentFocusBtn = currentZhuJianBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(zhuJianTanShangBtnList[i] != currentZhuJianBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentZhuJianBtn = zhuJianTanShangBtnList[i];
					SetFocus(currentZhuJianBtn);
					gCurrentFocusBtn = currentZhuJianBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if( gfDownBlindValue > fDownBlindValueStep)
						gfDownBlindValue -= fDownBlindValueStep;
					else 
						gfDownBlindValue = 0;

					ZhuJianSetBlindValue(hWnd, gfDownBlindValue);

					// 设定下盲区改变后闸门变化
					SetGuanLianDownBlindAreaGate();
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfDownBlindValue < (gfPlateHeight - gfUpBlindValue -fDownBlindValueStep))
						gfDownBlindValue += fDownBlindValueStep;
					else
						gfDownBlindValue = gfPlateHeight - gfUpBlindValue;

					ZhuJianSetBlindValue(hWnd, gfDownBlindValue);

					// 设定下盲区改变后闸门变化
					SetGuanLianDownBlindAreaGate();
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
	return CallWindowProc(OldBtnZhuJianDownBlindAreaProc,hWnd,Msg,wParam,lParam);
}
/**************************
*检测灵敏度按钮自定义处理过程
* @ZhuJian    hInstance[in]:
* @ZhuJian		message[in]:
* @ZhuJian		wParam[in]:
* @ZhuJian		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnZhuJianSensitiveProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(hWnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(zhuJianTanShangBtnList[i] != currentZhuJianBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentZhuJianBtn = zhuJianTanShangBtnList[i];
					SetFocus(currentZhuJianBtn);
					gCurrentFocusBtn = currentZhuJianBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(zhuJianTanShangBtnList[i] != currentZhuJianBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentZhuJianBtn = zhuJianTanShangBtnList[i];
					SetFocus(currentZhuJianBtn);
					gCurrentFocusBtn = currentZhuJianBtn;
				}
				else if(wParam == VK_LEFT)
				{
					
					if(ReceiveParam_ad[CurrentChannelNo].fRefPlus > MIN_PLUS && 
						ReceiveParam_ad[CurrentChannelNo].fRefPlus > fZhuJianSensitiveStep)
						ReceiveParam_ad[CurrentChannelNo].fRefPlus -= fZhuJianSensitiveStep;
					else
						ReceiveParam_ad[CurrentChannelNo].fRefPlus = MIN_PLUS;

					ZhuJianSetFnRefPlusLableAndExecute(hWnd);
				
				}
				else if(wParam == VK_RIGHT)
				{
					if(ReceiveParam_ad[CurrentChannelNo].fRefPlus < (MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus))
						ReceiveParam_ad[CurrentChannelNo].fRefPlus += fZhuJianSensitiveStep;
					else
						ReceiveParam_ad[CurrentChannelNo].fRefPlus = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus;

					ZhuJianSetFnRefPlusLableAndExecute(hWnd);
	
				}
				else if(wParam == VK_RETURN)
				{
				}

				else if(wParam == 'F')
				{
					if(wZhuJianSensitiveStepIndex < 3)
						wZhuJianSensitiveStepIndex++;
					else
						wZhuJianSensitiveStepIndex = 0;

					fZhuJianSensitiveStep = PLUSSTEPS[wZhuJianSensitiveStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fZhuJianSensitiveStep);
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
	return CallWindowProc(OldBtnZhuJianSensitiveProc,hWnd,Msg,wParam,lParam);
}
/**************************
*Φ值按钮自定义处理过程
* @ZhuJian    hInstance[in]:
* @ZhuJian		message[in]:
* @ZhuJian		wParam[in]:
* @ZhuJian		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnZhuJianDefectZhiProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(hWnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(zhuJianTanShangBtnList[i] != currentZhuJianBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentZhuJianBtn = zhuJianTanShangBtnList[i];
					SetFocus(currentZhuJianBtn);
					gCurrentFocusBtn = currentZhuJianBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(zhuJianTanShangBtnList[i] != currentZhuJianBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentZhuJianBtn = zhuJianTanShangBtnList[i];
					SetFocus(currentZhuJianBtn);
					gCurrentFocusBtn = currentZhuJianBtn;
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
	return CallWindowProc(OldBtnZhuJianDefectZhiProc,hWnd,Msg,wParam,lParam);
}
