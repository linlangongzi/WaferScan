/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 工艺参数配置确认窗口源文件
 * Filename: allParamConfirmWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "interfaceCmdID.h"

#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"
#include "craftParamWin.h"

HWND hWndAllParamConfirm = NULL;//工艺参数配置确认窗口句柄
HWND hBtnAllParamConfirmsave = NULL;//保存按钮句柄
HWND hBtnAllParamConfirmNoSave = NULL;//不保存按钮句柄
HWND hBtnAllParamConfirmCancel = NULL;//取消按钮句柄
HWND allParamConfirmBtnList[3] ={NULL,NULL,NULL};//工艺参数配置确认窗口的按钮表
WNDPROC OldBtnAllParamConfirmsaveProc = NULL;//保存按钮默认处理过程
WNDPROC OldBtnAllParamConfirmNoSaveProc = NULL;//不保存按钮默认处理过程
WNDPROC OldBtnAllParamConfirmCancelProc = NULL;//取消按钮默认处理过程
HWND currentAllParamConfirmBtn = NULL;//工艺参数配置确认窗中当前选中的按钮
static const int buttonNum = 3;//按钮控件数

ATOM registerAllParamConfirmChild(HINSTANCE hInstance);
static int allParamConfirm_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK allParamConfirmChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnAllParamConfirmsaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamConfirmNoSaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnAllParamConfirmCancelProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

extern HWND hWndMain;
extern HINSTANCE hInst;//程序实例句柄
extern HWND hStatusBar;
extern HWND hWndCraftParam;
extern HWND currentCraftParamBtn;
extern HWND hWndAllParamCfgCaption;	//工艺参数配置标题窗口句柄
extern HWND hWndAllParamCfg;		//工艺参数配置窗口句柄
extern HWND gCurrentWnd;			//当前显示的子窗口
extern HWND currentAllParamCfgBtn;
extern CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// 工作线程参数
extern SEND_PARAM_DATA SendParam_ad[2];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern SEND_PARAM_DATA Send_AllParamConfigWith_ad[2];
extern RECEIVE_PARAM_DATA Receive_AllParamConfigWith_ad[2];
extern DISPLAY_SET_DLG_DATA Display_AllParamConfigWith_ad[2];
extern TCHAR ParamFileName[MAX_PATH];
extern WORD gnParamNo;					//工艺序号
extern DWORD currentUsedParamNo;		//当前使用的工艺参数号
extern SYSTEMTIME st;//系统时间
//extern allParamConfigWithTime AllParam_Time;//工艺参数时间

//zhaoli add in 2010 -9-16
//extern float fBScanGangBanHouDu;//工件厚度参数
extern WORD wScanMode;//B扫扫描方式
extern BOOL gbBscanSwitch;//B扫开关
extern float gfPlateHeight;//中厚板探伤工件厚度
extern WORD nProbeType;//中厚板探伤探头类型
extern float gfUpBlindValue;//中厚板探伤上盲区
extern float gfDownBlindValue;//中厚板探伤下盲区
extern WORD gwPoKouLeiXing;//焊缝探伤坡口类型设置
extern float  gfPoKouJiaoDu;//焊缝探伤坡口角度设置
extern float  gfPoKouShenDu;//焊缝探伤坡口深度设置
extern BOOL bHanFengTanShangSwitch;//焊缝探伤开关
extern float fHanFengTanShangGangBanHouDu;//焊缝探伤工件厚度
extern float fHead2HanFengDistance;//探头到坡口的距离
extern BOOL nLCDValue;//仪器亮度

extern float gfPingDingGain[CHANNEL_NUM];				//评定增益
extern float gfDingLiangGain[CHANNEL_NUM];				//定量增益
extern float gfPanHuiGain[CHANNEL_NUM];				//判废增益
extern float gfParamPingDing[CHANNEL_NUM];				
extern float gfParamDingLiang[CHANNEL_NUM];	
extern float gfParamPanFei[CHANNEL_NUM];


WORD wTanTouLeiXing[CHANNEL_NUM]= {1, 1};//探头类型

extern WORD nTanTouType[CHANNEL_NUM];		//探头类型

extern SYS_INFO gSysInfo;
extern SYS_INFO gAllcfgSysInfo;
extern HWND hWndMain;

extern float gwQuXianGuiGe1;
extern float gwQuXianGuiGe2;
extern float gwQuXianGuiGe3;
extern float gfDefectZhi1[CHANNEL_NUM];
extern float gfDefectZhi2[CHANNEL_NUM];
extern float gfDefectZhi3[CHANNEL_NUM];

extern wchar_t gszTextChannel[10];
extern int gnWorkType;
extern WORD wChannelNo;
extern float allParamBScanGongJianHouDu;//B扫工件厚度
extern WORD allParamScanMode;//B扫扫描方式
extern BOOL allParamBscanSwitch;//B扫开关
extern float allParamPlateHeight;//中厚板探伤工件厚度
extern WORD allParamProbeType;//中厚板探伤探头类型
extern float allParamUpBlindValue;//中厚板探伤上盲区
extern float allParamDownBlindValue;//中厚板探伤下盲区
extern WORD allParamPoKouLeiXing;//焊缝探伤坡口类型设置
extern float allParamPoKouJiaoDu;//焊缝探伤坡口角度设置
extern float  allParamPoKouShenDu;//焊缝探伤坡口深度设置
extern BOOL allParamHanFengTanShangSwitch;//焊缝探伤开关
extern float allParamHanFengTanShangGangBanHouDu;//焊缝探伤工件厚度
extern float allParamHead2HanFengDistance;//探头到坡口的距离
extern BOOL allParamLCDValue;//仪器亮度
extern HWND gCurrentFocusBtn;

//zhaoli add in 2010 -9-16
// songchenguang add start 2010-12-08
extern BOOL gbHotCraftParamView;
extern WORD gwGongYiCanShu;
// songchenguang add end 2010-12-08
extern BOOL gAllCfgDgsSwitch[CHANNEL_NUM][GATE_NUM];	//DGS通道开关
extern float gfAllCfgOldRange[CHANNEL_NUM];				//最初生成DAC时的声程
extern WORD gwAllCfgOldSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//最初生成DAC时的每个点的位置
extern float fAllCfgOldRange[CHANNEL_NUM];			//刚读取时的声程
extern float fAllCfgOldPlus[CHANNEL_NUM];			//刚读取时的增益
extern float fAllCfgOldRefPlus[CHANNEL_NUM];		//刚读取时的参考增益
extern float fAllCfgOldDelay[CHANNEL_NUM];			//刚读取时的延迟值


extern wchar_t lpDefaultParamFile[MAX_PATH];
extern DWORD gAllCfgSelectedDgsPointDataNum[CHANNEL_NUM][GATE_NUM];//已选中的DGS点数量
extern CURVEPOINT_DATA gAllCfgSelectedDgsPointData[CHANNEL_NUM][GATE_NUM][MAX_CURVE_POINT_NUM];//添加的样点数据队列
extern WORD CurrentChannelNo;				// 当前通道号
extern ARS200FPGA_SYS_REG* sys_reg;
extern float gfParamGuanCaiWaiJing;//管材外径
extern float gfGuanCaiNeiJing;//管材内径
extern float gfNeiJing;//管材内径
extern float gfWaiJing;//管材外径
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//已选中的AVG点数量
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//添加的样点数据队列

extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];//已选中的DGS点数量
extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//添加的样点数据队列

extern float gfParamFaiZhi1[CHANNEL_NUM];//曲线Φ1值
extern float gfParamFaiZhi2[CHANNEL_NUM];//曲线Φ2值
extern float gfParamFaiZhi3[CHANNEL_NUM];//曲线Φ3值

extern BOOL  gbHotGateFlag;	//是否打开闸门快捷方式 wangbingfu add at 2010-8-9
extern BOOL  gbHotAutoGainFlag;	//是否打开自动增益快捷方式 
extern BOOL  gbHotAutoAdjustFlag;	//是否打开自动校准快捷方式 
extern HWND hWndGate;				// 闸门设定窗口句柄
extern HWND currentGateBtn;			// 闸门设定窗中当前选中的按钮
extern HWND hWndAutoGainPlus;//自动增益窗口句柄
extern HWND currentAutoGainPlusBtn;//自动增益窗中当前选中的按钮
extern HWND hWndAdjust;						//校准窗口句柄
extern HWND currentAdjustBtn;					//校准窗中当前选中的按钮

extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];		//添加的样点数据队列
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];											//已选中的DGS点数量
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];	//添加的样点数据队列
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];													//已选中的AVG点数量
extern short gwOldDacSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//最初生成DAC时的每个点的位置
extern short gwOldAvgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//最初生成AVG时的每个点的位置
extern float gfOldDacRange[CHANNEL_NUM];						//最初生成DAC时的声程
extern float gfOldAvgRange[CHANNEL_NUM];						//最初生成AVG时的声程

extern float gfAllOldRangeStart[CHANNEL_NUM];		//保存工艺参数之前的起始声程
extern float gfAllOldRangeEnd[CHANNEL_NUM];			//保存工艺参数之前的终点声程
extern float fRangeStart[2];

extern BOOL allCfgSaveParamFile(LPCTSTR lpszPathName);
extern BOOL ReadParamFile(LPCTSTR lpszPathName);

//wangbingfu add start at 2011-5-25
/*//wangbingfu delete at 2011-5-27
extern void setRailRoadParam(WORD wChannelNo, BOOL bCurveChangeFlag); 		//铁路
//extern BOOL gbTlParamFlag;	//当前是否使用铁路参数	
*/
//wangbingfu add end at 2011-5-25

//wangbingfu add start at 2011-5-27
extern WORD gwDeviceType;	//设备类型 0:探伤仪	1:测厚仪 2:铁路专用
extern WORD gwStandard[CHANNEL_NUM];	//检测标准
extern WORD gwPosition[CHANNEL_NUM][TL_STANDARD_NUM];	//检测部位
extern BOOL gbTlSheDingFlag[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//各标准各部位是否已设定标志
extern float gfTlRange[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];			//各标准各部位声程
extern float gfTlPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];				//各标准各部位增益
extern float gfTlBuChangPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//各标准各部位补偿增益
//wangbingfu add end at 2011-5-27

/**************************
*注册工艺参数配置确认窗口
* @param    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerAllParamConfirmChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) allParamConfirmChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szAllParamConfirmChild;

	return RegisterClass(&wc);
}
/**************************
*工艺参数配置确认窗口处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK allParamConfirmChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			//	if(hStaticBrush == NULL)	
			//		hStaticBrush = CreateSolidBrush(BK_COLOR_WHITE);
			//画按钮控件
			hBtnAllParamConfirmsave = CreateWindow(_T("BUTTON"),_T("save"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,80,60,hWnd,(HMENU)IDC_ALLPARAMCONFIRM_SAVE,hInst,NULL);
			hBtnAllParamConfirmNoSave = CreateWindow(_T("BUTTON"),_T("NoSave"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,80,
					0,80,60,hWnd,(HMENU)IDC_ALLPARAMCONFIRM_NOSAVE,hInst,NULL);
			hBtnAllParamConfirmCancel = CreateWindow(_T("BUTTON"),_T("Cancel"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,160,
					0,80,60,hWnd,(HMENU)IDC_ALLPARAMCONFIRM_CANCEL,hInst,NULL);
		
			allParamConfirmBtnList[0] = hBtnAllParamConfirmsave;
			allParamConfirmBtnList[1] = hBtnAllParamConfirmNoSave;
			allParamConfirmBtnList[2] = hBtnAllParamConfirmCancel;

			//按钮控件的自定义处理过程
			OldBtnAllParamConfirmsaveProc = (WNDPROC) GetWindowLong(hBtnAllParamConfirmsave,GWL_WNDPROC);
			SetWindowLong(hBtnAllParamConfirmsave,GWL_WNDPROC,(LONG)btnAllParamConfirmsaveProc);
			OldBtnAllParamConfirmNoSaveProc = (WNDPROC) GetWindowLong(hBtnAllParamConfirmNoSave,GWL_WNDPROC);
			SetWindowLong(hBtnAllParamConfirmNoSave,GWL_WNDPROC,(LONG)btnAllParamConfirmNoSaveProc);
			OldBtnAllParamConfirmCancelProc = (WNDPROC) GetWindowLong(hBtnAllParamConfirmCancel,GWL_WNDPROC);
		    SetWindowLong(hBtnAllParamConfirmCancel,GWL_WNDPROC,(LONG)btnAllParamConfirmCancelProc);
			
			currentAllParamConfirmBtn = allParamConfirmBtnList[0];//默认当前选中单工艺参数配置确认按钮
			SetFocus(currentAllParamConfirmBtn);//选中按钮获得输入焦点
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
			allParamConfirm_DrawButtonBitmap(hInst,pdis);
		/*	if(pdis->hwndItem == hBtnAllParamConfirmsave)
			{
				drawSrting(pdis->hwndItem,  _T("保存"), 50, STATIC_TEXT_COLOR, 
								0, 0,CONFIRM_STRING_WIDTH, CONFIRM_STRING_HEIGHT);
			}
			if(pdis->hwndItem == hBtnAllParamConfirmNoSave)
			{
				drawSrting(pdis->hwndItem,  _T("放弃"), 50, STATIC_TEXT_COLOR, 
								0, 0,CONFIRM_STRING_WIDTH, CONFIRM_STRING_HEIGHT);
			}
			if(pdis->hwndItem == hBtnAllParamConfirmCancel)
			{
				drawSrting(pdis->hwndItem,  _T("取消"), 50, STATIC_TEXT_COLOR, 
								0, 0,CONFIRM_STRING_WIDTH, CONFIRM_STRING_HEIGHT);
			}*/
			break;
		case WM_CLOSE:
		
			break;
		case WM_DESTROY:
		//	if(hStaticBrush)
		//	{
		//		DeleteObject(hStaticBrush);
		//		hStaticBrush = NULL;
		//	}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载工艺参数配置确认窗口按钮图片
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int allParamConfirm_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_allParamConfirmBtnBitmap[]=
	{
		{IDC_ALLPARAMCONFIRM_SAVE	,IDB_BITMAP_CONFIRM_BAOCUN_NOSEL_R	 ,  IDB_BITMAP_CONFIRM_BAOCUN_SELECT_R		},
		{IDC_ALLPARAMCONFIRM_NOSAVE	,IDB_BITMAP_CONFIRM_FANGQI_NOSEL_R	 ,  IDB_BITMAP_CONFIRM_FANGQI_SELECT_R		},
		{IDC_ALLPARAMCONFIRM_CANCEL	,IDB_BITMAP_CONFIRM_QUXIAO_NOSEL_R ,  IDB_BITMAP_CONFIRM_QUXIAO_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_allParamConfirmBtnBitmap);
	return ReturnVal;
}
/**************************
*保存按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAllParamConfirmsaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
			
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
				}
			
				else if(wParam == VK_LEFT)
				{
					while(allParamConfirmBtnList[i] != currentAllParamConfirmBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamConfirmBtn = allParamConfirmBtnList[i];
					SetFocus(currentAllParamConfirmBtn);
				}
				else if(wParam == VK_RIGHT)
				{
					while(allParamConfirmBtnList[i] != currentAllParamConfirmBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamConfirmBtn = allParamConfirmBtnList[i];
					SetFocus(currentAllParamConfirmBtn);
				}
				else if(wParam == VK_RETURN)
				{	


					for(int i = 0;i<2;i++)
					{
						wTanTouLeiXing[i] = nTanTouType[i];
					}
			
					//zhaoli add in 2010 9 16
		//			fBScanGangBanHouDu = allParamBScanGongJianHouDu;
					wScanMode = allParamScanMode;
					//gbBscanSwitch = allParamBscanSwitch;
					gfPlateHeight = allParamPlateHeight;
					//nProbeType = allParamProbeType;
					gfUpBlindValue = allParamUpBlindValue;
					gfDownBlindValue = allParamDownBlindValue;
					gwPoKouLeiXing = allParamPoKouLeiXing;
					gfPoKouJiaoDu = allParamPoKouJiaoDu;
					gfPoKouShenDu = allParamPoKouShenDu;
				//	bHanFengTanShangSwitch = allParamHanFengTanShangSwitch;
					fHanFengTanShangGangBanHouDu = allParamHanFengTanShangGangBanHouDu;
					fHead2HanFengDistance = allParamHead2HanFengDistance;
					nLCDValue = allParamLCDValue;
					gfWaiJing = gfParamGuanCaiWaiJing;
					gfNeiJing = gfGuanCaiNeiJing;
					CurrentChannelNo = wChannelNo;//修改通道
					gAllcfgSysInfo.wCurrentChNo = wChannelNo;
					

					
					for( int j = 0; j< CHANNEL_NUM; j++)
					{
						if(gSelectedDgsPointDataNum[j][3] > 1)
						{
							for(int i = 0; i < gSelectedDgsPointDataNum[j][3]; i++)
							{	
								gSelectedDgsPointData[j][0][i].data = (WORD)(gSelectedDgsPointData[j][3][i].data 
																							/ pow(10, (double)gfParamPingDing[j] / (-20)));
								gSelectedDgsPointData[j][0][i].serial = gSelectedDgsPointData[j][3][i].serial;
								
								gSelectedDgsPointData[j][1][i].data = (WORD)(gSelectedDgsPointData[j][3][i].data 
																							/ pow(10, (double)gfParamDingLiang[j] / (-20)));
								gSelectedDgsPointData[j][1][i].serial = gSelectedDgsPointData[j][3][i].serial;
								
								gSelectedDgsPointData[j][2][i].data = (WORD)(gSelectedDgsPointData[j][3][i].data 
																							/ pow(10, (double)gfParamPanFei[j] / (-20)));
								gSelectedDgsPointData[j][2][i].serial = gSelectedDgsPointData[j][3][i].serial;							
							}
						}
					}

					for( int i = 0; i< CHANNEL_NUM; i++)
					{
						gfPingDingGain[i] = gfParamPingDing[i];				
						gfDingLiangGain[i] = gfParamDingLiang[i];	
						gfPanHuiGain[i] = gfParamPanFei[i];
					}
					
					for( int i = 0; i< CHANNEL_NUM; i++)
					{
						if(gSelectedAvgPointDataNum[i] > 1)
						{
							for(int j = 0; j < AVG_CURVE_NUM; j++)
							{
								if(j == 0)
								{
									float fDeltaDB =(float)(40 * log10((float)gfParamFaiZhi1[i] / gfDefectZhi1[i]));
									for(int k = 0; k < gSelectedAvgPointDataNum[i]; k++)
									{
										gSelectedAvgPointData[i][j][k].data = (float)(gSelectedAvgPointData[i][j][k].data 
																							/ pow(10, (double)fDeltaDB / (-20)));
									}
								}
								else if(j == 1)
								{
									float fDeltaDB = (float)(40 * log10((float)gfParamFaiZhi2[i] / gfDefectZhi2[i]));
									for(int k = 0; k < gSelectedAvgPointDataNum[i]; k++)
									{
										gSelectedAvgPointData[i][j][k].data = (float)(gSelectedAvgPointData[i][j][k].data 
																							/ pow(10, (double)fDeltaDB / (-20)));
									}
								}
								else if(j == 2)
								{
									float fDeltaDB = (float)(40 * log10((float)gfParamFaiZhi3[i] / gfDefectZhi3[i]));
									for(int k = 0; k < gSelectedAvgPointDataNum[i]; k++)
									{
										gSelectedAvgPointData[i][j][k].data = (float)(gSelectedAvgPointData[i][j][k].data 
																							/ pow(10, (double)fDeltaDB / (-20)));
									}
								}
							}
						}
					}
					
					for( int i = 0; i< CHANNEL_NUM; i++)
					{
						gfDefectZhi1[i] = gfParamFaiZhi1[i];
						gfDefectZhi2[i] = gfParamFaiZhi2[i];
						gfDefectZhi3[i] = gfParamFaiZhi3[i];

					}
					
					
					
					//显示模式的改变
					if(gnWorkType == 1)//单通道
					{
						if(CurrentChannelNo == 0)
						{
							swprintf(gszTextChannel, _T("1"));
							SendMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
							
							gAllcfgSysInfo.wWorkMode = (WORKMODE)gnWorkType;
						}
						else if(CurrentChannelNo == 1)
						{
							swprintf(gszTextChannel, _T("2"));
							SendMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

							gnWorkType = 2;
							gAllcfgSysInfo.wWorkMode = (WORKMODE)gnWorkType;

						}
					}
			
					else if(gnWorkType == 3)//双通道
					{
						
						if(CurrentChannelNo == 0)
						{
							swprintf(gszTextChannel, _T("1"));
						}
						else if(CurrentChannelNo == 1)
						{
							swprintf(gszTextChannel, _T("2"));
						}

						SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
					}
					else if(gnWorkType == 4)//一发一收
					{
						CurrentChannelNo = 1;
						memset(gszTextChannel, 0, sizeof(gszTextChannel));
						
						swprintf(gszTextChannel, _T("%d"), CurrentChannelNo+1);

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
					}
					else if(gnWorkType == 5)
					{
						
						memset(gszTextChannel, 0, sizeof(gszTextChannel));

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
					}
					else if(gnWorkType == 6)
					{
						SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)ShuangJingGuanLian);	

						memset(gszTextChannel, 0, sizeof(gszTextChannel));

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
					}
					
					gSysInfo = gAllcfgSysInfo;
					sys_reg->wLedBrightness = allParamLCDValue;
					
					if(gwDeviceType == 2)//铁路专用时，补偿增益跟参考增益一致  wangbingfu add at 2011-05-27
					{
						for(int i = 0; i < 2; i++)
						{
							gfTlRange[i][gwStandard[i]][gwPosition[i][gwStandard[i]]] = Display_AllParamConfigWith_ad[i].fRange;
							gfTlBuChangPlus[i][gwStandard[i]][gwPosition[i][gwStandard[i]]] = Receive_AllParamConfigWith_ad[i].fRefPlus;
							gfTlPlus[i][gwStandard[i]][gwPosition[i][gwStandard[i]]] = Receive_AllParamConfigWith_ad[i].fPlus;
						}
					}
					allCfgSaveParamFile(ParamFileName);
//					if(gnParamNo == currentUsedParamNo)


					currentUsedParamNo = gwGongYiCanShu;
					HKEY HKeyNum;
					LONG ret = RegOpenKeyEx(HKEY_CURRENT_USER, _T("ParNum"), 0, 0,&HKeyNum);
				
					if(ret != ERROR_SUCCESS)
					{
							MessageBox (Wnd, TEXT ("读注册表失败"), TEXT("NDT_CE"), 0) ;
						
						return -1 ;
					}
					
					RegSetValueEx(HKeyNum, _T("ParNum"), 0, REG_DWORD, (const BYTE *)&currentUsedParamNo, sizeof(DWORD));
				
					RegCloseKey(HKeyNum);
					
					swprintf(lpDefaultParamFile, _T("\\ResidentFlash\\%d.par"), currentUsedParamNo);
			
					ReadParamFile(ParamFileName);
					// 把参数发下去
					//InitParamDetail();
					InitParam();
			
					//改变曲线
					for(int i = 0; i < 2; i++)
					{
						if(fAllCfgOldDelay[i] - Receive_AllParamConfigWith_ad[i].fDelay != 0)
						{
							for(int j = 0; j < (int)gSelectedDgsPointDataNum[i][1]; j++)
							{
								for(int k = 0; k < DAC_CURVE_NUM; k++)
								{
									gSelectedDgsPointData[i][k][j].serial = curveChangeByDelay(gSelectedDgsPointData[i][k][j].serial, 
																								(fRangeStart[i] - gfAllOldRangeStart[i]), (gfAllOldRangeEnd[i] - gfAllOldRangeStart[i]));
								}
							}
							for(int j = 0; j < (int)gSelectedAvgPointDataNum[i]; j++)
							{
								for(int k = 0; k < AVG_CURVE_NUM + 1; k++)
								{
									gSelectedAvgPointData[i][k][j].serial = curveChangeByDelay(gSelectedAvgPointData[i][k][j].serial, 
																								(fRangeStart[i] - gfAllOldRangeStart[i]), (gfAllOldRangeEnd[i] - gfAllOldRangeStart[i]));
								}
							}
						}
						
						if(fAllCfgOldRange[i] - Display_AllParamConfigWith_ad[i].fRange != 0)
						{
							for(int j = 0; j < (int)gSelectedDgsPointDataNum[i][1]; j++)
							{
								for(int k = 0; k < DAC_CURVE_NUM; k++)
								{
									gSelectedDgsPointData[i][k][j].serial = (float)(gwOldDacSerial[i][j] 
																								* (gfOldDacRange[i] / Display_AllParamConfigWith_ad[i].fRange));
								}
							}
							for(int j = 0; j < (int)gSelectedAvgPointDataNum[i]; j++)
							{
								for(int k = 0; k < AVG_CURVE_NUM + 1; k++)
								{
									gSelectedAvgPointData[i][k][j].serial = (float)(gwOldAvgSerial[i][j] 
																								* (gfOldAvgRange[i] / Display_AllParamConfigWith_ad[i].fRange));
								}
							}
						}
						
						if(fAllCfgOldPlus[i] - Receive_AllParamConfigWith_ad[i].fPlus != 0)
						{
							for(int j = 0; j < (int)gSelectedDgsPointDataNum[i][1]; j++)
							{
								for(int k = 0; k < DAC_CURVE_NUM; k++)
								{
									gSelectedDgsPointData[i][k][j].data = curveDataChangeByGain(gSelectedDgsPointData[i][k][j].data, (Receive_AllParamConfigWith_ad[i].fPlus - fAllCfgOldPlus[i]));
								}
							}
							for(int j = 0; j < (int)gSelectedAvgPointDataNum[i]; j++)
							{
								for(int k = 0; k < AVG_CURVE_NUM + 1; k++)
								{
									gSelectedAvgPointData[i][k][j].data = curveDataChangeByGain(gSelectedAvgPointData[i][k][j].data, (Receive_AllParamConfigWith_ad[i].fPlus - fAllCfgOldPlus[i]));
								}
							}
						}
					}				
					allCfgSaveParamFile(ParamFileName);

				/*	if(gwGongYiCanShu == currentUsedParamNo)
					{
						ReadParamFile(ParamFileName);
						// 把参数发下去
						InitParam();
					}
					*/						
					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					ShowWindow(hStatusBar,SW_SHOW);

					DestroyWindow(hWndAllParamCfg);
					hWndAllParamCfg = 0;
					DestroyWindow(hWndAllParamCfgCaption);
					hWndAllParamCfgCaption = 0;
					DestroyWindow(hWndAllParamConfirm);
					hWndAllParamConfirm = 0;

					if(gbHotCraftParamView)
					{
						gbHotCraftParamView = FALSE;

						if(gbHotGateFlag)
						{
							ShowWindow(hWndGate,SW_SHOW);
							SetFocus(currentGateBtn);
						}
						else if(gbHotAutoGainFlag)
						{
							ShowWindow(hWndAutoGainPlus,SW_SHOW);
							SetFocus(currentAutoGainPlusBtn);
						}
						else if(gbHotAutoAdjustFlag)
						{
							ShowWindow(hWndAdjust,SW_SHOW);
							SetFocus(currentAdjustBtn);
						}
						else
						{
							if(gCurrentWnd)//有子菜单窗口
							{
								ShowWindow(gCurrentWnd, SW_SHOW);
								SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
							}
							else
							{
								SetFocus(hWndMain);//主窗口获得输入焦点
							}
						}
					}
					else
					{
						ShowWindow(hWndCraftParam,SW_SHOW);
						SetFocus(currentCraftParamBtn);//上级菜单最后选中的按钮获得输入焦点
					}

					SetRedrawFlag(TRUE);
					
					Sleep(30);
					SetRedrawFlag(TRUE);//再多刷新次，因有可能先画显存,系统后刷显示，导致白屏

					//wangbingfu add start at 2011-5-25
				/*	//wangbingfu delete at 2011-5-27
					if(gwDeviceType == 2 && gbTlParamFlag == TRUE)
					{
						setRailRoadParam(CurrentChannelNo, FALSE);//设置该标准该部位声程，基准增益，补偿增益
					}*/
					//wangbingfu add end at 2011-5-25
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamConfirmsaveProc,Wnd,Msg,wParam,lParam);
}
/**************************
*不保存按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAllParamConfirmNoSaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
				}
			
				else if(wParam == VK_LEFT)
				{
					while(allParamConfirmBtnList[i] != currentAllParamConfirmBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamConfirmBtn = allParamConfirmBtnList[i];
					SetFocus(currentAllParamConfirmBtn);
				}
				else if(wParam == VK_RIGHT)
				{
					while(allParamConfirmBtnList[i] != currentAllParamConfirmBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamConfirmBtn = allParamConfirmBtnList[i];
					SetFocus(currentAllParamConfirmBtn);
				}
				else if(wParam == VK_RETURN)
				{				
					ShowWindow(hStatusBar,SW_SHOW);

					DestroyWindow(hWndAllParamCfg);
					hWndAllParamCfg = 0;
					DestroyWindow(hWndAllParamCfgCaption);
					hWndAllParamCfgCaption = 0;
					DestroyWindow(hWndAllParamConfirm);
					hWndAllParamConfirm = 0;

					if(gbHotCraftParamView)
					{
						gbHotCraftParamView = FALSE;

						if(gbHotGateFlag)
						{
							ShowWindow(hWndGate,SW_SHOW);
							SetFocus(currentGateBtn);
						}
						else if(gbHotAutoGainFlag)
						{
							ShowWindow(hWndAutoGainPlus,SW_SHOW);
							SetFocus(currentAutoGainPlusBtn);
						}
						else if(gbHotAutoAdjustFlag)
						{
							ShowWindow(hWndAdjust,SW_SHOW);
							SetFocus(currentAdjustBtn);
						}
						else
						{
							if(gCurrentWnd)//有子菜单窗口
							{
								ShowWindow(gCurrentWnd, SW_SHOW);
								SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
							}
							else
							{
								SetFocus(hWndMain);//主窗口获得输入焦点
							}
						}
					}
					else
					{
						ShowWindow(hWndCraftParam,SW_SHOW);
						SetFocus(currentCraftParamBtn);//上级菜单最后选中的按钮获得输入焦点
					}

					InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);

					SetRedrawFlag(TRUE);

					Sleep(50);
					SetRedrawFlag(TRUE);//再多刷新次，因有可能先画显存,系统后刷显示，导致白屏
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamConfirmNoSaveProc,Wnd,Msg,wParam,lParam);
}
/**************************
*取消按钮自定义处理过程
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAllParamConfirmCancelProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
				}
			
				else if(wParam == VK_LEFT)
				{
					while(allParamConfirmBtnList[i] != currentAllParamConfirmBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAllParamConfirmBtn = allParamConfirmBtnList[i];
					SetFocus(currentAllParamConfirmBtn);
				}
				else if(wParam == VK_RIGHT)
				{
					while(allParamConfirmBtnList[i] != currentAllParamConfirmBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAllParamConfirmBtn = allParamConfirmBtnList[i];
					SetFocus(currentAllParamConfirmBtn);
				}
				else if(wParam == VK_RETURN)
				{
					DestroyWindow(hWndAllParamConfirm);
					hWndAllParamConfirm = 0;
					SetFocus(currentAllParamCfgBtn);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAllParamConfirmCancelProc,Wnd,Msg,wParam,lParam);
}
