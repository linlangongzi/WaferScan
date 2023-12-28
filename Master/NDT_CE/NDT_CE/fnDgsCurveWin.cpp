/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * DGS���ߴ���Դ�ļ�
 * Filename: fnDgsCurveWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnDgsCurveWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "fnDgsMakeWin.h"
#include "craftParamWin.h"

HWND hWndDgsCurve = NULL;					//DGS���ߴ��ھ��
HWND hBtnDgsCurveMake = NULL;				//������ť���
HWND hBtnDgsCurveOnOrOff = NULL;			//�����رհ�ť���
HWND hBtnDgsCurveQueXianDaXiao = NULL;		//ȱ�ݴ�С��ť���
HWND hBtnDgsCurveShengChengWeiZhi = NULL;	//����λ�ð�ť���
HWND hBtnDgsCurveShenDuWeiZhi = NULL;		//���λ�ð�ť���
HWND hBtnDgsCurveshuiPingWeiZhi = NULL;	// ˮƽλ�ð�ť���

static HWND dgsCurveBtnList[2] = {NULL,NULL};							//DGS���ߴ��ڵİ�ť��
static WNDPROC OldBtnDgsCurveMakeProc = NULL;				//������ťĬ�ϴ������
static WNDPROC OldBtnDgsCurveOnOrOffProc = NULL;			//����\�رհ�ťĬ�ϴ������
static WNDPROC OldBtnDgsCurveQueXianDaXiaoProc = NULL;		//ȱ�ݴ�С��ťĬ�ϴ������
static WNDPROC OldBtnDgsCurveShengChengWeiZhiProc = NULL;	//����λ�ð�ťĬ�ϴ������
static WNDPROC OldBtnDgsCurveShenDuWeiZhiProc = NULL;		//���λ�ð�ťĬ�ϴ������
static WNDPROC OldBtnDgsCurveshuiPingWeiZhiProc = NULL;	//ˮƽλ�ð�ťĬ�ϴ������

 
HWND currentDgsCurveBtn = NULL;					//DGS���ߴ��е�ǰѡ�еİ�ť
BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM] = {{FALSE,FALSE,FALSE},{FALSE,FALSE,FALSE}};				//DGSͨ������


static const int buttonNum = 2;		//��ť�ؼ���
static BOOL bOldGateFlag = FALSE;		//����բ�ſ����ر�״̬
extern HINSTANCE hInst;		//����ʵ�����
extern HWND hWndCurve;	//�����������ھ��

extern HWND hWndMain;

extern HWND currentCurveBtn;
extern HWND hWndDgsMake;
extern HWND gCurrentFocusBtn;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern SEND_PARAM_DATA SendParam_ad[2];
extern WORD CurrentChannelNo;
extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];//��ѡ�е�DGS������
extern float fRangeEnd[2];
extern float fRangeStart[2];
extern BOOL gbGuanLian;
extern BOOL bFreezeUp;
extern HWND hWndDgsCurve;					//DGS���ߴ��ھ��
extern HWND gCurrentWnd;					//��ǰ��ʾ���Ӵ���
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern CRITICAL_SECTION csPeakDefectInfos;
extern PEAK_INFO gPeakDefectInfos[2][2];
extern GATE_INFO GateInfos[2][3];
extern GATE_INFO stOldGateInfos[2][3];

extern CRITICAL_SECTION csSysInfo;
extern SYS_INFO gSysInfo;

extern float gfOldDacRange[CHANNEL_NUM];				//�������DACʱ������
extern short gwOldDacSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������DACʱ��ÿ�����λ��
extern HWND hStatusBar;					// ״̬�����


extern HWND hWndTlAdjust;	//��У���ھ��
extern HWND currentTlAdjustBtn;	//��У���ڵ�ǰѡ�а�ť
extern WORD gwDeviceType;

extern void CALLBACK defectDisplayProc( HWND hwnd, 
							  UINT uMsg, 
							  UINT idEvent, 
							  DWORD dwTime 
							);
extern void SetOpenFlag(HWND hWnd, const BOOL& bOpen, const WORD& nGateIndex);

extern wchar_t gszTextPeak[20];
//extern void setRailRoadParam(WORD wChannelNo, BOOL bCurveChangeFlag); 		//��·wangbingfu 2010-2-24 //wangbingfu delete at 2011-5-27

/**************************
*ע��DGS���ߴ���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerDgsCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) dgsCurveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szDgsCurveChild;

	return RegisterClass(&wc);
}
/**************************
*��ʾȱ��
* @param		gateDefectAmp[in]:�����˷�
* @param		gateDefectTime[in]:������ʱ
* @return   LRESULT :
* @since    1.00
***************************/
void displayDacFault(double gateDefectAmp, double gateDefectTime)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	float shengChengWeiZhi = 0;			//����λ��
	float shenDuWeiZhi = 0;					//���λ��
	float shuiPingWeiZhi = 0;				//ˮƽλ��
//	float shangFu = 0;						//�˷�
	float queXianDaXiao = 0;				//ȱ�ݴ�С
	CURVEPOINT_DATA tempPointData1, tempPointData2;
	float range1; 
	float range2;
	float currentPointData;			//��ǰ��ʱ�����ߵ��ֵ
	double fAngle = DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI;

	float firstSoundDistance = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gSelectedDgsPointData[CurrentChannelNo][1][0].serial/(float)DATA_SIZE;
		
	float lastSoundDistance = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gSelectedDgsPointData[CurrentChannelNo][1][gSelectedDgsPointDataNum[CurrentChannelNo][1] - 1].serial/(float)DATA_SIZE;	

	if(gSelectedDgsPointDataNum[CurrentChannelNo][1] > 1)
	{
	//	shangFu = float(gateDefectAmp)/MAX_HEIGHT*100.0f;
		if(hWndDgsCurve)
		{
			shengChengWeiZhi = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)gateDefectTime);
			swprintf(strText, _T("%.1f"), shengChengWeiZhi);
			DrawLableText(hBtnDgsCurveShengChengWeiZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
							
			memset(strText, 0, sizeof(strText));
			shenDuWeiZhi = (float)(shengChengWeiZhi * cos(fAngle));
			swprintf(strText, _T("%.1f"), shenDuWeiZhi);
			DrawLableText(hBtnDgsCurveShenDuWeiZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
							
			memset(strText, 0, sizeof(strText));
			shuiPingWeiZhi = (float)(shengChengWeiZhi * sin(fAngle) - SendParam_ad[CurrentChannelNo].fQianYan);
			swprintf(strText, _T("%.1f"), shuiPingWeiZhi);
			DrawLableText(hBtnDgsCurveshuiPingWeiZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

			memset(strText, 0, sizeof(strText));
		
			if(shengChengWeiZhi < firstSoundDistance) //��ȡ���������ߵ�һ��֮ǰ
			{
				if(gateDefectAmp && gSelectedDgsPointData[CurrentChannelNo][1][0].data)
					queXianDaXiao = (float)(20 * log10((float)gateDefectAmp / gSelectedDgsPointData[CurrentChannelNo][1][0].data));
			}
			else if(shengChengWeiZhi > lastSoundDistance) //��ȡ��������������֮��
			{
				if(gateDefectAmp && gSelectedDgsPointData[CurrentChannelNo][1][gSelectedDgsPointDataNum[CurrentChannelNo][1] - 1].data)
					queXianDaXiao = (float)(20 * log10((float)gateDefectAmp / gSelectedDgsPointData[CurrentChannelNo][1][gSelectedDgsPointDataNum[CurrentChannelNo][1] - 1].data));
			}
			else
			{
				WORD i = 0;
				for(i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][1] - 1; i++)
				{
					range1 = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * gSelectedDgsPointData[CurrentChannelNo][1][i].serial / (float)DATA_SIZE;
					range2 = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * gSelectedDgsPointData[CurrentChannelNo][1][i + 1].serial / (float)DATA_SIZE;
					if((range1 <= shengChengWeiZhi) && (range2 >= shengChengWeiZhi))//�������Ƿ��ڵ�ǰ�������ߵ�֮��
						break;
				}//�ж�����λ����������DGS���ߵ�֮��
				//��ǰ��ʱ�����ߵ��ֵ
				//i--; //ǰ��FORѭ��i�����һ��
			//	swprintf(strText, _T("%d"), i);
			//	DrawLableText(hBtnDgsCurveShengChengWeiZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				tempPointData1 = gSelectedDgsPointData[CurrentChannelNo][1][i];
				tempPointData2 = gSelectedDgsPointData[CurrentChannelNo][1][i + 1];
				if((range2 -range1) != 0)
					currentPointData = tempPointData1.data + (shengChengWeiZhi -range1) / (range2 - range1) * ((float)tempPointData2.data - (float)tempPointData1.data);
			//	swprintf(strText, _T("%.1f"), float(currentPointData)/MAX_HEIGHT*100.0f);
			//	DrawLableText(hBtnDgsCurveShengChengWeiZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			//	swprintf(strText, _T("%.1f"), range1);//float(gateDefectAmp)/MAX_HEIGHT*100.0f
			//	DrawLableText(hBtnDgsCurveShenDuWeiZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			//	swprintf(strText, _T("%.1f"), range2);//float(currentPointData)/MAX_HEIGHT*100.0f
			//	DrawLableText(hBtnDgsCurveshuiPingWeiZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				if(fabs(currentPointData) > 1E-6)
				{
					if(gateDefectAmp)
						queXianDaXiao = (float)(20 * log10(gateDefectAmp / currentPointData));

					if(fabs(queXianDaXiao - 0.1) < 1E-6)
						queXianDaXiao = 0;

				}
			
			}
			swprintf(strText, _T("%.1f"), queXianDaXiao);
			DrawLableText(hBtnDgsCurveQueXianDaXiao, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
		}
	}
	else
	{
		swprintf(strText, _T("%.1f"), shengChengWeiZhi);
		DrawLableText(hBtnDgsCurveShengChengWeiZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

		swprintf(strText, _T("%.1f"), shenDuWeiZhi);
		DrawLableText(hBtnDgsCurveShenDuWeiZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

		swprintf(strText, _T("%.1f"), shuiPingWeiZhi);
		DrawLableText(hBtnDgsCurveshuiPingWeiZhi, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

		swprintf(strText, _T("%.1f"), queXianDaXiao);
		DrawLableText(hBtnDgsCurveQueXianDaXiao, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}
}

/**************************
*DGS���ߴ��ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK dgsCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			//closeAllGate();//�ر�����բ��			
			SetTimer(hWnd, TIMER_DAC_DEFECT_DISPLAY, 400, defectDisplayProc);
			//����ť�ؼ�
			hBtnDgsCurveMake = CreateWindow(_T("BUTTON"),_T("makeDgsCurve"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_DGSCURVE_MAKE,hInst,NULL);
			hBtnDgsCurveOnOrOff = CreateWindow(_T("BUTTON"),_T("onOrOffDgsCurve"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_DGSCURVE_ONOROFF,hInst,NULL);
			hBtnDgsCurveQueXianDaXiao = CreateWindow(_T("BUTTON"),_T("queXianDaXiaoDgsCurve"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_DGSCURVE_QUEXIANDAXIAO,hInst,NULL);
			hBtnDgsCurveShengChengWeiZhi = CreateWindow(_T("BUTTON"),_T("ShengChengWeiZhiDgsCurve"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_DGSCURVE_SHENGCHENGWEIZHI,hInst,NULL);
			hBtnDgsCurveShenDuWeiZhi = CreateWindow(_T("BUTTON"),_T("meiZhanKuanDgsCurve"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_DGSCURVE_SHENDUWEIZHI,hInst,NULL);
			hBtnDgsCurveshuiPingWeiZhi = CreateWindow(_T("BUTTON"),_T("shuiPingWeiZhiDgsCurve"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 5*BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_DGSCURVE_SHUIPINGWEIZHI,hInst,NULL);

			dgsCurveBtnList[0] = hBtnDgsCurveMake;
			dgsCurveBtnList[1] = hBtnDgsCurveOnOrOff;
			dgsCurveBtnList[2] = hBtnDgsCurveQueXianDaXiao;
			dgsCurveBtnList[3] = hBtnDgsCurveShengChengWeiZhi;
			dgsCurveBtnList[4] = hBtnDgsCurveShenDuWeiZhi;
			dgsCurveBtnList[5] = hBtnDgsCurveshuiPingWeiZhi;
			
			//��ť�ؼ����Զ��崦�����

			OldBtnDgsCurveMakeProc = (WNDPROC) GetWindowLong(hBtnDgsCurveMake,GWL_WNDPROC);
			SetWindowLong(hBtnDgsCurveMake,GWL_WNDPROC,(LONG)btnDgsCurveMakeProc);
			
			OldBtnDgsCurveOnOrOffProc = (WNDPROC) GetWindowLong(hBtnDgsCurveOnOrOff,GWL_WNDPROC);
			SetWindowLong(hBtnDgsCurveOnOrOff,GWL_WNDPROC,(LONG)btnDgsCurveOnOrOffProc);
			
			OldBtnDgsCurveQueXianDaXiaoProc = (WNDPROC) GetWindowLong(hBtnDgsCurveQueXianDaXiao,GWL_WNDPROC);
			SetWindowLong(hBtnDgsCurveQueXianDaXiao,GWL_WNDPROC,(LONG)btnDgsCurveQueXianDaXiaoProc);
			
			OldBtnDgsCurveShengChengWeiZhiProc = (WNDPROC) GetWindowLong(hBtnDgsCurveShengChengWeiZhi,GWL_WNDPROC);
			SetWindowLong(hBtnDgsCurveShengChengWeiZhi,GWL_WNDPROC,(LONG)btnDgsCurveShengChengWeiZhiProc);

			OldBtnDgsCurveShenDuWeiZhiProc = (WNDPROC) GetWindowLong(hBtnDgsCurveShenDuWeiZhi, GWL_WNDPROC);
			SetWindowLong(hBtnDgsCurveShenDuWeiZhi,GWL_WNDPROC,(LONG)btnDgsCurveShenDuWeiZhiProc);
			
			OldBtnDgsCurveshuiPingWeiZhiProc = (WNDPROC) GetWindowLong(hBtnDgsCurveshuiPingWeiZhi,GWL_WNDPROC);
			SetWindowLong(hBtnDgsCurveshuiPingWeiZhi,GWL_WNDPROC,(LONG)btnDgsCurveshuiPingWeiZhiProc);
		
			currentDgsCurveBtn = dgsCurveBtnList[0];//Ĭ�ϵ�ǰѡ��������ť
			SetFocus(currentDgsCurveBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentDgsCurveBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				if(wParam == VK_ESCAPE)
				{					
					if(hWndTlAdjust)	//��·ר��
					{
						ShowWindow(hWndTlAdjust,SW_SHOW);
						SetFocus(currentTlAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					
						gCurrentFocusBtn = currentTlAdjustBtn;
						gCurrentWnd = hWndTlAdjust;

					//	setRailRoadParam(CurrentChannelNo, FALSE);//wangbingfu delete at 2011-5-27
					}
					else
					{	
						ShowWindow(hWndCurve,SW_SHOW);
						SetFocus(currentCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					
						gCurrentFocusBtn = currentCurveBtn;
						gCurrentWnd = hWndCurve;
					}

					DestroyWindow(hWndDgsCurve);
					hWndDgsCurve = 0;

					KillTimer(hWnd, TIMER_DAC_DEFECT_DISPLAY);
				}
			}
			break;
		case WM_COMMAND:

			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			dgsCurve_DrawButtonBitmap(hInst,pdis);
			
			ReadParamFile(lpDefaultParamFile);	//����˵�ʱgDgsSwitchֵ�ᱻ�޸ģ������Ҳ���
			
			if(pdis->hwndItem == hBtnDgsCurveOnOrOff)
			{
					if(gDgsSwitch[CurrentChannelNo][0])
						DrawLableText(hBtnDgsCurveOnOrOff, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					else
						DrawLableText(hBtnDgsCurveOnOrOff, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
			}
			else if(pdis->hwndItem == hBtnDgsCurveQueXianDaXiao || pdis->hwndItem == hBtnDgsCurveShengChengWeiZhi
				|| pdis->hwndItem == hBtnDgsCurveShenDuWeiZhi || pdis->hwndItem == hBtnDgsCurveshuiPingWeiZhi)
			{
					PEAK_INFO peakDefectInfos[2][2];
					InitializeCriticalSection(&csPeakDefectInfos);
					EnterCriticalSection(&csPeakDefectInfos);
					for(int i = 0 ;i < CHANNEL_NUM; i++)
					{
						peakDefectInfos[i][0] = gPeakDefectInfos[i][0];
					}
					LeaveCriticalSection(&csPeakDefectInfos);
					DeleteCriticalSection(&csPeakDefectInfos);
					
					displayDacFault(peakDefectInfos[CurrentChannelNo][0].wDefectAmp, peakDefectInfos[CurrentChannelNo][0].wDefectTime);
			}
			else
			{
				;//
			}
			break;
		case WM_CHAR:
			InitializeCriticalSection(&csSysInfo);
			EnterCriticalSection(&csSysInfo);
			if( wParam == '\t' && gSysInfo.wWorkMode != OneSendOneReceive)
			{
				// ���°�ť������
				if(CurrentChannelNo == 0)
				{
					if(!gDgsSwitch[1][0])
						DrawLableText(hBtnDgsCurveOnOrOff, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					else
						DrawLableText(hBtnDgsCurveOnOrOff, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else
				{
					if(!gDgsSwitch[0][0])
						DrawLableText(hBtnDgsCurveOnOrOff, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					else
						DrawLableText(hBtnDgsCurveOnOrOff, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}

				PEAK_INFO peakDefectInfos[2][2];
				InitializeCriticalSection(&csPeakDefectInfos);
				EnterCriticalSection(&csPeakDefectInfos);
				for(int i = 0 ;i < CHANNEL_NUM; i++)
				{
					peakDefectInfos[i][0] = gPeakDefectInfos[i][0];
				}
				LeaveCriticalSection(&csPeakDefectInfos);
				DeleteCriticalSection(&csPeakDefectInfos);
				
				displayDacFault(peakDefectInfos[CurrentChannelNo][0].wDefectAmp, peakDefectInfos[CurrentChannelNo][0].wDefectTime);
			}

			LeaveCriticalSection(&csSysInfo);
			DeleteCriticalSection(&csSysInfo);

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
*����DGS���ߴ��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int dgsCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_dgsCurveBtnBitmap[]=
	{
		{IDC_DGSCURVE_MAKE	,IDB_BITMAP_ZHIZUO6_NOSEL_R	 ,  IDB_BITMAP_ZHIZUO6_SELECT_R		},
		{IDC_DGSCURVE_ONOROFF	,IDB_BITMAP_KAIQIGUANBI6_NOSEL_R	 ,  IDB_BITMAP_KAIQIGUANBI6_SELECT_R		},
		{IDC_DGSCURVE_QUEXIANDAXIAO	,IDB_BITMAP_DACQUEXIANDAXIAO_NOSEL_R ,  IDB_BITMAP_DACQUEXIANDAXIAO_SELECT_R	},
		{IDC_DGSCURVE_SHENGCHENGWEIZHI	,IDB_BITMAP_DACSHENGCHENGWEIZHI_NOSEL_R ,  IDB_BITMAP_DACSHENGCHENGWEIZHI_SELECT_R	},
		{IDC_DGSCURVE_SHENDUWEIZHI	,IDB_BITMAP_DACSHENDUWEIZHI_NOSEL_R ,  IDB_BITMAP_DACSHENDUWEIZHI_SELECT_R	},
		{IDC_DGSCURVE_SHUIPINGWEIZHI	,IDB_BITMAP_DACSHUIPINGWEIZHI_NOSEL_R ,  IDB_BITMAP_DACSHUIPINGWEIZHI_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_dgsCurveBtnBitmap);
	return ReturnVal;
}

/**************************
*������ť�Զ��崦�����
* @param    hInstance[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnDgsCurveMakeProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	/*	case WM_SETFOCUS:
			InitializeCriticalSection(&csPeakDefectInfos);
			EnterCriticalSection(&csPeakDefectInfos);
			displayDacFault(gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp, gPeakDefectInfos[CurrentChannelNo][0].wDefectTime);
			LeaveCriticalSection(&csPeakDefectInfos);
			DeleteCriticalSection(&csPeakDefectInfos);			
			break;*/
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
					while(dgsCurveBtnList[i] != currentDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentDgsCurveBtn = dgsCurveBtnList[i];
					SetFocus(currentDgsCurveBtn);
					gCurrentFocusBtn = currentDgsCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(dgsCurveBtnList[i] != currentDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentDgsCurveBtn = dgsCurveBtnList[i];
					SetFocus(currentDgsCurveBtn);
					gCurrentFocusBtn = currentDgsCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndDgsMake = CreateWindow(szDgsMakeChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndDgsCurve,SW_HIDE);
					//SetFocus(hWndMakeDgsCurve);
					ShowWindow(hWndDgsMake,SW_SHOW);
					UpdateWindow(hWndDgsMake);

					gCurrentWnd = hWndDgsMake;
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(hWnd);

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
	return CallWindowProc(OldBtnDgsCurveMakeProc,hWnd,Msg,wParam,lParam);
}
/**************************
*����\�رհ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnDgsCurveOnOrOffProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	/*	case WM_SETFOCUS:
			InitializeCriticalSection(&csPeakDefectInfos);
			EnterCriticalSection(&csPeakDefectInfos);
			displayDacFault(gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp, gPeakDefectInfos[CurrentChannelNo][0].wDefectTime);
			LeaveCriticalSection(&csPeakDefectInfos);
			DeleteCriticalSection(&csPeakDefectInfos);
			break;*/
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
					while(dgsCurveBtnList[i] != currentDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentDgsCurveBtn = dgsCurveBtnList[i];
					SetFocus(currentDgsCurveBtn);
					gCurrentFocusBtn = currentDgsCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(dgsCurveBtnList[i] != currentDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentDgsCurveBtn = dgsCurveBtnList[i];
					SetFocus(currentDgsCurveBtn);
					gCurrentFocusBtn = currentDgsCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
				//д��բ�ţ�����բ�ű���
				/*	if(chCurrent_gate_reg->wGate2Enable  == 0)
					{
						chCurrent_gate_reg->wGate1Enable = 1;
						chCurrent_gate_reg->wGate2Enable = 1;
						chCurrent_gate_reg->wGate3Enable = 1;

						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							for(int j = 0; j < GATE_NUM; j++)
							{
								gDgsSwitch[i][j] = ON;
								saveDgsFile(_T("\\ResidentFlash\\dgs.bin"));
							}
						}

						DrawLableText(hWnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					}
					else
					{
						chCurrent_gate_reg->wGate1Enable = 0;
						chCurrent_gate_reg->wGate2Enable = 0;
						chCurrent_gate_reg->wGate3Enable = 0;

						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							for(int j = 0; j < GATE_NUM; j++)
							{
								gDgsSwitch[i][j] = OFF;
								saveDgsFile(_T("\\ResidentFlash\\dgs.bin"));
							}
						}

						DrawLableText(hWnd, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					}	
					Sleep(50);
					InitializeCriticalSection(&csPeakDefectInfos);
					EnterCriticalSection(&csPeakDefectInfos);
					displayDacFault(gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp, gPeakDefectInfos[CurrentChannelNo][0].wDefectTime);
					LeaveCriticalSection(&csPeakDefectInfos);
					DeleteCriticalSection(&csPeakDefectInfos);
					*/
					//��д��բ�ţ�������բ�ű���
					if(!gDgsSwitch[CurrentChannelNo][0])
					{
						if(!gbGuanLian) //�ǹ���
						{
							if(gSelectedDgsPointDataNum[CurrentChannelNo][0] > 1)
							{
								for(int j = 0; j < GATE_NUM; j++)
								{
									gDgsSwitch[CurrentChannelNo][j] = TRUE;

									DrawLableText(hWnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
								}
							}
							else
							{
								if(CurrentChannelNo == 0)
								{
									PostMessage(hStatusBar, SB_SETTEXT, 
										(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("ͨ��1��DAC"));
								}
								else
								{
									PostMessage(hStatusBar, SB_SETTEXT, 
										(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("ͨ��2��DAC"));
								}
							}
						}
						else
						{
							for(int i = 0; i < CHANNEL_NUM; i++)
							{
								if(gSelectedDgsPointDataNum[i][0] > 1)
								{
									for(int j = 0; j < GATE_NUM; j++)
									{
										gDgsSwitch[i][j] = TRUE;
									}
									DrawLableText(hWnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
								}
								else
								{
									if(i == 0) //ͨ��1������
									{
										PostMessage(hStatusBar, SB_SETTEXT, 
											(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("ͨ��1��DAC"));
									}
									else if(i == 1)	//ͨ��2������
									{
										if(gSelectedDgsPointDataNum[0][0] <= 1)//ͨ��1Ҳ������
										{
											PostMessage(hStatusBar, SB_SETTEXT, 
												(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��ͨ������DAC"));
										}
										else//ͨ��1������
										{
											PostMessage(hStatusBar, SB_SETTEXT, 
											(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("ͨ��2��DAC"));
										}
									}
									else
									{
										;//
									}
								}
							}
						}
					//	SetOpenFlag(NULL, TRUE, 1);
					//	SetOpenFlag(NULL, FALSE, 2);
				
						//saveDgsFile(_T("\\ResidentFlash\\dgs.bin"));
						// �������õ�Ĭ���ļ�
						SaveParamFile(lpDefaultParamFile);

						Sleep(50);
						InitializeCriticalSection(&csPeakDefectInfos);
						EnterCriticalSection(&csPeakDefectInfos);
						displayDacFault(gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp, gPeakDefectInfos[CurrentChannelNo][0].wDefectTime);
						LeaveCriticalSection(&csPeakDefectInfos);
						DeleteCriticalSection(&csPeakDefectInfos);
					}
					else
					{

						if(!gbGuanLian) //�ǹ���
						{
							for(int j = 0; j < GATE_NUM; j++)
							{
								gDgsSwitch[CurrentChannelNo][j] = FALSE;
								//saveDgsFile(_T("\\ResidentFlash\\dgs.bin"));
								// �������õ�Ĭ���ļ�
								//SaveParamFile(lpDefaultParamFile);
							}
						}
						else
						{
							for(int i = 0; i < CHANNEL_NUM; i++)
							{
								for(int j = 0; j < GATE_NUM; j++)
								{
									gDgsSwitch[i][j] = FALSE;
								}
							}
						}

					/*	SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
						SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);*/
						DrawLableText(hWnd, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

						//saveDgsFile(_T("\\ResidentFlash\\dgs.bin"));
						// �������õ�Ĭ���ļ�
						SaveParamFile(lpDefaultParamFile);
					}	
					Sleep(50);
					InitializeCriticalSection(&csPeakDefectInfos);
					EnterCriticalSection(&csPeakDefectInfos);
					displayDacFault(gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp, gPeakDefectInfos[CurrentChannelNo][0].wDefectTime);
					LeaveCriticalSection(&csPeakDefectInfos);
					DeleteCriticalSection(&csPeakDefectInfos);
				}				
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(hWnd);

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
	return CallWindowProc(OldBtnDgsCurveOnOrOffProc,hWnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnDgsCurveQueXianDaXiaoProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			InitializeCriticalSection(&csPeakDefectInfos);
			EnterCriticalSection(&csPeakDefectInfos);
			displayDacFault(gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp, gPeakDefectInfos[CurrentChannelNo][0].wDefectTime);
			LeaveCriticalSection(&csPeakDefectInfos);
			DeleteCriticalSection(&csPeakDefectInfos);
			break;
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
					while(dgsCurveBtnList[i] != currentDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentDgsCurveBtn = dgsCurveBtnList[i];
					SetFocus(currentDgsCurveBtn);
					gCurrentFocusBtn = currentDgsCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(dgsCurveBtnList[i] != currentDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentDgsCurveBtn = dgsCurveBtnList[i];
					SetFocus(currentDgsCurveBtn);
					gCurrentFocusBtn = currentDgsCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(hWnd);

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
	return CallWindowProc(OldBtnDgsCurveQueXianDaXiaoProc,hWnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnDgsCurveShengChengWeiZhiProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			InitializeCriticalSection(&csPeakDefectInfos);
			EnterCriticalSection(&csPeakDefectInfos);
			displayDacFault(gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp, gPeakDefectInfos[CurrentChannelNo][0].wDefectTime);
			LeaveCriticalSection(&csPeakDefectInfos);
			DeleteCriticalSection(&csPeakDefectInfos);
			break;
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
					while(dgsCurveBtnList[i] != currentDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentDgsCurveBtn = dgsCurveBtnList[i];
					SetFocus(currentDgsCurveBtn);
					gCurrentFocusBtn = currentDgsCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(dgsCurveBtnList[i] != currentDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentDgsCurveBtn = dgsCurveBtnList[i];
					SetFocus(currentDgsCurveBtn);
					gCurrentFocusBtn = currentDgsCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{

					HWND hParentWnd = GetParent(hWnd);

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
	return CallWindowProc(OldBtnDgsCurveShengChengWeiZhiProc,hWnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnDgsCurveShenDuWeiZhiProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			InitializeCriticalSection(&csPeakDefectInfos);
			EnterCriticalSection(&csPeakDefectInfos);
			displayDacFault(gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp, gPeakDefectInfos[CurrentChannelNo][0].wDefectTime);
			LeaveCriticalSection(&csPeakDefectInfos);
			DeleteCriticalSection(&csPeakDefectInfos);
			break;
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
					while(dgsCurveBtnList[i] != currentDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentDgsCurveBtn = dgsCurveBtnList[i];
					SetFocus(currentDgsCurveBtn);
					gCurrentFocusBtn = currentDgsCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(dgsCurveBtnList[i] != currentDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentDgsCurveBtn = dgsCurveBtnList[i];
					SetFocus(currentDgsCurveBtn);
					gCurrentFocusBtn = currentDgsCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(hWnd);

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
	return CallWindowProc(OldBtnDgsCurveShenDuWeiZhiProc,hWnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnDgsCurveshuiPingWeiZhiProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			InitializeCriticalSection(&csPeakDefectInfos);
			EnterCriticalSection(&csPeakDefectInfos);
			displayDacFault(gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp, gPeakDefectInfos[CurrentChannelNo][0].wDefectTime);
			LeaveCriticalSection(&csPeakDefectInfos);
			DeleteCriticalSection(&csPeakDefectInfos);
			break;
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
					while(dgsCurveBtnList[i] != currentDgsCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentDgsCurveBtn = dgsCurveBtnList[i];
					SetFocus(currentDgsCurveBtn);
					gCurrentFocusBtn = currentDgsCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(dgsCurveBtnList[i] != currentDgsCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentDgsCurveBtn = dgsCurveBtnList[i];
					SetFocus(currentDgsCurveBtn);
					gCurrentFocusBtn = currentDgsCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(hWnd);

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
	return CallWindowProc(OldBtnDgsCurveshuiPingWeiZhiProc,hWnd,Msg,wParam,lParam);
}
