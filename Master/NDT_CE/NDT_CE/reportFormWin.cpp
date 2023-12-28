/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 报表窗口源文件
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

HWND hWndReportForm = NULL;//报表窗口句柄
HWND hBtnReportFormNumber = NULL;//报表号型号按钮句柄
HWND hBtnReportFormSave = NULL;//保存按钮句柄
HWND hBtnReportFormReview = NULL;//预览按钮句柄
HWND hBtnReportFormPrint = NULL;//打印按钮句柄
HWND reportFormBtnList[4] ={NULL,NULL,NULL,NULL};//报表窗口的按钮表
WNDPROC OldBtnReportFormNumberProc = NULL;//报表号型号按钮默认处理过程
WNDPROC OldBtnReportFormSaveProc = NULL;//保存按钮默认处理过程
WNDPROC OldBtnReportFormReviewProc = NULL;//预览按钮默认处理过程
WNDPROC OldBtnReportFormPrintProc = NULL;//打印按钮默认处理过程
HWND currentReportFormBtn = NULL;//报表窗中当前选中的按钮

//static HWND hLableNumber;//报表号型号按钮中的静态文本框
//static HBRUSH hStaticBrush = NULL;//画刷
static const int buttonNum = 4;//按钮控件数

extern HINSTANCE hInst;//程序实例句柄
extern HWND hWndFileManage;//文件管理窗口句柄
extern HWND currentFileManageBtn;//文件管理窗口中当前选中的按钮
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//当前显示的子窗口

/**************************
*注册报表窗口
* @reportForm    hInstance[in]:程序实例句柄
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
*报表窗口处理过程
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
			//画按钮控件
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
			//按钮控件的自定义处理过程
			OldBtnReportFormNumberProc = (WNDPROC) GetWindowLong(hBtnReportFormNumber,GWL_WNDPROC);
			SetWindowLong(hBtnReportFormNumber,GWL_WNDPROC,(LONG)btnReportFormNumberProc);
			OldBtnReportFormReviewProc = (WNDPROC) GetWindowLong(hBtnReportFormReview,GWL_WNDPROC);
			SetWindowLong(hBtnReportFormReview,GWL_WNDPROC,(LONG)btnReportFormReviewProc);
			OldBtnReportFormSaveProc = (WNDPROC) GetWindowLong(hBtnReportFormSave,GWL_WNDPROC);
			SetWindowLong(hBtnReportFormSave,GWL_WNDPROC,(LONG)btnReportFormSaveProc);
			OldBtnReportFormPrintProc = (WNDPROC) GetWindowLong(hBtnReportFormPrint,GWL_WNDPROC);
			SetWindowLong(hBtnReportFormPrint,GWL_WNDPROC,(LONG)btnReportFormPrintProc);
			//画静态文本框
		//	hLableNumber = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER|WS_BORDER,LABEL_START_X,
		//			LABEL_4_START_Y,LABEL_WIDTH,LABEL_HEIGHT,hBtnReportFormNumber,(HMENU)IDL_REPORTFORM_NUMBER,hInst,NULL);

			currentReportFormBtn = reportFormBtnList[0];//默认当前选中报表号型号按钮
			SetFocus(currentReportFormBtn);//选中按钮获得输入焦点
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
*加载报表窗口按钮图片
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
*报表号型号按钮自定义处理过程
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
					SetFocus(currentFileManageBtn);//上级菜单最后选中的按钮获得输入焦点

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
				else if(wParam == 'B')	// 冻结
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
*保存按钮自定义处理过程
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
					SetFocus(currentFileManageBtn);//上级菜单最后选中的按钮获得输入焦点

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
				else if(wParam == 'B')	// 冻结
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
*预览按钮自定义处理过程
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
					SetFocus(currentFileManageBtn);//上级菜单最后选中的按钮获得输入焦点

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
				else if(wParam == 'B')	// 冻结
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
*打印按钮自定义处理过程
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
					SetFocus(currentFileManageBtn);//上级菜单最后选中的按钮获得输入焦点

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
				else if(wParam == 'B')	// 冻结
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
