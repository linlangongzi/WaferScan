/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 参数窗口源文件
 * Filename: paramWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "const.h"
#include "paramWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"


static HWND hLableDeviceType;			//设备型号
static HWND hLableHeadType;				//探头型号
static HWND hLableShiKuai;				//试块
static HWND hLableName;					//被检工件名称


/**************************
*注册探伤报表预览窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerReportFormViewChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(255, 255, 255));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) paramChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szReportFormViewChild;

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
LRESULT CALLBACK reportFormViewChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
// 			hLableDeviceType = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER|WS_BORDER,0,
// 						0,LABEL_WIDTH,LABEL_HEIGHT,hWnd,(HMENU)0,hInst,NULL);
// 			hLableHeadType = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER|WS_BORDER,LABEL_START_X,
// 						LABEL_5_START_Y,LABEL_WIDTH,LABEL_HEIGHT,hWnd,(HMENU)0,hInst,NULL);
// 			hLableShiKuai = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER|WS_BORDER,LABEL_START_X,
// 						LABEL_5_START_Y,LABEL_WIDTH,LABEL_HEIGHT,hWnd,(HMENU)0,hInst,NULL);
// 			hLableName = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER|WS_BORDER,LABEL_START_X,
// 						LABEL_5_START_Y,LABEL_WIDTH,LABEL_HEIGHT,hWnd,(HMENU)0,hInst,NULL);
// 					
			break;
		case WM_PAINT:
// 			hdc = BeginPaint(hWndParam, &ps);
// 
// 			drawSrting(hWnd, _T("探伤报告"), 40, STATIC_TEXT_COLOR, 
// 								300, 0,100, 40);
// 			
// 			EndPaint(hWndParam, &ps);
			break;
		case WM_CLOSE:
		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

