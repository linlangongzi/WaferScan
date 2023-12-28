/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 快捷键窗口源文件
 * Filename: hotKeyWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "hotKeyWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"

HWND hWndHotKey = NULL;//快捷键窗口句柄
HWND hBtnHotKeySelect = NULL;//快捷键选择按钮句柄
HWND hBtnHotKeySetting = NULL;//设定按钮句柄
HWND hBtnHotKeyDelete = NULL;//删除按钮句柄
HWND hotKeyBtnList[3] ={NULL,NULL,NULL};//快捷键窗口的按钮表
WNDPROC OldBtnHotKeySelectProc = NULL;//快捷键选择按钮默认处理过程
WNDPROC OldBtnHotKeySettingProc = NULL;//设定按钮默认处理过程
WNDPROC OldBtnHotKeyDeleteProc = NULL;//一删除按钮默认处理过程
HWND currentHotKeyBtn = NULL;//模式窗中当前选中的按钮

static const int buttonNum = 3;//按钮控件数
static HWND hLableSelect = NULL;//快捷键选择按钮中的静态文本框
static HBRUSH hStaticBrush = NULL;
extern HWND hWndTsMenu;							//探伤菜单窗口句柄
extern HWND hWndChMenu;							//测厚菜单窗口句柄
extern HWND currentTsMenuBtn;					//探伤菜单窗口中当前选中的按钮
extern HWND currentChMenuBtn;					//测厚菜单窗口中当前选中的按钮

extern HINSTANCE hInst;//程序实例句柄
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//当前显示的子窗口

/**************************
*注册快捷键窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerHotKeyChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) hotKeyChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szHotKeyChild;

	return RegisterClass(&wc);
}
/**************************
*快捷键窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK hotKeyChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			if(hStaticBrush == NULL)	
				hStaticBrush = CreateSolidBrush(BK_COLOR_WHITE);
			//画按钮控件
			hBtnHotKeySelect = CreateWindow(_T("BUTTON"),_T("Select"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_HOTKEY_SELECT,hInst,NULL);
			hBtnHotKeySetting = CreateWindow(_T("BUTTON"),_T("Setting"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_HOTKEY_SETTING,hInst,NULL);
			hBtnHotKeyDelete = CreateWindow(_T("BUTTON"),_T("Delete"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_HOTKEY_DELETE,hInst,NULL);
			
			hotKeyBtnList[0] = hBtnHotKeySelect;
			hotKeyBtnList[1] = hBtnHotKeySetting;
			hotKeyBtnList[2] = hBtnHotKeyDelete;
			//按钮控件的自定义处理过程
			OldBtnHotKeySelectProc = (WNDPROC) GetWindowLong(hBtnHotKeySelect,GWL_WNDPROC);
			SetWindowLong(hBtnHotKeySelect,GWL_WNDPROC,(LONG)btnHotKeySelectProc);
			OldBtnHotKeySettingProc = (WNDPROC) GetWindowLong(hBtnHotKeySetting,GWL_WNDPROC);
			SetWindowLong(hBtnHotKeySetting,GWL_WNDPROC,(LONG)btnHotKeySettingProc);
			OldBtnHotKeyDeleteProc = (WNDPROC) GetWindowLong(hBtnHotKeyDelete,GWL_WNDPROC);
		    SetWindowLong(hBtnHotKeyDelete,GWL_WNDPROC,(LONG)btnHotKeyDeleteProc);
		    
		//	hLableSelect = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER|WS_BORDER,LABEL_START_X,
		//			LABEL_4_START_Y,LABEL_WIDTH,LABEL_HEIGHT,hBtnHotKeySelect,(HMENU)IDL_HOTKEY_SELECT,hInst,NULL);
			
			currentHotKeyBtn = hotKeyBtnList[0];//默认当前选中选择按钮
			SetFocus(currentHotKeyBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentHotKeyBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndHotKey, &ps);
			
			EndPaint(hWndHotKey, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				if(hWndTsMenu)
				{
					ShowWindow(hWndTsMenu,SW_SHOW);
					SetFocus(currentTsMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndHotKey);
					hWndHotKey = 0;
					
					gCurrentFocusBtn = currentTsMenuBtn;

					gCurrentWnd = hWndTsMenu;
				}
				
				else if(hWndChMenu)
				{
					DestroyWindow(hWndHotKey);
					hWndHotKey = 0;
					ShowWindow(hWndChMenu,SW_SHOW);
					SetFocus(currentChMenuBtn);//上级菜单最后选中的按钮获得输入焦点
					gCurrentFocusBtn = currentChMenuBtn;
					
					gCurrentWnd = hWndChMenu;
				}
				else
				{
					;//
				}
			}
			
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			hotKey_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_CLOSE:
		
			break;
		case WM_DESTROY:
			if(hStaticBrush)
			{
				DeleteObject(hStaticBrush);
				hStaticBrush = NULL;
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载快捷键窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int hotKey_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_hotKeyBtnBitmap[]=
	{
		{IDC_HOTKEY_SELECT	,IDB_BITMAP_KUAIJIEJIANXUANZE_NOSEL_R	 ,  IDB_BITMAP_KUAIJIEJIANXUANZE_SELECT_R		},
		{IDC_HOTKEY_SETTING	,IDB_BITMAP_SHEDING_NOSEL_R	 ,  IDB_BITMAP_SHEDING_SELECT_R		},
		{IDC_HOTKEY_DELETE	,IDB_BITMAP_HOTKEY_SHANCHU_NOSEL_R ,  IDB_BITMAP_HOTKEY_SHANCHU_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_hotKeyBtnBitmap);
	return ReturnVal;
}
/**************************
*快捷键选择按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHotKeySelectProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			
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
					while(hotKeyBtnList[i] != currentHotKeyBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHotKeyBtn = hotKeyBtnList[i];
					SetFocus(currentHotKeyBtn);
					gCurrentFocusBtn = currentHotKeyBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hotKeyBtnList[i] != currentHotKeyBtn)
					{
						i++;
					}
					i++;
					if(i > 2)
						i = 0;
					currentHotKeyBtn = hotKeyBtnList[i];
					SetFocus(currentHotKeyBtn);
					gCurrentFocusBtn = currentHotKeyBtn;
				}
				else if(wParam == VK_LEFT)
				{
				}
				else if(wParam == VK_RIGHT)
				{
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
		case WM_CTLCOLORSTATIC:
			SetBkMode((HDC)wParam,TRANSPARENT);
			SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
			return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnHotKeySelectProc,Wnd,Msg,wParam,lParam);
}
/**************************
*设定按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHotKeySettingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(hotKeyBtnList[i] != currentHotKeyBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = 2;
					currentHotKeyBtn = hotKeyBtnList[i];
					SetFocus(currentHotKeyBtn);
					gCurrentFocusBtn = currentHotKeyBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hotKeyBtnList[i] != currentHotKeyBtn)
					{
						i++;
					}
					i++;
					if(i > 2)
						i = 0;
					currentHotKeyBtn = hotKeyBtnList[i];
					SetFocus(currentHotKeyBtn);
					gCurrentFocusBtn = currentHotKeyBtn;
				}
				else if(wParam == VK_RETURN)
				{
				/*	hWndHotKey = CreateWindow(szHotKeyChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 0, 0, 120, 450, hWnd, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					SetFocus(hWndHotKey);
					ShowWindow(hWndHotKey,SW_SHOW);
					UpdateWindow(hWndHotKey);*/
				//	SetWindowPos(hWndHotKey,HWND_TOPMOST,0,0,120,450,
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
	return CallWindowProc(OldBtnHotKeySettingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*删除按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHotKeyDeleteProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(hotKeyBtnList[i] != currentHotKeyBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = 2;
					currentHotKeyBtn = hotKeyBtnList[i];
					SetFocus(currentHotKeyBtn);
					gCurrentFocusBtn = currentHotKeyBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hotKeyBtnList[i] != currentHotKeyBtn)
					{
						i++;
					}
					i++;
					if(i > 2)
						i = 0;
					currentHotKeyBtn = hotKeyBtnList[i];
					SetFocus(currentHotKeyBtn);
					gCurrentFocusBtn = currentHotKeyBtn;
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
	return CallWindowProc(OldBtnHotKeyDeleteProc,Wnd,Msg,wParam,lParam);
}
