/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ���У׼����Դ�ļ�
 * Filename: ChZeroSingleWin.cpp
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

HWND hWndChZeroSingle = NULL;					//��񵥵㷨���У׼���ھ��
HWND currentChZeroSingleBtn = NULL;				//��񵥵㷨���У׼���ڵ�ǰѡ�еİ�ť
BOOL gbChZeroSingleStart = FALSE;			//���㷨���У׼�Ƿ������	
float gfAdjustZero = 0;							//У׼�����

static HWND hBtnChZeroSingleThickness = NULL;			//�Կ��Ȱ�ť���
static HWND hBtnChZeroSingleStart = NULL;				//У׼��ʼ��ť���
static HWND hBtnChZeroSingleResult = NULL;				//У׼��㰴ť���
static HWND hBtnChZeroSingleAccept = NULL;				//����У׼�����ť���

static HWND ChZeroSingleBtnList[4] = {NULL,NULL,NULL,NULL};						//��񵥵㷨���У׼���ڵİ�ť��

static WNDPROC OldBtnChZeroSingleThicknessProc = NULL;			//�Կ��Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnChZeroSingleStartProc = NULL;				//У׼��ʼ��ťĬ�ϴ������
static WNDPROC OldBtnChZeroSingleResultProc = NULL;				//У׼��㰴ťĬ�ϴ������
static WNDPROC OldBtnChZeroSingleAcceptProc = NULL;				//����У׼�����ťĬ�ϴ������

static const WORD buttonNum = 4;		//��ť�ؼ���

float gfZeroThickness = 10;				//���У׼�Կ���
static float fThicknessStep = 1;		//�Կ��Ȳ���
static WORD wThicknessStepIndex = 1;	//�Կ��Ȳ�������

ATOM registerChZeroSingleChild(HINSTANCE hInstance);
static LRESULT CALLBACK ChZeroSingleChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int ChZeroSingle_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChZeroSingleThicknessProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChZeroSingleStartProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChZeroSingleResultProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChZeroSingleAcceptProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//����ʵ�����

extern wchar_t gszTextStep[10];
extern wchar_t gszTextPeak[20];
extern HWND hStatusBar;			// ״̬�����
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndChZeroAdjust;		//������У׼���ھ��
extern HWND currentChZeroAdjustBtn;	//���У׼�����е�ǰѡ�еİ�ť
extern CRITICAL_SECTION csAdjust;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD CurrentChannelNo;
extern CRITICAL_SECTION csGate;
extern SEND_PARAM_DATA SendParam_ad[2];
extern GATE_INFO GateInfos[2][3];
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern HWND gCurrentWnd;
extern GATE_INFO stOldGateInfos[2][3];

extern void displayThickness(HWND hWnd ,float fHouDu);
extern void displayAdjustZero(HWND hWnd ,float fZero);
extern void SetDistance(HWND hWnd, const float& fDistance, const WORD& nIndex);
extern void SetAmendLableAndExecute(HWND hWnd);

/**************************
*��ʾ���У׼���㷨�Կ���
* @param		hWnd[in]:���ھ��
* @param		fHouDu[in]:�ְ���
* @return   LRESULT :
* @since    1.00
***************************/
void displayZeroSingleThickness(HWND hWnd ,float fHouDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fHouDu);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

/**************************
*��ʾ���㷨���У׼�����
* @param		hWnd[in]:���ھ��
* @param		fZero[in]:���ֵ
* @return   LRESULT :
* @since    1.00
***************************/
void displaySingleAdjustZero(HWND hWnd ,float fZero)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(fZero == -1)
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	else
	{
		wsprintf(strText, _T("%.1f"), fZero);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
}
/*
void SetDistanceGate(const float& fDistance, const WORD& nIndex)
{
	float fPianCha = (DisplaySet_ad[CurrentChannelNo].fRange / 100) * 2;//����բ�ſ������Ļ��һ��
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);

//	GateInfos[CurrentChannelNo][nIndex-1].bOpen = TRUE;
	//GateInfos[CurrentChannelNo][nIndex-1].fFrontGate = fDistance - fPianCha;
	//GateInfos[CurrentChannelNo][nIndex-1].fBackGate = fDistance + fPianCha;
	float fQianDian = fDistance - fPianCha;
	float fBack = fDistance + fPianCha;

	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);

	// ������־
	SetOpenFlag(NULL, TRUE, nIndex);

	// ���
	SetQiDian(NULL, nIndex, fQianDian,fBack);
	
}
*/
void SetDistanceGate(const WORD& nIndex)
{
/*	float fPianCha = (DisplaySet_ad[CurrentChannelNo].fRange / 100) * 2;//����բ�ſ������Ļ��һ��
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);

//	GateInfos[CurrentChannelNo][nIndex-1].bOpen = TRUE;
	//GateInfos[CurrentChannelNo][nIndex-1].fFrontGate = fDistance - fPianCha;
	//GateInfos[CurrentChannelNo][nIndex-1].fBackGate = fDistance + fPianCha;
	float fQianDian = fDistance - fPianCha;
	float fBack = fDistance + fPianCha;

	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
*/
	float fGateWidth = (DisplaySet_ad[CurrentChannelNo].fRange / 100) * 4;	//����բ�ſ������Ļ��һ��
	float fHuiBoDistance = 0;												//�ײ�����
	float fGateQianDian = 0;												//բ�����
	float fGateBack = 0;													//բ�ź���

	if(nIndex == 1)
		fHuiBoDistance = firstHuiBoDistance();
	else if(nIndex == 2)
		fHuiBoDistance = secondHuiBoDistance();
	fGateQianDian = fHuiBoDistance - fGateWidth / 2;
	fGateBack = fGateQianDian + fGateWidth;
	
	// ���
	SetQiDian(NULL, nIndex, fGateQianDian,fGateBack, FALSE);
	//��ֵ
	SetPeakGate(NULL, nIndex, 62, FALSE);

	// ������־
	SetOpenFlag(NULL, TRUE, nIndex);
	Sleep(10);
}

/**************************
*ע����У׼����
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChZeroSingleChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) ChZeroSingleChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChZeroSingleChild;

	return RegisterClass(&wc);
}
/**************************
*���У׼���ڴ������
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK ChZeroSingleChildProc(HWND hWnd, UINT message, 
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
			fThicknessStep = AMENDSTEPS[wThicknessStepIndex];
			gfAdjustZero = -1;
			//����ť�ؼ�

			hBtnChZeroSingleThickness = CreateWindow(_T("BUTTON"),_T("ThicknessChZeroSingle"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZEROSINGLE_THICKNESS,hInst,NULL);

			hBtnChZeroSingleStart = CreateWindow(_T("BUTTON"),_T("ChZeroSingleStart"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZEROSINGLE_START,hInst,NULL);

			hBtnChZeroSingleResult = CreateWindow(_T("BUTTON"),_T("ChZeroSingleResult"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZEROSINGLE_RESULT,hInst,NULL);

			hBtnChZeroSingleAccept = CreateWindow(_T("BUTTON"),_T("ChZeroSingleAccept"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 3*BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZEROSINGLE_ACCEPT,hInst,NULL);
					
			ChZeroSingleBtnList[0] = hBtnChZeroSingleThickness;
			ChZeroSingleBtnList[1] = hBtnChZeroSingleStart;
			ChZeroSingleBtnList[2] = hBtnChZeroSingleResult;
			ChZeroSingleBtnList[3] = hBtnChZeroSingleAccept;
			
			//��ť�ؼ����Զ��崦�����			
			OldBtnChZeroSingleThicknessProc = (WNDPROC) GetWindowLong(hBtnChZeroSingleThickness,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroSingleThickness,GWL_WNDPROC,(LONG)btnChZeroSingleThicknessProc);
			
			OldBtnChZeroSingleStartProc = (WNDPROC) GetWindowLong(hBtnChZeroSingleStart,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroSingleStart,GWL_WNDPROC,(LONG)btnChZeroSingleStartProc);

			OldBtnChZeroSingleResultProc = (WNDPROC) GetWindowLong(hBtnChZeroSingleResult,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroSingleResult,GWL_WNDPROC,(LONG)btnChZeroSingleResultProc);

			OldBtnChZeroSingleAcceptProc = (WNDPROC) GetWindowLong(hBtnChZeroSingleAccept,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroSingleAccept,GWL_WNDPROC,(LONG)btnChZeroSingleAcceptProc);
		
			currentChZeroSingleBtn = ChZeroSingleBtnList[0];//Ĭ�ϵ�ǰѡ��У׼������ť
			SetFocus(currentChZeroSingleBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentChZeroSingleBtn;

			SetDistanceGate(1);
			SetOpenFlag(NULL, FALSE, 2);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
		/*		if(LOWORD(wParam) == IDC_CHZEROSINGLE_THICKNESS)
				{
					currentChZeroSingleBtn = ChZeroSingleBtnList[2]; 
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
					hWndThickness = CreateWindow(szThicknessChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							MENU_THICKNESS_X, MENU_THICKNESS_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChZeroSingle,SW_HIDE);
					//SetFocus(hWndThickness);
					ShowWindow(hWndThickness,SW_SHOW);
					UpdateWindow(hWndThickness);
				}
				else if(LOWORD(wParam) == IDC_CHZEROSINGLE_START)
				{
				//	currentChZeroSingleBtn = ChZeroSingleBtnList[3]; 
				//	SetFocus(currentChZeroSingleBtn);
				//	gCurrentFocusBtn = currentChZeroSingleBtn;
				}
			*/
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			ChZeroSingle_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnChZeroSingleThickness)
			{
				displayZeroSingleThickness(pdis->hwndItem, gfZeroThickness);
			}
			if(pdis->hwndItem == hBtnChZeroSingleResult)
			{
				displaySingleAdjustZero(pdis->hwndItem, gfAdjustZero);
			}
			if(pdis->hwndItem == hBtnChZeroSingleStart)
			{
				SetDistanceGate(1);
				SetOpenFlag(NULL, FALSE, 2);
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
*���ز��У׼���ڰ�ťͼƬ
* @ param    hInstance[in]:
* @ param    		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int ChZeroSingle_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_ChZeroSingleBtnBitmap[]=
	{
		{IDC_CHZEROSINGLE_THICKNESS	,IDB_BITMAP_SHIKUAIHOUDU4_NOSEL_R ,  IDB_BITMAP_SHIKUAIHOUDU4_SELECT_R	},
		{IDC_CHZEROSINGLE_START	,IDB_BITMAP_JIAOZHUNKAISHI5_NOSEL_R ,  IDB_BITMAP_JIAOZHUNKAISHI5_SELECT_R	},
		{IDC_CHZEROSINGLE_RESULT	,IDB_BITMAP_JIAOZHUNLINGDIAN4_NOSEL_R ,  IDB_BITMAP_JIAOZHUNLINGDIAN4_SELECT_R	},
		{IDC_CHZEROSINGLE_ACCEPT	,IDB_BITMAP_JIESHOUJIAOZHUNJIEGUO4_NOSEL_R ,  IDB_BITMAP_JIESHOUJIAOZHUNJIEGUO4_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_ChZeroSingleBtnBitmap);
	return ReturnVal;
}

/**************************
*�Կ��Ȱ�ť�Զ��崦�����
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChZeroSingleThicknessProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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
					ShowWindow(hWndChZeroAdjust,SW_SHOW);
					SetFocus(currentChZeroAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChZeroSingle);
					hWndChZeroSingle = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;

					gCurrentWnd = hWndChZeroAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if((gfZeroThickness - fThicknessStep) >= 1)
						gfZeroThickness -= fThicknessStep;
					else
						gfZeroThickness = 1;

					displayZeroSingleThickness(Wnd, gfZeroThickness);
					
					SetDistanceGate(1);
				}
				else if(wParam == VK_RIGHT)
				{
					gfZeroThickness += fThicknessStep;

					displayZeroSingleThickness(Wnd, gfZeroThickness);
					
					SetDistanceGate(1);
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
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChZeroSingleThicknessProc,Wnd,Msg,wParam,lParam);
}
/**************************
*У׼��ʼ��ť�Զ��崦�����
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChZeroSingleStartProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChZeroAdjust,SW_SHOW);
					SetFocus(currentChZeroAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChZeroSingle);
					hWndChZeroSingle = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;

					gCurrentWnd = hWndChZeroAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_RETURN)
				{					
					displaySingleAdjustZero(hBtnChZeroSingleResult, -1);					//У׼���ǰ��ʾN/A

					memset(gszTextPeak, 0, sizeof(gszTextPeak));
					swprintf(gszTextPeak, _T("У׼������..."));

					SendMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);

					SetDistanceGate(1);
					
					InitializeCriticalSection(&csAdjust);
					EnterCriticalSection(&csAdjust);
					gbChZeroSingleStart = TRUE;
					LeaveCriticalSection(&csAdjust);
					DeleteCriticalSection(&csAdjust);

					SetFocus(NULL);
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
				else if(wParam == '9')
				{
					if(currentChZeroSingleBtn == hBtnChZeroSingleStart)
					{
						SetFocus(currentChZeroSingleBtn);
						displaySingleAdjustZero(hBtnChZeroSingleResult, gfAdjustZero);
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
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChZeroSingleStartProc,Wnd,Msg,wParam,lParam);
}
/**************************
*У׼��㰴ť�Զ��崦�����
* @ param    hInstance[in]:
* @ param    message[in]:
* @ param    wParam[in]:
* @ param    lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChZeroSingleResultProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChZeroAdjust,SW_SHOW);
					SetFocus(currentChZeroAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChZeroSingle);
					hWndChZeroSingle = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;

					gCurrentWnd = hWndChZeroAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
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
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChZeroSingleResultProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����У׼�����ť�Զ��崦�����
* @ param    hInstance[in]:
* @ param    message[in]:
* @ param    wParam[in]:
* @ param    lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChZeroSingleAcceptProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChZeroAdjust,SW_SHOW);
					SetFocus(currentChZeroAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChZeroSingle);
					hWndChZeroSingle = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;

					gCurrentWnd = hWndChZeroAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroSingleBtnList[i] != currentChZeroSingleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroSingleBtn = ChZeroSingleBtnList[i];
					SetFocus(currentChZeroSingleBtn);
					gCurrentFocusBtn = currentChZeroSingleBtn;
				}
				else if(wParam == VK_RETURN)
				{
					if(gfAdjustZero > 0)
					{
						SendParam_ad[CurrentChannelNo].nAmend = gfAdjustZero;
						SetAmendLableAndExecute(NULL);
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
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChZeroSingleAcceptProc,Wnd,Msg,wParam,lParam);
}
