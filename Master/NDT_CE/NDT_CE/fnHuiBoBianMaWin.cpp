/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * �ز����봰��Դ�ļ�
 * Filename: huiBoBianMaWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnHuiBoBianMaWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "const.h"


HWND hWndHuiBoBianMa = NULL;			//�ز����봰�ھ��
HWND hBtnHuiBoBianMaSelect = NULL;		//�ز�����ѡ��ť���
HWND hBtnHuiBoBianMaGongJianHouDu = NULL;		//������Ȱ�ť���
HWND hBtnHuiBoBianMaOnOrOff = NULL;	//����/�رհ�ť���

HWND huiBoBianMaBtnList[3] = {NULL,NULL,NULL};						//�ز����봰�ڵİ�ť��
WNDPROC OldBtnHuiBoBianMaSelectProc = NULL;		//�ز�����ѡ��ťĬ�ϴ������
WNDPROC OldBtnHuiBoBianMaGongJianHouDuProc = NULL;		//������Ȱ�ťĬ�ϴ������
WNDPROC OldBtnHuiBoBianMaOnOrOffProc = NULL;	//����/�رհ�ťĬ�ϴ������

HWND currentHuiBoBianMaBtn = NULL;		//�ز����봰�е�ǰѡ�еİ�ť

static const int buttonNum = 3;		//��ť�ؼ���

extern HINSTANCE hInst;			//����ʵ�����

extern HWND hWndMain;
extern HWND hWndSpecialDisplay;
extern HWND currentSpecialDisplayBtn;
extern HWND gCurrentFocusBtn;

// songchenguang add start 2010-06-28
static WORD nHuiBoBianMaNo = 1;		// �ز������
BOOL gbHuiBoOpen[10] = {FALSE,FALSE,FALSE,FALSE,FALSE,
						FALSE,FALSE,FALSE,FALSE,FALSE};	// �ز����뿪��
//float gfHuiBoWidth = 10.0f;		// �ز����
extern float gfPlateHeight;

CRITICAL_SECTION csHuiBoState;
CRITICAL_SECTION csHuiBoValue;

static float fHuiBoStep = 1.0f; 
//extern float fRangeStep;
static WORD wHuiBoStepIndex = 0;
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;
extern BOOL gbHotDB;			// ��ݼ�����
extern BOOL gbHotRange;			// ��ݼ�����
// songchenguang add end 2010-06-28
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���

/**************************
*ע��ز����봰��
* @huiBoBianMa    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerHuiBoBianMaChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) huiBoBianMaChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szHuiBoBianMaChild;

	return RegisterClass(&wc);
}

void SetHuiBoNo(HWND hWnd, const WORD& nHuiBoNo)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%d"), nHuiBoNo);
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

void SetWidth(HWND hWnd, const float& fWidth)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fWidth);
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}


void SetHuiBoOpenFlag(HWND hWnd, const WORD& nHuiBoNo)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	InitializeCriticalSection(&csHuiBoState);
	EnterCriticalSection(&csHuiBoState);

	if(gbHuiBoOpen[nHuiBoNo -1])
		wsprintf(strText, _T("����"));
	else
		wsprintf(strText, _T("�ر�"));

	LeaveCriticalSection(&csHuiBoState);
	DeleteCriticalSection(&csHuiBoState);

	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

/**************************
*�ز����봰�ڴ������
* @huiBoBianMa    hInstance[in]:
* @huiBoBianMa		message[in]:
* @huiBoBianMa		wParam[in]:
* @huiBoBianMa		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK huiBoBianMaChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			//����ť�ؼ�
			hBtnHuiBoBianMaSelect = CreateWindow(_T("BUTTON"),_T("selectHuiBoBianMa"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_HUIBOBIANMA_SELECT,hInst,NULL);
			hBtnHuiBoBianMaGongJianHouDu = CreateWindow(_T("BUTTON"),_T("gongJianHouDuHuiBoBianMa"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_HUIBOBIANMA_GONGJIANHOUDU,hInst,NULL);
			hBtnHuiBoBianMaOnOrOff = CreateWindow(_T("BUTTON"),_T("onOrOffHuiBoBianMa"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_HUIBOBIANMA_ONOROFF,hInst,NULL);
					
			huiBoBianMaBtnList[0] = hBtnHuiBoBianMaSelect;
			huiBoBianMaBtnList[1] = hBtnHuiBoBianMaGongJianHouDu;
			huiBoBianMaBtnList[2] = hBtnHuiBoBianMaOnOrOff;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnHuiBoBianMaSelectProc = (WNDPROC) GetWindowLong(hBtnHuiBoBianMaSelect,GWL_WNDPROC);
			SetWindowLong(hBtnHuiBoBianMaSelect,GWL_WNDPROC,(LONG)btnHuiBoBianMaSelectProc);
			
			OldBtnHuiBoBianMaGongJianHouDuProc = (WNDPROC) GetWindowLong(hBtnHuiBoBianMaGongJianHouDu,GWL_WNDPROC);
			SetWindowLong(hBtnHuiBoBianMaGongJianHouDu,GWL_WNDPROC,(LONG)btnHuiBoBianMaGongJianHouDuProc);
			
			OldBtnHuiBoBianMaOnOrOffProc = (WNDPROC) GetWindowLong(hBtnHuiBoBianMaOnOrOff,GWL_WNDPROC);
			SetWindowLong(hBtnHuiBoBianMaOnOrOff,GWL_WNDPROC,(LONG)btnHuiBoBianMaOnOrOffProc);
			
		
			currentHuiBoBianMaBtn = huiBoBianMaBtnList[0];//Ĭ�ϵ�ǰѡ�лز�����ѡ��ť
			SetFocus(currentHuiBoBianMaBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentHuiBoBianMaBtn;
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
			huiBoBianMa_DrawButtonBitmap(hInst,pdis);
			// songchenguang add start 2010-07-02
			if(pdis->hwndItem == hBtnHuiBoBianMaSelect)
			{
				SetHuiBoNo(hBtnHuiBoBianMaSelect, nHuiBoBianMaNo);
				SetHuiBoOpenFlag(hBtnHuiBoBianMaOnOrOff, nHuiBoBianMaNo);
			}
			else if(pdis->hwndItem == hBtnHuiBoBianMaGongJianHouDu)
				SetWidth(hBtnHuiBoBianMaGongJianHouDu, gfPlateHeight);
			else if(pdis->hwndItem == hBtnHuiBoBianMaOnOrOff)
				SetHuiBoOpenFlag(hBtnHuiBoBianMaOnOrOff, nHuiBoBianMaNo);

			// ��ⰴť�������ʾ��Ӧ�Ĳ���
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentHuiBoBianMaBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnHuiBoBianMaGongJianHouDu)
						swprintf(gszTextStep, _T("%.1f"), fHuiBoStep);
					else
						swprintf(gszTextStep, _T(""));

					PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}
			// songchenguang add end 2010-07-02
			break;
		case WM_DESTROY:
			InitializeCriticalSection(&csHuiBoState);
			EnterCriticalSection(&csHuiBoState);

			memset(gbHuiBoOpen, FALSE, sizeof(gbHuiBoOpen));

			LeaveCriticalSection(&csHuiBoState);
			DeleteCriticalSection(&csHuiBoState);

			SetRedrawFlag(TRUE);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*���ػز����봰�ڰ�ťͼƬ
* @huiBoBianMa    hInstance[in]:
* @huiBoBianMa		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int huiBoBianMa_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_huiBoBianMaBtnBitmap[]=
	{
		{IDC_HUIBOBIANMA_SELECT	,IDB_BITMAP_HUIBOHAOXUANZE_NOSEL_R	 ,  IDB_BITMAP_HUIBOHAOXUANZE_SELECT_R		},
		{IDC_HUIBOBIANMA_GONGJIANHOUDU	,IDB_BITMAP_GONGJIANHOUDU_NOSEL_R	 ,  IDB_BITMAP_GONGJIANHOUDU_SELECT_R		},
		{IDC_HUIBOBIANMA_ONOROFF	,IDB_BITMAP_KAIQIGUANBI4_NOSEL_R ,  IDB_BITMAP_KAIQIGUANBI4_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_huiBoBianMaBtnBitmap);
	return ReturnVal;
}


/**************************
*�ز�����ѡ��ť�Զ��崦�����
* @huiBoBianMa    hInstance[in]:
* @huiBoBianMa		message[in]:
* @huiBoBianMa		wParam[in]:
* @huiBoBianMa		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHuiBoBianMaSelectProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndHuiBoBianMa);
					hWndHuiBoBianMa = 0;
					
					gCurrentFocusBtn = currentSpecialDisplayBtn;

					gCurrentWnd = hWndSpecialDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(huiBoBianMaBtnList[i] != currentHuiBoBianMaBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHuiBoBianMaBtn = huiBoBianMaBtnList[i];
					SetFocus(currentHuiBoBianMaBtn);
					gCurrentFocusBtn = currentHuiBoBianMaBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(huiBoBianMaBtnList[i] != currentHuiBoBianMaBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHuiBoBianMaBtn = huiBoBianMaBtnList[i];
					SetFocus(currentHuiBoBianMaBtn);
					gCurrentFocusBtn = currentHuiBoBianMaBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(nHuiBoBianMaNo > 1)
						nHuiBoBianMaNo--;
					else
						nHuiBoBianMaNo = 10;

					SetHuiBoNo(hWnd, nHuiBoBianMaNo);
					SetHuiBoOpenFlag(hBtnHuiBoBianMaOnOrOff, nHuiBoBianMaNo);
				}
				else if(wParam == VK_RIGHT)
				{
					if(nHuiBoBianMaNo < 10)
						nHuiBoBianMaNo++;
					else
						nHuiBoBianMaNo = 1;

					SetHuiBoNo(hWnd, nHuiBoBianMaNo);
					SetHuiBoOpenFlag(hBtnHuiBoBianMaOnOrOff, nHuiBoBianMaNo);
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
	return CallWindowProc(OldBtnHuiBoBianMaSelectProc,hWnd,Msg,wParam,lParam);
}

/**************************
*������Ȱ�ť�Զ��崦�����
* @huiBoBianMa    hInstance[in]:
* @huiBoBianMa		message[in]:
* @huiBoBianMa		wParam[in]:
* @huiBoBianMa		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHuiBoBianMaGongJianHouDuProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_CREATE:
		fHuiBoStep = AMENDSTEPS[wHuiBoStepIndex];
		break;
	case WM_KEYDOWN:
		{
			int i = 0;
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndSpecialDisplay,SW_SHOW);
				SetFocus(currentSpecialDisplayBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

				DestroyWindow(hWndHuiBoBianMa);
				hWndHuiBoBianMa = 0;
				
				gCurrentFocusBtn = currentSpecialDisplayBtn;

				gCurrentWnd = hWndSpecialDisplay;
			}
		
			else if(wParam == VK_UP)
			{
				while(huiBoBianMaBtnList[i] != currentHuiBoBianMaBtn)
				{
					i++;
				}
				i--;
				if(i < 0)
					i = buttonNum - 1;
				currentHuiBoBianMaBtn = huiBoBianMaBtnList[i];
				SetFocus(currentHuiBoBianMaBtn);
				gCurrentFocusBtn = currentHuiBoBianMaBtn;
			}
			else if(wParam == VK_DOWN)
			{
				while(huiBoBianMaBtnList[i] != currentHuiBoBianMaBtn)
				{
					i++;
				}
				i++;
				if(i > (buttonNum - 1))
					i = 0;
				currentHuiBoBianMaBtn = huiBoBianMaBtnList[i];
				SetFocus(currentHuiBoBianMaBtn);
				gCurrentFocusBtn = currentHuiBoBianMaBtn;
			}
			else if(wParam == VK_RETURN)
			{
			}
			else if(wParam == VK_LEFT)
			{
				if(gfPlateHeight > fHuiBoStep)
					gfPlateHeight -= fHuiBoStep;
				else
					gfPlateHeight = fHuiBoStep;

				SetWidth(hWnd, gfPlateHeight);

				SetRedrawFlag(TRUE);
			}
			else if(wParam == VK_RIGHT)
			{
				if((gfPlateHeight + fHuiBoStep) < MAX_fPlateHeight)
					gfPlateHeight += fHuiBoStep;
				else
					gfPlateHeight = MAX_fPlateHeight;

				SetWidth(hWnd, gfPlateHeight);

				SetRedrawFlag(TRUE);
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
			else if(wParam == 'F')
			{
				if(wHuiBoStepIndex < 3)
					wHuiBoStepIndex++;
				else
					wHuiBoStepIndex = 0;

				fHuiBoStep = AMENDSTEPS[wHuiBoStepIndex];

				memset(gszTextStep, 0, sizeof(gszTextStep));
				swprintf(gszTextStep, _T("%.1f"), fHuiBoStep);
				SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			}
		}
		break;
	default:
		break;
	}
	return CallWindowProc(OldBtnHuiBoBianMaGongJianHouDuProc,hWnd,Msg,wParam,lParam);
}

/**************************
*����/�رհ�ť�Զ��崦�����
* @huiBoBianMa    hInstance[in]:
* @huiBoBianMa		message[in]:
* @huiBoBianMa		wParam[in]:
* @huiBoBianMa		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHuiBoBianMaOnOrOffProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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

					DestroyWindow(hWndHuiBoBianMa);
					hWndHuiBoBianMa = 0;
					
					gCurrentFocusBtn = currentSpecialDisplayBtn;

					gCurrentWnd = hWndSpecialDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(huiBoBianMaBtnList[i] != currentHuiBoBianMaBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHuiBoBianMaBtn = huiBoBianMaBtnList[i];
					SetFocus(currentHuiBoBianMaBtn);
					gCurrentFocusBtn = currentHuiBoBianMaBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(huiBoBianMaBtnList[i] != currentHuiBoBianMaBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHuiBoBianMaBtn = huiBoBianMaBtnList[i];
					SetFocus(currentHuiBoBianMaBtn);
					gCurrentFocusBtn = currentHuiBoBianMaBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					InitializeCriticalSection(&csHuiBoState);
					EnterCriticalSection(&csHuiBoState);

					gbHuiBoOpen[nHuiBoBianMaNo -1] = !gbHuiBoOpen[nHuiBoBianMaNo -1];					
					
					LeaveCriticalSection(&csHuiBoState);
					DeleteCriticalSection(&csHuiBoState);

					SetHuiBoOpenFlag(hWnd, nHuiBoBianMaNo);
					SetRedrawFlag(TRUE);
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
	return CallWindowProc(OldBtnHuiBoBianMaOnOrOffProc,hWnd,Msg,wParam,lParam);
}
