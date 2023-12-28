/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ������У׼����Դ�ļ�
 * Filename: chZeroAdjustWin.cpp
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


HWND hWndChZeroAdjust = NULL;						//������У׼���ھ��
HWND currentChZeroAdjustBtn = NULL;				//������У׼���ڵ�ǰѡ�еİ�ť	

static HWND hBtnChZeroAdjustSingle = NULL;				//����У׼��ť���
static HWND hBtnChZeroAdjustDouble = NULL;				//����У׼��ť���

static HWND chZeroAdjustBtnList[2] = {NULL,NULL};					//���У׼���ڵİ�ť��
static const WORD buttonNum = 2;					//��ť�ؼ���

static WNDPROC OldBtnChZeroAdjustSingleProc = NULL;				//����У׼��ťĬ�ϴ������
static WNDPROC OldBtnChZeroAdjustDoubleProc = NULL;				//����У׼��ťĬ�ϴ������


ATOM registerChZeroAdjustChild(HINSTANCE hInstance);
static LRESULT CALLBACK chZeroAdjustChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chZeroAdjust_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChZeroAdjustSingleProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChZeroAdjustDoubleProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//����ʵ�����

extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndChAdjust;		//���У׼���ھ��
extern HWND currentChAdjustBtn;	//���У׼�����е�ǰѡ�еİ�ť
extern HWND hWndChZeroSingle;					//��񵥵㷨���У׼���ھ��
extern HWND hWndChZeroDouble;						//������㷨���У׼���ھ��
extern HWND gCurrentWnd;

/**************************
*ע����У׼����
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChZeroAdjustChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chZeroAdjustChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChZeroAdjustChild;

	return RegisterClass(&wc);
}
/**************************
*���У׼���ڴ������
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chZeroAdjustChildProc(HWND hWnd, UINT message, 
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

			//����ť�ؼ�
			hBtnChZeroAdjustSingle = CreateWindow(_T("BUTTON"),_T("methodChZeroAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZEROADJUST_SINGLE,hInst,NULL);

			hBtnChZeroAdjustDouble = CreateWindow(_T("BUTTON"),_T("minThicknessChZeroAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZEROADJUST_DOUBLE,hInst,NULL);

					
			chZeroAdjustBtnList[0] = hBtnChZeroAdjustSingle;
			chZeroAdjustBtnList[1] = hBtnChZeroAdjustDouble;

			
			//��ť�ؼ����Զ��崦�����
			OldBtnChZeroAdjustSingleProc = (WNDPROC) GetWindowLong(hBtnChZeroAdjustSingle,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroAdjustSingle,GWL_WNDPROC,(LONG)btnChZeroAdjustSingleProc);
			
			OldBtnChZeroAdjustDoubleProc = (WNDPROC) GetWindowLong(hBtnChZeroAdjustDouble,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroAdjustDouble,GWL_WNDPROC,(LONG)btnChZeroAdjustDoubleProc);
		
			currentChZeroAdjustBtn = chZeroAdjustBtnList[0];//Ĭ�ϵ�ǰѡ��У׼������ť
			SetFocus(currentChZeroAdjustBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentChZeroAdjustBtn;

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
			chZeroAdjust_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_CLOSE:
		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*���ز��У׼���ڰ�ťͼƬ
* @ param    hInstance[in]:
* @ param    		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chZeroAdjust_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chZeroAdjustBtnBitmap[]=
	{
		{IDC_CHZEROADJUST_SINGLE	,IDB_BITMAP_DANDIANFA_NOSEL_R	 ,  IDB_BITMAP_DANDIANFA_SELECT_R		},
		{IDC_CHZEROADJUST_DOUBLE	,IDB_BITMAP_LIANGDIANFA_NOSEL_R	 ,  IDB_BITMAP_LIANGDIANFA_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chZeroAdjustBtnBitmap);
	return ReturnVal;
}
/**************************
*����У׼ť�Զ��崦�����
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChZeroAdjustSingleProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChAdjust,SW_SHOW);
					SetFocus(currentChAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChZeroAdjust);
					hWndChZeroAdjust = 0;

					gCurrentFocusBtn = currentChAdjustBtn;

					gCurrentWnd = hWndChAdjust;
				}
			
				else if(wParam == VK_UP)
				{
					while(chZeroAdjustBtnList[i] != currentChZeroAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroAdjustBtn = chZeroAdjustBtnList[i];
					SetFocus(currentChZeroAdjustBtn);
					gCurrentFocusBtn = currentChZeroAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chZeroAdjustBtnList[i] != currentChZeroAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroAdjustBtn = chZeroAdjustBtnList[i];
					SetFocus(currentChZeroAdjustBtn);
					gCurrentFocusBtn = currentChZeroAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChZeroSingle = CreateWindow(szChZeroSingleChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChZeroAdjust,SW_HIDE);
					
					ShowWindow(hWndChZeroSingle,SW_SHOW);
					UpdateWindow(hWndChZeroSingle);

					gCurrentWnd = hWndChZeroSingle;
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
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChZeroAdjustSingleProc,Wnd,Msg,wParam,lParam);
}
/**************************
*˫��У׼ť�Զ��崦�����
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChZeroAdjustDoubleProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChAdjust,SW_SHOW);
					SetFocus(currentChAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChZeroAdjust);
					hWndChZeroAdjust = 0;

					gCurrentFocusBtn = currentChAdjustBtn;

					gCurrentWnd = hWndChAdjust;
				}
			
				else if(wParam == VK_UP)
				{
					while(chZeroAdjustBtnList[i] != currentChZeroAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroAdjustBtn = chZeroAdjustBtnList[i];
					SetFocus(currentChZeroAdjustBtn);
					gCurrentFocusBtn = currentChZeroAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chZeroAdjustBtnList[i] != currentChZeroAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroAdjustBtn = chZeroAdjustBtnList[i];
					SetFocus(currentChZeroAdjustBtn);
					gCurrentFocusBtn = currentChZeroAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChZeroDouble = CreateWindow(szChZeroDoubleChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChZeroAdjust,SW_HIDE);
					
					ShowWindow(hWndChZeroDouble,SW_SHOW);
					UpdateWindow(hWndChZeroDouble);

					gCurrentWnd = hWndChZeroDouble;
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
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChZeroAdjustDoubleProc,Wnd,Msg,wParam,lParam);
}
