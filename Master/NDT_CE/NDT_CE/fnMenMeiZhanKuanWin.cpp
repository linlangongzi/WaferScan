/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 门内展宽窗口源文件
 * Filename: menMeiZhanKuanWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnMenMeiZhanKuanWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"


HWND hWndMenMeiZhanKuan = NULL;			//门内展宽窗口句柄
HWND hBtnMenMeiZhanKuanQianYan = NULL;		//门内展宽选择按钮句柄
HWND hBtnMenMeiZhanKuanHouYan = NULL;		//保存按钮句柄
HWND hBtnMenMeiZhanKuanOnOrOff = NULL;		//开启/关闭按钮句柄

HWND menMeiZhanKuanBtnList[3] = {NULL,NULL,NULL};		//门内展宽窗口的按钮表
WNDPROC OldBtnMenMeiZhanKuanQianYanProc = NULL;//门内展宽选择按钮默认处理过程
WNDPROC OldBtnMenMeiZhanKuanHouYanProc = NULL;		//保存按钮默认处理过程
WNDPROC OldBtnMenMeiZhanKuanOnOrOffProc = NULL;	//开启/关闭按钮默认处理过程

HWND currentMenMeiZhanKuanBtn = NULL;					//门内展宽窗中当前选中的按钮

static const int buttonNum = 3;		//按钮控件数

extern HINSTANCE hInst;		//程序实例句柄
// songchenguang add start 2010-06-24
BOOL gbZhanKuan = FALSE;
//BOOL gbDisplayZhanKuanLine = FALSE;
float gZhanKuanFront = 0.0f;
float gZhanKuanBack = 10.0f;
static float fZhanKuanStep = 1.0f;

CRITICAL_SECTION csZhanKuan;
CRITICAL_SECTION csZhanKuanValue;

extern CRITICAL_SECTION csRange;
extern float fRangeStart[2];
extern float fRangeEnd[2];
extern WORD CurrentChannelNo;
// songchenguang add end 2010-06-24
extern HWND hWndMain;
extern HWND hWndSpecialDisplay;
extern HWND currentSpecialDisplayBtn;
extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//当前显示的子窗口
/**************************
*注册门内展宽窗口
* @menMeiZhanKuan    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerMenMeiZhanKuanChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) menMeiZhanKuanChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szMenMeiZhanKuanChild;

	return RegisterClass(&wc);
}

void SetZhanKuanFront(HWND hWnd, const float& fFront)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	swprintf(strText, _T("%.2f"), fFront);

	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}


void SetZhanKuanBack(HWND hWnd, const float& fBack)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	swprintf(strText, _T("%.2f"), gZhanKuanBack);

	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

void SetZhuanKuanOpenFlag(HWND hWnd)
{
	InitializeCriticalSection(&csZhanKuan);
	EnterCriticalSection(&csZhanKuan);

	BOOL bZhanKuan = gbZhanKuan;
	LeaveCriticalSection(&csZhanKuan);
	DeleteCriticalSection(&csZhanKuan);

	if(bZhanKuan)
	{
		DrawLableText(hWnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
/*
		InitializeCriticalSection(&csZhanKuanValue);
		EnterCriticalSection(&csZhanKuanValue);
		
		gbDisplayZhanKuanLine = FALSE;
		LeaveCriticalSection(&csZhanKuanValue);
		DeleteCriticalSection(&csZhanKuanValue);
*/
	}
	else
	{
		DrawLableText(hWnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
/*
		InitializeCriticalSection(&csZhanKuanValue);
		EnterCriticalSection(&csZhanKuanValue);
		
		gbDisplayZhanKuanLine = TRUE;
		LeaveCriticalSection(&csZhanKuanValue);
		DeleteCriticalSection(&csZhanKuanValue);
*/
	}
}
/**************************
*门内展宽窗口处理过程
* @menMeiZhanKuan    hInstance[in]:
* @menMeiZhanKuan		message[in]:
* @menMeiZhanKuan		wParam[in]:
* @menMeiZhanKuan		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK menMeiZhanKuanChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			//画按钮控件
			hBtnMenMeiZhanKuanQianYan = CreateWindow(_T("BUTTON"),_T("qianYanMenMeiZhanKuan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_MENMEIZHANKUAN_QIANYAN,hInst,NULL);
			hBtnMenMeiZhanKuanHouYan = CreateWindow(_T("BUTTON"),_T("houYanMenMeiZhanKuan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_MENMEIZHANKUAN_HOUYAN,hInst,NULL);
			hBtnMenMeiZhanKuanOnOrOff = CreateWindow(_T("BUTTON"),_T("onOrOffMenMeiZhanKuan"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_MENMEIZHANKUAN_ONOROFF,hInst,NULL);
					
			menMeiZhanKuanBtnList[0] = hBtnMenMeiZhanKuanQianYan;
			menMeiZhanKuanBtnList[1] = hBtnMenMeiZhanKuanHouYan;
			menMeiZhanKuanBtnList[2] = hBtnMenMeiZhanKuanOnOrOff;
			
			//按钮控件的自定义处理过程
			OldBtnMenMeiZhanKuanQianYanProc = (WNDPROC) GetWindowLong(hBtnMenMeiZhanKuanQianYan,GWL_WNDPROC);
			SetWindowLong(hBtnMenMeiZhanKuanQianYan,GWL_WNDPROC,(LONG)btnMenMeiZhanKuanQianYanProc);
			
			OldBtnMenMeiZhanKuanHouYanProc = (WNDPROC) GetWindowLong(hBtnMenMeiZhanKuanHouYan,GWL_WNDPROC);
			SetWindowLong(hBtnMenMeiZhanKuanHouYan,GWL_WNDPROC,(LONG)btnMenMeiZhanKuanHouYanProc);
			
			OldBtnMenMeiZhanKuanOnOrOffProc = (WNDPROC) GetWindowLong(hBtnMenMeiZhanKuanOnOrOff,GWL_WNDPROC);
			SetWindowLong(hBtnMenMeiZhanKuanOnOrOff,GWL_WNDPROC,(LONG)btnMenMeiZhanKuanOnOrOffProc);
			
		
			currentMenMeiZhanKuanBtn = menMeiZhanKuanBtnList[0];//默认当前选中门内展宽选择按钮
			SetFocus(currentMenMeiZhanKuanBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentMenMeiZhanKuanBtn;
/*
			// songchenguang add start 2010-07-09
			InitializeCriticalSection(&csZhanKuanValue);
			EnterCriticalSection(&csZhanKuanValue);
			gbDisplayZhanKuanLine = TRUE;
			LeaveCriticalSection(&csZhanKuanValue);
			DeleteCriticalSection(&csZhanKuanValue);
			// songchenguang add end 2010-07-09
*/
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndMenMeiZhanKuan, &ps);
			
			EndPaint(hWndMenMeiZhanKuan, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			menMeiZhanKuan_DrawButtonBitmap(hInst,pdis);

			// songchenguang add start 2010-07-02
			if(pdis->hwndItem == hBtnMenMeiZhanKuanQianYan)
				SetZhanKuanFront(hBtnMenMeiZhanKuanQianYan, gZhanKuanFront);
			else if(pdis->hwndItem == hBtnMenMeiZhanKuanHouYan)
				SetZhanKuanBack(hBtnMenMeiZhanKuanHouYan, gZhanKuanBack);
			else if(pdis->hwndItem == hBtnMenMeiZhanKuanOnOrOff)
				SetZhuanKuanOpenFlag(hBtnMenMeiZhanKuanOnOrOff);
			// songchenguang add end 2010-07-02
			break;
		case WM_DESTROY:
			// songchenguang add start 2010-07-09
			InitializeCriticalSection(&csZhanKuan);
			EnterCriticalSection(&csZhanKuan);

			gbZhanKuan = FALSE;
			LeaveCriticalSection(&csZhanKuan);
			DeleteCriticalSection(&csZhanKuan);
/*
			InitializeCriticalSection(&csZhanKuanValue);
			EnterCriticalSection(&csZhanKuanValue);
			
			gbDisplayZhanKuanLine = FALSE;
			LeaveCriticalSection(&csZhanKuanValue);
			DeleteCriticalSection(&csZhanKuanValue);
*/			
			SetRedrawFlag(TRUE);
			// songchenguang add end 2010-07-09
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载门内展宽窗口按钮图片
* @menMeiZhanKuan    hInstance[in]:
* @menMeiZhanKuan		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int menMeiZhanKuan_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_menMeiZhanKuanBtnBitmap[]=
	{
		{IDC_MENMEIZHANKUAN_QIANYAN	,IDB_BITMAP_MENMEIZHANKUAN_QIANYAN_NOSEL_R	 ,  IDB_BITMAP_MENMEIZHANKUAN_QIANYAN_SELECT_R		},
		{IDC_MENMEIZHANKUAN_HOUYAN	,IDB_BITMAP_MENMEIZHANKUAN_HOUYAN_NOSEL_R	 ,  IDB_BITMAP_MENMEIZHANKUAN_HOUYAN_SELECT_R		},
		{IDC_MENMEIZHANKUAN_ONOROFF	,IDB_BITMAP_KAIQIGUANBI4_NOSEL_R ,  IDB_BITMAP_KAIQIGUANBI4_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_menMeiZhanKuanBtnBitmap);
	return ReturnVal;
}



/**************************
*门内展宽前沿按钮自定义处理过程
* @menMeiZhanKuan    hInstance[in]:
* @menMeiZhanKuan		message[in]:
* @menMeiZhanKuan		wParam[in]:
* @menMeiZhanKuan		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnMenMeiZhanKuanQianYanProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	InitializeCriticalSection(&csRange);
	EnterCriticalSection(&csRange);
	float fStart = fRangeStart[CurrentChannelNo];
	float fEnd = fRangeEnd[CurrentChannelNo];
	LeaveCriticalSection(&csRange);
	DeleteCriticalSection(&csRange);

	switch(Msg)
	{
	case WM_CREATE:
		InitializeCriticalSection(&csZhanKuanValue);
		EnterCriticalSection(&csZhanKuanValue);
		gZhanKuanFront = fStart;

		LeaveCriticalSection(&csZhanKuanValue);
		DeleteCriticalSection(&csZhanKuanValue);
		
		break;
		case WM_KEYDOWN:
			{
				

				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialDisplay,SW_SHOW);
					SetFocus(currentSpecialDisplayBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndMenMeiZhanKuan);
					hWndMenMeiZhanKuan = 0;

					gCurrentFocusBtn = currentSpecialDisplayBtn;

					gCurrentWnd = hWndSpecialDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(menMeiZhanKuanBtnList[i] != currentMenMeiZhanKuanBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentMenMeiZhanKuanBtn = menMeiZhanKuanBtnList[i];
					SetFocus(currentMenMeiZhanKuanBtn);
					gCurrentFocusBtn = currentMenMeiZhanKuanBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(menMeiZhanKuanBtnList[i] != currentMenMeiZhanKuanBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentMenMeiZhanKuanBtn = menMeiZhanKuanBtnList[i];
					SetFocus(currentMenMeiZhanKuanBtn);
					gCurrentFocusBtn = currentMenMeiZhanKuanBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				// songchenguang add start 2010-06-18
				else if(wParam == VK_LEFT)
				{
					InitializeCriticalSection(&csZhanKuanValue);
					EnterCriticalSection(&csZhanKuanValue);

					float fCha = gZhanKuanFront - fStart;
					// 左移
					if(gZhanKuanFront > fStart && fCha > fZhanKuanStep)
						gZhanKuanFront -= fZhanKuanStep;
					else
						gZhanKuanFront = fStart;

					LeaveCriticalSection(&csZhanKuanValue);
					DeleteCriticalSection(&csZhanKuanValue);

					SetZhanKuanFront(hWnd, gZhanKuanFront);

					SetRedrawFlag(TRUE);
				}
				else if(wParam == VK_RIGHT)
				{
					InitializeCriticalSection(&csZhanKuanValue);
					EnterCriticalSection(&csZhanKuanValue);
					// 右移
					float fCha = gZhanKuanBack - gZhanKuanFront;
					
					if(gZhanKuanFront < gZhanKuanBack && fCha > (fZhanKuanStep*2))
						gZhanKuanFront += fZhanKuanStep;
					else
						gZhanKuanFront = gZhanKuanBack - fZhanKuanStep*2;

					LeaveCriticalSection(&csZhanKuanValue);
					DeleteCriticalSection(&csZhanKuanValue);

					SetZhanKuanFront(hWnd, gZhanKuanFront);

					SetRedrawFlag(TRUE);
				}
				// songchenguang add end 2010-06-18.
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
	return CallWindowProc(OldBtnMenMeiZhanKuanQianYanProc,hWnd,Msg,wParam,lParam);
}

/**************************
* 后沿按钮自定义处理过程
* @menMeiZhanKuan    hInstance[in]:
* @menMeiZhanKuan		message[in]:
* @menMeiZhanKuan		wParam[in]:
* @menMeiZhanKuan		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnMenMeiZhanKuanHouYanProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	InitializeCriticalSection(&csRange);
	EnterCriticalSection(&csRange);
	float fStart = fRangeStart[CurrentChannelNo];
	float fEnd = fRangeEnd[CurrentChannelNo];
	LeaveCriticalSection(&csRange);
	DeleteCriticalSection(&csRange);

	switch(Msg)
	{
	case WM_CREATE:
		InitializeCriticalSection(&csZhanKuanValue);
		EnterCriticalSection(&csZhanKuanValue);

		gZhanKuanBack = fEnd;

		LeaveCriticalSection(&csZhanKuanValue);
		DeleteCriticalSection(&csZhanKuanValue);
		break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialDisplay,SW_SHOW);
					SetFocus(currentSpecialDisplayBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndMenMeiZhanKuan);
					hWndMenMeiZhanKuan = 0;

					gCurrentFocusBtn = currentSpecialDisplayBtn;

					gCurrentWnd = hWndSpecialDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(menMeiZhanKuanBtnList[i] != currentMenMeiZhanKuanBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentMenMeiZhanKuanBtn = menMeiZhanKuanBtnList[i];
					SetFocus(currentMenMeiZhanKuanBtn);
					gCurrentFocusBtn = currentMenMeiZhanKuanBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(menMeiZhanKuanBtnList[i] != currentMenMeiZhanKuanBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentMenMeiZhanKuanBtn = menMeiZhanKuanBtnList[i];
					SetFocus(currentMenMeiZhanKuanBtn);
					gCurrentFocusBtn = currentMenMeiZhanKuanBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				// songchenguang add start 2010-06-24.
				else if(wParam == VK_LEFT)
				{
					// 左移
					InitializeCriticalSection(&csZhanKuanValue);
					EnterCriticalSection(&csZhanKuanValue);

					float fCha = gZhanKuanBack - gZhanKuanFront;
					
					if(gZhanKuanBack > gZhanKuanFront && fCha > (fZhanKuanStep*2))
						gZhanKuanBack -= fZhanKuanStep;
					else
						gZhanKuanBack = gZhanKuanFront + fZhanKuanStep*2;

					LeaveCriticalSection(&csZhanKuanValue);
					DeleteCriticalSection(&csZhanKuanValue);

					SetZhanKuanBack(hWnd, gZhanKuanBack);

					SetRedrawFlag(TRUE);
				}
				else if(wParam == VK_RIGHT)
				{
					// 右移
					InitializeCriticalSection(&csZhanKuanValue);
					EnterCriticalSection(&csZhanKuanValue);
					
					float fCha = fEnd - gZhanKuanBack;
					
					if(gZhanKuanBack < fEnd && fCha > fZhanKuanStep)
						gZhanKuanBack += fZhanKuanStep;
					else
						gZhanKuanBack = fEnd;

					LeaveCriticalSection(&csZhanKuanValue);
					DeleteCriticalSection(&csZhanKuanValue);

					SetZhanKuanBack(hWnd, gZhanKuanBack);

					SetRedrawFlag(TRUE);
				}
				// songchenguang add end 2010-06-24.
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
	return CallWindowProc(OldBtnMenMeiZhanKuanHouYanProc,hWnd,Msg,wParam,lParam);
}

/**************************
*开启/关闭按钮自定义处理过程
* @menMeiZhanKuan    hInstance[in]:
* @menMeiZhanKuan		message[in]:
* @menMeiZhanKuan		wParam[in]:
* @menMeiZhanKuan		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnMenMeiZhanKuanOnOrOffProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialDisplay,SW_SHOW);
					SetFocus(currentSpecialDisplayBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndMenMeiZhanKuan);
					hWndMenMeiZhanKuan = 0;

					gCurrentFocusBtn = currentSpecialDisplayBtn;

					gCurrentWnd = hWndSpecialDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(menMeiZhanKuanBtnList[i] != currentMenMeiZhanKuanBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentMenMeiZhanKuanBtn = menMeiZhanKuanBtnList[i];
					SetFocus(currentMenMeiZhanKuanBtn);
					gCurrentFocusBtn = currentMenMeiZhanKuanBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(menMeiZhanKuanBtnList[i] != currentMenMeiZhanKuanBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentMenMeiZhanKuanBtn = menMeiZhanKuanBtnList[i];
					SetFocus(currentMenMeiZhanKuanBtn);
					gCurrentFocusBtn = currentMenMeiZhanKuanBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					// songchenguang add start 2010-06-18
					InitializeCriticalSection(&csZhanKuan);
					EnterCriticalSection(&csZhanKuan);
					gbZhanKuan = !gbZhanKuan;

					LeaveCriticalSection(&csZhanKuan);
					DeleteCriticalSection(&csZhanKuan);

					SetZhuanKuanOpenFlag(hWnd);

					SetRedrawFlag(TRUE);
					// songchenguang add end 2010-06-18
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
	return CallWindowProc(OldBtnMenMeiZhanKuanOnOrOffProc,hWnd,Msg,wParam,lParam);
}
