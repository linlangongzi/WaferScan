/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ����¶Ȳ������ߴ�������Դ�ļ�
 * Filename: chTempCurveCreateWin.cpp
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

HWND hWndChTempCurveCreate = NULL;							//����¶Ȳ������ߴ������ھ��
HWND currentChTempCurveCreateBtn = NULL;					//����¶Ȳ������ߴ������ڵ�ǰѡ�еİ�ť
WORD gwTempCurvePointNum[MATERIAL_NUM + 10] = {0,0};						//�¶Ȳ������ߵ�ĸ���
BOOL gbDrawTempCurveCurveFlag = FALSE;					//�Ƿ��ڻ��¶Ȳ�������
TEMPCURVEPOINT_DATA gstTempCurvePointData[MATERIAL_NUM + 10][TEMPCURVE_POINT_MAX];		//�ɼ����¶Ȳ������ߵ�
BOOL gbTempCurveCurveOkFlag = FALSE;					//�¶Ȳ��������Ƿ����

static HWND hBtnChTempCurveCreateHoudu = NULL;					//��Ȱ�ť���
static HWND hBtnChTempCurveCreateTemp = NULL;					//�¶Ȱ�ť���
static HWND hBtnChTempCurveCreateMeasureSpeed = NULL;			//���������ٰ�ť���
static HWND hBtnChTempCurveCreateAddPoint = NULL;				//�����㰴ť���
static HWND hBtnChTempCurveCreateDeletePoint = NULL;			//ɾ���㰴ť���
static HWND hBtnChTempCurveCreateEnd = NULL;					//������ť���

static HWND chTempCurveCreateBtnList[6] = {NULL,NULL,NULL,NULL,NULL,NULL};							//����¶Ȳ������ߴ������ڵİ�ť��
static WNDPROC OldBtnChTempCurveCreateHouduProc = NULL;				//��Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnChTempCurveCreateTempProc = NULL;				//�¶Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnChTempCurveCreateMeasureSpeedProc = NULL;		//���������ٰ�ťĬ�ϴ������
static WNDPROC OldBtnChTempCurveCreateAddPointProc = NULL;			//�����㰴ťĬ�ϴ������
static WNDPROC OldBtnChTempCurveCreateDeletePointProc = NULL;		//ɾ���㰴ťĬ�ϴ������
static WNDPROC OldBtnChTempCurveCreateEndProc = NULL;				//������ťĬ�ϴ������

static const int buttonNum = 6;				//��ť�ؼ���
static float fThickness = 10;			//�Կ���
static float fThicknessStep = 1;		//�Կ��Ȳ���
static WORD wThicknessStepIndex = 1;	//�Կ��Ȳ�������
static float fTemperature = 10;			//�¶�
static float fTemperatureStep = 1;		//�¶Ȳ���
static WORD wTemperatureStepIndex = 1;	//�¶Ȳ�������

static WORD wMeasureWaveSpeed = 0;		//����������
static WORD wDeleteStep = 0;			
static POINT stTempCurvePoint[TEMPCURVE_POINT_MAX];

ATOM registerChTempCurveCreateChild(HINSTANCE hInstance);
static LRESULT CALLBACK chTempCurveCreateChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chTempCurveCreate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChTempCurveCreateHouduProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChTempCurveCreateTempProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChTempCurveCreateMeasureSpeedProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChTempCurveCreateAddPointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChTempCurveCreateDeletePointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChTempCurveCreateEndProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//����ʵ�����
extern HWND hWndTempCurveMake;				//�¶Ȳ��������������ھ�
extern HWND currentTempCurveMakeBtn;		//�¶Ȳ��������������е�ǰѡ�еİ�ť
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND gCurrentWnd;
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;						// ״̬�����
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD CurrentChannelNo;				// ��ǰͨ����
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern WORD gwTempCurveNum;			//�¶Ȳ����������
extern CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// �����̲߳���
extern PEAK_INFO gPeakDefectInfos[2][2];	//����բ�ŷ�ֵ����Ϣ
extern CRITICAL_SECTION csPeakDefectInfos;
extern CRITICAL_SECTION csMenuState;
extern SYS_COLOR_INFO gstSysColor[SYS_COLOR_NUM];
extern int gCurrentSysColorIndex;
extern HWND hBtnTempCurveMakeCreate;//������ť���
extern TCHAR strTempCurveFileName[MAX_PATH];
extern GATE_INFO GateInfos[2][3];
extern GATE_INFO stOldGateInfos[2][3];

extern void SetDistanceGate(const WORD& nIndex);
extern void getTempCurveData();
/**************************
*�����¶Ȳ������ߵ��ļ�
* @param		hWnd[in]:���ھ��
* @param		wNum[in]:���
* @return   LRESULT :
* @since    1.00
***************************/
BOOL saveTempCurveFile(LPCTSTR lpszPathName)
{
	HANDLE hFileWrite;	//�ļ����
	DWORD dwWriteDataSize;
	hFileWrite = CreateFile(lpszPathName,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("�����ļ�ʧ��"));
		return -1;
	}
	for(int i = 0; i < MATERIAL_NUM + 10; i++)
	{
		if(!WriteFile(hFileWrite,&gwTempCurvePointNum[i], sizeof(WORD),&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("д�ļ�ʧ��"));

			CloseHandle(hFileWrite);
			return FALSE;
		}
	}
	for(int i = 0; i < MATERIAL_NUM + 10; i++)
	{
		if(!WriteFile(hFileWrite,gstTempCurvePointData[i], sizeof(TEMPCURVEPOINT_DATA) * TEMPCURVE_POINT_MAX, &dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("д�ļ�ʧ��"));

			CloseHandle(hFileWrite);
			return FALSE;
		}
	}

	CloseHandle(hFileWrite);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("�ļ�����ɹ�"));

	return TRUE;	
}


/**************************
*��ʾ�Կ���
* @param		hWnd[in]:���ھ��
* @param		fHouDu[in]:���
* @return   LRESULT :
* @since    1.00
***************************/
static void displayThickness(HWND hWnd ,float fHouDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fHouDu);	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}
/**************************
*��ʾ�¶�
* @param		hWnd[in]:���ھ��
* @param		fTemp[in]:�¶�
* @return   LRESULT :
* @since    1.00
***************************/
static void displayTemperature(HWND hWnd ,float fTemp)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fTemp);	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}

/**************************
*��ʾ����������
* @param		hWnd[in]:���ھ��
* @param		wWaveSpeed[in]:����
* @return   LRESULT :
* @since    1.00
***************************/
static void displayMeasureSpeed(HWND hWnd ,WORD wWaveSpeed)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(wWaveSpeed == 0)
		wsprintf(strText, _T("N/A"));
	else
		wsprintf(strText, _T("%d"), wWaveSpeed);
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}

/**************************
*ע�����¶Ȳ������ߴ�������
* @chTempCurveCreate    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChTempCurveCreateChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chTempCurveCreateChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChTempCurveCreateChild;

	return RegisterClass(&wc);
}
/**************************
*����¶Ȳ������ߴ������ڴ������
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chTempCurveCreateChildProc(HWND hWnd, UINT message, 
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
			SetDistanceGate(1);
			SetDistanceGate(2);
			wMeasureWaveSpeed = 0;
			if(currentTempCurveMakeBtn == hBtnTempCurveMakeCreate)//����, �������
			{
				gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] = 0;
				memset(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM], 0, sizeof(TEMPCURVE_POINT_MAX) * TEMPCURVE_POINT_MAX);
				//saveTempCurveFile(strTempCurveFileName);
			}
		//	getTempCurveData();
			//TEST
			/*gwTempCurvePointNum[10] = 2;
			gstTempCurvePointData[10][0].fTemperature= 25;
			gstTempCurvePointData[10][0].wWaveSpeed= 5920;
			gstTempCurvePointData[10][1].fTemperature= 80;
			gstTempCurvePointData[10][1].wWaveSpeed= 5979;*/
			
			gwTempCurvePointNum[11] = 2;
			gstTempCurvePointData[11][0].fTemperature= 100;
			gstTempCurvePointData[11][0].wWaveSpeed= 5000;
			gstTempCurvePointData[11][1].fTemperature= 200;
			gstTempCurvePointData[11][1].wWaveSpeed= 6000;
			//TEST
			memset(stTempCurvePoint, 0, sizeof(POINT) * TEMPCURVE_POINT_MAX);
			for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
			{
				stTempCurvePoint[i] = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i]);
			}
			gbTempCurveCurveOkFlag = FALSE;
			
			//����ť�ؼ�
			hBtnChTempCurveCreateHoudu = CreateWindow(_T("BUTTON"),_T("chTempCurveCreateHoudu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCURVECREATE_HOUDU,hInst,NULL);
			hBtnChTempCurveCreateTemp = CreateWindow(_T("BUTTON"),_T("chTempCurveCreateTemp"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0+ BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCURVECREATE_TEMP,hInst,NULL);
			hBtnChTempCurveCreateMeasureSpeed = CreateWindow(_T("BUTTON"),_T("chTempCurveCreateSpeed"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCURVECREATE_MEASURESPEED,hInst,NULL);
			hBtnChTempCurveCreateAddPoint = CreateWindow(_T("BUTTON"),_T("chTempCurveCreateAddPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCURVECREATE_ADDPOINT,hInst,NULL);
			hBtnChTempCurveCreateDeletePoint = CreateWindow(_T("BUTTON"),_T("chTempCurveCreateDeletePoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCURVECREATE_DELETEPOINT,hInst,NULL);
			hBtnChTempCurveCreateEnd = CreateWindow(_T("BUTTON"),_T("chTempCurveCreateEnd"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 5*BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCURVECREATE_END,hInst,NULL);
					
			chTempCurveCreateBtnList[0] = hBtnChTempCurveCreateHoudu;
			chTempCurveCreateBtnList[1] = hBtnChTempCurveCreateTemp;
			chTempCurveCreateBtnList[2] = hBtnChTempCurveCreateMeasureSpeed;
			chTempCurveCreateBtnList[3] = hBtnChTempCurveCreateAddPoint;
			chTempCurveCreateBtnList[4] = hBtnChTempCurveCreateDeletePoint;
			chTempCurveCreateBtnList[5] = hBtnChTempCurveCreateEnd;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnChTempCurveCreateHouduProc = (WNDPROC) GetWindowLong(hBtnChTempCurveCreateHoudu,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCurveCreateHoudu,GWL_WNDPROC,(LONG)btnChTempCurveCreateHouduProc);

			OldBtnChTempCurveCreateTempProc = (WNDPROC) GetWindowLong(hBtnChTempCurveCreateTemp,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCurveCreateTemp,GWL_WNDPROC,(LONG)btnChTempCurveCreateTempProc);
			
			OldBtnChTempCurveCreateMeasureSpeedProc = (WNDPROC) GetWindowLong(hBtnChTempCurveCreateMeasureSpeed,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCurveCreateMeasureSpeed,GWL_WNDPROC,(LONG)btnChTempCurveCreateMeasureSpeedProc);
			
			OldBtnChTempCurveCreateAddPointProc = (WNDPROC) GetWindowLong(hBtnChTempCurveCreateAddPoint,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCurveCreateAddPoint,GWL_WNDPROC,(LONG)btnChTempCurveCreateAddPointProc);
			
			OldBtnChTempCurveCreateDeletePointProc = (WNDPROC) GetWindowLong(hBtnChTempCurveCreateDeletePoint,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCurveCreateDeletePoint,GWL_WNDPROC,(LONG)btnChTempCurveCreateDeletePointProc);
			
			OldBtnChTempCurveCreateEndProc = (WNDPROC) GetWindowLong(hBtnChTempCurveCreateEnd,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCurveCreateEnd,GWL_WNDPROC,(LONG)btnChTempCurveCreateEndProc);
		
			currentChTempCurveCreateBtn = chTempCurveCreateBtnList[0];//Ĭ�ϵ�ǰѡ�в�����Ȱ�ť
			SetFocus(currentChTempCurveCreateBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentChTempCurveCreateBtn;
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
			chTempCurveCreate_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnChTempCurveCreateHoudu)
			{
				displayThickness(pdis->hwndItem,fThickness);
			}
			if(pdis->hwndItem == hBtnChTempCurveCreateTemp)
			{
				displayTemperature(pdis->hwndItem,fTemperature);
			}
			if(pdis->hwndItem == hBtnChTempCurveCreateMeasureSpeed)
			{
				SetDistanceGate(1);
				SetDistanceGate(2);
				displayMeasureSpeed(pdis->hwndItem,wMeasureWaveSpeed);
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
*���ز���¶Ȳ������ߴ������ڰ�ťͼƬ
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chTempCurveCreate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chTempCurveCreateBtnBitmap[]=
	{
		{IDC_CHTEMPCURVECREATE_HOUDU	,IDB_BITMAP_SHIKUAIHOUDU6_NOSEL_R	 ,  IDB_BITMAP_SHIKUAIHOUDU6_SELECT_R		},
		{IDC_CHTEMPCURVECREATE_TEMP	,IDB_BITMAP_WENDU_NOSEL_R	 ,  IDB_BITMAP_WENDU_SELECT_R		},
		{IDC_CHTEMPCURVECREATE_MEASURESPEED	,IDB_BITMAP_CELIANGSHENGSU_NOSEL_R	 ,  IDB_BITMAP_CELIANGSHENGSU_SELECT_R		},	
		{IDC_CHTEMPCURVECREATE_ADDPOINT	,IDB_BITMAP_CHJIAYANGDIAN5_NOSEL_R ,  IDB_BITMAP_CHJIAYANGDIAN5_SELECT_R	},
		{IDC_CHTEMPCURVECREATE_DELETEPOINT	,IDB_BITMAP_SHANYANGDIAN5_NOSEL_R ,  IDB_BITMAP_SHANYANGDIAN5_SELECT_R	},
		{IDC_CHTEMPCURVECREATE_END	,IDB_BITMAP_JIESHU5_NOSEL_R ,  IDB_BITMAP_JIESHU5_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chTempCurveCreateBtnBitmap);
	return ReturnVal;
}

/**************************
*��Ȱ�ť�Զ��崦�����
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCurveCreateHouduProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;

					gCurrentFocusBtn = currentTempCurveMakeBtn;

					gCurrentWnd = hWndTempCurveMake;
					
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					SetOpenFlag(NULL, FALSE, 1);		//�ص�բ��1
					SetOpenFlag(NULL, FALSE, 2);		//�ص�բ��2

					gbDrawTempCurveCurveFlag = TRUE;		//�ı�������
					SetRedrawFlag(TRUE);					//Y������ػ�
					Sleep(100);						//�ȴ�	ȷ���ػ����

					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					//gbEndFlag =TRUE;
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					Sleep(50);//�ȴ���ȷ�ϻ�ͼ�߳�ֹͣ������
					clearWave();//ɾ������
				//	SetRedrawFlag(TRUE);	//�ָ������п��ܱ����ֲ���������Ҫ�ػ� �߳���ͣ��������
					//�����е�
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i],BLUE_POINT, TRUE);
					}
				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(fThickness > 1)
					{
						if((fThickness - fThicknessStep) >= 1)
							fThickness -= fThicknessStep;
						else
							fThickness = 1;

						wMeasureWaveSpeed = 0;
						displayMeasureSpeed(hBtnChTempCurveCreateMeasureSpeed, wMeasureWaveSpeed);
					}
					displayThickness(Wnd, fThickness);

					SetDistanceGate(1);
					SetDistanceGate(2);
				}
				else if(wParam == VK_RIGHT)
				{
					fThickness += fThicknessStep;

					wMeasureWaveSpeed = 0;
					displayMeasureSpeed(hBtnChTempCurveCreateMeasureSpeed, wMeasureWaveSpeed);
					
					displayThickness(Wnd, fThickness);

					SetDistanceGate(1);
					SetDistanceGate(2);
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
	return CallWindowProc(OldBtnChTempCurveCreateHouduProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�¶Ȱ�ť�Զ��崦�����
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCurveCreateTempProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), fTemperatureStep);
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
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;

					gCurrentFocusBtn = currentTempCurveMakeBtn;

					gCurrentWnd = hWndTempCurveMake;
					
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(fTemperature > 0)
					{
						if((fTemperature - fTemperatureStep) >= 0)
							fTemperature -= fTemperatureStep;
						else
							fTemperature = 0;

						wMeasureWaveSpeed = 0;
						displayMeasureSpeed(hBtnChTempCurveCreateMeasureSpeed, wMeasureWaveSpeed);
					}
					displayTemperature(Wnd, fTemperature);
				}
				else if(wParam == VK_RIGHT)
				{
					fTemperature += fTemperatureStep;

					wMeasureWaveSpeed = 0;
					displayMeasureSpeed(hBtnChTempCurveCreateMeasureSpeed, wMeasureWaveSpeed);
					
					displayTemperature(Wnd, fTemperature);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == 'F')
				{
					if(wTemperatureStepIndex < 3)
						wTemperatureStepIndex++;
					else
						wTemperatureStepIndex = 0;

					fTemperatureStep = AMENDSTEPS[wTemperatureStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fTemperatureStep);
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
	return CallWindowProc(OldBtnChTempCurveCreateTempProc,Wnd,Msg,wParam,lParam);
}


/**************************
*���������ٰ�ť�Զ��崦�����
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCurveCreateMeasureSpeedProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;

					gCurrentFocusBtn = currentTempCurveMakeBtn;

					gCurrentWnd = hWndTempCurveMake;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					SetOpenFlag(NULL, FALSE, 1);		//�ص�բ��1
					SetOpenFlag(NULL, FALSE, 2);		//�ص�բ��2

					gbDrawTempCurveCurveFlag = TRUE;		//�ı�������
					SetRedrawFlag(TRUE);					//Y������ػ�
					Sleep(100);								//�ȴ�	ȷ���ػ����
					
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					//gbEndFlag =TRUE;
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					Sleep(50);//�ȴ���ȷ�ϻ�ͼ�߳�ֹͣ������
					clearWave();//ɾ������
					//SetRedrawFlag(TRUE);		//�ָ������п��ܱ����ֲ���������Ҫ�ػ�,�߳���ֹͣ����������
					// ������,�ָ������п��ܱ����ֲ���������Ҫ�ػ�
					RECT RctWave;	// ��������
					RECT RctYScale; // Y��������
					RECT RctXScale; // X��������
					RECT RctRight;	// �Ҳ�հ���
					RECT rect;	// ��������ʾ�����ľ���
					HDC hdc = GetDC(hWndMain);
					GetClipBox(hdc, &rect);
						
					InitializeCriticalSection(&csMenuState);
					EnterCriticalSection(&csMenuState);
					GetDisplayRect(rect, FALSE, RctWave, RctXScale, RctYScale, RctRight);
					LeaveCriticalSection(&csMenuState);
					DeleteCriticalSection(&csMenuState);
					
					DrawRectDashLine(RctWave, RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack));
					//�����е�
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i],BLUE_POINT, TRUE);
					}
					//������
					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], TRUE);
					
				}
				else if(wParam == VK_RETURN)
				{
					float fMeasureHoudu = 0;	//�����ĺ��
				 	float fRang1 = 0;
					float fRang2 = 0;
					PEAK_INFO peakDefectInfos[2][2];
					InitializeCriticalSection(&csPeakDefectInfos);
					EnterCriticalSection(&csPeakDefectInfos);
					for(int i = 0 ;i < CHANNEL_NUM; i++)
					{
						peakDefectInfos[i][0] = gPeakDefectInfos[i][0];
						peakDefectInfos[i][1] = gPeakDefectInfos[i][1];
					}
					LeaveCriticalSection(&csPeakDefectInfos);
					DeleteCriticalSection(&csPeakDefectInfos);
					fRang1 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][0].wDefectTime);
					fRang2 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][1].wDefectTime);
					fMeasureHoudu = fRang2 - fRang1;
					wMeasureWaveSpeed = (WORD)(fMeasureHoudu / (fThickness / DisplaySet_ad[CurrentChannelNo].nWaveSpeed));
					displayMeasureSpeed(hBtnChTempCurveCreateMeasureSpeed, wMeasureWaveSpeed);
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
	return CallWindowProc(OldBtnChTempCurveCreateMeasureSpeedProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�����㰴ť�Զ��崦�����
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCurveCreateAddPointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;

					gCurrentFocusBtn = currentTempCurveMakeBtn;

					gCurrentWnd = hWndTempCurveMake;
					
					gbDrawTempCurveCurveFlag = FALSE;		//��ԭ������
					SetRedrawFlag(TRUE);					//Y������ػ�

					//�������е�
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i], BLUE_POINT, FALSE);
					}
					//��������
					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);
					//��ʼ������
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					gbDrawTempCurveCurveFlag = FALSE;		//��ԭ������
					SetRedrawFlag(TRUE);					//Y������ػ�

					//�������е�
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i], BLUE_POINT, FALSE);
					}
					//��������
					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);
					//��ʼ������
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					SetOpenFlag(NULL, TRUE, 1);//  ��բ����
					SetOpenFlag(NULL, TRUE, 2);//  ��բ����
				}
				else if(wParam == VK_DOWN)
				{
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					if(gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] != 0)
						stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
					drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//ѡ�е㻭��
				}
				else if(wParam == VK_RETURN)
				{
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));
					if(wMeasureWaveSpeed != 0)
					{
						drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);//��������
						
						for(DWORD i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
						{
							if(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i].fTemperature == fTemperature)
							{
								stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i]);
								drawShiZhi1(stTempPoint, BLUE_POINT, FALSE);//����ͬһ�¶ȵĵ�
								
								gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i].wWaveSpeed= wMeasureWaveSpeed;
								stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i]);
								drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);//����
								break;
							}
						}
						gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]++;
						if(gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] > TEMPCURVE_POINT_MAX)
							gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] = TEMPCURVE_POINT_MAX;
						gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1].fTemperature = fTemperature;
						gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1].wWaveSpeed = wMeasureWaveSpeed;
						stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1]);
						drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);//�����ӵĵ�

						sortSelectedPointDataByTemp(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM], gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]);

						for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
						{
							stTempCurvePoint[i] = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i]);
						}
						
						drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], TRUE);//���»�����
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
			/*	else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChTempCurveCreateAddPointProc,Wnd,Msg,wParam,lParam);
}
/**************************
*ɾ���㰴ť�Զ��崦�����
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCurveCreateDeletePointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;

					gCurrentFocusBtn = currentTempCurveMakeBtn;

					gCurrentWnd = hWndTempCurveMake;

					gbDrawTempCurveCurveFlag = FALSE;		//��ԭ������
					SetRedrawFlag(TRUE);					//Y������ػ�

					//�������е�
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i], BLUE_POINT, FALSE);
					}
					//��������
					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);
					//��ʼ������
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);//��������
					//�����е�
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i], BLUE_POINT, TRUE);
					}
				}
				else if(wParam == VK_LEFT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));

					stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
					drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);			//�ϴ�ѡ�е㻹ԭ��ɫ
					if(gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM])
					{						
						if(wDeleteStep == 0)
							wDeleteStep = gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1;
						else
							wDeleteStep--;

						stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
						drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//ѡ�е㻭��
						float fRate = 100 * ((float)gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep].wWaveSpeed -(float) DisplaySet_ad[CurrentChannelNo].nWaveSpeed) / DisplaySet_ad[CurrentChannelNo].nWaveSpeed;
						swprintf(strText, _T("%.1f,%.1f"), gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep].fTemperature, fRate);
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(Wnd, strText, 18, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else if(wParam == VK_RIGHT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));

					stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
					drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);			//�ϴ�ѡ�е㻹ԭ��ɫ
					if(gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM])
					{
						if(wDeleteStep == gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1)
							wDeleteStep = 0;
						else
							wDeleteStep++;

						stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
						drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//ѡ�е㻭��
						float fRate = 100 * ((float)gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep].wWaveSpeed - (float)DisplaySet_ad[CurrentChannelNo].nWaveSpeed) / DisplaySet_ad[CurrentChannelNo].nWaveSpeed;
						swprintf(strText, _T("%.1f,%.1f"), gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep].fTemperature, fRate);
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(Wnd, strText, 18, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else if(wParam == VK_RETURN)
				{
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));

					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);//��������
					
					if(gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM])
					{
						if(gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] == 1)		//ֻ��һ����
						{	
							stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][0]);
							drawShiZhi1(stTempPoint, RED_POINT, FALSE);			//ɾ��ѡ�е�
							gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]--;
						}
						else
						{							
							if(wDeleteStep == (gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1))//ɾ���һ����
							{
								stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
								drawShiZhi1(stTempPoint, RED_POINT, FALSE);			//����ѡ�е�
								gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]--;
								wDeleteStep--;
							}
							else
							{
								for(int i = wDeleteStep; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM] - 1; i++)
								{
									stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
									drawShiZhi1(stTempPoint, RED_POINT, FALSE);			//����ѡ�е�
									gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i] = gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i + 1];//ѡ�е����ĵ�������ǰ��
								}
								gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]--;
							}
							stTempPoint = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][wDeleteStep]);
							drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//�µ�ѡ�е㻭��
							
							for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
							{
								stTempCurvePoint[i] = tempCurveDataToPoint(gstTempCurvePointData[gwTempCurveNum + MATERIAL_NUM][i]);
							}
							
							drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], TRUE);//���»�����
						}
					}
					else
					{
						TCHAR strText[20];
						memset(strText, 0, sizeof(strText));
						swprintf(strText, _T("�������ɾ"));
						DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
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
			/*	else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChTempCurveCreateDeletePointProc,Wnd,Msg,wParam,lParam);
}
/**************************
*ɾ���㰴ť�Զ��崦�����
* @chTempCurveCreate    hInstance[in]:
* @chTempCurveCreate		message[in]:
* @chTempCurveCreate		wParam[in]:
* @chTempCurveCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCurveCreateEndProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;

					gCurrentFocusBtn = currentTempCurveMakeBtn;

					gbDrawTempCurveCurveFlag = FALSE;		//��ԭ������
					SetRedrawFlag(TRUE);					//Y������ػ�

					gCurrentWnd = hWndTempCurveMake;

					//�������е�
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i], BLUE_POINT, FALSE);
					}
					//��������
					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], FALSE);
					//��ʼ������
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					drawCurve1(stTempCurvePoint, gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM], TRUE);//������
				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCurveCreateBtnList[i] != currentChTempCurveCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCurveCreateBtn = chTempCurveCreateBtnList[i];
					SetFocus(currentChTempCurveCreateBtn);
					gCurrentFocusBtn = currentChTempCurveCreateBtn;

					SetOpenFlag(NULL, TRUE, 1);		//��բ��1
					SetOpenFlag(NULL, TRUE, 2);		//��բ��2

					gbDrawTempCurveCurveFlag = FALSE;		//��ԭ������
					SetRedrawFlag(TRUE);					//Y������ػ�

					//�������е�
					for(int i = 0; i < gwTempCurvePointNum[gwTempCurveNum + MATERIAL_NUM]; i++)
					{
						drawShiZhi1(stTempCurvePoint[i], BLUE_POINT, FALSE);
					}
					//��ʼ������
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					SetOpenFlag(NULL, TRUE, 1);//  ��բ����
					SetOpenFlag(NULL, TRUE, 2);//  ��բ����
				}
				else if(wParam == VK_RETURN)
				{
					saveTempCurveFile(strTempCurveFileName);
					
					gbDrawTempCurveCurveFlag = FALSE;		//��ԭ������
					SetRedrawFlag(TRUE);					//Y������ػ�
					
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
					
					gbTempCurveCurveOkFlag = TRUE;
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					DestroyWindow(hWndChTempCurveCreate);
					hWndChTempCurveCreate = 0;
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					SetFocus(currentTempCurveMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					gCurrentFocusBtn = currentTempCurveMakeBtn;
					gCurrentWnd = hWndTempCurveMake;
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
			/*	else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnChTempCurveCreateEndProc,Wnd,Msg,wParam,lParam);
}

