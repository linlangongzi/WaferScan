/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ¼�񴰿�Դ�ļ�
 * Filename: fnVideoRecordWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnVideoRecordWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"

HWND hWndVideoRecord = NULL;					//¼�񴰿ھ��
HWND hBtnVideoRecordFileNumber = NULL;			//�ļ���Ű�ť���
HWND hBtnVideoRecordRecordOnOff = NULL;		//¼�񿪹ذ�ť���
HWND hBtnVideoRecordPlay = NULL;				//�طŰ�ť���

HWND videoRecordBtnList[3] = {NULL,NULL,NULL};	//¼�񴰿ڵİ�ť��
WNDPROC OldBtnVideoRecordFileNumberProc = NULL;		//�ļ���Ű�ťĬ�ϴ������
WNDPROC OldBtnVideoRecordRecordOnOffProc = NULL;	//¼�񿪹ذ�ťĬ�ϴ������
WNDPROC OldBtnVideoRecordPlayProc = NULL;			//�طŰ�ťĬ�ϴ������

HWND currentVideoRecordBtn = NULL;						//¼���е�ǰѡ�еİ�ť
VIDEO_INFO stVideoInfo;							//¼�񻺴�
BOOL gbReplayFlag = OFF;						//�Ƿ��п����ط�
static BOOL bRecordFlag[VIDEO_NUM];				//¼���־
static BOOL bReplayFlag[VIDEO_NUM];				//�طű�־

static const int buttonNum = 3;						//��ť�ؼ���
static WORD wRecordIndex = 0;					//¼������
static WORD wVideoDataNum = 0;					//�洢����
static DWORD64 qwMaxRecordNum = 0;				//���ɴ洢����
static DWORD dwPlayDataNum = 0;					//��������
static DWORD dwPlayedDataNum = 0;					//�Ѳ�������
static TCHAR szVideoFileName[MAX_PATH];
static HANDLE hFileWrite;	//д�ļ����
static HANDLE hFileRead;	//���ļ����
//static LARGE_INTEGER liFileSize;	//�ļ���С
static DWORD dwFileSize;	//�ļ���С
static WORKMODE emOldWorkMode;
static WORD wOldChannel;
static float fOldBasePlus;
static float fOldRefPlus;
static float fOldRangeStart;
static float fOldRangeEnd;
static DWORD64 qwFreeBytesToCaller;			//�ɻ�ÿ��пռ�
extern HINSTANCE hInst;				//����ʵ�����
extern HWND hWndMain;
extern HWND hWndFnMenu;
extern HWND currentFnMenuBtn;
extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;			//��ǰ��ʾ���Ӵ���
extern HWND hStatusBar;
extern WORD gwStoreType;			//�洢����
extern WORD CurrentChannelNo;		//��ǰ����ͨ��
extern PIC_DATA gPicData[2];		// ͼ������
extern HANDLE hDrawEvent;			// ��ͼ�¼�
extern HANDLE hWorkEvent;			// �����¼�
extern CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// �����̲߳���
extern BOOL gbGuanLian;
extern ARS200FPGA_SYS_REG* sys_reg;
extern SYS_INFO gSysInfo;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern wchar_t gszTextChannel[10];
extern wchar_t gszTextPlus[10];
extern wchar_t gszTextRefPlus[10];
extern TCHAR FileDirectory[MAX_PATH];
extern float fRangeStart[2];
extern float fRangeEnd[2];
extern wchar_t gszInfo[2][2][30];
/*
BOOL ReadVideoFile(LPCTSTR lpszPathName)
{				
	HANDLE hFileRead;	//�ļ����
	DWORD dwWriteDataSize;
	hFileRead = CreateFile(lpszPathName,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY,
							NULL);
	if(hFileRead == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��video�ļ�ʧ��"));
		return FALSE;
	}

	if(!ReadFile(hFileRead,&stVideoInfo, sizeof(stVideoInfo),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��video�ļ�ʧ��"));
		
		CloseHandle(hFileRead);
		return FALSE;
	}

	CloseHandle(hFileRead);

	return TRUE;
}

BOOL SaveVideoFile(LPCTSTR lpszPathName)
{	
	HANDLE hFileWrite;	//�ļ����
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
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("����video�ļ�ʧ��"));
		return -1;
	}

	if(!WriteFile(hFileWrite,&stVideoInfo, sizeof(stVideoInfo),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("дvideo�ļ�ʧ��"));

		CloseHandle(hFileWrite);
		return FALSE;
	}

	CloseHandle(hFileWrite);

	return TRUE;	
}
*/
/**************************
*¼��ʱ�������
***************************/
void CALLBACK videoRecordProc( HWND hwnd, 
							  UINT uMsg, 
							  UINT idEvent, 
							  DWORD dwTime 
							)
{
	DWORD dwWriteDataSize;

	if(gwStoreType == 2)	//����
	{
		if(wVideoDataNum < qwMaxRecordNum)
		{		
			for(int i = 0; i < DATA_SIZE; i++)
			{
			/*	if(sys_reg->wWorkMode == SingleChannelB || sys_reg->wWorkMode == SingleChannelA || sys_reg->wWorkMode == OneSendOneReceive)
					stVideoInfo.stData.wPicData[CurrentChannelNo][i] = gPicData[CurrentChannelNo].data[i];
				else
				{
					stVideoInfo.stData.wPicData[0][i] = gPicData[0].data[i];
					stVideoInfo.stData.wPicData[1][i] = gPicData[1].data[i];
				}*/
				stVideoInfo.stData.wPicData[0][i] = gPicData[0].data[i];
				stVideoInfo.stData.wPicData[1][i] = gPicData[1].data[i];
			}
			stVideoInfo.wWorkMode = gSysInfo.wWorkMode;
			stVideoInfo.wCurrentChannel = CurrentChannelNo;
			stVideoInfo.fBasePlus = ReceiveParam_ad[CurrentChannelNo].fPlus;
			stVideoInfo.fRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
			stVideoInfo.fRangeStart = fRangeStart[CurrentChannelNo];
			stVideoInfo.fRangeEnd = fRangeEnd[CurrentChannelNo];
				
			wVideoDataNum++;

			if(!WriteFile(hFileWrite,&stVideoInfo, sizeof(stVideoInfo),&dwWriteDataSize,NULL))
			{
				PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("дvideo�ļ�ʧ��"));

				CloseHandle(hFileWrite);

			//	stVideoInfo.dwDataNum = wVideoDataNum;
				SendMessage(hwnd, WM_KEYDOWN, (WPARAM)'9', (LPARAM)0);
			}
		}
		else
		{
		//	stVideoInfo.dwDataNum = wVideoDataNum;
			SendMessage(hwnd, WM_KEYDOWN, (WPARAM)'9', (LPARAM)0);
		}
	}
	else
	{
		for(int i = 0; i < DATA_SIZE; i++)
		{
		/*	if(sys_reg->wWorkMode == SingleChannelB || sys_reg->wWorkMode == SingleChannelA || sys_reg->wWorkMode == OneSendOneReceive)
				stVideoInfo.stData.wPicData[CurrentChannelNo][i] = gPicData[CurrentChannelNo].data[i];
			else
			{
				stVideoInfo.stData.wPicData[0][i] = gPicData[0].data[i];
				stVideoInfo.stData.wPicData[1][i] = gPicData[1].data[i];
			}*/
			stVideoInfo.stData.wPicData[0][i] = gPicData[0].data[i];
			stVideoInfo.stData.wPicData[1][i] = gPicData[1].data[i];
		}

		if(!WriteFile(hFileWrite,&stVideoInfo, sizeof(stVideoInfo),&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("дvideo�ļ�ʧ��"));

			CloseHandle(hFileWrite);

		//	stVideoInfo.dwDataNum = wVideoDataNum;
			SendMessage(hwnd, WM_KEYDOWN, (WPARAM)'9', (LPARAM)0);
		}
	}
}

/**************************
*���Ŷ�ʱ�������
***************************/

void CALLBACK videoReplayProc( HWND hwnd, 
							  UINT uMsg, 
							  UINT idEvent, 
							  DWORD dwTime 
							)
{
	DWORD dwWriteDataSize;
	
 	if(dwPlayedDataNum < dwPlayDataNum)
    {
    	if(!ReadFile(hFileRead,&stVideoInfo, sizeof(stVideoInfo),&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��video�ļ�ʧ��"));
			
			SendMessage(hwnd, WM_KEYDOWN, (WPARAM)'8', (LPARAM)0);
		}
		
		for(int i = 0; i < DATA_SIZE; i++)
		{
		/*	if(sys_reg->wWorkMode == SingleChannelB || sys_reg->wWorkMode == SingleChannelA || sys_reg->wWorkMode == OneSendOneReceive)
			{
				gPicData[CurrentChannelNo].data[i] = stVideoInfo.stData.wPicData[CurrentChannelNo][i];
				gPicData[CurrentChannelNo].bDrawed = FALSE;
			}
			else
			{
				gPicData[0].data[i] = stVideoInfo.stData.wPicData[0][i];
				gPicData[1].data[i] = stVideoInfo.stData.wPicData[1][i];
				gPicData[0].bDrawed = FALSE;
				gPicData[1].bDrawed = FALSE;
			}*/
			gPicData[0].data[i] = stVideoInfo.stData.wPicData[0][i];
			gPicData[1].data[i] = stVideoInfo.stData.wPicData[1][i];
			
		}
		if((WORKMODE)stVideoInfo.wWorkMode == SingleChannelA || (WORKMODE)stVideoInfo.wWorkMode == SingleChannelB
				|| (WORKMODE)stVideoInfo.wWorkMode == OneSendOneReceive)// ��ͨ��A����ͨ��B��һ��һ��
		{
			gPicData[stVideoInfo.wCurrentChannel].bDrawed = FALSE;
		}
		else	//˫ͨ��, ��������, ˫������
		{
			gPicData[0].bDrawed = FALSE;
			gPicData[1].bDrawed = FALSE;
		}
		if(dwPlayedDataNum == 0)
		{
			gSysInfo.wWorkMode = (WORKMODE)stVideoInfo.wWorkMode;
			CurrentChannelNo = stVideoInfo.wCurrentChannel;
			fOldBasePlus = stVideoInfo.fBasePlus;
			fOldRefPlus = stVideoInfo.fRefPlus;

			PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)gSysInfo.wWorkMode);

			if(CurrentChannelNo == 0)
			{
				swprintf(gszTextChannel, _T("1"));
			}
			else if(CurrentChannelNo == 1)
			{
				swprintf(gszTextChannel, _T("2"));
			}

			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

			memset(gszTextPlus, 0, sizeof(gszTextPlus));
			swprintf(gszTextPlus, _T("%.1f"), fOldBasePlus);

			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

			memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
			swprintf(gszTextRefPlus, _T("%.1f"), fOldRefPlus);
		
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
		}
		
		if(gSysInfo.wWorkMode != (WORKMODE)stVideoInfo.wWorkMode)
		{
			SetRedrawFlag(TRUE);		//����ǰһͨ���Ĳ���
			gSysInfo.wWorkMode = (WORKMODE)stVideoInfo.wWorkMode;
			PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)gSysInfo.wWorkMode);
		}
		if(CurrentChannelNo != stVideoInfo.wCurrentChannel)
		{
			CurrentChannelNo = stVideoInfo.wCurrentChannel;
			if(CurrentChannelNo == 0)
			{
				swprintf(gszTextChannel, _T("1"));
			}
			else if(CurrentChannelNo == 1)
			{
				swprintf(gszTextChannel, _T("2"));
			}

			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
		}
		if(fOldBasePlus != stVideoInfo.fBasePlus)
		{
			fOldBasePlus = stVideoInfo.fBasePlus;
			memset(gszTextPlus, 0, sizeof(gszTextPlus));
			swprintf(gszTextPlus, _T("%.1f"), fOldBasePlus);

			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);
		}
		if(fOldRefPlus != stVideoInfo.fRefPlus)
		{
			fOldRefPlus = stVideoInfo.fRefPlus;
			memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
			swprintf(gszTextRefPlus, _T("%.1f"), fOldRefPlus);
		
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
		}
		fRangeStart[CurrentChannelNo] = stVideoInfo.fRangeStart;
		fRangeEnd[CurrentChannelNo] = stVideoInfo.fRangeEnd;
		SetEvent(hDrawEvent);
		dwPlayedDataNum++;
    }
	else
	{
		SendMessage(hwnd, WM_KEYDOWN, (WPARAM)'8', (LPARAM)0);
	}
	// ������	
	//DrawRectDashLine(RctWave, RGBtoRGB565(SysColor[nColorIndex].clrWaveBack));
}

/**************************
*ע��¼�񴰿�
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerVideoRecordChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) videoRecordChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szVideoRecordChild;

	return RegisterClass(&wc);
}
static void displayRecordStatus(HWND hWnd, BOOL bStatus)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(bStatus == 0)
	{
		DrawLableText(hWnd, _T("ֹͣ"), DEFAULT_LABLE_TEXT_HEIGHT, 
					STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else
	{
		DrawLableText(hWnd, _T("¼��"), DEFAULT_LABLE_TEXT_HEIGHT, 
					STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
}

static void displayReplayStatus(HWND hWnd, BOOL bStatus)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(bStatus == 0)
	{
		DrawLableText(hWnd, _T("ֹͣ"), DEFAULT_LABLE_TEXT_HEIGHT, 
					STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else
	{
		DrawLableText(hWnd, _T("�ط�"), DEFAULT_LABLE_TEXT_HEIGHT, 
					STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
}


static void displayRecordIndex(HWND hWnd, WORD wIndex)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	if(gwStoreType == 2)	//����
		DrawLableText(hWnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	else
	{
		swprintf(szVideoFileName, _T("%s%d.video"), FileDirectory, wIndex + 1);
		if(GetFileAttributes(szVideoFileName)  == 0xFFFFFFFF)				
			swprintf(strText, _T("*%d"), wIndex + 1);
		else
			swprintf(strText, _T("%d"), wIndex + 1);

		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, 
						STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	
	displayRecordStatus(hBtnVideoRecordRecordOnOff, bRecordFlag[wIndex]);
	displayReplayStatus(hBtnVideoRecordPlay, bReplayFlag[wIndex]);
}

/**************************
*¼�񴰿ڴ������
* @videoRecord    hInstance[in]:
* @videoRecord		message[in]:
* @videoRecord		wParam[in]:
* @videoRecord		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK videoRecordChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			gbReplayFlag = OFF;
			for(int i = 0; i < VIDEO_NUM; i++)
			{	
				bRecordFlag[i] = OFF;
				bReplayFlag[i] = OFF;
			}
			if(gwStoreType == 2)	//����
				memcpy(szVideoFileName, _T("\\ResidentFlash\\video.dat"), sizeof(szVideoFileName));
				
			else
				swprintf(szVideoFileName, _T("%s%d.video"), FileDirectory, wRecordIndex + 1);
			//����ť�ؼ�
			hBtnVideoRecordFileNumber = CreateWindow(_T("BUTTON"),_T("FileNumberVideoRecord"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_VIDEORECORD_FILENUMBER,hInst,NULL);
			hBtnVideoRecordRecordOnOff = CreateWindow(_T("BUTTON"),_T("recordOnOffVideoRecord"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_VIDEORECORD_RECORDONOFF,hInst,NULL);
			hBtnVideoRecordPlay = CreateWindow(_T("BUTTON"),_T("playVideoRecord"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_VIDEORECORD_PLAY,hInst,NULL);
					
			videoRecordBtnList[0] = hBtnVideoRecordFileNumber;
			videoRecordBtnList[1] = hBtnVideoRecordRecordOnOff;
			videoRecordBtnList[2] = hBtnVideoRecordPlay;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnVideoRecordFileNumberProc = (WNDPROC) GetWindowLong(hBtnVideoRecordFileNumber,GWL_WNDPROC);
			SetWindowLong(hBtnVideoRecordFileNumber,GWL_WNDPROC,(LONG)btnVideoRecordFileNumberProc);
			
			OldBtnVideoRecordRecordOnOffProc = (WNDPROC) GetWindowLong(hBtnVideoRecordRecordOnOff,GWL_WNDPROC);
			SetWindowLong(hBtnVideoRecordRecordOnOff,GWL_WNDPROC,(LONG)btnVideoRecordRecordOnOffProc);
			
			OldBtnVideoRecordPlayProc = (WNDPROC) GetWindowLong(hBtnVideoRecordPlay,GWL_WNDPROC);
			SetWindowLong(hBtnVideoRecordPlay,GWL_WNDPROC,(LONG)btnVideoRecordPlayProc);
			
			currentVideoRecordBtn = videoRecordBtnList[0];//Ĭ�ϵ�ǰѡ���ļ���Ű�ť
			SetFocus(currentVideoRecordBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentVideoRecordBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == '9')
			{
				KillTimer(hWnd, TIMER_RECORD);
				
			    CloseHandle(hFileWrite);

				bRecordFlag[wRecordIndex] = OFF;
				displayRecordStatus(hBtnVideoRecordRecordOnOff, bRecordFlag[wRecordIndex]);

				PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("�ռ�����"));				
			}
			else if(wParam == '8')
			{
				KillTimer(hWnd, TIMER_REPLAY);
				SetRedrawFlag(TRUE);//ˢ��
				gbReplayFlag = OFF;				
				
				fRangeStart[CurrentChannelNo] = fOldRangeStart;
				fRangeEnd[CurrentChannelNo] = fOldRangeEnd;
				CurrentChannelNo = wOldChannel;
				if(CurrentChannelNo == 0)
				{
					swprintf(gszTextChannel, _T("1"));
				}
				else if(CurrentChannelNo == 1)
				{
					swprintf(gszTextChannel, _T("2"));
				}
				PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

				gSysInfo.wWorkMode = emOldWorkMode;
				PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)gSysInfo.wWorkMode);

				memset(gszTextPlus, 0, sizeof(gszTextPlus));
				swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

				PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

				memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
				swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

				PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
							
				CloseHandle(hFileRead);

				SetEvent(hWorkEvent);
				bReplayFlag[wRecordIndex] = OFF;
				displayReplayStatus(hBtnVideoRecordPlay, bReplayFlag[wRecordIndex]);								
			}
			else
			{
				;//
			}
			break;
		case WM_COMMAND:
			
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			videoRecord_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnVideoRecordFileNumber)
			{				
				displayRecordIndex(pdis->hwndItem, wRecordIndex);
			}

			else if(pdis->hwndItem == hBtnVideoRecordRecordOnOff)
			{				
				displayRecordStatus(pdis->hwndItem, bRecordFlag[wRecordIndex]);
			}

			else if(pdis->hwndItem == hBtnVideoRecordPlay)
			{				
				displayReplayStatus(pdis->hwndItem, bReplayFlag[wRecordIndex]);
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
*����¼�񴰿ڰ�ťͼƬ
* @videoRecord    hInstance[in]:
* @videoRecord		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int videoRecord_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_videoRecordBtnBitmap[]=
	{
		{IDC_VIDEORECORD_FILENUMBER	,IDB_BITMAP_WENJIANXUHAO_NOSEL_R	 ,  IDB_BITMAP_WENJIANXUHAO_SELECT_R		},
		{IDC_VIDEORECORD_RECORDONOFF	,IDB_BITMAP_LUXIANGKAIGUAN_NOSEL_R	 ,  IDB_BITMAP_LUXIANGKAIGUAN_SELECT_R		},
		{IDC_VIDEORECORD_PLAY	,IDB_BITMAP_HUIFANG_NOSEL_R ,  IDB_BITMAP_HUIFANG_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_videoRecordBtnBitmap);
	return ReturnVal;
}
/**************************
*�ļ���Ű�ť�Զ��崦�����
* @videoRecord    hInstance[in]:
* @videoRecord		message[in]:
* @videoRecord		wParam[in]:
* @videoRecord		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnVideoRecordFileNumberProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndVideoRecord);
					hWndVideoRecord = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;

					for(int i = 0; i < VIDEO_NUM; i++)
					{	
						if(bReplayFlag[i] == ON)
						{
							KillTimer(hWndVideoRecord, TIMER_REPLAY);
							SetRedrawFlag(TRUE);//ˢ��
							gbReplayFlag = OFF;				
							
							fRangeStart[CurrentChannelNo] = fOldRangeStart;
							fRangeEnd[CurrentChannelNo] = fOldRangeEnd;
							CurrentChannelNo = wOldChannel;
							if(CurrentChannelNo == 0)
							{
								swprintf(gszTextChannel, _T("1"));
							}
							else if(CurrentChannelNo == 1)
							{
								swprintf(gszTextChannel, _T("2"));
							}
							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

							gSysInfo.wWorkMode = emOldWorkMode;
							PostMessage(hStatusBar, SB_SETTEXT, 
									(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)gSysInfo.wWorkMode);

							memset(gszTextPlus, 0, sizeof(gszTextPlus));
							swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

							memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
							swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
										
							CloseHandle(hFileRead);

							SetEvent(hWorkEvent);
						}
					}
								
					for(int i = 0; i < VIDEO_NUM; i++)
					{	
						bRecordFlag[i] = OFF;
						bReplayFlag[i] = OFF;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(videoRecordBtnList[i] != currentVideoRecordBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentVideoRecordBtn = videoRecordBtnList[i];
					SetFocus(currentVideoRecordBtn);
					gCurrentFocusBtn = currentVideoRecordBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(videoRecordBtnList[i] != currentVideoRecordBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentVideoRecordBtn = videoRecordBtnList[i];
					SetFocus(currentVideoRecordBtn);
					gCurrentFocusBtn = currentVideoRecordBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(gwStoreType != 2)
					{
						if(wRecordIndex > 0)
							wRecordIndex--;
						else if(wRecordIndex == 0)
							wRecordIndex = VIDEO_NUM - 1;
						
						for(int i = 0; i < VIDEO_NUM; i++)
						{	
							if(bReplayFlag[i] == ON)
							{
								KillTimer(hWndVideoRecord, TIMER_REPLAY);
								SetRedrawFlag(TRUE);//ˢ��
								gbReplayFlag = OFF;
								fRangeStart[CurrentChannelNo] = fOldRangeStart;
								fRangeEnd[CurrentChannelNo] = fOldRangeEnd;
								CurrentChannelNo = wOldChannel;
								if(emOldWorkMode == DanJingGuanLian || emOldWorkMode == ShuangJingGuanLian)
								{
									PostMessage(hStatusBar, SB_SETTEXT, 
									(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T(""));
								}
								else
								{
									if(CurrentChannelNo == 0)
									{
										swprintf(gszTextChannel, _T("1"));
									}
									else if(CurrentChannelNo == 1)
									{
										swprintf(gszTextChannel, _T("2"));
									}
									PostMessage(hStatusBar, SB_SETTEXT, 
										(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
								}

								gSysInfo.wWorkMode = emOldWorkMode;
								PostMessage(hStatusBar, SB_SETTEXT, 
										(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)gSysInfo.wWorkMode);
								
								SetEvent(hWorkEvent);
								
								CloseHandle(hFileRead);
							}
						}
						
						for(int i = 0; i < VIDEO_NUM; i++)
						{	
							bRecordFlag[i] = OFF;
							bReplayFlag[i] = OFF;
						}

						displayRecordIndex(Wnd, wRecordIndex);						
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(gwStoreType != 2)
					{
						if(gwStoreType != 2)
						{
							if(wRecordIndex < VIDEO_NUM - 1)
								wRecordIndex++;
							else if(wRecordIndex == VIDEO_NUM - 1)
								wRecordIndex = 0;

							for(int i = 0; i < VIDEO_NUM; i++)
							{	
								if(bReplayFlag[i] == ON)
								{
									KillTimer(hWndVideoRecord, TIMER_REPLAY);
									SetRedrawFlag(TRUE);//ˢ��
									gbReplayFlag = OFF;
									fRangeStart[CurrentChannelNo] = fOldRangeStart;
									fRangeEnd[CurrentChannelNo] = fOldRangeEnd;
									CurrentChannelNo = wOldChannel;
									if(emOldWorkMode == DanJingGuanLian || emOldWorkMode == ShuangJingGuanLian)
									{
										PostMessage(hStatusBar, SB_SETTEXT, 
										(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T(""));
									}
									else
									{
										if(CurrentChannelNo == 0)
										{
											swprintf(gszTextChannel, _T("1"));
										}
										else if(CurrentChannelNo == 1)
										{
											swprintf(gszTextChannel, _T("2"));
										}
										PostMessage(hStatusBar, SB_SETTEXT, 
											(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
									}

									gSysInfo.wWorkMode = emOldWorkMode;
									PostMessage(hStatusBar, SB_SETTEXT, 
											(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)gSysInfo.wWorkMode);
									
									SetEvent(hWorkEvent);
									
									CloseHandle(hFileRead);
								}
							}
							for(int i = 0; i < VIDEO_NUM; i++)
							{	
								bRecordFlag[i] = OFF;
								bReplayFlag[i] = OFF;
							}

							displayRecordIndex(Wnd, wRecordIndex);						
						}
					}
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
	return CallWindowProc(OldBtnVideoRecordFileNumberProc,Wnd,Msg,wParam,lParam);
}
/**************************
*¼�񿪹ذ�ť�Զ��崦�����
* @videoRecord    hInstance[in]:
* @videoRecord		message[in]:
* @videoRecord		wParam[in]:
* @videoRecord		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnVideoRecordRecordOnOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndVideoRecord);
					hWndVideoRecord = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;

					for(int i = 0; i < VIDEO_NUM; i++)
					{	
						if(bReplayFlag[i] == ON)
						{
							KillTimer(hWndVideoRecord, TIMER_REPLAY);
							SetRedrawFlag(TRUE);//ˢ��
							gbReplayFlag = OFF;				
							
							fRangeStart[CurrentChannelNo] = fOldRangeStart;
							fRangeEnd[CurrentChannelNo] = fOldRangeEnd;
							CurrentChannelNo = wOldChannel;
							if(CurrentChannelNo == 0)
							{
								swprintf(gszTextChannel, _T("1"));
							}
							else if(CurrentChannelNo == 1)
							{
								swprintf(gszTextChannel, _T("2"));
							}
							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

							gSysInfo.wWorkMode = emOldWorkMode;
							PostMessage(hStatusBar, SB_SETTEXT, 
									(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)gSysInfo.wWorkMode);

							memset(gszTextPlus, 0, sizeof(gszTextPlus));
							swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

							memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
							swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
										
							CloseHandle(hFileRead);

							SetEvent(hWorkEvent);
						}
					}
					for(int i = 0; i < VIDEO_NUM; i++)
					{	
						bRecordFlag[i] = OFF;
						bReplayFlag[i] = OFF;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(videoRecordBtnList[i] != currentVideoRecordBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentVideoRecordBtn = videoRecordBtnList[i];
					SetFocus(currentVideoRecordBtn);
					gCurrentFocusBtn = currentVideoRecordBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(videoRecordBtnList[i] != currentVideoRecordBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentVideoRecordBtn = videoRecordBtnList[i];
					SetFocus(currentVideoRecordBtn);
					gCurrentFocusBtn = currentVideoRecordBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					bRecordFlag[wRecordIndex] = !bRecordFlag[wRecordIndex];
					
					displayRecordStatus(Wnd ,bRecordFlag[wRecordIndex]);

					if(bRecordFlag[wRecordIndex] == ON)
					{
					 	if(bReplayFlag[wRecordIndex] == ON)
					 	{
							bReplayFlag[wRecordIndex] = OFF;
							displayRecordStatus(hBtnVideoRecordPlay ,bReplayFlag[wRecordIndex]);
					 	}
						
						wVideoDataNum = 0;
						memset(&stVideoInfo, 0 , sizeof(VIDEO_INFO));

						hFileWrite = CreateFile(szVideoFileName,
													GENERIC_READ|GENERIC_WRITE,
													0,
													NULL,
													CREATE_ALWAYS,
													FILE_ATTRIBUTE_NORMAL,
													NULL);
						if(hFileWrite == INVALID_HANDLE_VALUE)
						{
							PostMessage(hStatusBar, SB_SETTEXT, 
										(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("����video�ļ�ʧ��"));
						}
						if(gwStoreType == 0)	//SD
						{
							GetDiskFreeSpaceEx(_T("\\Storage Card"), (PULARGE_INTEGER)&qwFreeBytesToCaller, NULL, NULL);
						}
						else if((gwStoreType == 1))	//USB
						{
							if(!GetDiskFreeSpaceEx(_T("\\Ӳ��"), (PULARGE_INTEGER)&qwFreeBytesToCaller, NULL, NULL))
							{
								PostMessage(hStatusBar, SB_SETTEXT, 
									(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��ȡ��������ʧ��"));
							}
						}
						else if((gwStoreType == 2))	//����
						{
							GetDiskFreeSpaceEx(_T("\\ResidentFlash"), (PULARGE_INTEGER)&qwFreeBytesToCaller, NULL, NULL);
						}
						qwMaxRecordNum = qwFreeBytesToCaller / sizeof(stVideoInfo);
						if(qwMaxRecordNum < (DWORD64)sizeof(stVideoInfo))
						{
							PostMessage(hStatusBar, SB_SETTEXT, 
									(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("�ռ�����"));
						}
					/*	wchar_t strText[20];
						memset(strText, 0, sizeof(strText));
				
						swprintf(strText, _T("%I64d"), qwFreeBytesToCaller /1024 /1024);
						DrawLableText(hBtnVideoRecordPlay, strText, 12, 
												STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					*/
						SetTimer(hWndVideoRecord, TIMER_RECORD, 33, videoRecordProc);
					}
					else
					{
						KillTimer(hWndVideoRecord, TIMER_RECORD);

					    CloseHandle(hFileWrite);
						/*test
						wchar_t strText[20];
						memset(strText, 0, sizeof(strText));
				
						swprintf(strText, _T("%d"), wVideoDataNum);
						DrawLableText(hBtnVideoRecordFileNumber, strText, DEFAULT_LABLE_TEXT_HEIGHT, 
											STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					*/
					}
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
	return CallWindowProc(OldBtnVideoRecordRecordOnOffProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�طŰ�ť�Զ��崦�����
* @videoRecord    hInstance[in]:
* @videoRecord		message[in]:
* @videoRecord		wParam[in]:
* @videoRecord		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnVideoRecordPlayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndVideoRecord);
					hWndVideoRecord = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;

					for(int i = 0; i < VIDEO_NUM; i++)
					{	
						if(bReplayFlag[i] == ON)
						{
							KillTimer(hWndVideoRecord, TIMER_REPLAY);
							SetRedrawFlag(TRUE);//ˢ��
							gbReplayFlag = OFF;				
							
							fRangeStart[CurrentChannelNo] = fOldRangeStart;
							fRangeEnd[CurrentChannelNo] = fOldRangeEnd;
							CurrentChannelNo = wOldChannel;
							if(CurrentChannelNo == 0)
							{
								swprintf(gszTextChannel, _T("1"));
							}
							else if(CurrentChannelNo == 1)
							{
								swprintf(gszTextChannel, _T("2"));
							}
							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

							gSysInfo.wWorkMode = emOldWorkMode;
							PostMessage(hStatusBar, SB_SETTEXT, 
									(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)gSysInfo.wWorkMode);

							memset(gszTextPlus, 0, sizeof(gszTextPlus));
							swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

							memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
							swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
										
							CloseHandle(hFileRead);

							SetEvent(hWorkEvent);
						}
					}
					for(int i = 0; i < VIDEO_NUM; i++)
					{	
						bRecordFlag[i] = OFF;
						bReplayFlag[i] = OFF;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(videoRecordBtnList[i] != currentVideoRecordBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentVideoRecordBtn = videoRecordBtnList[i];
					SetFocus(currentVideoRecordBtn);
					gCurrentFocusBtn = currentVideoRecordBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(videoRecordBtnList[i] != currentVideoRecordBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentVideoRecordBtn = videoRecordBtnList[i];
					SetFocus(currentVideoRecordBtn);
					gCurrentFocusBtn = currentVideoRecordBtn;
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					bReplayFlag[wRecordIndex] = !bReplayFlag[wRecordIndex];
					
					displayReplayStatus(Wnd ,bReplayFlag[wRecordIndex]);

					if(bReplayFlag[wRecordIndex] == ON)
					{
						gbReplayFlag = ON;
						if(bRecordFlag[wRecordIndex] == ON)
						{
							KillTimer(hWndVideoRecord, TIMER_RECORD);
				
			    			CloseHandle(hFileWrite);

							bRecordFlag[wRecordIndex] = OFF;
							displayRecordStatus(hBtnVideoRecordRecordOnOff ,bRecordFlag[wRecordIndex]);
						}//�������¼��״̬��ر�¼��
					
						dwPlayedDataNum = 0;
					//	ReadVideoFile(_T("\\ResidentFlash\\video.dat"));
						hFileRead = CreateFile(szVideoFileName,
												GENERIC_READ|GENERIC_WRITE,
												0,
												NULL,
												OPEN_EXISTING,
												FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY,
												NULL);
						if(hFileRead == INVALID_HANDLE_VALUE)
						{
							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��video�ļ�ʧ��"));
						}
						dwFileSize = GetFileSize(hFileRead, NULL);
				/*		if(dwFileSize == INVALD_FILE_SIZE)//��ȡ�ļ���С
						{
							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��ȡ�ļ���Сʧ��"));
						}*/
						fOldRangeStart = fRangeStart[CurrentChannelNo];
						fOldRangeEnd = fRangeEnd[CurrentChannelNo];
						wOldChannel = CurrentChannelNo;
						emOldWorkMode = gSysInfo.wWorkMode;
						dwPlayDataNum = dwFileSize / sizeof(stVideoInfo);
						memset(gszInfo, 0, sizeof(gszInfo));//���ϽǷ�ֵ��Ϣ���,��������ʾ
					//	wchar_t strText[20];
					//	memset(strText, 0, sizeof(strText));
			
					//	swprintf(strText, _T("%d"), dwPlayDataNum);
					//	DrawLableText(hBtnVideoRecordPlay, strText, DEFAULT_LABLE_TEXT_HEIGHT, 
					//						STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
						SetRedrawFlag(TRUE);//ˢ����Ļ
						SetTimer(hWndVideoRecord, TIMER_REPLAY, 33, videoReplayProc);
					}
					else
					{	
						KillTimer(hWndVideoRecord, TIMER_REPLAY);
						SetRedrawFlag(TRUE);//ˢ��
						gbReplayFlag = OFF;				
						
						fRangeStart[CurrentChannelNo] = fOldRangeStart;
						fRangeEnd[CurrentChannelNo] = fOldRangeEnd;
						CurrentChannelNo = wOldChannel;
						if(CurrentChannelNo == 0)
						{
							swprintf(gszTextChannel, _T("1"));
						}
						else if(CurrentChannelNo == 1)
						{
							swprintf(gszTextChannel, _T("2"));
						}
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

						gSysInfo.wWorkMode = emOldWorkMode;
						PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)gSysInfo.wWorkMode);

						memset(gszTextPlus, 0, sizeof(gszTextPlus));
						swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

						memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
						swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
									
						CloseHandle(hFileRead);

						SetEvent(hWorkEvent);
					}
						
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
	return CallWindowProc(OldBtnVideoRecordPlayProc,Wnd,Msg,wParam,lParam);
}
