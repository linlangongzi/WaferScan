/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ������Դ�ļ�
 * Filename: reportFormWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "reportFormWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"

HWND hWndReportForm = NULL;//�����ھ��
HWND hBtnReportFormNumber = NULL;//������ͺŰ�ť���
HWND hBtnReportFormSave = NULL;//���水ť���
HWND hBtnReportFormReview = NULL;//Ԥ����ť���
HWND hBtnReportFormPrint = NULL;//��ӡ��ť���
HWND reportFormBtnList[4] ={NULL,NULL,NULL,NULL};//�����ڵİ�ť��
WNDPROC OldBtnReportFormNumberProc = NULL;//������ͺŰ�ťĬ�ϴ������
WNDPROC OldBtnReportFormSaveProc = NULL;//���水ťĬ�ϴ������
WNDPROC OldBtnReportFormReviewProc = NULL;//Ԥ����ťĬ�ϴ������
WNDPROC OldBtnReportFormPrintProc = NULL;//��ӡ��ťĬ�ϴ������
HWND currentReportFormBtn = NULL;//�����е�ǰѡ�еİ�ť

//static HWND hLableNumber;//������ͺŰ�ť�еľ�̬�ı���
//static HBRUSH hStaticBrush = NULL;//��ˢ
static const int buttonNum = 4;//��ť�ؼ���

extern HINSTANCE hInst;//����ʵ�����
extern HWND hWndFileManage;//�ļ������ھ��
extern HWND currentFileManageBtn;//�ļ��������е�ǰѡ�еİ�ť
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���

/**************************
*ע�ᱨ����
* @reportForm    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerReportFormChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) reportFormChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szReportFormChild;

	return RegisterClass(&wc);
}
/**************************
*�����ڴ������
* @reportForm    hInstance[in]:
* @reportForm		message[in]:
* @reportForm		wParam [in]:
* @reportForm		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK reportFormChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		//	if(hStaticBrush == NULL)	
		//		hStaticBrush = CreateSolidBrush(BK_COLOR_WHITE);
			//����ť�ؼ�
			hBtnReportFormNumber = CreateWindow(_T("BUTTON"),_T("reportNumber"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_REPORTFORM_NUMBER,hInst,NULL);
			hBtnReportFormReview = CreateWindow(_T("BUTTON"),_T("reportReview"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_REPORTFORM_REVIEW,hInst,NULL);
			hBtnReportFormSave = CreateWindow(_T("BUTTON"),_T("reportSave"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_REPORTFORM_SAVE,hInst,NULL);
			hBtnReportFormPrint = CreateWindow(_T("BUTTON"),_T("reportPrint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 3*BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_REPORTFORM_PRINT,hInst,NULL);
					
			reportFormBtnList[0] = hBtnReportFormNumber;
			reportFormBtnList[1] = hBtnReportFormReview;
			reportFormBtnList[2] = hBtnReportFormSave;
			reportFormBtnList[3] = hBtnReportFormPrint;
			//��ť�ؼ����Զ��崦�����
			OldBtnReportFormNumberProc = (WNDPROC) GetWindowLong(hBtnReportFormNumber,GWL_WNDPROC);
			SetWindowLong(hBtnReportFormNumber,GWL_WNDPROC,(LONG)btnReportFormNumberProc);
			OldBtnReportFormReviewProc = (WNDPROC) GetWindowLong(hBtnReportFormReview,GWL_WNDPROC);
			SetWindowLong(hBtnReportFormReview,GWL_WNDPROC,(LONG)btnReportFormReviewProc);
			OldBtnReportFormSaveProc = (WNDPROC) GetWindowLong(hBtnReportFormSave,GWL_WNDPROC);
			SetWindowLong(hBtnReportFormSave,GWL_WNDPROC,(LONG)btnReportFormSaveProc);
			OldBtnReportFormPrintProc = (WNDPROC) GetWindowLong(hBtnReportFormPrint,GWL_WNDPROC);
			SetWindowLong(hBtnReportFormPrint,GWL_WNDPROC,(LONG)btnReportFormPrintProc);
			//����̬�ı���
		//	hLableNumber = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER|WS_BORDER,LABEL_START_X,
		//			LABEL_4_START_Y,LABEL_WIDTH,LABEL_HEIGHT,hBtnReportFormNumber,(HMENU)IDL_REPORTFORM_NUMBER,hInst,NULL);

			currentReportFormBtn = reportFormBtnList[0];//Ĭ�ϵ�ǰѡ�б�����ͺŰ�ť
			SetFocus(currentReportFormBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentReportFormBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndReportForm, &ps);
		
			EndPaint(hWndReportForm, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			reportForm_DrawButtonBitmap(hInst,pdis);
			if(pdis->hwndItem == hBtnReportFormNumber)
			{
			}
			break;
		case WM_CLOSE:
		
			break;
		case WM_DESTROY:
	//		if(hStaticBrush)
	//		{
	//			DeleteObject(hStaticBrush);
	//			hStaticBrush = NULL;
	//		}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*���ر����ڰ�ťͼƬ
* @reportForm    hInstance[in]:
* @reportForm		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int reportForm_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_reportFormBtnBitmap[]=
	{
		{IDC_REPORTFORM_NUMBER	,IDB_BITMAP_BAOBIAOHAOXINGHAO_NOSEL_R	 ,  IDB_BITMAP_BAOBIAOHAOXINGHAO_SELECT_R		},
		{IDC_REPORTFORM_REVIEW	,IDB_BITMAP_YULAN_NOSEL_R	 ,  IDB_BITMAP_YULAN_SELECT_R		},
		{IDC_REPORTFORM_SAVE	,IDB_BITMAP_BAOCUN_NOSEL_R	 ,  IDB_BITMAP_BAOCUN_SELECT_R		},
		{IDC_REPORTFORM_PRINT	,IDB_BITMAP_DAYIN_NOSEL_R ,  IDB_BITMAP_DAYIN_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_reportFormBtnBitmap);
	return ReturnVal;
}
/**************************
*������ͺŰ�ť�Զ��崦�����
* @reportForm    hInstance[in]:
* @reportForm		message[in]:
* @reportForm		wParam [in]:
* @reportForm		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnReportFormNumberProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFileManage,SW_SHOW);
					SetFocus(currentFileManageBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndReportForm);
					hWndReportForm = 0;					
					
					gCurrentFocusBtn = currentFileManageBtn;

					gCurrentWnd = hWndFileManage;
				}
			
				else if(wParam == VK_UP)
				{
					while(reportFormBtnList[i] != currentReportFormBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentReportFormBtn = reportFormBtnList[i];
					SetFocus(currentReportFormBtn);
					gCurrentFocusBtn = currentReportFormBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(reportFormBtnList[i] != currentReportFormBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentReportFormBtn = reportFormBtnList[i];
					SetFocus(currentReportFormBtn);
					gCurrentFocusBtn = currentReportFormBtn;
				}
				else if(wParam == VK_RETURN)
				{
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
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnReportFormNumberProc,Wnd,Msg,wParam,lParam);
}
/**************************
*���水ť�Զ��崦�����
* @reportForm    hInstance[in]:
* @reportForm		message[in]:
* @reportForm		wParam [in]:
* @reportForm		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnReportFormSaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFileManage,SW_SHOW);
					SetFocus(currentFileManageBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndReportForm);
					hWndReportForm = 0;					
					
					gCurrentFocusBtn = currentFileManageBtn;

					gCurrentWnd = hWndFileManage;
				}
			
				else if(wParam == VK_UP)
				{
					while(reportFormBtnList[i] != currentReportFormBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentReportFormBtn = reportFormBtnList[i];
					SetFocus(currentReportFormBtn);
					gCurrentFocusBtn = currentReportFormBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(reportFormBtnList[i] != currentReportFormBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentReportFormBtn = reportFormBtnList[i];
					SetFocus(currentReportFormBtn);
					gCurrentFocusBtn = currentReportFormBtn;
				}
				else if(wParam == VK_RETURN)
				{
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
	return CallWindowProc(OldBtnReportFormSaveProc,Wnd,Msg,wParam,lParam);
}
/**************************
*Ԥ����ť�Զ��崦�����
* @reportForm    hInstance[in]:
* @reportForm		message[in]:
* @reportForm		wParam [in]:
* @reportForm		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnReportFormReviewProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFileManage,SW_SHOW);
					SetFocus(currentFileManageBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndReportForm);
					hWndReportForm = 0;					
					
					gCurrentFocusBtn = currentFileManageBtn;

					gCurrentWnd = hWndFileManage;
				}
			
				else if(wParam == VK_UP)
				{
					while(reportFormBtnList[i] != currentReportFormBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentReportFormBtn = reportFormBtnList[i];
					SetFocus(currentReportFormBtn);
					gCurrentFocusBtn = currentReportFormBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(reportFormBtnList[i] != currentReportFormBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentReportFormBtn = reportFormBtnList[i];
					SetFocus(currentReportFormBtn);
					gCurrentFocusBtn = currentReportFormBtn;
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
	return CallWindowProc(OldBtnReportFormReviewProc,Wnd,Msg,wParam,lParam);
}
/**************************
*��ӡ��ť�Զ��崦�����
* @reportForm    hInstance[in]:
* @reportForm		message[in]:
* @reportForm		wParam [in]:
* @reportForm		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnReportFormPrintProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFileManage,SW_SHOW);
					SetFocus(currentFileManageBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndReportForm);
					hWndReportForm = 0;					
					
					gCurrentFocusBtn = currentFileManageBtn;

					gCurrentWnd = hWndFileManage;
				}
			
				else if(wParam == VK_UP)
				{
					while(reportFormBtnList[i] != currentReportFormBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentReportFormBtn = reportFormBtnList[i];
					SetFocus(currentReportFormBtn);
					gCurrentFocusBtn = currentReportFormBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(reportFormBtnList[i] != currentReportFormBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentReportFormBtn = reportFormBtnList[i];
					SetFocus(currentReportFormBtn);
					gCurrentFocusBtn = currentReportFormBtn;
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
	return CallWindowProc(OldBtnReportFormPrintProc,Wnd,Msg,wParam,lParam);
}
