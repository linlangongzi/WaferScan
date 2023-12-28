/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ��˾ͼ�괰��Դ�ļ�
 * Filename: logoWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "interfaceCmdID.h"

#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"

HWND hWndLogo = NULL;//��˾ͼ�괰�ھ��

static LRESULT CALLBACK logoChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

extern HWND gCurrentWnd;
extern HWND hStatusBar;
extern HWND hWndCraftParam;
extern HWND gCurrentFocusBtn;
extern CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// �����̲߳���
extern HMODULE hRes;							// ��Դ���
/**************************
*ע�ṫ˾ͼ�괰��
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerLogoChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(255, 255, 255));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) logoChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szLogoChild;

	return RegisterClass(&wc);
}
/**************************
*��˾ͼ�괰�ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK logoChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBITMAP hbm;
	BITMAP bm;
	HDC hMemDC;
	switch (message) 
	{
		case WM_CREATE:
			{
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			hMemDC = CreateCompatibleDC(hdc);
			hbm=LoadBitmap(hRes,MAKEINTRESOURCE(IDB_BITMAP_LOGO_R));

			SelectObject(hMemDC,hbm);
			GetObject(hbm,sizeof(BITMAP),&bm);
			BitBlt(hdc, 54, 169, bm.bmWidth, bm.bmHeight, hMemDC,0, 0, SRCCOPY);
			//StretchBlt(hdc,135,190,370,100,
			//	hMemDC,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
			DeleteObject(hbm);
			DeleteDC(hMemDC);
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				InitializeCriticalSection(&csCommand);
				EnterCriticalSection(&csCommand);
				thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
				LeaveCriticalSection(&csCommand);
				DeleteCriticalSection(&csCommand);
				
				ShowWindow(hStatusBar,SW_SHOW);
				ShowWindow(gCurrentWnd,SW_SHOW);
				SetFocus(gCurrentFocusBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
				
				DestroyWindow(hWndLogo);
				hWndLogo = 0;

				SetRedrawFlag(TRUE);
			}
			break;
		case WM_CLOSE:		
			break;
		case WM_DESTROY:
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
