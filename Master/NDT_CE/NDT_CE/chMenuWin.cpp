/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 测厚菜单窗口源文件
 * Filename: chMenuWin.cpp
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

HWND hWndChMenu = NULL;//测厚菜单窗口句柄
HWND currentChMenuBtn = NULL;// 测厚菜单窗口当前选中的按钮

static HWND hBtnMeasureCfg = NULL;//测量设置按钮句柄
static HWND hBtnParam = NULL;//参数按钮句柄
static HWND hBtnAdjust = NULL;//校准按钮句柄
static HWND hBtnOtherCfg = NULL;//通信设置按钮句柄
static HWND hBtnFile = NULL;//文件设置按钮句柄
static HWND hBtnHotkey = NULL;//快捷键按钮句柄
static HWND chMenuBtnList[6] = {NULL,NULL,NULL,NULL,NULL,NULL};//测厚菜单窗口的按钮表
static const int buttonNum = 6;//按钮控件数

static WNDPROC OldBtnMeasureCfgProc = NULL;//测量设置按钮默认处理过程
static WNDPROC OldBtnParamProc = NULL;//参数按钮默认处理过程
static WNDPROC OldBtnAdjustProc = NULL;// 校准按钮默认处理过程
static WNDPROC OldBtnOtherCfgProc = NULL;//通信设置按钮默认处理过程
static WNDPROC OldBtnFileProc = NULL;//文件设置按钮默认处理过程
static WNDPROC OldBtnHotkeyProc = NULL;//快捷键按钮默认处理过程

ATOM registerChMenuChild(HINSTANCE hInstance);
static LRESULT CALLBACK chMenuChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int chMenu_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnMeasureCfgProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnParamProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAdjustProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnOtherCfgProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnHotkeyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

extern HINSTANCE hInst;			//程序实例句柄
extern HWND hWndMain;			//主窗口句柄
extern HWND gCurrentFocusBtn;
extern HWND hWndChMeasureCfg;	//测量设置窗口句柄
extern HWND hWndChParam;		//测厚参数窗口句柄
extern HWND hWndChAdjust;		//测厚校准窗口句柄
extern HWND hWndHotKey;			//快捷键窗口句柄
extern HWND hWndFileManage;		//文件管理窗口句柄
extern HWND hWndOtherCfg;		//其它配置窗口句柄

extern HWND hWndChDisplay;		//测厚测量显示窗口句柄
extern HWND gCurrentWnd;
extern HWND currentChDisplayBtn;
/**************************
*注册测厚菜单窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChMenuChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = (WNDPROC)chMenuChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChMenuChild;

	return RegisterClass(&wc);
}

/**************************
*测量设置按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnMeasureCfgProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);

					gCurrentFocusBtn = currentChDisplayBtn;
					SetFocus(gCurrentFocusBtn);

					DestroyWindow(hWndChMenu);
					hWndChMenu = 0;	

					gCurrentWnd =  hWndChDisplay;					
				}
			
				else if(wParam == VK_UP)
				{
					while(chMenuBtnList[i] != currentChMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMenuBtn = chMenuBtnList[i];
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMenuBtnList[i] != currentChMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMenuBtn = chMenuBtnList[i];
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChMeasureCfg = CreateWindow(szChMeasureCfgChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMenu,SW_HIDE);
					//SetFocus(hWndChMeasureCfg);
					ShowWindow(hWndChMeasureCfg,SW_SHOW);
					UpdateWindow(hWndChMeasureCfg);

					gCurrentWnd = hWndChMeasureCfg;
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
	return CallWindowProc(OldBtnMeasureCfgProc,Wnd,Msg,wParam,lParam);
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
static LRESULT CALLBACK btnParamProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);

					gCurrentFocusBtn = currentChDisplayBtn;
					SetFocus(gCurrentFocusBtn);

					DestroyWindow(hWndChMenu);
					hWndChMenu = 0;

					gCurrentWnd =  hWndChDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(chMenuBtnList[i] != currentChMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMenuBtn = chMenuBtnList[i];
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMenuBtnList[i] != currentChMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMenuBtn = chMenuBtnList[i];
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChParam = CreateWindow(szChParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMenu,SW_HIDE);
					//SetFocus(hWndChParam);
					ShowWindow(hWndChParam,SW_SHOW);
					UpdateWindow(hWndChParam);

					gCurrentWnd = hWndChParam;
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
*校准按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAdjustProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);

					gCurrentFocusBtn = currentChDisplayBtn;
					SetFocus(gCurrentFocusBtn);

					DestroyWindow(hWndChMenu);
					hWndChMenu = 0;

					gCurrentWnd =  hWndChDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(chMenuBtnList[i] != currentChMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMenuBtn = chMenuBtnList[i];
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMenuBtnList[i] != currentChMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMenuBtn = chMenuBtnList[i];
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChAdjust = CreateWindow(szChAdjustChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMenu,SW_HIDE);
					//SetFocus(hWndParam);
					ShowWindow(hWndChAdjust,SW_SHOW);
					UpdateWindow(hWndChAdjust);

					gCurrentWnd = hWndChAdjust;
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
	return CallWindowProc(OldBtnAdjustProc,Wnd,Msg,wParam,lParam);
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
static LRESULT CALLBACK btnOtherCfgProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);

					gCurrentFocusBtn = currentChDisplayBtn;
					SetFocus(gCurrentFocusBtn);

					DestroyWindow(hWndChMenu);
					hWndChMenu = 0;

					gCurrentWnd =  hWndChDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(chMenuBtnList[i] != currentChMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMenuBtn = chMenuBtnList[i];
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMenuBtnList[i] != currentChMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMenuBtn = chMenuBtnList[i];
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndOtherCfg = CreateWindow(szOtherCfgChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMenu,SW_HIDE);
					//SetFocus(hWndOtherCfg);
					ShowWindow(hWndOtherCfg,SW_SHOW);
					UpdateWindow(hWndOtherCfg);

					gCurrentWnd = hWndOtherCfg;
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
static LRESULT CALLBACK btnFileProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);

					gCurrentFocusBtn = currentChDisplayBtn;
					SetFocus(gCurrentFocusBtn);

					DestroyWindow(hWndChMenu);
					hWndChMenu = 0;

					gCurrentWnd =  hWndChDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(chMenuBtnList[i] != currentChMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMenuBtn = chMenuBtnList[i];
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMenuBtnList[i] != currentChMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMenuBtn = chMenuBtnList[i];
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndFileManage = CreateWindow(szFileManageChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMenu,SW_HIDE);
					//SetFocus(hWndFileManage);
					ShowWindow(hWndFileManage,SW_SHOW);
					UpdateWindow(hWndFileManage);

					gCurrentWnd = hWndFileManage;
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
static LRESULT CALLBACK btnHotkeyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);

					gCurrentFocusBtn = currentChDisplayBtn;
					SetFocus(gCurrentFocusBtn);

					DestroyWindow(hWndChMenu);
					hWndChMenu = 0;

					gCurrentWnd =  hWndChDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(chMenuBtnList[i] != currentChMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMenuBtn = chMenuBtnList[i];
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
				}
				else if(wParam ==VK_DOWN)
				{
					while(chMenuBtnList[i] != currentChMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMenuBtn = chMenuBtnList[i];
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndHotKey = CreateWindow(szHotKeyChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMenu,SW_HIDE);
					//SetFocus(hWndHotKey);
					ShowWindow(hWndHotKey,SW_SHOW);
					UpdateWindow(hWndHotKey);

					gCurrentWnd = hWndHotKey;
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

/**************************
*测厚菜单窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK chMenuChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			hBtnMeasureCfg = CreateWindow(_T("BUTTON"),_T("MeasureCfg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMENU_MEASURECFG,hInst,NULL);

			hBtnParam = CreateWindow(_T("BUTTON"),_T("param"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMENU_PARAM,hInst,NULL);

			hBtnAdjust= CreateWindow(_T("BUTTON"),_T("adjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMENU_ADJUST,hInst,NULL);

			hBtnOtherCfg = CreateWindow(_T("BUTTON"),_T("othercfg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMENU_OTHERCFG,hInst,NULL);

			hBtnFile = CreateWindow(_T("BUTTON"),_T("fileMange"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMENU_FILE,hInst,NULL);

			hBtnHotkey = CreateWindow(_T("BUTTON"),_T("hotkey"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 5*BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMENU_HOTKEY,hInst,NULL);

			chMenuBtnList[0] = hBtnMeasureCfg;
			chMenuBtnList[1] = hBtnParam;
			chMenuBtnList[2] = hBtnAdjust;
			chMenuBtnList[3] = hBtnOtherCfg;
			chMenuBtnList[4] = hBtnFile;
			chMenuBtnList[5] = hBtnHotkey;
			//按钮控件的自定义处理过程
			OldBtnMeasureCfgProc = (WNDPROC) GetWindowLong(hBtnMeasureCfg,GWL_WNDPROC);
			SetWindowLong(hBtnMeasureCfg,GWL_WNDPROC,(LONG)btnMeasureCfgProc);
			
			OldBtnParamProc = (WNDPROC) GetWindowLong(hBtnParam,GWL_WNDPROC);
			SetWindowLong(hBtnParam,GWL_WNDPROC,(LONG)btnParamProc);
			
			OldBtnAdjustProc = (WNDPROC) GetWindowLong(hBtnAdjust,GWL_WNDPROC);
			SetWindowLong(hBtnAdjust,GWL_WNDPROC,(LONG)btnAdjustProc);
			
			OldBtnOtherCfgProc = (WNDPROC) GetWindowLong(hBtnOtherCfg,GWL_WNDPROC);
		    SetWindowLong(hBtnOtherCfg,GWL_WNDPROC,(LONG)btnOtherCfgProc);
			
			OldBtnFileProc = (WNDPROC) GetWindowLong(hBtnFile,GWL_WNDPROC);
			SetWindowLong(hBtnFile,GWL_WNDPROC,(LONG)btnFileProc);
			
			OldBtnHotkeyProc = (WNDPROC) GetWindowLong(hBtnHotkey,GWL_WNDPROC);
			SetWindowLong(hBtnHotkey,GWL_WNDPROC,(LONG)btnHotkeyProc);
			
			currentChMenuBtn = chMenuBtnList[0];//默认当前选中测量设置按钮
			SetFocus(currentChMenuBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentChMenuBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndChMenu, &ps);
			// TODO: Add any drawing code here...
		//	RECT rt;
		//	GetClientRect(hWnd, &rt);
		//	DrawText(hdc, szHello, _tcslen(szHello), &rt, DT_CENTER);
			//画按钮控件
			
			
			EndPaint(hWndChMenu, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				DestroyWindow(hWndChMenu);
				hWndChMenu = 0;

				// songchenguang add start 2010-4-26
				SetMenuStateFlag(FALSE);
				// songchenguang add end 2010-4-26
			}
			break;
		case WM_DRAWITEM://重画按钮
			pdis=(LPDRAWITEMSTRUCT)lParam;
			chMenu_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_COMMAND:
			/*
				if(LOWORD(wParam) == IDC_CHMENU_MEASURECFG)
				{
					currentChMenuBtn = chMenuBtnList[0]; 
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
					hWndMeasureCfg = CreateWindow(szMeasureCfgChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMenu,SW_HIDE);
					//SetFocus(hWndMeasureCfg);
					ShowWindow(hWndMeasureCfg,SW_SHOW);
					UpdateWindow(hWndMeasureCfg);
				}
				else if(LOWORD(wParam) == IDC_CHMENU_PARAM)
				{
					currentChMenuBtn = chMenuBtnList[1]; 
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
					hWndChParam = CreateWindow(szChParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMenu,SW_HIDE);
					//SetFocus(hWndChParam);
					ShowWindow(hWndChParam,SW_SHOW);
					UpdateWindow(hWndChParam);
				}
				else if(LOWORD(wParam) == IDC_CHMENU_OTHERCFG)
				{
					currentChMenuBtn = chMenuBtnList[2]; 
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
					hWndOtherCfg = CreateWindow(szOtherCfgChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMenu,SW_HIDE);
					//SetFocus(hWndOtherCfg);
					ShowWindow(hWndOtherCfg,SW_SHOW);
					UpdateWindow(hWndOtherCfg);
				}
				else if(LOWORD(wParam) == IDC_CHMENU_FILE)
				{
					currentChMenuBtn = chMenuBtnList[3]; 
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
					hWndFileManage = CreateWindow(szFileManageChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMenu,SW_HIDE);
					//SetFocus(hWndFileManage);
					ShowWindow(hWndFileManage,SW_SHOW);
					UpdateWindow(hWndFileManage);
				}
				else if(LOWORD(wParam) == IDC_CHMENU_HOTKEY)
				{
					currentChMenuBtn = chMenuBtnList[4]; 
					SetFocus(currentChMenuBtn);
					gCurrentFocusBtn = currentChMenuBtn;
					hWndHotKey = CreateWindow(szHotKeyChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMenu,SW_HIDE);
					//SetFocus(hWndHotKey);
					ShowWindow(hWndHotKey,SW_SHOW);
					UpdateWindow(hWndHotKey);
				}
				*/
			break;
		case WM_CLOSE:
		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/**************************
*加载测厚菜单窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chMenu_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_smBtnBitmap[]=
	{
		{IDC_CHMENU_MEASURECFG	,IDB_BITMAP_CH_CELIANGSHEZHI_NOSEL_R	 ,  IDB_BITMAP_CH_CELIANGSHEZHI_SELECT_R		},
		{IDC_CHMENU_PARAM	,IDB_BITMAP_PARAMETER_NOSEL_R	 ,  IDB_BITMAP_PARAMETER_SELECT_R		},
		{IDC_CHMENU_ADJUST	,IDB_BITMAP_CH_JIAOZHUN_NOSEL_R	 ,  IDB_BITMAP_CH_JIAOZHUN_SELECT_R		},
		{IDC_CHMENU_OTHERCFG	,IDB_BITMAP_OTHERCONFIGURE_NOSEL_R ,  IDB_BITMAP_OTHERCONFIGURE_SELECT_R	},
		{IDC_CHMENU_FILE	,IDB_BITMAP_FILE_NOSEL_R	 ,  IDB_BITMAP_FILE_SELECT_R		},
		{IDC_CHMENU_HOTKEY	,IDB_BITMAP_HOTKEY_NOSEL_R ,  IDB_BITMAP_HOTKEY_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_smBtnBitmap);
	return ReturnVal;
}
