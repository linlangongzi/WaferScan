/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * �������У׼����Դ�ļ�
 * Filename: chWaveSpeedAdjustWin.cpp
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

HWND hWndChWaveSpeedAdjust = NULL;						//�������У׼���ھ��
HWND currentChWaveSpeedAdjustBtn = NULL;					//�������У׼���ڵ�ǰѡ�еİ�ť
BOOL gbChWaveSpeedAdjustStart = FALSE;			//����У׼�Ƿ������	
WORD gwAdjustWaveSpeed = 0;						//У׼�������
static HWND hBtnChWaveSpeedAdjustThickness = NULL;				//�Կ��Ȱ�ť���
static HWND hBtnChWaveSpeedAdjustStandard = NULL;			//������ٰ�ť���
static HWND hBtnChWaveSpeedAdjustStart = NULL;			//����У׼��ʼ��ť���
static HWND hBtnChWaveSpeedAdjustAdjustWaveSpeed = NULL;				//����У׼���ٰ�ť���
static HWND hBtnChWaveSpeedAdjustAccept = NULL;				//������������У׼�����ť���

static HWND chWaveSpeedAdjustBtnList[5] = {NULL,NULL,NULL,NULL,NULL};					//�������У׼���ڵİ�ť��

static WNDPROC OldBtnChWaveSpeedAdjustThicknessProc = NULL;//�Կ��Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnChWaveSpeedAdjustStandardProc = NULL;		//������ٰ�ťĬ�ϴ������
static WNDPROC OldBtnChWaveSpeedAdjustStartProc = NULL;		//����У׼��ʼ��ťĬ�ϴ������
static WNDPROC OldBtnChWaveSpeedAdjustAdjustWaveSpeedProc = NULL;			//����У׼���ٰ�ťĬ�ϴ������
static WNDPROC OldBtnChWaveSpeedAdjustAcceptProc = NULL;			//������������У׼�����ťĬ�ϴ������

static const WORD buttonNum = 5;		//��ť�ؼ���

float gfSpeedAdjustThickness = 10;		//�Կ���
static DWORD dwAdjustWaveSpeed;			//У׼����
static float fThicknessStep = 1;		//�Կ��Ȳ���
static WORD wThicknessStepIndex = 1;	//�Կ��Ȳ�������

ATOM registerChWaveSpeedAdjustChild(HINSTANCE hInstance);
static LRESULT CALLBACK chWaveSpeedAdjustChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chWaveSpeedAdjust_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChWaveSpeedAdjustThicknessProc( HWND Wnd, 
													UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChWaveSpeedAdjustStandardProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChWaveSpeedAdjustStartProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChWaveSpeedAdjustAdjustWaveSpeedProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChWaveSpeedAdjustAcceptProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//����ʵ�����

extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndChAdjust;		//���У׼���ھ��
extern HWND currentChAdjustBtn;	//���У׼�����е�ǰѡ�еİ�ť

extern wchar_t gszTextStep[10];
extern wchar_t gszTextPeak[20];
extern HWND hStatusBar;			// ״̬�����
extern HWND gCurrentWnd;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD CurrentChannelNo;
extern CRITICAL_SECTION csAdjust;
extern BOOL gbGuanLian;
extern GATE_INFO GateInfos[2][3];
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;

extern void SetWaveSpeedLabelAndExecute(HWND hWnd);
extern void SetDistanceGate(const WORD& nIndex);
extern PEAK_INFO gPeakInfos[2][2];//test
extern GATE_INFO stOldGateInfos[2][3];
/**************************
*��ʾ�Կ���
* @param		hWnd[in]:���ھ��
* @param		fHouDu[in]:�ְ���
* @return   LRESULT :
* @since    1.00
***************************/
static void displayThickness(HWND hWnd ,float fHouDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fHouDu);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}
/**************************
*��ʾ�������
* @param		hWnd[in]:���ھ��
* @param		wSpeed[in]:����
* @return   LRESULT :
* @since    1.00
***************************/
static void displayBiaoChengSpeed(HWND hWnd ,WORD wSpeed)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%d"), wSpeed);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}
/**************************
*��ʾУ׼����
* @param		hWnd[in]:���ھ��
* @param		wSpeed[in]:����
* @return   LRESULT :
* @since    1.00
***************************/
static void displayAdjustWaveSpeed(HWND hWnd ,WORD wSpeed)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(wSpeed == 0)
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	else
	{
		wsprintf(strText, _T("%d"), wSpeed);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	}
}


/**************************
*ע��������У׼����
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChWaveSpeedAdjustChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chWaveSpeedAdjustChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChWaveSpeedAdjustChild;

	return RegisterClass(&wc);
}
/**************************
*�������У׼���ڴ������
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chWaveSpeedAdjustChildProc(HWND hWnd, UINT message, 
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
			gwAdjustWaveSpeed = 0;	//��ʼ��У׼����
			//����ť�ؼ�
			hBtnChWaveSpeedAdjustThickness = CreateWindow(_T("BUTTON"),_T("thicknessChWaveSpeedAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHWAVESPEEDADJUST_THICKNESS,hInst,NULL);

			hBtnChWaveSpeedAdjustStandard = CreateWindow(_T("BUTTON"),_T("standardChWaveSpeedAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHWAVESPEEDADJUST_STANDARD,hInst,NULL);

			hBtnChWaveSpeedAdjustStart = CreateWindow(_T("BUTTON"),_T("startChWaveSpeedAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHWAVESPEEDADJUST_START,hInst,NULL);

			hBtnChWaveSpeedAdjustAdjustWaveSpeed = CreateWindow(_T("BUTTON"),_T("adjustWaveSpeedChWaveSpeedAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHWAVESPEEDADJUST_ADJUSTWAVESPEED,hInst,NULL);

			hBtnChWaveSpeedAdjustAccept = CreateWindow(_T("BUTTON"),_T("adjustWaveSpeedChWaveSpeedAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHWAVESPEEDADJUST_ACCEPT,hInst,NULL);
					
			chWaveSpeedAdjustBtnList[0] = hBtnChWaveSpeedAdjustThickness;
			chWaveSpeedAdjustBtnList[1] = hBtnChWaveSpeedAdjustStandard;
			chWaveSpeedAdjustBtnList[2] = hBtnChWaveSpeedAdjustStart;
			chWaveSpeedAdjustBtnList[3] = hBtnChWaveSpeedAdjustAdjustWaveSpeed;
			chWaveSpeedAdjustBtnList[4] = hBtnChWaveSpeedAdjustAccept;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnChWaveSpeedAdjustThicknessProc = (WNDPROC) GetWindowLong(hBtnChWaveSpeedAdjustThickness,GWL_WNDPROC);
			SetWindowLong(hBtnChWaveSpeedAdjustThickness,GWL_WNDPROC,(LONG)btnChWaveSpeedAdjustThicknessProc);
			
			OldBtnChWaveSpeedAdjustStandardProc = (WNDPROC) GetWindowLong(hBtnChWaveSpeedAdjustStandard,GWL_WNDPROC);
			SetWindowLong(hBtnChWaveSpeedAdjustStandard,GWL_WNDPROC,(LONG)btnChWaveSpeedAdjustStandardProc);
			
			OldBtnChWaveSpeedAdjustStartProc = (WNDPROC) GetWindowLong(hBtnChWaveSpeedAdjustStart,GWL_WNDPROC);
			SetWindowLong(hBtnChWaveSpeedAdjustStart,GWL_WNDPROC,(LONG)btnChWaveSpeedAdjustStartProc);
			
			OldBtnChWaveSpeedAdjustAdjustWaveSpeedProc = (WNDPROC) GetWindowLong(hBtnChWaveSpeedAdjustAdjustWaveSpeed,GWL_WNDPROC);
			SetWindowLong(hBtnChWaveSpeedAdjustAdjustWaveSpeed,GWL_WNDPROC,(LONG)btnChWaveSpeedAdjustAdjustWaveSpeedProc);

			OldBtnChWaveSpeedAdjustAcceptProc = (WNDPROC) GetWindowLong(hBtnChWaveSpeedAdjustAccept,GWL_WNDPROC);
			SetWindowLong(hBtnChWaveSpeedAdjustAccept,GWL_WNDPROC,(LONG)btnChWaveSpeedAdjustAcceptProc);
		
			currentChWaveSpeedAdjustBtn = chWaveSpeedAdjustBtnList[0];//Ĭ�ϵ�ǰѡ���Կ��Ȱ�ť
			SetFocus(currentChWaveSpeedAdjustBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentChWaveSpeedAdjustBtn;
			
			SetDistanceGate(1);
			SetDistanceGate(2);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
		/*		if(LOWORD(wParam) == IDC_CHWAVESPEEDADJUST_START)
				{
					currentChWaveSpeedAdjustBtn = chWaveSpeedAdjustBtnList[2]; 
					SetFocus(currentChWaveSpeedAdjustBtn);
					gCurrentFocusBtn = currentChWaveSpeedAdjustBtn;
					hWndStart = CreateWindow(szStartChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChWaveSpeedAdjust,SW_HIDE);
					//SetFocus(hWndStart);
					ShowWindow(hWndStart,SW_SHOW);
					UpdateWindow(hWndStart);
				}
				else if(LOWORD(wParam) == IDC_CHWAVESPEEDADJUST_ADJUSTWAVESPEED)
				{
				//	currentChWaveSpeedAdjustBtn = chWaveSpeedAdjustBtnList[3]; 
				//	SetFocus(currentChWaveSpeedAdjustBtn);
				//	gCurrentFocusBtn = currentChWaveSpeedAdjustBtn;
				}
			*/
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			chWaveSpeedAdjust_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnChWaveSpeedAdjustThickness)
			{
				displayThickness(pdis->hwndItem, gfSpeedAdjustThickness);
			}
			if(pdis->hwndItem == hBtnChWaveSpeedAdjustStandard)
			{
				displayBiaoChengSpeed(pdis->hwndItem, DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
			}
			if(pdis->hwndItem == hBtnChWaveSpeedAdjustAdjustWaveSpeed)
			{
				displayAdjustWaveSpeed(pdis->hwndItem, gwAdjustWaveSpeed);
			}
			if(pdis->hwndItem == hBtnChWaveSpeedAdjustStart)
			{
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
*���ز������У׼���ڰ�ťͼƬ
* @ param    hInstance[in]:
* @ param    		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chWaveSpeedAdjust_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chWaveSpeedAdjustBtnBitmap[]=
	{
		{IDC_CHWAVESPEEDADJUST_THICKNESS	,IDB_BITMAP_SHIKUAIHOUDU5_NOSEL_R	 ,  IDB_BITMAP_SHIKUAIHOUDU5_SELECT_R		},
		{IDC_CHWAVESPEEDADJUST_STANDARD	,IDB_BITMAP_BIAOCHENGSHENGSU_NOSEL_R	 ,  IDB_BITMAP_BIAOCHENGSHENGSU_SELECT_R		},
		{IDC_CHWAVESPEEDADJUST_START	,IDB_BITMAP_JIAOZHUNKAISHI5_NOSEL_R ,  IDB_BITMAP_JIAOZHUNKAISHI5_SELECT_R	},
		{IDC_CHWAVESPEEDADJUST_ADJUSTWAVESPEED	,IDB_BITMAP_JIAOZHUNSHENGSU_NOSEL_R ,  IDB_BITMAP_JIAOZHUNSHENGSU_SELECT_R	},
		{IDC_CHWAVESPEEDADJUST_ACCEPT	,IDB_BITMAP_JIESHOUJIAOZHUNJIEGUO5_NOSEL_R ,  IDB_BITMAP_JIESHOUJIAOZHUNJIEGUO5_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chWaveSpeedAdjustBtnBitmap);
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
static LRESULT CALLBACK btnChWaveSpeedAdjustThicknessProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					ShowWindow(hWndChAdjust,SW_SHOW);
					SetFocus(currentChAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChWaveSpeedAdjust);
					hWndChWaveSpeedAdjust = 0;

					gCurrentFocusBtn = currentChAdjustBtn;

					gCurrentWnd = hWndChAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chWaveSpeedAdjustBtnList[i] != currentChWaveSpeedAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChWaveSpeedAdjustBtn = chWaveSpeedAdjustBtnList[i];
					SetFocus(currentChWaveSpeedAdjustBtn);
					gCurrentFocusBtn = currentChWaveSpeedAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chWaveSpeedAdjustBtnList[i] != currentChWaveSpeedAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChWaveSpeedAdjustBtn = chWaveSpeedAdjustBtnList[i];
					SetFocus(currentChWaveSpeedAdjustBtn);
					gCurrentFocusBtn = currentChWaveSpeedAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if((gfSpeedAdjustThickness - fThicknessStep) >= 1)
						gfSpeedAdjustThickness -= fThicknessStep;
					else
						gfSpeedAdjustThickness = 1;

					displayThickness(Wnd, gfSpeedAdjustThickness);

					SetDistanceGate(1);
					SetDistanceGate(2);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfSpeedAdjustThickness + fThicknessStep < 3000)
					{
						gfSpeedAdjustThickness += fThicknessStep;

						displayThickness(Wnd, gfSpeedAdjustThickness);
					}
					
					SetDistanceGate(1);
					SetDistanceGate(2);
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
	return CallWindowProc(OldBtnChWaveSpeedAdjustThicknessProc,Wnd,Msg,wParam,lParam);
}
/**************************
*������ٰ�ť�Զ��崦�����
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChWaveSpeedAdjustStandardProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChAdjust,SW_SHOW);
					SetFocus(currentChAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChWaveSpeedAdjust);
					hWndChWaveSpeedAdjust = 0;

					gCurrentFocusBtn = currentChAdjustBtn;

					gCurrentWnd = hWndChAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chWaveSpeedAdjustBtnList[i] != currentChWaveSpeedAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChWaveSpeedAdjustBtn = chWaveSpeedAdjustBtnList[i];
					SetFocus(currentChWaveSpeedAdjustBtn);
					gCurrentFocusBtn = currentChWaveSpeedAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chWaveSpeedAdjustBtnList[i] != currentChWaveSpeedAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChWaveSpeedAdjustBtn = chWaveSpeedAdjustBtnList[i];
					SetFocus(currentChWaveSpeedAdjustBtn);
					gCurrentFocusBtn = currentChWaveSpeedAdjustBtn;
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
	return CallWindowProc(OldBtnChWaveSpeedAdjustStandardProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����У׼��ʼ��ť�Զ��崦�����
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChWaveSpeedAdjustStartProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChAdjust,SW_SHOW);
					SetFocus(currentChAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChWaveSpeedAdjust);
					hWndChWaveSpeedAdjust = 0;

					gCurrentFocusBtn = currentChAdjustBtn;

					gCurrentWnd = hWndChAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chWaveSpeedAdjustBtnList[i] != currentChWaveSpeedAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChWaveSpeedAdjustBtn = chWaveSpeedAdjustBtnList[i];
					SetFocus(currentChWaveSpeedAdjustBtn);
					gCurrentFocusBtn = currentChWaveSpeedAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chWaveSpeedAdjustBtnList[i] != currentChWaveSpeedAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChWaveSpeedAdjustBtn = chWaveSpeedAdjustBtnList[i];
					SetFocus(currentChWaveSpeedAdjustBtn);
					gCurrentFocusBtn = currentChWaveSpeedAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{	
					SetDistanceGate(1);
					SetDistanceGate(2);
					
					displayAdjustWaveSpeed(hBtnChWaveSpeedAdjustAdjustWaveSpeed, 0);//У׼���ǰ��ʾN/A

					memset(gszTextPeak, 0, sizeof(gszTextPeak));
					swprintf(gszTextPeak, _T("У׼������..."));

					SendMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);

					SetDistanceGate(1);
					SetDistanceGate(2);

					InitializeCriticalSection(&csAdjust);
					EnterCriticalSection(&csAdjust);
					gbChWaveSpeedAdjustStart = TRUE;
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
					if(currentChWaveSpeedAdjustBtn == hBtnChWaveSpeedAdjustStart)
					{
						SetFocus(currentChWaveSpeedAdjustBtn);
						displayAdjustWaveSpeed(hBtnChWaveSpeedAdjustAdjustWaveSpeed, gwAdjustWaveSpeed);
						//test
						//displayThickness(hBtnChWaveSpeedAdjustThickness,gPeakInfos[CurrentChannelNo][0].wDefectTime);
						//displayThickness(hBtnChWaveSpeedAdjustStandard,gPeakInfos[CurrentChannelNo][1].wDefectTime);
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
	return CallWindowProc(OldBtnChWaveSpeedAdjustStartProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����У׼���ٰ�ť�Զ��崦�����
* @ param    hInstance[in]:
* @ param    		message[in]:
* @ param    		wParam[in]:
* @ param    		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChWaveSpeedAdjustAdjustWaveSpeedProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChAdjust,SW_SHOW);
					SetFocus(currentChAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChWaveSpeedAdjust);
					hWndChWaveSpeedAdjust = 0;

					gCurrentFocusBtn = currentChAdjustBtn;

					gCurrentWnd = hWndChAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chWaveSpeedAdjustBtnList[i] != currentChWaveSpeedAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChWaveSpeedAdjustBtn = chWaveSpeedAdjustBtnList[i];
					SetFocus(currentChWaveSpeedAdjustBtn);
					gCurrentFocusBtn = currentChWaveSpeedAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chWaveSpeedAdjustBtnList[i] != currentChWaveSpeedAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChWaveSpeedAdjustBtn = chWaveSpeedAdjustBtnList[i];
					SetFocus(currentChWaveSpeedAdjustBtn);
					gCurrentFocusBtn = currentChWaveSpeedAdjustBtn;
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
	return CallWindowProc(OldBtnChWaveSpeedAdjustAdjustWaveSpeedProc,Wnd,Msg,wParam,lParam);
}
/**************************
*������������У׼�����ť�Զ��崦�����
* @ param    hInstance[in]:
* @ param    message[in]:
* @ param    wParam[in]:
* @ param    lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChWaveSpeedAdjustAcceptProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChAdjust,SW_SHOW);
					SetFocus(currentChAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChWaveSpeedAdjust);
					hWndChWaveSpeedAdjust = 0;

					gCurrentFocusBtn = currentChAdjustBtn;

					gCurrentWnd = hWndChAdjust;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chWaveSpeedAdjustBtnList[i] != currentChWaveSpeedAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChWaveSpeedAdjustBtn = chWaveSpeedAdjustBtnList[i];
					SetFocus(currentChWaveSpeedAdjustBtn);
					gCurrentFocusBtn = currentChWaveSpeedAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chWaveSpeedAdjustBtnList[i] != currentChWaveSpeedAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChWaveSpeedAdjustBtn = chWaveSpeedAdjustBtnList[i];
					SetFocus(currentChWaveSpeedAdjustBtn);
					gCurrentFocusBtn = currentChWaveSpeedAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
					if(gwAdjustWaveSpeed > 0)
					{
						if(!gbGuanLian)
							DisplaySet_ad[CurrentChannelNo].nWaveSpeed = gwAdjustWaveSpeed;
						else
						{
							DisplaySet_ad[0].nWaveSpeed = gwAdjustWaveSpeed;
							DisplaySet_ad[1].nWaveSpeed = gwAdjustWaveSpeed;
						}

						SetWaveSpeedLabelAndExecute(NULL);
						displayBiaoChengSpeed(hBtnChWaveSpeedAdjustStandard, DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
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
	return CallWindowProc(OldBtnChWaveSpeedAdjustAcceptProc,Wnd,Msg,wParam,lParam);
}
