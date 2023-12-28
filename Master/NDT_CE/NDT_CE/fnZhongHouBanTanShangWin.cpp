/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 中厚板探伤窗口源文件
 * Filename: fnZhongHouBanTanShangWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnZhongHouBanTanShangWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "recvParamWin.h"
#include "gateWin.h"
#include "craftParamWin.h"
#include "modeWin.h"
#include "const.h"

HWND hWndZhongHouBanTanShang = NULL;				//中厚板探伤窗口句柄
HWND hBtnZhongHouBanTanShangThickness = NULL;		//钢板厚度按钮句柄

HWND hBtnZhongHouBanTanShangHeadModel = NULL;	//探头型号按钮句柄
HWND hBtnZhongHouBanTanShangDacInput = NULL;		//DAC导入按钮句柄
HWND hBtnZhongHouBanTanShangDacOnOrOff = NULL;	//DAC曲线开启/关闭按钮句柄

HWND hBtnZhongHouBanTanShangDelay = NULL;			//延迟按钮句柄
HWND hBtnZhongHouBanTanShangUpBlindArea = NULL;	//上盲区按钮句柄
HWND hBtnZhongHouBanTanShangDownBlindArea = NULL;	//下盲区按钮句柄
HWND hBtnZhongHouBanTanShangSensitive = NULL;		//检测灵敏度按钮句柄

HWND ZhongHouBanTanShangBtnList[5] = {NULL,NULL,NULL,NULL,NULL};	//中厚板探伤窗口的按钮表
WNDPROC OldBtnZhongHouBanTanShangThicknessProc = NULL;		//钢板厚度按钮默认处理过程

WNDPROC OldBtnZhongHouBanTanShangHeadModelProc = NULL;	//探头型号按钮默认处理过程
WNDPROC OldBtnZhongHouBanTanShangDacInputProc = NULL;		//DAC导入按钮默认处理过程
WNDPROC OldBtnZhongHouBanTanShangDacOnOrOffProc = NULL;	//DAC曲线开启/关闭按钮默认处理过程

WNDPROC OldBtnZhongHouBanTanShangDelayProc = NULL;			//延迟按钮默认处理过程
WNDPROC OldBtnZhongHouBanTanShangUpBlindAreaProc = NULL;	//上盲区按钮默认处理过程
WNDPROC OldBtnZhongHouBanTanShangDownBlindAreaProc = NULL;	//下盲区按钮默认处理过程
WNDPROC OldBtnZhongHouBanTanShangSensitiveProc = NULL;		//检测灵敏度按钮默认处理过程

 
HWND currentZhongHouBanTanShangBtn = NULL;					//中厚板探伤窗中当前选中的按钮

static const int buttonNum = 5;		//按钮控件数
static float fDelayStep = 0.1;//AMENDSTEPS[0];
static WORD nDelayStepIndex = 0;			// 当前延时步长序号
static float fUpBlindValueStep = 0.1;		//上盲区步长
static WORD wUpBlindValueIndex = 0;			//上盲区步长序号
static float fDownBlindValueStep = 0.1;		//下盲区步长
static WORD wDownBlindValueIndex = 0;		//下盲区步长序号
extern HINSTANCE hInst;		//程序实例句柄

extern HWND hWndMain;
extern HWND hWndSpecialFunction;
extern HWND currentSpecialFunctionBtn;
extern HWND gCurrentFocusBtn;

// songchenguang add start 2010-06-30
float gfPlateHeight = 10.0f;
WORD nProbeType = 1;//去掉了static
float gfUpBlindValue = 1.0f;
float gfDownBlindValue = 1.0f;
static float fPlateHeightStep = 1.0f;
static float fBlindValueStep = 0.1f;

static WORD wPlateHeightStepIndex = 1;

BOOL bDACOpen = FALSE;
BOOL bDACLoaded[10] = {FALSE,FALSE,FALSE,FALSE,FALSE,
						FALSE,FALSE,FALSE,FALSE,FALSE};
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
// songchenguang add end 2011-06-22
extern WORD MAX_PLUS;
// songchenguang add end 2011-06-22
/**************************
* 注册中厚板探伤窗口
* @ZhongHouBanTanShang    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerZhongHouBanTanShangChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) zhongHouBanTanShangChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szZhongHouBanTanShangChild;

	return RegisterClass(&wc);
}
void displayDelay(HWND hWnd, float fDelay, const WORD& nType)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	// 避免显示-0.00
	if(fabs(fDelay)<1E-3)
		fDelay = 0.0f;

	wsprintf(strText, _T("%.2f"), fDelay);
	if(nType == 5)
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	else if(nType == 6)
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	else
		;
}
void SetProbeType(HWND hWnd, const WORD& nProbeNo)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));
	if(nProbeNo != 10)
		wsprintf(strText, _T("ARS-P0%d"), nProbeNo);
	else
		wsprintf(strText, _T("ARS-P10"));
	
	DrawLableText(hWnd, strText, 23, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);

	// 更改DAC文件名
	wchar_t strFileName[10];
	memset(strFileName, 0, sizeof(strFileName));

	wsprintf(strFileName, _T("%d.dac"), nProbeNo);

	memcpy(DACFileName, FileDirectory, sizeof(DACFileName));
	StringCchCat(DACFileName, MAX_PATH, strFileName);

	memset(strText, 0, sizeof(strText));
	if(bDACLoaded[nProbeNo-1])
		wsprintf(strText, _T("已导入"));
	else
		wsprintf(strText, _T("未导入"));

	DrawLableText(hBtnZhongHouBanTanShangDacInput, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
}

void SetPlateHeight(HWND hWnd, const float& fHeight)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fHeight);
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

}

void SetDACOpenFlag(HWND hWnd, const BOOL& bOpen)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	if(bOpen)
	{
		wsprintf(strText, _T("开"));

		chCurrent_dac_reg->wDACEnable |= 0x1;
		float fTemp = SendParam_ad[CurrentChannelNo].nAmend + ReceiveParam_ad[CurrentChannelNo].fDelay;
		chCurrent_dac_reg->wDACFront = (WORD)Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fTemp);
		chCurrent_dac_reg->wDACRear = chCurrent_dac_reg->wDACFront + (64*50);

	//	GateInfos[CurrentChannelNo][0].bOpen = TRUE;	//wangbingfu delete at 2010-09-07
	//	GateInfos[CurrentChannelNo][0].alarmMode = SuperHigh;	//wangbingfu delete at 2010-09-07
	//	chCurrent_gate_reg->wGate1Enable = 1;	//wangbingfu delete at 2010-09-07
		//wangbingfu add start at 2010-09-07
		GateInfos[0][0].alarmMode = SuperHigh;
		GateInfos[1][0].alarmMode = SuperHigh;
		ch1_gate_reg->wGate1Enable = 1;	
		ch2_gate_reg->wGate1Enable = 1;	
		//wangbingfu add end at 2010-09-07
		
		WORD nFront = (WORD)Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, gfUpBlindValue);
		WORD nBack = (WORD)Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (gfPlateHeight - gfDownBlindValue));
		WORD nRange = nBack - nFront;

		//SetGateCurve(chCurrent_gate1curve, nRange, 50);
		chCurrent_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 50 / 100.0f);
		GateInfos[CurrentChannelNo][0].fPeakGate = 50; //wangbingfu add at 2010-9-07
	}
	else
	{
		wsprintf(strText, _T("关"));
		chCurrent_dac_reg->wDACEnable &= 0xFFFE;

	//	GateInfos[CurrentChannelNo][0].bOpen = FALSE;	//wangbingfu modify at 2010-09-07
	//	chCurrent_gate_reg->wGate1Enable = 0;	//wangbingfu delete at 2010-09-07
		//wangbingfu add start at 2010-09-07
		ch1_gate_reg->wGate1Enable = 0;
		ch2_gate_reg->wGate1Enable = 0;
		//wangbingfu add end at 2010-09-07
	}

	if(hWnd)
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
}

void SetBlindValue(HWND hWnd, const float& fValue)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fValue);

	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

	
}

void SetFnRefPlusLableAndExecute(HWND hWnd)
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
//	if(hWnd)
		SaveParamFile(lpDefaultParamFile);
}
/**************************
*中厚板探伤窗口处理过程
* @ZhongHouBanTanShang    hInstance[in]:
* @ZhongHouBanTanShang		message[in]:
* @ZhongHouBanTanShang		wParam[in]:
* @ZhongHouBanTanShang		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK zhongHouBanTanShangChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// songchenguang add start 2010-06-30
			memset(DACFileName, 0, sizeof(DACFileName));
			fPlateHeightStep = AMENDSTEPS[wPlateHeightStepIndex];
			fPlusStep = PLUSSTEPS[nPlusStepIndex];
			// songchenguang add end 2010-06-30

			//画按钮控件
		/*	hBtnZhongHouBanTanShangThickness = CreateWindow(_T("BUTTON"),_T("thicknessZhongHouBanTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHONGHOUBANTANSHANG_THICKNESS,hInst,NULL);
			hBtnZhongHouBanTanShangHeadModel = CreateWindow(_T("BUTTON"),_T("headModelZhongHouBanTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHONGHOUBANTANSHANG_HEADMODEL,hInst,NULL);
			hBtnZhongHouBanTanShangDacInput = CreateWindow(_T("BUTTON"),_T("dacInputZhongHouBanTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHONGHOUBANTANSHANG_DACINPUT,hInst,NULL);
			hBtnZhongHouBanTanShangDacOnOrOff = CreateWindow(_T("BUTTON"),_T("dacOnOrOffZhongHouBanTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHONGHOUBANTANSHANG_DACONOROFF,hInst,NULL);
			hBtnZhongHouBanTanShangUpBlindArea = CreateWindow(_T("BUTTON"),_T("upBlindAreaZhongHouBanTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHONGHOUBANTANSHANG_UPBLINDAREA,hInst,NULL);
			hBtnZhongHouBanTanShangDownBlindArea = CreateWindow(_T("BUTTON"),_T("downBlindAreaZhongHouBanTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHONGHOUBANTANSHANG_DOWNBLINDAREA,hInst,NULL);
			hBtnZhongHouBanTanShangSensitive = CreateWindow(_T("BUTTON"),_T("upBlindAreaZhongHouBanTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 6*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 6*BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHONGHOUBANTANSHANG_SENSITIVE,hInst,NULL);

			ZhongHouBanTanShangBtnList[0] = hBtnZhongHouBanTanShangThickness;
			ZhongHouBanTanShangBtnList[1] = hBtnZhongHouBanTanShangHeadModel;
			ZhongHouBanTanShangBtnList[2] = hBtnZhongHouBanTanShangDacInput;
			ZhongHouBanTanShangBtnList[3] = hBtnZhongHouBanTanShangDacOnOrOff;
			ZhongHouBanTanShangBtnList[4] = hBtnZhongHouBanTanShangUpBlindArea;
			ZhongHouBanTanShangBtnList[5] = hBtnZhongHouBanTanShangDownBlindArea;
			ZhongHouBanTanShangBtnList[6] = hBtnZhongHouBanTanShangSensitive;
			*/
			
			hBtnZhongHouBanTanShangThickness = CreateWindow(_T("BUTTON"),_T("thicknessZhongHouBanTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHONGHOUBANTANSHANG_THICKNESS,hInst,NULL);
			hBtnZhongHouBanTanShangDelay = CreateWindow(_T("BUTTON"),_T("delayZhongHouBanTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHONGHOUBANTANSHANG_DELAY,hInst,NULL);
			hBtnZhongHouBanTanShangUpBlindArea = CreateWindow(_T("BUTTON"),_T("upBlindAreaZhongHouBanTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHONGHOUBANTANSHANG_UPBLINDAREA,hInst,NULL);
			hBtnZhongHouBanTanShangDownBlindArea = CreateWindow(_T("BUTTON"),_T("downBlindAreaZhongHouBanTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHONGHOUBANTANSHANG_DOWNBLINDAREA,hInst,NULL);
			hBtnZhongHouBanTanShangSensitive = CreateWindow(_T("BUTTON"),_T("upBlindAreaZhongHouBanTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ZHONGHOUBANTANSHANG_SENSITIVE,hInst,NULL);

			ZhongHouBanTanShangBtnList[0] = hBtnZhongHouBanTanShangThickness;
			ZhongHouBanTanShangBtnList[1] = hBtnZhongHouBanTanShangDelay;
			ZhongHouBanTanShangBtnList[2] = hBtnZhongHouBanTanShangUpBlindArea;
			ZhongHouBanTanShangBtnList[3] = hBtnZhongHouBanTanShangDownBlindArea;
			ZhongHouBanTanShangBtnList[4] = hBtnZhongHouBanTanShangSensitive;
			
			//按钮控件的自定义处理过程
			OldBtnZhongHouBanTanShangThicknessProc = (WNDPROC) GetWindowLong(hBtnZhongHouBanTanShangThickness,GWL_WNDPROC);
			SetWindowLong(hBtnZhongHouBanTanShangThickness,GWL_WNDPROC,(LONG)btnZhongHouBanTanShangThicknessProc);
			//no use 
			OldBtnZhongHouBanTanShangHeadModelProc = (WNDPROC) GetWindowLong(hBtnZhongHouBanTanShangHeadModel,GWL_WNDPROC);
			SetWindowLong(hBtnZhongHouBanTanShangHeadModel,GWL_WNDPROC,(LONG)btnZhongHouBanTanShangHeadModelProc);
			
			OldBtnZhongHouBanTanShangDacInputProc = (WNDPROC) GetWindowLong(hBtnZhongHouBanTanShangDacInput,GWL_WNDPROC);
			SetWindowLong(hBtnZhongHouBanTanShangDacInput,GWL_WNDPROC,(LONG)btnZhongHouBanTanShangDacInputProc);
			
			OldBtnZhongHouBanTanShangDacOnOrOffProc = (WNDPROC) GetWindowLong(hBtnZhongHouBanTanShangDacOnOrOff,GWL_WNDPROC);
			SetWindowLong(hBtnZhongHouBanTanShangDacOnOrOff,GWL_WNDPROC,(LONG)btnZhongHouBanTanShangDacOnOrOffProc);
			//no use
			OldBtnZhongHouBanTanShangDelayProc = (WNDPROC) GetWindowLong(hBtnZhongHouBanTanShangDelay,GWL_WNDPROC);
			SetWindowLong(hBtnZhongHouBanTanShangDelay,GWL_WNDPROC,(LONG)btnZhongHouBanTanShangDelayProc);
			
			OldBtnZhongHouBanTanShangUpBlindAreaProc = (WNDPROC) GetWindowLong(hBtnZhongHouBanTanShangUpBlindArea, GWL_WNDPROC);
			SetWindowLong(hBtnZhongHouBanTanShangUpBlindArea,GWL_WNDPROC,(LONG)btnZhongHouBanTanShangUpBlindAreaProc);

			OldBtnZhongHouBanTanShangDownBlindAreaProc = (WNDPROC) GetWindowLong(hBtnZhongHouBanTanShangDownBlindArea, GWL_WNDPROC);
			SetWindowLong(hBtnZhongHouBanTanShangDownBlindArea,GWL_WNDPROC,(LONG)btnZhongHouBanTanShangDownBlindAreaProc);

			OldBtnZhongHouBanTanShangSensitiveProc = (WNDPROC) GetWindowLong(hBtnZhongHouBanTanShangSensitive, GWL_WNDPROC);
			SetWindowLong(hBtnZhongHouBanTanShangSensitive,GWL_WNDPROC,(LONG)btnZhongHouBanTanShangSensitiveProc);
			
			currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[0];//默认当前选中钢板厚度按钮
			SetFocus(currentZhongHouBanTanShangBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentZhongHouBanTanShangBtn;

			// 设为双晶关联
			SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)ShuangJingGuanLian);					

			sys_reg->wWorkMode = ShuangJingGuanLian;

			InitializeCriticalSection(&csSysInfo);
			EnterCriticalSection(&csSysInfo);

			gSysInfo.wRelay |= 0x3;
			//yachang.huang rum on 2013-11-19
			//sys_reg->wRelay = gSysInfo.wRelay;
			gSysInfo.wWorkMode = ShuangJingGuanLian;

			LeaveCriticalSection(&csSysInfo);
			DeleteCriticalSection(&csSysInfo);

			SetRedrawFlag(TRUE);
			gbGuanLian = TRUE;

			// 将Ch1数据复制到Ch2
			CurrentChannelNo = 0;
			chCurrent_data_reg = ch1_data_reg;
			chCurrent_gate_reg = ch1_gate_reg;
			chCurrent_con_reg = ch1_con_reg;
			chCurrent_interval = ch1_interval;
			chCurrent_data = ch1_data;

			chCurrent_plus_reg = ch1_plus_reg;
			chCurrent_dac_reg = ch1_dac_reg;

			chCurrent_gate1curve = ch1_gate1curve;
			chCurrent_gate2curve = ch1_gate2curve;
			chCurrent_gate3curve = ch1_gate3curve;

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
			// 将Ch1数据复制到Ch2
			Ch1Data2Ch2Data();
			InitParamDetail();

			SetOpenFlag(NULL, TRUE, 1);
			SetAlarmMode(NULL, 1, SuperHigh);
			SetPeakGate(NULL, 1, 50.0f, FALSE);
			// 设定闸门前沿 
			SetQianYan(NULL, 1, gfUpBlindValue, FALSE);
			// 设定后沿
			SetHouYan(NULL, 1, (gfPlateHeight - gfDownBlindValue), FALSE);

			SetOpenFlag(NULL, TRUE, 2);
			SetAlarmMode(NULL, 2, LostWave);
			SetPeakGate(NULL, 2, 50.0f, FALSE);
			// 设定闸门前沿 
			SetQianYan(NULL, 2, (gfPlateHeight - gfDownBlindValue), FALSE);
			// 设定后沿
			SetHouYan(NULL, 2, (gfPlateHeight - gfDownBlindValue + 10), FALSE);

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
			// wangbingfu add start 2010-09-07
		//	closeAllGate();//关闭所有闸门 快捷切到闸门 再切回到中厚板探伤时
		//	GateInfos[0][0].bOpen = ON; //画通道1闸门1线
		//	GateInfos[1][0].bOpen = ON; //画通道2闸门1线
			// wangbingfu add end 2010-09-07
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			zhongHouBanTanShang_DrawButtonBitmap(hInst,pdis);

			// songchenguang add start 2010-06-18
			if(pdis->hwndItem == hBtnZhongHouBanTanShangHeadModel)
				SetProbeType(pdis->hwndItem, nProbeType);
			else if(pdis->hwndItem == hBtnZhongHouBanTanShangThickness)
				SetPlateHeight(pdis->hwndItem, gfPlateHeight);
			else if(pdis->hwndItem == hBtnZhongHouBanTanShangDacInput)
			{
				wchar_t strText[10];
				memset(strText, 0, sizeof(strText));
				if(bDACLoaded[nProbeType-1])
					wsprintf(strText, _T("已导入"));
				else
					wsprintf(strText, _T("未导入"));

				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
			}
			else if(pdis->hwndItem == hBtnZhongHouBanTanShangDacOnOrOff)
				SetDACOpenFlag(pdis->hwndItem, bDACOpen);
			else if(pdis->hwndItem == hBtnZhongHouBanTanShangDelay)
			{
				displayDelay(pdis->hwndItem, ReceiveParam_ad[CurrentChannelNo].fDelay, 5);
			}
			else if(pdis->hwndItem == hBtnZhongHouBanTanShangUpBlindArea)
				SetBlindValue(pdis->hwndItem,gfUpBlindValue);
			else if(pdis->hwndItem == hBtnZhongHouBanTanShangDownBlindArea)
				SetBlindValue(pdis->hwndItem,gfDownBlindValue);
			else if(pdis->hwndItem == hBtnZhongHouBanTanShangSensitive)
				SetFnRefPlusLableAndExecute(pdis->hwndItem);

			// songchenguang add end 2010-06-18

			// 检测按钮句柄，显示相应的步长
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentZhongHouBanTanShangBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnZhongHouBanTanShangThickness)
						swprintf(gszTextStep, _T("%.1f"), fPlateHeightStep);
					else if(pdis->hwndItem == hBtnZhongHouBanTanShangDelay)
						swprintf(gszTextStep, _T("%.1f"), fDelayStep);
					else if(pdis->hwndItem == hBtnZhongHouBanTanShangUpBlindArea)
						swprintf(gszTextStep, _T("%.1f"), fUpBlindValueStep);
					else if(pdis->hwndItem == hBtnZhongHouBanTanShangDownBlindArea)
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
		case WM_DESTROY:
			// 若DAC打开，关闭DAC
//			bDACOpen = FALSE;
//			SetDACOpenFlag(NULL, bDACOpen);
			//wangbingfu add at 2010-09-07
//			GateInfos[0][0].bOpen = OFF;//关闭闸门线
//			GateInfos[1][0].bOpen = OFF;//关闭闸门线
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载中厚板探伤窗口按钮图片
* @ZhongHouBanTanShang    hInstance[in]:
* @ZhongHouBanTanShang		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int zhongHouBanTanShang_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
/*	const BTN_BMP_INFO tbl_zhongHouBanTanShangBtnBitmap[]=
	{
		{IDC_ZHONGHOUBANTANSHANG_THICKNESS	,IDB_BITMAP_GANGBANHOUDU7_NOSEL_R	 ,  IDB_BITMAP_GANGBANHOUDU7_SELECT_R		},
		{IDC_ZHONGHOUBANTANSHANG_HEADMODEL	,IDB_BITMAP_TANTOUXINGHAO_NOSEL_R	 ,  IDB_BITMAP_TANTOUXINGHAO_SELECT_R		},
		{IDC_ZHONGHOUBANTANSHANG_DACINPUT	,IDB_BITMAP_DACQUXIANDAORU_NOSEL_R ,  IDB_BITMAP_DACQUXIANDAORU_SELECT_R	},
		{IDC_ZHONGHOUBANTANSHANG_DACONOROFF	,IDB_BITMAP_DAC_KAIQIGUANBI_NOSEL_R ,  IDB_BITMAP_DAC_KAIQIGUANBI_SELECT_R	},
		{IDC_ZHONGHOUBANTANSHANG_UPBLINDAREA	,IDB_BITMAP_SHANGMANGQU_NOSEL_R ,  IDB_BITMAP_SHANGMANGQU_SELECT_R	},
		{IDC_ZHONGHOUBANTANSHANG_DOWNBLINDAREA	,IDB_BITMAP_XIAMANGQU_NOSEL_R ,  IDB_BITMAP_XIAMANGQU_SELECT_R	},
		{IDC_ZHONGHOUBANTANSHANG_SENSITIVE	,IDB_BITMAP_JIANCELINGMINDU_NOSEL_R ,  IDB_BITMAP_JIANCELINGMINDU_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 
*/
	const BTN_BMP_INFO tbl_zhongHouBanTanShangBtnBitmap[]=
	{
		{IDC_ZHONGHOUBANTANSHANG_THICKNESS,		IDB_BITMAP_GANGBANHOUDU5_NOSEL_R,  	IDB_BITMAP_GANGBANHOUDU5_SELECT_R		},
		{IDC_ZHONGHOUBANTANSHANG_DELAY,			IDB_BITMAP_YANCHI5_NOSEL_R ,  		IDB_BITMAP_YANCHI5_SELECT_R		},
		{IDC_ZHONGHOUBANTANSHANG_UPBLINDAREA,	IDB_BITMAP_SHANGMANGQU_NOSEL_R ,  	IDB_BITMAP_SHANGMANGQU_SELECT_R	},
		{IDC_ZHONGHOUBANTANSHANG_DOWNBLINDAREA,	IDB_BITMAP_XIAMANGQU_NOSEL_R , 		IDB_BITMAP_XIAMANGQU_SELECT_R	},
		{IDC_ZHONGHOUBANTANSHANG_SENSITIVE,		IDB_BITMAP_JIANCELINGMINDU_NOSEL_R ,  IDB_BITMAP_JIANCELINGMINDU_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 
	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_zhongHouBanTanShangBtnBitmap);
	return ReturnVal;
}

void SetGuanLianDownBlindAreaGate()
{
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	float fLength = GateInfos[0][1].fBackGate - GateInfos[0][1].fFrontGate;
	float fG12Distance = GateInfos[0][1].fFrontGate - GateInfos[0][0].fBackGate;
	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);

	// 设定后沿
	SetHouYan(NULL, 1, (gfPlateHeight - gfDownBlindValue), FALSE);					

	SetQianYan(NULL, 2, (gfPlateHeight - gfDownBlindValue + fG12Distance), FALSE);
	SetHouYan(NULL, 2, (gfPlateHeight - gfDownBlindValue  + fG12Distance + fLength), TRUE);
}

/**************************
*钢板厚度按钮自定义处理过程
* @ZhongHouBanTanShang    hInstance[in]:
* @ZhongHouBanTanShang		message[in]:
* @ZhongHouBanTanShang		wParam[in]:
* @ZhongHouBanTanShang		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnZhongHouBanTanShangThicknessProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndZhongHouBanTanShang);
					hWndZhongHouBanTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}			
				else if(wParam == VK_UP)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					float fCha = gfPlateHeight - 5.0f;
					if(gfPlateHeight > 5.0f && fCha > fPlateHeightStep)
						gfPlateHeight -= fPlateHeightStep;
					else
						gfPlateHeight = 5.0f;

					SetPlateHeight(hWnd, gfPlateHeight);
					
					// 设定闸门前沿 
					SetQianYan(NULL, 1, gfUpBlindValue, FALSE);

					////////////////////////////////////////////////////////
					// 可以提取出来 songchenguang
					SetGuanLianDownBlindAreaGate();
					///////////////////////////////////////////////////////
				}
				else if(wParam == VK_RIGHT)
				{
					float fCha = MAX_fPlateHeight - gfPlateHeight;
					if(gfPlateHeight < MAX_fPlateHeight && fCha > fPlateHeightStep)
						gfPlateHeight += fPlateHeightStep;
					else
						gfPlateHeight = MAX_fPlateHeight;

					SetPlateHeight(hWnd, gfPlateHeight);

					// 设定闸门前沿 
					SetQianYan(NULL, 1, gfUpBlindValue, FALSE);

					////////////////////////////////////////////////////////
					// 可以提取出来 songchenguang
					SetGuanLianDownBlindAreaGate();
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
	return CallWindowProc(OldBtnZhongHouBanTanShangThicknessProc,hWnd,Msg,wParam,lParam);
}


/**************************
*探头型号按钮自定义处理过程
* @ZhongHouBanTanShang    hInstance[in]:
* @ZhongHouBanTanShang		message[in]:
* @ZhongHouBanTanShang		wParam[in]:
* @ZhongHouBanTanShang		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnZhongHouBanTanShangHeadModelProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndZhongHouBanTanShang);
					hWndZhongHouBanTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}
			
				else if(wParam == VK_UP)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(nProbeType > 1)
						nProbeType --;
					else
						nProbeType = 10;

					SetProbeType(hWnd, nProbeType);
				}
				else if(wParam == VK_RIGHT)
				{
					if(nProbeType < 10)
						nProbeType ++;
					else
						nProbeType = 1;

					SetProbeType(hWnd, nProbeType);
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

	return CallWindowProc(OldBtnZhongHouBanTanShangHeadModelProc,hWnd,Msg,wParam,lParam);
}

BOOL LoadDACFile(LPCTSTR lpDACFilePath)
{
	WORD DAC_DATA[64];
	memset(DAC_DATA, 0, sizeof(DAC_DATA));

	HANDLE hFileWrite;	//文件句柄
	DWORD dwWriteDataSize;
	hFileWrite = CreateFile(lpDACFilePath,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("打开文件失败"));
		return FALSE;
	}

	for(int i=0; i<64; i++)
	{
		WORD gain = 0; 
		if(!ReadFile(hFileWrite,&gain, sizeof(WORD),&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("读文件失败"));
			
			CloseHandle(hFileWrite);
			return FALSE;
		}

		DAC_DATA[i] = WORD(MAX_HEIGHT / (10^(gain/20)));
	}

	memcpy(chCurrent_tgc, DAC_DATA, sizeof(DAC_DATA));
	CloseHandle(hFileWrite);
	
	return TRUE;
}
/**************************
*DAC导入按钮自定义处理过程
* @ZhongHouBanTanShang    hInstance[in]:
* @ZhongHouBanTanShang		message[in]:
* @ZhongHouBanTanShang		wParam[in]:
* @ZhongHouBanTanShang		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnZhongHouBanTanShangDacInputProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndZhongHouBanTanShang);
					hWndZhongHouBanTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}
			
				else if(wParam == VK_UP)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
					wchar_t strText[10];
					// 导入DAC文件
					BOOL bRet = LoadDACFile(DACFileName);
					if(bRet)
					{
						for(int i=0; i<10; i++)
							bDACLoaded[i] = FALSE;

						bDACLoaded[nProbeType-1] = TRUE;

						memset(strText, 0, sizeof(strText));
						wsprintf(strText, _T("已导入"));
	
					}
					else
						wsprintf(strText, _T("未导入"));

					DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
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
	return CallWindowProc(OldBtnZhongHouBanTanShangDacInputProc,Wnd,Msg,wParam,lParam);
}
/**************************
*DAC曲线开启/关闭按钮自定义处理过程
* @ZhongHouBanTanShang    hInstance[in]:
* @ZhongHouBanTanShang		message[in]:
* @ZhongHouBanTanShang		wParam[in]:
* @ZhongHouBanTanShang		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnZhongHouBanTanShangDacOnOrOffProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndZhongHouBanTanShang);
					hWndZhongHouBanTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}
			
				else if(wParam == VK_UP)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					bDACOpen = !bDACOpen;

					SetDACOpenFlag(hWnd, bDACOpen);
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
	return CallWindowProc(OldBtnZhongHouBanTanShangDacOnOrOffProc,hWnd,Msg,wParam,lParam);
}

/**************************
*延迟按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnZhongHouBanTanShangDelayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			fDelayStep = AMENDSTEPS[nDelayStepIndex];
			break;
		case WM_PAINT:
			wchar_t strText[20];
			memset(strText, 0, sizeof(strText));
			wsprintf(strText, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fDelay);

			//SetWindowText(hLableDelay, strText);
			DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndZhongHouBanTanShang);
					hWndZhongHouBanTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}
			
				else if(wParam == VK_UP)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(ReceiveParam_ad[CurrentChannelNo].fDelay > MIN_DELAY[CurrentChannelNo] && 
						ReceiveParam_ad[CurrentChannelNo].fDelay > (MIN_DELAY[CurrentChannelNo] + fDelayStep))
						ReceiveParam_ad[CurrentChannelNo].fDelay -= fDelayStep;
					else
						ReceiveParam_ad[CurrentChannelNo].fDelay = MIN_DELAY[CurrentChannelNo];

					displayDelay(Wnd, ReceiveParam_ad[CurrentChannelNo].fDelay, 5);
					SetDelayLableAndExecute(NULL, CurrentChannelNo);
				}
				else if(wParam == VK_RIGHT)
				{
					if(ReceiveParam_ad[CurrentChannelNo].fDelay < MAX_DELAY)
						ReceiveParam_ad[CurrentChannelNo].fDelay += fDelayStep;
					else
						ReceiveParam_ad[CurrentChannelNo].fDelay = MAX_DELAY;

					displayDelay(Wnd, ReceiveParam_ad[CurrentChannelNo].fDelay, 5);
					SetDelayLableAndExecute(NULL, CurrentChannelNo);
				}
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
				else if(wParam == 'B')	// 冻结
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
	return CallWindowProc(OldBtnZhongHouBanTanShangDelayProc,Wnd,Msg,wParam,lParam);
}

/**************************
* 上盲区自定义处理过程
* @ZhongHouBanTanShang    hInstance[in]:
* @ZhongHouBanTanShang		message[in]:
* @ZhongHouBanTanShang		wParam[in]:
* @ZhongHouBanTanShang		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnZhongHouBanTanShangUpBlindAreaProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndZhongHouBanTanShang);
					hWndZhongHouBanTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}
			
				else if(wParam == VK_UP)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(gfUpBlindValue > fUpBlindValueStep)
						gfUpBlindValue -= fUpBlindValueStep;
					else
						gfUpBlindValue = 0.0f;

					SetBlindValue(hWnd, gfUpBlindValue);

					// 设定闸门前沿
					SetQianYan(NULL, 1, gfUpBlindValue, TRUE);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfUpBlindValue < (gfPlateHeight - gfDownBlindValue - fUpBlindValueStep))
						gfUpBlindValue += fUpBlindValueStep;
					else
						gfUpBlindValue = gfPlateHeight - gfDownBlindValue;

					SetBlindValue(hWnd, gfUpBlindValue);

					// 设定闸门前沿 
					SetQianYan(NULL, 1, gfUpBlindValue, TRUE);
				}
				else if(wParam == 'F')
				{
					if(wUpBlindValueIndex < 3)
						wUpBlindValueIndex ++;
					else 
						wUpBlindValueIndex = 0;
					fUpBlindValueStep = AMENDSTEPS[wUpBlindValueIndex];
					
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
	return CallWindowProc(OldBtnZhongHouBanTanShangUpBlindAreaProc,hWnd,Msg,wParam,lParam);
}

/**************************
* 下盲区自定义处理过程
* @ZhongHouBanTanShang    hInstance[in]:
* @ZhongHouBanTanShang		message[in]:
* @ZhongHouBanTanShang		wParam[in]:
* @ZhongHouBanTanShang		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnZhongHouBanTanShangDownBlindAreaProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndZhongHouBanTanShang);
					hWndZhongHouBanTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}
			
				else if(wParam == VK_UP)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(gfDownBlindValue > fDownBlindValueStep)
						gfDownBlindValue -= fDownBlindValueStep;
					else
						gfDownBlindValue = 0.0f;

					SetBlindValue(hWnd, gfDownBlindValue);

					// 设定下盲区改变后闸门变化
					SetGuanLianDownBlindAreaGate();
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfDownBlindValue < (gfPlateHeight - gfUpBlindValue -fDownBlindValueStep))
						gfDownBlindValue += fDownBlindValueStep;
					else
						gfDownBlindValue = gfPlateHeight - gfUpBlindValue;

					SetBlindValue(hWnd, gfDownBlindValue);

					// 设定下盲区改变后闸门变化
					SetGuanLianDownBlindAreaGate();
				}

				else if(wParam == 'F')
				{
					if(wDownBlindValueIndex < 3)
						wDownBlindValueIndex ++;
					else 
						wDownBlindValueIndex = 0;
					fDownBlindValueStep = AMENDSTEPS[wDownBlindValueIndex];
					
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
	return CallWindowProc(OldBtnZhongHouBanTanShangDownBlindAreaProc, hWnd,Msg,wParam,lParam);
}

/**************************
* 探伤灵敏度自定义处理过程
* @ZhongHouBanTanShang    hInstance[in]:
* @ZhongHouBanTanShang		message[in]:
* @ZhongHouBanTanShang		wParam[in]:
* @ZhongHouBanTanShang		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnZhongHouBanTanShangSensitiveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndZhongHouBanTanShang);
					hWndZhongHouBanTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;
				}
			
				else if(wParam == VK_UP)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ZhongHouBanTanShangBtnList[i] != currentZhongHouBanTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentZhongHouBanTanShangBtn = ZhongHouBanTanShangBtnList[i];
					SetFocus(currentZhongHouBanTanShangBtn);
					gCurrentFocusBtn = currentZhongHouBanTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
					// songchenguang add start 2010-04-22
					if((ReceiveParam_ad[CurrentChannelNo].fRefPlus - fPlusStep) > MIN_PLUS)
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
					float fPlusRange = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus;
					float fMaxValue = fPlusRange > 80.0f ? 80.0f : fPlusRange;

					if((ReceiveParam_ad[CurrentChannelNo].fRefPlus + fPlusStep) < fMaxValue)
						ReceiveParam_ad[CurrentChannelNo].fRefPlus += fPlusStep;
					else
						ReceiveParam_ad[CurrentChannelNo].fRefPlus = fMaxValue;

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
	return CallWindowProc(OldBtnZhongHouBanTanShangSensitiveProc,Wnd,Msg,wParam,lParam);
}
