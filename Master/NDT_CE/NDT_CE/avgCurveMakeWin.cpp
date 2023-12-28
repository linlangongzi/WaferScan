/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ����AVG���ߴ���Դ�ļ�
 * Filename: avgCurveMakeWin.cpp
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
#include "interfaceCmdID.h"
#include "resource.h"

HWND hWndMakeAvgCurve  = NULL;//����AVG���ߴ��ھ��
HWND hBtnMakeAvgCurveType = NULL;//AVG���Ͱ�ť���
HWND hBtnMakeAvgCurveCreate = NULL;//AVG������ť���
HWND hBtnMakeAvgCurveModify = NULL;//AVG�޸İ�ť���
HWND makeAvgCurveBtnList[3] = {NULL,NULL,NULL};//����AVG���ߴ��ڵİ�ť��
WNDPROC OldBtnMakeAvgCurveTypeProc = NULL;//AVG���Ͱ�ťĬ�ϴ������
WNDPROC OldBtnMakeAvgCurveCreateProc = NULL;//������ťĬ�ϴ������
WNDPROC OldBtnMakeAvgCurveModifyProc = NULL;//�޸İ�ťĬ�ϴ������
HWND currentMakeAvgCurveBtn = NULL;//����AVG���ߴ��е�ǰѡ�еİ�ť
WORD gwAvgType = 0; //AVG���� 0:ƽ�׿� 1����ƽ��

static const int buttonNum = 3;//��ť�ؼ���

ATOM registerMakeAvgCurveChild(HINSTANCE hInstance);
static LRESULT CALLBACK makeAvgCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int makeAvgCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnMakeAvgCurveTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnMakeAvgCurveCreateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnMakeAvgCurveModifyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

extern HWND hWndMain;//�����ھ��
extern HWND hWndAvgCurve;//AVG���ߴ��ھ��
extern HWND currentAvgCurveBtn;//AVG���ߴ����е�ǰѡ�еİ�ť
extern HWND hWndCreateAvgCurve;//�½�AVG���ߴ��ھ��
extern HWND hWndModifyAvgCurve;//�޸�AVG���ߴ��ھ��
extern HINSTANCE hInst;//����ʵ�����
extern BOOL gWaveDrawAvgFlag;//�����Ƿ�AVG
extern HWND gCurrentFocusBtn;

extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG���߿��ر�־
extern WORD CurrentChannelNo;				// ��ǰͨ����

void displayDgsType(HWND hWnd, WORD wAvgType)
{
	if(wAvgType == 0)
	{
		DrawLableText(hWnd, _T("ƽ�׿�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else if(wAvgType == 1)
	{
		DrawLableText(hWnd, _T("��ƽ��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else
	{
		;//
	}
}


/**************************
*ע������AVG���ߴ���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerMakeAvgCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) makeAvgCurveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szMakeAvgCurveChild;

	return RegisterClass(&wc);
}
/**************************
*����AVG���ߴ��ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK makeAvgCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;
//	HWND hLableTemp;
//	LPDRAWITEMSTRUCT pdis;
//	TCHAR szHello[MAX_LOADSTRING];
//	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	switch (message) 
	{
		case WM_CREATE:	
			//����ť�ؼ�
			hBtnMakeAvgCurveType = CreateWindow(_T("BUTTON"),_T("avgType"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_MAKEAVGCURVE_TYPE,hInst,NULL);
			hBtnMakeAvgCurveCreate = CreateWindow(_T("BUTTON"),_T("avgCreate"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_MAKEAVGCURVE_CREATE,hInst,NULL);
			hBtnMakeAvgCurveModify = CreateWindow(_T("BUTTON"),_T("avgMake"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_MAKEAVGCURVE_MODIFY,hInst,NULL);
			
			makeAvgCurveBtnList[0] = hBtnMakeAvgCurveType;
			makeAvgCurveBtnList[1] = hBtnMakeAvgCurveCreate;
			makeAvgCurveBtnList[2] = hBtnMakeAvgCurveModify;
			//��ť�ؼ����Զ��崦�����
			OldBtnMakeAvgCurveTypeProc = (WNDPROC) GetWindowLong(hBtnMakeAvgCurveType,GWL_WNDPROC);
			SetWindowLong(hBtnMakeAvgCurveType,GWL_WNDPROC,(LONG)btnMakeAvgCurveTypeProc);
			OldBtnMakeAvgCurveCreateProc = (WNDPROC) GetWindowLong(hBtnMakeAvgCurveCreate,GWL_WNDPROC);
			SetWindowLong(hBtnMakeAvgCurveCreate,GWL_WNDPROC,(LONG)btnMakeAvgCurveCreateProc);
			OldBtnMakeAvgCurveModifyProc = (WNDPROC) GetWindowLong(hBtnMakeAvgCurveModify,GWL_WNDPROC);
			SetWindowLong(hBtnMakeAvgCurveModify,GWL_WNDPROC,(LONG)btnMakeAvgCurveModifyProc);
			
			currentMakeAvgCurveBtn = makeAvgCurveBtnList[0];  //Ĭ�ϵ�ǰѡ�д�����ť
			SetFocus(currentMakeAvgCurveBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentMakeAvgCurveBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndMakeAvgCurve, &ps);
			
			EndPaint(hWndMakeAvgCurve, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndAvgCurve,SW_SHOW);
				SetFocus(currentAvgCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

				DestroyWindow(hWndMakeAvgCurve);
				hWndMakeAvgCurve = 0;
				
				gCurrentFocusBtn = currentAvgCurveBtn;

				gCurrentWnd = hWndAvgCurve;
				
			}
			break;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDC_MAKEAVGCURVE_CREATE)
			{
			}
			else if(LOWORD(wParam) == IDC_MAKEAVGCURVE_MODIFY)
			{
			}
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			makeAvgCurve_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnMakeAvgCurveType)
			{
				displayDgsType(pdis->hwndItem, gwAvgType);
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
*��������AVG���ߴ��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int makeAvgCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_makeAvgCurveBtnBitmap[]=
	{
		{IDC_MAKEAVGCURVE_TYPE	,	IDB_BITMAP_QUXIANLEIXING_NOSEL_R	 ,  IDB_BITMAP_QUXIANLEIXING_SELECT_R	},
		{IDC_MAKEAVGCURVE_CREATE	,IDB_BITMAP_XINJIAN_NOSEL_R	 ,  IDB_BITMAP_XINJIAN_SELECT_R		},
		{IDC_MAKEAVGCURVE_MODIFY	,IDB_BITMAP_XIUGAI_NOSEL_R	 ,  IDB_BITMAP_XIUGAI_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_makeAvgCurveBtnBitmap);
	return ReturnVal;
}
/**************************
*AVG���Ͱ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnMakeAvgCurveTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(makeAvgCurveBtnList[i] != currentMakeAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentMakeAvgCurveBtn = makeAvgCurveBtnList[i];
					SetFocus(currentMakeAvgCurveBtn);
					gCurrentFocusBtn = currentMakeAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(makeAvgCurveBtnList[i] != currentMakeAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentMakeAvgCurveBtn = makeAvgCurveBtnList[i];
					SetFocus(currentMakeAvgCurveBtn);
					gCurrentFocusBtn = currentMakeAvgCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					if(gwAvgType == 0)
						gwAvgType = 1;
					else
						gwAvgType = 0;

					displayDgsType(Wnd, gwAvgType);
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
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnMakeAvgCurveTypeProc,Wnd,Msg,wParam,lParam);
}

/**************************
*������ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnMakeAvgCurveCreateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(makeAvgCurveBtnList[i] != currentMakeAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentMakeAvgCurveBtn = makeAvgCurveBtnList[i];
					SetFocus(currentMakeAvgCurveBtn);
					gCurrentFocusBtn = currentMakeAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(makeAvgCurveBtnList[i] != currentMakeAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentMakeAvgCurveBtn = makeAvgCurveBtnList[i];
					SetFocus(currentMakeAvgCurveBtn);
					gCurrentFocusBtn = currentMakeAvgCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndCreateAvgCurve = CreateWindow(szCreateAvgCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndMakeAvgCurve,SW_HIDE);
					ShowWindow(hWndCreateAvgCurve,SW_SHOW);
					UpdateWindow(hWndCreateAvgCurve);

					gCurrentWnd = hWndCreateAvgCurve;
					gbAvgCurveFlag[CurrentChannelNo] = FALSE;
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
				else if(wParam == 'B')	// ����
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
	return CallWindowProc(OldBtnMakeAvgCurveCreateProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�޸İ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnMakeAvgCurveModifyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(makeAvgCurveBtnList[i] != currentMakeAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentMakeAvgCurveBtn = makeAvgCurveBtnList[i];
					SetFocus(currentMakeAvgCurveBtn);
					gCurrentFocusBtn = currentMakeAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(makeAvgCurveBtnList[i] != currentMakeAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentMakeAvgCurveBtn = makeAvgCurveBtnList[i];
					SetFocus(currentMakeAvgCurveBtn);
					gCurrentFocusBtn = currentMakeAvgCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndModifyAvgCurve = CreateWindow(szModifyAvgCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndMakeAvgCurve,SW_HIDE);
					ShowWindow(hWndModifyAvgCurve,SW_SHOW);
					UpdateWindow(hWndModifyAvgCurve);

					gCurrentWnd = hWndModifyAvgCurve;
					gbAvgCurveFlag[CurrentChannelNo] = FALSE;
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
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnMakeAvgCurveModifyProc,Wnd,Msg,wParam,lParam);
}

