/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * AVG曲线窗口源文件
 * Filename: curveWin.cpp
 *
 * Spec: Trace Tag write here. If has not, OnOff this line. If have mutil-TraceTag, write one Tag per line
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
#include "craftParamWin.h"


HWND hWndAvgCurve = NULL;//AVG曲线窗口句柄
HWND hBtnAvgCurveMake = NULL;//制作按钮句柄
HWND hBtnAvgCurveOnOff = NULL;//开启/关闭按钮句柄
HWND hBtnAvgCurveDefectZhi1 = NULL;//Φ1值按钮句柄
HWND hBtnAvgCurveDefectZhi2 = NULL;//Φ2值按钮句柄
HWND hBtnAvgCurveDefectZhi3 = NULL;//Φ3值按钮句柄

HWND avgCurveBtnList[5] ={NULL,NULL,NULL,NULL,NULL};//AVG曲线窗口的按钮表
WNDPROC OldBtnAvgCurveMakeProc = NULL;//制作按钮默认处理过程
WNDPROC OldBtnAvgCurveOnOffProc = NULL;//开启/关闭按钮默认处理过程
WNDPROC OldBtnAvgCurveDefectZhi1Proc = NULL;//Φ1值按钮默认处理过程
WNDPROC OldBtnAvgCurveDefectZhi2Proc = NULL;//Φ2值按钮默认处理过程
WNDPROC OldBtnAvgCurveDefectZhi3Proc = NULL;//Φ3值按钮默认处理过程

HWND currentAvgCurveBtn = NULL;				//AVG曲线窗中当前选中的按钮
BOOL gbAvgCurveFlag[CHANNEL_NUM] = {FALSE, FALSE};		//AVG曲线开关标志
float gfDefectZhi1[CHANNEL_NUM] = {2, 2};			//所测孔径Φ1
float gfDefectZhi2[CHANNEL_NUM] = {0, 0};			//Φ2
float gfDefectZhi3[CHANNEL_NUM] = {0, 0};			//Φ3

static const int buttonNum = 5;//按钮控件数
static float fFaiZhiStep = 1;	//Φ值步进
static WORD wFaiZhiStepIndex = 1;	//Φ值步进索引
static float fOldSelectedAvgPointData[MAX_CURVE_POINT_NUM] = {0};	//保存Φ1的峰值点信息
static float fOldDefectZhi1 = 0;			//保存Φ1孔径

ATOM registerAvgCurveChild(HINSTANCE hInstance);
static LRESULT CALLBACK avgCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int avgCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnAvgCurveMakeProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnAvgCurveOnOffProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnAvgCurveDefectZhi1Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnAvgCurveDefectZhi2Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnAvgCurveDefectZhi3Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);


extern HWND hWndCurve;//参数设置窗口句柄
extern HWND hWndMakeCurve;//制作AVG曲线窗口
extern HWND hWndMain;//主窗口
extern HWND currentCurveBtn;//AVG曲线窗口中当前选中的按钮
extern HINSTANCE hInst;//程序实例句柄

extern ARS200FPGA_CH_DAC_REG *chCurrent_dac_reg;

extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;			//当前显示的子窗口
extern WORD CurrentChannelNo;		// 当前通道号
extern HWND hWndMakeAvgCurve;//制作AVG曲线窗口句柄
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG曲线开关标志
extern BOOL bgAvgChangeFlag;					//AVG曲线位置是否改变标志
extern int gCurrentSysColorIndex;
extern SYS_COLOR_INFO gstSysColor[SYS_COLOR_NUM];
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//添加的样点数据队列
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//已选中的AVG点数量
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern CRITICAL_SECTION csSysInfo;
extern SYS_INFO gSysInfo;
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;
extern BOOL gbGuanLian;
extern BOOL gbHotDB;			// 快捷键增益
extern BOOL gbHotRange;			// 快捷键声程

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
		DrawLableText(hWnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	else
		DrawLableText(hWnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);		
}

/**************************
*显示Φ值
* @param		hWnd[in]:窗口句柄
* @param		fWaiJing[in]:外径值
* @return   LRESULT :
* @since    1.00
***************************/
static void displayDefectZhi(HWND hWnd ,float fDefectZhi)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fDefectZhi);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}

static void UpdateAvgMenuInfo(const WORD& chNo)
{
	displaySwitch(hBtnAvgCurveOnOff, gbAvgCurveFlag[chNo]);
	displayDefectZhi(hBtnAvgCurveDefectZhi1, gfDefectZhi1[chNo]);
	displayDefectZhi(hBtnAvgCurveDefectZhi2, gfDefectZhi2[chNo]);
	displayDefectZhi(hBtnAvgCurveDefectZhi3, gfDefectZhi3[chNo]);
}
/**************************
*注册AVG曲线窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerAvgCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));

	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) avgCurveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szAvgCurveChild;

	return RegisterClass(&wc);
}

/**************************
*AVG曲线窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK avgCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:				
			//画按钮控件
			hBtnAvgCurveMake = CreateWindow(_T("BUTTON"),_T("Make"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 ,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_AVGCURVE_MAKE,hInst,NULL);
			hBtnAvgCurveOnOff = CreateWindow(_T("BUTTON"),_T("OnOff"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_AVGCURVE_SWITCH,hInst,NULL);
			hBtnAvgCurveDefectZhi1 = CreateWindow(_T("BUTTON"),_T("DefectZhi1"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_AVGCURVE_DEFECTZHI1,hInst,NULL);
			hBtnAvgCurveDefectZhi2 = CreateWindow(_T("BUTTON"),_T("DefectZhi2"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_AVGCURVE_DEFECTZHI2,hInst,NULL);
			hBtnAvgCurveDefectZhi3 = CreateWindow(_T("BUTTON"),_T("DefectZhi3"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_AVGCURVE_DEFECTZHI3,hInst,NULL);
			
			avgCurveBtnList[0] = hBtnAvgCurveMake;
			avgCurveBtnList[1] = hBtnAvgCurveOnOff;
			avgCurveBtnList[2] = hBtnAvgCurveDefectZhi1;
			avgCurveBtnList[3] = hBtnAvgCurveDefectZhi2;
			avgCurveBtnList[4] = hBtnAvgCurveDefectZhi3;
			//按钮控件的自定义处理过程
			OldBtnAvgCurveMakeProc = (WNDPROC) GetWindowLong(hBtnAvgCurveMake,GWL_WNDPROC);
			SetWindowLong(hBtnAvgCurveMake,GWL_WNDPROC,(LONG)btnAvgCurveMakeProc);
			OldBtnAvgCurveOnOffProc = (WNDPROC) GetWindowLong(hBtnAvgCurveOnOff,GWL_WNDPROC);
		    SetWindowLong(hBtnAvgCurveOnOff,GWL_WNDPROC,(LONG)btnAvgCurveOnOffProc);
			OldBtnAvgCurveDefectZhi1Proc = (WNDPROC) GetWindowLong(hBtnAvgCurveDefectZhi1,GWL_WNDPROC);
		    SetWindowLong(hBtnAvgCurveDefectZhi1,GWL_WNDPROC,(LONG)btnAvgCurveDefectZhi1Proc);
			OldBtnAvgCurveDefectZhi2Proc = (WNDPROC) GetWindowLong(hBtnAvgCurveDefectZhi2,GWL_WNDPROC);
		    SetWindowLong(hBtnAvgCurveDefectZhi2,GWL_WNDPROC,(LONG)btnAvgCurveDefectZhi2Proc);
			OldBtnAvgCurveDefectZhi3Proc = (WNDPROC) GetWindowLong(hBtnAvgCurveDefectZhi3,GWL_WNDPROC);
		    SetWindowLong(hBtnAvgCurveDefectZhi3,GWL_WNDPROC,(LONG)btnAvgCurveDefectZhi3Proc);
			
			
			currentAvgCurveBtn = avgCurveBtnList[0];  //默认当前选中制作按钮
			SetFocus(currentAvgCurveBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentAvgCurveBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:

			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			avgCurve_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnAvgCurveOnOff)
			{
				displaySwitch(pdis->hwndItem, gbAvgCurveFlag[CurrentChannelNo]);
			}
			else if(pdis->hwndItem == hBtnAvgCurveDefectZhi1)
			{
				displayDefectZhi(pdis->hwndItem, gfDefectZhi1[CurrentChannelNo]);
				//保存Φ1的峰值点信息
				for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
				{
					fOldSelectedAvgPointData[i] = gSelectedAvgPointData[CurrentChannelNo][0][i].data;
				}
				//保存Φ1孔径
				 fOldDefectZhi1 = gfDefectZhi1[CurrentChannelNo];
			}
			else if(pdis->hwndItem == hBtnAvgCurveDefectZhi2)
			{
				displayDefectZhi(pdis->hwndItem, gfDefectZhi2[CurrentChannelNo]);
			}
			else if(pdis->hwndItem == hBtnAvgCurveDefectZhi3)
			{
				displayDefectZhi(pdis->hwndItem, gfDefectZhi3[CurrentChannelNo]);
			}
			else
			{
				;//
			}

			// 检测按钮句柄，显示相应的步长
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentAvgCurveBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnAvgCurveDefectZhi1 || pdis->hwndItem == hBtnAvgCurveDefectZhi2 || pdis->hwndItem == hBtnAvgCurveDefectZhi3)
						swprintf(gszTextStep, _T("%.1f"), fFaiZhiStep);
					else
						swprintf(gszTextStep, _T(""));

					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}
			break;
		case WM_CHAR:
			InitializeCriticalSection(&csSysInfo);
			EnterCriticalSection(&csSysInfo);
			if( wParam == '\t' && gSysInfo.wWorkMode != OneSendOneReceive)
			{
				// 更新按钮项数据
				if(CurrentChannelNo == 0)
				{
					UpdateAvgMenuInfo(1);
				}
				else
				{
					UpdateAvgMenuInfo(0);
				}
			}

			LeaveCriticalSection(&csSysInfo);
			DeleteCriticalSection(&csSysInfo);

			SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			break;
		case WM_CLOSE:		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载AVG曲线窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int avgCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_avgCurveBtnBitmap[]=
	{
		{IDC_AVGCURVE_MAKE	,IDB_BITMAP_ZHIZUO_NOSEL_R	 ,  IDB_BITMAP_ZHIZUO_SELECT_R		},
		{IDC_AVGCURVE_SWITCH	,IDB_BITMAP_KAIQIGUANBI5_NOSEL_R ,  IDB_BITMAP_KAIQIGUANBI5_SELECT_R	},
		{IDC_AVGCURVE_DEFECTZHI1	,IDB_BITMAP_FAI1_NOSEL_R ,  IDB_BITMAP_FAI1_SELECT_R	},
		{IDC_AVGCURVE_DEFECTZHI2	,IDB_BITMAP_FAI2_NOSEL_R ,  IDB_BITMAP_FAI2_SELECT_R	},
		{IDC_AVGCURVE_DEFECTZHI3	,IDB_BITMAP_FAI3_NOSEL_R ,  IDB_BITMAP_FAI3_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_avgCurveBtnBitmap);
	return ReturnVal;
}
/**************************
*制作按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAvgCurveMakeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndCurve,SW_SHOW);
					SetFocus(currentCurveBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndAvgCurve);
					hWndAvgCurve = 0;
					
					gCurrentFocusBtn = currentCurveBtn;

					gCurrentWnd = hWndCurve;
				}
			
				else if(wParam == VK_UP)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndMakeAvgCurve = CreateWindow(szMakeAvgCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndAvgCurve,SW_HIDE);
					ShowWindow(hWndMakeAvgCurve,SW_SHOW);
					UpdateWindow(hWndMakeAvgCurve);

					gCurrentWnd = hWndMakeAvgCurve;
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
	return CallWindowProc(OldBtnAvgCurveMakeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*开启/关闭按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAvgCurveOnOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					ShowWindow(hWndCurve,SW_SHOW);
					SetFocus(currentCurveBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndAvgCurve);
					hWndAvgCurve = 0;
					
					gCurrentFocusBtn = currentCurveBtn;

					gCurrentWnd = hWndCurve;
				}
			
				else if(wParam == VK_UP)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
				/*	if(!gbGuanLian) //非关联
					{
						gbAvgCurveFlag[CurrentChannelNo] = !gbAvgCurveFlag[CurrentChannelNo];
					}
					else
					{
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							gbAvgCurveFlag[i] = !gbAvgCurveFlag[i];
						}
					}

					displaySwitch(Wnd, gbAvgCurveFlag[CurrentChannelNo]);*/

					if(gbAvgCurveFlag[CurrentChannelNo] == OFF)
					{
						if(!gbGuanLian) //非关联
						{
							if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
							{
								gbAvgCurveFlag[CurrentChannelNo] = ON;
								DrawLableText(Wnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
							}
							else
							{
								if(CurrentChannelNo == 0)
								{
									PostMessage(hStatusBar, SB_SETTEXT, 
										(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("通道1无AVG"));
								}
								else
								{
									PostMessage(hStatusBar, SB_SETTEXT, 
										(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("通道2无AVG"));
								}
							}
						}
						else
						{
							for(int i = 0; i < CHANNEL_NUM; i++)
							{
								if(gSelectedAvgPointDataNum[i] > 1)
								{
									gbAvgCurveFlag[i] = ON;
									DrawLableText(Wnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
								}
								else
								{
									if(i == 0) //通道1无曲线
									{
										PostMessage(hStatusBar, SB_SETTEXT, 
											(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("通道1无AVG"));
									}
									else if(i == 1)	//通道2无曲线
									{
										if(gSelectedAvgPointDataNum[0] <= 1)//通道1也无曲线
										{
											PostMessage(hStatusBar, SB_SETTEXT, 
												(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("两通道均无AVG"));
										}
										else//通道1有曲线
										{
											PostMessage(hStatusBar, SB_SETTEXT, 
											(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("通道2无AVG"));
										}
									}
									else
									{
										;//
									}
								}
							}
						}
					}
					else
					{
						if(!gbGuanLian) //非关联
						{
							gbAvgCurveFlag[CurrentChannelNo] = OFF;
						}
						else
						{
							for(int i = 0; i < CHANNEL_NUM; i++)
							{
								gbAvgCurveFlag[i] = OFF;
							}
						}
						DrawLableText(Wnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
					}

					// 保存配置到默认文件
					SaveParamFile(lpDefaultParamFile);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnAvgCurveOnOffProc,Wnd,Msg,wParam,lParam);
}
/**************************
*Φ1值按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAvgCurveDefectZhi1Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					ShowWindow(hWndCurve,SW_SHOW);
					SetFocus(currentCurveBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndAvgCurve);
					hWndAvgCurve = 0;
					
					gCurrentFocusBtn = currentCurveBtn;

					gCurrentWnd = hWndCurve;
				}
			
				else if(wParam == VK_UP)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfDefectZhi1[CurrentChannelNo] - fFaiZhiStep >= MIN_FAIZHI + 0.1)
					{
						gfDefectZhi1[CurrentChannelNo] = gfDefectZhi1[CurrentChannelNo] - fFaiZhiStep;

						if(gbAvgCurveFlag[CurrentChannelNo] == ON) //有曲线
						{
							bgAvgChangeFlag = TRUE;//停止画线
							Sleep(30);//等待停止画线
							/*if(CurrentChannelNo == 0)
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}
							else
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}*/
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
							}
							//擦掉Φ值
							HDC hdc = GetDC(hWndMain);
							WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
							HBRUSH hbr = CreateSolidBrush(wColorBack);
							
							RECT rect;
							GetClipBox(hdc, &rect);
							
							RECT RctWave;// 波形区域
							RctWave.left = rect.left + SCALE_WIDTH;
							RctWave.right = rect.right - SCALE_WIDTH;
							RctWave.top = rect.top;
							RctWave.bottom = rect.bottom - SCALE_WIDTH;
							
							RECT stTextRect;
							stTextRect.left = RctWave.left + 2;
							stTextRect.right = RctWave.left + 60;
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
								stTextRect.top = stFirstPoint.y - 25;
								stTextRect.bottom = stFirstPoint.y;
							
								FillRect(hdc, &stTextRect, hbr);
							}
							DeleteObject(hbr);
							ReleaseDC(hWndMain,hdc);
						}
						
						if(gbAvgCurveFlag[CurrentChannelNo] == ON)
							bgAvgChangeFlag = FALSE;//AVG曲线需要重画

						if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
						{
							float fDeltaDB = (float)(40 * log10(gfDefectZhi1[CurrentChannelNo] / fOldDefectZhi1));
							for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
							{
								gSelectedAvgPointData[CurrentChannelNo][0][j].data = curveDataChangeByGain(fOldSelectedAvgPointData[j], fDeltaDB);
							}

							SaveParamFile(lpDefaultParamFile);
						}
					}
				/*	else
					{
						gfDefectZhi1[CurrentChannelNo] = 0;
					}*/

					displayDefectZhi(Wnd, gfDefectZhi1[CurrentChannelNo]);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfDefectZhi1[CurrentChannelNo] + fFaiZhiStep <= MAX_FAIZHI)
					{
						gfDefectZhi1[CurrentChannelNo] = gfDefectZhi1[CurrentChannelNo] + fFaiZhiStep;

						if(gbAvgCurveFlag[CurrentChannelNo] == ON) //有曲线
						{
							bgAvgChangeFlag = TRUE;//停止画线
							Sleep(30);//等待停止画线
						/*	if(CurrentChannelNo == 0)
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}
							else
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}*/
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
							}
							//擦掉Φ值
							HDC hdc = GetDC(hWndMain);
							WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
							HBRUSH hbr = CreateSolidBrush(wColorBack);
							
							RECT rect;
							GetClipBox(hdc, &rect);
							
							RECT RctWave;// 波形区域
							RctWave.left = rect.left + SCALE_WIDTH;
							RctWave.right = rect.right - SCALE_WIDTH;
							RctWave.top = rect.top;
							RctWave.bottom = rect.bottom - SCALE_WIDTH;
							
							RECT stTextRect;
							stTextRect.left = RctWave.left + 2;
							stTextRect.right = RctWave.left + 60;
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
								stTextRect.top = stFirstPoint.y - 25;
								stTextRect.bottom = stFirstPoint.y;
							
								FillRect(hdc, &stTextRect, hbr);
							}
							DeleteObject(hbr);
							ReleaseDC(hWndMain,hdc);
						}
						
						if(gbAvgCurveFlag[CurrentChannelNo] == ON)
							bgAvgChangeFlag = FALSE;//AVG曲线需要重画

						if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
						{
							float fDeltaDB = (float)(40 * log10(gfDefectZhi1[CurrentChannelNo] / fOldDefectZhi1));
							for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
							{
								gSelectedAvgPointData[CurrentChannelNo][0][j].data = curveDataChangeByGain(fOldSelectedAvgPointData[j], fDeltaDB);
							}

							SaveParamFile(lpDefaultParamFile);
						}
					}
					else
					{
						gfDefectZhi1[CurrentChannelNo] = 20;
					}

					displayDefectZhi(Wnd, gfDefectZhi1[CurrentChannelNo]);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
				else if(wParam == 'F')
				{
					if(wFaiZhiStepIndex < 1)
						wFaiZhiStepIndex++;
					else
						wFaiZhiStepIndex = 0;

					fFaiZhiStep = FAIZHISTEPS[wFaiZhiStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fFaiZhiStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnAvgCurveDefectZhi1Proc,Wnd,Msg,wParam,lParam);
}
/**************************
*Φ2值按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAvgCurveDefectZhi2Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					ShowWindow(hWndCurve,SW_SHOW);
					SetFocus(currentCurveBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndAvgCurve);
					hWndAvgCurve = 0;
					
					gCurrentFocusBtn = currentCurveBtn;

					gCurrentWnd = hWndCurve;
				}
			
				else if(wParam == VK_UP)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfDefectZhi2[CurrentChannelNo] - fFaiZhiStep >= MIN_FAIZHI)
					{
						gfDefectZhi2[CurrentChannelNo] = gfDefectZhi2[CurrentChannelNo] - fFaiZhiStep;

						if(gbAvgCurveFlag[CurrentChannelNo] == ON) //有曲线
						{
							bgAvgChangeFlag = TRUE;//停止画线
							Sleep(30);//等待停止画线
						/*	if(CurrentChannelNo == 0)
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}
							else
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}*/
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
							}
							//擦掉Φ值
							HDC hdc = GetDC(hWndMain);
							WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
							HBRUSH hbr = CreateSolidBrush(wColorBack);
							
							RECT rect;
							GetClipBox(hdc, &rect);
							
							RECT RctWave;// 波形区域
							RctWave.left = rect.left + SCALE_WIDTH;
							RctWave.right = rect.right - SCALE_WIDTH;
							RctWave.top = rect.top;
							RctWave.bottom = rect.bottom - SCALE_WIDTH;
							
							RECT stTextRect;
							stTextRect.left = RctWave.left + 2;
							stTextRect.right = RctWave.left + 60;
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
								stTextRect.top = stFirstPoint.y - 25;
								stTextRect.bottom = stFirstPoint.y;
							
								FillRect(hdc, &stTextRect, hbr);
							}
							DeleteObject(hbr);
							ReleaseDC(hWndMain,hdc);
						}
						
						if(gbAvgCurveFlag[CurrentChannelNo] == ON)
							bgAvgChangeFlag = FALSE;//AVG曲线需要重画

						if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
						{
							float fDeltaDB = (float)(40 * log10((float)gfDefectZhi2[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
							for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
							{
								gSelectedAvgPointData[CurrentChannelNo][1][j].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][0][j].data, fDeltaDB);
							}

							SaveParamFile(lpDefaultParamFile);
						}
					}
					else
					{
						gfDefectZhi2[CurrentChannelNo] = 0;
					}

					displayDefectZhi(Wnd, gfDefectZhi2[CurrentChannelNo]);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfDefectZhi2[CurrentChannelNo] + fFaiZhiStep <= MAX_FAIZHI)
					{
						gfDefectZhi2[CurrentChannelNo] = gfDefectZhi2[CurrentChannelNo] + fFaiZhiStep;

						if(gbAvgCurveFlag[CurrentChannelNo] == ON) //有曲线
						{
							bgAvgChangeFlag = TRUE;//停止画线
							Sleep(30);//等待停止画线
						/*	if(CurrentChannelNo == 0)
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}
							else
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}*/
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
							}
							//擦掉Φ值
							HDC hdc = GetDC(hWndMain);
							WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
							HBRUSH hbr = CreateSolidBrush(wColorBack);
							
							RECT rect;
							GetClipBox(hdc, &rect);
							
							RECT RctWave;// 波形区域
							RctWave.left = rect.left + SCALE_WIDTH;
							RctWave.right = rect.right - SCALE_WIDTH;
							RctWave.top = rect.top;
							RctWave.bottom = rect.bottom - SCALE_WIDTH;
							
							RECT stTextRect;
							stTextRect.left = RctWave.left + 2;
							stTextRect.right = RctWave.left + 60;
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
								stTextRect.top = stFirstPoint.y - 25;
								stTextRect.bottom = stFirstPoint.y;
							
								FillRect(hdc, &stTextRect, hbr);
							}
							DeleteObject(hbr);
							ReleaseDC(hWndMain,hdc);
						}
						
						if(gbAvgCurveFlag[CurrentChannelNo] == ON)
							bgAvgChangeFlag = FALSE;//AVG曲线需要重画

						if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
						{
							float fDeltaDB = (float)(40 * log10((float)gfDefectZhi2[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
							for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
							{
								gSelectedAvgPointData[CurrentChannelNo][1][j].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][0][j].data, fDeltaDB);
							}

							SaveParamFile(lpDefaultParamFile);
						}
					}
					else
					{
						gfDefectZhi2[CurrentChannelNo] = 20;
					}

					displayDefectZhi(Wnd, gfDefectZhi2[CurrentChannelNo]);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
				else if(wParam == 'F')
				{
					if(wFaiZhiStepIndex < 1)
						wFaiZhiStepIndex++;
					else
						wFaiZhiStepIndex = 0;

					fFaiZhiStep = FAIZHISTEPS[wFaiZhiStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fFaiZhiStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnAvgCurveDefectZhi2Proc,Wnd,Msg,wParam,lParam);
}
/**************************
*Φ3值按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAvgCurveDefectZhi3Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					ShowWindow(hWndCurve,SW_SHOW);
					SetFocus(currentCurveBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndAvgCurve);
					hWndAvgCurve = 0;
					
					gCurrentFocusBtn = currentCurveBtn;

					gCurrentWnd = hWndCurve;
				}
			
				else if(wParam == VK_UP)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfDefectZhi3[CurrentChannelNo] - fFaiZhiStep >= MIN_FAIZHI)
					{
						gfDefectZhi3[CurrentChannelNo] = gfDefectZhi3[CurrentChannelNo] - fFaiZhiStep;

						if(gbAvgCurveFlag[CurrentChannelNo] == ON) //有曲线
						{
							bgAvgChangeFlag = TRUE;//停止画线
							Sleep(30);//等待停止画线
						/*	if(CurrentChannelNo == 0)
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}
							else
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}*/
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
							}
							//擦掉Φ值
							HDC hdc = GetDC(hWndMain);
							WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
							HBRUSH hbr = CreateSolidBrush(wColorBack);
							
							RECT rect;
							GetClipBox(hdc, &rect);
							
							RECT RctWave;// 波形区域
							RctWave.left = rect.left + SCALE_WIDTH;
							RctWave.right = rect.right - SCALE_WIDTH;
							RctWave.top = rect.top;
							RctWave.bottom = rect.bottom - SCALE_WIDTH;
							
							RECT stTextRect;
							stTextRect.left = RctWave.left + 2;
							stTextRect.right = RctWave.left + 60;
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
								stTextRect.top = stFirstPoint.y - 25;
								stTextRect.bottom = stFirstPoint.y;
							
								FillRect(hdc, &stTextRect, hbr);
							}
							DeleteObject(hbr);
							ReleaseDC(hWndMain,hdc);
						}
						
						if(gbAvgCurveFlag[CurrentChannelNo] == ON)
							bgAvgChangeFlag = FALSE;//AVG曲线需要重画

						if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
						{
							float fDeltaDB = (float)(40 * log10((float)gfDefectZhi3[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
							for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
							{
								gSelectedAvgPointData[CurrentChannelNo][2][j].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][0][j].data, fDeltaDB);
							}

							SaveParamFile(lpDefaultParamFile);
						}
					}
					else
					{
						gfDefectZhi3[CurrentChannelNo] = 0;
					}

					displayDefectZhi(Wnd, gfDefectZhi3[CurrentChannelNo]);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfDefectZhi3[CurrentChannelNo] + fFaiZhiStep <= MAX_FAIZHI)
					{
						gfDefectZhi3[CurrentChannelNo] = gfDefectZhi3[CurrentChannelNo] + fFaiZhiStep;

						if(gbAvgCurveFlag[CurrentChannelNo] == ON) //有曲线
						{
							bgAvgChangeFlag = TRUE;//停止画线
							Sleep(30);//等待停止画线
						/*	if(CurrentChannelNo == 0)
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}
							else
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}*/
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
							}
							//擦掉Φ值
							HDC hdc = GetDC(hWndMain);
							WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
							HBRUSH hbr = CreateSolidBrush(wColorBack);
							
							RECT rect;
							GetClipBox(hdc, &rect);
							
							RECT RctWave;// 波形区域
							RctWave.left = rect.left + SCALE_WIDTH;
							RctWave.right = rect.right - SCALE_WIDTH;
							RctWave.top = rect.top;
							RctWave.bottom = rect.bottom - SCALE_WIDTH;
							
							RECT stTextRect;
							stTextRect.left = RctWave.left + 2;
							stTextRect.right = RctWave.left + 60;
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
								stTextRect.top = stFirstPoint.y - 25;
								stTextRect.bottom = stFirstPoint.y;
							
								FillRect(hdc, &stTextRect, hbr);
							}
							DeleteObject(hbr);
							ReleaseDC(hWndMain,hdc);
						}
						
						if(gbAvgCurveFlag[CurrentChannelNo] == ON)
							bgAvgChangeFlag = FALSE;//AVG曲线需要重画

						if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
						{
							float fDeltaDB = (float)(40 * log10(gfDefectZhi3[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
							for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
							{
								gSelectedAvgPointData[CurrentChannelNo][2][j].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][0][j].data, fDeltaDB);
							}
						}

						SaveParamFile(lpDefaultParamFile);
					}
					else
					{
						gfDefectZhi3[CurrentChannelNo] = 20;
					}

					displayDefectZhi(Wnd, gfDefectZhi3[CurrentChannelNo]);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
				else if(wParam == 'F')
				{
					if(wFaiZhiStepIndex < 1)
						wFaiZhiStepIndex++;
					else
						wFaiZhiStepIndex = 0;

					fFaiZhiStep = FAIZHISTEPS[wFaiZhiStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fFaiZhiStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnAvgCurveDefectZhi3Proc,Wnd,Msg,wParam,lParam);
}


