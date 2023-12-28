/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ��·ר�ò˵�����Դ�ļ�
 * Filename: tlMenuWin.cpp
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
#include "displayWin.h"
#include "recvParamWin.h"
#include "craftParamWin.h"

HWND hWndTlMenu = NULL;//��·ר�ò˵����ھ��
HWND currentTlMenuBtn = NULL;// ��·ר�ò˵����ڵ�ǰѡ�еİ�ť

WORD gwStandard[CHANNEL_NUM] = {0, 0};	//����׼
WORD gwPosition[CHANNEL_NUM][TL_STANDARD_NUM] = {{0, 0, 0, 0, 0},{0, 0, 0 ,0, 0}};	//��ⲿλ
BOOL gbTlSheDingFlag[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//����׼����λ�Ƿ����趨��־
float gfTlRange[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];			//����׼����λ����
float gfTlPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];				//����׼����λ����
float gfTlBuChangPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//����׼����λ��������

float gfTsOldRange[CHANNEL_NUM] = {0.0f, 0.0f};		//����ͨ��̽�˵�����
float gfTsOldPlus[CHANNEL_NUM] = {0.0f, 0.0f};		//����ͨ��̽�˵Ļ�׼����
float gfTsOldRefPlus[CHANNEL_NUM] = {0.0f, 0.0f};	//����ͨ��̽�˵Ĳο�����

BOOL gbTlParamFlag = TRUE;	//��ǰ�Ƿ�ʹ����·����	//wangbingfu add at 2011-5-25

static HWND hBtnTlMenuStandard = NULL;	//��׼��ť���
static HWND hBtnTlMenuPosition = NULL;	//��ⲿλ��ť���
static HWND hBtnTlMenuAdjust = NULL;	//��У��ť���
// songchenguang add start 2011-02-09
static HWND hBtnTlMenuCommonTest = NULL;	//ͨ��̽�˰�ť���
// songchenguang add end 2011-02-09

static HWND tlMenuBtnList[4] = {NULL, NULL, NULL, NULL};//��·ר�ò˵����ڵİ�ť��
static const int buttonNum = 4;//��ť�ؼ���

static WNDPROC OldBtnTlMenuStandardProc = NULL;//��׼��ťĬ�ϴ������
static WNDPROC OldBtnTlMenuPositionProc = NULL;//��ⲿλ��ťĬ�ϴ������
static WNDPROC OldBtnTlMenuAdjustProc = NULL;// ��У��ťĬ�ϴ������
// songchenguang add start 2011-02-09
static WNDPROC OldBtnTlMenuCommonTestProc = NULL;	//ͨ��̽�˰�ťĬ�ϴ������
extern HWND hWndTsMenu;			//̽�˲˵����ھ��
// songchenguang add end 2011-02-09

ATOM registerTlMenuChild(HINSTANCE hInstance);
static LRESULT CALLBACK tlMenuChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int tlMenu_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnTlMenuStandardProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnTlMenuPositionProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnTlMenuAdjustProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
// songchenguang add start 2011-02-09
static LRESULT CALLBACK btnTlMenuCommonTestProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
// songchenguang add end 2011-02-09


extern HINSTANCE hInst;			//����ʵ�����
extern HWND hWndMain;			//�����ھ��
extern HWND gCurrentFocusBtn;
extern HWND gCurrentWnd;
extern HWND hWndTlAdjust;		//��У���ھ��
extern float gfTlGainBuChang[CHANNEL_NUM];	//���油��
extern WORD CurrentChannelNo;				// ��ǰͨ����
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern wchar_t gszTextPeak[20];
extern HWND hStatusBar;					//״̬�����
extern WORD gwDeviceTypeValue;	// wangbingfu add at 2010-05-30

void initRailRoadParam()
{
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		for(int j = 0; j < TL_STANDARD_NUM; j++)
		{
			for(int k = 0; k < TL_POSITION_NUM; k++)
			{
				gbTlSheDingFlag[i][j][k] = FALSE;
			}
			for(int k = 0; k < TL_POSITION_NUM; k++)
			{
				gfTlRange[i][j][k] = 50.0f;
			}
			for(int k = 0; k < TL_POSITION_NUM; k++)
			{
				gfTlPlus[i][j][k] = 30.0f;
			}
			for(int k = 0; k < TL_POSITION_NUM; k++)
			{
				gfTlBuChangPlus[i][j][k] = 0.0f;
			}
		}
	}
}
/*
void TlSetPlusLableAndExecute(const float& fBasePlus, const float& fRefPlus, const WORD& wChNo )
{
	// ��ֵ
	if(!gbGuanLian)
	{
		ARS200FPGA_CH_PLUS_REG* ch_plus_reg = NULL;

		if(wChNo == 0)
		{
			ch_plus_reg = ch1_plus_reg;
		}
		else if(wChNo == 1)
		{
			ch_plus_reg = ch2_plus_reg;
		}
		else
			return;

		WORD wTemp0 = 0;
		WORD wTemp1 = 0;
		WORD wTemp2 = 0;
		WORD wTemp3 = 0;

		float fTotalPlus = fBasePlus + fRefPlus;
		dBProcess(fTotalPlus, wTemp0, wTemp1, wTemp2, wTemp3);

		ch_plus_reg->wdBRef = wTemp0;
		PlusChangeTurn(wChNo);	

		ch_plus_reg->wdBLevel1 = wTemp1;
		PlusChangeTurn(wChNo);

		ch_plus_reg->wdBLevel2 = wTemp2;
		PlusChangeTurn(wChNo);

		ch_plus_reg->wdBLevel3 = wTemp3;
		PlusChangeTurn(wChNo);

		if(wChNo == 0)
			gCh1Plus = fTotalPlus;
		else if(wChNo == 1)
			gCh2Plus = fTotalPlus;
	}
	else
	{
		DB_ITEM dB1;
		memset(&dB1, 0, sizeof(DB_ITEM));
		DB_ITEM dB2;
		memset(&dB2, 0, sizeof(DB_ITEM));

		GuanLianDBProcess(fBasePlus,
						  fRefPlus,
						  dB1,
						  dB2);

		ch1_plus_reg->wdBRef = dB1.wDB_A;
		PlusChangeTurn(0);	

		ch1_plus_reg->wdBLevel1 = dB1.wDB_B;
		PlusChangeTurn(0);

		ch1_plus_reg->wdBLevel2 = dB1.wDB_C;
		PlusChangeTurn(0);

		ch1_plus_reg->wdBLevel3 = dB1.wDB_D;
		PlusChangeTurn(0);

		ch2_plus_reg->wdBRef = dB2.wDB_A;
		PlusChangeTurn(1);	

		ch2_plus_reg->wdBLevel1 = dB2.wDB_B;
		PlusChangeTurn(1);

		ch2_plus_reg->wdBLevel2 = dB2.wDB_C;
		PlusChangeTurn(1);

		ch2_plus_reg->wdBLevel3 = dB2.wDB_D;
		PlusChangeTurn(1);

	}

	memset(gszTextPlus, 0, sizeof(gszTextPlus));
	swprintf(gszTextPlus, _T("%.1f"), fBasePlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

	memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
	swprintf(gszTextRefPlus, _T("%.1f"), fRefPlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
}
*/
/**************************
*���øñ�׼�ò�λ���̣���׼���棬��������
* @since    1.00
***************************/
// wangbingfu delete at 2011-05-27
/*
void setRailRoadParam(WORD wChannelNo, BOOL bCurveChangeFlag)
{
	//����ͨ��̽�˲������е�ͨ��̽��ʱ����
	//gfTsOldRange = DisplaySet_ad[wChannelNo].fRange;
	//gfTsOldPlus = ReceiveParam_ad[wChannelNo].fPlus;
	//gfTsOldRefPlus = ReceiveParam_ad[wChannelNo].fRefPlus;
	float fOldGain = ReceiveParam_ad[wChannelNo].fPlus;
	float fOldRange = DisplaySet_ad[wChannelNo].fRange;
	float fOldRefGain = ReceiveParam_ad[wChannelNo].fRefPlus;
	
	DisplaySet_ad[wChannelNo].fRange = gfTlRange[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]];
	ReceiveParam_ad[wChannelNo].fPlus = gfTlPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]];
	ReceiveParam_ad[wChannelNo].fRefPlus = gfTlBuChangPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]];
	
	SetRangeLableAndExecute(NULL, gfTlRange[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]], FALSE);
	SetPlusLableAndExecute(NULL, gfTlPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]], 
						gfTlBuChangPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]],FALSE, wChannelNo);
	SetRefPlusLableAndExecute(NULL, gfTlPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]], 
						gfTlBuChangPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]],FALSE, wChannelNo);
	if(bCurveChangeFlag)
	{
		gainChangeCurve(fOldGain, FALSE);//����������ı�
		rangeChangeCurve(fOldRange, FALSE);//���������̸ı�
		refGainChangeCurve(fOldRefGain, FALSE);//wangbingfu add at 2011-03-09

		//����ԭ������
		for(int i = 0; i < CHANNEL_NUM; i++)
		{
			DisplaySet_ad[i].fRange = gfTsOldRange[i];
			ReceiveParam_ad[i].fPlus = gfTsOldPlus[i];
			ReceiveParam_ad[i].fRefPlus = gfTsOldRefPlus[i];
		}
		SaveParamFile(lpDefaultParamFile);
		DisplaySet_ad[wChannelNo].fRange = gfTlRange[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]];
		ReceiveParam_ad[wChannelNo].fPlus = gfTlPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]];
		ReceiveParam_ad[wChannelNo].fRefPlus = gfTlBuChangPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]];
	}
	
	
	if(gbTlSheDingFlag[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]])
	{
		memset(gszTextPeak, 0, sizeof(gszTextPeak));
		swprintf(gszTextPeak, _T("���趨"));

		SendMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	}
	else
	{
		memset(gszTextPeak, 0, sizeof(gszTextPeak));
		swprintf(gszTextPeak, _T("δ�趨"));

		SendMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	}

	gbTlParamFlag = TRUE;
}*/
//wangbingfu add at 2011-05-27
void setRailRoadParam(WORD wChannelNo, BOOL bCurveChangeFlag)
{
	//����ͨ��̽�˲������е�ͨ��̽��ʱ����
	//gfTsOldRange = DisplaySet_ad[wChannelNo].fRange;
	//gfTsOldPlus = ReceiveParam_ad[wChannelNo].fPlus;
	//gfTsOldRefPlus = ReceiveParam_ad[wChannelNo].fRefPlus;
	float fOldGain = ReceiveParam_ad[wChannelNo].fPlus;
	float fOldRange = DisplaySet_ad[wChannelNo].fRange;
	float fOldRefGain = ReceiveParam_ad[wChannelNo].fRefPlus;
	
	DisplaySet_ad[wChannelNo].fRange = gfTlRange[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]];
	ReceiveParam_ad[wChannelNo].fPlus = gfTlPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]];
	ReceiveParam_ad[wChannelNo].fRefPlus = gfTlBuChangPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]];
	
	SetRangeLableAndExecute(NULL, gfTlRange[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]], FALSE);
	SetPlusLableAndExecute(NULL, gfTlPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]], 
						gfTlBuChangPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]],FALSE, wChannelNo);
	SetRefPlusLableAndExecute(NULL, gfTlPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]], 
						gfTlBuChangPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]],FALSE, wChannelNo);

	gainChangeCurve(fOldGain, FALSE);//����������ı�
	rangeChangeCurve(fOldRange, FALSE);//���������̸ı�
	refGainChangeCurve(fOldRefGain, FALSE);//wangbingfu add at 2011-03-09 
	SaveParamFile(lpDefaultParamFile);//���ʱ�ٱ������
	
	
	if(gbTlSheDingFlag[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]])
	{
		memset(gszTextPeak, 0, sizeof(gszTextPeak));
		swprintf(gszTextPeak, _T("���趨"));

		SendMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	}
	else
	{
		memset(gszTextPeak, 0, sizeof(gszTextPeak));
		swprintf(gszTextPeak, _T("δ�趨"));

		SendMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	}
}

/**************************
*����ͨ��̽�����̣���׼���棬��������
* @since    1.00
***************************/
void setCommonTSParam(WORD wChannelNo)
{	
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		DisplaySet_ad[i].fRange = gfTsOldRange[i];
		ReceiveParam_ad[i].fPlus = gfTsOldPlus[i];
		ReceiveParam_ad[i].fRefPlus = gfTsOldRefPlus[i];
	}
	
	float fOldGain = gfTlPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]];
	float fOldRange = gfTlRange[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]];
	float fOldRefGain = gfTlBuChangPlus[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]];
	
	gainChangeCurve(fOldGain, TRUE);//����������ı�
	rangeChangeCurve(fOldRange, TRUE);//���������̸ı�
	refGainChangeCurve(fOldRefGain, FALSE);//wangbingfu add at 2011-03-09
	
//	ReadParamFile(lpDefaultParamFile);
	SetRangeLableAndExecute(NULL, DisplaySet_ad[wChannelNo].fRange, TRUE);
	SetPlusLableAndExecute(NULL, ReceiveParam_ad[wChannelNo].fPlus, ReceiveParam_ad[wChannelNo].fRefPlus, TRUE, wChannelNo);
	SetRefPlusLableAndExecute(NULL, ReceiveParam_ad[wChannelNo].fPlus, ReceiveParam_ad[wChannelNo].fRefPlus, TRUE, wChannelNo);

//	gbTlParamFlag = FALSE; // wangbingfu delete at 2011-05-27
}

/**************************
*��ʾ��ⲿλ
* @param    hWnd[in]:����
* @param    wStandard[in]:��׼
* @param    wPosition[in]:��λ
* @return   ATOM :
* @since    1.00
***************************/
static void  displayPosition(HWND hWnd, WORD wStandard, WORD wPosition)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	if(wStandard == 0)		//TB/T1618
	{
		if(wPosition == 0)
		{
			wsprintf(strText, _T("�����޶����"));
		//	DrawLableText(hWnd, strText, 15, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);	
		}
		else if(wPosition == 1)
		{
			wsprintf(strText, _T("�����ж����"));
		//	DrawLableText(hWnd, strText, 15, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);	
		}
		else if(wPosition == 2)
		{
			wsprintf(strText, _T("��������˨��"));
		//	DrawLableText(hWnd, strText, 15, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);	
		}
		else if(wPosition == 3)
		{
			wsprintf(strText, _T("����̽��"));
		//	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 4)
		{
			wsprintf(strText, _T("�����ᾱ"));
		//	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 5)
		{
			wsprintf(strText, _T("��������"));
		//	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 6)
		{
			wsprintf(strText, _T("��������"));
		//	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 7)
		{
			wsprintf(strText, _T("�ײ�˥��"));
		//	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else
		{
			;//
		}
		DrawLableText3(hWnd, strText, 20, STATIC_TEXT_COLOR,LABEL_START_X, LABEL_4_START_Y - 10,LABEL_WIDTH, LABEL_HEIGHT2);
	}
	else if(wStandard == 1)		//TB/T2494.1
	{
		if(wPosition == 0)
		{
			wsprintf(strText, _T("����͸��"));
		//	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 1)
		{
			wsprintf(strText, _T("������"));
		//	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 2)
		{
			wsprintf(strText, _T("����Ա�͸��"));
		//	DrawLableText(hWnd, strText, 15, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);	
		}
		else
		{
			;//
		}
		DrawLableText3(hWnd, strText, 20, STATIC_TEXT_COLOR,LABEL_START_X, LABEL_4_START_Y - 10,LABEL_WIDTH, LABEL_HEIGHT2);
	}
/*	else if(wStandard == 2)		//TB/T2494.2
	{
		if(wPosition == 0)
		{
			wsprintf(strText, _T("TS-1͸��"));
		//	DrawLableText(hWnd, strText, 18, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 1)
		{
			wsprintf(strText, _T("����Ա�͸��"));
		//	DrawLableText(hWnd, strText, 15, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 2)
		{
			wsprintf(strText, _T("�ᾱ"));
		//	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 3)
		{
			wsprintf(strText, _T("�������"));
		//	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 4)
		{
			wsprintf(strText, _T("�����ڲ�"));
		}
		else if(wPosition == 5)
		{
			wsprintf(strText, _T("���������"));
		}
		else if(wPosition == 6)
		{
			wsprintf(strText, _T("�������ڲ�"));
		}
		else if(wPosition == 7)
		{
			wsprintf(strText, _T("LZ40���ᴩ͸"));
		}
		else if(wPosition == 8)
		{
			wsprintf(strText, _T("LZ50���ᴩ͸"));
		}
		else if(wPosition == 9)
		{
			wsprintf(strText, _T("�Შ̽��"));
		}
		else
		{
			;//
		}
		DrawLableText3(hWnd, strText, 20, STATIC_TEXT_COLOR,LABEL_START_X, LABEL_4_START_Y - 10,LABEL_WIDTH, LABEL_HEIGHT2);
	}
	*/
	else if(wStandard == 2)		//TB/T2995
	{
		if(wPosition == 0)
		{
			wsprintf(strText, _T("�ֹ�����̽��"));
		}
		else if(wPosition == 1)
		{
			wsprintf(strText, _T("�ֹ�����̽��"));
		}
		else if(wPosition == 2)
		{
			wsprintf(strText, _T("�ֹ��ھ���̽��"));
		}
		else if(wPosition == 3)
		{
			wsprintf(strText, _T("��������̽��"));
		}
		else if(wPosition == 4)
		{
			wsprintf(strText, _T("��������̽��"));
		}
		else if(wPosition == 5)
		{
			wsprintf(strText, _T("�ֹ�����͸��"));
		}
		else if(wPosition == 6)
		{
			wsprintf(strText, _T("��������͸��"));
		}
		else
		{
			;//
		}
		DrawLableText3(hWnd, strText, 20, STATIC_TEXT_COLOR,LABEL_START_X, LABEL_4_START_Y - 10,LABEL_WIDTH, LABEL_HEIGHT2);
	}
	else if(wStandard == 3)		//2007-98����
	{
		if(wPosition == 0)
		{
			wsprintf(strText, _T("ȫ�ᴩ͸RD2-TZSR"));
		}
		else if(wPosition == 1)
		{
			wsprintf(strText, _T("ȫ�ᴩ͸RD2-TS1"));
		}
		else if(wPosition == 2)
		{
			wsprintf(strText, _T("ȫ�ᴩ͸RD2ʵ��"));
		}
		else if(wPosition == 3)
		{
			wsprintf(strText, _T("ȫ�ᴩ͸RE2B"));
		}
		else if(wPosition == 4)
		{
			wsprintf(strText, _T("��������RD2��TZSR"));
		}
		else if(wPosition == 5)
		{
			wsprintf(strText, _T("��������RD2��ʵ��"));
		}
		else if(wPosition == 6)
		{
			wsprintf(strText, _T("��������RE2B�Შ"));
		}
		else if(wPosition == 7)
		{
			wsprintf(strText, _T("��������RD2��TZSR"));
		}
		else if(wPosition == 8)
		{
			wsprintf(strText, _T("��������RD2��ʵ��"));
		}
		else if(wPosition ==9)
		{
			wsprintf(strText, _T("��������RE2B�ݲ�"));
		}
		else if(wPosition == 10)
		{
			wsprintf(strText, _T("�ᾱ��ж�ɲ�RD2-TZSR"));
		}
		else if(wPosition == 11)
		{
			wsprintf(strText, _T("�ᾱ��ж�ɲ�RD2ʵ��"));
		}
		else if(wPosition == 12)
		{
			wsprintf(strText, _T("�ᾱ��ж�ɲ�RE2B"));
		}
		else
		{
			;//
		}
		DrawLableText3(hWnd, strText, 16, STATIC_TEXT_COLOR,LABEL_START_X, LABEL_4_START_Y - 10,LABEL_WIDTH, LABEL_HEIGHT2);
	}
	else if(wStandard == 4)
	{
		if(wPosition == 0)
		{
			wsprintf(strText, _T("ȫ�ᴩ͸"));
		}
		else if(wPosition == 1)
		{
			wsprintf(strText, _T("�������"));
		}
		else if(wPosition == 2)
		{
			wsprintf(strText, _T("�����ڲ�"));
		}
		else if(wPosition == 3)
		{
			wsprintf(strText, _T("�ƶ�����"));
		}
		else if(wPosition == 4)
		{
			wsprintf(strText, _T("�ᾱ��ж�ɲ�"));
		}
		else if(wPosition == 5)
		{
			wsprintf(strText, _T("�����Შ"));
		}
		else if(wPosition == 6)
		{
			wsprintf(strText, _T("�����ݲ�˫��"));
		}
		else
		{
			;//
		}
		DrawLableText3(hWnd, strText, 20, STATIC_TEXT_COLOR,LABEL_START_X, LABEL_4_START_Y - 10,LABEL_WIDTH, LABEL_HEIGHT2);
	}
	else
	{
		;//
	}
	//DrawLableText3(hWnd, strText, 20, STATIC_TEXT_COLOR,LABEL_START_X, LABEL_4_START_Y - 10,LABEL_WIDTH, LABEL_HEIGHT2);
}

/**************************
*��ʾ��׼
* @param    hWnd[in]:����
* @param    wStandard[in]:��׼
* @return   ATOM :
* @since    1.00
***************************/
static void  displayStandard(HWND hWnd, WORD wStandard, WORD wCurrentChannelNo)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	if(wStandard == 0)
	{
		wsprintf(strText, _T("TB/T1618"));
		DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else if(wStandard == 1)
	{
		wsprintf(strText, _T("TB/T2494.1"));
		DrawLableText(hWnd, strText, 17, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
/*	else if(wStandard == 2)
	{
		wsprintf(strText, _T("TB/T2494.2"));
		DrawLableText(hWnd, strText, 17, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}*/
	else if(wStandard == 2)
	{
		wsprintf(strText, _T("TB/T2995"));
		DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else if(wStandard == 3)
	{
		wsprintf(strText, _T("2007-98"));
		DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else if(wStandard == 4)
	{
		wsprintf(strText, _T("2009-493"));
		DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else
	{
		;//
	}
	displayPosition(hBtnTlMenuPosition, wStandard, gwPosition[wCurrentChannelNo][wStandard]);
}

/**************************
*ע����·ר�ò˵�����
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerTlMenuChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = (WNDPROC)tlMenuChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szTlMenuChild;

	return RegisterClass(&wc);
}

/**************************
*��·ר�ò˵����ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK tlMenuChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			ReadParamFile(lpDefaultParamFile);
			for(int i = 0; i < CHANNEL_NUM; i++)
			{
				gfTsOldRange[i] = DisplaySet_ad[i].fRange;
				gfTsOldPlus[i] = ReceiveParam_ad[i].fPlus;
				gfTsOldRefPlus[i] = ReceiveParam_ad[i].fRefPlus;
			}
			setRailRoadParam(CurrentChannelNo, TRUE);//���øñ�׼�ò�λ���̣���׼���棬��������
			
			hBtnTlMenuStandard = CreateWindow(_T("BUTTON"),_T("TlMenuStandard"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_TLMENU_TLMENUSTANDARD,hInst,NULL);

			hBtnTlMenuPosition = CreateWindow(_T("BUTTON"),_T("param"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_TLMENU_POSITION,hInst,NULL);

			hBtnTlMenuAdjust= CreateWindow(_T("BUTTON"),_T("adjust"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_TLMENU_ADJUST,hInst,NULL);
			
			hBtnTlMenuCommonTest = CreateWindow(_T("BUTTON"),_T("CommonTest"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 3*BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_TLMENU_COMMON_TEST,hInst,NULL);

			tlMenuBtnList[0] = hBtnTlMenuStandard;
			tlMenuBtnList[1] = hBtnTlMenuPosition;
			tlMenuBtnList[2] = hBtnTlMenuAdjust;
			tlMenuBtnList[3] = hBtnTlMenuCommonTest;

			//��ť�ؼ����Զ��崦�����
			OldBtnTlMenuStandardProc = (WNDPROC) GetWindowLong(hBtnTlMenuStandard,GWL_WNDPROC);
			SetWindowLong(hBtnTlMenuStandard,GWL_WNDPROC,(LONG)btnTlMenuStandardProc);
			
			OldBtnTlMenuPositionProc = (WNDPROC) GetWindowLong(hBtnTlMenuPosition,GWL_WNDPROC);
			SetWindowLong(hBtnTlMenuPosition,GWL_WNDPROC,(LONG)btnTlMenuPositionProc);
			
			OldBtnTlMenuAdjustProc = (WNDPROC) GetWindowLong(hBtnTlMenuAdjust,GWL_WNDPROC);
			SetWindowLong(hBtnTlMenuAdjust,GWL_WNDPROC,(LONG)btnTlMenuAdjustProc);

			// songchenguang add start 2011-02-09
			OldBtnTlMenuCommonTestProc = (WNDPROC) GetWindowLong(hBtnTlMenuCommonTest,GWL_WNDPROC);
			SetWindowLong(hBtnTlMenuCommonTest,GWL_WNDPROC,(LONG)btnTlMenuCommonTestProc);
			// songchenguang add end 2011-02-09
			
			currentTlMenuBtn = tlMenuBtnList[0];//Ĭ�ϵ�ǰѡ�б�׼��ť
			SetFocus(currentTlMenuBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentTlMenuBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndTlMenu, &ps);
			// TODO: Add any drawing code here...
		//	RECT rt;
		//	GetClientRect(hWnd, &rt);
		//	DrawText(hdc, szHello, _tcslen(szHello), &rt, DT_CENTER);
			//����ť�ؼ�
			
			
			EndPaint(hWndTlMenu, &ps);
			break;
		case WM_KEYDOWN:

			break;
		case WM_DRAWITEM://�ػ���ť
			pdis=(LPDRAWITEMSTRUCT)lParam;
			tlMenu_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnTlMenuStandard)
			{
				displayStandard(pdis->hwndItem, gwStandard[CurrentChannelNo], CurrentChannelNo);
			}
			else if(pdis->hwndItem == hBtnTlMenuPosition)
			{
				displayPosition(pdis->hwndItem, gwStandard[CurrentChannelNo], gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]);
			}
			else
			{
				;//
			}
			break;
		case WM_CHAR:			
			if( wParam == '\t')
			{
				// ���°�ť������
				if(CurrentChannelNo == 0)
				{
					displayStandard(hBtnTlMenuStandard, gwStandard[1], 1);
					setRailRoadParam(1, FALSE);
				}
				else
				{
					displayStandard(hBtnTlMenuStandard, gwStandard[0], 0);
					setRailRoadParam(0, FALSE);
				}
			}
			SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			
			if(gbTlSheDingFlag[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]])
			{
				memset(gszTextPeak, 0, sizeof(gszTextPeak));
				swprintf(gszTextPeak, _T("���趨"));

				SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
			}
			else
			{
				memset(gszTextPeak, 0, sizeof(gszTextPeak));
				swprintf(gszTextPeak, _T("δ�趨"));

				SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
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
*������·ר�ò˵����ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int tlMenu_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_smBtnBitmap[]=
	{
		{IDC_TLMENU_TLMENUSTANDARD	,IDB_BITMAP_TL_BIAOZHUN_NOSEL_R	 ,  IDB_BITMAP_TL_BIAOZHUN_SEL_R		},
		{IDC_TLMENU_POSITION	,IDB_BITMAP_TL_JIANCEBUWEI_NOSEL_R	 ,  IDB_BITMAP_TL_JIANCEBUWEI_SEL_R		},
		{IDC_TLMENU_ADJUST	,IDB_BITMAP_TL_TIAOJIAO_NOSEL_R	 ,  IDB_BITMAP_TL_TIAOJIAO_SEL_R		},
		{IDC_TLMENU_COMMON_TEST, IDB_BITMAP_COMMON_TEST_NOSEL_R	 ,  IDB_BITMAP_COMMON_TEST_SEL_R},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_smBtnBitmap);
	return ReturnVal;
}

/**************************
*��׼��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnTlMenuStandardProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tlMenuBtnList[i] != currentTlMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTlMenuBtn = tlMenuBtnList[i];
					SetFocus(currentTlMenuBtn);
					gCurrentFocusBtn = currentTlMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tlMenuBtnList[i] != currentTlMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTlMenuBtn = tlMenuBtnList[i];
					SetFocus(currentTlMenuBtn);
					gCurrentFocusBtn = currentTlMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
				/*	hWndChTlMenuStandard = CreateWindow(szChTlMenuStandardChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTlMenu,SW_HIDE);
					//SetFocus(hWndChTlMenuStandard);
					ShowWindow(hWndChTlMenuStandard,SW_SHOW);
					UpdateWindow(hWndChTlMenuStandard);

					gCurrentWnd = hWndChTlMenuStandard;
					*/
				}
				else if(wParam == VK_LEFT)
				{
					if(gwDeviceTypeValue == 0x2024)
					{
						if(gwStandard[CurrentChannelNo] == 0)
						{
							gwStandard[CurrentChannelNo]  = TL_STANDARD_NUM - 1;
						}
						else
						{
							gwStandard[CurrentChannelNo]--;
						}
					}
					else if(gwDeviceTypeValue == 0x2025)
					{
						if(gwStandard[CurrentChannelNo] != 0)
						{
							gwStandard[CurrentChannelNo] = 0;
						}
					}
					else if(gwDeviceTypeValue == 0x2026)
					{
						if(gwStandard[CurrentChannelNo] == 1)
						{
							gwStandard[CurrentChannelNo]  = 4;
						}
						else if(gwStandard[CurrentChannelNo] == 3)
						{
							gwStandard[CurrentChannelNo]  = 1;
						}
						else if(gwStandard[CurrentChannelNo] == 4)
						{
							gwStandard[CurrentChannelNo]  = 3;
						}
						else
						{
							gwStandard[CurrentChannelNo] = 1;
						}
					}
					else if(gwDeviceTypeValue == 0x2027)
					{
						if(gwStandard[CurrentChannelNo] != 4)
						{
							gwStandard[CurrentChannelNo] = 4;
						}
					}
					else
					{
						;//
					}

					displayStandard(hBtnTlMenuStandard, gwStandard[CurrentChannelNo], CurrentChannelNo);
					//SaveParamFile(lpDefaultParamFile);
					setRailRoadParam(CurrentChannelNo, TRUE);//���øñ�׼�ò�λ���̣���׼���棬��������
				}
				else if(wParam == VK_RIGHT)
				{
					if(gwDeviceTypeValue == 0x2024)
					{
						if(gwStandard[CurrentChannelNo] == TL_STANDARD_NUM - 1)
						{
							gwStandard[CurrentChannelNo]  = 0;
						}
						else
						{
							gwStandard[CurrentChannelNo]++;
						}
					}
					else if(gwDeviceTypeValue == 0x2025)
					{
						if(gwStandard[CurrentChannelNo] != 0)
						{
							gwStandard[CurrentChannelNo] = 0;
						}
					}
					else if(gwDeviceTypeValue == 0x2026)
					{
						if(gwStandard[CurrentChannelNo] == 1)
						{
							gwStandard[CurrentChannelNo]  = 3;
						}
						else if(gwStandard[CurrentChannelNo] == 3)
						{
							gwStandard[CurrentChannelNo]  = 4;
						}
						else if(gwStandard[CurrentChannelNo] == 4)
						{
							gwStandard[CurrentChannelNo]  = 1;
						}
						else
						{
							gwStandard[CurrentChannelNo] = 1;
						}
					}
					else if(gwDeviceTypeValue == 0x2027)
					{
						if(gwStandard[CurrentChannelNo] != 4)
						{
							gwStandard[CurrentChannelNo] = 4;
						}
					}
					else
					{
						;//
					}

					displayStandard(hBtnTlMenuStandard, gwStandard[CurrentChannelNo], CurrentChannelNo);
					//SaveParamFile(lpDefaultParamFile);
					setRailRoadParam(CurrentChannelNo, TRUE);//���øñ�׼�ò�λ���̣���׼���棬��������
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
				else if(wParam == 'X')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
	return CallWindowProc(OldBtnTlMenuStandardProc,Wnd,Msg,wParam,lParam);
}
/**************************
*��ⲿλ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnTlMenuPositionProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tlMenuBtnList[i] != currentTlMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTlMenuBtn = tlMenuBtnList[i];
					SetFocus(currentTlMenuBtn);
					gCurrentFocusBtn = currentTlMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tlMenuBtnList[i] != currentTlMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTlMenuBtn = tlMenuBtnList[i];
					SetFocus(currentTlMenuBtn);
					gCurrentFocusBtn = currentTlMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{ 
				}
				else if(wParam == VK_LEFT)
				{
					if(gwStandard[CurrentChannelNo] == 0)		//TB/T1618
					{
						if(gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] == 0)
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] = 7;
						}
						else
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]--;
						}
					}
					else if(gwStandard[CurrentChannelNo] == 1)	//TB/T2494.1
					{
						if(gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] == 0)
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] = 2;
						}
						else
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]--;
						}
					}
			/*		else if(gwStandard[CurrentChannelNo] == 2)	//TB/T2494.2
					{
						if(gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] == 0)
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] = 9;
						}
						else
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]--;
						}
					}*/
					else if(gwStandard[CurrentChannelNo] == 2)//TB/T2995
					{
						if(gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] == 0)
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] = 6;
						}
						else
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]--;
						}
					}
					else if(gwStandard[CurrentChannelNo] == 3)//2007-98
					{
						if(gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] == 0)
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] = 12;
						}
						else
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]--;
						}
					}
					else if(gwStandard[CurrentChannelNo] == 4)//2009-493
					{
						if(gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] == 0)
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] = 6;
						}
						else
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]--;
						}
					}
					else
					{
						;//
					}

					displayPosition(hBtnTlMenuPosition, gwStandard[CurrentChannelNo], gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]);
				//	SaveParamFile(lpDefaultParamFile);
					setRailRoadParam(CurrentChannelNo, TRUE);//���øñ�׼�ò�λ���̣���׼���棬��������
				}
				else if(wParam == VK_RIGHT)
				{
					if(gwStandard[CurrentChannelNo] == 0)//TB/T1618
					{
						if(gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] == 7)
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] = 0;
						}
						else
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]++;
						}
					}
					else if(gwStandard[CurrentChannelNo] == 1)//TB/T2494.1
					{
						if(gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] == 2)
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] = 0;
						}
						else
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]++;
						}
					}
				/*	else if(gwStandard[CurrentChannelNo] == 2)//TB/T2494.2
					{
						if(gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] == 9)
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] = 0;
						}
						else
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]++;
						}
					}*/
					else if(gwStandard[CurrentChannelNo] == 2)//TB/T2995
					{
						if(gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] == 6)
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] = 0;
						}
						else
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]++;
						}
					}
					else if(gwStandard[CurrentChannelNo] == 3)//2007-98
					{
						if(gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] == 12)
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] = 0;
						}
						else
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]++;
						}
					}
					else if(gwStandard[CurrentChannelNo] == 4)//2009-493
					{
						if(gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] == 6)
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]] = 0;
						}
						else
						{
							gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]++;
						}
					}
					else
					{
						;//
					}

					displayPosition(hBtnTlMenuPosition, gwStandard[CurrentChannelNo], gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]);
				//	SaveParamFile(lpDefaultParamFile);
					setRailRoadParam(CurrentChannelNo, TRUE);//���øñ�׼�ò�λ���̣���׼���棬��������
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
				else if(wParam == 'X')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
	return CallWindowProc(OldBtnTlMenuPositionProc,Wnd,Msg,wParam,lParam);
}

/**************************
*��У��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnTlMenuAdjustProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tlMenuBtnList[i] != currentTlMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTlMenuBtn = tlMenuBtnList[i];
					SetFocus(currentTlMenuBtn);
					gCurrentFocusBtn = currentTlMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tlMenuBtnList[i] != currentTlMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTlMenuBtn = tlMenuBtnList[i];
					SetFocus(currentTlMenuBtn);
					gCurrentFocusBtn = currentTlMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndTlAdjust = CreateWindow(szTlAdjustChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTlMenu,SW_HIDE);

					ShowWindow(hWndTlAdjust,SW_SHOW);
					UpdateWindow(hWndTlAdjust);

					gCurrentWnd = hWndTlAdjust;				
					
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
				else if(wParam == 'X')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
	return CallWindowProc(OldBtnTlMenuAdjustProc,Wnd,Msg,wParam,lParam);
}

// songchenguang add start 2011-02-09
/**************************
* ͨ��̽�˰�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/

static LRESULT CALLBACK btnTlMenuCommonTestProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tlMenuBtnList[i] != currentTlMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTlMenuBtn = tlMenuBtnList[i];
					SetFocus(currentTlMenuBtn);
					gCurrentFocusBtn = currentTlMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tlMenuBtnList[i] != currentTlMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTlMenuBtn = tlMenuBtnList[i];
					SetFocus(currentTlMenuBtn);
					gCurrentFocusBtn = currentTlMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{					
					hWndTsMenu = CreateWindow(szTsMenuChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTlMenu,SW_HIDE);
					
					ShowWindow(hWndTsMenu,SW_SHOW);
					UpdateWindow(hWndTsMenu);

					gCurrentWnd = hWndTsMenu;

					//setCommonTSParam(CurrentChannelNo);// wangbingfu delete at 2011-05-27
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
				else if(wParam == 'X')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
	return CallWindowProc(OldBtnTlMenuCommonTestProc,Wnd,Msg,wParam,lParam);

}
// songchenguang add end 2011-02-09


