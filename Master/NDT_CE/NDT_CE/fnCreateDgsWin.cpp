/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 新建DAC曲线窗口源文件
 * Filename: CreateDacCurveWin.cpp
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
#include "fnCreateDgsWin.h"
#include "interfaceCmdID.h"
#include "const.h"
#include "resource.h"
#include "craftParamWin.h"
#include "gateWin.h"
#include "recvParamWin.h"

HWND hWndCreateDacCurve = NULL;//新建DAC曲线窗口句柄
HWND hBtnCreateDgsCurveAddPoint = NULL;//加样点按钮句柄
HWND hBtnCreateDgsCurveDeletePoint = NULL;//删样点按钮句柄
HWND hBtnCreateDgsCurvePingDing = NULL;//评定线按钮句柄
HWND hBtnCreateDgsCurveDingLiang = NULL;//定量线按钮句柄
HWND hBtnCreateDgsCurvePanHui = NULL;//判废线按钮句柄
HWND hBtnCreateDgsCurveEnd = NULL;//结束按钮句柄
HWND CreateDgsCurveBtnList[6];//新建DGS曲线窗口的按钮表
WNDPROC OldBtnCreateDgsCurveAddPointProc = NULL;//加样点按钮默认处理过程
WNDPROC OldBtnCreateDgsCurveDeletePointProc = NULL;//删样点按钮默认处理过程
WNDPROC OldBtnCreateDgsCurvePingDingProc = NULL;//评定线按钮默认处理过程
WNDPROC OldBtnCreateDgsCurveDingLiangProc = NULL;//定量线按钮默认处理过程
WNDPROC OldBtnCreateDgsCurvePanHuiProc = NULL;//判废线按钮默认处理过程
WNDPROC OldBtnCreateDgsCurveEndProc = NULL;//结束按钮默认处理过程
HWND currentCreateDgsCurveBtn = NULL;//新建DGS曲线窗中当前选中的按钮
POINT gstCurrentDgsPoint;//当前选中的DGS曲线点坐标

CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//添加的样点数据队列
CURVEPOINT_DATA gOldMakeDgsPointData[MAX_CURVE_POINT_NUM] = {0};//以加第一点基准增益为基准的制作线

CURVEPOINT_DATA gCurrentSelectedDgsPointData;//添加的样点中当前选中的DGS曲线点数据
WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM] = {{0,0,0,0},{0,0,0,0}};//已选中的DGS点数量

BOOL gWaveDrawDacFlag = FALSE;		//波形是否画DAC
float gfPingDingGain[CHANNEL_NUM] = {-3, -3};				//评定增益
float gfDingLiangGain[CHANNEL_NUM] = {0, 0};				//定量增益
float gfPanHuiGain[CHANNEL_NUM] = {3, 3};				//判废增益

float gfOldDacRange[CHANNEL_NUM] = {0, 0};				//最初生成DAC时的声程
short gwOldDacSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM] = {{0}, {0}};	//最初生成DAC时的每个点的位置
float gfDacFirstPointGain[CHANNEL_NUM] = {0, 0};			//DAC第一点增益值
float gfDacFirstPointChangeGain[CHANNEL_NUM] = {0, 0};			//制作时增益键改变的增益值

extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];

static WORD wCurveGainStepIndex = 1;		//线增益步长索引
static float fCurveGainStep = 1;			//线增益步长

static const int buttonNum = 3;//按钮控件数
static BOOL gOldDgsSwitch[CHANNEL_NUM][GATE_NUM] = {{0,0},{0,0}};				//保存DGS开关状态
static BOOL bOldAvgCurveFlag[CHANNEL_NUM] = {0,0};				//保存AVG开关状态

static float fOldGain = 0;		//刚进入新建或修改曲线时的基准增益值
extern HWND hWndDgsMake;//制作曲线窗口
extern HWND currentDgsMakeBtn;//制作曲线窗口中当前选中的按钮
extern HINSTANCE hInst;//程序实例句柄
extern HWND hWndMain;//主窗口句柄
extern HWND hStatusBar;

extern WORD CurrentChannelNo;

extern float fRangeEnd[2];
extern float fRangeStart[2];

extern HWND gCurrentFocusBtn;
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];		//DGS通道开关
extern wchar_t gszTextStep[10];
extern BOOL gbGuanLian;
extern BOOL bFreezeUp;
extern wchar_t gszTextPeak[20];
extern HWND gCurrentWnd;		//当前显示的子窗口
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD gwDeleteStep;					//删样点位置
extern PEAK_INFO gPeakDefectInfos[2][2];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern BOOL gbDeletePointFlag;		//是否在删样点操作
extern HWND hBtnDgsMakeCreate;//创建按钮句柄
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG曲线开关标志
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//AVG添加的样点数据队列
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//已选中的AVG点数量
extern BOOL gbHotDB;			// 快捷键增益
extern BOOL gbHotRange;			// 快捷键声程


/**************************
*注册新建DAC曲线窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerCreateDgsCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));

	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) createDgsCurveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szCreateDacCurveChild;

	return RegisterClass(&wc);
}
static void displayCurveGain(HWND hWnd, float fGain)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fGain);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}

/**************************
*新建DGS曲线窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK createDgsCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
				fOldGain = ReceiveParam_ad[CurrentChannelNo].fPlus;
				memset(gOldMakeDgsPointData, 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
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
						gDgsSwitch[i][j] = FALSE;
					}
				}
				//保存AVG开关状态					
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
						bOldAvgCurveFlag[i] = gbAvgCurveFlag[i];
				}
				//关闭AVG 方便做曲线
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					gbAvgCurveFlag[i] = FALSE;
				}
				
				gwDeleteStep = 0;
				gbDeletePointFlag = FALSE;
				float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
				ReceiveParam_ad[CurrentChannelNo].fRefPlus = 0;
				SetRefPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);
				refGainChangeCurve(fOldRefPlus, FALSE);//wangbingfu add at 2011-03-09
				
				gfDacFirstPointChangeGain[0] = 0;
				gfDacFirstPointChangeGain[1] = 0;
				if(currentDgsMakeBtn == hBtnDgsMakeCreate)
				{
					gSelectedDgsPointDataNum[CurrentChannelNo][0] = 0;
					gSelectedDgsPointDataNum[CurrentChannelNo][1] = 0;
					gSelectedDgsPointDataNum[CurrentChannelNo][2] = 0;
					gSelectedDgsPointDataNum[CurrentChannelNo][3] = 0;

					memset(gSelectedDgsPointData[CurrentChannelNo][0], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
					memset(gSelectedDgsPointData[CurrentChannelNo][1], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
					memset(gSelectedDgsPointData[CurrentChannelNo][2], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
					memset(gSelectedDgsPointData[CurrentChannelNo][3], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);

					memset(&gCurrentSelectedDgsPointData, 0, sizeof(CURVEPOINT_DATA));

					memset(gwOldDacSerial[CurrentChannelNo], 0, sizeof(WORD) * MAX_CURVE_POINT_NUM);
				}
				
				gWaveDrawDacFlag = TRUE;
				SetRedrawFlag(TRUE);

				if(gSelectedDgsPointDataNum[CurrentChannelNo][0] > 1)
				{
					for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][0]; i++)
					{
						gOldMakeDgsPointData[i] = gSelectedDgsPointData[CurrentChannelNo][3][i];
					}
				}
				
				//画按钮控件
				hBtnCreateDgsCurveAddPoint = CreateWindow(_T("BUTTON"),_T("DgsCurveAddPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_ADDPOINT,hInst,NULL);

				hBtnCreateDgsCurveDeletePoint = CreateWindow(_T("BUTTON"),_T("DgsCurveDeletePoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_DELETEPOINT,hInst,NULL);
				
		/*		hBtnCreateDgsCurvePingDing = CreateWindow(_T("BUTTON"),_T("pingDIng"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_PINGDING,hInst,NULL);

				
				hBtnCreateDgsCurveDingLiang = CreateWindow(_T("BUTTON"),_T("dingLiang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_DINGLIANG,hInst,NULL);

				hBtnCreateDgsCurvePanHui = CreateWindow(_T("BUTTON"),_T("panHui"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_PANHUI,hInst,NULL);
		*/
				hBtnCreateDgsCurveEnd = CreateWindow(_T("BUTTON"),_T("DgsCurveEnd"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_END,hInst,NULL);
			
				CreateDgsCurveBtnList[0] = hBtnCreateDgsCurveAddPoint;
				CreateDgsCurveBtnList[1] = hBtnCreateDgsCurveDeletePoint;
			//	CreateDgsCurveBtnList[2] = hBtnCreateDgsCurvePingDing;
			//	CreateDgsCurveBtnList[3] = hBtnCreateDgsCurveDingLiang;
			//	CreateDgsCurveBtnList[4] = hBtnCreateDgsCurvePanHui;
				CreateDgsCurveBtnList[2] = hBtnCreateDgsCurveEnd;
				//按钮控件的自定义处理过程
				OldBtnCreateDgsCurveAddPointProc = (WNDPROC) GetWindowLong(hBtnCreateDgsCurveAddPoint,GWL_WNDPROC);
				SetWindowLong(hBtnCreateDgsCurveAddPoint,GWL_WNDPROC,(LONG)btnCreateDgsCurveAddPointProc);

				OldBtnCreateDgsCurveDeletePointProc = (WNDPROC) GetWindowLong(hBtnCreateDgsCurveDeletePoint,GWL_WNDPROC);
				SetWindowLong(hBtnCreateDgsCurveDeletePoint,GWL_WNDPROC,(LONG)btnCreateDgsCurveDeletePointProc);

				OldBtnCreateDgsCurvePingDingProc = (WNDPROC) GetWindowLong(hBtnCreateDgsCurvePingDing,GWL_WNDPROC);
				SetWindowLong(hBtnCreateDgsCurvePingDing,GWL_WNDPROC,(LONG)btnCreateDgsCurvePingDingProc);

				OldBtnCreateDgsCurveDingLiangProc = (WNDPROC) GetWindowLong(hBtnCreateDgsCurveDingLiang,GWL_WNDPROC);
				SetWindowLong(hBtnCreateDgsCurveDingLiang,GWL_WNDPROC,(LONG)btnCreateDgsCurveDingLiangProc);

				OldBtnCreateDgsCurvePanHuiProc = (WNDPROC) GetWindowLong(hBtnCreateDgsCurvePanHui,GWL_WNDPROC);
				SetWindowLong(hBtnCreateDgsCurvePanHui,GWL_WNDPROC,(LONG)btnCreateDgsCurvePanHuiProc);

				OldBtnCreateDgsCurveEndProc = (WNDPROC) GetWindowLong(hBtnCreateDgsCurveEnd,GWL_WNDPROC);
				SetWindowLong(hBtnCreateDgsCurveEnd,GWL_WNDPROC,(LONG)btnCreateDgsCurveEndProc);
			
				currentCreateDgsCurveBtn = CreateDgsCurveBtnList[0];  //默认当前选中加样点参数按钮
				SetFocus(currentCreateDgsCurveBtn);//选中按钮获得输入焦点
				gCurrentFocusBtn = currentCreateDgsCurveBtn;

			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndDgsMake,SW_SHOW);
				SetFocus(currentDgsMakeBtn);//上级菜单最后选中的按钮获得输入焦点

				DestroyWindow(hWndCreateDacCurve);
				hWndCreateDacCurve = 0;
				
				gCurrentFocusBtn = currentDgsMakeBtn;

				gCurrentWnd = hWndDgsMake;

				gWaveDrawDacFlag = FALSE;

				//还原DAC开关状态				
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
					{
						gDgsSwitch[i][j] = gOldDgsSwitch[i][j];
					}
				}
				//还原AVG开关状态
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
						gbAvgCurveFlag[i] = bOldAvgCurveFlag[i];
				}
			}
			
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:			
			pdis=(LPDRAWITEMSTRUCT)lParam;
			createDgsCurve_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnCreateDgsCurvePingDing)
			{
				displayCurveGain(pdis->hwndItem, gfPingDingGain[CurrentChannelNo]);
			}
			if(pdis->hwndItem == hBtnCreateDgsCurveDingLiang)
			{
				displayCurveGain(pdis->hwndItem, gfDingLiangGain[CurrentChannelNo]);
			}
			if(pdis->hwndItem == hBtnCreateDgsCurvePanHui)
			{
				displayCurveGain(pdis->hwndItem, gfPanHuiGain[CurrentChannelNo]);
			}

			// 检测按钮句柄，显示相应的步长
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentCreateDgsCurveBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnCreateDgsCurvePingDing || pdis->hwndItem == hBtnCreateDgsCurveDingLiang || pdis->hwndItem == hBtnCreateDgsCurvePanHui)
						swprintf(gszTextStep, _T("%.1f"), fCurveGainStep);
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
*加载新建DGS曲线窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int createDgsCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_CreateDgsCurveBtnBitmap[]=
	{
		{IDC_CREATEDGSCURVE_ADDPOINT	,IDB_BITMAP_JIAYANGDIAN_NOSEL_R	 ,  IDB_BITMAP_JIAYANGDIAN_SELECT_R		},
		{IDC_CREATEDGSCURVE_DELETEPOINT	,IDB_BITMAP_SHANYANGDIAN_NOSEL_R	 ,  IDB_BITMAP_SHANYANGDIAN_SELECT_R		},
	//	{IDC_CREATEDGSCURVE_PINGDING	,IDB_BITMAP_PINGDINGXIAN_NOSEL_R ,  IDB_BITMAP_PINGDINGXIAN_SELECT_R	},
	//	{IDC_CREATEDGSCURVE_DINGLIANG	,IDB_BITMAP_DINGLIANGXIAN_NOSEL_R ,  IDB_BITMAP_DINGLIANGXIAN_SELECT_R	},
	//	{IDC_CREATEDGSCURVE_PANHUI	,IDB_BITMAP_PANFEIXIAN_NOSEL_R ,  IDB_BITMAP_PANFEIXIAN_SELECT_R	},
		{IDC_CREATEDGSCURVE_END	,IDB_BITMAP_JIESHU_NOSEL_R ,  IDB_BITMAP_JIESHU_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_CreateDgsCurveBtnBitmap);
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
LRESULT CALLBACK btnCreateDgsCurveAddPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;					
				}
				else if(wParam == VK_DOWN)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;

					
					gbDeletePointFlag = TRUE;

					if(gSelectedDgsPointDataNum[CurrentChannelNo][3])
					{
						gCurrentSelectedDgsPointData = gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep];
						gstCurrentDgsPoint = dataToPoint(gCurrentSelectedDgsPointData);
						swprintf(strText, _T("%.1f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDgsPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDgsPointData.data * 100 / MAX_HEIGHT));
					}	
					else
					{
						swprintf(strText, _T("无样点"));
					}

					DrawLableText(hBtnCreateDgsCurveDeletePoint, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);					
				}
				else if(wParam == VK_LEFT)
				{
				}
				else if(wParam == VK_RIGHT)
				{
				}
				else if(wParam == VK_RETURN)
				{
					gWaveDrawDacFlag = FALSE;
					Sleep(100);//等待擦掉波形
					if(gSelectedDgsPointDataNum[CurrentChannelNo][3] < MAX_CURVE_POINT_NUM)
						gSelectedDgsPointDataNum[CurrentChannelNo][3]++;

					if(gSelectedDgsPointDataNum[CurrentChannelNo][3] == 1)
					{
						gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)gPeakDefectInfos[CurrentChannelNo][0].wDefectTime)
																										/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
						gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial = curveChangeByDelay(gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial, 
																								fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
						
						gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].data = (float)gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp;

						gOldMakeDgsPointData[0].data = gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].data;
						gOldMakeDgsPointData[0].serial = gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial;

						gfDacFirstPointGain[CurrentChannelNo] = ReceiveParam_ad[CurrentChannelNo].fPlus;
					}
					else
					{
						gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)gPeakDefectInfos[CurrentChannelNo][0].wDefectTime)
																										/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
						gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial = curveChangeByDelay(gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial, 
																								fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
						
						gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].data = (float)(gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp 
																														* pow(10, (double)((gfDacFirstPointGain[CurrentChannelNo] - gfDacFirstPointChangeGain[CurrentChannelNo]) - ReceiveParam_ad[CurrentChannelNo].fPlus) / 20));

						gOldMakeDgsPointData[gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].data , gfDacFirstPointChangeGain[CurrentChannelNo]);
						gOldMakeDgsPointData[gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial = gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial;
						sortSelectedPointDataBySerial(gOldMakeDgsPointData,gSelectedDgsPointDataNum[CurrentChannelNo][3]);
					}
					
					sortSelectedPointDataBySerial(gSelectedDgsPointData[CurrentChannelNo][3],gSelectedDgsPointDataNum[CurrentChannelNo][3]);

					gWaveDrawDacFlag = TRUE;

					if(!gbGuanLian)
					{
						gfOldDacRange[CurrentChannelNo] = DisplaySet_ad[CurrentChannelNo].fRange;
						
						for(int j = 0; j < gSelectedDgsPointDataNum[CurrentChannelNo][3]; j++)
						{
							gwOldDacSerial[CurrentChannelNo][j] = (short)gSelectedDgsPointData[CurrentChannelNo][3][j].serial;
						}
					}
					else
					{
						gfOldDacRange[0] = DisplaySet_ad[CurrentChannelNo].fRange;
						gfOldDacRange[1] = DisplaySet_ad[CurrentChannelNo].fRange;

						for(int j = 0; j < gSelectedDgsPointDataNum[CurrentChannelNo][3]; j++)
						{
							gwOldDacSerial[0][j] = (short)gSelectedDgsPointData[CurrentChannelNo][3][j].serial;
							gwOldDacSerial[1][j] = (short)gSelectedDgsPointData[CurrentChannelNo][3][j].serial;
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
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnCreateDgsCurveAddPointProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnCreateDgsCurveDeletePointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
					
					gbDeletePointFlag = FALSE;
				}
				else if(wParam == VK_DOWN)
				{
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;

					gbDeletePointFlag = FALSE;
				  }
				else if(wParam == VK_LEFT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					if(gSelectedDgsPointDataNum[CurrentChannelNo][3])
					{
						
						if(gwDeleteStep == 0)
							gwDeleteStep = gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1;
						else
							gwDeleteStep--;
						gCurrentSelectedDgsPointData = gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep];
						
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDgsPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDgsPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("无样点"));
						
					}
					DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else if(wParam == VK_RIGHT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					if(gSelectedDgsPointDataNum[CurrentChannelNo][3])
					{
						
						if(gwDeleteStep == gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1)
							gwDeleteStep = 0;
						else
							gwDeleteStep++;
						gCurrentSelectedDgsPointData = gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep];
						
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDgsPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDgsPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("无样点"));
						
					}
					DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else if(wParam == VK_RETURN)
				{
					if(gSelectedDgsPointDataNum[CurrentChannelNo][3])
					{
						gWaveDrawDacFlag = FALSE;
						Sleep(100);//等待擦掉波形
					 	if(gSelectedDgsPointDataNum[CurrentChannelNo][3] == 1)
					 	{
					 		gSelectedDgsPointDataNum[CurrentChannelNo][3]--;
					 	}
						else
						{
							if(gwDeleteStep == (gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1))//删最后一个点
							{
								gSelectedDgsPointDataNum[CurrentChannelNo][3]--;
								gwDeleteStep--;
							}
							else
							{
								for(i = gwDeleteStep; i < gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1; i++)
								{
									gSelectedDgsPointData[CurrentChannelNo][3][i] = gSelectedDgsPointData[CurrentChannelNo][3][i + 1];//选中点后面的点数据往前移
								}
								gSelectedDgsPointDataNum[CurrentChannelNo][3]--;
								for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
								{
									gOldMakeDgsPointData[i] = gSelectedDgsPointData[CurrentChannelNo][3][i];
								}
							}
						}
						gWaveDrawDacFlag = TRUE;
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
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnCreateDgsCurveDeletePointProc,Wnd,Msg,wParam,lParam);
}
/**************************
*评定线按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCreateDgsCurvePingDingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;

					gbDeletePointFlag = TRUE;

					if(gSelectedDgsPointDataNum[CurrentChannelNo][3])
					{
						gCurrentSelectedDgsPointData = gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep];
						
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDgsPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDgsPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("无样点"));
						
					}
					DrawLableText(hBtnCreateDgsCurveDeletePoint, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				
				else if(wParam == VK_DOWN)
				{
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfPingDingGain[CurrentChannelNo] != MIN_CURVE_GAIN)
					{
						if(gfPingDingGain[CurrentChannelNo] - fCurveGainStep > MIN_CURVE_GAIN)
						{
							gfPingDingGain[CurrentChannelNo] -= fCurveGainStep;
						}
						else
							gfPingDingGain[CurrentChannelNo] = MIN_CURVE_GAIN;

						displayCurveGain(Wnd, gfPingDingGain[CurrentChannelNo]);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfPingDingGain[CurrentChannelNo] != MAX_CURVE_GAIN)
					{
						if(gfPingDingGain[CurrentChannelNo] + fCurveGainStep < gfDingLiangGain[CurrentChannelNo])
						{
							gfPingDingGain[CurrentChannelNo] += fCurveGainStep;
						}

						displayCurveGain(Wnd, gfPingDingGain[CurrentChannelNo]);
					}
				}
				else if(wParam == VK_RETURN)
				{
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
					if(wCurveGainStepIndex < 1)
						wCurveGainStepIndex++;
					else
						wCurveGainStepIndex = 0;

					fCurveGainStep = CURVEGAINSTEPS[wCurveGainStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fCurveGainStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnCreateDgsCurvePingDingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*定量线按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCreateDgsCurveDingLiangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
				}
				
				else if(wParam == VK_DOWN)
				{
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfDingLiangGain[CurrentChannelNo] != MIN_CURVE_GAIN)
					{
						if(gfDingLiangGain[CurrentChannelNo] - fCurveGainStep > gfPingDingGain[CurrentChannelNo])
						{
							gfDingLiangGain[CurrentChannelNo] -= fCurveGainStep;
						}

						displayCurveGain(Wnd, gfDingLiangGain[CurrentChannelNo]);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfDingLiangGain[CurrentChannelNo] != MAX_CURVE_GAIN)
					{
						if(gfDingLiangGain[CurrentChannelNo] + fCurveGainStep < gfPanHuiGain[CurrentChannelNo])
						{
							gfDingLiangGain[CurrentChannelNo] += fCurveGainStep;
						}

						displayCurveGain(Wnd, gfDingLiangGain[CurrentChannelNo]);
					}
				}
				else if(wParam == VK_RETURN)
				{
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
					if(wCurveGainStepIndex < 1)
						wCurveGainStepIndex++;
					else
						wCurveGainStepIndex = 0;

					fCurveGainStep = CURVEGAINSTEPS[wCurveGainStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fCurveGainStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnCreateDgsCurveDingLiangProc,Wnd,Msg,wParam,lParam);
}

/**************************
*判废线按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCreateDgsCurvePanHuiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
				}
				
				else if(wParam == VK_DOWN)
				{
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfPanHuiGain[CurrentChannelNo] != MIN_CURVE_GAIN)
					{
						if(gfPanHuiGain[CurrentChannelNo] - fCurveGainStep > gfDingLiangGain[CurrentChannelNo])
						{
							gfPanHuiGain[CurrentChannelNo] -= fCurveGainStep;
						}

						displayCurveGain(Wnd, gfPanHuiGain[CurrentChannelNo]);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfPanHuiGain[CurrentChannelNo] != MAX_CURVE_GAIN)
					{
						if(gfPanHuiGain[CurrentChannelNo] + fCurveGainStep < MAX_CURVE_GAIN)
						{
							gfPanHuiGain[CurrentChannelNo] += fCurveGainStep;
						}
						else
							gfPanHuiGain[CurrentChannelNo] = MAX_CURVE_GAIN;

						displayCurveGain(Wnd, gfPanHuiGain[CurrentChannelNo]);
					}
				}
				else if(wParam == VK_RETURN)
				{
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
					if(wCurveGainStepIndex < 1)
						wCurveGainStepIndex++;
					else
						wCurveGainStepIndex = 0;

					fCurveGainStep = CURVEGAINSTEPS[wCurveGainStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fCurveGainStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnCreateDgsCurvePanHuiProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnCreateDgsCurveEndProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
				}
				
				else if(wParam == VK_DOWN)
				{
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{	
					ShowWindow(hWndDgsMake,SW_SHOW);
					SetFocus(currentDgsMakeBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndCreateDacCurve);
					hWndCreateDacCurve = 0;
					
					gCurrentFocusBtn = currentDgsMakeBtn;

					gCurrentWnd = hWndDgsMake;

					gWaveDrawDacFlag = FALSE;
					SetRedrawFlag(TRUE);
					
					ReceiveParam_ad[CurrentChannelNo].fPlus = gfDacFirstPointGain[CurrentChannelNo];
					SetPlusLableAndExecute(Wnd, gfDacFirstPointGain[CurrentChannelNo], ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);

					//制作过程中基准增益改变，制作线以加第一点的基准增益为基准再变回去
					/*for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
					{
						gSelectedDgsPointData[CurrentChannelNo][3][i].data = (DWORD)(gSelectedDgsPointData[CurrentChannelNo][3][i].data
																					/ pow(10, (double)(gfDacFirstPointChangeGain[CurrentChannelNo]) / (-20)));
					}*/
					if(gSelectedDgsPointDataNum[CurrentChannelNo][3] > 1)
					{
						for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
						{
							gSelectedDgsPointData[CurrentChannelNo][3][i] = gOldMakeDgsPointData[i];
						}
					}
					
					if(!gbGuanLian)
					{
						for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
						{	
							gSelectedDgsPointData[CurrentChannelNo][0][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, gfPingDingGain[CurrentChannelNo]);
							gSelectedDgsPointData[CurrentChannelNo][0][i].serial = gSelectedDgsPointData[CurrentChannelNo][3][i].serial;
							
							gSelectedDgsPointData[CurrentChannelNo][1][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, gfDingLiangGain[CurrentChannelNo]);
							gSelectedDgsPointData[CurrentChannelNo][1][i].serial = gSelectedDgsPointData[CurrentChannelNo][3][i].serial;
							
							gSelectedDgsPointData[CurrentChannelNo][2][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, gfPanHuiGain[CurrentChannelNo]);
							gSelectedDgsPointData[CurrentChannelNo][2][i].serial = gSelectedDgsPointData[CurrentChannelNo][3][i].serial;							
						}

						gSelectedDgsPointDataNum[CurrentChannelNo][0] = gSelectedDgsPointDataNum[CurrentChannelNo][3];
						gSelectedDgsPointDataNum[CurrentChannelNo][1] = gSelectedDgsPointDataNum[CurrentChannelNo][3];
						gSelectedDgsPointDataNum[CurrentChannelNo][2] = gSelectedDgsPointDataNum[CurrentChannelNo][3];

						for(int i = 0; i < 3; i++)
						{
							gDgsSwitch[CurrentChannelNo][i] = TRUE;
						}
						gfOldDacRange[CurrentChannelNo] = DisplaySet_ad[CurrentChannelNo].fRange;

						for(int j = 0; j < gSelectedDgsPointDataNum[CurrentChannelNo][3]; j++)
						{
							gwOldDacSerial[CurrentChannelNo][j] = (short)gSelectedDgsPointData[CurrentChannelNo][3][j].serial;
						}
					}
					else
					{
						for(int k = 0; k < CHANNEL_NUM; k++)
						{
							for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
							{	
								gSelectedDgsPointData[k][3][i].data = gSelectedDgsPointData[CurrentChannelNo][3][i].data;
								gSelectedDgsPointData[k][3][i].serial = gSelectedDgsPointData[CurrentChannelNo][3][i].serial;
								
								gSelectedDgsPointData[k][0][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, gfPingDingGain[CurrentChannelNo]);
								gSelectedDgsPointData[k][0][i].serial = gSelectedDgsPointData[k][3][i].serial;

								gSelectedDgsPointData[k][1][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, gfDingLiangGain[CurrentChannelNo]);
								gSelectedDgsPointData[k][1][i].serial = gSelectedDgsPointData[CurrentChannelNo][3][i].serial;
								
								gSelectedDgsPointData[k][2][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, gfPanHuiGain[CurrentChannelNo]);
								gSelectedDgsPointData[k][2][i].serial = gSelectedDgsPointData[CurrentChannelNo][3][i].serial;

							}

							gSelectedDgsPointDataNum[k][0] = gSelectedDgsPointDataNum[CurrentChannelNo][3];
							gSelectedDgsPointDataNum[k][1] = gSelectedDgsPointDataNum[CurrentChannelNo][3];
							gSelectedDgsPointDataNum[k][2] = gSelectedDgsPointDataNum[CurrentChannelNo][3];
							gSelectedDgsPointDataNum[k][3] = gSelectedDgsPointDataNum[CurrentChannelNo][3];

							for(int i = 0; i < 3; i++)
							{
								gDgsSwitch[k][i] = TRUE;
							}
							gfOldDacRange[k] = DisplaySet_ad[CurrentChannelNo].fRange;

							for(int j = 0; j < gSelectedDgsPointDataNum[CurrentChannelNo][3]; j++)
							{
								gwOldDacSerial[k][j] = (short)gSelectedDgsPointData[CurrentChannelNo][3][j].serial;
							}
						}
					}
					//还原AVG开关状态
					for(int i = 0; i < CHANNEL_NUM; i++)
					{
							gbAvgCurveFlag[i] = bOldAvgCurveFlag[i];
					}
					
					SaveParamFile(lpDefaultParamFile);					
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
	return CallWindowProc(OldBtnCreateDgsCurveEndProc,Wnd,Msg,wParam,lParam);
}
