/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * �¶Ȳ���������������Դ�ļ�
 * Filename: chTempCurveMakeWin.cpp
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

HWND hWndTempCurveMake = NULL;//�¶Ȳ��������������ھ�
HWND hBtnTempCurveMakeCurveNum = NULL;//�Զ���������Ű�ť���
HWND hBtnTempCurveMakeCreate = NULL;//������ť���
HWND hBtnTempCurveMakeModify = NULL;//�޸İ�ť���
HWND tempCurveMakeBtnList[3] ={NULL,NULL,NULL};//�¶Ȳ��������������ڵİ�ť��
WNDPROC OldBtnTempCurveMakeCurveNumProc = NULL;//�Զ���������Ű�ťĬ�ϴ������
WNDPROC OldBtnTempCurveMakeCreateProc = NULL;//������ťĬ�ϴ������
WNDPROC OldBtnTempCurveMakeModifyProc = NULL;//�޸İ�ťĬ�ϴ������
HWND currentTempCurveMakeBtn = NULL;//�¶Ȳ��������������е�ǰѡ�еİ�ť
WORD gwTempCurveNum = 0;		//�¶Ȳ����������

static const int buttonNum = 3;		//��ť�ؼ���

extern HWND hWndMain;				//�����ھ��
extern HWND hWndChTempCompensate;						//����¶Ȳ������ھ��
extern HWND currentChTempCompensateBtn;					//����¶Ȳ������ڵ�ǰѡ�еİ�ť
extern HWND hWndChTempCurveCreate;	//����¶Ȳ������ߴ������ھ��
//extern HWND hWndChTempCurveModify;	//����¶Ȳ��������޸Ĵ��ھ��

extern HINSTANCE hInst;				//����ʵ�����
extern HWND gCurrentFocusBtn;

extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���
extern TCHAR FileDirectory[MAX_PATH];
extern TCHAR strTempCurveFileName[MAX_PATH];
extern HWND gCurrentWnd;
extern WORD gwTempCurvePointNum[MATERIAL_NUM + 10];						//�¶Ȳ������ߵ�ĸ���


ATOM registerChTempCurveMakeChild(HINSTANCE hInstance);
static LRESULT CALLBACK tempCurveMakeChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int tempCurveMake_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnTempCurveMakeCurveNumProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnTempCurveMakeCreateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnTempCurveMakeModifyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
extern BOOL readTempCurveFile(LPCTSTR lpszPathName);


/**************************
*��ʾ�Զ����������
* @param		hWnd[in]:���ھ��
* @param		wNum[in]:���
* @return   LRESULT :
* @since    1.00
***************************/
/*
static void displayCurveNum(HWND hWnd ,float wNum)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wchar_t strFileName[20];
	memset(strFileName, 0, sizeof(strFileName));

	wsprintf(strFileName, _T("%d.tempCurve"), wNum);

	memcpy(strTempCurveFileName, FileDirectory, sizeof(strTempCurveFileName));
	StringCchCat(strTempCurveFileName, MAX_PATH, strFileName);

	if(GetFileAttributes(strTempCurveFileName)  == 0xFFFFFFFF)
		wsprintf(strText, _T("*�Զ���%d"), wNum + 1);
	else
		wsprintf(strText, _T("�Զ���%d"), wNum + 1);
	
	DrawLableText(hWnd, strText, 25, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}
*/
static void displayCurveNum(HWND hWnd ,WORD wNum)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	readTempCurveFile(strTempCurveFileName);

	if(gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]  == 0)
		wsprintf(strText, _T("*�Զ���%d"), wNum + 1);
	else
		wsprintf(strText, _T("�Զ���%d"), wNum + 1);
	
	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}
/**************************
*ע���¶Ȳ���������������
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChTempCurveMakeChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) tempCurveMakeChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChTempCurveMakeChild;

	return RegisterClass(&wc);
}

/**************************
*�¶Ȳ��������������ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK tempCurveMakeChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:	
			//����ť�ؼ�
			hBtnTempCurveMakeCurveNum = CreateWindow(_T("BUTTON"),_T("CurveNum"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_TEMPCURVEMAKE_CURVENUM,hInst,NULL);
			hBtnTempCurveMakeCreate = CreateWindow(_T("BUTTON"),_T("Create"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_TEMPCURVEMAKE_CREATE,hInst,NULL);
			hBtnTempCurveMakeModify = CreateWindow(_T("BUTTON"),_T("Make"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2 * BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_TEMPCURVEMAKE_MODIFY,hInst,NULL);

			tempCurveMakeBtnList[0] = hBtnTempCurveMakeCurveNum;
			tempCurveMakeBtnList[1] = hBtnTempCurveMakeCreate;
			tempCurveMakeBtnList[2] = hBtnTempCurveMakeModify;
			//��ť�ؼ����Զ��崦�����
			OldBtnTempCurveMakeCurveNumProc = (WNDPROC) GetWindowLong(hBtnTempCurveMakeCurveNum,GWL_WNDPROC);
			SetWindowLong(hBtnTempCurveMakeCurveNum,GWL_WNDPROC,(LONG)btnTempCurveMakeCurveNumProc);
			OldBtnTempCurveMakeCreateProc = (WNDPROC) GetWindowLong(hBtnTempCurveMakeCreate,GWL_WNDPROC);
			SetWindowLong(hBtnTempCurveMakeCreate,GWL_WNDPROC,(LONG)btnTempCurveMakeCreateProc);
			OldBtnTempCurveMakeModifyProc = (WNDPROC) GetWindowLong(hBtnTempCurveMakeModify,GWL_WNDPROC);
			SetWindowLong(hBtnTempCurveMakeModify,GWL_WNDPROC,(LONG)btnTempCurveMakeModifyProc);
			
			currentTempCurveMakeBtn = tempCurveMakeBtnList[0];  //Ĭ�ϵ�ǰѡ�����Ͱ�ť
			SetFocus(currentTempCurveMakeBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentTempCurveMakeBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndTempCurveMake, &ps);
			
			EndPaint(hWndTempCurveMake, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			tempCurveMake_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnTempCurveMakeCurveNum)
			{
				displayCurveNum(pdis->hwndItem, gwTempCurveNum);
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
*�����¶Ȳ��������������ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int tempCurveMake_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_tempCurveMakeBtnBitmap[]=
	{
		{IDC_TEMPCURVEMAKE_CURVENUM	,IDB_BITMAP_QUXIANXUHAO_NOSEL_R	 ,  IDB_BITMAP_QUXIANXUHAO_SELECT_R		},
		{IDC_TEMPCURVEMAKE_CREATE	,IDB_BITMAP_XINJIAN_NOSEL_R	 ,  IDB_BITMAP_XINJIAN_SELECT_R		},
		{IDC_TEMPCURVEMAKE_MODIFY	,IDB_BITMAP_XIUGAI_NOSEL_R	 ,  IDB_BITMAP_XIUGAI_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_tempCurveMakeBtnBitmap);
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
static LRESULT CALLBACK btnTempCurveMakeCurveNumProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					ShowWindow(hWndChTempCompensate,SW_SHOW);
					SetFocus(currentChTempCompensateBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndTempCurveMake);
					hWndTempCurveMake = 0;

					gCurrentFocusBtn = currentChTempCompensateBtn;

					gCurrentWnd = hWndChTempCompensate;
					
				}
			
				else if(wParam == VK_UP)
				{
					while(tempCurveMakeBtnList[i] != currentTempCurveMakeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTempCurveMakeBtn = tempCurveMakeBtnList[i];
					SetFocus(currentTempCurveMakeBtn);
					gCurrentFocusBtn = currentTempCurveMakeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tempCurveMakeBtnList[i] != currentTempCurveMakeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTempCurveMakeBtn = tempCurveMakeBtnList[i];
					SetFocus(currentTempCurveMakeBtn);
					gCurrentFocusBtn = currentTempCurveMakeBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(gwTempCurveNum > 0)
						gwTempCurveNum--;
					else
						gwTempCurveNum = 9;

					displayCurveNum(Wnd, gwTempCurveNum);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gwTempCurveNum < 9)
						gwTempCurveNum++;
					else
						gwTempCurveNum = 0;

					displayCurveNum(Wnd, gwTempCurveNum);
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
	return CallWindowProc(OldBtnTempCurveMakeCurveNumProc,Wnd,Msg,wParam,lParam);
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
static LRESULT CALLBACK btnTempCurveMakeCreateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					ShowWindow(hWndChTempCompensate,SW_SHOW);
					SetFocus(currentChTempCompensateBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndTempCurveMake);
					hWndTempCurveMake = 0;

					gCurrentFocusBtn = currentChTempCompensateBtn;

					gCurrentWnd = hWndChTempCompensate;
					
				}
			
				else if(wParam == VK_UP)
				{
					while(tempCurveMakeBtnList[i] != currentTempCurveMakeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTempCurveMakeBtn = tempCurveMakeBtnList[i];
					SetFocus(currentTempCurveMakeBtn);
					gCurrentFocusBtn = currentTempCurveMakeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tempCurveMakeBtnList[i] != currentTempCurveMakeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTempCurveMakeBtn = tempCurveMakeBtnList[i];
					SetFocus(currentTempCurveMakeBtn);
					gCurrentFocusBtn = currentTempCurveMakeBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChTempCurveCreate = CreateWindow(szChTempCurveCreateChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTempCurveMake,SW_HIDE);
				//	SetFocus(hWndChTempCurveCreate);
					ShowWindow(hWndChTempCurveCreate,SW_SHOW);
					UpdateWindow(hWndChTempCurveCreate);

					gCurrentWnd = hWndChTempCurveCreate;
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
	return CallWindowProc(OldBtnTempCurveMakeCreateProc,Wnd,Msg,wParam,lParam);
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
static LRESULT CALLBACK btnTempCurveMakeModifyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChTempCompensate,SW_SHOW);
					SetFocus(currentChTempCompensateBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndTempCurveMake);
					hWndTempCurveMake = 0;

					gCurrentFocusBtn = currentChTempCompensateBtn;

					gCurrentWnd = hWndChTempCompensate;
				}
			
				else if(wParam == VK_UP)
				{
					while(tempCurveMakeBtnList[i] != currentTempCurveMakeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTempCurveMakeBtn = tempCurveMakeBtnList[i];
					SetFocus(currentTempCurveMakeBtn);
					gCurrentFocusBtn = currentTempCurveMakeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tempCurveMakeBtnList[i] != currentTempCurveMakeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTempCurveMakeBtn = tempCurveMakeBtnList[i];
					SetFocus(currentTempCurveMakeBtn);
					gCurrentFocusBtn = currentTempCurveMakeBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChTempCurveCreate = CreateWindow(szChTempCurveCreateChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTempCurveMake,SW_HIDE);
				//	SetFocus(hWndChTempCurveCreate);
					ShowWindow(hWndChTempCurveCreate,SW_SHOW);
					UpdateWindow(hWndChTempCurveCreate);

					gCurrentWnd = hWndChTempCurveCreate;
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
	return CallWindowProc(OldBtnTempCurveMakeModifyProc,Wnd,Msg,wParam,lParam);
}

