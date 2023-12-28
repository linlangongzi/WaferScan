/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 文件管理窗口源文件
 * Filename: fileManageWin.cpp
 *
 * Spec: Trace Tag write here. If has not, ScreenCopy this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fileManageWin.h"

#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"

HWND hWndFileManage = NULL;//文件管理窗口句柄
HWND hBtnFileManageCraftParam = NULL;//工艺参数按钮句柄
HWND hBtnFileManageReportForm = NULL;//报表按钮句柄
HWND hBtnFileManageScreenCopy = NULL;//拷屏按钮句柄

HWND fileManageBtnList[3] = {NULL,NULL,NULL};//文件管理窗口的按钮表
WNDPROC OldBtnFileManageCraftParamProc = NULL;//工艺参数按钮默认处理过程
WNDPROC OldBtnFileManageReportFormProc = NULL;//报表按钮默认处理过程
WNDPROC OldBtnFileManageScreenCopyProc = NULL;//拷屏按钮默认处理过程
HWND currentFileManageBtn = NULL;//文件管理窗中当前选中的按钮

static const int buttonNum = 3;//按钮控件数

extern HWND hWndTsMenu;							//探伤菜单窗口句柄
extern HWND hWndChMenu;							//测厚菜单窗口句柄
extern HWND currentTsMenuBtn;					//探伤菜单窗口中当前选中的按钮
extern HWND currentChMenuBtn;					//测厚菜单窗口中当前选中的按钮

extern HINSTANCE hInst;//程序实例句柄
extern HWND hWndCraftParam;//工艺参数窗口句柄
extern HWND hWndReportForm;//报表窗口句柄
extern HWND hWndScreenCopy;//拷屏窗口句柄
extern HWND hWndMain;//主窗口
extern HWND gCurrentFocusBtn;

extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//当前显示的子窗口
/**************************
*注册文件管理窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerFileManageChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) fileManageChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szFileManageChild;

	return RegisterClass(&wc);
}
/**************************
*文件管理窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK fileManageChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			//画按钮控件
			hBtnFileManageCraftParam = CreateWindow(_T("BUTTON"),_T("CraftParam"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_FILEMANAGE_CRAFTPARAM,hInst,NULL);
			hBtnFileManageReportForm = CreateWindow(_T("BUTTON"),_T("ReportForm"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_FILEMANAGE_REPORTFORM,hInst,NULL);
			hBtnFileManageScreenCopy = CreateWindow(_T("BUTTON"),_T("ScreenCopy"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_FILEMANAGE_SCREENCOPY,hInst,NULL);
			
			fileManageBtnList[0] = hBtnFileManageCraftParam;
			fileManageBtnList[1] = hBtnFileManageReportForm;
			fileManageBtnList[2] = hBtnFileManageScreenCopy;
			//按钮控件的自定义处理过程
			OldBtnFileManageCraftParamProc = (WNDPROC) GetWindowLong(hBtnFileManageCraftParam,GWL_WNDPROC);
			SetWindowLong(hBtnFileManageCraftParam,GWL_WNDPROC,(LONG)btnFileManageCraftParamProc);
			OldBtnFileManageReportFormProc = (WNDPROC) GetWindowLong(hBtnFileManageReportForm,GWL_WNDPROC);
			SetWindowLong(hBtnFileManageReportForm,GWL_WNDPROC,(LONG)btnFileManageReportFormProc);
			OldBtnFileManageScreenCopyProc = (WNDPROC) GetWindowLong(hBtnFileManageScreenCopy,GWL_WNDPROC);
		    SetWindowLong(hBtnFileManageScreenCopy,GWL_WNDPROC,(LONG)btnFileManageScreenCopyProc);

			currentFileManageBtn = fileManageBtnList[0];//默认当前选中工艺参数按钮
			SetFocus(currentFileManageBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentFileManageBtn;

			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndFileManage, &ps);
			
			EndPaint(hWndFileManage, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				if(hWndTsMenu)
				{						
					ShowWindow(hWndTsMenu,SW_SHOW);
					SetFocus(currentTsMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndFileManage);
					hWndFileManage = 0;

					gCurrentFocusBtn = currentTsMenuBtn;
					
					gCurrentWnd = hWndTsMenu;
				}

				else if(hWndChMenu)
				{
					DestroyWindow(hWndFileManage);
					hWndFileManage = 0;
					ShowWindow(hWndChMenu,SW_SHOW);
					SetFocus(currentChMenuBtn);//上级菜单最后选中的按钮获得输入焦点
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd = hWndChMenu;
				}
				else
				{
					;//
				}
			}
			break;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDC_FILEMANAGE_CRAFTPARAM)
			{
				currentFileManageBtn = fileManageBtnList[0]; 
				SetFocus(currentFileManageBtn);
				gCurrentFocusBtn = currentFileManageBtn;
				hWndCraftParam = CreateWindow(szCraftParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
						MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
				ShowWindow(hWndFileManage,SW_HIDE);
			//	SetFocus(hWndCraftParam);
				ShowWindow(hWndCraftParam,SW_SHOW);
				UpdateWindow(hWndCraftParam);
			}
			if(LOWORD(wParam) == IDC_FILEMANAGE_REPORTFORM)
			{
				currentFileManageBtn = fileManageBtnList[0]; 
				SetFocus(currentFileManageBtn);
				gCurrentFocusBtn = currentFileManageBtn;
				hWndReportForm = CreateWindow(szReportFormChild, TEXT(""), WS_CHILD|WS_VISIBLE,
						MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
				ShowWindow(hWndFileManage,SW_HIDE);
			//	SetFocus(hWndReportForm);
				ShowWindow(hWndReportForm,SW_SHOW);
				UpdateWindow(hWndReportForm);
			}
			else if(LOWORD(wParam) == IDC_FILEMANAGE_SCREENCOPY)
			{
				currentFileManageBtn = fileManageBtnList[0]; 
				SetFocus(currentFileManageBtn);
				gCurrentFocusBtn = currentFileManageBtn;
				hWndScreenCopy = CreateWindow(szScreenCopyChild, TEXT(""), WS_CHILD|WS_VISIBLE,
						MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
				ShowWindow(hWndFileManage,SW_HIDE);
			//	SetFocus(hWndScreenCopy);
				ShowWindow(hWndScreenCopy,SW_SHOW);
				UpdateWindow(hWndScreenCopy);
			}
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			fileManage_DrawButtonBitmap(hInst,pdis);
			break;
	//	case WM_CTLCOLORSTATIC:
	//		return (LRESULT)hStaticBrush;
		case WM_CLOSE:		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载文件管理窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int fileManage_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_fileManageBtnBitmap[]=
	{
		{IDC_FILEMANAGE_CRAFTPARAM	,IDB_BITMAP_GONGYICANSHU_NOSEL_R	 ,  IDB_BITMAP_GONGYICANSHU_SELECT_R		},
		{IDC_FILEMANAGE_REPORTFORM	,IDB_BITMAP_BAOBIAO_NOSEL_R	 ,  IDB_BITMAP_BAOBIAO_SELECT_R		},
		{IDC_FILEMANAGE_SCREENCOPY	,IDB_BITMAP_KAOPIN_NOSEL_R ,  IDB_BITMAP_KAOPIN_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_fileManageBtnBitmap);
	return ReturnVal;
}
/**************************
*工艺参数按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFileManageCraftParamProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fileManageBtnList[i] != currentFileManageBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFileManageBtn = fileManageBtnList[i];
					SetFocus(currentFileManageBtn);
					gCurrentFocusBtn = currentFileManageBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fileManageBtnList[i] != currentFileManageBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFileManageBtn = fileManageBtnList[i];
					SetFocus(currentFileManageBtn);
					gCurrentFocusBtn = currentFileManageBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndCraftParam = CreateWindow(szCraftParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFileManage,SW_HIDE);
					//SetFocus(hWndCraftParam);
					ShowWindow(hWndCraftParam,SW_SHOW);
					UpdateWindow(hWndCraftParam);

					gCurrentWnd = hWndCraftParam;
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
	return CallWindowProc(OldBtnFileManageCraftParamProc,Wnd,Msg,wParam,lParam);
}
/**************************
*报表按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFileManageReportFormProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fileManageBtnList[i] != currentFileManageBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFileManageBtn = fileManageBtnList[i];
					SetFocus(currentFileManageBtn);
					gCurrentFocusBtn = currentFileManageBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fileManageBtnList[i] != currentFileManageBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFileManageBtn = fileManageBtnList[i];
					SetFocus(currentFileManageBtn);
					gCurrentFocusBtn = currentFileManageBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndReportForm = CreateWindow(szReportFormChild, TEXT(""), WS_CHILD|WS_VISIBLE,
						MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFileManage,SW_HIDE);
					//SetFocus(hWndReportForm);
					ShowWindow(hWndReportForm,SW_SHOW);
					UpdateWindow(hWndReportForm);

					gCurrentWnd = hWndReportForm;
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
	return CallWindowProc(OldBtnFileManageReportFormProc,Wnd,Msg,wParam,lParam);
}
/**************************
*拷屏按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFileManageScreenCopyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fileManageBtnList[i] != currentFileManageBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFileManageBtn = fileManageBtnList[i];
					SetFocus(currentFileManageBtn);
					gCurrentFocusBtn = currentFileManageBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fileManageBtnList[i] != currentFileManageBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFileManageBtn = fileManageBtnList[i];
					SetFocus(currentFileManageBtn);
					gCurrentFocusBtn = currentFileManageBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndScreenCopy = CreateWindow(szScreenCopyChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFileManage,SW_HIDE);
					//SetFocus(hWndScreenCopy);
					ShowWindow(hWndScreenCopy,SW_SHOW);
					UpdateWindow(hWndScreenCopy);

					gCurrentWnd = hWndScreenCopy;
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
	return CallWindowProc(OldBtnFileManageScreenCopyProc,Wnd,Msg,wParam,lParam);
}
