/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 测厚校准窗口源文件
 * Filename: ChZeroSingleWin.cpp
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
#include "gateWin.h"
#include "sendParamWin.h"

HWND hWndChZeroSingle = NULL;					//测厚单点法零点校准窗口句柄
HWND currentChZeroSingleBtn = NULL;				//测厚单点法零点校准窗口当前选中的按钮
BOOL gbChZeroSingleStart = FALSE;			//单点法零点校准是否进行中	
float gfAdjustZero = 0;							//校准的零点

static HWND hBtnChZeroSingleThickness = NULL;			//试块厚度按钮句柄
static HWND hBtnChZeroSingleStart = NULL;				//校准开始按钮句柄
static HWND hBtnChZeroSingleResult = NULL;				//校准零点按钮句柄
static HWND hBtnChZeroSingleAccept = NULL;				//接受校准结果按钮句柄

static HWND ChZeroSingleBtnList[4] = {NULL,NULL,NULL,NULL};						//测厚单点法零点校准窗口的按钮表

static WNDPROC OldBtnChZeroSingleThicknessProc = NULL;			//试块厚度按钮默认处理过程
static WNDPROC OldBtnChZeroSingleStartProc = NULL;				//校准开始按钮默认处理过程
static WNDPROC OldBtnChZeroSingleResultProc = NULL;				//校准零点按钮默认处理过程
static WNDPROC OldBtnChZeroSingleAcceptProc = NULL;				//接受校准结果按钮默认处理过程

static const WORD buttonNum = 4;		//按钮控件数

float gfZeroThickness = 10;				//零点校准试块厚度
static float fThicknessStep = 1;		//试块厚度步长
static WORD wThicknessStepIndex = 1;	//试块厚度步长索引

ATOM registerChZeroSingleChild(HINSTANCE hInstance);
static LRESULT CALLBACK ChZeroSingleChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int ChZeroSingle_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChZeroSingleThicknessProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChZeroSingleStartProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChZeroSingleResultProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChZeroSingleAcceptProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//程序实例句柄

extern wchar_t gszTextStep[10];
extern wchar_t gszTextPeak[20];
extern HWND hStatusBar;			// 状态栏句柄
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndChZeroAdjust;		//测厚零点校准窗口句柄
extern HWND currentChZeroAdjustBtn;	//测厚校准窗口中当前选中的按钮
extern CRITICAL_SECTION csAdjust;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD CurrentChannelNo;
extern CRITICAL_SECTION csGate;
extern SEND_PARAM_DATA SendParam_ad[2];
extern GATE_INFO GateInfos[2][3];
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern HWND gCurrentWnd;
extern GATE_INFO stOldGateInfos[2][3];

extern void displayThickness(HWND hWnd ,float fHouDu);
extern void displayAdjustZero(HWND hWnd ,float fZero);
extern void SetDistance(HWND hWnd, const float& fDistance, const WORD& nIndex);
extern void SetAmendLableAndExecute(HWND hWnd);

/**************************
*显示零点校准单点法试块厚度
* @param		hWnd[in]:窗口句柄
* @param		fHouDu[in]:钢板厚度
* @return   LRESULT :
* @since    1.00
***************************/
void displayZeroSingleThickness(HWND hWnd ,float fHouDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fHouDu);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

/**************************
*显示单点法零点校准的零点
* @param		hWnd[in]:窗口句柄
* @param		fZero[in]:零点值
* @return   LRESULT :
* @since    1.00
***************************/
void displaySingleAdjustZero(HWND hWnd ,float fZero)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(fZero == -1)
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	else
	{
		wsprintf(strText, _T("%.1f"), fZero);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
}
/*
void SetDistanceGate(const float& fDistance, const WORD& nIndex)
{
	float fPianCha = (DisplaySet_ad[CurrentChannelNo].fRange / 100) * 2;//保持闸门宽度在屏幕上一致
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);

//	GateInfos[CurrentChannelNo][nIndex-1].bOpen = TRUE;
	//GateInfos[CurrentChannelNo][nIndex-1].fFrontGate = fDistance - fPianCha;
	//GateInfos[CurrentChannelNo][nIndex-1].fBackGate = fDistance + fPianCha;
	float fQianDian = fDistance - fPianCha;
	float fBack = fDistance + fPianCha;

	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);

	// 开启标志
	SetOpenFlag(NULL, TRUE, nIndex);

	// 起点
	SetQiDian(NULL, nIndex, fQianDian,fBack);
	
}
*/
void SetDistanceGate(const WORD& nIndex)
{
/*	float fPianCha = (DisplaySet_ad[CurrentChannelNo].fRange / 100) * 2;//保持闸门宽度在屏幕上一致
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);

//	GateInfos[CurrentChannelNo][nIndex-1].bOpen = TRUE;
	//GateInfos[CurrentChannelNo][nIndex-1].fFrontGate = fDistance - fPianCha;
	//GateInfos[CurrentChannelNo][nIndex-1].fBackGate = fDistance + fPianCha;
	float fQianDian = fDistance - fPianCha;
	float fBack = fDistance + fPianCha;

	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
*/
	float fGateWidth = (DisplaySet_ad[CurrentChannelNo].fRange / 100) * 4;	//保持闸门宽度在屏幕上一致
	float fHuiBoDistance = 0;												//底波声程
	float fGateQianDian = 0;												//闸门起点
	float fGateBack = 0;													//闸门后沿

	if(nIndex == 1)
		fHuiBoDistance = firstHuiBoDistance();
	else if(nIndex == 2)
		fHuiBoDistance = secondHuiBoDistance();
	fGateQianDian = fHuiBoDistance - fGateWidth / 2;
	fGateBack = fGateQianDian + fGateWidth;
	
	// 起点
	SetQiDian(NULL, nIndex, fGateQianDian,fGateBack, FALSE);
	//幅值
	SetPeakGate(NULL, nIndex, 62, FALSE);

	// 开启标志
	SetOpenFlag(NULL, TRUE, nIndex);
	Sleep(10);
}

/**************************
*注册测厚校准窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChZeroSingleChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) ChZeroSingleChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChZeroSingleChild;

	return RegisterClass(&wc);
}
/**************************
*测厚校准窗口处理过程
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK ChZeroSingleChildProc(HWND hWnd, UINT message, 
											  WPARAM wParam, LPARAM lParam)
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
			fThicknessStep = AMENDSTEPS[wThicknessStepIndex];
			gfAdjustZero = -1;
			//画按钮控件

			hBtnChZeroSingleThickness = CreateWindow(_T("BUTTON"),_T("ThicknessChZeroSingle"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZEROSINGLE_THICKNESS,hInst,NULL);

			hBtnChZeroSingleStart = CreateWindow(_T("BUTTON"),_T("ChZeroSingleStart"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZEROSINGLE_START,hInst,NULL);

			hBtnChZeroSingleResult = CreateWindow(_T("BUTTON"),_T("ChZeroSingleResult"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZEROSINGLE_RESULT,hInst,NULL);

			hBtnChZeroSingleAccept = CreateWindow(_T("BUTTON"),_T("ChZeroSingleAccept"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 3*BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZEROSINGLE_ACCEPT,hInst,NULL);
					
			ChZeroSingleBtnList[0] = hBtnChZeroSingleThickness;
			ChZeroSingleBtnList[1] = hBtnChZeroSingleStart;
			ChZeroSingleBtnList[2] = hBtnChZeroSingleResult;
			ChZeroSingleBtnList[3] = hBtnChZeroSingleAccept;
			
			//按钮控件的自定义处理过程			
			OldBtnChZeroSingleThicknessProc = (WNDPROC) GetWindowLong(hBtnChZeroSingleThickness,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroSingleThickness,GWL_WNDPROC,(LONG)btnChZeroSingleThicknessProc);
			
			OldBtnChZeroSingleStartProc = (WNDPROC) GetWindowLong(hBtnChZeroSingleStart,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroSingleStart,GWL_WNDPROC,(LONG)btnChZeroSingleStartProc);

			OldBtnChZeroSingleResultProc = (WNDPROC) GetWindowLong(hBtnChZeroSingleResult,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroSingleResult,GWL_WNDPROC,(LONG)btnChZeroSingleResultProc);

			OldBtnChZeroSingleAcceptProc = (WNDPROC) GetWindowLong(hBtnChZeroSingleAccept,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroSingleAccept,GWL_WNDPROC,(LONG)btnChZeroSingleAcceptProc);
		
			currentChZeroSingleBtn = ChZeroSingleBtnList[0];//默认当前选中校准方法按钮
			SetFocus(currentChZeroSingleBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentChZeroSingleBtn;

			SetDistanceGate(1);
			SetOpenFlag(NULL, FALSE, 2);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
		/*		if(LOWORD(wParam) == IDC_CHZEROSINGLE_THICKNESS)
				{
					currentChZeroSingleBtn = ChZeroSingleBtnList[2]; 
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
					hWndThickness = CreateWindow(szThicknessChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							MENU_THICKNESS_X, MENU_THICKNESS_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChZeroSingle,SW_HIDE);
					//SetFocus(hWndThickness);
					ShowWindow(hWndThickness,SW_SHOW);
					UpdateWindow(hWndThickness);
				}
				else if(LOWORD(wParam) == IDC_CHZEROSINGLE_START)
				{
				//	currentChZeroSingleBtn = ChZeroSingleBtnList[3]; 
				//	SetFocus(currentChZeroSingleBtn);
				//	gCurrentFocusBtn = currentChZeroSingleBtn;
				}
			*/
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			ChZeroSingle_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnChZeroSingleThickness)
			{
				displayZeroSingleThickness(pdis->hwndItem, gfZeroThickness);
			}
			if(pdis->hwndItem == hBtnChZeroSingleResult)
			{
				displaySingleAdjustZero(pdis->hwndItem, gfAdjustZero);
			}
			if(pdis->hwndItem == hBtnChZeroSingleStart)
			{
				SetDistanceGate(1);
				SetOpenFlag(NULL, FALSE, 2);
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
*加载测厚校准窗口按钮图片
* @ param    hInstance[in]:
* @ param    		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int ChZeroSingle_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_ChZeroSingleBtnBitmap[]=
	{
		{IDC_CHZEROSINGLE_THICKNESS	,IDB_BITMAP_SHIKUAIHOUDU4_NOSEL_R ,  IDB_BITMAP_SHIKUAIHOUDU4_SELECT_R	},
		{IDC_CHZEROSINGLE_START	,IDB_BITMAP_JIAOZHUNKAISHI5_NOSEL_R ,  IDB_BITMAP_JIAOZHUNKAISHI5_SELECT_R	},
		{IDC_CHZEROSINGLE_RESULT	,IDB_BITMAP_JIAOZHUNLINGDIAN4_NOSEL_R ,  IDB_BITMAP_JIAOZHUNLINGDIAN4_SELECT_R	},
		{IDC_CHZEROSINGLE_ACCEPT	,IDB_BITMAP_JIESHOUJIAOZHUNJIEGUO4_NOSEL_R ,  IDB_BITMAP_JIESHOUJIAOZHUNJIEGUO4_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_ChZeroSingleBtnBitmap);
	return ReturnVal;
}

/**************************
*试块厚度按钮自定义处理过程
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChZeroSingleThicknessProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), fThicknessStep);
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
					ShowWindow(hWndChZeroAdjust,SW_SHOW);
					SetFocus(currentChZeroAdjustBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChZeroSingle);
					hWndChZeroSingle = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;

					gCurrentWnd = hWndChZeroAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if((gfZeroThickness - fThicknessStep) >= 1)
						gfZeroThickness -= fThicknessStep;
					else
						gfZeroThickness = 1;

					displayZeroSingleThickness(Wnd, gfZeroThickness);
					
					SetDistanceGate(1);
				}
				else if(wParam == VK_RIGHT)
				{
					gfZeroThickness += fThicknessStep;

					displayZeroSingleThickness(Wnd, gfZeroThickness);
					
					SetDistanceGate(1);
				}
				else if(wParam == 'F')
				{
					if(wThicknessStepIndex < 3)
						wThicknessStepIndex++;
					else
						wThicknessStepIndex = 0;

					fThicknessStep = AMENDSTEPS[wThicknessStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fThicknessStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnChZeroSingleThicknessProc,Wnd,Msg,wParam,lParam);
}
/**************************
*校准开始按钮自定义处理过程
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChZeroSingleStartProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChZeroAdjust,SW_SHOW);
					SetFocus(currentChZeroAdjustBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChZeroSingle);
					hWndChZeroSingle = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;

					gCurrentWnd = hWndChZeroAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_RETURN)
				{					
					displaySingleAdjustZero(hBtnChZeroSingleResult, -1);					//校准完成前显示N/A

					memset(gszTextPeak, 0, sizeof(gszTextPeak));
					swprintf(gszTextPeak, _T("校准进行中..."));

					SendMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);

					SetDistanceGate(1);
					
					InitializeCriticalSection(&csAdjust);
					EnterCriticalSection(&csAdjust);
					gbChZeroSingleStart = TRUE;
					LeaveCriticalSection(&csAdjust);
					DeleteCriticalSection(&csAdjust);

					SetFocus(NULL);
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
				else if(wParam == '9')
				{
					if(currentChZeroSingleBtn == hBtnChZeroSingleStart)
					{
						SetFocus(currentChZeroSingleBtn);
						displaySingleAdjustZero(hBtnChZeroSingleResult, gfAdjustZero);
					}
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
	return CallWindowProc(OldBtnChZeroSingleStartProc,Wnd,Msg,wParam,lParam);
}
/**************************
*校准零点按钮自定义处理过程
* @ param    hInstance[in]:
* @ param    message[in]:
* @ param    wParam[in]:
* @ param    lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChZeroSingleResultProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChZeroAdjust,SW_SHOW);
					SetFocus(currentChZeroAdjustBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChZeroSingle);
					hWndChZeroSingle = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;

					gCurrentWnd = hWndChZeroAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_RETURN)
				{
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
	return CallWindowProc(OldBtnChZeroSingleResultProc,Wnd,Msg,wParam,lParam);
}
/**************************
*接受校准结果按钮自定义处理过程
* @ param    hInstance[in]:
* @ param    message[in]:
* @ param    wParam[in]:
* @ param    lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChZeroSingleAcceptProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChZeroAdjust,SW_SHOW);
					SetFocus(currentChZeroAdjustBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChZeroSingle);
					hWndChZeroSingle = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;

					gCurrentWnd = hWndChZeroAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_RETURN)
				{
					if(gfAdjustZero > 0)
					{
						SendParam_ad[CurrentChannelNo].nAmend = gfAdjustZero;
						SetAmendLableAndExecute(NULL);
					}
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
	return CallWindowProc(OldBtnChZeroSingleAcceptProc,Wnd,Msg,wParam,lParam);
}
