/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ����̽�˴���Դ�ļ�
 * Filename: fnHanFengTanShangWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnHanFengTanShangWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "fnPoKouSheZhiWin.h"
#include "gateWin.h"
#include "const.h"

HWND hWndHanFengTanShang = NULL;			//����̽�˴��ھ��
HWND hBtnHanFengTanShangPoKouSheZhi = NULL;		//�¿����ð�ť���
HWND hBtnHanFengTanShangGangBanHouDu = NULL;		//�ְ��Ȱ�ť���
HWND hBtnHanFengTanShangDistance = NULL;	//̽ͷ��������밴ť���
HWND hBtnHanFengTanShangRefractAngle = NULL;	//����ǰ�ť���
HWND hBtnHanFengTanShangOnOrOff = NULL;	//�����رհ�ť���

HWND hanFengTanShangBtnList[5] = {NULL, NULL, NULL, NULL, NULL,};	//����̽�˴��ڵİ�ť��
WNDPROC OldBtnHanFengTanShangPoKouSheZhiProc = NULL;//�¿����ð�ťĬ�ϴ������
WNDPROC OldBtnHanFengTanShangGangBanHouDuProc = NULL;		//�ְ��Ȱ�ťĬ�ϴ������
WNDPROC OldBtnHanFengTanShangDistanceProc = NULL;	//̽ͷ��������밴ťĬ�ϴ������
WNDPROC OldBtnHanFengTanShangRefractAngleProc = NULL;	//����ǰ�ťĬ�ϴ������
WNDPROC OldBtnHanFengTanShangOnOrOffProc = NULL;	//�����رհ�ťĬ�ϴ������

 
HWND currentHanFengTanShangBtn;					//����̽�˴��ڵ�ǰѡ�еİ�ť
BOOL bHanFengTanShangSwitch = OFF;				//����̽�˿���

HANDLE hDrawWaveTraceThread = NULL;				// ��������·���߳̾��
DRAWTHREAD_PARAM DrawWaveTraceThread_param;		// ��������·���̲߳���
//HANDLE hDrawWaveTraceEvent = NULL;				// ��������·���¼�
DWORD dwDrawWaveTraceThreadID = 0;				// ��������·���߳�ID

CRITICAL_SECTION csHanFengTanShang;

static const int buttonNum = 5;					//��ť�ؼ���

float fHanFengTanShangGangBanHouDu = 0; //�ְ���
float fHead2HanFengDistance = 0;;			//̽ͷ���������
static float fHanFengTanShangGangBanHouDuStep = 1;	//�ְ��Ȳ���
static float fHead2HanFengDistanceStep = 1;		//̽ͷ��������벽��
static WORD wHanFengTanShangGangBanHouDuStepIndex = 1;
static WORD wHead2HanFengDistanceStepIndex = 1;

extern HINSTANCE hInst;						//����ʵ�����

extern HWND hWndMain;
extern HWND hWndSpecialFunction;
extern HWND currentSpecialFunctionBtn;
extern HWND gCurrentFocusBtn;
extern HWND hWndPoKouSheZhi;
extern HWND currentPoKouSheZhiBtn;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD CurrentChannelNo;
extern float fZheSheAngleStep;
extern WORD nZheSheAngleStepIndex; 

extern WORD gwPoKouLeiXing;					//�¿�����
extern float gfPoKouJiaoDu;					//�¿ڽǶ�
extern float gfPoKouShenDu;					//�¿����

extern BOOL gbGuanLian;
extern WORD *chCurrent_gate1curve;
extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
extern ARS200FPGA_CH_GATE_REG *ch2_gate_reg;
extern WORD *ch1_gate1curve;
extern WORD *ch2_gate1curve;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern ARS200FPGA_CH_CON_REG *chCurrent_con_reg;
extern GATE_INFO GateInfos[2][3];

extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];		//DACͨ������
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;							//��ǰ��ʾ���Ӵ���

extern wchar_t gszTextStep[10];
extern BOOL gbHotRange;			// ��ݼ�����
extern BOOL gbHotDB;			// ��ݼ�����
extern HWND hStatusBar;			// ״̬�����
extern PEAK_INFO gPeakDefectInfos[2][2];
extern CRITICAL_SECTION csPeakDefectInfos;
extern ARS200FPGA_SYS_REG* sys_reg;
extern SYS_COLOR_INFO gstSysColor[SYS_COLOR_NUM];
extern int gCurrentSysColorIndex;
extern float gfPlateHeight;		//�������
extern CRITICAL_SECTION csProbeType;
extern WORD wTanTouLeiXing[CHANNEL_NUM];
extern WORD gwMaterialType;				//��������
extern BOOL gbAlarmRegFlag[CHANNEL_NUM][GATE_NUM];		//ͨ��������־
extern BOOL gbChannelAlarmFlag[CHANNEL_NUM];			//բ�ű�����־
extern CRITICAL_SECTION csAlarmFlag;

extern BOOL gbAlarmFlag;
extern WORD typeToWaveSpeed(WORD wType);
extern void SetWaveSpeedLabelAndExecute(HWND hWnd);


extern void SetAngleLabelAndExecute();
extern WORD waveSpeedToType(WORD wWaveSpeed);
/**************************
*ע�Ẹ��̽�˴���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerHanFengTanShangChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) hanFengTanShangChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szHanFengTanShangChild;

	return RegisterClass(&wc);
}
/**************************
*��ʾ�ְ���
* @param		hWnd[in]: ���ھ��
* @param		fHouDu[in]: �ְ���
* @return   LRESULT :
* @since    1.00
***************************/
void displayHanFengTanShangGangBanHouDu(HWND hWnd ,float fHouDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fHouDu);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}
void displayHanFengTanShangJiaoDu(HWND hWnd ,float fJiaoDu)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fJiaoDu);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}

/**************************
*��ʾ̽ͷ���¿ھ���
* @param		hWnd[in]: ���ھ��
* @param		fDistance[in]: ̽ͷ���¿ھ���
* @return   LRESULT :
* @since    1.00
***************************/
void displayHead2HanFengDistance(HWND hWnd ,float fDistance)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fDistance);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}
/**************************
*��ʾ̽ͷ�Ƕ�
* @param		hWnd[in]: ���ھ��
* @param		fAngle[in]: ̽ͷ�Ƕ�
* @return   LRESULT :
* @since    1.00
***************************/
void displayHanFengTanShangRefractAngle(HWND hWnd ,float fAngle)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fAngle);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}
/**************************
*��ʾ����
* @param		hWnd[in]: ���ھ��
* @param		bSwitch[in]: ����
* @return   LRESULT :
* @since    1.00
***************************/
void displayHanFengTanShangOnOrOff(HWND hWnd ,BOOL bSwitch)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(bSwitch == 0)
		wsprintf(strText, _T("�ر�"));
	else
		wsprintf(strText, _T("����"));
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

}
/**************************
*��ʾ����
* @param		hWnd[in]: ���ھ��
* @param		bSwitch[in]: ����
* @return   LRESULT :
* @since    1.00
***************************/
void drawPoKou(WORD wPoKouLeiXing, float fGangBanHouDu, float fPoKouShenDu, float fPoKouJiaoDu, BOOL bDrawFlag, HDC hdc)
{
	WORD wColor;
	DWORD dwPoKouScreenShenDu; //��Ļ�¿����
	DWORD dwScreenGangBanHouDu; //��Ļ�ְ��ȳ���
	float fGangBanLength;		//������Ļ��ȡ�ĸְ峤��
	float fPoKouLength;			//�¿ڳ���
	HDC hdcs = GetDC(hWndMain);
	RECT rect;
	GetClipBox(hdcs, &rect);

	POINT point1,point2,point3,point4,point5,point6;
	POINT pointLeft, pointRight;

	RECT RctWave;// ��������
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH; //- MENU_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	// ���������ȡ��߶�
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;

	memset(&point1, 0 ,sizeof(POINT));
	memset(&point2, 0 ,sizeof(POINT));
	memset(&point3, 0 ,sizeof(POINT));
	memset(&point4, 0 ,sizeof(POINT));
	memset(&point5, 0 ,sizeof(POINT));
	memset(&point6, 0 ,sizeof(POINT));
	memset(&pointLeft, 0 ,sizeof(POINT));
	memset(&pointRight, 0 ,sizeof(POINT));
	
	double fAngle = fPoKouJiaoDu/180.0f * PI;

	if(bDrawFlag == TRUE)
		wColor = RED_POINT;
	else
		wColor = BLACK_POINT;
	
	fGangBanLength = float(fHead2HanFengDistance + fPoKouShenDu / tan(fAngle) + fHead2HanFengDistance / 6);
	fPoKouLength =	float(2* (fPoKouShenDu / tan(fAngle)));

	dwScreenGangBanHouDu = DWORD((RctWave.right - RctWave.left) * (fGangBanHouDu / fGangBanLength));
	if(dwScreenGangBanHouDu > DWORD(nWaveHeight /2))
		dwScreenGangBanHouDu = nWaveHeight /2; //���ֻռ��������߶ȵ�һ��
	dwPoKouScreenShenDu = DWORD(dwScreenGangBanHouDu *(fPoKouShenDu / fGangBanHouDu));

	point1.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance - fPoKouLength / 2) / fGangBanLength);
	if(point1.x < RctWave.left)
		point1.x = RctWave.right;
	point1.y = DWORD(RctWave.top);

	point2.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance + fPoKouLength / 2) / fGangBanLength);
	if(point2.x > RctWave.right)
		point2.x = RctWave.right;
	point2.y = DWORD(RctWave.top);

	if(wPoKouLeiXing == 0)//V�Ϳ�
	{
		point3.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance / fGangBanLength));
		point3.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);
		drawLine(point1, point3, wColor);	//��V�Ϳ�����
		drawLine(point3, point2, wColor);	//��V�Ϳ��Ұ��
	}
	else if(wPoKouLeiXing == 1)//Y�Ϳ�
	{
		point3.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance / fGangBanLength));
		point3.y = DWORD(RctWave.top + dwPoKouScreenShenDu);

		point4.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance / fGangBanLength));
		point4.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

		drawLine(point1, point3, wColor);	//��Y�Ϳ�����
		drawLine(point3, point2, wColor);	//��Y�Ϳ��Ұ��
		drawLine(point3, point4, wColor);	//��Y�Ϳ�����
	}

	else if(wPoKouLeiXing == 2)//˫Y�Ϳ�
	{
		point3.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance / fGangBanLength));
		point3.y = DWORD(RctWave.top + dwPoKouScreenShenDu);
		
		point4.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance / fGangBanLength));
		point4.y = DWORD(RctWave.top + dwScreenGangBanHouDu - dwPoKouScreenShenDu);

		point5.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance - fPoKouLength / 2) / fGangBanLength);
		if(point5.x < RctWave.left)
			point5.x = RctWave.right;
		point5.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

		point6.x = DWORD(RctWave.left + (RctWave.right - RctWave.left) * (fHead2HanFengDistance + fPoKouLength / 2) / fGangBanLength);
		if(point6.x > RctWave.right)
			point6.x = RctWave.right;
		point6.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

		drawLine(point1, point3, wColor);	//��˫Y�Ϳ��ϰ벿������
		drawLine(point3, point2, wColor);	//��˫Y�Ϳ��ϰ벿���Ұ��
		drawLine(point3, point4, wColor);	//��˫Y�Ϳ�����
		drawLine(point5, point4, wColor);	//��˫Y�Ϳ��°벿������
		drawLine(point4, point6, wColor);	//��˫Y�Ϳ��°벿���Ұ��
	}
	else
	{
		;//
	}
	pointLeft.x = RctWave.left;
	pointLeft.y = DWORD(RctWave.top + dwScreenGangBanHouDu);

	pointRight.x = RctWave.right;
	pointRight.y = DWORD(RctWave.top + dwScreenGangBanHouDu);

	drawLine(pointLeft, pointRight, wColor);

	ReleaseDC(hWndMain, hdcs);
}
DWORD WINAPI DrawWaveTraceThreadProc(LPVOID lpParameter)
{
	DRAWTHREAD_PARAM* pParam = (DRAWTHREAD_PARAM*)lpParameter;
	HDC hdc = pParam->hDC;
	RECT rect;
	GetClipBox(hdc, &rect);

	RECT RctWave;// ��������
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH; //- MENU_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	// ���������ȡ��߶�
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;

	RECT RctHanFeng;// ��������
	RctHanFeng.left = RctWave.left;
	RctHanFeng.right = RctWave.right; //- MENU_WIDTH;
	RctHanFeng.top = RctWave.top;
	RctHanFeng.bottom = RctWave.bottom / 2;

	POINT point1;	//�����
	POINT point2;
	POINT point3;
	POINT point4;
	POINT point5;
	POINT point6;
	
	memset(&point1, 0 ,sizeof(POINT));
	memset(&point2, 0 ,sizeof(POINT));
	memset(&point3, 0 ,sizeof(POINT));
	memset(&point4, 0 ,sizeof(POINT));
	memset(&point5, 0 ,sizeof(POINT));
	memset(&point6, 0 ,sizeof(POINT));
	
	float fRange = 0;
	float fDistance = 0;	//�����ײ�������
	DWORD dwBottomPointScreenDistance = 0;	//�ײ���������Ļ����
	DWORD dwPerRangeDots = 0;	//��λ���̵����ص�
	DWORD dwScreenGangBanHouDu = 0; //��Ļ�ְ��ȳ���
	float fGangBanLength = 0;		//������Ļ��ȡ�ĸְ峤��
	float fPoKouLength = 0;			//�¿ڳ���
	WORD wZheXianNum = 0;			//�������� 
		
	double fAngle1 = DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI; //̽ͷ�Ƕ�
	double fAngle2 = gfPoKouJiaoDu/180.0f * PI;	//�¿ڽǶ�

	if(cos(fAngle1) > 0)
		fDistance = float(gfPlateHeight / cos(fAngle1));

	fPoKouLength =	float(2* (gfPoKouShenDu / tan(fAngle2)));
	fGangBanLength = float(fHead2HanFengDistance + gfPoKouShenDu / tan(fAngle2) + fHead2HanFengDistance / 6);
	dwScreenGangBanHouDu = DWORD((RctWave.right - RctWave.left) * (gfPlateHeight / fGangBanLength));
	if(dwScreenGangBanHouDu > DWORD(nWaveHeight /2))
		dwScreenGangBanHouDu = nWaveHeight /2; //���ֻռ��������߶ȵ�һ��
	dwPerRangeDots = DWORD((RctWave.right - RctWave.left) / fGangBanLength);
	
	dwBottomPointScreenDistance = DWORD(dwScreenGangBanHouDu * tan(fAngle1));

	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	HBRUSH hbr = CreateSolidBrush(wColorBack);
	FillRect(hdc, &RctHanFeng, hbr);
	DeleteObject(hbr);
	while(1)
	{
		if(wZheXianNum == 1)
		{
			drawLine(point1, point2, wColorBack);
			drawShiZhi(point2, BLUE_POINT, FALSE, RctWave);
		}
		else if(wZheXianNum == 2)
		{
			drawLine(point1, point2, wColorBack);
			drawLine(point2, point3, wColorBack);
			drawShiZhi(point3, BLUE_POINT, FALSE, RctWave);
		}
			
		else if(wZheXianNum == 3)
		{
			drawLine(point1, point2, wColorBack);
			drawLine(point2, point3, wColorBack);
			drawLine(point3, point4, wColorBack);
			drawShiZhi(point4, BLUE_POINT, FALSE, RctWave);
		}
		else if(wZheXianNum == 4)
		{
			drawLine(point1, point2, wColorBack);
			drawLine(point2, point3, wColorBack);
			drawLine(point3, point4, wColorBack);
			drawLine(point4, point5, wColorBack);
			drawShiZhi(point5, BLUE_POINT, FALSE, RctWave);
		}
		else if(wZheXianNum == 5)
		{
			drawLine(point1, point2, wColorBack);
			drawLine(point2, point3, wColorBack);
			drawLine(point3, point4, wColorBack);
			drawLine(point4, point5, wColorBack);
			drawLine(point5, point5, wColorBack);
			drawShiZhi(point6, BLUE_POINT, FALSE, RctWave);
		}
		else if(wZheXianNum > 5)
		{
			drawLine(point1, point2, wColorBack);
			drawLine(point2, point3, wColorBack);
			drawLine(point3, point4, wColorBack);
			drawLine(point4, point5, wColorBack);
			drawLine(point5, point6, wColorBack);
		}

		InitializeCriticalSection(&csHanFengTanShang);
		EnterCriticalSection(&csHanFengTanShang);
		BOOL bExit = bHanFengTanShangSwitch;
		LeaveCriticalSection(&csHanFengTanShang);
		DeleteCriticalSection(&csHanFengTanShang);

		if(bExit == OFF)
		{
			hDrawWaveTraceThread = NULL;
			return 0;
		}

		drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, TRUE, hdc);
		
		BOOL bAlarmRegFlag[CHANNEL_NUM][GATE_NUM];		//ͨ��������־
		BOOL bChannelAlarmFlag[CHANNEL_NUM];			//բ�ű�����־
		
		InitializeCriticalSection(&csAlarmFlag);
		EnterCriticalSection(&csAlarmFlag);
		for(int i = 0; i < CHANNEL_NUM; i++)
		{
			for(int j = 0; j < GATE_NUM; j++)
			{
				bAlarmRegFlag[i][j] = gbAlarmRegFlag[i][j];
			}
			bChannelAlarmFlag[i] = gbChannelAlarmFlag[i];
		}
		LeaveCriticalSection(&csAlarmFlag);
		DeleteCriticalSection(&csAlarmFlag);

		if((bChannelAlarmFlag[CurrentChannelNo]) && bAlarmRegFlag[CurrentChannelNo][0])
		{
			point1.x = (DWORD)(RctWave.left);
			point1.y = (DWORD)(RctWave.top);

		//	point2.x = (DWORD)(RctWave.left+ dwBottomPointScreenDistance);
		//	point2.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

			InitializeCriticalSection(&csPeakDefectInfos);
			EnterCriticalSection(&csPeakDefectInfos);
			fRange = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, int(gPeakDefectInfos[CurrentChannelNo][0].wDefectTime));
			LeaveCriticalSection(&csPeakDefectInfos);
			DeleteCriticalSection(&csPeakDefectInfos);

			wZheXianNum = (WORD)(fRange / fDistance + 1);

			if(wZheXianNum == 1)
			{
				point2.x = DWORD(RctWave.left + (fRange * sin(fAngle1) * dwPerRangeDots));
				point2.y = DWORD(RctWave.top + (fRange *cos(fAngle1) *dwPerRangeDots));

				drawLine(point1, point2, BLUE_POINT);
				drawShiZhi(point2, BLUE_POINT, TRUE, RctWave);
			}
			else if(wZheXianNum == 2)
			{
				point2.x = (DWORD)(RctWave.left+ dwBottomPointScreenDistance);
				point2.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

				point3.x = DWORD(RctWave.left + dwBottomPointScreenDistance + ((fRange - fDistance) *sin(fAngle1) * dwPerRangeDots));
				point3.y = DWORD(point2.y - ((fRange - fDistance) *cos(fAngle1) *dwPerRangeDots));

				drawLine(point1, point2, BLUE_POINT);
				drawLine(point2, point3, BLUE_POINT);
				drawShiZhi(point3, BLUE_POINT, TRUE, RctWave);
			}
				
			else if(wZheXianNum == 3)
			{
				point2.x = (DWORD)(RctWave.left+ dwBottomPointScreenDistance);
				point2.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

				point3.x = (DWORD)(point2.x + dwBottomPointScreenDistance);
				point3.y = (DWORD)RctWave.top;

				point4.x = DWORD(point3.x + ((fRange - 2*fDistance) *sin(fAngle1) * dwPerRangeDots));
				point4.y = DWORD(point3.y + (fRange - 2*fDistance) *cos(fAngle1) *dwPerRangeDots);

				drawLine(point1, point2, BLUE_POINT);
				drawLine(point2, point3, BLUE_POINT);
				drawLine(point3, point4, BLUE_POINT);
				drawShiZhi(point4, BLUE_POINT, TRUE, RctWave);
			}
			else if(wZheXianNum == 4)
			{
				point2.x = (DWORD)(RctWave.left+ dwBottomPointScreenDistance);
				point2.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

				point3.x = (DWORD)(point2.x + dwBottomPointScreenDistance);
				point3.y = (DWORD)RctWave.top;

				point4.x = DWORD(point3.x + dwBottomPointScreenDistance);
				point4.y = DWORD(RctWave.top + dwScreenGangBanHouDu);

				point5.x = DWORD(point4.x + ((fRange - 3*fDistance) *sin(fAngle1) * dwPerRangeDots));
				point5.y = DWORD(point4.y - ((fRange - 3*fDistance) *cos(fAngle1) *dwPerRangeDots));

				drawLine(point1, point2, BLUE_POINT);
				drawLine(point2, point3, BLUE_POINT);
				drawLine(point3, point4, BLUE_POINT);
				drawLine(point4, point5, BLUE_POINT);
				drawShiZhi(point5, BLUE_POINT, TRUE, RctWave);
			}
			else if(wZheXianNum == 5)
			{
				point2.x = (DWORD)(RctWave.left+ dwBottomPointScreenDistance);
				point2.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

				point3.x = (DWORD)(point2.x + dwBottomPointScreenDistance);
				point3.y = (DWORD)RctWave.top;

				point4.x = DWORD(point3.x + dwBottomPointScreenDistance);
				point4.y = DWORD(RctWave.top + dwScreenGangBanHouDu);

				point5.x = DWORD(point4.x + dwBottomPointScreenDistance);
				point5.y = DWORD(RctWave.top);

				point6.x = DWORD(point5.x + ((fRange - 4*fDistance) *sin(fAngle1) * dwPerRangeDots));
				point6.y = DWORD(point5.y + (fRange - 4*fDistance) *cos(fAngle1) *dwPerRangeDots);

				drawLine(point1, point2, BLUE_POINT);
				drawLine(point2, point3, BLUE_POINT);
				drawLine(point3, point4, BLUE_POINT);
				drawLine(point4, point5, BLUE_POINT);
				drawLine(point5, point6, BLUE_POINT);
				drawShiZhi(point6, BLUE_POINT, TRUE, RctWave);
			}
			else if(wZheXianNum > 5)
			{
				point2.x = (DWORD)(RctWave.left+ dwBottomPointScreenDistance);
				point2.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

				point3.x = (DWORD)(point2.x + dwBottomPointScreenDistance);
				point3.y = (DWORD)RctWave.top;

				point4.x = DWORD(point3.x + dwBottomPointScreenDistance);
				point4.y = DWORD(RctWave.top + dwScreenGangBanHouDu);

				point5.x = DWORD(point4.x + dwBottomPointScreenDistance);
				point5.y = DWORD(RctWave.top);

				point6.x = DWORD(point5.x + dwBottomPointScreenDistance);
				point6.y = (DWORD)(RctWave.top + dwScreenGangBanHouDu);

				drawLine(point1, point2, BLUE_POINT);
				drawLine(point2, point3, BLUE_POINT);
				drawLine(point3, point4, BLUE_POINT);
				drawLine(point4, point5, BLUE_POINT);
				drawLine(point5, point6, BLUE_POINT);
			}
			//������ȡһ������
		//	chCurrent_con_reg->wReset |= 0x1;
		//	chCurrent_con_reg->wReset &= 0xFFFE;
			
		}
		Sleep(10);
	}
	return 0;

}
void setHanFengTanShangOnOrOff(HWND hWnd ,BOOL bSwitch)
{
//	float fFront;
//	float fBack;
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(bSwitch == 0)
		wsprintf(strText, _T("�ر�"));
	else
		wsprintf(strText, _T("����"));
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	double fAngle = DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI;

    if(bSwitch == ON)
	{
	/*	if(cos(fAngle) != 0)
		{
			fFront = float(gfPlateHeight / cos(fAngle));
			fBack = float(2 * (gfPlateHeight / cos(fAngle)));
		}

		float fRange = fBack - fFront;
		WORD wDots = (WORD)Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fRange);

		if(!gbGuanLian)
		{
			chCurrent_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			chCurrent_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
			
			GateInfos[CurrentChannelNo][0].fFrontGate = fFront;
			GateInfos[CurrentChannelNo][0].fBackGate = fBack;

			//SetGateCurve(chCurrent_gate1curve, wDots, 50);
			chCurrent_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 50 / 100.0f);

			GateInfos[CurrentChannelNo][0].fPeakGate = 50;

			chCurrent_gate_reg->wGate1Alarm = 0;

			GateInfos[CurrentChannelNo][0].alarmMode = (alarm_modes)0;
		}
		else
		{
			ch1_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));
			ch2_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fFront));

			GateInfos[0][0].fFrontGate = fFront;
			GateInfos[0][0].fBackGate = fBack;

			ch1_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));
			ch2_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, fBack));

			GateInfos[1][0].fFrontGate = fFront;
			GateInfos[1][0].fBackGate = fBack;

			//SetGateCurve(ch1_gate1curve, wDots, 50);
			ch1_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 50 / 100.0f);
			//SetGateCurve(ch2_gate1curve, wDots, 50);
			ch2_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * 50 / 100.0f);

			GateInfos[0][0].fPeakGate = 50;
			GateInfos[1][0].fPeakGate = 50;

			ch1_gate_reg->wGate1Alarm = 0;
			ch2_gate_reg->wGate1Alarm = 0;

			GateInfos[0][0].alarmMode = (alarm_modes)0;
			GateInfos[1][0].alarmMode = (alarm_modes)0;
		}*/
		chCurrent_gate_reg->wGate1Enable = ON;
		GateInfos[CurrentChannelNo][0].bOpen = ON;
		// ������������·��ͼ�߳�
		if(hDrawWaveTraceThread == NULL)
		{
			// �����߳�							
			DrawWaveTraceThread_param.hWnd = hWnd;
			HDC hDC = GetDC(hWndMain);
			DrawWaveTraceThread_param.hDC = hDC;

			hDrawWaveTraceThread = ::CreateThread(NULL, 0, DrawWaveTraceThreadProc, (LPVOID)&DrawWaveTraceThread_param, 0, &dwDrawWaveTraceThreadID);
		//	hDrawWaveTraceEvent = CreateEvent(NULL, TRUE, TRUE, TEXT("DrawWaveTraceEvent0"));
		//	ReleaseDC(hWnd, hDC);//���ﲻ��RELEASE
		}
	//	if(hDrawWaveTraceThread != NULL)
	//		SetEvent(hDrawWaveTraceEvent);

	//	drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, TRUE);
	//	drawWaveTrace(gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, TRUE);
	}
/*	if(bSwitch == OFF)
	{
	//	chCurrent_gate_reg->wGate1Enable = OFF;		//Delete at2010-12-22 wangbingfu
	//	GateInfos[CurrentChannelNo][0].bOpen = OFF; //Delete at2010-12-22 wangbingfu
	//	drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);
	//	drawWaveTrace(gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);
	}*/
	
}
/**************************
*����̽�˴��ڴ������
* @param		Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK hanFengTanShangChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			gwMaterialType = waveSpeedToType(DisplaySet_ad[CurrentChannelNo].nWaveSpeed); //���룬��Ȼ������ǻ����
			if(gwPoKouLeiXing == 0)
			{	
				double fAngle = gfPoKouJiaoDu/180.0f * PI;
				gfPoKouShenDu = gfPlateHeight;
				fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
			}
			else if(gwPoKouLeiXing == 1)
			{
				if(gfPoKouShenDu >  gfPlateHeight)
				{
					double fAngle = gfPoKouJiaoDu/180.0f * PI;
					gfPoKouShenDu = gfPlateHeight - 1;
					fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
					
				}
			}
			else
			{
				if(2 * gfPoKouShenDu >  gfPlateHeight)
				{
					double fAngle = gfPoKouJiaoDu/180.0f * PI;
					gfPoKouShenDu = gfPlateHeight / 2 - 1;
					fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
					
				}
			}
	//		closeAllGate();//�ر�����բ��
			//�ص��񶯺͵���
  			if((sys_reg->wLedAlarm & 0x1) == 0x1)
			{
				sys_reg->wLedAlarm &= 0xFFFE;
			}
			//�ر�DAC
		/*	for(int i =0; i < GATE_NUM; i++)
			{
				gDgsSwitch[CurrentChannelNo][i] = 0;
			}*/

			fHanFengTanShangGangBanHouDuStep = AMENDSTEPS[wHanFengTanShangGangBanHouDuStepIndex];
			fHead2HanFengDistanceStep = AMENDSTEPS[wHead2HanFengDistanceStepIndex];
			fZheSheAngleStep = ANGLESTEPS[nZheSheAngleStepIndex];
			//����ť�ؼ�
			hBtnHanFengTanShangPoKouSheZhi = CreateWindow(_T("BUTTON"),_T("PoKouSheZhiHanFengTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_HANFENGTANSHANG_POKOUSHEZHI,hInst,NULL);
			hBtnHanFengTanShangGangBanHouDu = CreateWindow(_T("BUTTON"),_T("gangBanHouDuHanFengTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_HANFENGTANSHANG_GANGBANHOUDU,hInst,NULL);
			hBtnHanFengTanShangDistance = CreateWindow(_T("BUTTON"),_T("distanceHanFengTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_HANFENGTANSHANG_DISTANCE,hInst,NULL);
			hBtnHanFengTanShangRefractAngle = CreateWindow(_T("BUTTON"),_T("refractAngleHanFengTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_HANFENGTANSHANG_REFRACTANGLE,hInst,NULL);
			hBtnHanFengTanShangOnOrOff = CreateWindow(_T("BUTTON"),_T("OnOrOffHanFengTanShang"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_HANFENGTANSHANG_ONOROFF,hInst,NULL);			
			
			hanFengTanShangBtnList[0] = hBtnHanFengTanShangPoKouSheZhi;
			hanFengTanShangBtnList[1] = hBtnHanFengTanShangGangBanHouDu;
			hanFengTanShangBtnList[2] = hBtnHanFengTanShangDistance;
			hanFengTanShangBtnList[3] = hBtnHanFengTanShangRefractAngle;
			hanFengTanShangBtnList[4] = hBtnHanFengTanShangOnOrOff;
			
			
			//��ť�ؼ����Զ��崦�����
			OldBtnHanFengTanShangPoKouSheZhiProc = (WNDPROC) GetWindowLong(hBtnHanFengTanShangPoKouSheZhi,GWL_WNDPROC);
			SetWindowLong(hBtnHanFengTanShangPoKouSheZhi,GWL_WNDPROC,(LONG)btnHanFengTanShangPoKouSheZhiProc);
			
			OldBtnHanFengTanShangGangBanHouDuProc = (WNDPROC) GetWindowLong(hBtnHanFengTanShangGangBanHouDu,GWL_WNDPROC);
			SetWindowLong(hBtnHanFengTanShangGangBanHouDu,GWL_WNDPROC,(LONG)btnHanFengTanShangGangBanHouDuProc);
			
			OldBtnHanFengTanShangDistanceProc = (WNDPROC) GetWindowLong(hBtnHanFengTanShangDistance,GWL_WNDPROC);
			SetWindowLong(hBtnHanFengTanShangDistance,GWL_WNDPROC,(LONG)btnHanFengTanShangDistanceProc);
			
			OldBtnHanFengTanShangRefractAngleProc = (WNDPROC) GetWindowLong(hBtnHanFengTanShangRefractAngle,GWL_WNDPROC);
			SetWindowLong(hBtnHanFengTanShangRefractAngle,GWL_WNDPROC,(LONG)btnHanFengTanShangRefractAngleProc);

			OldBtnHanFengTanShangOnOrOffProc = (WNDPROC) GetWindowLong(hBtnHanFengTanShangOnOrOff,GWL_WNDPROC);
			SetWindowLong(hBtnHanFengTanShangOnOrOff,GWL_WNDPROC,(LONG)btnHanFengTanShangOnOrOffProc);
		
			currentHanFengTanShangBtn = hanFengTanShangBtnList[0];//Ĭ�ϵ�ǰѡ���¿����ð�ť
			SetFocus(currentHanFengTanShangBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentHanFengTanShangBtn;
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
			hanFengTanShang_DrawButtonBitmap(hInst,pdis);
			
			if(pdis->hwndItem == hBtnHanFengTanShangGangBanHouDu)
			{
				displayHanFengTanShangGangBanHouDu(pdis->hwndItem, gfPlateHeight);
			}
			else if(pdis->hwndItem == hBtnHanFengTanShangDistance)
			{
				displayHead2HanFengDistance(pdis->hwndItem, fHead2HanFengDistance);
			}
			else if(pdis->hwndItem == hBtnHanFengTanShangRefractAngle)
			{
				displayHanFengTanShangRefractAngle(pdis->hwndItem, DisplaySet_ad[CurrentChannelNo].fZheSheJiao);
			}
			else if(pdis->hwndItem == hBtnHanFengTanShangOnOrOff)
			{
				displayHanFengTanShangOnOrOff(pdis->hwndItem, bHanFengTanShangSwitch);
			}
			else
			{
				;//
			}

			// ��ⰴť�������ʾ��Ӧ�Ĳ���
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentHanFengTanShangBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnHanFengTanShangGangBanHouDu)
						swprintf(gszTextStep, _T("%.1f"), fHanFengTanShangGangBanHouDuStep);
					else if(pdis->hwndItem == hBtnHanFengTanShangDistance)
						swprintf(gszTextStep, _T("%.1f"), fHead2HanFengDistanceStep);
					else if(pdis->hwndItem == hBtnHanFengTanShangRefractAngle)
						swprintf(gszTextStep, _T("%.1f"), fZheSheAngleStep);
					else
						swprintf(gszTextStep, _T(""));

					PostMessage(hStatusBar, SB_SETTEXT, 
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
*���غ���̽�˴��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int hanFengTanShang_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_hanFengTanShangBtnBitmap[]=
	{
		{IDC_HANFENGTANSHANG_POKOUSHEZHI	,IDB_BITMAP_POKOUSHEZHI_NOSEL_R	 ,  IDB_BITMAP_POKOUSHEZHI_SELECT_R		},
		{IDC_HANFENGTANSHANG_GANGBANHOUDU	,IDB_BITMAP_GANGBANHOUDU5_NOSEL_R	 ,  IDB_BITMAP_GANGBANHOUDU5_SELECT_R		},
		{IDC_HANFENGTANSHANG_DISTANCE	,IDB_BITMAP_TANPOJU_NOSEL_R ,  IDB_BITMAP_TANPOJU_SELECT_R	},
		{IDC_HANFENGTANSHANG_REFRACTANGLE	,IDB_BITMAP_ZHESHEJIAO_NOSEL_R ,  IDB_BITMAP_ZHESHEJIAO_SELECT_R	},
		{IDC_HANFENGTANSHANG_ONOROFF	,IDB_BITMAP_KAIQIGUANBI5_NOSEL_R ,  IDB_BITMAP_KAIQIGUANBI5_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_hanFengTanShangBtnBitmap);
	return ReturnVal;
}

/**************************
*�¿����ð�ť�Զ��崦�����
* @param		Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHanFengTanShangPoKouSheZhiProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndHanFengTanShang);
					hWndHanFengTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(bHanFengTanShangSwitch == ON)
					{
						chCurrent_gate_reg->wGate1Enable = OFF;
						GateInfos[CurrentChannelNo][0].bOpen = OFF;
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);
						
						//drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);	//����¿�ͼ
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndPoKouSheZhi = CreateWindow(szPoKouSheZhiChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndHanFengTanShang,SW_HIDE);
				//	SetFocus(hWndPoKouSheZhi);
					ShowWindow(hWndPoKouSheZhi,SW_SHOW);
					UpdateWindow(hWndPoKouSheZhi);

					gCurrentWnd = hWndPoKouSheZhi;
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
	return CallWindowProc(OldBtnHanFengTanShangPoKouSheZhiProc,hWnd,Msg,wParam,lParam);
}
/**************************
*�ְ��Ȱ�ť�Զ��崦�����
* @param		hWnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHanFengTanShangGangBanHouDuProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndHanFengTanShang);
					hWndHanFengTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(bHanFengTanShangSwitch == ON)
					{
						chCurrent_gate_reg->wGate1Enable = OFF;
						GateInfos[CurrentChannelNo][0].bOpen = OFF;
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);

						//drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);	//����¿�ͼ
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if((gfPlateHeight -  fHanFengTanShangGangBanHouDuStep) > MIN_GONGJIANHOUDU)
						{
								gfPlateHeight -= fHanFengTanShangGangBanHouDuStep;
						}
						else
						{
							gfPlateHeight = MIN_GONGJIANHOUDU;
						}
						displayHanFengTanShangGangBanHouDu(hWnd, gfPlateHeight);
						
						if(gwPoKouLeiXing == 0)
						{	
							double fAngle = gfPoKouJiaoDu/180.0f * PI;
							gfPoKouShenDu = gfPlateHeight;
							fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
							displayHead2HanFengDistance(hBtnHanFengTanShangDistance,fHead2HanFengDistance);
						}
						else if(gwPoKouLeiXing == 1)
						{
							if(gfPoKouShenDu >  gfPlateHeight)
							{
								double fAngle = gfPoKouJiaoDu/180.0f * PI;
								gfPoKouShenDu = gfPlateHeight - 1;
								fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
								displayHead2HanFengDistance(hBtnHanFengTanShangDistance,fHead2HanFengDistance);
								
							}
						}
						else
						{
							if(2 * gfPoKouShenDu >  gfPlateHeight)
							{
								double fAngle = gfPoKouJiaoDu/180.0f * PI;
								gfPoKouShenDu = gfPlateHeight / 2 - 1;
								fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
								displayHead2HanFengDistance(hBtnHanFengTanShangDistance,fHead2HanFengDistance);
								
							}
						}
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if((gfPlateHeight + fHanFengTanShangGangBanHouDuStep) <= MAX_fPlateHeight)
							gfPlateHeight += fHanFengTanShangGangBanHouDuStep;
						else
							gfPlateHeight = MAX_fPlateHeight;

						if(gwPoKouLeiXing == 0) //V�Ϳ�
						{
							double fAngle = gfPoKouJiaoDu/180.0f * PI;
							gfPoKouShenDu = gfPlateHeight;
							fHead2HanFengDistance = float(gfPoKouShenDu / tan(fAngle));
							displayHead2HanFengDistance(hBtnHanFengTanShangDistance,fHead2HanFengDistance);
						}
					
						displayHanFengTanShangGangBanHouDu(hWnd, gfPlateHeight);
					}
				}
				else if(wParam == 'F')
				{
					if(wHanFengTanShangGangBanHouDuStepIndex < 3)
						wHanFengTanShangGangBanHouDuStepIndex++;
					else
						wHanFengTanShangGangBanHouDuStepIndex = 0;

					fHanFengTanShangGangBanHouDuStep = AMENDSTEPS[wHanFengTanShangGangBanHouDuStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fHanFengTanShangGangBanHouDuStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnHanFengTanShangGangBanHouDuProc,hWnd,Msg,wParam,lParam);
}
/**************************
*̽ͷ��������밴ť�Զ��崦�����
* @param		hWnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHanFengTanShangDistanceProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndHanFengTanShang);
					hWndHanFengTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(bHanFengTanShangSwitch == ON)
					{
						chCurrent_gate_reg->wGate1Enable = OFF;
						GateInfos[CurrentChannelNo][0].bOpen = OFF;
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);

						//drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);	//����¿�ͼ
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						double fAngle = gfPoKouJiaoDu/180.0f * PI;
						if((fHead2HanFengDistance -  fHead2HanFengDistanceStep) >= float(gfPoKouShenDu / tan(fAngle)))
							fHead2HanFengDistance -= fHead2HanFengDistanceStep;
						displayHead2HanFengDistance(hWnd, fHead2HanFengDistance);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						fHead2HanFengDistance += fHead2HanFengDistanceStep;
						displayHead2HanFengDistance(hWnd, fHead2HanFengDistance);
					}
				}
				else if(wParam == 'F')
				{
					if(wHead2HanFengDistanceStepIndex < 3)
						wHead2HanFengDistanceStepIndex++;
					else
						wHead2HanFengDistanceStepIndex = 0;

					fHead2HanFengDistanceStep = AMENDSTEPS[wHead2HanFengDistanceStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fHead2HanFengDistanceStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnHanFengTanShangDistanceProc,hWnd,Msg,wParam,lParam);
}
/**************************
*����ǰ�ť�Զ��崦�����
* @param		hWnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHanFengTanShangRefractAngleProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndHanFengTanShang);
					hWndHanFengTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(bHanFengTanShangSwitch == ON)
					{
						chCurrent_gate_reg->wGate1Enable = OFF;
						GateInfos[CurrentChannelNo][0].bOpen = OFF;
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);

						//drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);	//����¿�ͼ
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if(DisplaySet_ad[CurrentChannelNo].fZheSheJiao - fZheSheAngleStep > MIN_ANGLE)
						{
							DisplaySet_ad[CurrentChannelNo].fZheSheJiao -= fZheSheAngleStep;
						}
						else
						{
							DisplaySet_ad[CurrentChannelNo].fZheSheJiao = MIN_ANGLE;

						}						
						if ( fabs(DisplaySet_ad[CurrentChannelNo].fZheSheJiao) < 1E-6 )
						{
							InitializeCriticalSection(&csProbeType);
							EnterCriticalSection(&csProbeType);

							wTanTouLeiXing[CurrentChannelNo] = 1;//ֱ̽ͷ
							InitializeCriticalSection(&csProbeType);
							EnterCriticalSection(&csProbeType);

							DisplaySet_ad[CurrentChannelNo].nWaveSpeed = typeToWaveSpeed(gwMaterialType);
							SetWaveSpeedLabelAndExecute(NULL);
						}
						
						displayHanFengTanShangJiaoDu(hWnd, DisplaySet_ad[CurrentChannelNo].fZheSheJiao);

						SetAngleLabelAndExecute();
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(bHanFengTanShangSwitch == OFF)
					{
						if(DisplaySet_ad[CurrentChannelNo].fZheSheJiao + fZheSheAngleStep < MAX_ANGLE)
						{
							DisplaySet_ad[CurrentChannelNo].fZheSheJiao += fZheSheAngleStep;

							InitializeCriticalSection(&csProbeType);
							EnterCriticalSection(&csProbeType);

							wTanTouLeiXing[CurrentChannelNo] = 0;//б̽ͷ
							InitializeCriticalSection(&csProbeType);
							EnterCriticalSection(&csProbeType);

							DisplaySet_ad[CurrentChannelNo].nWaveSpeed = typeToWaveSpeed(gwMaterialType);
							SetWaveSpeedLabelAndExecute(NULL);
						}
						else
							DisplaySet_ad[CurrentChannelNo].fZheSheJiao = MAX_ANGLE;

						displayHanFengTanShangJiaoDu(hWnd, DisplaySet_ad[CurrentChannelNo].fZheSheJiao);

						SetAngleLabelAndExecute();						
					}
				}
				else if(wParam == 'F')
				{
					if(nZheSheAngleStepIndex < 1)
						nZheSheAngleStepIndex++;
					else
						nZheSheAngleStepIndex = 0;

					fZheSheAngleStep = ANGLESTEPS[nZheSheAngleStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fZheSheAngleStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnHanFengTanShangRefractAngleProc,hWnd,Msg,wParam,lParam);
}
/**************************
*�����رհ�ť�Զ��崦�����
* @hanFengTanShang    hInstance[in]:
* @hanFengTanShang		message[in]:
* @hanFengTanShang		wParam[in]:
* @hanFengTanShang		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHanFengTanShangOnOrOffProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					SetFocus(currentSpecialFunctionBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndHanFengTanShang);
					hWndHanFengTanShang = 0;
					
					gCurrentFocusBtn = currentSpecialFunctionBtn;

					gCurrentWnd = hWndSpecialFunction;

					if(bHanFengTanShangSwitch == ON)
					{
						chCurrent_gate_reg->wGate1Enable = OFF;
						GateInfos[CurrentChannelNo][0].bOpen = OFF;
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);

						//drawPoKou(gwPoKouLeiXing, gfPlateHeight, gfPoKouShenDu, gfPoKouJiaoDu, FALSE);	//����¿�ͼ
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hanFengTanShangBtnList[i] != currentHanFengTanShangBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentHanFengTanShangBtn = hanFengTanShangBtnList[i];
					SetFocus(currentHanFengTanShangBtn);
					gCurrentFocusBtn = currentHanFengTanShangBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(bHanFengTanShangSwitch == ON)
					{
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);
						setHanFengTanShangOnOrOff(hWnd, bHanFengTanShangSwitch);
					}
					else if(bHanFengTanShangSwitch == OFF && gfPlateHeight != 0)
					{
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = ON;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);
						Sleep(150);
						setHanFengTanShangOnOrOff(hWnd, bHanFengTanShangSwitch);
					}
				}
				else if(wParam == VK_RIGHT)
				{
					if(bHanFengTanShangSwitch == ON)
					{
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = OFF;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);
						setHanFengTanShangOnOrOff(hWnd, bHanFengTanShangSwitch);
					}
					else if(bHanFengTanShangSwitch == OFF && gfPlateHeight != 0)
					{
						InitializeCriticalSection(&csHanFengTanShang);
						EnterCriticalSection(&csHanFengTanShang);
						bHanFengTanShangSwitch = ON;
						SetRedrawFlag(TRUE);
						LeaveCriticalSection(&csHanFengTanShang);
						DeleteCriticalSection(&csHanFengTanShang);
						Sleep(150);
						setHanFengTanShangOnOrOff(hWnd, bHanFengTanShangSwitch);
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
	return CallWindowProc(OldBtnHanFengTanShangOnOrOffProc,hWnd,Msg,wParam,lParam);
}
