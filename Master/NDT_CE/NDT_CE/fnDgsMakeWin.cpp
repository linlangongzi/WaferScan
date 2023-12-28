/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * DGS������������Դ�ļ�
 * Filename: fnDgsMakeWin.cpp
 *
 * Spec: Trace Tag write here. If has not, OnOff this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"
#include "fnDgsMakeWin.h"
#include "fnCreateDgsWin.h"
#include "fnModifyDgsWin.h"
#include "interfaceCmdID.h"
#include "craftParamWin.h"
#include "resource.h"

HWND hWndDgsMake = NULL;//DGS�����������ھ�
//HWND hBtnDgsMakeType;//DGS���Ͱ�ť���
HWND hBtnDgsMakeCreate = NULL;//������ť���
HWND hBtnDgsMakeModify = NULL;//�޸İ�ť���
HWND hBtnDgsMakePingDing = NULL;//�����߰�ť���
HWND hBtnDgsMakeDingLiang = NULL;//�����߰�ť���
HWND hBtnDgsMakePanHui = NULL;//�з��߰�ť���

HWND dgsMakeBtnList[5] = {NULL,NULL,NULL,NULL,NULL};//DGS�����������ڵİ�ť��
WNDPROC OldBtnDgsMakeTypeProc = NULL;//DGS���Ͱ�ťĬ�ϴ������
WNDPROC OldBtnDgsMakeCreateProc = NULL;//������ťĬ�ϴ������
WNDPROC OldBtnDgsMakeModifyProc = NULL;//�޸İ�ťĬ�ϴ������
WNDPROC OldBtnDgsMakePingDingProc = NULL;//�����߰�ťĬ�ϴ������
WNDPROC OldBtnDgsMakeDingLiangProc = NULL;//�����߰�ťĬ�ϴ������
WNDPROC OldBtnDgsMakePanHuiProc = NULL;//�з��߰�ťĬ�ϴ������

HWND currentDgsMakeBtn = NULL;//DGS�����������е�ǰѡ�еİ�ť
//WORD gDgsType;			//DGS����(ƽ�׿ס���ͨ�ס���ƽ��)

static const int buttonNum = 5;//��ť�ؼ���
static WORD wCurveGainStepIndex = 1;		//�����沽������
static float fCurveGainStep = 1;			//�����沽��

extern HWND hWndMain;//�����ھ��
extern HWND hWndDgsCurve;//�ļ������ھ��
extern HWND currentDgsCurveBtn;//���ߴ����е�ǰѡ�еİ�ť
extern HWND hWndCreateDacCurve;//�½�DAC���ߴ��ھ��
extern HWND hWndModifyDacCurve;//�޸�DAC���ߴ��ھ��
extern HINSTANCE hInst;//����ʵ�����
extern HWND gCurrentFocusBtn;
extern BOOL gWaveDrawDacFlag;//�����Ƿ�DAC

extern WORD CurrentChannelNo;				// ��ǰͨ����
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];				//DACͨ������
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���
extern wchar_t lpDefaultParamFile[MAX_PATH];

extern float gfPingDingGain[CHANNEL_NUM];				//��������
extern float gfDingLiangGain[CHANNEL_NUM];				//��������
extern float gfPanHuiGain[CHANNEL_NUM];				//�з�����
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;					// ״̬�����
extern BOOL gbHotDB;			// ��ݼ�����
extern BOOL gbHotRange;			// ��ݼ�����


extern void SetOpenFlag(HWND hWnd, const BOOL& bOpen, const WORD& nGateIndex);
extern void CALLBACK defectDisplayProc( HWND hwnd, 
							  UINT uMsg, 
							  UINT idEvent, 
							  DWORD dwTime 
							);

/**************************
*ע��DGS������������
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerDgsMakeChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) dgsMakeChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szDgsMakeChild;

	return RegisterClass(&wc);
}
/*
void displayDgsType(HWND hWnd, WORD dgsType)
{
	if(dgsType == 0)
	{
		DrawLableText(hWnd, _T("ƽ�׿�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	if(dgsType == 1)
	{
		DrawLableText(hWnd, _T("��ͨ��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	if(dgsType == 2)
	{
		DrawLableText(hWnd, _T("��ƽ��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
}
*/
static void displayCurveGain(HWND hWnd, float fGain)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fGain);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}

/**************************
*DGS�����������ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK dgsMakeChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:	
			//����ť�ؼ�
		//	hBtnDgsMakeType = CreateWindow(_T("BUTTON"),_T("type"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
		//			0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_DGSMAKE_TYPE,hInst,NULL);
			hBtnDgsMakeCreate = CreateWindow(_T("BUTTON"),_T("Create"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_DGSMAKE_CREATE,hInst,NULL);
			hBtnDgsMakeModify = CreateWindow(_T("BUTTON"),_T("Make"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_DGSMAKE_MODIFY,hInst,NULL);
			hBtnDgsMakePingDing = CreateWindow(_T("BUTTON"),_T("pingDIng"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_PINGDING,hInst,NULL);				
			hBtnDgsMakeDingLiang = CreateWindow(_T("BUTTON"),_T("dingLiang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_DINGLIANG,hInst,NULL);
			hBtnDgsMakePanHui = CreateWindow(_T("BUTTON"),_T("panHui"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_PANHUI,hInst,NULL);
			
			//dgsMakeBtnList[0] = hBtnDgsMakeType;
			dgsMakeBtnList[0] = hBtnDgsMakeCreate;
			dgsMakeBtnList[1] = hBtnDgsMakeModify;
			dgsMakeBtnList[2] = hBtnDgsMakePingDing;
			dgsMakeBtnList[3] = hBtnDgsMakeDingLiang;
			dgsMakeBtnList[4] = hBtnDgsMakePanHui;
			//��ť�ؼ����Զ��崦�����
		//	OldBtnDgsMakeTypeProc = (WNDPROC) GetWindowLong(hBtnDgsMakeType,GWL_WNDPROC);
		//	SetWindowLong(hBtnDgsMakeType,GWL_WNDPROC,(LONG)btnDgsMakeTypeProc);
			OldBtnDgsMakeCreateProc = (WNDPROC) GetWindowLong(hBtnDgsMakeCreate,GWL_WNDPROC);
			SetWindowLong(hBtnDgsMakeCreate,GWL_WNDPROC,(LONG)btnDgsMakeCreateProc);
			
			OldBtnDgsMakeModifyProc = (WNDPROC) GetWindowLong(hBtnDgsMakeModify,GWL_WNDPROC);
			SetWindowLong(hBtnDgsMakeModify,GWL_WNDPROC,(LONG)btnDgsMakeModifyProc);
			
			OldBtnDgsMakePingDingProc = (WNDPROC) GetWindowLong(hBtnDgsMakePingDing,GWL_WNDPROC);
			SetWindowLong(hBtnDgsMakePingDing,GWL_WNDPROC,(LONG)btnDgsMakePingDingProc);

			OldBtnDgsMakeDingLiangProc = (WNDPROC) GetWindowLong(hBtnDgsMakeDingLiang,GWL_WNDPROC);
			SetWindowLong(hBtnDgsMakeDingLiang,GWL_WNDPROC,(LONG)btnDgsMakeDingLiangProc);

			OldBtnDgsMakePanHuiProc = (WNDPROC) GetWindowLong(hBtnDgsMakePanHui,GWL_WNDPROC);
			SetWindowLong(hBtnDgsMakePanHui,GWL_WNDPROC,(LONG)btnDgsMakePanHuiProc);
			
			currentDgsMakeBtn = dgsMakeBtnList[0];  //Ĭ�ϵ�ǰѡ�����Ͱ�ť
			SetFocus(currentDgsMakeBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentDgsMakeBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndDgsMake, &ps);
			
			EndPaint(hWndDgsMake, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDC_DGSMAKE_CREATE)
			{
			}
			if(LOWORD(wParam) == IDC_DGSMAKE_MODIFY)
			{
			}
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			dgsMake_DrawButtonBitmap(hInst,pdis);
		/*	if(pdis->hwndItem == hBtnDgsMakeType)
			{
				displayDgsType(pdis->hwndItem, gDgsType);
			}*/
			if(pdis->hwndItem == hBtnDgsMakePingDing)
			{
				displayCurveGain(pdis->hwndItem, gfPingDingGain[CurrentChannelNo]);
			}
			if(pdis->hwndItem == hBtnDgsMakeDingLiang)
			{
				displayCurveGain(pdis->hwndItem, gfDingLiangGain[CurrentChannelNo]);
			}
			if(pdis->hwndItem == hBtnDgsMakePanHui)
			{
				displayCurveGain(pdis->hwndItem, gfPanHuiGain[CurrentChannelNo]);
			}

			// ��ⰴť�������ʾ��Ӧ�Ĳ���
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentDgsMakeBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnDgsMakePingDing || pdis->hwndItem == hBtnDgsMakeDingLiang || pdis->hwndItem == hBtnDgsMakePanHui)
						swprintf(gszTextStep, _T("%.1f"), fCurveGainStep);
					else
						swprintf(gszTextStep, _T(""));

					SendMessage(hStatusBar, SB_SETTEXT, 
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
*����DGS�����������ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int dgsMake_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_dgsMakeBtnBitmap[]=
	{
	//	{IDC_DGSMAKE_TYPE	,IDB_BITMAP_DGSLEIXING_NOSEL_R	 ,  IDB_BITMAP_DGSLEIXING_SELECT_R		},
		{IDC_DGSMAKE_CREATE	,IDB_BITMAP_XINJIAN_NOSEL_R	 ,  IDB_BITMAP_XINJIAN_SELECT_R		},
		{IDC_DGSMAKE_MODIFY	,IDB_BITMAP_XIUGAI_NOSEL_R	 ,  IDB_BITMAP_XIUGAI_SELECT_R		},
		{IDC_CREATEDGSCURVE_PINGDING	,IDB_BITMAP_PINGDINGXIAN_NOSEL_R ,  IDB_BITMAP_PINGDINGXIAN_SELECT_R	},
		{IDC_CREATEDGSCURVE_DINGLIANG	,IDB_BITMAP_DINGLIANGXIAN_NOSEL_R ,  IDB_BITMAP_DINGLIANGXIAN_SELECT_R	},
		{IDC_CREATEDGSCURVE_PANHUI	,IDB_BITMAP_PANFEIXIAN_NOSEL_R ,  IDB_BITMAP_PANFEIXIAN_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_dgsMakeBtnBitmap);
	return ReturnVal;
}

/**************************
*������ť�Զ��崦�����
* @param    Wnd[in]:
* @param		Wnd[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
/*
LRESULT CALLBACK btnDgsMakeTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					DestroyWindow(hWndDgsMake);
					hWndDgsMake = 0;
					ShowWindow(hWndDgsCurve,SW_SHOW);
					SetFocus(currentDgsCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					gCurrentFocusBtn = currentDgsCurveBtn;
					
				}
			
				else if(wParam == VK_UP)
				{
					while(dgsMakeBtnList[i] != currentDgsMakeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentDgsMakeBtn = dgsMakeBtnList[i];
					SetFocus(currentDgsMakeBtn);
					gCurrentFocusBtn = currentDgsMakeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(dgsMakeBtnList[i] != currentDgsMakeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentDgsMakeBtn = dgsMakeBtnList[i];
					SetFocus(currentDgsMakeBtn);
					gCurrentFocusBtn = currentDgsMakeBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(gDgsType > 0)
					{
						gDgsType--;
						displayDgsType(Wnd, gDgsType);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(gDgsType < 2)
					{
						gDgsType++;
						displayDgsType(Wnd, gDgsType);
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
	return CallWindowProc(OldBtnDgsMakeTypeProc,Wnd,Msg,wParam,lParam);
}
*/
/**************************
*������ť�Զ��崦�����
* @param    Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnDgsMakeCreateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{					
					ShowWindow(hWndDgsCurve,SW_SHOW);
					SetFocus(currentDgsCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndDgsMake);
					hWndDgsMake = 0;

					gCurrentFocusBtn = currentDgsCurveBtn;

					gCurrentWnd = hWndDgsCurve;

					SetTimer(hWndDgsCurve, TIMER_DAC_DEFECT_DISPLAY, 400, defectDisplayProc);
				}
			
				else if(wParam == VK_UP)
				{
					while(dgsMakeBtnList[i] != currentDgsMakeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentDgsMakeBtn = dgsMakeBtnList[i];
					SetFocus(currentDgsMakeBtn);
					gCurrentFocusBtn = currentDgsMakeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(dgsMakeBtnList[i] != currentDgsMakeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentDgsMakeBtn = dgsMakeBtnList[i];
					SetFocus(currentDgsMakeBtn);
					gCurrentFocusBtn = currentDgsMakeBtn;
				}
				else if(wParam == VK_RETURN)
				{
					ReadParamFile(lpDefaultParamFile);//������/����/�з�
					
					hWndCreateDacCurve = CreateWindow(szCreateDacCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndDgsMake,SW_HIDE);
				//	SetFocus(hWndCreateDacCurve);
					ShowWindow(hWndCreateDacCurve,SW_SHOW);
					UpdateWindow(hWndCreateDacCurve);

					gCurrentWnd = hWndCreateDacCurve;

					for(int i = 0; i < CHANNEL_NUM; i++)
					{
						for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
						{
							gDgsSwitch[i][j] = FALSE;
						}
					}
				}
				else if(wParam == VK_LEFT)
				{
				}
				else if(wParam == VK_RIGHT)
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
	return CallWindowProc(OldBtnDgsMakeCreateProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�޸İ�ť�Զ��崦�����
* @param    Wnd[in]:
* @param		Wnd[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnDgsMakeModifyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndDgsCurve,SW_SHOW);
					SetFocus(currentDgsCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndDgsMake);
					hWndDgsMake = 0;

					gCurrentFocusBtn = currentDgsCurveBtn;

					gCurrentWnd = hWndDgsCurve;

					SetTimer(hWndDgsCurve, TIMER_DAC_DEFECT_DISPLAY, 400, defectDisplayProc);
				}
			
				else if(wParam == VK_UP)
				{
					while(dgsMakeBtnList[i] != currentDgsMakeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentDgsMakeBtn = dgsMakeBtnList[i];
					SetFocus(currentDgsMakeBtn);
					gCurrentFocusBtn = currentDgsMakeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(dgsMakeBtnList[i] != currentDgsMakeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentDgsMakeBtn = dgsMakeBtnList[i];
					SetFocus(currentDgsMakeBtn);
					gCurrentFocusBtn = currentDgsMakeBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndModifyDacCurve = CreateWindow(szModifyDgsCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndDgsMake,SW_HIDE);
				//	SetFocus(hWndModifyDacCurve);
					ShowWindow(hWndModifyDacCurve,SW_SHOW);
					UpdateWindow(hWndModifyDacCurve);

					gCurrentWnd = hWndModifyDacCurve;  

					for(int i = 0; i < CHANNEL_NUM; i++)
					{
						for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
						{
							gDgsSwitch[i][j] = FALSE;
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
	return CallWindowProc(OldBtnDgsMakeModifyProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�����߰�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnDgsMakePingDingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndDgsCurve,SW_SHOW);
					SetFocus(currentDgsCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndDgsMake);
					hWndDgsMake = 0;

					gCurrentFocusBtn = currentDgsCurveBtn;

					gCurrentWnd = hWndDgsCurve;

					SetTimer(hWndDgsCurve, TIMER_DAC_DEFECT_DISPLAY, 400, defectDisplayProc);
				}
			
				else if(wParam == VK_UP)
				{
					while(dgsMakeBtnList[i] != currentDgsMakeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentDgsMakeBtn = dgsMakeBtnList[i];
					SetFocus(currentDgsMakeBtn);
					gCurrentFocusBtn = currentDgsMakeBtn;
				}				
				else if(wParam == VK_DOWN)
				{
					while(dgsMakeBtnList[i] != currentDgsMakeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentDgsMakeBtn = dgsMakeBtnList[i];
					SetFocus(currentDgsMakeBtn);
					gCurrentFocusBtn = currentDgsMakeBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfPingDingGain[CurrentChannelNo] != MIN_CURVE_GAIN)
					{
						if(gfPingDingGain[CurrentChannelNo] - fCurveGainStep > MIN_CURVE_GAIN)
						{
							gfPingDingGain[CurrentChannelNo] -= fCurveGainStep;
						}
						else
							gfPingDingGain[CurrentChannelNo] = MIN_CURVE_GAIN;

						displayCurveGain(Wnd, gfPingDingGain[CurrentChannelNo]);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfPingDingGain[CurrentChannelNo] != MAX_CURVE_GAIN)
					{
						if(gfPingDingGain[CurrentChannelNo] + fCurveGainStep < gfDingLiangGain[CurrentChannelNo])
						{
							gfPingDingGain[CurrentChannelNo] += fCurveGainStep;
						}

						displayCurveGain(Wnd, gfPingDingGain[CurrentChannelNo]);
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
				else if(wParam == 'F')
				{
					if(wCurveGainStepIndex < 1)
						wCurveGainStepIndex++;
					else
						wCurveGainStepIndex = 0;

					fCurveGainStep = CURVEGAINSTEPS[wCurveGainStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fCurveGainStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDgsMakePingDingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�����߰�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnDgsMakeDingLiangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndDgsCurve,SW_SHOW);
					SetFocus(currentDgsCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndDgsMake);
					hWndDgsMake = 0;

					gCurrentFocusBtn = currentDgsCurveBtn;

					gCurrentWnd = hWndDgsCurve;

					SetTimer(hWndDgsCurve, TIMER_DAC_DEFECT_DISPLAY, 400, defectDisplayProc);
				}
			
				else if(wParam == VK_UP)
				{
					while(dgsMakeBtnList[i] != currentDgsMakeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentDgsMakeBtn = dgsMakeBtnList[i];
					SetFocus(currentDgsMakeBtn);
					gCurrentFocusBtn = currentDgsMakeBtn;
				}				
				else if(wParam == VK_DOWN)
				{
					while(dgsMakeBtnList[i] != currentDgsMakeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentDgsMakeBtn = dgsMakeBtnList[i];
					SetFocus(currentDgsMakeBtn);
					gCurrentFocusBtn = currentDgsMakeBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfDingLiangGain[CurrentChannelNo] != MIN_CURVE_GAIN)
					{
						if(gfDingLiangGain[CurrentChannelNo] - fCurveGainStep > gfPingDingGain[CurrentChannelNo])
						{
							gfDingLiangGain[CurrentChannelNo] -= fCurveGainStep;
						}

						displayCurveGain(Wnd, gfDingLiangGain[CurrentChannelNo]);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfDingLiangGain[CurrentChannelNo] != MAX_CURVE_GAIN)
					{
						if(gfDingLiangGain[CurrentChannelNo] + fCurveGainStep < gfPanHuiGain[CurrentChannelNo])
						{
							gfDingLiangGain[CurrentChannelNo] += fCurveGainStep;
						}

						displayCurveGain(Wnd, gfDingLiangGain[CurrentChannelNo]);
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
				else if(wParam == 'F')
				{
					if(wCurveGainStepIndex < 1)
						wCurveGainStepIndex++;
					else
						wCurveGainStepIndex = 0;

					fCurveGainStep = CURVEGAINSTEPS[wCurveGainStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fCurveGainStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
				/*else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDgsMakeDingLiangProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�з��߰�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnDgsMakePanHuiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndDgsCurve,SW_SHOW);
					SetFocus(currentDgsCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndDgsMake);
					hWndDgsMake = 0;

					gCurrentFocusBtn = currentDgsCurveBtn;

					gCurrentWnd = hWndDgsCurve;

					SetTimer(hWndDgsCurve, TIMER_DAC_DEFECT_DISPLAY, 400, defectDisplayProc);
				}
			
				else if(wParam == VK_UP)
				{
					while(dgsMakeBtnList[i] != currentDgsMakeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentDgsMakeBtn = dgsMakeBtnList[i];
					SetFocus(currentDgsMakeBtn);
					gCurrentFocusBtn = currentDgsMakeBtn;
				}				
				else if(wParam == VK_DOWN)
				{
					while(dgsMakeBtnList[i] != currentDgsMakeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentDgsMakeBtn = dgsMakeBtnList[i];
					SetFocus(currentDgsMakeBtn);
					gCurrentFocusBtn = currentDgsMakeBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfPanHuiGain[CurrentChannelNo] != MIN_CURVE_GAIN)
					{
						if(gfPanHuiGain[CurrentChannelNo] - fCurveGainStep > gfDingLiangGain[CurrentChannelNo])
						{
							gfPanHuiGain[CurrentChannelNo] -= fCurveGainStep;
						}

						displayCurveGain(Wnd, gfPanHuiGain[CurrentChannelNo]);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfPanHuiGain[CurrentChannelNo] != MAX_CURVE_GAIN)
					{
						if(gfPanHuiGain[CurrentChannelNo] + fCurveGainStep < MAX_CURVE_GAIN)
						{
							gfPanHuiGain[CurrentChannelNo] += fCurveGainStep;
						}
						else
							gfPanHuiGain[CurrentChannelNo] = MAX_CURVE_GAIN;

						displayCurveGain(Wnd, gfPanHuiGain[CurrentChannelNo]);
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
				else if(wParam == 'F')
				{
					if(wCurveGainStepIndex < 1)
						wCurveGainStepIndex++;
					else
						wCurveGainStepIndex = 0;

					fCurveGainStep = CURVEGAINSTEPS[wCurveGainStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fCurveGainStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDgsMakePanHuiProc,Wnd,Msg,wParam,lParam);
}
