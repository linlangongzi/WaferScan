/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * Fn功能窗口源文件
 * Filename: fnMenuWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnMenuWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "fnSpecialDisplayWin.h"
#include "fnQuantitativeAnalysisWin.h"
#include "fnSpecialFunctionWin.h"
#include "fnVideoRecordWin.h"


HWND hWndFnMenu;						//Fn功能窗口句柄
HWND hBtnFnMenuQuantitativeAnalysis;	//定量分析按钮句柄
HWND hBtnFnMenuSpecialDisplay;			//特殊显示按钮句柄
HWND hBtnFnMenuSpecialFunction;			//专用功能按钮句柄
HWND hBtnFnMenuVideoRecord;				//录像按钮句柄
HWND hBtnFnMenuChangeType;				//切换设备类型按钮句柄

HWND fnMenuBtnList[5];					//Fn功能窗口的按钮表
WNDPROC OldBtnFnMenuQuantitativeAnalysisProc = NULL;//定量分析按钮默认处理过程
WNDPROC OldBtnFnMenuSpecialDisplayProc = NULL;		//特殊显示按钮默认处理过程
WNDPROC OldBtnFnMenuSpecialFunctionProc = NULL;		//专用功能按钮默认处理过程
WNDPROC OldBtnFnMenuVideoRecordProc = NULL;			//录像按钮默认处理过程
WNDPROC OldBtnFnMenuChangeTypeProc = NULL;			//切换设备类型按钮默认处理过程
HWND currentFnMenuBtn;					//Fn功能窗中当前选中的按钮

static WORD buttonNum;					//按钮控件数
static WORD wReducedButtonNum = 0;		//因功能开关减少按扭数量
static BOOL bOldDgsSwitch[CHANNEL_NUM][GATE_NUM] = {{FALSE,FALSE,FALSE},{FALSE,FALSE,FALSE}};	//DGS通道开关
static BOOL bOldAvgCurveFlag[CHANNEL_NUM] = {FALSE, FALSE};		//AVG曲线开关标志

LRESULT CALLBACK btnFnMenuChangeTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

extern HINSTANCE hInst;					//程序实例句柄
extern HWND hWndSpecialDisplay;			//特殊显示窗口句柄
extern HWND hWndQuantitativeAnalysis;	//定量分析窗口句柄
extern HWND hWndVideoRecord;			//录像窗口句柄
extern HWND hWndSpecialFunction;		//专用功能窗口句柄 
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern WORD gwDeviceType;				//设备类型 0:探伤仪	1:测厚仪	
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;			//当前显示的子窗口	

extern WORD gwDeviceType;			//设备类型 0:探伤仪	1:测厚仪
extern HWND hWndChDisplay;			//测厚测量显示窗口句柄
extern HWND currentChDisplayBtn;	// 测厚测量显示窗口当前选中的按钮
extern DWORD gFunctionDefine;		//功能定义
extern HWND hWndTsMenu;				//探伤菜单窗口句柄
extern WORD gwChHeadType;			//探头类型 0:单晶 1:双晶

extern ARS200FPGA_CH_DAC_REG* ch1_dac_reg;
extern ARS200FPGA_CH_DAC_REG* ch2_dac_reg;
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];	//DGS通道开关
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG曲线开关标志
extern HWND hWndTlMenu;//铁路专用菜单窗口句柄
extern HWND currentTlMenuBtn;// 铁路专用菜单窗口当前选中的按钮

extern void chChangeHeadType(WORD wHeadType);
/**************************
*注册Fn功能窗口
* @fnMenu    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerFnMenuChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) fnMenuChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szFnMenuChild;

	return RegisterClass(&wc);
}
/**************************
*Fn功能窗口处理过程
* @fnMenu    hInstance[in]:
* @fnMenu		message[in]:
* @fnMenu		wParam[in]:
* @fnMenu		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK fnMenuChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			if(gwDeviceType == 0 || gwDeviceType == 2) //探伤或铁路
			{
				buttonNum = 5;
				wReducedButtonNum = 0;
				if((gFunctionDefine & DINGLIANGFENXI_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & SPECIALDISPLAY_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & SPECIALFUNCTION_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & RECORD_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) == 0)
					wReducedButtonNum ++;

				buttonNum = buttonNum - wReducedButtonNum;
				
				hBtnFnMenuQuantitativeAnalysis = CreateWindow(_T("BUTTON"),_T("quantitativeAnalysisFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_QUANTITATIVEANALYSIS,hInst,NULL);
				hBtnFnMenuSpecialDisplay = CreateWindow(_T("BUTTON"),_T("specialDisplayFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_SPECIALDISPLAY,hInst,NULL);
				hBtnFnMenuSpecialFunction = CreateWindow(_T("BUTTON"),_T("specialFunctionFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_SPECIALFUNCTION,hInst,NULL);
				hBtnFnMenuChangeType = CreateWindow(_T("BUTTON"),_T("changeTypeFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_CHANGETYPE,hInst,NULL);
				hBtnFnMenuVideoRecord = CreateWindow(_T("BUTTON"),_T("videoRecordFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_VIDEORECORD,hInst,NULL);

				if((gFunctionDefine & DINGLIANGFENXI_SWITCH) != 0)	//dgs曲线功能开
				{
					fnMenuBtnList[0] = hBtnFnMenuQuantitativeAnalysis;
					if((gFunctionDefine & SPECIALDISPLAY_SWITCH) != 0)
					{
						fnMenuBtnList[1] = hBtnFnMenuSpecialDisplay;
						if((gFunctionDefine & SPECIALFUNCTION_SWITCH) != 0)
						{
							fnMenuBtnList[2] = hBtnFnMenuSpecialFunction;
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[3] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[4] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[3] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[2] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[3] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
						}
					}
					else 
					{
						if((gFunctionDefine & SPECIALFUNCTION_SWITCH) != 0)
						{
							fnMenuBtnList[1] = hBtnFnMenuSpecialFunction;
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[2] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[3] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[1] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
						}
					}
				}
				else
				{
					if((gFunctionDefine & SPECIALDISPLAY_SWITCH) != 0)
					{
						fnMenuBtnList[0] = hBtnFnMenuSpecialDisplay;
						if((gFunctionDefine & SPECIALFUNCTION_SWITCH) != 0)
						{
							fnMenuBtnList[1] = hBtnFnMenuSpecialFunction;
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[2] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[3] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[1] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
						}
					}
					else 
					{
						if((gFunctionDefine & SPECIALFUNCTION_SWITCH) != 0)
						{
							fnMenuBtnList[0] = hBtnFnMenuSpecialFunction;
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[1] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[0] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[0] = hBtnFnMenuVideoRecord;
							}
						}
					}
				}
			}
			
			else if(gwDeviceType == 1)	//测厚
			{
				buttonNum = 4;
				wReducedButtonNum = 0;
				if((gFunctionDefine & SPECIALDISPLAY_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & SPECIALFUNCTION_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & RECORD_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) == 0)
					wReducedButtonNum ++;

				buttonNum = buttonNum - wReducedButtonNum;
				
				hBtnFnMenuSpecialDisplay = CreateWindow(_T("BUTTON"),_T("specialDisplayFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_SPECIALDISPLAY,hInst,NULL);
				hBtnFnMenuSpecialFunction = CreateWindow(_T("BUTTON"),_T("specialFunctionFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 1*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_SPECIALFUNCTION,hInst,NULL);
				hBtnFnMenuChangeType = CreateWindow(_T("BUTTON"),_T("changeTypeFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_CHANGETYPE,hInst,NULL);
				hBtnFnMenuVideoRecord = CreateWindow(_T("BUTTON"),_T("videoRecordFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_VIDEORECORD,hInst,NULL);
			
				if((gFunctionDefine & SPECIALDISPLAY_SWITCH) != 0)	//dgs曲线功能开
				{
					fnMenuBtnList[0] = hBtnFnMenuSpecialDisplay;
					if((gFunctionDefine & SPECIALFUNCTION_SWITCH) != 0)
					{
						fnMenuBtnList[1] = hBtnFnMenuSpecialFunction;
						if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
						{
							fnMenuBtnList[2] = hBtnFnMenuChangeType;
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[3] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
						}
					}
					else 
					{
						if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
						{
							fnMenuBtnList[1] = hBtnFnMenuChangeType;
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
						}					
					}
				}
				else
				{
					if((gFunctionDefine & SPECIALFUNCTION_SWITCH) != 0)
					{
						fnMenuBtnList[0] = hBtnFnMenuSpecialFunction;
						if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
						{
							fnMenuBtnList[1] = hBtnFnMenuChangeType;
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
						}
					}
					else 
					{
						if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
						{
							fnMenuBtnList[0] = hBtnFnMenuChangeType;
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[0] = hBtnFnMenuVideoRecord;
							}
						}					
					}
				}
			}
			
			//按钮控件的自定义处理过程
			OldBtnFnMenuQuantitativeAnalysisProc = (WNDPROC) GetWindowLong(hBtnFnMenuQuantitativeAnalysis,GWL_WNDPROC);
			SetWindowLong(hBtnFnMenuQuantitativeAnalysis,GWL_WNDPROC,(LONG)btnFnMenuQuantitativeAnalysisProc);
			
			OldBtnFnMenuSpecialDisplayProc = (WNDPROC) GetWindowLong(hBtnFnMenuSpecialDisplay,GWL_WNDPROC);
			SetWindowLong(hBtnFnMenuSpecialDisplay,GWL_WNDPROC,(LONG)btnFnMenuSpecialDisplayProc);
			
			OldBtnFnMenuSpecialFunctionProc = (WNDPROC) GetWindowLong(hBtnFnMenuSpecialFunction,GWL_WNDPROC);
			SetWindowLong(hBtnFnMenuSpecialFunction,GWL_WNDPROC,(LONG)btnFnMenuSpecialFunctionProc);
			
			OldBtnFnMenuVideoRecordProc = (WNDPROC) GetWindowLong(hBtnFnMenuVideoRecord,GWL_WNDPROC);
			SetWindowLong(hBtnFnMenuVideoRecord,GWL_WNDPROC,(LONG)btnFnMenuVideoRecordProc);

			OldBtnFnMenuChangeTypeProc = (WNDPROC) GetWindowLong(hBtnFnMenuChangeType,GWL_WNDPROC);
			SetWindowLong(hBtnFnMenuChangeType,GWL_WNDPROC,(LONG)btnFnMenuChangeTypeProc);
		
			currentFnMenuBtn = fnMenuBtnList[0];//默认当前选中定量分析按钮
			if(currentFnMenuBtn)
			{
				SetFocus(currentFnMenuBtn);//选中按钮获得输入焦点
				gCurrentFocusBtn = currentFnMenuBtn;
			}
			else			//功能都关闭按钮的父窗口获得输入焦点
				SetFocus(hWnd);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				if(gwDeviceType == 0) //探伤
				{
					DestroyWindow(hWndFnMenu);
					hWndFnMenu = 0;
				
					SetMenuStateFlag(FALSE);
					SetFocus(hWndMain);//主窗口获得输入焦点
				
					gCurrentWnd = 0;
				}
				else if(gwDeviceType == 1)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);
					gCurrentWnd = hWndChDisplay;

					DestroyWindow(hWndFnMenu);
					hWndFnMenu = 0;

					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentTlMenuBtn;	//2011-9-1 wangbingfu add at 2011-9-1
				}
				else if(gwDeviceType == 2)	//铁路
				{
					ShowWindow(hWndTlMenu,SW_SHOW);
					gCurrentWnd = hWndTlMenu;

					DestroyWindow(hWndFnMenu);
					hWndFnMenu = 0;

					SetFocus(currentTlMenuBtn);
					gCurrentFocusBtn = currentTlMenuBtn;	//2011-9-1 wangbingfu add at 2011-9-1
				}
			}
			break;
		case WM_COMMAND:
				if(LOWORD(wParam) == IDC_FNMENU_QUANTITATIVEANALYSIS)
				{
					currentFnMenuBtn = fnMenuBtnList[0]; 
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
					hWndQuantitativeAnalysis = CreateWindow(szQuantitativeAnalysisChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);

					ShowWindow(hWndFnMenu,SW_HIDE);
					//SetFocus(hWndQuantitativeAnalysis);
					ShowWindow(hWndQuantitativeAnalysis,SW_SHOW);
					UpdateWindow(hWndQuantitativeAnalysis);
				}
				else if(LOWORD(wParam) == IDC_FNMENU_SPECIALDISPLAY)
				{
					currentFnMenuBtn = fnMenuBtnList[1]; 
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
					hWndSpecialDisplay = CreateWindow(szSpecialDisplayChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFnMenu,SW_HIDE);
					//SetFocus(hWndSpecialDisplay);
					ShowWindow(hWndSpecialDisplay,SW_SHOW);
					UpdateWindow(hWndSpecialDisplay);
				}
				else if(LOWORD(wParam) == IDC_FNMENU_SPECIALFUNCTION)
				{
					currentFnMenuBtn = fnMenuBtnList[2]; 
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
					hWndSpecialFunction = CreateWindow(szSpecialFunctionChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFnMenu,SW_HIDE);
					//SetFocus(hWndSpecialFunction);
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					UpdateWindow(hWndSpecialFunction);
				}
				else if(LOWORD(wParam) == IDC_FNMENU_VIDEORECORD)
				{
				//	currentFnMenuBtn = fnMenuBtnList[3]; 
				//	SetFocus(currentFnMenuBtn);
				//	gCurrentFocusBtn = currentFnMenuBtn;
				}
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			fnMenu_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_CLOSE:
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载Fn功能窗口按钮图片
* @fnMenu    hInstance[in]:
* @fnMenu		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int fnMenu_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	if(gwDeviceType == 0 || gwDeviceType == 2) //探伤或铁路
	{
		BTN_BMP_INFO tbl_fnMenuBtnBitmap[]=
		{
			{IDC_FNMENU_QUANTITATIVEANALYSIS	,IDB_BITMAP_DINGLIANGFENXI_NOSEL_R	 ,  IDB_BITMAP_DINGLIANGFENXI_SELECT_R		},
			{IDC_FNMENU_SPECIALDISPLAY	,IDB_BITMAP_TESHUXIANSHI_NOSEL_R	 ,  IDB_BITMAP_TESHUXIANSHI_SELECT_R		},
			{IDC_FNMENU_SPECIALFUNCTION	,IDB_BITMAP_ZHUANYONGGONGNENG_NOSEL_R ,  IDB_BITMAP_ZHUANYONGGONGNENG_SELECT_R	},
			{IDC_FNMENU_CHANGETYPE	,IDB_BITMAP_CEHOUMOSHI_NOSEL_R ,  IDB_BITMAP_CEHOUMOSHI_SELECT_R	},
			{IDC_FNMENU_VIDEORECORD	,IDB_BITMAP_LUXIANG_NOSEL_R ,  IDB_BITMAP_LUXIANG_SELECT_R	},
			{	-1			,	-1					 ,	-1							}    
		};
		if((gFunctionDefine & DINGLIANGFENXI_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[0].nBtnNoFocID = IDB_BITMAP_DINGLIANGFENXI_DISABLE_R;
		if((gFunctionDefine & SPECIALDISPLAY_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[1].nBtnNoFocID = IDB_BITMAP_TESHUXIANSHI_DISABLE_R;
		if((gFunctionDefine & SPECIALFUNCTION_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[2].nBtnNoFocID = IDB_BITMAP_ZHUANYONGGONGNENG_DISABLE_R;
		if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[3].nBtnNoFocID = IDB_BITMAP_CEHOUMOSHI_DISABLE_R;
		if((gFunctionDefine & RECORD_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[4].nBtnNoFocID = IDB_BITMAP_LUXIANG_DISABLE_R;
		
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_fnMenuBtnBitmap);
	}
	else if(gwDeviceType == 1)	//测厚
	{
		BTN_BMP_INFO tbl_fnMenuBtnBitmap[]=
		{
			{IDC_FNMENU_SPECIALDISPLAY	,IDB_BITMAP_TESHUXIANSHI_NOSEL_R	 ,  IDB_BITMAP_TESHUXIANSHI_SELECT_R		},
			{IDC_FNMENU_SPECIALFUNCTION	,IDB_BITMAP_ZHUANYONGGONGNENG_NOSEL_R ,  IDB_BITMAP_ZHUANYONGGONGNENG_SELECT_R	},
			{IDC_FNMENU_CHANGETYPE	,IDB_BITMAP_TANSHANGMOSHI_NOSEL_R ,  IDB_BITMAP_TANSHANGMOSHI_SELECT_R	},
			{IDC_FNMENU_VIDEORECORD	,IDB_BITMAP_LUXIANG_NOSEL_R ,  IDB_BITMAP_LUXIANG_SELECT_R	},			
			{	-1			,	-1					 ,	-1							}    
		};
		if((gFunctionDefine & SPECIALDISPLAY_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[0].nBtnNoFocID = IDB_BITMAP_TESHUXIANSHI_DISABLE_R;
		if((gFunctionDefine & SPECIALFUNCTION_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[1].nBtnNoFocID = IDB_BITMAP_ZHUANYONGGONGNENG_DISABLE_R;
		if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[2].nBtnNoFocID = IDB_BITMAP_TANSHANGMOSHI_DISABLE_R;
		if((gFunctionDefine & RECORD_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[3].nBtnNoFocID = IDB_BITMAP_LUXIANG_DISABLE_R;
		
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_fnMenuBtnBitmap);
	}
	
	return ReturnVal;
}
/**************************
*定量分析按钮自定义处理过程
* @fnMenu    hInstance[in]:
* @fnMenu		message[in]:
* @fnMenu		wParam[in]:
* @fnMenu		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFnMenuQuantitativeAnalysisProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndQuantitativeAnalysis = CreateWindow(szQuantitativeAnalysisChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFnMenu,SW_HIDE);
				//	SetFocus(hWndQuantitativeAnalysis);
					ShowWindow(hWndQuantitativeAnalysis,SW_SHOW);
					UpdateWindow(hWndQuantitativeAnalysis);

					gCurrentWnd = hWndQuantitativeAnalysis;
					
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
	return CallWindowProc(OldBtnFnMenuQuantitativeAnalysisProc,Wnd,Msg,wParam,lParam);
}
/**************************
*特殊显示按钮自定义处理过程
* @fnMenu    hInstance[in]:
* @fnMenu		message[in]:
* @fnMenu		wParam[in]:
* @fnMenu		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFnMenuSpecialDisplayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndSpecialDisplay = CreateWindow(szSpecialDisplayChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFnMenu,SW_HIDE);
					//SetFocus(hWndSpecialDisplay);
					ShowWindow(hWndSpecialDisplay,SW_SHOW);
					UpdateWindow(hWndSpecialDisplay);

					gCurrentWnd = hWndSpecialDisplay;
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
	return CallWindowProc(OldBtnFnMenuSpecialDisplayProc,Wnd,Msg,wParam,lParam);
}
/**************************
*专用功能按钮自定义处理过程
* @fnMenu    hInstance[in]:
* @fnMenu		message[in]:
* @fnMenu		wParam[in]:
* @fnMenu		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFnMenuSpecialFunctionProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndSpecialFunction = CreateWindow(szSpecialFunctionChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFnMenu,SW_HIDE);
					//SetFocus(hWndSpecialFunction);
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					UpdateWindow(hWndSpecialFunction);

					gCurrentWnd = hWndSpecialFunction;
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
	return CallWindowProc(OldBtnFnMenuSpecialFunctionProc,Wnd,Msg,wParam,lParam);
}
/**************************
*录像按钮自定义处理过程
* @fnMenu    hInstance[in]:
* @fnMenu		message[in]:
* @fnMenu		wParam[in]:
* @fnMenu		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFnMenuVideoRecordProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndVideoRecord= CreateWindow(szVideoRecordChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFnMenu,SW_HIDE);
					//SetFocus(hWndVideoRecord);
					ShowWindow(hWndVideoRecord,SW_SHOW);
					UpdateWindow(hWndVideoRecord);

					gCurrentWnd = hWndVideoRecord;
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
	return CallWindowProc(OldBtnFnMenuVideoRecordProc,Wnd,Msg,wParam,lParam);
}
/**************************
*设备转换按钮自定义处理过程
* @fnMenu    hInstance[in]:
* @fnMenu		message[in]:
* @fnMenu		wParam[in]:
* @fnMenu		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFnMenuChangeTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					if(gwDeviceType == 0)	//探伤
					{
						gwDeviceType = 1;

					//	if(hWndChDisplay == 0)
					//	{
							DestroyWindow(hWndFnMenu);
							hWndFnMenu = 0;

							hWndChDisplay = CreateWindow(szChDisplayChild, TEXT(""), WS_CHILD,
										 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);//创建测厚测量显示窗口
							ShowWindow(hWndChDisplay,SW_SHOW);
							UpdateWindow(hWndChDisplay);

							gCurrentWnd = hWndChDisplay;
					//	}
						chChangeHeadType(gwChHeadType); //切换测厚的探头类型

						ch1_dac_reg->wDACEnable = 0; //关闭TCG
						//保存开关DAC状态
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							for(int j = 0; j < GATE_NUM; j++)
							{
								bOldDgsSwitch[i][j] = gDgsSwitch[i][j];
							}
						}
						//保存开关AVG状态
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							bOldAvgCurveFlag[i] = gbAvgCurveFlag[i];
						}
						//关闭DAC
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							for(int j = 0; j < GATE_NUM; j++)
							{
								gDgsSwitch[i][j] = FALSE;
							}
						}
						//关闭AVG
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							gbAvgCurveFlag[i] = OFF;
						}
					}
					else if(gwDeviceType == 1)	//测厚
					{
						gwDeviceType = 0;

						DestroyWindow(hWndFnMenu);
						hWndFnMenu = 0;
						
						DestroyWindow(hWndChDisplay);
						hWndChDisplay = 0;

						hWndTsMenu = CreateWindow(szTsMenuChild, TEXT(""), WS_CHILD,
									 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);//创建探伤菜单窗口
						ShowWindow(hWndTsMenu,SW_SHOW);
						UpdateWindow(hWndTsMenu);

						gCurrentWnd = hWndTsMenu;

						//还原开关DAC状态
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							for(int j = 0; j < GATE_NUM; j++)
							{
								gDgsSwitch[i][j] = bOldDgsSwitch[i][j];
							}
						}
						//还原开关AVG状态
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							gbAvgCurveFlag[i] = bOldAvgCurveFlag[i];
						}
							
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
	return CallWindowProc(OldBtnFnMenuVideoRecordProc,Wnd,Msg,wParam,lParam);
}
