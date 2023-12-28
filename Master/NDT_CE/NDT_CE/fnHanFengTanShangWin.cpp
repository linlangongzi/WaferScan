/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 焊缝探伤窗口源文件
 * Filename: fnHanFengTanShangWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnHanFengTanShangWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "fnPoKouSheZhiWin.h"
#include "gateWin.h"
#include "const.h"

HWND hWndHanFengTanShang = NULL;			//焊缝探伤窗口句柄
HWND hBtnHanFengTanShangPoKouSheZhi = NULL;		//坡口设置按钮句柄
HWND hBtnHanFengTanShangGangBanHouDu = NULL;		//钢板厚度按钮句柄
HWND hBtnHanFengTanShangDistance = NULL;	//探头到焊缝距离按钮句柄
HWND hBtnHanFengTanShangRefractAngle = NULL;	//折射角按钮句柄
HWND hBtnHanFengTanShangOnOrOff = NULL;	//开启关闭按钮句柄

HWND hanFengTanShangBtnList[5] = {NULL, NULL, NULL, NULL, NULL,};	//焊缝探伤窗口的按钮表
WNDPROC OldBtnHanFengTanShangPoKouSheZhiProc = NULL;//坡口设置按钮默认处理过程
WNDPROC OldBtnHanFengTanShangGangBanHouDuProc = NULL;		//钢板厚度按钮默认处理过程
WNDPROC OldBtnHanFengTanShangDistanceProc = NULL;	//探头到焊缝距离按钮默认处理过程
WNDPROC OldBtnHanFengTanShangRefractAngleProc = NULL;	//折射角按钮默认处理过程
WNDPROC OldBtnHanFengTanShangOnOrOffProc = NULL;	//开启关闭按钮默认处理过程

 
HWND currentHanFengTanShangBtn;					//焊缝探伤窗口当前选中的按钮
BOOL bHanFengTanShangSwitch = OFF;				//焊缝探伤开关

HANDLE hDrawWaveTraceThread = NULL;				// 画超声波路径线程句柄
DRAWTHREAD_PARAM DrawWaveTraceThread_param;		// 画超声波路径线程参数
//HANDLE hDrawWaveTraceEvent = NULL;				// 画超声波路径事件
DWORD dwDrawWaveTraceThreadID = 0;				// 画超声波路径线程ID

CRITICAL_SECTION csHanFengTanShang;

static const int buttonNum = 5;					//按钮控件数

float fHanFengTanShangGangBanHouDu = 0; //钢板厚度
float fHead2HanFengDistance = 0;;			//探头到焊缝距离
static float fHanFengTanShangGangBanHouDuStep = 1;	//钢板厚度步长
static float fHead2HanFengDistanceStep = 1;		//探头到焊缝距离步长
static WORD wHanFengTanShangGangBanHouDuStepIndex = 1;
static WORD wHead2HanFengDistanceStepIndex = 1;

extern HINSTANCE hInst;						//程序实例句柄

extern HWND hWndMain;
extern HWND hWndSpecialFunction;
extern HWND currentSpecialFunctionBtn;
extern HWND gCurrentFocusBtn;
extern HWND hWndPoKouSheZhi;
extern HWND currentPoKouSheZhiBtn;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD CurrentChannelNo;
extern float fZheSheAngleStep;
extern WORD nZheSheAngleStepIndex; 

extern WORD gwPoKouLeiXing;					//坡口类型
extern float gfPoKouJiaoDu;					//坡口角度
extern float gfPoKouShenDu;					//坡口深度

extern BOOL gbGuanLian;
extern WORD *chCurrent_gate1curve;
extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
extern ARS200FPGA_CH_GATE_REG *ch2_gate_reg;
extern WORD *ch1_gate1curve;
extern WORD *ch2_gate1curve;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern ARS200FPGA_CH_CON_REG *chCurrent_con_reg;
extern GATE_INFO GateInfos[2][3];

extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];		//DAC通道开关
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;							//当前显示的子窗口

extern wchar_t gszTextStep[10];
extern BOOL gbHotRange;			// 快捷键声程
extern BOOL gbHotDB;			// 快捷键增益
extern HWND hStatusBar;			// 状态栏句柄
extern PEAK_INFO gPeakDefectInfos[2][2];
extern CRITICAL_SECTION csPeakDefectInfos;
extern ARS200FPGA_SYS_REG* sys_reg;
extern SYS_COLOR_INFO gstSysColor[SYS_COLOR_NUM];
extern int gCurrentSysColorIndex;
extern float gfPlateHeight;		//工件厚度
extern CRITICAL_SECTION csProbeType;
extern WORD wTanTouLeiXing[CHANNEL_NUM];
extern WORD gwMaterialType;				//材料类型
extern BOOL gbAlarmRegFlag[CHANNEL_NUM][GATE_NUM];		//通道报警标志
extern BOOL gbChannelAlarmFlag[CHANNEL_NUM];			//闸门报警标志
extern CRITICAL_SECTION csAlarmFlag;

extern BOOL gbAlarmFlag;
extern WORD typeToWaveSpeed(WORD wType);
extern void SetWaveSpeedLabelAndExecute(HWND hWnd);


extern void SetAngleLabelAndExecute();
extern WORD waveSpeedToType(WORD wWaveSpeed);
/**************************
*注册焊缝探伤窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerHanFengTanShangChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) hanFengTanShangChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szHanFengTanShangChild;

	return RegisterClass(&wc);
}
/**************************
*显示钢板厚度
* @param		hWnd[in]: 窗口句柄
* @param		fHouDu[in]: 钢板厚度
* @return   LRESULT :
* @since    1.00
***************************/
void displayHanFengTanShangGangBanHouDu(HWND hWnd ,float fHouDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fHouDu);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}
void displayHanFengTanShangJiaoDu(HWND hWnd ,float fJiaoDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fJiaoDu);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}

/**************************
*显示探头到坡口距离
* @param		hWnd[in]: 窗口句柄
* @param		fDistance[in]: 探头到坡口距离
* @return   LRESULT :
* @since    1.00
***************************/
void displayHead2HanFengDistance(HWND hWnd ,float fDistance)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fDistance);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}
/**************************
*显示探头角度
* @param		hWnd[in]: 窗口句柄
* @param		fAngle[in]: 探头角度
* @return   LRESULT :
* @since    1.00
***************************/
void displayHanFengTanShangRefractAngle(HWND hWnd ,float fAngle)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fAngle);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}
/**************************
*显示开关
* @param		hWnd[in]: 窗口句柄
* @param		bSwitch[in]: 开关
* @return   LRESULT :
* @since    1.00
***************************/
void displayHanFengTanShangOnOrOff(HWND hWnd ,BOOL bSwitch)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(bSwitch == 0)
		wsprintf(strText, _T("关闭"));
	else
		wsprintf(strText, _T("开启"));
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

}
/**************************
*显示开关
* @param		hWnd[in]: 窗口句柄
* @param		bSwitch[in]: 开关
* @return   LRESULT :
* @since    1.00
***************************/
void drawPoKou(WORD wPoKouLeiXing, float fGangBanHouDu, float fPoKouShenDu, float fPoKouJiaoDu, BOOL bDrawFlag, HDC hdc)
{
	WORD wColor;
	DWORD dwPoKouScreenShenDu; //屏幕坡口深度
	DWORD dwScreenGangBanHouDu; //屏幕钢板厚度长度
	float fGangBanLength;		//画进屏幕截取的钢板长度
	float fPoKouLength;			//坡口长度
	HDC hdcs = GetDC(hWndMain);
	RECT rect;
	GetClipBox(hdcs, &rect);

	POINT point1,point2,point3,point4,point5,point6;
	POINT pointLeft, pointRight;

	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH; //- MENU_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	// 波形区域宽度、高度
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;

	memset(&point1, 0 ,sizeof(POINT));
	memset(&point2, 0 ,sizeof(POINT));
	memset(&point3, 0 ,sizeof(POINT));
	memset(&point4, 0 ,sizeof(POINT));
	memset(&point5, 0 ,sizeof(POINT));
	memset(&point6, 0 ,sizeof(POINT));
	memset(&pointLeft, 0 ,sizeof(POINT));
	memset(&pointRight, 0 ,sizeof(POINT));
	
	double fAngle = fPoKouJiaoDu/180.0f * PI;

	if(bDrawFlag == TRUE)
		wColor = RED_POINT;
	else
		wColor = BLACK_POINT;
	
	fGangBanLength = float(fHead2HanFengDistance + fPoKouShenDu / tan(fAngle) + fHead2HanFengDistance / 6);
	fPoKouLength =	float(2* (fPoKouShenDu / tan(fAngle)));

	dwScreenGangBanHouDu = DWORD((RctWave.right - RctWave.left) * (fGangBanHouDu / fGangBanLength));
	if(dwScreenGangBanHouDu > DWORD(nWaveHeight /2))
		dwScreenGangBanHouDu = nWaveHeight /2; //最多只占波形区域高度的一半
	dwPoKouScreenShenDu = DWORD(dwScreenGangBanHouDu *(fPoKouShenDu / fGangBanHouDu));

	point1.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance - fPoKouLength / 2) / fGangBanLength);
	if(point1.x < RctWave.left)
		point1.x = RctWave.right;
	point1.y = DWORD(RctWave.top);

	point2.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance + fPoKouLength / 2) / fGangBanLength);
	if(point2.x > RctWave.right)
		point2.x = RctWave.right;
	point2.y = DWORD(RctWave.top);

	if(wPoKouLeiXing == 0)//V型口
	{
		point3.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance / fGangBanLength));
		point3.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);
		drawLine(point1, point3, wColor);	//画V型口左半边
		drawLine(point3, point2, wColor);	//画V型口右半边
	}
	else if(wPoKouLeiXing == 1)//Y型口
	{
		point3.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance / fGangBanLength));
		point3.y = DWORD(RctWave.top + dwPoKouScreenShenDu);

		point4.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance / fGangBanLength));
		point4.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

		drawLine(point1, point3, wColor);	//画Y型口左半边
		drawLine(point3, point2, wColor);	//画Y型口右半边
		drawLine(point3, point4, wColor);	//画Y型口竖线
	}

	else if(wPoKouLeiXing == 2)//双Y型口
	{
		point3.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance / fGangBanLength));
		point3.y = DWORD(RctWave.top + dwPoKouScreenShenDu);
		
		point4.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance / fGangBanLength));
		point4.y = DWORD(RctWave.top + dwScreenGangBanHouDu - dwPoKouScreenShenDu);

		point5.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance - fPoKouLength / 2) / fGangBanLength);
		if(point5.x < RctWave.left)
			point5.x = RctWave.right;
		point5.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

		point6.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance + fPoKouLength / 2) / fGangBanLength);
		if(point6.x > RctWave.right)
			point6.x = RctWave.right;
		point6.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

		drawLine(point1, point3, wColor);	//画双Y型口上半部分左半边
		drawLine(point3, point2, wColor);	//画双Y型口上半部分右半边
		drawLine(point3, point4, wColor);	//画双Y型口竖线
		drawLine(point5, point4, wColor);	//画双Y型口下半部分左半边
		drawLine(point4, point6, wColor);	//画双Y型口下半部分右半边
	}
	else
	{
		;//
	}
	pointLeft.x = RctWave.left;
	pointLeft.y = DWORD(RctWave.top + dwScreenGangBanHouDu);

	pointRight.x = RctWave.right;
	pointRight.y = DWORD(RctWave.top + dwScreenGangBanHouDu);

	drawLine(pointLeft, pointRight, wColor);

	ReleaseDC(hWndMain, hdcs);
}
DWORD WINAPI DrawWaveTraceThreadProc(LPVOID lpParameter)
{
	DRAWTHREAD_PARAM* pParam = (DRAWTHREAD_PARAM*)lpParameter;
	HDC hdc = pParam->hDC;
	RECT rect;
	GetClipBox(hdc, &rect);

	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH; //- MENU_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	// 波形区域宽度、高度
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;

	RECT RctHanFeng;// 焊缝区域
	RctHanFeng.left = RctWave.left;
	RctHanFeng.right = RctWave.right; //- MENU_WIDTH;
	RctHanFeng.top = RctWave.top;
	RctHanFeng.bottom = RctWave.bottom / 2;

	POINT point1;	//发射点
	POINT point2;
	POINT point3;
	POINT point4;
	POINT point5;
	POINT point6;
	
	memset(&point1, 0 ,sizeof(POINT));
	memset(&point2, 0 ,sizeof(POINT));
	memset(&point3, 0 ,sizeof(POINT));
	memset(&point4, 0 ,sizeof(POINT));
	memset(&point5, 0 ,sizeof(POINT));
	memset(&point6, 0 ,sizeof(POINT));
	
	float fRange = 0;
	float fDistance = 0;	//波到底部的声程
	DWORD dwBottomPointScreenDistance = 0;	//底部折射点的屏幕长度
	DWORD dwPerRangeDots = 0;	//单位声程的像素点
	DWORD dwScreenGangBanHouDu = 0; //屏幕钢板厚度长度
	float fGangBanLength = 0;		//画进屏幕截取的钢板长度
	float fPoKouLength = 0;			//坡口长度
	WORD wZheXianNum = 0;			//折线数量 
		
	double fAngle1 = DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI; //探头角度
	double fAngle2 = gfPoKouJiaoDu/180.0f * PI;	//坡口角度

	if(cos(fAngle1) > 0)
		fDistance = float(gfPlateHeight / cos(fAngle1));

	fPoKouLength =	float(2* (gfPoKouShenDu / tan(fAngle2)));
	fGangBanLength = float(fHead2HanFengDistance + gfPoKouShenDu / tan(fAngle2) + fHead2HanFengDistance / 6);
	dwScreenGangBanHouDu = DWORD((RctWave.right - RctWave.left) * (gfPlateHeight / fGangBanLength));
	if(dwScreenGangBanHouDu > DWORD(nWaveHeight /2))
		dwScreenGangBanHouDu = nWaveHeight /2; //最多只占波形区域高度的一半
	dwPerRangeDots = DWORD((RctWave.right - RctWave.left) / fGangBanLength);
	
	dwBottomPointScreenDistance = DWORD(dwScreenGangBanHouDu * tan(fAngle1));

	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	HBRUSH hbr = CreateSolidBrush(wColorBack);
	FillRect(hdc, &RctHanFeng, hbr);
	DeleteObject(hbr);
	while(1)
	{
		if(wZheXianNum == 1)
		{
			drawLine(point1, point2, wColorBack);
			drawShiZhi(point2, BLUE_POINT, FALSE, RctWave);
		}
		else if(wZheXianNum == 2)
		{
			drawLine(point1, point2, wColorBack);
			drawLine(point2, point3, wColorBack);
			drawShiZhi(point3, BLUE_POINT, FALSE, RctWave);
		}
			
		else if(wZheXianNum == 3)
		{
			drawLine(point1, point2, wColorBack);
			drawLine(point2, point3, wColorBack);
			drawLine(point3, point4, wColorBack);
			drawShiZhi(point4, BLUE_POINT, FALSE, RctWave);
		}
		else if(wZheXianNum == 4)
		{
			drawLine(point1, point2, wColorBack);
			drawLine(point2, point3, wColorBack);
			drawLine(point3, point4, wColorBack);
			drawLine(point4, point5, wColorBack);
			drawShiZhi(point5, BLUE_POINT, FALSE, RctWave);
		}
		else if(wZheXianNum == 5)
		{
			drawLine(point1, point2, wColorBack);
			drawLine(point2, point3, wColorBack);
			drawLine(point3, point4, wColorBack);
			drawLine(point4, point5, wColorBack);
			drawLine(point5, point5, wColorBack);
			drawShiZhi(point6, BLUE_POINT, FALSE, RctWave);
		}
		else if(wZheXianNum > 5)
		{
			drawLine(point1, point2, wColorBack);
			drawLine(point2, point3, wColorBack);
			drawLine(point3, point4, wColorBack);
			drawLine(point4, point5, wColorBack);
			drawLine(point5, point6, wColorBack);
		}

		InitializeCriticalSection(&csHanFengTanShang);
		EnterCriticalSection(&csHanFengTanShang);
		BOOL bExit = bHanFengTanShangSwitch;
		LeaveCriticalSection(&csHanFengTanShang);
		DeleteCriticalSection(&csHanFengTanShang);

		if(bExit == OFF)
		{
			hDrawWaveTraceThread = NULL;
			return 0;
		}

		drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, TRUE, hdc);
		
		BOOL bAlarmRegFlag[CHANNEL_NUM][GATE_NUM];		//通道报警标志
		BOOL bChannelAlarmFlag[CHANNEL_NUM];			//闸门报警标志
		
		InitializeCriticalSection(&csAlarmFlag);
		EnterCriticalSection(&csAlarmFlag);
		for(int i = 0; i < CHANNEL_NUM; i++)
		{
			for(int j = 0; j < GATE_NUM; j++)
			{
				bAlarmRegFlag[i][j] = gbAlarmRegFlag[i][j];
			}
			bChannelAlarmFlag[i] = gbChannelAlarmFlag[i];
		}
		LeaveCriticalSection(&csAlarmFlag);
		DeleteCriticalSection(&csAlarmFlag);

		if((bChannelAlarmFlag[CurrentChannelNo]) && bAlarmRegFlag[CurrentChannelNo][0])
		{
			point1.x = (DWORD)(RctWave.left);
			point1.y = (DWORD)(RctWave.top);

		//	point2.x = (DWORD)(RctWave.left+ dwBottomPointScreenDistance);
		//	point2.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

			InitializeCriticalSection(&csPeakDefectInfos);
			EnterCriticalSection(&csPeakDefectInfos);
			fRange = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, int(gPeakDefectInfos[CurrentChannelNo][0].wDefectTime));
			LeaveCriticalSection(&csPeakDefectInfos);
			DeleteCriticalSection(&csPeakDefectInfos);

			wZheXianNum = (WORD)(fRange / fDistance + 1);

			if(wZheXianNum == 1)
			{
				point2.x = DWORD(RctWave.left + (fRange * sin(fAngle1) * dwPerRangeDots));
				point2.y = DWORD(RctWave.top + (fRange *cos(fAngle1) *dwPerRangeDots));

				drawLine(point1, point2, BLUE_POINT);
				drawShiZhi(point2, BLUE_POINT, TRUE, RctWave);
			}
			else if(wZheXianNum == 2)
			{
				point2.x = (DWORD)(RctWave.left+ dwBottomPointScreenDistance);
				point2.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

				point3.x = DWORD(RctWave.left + dwBottomPointScreenDistance + ((fRange - fDistance) *sin(fAngle1) * dwPerRangeDots));
				point3.y = DWORD(point2.y - ((fRange - fDistance) *cos(fAngle1) *dwPerRangeDots));

				drawLine(point1, point2, BLUE_POINT);
				drawLine(point2, point3, BLUE_POINT);
				drawShiZhi(point3, BLUE_POINT, TRUE, RctWave);
			}
				
			else if(wZheXianNum == 3)
			{
				point2.x = (DWORD)(RctWave.left+ dwBottomPointScreenDistance);
				point2.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

				point3.x = (DWORD)(point2.x + dwBottomPointScreenDistance);
				point3.y = (DWORD)RctWave.top;

				point4.x = DWORD(point3.x + ((fRange - 2*fDistance) *sin(fAngle1) * dwPerRangeDots));
				point4.y = DWORD(point3.y + (fRange - 2*fDistance) *cos(fAngle1) *dwPerRangeDots);

				drawLine(point1, point2, BLUE_POINT);
				drawLine(point2, point3, BLUE_POINT);
				drawLine(point3, point4, BLUE_POINT);
				drawShiZhi(point4, BLUE_POINT, TRUE, RctWave);
			}
			else if(wZheXianNum == 4)
			{
				point2.x = (DWORD)(RctWave.left+ dwBottomPointScreenDistance);
				point2.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

				point3.x = (DWORD)(point2.x + dwBottomPointScreenDistance);
				point3.y = (DWORD)RctWave.top;

				point4.x = DWORD(point3.x + dwBottomPointScreenDistance);
				point4.y = DWORD(RctWave.top + dwScreenGangBanHouDu);

				point5.x = DWORD(point4.x + ((fRange - 3*fDistance) *sin(fAngle1) * dwPerRangeDots));
				point5.y = DWORD(point4.y - ((fRange - 3*fDistance) *cos(fAngle1) *dwPerRangeDots));

				drawLine(point1, point2, BLUE_POINT);
				drawLine(point2, point3, BLUE_POINT);
				drawLine(point3, point4, BLUE_POINT);
				drawLine(point4, point5, BLUE_POINT);
				drawShiZhi(point5, BLUE_POINT, TRUE, RctWave);
			}
			else if(wZheXianNum == 5)
			{
				point2.x = (DWORD)(RctWave.left+ dwBottomPointScreenDistance);
				point2.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

				point3.x = (DWORD)(point2.x + dwBottomPointScreenDistance);
				point3.y = (DWORD)RctWave.top;

				point4.x = DWORD(point3.x + dwBottomPointScreenDistance);
				point4.y = DWORD(RctWave.top + dwScreenGangBanHouDu);

				point5.x = DWORD(point4.x + dwBottomPointScreenDistance);
				point5.y = DWORD(RctWave.top);

				point6.x = DWORD(point5.x + ((fRange - 4*fDistance) *sin(fAngle1) * dwPerRangeDots));
				point6.y = DWORD(point5.y + (fRange - 4*fDistance) *cos(fAngle1) *dwPerRangeDots);

				drawLine(point1, point2, BLUE_POINT);
				drawLine(point2, point3, BLUE_POINT);
				drawLine(point3, point4, BLUE_POINT);
				drawLine(point4, point5, BLUE_POINT);
				drawLine(point5, point6, BLUE_POINT);
				drawShiZhi(point6, BLUE_POINT, TRUE, RctWave);
			}
			else if(wZheXianNum > 5)
			{
				point2.x = (DWORD)(RctWave.left+ dwBottomPointScreenDistance);
				point2.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

				point3.x = (DWORD)(point2.x + dwBottomPointScreenDistance);
				point3.y = (DWORD)RctWave.top;

				point4.x = DWORD(point3.x + dwBottomPointScreenDistance);
				point4.y = DWORD(RctWave.top + dwScreenGangBanHouDu);

				point5.x = DWORD(point4.x + dwBottomPointScreenDistance);
				point5.y = DWORD(RctWave.top);

				point6.x = DWORD(point5.x + dwBottomPointScreenDistance);
				point6.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

				drawLine(point1, point2, BLUE_POINT);
				drawLine(point2, point3, BLUE_POINT);
				drawLine(point3, point4, BLUE_POINT);
				drawLine(point4, point5, BLUE_POINT);
				drawLine(point5, point6, BLUE_POINT);
			}
			//报警读取一次重置
		//	chCurrent_con_reg->wReset |= 0x1;
		//	chCurrent_con_reg->wReset &= 0xFFFE;
			
		}
		Sleep(10);
	}
	return 0;

}
void setHanFengTanShangOnOrOff(HWND hWnd ,BOOL bSwitch)
{
//	float fFront;
//	float fBack;
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(bSwitch == 0)
		wsprintf(strText, _T("关闭"));
	else
		wsprintf(strText, _T("开启"));
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	double fAngle = DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI;

    if(bSwitch == ON)
	{
	/*	if(cos(fAngle) != 0)
		{
			fFront = float(gfPlateHeight / cos(fAngle));
			fBack = float(2 * (gfPlateHeight / cos(fAngle)));
		}

		float fRange = fBack - fFront;
		WORD wDots = (WORD)Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fRange);

		if(!gbGuanLian)
		{
			chCurrent_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			chCurrent_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
			
			GateInfos[CurrentChannelNo][0].fFrontGate = fFront;
			GateInfos[CurrentChannelNo][0].fBackGate = fBack;

			//SetGateCurve(chCurrent_gate1curve, wDots, 50);
			chCurrent_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 50 / 100.0f);

			GateInfos[CurrentChannelNo][0].fPeakGate = 50;

			chCurrent_gate_reg->wGate1Alarm = 0;

			GateInfos[CurrentChannelNo][0].alarmMode = (alarm_modes)0;
		}
		else
		{
			ch1_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			ch2_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));

			GateInfos[0][0].fFrontGate = fFront;
			GateInfos[0][0].fBackGate = fBack;

			ch1_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
			ch2_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));

			GateInfos[1][0].fFrontGate = fFront;
			GateInfos[1][0].fBackGate = fBack;

			//SetGateCurve(ch1_gate1curve, wDots, 50);
			ch1_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 50 / 100.0f);
			//SetGateCurve(ch2_gate1curve, wDots, 50);
			ch2_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 50 / 100.0f);

			GateInfos[0][0].fPeakGate = 50;
			GateInfos[1][0].fPeakGate = 50;

			ch1_gate_reg->wGate1Alarm = 0;
			ch2_gate_reg->wGate1Alarm = 0;

			GateInfos[0][0].alarmMode = (alarm_modes)0;
			GateInfos[1][0].alarmMode = (alarm_modes)0;
		}*/
		chCurrent_gate_reg->wGate1Enable = ON;
		GateInfos[CurrentChannelNo][0].bOpen = ON;
		// 创建画超声波路径图线程
		if(hDrawWaveTraceThread == NULL)
		{
			// 创建线程							
			DrawWaveTraceThread_param.hWnd = hWnd;
			HDC hDC = GetDC(hWndMain);
			DrawWaveTraceThread_param.hDC = hDC;

			hDrawWaveTraceThread = ::CreateThread(NULL, 0, DrawWaveTraceThreadProc, (LPVOID)&DrawWaveTraceThread_param, 0, &dwDrawWaveTraceThreadID);
		//	hDrawWaveTraceEvent = CreateEvent(NULL, TRUE, TRUE, TEXT("DrawWaveTraceEvent0"));
		//	ReleaseDC(hWnd, hDC);//这里不能RELEASE
		}
	//	if(hDrawWaveTraceThread != NULL)
	//		SetEvent(hDrawWaveTraceEvent);

	//	drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, TRUE);
	//	drawWaveTrace(gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, TRUE);
	}
/*	if(bSwitch == OFF)
	{
	//	chCurrent_gate_reg->wGate1Enable = OFF;		//Delete at2010-12-22 wangbingfu
	//	GateInfos[CurrentChannelNo][0].bOpen = OFF; //Delete at2010-12-22 wangbingfu
	//	drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);
	//	drawWaveTrace(gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);
	}*/
	
}
/**************************
*焊缝探伤窗口处理过程
* @param		Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK hanFengTanShangChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			gwMaterialType = waveSpeedToType(DisplaySet_ad[CurrentChannelNo].nWaveSpeed); //必须，不然变折射角会出错
			if(gwPoKouLeiXing == 0)
			{	
				double fAngle = gfPoKouJiaoDu/180.0f * PI;
				gfPoKouShenDu = gfPlateHeight;
				fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
			}
			else if(gwPoKouLeiXing == 1)
			{
				if(gfPoKouShenDu >  gfPlateHeight)
				{
					double fAngle = gfPoKouJiaoDu/180.0f * PI;
					gfPoKouShenDu = gfPlateHeight - 1;
					fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
					
				}
			}
			else
			{
				if(2 * gfPoKouShenDu >  gfPlateHeight)
				{
					double fAngle = gfPoKouJiaoDu/180.0f * PI;
					gfPoKouShenDu = gfPlateHeight / 2 - 1;
					fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
					
				}
			}
	//		closeAllGate();//关闭所有闸门
			//关掉振动和灯闪
  			if((sys_reg->wLedAlarm & 0x1) == 0x1)
			{
				sys_reg->wLedAlarm &= 0xFFFE;
			}
			//关闭DAC
		/*	for(int i =0; i < GATE_NUM; i++)
			{
				gDgsSwitch[CurrentChannelNo][i] = 0;
			}*/

			fHanFengTanShangGangBanHouDuStep = AMENDSTEPS[wHanFengTanShangGangBanHouDuStepIndex];
			fHead2HanFengDistanceStep = AMENDSTEPS[wHead2HanFengDistanceStepIndex];
			fZheSheAngleStep = ANGLESTEPS[nZheSheAngleStepIndex];
			//画按钮控件
			hBtnHanFengTanShangPoKouSheZhi = CreateWindow(_T("BUTTON"),_T("PoKouSheZhiHanFengTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_HANFENGTANSHANG_POKOUSHEZHI,hInst,NULL);
			hBtnHanFengTanShangGangBanHouDu = CreateWindow(_T("BUTTON"),_T("gangBanHouDuHanFengTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_HANFENGTANSHANG_GANGBANHOUDU,hInst,NULL);
			hBtnHanFengTanShangDistance = CreateWindow(_T("BUTTON"),_T("distanceHanFengTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_HANFENGTANSHANG_DISTANCE,hInst,NULL);
			hBtnHanFengTanShangRefractAngle = CreateWindow(_T("BUTTON"),_T("refractAngleHanFengTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_HANFENGTANSHANG_REFRACTANGLE,hInst,NULL);
			hBtnHanFengTanShangOnOrOff = CreateWindow(_T("BUTTON"),_T("OnOrOffHanFengTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_HANFENGTANSHANG_ONOROFF,hInst,NULL);			
			
			hanFengTanShangBtnList[0] = hBtnHanFengTanShangPoKouSheZhi;
			hanFengTanShangBtnList[1] = hBtnHanFengTanShangGangBanHouDu;
			hanFengTanShangBtnList[2] = hBtnHanFengTanShangDistance;
			hanFengTanShangBtnList[3] = hBtnHanFengTanShangRefractAngle;
			hanFengTanShangBtnList[4] = hBtnHanFengTanShangOnOrOff;
			
			
			//按钮控件的自定义处理过程
			OldBtnHanFengTanShangPoKouSheZhiProc = (WNDPROC) GetWindowLong(hBtnHanFengTanShangPoKouSheZhi,GWL_WNDPROC);
			SetWindowLong(hBtnHanFengTanShangPoKouSheZhi,GWL_WNDPROC,(LONG)btnHanFengTanShangPoKouSheZhiProc);
			
			OldBtnHanFengTanShangGangBanHouDuProc = (WNDPROC) GetWindowLong(hBtnHanFengTanShangGangBanHouDu,GWL_WNDPROC);
			SetWindowLong(hBtnHanFengTanShangGangBanHouDu,GWL_WNDPROC,(LONG)btnHanFengTanShangGangBanHouDuProc);
			
			OldBtnHanFengTanShangDistanceProc = (WNDPROC) GetWindowLong(hBtnHanFengTanShangDistance,GWL_WNDPROC);
			SetWindowLong(hBtnHanFengTanShangDistance,GWL_WNDPROC,(LONG)btnHanFengTanShangDistanceProc);
			
			OldBtnHanFengTanShangRefractAngleProc = (WNDPROC) GetWindowLong(hBtnHanFengTanShangRefractAngle,GWL_WNDPROC);
			SetWindowLong(hBtnHanFengTanShangRefractAngle,GWL_WNDPROC,(LONG)btnHanFengTanShangRefractAngleProc);

			OldBtnHanFengTanShangOnOrOffProc = (WNDPROC) GetWindowLong(hBtnHanFengTanShangOnOrOff,GWL_WNDPROC);
			SetWindowLong(hBtnHanFengTanShangOnOrOff,GWL_WNDPROC,(LONG)btnHanFengTanShangOnOrOffProc);
		
			currentHanFengTanShangBtn = hanFengTanShangBtnList[0];//默认当前选中坡口设置按钮
			SetFocus(currentHanFengTanShangBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentHanFengTanShangBtn;
			break;
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
			hanFengTanShang_DrawButtonBitmap(hInst,pdis);
			
			if(pdis->hwndItem == hBtnHanFengTanShangGangBanHouDu)
			{
				displayHanFengTanShangGangBanHouDu(pdis->hwndItem, gfPlateHeight);
			}
			else if(pdis->hwndItem == hBtnHanFengTanShangDistance)
			{
				displayHead2HanFengDistance(pdis->hwndItem, fHead2HanFengDistance);
			}
			else if(pdis->hwndItem == hBtnHanFengTanShangRefractAngle)
			{
				displayHanFengTanShangRefractAngle(pdis->hwndItem, DisplaySet_ad[CurrentChannelNo].fZheSheJiao);
			}
			else if(pdis->hwndItem == hBtnHanFengTanShangOnOrOff)
			{
				displayHanFengTanShangOnOrOff(pdis->hwndItem, bHanFengTanShangSwitch);
			}
			else
			{
				;//
			}

			// 检测按钮句柄，显示相应的步长
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentHanFengTanShangBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnHanFengTanShangGangBanHouDu)
						swprintf(gszTextStep, _T("%.1f"), fHanFengTanShangGangBanHouDuStep);
					else if(pdis->hwndItem == hBtnHanFengTanShangDistance)
						swprintf(gszTextStep, _T("%.1f"), fHead2HanFengDistanceStep);
					else if(pdis->hwndItem == hBtnHanFengTanShangRefractAngle)
						swprintf(gszTextStep, _T("%.1f"), fZheSheAngleStep);
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
*加载焊缝探伤窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int hanFengTanShang_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_hanFengTanShangBtnBitmap[]=
	{
		{IDC_HANFENGTANSHANG_POKOUSHEZHI	,IDB_BITMAP_POKOUSHEZHI_NOSEL_R	 ,  IDB_BITMAP_POKOUSHEZHI_SELECT_R		},
		{IDC_HANFENGTANSHANG_GANGBANHOUDU	,IDB_BITMAP_GANGBANHOUDU5_NOSEL_R	 ,  IDB_BITMAP_GANGBANHOUDU5_SELECT_R		},
		{IDC_HANFENGTANSHANG_DISTANCE	,IDB_BITMAP_TANPOJU_NOSEL_R ,  IDB_BITMAP_TANPOJU_SELECT_R	},
		{IDC_HANFENGTANSHANG_REFRACTANGLE	,IDB_BITMAP_ZHESHEJIAO_NOSEL_R ,  IDB_BITMAP_ZHESHEJIAO_SELECT_R	},
		{IDC_HANFENGTANSHANG_ONOROFF	,IDB_BITMAP_KAIQIGUANBI5_NOSEL_R ,  IDB_BITMAP_KAIQIGUANBI5_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_hanFengTanShangBtnBitmap);
	return ReturnVal;
}

/**************************
*坡口设置按钮自定义处理过程
* @param		Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHanFengTanShangPoKouSheZhiProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndHanFengTanShang);
					hWndHanFengTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(bHanFengTanShangSwitch == ON)
					{
						chCurrent_gate_reg->wGate1Enable = OFF;
						GateInfos[CurrentChannelNo][0].bOpen = OFF;
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);
						
						//drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);	//清除坡口图
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndPoKouSheZhi = CreateWindow(szPoKouSheZhiChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndHanFengTanShang,SW_HIDE);
				//	SetFocus(hWndPoKouSheZhi);
					ShowWindow(hWndPoKouSheZhi,SW_SHOW);
					UpdateWindow(hWndPoKouSheZhi);

					gCurrentWnd = hWndPoKouSheZhi;
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
	return CallWindowProc(OldBtnHanFengTanShangPoKouSheZhiProc,hWnd,Msg,wParam,lParam);
}
/**************************
*钢板厚度按钮自定义处理过程
* @param		hWnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHanFengTanShangGangBanHouDuProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndHanFengTanShang);
					hWndHanFengTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(bHanFengTanShangSwitch == ON)
					{
						chCurrent_gate_reg->wGate1Enable = OFF;
						GateInfos[CurrentChannelNo][0].bOpen = OFF;
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);

						//drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);	//清除坡口图
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if((gfPlateHeight -  fHanFengTanShangGangBanHouDuStep) > MIN_GONGJIANHOUDU)
						{
								gfPlateHeight -= fHanFengTanShangGangBanHouDuStep;
						}
						else
						{
							gfPlateHeight = MIN_GONGJIANHOUDU;
						}
						displayHanFengTanShangGangBanHouDu(hWnd, gfPlateHeight);
						
						if(gwPoKouLeiXing == 0)
						{	
							double fAngle = gfPoKouJiaoDu/180.0f * PI;
							gfPoKouShenDu = gfPlateHeight;
							fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
							displayHead2HanFengDistance(hBtnHanFengTanShangDistance,fHead2HanFengDistance);
						}
						else if(gwPoKouLeiXing == 1)
						{
							if(gfPoKouShenDu >  gfPlateHeight)
							{
								double fAngle = gfPoKouJiaoDu/180.0f * PI;
								gfPoKouShenDu = gfPlateHeight - 1;
								fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
								displayHead2HanFengDistance(hBtnHanFengTanShangDistance,fHead2HanFengDistance);
								
							}
						}
						else
						{
							if(2 * gfPoKouShenDu >  gfPlateHeight)
							{
								double fAngle = gfPoKouJiaoDu/180.0f * PI;
								gfPoKouShenDu = gfPlateHeight / 2 - 1;
								fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
								displayHead2HanFengDistance(hBtnHanFengTanShangDistance,fHead2HanFengDistance);
								
							}
						}
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if((gfPlateHeight + fHanFengTanShangGangBanHouDuStep) <= MAX_fPlateHeight)
							gfPlateHeight += fHanFengTanShangGangBanHouDuStep;
						else
							gfPlateHeight = MAX_fPlateHeight;

						if(gwPoKouLeiXing == 0) //V型口
						{
							double fAngle = gfPoKouJiaoDu/180.0f * PI;
							gfPoKouShenDu = gfPlateHeight;
							fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
							displayHead2HanFengDistance(hBtnHanFengTanShangDistance,fHead2HanFengDistance);
						}
					
						displayHanFengTanShangGangBanHouDu(hWnd, gfPlateHeight);
					}
				}
				else if(wParam == 'F')
				{
					if(wHanFengTanShangGangBanHouDuStepIndex < 3)
						wHanFengTanShangGangBanHouDuStepIndex++;
					else
						wHanFengTanShangGangBanHouDuStepIndex = 0;

					fHanFengTanShangGangBanHouDuStep = AMENDSTEPS[wHanFengTanShangGangBanHouDuStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fHanFengTanShangGangBanHouDuStep);
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
	return CallWindowProc(OldBtnHanFengTanShangGangBanHouDuProc,hWnd,Msg,wParam,lParam);
}
/**************************
*探头到焊缝距离按钮自定义处理过程
* @param		hWnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHanFengTanShangDistanceProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndHanFengTanShang);
					hWndHanFengTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(bHanFengTanShangSwitch == ON)
					{
						chCurrent_gate_reg->wGate1Enable = OFF;
						GateInfos[CurrentChannelNo][0].bOpen = OFF;
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);

						//drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);	//清除坡口图
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						double fAngle = gfPoKouJiaoDu/180.0f * PI;
						if((fHead2HanFengDistance -  fHead2HanFengDistanceStep) >= float(gfPoKouShenDu / tan(fAngle)))
							fHead2HanFengDistance -= fHead2HanFengDistanceStep;
						displayHead2HanFengDistance(hWnd, fHead2HanFengDistance);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						fHead2HanFengDistance += fHead2HanFengDistanceStep;
						displayHead2HanFengDistance(hWnd, fHead2HanFengDistance);
					}
				}
				else if(wParam == 'F')
				{
					if(wHead2HanFengDistanceStepIndex < 3)
						wHead2HanFengDistanceStepIndex++;
					else
						wHead2HanFengDistanceStepIndex = 0;

					fHead2HanFengDistanceStep = AMENDSTEPS[wHead2HanFengDistanceStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fHead2HanFengDistanceStep);
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
	return CallWindowProc(OldBtnHanFengTanShangDistanceProc,hWnd,Msg,wParam,lParam);
}
/**************************
*折射角按钮自定义处理过程
* @param		hWnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHanFengTanShangRefractAngleProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndHanFengTanShang);
					hWndHanFengTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(bHanFengTanShangSwitch == ON)
					{
						chCurrent_gate_reg->wGate1Enable = OFF;
						GateInfos[CurrentChannelNo][0].bOpen = OFF;
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);

						//drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);	//清除坡口图
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if(DisplaySet_ad[CurrentChannelNo].fZheSheJiao - fZheSheAngleStep > MIN_ANGLE)
						{
							DisplaySet_ad[CurrentChannelNo].fZheSheJiao -= fZheSheAngleStep;
						}
						else
						{
							DisplaySet_ad[CurrentChannelNo].fZheSheJiao = MIN_ANGLE;

						}						
						if ( fabs(DisplaySet_ad[CurrentChannelNo].fZheSheJiao) < 1E-6 )
						{
							InitializeCriticalSection(&csProbeType);
							EnterCriticalSection(&csProbeType);

							wTanTouLeiXing[CurrentChannelNo] = 1;//直探头
							InitializeCriticalSection(&csProbeType);
							EnterCriticalSection(&csProbeType);

							DisplaySet_ad[CurrentChannelNo].nWaveSpeed = typeToWaveSpeed(gwMaterialType);
							SetWaveSpeedLabelAndExecute(NULL);
						}
						
						displayHanFengTanShangJiaoDu(hWnd, DisplaySet_ad[CurrentChannelNo].fZheSheJiao);

						SetAngleLabelAndExecute();
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if(DisplaySet_ad[CurrentChannelNo].fZheSheJiao + fZheSheAngleStep < MAX_ANGLE)
						{
							DisplaySet_ad[CurrentChannelNo].fZheSheJiao += fZheSheAngleStep;

							InitializeCriticalSection(&csProbeType);
							EnterCriticalSection(&csProbeType);

							wTanTouLeiXing[CurrentChannelNo] = 0;//斜探头
							InitializeCriticalSection(&csProbeType);
							EnterCriticalSection(&csProbeType);

							DisplaySet_ad[CurrentChannelNo].nWaveSpeed = typeToWaveSpeed(gwMaterialType);
							SetWaveSpeedLabelAndExecute(NULL);
						}
						else
							DisplaySet_ad[CurrentChannelNo].fZheSheJiao = MAX_ANGLE;

						displayHanFengTanShangJiaoDu(hWnd, DisplaySet_ad[CurrentChannelNo].fZheSheJiao);

						SetAngleLabelAndExecute();						
					}
				}
				else if(wParam == 'F')
				{
					if(nZheSheAngleStepIndex < 1)
						nZheSheAngleStepIndex++;
					else
						nZheSheAngleStepIndex = 0;

					fZheSheAngleStep = ANGLESTEPS[nZheSheAngleStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fZheSheAngleStep);
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
	return CallWindowProc(OldBtnHanFengTanShangRefractAngleProc,hWnd,Msg,wParam,lParam);
}
/**************************
*开启关闭按钮自定义处理过程
* @hanFengTanShang    hInstance[in]:
* @hanFengTanShang		message[in]:
* @hanFengTanShang		wParam[in]:
* @hanFengTanShang		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHanFengTanShangOnOrOffProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndHanFengTanShang);
					hWndHanFengTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(bHanFengTanShangSwitch == ON)
					{
						chCurrent_gate_reg->wGate1Enable = OFF;
						GateInfos[CurrentChannelNo][0].bOpen = OFF;
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);

						//drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);	//清除坡口图
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(bHanFengTanShangSwitch == ON)
					{
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);
						setHanFengTanShangOnOrOff(hWnd, bHanFengTanShangSwitch);
					}
					else if(bHanFengTanShangSwitch == OFF && gfPlateHeight != 0)
					{
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = ON;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);
						Sleep(150);
						setHanFengTanShangOnOrOff(hWnd, bHanFengTanShangSwitch);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(bHanFengTanShangSwitch == ON)
					{
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);
						setHanFengTanShangOnOrOff(hWnd, bHanFengTanShangSwitch);
					}
					else if(bHanFengTanShangSwitch == OFF && gfPlateHeight != 0)
					{
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = ON;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);
						Sleep(150);
						setHanFengTanShangOnOrOff(hWnd, bHanFengTanShangSwitch);
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
	return CallWindowProc(OldBtnHanFengTanShangOnOrOffProc,hWnd,Msg,wParam,lParam);
}
