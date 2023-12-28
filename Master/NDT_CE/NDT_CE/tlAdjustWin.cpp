/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ��У����Դ�ļ�
 * Filename: chAdjustWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       zhaoli
 * @version      1.00         Date: 2011-01-18
 */
#include "stdafx.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "gateWin.h"
#include "recvParamWin.h"

HWND hWndTlAdjust = NULL;	//��У���ھ��
HWND currentTlAdjustBtn = NULL;	//��У���ڵ�ǰѡ�а�ť

HWND hBtnTlAdjustWaveSpeedZero = NULL; //����������У���ڰ�ť���
HWND hBtnTlAdjustDacWave = NULL;		//DAC���ߵ�У���ڰ�ť���
HWND hBtnTlAdjustSensitive = NULL;			//�������趨��У���ڰ�ť���
HWND hBtnTlAdjustBiaoMianBuChang = NULL;	//���油����У���ڰ�ť���
HWND hBtnTlAdjustOther = NULL;			//������У���ڰ�ť���

static HWND TlAdjustBtnList[5] = {NULL, NULL, NULL, NULL, NULL};				//��У���ڰ�ť��

static WNDPROC OldBtnTlAdjustWaveSpeedZeroProc = NULL;		//����������У��ťĬ�ϴ������
static WNDPROC OldBtnTlAdjustDacWaveProc = NULL;			//DAC���ߵ�У��ťĬ�ϴ������
static WNDPROC OldBtnTlAdjustSensitiveProc = NULL;				//�������趨��У���ڰ�ťĬ�ϴ������
static WNDPROC OldBtnTlAdjustBiaoMianBuChangProc = NULL;	//���油����У���ڰ�ťĬ�ϴ������
static WNDPROC OldBtnTlAdjustOtherProc = NULL;				//������У���ڰ�ťĬ�ϴ������

static const int buttonNum = 5;		//��ť����
static WORD wGainBuChangStepIndex = 1;
static float fGainBuChangStep = 0;

float gfTlGainBuChang[CHANNEL_NUM] = {0, 0};//���油��

extern HWND gCurrentFocusBtn;		//
extern HINSTANCE hInst;				//����ʵ�����
extern HWND hWndTlMenu;				//�ϼ��˵����ھ��
extern HWND currentTlMenuBtn;		//�ϼ��˵���ǰѡ�а�ť
extern HWND hWndMain;
extern HWND hWndDgsCurve;			//DAC���ߴ��ھ��
extern HWND hWndTsMenu;				//̽��ģʽ���ھ��
extern HWND hWndAdjust;				//У׼���ھ��
extern HWND gCurrentWnd;			//��ǰ��ʾ���Ӵ���

ATOM registerTlAdjustChild(HINSTANCE hInstance);
static LRESULT CALLBACK TlAdjustChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int TlAdjust_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnTlAdjustWaveSpeedZeroProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnTlAdjustDacWaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnTlAdjustSensitiveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnTlAdjustBiaoMianBuChangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnTlAdjustOtherProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

extern WORD gwStandard[CHANNEL_NUM];		//����׼
extern WORD gwPosition[CHANNEL_NUM][TL_STANDARD_NUM];	//��ⲿλ
extern void SetRangeLableAndExecute(HWND hWnd, const float& fRange, const BOOL& bSave);
extern HANDLE hDrawAutoGainThread;				// �Զ�����̾��
extern DWORD dwDrawAutoGainThreadID;			// �Զ������߳�ID
extern THREAD_PARAM stAutoGainThread_param;		// �Զ������̲߳���

extern BOOL gbPeakRememberFlag;  	//��ֵ�����Ƿ���
extern CRITICAL_SECTION csPeakRemember;
extern DWORD WINAPI autoGainProc(LPVOID lpParameter);
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];

extern WORD CurrentChannelNo;				// ��ǰͨ����
extern wchar_t gszTextPeak[20];
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;					// ״̬�����

extern BOOL gbTlSheDingFlag[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//����׼����λ�Ƿ����趨��־
extern float gfTlRange[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//����׼����λ����
extern float gfTlPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//����׼����λ����
extern float gfTlBuChangPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//����׼����λ��������
extern wchar_t lpDefaultParamFile[MAX_PATH];
// songchenguang add end 2011-06-22
extern WORD MAX_PLUS;
// songchenguang add end 2011-06-22
extern void SetFnRefPlusLableAndExecute(HWND hWnd);
extern void setRailRoadParam(WORD wChannelNo, BOOL bCurveChangeFlag);
//extern void setCommonTSParam(WORD wChannelNo); // wangbingfu delete at 2011-05-27


void autoAddRefGain(HWND hWnd, WORD wStandard, WORD wPosition)
{
	// �رշ�ֵ����
	InitializeCriticalSection(&csPeakRemember);
	EnterCriticalSection(&csPeakRemember);

	gbPeakRememberFlag = FALSE;

	LeaveCriticalSection(&csPeakRemember);
	DeleteCriticalSection(&csPeakRemember);

	stAutoGainThread_param.hWnd = hWnd;
	if(wStandard == 0)	//TB/T1618
	{
		if(wPosition == 0 || wPosition == 1
			|| wPosition == 2 || wPosition == 3)		//����͸���޶����
		{
		/*	SetRangeLableAndExecute(NULL, 3000);
			SetQiDian(NULL, 1, 2300, 2500, TRUE);
			SetOpenFlag(NULL, TRUE, 1);
		*/		
			stAutoGainThread_param.fValue = 90;
		}
		else if(wPosition == 4 || wPosition == 5
				|| wPosition == 6 || wPosition == 7)
		{
			stAutoGainThread_param.fValue = 50;
		}
		else
		{
			;//
		}
	}
	else if(wStandard == 1 || wStandard == 3)	//TB/T2494.1��2007-98(wStandard == 1 || wStandard == 2 || wStandard == 4)	//TB/T2494.1��TB/T2494.2��2007-98
	{
		/*	SetRangeLableAndExecute(NULL, 3000);
			SetQiDian(NULL, 1, 2300, 2500, TRUE);
			SetOpenFlag(NULL, TRUE, 1);
		*/	
			stAutoGainThread_param.fValue = 80;
	}
	else if(wStandard == 2)//(wStandard == 3) //TB/T 2995
	{
		stAutoGainThread_param.fValue = 50;
	}
	else if(wStandard == 4)//(wStandard == 5) //2009-493
	{
		if(wPosition == 0)
		{
			stAutoGainThread_param.fValue = 90;
		}
		else if(wPosition == 5 || wPosition == 6)
		{
			stAutoGainThread_param.fValue = 50;
		}
		else
		{
			stAutoGainThread_param.fValue = 80;
		}
	}
	else
	{
		;//
	}
	// �����Զ������߳�
	if(hDrawAutoGainThread == NULL)
	{
		// �����߳�		
		hDrawAutoGainThread = ::CreateThread(NULL, 0, autoGainProc, (LPVOID)&stAutoGainThread_param, 0, &dwDrawAutoGainThreadID);
	}
}
void addRefGain(WORD wStandard, WORD wPosition)
{
	//�������油��ֵ
	if(wStandard == 0)	//TB/T1618
	{
		if(wPosition == 0)
		{
			gfTlGainBuChang[CurrentChannelNo] = 10;
		}
		else if(wPosition == 1 || wPosition == 3)
		{
			gfTlGainBuChang[CurrentChannelNo] = 13;
		}
		else if(wPosition == 2)
		{
			gfTlGainBuChang[CurrentChannelNo] = 16;
		}
		else if(wPosition == 4 || wPosition == 5
				|| wPosition == 6)
		{
			gfTlGainBuChang[CurrentChannelNo] = 4;
		}
		else
		{
			;//
		}
	
	}
	else if(wStandard == 1)	//TB/T2494.1
	{
		if(wPosition == 0)
		{
			gfTlGainBuChang[CurrentChannelNo] = 10;
		}
		else if(wPosition == 1)
		{
			gfTlGainBuChang[CurrentChannelNo] = 13;
		}
		else
		{
			;//
		}
	}
/*	else if(wStandard == 2)	//TB/T2494.2
	{
		if(wPosition == 0)		//TS-1͸��
		{
			gfTlGainBuChang[CurrentChannelNo] = 10;
		}
		else if(wPosition == 1)		//����Ա�͸��
		{
			gfTlGainBuChang[CurrentChannelNo] = 14;
		}
		else if(wPosition == 2)		//�ᾱ
		{
			gfTlGainBuChang[CurrentChannelNo] = 2;
		}
		else if(wPosition == 3)		//�������
		{
			gfTlGainBuChang[CurrentChannelNo] = 3;
		}
		else if(wPosition == 4)		//�����ڲ�
		{
			gfTlGainBuChang[CurrentChannelNo] = 3;
		}
		else if(wPosition == 5)		//���������
		{
			gfTlGainBuChang[CurrentChannelNo] = 3;
		}
		else if(wPosition == 6)		//�������ڲ�
		{
			gfTlGainBuChang[CurrentChannelNo] = 3;
		}
		else if(wPosition == 7)		//LZ40���ᴩ͸
		{
			gfTlGainBuChang[CurrentChannelNo] = 12;
		}
		else if(wPosition == 8)		//LZ50���ᴩ͸
		{
			gfTlGainBuChang[CurrentChannelNo] = 14;
		}
		else if(wPosition == 9)		//�Შ̽��
		{
			gfTlGainBuChang[CurrentChannelNo] = 12;
		}
		else
		{
			;//
		}
	}*/
	else if(wStandard == 2)	//TB/T2995
	{
		if(wPosition == 2)
		{
			gfTlGainBuChang[CurrentChannelNo] = 6;
		}
		else
		{
			gfTlGainBuChang[CurrentChannelNo] = 4;
		}
	}
	else if(wStandard == 3)	//2007-98
	{
		if(wPosition == 0)		//ȫ�ᴩ͸RD2-TZSR
		{
			gfTlGainBuChang[CurrentChannelNo] = 45;
		}
		if(wPosition == 1)		//ȫ�ᴩ͸RD2-TS1
		{
			gfTlGainBuChang[CurrentChannelNo] = 10;
		}
		if(wPosition == 2)		//ȫ�ᴩ͸RD2ʵ��
		{
			gfTlGainBuChang[CurrentChannelNo] = 8;
		}
		if(wPosition == 3)		//ȫ�ᴩ͸RE2B
		{
			gfTlGainBuChang[CurrentChannelNo] = 8;
		}
		if(wPosition == 4)		//��������RD2�ᲨTZSR
		{
			gfTlGainBuChang[CurrentChannelNo] = 12;
		}
		if(wPosition == 5)		//��������RD2�Შʵ��
		{
			gfTlGainBuChang[CurrentChannelNo] = 2;
		}
		if(wPosition == 6)		//��������RE2B�Შ
		{
			gfTlGainBuChang[CurrentChannelNo] = 2;
		}
		if(wPosition == 7)		//��������RD2�ݲ�TZSR
		{
			gfTlGainBuChang[CurrentChannelNo] = 6;
		}
		if(wPosition == 8)		//��������RD2�ݲ�ʵ��
		{
			gfTlGainBuChang[CurrentChannelNo] = 6;
		}
		if(wPosition == 9)		//��������RE2B�ݲ�
		{
			gfTlGainBuChang[CurrentChannelNo] = 2;
		}
		if(wPosition == 10)		//�ᾱ��ж�ɲ�RD2-TZSR
		{
			gfTlGainBuChang[CurrentChannelNo] = 6;
		}
		if(wPosition == 11)		//�ᾱ��ж�ɲ�RD2ʵ��
		{
			gfTlGainBuChang[CurrentChannelNo] = 6;
		}
		if(wPosition == 12)		//�ᾱ��ж�ɲ�RE2B
		{
			gfTlGainBuChang[CurrentChannelNo] = 2;
		}
		else
		{
			;//
		}
	}
	else if(wStandard == 4)	//2009-493
	{
		if(wPosition == 0)		//ȫ�ᴩ͸
		{
			gfTlGainBuChang[CurrentChannelNo] = 18;
		}
		if(wPosition == 1)		//�������
		{
			gfTlGainBuChang[CurrentChannelNo] = 6;
		}
		if(wPosition == 2)		//�����ڲ�
		{
			gfTlGainBuChang[CurrentChannelNo] = 9;
		}
		if(wPosition == 3)		//�ƶ�����
		{
			gfTlGainBuChang[CurrentChannelNo] = 9;
		}
		if(wPosition == 4)		//�ᾱ������ж�ɲۣ�
		{
			gfTlGainBuChang[CurrentChannelNo] = 8;
		}
		if(wPosition == 5)		//�����Შ
		{
			gfTlGainBuChang[CurrentChannelNo] = 17;
		}
		if(wPosition == 6)		//�����ݲ�˫��
		{
			gfTlGainBuChang[CurrentChannelNo] = 8;
		}
	}
	else
	{
	}
	//�������油��ֵ
	if(ReceiveParam_ad[CurrentChannelNo].fRefPlus + gfTlGainBuChang[CurrentChannelNo] <= (MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus))
	{
		ReceiveParam_ad[CurrentChannelNo].fRefPlus += gfTlGainBuChang[CurrentChannelNo];

		memset(gszTextPeak, 0, sizeof(gszTextPeak));
		swprintf(gszTextPeak, _T("�趨�ɹ�"));

		SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	}
	else
	{
		gfTlGainBuChang[CurrentChannelNo] = 0;
	//	ReceiveParam_ad[CurrentChannelNo].fRefPlus = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus;
		memset(gszTextPeak, 0, sizeof(gszTextPeak));
		swprintf(gszTextPeak, _T("ʧ��"));

		SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	}
}
/**************************
*ע���У����
* @TlAdjust    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerTlAdjustChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) TlAdjustChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szTlAdjustChild;

	return RegisterClass(&wc);
}


void TlBiaoMianBuChangDisplay(HWND hWnd, float fPlusBuChang)
{
	wchar_t strText[10];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fPlusBuChang);
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);	
}
/**************************
*��У���ڴ������
* @TlAdjust    hInstance[in]:
* @TlAdjust		message[in]:
* @TlAdjust		wParam[in]:
* @TlAdjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK TlAdjustChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch(message)
	{
		case WM_CREATE:
			fGainBuChangStep = RATESTEPS[wGainBuChangStepIndex];
			
			hBtnTlAdjustWaveSpeedZero = CreateWindow(_T("BUTTON"),_T("waveSpeedTlAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TLADJUST_WAVESPEEDZERO,hInst,NULL);
			hBtnTlAdjustDacWave = CreateWindow(_T("BUTTON"),_T("DacWaveTlAdjust"),WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TLADJUST_DACWAVE,hInst,NULL);
			hBtnTlAdjustSensitive = CreateWindow(_T("BUTTON"),_T("SitiveTlAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TLADJUST_SENSITIVE,hInst,NULL);
			hBtnTlAdjustBiaoMianBuChang = CreateWindow(_T("BUTTON"),_T("BiaoMianBuChangTlAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TLADJUST_BIAOMIANBUCHANG,hInst,NULL);
			hBtnTlAdjustOther =  CreateWindow(_T("BUTTON"),_T("OtherTlAdjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TLADJUST_OTHER,hInst,NULL);

			TlAdjustBtnList[0] = hBtnTlAdjustWaveSpeedZero;
			TlAdjustBtnList[1] = hBtnTlAdjustDacWave;
			TlAdjustBtnList[2] = hBtnTlAdjustSensitive;
			TlAdjustBtnList[3] = hBtnTlAdjustBiaoMianBuChang;
			TlAdjustBtnList[4] = hBtnTlAdjustOther;

			OldBtnTlAdjustWaveSpeedZeroProc = (WNDPROC) GetWindowLong(hBtnTlAdjustWaveSpeedZero,GWL_WNDPROC);
			SetWindowLong(hBtnTlAdjustWaveSpeedZero,GWL_WNDPROC,(LONG)btnTlAdjustWaveSpeedZeroProc);

			OldBtnTlAdjustDacWaveProc = (WNDPROC) GetWindowLong(hBtnTlAdjustDacWave,GWL_WNDPROC);
			SetWindowLong(hBtnTlAdjustDacWave,GWL_WNDPROC,(LONG)btnTlAdjustDacWaveProc);

			OldBtnTlAdjustSensitiveProc = (WNDPROC)GetWindowLong(hBtnTlAdjustSensitive,GWL_WNDPROC);
			SetWindowLong(hBtnTlAdjustSensitive,GWL_WNDPROC,(LONG)btnTlAdjustSensitiveProc);

			OldBtnTlAdjustBiaoMianBuChangProc = (WNDPROC)GetWindowLong(hBtnTlAdjustBiaoMianBuChang,GWL_WNDPROC);
			SetWindowLong(hBtnTlAdjustBiaoMianBuChang,GWL_WNDPROC,(LONG)btnTlAdjustBiaoMianBuChangProc);

			OldBtnTlAdjustOtherProc = (WNDPROC)GetWindowLong(hBtnTlAdjustOther,GWL_WNDPROC);
			SetWindowLong(hBtnTlAdjustOther,GWL_WNDPROC,(LONG)btnTlAdjustOtherProc);

			currentTlAdjustBtn = TlAdjustBtnList[0];		//Ĭ�����������Ϊѡ�а�ť
			SetFocus(currentTlAdjustBtn);
			gCurrentFocusBtn = currentTlAdjustBtn;

			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd,&ps);

			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:

			break;
		case WM_COMMAND:

			break;
		case WM_DRAWITEM:
			pdis = (LPDRAWITEMSTRUCT)lParam;
			TlAdjust_DrawButtonBitmap(hInst, pdis);
			if(pdis->hwndItem == hBtnTlAdjustBiaoMianBuChang)
				TlBiaoMianBuChangDisplay(pdis->hwndItem,gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]]);
			break;
		case WM_CHAR:			
			if( wParam == '\t')
			{
				// ���°�ť������
				if(CurrentChannelNo == 0)
				{
					TlBiaoMianBuChangDisplay(hBtnTlAdjustBiaoMianBuChang,gfTlBuChangPlus[1][gwStandard[1]][gwPosition[1][gwStandard[1]]]);
					setRailRoadParam(1, TRUE);
				}
				else
				{
					TlBiaoMianBuChangDisplay(hBtnTlAdjustBiaoMianBuChang,gfTlBuChangPlus[0][gwStandard[0]][gwPosition[0][gwStandard[0]]]);
					setRailRoadParam(0, TRUE);
				}
			}
			SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			break;
		case WM_CLOSE:
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			
	}
	return 0;
}
/**************************
*���ص��̴��ڰ�ťͼƬ
* @tlAdjust    hInstance[in]:
* @tlAdjust		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int TlAdjust_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_tlAdjustBtnBitmap[]=
	{
		{IDC_TLADJUST_WAVESPEEDZERO	,IDB_BITMAP_TL_JIAOZHUN_NOSEL_R	 ,  IDB_BITMAP_TL_JIAOZHUN_SEL_R		},
		{IDC_TLADJUST_DACWAVE	,IDB_BITMAP_TL_DACCURVE_NOSEL_R	 ,  IDB_BITMAP_TL_DACCURVE_SEL_R		},
		{IDC_TLADJUST_SENSITIVE	,IDB_BITMAP_TL_LINGMINDU_NOSEL_R ,  IDB_BITMAP_TL_LINGMINDU_SEL_R	},
		{IDC_TLADJUST_BIAOMIANBUCHANG	,IDB_BITMAP_TL_BIAOMIANBUCHANG_NOSEL_R ,  IDB_BITMAP_TL_BIAOMIANBUCHANG_SEL_R	},
		{IDC_TLADJUST_OTHER	,IDB_BITMAP_TL_OTHERS_NOSEL_R ,IDB_BITMAP_TL_OTHERS_SEL_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_tlAdjustBtnBitmap);
	return ReturnVal;
}
/**************************
*����������У��ť�Զ��崦�����
* @tlAdjust    hInstance[in]:
* @tlAdjust		message[in]:
* @tlAdjust		wParam[in]:
* @tlAdjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnTlAdjustWaveSpeedZeroProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTlMenu,SW_SHOW);
					SetFocus(currentTlMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndTlAdjust);
					hWndTlAdjust = NULL;

					gCurrentFocusBtn = currentTlMenuBtn;

					gCurrentWnd = hWndTlMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(TlAdjustBtnList[i] != currentTlAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTlAdjustBtn = TlAdjustBtnList[i];
					SetFocus(currentTlAdjustBtn);
					gCurrentFocusBtn = currentTlAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(TlAdjustBtnList[i] != currentTlAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTlAdjustBtn = TlAdjustBtnList[i];
					SetFocus(currentTlAdjustBtn);
					gCurrentFocusBtn = currentTlAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndAdjust = CreateWindow(szAdjustChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTlAdjust,SW_HIDE);
					
					ShowWindow(hWndAdjust,SW_SHOW);
					UpdateWindow(hWndAdjust);

					gCurrentWnd = hWndAdjust;
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
	return CallWindowProc(OldBtnTlAdjustWaveSpeedZeroProc,Wnd,Msg,wParam,lParam);
}

/**************************
*DAC���ߵ�У��ť�Զ��崦�����
* @tlAdjust    hInstance[in]:
* @tlAdjust		message[in]:
* @tlAdjust		wParam[in]:
* @tlAdjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnTlAdjustDacWaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTlMenu,SW_SHOW);
					SetFocus(currentTlMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndTlAdjust);
					hWndTlAdjust = NULL;

					gCurrentFocusBtn = currentTlMenuBtn;

					gCurrentWnd = hWndTlMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(TlAdjustBtnList[i] != currentTlAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTlAdjustBtn = TlAdjustBtnList[i];
					SetFocus(currentTlAdjustBtn);
					gCurrentFocusBtn = currentTlAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(TlAdjustBtnList[i] != currentTlAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTlAdjustBtn = TlAdjustBtnList[i];
					SetFocus(currentTlAdjustBtn);
					gCurrentFocusBtn = currentTlAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndDgsCurve = CreateWindow(szDgsCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTlAdjust,SW_HIDE);
					ShowWindow(hWndDgsCurve,SW_SHOW);
					UpdateWindow(hWndDgsCurve);

					gCurrentWnd = hWndDgsCurve;
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
	return CallWindowProc(OldBtnTlAdjustDacWaveProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�������趨��У��ť�Զ��崦�����
* @tlAdjust    hInstance[in]:
* @tlAdjust		message[in]:
* @tlAdjust		wParam[in]:
* @tlAdjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnTlAdjustSensitiveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTlMenu,SW_SHOW);
					SetFocus(currentTlMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndTlAdjust);
					hWndTlAdjust = NULL;

					gCurrentFocusBtn = currentTlMenuBtn;

					gCurrentWnd = hWndTlMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(TlAdjustBtnList[i] != currentTlAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTlAdjustBtn = TlAdjustBtnList[i];
					SetFocus(currentTlAdjustBtn);
					gCurrentFocusBtn = currentTlAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(TlAdjustBtnList[i] != currentTlAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTlAdjustBtn = TlAdjustBtnList[i];
					SetFocus(currentTlAdjustBtn);
					gCurrentFocusBtn = currentTlAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
					//�ȰѲ������潵Ϊ0
					float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
					ReceiveParam_ad[CurrentChannelNo].fRefPlus = MIN_PLUS;
					gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]] = MIN_PLUS;
					SetFnRefPlusLableAndExecute(NULL);
					refGainChangeCurve(fOldRefPlus, TRUE);//wangbingfu add at 2011-03-09
					
					TlBiaoMianBuChangDisplay(hBtnTlAdjustBiaoMianBuChang,gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]]);//�������油��
					
					SetFocus(NULL);
						autoAddRefGain(Wnd, gwStandard[CurrentChannelNo], gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]);
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
				else if(wParam == '8')	//�Զ�����ʧ��
				{
					if(currentTlAdjustBtn == hBtnTlAdjustSensitive)
					{
						SetFocus(currentTlAdjustBtn);
					}

					CloseHandle(hDrawAutoGainThread);
					hDrawAutoGainThread = NULL;
				}
				else if(wParam == '9')		//�Զ�����ɹ�
				{
					if(currentTlAdjustBtn == hBtnTlAdjustSensitive)
					{
						float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
						SetFocus(currentTlAdjustBtn);
				
						addRefGain(gwStandard[CurrentChannelNo], gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]);
						
						//����ñ�׼�ò�λ���趨��־�����̣���׼����
						gbTlSheDingFlag[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]] = TRUE;
						gfTlRange[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]] = DisplaySet_ad[CurrentChannelNo].fRange;
						gfTlPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]] = ReceiveParam_ad[CurrentChannelNo].fPlus;
						gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]] = gfTlGainBuChang[CurrentChannelNo];

						SetFnRefPlusLableAndExecute(NULL);
						refGainChangeCurve(fOldRefPlus, TRUE);//wangbingfu add at 2011-03-09
						TlBiaoMianBuChangDisplay(hBtnTlAdjustBiaoMianBuChang,gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]]);//���²�������
					}

					CloseHandle(hDrawAutoGainThread);
					hDrawAutoGainThread = NULL;
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
	return CallWindowProc(OldBtnTlAdjustSensitiveProc,Wnd,Msg,wParam,lParam);
}
/**************************
*���油����У��ť�Զ��崦�����
* @tlAdjust    hInstance[in]:
* @tlAdjust		message[in]:
* @tlAdjust		wParam[in]:
* @tlAdjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnTlAdjustBiaoMianBuChangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), fGainBuChangStep);
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
					ShowWindow(hWndTlMenu,SW_SHOW);
					SetFocus(currentTlMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndTlAdjust);
					hWndTlAdjust = NULL;

					gCurrentFocusBtn = currentTlMenuBtn;

					gCurrentWnd = hWndTlMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(TlAdjustBtnList[i] != currentTlAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTlAdjustBtn = TlAdjustBtnList[i];
					SetFocus(currentTlAdjustBtn);
					gCurrentFocusBtn = currentTlAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(TlAdjustBtnList[i] != currentTlAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTlAdjustBtn = TlAdjustBtnList[i];
					SetFocus(currentTlAdjustBtn);
					gCurrentFocusBtn = currentTlAdjustBtn;
				}
				else if(wParam == VK_LEFT)
				{
					float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
					if(gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]] - fGainBuChangStep >= 0)
					{
						gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]] -= fGainBuChangStep;
					}
					else
					{
						gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]] = MIN_PLUS;
					}
					TlBiaoMianBuChangDisplay(Wnd, gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]]);
						
					ReceiveParam_ad[CurrentChannelNo].fRefPlus = gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]];
					SetFnRefPlusLableAndExecute(NULL);
					refGainChangeCurve(fOldRefPlus, TRUE);//wangbingfu add at 2011-03-09
				}
				else if(wParam == VK_RIGHT)
				{
					float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
					if(gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]] + fGainBuChangStep <= MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus) 
					{
						gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]] += fGainBuChangStep;
						TlBiaoMianBuChangDisplay(Wnd, gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]]);
						
						ReceiveParam_ad[CurrentChannelNo].fRefPlus = gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]];
						SetFnRefPlusLableAndExecute(NULL);
						refGainChangeCurve(fOldRefPlus, TRUE);//wangbingfu add at 2011-03-09
					}					
				}
				else if(wParam == 'F')
				{
					if(wGainBuChangStepIndex < 1)
						wGainBuChangStepIndex++;
					else
						wGainBuChangStepIndex = 0;

					fGainBuChangStep = RATESTEPS[wGainBuChangStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fGainBuChangStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
	return CallWindowProc(OldBtnTlAdjustWaveSpeedZeroProc,Wnd,Msg,wParam,lParam);
}
/**************************
*������У��ť�Զ��崦�����
* @tlAdjust    hInstance[in]:
* @tlAdjust		message[in]:
* @tlAdjust		wParam[in]:
* @tlAdjust		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnTlAdjustOtherProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTlMenu,SW_SHOW);
					SetFocus(currentTlMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndTlAdjust);
					hWndTlAdjust = NULL;

					gCurrentFocusBtn = currentTlMenuBtn;

					gCurrentWnd = hWndTlMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(TlAdjustBtnList[i] != currentTlAdjustBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTlAdjustBtn = TlAdjustBtnList[i];
					SetFocus(currentTlAdjustBtn);
					gCurrentFocusBtn = currentTlAdjustBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(TlAdjustBtnList[i] != currentTlAdjustBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTlAdjustBtn = TlAdjustBtnList[i];
					SetFocus(currentTlAdjustBtn);
					gCurrentFocusBtn = currentTlAdjustBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndTsMenu = CreateWindow(szTsMenuChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTlAdjust,SW_HIDE);
					
					ShowWindow(hWndTsMenu,SW_SHOW);
					UpdateWindow(hWndTsMenu);

					gCurrentWnd = hWndTsMenu;

				//	setCommonTSParam(CurrentChannelNo);	// wangbingfu delete at 2011-5-27
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
	return CallWindowProc(OldBtnTlAdjustOtherProc,Wnd,Msg,wParam,lParam);
}
