/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 定量分析窗口源文件
 * Filename: fnQuantitativeAnalysisWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnQuantitativeAnalysisWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"


HWND hWndQuantitativeAnalysis = NULL;			//定量分析窗口句柄

HWND hBtnQuantitativeAnalysisDgsCurve = NULL;		//DGS曲线按钮句柄
HWND hBtnQuantitativeAnalysisAutoGainPlus = NULL;//自动增益按钮句柄
HWND hBtnQuantitativeAnalysisAdjust = NULL;//校准按钮句柄
HWND hBtnQuantitativeAnalysisPeakRemember = NULL;//峰值记忆按钮句柄

HWND quantitativeAnalysisBtnList[4] ={NULL,NULL,NULL,NULL};	//定量分析窗口的按钮表

WNDPROC OldBtnQuantitativeAnalysisDgsCurveProc = NULL;		//DGS曲线按钮默认处理过程
WNDPROC OldBtnQuantitativeAnalysisAutoGainPlusProc = NULL;//自动增益按钮默认处理过程
WNDPROC OldBtnQuantitativeAnalysisAdjustProc = NULL;//校准按钮默认处理过程
WNDPROC OldBtnQuantitativeAnalysisPeakRememberProc = NULL;//校准按钮默认处理过程


HWND currentQuantitativeAnalysisBtn = NULL;					//定量分析窗中当前选中的按钮

BOOL gbPeakRememberFlag = FALSE;  	//峰值记忆是否开启
CRITICAL_SECTION csPeakRemember;

static WORD wReducedButtonNum = 0;	//因功能开关减少按扭数量

static int  buttonNum = 4;			//按钮控件数

extern HINSTANCE hInst;				//程序实例句柄
extern HWND hWndFnMenu;				//Fn功能窗口句柄
extern HWND currentFnMenuBtn;		//Fn功能窗口当前选中句柄
extern HWND hWndMain;
//extern HWND hWndDgsCurve;			//DGS曲线窗口句柄
extern HWND hWndAutoGainPlus;		//自动增益窗口句柄
extern HWND hWndAdjust;				//校准窗口句柄
extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;			//当前显示的子窗口
extern DWORD gFunctionDefine;		//功能定义

/**************************
*注册定量分析窗口
* @quantitativeAnalysis    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerQuantitativeAnalysisChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) quantitativeAnalysisChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szQuantitativeAnalysisChild;

	return RegisterClass(&wc);
}

void displayPeakRemember(HWND hWnd ,BOOL bSwitch)
{
	if(bSwitch)
		DrawLableText(hWnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	else
		DrawLableText(hWnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

/**************************
*定量分析窗口处理过程
* @quantitativeAnalysis    hInstance[in]:
* @quantitativeAnalysis		message[in]:
* @quantitativeAnalysis		wParam[in]:
* @quantitativeAnalysis		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK quantitativeAnalysisChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;
	
	switch (message) 
	{
		case WM_CREATE:
		//	gFunctionDefine = 	0x0000000A;
			buttonNum = 3;
			wReducedButtonNum = 0;
		//	if((gFunctionDefine & DGSCURVE_SWITCH) == 0)
		//		wReducedButtonNum ++;
			if((gFunctionDefine & AUTOGAIN_SWITCH) == 0)
				wReducedButtonNum ++;
			if((gFunctionDefine & AUTOADJUST_SWITCH) == 0)
				wReducedButtonNum ++;
			if((gFunctionDefine & PEAKMEMORY_SWITCH) == 0)
				wReducedButtonNum ++;

			buttonNum = buttonNum - wReducedButtonNum;
			//画按钮控件
		//	hBtnQuantitativeAnalysisDgsCurve = CreateWindow(_T("BUTTON"),_T("dgsCurveQuantitativeAnalysis"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
		//			0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUANTITATIVEANALYSIS_DGSCURVE,hInst,NULL);
			hBtnQuantitativeAnalysisAutoGainPlus = CreateWindow(_T("BUTTON"),_T("AutoGainPlusQuantitativeAnalysis"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUANTITATIVEANALYSIS_AUTOGAINPLUS,hInst,NULL);
			hBtnQuantitativeAnalysisAdjust = CreateWindow(_T("BUTTON"),_T("AdjustQuantitativeAnalysis"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUANTITATIVEANALYSIS_ADJUST,hInst,NULL);
			hBtnQuantitativeAnalysisPeakRemember = CreateWindow(_T("BUTTON"),_T("peakRememberQuantitativeAnalysis"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUANTITATIVEANALYSIS_PEAKREMEMBER,hInst,NULL);

		/*	if((gFunctionDefine & DGSCURVE_SWITCH) != 0)	//dgs曲线功能开
			{
				quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisDgsCurve;
				if((gFunctionDefine & AUTOGAIN_SWITCH) != 0)
				{
					quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisAutoGainPlus;
					if((gFunctionDefine & AUTOADJUST_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[2] = hBtnQuantitativeAnalysisAdjust;
						if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
						{
							quantitativeAnalysisBtnList[3] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
					else
					{
						if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
						{
							quantitativeAnalysisBtnList[2] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
				}
				else 
				{
					if((gFunctionDefine & AUTOADJUST_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisAdjust;
						if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
						{
							quantitativeAnalysisBtnList[2] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
					else
					{
						if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
						{
							quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
					
				}
			}
			else					//dgs曲线功能关
			{	
				if((gFunctionDefine & AUTOGAIN_SWITCH) != 0)
				{
					quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisAutoGainPlus;
					if((gFunctionDefine & AUTOADJUST_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisAdjust;
						if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
						{
							quantitativeAnalysisBtnList[2] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
					else
					{
						if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
						{
							quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
				}
				else 
				{
					if((gFunctionDefine & 0x0004) != 0)
					{
						quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisAdjust;
						if((gFunctionDefine & 0x0008) != 0)
						{
							quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
					else
					{
						if((gFunctionDefine & 0x0008) != 0)
						{
							quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}					
				}
			}*/

			if((gFunctionDefine & AUTOGAIN_SWITCH) != 0)
			{
				quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisAutoGainPlus;
				if((gFunctionDefine & AUTOADJUST_SWITCH) != 0)
				{
					quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisAdjust;
					if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[2] = hBtnQuantitativeAnalysisPeakRemember;
					}
				}
				else
				{
					if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisPeakRemember;
					}
				}
			}
			else 
			{
				if((gFunctionDefine & AUTOADJUST_SWITCH) != 0)
				{
					quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisAdjust;
					if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisPeakRemember;
					}
				}
				else
				{
					if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisPeakRemember;
					}
				}
				
			}
			
			/*
			quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisDgsCurve;
			quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisAutoGainPlus;
			quantitativeAnalysisBtnList[2] = hBtnQuantitativeAnalysisAdjust;
			quantitativeAnalysisBtnList[3] = hBtnQuantitativeAnalysisPeakRemember;
			*/

			//按钮控件的自定义处理过程
	//		OldBtnQuantitativeAnalysisDgsCurveProc = (WNDPROC) GetWindowLong(hBtnQuantitativeAnalysisDgsCurve,GWL_WNDPROC);
	//		SetWindowLong(hBtnQuantitativeAnalysisDgsCurve,GWL_WNDPROC,(LONG)btnQuantitativeAnalysisDgsCurveProc);
			
			OldBtnQuantitativeAnalysisAutoGainPlusProc = (WNDPROC) GetWindowLong(hBtnQuantitativeAnalysisAutoGainPlus,GWL_WNDPROC);
			SetWindowLong(hBtnQuantitativeAnalysisAutoGainPlus,GWL_WNDPROC,(LONG)btnQuantitativeAnalysisAutoGainPlusProc);

			OldBtnQuantitativeAnalysisAdjustProc = (WNDPROC) GetWindowLong(hBtnQuantitativeAnalysisAdjust,GWL_WNDPROC);
			SetWindowLong(hBtnQuantitativeAnalysisAdjust,GWL_WNDPROC,(LONG)btnQuantitativeAnalysisAdjustProc);

			
			OldBtnQuantitativeAnalysisPeakRememberProc = (WNDPROC) GetWindowLong(hBtnQuantitativeAnalysisPeakRemember,GWL_WNDPROC);
			SetWindowLong(hBtnQuantitativeAnalysisPeakRemember,GWL_WNDPROC,(LONG)btnQuantitativeAnalysisPeakRememberProc);

			currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[0];
			if(currentQuantitativeAnalysisBtn)
			{
				SetFocus(currentQuantitativeAnalysisBtn);//选中按钮获得输入焦点
				gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
			}
			else			//功能都关闭按钮的父窗口获得输入焦点
				SetFocus(hWnd);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndFnMenu,SW_SHOW);
				SetFocus(currentFnMenuBtn);//上级菜单最后选中的按钮获得输入焦点

				DestroyWindow(hWndQuantitativeAnalysis);
				hWndQuantitativeAnalysis = 0;
				
				gCurrentFocusBtn = currentFnMenuBtn;

				gCurrentWnd = hWndFnMenu;
			}
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			quantitativeAnalysis_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnQuantitativeAnalysisAutoGainPlus)
			{
//				DrawLableText(pdis->hwndItem, _T("自动增益"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			}
			if(pdis->hwndItem == hBtnQuantitativeAnalysisAdjust)
			{
//				DrawLableText(pdis->hwndItem, _T("校准"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			}
			if(pdis->hwndItem == hBtnQuantitativeAnalysisPeakRemember)
			{
//				DrawLableText(pdis->hwndItem, _T("峰值记忆"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y - 20);
				displayPeakRemember(pdis->hwndItem, gbPeakRememberFlag);
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
*加载定量分析窗口按钮图片
* @quantitativeAnalysis    hInstance[in]:
* @quantitativeAnalysis		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int quantitativeAnalysis_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	BTN_BMP_INFO tbl_quantitativeAnalysisBtnBitmap[]=
	{
		{IDC_QUANTITATIVEANALYSIS_DGSCURVE	,IDB_BITMAP_DGSQUXIAN_NOSEL_R	 ,  IDB_BITMAP_DGSQUXIAN_SELECT_R		},
		{IDC_QUANTITATIVEANALYSIS_AUTOGAINPLUS	,IDB_BITMAP_ZIDONGZENGYI_NOSEL_R	 ,  IDB_BITMAP_ZIDONGZENGYI_SELECT_R		},
		{IDC_QUANTITATIVEANALYSIS_ADJUST	,IDB_BITMAP_JIAOZHUN_NOSEL_R	 ,  IDB_BITMAP_JIAOZHUN_SELECT_R		},
		{IDC_QUANTITATIVEANALYSIS_PEAKREMEMBER	,IDB_BITMAP_FENGZHIJIYI_NOSEL_R	 ,  IDB_BITMAP_FENGZHIJIYI_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 
	//设置功能关闭的图片
	if((gFunctionDefine & DGSCURVE_SWITCH) == 0)
		tbl_quantitativeAnalysisBtnBitmap[0].nBtnNoFocID = IDB_BITMAP_DGSQUXIAN_DISABLE_R;
	if((gFunctionDefine & AUTOGAIN_SWITCH) == 0)
		tbl_quantitativeAnalysisBtnBitmap[1].nBtnNoFocID = IDB_BITMAP_ZIDONGZENGYI_DISABLE_R;
	if((gFunctionDefine & AUTOADJUST_SWITCH) == 0)
		tbl_quantitativeAnalysisBtnBitmap[2].nBtnNoFocID = IDB_BITMAP_ZIDONGJIAOZHUN_DISABLE_R;
	if((gFunctionDefine & PEAKMEMORY_SWITCH) == 0)
		tbl_quantitativeAnalysisBtnBitmap[3].nBtnNoFocID = IDB_BITMAP_FENGZHIJIYI_DISABLE_R;
	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_quantitativeAnalysisBtnBitmap);
	return ReturnVal;
}

/**************************
*DGS曲线按钮自定义处理过程
* @quantitativeAnalysis    hInstance[in]:
* @quantitativeAnalysis		message[in]:
* @quantitativeAnalysis		wParam[in]:
* @quantitativeAnalysis		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
#if 0
LRESULT CALLBACK btnQuantitativeAnalysisDgsCurveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
									
				/*	if((wFunctionControl & 0x0008) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisPeakRemember;
					else if((wFunctionControl & 0x0004) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAdjust;
					else if((wFunctionControl & 0x0002) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAutoGainPlus;
			
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
				*/
				}
				else if(wParam == VK_DOWN)
				{
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
	
				/*
					if((wFunctionControl & 0x0002) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAutoGainPlus;
					else if((wFunctionControl & 0x0004) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAdjust;
					else if((wFunctionControl & 0x0008) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisPeakRemember;
				*/
				}
				else if(wParam == VK_RETURN)
				{
				/*	hWndDgsCurve = CreateWindow(szDgsCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndQuantitativeAnalysis,SW_HIDE);
					//SetFocus(hWndDgsCurve);
					ShowWindow(hWndDgsCurve,SW_SHOW);
					UpdateWindow(hWndDgsCurve);
					*/
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
	return CallWindowProc(OldBtnQuantitativeAnalysisDgsCurveProc,Wnd,Msg,wParam,lParam);
}
#endif
/**************************
*自动增益按钮自定义处理过程
* @quantitativeAnalysis    hInstance[in]:
* @quantitativeAnalysis		message[in]:
* @quantitativeAnalysis		wParam[in]:
* @quantitativeAnalysis		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnQuantitativeAnalysisAutoGainPlusProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
		
				/*	if((wFunctionControl & 0x0001) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisDgsCurve;
					else if((wFunctionControl & 0x0008) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisPeakRemember;
					else if((wFunctionControl & 0x0004) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAdjust;
				*/
				}
				else if(wParam == VK_DOWN)
				{
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
			
				/*	if((wFunctionControl & 0x0004) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAdjust;
					else if((wFunctionControl & 0x0008) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisPeakRemember;
					if((wFunctionControl & 0x0001) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisDgsCurve;
				*/
				}
				else if(wParam == VK_RETURN)
				{
					hWndAutoGainPlus = CreateWindow(szAutoGainPlusChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndQuantitativeAnalysis,SW_HIDE);
					//SetFocus(hWndAutoGainPlus);
					ShowWindow(hWndAutoGainPlus,SW_SHOW);
					UpdateWindow(hWndAutoGainPlus);

					gCurrentWnd = hWndAutoGainPlus;
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
	return CallWindowProc(OldBtnQuantitativeAnalysisAutoGainPlusProc,Wnd,Msg,wParam,lParam);
}
/**************************
*校准按钮自定义处理过程
* @quantitativeAnalysis    hInstance[in]:
* @quantitativeAnalysis		message[in]:
* @quantitativeAnalysis		wParam[in]:
* @quantitativeAnalysis		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnQuantitativeAnalysisAdjustProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
				
				/*	if((wFunctionControl & 0x0008) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisPeakRemember;
					else if((wFunctionControl & 0x0001) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisDgsCurve;
					else if((wFunctionControl & 0x0002) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAutoGainPlus;
				*/
				}
				else if(wParam == VK_DOWN)
				{
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndAdjust = CreateWindow(szAdjustChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndQuantitativeAnalysis,SW_HIDE);
					//SetFocus(hWndAdjust);
					ShowWindow(hWndAdjust,SW_SHOW);
					UpdateWindow(hWndAdjust);

					gCurrentWnd = hWndAdjust;
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
	return CallWindowProc(OldBtnQuantitativeAnalysisAdjustProc,Wnd,Msg,wParam,lParam);
}

/**************************
*校准按钮自定义处理过程
* @quantitativeAnalysis    hInstance[in]:
* @quantitativeAnalysis		message[in]:
* @quantitativeAnalysis		wParam[in]:
* @quantitativeAnalysis		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnQuantitativeAnalysisPeakRememberProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					InitializeCriticalSection(&csPeakRemember);
					EnterCriticalSection(&csPeakRemember);

					gbPeakRememberFlag = !gbPeakRememberFlag;

					LeaveCriticalSection(&csPeakRemember);
					DeleteCriticalSection(&csPeakRemember);

					displayPeakRemember(Wnd, gbPeakRememberFlag);					
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
	return CallWindowProc(OldBtnQuantitativeAnalysisPeakRememberProc,Wnd,Msg,wParam,lParam);
}

