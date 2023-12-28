/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 测厚校准窗口源文件
 * Filename: chAdjustWin.cpp
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


HWND hWndChAdjust = NULL;						//测厚校准窗口句柄
HWND currentChAdjustBtn = NULL;					//测厚校准窗口当前选中的按钮

static HWND hBtnChAdjustWaveSpeed = NULL;				//声速校准按钮句柄
static HWND hBtnChAdjustZero = NULL;			//零点校准按钮句柄
static HWND hBtnChAdjustTempCompensate = NULL;			//温度补偿按钮句柄
static HWND hBtnChAdjustHeadVCompensate = NULL;				//探头v型补偿按钮句柄
static HWND chAdjustBtnList[4] = {NULL,NULL,NULL,NULL};					//测厚校准窗口的按钮表
static WNDPROC OldBtnChAdjustWaveSpeedProc = NULL;//声速校准按钮默认处理过程
static WNDPROC OldBtnChAdjustZeroProc = NULL;		//零点校准按钮默认处理过程
static WNDPROC OldBtnChAdjustTempCompensateProc = NULL;		//温度补偿按钮默认处理过程
static WNDPROC OldBtnChAdjustHeadVCompensateProc = NULL;			//探头v型补偿按钮默认处理过程

static const int buttonNum = 4;		//按钮控件数

ATOM registerChAdjustChild(HINSTANCE hInstance);
static LRESULT CALLBACK chAdjustChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chAdjust_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChAdjustWaveSpeedProc( HWND Wnd, 
													UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChAdjustZeroProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChAdjustTempCompensateProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChAdjustHeadVCompensateProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//程序实例句柄
extern HWND hWndChWaveSpeedAdjust;		//声速校准窗口句柄
extern HWND hWndChZeroAdjust;			//零点校准窗口句柄
extern HWND hWndChHeadVCompensate;		//探头v型补偿窗口句柄
extern HWND hWndChTempCompensate;		//温度补偿窗口句柄 
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndChMenu;		//测厚菜单窗口句柄
extern HWND currentChMenuBtn;	//测厚菜单窗口中当前选中的按钮
extern HWND gCurrentWnd;
/**************************
*注册测厚校准窗口
* @chAdjust    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChAdjustChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chAdjustChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChAdjustChild;

	return RegisterClass(&wc);
}
/**************************
*测厚校准窗口处理过程
* @chAdjust    hInstance[in]:
* @chAdjust		message[in]:
* @chAdjust		wParam[in]:
* @chAdjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chAdjustChildProc(HWND hWnd, UINT message, 
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
			hBtnChAdjustWaveSpeed = CreateWindow(_T("BUTTON"),_T("waveSpeedChAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHADJUST_WAVESPEED,hInst,NULL);
			hBtnChAdjustZero = CreateWindow(_T("BUTTON"),_T("zeroChAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHADJUST_ZERO,hInst,NULL);
			hBtnChAdjustTempCompensate = CreateWindow(_T("BUTTON"),_T("tempCompensateChAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHADJUST_TEMPCOMPENSATE,hInst,NULL);
			hBtnChAdjustHeadVCompensate = CreateWindow(_T("BUTTON"),_T("headVCompensateChAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 3*BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHADJUST_HEADVCOMPENSATE,hInst,NULL);
					
			chAdjustBtnList[0] = hBtnChAdjustWaveSpeed;
			chAdjustBtnList[1] = hBtnChAdjustZero;
			chAdjustBtnList[2] = hBtnChAdjustTempCompensate;
			chAdjustBtnList[3] = hBtnChAdjustHeadVCompensate;
			
			//按钮控件的自定义处理过程
			OldBtnChAdjustWaveSpeedProc = (WNDPROC) GetWindowLong(hBtnChAdjustWaveSpeed,GWL_WNDPROC);
			SetWindowLong(hBtnChAdjustWaveSpeed,GWL_WNDPROC,(LONG)btnChAdjustWaveSpeedProc);
			
			OldBtnChAdjustZeroProc = (WNDPROC) GetWindowLong(hBtnChAdjustZero,GWL_WNDPROC);
			SetWindowLong(hBtnChAdjustZero,GWL_WNDPROC,(LONG)btnChAdjustZeroProc);
			
			OldBtnChAdjustTempCompensateProc = (WNDPROC) GetWindowLong(hBtnChAdjustTempCompensate,GWL_WNDPROC);
			SetWindowLong(hBtnChAdjustTempCompensate,GWL_WNDPROC,(LONG)btnChAdjustTempCompensateProc);
			
			OldBtnChAdjustHeadVCompensateProc = (WNDPROC) GetWindowLong(hBtnChAdjustHeadVCompensate,GWL_WNDPROC);
			SetWindowLong(hBtnChAdjustHeadVCompensate,GWL_WNDPROC,(LONG)btnChAdjustHeadVCompensateProc);
		
			currentChAdjustBtn = chAdjustBtnList[0];//默认当前选中声速校准按钮
			SetFocus(currentChAdjustBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentChAdjustBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
		/*		if(LOWORD(wParam) == IDC_CHADJUST_TEMPCOMPENSATE)
				{
					currentChAdjustBtn = chAdjustBtnList[2]; 
					SetFocus(currentChAdjustBtn);
					gCurrentFocusBtn = currentChAdjustBtn;
					hWndTempCompensate = CreateWindow(szTempCompensateChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChAdjust,SW_HIDE);
					//SetFocus(hWndTempCompensate);
					ShowWindow(hWndTempCompensate,SW_SHOW);
					UpdateWindow(hWndTempCompensate);
				}
				else if(LOWORD(wParam) == IDC_CHADJUST_HEADVCOMPENSATE)
				{
				//	currentChAdjustBtn = chAdjustBtnList[3]; 
				//	SetFocus(currentChAdjustBtn);
				//	gCurrentFocusBtn = currentChAdjustBtn;
				}
			*/
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			chAdjust_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_CLOSE:
		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载测厚校准窗口按钮图片
* @chAdjust    hInstance[in]:
* @chAdjust		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chAdjust_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chAdjustBtnBitmap[]=
	{
		{IDC_CHADJUST_WAVESPEED	,IDB_BITMAP_SHENGSUJIAOZHUN_NOSEL_R	 ,  IDB_BITMAP_SHENGSUJIAOZHUN_SELECT_R		},
		{IDC_CHADJUST_ZERO	,IDB_BITMAP_LINGDIANJIAOZHUN_NOSEL_R	 ,  IDB_BITMAP_LINGDIANJIAOZHUN_SELECT_R		},
		{IDC_CHADJUST_TEMPCOMPENSATE	,IDB_BITMAP_WENDUBUCHANG_NOSEL_R ,  IDB_BITMAP_WENDUBUCHANG_SELECT_R	},
		{IDC_CHADJUST_HEADVCOMPENSATE	,IDB_BITMAP_TANTOUVXINGBUCHANG_NOSEL_R ,  IDB_BITMAP_TANTOUVXINGBUCHANG_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chAdjustBtnBitmap);
	return ReturnVal;
}
/**************************
*声速校准按钮自定义处理过程
* @chAdjust    hInstance[in]:
* @chAdjust		message[in]:
* @chAdjust		wParam[in]:
* @chAdjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAdjustWaveSpeedProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndChAdjust);
					hWndChAdjust = 0;

					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd = hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(chAdjustBtnList[i] != currentChAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAdjustBtn = chAdjustBtnList[i];
					SetFocus(currentChAdjustBtn);
					gCurrentFocusBtn = currentChAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAdjustBtnList[i] != currentChAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAdjustBtn = chAdjustBtnList[i];
					SetFocus(currentChAdjustBtn);
					gCurrentFocusBtn = currentChAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChWaveSpeedAdjust = CreateWindow(szChWaveSpeedAdjustChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChAdjust,SW_HIDE);
					
					ShowWindow(hWndChWaveSpeedAdjust,SW_SHOW);
					UpdateWindow(hWndChWaveSpeedAdjust);

					gCurrentWnd = hWndChWaveSpeedAdjust;
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
	return CallWindowProc(OldBtnChAdjustWaveSpeedProc,Wnd,Msg,wParam,lParam);
}
/**************************
*零点校准按钮自定义处理过程
* @chAdjust    hInstance[in]:
* @chAdjust		message[in]:
* @chAdjust		wParam[in]:
* @chAdjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAdjustZeroProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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

					DestroyWindow(hWndChAdjust);
					hWndChAdjust = 0;

					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd = hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(chAdjustBtnList[i] != currentChAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAdjustBtn = chAdjustBtnList[i];
					SetFocus(currentChAdjustBtn);
					gCurrentFocusBtn = currentChAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAdjustBtnList[i] != currentChAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAdjustBtn = chAdjustBtnList[i];
					SetFocus(currentChAdjustBtn);
					gCurrentFocusBtn = currentChAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChZeroAdjust = CreateWindow(szChZeroAdjustChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChAdjust,SW_HIDE);
					//SetFocus(hWndChZeroAdjust);
					ShowWindow(hWndChZeroAdjust,SW_SHOW);
					UpdateWindow(hWndChZeroAdjust);

					gCurrentWnd = hWndChZeroAdjust;
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
	return CallWindowProc(OldBtnChAdjustZeroProc,Wnd,Msg,wParam,lParam);
}
/**************************
*温度补偿按钮自定义处理过程
* @chAdjust    hInstance[in]:
* @chAdjust		message[in]:
* @chAdjust		wParam[in]:
* @chAdjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAdjustTempCompensateProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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

					DestroyWindow(hWndChAdjust);
					hWndChAdjust = 0;

					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd = hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(chAdjustBtnList[i] != currentChAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAdjustBtn = chAdjustBtnList[i];
					SetFocus(currentChAdjustBtn);
					gCurrentFocusBtn = currentChAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAdjustBtnList[i] != currentChAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAdjustBtn = chAdjustBtnList[i];
					SetFocus(currentChAdjustBtn);
					gCurrentFocusBtn = currentChAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChTempCompensate = CreateWindow(szChTempCompensateChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChAdjust,SW_HIDE);
					//SetFocus(hWndChTempCompensate);
					ShowWindow(hWndChTempCompensate,SW_SHOW);
					UpdateWindow(hWndChTempCompensate);

					gCurrentWnd = hWndChTempCompensate;
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
	return CallWindowProc(OldBtnChAdjustTempCompensateProc,Wnd,Msg,wParam,lParam);
}
/**************************
*探头v型补偿按钮自定义处理过程
* @chAdjust    hInstance[in]:
* @chAdjust		message[in]:
* @chAdjust		wParam[in]:
* @chAdjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAdjustHeadVCompensateProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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

					DestroyWindow(hWndChAdjust);
					hWndChAdjust = 0;

					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd = hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(chAdjustBtnList[i] != currentChAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAdjustBtn = chAdjustBtnList[i];
					SetFocus(currentChAdjustBtn);
					gCurrentFocusBtn = currentChAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAdjustBtnList[i] != currentChAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAdjustBtn = chAdjustBtnList[i];
					SetFocus(currentChAdjustBtn);
					gCurrentFocusBtn = currentChAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChHeadVCompensate = CreateWindow(szChHeadVCompensateChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChAdjust,SW_HIDE);
					//SetFocus(hWndChHeadVCompensate);
					ShowWindow(hWndChHeadVCompensate,SW_SHOW);
					UpdateWindow(hWndChHeadVCompensate);

					gCurrentWnd = hWndChHeadVCompensate;

					SetRedrawFlag(TRUE); //要重新画坐标轴
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
	return CallWindowProc(OldBtnChAdjustHeadVCompensateProc,Wnd,Msg,wParam,lParam);
}
