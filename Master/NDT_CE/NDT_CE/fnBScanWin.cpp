/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * B扫描窗口源文件
 * Filename: fnBScanWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnBScanWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
//#include "bsp.h"
#include "gateWin.h"
#include "const.h"

HWND hWndBScan = NULL;			//B扫描窗口句柄
HWND hBtnBScanGangBanHouDu = NULL;	//钢板厚度按钮句柄
HWND hBtnBScanOnOrOff = NULL;		//开启/关闭按钮句柄
HWND hBtnBScanScanMode = NULL;		//扫描方式按钮句柄
HWND hBtnBScanGateQianYan = NULL;	//闸门前沿按钮句柄
HWND hBtnBScanGateHouYan = NULL;	//闸门后沿按钮句柄
HWND hBtnBScanGateFuZhi = NULL;	//闸门幅值按钮句柄

HWND BScanBtnList[3] = {NULL,NULL,NULL};	//B扫描窗口的按钮表
WNDPROC OldBtnBScanGangBanHouDuProc = NULL;//钢板厚度按钮默认处理过程
WNDPROC OldBtnBScanOnOrOffProc = NULL;//开启/关闭按钮默认处理过程
WNDPROC OldBtnBScanScanModeProc = NULL;		//扫描方式按钮默认处理过程
WNDPROC OldBtnBScanGateQianYanProc = NULL;	//闸门前沿按钮默认处理过程
WNDPROC OldBtnBScanGateHouYanProc = NULL;	//闸门后沿按钮默认处理过程
WNDPROC OldBtnBScanGateFuZhiProc = NULL;	//闸门幅值按钮默认处理过程

HWND currentBScanBtn = NULL;					//B扫描窗中当前选中的按钮
HANDLE hDrawBScanThread = NULL;				// 画B扫图线程句柄
DRAWTHREAD_PARAM DrawBScanThread_param;		// 画B扫图线程参数
HANDLE hDrawBScanEvent = NULL;				// 画B扫图事件
DWORD dwDrawBScanThreadID = 0;				// 画B扫图线程ID
BOOL gbBscanSwitch  = OFF;	    //B扫开关、
CRITICAL_SECTION csBScan;

static const int buttonNum = 3;		//按钮控件数
WORD wScanMode;			//扫描方式
static BOOL drawBScanFlag = FALSE;      //是否有画B扫图标志

static float fBScanGangBanHouDuStep = 1;		//工件厚度步长
static float fBScanGateStep = 1.0f;
static WORD wBScanPeakStep = 1;
//GATE_INFO BScanGateInfo[CHANNEL_NUM];
static WORD wBScanGangBanHouDuStepIndex = 1;	//工件厚度步长索引

float fBScanGangBanHouDu = 1;

extern HINSTANCE hInst;		//程序实例句柄

extern HWND hWndMain;
extern HWND hWndSpecialFunction;
extern HWND currentSpecialFunctionBtn;
extern HWND gCurrentFocusBtn;
extern BOOL gbGuanLian;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern float fRangeStart[2];
extern float fRangeEnd[2];
extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
extern ARS200FPGA_CH_GATE_REG *ch2_gate_reg;
extern WORD *ch1_gate1curve;
extern WORD *ch2_gate1curve;
extern WORD CurrentChannelNo;
extern WORD *chCurrent_gate1curve;
extern PIC_DATA gPicData[2];			// 图像数据
extern  CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// 工作线程参数
extern ARS200FPGA_CH_CON_REG *chCurrent_con_reg;
extern ARS200FPGA_CH_CON_REG *ch1_con_reg;
extern ARS200FPGA_CH_CON_REG *ch2_con_reg;
extern ARS200FPGA_CH_DATA_REG * chCurrent_data_reg;

extern BOOL g_bMenuShown;//是否显菜单标志
extern HWND hWndMain;
extern GATE_INFO GateInfos[2][3];
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//当前显示的子窗口
extern wchar_t gszTextStep[10];
extern BOOL gbHotRange;			// 快捷键声程
extern BOOL gbHotDB;			// 快捷键增益
extern HWND hStatusBar;			// 状态栏句柄
extern PEAK_INFO gPeakDefectInfos[2][2];
extern CRITICAL_SECTION csPeakDefectInfos;
extern float gfPlateHeight;		//工件厚度
extern ARS200FPGA_SYS_REG* sys_reg;

/**************************
*注册B扫描窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerBScanChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) BScanChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szBScanChild;

	return RegisterClass(&wc);
}
/**************************
*B扫线程处理过程
* @param    lpParameter[in]:
* @return   LRESULT :
* @since    1.00
***************************/
#if 0
DWORD WINAPI DrawBScanThreadProc(LPVOID lpParameter)
{
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;
	float fRange;
	DWORD wLastPosition; //要画的线最底部位置
	DRAWTHREAD_PARAM* pParam = (DRAWTHREAD_PARAM*)lpParameter;
	HDC hdc = pParam->hDC;

	RECT rect;	// 主窗口显示出来的矩形
	GetClipBox(hdc, &rect); 
	RECT RctWave;	// 波形区域

	wchar_t strText1[20], strText2[20];
	memset(strText1, 0, sizeof(strText1));
	memset(strText2, 0, sizeof(strText2));

	GetClipBox(hdc, &rect); // 主窗口显示出来的矩形

	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	
	while(1)
	{
		DWORD ret = WaitForSingleObject(hDrawBScanEvent, 0);

		if (ret == WAIT_OBJECT_0) //事件有信号
		{
			for(int i = RctWave.left; i < RctWave.right; i++)
			{
				if((chCurrent_gate_reg->wAlarmFlag & 0x1) && (chCurrent_gate_reg->wAlarmReg & 0x1) && (chCurrent_con_reg->wEnable & 0x1))
				{
					InitializeCriticalSection(&csPeakDefectInfos);
					EnterCriticalSection(&csPeakDefectInfos);
					fRange = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)gPeakDefectInfos[CurrentChannelNo][0].wDefectTime); //报警点的声程
					LeaveCriticalSection(&csPeakDefectInfos);
					DeleteCriticalSection(&csPeakDefectInfos);
					
					if((WORD)(fRange / gfPlateHeight) >= 1)
						wLastPosition = RctWave.bottom;
					else
					{
						wLastPosition = RctWave.top + (WORD)(WAVE_AREA_HEIGHT * (fRange / gfPlateHeight));
					}
					//报警读取一次重置
					chCurrent_con_reg->wReset |= 0x1;
					chCurrent_con_reg->wReset &= 0xFFFE;
					chCurrent_data_reg->wMonitorReset = 1;
					Sleep(1);
					chCurrent_data_reg->wMonitorReset = 0;
				}
				else //无报警画到底部
				{
					wLastPosition = RctWave.bottom;

					chCurrent_con_reg->wReset |= 0x1;
					chCurrent_con_reg->wReset &= 0xFFFE;
				//	ch2_con_reg->wReset |= 0x1;
				//	ch2_con_reg->wReset &= 0xFFFE;
					chCurrent_data_reg->wMonitorReset = 1;
					Sleep(1);
					chCurrent_data_reg->wMonitorReset = 0;
				//	wsprintf(strText1, _T("ssss"));
				//	DrawLableText(hBtnBScanGateHouYan, strText1, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
			//	wsprintf(strText1, _T("%.1f"), fRange);
			//	DrawLableText(hBtnBScanGateHouYan, strText1, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			//	if(wLastPosition == RctWave.top)
			//	{
			//		wsprintf(strText1, _T("%.3f"), fRange);
			//		DrawLableText(hBtnBScanGateHouYan, strText1, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			//		wsprintf(strText2, _T("%d"), wLastPosition);
			//		DrawLableText(hBtnBScanGateFuZhi, strText2, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			//	}

				for(long j = RctWave.top; j < RctWave.bottom; j++)
				{
					//当前要画的竖线先描黑
					pPixel = pBaseAddress + j*LCD_XSIZE_TFT + i;
					*pPixel = BLACK_POINT;
					
					//当前要画的竖线描绿
					if(j <= (long)wLastPosition)
					{
						pPixel = pBaseAddress + j*LCD_XSIZE_TFT + i;
						*pPixel = GREEN_POINT;
					}
				}
				if(gbBscanSwitch == OFF)
				{
					hDrawBScanThread = NULL;
					return 0;
				} //B扫关闭结束线程
				Sleep(50);
			}
			if(!wScanMode) //非循环模式事件置为无信号,单次模式画一次
				ResetEvent(hDrawBScanEvent);			
		}
		if(gbBscanSwitch == OFF)
		{
			hDrawBScanThread = NULL;
			return 0;
		}//B扫关闭结束线程
		Sleep(10);
	}
	return 0;
	
}
#endif
/**************************
*B扫线程处理过程
* @param    lpParameter[in]:
* @return   LRESULT :
* @since    1.00
*date 2011-3-25
*author	wangbingfu
***************************/

DWORD WINAPI DrawBScanThreadProc(LPVOID lpParameter)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;
	float fRange;
	DWORD wLastPosition; //要画的线最底部位置
	DRAWTHREAD_PARAM* pParam = (DRAWTHREAD_PARAM*)lpParameter;
	HDC hdc = pParam->hDC;

	RECT rect;	// 主窗口显示出来的矩形
	GetClipBox(hdc, &rect); 
	RECT RctWave;	// 波形区域

	wchar_t strText1[20], strText2[20];
	memset(strText1, 0, sizeof(strText1));
	memset(strText2, 0, sizeof(strText2));

	GetClipBox(hdc, &rect); // 主窗口显示出来的矩形

	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;

	sys_reg->wLedAlarm &= 0xFFFE;//wangbingfu add at 2011-3-28 开B扫前关闭报警
	
	while(1)
	{
		DWORD ret = WaitForSingleObject(hDrawBScanEvent, 0);

		if (ret == WAIT_OBJECT_0) //事件有信号
		{
			for(int i = RctWave.left; i < RctWave.right; i++)
			{
				WORD wDefectTime = chCurrent_gate_reg->wGate1DefectTime;
				//读取一次复位
				chCurrent_con_reg->wReset |= 0x1;
				chCurrent_con_reg->wReset &= 0xFFFE;
				chCurrent_data_reg->wMonitorReset = 1;
				Sleep(1);
				chCurrent_data_reg->wMonitorReset = 0;
				
			//	InitializeCriticalSection(&csPeakDefectInfos);
			//	EnterCriticalSection(&csPeakDefectInfos);
				fRange = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, wDefectTime); //工作线程已经停，直接读硬件，不取变量gPeakDefectInfos
			//	LeaveCriticalSection(&csPeakDefectInfos);
			//	DeleteCriticalSection(&csPeakDefectInfos);
				
				if((WORD)(fRange / gfPlateHeight) >= 1)
					wLastPosition = RctWave.bottom;
				else
				{
					wLastPosition = RctWave.top + (WORD)(WAVE_AREA_HEIGHT * (fRange / gfPlateHeight));
				}
			
				for(long j = RctWave.top; j < RctWave.bottom; j++)
				{
					//当前要画的竖线先描黑
					pPixel = pBaseAddress + j*LCD_XSIZE_TFT + i;
					*pPixel = BLACK_POINT;
					
					//当前要画的竖线描绿
					if(j <= (long)wLastPosition)
					{
						pPixel = pBaseAddress + j*LCD_XSIZE_TFT + i;
						*pPixel = GREEN_POINT;
					}
				}
				if(gbBscanSwitch == OFF)
				{
					hDrawBScanThread = NULL;
					return 0;
				} //B扫关闭结束线程
				Sleep(10);
			}
			if(!wScanMode) //非循环模式事件置为无信号,单次模式画一次
				ResetEvent(hDrawBScanEvent);			
		}
		if(gbBscanSwitch == OFF)
		{
			hDrawBScanThread = NULL;
			return 0;
		}//B扫关闭结束线程
		Sleep(10);
	}
	*/
	return 0;
	
}

/**************************
*显示钢板厚度
* @param		hWnd[in]:窗口句柄
* @param		fHouDu[in]:钢板厚度
* @return   LRESULT :
* @since    1.00
***************************/
void displayBScanGangBanHouDu(HWND hWnd ,float fHouDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fHouDu);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}

/**************************
*设置B扫闸门前沿
* @BScan		hWnd[in]:窗口句柄
* @BScan		fFront[in]:闸门前沿
* @return   LRESULT :
* @since    1.00
***************************/
void setBScanGateQianYan(HWND hWnd ,float fFront)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.2f"), fFront);
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	if(!gbGuanLian)
	{
		chCurrent_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
		GateInfos[CurrentChannelNo][0].fFrontGate = fFront;
	}
	else
	{
		ch1_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
		ch2_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));

		GateInfos[0][0].fFrontGate = fFront;
		GateInfos[1][0].fFrontGate = fFront;
	}
}
/**************************
*设置B扫闸门后沿
* @param		hWnd[in]:窗口句柄
* @param		fBack[in]:闸门后沿
* @return   LRESULT :
* @since    1.00
***************************/
void setBScanGateHouYan(HWND hWnd ,float fBack)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.2f"), fBack);
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	if(!gbGuanLian)
	{
		chCurrent_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
		GateInfos[CurrentChannelNo][0].fBackGate = fBack;
	}
	else
	{
		ch1_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
		ch2_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));

		GateInfos[0][0].fBackGate = fBack;
		GateInfos[1][0].fBackGate = fBack;
	}

}
/*
void setBScantGateCurve(WORD* wCurve, DWORD dwDots,float fFuzhi)
{
	WORD *wp = wCurve;
	for(DWORD i=0; i<= dwDots; i++)
	{
		*wp = WORD(MAX_HEIGHT * fFuzhi / 100.0f);
		wp++;
	}	
}
*/
/**************************
*设置B扫闸门幅值
* @param		hWnd[in]:窗口句柄
* @param		fFuzhi[in]:闸门幅值
* @return   LRESULT :
* @since    1.00
***************************/
void setBScanGateFuZhi(HWND hWnd, float fFuzhi)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.2f"), fFuzhi);

	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	float fFront = GateInfos[CurrentChannelNo][0].fFrontGate;
	float fBack = GateInfos[CurrentChannelNo][0].fBackGate;

	float fRange = fBack - fFront;

	WORD wDots = (WORD)Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fRange);

	if(!gbGuanLian)
	{
		//SetGateCurve(chCurrent_gate1curve, wDots, fFuzhi);
		chCurrent_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * fFuzhi / 100.0f);
		GateInfos[CurrentChannelNo][0].fPeakGate = fFuzhi;
	}
	else
	{
		//SetGateCurve(ch1_gate1curve, wDots, fFuzhi);
		ch1_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * fFuzhi / 100.0f);
		//SetGateCurve(ch2_gate1curve, wDots, fFuzhi);
		ch2_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * fFuzhi / 100.0f);

		GateInfos[CurrentChannelNo][0].fPeakGate = fFuzhi;
		GateInfos[CurrentChannelNo][0].fPeakGate = fFuzhi;
	}

}
/**************************
*设置B扫开关
* @param		hWnd[in]:窗口句柄
* @param		bSwitch[in]:开关
* @return   LRESULT :
* @since    1.00
***************************/
void setBScanSwitch(HWND hWnd, BOOL bSwitch)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(bSwitch)
	{
		wsprintf(strText, _T("开"));	
	}
	else 
	{
		wsprintf(strText, _T("关"));
	}
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	SetOpenFlag(NULL, bSwitch, 1);
}
/*
void unDrawWave()
{
	HDC hdc = GetDC(hWndMain);
	RECT rect;
	GetClipBox(hdc, &rect);
	POINT preData[DATA_SIZE];

	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	// 波形区域宽度、高度
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;
	float yb = nWaveHeight/MAX_HEIGHT;
	float xb = nWaveWidth/512.0f;
	long x, y;
	for(int j = 0; j < 2; j++)
	{
		if(gPicData[j].bDrawed)
		{
			for (int i = 0;i < DATA_SIZE; i++)
			{
 				if (gPicData[j].data[i] > MAX_HEIGHT) 
 					gPicData[j].data[i] = MAX_HEIGHT;
									
				x = long(RctWave.left+i*xb);
				y = long((nWaveHeight-1)-gPicData[j].data[i]*yb) + RctWave.top;
				if(i>0)
					Line(preData[i-1].x, preData[i-1].y, x, y, 0);
				preData[i].x = x;
				preData[i].y = y;
			}
		}
	}
}
*/
/*
void drawBScanPicture(BOOL bDrawBScanFlag)
{
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;
	float fRange;
	WORD wLastPosition;

	if(bDrawBScanFlag == TRUE)
	{
		for(int i = SCALE_WIDTH + 1; i < 640 - SCALE_WIDTH - MENU_WIDTH; i++)
		{
			if((chCurrent_gate_reg->wAlarmFlag & 0x1) && (chCurrent_gate_reg->wAlarmReg & 0x1) && (chCurrent_con_reg->wEnable & 0x1))
			{
				fRange = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, gPeakDefectInfos[CurrentChannelNo][0].wDefectTime);
				if((WORD)(fRange / gfPlateHeight) >= 1)
					wLastPosition = MIN_STATUS_HEIGHT + WAVE_AREA_HEIGHT - 1;
				else
				{
					wLastPosition = MIN_STATUS_HEIGHT + (WORD)(WAVE_AREA_HEIGHT * (fRange / gfPlateHeight)) - 1;
				}
			}
			else
			{
				wLastPosition = MIN_STATUS_HEIGHT + WAVE_AREA_HEIGHT - 1;
			}
			for(int j = MIN_STATUS_HEIGHT + 1; j < 480- SCALE_WIDTH; j++)
			{
				//当前要画的竖线先描黑
				pPixel = pBaseAddress + j*LCD_XSIZE_TFT + i;
				*pPixel = BLACK_POINT;
				
				//当前要画的竖线描白
				if(j <= wLastPosition)
				{
					pPixel = pBaseAddress + j*LCD_XSIZE_TFT + i;
					*pPixel = GREEN_POINT;
				}
			//	Sleep(20);
			}
			if((wScanMode == 1) && (i == 640 - SCALE_WIDTH - MENU_WIDTH) && (chCurrent_gate_reg->wGate1Enable == ON))
			{
				i = SCALE_WIDTH + 1;//从头再画B扫图
			}
		}
	}
	else if(bDrawBScanFlag == FALSE)
	{
		for(int i = SCALE_WIDTH + 1; i < 640 - SCALE_WIDTH - MENU_WIDTH; i++)
		{
			for(int j = MIN_STATUS_HEIGHT + 1; j < 480- SCALE_WIDTH; j++)
			{
				pPixel = pBaseAddress + j*LCD_XSIZE_TFT + i;
				*pPixel = BLACK_POINT;
			}
		}
	}
}
*/
/**************************
*B扫窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK BScanChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		//	closeAllGate();//关闭所有闸门
		//	GateInfos[CurrentChannelNo][0].bOpen = ON; //画B扫闸门线

		//	SetOpenFlag(NULL, TRUE, 1);//wangbingfu delete at 2011-3-25
			fBScanGangBanHouDuStep = AMENDSTEPS[wBScanGangBanHouDuStepIndex];
			//画按钮控件
			hBtnBScanGangBanHouDu = CreateWindow(_T("BUTTON"),_T("gangBanHouDuBScan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 ,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_BSCAN_GANGBANHOUDU,hInst,NULL);
		//	hBtnBScanGateQianYan = CreateWindow(_T("BUTTON"),_T("gateQianYanBScan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
		//			0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_BSCAN_GATEQIANYAN,hInst,NULL);
		//	hBtnBScanGateHouYan = CreateWindow(_T("BUTTON"),_T("gateHouYanBScan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
		//			0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_BSCAN_GATEHOUYAN,hInst,NULL);
		//	hBtnBScanGateFuZhi = CreateWindow(_T("BUTTON"),_T("gateFuZhiBScan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
		//			0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_BSCAN_GATEFUZHI,hInst,NULL);
			hBtnBScanScanMode = CreateWindow(_T("BUTTON"),_T("scanModeBScan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_BSCAN_SCANMODE,hInst,NULL);
			hBtnBScanOnOrOff = CreateWindow(_T("BUTTON"),_T("onOrOffBScan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_BSCAN_ONOROFF,hInst,NULL);
					
			BScanBtnList[0] = hBtnBScanGangBanHouDu;
			//BScanBtnList[1] = hBtnBScanGateQianYan;
			//BScanBtnList[2] = hBtnBScanGateHouYan;
			//BScanBtnList[3] = hBtnBScanGateFuZhi;
			BScanBtnList[1] = hBtnBScanScanMode;
			BScanBtnList[2] = hBtnBScanOnOrOff;
			
			//按钮控件的自定义处理过程
			OldBtnBScanGangBanHouDuProc = (WNDPROC) GetWindowLong(hBtnBScanGangBanHouDu,GWL_WNDPROC);
			SetWindowLong(hBtnBScanGangBanHouDu,GWL_WNDPROC,(LONG)btnBScanGangBanHouDuProc);

			OldBtnBScanGateQianYanProc = (WNDPROC) GetWindowLong(hBtnBScanGateQianYan,GWL_WNDPROC);
			SetWindowLong(hBtnBScanGateQianYan,GWL_WNDPROC,(LONG)btnBScanGateQianYanProc);
			
			OldBtnBScanGateHouYanProc = (WNDPROC) GetWindowLong(hBtnBScanGateHouYan,GWL_WNDPROC);
			SetWindowLong(hBtnBScanGateHouYan,GWL_WNDPROC,(LONG)btnBScanGateHouYanProc);

			OldBtnBScanGateFuZhiProc = (WNDPROC) GetWindowLong(hBtnBScanGateFuZhi, GWL_WNDPROC);
			SetWindowLong(hBtnBScanGateFuZhi,GWL_WNDPROC,(LONG)btnBScanGateFuZhiProc);
			
			OldBtnBScanScanModeProc = (WNDPROC) GetWindowLong(hBtnBScanScanMode,GWL_WNDPROC);
			SetWindowLong(hBtnBScanScanMode,GWL_WNDPROC,(LONG)btnBScanScanModeProc);

			OldBtnBScanOnOrOffProc = (WNDPROC) GetWindowLong(hBtnBScanOnOrOff,GWL_WNDPROC);
			SetWindowLong(hBtnBScanOnOrOff,GWL_WNDPROC,(LONG)btnBScanOnOrOffProc);
			
			
			
			
			currentBScanBtn = BScanBtnList[0];//默认当前选中开启/关闭按钮
			SetFocus(currentBScanBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentBScanBtn;

			
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
		//	closeAllGate();//关闭所有闸门 快捷切到闸门 再切回到B扫时
		//	GateInfos[CurrentChannelNo][0].bOpen = ON; //画B扫闸门线

		//	SetOpenFlag(NULL, TRUE, 1);//wangbingfu delete at 2011-3-25
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			wchar_t strText[20];
			memset(strText, 0, sizeof(strText));

			pdis=(LPDRAWITEMSTRUCT)lParam;
			BScan_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnBScanGangBanHouDu)
			{
				displayBScanGangBanHouDu(pdis->hwndItem, gfPlateHeight);
			}
		/*	if(pdis->hwndItem == hBtnBScanGateQianYan)
			{
				float fFront = GateInfos[CurrentChannelNo][0].fFrontGate;
				swprintf(strText, _T("%.2f"), fFront);
				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			}
			if(pdis->hwndItem == hBtnBScanGateHouYan)
			{
				float fBack = GateInfos[CurrentChannelNo][0].fBackGate;
				swprintf(strText, _T("%.2f"), fBack);
				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			}
			if(pdis->hwndItem == hBtnBScanGateFuZhi)
			{
				float fPeakGate = GateInfos[CurrentChannelNo][0].fPeakGate;
				swprintf(strText, _T("%.2f"), fPeakGate);
				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			}*/
			if(pdis->hwndItem == hBtnBScanScanMode)
			{
				if(wScanMode == 0)
				{
					wsprintf(strText, _T("单次"));
				}
				else if(wScanMode == 1)
				{
					wsprintf(strText, _T("循环"));	
				}
				
				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			}
			if(pdis->hwndItem == hBtnBScanOnOrOff)
			{
				if(gbBscanSwitch)
				{
					wsprintf(strText, _T("开"));	
				}
				else 
				{
					wsprintf(strText, _T("关"));
				}
				DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			}

		/*	// 检测按钮句柄，显示相应的步长
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentBScanBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnBScanGangBanHouDu)
						swprintf(gszTextStep, _T("%.1f"), fBScanGangBanHouDuStep);
					else
						swprintf(gszTextStep, _T(""));

					PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}*/
			break;
		case WM_CLOSE:
		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载B扫描窗口按钮图片
* @param    hInstance[in]: 实例句柄
* @param		pdis[in]                                              
* @return   int
* @since    1.00
***************************/
int BScan_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_BScanBtnBitmap[]=
	{
		{IDC_BSCAN_GANGBANHOUDU	,IDB_BITMAP_GANGBANHOUDU6_NOSEL_R ,  IDB_BITMAP_GANGBANHOUDU6_SELECT_R	},
		{IDC_BSCAN_GATEQIANYAN	,IDB_BITMAP_BSCAN_ZHAMENQIANYAN_NOSEL_R ,  IDB_BITMAP_BSCAN_ZHAMENQIANYAN_SELECT_R	},
		{IDC_BSCAN_GATEHOUYAN	,IDB_BITMAP_BSCAN_ZHAMENHOUYAN_NOSEL_R ,  IDB_BITMAP_BSCAN_ZHAMENHOUYAN_SELECT_R	},
		{IDC_BSCAN_GATEFUZHI	,IDB_BITMAP_BSCAN_ZHAMENFUZHI_NOSEL_R ,  IDB_BITMAP_BSCAN_ZHAMENFUZHI_SELECT_R	},
		{IDC_BSCAN_SCANMODE	,IDB_BITMAP_SAOMIAOFANGSHI_NOSEL_R	 ,  IDB_BITMAP_SAOMIAOFANGSHI_SELECT_R		},
		{IDC_BSCAN_ONOROFF	,IDB_BITMAP_KAIQIGUANBI6_NOSEL_R	 ,  IDB_BITMAP_KAIQIGUANBI6_SELECT_R		},				
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_BScanBtnBitmap);
	return ReturnVal;
}
/**************************
*钢板厚度按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnBScanGangBanHouDuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), fBScanGangBanHouDuStep);
			PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			break;
		case WM_KILLFOCUS:
			swprintf(gszTextStep, _T(""));
			PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndBScan);
					hWndBScan = 0;

					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(drawBScanFlag = TRUE)
					{
						//结束B扫描
						gbBscanSwitch = OFF;
					//	setBScanSwitch(Wnd, gbBscanSwitch);
						SetRedrawFlag(TRUE);		//擦掉B扫图	

						InitializeCriticalSection(&csCommand);
						EnterCriticalSection(&csCommand);
						//gbEndFlag =TRUE;
						thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
						LeaveCriticalSection(&csCommand);
						DeleteCriticalSection(&csCommand);
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(BScanBtnList[i] != currentBScanBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentBScanBtn = BScanBtnList[i];
					SetFocus(currentBScanBtn);
					gCurrentFocusBtn = currentBScanBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(BScanBtnList[i] != currentBScanBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentBScanBtn = BScanBtnList[i];
					SetFocus(currentBScanBtn);
					gCurrentFocusBtn = currentBScanBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if((gfPlateHeight - fBScanGangBanHouDuStep) >= MIN_GONGJIANHOUDU)
						gfPlateHeight -= fBScanGangBanHouDuStep;
					else
						gfPlateHeight = MIN_GONGJIANHOUDU;

					displayBScanGangBanHouDu(Wnd, gfPlateHeight);					
				}
				else if(wParam == VK_RIGHT)
				{
					if((gfPlateHeight + fBScanGangBanHouDuStep) <= MAX_fPlateHeight)
						gfPlateHeight += fBScanGangBanHouDuStep;
					else
						gfPlateHeight = MAX_fPlateHeight;

					displayBScanGangBanHouDu(Wnd, gfPlateHeight);
				}
				else if(wParam == 'F')
				{
					if(wBScanGangBanHouDuStepIndex < 3)
						wBScanGangBanHouDuStepIndex++;
					else
						wBScanGangBanHouDuStepIndex = 0;

					fBScanGangBanHouDuStep = AMENDSTEPS[wBScanGangBanHouDuStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fBScanGangBanHouDuStep);
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
	return CallWindowProc(OldBtnBScanGangBanHouDuProc,Wnd,Msg,wParam,lParam);
}

/**************************
*闸门前沿按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnBScanGateQianYanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	float fStart = fRangeStart[CurrentChannelNo];
	float fEnd = fRangeEnd[CurrentChannelNo];
	float fFront = GateInfos[CurrentChannelNo][0].fFrontGate;
	float fBack = GateInfos[CurrentChannelNo][0].fBackGate;
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndBScan);
					hWndBScan = 0;

					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(drawBScanFlag = TRUE)
					{
						//结束B扫描
						gbBscanSwitch = OFF;
					//	setBScanSwitch(Wnd, gbBscanSwitch);
						SetRedrawFlag(TRUE);		//擦掉B扫图	

						InitializeCriticalSection(&csCommand);
						EnterCriticalSection(&csCommand);
						//gbEndFlag =TRUE;
						thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
						LeaveCriticalSection(&csCommand);
						DeleteCriticalSection(&csCommand);
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(BScanBtnList[i] != currentBScanBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentBScanBtn = BScanBtnList[i];
					SetFocus(currentBScanBtn);
					gCurrentFocusBtn = currentBScanBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(BScanBtnList[i] != currentBScanBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentBScanBtn = BScanBtnList[i];
					SetFocus(currentBScanBtn);
					gCurrentFocusBtn = currentBScanBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(fFront > fStart)
						fFront -= fBScanGateStep;

					setBScanGateQianYan(Wnd, fFront);					
				}
				else if(wParam == VK_RIGHT)
				{
					if(fFront < fBack)
						fFront += fBScanGateStep;

					setBScanGateQianYan(Wnd, fFront);
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
	return CallWindowProc(OldBtnBScanGateQianYanProc,Wnd,Msg,wParam,lParam);
}
/**************************
*闸门后沿按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnBScanGateHouYanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	float fStart = fRangeStart[CurrentChannelNo];
	float fEnd = fRangeEnd[CurrentChannelNo];
	float fFront = GateInfos[CurrentChannelNo][0].fFrontGate;
	float fBack = GateInfos[CurrentChannelNo][0].fBackGate;
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndBScan);
					hWndBScan = 0;

					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(drawBScanFlag = TRUE)
					{
						//结束B扫描
						gbBscanSwitch = OFF;
					//	setBScanSwitch(Wnd, gbBscanSwitch);
						SetRedrawFlag(TRUE);		//擦掉B扫图	

						InitializeCriticalSection(&csCommand);
						EnterCriticalSection(&csCommand);
						//gbEndFlag =TRUE;
						thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
						LeaveCriticalSection(&csCommand);
						DeleteCriticalSection(&csCommand);
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(BScanBtnList[i] != currentBScanBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentBScanBtn = BScanBtnList[i];
					SetFocus(currentBScanBtn);
					gCurrentFocusBtn = currentBScanBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(BScanBtnList[i] != currentBScanBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentBScanBtn = BScanBtnList[i];
					SetFocus(currentBScanBtn);
					gCurrentFocusBtn = currentBScanBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(fBack > fFront)
						fBack -= fBScanGateStep;

					setBScanGateHouYan(Wnd, fBack);					
				}
				else if(wParam == VK_RIGHT)
				{
					if(fBack < gfPlateHeight)
						fBack += fBScanGateStep;

					setBScanGateHouYan(Wnd, fBack);
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
	return CallWindowProc(OldBtnBScanGateHouYanProc,Wnd,Msg,wParam,lParam);
}

LRESULT CALLBACK btnBScanGateFuZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	float fPeakGate = GateInfos[CurrentChannelNo][0].fPeakGate;
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndBScan);
					hWndBScan = 0;

					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(drawBScanFlag = TRUE)
					{
						//结束B扫描
						gbBscanSwitch = OFF;
					//	setBScanSwitch(Wnd, gbBscanSwitch);
						SetRedrawFlag(TRUE);		//擦掉B扫图	

						InitializeCriticalSection(&csCommand);
						EnterCriticalSection(&csCommand);
						//gbEndFlag =TRUE;
						thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
						LeaveCriticalSection(&csCommand);
						DeleteCriticalSection(&csCommand);
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(BScanBtnList[i] != currentBScanBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentBScanBtn = BScanBtnList[i];
					SetFocus(currentBScanBtn);
					gCurrentFocusBtn = currentBScanBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(BScanBtnList[i] != currentBScanBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentBScanBtn = BScanBtnList[i];
					SetFocus(currentBScanBtn);
					gCurrentFocusBtn = currentBScanBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(fPeakGate > 0)
						fPeakGate -= wBScanPeakStep;

					// 传值
					setBScanGateFuZhi(Wnd, fPeakGate);
				}
				else if(wParam == VK_RIGHT)
				{
					if(fPeakGate < 100)
						fPeakGate += wBScanPeakStep;
					// 传值
					setBScanGateFuZhi(Wnd, fPeakGate);
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
	return CallWindowProc(OldBtnBScanGateFuZhiProc,Wnd,Msg,wParam,lParam);
}
/**************************
*扫描方式按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnBScanScanModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndBScan);
					hWndBScan = 0;

					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(drawBScanFlag = TRUE)
					{
						//结束B扫描
						InitializeCriticalSection(&csBScan);
						EnterCriticalSection(&csBScan);
						gbBscanSwitch = OFF;
						LeaveCriticalSection(&csBScan);
						DeleteCriticalSection(&csBScan);
					//	setBScanSwitch(Wnd, gbBscanSwitch);
						SetRedrawFlag(TRUE);		//擦掉B扫图	

						InitializeCriticalSection(&csCommand);
						EnterCriticalSection(&csCommand);
						//gbEndFlag =TRUE;
						thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
						LeaveCriticalSection(&csCommand);
						DeleteCriticalSection(&csCommand);
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(BScanBtnList[i] != currentBScanBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentBScanBtn = BScanBtnList[i];
					SetFocus(currentBScanBtn);
					gCurrentFocusBtn = currentBScanBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(BScanBtnList[i] != currentBScanBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentBScanBtn = BScanBtnList[i];
					SetFocus(currentBScanBtn);
					gCurrentFocusBtn = currentBScanBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(wScanMode == 1)
					{
						wchar_t strText[20];
						memset(strText, 0, sizeof(strText));
						wScanMode = 0;
						wsprintf(strText, _T("单次"));	
						DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					}
					else
					{
						wchar_t strText[20];
						memset(strText, 0, sizeof(strText));
						wScanMode = 1;
						wsprintf(strText, _T("循环"));	
						DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
						if(hDrawBScanThread != NULL)
							SetEvent(hDrawBScanEvent);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(wScanMode == 1)
					{
						wchar_t strText[20];
						memset(strText, 0, sizeof(strText));
						wScanMode = 0;
						wsprintf(strText, _T("单次"));	
						DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					}
					else
					{
						wchar_t strText[20];
						memset(strText, 0, sizeof(strText));
						wScanMode = 1;
						wsprintf(strText, _T("循环"));	
						DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
						if(hDrawBScanThread != NULL)
							SetEvent(hDrawBScanEvent);
					}
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
	return CallWindowProc(OldBtnBScanScanModeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*开启/关闭按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnBScanOnOrOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndBScan);
					hWndBScan = 0;

					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(drawBScanFlag = TRUE)
					{
						//结束B扫描
						gbBscanSwitch = OFF;
					//	setBScanSwitch(Wnd, gbBscanSwitch);
						SetRedrawFlag(TRUE);		//擦掉B扫图	

						InitializeCriticalSection(&csCommand);
						EnterCriticalSection(&csCommand);
						//gbEndFlag =TRUE;
						thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
						LeaveCriticalSection(&csCommand);
						DeleteCriticalSection(&csCommand);
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(BScanBtnList[i] != currentBScanBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentBScanBtn = BScanBtnList[i];
					SetFocus(currentBScanBtn);
					gCurrentFocusBtn = currentBScanBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(BScanBtnList[i] != currentBScanBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentBScanBtn = BScanBtnList[i];
					SetFocus(currentBScanBtn);
					gCurrentFocusBtn = currentBScanBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_RIGHT || wParam == VK_LEFT)
				{
					if(gbBscanSwitch == ON)
					{
						InitializeCriticalSection(&csBScan);
						EnterCriticalSection(&csBScan);
						gbBscanSwitch = OFF;
						LeaveCriticalSection(&csBScan);
						DeleteCriticalSection(&csBScan);

						SetRedrawFlag(TRUE);
						drawBScanFlag = FALSE;
						setBScanSwitch(Wnd, gbBscanSwitch);
						SetRedrawFlag(TRUE);		//擦掉B扫图	

						InitializeCriticalSection(&csCommand);
						EnterCriticalSection(&csCommand);
						//gbEndFlag =TRUE;
						thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
						LeaveCriticalSection(&csCommand);
						DeleteCriticalSection(&csCommand);
					}
					else
					{
						InitializeCriticalSection(&csBScan);
						EnterCriticalSection(&csBScan);
						gbBscanSwitch = ON;
						LeaveCriticalSection(&csBScan);
						DeleteCriticalSection(&csBScan);

						SetRedrawFlag(TRUE);
						drawBScanFlag = TRUE;
						setBScanSwitch(Wnd, gbBscanSwitch);
						//wangbingfu modify start at 2011-3-25
						/*chCurrent_gate_reg->wGate1Alarm = 0;//设置为超高报警
						
						GateInfos[CurrentChannelNo][0].alarmMode = (alarm_modes)0; //设置为超高报警
						*/
					//	SetQiDian(NULL,1, 1, gfPlateHeight + 10, TRUE);
						Sleep(50);//停止工作线程前先等待坐标轴变换完成
						//wangbingfu modify end at 2011-3-25
						//停止画波形
						InitializeCriticalSection(&csCommand);
						EnterCriticalSection(&csCommand);
						//gbEndFlag =TRUE;
						thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;
						LeaveCriticalSection(&csCommand);
						DeleteCriticalSection(&csCommand);
						Sleep(50);
						//unDrawWave();		//擦掉波形
						DisplayClear(g_bMenuShown);
						// 创建画B扫图线程
						if(hDrawBScanThread == NULL)
						{
							// 创建线程							
							DrawBScanThread_param.hWnd = Wnd;
							HDC hDC = GetDC(hWndMain);
							DrawBScanThread_param.hDC = hDC;

							hDrawBScanThread = ::CreateThread(NULL, 0, DrawBScanThreadProc, (LPVOID)&DrawBScanThread_param, 0, &dwDrawBScanThreadID);
							hDrawBScanEvent = CreateEvent(NULL, TRUE, TRUE, TEXT("DrawBScanEvent0"));
						}
						if(hDrawBScanThread != NULL)
							SetEvent(hDrawBScanEvent);
					}
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
	return CallWindowProc(OldBtnBScanOnOrOffProc,Wnd,Msg,wParam,lParam);
}
