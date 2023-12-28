/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ��������ʾ����Դ�ļ�
 * DisplayCurrentname: chDisplayWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"

#include "resource.h"
#include "interfaceCmdID.h"

#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"
#include "gateWin.h"

#define AVER_NUM 20// ƽ������
HWND hWndChDisplay = NULL;//��������ʾ���ھ��
HWND currentChDisplayBtn = NULL;// ��������ʾ���ڵ�ǰѡ�еİ�ť

BOOL gbMeasureSwitch = FALSE;	//�Ƿ��ڲ���

HANDLE hDrawMeasureThread = NULL;				// ������ʾ�߳̾��
DRAWTHREAD_PARAM DrawMeasureThread_param;		// ������ʾ�̲߳���
DWORD dwDrawMeasureThreadID = 0;				// ������ʾ�߳�ID
CRITICAL_SECTION csMeasure;

static HWND hBtnDisplaySwitch = NULL;//���ذ�ť���
static HWND hBtnDisplayMax = NULL;//���ֵ��ť���
static HWND hBtnDisplayMin = NULL;//��Сֵ��ť���
static HWND hBtnDisplayAverage = NULL;//ƽ��ֵ��ť���
static HWND hBtnDisplayCurrent = NULL;//��ǰֵ��ť���
static HWND hBtnDisplayChaZhi = NULL;//��ֵ��ť���
static HWND hBtnDisplayTuCeng = NULL;	//Ϳ���Ȱ�ť���


static HWND chDisplayBtnList[2] = {NULL,NULL};//��������ʾ���ڵİ�ť��
static const int buttonNum = 2;//��ť�ؼ���

static WNDPROC OldBtnDisplaySwitchProc = NULL;//���ذ�ťĬ�ϴ������
static WNDPROC OldBtnDisplayAverageProc = NULL;//ƽ��ֵ��ťĬ�ϴ������
//static WNDPROC OldBtnDisplayMaxProc = NULL;//���ֵ��ťĬ�ϴ������
//static WNDPROC OldBtnDisplayMinProc = NULL;// ��Сֵ��ťĬ�ϴ������
//static WNDPROC OldBtnDisplayCurrentProc = NULL;//��ǰֵ��ťĬ�ϴ������
//static WNDPROC OldBtnDisplayChaZhiProc = NULL;//��ֵ��ťĬ�ϴ������

ATOM registerChDisplayChild(HINSTANCE hInstance);
static LRESULT CALLBACK chDisplayChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int chDisplay_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnDisplaySwitchProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnDisplayAverageProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
//static LRESULT CALLBACK BtnDisplayMaxProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
//static LRESULT CALLBACK btnDisplayMinProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
//static LRESULT CALLBACK btnDisplayChaZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

extern HINSTANCE hInst;			//����ʵ�����
extern HWND hWndMain;			//�����ھ��
extern HWND gCurrentFocusBtn;
extern HWND gCurrentWnd;
extern MEASURE_DISPLAY_ALL gstMeasureDisplay;
extern WORD CurrentChannelNo;
extern float fRangeEnd[2];
extern float fRangeStart[2];
extern PIC_DATA gPicData[2];									// ͼ������
extern float gfMinRange[2];
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern PEAK_INFO gPeakDefectInfos[2][2];						//����բ�ŷ�ֵ����Ϣ
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern GATE_INFO GateInfos[2][3];
extern BOOL gbHeadVCurveFlag;									//̽ͷV�Ͳ��������Ƿ��
extern BOOL gbTempCurveFlag;									//�Ƿ���¶Ȳ�������
extern WORD gwHeadVPointNum[20];								//̽ͷV�Ͳ������ߵ�ĸ���
extern HEADVPOINT_DATA gstHeadVPointData[20][HEADV_POINT_MAX];	//�ɼ���̽ͷV�Ͳ������ߵ�
extern WORD gwHeadVHeadNum;										//̽ͷ�ͺ�
extern float gfCurrentTemperature;								//��ǰ�¶�
extern WORD gwTempCurvePointNum[MATERIAL_NUM + 10];								//̽ͷV�Ͳ������ߵ�ĸ���
extern TEMPCURVEPOINT_DATA gstTempCurvePointData[MATERIAL_NUM + 10][HEADV_POINT_MAX];	//�ɼ���̽ͷV�Ͳ������ߵ�
extern WORD gwTempCurveMaterial;								//�¶Ȳ�������
extern WORD gwTuCengSpeed;										//Ϳ������
extern WORD gwTuCengType;										//Ϳ��������ͺ�
extern float fCurrentValue1;									//��ǰֵ1
extern float fCurrentValue2;									//��ǰֵ2
extern float fCurrentValue3;									//��ǰֵ3
extern float fCurrentValue4;									//��ǰֵ4
extern float fCurrentValue5;									//��ǰֵ5
extern float fCurrentValue6;									//��ǰֵ6
extern HWND hWndChAverage;										//���ƽ��ֵ���ھ��
extern HWND currentChAverageBtn;								//���ƽ��ֵ���ڵ�ǰѡ�еİ�ť
extern BOOL  gbHotGateFlag;	//�Ƿ��բ�ſ�ݷ�ʽ
extern BOOL  gbHotAutoGainFlag;	//�Ƿ���Զ������ݷ�ʽ 
extern BOOL  gbHotAutoAdjustFlag;	//�Ƿ���Զ�У׼��ݷ�ʽ 
extern ARS200FPGA_SYS_REG* sys_reg;
extern GATE_INFO stOldGateInfos[2][3];


extern HWND hWndChMenu;//���˵����ھ��
extern BOOL bFreezeUp;
extern BOOL gbHotDB;
extern BOOL gbHotRange;	// ��ݼ�����
extern wchar_t gszTextPlus[10];
extern wchar_t gszTextRefPlus[10];
extern CRITICAL_SECTION csHotRange;
extern HWND hStatusBar;					// ״̬�����
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern CRITICAL_SECTION csPeakDefectInfos;
extern WORD tuCengTypeToWaveSpeed(WORD wType);

#if 0
/**************************
*��һ���ز�����
* @return   fDistance : ����
* @since    1.00
***************************/
float firstHuiBoDistance()
{
	int i = 21;
	int nSerial = 0;
	WORD nCurrenData = 0;
	WORD wNum = DATA_SIZE;
	float fDistance = 0;

	if(DisplaySet_ad[CurrentChannelNo].fRange < gfMinRange[CurrentChannelNo])//С����С����
	{
		wNum = WORD((DisplaySet_ad[CurrentChannelNo].fRange / gfMinRange[CurrentChannelNo])*DATA_SIZE);
	}
	while(gPicData[CurrentChannelNo].data[i] > 200)
	{
		i++;
	}
	
	for(int j = i + 10; j < DATA_SIZE - 1; j++)
	{
		if(nCurrenData < gPicData[CurrentChannelNo].data[j])
		{
			nCurrenData = gPicData[CurrentChannelNo].data[j];
			nSerial = j;
		}
	}
	fDistance = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * nSerial / wNum;

	return fDistance;
}

/**************************
*�ڶ����ز�����
* @return   fDistance : ����
* @since    1.00
***************************/
float secondHuiBoDistance()
{
	int i = 21;
	int nSerial = 0;
	WORD nCurrenData = 0;
	WORD wNum = DATA_SIZE;
	float fDistance = 0;

	if(DisplaySet_ad[CurrentChannelNo].fRange < gfMinRange[CurrentChannelNo])//С����С����
	{
		wNum = WORD((DisplaySet_ad[CurrentChannelNo].fRange / gfMinRange[CurrentChannelNo])*DATA_SIZE);
	}
	while(gPicData[CurrentChannelNo].data[i] > 200)
	{
		i++;
	}
	//��һ���ز���ߵ�
	for(int k = i + 10; k < DATA_SIZE - 1; k++)
	{
		if(nCurrenData < gPicData[CurrentChannelNo].data[k])
		{
			nCurrenData = gPicData[CurrentChannelNo].data[k];
			nSerial = k;
		}
	}
	nCurrenData = 0;
	//��һ���ز�֮��ĵ�
	while(gPicData[CurrentChannelNo].data[nSerial] > 150)
	{
		nSerial++;
	}
	//�ڶ����ز���ߵ�
	for(int j = nSerial + 10; j < DATA_SIZE - 1; j++)
	{
		if(nCurrenData < gPicData[CurrentChannelNo].data[j])
		{
			nCurrenData = gPicData[CurrentChannelNo].data[j];
			nSerial = j;
		}
	}
	fDistance = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * nSerial / wNum;

	return fDistance;
}
#endif
/**************************
*��ʾ��������
* @param    hWnd[in]:���ھ��
* @param    bSwitch[in]: ��/��
* @return   ATOM :
* @since    1.00
***************************/
void displayMeasureSwitch(HWND hWnd, BOOL bSwitch)
{
	if(bSwitch)
		DrawLableText(hWnd, _T("��ʼ����"), 22, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	else
	{
		DrawLableText(hWnd, _T("��������"), 22, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		
		if(gstMeasureDisplay.stMax.bSwitch == ON)//���ֵ
			DrawLableText(hBtnDisplayMax, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		else
			DrawLableText(hBtnDisplayMax, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		
		if(gstMeasureDisplay.stMin.bSwitch == ON)//��Сֵ
			DrawLableText(hBtnDisplayMin, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		else
			DrawLableText(hBtnDisplayMin, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		
		if(gstMeasureDisplay.stAverage.bSwitch == ON)//ƽ��ֵ
			DrawLableText(hBtnDisplayAverage, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		else
			DrawLableText(hBtnDisplayAverage, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		
		if(gstMeasureDisplay.stCurrent.bSwitch == ON)//��ǰֵ
			DrawLableText(hBtnDisplayCurrent, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		else
			DrawLableText(hBtnDisplayCurrent, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		
		if(gstMeasureDisplay.stChaZhi.bSwitch == ON)//��ֵ
			DrawLableText(hBtnDisplayChaZhi, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		else
			DrawLableText(hBtnDisplayChaZhi, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);

		DrawLableText(hBtnDisplayTuCeng, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	}
}
/**************************
*��ʾ���ֵ
* @param		hWnd[in]:���ھ��
* @param		fMax[in]:���ֵ
* @return   LRESULT :
* @since    1.00
***************************/
void displayMeasureMax(HWND hWnd ,float fMax)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(gbMeasureSwitch)
	{
		wsprintf(strText, _T("%.2f"), fMax);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	}
	else
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
}
/**************************
*��ʾ��Сֵ
* @param		hWnd[in]:���ھ��
* @param		fMax[in]:���ֵ
* @return   LRESULT :
* @since    1.00
***************************/
void displayMeasureMin(HWND hWnd ,float fMin)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(gbMeasureSwitch)
	{
		if(fabs(fMin - 10000) < 1E-6)
		{
			fMin = 0.0f;
		}
		wsprintf(strText, _T("%.2f"), fMin);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	}
	else
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
}
/**************************
*��ʾ��ƽ��ֵ
* @param		hWnd[in]:���ھ��
* @param		fMax[in]:���ֵ
* @return   LRESULT :
* @since    1.00
***************************/
void displayMeasureAverage(HWND hWnd ,float fAverage)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	if(fAverage == 0)
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	else
	{
		wsprintf(strText, _T("%.2f"), fAverage);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	}
}

/**************************
*��ʾ��ǰֵ
* @param		hWnd[in]:���ھ��
* @param		fMax[in]:���ֵ
* @return   LRESULT :
* @since    1.00
***************************/
void displayMeasureCurrent(HWND hWnd ,float fCurrent)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(gbMeasureSwitch)
	{
		if(fCurrent < 0)
		{
			fCurrent = 0;
			
		}
		wsprintf(strText, _T("%.2f"), fCurrent);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	}
	else
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
}

/**************************
*��ʾ��ֵ
* @param		hWnd[in]:���ھ��
* @param		fMax[in]:���ֵ
* @return   LRESULT :
* @since    1.00
***************************/
void displayMeasureChaZhi(HWND hWnd ,WORD wMode, float fChaZhi)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	if(gbMeasureSwitch)
	{
		if(wMode == 0)
		{
			wsprintf(strText, _T("%.2f"), fChaZhi);
			DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		}
		else if(wMode == 1)
		{
			wsprintf(strText, _T("%.2f%c"), fChaZhi * 100, '%');
			DrawLableText(hWnd, strText, 24, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		}				
	}
	else
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
}

/**************************
*��ʾͿ����
* @param		hWnd[in]:���ھ��
* @param		fHouDu[in]:���ֵ
* @return   LRESULT :
* @since    1.00
***************************/
void displayMeasureTuCeng(HWND hWnd ,float fHouDu, WORD wMeasureMethod)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(gbMeasureSwitch)
	{
		wsprintf(strText, _T("%.2f"), fHouDu);
		if(wMeasureMethod != 3)
			DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
		else
			DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
	}
	else
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_7_START_Y);
}

/**************************
*����������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
DWORD WINAPI measureProc(LPVOID lpParameter)
{
	WORD wCount = 0;//������20������ȡƽ��
	float fCurrentValue[AVER_NUM] = {0};
	float fGateWidth = (DisplaySet_ad[CurrentChannelNo].fRange / 100) * 4;	//����բ�ſ������Ļ��һ��
	float fFirstHuiBoDistance = firstHuiBoDistance();						//��һ���ײ�����
	float fSecondHuiBoDistance = secondHuiBoDistance();						//�ڶ����ײ�����
	float fGate1QianDian = fFirstHuiBoDistance - fGateWidth / 2;			//բ��1���
	float fGate2QianDian = fSecondHuiBoDistance - fGateWidth /2;			//բ��2���
	float fGate1Back = fGate1QianDian + fGateWidth;							//բ��1����
	float fGate2Back = fGate2QianDian + fGateWidth;							//բ��2����
	float fRange1 = 0;														//բ��1��ȡ�Ĳ��������
	float fRange2 = 0;														//բ��2��ȡ�Ĳ��������
	gstMeasureDisplay.stMax.fValue = 0;
	gstMeasureDisplay.stMin.fValue = 10000;

	PEAK_INFO peakDefectInfos[2][2];
	memset(peakDefectInfos, 0, sizeof(peakDefectInfos));
		
/*	InitializeCriticalSection(&csPeakDefectInfos);
	EnterCriticalSection(&csPeakDefectInfos);
	for(int i = 0 ;i < CHANNEL_NUM; i++)
	{
		if(gPeakDefectInfos[i][0].wDefectAmp > double(MAX_HEIGHT * 5 / 100)) //��ֵ����5%ȡ��
		{
			peakDefectInfos[i][0] = gPeakDefectInfos[i][0];
		}
	}
	LeaveCriticalSection(&csPeakDefectInfos);
	DeleteCriticalSection(&csPeakDefectInfos);
	
	if(gstMeasureDisplay.wMethod == 0)
	{
		SetQiDian(NULL, 1, fGate1QianDian,fGate1Back, FALSE);

		GateInfos[CurrentChannelNo][0].fPeakGate = 62;
		chCurrent_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 62 / 100.0f);
		
		GateInfos[CurrentChannelNo][0].bOpen = ON;
		chCurrent_gate_reg->wGate1Enable = ON;
		Sleep(15);
		
		fRange1 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][0].wDefectTime);
		gstMeasureDisplay.stMin.fValue = fRange1;
	}
	else if(gstMeasureDisplay.wMethod == 2)
	{
		SetQiDian(NULL, 1, fGate1QianDian,fGate1Back, FALSE);
		SetQiDian(NULL, 2, fGate2QianDian,fGate2Back, FALSE);

		GateInfos[CurrentChannelNo][0].fPeakGate = 62;
		chCurrent_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 62 / 100.0f);
		GateInfos[CurrentChannelNo][1].fPeakGate = 62;
		chCurrent_gate_reg->wGate2Peak = WORD(MAX_HEIGHT * 62 / 100.0f);

		GateInfos[CurrentChannelNo][0].bOpen = ON;
		chCurrent_gate_reg->wGate1Enable = ON;
		GateInfos[CurrentChannelNo][1].bOpen = ON;
		chCurrent_gate_reg->wGate2Enable = ON;
		Sleep(15);
		
		fRange1 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][0].wDefectTime);
		fRange2 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][1].wDefectTime);
		gstMeasureDisplay.stMin.fValue = fRange2 - fRange1;
	}*/
	while(1)
	{
		InitializeCriticalSection(&csMeasure);
		EnterCriticalSection(&csMeasure);
		
		BOOL bMeasureSwitch = gbMeasureSwitch;
		
		LeaveCriticalSection(&csMeasure);
		DeleteCriticalSection(&csMeasure);

		memset(peakDefectInfos, 0, sizeof(peakDefectInfos));
		InitializeCriticalSection(&csPeakDefectInfos);
		EnterCriticalSection(&csPeakDefectInfos);
		for(int i = 0 ;i < CHANNEL_NUM; i++)
		{
			for(int j = 0; j < 2; j++)
			{
				if(gPeakDefectInfos[i][j].wDefectAmp > double(MAX_HEIGHT * 5 / 100)) //��ֵ����5%ȡ��
				{
					peakDefectInfos[i][j] = gPeakDefectInfos[i][j];
				}
			}
		}
		LeaveCriticalSection(&csPeakDefectInfos);
		DeleteCriticalSection(&csPeakDefectInfos);
	
		fGateWidth = (DisplaySet_ad[CurrentChannelNo].fRange / 100) * 4;//����բ�ſ������Ļ��һ��
		
		if(bMeasureSwitch == ON)
		{
			if((gCurrentWnd == hWndChDisplay  || gCurrentWnd == hWndChAverage)&& !gbHotGateFlag && !gbHotAutoGainFlag && !gbHotAutoAdjustFlag)//GetForegroundWindow() == hBtnDisplaySwitch || GetForegroundWindow() == hBtnDisplayAverage���ã�
			{
				if(gstMeasureDisplay.wMethod == 0 || gstMeasureDisplay.wMethod == 1)
				{
					//�ȹص�����բ�� ����ײ��ײ�����������ز���ʱ����Ҫ�ȹص�����բ��
					GateInfos[CurrentChannelNo][0].bOpen = OFF;
					chCurrent_gate_reg->wGate1Enable = OFF;
					GateInfos[CurrentChannelNo][1].bOpen = OFF;
					chCurrent_gate_reg->wGate2Enable = OFF;
					
					fFirstHuiBoDistance = firstHuiBoDistance();
					fGate1QianDian = fFirstHuiBoDistance - fGateWidth / 2;
					fGate1Back = fGate1QianDian + fGateWidth;
					
					SetQiDian(NULL, 1, fGate1QianDian,fGate1Back, FALSE);
					
					GateInfos[CurrentChannelNo][0].fPeakGate = 62;
					chCurrent_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 62 / 100.0f);
					
					GateInfos[CurrentChannelNo][0].bOpen = ON;
					chCurrent_gate_reg->wGate1Enable = ON;
					Sleep(5);
					
					fRange1 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][0].wDefectTime);
				//	gstMeasureDisplay.stCurrent.fValue = fRange1;
					fCurrentValue[wCount] = fRange1;
				}
				else if(gstMeasureDisplay.wMethod == 2 || gstMeasureDisplay.wMethod == 3)
				{
					fFirstHuiBoDistance = firstHuiBoDistance();
					fSecondHuiBoDistance = secondHuiBoDistance();
					fGate1QianDian = fFirstHuiBoDistance - fGateWidth / 2;
					fGate2QianDian = fSecondHuiBoDistance - fGateWidth / 2;
					fGate1Back = fGate1QianDian + fGateWidth;
					fGate2Back = fGate2QianDian + fGateWidth;
					
					SetQiDian(NULL, 1, fGate1QianDian,fGate1Back, FALSE);
					SetQiDian(NULL, 2, fGate2QianDian,fGate2Back, FALSE);
					
					GateInfos[CurrentChannelNo][0].fPeakGate = 62;
					chCurrent_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 62 / 100.0f);
					GateInfos[CurrentChannelNo][1].fPeakGate = 62;
					chCurrent_gate_reg->wGate2Peak = WORD(MAX_HEIGHT * 62 / 100.0f);

					GateInfos[CurrentChannelNo][0].bOpen = ON;
					chCurrent_gate_reg->wGate1Enable = ON;
					GateInfos[CurrentChannelNo][1].bOpen = ON;
					chCurrent_gate_reg->wGate2Enable = ON;
					Sleep(5);
					
					fRange1 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][0].wDefectTime);
					fRange2 = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, (int)peakDefectInfos[CurrentChannelNo][1].wDefectTime);
				//	gstMeasureDisplay.stCurrent.fValue = fRange2 - fRange1;
					fCurrentValue[wCount] = fRange2 - fRange1;
				}
				wCount++;
				if(wCount == AVER_NUM)
				{
					BOOL bSameFlag[AVER_NUM];
					memset(bSameFlag, 0 ,sizeof(bSameFlag));
					WORD wSameCount[AVER_NUM];
					memset(wSameCount, 1 ,sizeof(wSameCount));
					float fTemp = 0;
					//��С��������
					for(int i = 0; i < wCount - 1; i++)
					{
						for(int j = i + 1; j < wCount; j++)
						{
							if(fCurrentValue[i] > fCurrentValue[j]) 
							{
								fTemp = fCurrentValue[j];
								fCurrentValue[j] = fCurrentValue[i];
								fCurrentValue[i] = fTemp;
							}
						}
					}
				/*	for(int i = 1; i < wCount ; i++)//������һ�����Ƚ�
					{
						fCurrentValue[0] = TRUE;
						if(fabs(WORD(fCurrentValue[i]) - WORD(fCurrentValue[0])) < 1E-6)
						{
							bSameFlag[i] = TRUE;
							wSameCount[0]++;
						}
					}*/
					for(int i = 0; i < wCount ; i++)
					{
						if(!bSameFlag[i])	//�����������������ͬ
						{
							bSameFlag[i] = TRUE;   //�´β����������Ƚ�
							for(int j = i + 1; j < wCount; j++)
							{
								if(fabs(WORD(fCurrentValue[j]) - WORD(fCurrentValue[i])) < 1E-6)
								{
									bSameFlag[j] = TRUE;
									wSameCount[i]++;
								}
							}
						}
					}
					WORD wMaxCountPosition = 0;	//��������λ��
					WORD wTempCount = wSameCount[0]; //��ʱ����������
					//ȡ��������λ��
					for(int i = 0; i < wCount; i++)
					{
						if(wTempCount < wSameCount[i])
						{
							wTempCount = wSameCount[i];
							wMaxCountPosition = i;
						}
					}
					float fAllCurrentValue = 0.0f;
					for(int i = 0; i < wCount; i++)
					{
						if(fabs(WORD(fCurrentValue[i]) - WORD(fCurrentValue[wMaxCountPosition])) < 1E-6)
						{
							fAllCurrentValue += fCurrentValue[i];
						}
					}
					gstMeasureDisplay.stCurrent.fValue = fAllCurrentValue / wSameCount[wMaxCountPosition];
			//		displayMeasureAverage(hBtnDisplayAverage, (float)wSameCount[wMaxCountPosition]);
			//		displayMeasureAverage(hBtnDisplayTuCeng, fAllCurrentValue);
					wCount = 0;
					if(gbHeadVCurveFlag == ON)
					{
						HEADVPOINT_DATA stTempPointData1; 
						HEADVPOINT_DATA stTempPointData2;
						memset(&stTempPointData1, 0, sizeof(HEADVPOINT_DATA));
						memset(&stTempPointData1, 0, sizeof(HEADVPOINT_DATA));
						if(gwHeadVPointNum[gwHeadVHeadNum] == 1)
						{
							if(gstMeasureDisplay.stCurrent.fValue == gstHeadVPointData[gwHeadVHeadNum][0].fActualHouDu)
								gstMeasureDisplay.stCurrent.fValue = gstHeadVPointData[gwHeadVHeadNum][0].fMeasureHouDu;
						}
						for(WORD i = 0; i < gwHeadVPointNum[gwHeadVHeadNum] - 1; i++)
						{
							stTempPointData1 = gstHeadVPointData[gwHeadVHeadNum][i];
							stTempPointData2 = gstHeadVPointData[gwHeadVHeadNum][i + 1];
							if(gstMeasureDisplay.stCurrent.fValue >= gstHeadVPointData[gwHeadVHeadNum][i].fActualHouDu
								&& gstMeasureDisplay.stCurrent.fValue <= gstHeadVPointData[gwHeadVHeadNum][i + 1].fActualHouDu)
							{
								if(stTempPointData2.fActualHouDu- stTempPointData1.fActualHouDu != 0)
								{
									gstMeasureDisplay.stCurrent.fValue = (stTempPointData2.fMeasureHouDu - stTempPointData1.fMeasureHouDu) / (stTempPointData2.fActualHouDu- stTempPointData1.fActualHouDu)
																		* (gstMeasureDisplay.stCurrent.fValue - stTempPointData1.fActualHouDu) + stTempPointData1.fMeasureHouDu;
								}
							}
						}
					}
					else if(gbTempCurveFlag)
					{
						TEMPCURVEPOINT_DATA stTempPointData1; 
						TEMPCURVEPOINT_DATA stTempPointData2;
						memset(&stTempPointData1, 0, sizeof(TEMPCURVEPOINT_DATA));
						memset(&stTempPointData1, 0, sizeof(TEMPCURVEPOINT_DATA));
						WORD wCurrentWaveSpeed = 0;		//��ǰ�¶�����
						if(gwTempCurvePointNum[gwTempCurveMaterial] == 1)
						{
							wCurrentWaveSpeed = gstTempCurvePointData[gwTempCurveMaterial][0].wWaveSpeed;
						}
						for(WORD i = 0; i < gwTempCurvePointNum[gwTempCurveMaterial] - 1; i++)
						{
							stTempPointData1 = gstTempCurvePointData[gwTempCurveMaterial][i];
							stTempPointData2 = gstTempCurvePointData[gwTempCurveMaterial][i + 1];
							if(gfCurrentTemperature>= gstTempCurvePointData[gwTempCurveMaterial][i].fTemperature
								&& gfCurrentTemperature <= gstTempCurvePointData[gwTempCurveMaterial][i + 1].fTemperature)
							{
								if(stTempPointData2.fTemperature- stTempPointData1.fTemperature != 0)
								{
									wCurrentWaveSpeed = (WORD)(((float)stTempPointData2.wWaveSpeed - (float)stTempPointData1.wWaveSpeed) / (stTempPointData2.fTemperature- stTempPointData1.fTemperature)
																		* (gfCurrentTemperature - stTempPointData1.fTemperature) + stTempPointData1.wWaveSpeed);
								}
							}
						}
						gstMeasureDisplay.stCurrent.fValue = gstMeasureDisplay.stCurrent.fValue * ((float)wCurrentWaveSpeed / DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
					}	

					if(gstMeasureDisplay.stCurrent.bSwitch == ON)
						displayMeasureCurrent(hBtnDisplayCurrent, gstMeasureDisplay.stCurrent.fValue);

					if(gstMeasureDisplay.wChaZhiMode == 0)			//����ֵ
						gstMeasureDisplay.stChaZhi.fValue = gstMeasureDisplay.stCurrent.fValue - gstMeasureDisplay.fBiaoChengZhi;
					else if(gstMeasureDisplay.wChaZhiMode == 1)		//���ֵ
						gstMeasureDisplay.stChaZhi.fValue = (gstMeasureDisplay.stCurrent.fValue - gstMeasureDisplay.fBiaoChengZhi) / gstMeasureDisplay.fBiaoChengZhi;
					if(gstMeasureDisplay.stChaZhi.bSwitch == ON)
						displayMeasureChaZhi(hBtnDisplayChaZhi, gstMeasureDisplay.wChaZhiMode, gstMeasureDisplay.stChaZhi.fValue);
					
					if(gstMeasureDisplay.stMax.fValue < gstMeasureDisplay.stCurrent.fValue)
					{
						gstMeasureDisplay.stMax.fValue = gstMeasureDisplay.stCurrent.fValue;
						if(gstMeasureDisplay.stMax.fValue < 0)
							gstMeasureDisplay.stMax.fValue = 0;
					}
					
					if(gstMeasureDisplay.stMax.bSwitch == ON)
						displayMeasureMax(hBtnDisplayMax, gstMeasureDisplay.stMax.fValue);

					if((gstMeasureDisplay.stMin.fValue > gstMeasureDisplay.stCurrent.fValue) &&(gstMeasureDisplay.stCurrent.fValue != 0))
					{
						gstMeasureDisplay.stMin.fValue = gstMeasureDisplay.stCurrent.fValue;
						if(gstMeasureDisplay.stMin.fValue < 0)
							gstMeasureDisplay.stMin.fValue = 0;
					}
					if(gstMeasureDisplay.stMin.bSwitch == ON)
						displayMeasureMin(hBtnDisplayMin, gstMeasureDisplay.stMin.fValue);
					
					if(gstMeasureDisplay.wMethod == 3)
					{
							gstMeasureDisplay.fTuCengHouDu = (fRange1 - (fRange2 - fRange1))	//����ײ��������ֵ-�ײ��ײ������ֵ
											*((float)gwTuCengSpeed/DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
							
							displayMeasureTuCeng(hBtnDisplayTuCeng, gstMeasureDisplay.fTuCengHouDu, gstMeasureDisplay.wMethod);
					}
				}
			//	displayMeasureAverage(hBtnDisplayAverage, gstMeasureDisplay.stAverage.fValue);
			}
			else
			{
				if(gstMeasureDisplay.wMethod == 0)
				{
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
				}
				else if(gstMeasureDisplay.wMethod == 2 || gstMeasureDisplay.wMethod == 3)
				{
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
					SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
				}
				SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
				SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
				SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
				SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
				hDrawMeasureThread = NULL;
				return 0;
			}
		}
		else
		{
			if(gstMeasureDisplay.wMethod == 0)
			{
				SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
			}
			else if(gstMeasureDisplay.wMethod == 2 || gstMeasureDisplay.wMethod == 3)
			{
				SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][0].bOpen, 1);
				SetOpenFlag(NULL, stOldGateInfos[CurrentChannelNo][1].bOpen, 2);
			}
			SetPeakGate(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fPeakGate, FALSE);
			SetPeakGate(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fPeakGate, FALSE);
			SetQiDian(NULL, 1, stOldGateInfos[CurrentChannelNo][0].fFrontGate, stOldGateInfos[CurrentChannelNo][0].fBackGate, FALSE);
			SetQiDian(NULL, 2, stOldGateInfos[CurrentChannelNo][1].fFrontGate, stOldGateInfos[CurrentChannelNo][1].fBackGate, FALSE);
			hDrawMeasureThread = NULL;
			return 0;
		}
		Sleep(20);
	}
	return 0;
}


/**************************
*ע���������ʾ����
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChDisplayChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = (WNDPROC)chDisplayChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChDisplayChild;

	return RegisterClass(&wc);
}

/**************************
*���ذ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnDisplaySwitchProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
				}			
				else if(wParam == VK_UP)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
					if(!hWndChMenu && !bFreezeUp)
					{
						ShowWindow(hWndChDisplay, SW_HIDE);


						hWndChMenu = CreateWindow(szChMenuChild, TEXT(""), WS_CHILD,
									 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);//�������˵�����
						ShowWindow(hWndChMenu,SW_SHOW);
						UpdateWindow(hWndChMenu);

						gCurrentWnd = hWndChMenu;

						// close other hotkey
						gbHotDB = FALSE;
					//	gDBIndex = 0;

						memset(gszTextPlus, 0, sizeof(gszTextPlus));
						swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

						memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
						swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);

						InitializeCriticalSection(&csHotRange);
						EnterCriticalSection(&csHotRange);
						gbHotRange = FALSE;
						LeaveCriticalSection(&csHotRange);
						DeleteCriticalSection(&csHotRange);
					}
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					gstMeasureDisplay.stCurrent.fValue = 0;
					fCurrentValue1 = 0;
					fCurrentValue2 = 0;
					fCurrentValue3 = 0;
					fCurrentValue4 = 0;
					fCurrentValue5 = 0;
					fCurrentValue6 = 0;
					SetOpenFlag(NULL, FALSE, 1);    //�ȹر�բ�� ����رտ���ʱ ���б�����һ��
					SetOpenFlag(NULL, FALSE, 2);	//�ȹر�բ�� ����رտ���ʱ ���б�����һ��
					Sleep(100);
				 	InitializeCriticalSection(&csMeasure);
					EnterCriticalSection(&csMeasure);
					
					gbMeasureSwitch = !gbMeasureSwitch;
					
					LeaveCriticalSection(&csMeasure);
					DeleteCriticalSection(&csMeasure);	

					Sleep(10);					
					displayMeasureSwitch(Wnd, gbMeasureSwitch);
					
					

				/*	if(gstMeasureDisplay.wMethod == 0)
					{
						chCurrent_gate_reg->wGate1Enable = (WORD)gbMeasureSwitch;
						GateInfos[CurrentChannelNo][0].bOpen = gbMeasureSwitch;
					}
					else if(gstMeasureDisplay.wMethod == 2)
					{
						chCurrent_gate_reg->wGate1Enable = (WORD)gbMeasureSwitch;
						chCurrent_gate_reg->wGate2Enable = (WORD)gbMeasureSwitch;
						GateInfos[CurrentChannelNo][0].bOpen = gbMeasureSwitch;
						GateInfos[CurrentChannelNo][1].bOpen = gbMeasureSwitch;
					}
				*/	
					if(gbMeasureSwitch == ON)
					{
						if((sys_reg->wLedAlarm & 0x0001) == 0x0001)
						{
							sys_reg->wLedAlarm &= 0xFFFE;
						}
						// ���������߳�
						if(hDrawMeasureThread == NULL)
						{
							// �����߳�							
							DrawMeasureThread_param.hWnd = hWndChDisplay;
							HDC hDC = GetDC(hWndMain);
							DrawMeasureThread_param.hDC = hDC;

							hDrawMeasureThread = ::CreateThread(NULL, 0, measureProc, (LPVOID)&DrawMeasureThread_param, 0, &dwDrawMeasureThreadID);
						}
					}					
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				
				else if(wParam == 'x' || wParam == 'X')
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
				else if(wParam == 'A')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'C')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDisplaySwitchProc,Wnd,Msg,wParam,lParam);
}


/**************************
*�������ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnDisplayAverageProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
				}	
				else if(wParam == VK_UP)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndChAverage = CreateWindow(szChAverageChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChDisplay,SW_HIDE);
				//	SetFocus(hWndChAverage);
					ShowWindow(hWndChAverage,SW_SHOW);
					UpdateWindow(hWndChAverage);

					gCurrentWnd = hWndChAverage;
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				
				else if(wParam == 'x' || wParam == 'X')
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
				else if(wParam == 'A')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'C')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDisplayAverageProc,Wnd,Msg,wParam,lParam);
}

#if 0
/**************************
*���ֵ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK BtnDisplayMaxProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					DestroyWindow(hWndChDisplay);
					hWndChDisplay = 0;
					// songchenguang add start 2010-4-26
					SetDisplayStateFlag(FALSE);
					// songchenguang add end 2010-4-26

					SetFocus(hWndMain);//�����ڻ�����뽹��
				}
			
				else if(wParam == VK_UP)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
				
				}
				else if(wParam == '\t')
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDisplayMaxProc,Wnd,Msg,wParam,lParam);
}

/**************************
*��Сֵ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnDisplayMinProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					DestroyWindow(hWndChDisplay);
					hWndChDisplay = 0;
					// songchenguang add start 2010-4-26
					SetDisplayStateFlag(FALSE);
					// songchenguang add end 2010-4-26

					SetFocus(hWndMain);//�����ڻ�����뽹��
				}
			
				else if(wParam == VK_UP)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDisplayMinProc,Wnd,Msg,wParam,lParam);
}

/**************************
*��ǰֵ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnDisplayCurrentProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					DestroyWindow(hWndChDisplay);
					hWndChDisplay = 0;
					// songchenguang add start 2010-4-26
					SetDisplayStateFlag(FALSE);
					// songchenguang add end 2010-4-26
					SetFocus(hWndMain);//�����ڻ�����뽹��
				}
			
				else if(wParam == VK_UP)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{					
				}
				else if(wParam == '\t')
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDisplayCurrentProc,Wnd,Msg,wParam,lParam);
}
/**************************
*��ֵ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnDisplayChaZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					DestroyWindow(hWndChDisplay);
					hWndChDisplay = 0;
					// songchenguang add start 2010-4-26
					SetDisplayStateFlag(FALSE);
					// songchenguang add end 2010-4-26
					SetFocus(hWndMain);//�����ڻ�����뽹��
				}
			
				else if(wParam == VK_UP)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam ==VK_DOWN)
				{
					while(chDisplayBtnList[i] != currentChDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChDisplayBtn = chDisplayBtnList[i];
					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentChDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{					
				}
				else if(wParam == '\t')
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnDisplayChaZhiProc,Wnd,Msg,wParam,lParam);
}
#endif
/**************************
*��������ʾ���ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK chDisplayChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			gstMeasureDisplay.wMethod = 2;
			gstMeasureDisplay.fBiaoChengZhi = 0.1;
			gstMeasureDisplay.stAverage.bSwitch = ON;
			gstMeasureDisplay.stMax.bSwitch = ON;
			gstMeasureDisplay.stMin.bSwitch = ON;
			gstMeasureDisplay.stCurrent.bSwitch = ON;
			gstMeasureDisplay.stChaZhi.bSwitch = ON;

			gwTuCengSpeed = tuCengTypeToWaveSpeed(gwTuCengType);
			
			hBtnDisplaySwitch = CreateWindow(_T("BUTTON"),_T("displaySwitch"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_SWITCH,hInst,NULL);

			hBtnDisplayAverage = CreateWindow(_T("BUTTON"),_T("displayAverage"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_AVERAGE,hInst,NULL);
		
			hBtnDisplayCurrent = CreateWindow(_T("BUTTON"),_T("displayCurrentValue"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_CURRENT,hInst,NULL);
			
			hBtnDisplayMax = CreateWindow(_T("BUTTON"),_T("displayMax"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_MAX,hInst,NULL);

			hBtnDisplayMin= CreateWindow(_T("BUTTON"),_T("displayMin"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_MIN,hInst,NULL);

			hBtnDisplayChaZhi = CreateWindow(_T("BUTTON"),_T("displayChaZhi"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_CHAZHI,hInst,NULL);
			
			hBtnDisplayTuCeng = CreateWindow(_T("BUTTON"),_T("displayTuCeng"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 6*BUTTON_7_EACH_HEIGHT,MENU_WIDTH,BUTTON_7_EACH_HEIGHT,hWnd,(HMENU)IDC_CHDISPLAY_TUCENG,hInst,NULL);

			chDisplayBtnList[0] = hBtnDisplaySwitch;
			chDisplayBtnList[1] = hBtnDisplayAverage;

			//��ť�ؼ����Զ��崦�����
			OldBtnDisplaySwitchProc = (WNDPROC) GetWindowLong(hBtnDisplaySwitch,GWL_WNDPROC);
		    SetWindowLong(hBtnDisplaySwitch,GWL_WNDPROC,(LONG)btnDisplaySwitchProc);
			OldBtnDisplayAverageProc = (WNDPROC) GetWindowLong(hBtnDisplayAverage,GWL_WNDPROC);
		    SetWindowLong(hBtnDisplayAverage,GWL_WNDPROC,(LONG)btnDisplayAverageProc);
		/*				
			OldBtnDisplayMaxProc = (WNDPROC) GetWindowLong(hBtnDisplayMax,GWL_WNDPROC);
			SetWindowLong(hBtnDisplayMax,GWL_WNDPROC,(LONG)BtnDisplayMaxProc);
			
			OldBtnDisplayMinProc = (WNDPROC) GetWindowLong(hBtnDisplayMin,GWL_WNDPROC);
			SetWindowLong(hBtnDisplayMin,GWL_WNDPROC,(LONG)btnDisplayMinProc);
			
			OldBtnDisplayCurrentProc = (WNDPROC) GetWindowLong(hBtnDisplayCurrent,GWL_WNDPROC);
			SetWindowLong(hBtnDisplayCurrent,GWL_WNDPROC,(LONG)btnDisplayCurrentProc);
			
			OldBtnDisplayChaZhiProc = (WNDPROC) GetWindowLong(hBtnDisplayChaZhi,GWL_WNDPROC);
			SetWindowLong(hBtnDisplayChaZhi,GWL_WNDPROC,(LONG)btnDisplayChaZhiProc);
			*/
			currentChDisplayBtn = chDisplayBtnList[0];//Ĭ�ϵ�ǰѡ�п��ذ�ť
			SetFocus(currentChDisplayBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentChDisplayBtn;

			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndChDisplay, &ps);
						
			if(gbMeasureSwitch == ON)
			{
				closeAllGate();
				// ���������߳�
				if(hDrawMeasureThread == NULL)
				{
					// �����߳�							
					DrawMeasureThread_param.hWnd = hWnd;
					HDC hDC = GetDC(hWndMain);
					DrawMeasureThread_param.hDC = hDC;

					hDrawMeasureThread = ::CreateThread(NULL, 0, measureProc, (LPVOID)&DrawMeasureThread_param, 0, &dwDrawMeasureThreadID);
				}
			}

			EndPaint(hWndChDisplay, &ps);
			break;
		case WM_KEYDOWN:
			break;
		case WM_DRAWITEM://�ػ���ť
			pdis=(LPDRAWITEMSTRUCT)lParam;
			chDisplay_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnDisplaySwitch)
			{
				displayMeasureSwitch(pdis->hwndItem, gbMeasureSwitch);
			}
			if(pdis->hwndItem == hBtnDisplayAverage)
			{
				displayMeasureAverage(pdis->hwndItem, gstMeasureDisplay.stAverage.fValue);
			}
			if(pdis->hwndItem == hBtnDisplayCurrent)
			{
				displayMeasureCurrent(pdis->hwndItem, gstMeasureDisplay.stCurrent.fValue);
			}
			if(pdis->hwndItem == hBtnDisplayMax)
			{
				displayMeasureMax(pdis->hwndItem, gstMeasureDisplay.stMax.fValue);
			}
			if(pdis->hwndItem == hBtnDisplayMin)
			{
				displayMeasureMin(pdis->hwndItem, gstMeasureDisplay.stMin.fValue);
			}			
			if(pdis->hwndItem == hBtnDisplayChaZhi)
			{
				displayMeasureChaZhi(pdis->hwndItem, gstMeasureDisplay.wChaZhiMode, gstMeasureDisplay.stChaZhi.fValue);
			}
			if(pdis->hwndItem == hBtnDisplayTuCeng)
			{
				displayMeasureTuCeng(pdis->hwndItem, gstMeasureDisplay.fTuCengHouDu, gstMeasureDisplay.wMethod);
			}

			if(gbMeasureSwitch == ON)
			{
				// ���������߳�
				if(hDrawMeasureThread == NULL)
				{
					// �����߳�							
					DrawMeasureThread_param.hWnd = hWnd;
					HDC hDC = GetDC(hWndMain);
					DrawMeasureThread_param.hDC = hDC;

					hDrawMeasureThread = ::CreateThread(NULL, 0, measureProc, (LPVOID)&DrawMeasureThread_param, 0, &dwDrawMeasureThreadID);
				}
			}
			break;
		case WM_COMMAND:
			break;
		case WM_CLOSE:
		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/**************************
*���ز�������ʾ���ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chDisplay_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_smBtnBitmap[]=
	{
		{IDC_CHDISPLAY_SWITCH,	IDB_BITMAP_KAIQIGUANBI6_NOSEL_R	 ,  IDB_BITMAP_KAIQIGUANBI6_SELECT_R		},
		{IDC_CHDISPLAY_AVERAGE	,IDB_BITMAP_PINGJUNZHI_NOSEL_R ,  IDB_BITMAP_PINGJUNZHI_SELECT_R	},
		{IDC_CHDISPLAY_CURRENT	,IDB_BITMAP_DANGQIANZHI_NOSEL_R	 ,  IDB_BITMAP_DANGQIANZHI_NOSEL_R		},
		{IDC_CHDISPLAY_MAX	,IDB_BITMAP_ZHUIDAZHI_NOSEL_R	 ,  IDB_BITMAP_ZHUIDAZHI_NOSEL_R		},
		{IDC_CHDISPLAY_MIN	,IDB_BITMAP_ZHUIXIAOZHI_NOSEL_R	 ,  IDB_BITMAP_ZHUIXIAOZHI_NOSEL_R		},
		{IDC_CHDISPLAY_CHAZHI	,IDB_BITMAP_CHAZHI_NOSEL_R ,  IDB_BITMAP_CHAZHI_NOSEL_R	},
		{IDC_CHDISPLAY_TUCENG	,IDB_BITMAP_TUCENGHOUDU_NOSEL_R ,  IDB_BITMAP_TUCENGHOUDU_NOSEL_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_smBtnBitmap);
	return ReturnVal;
}
