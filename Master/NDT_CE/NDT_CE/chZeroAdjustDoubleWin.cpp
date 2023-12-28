/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ������㷨���У׼����Դ�ļ�
 * Filename: ChZeroDoubleWin.cpp
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

HWND hWndChZeroDouble = NULL;						//������㷨���У׼���ھ��
HWND currentChZeroDoubleBtn = NULL;				//������㷨���У׼���ڵ�ǰѡ�еİ�ť
BOOL gbChZeroDoubleStart = FALSE;			//˫�㷨���У׼�Ƿ������	
BOOL gbChZeroDoubleMinStart = FALSE;		//˫�㷨��С�Կ����У׼�Ƿ������	
float gfAdjustZeroMin = -1;					//��С�Կ�У׼�����

static HWND hBtnChZeroDoubleMinThickness = NULL;		//��С�Կ��Ȱ�ť���
static HWND hBtnChZeroDoubleMinAdjust = NULL;			//��С�Կ�У׼��ť���
static HWND hBtnChZeroDoubleMaxThickness = NULL;		//����Կ��Ȱ�ť���
static HWND hBtnChZeroDoubleStart = NULL;				//У׼��ʼ��ť���
static HWND hBtnChZeroDoubleResult = NULL;				//У׼��㰴ť���
static HWND hBtnChZeroDoubleAccept = NULL;				//����У׼�����ť���

static HWND ChZeroDoubleBtnList[6] = {NULL,NULL,NULL,NULL,NULL,NULL};					//������㷨���У׼���ڵİ�ť��

static WNDPROC OldBtnChZeroDoubleMinThicknessProc = NULL;				//��С�Կ��Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnChZeroDoubleMinAdjustProc = NULL;		//��С�Կ�У׼��ťĬ�ϴ������
static WNDPROC OldBtnChZeroDoubleMaxThicknessProc = NULL;		//����Կ��Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnChZeroDoubleStartProc = NULL;				//У׼��ʼ��ťĬ�ϴ������
static WNDPROC OldBtnChZeroDoubleResultProc = NULL;				//У׼��㰴ťĬ�ϴ������
static WNDPROC OldBtnChZeroDoubleAcceptProc = NULL;				//����У׼�����ťĬ�ϴ������

static const WORD buttonNum = 6;		//��ť�ؼ���
float fMinThickness = 10;		//�Կ���(min)
float fMaxThickness = 10;		//�Կ���(max)
static float fThicknessStep = 1;		//�Կ��Ȳ���
static WORD wThicknessStepIndex = 1;	//�Կ��Ȳ�������

ATOM registerChZeroDoubleChild(HINSTANCE hInstance);
static LRESULT CALLBACK ChZeroDoubleChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int ChZeroDouble_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChZeroDoubleMinThicknessProc( HWND Wnd, 
													UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChZeroDoubleMinAdjustProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChZeroDoubleMaxThicknessProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChZeroDoubleStartProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChZeroDoubleResultProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChZeroDoubleAcceptProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//����ʵ�����

extern wchar_t gszTextStep[10];
extern wchar_t gszTextPeak[20];
extern HWND hStatusBar;			// ״̬�����
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndChZeroAdjust;		//������㷨���У׼���ھ��
extern HWND currentChZeroAdjustBtn;	//������㷨���У׼�����е�ǰѡ�еİ�ť
extern CRITICAL_SECTION csAdjust;
extern float gfAdjustZero;							//У׼�����
extern GATE_INFO GateInfos[2][3];
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern WORD CurrentChannelNo;				// ��ǰͨ����
extern SEND_PARAM_DATA SendParam_ad[2];
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern HWND gCurrentWnd;
extern GATE_INFO stOldGateInfos[2][3];
//extern void SetDistance(HWND hWnd, const float& fDistance, const WORD& nIndex);
extern void SetDistanceGate(const WORD& nIndex);

/**************************
*��ʾ���У׼���㷨�Կ���
* @param		hWnd[in]:���ھ��
* @param		fHouDu[in]:�ְ���
* @return   LRESULT :
* @since    1.00
***************************/
void displayZeroDoubleThickness(HWND hWnd ,float fHouDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fHouDu);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}

/**************************
*��ʾ���㷨���У׼�����
* @param		hWnd[in]:���ھ��
* @param		fZero[in]:���ֵ
* @return   LRESULT :
* @since    1.00
***************************/
void displayDoubleAdjustZero(HWND hWnd ,float fZero)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(fZero == -1)
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	else
	{
		wsprintf(strText, _T("%.1f"), fZero);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}
}

/**************************
*ע�������㷨���У׼����
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChZeroDoubleChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) ChZeroDoubleChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChZeroDoubleChild;

	return RegisterClass(&wc);
}
/**************************
*������㷨���У׼���ڴ������
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK ChZeroDoubleChildProc(HWND hWnd, UINT message, 
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
			gfAdjustZeroMin = -1;
			//����ť�ؼ�
			hBtnChZeroDoubleMinThickness = CreateWindow(_T("BUTTON"),_T("MinThicknessChZeroDouble"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZERODOUBLE_MINTHICKNESS,hInst,NULL);

			hBtnChZeroDoubleMinAdjust = CreateWindow(_T("BUTTON"),_T("MinAdjustChZeroDouble"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZERODOUBLE_MINADJUST,hInst,NULL);

			hBtnChZeroDoubleMaxThickness = CreateWindow(_T("BUTTON"),_T("maxThicknessChZeroDouble"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZERODOUBLE_MAXTHICKNESS,hInst,NULL);

			hBtnChZeroDoubleStart = CreateWindow(_T("BUTTON"),_T("ChZeroDoubleStart"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZERODOUBLE_START,hInst,NULL);

			hBtnChZeroDoubleResult = CreateWindow(_T("BUTTON"),_T("ChZeroDoubleResult"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZERODOUBLE_RESULT,hInst,NULL);

			hBtnChZeroDoubleAccept = CreateWindow(_T("BUTTON"),_T("ChZeroDoubleAccept"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 5*BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHZERODOUBLE_ACCEPT,hInst,NULL);
					
			ChZeroDoubleBtnList[0] = hBtnChZeroDoubleMinThickness;
			ChZeroDoubleBtnList[1] = hBtnChZeroDoubleMinAdjust;
			ChZeroDoubleBtnList[2] = hBtnChZeroDoubleMaxThickness;
			ChZeroDoubleBtnList[3] = hBtnChZeroDoubleStart;
			ChZeroDoubleBtnList[4] = hBtnChZeroDoubleResult;
			ChZeroDoubleBtnList[5] = hBtnChZeroDoubleAccept;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnChZeroDoubleMinThicknessProc = (WNDPROC) GetWindowLong(hBtnChZeroDoubleMinThickness,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroDoubleMinThickness,GWL_WNDPROC,(LONG)btnChZeroDoubleMinThicknessProc);
			
			OldBtnChZeroDoubleMinAdjustProc = (WNDPROC) GetWindowLong(hBtnChZeroDoubleMinAdjust,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroDoubleMinAdjust,GWL_WNDPROC,(LONG)btnChZeroDoubleMinAdjustProc);
			
			OldBtnChZeroDoubleMaxThicknessProc = (WNDPROC) GetWindowLong(hBtnChZeroDoubleMaxThickness,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroDoubleMaxThickness,GWL_WNDPROC,(LONG)btnChZeroDoubleMaxThicknessProc);
			
			OldBtnChZeroDoubleStartProc = (WNDPROC) GetWindowLong(hBtnChZeroDoubleStart,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroDoubleStart,GWL_WNDPROC,(LONG)btnChZeroDoubleStartProc);

			OldBtnChZeroDoubleResultProc = (WNDPROC) GetWindowLong(hBtnChZeroDoubleResult,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroDoubleResult,GWL_WNDPROC,(LONG)btnChZeroDoubleResultProc);

			OldBtnChZeroDoubleAcceptProc = (WNDPROC) GetWindowLong(hBtnChZeroDoubleAccept,GWL_WNDPROC);
			SetWindowLong(hBtnChZeroDoubleAccept,GWL_WNDPROC,(LONG)btnChZeroDoubleAcceptProc);
		
			currentChZeroDoubleBtn = ChZeroDoubleBtnList[0];//Ĭ�ϵ�ǰѡ����С�Կ��Ȱ�ť
			SetFocus(currentChZeroDoubleBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentChZeroDoubleBtn;

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
		/*		if(LOWORD(wParam) == IDC_CHZERODOUBLE_MAXTHICKNESS)
				{
					currentChZeroDoubleBtn = ChZeroDoubleBtnList[2]; 
					SetFocus(currentChZeroDoubleBtn);
					gCurrentFocusBtn = currentChZeroDoubleBtn;
					hWndMaxThickness = CreateWindow(szMaxThicknessChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							MENU_MAXTHICKNESS_X, MENU_MAXTHICKNESS_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChZeroDouble,SW_HIDE);
					//SetFocus(hWndMaxThickness);
					ShowWindow(hWndMaxThickness,SW_SHOW);
					UpdateWindow(hWndMaxThickness);
				}
				else if(LOWORD(wParam) == IDC_CHZERODOUBLE_START)
				{
				//	currentChZeroDoubleBtn = ChZeroDoubleBtnList[3]; 
				//	SetFocus(currentChZeroDoubleBtn);
				//	gCurrentFocusBtn = currentChZeroDoubleBtn;
				}
			*/
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			ChZeroDouble_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnChZeroDoubleMinThickness)
			{
				displayZeroDoubleThickness(pdis->hwndItem, fMinThickness);
			}
			if(pdis->hwndItem == hBtnChZeroDoubleMaxThickness)
			{
				displayZeroDoubleThickness(pdis->hwndItem, fMaxThickness);
			}
			if(pdis->hwndItem == hBtnChZeroDoubleResult)
			{
				displayDoubleAdjustZero(pdis->hwndItem, gfAdjustZero);
			}
			if(pdis->hwndItem == hBtnChZeroDoubleMinAdjust)
			{
				SetOpenFlag(NULL, TRUE, 1);
			}
			if(pdis->hwndItem == hBtnChZeroDoubleStart)
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
*���ز�����㷨���У׼���ڰ�ťͼƬ
* @ param    hInstance[in]:
* @ param    		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int ChZeroDouble_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_ChZeroDoubleBtnBitmap[]=
	{
		{IDC_CHZERODOUBLE_MINTHICKNESS	,IDB_BITMAP_SHIKUAIHOUDUMIN_NOSEL_R	 ,  IDB_BITMAP_SHIKUAIHOUDUMIN_SELECT_R		},
		{IDC_CHZERODOUBLE_MINADJUST	,IDB_BITMAP_BOSHIKUAIJIAOZHUN_NOSEL_R	 ,  IDB_BITMAP_BOSHIKUAIJIAOZHUN_SELECT_R		},
		{IDC_CHZERODOUBLE_MAXTHICKNESS	,IDB_BITMAP_SHIKUAIHOUDUMAX_NOSEL_R ,  IDB_BITMAP_SHIKUAIHOUDUMAX_SELECT_R	},
		{IDC_CHZERODOUBLE_START	,IDB_BITMAP_JIAOZHUNKAISHI5_NOSEL_R ,  IDB_BITMAP_JIAOZHUNKAISHI5_SELECT_R	},
		{IDC_CHZERODOUBLE_RESULT	,IDB_BITMAP_JIAOZHUNLINGDIAN6_NOSEL_R ,  IDB_BITMAP_JIAOZHUNLINGDIAN6_SELECT_R	},
		{IDC_CHZERODOUBLE_ACCEPT	,IDB_BITMAP_JIESHOUJIAOZHUNJIEGUO6_NOSEL_R ,  IDB_BITMAP_JIESHOUJIAOZHUNJIEGUO6_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_ChZeroDoubleBtnBitmap);
	return ReturnVal;
}
/**************************
*��С�Կ��Ȱ�ť�Զ��崦�����
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChZeroDoubleMinThicknessProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndChZeroDouble);
					hWndChZeroDouble = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;

					gCurrentWnd = hWndChZeroAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroDoubleBtnList[i] != currentChZeroDoubleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroDoubleBtn = ChZeroDoubleBtnList[i];
					SetFocus(currentChZeroDoubleBtn);
					gCurrentFocusBtn = currentChZeroDoubleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroDoubleBtnList[i] != currentChZeroDoubleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroDoubleBtn = ChZeroDoubleBtnList[i];
					SetFocus(currentChZeroDoubleBtn);
					gCurrentFocusBtn = currentChZeroDoubleBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if((fMinThickness - fThicknessStep) >= 1)
						fMinThickness -= fThicknessStep;
					else
						fMinThickness = 1;

					displayZeroDoubleThickness(Wnd, fMinThickness);	

					SetDistanceGate(1);
				}
				else if(wParam == VK_RIGHT)
				{
					fMinThickness += fThicknessStep;

					displayZeroDoubleThickness(Wnd, fMinThickness);

					SetDistanceGate(1);
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
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChZeroDoubleMinThicknessProc,Wnd,Msg,wParam,lParam);
}
/**************************
*��С�Կ�У׼��ť�Զ��崦�����
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChZeroDoubleMinAdjustProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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

					DestroyWindow(hWndChZeroDouble);
					hWndChZeroDouble = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;

					gCurrentWnd = hWndChZeroAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroDoubleBtnList[i] != currentChZeroDoubleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroDoubleBtn = ChZeroDoubleBtnList[i];
					SetFocus(currentChZeroDoubleBtn);
					gCurrentFocusBtn = currentChZeroDoubleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroDoubleBtnList[i] != currentChZeroDoubleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroDoubleBtn = ChZeroDoubleBtnList[i];
					SetFocus(currentChZeroDoubleBtn);
					gCurrentFocusBtn = currentChZeroDoubleBtn;
				}
				else if(wParam == VK_RETURN)
				{					
					swprintf(gszTextPeak, _T("У׼������..."));

					SendMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);

					SetDistanceGate(1);

					InitializeCriticalSection(&csAdjust);
					EnterCriticalSection(&csAdjust);
					gbChZeroDoubleMinStart = TRUE;
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
					if(currentChZeroDoubleBtn == hBtnChZeroDoubleMinAdjust)
					{
						SetFocus(currentChZeroDoubleBtn);
						/*ʹ��AttachThreadInput������һ���߳̿ɽ����봦�����ӵ������̡߳�
						��ʹ���߳̿��Ե���SetFocus����Ϊһ���������̵߳���Ϣ������صĴ������ü��̽��㡣
						Windows CE����ʹ��SetFocus����Ϊһ���������̵߳���Ϣ������صĴ������ü��̽��㡣����һ�����⡣
						��һ���̵߳Ĵ�������һ�̵߳��Ӵ��ڣ�����Щ�����Ǿ���ͬһ�����ڵ��ֵܴ��ڣ�
						����һ���̹߳����Ĵ��ڿ���Ϊ�����������ý��㣬���ܸô�������һ����ͬ���̡߳�
						����������£��Ͳ����ȵ���AttachThreadlnpUt������ */
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
	return CallWindowProc(OldBtnChZeroDoubleMinAdjustProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����Կ��Ȱ�ť�Զ��崦�����
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChZeroDoubleMaxThicknessProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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

					DestroyWindow(hWndChZeroDouble);
					hWndChZeroDouble = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;

					gCurrentWnd = hWndChZeroAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroDoubleBtnList[i] != currentChZeroDoubleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroDoubleBtn = ChZeroDoubleBtnList[i];
					SetFocus(currentChZeroDoubleBtn);
					gCurrentFocusBtn = currentChZeroDoubleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroDoubleBtnList[i] != currentChZeroDoubleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroDoubleBtn = ChZeroDoubleBtnList[i];
					SetFocus(currentChZeroDoubleBtn);
					gCurrentFocusBtn = currentChZeroDoubleBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if((fMaxThickness - fThicknessStep) >= 1)
						fMaxThickness -= fThicknessStep;
					else
						fMaxThickness = 1;

					displayZeroDoubleThickness(Wnd, fMaxThickness);

					SetDistanceGate(1);
				}
				else if(wParam == VK_RIGHT)
				{
					fMaxThickness += fThicknessStep;

					displayZeroDoubleThickness(Wnd, fMaxThickness);

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
	return CallWindowProc(OldBtnChZeroDoubleMaxThicknessProc,Wnd,Msg,wParam,lParam);
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
static LRESULT CALLBACK btnChZeroDoubleStartProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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

					DestroyWindow(hWndChZeroDouble);
					hWndChZeroDouble = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;
					
					gCurrentWnd = hWndChZeroAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroDoubleBtnList[i] != currentChZeroDoubleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroDoubleBtn = ChZeroDoubleBtnList[i];
					SetFocus(currentChZeroDoubleBtn);
					gCurrentFocusBtn = currentChZeroDoubleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroDoubleBtnList[i] != currentChZeroDoubleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroDoubleBtn = ChZeroDoubleBtnList[i];
					SetFocus(currentChZeroDoubleBtn);
					gCurrentFocusBtn = currentChZeroDoubleBtn;
				}
				else if(wParam == VK_RETURN)
				{
					if(gfAdjustZeroMin == -1)
					{
						memset(gszTextPeak, 0, sizeof(gszTextPeak));
						swprintf(gszTextPeak, _T("��У׼���Կ�"));

						SendMessage(hStatusBar, SB_SETTEXT, 
									(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
					}
					else
					{
						gfAdjustZero = -1;
						displayDoubleAdjustZero(hBtnChZeroDoubleResult, gfAdjustZero);//У׼���ǰ��ʾN/A

						memset(gszTextPeak, 0, sizeof(gszTextPeak));
						swprintf(gszTextPeak, _T("У׼������..."));

						SendMessage(hStatusBar, SB_SETTEXT, 
									(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);

						SetDistanceGate(1);

						InitializeCriticalSection(&csAdjust);
						EnterCriticalSection(&csAdjust);
						gbChZeroDoubleStart = TRUE;
						LeaveCriticalSection(&csAdjust);
						DeleteCriticalSection(&csAdjust);
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
				else if(wParam == '9')
				{
					if(currentChZeroDoubleBtn == hBtnChZeroDoubleStart)
					{
						SetFocus(currentChZeroDoubleBtn);
						displayDoubleAdjustZero(hBtnChZeroDoubleResult, gfAdjustZero);
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
	return CallWindowProc(OldBtnChZeroDoubleStartProc,Wnd,Msg,wParam,lParam);
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
static LRESULT CALLBACK btnChZeroDoubleResultProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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

					DestroyWindow(hWndChZeroDouble);
					hWndChZeroDouble = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;

					gCurrentWnd = hWndChZeroAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroDoubleBtnList[i] != currentChZeroDoubleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroDoubleBtn = ChZeroDoubleBtnList[i];
					SetFocus(currentChZeroDoubleBtn);
					gCurrentFocusBtn = currentChZeroDoubleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroDoubleBtnList[i] != currentChZeroDoubleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroDoubleBtn = ChZeroDoubleBtnList[i];
					SetFocus(currentChZeroDoubleBtn);
					gCurrentFocusBtn = currentChZeroDoubleBtn;
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
	return CallWindowProc(OldBtnChZeroDoubleResultProc,Wnd,Msg,wParam,lParam);
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
static LRESULT CALLBACK btnChZeroDoubleAcceptProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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

					DestroyWindow(hWndChZeroDouble);
					hWndChZeroDouble = 0;

					gCurrentFocusBtn = currentChZeroAdjustBtn;

					gCurrentWnd = hWndChZeroAdjust;
					
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(ChZeroDoubleBtnList[i] != currentChZeroDoubleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChZeroDoubleBtn = ChZeroDoubleBtnList[i];
					SetFocus(currentChZeroDoubleBtn);
					gCurrentFocusBtn = currentChZeroDoubleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ChZeroDoubleBtnList[i] != currentChZeroDoubleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChZeroDoubleBtn = ChZeroDoubleBtnList[i];
					SetFocus(currentChZeroDoubleBtn);
					gCurrentFocusBtn = currentChZeroDoubleBtn;
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
	return CallWindowProc(OldBtnChZeroDoubleAcceptProc,Wnd,Msg,wParam,lParam);
}

