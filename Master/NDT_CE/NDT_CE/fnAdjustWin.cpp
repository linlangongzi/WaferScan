/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 校准窗口源文件
 * Filename: adjustWin.cpp
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
#include "recvParamWin.h"
#include "win_com.h"

HWND hWndAdjust = NULL;						//校准窗口句柄
HWND currentAdjustBtn= NULL;					//校准窗中当前选中的按钮

static HWND hBtnAdjustHeadType= NULL;				//探头类型按钮句柄
static HWND hBtnAdjustDistance1= NULL;			//声程1按钮句柄
static HWND hBtnAdjustDistance2= NULL;			//声程2按钮句柄
static HWND hBtnAdjustOffset= NULL;				//初始化偏移按钮句柄
static HWND hBtnAdjustStart= NULL;				//开始校准按钮句柄
static HWND adjustBtnList[5]={NULL,NULL,NULL,NULL,NULL};							//校准窗口的按钮表
static WNDPROC OldBtnAdjustHeadTypeProc = NULL;			//探头类型按钮默认处理过程
static WNDPROC OldBtnAdjustDistance1Proc = NULL;		//声程1按钮默认处理过程
static WNDPROC OldBtnAdjustDistance2Proc = NULL;		//声程2按钮默认处理过程
static WNDPROC OldBtnAdjustOffsetProc = NULL;			//初始化偏移按钮默认处理过程
static WNDPROC OldBtnAdjustStartProc = NULL;			//开始校准按钮默认处理过程

static WORD buttonNum = 5;					//按钮控件数
static BOOL bVertical = TRUE;
float fDistance1 = 50.0f;
float fDistance2 = 100.0f;
float fDistanceStep = 1.0f;
static WORD nDistanceStepIndex = 0;		// 距离步进索引
float fPianCha = 5.0f;
//static float fAmend = 0.0;
static float fAmendStep = 0.1;
float fDefectDepth = 1.0f;							//缺陷深度
float fDefectRadius = 1.0f;							//缺陷半径
static float fDefectDepthStep = 1.0f;				//缺陷深度步长
static float fDefectRadiusStep = 1.0f;				//缺陷半径步长
static WORD wDefectDepthStepIndex = 1;				//缺陷深度步长索引
static WORD wDefectRadiusStepIndex = 1;				//缺陷半径步长索引
// songchenguang add start 2010-12-09
static WORD nAmendStepIndex = 0;				// 当前步长序号
// songchenguang add end 2010-12-09
static float fUSStep = 0.01f;
static WORD wUSStepIndex = 0;

BOOL gbAdjustStart = FALSE;
BOOL gbAvgPeakFinished = FALSE;
CRITICAL_SECTION csAdjust;
BOOL gbAngleAdjustStart = FALSE;

ATOM registerAdjustChild(HINSTANCE hInstance);
static LRESULT CALLBACK adjustChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int adjust_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnAdjustHeadTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAdjustDistance1Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAdjustDistance2Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAdjustOffsetProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAdjustStartProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

extern void SetWaveSpeedLabelAndExecute(HWND hWnd);

extern HINSTANCE hInst;					//程序实例句柄
extern HWND hWndMain;
extern HWND hWndQuantitativeAnalysis;			//定量分析窗口句柄
extern HWND currentQuantitativeAnalysisBtn;		//定量分析窗中当前选中的按钮

extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern CRITICAL_SECTION csGate;
extern GATE_INFO GateInfos[2][3];
extern WORD CurrentChannelNo;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern SEND_PARAM_DATA SendParam_ad[2];
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern wchar_t gszTextPeak[20];
extern HWND hStatusBar;
extern PEAK_INFO gPeakInfos[2][2];
extern CRITICAL_SECTION csPeakInfo;
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern BOOL gbGuanLian;

extern HWND gCurrentWnd;			//当前显示的子窗口
extern BOOL gbHotAutoAdjustFlag;	//是否打开工艺参数快捷方式 
extern wchar_t gszTextStep[10];
extern BOOL gbHotRange;				// 快捷键声程
extern BOOL gbHotDB;				// 快捷键增益
extern GATE_INFO GateInfos[2][3];
extern GATE_INFO stOldGateInfos[2][3];

extern HWND hWndTlAdjust;	//调校窗口句柄
extern HWND currentTlAdjustBtn;	//调校窗口当前选中按钮
extern WORD gwDeviceType;	//设备类型 0:探伤仪	1:测厚仪 2:铁路专用
extern HWND hWndTlAdjust;	//调校窗口句柄


void SetDistance(HWND hWnd, const float& fDistance, const WORD& nIndex);
extern void SetDistanceGate(const WORD& nIndex);
extern void SetAngleLabelAndExecute();


void UpdateDistanceWnd(HWND hWnd, const float& fDistance)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fDistance);
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}

void SetZeroOffset(HWND hWnd, const float& fValue)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.2f"), fValue);
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);


}

void SetModeType(HWND hWnd, BOOL bTypeA)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(bTypeA)
	{
		wsprintf(strText, _T("声速和零点"));	
		DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	}
	else 
	{
		wsprintf(strText, _T("角度"));
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	}
}
void angleAdjust()
{
	float fZheSheJiao;
	float fRange = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, int(gPeakInfos[CurrentChannelNo][0].wDefectTime));
	float fDepth = fDefectDepth;

	if(fDepth / (fRange + fDefectRadius) <= 1)
	{
		fZheSheJiao = 90 - float(asin(double(fDepth / (fRange + fDefectRadius))) / PI * 180);

		DisplaySet_ad[CurrentChannelNo].fZheSheJiao = fZheSheJiao;

		SetAngleLabelAndExecute();
	}
	else
	{
		memset(gszTextPeak, 0, sizeof(gszTextPeak));

		swprintf(gszTextPeak, _T("校准失败"));

		SendMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
		return;
	}
	// 显示校准完成
	MessageBeep(0xFFFFFFFF);

	memset(gszTextPeak, 0, sizeof(gszTextPeak));

	swprintf(gszTextPeak, _T("校准角度:%.1f"),DisplaySet_ad[CurrentChannelNo].fZheSheJiao);

	SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
}
/**************************
*注册校准窗口
* @adjust    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerAdjustChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) adjustChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szAdjustChild;

	return RegisterClass(&wc);
}
/**************************
*校准窗口处理过程
* @adjust    hInstance[in]:
* @adjust		message[in]:
* @adjust		wParam[in]:
* @adjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK adjustChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			fDefectDepthStep = AMENDSTEPS[wDefectDepthStepIndex];
			fDefectRadiusStep = AMENDSTEPS[wDefectRadiusStepIndex];
		/*	if(!bVertical)
			{
				buttonNum = 4;
			}
			else
			{
				buttonNum = 5;
			}*/
			//画按钮控件
			hBtnAdjustHeadType = CreateWindow(_T("BUTTON"),_T("headTypeAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ADJUST_HEADTYPE,hInst,NULL);

			hBtnAdjustDistance1 = CreateWindow(_T("BUTTON"),_T("distance1Adjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ADJUST_DISTANCE1,hInst,NULL);

			hBtnAdjustDistance2 = CreateWindow(_T("BUTTON"),_T("distance2Adjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ADJUST_DISTANCE2,hInst,NULL);

			hBtnAdjustOffset = CreateWindow(_T("BUTTON"),_T("offsetAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ADJUST_OFFSET,hInst,NULL);

			hBtnAdjustStart = CreateWindow(_T("BUTTON"),_T("startAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_ADJUST_START,hInst,NULL);
					
			adjustBtnList[0] = hBtnAdjustHeadType;
			adjustBtnList[1] = hBtnAdjustDistance1;
			adjustBtnList[2] = hBtnAdjustDistance2;
			adjustBtnList[3] = hBtnAdjustOffset;
			adjustBtnList[4] = hBtnAdjustStart;
			
			//按钮控件的自定义处理过程
			OldBtnAdjustHeadTypeProc = (WNDPROC) GetWindowLong(hBtnAdjustHeadType,GWL_WNDPROC);
			SetWindowLong(hBtnAdjustHeadType,GWL_WNDPROC,(LONG)btnAdjustHeadTypeProc);
			
			OldBtnAdjustDistance1Proc = (WNDPROC) GetWindowLong(hBtnAdjustDistance1,GWL_WNDPROC);
			SetWindowLong(hBtnAdjustDistance1,GWL_WNDPROC,(LONG)btnAdjustDistance1Proc);
			
			OldBtnAdjustDistance2Proc = (WNDPROC) GetWindowLong(hBtnAdjustDistance2,GWL_WNDPROC);
			SetWindowLong(hBtnAdjustDistance2,GWL_WNDPROC,(LONG)btnAdjustDistance2Proc);

			OldBtnAdjustOffsetProc = (WNDPROC) GetWindowLong(hBtnAdjustOffset,GWL_WNDPROC);
			SetWindowLong(hBtnAdjustOffset,GWL_WNDPROC,(LONG)btnAdjustOffsetProc);
			
			OldBtnAdjustStartProc = (WNDPROC) GetWindowLong(hBtnAdjustStart,GWL_WNDPROC);
			SetWindowLong(hBtnAdjustStart,GWL_WNDPROC,(LONG)btnAdjustStartProc);
		
			currentAdjustBtn = adjustBtnList[0];//默认当前选中探头类型按钮

			SetFocus(currentAdjustBtn);//选中按钮获得输入焦点
			if(!gbHotAutoAdjustFlag)
				gCurrentFocusBtn = currentAdjustBtn;

			SetDistance(NULL, fDistance1, 1);
			SetDistance(NULL, fDistance2, 2);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
		/*	if(!bVertical)
			{
				ShowWindow(hBtnAdjustStart,SW_HIDE);
				buttonNum = 4;
			}*/
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				if(!gbHotAutoAdjustFlag)
				{					
					if(hWndTlAdjust != NULL)//当执行铁路专用的校准返回时
					{
						ShowWindow(hWndTlAdjust,SW_SHOW);
						SetFocus(currentTlAdjustBtn);//主窗口获得输入焦点
						
						gCurrentFocusBtn = currentTlAdjustBtn;
						gCurrentWnd = hWndTlAdjust;
					}
					else
					{
						ShowWindow(hWndQuantitativeAnalysis,SW_SHOW);
						SetFocus(currentQuantitativeAnalysisBtn);//主窗口获得输入焦点
						
						gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
						gCurrentWnd = hWndQuantitativeAnalysis;
					}

					DestroyWindow(hWndAdjust);
					hWndAdjust = NULL;
				}
				else
				{
					if(gCurrentWnd)//有子菜单窗口
					{
						ShowWindow(gCurrentWnd, SW_SHOW);
						SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
					}					
					else
					{
						SetMenuStateFlag(FALSE);
						SetFocus(hWndMain);//主窗口获得输入焦点
					}
					DestroyWindow(hWndAdjust);
					hWndAdjust = NULL;
					
					gbHotAutoAdjustFlag = FALSE;
				}
			}
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			adjust_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnAdjustHeadType)
			{
				SetModeType(pdis->hwndItem, bVertical);
			}
			else if(pdis->hwndItem == hBtnAdjustDistance1)
			{
				if(bVertical)
					UpdateDistanceWnd(pdis->hwndItem, fDistance1);
				else
				{
					wchar_t strText[20];
					memset(strText, 0, sizeof(strText));
			
					swprintf(strText, _T("%.1f"), SendParam_ad[CurrentChannelNo].fQianYan);
					DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
				}
			}
			else if(pdis->hwndItem == hBtnAdjustDistance2)
			{
				if(bVertical)
					UpdateDistanceWnd(pdis->hwndItem, fDistance2);
				else
					UpdateDistanceWnd(pdis->hwndItem, fDefectDepth);
			}
			else if(pdis->hwndItem == hBtnAdjustOffset)
			{
				if(bVertical)
					SetZeroOffset(pdis->hwndItem, SendParam_ad[CurrentChannelNo].nAmend);
				else
					UpdateDistanceWnd(pdis->hwndItem, fDefectRadius);
			}

			// 检测按钮句柄，显示相应的步长
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentAdjustBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnAdjustDistance1)
					{
						if(bVertical)
						{
							swprintf(gszTextStep, _T("%.1f"), fDistanceStep);
						}
						else
							swprintf(gszTextStep, _T("%.1f"), fAmendStep);
					}
					else if(pdis->hwndItem == hBtnAdjustDistance2)
					{
						if(bVertical)
						{
							swprintf(gszTextStep, _T("%.1f"), fDistanceStep);
						}
						else
							swprintf(gszTextStep, _T("%.1f"), fDefectDepthStep);
							
					}
					else if(pdis->hwndItem == hBtnAdjustOffset)
					{
						if(bVertical)
							swprintf(gszTextStep, _T("%.2f"), fUSStep);
						else
							swprintf(gszTextStep, _T("%.1f"), fDefectRadiusStep);
					}
					else
						swprintf(gszTextStep, _T(""));

					PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}
			break;
		case WM_DESTROY:
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载校准窗口按钮图片
* @adjust    hInstance[in]:
* @adjust		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int adjust_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	if(bVertical)
	{
		const BTN_BMP_INFO tbl_adjustBtnBitmap[]=
		{
			{IDC_ADJUST_HEADTYPE	,IDB_BITMAP_JIAOZHUNLEIXING_NOSEL_R	 ,  IDB_BITMAP_JIAOZHUNLEIXING_SELECT_R		},
			{IDC_ADJUST_DISTANCE1	,IDB_BITMAP_QISHIJULI_NOSEL_R	 ,  IDB_BITMAP_QISHIJULI_SELECT_R		},
			{IDC_ADJUST_DISTANCE2	,IDB_BITMAP_ZHONGZHIJULI_NOSEL_R ,  IDB_BITMAP_ZHONGZHIJULI_SELETC_R	},
			{IDC_ADJUST_OFFSET	,IDB_BITMAP_LINGDIAN_NOSEL_R ,  IDB_BITMAP_LINGDIAN_SELECT_R	},
			{IDC_ADJUST_START	,IDB_BITMAP_JIAOZHUNKAISHI5_NOSEL_R ,  IDB_BITMAP_JIAOZHUNKAISHI5_SELECT_R	},
			{	-1			,	-1					 ,	-1							}    
		};
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_adjustBtnBitmap);
	}
	else
	{
		const BTN_BMP_INFO tbl_adjustBtnBitmap[]=
		{
			{IDC_ADJUST_HEADTYPE	,IDB_BITMAP_JIAOZHUNLEIXING_NOSEL_R	 ,  IDB_BITMAP_JIAOZHUNLEIXING_SELECT_R		},
			{IDC_ADJUST_DISTANCE1	,IDB_BITMAP_TANTOUQIANYAN_NOSEL_R ,  IDB_BITMAP_TANTOUQIANYAN_SELECT_R	},
			{IDC_ADJUST_DISTANCE2	,IDB_BITMAP_QUEXIANSHENDU_NOSEL_R	 ,  IDB_BITMAP_QUEXIANSHENDU_SELECT_R	},
			{IDC_ADJUST_OFFSET	,IDB_BITMAP_QUEXIANBANJING_NOSEL_R ,  	IDB_BITMAP_QUEXIANBANJING_SELECT_R},
			{IDC_ADJUST_START	,IDB_BITMAP_JIAOZHUNKAISHI5_NOSEL_R ,  IDB_BITMAP_JIAOZHUNKAISHI5_SELECT_R	},			
			{	-1			,	-1					 ,	-1							}    
		};
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_adjustBtnBitmap);
	}
	return ReturnVal;
}
	

	

/**************************
*探头类型按钮自定义处理过程
* @adjust    hInstance[in]:
* @adjust		message[in]:
* @adjust		wParam[in]:
* @adjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAdjustHeadTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(adjustBtnList[i] != currentAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAdjustBtn = adjustBtnList[i];
					SetFocus(currentAdjustBtn);
					if(!gbHotAutoAdjustFlag)
						gCurrentFocusBtn = currentAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(adjustBtnList[i] != currentAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAdjustBtn = adjustBtnList[i];
					SetFocus(currentAdjustBtn);
					if(!gbHotAutoAdjustFlag)
						gCurrentFocusBtn = currentAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{					
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					if(gwDeviceType != 1)	//测厚时无角度校准
					{
						bVertical = !bVertical;

						SetModeType(Wnd, bVertical);
						
					/*	if(bVertical)
						{
							ShowWindow(hBtnAdjustStart,SW_SHOW);
							buttonNum = 5;
						}
						else
						{
							ShowWindow(hBtnAdjustStart,SW_HIDE);
							buttonNum = 4;
						}*/
					/*
						LPDRAWITEMSTRUCT pDis;
						
						pDis->CtlID = IDC_ADJUST_DISTANCE1;
						pDis->itemState = ODS_SELECTED;
						pDis->hDC = GetDC(hBtnAdjustDistance1);
						adjust_DrawButtonBitmap(hInst,pDis);

						pDis->CtlID = IDC_ADJUST_DISTANCE2;
						pDis->itemState = ODS_SELECTED;
						pDis->hDC = GetDC(hBtnAdjustDistance2);
						adjust_DrawButtonBitmap(hInst,pDis);

						pDis->CtlID = IDC_ADJUST_OFFSET;
						pDis->itemState = ODS_SELECTED;
						pDis->hDC = GetDC(hBtnAdjustOffset);
						adjust_DrawButtonBitmap(hInst,pDis);
					*/
						SetFocus(hBtnAdjustDistance1);
						SetFocus(hBtnAdjustDistance2);
						SetFocus(hBtnAdjustOffset);
						SetFocus(hBtnAdjustStart);
						SetFocus(Wnd);
					}
					
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					if(!gbHotAutoAdjustFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(!gbHotAutoAdjustFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(gbHotAutoAdjustFlag)
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
	return CallWindowProc(OldBtnAdjustHeadTypeProc,Wnd,Msg,wParam,lParam);
}


void SetDistance(HWND hWnd, const float& fDistance, const WORD& nIndex)
{
	if(hWnd)
	{
		UpdateDistanceWnd(hWnd, fDistance);
	}

	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);

	GateInfos[CurrentChannelNo][nIndex-1].bOpen = TRUE;
	//GateInfos[CurrentChannelNo][nIndex-1].fFrontGate = fDistance - fPianCha;
	//GateInfos[CurrentChannelNo][nIndex-1].fBackGate = fDistance + fPianCha;
	float fFront = fDistance - 
		(GateInfos[CurrentChannelNo][nIndex-1].fBackGate - GateInfos[CurrentChannelNo][nIndex-1].fFrontGate)/2;

	float fBack = fDistance + 
		(GateInfos[CurrentChannelNo][nIndex-1].fBackGate - GateInfos[CurrentChannelNo][nIndex-1].fFrontGate)/2;

	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);

	BOOL bOpen = TRUE;
	float fKuanDu = fBack - fFront;

	// 开启标志
	SetOpenFlag(NULL, bOpen, nIndex);
/*
	// 起点
	SetQiDian(NULL, nIndex, fFront, fBack);
	// 宽度
	SetKuanDu(NULL, nIndex, fFront, fKuanDu);
*/	
}
/**************************
*声程1按钮自定义处理过程
* @adjust    hInstance[in]:
* @adjust		message[in]:
* @adjust		wParam[in]:
* @adjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAdjustDistance1Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_CREATE:
		fDistanceStep = AMENDSTEPS[nDistanceStepIndex];
		fAmendStep = AMENDSTEPS[nAmendStepIndex];
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
					while(adjustBtnList[i] != currentAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAdjustBtn = adjustBtnList[i];
					SetFocus(currentAdjustBtn);
					if(!gbHotAutoAdjustFlag)
						gCurrentFocusBtn = currentAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(adjustBtnList[i] != currentAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAdjustBtn = adjustBtnList[i];
					SetFocus(currentAdjustBtn);
					if(!gbHotAutoAdjustFlag)
						gCurrentFocusBtn = currentAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(bVertical)
					{
						if(fDistance1 > fDistanceStep)
							fDistance1 -= fDistanceStep;
						else
							fDistance1 = 0;

						SetDistance(Wnd, fDistance1, 1);
					}
					else
					{
						if(SendParam_ad[CurrentChannelNo].fQianYan > fAmendStep)
							SendParam_ad[CurrentChannelNo].fQianYan -= fAmendStep;
						else
							SendParam_ad[CurrentChannelNo].fQianYan = 0.0f;

						SetObliqueProbeQianYan(Wnd);
					}
				}
				else if(wParam == VK_RIGHT)
				{
/*					if(fDistance1 < (fDistance2 - fPianCha*2))
						fDistance1 += fDistanceStep;
					else
						fDistance1 = fDistance2 - fPianCha*2;
*/				
					if(bVertical)
					{
						float fMax = MAX_RANGE - fPianCha;
						if(fDistance1 < fMax)
							fDistance1 += fDistanceStep;
						else
							fDistance1 = fMax;

						SetDistance(Wnd, fDistance1, 1);
					}
					else
					{
						if(SendParam_ad[CurrentChannelNo].fQianYan < 100)
							SendParam_ad[CurrentChannelNo].fQianYan += fAmendStep;
						else
							SendParam_ad[CurrentChannelNo].fQianYan = 100;

						SetObliqueProbeQianYan(Wnd);
					}
				}
				else if(wParam == 'F') // 步长
				{
					if(!bVertical)
					{
						if(nAmendStepIndex < 2)
							nAmendStepIndex++;
						else
							nAmendStepIndex = 0;

						fAmendStep = AMENDSTEPS[nAmendStepIndex];

						memset(gszTextStep, 0, sizeof(gszTextStep));
						swprintf(gszTextStep, _T("%.1f"), fAmendStep);
						SendMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
					}
				else
				{
					if(nDistanceStepIndex < 2)
						nDistanceStepIndex++;
					else
						nDistanceStepIndex = 0;

					fDistanceStep = AMENDSTEPS[nDistanceStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fDistanceStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					if(!gbHotAutoAdjustFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(!gbHotAutoAdjustFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(gbHotAutoAdjustFlag)
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
	return CallWindowProc(OldBtnAdjustDistance1Proc,Wnd,Msg,wParam,lParam);
}
/**************************
*声程2按钮自定义处理过程
* @adjust    hInstance[in]:
* @adjust		message[in]:
* @adjust		wParam[in]:
* @adjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAdjustDistance2Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_CREATE:
		fDistanceStep = AMENDSTEPS[nDistanceStepIndex];
		fAmendStep = AMENDSTEPS[nAmendStepIndex];
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
					while(adjustBtnList[i] != currentAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAdjustBtn = adjustBtnList[i];
					SetFocus(currentAdjustBtn);
					if(!gbHotAutoAdjustFlag)
						gCurrentFocusBtn = currentAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(adjustBtnList[i] != currentAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAdjustBtn = adjustBtnList[i];
					SetFocus(currentAdjustBtn);
					if(!gbHotAutoAdjustFlag)
						gCurrentFocusBtn = currentAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
/*					float fMin = fDistance1 + fPianCha*2 + fDistanceStep;
					if(fDistance2 > fMin)
						fDistance2 -= fDistanceStep;
					else
						fDistance2 = fDistance1 + fPianCha*2;					
*/					
					if(bVertical)
					{
						if(fDistance2 > fDistanceStep)
							fDistance2 -= fDistanceStep;
						else
							fDistance2 = 0;

						SetDistance(Wnd, fDistance2, 2);
					}
					else
					{
						if(fDefectDepth > fDefectDepthStep)
							fDefectDepth -= fDefectDepthStep;
						else
							fDefectDepth = 0;

						UpdateDistanceWnd(Wnd, fDefectDepth);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(bVertical)
					{
						float fMax = MAX_RANGE - fPianCha;
						if(fDistance2 < fMax)
							fDistance2 += fDistanceStep;
						else
							fDistance2 = fMax;

						SetDistance(Wnd, fDistance2, 2);
					}
					else
					{
						if(fDefectDepth + fDefectDepthStep < MAX_RANGE)
							fDefectDepth += fDefectDepthStep;
						else
							fDefectDepth = MAX_RANGE;

						UpdateDistanceWnd(Wnd, fDefectDepth);
					}
				}
				else if(wParam == 'F')
				{
					if(bVertical)
					{
					if(nDistanceStepIndex < 2)
						nDistanceStepIndex++;
					else
						nDistanceStepIndex = 0;

					fDistanceStep = AMENDSTEPS[nDistanceStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fDistanceStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
					}
					else
					{
						if(wDefectDepthStepIndex < 3)
							wDefectDepthStepIndex++;
						else
							wDefectDepthStepIndex = 0;

						fDefectDepthStep = AMENDSTEPS[wDefectDepthStepIndex];

						memset(gszTextStep, 0, sizeof(gszTextStep));
						swprintf(gszTextStep, _T("%.1f"), fDefectDepthStep);
						SendMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					if(!gbHotAutoAdjustFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(!gbHotAutoAdjustFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(gbHotAutoAdjustFlag)
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
	return CallWindowProc(OldBtnAdjustDistance2Proc,Wnd,Msg,wParam,lParam);
}


/**************************
*初始化偏移按钮自定义处理过程
* @adjust    hInstance[in]:
* @adjust		message[in]:
* @adjust		wParam[in]:
* @adjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAdjustOffsetProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_CREATE:
//		fAmend = SendParam_ad[CurrentChannelNo].nAmend;
		fUSStep = USSTEPS[wUSStepIndex];
		break;

	case WM_KEYDOWN:
		{
			int i = 0;
			if(wParam == VK_ESCAPE)
			{	
				if(!gbHotAutoAdjustFlag)
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
				else
				{
					if(gCurrentWnd)//有子菜单窗口
					{
						ShowWindow(gCurrentWnd, SW_SHOW);
						SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
					}

					DestroyWindow(hWndAdjust);
					hWndAdjust = 0;
					if(!gCurrentWnd)//没有子菜单窗口
					{
						SetMenuStateFlag(FALSE);
						SetFocus(hWndMain);//主窗口获得输入焦点
					}
					
					gbHotAutoAdjustFlag = FALSE;
				}
			}
		
			else if(wParam == VK_UP)
			{
				while(adjustBtnList[i] != currentAdjustBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentAdjustBtn = adjustBtnList[i];
				SetFocus(currentAdjustBtn);
				if(!gbHotAutoAdjustFlag)
						gCurrentFocusBtn = currentAdjustBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(adjustBtnList[i] != currentAdjustBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentAdjustBtn = adjustBtnList[i];
				SetFocus(currentAdjustBtn);
				if(!gbHotAutoAdjustFlag)
						gCurrentFocusBtn = currentAdjustBtn;
			}
			else if(wParam == VK_RETURN)
			{
			}
			else if(wParam == VK_LEFT)
			{
				if(bVertical)
				{
					if(SendParam_ad[CurrentChannelNo].nAmend > fUSStep)
						SendParam_ad[CurrentChannelNo].nAmend -= fUSStep;
					else
						SendParam_ad[CurrentChannelNo].nAmend = 0;

					SetZeroOffset(Wnd, SendParam_ad[CurrentChannelNo].nAmend);

//					SendParam_ad[CurrentChannelNo].nAmend = fAmend;
					SetAmendLableAndExecute(NULL);
				}
				else
				{
					if(fDefectRadius > fDefectRadiusStep)
						fDefectRadius -= fDefectRadiusStep;
					else
						fDefectRadius = 0;

					UpdateDistanceWnd(Wnd, fDefectRadius);
				}
			}
			else if(wParam == VK_RIGHT)
			{
				if(bVertical)
				{
					if((SendParam_ad[CurrentChannelNo].nAmend + fUSStep) < 100)
						SendParam_ad[CurrentChannelNo].nAmend += fUSStep;
					else
						SendParam_ad[CurrentChannelNo].nAmend = 100;

					SetZeroOffset(Wnd, SendParam_ad[CurrentChannelNo].nAmend);

//					SendParam_ad[CurrentChannelNo].nAmend = fAmend;
					SetAmendLableAndExecute(NULL);
				}
				else
				{
					if(fDefectRadius + fDefectRadiusStep < MAX_RANGE)
						fDefectRadius += fDefectRadiusStep;
					else
						fDefectRadius = MAX_RANGE;

					UpdateDistanceWnd(Wnd, fDefectRadius);
				}
			}
			else if(wParam == 'F') // 步长
			{
				if(bVertical)
				{
					if(wUSStepIndex < 2)
						wUSStepIndex++;
					else
						wUSStepIndex = 0;

					fUSStep = USSTEPS[wUSStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.2f"), fUSStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
				else
				{
					if(wDefectRadiusStepIndex < 3)
						wDefectRadiusStepIndex++;
					else
						wDefectRadiusStepIndex = 0;

					fDefectRadiusStep = AMENDSTEPS[wDefectRadiusStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fDefectRadiusStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}
			else if(wParam == '\t')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '1')
			{
				if(!gbHotAutoAdjustFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '2')
			{
				if(!gbHotAutoAdjustFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '3')
			{
				SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			}
			else if(wParam == '4')
			{
				if(gbHotAutoAdjustFlag)
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
	return CallWindowProc(OldBtnAdjustOffsetProc,Wnd,Msg,wParam,lParam);
}


void AutoJustParam(HWND hWnd)
{
	// 写标题
	if(hWnd)
	{
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));

		wsprintf(strText, _T("开始"));
		
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	// 计算校准值
	
	// 查询闸门一伤时， 闸门二伤时, 计算声速
	InitializeCriticalSection(&csPeakInfo);
	EnterCriticalSection(&csPeakInfo);

	WORD nWaveSpeed = 1;
	nWaveSpeed = 
		WORD( (fDistance2 - fDistance1)* ((ADFREQUENCY*1000000) / 1000) / (fabs(gPeakInfos[CurrentChannelNo][1].wDefectTime - gPeakInfos[CurrentChannelNo][0].wDefectTime)/2.0) );

	if(nWaveSpeed < MAX_WAVE_SPEED && nWaveSpeed > MIN_WAVE_SPEED)
	{
		if(!gbGuanLian)
			DisplaySet_ad[CurrentChannelNo].nWaveSpeed = nWaveSpeed;
		else
		{
			DisplaySet_ad[0].nWaveSpeed = nWaveSpeed;
			DisplaySet_ad[1].nWaveSpeed = nWaveSpeed;
		}
	}
	else
	{
		memset(gszTextPeak, 0, sizeof(gszTextPeak));
//		swprintf(gszTextPeak, _T("校准失败"));
//		swprintf(gszTextPeak, _T("S: %d"), nWaveSpeed);
		swprintf(gszTextPeak, _T("%.2f, %.2f"), gPeakInfos[CurrentChannelNo][1].wDefectTime, gPeakInfos[CurrentChannelNo][0].wDefectTime);

		SendMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);

		LeaveCriticalSection(&csPeakInfo);
		DeleteCriticalSection(&csPeakInfo);
		return;
	}

	// 计算TS1'
	float nNewTimeS1 = fDistance1 / (DisplaySet_ad[CurrentChannelNo].nWaveSpeed * 1000 ) *1000000; 
	// 计算新的修正值，发下去
    float fAmend = float(SendParam_ad[CurrentChannelNo].nAmend + (gPeakInfos[CurrentChannelNo][0].wDefectTime / float(ADFREQUENCY) - nNewTimeS1*2));

	// 防止负值
	if(fAmend < 0.0)
		fAmend = 0.0f;

	if(!gbGuanLian)
	{
		SendParam_ad[CurrentChannelNo].nAmend = fAmend;
	}
	else
	{
		for(int i=0; i<2; i++)
		{
			SendParam_ad[i].nAmend = fAmend;
		}
	}
	
	LeaveCriticalSection(&csPeakInfo);
	DeleteCriticalSection(&csPeakInfo);
	
	SetWaveSpeedLabelAndExecute(NULL);

	// 显示校准完成
	MessageBeep(0xFFFFFFFF);

	memset(gszTextPeak, 0, sizeof(gszTextPeak));

	swprintf(gszTextPeak, _T("Z:%.1f,V:%d"),SendParam_ad[CurrentChannelNo].nAmend, DisplaySet_ad[CurrentChannelNo].nWaveSpeed);

	SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);

	//wangbingfu modify at 2010-09-21
	SetZeroOffset(hBtnAdjustOffset, fAmend);
	SetAmendLableAndExecute(NULL);
}
/**************************
*开始校准按钮自定义处理过程
* @adjust    hInstance[in]:
* @adjust		message[in]:
* @adjust		wParam[in]:
* @adjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAdjustStartProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(adjustBtnList[i] != currentAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAdjustBtn = adjustBtnList[i];
					SetFocus(currentAdjustBtn);
					if(!gbHotAutoAdjustFlag)
						gCurrentFocusBtn = currentAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(adjustBtnList[i] != currentAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAdjustBtn = adjustBtnList[i];
					SetFocus(currentAdjustBtn);
					if(!gbHotAutoAdjustFlag)
						gCurrentFocusBtn = currentAdjustBtn;
				}
				else if(wParam == VK_LEFT)
				{
				}
				else if(wParam == VK_RIGHT)
				{
				}
				else if(wParam == VK_RETURN)
				{
					if(bVertical)
					{
						memset(gszTextPeak, 0, sizeof(gszTextPeak));
						swprintf(gszTextPeak, _T("校准进行中..."));

						SendMessage(hStatusBar, SB_SETTEXT, 
									(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);

						// 进行修正
//						SendParam_ad[CurrentChannelNo].nAmend = fAmend;
//						SetAmendLableAndExecute(NULL);
						ReceiveParam_ad[CurrentChannelNo].fDelay = 0;
						SetDelayLableAndExecute(NULL, CurrentChannelNo);
						Sleep(1);

						InitializeCriticalSection(&csAdjust);
						EnterCriticalSection(&csAdjust);
						gbAdjustStart = TRUE;
						LeaveCriticalSection(&csAdjust);
						DeleteCriticalSection(&csAdjust);

						SetFocus(NULL);
					/*	BOOL bFinished = FALSE;
						do
						{
							InitializeCriticalSection(&csAdjust);
							EnterCriticalSection(&csAdjust);
							bFinished = gbAvgPeakFinished;
							LeaveCriticalSection(&csAdjust);
							DeleteCriticalSection(&csAdjust);

							Sleep(5);
						}while(!bFinished);

						AutoJustParam(Wnd);
						
						InitializeCriticalSection(&csAdjust);
						EnterCriticalSection(&csAdjust);
						gbAvgPeakFinished = FALSE;
						LeaveCriticalSection(&csAdjust);
						DeleteCriticalSection(&csAdjust);
						SetFocus(currentAdjustBtn);*/
					}
					else
					{
						SetDistanceGate(1);
					//	SetOpenFlag(NULL, FALSE, 2);
						Sleep(20);
						memset(gszTextPeak, 0, sizeof(gszTextPeak));
						swprintf(gszTextPeak, _T("校准进行中..."));
						
						SendMessage(hStatusBar, SB_SETTEXT, 
									(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
						
						InitializeCriticalSection(&csAdjust);
						EnterCriticalSection(&csAdjust);
						gbAngleAdjustStart = TRUE;
						LeaveCriticalSection(&csAdjust);
						DeleteCriticalSection(&csAdjust);
						
						SetFocus(NULL);
					}
					
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					if(!gbHotAutoAdjustFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(!gbHotAutoAdjustFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(gbHotAutoAdjustFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '9')
				{
					if(currentAdjustBtn == hBtnAdjustStart)
					{
						SetFocus(currentAdjustBtn);
						if(bVertical)
							SetZeroOffset(hBtnAdjustOffset, SendParam_ad[CurrentChannelNo].nAmend);
						else
						{
							SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);		
							SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);					
							SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
						}
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
	return CallWindowProc(OldBtnAdjustStartProc,Wnd,Msg,wParam,lParam);
}
