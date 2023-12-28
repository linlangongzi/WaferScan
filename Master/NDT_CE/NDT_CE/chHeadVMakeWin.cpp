/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ̽ͷV�Ͳ���������������Դ�ļ�
 * Filename: chHeadVMakeWin.cpp
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

HWND hWndHeadVMake = NULL;//̽ͷV�Ͳ��������������ھ�
HWND hBtnHeadVMakeCurveNum = NULL;//�Զ���������Ű�ť���
HWND hBtnHeadVMakeCreate = NULL;//������ť���
HWND hBtnHeadVMakeModify = NULL;//�޸İ�ť���
HWND headVMakeBtnList[3] = {NULL,NULL,NULL};//̽ͷV�Ͳ��������������ڵİ�ť��
WNDPROC OldBtnHeadVMakeCurveNumProc = NULL;//�Զ���������Ű�ťĬ�ϴ������
WNDPROC OldBtnHeadVMakeCreateProc = NULL;//������ťĬ�ϴ������
WNDPROC OldBtnHeadVMakeModifyProc = NULL;//�޸İ�ťĬ�ϴ������
HWND currentHeadVMakeBtn = NULL;//̽ͷV�Ͳ��������������е�ǰѡ�еİ�ť
WORD gwHeadVCurveNum = 0;		//̽ͷV�Ͳ����������

static const int buttonNum = 3;		//��ť�ؼ���
extern HWND hWndMain;				//�����ھ��
extern HWND hWndChHeadVCreate;		//���̽ͷV�Ͳ������ߴ������ھ��
//extern HWND hWndChHeadVModify;	//���̽ͷV�Ͳ��������޸Ĵ��ھ��
extern HWND hWndModifyDacCurve;		//�޸�DAC���ߴ��ھ��
extern HINSTANCE hInst;				//����ʵ�����
extern HWND gCurrentFocusBtn;

extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���
extern HWND hWndChHeadVCompensate;						//���̽ͷV�Ͳ������ھ��
extern HWND currentChHeadVCompensateBtn;					//���̽ͷV�Ͳ������ڵ�ǰѡ�еİ�ť
extern TCHAR FileDirectory[MAX_PATH];
extern TCHAR strHeadVFileName[MAX_PATH];
extern WORD gwHeadVPointNum[HEAD_NUM + 10];

ATOM registerChHeadVMakeChild(HINSTANCE hInstance);
static LRESULT CALLBACK headVMakeChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int headVMake_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnHeadVMakeCurveNumProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnHeadVMakeCreateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnHeadVMakeModifyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
extern BOOL readHeadVFile(LPCTSTR lpszPathName);
/**************************
*��ʾ�Զ����������
* @param		hWnd[in]:���ھ��
* @param		wNum[in]:���
* @return   LRESULT :
* @since    1.00
***************************/
/*//�ֳɶ���ļ�
static void displayCurveNum(HWND hWnd ,WORD wNum)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wchar_t strFileName[20];
	memset(strFileName, 0, sizeof(strFileName));

	wsprintf(strFileName, _T("%d.headVCurve"), wNum + 10);

	memcpy(strHeadVFileName, FileDirectory, sizeof(strHeadVFileName));
	StringCchCat(strHeadVFileName, MAX_PATH, strFileName);

	if(GetFileAttributes(strHeadVFileName)  == 0xFFFFFFFF)
		wsprintf(strText, _T("*�Զ���%d"), wNum + 1);
	else
		wsprintf(strText, _T("�Զ���%d"), wNum + 1);
	
	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}
*/
static void displayCurveNum(HWND hWnd ,WORD wNum)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	readHeadVFile(strHeadVFileName);

	if(gwHeadVPointNum[wNum + HEAD_NUM]  == 0)
		wsprintf(strText, _T("*�Զ���%d"), wNum + 1);
	else
		wsprintf(strText, _T("�Զ���%d"), wNum + 1);
	
	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

/**************************
*ע��̽ͷV�Ͳ���������������
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChHeadVMakeChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) headVMakeChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChHeadVMakeChild;

	return RegisterClass(&wc);
}

/**************************
*̽ͷV�Ͳ��������������ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK headVMakeChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:	
			//����ť�ؼ�
			hBtnHeadVMakeCurveNum = CreateWindow(_T("BUTTON"),_T("CurveNum"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_HEADVMAKE_CURVENUM,hInst,NULL);
			hBtnHeadVMakeCreate = CreateWindow(_T("BUTTON"),_T("Create"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_HEADVMAKE_CREATE,hInst,NULL);
			hBtnHeadVMakeModify = CreateWindow(_T("BUTTON"),_T("Make"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2 * BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_HEADVMAKE_MODIFY,hInst,NULL);

			headVMakeBtnList[0] = hBtnHeadVMakeCurveNum;
			headVMakeBtnList[1] = hBtnHeadVMakeCreate;
			headVMakeBtnList[2] = hBtnHeadVMakeModify;
			//��ť�ؼ����Զ��崦�����
			OldBtnHeadVMakeCurveNumProc = (WNDPROC) GetWindowLong(hBtnHeadVMakeCurveNum,GWL_WNDPROC);
			SetWindowLong(hBtnHeadVMakeCurveNum,GWL_WNDPROC,(LONG)btnHeadVMakeCurveNumProc);
			OldBtnHeadVMakeCreateProc = (WNDPROC) GetWindowLong(hBtnHeadVMakeCreate,GWL_WNDPROC);
			SetWindowLong(hBtnHeadVMakeCreate,GWL_WNDPROC,(LONG)btnHeadVMakeCreateProc);
			OldBtnHeadVMakeModifyProc = (WNDPROC) GetWindowLong(hBtnHeadVMakeModify,GWL_WNDPROC);
			SetWindowLong(hBtnHeadVMakeModify,GWL_WNDPROC,(LONG)btnHeadVMakeModifyProc);
			
			currentHeadVMakeBtn = headVMakeBtnList[0];  //Ĭ�ϵ�ǰѡ�����Ͱ�ť
			SetFocus(currentHeadVMakeBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentHeadVMakeBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndHeadVMake, &ps);
			
			EndPaint(hWndHeadVMake, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDC_HEADVMAKE_CREATE)
			{
			}
			if(LOWORD(wParam) == IDC_HEADVMAKE_MODIFY)
			{
			}
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			headVMake_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnHeadVMakeCurveNum)
			{
				displayCurveNum(pdis->hwndItem, gwHeadVCurveNum);
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
*����̽ͷV�Ͳ��������������ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int headVMake_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_headVMakeBtnBitmap[]=
	{
		{IDC_HEADVMAKE_CURVENUM	,IDB_BITMAP_QUXIANXUHAO_NOSEL_R	 ,  IDB_BITMAP_QUXIANXUHAO_SELECT_R		},
		{IDC_HEADVMAKE_CREATE	,IDB_BITMAP_XINJIAN_NOSEL_R	 ,  IDB_BITMAP_XINJIAN_SELECT_R		},
		{IDC_HEADVMAKE_MODIFY	,IDB_BITMAP_XIUGAI_NOSEL_R	 ,  IDB_BITMAP_XIUGAI_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_headVMakeBtnBitmap);
	return ReturnVal;
}

/**************************
*�Զ�����������Զ��崦�����
* @param    Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnHeadVMakeCurveNumProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					ShowWindow(hWndChHeadVCompensate,SW_SHOW);
					SetFocus(currentChHeadVCompensateBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndHeadVMake);
					hWndHeadVMake = 0;

					gCurrentFocusBtn = currentChHeadVCompensateBtn;

					gCurrentWnd = hWndChHeadVCompensate;
					
				}
			
				else if(wParam == VK_UP)
				{
					while(headVMakeBtnList[i] != currentHeadVMakeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHeadVMakeBtn = headVMakeBtnList[i];
					SetFocus(currentHeadVMakeBtn);
					gCurrentFocusBtn = currentHeadVMakeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(headVMakeBtnList[i] != currentHeadVMakeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHeadVMakeBtn = headVMakeBtnList[i];
					SetFocus(currentHeadVMakeBtn);
					gCurrentFocusBtn = currentHeadVMakeBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(gwHeadVCurveNum > 0)
						gwHeadVCurveNum--;
					else
						gwHeadVCurveNum = 9;

					displayCurveNum(Wnd, gwHeadVCurveNum);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gwHeadVCurveNum < 9)
						gwHeadVCurveNum++;
					else
						gwHeadVCurveNum = 0;

					displayCurveNum(Wnd, gwHeadVCurveNum);
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
	return CallWindowProc(OldBtnHeadVMakeCurveNumProc,Wnd,Msg,wParam,lParam);
}

/**************************
*������ť�Զ��崦�����
* @param    Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnHeadVMakeCreateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					ShowWindow(hWndChHeadVCompensate,SW_SHOW);
					SetFocus(currentChHeadVCompensateBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndHeadVMake);
					hWndHeadVMake = 0;

					gCurrentFocusBtn = currentChHeadVCompensateBtn;

					gCurrentWnd = hWndChHeadVCompensate;
					
				}
			
				else if(wParam == VK_UP)
				{
					while(headVMakeBtnList[i] != currentHeadVMakeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHeadVMakeBtn = headVMakeBtnList[i];
					SetFocus(currentHeadVMakeBtn);
					gCurrentFocusBtn = currentHeadVMakeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(headVMakeBtnList[i] != currentHeadVMakeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHeadVMakeBtn = headVMakeBtnList[i];
					SetFocus(currentHeadVMakeBtn);
					gCurrentFocusBtn = currentHeadVMakeBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChHeadVCreate = CreateWindow(szChHeadVCreateChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndHeadVMake,SW_HIDE);
				//	SetFocus(hWndChHeadVCreate);
					ShowWindow(hWndChHeadVCreate,SW_SHOW);
					UpdateWindow(hWndChHeadVCreate);

					gCurrentWnd = hWndChHeadVCreate;
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnHeadVMakeCreateProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�޸İ�ť�Զ��崦�����
* @param    Wnd[in]:
* @param		Wnd[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnHeadVMakeModifyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChHeadVCompensate,SW_SHOW);
					SetFocus(currentChHeadVCompensateBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndHeadVMake);
					hWndHeadVMake = 0;

					gCurrentFocusBtn = currentChHeadVCompensateBtn;

					gCurrentWnd = hWndChHeadVCompensate;
				}
			
				else if(wParam == VK_UP)
				{
					while(headVMakeBtnList[i] != currentHeadVMakeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHeadVMakeBtn = headVMakeBtnList[i];
					SetFocus(currentHeadVMakeBtn);
					gCurrentFocusBtn = currentHeadVMakeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(headVMakeBtnList[i] != currentHeadVMakeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHeadVMakeBtn = headVMakeBtnList[i];
					SetFocus(currentHeadVMakeBtn);
					gCurrentFocusBtn = currentHeadVMakeBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChHeadVCreate = CreateWindow(szChHeadVCreateChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndHeadVMake,SW_HIDE);
				//	SetFocus(hWndChHeadVCreate);
					ShowWindow(hWndChHeadVCreate,SW_SHOW);
					UpdateWindow(hWndChHeadVCreate);

					gCurrentWnd = hWndChHeadVCreate;
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
	return CallWindowProc(OldBtnHeadVMakeModifyProc,Wnd,Msg,wParam,lParam);
}

