/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 修改TCG曲线窗口源文件
 * Filename: modifyCurveWin.cpp
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
#include "modifyCurveWin.h"
#include "interfaceCmdID.h"
#include "const.h"
#include "resource.h"
#include "craftParamWin.h"
#include "gateWin.h"
#include "recvParamWin.h"

HWND hWndModifyCurve = NULL;//修改DAC曲线窗口句柄
HWND hBtnModifyCurveAddPoint = NULL;//加样点按钮句柄
HWND hBtnModifyCurveSelPoint = NULL;//选择样点按钮句柄
HWND hBtnModifyCurveModifyAmp = NULL;//修改幅值按钮句柄
HWND hBtnModifyCurveDeletePoint = NULL;//删样点按钮句柄
HWND hBtnModifyCurveEnd = NULL;//结束按钮句柄
HWND ModifyCurveBtnList[5] = {NULL,NULL,NULL, NULL, NULL};//修改曲线窗口的按钮表
WNDPROC OldBtnModifyCurveAddPointProc = NULL;//加样点按钮默认处理过程
WNDPROC OldBtnModifyCurveSelPointProc = NULL;//选择样点按钮默认处理过程
WNDPROC OldBtnModifyCurveModifyAmpProc = NULL;//修改幅值按钮默认处理过程
WNDPROC OldBtnModifyCurveDeletePointProc = NULL;//删样点按钮默认处理过程
WNDPROC OldBtnModifyCurveEndProc = NULL;//结束按钮默认处理过程
HWND currentModifyCurveBtn = NULL;//修改曲线窗中当前选中的按钮

static WORD wModifyAmpStepIndex = 1;		//修改幅值步长索引
static float fModifyAmpStep = 1;			//修改幅值步长

extern CURVEPOINT_DATA gSelectedDacPointData[CHANNEL_NUM][MAX_CURVE_POINT_NUM];//添加的样点数据队列
extern CURVEPOINT_DATA gCurrentSelectedDacPointData;//添加的样点中当前选中的DAC曲线点数据
extern DWORD gSelectedDacPointDataNum[CHANNEL_NUM];//已选中的DAC点数量
extern float gfTcgFirstPointGain[CHANNEL_NUM];		//添加的点第一个点对应的Db值

extern float gfOldTcgRange[CHANNEL_NUM];				//最初生成TCG时的声程
extern short gwOldTcgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//最初生成TCG时的每个点的位置

extern BOOL gWaveDrawTcgFlag;//波形是否画TCG
static const int buttonNum = 5;//按钮控件数

extern HWND hWndMakeCurve;//制作曲线窗口
extern HWND currentMakeCurveBtn;//制作曲线窗口中当前选中的按钮
extern HINSTANCE hInst;//程序实例句柄
extern HWND hWndMain;//主窗口句柄
extern WORD CurrentChannelNo;
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern float fRangeEnd[2];
extern float fRangeStart[2];
extern HWND gCurrentFocusBtn;
extern ARS200FPGA_CH_DAC_REG *chCurrent_dac_reg;
extern HWND gCurrentWnd;		//当前显示的子窗口
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD gwDeleteStep;
extern BOOL gbDeletePointFlag;		//是否在删样点操作
extern PEAK_INFO gPeakDefectInfos[2][2];	//保存闸门峰值点信息
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern HWND hBtnMakeCurveCreate;//创建按钮句柄
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern HWND hStatusBar;
extern wchar_t gszTextStep[10];
extern BOOL gbHotDB;			// 快捷键增益
extern BOOL gbHotRange;			// 快捷键声程

extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG曲线开关标志
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];	//DGS通道开关
static BOOL gbOldAvgCurveFlag[CHANNEL_NUM] = {FALSE, FALSE};		//保存AVG曲线开关标志
static BOOL gOldDgsSwitch[CHANNEL_NUM][GATE_NUM] = {{FALSE,FALSE},{FALSE,FALSE}};				//保存DGS开关状态

static void displayPointAmp(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
		
	if(gSelectedDacPointDataNum[CurrentChannelNo])
	{
		float fSelectPointAmp = gSelectedDacPointData[CurrentChannelNo][gwDeleteStep].data * 100 / MAX_HEIGHT;
		
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

	if(gSelectedDacPointDataNum[CurrentChannelNo])
	{
		gCurrentSelectedDacPointData = gSelectedDacPointData[CurrentChannelNo][gwDeleteStep];
		
		swprintf(strText, _T("%.0f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDacPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDacPointData.data * 100 / MAX_HEIGHT));
	}
	else
	{
		swprintf(strText, _T("无样点"));
		
	}
	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

}

/**************************
*注册修改曲线窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerModifyCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));

	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) ModifyCurveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szModifyCurveChild;

	return RegisterClass(&wc);
}
/**************************
*修改曲线窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK ModifyCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
				gwDeleteStep = 0;
				gbDeletePointFlag = FALSE;
				gWaveDrawTcgFlag = TRUE;

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

				//画按钮控件
				hBtnModifyCurveAddPoint = CreateWindow(_T("BUTTON"),_T("tcgAddPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYCURVE_ADDPOINT,hInst,NULL);

				hBtnModifyCurveSelPoint = CreateWindow(_T("BUTTON"),_T("tcgCurveSelPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYCURVE_SELPOINT,hInst,NULL);

				hBtnModifyCurveModifyAmp = CreateWindow(_T("BUTTON"),_T("tcgCurveModifyAmp"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYCURVE_MODIFYAMP,hInst,NULL);
				
				hBtnModifyCurveDeletePoint = CreateWindow(_T("BUTTON"),_T("tcgDeletePoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYCURVE_DELETEPOINT,hInst,NULL);
				
				hBtnModifyCurveEnd = CreateWindow(_T("BUTTON"),_T("tcgEnd"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYCURVE_END,hInst,NULL);
			
				ModifyCurveBtnList[0] = hBtnModifyCurveAddPoint;
				ModifyCurveBtnList[1] = hBtnModifyCurveSelPoint;
				ModifyCurveBtnList[2] = hBtnModifyCurveModifyAmp;
				ModifyCurveBtnList[3] = hBtnModifyCurveDeletePoint;
				ModifyCurveBtnList[4] = hBtnModifyCurveEnd;
				//按钮控件的自定义处理过程
				OldBtnModifyCurveAddPointProc = (WNDPROC) GetWindowLong(hBtnModifyCurveAddPoint,GWL_WNDPROC);
				SetWindowLong(hBtnModifyCurveAddPoint,GWL_WNDPROC,(LONG)btnModifyCurveAddPointProc);
				
				OldBtnModifyCurveSelPointProc = (WNDPROC) GetWindowLong(hBtnModifyCurveSelPoint,GWL_WNDPROC);
				SetWindowLong(hBtnModifyCurveSelPoint,GWL_WNDPROC,(LONG)btnModifyCurveSelPointProc);

				OldBtnModifyCurveModifyAmpProc = (WNDPROC) GetWindowLong(hBtnModifyCurveModifyAmp,GWL_WNDPROC);
				SetWindowLong(hBtnModifyCurveModifyAmp,GWL_WNDPROC,(LONG)btnModifyCurveModifyAmpProc);

				OldBtnModifyCurveDeletePointProc = (WNDPROC) GetWindowLong(hBtnModifyCurveDeletePoint,GWL_WNDPROC);
				SetWindowLong(hBtnModifyCurveDeletePoint,GWL_WNDPROC,(LONG)btnModifyCurveDeletePointProc);
				
				OldBtnModifyCurveEndProc = (WNDPROC) GetWindowLong(hBtnModifyCurveEnd,GWL_WNDPROC);
				SetWindowLong(hBtnModifyCurveEnd,GWL_WNDPROC,(LONG)btnModifyCurveEndProc);
			
				currentModifyCurveBtn = ModifyCurveBtnList[0];  //默认当前选中加样点参数按钮
				SetFocus(currentModifyCurveBtn);//选中按钮获得输入焦点
				gCurrentFocusBtn = currentModifyCurveBtn;

			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				DestroyWindow(hWndModifyCurve);
				hWndModifyCurve = 0;
				ShowWindow(hWndMakeCurve,SW_SHOW);
				SetFocus(currentMakeCurveBtn);//上级菜单最后选中的按钮获得输入焦点
				gCurrentFocusBtn = currentMakeCurveBtn;

				gCurrentWnd = hWndMakeCurve;

				ReceiveParam_ad[CurrentChannelNo].fPlus = gfTcgFirstPointGain[CurrentChannelNo];
				SetPlusLableAndExecute(NULL, gfTcgFirstPointGain[CurrentChannelNo], ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);
				getGain2TgcSram(gSelectedDacPointData[CurrentChannelNo], gSelectedDacPointDataNum[CurrentChannelNo], CurrentChannelNo);					

				// songchenguang add start 2010-06-13
				// 保存配置到默认文件
				SaveParamFile(lpDefaultParamFile);
				// songchenguang add end 2010-06-13
				
				gWaveDrawTcgFlag = FALSE;
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
			}
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			ModifyCurve_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnModifyCurveSelPoint)
			{
				displaySelPoint(pdis->hwndItem);
			}
			if(pdis->hwndItem == hBtnModifyCurveModifyAmp)
			{
				displayPointAmp(pdis->hwndItem);
			}
			// 检测按钮句柄，显示相应的步长
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentModifyCurveBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnModifyCurveModifyAmp)
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
*加载修改曲线窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int ModifyCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_ModifyCurveBtnBitmap[]=
	{
		{IDC_MODIFYCURVE_ADDPOINT	,IDB_BITMAP_JIAYANGDIAN6_NOSEL_R	 ,  IDB_BITMAP_JIAYANGDIAN6_SELECT_R		},
		{IDC_MODIFYCURVE_SELPOINT	,IDB_BITMAP_XUANZEYANGDIAN_NOSEL_R	 ,  IDB_BITMAP_XUANZEYANGDIAN_SELECT_R		},
		{IDC_MODIFYCURVE_MODIFYAMP	,IDB_BITMAP_FUZHI_NOSEL_R	 ,  		IDB_BITMAP_FUZHI_SELECT_R		},
		{IDC_MODIFYCURVE_DELETEPOINT	,IDB_BITMAP_SHANYANGDIAN6_NOSEL_R	 ,  IDB_BITMAP_SHANYANGDIANG6_SELECT_R		},
		{IDC_MODIFYCURVE_END	,IDB_BITMAP_JIESHU6_NOSEL_R ,  IDB_BITMAP_JIESHU6_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_ModifyCurveBtnBitmap);
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
LRESULT CALLBACK btnModifyCurveAddPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyCurveBtnList[i] != currentModifyCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyCurveBtn = ModifyCurveBtnList[i];
					SetFocus(currentModifyCurveBtn);
					gCurrentFocusBtn = currentModifyCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					while(ModifyCurveBtnList[i] != currentModifyCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyCurveBtn = ModifyCurveBtnList[i];
					SetFocus(currentModifyCurveBtn);
					gCurrentFocusBtn = currentModifyCurveBtn;

					gbDeletePointFlag = TRUE;

					displaySelPoint(hBtnModifyCurveSelPoint);
					displayPointAmp(hBtnModifyCurveModifyAmp);
				}
				else if(wParam == VK_LEFT)
				{
				}
				else if(wParam == VK_RIGHT)
				{
				}
				else if(wParam == VK_RETURN)
				{
					gWaveDrawTcgFlag = FALSE;
					Sleep(100);//等待擦掉波形

					if(gSelectedDacPointDataNum[CurrentChannelNo] < MAX_CURVE_POINT_NUM)
							gSelectedDacPointDataNum[CurrentChannelNo]++;

					if(gSelectedDacPointDataNum[CurrentChannelNo] == 1)
					{
						gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)gPeakDefectInfos[CurrentChannelNo][0].wDefectTime)
																										/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
						gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].serial = curveChangeByDelay(gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].serial, 
																								fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
						
						gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].data = (float)gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp;

						gfTcgFirstPointGain[CurrentChannelNo] = ReceiveParam_ad[CurrentChannelNo].fPlus;
					}
					else
					{
						gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)gPeakDefectInfos[CurrentChannelNo][0].wDefectTime)
																										/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
						gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].serial = curveChangeByDelay(gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].serial, 
																								fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
						
						gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].data = (float)(gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp 
																														* pow(10, (double)(gfTcgFirstPointGain[CurrentChannelNo] - ReceiveParam_ad[CurrentChannelNo].fPlus) / 20));
					}
					
					sortSelectedPointDataBySerial(gSelectedDacPointData[CurrentChannelNo],gSelectedDacPointDataNum[CurrentChannelNo]);

					gWaveDrawTcgFlag = TRUE;

					gfOldTcgRange[CurrentChannelNo] = DisplaySet_ad[CurrentChannelNo].fRange;
							
					for(WORD i = 0; i < gSelectedDacPointDataNum[CurrentChannelNo]; i++)
					{	
						gwOldTcgSerial[CurrentChannelNo][i] = (short)gSelectedDacPointData[CurrentChannelNo][i].serial;
					}
				}
			//	else if(wParam == '\t')
			//	{
			//		if(gAddingFlag == FALSE)
			//			SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnModifyCurveAddPointProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnModifyCurveSelPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyCurveBtnList[i] != currentModifyCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyCurveBtn = ModifyCurveBtnList[i];
					SetFocus(currentModifyCurveBtn);
					gCurrentFocusBtn = currentModifyCurveBtn;

					gbDeletePointFlag = FALSE;
				}
				else if(wParam == VK_DOWN)
				{
					while(ModifyCurveBtnList[i] != currentModifyCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyCurveBtn = ModifyCurveBtnList[i];
					SetFocus(currentModifyCurveBtn);
					gCurrentFocusBtn = currentModifyCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					if(gSelectedDacPointDataNum[CurrentChannelNo])
					{
						
						if(gwDeleteStep == 0)
							gwDeleteStep = (WORD)gSelectedDacPointDataNum[CurrentChannelNo] - 1;
						else
							gwDeleteStep--;
					}
					
					displaySelPoint(Wnd);
					displayPointAmp(hBtnModifyCurveModifyAmp);				
				}
				else if(wParam == VK_RIGHT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					if(gSelectedDacPointDataNum[CurrentChannelNo])
					{
						
						if(gwDeleteStep == gSelectedDacPointDataNum[CurrentChannelNo] - 1)
							gwDeleteStep = 0;
						else
							gwDeleteStep++;
					}
					displaySelPoint(Wnd);
					displayPointAmp(hBtnModifyCurveModifyAmp);
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
	return CallWindowProc(OldBtnModifyCurveSelPointProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnModifyCurveModifyAmpProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyCurveBtnList[i] != currentModifyCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyCurveBtn = ModifyCurveBtnList[i];
					SetFocus(currentModifyCurveBtn);
					gCurrentFocusBtn = currentModifyCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ModifyCurveBtnList[i] != currentModifyCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyCurveBtn = ModifyCurveBtnList[i];
					SetFocus(currentModifyCurveBtn);
					gCurrentFocusBtn = currentModifyCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					gWaveDrawTcgFlag = FALSE;
					Sleep(100);//等待擦掉波形
					float fSelectPointAmp = gSelectedDacPointData[CurrentChannelNo][gwDeleteStep].data * 100 / MAX_HEIGHT;
					if(fSelectPointAmp - fModifyAmpStep >= 0)
					{
						fSelectPointAmp = fSelectPointAmp - fModifyAmpStep;
						gSelectedDacPointData[CurrentChannelNo][gwDeleteStep].data = fSelectPointAmp / 100 * MAX_HEIGHT;
						displayPointAmp(Wnd);
					}
					
					gWaveDrawTcgFlag = TRUE;
					
					
				}
				else if(wParam == VK_RIGHT)
				{
					gWaveDrawTcgFlag = FALSE;
					Sleep(100);//等待擦掉波形
					float fSelectPointAmp = gSelectedDacPointData[CurrentChannelNo][gwDeleteStep].data * 100 / MAX_HEIGHT;
					if(fSelectPointAmp + fModifyAmpStep <= 100)
					{
						fSelectPointAmp = fSelectPointAmp + fModifyAmpStep;
						gSelectedDacPointData[CurrentChannelNo][gwDeleteStep].data = fSelectPointAmp / 100 * MAX_HEIGHT;
						displayPointAmp(Wnd);
					}
					
					gWaveDrawTcgFlag = TRUE;
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
	return CallWindowProc(OldBtnModifyCurveSelPointProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnModifyCurveDeletePointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyCurveBtnList[i] != currentModifyCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyCurveBtn = ModifyCurveBtnList[i];
					SetFocus(currentModifyCurveBtn);
					gCurrentFocusBtn = currentModifyCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ModifyCurveBtnList[i] != currentModifyCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyCurveBtn = ModifyCurveBtnList[i];
					SetFocus(currentModifyCurveBtn);
					gCurrentFocusBtn = currentModifyCurveBtn;

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
					if(gSelectedDacPointDataNum[CurrentChannelNo])
					{
						gWaveDrawTcgFlag = FALSE;
						Sleep(100);//等待擦掉波形
					 	if(gSelectedDacPointDataNum[CurrentChannelNo] == 1)
					 	{
					 		gSelectedDacPointDataNum[CurrentChannelNo]--;
					 	}
						else
						{
							if(gwDeleteStep == (gSelectedDacPointDataNum[CurrentChannelNo] - 1))//删最后一个点
							{
								gSelectedDacPointDataNum[CurrentChannelNo]--;
								gwDeleteStep--;
							}
							else
							{
								for(WORD i = gwDeleteStep; i < (WORD)gSelectedDacPointDataNum[CurrentChannelNo] - 1; i++)
								{
									gSelectedDacPointData[CurrentChannelNo][i] = gSelectedDacPointData[CurrentChannelNo][i + 1];//选中点后面的点数据往前移
								}
								gSelectedDacPointDataNum[CurrentChannelNo]--;
							}
						}
						gWaveDrawTcgFlag = TRUE;
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
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnModifyCurveDeletePointProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnModifyCurveEndProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyCurveBtnList[i] != currentModifyCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyCurveBtn = ModifyCurveBtnList[i];
					SetFocus(currentModifyCurveBtn);
					gCurrentFocusBtn = currentModifyCurveBtn;

					gbDeletePointFlag = TRUE;

					displaySelPoint(hBtnModifyCurveSelPoint);
					displayPointAmp(hBtnModifyCurveModifyAmp);
				}
				
				else if(wParam == VK_DOWN)
				{
					while(ModifyCurveBtnList[i] != currentModifyCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyCurveBtn = ModifyCurveBtnList[i];
					SetFocus(currentModifyCurveBtn);
					gCurrentFocusBtn = currentModifyCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					ReceiveParam_ad[CurrentChannelNo].fPlus = gfTcgFirstPointGain[CurrentChannelNo];
					SetPlusLableAndExecute(Wnd, gfTcgFirstPointGain[CurrentChannelNo], ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);
					getGain2TgcSram(gSelectedDacPointData[CurrentChannelNo], gSelectedDacPointDataNum[CurrentChannelNo], CurrentChannelNo);					

					// songchenguang add start 2010-06-13
					// 保存配置到默认文件
					SaveParamFile(lpDefaultParamFile);
					// songchenguang add end 2010-06-13
					
					gWaveDrawTcgFlag = FALSE;
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
					
					chCurrent_dac_reg->wDACEnable = 1;
					
					DestroyWindow(hWndModifyCurve);
					hWndModifyCurve = 0;
					ShowWindow(hWndMakeCurve,SW_SHOW);
					SetFocus(currentMakeCurveBtn);//上级菜单最后选中的按钮获得输入焦点
					gCurrentFocusBtn = currentMakeCurveBtn;

					gCurrentWnd = hWndMakeCurve;					
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
	return CallWindowProc(OldBtnModifyCurveEndProc,Wnd,Msg,wParam,lParam);
}

