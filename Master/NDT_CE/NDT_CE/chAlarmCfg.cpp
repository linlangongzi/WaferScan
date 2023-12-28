/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ��񱨾��趨����Դ�ļ�
 * Filename: chAlarmCfgWin.cpp
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


HWND hWndChAlarmCfg = NULL;						//��񱨾��趨���ھ��
HWND currentChAlarmCfgBtn = NULL;					//��񱨾��趨���ڵ�ǰѡ�еİ�ť

MEASURE_ALARM stMeasureAlarm[10];				//��������
CRITICAL_SECTION csMeasureAlarm;

HANDLE hDrawMeasureAlarmThread = NULL;				// ���������߳̾��
DRAWTHREAD_PARAM DrawMeasureAlarmThread_param;		// ���������̲߳���
DWORD dwDrawMeasureAlarmThreadID = 0;				// ���������߳�ID

static HWND hBtnChAlarmCfgNumber = NULL;				//�����Ű�ť���
static HWND hBtnChAlarmCfgMode = NULL;			//����ģʽ��ť���
static HWND hBtnChAlarmCfgChaoChaYuZhi = NULL;			//������ֵ��ť���
static HWND hBtnChAlarmCfgSwitch = NULL;				//��/�ذ�ť���
static HWND chAlarmCfgBtnList[4] ={NULL,NULL,NULL,NULL};					//��񱨾��趨���ڵİ�ť��
static WNDPROC OldBtnChAlarmCfgNumberProc = NULL;//�����Ű�ťĬ�ϴ������
static WNDPROC OldBtnChAlarmCfgModeProc = NULL;		//����ģʽ��ťĬ�ϴ������
static WNDPROC OldBtnChAlarmCfgChaoChaYuZhiProc = NULL;		//������ֵ��ťĬ�ϴ������
static WNDPROC OldBtnChAlarmCfgSwitchProc = NULL;			//��/�ذ�ťĬ�ϴ������

static const int buttonNum = 4;				//��ť�ؼ���
static WORD wAlarmIndex = 0;			//��������
static float fChaoChaYuZhiStep = 0;		//������ֵ����
static WORD wChaoChaYuZhiStepIndex = 1;	//������ֵ��������

ATOM registerChAlarmCfgChild(HINSTANCE hInstance);
static LRESULT CALLBACK chAlarmCfgChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chAlarmCfg_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChAlarmCfgNumberProc( HWND Wnd, 
													UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChAlarmCfgModeProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChAlarmCfgChaoChaYuZhiProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChAlarmCfgSwitchProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//����ʵ�����

//extern HWND hWndSwitch;			//��/�ش��ھ��
//extern HWND hWndChaoChaYuZhi;		//������ֵ���ھ�� 
extern HWND gCurrentWnd;
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndChMeasureCfg;						//���������ô��ھ��
extern HWND currentChMeasureCfgBtn;					//���������ô��ڵ�ǰѡ�еİ�ť
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;
extern ARS200FPGA_SYS_REG* sys_reg;
extern CRITICAL_SECTION csMeasure;
extern BOOL gbMeasureSwitch;
extern MEASURE_DISPLAY_ALL gstMeasureDisplay;
extern BOOL gbAlarmFlag;
/**************************
*��ʾ����ģʽ
* @param		hWnd[in]:���ھ��
* @param		fMax[in]:���ֵ
* @return   LRESULT :
* @since    1.00
***************************/
void displayAlarmMode(HWND hWnd ,WORD wMode)
{
	if(wMode == 0)
		DrawLableText(hWnd, _T("���ֵ����"), 18, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	if(wMode == 1)
		DrawLableText(hWnd, _T("��Сֵ����"), 18, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	if(wMode == 2)
		DrawLableText(hWnd, _T("��ֵ����"), 18, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);		
}
/**************************
*��ʾ������ֵ
* @param		hWnd[in]:���ھ��
* @param		fMax[in]:���ֵ
* @return   LRESULT :
* @since    1.00
***************************/
void displayChaoChaYuZhi(HWND hWnd ,float fValue)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	if(stMeasureAlarm[wAlarmIndex].wAlarmMode == 2)
	{
		if(gstMeasureDisplay.wChaZhiMode == 1)
			wsprintf(strText, _T("%.1f%c"), fValue, '%');
		else if(gstMeasureDisplay.wChaZhiMode == 0)
			wsprintf(strText, _T("%.1f"), fValue);
		else
		{
			;//
		}
	}
	else
		wsprintf(strText, _T("%.1f"), fValue);
	
	DrawLableText(hWnd, strText, 25, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}
/**************************
*��ʾ��������
* @param		hWnd[in]:���ھ��
* @param		fMax[in]:���ֵ
* @return   LRESULT :
* @since    1.00
***************************/
void displayAlarmSwitch(HWND hWnd ,BOOL bSwitch)
{
	if(bSwitch == ON)
		DrawLableText(hWnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	else
		DrawLableText(hWnd, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

/**************************
*��ʾ������
* @param		hWnd[in]:���ھ��
* @param		fMax[in]:���ֵ
* @return   LRESULT :
* @since    1.00
***************************/
void displayAlarmNum(HWND hWnd ,WORD wNum)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%d"), wNum + 1);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);

	displayAlarmMode(hBtnChAlarmCfgMode, stMeasureAlarm[wNum].wAlarmMode);
	displayChaoChaYuZhi(hBtnChAlarmCfgChaoChaYuZhi, stMeasureAlarm[wNum].fChaoChaYuZhi);
	displayAlarmSwitch(hBtnChAlarmCfgSwitch, stMeasureAlarm[wNum].bSwitch);
}
/**************************
*��������������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
DWORD WINAPI measureAlarmProc(LPVOID lpParameter)
{
	BOOL bAlarmSwitch[10];
	
	while(1)
	{
		BOOL bSwitch = OFF;
		
		InitializeCriticalSection(&csMeasure);
		EnterCriticalSection(&csMeasure);
		
		BOOL bMeasureSwitch = gbMeasureSwitch;
		
		LeaveCriticalSection(&csMeasure);
		DeleteCriticalSection(&csMeasure);

		InitializeCriticalSection(&csMeasureAlarm);
		EnterCriticalSection(&csMeasureAlarm);

		for(int i = 0; i < 10; i++)
		{
			bAlarmSwitch[i] = stMeasureAlarm[i].bSwitch;
		}
		
		LeaveCriticalSection(&csMeasureAlarm);
		DeleteCriticalSection(&csMeasureAlarm);

		for(int i = 0; i < 10; i++)
		{
			if(bAlarmSwitch[i] == ON)
				bSwitch = ON;
		}
		if(bSwitch == ON)
		{
			if(bMeasureSwitch == ON)
			{
				for(int i = 0; i < 10; i++)
				{
					if(bAlarmSwitch[i] == ON)
					{
						if(stMeasureAlarm[i].wAlarmMode == 0)				//���ֵ����
						{
							if(gstMeasureDisplay.stMax.bSwitch == ON)
							{	
								if(stMeasureAlarm[i].fChaoChaYuZhi > gstMeasureDisplay.stMax.fValue)
								{
									if(gbAlarmFlag)
									{					
										sys_reg->wLedAlarm |= 0x1;
										//MessageBeep(0xFFFFFFFF);
										PlaySound(_T("SystemStart"),NULL,SND_ALIAS|SND_ASYNC); 
									}
									else
									{
										sys_reg->wLedAlarm &= 0xFFFE;			
									}
								}
							}
						}
						else if(stMeasureAlarm[i].wAlarmMode == 1)		//��Сֵ����
						{
							if(gstMeasureDisplay.stMin.bSwitch == ON)
							{
								if(stMeasureAlarm[i].fChaoChaYuZhi < gstMeasureDisplay.stMin.fValue)
								{
									if(gbAlarmFlag)
									{					
										sys_reg->wLedAlarm |= 0x1;
										//MessageBeep(0xFFFFFFFF);
										PlaySound(_T("SystemStart"),NULL,SND_ALIAS|SND_ASYNC); 
									}
									else
									{
										sys_reg->wLedAlarm &= 0xFFFE;			
									}
								}
							}
						}
						else if(stMeasureAlarm[i].wAlarmMode == 2)		//��ֵ����
						{
							if(gstMeasureDisplay.stChaZhi.bSwitch== ON)
							{
								if(stMeasureAlarm[i].fChaoChaYuZhi > (float)fabs((double)gstMeasureDisplay.stChaZhi.fValue))
								{
									if(gbAlarmFlag)
									{					
										sys_reg->wLedAlarm |= 0x1;
										//MessageBeep(0xFFFFFFFF);
										PlaySound(_T("SystemStart"),NULL,SND_ALIAS|SND_ASYNC); 
									}
									else
									{
										sys_reg->wLedAlarm &= 0xFFFE;			
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			hDrawMeasureAlarmThread = NULL;
			return 0;
		}
		Sleep(20);
	}
	return 0;
}
/**************************
*ע���񱨾��趨����
* @chAlarmCfg    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChAlarmCfgChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chAlarmCfgChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChAlarmCfgChild;

	return RegisterClass(&wc);
}
/**************************
*��񱨾��趨���ڴ������
* @chAlarmCfg    hInstance[in]:
* @chAlarmCfg		message[in]:
* @chAlarmCfg		wParam[in]:
* @chAlarmCfg		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chAlarmCfgChildProc(HWND hWnd, UINT message, 
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
			fChaoChaYuZhiStep = AMENDSTEPS[wChaoChaYuZhiStepIndex];
			//����ť�ؼ�
			hBtnChAlarmCfgNumber = CreateWindow(_T("BUTTON"),_T("numberChAlarmCfg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHALARMCFG_NUMBER,hInst,NULL);
			hBtnChAlarmCfgMode = CreateWindow(_T("BUTTON"),_T("modeChAlarmCfg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHALARMCFG_MODE,hInst,NULL);
			hBtnChAlarmCfgChaoChaYuZhi = CreateWindow(_T("BUTTON"),_T("chaoChaYuZhiChAlarmCfg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHALARMCFG_CHAOCHAYUZHI,hInst,NULL);
			hBtnChAlarmCfgSwitch = CreateWindow(_T("BUTTON"),_T("switchChAlarmCfg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 3*BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHALARMCFG_SWITCH,hInst,NULL);
					
			chAlarmCfgBtnList[0] = hBtnChAlarmCfgNumber;
			chAlarmCfgBtnList[1] = hBtnChAlarmCfgMode;
			chAlarmCfgBtnList[2] = hBtnChAlarmCfgChaoChaYuZhi;
			chAlarmCfgBtnList[3] = hBtnChAlarmCfgSwitch;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnChAlarmCfgNumberProc = (WNDPROC) GetWindowLong(hBtnChAlarmCfgNumber,GWL_WNDPROC);
			SetWindowLong(hBtnChAlarmCfgNumber,GWL_WNDPROC,(LONG)btnChAlarmCfgNumberProc);
			
			OldBtnChAlarmCfgModeProc = (WNDPROC) GetWindowLong(hBtnChAlarmCfgMode,GWL_WNDPROC);
			SetWindowLong(hBtnChAlarmCfgMode,GWL_WNDPROC,(LONG)btnChAlarmCfgModeProc);
			
			OldBtnChAlarmCfgChaoChaYuZhiProc = (WNDPROC) GetWindowLong(hBtnChAlarmCfgChaoChaYuZhi,GWL_WNDPROC);
			SetWindowLong(hBtnChAlarmCfgChaoChaYuZhi,GWL_WNDPROC,(LONG)btnChAlarmCfgChaoChaYuZhiProc);
			
			OldBtnChAlarmCfgSwitchProc = (WNDPROC) GetWindowLong(hBtnChAlarmCfgSwitch,GWL_WNDPROC);
			SetWindowLong(hBtnChAlarmCfgSwitch,GWL_WNDPROC,(LONG)btnChAlarmCfgSwitchProc);
		
			currentChAlarmCfgBtn = chAlarmCfgBtnList[0];//Ĭ�ϵ�ǰѡ�б����Ű�ť
			SetFocus(currentChAlarmCfgBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentChAlarmCfgBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
		/*		if(LOWORD(wParam) == IDC_CHALARMCFG_CHAOCHAYUZHI)
				{
					currentChAlarmCfgBtn = chAlarmCfgBtnList[2]; 
					SetFocus(currentChAlarmCfgBtn);
					gCurrentFocusBtn = currentChAlarmCfgBtn;
					hWndChaoChaYuZhi = CreateWindow(szChaoChaYuZhiChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChAlarmCfg,SW_HIDE);
					//SetFocus(hWndChaoChaYuZhi);
					ShowWindow(hWndChaoChaYuZhi,SW_SHOW);
					UpdateWindow(hWndChaoChaYuZhi);
				}
				else if(LOWORD(wParam) == IDC_CHALARMCFG_SWITCH)
				{
				//	currentChAlarmCfgBtn = chAlarmCfgBtnList[3]; 
				//	SetFocus(currentChAlarmCfgBtn);
				//	gCurrentFocusBtn = currentChAlarmCfgBtn;
				}
			*/
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			chAlarmCfg_DrawButtonBitmap(hInst,pdis);
			if(pdis->hwndItem == hBtnChAlarmCfgNumber)
			{
				displayAlarmNum(pdis->hwndItem, wAlarmIndex);
			}
			if(pdis->hwndItem == hBtnChAlarmCfgMode)
			{
				displayAlarmMode(pdis->hwndItem, stMeasureAlarm[wAlarmIndex].wAlarmMode);
			}
			if(pdis->hwndItem == hBtnChAlarmCfgChaoChaYuZhi)
			{
				displayChaoChaYuZhi(pdis->hwndItem, stMeasureAlarm[wAlarmIndex].fChaoChaYuZhi);
			}
			if(pdis->hwndItem == hBtnChAlarmCfgSwitch)
			{
				displayAlarmSwitch(pdis->hwndItem, stMeasureAlarm[wAlarmIndex].bSwitch);
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
*���ز�񱨾��趨���ڰ�ťͼƬ
* @chAlarmCfg    hInstance[in]:
* @chAlarmCfg		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chAlarmCfg_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chAlarmCfgBtnBitmap[]=
	{
		{IDC_CHALARMCFG_NUMBER	,IDB_BITMAP_BAOJINGHAO_NOSEL_R	 ,  IDB_BITMAP_BAOJINGHAO_SELECT_R		},
		{IDC_CHALARMCFG_MODE	,IDB_BITMAP_BAOJINGMOSHI4_NOSEL_R	 ,  IDB_BITMAP_BAOJINGMOSHI4_SELECT_R		},
		{IDC_CHALARMCFG_CHAOCHAYUZHI	,IDB_BITMAP_CHAOCHAYUZHI_NOSEL_R ,  IDB_BITMAP_CHAOCHAYUZHI_SELECT_R	},
		{IDC_CHALARMCFG_SWITCH	,IDB_BITMAP_KAIQIGUANBI4_NOSEL_R ,  IDB_BITMAP_KAIQIGUANBI4_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chAlarmCfgBtnBitmap);
	return ReturnVal;
}
/**************************
*�����Ű�ť�Զ��崦�����
* @chAlarmCfg    hInstance[in]:
* @chAlarmCfg		message[in]:
* @chAlarmCfg		wParam[in]:
* @chAlarmCfg		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAlarmCfgNumberProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChMeasureCfg,SW_SHOW);
					SetFocus(currentChMeasureCfgBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChAlarmCfg);
					hWndChAlarmCfg = 0;
				
					gCurrentFocusBtn = currentChMeasureCfgBtn;

					gCurrentWnd = hWndChMeasureCfg;
				}
			
				else if(wParam == VK_UP)
				{
					while(chAlarmCfgBtnList[i] != currentChAlarmCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAlarmCfgBtn = chAlarmCfgBtnList[i];
					SetFocus(currentChAlarmCfgBtn);
					gCurrentFocusBtn = currentChAlarmCfgBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAlarmCfgBtnList[i] != currentChAlarmCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAlarmCfgBtn = chAlarmCfgBtnList[i];
					SetFocus(currentChAlarmCfgBtn);
					gCurrentFocusBtn = currentChAlarmCfgBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(wAlarmIndex > 0)
						wAlarmIndex--;
					else
						wAlarmIndex = 9;

					displayAlarmNum(Wnd, wAlarmIndex);
				}
				else if(wParam == VK_RIGHT)
				{
					if(wAlarmIndex < 9)
						wAlarmIndex++;
					else
						wAlarmIndex = 0;

					displayAlarmNum(Wnd, wAlarmIndex);
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
	return CallWindowProc(OldBtnChAlarmCfgNumberProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����ģʽ��ť�Զ��崦�����
* @chAlarmCfg    hInstance[in]:
* @chAlarmCfg		message[in]:
* @chAlarmCfg		wParam[in]:
* @chAlarmCfg		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAlarmCfgModeProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChMeasureCfg,SW_SHOW);
					SetFocus(currentChMeasureCfgBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChAlarmCfg);
					hWndChAlarmCfg = 0;
				
					gCurrentFocusBtn = currentChMeasureCfgBtn;

					gCurrentWnd = hWndChMeasureCfg;
				}
			
				else if(wParam == VK_UP)
				{
					while(chAlarmCfgBtnList[i] != currentChAlarmCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAlarmCfgBtn = chAlarmCfgBtnList[i];
					SetFocus(currentChAlarmCfgBtn);
					gCurrentFocusBtn = currentChAlarmCfgBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAlarmCfgBtnList[i] != currentChAlarmCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAlarmCfgBtn = chAlarmCfgBtnList[i];
					SetFocus(currentChAlarmCfgBtn);
					gCurrentFocusBtn = currentChAlarmCfgBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(stMeasureAlarm[wAlarmIndex].wAlarmMode > 0)
						stMeasureAlarm[wAlarmIndex].wAlarmMode--;
					else
						stMeasureAlarm[wAlarmIndex].wAlarmMode = 2;

					displayAlarmMode(Wnd, stMeasureAlarm[wAlarmIndex].wAlarmMode);
					displayChaoChaYuZhi(hBtnChAlarmCfgChaoChaYuZhi, stMeasureAlarm[wAlarmIndex].fChaoChaYuZhi);
				}
				else if(wParam == VK_RIGHT)
				{
					if(stMeasureAlarm[wAlarmIndex].wAlarmMode < 2)
						stMeasureAlarm[wAlarmIndex].wAlarmMode++;
					else
						stMeasureAlarm[wAlarmIndex].wAlarmMode = 0;

					displayAlarmMode(Wnd, stMeasureAlarm[wAlarmIndex].wAlarmMode);
					displayChaoChaYuZhi(hBtnChAlarmCfgChaoChaYuZhi, stMeasureAlarm[wAlarmIndex].fChaoChaYuZhi);
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
	return CallWindowProc(OldBtnChAlarmCfgModeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*������ֵ��ť�Զ��崦�����
* @chAlarmCfg    hInstance[in]:
* @chAlarmCfg		message[in]:
* @chAlarmCfg		wParam[in]:
* @chAlarmCfg		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAlarmCfgChaoChaYuZhiProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChMeasureCfg,SW_SHOW);
					SetFocus(currentChMeasureCfgBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChAlarmCfg);
					hWndChAlarmCfg = 0;
				
					gCurrentFocusBtn = currentChMeasureCfgBtn;

					gCurrentWnd = hWndChMeasureCfg;
				}
			
				else if(wParam == VK_UP)
				{
					while(chAlarmCfgBtnList[i] != currentChAlarmCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAlarmCfgBtn = chAlarmCfgBtnList[i];
					SetFocus(currentChAlarmCfgBtn);
					gCurrentFocusBtn = currentChAlarmCfgBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAlarmCfgBtnList[i] != currentChAlarmCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAlarmCfgBtn = chAlarmCfgBtnList[i];
					SetFocus(currentChAlarmCfgBtn);
					gCurrentFocusBtn = currentChAlarmCfgBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if((stMeasureAlarm[wAlarmIndex].fChaoChaYuZhi - fChaoChaYuZhiStep) >= 0)
						stMeasureAlarm[wAlarmIndex].fChaoChaYuZhi -= fChaoChaYuZhiStep;
					else
						stMeasureAlarm[wAlarmIndex].fChaoChaYuZhi = 0;

					displayChaoChaYuZhi(Wnd, stMeasureAlarm[wAlarmIndex].fChaoChaYuZhi);

					if(gbMeasureSwitch == ON)
					{
						for(int i = 0; i < 10; i++)
						{
							if(stMeasureAlarm[i].bSwitch  == ON)
							{
								if(stMeasureAlarm[i].wAlarmMode == 0)				//���ֵ����
								{
									if(gstMeasureDisplay.stMax.bSwitch == ON)
									{	
										if(stMeasureAlarm[i].fChaoChaYuZhi > gstMeasureDisplay.stMax.fValue)
										{
											if(gbAlarmFlag)
											{					
												sys_reg->wLedAlarm |= 0x1;
												//MessageBeep(0xFFFFFFFF);
												PlaySound(_T("SystemStart"),NULL,SND_ALIAS|SND_ASYNC); 
											}
											else
											{
												sys_reg->wLedAlarm &= 0xFFFE;			
											}
										}
									}
								}
								else if(stMeasureAlarm[i].wAlarmMode == 1)		//��Сֵ����
								{
									if(gstMeasureDisplay.stMin.bSwitch == ON)
									{
										if(stMeasureAlarm[i].fChaoChaYuZhi < gstMeasureDisplay.stMin.fValue)
										{
											if(gbAlarmFlag)
											{					
												sys_reg->wLedAlarm |= 0x1;
												//MessageBeep(0xFFFFFFFF);
												PlaySound(_T("SystemStart"),NULL,SND_ALIAS|SND_ASYNC); 
											}
											else
											{
												sys_reg->wLedAlarm &= 0xFFFE;			
											}
										}
									}
								}
								else if(stMeasureAlarm[i].wAlarmMode == 2)		//��ֵ����
								{
									if(gstMeasureDisplay.stChaZhi.bSwitch== ON)
									{
										if(stMeasureAlarm[i].fChaoChaYuZhi > (float)fabs((double)gstMeasureDisplay.stChaZhi.fValue))
										{
											if(gbAlarmFlag)
											{					
												sys_reg->wLedAlarm |= 0x1;
												//MessageBeep(0xFFFFFFFF);
												PlaySound(_T("SystemStart"),NULL,SND_ALIAS|SND_ASYNC); 
											}
											else
											{
												sys_reg->wLedAlarm &= 0xFFFE;			
											}
										}
									}
								}
							}
						}
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(stMeasureAlarm[wAlarmIndex].fChaoChaYuZhi + fChaoChaYuZhiStep < 3000)
					{
						stMeasureAlarm[wAlarmIndex].fChaoChaYuZhi += fChaoChaYuZhiStep;

						displayChaoChaYuZhi(Wnd, stMeasureAlarm[wAlarmIndex].fChaoChaYuZhi);
					}

					if(gbMeasureSwitch == ON)
					{
						for(int i = 0; i < 10; i++)
						{
							if(stMeasureAlarm[i].bSwitch == ON)
							{
								if(stMeasureAlarm[i].wAlarmMode == 0)				//���ֵ����
								{
									if(gstMeasureDisplay.stMax.bSwitch == ON)
									{	
										if(stMeasureAlarm[i].fChaoChaYuZhi > gstMeasureDisplay.stMax.fValue)
										{
											if(gbAlarmFlag)
											{					
												sys_reg->wLedAlarm |= 0x1;
												//MessageBeep(0xFFFFFFFF);
												PlaySound(_T("SystemStart"),NULL,SND_ALIAS|SND_ASYNC); 
											}
											else
											{
												sys_reg->wLedAlarm &= 0xFFFE;			
											}
										}
									}
								}
								else if(stMeasureAlarm[i].wAlarmMode == 1)		//��Сֵ����
								{
									if(gstMeasureDisplay.stMin.bSwitch == ON)
									{
										if(stMeasureAlarm[i].fChaoChaYuZhi < gstMeasureDisplay.stMin.fValue)
										{
											if(gbAlarmFlag)
											{					
												sys_reg->wLedAlarm |= 0x1;
												//MessageBeep(0xFFFFFFFF);
												PlaySound(_T("SystemStart"),NULL,SND_ALIAS|SND_ASYNC); 
											}
											else
											{
												sys_reg->wLedAlarm &= 0xFFFE;			
											}
										}
									}
								}
								else if(stMeasureAlarm[i].wAlarmMode == 2)		//��ֵ����
								{
									if(gstMeasureDisplay.stChaZhi.bSwitch== ON)
									{
										if(stMeasureAlarm[i].fChaoChaYuZhi > (float)fabs((double)gstMeasureDisplay.stChaZhi.fValue))
										{
											if(gbAlarmFlag)
											{					
												sys_reg->wLedAlarm |= 0x1;
												//MessageBeep(0xFFFFFFFF);
												PlaySound(_T("SystemStart"),NULL,SND_ALIAS|SND_ASYNC); 
											}
											else
											{
												sys_reg->wLedAlarm &= 0xFFFE;			
											}
										}
									}
								}
							}
						}
					}
				}
				else if(wParam == 'F')
				{
					if(wChaoChaYuZhiStepIndex < 3)
						wChaoChaYuZhiStepIndex++;
					else
						wChaoChaYuZhiStepIndex = 0;

					fChaoChaYuZhiStep = AMENDSTEPS[wChaoChaYuZhiStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fChaoChaYuZhiStep);
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
	return CallWindowProc(OldBtnChAlarmCfgChaoChaYuZhiProc,Wnd,Msg,wParam,lParam);
}
/**************************
*��/�ذ�ť�Զ��崦�����
* @chAlarmCfg    hInstance[in]:
* @chAlarmCfg		message[in]:
* @chAlarmCfg		wParam[in]:
* @chAlarmCfg		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAlarmCfgSwitchProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChMeasureCfg,SW_SHOW);
					SetFocus(currentChMeasureCfgBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChAlarmCfg);
					hWndChAlarmCfg = 0;
				
					gCurrentFocusBtn = currentChMeasureCfgBtn;

					gCurrentWnd = hWndChMeasureCfg;
				}
			
				else if(wParam == VK_UP)
				{
					while(chAlarmCfgBtnList[i] != currentChAlarmCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAlarmCfgBtn = chAlarmCfgBtnList[i];
					SetFocus(currentChAlarmCfgBtn);
					gCurrentFocusBtn = currentChAlarmCfgBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAlarmCfgBtnList[i] != currentChAlarmCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAlarmCfgBtn = chAlarmCfgBtnList[i];
					SetFocus(currentChAlarmCfgBtn);
					gCurrentFocusBtn = currentChAlarmCfgBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					BOOL bAlarmSwitch = OFF;
					InitializeCriticalSection(&csMeasureAlarm);
					EnterCriticalSection(&csMeasureAlarm);
					
					stMeasureAlarm[wAlarmIndex].bSwitch = !stMeasureAlarm[wAlarmIndex].bSwitch;
					
					LeaveCriticalSection(&csMeasureAlarm);
					DeleteCriticalSection(&csMeasureAlarm);

					displayAlarmSwitch(Wnd, stMeasureAlarm[wAlarmIndex].bSwitch);

					for(int i = 0; i < 10; i++)
					{
						if(stMeasureAlarm[i].bSwitch == ON)
						{
							bAlarmSwitch = ON;
						}
					}

					if(bAlarmSwitch == ON)
					{						
						// ���������߳�
						if(hDrawMeasureAlarmThread == NULL)
						{
							// �����߳�							
							DrawMeasureAlarmThread_param.hWnd = hWndMain;
							HDC hDC = GetDC(hWndMain);
							DrawMeasureAlarmThread_param.hDC = hDC;

							hDrawMeasureAlarmThread = ::CreateThread(NULL, 0, measureAlarmProc, (LPVOID)&DrawMeasureAlarmThread_param, 0, &dwDrawMeasureAlarmThreadID);
						}
					}

					if(stMeasureAlarm[wAlarmIndex].bSwitch == OFF)
						sys_reg->wLedAlarm &= 0xFFFE;
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
	return CallWindowProc(OldBtnChAlarmCfgSwitchProc,Wnd,Msg,wParam,lParam);
}
