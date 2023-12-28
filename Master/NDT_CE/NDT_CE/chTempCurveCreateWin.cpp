/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 测厚温度补偿曲线创建窗口源文件
 * Filename: chTempCurveCreateWin.cpp
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
#include "gateWin.h"

HWND hWndChTempCurveCreate = NULL;							//测厚温度补偿曲线创建窗口句柄
HWND currentChTempCurveCreateBtn = NULL;					//测厚温度补偿曲线创建窗口当前选中的按钮
WORD gwTempCurvePointNum[MATERIAL_NUM + 10] = {0,0};						//温度补偿曲线点的个数
BOOL gbDrawTempCurveCurveFlag = FALSE;					//是否在画温度补偿曲线
TEMPCURVEPOINT_DATA gstTempCurvePointData[MATERIAL_NUM + 10][TEMPCURVE_POINT_MAX];		//采集的温度补偿曲线点
BOOL gbTempCurveCurveOkFlag = FALSE;					//温度补偿曲线是否完成

static HWND hBtnChTempCurveCreateHoudu = NULL;					//厚度按钮句柄
static HWND hBtnChTempCurveCreateTemp = NULL;					//温度按钮句柄
static HWND hBtnChTempCurveCreateMeasureSpeed = NULL;			//测量的声速按钮句柄
static HWND hBtnChTempCurveCreateAddPoint = NULL;				//加样点按钮句柄
static HWND hBtnChTempCurveCreateDeletePoint = NULL;			//删样点按钮句柄
static HWND hBtnChTempCurveCreateEnd = NULL;					//结束按钮句柄

static HWND chTempCurveCreateBtnList[6] = {NULL,NULL,NULL,NULL,NULL,NULL};							//测厚温度补偿曲线创建窗口的按钮表
static WNDPROC OldBtnChTempCurveCreateHouduProc = NULL;				//厚度按钮默认处理过程
static WNDPROC OldBtnChTempCurveCreateTempProc = NULL;				//温度按钮默认处理过程
static WNDPROC OldBtnChTempCurveCreateMeasureSpeedProc = NULL;		//测量的声速按钮默认处理过程
static WNDPROC OldBtnChTempCurveCreateAddPointProc = NULL;			//加样点按钮默认处理过程
static WNDPROC OldBtnChTempCurveCreateDeletePointProc = NULL;		//删样点按钮默认处理过程
static WNDPROC OldBtnChTempCurveCreateEndProc = NULL;				//结束按钮默认处理过程

static const int buttonNum = 6;				//按钮控件数
static float fThickness = 10;			//试块厚度
static float fThicknessStep = 1;		//试块厚度步长
static WORD wThicknessStepIndex = 1;	//试块厚度步长索引
static float fTemperature = 10;			//温度
static float fTemperatureStep = 1;		//温度步长
static WORD wTemperatureStepIndex = 1;	//温度步长索引

static WORD wMeasureWaveSpeed = 0;		//测量的声速
static WORD wDeleteStep = 0;			
static POINT stTempCurvePoint[TEMPCURVE_POINT_MAX];

ATOM registerChTempCurveCreateChild(HINSTANCE hInstance);
static LRESULT CALLBACK chTempCurveCreateChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chTempCurveCreate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChTempCurveCreateHouduProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChTempCurveCreateTempProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChTempCurveCreateMeasureSpeedProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChTempCurveCreateAddPointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChTempCurveCreateDeletePointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChTempCurveCreateEndProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//程序实例句柄
extern HWND hWndTempCurveMake;				//温度补偿曲线制作窗口句
extern HWND currentTempCurveMakeBtn;		//温度补偿曲线制作窗中当前选中的按钮
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND gCurrentWnd;
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;						// 状态栏句柄
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD CurrentChannelNo;				// 当前通道号
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern WORD gwTempCurveNum;			//温度补偿曲线序号
extern CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// 工作线程参数
extern PEAK_INFO gPeakDefectInfos[2][2];	//保存闸门峰值点信息
extern CRITICAL_SECTION csPeakDefectInfos;
extern CRITICAL_SECTION csMenuState;
extern SYS_COLOR_INFO gstSysColor[SYS_COLOR_NUM];
extern int gCurrentSysColorIndex;
extern HWND hBtnTempCurveMakeCreate;//创建按钮句柄
extern TCHAR strTempCurveFileName[MAX_PATH];
extern GATE_INFO GateInfos[2][3];
extern GATE_INFO stOldGateInfos[2][3];

extern void SetDistanceGate(const WORD& nIndex);
extern void getTempCurveData();
/**************************
*保存温度补偿曲线到文件
* @param		hWnd[in]:窗口句柄
* @param		wNum[in]:序号
* @return   LRESULT :
* @since    1.00
***************************/
BOOL saveTempCurveFile(LPCTSTR lpszPathName)
{
	HANDLE hFileWrite;	//文件句柄
	DWORD dwWriteDataSize;
	hFileWrite = CreateFile(lpszPathName,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("创建文件失败"));
		return -1;
	}
	for(int i = 0; i < MATERIAL_NUM + 10; i++)
	{
		if(!WriteFile(hFileWrite,&gwTempCurvePointNum[i], sizeof(WORD),&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写文件失败"));

			CloseHandle(hFileWrite);
			return FALSE;
		}
	}
	for(int i = 0; i < MATERIAL_NUM + 10; i++)
	{
		if(!WriteFile(hFileWrite,gstTempCurvePointData[i], sizeof(TEMPCURVEPOINT_DATA) * TEMPCURVE_POINT_MAX, &dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写文件失败"));

			CloseHandle(hFileWrite);
			return FALSE;
		}
	}

	CloseHandle(hFileWrite);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("文件保存成功"));

	return TRUE;	
}


/**************************
*显示试块厚度
* @param		hWnd[in]:窗口句柄
* @param		fHouDu[in]:厚度
* @return   LRESULT :
* @since    1.00
***************************/
static void displayThickness(HWND hWnd ,float fHouDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fHouDu);	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}
/**************************
*显示温度
* @param		hWnd[in]:窗口句柄
* @param		fTemp[in]:温度
* @return   LRESULT :
* @since    1.00
***************************/
static void displayTemperature(HWND hWnd ,float fTemp)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fTemp);	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}

/**************************
*显示测量的声速
* @param		hWnd[in]:窗口句柄
* @param		wWaveSpeed[in]:声速
* @return   LRESULT :
* @since    1.00
***************************/
static void displayMeasureSpeed(HWND hWnd ,WORD wWaveSpeed)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(wWaveSpeed == 0)
		wsprintf(strText, _T("N/A"));
	else
		wsprintf(strText, _T("%d"), wWaveSpeed);
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}

/**************************
*注册测厚温度补偿曲线创建窗口
* @chTempCurveCreate    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChTempCurveCreateChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chTempCurveCreateChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChTempCurveCreateChild;

	return RegisterClass(&wc);
}
/**************************
*测厚温度补偿曲线创建窗口处理过程
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chTempCurveCreateChildProc(HWND hWnd, UINT message, 
											  WPARAM wParam, LPARAM lParam)
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
			SetDistanceGate(1);
			SetDistanceGate(2);
			wMeasureWaveSpeed = 0;
			if(currentTempCurveMakeBtn == hBtnTempCurveMakeCreate)//创建, 清空数据
			{
				gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] = 0;
				memset(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM], 0, sizeof(TEMPCURVE_POINT_MAX) * TEMPCURVE_POINT_MAX);
				//saveTempCurveFile(strTempCurveFileName);
			}
		//	getTempCurveData();
			//TEST
			/*gwTempCurvePointNum[10] = 2;
			gstTempCurvePointData[10][0].fTemperature= 25;
			gstTempCurvePointData[10][0].wWaveSpeed= 5920;
			gstTempCurvePointData[10][1].fTemperature= 80;
			gstTempCurvePointData[10][1].wWaveSpeed= 5979;*/
			
			gwTempCurvePointNum[11] = 2;
			gstTempCurvePointData[11][0].fTemperature= 100;
			gstTempCurvePointData[11][0].wWaveSpeed= 5000;
			gstTempCurvePointData[11][1].fTemperature= 200;
			gstTempCurvePointData[11][1].wWaveSpeed= 6000;
			//TEST
			memset(stTempCurvePoint, 0, sizeof(POINT) * TEMPCURVE_POINT_MAX);
			for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
			{
				stTempCurvePoint[i] = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i]);
			}
			gbTempCurveCurveOkFlag = FALSE;
			
			//画按钮控件
			hBtnChTempCurveCreateHoudu = CreateWindow(_T("BUTTON"),_T("chTempCurveCreateHoudu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCURVECREATE_HOUDU,hInst,NULL);
			hBtnChTempCurveCreateTemp = CreateWindow(_T("BUTTON"),_T("chTempCurveCreateTemp"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0+ BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCURVECREATE_TEMP,hInst,NULL);
			hBtnChTempCurveCreateMeasureSpeed = CreateWindow(_T("BUTTON"),_T("chTempCurveCreateSpeed"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCURVECREATE_MEASURESPEED,hInst,NULL);
			hBtnChTempCurveCreateAddPoint = CreateWindow(_T("BUTTON"),_T("chTempCurveCreateAddPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCURVECREATE_ADDPOINT,hInst,NULL);
			hBtnChTempCurveCreateDeletePoint = CreateWindow(_T("BUTTON"),_T("chTempCurveCreateDeletePoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCURVECREATE_DELETEPOINT,hInst,NULL);
			hBtnChTempCurveCreateEnd = CreateWindow(_T("BUTTON"),_T("chTempCurveCreateEnd"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 5*BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCURVECREATE_END,hInst,NULL);
					
			chTempCurveCreateBtnList[0] = hBtnChTempCurveCreateHoudu;
			chTempCurveCreateBtnList[1] = hBtnChTempCurveCreateTemp;
			chTempCurveCreateBtnList[2] = hBtnChTempCurveCreateMeasureSpeed;
			chTempCurveCreateBtnList[3] = hBtnChTempCurveCreateAddPoint;
			chTempCurveCreateBtnList[4] = hBtnChTempCurveCreateDeletePoint;
			chTempCurveCreateBtnList[5] = hBtnChTempCurveCreateEnd;
			
			//按钮控件的自定义处理过程
			OldBtnChTempCurveCreateHouduProc = (WNDPROC) GetWindowLong(hBtnChTempCurveCreateHoudu,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCurveCreateHoudu,GWL_WNDPROC,(LONG)btnChTempCurveCreateHouduProc);

			OldBtnChTempCurveCreateTempProc = (WNDPROC) GetWindowLong(hBtnChTempCurveCreateTemp,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCurveCreateTemp,GWL_WNDPROC,(LONG)btnChTempCurveCreateTempProc);
			
			OldBtnChTempCurveCreateMeasureSpeedProc = (WNDPROC) GetWindowLong(hBtnChTempCurveCreateMeasureSpeed,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCurveCreateMeasureSpeed,GWL_WNDPROC,(LONG)btnChTempCurveCreateMeasureSpeedProc);
			
			OldBtnChTempCurveCreateAddPointProc = (WNDPROC) GetWindowLong(hBtnChTempCurveCreateAddPoint,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCurveCreateAddPoint,GWL_WNDPROC,(LONG)btnChTempCurveCreateAddPointProc);
			
			OldBtnChTempCurveCreateDeletePointProc = (WNDPROC) GetWindowLong(hBtnChTempCurveCreateDeletePoint,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCurveCreateDeletePoint,GWL_WNDPROC,(LONG)btnChTempCurveCreateDeletePointProc);
			
			OldBtnChTempCurveCreateEndProc = (WNDPROC) GetWindowLong(hBtnChTempCurveCreateEnd,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCurveCreateEnd,GWL_WNDPROC,(LONG)btnChTempCurveCreateEndProc);
		
			currentChTempCurveCreateBtn = chTempCurveCreateBtnList[0];//默认当前选中测量厚度按钮
			SetFocus(currentChTempCurveCreateBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentChTempCurveCreateBtn;
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
			chTempCurveCreate_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnChTempCurveCreateHoudu)
			{
				displayThickness(pdis->hwndItem,fThickness);
			}
			if(pdis->hwndItem == hBtnChTempCurveCreateTemp)
			{
				displayTemperature(pdis->hwndItem,fTemperature);
			}
			if(pdis->hwndItem == hBtnChTempCurveCreateMeasureSpeed)
			{
				SetDistanceGate(1);
				SetDistanceGate(2);
				displayMeasureSpeed(pdis->hwndItem,wMeasureWaveSpeed);
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
*加载测厚温度补偿曲线创建窗口按钮图片
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chTempCurveCreate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chTempCurveCreateBtnBitmap[]=
	{
		{IDC_CHTEMPCURVECREATE_HOUDU	,IDB_BITMAP_SHIKUAIHOUDU6_NOSEL_R	 ,  IDB_BITMAP_SHIKUAIHOUDU6_SELECT_R		},
		{IDC_CHTEMPCURVECREATE_TEMP	,IDB_BITMAP_WENDU_NOSEL_R	 ,  IDB_BITMAP_WENDU_SELECT_R		},
		{IDC_CHTEMPCURVECREATE_MEASURESPEED	,IDB_BITMAP_CELIANGSHENGSU_NOSEL_R	 ,  IDB_BITMAP_CELIANGSHENGSU_SELECT_R		},	
		{IDC_CHTEMPCURVECREATE_ADDPOINT	,IDB_BITMAP_CHJIAYANGDIAN5_NOSEL_R ,  IDB_BITMAP_CHJIAYANGDIAN5_SELECT_R	},
		{IDC_CHTEMPCURVECREATE_DELETEPOINT	,IDB_BITMAP_SHANYANGDIAN5_NOSEL_R ,  IDB_BITMAP_SHANYANGDIAN5_SELECT_R	},
		{IDC_CHTEMPCURVECREATE_END	,IDB_BITMAP_JIESHU5_NOSEL_R ,  IDB_BITMAP_JIESHU5_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chTempCurveCreateBtnBitmap);
	return ReturnVal;
}

/**************************
*厚度按钮自定义处理过程
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCurveCreateHouduProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), fThicknessStep);
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
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;

					gCurrentFocusBtn = currentTempCurveMakeBtn;

					gCurrentWnd = hWndTempCurveMake;
					
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					SetOpenFlag(NULL, FALSE, 1);		//关掉闸门1
					SetOpenFlag(NULL, FALSE, 2);		//关掉闸门2

					gbDrawTempCurveCurveFlag = TRUE;		//改变坐标轴
					SetRedrawFlag(TRUE);					//Y轴才能重画
					Sleep(100);						//等待	确认重画完成

					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					//gbEndFlag =TRUE;
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					Sleep(50);//等待，确认画图线程停止画波形
					clearWave();//删除波形
				//	SetRedrawFlag(TRUE);	//分格虚线有可能被部分擦掉，所以要重画 线程已停不起作用
					//画所有点
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i],BLUE_POINT, TRUE);
					}
				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(fThickness > 1)
					{
						if((fThickness - fThicknessStep) >= 1)
							fThickness -= fThicknessStep;
						else
							fThickness = 1;

						wMeasureWaveSpeed = 0;
						displayMeasureSpeed(hBtnChTempCurveCreateMeasureSpeed, wMeasureWaveSpeed);
					}
					displayThickness(Wnd, fThickness);

					SetDistanceGate(1);
					SetDistanceGate(2);
				}
				else if(wParam == VK_RIGHT)
				{
					fThickness += fThicknessStep;

					wMeasureWaveSpeed = 0;
					displayMeasureSpeed(hBtnChTempCurveCreateMeasureSpeed, wMeasureWaveSpeed);
					
					displayThickness(Wnd, fThickness);

					SetDistanceGate(1);
					SetDistanceGate(2);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == 'F')
				{
					if(wThicknessStepIndex < 3)
						wThicknessStepIndex++;
					else
						wThicknessStepIndex = 0;

					fThicknessStep = AMENDSTEPS[wThicknessStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fThicknessStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnChTempCurveCreateHouduProc,Wnd,Msg,wParam,lParam);
}

/**************************
*温度按钮自定义处理过程
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCurveCreateTempProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), fTemperatureStep);
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
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;

					gCurrentFocusBtn = currentTempCurveMakeBtn;

					gCurrentWnd = hWndTempCurveMake;
					
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(fTemperature > 0)
					{
						if((fTemperature - fTemperatureStep) >= 0)
							fTemperature -= fTemperatureStep;
						else
							fTemperature = 0;

						wMeasureWaveSpeed = 0;
						displayMeasureSpeed(hBtnChTempCurveCreateMeasureSpeed, wMeasureWaveSpeed);
					}
					displayTemperature(Wnd, fTemperature);
				}
				else if(wParam == VK_RIGHT)
				{
					fTemperature += fTemperatureStep;

					wMeasureWaveSpeed = 0;
					displayMeasureSpeed(hBtnChTempCurveCreateMeasureSpeed, wMeasureWaveSpeed);
					
					displayTemperature(Wnd, fTemperature);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == 'F')
				{
					if(wTemperatureStepIndex < 3)
						wTemperatureStepIndex++;
					else
						wTemperatureStepIndex = 0;

					fTemperatureStep = AMENDSTEPS[wTemperatureStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fTemperatureStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnChTempCurveCreateTempProc,Wnd,Msg,wParam,lParam);
}


/**************************
*测量的声速按钮自定义处理过程
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCurveCreateMeasureSpeedProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;

					gCurrentFocusBtn = currentTempCurveMakeBtn;

					gCurrentWnd = hWndTempCurveMake;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					SetOpenFlag(NULL, FALSE, 1);		//关掉闸门1
					SetOpenFlag(NULL, FALSE, 2);		//关掉闸门2

					gbDrawTempCurveCurveFlag = TRUE;		//改变坐标轴
					SetRedrawFlag(TRUE);					//Y轴才能重画
					Sleep(100);								//等待	确认重画完成
					
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					//gbEndFlag =TRUE;
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					Sleep(50);//等待，确认画图线程停止画波形
					clearWave();//删除波形
					//SetRedrawFlag(TRUE);		//分格虚线有可能被部分擦掉，所以要重画,线程已停止，不起作用
					// 画虚线,分格虚线有可能被部分擦掉，所以要重画
					RECT RctWave;	// 波形区域
					RECT RctYScale; // Y坐标区域
					RECT RctXScale; // X坐标区域
					RECT RctRight;	// 右侧空白区
					RECT rect;	// 主窗口显示出来的矩形
					HDC hdc = GetDC(hWndMain);
					GetClipBox(hdc, &rect);
						
					InitializeCriticalSection(&csMenuState);
					EnterCriticalSection(&csMenuState);
					GetDisplayRect(rect, FALSE, RctWave, RctXScale, RctYScale, RctRight);
					LeaveCriticalSection(&csMenuState);
					DeleteCriticalSection(&csMenuState);
					
					DrawRectDashLine(RctWave, RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack));
					//画所有点
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i],BLUE_POINT, TRUE);
					}
					//画曲线
					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], TRUE);
					
				}
				else if(wParam == VK_RETURN)
				{
					float fMeasureHoudu = 0;	//测量的厚度
				 	float fRang1 = 0;
					float fRang2 = 0;
					PEAK_INFO peakDefectInfos[2][2];
					InitializeCriticalSection(&csPeakDefectInfos);
					EnterCriticalSection(&csPeakDefectInfos);
					for(int i = 0 ;i < CHANNEL_NUM; i++)
					{
						peakDefectInfos[i][0] = gPeakDefectInfos[i][0];
						peakDefectInfos[i][1] = gPeakDefectInfos[i][1];
					}
					LeaveCriticalSection(&csPeakDefectInfos);
					DeleteCriticalSection(&csPeakDefectInfos);
					fRang1 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][0].wDefectTime);
					fRang2 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][1].wDefectTime);
					fMeasureHoudu = fRang2 - fRang1;
					wMeasureWaveSpeed = (WORD)(fMeasureHoudu / (fThickness / DisplaySet_ad[CurrentChannelNo].nWaveSpeed));
					displayMeasureSpeed(hBtnChTempCurveCreateMeasureSpeed, wMeasureWaveSpeed);
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
	return CallWindowProc(OldBtnChTempCurveCreateMeasureSpeedProc,Wnd,Msg,wParam,lParam);
}

/**************************
*加样点按钮自定义处理过程
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCurveCreateAddPointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;

					gCurrentFocusBtn = currentTempCurveMakeBtn;

					gCurrentWnd = hWndTempCurveMake;
					
					gbDrawTempCurveCurveFlag = FALSE;		//还原坐标轴
					SetRedrawFlag(TRUE);					//Y轴才能重画

					//擦掉所有点
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i], BLUE_POINT, FALSE);
					}
					//擦掉曲线
					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);
					//开始画波形
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					gbDrawTempCurveCurveFlag = FALSE;		//还原坐标轴
					SetRedrawFlag(TRUE);					//Y轴才能重画

					//擦掉所有点
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i], BLUE_POINT, FALSE);
					}
					//擦掉曲线
					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);
					//开始画波形
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					SetOpenFlag(NULL, TRUE, 1);//  打开闸门线
					SetOpenFlag(NULL, TRUE, 2);//  打开闸门线
				}
				else if(wParam == VK_DOWN)
				{
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					if(gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] != 0)
						stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
					drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//选中点画红
				}
				else if(wParam == VK_RETURN)
				{
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));
					if(wMeasureWaveSpeed != 0)
					{
						drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);//擦掉曲线
						
						for(DWORD i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
						{
							if(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i].fTemperature == fTemperature)
							{
								stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i]);
								drawShiZhi1(stTempPoint, BLUE_POINT, FALSE);//擦掉同一温度的点
								
								gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i].wWaveSpeed= wMeasureWaveSpeed;
								stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i]);
								drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);//画点
								break;
							}
						}
						gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]++;
						if(gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] > TEMPCURVE_POINT_MAX)
							gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] = TEMPCURVE_POINT_MAX;
						gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1].fTemperature = fTemperature;
						gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1].wWaveSpeed = wMeasureWaveSpeed;
						stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1]);
						drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);//画增加的点

						sortSelectedPointDataByTemp(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM], gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]);

						for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
						{
							stTempCurvePoint[i] = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i]);
						}
						
						drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], TRUE);//重新画曲线
					}
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
			/*	else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
				else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChTempCurveCreateAddPointProc,Wnd,Msg,wParam,lParam);
}
/**************************
*删样点按钮自定义处理过程
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCurveCreateDeletePointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;

					gCurrentFocusBtn = currentTempCurveMakeBtn;

					gCurrentWnd = hWndTempCurveMake;

					gbDrawTempCurveCurveFlag = FALSE;		//还原坐标轴
					SetRedrawFlag(TRUE);					//Y轴才能重画

					//擦掉所有点
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i], BLUE_POINT, FALSE);
					}
					//擦掉曲线
					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);
					//开始画波形
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);//擦掉曲线
					//画所有点
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i], BLUE_POINT, TRUE);
					}
				}
				else if(wParam == VK_LEFT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));

					stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
					drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);			//上次选中点还原颜色
					if(gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM])
					{						
						if(wDeleteStep == 0)
							wDeleteStep = gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1;
						else
							wDeleteStep--;

						stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
						drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//选中点画红
						float fRate = 100 * ((float)gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep].wWaveSpeed -(float) DisplaySet_ad[CurrentChannelNo].nWaveSpeed) / DisplaySet_ad[CurrentChannelNo].nWaveSpeed;
						swprintf(strText, _T("%.1f,%.1f"), gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep].fTemperature, fRate);
					}
					else
					{
						swprintf(strText, _T("无样点"));
						
					}
					DrawLableText(Wnd, strText, 18, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else if(wParam == VK_RIGHT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));

					stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
					drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);			//上次选中点还原颜色
					if(gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM])
					{
						if(wDeleteStep == gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1)
							wDeleteStep = 0;
						else
							wDeleteStep++;

						stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
						drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//选中点画红
						float fRate = 100 * ((float)gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep].wWaveSpeed - (float)DisplaySet_ad[CurrentChannelNo].nWaveSpeed) / DisplaySet_ad[CurrentChannelNo].nWaveSpeed;
						swprintf(strText, _T("%.1f,%.1f"), gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep].fTemperature, fRate);
					}
					else
					{
						swprintf(strText, _T("无样点"));
						
					}
					DrawLableText(Wnd, strText, 18, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else if(wParam == VK_RETURN)
				{
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));

					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);//擦掉曲线
					
					if(gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM])
					{
						if(gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] == 1)		//只有一个点
						{	
							stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][0]);
							drawShiZhi1(stTempPoint, RED_POINT, FALSE);			//删除选中点
							gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]--;
						}
						else
						{							
							if(wDeleteStep == (gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1))//删最后一个点
							{
								stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
								drawShiZhi1(stTempPoint, RED_POINT, FALSE);			//擦掉选中点
								gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]--;
								wDeleteStep--;
							}
							else
							{
								for(int i = wDeleteStep; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1; i++)
								{
									stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
									drawShiZhi1(stTempPoint, RED_POINT, FALSE);			//擦掉选中点
									gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i] = gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i + 1];//选中点后面的点数据往前移
								}
								gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]--;
							}
							stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
							drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//新的选中点画红
							
							for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
							{
								stTempCurvePoint[i] = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i]);
							}
							
							drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], TRUE);//重新画曲线
						}
					}
					else
					{
						TCHAR strText[20];
						memset(strText, 0, sizeof(strText));
						swprintf(strText, _T("无样点可删"));
						DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					}
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
			/*	else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
				else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChTempCurveCreateDeletePointProc,Wnd,Msg,wParam,lParam);
}
/**************************
*删样点按钮自定义处理过程
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCurveCreateEndProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;

					gCurrentFocusBtn = currentTempCurveMakeBtn;

					gbDrawTempCurveCurveFlag = FALSE;		//还原坐标轴
					SetRedrawFlag(TRUE);					//Y轴才能重画

					gCurrentWnd = hWndTempCurveMake;

					//擦掉所有点
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i], BLUE_POINT, FALSE);
					}
					//擦掉曲线
					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);
					//开始画波形
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], TRUE);//画曲线
				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					SetOpenFlag(NULL, TRUE, 1);		//打开闸门1
					SetOpenFlag(NULL, TRUE, 2);		//打开闸门2

					gbDrawTempCurveCurveFlag = FALSE;		//还原坐标轴
					SetRedrawFlag(TRUE);					//Y轴才能重画

					//擦掉所有点
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i], BLUE_POINT, FALSE);
					}
					//开始画波形
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					SetOpenFlag(NULL, TRUE, 1);//  打开闸门线
					SetOpenFlag(NULL, TRUE, 2);//  打开闸门线
				}
				else if(wParam == VK_RETURN)
				{
					saveTempCurveFile(strTempCurveFileName);
					
					gbDrawTempCurveCurveFlag = FALSE;		//还原坐标轴
					SetRedrawFlag(TRUE);					//Y轴才能重画
					
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
					
					gbTempCurveCurveOkFlag = TRUE;
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//上级菜单最后选中的按钮获得输入焦点
					gCurrentFocusBtn = currentTempCurveMakeBtn;
					gCurrentWnd = hWndTempCurveMake;
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
			/*	else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
				else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChTempCurveCreateEndProc,Wnd,Msg,wParam,lParam);
}

