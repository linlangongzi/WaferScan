/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * �޸�DAC���ߴ���Դ�ļ�
 * Filename: ModifyDacCurveWin.cpp
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
#include "fnModifyDgsWin.h"
#include "interfaceCmdID.h"
#include "const.h"
#include "resource.h"
#include "craftParamWin.h"
#include "gateWin.h"
#include "recvParamWin.h"

HWND hWndModifyDacCurve = NULL;//�޸�DAC���ߴ��ھ��
HWND hBtnModifyDgsCurveAddPoint = NULL;//�����㰴ť���
HWND hBtnModifyDgsCurveSelPoint = NULL;//ѡ�����㰴ť���
HWND hBtnModifyDgsCurveModifyAmp = NULL;//�޸ķ�ֵ��ť���
HWND hBtnModifyDgsCurveDeletePoint = NULL;//ɾ���㰴ť���
HWND hBtnModifyDgsCurvePingDing = NULL;//�����߰�ť���
HWND hBtnModifyDgsCurveDingLiang = NULL;//�����߰�ť���
HWND hBtnModifyDgsCurvePanHui = NULL;//�з��߰�ť���
HWND hBtnModifyDgsCurveEnd = NULL;//������ť���
HWND ModifyDgsCurveBtnList[6];//�޸�DGS���ߴ��ڵİ�ť��
WNDPROC OldBtnModifyDgsCurveAddPointProc = NULL;//�����㰴ťĬ�ϴ������
WNDPROC OldBtnModifyDgsCurveSelPointProc = NULL;//ѡ�����㰴ťĬ�ϴ������
WNDPROC OldBtnModifyDgsCurveModifyAmpProc = NULL;//�޸ķ�ֵ��ťĬ�ϴ������
WNDPROC OldBtnModifyDgsCurveDeletePointProc = NULL;//ɾ���㰴ťĬ�ϴ������
WNDPROC OldBtnModifyDgsCurvePingDingProc = NULL;//�����߰�ťĬ�ϴ������
WNDPROC OldBtnModifyDgsCurveDingLiangProc = NULL;//�����߰�ťĬ�ϴ������
WNDPROC OldBtnModifyDgsCurvePanHuiProc = NULL;//�з��߰�ťĬ�ϴ������
WNDPROC OldBtnModifyDgsCurveEndProc = NULL;//������ťĬ�ϴ������
HWND currentModifyDgsCurveBtn = NULL;//�޸�DGS���ߴ��е�ǰѡ�еİ�ť
extern POINT gstCurrentDgsPoint;//��ǰѡ�е�DGS���ߵ�����

extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
extern CURVEPOINT_DATA gOldMakeDgsPointData[MAX_CURVE_POINT_NUM];//�Լӵ�һ���׼����Ϊ��׼��������

extern CURVEPOINT_DATA gCurrentSelectedDgsPointData;//��ӵ������е�ǰѡ�е�DGS���ߵ�����
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];//��ѡ�е�DGS������

extern BOOL gWaveDrawDacFlag;		//�����Ƿ�DAC
extern float gfPingDingGain[CHANNEL_NUM];				//��������
extern float gfDingLiangGain[CHANNEL_NUM];				//��������
extern float gfPanHuiGain[CHANNEL_NUM];				//�з�����
extern float gfOldDacRange[CHANNEL_NUM];				//�������DACʱ������
extern short gwOldDacSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������DACʱ��ÿ�����λ��
extern float gfDacFirstPointGain[CHANNEL_NUM];			//DAC��һ������ֵ
extern float gfDacFirstPointChangeGain[CHANNEL_NUM];			//����ʱ������ı������ֵ

extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];

static WORD wModifyAmpStepIndex = 1;		//�޸ķ�ֵ��������
static float fModifyAmpStep = 1;			//�޸ķ�ֵ����

static const int buttonNum = 5;//��ť�ؼ���
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
*ע���޸�DAC���ߴ���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerModifyDgsCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));

	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) ModifyDgsCurveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szModifyDgsCurveChild;

	return RegisterClass(&wc);
}
static void displayCurveGain(HWND hWnd, float fGain)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fGain);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}
static void displayPointAmp(HWND hWnd)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	if(gSelectedDgsPointDataNum[CurrentChannelNo][3])
	{
		float fSelectPointAmp = gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep].data * 100 / MAX_HEIGHT;

		wsprintf(strText, _T("%.1f"), fSelectPointAmp);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}
	else
	{
		swprintf(strText, _T("NULL"));
	}
}

static void displaySelPoint(HWND hWnd)
{	
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	if(gSelectedDgsPointDataNum[CurrentChannelNo][3])
	{
		gCurrentSelectedDgsPointData = gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep];
		gstCurrentDgsPoint = dataToPoint(gCurrentSelectedDgsPointData);
		swprintf(strText, _T("%.0f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDgsPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDgsPointData.data * 100 / MAX_HEIGHT));
	}	
	else
	{
		swprintf(strText, _T("������"));
	}

	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
}

static void quitCurveProcess()
{
	gWaveDrawDacFlag = FALSE;
	SetRedrawFlag(TRUE);

	ReceiveParam_ad[CurrentChannelNo].fPlus = gfDacFirstPointGain[CurrentChannelNo];
	SetPlusLableAndExecute(NULL, gfDacFirstPointGain[CurrentChannelNo], ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);

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

/**************************
*�޸�DGS���ߴ��ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK ModifyDgsCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
				hBtnModifyDgsCurveAddPoint = CreateWindow(_T("BUTTON"),_T("ModifyDgsCurveAddPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYDGSCURVE_ADDPOINT,hInst,NULL);

				hBtnModifyDgsCurveSelPoint = CreateWindow(_T("BUTTON"),_T("ModifyDgsCurveSelPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYDGSCURVE_SELPOINT,hInst,NULL);

				hBtnModifyDgsCurveModifyAmp = CreateWindow(_T("BUTTON"),_T("ModifyDgsCurveModifyAmp"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYDGSCURVE_MODIFYAMP,hInst,NULL);

				hBtnModifyDgsCurveDeletePoint = CreateWindow(_T("BUTTON"),_T("ModifyDgsCurveDeletePoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYDGSCURVE_DELETEPOINT,hInst,NULL);
				
				hBtnModifyDgsCurveEnd = CreateWindow(_T("BUTTON"),_T("ModifyDgsCurveEnd"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_MODIFYDGSCURVE_END,hInst,NULL);
			
				ModifyDgsCurveBtnList[0] = hBtnModifyDgsCurveAddPoint;
				ModifyDgsCurveBtnList[1] = hBtnModifyDgsCurveSelPoint;
				ModifyDgsCurveBtnList[2] = hBtnModifyDgsCurveModifyAmp;
				ModifyDgsCurveBtnList[3] = hBtnModifyDgsCurveDeletePoint;
				ModifyDgsCurveBtnList[4] = hBtnModifyDgsCurveEnd;
				//��ť�ؼ����Զ��崦�����
				OldBtnModifyDgsCurveAddPointProc = (WNDPROC) GetWindowLong(hBtnModifyDgsCurveAddPoint,GWL_WNDPROC);
				SetWindowLong(hBtnModifyDgsCurveAddPoint,GWL_WNDPROC,(LONG)btnModifyDgsCurveAddPointProc);


				OldBtnModifyDgsCurveSelPointProc = (WNDPROC) GetWindowLong(hBtnModifyDgsCurveSelPoint,GWL_WNDPROC);
				SetWindowLong(hBtnModifyDgsCurveSelPoint,GWL_WNDPROC,(LONG)btnModifyDgsCurveSelPointProc);

				OldBtnModifyDgsCurveModifyAmpProc = (WNDPROC) GetWindowLong(hBtnModifyDgsCurveModifyAmp,GWL_WNDPROC);
				SetWindowLong(hBtnModifyDgsCurveModifyAmp,GWL_WNDPROC,(LONG)btnModifyDgsCurveModifyAmpProc);

				OldBtnModifyDgsCurveDeletePointProc = (WNDPROC) GetWindowLong(hBtnModifyDgsCurveDeletePoint,GWL_WNDPROC);
				SetWindowLong(hBtnModifyDgsCurveDeletePoint,GWL_WNDPROC,(LONG)btnModifyDgsCurveDeletePointProc);

				OldBtnModifyDgsCurveEndProc = (WNDPROC) GetWindowLong(hBtnModifyDgsCurveEnd,GWL_WNDPROC);
				SetWindowLong(hBtnModifyDgsCurveEnd,GWL_WNDPROC,(LONG)btnModifyDgsCurveEndProc);
			
				currentModifyDgsCurveBtn = ModifyDgsCurveBtnList[0];  //Ĭ�ϵ�ǰѡ�м����������ť
				SetFocus(currentModifyDgsCurveBtn);//ѡ�а�ť������뽹��
				gCurrentFocusBtn = currentModifyDgsCurveBtn;

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

				DestroyWindow(hWndModifyDacCurve);
				hWndModifyDacCurve = 0;
				
				gCurrentFocusBtn = currentDgsMakeBtn;

				gCurrentWnd = hWndDgsMake;

				gWaveDrawDacFlag = FALSE;

				quitCurveProcess();

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
			ModifyDgsCurve_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnModifyDgsCurveSelPoint)
			{
				displaySelPoint(pdis->hwndItem);
			}
			if(pdis->hwndItem == hBtnModifyDgsCurveModifyAmp)
			{
				displayPointAmp(pdis->hwndItem);
			}
			
			// ��ⰴť�������ʾ��Ӧ�Ĳ���
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentModifyDgsCurveBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnModifyDgsCurveModifyAmp)
						swprintf(gszTextStep, _T("%.1f"), fModifyAmpStep);
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
*�����޸�DGS���ߴ��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int ModifyDgsCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_ModifyDgsCurveBtnBitmap[]=
	{
		{IDC_MODIFYDGSCURVE_ADDPOINT	,IDB_BITMAP_JIAYANGDIAN6_NOSEL_R	 ,  IDB_BITMAP_JIAYANGDIAN6_SELECT_R		},
		{IDC_MODIFYDGSCURVE_SELPOINT	,IDB_BITMAP_XUANZEYANGDIAN_NOSEL_R	 ,  IDB_BITMAP_XUANZEYANGDIAN_SELECT_R		},
		{IDC_MODIFYDGSCURVE_MODIFYAMP	,IDB_BITMAP_FUZHI_NOSEL_R	 ,  		IDB_BITMAP_FUZHI_SELECT_R		},
		{IDC_MODIFYDGSCURVE_DELETEPOINT	,IDB_BITMAP_SHANYANGDIAN6_NOSEL_R	 ,  IDB_BITMAP_SHANYANGDIANG6_SELECT_R		},
		{IDC_MODIFYDGSCURVE_END	,IDB_BITMAP_JIESHU6_NOSEL_R ,  IDB_BITMAP_JIESHU6_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_ModifyDgsCurveBtnBitmap);
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
LRESULT CALLBACK btnModifyDgsCurveAddPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyDgsCurveBtnList[i] != currentModifyDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyDgsCurveBtn = ModifyDgsCurveBtnList[i];
					SetFocus(currentModifyDgsCurveBtn);
					gCurrentFocusBtn = currentModifyDgsCurveBtn;					
				}
				else if(wParam == VK_DOWN)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					while(ModifyDgsCurveBtnList[i] != currentModifyDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyDgsCurveBtn = ModifyDgsCurveBtnList[i];
					SetFocus(currentModifyDgsCurveBtn);
					gCurrentFocusBtn = currentModifyDgsCurveBtn;

					
					gbDeletePointFlag = TRUE;

					displaySelPoint(hBtnModifyDgsCurveSelPoint);							
					displayPointAmp(hBtnModifyDgsCurveModifyAmp);
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
	return CallWindowProc(OldBtnModifyDgsCurveAddPointProc,Wnd,Msg,wParam,lParam);
}

/**************************
*ѡ�����㰴ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnModifyDgsCurveSelPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyDgsCurveBtnList[i] != currentModifyDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyDgsCurveBtn = ModifyDgsCurveBtnList[i];
					SetFocus(currentModifyDgsCurveBtn);
					gCurrentFocusBtn = currentModifyDgsCurveBtn;

					gbDeletePointFlag = FALSE;
				}
				else if(wParam == VK_DOWN)
				{
					while(ModifyDgsCurveBtnList[i] != currentModifyDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyDgsCurveBtn = ModifyDgsCurveBtnList[i];
					SetFocus(currentModifyDgsCurveBtn);
					gCurrentFocusBtn = currentModifyDgsCurveBtn;
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
					}
					displaySelPoint(Wnd);
					displayPointAmp(hBtnModifyDgsCurveModifyAmp);
					
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
					}				
					displaySelPoint(Wnd);
					displayPointAmp(hBtnModifyDgsCurveModifyAmp);
				}
				else if(wParam == VK_RETURN)
				{
					;//
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
	return CallWindowProc(OldBtnModifyDgsCurveSelPointProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�޸ķ�ֵ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnModifyDgsCurveModifyAmpProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyDgsCurveBtnList[i] != currentModifyDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyDgsCurveBtn = ModifyDgsCurveBtnList[i];
					SetFocus(currentModifyDgsCurveBtn);
					gCurrentFocusBtn = currentModifyDgsCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ModifyDgsCurveBtnList[i] != currentModifyDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyDgsCurveBtn = ModifyDgsCurveBtnList[i];
					SetFocus(currentModifyDgsCurveBtn);
					gCurrentFocusBtn = currentModifyDgsCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					gWaveDrawDacFlag = FALSE;
					Sleep(100);//�ȴ���������
					float fSelectPointAmp = gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep].data * 100 / MAX_HEIGHT;
					if(fSelectPointAmp - fModifyAmpStep >= 0)
					{
						fSelectPointAmp = fSelectPointAmp - fModifyAmpStep;
						gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep].data = fSelectPointAmp / 100 * MAX_HEIGHT;
						displayPointAmp(Wnd);
					}
					
					gWaveDrawDacFlag = TRUE;			
				}
				else if(wParam == VK_RIGHT)
				{
					gWaveDrawDacFlag = FALSE;
					Sleep(100);//�ȴ���������
					float fSelectPointAmp = gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep].data * 100 / MAX_HEIGHT;
					if(fSelectPointAmp + fModifyAmpStep <= 100)
					{
						fSelectPointAmp = fSelectPointAmp + fModifyAmpStep;
						gSelectedDgsPointData[CurrentChannelNo][3][gwDeleteStep].data = fSelectPointAmp / 100 * MAX_HEIGHT;
						displayPointAmp(Wnd);
					}
					
					gWaveDrawDacFlag = TRUE;
				}
				else if(wParam == VK_RETURN)
				{
					;//
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
					if(wModifyAmpStepIndex < 2)
						wModifyAmpStepIndex++;
					else
						wModifyAmpStepIndex = 0;

					fModifyAmpStep = RATESTEPS[wModifyAmpStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fModifyAmpStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnModifyDgsCurveSelPointProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnModifyDgsCurveDeletePointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyDgsCurveBtnList[i] != currentModifyDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyDgsCurveBtn = ModifyDgsCurveBtnList[i];
					SetFocus(currentModifyDgsCurveBtn);
					gCurrentFocusBtn = currentModifyDgsCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(ModifyDgsCurveBtnList[i] != currentModifyDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyDgsCurveBtn = ModifyDgsCurveBtnList[i];
					SetFocus(currentModifyDgsCurveBtn);
					gCurrentFocusBtn = currentModifyDgsCurveBtn;

					gbDeletePointFlag = FALSE;
				  }
				else if(wParam == VK_LEFT)
				{
					;//
				}
				else if(wParam == VK_RIGHT)
				{
					;//
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
	return CallWindowProc(OldBtnModifyDgsCurveDeletePointProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnModifyDgsCurveEndProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(ModifyDgsCurveBtnList[i] != currentModifyDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModifyDgsCurveBtn = ModifyDgsCurveBtnList[i];
					SetFocus(currentModifyDgsCurveBtn);
					gCurrentFocusBtn = currentModifyDgsCurveBtn;

					gbDeletePointFlag = TRUE;

					displaySelPoint(hBtnModifyDgsCurveSelPoint);
					displayPointAmp(hBtnModifyDgsCurveModifyAmp);
				}
				
				else if(wParam == VK_DOWN)
				{
					while(ModifyDgsCurveBtnList[i] != currentModifyDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModifyDgsCurveBtn = ModifyDgsCurveBtnList[i];
					SetFocus(currentModifyDgsCurveBtn);
					gCurrentFocusBtn = currentModifyDgsCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{	
					ShowWindow(hWndDgsMake,SW_SHOW);
					SetFocus(currentDgsMakeBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndModifyDacCurve);
					hWndModifyDacCurve = 0;
					
					gCurrentFocusBtn = currentDgsMakeBtn;

					gCurrentWnd = hWndDgsMake;
					
					quitCurveProcess();					
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
	return CallWindowProc(OldBtnModifyDgsCurveEndProc,Wnd,Msg,wParam,lParam);
}

