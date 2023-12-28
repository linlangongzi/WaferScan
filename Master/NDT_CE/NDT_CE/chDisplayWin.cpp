/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 测厚测量显示窗口源文件
 * DisplayCurrentname: chDisplayWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"

#include "resource.h"
#include "interfaceCmdID.h"

#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"
#include "gateWin.h"

#define AVER_NUM 20// 平均次数
HWND hWndChDisplay = NULL;//测厚测量显示窗口句柄
HWND currentChDisplayBtn = NULL;// 测厚测量显示窗口当前选中的按钮

BOOL gbMeasureSwitch = FALSE;	//是否在测量

HANDLE hDrawMeasureThread = NULL;				// 测量显示线程句柄
DRAWTHREAD_PARAM DrawMeasureThread_param;		// 测量显示线程参数
DWORD dwDrawMeasureThreadID = 0;				// 测量显示线程ID
CRITICAL_SECTION csMeasure;

static HWND hBtnDisplaySwitch = NULL;//开关按钮句柄
static HWND hBtnDisplayMax = NULL;//最大值按钮句柄
static HWND hBtnDisplayMin = NULL;//最小值按钮句柄
static HWND hBtnDisplayAverage = NULL;//平均值按钮句柄
static HWND hBtnDisplayCurrent = NULL;//当前值按钮句柄
static HWND hBtnDisplayChaZhi = NULL;//差值按钮句柄
static HWND hBtnDisplayTuCeng = NULL;	//涂层厚度按钮句柄


static HWND chDisplayBtnList[2] = {NULL,NULL};//测厚测量显示窗口的按钮表
static const int buttonNum = 2;//按钮控件数

static WNDPROC OldBtnDisplaySwitchProc = NULL;//开关按钮默认处理过程
static WNDPROC OldBtnDisplayAverageProc = NULL;//平均值按钮默认处理过程
//static WNDPROC OldBtnDisplayMaxProc = NULL;//最大值按钮默认处理过程
//static WNDPROC OldBtnDisplayMinProc = NULL;// 最小值按钮默认处理过程
//static WNDPROC OldBtnDisplayCurrentProc = NULL;//当前值按钮默认处理过程
//static WNDPROC OldBtnDisplayChaZhiProc = NULL;//差值按钮默认处理过程

ATOM registerChDisplayChild(HINSTANCE hInstance);
static LRESULT CALLBACK chDisplayChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int chDisplay_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnDisplaySwitchProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnDisplayAverageProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
//static LRESULT CALLBACK BtnDisplayMaxProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
//static LRESULT CALLBACK btnDisplayMinProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
//static LRESULT CALLBACK btnDisplayChaZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

extern HINSTANCE hInst;			//程序实例句柄
extern HWND hWndMain;			//主窗口句柄
extern HWND gCurrentFocusBtn;
extern HWND gCurrentWnd;
extern MEASURE_DISPLAY_ALL gstMeasureDisplay;
extern WORD CurrentChannelNo;
extern float fRangeEnd[2];
extern float fRangeStart[2];
extern PIC_DATA gPicData[2];									// 图像数据
extern float gfMinRange[2];
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern PEAK_INFO gPeakDefectInfos[2][2];						//保存闸门峰值点信息
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern GATE_INFO GateInfos[2][3];
extern BOOL gbHeadVCurveFlag;									//探头V型补偿曲线是否打开
extern BOOL gbTempCurveFlag;									//是否打开温度补偿曲线
extern WORD gwHeadVPointNum[20];								//探头V型补偿曲线点的个数
extern HEADVPOINT_DATA gstHeadVPointData[20][HEADV_POINT_MAX];	//采集的探头V型补偿曲线点
extern WORD gwHeadVHeadNum;										//探头型号
extern float gfCurrentTemperature;								//当前温度
extern WORD gwTempCurvePointNum[MATERIAL_NUM + 10];								//探头V型补偿曲线点的个数
extern TEMPCURVEPOINT_DATA gstTempCurvePointData[MATERIAL_NUM + 10][HEADV_POINT_MAX];	//采集的探头V型补偿曲线点
extern WORD gwTempCurveMaterial;								//温度补偿材料
extern WORD gwTuCengSpeed;										//涂层声速
extern WORD gwTuCengType;										//涂层材料类型号
extern float fCurrentValue1;									//当前值1
extern float fCurrentValue2;									//当前值2
extern float fCurrentValue3;									//当前值3
extern float fCurrentValue4;									//当前值4
extern float fCurrentValue5;									//当前值5
extern float fCurrentValue6;									//当前值6
extern HWND hWndChAverage;										//测厚平均值窗口句柄
extern HWND currentChAverageBtn;								//测厚平均值窗口当前选中的按钮
extern BOOL  gbHotGateFlag;	//是否打开闸门快捷方式
extern BOOL  gbHotAutoGainFlag;	//是否打开自动增益快捷方式 
extern BOOL  gbHotAutoAdjustFlag;	//是否打开自动校准快捷方式 
extern ARS200FPGA_SYS_REG* sys_reg;
extern GATE_INFO stOldGateInfos[2][3];


extern HWND hWndChMenu;//测厚菜单窗口句柄
extern BOOL bFreezeUp;
extern BOOL gbHotDB;
extern BOOL gbHotRange;	// 快捷键声程
extern wchar_t gszTextPlus[10];
extern wchar_t gszTextRefPlus[10];
extern CRITICAL_SECTION csHotRange;
extern HWND hStatusBar;					// 状态栏句柄
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern CRITICAL_SECTION csPeakDefectInfos;
extern WORD tuCengTypeToWaveSpeed(WORD wType);

#if 0
/**************************
*第一个回波声程
* @return   fDistance : 声程
* @since    1.00
***************************/
float firstHuiBoDistance()
{
	int i = 21;
	int nSerial = 0;
	WORD nCurrenData = 0;
	WORD wNum = DATA_SIZE;
	float fDistance = 0;

	if(DisplaySet_ad[CurrentChannelNo].fRange < gfMinRange[CurrentChannelNo])//小于最小声程
	{
		wNum = WORD((DisplaySet_ad[CurrentChannelNo].fRange / gfMinRange[CurrentChannelNo])*DATA_SIZE);
	}
	while(gPicData[CurrentChannelNo].data[i] > 200)
	{
		i++;
	}
	
	for(int j = i + 10; j < DATA_SIZE - 1; j++)
	{
		if(nCurrenData < gPicData[CurrentChannelNo].data[j])
		{
			nCurrenData = gPicData[CurrentChannelNo].data[j];
			nSerial = j;
		}
	}
	fDistance = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * nSerial / wNum;

	return fDistance;
}

/**************************
*第二个回波声程
* @return   fDistance : 声程
* @since    1.00
***************************/
float secondHuiBoDistance()
{
	int i = 21;
	int nSerial = 0;
	WORD nCurrenData = 0;
	WORD wNum = DATA_SIZE;
	float fDistance = 0;

	if(DisplaySet_ad[CurrentChannelNo].fRange < gfMinRange[CurrentChannelNo])//小于最小声程
	{
		wNum = WORD((DisplaySet_ad[CurrentChannelNo].fRange / gfMinRange[CurrentChannelNo])*DATA_SIZE);
	}
	while(gPicData[CurrentChannelNo].data[i] > 200)
	{
		i++;
	}
	//第一个回波最高点
	for(int k = i + 10; k < DATA_SIZE - 1; k++)
	{
		if(nCurrenData < gPicData[CurrentChannelNo].data[k])
		{
			nCurrenData = gPicData[CurrentChannelNo].data[k];
			nSerial = k;
		}
	}
	nCurrenData = 0;
	//第一个回波之后的点
	while(gPicData[CurrentChannelNo].data[nSerial] > 150)
	{
		nSerial++;
	}
	//第二个回波最高点
	for(int j = nSerial + 10; j < DATA_SIZE - 1; j++)
	{
		if(nCurrenData < gPicData[CurrentChannelNo].data[j])
		{
			nCurrenData = gPicData[CurrentChannelNo].data[j];
			nSerial = j;
		}
	}
	fDistance = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * nSerial / wNum;

	return fDistance;
}
#endif
/**************************
*显示测量开关
* @param    hWnd[in]:窗口句柄
* @param    bSwitch[in]: 开/关
* @return   ATOM :
* @since    1.00
***************************/
void displayMeasureSwitch(HWND hWnd, BOOL bSwitch)
{
	if(bSwitch)
		DrawLableText(hWnd, _T("开始测量"), 22, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	else
	{
		DrawLableText(hWnd, _T("结束测量"), 22, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		
		if(gstMeasureDisplay.stMax.bSwitch == ON)//最大值
			DrawLableText(hBtnDisplayMax, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		else
			DrawLableText(hBtnDisplayMax, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		
		if(gstMeasureDisplay.stMin.bSwitch == ON)//最小值
			DrawLableText(hBtnDisplayMin, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		else
			DrawLableText(hBtnDisplayMin, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		
		if(gstMeasureDisplay.stAverage.bSwitch == ON)//平均值
			DrawLableText(hBtnDisplayAverage, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		else
			DrawLableText(hBtnDisplayAverage, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		
		if(gstMeasureDisplay.stCurrent.bSwitch == ON)//当前值
			DrawLableText(hBtnDisplayCurrent, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		else
			DrawLableText(hBtnDisplayCurrent, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		
		if(gstMeasureDisplay.stChaZhi.bSwitch == ON)//差值
			DrawLableText(hBtnDisplayChaZhi, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		else
			DrawLableText(hBtnDisplayChaZhi, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);

		DrawLableText(hBtnDisplayTuCeng, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	}
}
/**************************
*显示最大值
* @param		hWnd[in]:窗口句柄
* @param		fMax[in]:最大值
* @return   LRESULT :
* @since    1.00
***************************/
void displayMeasureMax(HWND hWnd ,float fMax)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(gbMeasureSwitch)
	{
		wsprintf(strText, _T("%.2f"), fMax);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	}
	else
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
}
/**************************
*显示最小值
* @param		hWnd[in]:窗口句柄
* @param		fMax[in]:最大值
* @return   LRESULT :
* @since    1.00
***************************/
void displayMeasureMin(HWND hWnd ,float fMin)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(gbMeasureSwitch)
	{
		if(fabs(fMin - 10000) < 1E-6)
		{
			fMin = 0.0f;
		}
		wsprintf(strText, _T("%.2f"), fMin);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	}
	else
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
}
/**************************
*显示最平均值
* @param		hWnd[in]:窗口句柄
* @param		fMax[in]:最大值
* @return   LRESULT :
* @since    1.00
***************************/
void displayMeasureAverage(HWND hWnd ,float fAverage)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	if(fAverage == 0)
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	else
	{
		wsprintf(strText, _T("%.2f"), fAverage);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	}
}

/**************************
*显示当前值
* @param		hWnd[in]:窗口句柄
* @param		fMax[in]:最大值
* @return   LRESULT :
* @since    1.00
***************************/
void displayMeasureCurrent(HWND hWnd ,float fCurrent)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(gbMeasureSwitch)
	{
		if(fCurrent < 0)
		{
			fCurrent = 0;
			
		}
		wsprintf(strText, _T("%.2f"), fCurrent);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	}
	else
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
}

/**************************
*显示差值
* @param		hWnd[in]:窗口句柄
* @param		fMax[in]:最大值
* @return   LRESULT :
* @since    1.00
***************************/
void displayMeasureChaZhi(HWND hWnd ,WORD wMode, float fChaZhi)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	if(gbMeasureSwitch)
	{
		if(wMode == 0)
		{
			wsprintf(strText, _T("%.2f"), fChaZhi);
			DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		}
		else if(wMode == 1)
		{
			wsprintf(strText, _T("%.2f%c"), fChaZhi * 100, '%');
			DrawLableText(hWnd, strText, 24, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		}				
	}
	else
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
}

/**************************
*显示涂层厚度
* @param		hWnd[in]:窗口句柄
* @param		fHouDu[in]:最大值
* @return   LRESULT :
* @since    1.00
***************************/
void displayMeasureTuCeng(HWND hWnd ,float fHouDu, WORD wMeasureMethod)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(gbMeasureSwitch)
	{
		wsprintf(strText, _T("%.2f"), fHouDu);
		if(wMeasureMethod != 3)
			DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		else
			DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	}
	else
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
}

/**************************
*测量处理函数
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
DWORD WINAPI measureProc(LPVOID lpParameter)
{
	WORD wCount = 0;//计数，20次数据取平均
	float fCurrentValue[AVER_NUM] = {0};
	float fGateWidth = (DisplaySet_ad[CurrentChannelNo].fRange / 100) * 4;	//保持闸门宽度在屏幕上一致
	float fFirstHuiBoDistance = firstHuiBoDistance();						//第一个底波声程
	float fSecondHuiBoDistance = secondHuiBoDistance();						//第二个底波声程
	float fGate1QianDian = fFirstHuiBoDistance - fGateWidth / 2;			//闸门1起点
	float fGate2QianDian = fSecondHuiBoDistance - fGateWidth /2;			//闸门2起点
	float fGate1Back = fGate1QianDian + fGateWidth;							//闸门1后沿
	float fGate2Back = fGate2QianDian + fGateWidth;							//闸门2后沿
	float fRange1 = 0;														//闸门1读取的波峰点声程
	float fRange2 = 0;														//闸门2读取的波峰点声程
	gstMeasureDisplay.stMax.fValue = 0;
	gstMeasureDisplay.stMin.fValue = 10000;

	PEAK_INFO peakDefectInfos[2][2];
	memset(peakDefectInfos, 0, sizeof(peakDefectInfos));
		
/*	InitializeCriticalSection(&csPeakDefectInfos);
	EnterCriticalSection(&csPeakDefectInfos);
	for(int i = 0 ;i < CHANNEL_NUM; i++)
	{
		if(gPeakDefectInfos[i][0].wDefectAmp > double(MAX_HEIGHT * 5 / 100)) //幅值大于5%取点
		{
			peakDefectInfos[i][0] = gPeakDefectInfos[i][0];
		}
	}
	LeaveCriticalSection(&csPeakDefectInfos);
	DeleteCriticalSection(&csPeakDefectInfos);
	
	if(gstMeasureDisplay.wMethod == 0)
	{
		SetQiDian(NULL, 1, fGate1QianDian,fGate1Back, FALSE);

		GateInfos[CurrentChannelNo][0].fPeakGate = 62;
		chCurrent_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 62 / 100.0f);
		
		GateInfos[CurrentChannelNo][0].bOpen = ON;
		chCurrent_gate_reg->wGate1Enable = ON;
		Sleep(15);
		
		fRange1 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][0].wDefectTime);
		gstMeasureDisplay.stMin.fValue = fRange1;
	}
	else if(gstMeasureDisplay.wMethod == 2)
	{
		SetQiDian(NULL, 1, fGate1QianDian,fGate1Back, FALSE);
		SetQiDian(NULL, 2, fGate2QianDian,fGate2Back, FALSE);

		GateInfos[CurrentChannelNo][0].fPeakGate = 62;
		chCurrent_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 62 / 100.0f);
		GateInfos[CurrentChannelNo][1].fPeakGate = 62;
		chCurrent_gate_reg->wGate2Peak = WORD(MAX_HEIGHT * 62 / 100.0f);

		GateInfos[CurrentChannelNo][0].bOpen = ON;
		chCurrent_gate_reg->wGate1Enable = ON;
		GateInfos[CurrentChannelNo][1].bOpen = ON;
		chCurrent_gate_reg->wGate2Enable = ON;
		Sleep(15);
		
		fRange1 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][0].wDefectTime);
		fRange2 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][1].wDefectTime);
		gstMeasureDisplay.stMin.fValue = fRange2 - fRange1;
	}*/
	while(1)
	{
		InitializeCriticalSection(&csMeasure);
		EnterCriticalSection(&csMeasure);
		
		BOOL bMeasureSwitch = gbMeasureSwitch;
		
		LeaveCriticalSection(&csMeasure);
		DeleteCriticalSection(&csMeasure);

		memset(peakDefectInfos, 0, sizeof(peakDefectInfos));
		InitializeCriticalSection(&csPeakDefectInfos);
		EnterCriticalSection(&csPeakDefectInfos);
		for(int i = 0 ;i < CHANNEL_NUM; i++)
		{
			for(int j = 0; j < 2; j++)
			{
				if(gPeakDefectInfos[i][j].wDefectAmp > double(MAX_HEIGHT * 5 / 100)) //幅值大于5%取点
				{
					peakDefectInfos[i][j] = gPeakDefectInfos[i][j];
				}
			}
		}
		LeaveCriticalSection(&csPeakDefectInfos);
		DeleteCriticalSection(&csPeakDefectInfos);
	
		fGateWidth = (DisplaySet_ad[CurrentChannelNo].fRange / 100) * 4;//保持闸门宽度在屏幕上一致
		
		if(bMeasureSwitch == ON)
		{
			if((gCurrentWnd == hWndChDisplay  || gCurrentWnd == hWndChAverage)&& !gbHotGateFlag && !gbHotAutoGainFlag && !gbHotAutoAdjustFlag)//GetForegroundWindow() == hBtnDisplaySwitch || GetForegroundWindow() == hBtnDisplayAverage无用？
			{
				if(gstMeasureDisplay.wMethod == 0 || gstMeasureDisplay.wMethod == 1)
				{
					//先关掉两个闸门 比如底波底波法切有脉冲回波法时就需要先关掉所有闸门
					GateInfos[CurrentChannelNo][0].bOpen = OFF;
					chCurrent_gate_reg->wGate1Enable = OFF;
					GateInfos[CurrentChannelNo][1].bOpen = OFF;
					chCurrent_gate_reg->wGate2Enable = OFF;
					
					fFirstHuiBoDistance = firstHuiBoDistance();
					fGate1QianDian = fFirstHuiBoDistance - fGateWidth / 2;
					fGate1Back = fGate1QianDian + fGateWidth;
					
					SetQiDian(NULL, 1, fGate1QianDian,fGate1Back, FALSE);
					
					GateInfos[CurrentChannelNo][0].fPeakGate = 62;
					chCurrent_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 62 / 100.0f);
					
					GateInfos[CurrentChannelNo][0].bOpen = ON;
					chCurrent_gate_reg->wGate1Enable = ON;
					Sleep(5);
					
					fRange1 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][0].wDefectTime);
				//	gstMeasureDisplay.stCurrent.fValue = fRange1;
					fCurrentValue[wCount] = fRange1;
				}
				else if(gstMeasureDisplay.wMethod == 2 || gstMeasureDisplay.wMethod == 3)
				{
					fFirstHuiBoDistance = firstHuiBoDistance();
					fSecondHuiBoDistance = secondHuiBoDistance();
					fGate1QianDian = fFirstHuiBoDistance - fGateWidth / 2;
					fGate2QianDian = fSecondHuiBoDistance - fGateWidth / 2;
					fGate1Back = fGate1QianDian + fGateWidth;
					fGate2Back = fGate2QianDian + fGateWidth;
					
					SetQiDian(NULL, 1, fGate1QianDian,fGate1Back, FALSE);
					SetQiDian(NULL, 2, fGate2QianDian,fGate2Back, FALSE);
					
					GateInfos[CurrentChannelNo][0].fPeakGate = 62;
					chCurrent_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 62 / 100.0f);
					GateInfos[CurrentChannelNo][1].fPeakGate = 62;
					chCurrent_gate_reg->wGate2Peak = WORD(MAX_HEIGHT * 62 / 100.0f);

					GateInfos[CurrentChannelNo][0].bOpen = ON;
					chCurrent_gate_reg->wGate1Enable = ON;
					GateInfos[CurrentChannelNo][1].bOpen = ON;
					chCurrent_gate_reg->wGate2Enable = ON;
					Sleep(5);
					
					fRange1 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][0].wDefectTime);
					fRange2 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][1].wDefectTime);
				//	gstMeasureDisplay.stCurrent.fValue = fRange2 - fRange1;
					fCurrentValue[wCount] = fRange2 - fRange1;
				}
				wCount++;
				if(wCount == AVER_NUM)
				{
					BOOL bSameFlag[AVER_NUM];
					memset(bSameFlag, 0 ,sizeof(bSameFlag));
					WORD wSameCount[AVER_NUM];
					memset(wSameCount, 1 ,sizeof(wSameCount));
					float fTemp = 0;
					//从小到大排序
					for(int i = 0; i < wCount - 1; i++)
					{
						for(int j = i + 1; j < wCount; j++)
						{
							if(fCurrentValue[i] > fCurrentValue[j]) 
							{
								fTemp = fCurrentValue[j];
								fCurrentValue[j] = fCurrentValue[i];
								fCurrentValue[i] = fTemp;
							}
						}
					}
				/*	for(int i = 1; i < wCount ; i++)//都跟第一个数比较
					{
						fCurrentValue[0] = TRUE;
						if(fabs(WORD(fCurrentValue[i]) - WORD(fCurrentValue[0])) < 1E-6)
						{
							bSameFlag[i] = TRUE;
							wSameCount[0]++;
						}
					}*/
					for(int i = 0; i < wCount ; i++)
					{
						if(!bSameFlag[i])	//这个数不跟其他数相同
						{
							bSameFlag[i] = TRUE;   //下次不再用来做比较
							for(int j = i + 1; j < wCount; j++)
							{
								if(fabs(WORD(fCurrentValue[j]) - WORD(fCurrentValue[i])) < 1E-6)
								{
									bSameFlag[j] = TRUE;
									wSameCount[i]++;
								}
							}
						}
					}
					WORD wMaxCountPosition = 0;	//计数最大的位置
					WORD wTempCount = wSameCount[0]; //临时保存最大的数
					//取计数最多的位置
					for(int i = 0; i < wCount; i++)
					{
						if(wTempCount < wSameCount[i])
						{
							wTempCount = wSameCount[i];
							wMaxCountPosition = i;
						}
					}
					float fAllCurrentValue = 0.0f;
					for(int i = 0; i < wCount; i++)
					{
						if(fabs(WORD(fCurrentValue[i]) - WORD(fCurrentValue[wMaxCountPosition])) < 1E-6)
						{
							fAllCurrentValue += fCurrentValue[i];
						}
					}
					gstMeasureDisplay.stCurrent.fValue = fAllCurrentValue / wSameCount[wMaxCountPosition];
			//		displayMeasureAverage(hBtnDisplayAverage, (float)wSameCount[wMaxCountPosition]);
			//		displayMeasureAverage(hBtnDisplayTuCeng, fAllCurrentValue);
					wCount = 0;
					if(gbHeadVCurveFlag == ON)
					{
						HEADVPOINT_DATA stTempPointData1; 
						HEADVPOINT_DATA stTempPointData2;
						memset(&stTempPointData1, 0, sizeof(HEADVPOINT_DATA));
						memset(&stTempPointData1, 0, sizeof(HEADVPOINT_DATA));
						if(gwHeadVPointNum[gwHeadVHeadNum] == 1)
						{
							if(gstMeasureDisplay.stCurrent.fValue == gstHeadVPointData[gwHeadVHeadNum][0].fActualHouDu)
								gstMeasureDisplay.stCurrent.fValue = gstHeadVPointData[gwHeadVHeadNum][0].fMeasureHouDu;
						}
						for(WORD i = 0; i < gwHeadVPointNum[gwHeadVHeadNum] - 1; i++)
						{
							stTempPointData1 = gstHeadVPointData[gwHeadVHeadNum][i];
							stTempPointData2 = gstHeadVPointData[gwHeadVHeadNum][i + 1];
							if(gstMeasureDisplay.stCurrent.fValue >= gstHeadVPointData[gwHeadVHeadNum][i].fActualHouDu
								&& gstMeasureDisplay.stCurrent.fValue <= gstHeadVPointData[gwHeadVHeadNum][i + 1].fActualHouDu)
							{
								if(stTempPointData2.fActualHouDu- stTempPointData1.fActualHouDu != 0)
								{
									gstMeasureDisplay.stCurrent.fValue = (stTempPointData2.fMeasureHouDu - stTempPointData1.fMeasureHouDu) / (stTempPointData2.fActualHouDu- stTempPointData1.fActualHouDu)
																		* (gstMeasureDisplay.stCurrent.fValue - stTempPointData1.fActualHouDu) + stTempPointData1.fMeasureHouDu;
								}
							}
						}
					}
					else if(gbTempCurveFlag)
					{
						TEMPCURVEPOINT_DATA stTempPointData1; 
						TEMPCURVEPOINT_DATA stTempPointData2;
						memset(&stTempPointData1, 0, sizeof(TEMPCURVEPOINT_DATA));
						memset(&stTempPointData1, 0, sizeof(TEMPCURVEPOINT_DATA));
						WORD wCurrentWaveSpeed = 0;		//当前温度声速
						if(gwTempCurvePointNum[gwTempCurveMaterial] == 1)
						{
							wCurrentWaveSpeed = gstTempCurvePointData[gwTempCurveMaterial][0].wWaveSpeed;
						}
						for(WORD i = 0; i < gwTempCurvePointNum[gwTempCurveMaterial] - 1; i++)
						{
							stTempPointData1 = gstTempCurvePointData[gwTempCurveMaterial][i];
							stTempPointData2 = gstTempCurvePointData[gwTempCurveMaterial][i + 1];
							if(gfCurrentTemperature>= gstTempCurvePointData[gwTempCurveMaterial][i].fTemperature
								&& gfCurrentTemperature <= gstTempCurvePointData[gwTempCurveMaterial][i + 1].fTemperature)
							{
								if(stTempPointData2.fTemperature- stTempPointData1.fTemperature != 0)
								{
									wCurrentWaveSpeed = (WORD)(((float)stTempPointData2.wWaveSpeed - (float)stTempPointData1.wWaveSpeed) / (stTempPointData2.fTemperature- stTempPointData1.fTemperature)
																		* (gfCurrentTemperature - stTempPointData1.fTemperature) + stTempPointData1.wWaveSpeed);
								}
							}
						}
						gstMeasureDisplay.stCurrent.fValue = gstMeasureDisplay.stCurrent.fValue * ((float)wCurrentWaveSpeed / DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
					}	

					if(gstMeasureDisplay.stCurrent.bSwitch == ON)
						displayMeasureCurrent(hBtnDisplayCurrent, gstMeasureDisplay.stCurrent.fValue);

					if(gstMeasureDisplay.wChaZhiMode == 0)			//绝对值
						gstMeasureDisplay.stChaZhi.fValue = gstMeasureDisplay.stCurrent.fValue - gstMeasureDisplay.fBiaoChengZhi;
					else if(gstMeasureDisplay.wChaZhiMode == 1)		//相对值
						gstMeasureDisplay.stChaZhi.fValue = (gstMeasureDisplay.stCurrent.fValue - gstMeasureDisplay.fBiaoChengZhi) / gstMeasureDisplay.fBiaoChengZhi;
					if(gstMeasureDisplay.stChaZhi.bSwitch == ON)
						displayMeasureChaZhi(hBtnDisplayChaZhi, gstMeasureDisplay.wChaZhiMode, gstMeasureDisplay.stChaZhi.fValue);
					
					if(gstMeasureDisplay.stMax.fValue < gstMeasureDisplay.stCurrent.fValue)
					{
						gstMeasureDisplay.stMax.fValue = gstMeasureDisplay.stCurrent.fValue;
						if(gstMeasureDisplay.stMax.fValue < 0)
							gstMeasureDisplay.stMax.fValue = 0;
					}
					
					if(gstMeasureDisplay.stMax.bSwitch == ON)
						displayMeasureMax(hBtnDisplayMax, gstMeasureDisplay.stMax.fValue);

					if((gstMeasureDisplay.stMin.fValue > gstMeasureDisplay.stCurrent.fValue) &&(gstMeasureDisplay.stCurrent.fValue != 0))
					{
						gstMeasureDisplay.stMin.fValue = gstMeasureDisplay.stCurrent.fValue;
						if(gstMeasureDisplay.stMin.fValue < 0)
							gstMeasureDisplay.stMin.fValue = 0;
					}
					if(gstMeasureDisplay.stMin.bSwitch == ON)
						displayMeasureMin(hBtnDisplayMin, gstMeasureDisplay.stMin.fValue);
					
					if(gstMeasureDisplay.wMethod == 3)
					{
							gstMeasureDisplay.fTuCengHouDu = (fRange1 - (fRange2 - fRange1))	//脉冲底波法求出的值-底波底波求出的值
											*((float)gwTuCengSpeed/DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
							
							displayMeasureTuCeng(hBtnDisplayTuCeng, gstMeasureDisplay.fTuCengHouDu, gstMeasureDisplay.wMethod);
					}
				}
			//	displayMeasureAverage(hBtnDisplayAverage, gstMeasureDisplay.stAverage.fValue);
			}
			else
			{
				if(gstMeasureDisplay.wMethod == 0)
				{
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
				}
				else if(gstMeasureDisplay.wMethod == 2 || gstMeasureDisplay.wMethod == 3)
				{
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
				}
				SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
				SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
				SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				hDrawMeasureThread = NULL;
				return 0;
			}
		}
		else
		{
			if(gstMeasureDisplay.wMethod == 0)
			{
				SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
			}
			else if(gstMeasureDisplay.wMethod == 2 || gstMeasureDisplay.wMethod == 3)
			{
				SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
				SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
			}
			SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
			SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
			SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
			SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
			hDrawMeasureThread = NULL;
			return 0;
		}
		Sleep(20);
	}
	return 0;
}


/**************************
*注册测厚测量显示窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChDisplayChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = (WNDPROC)chDisplayChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChDisplayChild;

	return RegisterClass(&wc);
}

/**************************
*开关按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnDisplaySwitchProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
				}			
				else if(wParam == VK_UP)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
					if(!hWndChMenu && !bFreezeUp)
					{
						ShowWindow(hWndChDisplay, SW_HIDE);


						hWndChMenu = CreateWindow(szChMenuChild, TEXT(""), WS_CHILD,
									 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);//创建测厚菜单窗口
						ShowWindow(hWndChMenu,SW_SHOW);
						UpdateWindow(hWndChMenu);

						gCurrentWnd = hWndChMenu;

						// close other hotkey
						gbHotDB = FALSE;
					//	gDBIndex = 0;

						memset(gszTextPlus, 0, sizeof(gszTextPlus));
						swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

						memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
						swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);

						InitializeCriticalSection(&csHotRange);
						EnterCriticalSection(&csHotRange);
						gbHotRange = FALSE;
						LeaveCriticalSection(&csHotRange);
						DeleteCriticalSection(&csHotRange);
					}
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					gstMeasureDisplay.stCurrent.fValue = 0;
					fCurrentValue1 = 0;
					fCurrentValue2 = 0;
					fCurrentValue3 = 0;
					fCurrentValue4 = 0;
					fCurrentValue5 = 0;
					fCurrentValue6 = 0;
					SetOpenFlag(NULL, FALSE, 1);    //先关闭闸门 否则关闭开关时 会有报警响一下
					SetOpenFlag(NULL, FALSE, 2);	//先关闭闸门 否则关闭开关时 会有报警响一下
					Sleep(100);
				 	InitializeCriticalSection(&csMeasure);
					EnterCriticalSection(&csMeasure);
					
					gbMeasureSwitch = !gbMeasureSwitch;
					
					LeaveCriticalSection(&csMeasure);
					DeleteCriticalSection(&csMeasure);	

					Sleep(10);					
					displayMeasureSwitch(Wnd, gbMeasureSwitch);
					
					

				/*	if(gstMeasureDisplay.wMethod == 0)
					{
						chCurrent_gate_reg->wGate1Enable = (WORD)gbMeasureSwitch;
						GateInfos[CurrentChannelNo][0].bOpen = gbMeasureSwitch;
					}
					else if(gstMeasureDisplay.wMethod == 2)
					{
						chCurrent_gate_reg->wGate1Enable = (WORD)gbMeasureSwitch;
						chCurrent_gate_reg->wGate2Enable = (WORD)gbMeasureSwitch;
						GateInfos[CurrentChannelNo][0].bOpen = gbMeasureSwitch;
						GateInfos[CurrentChannelNo][1].bOpen = gbMeasureSwitch;
					}
				*/	
					if(gbMeasureSwitch == ON)
					{
						if((sys_reg->wLedAlarm & 0x0001) == 0x0001)
						{
							sys_reg->wLedAlarm &= 0xFFFE;
						}
						// 创建测量线程
						if(hDrawMeasureThread == NULL)
						{
							// 创建线程							
							DrawMeasureThread_param.hWnd = hWndChDisplay;
							HDC hDC = GetDC(hWndMain);
							DrawMeasureThread_param.hDC = hDC;

							hDrawMeasureThread = ::CreateThread(NULL, 0, measureProc, (LPVOID)&DrawMeasureThread_param, 0, &dwDrawMeasureThreadID);
						}
					}					
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				
				else if(wParam == 'x' || wParam == 'X')
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
				else if(wParam == 'A')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'C')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDisplaySwitchProc,Wnd,Msg,wParam,lParam);
}


/**************************
*其它设置按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnDisplayAverageProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
				}	
				else if(wParam == VK_UP)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChAverage = CreateWindow(szChAverageChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChDisplay,SW_HIDE);
				//	SetFocus(hWndChAverage);
					ShowWindow(hWndChAverage,SW_SHOW);
					UpdateWindow(hWndChAverage);

					gCurrentWnd = hWndChAverage;
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				
				else if(wParam == 'x' || wParam == 'X')
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
				else if(wParam == 'A')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'C')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDisplayAverageProc,Wnd,Msg,wParam,lParam);
}

#if 0
/**************************
*最大值按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK BtnDisplayMaxProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					DestroyWindow(hWndChDisplay);
					hWndChDisplay = 0;
					// songchenguang add start 2010-4-26
					SetDisplayStateFlag(FALSE);
					// songchenguang add end 2010-4-26

					SetFocus(hWndMain);//主窗口获得输入焦点
				}
			
				else if(wParam == VK_UP)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
				
				}
				else if(wParam == '\t')
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDisplayMaxProc,Wnd,Msg,wParam,lParam);
}

/**************************
*最小值按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnDisplayMinProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					DestroyWindow(hWndChDisplay);
					hWndChDisplay = 0;
					// songchenguang add start 2010-4-26
					SetDisplayStateFlag(FALSE);
					// songchenguang add end 2010-4-26

					SetFocus(hWndMain);//主窗口获得输入焦点
				}
			
				else if(wParam == VK_UP)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDisplayMinProc,Wnd,Msg,wParam,lParam);
}

/**************************
*当前值按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnDisplayCurrentProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					DestroyWindow(hWndChDisplay);
					hWndChDisplay = 0;
					// songchenguang add start 2010-4-26
					SetDisplayStateFlag(FALSE);
					// songchenguang add end 2010-4-26
					SetFocus(hWndMain);//主窗口获得输入焦点
				}
			
				else if(wParam == VK_UP)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{					
				}
				else if(wParam == '\t')
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDisplayCurrentProc,Wnd,Msg,wParam,lParam);
}
/**************************
*差值按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnDisplayChaZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					DestroyWindow(hWndChDisplay);
					hWndChDisplay = 0;
					// songchenguang add start 2010-4-26
					SetDisplayStateFlag(FALSE);
					// songchenguang add end 2010-4-26
					SetFocus(hWndMain);//主窗口获得输入焦点
				}
			
				else if(wParam == VK_UP)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam ==VK_DOWN)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{					
				}
				else if(wParam == '\t')
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDisplayChaZhiProc,Wnd,Msg,wParam,lParam);
}
#endif
/**************************
*测厚测量显示窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK chDisplayChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			gstMeasureDisplay.wMethod = 2;
			gstMeasureDisplay.fBiaoChengZhi = 0.1;
			gstMeasureDisplay.stAverage.bSwitch = ON;
			gstMeasureDisplay.stMax.bSwitch = ON;
			gstMeasureDisplay.stMin.bSwitch = ON;
			gstMeasureDisplay.stCurrent.bSwitch = ON;
			gstMeasureDisplay.stChaZhi.bSwitch = ON;

			gwTuCengSpeed = tuCengTypeToWaveSpeed(gwTuCengType);
			
			hBtnDisplaySwitch = CreateWindow(_T("BUTTON"),_T("displaySwitch"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_SWITCH,hInst,NULL);

			hBtnDisplayAverage = CreateWindow(_T("BUTTON"),_T("displayAverage"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_AVERAGE,hInst,NULL);
		
			hBtnDisplayCurrent = CreateWindow(_T("BUTTON"),_T("displayCurrentValue"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_CURRENT,hInst,NULL);
			
			hBtnDisplayMax = CreateWindow(_T("BUTTON"),_T("displayMax"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_MAX,hInst,NULL);

			hBtnDisplayMin= CreateWindow(_T("BUTTON"),_T("displayMin"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_MIN,hInst,NULL);

			hBtnDisplayChaZhi = CreateWindow(_T("BUTTON"),_T("displayChaZhi"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_CHAZHI,hInst,NULL);
			
			hBtnDisplayTuCeng = CreateWindow(_T("BUTTON"),_T("displayTuCeng"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 6*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_TUCENG,hInst,NULL);

			chDisplayBtnList[0] = hBtnDisplaySwitch;
			chDisplayBtnList[1] = hBtnDisplayAverage;

			//按钮控件的自定义处理过程
			OldBtnDisplaySwitchProc = (WNDPROC) GetWindowLong(hBtnDisplaySwitch,GWL_WNDPROC);
		    SetWindowLong(hBtnDisplaySwitch,GWL_WNDPROC,(LONG)btnDisplaySwitchProc);
			OldBtnDisplayAverageProc = (WNDPROC) GetWindowLong(hBtnDisplayAverage,GWL_WNDPROC);
		    SetWindowLong(hBtnDisplayAverage,GWL_WNDPROC,(LONG)btnDisplayAverageProc);
		/*				
			OldBtnDisplayMaxProc = (WNDPROC) GetWindowLong(hBtnDisplayMax,GWL_WNDPROC);
			SetWindowLong(hBtnDisplayMax,GWL_WNDPROC,(LONG)BtnDisplayMaxProc);
			
			OldBtnDisplayMinProc = (WNDPROC) GetWindowLong(hBtnDisplayMin,GWL_WNDPROC);
			SetWindowLong(hBtnDisplayMin,GWL_WNDPROC,(LONG)btnDisplayMinProc);
			
			OldBtnDisplayCurrentProc = (WNDPROC) GetWindowLong(hBtnDisplayCurrent,GWL_WNDPROC);
			SetWindowLong(hBtnDisplayCurrent,GWL_WNDPROC,(LONG)btnDisplayCurrentProc);
			
			OldBtnDisplayChaZhiProc = (WNDPROC) GetWindowLong(hBtnDisplayChaZhi,GWL_WNDPROC);
			SetWindowLong(hBtnDisplayChaZhi,GWL_WNDPROC,(LONG)btnDisplayChaZhiProc);
			*/
			currentChDisplayBtn = chDisplayBtnList[0];//默认当前选中开关按钮
			SetFocus(currentChDisplayBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentChDisplayBtn;

			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndChDisplay, &ps);
						
			if(gbMeasureSwitch == ON)
			{
				closeAllGate();
				// 创建测量线程
				if(hDrawMeasureThread == NULL)
				{
					// 创建线程							
					DrawMeasureThread_param.hWnd = hWnd;
					HDC hDC = GetDC(hWndMain);
					DrawMeasureThread_param.hDC = hDC;

					hDrawMeasureThread = ::CreateThread(NULL, 0, measureProc, (LPVOID)&DrawMeasureThread_param, 0, &dwDrawMeasureThreadID);
				}
			}

			EndPaint(hWndChDisplay, &ps);
			break;
		case WM_KEYDOWN:
			break;
		case WM_DRAWITEM://重画按钮
			pdis=(LPDRAWITEMSTRUCT)lParam;
			chDisplay_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnDisplaySwitch)
			{
				displayMeasureSwitch(pdis->hwndItem, gbMeasureSwitch);
			}
			if(pdis->hwndItem == hBtnDisplayAverage)
			{
				displayMeasureAverage(pdis->hwndItem, gstMeasureDisplay.stAverage.fValue);
			}
			if(pdis->hwndItem == hBtnDisplayCurrent)
			{
				displayMeasureCurrent(pdis->hwndItem, gstMeasureDisplay.stCurrent.fValue);
			}
			if(pdis->hwndItem == hBtnDisplayMax)
			{
				displayMeasureMax(pdis->hwndItem, gstMeasureDisplay.stMax.fValue);
			}
			if(pdis->hwndItem == hBtnDisplayMin)
			{
				displayMeasureMin(pdis->hwndItem, gstMeasureDisplay.stMin.fValue);
			}			
			if(pdis->hwndItem == hBtnDisplayChaZhi)
			{
				displayMeasureChaZhi(pdis->hwndItem, gstMeasureDisplay.wChaZhiMode, gstMeasureDisplay.stChaZhi.fValue);
			}
			if(pdis->hwndItem == hBtnDisplayTuCeng)
			{
				displayMeasureTuCeng(pdis->hwndItem, gstMeasureDisplay.fTuCengHouDu, gstMeasureDisplay.wMethod);
			}

			if(gbMeasureSwitch == ON)
			{
				// 创建测量线程
				if(hDrawMeasureThread == NULL)
				{
					// 创建线程							
					DrawMeasureThread_param.hWnd = hWnd;
					HDC hDC = GetDC(hWndMain);
					DrawMeasureThread_param.hDC = hDC;

					hDrawMeasureThread = ::CreateThread(NULL, 0, measureProc, (LPVOID)&DrawMeasureThread_param, 0, &dwDrawMeasureThreadID);
				}
			}
			break;
		case WM_COMMAND:
			break;
		case WM_CLOSE:
		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/**************************
*加载测厚测量显示窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chDisplay_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_smBtnBitmap[]=
	{
		{IDC_CHDISPLAY_SWITCH,	IDB_BITMAP_KAIQIGUANBI6_NOSEL_R	 ,  IDB_BITMAP_KAIQIGUANBI6_SELECT_R		},
		{IDC_CHDISPLAY_AVERAGE	,IDB_BITMAP_PINGJUNZHI_NOSEL_R ,  IDB_BITMAP_PINGJUNZHI_SELECT_R	},
		{IDC_CHDISPLAY_CURRENT	,IDB_BITMAP_DANGQIANZHI_NOSEL_R	 ,  IDB_BITMAP_DANGQIANZHI_NOSEL_R		},
		{IDC_CHDISPLAY_MAX	,IDB_BITMAP_ZHUIDAZHI_NOSEL_R	 ,  IDB_BITMAP_ZHUIDAZHI_NOSEL_R		},
		{IDC_CHDISPLAY_MIN	,IDB_BITMAP_ZHUIXIAOZHI_NOSEL_R	 ,  IDB_BITMAP_ZHUIXIAOZHI_NOSEL_R		},
		{IDC_CHDISPLAY_CHAZHI	,IDB_BITMAP_CHAZHI_NOSEL_R ,  IDB_BITMAP_CHAZHI_NOSEL_R	},
		{IDC_CHDISPLAY_TUCENG	,IDB_BITMAP_TUCENGHOUDU_NOSEL_R ,  IDB_BITMAP_TUCENGHOUDU_NOSEL_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_smBtnBitmap);
	return ReturnVal;
}
