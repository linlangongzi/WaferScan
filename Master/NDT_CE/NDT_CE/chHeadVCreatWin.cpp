/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ���̽ͷV�Ͳ������ߴ�������Դ�ļ�
 * Filename: chHeadVCreateWin.cpp
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

HWND hWndChHeadVCreate = NULL;							//���̽ͷV�Ͳ������ߴ������ھ��
HWND currentChHeadVCreateBtn = NULL;					//���̽ͷV�Ͳ������ߴ������ڵ�ǰѡ�еİ�ť
WORD gwHeadVPointNum[HEAD_NUM + 10] ={0};						//̽ͷV�Ͳ������ߵ�ĸ���
BOOL gbDrawHeadVCurveFlag = FALSE;					//�Ƿ��ڻ�̽ͷV�Ͳ�������
HEADVPOINT_DATA gstHeadVPointData[HEAD_NUM + 10][HEADV_POINT_MAX];		//�ɼ���̽ͷV�Ͳ������ߵ�
BOOL gbHeadVCurveOkFlag = FALSE;					//̽ͷV�Ͳ��������Ƿ����

static HWND hBtnChHeadVCreateActualHoudu = NULL;			//��Ȱ�ť���
static HWND hBtnChHeadVCreateMeasureHoudu = NULL;			//�����ĺ�Ȱ�ť���
static HWND hBtnChHeadVCreateAddPoint = NULL;				//�����㰴ť���
static HWND hBtnChHeadVCreateDeletePoint = NULL;			//ɾ���㰴ť���
static HWND hBtnChHeadVCreateEnd = NULL;					//������ť���

static HWND chHeadVCreateBtnList[5] ={NULL,NULL,NULL,NULL,NULL};							//���̽ͷV�Ͳ������ߴ������ڵİ�ť��
static WNDPROC OldBtnChHeadVCreateMeasureHouduProc = NULL;		//�����ĺ�Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnChHeadVCreateActualHouduProc = NULL;		//��Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnChHeadVCreateAddPointProc = NULL;			//�����㰴ťĬ�ϴ������
static WNDPROC OldBtnChHeadVCreateDeletePointProc = NULL;		//ɾ���㰴ťĬ�ϴ������
static WNDPROC OldBtnChHeadVCreateEndProc = NULL;				//������ťĬ�ϴ������

static const int buttonNum = 5;				//��ť�ؼ���
static float fThickness = 10;			//�Կ���
static float fMeasureThickness = 0;	//�������Կ���
static float fThicknessStep = 1;		//�Կ��Ȳ���
static WORD wThicknessStepIndex = 1;	//�Կ��Ȳ�������
static WORD wDeleteStep = 0;			
static POINT stHeadVPoint[HEADV_POINT_MAX];

ATOM registerChHeadVCreateChild(HINSTANCE hInstance);
static LRESULT CALLBACK chHeadVCreateChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chHeadVCreate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChHeadVCreateMeasureHouduProc( HWND Wnd, 
													UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChHeadVCreateActualHouduProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChHeadVCreateAddPointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChHeadVCreateDeletePointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChHeadVCreateEndProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//����ʵ�����
extern HWND hWndHeadVMake;				//̽ͷV�Ͳ��������������ھ�
extern HWND currentHeadVMakeBtn;		//̽ͷV�Ͳ��������������е�ǰѡ�еİ�ť
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND gCurrentWnd;
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;						// ״̬�����
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD CurrentChannelNo;				// ��ǰͨ����
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern WORD gwHeadVCurveNum;			//̽ͷV�Ͳ����������
extern CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// �����̲߳���
extern PEAK_INFO gPeakDefectInfos[2][2];	//����բ�ŷ�ֵ����Ϣ
extern CRITICAL_SECTION csPeakDefectInfos;
extern CRITICAL_SECTION csMenuState;
extern SYS_COLOR_INFO gstSysColor[SYS_COLOR_NUM];
extern int gCurrentSysColorIndex;
extern TCHAR strHeadVFileName[MAX_PATH];
extern HWND hBtnHeadVMakeCreate;//������ť���
extern GATE_INFO GateInfos[2][3];
extern GATE_INFO stOldGateInfos[2][3];

extern void SetDistanceGate(const WORD& nIndex);

/**************************
*����̽ͷV�Ͳ������ߵ��ļ�
* @param		hWnd[in]:���ھ��
* @param		wNum[in]:���
* @return   LRESULT :
* @since    1.00
***************************/
/*	//�ֳɶ���ļ�
BOOL saveHeadVFile(LPCTSTR lpszPathName)
{
	WORD wHeadVPoinNum = gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM];
	HEADVPOINT_DATA stHeadVpointData[HEADV_POINT_MAX];
	memcpy(stHeadVpointData, gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM], sizeof(HEADVPOINT_DATA)*HEADV_POINT_MAX);
	
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

	if(!WriteFile(hFileWrite,&wHeadVPoinNum, sizeof(WORD),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("д�ļ�ʧ��"));

		CloseHandle(hFileWrite);
		return FALSE;
	}
	if(!WriteFile(hFileWrite,stHeadVpointData, sizeof(HEADVPOINT_DATA) * HEADV_POINT_MAX,&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("д�ļ�ʧ��"));

		CloseHandle(hFileWrite);
		return FALSE;
	}

	CloseHandle(hFileWrite);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("�ļ�����ɹ�"));

	return TRUE;	
}
*/
BOOL saveHeadVFile(LPCTSTR lpszPathName)
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
	for(int i = 0; i < HEAD_NUM + 10; i++)
	{
		if(!WriteFile(hFileWrite,&gwHeadVPointNum[i], sizeof(WORD),&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("д�ļ�ʧ��"));

			CloseHandle(hFileWrite);
			return FALSE;
		}
	}
	for(int i = 0; i < HEAD_NUM + 10; i++)
	{
		if(!WriteFile(hFileWrite,gstHeadVPointData[i], sizeof(HEADVPOINT_DATA) * HEADV_POINT_MAX, &dwWriteDataSize,NULL))
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
* @param		fHouDu[in]:�ְ���
* @return   LRESULT :
* @since    1.00
***************************/
static void displayThickness(HWND hWnd ,float fHouDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(fHouDu == 0)
		wsprintf(strText, _T("N/A"));
	else
		wsprintf(strText, _T("%.1f"), fHouDu);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}

/**************************
*ע����̽ͷV�Ͳ������ߴ�������
* @chHeadVCreate    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChHeadVCreateChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chHeadVCreateChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChHeadVCreateChild;

	return RegisterClass(&wc);
}
/**************************
*���̽ͷV�Ͳ������ߴ������ڴ������
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		message[in]:
* @chHeadVCreate		wParam[in]:
* @chHeadVCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chHeadVCreateChildProc(HWND hWnd, UINT message, 
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
			fMeasureThickness = 0;
			
			if(currentHeadVMakeBtn == hBtnHeadVMakeCreate)//����, �������
			{
				gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] = 0;
				memset(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM], 0, sizeof(HEADV_POINT_MAX) * HEADV_POINT_MAX);
				//saveHeadVFile(strHeadVFileName);
			}
		/*	//TEST
			gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] = 2;
			gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][1].fActualHouDu = 20;
			gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][1].fMeasureHouDu= 40;
			//TEST */
			memset(stHeadVPoint, 0, sizeof(POINT) * HEADV_POINT_MAX);
			for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
			{
				stHeadVPoint[i] = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i]);
			}
			
			gbHeadVCurveOkFlag = FALSE;
			
			//����ť�ؼ�
			hBtnChHeadVCreateActualHoudu = CreateWindow(_T("BUTTON"),_T("ChHeadVCreateActualHoudu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHHEADVCREATE_ACTUALHOUDU,hInst,NULL);
			hBtnChHeadVCreateMeasureHoudu = CreateWindow(_T("BUTTON"),_T("ChHeadVCreateMeasureHoudu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHHEADVCREATE_MEASUREHOUDU,hInst,NULL);			
			hBtnChHeadVCreateAddPoint = CreateWindow(_T("BUTTON"),_T("ChHeadVCreateAddPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHHEADVCREATE_ADDPOINT,hInst,NULL);
			hBtnChHeadVCreateDeletePoint = CreateWindow(_T("BUTTON"),_T("ChHeadVCreateDeletePoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHHEADVCREATE_DELETEPOINT,hInst,NULL);
			hBtnChHeadVCreateEnd = CreateWindow(_T("BUTTON"),_T("ChHeadVCreateEnd"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_CHHEADVCREATE_END,hInst,NULL);
					
			chHeadVCreateBtnList[0] = hBtnChHeadVCreateActualHoudu;
			chHeadVCreateBtnList[1] = hBtnChHeadVCreateMeasureHoudu;
			chHeadVCreateBtnList[2] = hBtnChHeadVCreateAddPoint;
			chHeadVCreateBtnList[3] = hBtnChHeadVCreateDeletePoint;
			chHeadVCreateBtnList[4] = hBtnChHeadVCreateEnd;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnChHeadVCreateMeasureHouduProc = (WNDPROC) GetWindowLong(hBtnChHeadVCreateMeasureHoudu,GWL_WNDPROC);
			SetWindowLong(hBtnChHeadVCreateMeasureHoudu,GWL_WNDPROC,(LONG)btnChHeadVCreateMeasureHouduProc);
			
			OldBtnChHeadVCreateActualHouduProc = (WNDPROC) GetWindowLong(hBtnChHeadVCreateActualHoudu,GWL_WNDPROC);
			SetWindowLong(hBtnChHeadVCreateActualHoudu,GWL_WNDPROC,(LONG)btnChHeadVCreateActualHouduProc);
			
			OldBtnChHeadVCreateAddPointProc = (WNDPROC) GetWindowLong(hBtnChHeadVCreateAddPoint,GWL_WNDPROC);
			SetWindowLong(hBtnChHeadVCreateAddPoint,GWL_WNDPROC,(LONG)btnChHeadVCreateAddPointProc);
			
			OldBtnChHeadVCreateDeletePointProc = (WNDPROC) GetWindowLong(hBtnChHeadVCreateDeletePoint,GWL_WNDPROC);
			SetWindowLong(hBtnChHeadVCreateDeletePoint,GWL_WNDPROC,(LONG)btnChHeadVCreateDeletePointProc);
			
			OldBtnChHeadVCreateEndProc = (WNDPROC) GetWindowLong(hBtnChHeadVCreateEnd,GWL_WNDPROC);
			SetWindowLong(hBtnChHeadVCreateEnd,GWL_WNDPROC,(LONG)btnChHeadVCreateEndProc);
		
			currentChHeadVCreateBtn = chHeadVCreateBtnList[0];//Ĭ�ϵ�ǰѡ�в�����Ȱ�ť
			SetFocus(currentChHeadVCreateBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentChHeadVCreateBtn;
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
			chHeadVCreate_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnChHeadVCreateActualHoudu)
			{
				displayThickness(pdis->hwndItem,fThickness);
			}
			if(pdis->hwndItem == hBtnChHeadVCreateMeasureHoudu)
			{
				displayThickness(pdis->hwndItem,fMeasureThickness);
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
*���ز��̽ͷV�Ͳ������ߴ������ڰ�ťͼƬ
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chHeadVCreate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chHeadVCreateBtnBitmap[]=
	{
		{IDC_CHHEADVCREATE_ACTUALHOUDU	,IDB_BITMAP_SHIKUAIHOUDU5_NOSEL_R	 ,  IDB_BITMAP_SHIKUAIHOUDU5_SELECT_R		},
		{IDC_CHHEADVCREATE_MEASUREHOUDU	,IDB_BITMAP_CELIANGHOUDU_NOSEL_R	 ,  IDB_BITMAP_CELIANGHOUDU_SELECT_R		},
		{IDC_CHHEADVCREATE_ADDPOINT	,IDB_BITMAP_CHJIAYANGDIAN5_NOSEL_R ,  IDB_BITMAP_CHJIAYANGDIAN5_SELECT_R	},
		{IDC_CHHEADVCREATE_DELETEPOINT	,IDB_BITMAP_SHANYANGDIAN5_NOSEL_R ,  IDB_BITMAP_SHANYANGDIAN5_SELECT_R	},
		{IDC_CHHEADVCREATE_END	,IDB_BITMAP_JIESHU5_NOSEL_R ,  IDB_BITMAP_JIESHU5_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chHeadVCreateBtnBitmap);
	return ReturnVal;
}

/**************************
*��Ȱ�ť�Զ��崦�����
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		message[in]:
* @chHeadVCreate		wParam[in]:
* @chHeadVCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChHeadVCreateActualHouduProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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
					ShowWindow(hWndHeadVMake,SW_SHOW);
					SetFocus(currentHeadVMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChHeadVCreate);
					hWndChHeadVCreate = 0;

					gCurrentFocusBtn = currentHeadVMakeBtn;
					gCurrentWnd = hWndHeadVMake;

					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					gbDrawHeadVCurveFlag = TRUE;		//X������ĳ��Կ���
					
					SetOpenFlag(NULL, FALSE, 1);		//�ص�բ��1
					SetOpenFlag(NULL, FALSE, 2);		//�ص�բ��2
					Sleep(100);								//�ȴ�	ȷ���ػ����
					
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					//gbEndFlag =TRUE;
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					Sleep(50);//�ȴ���ȷ�ϻ�ͼ�߳�ֹͣ������
					clearWave();//ɾ������
					//SetRedrawFlag(TRUE);	//�ָ������п��ܱ����ֲ���������Ҫ�ػ�
					//�����е�
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i],BLUE_POINT, TRUE);
					}
				}
				else if(wParam == VK_DOWN)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(fThickness > 1)
					{
						if((fThickness - fThicknessStep) >= 1)
							fThickness -= fThicknessStep;
						else
							fThickness = 1;

						fMeasureThickness = 0;
						displayThickness(hBtnChHeadVCreateMeasureHoudu, fMeasureThickness);
					}
					displayThickness(Wnd, fThickness);

					SetDistanceGate(1);
					SetDistanceGate(2);
				}
				else if(wParam == VK_RIGHT)
				{
					fThickness += fThicknessStep;

					fMeasureThickness = 0;
					displayThickness(hBtnChHeadVCreateMeasureHoudu, fMeasureThickness);
					
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
	return CallWindowProc(OldBtnChHeadVCreateActualHouduProc,Wnd,Msg,wParam,lParam);
}
/**************************
*���������ٰ�ť�Զ��崦�����
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		message[in]:
* @chHeadVCreate		wParam[in]:
* @chHeadVCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChHeadVCreateMeasureHouduProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndHeadVMake,SW_SHOW);
					SetFocus(currentHeadVMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChHeadVCreate);
					hWndChHeadVCreate = 0;

					gCurrentFocusBtn = currentHeadVMakeBtn;
					gCurrentWnd = hWndHeadVMake;
							
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
				}
			
				else if(wParam == VK_UP)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					gbDrawHeadVCurveFlag = TRUE;		//X������ĳ��Կ���

					SetOpenFlag(NULL, FALSE, 1);		//�ص�բ��1
					SetOpenFlag(NULL, FALSE, 2);		//�ص�բ��2
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
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i],BLUE_POINT, TRUE);
					}
					//������
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], TRUE);
					
				}
				else if(wParam == VK_RETURN)
				{
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
					fMeasureThickness = fRang2 - fRang1;
					displayThickness(hBtnChHeadVCreateMeasureHoudu, fMeasureThickness);
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
	return CallWindowProc(OldBtnChHeadVCreateMeasureHouduProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�����㰴ť�Զ��崦�����
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		message[in]:
* @chHeadVCreate		wParam[in]:
* @chHeadVCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChHeadVCreateAddPointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndHeadVMake,SW_SHOW);
					SetFocus(currentHeadVMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChHeadVCreate);
					hWndChHeadVCreate = 0;

					gCurrentFocusBtn = currentHeadVMakeBtn;
					gCurrentWnd = hWndHeadVMake;

					gbDrawHeadVCurveFlag = FALSE;		//X������ĳ�����ֵ					

					//�������е�
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i], BLUE_POINT, FALSE);
					}
					//��������
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);
					//��ʼ������
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
	
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
				}
			
				else if(wParam == VK_UP)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					gbDrawHeadVCurveFlag = FALSE;		//X������ĳ�����ֵ

					//�������е�
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i], BLUE_POINT, FALSE);
					}
					//��������
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);
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
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					if(gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] != 0)
						stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
					drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//ѡ�е㻭��
				}
				else if(wParam == VK_RETURN)
				{
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));
					
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);//��������
					
					for(DWORD i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						if(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i].fActualHouDu == fThickness)
						{
							stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i]);
							drawShiZhi1(stTempPoint, BLUE_POINT, FALSE);//����ͬһ��ȵĵ�
							
							gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i].fMeasureHouDu = fMeasureThickness;
							stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i]);
							drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);//����
							break;
						}
					}
					gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]++;
					if(gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] > HEADV_POINT_MAX)
						gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] = HEADV_POINT_MAX;
					gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1].fActualHouDu = fThickness;
					gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1].fMeasureHouDu = fMeasureThickness;
					stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1]);
					drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);//�����ӵĵ�

					sortSelectedPointDataByHoudu(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM], gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]);

					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						stHeadVPoint[i] = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i]);
					}
					
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + + HEAD_NUM], TRUE);//���»�����
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
	return CallWindowProc(OldBtnChHeadVCreateAddPointProc,Wnd,Msg,wParam,lParam);
}
/**************************
*ɾ���㰴ť�Զ��崦�����
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		message[in]:
* @chHeadVCreate		wParam[in]:
* @chHeadVCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChHeadVCreateDeletePointProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndHeadVMake,SW_SHOW);
					SetFocus(currentHeadVMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChHeadVCreate);
					hWndChHeadVCreate = 0;

					gCurrentFocusBtn = currentHeadVMakeBtn;
					gCurrentWnd = hWndHeadVMake;

					gbDrawHeadVCurveFlag = FALSE;		//X������ĳ�����ֵ

					//�������е�
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i], BLUE_POINT, FALSE);
					}
					//��������
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);
					//��ʼ������
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
				}
			
				else if(wParam == VK_UP)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);//��������
					//�����е�
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i], BLUE_POINT, TRUE);
					}
				}
				else if(wParam == VK_LEFT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));

					stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
					drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);			//�ϴ�ѡ�е㻹ԭ��ɫ
					if(gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM])
					{						
						if(wDeleteStep == 0)
							wDeleteStep = gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1;
						else
							wDeleteStep--;

						stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
						drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//ѡ�е㻭��
						swprintf(strText, _T("%.1f,%.1f"), gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep].fActualHouDu, 
											gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep].fMeasureHouDu);
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
				}
				else if(wParam == VK_RIGHT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));

					stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
					drawShiZhi1(stTempPoint, BLUE_POINT, TRUE);			//�ϴ�ѡ�е㻹ԭ��ɫ
					if(gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM])
					{
						if(wDeleteStep == gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1)
							wDeleteStep = 0;
						else
							wDeleteStep++;

						stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
						drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//ѡ�е㻭��
						swprintf(strText, _T("%.1f,%.1f"), gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep].fActualHouDu, 
											gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep].fMeasureHouDu);
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
				}
				else if(wParam == VK_RETURN)
				{
					POINT stTempPoint;
					memset(&stTempPoint, 0, sizeof(POINT));

					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);//��������
					
					if(gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM])
					{
						if(gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] == 1)		//ֻ��һ����
						{	
							stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][0]);
							drawShiZhi1(stTempPoint, RED_POINT, FALSE);			//ɾ��ѡ�е�
							gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]--;
						}
						else
						{							
							if(wDeleteStep == (gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1))//ɾ���һ����
							{
								stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
								drawShiZhi1(stTempPoint, RED_POINT, FALSE);			//����ѡ�е�
								gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]--;
								wDeleteStep--;
							}
							else
							{
								for(int i = wDeleteStep; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM] - 1; i++)
								{
									stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
									drawShiZhi1(stTempPoint, RED_POINT, FALSE);			//����ѡ�е�
									gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i] = gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i + 1];//ѡ�е����ĵ�������ǰ��
								}
								gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]--;
							}
							stTempPoint = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][wDeleteStep]);
							drawShiZhi1(stTempPoint, RED_POINT, TRUE);			//�µ�ѡ�е㻭��
							
							for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
							{
								stHeadVPoint[i] = headVDataToPoint(gstHeadVPointData[gwHeadVCurveNum + HEAD_NUM][i]);
							}
							
							drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], TRUE);//���»�����
						}
					}
					else
					{
						TCHAR strText[20];
						memset(strText, 0, sizeof(strText));
						swprintf(strText, _T("�������ɾ"));
						DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
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
	return CallWindowProc(OldBtnChHeadVCreateDeletePointProc,Wnd,Msg,wParam,lParam);
}
/**************************
*ɾ���㰴ť�Զ��崦�����
* @chHeadVCreate    hInstance[in]:
* @chHeadVCreate		message[in]:
* @chHeadVCreate		wParam[in]:
* @chHeadVCreate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChHeadVCreateEndProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndHeadVMake,SW_SHOW);
					SetFocus(currentHeadVMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChHeadVCreate);
					hWndChHeadVCreate = 0;

					gCurrentFocusBtn = currentHeadVMakeBtn;
					gCurrentWnd = hWndHeadVMake;

					gbDrawHeadVCurveFlag = FALSE;		//X������ĳ�����ֵ


					//�������е�
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i], BLUE_POINT, FALSE);
					}
					//��������
					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], FALSE);
					//��ʼ������
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
				}
			
				else if(wParam == VK_UP)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					drawCurve1(stHeadVPoint, gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM], TRUE);//������
				}
				else if(wParam == VK_DOWN)
				{
					while(chHeadVCreateBtnList[i] != currentChHeadVCreateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChHeadVCreateBtn = chHeadVCreateBtnList[i];
					SetFocus(currentChHeadVCreateBtn);
					gCurrentFocusBtn = currentChHeadVCreateBtn;

					gbDrawHeadVCurveFlag = FALSE;		//X������ĳ�����ֵ

					SetOpenFlag(NULL, TRUE, 1);		//��բ��1
					SetOpenFlag(NULL, TRUE, 2);		//��բ��2

					//�������е�
					for(int i = 0; i < gwHeadVPointNum[gwHeadVCurveNum + HEAD_NUM]; i++)
					{
						drawShiZhi1(stHeadVPoint[i], BLUE_POINT, FALSE);
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
					saveHeadVFile(strHeadVFileName);

					gbDrawHeadVCurveFlag = FALSE;		//X������ĳ�����ֵ	
					
					gbHeadVCurveOkFlag = TRUE;
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					DestroyWindow(hWndChHeadVCreate);
					hWndChHeadVCreate = 0;
					ShowWindow(hWndHeadVMake,SW_SHOW);
					SetFocus(currentHeadVMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					gCurrentFocusBtn = currentHeadVMakeBtn;
					gCurrentWnd = hWndHeadVMake;
		
					SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
					SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
					SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
					SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
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
	return CallWindowProc(OldBtnChHeadVCreateEndProc,Wnd,Msg,wParam,lParam);
}

