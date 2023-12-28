/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 曲线窗口源文件
 * Filename: curveWin.cpp
 *
 * Spec: Trace Tag write here. If has not, OnOff this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "tcgCurveWin.h"
#include "makeCurveWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"

HWND hWndTcgCurve = NULL;//曲线窗口句柄
HWND hBtnTcgCurveMake = NULL;//制作按钮句柄
HWND hBtnTcgCurveOnOff = NULL;//开启/关闭按钮句柄
HWND tcgCurveBtnList[2] ={NULL,NULL};//曲线窗口的按钮表
WNDPROC OldBtnTcgCurveMakeProc = NULL;//制作按钮默认处理过程
WNDPROC OldBtnTcgCurveOnOffProc = NULL;//开启/关闭按钮默认处理过程
HWND currentTcgCurveBtn = NULL;//曲线窗中当前选中的按钮
static const int buttonNum = 2;//按钮控件数
//static HWND hLableCurveType;//DAC开启或关闭	
//static HWND hLableCurveOnOff;//DAC开启或关闭	
//static HBRUSH hStaticBrush = NULL;					//画刷
extern HWND hWndCurve;//参数设置窗口句柄
extern HWND hWndMakeCurve;//制作曲线窗口
extern HWND hWndMain;//主窗口
extern HWND currentCurveBtn;//曲线窗口中当前选中的按钮
extern HINSTANCE hInst;//程序实例句柄

extern ARS200FPGA_CH_DAC_REG *chCurrent_dac_reg;

extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//当前显示的子窗口

/**************************
*注册曲线窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerTcgCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));

	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) tcgCurveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szTcgCurveChild;

	return RegisterClass(&wc);
}

/**************************
*曲线窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK tcgCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:	
		//	if(hStaticBrush == NULL)	
		//		hStaticBrush = CreateSolidBrush(BK_COLOR_WHITE);
			//画按钮控件
			hBtnTcgCurveMake = CreateWindow(_T("BUTTON"),_T("Make"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 ,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_TCGCURVE_MAKE,hInst,NULL);
			hBtnTcgCurveOnOff = CreateWindow(_T("BUTTON"),_T("OnOff"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_TCGCURVE_ONOFF,hInst,NULL);
			
			tcgCurveBtnList[0] = hBtnTcgCurveMake;
			tcgCurveBtnList[1] = hBtnTcgCurveOnOff;
			//按钮控件的自定义处理过程
			OldBtnTcgCurveMakeProc = (WNDPROC) GetWindowLong(hBtnTcgCurveMake,GWL_WNDPROC);
			SetWindowLong(hBtnTcgCurveMake,GWL_WNDPROC,(LONG)btnTcgCurveMakeProc);
			OldBtnTcgCurveOnOffProc = (WNDPROC) GetWindowLong(hBtnTcgCurveOnOff,GWL_WNDPROC);
		    SetWindowLong(hBtnTcgCurveOnOff,GWL_WNDPROC,(LONG)btnTcgCurveOnOffProc);
			
			
			currentTcgCurveBtn = tcgCurveBtnList[0];  //默认当前选中制作按钮
			SetFocus(currentTcgCurveBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentTcgCurveBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:

			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			tcgCurve_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnTcgCurveOnOff)
			{
				if(chCurrent_dac_reg->wDACEnable == 0)
				{
					DrawLableText(pdis->hwndItem, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
				}
				else
				{
					DrawLableText(pdis->hwndItem, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
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
*加载曲线窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int tcgCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_tcgCurveBtnBitmap[]=
	{
		{IDC_TCGCURVE_MAKE	,IDB_BITMAP_ZHIZUO_NOSEL_R	 ,  IDB_BITMAP_ZHIZUO_SELECT_R		},
		{IDC_TCGCURVE_ONOFF	,IDB_BITMAP_KAIQIGUANBI4_NOSEL_R ,  IDB_BITMAP_KAIQIGUANBI4_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_tcgCurveBtnBitmap);
	return ReturnVal;
}
/**************************
*制作按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnTcgCurveMakeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndCurve,SW_SHOW);
					SetFocus(currentCurveBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndTcgCurve);
					hWndTcgCurve = 0;
					
					gCurrentFocusBtn = currentCurveBtn;

					gCurrentWnd = hWndCurve;
				}
			
				else if(wParam == VK_UP)
				{
					while(tcgCurveBtnList[i] != currentTcgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTcgCurveBtn = tcgCurveBtnList[i];
					SetFocus(currentTcgCurveBtn);
					gCurrentFocusBtn = currentTcgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tcgCurveBtnList[i] != currentTcgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTcgCurveBtn = tcgCurveBtnList[i];
					SetFocus(currentTcgCurveBtn);
					gCurrentFocusBtn = currentTcgCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndMakeCurve = CreateWindow(szMakeCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTcgCurve,SW_HIDE);
				//	SetFocus(hWndMakeCurve);
					ShowWindow(hWndMakeCurve,SW_SHOW);
					UpdateWindow(hWndMakeCurve);

					gCurrentWnd = hWndMakeCurve;
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
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnTcgCurveMakeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*开启/关闭按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnTcgCurveOnOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_PAINT:
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndCurve,SW_SHOW);
					SetFocus(currentCurveBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndTcgCurve);
					hWndTcgCurve = 0;
					
					gCurrentFocusBtn = currentCurveBtn;

					gCurrentWnd = hWndCurve;
				}
			
				else if(wParam == VK_UP)
				{
					while(tcgCurveBtnList[i] != currentTcgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTcgCurveBtn = tcgCurveBtnList[i];
					SetFocus(currentTcgCurveBtn);
					gCurrentFocusBtn = currentTcgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tcgCurveBtnList[i] != currentTcgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTcgCurveBtn = tcgCurveBtnList[i];
					SetFocus(currentTcgCurveBtn);
					gCurrentFocusBtn = currentTcgCurveBtn;
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					if(chCurrent_dac_reg->wDACEnable ==1)
					{
						chCurrent_dac_reg->wDACEnable = 0;
						DrawLableText(Wnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					}
					else
					{
						chCurrent_dac_reg->wDACEnable = 1;
						DrawLableText(Wnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					}
				}
				else if(wParam == VK_RETURN)
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
				else
				{
					;//
				}
			}
			break;
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnTcgCurveOnOffProc,Wnd,Msg,wParam,lParam);
}
