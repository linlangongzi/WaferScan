/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 测厚参数窗口源文件
 * Filename: paramWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "const.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"


HWND hWndChParam = NULL;			//测厚参数窗口句柄
HWND currentChParamBtn = NULL;		//测厚参数窗中当前选中的按钮
static HWND hBtnParamSend = NULL;		//发射参数按钮句柄
static HWND hBtnParamRecv = NULL;		//接收参数按钮句柄
static HWND hBtnParamWaveSpeed = NULL;	//声速按钮句柄
static HWND hBtnParamDisplay = NULL;	//显示设置按钮句柄
static HWND hBtnParamGate = NULL;		//闸门设置按钮句柄

static HWND paramBtnList[5] = {NULL,NULL,NULL,NULL,NULL};	//参数窗口的按钮表
static const int buttonNum = 5;		//按钮控件数

static WNDPROC OldBtnParamSendProc = NULL;		//发射参数按钮默认处理过程
static WNDPROC OldBtnParamRecvProc = NULL;		//接收参数按钮默认处理过程
static WNDPROC OldBtnParamWaveSpeedProc = NULL;	//声速按钮默认处理过程
static WNDPROC OldBtnParamDisplayProc = NULL;	//显示设置按钮默认处理过程
static WNDPROC OldBtnParamGateProc = NULL;		//闸门设置按钮默认处理过程

ATOM registerChParamChild(HINSTANCE hInstance);
static LRESULT CALLBACK paramChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int param_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnParamSendProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnParamRecvProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnParamWaveSpeedProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnParamDisplayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnParamGateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );


extern HWND hWndChMenu;		//测厚菜单窗口句柄
extern HWND currentChMenuBtn;	//测厚菜单窗口中当前选中的按钮
extern HINSTANCE hInst;		//程序实例句柄
extern HWND hWndRecvParam;	//接收参数窗口句柄
extern HWND hWndSendParam;	//发射参数窗口句柄
extern HWND hWndDisplay;	//显示窗口句柄 
extern HWND hWndGateDisplay;
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndWaveSpeed;
extern HWND gCurrentWnd;
/**************************
*注册参数窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChParamChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) paramChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChParamChild;

	return RegisterClass(&wc);
}
/**************************
*参数窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK paramChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			hBtnParamSend = CreateWindow(_T("BUTTON"),_T("sendParam"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHPARAM_SEND,hInst,NULL);

			hBtnParamRecv = CreateWindow(_T("BUTTON"),_T("recvParam"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHPARAM_RECV,hInst,NULL);

			hBtnParamWaveSpeed = CreateWindow(_T("BUTTON"),_T("waveSpeedParam"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHPARAM_WAVESPEED,hInst,NULL);

			hBtnParamDisplay = CreateWindow(_T("BUTTON"),_T("displayParam"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHPARAM_DISPLAY,hInst,NULL);
			
			hBtnParamGate = CreateWindow(_T("BUTTON"),_T("gateParam"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_PARAM_GATE,hInst,NULL);
					
			paramBtnList[0] = hBtnParamSend;
			paramBtnList[1] = hBtnParamRecv;			
			paramBtnList[2] = hBtnParamWaveSpeed;
			paramBtnList[3] = hBtnParamDisplay;
			paramBtnList[4] = hBtnParamGate;
			//按钮控件的自定义处理过程
			OldBtnParamSendProc = (WNDPROC) GetWindowLong(hBtnParamSend,GWL_WNDPROC);
			SetWindowLong(hBtnParamSend,GWL_WNDPROC,(LONG)btnParamSendProc);

			OldBtnParamRecvProc = (WNDPROC) GetWindowLong(hBtnParamRecv,GWL_WNDPROC);
			SetWindowLong(hBtnParamRecv,GWL_WNDPROC,(LONG)btnParamRecvProc);

			OldBtnParamWaveSpeedProc = (WNDPROC) GetWindowLong(hBtnParamWaveSpeed,GWL_WNDPROC);
			SetWindowLong(hBtnParamWaveSpeed,GWL_WNDPROC,(LONG)btnParamWaveSpeedProc);

			OldBtnParamDisplayProc = (WNDPROC) GetWindowLong(hBtnParamDisplay,GWL_WNDPROC);
			SetWindowLong(hBtnParamDisplay,GWL_WNDPROC,(LONG)btnParamDisplayProc);

			OldBtnParamGateProc = (WNDPROC) GetWindowLong(hBtnParamGate,GWL_WNDPROC);
			SetWindowLong(hBtnParamGate,GWL_WNDPROC,(LONG)btnParamGateProc);
		
			currentChParamBtn = paramBtnList[0];//默认当前选中发射参数按钮
			SetFocus(currentChParamBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentChParamBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndChParam, &ps);
			
			EndPaint(hWndChParam, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			/*
				if(LOWORD(wParam) == IDC_CHPARAM_SEND)
				{
					currentChParamBtn = paramBtnList[0]; 
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
					hWndSendParam = CreateWindow(szSendParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					hWndSendParam = CreateWindow(szSendParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChParam,SW_HIDE);
					//SetFocus(hWndSendParam);
					ShowWindow(hWndSendParam,SW_SHOW);
					UpdateWindow(hWndSendParam);
				}
				else if(LOWORD(wParam) == IDC_CHPARAM_RECV)
				{
					currentChParamBtn = paramBtnList[1]; 
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
					hWndRecvParam = CreateWindow(szRecvParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChParam,SW_HIDE);
					//SetFocus(hWndRecvParam);
					ShowWindow(hWndRecvParam,SW_SHOW);
					UpdateWindow(hWndRecvParam);
				}
				else if(LOWORD(wParam) == IDC_CHPARAM_WAVESPEED)
				{
					currentChParamBtn = paramBtnList[2]; 
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
					hWndRecvParam = CreateWindow(szRecvParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChParam,SW_HIDE);
					//SetFocus(hWndRecvParam);
					ShowWindow(hWndRecvParam,SW_SHOW);
					UpdateWindow(hWndRecvParam);
				}
				else if(LOWORD(wParam) == IDC_CHPARAM_DISPLAY)
				{
					currentChParamBtn = paramBtnList[3]; 
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
					hWndDisplay= CreateWindow(szDisplayChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChParam,SW_HIDE);
					//SetFocus(hWndDisplay);
					ShowWindow(hWndDisplay,SW_SHOW);
					UpdateWindow(hWndDisplay);
				}
				*/
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			param_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_CLOSE:
		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载参数窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int param_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_paramBtnBitmap[]=
	{
		{IDC_CHPARAM_SEND,	IDB_BITMAP_SENDPARAM_NOSEL_R	 ,  IDB_BITMAP_SENDPARAM_SELECT_R		},
		{IDC_CHPARAM_RECV,	IDB_BITMAP_RECVPARAM_NOSEL_R	 ,  IDB_BITMAP_RECVPARAM_SELECT_R		},
		{IDC_CHPARAM_WAVESPEED,	IDB_BITMAP_SHENGSU_NOSEL_R ,  IDB_BITMAP_SHENGSU_SELECT_R	},
		{IDC_CHPARAM_DISPLAY,	IDB_BITMAP_DISPLAY_NOSEL_R ,  IDB_BITMAP_DISPLAY_SELECT_R	},
		{IDC_PARAM_GATE	,IDB_BITMAP_GATE_NOSEL_R	 ,  IDB_BITMAP_GATE_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_paramBtnBitmap);
	return ReturnVal;
}
/**************************
*发射参数按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnParamSendProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndChParam);
					hWndChParam = 0;
					
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd	= hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(paramBtnList[i] != currentChParamBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChParamBtn = paramBtnList[i];
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(paramBtnList[i] != currentChParamBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChParamBtn = paramBtnList[i];
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndSendParam = CreateWindow(szSendParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChParam,SW_HIDE);
					//SetFocus(hWndSendParam);
					ShowWindow(hWndSendParam,SW_SHOW);
					UpdateWindow(hWndSendParam);

					gCurrentWnd	= hWndSendParam;
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
	return CallWindowProc(OldBtnParamSendProc,Wnd,Msg,wParam,lParam);
}
/**************************
*接收参数按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnParamRecvProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndChParam);
					hWndChParam = 0;
					
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd	= hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(paramBtnList[i] != currentChParamBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChParamBtn = paramBtnList[i];
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(paramBtnList[i] != currentChParamBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChParamBtn = paramBtnList[i];
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndRecvParam = CreateWindow(szRecvParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChParam,SW_HIDE);
					//SetFocus(hWndRecvParam);
					UpdateWindow(hWndRecvParam);

					gCurrentWnd	= hWndRecvParam;
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
	return CallWindowProc(OldBtnParamRecvProc,Wnd,Msg,wParam,lParam);
}

/**************************
*显示设置按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnParamWaveSpeedProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndChParam);
					hWndChParam = 0;
					
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd	= hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(paramBtnList[i] != currentChParamBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChParamBtn = paramBtnList[i];
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(paramBtnList[i] != currentChParamBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChParamBtn = paramBtnList[i];
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
				}
				else if(wParam == VK_RETURN)
				{
				/*	hWndChWaveSpeed = CreateWindow(szChWaveSpeedChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChParam,SW_HIDE);
					//SetFocus(hWndChWaveSpeed);
					ShowWindow(hWndChWaveSpeed,SW_SHOW);
					UpdateWindow(hWndChWaveSpeed);
				*/

					hWndWaveSpeed = CreateWindow(szWaveSpeedChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChParam,SW_HIDE);
					//SetFocus(hWndWaveSpeed);
					ShowWindow(hWndWaveSpeed,SW_SHOW);
					UpdateWindow(hWndWaveSpeed);

					gCurrentWnd = hWndWaveSpeed;
				
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
	return CallWindowProc(OldBtnParamWaveSpeedProc,Wnd,Msg,wParam,lParam);
}


/**************************
*显示设置按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnParamDisplayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndChParam);
					hWndChParam = 0;
					
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd	= hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(paramBtnList[i] != currentChParamBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChParamBtn = paramBtnList[i];
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(paramBtnList[i] != currentChParamBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChParamBtn = paramBtnList[i];
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndDisplay= CreateWindow(szDisplayChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChParam,SW_HIDE);
					//SetFocus(hWndDisplay);
					ShowWindow(hWndDisplay,SW_SHOW);
					UpdateWindow(hWndDisplay);

					gCurrentWnd	= hWndDisplay;
					
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
	return CallWindowProc(OldBtnParamDisplayProc,Wnd,Msg,wParam,lParam);
}
/**************************
*闸门设置按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnParamGateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndChParam);
					hWndChParam = 0;
					
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd	= hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(paramBtnList[i] != currentChParamBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChParamBtn = paramBtnList[i];
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(paramBtnList[i] != currentChParamBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChParamBtn = paramBtnList[i];
					SetFocus(currentChParamBtn);
					gCurrentFocusBtn = currentChParamBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndGateDisplay = CreateWindow(szGateDisplayChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChParam,SW_HIDE);
					//SetFocus(hWndGateDisplay);
					ShowWindow(hWndGateDisplay,SW_SHOW);
					
					gCurrentWnd	= hWndGateDisplay;					
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
	return CallWindowProc(OldBtnParamGateProc,Wnd,Msg,wParam,lParam);
}