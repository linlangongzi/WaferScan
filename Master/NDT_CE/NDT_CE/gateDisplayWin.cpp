/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * բ�Ŵ���Դ�ļ�
 * Filename: gateDisplayWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "gateDisplayWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"


HWND hWndGateDisplay;					//բ�Ŵ��ھ��
HWND hBtnGateDisplayGateCfg;			//բ�����ð�ť���
HWND hBtnGateDisplaySelect;				//բ��ѡ��ť���
HWND hBtnGateDisplayQueXianDaXiao;		//ȱ�ݴ�С��ť���
HWND hBtnGateDisplayShengChengWeiZhi;	//����λ�ð�ť���
HWND hBtnGateDisplayShenDuWeiZhi;		//���λ�ð�ť���
HWND hBtnGateDisplayshuiPingWeiZhi;	// ˮƽλ�ð�ť���

HWND gateDisplayBtnList[2];							//բ�Ŵ��ڵİ�ť��
WNDPROC OldBtnGateDisplayGateCfgProc = NULL;			//բ�����ð�ťĬ�ϴ������
WNDPROC OldBtnGateDisplaySelectProc = NULL;				//բ��ѡ��ťĬ�ϴ������
WNDPROC OldBtnGateDisplayQueXianDaXiaoProc = NULL;		//ȱ�ݴ�С��ťĬ�ϴ������
WNDPROC OldBtnGateDisplayShengChengWeiZhiProc = NULL;	//����λ�ð�ťĬ�ϴ������
WNDPROC OldBtnGateDisplayShenDuWeiZhiProc = NULL;		//���λ�ð�ťĬ�ϴ������
WNDPROC OldBtnGateDisplayshuiPingWeiZhiProc = NULL;	//ˮƽλ�ð�ťĬ�ϴ������

 
HWND currentGateDisplayBtn;					//բ�Ŵ��е�ǰѡ�еİ�ť

static const int buttonNum = 2;		//��ť�ؼ���

//WORD  wGateIndex = 0;		//բ������
WORD gwGateIndex = 0;		//բ������

static void displayGagteIndex(HWND hWnd, WORD wGateIndex);
static void displayFaultInfrom(WORD wGateIndex);
void displayGateFault(double gateDefectAmp, double gateDefectTime, float fFuZhi);

extern HINSTANCE hInst;		//����ʵ�����

extern HWND hWndMain;

extern HWND hWndGate;
extern HWND hWndParam;				// �������ھ��
extern HWND currentParamBtn;		// �������ڵ�ǰѡ�еİ�ť
extern HWND gCurrentFocusBtn;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern SEND_PARAM_DATA SendParam_ad[2];
extern WORD CurrentChannelNo;
extern float fRangeEnd[2];
extern float fRangeStart[2];
extern GATE_INFO GateInfos[2][3];
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���
extern CRITICAL_SECTION csPeakDefectInfos;
extern PEAK_INFO gPeakDefectInfos[2][2];

extern WORD gwDeviceType;	//�豸���� 0:̽����	1:�����
extern HWND hWndChParam;			//���������ھ��
extern HWND currentChParamBtn;		//���������е�ǰѡ�еİ�ť

extern HWND hWndDgsCurve;
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];				//DGSͨ������
extern void displayDacFault(double gateDefectAmp, double gateDefectTime);
extern WORD GateIndex;
/**************************
*ע��բ�Ŵ���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerGateDisplayChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) gateDisplayChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szGateDisplayChild;

	return RegisterClass(&wc);
}
static void displayGagteIndex(HWND hWnd,  WORD nGateIndex)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	swprintf(strText, _T("%d"), nGateIndex);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, 
						STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

	displayFaultInfrom(nGateIndex);
}
/**************************
*��ʾȱ��
* @param		gateDefectAmp[in]:�����˷�
* @param		gateDefectTime[in]:������ʱ
* @return   LRESULT :
* @since    1.00
***************************/
void displayGateFault(double gateDefectAmp, double gateDefectTime, float fFuZhi)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	float shengChengWeiZhi = 0;			//����λ��
	float shenDuWeiZhi = 0;					//���λ��
	float shuiPingWeiZhi = 0;				//ˮƽλ��
	float queXianDaXiao = 0;				//ȱ�ݴ�С

	double fAngle = DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI;
	if(hWndGateDisplay)
	{
		shengChengWeiZhi = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)gateDefectTime);
		swprintf(strText, _T("%.1f"), shengChengWeiZhi);
		DrawLableText(hBtnGateDisplayShengChengWeiZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
						
		memset(strText, 0, sizeof(strText));
		shenDuWeiZhi = (float)(shengChengWeiZhi * cos(fAngle));
		swprintf(strText, _T("%.1f"), shenDuWeiZhi);
		DrawLableText(hBtnGateDisplayShenDuWeiZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
						
		memset(strText, 0, sizeof(strText));
		shuiPingWeiZhi = (float)(shengChengWeiZhi * sin(fAngle) - SendParam_ad[CurrentChannelNo].fQianYan);
		swprintf(strText, _T("%.1f"), shuiPingWeiZhi);
		DrawLableText(hBtnGateDisplayshuiPingWeiZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

		memset(strText, 0, sizeof(strText));
		
		if(fFuZhi)
		{
			if(gateDefectAmp)
				queXianDaXiao = (float)(20 * log10(((float)gateDefectAmp/MAX_HEIGHT*100.0f) / fFuZhi));
				
		}
		swprintf(strText, _T("%.1f"), queXianDaXiao);
		DrawLableText(hBtnGateDisplayQueXianDaXiao, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}
}
/**************************
*��ʾȱ����Ϣ
* @param		wGateIndex[in]: DGS����
* @return   LRESULT :
* @since    1.00
***************************/
static void displayFaultInfrom(WORD wGateIndex)
{
	PEAK_INFO peakDefectInfos[2][2];
	InitializeCriticalSection(&csPeakDefectInfos);
	EnterCriticalSection(&csPeakDefectInfos);
	for(int i = 0 ;i < CHANNEL_NUM; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			peakDefectInfos[i][j] = gPeakDefectInfos[i][j];
		}
	}
	LeaveCriticalSection(&csPeakDefectInfos);
	DeleteCriticalSection(&csPeakDefectInfos);
	
	switch(wGateIndex)
	{
		case 0:
			displayGateFault(peakDefectInfos[CurrentChannelNo][0].wDefectAmp, peakDefectInfos[CurrentChannelNo][0].wDefectTime, 
								GateInfos[CurrentChannelNo][0].fPeakGate);
			break;
		case 1:
			displayGateFault(peakDefectInfos[CurrentChannelNo][1].wDefectAmp, peakDefectInfos[CurrentChannelNo][1].wDefectTime, 
								GateInfos[CurrentChannelNo][1].fPeakGate);
			break;
	/*	case 2:
	//		displayGateFault(chCurrent_gate_reg->wGate3DefectAmp, chCurrent_gate_reg->wGate3DefectTime, 
	//							GateInfos[CurrentChannelNo][2].fPeakGate);
			break;*/
		default:
			break;
			
	}
}
void CALLBACK defectDisplayProc( HWND hwnd, 
							  UINT uMsg, 
							  UINT idEvent, 
							  DWORD dwTime 
							)
{
	PEAK_INFO peakDefectInfos[2][2];
	InitializeCriticalSection(&csPeakDefectInfos);
	EnterCriticalSection(&csPeakDefectInfos);
	for(int i = 0 ;i < CHANNEL_NUM; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			peakDefectInfos[i][j] = gPeakDefectInfos[i][j];
		}
	}
	LeaveCriticalSection(&csPeakDefectInfos);
	DeleteCriticalSection(&csPeakDefectInfos);
	if(hWndGateDisplay)
	{
		if(GateInfos[CurrentChannelNo][GateIndex - 1].bOpen)
		{
			if(GateIndex == 1)
			{
				displayGateFault(peakDefectInfos[CurrentChannelNo][0].wDefectAmp, peakDefectInfos[CurrentChannelNo][0].wDefectTime, 
									GateInfos[CurrentChannelNo][0].fPeakGate);
			}
			else if(GateIndex == 2)
			{
				displayGateFault(peakDefectInfos[CurrentChannelNo][1].wDefectAmp, peakDefectInfos[CurrentChannelNo][1].wDefectTime, 
									GateInfos[CurrentChannelNo][1].fPeakGate);
			}
		}
	}
	else if(hWndDgsCurve)
	{
		if(gDgsSwitch[CurrentChannelNo][0])
		{
			if(GateInfos[CurrentChannelNo][0].bOpen)
			{
				displayDacFault(peakDefectInfos[CurrentChannelNo][0].wDefectAmp, peakDefectInfos[CurrentChannelNo][0].wDefectTime);
			}
		}
	}
}

/**************************
*բ�Ŵ��ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK gateDisplayChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
		//	closeAllGate();//�ر�����բ��
			SetTimer(hWnd, TIMER_GATE_DEFECT_DISPLAY, 200, defectDisplayProc);
			//����ť�ؼ�
			hBtnGateDisplayGateCfg = CreateWindow(_T("BUTTON"),_T("gateCfgGateDisplay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_GATEDISPLAY_DGSGATECFG,hInst,NULL);
			hBtnGateDisplaySelect = CreateWindow(_T("BUTTON"),_T("selectGateDisplay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_GATEDISPLAY_SELECT,hInst,NULL);
			hBtnGateDisplayQueXianDaXiao = CreateWindow(_T("BUTTON"),_T("queXianDaXiaoGateDisplay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_GATEDISPLAY_QUEXIANDAXIAO,hInst,NULL);
			hBtnGateDisplayShengChengWeiZhi = CreateWindow(_T("BUTTON"),_T("ShengChengWeiZhiGateDisplay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_GATEDISPLAY_SHENGCHENGWEIZHI,hInst,NULL);
			hBtnGateDisplayShenDuWeiZhi = CreateWindow(_T("BUTTON"),_T("meiZhanKuanGateDisplay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_GATEDISPLAY_SHENDUWEIZHI,hInst,NULL);
			hBtnGateDisplayshuiPingWeiZhi = CreateWindow(_T("BUTTON"),_T("shuiPingWeiZhiGateDisplay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 5*BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_GATEDISPLAY_SHUIPINGWEIZHI,hInst,NULL);
					
			gateDisplayBtnList[0] = hBtnGateDisplayGateCfg;
			gateDisplayBtnList[1] = hBtnGateDisplaySelect;
			gateDisplayBtnList[2] = hBtnGateDisplayQueXianDaXiao;
			gateDisplayBtnList[3] = hBtnGateDisplayShengChengWeiZhi;
			gateDisplayBtnList[4] = hBtnGateDisplayShenDuWeiZhi;
			gateDisplayBtnList[5] = hBtnGateDisplayshuiPingWeiZhi;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnGateDisplayGateCfgProc = (WNDPROC) GetWindowLong(hBtnGateDisplayGateCfg,GWL_WNDPROC);
			SetWindowLong(hBtnGateDisplayGateCfg,GWL_WNDPROC,(LONG)btnGateDisplayGateCfgProc);

			OldBtnGateDisplaySelectProc = (WNDPROC) GetWindowLong(hBtnGateDisplaySelect,GWL_WNDPROC);
			SetWindowLong(hBtnGateDisplaySelect,GWL_WNDPROC,(LONG)btnGateDisplaySelectProc);
			
			OldBtnGateDisplayQueXianDaXiaoProc = (WNDPROC) GetWindowLong(hBtnGateDisplayQueXianDaXiao,GWL_WNDPROC);
			SetWindowLong(hBtnGateDisplayQueXianDaXiao,GWL_WNDPROC,(LONG)btnGateDisplayQueXianDaXiaoProc);
			
			OldBtnGateDisplayShengChengWeiZhiProc = (WNDPROC) GetWindowLong(hBtnGateDisplayShengChengWeiZhi,GWL_WNDPROC);
			SetWindowLong(hBtnGateDisplayShengChengWeiZhi,GWL_WNDPROC,(LONG)btnGateDisplayShengChengWeiZhiProc);

			OldBtnGateDisplayShenDuWeiZhiProc = (WNDPROC) GetWindowLong(hBtnGateDisplayShenDuWeiZhi, GWL_WNDPROC);
			SetWindowLong(hBtnGateDisplayShenDuWeiZhi,GWL_WNDPROC,(LONG)btnGateDisplayShenDuWeiZhiProc);
			
			OldBtnGateDisplayshuiPingWeiZhiProc = (WNDPROC) GetWindowLong(hBtnGateDisplayshuiPingWeiZhi,GWL_WNDPROC);
			SetWindowLong(hBtnGateDisplayshuiPingWeiZhi,GWL_WNDPROC,(LONG)btnGateDisplayshuiPingWeiZhiProc);
		
			currentGateDisplayBtn = gateDisplayBtnList[0];//Ĭ�ϵ�ǰѡ��բ�����ð�ť
			SetFocus(currentGateDisplayBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentGateDisplayBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				if(gwDeviceType == 0 || gwDeviceType == 2) //̽�˻���·
				{
					ShowWindow(hWndParam,SW_SHOW);
					SetFocus(currentParamBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��				
				
					gCurrentFocusBtn = currentParamBtn;
					gCurrentWnd = hWndParam;
				}
				else if(gwDeviceType == 1)//���
				{
					ShowWindow(hWndChParam,SW_SHOW);
					SetFocus(currentChParamBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					
					gCurrentFocusBtn = currentChParamBtn;
					gCurrentWnd = hWndChParam;
				}
				DestroyWindow(hWndGateDisplay);
				hWndGateDisplay = 0;

				KillTimer(hWnd, TIMER_GATE_DEFECT_DISPLAY);
			}
			break;
		case WM_COMMAND:

			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			gateDisplay_DrawButtonBitmap(hInst,pdis);
			
			if(pdis->hwndItem == hBtnGateDisplaySelect)
			{				
				displayGagteIndex(pdis->hwndItem, GateIndex);
			}

			if(pdis->hwndItem == hBtnGateDisplayQueXianDaXiao || pdis->hwndItem == hBtnGateDisplayShengChengWeiZhi
				|| pdis->hwndItem == hBtnGateDisplayShenDuWeiZhi || pdis->hwndItem == hBtnGateDisplayshuiPingWeiZhi)
			{
				displayFaultInfrom(GateIndex - 1);
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
*����բ�Ŵ��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int gateDisplay_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_gateDisplayBtnBitmap[]=
	{
		{IDC_GATEDISPLAY_DGSGATECFG,IDB_BITMAP_ZHAMENSHEDING_NOSEL_R,	IDB_BITMAP_ZHAMENSHEDING_SELECT_R		},
		{IDC_GATEDISPLAY_SELECT	,IDB_BITMAP_ZHAMENXUANZE_NOSEL_R	 ,  IDB_BITMAP_ZHAMENXUANZE_SELECT_R		},
		{IDC_GATEDISPLAY_QUEXIANDAXIAO	,IDB_BITMAP_ZHAMENQUEXIANDAXIAO_NOSEL_R ,  IDB_BITMAP_ZHAMENQUEXIANDAXIAO_SELECT_R	},
		{IDC_GATEDISPLAY_SHENGCHENGWEIZHI	,IDB_BITMAP_ZHAMENSHENGCHENGWEIZHI_NOSEL_R ,  IDB_BITMAP_ZHAMENSHENGCHENGWEIZHI_SELECT_R	},
		{IDC_GATEDISPLAY_SHENDUWEIZHI	,IDB_BITMAP_ZHAMENSHENDUWEIZHI_NOSEL_R ,  IDB_BITMAP_ZHAMENSHENDUWEIZHI_SELECT_R	},
		{IDC_GATEDISPLAY_SHUIPINGWEIZHI	,IDB_BITMAP_ZHAMENSHUIPINGWEIZHI_NOSEL_R ,  IDB_BITMAP_ZHAMENSHUIPINGWEIZHI_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_gateDisplayBtnBitmap);
	return ReturnVal;
}
/**************************
*բ�����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnGateDisplayGateCfgProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(hWnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);	
				}
			
				else if(wParam == VK_UP)
				{
					while(gateDisplayBtnList[i] != currentGateDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentGateDisplayBtn = gateDisplayBtnList[i];
					SetFocus(currentGateDisplayBtn);
					gCurrentFocusBtn = currentGateDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(gateDisplayBtnList[i] != currentGateDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentGateDisplayBtn = gateDisplayBtnList[i];
					SetFocus(currentGateDisplayBtn);
					gCurrentFocusBtn = currentGateDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{	
					hWndGate= CreateWindow(szGateChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndGateDisplay,SW_HIDE);
					//SetFocus(hWndGate);
					ShowWindow(hWndGate,SW_SHOW);
					UpdateWindow(hWndGate);

					gCurrentWnd = hWndGate;
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
					displayFaultInfrom(GateIndex - 1);
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
	return CallWindowProc(OldBtnGateDisplayGateCfgProc,hWnd,Msg,wParam,lParam);
}
/**************************
*բ��ѡ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnGateDisplaySelectProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(hWnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(gateDisplayBtnList[i] != currentGateDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentGateDisplayBtn = gateDisplayBtnList[i];
					SetFocus(currentGateDisplayBtn);
					gCurrentFocusBtn = currentGateDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(gateDisplayBtnList[i] != currentGateDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentGateDisplayBtn = gateDisplayBtnList[i];
					SetFocus(currentGateDisplayBtn);
					gCurrentFocusBtn = currentGateDisplayBtn;
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					if(GateIndex == 1)
					{
						GateIndex = 2;						
					}
					else
					{
						GateIndex = 1;
					}
					displayGagteIndex(hWnd, GateIndex);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
					displayFaultInfrom(GateIndex - 1);
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
	return CallWindowProc(OldBtnGateDisplaySelectProc,hWnd,Msg,wParam,lParam);
}
/**************************
*ȱ�ݴ�С��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnGateDisplayQueXianDaXiaoProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(hWnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(gateDisplayBtnList[i] != currentGateDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentGateDisplayBtn = gateDisplayBtnList[i];
					SetFocus(currentGateDisplayBtn);
					gCurrentFocusBtn = currentGateDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(gateDisplayBtnList[i] != currentGateDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentGateDisplayBtn = gateDisplayBtnList[i];
					SetFocus(currentGateDisplayBtn);
					gCurrentFocusBtn = currentGateDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
					displayFaultInfrom(GateIndex - 1);
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
	return CallWindowProc(OldBtnGateDisplayQueXianDaXiaoProc,hWnd,Msg,wParam,lParam);
}
/**************************
*����λ�ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnGateDisplayShengChengWeiZhiProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(hWnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(gateDisplayBtnList[i] != currentGateDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentGateDisplayBtn = gateDisplayBtnList[i];
					SetFocus(currentGateDisplayBtn);
					gCurrentFocusBtn = currentGateDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(gateDisplayBtnList[i] != currentGateDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentGateDisplayBtn = gateDisplayBtnList[i];
					SetFocus(currentGateDisplayBtn);
					gCurrentFocusBtn = currentGateDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
					displayFaultInfrom(GateIndex - 1);
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
	return CallWindowProc(OldBtnGateDisplayShengChengWeiZhiProc,hWnd,Msg,wParam,lParam);
}
/**************************
*���λ�ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnGateDisplayShenDuWeiZhiProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(hWnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(gateDisplayBtnList[i] != currentGateDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentGateDisplayBtn = gateDisplayBtnList[i];
					SetFocus(currentGateDisplayBtn);
					gCurrentFocusBtn = currentGateDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(gateDisplayBtnList[i] != currentGateDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentGateDisplayBtn = gateDisplayBtnList[i];
					SetFocus(currentGateDisplayBtn);
					gCurrentFocusBtn = currentGateDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
					displayFaultInfrom(GateIndex - 1);
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
	return CallWindowProc(OldBtnGateDisplayShenDuWeiZhiProc,hWnd,Msg,wParam,lParam);
}
/**************************
*ˮƽλ�ð�ť�Զ��崦�����
* @param		hInstance[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnGateDisplayshuiPingWeiZhiProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(hWnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(gateDisplayBtnList[i] != currentGateDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentGateDisplayBtn = gateDisplayBtnList[i];
					SetFocus(currentGateDisplayBtn);
					gCurrentFocusBtn = currentGateDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(gateDisplayBtnList[i] != currentGateDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentGateDisplayBtn = gateDisplayBtnList[i];
					SetFocus(currentGateDisplayBtn);
					gCurrentFocusBtn = currentGateDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
					displayFaultInfrom(GateIndex - 1);
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
	return CallWindowProc(OldBtnGateDisplayshuiPingWeiZhiProc,hWnd,Msg,wParam,lParam);
}
