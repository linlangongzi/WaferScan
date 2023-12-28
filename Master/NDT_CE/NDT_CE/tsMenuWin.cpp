/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 探伤菜单窗口源文件
 * Filename: tsMenuWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "tsMenuWin.h"

#include "resource.h"
#include "interfaceCmdID.h"

#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"

HWND hWndTsMenu;//探伤菜单窗口句柄
static HWND hBtnMode;//模式按钮句柄
static HWND hBtnParam;//参数按钮句柄
static HWND hBtnOtherCfg;//通信设置按钮句柄
static HWND hBtnFile;//文件设置按钮句柄
static HWND hBtnHotkey;//快捷键按钮句柄

static WNDPROC OldBtnModeProc = NULL;//模式按钮默认处理过程
static WNDPROC OldBtnParamProc = NULL;//参数按钮默认处理过程
static WNDPROC OldBtnOtherCfgProc = NULL;//通信设置按钮默认处理过程
static WNDPROC OldBtnFileProc = NULL;//文件设置按钮默认处理过程
static WNDPROC OldBtnHotkeyProc = NULL;//快捷键按钮默认处理过程
static HWND tsMenuBtnList[5];//探伤菜单窗口的按钮表
HWND currentTsMenuBtn;//探伤菜单窗口中当前选中的按钮

static const int buttonNum = 5;//按钮控件数

extern HWND hWndMode;//模式窗口句柄
extern HWND hWndParam;//参数窗口句柄
extern HINSTANCE hInst;//程序实例句柄
extern HWND hWndMain;//主窗口句柄
extern HWND hWndHotKey;//快捷键窗口句柄
extern HWND hWndFileManage;//文件管理窗口句柄
extern HWND hWndOtherCfg;//其它配置窗口句柄
extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//当前显示的子窗口

extern HWND hWndTlAdjust;	//调校窗口句柄
extern HWND currentTlAdjustBtn;	//调校窗口当前选中按钮

// songchenguang add start 2011-02-09
extern HWND hWndTlMenu;	//铁路专用菜单窗口句柄
extern HWND currentTlMenuBtn;  	// 铁路专用菜单窗口当前选中的按钮

// songchenguang add end 2011-02-09

extern WORD gwDeviceType;	//设备类型 0:探伤仪	1:测厚仪 2:铁路专用
extern WORD CurrentChannelNo;// 当前通道号

extern float gfTsOldRange[CHANNEL_NUM];		//保存通用探伤的声程
extern float gfTsOldPlus[CHANNEL_NUM];		//保存通用探伤的基准增益
extern float gfTsOldRefPlus[CHANNEL_NUM];	//保存通用探伤的参考增益
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];

//extern void setRailRoadParam(WORD wChannelNo, BOOL bCurveChangeFlag);//wangbingfu delete at 2011-5-27
/**************************
*注册设探伤菜单窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerTsMenuChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = (WNDPROC) tsMenuChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szTsMenuChild;

	return RegisterClass(&wc);
}
/**************************
*探伤菜单窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK tsMenuChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	switch (message) 
	{
		case WM_CREATE:
			hBtnMode = CreateWindow(_T("BUTTON"),_T("mode"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TSMENU_MODE,hInst,NULL);
			hBtnParam = CreateWindow(_T("BUTTON"),_T("param"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TSMENU_PARAM,hInst,NULL);
			hBtnOtherCfg = CreateWindow(_T("BUTTON"),_T("othercfg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TSMENU_OTHERCFG,hInst,NULL);
			hBtnFile = CreateWindow(_T("BUTTON"),_T("fileMange"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TSMENU_FILE,hInst,NULL);
			hBtnHotkey = CreateWindow(_T("BUTTON"),_T("hotkey"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TSMENU_HOTKEY,hInst,NULL);
			tsMenuBtnList[0] = hBtnMode;
			tsMenuBtnList[1] = hBtnParam;
			tsMenuBtnList[2] = hBtnOtherCfg;
			tsMenuBtnList[3] = hBtnFile;
			tsMenuBtnList[4] = hBtnHotkey;
			//按钮控件的自定义处理过程
			OldBtnModeProc = (WNDPROC) GetWindowLong(hBtnMode,GWL_WNDPROC);
			SetWindowLong(hBtnMode,GWL_WNDPROC,(LONG)btnModeProc);
			OldBtnParamProc = (WNDPROC) GetWindowLong(hBtnParam,GWL_WNDPROC);
			SetWindowLong(hBtnParam,GWL_WNDPROC,(LONG)btnParamProc);
			OldBtnOtherCfgProc = (WNDPROC) GetWindowLong(hBtnOtherCfg,GWL_WNDPROC);
		    SetWindowLong(hBtnOtherCfg,GWL_WNDPROC,(LONG)btnOtherCfgProc);
			OldBtnFileProc = (WNDPROC) GetWindowLong(hBtnFile,GWL_WNDPROC);
			SetWindowLong(hBtnFile,GWL_WNDPROC,(LONG)btnFileProc);
			OldBtnHotkeyProc = (WNDPROC) GetWindowLong(hBtnHotkey,GWL_WNDPROC);
			SetWindowLong(hBtnHotkey,GWL_WNDPROC,(LONG)btnHotkeyProc);
			
			currentTsMenuBtn = tsMenuBtnList[0];//默认当前选中模式按钮
			SetFocus(currentTsMenuBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentTsMenuBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndTsMenu, &ps);
			// TODO: Add any drawing code here...
		//	RECT rt;
		//	GetClientRect(hWnd, &rt);
		//	DrawText(hdc, szHello, _tcslen(szHello), &rt, DT_CENTER);
			//画按钮控件
			
			
			EndPaint(hWndTsMenu, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				if(!bFreezeUp)
				{
					DestroyWindow(hWndTsMenu);
					hWndTsMenu = 0;
					
					if(gwDeviceType == 2)	//铁路专用
					{
						if(hWndTlAdjust != NULL)
						{
							ShowWindow(hWndTlAdjust,SW_SHOW);
							SetFocus(currentTlAdjustBtn);//主窗口获得输入焦点
							
							gCurrentFocusBtn = currentTlAdjustBtn;
							gCurrentWnd = hWndTlAdjust;
						}
						else
						{
							ShowWindow(hWndTlMenu,SW_SHOW);
							SetFocus(currentTlMenuBtn);//主窗口获得输入焦点
							
							gCurrentFocusBtn = currentTlMenuBtn;
							gCurrentWnd = hWndTlMenu;
						}
						/*	//wangbingfu delete at 2011-5-27
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							gfTsOldRange[i] = DisplaySet_ad[i].fRange;
							gfTsOldPlus[i] = ReceiveParam_ad[i].fPlus;
							gfTsOldRefPlus[i] = ReceiveParam_ad[i].fRefPlus;
						}
						setRailRoadParam(CurrentChannelNo, TRUE);//设置该标准该部位声程，基准增益，补偿增益
						*/
					}
					else
					{
						// songchenguang add start 2010-4-26
						SetMenuStateFlag(FALSE);
						// songchenguang add end 2010-4-26
						gCurrentFocusBtn = 0;//没有任何按钮有焦点

						gCurrentWnd = 0; //没有子菜单窗口
						SetFocus(hWndMain);//主窗口获得输入焦点
					}
				}
			}
			break;
		case WM_DRAWITEM://重画按钮
			pdis=(LPDRAWITEMSTRUCT)lParam;
			tsMenu_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_COMMAND:
				if(LOWORD(wParam) == IDC_TSMENU_MODE)
				{
					currentTsMenuBtn = tsMenuBtnList[0]; 
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
					hWndMode = CreateWindow(szModeChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndMode);
					ShowWindow(hWndMode,SW_SHOW);
					UpdateWindow(hWndMode);
				}
				else if(LOWORD(wParam) == IDC_TSMENU_PARAM)
				{
					currentTsMenuBtn = tsMenuBtnList[1]; 
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
					hWndParam = CreateWindow(szParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndParam);
					ShowWindow(hWndParam,SW_SHOW);
					UpdateWindow(hWndParam);
				}
				else if(LOWORD(wParam) == IDC_TSMENU_OTHERCFG)
				{
					currentTsMenuBtn = tsMenuBtnList[2]; 
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
					hWndOtherCfg = CreateWindow(szOtherCfgChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndOtherCfg);
					ShowWindow(hWndOtherCfg,SW_SHOW);
					UpdateWindow(hWndOtherCfg);
				}
				else if(LOWORD(wParam) == IDC_TSMENU_FILE)
				{
					currentTsMenuBtn = tsMenuBtnList[3]; 
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
					hWndFileManage = CreateWindow(szFileManageChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndFileManage);
					ShowWindow(hWndFileManage,SW_SHOW);
					UpdateWindow(hWndFileManage);
				}
				else if(LOWORD(wParam) == IDC_TSMENU_HOTKEY)
				{
					currentTsMenuBtn = tsMenuBtnList[4]; 
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
					hWndHotKey = CreateWindow(szHotKeyChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndHotKey);
					ShowWindow(hWndHotKey,SW_SHOW);
					UpdateWindow(hWndHotKey);
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
*加载探伤菜单窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int tsMenu_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_smBtnBitmap[]=
	{
		{IDC_TSMENU_MODE	,IDB_BITMAP_MODE_NOSEL_R	 ,  IDB_BITMAP_MODE_SELECT_R		},
		{IDC_TSMENU_PARAM	,IDB_BITMAP_PARAMETER_NOSEL_R	 ,  IDB_BITMAP_PARAMETER_SELECT_R		},
		{IDC_TSMENU_OTHERCFG	,IDB_BITMAP_OTHERCONFIGURE_NOSEL_R ,  IDB_BITMAP_OTHERCONFIGURE_SELECT_R	},
		{IDC_TSMENU_FILE	,IDB_BITMAP_FILE_NOSEL_R	 ,  IDB_BITMAP_FILE_SELECT_R		},
		{IDC_TSMENU_HOTKEY	,IDB_BITMAP_HOTKEY_NOSEL_R ,  IDB_BITMAP_HOTKEY_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_smBtnBitmap);
	return ReturnVal;
}
/**************************
*模式按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndMode = CreateWindow(szModeChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndMode);
					ShowWindow(hWndMode,SW_SHOW);
					UpdateWindow(hWndMode);

					gCurrentWnd = hWndMode;
				//	SetWindowPos(hWndMode,hWndTsMenu,0,0,120,450,
				//	SWP_NOMOVE|SWP_SHOWWINDOW);
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
	return CallWindowProc(OldBtnModeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*参数按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnParamProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndParam = CreateWindow(szParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndParam);
					ShowWindow(hWndParam,SW_SHOW);
					UpdateWindow(hWndParam);

					gCurrentWnd = hWndParam;
				//	SetWindowPos(hWndParam,HWND_TOPMOST,0,0,120,450,
				//	SWP_NOMOVE|SWP_SHOWWINDOW);
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
	return CallWindowProc(OldBtnParamProc,Wnd,Msg,wParam,lParam);
}
/**************************
*通信设置按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnOtherCfgProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndOtherCfg = CreateWindow(szOtherCfgChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndOtherCfg);
					ShowWindow(hWndOtherCfg,SW_SHOW);
					UpdateWindow(hWndOtherCfg);

					gCurrentWnd = hWndOtherCfg;
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
	return CallWindowProc(OldBtnOtherCfgProc,Wnd,Msg,wParam,lParam);
}
/**************************
*文件设置按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFileProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndFileManage = CreateWindow(szFileManageChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndFileManage);
					ShowWindow(hWndFileManage,SW_SHOW);
					UpdateWindow(hWndFileManage);

					gCurrentWnd = hWndFileManage;
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
	return CallWindowProc(OldBtnFileProc,Wnd,Msg,wParam,lParam);
}
/**************************
*快捷键按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHotkeyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam ==VK_DOWN)
				{
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndHotKey = CreateWindow(szHotKeyChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndHotKey);
					ShowWindow(hWndHotKey,SW_SHOW);
					UpdateWindow(hWndHotKey);

					gCurrentWnd = hWndHotKey;
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
	return CallWindowProc(OldBtnHotkeyProc,Wnd,Msg,wParam,lParam);
}
