/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 测厚探头V型补偿曲线创建窗口源文件
 * Filename: chHeadVCreateWin.cpp
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

HWND hWndChHeadVCreate = NULL;							//测厚探头V型补偿曲线创建窗口句柄
HWND currentChHeadVCreateBtn = NULL;					//测厚探头V型补偿曲线创建窗口当前选中的按钮
WORD gwHeadVPointNum[HEAD_NUM + 10] ={0};						//探头V型补偿曲线点的个数
BOOL gbDrawHeadVCurveFlag = FALSE;					//是否在画探头V型补偿曲线
HEADVPOINT_DATA gstHeadVPointData[HEAD_NUM + 10][HEADV_POINT_MAX];		//采集的探头V型补偿曲线点
BOOL gbHeadVCurveOkFlag = FALSE;					//探头V型补偿曲线是否完成

static HWND hBtnChHeadVCreateActualHoudu = NULL;			//厚度按钮句柄
static HWND hBtnChHeadVCreateMeasureHoudu = NULL;			//测量的厚度按钮句柄
static HWND hBtnChHeadVCreateAddPoint = NULL;				//加样点按钮句柄
static HWND hBtnChHeadVCreateDeletePoint = NULL;			//删样点按钮句柄
static HWND hBtnChHeadVCreateEnd = NULL;					//结束按钮句柄

static HWND chHeadVCreateBtnList[5] ={NULL,NULL,NULL,NULL,NULL};							//测厚探头V型补偿曲线创建窗口的按钮表
static WNDPROC OldBtnChHeadVCreateMeasureHouduProc = NULL;		//测量的厚度按钮默认处理过程
static WNDPROC OldBtnChHeadVCreateActualHouduProc = NULL;		//厚度按钮默认处理过程
static WNDPROC OldBtnChHeadVCreateAddPointProc = NULL;			//加样点按钮默认处理过程
static WNDPROC OldBtnChHeadVCreateDeletePointProc = NULL;		//删样点按钮默认处理过程
static WNDPROC OldBtnChHeadVCreateEndProc = NULL;				//结束按钮默认处理过程

static const int buttonNum = 5;				//按钮控件数
static float fThickness = 10;			//试块厚度
static float fMeasureThickness = 0;	//测量的试块厚度
static float fThicknessStep = 1;		//试块厚度步长
static WORD wThicknessStepIndex = 1;	//试块厚度步长索引
static WORD wDeleteStep = 0;			
static POINT stHeadVPoint[HEADV_POINT_MAX];

ATOM registerChHeadVCreateChild(HINSTANCE hInstance);
static LRESULT CALLBACK chHeadVCreateChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chHeadVCreate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChHeadVCreateMeasureHouduProc( HWND Wnd, 
													UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChHeadVCreateActualHouduProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChHeadVCreateAddPointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChHeadVCreateDeletePointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChHeadVCreateEndProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//程序实例句柄
extern HWND hWndHeadVMake;				//探头V型补偿曲线制作窗口句
extern HWND currentHeadVMakeBtn;		//探头V型补偿曲线制作窗中当前选中的按钮
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND gCurrentWnd;
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;						// 状态栏句柄
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD CurrentChannelNo;				// 当前通道号
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern WORD gwHeadVCurveNum;			//探头V型补偿曲线序号
extern CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// 工作线程参数
extern PEAK_INFO gPeakDefectInfos[2][2];	//保存闸门峰值点信息
extern CRITICAL_SECTION csPeakDefectInfos;
extern CRITICAL_SECTION csMenuState;
extern SYS_COLOR_INFO gstSysColor[SYS_COLOR_NUM];
extern int gCurrentSysColorIndex;
extern TCHAR strHeadVFileName[MAX_PATH];
extern HWND hBtnHeadVMakeCreate;//创建按钮句柄
extern GATE_INFO GateInfos[2][3];
extern GATE_INFO stOldGateInfos[2][3];

extern void SetDistanceGate(const WORD& nIndex);

/**************************
*保存探头V型补偿曲线到文件
* @param		hWnd[in]:窗口句柄
* @param		wNum[in]:序号
* @return   LRESULT :
* @since    1.00
***************************/
/*	//分成多个文件
BOOL saveHeadVFile(LPCTSTR lpszPathName)
{
	WORD wHeadVPoinNum = gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM];
	HEADVPOINT_DATA stHeadVpointData[HEADV_POINT_MAX];
	memcpy(stHeadVpointData, gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM], sizeof(HEADVPOINT_DATA)*HEADV_POINT_MAX);
	
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
		return -1;
	}

	if(!WriteFile(hFileWrite,&wHeadVPoinNum, sizeof(WORD),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写文件失败"));

		CloseHandle(hFileWrite);
		return FALSE;
	}
	if(!WriteFile(hFileWrite,stHeadVpointData, sizeof(HEADVPOINT_DATA) * HEADV_POINT_MAX,&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写文件失败"));

		CloseHandle(hFileWrite);
		return FALSE;
	}

	CloseHandle(hFileWrite);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("文件保存成功"));

	return TRUE;	
}
*/
BOOL saveHeadVFile(LPCTSTR lpszPathName)
{	
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
		return -1;
	}
	for(int i = 0; i < HEAD_NUM + 10; i++)
	{
		if(!WriteFile(hFileWrite,&gwHeadVPointNum[i], sizeof(WORD),&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写文件失败"));

			CloseHandle(hFileWrite);
			return FALSE;
		}
	}
	for(int i = 0; i < HEAD_NUM + 10; i++)
	{
		if(!WriteFile(hFileWrite,gstHeadVPointData[i], sizeof(HEADVPOINT_DATA) * HEADV_POINT_MAX, &dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写文件失败"));

			CloseHandle(hFileWrite);
			return FALSE;
		}
	}

	CloseHandle(hFileWrite);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("文件保存成功"));

	return TRUE;	
}

/**************************
*显示试块厚度
* @param		hWnd[in]:窗口句柄
* @param		fHouDu[in]:钢板厚度
* @return   LRESULT :
* @since    1.00
***************************/
static void displayThickness(HWND hWnd ,float fHouDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(fHouDu == 0)
		wsprintf(strText, _T("N/A"));
	else
		wsprintf(strText, _T("%.1f"), fHouDu);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}

/**************************
*注册测厚探头V型补偿曲线创建窗口
* @chHeadVCreate    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChHeadVCreateChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chHeadVCreateChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChHeadVCreateChild;

	return RegisterClass(&wc);
}
/**************************
*测厚探头V型补偿曲线创建窗口处理过程
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		message[in]:
* @chHeadVCreate		wParam[in]:
* @chHeadVCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chHeadVCreateChildProc(HWND hWnd, UINT message, 
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
			SetDistanceGate(1);
			SetDistanceGate(2);
			fMeasureThickness = 0;
			
			if(currentHeadVMakeBtn == hBtnHeadVMakeCreate)//创建, 清空数据
			{
				gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] = 0;
				memset(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM], 0, sizeof(HEADV_POINT_MAX) * HEADV_POINT_MAX);
				//saveHeadVFile(strHeadVFileName);
			}
		/*	//TEST
			gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] = 2;
			gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][1].fActualHouDu = 20;
			gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][1].fMeasureHouDu= 40;
			//TEST */
			memset(stHeadVPoint, 0, sizeof(POINT) * HEADV_POINT_MAX);
			for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
			{
				stHeadVPoint[i] = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i]);
			}
			
			gbHeadVCurveOkFlag = FALSE;
			
			//画按钮控件
			hBtnChHeadVCreateActualHoudu = CreateWindow(_T("BUTTON"),_T("ChHeadVCreateActualHoudu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHHEADVCREATE_ACTUALHOUDU,hInst,NULL);
			hBtnChHeadVCreateMeasureHoudu = CreateWindow(_T("BUTTON"),_T("ChHeadVCreateMeasureHoudu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHHEADVCREATE_MEASUREHOUDU,hInst,NULL);			
			hBtnChHeadVCreateAddPoint = CreateWindow(_T("BUTTON"),_T("ChHeadVCreateAddPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHHEADVCREATE_ADDPOINT,hInst,NULL);
			hBtnChHeadVCreateDeletePoint = CreateWindow(_T("BUTTON"),_T("ChHeadVCreateDeletePoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHHEADVCREATE_DELETEPOINT,hInst,NULL);
			hBtnChHeadVCreateEnd = CreateWindow(_T("BUTTON"),_T("ChHeadVCreateEnd"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHHEADVCREATE_END,hInst,NULL);
					
			chHeadVCreateBtnList[0] = hBtnChHeadVCreateActualHoudu;
			chHeadVCreateBtnList[1] = hBtnChHeadVCreateMeasureHoudu;
			chHeadVCreateBtnList[2] = hBtnChHeadVCreateAddPoint;
			chHeadVCreateBtnList[3] = hBtnChHeadVCreateDeletePoint;
			chHeadVCreateBtnList[4] = hBtnChHeadVCreateEnd;
			
			//按钮控件的自定义处理过程
			OldBtnChHeadVCreateMeasureHouduProc = (WNDPROC) GetWindowLong(hBtnChHeadVCreateMeasureHoudu,GWL_WNDPROC);
			SetWindowLong(hBtnChHeadVCreateMeasureHoudu,GWL_WNDPROC,(LONG)btnChHeadVCreateMeasureHouduProc);
			
			OldBtnChHeadVCreateActualHouduProc = (WNDPROC) GetWindowLong(hBtnChHeadVCreateActualHoudu,GWL_WNDPROC);
			SetWindowLong(hBtnChHeadVCreateActualHoudu,GWL_WNDPROC,(LONG)btnChHeadVCreateActualHouduProc);
			
			OldBtnChHeadVCreateAddPointProc = (WNDPROC) GetWindowLong(hBtnChHeadVCreateAddPoint,GWL_WNDPROC);
			SetWindowLong(hBtnChHeadVCreateAddPoint,GWL_WNDPROC,(LONG)btnChHeadVCreateAddPointProc);
			
			OldBtnChHeadVCreateDeletePointProc = (WNDPROC) GetWindowLong(hBtnChHeadVCreateDeletePoint,GWL_WNDPROC);
			SetWindowLong(hBtnChHeadVCreateDeletePoint,GWL_WNDPROC,(LONG)btnChHeadVCreateDeletePointProc);
			
			OldBtnChHeadVCreateEndProc = (WNDPROC) GetWindowLong(hBtnChHeadVCreateEnd,GWL_WNDPROC);
			SetWindowLong(hBtnChHeadVCreateEnd,GWL_WNDPROC,(LONG)btnChHeadVCreateEndProc);
		
			currentChHeadVCreateBtn = chHeadVCreateBtnList[0];//默认当前选中测量厚度按钮
			SetFocus(currentChHeadVCreateBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentChHeadVCreateBtn;
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
			chHeadVCreate_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnChHeadVCreateActualHoudu)
			{
				displayThickness(pdis->hwndItem,fThickness);
			}
			if(pdis->hwndItem == hBtnChHeadVCreateMeasureHoudu)
			{
				displayThickness(pdis->hwndItem,fMeasureThickness);
				SetDistanceGate(1);
				SetDistanceGate(2);
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
*加载测厚探头V型补偿曲线创建窗口按钮图片
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chHeadVCreate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chHeadVCreateBtnBitmap[]=
	{
		{IDC_CHHEADVCREATE_ACTUALHOUDU	,IDB_BITMAP_SHIKUAIHOUDU5_NOSEL_R	 ,  IDB_BITMAP_SHIKUAIHOUDU5_SELECT_R		},
		{IDC_CHHEADVCREATE_MEASUREHOUDU	,IDB_BITMAP_CELIANGHOUDU_NOSEL_R	 ,  IDB_BITMAP_CELIANGHOUDU_SELECT_R		},
		{IDC_CHHEADVCREATE_ADDPOINT	,IDB_BITMAP_CHJIAYANGDIAN5_NOSEL_R ,  IDB_BITMAP_CHJIAYANGDIAN5_SELECT_R	},
		{IDC_CHHEADVCREATE_DELETEPOINT	,IDB_BITMAP_SHANYANGDIAN5_NOSEL_R ,  IDB_BITMAP_SHANYANGDIAN5_SELECT_R	},
		{IDC_CHHEADVCREATE_END	,IDB_BITMAP_JIESHU5_NOSEL_R ,  IDB_BITMAP_JIESHU5_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chHeadVCreateBtnBitmap);
	return ReturnVal;
}

/**************************
*厚度按钮自定义处理过程
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		message[in]:
* @chHeadVCreate		wParam[in]:
* @chHeadVCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChHeadVCreateActualHouduProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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
					ShowWindow(hWndHeadVMake,SW_SHOW);
					SetFocus(currentHeadVMakeBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChHeadVCreate);
					hWndChHeadVCreate = 0;

					gCurrentFocusBtn = currentHeadVMakeBtn;
					gCurrentWnd = hWndHeadVMake;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					gbDrawHeadVCurveFlag = TRUE;		//X坐标轴改成试块厚度
					
					SetOpenFlag(NULL, FALSE, 1);		//关掉闸门1
					SetOpenFlag(NULL, FALSE, 2);		//关掉闸门2
					Sleep(100);								//等待	确认重画完成
					
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					//gbEndFlag =TRUE;
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					Sleep(50);//等待，确认画图线程停止画波形
					clearWave();//删除波形
					//SetRedrawFlag(TRUE);	//分格虚线有可能被部分擦掉，所以要重画
					//画所有点
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i],BLUE_POINT, TRUE);
					}
				}
				else if(wParam == VK_DOWN)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(fThickness > 1)
					{
						if((fThickness - fThicknessStep) >= 1)
							fThickness -= fThicknessStep;
						else
							fThickness = 1;

						fMeasureThickness = 0;
						displayThickness(hBtnChHeadVCreateMeasureHoudu, fMeasureThickness);
					}
					displayThickness(Wnd, fThickness);

					SetDistanceGate(1);
					SetDistanceGate(2);
				}
				else if(wParam == VK_RIGHT)
				{
					fThickness += fThicknessStep;

					fMeasureThickness = 0;
					displayThickness(hBtnChHeadVCreateMeasureHoudu, fMeasureThickness);
					
					displayThickness(Wnd, fThickness);

					SetDistanceGate(1);
					SetDistanceGate(2);
				}
				else if(wParam == VK_RETURN)
				{
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
	return CallWindowProc(OldBtnChHeadVCreateActualHouduProc,Wnd,Msg,wParam,lParam);
}
/**************************
*测量的声速按钮自定义处理过程
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		message[in]:
* @chHeadVCreate		wParam[in]:
* @chHeadVCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChHeadVCreateMeasureHouduProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndHeadVMake,SW_SHOW);
					SetFocus(currentHeadVMakeBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChHeadVCreate);
					hWndChHeadVCreate = 0;

					gCurrentFocusBtn = currentHeadVMakeBtn;
					gCurrentWnd = hWndHeadVMake;
							
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
				}
			
				else if(wParam == VK_UP)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					gbDrawHeadVCurveFlag = TRUE;		//X坐标轴改成试块厚度

					SetOpenFlag(NULL, FALSE, 1);		//关掉闸门1
					SetOpenFlag(NULL, FALSE, 2);		//关掉闸门2
					Sleep(100);								//等待	确认重画完成

					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					//gbEndFlag =TRUE;
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					Sleep(50);//等待，确认画图线程停止画波形
					clearWave();//删除波形
					//SetRedrawFlag(TRUE);		//分格虚线有可能被部分擦掉，所以要重画,线程已停止，不起作用
					// 画虚线,分格虚线有可能被部分擦掉，所以要重画
					RECT RctWave;	// 波形区域
					RECT RctYScale; // Y坐标区域
					RECT RctXScale; // X坐标区域
					RECT RctRight;	// 右侧空白区
					RECT rect;	// 主窗口显示出来的矩形
					HDC hdc = GetDC(hWndMain);
					GetClipBox(hdc, &rect);
						
					InitializeCriticalSection(&csMenuState);
					EnterCriticalSection(&csMenuState);
					GetDisplayRect(rect, FALSE, RctWave, RctXScale, RctYScale, RctRight);
					LeaveCriticalSection(&csMenuState);
					DeleteCriticalSection(&csMenuState);
					
					DrawRectDashLine(RctWave, RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack));
					//画所有点
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i],BLUE_POINT, TRUE);
					}
					//画曲线
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], TRUE);
					
				}
				else if(wParam == VK_RETURN)
				{
				 	float fRang1 = 0;
					float fRang2 = 0;
					PEAK_INFO peakDefectInfos[2][2];
					InitializeCriticalSection(&csPeakDefectInfos);
					EnterCriticalSection(&csPeakDefectInfos);
					for(int i = 0 ;i < CHANNEL_NUM; i++)
					{
						peakDefectInfos[i][0] = gPeakDefectInfos[i][0];
						peakDefectInfos[i][1] = gPeakDefectInfos[i][1];
					}
					LeaveCriticalSection(&csPeakDefectInfos);
					DeleteCriticalSection(&csPeakDefectInfos);
					fRang1 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][0].wDefectTime);
					fRang2 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][1].wDefectTime);
					fMeasureThickness = fRang2 - fRang1;
					displayThickness(hBtnChHeadVCreateMeasureHoudu, fMeasureThickness);
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
	return CallWindowProc(OldBtnChHeadVCreateMeasureHouduProc,Wnd,Msg,wParam,lParam);
}

/**************************
*加样点按钮自定义处理过程
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		message[in]:
* @chHeadVCreate		wParam[in]:
* @chHeadVCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChHeadVCreateAddPointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndHeadVMake,SW_SHOW);
					SetFocus(currentHeadVMakeBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChHeadVCreate);
					hWndChHeadVCreate = 0;

					gCurrentFocusBtn = currentHeadVMakeBtn;
					gCurrentWnd = hWndHeadVMake;

					gbDrawHeadVCurveFlag = FALSE;		//X坐标轴改成声程值					

					//擦掉所有点
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i], BLUE_POINT, FALSE);
					}
					//擦掉曲线
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);
					//开始画波形
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
	
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
				}
			
				else if(wParam == VK_UP)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					gbDrawHeadVCurveFlag = FALSE;		//X坐标轴改成声程值

					//擦掉所有点
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i], BLUE_POINT, FALSE);
					}
					//擦掉曲线
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);
					//开始画波形
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					SetOpenFlag(NULL, TRUE, 1);//  打开闸门线
					SetOpenFlag(NULL, TRUE, 2);//  打开闸门线
				}
				else if(wParam == VK_DOWN)
				{
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					if(gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] != 0)
						stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
					drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//选中点画红
				}
				else if(wParam == VK_RETURN)
				{
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));
					
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);//擦掉曲线
					
					for(DWORD i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						if(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i].fActualHouDu == fThickness)
						{
							stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i]);
							drawShiZhi1(stTempPoint, BLUE_POINT, FALSE);//擦掉同一厚度的点
							
							gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i].fMeasureHouDu = fMeasureThickness;
							stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i]);
							drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);//画点
							break;
						}
					}
					gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]++;
					if(gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] > HEADV_POINT_MAX)
						gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] = HEADV_POINT_MAX;
					gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1].fActualHouDu = fThickness;
					gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1].fMeasureHouDu = fMeasureThickness;
					stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1]);
					drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);//画增加的点

					sortSelectedPointDataByHoudu(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM], gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]);

					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						stHeadVPoint[i] = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i]);
					}
					
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + + HEAD_NUM], TRUE);//重新画曲线
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
			/*	else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
				else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChHeadVCreateAddPointProc,Wnd,Msg,wParam,lParam);
}
/**************************
*删样点按钮自定义处理过程
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		message[in]:
* @chHeadVCreate		wParam[in]:
* @chHeadVCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChHeadVCreateDeletePointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndHeadVMake,SW_SHOW);
					SetFocus(currentHeadVMakeBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChHeadVCreate);
					hWndChHeadVCreate = 0;

					gCurrentFocusBtn = currentHeadVMakeBtn;
					gCurrentWnd = hWndHeadVMake;

					gbDrawHeadVCurveFlag = FALSE;		//X坐标轴改成声程值

					//擦掉所有点
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i], BLUE_POINT, FALSE);
					}
					//擦掉曲线
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);
					//开始画波形
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
				}
			
				else if(wParam == VK_UP)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);//擦掉曲线
					//画所有点
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i], BLUE_POINT, TRUE);
					}
				}
				else if(wParam == VK_LEFT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));

					stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
					drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);			//上次选中点还原颜色
					if(gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM])
					{						
						if(wDeleteStep == 0)
							wDeleteStep = gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1;
						else
							wDeleteStep--;

						stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
						drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//选中点画红
						swprintf(strText, _T("%.1f,%.1f"), gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep].fActualHouDu, 
											gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep].fMeasureHouDu);
					}
					else
					{
						swprintf(strText, _T("无样点"));
						
					}
					DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
				}
				else if(wParam == VK_RIGHT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));

					stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
					drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);			//上次选中点还原颜色
					if(gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM])
					{
						if(wDeleteStep == gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1)
							wDeleteStep = 0;
						else
							wDeleteStep++;

						stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
						drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//选中点画红
						swprintf(strText, _T("%.1f,%.1f"), gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep].fActualHouDu, 
											gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep].fMeasureHouDu);
					}
					else
					{
						swprintf(strText, _T("无样点"));
						
					}
					DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
				}
				else if(wParam == VK_RETURN)
				{
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));

					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);//擦掉曲线
					
					if(gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM])
					{
						if(gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] == 1)		//只有一个点
						{	
							stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][0]);
							drawShiZhi1(stTempPoint, RED_POINT, FALSE);			//删除选中点
							gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]--;
						}
						else
						{							
							if(wDeleteStep == (gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1))//删最后一个点
							{
								stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
								drawShiZhi1(stTempPoint, RED_POINT, FALSE);			//擦掉选中点
								gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]--;
								wDeleteStep--;
							}
							else
							{
								for(int i = wDeleteStep; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1; i++)
								{
									stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
									drawShiZhi1(stTempPoint, RED_POINT, FALSE);			//擦掉选中点
									gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i] = gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i + 1];//选中点后面的点数据往前移
								}
								gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]--;
							}
							stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
							drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//新的选中点画红
							
							for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
							{
								stHeadVPoint[i] = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i]);
							}
							
							drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], TRUE);//重新画曲线
						}
					}
					else
					{
						TCHAR strText[20];
						memset(strText, 0, sizeof(strText));
						swprintf(strText, _T("无样点可删"));
						DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
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
			/*	else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
				else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChHeadVCreateDeletePointProc,Wnd,Msg,wParam,lParam);
}
/**************************
*删样点按钮自定义处理过程
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		message[in]:
* @chHeadVCreate		wParam[in]:
* @chHeadVCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChHeadVCreateEndProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndHeadVMake,SW_SHOW);
					SetFocus(currentHeadVMakeBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndChHeadVCreate);
					hWndChHeadVCreate = 0;

					gCurrentFocusBtn = currentHeadVMakeBtn;
					gCurrentWnd = hWndHeadVMake;

					gbDrawHeadVCurveFlag = FALSE;		//X坐标轴改成声程值


					//擦掉所有点
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i], BLUE_POINT, FALSE);
					}
					//擦掉曲线
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);
					//开始画波形
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
				}
			
				else if(wParam == VK_UP)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], TRUE);//画曲线
				}
				else if(wParam == VK_DOWN)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					gbDrawHeadVCurveFlag = FALSE;		//X坐标轴改成声程值

					SetOpenFlag(NULL, TRUE, 1);		//打开闸门1
					SetOpenFlag(NULL, TRUE, 2);		//打开闸门2

					//擦掉所有点
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i], BLUE_POINT, FALSE);
					}
					//开始画波形
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					SetOpenFlag(NULL, TRUE, 1);//  打开闸门线
					SetOpenFlag(NULL, TRUE, 2);//  打开闸门线
				}
				else if(wParam == VK_RETURN)
				{
					saveHeadVFile(strHeadVFileName);

					gbDrawHeadVCurveFlag = FALSE;		//X坐标轴改成声程值	
					
					gbHeadVCurveOkFlag = TRUE;
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					DestroyWindow(hWndChHeadVCreate);
					hWndChHeadVCreate = 0;
					ShowWindow(hWndHeadVMake,SW_SHOW);
					SetFocus(currentHeadVMakeBtn);//上级菜单最后选中的按钮获得输入焦点
					gCurrentFocusBtn = currentHeadVMakeBtn;
					gCurrentWnd = hWndHeadVMake;
		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
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
			/*	else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
				else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChHeadVCreateEndProc,Wnd,Msg,wParam,lParam);
}

