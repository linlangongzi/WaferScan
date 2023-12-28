/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 测厚声速窗口源文件
 * Filename: chWaveSpeedWin.cpp
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


HWND hWndChWaveSpeed = NULL;						//测厚声速窗口句柄
HWND currentChWaveSpeedBtn = NULL;					//测厚声速窗口当前选中的按钮

static HWND hBtnChWaveSpeedSteel = NULL;				//钢按钮句柄
static HWND hBtnChWaveSpeedIron = NULL;			//铁按钮句柄
static HWND hBtnChWaveSpeedAluminium = NULL;			//铝按钮句柄
static HWND hBtnChWaveSpeedSelfDefine = NULL;				//开/关按钮句柄
static HWND chWaveSpeedBtnList[4] = {NULL,NULL,NULL,NULL};					//测厚声速窗口的按钮表
static WNDPROC OldBtnChWaveSpeedSteelProc = NULL;//钢按钮默认处理过程
static WNDPROC OldBtnChWaveSpeedIronProc = NULL;		//铁按钮默认处理过程
static WNDPROC OldBtnChWaveSpeedAluminiumProc = NULL;		//铝按钮默认处理过程
static WNDPROC OldBtnChWaveSpeedSelfDefineProc = NULL;			//开/关按钮默认处理过程

static const int buttonNum = 4;		//按钮控件数

ATOM registerChWaveSpeedChild(HINSTANCE hInstance);
static LRESULT CALLBACK chWaveSpeedChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chWaveSpeed_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChWaveSpeedSteelProc( HWND Wnd, 
													UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChWaveSpeedIronProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChWaveSpeedAluminiumProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChWaveSpeedSelfDefineProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//程序实例句柄

//extern HWND hWndSelfDefine;			//开/关窗口句柄
//extern HWND hWndAluminium;		//铝窗口句柄 
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndChMenu;		//测厚菜单窗口句柄
extern HWND currentChMenuBtn;	//测厚菜单窗口中当前选中的按钮
extern HWND gCurrentWnd;
/**************************
*注册测厚声速窗口
* @chWaveSpeed    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChWaveSpeedChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chWaveSpeedChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChWaveSpeedChild;

	return RegisterClass(&wc);
}
/**************************
*测厚声速窗口处理过程
* @chWaveSpeed    hInstance[in]:
* @chWaveSpeed		message[in]:
* @chWaveSpeed		wParam[in]:
* @chWaveSpeed		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chWaveSpeedChildProc(HWND hWnd, UINT message, 
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
			//画按钮控件
			hBtnChWaveSpeedSteel = CreateWindow(_T("BUTTON"),_T("SteelChWaveSpeed"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHWAVESPEED_STEEL,hInst,NULL);
			hBtnChWaveSpeedIron = CreateWindow(_T("BUTTON"),_T("ironChWaveSpeed"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHWAVESPEED_IRON,hInst,NULL);
			hBtnChWaveSpeedAluminium = CreateWindow(_T("BUTTON"),_T("aluminiumChWaveSpeed"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHWAVESPEED_ALUMINIUM,hInst,NULL);
			hBtnChWaveSpeedSelfDefine = CreateWindow(_T("BUTTON"),_T("selfDefineChWaveSpeed"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 3*BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHWAVESPEED_SELFDEFINE,hInst,NULL);
					
			chWaveSpeedBtnList[0] = hBtnChWaveSpeedSteel;
			chWaveSpeedBtnList[1] = hBtnChWaveSpeedIron;
			chWaveSpeedBtnList[2] = hBtnChWaveSpeedAluminium;
			chWaveSpeedBtnList[3] = hBtnChWaveSpeedSelfDefine;
			
			//按钮控件的自定义处理过程
			OldBtnChWaveSpeedSteelProc = (WNDPROC) GetWindowLong(hBtnChWaveSpeedSteel,GWL_WNDPROC);
			SetWindowLong(hBtnChWaveSpeedSteel,GWL_WNDPROC,(LONG)btnChWaveSpeedSteelProc);
			
			OldBtnChWaveSpeedIronProc = (WNDPROC) GetWindowLong(hBtnChWaveSpeedIron,GWL_WNDPROC);
			SetWindowLong(hBtnChWaveSpeedIron,GWL_WNDPROC,(LONG)btnChWaveSpeedIronProc);
			
			OldBtnChWaveSpeedAluminiumProc = (WNDPROC) GetWindowLong(hBtnChWaveSpeedAluminium,GWL_WNDPROC);
			SetWindowLong(hBtnChWaveSpeedAluminium,GWL_WNDPROC,(LONG)btnChWaveSpeedAluminiumProc);
			
			OldBtnChWaveSpeedSelfDefineProc = (WNDPROC) GetWindowLong(hBtnChWaveSpeedSelfDefine,GWL_WNDPROC);
			SetWindowLong(hBtnChWaveSpeedSelfDefine,GWL_WNDPROC,(LONG)btnChWaveSpeedSelfDefineProc);
		
			currentChWaveSpeedBtn = chWaveSpeedBtnList[0];//默认当前选中钢按钮
			SetFocus(currentChWaveSpeedBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentChWaveSpeedBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
		/*		if(LOWORD(wParam) == IDC_CHWAVESPEED_ALUMINIUM)
				{
					currentChWaveSpeedBtn = chWaveSpeedBtnList[2]; 
					SetFocus(currentChWaveSpeedBtn);
					gCurrentFocusBtn = currentChWaveSpeedBtn;
					hWndAluminium = CreateWindow(szAluminiumChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChWaveSpeed,SW_HIDE);
					//SetFocus(hWndAluminium);
					ShowWindow(hWndAluminium,SW_SHOW);
					UpdateWindow(hWndAluminium);
				}
				else if(LOWORD(wParam) == IDC_CHWAVESPEED_SELFDEFINE)
				{
				//	currentChWaveSpeedBtn = chWaveSpeedBtnList[3]; 
				//	SetFocus(currentChWaveSpeedBtn);
				//	gCurrentFocusBtn = currentChWaveSpeedBtn;
				}
			*/
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			chWaveSpeed_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_CLOSE:
		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载测厚声速窗口按钮图片
* @chWaveSpeed    hInstance[in]:
* @chWaveSpeed		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chWaveSpeed_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chWaveSpeedBtnBitmap[]=
	{
		{IDC_CHWAVESPEED_STEEL	,IDB_BITMAP_DINGLIANGFENXI_NOSEL_R	 ,  IDB_BITMAP_DINGLIANGFENXI_SELECT_R		},
		{IDC_CHWAVESPEED_IRON	,IDB_BITMAP_TESHUXIANSHI_NOSEL_R	 ,  IDB_BITMAP_TESHUXIANSHI_SELECT_R		},
		{IDC_CHWAVESPEED_ALUMINIUM	,IDB_BITMAP_ZHUANYONGGONGNENG_NOSEL_R ,  IDB_BITMAP_ZHUANYONGGONGNENG_SELECT_R	},
		{IDC_CHWAVESPEED_SELFDEFINE	,IDB_BITMAP_LUXIANG_NOSEL_R ,  IDB_BITMAP_LUXIANG_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chWaveSpeedBtnBitmap);
	return ReturnVal;
}
/**************************
*钢按钮自定义处理过程
* @chWaveSpeed    hInstance[in]:
* @chWaveSpeed		message[in]:
* @chWaveSpeed		wParam[in]:
* @chWaveSpeed		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChWaveSpeedSteelProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChMenu,SW_SHOW);
					SetFocus(currentChMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChWaveSpeed);
					hWndChWaveSpeed = 0;
					
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd = hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(chWaveSpeedBtnList[i] != currentChWaveSpeedBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChWaveSpeedBtn = chWaveSpeedBtnList[i];
					SetFocus(currentChWaveSpeedBtn);
					gCurrentFocusBtn = currentChWaveSpeedBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chWaveSpeedBtnList[i] != currentChWaveSpeedBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChWaveSpeedBtn = chWaveSpeedBtnList[i];
					SetFocus(currentChWaveSpeedBtn);
					gCurrentFocusBtn = currentChWaveSpeedBtn;
				}
				else if(wParam == VK_RETURN)
				{
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
	return CallWindowProc(OldBtnChWaveSpeedSteelProc,Wnd,Msg,wParam,lParam);
}
/**************************
*铁按钮自定义处理过程
* @chWaveSpeed    hInstance[in]:
* @chWaveSpeed		message[in]:
* @chWaveSpeed		wParam[in]:
* @chWaveSpeed		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChWaveSpeedIronProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChMenu,SW_SHOW);
					SetFocus(currentChMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChWaveSpeed);
					hWndChWaveSpeed = 0;
					
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd = hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(chWaveSpeedBtnList[i] != currentChWaveSpeedBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChWaveSpeedBtn = chWaveSpeedBtnList[i];
					SetFocus(currentChWaveSpeedBtn);
					gCurrentFocusBtn = currentChWaveSpeedBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chWaveSpeedBtnList[i] != currentChWaveSpeedBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChWaveSpeedBtn = chWaveSpeedBtnList[i];
					SetFocus(currentChWaveSpeedBtn);
					gCurrentFocusBtn = currentChWaveSpeedBtn;
				}
				else if(wParam == VK_RETURN)
				{
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
	return CallWindowProc(OldBtnChWaveSpeedIronProc,Wnd,Msg,wParam,lParam);
}
/**************************
*铝按钮自定义处理过程
* @chWaveSpeed    hInstance[in]:
* @chWaveSpeed		message[in]:
* @chWaveSpeed		wParam[in]:
* @chWaveSpeed		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChWaveSpeedAluminiumProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChMenu,SW_SHOW);
					SetFocus(currentChMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChWaveSpeed);
					hWndChWaveSpeed = 0;
					
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd = hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(chWaveSpeedBtnList[i] != currentChWaveSpeedBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChWaveSpeedBtn = chWaveSpeedBtnList[i];
					SetFocus(currentChWaveSpeedBtn);
					gCurrentFocusBtn = currentChWaveSpeedBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chWaveSpeedBtnList[i] != currentChWaveSpeedBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChWaveSpeedBtn = chWaveSpeedBtnList[i];
					SetFocus(currentChWaveSpeedBtn);
					gCurrentFocusBtn = currentChWaveSpeedBtn;
				}
				else if(wParam == VK_RETURN)
				{
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
	return CallWindowProc(OldBtnChWaveSpeedAluminiumProc,Wnd,Msg,wParam,lParam);
}
/**************************
*开/关按钮自定义处理过程
* @chWaveSpeed    hInstance[in]:
* @chWaveSpeed		message[in]:
* @chWaveSpeed		wParam[in]:
* @chWaveSpeed		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChWaveSpeedSelfDefineProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChMenu,SW_SHOW);
					SetFocus(currentChMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChWaveSpeed);
					hWndChWaveSpeed = 0;
					
					gCurrentFocusBtn = currentChMenuBtn;
				}
			
				else if(wParam == VK_UP)
				{
					while(chWaveSpeedBtnList[i] != currentChWaveSpeedBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChWaveSpeedBtn = chWaveSpeedBtnList[i];
					SetFocus(currentChWaveSpeedBtn);
					gCurrentFocusBtn = currentChWaveSpeedBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chWaveSpeedBtnList[i] != currentChWaveSpeedBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChWaveSpeedBtn = chWaveSpeedBtnList[i];
					SetFocus(currentChWaveSpeedBtn);
					gCurrentFocusBtn = currentChWaveSpeedBtn;
				}
				else if(wParam == VK_RETURN)
				{
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
	return CallWindowProc(OldBtnChWaveSpeedSelfDefineProc,Wnd,Msg,wParam,lParam);
}
