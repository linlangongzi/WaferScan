/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ���ߴ���Դ�ļ�
 * Filename: curveWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "curveWin.h"

#include "resource.h"
#include "interfaceCmdID.h"

#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"

HWND hWndCurve = NULL;//���ߴ��ھ��

static HWND hBtnCurveTcg = NULL;//TCG��ť���
static HWND hBtnCurveDac = NULL;//DAC��ť���
static HWND hBtnCurveAvg = NULL;//AVG��ť���
static HWND hBtnCurveQuMianXiuZheng = NULL;//����������ť���

static WNDPROC OldBtnCurveTcgProc = NULL;//TCG��ťĬ�ϴ������
static WNDPROC OldBtnCurveDacProc = NULL;//DAC��ťĬ�ϴ������
static WNDPROC OldBtnCurveAvgProc = NULL;//AVG��ťĬ�ϴ������
static WNDPROC OldBtnCurveQuMianXiuZhengProc = NULL;//����������ťĬ�ϴ������

static HWND curveBtnList[4] ={NULL,NULL,NULL,NULL};//���ߴ��ڵİ�ť��
HWND currentCurveBtn = NULL;//���ߴ����е�ǰѡ�еİ�ť

static const int buttonNum = 4;//��ť�ؼ���

extern HINSTANCE hInst;//����ʵ�����
extern HWND hWndMain;//�����ھ��

extern HWND gCurrentFocusBtn;
extern HWND hWndDgsCurve;
extern HWND hWndTcgCurve;
extern HWND hWndParam;
extern HWND currentParamBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;					//��ǰ��ʾ���Ӵ���
extern HWND hWndQuMianXiuZheng;				//�����������ھ��
extern HWND hWndAvgCurve;					//AVG���ߴ��ھ��

/**************************
*ע�������ߴ���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = (WNDPROC) curveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szCurveChild;

	return RegisterClass(&wc);
}
/**************************
*���ߴ��ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK curveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;
	switch (message) 
	{
		case WM_CREATE:
			hBtnCurveTcg = CreateWindow(_T("BUTTON"),_T("CurveTcg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0, MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CURVE_TCG,hInst,NULL);
			hBtnCurveDac = CreateWindow(_T("BUTTON"),_T("CurveDac"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CURVE_DAC,hInst,NULL);
			hBtnCurveAvg = CreateWindow(_T("BUTTON"),_T("CurveAvg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CURVE_AVG,hInst,NULL);
			hBtnCurveQuMianXiuZheng = CreateWindow(_T("BUTTON"),_T("CurveQuMianXiuZheng"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CURVE_QUMIANXIUZHENG,hInst,NULL);
					
			curveBtnList[0] = hBtnCurveTcg;
			curveBtnList[1] = hBtnCurveDac;
			curveBtnList[2] = hBtnCurveAvg;
			curveBtnList[3] = hBtnCurveQuMianXiuZheng;
			//��ť�ؼ����Զ��崦�����
			
			OldBtnCurveTcgProc = (WNDPROC) GetWindowLong(hBtnCurveTcg,GWL_WNDPROC);
			SetWindowLong(hBtnCurveTcg,GWL_WNDPROC,(LONG)btnCurveTcgProc);
			OldBtnCurveDacProc = (WNDPROC) GetWindowLong(hBtnCurveDac,GWL_WNDPROC);
			SetWindowLong(hBtnCurveDac,GWL_WNDPROC,(LONG)btnCurveDacProc);
			OldBtnCurveAvgProc = (WNDPROC) GetWindowLong(hBtnCurveAvg,GWL_WNDPROC);
			SetWindowLong(hBtnCurveAvg,GWL_WNDPROC,(LONG)btnCurveAvgProc);
			OldBtnCurveQuMianXiuZhengProc = (WNDPROC) GetWindowLong(hBtnCurveQuMianXiuZheng,GWL_WNDPROC);
			SetWindowLong(hBtnCurveQuMianXiuZheng,GWL_WNDPROC,(LONG)btnCurveQuMianXiuZhengProc);
			
			currentCurveBtn = curveBtnList[0];//Ĭ�ϵ�ǰѡ��tcg��ť
			SetFocus(currentCurveBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentCurveBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndCurve, &ps);
			// TODO: Add any drawing code here...
		//	RECT rt;
		//	GetClientRect(hWnd, &rt);
		//	DrawText(hdc, szHello, _tcslen(szHello), &rt, DT_CENTER);
			//����ť�ؼ�
			
			
			EndPaint(hWndCurve, &ps);
			break;
		case WM_KEYDOWN:
		/*	if(wParam == VK_ESCAPE)
			{
				DestroyWindow(hWndCurve);
				hWndCurve = 0;

				// songchenguang add start 2010-4-26
				SetMenuStateFlag(FALSE);
				// songchenguang add end 2010-4-26
			}*/
			break;
		case WM_DRAWITEM://�ػ���ť
			pdis=(LPDRAWITEMSTRUCT)lParam;
			curve_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_COMMAND:
			break;
		case WM_CLOSE:
		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*�������ߴ��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int curve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_curveBtnBitmap[]=
	{
		{IDC_CURVE_TCG	,IDB_BITMAP_TCGQUXIAN_NOSEL_R	 ,  IDB_BITMAP_TCGQUXIAN_SELECT_R		},
		{IDC_CURVE_DAC	,IDB_BITMAP_DACQUXIAN_NOSEL_R	 ,  IDB_BITMAP_DACQUXIAN_SELECT_R		},
		{IDC_CURVE_AVG	,IDB_BITMAP_AVGQUXIANG_NOSEL_R	 ,  IDB_BITMAP_AVGQUXIAN_SELECT_R		},
		{IDC_CURVE_QUMIANXIUZHENG	,IDB_BITMAP_QUMIANXIUZHENG_NOSEL_R	 ,  IDB_BITMAP_QUMIANXIUZHENG_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_curveBtnBitmap);
	return ReturnVal;
}
/**************************
*TCG��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCurveTcgProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndParam,SW_SHOW);
					SetFocus(currentParamBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndCurve);
					hWndCurve = 0;
					
					gCurrentFocusBtn = currentParamBtn;

					gCurrentWnd = hWndParam;
				}
			
				else if(wParam == VK_UP)
				{
					while(curveBtnList[i] != currentCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCurveBtn = curveBtnList[i];
					SetFocus(currentCurveBtn);
					gCurrentFocusBtn = currentCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(curveBtnList[i] != currentCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCurveBtn = curveBtnList[i];
					SetFocus(currentCurveBtn);
					gCurrentFocusBtn = currentCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndTcgCurve = CreateWindow(szTcgCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndCurve,SW_HIDE);
					//SetFocus(hWndTcgCurve);
					ShowWindow(hWndTcgCurve,SW_SHOW);
					UpdateWindow(hWndTcgCurve);

					gCurrentWnd = hWndTcgCurve;
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
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnCurveTcgProc,Wnd,Msg,wParam,lParam);
}
/**************************
*DAC��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCurveDacProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndParam,SW_SHOW);
					SetFocus(currentParamBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndCurve);
					hWndCurve = 0;
					
					gCurrentFocusBtn = currentParamBtn;

					gCurrentWnd = hWndParam;
				}
			
				else if(wParam == VK_UP)
				{
					while(curveBtnList[i] != currentCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCurveBtn = curveBtnList[i];
					SetFocus(currentCurveBtn);
					gCurrentFocusBtn = currentCurveBtn;
				}
				else if(wParam ==VK_DOWN)
				{
					while(curveBtnList[i] != currentCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCurveBtn = curveBtnList[i];
					SetFocus(currentCurveBtn);
					gCurrentFocusBtn = currentCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndDgsCurve = CreateWindow(szDgsCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndCurve,SW_HIDE);
					//SetFocus(hWndDgsCurve);
					ShowWindow(hWndDgsCurve,SW_SHOW);
					UpdateWindow(hWndDgsCurve);

					gCurrentWnd = hWndDgsCurve;
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
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnCurveDacProc,Wnd,Msg,wParam,lParam);
}
/**************************
*AVG��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCurveAvgProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndParam,SW_SHOW);
					SetFocus(currentParamBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndCurve);
					hWndCurve = 0;
					
					gCurrentFocusBtn = currentParamBtn;

					gCurrentWnd = hWndParam;
				}
			
				else if(wParam == VK_UP)
				{
					while(curveBtnList[i] != currentCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCurveBtn = curveBtnList[i];
					SetFocus(currentCurveBtn);
					gCurrentFocusBtn = currentCurveBtn;
				}
				else if(wParam ==VK_DOWN)
				{
					while(curveBtnList[i] != currentCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCurveBtn = curveBtnList[i];
					SetFocus(currentCurveBtn);
					gCurrentFocusBtn = currentCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndAvgCurve = CreateWindow(szAvgCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndCurve,SW_HIDE);
					//SetFocus(hWndAvgCurve);
					ShowWindow(hWndAvgCurve,SW_SHOW);
					UpdateWindow(hWndAvgCurve);

					gCurrentWnd = hWndAvgCurve;
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
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnCurveAvgProc,Wnd,Msg,wParam,lParam);
}

/**************************
*����������ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCurveQuMianXiuZhengProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndParam,SW_SHOW);
					SetFocus(currentParamBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndCurve);
					hWndCurve = 0;
					
					gCurrentFocusBtn = currentParamBtn;

					gCurrentWnd = hWndParam;
				}
			
				else if(wParam == VK_UP)
				{
					while(curveBtnList[i] != currentCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCurveBtn = curveBtnList[i];
					SetFocus(currentCurveBtn);
					gCurrentFocusBtn = currentCurveBtn;
				}
				else if(wParam ==VK_DOWN)
				{
					while(curveBtnList[i] != currentCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCurveBtn = curveBtnList[i];
					SetFocus(currentCurveBtn);
					gCurrentFocusBtn = currentCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndQuMianXiuZheng = CreateWindow(szQuMianXiuZhengChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndCurve,SW_HIDE);
					//SetFocus(hWndQuMianXiuZheng);
					ShowWindow(hWndQuMianXiuZheng,SW_SHOW);
					UpdateWindow(hWndQuMianXiuZheng);

					gCurrentWnd = hWndQuMianXiuZheng;
					
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
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnCurveQuMianXiuZhengProc,Wnd,Msg,wParam,lParam);
}

