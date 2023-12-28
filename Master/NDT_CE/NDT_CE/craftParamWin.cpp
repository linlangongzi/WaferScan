/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 工艺参数窗口源文件
 * Filename: craftParamWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "craftParamWin.h"
#include "interfaceCmdID.h"

#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"
#include "const.h"
#include "fnBscanWin.h"

HWND hWndCraftParam = 0;//工艺参数窗口句柄
HWND hBtnCraftParamNumber = NULL;//序号按钮句柄
HWND hBtnCraftParamReview = NULL;//预览按钮句柄
HWND hBtnCraftParamSave = NULL;//保存按钮句柄
HWND hBtnCraftParamGet = NULL;//取出按钮句柄
HWND hBtnCraftParamDelete = NULL;//删除按钮句柄
HWND craftParamBtnList[5] = {NULL,NULL,NULL,NULL,NULL};//工艺参数窗口的按钮表
WNDPROC OldBtnCraftParamNumberProc = NULL;//序号按钮默认处理过程
WNDPROC OldBtnCraftParamReviewProc = NULL;//预览按钮默认处理过程
WNDPROC OldBtnCraftParamSaveProc = NULL;//保存按钮默认处理过程
WNDPROC OldBtnCraftParamGetProc = NULL;//取出按钮默认处理过程
WNDPROC OldBtnCraftParamDeleteProc = NULL;//删除按钮默认处理过程
HWND currentCraftParamBtn = NULL;//工艺参数窗中当前选中的按钮

extern HWND hWndAllParamCfgCaption;//工艺参数配置标题窗口句柄
extern HWND hWndAllParamCfg;      //工艺参数设置窗口句柄
static HBRUSH hStaticBrush = NULL;//画刷
static const int buttonNum = 5;//按钮控件数

extern HINSTANCE hInst;//程序实例句柄
extern HWND hWndFileManage;//文件管理窗口句柄
extern HWND currentFileManageBtn;//文件管理窗口中当前选中的按钮
extern HWND gCurrentFocusBtn;

// songchenguang add start 2010-05-18
const WORD MIN_PARAM_NO = 1;
const WORD MAX_PARAM_NO = 1000;
WORD gnParamNo = MIN_PARAM_NO;		//工艺序号
DWORD currentUsedParamNo = 1;		//当前使用的工艺参数号
TCHAR ParamFileName[MAX_PATH];
extern TCHAR FileDirectory[MAX_PATH];
extern SEND_PARAM_DATA SendParam_ad[2];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern GATE_INFO GateInfos[2][3];
extern HWND hStatusBar;
extern HWND hWndMain;
extern CURVEPOINT_DATA gSelectedDacPointData[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//添加的样点数据队列
extern DWORD gSelectedDacPointDataNum[CHANNEL_NUM];					//已选中的DAC点数量
extern SYS_INFO gSysInfo;
extern CRITICAL_SECTION csSysInfo;
// songchenguang add end 2010-05-18
extern WORD CurrentChannelNo;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//当前显示的子窗口
extern float MIN_DELAY[2];
extern float MAX_DELAY;
extern wchar_t lpDefaultParamFile[MAX_PATH];

extern BOOL gbHotCraftParamFlag;	//是否打开工艺参数快捷方式 
extern  CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// 工作线程参数
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];				//DGS通道开关
extern float gfOldDacRange[CHANNEL_NUM];				//最初生成DAC时的声程
extern short gwOldDacSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//最初生成DAC时的每个点的位置
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];//已选中的DGS点数量
extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//添加的样点数据队列
extern WORD gwDeviceTypeValue;
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG曲线开关标志
extern float gfOldAvgRange[CHANNEL_NUM];				//最初生成AVG时的声程
extern short gwOldAvgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//最初生成AVG时的每个点的位置
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//添加的样点数据队列
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//已选中的AVG点数量
extern float gfTcgFirstPointGain[CHANNEL_NUM];		//添加的点第一个点对应的Db值
extern float gfDacFirstPointGain[CHANNEL_NUM];		//添加的点第一个点对应的Db值
extern float gfAvgFirstPointGain[CHANNEL_NUM];		//添加的点第一个点对应的Db值
extern float gfDefectZhi1[CHANNEL_NUM];			//所测孔径Φ1
extern float gfDefectZhi2[CHANNEL_NUM];			//Φ2
extern float gfDefectZhi3[CHANNEL_NUM];			//Φ3
extern float gfPingDingGain[CHANNEL_NUM];			//评定增益
extern float gfDingLiangGain[CHANNEL_NUM];			//定量增益
extern float gfPanHuiGain[CHANNEL_NUM];				//判废增益

// songchenguang add start 2010-12-27
extern WORD wTanTouLeiXing[CHANNEL_NUM];//探头类型
extern CRITICAL_SECTION csProbeType;
// songchenguang add end 2010-12-27

extern float gfOldTcgRange[CHANNEL_NUM];				//最初生成TCG时的声程
extern short gwOldTcgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//最初生成TCG时的每个点的位置
//铁路
extern WORD gwStandard[CHANNEL_NUM];	//检测标准
extern WORD gwPosition[CHANNEL_NUM][TL_STANDARD_NUM];	//检测部位
extern float gfTlGainBuChang[CHANNEL_NUM];//增益补偿
extern BOOL gbTlSheDingFlag[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//各标准各部位是否已设定标志
extern float gfTlRange[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];				//各标准各部位声程
extern float gfTlPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];				//各标准各部位增益
extern float gfTlBuChangPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//各标准各部位补偿增益
// songchenguang add end 2011-06-22
extern WORD MAX_PLUS;
// songchenguang add end 2011-06-22

extern CSCAN_ARS200FPGA_CH_GATE_REG *cscan_ch1_gate_reg;
extern CSCAN_ARS200FPGA_CH_GATE_REG *cscan_ch2_gate_reg;
extern CSCAN_ARS200FPGA_CODE_REG* cscan_code_reg;

/**************************
*注册工艺参数窗口
* @craftParam    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerCraftParamChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));

	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) craftParamChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szCraftParamChild;

	return RegisterClass(&wc);
}

void SetParamFileName(HWND hWnd, const WORD& nParamNo)
{
	wchar_t strFileName[10];
	memset(strFileName, 0, sizeof(strFileName));

	wsprintf(strFileName, _T("%d.par"), nParamNo);

	memcpy(ParamFileName, FileDirectory, sizeof(ParamFileName));
	StringCchCat(ParamFileName, MAX_PATH, strFileName);

	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	if(GetFileAttributes(ParamFileName)  == 0xFFFFFFFF)
		wsprintf(strText, _T("*%d"), nParamNo);
	else
		wsprintf(strText, _T("%d"), nParamNo);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}

/**************************
*工艺参数窗口处理过程
* @craftParam    hInstance[in]:
* @craftParam		message[in]:
* @craftParam		wParam [in]:
* @craftParam		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK craftParamChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			if(hStaticBrush == NULL)	
				hStaticBrush = CreateSolidBrush(BK_COLOR_WHITE);

			// songchenguang add start 2010-05-18
			memset(ParamFileName, 0, sizeof(ParamFileName));
				
			SetParamFileName(hWnd, gnParamNo);
			// songchenguang add end 2010-05-18
			//画按钮控件
			hBtnCraftParamNumber = CreateWindow(_T("BUTTON"),_T("craftParamNumber"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CRAFTPARAM_NUMBER,hInst,NULL);
			hBtnCraftParamReview = CreateWindow(_T("BUTTON"),_T("reviewNumber"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CRAFTPARAM_REVIEW,hInst,NULL);
			hBtnCraftParamSave = CreateWindow(_T("BUTTON"),_T("craftParamSave"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CRAFTPARAM_SAVE,hInst,NULL);
			hBtnCraftParamGet = CreateWindow(_T("BUTTON"),_T("craftParamGet"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CRAFTPARAM_GET,hInst,NULL);
			hBtnCraftParamDelete = CreateWindow(_T("BUTTON"),_T("craftParamDelete"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CRAFTPARAM_DELETE,hInst,NULL);
					
			craftParamBtnList[0] = hBtnCraftParamNumber;
			craftParamBtnList[1] = hBtnCraftParamReview;
			craftParamBtnList[2] = hBtnCraftParamSave;
			craftParamBtnList[3] = hBtnCraftParamGet;
			craftParamBtnList[4] = hBtnCraftParamDelete;
			//按钮控件的自定义处理过程
			OldBtnCraftParamNumberProc = (WNDPROC) GetWindowLong(hBtnCraftParamNumber,GWL_WNDPROC);
			SetWindowLong(hBtnCraftParamNumber,GWL_WNDPROC,(LONG)btnCraftParamNumberProc);
			OldBtnCraftParamReviewProc = (WNDPROC) GetWindowLong(hBtnCraftParamReview,GWL_WNDPROC);
			SetWindowLong(hBtnCraftParamReview,GWL_WNDPROC,(LONG)btnCraftParamReviewProc);
			OldBtnCraftParamSaveProc = (WNDPROC) GetWindowLong(hBtnCraftParamSave,GWL_WNDPROC);
			SetWindowLong(hBtnCraftParamSave,GWL_WNDPROC,(LONG)btnCraftParamSaveProc);
			OldBtnCraftParamGetProc = (WNDPROC) GetWindowLong(hBtnCraftParamGet,GWL_WNDPROC);
			SetWindowLong(hBtnCraftParamGet,GWL_WNDPROC,(LONG)btnCraftParamGetProc);
			OldBtnCraftParamDeleteProc = (WNDPROC) GetWindowLong(hBtnCraftParamDelete,GWL_WNDPROC);
			SetWindowLong(hBtnCraftParamDelete,GWL_WNDPROC,(LONG)btnCraftParamDeleteProc);

			currentCraftParamBtn = craftParamBtnList[0];//默认当前选中序号按钮
			SetFocus(currentCraftParamBtn);//选中按钮获得输入焦点

			if(!gbHotCraftParamFlag)
				gCurrentFocusBtn = currentCraftParamBtn;
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
			craftParam_DrawButtonBitmap(hInst,pdis);
			if(pdis->hwndItem == hBtnCraftParamNumber)
				SetParamFileName(hBtnCraftParamNumber, gnParamNo);
			break;
		case WM_CLOSE:
		
			break;
		case WM_DESTROY:
			if(hStaticBrush)
			{
				DeleteObject(hStaticBrush);
				hStaticBrush = NULL;
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载工艺参数窗口按钮图片
* @craftParam    hInstance[in]:
* @craftParam		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int craftParam_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_craftParamBtnBitmap[]=
	{
		{IDC_CRAFTPARAM_NUMBER	,IDB_BITMAP_XUHAO_NOSEL_R	 ,  IDB_BITMAP_XUHAO_SELECT_R		},
		{IDC_CRAFTPARAM_REVIEW	,IDB_BITMAP_YULAN_NOSEL_R	 ,  IDB_BITMAP_YULAN_SELECT_R		},
		{IDC_CRAFTPARAM_SAVE	,IDB_BITMAP_BAOCUN_NOSEL_R	 ,  IDB_BITMAP_BAOCUN_SELECT_R		},		
		{IDC_CRAFTPARAM_GET	,IDB_BITMAP_QUCHU_NOSEL_R	 ,  IDB_BITMAP_QUCHU_SELECT_R		},
		{IDC_CRAFTPARAM_DELETE	,IDB_BITMAP_SHANCHU_NOSEL_R ,  IDB_BITMAP_SHANCHU_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_craftParamBtnBitmap);
	return ReturnVal;
}


/**************************
*序号按钮自定义处理过程
* @craftParam    hInstance[in]:
* @craftParam		message[in]:
* @craftParam		wParam [in]:
* @craftParam		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCraftParamNumberProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					if(!gbHotCraftParamFlag)
					{
						ShowWindow(hWndFileManage,SW_SHOW);
						UpdateWindow(hWndFileManage);
						SetFocus(currentFileManageBtn);//上级菜单最后选中的按钮获得输入焦点

						DestroyWindow(hWndCraftParam);
						hWndCraftParam = 0;					
						
						gCurrentFocusBtn = currentFileManageBtn;

						gCurrentWnd = hWndFileManage;
					}
					else
					{
						if(gCurrentWnd)//有子菜单窗口
						{
							ShowWindow(gCurrentWnd, SW_SHOW);
							SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
						}

						DestroyWindow(hWndCraftParam);
						hWndCraftParam = 0;
						if(!gCurrentWnd)//没有子菜单窗口
						{
							SetMenuStateFlag(FALSE);
							SetFocus(hWndMain);//主窗口获得输入焦点
						}
						
						gbHotCraftParamFlag = FALSE;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(craftParamBtnList[i] != currentCraftParamBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCraftParamBtn = craftParamBtnList[i];
					SetFocus(currentCraftParamBtn);
					if(!gbHotCraftParamFlag)//没有弹出所有参数配置窗口
						gCurrentFocusBtn = currentCraftParamBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(craftParamBtnList[i] != currentCraftParamBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCraftParamBtn = craftParamBtnList[i];
					SetFocus(currentCraftParamBtn);
					if(!gbHotCraftParamFlag)//没有弹出所有参数配置窗口
						gCurrentFocusBtn = currentCraftParamBtn;
				}
				else if(wParam == VK_RETURN)
				{
					cscan_code_reg->wReset = 1;
					Sleep(1);
					cscan_code_reg->wReset = 0;
				}
				else if(wParam == VK_LEFT)
				{
					// songchenguang add start 2010-05-18
					if(gnParamNo > MIN_PARAM_NO)
						gnParamNo--;
					else
					{
					    if((gwDeviceTypeValue == 0x2010) || (gwDeviceTypeValue == 0x2020))
							gnParamNo = 100;
						else
							gnParamNo = MAX_PARAM_NO;
					}

					SetParamFileName(Wnd, gnParamNo);
					// songchenguang add end 2010-05-18
					if(gnParamNo <= 5)
					{
						cscan_ch1_gate_reg->wSimpleInterval = gnParamNo - 1;
					}
				}
				else if(wParam == VK_RIGHT)
				{
					// songchenguang add start 2010-05-18
					if((gwDeviceTypeValue == 0x2010) || (gwDeviceTypeValue == 0x2020))
					{
						if(gnParamNo < 100)
							gnParamNo++;
						else
							gnParamNo = MIN_PARAM_NO;
					}
					else
					{
						if(gnParamNo < MAX_PARAM_NO)
							gnParamNo++;
						else
							gnParamNo = MIN_PARAM_NO;
					}

					SetParamFileName(Wnd, gnParamNo);

					if(gnParamNo <= 5)
					{
						cscan_ch1_gate_reg->wSimpleInterval = gnParamNo - 1;
					}
					// songchenguang add end 2010-05-18
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					if(!gbHotCraftParamFlag)
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
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnCraftParamNumberProc,Wnd,Msg,wParam,lParam);
}

/**************************
*序号按钮自定义处理过程
* @craftParam    hInstance[in]:
* @craftParam		message[in]:
* @craftParam		wParam [in]:
* @craftParam		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCraftParamReviewProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					if(!gbHotCraftParamFlag)
					{
						ShowWindow(hWndFileManage,SW_SHOW);
						UpdateWindow(hWndFileManage);
						SetFocus(currentFileManageBtn);//上级菜单最后选中的按钮获得输入焦点

						DestroyWindow(hWndCraftParam);
						hWndCraftParam = 0;					
						
						gCurrentFocusBtn = currentFileManageBtn;

						gCurrentWnd = hWndFileManage;
					}
					else
					{
						if(gCurrentWnd)//有子菜单窗口
						{
							ShowWindow(gCurrentWnd, SW_SHOW);
							SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
						}

						DestroyWindow(hWndCraftParam);
						hWndCraftParam = 0;
						if(!gCurrentWnd)//没有子菜单窗口
						{
							SetMenuStateFlag(FALSE);
							SetFocus(hWndMain);//主窗口获得输入焦点
						}
						
						gbHotCraftParamFlag = FALSE;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(craftParamBtnList[i] != currentCraftParamBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCraftParamBtn = craftParamBtnList[i];
					SetFocus(currentCraftParamBtn);
					if(!gbHotCraftParamFlag)//没有弹出所有参数配置窗口
						gCurrentFocusBtn = currentCraftParamBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(craftParamBtnList[i] != currentCraftParamBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCraftParamBtn = craftParamBtnList[i];
					SetFocus(currentCraftParamBtn);
					if(!gbHotCraftParamFlag)//没有弹出所有参数配置窗口
						gCurrentFocusBtn = currentCraftParamBtn;
				}
				else if(wParam == VK_RETURN)
				{
					if(ReadParamFile(ParamFileName))
					{
						InitializeCriticalSection(&csCommand);
						EnterCriticalSection(&csCommand);
						//gbEndFlag =TRUE;
						thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;
						LeaveCriticalSection(&csCommand);
						DeleteCriticalSection(&csCommand);
						Sleep(20); //等待线程完全停止
						setBScanSwitch(NULL, FALSE);//关闭BSCAN
						
						hWndAllParamCfgCaption = CreateWindow(szAllParamCfgCaptionChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								 0, 0, PARAM_WIDTH, 30, hWndMain, NULL, hInst, NULL);
						hWndAllParamCfg = CreateWindow(szAllParamCfgChild, TEXT(""), WS_CHILD|WS_VISIBLE|WS_VSCROLL,
								 PARAM_STRART_X, PARAM_STRART_Y, PARAM_WIDTH, PARAM_HEIGHT - 30, hWndMain, NULL, hInst, NULL);
						ShowWindow(hWndCraftParam,SW_HIDE);
						ShowWindow(hStatusBar,SW_HIDE);
						//SetFocus(hWndAllParamConfig);
						ShowWindow(hWndAllParamCfgCaption,SW_SHOW);
						UpdateWindow(hWndAllParamCfgCaption);
						ShowWindow(hWndAllParamCfg,SW_SHOW);
						UpdateWindow(hWndAllParamCfg);
					}
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					if(!gbHotCraftParamFlag)
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
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnCraftParamReviewProc,Wnd,Msg,wParam,lParam);
}

BOOL SaveParamFile(LPCTSTR lpszPathName)
{
	CRAFT_PARAM ch1Param;
	CRAFT_PARAM ch2Param;

	ch1Param.SendParam = SendParam_ad[0];
	ch1Param.ReceiveParam = ReceiveParam_ad[0];
	ch1Param.DisplayParam = DisplaySet_ad[0];
//	memcpy(ch1Param.GateInfos, GateInfos[0], sizeof(GATE_INFO)*3);
	for(int i = 0; i < GATE_NUM; i++)
	{
		ch1Param.GateInfos[i] = GateInfos[0][i];
	}
	//TCG
	ch1Param.SelectedDacPointDataNum = gSelectedDacPointDataNum[0];
	ch1Param.fTcgFirstGain = gfTcgFirstPointGain[0];
	memcpy(ch1Param.SelectedDacPointData, gSelectedDacPointData[0], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	ch1Param.fOldTcgRang = gfOldTcgRange[0];
	for(WORD j = 0; j < gSelectedDacPointDataNum[0]; j++)
	{
		ch1Param.wOldTcgSerial[j] = gwOldTcgSerial[0][j];
	}
	//DAC
	for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
	{
		ch1Param.dgsCurve.bSwitch[j] = gDgsSwitch[0][j];
	}
	
	ch1Param.dgsCurve.fPingDing = gfPingDingGain[0];
	ch1Param.dgsCurve.fDingLiang = gfDingLiangGain[0];
	ch1Param.dgsCurve.fPanFei = gfPanHuiGain[0];
	
	ch1Param.dgsCurve.fFirstGain= gfDacFirstPointGain[0];
	ch1Param.dgsCurve.fOldRang = gfOldDacRange[0];
	for(int j = 0; j < gSelectedDgsPointDataNum[0][1]; j++)
	{
		ch1Param.dgsCurve.wOldSerial[j] = gwOldDacSerial[0][j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		ch1Param.dgsCurve.selectedDgsPointDataNum[j] = gSelectedDgsPointDataNum[0][j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		memcpy(ch1Param.dgsCurve.selectedDgsPointData[j], gSelectedDgsPointData[0][j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//AVG
	ch1Param.avgCurve.bSwitch = gbAvgCurveFlag[0];
	ch1Param.avgCurve.fFai1 = gfDefectZhi1[0];
	ch1Param.avgCurve.fFai2 = gfDefectZhi2[0];
	ch1Param.avgCurve.fFai3 = gfDefectZhi3[0];
	ch1Param.avgCurve.fFirstGain = gfAvgFirstPointGain[0];
	ch1Param.avgCurve.fOldRange = gfOldAvgRange[0];
	for(int j = 0; j < gSelectedAvgPointDataNum[0]; j++)
	{
		ch1Param.avgCurve.wOldSerial[j] = gwOldAvgSerial[0][j];
	}
	ch1Param.avgCurve.selectedPointDataNum = gSelectedAvgPointDataNum[0];
	for(int j = 0; j < AVG_CURVE_NUM + 1; j++)
	{
		memcpy(ch1Param.avgCurve.selectedPointData[j], gSelectedAvgPointData[0][j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//铁路
	ch1Param.RailRoadParam.wStandard = gwStandard[0];
	ch1Param.RailRoadParam.wPosition = gwPosition[0][gwStandard[0]];
	memcpy(ch1Param.RailRoadParam.fBuChangGain, gfTlBuChangPlus[0], sizeof(ch1Param.RailRoadParam.fBuChangGain));
	memcpy(ch1Param.RailRoadParam.fRange, gfTlRange[0], sizeof(ch1Param.RailRoadParam.fRange));
	memcpy(ch1Param.RailRoadParam.fPlus, gfTlPlus[0], sizeof(ch1Param.RailRoadParam.fPlus));
	memcpy(ch1Param.RailRoadParam.bFlag, gbTlSheDingFlag[0], sizeof(ch1Param.RailRoadParam.bFlag));
	
	ch2Param.SendParam = SendParam_ad[1];
	ch2Param.ReceiveParam = ReceiveParam_ad[1];
	ch2Param.DisplayParam = DisplaySet_ad[1];
//	memcpy(ch2Param.GateInfos, GateInfos[1], sizeof(GATE_INFO)*3);
	for(int i = 0; i < GATE_NUM; i++)
	{
		ch2Param.GateInfos[i] = GateInfos[1][i];
	}
	//TCG
	ch2Param.SelectedDacPointDataNum = gSelectedDacPointDataNum[1];
	ch2Param.fTcgFirstGain = gfTcgFirstPointGain[1];
	memcpy(ch2Param.SelectedDacPointData, gSelectedDacPointData[1], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	ch2Param.fOldTcgRang = gfOldTcgRange[1];
	for(WORD j = 0; j < gSelectedDacPointDataNum[1]; j++)
	{
		ch2Param.wOldTcgSerial[j] = gwOldTcgSerial[1][j];
	}
	//DAC
	for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
	{
		ch2Param.dgsCurve.bSwitch[j] = gDgsSwitch[1][j];
	}
	
	ch2Param.dgsCurve.fPingDing = gfPingDingGain[1];
	ch2Param.dgsCurve.fDingLiang = gfDingLiangGain[1];
	ch2Param.dgsCurve.fPanFei = gfPanHuiGain[1];
	
	ch2Param.dgsCurve.fFirstGain= gfDacFirstPointGain[1];
	ch2Param.dgsCurve.fOldRang = gfOldDacRange[1];
	for(int j = 0; j < gSelectedDgsPointDataNum[1][1]; j++)
	{
		ch2Param.dgsCurve.wOldSerial[j] = gwOldDacSerial[1][j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		ch2Param.dgsCurve.selectedDgsPointDataNum[j] = gSelectedDgsPointDataNum[1][j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		memcpy(ch2Param.dgsCurve.selectedDgsPointData[j], gSelectedDgsPointData[1][j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//AVG
	ch2Param.avgCurve.bSwitch = gbAvgCurveFlag[1];
	ch2Param.avgCurve.fFai1 = gfDefectZhi1[1];
	ch2Param.avgCurve.fFai2 = gfDefectZhi2[1];
	ch2Param.avgCurve.fFai3 = gfDefectZhi3[1];
	ch2Param.avgCurve.fFirstGain = gfAvgFirstPointGain[1];
	ch2Param.avgCurve.fOldRange = gfOldAvgRange[1];
	for(int j = 0; j < gSelectedAvgPointDataNum[1]; j++)
	{
		ch2Param.avgCurve.wOldSerial[j] = gwOldAvgSerial[1][j];
	}
	ch2Param.avgCurve.selectedPointDataNum = gSelectedAvgPointDataNum[1];
	for(int j = 0; j < AVG_CURVE_NUM + 1; j++)
	{
		memcpy(ch2Param.avgCurve.selectedPointData[j], gSelectedAvgPointData[1][j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//铁路
	ch2Param.RailRoadParam.wStandard = gwStandard[1];
	ch2Param.RailRoadParam.wPosition = gwPosition[1][gwStandard[1]];
	memcpy(ch2Param.RailRoadParam.fBuChangGain, gfTlBuChangPlus[1], sizeof(ch2Param.RailRoadParam.fBuChangGain));
	memcpy(ch2Param.RailRoadParam.fRange, gfTlRange[1], sizeof(ch2Param.RailRoadParam.fRange));
	memcpy(ch2Param.RailRoadParam.fPlus, gfTlPlus[1], sizeof(ch2Param.RailRoadParam.fPlus));
	memcpy(ch2Param.RailRoadParam.bFlag, gbTlSheDingFlag[1], sizeof(ch2Param.RailRoadParam.bFlag));
	
	HANDLE hFileWrite;	//文件句柄
	DWORD dwWriteDataSize;
	hFileWrite = CreateFile(lpszPathName,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("创建文件失败"));
		return FALSE;
	}

	InitializeCriticalSection(&csSysInfo);
	EnterCriticalSection(&csSysInfo);
	if(!WriteFile(hFileWrite,&gSysInfo, sizeof(SYS_INFO),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写文件失败"));
		
		CloseHandle(hFileWrite);
		return FALSE;
	}

	LeaveCriticalSection(&csSysInfo);
	DeleteCriticalSection(&csSysInfo);

	if(!WriteFile(hFileWrite,&ch1Param, sizeof(CRAFT_PARAM),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写文件失败"));

		CloseHandle(hFileWrite);
		return FALSE;
	}

	if(!WriteFile(hFileWrite,&ch2Param, sizeof(CRAFT_PARAM),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写文件失败"));

		CloseHandle(hFileWrite);
		return FALSE;
	}

	CloseHandle(hFileWrite);

	int nRet = wcscmp(lpszPathName, lpDefaultParamFile);
	if(nRet != 0)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("文件保存成功"));
	}
	else
	{
		if(hWndCraftParam != 0)
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("文件保存成功"));
		}
	}
	return TRUE;	
}
/**************************
*保存按钮自定义处理过程
* @craftParam    hInstance[in]:
* @craftParam		message[in]:
* @craftParam		wParam [in]:
* @craftParam		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCraftParamSaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					if(!gbHotCraftParamFlag)
					{
						ShowWindow(hWndFileManage,SW_SHOW);
						UpdateWindow(hWndFileManage);
						SetFocus(currentFileManageBtn);//上级菜单最后选中的按钮获得输入焦点

						DestroyWindow(hWndCraftParam);
						hWndCraftParam = 0;					
						
						gCurrentFocusBtn = currentFileManageBtn;

						gCurrentWnd = hWndFileManage;
					}
					else
					{
						if(gCurrentWnd)//有子菜单窗口
						{
							ShowWindow(gCurrentWnd, SW_SHOW);
							SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
						}

						DestroyWindow(hWndCraftParam);
						hWndCraftParam = 0;
						if(!gCurrentWnd)//没有子菜单窗口
						{
							SetMenuStateFlag(FALSE);
							SetFocus(hWndMain);//主窗口获得输入焦点
						}
						
						gbHotCraftParamFlag = FALSE;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(craftParamBtnList[i] != currentCraftParamBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCraftParamBtn = craftParamBtnList[i];
					SetFocus(currentCraftParamBtn);
					if(!gbHotCraftParamFlag)//没有弹出所有参数配置窗口
						gCurrentFocusBtn = currentCraftParamBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(craftParamBtnList[i] != currentCraftParamBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCraftParamBtn = craftParamBtnList[i];
					SetFocus(currentCraftParamBtn);
					if(!gbHotCraftParamFlag)//没有弹出所有参数配置窗口
						gCurrentFocusBtn = currentCraftParamBtn;
				}
				else if(wParam == VK_RETURN)
				{
					if(SaveParamFile(ParamFileName))
					{
						SetParamFileName(hBtnCraftParamNumber, gnParamNo);//保存成功，刷新序号，以去掉无该文件标志
					}
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
					if(!gbHotCraftParamFlag)
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
	return CallWindowProc(OldBtnCraftParamSaveProc,Wnd,Msg,wParam,lParam);
}

void CheckChannelParam(CRAFT_PARAM& chParam, const WORD& chNo)
{
	SEND_PARAM_DATA sendParam;

	if(!(chParam.SendParam.nAmend >= MIN_AMEND && chParam.SendParam.nAmend <= MAX_AMEND) ||
	   !(chParam.SendParam.nMaiKuan >= MIN_MAIKUAN && chParam.SendParam.nMaiKuan <= MAX_MAIKUAN) ||
	   !(chParam.SendParam.nPress >= MIN_PRESSURE && chParam.SendParam.nPress <=MAX_PRESSURE) ||
	   !(chParam.SendParam.nRefrainFrequency >= MIN_RE_FREQ && chParam.SendParam.nRefrainFrequency <= MAX_RE_FREQ))
	{
		chParam.SendParam =  sendParam;
	}

	RECEIVE_PARAM_DATA receiveParam;

	if(!(chParam.ReceiveParam.nProbeFreq >= MIN_LVBO_TYPE && chParam.ReceiveParam.nProbeFreq <= MAX_LVBO_TYPE) ||
	   !(chParam.ReceiveParam.fDelay >= MIN_DELAY[chNo] && chParam.ReceiveParam.fDelay <= MAX_DELAY) ||
	   !(chParam.ReceiveParam.fPlus >= MIN_PLUS && chParam.ReceiveParam.fPlus <= MAX_PLUS) ||
	   !(chParam.ReceiveParam.fRefPlus >= MIN_PLUS && chParam.ReceiveParam.fRefPlus <= MAX_PLUS) ||
	   !(chParam.ReceiveParam.nJianBoMode >= 0 && chParam.ReceiveParam.nJianBoMode <= 3) ||
	   !(chParam.ReceiveParam.nLvBo >= 0 && chParam.ReceiveParam.nLvBo <= 255))
	{
		chParam.ReceiveParam = receiveParam;
	}

	DISPLAY_SET_DLG_DATA displayParam;
	if(!(chParam.DisplayParam.fRange >= 1 && chParam.DisplayParam.fRange <= MAX_RANGE) ||
	   !(chParam.DisplayParam.nWaveSpeed >= MIN_WAVE_SPEED && chParam.DisplayParam.nWaveSpeed <= MAX_WAVE_SPEED) ||
	   !(chParam.DisplayParam.fZheSheJiao >= MIN_ANGLE && chParam.DisplayParam.fZheSheJiao <= MAX_ANGLE) ||
	   !(chParam.DisplayParam.nBate >= MIN_BATE && chParam.DisplayParam.nBate <= MAX_BATE))
	{
		chParam.DisplayParam = displayParam;
	}

	GATE_INFO gateInfo[3];
	for(int i=0; i<3; i++)
	{
		if(!(chParam.GateInfos[i].fFrontGate >=0 && chParam.GateInfos[i].fFrontGate <= chParam.GateInfos[i].fBackGate) ||
		   !(chParam.GateInfos[i].fBackGate >= chParam.GateInfos[i].fFrontGate && chParam.GateInfos[i].fBackGate <= MAX_RANGE) ||
		   !(chParam.GateInfos[i].fPeakGate >= 0 && chParam.GateInfos[i].fPeakGate <= 100))
		{
			chParam.GateInfos[i].fFrontGate = gateInfo[i].fFrontGate;
			chParam.GateInfos[i].fBackGate = gateInfo[i].fBackGate;
			chParam.GateInfos[i].fPeakGate = gateInfo[i].fPeakGate;
		}
	}

	RAILROAD_PARAM_DATA railRoadParam;
	for(int i = 0; i < TL_STANDARD_NUM; i++)
	{
		for(int j = 0; j < TL_POSITION_NUM; j++)
		{
			if(chParam.RailRoadParam.fRange[i][j] <= 0 || chParam.RailRoadParam.fRange[i][j] > MAX_RANGE)
			{
				chParam.RailRoadParam.fRange[i][j] = railRoadParam.fRange[i][j];
			}
			
			if(chParam.RailRoadParam.fPlus[i][j] <= MIN_PLUS || chParam.RailRoadParam.fPlus[i][j] > MAX_PLUS)
			{
				chParam.RailRoadParam.fPlus[i][j] = railRoadParam.fPlus[i][j];
			}
			if(chParam.RailRoadParam.fBuChangGain< MIN_PLUS || chParam.RailRoadParam.fBuChangGain[i][j] > MAX_PLUS)
			{
				chParam.RailRoadParam.fPlus[i][j] = railRoadParam.fPlus[i][j];
			}
		}
	}
	
	if(chParam.SelectedDacPointDataNum > MAX_CURVE_POINT_NUM || chParam.fTcgFirstGain > MAX_PLUS || chParam.fTcgFirstGain < MIN_PLUS)
	{
		chParam.SelectedDacPointDataNum = 0;
		memset(chParam.SelectedDacPointData, 0 ,sizeof(chParam.SelectedDacPointData));
	}

	if(chParam.dgsCurve.selectedDgsPointDataNum[0] > MAX_CURVE_POINT_NUM || chParam.dgsCurve.fFirstGain > MAX_PLUS || 
		chParam.dgsCurve.fFirstGain < MIN_PLUS || chParam.dgsCurve.fPingDing > MAX_CURVE_GAIN || chParam.dgsCurve.fPingDing < MIN_CURVE_GAIN ||
		chParam.dgsCurve.fDingLiang> MAX_CURVE_GAIN || chParam.dgsCurve.fDingLiang < MIN_CURVE_GAIN ||
		chParam.dgsCurve.fPanFei > MAX_CURVE_GAIN || chParam.dgsCurve.fPanFei < MIN_CURVE_GAIN)
	{
		chParam.dgsCurve.selectedDgsPointDataNum[0] = 0;
		memset(chParam.dgsCurve.selectedDgsPointData, 0 ,sizeof(chParam.dgsCurve.selectedDgsPointData));
	}
	
	if(chParam.avgCurve.selectedPointDataNum> MAX_CURVE_POINT_NUM || chParam.avgCurve.fFirstGain > MAX_PLUS ||
		chParam.avgCurve.fFirstGain < MIN_PLUS || chParam.avgCurve.fFai1> MAX_FAIZHI || chParam.avgCurve.fFai1 < MIN_FAIZHI + 1 ||
		chParam.avgCurve.fFai2> MAX_FAIZHI || chParam.avgCurve.fFai2 < MIN_FAIZHI ||
		chParam.avgCurve.fFai3 > MAX_FAIZHI || chParam.avgCurve.fFai3 < MIN_FAIZHI)
	{
		chParam.avgCurve.selectedPointDataNum = 0;
		memset(chParam.avgCurve.selectedPointData, 0 ,sizeof(chParam.avgCurve.selectedPointData));
		chParam.avgCurve.fFai1 = 1;
	}
}

BOOL ReadParamFile(LPCTSTR lpszPathName)
{
	CRAFT_PARAM ch1Param;
	CRAFT_PARAM ch2Param;					

	HANDLE hFileWrite;	//文件句柄
	DWORD dwWriteDataSize;
	hFileWrite = CreateFile(lpszPathName,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("打开PAR文件失败"));
		return FALSE;
	}

	InitializeCriticalSection(&csSysInfo);
	EnterCriticalSection(&csSysInfo);
	if(!ReadFile(hFileWrite,&gSysInfo, sizeof(SYS_INFO),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("读文件失败"));

		CloseHandle(hFileWrite);
		return FALSE;
	}
	LeaveCriticalSection(&csSysInfo);
	DeleteCriticalSection(&csSysInfo);

	if(!ReadFile(hFileWrite,&ch1Param, sizeof(CRAFT_PARAM),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("读文件失败"));
		
		CloseHandle(hFileWrite);
		return FALSE;
	}

	if(!ReadFile(hFileWrite,&ch2Param, sizeof(CRAFT_PARAM),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("读文件失败"));

		CloseHandle(hFileWrite);
		return FALSE;
	}

	CloseHandle(hFileWrite);

//	SendParam_ad[0] = ch1Param.SendParam;
//	ReceiveParam_ad[0] = ch1Param.ReceiveParam;
//	DisplaySet_ad[0] = ch1Param.DisplayParam;

	CheckChannelParam(ch1Param, 0);
	CheckChannelParam(ch2Param, 1);

	// 赋值并检测
	memcpy(&SendParam_ad[0], &(ch1Param.SendParam), sizeof(SEND_PARAM_DATA));
	

	memcpy(&ReceiveParam_ad[0], &(ch1Param.ReceiveParam), sizeof(RECEIVE_PARAM_DATA));
	memcpy(&DisplaySet_ad[0], &(ch1Param.DisplayParam), sizeof(DISPLAY_SET_DLG_DATA));

	memcpy(GateInfos[0], ch1Param.GateInfos, sizeof(GATE_INFO)*3);
	//TCG
	gSelectedDacPointDataNum[0] = ch1Param.SelectedDacPointDataNum;
	gfTcgFirstPointGain[0] = ch1Param.fTcgFirstGain;
	memcpy(gSelectedDacPointData[0], ch1Param.SelectedDacPointData, sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	gfOldTcgRange[0]  = ch1Param.fOldTcgRang;
	memcpy(gwOldTcgSerial[0], ch1Param.wOldTcgSerial, sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	//DAC
	for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
	{
		gDgsSwitch[0][j] = ch1Param.dgsCurve.bSwitch[j];
	}

	gfPingDingGain[0] = ch1Param.dgsCurve.fPingDing;
	gfDingLiangGain[0] = ch1Param.dgsCurve.fDingLiang;
	gfPanHuiGain[0] = ch1Param.dgsCurve.fPanFei;
	
	gfDacFirstPointGain[0] = ch1Param.dgsCurve.fFirstGain;
	gfOldDacRange[0] = ch1Param.dgsCurve.fOldRang;
	memcpy(gwOldDacSerial[0], ch1Param.dgsCurve.wOldSerial, sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		gSelectedDgsPointDataNum[0][j] = ch1Param.dgsCurve.selectedDgsPointDataNum[j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		memcpy(gSelectedDgsPointData[0][j], ch1Param.dgsCurve.selectedDgsPointData[j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//AVG
	gbAvgCurveFlag[0] = ch1Param.avgCurve.bSwitch;
	gfDefectZhi1[0] = ch1Param.avgCurve.fFai1 ;
	gfDefectZhi2[0] = ch1Param.avgCurve.fFai2;
	gfDefectZhi3[0] = ch1Param.avgCurve.fFai3;
	gfAvgFirstPointGain[0] = ch1Param.avgCurve.fFirstGain;
	gfOldAvgRange[0] = ch1Param.avgCurve.fOldRange;
	memcpy(gwOldAvgSerial[0], ch1Param.avgCurve.wOldSerial, sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	gSelectedAvgPointDataNum[0] = ch1Param.avgCurve.selectedPointDataNum;
	for(int j = 0; j < AVG_CURVE_NUM + 1; j++)
	{
		memcpy(gSelectedAvgPointData[0][j], ch1Param.avgCurve.selectedPointData[j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}	
	//铁路
	gwStandard[0] = ch1Param.RailRoadParam.wStandard;
	gwPosition[0][gwStandard[0]] = ch1Param.RailRoadParam.wPosition;
	memcpy(gfTlBuChangPlus[0],ch1Param.RailRoadParam.fBuChangGain, sizeof(ch1Param.RailRoadParam.fBuChangGain));
	memcpy(gfTlRange[0],ch1Param.RailRoadParam.fRange, sizeof(ch1Param.RailRoadParam.fRange));
	memcpy(gfTlPlus[0],ch1Param.RailRoadParam.fPlus, sizeof(ch1Param.RailRoadParam.fPlus));
	memcpy(gbTlSheDingFlag[0], ch1Param.RailRoadParam.bFlag, sizeof(ch1Param.RailRoadParam.bFlag));
	
//	SendParam_ad[1] = ch2Param.SendParam;
//	ReceiveParam_ad[1] = ch2Param.ReceiveParam;
//	DisplaySet_ad[1] = ch2Param.DisplayParam;
	memcpy(&SendParam_ad[1], &(ch2Param.SendParam), sizeof(SEND_PARAM_DATA));
	memcpy(&ReceiveParam_ad[1], &(ch2Param.ReceiveParam), sizeof(RECEIVE_PARAM_DATA));
	memcpy(&DisplaySet_ad[1], &(ch2Param.DisplayParam), sizeof(DISPLAY_SET_DLG_DATA));

	memcpy(GateInfos[1], ch2Param.GateInfos, sizeof(GATE_INFO)*3);
	//TCG
	gSelectedDacPointDataNum[1] = ch2Param.SelectedDacPointDataNum;
	gfTcgFirstPointGain[1] = ch2Param.fTcgFirstGain;
	memcpy(gSelectedDacPointData[1], ch2Param.SelectedDacPointData, sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	gfOldTcgRange[1]  = ch2Param.fOldTcgRang;
	memcpy(gwOldTcgSerial[1], ch2Param.dgsCurve.wOldSerial, sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	//DAC
	for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
	{
		gDgsSwitch[1][j] = ch2Param.dgsCurve.bSwitch[j];
	}
	
	gfPingDingGain[1] = ch2Param.dgsCurve.fPingDing;
	gfDingLiangGain[1] = ch2Param.dgsCurve.fDingLiang;
	gfPanHuiGain[1] = ch2Param.dgsCurve.fPanFei;
	
	gfDacFirstPointGain[1] = ch2Param.dgsCurve.fFirstGain;
	gfOldDacRange[1] = ch2Param.dgsCurve.fOldRang;
	memcpy(gwOldDacSerial[1], ch2Param.dgsCurve.wOldSerial, sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		gSelectedDgsPointDataNum[1][j] = ch2Param.dgsCurve.selectedDgsPointDataNum[j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		memcpy(gSelectedDgsPointData[1][j], ch2Param.dgsCurve.selectedDgsPointData[j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//AVG
	gbAvgCurveFlag[1] = ch2Param.avgCurve.bSwitch;
	gfDefectZhi1[1] = ch2Param.avgCurve.fFai1 ;
	gfDefectZhi2[1] = ch2Param.avgCurve.fFai2;
	gfDefectZhi3[1] = ch2Param.avgCurve.fFai3;
	gfAvgFirstPointGain[1] = ch2Param.avgCurve.fFirstGain;
	gfOldAvgRange[1] = ch2Param.avgCurve.fOldRange;
	for(WORD j = 0; j < ch2Param.avgCurve.selectedPointDataNum; j++)
	{
		gwOldAvgSerial[1][j] = ch2Param.avgCurve.wOldSerial[j];
	}
	gSelectedAvgPointDataNum[1] = ch2Param.avgCurve.selectedPointDataNum;
	for(int j = 0; j < AVG_CURVE_NUM + 1; j++)
	{
		memcpy(gSelectedAvgPointData[1][j], ch2Param.avgCurve.selectedPointData[j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//铁路
	gwStandard[1] = ch2Param.RailRoadParam.wStandard;
	gwPosition[1][gwStandard[1]] = ch2Param.RailRoadParam.wPosition;
	memcpy(gfTlBuChangPlus[1],ch2Param.RailRoadParam.fBuChangGain, sizeof(ch2Param.RailRoadParam.fBuChangGain));
	memcpy(gfTlRange[1],ch2Param.RailRoadParam.fRange, sizeof(ch2Param.RailRoadParam.fRange));
	memcpy( gfTlPlus[1],ch2Param.RailRoadParam.fPlus, sizeof(ch2Param.RailRoadParam.fPlus));
	memcpy(gbTlSheDingFlag[1], ch2Param.RailRoadParam.bFlag, sizeof(ch2Param.RailRoadParam.bFlag));
	// 根据角度是否为零，设置直、斜探头标志
	InitializeCriticalSection(&csProbeType);

	for(int i=0; i<2; i++)
	{
		EnterCriticalSection(&csProbeType);
		if(fabs(DisplaySet_ad[i].fZheSheJiao) < 1E-6)
			wTanTouLeiXing[i] = 1;
		else
			wTanTouLeiXing[i] = 0;
		LeaveCriticalSection(&csProbeType);
	}

	DeleteCriticalSection(&csProbeType);
	return TRUE;
}
/**************************
*取出按钮自定义处理过程
* @craftParam    hInstance[in]:
* @craftParam		message[in]:
* @craftParam		wParam [in]:
* @craftParam		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCraftParamGetProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					if(!gbHotCraftParamFlag)
					{
						ShowWindow(hWndFileManage,SW_SHOW);
						UpdateWindow(hWndFileManage);
						SetFocus(currentFileManageBtn);//上级菜单最后选中的按钮获得输入焦点

						DestroyWindow(hWndCraftParam);
						hWndCraftParam = 0;					
						
						gCurrentFocusBtn = currentFileManageBtn;

						gCurrentWnd = hWndFileManage;
					}
					else
					{
						if(gCurrentWnd)//有子菜单窗口
						{
							ShowWindow(gCurrentWnd, SW_SHOW);
							SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
						}

						DestroyWindow(hWndCraftParam);
						hWndCraftParam = 0;
						if(!gCurrentWnd)//没有子菜单窗口
						{
							SetMenuStateFlag(FALSE);
							SetFocus(hWndMain);//主窗口获得输入焦点
						}
						
						gbHotCraftParamFlag = FALSE;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(craftParamBtnList[i] != currentCraftParamBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCraftParamBtn = craftParamBtnList[i];
					SetFocus(currentCraftParamBtn);
					if(!gbHotCraftParamFlag)//没有弹出所有参数配置窗口
						gCurrentFocusBtn = currentCraftParamBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(craftParamBtnList[i] != currentCraftParamBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCraftParamBtn = craftParamBtnList[i];
					SetFocus(currentCraftParamBtn);
					if(!gbHotCraftParamFlag)//没有弹出所有参数配置窗口
						gCurrentFocusBtn = currentCraftParamBtn;
				}
				else if(wParam == VK_RETURN)
				{
					if(ReadParamFile(ParamFileName))
					{
						currentUsedParamNo = gnParamNo;
						
						HKEY HKeyNum;
						LONG ret = RegOpenKeyEx(HKEY_CURRENT_USER, _T("ParNum"), 0, 0,&HKeyNum);
					
						if(ret != ERROR_SUCCESS)
						{
								MessageBox (Wnd, TEXT ("读注册表失败"), TEXT("NDT_CE"), 0) ;
					        
					        return -1 ;
						}
						
						RegSetValueEx(HKeyNum, _T("ParNum"), 0, REG_DWORD, (const BYTE *)&currentUsedParamNo, sizeof(DWORD));
					
						RegCloseKey(HKeyNum);
						
						swprintf(lpDefaultParamFile, _T("\\ResidentFlash\\%d.par"), currentUsedParamNo);
						// songchenguang add start 2010-05-18
						//ReadParamFile(ParamFileName);
						// 把参数发下去
						InitParam();
						// songchenguang add end 2010-05-18						
						
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("工艺取出成功"));
					}
					else
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("工艺取出失败"));
					
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
					if(!gbHotCraftParamFlag)
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
	return CallWindowProc(OldBtnCraftParamGetProc,Wnd,Msg,wParam,lParam);
}
/**************************
*删除按钮自定义处理过程
* @craftParam    hInstance[in]:
* @craftParam		message[in]:
* @craftParam		wParam [in]:
* @craftParam		lParam [in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCraftParamDeleteProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					if(!gbHotCraftParamFlag)
					{
						ShowWindow(hWndFileManage,SW_SHOW);
						UpdateWindow(hWndFileManage);
						SetFocus(currentFileManageBtn);//上级菜单最后选中的按钮获得输入焦点

						DestroyWindow(hWndCraftParam);
						hWndCraftParam = 0;					
						
						gCurrentFocusBtn = currentFileManageBtn;

						gCurrentWnd = hWndFileManage;
					}
					else
					{
						if(gCurrentWnd)//有子菜单窗口
						{
							ShowWindow(gCurrentWnd, SW_SHOW);
							SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
						}

						DestroyWindow(hWndCraftParam);
						hWndCraftParam = 0;
						if(!gCurrentWnd)//没有子菜单窗口
						{
							SetMenuStateFlag(FALSE);
							SetFocus(hWndMain);//主窗口获得输入焦点
						}
						
						gbHotCraftParamFlag = FALSE;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(craftParamBtnList[i] != currentCraftParamBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCraftParamBtn = craftParamBtnList[i];
					SetFocus(currentCraftParamBtn);
					if(!gbHotCraftParamFlag)//没有弹出所有参数配置窗口
						gCurrentFocusBtn = currentCraftParamBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(craftParamBtnList[i] != currentCraftParamBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCraftParamBtn = craftParamBtnList[i];
					SetFocus(currentCraftParamBtn);
					if(!gbHotCraftParamFlag)//没有弹出所有参数配置窗口
						gCurrentFocusBtn = currentCraftParamBtn;
				}
				else if(wParam == VK_RETURN)
				{
					// songchenguang add start 2010-05-18
					BOOL bRet = DeleteFile(ParamFileName);

					if(!bRet)
					{
						PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("文件不存在"));
					}
					else
					{
						PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("文件删除成功"));
						SetParamFileName(hBtnCraftParamNumber, gnParamNo);
					}
					// songchenguang add end 2010-05-18
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
					if(!gbHotCraftParamFlag)
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
	return CallWindowProc(OldBtnCraftParamDeleteProc,Wnd,Msg,wParam,lParam);
}
