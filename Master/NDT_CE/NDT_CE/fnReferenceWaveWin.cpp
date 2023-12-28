/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 参考波窗口源文件
 * Filename: referenceWaveWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnReferenceWaveWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"


HWND hWndReferenceWave = NULL;			//参考波窗口句柄
HWND hBtnReferenceWaveSelect = NULL;	//参考波选择按钮句柄
HWND hBtnReferenceWaveSave = NULL;		//保存按钮句柄
HWND hBtnReferenceWaveOnOrOff = NULL;	//开启/关闭按钮句柄

HWND referenceWaveBtnList[3] ={NULL,NULL,NULL};					//参考波窗口的按钮表
WNDPROC OldBtnReferenceWaveSelectProc = NULL;	//参考波选择按钮默认处理过程
WNDPROC OldBtnReferenceWaveSaveProc = NULL;		//保存按钮默认处理过程
WNDPROC OldBtnReferenceWaveOnOrOffProc = NULL;	//开启/关闭按钮默认处理过程

HWND currentReferenceWaveBtn = NULL;					//参考波窗中当前选中的按钮

const WORD buttonNum = 3;		//按钮控件数
// songchenguang add start 2010-06-18
static WORD nWaveNo = 1;
//static BOOL bOpen = FALSE;
static TCHAR RefWaveFileName[MAX_PATH];
static BOOL waveNoFlag[10];		//wangbingfu add at 2010-8-25
WORD gRefWaveData[DATA_SIZE];

extern TCHAR FileDirectory[MAX_PATH];
extern PIC_DATA gPicData[2];
extern WORD CurrentChannelNo;
extern HWND hStatusBar;
extern CRITICAL_SECTION csRange;
extern float fRangeStart[2];
extern float fRangeEnd[2];

CRITICAL_SECTION csPicData;
CRITICAL_SECTION csRefWave;
CRITICAL_SECTION csRefWaveRange;

BOOL g_bDisplayRefWave = FALSE;
float gfRefWaveStart = 0.0f;
float gfRefWaveEnd = 0.0f;
// songchenguang add end 2010-06-18
extern HINSTANCE hInst;			//程序实例句柄

extern HWND hWndMain;
extern HWND hWndSpecialDisplay;
extern HWND currentSpecialDisplayBtn;
extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//当前显示的子窗口

/**************************
*注册参考波窗口
* @referenceWave    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerReferenceWaveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) referenceWaveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szReferenceWaveChild;

	return RegisterClass(&wc);
}

void SetWaveNo(HWND hWnd, const WORD& nWaveNo)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%d"), nWaveNo);
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);

	memset(strText, 0, sizeof(strText));

	//wangbingfu add start at 2010-8-25
	if(waveNoFlag[nWaveNo - 1])
		DrawLableText(hBtnReferenceWaveOnOrOff, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	else
		DrawLableText(hBtnReferenceWaveOnOrOff, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	//wangbingfu add end at 2010-8-25

	wchar_t strFileName[10];
	memset(strFileName, 0, sizeof(strFileName));

	wsprintf(strFileName, _T("%d.rw"), nWaveNo);

	memcpy(RefWaveFileName, FileDirectory, sizeof(RefWaveFileName));
	StringCchCat(RefWaveFileName, MAX_PATH, strFileName);
}

BOOL SetOpen(HWND hWnd, const WORD& nWaveNo, const BOOL& bOpen)
{
	if(bOpen)
	{
//		waveNoFlag[nWaveNo - 1] = TRUE;
		
		DrawLableText(hWnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);

		// 读取nWaveNo序号的参考波文件
		HANDLE hFileWrite;	//文件句柄
		DWORD dwWriteDataSize;

		TCHAR szFilePath[MAX_PATH];
		memset(szFilePath, 0, sizeof(szFilePath));
		
		wchar_t strFileName[10];
		memset(strFileName, 0, sizeof(strFileName));
		wsprintf(strFileName, _T("%d.rw"), nWaveNo);

		memcpy(szFilePath, FileDirectory, sizeof(szFilePath));
		StringCchCat(szFilePath, MAX_PATH, strFileName);

		hFileWrite = CreateFile(szFilePath,
								GENERIC_READ|GENERIC_WRITE,
								0,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY,
								NULL);
		if(hFileWrite == INVALID_HANDLE_VALUE)
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("打开文件失败"));
			return FALSE;
		}

		// fRangeStart, fRangeEnd
		InitializeCriticalSection(&csRefWaveRange);
		EnterCriticalSection(&csRefWaveRange);
		if(!ReadFile(hFileWrite,&gfRefWaveStart, sizeof(float),&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("读文件失败"));

			CloseHandle(hFileWrite);
			return FALSE;
		}

		if(!ReadFile(hFileWrite,&gfRefWaveEnd, sizeof(float),&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("读文件失败"));

			CloseHandle(hFileWrite);
			return FALSE;
		}

		LeaveCriticalSection(&csRefWaveRange);
		DeleteCriticalSection(&csRefWaveRange);

		// 512 data
		if(!ReadFile(hFileWrite,gRefWaveData, sizeof(gRefWaveData), &dwWriteDataSize, NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("读文件失败"));
			
			CloseHandle(hFileWrite);
			return FALSE;
		}

		CloseHandle(hFileWrite);

		// 显示波
		InitializeCriticalSection(&csRefWave);
		EnterCriticalSection(&csRefWave);
		
		g_bDisplayRefWave = TRUE;
		LeaveCriticalSection(&csRefWave);
		DeleteCriticalSection(&csRefWave);
	}
	else
	{
//		waveNoFlag[nWaveNo - 1] = FALSE;
		
		DrawLableText(hWnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);

		InitializeCriticalSection(&csRefWave);
		EnterCriticalSection(&csRefWave);
		
		g_bDisplayRefWave = FALSE;
		LeaveCriticalSection(&csRefWave);
		DeleteCriticalSection(&csRefWave);

		SetRedrawFlag(TRUE);
	}

	return TRUE;
}
/**************************
*参考波窗口处理过程
* @referenceWave    hInstance[in]:
* @referenceWave		message[in]:
* @referenceWave		wParam[in]:
* @referenceWave		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK referenceWaveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			//画按钮控件
			hBtnReferenceWaveSelect = CreateWindow(_T("BUTTON"),_T("selectReferenceWave"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_REFERENCEWAVE_SELECT,hInst,NULL);
			hBtnReferenceWaveSave = CreateWindow(_T("BUTTON"),_T("saveReferenceWave"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_REFERENCEWAVE_SAVE,hInst,NULL);
			hBtnReferenceWaveOnOrOff = CreateWindow(_T("BUTTON"),_T("onOrOffReferenceWave"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_REFERENCEWAVE_ONOROFF,hInst,NULL);
					
			referenceWaveBtnList[0] = hBtnReferenceWaveSelect;
			referenceWaveBtnList[1] = hBtnReferenceWaveSave;
			referenceWaveBtnList[2] = hBtnReferenceWaveOnOrOff;
			
			//按钮控件的自定义处理过程
			OldBtnReferenceWaveSelectProc = (WNDPROC) GetWindowLong(hBtnReferenceWaveSelect,GWL_WNDPROC);
			SetWindowLong(hBtnReferenceWaveSelect,GWL_WNDPROC,(LONG)btnReferenceWaveSelectProc);
			
			OldBtnReferenceWaveSaveProc = (WNDPROC) GetWindowLong(hBtnReferenceWaveSave,GWL_WNDPROC);
			SetWindowLong(hBtnReferenceWaveSave,GWL_WNDPROC,(LONG)btnReferenceWaveSaveProc);
			
			OldBtnReferenceWaveOnOrOffProc = (WNDPROC) GetWindowLong(hBtnReferenceWaveOnOrOff,GWL_WNDPROC);
			SetWindowLong(hBtnReferenceWaveOnOrOff,GWL_WNDPROC,(LONG)btnReferenceWaveOnOrOffProc);
			
		
			currentReferenceWaveBtn = referenceWaveBtnList[0];//默认当前选中参考波选择按钮
			SetFocus(currentReferenceWaveBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentReferenceWaveBtn;

			memset(gRefWaveData, 0, sizeof(gRefWaveData));
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
			referenceWave_DrawButtonBitmap(hInst,pdis);

			// songchenguang add start 2010-07-02
			if(pdis->hwndItem == hBtnReferenceWaveSelect)
			{
				SetWaveNo(hBtnReferenceWaveSelect, nWaveNo);
			}
			else if(pdis->hwndItem == hBtnReferenceWaveOnOrOff)
			{
//				SetOpen(hBtnReferenceWaveOnOrOff, nWaveNo, waveNoFlag[nWaveNo - 1]);
				if(waveNoFlag[nWaveNo - 1])			
					DrawLableText(pdis->hwndItem, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
				else
					DrawLableText(pdis->hwndItem, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);

			}
			// songchenguang add end 2010-07-02
			break;
		case WM_DESTROY:
			// close ref wave
			InitializeCriticalSection(&csRefWave);
			EnterCriticalSection(&csRefWave);
			
			g_bDisplayRefWave = FALSE;
			LeaveCriticalSection(&csRefWave);
			DeleteCriticalSection(&csRefWave);

			SetRedrawFlag(TRUE);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载参考波窗口按钮图片
* @referenceWave    hInstance[in]:
* @referenceWave		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int referenceWave_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_referenceWaveBtnBitmap[]=
	{
		{IDC_REFERENCEWAVE_SELECT	,IDB_BITMAP_CANKAOBOXUANZE_NOSEL_R	 ,  IDB_BITMAP_CANKAOBOXUANZE_SELECT_R		},
		{IDC_REFERENCEWAVE_SAVE	,IDB_BITMAP_BAOCUN_NOSEL_R	 ,  IDB_BITMAP_BAOCUN_SELECT_R		},
		{IDC_REFERENCEWAVE_ONOROFF	,IDB_BITMAP_KAIQIGUANBI4_NOSEL_R ,  IDB_BITMAP_KAIQIGUANBI4_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_referenceWaveBtnBitmap);
	return ReturnVal;
}



/**************************
*参考波选择按钮自定义处理过程
* @referenceWave    hInstance[in]:
* @referenceWave		message[in]:
* @referenceWave		wParam[in]:
* @referenceWave		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnReferenceWaveSelectProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndReferenceWave);
					hWndReferenceWave = 0;
					
					gCurrentFocusBtn = currentSpecialDisplayBtn;

					gCurrentWnd = hWndSpecialDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(referenceWaveBtnList[i] != currentReferenceWaveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentReferenceWaveBtn = referenceWaveBtnList[i];
					SetFocus(currentReferenceWaveBtn);
					gCurrentFocusBtn = currentReferenceWaveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(referenceWaveBtnList[i] != currentReferenceWaveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentReferenceWaveBtn = referenceWaveBtnList[i];
					SetFocus(currentReferenceWaveBtn);
					gCurrentFocusBtn = currentReferenceWaveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					// songchenguang add start 2010-06-18
					if(nWaveNo > 1)
						nWaveNo--;
					else
						nWaveNo = 10;

					SetWaveNo(hWnd, nWaveNo);
					// songchenguang add end 2010-06-18
				}
				else if(wParam == VK_RIGHT)
				{
					// songchenguang add start 2010-06-18
					if(nWaveNo < 10)
						nWaveNo++;
					else
						nWaveNo = 1;

					SetWaveNo(hWnd, nWaveNo);
					// songchenguang add end 2010-06-18
				}
				else if(wParam == VK_RETURN)
				{
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
	return CallWindowProc(OldBtnReferenceWaveSelectProc,hWnd,Msg,wParam,lParam);
}
/**************************
*保存按钮自定义处理过程
* @referenceWave    hInstance[in]:
* @referenceWave		message[in]:
* @referenceWave		wParam[in]:
* @referenceWave		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnReferenceWaveSaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndReferenceWave);
					hWndReferenceWave = 0;
					
					gCurrentFocusBtn = currentSpecialDisplayBtn;

					gCurrentWnd = hWndSpecialDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(referenceWaveBtnList[i] != currentReferenceWaveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentReferenceWaveBtn = referenceWaveBtnList[i];
					SetFocus(currentReferenceWaveBtn);
					gCurrentFocusBtn = currentReferenceWaveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(referenceWaveBtnList[i] != currentReferenceWaveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentReferenceWaveBtn = referenceWaveBtnList[i];
					SetFocus(currentReferenceWaveBtn);
					gCurrentFocusBtn = currentReferenceWaveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					HANDLE hFileWrite;	//文件句柄
					DWORD dwWriteDataSize;
					hFileWrite = CreateFile(RefWaveFileName,
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
			

					// 声程起点、终点
					InitializeCriticalSection(&csRange);
					EnterCriticalSection(&csRange);
					float fStart = fRangeStart[CurrentChannelNo];
					float fEnd = fRangeEnd[CurrentChannelNo]; 

					LeaveCriticalSection(&csRange);
					DeleteCriticalSection(&csRange);

					if(!WriteFile(hFileWrite,&fStart, sizeof(float), &dwWriteDataSize,NULL))
					{
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写文件失败"));
						
						CloseHandle(hFileWrite);
						return -1;
					}

					if(!WriteFile(hFileWrite,&fEnd, sizeof(float), &dwWriteDataSize,NULL))
					{
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写文件失败"));
						
						CloseHandle(hFileWrite);
						return -1;
					}

					InitializeCriticalSection(&csPicData);
					EnterCriticalSection(&csPicData);
					// 512个点数据 
					if(!WriteFile(hFileWrite,&gPicData[CurrentChannelNo].data, sizeof(WORD)*DATA_SIZE, &dwWriteDataSize,NULL))
					{
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写文件失败"));
						
						CloseHandle(hFileWrite);
						return -1;
					}

					LeaveCriticalSection(&csPicData);
					DeleteCriticalSection(&csPicData);

					CloseHandle(hFileWrite);

					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("文件保存成功"));
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
	return CallWindowProc(OldBtnReferenceWaveSaveProc,Wnd,Msg,wParam,lParam);
}


/**************************
*开启/关闭按钮自定义处理过程
* @referenceWave    hInstance[in]:
* @referenceWave		message[in]:
* @referenceWave		wParam[in]:
* @referenceWave		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnReferenceWaveOnOrOffProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndReferenceWave);
					hWndReferenceWave = 0;
					
					gCurrentFocusBtn = currentSpecialDisplayBtn;

					gCurrentWnd = hWndSpecialDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(referenceWaveBtnList[i] != currentReferenceWaveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentReferenceWaveBtn = referenceWaveBtnList[i];
					SetFocus(currentReferenceWaveBtn);
					gCurrentFocusBtn = currentReferenceWaveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(referenceWaveBtnList[i] != currentReferenceWaveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentReferenceWaveBtn = referenceWaveBtnList[i];
					SetFocus(currentReferenceWaveBtn);
					gCurrentFocusBtn = currentReferenceWaveBtn;
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					waveNoFlag[nWaveNo - 1] = !waveNoFlag[nWaveNo - 1];

					for(int i=0; i<10; i++)
					{
						if(i != (nWaveNo - 1))
							waveNoFlag[i] = FALSE;
					}
					
					// 打开相应编号的参考波文件
					SetOpen(hWnd, nWaveNo, waveNoFlag[nWaveNo - 1]);

					SetRedrawFlag(TRUE);
				}
				else if(wParam == VK_RETURN)
				{
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
	return CallWindowProc(OldBtnReferenceWaveOnOrOffProc,hWnd,Msg,wParam,lParam);
}
