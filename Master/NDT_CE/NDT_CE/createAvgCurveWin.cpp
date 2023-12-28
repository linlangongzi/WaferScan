/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * �½�AVG���ߴ���Դ�ļ�
 * Filename: CreateAvgCurveWin.cpp
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
#include "interfaceCmdID.h"
#include "const.h"
#include "resource.h"
#include "craftParamWin.h"
#include "gateWin.h"
#include "recvParamWin.h"


HWND hWndCreateAvgCurve = NULL;//�½�AVG���ߴ��ھ��
HWND hBtnCreateAvgCurveAddPoint = NULL;//�����㰴ť���
HWND hBtnCreateAvgCurveDeletePoint = NULL;//ɾ���㰴ť���
HWND hBtnCreateAvgCurveEnd = NULL;//������ť���
HWND CreateAvgCurveBtnList[3];//�½�AVG���ߴ��ڵİ�ť��
WNDPROC OldBtnCreateAvgCurveAddPointProc = NULL;//�����㰴ťĬ�ϴ������
WNDPROC OldBtnCreateAvgCurveDeletePointProc = NULL;//ɾ���㰴ťĬ�ϴ������
WNDPROC OldBtnCreateAvgCurveEndProc = NULL;//������ťĬ�ϴ������
HWND currentCreateAvgCurveBtn = NULL;//�½�AVG���ߴ��е�ǰѡ�еİ�ť

CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
CURVEPOINT_DATA gOldMakeAvgPointData[MAX_CURVE_POINT_NUM] = {0};//�Լӵ�һ���׼����Ϊ��׼��������

CURVEPOINT_DATA gCurrentSelectedAvgPointData;//��ӵ������е�ǰѡ�е�AVG���ߵ�����
CURVEPOINT_DATA gPreSelectedAvgPointData;//��ӵ�������ǰһ��ѡ�е�AVG���ߵ�����
WORD gSelectedAvgPointDataNum[CHANNEL_NUM] = {0, 0};//��ѡ�е�AVG������
float gfAvgFirstPointGain[CHANNEL_NUM] = {0, 0};		//��ӵĵ��һ�����Ӧ��Dbֵ
float gfAvgFirstPointChangeGain[CHANNEL_NUM] = {0, 0};			//����ʱ������ı������ֵ

//WORD *gpAvgPixels[SCREEN_DOT_NUM];		//AVG�������ص�

BOOL gWaveDrawAvgFlag = FALSE;		//�����Ƿ�AVG
BOOL gbDeletePointFlag = FALSE;		//�Ƿ���ɾ�������
float gfOldAvgRange[CHANNEL_NUM] = {0, 0};				//�������AVGʱ������
short gwOldAvgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������AVGʱ��ÿ�����λ��
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];

//static int iAddStep = 0;
//static int iDeleteStep = 0;
static POINT stAvgPoint[MAX_CURVE_POINT_NUM] = {0};
WORD gwDeleteStep = 0;		//ɾ����λ��

static const int buttonNum = 3;//��ť�ؼ���
static BOOL gbOldAvgCurveFlag[CHANNEL_NUM];		//����AVG���߿��ر�־
static BOOL gOldDgsSwitch[CHANNEL_NUM][GATE_NUM] = {{0,0},{0,0}};				//����DGS����״̬

ATOM registerCreateAvgCurveChild(HINSTANCE hInstance);
static LRESULT CALLBACK createAvgCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int createAvgCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnCreateAvgCurveAddPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnCreateAvgCurveDeletePointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnCreateAvgCurveEndProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

extern WORD CurrentChannelNo;					// ��ǰͨ����
extern HINSTANCE hInst;							// current instance
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern PEAK_INFO gPeakDefectInfos[2][2];	//����բ�ŷ�ֵ����Ϣ
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern HWND hWndMakeAvgCurve;//����AVG���ߴ��ھ��
extern HWND currentMakeAvgCurveBtn;//����AVG���ߴ��е�ǰѡ�еİ�ť
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���
extern CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// �����̲߳���
extern float fRangeStart[2];
extern float fRangeEnd[2];
extern HWND hBtnMakeAvgCurveCreate;//AVG������ť���
extern HWND hBtnMakeAvgCurveModify;//AVG�޸İ�ť���
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG���߿��ر�־
extern float gfDefectZhi1[CHANNEL_NUM];			//����׾���1
extern float gfDefectZhi2[CHANNEL_NUM];			//��2
extern float gfDefectZhi3[CHANNEL_NUM];			//��3
extern BOOL gbGuanLian;
extern WORD gwAvgType; //AVG����
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern CRITICAL_SECTION csPeakDefectInfos;

extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//DAC��ӵ��������ݶ���
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];//��ѡ�е�DGS������
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];				//DGSͨ������

extern void SetRefPlusLableAndExecute(HWND hWnd, const float& fBasePlus, const float& fRefPlus, const BOOL& bSave, const WORD& chNo);

/**************************
*ע���½�AVG���ߴ���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerCreateAvgCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));

	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) createAvgCurveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szCreateAvgCurveChild;

	return RegisterClass(&wc);
}
/**************************
*�½�AVG���ߴ��ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK createAvgCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			{
				memset(gOldMakeAvgPointData, 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
				//����AVG����״̬
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					gbOldAvgCurveFlag[i] = gbAvgCurveFlag[i];
				}
				//�ر�AVG ����������
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					gbAvgCurveFlag[i] = OFF;
				}
				//����DAC����״̬					
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
					{
						gOldDgsSwitch[i][j] = gDgsSwitch[i][j];
					}
				}
				//�ر�DAC ����������
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
					{
						gDgsSwitch[i][j] = OFF;
					}
				}
				gwDeleteStep = 0;
				gbDeletePointFlag = FALSE;
				float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
				ReceiveParam_ad[CurrentChannelNo].fRefPlus = 0;
				SetRefPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);
				refGainChangeCurve(fOldRefPlus, FALSE);//wangbingfu add at 2011-03-09
				
				gfAvgFirstPointChangeGain[0] = 0;
				gfAvgFirstPointChangeGain[1] = 0;

				if(currentMakeAvgCurveBtn == hBtnMakeAvgCurveCreate)
				{
					gSelectedAvgPointDataNum[CurrentChannelNo] = 0;

					memset(gSelectedAvgPointData[CurrentChannelNo][0], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
					memset(gSelectedAvgPointData[CurrentChannelNo][1], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
					memset(gSelectedAvgPointData[CurrentChannelNo][2], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
					memset(gSelectedAvgPointData[CurrentChannelNo][3], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);

					memset(&gCurrentSelectedAvgPointData, 0, sizeof(CURVEPOINT_DATA));

					memset(gwOldAvgSerial[CurrentChannelNo], 0, sizeof(WORD) * MAX_CURVE_POINT_NUM);
				}
				else if(currentMakeAvgCurveBtn == hBtnMakeAvgCurveModify)
				{
					if(gwAvgType == 1)	//��ƽ��
					{
						DWORD dwHeadFrequence = 0;
							
							if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 0)
								dwHeadFrequence = 1000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 1)
								dwHeadFrequence = 500000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 2)
								dwHeadFrequence = 2000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 3)
								dwHeadFrequence = 5000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 4)
								dwHeadFrequence = 5000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 5)
								dwHeadFrequence = 2500000;
							
						for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
						{
							float fRange = gSelectedAvgPointData[CurrentChannelNo][3][i].serial * (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) / DATA_SIZE;
							//��ƽ����ƽ�׿׵���ѹ��
							float fDeltaDB = (float)(20 * log10(PI * gfDefectZhi1[CurrentChannelNo] * gfDefectZhi1[CurrentChannelNo] / (2 *((float)DisplaySet_ad[CurrentChannelNo].nWaveSpeed  * 1000 / dwHeadFrequence) * fRange)));
							
							gSelectedAvgPointData[CurrentChannelNo][3][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data, -fDeltaDB);
						}
					}
				}
				else
				{
					;//
				}
				memset(stAvgPoint, 0, sizeof(POINT) * MAX_CURVE_POINT_NUM);
				for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
				{
					stAvgPoint[i] = dataToPoint(gSelectedAvgPointData[CurrentChannelNo][3][i]);
				}
				gWaveDrawAvgFlag = TRUE;

				SetRedrawFlag(TRUE);

				if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
				{
					for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
					{
						gOldMakeAvgPointData[i] = gSelectedAvgPointData[CurrentChannelNo][3][i];
					}
				}
				
				//����ť�ؼ�
				hBtnCreateAvgCurveAddPoint = CreateWindow(_T("BUTTON"),_T("AvgCurveAddPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEAVGCURVE_ADDPOINT,hInst,NULL);

				hBtnCreateAvgCurveDeletePoint = CreateWindow(_T("BUTTON"),_T("AvgCurveDeletePoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEAVGCURVE_DELETEPOINT,hInst,NULL);
				
				hBtnCreateAvgCurveEnd = CreateWindow(_T("BUTTON"),_T("AvgCurveEnd"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEAVGCURVE_END,hInst,NULL);
			
				CreateAvgCurveBtnList[0] = hBtnCreateAvgCurveAddPoint;
				CreateAvgCurveBtnList[1] = hBtnCreateAvgCurveDeletePoint;
				CreateAvgCurveBtnList[2] = hBtnCreateAvgCurveEnd;
				//��ť�ؼ����Զ��崦�����
				OldBtnCreateAvgCurveAddPointProc = (WNDPROC) GetWindowLong(hBtnCreateAvgCurveAddPoint,GWL_WNDPROC);
				SetWindowLong(hBtnCreateAvgCurveAddPoint,GWL_WNDPROC,(LONG)btnCreateAvgCurveAddPointProc);

				OldBtnCreateAvgCurveDeletePointProc = (WNDPROC) GetWindowLong(hBtnCreateAvgCurveDeletePoint,GWL_WNDPROC);
				SetWindowLong(hBtnCreateAvgCurveDeletePoint,GWL_WNDPROC,(LONG)btnCreateAvgCurveDeletePointProc);

				OldBtnCreateAvgCurveEndProc = (WNDPROC) GetWindowLong(hBtnCreateAvgCurveEnd,GWL_WNDPROC);
				SetWindowLong(hBtnCreateAvgCurveEnd,GWL_WNDPROC,(LONG)btnCreateAvgCurveEndProc);
			
				currentCreateAvgCurveBtn = CreateAvgCurveBtnList[0];  //Ĭ�ϵ�ǰѡ�м����������ť
				SetFocus(currentCreateAvgCurveBtn);//ѡ�а�ť������뽹��
				gCurrentFocusBtn = currentCreateAvgCurveBtn;

			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndMakeAvgCurve,SW_SHOW);
				SetFocus(currentMakeAvgCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

				DestroyWindow(hWndCreateAvgCurve);
				hWndCreateAvgCurve = 0;
				
				gCurrentFocusBtn = currentMakeAvgCurveBtn;

				gCurrentWnd = hWndMakeAvgCurve;
				
				//��ԭAVG����״̬
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					gbAvgCurveFlag[i] = gbOldAvgCurveFlag[i];
				}
				//��ԭDAC����״̬				
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
					{
						gDgsSwitch[i][j] = gOldDgsSwitch[i][j];
					}
				}
				//�����ƽ�׿�
				if(gwAvgType == 1)	//��ƽ��
				{
					DWORD dwHeadFrequence = 0;
					
					if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 0)
						dwHeadFrequence = 1000000;
					else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 1)
						dwHeadFrequence = 500000;
					else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 2)
						dwHeadFrequence = 2000000;
					else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 3)
						dwHeadFrequence = 5000000;
					else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 4)
						dwHeadFrequence = 5000000;
					else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 5)
						dwHeadFrequence = 2500000;
					
					for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
					{
						float fRange = gSelectedAvgPointData[CurrentChannelNo][3][i].serial * (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) / DATA_SIZE;
						//ƽ�׿����ƽ�׵���ѹ��
						float fDeltaDB = (float)(20 * log10(PI * gfDefectZhi1[CurrentChannelNo] * gfDefectZhi1[CurrentChannelNo] / (2 *((float)DisplaySet_ad[CurrentChannelNo].nWaveSpeed  * 1000 / dwHeadFrequence) * fRange)));
						
						gSelectedAvgPointData[CurrentChannelNo][3][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data, fDeltaDB);
					}
				}

				gWaveDrawAvgFlag = FALSE;
				SetRedrawFlag(TRUE);
			}
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:			
			pdis=(LPDRAWITEMSTRUCT)lParam;
			createAvgCurve_DrawButtonBitmap(hInst,pdis);

			break;
		case WM_CLOSE:		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*�����½�AVG���ߴ��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int createAvgCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_CreateAvgCurveBtnBitmap[]=
	{
		{IDC_CREATEAVGCURVE_ADDPOINT	,IDB_BITMAP_JIAYANGDIAN_NOSEL_R	 ,  IDB_BITMAP_JIAYANGDIAN_SELECT_R		},
		{IDC_CREATEAVGCURVE_DELETEPOINT	,IDB_BITMAP_SHANYANGDIAN_NOSEL_R	 ,  IDB_BITMAP_SHANYANGDIAN_SELECT_R		},
		{IDC_CREATEAVGCURVE_END	,IDB_BITMAP_JIESHU_NOSEL_R ,  IDB_BITMAP_JIESHU_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_CreateAvgCurveBtnBitmap);
	return ReturnVal;
}
/**************************
*�����㰴ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnCreateAvgCurveAddPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			
			break;
		case WM_PAINT:
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
					while(CreateAvgCurveBtnList[i] != currentCreateAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateAvgCurveBtn = CreateAvgCurveBtnList[i];
					SetFocus(currentCreateAvgCurveBtn);
					gCurrentFocusBtn = currentCreateAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{					
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					while(CreateAvgCurveBtnList[i] != currentCreateAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateAvgCurveBtn = CreateAvgCurveBtnList[i];
					SetFocus(currentCreateAvgCurveBtn);
					gCurrentFocusBtn = currentCreateAvgCurveBtn;
					
					gbDeletePointFlag = TRUE;

					if(gSelectedAvgPointDataNum[CurrentChannelNo])
					{
						gCurrentSelectedAvgPointData = gSelectedAvgPointData[CurrentChannelNo][3][gwDeleteStep];
						
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedAvgPointData.serial/DATA_SIZE, (int)(gCurrentSelectedAvgPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(hBtnCreateAvgCurveDeletePoint, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
				}
				else if(wParam == VK_LEFT)
				{
				}
				else if(wParam == VK_RIGHT)
				{
				}
				else if(wParam == VK_RETURN)
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

					if(peakDefectInfos[CurrentChannelNo][0].wDefectTime!= 0 && peakDefectInfos[CurrentChannelNo][0].wDefectAmp != 0)
					{
						gWaveDrawAvgFlag = FALSE;
						Sleep(100);//�ȴ���������
						
						if(gSelectedAvgPointDataNum[CurrentChannelNo] < MAX_CURVE_POINT_NUM)
							gSelectedAvgPointDataNum[CurrentChannelNo]++;

						if(gSelectedAvgPointDataNum[CurrentChannelNo] == 1)
						{
							gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)peakDefectInfos[CurrentChannelNo][0].wDefectTime)
																											/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
							gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial = curveChangeByDelay(gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial, 
																								fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
							
							gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].data = (float)peakDefectInfos[CurrentChannelNo][0].wDefectAmp;

							gOldMakeAvgPointData[0].data = gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].data;
							gOldMakeAvgPointData[0].serial = gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial;

							gfAvgFirstPointGain[CurrentChannelNo] = ReceiveParam_ad[CurrentChannelNo].fPlus;
						}
						else
						{
							gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)peakDefectInfos[CurrentChannelNo][0].wDefectTime)
																											/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
							gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial = curveChangeByDelay(gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial, 
																								fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
							
							gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].data = (float)(peakDefectInfos[CurrentChannelNo][0].wDefectAmp 
																															* pow(10, (double)((gfAvgFirstPointGain[CurrentChannelNo] - gfAvgFirstPointChangeGain[CurrentChannelNo])- ReceiveParam_ad[CurrentChannelNo].fPlus) / 20));

							gOldMakeAvgPointData[gSelectedAvgPointDataNum[CurrentChannelNo] - 1].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].data, gfAvgFirstPointChangeGain[CurrentChannelNo]);
							gOldMakeAvgPointData[gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial = gSelectedAvgPointData[CurrentChannelNo][3][gSelectedAvgPointDataNum[CurrentChannelNo] - 1].serial;
							sortSelectedPointDataBySerial(gOldMakeAvgPointData,gSelectedAvgPointDataNum[CurrentChannelNo]);
						}
						
						sortSelectedPointDataBySerial(gSelectedAvgPointData[CurrentChannelNo][3],gSelectedAvgPointDataNum[CurrentChannelNo]);

						gWaveDrawAvgFlag = TRUE;

						if(!gbGuanLian)
						{
							gfOldAvgRange[CurrentChannelNo] = DisplaySet_ad[CurrentChannelNo].fRange;
							
							for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
							{	
								gwOldAvgSerial[CurrentChannelNo][i] = (short)gSelectedAvgPointData[CurrentChannelNo][3][i].serial;
							}
						}
						else
						{
							gfOldAvgRange[0] = DisplaySet_ad[CurrentChannelNo].fRange;
							gfOldAvgRange[1] = DisplaySet_ad[CurrentChannelNo].fRange;

							for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
							{	
								gwOldAvgSerial[0][i] = (short)gSelectedAvgPointData[CurrentChannelNo][3][i].serial;
								gwOldAvgSerial[1][i] = (short)gSelectedAvgPointData[CurrentChannelNo][3][i].serial;
							}
						}
					}
				}
			//	else if(wParam == '\t')
			//	{
			//		SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			//	}
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
			/*	else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnCreateAvgCurveAddPointProc,Wnd,Msg,wParam,lParam);
}
/**************************
*ɾ���㰴ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnCreateAvgCurveDeletePointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(CreateAvgCurveBtnList[i] != currentCreateAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateAvgCurveBtn = CreateAvgCurveBtnList[i];
					SetFocus(currentCreateAvgCurveBtn);
					gCurrentFocusBtn = currentCreateAvgCurveBtn;

					gbDeletePointFlag = FALSE;
				/*	InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					//gbEndFlag =TRUE;
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					*/
				}
				else if(wParam == VK_DOWN)
				{
					while(CreateAvgCurveBtnList[i] != currentCreateAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateAvgCurveBtn = CreateAvgCurveBtnList[i];
					SetFocus(currentCreateAvgCurveBtn);
					gCurrentFocusBtn = currentCreateAvgCurveBtn;

					gbDeletePointFlag = FALSE;
				  }
				else if(wParam == VK_LEFT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					if(gSelectedAvgPointDataNum[CurrentChannelNo])
					{
						
						if(gwDeleteStep == 0)
							gwDeleteStep = gSelectedAvgPointDataNum[CurrentChannelNo] - 1;
						else
							gwDeleteStep--;
						gCurrentSelectedAvgPointData = gSelectedAvgPointData[CurrentChannelNo][3][gwDeleteStep];
						
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedAvgPointData.serial/DATA_SIZE, (int)(gCurrentSelectedAvgPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
				}
				else if(wParam == VK_RIGHT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					if(gSelectedAvgPointDataNum[CurrentChannelNo])
					{						
						if(gwDeleteStep == gSelectedAvgPointDataNum[CurrentChannelNo] - 1)
							gwDeleteStep = 0;
						else
							gwDeleteStep++;
						gCurrentSelectedAvgPointData = gSelectedAvgPointData[CurrentChannelNo][3][gwDeleteStep];
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedAvgPointData.serial/DATA_SIZE, (int)(gCurrentSelectedAvgPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
				}
				else if(wParam == VK_RETURN)
				{
					if(gSelectedAvgPointDataNum[CurrentChannelNo])
					{
						gWaveDrawAvgFlag = FALSE;
						Sleep(100);//�ȴ���������
					 	if(gSelectedAvgPointDataNum[CurrentChannelNo] == 1)
					 	{
					 		gSelectedAvgPointDataNum[CurrentChannelNo]--;
					 	}
						else
						{
							if(gwDeleteStep == (gSelectedAvgPointDataNum[CurrentChannelNo] - 1))//ɾ���һ����
							{
								gSelectedAvgPointDataNum[CurrentChannelNo]--;
								gwDeleteStep--;
							}
							else
							{
								for(i = gwDeleteStep; i < gSelectedAvgPointDataNum[CurrentChannelNo] - 1; i++)
								{
									gSelectedAvgPointData[CurrentChannelNo][3][i] = gSelectedAvgPointData[CurrentChannelNo][3][i + 1];//ѡ�е����ĵ�������ǰ��
								}
								gSelectedAvgPointDataNum[CurrentChannelNo]--;
								for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
								{
									gOldMakeAvgPointData[i] = gSelectedAvgPointData[CurrentChannelNo][3][i];
								}
							}
						}
						gWaveDrawAvgFlag = TRUE;
					}
				}
			//	else if(wParam == '\t')
			//	{
			//		SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			//	}
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
				/*else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnCreateAvgCurveDeletePointProc,Wnd,Msg,wParam,lParam);
}

/**************************
*������ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnCreateAvgCurveEndProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					while(CreateAvgCurveBtnList[i] != currentCreateAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateAvgCurveBtn = CreateAvgCurveBtnList[i];
					SetFocus(currentCreateAvgCurveBtn);
					gCurrentFocusBtn = currentCreateAvgCurveBtn;

					gbDeletePointFlag = TRUE;

					if(gSelectedAvgPointDataNum[CurrentChannelNo])
					{
						gCurrentSelectedAvgPointData = gSelectedAvgPointData[CurrentChannelNo][3][gwDeleteStep];
						
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedAvgPointData.serial/DATA_SIZE, (int)(gCurrentSelectedAvgPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(hBtnCreateAvgCurveDeletePoint, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
				}
				
				else if(wParam == VK_DOWN)
				{
					while(CreateAvgCurveBtnList[i] != currentCreateAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateAvgCurveBtn = CreateAvgCurveBtnList[i];
					SetFocus(currentCreateAvgCurveBtn);
					gCurrentFocusBtn = currentCreateAvgCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{					
					ShowWindow(hWndMakeAvgCurve,SW_SHOW);
					SetFocus(currentMakeAvgCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndCreateAvgCurve);
					hWndCreateAvgCurve = 0;
					
					gCurrentFocusBtn = currentMakeAvgCurveBtn;

					gCurrentWnd = hWndMakeAvgCurve;

					gWaveDrawAvgFlag = FALSE;
					SetRedrawFlag(TRUE);

					//���������л�׼����ı䣬�������Լӵ�һ��Ļ�׼����Ϊ��׼�ٱ��ȥ
					/*for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
					{
						gSelectedAvgPointData[CurrentChannelNo][3][i].data = (DWORD)(gSelectedAvgPointData[CurrentChannelNo][3][i].data
																					/ pow(10, (double)(gfAvgFirstPointChangeGain[CurrentChannelNo]) / (-20)));
					}*/
					if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
					{
						for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
						{
							gSelectedAvgPointData[CurrentChannelNo][3][i] = gOldMakeAvgPointData[i];
						}
					}

					ReceiveParam_ad[CurrentChannelNo].fPlus = gfAvgFirstPointGain[CurrentChannelNo];
					SetPlusLableAndExecute(Wnd, gfAvgFirstPointGain[CurrentChannelNo], ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);
					
					if(!gbGuanLian)
					{
						if(gwAvgType == 1)	//��ƽ��
						{
							DWORD dwHeadFrequence = 0;
							
							if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 0)
								dwHeadFrequence = 1000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 1)
								dwHeadFrequence = 500000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 2)
								dwHeadFrequence = 2000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 3)
								dwHeadFrequence = 5000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 4)
								dwHeadFrequence = 5000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 5)
								dwHeadFrequence = 2500000;
							
							for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
							{
								float fRange = gSelectedAvgPointData[CurrentChannelNo][3][i].serial * (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) / DATA_SIZE;
								//ƽ�׿����ƽ�׵���ѹ��
								float fDeltaDB = (float)(20 * log10(PI * gfDefectZhi1[CurrentChannelNo] * gfDefectZhi1[CurrentChannelNo] / (2 *((float)DisplaySet_ad[CurrentChannelNo].nWaveSpeed  * 1000 / dwHeadFrequence) * fRange)));
								
								gSelectedAvgPointData[CurrentChannelNo][3][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data, fDeltaDB);
							}
						}
						float fDb1 = (float)(40 * log10(gfDefectZhi2[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
						float fDb2 = (float)(40 * log10(gfDefectZhi3[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
						for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
						{	
							gSelectedAvgPointData[CurrentChannelNo][0][i].data = gSelectedAvgPointData[CurrentChannelNo][3][i].data;
							gSelectedAvgPointData[CurrentChannelNo][0][i].serial = gSelectedAvgPointData[CurrentChannelNo][3][i].serial;
							
							gSelectedAvgPointData[CurrentChannelNo][1][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data, fDb1);
							gSelectedAvgPointData[CurrentChannelNo][1][i].serial = gSelectedAvgPointData[CurrentChannelNo][3][i].serial;

							gSelectedAvgPointData[CurrentChannelNo][2][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data, fDb2);
							gSelectedAvgPointData[CurrentChannelNo][2][i].serial = gSelectedAvgPointData[CurrentChannelNo][3][i].serial;

							gwOldAvgSerial[CurrentChannelNo][i] = (short)gSelectedAvgPointData[CurrentChannelNo][3][i].serial;
						}
					
						gbAvgCurveFlag[CurrentChannelNo] = ON;
						gfOldAvgRange[CurrentChannelNo] = DisplaySet_ad[CurrentChannelNo].fRange;
						
					}
					else
					{
						if(gwAvgType == 1)	//��ƽ��
						{
							DWORD dwHeadFrequence = 0;
							
							if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 0)
								dwHeadFrequence = 1000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 1)
								dwHeadFrequence = 500000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 2)
								dwHeadFrequence = 2000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 3)
								dwHeadFrequence = 5000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 4)
								dwHeadFrequence = 5000000;
							else if(ReceiveParam_ad[CurrentChannelNo].nProbeFreq == 5)
								dwHeadFrequence = 2500000;
							
							for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
							{
								float fRange = gSelectedAvgPointData[CurrentChannelNo][3][i].serial * (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) / DATA_SIZE;
								float fDeltaDB = (float)(20 * log10(PI * gfDefectZhi1[CurrentChannelNo] * gfDefectZhi1[CurrentChannelNo] / (2 *((float)DisplaySet_ad[CurrentChannelNo].nWaveSpeed  * 1000 / dwHeadFrequence) * fRange)));
								
								gSelectedAvgPointData[CurrentChannelNo][3][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data, fDeltaDB);
							}
						}
						float fDb1 = (float)(40 * log10(gfDefectZhi2[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
						float fDb2 = (float)(40 * log10(gfDefectZhi3[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
							{	
								gSelectedAvgPointData[i][3][j].data = gSelectedAvgPointData[CurrentChannelNo][3][j].data;
								gSelectedAvgPointData[i][3][j].serial = gSelectedAvgPointData[CurrentChannelNo][3][j].serial;
									
								gSelectedAvgPointData[i][0][j].data = gSelectedAvgPointData[CurrentChannelNo][3][j].data;
								gSelectedAvgPointData[i][0][j].serial = gSelectedAvgPointData[CurrentChannelNo][3][j].serial;
									
								gSelectedAvgPointData[i][1][j].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][j].data, fDb1);
								gSelectedAvgPointData[i][1][j].serial = gSelectedAvgPointData[CurrentChannelNo][3][j].serial;

								gSelectedAvgPointData[i][2][j].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][j].data, fDb2);
								gSelectedAvgPointData[i][2][j].serial = gSelectedAvgPointData[CurrentChannelNo][3][j].serial;

								gwOldAvgSerial[i][j] = (short)gSelectedAvgPointData[CurrentChannelNo][3][j].serial;
							}
							gSelectedAvgPointDataNum[i] = gSelectedAvgPointDataNum[CurrentChannelNo];
							gbAvgCurveFlag[i] = ON;
							gfOldAvgRange[i] = DisplaySet_ad[CurrentChannelNo].fRange;
						}
					}
					//��ԭDAC����״̬				
					for(int i = 0; i < CHANNEL_NUM; i++)
					{
						for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
						{
							gDgsSwitch[i][j] = gOldDgsSwitch[i][j];
						}
					}
					SaveParamFile(lpDefaultParamFile);
				}
			//	else if(wParam == '\t')
			//	{
			//		SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			//	}
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
				/*else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnCreateAvgCurveEndProc,Wnd,Msg,wParam,lParam);
}

