/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 修改AVG曲线窗口源文件
 * Filename: ModifyAvgCurveWin.cpp
 *
 * Spec: Trace Tag write here. If has not, OnOff this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"
#include "interfaceCmdID.h"
#include "const.h"
#include "resource.h"
#include "craftParamWin.h"
#include "gateWin.h"
#include "recvParamWin.h"


HWND hWndModifyAvgCurve = NULL;//修改AVG曲线窗口句柄
HWND hBtnModifyAvgCurveAddPoint = NULL;//加样点按钮句柄
HWND hBtnModifyAvgCurveSelPoint = NULL;//选择样点按钮句柄
HWND hBtnModifyAvgCurveModifyAmp = NULL;//修改幅值按钮句柄
HWND hBtnModifyAvgCurveDeletePoint = NULL;//删样点按钮句柄
HWND hBtnModifyAvgCurveEnd = NULL;//结束按钮句柄
HWND ModifyAvgCurveBtnList[5] = {NULL,NULL,NULL, NULL, NULL};//修改AVG曲线窗口的按钮表
WNDPROC OldBtnModifyAvgCurveAddPointProc = NULL;//加样点按钮默认处理过程
WNDPROC OldBtnModifyAvgCurveSelPointProc = NULL;//选择样点按钮默认处理过程
WNDPROC OldBtnModifyAvgCurveModifyAmpProc = NULL;//修改幅值按钮默认处理过程
WNDPROC OldBtnModifyAvgCurveDeletePointProc = NULL;//删样点按钮默认处理过程
WNDPROC OldBtnModifyAvgCurveEndProc = NULL;//结束按钮默认处理过程
HWND currentModifyAvgCurveBtn = NULL;//修改AVG曲线窗中当前选中的按钮

extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//添加的样点数据队列
extern CURVEPOINT_DATA gOldMakeAvgPointData[MAX_CURVE_POINT_NUM];//以加第一点基准增益为基准的制作线

extern CURVEPOINT_DATA gCurrentSelectedAvgPointData;//添加的样点中当前选中的AVG曲线点数据
extern CURVEPOINT_DATA gPreSelectedAvgPointData;//添加的样点中前一个选中的AVG曲线点数据
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//已选中的AVG点数量
extern float gfAvgFirstPointGain[CHANNEL_NUM];		//添加的点第一个点对应的Db值
extern float gfAvgFirstPointChangeGain[CHANNEL_NUM];			//制作时增益键改变的增益值

//WORD *gpAvgPixels[SCREEN_DOT_NUM];		//AVG曲线像素点

extern BOOL gWaveDrawAvgFlag;		//波形是否画AVG
extern BOOL gbDeletePointFlag;		//是否在删样点操作
extern float gfOldAvgRange[CHANNEL_NUM];				//最初生成AVG时的声程
extern short gwOldAvgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//最初生成AVG时的每个点的位置
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];

//static int iAddStep = 0;
//static int iDeleteStep = 0;
static POINT stAvgPoint[MAX_CURVE_POINT_NUM] = {0};
extern WORD gwDeleteStep;		//删样点位置

static const int buttonNum = 5;//按钮控件数
static BOOL gbOldAvgCurveFlag[CHANNEL_NUM];		//保存AVG曲线开关标志
static BOOL gOldDgsSwitch[CHANNEL_NUM][GATE_NUM] = {{0,0},{0,0}};				//保存DGS开关状态

static WORD wModifyAmpStepIndex = 1;		//修改幅值步长索引
static float fModifyAmpStep = 1;			//修改幅值步长

ATOM registerModifyAvgCurveChild(HINSTANCE hInstance);
static LRESULT CALLBACK ModifyAvgCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int ModifyAvgCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnModifyAvgCurveAddPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnModifyAvgCurveSelPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnModifyAvgCurveModifyAmpProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnModifyAvgCurveDeletePointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnModifyAvgCurveEndProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

extern WORD CurrentChannelNo;					// 当前通道号
extern HINSTANCE hInst;							// current instance
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern PEAK_INFO gPeakDefectInfos[2][2];	//保存闸门峰值点信息
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern HWND hWndMakeAvgCurve;//制作AVG曲线窗口句柄
extern HWND currentMakeAvgCurveBtn;//制作AVG曲线窗中当前选中的按钮
extern HWND gCurrentWnd;		//当前显示的子窗口
extern CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// 工作线程参数
extern float fRangeStart[2];
extern float fRangeEnd[2];
extern HWND hBtnMakeAvgCurveCreate;//AVG创建按钮句柄
extern HWND hBtnMakeAvgCurveModify;//AVG修改按钮句柄
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG曲线开关标志
extern float gfDefectZhi1[CHANNEL_NUM];			//所测孔径Φ1
extern float gfDefectZhi2[CHANNEL_NUM];			//Φ2
extern float gfDefectZhi3[CHANNEL_NUM];			//Φ3
extern BOOL gbGuanLian;
extern WORD gwAvgType; //AVG类型
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern CRITICAL_SECTION csPeakDefectInfos;
extern HWND hStatusBar;
extern wchar_t gszTextStep[10];

extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//DAC添加的样点数据队列
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];//已选中的DGS点数量
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];				//DGS通道开关
extern BOOL gbHotDB;			// 快捷键增益
extern BOOL gbHotRange;			// 快捷键声程

extern void SetRefPlusLableAndExecute(HWND hWnd, const float& fBasePlus, const float& fRefPlus, const BOOL& bSave, const WORD& chNo);

static void displayPointAmp(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
		
	if(gSelectedAvgPointDataNum[CurrentChannelNo])
	{
		float fSelectPointAmp = gSelectedAvgPointData[CurrentChannelNo][3][gwDeleteStep].data * 100 / MAX_HEIGHT;
		
		wsprintf(strText, _T("%.1f"), fSelectPointAmp);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}
	else
	{
		swprintf(strText, _T("NULL"));
	}
}
static void displaySelPoint(HWND hWnd)
{	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	if(gSelectedAvgPointDataNum[CurrentChannelNo])
	{
		gCurrentSelectedAvgPointData = gSelectedAvgPointData[CurrentChannelNo][3][gwDeleteStep];
		
		swprintf(strText, _T("%.0f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedAvgPointData.serial/DATA_SIZE, (int)(gCurrentSelectedAvgPointData.data * 100 / MAX_HEIGHT));
	}
	else
	{
		swprintf(strText, _T("无样点"));
		
	}
	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

}

static void quitCurveProcess()
{
	gWaveDrawAvgFlag = FALSE;
	SetRedrawFlag(TRUE);
	//制作过程中基准增益改变，制作线以加第一点的基准增益为基准再变回去
	/*for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
	{
		gSelectedAvgPointData[CurrentChannelNo][3][i].data = (DWORD)(gSelectedAvgPointData[CurrentChannelNo][3][i].data
																	/ pow(10, (double)(gfAvgFirstPointChangeGain[CurrentChannelNo]) / (-20)));
	}*/
	if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
	{
		for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
		{
			gSelectedAvgPointData[CurrentChannelNo][3][i] = gOldMakeAvgPointData[i];
		}
	}

	ReceiveParam_ad[CurrentChannelNo].fPlus = gfAvgFirstPointGain[CurrentChannelNo];
	SetPlusLableAndExecute(NULL, gfAvgFirstPointGain[CurrentChannelNo], ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);
	
	if(!gbGuanLian)
	{
		if(gwAvgType == 1)	//大平底
		{
			DWORD dwHeadFrequence = 0;
			
			if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 0)
				dwHeadFrequence = 1000000;
			else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 1)
				dwHeadFrequence = 500000;
			else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 2)
				dwHeadFrequence = 2000000;
			else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 3)
				dwHeadFrequence = 5000000;
			else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 4)
				dwHeadFrequence = 5000000;
			else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 5)
				dwHeadFrequence = 2500000;
			
			for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
			{
				float fRange = gSelectedAvgPointData[CurrentChannelNo][3][i].serial * (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) / DATA_SIZE;
				//平底孔与大平底的声压差
				float fDeltaDB = (float)(20 * log10(PI * gfDefectZhi1[CurrentChannelNo] * gfDefectZhi1[CurrentChannelNo] / (2 *((float)DisplaySet_ad[CurrentChannelNo].nWaveSpeed  * 1000 / dwHeadFrequence) * fRange)));
				
				gSelectedAvgPointData[CurrentChannelNo][3][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data, fDeltaDB);
			}
		}
		float fDb1 = (float)(40 * log10(gfDefectZhi2[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
		float fDb2 = (float)(40 * log10(gfDefectZhi3[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
		for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
		{	
			gSelectedAvgPointData[CurrentChannelNo][0][i].data = gSelectedAvgPointData[CurrentChannelNo][3][i].data;
			gSelectedAvgPointData[CurrentChannelNo][0][i].serial = gSelectedAvgPointData[CurrentChannelNo][3][i].serial;
			
			gSelectedAvgPointData[CurrentChannelNo][1][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data, fDb1);
			gSelectedAvgPointData[CurrentChannelNo][1][i].serial = gSelectedAvgPointData[CurrentChannelNo][3][i].serial;

			gSelectedAvgPointData[CurrentChannelNo][2][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data, fDb2);
			gSelectedAvgPointData[CurrentChannelNo][2][i].serial = gSelectedAvgPointData[CurrentChannelNo][3][i].serial;

			gwOldAvgSerial[CurrentChannelNo][i] = (short)gSelectedAvgPointData[CurrentChannelNo][3][i].serial;
		}
	
		gbAvgCurveFlag[CurrentChannelNo] = ON;
		gfOldAvgRange[CurrentChannelNo] = DisplaySet_ad[CurrentChannelNo].fRange;
		
	}
	else
	{
		if(gwAvgType == 1)	//大平底
		{
			DWORD dwHeadFrequence = 0;
			
			if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 0)
				dwHeadFrequence = 1000000;
			else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 1)
				dwHeadFrequence = 500000;
			else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 2)
				dwHeadFrequence = 2000000;
			else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 3)
				dwHeadFrequence = 5000000;
			else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 4)
				dwHeadFrequence = 5000000;
			else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 5)
				dwHeadFrequence = 2500000;
			
			for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
			{
				float fRange = gSelectedAvgPointData[CurrentChannelNo][3][i].serial * (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) / DATA_SIZE;
				float fDeltaDB = (float)(20 * log10(PI * gfDefectZhi1[CurrentChannelNo] * gfDefectZhi1[CurrentChannelNo] / (2 *((float)DisplaySet_ad[CurrentChannelNo].nWaveSpeed  * 1000 / dwHeadFrequence) * fRange)));
				
				gSelectedAvgPointData[CurrentChannelNo][3][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data, fDeltaDB);
			}
		}
		float fDb1 = (float)(40 * log10(gfDefectZhi2[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
		float fDb2 = (float)(40 * log10(gfDefectZhi3[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
		for(int i = 0; i < CHANNEL_NUM; i++)
		{
			for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
			{	
				gSelectedAvgPointData[i][3][j].data = gSelectedAvgPointData[CurrentChannelNo][3][j].data;
				gSelectedAvgPointData[i][3][j].serial = gSelectedAvgPointData[CurrentChannelNo][3][j].serial;
					
				gSelectedAvgPointData[i][0][j].data = gSelectedAvgPointData[CurrentChannelNo][3][j].data;
				gSelectedAvgPointData[i][0][j].serial = gSelectedAvgPointData[CurrentChannelNo][3][j].serial;
					
				gSelectedAvgPointData[i][1][j].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][j].data, fDb1);
				gSelectedAvgPointData[i][1][j].serial = gSelectedAvgPointData[CurrentChannelNo][3][j].serial;

				gSelectedAvgPointData[i][2][j].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][j].data, fDb2);
				gSelectedAvgPointData[i][2][j].serial = gSelectedAvgPointData[CurrentChannelNo][3][j].serial;

				gwOldAvgSerial[i][j] = (short)gSelectedAvgPointData[CurrentChannelNo][3][j].serial;
			}
			gSelectedAvgPointDataNum[i] = gSelectedAvgPointDataNum[CurrentChannelNo];
			gbAvgCurveFlag[i] = ON;
			gfOldAvgRange[i] = DisplaySet_ad[CurrentChannelNo].fRange;
		}
	}
	//还原DAC开关状态				
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
		{
			gDgsSwitch[i][j] = gOldDgsSwitch[i][j];
		}
	}
	SaveParamFile(lpDefaultParamFile);
}

/**************************
*注册修改AVG曲线窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerModifyAvgCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));

	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) ModifyAvgCurveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szModifyAvgCurveChild;

	return RegisterClass(&wc);
}
/**************************
*修改AVG曲线窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK ModifyAvgCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
				memset(gOldMakeAvgPointData, 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
				//保存AVG开关状态
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					gbOldAvgCurveFlag[i] = gbAvgCurveFlag[i];
				}
				//关闭AVG 方便做曲线
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					gbAvgCurveFlag[i] = OFF;
				}
				//保存DAC开关状态					
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
					{
						gOldDgsSwitch[i][j] = gDgsSwitch[i][j];
					}
				}
				//关闭DAC 方便做曲线
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
					{
						gDgsSwitch[i][j] = OFF;
					}
				}
				gwDeleteStep = 0;
				gbDeletePointFlag = FALSE;
				float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
				ReceiveParam_ad[CurrentChannelNo].fRefPlus = 0;
				SetRefPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);
				refGainChangeCurve(fOldRefPlus, FALSE);//wangbingfu add at 2011-03-09
				
				gfAvgFirstPointChangeGain[0] = 0;
				gfAvgFirstPointChangeGain[1] = 0;

				if(currentMakeAvgCurveBtn == hBtnMakeAvgCurveCreate)
				{
					gSelectedAvgPointDataNum[CurrentChannelNo] = 0;

					memset(gSelectedAvgPointData[CurrentChannelNo][0], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
					memset(gSelectedAvgPointData[CurrentChannelNo][1], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
					memset(gSelectedAvgPointData[CurrentChannelNo][2], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
					memset(gSelectedAvgPointData[CurrentChannelNo][3], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);

					memset(&gCurrentSelectedAvgPointData, 0, sizeof(CURVEPOINT_DATA));

					memset(gwOldAvgSerial[CurrentChannelNo], 0, sizeof(WORD) * MAX_CURVE_POINT_NUM);
				}
				else if(currentMakeAvgCurveBtn == hBtnMakeAvgCurveModify)
				{
					if(gwAvgType == 1)	//大平底
					{
						DWORD dwHeadFrequence = 0;
							
							if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 0)
								dwHeadFrequence = 1000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 1)
								dwHeadFrequence = 500000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 2)
								dwHeadFrequence = 2000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 3)
								dwHeadFrequence = 5000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 4)
								dwHeadFrequence = 5000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 5)
								dwHeadFrequence = 2500000;
							
						for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
						{
							float fRange = gSelectedAvgPointData[CurrentChannelNo][3][i].serial * (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) / DATA_SIZE;
							//大平底与平底孔的声压差
							float fDeltaDB = (float)(20 * log10(PI * gfDefectZhi1[CurrentChannelNo] * gfDefectZhi1[CurrentChannelNo] / (2 *((float)DisplaySet_ad[CurrentChannelNo].nWaveSpeed  * 1000 / dwHeadFrequence) * fRange)));
							
							gSelectedAvgPointData[CurrentChannelNo][3][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data, -fDeltaDB);
						}
					}
				}
				else
				{
					;//
				}
				memset(stAvgPoint, 0, sizeof(POINT) * MAX_CURVE_POINT_NUM);
				for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
				{
					stAvgPoint[i] = dataToPoint(gSelectedAvgPointData[CurrentChannelNo][3][i]);
				}
				gWaveDrawAvgFlag = TRUE;

				SetRedrawFlag(TRUE);

				if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
				{
					for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
					{
						gOldMakeAvgPointData[i] = gSelectedAvgPointData[CurrentChannelNo][3][i];
					}
				}
				
				//画按钮控件
				hBtnModifyAvgCurveAddPoint = CreateWindow(_T("BUTTON"),_T("avgCurveAddPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYAVGCURVE_ADDPOINT,hInst,NULL);	

				hBtnModifyAvgCurveSelPoint = CreateWindow(_T("BUTTON"),_T("avgCurveSelPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYAVGCURVE_SELPOINT,hInst,NULL);

				hBtnModifyAvgCurveModifyAmp = CreateWindow(_T("BUTTON"),_T("avgCurveModifyAmp"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYAVGCURVE_MODIFYAMP,hInst,NULL);

				hBtnModifyAvgCurveDeletePoint = CreateWindow(_T("BUTTON"),_T("avgCurveDeletePoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYAVGCURVE_DELETEPOINT,hInst,NULL);
				
				hBtnModifyAvgCurveEnd = CreateWindow(_T("BUTTON"),_T("avgCurveEnd"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYAVGCURVE_END,hInst,NULL);
			
				ModifyAvgCurveBtnList[0] = hBtnModifyAvgCurveAddPoint;
				ModifyAvgCurveBtnList[1] = hBtnModifyAvgCurveSelPoint;
				ModifyAvgCurveBtnList[2] = hBtnModifyAvgCurveModifyAmp;
				ModifyAvgCurveBtnList[3] = hBtnModifyAvgCurveDeletePoint;
				ModifyAvgCurveBtnList[4] = hBtnModifyAvgCurveEnd;
				//按钮控件的自定义处理过程
				OldBtnModifyAvgCurveAddPointProc = (WNDPROC) GetWindowLong(hBtnModifyAvgCurveAddPoint,GWL_WNDPROC);
				SetWindowLong(hBtnModifyAvgCurveAddPoint,GWL_WNDPROC,(LONG)btnModifyAvgCurveAddPointProc);

				OldBtnModifyAvgCurveSelPointProc = (WNDPROC) GetWindowLong(hBtnModifyAvgCurveSelPoint,GWL_WNDPROC);
				SetWindowLong(hBtnModifyAvgCurveSelPoint,GWL_WNDPROC,(LONG)btnModifyAvgCurveSelPointProc);
				
				OldBtnModifyAvgCurveModifyAmpProc = (WNDPROC) GetWindowLong(hBtnModifyAvgCurveModifyAmp,GWL_WNDPROC);
				SetWindowLong(hBtnModifyAvgCurveModifyAmp,GWL_WNDPROC,(LONG)btnModifyAvgCurveModifyAmpProc);

				OldBtnModifyAvgCurveDeletePointProc = (WNDPROC) GetWindowLong(hBtnModifyAvgCurveDeletePoint,GWL_WNDPROC);
				SetWindowLong(hBtnModifyAvgCurveDeletePoint,GWL_WNDPROC,(LONG)btnModifyAvgCurveDeletePointProc);

				OldBtnModifyAvgCurveEndProc = (WNDPROC) GetWindowLong(hBtnModifyAvgCurveEnd,GWL_WNDPROC);
				SetWindowLong(hBtnModifyAvgCurveEnd,GWL_WNDPROC,(LONG)btnModifyAvgCurveEndProc);
			
				currentModifyAvgCurveBtn = ModifyAvgCurveBtnList[0];  //默认当前选中加样点参数按钮
				SetFocus(currentModifyAvgCurveBtn);//选中按钮获得输入焦点
				gCurrentFocusBtn = currentModifyAvgCurveBtn;

			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndMakeAvgCurve,SW_SHOW);
				SetFocus(currentMakeAvgCurveBtn);//上级菜单最后选中的按钮获得输入焦点

				DestroyWindow(hWndModifyAvgCurve);
				hWndModifyAvgCurve = 0;
				
				gCurrentFocusBtn = currentMakeAvgCurveBtn;

				gCurrentWnd = hWndMakeAvgCurve;

				quitCurveProcess();
				
				//还原AVG开关状态
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					gbAvgCurveFlag[i] = gbOldAvgCurveFlag[i];
				}
				//还原DAC开关状态				
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
					{
						gDgsSwitch[i][j] = gOldDgsSwitch[i][j];
					}
				}
			/*	//换算成平底孔
				if(gwAvgType == 1)	//大平底
				{
					DWORD dwHeadFrequence = 0;
					
					if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 0)
						dwHeadFrequence = 1000000;
					else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 1)
						dwHeadFrequence = 500000;
					else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 2)
						dwHeadFrequence = 2000000;
					else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 3)
						dwHeadFrequence = 5000000;
					else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 4)
						dwHeadFrequence = 5000000;
					else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 5)
						dwHeadFrequence = 2500000;
					
					for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
					{
						float fRange = gSelectedAvgPointData[CurrentChannelNo][3][i].serial * (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) / DATA_SIZE;
						//平底孔与大平底的声压差
						float fDeltaDB = (float)(20 * log10(PI * gfDefectZhi1[CurrentChannelNo] * gfDefectZhi1[CurrentChannelNo] / (2 *((float)DisplaySet_ad[CurrentChannelNo].nWaveSpeed  * 1000 / dwHeadFrequence) * fRange)));
						
						gSelectedAvgPointData[CurrentChannelNo][3][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data, fDeltaDB);
					}
				}

				gWaveDrawAvgFlag = FALSE;
				SetRedrawFlag(TRUE);*/
			}
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:			
			pdis=(LPDRAWITEMSTRUCT)lParam;
			ModifyAvgCurve_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnModifyAvgCurveSelPoint)
			{
				displaySelPoint(pdis->hwndItem);
			}
			if(pdis->hwndItem == hBtnModifyAvgCurveModifyAmp)
			{
				displayPointAmp(pdis->hwndItem);
			}
			
			// 检测按钮句柄，显示相应的步长
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentModifyAvgCurveBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnModifyAvgCurveModifyAmp)
						swprintf(gszTextStep, _T("%.1f"), fModifyAmpStep);
					else
						swprintf(gszTextStep, _T(""));

					SendMessage(hStatusBar, SB_SETTEXT, 
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
*加载修改AVG曲线窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int ModifyAvgCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_ModifyAvgCurveBtnBitmap[]=
	{
		{IDC_MODIFYAVGCURVE_ADDPOINT	,IDB_BITMAP_JIAYANGDIAN6_NOSEL_R	 ,  IDB_BITMAP_JIAYANGDIAN6_SELECT_R		},
		{IDC_MODIFYAVGCURVE_SELPOINT	,IDB_BITMAP_XUANZEYANGDIAN_NOSEL_R	 ,  IDB_BITMAP_XUANZEYANGDIAN_SELECT_R		},
		{IDC_MODIFYAVGCURVE_MODIFYAMP	,IDB_BITMAP_FUZHI_NOSEL_R	 ,  		IDB_BITMAP_FUZHI_SELECT_R		},
		{IDC_MODIFYAVGCURVE_DELETEPOINT	,IDB_BITMAP_SHANYANGDIAN6_NOSEL_R	 ,  IDB_BITMAP_SHANYANGDIANG6_SELECT_R		},
		{IDC_MODIFYAVGCURVE_END	,IDB_BITMAP_JIESHU6_NOSEL_R ,  IDB_BITMAP_JIESHU6_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_ModifyAvgCurveBtnBitmap);
	return ReturnVal;
}
/**************************
*加样点按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnModifyAvgCurveAddPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyAvgCurveBtnList[i] != currentModifyAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyAvgCurveBtn = ModifyAvgCurveBtnList[i];
					SetFocus(currentModifyAvgCurveBtn);
					gCurrentFocusBtn = currentModifyAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{					
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					while(ModifyAvgCurveBtnList[i] != currentModifyAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyAvgCurveBtn = ModifyAvgCurveBtnList[i];
					SetFocus(currentModifyAvgCurveBtn);
					gCurrentFocusBtn = currentModifyAvgCurveBtn;
					
					gbDeletePointFlag = TRUE;

					displaySelPoint(hBtnModifyAvgCurveSelPoint);
					displayPointAmp(hBtnModifyAvgCurveModifyAmp);
				}
				else if(wParam == VK_LEFT)
				{
				}
				else if(wParam == VK_RIGHT)
				{
				}
				else if(wParam == VK_RETURN)
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

					if(peakDefectInfos[CurrentChannelNo][0].wDefectTime!= 0 && peakDefectInfos[CurrentChannelNo][0].wDefectAmp != 0)
					{
						gWaveDrawAvgFlag = FALSE;
						Sleep(100);//等待擦掉波形
						
						if(gSelectedAvgPointDataNum[CurrentChannelNo] < MAX_CURVE_POINT_NUM)
							gSelectedAvgPointDataNum[CurrentChannelNo]++;

						if(gSelectedAvgPointDataNum[CurrentChannelNo] == 1)
						{
							gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)peakDefectInfos[CurrentChannelNo][0].wDefectTime)
																											/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
							gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial = curveChangeByDelay(gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial, 
																								fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
							
							gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].data = (float)peakDefectInfos[CurrentChannelNo][0].wDefectAmp;

							gOldMakeAvgPointData[0].data = gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].data;
							gOldMakeAvgPointData[0].serial = gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial;

							gfAvgFirstPointGain[CurrentChannelNo] = ReceiveParam_ad[CurrentChannelNo].fPlus;
						}
						else
						{
							gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)peakDefectInfos[CurrentChannelNo][0].wDefectTime)
																											/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
							gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial = curveChangeByDelay(gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial, 
																								fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
							
							gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].data = (float)(peakDefectInfos[CurrentChannelNo][0].wDefectAmp 
																															* pow(10, (double)((gfAvgFirstPointGain[CurrentChannelNo] - gfAvgFirstPointChangeGain[CurrentChannelNo])- ReceiveParam_ad[CurrentChannelNo].fPlus) / 20));

							gOldMakeAvgPointData[gSelectedAvgPointDataNum[CurrentChannelNo] - 1].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].data, gfAvgFirstPointChangeGain[CurrentChannelNo]);
							gOldMakeAvgPointData[gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial = gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial;
							sortSelectedPointDataBySerial(gOldMakeAvgPointData,gSelectedAvgPointDataNum[CurrentChannelNo]);
						}
						
						sortSelectedPointDataBySerial(gSelectedAvgPointData[CurrentChannelNo][3],gSelectedAvgPointDataNum[CurrentChannelNo]);

						gWaveDrawAvgFlag = TRUE;

						if(!gbGuanLian)
						{
							gfOldAvgRange[CurrentChannelNo] = DisplaySet_ad[CurrentChannelNo].fRange;
							
							for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
							{	
								gwOldAvgSerial[CurrentChannelNo][i] = (short)gSelectedAvgPointData[CurrentChannelNo][3][i].serial;
							}
						}
						else
						{
							gfOldAvgRange[0] = DisplaySet_ad[CurrentChannelNo].fRange;
							gfOldAvgRange[1] = DisplaySet_ad[CurrentChannelNo].fRange;

							for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
							{	
								gwOldAvgSerial[0][i] = (short)gSelectedAvgPointData[CurrentChannelNo][3][i].serial;
								gwOldAvgSerial[1][i] = (short)gSelectedAvgPointData[CurrentChannelNo][3][i].serial;
							}
						}
					}
				}
			//	else if(wParam == '\t')
			//	{
			//		SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			//	}
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
			/*	else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnModifyAvgCurveAddPointProc,Wnd,Msg,wParam,lParam);
}

/**************************
*选择样点按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnModifyAvgCurveSelPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyAvgCurveBtnList[i] != currentModifyAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyAvgCurveBtn = ModifyAvgCurveBtnList[i];
					SetFocus(currentModifyAvgCurveBtn);
					gCurrentFocusBtn = currentModifyAvgCurveBtn;
					
					gbDeletePointFlag = FALSE;
				}
				else if(wParam == VK_DOWN)
				{
					while(ModifyAvgCurveBtnList[i] != currentModifyAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyAvgCurveBtn = ModifyAvgCurveBtnList[i];
					SetFocus(currentModifyAvgCurveBtn);
					gCurrentFocusBtn = currentModifyAvgCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					if(gSelectedAvgPointDataNum[CurrentChannelNo])
					{
						
						if(gwDeleteStep == 0)
							gwDeleteStep = gSelectedAvgPointDataNum[CurrentChannelNo] - 1;
						else
							gwDeleteStep--;
					}
					
					displaySelPoint(Wnd);
					displayPointAmp(hBtnModifyAvgCurveModifyAmp);
					
				}
				else if(wParam == VK_RIGHT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					if(gSelectedAvgPointDataNum[CurrentChannelNo])
					{
						
						if(gwDeleteStep == gSelectedAvgPointDataNum[CurrentChannelNo] - 1)
							gwDeleteStep = 0;
						else
							gwDeleteStep++;
					}
					displaySelPoint(Wnd);
					displayPointAmp(hBtnModifyAvgCurveModifyAmp);
				}
				else if(wParam == VK_RETURN)
				{
					;//
				}
			//	else if(wParam == '\t')
			//	{
			//		SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			//	}
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
			/*	else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnModifyAvgCurveSelPointProc,Wnd,Msg,wParam,lParam);
}

/**************************
*修改幅值按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnModifyAvgCurveModifyAmpProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyAvgCurveBtnList[i] != currentModifyAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyAvgCurveBtn = ModifyAvgCurveBtnList[i];
					SetFocus(currentModifyAvgCurveBtn);
					gCurrentFocusBtn = currentModifyAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ModifyAvgCurveBtnList[i] != currentModifyAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyAvgCurveBtn = ModifyAvgCurveBtnList[i];
					SetFocus(currentModifyAvgCurveBtn);
					gCurrentFocusBtn = currentModifyAvgCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					gWaveDrawAvgFlag = FALSE;
					Sleep(100);//等待擦掉波形
					float fSelectPointAmp = gSelectedAvgPointData[CurrentChannelNo][3][gwDeleteStep].data * 100 / MAX_HEIGHT;
					if(fSelectPointAmp - fModifyAmpStep >= 0)
					{
						fSelectPointAmp = fSelectPointAmp - fModifyAmpStep;
						gSelectedAvgPointData[CurrentChannelNo][3][gwDeleteStep].data = fSelectPointAmp / 100 * MAX_HEIGHT;
						displayPointAmp(Wnd);
					}
					
					gWaveDrawAvgFlag = TRUE;								
				}
				else if(wParam == VK_RIGHT)
				{
					gWaveDrawAvgFlag = FALSE;
					Sleep(100);//等待擦掉波形
					float fSelectPointAmp = gSelectedAvgPointData[CurrentChannelNo][3][gwDeleteStep].data * 100 / MAX_HEIGHT;
					if(fSelectPointAmp + fModifyAmpStep <= 100)
					{
						fSelectPointAmp = fSelectPointAmp + fModifyAmpStep;
						gSelectedAvgPointData[CurrentChannelNo][3][gwDeleteStep].data = fSelectPointAmp / 100 * MAX_HEIGHT;
						displayPointAmp(Wnd);
					}
					
					gWaveDrawAvgFlag = TRUE;
				}
				else if(wParam == VK_RETURN)
				{
					;//
				}
			//	else if(wParam == '\t')
			//	{
			//		SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			//	}
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
				else if(wParam == 'F')
				{
					if(wModifyAmpStepIndex < 2)
						wModifyAmpStepIndex++;
					else
						wModifyAmpStepIndex = 0;

					fModifyAmpStep = RATESTEPS[wModifyAmpStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fModifyAmpStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			/*	else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnModifyAvgCurveSelPointProc,Wnd,Msg,wParam,lParam);
}

/**************************
*删样点按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnModifyAvgCurveDeletePointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyAvgCurveBtnList[i] != currentModifyAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyAvgCurveBtn = ModifyAvgCurveBtnList[i];
					SetFocus(currentModifyAvgCurveBtn);
					gCurrentFocusBtn = currentModifyAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ModifyAvgCurveBtnList[i] != currentModifyAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyAvgCurveBtn = ModifyAvgCurveBtnList[i];
					SetFocus(currentModifyAvgCurveBtn);
					gCurrentFocusBtn = currentModifyAvgCurveBtn;

					gbDeletePointFlag = FALSE;
				  }
				else if(wParam == VK_LEFT)
				{
					;//
				}
				else if(wParam == VK_RIGHT)
				{
					;//
				}
				else if(wParam == VK_RETURN)
				{
					if(gSelectedAvgPointDataNum[CurrentChannelNo])
					{
						gWaveDrawAvgFlag = FALSE;
						Sleep(100);//等待擦掉波形
					 	if(gSelectedAvgPointDataNum[CurrentChannelNo] == 1)
					 	{
					 		gSelectedAvgPointDataNum[CurrentChannelNo]--;
					 	}
						else
						{
							if(gwDeleteStep == (gSelectedAvgPointDataNum[CurrentChannelNo] - 1))//删最后一个点
							{
								gSelectedAvgPointDataNum[CurrentChannelNo]--;
								gwDeleteStep--;
							}
							else
							{
								for(i = gwDeleteStep; i < gSelectedAvgPointDataNum[CurrentChannelNo] - 1; i++)
								{
									gSelectedAvgPointData[CurrentChannelNo][3][i] = gSelectedAvgPointData[CurrentChannelNo][3][i + 1];//选中点后面的点数据往前移
								}
								gSelectedAvgPointDataNum[CurrentChannelNo]--;
								for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
								{
									gOldMakeAvgPointData[i] = gSelectedAvgPointData[CurrentChannelNo][3][i];
								}
							}
						}
						gWaveDrawAvgFlag = TRUE;
					}
				}
			//	else if(wParam == '\t')
			//	{
			//		SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			//	}
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
				/*else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnModifyAvgCurveDeletePointProc,Wnd,Msg,wParam,lParam);
}

/**************************
*结束按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnModifyAvgCurveEndProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					while(ModifyAvgCurveBtnList[i] != currentModifyAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyAvgCurveBtn = ModifyAvgCurveBtnList[i];
					SetFocus(currentModifyAvgCurveBtn);
					gCurrentFocusBtn = currentModifyAvgCurveBtn;

					gbDeletePointFlag = TRUE;
					
					displaySelPoint(hBtnModifyAvgCurveSelPoint);
					displayPointAmp(hBtnModifyAvgCurveModifyAmp);
				}
				
				else if(wParam == VK_DOWN)
				{
					while(ModifyAvgCurveBtnList[i] != currentModifyAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyAvgCurveBtn = ModifyAvgCurveBtnList[i];
					SetFocus(currentModifyAvgCurveBtn);
					gCurrentFocusBtn = currentModifyAvgCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{					
					ShowWindow(hWndMakeAvgCurve,SW_SHOW);
					SetFocus(currentMakeAvgCurveBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndModifyAvgCurve);
					hWndModifyAvgCurve = 0;
					
					gCurrentFocusBtn = currentMakeAvgCurveBtn;

					gCurrentWnd = hWndMakeAvgCurve;

					quitCurveProcess();
				}
			//	else if(wParam == '\t')
			//	{
			//		SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			//	}
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
				/*else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnModifyAvgCurveEndProc,Wnd,Msg,wParam,lParam);
}

