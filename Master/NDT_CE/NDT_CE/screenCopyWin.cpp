/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 拷屏窗口源文件
 * Filename: screenCopyWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "screenCopyWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"

HWND hWndScreenCopy = NULL;//拷屏窗口句柄
HWND hBtnScreenCopyNumber = NULL;//图号按钮句柄
HWND hBtnScreenCopySave = NULL;//保存按钮句柄
HWND hBtnScreenCopyPrint = NULL;//打印按钮句柄
HWND hBtnScreenCopyDelete = NULL;//删除按钮句柄
HWND screenCopyBtnList[4] ={NULL,NULL,NULL,NULL};//拷屏窗口的按钮表
WNDPROC OldBtnScreenCopyNumberProc = NULL;//拷屏号型号按钮默认处理过程
WNDPROC OldBtnScreenCopySaveProc = NULL;//保存按钮默认处理过程
WNDPROC OldBtnScreenCopyPrintProc = NULL;//打印按钮默认处理过程
WNDPROC OldBtnScreenCopyDeleteProc = NULL;//删除按钮默认处理过程
HWND currentScreenCopyBtn = NULL;//拷屏窗中当前选中的按钮
 
//static HWND hLableNumber;//图号按钮中的静态文本框
//static HBRUSH hStaticBrush = NULL;//画刷
static const int buttonNum = 4;//按钮控件数
DWORD pictureNum = 1;
extern HINSTANCE hInst;//程序实例句柄
extern HWND hWndFileManage;//文件管理窗口句柄
extern HWND currentFileManageBtn;//文件管理窗口中当前选中的按钮
extern HANDLE hDrawEvent;				// 画图事件
extern HWND hWndMain;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad;
extern RECEIVE_PARAM_DATA ReceiveParam_ad;
extern PIC_DATA gPicData;						// 图像数据
extern TCHAR FileDirectory[MAX_PATH];
extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//当前显示的子窗口
extern WORD gwDeviceTypeValue;
extern HWND hStatusBar;					// 状态栏句柄

/**************************
*注册拷屏窗口
* @screenCopy    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
void displayPictureNum(HWND hWnd, DWORD dwNum)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	TCHAR szFilePath[30];
	swprintf(szFilePath, _T("%s%d.bmp"), FileDirectory, dwNum);
	
	if(GetFileAttributes(szFilePath)  == 0xFFFFFFFF)				
		swprintf(strText, _T("*%d"), dwNum);
	else
		swprintf(strText, _T("%d"), dwNum);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}
ATOM registerScreenCopyChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) screenCopyChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szScreenCopyChild;

	return RegisterClass(&wc);
}
/**************************
*拷屏窗口处理过程
* @screenCopy    hInstance[in]:
* @screenCopy		message[in]:
* @screenCopy		wParam [in]:
* @screenCopy		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/

LRESULT CALLBACK screenCopyChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			hBtnScreenCopyNumber = CreateWindow(_T("BUTTON"),_T("screenCopyNumber"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_SCREENCOPY_NUMBER,hInst,NULL);			
			hBtnScreenCopySave = CreateWindow(_T("BUTTON"),_T("screenCopySave"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_SCREENCOPY_SAVE,hInst,NULL);
			hBtnScreenCopyPrint = CreateWindow(_T("BUTTON"),_T("screenCopyPrint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_SCREENCOPY_PRINT,hInst,NULL);
			hBtnScreenCopyDelete = CreateWindow(_T("BUTTON"),_T("screenCopyDelete"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 3*BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_SCREENCOPY_DELETE,hInst,NULL);
					
			screenCopyBtnList[0] = hBtnScreenCopyNumber;
			screenCopyBtnList[1] = hBtnScreenCopySave;
			screenCopyBtnList[2] = hBtnScreenCopyPrint;
			screenCopyBtnList[3] = hBtnScreenCopyDelete;
			//按钮控件的自定义处理过程
			OldBtnScreenCopyNumberProc = (WNDPROC) GetWindowLong(hBtnScreenCopyNumber,GWL_WNDPROC);
			SetWindowLong(hBtnScreenCopyNumber,GWL_WNDPROC,(LONG)btnScreenCopyNumberProc);
			OldBtnScreenCopySaveProc = (WNDPROC) GetWindowLong(hBtnScreenCopySave,GWL_WNDPROC);
			SetWindowLong(hBtnScreenCopySave,GWL_WNDPROC,(LONG)btnScreenCopySaveProc);
			OldBtnScreenCopyPrintProc = (WNDPROC) GetWindowLong(hBtnScreenCopyPrint,GWL_WNDPROC);
			SetWindowLong(hBtnScreenCopyPrint,GWL_WNDPROC,(LONG)btnScreenCopyPrintProc);
			OldBtnScreenCopyDeleteProc = (WNDPROC) GetWindowLong(hBtnScreenCopyDelete,GWL_WNDPROC);
			SetWindowLong(hBtnScreenCopyDelete,GWL_WNDPROC,(LONG)btnScreenCopyDeleteProc);
			//画静态文本框
		//	hLableNumber = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER|WS_BORDER,LABEL_START_X,
		//			LABEL_4_START_Y,LABEL_WIDTH,LABEL_HEIGHT,hBtnScreenCopyNumber,(HMENU)IDL_SCREENCOPY_NUMBER,hInst,NULL);

			currentScreenCopyBtn = screenCopyBtnList[0];//默认当前选中拷屏号型号按钮
			SetFocus(currentScreenCopyBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentScreenCopyBtn;
				break;
		case WM_PAINT:
			hdc = BeginPaint(hWndScreenCopy, &ps);
			
			EndPaint(hWndScreenCopy, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			screenCopy_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnScreenCopyNumber)
			{
				displayPictureNum(pdis->hwndItem, pictureNum);
			}
			break;
		case WM_CLOSE:
		
			break;
		case WM_DESTROY:
//			if(hStaticBrush)
//			{
//				DeleteObject(hStaticBrush);
//				hStaticBrush = NULL;
//			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载拷屏窗口按钮图片
* @screenCopy    hInstance[in]:
* @screenCopy		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int screenCopy_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_screenCopyBtnBitmap[]=
	{
		{IDC_SCREENCOPY_NUMBER	,IDB_BITMAP_TUHAO_NOSEL_R	 ,  IDB_BITMAP_TUHAO_SELECT_R		},
		{IDC_SCREENCOPY_SAVE	,IDB_BITMAP_BAOCUN_NOSEL_R	 ,  IDB_BITMAP_BAOCUN_SELECT_R		},
		{IDC_SCREENCOPY_PRINT	,IDB_BITMAP_DAYIN_NOSEL_R	 ,  IDB_BITMAP_DAYIN_SELECT_R		},
		{IDC_SCREENCOPY_DELETE	,IDB_BITMAP_SHANCHU_NOSEL_R ,  IDB_BITMAP_SHANCHU_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_screenCopyBtnBitmap);
	return ReturnVal;
}
/**************************
*拷屏号型号按钮自定义处理过程
* @screenCopy    hInstance[in]:
* @screenCopy		message[in]:
* @screenCopy		wParam [in]:
* @screenCopy		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnScreenCopyNumberProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_PAINT:
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFileManage,SW_SHOW);
					SetFocus(currentFileManageBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndScreenCopy);
					hWndScreenCopy = 0;
					
					gCurrentFocusBtn = currentFileManageBtn;

					gCurrentWnd = hWndFileManage;
				}
			
				else if(wParam == VK_UP)
				{
					while(screenCopyBtnList[i] != currentScreenCopyBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentScreenCopyBtn = screenCopyBtnList[i];
					SetFocus(currentScreenCopyBtn);
					gCurrentFocusBtn = currentScreenCopyBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(screenCopyBtnList[i] != currentScreenCopyBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentScreenCopyBtn = screenCopyBtnList[i];
					SetFocus(currentScreenCopyBtn);
					gCurrentFocusBtn = currentScreenCopyBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(pictureNum != 1)
						pictureNum--;
					else
					{
						if((gwDeviceTypeValue == 0x2010) || (gwDeviceTypeValue == 0x2020))
							pictureNum = 100;
						else
							pictureNum = 1000;
					}
					
					displayPictureNum(Wnd, pictureNum);
				}
				else if(wParam == VK_RIGHT)
				{
					if((gwDeviceTypeValue == 0x2010) || (gwDeviceTypeValue == 0x2020))
					{
						if(pictureNum < 100)
							pictureNum++;
						else
							pictureNum = 1;
					}
					else
					{
						if(pictureNum < 1000)
							pictureNum++;
						else
							pictureNum = 1;
					}

					displayPictureNum(Wnd, pictureNum);
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
//		case WM_CTLCOLORSTATIC:
//			SetBkMode((HDC)wParam,TRANSPARENT);
//			SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
//			return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnScreenCopyNumberProc,Wnd,Msg,wParam,lParam);
}
/**************************
*保存按钮自定义处理过程
* @screenCopy    hInstance[in]:
* @screenCopy		message[in]:
* @screenCopy		wParam [in]:
* @screenCopy		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnScreenCopySaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndScreenCopy);
					hWndScreenCopy = 0;
					
					gCurrentFocusBtn = currentFileManageBtn;

					gCurrentWnd = hWndFileManage;
				}
			
				else if(wParam == VK_UP)
				{
					while(screenCopyBtnList[i] != currentScreenCopyBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentScreenCopyBtn = screenCopyBtnList[i];
					SetFocus(currentScreenCopyBtn);
					gCurrentFocusBtn = currentScreenCopyBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(screenCopyBtnList[i] != currentScreenCopyBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentScreenCopyBtn = screenCopyBtnList[i];
					SetFocus(currentScreenCopyBtn);
					gCurrentFocusBtn = currentScreenCopyBtn;
				}
				else if(wParam == VK_RETURN)
				{	
					TCHAR szFilePath[30];
					swprintf(szFilePath, _T("%s%d.bmp"), FileDirectory, pictureNum);
					ScreenCapture(szFilePath, 16, NULL);

					displayPictureNum(hBtnScreenCopyNumber, pictureNum);
				/*	//保存波形数据
					WORD dataTemp[DATA_SIZE + 2];
					ResetEvent(hDrawEvent);
					for (int i = 0;i < DATA_SIZE; i++)
					{
						dataTemp[i] = gPicData.data[i];
					}
					SetEvent(hDrawEvent);
					dataTemp[i + 1] = DisplaySet_ad.nRange;
					dataTemp[i + 2] = ReceiveParam_ad.fPlus;
					swprintf(szFilePath, _T("%d.bin"), pictureNum);
					saveDataToFile(szFilePath,dataTemp,2*(DATA_SIZE + 2));*/
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
	return CallWindowProc(OldBtnScreenCopySaveProc,Wnd,Msg,wParam,lParam);
}
/**************************
*打印按钮自定义处理过程
* @screenCopy    hInstance[in]:
* @screenCopy		message[in]:
* @screenCopy		wParam [in]:
* @screenCopy		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnScreenCopyPrintProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndScreenCopy);
					hWndScreenCopy = 0;
					
					gCurrentFocusBtn = currentFileManageBtn;

					gCurrentWnd = hWndFileManage;
				}
			
				else if(wParam == VK_UP)
				{
					while(screenCopyBtnList[i] != currentScreenCopyBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentScreenCopyBtn = screenCopyBtnList[i];
					SetFocus(currentScreenCopyBtn);
					gCurrentFocusBtn = currentScreenCopyBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(screenCopyBtnList[i] != currentScreenCopyBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentScreenCopyBtn = screenCopyBtnList[i];
					SetFocus(currentScreenCopyBtn);
					gCurrentFocusBtn = currentScreenCopyBtn;
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
	return CallWindowProc(OldBtnScreenCopyPrintProc,Wnd,Msg,wParam,lParam);
}
/**************************
*删除按钮自定义处理过程
* @screenCopy    hInstance[in]:
* @screenCopy		message[in]:
* @screenCopy		wParam [in]:
* @screenCopy		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnScreenCopyDeleteProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndScreenCopy);
					hWndScreenCopy = 0;
					
					gCurrentFocusBtn = currentFileManageBtn;
					
					gCurrentWnd = hWndFileManage;
				}
			
				else if(wParam == VK_UP)
				{
					while(screenCopyBtnList[i] != currentScreenCopyBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentScreenCopyBtn = screenCopyBtnList[i];
					SetFocus(currentScreenCopyBtn);
					gCurrentFocusBtn = currentScreenCopyBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(screenCopyBtnList[i] != currentScreenCopyBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentScreenCopyBtn = screenCopyBtnList[i];
					SetFocus(currentScreenCopyBtn);
					gCurrentFocusBtn = currentScreenCopyBtn;
				}
				else if(wParam == VK_LEFT)
				{
				}
				else if(wParam == VK_RIGHT)
				{
				}
				else if(wParam == VK_RETURN)
				{
					TCHAR szFilePath[30];
					swprintf(szFilePath, _T("%s%d.bmp"), FileDirectory, pictureNum);
					
					BOOL bRet = DeleteFile(szFilePath);

					if(!bRet)
					{
						PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("文件不存在"));
					}
					else
					{
						PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("文件删除成功"));
						displayPictureNum(hBtnScreenCopyNumber, pictureNum);
					}
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
	return CallWindowProc(OldBtnScreenCopyDeleteProc,Wnd,Msg,wParam,lParam);
}
