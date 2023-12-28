/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * �½�DAC���ߴ���Դ�ļ�
 * Filename: CreateDacCurveWin.cpp
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
#include "fnCreateDgsWin.h"
#include "interfaceCmdID.h"
#include "const.h"
#include "resource.h"
#include "craftParamWin.h"
#include "gateWin.h"
#include "recvParamWin.h"

HWND hWndCreateDacCurve = NULL;//�½�DAC���ߴ��ھ��
HWND hBtnCreateDgsCurveAddPoint = NULL;//�����㰴ť���
HWND hBtnCreateDgsCurveDeletePoint = NULL;//ɾ���㰴ť���
HWND hBtnCreateDgsCurvePingDing = NULL;//�����߰�ť���
HWND hBtnCreateDgsCurveDingLiang = NULL;//�����߰�ť���
HWND hBtnCreateDgsCurvePanHui = NULL;//�з��߰�ť���
HWND hBtnCreateDgsCurveEnd = NULL;//������ť���
HWND CreateDgsCurveBtnList[6];//�½�DGS���ߴ��ڵİ�ť��
WNDPROC OldBtnCreateDgsCurveAddPointProc = NULL;//�����㰴ťĬ�ϴ������
WNDPROC OldBtnCreateDgsCurveDeletePointProc = NULL;//ɾ���㰴ťĬ�ϴ������
WNDPROC OldBtnCreateDgsCurvePingDingProc = NULL;//�����߰�ťĬ�ϴ������
WNDPROC OldBtnCreateDgsCurveDingLiangProc = NULL;//�����߰�ťĬ�ϴ������
WNDPROC OldBtnCreateDgsCurvePanHuiProc = NULL;//�з��߰�ťĬ�ϴ������
WNDPROC OldBtnCreateDgsCurveEndProc = NULL;//������ťĬ�ϴ������
HWND currentCreateDgsCurveBtn = NULL;//�½�DGS���ߴ��е�ǰѡ�еİ�ť
POINT gstCurrentDgsPoint;//��ǰѡ�е�DGS���ߵ�����

CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
CURVEPOINT_DATA gOldMakeDgsPointData[MAX_CURVE_POINT_NUM] = {0};//�Լӵ�һ���׼����Ϊ��׼��������

CURVEPOINT_DATA gCurrentSelectedDgsPointData;//��ӵ������е�ǰѡ�е�DGS���ߵ�����
WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM] = {{0,0,0,0},{0,0,0,0}};//��ѡ�е�DGS������

BOOL gWaveDrawDacFlag = FALSE;		//�����Ƿ�DAC
float gfPingDingGain[CHANNEL_NUM] = {-3, -3};				//��������
float gfDingLiangGain[CHANNEL_NUM] = {0, 0};				//��������
float gfPanHuiGain[CHANNEL_NUM] = {3, 3};				//�з�����

float gfOldDacRange[CHANNEL_NUM] = {0, 0};				//�������DACʱ������
short gwOldDacSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM] = {{0}, {0}};	//�������DACʱ��ÿ�����λ��
float gfDacFirstPointGain[CHANNEL_NUM] = {0, 0};			//DAC��һ������ֵ
float gfDacFirstPointChangeGain[CHANNEL_NUM] = {0, 0};			//����ʱ������ı������ֵ

extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];

static WORD wCurveGainStepIndex = 1;		//�����沽������
static float fCurveGainStep = 1;			//�����沽��

static const int buttonNum = 3;//��ť�ؼ���
static BOOL gOldDgsSwitch[CHANNEL_NUM][GATE_NUM] = {{0,0},{0,0}};				//����DGS����״̬
static BOOL bOldAvgCurveFlag[CHANNEL_NUM] = {0,0};				//����AVG����״̬

static float fOldGain = 0;		//�ս����½����޸�����ʱ�Ļ�׼����ֵ
extern HWND hWndDgsMake;//�������ߴ���
extern HWND currentDgsMakeBtn;//�������ߴ����е�ǰѡ�еİ�ť
extern HINSTANCE hInst;//����ʵ�����
extern HWND hWndMain;//�����ھ��
extern HWND hStatusBar;

extern WORD CurrentChannelNo;

extern float fRangeEnd[2];
extern float fRangeStart[2];

extern HWND gCurrentFocusBtn;
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];		//DGSͨ������
extern wchar_t gszTextStep[10];
extern BOOL gbGuanLian;
extern BOOL bFreezeUp;
extern wchar_t gszTextPeak[20];
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD gwDeleteStep;					//ɾ����λ��
extern PEAK_INFO gPeakDefectInfos[2][2];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern BOOL gbDeletePointFlag;		//�Ƿ���ɾ�������
extern HWND hBtnDgsMakeCreate;//������ť���
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG���߿��ر�־
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//AVG��ӵ��������ݶ���
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//��ѡ�е�AVG������
extern BOOL gbHotDB;			// ��ݼ�����
extern BOOL gbHotRange;			// ��ݼ�����


/**************************
*ע���½�DAC���ߴ���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerCreateDgsCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));

	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) createDgsCurveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szCreateDacCurveChild;

	return RegisterClass(&wc);
}
static void displayCurveGain(HWND hWnd, float fGain)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fGain);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}

/**************************
*�½�DGS���ߴ��ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK createDgsCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
				fOldGain = ReceiveParam_ad[CurrentChannelNo].fPlus;
				memset(gOldMakeDgsPointData, 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
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
						gDgsSwitch[i][j] = FALSE;
					}
				}
				//����AVG����״̬					
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
						bOldAvgCurveFlag[i] = gbAvgCurveFlag[i];
				}
				//�ر�AVG ����������
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					gbAvgCurveFlag[i] = FALSE;
				}
				
				gwDeleteStep = 0;
				gbDeletePointFlag = FALSE;
				float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
				ReceiveParam_ad[CurrentChannelNo].fRefPlus = 0;
				SetRefPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);
				refGainChangeCurve(fOldRefPlus, FALSE);//wangbingfu add at 2011-03-09
				
				gfDacFirstPointChangeGain[0] = 0;
				gfDacFirstPointChangeGain[1] = 0;
				if(currentDgsMakeBtn == hBtnDgsMakeCreate)
				{
					gSelectedDgsPointDataNum[CurrentChannelNo][0] = 0;
					gSelectedDgsPointDataNum[CurrentChannelNo][1] = 0;
					gSelectedDgsPointDataNum[CurrentChannelNo][2] = 0;
					gSelectedDgsPointDataNum[CurrentChannelNo][3] = 0;

					memset(gSelectedDgsPointData[CurrentChannelNo][0], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
					memset(gSelectedDgsPointData[CurrentChannelNo][1], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
					memset(gSelectedDgsPointData[CurrentChannelNo][2], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
					memset(gSelectedDgsPointData[CurrentChannelNo][3], 0, sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);

					memset(&gCurrentSelectedDgsPointData, 0, sizeof(CURVEPOINT_DATA));

					memset(gwOldDacSerial[CurrentChannelNo], 0, sizeof(WORD) * MAX_CURVE_POINT_NUM);
				}
				
				gWaveDrawDacFlag = TRUE;
				SetRedrawFlag(TRUE);

				if(gSelectedDgsPointDataNum[CurrentChannelNo][0] > 1)
				{
					for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][0]; i++)
					{
						gOldMakeDgsPointData[i] = gSelectedDgsPointData[CurrentChannelNo][3][i];
					}
				}
				
				//����ť�ؼ�
				hBtnCreateDgsCurveAddPoint = CreateWindow(_T("BUTTON"),_T("DgsCurveAddPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_ADDPOINT,hInst,NULL);

				hBtnCreateDgsCurveDeletePoint = CreateWindow(_T("BUTTON"),_T("DgsCurveDeletePoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_DELETEPOINT,hInst,NULL);
				
		/*		hBtnCreateDgsCurvePingDing = CreateWindow(_T("BUTTON"),_T("pingDIng"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_PINGDING,hInst,NULL);

				
				hBtnCreateDgsCurveDingLiang = CreateWindow(_T("BUTTON"),_T("dingLiang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_DINGLIANG,hInst,NULL);

				hBtnCreateDgsCurvePanHui = CreateWindow(_T("BUTTON"),_T("panHui"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_PANHUI,hInst,NULL);
		*/
				hBtnCreateDgsCurveEnd = CreateWindow(_T("BUTTON"),_T("DgsCurveEnd"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATEDGSCURVE_END,hInst,NULL);
			
				CreateDgsCurveBtnList[0] = hBtnCreateDgsCurveAddPoint;
				CreateDgsCurveBtnList[1] = hBtnCreateDgsCurveDeletePoint;
			//	CreateDgsCurveBtnList[2] = hBtnCreateDgsCurvePingDing;
			//	CreateDgsCurveBtnList[3] = hBtnCreateDgsCurveDingLiang;
			//	CreateDgsCurveBtnList[4] = hBtnCreateDgsCurvePanHui;
				CreateDgsCurveBtnList[2] = hBtnCreateDgsCurveEnd;
				//��ť�ؼ����Զ��崦�����
				OldBtnCreateDgsCurveAddPointProc = (WNDPROC) GetWindowLong(hBtnCreateDgsCurveAddPoint,GWL_WNDPROC);
				SetWindowLong(hBtnCreateDgsCurveAddPoint,GWL_WNDPROC,(LONG)btnCreateDgsCurveAddPointProc);

				OldBtnCreateDgsCurveDeletePointProc = (WNDPROC) GetWindowLong(hBtnCreateDgsCurveDeletePoint,GWL_WNDPROC);
				SetWindowLong(hBtnCreateDgsCurveDeletePoint,GWL_WNDPROC,(LONG)btnCreateDgsCurveDeletePointProc);

				OldBtnCreateDgsCurvePingDingProc = (WNDPROC) GetWindowLong(hBtnCreateDgsCurvePingDing,GWL_WNDPROC);
				SetWindowLong(hBtnCreateDgsCurvePingDing,GWL_WNDPROC,(LONG)btnCreateDgsCurvePingDingProc);

				OldBtnCreateDgsCurveDingLiangProc = (WNDPROC) GetWindowLong(hBtnCreateDgsCurveDingLiang,GWL_WNDPROC);
				SetWindowLong(hBtnCreateDgsCurveDingLiang,GWL_WNDPROC,(LONG)btnCreateDgsCurveDingLiangProc);

				OldBtnCreateDgsCurvePanHuiProc = (WNDPROC) GetWindowLong(hBtnCreateDgsCurvePanHui,GWL_WNDPROC);
				SetWindowLong(hBtnCreateDgsCurvePanHui,GWL_WNDPROC,(LONG)btnCreateDgsCurvePanHuiProc);

				OldBtnCreateDgsCurveEndProc = (WNDPROC) GetWindowLong(hBtnCreateDgsCurveEnd,GWL_WNDPROC);
				SetWindowLong(hBtnCreateDgsCurveEnd,GWL_WNDPROC,(LONG)btnCreateDgsCurveEndProc);
			
				currentCreateDgsCurveBtn = CreateDgsCurveBtnList[0];  //Ĭ�ϵ�ǰѡ�м����������ť
				SetFocus(currentCreateDgsCurveBtn);//ѡ�а�ť������뽹��
				gCurrentFocusBtn = currentCreateDgsCurveBtn;

			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndDgsMake,SW_SHOW);
				SetFocus(currentDgsMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

				DestroyWindow(hWndCreateDacCurve);
				hWndCreateDacCurve = 0;
				
				gCurrentFocusBtn = currentDgsMakeBtn;

				gCurrentWnd = hWndDgsMake;

				gWaveDrawDacFlag = FALSE;

				//��ԭDAC����״̬				
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
					{
						gDgsSwitch[i][j] = gOldDgsSwitch[i][j];
					}
				}
				//��ԭAVG����״̬
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
						gbAvgCurveFlag[i] = bOldAvgCurveFlag[i];
				}
			}
			
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:			
			pdis=(LPDRAWITEMSTRUCT)lParam;
			createDgsCurve_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnCreateDgsCurvePingDing)
			{
				displayCurveGain(pdis->hwndItem, gfPingDingGain[CurrentChannelNo]);
			}
			if(pdis->hwndItem == hBtnCreateDgsCurveDingLiang)
			{
				displayCurveGain(pdis->hwndItem, gfDingLiangGain[CurrentChannelNo]);
			}
			if(pdis->hwndItem == hBtnCreateDgsCurvePanHui)
			{
				displayCurveGain(pdis->hwndItem, gfPanHuiGain[CurrentChannelNo]);
			}

			// ��ⰴť�������ʾ��Ӧ�Ĳ���
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentCreateDgsCurveBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnCreateDgsCurvePingDing || pdis->hwndItem == hBtnCreateDgsCurveDingLiang || pdis->hwndItem == hBtnCreateDgsCurvePanHui)
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
*�����½�DGS���ߴ��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int createDgsCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_CreateDgsCurveBtnBitmap[]=
	{
		{IDC_CREATEDGSCURVE_ADDPOINT	,IDB_BITMAP_JIAYANGDIAN_NOSEL_R	 ,  IDB_BITMAP_JIAYANGDIAN_SELECT_R		},
		{IDC_CREATEDGSCURVE_DELETEPOINT	,IDB_BITMAP_SHANYANGDIAN_NOSEL_R	 ,  IDB_BITMAP_SHANYANGDIAN_SELECT_R		},
	//	{IDC_CREATEDGSCURVE_PINGDING	,IDB_BITMAP_PINGDINGXIAN_NOSEL_R ,  IDB_BITMAP_PINGDINGXIAN_SELECT_R	},
	//	{IDC_CREATEDGSCURVE_DINGLIANG	,IDB_BITMAP_DINGLIANGXIAN_NOSEL_R ,  IDB_BITMAP_DINGLIANGXIAN_SELECT_R	},
	//	{IDC_CREATEDGSCURVE_PANHUI	,IDB_BITMAP_PANFEIXIAN_NOSEL_R ,  IDB_BITMAP_PANFEIXIAN_SELECT_R	},
		{IDC_CREATEDGSCURVE_END	,IDB_BITMAP_JIESHU_NOSEL_R ,  IDB_BITMAP_JIESHU_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_CreateDgsCurveBtnBitmap);
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
LRESULT CALLBACK btnCreateDgsCurveAddPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;					
				}
				else if(wParam == VK_DOWN)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;

					
					gbDeletePointFlag = TRUE;

					if(gSelectedDgsPointDataNum[CurrentChannelNo][3])
					{
						gCurrentSelectedDgsPointData = gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep];
						gstCurrentDgsPoint = dataToPoint(gCurrentSelectedDgsPointData);
						swprintf(strText, _T("%.1f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDgsPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDgsPointData.data * 100 / MAX_HEIGHT));
					}	
					else
					{
						swprintf(strText, _T("������"));
					}

					DrawLableText(hBtnCreateDgsCurveDeletePoint, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);					
				}
				else if(wParam == VK_LEFT)
				{
				}
				else if(wParam == VK_RIGHT)
				{
				}
				else if(wParam == VK_RETURN)
				{
					gWaveDrawDacFlag = FALSE;
					Sleep(100);//�ȴ���������
					if(gSelectedDgsPointDataNum[CurrentChannelNo][3] < MAX_CURVE_POINT_NUM)
						gSelectedDgsPointDataNum[CurrentChannelNo][3]++;

					if(gSelectedDgsPointDataNum[CurrentChannelNo][3] == 1)
					{
						gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)gPeakDefectInfos[CurrentChannelNo][0].wDefectTime)
																										/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
						gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial = curveChangeByDelay(gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial, 
																								fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
						
						gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].data = (float)gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp;

						gOldMakeDgsPointData[0].data = gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].data;
						gOldMakeDgsPointData[0].serial = gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial;

						gfDacFirstPointGain[CurrentChannelNo] = ReceiveParam_ad[CurrentChannelNo].fPlus;
					}
					else
					{
						gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)gPeakDefectInfos[CurrentChannelNo][0].wDefectTime)
																										/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
						gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial = curveChangeByDelay(gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial, 
																								fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
						
						gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].data = (float)(gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp 
																														* pow(10, (double)((gfDacFirstPointGain[CurrentChannelNo] - gfDacFirstPointChangeGain[CurrentChannelNo]) - ReceiveParam_ad[CurrentChannelNo].fPlus) / 20));

						gOldMakeDgsPointData[gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].data , gfDacFirstPointChangeGain[CurrentChannelNo]);
						gOldMakeDgsPointData[gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial = gSelectedDgsPointData[CurrentChannelNo][3][gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1].serial;
						sortSelectedPointDataBySerial(gOldMakeDgsPointData,gSelectedDgsPointDataNum[CurrentChannelNo][3]);
					}
					
					sortSelectedPointDataBySerial(gSelectedDgsPointData[CurrentChannelNo][3],gSelectedDgsPointDataNum[CurrentChannelNo][3]);

					gWaveDrawDacFlag = TRUE;

					if(!gbGuanLian)
					{
						gfOldDacRange[CurrentChannelNo] = DisplaySet_ad[CurrentChannelNo].fRange;
						
						for(int j = 0; j < gSelectedDgsPointDataNum[CurrentChannelNo][3]; j++)
						{
							gwOldDacSerial[CurrentChannelNo][j] = (short)gSelectedDgsPointData[CurrentChannelNo][3][j].serial;
						}
					}
					else
					{
						gfOldDacRange[0] = DisplaySet_ad[CurrentChannelNo].fRange;
						gfOldDacRange[1] = DisplaySet_ad[CurrentChannelNo].fRange;

						for(int j = 0; j < gSelectedDgsPointDataNum[CurrentChannelNo][3]; j++)
						{
							gwOldDacSerial[0][j] = (short)gSelectedDgsPointData[CurrentChannelNo][3][j].serial;
							gwOldDacSerial[1][j] = (short)gSelectedDgsPointData[CurrentChannelNo][3][j].serial;
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
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnCreateDgsCurveAddPointProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnCreateDgsCurveDeletePointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
					
					gbDeletePointFlag = FALSE;
				}
				else if(wParam == VK_DOWN)
				{
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;

					gbDeletePointFlag = FALSE;
				  }
				else if(wParam == VK_LEFT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					if(gSelectedDgsPointDataNum[CurrentChannelNo][3])
					{
						
						if(gwDeleteStep == 0)
							gwDeleteStep = gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1;
						else
							gwDeleteStep--;
						gCurrentSelectedDgsPointData = gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep];
						
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDgsPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDgsPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else if(wParam == VK_RIGHT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					if(gSelectedDgsPointDataNum[CurrentChannelNo][3])
					{
						
						if(gwDeleteStep == gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1)
							gwDeleteStep = 0;
						else
							gwDeleteStep++;
						gCurrentSelectedDgsPointData = gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep];
						
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDgsPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDgsPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else if(wParam == VK_RETURN)
				{
					if(gSelectedDgsPointDataNum[CurrentChannelNo][3])
					{
						gWaveDrawDacFlag = FALSE;
						Sleep(100);//�ȴ���������
					 	if(gSelectedDgsPointDataNum[CurrentChannelNo][3] == 1)
					 	{
					 		gSelectedDgsPointDataNum[CurrentChannelNo][3]--;
					 	}
						else
						{
							if(gwDeleteStep == (gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1))//ɾ���һ����
							{
								gSelectedDgsPointDataNum[CurrentChannelNo][3]--;
								gwDeleteStep--;
							}
							else
							{
								for(i = gwDeleteStep; i < gSelectedDgsPointDataNum[CurrentChannelNo][3] - 1; i++)
								{
									gSelectedDgsPointData[CurrentChannelNo][3][i] = gSelectedDgsPointData[CurrentChannelNo][3][i + 1];//ѡ�е����ĵ�������ǰ��
								}
								gSelectedDgsPointDataNum[CurrentChannelNo][3]--;
								for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
								{
									gOldMakeDgsPointData[i] = gSelectedDgsPointData[CurrentChannelNo][3][i];
								}
							}
						}
						gWaveDrawDacFlag = TRUE;
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
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnCreateDgsCurveDeletePointProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnCreateDgsCurvePingDingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;

					gbDeletePointFlag = TRUE;

					if(gSelectedDgsPointDataNum[CurrentChannelNo][3])
					{
						gCurrentSelectedDgsPointData = gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep];
						
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDgsPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDgsPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(hBtnCreateDgsCurveDeletePoint, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				
				else if(wParam == VK_DOWN)
				{
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
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
	return CallWindowProc(OldBtnCreateDgsCurvePingDingProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnCreateDgsCurveDingLiangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
				}
				
				else if(wParam == VK_DOWN)
				{
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
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
	return CallWindowProc(OldBtnCreateDgsCurveDingLiangProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnCreateDgsCurvePanHuiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
				}
				
				else if(wParam == VK_DOWN)
				{
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
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
	return CallWindowProc(OldBtnCreateDgsCurvePanHuiProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnCreateDgsCurveEndProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
				}
				
				else if(wParam == VK_DOWN)
				{
					while(CreateDgsCurveBtnList[i] != currentCreateDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateDgsCurveBtn = CreateDgsCurveBtnList[i];
					SetFocus(currentCreateDgsCurveBtn);
					gCurrentFocusBtn = currentCreateDgsCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{	
					ShowWindow(hWndDgsMake,SW_SHOW);
					SetFocus(currentDgsMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndCreateDacCurve);
					hWndCreateDacCurve = 0;
					
					gCurrentFocusBtn = currentDgsMakeBtn;

					gCurrentWnd = hWndDgsMake;

					gWaveDrawDacFlag = FALSE;
					SetRedrawFlag(TRUE);
					
					ReceiveParam_ad[CurrentChannelNo].fPlus = gfDacFirstPointGain[CurrentChannelNo];
					SetPlusLableAndExecute(Wnd, gfDacFirstPointGain[CurrentChannelNo], ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);

					//���������л�׼����ı䣬�������Լӵ�һ��Ļ�׼����Ϊ��׼�ٱ��ȥ
					/*for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
					{
						gSelectedDgsPointData[CurrentChannelNo][3][i].data = (DWORD)(gSelectedDgsPointData[CurrentChannelNo][3][i].data
																					/ pow(10, (double)(gfDacFirstPointChangeGain[CurrentChannelNo]) / (-20)));
					}*/
					if(gSelectedDgsPointDataNum[CurrentChannelNo][3] > 1)
					{
						for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
						{
							gSelectedDgsPointData[CurrentChannelNo][3][i] = gOldMakeDgsPointData[i];
						}
					}
					
					if(!gbGuanLian)
					{
						for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
						{	
							gSelectedDgsPointData[CurrentChannelNo][0][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, gfPingDingGain[CurrentChannelNo]);
							gSelectedDgsPointData[CurrentChannelNo][0][i].serial = gSelectedDgsPointData[CurrentChannelNo][3][i].serial;
							
							gSelectedDgsPointData[CurrentChannelNo][1][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, gfDingLiangGain[CurrentChannelNo]);
							gSelectedDgsPointData[CurrentChannelNo][1][i].serial = gSelectedDgsPointData[CurrentChannelNo][3][i].serial;
							
							gSelectedDgsPointData[CurrentChannelNo][2][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, gfPanHuiGain[CurrentChannelNo]);
							gSelectedDgsPointData[CurrentChannelNo][2][i].serial = gSelectedDgsPointData[CurrentChannelNo][3][i].serial;							
						}

						gSelectedDgsPointDataNum[CurrentChannelNo][0] = gSelectedDgsPointDataNum[CurrentChannelNo][3];
						gSelectedDgsPointDataNum[CurrentChannelNo][1] = gSelectedDgsPointDataNum[CurrentChannelNo][3];
						gSelectedDgsPointDataNum[CurrentChannelNo][2] = gSelectedDgsPointDataNum[CurrentChannelNo][3];

						for(int i = 0; i < 3; i++)
						{
							gDgsSwitch[CurrentChannelNo][i] = TRUE;
						}
						gfOldDacRange[CurrentChannelNo] = DisplaySet_ad[CurrentChannelNo].fRange;

						for(int j = 0; j < gSelectedDgsPointDataNum[CurrentChannelNo][3]; j++)
						{
							gwOldDacSerial[CurrentChannelNo][j] = (short)gSelectedDgsPointData[CurrentChannelNo][3][j].serial;
						}
					}
					else
					{
						for(int k = 0; k < CHANNEL_NUM; k++)
						{
							for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
							{	
								gSelectedDgsPointData[k][3][i].data = gSelectedDgsPointData[CurrentChannelNo][3][i].data;
								gSelectedDgsPointData[k][3][i].serial = gSelectedDgsPointData[CurrentChannelNo][3][i].serial;
								
								gSelectedDgsPointData[k][0][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, gfPingDingGain[CurrentChannelNo]);
								gSelectedDgsPointData[k][0][i].serial = gSelectedDgsPointData[k][3][i].serial;

								gSelectedDgsPointData[k][1][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, gfDingLiangGain[CurrentChannelNo]);
								gSelectedDgsPointData[k][1][i].serial = gSelectedDgsPointData[CurrentChannelNo][3][i].serial;
								
								gSelectedDgsPointData[k][2][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, gfPanHuiGain[CurrentChannelNo]);
								gSelectedDgsPointData[k][2][i].serial = gSelectedDgsPointData[CurrentChannelNo][3][i].serial;

							}

							gSelectedDgsPointDataNum[k][0] = gSelectedDgsPointDataNum[CurrentChannelNo][3];
							gSelectedDgsPointDataNum[k][1] = gSelectedDgsPointDataNum[CurrentChannelNo][3];
							gSelectedDgsPointDataNum[k][2] = gSelectedDgsPointDataNum[CurrentChannelNo][3];
							gSelectedDgsPointDataNum[k][3] = gSelectedDgsPointDataNum[CurrentChannelNo][3];

							for(int i = 0; i < 3; i++)
							{
								gDgsSwitch[k][i] = TRUE;
							}
							gfOldDacRange[k] = DisplaySet_ad[CurrentChannelNo].fRange;

							for(int j = 0; j < gSelectedDgsPointDataNum[CurrentChannelNo][3]; j++)
							{
								gwOldDacSerial[k][j] = (short)gSelectedDgsPointData[CurrentChannelNo][3][j].serial;
							}
						}
					}
					//��ԭAVG����״̬
					for(int i = 0; i < CHANNEL_NUM; i++)
					{
							gbAvgCurveFlag[i] = bOldAvgCurveFlag[i];
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
	return CallWindowProc(OldBtnCreateDgsCurveEndProc,Wnd,Msg,wParam,lParam);
}
