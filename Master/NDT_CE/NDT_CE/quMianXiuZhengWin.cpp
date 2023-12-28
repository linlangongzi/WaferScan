/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 曲面修正窗口源文件
 * Filename: quMianXiuZhengWin.cpp
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

HWND hWndQuMianXiuZheng = NULL;						//曲面修正窗口句柄
HWND hBtnQuMianXiuZhengOnOff = NULL;					//开关按钮句柄
HWND hBtnQuMianXiuZhengWaiJing = NULL;					//管材外径按钮句柄
HWND hBtnQuMianXiuZhengNeiJing = NULL;					//管材内径按钮句柄


HWND quMianXiuZhengBtnList[3] = {NULL, NULL, NULL};					//曲面修正窗口的按钮表
WNDPROC OldBtnQuMianXiuZhengOnOffProc = NULL;	//开关按钮默认处理过程
WNDPROC OldBtnQuMianXiuZhengWaiJingProc = NULL;		//管材外径按钮默认处理过程
WNDPROC OldBtnQuMianXiuZhengNeiJingProc = NULL;		//管材外径按钮默认处理过程

HWND currentQuMianXiuZhengBtn = NULL;					//曲面修正窗中当前选中的按钮

BOOL gbQuMianXiuZhengFlag = FALSE;			//曲面修正开关标志
float gfWaiJing = 10;						//管材外径
float gfNeiJing = 5;						//管材内径
static WORD buttonNum = 3;					//按钮控件数
static float fWaiJingStep = 1.0f;
static WORD wWaiJingStepIndex = 1;

ATOM registerQuMianXiuZhengChild(HINSTANCE hInstance);
static LRESULT CALLBACK quMianXiuZhengChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int quMianXiuZheng_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnQuMianXiuZhengOnOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnQuMianXiuZhengWaiJingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnQuMianXiuZhengNeiJingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//程序实例句柄
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;	
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;				//当前显示的子窗口	
extern HWND hWndSpecialFunction;		//专用功能窗口句柄
extern HWND currentSpecialFunctionBtn;	//专用功能窗中当前选中的按钮
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;					// 状态栏句柄
extern HWND hWndCurve;//曲线窗口句柄
extern HWND currentCurveBtn;//曲线窗口中当前选中的按钮
extern BOOL gbHotRange;		// 快捷键声程
extern BOOL gbHotDB;		// 快捷键增益

/**************************
*显示开关
* @param		hWnd[in]:窗口句柄
* @param		bSwitch[in]:开关
* @return   LRESULT :
* @since    1.00
***************************/
static void displaySwitch(HWND hWnd ,BOOL bSwitch)
{
	if(bSwitch)
		DrawLableText(hWnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	else
		DrawLableText(hWnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);		
}
/**************************
*显示管材外径
* @param		hWnd[in]:窗口句柄
* @param		fWaiJing[in]:外径值
* @return   LRESULT :
* @since    1.00
***************************/
static void displayZhiJing(HWND hWnd ,float fWaiJing)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fWaiJing);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

/**************************
*注册曲面修正窗口
* @QuMianXiuZheng    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerQuMianXiuZhengChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) quMianXiuZhengChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szQuMianXiuZhengChild;

	return RegisterClass(&wc);
}
/**************************
*曲面修正窗口处理过程
* @QuMianXiuZheng    hInstance[in]:
* @QuMianXiuZheng		message[in]:
* @QuMianXiuZheng		wParam[in]:
* @QuMianXiuZheng		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK quMianXiuZhengChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			hBtnQuMianXiuZhengWaiJing = CreateWindow(_T("BUTTON"),_T("waiJingQuMianXiuZheng"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUMIANXIUZHENG_WAIJING,hInst,NULL);

			hBtnQuMianXiuZhengNeiJing = CreateWindow(_T("BUTTON"),_T("waiJingQuMianXiuZheng"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUMIANXIUZHENG_NEIJING,hInst,NULL);

			hBtnQuMianXiuZhengOnOff = CreateWindow(_T("BUTTON"),_T("onOffQuMianXiuZheng"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
				0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUMIANXIUZHENG_ONOFF,hInst,NULL);
			
			quMianXiuZhengBtnList[0] = hBtnQuMianXiuZhengWaiJing;
			quMianXiuZhengBtnList[1] = hBtnQuMianXiuZhengNeiJing;
			quMianXiuZhengBtnList[2] = hBtnQuMianXiuZhengOnOff;
			
			//按钮控件的自定义处理过程
			
			OldBtnQuMianXiuZhengWaiJingProc = (WNDPROC) GetWindowLong(hBtnQuMianXiuZhengWaiJing,GWL_WNDPROC);
			SetWindowLong(hBtnQuMianXiuZhengWaiJing,GWL_WNDPROC,(LONG)btnQuMianXiuZhengWaiJingProc);
			
			OldBtnQuMianXiuZhengNeiJingProc = (WNDPROC) GetWindowLong(hBtnQuMianXiuZhengNeiJing,GWL_WNDPROC);
			SetWindowLong(hBtnQuMianXiuZhengNeiJing,GWL_WNDPROC,(LONG)btnQuMianXiuZhengNeiJingProc);
			
			OldBtnQuMianXiuZhengOnOffProc = (WNDPROC) GetWindowLong(hBtnQuMianXiuZhengOnOff,GWL_WNDPROC);
			SetWindowLong(hBtnQuMianXiuZhengOnOff,GWL_WNDPROC,(LONG)btnQuMianXiuZhengOnOffProc);
				
			currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[0];//默认当前选中开关按钮			
			SetFocus(currentQuMianXiuZhengBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentQuMianXiuZhengBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndCurve,SW_SHOW);
				SetFocus(currentCurveBtn);//上级菜单最后选中的按钮获得输入焦点

				DestroyWindow(hWndQuMianXiuZheng);
				hWndQuMianXiuZheng = 0;
				
				gCurrentFocusBtn = currentCurveBtn;

				gCurrentWnd = hWndCurve;

				swprintf(gszTextStep, _T(""));
				PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			}
			break;
		case WM_COMMAND:
				
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			quMianXiuZheng_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnQuMianXiuZhengOnOff)
			{
				displaySwitch(pdis->hwndItem, gbQuMianXiuZhengFlag);
			}
			else if(pdis->hwndItem == hBtnQuMianXiuZhengWaiJing)
			{
				displayZhiJing(pdis->hwndItem,gfWaiJing);
			}
			else if(pdis->hwndItem == hBtnQuMianXiuZhengNeiJing)
			{
				displayZhiJing(pdis->hwndItem,gfNeiJing);
			}
			else
			{
				;//
			}

			// 检测按钮句柄，显示相应的步长
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentQuMianXiuZhengBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnQuMianXiuZhengWaiJing)
						swprintf(gszTextStep, _T("%.1f"), fWaiJingStep);
					else if(pdis->hwndItem == hBtnQuMianXiuZhengNeiJing)
						swprintf(gszTextStep, _T("%.1f"), fWaiJingStep);
					else
						swprintf(gszTextStep, _T(""));

					PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
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
*加载曲面修正窗口按钮图片
* @QuMianXiuZheng    hInstance[in]:
* @QuMianXiuZheng		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int quMianXiuZheng_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal = 0;

	BTN_BMP_INFO tbl_QuMianXiuZhengBtnBitmap[]=
	{
		{IDC_QUMIANXIUZHENG_ONOFF	,IDB_BITMAP_KAIQIGUANBI4_NOSEL_R	 ,  IDB_BITMAP_KAIQIGUANBI4_SELECT_R		},
		{IDC_QUMIANXIUZHENG_WAIJING	,IDB_BITMAP_GUANCAIWAIJING_NOSEL_R	 ,  IDB_BITMAP_GUANCAIWAIJING_SELECT_R		},
		{IDC_QUMIANXIUZHENG_NEIJING	,IDB_BITMAP_GUANCAINEIJING_NOSEL_R	 ,  IDB_BITMAP_GUANCAINEIJING_SEL_R		},
		{	-1			,	-1					 ,	-1							}    
	};
		
	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_QuMianXiuZhengBtnBitmap);
		
	return ReturnVal;
}
/**************************
*开关按钮自定义处理过程
* @QuMianXiuZheng    hInstance[in]:
* @QuMianXiuZheng		message[in]:
* @QuMianXiuZheng		wParam[in]:
* @QuMianXiuZheng		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnQuMianXiuZhengOnOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(quMianXiuZhengBtnList[i] != currentQuMianXiuZhengBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[i];
					SetFocus(currentQuMianXiuZhengBtn);
					gCurrentFocusBtn = currentQuMianXiuZhengBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(quMianXiuZhengBtnList[i] != currentQuMianXiuZhengBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[i];
					SetFocus(currentQuMianXiuZhengBtn);
					gCurrentFocusBtn = currentQuMianXiuZhengBtn;
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{	
					gbQuMianXiuZhengFlag = !gbQuMianXiuZhengFlag;

					displaySwitch(Wnd, gbQuMianXiuZhengFlag);
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
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnQuMianXiuZhengOnOffProc,Wnd,Msg,wParam,lParam);
}
/**************************
*管材外径按钮自定义处理过程
* @QuMianXiuZheng    hInstance[in]:
* @QuMianXiuZheng		message[in]:
* @QuMianXiuZheng		wParam[in]:
* @QuMianXiuZheng		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnQuMianXiuZhengWaiJingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), fWaiJingStep);
			PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			break;
		case WM_KILLFOCUS:
			swprintf(gszTextStep, _T(""));
			PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
					while(quMianXiuZhengBtnList[i] != currentQuMianXiuZhengBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[i];
					SetFocus(currentQuMianXiuZhengBtn);
					gCurrentFocusBtn = currentQuMianXiuZhengBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(quMianXiuZhengBtnList[i] != currentQuMianXiuZhengBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[i];
					SetFocus(currentQuMianXiuZhengBtn);
					gCurrentFocusBtn = currentQuMianXiuZhengBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfWaiJing - fWaiJingStep >= MIN_GUANCAIWAIJING)
						gfWaiJing = gfWaiJing - fWaiJingStep;
					else
						gfWaiJing = MIN_GUANCAIWAIJING;
					displayZhiJing(Wnd,gfWaiJing);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfWaiJing + fWaiJingStep <= MAX_GUANCAIWAIJING)
						gfWaiJing = gfWaiJing + fWaiJingStep;
					else
						gfWaiJing = MAX_GUANCAIWAIJING;
					displayZhiJing(Wnd,gfWaiJing);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == 'F')
				{
					if(wWaiJingStepIndex < 3)
						wWaiJingStepIndex++;
					else
						wWaiJingStepIndex = 0;

					fWaiJingStep = AMENDSTEPS[wWaiJingStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fWaiJingStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnQuMianXiuZhengWaiJingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*管材内径按钮自定义处理过程
* @QuMianXiuZheng    hInstance[in]:
* @QuMianXiuZheng		message[in]:
* @QuMianXiuZheng		wParam[in]:
* @QuMianXiuZheng		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnQuMianXiuZhengNeiJingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(quMianXiuZhengBtnList[i] != currentQuMianXiuZhengBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[i];
					SetFocus(currentQuMianXiuZhengBtn);
					gCurrentFocusBtn = currentQuMianXiuZhengBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(quMianXiuZhengBtnList[i] != currentQuMianXiuZhengBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[i];
					SetFocus(currentQuMianXiuZhengBtn);
					gCurrentFocusBtn = currentQuMianXiuZhengBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfNeiJing - fWaiJingStep >= MIN_GUANCAIWAIJING)
						gfNeiJing = gfNeiJing - fWaiJingStep;
					else
						gfNeiJing = MIN_GUANCAIWAIJING;
					displayZhiJing(Wnd,gfNeiJing);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfNeiJing + fWaiJingStep <= gfWaiJing)
						gfNeiJing = gfNeiJing + fWaiJingStep;
					else
						gfNeiJing = gfWaiJing - 0.1f;
					displayZhiJing(Wnd,gfNeiJing);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == 'F')
				{
					if(wWaiJingStepIndex < 3)
						wWaiJingStepIndex++;
					else
						wWaiJingStepIndex = 0;

					fWaiJingStep = AMENDSTEPS[wWaiJingStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fWaiJingStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnQuMianXiuZhengNeiJingProc,Wnd,Msg,wParam,lParam);
}

