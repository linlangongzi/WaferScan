/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * �¿����ô���Դ�ļ�
 * Filename: poKouSheZhiWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnPoKouSheZhiWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"


HWND hWndPoKouSheZhi = NULL;						//�¿����ô��ھ��
HWND hBtnPoKouSheZhiLeiXing = NULL;	//�¿����Ͱ�ť���
HWND hBtnPoKouSheZhiPoKouJiaoDu = NULL;			//�¿ڽǶȰ�ť���
HWND hBtnPoKouSheZhiPoKouShenDu = NULL;			//�¿���Ȱ�ť���

HWND poKouSheZhiBtnList[3] ={NULL,NULL,NULL};					//�¿����ô��ڵİ�ť��
WNDPROC OldBtnPoKouSheZhiLeiXingProc = NULL;//�¿����Ͱ�ťĬ�ϴ������
WNDPROC OldBtnPoKouSheZhiPoKouJiaoDuProc = NULL;		//�¿ڽǶȰ�ťĬ�ϴ������
WNDPROC OldBtnPoKouSheZhiPoKouShenDuProc = NULL;		//�¿���Ȱ�ťĬ�ϴ������
 
HWND currentPoKouSheZhiBtn = NULL;					//�¿����ô��е�ǰѡ�еİ�ť
WORD gwPoKouLeiXing = 0;					//�¿�����
float gfPoKouJiaoDu = 45;					//�¿ڽǶ�
float gfPoKouShenDu = 1;					//�¿����
static float fPoKouJiaoDuStep = 1;				//�¿ڽǶȲ���
static WORD wPoKouJiaoDuStepIndex = 0;				///�¿ڽǶȲ�������
static float fPoKouShenDuStep = 1;				//�¿���Ȳ���
static WORD wPoKouShenDuStepIndex = 1;				//�¿���Ȳ�������


static const int buttonNum = 3;				//��ť�ؼ���



extern HINSTANCE hInst;					//����ʵ�����
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndHanFengTanShang;
extern HWND currentHanFengTanShangBtn;
extern float fHanFengTanShangGangBanHouDu;
extern float fHead2HanFengDistance;
extern BOOL bHanFengTanShangSwitch;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���

extern wchar_t gszTextStep[10];
extern BOOL gbHotRange;			// ��ݼ�����
extern BOOL gbHotDB;			// ��ݼ�����
extern HWND hStatusBar;			// ״̬�����
extern float gfPlateHeight;		//�������

/**************************
*ע���¿����ô���
* @poKouSheZhi    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerPoKouSheZhiChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) poKouSheZhiChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szPoKouSheZhiChild;

	return RegisterClass(&wc);
}
void displayPoKouShenDu(HWND hWnd ,float fPoKouShenDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(gwPoKouLeiXing == 0)
	{ 
		wsprintf(strText, _T("�������"));
		DrawLableText(hWnd, strText, 22, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else if(gwPoKouLeiXing == 1)
	{
		wsprintf(strText, _T("%.1f"), fPoKouShenDu);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else if(gwPoKouLeiXing == 2)
	{
		wsprintf(strText, _T("%.1f"), fPoKouShenDu);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
}
void displayPoKouJiaoDu(HWND hWnd ,float fPoKouJiaoDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fPoKouJiaoDu);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}
void displayPoKouLeiXing(HWND hWnd ,WORD wPoKouLeiXing)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(wPoKouLeiXing == 0)
		wsprintf(strText, _T("V�Ϳ�"));
	else if(wPoKouLeiXing == 1)
		wsprintf(strText, _T("Y�Ϳ�"));
	else
		wsprintf(strText, _T("˫Y�Ϳ�"));
	DrawLableText(hWnd, strText, 25, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	displayPoKouJiaoDu(hBtnPoKouSheZhiPoKouJiaoDu, gfPoKouJiaoDu);
	displayPoKouShenDu(hBtnPoKouSheZhiPoKouShenDu, gfPoKouShenDu);
}

/**************************
*�¿����ô��ڴ������
* @poKouSheZhi		hWnd[in]:
* @poKouSheZhi		message[in]:
* @poKouSheZhi		wParam[in]:
* @poKouSheZhi		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK poKouSheZhiChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			fPoKouJiaoDuStep = ANGLESTEPS[wPoKouJiaoDuStepIndex];
			fPoKouShenDuStep = ANGLESTEPS[wPoKouShenDuStepIndex];
			//����ť�ؼ�
			hBtnPoKouSheZhiLeiXing = CreateWindow(_T("BUTTON"),_T("LeiXingPoKouSheZhi"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_POKOUSHEZHI_LEIXING,hInst,NULL);
			hBtnPoKouSheZhiPoKouJiaoDu = CreateWindow(_T("BUTTON"),_T("poKouJiaoDuPoKouSheZhi"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_POKOUSHEZHI_POKOUJIAODU,hInst,NULL);
			hBtnPoKouSheZhiPoKouShenDu = CreateWindow(_T("BUTTON"),_T("poKouShenDuPoKouSheZhi"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_POKOUSHEZHI_POKOUSHENDU,hInst,NULL);
					
			poKouSheZhiBtnList[0] = hBtnPoKouSheZhiLeiXing;
			poKouSheZhiBtnList[1] = hBtnPoKouSheZhiPoKouJiaoDu;
			poKouSheZhiBtnList[2] = hBtnPoKouSheZhiPoKouShenDu;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnPoKouSheZhiLeiXingProc = (WNDPROC) GetWindowLong(hBtnPoKouSheZhiLeiXing,GWL_WNDPROC);
			SetWindowLong(hBtnPoKouSheZhiLeiXing,GWL_WNDPROC,(LONG)btnPoKouSheZhiLeiXingProc);
			
			OldBtnPoKouSheZhiPoKouJiaoDuProc = (WNDPROC) GetWindowLong(hBtnPoKouSheZhiPoKouJiaoDu,GWL_WNDPROC);
			SetWindowLong(hBtnPoKouSheZhiPoKouJiaoDu,GWL_WNDPROC,(LONG)btnPoKouSheZhiPoKouJiaoDuProc);
			
			OldBtnPoKouSheZhiPoKouShenDuProc = (WNDPROC) GetWindowLong(hBtnPoKouSheZhiPoKouShenDu,GWL_WNDPROC);
			SetWindowLong(hBtnPoKouSheZhiPoKouShenDu,GWL_WNDPROC,(LONG)btnPoKouSheZhiPoKouShenDuProc);
		
			currentPoKouSheZhiBtn = poKouSheZhiBtnList[0];//Ĭ�ϵ�ǰѡ���¿����Ͱ�ť
			SetFocus(currentPoKouSheZhiBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentPoKouSheZhiBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
				if(LOWORD(wParam) == IDC_POKOUSHEZHI_LEIXING)
				{
				}
				else if(LOWORD(wParam) == IDC_POKOUSHEZHI_POKOUJIAODU)
				{
				}
				else if(LOWORD(wParam) == IDC_POKOUSHEZHI_POKOUSHENDU)
				{
				}
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			poKouSheZhi_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnPoKouSheZhiLeiXing)
			{
				displayPoKouLeiXing(pdis->hwndItem, gwPoKouLeiXing);
			}
			else if(pdis->hwndItem == hBtnPoKouSheZhiPoKouJiaoDu)
			{
				displayPoKouJiaoDu(pdis->hwndItem, gfPoKouJiaoDu);
			}
			else if(pdis->hwndItem == hBtnPoKouSheZhiPoKouShenDu)
			{
				displayPoKouShenDu(pdis->hwndItem, gfPoKouShenDu);
			}
			else
			{
				;//
			}

			
			// ��ⰴť�������ʾ��Ӧ�Ĳ���
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentPoKouSheZhiBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnPoKouSheZhiPoKouJiaoDu)
						swprintf(gszTextStep, _T("%.1f"), fPoKouJiaoDuStep);
					else if(pdis->hwndItem == hBtnPoKouSheZhiPoKouShenDu)
						swprintf(gszTextStep, _T("%.1f"), fPoKouShenDuStep);
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
*�����¿����ô��ڰ�ťͼƬ
* @poKouSheZhi    hInstance[in]:
* @poKouSheZhi		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int poKouSheZhi_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_poKouSheZhiBtnBitmap[]=
	{
		{IDC_POKOUSHEZHI_LEIXING	,IDB_BITMAP_POKOULEIXING_NOSEL_R	 ,  IDB_BITMAP_POKOULEIXING_SELECT_R		},
		{IDC_POKOUSHEZHI_POKOUJIAODU	,IDB_BITMAP_POKOUJIAODU_NOSEL_R	 ,  IDB_BITMAP_POKOUJIAODU_SELECT_R		},
		{IDC_POKOUSHEZHI_POKOUSHENDU	,IDB_BITMAP_POKOUSHENDU_NOSEL_R ,  IDB_BITMAP_POKOUSHENDU_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_poKouSheZhiBtnBitmap);
	return ReturnVal;
}
/**************************
*�¿����Ͱ�ť�Զ��崦�����
* @poKouSheZhi		Wnd[in]:
* @poKouSheZhi		message[in]:
* @poKouSheZhi		wParam[in]:
* @poKouSheZhi		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnPoKouSheZhiLeiXingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndHanFengTanShang,SW_SHOW);
					SetFocus(currentHanFengTanShangBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndPoKouSheZhi);
					hWndPoKouSheZhi = 0;
					
					gCurrentFocusBtn = currentHanFengTanShangBtn;

					gCurrentWnd = hWndHanFengTanShang;

				/*	if(gwPoKouLeiXing == 1)
					{ 
						gfPlateHeight = gfPoKouShenDu;
					}
					else if(gwPoKouLeiXing == 2)
					{ 
						gfPlateHeight = 2 * gfPoKouShenDu;
					}*/
				}
			
				else if(wParam == VK_UP)
				{
					while(poKouSheZhiBtnList[i] != currentPoKouSheZhiBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentPoKouSheZhiBtn = poKouSheZhiBtnList[i];
					SetFocus(currentPoKouSheZhiBtn);
					gCurrentFocusBtn = currentPoKouSheZhiBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(poKouSheZhiBtnList[i] != currentPoKouSheZhiBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentPoKouSheZhiBtn = poKouSheZhiBtnList[i];
					SetFocus(currentPoKouSheZhiBtn);
					gCurrentFocusBtn = currentPoKouSheZhiBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if(gwPoKouLeiXing > 0)
							gwPoKouLeiXing--;
						displayPoKouLeiXing(Wnd, gwPoKouLeiXing);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if(gwPoKouLeiXing < 2)
							gwPoKouLeiXing++;
						displayPoKouLeiXing(Wnd, gwPoKouLeiXing);
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
	return CallWindowProc(OldBtnPoKouSheZhiLeiXingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�¿ڽǶȰ�ť�Զ��崦�����
* @poKouSheZhi		Wnd[in]:
* @poKouSheZhi		message[in]:
* @poKouSheZhi		wParam[in]:
* @poKouSheZhi		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnPoKouSheZhiPoKouJiaoDuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndHanFengTanShang,SW_SHOW);
					SetFocus(currentHanFengTanShangBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndPoKouSheZhi);
					hWndPoKouSheZhi = 0;
					
					gCurrentFocusBtn = currentHanFengTanShangBtn;

					gCurrentWnd = hWndHanFengTanShang;

				/*	if(gwPoKouLeiXing == 1)
					{ 
						gfPlateHeight = gfPoKouShenDu;
					}
					else if(gwPoKouLeiXing == 2)
					{ 
						gfPlateHeight = 2 * gfPoKouShenDu;
					}*/
				}
			
				else if(wParam == VK_UP)
				{
					while(poKouSheZhiBtnList[i] != currentPoKouSheZhiBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentPoKouSheZhiBtn = poKouSheZhiBtnList[i];
					SetFocus(currentPoKouSheZhiBtn);
					gCurrentFocusBtn = currentPoKouSheZhiBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(poKouSheZhiBtnList[i] != currentPoKouSheZhiBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentPoKouSheZhiBtn = poKouSheZhiBtnList[i];
					SetFocus(currentPoKouSheZhiBtn);
					gCurrentFocusBtn = currentPoKouSheZhiBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if((gfPoKouJiaoDu - fPoKouJiaoDuStep) > 0)
						{
							gfPoKouJiaoDu = gfPoKouJiaoDu - fPoKouJiaoDuStep;

							double fAngle = gfPoKouJiaoDu/180.0f * PI;
							fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
						}
						displayPoKouJiaoDu(Wnd, gfPoKouJiaoDu);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if((gfPoKouJiaoDu + fPoKouJiaoDuStep) < 90)
						{
							gfPoKouJiaoDu = gfPoKouJiaoDu + fPoKouJiaoDuStep;

							double fAngle = gfPoKouJiaoDu/180.0f * PI;
							fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
						}
						displayPoKouJiaoDu(Wnd, gfPoKouJiaoDu);
					}
				}
				else if(wParam == 'F')
				{
					if(wPoKouJiaoDuStepIndex < 1)
						wPoKouJiaoDuStepIndex++;
					else
						wPoKouJiaoDuStepIndex = 0;

					fPoKouJiaoDuStep = ANGLESTEPS[wPoKouJiaoDuStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fPoKouJiaoDuStep);
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnPoKouSheZhiPoKouJiaoDuProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�¿���Ȱ�ť�Զ��崦�����
* @poKouSheZhi		Wnd[in]:
* @poKouSheZhi		message[in]:
* @poKouSheZhi		wParam[in]:
* @poKouSheZhi		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnPoKouSheZhiPoKouShenDuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndHanFengTanShang,SW_SHOW);
					SetFocus(currentHanFengTanShangBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndPoKouSheZhi);
					hWndPoKouSheZhi = 0;
					
					gCurrentFocusBtn = currentHanFengTanShangBtn;

					gCurrentWnd = hWndHanFengTanShang;

				/*	if(gwPoKouLeiXing == 1)
					{ 
						gfPlateHeight = gfPoKouShenDu;
					}
					else if(gwPoKouLeiXing == 2)
					{ 
						gfPlateHeight = 2 * gfPoKouShenDu;
					}*/
				}
			
				else if(wParam == VK_UP)
				{
					while(poKouSheZhiBtnList[i] != currentPoKouSheZhiBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentPoKouSheZhiBtn = poKouSheZhiBtnList[i];
					SetFocus(currentPoKouSheZhiBtn);
					gCurrentFocusBtn = currentPoKouSheZhiBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(poKouSheZhiBtnList[i] != currentPoKouSheZhiBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentPoKouSheZhiBtn = poKouSheZhiBtnList[i];
					SetFocus(currentPoKouSheZhiBtn);
					gCurrentFocusBtn = currentPoKouSheZhiBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if(gwPoKouLeiXing == 1)
						{
							if((gfPoKouShenDu - fPoKouShenDuStep) > 1)
							{
								gfPoKouShenDu = gfPoKouShenDu - fPoKouShenDuStep;
								double fAngle = gfPoKouJiaoDu/180.0f * PI;
								fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
							}
							else
							{
								gfPoKouShenDu = 1;
								double fAngle = gfPoKouJiaoDu/180.0f * PI;
								fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
							}
							//gfPlateHeight = gfPoKouShenDu;
						}
						else if(gwPoKouLeiXing == 2)
						{
							if((2 * (gfPoKouShenDu - fPoKouShenDuStep)) > 2)
							{
								gfPoKouShenDu = gfPoKouShenDu - fPoKouShenDuStep;
								double fAngle = gfPoKouJiaoDu/180.0f * PI;
								fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
							}
							else
							{
								gfPoKouShenDu = 1;
								double fAngle = gfPoKouJiaoDu/180.0f * PI;
								fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
							}
							//gfPlateHeight = 2 * gfPoKouShenDu;
						}
						displayPoKouShenDu(Wnd, gfPoKouShenDu);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if(gwPoKouLeiXing == 1)
						{
							if((gfPoKouShenDu + fPoKouShenDuStep) <  gfPlateHeight)
							{
								gfPoKouShenDu = gfPoKouShenDu + fPoKouShenDuStep;
								double fAngle = gfPoKouJiaoDu/180.0f * PI;
								fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
							}
							//gfPlateHeight = gfPoKouShenDu;
						}
						else if(gwPoKouLeiXing == 2)
						{
							if((2 * (gfPoKouShenDu + fPoKouShenDuStep)) <  gfPlateHeight)
							{
								gfPoKouShenDu = gfPoKouShenDu + fPoKouShenDuStep;
								double fAngle = gfPoKouJiaoDu/180.0f * PI;
								fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
							}
							//gfPlateHeight = 2 * gfPoKouShenDu;
						}
						displayPoKouShenDu(Wnd, gfPoKouShenDu);
					}
				}
				else if(wParam == 'F')
				{
					if(wPoKouShenDuStepIndex < 1)
						wPoKouShenDuStepIndex++;
					else
						wPoKouShenDuStepIndex = 0;

					fPoKouShenDuStep = ANGLESTEPS[wPoKouShenDuStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fPoKouShenDuStep);
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnPoKouSheZhiPoKouShenDuProc,Wnd,Msg,wParam,lParam);
}
