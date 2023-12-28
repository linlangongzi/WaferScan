/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 制作曲线窗口源文件
 * Filename: makeCurveWin.cpp
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
#include "makeCurveWin.h"

#include "interfaceCmdID.h"

#include "resource.h"

HWND hWndMakeCurve = NULL;//制作曲线窗口句柄
HWND hBtnMakeCurveCreate = NULL;//创建按钮句柄
HWND hBtnMakeCurveModify = NULL;//修改按钮句柄
HWND makeCurveBtnList[2] ={NULL,NULL};//制作曲线窗口的按钮表
WNDPROC OldBtnMakeCurveCreateProc = NULL;//创建按钮默认处理过程
WNDPROC OldBtnMakeCurveModifyProc = NULL;//修改按钮默认处理过程
HWND currentMakeCurveBtn = NULL;//制作曲线窗中当前选中的按钮

static const int buttonNum = 2;//按钮控件数
extern HWND hWndMain;//主窗口句柄
extern HWND hWndTcgCurve;//曲线窗口句柄
extern HWND currentTcgCurveBtn;//曲线窗口中当前选中的按钮
extern HWND hWndCreateCurve;//新建DAC曲线窗口句柄
extern HWND hWndModifyCurve;//修改DAC曲线窗口句柄
extern HINSTANCE hInst;//程序实例句柄
extern BOOL gWaveDrawTcgFlag;//波形是否画TCG
extern HWND gCurrentFocusBtn;

extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//当前显示的子窗口

/**************************
*注册制作曲线窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerMakeCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) makeCurveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szMakeCurveChild;

	return RegisterClass(&wc);
}
/**************************
*制作曲线窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK makeCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;
	HWND hLableTemp;
//	LPDRAWITEMSTRUCT pdis;
//	TCHAR szHello[MAX_LOADSTRING];
//	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	switch (message) 
	{
		case WM_CREATE:	
			//画按钮控件
			hBtnMakeCurveCreate = CreateWindow(_T("BUTTON"),_T("Create"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_MAKECURVE_CREATE,hInst,NULL);
			hBtnMakeCurveModify = CreateWindow(_T("BUTTON"),_T("Make"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_MAKECURVE_MODIFY,hInst,NULL);
			
			makeCurveBtnList[0] = hBtnMakeCurveCreate;
			makeCurveBtnList[1] = hBtnMakeCurveModify;
			//按钮控件的自定义处理过程
			OldBtnMakeCurveCreateProc = (WNDPROC) GetWindowLong(hBtnMakeCurveCreate,GWL_WNDPROC);
			SetWindowLong(hBtnMakeCurveCreate,GWL_WNDPROC,(LONG)btnMakeCurveCreateProc);
			OldBtnMakeCurveModifyProc = (WNDPROC) GetWindowLong(hBtnMakeCurveModify,GWL_WNDPROC);
			SetWindowLong(hBtnMakeCurveModify,GWL_WNDPROC,(LONG)btnMakeCurveModifyProc);
			
			hLableTemp = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 2*BUTTON_5_EACH_HEIGHT,hWndMakeCurve,(HMENU)0,hInst,NULL);
			currentMakeCurveBtn = makeCurveBtnList[0];  //默认当前选中创建按钮
			SetFocus(currentMakeCurveBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentMakeCurveBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndMakeCurve, &ps);
			
			EndPaint(hWndMakeCurve, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDC_MAKECURVE_CREATE)
			{
			}
			if(LOWORD(wParam) == IDC_MAKECURVE_MODIFY)
			{
			}
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			makeCurve_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_CLOSE:		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载制作曲线窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int makeCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_makeCurveBtnBitmap[]=
	{
		{IDC_MAKECURVE_CREATE	,IDB_BITMAP_XINJIAN_NOSEL_R	 ,  IDB_BITMAP_XINJIAN_SELECT_R		},
		{IDC_MAKECURVE_MODIFY	,IDB_BITMAP_XIUGAI_NOSEL_R	 ,  IDB_BITMAP_XIUGAI_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_makeCurveBtnBitmap);
	return ReturnVal;
}
/**************************
*创建按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnMakeCurveCreateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					ShowWindow(hWndTcgCurve,SW_SHOW);
					SetFocus(currentTcgCurveBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndMakeCurve);
					hWndMakeCurve = 0;
					
					gCurrentFocusBtn = currentTcgCurveBtn;

					gCurrentWnd = hWndTcgCurve;
					
				}
			
				else if(wParam == VK_UP)
				{
					while(makeCurveBtnList[i] != currentMakeCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentMakeCurveBtn = makeCurveBtnList[i];
					SetFocus(currentMakeCurveBtn);
					gCurrentFocusBtn = currentMakeCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(makeCurveBtnList[i] != currentMakeCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentMakeCurveBtn = makeCurveBtnList[i];
					SetFocus(currentMakeCurveBtn);
					gCurrentFocusBtn = currentMakeCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndCreateCurve = CreateWindow(szCreateCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndMakeCurve,SW_HIDE);
				//	SetFocus(hWndCreateCurve);
					ShowWindow(hWndCreateCurve,SW_SHOW);
					UpdateWindow(hWndCreateCurve);

					gCurrentWnd = hWndCreateCurve;
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
		default:
			break;
	}
	return CallWindowProc(OldBtnMakeCurveCreateProc,Wnd,Msg,wParam,lParam);
}
/**************************
*修改按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnMakeCurveModifyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTcgCurve,SW_SHOW);
					SetFocus(currentTcgCurveBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndMakeCurve);
					hWndMakeCurve = 0;
					
					gCurrentFocusBtn = currentTcgCurveBtn;

					gCurrentWnd = hWndTcgCurve;
				}
			
				else if(wParam == VK_UP)
				{
					while(makeCurveBtnList[i] != currentMakeCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentMakeCurveBtn = makeCurveBtnList[i];
					SetFocus(currentMakeCurveBtn);
					gCurrentFocusBtn = currentMakeCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(makeCurveBtnList[i] != currentMakeCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentMakeCurveBtn = makeCurveBtnList[i];
					SetFocus(currentMakeCurveBtn);
					gCurrentFocusBtn = currentMakeCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndModifyCurve = CreateWindow(szModifyCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndMakeCurve,SW_HIDE);
				//	SetFocus(hWndModifyCurve);
					ShowWindow(hWndModifyCurve,SW_SHOW);
					UpdateWindow(hWndModifyCurve);
					gWaveDrawTcgFlag = TRUE;

					gCurrentWnd = hWndModifyCurve;
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
	return CallWindowProc(OldBtnMakeCurveModifyProc,Wnd,Msg,wParam,lParam);
}