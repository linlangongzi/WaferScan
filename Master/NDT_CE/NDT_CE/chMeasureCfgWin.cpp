/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 测厚测量设置窗口源文件
 * Filename: chMeasureCfgWin.cpp
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


HWND hWndChMeasureCfg = NULL;						//测厚测量设置窗口句柄
HWND currentChMeasureCfgBtn = NULL;					//测厚测量设置窗口当前选中的按钮
WORD gwChHeadType = 0;			//探头类型 0:单晶 1:双晶
MEASURE_DISPLAY_ALL gstMeasureDisplay;			

static HWND hBtnChMeasureCfgHeadType = NULL;				//探头类型按钮句柄
static HWND hBtnChMeasureCfgMethod = NULL;			//测量方法按钮句柄
static HWND hBtnChMeasureCfgContent = NULL;			//测量内容按钮句柄
static HWND hBtnChMeasureCfgAlarmCfg = NULL;				//报警设定按钮句柄
static HWND chMeasureCfgBtnList[4] ={NULL,NULL,NULL,NULL};					//测厚测量设置窗口的按钮表
static WNDPROC OldBtnChMeasureCfgHeadTypeProc = NULL;//探头类型按钮默认处理过程
static WNDPROC OldBtnChMeasureCfgMethodProc = NULL;		//测量方法按钮默认处理过程
static WNDPROC OldBtnChMeasureCfgContentProc = NULL;		//测量内容按钮默认处理过程
static WNDPROC OldBtnChMeasureCfgAlarmCfgProc = NULL;			//报警设定按钮默认处理过程

static const int buttonNum = 4;		//按钮控件数

ATOM registerChMeasureCfgChild(HINSTANCE hInstance);
static LRESULT CALLBACK chMeasureCfgChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chMeasureCfg_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChMeasureCfgHeadTypeProc( HWND Wnd, 
													UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChMeasureCfgMethodProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChMeasureCfgContentProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChMeasureCfgAlarmCfgProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

static WORD wAalarmNum;			//报警号

extern HINSTANCE hInst;				//程序实例句柄

extern HWND hWndChAlarmCfg;			//报警设定窗口句柄
//extern HWND hWndContent;			//测量内容窗口句柄 
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndChMenu;				//测厚菜单窗口句柄
extern HWND currentChMenuBtn;		//测厚菜单窗口中当前选中的按钮
extern HWND hWndChMeasureContent;	//测厚测量内容窗口句柄

extern ARS200FPGA_SYS_REG* sys_reg;
extern CRITICAL_SECTION csSysInfo;
extern SYS_INFO gSysInfo;
extern wchar_t gszTextChannel[10];
extern wchar_t gszTextPlus[10];
extern wchar_t gszTextRefPlus[10];

extern HWND hStatusBar;
extern HWND gCurrentWnd;
extern WORD CurrentChannelNo;

extern ARS200FPGA_CH_DATA_REG *chCurrent_data_reg;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern ARS200FPGA_CH_CON_REG *chCurrent_con_reg;
extern ARS200FPGA_CH_PLUS_REG* chCurrent_plus_reg;
extern ARS200FPGA_CH_DAC_REG* chCurrent_dac_reg;

extern WORD *chCurrent_interval;
extern WORD *chCurrent_data;

extern WORD *chCurrent_gate1curve;
extern WORD *chCurrent_gate2curve;
extern WORD *chCurrent_gate3curve;
extern WORD *chCurrent_tgc;

extern WORD *ch1_interval;
extern WORD *ch1_data;
extern ARS200FPGA_CH_DATA_REG *ch1_data_reg;
extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
extern ARS200FPGA_CH_CON_REG *ch1_con_reg;

extern ARS200FPGA_CH_PLUS_REG* ch1_plus_reg;
extern ARS200FPGA_CH_DAC_REG* ch1_dac_reg;

extern WORD *ch1_gate1curve;
extern WORD *ch1_gate2curve;
extern WORD *ch1_gate3curve;
extern WORD *ch1_tgc;

extern WORD *ch2_interval;
extern WORD *ch2_data;
extern ARS200FPGA_CH_DATA_REG *ch2_data_reg;
extern ARS200FPGA_CH_GATE_REG *ch2_gate_reg;
extern ARS200FPGA_CH_CON_REG *ch2_con_reg;

extern ARS200FPGA_CH_PLUS_REG* ch2_plus_reg;
extern ARS200FPGA_CH_DAC_REG* ch2_dac_reg;

extern WORD *ch2_gate1curve;
extern WORD *ch2_gate2curve;
extern WORD *ch2_gate3curve;
extern WORD *ch2_tgc;

extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];

extern BOOL gbGuanLian;
extern wchar_t lpDefaultParamFile[MAX_PATH];

extern void Ch1Data2Ch2Data();
extern BOOL SaveParamFile(LPCTSTR lpszPathName);



/**************************
*注册测厚测量设置窗口
* @chMeasureCfg    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChMeasureCfgChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chMeasureCfgChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChMeasureCfgChild;

	return RegisterClass(&wc);
}
void chChangeHeadType(WORD wHeadType)
{
	if(wHeadType == 0) //单晶
	{
		InitializeCriticalSection(&csSysInfo);
		EnterCriticalSection(&csSysInfo);

		gSysInfo.wCurrentChNo = 0;
		sys_reg->wWorkMode = SingleChannelA;
		gSysInfo.wWorkMode = SingleChannelA;

		gSysInfo.wRelay &= 0xFFFC;
		//yachang.huang rum on 2013-11-19
		//sys_reg->wRelay = gSysInfo.wRelay;
		
		LeaveCriticalSection(&csSysInfo);
		DeleteCriticalSection(&csSysInfo);
		
		CurrentChannelNo = 0;
		chCurrent_data_reg = ch1_data_reg;
		chCurrent_gate_reg = ch1_gate_reg;
		chCurrent_con_reg = ch1_con_reg;
		chCurrent_interval = ch1_interval;
		chCurrent_data = ch1_data;

		chCurrent_plus_reg = ch1_plus_reg;
		chCurrent_dac_reg = ch1_dac_reg;

		chCurrent_gate1curve = ch1_gate1curve;
		chCurrent_gate2curve = ch1_gate2curve;
		chCurrent_gate3curve = ch1_gate3curve;

		chCurrent_tgc = ch1_tgc;
		swprintf(gszTextChannel, _T("1"));
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
		
		SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)SingleChannelA);

		memset(gszTextPlus, 0, sizeof(gszTextPlus));
		swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

		memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
		swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);

		SetRedrawFlag(TRUE);

		gbGuanLian = FALSE; 

		// 保存配置到默认文件
		SaveParamFile(lpDefaultParamFile);
	}
	else if(wHeadType == 1)	//双晶
	{
		SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)OneSendOneReceive);				

		InitializeCriticalSection(&csSysInfo);
		EnterCriticalSection(&csSysInfo);

		gSysInfo.wCurrentChNo = 1;
		sys_reg->wWorkMode = OneSendOneReceive;
		gSysInfo.wWorkMode = OneSendOneReceive;
		gSysInfo.wRelay &= 0xFFFD;
		gSysInfo.wRelay |= 0x1;
		//yachang.huang rum on 2013-11-19
		//sys_reg->wRelay = gSysInfo.wRelay;

		LeaveCriticalSection(&csSysInfo);
		DeleteCriticalSection(&csSysInfo);

		////////////					
		CurrentChannelNo = 1;
		chCurrent_data_reg = ch2_data_reg;
		chCurrent_gate_reg = ch2_gate_reg;
		chCurrent_con_reg = ch2_con_reg;
		chCurrent_interval = ch2_interval;
		chCurrent_data = ch2_data;

		chCurrent_plus_reg = ch2_plus_reg;
		chCurrent_dac_reg = ch2_dac_reg;

		chCurrent_gate1curve = ch2_gate1curve;
		chCurrent_gate2curve = ch2_gate2curve;
		chCurrent_gate3curve = ch2_gate3curve;
		
		chCurrent_tgc = ch2_tgc;
		memset(gszTextChannel, 0, sizeof(gszTextChannel));
		
		swprintf(gszTextChannel, _T("2"));

		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

		memset(gszTextPlus, 0, sizeof(gszTextPlus));
		swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

		memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
		swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
			
		//////////////

		SetRedrawFlag(TRUE);
		gbGuanLian = TRUE;

		Ch1Data2Ch2Data();

		InitParamDetail();

		// 保存配置到默认文件
		SaveParamFile(lpDefaultParamFile);
	}
}
/**************************
*显示探头类型
* @param    hWnd[in]:窗口句柄
* @param    wHeadType[in]:探头类型
* @return   ATOM :
* @since    1.00
***************************/

void displayHeadType(HWND hWnd,WORD wHeadType)
{
	if(wHeadType == 0)
	{
		DrawLableText(hWnd, _T("单晶"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else if(wHeadType == 1)
	{
		DrawLableText(hWnd, _T("双晶"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
}
/**************************
*显示测量方法
* @param    hWnd[in]:窗口句柄
* @param    wMethod[in]:测量方法
* @return   ATOM :
* @since    1.00
***************************/

void displayMeasureMethod(HWND hWnd,WORD wMethod)
{
	if(wMethod == 0)
	{
		DrawLableText(hWnd, _T("脉冲-回波"), 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else if(wMethod == 1)
	{
		DrawLableText(hWnd, _T("界面-底波"), 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else if(wMethod == 2)
	{
		DrawLableText(hWnd, _T("相邻底波"), 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else if(wMethod == 3)
	{
		DrawLableText(hWnd, _T("穿透涂层"), 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
}

/**************************
*测厚测量设置窗口处理过程
* @chMeasureCfg    hInstance[in]:
* @chMeasureCfg		message[in]:
* @chMeasureCfg		wParam[in]:
* @chMeasureCfg		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chMeasureCfgChildProc(HWND hWnd, UINT message, 
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
			//画按钮控件
			hBtnChMeasureCfgHeadType = CreateWindow(_T("BUTTON"),_T("headTypeChMeasureCfg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMEASURECFG_HEADTYPE,hInst,NULL);
			hBtnChMeasureCfgMethod = CreateWindow(_T("BUTTON"),_T("methodChMeasureCfg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMEASURECFG_METHOD,hInst,NULL);
			hBtnChMeasureCfgContent = CreateWindow(_T("BUTTON"),_T("contentChMeasureCfg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMEASURECFG_CONTENT,hInst,NULL);
			hBtnChMeasureCfgAlarmCfg = CreateWindow(_T("BUTTON"),_T("alarmCfgChMeasureCfg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 3*BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHMEASURECFG_ALARMCFG,hInst,NULL);
					
			chMeasureCfgBtnList[0] = hBtnChMeasureCfgHeadType;
			chMeasureCfgBtnList[1] = hBtnChMeasureCfgMethod;
			chMeasureCfgBtnList[2] = hBtnChMeasureCfgContent;
			chMeasureCfgBtnList[3] = hBtnChMeasureCfgAlarmCfg;
			
			//按钮控件的自定义处理过程
			OldBtnChMeasureCfgHeadTypeProc = (WNDPROC) GetWindowLong(hBtnChMeasureCfgHeadType,GWL_WNDPROC);
			SetWindowLong(hBtnChMeasureCfgHeadType,GWL_WNDPROC,(LONG)btnChMeasureCfgHeadTypeProc);
			
			OldBtnChMeasureCfgMethodProc = (WNDPROC) GetWindowLong(hBtnChMeasureCfgMethod,GWL_WNDPROC);
			SetWindowLong(hBtnChMeasureCfgMethod,GWL_WNDPROC,(LONG)btnChMeasureCfgMethodProc);
			
			OldBtnChMeasureCfgContentProc = (WNDPROC) GetWindowLong(hBtnChMeasureCfgContent,GWL_WNDPROC);
			SetWindowLong(hBtnChMeasureCfgContent,GWL_WNDPROC,(LONG)btnChMeasureCfgContentProc);
			
			OldBtnChMeasureCfgAlarmCfgProc = (WNDPROC) GetWindowLong(hBtnChMeasureCfgAlarmCfg,GWL_WNDPROC);
			SetWindowLong(hBtnChMeasureCfgAlarmCfg,GWL_WNDPROC,(LONG)btnChMeasureCfgAlarmCfgProc);
		
			currentChMeasureCfgBtn = chMeasureCfgBtnList[0];//默认当前选中探头类型按钮
			SetFocus(currentChMeasureCfgBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentChMeasureCfgBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
		/*		if(LOWORD(wParam) == IDC_CHMEASURECFG_CONTENT)
				{
					currentChMeasureCfgBtn = chMeasureCfgBtnList[2]; 
					SetFocus(currentChMeasureCfgBtn);
					gCurrentFocusBtn = currentChMeasureCfgBtn;
					hWndContent = CreateWindow(szContentChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMeasureCfg,SW_HIDE);
					//SetFocus(hWndContent);
					ShowWindow(hWndContent,SW_SHOW);
					UpdateWindow(hWndContent);
				}
				else if(LOWORD(wParam) == IDC_CHMEASURECFG_ALARMCFG)
				{
				//	currentChMeasureCfgBtn = chMeasureCfgBtnList[3]; 
				//	SetFocus(currentChMeasureCfgBtn);
				//	gCurrentFocusBtn = currentChMeasureCfgBtn;
				}
			*/
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			chMeasureCfg_DrawButtonBitmap(hInst,pdis);
			if(pdis->hwndItem == hBtnChMeasureCfgHeadType)
			{
				 if(sys_reg->wWorkMode == SingleChannelA)
					gwChHeadType = 0;	//单晶
				 else if(sys_reg->wWorkMode == OneSendOneReceive)
				 	gwChHeadType = 1;	//双晶
				 	
				displayHeadType(pdis->hwndItem, gwChHeadType);
			}
			if(pdis->hwndItem == hBtnChMeasureCfgMethod)
			{
				displayMeasureMethod(pdis->hwndItem, gstMeasureDisplay.wMethod);
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
*加载测厚测量设置窗口按钮图片
* @chMeasureCfg    hInstance[in]:
* @chMeasureCfg		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chMeasureCfg_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chMeasureCfgBtnBitmap[]=
	{
		{IDC_CHMEASURECFG_HEADTYPE	,IDB_BITMAP_TANTOULEIXING_NOSEL_R	 ,  IDB_BITMAP_TANTOULEIXING_SELECT_R		},
		{IDC_CHMEASURECFG_METHOD	,IDB_BITMAP_CELIANGFANGFA_NOSEL_R	 ,  IDB_BITMAP_CELIANGFANGFA_SELECT_R		},
		{IDC_CHMEASURECFG_CONTENT	,IDB_BITMAP_CELIANGNEIRONG_NOSEL_R ,  IDB_BITMAP_CELIANGNEIRONG_SELECT_R	},
		{IDC_CHMEASURECFG_ALARMCFG	,IDB_BITMAP_BAOJIANGSHEDING_NOSEL_R ,  IDB_BITMAP_BAOJIANGSHEDING_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chMeasureCfgBtnBitmap);
	return ReturnVal;
}
/**************************
*探头类型按钮自定义处理过程
* @chMeasureCfg    hInstance[in]:
* @chMeasureCfg		message[in]:
* @chMeasureCfg		wParam[in]:
* @chMeasureCfg		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChMeasureCfgHeadTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChMenu,SW_SHOW);
					SetFocus(currentChMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChMeasureCfg);
					hWndChMeasureCfg = 0;
					
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd = hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(chMeasureCfgBtnList[i] != currentChMeasureCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMeasureCfgBtn = chMeasureCfgBtnList[i];
					SetFocus(currentChMeasureCfgBtn);
					gCurrentFocusBtn = currentChMeasureCfgBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMeasureCfgBtnList[i] != currentChMeasureCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMeasureCfgBtn = chMeasureCfgBtnList[i];
					SetFocus(currentChMeasureCfgBtn);
					gCurrentFocusBtn = currentChMeasureCfgBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{					
					if(sys_reg->wWorkMode == OneSendOneReceive)
					{
						gwChHeadType = 0;	//单晶
						chChangeHeadType(gwChHeadType);
						displayHeadType(Wnd, gwChHeadType);
					}
					else if(sys_reg->wWorkMode == SingleChannelA)
					{
						gwChHeadType = 1;	//双晶
						chChangeHeadType(gwChHeadType);
						displayHeadType(Wnd, gwChHeadType);
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
	return CallWindowProc(OldBtnChMeasureCfgHeadTypeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*测量方法按钮自定义处理过程
* @chMeasureCfg    hInstance[in]:
* @chMeasureCfg		message[in]:
* @chMeasureCfg		wParam[in]:
* @chMeasureCfg		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChMeasureCfgMethodProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChMenu,SW_SHOW);
					SetFocus(currentChMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChMeasureCfg);
					hWndChMeasureCfg = 0;
					
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd = hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(chMeasureCfgBtnList[i] != currentChMeasureCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMeasureCfgBtn = chMeasureCfgBtnList[i];
					SetFocus(currentChMeasureCfgBtn);
					gCurrentFocusBtn = currentChMeasureCfgBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMeasureCfgBtnList[i] != currentChMeasureCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMeasureCfgBtn = chMeasureCfgBtnList[i];
					SetFocus(currentChMeasureCfgBtn);
					gCurrentFocusBtn = currentChMeasureCfgBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(gstMeasureDisplay.wMethod == 0)
						gstMeasureDisplay.wMethod = 3;
					else
						gstMeasureDisplay.wMethod--;

					displayMeasureMethod(Wnd,gstMeasureDisplay.wMethod);

					if(gstMeasureDisplay.wMethod == 1)
					{
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(gstMeasureDisplay.wMethod == 3)
						gstMeasureDisplay.wMethod = 0;
					else
						gstMeasureDisplay.wMethod++;

					displayMeasureMethod(Wnd,gstMeasureDisplay.wMethod);

					if(gstMeasureDisplay.wMethod == 1)
					{
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
	return CallWindowProc(OldBtnChMeasureCfgMethodProc,Wnd,Msg,wParam,lParam);
}
/**************************
*测量内容按钮自定义处理过程
* @chMeasureCfg    hInstance[in]:
* @chMeasureCfg		message[in]:
* @chMeasureCfg		wParam[in]:
* @chMeasureCfg		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChMeasureCfgContentProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChMenu,SW_SHOW);
					SetFocus(currentChMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChMeasureCfg);
					hWndChMeasureCfg = 0;
					
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd = hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(chMeasureCfgBtnList[i] != currentChMeasureCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMeasureCfgBtn = chMeasureCfgBtnList[i];
					SetFocus(currentChMeasureCfgBtn);
					gCurrentFocusBtn = currentChMeasureCfgBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMeasureCfgBtnList[i] != currentChMeasureCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMeasureCfgBtn = chMeasureCfgBtnList[i];
					SetFocus(currentChMeasureCfgBtn);
					gCurrentFocusBtn = currentChMeasureCfgBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChMeasureContent = CreateWindow(szChMeasureContentChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMeasureCfg,SW_HIDE);
					//SetFocus(hWndChMeasureContent);
					ShowWindow(hWndChMeasureContent,SW_SHOW);
					UpdateWindow(hWndChMeasureContent);

					gCurrentWnd = hWndChMeasureContent;
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
	return CallWindowProc(OldBtnChMeasureCfgContentProc,Wnd,Msg,wParam,lParam);
}
/**************************
*报警设定按钮自定义处理过程
* @chMeasureCfg    hInstance[in]:
* @chMeasureCfg		message[in]:
* @chMeasureCfg		wParam[in]:
* @chMeasureCfg		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChMeasureCfgAlarmCfgProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChMenu,SW_SHOW);
					SetFocus(currentChMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChMeasureCfg);
					hWndChMeasureCfg = 0;
					
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd = hWndChMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(chMeasureCfgBtnList[i] != currentChMeasureCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChMeasureCfgBtn = chMeasureCfgBtnList[i];
					SetFocus(currentChMeasureCfgBtn);
					gCurrentFocusBtn = currentChMeasureCfgBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chMeasureCfgBtnList[i] != currentChMeasureCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChMeasureCfgBtn = chMeasureCfgBtnList[i];
					SetFocus(currentChMeasureCfgBtn);
					gCurrentFocusBtn = currentChMeasureCfgBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChAlarmCfg= CreateWindow(szChAlarmCfgChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChMeasureCfg,SW_HIDE);
					//SetFocus(hWndChAlarmCfg);
					ShowWindow(hWndChAlarmCfg,SW_SHOW);
					UpdateWindow(hWndChAlarmCfg);

					gCurrentWnd = hWndChAlarmCfg;
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
	return CallWindowProc(OldBtnChMeasureCfgAlarmCfgProc,Wnd,Msg,wParam,lParam);
}
