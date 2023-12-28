/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ������������Դ�ļ�
 * Filename: quMianXiuZhengWin.cpp
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

HWND hWndQuMianXiuZheng = NULL;						//�����������ھ��
HWND hBtnQuMianXiuZhengOnOff = NULL;					//���ذ�ť���
HWND hBtnQuMianXiuZhengWaiJing = NULL;					//�ܲ��⾶��ť���
HWND hBtnQuMianXiuZhengNeiJing = NULL;					//�ܲ��ھ���ť���


HWND quMianXiuZhengBtnList[3] = {NULL, NULL, NULL};					//�����������ڵİ�ť��
WNDPROC OldBtnQuMianXiuZhengOnOffProc = NULL;	//���ذ�ťĬ�ϴ������
WNDPROC OldBtnQuMianXiuZhengWaiJingProc = NULL;		//�ܲ��⾶��ťĬ�ϴ������
WNDPROC OldBtnQuMianXiuZhengNeiJingProc = NULL;		//�ܲ��⾶��ťĬ�ϴ������

HWND currentQuMianXiuZhengBtn = NULL;					//�����������е�ǰѡ�еİ�ť

BOOL gbQuMianXiuZhengFlag = FALSE;			//�����������ر�־
float gfWaiJing = 10;						//�ܲ��⾶
float gfNeiJing = 5;						//�ܲ��ھ�
static WORD buttonNum = 3;					//��ť�ؼ���
static float fWaiJingStep = 1.0f;
static WORD wWaiJingStepIndex = 1;

ATOM registerQuMianXiuZhengChild(HINSTANCE hInstance);
static LRESULT CALLBACK quMianXiuZhengChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int quMianXiuZheng_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnQuMianXiuZhengOnOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnQuMianXiuZhengWaiJingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnQuMianXiuZhengNeiJingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//����ʵ�����
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;	
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;				//��ǰ��ʾ���Ӵ���	
extern HWND hWndSpecialFunction;		//ר�ù��ܴ��ھ��
extern HWND currentSpecialFunctionBtn;	//ר�ù��ܴ��е�ǰѡ�еİ�ť
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;					// ״̬�����
extern HWND hWndCurve;//���ߴ��ھ��
extern HWND currentCurveBtn;//���ߴ����е�ǰѡ�еİ�ť
extern BOOL gbHotRange;		// ��ݼ�����
extern BOOL gbHotDB;		// ��ݼ�����

/**************************
*��ʾ����
* @param		hWnd[in]:���ھ��
* @param		bSwitch[in]:����
* @return   LRESULT :
* @since    1.00
***************************/
static void displaySwitch(HWND hWnd ,BOOL bSwitch)
{
	if(bSwitch)
		DrawLableText(hWnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	else
		DrawLableText(hWnd, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);		
}
/**************************
*��ʾ�ܲ��⾶
* @param		hWnd[in]:���ھ��
* @param		fWaiJing[in]:�⾶ֵ
* @return   LRESULT :
* @since    1.00
***************************/
static void displayZhiJing(HWND hWnd ,float fWaiJing)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fWaiJing);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

/**************************
*ע��������������
* @QuMianXiuZheng    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerQuMianXiuZhengChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) quMianXiuZhengChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szQuMianXiuZhengChild;

	return RegisterClass(&wc);
}
/**************************
*�����������ڴ������
* @QuMianXiuZheng    hInstance[in]:
* @QuMianXiuZheng		message[in]:
* @QuMianXiuZheng		wParam[in]:
* @QuMianXiuZheng		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK quMianXiuZhengChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			hBtnQuMianXiuZhengWaiJing = CreateWindow(_T("BUTTON"),_T("waiJingQuMianXiuZheng"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUMIANXIUZHENG_WAIJING,hInst,NULL);

			hBtnQuMianXiuZhengNeiJing = CreateWindow(_T("BUTTON"),_T("waiJingQuMianXiuZheng"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUMIANXIUZHENG_NEIJING,hInst,NULL);

			hBtnQuMianXiuZhengOnOff = CreateWindow(_T("BUTTON"),_T("onOffQuMianXiuZheng"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
				0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUMIANXIUZHENG_ONOFF,hInst,NULL);
			
			quMianXiuZhengBtnList[0] = hBtnQuMianXiuZhengWaiJing;
			quMianXiuZhengBtnList[1] = hBtnQuMianXiuZhengNeiJing;
			quMianXiuZhengBtnList[2] = hBtnQuMianXiuZhengOnOff;
			
			//��ť�ؼ����Զ��崦�����
			
			OldBtnQuMianXiuZhengWaiJingProc = (WNDPROC) GetWindowLong(hBtnQuMianXiuZhengWaiJing,GWL_WNDPROC);
			SetWindowLong(hBtnQuMianXiuZhengWaiJing,GWL_WNDPROC,(LONG)btnQuMianXiuZhengWaiJingProc);
			
			OldBtnQuMianXiuZhengNeiJingProc = (WNDPROC) GetWindowLong(hBtnQuMianXiuZhengNeiJing,GWL_WNDPROC);
			SetWindowLong(hBtnQuMianXiuZhengNeiJing,GWL_WNDPROC,(LONG)btnQuMianXiuZhengNeiJingProc);
			
			OldBtnQuMianXiuZhengOnOffProc = (WNDPROC) GetWindowLong(hBtnQuMianXiuZhengOnOff,GWL_WNDPROC);
			SetWindowLong(hBtnQuMianXiuZhengOnOff,GWL_WNDPROC,(LONG)btnQuMianXiuZhengOnOffProc);
				
			currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[0];//Ĭ�ϵ�ǰѡ�п��ذ�ť			
			SetFocus(currentQuMianXiuZhengBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentQuMianXiuZhengBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndCurve,SW_SHOW);
				SetFocus(currentCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

				DestroyWindow(hWndQuMianXiuZheng);
				hWndQuMianXiuZheng = 0;
				
				gCurrentFocusBtn = currentCurveBtn;

				gCurrentWnd = hWndCurve;

				swprintf(gszTextStep, _T(""));
				PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			}
			break;
		case WM_COMMAND:
				
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			quMianXiuZheng_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnQuMianXiuZhengOnOff)
			{
				displaySwitch(pdis->hwndItem, gbQuMianXiuZhengFlag);
			}
			else if(pdis->hwndItem == hBtnQuMianXiuZhengWaiJing)
			{
				displayZhiJing(pdis->hwndItem,gfWaiJing);
			}
			else if(pdis->hwndItem == hBtnQuMianXiuZhengNeiJing)
			{
				displayZhiJing(pdis->hwndItem,gfNeiJing);
			}
			else
			{
				;//
			}

			// ��ⰴť�������ʾ��Ӧ�Ĳ���
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentQuMianXiuZhengBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnQuMianXiuZhengWaiJing)
						swprintf(gszTextStep, _T("%.1f"), fWaiJingStep);
					else if(pdis->hwndItem == hBtnQuMianXiuZhengNeiJing)
						swprintf(gszTextStep, _T("%.1f"), fWaiJingStep);
					else
						swprintf(gszTextStep, _T(""));

					PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
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
*���������������ڰ�ťͼƬ
* @QuMianXiuZheng    hInstance[in]:
* @QuMianXiuZheng		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int quMianXiuZheng_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal = 0;

	BTN_BMP_INFO tbl_QuMianXiuZhengBtnBitmap[]=
	{
		{IDC_QUMIANXIUZHENG_ONOFF	,IDB_BITMAP_KAIQIGUANBI4_NOSEL_R	 ,  IDB_BITMAP_KAIQIGUANBI4_SELECT_R		},
		{IDC_QUMIANXIUZHENG_WAIJING	,IDB_BITMAP_GUANCAIWAIJING_NOSEL_R	 ,  IDB_BITMAP_GUANCAIWAIJING_SELECT_R		},
		{IDC_QUMIANXIUZHENG_NEIJING	,IDB_BITMAP_GUANCAINEIJING_NOSEL_R	 ,  IDB_BITMAP_GUANCAINEIJING_SEL_R		},
		{	-1			,	-1					 ,	-1							}    
	};
		
	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_QuMianXiuZhengBtnBitmap);
		
	return ReturnVal;
}
/**************************
*���ذ�ť�Զ��崦�����
* @QuMianXiuZheng    hInstance[in]:
* @QuMianXiuZheng		message[in]:
* @QuMianXiuZheng		wParam[in]:
* @QuMianXiuZheng		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnQuMianXiuZhengOnOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(quMianXiuZhengBtnList[i] != currentQuMianXiuZhengBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[i];
					SetFocus(currentQuMianXiuZhengBtn);
					gCurrentFocusBtn = currentQuMianXiuZhengBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(quMianXiuZhengBtnList[i] != currentQuMianXiuZhengBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[i];
					SetFocus(currentQuMianXiuZhengBtn);
					gCurrentFocusBtn = currentQuMianXiuZhengBtn;
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{	
					gbQuMianXiuZhengFlag = !gbQuMianXiuZhengFlag;

					displaySwitch(Wnd, gbQuMianXiuZhengFlag);
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
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnQuMianXiuZhengOnOffProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�ܲ��⾶��ť�Զ��崦�����
* @QuMianXiuZheng    hInstance[in]:
* @QuMianXiuZheng		message[in]:
* @QuMianXiuZheng		wParam[in]:
* @QuMianXiuZheng		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnQuMianXiuZhengWaiJingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), fWaiJingStep);
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
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(quMianXiuZhengBtnList[i] != currentQuMianXiuZhengBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[i];
					SetFocus(currentQuMianXiuZhengBtn);
					gCurrentFocusBtn = currentQuMianXiuZhengBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(quMianXiuZhengBtnList[i] != currentQuMianXiuZhengBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[i];
					SetFocus(currentQuMianXiuZhengBtn);
					gCurrentFocusBtn = currentQuMianXiuZhengBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfWaiJing - fWaiJingStep >= MIN_GUANCAIWAIJING)
						gfWaiJing = gfWaiJing - fWaiJingStep;
					else
						gfWaiJing = MIN_GUANCAIWAIJING;
					displayZhiJing(Wnd,gfWaiJing);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfWaiJing + fWaiJingStep <= MAX_GUANCAIWAIJING)
						gfWaiJing = gfWaiJing + fWaiJingStep;
					else
						gfWaiJing = MAX_GUANCAIWAIJING;
					displayZhiJing(Wnd,gfWaiJing);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == 'F')
				{
					if(wWaiJingStepIndex < 3)
						wWaiJingStepIndex++;
					else
						wWaiJingStepIndex = 0;

					fWaiJingStep = AMENDSTEPS[wWaiJingStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fWaiJingStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnQuMianXiuZhengWaiJingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�ܲ��ھ���ť�Զ��崦�����
* @QuMianXiuZheng    hInstance[in]:
* @QuMianXiuZheng		message[in]:
* @QuMianXiuZheng		wParam[in]:
* @QuMianXiuZheng		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnQuMianXiuZhengNeiJingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(quMianXiuZhengBtnList[i] != currentQuMianXiuZhengBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[i];
					SetFocus(currentQuMianXiuZhengBtn);
					gCurrentFocusBtn = currentQuMianXiuZhengBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(quMianXiuZhengBtnList[i] != currentQuMianXiuZhengBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuMianXiuZhengBtn = quMianXiuZhengBtnList[i];
					SetFocus(currentQuMianXiuZhengBtn);
					gCurrentFocusBtn = currentQuMianXiuZhengBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfNeiJing - fWaiJingStep >= MIN_GUANCAIWAIJING)
						gfNeiJing = gfNeiJing - fWaiJingStep;
					else
						gfNeiJing = MIN_GUANCAIWAIJING;
					displayZhiJing(Wnd,gfNeiJing);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfNeiJing + fWaiJingStep <= gfWaiJing)
						gfNeiJing = gfNeiJing + fWaiJingStep;
					else
						gfNeiJing = gfWaiJing - 0.1f;
					displayZhiJing(Wnd,gfNeiJing);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == 'F')
				{
					if(wWaiJingStepIndex < 3)
						wWaiJingStepIndex++;
					else
						wWaiJingStepIndex = 0;

					fWaiJingStep = AMENDSTEPS[wWaiJingStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fWaiJingStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnQuMianXiuZhengNeiJingProc,Wnd,Msg,wParam,lParam);
}

