/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 测厚测量内容窗口源文件
 * Filename: chMeasureContentWin.cpp
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


HWND hWndChMeasureContent = NULL;									//测厚测量内容窗口句柄
HWND currentChMeasureContentBtn = NULL;							//测厚测量内容窗口当前选中的按钮

WORD gwMeasureDisplayIndex = 0;								//测量显示索引
WORD gwTuCengType = 0;										//涂层材料类型号
WORD gwTuCengSpeed = 0;										//涂层声速	
static HWND hBtnChMeasureContentDisplay = NULL;					//报警号按钮句柄
static HWND hBtnChMeasureContentOnOff = NULL;						//开关按钮句柄
static HWND hBtnChMeasureContentStandard = NULL;					//标称值按钮句柄
static HWND hBtnChMeasureContentMode = NULL;						//差值模式按钮句柄
static HWND hBtnChMeasureContentTuCengMaterial = NULL;				//涂层材料按钮句柄
static HWND hBtnChMeasureContentTuCengSpeed = NULL;				//涂层声速按钮句柄

static HWND chMeasureContentBtnList[6] ={NULL,NULL,NULL,NULL,NULL,NULL};						//测厚测量内容窗口的按钮表
static WNDPROC OldBtnChMeasureContentDisplayProc = NULL;	//报警号按钮默认处理过程
static WNDPROC OldBtnChMeasureContentOnOffProc = NULL;		//开关按钮默认处理过程
static WNDPROC OldBtnChMeasureContentStandardProc = NULL;	//标称值按钮默认处理过程
static WNDPROC OldBtnChMeasureContentModeProc = NULL;		//差值模式按钮默认处理过程
static WNDPROC OldBtnChMeasureContentTuCengMaterialProc = NULL;		//涂层材料按钮默认处理过程
static WNDPROC OldBtnChMeasureContentTuCengSpeedProc = NULL;		//涂层声速按钮默认处理过程

static WORD buttonNum = 6;		//按钮控件数
static float fBiaoChengZhiStep = 1;		//工件厚度步长
static WORD wBiaoChengZhiStepIndex = 1;	//工件厚度步长索引
static WORD wUserDefineWaveSpeed = 6000;  		//自定义声速
static WORD nWaveSpeedStep = 1;			//声速步长
static WORD nWaveSpeedStepIndex = 0;	//声速步长索引


ATOM registerChMeasureContentChild(HINSTANCE hInstance);
static LRESULT CALLBACK chMeasureContentChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chMeasureContent_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChMeasureContentDisplayProc( HWND Wnd, 
													UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChMeasureContentOnOffProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChMeasureContentStandardProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChMeasureContentModeProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChMeasureContentTuCengMaterialProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChMeasureContentTuCengSpeedProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//程序实例句柄

//extern HWND hWndMode;			//差值模式窗口句柄
//extern HWND hWndStandard;		//标称值窗口句柄 
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndChMeasureCfg;						//测厚测量设置窗口句柄
extern HWND currentChMeasureCfgBtn;					//测厚测量设置窗口当前选中的按钮
extern MEASURE_DISPLAY_ALL gstMeasureDisplay;				//测量显示
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;
extern HWND gCurrentWnd;
extern MEASURE_DISPLAY_ALL gstMeasureDisplay;


/**************************
*显示开关
* @param    hWnd[in]:窗口句柄
* @param    wMode[in]:模式
* @return   ATOM :
* @since    1.00
***************************/
void displayMeasureDisplaySwitch(HWND hWnd, WORD wIndex)
{
	BOOL bSwitch = OFF;
	if(wIndex == 0)
		bSwitch = gstMeasureDisplay.stMax.bSwitch;
	else if(wIndex == 1)
		bSwitch = gstMeasureDisplay.stMin.bSwitch;
	else if(wIndex == 2)
		bSwitch = gstMeasureDisplay.stAverage.bSwitch;
	else if(wIndex == 3)
		bSwitch = gstMeasureDisplay.stCurrent.bSwitch;
	else if(wIndex == 4)
		bSwitch = gstMeasureDisplay.stChaZhi.bSwitch;
	
	if(bSwitch == ON)
		DrawLableText(hWnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	else
		DrawLableText(hWnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}

/**************************
*显示测量显示
* @param    hWnd[in]:窗口句柄
* @param    wItem[in]:测量显示项
* @return   ATOM :
* @since    1.00
***************************/
void displayMeasureDisplay(HWND hWnd, WORD wItem)
{
	if(wItem == 0)
		DrawLableText(hWnd, _T("最大值"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	else if(wItem == 1)
		DrawLableText(hWnd, _T("最小值"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	else if(wItem == 2)
		DrawLableText(hWnd, _T("平均值"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	else if(wItem == 3)
		DrawLableText(hWnd, _T("当前值"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	else if(wItem == 4)
		DrawLableText(hWnd, _T("差值"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	
	displayMeasureDisplaySwitch(hBtnChMeasureContentOnOff, wItem);
}

/**************************
*显示差值模式
* @param    hWnd[in]:窗口句柄
* @param    wMode[in]:模式
* @return   ATOM :
* @since    1.00
***************************/
void displayChaZhiMode(HWND hWnd, WORD wMode)
{
	if(wMode == 0)
		DrawLableText(hWnd, _T("绝对值"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	else
		DrawLableText(hWnd, _T("相对值"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}

/**************************
*显示标称值
* @param    hWnd[in]:窗口句柄
* @param    fValue[in]:值
* @return   ATOM :
* @since    1.00
***************************/
void displayBiaoChengZhi(HWND hWnd, float fValue)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fValue);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}

/**************************
*显示涂层材料
* @param    hWnd[in]:窗口句柄
* @param    fValue[in]:值
* @return   ATOM :
* @since    1.00
***************************/
static void displayTuCengMaterial(HWND hWnd, WORD wMaterialType)
{
	switch(wMaterialType)
	{
		case 0:
			DrawLableText(hWnd, _T("自定义"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		case 1:
			DrawLableText(hWnd, _T("铝"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		case 2:
			DrawLableText(hWnd, _T("铜"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		case 3:
			DrawLableText(hWnd, _T("硫化橡胶"), 22, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		case 4:
			DrawLableText(hWnd, _T("铬"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		case 5:
			DrawLableText(hWnd, _T("锡"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		case 6:
			DrawLableText(hWnd, _T("锌"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		case 7:
			DrawLableText(hWnd, _T("塑料PVC"), 22, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		case 8:
			DrawLableText(hWnd, _T("环氧树脂"), 22, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		case 9:
			DrawLableText(hWnd, _T("银"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		case 10:
			DrawLableText(hWnd, _T("金"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		case 11:
			DrawLableText(hWnd, _T("油漆"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		case 12:
			DrawLableText(hWnd, _T("搪瓷"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		case 13:
			DrawLableText(hWnd, _T("天然橡胶"), 22, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			break;
		default:
			break;						
	}
}
/**************************
*显示涂层声速
* @param    hWnd[in]:窗口句柄
* @param    fValue[in]:值
* @return   ATOM :
* @since    1.00
***************************/

static void displayTuCengSpeed(HWND hWnd, WORD wWaveSpeed)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%d"), wWaveSpeed);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}
static WORD tuCengSpeedToType(WORD wWaveSpeed)
{
    WORD wMaterialType = 0;
	if(wWaveSpeed == 6305)
	{
		wMaterialType = 1;	//铝
	}
	if(wWaveSpeed == 4700)
	{
		wMaterialType = 2;	//铜
	}
	else if(wWaveSpeed == 2311)
	{
		wMaterialType = 3;	//硫化橡胶
	}
	else if(wWaveSpeed == 6200)
	{
		wMaterialType = 4;	//铬
	}
	else if(wWaveSpeed == 3327)
	{
		wMaterialType = 5;	//锡
	}
	else if(wWaveSpeed == 4216)
	{
		wMaterialType = 6;	//锌
	}
	else if(wWaveSpeed == 2388)
	{
		wMaterialType = 7;	//塑料（PVC）
	}
	else if(wWaveSpeed == 2540)
	{
		wMaterialType = 8;	//环氧树脂
	}
	else if(wWaveSpeed == 3607)
	{
		wMaterialType = 9;	//银
	}
	else if(wWaveSpeed == 3251)
	{
		wMaterialType = 10;	//金
	}
	else if(wWaveSpeed == 2550)
	{
		wMaterialType = 11;	//油漆
	}
	else if(wWaveSpeed == 5639)
	{
		wMaterialType = 12;	//搪瓷
	}
	else if(wWaveSpeed == 1510)
	{
		wMaterialType = 13;	//天然橡胶
	}
	else
	{
		wMaterialType = 0;	//自定义
	}

	return wMaterialType;
}

WORD tuCengTypeToWaveSpeed(WORD wType)
{
	WORD wWaveSpeed = 0;

	if(wType == 1)
	{
		wWaveSpeed = 6305;			
	}
	else if(wType == 2)
	{
		wWaveSpeed = 4700;			
	}
	else if(wType == 3)
	{
		wWaveSpeed = 2311;
	}
	else if(wType == 4)
	{
		wWaveSpeed = 6200;
	}
	else if(wType == 5)
	{
		wWaveSpeed = 3327;
	}
	else if(wType == 6)
	{
		wWaveSpeed = 4216;
	}
	else if(wType == 7)
	{
		wWaveSpeed = 2388;
	}
	else if(wType == 8)
	{
		wWaveSpeed = 2540;
	}
	else if(wType == 9)
	{
		wWaveSpeed = 3607;
	}
	else if(wType == 10)
	{
		wWaveSpeed = 3251;
	}
	else if(wType == 11)
	{
		wWaveSpeed = 2550;
	}
	else if(wType == 12)
	{
		wWaveSpeed = 5639;
	}
	else if(wType == 13)
	{
		wWaveSpeed = 1510;
	}
	else if(wType == 0)
	{
		wWaveSpeed = wUserDefineWaveSpeed;
	}

	return wWaveSpeed;
}

/**************************
*注册测厚测量内容窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChMeasureContentChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chMeasureContentChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChMeasureContentChild;

	return RegisterClass(&wc);
}
/**************************
*测厚测量内容窗口处理过程
* @param    	hWnd[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chMeasureContentChildProc(HWND hWnd, UINT message, 
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
			nWaveSpeedStep = WAVESPEEDSTEPS[nWaveSpeedStepIndex];
			fBiaoChengZhiStep = AMENDSTEPS[wBiaoChengZhiStepIndex];
			if(gstMeasureDisplay.wMethod == 3)
				buttonNum = 6;
			else
				buttonNum = 4;
			//画按钮控件
			hBtnChMeasureContentDisplay = CreateWindow(_T("BUTTON"),_T("displayChMeasureContent"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMEASURECONTENT_DISPLAY,hInst,NULL);
			hBtnChMeasureContentOnOff = CreateWindow(_T("BUTTON"),_T("OnOffChMeasureContent"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMEASURECONTENT_ONOFF,hInst,NULL);
			hBtnChMeasureContentStandard = CreateWindow(_T("BUTTON"),_T("standardChMeasureContent"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMEASURECONTENT_STANDARD,hInst,NULL);
			hBtnChMeasureContentMode = CreateWindow(_T("BUTTON"),_T("modeChMeasureContent"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMEASURECONTENT_MODE,hInst,NULL);
			if(gstMeasureDisplay.wMethod == 3)
			{
				hBtnChMeasureContentTuCengMaterial = CreateWindow(_T("BUTTON"),_T("tuCengMaterialChMeasureContent"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMEASURECONTENT_TUCENGMATERIAL,hInst,NULL);
				hBtnChMeasureContentTuCengSpeed = CreateWindow(_T("BUTTON"),_T("tuCengSpeedChMeasureContent"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 5*BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMEASURECONTENT_TUCENGSPEED,hInst,NULL);
			}
			chMeasureContentBtnList[0] = hBtnChMeasureContentDisplay;
			chMeasureContentBtnList[1] = hBtnChMeasureContentOnOff;
			chMeasureContentBtnList[2] = hBtnChMeasureContentStandard;
			chMeasureContentBtnList[3] = hBtnChMeasureContentMode;
			if(gstMeasureDisplay.wMethod == 3)
			{
				chMeasureContentBtnList[4] = hBtnChMeasureContentTuCengMaterial;
				chMeasureContentBtnList[5] = hBtnChMeasureContentTuCengSpeed;
			}
			
			//按钮控件的自定义处理过程
			OldBtnChMeasureContentDisplayProc = (WNDPROC) GetWindowLong(hBtnChMeasureContentDisplay,GWL_WNDPROC);
			SetWindowLong(hBtnChMeasureContentDisplay,GWL_WNDPROC,(LONG)btnChMeasureContentDisplayProc);
			
			OldBtnChMeasureContentOnOffProc = (WNDPROC) GetWindowLong(hBtnChMeasureContentOnOff,GWL_WNDPROC);
			SetWindowLong(hBtnChMeasureContentOnOff,GWL_WNDPROC,(LONG)btnChMeasureContentOnOffProc);
			
			OldBtnChMeasureContentStandardProc = (WNDPROC) GetWindowLong(hBtnChMeasureContentStandard,GWL_WNDPROC);
			SetWindowLong(hBtnChMeasureContentStandard,GWL_WNDPROC,(LONG)btnChMeasureContentStandardProc);
			
			OldBtnChMeasureContentModeProc = (WNDPROC) GetWindowLong(hBtnChMeasureContentMode,GWL_WNDPROC);
			SetWindowLong(hBtnChMeasureContentMode,GWL_WNDPROC,(LONG)btnChMeasureContentModeProc);
			
			if(gstMeasureDisplay.wMethod == 3)
			{
				OldBtnChMeasureContentTuCengMaterialProc = (WNDPROC) GetWindowLong(hBtnChMeasureContentTuCengMaterial,GWL_WNDPROC);
				SetWindowLong(hBtnChMeasureContentTuCengMaterial,GWL_WNDPROC,(LONG)btnChMeasureContentTuCengMaterialProc);
				
				OldBtnChMeasureContentTuCengSpeedProc = (WNDPROC) GetWindowLong(hBtnChMeasureContentTuCengSpeed,GWL_WNDPROC);
				SetWindowLong(hBtnChMeasureContentTuCengSpeed,GWL_WNDPROC,(LONG)btnChMeasureContentTuCengSpeedProc);
			}
		
			currentChMeasureContentBtn = chMeasureContentBtnList[0];//默认当前选中报警号按钮
			SetFocus(currentChMeasureContentBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentChMeasureContentBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndChMeasureCfg,SW_SHOW);
				SetFocus(currentChMeasureCfgBtn);//上级菜单最后选中的按钮获得输入焦点

				DestroyWindow(hWndChMeasureContent);
				hWndChMeasureContent = NULL;

				gCurrentFocusBtn = currentChMeasureCfgBtn;

				gCurrentWnd = hWndChMeasureCfg;
			}
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			chMeasureContent_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnChMeasureContentMode)
			{
				displayChaZhiMode(pdis->hwndItem, gstMeasureDisplay.wChaZhiMode);
			}
			else if(pdis->hwndItem == hBtnChMeasureContentStandard)
			{
				displayBiaoChengZhi(pdis->hwndItem, gstMeasureDisplay.fBiaoChengZhi);
			}
			else if(pdis->hwndItem == hBtnChMeasureContentDisplay)
			{
				displayMeasureDisplay(pdis->hwndItem, gwMeasureDisplayIndex);
			}
			else if(pdis->hwndItem == hBtnChMeasureContentOnOff)
			{
				displayMeasureDisplaySwitch(pdis->hwndItem, gwMeasureDisplayIndex);
			}
			else if(pdis->hwndItem == hBtnChMeasureContentTuCengMaterial)
			{
				gwTuCengType = tuCengSpeedToType(gwTuCengSpeed);
				displayTuCengMaterial(pdis->hwndItem, gwTuCengType);
			}
			else if(pdis->hwndItem == hBtnChMeasureContentTuCengSpeed)
			{
				gwTuCengSpeed = tuCengTypeToWaveSpeed(gwTuCengType);
				displayTuCengSpeed(pdis->hwndItem, gwTuCengSpeed);
			}
			else
			{
				;//
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
*加载测厚测量内容窗口按钮图片
* @param    	hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chMeasureContent_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chMeasureContentBtnBitmap[]=
	{
		{IDC_CHMEASURECONTENT_DISPLAY	,IDB_BITMAP_CELIANGXIANSHI_NOSEL_R	 ,  IDB_BITMAP_CELIANGXIANSHI_SELECT_R		},
		{IDC_CHMEASURECONTENT_ONOFF	,IDB_BITMAP_KAIQIGUANBI6_NOSEL_R	 ,  IDB_BITMAP_KAIQIGUANBI6_SELECT_R		},
		{IDC_CHMEASURECONTENT_STANDARD	,IDB_BITMAP_BIAOCHENGZHI_NOSEL_R ,  IDB_BITMAP_BIAOCHENGZHI_SELECT_R	},
		{IDC_CHMEASURECONTENT_MODE	,IDB_BITMAP_CHAZHIMOSHI_NOSEL_R ,  IDB_BITMAP_CHAZHIMOSHI_SELECT_R	},
		{IDC_CHMEASURECONTENT_TUCENGMATERIAL,IDB_BITMAP_TUCENGCAILIAO_NOSEL_R ,  IDB_BITMAP_TUCENGCAILIAO_SELECT_R	},
		{IDC_CHMEASURECONTENT_TUCENGSPEED,IDB_BITMAP_TUCENGSHENGSU_NOSEL_R ,  IDB_BITMAP_TUCENGSHENGSU_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	}; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chMeasureContentBtnBitmap);
	return ReturnVal;
}
/**************************
*测量显示按钮自定义处理过程
* @param    	Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChMeasureContentDisplayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(chMeasureContentBtnList[i] != currentChMeasureContentBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMeasureContentBtn = chMeasureContentBtnList[i];
					SetFocus(currentChMeasureContentBtn);
					gCurrentFocusBtn = currentChMeasureContentBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMeasureContentBtnList[i] != currentChMeasureContentBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMeasureContentBtn = chMeasureContentBtnList[i];
					SetFocus(currentChMeasureContentBtn);
					gCurrentFocusBtn = currentChMeasureContentBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(gwMeasureDisplayIndex > 0)
						gwMeasureDisplayIndex--;
					else
						gwMeasureDisplayIndex = 4;

					displayMeasureDisplay(Wnd, gwMeasureDisplayIndex);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gwMeasureDisplayIndex < 4)
						gwMeasureDisplayIndex++;
					else
						gwMeasureDisplayIndex = 0;

					displayMeasureDisplay(Wnd, gwMeasureDisplayIndex);
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
	return CallWindowProc(OldBtnChMeasureContentDisplayProc,Wnd,Msg,wParam,lParam);
}
/**************************
*开关按钮自定义处理过程
* @param    	Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChMeasureContentOnOffProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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
					while(chMeasureContentBtnList[i] != currentChMeasureContentBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMeasureContentBtn = chMeasureContentBtnList[i];
					SetFocus(currentChMeasureContentBtn);
					gCurrentFocusBtn = currentChMeasureContentBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMeasureContentBtnList[i] != currentChMeasureContentBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMeasureContentBtn = chMeasureContentBtnList[i];
					SetFocus(currentChMeasureContentBtn);
					gCurrentFocusBtn = currentChMeasureContentBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(gwMeasureDisplayIndex == 0)
						gstMeasureDisplay.stMax.bSwitch = !gstMeasureDisplay.stMax.bSwitch;
					else if(gwMeasureDisplayIndex == 1)
						gstMeasureDisplay.stMin.bSwitch = !gstMeasureDisplay.stMin.bSwitch;
					else if(gwMeasureDisplayIndex == 2)
						gstMeasureDisplay.stAverage.bSwitch = !gstMeasureDisplay.stAverage.bSwitch;
					else if(gwMeasureDisplayIndex == 3)
						gstMeasureDisplay.stCurrent.bSwitch = !gstMeasureDisplay.stCurrent.bSwitch;
					else if(gwMeasureDisplayIndex == 4)
						gstMeasureDisplay.stChaZhi.bSwitch = !gstMeasureDisplay.stChaZhi.bSwitch;
						
					displayMeasureDisplaySwitch(Wnd, gwMeasureDisplayIndex);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gwMeasureDisplayIndex == 0)
						gstMeasureDisplay.stMax.bSwitch = !gstMeasureDisplay.stMax.bSwitch;
					else if(gwMeasureDisplayIndex == 1)
						gstMeasureDisplay.stMin.bSwitch = !gstMeasureDisplay.stMin.bSwitch;
					else if(gwMeasureDisplayIndex == 2)
						gstMeasureDisplay.stAverage.bSwitch = !gstMeasureDisplay.stAverage.bSwitch;
					else if(gwMeasureDisplayIndex == 3)
						gstMeasureDisplay.stCurrent.bSwitch = !gstMeasureDisplay.stCurrent.bSwitch;
					else if(gwMeasureDisplayIndex == 4)
						gstMeasureDisplay.stChaZhi.bSwitch = !gstMeasureDisplay.stChaZhi.bSwitch;
						
					displayMeasureDisplaySwitch(Wnd, gwMeasureDisplayIndex);
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
	return CallWindowProc(OldBtnChMeasureContentOnOffProc,Wnd,Msg,wParam,lParam);
}
/**************************
*标称值按钮自定义处理过程
* @param    	Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChMeasureContentStandardProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), fBiaoChengZhiStep);
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
					while(chMeasureContentBtnList[i] != currentChMeasureContentBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMeasureContentBtn = chMeasureContentBtnList[i];
					SetFocus(currentChMeasureContentBtn);
					gCurrentFocusBtn = currentChMeasureContentBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMeasureContentBtnList[i] != currentChMeasureContentBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMeasureContentBtn = chMeasureContentBtnList[i];
					SetFocus(currentChMeasureContentBtn);
					gCurrentFocusBtn = currentChMeasureContentBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if((gstMeasureDisplay.fBiaoChengZhi - fBiaoChengZhiStep) >= MIN_BIAOCHENGZHI)
						gstMeasureDisplay.fBiaoChengZhi -= fBiaoChengZhiStep;
					else
						gstMeasureDisplay.fBiaoChengZhi = MIN_BIAOCHENGZHI;

					displayBiaoChengZhi(Wnd, gstMeasureDisplay.fBiaoChengZhi);					
				}
				else if(wParam == VK_RIGHT)
				{
					gstMeasureDisplay.fBiaoChengZhi += fBiaoChengZhiStep;

					displayBiaoChengZhi(Wnd, gstMeasureDisplay.fBiaoChengZhi);
				}
				else if(wParam == 'F')
				{
					if(wBiaoChengZhiStepIndex < 3)
						wBiaoChengZhiStepIndex++;
					else
						wBiaoChengZhiStepIndex = 0;

					fBiaoChengZhiStep = AMENDSTEPS[wBiaoChengZhiStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fBiaoChengZhiStep);
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
	return CallWindowProc(OldBtnChMeasureContentStandardProc,Wnd,Msg,wParam,lParam);
}
/**************************
*差值模式按钮自定义处理过程
* @param    	Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChMeasureContentModeProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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
					while(chMeasureContentBtnList[i] != currentChMeasureContentBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMeasureContentBtn = chMeasureContentBtnList[i];
					SetFocus(currentChMeasureContentBtn);
					gCurrentFocusBtn = currentChMeasureContentBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMeasureContentBtnList[i] != currentChMeasureContentBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMeasureContentBtn = chMeasureContentBtnList[i];
					SetFocus(currentChMeasureContentBtn);
					gCurrentFocusBtn = currentChMeasureContentBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					 if(gstMeasureDisplay.wChaZhiMode == 0)
					 	gstMeasureDisplay.wChaZhiMode = 1;
					 else
					 	gstMeasureDisplay.wChaZhiMode = 0;
					 displayChaZhiMode(Wnd, gstMeasureDisplay.wChaZhiMode);
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
	return CallWindowProc(OldBtnChMeasureContentModeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*涂层材料按钮自定义处理过程
* @param    	Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChMeasureContentTuCengMaterialProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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
					while(chMeasureContentBtnList[i] != currentChMeasureContentBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMeasureContentBtn = chMeasureContentBtnList[i];
					SetFocus(currentChMeasureContentBtn);
					gCurrentFocusBtn = currentChMeasureContentBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMeasureContentBtnList[i] != currentChMeasureContentBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMeasureContentBtn = chMeasureContentBtnList[i];
					SetFocus(currentChMeasureContentBtn);
					gCurrentFocusBtn = currentChMeasureContentBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gwTuCengType == 0)
						gwTuCengType = 13;
					else
						gwTuCengType--;
											
					displayTuCengMaterial(Wnd, gwTuCengType);
			
					gwTuCengSpeed = tuCengTypeToWaveSpeed(gwTuCengType);
					displayTuCengSpeed(hBtnChMeasureContentTuCengSpeed, gwTuCengSpeed);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gwTuCengType == 13)
						gwTuCengType = 0;
					else
						gwTuCengType++;
											
					displayTuCengMaterial(Wnd, gwTuCengType);
			
					gwTuCengSpeed = tuCengTypeToWaveSpeed(gwTuCengType);
					displayTuCengSpeed(hBtnChMeasureContentTuCengSpeed, gwTuCengSpeed);
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
	return CallWindowProc(OldBtnChMeasureContentTuCengMaterialProc,Wnd,Msg,wParam,lParam);
}

/**************************
*涂层声速按钮自定义处理过程
* @param    	Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChMeasureContentTuCengSpeedProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), nWaveSpeedStep);
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
					while(chMeasureContentBtnList[i] != currentChMeasureContentBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMeasureContentBtn = chMeasureContentBtnList[i];
					SetFocus(currentChMeasureContentBtn);
					gCurrentFocusBtn = currentChMeasureContentBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMeasureContentBtnList[i] != currentChMeasureContentBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMeasureContentBtn = chMeasureContentBtnList[i];
					SetFocus(currentChMeasureContentBtn);
					gCurrentFocusBtn = currentChMeasureContentBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gwTuCengSpeed - nWaveSpeedStep > MIN_WAVE_SPEED)
					{
						gwTuCengSpeed -= nWaveSpeedStep;
					}
					else
						gwTuCengSpeed = MIN_WAVE_SPEED;

					displayTuCengSpeed(Wnd, gwTuCengSpeed);

					gwTuCengType = tuCengSpeedToType(gwTuCengSpeed);
					displayTuCengMaterial(hBtnChMeasureContentTuCengMaterial, gwTuCengType);
					
					if(gwTuCengType == 0)
						wUserDefineWaveSpeed = gwTuCengSpeed;
				}
				else if(wParam == VK_RIGHT)
				{
					if(gwTuCengSpeed + nWaveSpeedStep < MAX_WAVE_SPEED)
					{
						gwTuCengSpeed += nWaveSpeedStep;
					}
					else
						gwTuCengSpeed = MAX_WAVE_SPEED;

					displayTuCengSpeed(Wnd, gwTuCengSpeed);

					gwTuCengType = tuCengSpeedToType(gwTuCengSpeed);
					displayTuCengMaterial(hBtnChMeasureContentTuCengMaterial, gwTuCengType);
					
					if(gwTuCengType == 0)
						wUserDefineWaveSpeed = gwTuCengSpeed;
				}
				else if(wParam == 'F')
				{
					if(nWaveSpeedStepIndex < 2)
						nWaveSpeedStepIndex++;
					else
						nWaveSpeedStepIndex = 0;

					nWaveSpeedStep = WAVESPEEDSTEPS[nWaveSpeedStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%d"), nWaveSpeedStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnChMeasureContentTuCengSpeedProc,Wnd,Msg,wParam,lParam);
}

