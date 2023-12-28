/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * �ο�������Դ�ļ�
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


HWND hWndReferenceWave = NULL;			//�ο������ھ��
HWND hBtnReferenceWaveSelect = NULL;	//�ο���ѡ��ť���
HWND hBtnReferenceWaveSave = NULL;		//���水ť���
HWND hBtnReferenceWaveOnOrOff = NULL;	//����/�رհ�ť���

HWND referenceWaveBtnList[3] ={NULL,NULL,NULL};					//�ο������ڵİ�ť��
WNDPROC OldBtnReferenceWaveSelectProc = NULL;	//�ο���ѡ��ťĬ�ϴ������
WNDPROC OldBtnReferenceWaveSaveProc = NULL;		//���水ťĬ�ϴ������
WNDPROC OldBtnReferenceWaveOnOrOffProc = NULL;	//����/�رհ�ťĬ�ϴ������

HWND currentReferenceWaveBtn = NULL;					//�ο������е�ǰѡ�еİ�ť

const WORD buttonNum = 3;		//��ť�ؼ���
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
extern HINSTANCE hInst;			//����ʵ�����

extern HWND hWndMain;
extern HWND hWndSpecialDisplay;
extern HWND currentSpecialDisplayBtn;
extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���

/**************************
*ע��ο�������
* @referenceWave    hInstance[in]:����ʵ�����
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
		DrawLableText(hBtnReferenceWaveOnOrOff, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	else
		DrawLableText(hBtnReferenceWaveOnOrOff, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
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
		
		DrawLableText(hWnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);

		// ��ȡnWaveNo��ŵĲο����ļ�
		HANDLE hFileWrite;	//�ļ����
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
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("���ļ�ʧ��"));
			return FALSE;
		}

		// fRangeStart, fRangeEnd
		InitializeCriticalSection(&csRefWaveRange);
		EnterCriticalSection(&csRefWaveRange);
		if(!ReadFile(hFileWrite,&gfRefWaveStart, sizeof(float),&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("���ļ�ʧ��"));

			CloseHandle(hFileWrite);
			return FALSE;
		}

		if(!ReadFile(hFileWrite,&gfRefWaveEnd, sizeof(float),&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("���ļ�ʧ��"));

			CloseHandle(hFileWrite);
			return FALSE;
		}

		LeaveCriticalSection(&csRefWaveRange);
		DeleteCriticalSection(&csRefWaveRange);

		// 512 data
		if(!ReadFile(hFileWrite,gRefWaveData, sizeof(gRefWaveData), &dwWriteDataSize, NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("���ļ�ʧ��"));
			
			CloseHandle(hFileWrite);
			return FALSE;
		}

		CloseHandle(hFileWrite);

		// ��ʾ��
		InitializeCriticalSection(&csRefWave);
		EnterCriticalSection(&csRefWave);
		
		g_bDisplayRefWave = TRUE;
		LeaveCriticalSection(&csRefWave);
		DeleteCriticalSection(&csRefWave);
	}
	else
	{
//		waveNoFlag[nWaveNo - 1] = FALSE;
		
		DrawLableText(hWnd, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);

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
*�ο������ڴ������
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
			//����ť�ؼ�
			hBtnReferenceWaveSelect = CreateWindow(_T("BUTTON"),_T("selectReferenceWave"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_REFERENCEWAVE_SELECT,hInst,NULL);
			hBtnReferenceWaveSave = CreateWindow(_T("BUTTON"),_T("saveReferenceWave"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_REFERENCEWAVE_SAVE,hInst,NULL);
			hBtnReferenceWaveOnOrOff = CreateWindow(_T("BUTTON"),_T("onOrOffReferenceWave"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_REFERENCEWAVE_ONOROFF,hInst,NULL);
					
			referenceWaveBtnList[0] = hBtnReferenceWaveSelect;
			referenceWaveBtnList[1] = hBtnReferenceWaveSave;
			referenceWaveBtnList[2] = hBtnReferenceWaveOnOrOff;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnReferenceWaveSelectProc = (WNDPROC) GetWindowLong(hBtnReferenceWaveSelect,GWL_WNDPROC);
			SetWindowLong(hBtnReferenceWaveSelect,GWL_WNDPROC,(LONG)btnReferenceWaveSelectProc);
			
			OldBtnReferenceWaveSaveProc = (WNDPROC) GetWindowLong(hBtnReferenceWaveSave,GWL_WNDPROC);
			SetWindowLong(hBtnReferenceWaveSave,GWL_WNDPROC,(LONG)btnReferenceWaveSaveProc);
			
			OldBtnReferenceWaveOnOrOffProc = (WNDPROC) GetWindowLong(hBtnReferenceWaveOnOrOff,GWL_WNDPROC);
			SetWindowLong(hBtnReferenceWaveOnOrOff,GWL_WNDPROC,(LONG)btnReferenceWaveOnOrOffProc);
			
		
			currentReferenceWaveBtn = referenceWaveBtnList[0];//Ĭ�ϵ�ǰѡ�вο���ѡ��ť
			SetFocus(currentReferenceWaveBtn);//ѡ�а�ť������뽹��
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
					DrawLableText(pdis->hwndItem, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
				else
					DrawLableText(pdis->hwndItem, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);

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
*���زο������ڰ�ťͼƬ
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
*�ο���ѡ��ť�Զ��崦�����
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
					SetFocus(currentSpecialDisplayBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

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
				else if(wParam == 'B')	// ����
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
*���水ť�Զ��崦�����
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
					SetFocus(currentSpecialDisplayBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

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
					HANDLE hFileWrite;	//�ļ����
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
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("�����ļ�ʧ��"));
						return -1;
					}
			

					// ������㡢�յ�
					InitializeCriticalSection(&csRange);
					EnterCriticalSection(&csRange);
					float fStart = fRangeStart[CurrentChannelNo];
					float fEnd = fRangeEnd[CurrentChannelNo]; 

					LeaveCriticalSection(&csRange);
					DeleteCriticalSection(&csRange);

					if(!WriteFile(hFileWrite,&fStart, sizeof(float), &dwWriteDataSize,NULL))
					{
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("д�ļ�ʧ��"));
						
						CloseHandle(hFileWrite);
						return -1;
					}

					if(!WriteFile(hFileWrite,&fEnd, sizeof(float), &dwWriteDataSize,NULL))
					{
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("д�ļ�ʧ��"));
						
						CloseHandle(hFileWrite);
						return -1;
					}

					InitializeCriticalSection(&csPicData);
					EnterCriticalSection(&csPicData);
					// 512�������� 
					if(!WriteFile(hFileWrite,&gPicData[CurrentChannelNo].data, sizeof(WORD)*DATA_SIZE, &dwWriteDataSize,NULL))
					{
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("д�ļ�ʧ��"));
						
						CloseHandle(hFileWrite);
						return -1;
					}

					LeaveCriticalSection(&csPicData);
					DeleteCriticalSection(&csPicData);

					CloseHandle(hFileWrite);

					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("�ļ�����ɹ�"));
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
				else if(wParam == 'B')	// ����
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
*����/�رհ�ť�Զ��崦�����
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
					SetFocus(currentSpecialDisplayBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

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
					
					// ����Ӧ��ŵĲο����ļ�
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
				else if(wParam == 'B')	// ����
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
