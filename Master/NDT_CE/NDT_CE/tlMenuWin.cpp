/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 铁路专用菜单窗口源文件
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

HWND hWndTlMenu = NULL;//铁路专用菜单窗口句柄
HWND currentTlMenuBtn = NULL;// 铁路专用菜单窗口当前选中的按钮

WORD gwStandard[CHANNEL_NUM] = {0, 0};	//检测标准
WORD gwPosition[CHANNEL_NUM][TL_STANDARD_NUM] = {{0, 0, 0, 0, 0},{0, 0, 0 ,0, 0}};	//检测部位
BOOL gbTlSheDingFlag[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//各标准各部位是否已设定标志
float gfTlRange[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];			//各标准各部位声程
float gfTlPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];				//各标准各部位增益
float gfTlBuChangPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//各标准各部位补偿增益

float gfTsOldRange[CHANNEL_NUM] = {0.0f, 0.0f};		//保存通用探伤的声程
float gfTsOldPlus[CHANNEL_NUM] = {0.0f, 0.0f};		//保存通用探伤的基准增益
float gfTsOldRefPlus[CHANNEL_NUM] = {0.0f, 0.0f};	//保存通用探伤的参考增益

BOOL gbTlParamFlag = TRUE;	//当前是否使用铁路参数	//wangbingfu add at 2011-5-25

static HWND hBtnTlMenuStandard = NULL;	//标准按钮句柄
static HWND hBtnTlMenuPosition = NULL;	//检测部位按钮句柄
static HWND hBtnTlMenuAdjust = NULL;	//调校按钮句柄
// songchenguang add start 2011-02-09
static HWND hBtnTlMenuCommonTest = NULL;	//通用探伤按钮句柄
// songchenguang add end 2011-02-09

static HWND tlMenuBtnList[4] = {NULL, NULL, NULL, NULL};//铁路专用菜单窗口的按钮表
static const int buttonNum = 4;//按钮控件数

static WNDPROC OldBtnTlMenuStandardProc = NULL;//标准按钮默认处理过程
static WNDPROC OldBtnTlMenuPositionProc = NULL;//检测部位按钮默认处理过程
static WNDPROC OldBtnTlMenuAdjustProc = NULL;// 调校按钮默认处理过程
// songchenguang add start 2011-02-09
static WNDPROC OldBtnTlMenuCommonTestProc = NULL;	//通用探伤按钮默认处理过程
extern HWND hWndTsMenu;			//探伤菜单窗口句柄
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


extern HINSTANCE hInst;			//程序实例句柄
extern HWND hWndMain;			//主窗口句柄
extern HWND gCurrentFocusBtn;
extern HWND gCurrentWnd;
extern HWND hWndTlAdjust;		//调校窗口句柄
extern float gfTlGainBuChang[CHANNEL_NUM];	//增益补偿
extern WORD CurrentChannelNo;				// 当前通道号
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern wchar_t gszTextPeak[20];
extern HWND hStatusBar;					//状态栏句柄
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
	// 传值
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
*设置该标准该部位声程，基准增益，补偿增益
* @since    1.00
***************************/
// wangbingfu delete at 2011-05-27
/*
void setRailRoadParam(WORD wChannelNo, BOOL bCurveChangeFlag)
{
	//保存通用探伤参数，切到通用探伤时调用
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
		gainChangeCurve(fOldGain, FALSE);//曲线随增益改变
		rangeChangeCurve(fOldRange, FALSE);//曲线随声程改变
		refGainChangeCurve(fOldRefGain, FALSE);//wangbingfu add at 2011-03-09

		//保存原来参数
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
		swprintf(gszTextPeak, _T("已设定"));

		SendMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	}
	else
	{
		memset(gszTextPeak, 0, sizeof(gszTextPeak));
		swprintf(gszTextPeak, _T("未设定"));

		SendMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	}

	gbTlParamFlag = TRUE;
}*/
//wangbingfu add at 2011-05-27
void setRailRoadParam(WORD wChannelNo, BOOL bCurveChangeFlag)
{
	//保存通用探伤参数，切到通用探伤时调用
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

	gainChangeCurve(fOldGain, FALSE);//曲线随增益改变
	rangeChangeCurve(fOldRange, FALSE);//曲线随声程改变
	refGainChangeCurve(fOldRefGain, FALSE);//wangbingfu add at 2011-03-09 
	SaveParamFile(lpDefaultParamFile);//最后时再保存参数
	
	
	if(gbTlSheDingFlag[wChannelNo][gwStandard[wChannelNo]][gwPosition[wChannelNo][gwStandard[wChannelNo]]])
	{
		memset(gszTextPeak, 0, sizeof(gszTextPeak));
		swprintf(gszTextPeak, _T("已设定"));

		SendMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	}
	else
	{
		memset(gszTextPeak, 0, sizeof(gszTextPeak));
		swprintf(gszTextPeak, _T("未设定"));

		SendMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	}
}

/**************************
*设置通用探伤声程，基准增益，补偿增益
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
	
	gainChangeCurve(fOldGain, TRUE);//曲线随增益改变
	rangeChangeCurve(fOldRange, TRUE);//曲线随声程改变
	refGainChangeCurve(fOldRefGain, FALSE);//wangbingfu add at 2011-03-09
	
//	ReadParamFile(lpDefaultParamFile);
	SetRangeLableAndExecute(NULL, DisplaySet_ad[wChannelNo].fRange, TRUE);
	SetPlusLableAndExecute(NULL, ReceiveParam_ad[wChannelNo].fPlus, ReceiveParam_ad[wChannelNo].fRefPlus, TRUE, wChannelNo);
	SetRefPlusLableAndExecute(NULL, ReceiveParam_ad[wChannelNo].fPlus, ReceiveParam_ad[wChannelNo].fRefPlus, TRUE, wChannelNo);

//	gbTlParamFlag = FALSE; // wangbingfu delete at 2011-05-27
}

/**************************
*显示检测部位
* @param    hWnd[in]:窗口
* @param    wStandard[in]:标准
* @param    wPosition[in]:部位
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
			wsprintf(strText, _T("轴向无顶针孔"));
		//	DrawLableText(hWnd, strText, 15, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);	
		}
		else if(wPosition == 1)
		{
			wsprintf(strText, _T("轴向有顶针孔"));
		//	DrawLableText(hWnd, strText, 15, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);	
		}
		else if(wPosition == 2)
		{
			wsprintf(strText, _T("轴向有螺栓孔"));
		//	DrawLableText(hWnd, strText, 15, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);	
		}
		else if(wPosition == 3)
		{
			wsprintf(strText, _T("轴向探伤"));
		//	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 4)
		{
			wsprintf(strText, _T("径向轴颈"));
		//	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 5)
		{
			wsprintf(strText, _T("径向轮座"));
		//	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 6)
		{
			wsprintf(strText, _T("径向轴身"));
		//	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 7)
		{
			wsprintf(strText, _T("底波衰减"));
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
			wsprintf(strText, _T("轴向透声"));
		//	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 1)
		{
			wsprintf(strText, _T("轴向定量"));
		//	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 2)
		{
			wsprintf(strText, _T("半轴对比透声"));
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
			wsprintf(strText, _T("TS-1透声"));
		//	DrawLableText(hWnd, strText, 18, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 1)
		{
			wsprintf(strText, _T("半轴对比透声"));
		//	DrawLableText(hWnd, strText, 15, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 2)
		{
			wsprintf(strText, _T("轴颈"));
		//	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 3)
		{
			wsprintf(strText, _T("轮座外侧"));
		//	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else if(wPosition == 4)
		{
			wsprintf(strText, _T("轮座内侧"));
		}
		else if(wPosition == 5)
		{
			wsprintf(strText, _T("齿轮座外侧"));
		}
		else if(wPosition == 6)
		{
			wsprintf(strText, _T("齿轮座内侧"));
		}
		else if(wPosition == 7)
		{
			wsprintf(strText, _T("LZ40车轴穿透"));
		}
		else if(wPosition == 8)
		{
			wsprintf(strText, _T("LZ50车轴穿透"));
		}
		else if(wPosition == 9)
		{
			wsprintf(strText, _T("横波探伤"));
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
			wsprintf(strText, _T("轮箍轴向探伤"));
		}
		else if(wPosition == 1)
		{
			wsprintf(strText, _T("轮箍径向探伤"));
		}
		else if(wPosition == 2)
		{
			wsprintf(strText, _T("轮箍内径面探伤"));
		}
		else if(wPosition == 3)
		{
			wsprintf(strText, _T("轮辋轴向探伤"));
		}
		else if(wPosition == 4)
		{
			wsprintf(strText, _T("轮辋径向探伤"));
		}
		else if(wPosition == 5)
		{
			wsprintf(strText, _T("轮箍轴向透声"));
		}
		else if(wPosition == 6)
		{
			wsprintf(strText, _T("轮辋轴向透声"));
		}
		else
		{
			;//
		}
		DrawLableText3(hWnd, strText, 20, STATIC_TEXT_COLOR,LABEL_START_X, LABEL_4_START_Y - 10,LABEL_WIDTH, LABEL_HEIGHT2);
	}
	else if(wStandard == 3)		//2007-98号文
	{
		if(wPosition == 0)
		{
			wsprintf(strText, _T("全轴穿透RD2-TZSR"));
		}
		else if(wPosition == 1)
		{
			wsprintf(strText, _T("全轴穿透RD2-TS1"));
		}
		else if(wPosition == 2)
		{
			wsprintf(strText, _T("全轴穿透RD2实物"));
		}
		else if(wPosition == 3)
		{
			wsprintf(strText, _T("全轴穿透RE2B"));
		}
		else if(wPosition == 4)
		{
			wsprintf(strText, _T("轮座镶入RD2横TZSR"));
		}
		else if(wPosition == 5)
		{
			wsprintf(strText, _T("轮座镶入RD2横实物"));
		}
		else if(wPosition == 6)
		{
			wsprintf(strText, _T("轮座镶入RE2B横波"));
		}
		else if(wPosition == 7)
		{
			wsprintf(strText, _T("轮座镶入RD2纵TZSR"));
		}
		else if(wPosition == 8)
		{
			wsprintf(strText, _T("轮座镶入RD2纵实物"));
		}
		else if(wPosition ==9)
		{
			wsprintf(strText, _T("轮座镶入RE2B纵波"));
		}
		else if(wPosition == 10)
		{
			wsprintf(strText, _T("轴颈根卸荷槽RD2-TZSR"));
		}
		else if(wPosition == 11)
		{
			wsprintf(strText, _T("轴颈根卸荷槽RD2实物"));
		}
		else if(wPosition == 12)
		{
			wsprintf(strText, _T("轴颈根卸荷槽RE2B"));
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
			wsprintf(strText, _T("全轴穿透"));
		}
		else if(wPosition == 1)
		{
			wsprintf(strText, _T("轮座外侧"));
		}
		else if(wPosition == 2)
		{
			wsprintf(strText, _T("轮座内侧"));
		}
		else if(wPosition == 3)
		{
			wsprintf(strText, _T("制动盘座"));
		}
		else if(wPosition == 4)
		{
			wsprintf(strText, _T("轴颈根卸荷槽"));
		}
		else if(wPosition == 5)
		{
			wsprintf(strText, _T("轮辋横波"));
		}
		else if(wPosition == 6)
		{
			wsprintf(strText, _T("轮辋纵波双晶"));
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
*显示标准
* @param    hWnd[in]:窗口
* @param    wStandard[in]:标准
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
*注册铁路专用菜单窗口
* @param    hInstance[in]:程序实例句柄
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
*铁路专用菜单窗口处理过程
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
			setRailRoadParam(CurrentChannelNo, TRUE);//设置该标准该部位声程，基准增益，补偿增益
			
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

			//按钮控件的自定义处理过程
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
			
			currentTlMenuBtn = tlMenuBtnList[0];//默认当前选中标准按钮
			SetFocus(currentTlMenuBtn);//选中按钮获得输入焦点
			gCurrentFocusBtn = currentTlMenuBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndTlMenu, &ps);
			// TODO: Add any drawing code here...
		//	RECT rt;
		//	GetClientRect(hWnd, &rt);
		//	DrawText(hdc, szHello, _tcslen(szHello), &rt, DT_CENTER);
			//画按钮控件
			
			
			EndPaint(hWndTlMenu, &ps);
			break;
		case WM_KEYDOWN:

			break;
		case WM_DRAWITEM://重画按钮
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
				// 更新按钮项数据
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
				swprintf(gszTextPeak, _T("已设定"));

				SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
			}
			else
			{
				memset(gszTextPeak, 0, sizeof(gszTextPeak));
				swprintf(gszTextPeak, _T("未设定"));

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
*加载铁路专用菜单窗口按钮图片
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
*标准按钮自定义处理过程
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
					setRailRoadParam(CurrentChannelNo, TRUE);//设置该标准该部位声程，基准增益，补偿增益
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
					setRailRoadParam(CurrentChannelNo, TRUE);//设置该标准该部位声程，基准增益，补偿增益
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
				else if(wParam == 'B')	// 冻结
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
*检测部位按钮自定义处理过程
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
					setRailRoadParam(CurrentChannelNo, TRUE);//设置该标准该部位声程，基准增益，补偿增益
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
					setRailRoadParam(CurrentChannelNo, TRUE);//设置该标准该部位声程，基准增益，补偿增益
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
				else if(wParam == 'B')	// 冻结
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
*调校按钮自定义处理过程
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
				else if(wParam == 'B')	// 冻结
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
* 通用探伤按钮自定义处理过程
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
				else if(wParam == 'B')	// 冻结
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


