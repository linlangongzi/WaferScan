/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 自动增益窗口源文件
 * Filename: autoGainPlusWin.cpp
 *
 * Spec: Trace Tag write here. If has not, ScreenCopy this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "gateWin.h"
#include "NDT_CE.h"
#include "recvParamWin.h"
#include "craftParamWin.h"

HWND hWndAutoGainPlus = NULL;//自动增益窗口句柄
HWND hBtnAutoGainPlusStart = NULL;//开始按钮句柄
HWND hBtnAutoGainPlusWaveHeight = NULL;//波高按钮句柄

HWND autoGainPlusBtnList[2] = {NULL, NULL};//自动增益窗口的按钮表
WNDPROC OldBtnAutoGainPlusStartProc = NULL;//开始按钮默认处理过程
WNDPROC OldBtnAutoGainPlusWaveHeightProc = NULL;//波高按钮默认处理过程
HWND currentAutoGainPlusBtn = NULL;//自动增益窗中当前选中的按钮

ATOM registerAutoGainPlusChild(HINSTANCE hInstance);
LRESULT CALLBACK autoGainPlusChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int autoGainPlus_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnAutoGainPlusStartProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnAutoGainPlusWaveHeightProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

static const int buttonNum = 2;//按钮控件数

static float fPeakSheDing = 80.0f;
static float PeakSheDingStep = 1;
static float fOldPlus = 0;				//改变增益之前的增益

HANDLE hDrawAutoGainThread = NULL;				// 自动增益程句柄
DWORD dwDrawAutoGainThreadID = 0;				// 自动增益线程ID
THREAD_PARAM stAutoGainThread_param;		// 自动增益线程参数

extern HWND hWndQuantitativeAnalysis;			//定量分析窗口句柄
extern HWND currentQuantitativeAnalysisBtn;		//定量分析窗中当前选中的按钮
extern HINSTANCE hInst;//程序实例句柄
extern HWND hWndMain;//主窗口
extern HWND gCurrentFocusBtn;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern WORD CurrentChannelNo;
extern HWND hStatusBar;
extern wchar_t gszTextPeak[20];
extern GATE_INFO GateInfos[2][3];
extern BOOL gbPeakRememberFlag;  	//峰值记忆是否开启
extern CRITICAL_SECTION csPeakRemember;
extern CRITICAL_SECTION csPeakInfo;
extern PEAK_INFO gPeakInfosRemeber[2][2];
extern BOOL gbHotAutoGainFlag;	//是否打开自动增益快捷方式 
extern HWND gCurrentWnd;
extern wchar_t lpDefaultParamFile[MAX_PATH];

extern BOOL gWaveDrawDacFlag;		//波形是否画DAC
extern BOOL gWaveDrawAvgFlag;		//波形是否画AVG
extern HWND hWndTlAdjust;	//调校窗口句柄
// songchenguang add end 2011-06-22
extern WORD MAX_PLUS;
// songchenguang add end 2011-06-22

/**************************
*注册自动增益窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerAutoGainPlusChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) autoGainPlusChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szAutoGainPlusChild;

	return RegisterClass(&wc);
}

void SetPeakSheDing(HWND hWnd, const float& fPeak)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fPeak);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

/**************************
*自动增益窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK autoGainPlusChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			//画按钮控件
			hBtnAutoGainPlusStart = CreateWindow(_T("BUTTON"),_T("Start"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_AUTOGAINPLUS_START,hInst,NULL);
			hBtnAutoGainPlusWaveHeight = CreateWindow(_T("BUTTON"),_T("WaveHeight"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_AUTOGAINPLUS_WAVEHEIGHT,hInst,NULL);
			
			autoGainPlusBtnList[0] = hBtnAutoGainPlusStart;
			autoGainPlusBtnList[1] = hBtnAutoGainPlusWaveHeight;
			//按钮控件的自定义处理过程
			OldBtnAutoGainPlusStartProc = (WNDPROC) GetWindowLong(hBtnAutoGainPlusStart,GWL_WNDPROC);
			SetWindowLong(hBtnAutoGainPlusStart,GWL_WNDPROC,(LONG)btnAutoGainPlusStartProc);
			OldBtnAutoGainPlusWaveHeightProc = (WNDPROC) GetWindowLong(hBtnAutoGainPlusWaveHeight,GWL_WNDPROC);
			SetWindowLong(hBtnAutoGainPlusWaveHeight,GWL_WNDPROC,(LONG)btnAutoGainPlusWaveHeightProc);

			currentAutoGainPlusBtn = autoGainPlusBtnList[0];//默认当前选中开始按钮
			SetFocus(currentAutoGainPlusBtn);//选中按钮获得输入焦点

			if(!gbHotAutoGainFlag)
				gCurrentFocusBtn = currentAutoGainPlusBtn;

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
			autoGainPlus_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnAutoGainPlusWaveHeight)
			{
				SetPeakSheDing(pdis->hwndItem, fPeakSheDing);
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
*加载自动增益窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int autoGainPlus_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	const BTN_BMP_INFO tbl_autoGainPlusBtnBitmap[]=
	{
		{IDC_AUTOGAINPLUS_START	,IDB_BITMAP_DBKAISHI_NOSEL_R	 ,  IDB_BITMAP_DBKAISHI_SELECT_R		},
		{IDC_AUTOGAINPLUS_WAVEHEIGHT	,IDB_BITMAP_BOGAO_NOSEL_R	 ,  IDB_BITMAP_BOGAO_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	int ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_autoGainPlusBtnBitmap);
	return ReturnVal;
}

DWORD WINAPI autoGainProc(LPVOID lpParameter)
{
	THREAD_PARAM* pParam = (THREAD_PARAM*)lpParameter;
	InitializeCriticalSection(&csPeakInfo);

	float fPeakValue = 0;
	do
	{
		// 检查当前波峰值
		EnterCriticalSection(&csPeakInfo);
		fPeakValue = float(gPeakInfosRemeber[CurrentChannelNo][0].wDefectAmp) / MAX_HEIGHT*100.0f;
		LeaveCriticalSection(&csPeakInfo);

		float fStep = 0.3;
		double fCha = fabs(fPeakValue - pParam->fValue);
		if(fCha > 5.0f)
			fStep = 0.3f;
		else
			fStep = 0.05f;

		// 若大于设定值，减基准增益
		if(fPeakValue > pParam->fValue)
		{
			if((ReceiveParam_ad[CurrentChannelNo].fPlus - fStep) > MIN_PLUS)
				ReceiveParam_ad[CurrentChannelNo].fPlus -= fStep;
			else
				ReceiveParam_ad[CurrentChannelNo].fPlus = MIN_PLUS;
		}
		// 否则增基准增益
		else
		{
			if(ReceiveParam_ad[CurrentChannelNo].fPlus  + ReceiveParam_ad[CurrentChannelNo].fRefPlus + fStep > MAX_PLUS)
				ReceiveParam_ad[CurrentChannelNo].fPlus = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fRefPlus;
			else
				ReceiveParam_ad[CurrentChannelNo].fPlus += fStep;
		}
		float fCompare = (float)fabs(ReceiveParam_ad[CurrentChannelNo].fPlus  + ReceiveParam_ad[CurrentChannelNo].fRefPlus - MAX_PLUS);
		if((fabs(ReceiveParam_ad[CurrentChannelNo].fPlus - MIN_PLUS) < 1E-3 
			|| fCompare < 1E-3) && fabs(fPeakValue - pParam->fValue) > 1E-3)
		{
			DeleteCriticalSection(&csPeakInfo);

			memset(gszTextPeak, 0, sizeof(gszTextPeak));
			swprintf(gszTextPeak, _T("自动增益失败"));

			SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
			if((hWndTlAdjust != NULL) && (!gbHotAutoGainFlag))//当执行铁路专用的灵敏度设定进行自动增益时
				PostMessage(pParam->hWnd, WM_KEYDOWN, (WPARAM)'8', (LPARAM)0);
			else
				PostMessage(pParam->hWnd, WM_KEYDOWN, (WPARAM)'9', (LPARAM)0);
			
			return 0;
		}

		// 向下发值
		SetPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, FALSE, CurrentChannelNo);
		
		Sleep(50);
	}while(fabs(fPeakValue - pParam->fValue) > 1);// 波峰值和设定值误差大于1E-1

	DeleteCriticalSection(&csPeakInfo);

	SaveParamFile(lpDefaultParamFile);

	memset(gszTextPeak, 0, sizeof(gszTextPeak));
	swprintf(gszTextPeak, _T("自动增益完成"));

	SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	
	PostMessage(pParam->hWnd, WM_KEYDOWN, (WPARAM)'9', (LPARAM)0);

	return 0;
}
/**************************
*开始按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAutoGainPlusStartProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{	
					if(!gbHotAutoGainFlag)
					{
						ShowWindow(hWndQuantitativeAnalysis,SW_SHOW);
						SetFocus(currentQuantitativeAnalysisBtn);//上级菜单最后选中的按钮获得输入焦点

						DestroyWindow(hWndAutoGainPlus);
						hWndAutoGainPlus = 0;

						gCurrentFocusBtn = currentQuantitativeAnalysisBtn;

						gCurrentWnd = hWndQuantitativeAnalysis;
					}
					else
					{
						if(gCurrentWnd)//有子菜单窗口
						{
							ShowWindow(gCurrentWnd, SW_SHOW);
							SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
						}
						else //没有子菜单窗口
						{
							SetMenuStateFlag(FALSE);
							SetFocus(hWndMain);//主窗口获得输入焦点
						}

						DestroyWindow(hWndAutoGainPlus);
						hWndAutoGainPlus = 0;						
						
						gbHotAutoGainFlag = FALSE;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(autoGainPlusBtnList[i] != currentAutoGainPlusBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAutoGainPlusBtn = autoGainPlusBtnList[i];
					SetFocus(currentAutoGainPlusBtn);
					if(!gbHotAutoGainFlag)
						gCurrentFocusBtn = currentAutoGainPlusBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(autoGainPlusBtnList[i] != currentAutoGainPlusBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAutoGainPlusBtn = autoGainPlusBtnList[i];
					SetFocus(currentAutoGainPlusBtn);
					if(!gbHotAutoGainFlag)
						gCurrentFocusBtn = currentAutoGainPlusBtn;
				}
				else if(wParam == VK_RETURN)
				{
					SetFocus(NULL);
					//wangbingfu add start at 2010-12-27
					fOldPlus = ReceiveParam_ad[CurrentChannelNo].fPlus;
					//wangbingfu add start end 2010-12-27

					memset(gszTextPeak, 0, sizeof(gszTextPeak));
					swprintf(gszTextPeak, _T("增益进行中..."));

					SendMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
				
					// 关闭峰值记忆
					InitializeCriticalSection(&csPeakRemember);
					EnterCriticalSection(&csPeakRemember);

					gbPeakRememberFlag = FALSE;

					LeaveCriticalSection(&csPeakRemember);
					DeleteCriticalSection(&csPeakRemember);

					// 打开当前通道第一个闸门
					GateInfos[CurrentChannelNo][0].bOpen = TRUE;
					SetOpenFlag(NULL, GateInfos[CurrentChannelNo][0].bOpen, 1);
					stAutoGainThread_param.hWnd = Wnd;
					stAutoGainThread_param.fValue = fPeakSheDing;
					// 创建自动增益线程
					if(hDrawAutoGainThread == NULL)
					{
						// 创建线程		
						hDrawAutoGainThread = ::CreateThread(NULL, 0, autoGainProc, (LPVOID)&stAutoGainThread_param, 0, &dwDrawAutoGainThreadID);
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
					if(!gbHotAutoGainFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(gbHotAutoGainFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(!gbHotAutoGainFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '9')
				{
					if(currentAutoGainPlusBtn == hBtnAutoGainPlusStart)
					{
						SetFocus(currentAutoGainPlusBtn);
						SetPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);//wangbingfu add  at 2010-2-25增益完成后要保存,而失败也要刷新基准增益增到最大值
						if(!gWaveDrawDacFlag && !gWaveDrawAvgFlag)	//正在制作曲线
							gainChangeCurve(fOldPlus, TRUE); //wangbingfu add start at 2010-12-27
					}

					CloseHandle(hDrawAutoGainThread);
					hDrawAutoGainThread = NULL;
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
	return CallWindowProc(OldBtnAutoGainPlusStartProc,Wnd,Msg,wParam,lParam);
}



/**************************
*波高按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAutoGainPlusWaveHeightProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					if(!gbHotAutoGainFlag)
					{
						ShowWindow(hWndQuantitativeAnalysis,SW_SHOW);
						SetFocus(currentQuantitativeAnalysisBtn);//上级菜单最后选中的按钮获得输入焦点

						DestroyWindow(hWndAutoGainPlus);
						hWndAutoGainPlus = 0;

						gCurrentFocusBtn = currentQuantitativeAnalysisBtn;

						gCurrentWnd = hWndQuantitativeAnalysis;
					}
					else
					{
						if(gCurrentWnd)	//有子菜单窗口
						{
							ShowWindow(gCurrentWnd, SW_SHOW);
							SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
						}
						else	//没有子菜单窗口
						{
							SetMenuStateFlag(FALSE);
							SetFocus(hWndMain);//主窗口获得输入焦点
						}

						DestroyWindow(hWndAutoGainPlus);
						hWndAutoGainPlus = 0;					
						
						gbHotAutoGainFlag = FALSE;
					}
				}			
				else if(wParam == VK_UP)
				{
					while(autoGainPlusBtnList[i] != currentAutoGainPlusBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAutoGainPlusBtn = autoGainPlusBtnList[i];
					SetFocus(currentAutoGainPlusBtn);
					if(!gbHotAutoGainFlag)
						gCurrentFocusBtn = currentAutoGainPlusBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(autoGainPlusBtnList[i] != currentAutoGainPlusBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAutoGainPlusBtn = autoGainPlusBtnList[i];
					SetFocus(currentAutoGainPlusBtn);
					if(!gbHotAutoGainFlag)
						gCurrentFocusBtn = currentAutoGainPlusBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(fPeakSheDing > 0 && fPeakSheDing > PeakSheDingStep)
						fPeakSheDing -= PeakSheDingStep;
					else 
						fPeakSheDing = 0;		

					SetPeakSheDing(Wnd, fPeakSheDing);
				}
				else if(wParam == VK_RIGHT)
				{
					if((fPeakSheDing + PeakSheDingStep) < 100)
						fPeakSheDing += PeakSheDingStep;
					else 
						fPeakSheDing = 100;

					SetPeakSheDing(Wnd, fPeakSheDing);
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					if(!gbHotAutoGainFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(gbHotAutoGainFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(!gbHotAutoGainFlag)
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
				{}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAutoGainPlusWaveHeightProc,Wnd,Msg,wParam,lParam);
}

DWORD WINAPI autoRefGainProc(LPVOID lpParameter)
{
	THREAD_PARAM* pParam = (THREAD_PARAM*)lpParameter;
	InitializeCriticalSection(&csPeakInfo);

	float fPeakValue = 0;
	do
	{
		// 检查当前波峰值
		EnterCriticalSection(&csPeakInfo);
		fPeakValue = float(gPeakInfosRemeber[CurrentChannelNo][0].wDefectAmp) / MAX_HEIGHT*100.0f;
		LeaveCriticalSection(&csPeakInfo);

		float fStep = 0.3;
		double fCha = fabs(fPeakValue - pParam->fValue);
		if(fCha > 5.0f)
			fStep = 0.3f;
		else
			fStep = 0.05f;

		// 若大于设定值，减基准增益
		if(fPeakValue > pParam->fValue)
		{
			if((ReceiveParam_ad[CurrentChannelNo].fRefPlus - fStep) > MIN_PLUS)
				ReceiveParam_ad[CurrentChannelNo].fRefPlus -= fStep;
			else
				ReceiveParam_ad[CurrentChannelNo].fRefPlus = MIN_PLUS;
		}
		// 否则增基准增益
		else
		{
			ReceiveParam_ad[CurrentChannelNo].fRefPlus += fStep;
		}
		
		if(fabs(ReceiveParam_ad[CurrentChannelNo].fRefPlus) < 1E-3 
			|| ReceiveParam_ad[CurrentChannelNo].fPlus  + ReceiveParam_ad[CurrentChannelNo].fRefPlus > MAX_PLUS)
		{
			DeleteCriticalSection(&csPeakInfo);

			memset(gszTextPeak, 0, sizeof(gszTextPeak));
			swprintf(gszTextPeak, _T("失败"));

			SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);

			PostMessage(pParam->hWnd, WM_KEYDOWN, (WPARAM)'8', (LPARAM)0);

			return 0;
		}

		// 向下发值
		if((ReceiveParam_ad[CurrentChannelNo].fPlus  + ReceiveParam_ad[CurrentChannelNo].fRefPlus) > MAX_PLUS)
		{
			ReceiveParam_ad[CurrentChannelNo].fRefPlus = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus;
		}
		SetRefPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);
		
		Sleep(50);
	}while(fabs(fPeakValue - pParam->fValue) > 1);// 波峰值和设定值误差大于1E-1

	DeleteCriticalSection(&csPeakInfo);

	SaveParamFile(lpDefaultParamFile);

//	memset(gszTextPeak, 0, sizeof(gszTextPeak));
//	swprintf(gszTextPeak, _T("自动增益完成"));

	SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	
	PostMessage(pParam->hWnd, WM_KEYDOWN, (WPARAM)'9', (LPARAM)0);

	return 0;
}
