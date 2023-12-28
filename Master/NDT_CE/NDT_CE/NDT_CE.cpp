// NDT_CE.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "Winsock2.h" //wireless 2011-05-31
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "ParamSetDlg.h"
#include "ParamSet2Dlg.h"
#include "tsMenuWin.h"
#include "fnMenuWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "Aygshell.h"
//#include "bsp.h"
#include "gateWin.h"
#include "AllParamSetDlg.h"

#include "craftParamWin.h"
#include "fnSpecialDisplayWin.h"
#include "displayWin.h"
#include "recvParamWin.h"
#include "sendParamWin.h"
#include "gateWin.h"	//wangbingfu add at 2010-11-29
#include <vector>
#include <Dbt.h>	//wangbingfu add at 2010-11-18
#include "fnBscanWin.h"
#include "netCommand.h"
#include "otherConfigureWin.h"
/*包含在s3c2440a.h,bsp.h又包含s3c2440a.h
#include "s3c2440a_ioport.h"	//wangbingfu add at 2011-07-07
#include "s3c2440a_base_regs.h"	//wangbingfu add at 2011-07-07
#include "s3c2440a_clkpwr.h"	//wangbingfu add at 2011-07-07
*/
#include "Ndis.h"	//网卡 IOCTL_NDIS_REBIND_ADAPTER
#include <stdlib.h>
#include <Winbase.h>
#include "TcpServer.h"
#include "TcpMonitor.h"
#include "TcpCScan.h"
#include "TcpACScan.h"
#include "TcpParam.h"
#include "IniParse.h"
#include "IniFile.h"
// #include "zlib.h"
// #include "zconf.h"
// #include "TcpParamString.h"
#include "SoftwareVersion.inc"

#include "lua.hpp"

using namespace std;

#define FRAME_COUNT_DISP 1
#define MAX_LOADSTRING 100

//wireless

BOOL gbChannelSend[CHANNEL_NUM] = {FALSE, FALSE};
BOOL gbAscanDataStoreSend = FALSE;
BOOL gbChannelOk[CHANNEL_NUM];
CRITICAL_SECTION gCs;
//SOCKET sockList[MAX_SOCKETS + 1];
//SOCKET gSockClient
SOCKET gSockClient2;
SOCKET gSockClient3;
vector<SOCKET> gVecSockMonitor;
vector<SOCKET> gVecSockCScan;
vector<SOCKET> gVecSockParam;
vector<SOCKET> gVecSockACScan;
vector<SOCKET> gVecSockParamString;
//wireless

// Global Variables:

HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// The title bar text

// wangbingfu add start at 2010-04-19
HWND hWndMain;
extern HWND hWndTsMenu;	//探伤菜单窗口句柄（一级菜单）
extern HWND hWndFnMenu;//Fn功能窗口句柄
extern HWND gCurrentFocusBtn;
// wangbingfu add end at 2010-04-19
// wangbingfu add at 2010-07-21
extern HWND hWndChMenu;
// wangbingfu add at 2010-07-21
// songchenguang add start
HMENU hMainMenu = NULL;

DWORD ARS500_Server(LPVOID);

const struct decodeUINT MainMessages[] = {
	WM_CREATE, DoCreateMain,
	WM_PAINT, DoPaintMain,
	WM_COMMAND, DoCommandMain,
	WM_DESTROY, DoDestoryMain,
	WM_KEYUP, DoKeysMain,
	WM_KEYDOWN, DoKeysMain,
	WM_CHAR, DoKeysMain,
	WM_RBUTTONUP, DoMouseMain,
	WM_DRAWITEM, DoDrawItem,
	WM_HOTKEY, DoHotKey,	//wangbingfu add at 2010-12-08
};

const struct decodeCMD MainCommandItems[] = {
	ID_PARAM_SET, OnParamSet,
	IDC_PARAM_SET2, OnParamSet2,
	ID_START, OnStart,
	ID_STOP, OnStop,
	IDC_ALAM_OPEN, OnAlarmOpen,
	IDC_ALAM_CLOSE, OnAlarmClose,
	IDC_SEND_SPI_DELAY, OnSendSPIDelay,
//	IDC_SEND_PARAM, OnSendParam,
	IDC_SET_DISPLAY, OnDisplaySet,
};



DRAWTHREAD_PARAM DrawThread_param;		// 画图线程参数
AECTHREAD_PARAM thread_param;			// 工作线程参数
BitBltTHREAD_PARAM BitBltThread_Param;	// 拷屏线程函数

DWORD dwDrawThreadID = 0;				// 画图线程ID
DWORD dwBitBltThreadID = 0;				// 拷屏线程ID
DWORD dwPutsThreadID = 0;				// 卡线程ID
DWORD dwSocketThreadID = 0;				// 通讯线程ID
DWORD dwSocket2ThreadID = 0;				// 通讯线程ID
DWORD dwSocket3ThreadID = 0;				// 参数下发通讯线程ID
DWORD dwSocket4ThreadID = 0;				// 发送A扫存储数据通讯线程ID

HANDLE hDrawThread = NULL;				// 画图线程句柄
HANDLE hDrawEvent = NULL;				// 画图事件
HANDLE hWorkEvent = NULL;
BOOL bTerminated = FALSE;				// 线程结束标志
HANDLE hSocketThread = NULL;			// 通信线程句柄wireless 2011-5-30
HANDLE hSocketThread2 = NULL;			// 通信线程句柄wireless 2011-5-30
HANDLE hSocketThread3 = NULL;			// 参数下发通信线程句柄 yachang.huang 2013-11-13
HANDLE hSocketThread4 = NULL;			// A扫存储数据通信线程句柄 yachang.huang 2013-12-19


HANDLE hPutsThread = NULL;				// 工作线程句柄
PIC_DATA gPicData[2];					// 图像数据
HANDLE hWorkMutex;						// 工作互斥器				

BOOL gbInitFlag = FALSE;				// 卡线程初始化标志
BOOL gbStartFlag = FALSE;				// 卡线程运行标志
BOOL gbEndFlag = FALSE;					// 卡线程结束标志
BOOL gbSetOneFlag = FALSE;				// 卡线程发操作命令标志
WORD WaveSpeed = VSOUND;				// 声速

// songchenguang add end 2011-06-22
WORD MAX_PLUS = 110;
// songchenguang add end 2011-06-22
// 发送参数对话框参数
extern SEND_PARAM_DATA SendParam_ad[2];

// 接收参数对话框参数
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];

extern CRITICAL_SECTION csRefWave;
extern BOOL g_bDisplayRefWave;

//wangbingfu add 2010-06-04
BOOL gTcgClearFlag = TRUE;
extern BOOL gWaveDrawTcgFlag;
extern CURVEPOINT_DATA gSelectedDacPointData[CHANNEL_NUM][MAX_CURVE_POINT_NUM];
extern DWORD gSelectedDacPointDataNum[CHANNEL_NUM];
extern TCHAR FileDirectory[MAX_PATH];
extern DWORD pictureNum;
//wangbingfu add 2010-06-04
//wangbingfu add 2010-06-18
BOOL gDacClearFlag = TRUE;
extern BOOL gWaveDrawDacFlag;		//波形是否画DAC
extern BOOL gWaveDrawAvgFlag;		//波形是否画AVG wangbingfu add at 2010-12-02

extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];
//wangbingfu add strat at 2010-12-02
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//添加的样点数据队列
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//已选中的AVG点数量
//wangbingfu add end at 2010-12-02

//wangbingfu add 2010-06-18
BOOL gAvgClearFlag = TRUE;	//wangbingfu add at 2010-12-02
// wangbingfu add  2010-07-08
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];
BOOL gDgsGateDrawFlag[CHANNEL_NUM];	//是否有画DGS闸门标志
// wangbingfu add end 2010-07-08

// wangbingfu add end 2010-07-19
WORD gwDeviceType =  1;	//设备类型 0:探伤仪	1:测厚仪 2:铁路专用
// wangbingfu add end 2010-07-19
// wangbingfu add  2010-07-22
WORD gwWaveFileNum = 2;	//test 读取WAVE文件
// wangbingfu add end 2010-07-22
// wangbingfu add  2010-8-23
BOOL gbAlarmRegFlag[CHANNEL_NUM][GATE_NUM];		//通道报警标志
BOOL gbChannelAlarmFlag[CHANNEL_NUM];			//闸门报警标志
CRITICAL_SECTION csAlarmFlag;
// wangbingfu add end 2010-8-23

// wangbingfu add  2010-07-26
extern HWND hWndChDisplay;
// wangbingfu add  end 2010-07-26
extern WORD  wGateIndex;		//闸门索引//wangbingfu add start at 2010-8-6
POINT oldPointLeftBottom= {0, 0};
POINT oldPointLeftTop = {0, 0};
POINT oldPointRightTop= {0, 0};
POINT oldPointRightBottom= {0, 0};
CURVEPOINT_DATA oldSelectedDgsPointData[CHANNEL_NUM][GATE_NUM][MAX_CURVE_POINT_NUM];
DWORD oldSelectedDgsPointDataNum[CHANNEL_NUM][GATE_NUM] = {{0,0,0},{0,0,0}};

BOOL gDacChangeFlag = FALSE; //DAC曲线位置是否改变标志



static BOOL bTopState = TRUE;
//WORD nGateColor[2][3] = {63488, 2016, 31, 31743, 64495, 31727}; //闸门颜色
WORD nGateColor[2][3] = {RGB888toRGB565(255,0,0), RGB888toRGB565(0,255,0), RGB888toRGB565(0,0,255), 
						 RGB888toRGB565(255,255,0), RGB888toRGB565(255,0,255), RGB888toRGB565(0,255,255)}; //闸门颜色
//wangbingfu add start at 2010-8-2
WORD nDacColor[CHANNEL_NUM][3] = {{	RGB888toRGB565(119, 190, 93),  //通道1评定线，灰(192,192,192)
								   	RGB888toRGB565(221, 19, 123), //通道1合格线，绿(0,255,0)
								   	RGB888toRGB565(0, 147, 221),//通道1判废线，红(255,0,0)
									},
									{
										RGB888toRGB565(0, 109, 57),  //通道2评定线，灰
										RGB888toRGB565(197, 168, 204),  //通道2合格线，绿
										RGB888toRGB565(42, 170, 170),  //通道2判废线，红
									}
					};//TCG曲线颜色
					
WORD wAvgColor[CHANNEL_NUM][3] = {{	RGB888toRGB565(0, 76, 131),  //通道1Φ1红
									RGB888toRGB565(179, 39, 116), //通道1Φ2绿
									RGB888toRGB565(226, 189, 78),//通道1Φ3蓝	
									},
									{
										RGB888toRGB565(218, 37, 29),  //通道2Φ1红
										RGB888toRGB565(132, 194, 37), //通道2Φ2绿
										RGB888toRGB565(131, 200, 196),//通道2Φ3蓝
									}
					};//AVG曲线颜色


//wangbingfu add end at 2010-8-2
HWND gCurrentWnd = NULL;	//当前显示的子窗口wangbingfu add at 2010-8-9
BOOL  gbHotGateFlag = FALSE;	//是否打开闸门快捷方式 wangbingfu add at 2010-8-9
BOOL  gbHotCraftParamFlag = FALSE;	//是否打开工艺参数快捷方式 
BOOL  gbHotAutoGainFlag = FALSE;	//是否打开自动增益快捷方式 
BOOL  gbHotAutoAdjustFlag = FALSE;	//是否打开自动校准快捷方式 

BOOL gbAlarmFlag = TRUE;	 			//蜂鸣

extern HWND currentGateBtn;			// 闸门设定窗中当前选中的按钮wangbingfu add at 2010-8-10
extern HWND currentCraftParamBtn;

extern HWND hWndGate;	//闸门窗口
extern HWND hWndCraftParam;//工艺参数窗口句柄
extern HWND hWndAutoGainPlus;//自动增益窗口句柄
extern HWND currentAutoGainPlusBtn;//自动增益窗中当前选中的按钮
extern HWND hWndAdjust;						//校准窗口句柄
extern HWND currentAdjustBtn;					//校准窗中当前选中的按钮

HMODULE hRes = NULL;					// 资源句柄
HWND hStatusBar = NULL;					// 状态栏句柄

WORD CurrentChannelNo = 0;				// 当前通道号
static WORD gPressValue = 6;			// 电压值

float fRangeStart[2] = {0.0f, 0.0f};
float fRangeEnd[2] = {MIN_RANGE, MIN_RANGE};

float gCh1Plus = 60;
float gCh2Plus = 0;

float gCh1Range = Dot2Range(DisplaySet_ad[0].nWaveSpeed, MONITOR_SIZE);
float gCh2Range = Dot2Range(DisplaySet_ad[1].nWaveSpeed, MONITOR_SIZE);


wchar_t gszTextChannel[10];
wchar_t gszTextStep[10];
wchar_t gszTextPlus[10];
wchar_t gszTextRefPlus[10];
wchar_t gszTextTime[20];
wchar_t gszTextPeak[20];
wchar_t gszInfo[2][2][30];

PEAK_INFO gPeakInfos[2][2];
PEAK_INFO gPeakInfosRemeber[2][2];
PEAK_INFO gPeakDefectInfos[2][2];	//保存闸门峰值点信息 wangbingfu add at 2010-8-26
CRITICAL_SECTION csPeakInfo;
extern BOOL gbAdjustStart;
extern BOOL gbAvgPeakFinished;
extern CRITICAL_SECTION csAdjust;
extern BOOL gbPeakRememberFlag;  	//峰值记忆是否开启
extern CRITICAL_SECTION csPeakRemember;
extern BOOL gbAngleAdjustStart;		//角度校准标志wangbingfu add at 2010-11-26

// songchenguang add start 2010-12-24
extern float gfPlateHeight;	// 中厚板探伤工件厚度
// songchenguang add end 2010-12-24
	
// songchenguang add end
//extern
extern WORD *ch1_interval;
extern WORD *ch2_interval;
extern WORD *ch1_data;
extern WORD *ch2_data;
extern ARS200FPGA_SYS_REG* sys_reg;
extern WORD *arm_selftest;
extern ARS200FPGA_CH_DATA_REG *ch1_data_reg;
extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
extern ARS200FPGA_CH_CON_REG *ch1_con_reg;
extern ARS200FPGA_CH_DATA_REG *ch2_data_reg;
extern ARS200FPGA_CH_GATE_REG *ch2_gate_reg;
extern ARS200FPGA_CH_CON_REG *ch2_con_reg;
extern WORD *ch1_tgc;
extern WORD *ch2_tgc;
extern GATE_INFO GateInfos[2][3];
extern GATE_INFO stOldGateInfos[2][3];		//wangbingfu add at 2010-11-22
extern CRITICAL_SECTION csGate;

extern ARS200FPGA_CH_PLUS_REG* ch1_plus_reg;
extern ARS200FPGA_CH_PLUS_REG* ch2_plus_reg;
extern ARS200FPGA_CH_DAC_REG* ch1_dac_reg;
extern ARS200FPGA_CH_DAC_REG* ch2_dac_reg;

extern WORD *ch1_gate1curve;
extern WORD *ch2_gate1curve;
extern WORD *ch1_gate2curve;
extern WORD *ch2_gate2curve;
extern WORD *ch1_gate3curve;
extern WORD *ch2_gate3curve;
extern WORD *lamp;

// songchenguang add start 2010-06-07
WORD preDrawData[CHANNEL_NUM][DATA_SIZE];
static LONG gnDrawCount = 0;

extern BOOL gbBaoluo;	// 包络打开标志
BOOL gbHotRange = FALSE;	// 快捷键声程
BOOL gbHotDB = FALSE;		// 快捷键增益
static WORD gDBIndex = 0;	// 增益序号(1 基准增益 2 参考增益)
extern float fRangeStep;
extern float fPlusStep;
extern WORD nRangeStepIndex;
extern WORD nPlusStepIndex;
extern BOOL gb110Display;
extern CRITICAL_SECTION cs110Display;
extern CRITICAL_SECTION csRefWaveRange;
extern float gfRefWaveStart;
extern float gfRefWaveEnd;
//songchenguang add start 2010-12-02
extern WORD wTanTouLeiXing[CHANNEL_NUM];
extern CRITICAL_SECTION csProbeType;

extern WORD*  ch1_opa3692_simulate_switch;
extern WORD*  ch1_lmh6720_sd;
extern WORD*  ch1_associate_detection;
extern WORD*  ch1_wave_filter;
extern WORD*  ch1_wave_filter_reset;
extern WORD*  ch1_wave_filter_control;

extern WORD*  ch2_opa3692_simulate_switch;
extern WORD*  ch2_lmh6720_sd;
extern WORD*  ch2_associate_detection;
extern WORD*  ch2_wave_filter;
extern WORD*  ch2_wave_filter_reset;
extern WORD*  ch2_wave_filter_control;

BOOL gbHotCraftParamView = FALSE;
//songchenguang add end 2010-12-02
SYS_INFO gSysInfo;
wchar_t lpDefaultParamFile[MAX_PATH];
// songchenguang add end 2010-06-07

ARS200FPGA_CH_DATA_REG *chCurrent_data_reg = NULL;
ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg = NULL;
ARS200FPGA_CH_CON_REG *chCurrent_con_reg = NULL;
ARS200FPGA_CH_PLUS_REG* chCurrent_plus_reg;
ARS200FPGA_CH_DAC_REG* chCurrent_dac_reg;

//wangbf add start 2010-06-17
WORD *chCurrent_tgc = NULL;
//wangbf add end 2010-06-17

WORD *chCurrent_interval = NULL;
WORD *chCurrent_data = NULL;

WORD *chCurrent_gate1curve = NULL;
WORD *chCurrent_gate2curve = NULL;
WORD *chCurrent_gate3curve = NULL;

BOOL g_bMenuShown = FALSE;

BOOL g_bRedraw = FALSE;		// 是否重画
//volatile S3C2440A_LCD_REG *v_pIOPregs;

CRITICAL_SECTION csStop;
CRITICAL_SECTION csMenuState;
CRITICAL_SECTION csCommand;
CRITICAL_SECTION csRange;
CRITICAL_SECTION csRedraw;
CRITICAL_SECTION csBaoluo;
CRITICAL_SECTION csHotRange;
CRITICAL_SECTION csSysInfo;
CRITICAL_SECTION csTextInfo;
CRITICAL_SECTION csDisplayParam;
CRITICAL_SECTION csSendParam;
CRITICAL_SECTION csPeakDefectInfos;


float gfMinRange[2] = {MIN_RANGE, MIN_RANGE};	// 最小声程
CRITICAL_SECTION csMinRange;

extern WORD gRefWaveData[DATA_SIZE];

int gCurrentSysColorIndex = 0;
BOOL gbInitializeSysColor = TRUE;
CRITICAL_SECTION csSysColor;
SYS_COLOR_INFO gstSysColor[SYS_COLOR_NUM];		//wangbingfu add at 2010-10-21
//DWORD gParamIndex = 1;
extern DWORD currentUsedParamNo;
extern BOOL gbZhanKuan;
extern float gZhanKuanFront;
extern float gZhanKuanBack;
extern CRITICAL_SECTION csZhanKuan;
extern CRITICAL_SECTION csZhanKuanValue;
extern CRITICAL_SECTION csHuiBoState;
extern BOOL gbHuiBoOpen[10];
//extern float gfHuiBoWidth;
extern CRITICAL_SECTION csHanFengTanShang;
extern BOOL bFreezeUp;
extern BOOL gbBscanSwitch;
extern BOOL bHanFengTanShangSwitch;				//焊缝探伤开关

extern WORD nAddStep;
extern WORD nAllPeakPointDataNum;
extern float fBScanGangBanHouDu;
extern CRITICAL_SECTION csBScan;
extern BOOL gbGuanLian;

extern HWND hBtnDisplaySoundDistance;
extern HWND hWndDisplay;
extern HWND hWndRecvParam;
extern HWND hBtnRecvParamGain;

extern HWND hBtnRecvParamReferenceGain;
extern HWND hBtnRecvParamImpedance;		//wangbingfu add at 2010-09-08
// songchenguang add start 2010-10-13
extern ARS200FUN_DEFINE* Sys_Function_Def;
extern ARS200VER* Sys_Hardware_Ver;
DWORD gFunctionDefine = 0xfffffff;  //0xFFFFFFFF; //= 0;
float HARDWARE_VER = 0.0f;
// songchenguang add end 2010-10-13
// songchenguang add start 2010-12-07
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG曲线开关标志
extern BOOL gbQuMianXiuZhengFlag;
extern float gfWaiJing;				//管材外径
extern float gfNeiJing;				//管材内径
extern HWND hWndAllParamCfgCaption;	//工艺参数配置标题窗口句柄
extern HWND hWndAllParamCfg;		//工艺参数配置窗口句柄
extern HWND hWndSpecialDisplay;			//Fn特殊显示窗口句柄

// songchenguang add end 2010-12-07

extern WORD* Sys_device_type;
WORD gwDeviceTypeValue = 0;	// wangbingfu add at 2010-11-11
// wangbingfu add start 2010-10-14

extern WORD gwDeleteStep;		//删样点位置	wangbingfu add at 2010-12-03
extern BOOL gbDeletePointFlag;		//是否在删样点操作 wangbingfu add at 2010-12-03
BOOL gbAvgDrawFlag[CHANNEL_NUM] = {FALSE, FALSE};	//是否有画AVG曲线标志wangbingfu add at 2010-12-06
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG曲线开关标志wangbingfu add at 2010-12-06
BOOL bgAvgChangeFlag = FALSE;						////AVG曲线位置是否改变标志
extern BOOL bgAvgChangeFlag;
extern float gfOldAvgRange[CHANNEL_NUM];				//最初生成AVG时的声程
extern short gwOldAvgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//最初生成AVG时的每个点的位置
extern float gfDefectZhi1[CHANNEL_NUM];			//所测孔径Φ1
extern float gfDefectZhi2[CHANNEL_NUM];			//Φ2
extern float gfDefectZhi3[CHANNEL_NUM];			//Φ3


extern float gfAdjustZero;				//校准的零点
extern float gfAdjustZeroMin;			//最小试块校准的零点
extern WORD gwAdjustWaveSpeed;			//校准后的声速
extern BOOL gbChZeroDoubleStart;		//双点法零点校准是否进行中	
extern BOOL gbChZeroDoubleMinStart;		//双点法最小试块零点校准是否进行中	
extern BOOL gbChZeroSingleStart;		//单点法零点校准是否进行中
extern BOOL gbChWaveSpeedAdjustStart;	//声速校准是否进行中

extern float fMinThickness;				//试块厚度(min)
extern float fMaxThickness ;			//试块厚度(max)
extern float gfZeroThickness;			//零点校准试块厚度
extern float gfSpeedAdjustThickness;	//声速校准试块厚度
extern HWND currentChZeroSingleBtn;		//测厚单点法零点校准窗口当前选中的按钮
extern HWND currentChZeroDoubleBtn;		//测厚两点法零点校准窗口当前选中的按钮
extern HWND currentChWaveSpeedAdjustBtn;		//测厚声速校准窗口当前选中的按钮


extern BOOL gbMeasureSwitch;	//是否在测量

// wangbingfu add at 2010-10-19
extern BOOL gbDrawHeadVCurveFlag;					//是否在画探头V型补偿曲线
// wangbingfu end at 2010-10-19
// wangbingfu add at 2010-10-25
extern BOOL gbDrawTempCurveCurveFlag;				//是否在画温度补偿曲线
// wangbingfu end at 2010-10-25
extern BOOL gbReplayFlag;					//是否有开启回放wangbingfu end at 2010-11-10
extern HWND hBtnSpecialDisplayFreeze;		//冻结按扭
extern HWND hWndCreateDacCurve;//新建DAC曲线窗口句柄wangbingfu end at 2010-12-27
extern HWND hWndCreateAvgCurve;//新建DAC曲线窗口句柄wangbingfu end at 2010-12-27


extern float gfOldDacRange[CHANNEL_NUM];				//最初生成DAC时的声程
extern short gwOldDacSerial[GATE_NUM][MAX_CURVE_POINT_NUM];	//最初生成DAC时的每个点的位置

extern float gfOldTcgRange[CHANNEL_NUM];				//最初生成TCG时的声程
extern short gwOldTcgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//最初生成TCG时的每个点的位置

extern HWND hBtnZhongHouBanTanShangSensitive;		//中厚板检测灵敏度按钮句柄
extern HWND hBtnFuHeCaiLiaoJianCeSensitive;		//复合材料检测灵敏度按钮句柄
extern HWND hBtnZhuJianSensitive;				//铸件检测灵敏度按钮句柄

extern float gfDacFirstPointChangeGain[CHANNEL_NUM];			//DAC制作时增益键改变的增益值
extern float gfAvgFirstPointChangeGain[CHANNEL_NUM];			//AVG制作时增益键改变的增益值

extern HWND hWndTlMenu;//铁路专用菜单窗口句柄wangbingfu end at 2011-1-19
//wangbingfu add start at 2011-2-24
extern WORD gwStandard[CHANNEL_NUM];	//检测标准
extern WORD gwPosition[CHANNEL_NUM][TL_STANDARD_NUM];	//检测部位
extern float gfTlBuChangPlus[CHANNEL_NUM][TL_STANDARD_NUM][TL_POSITION_NUM];		//各标准各部位补偿增益
extern HWND hWndTlAdjust;					//调校窗口句柄
extern HWND hBtnTlAdjustBiaoMianBuChang;	//表面补偿调校窗口按钮句柄
extern void TlBiaoMianBuChangDisplay(HWND hWnd, float fPlusBuChang);
//wangbingfu add end at 2011-2-24
extern CURVEPOINT_DATA gOldMakeDgsPointData[MAX_CURVE_POINT_NUM];//以加第一点基准增益为基准的制作线wangbingfu add at 2011-4-1
extern CURVEPOINT_DATA gOldMakeAvgPointData[MAX_CURVE_POINT_NUM];//以加第一点基准增益为基准的制作线wangbingfu add at 2011-4-1

extern BOOL gbTlParamFlag;	//当前是否使用铁路参数	//wangbingfu add at 2011-5-25

extern float MIN_DELAY[2];
extern float MAX_DELAY;

extern void displayGateFault(double gateDefectAmp, double gateDefectTime, float fFuZhi);
extern void AutoJustParam(HWND hWnd);
extern void angleAdjust();				//wangbing add at 2010-11-29
extern void SetFnRefPlusLableAndExecute(HWND hWnd);
extern void initRailRoadParam();

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

static HANDLE hFileWrite;	//写文件句柄

//wireless start at 2011-05-31
int gnSendCount = 0;			//发送数据计数
int gnSendCount1 = 0;			//发送数据计数
int gnTotalSendBytes = 0;
PEAK_INFO gstConvertPeakInfo[2][2];		//转换后的伤时伤幅
extern HWND hBtnSendParamVoltage;		//电压按钮句柄
extern HWND hBtnSendParamMaiKuan;		//脉宽方式按钮句柄
extern HWND hBtnSendParamRepeatFreq;	//重复频率按钮句柄
extern HWND hBtnSendParamModify;		//修正按钮句柄
extern HWND hBtnSendParamHeadQianYan;	//探头前沿按钮句柄
extern HWND hBtnRecvParamWaveMode;		// 检波方式按钮句柄
extern HWND hBtnRecvParamGain;			// 增益按钮句柄
extern HWND hBtnRecvParamDelay;			// 延迟按钮句柄
extern HWND hBtnDisplaySoundDistance;	//声程按钮句柄
extern HWND hBtnWaveSpeedValue;			//声速按钮句柄
extern HWND hBtnDisplayRestraint;		//抑制按钮句柄
extern HWND hBtnGateAlarmMode;			// 报警模式选择按钮句柄
extern HWND hBtnGateSwitch;				// 开关按钮句柄
extern HWND hBtnGateQiDian;				// 起点按钮句柄
extern HWND hBtnGateKuanDu;				// 宽度按钮句柄
extern HWND hBtnGateFuZhi;				// 幅值按钮句柄
extern HWND hBtnOtherCfgPrint;			//打印配置按钮句柄
extern HWND hBtnOtherCfgLcd;			//背光设置按钮句柄
extern HWND hBtnOtherCfgVolume;			//声音设置按钮句柄
extern HWND hBtnOtherCfgStoreMode;		//当前存储介质按钮句柄
extern HWND hBtnRecvParamWaveBand;		// 滤波频带按钮句柄
extern HWND hBtnDisplayRefractMode;		//折射角模式
extern HWND hBtnRefractAngleKValue;		//K值按钮句柄
extern HWND hBtnRefractAngleJiaoDu;		//角度按钮句柄

extern HWND hWndMode;//模式窗口句柄
extern HWND hBtnMode1or2;//单通道按钮句柄
extern HWND hBtnMode1plus2;//双通道按钮句柄
extern HWND hBtnMode1to2;//一发一收按钮句柄
extern HWND hBtnModeDanjing;//单晶关联按钮句柄
extern HWND hBtnModeShuangjing;//双晶关联按钮句柄

extern HWND hBtnParamSend;		//发射参数按钮句柄
extern HWND hBtnParamRecv;		//接收参数按钮句柄
extern HWND hBtnParamDisplay;	//显示设置按钮句柄
extern HWND hBtnParamCurve;	//曲线按钮句柄
extern HWND hBtnParamGate;		//闸门设置按钮句柄


extern BOOL nLCDValue;//仪器亮度
extern int gnVolumn;
extern WORD gwMaterialType;				//材料类型
extern WORD gwStoreType;				//存储类型
extern BOOL gbUSB;						// USB是否有存储器标志
extern BOOL gbSD;						// SD卡槽是否有存储器标志

static float fKValue = 0;					//K值

extern void ChangeWorkMode(WORKMODE emWorkMode, BOOL bSaveFlag);	//切换工作模式
extern void SetWaveSpeedLabelAndExecute(HWND hWnd);
extern WORD typeToWaveSpeed(WORD wType);
extern void displayKValue(HWND hWnd, float fKValue);
extern void displayAngle(HWND hWnd, float fAngle);
extern void SetAngleLabelAndExecute();

extern ARS200FPGA_SYS_REG* cscan_sys_reg;
extern CSCAN_ARS200FPGA_CODE_REG* cscan_code_reg;

extern CSCAN_ARS200FPGA_CH_CON_REG *cscan_ch1_con_reg;
extern CSCAN_ARS200FPGA_CH_CON_REG *cscan_ch2_con_reg;

extern CSCAN_ARS200FPGA_CH_CONFIG_REG *cscan_ch1_config_reg;
extern CSCAN_ARS200FPGA_CH_CONFIG_REG *cscan_ch2_config_reg;

extern CSCAN_ARS200FPGA_CH_GATE_REG *cscan_ch1_gate_reg;
extern CSCAN_ARS200FPGA_CH_GATE_REG *cscan_ch2_gate_reg;

extern CSCAN_ARS200FPGA_CH_FEATURE_REG *cscan_ch1_feature_reg;
extern CSCAN_ARS200FPGA_CH_FEATURE_REG *cscan_ch2_feature_reg;

extern CSCAN_ARS200FPGA_CH_DATA_REG *cscan_ch1_data_reg;
extern CSCAN_ARS200FPGA_CH_DATA_REG *cscan_ch2_data_reg;
extern WORD *cscan_ch1_interval;
extern WORD *cscan_ch2_interval;
extern WORD *cscan_ch1_data;
extern WORD *cscan_ch2_data;
extern WORD *cscan_ch1_tgc;
extern WORD *cscan_ch2_tgc;
extern WORD *cscan_ch1_dac_reg;
extern WORD *cscan_ch2_dac_reg;

extern WORD *cscan_ch1_sampling_interval_tgc;
extern WORD *cscan_ch2_sampling_interval_tgc;

extern WORD *cscan_arm_selftest;
extern WORD *cscan_ch1_feature_buffer_flag;
extern WORD *cscan_ch2_feature_buffer_flag;
extern WORD *cscan_ch1_feature_buffer1;
extern WORD *cscan_ch1_feature_buffer2;
extern WORD *cscan_ch2_feature_buffer1;
extern WORD *cscan_ch2_feature_buffer2;

extern WORD* cscan_ch1_monitor_flag;
extern WORD* cscan_ch2_monitor_flag;

extern WORD *cscan_ch1_ascan_data_store_monitor_flag;
extern WORD *cscan_ch2_ascan_data_store_monitor_flag;
extern WORD *cscan_ch1_ascan_data_store_reset;
extern WORD *cscan_ch2_ascan_data_store_reset;
extern WORD *cscan_ch1_ascan_data_store_buffer_flag;
extern WORD *cscan_ch2_ascan_data_store_buffer_flag;
extern WORD *cscan_ch1_ascan_data_store_buffer1;
extern WORD *cscan_ch1_ascan_data_store_buffer2;
extern WORD *cscan_ch2_ascan_data_store_buffer1;
extern WORD *cscan_ch2_ascan_data_store_buffer2;

const float CODER_PER_RING_XY = 500.0f;	//10000 * 25.0f / 264.0f / 4.0f;
const float CODER_PER_RING_Z = 500.0f;	//10000 * 25.0f / 155.0f / 4.0f;

//wireless end at 2011-05-31

volatile unsigned int *pGPHCON = NULL;	//Port H control
//volatile S3C2440A_IOPORT_REG *g_pIOPregs		= NULL;		// GPIO registers (needed to enable I2S and SPI)
//volatile S3C2440A_CLKPWR_REG *g_pCLKPWRreg	= NULL;		// Clock power registers (needed to enable I2S and SPI clocks)

CSCAN_CH_SEND_MONITOR_DATA_t gstMonitorData[CHANNEL_NUM];
int g_beginTime = 0;
CSCAN_ARS200FPGA_SETONE gstSetOne[CHANNEL_NUM];
WORD gwCscanWorkMode = 2;

//1X11为开头为第一频带标识的增益补偿，往后以此类推；1X11为第一频带第一通道的第一补偿，以此类推
float gfX11[8] = {0.0f};
float gfX12[8] = {0.0f};
float gfX21[8] = {0.0f};
float gfX22[8] = {0.0f};
float gMidValue[CHANNEL_NUM] = {511};// 中值

int WINAPI WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR     lpCmdLine,
                     int       nCmdShow)
{
 	
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_NDT_CE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	registerAll(hInstance);//注册各个窗口

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	RegisterHotKey(hWndMain, 1001, NULL ,'8');//wangbingfu added at 2010-12-08
	RegisterHotKey(hWndMain, 1002, NULL ,'A');//wangbingfu added at 2010-12-08
	
	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_NDT_CE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASS wc;

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

   hInst = hInstance; // Store instance handle in our global variable

	hWndMain = CreateWindow(szWindowClass, szTitle, WS_VISIBLE | WS_SYSMENU,
      0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);


   if (!hWndMain)
   {
      return FALSE;
   }

   ShowWindow(hWndMain, nCmdShow);
   UpdateWindow(hWndMain);
	
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;


	for(i=0; i<dim(MainMessages); i++)
	{
		if(message == MainMessages[i].Code)
			return (*MainMessages[i].Fxn)(hWnd, message, wParam, lParam);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

int  get_battery()
{
	return 0;

}

void GetParamFormIni()
{
	/*	CIniParse iniParse;
	TCHAR strFolder2[MAX_PATH] = _T("PipeSet.ini");
	TCHAR strFolder[MAX_PATH];
	//GetWorkingFolder(strFolder, MAX_PATH);
	wchar_t strFolder1[MAX_PATH];
	GetCurPath(strFolder);
	swprintf(strFolder1, _T("%s\%s"), strFolder, _T("PipeSet.ini"));
	//strcat(strFolder, _T("PipeSet.int"));
	//	MessageBox(hWnd, strFolder1, _T("info"), MB_OK);
	TSTRING strSection = _T("GroupSelect");
	TSTRING strKey = _T("G11");
	int nAngle = iniParse.GetPrivateProfileInt(strSection, strKey);
*/
	TCHAR strFolder[MAX_PATH];
	wchar_t strFilePath[MAX_PATH];
	GetCurPath(strFolder);
	swprintf(strFilePath, _T("%s\%s"), strFolder, _T("Param.ini"));
	//_T("//本地硬盘//Documents and Settings//Param.ini"));
	
	//1X11为开头为第一频带标识的增益补偿，往后以此类推；1X11为第一频带第一通道的第一补偿，以此类推
 	wchar_t strKey11[MAX_PATH];
 	wchar_t strKey12[MAX_PATH];
 	wchar_t strKey21[MAX_PATH];
 	wchar_t strKey22[MAX_PATH];

	for(int i = 0; i < 8; i++)
	{
		swprintf(strKey11, _T("%d%s"), i+1, _T("X11"));
		swprintf(strKey12, _T("%d%s"), i+1, _T("X12"));
		swprintf(strKey21, _T("%d%s"), i+1, _T("X21"));
		swprintf(strKey22, _T("%d%s"), i+1, _T("X22"));
		gfX11[i] = CeGetPrivateProfileFloat(_T("Gain"), strKey11, -1, strFilePath);
		gfX12[i] = CeGetPrivateProfileFloat(_T("Gain"), strKey12, -1, strFilePath);
		gfX21[i] = CeGetPrivateProfileFloat(_T("Gain"), strKey21, -1, strFilePath);
		gfX22[i] = CeGetPrivateProfileFloat(_T("Gain"), strKey22, -1, strFilePath);
	}
	gMidValue[0] = CeGetPrivateProfileFloat(_T("MidValue"), _T("MidValue1"), -1, strFilePath);
	gMidValue[1] = CeGetPrivateProfileFloat(_T("MidValue"), _T("MidValue2"), -1, strFilePath);
}

LRESULT DoCreateMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	GetParamFormIni();
// 	wchar_t szTest[50];
// 	memset(szTest, 0, sizeof(szTest));
// 	swprintf(szTest, _T("%f %f %f %f"), gfX11[3], gfX12[3], gfX21[3], gfX22[3]);
// 	MessageBox(hWnd, szTest, _T("ifo"), MB_OK);

	SetTimer(hWnd, TIMER_CLOCK, 30000, TimerClockProc);

	initRailRoadParam(); //初始化铁路参数 wangbingfu 2010-2-24

	BOOL bRet = ars200fpga_alloc_io();
	cscan_ars200fpga_alloc_io();
    
    Process_Reg_Init();//yachang.huang add on 2013-11-18

	// function define
	DWORD dwLowValue = Sys_Function_Def->wLowFunctionDefine;
	DWORD dwHighValue = Sys_Function_Def->wHighFunctionDefine;


	//	gFunctionDefine = (dwHighValue << 16) + dwLowValue;

#ifdef _NDT_TEST
	wchar_t szTest[50];
	memset(szTest, 0, sizeof(szTest));
	swprintf(szTest, _T("H: %x L: %x FunD: %x"), 
		Sys_Function_Def->wHighFunctionDefine, Sys_Function_Def->wLowFunctionDefine, gFunctionDefine);

	MessageBox(hWnd, szTest, _T("info"), MB_OK);
#endif
	// hardware version
	WORD V1 = Sys_Hardware_Ver->wHardwareVer>>12;
	WORD V2 = (Sys_Hardware_Ver->wHardwareVer>> 8) & 0xF;
	WORD V3 = (Sys_Hardware_Ver->wHardwareVer>> 4) & 0xF;
	WORD V4 = Sys_Hardware_Ver->wHardwareVer & 0xF;
	HARDWARE_VER = V1*10 + V2 + (V3*0.1) + (V4*0.01);
#ifdef _NDT_TEST
	memset(szTest, 0, sizeof(szTest));
	swprintf(szTest, _T("HardwareVer: %.2f"), HARDWARE_VER);

	MessageBox(hWnd, szTest, _T("info"), MB_OK);
#endif
	//wangbingfu add start at 2010-11-15
	gwDeviceTypeValue = *Sys_device_type;
	//	gwDeviceTypeValue = 0x2021;

	if(gwDeviceTypeValue >> 12 == 0x3)
	{
		gwDeviceType = 1;
	}
	else if(((gwDeviceTypeValue & 0x00FF) == 0x24) || ((gwDeviceTypeValue & 0x00FF) == 0x25) || 
		((gwDeviceTypeValue & 0x00FF) == 0x26) || ((gwDeviceTypeValue & 0x00FF) == 0x27)) //铁路专用打开
	{
		gwDeviceType = 2;
	}
	else
		gwDeviceType = 0;
	// 铁路版本最大增益为92，其它情况为110
	if(gwDeviceType == 2)
		MAX_PLUS = 92;
	else
		MAX_PLUS = 110;


	//	gwDeviceType = 2;	//TEST 铁路专用
	//	gwDeviceType = 1;	//TEST 测厚专用
	gwDeviceType = 0;	//TEST 探伤专用

	//wangbingfu add end at 2010-11-15
	// status bar	
	hStatusBar = DoCreateStatusBar(hWnd, ID_STATUS_BAR, hInst, 7);
	if( hStatusBar == NULL )
	{
		MessageBox(hWnd,L"Status Bar not created!",NULL,MB_ICONERROR);
		return FALSE;
	}	

	// 调用资源dll
	hRes = LoadLibrary(TEXT ("NDT_CHN_LIB.dll"));
	if(hRes == NULL)
	{
		MessageBox (hWnd, TEXT ("Can't load NDT_CHN_LIB.dll."), TEXT("NDT_CE"), 0) ;

		return -1 ;
	}

	memset(gPicData, 0, sizeof(gPicData));
	memset(preDrawData, 0, sizeof(WORD) * DATA_SIZE * CHANNEL_NUM);
	
	GetCurPath(lpDefaultParamFile);
	//strcat(lpDefaultParamFile, _T("1.par"));
	ReadParamFile(lpDefaultParamFile);
	if(gwDeviceType == 2)
	{
		ReceiveParam_ad[0].nProbeFreq = 5;
		ReceiveParam_ad[1].nProbeFreq = 5;
	}
	//	readDgsFile(_T("\\ResidentFlash\\dgs.bin"));
	gSysInfo.wDeviceType = *Sys_device_type;
	SaveParamFile(lpDefaultParamFile);	//保存设备型号 wangbingfu add at 2010-11-18
	memcpy(stOldGateInfos, GateInfos, sizeof(GATE_INFO)*CHANNEL_NUM*GATE_NUM);	//保存闸门信息wangbingfu add at 2010-11-22

	// ARS201,ARS201E特别处理，设为单通道模式，指定通道
	if(gwDeviceTypeValue == 0x2010 || gwDeviceTypeValue == 0x2011)
	{
		gSysInfo.wCurrentChNo = 0;
		gSysInfo.wWorkMode =  SingleChannelA;
		//		gSysInfo.wCurrentChNo = 1;
		//		gSysInfo.wWorkMode =  SingleChannelB;
	}
	InitializeCriticalSection(&csSysInfo);
	EnterCriticalSection(&csSysInfo);

	CurrentChannelNo = gSysInfo.wCurrentChNo;

	WORKMODE wWorkMode = gSysInfo.wWorkMode;
	LeaveCriticalSection(&csSysInfo);
	DeleteCriticalSection(&csSysInfo);

	if(CurrentChannelNo == 0)
	{
		chCurrent_data_reg = ch1_data_reg;
		chCurrent_gate_reg = ch1_gate_reg;
		chCurrent_con_reg = ch1_con_reg;
		chCurrent_interval = ch1_interval;
		chCurrent_data = ch1_data;
		chCurrent_plus_reg = ch1_plus_reg;
		chCurrent_dac_reg = ch1_dac_reg;

		chCurrent_gate1curve = ch1_gate1curve;
		chCurrent_gate2curve = ch1_gate2curve;
		chCurrent_gate3curve = ch1_gate3curve;

		chCurrent_tgc = ch1_tgc;//wangbingfu add at 2010-06-17
	}
	else if(CurrentChannelNo == 1)
	{
		chCurrent_data_reg = ch2_data_reg;
		chCurrent_gate_reg = ch2_gate_reg;
		chCurrent_con_reg = ch2_con_reg;
		chCurrent_interval = ch2_interval;
		chCurrent_data = ch2_data;
		chCurrent_plus_reg = ch2_plus_reg;
		chCurrent_dac_reg = ch2_dac_reg;

		chCurrent_gate1curve = ch2_gate1curve;
		chCurrent_gate2curve = ch2_gate2curve;
		chCurrent_gate3curve = ch2_gate3curve;

		chCurrent_tgc = ch2_tgc;//wangbingfu add at 2010-06-17
	}

	//	InitParam();
	SetInit(); 
	//wangbingfu add start at 2010-10-21
	// 初始化系统颜色
	InitializeSysColor(gstSysColor, SYS_COLOR_NUM);
	//wangbingfu add end at 2010-10-21
	// 模式
	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)(WORD)gSysInfo.wWorkMode);

	// 通道
	memset(gszTextChannel, 0, sizeof(gszTextChannel));

	if(wWorkMode != DanJingGuanLian && wWorkMode != ShuangJingGuanLian)
	{
		swprintf(gszTextChannel, _T("%d"), CurrentChannelNo+1);
	}

	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

	// 步进
	memset(gszTextStep, 0, sizeof(gszTextStep));
	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T(""));

	// 增益
	memset(gszTextPlus, 0, sizeof(gszTextPlus));
	swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);
	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (3| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

	memset(gszTextRefPlus, 0, sizeof(gszTextPlus));
	swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);
	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);

	// 峰值...
	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T(""));

	// 电量 + 时间
	// 获取电压值
	gPressValue = (WORD)get_battery();

	if(gPressValue < 620)
	{
		MessageBeep(0xFFFFFFFF);

		memset(gszTextPeak, 0, sizeof(gszTextPeak));
		swprintf(gszTextPeak, _T("请及时充电"));

		SendMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	}

	SYSTEMTIME time;
	GetLocalTime(&time);

	memset(gszTextTime, 0, sizeof(gszTextTime));
	swprintf(gszTextTime, _T("%2d:%02d"), time.wHour, time.wMinute);

	//battery test
#ifdef _NDT_TEST
	if(GetFileAttributes(_T("\\ResidentFlash\\battery.txt"))  == 0xFFFFFFFF)
	{
		hFileWrite = CreateFile(_T("\\ResidentFlash\\battery.txt"),
			GENERIC_READ|GENERIC_WRITE,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if(hFileWrite == INVALID_HANDLE_VALUE)
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("创建battery文件失败"));
		}
		char buffer1[5];
		memset(buffer1, 0, sizeof(buffer1));
		_itoa(gPressValue, buffer1, 10);
		buffer1[3] = ' ';
		buffer1[4] = ' ';

		char buffer2[3];
		memset(buffer2, 0, sizeof(buffer2));
		_itoa(time.wHour, buffer2, 10);
		buffer2[2] = ':';

		char buffer3[3];
		memset(buffer3, 0, sizeof(buffer3));
		_itoa(time.wMinute, buffer3, 10);
		buffer3[2] = ':';

		char buffer4[4];
		memset(buffer4, 0, sizeof(buffer4));
		_itoa(time.wSecond, buffer4, 10);
		buffer4[2] = '\r';
		buffer4[3] = '\n';
		DWORD dwWriteDataSize;
		if(!WriteFile(hFileWrite,buffer1,5,&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写battery文件失败"));

			//CloseHandle(hFileWrite);
		}
		if(!WriteFile(hFileWrite,buffer2,3,&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写battery文件失败"));

			//CloseHandle(hFileWrite);
		}
		if(!WriteFile(hFileWrite,buffer3,3,&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写battery文件失败"));

			//CloseHandle(hFileWrite);
		}
		if(!WriteFile(hFileWrite,buffer4,4,&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写battery文件失败"));

			//CloseHandle(hFileWrite);
		}
		CloseHandle(hFileWrite);
	}
#endif
	//battery test

	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (6 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextTime);

#if 0
	//wangbingfu add at 2010-7-26
	if(gwDeviceType == 1)
	{

		//	SetMenuStateFlag(TRUE);

		hWndChDisplay = CreateWindow(szChDisplayChild, TEXT(""), WS_CHILD,
			MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWnd, NULL, hInst, NULL);//创建测厚测量显示窗口
		ShowWindow(hWndChDisplay,SW_SHOW);
		UpdateWindow(hWndChDisplay);				

	}
	//wangbingfu add at 2010-7-26
#endif
	// songchenguang add start 2010-07-31
	memset(gszInfo, 0, sizeof(gszInfo));
	// songchenguang add end 2010-07-31
	ch1_data_reg->wMonitorReset = 1;
	//wangbingfu modify start at 2011-3-14
	Sleep(1);	
	//	wait400000();
	//wangbingfu modify end at 2011-3-14
	ch1_data_reg->wMonitorReset = 0;

	ch2_data_reg->wMonitorReset = 1;
	//wangbingfu modify start at 2011-3-14
	Sleep(1);	

	ch2_data_reg->wMonitorReset = 0;


	//通道1软复位
	ch1_con_reg->wReset |= 0x1;
	ch1_con_reg->wReset &= 0xFFFE;
	//通道2软复位
	ch2_con_reg->wReset |= 0x1;
	ch2_con_reg->wReset &= 0xFFFE;
	//wangbingfu modify end at 2011-3-14

	WORD wLamp = 1;
	memcpy(lamp,&wLamp,2);//点灯

	gbInitFlag = TRUE;

    HANDLE serverThread = ::CreateThread(NULL, 0, &ARS500_Server, NULL, CREATE_SUSPENDED, NULL);
    ::SetThreadPriority(serverThread, THREAD_PRIORITY_TIME_CRITICAL);
    ResumeThread(serverThread);


	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT DoDrawItem(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(wParam == ID_STATUS_BAR)	// 状态栏
	{
		DRAWITEMSTRUCT* lpDrawItemStruct = (DRAWITEMSTRUCT*)lParam;

		HDC hDC = lpDrawItemStruct->hDC;
		RECT rect = lpDrawItemStruct->rcItem;

#ifdef _NDT_TEST
		wchar_t szText[20];
		memset(szText, 0 , sizeof(szText));
		swprintf(szText, _T("Width: %d , Height = %d"), (rect.right - rect.left),
			(rect.bottom -  rect.top));
		MessageBox(NULL, szText, _T("Info"), MB_OK);
#endif
		////////////////
		RECT statusRect;
		GetClientRect(lpDrawItemStruct->hwndItem, &statusRect);
		HBRUSH hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
		FillRect(hDC, &statusRect, hbr);

#ifdef _NDT_TEST
		wchar_t szText2[20];
		memset(szText2, 0 , sizeof(szText2));
		swprintf(szText2, _T("left: %d, top = %d, right = %d, bottom = %d"), statusRect.left,
			statusRect.top, statusRect.right, statusRect.bottom);
		MessageBox(NULL, szText2, _T("Info"), MB_OK);
#endif
		////////////////
		if(lpDrawItemStruct->itemID == 0)	// 工作模式
		{
			WORKMODE nMode = (WORKMODE)lpDrawItemStruct->itemData;

			HDC hSrcDC = CreateCompatibleDC(hDC);
			HBITMAP hBmp = GetModeBmp(nMode);
			
			HBITMAP hOldBmp = (HBITMAP)SelectObject(hSrcDC, hBmp);

			StretchBlt(hDC,rect.left, rect.top, (rect.right - rect.left),
				(rect.bottom -  rect.top), hSrcDC, 0,0, (rect.right - rect.left),
				(rect.bottom -  rect.top), SRCCOPY);

			SelectObject(hSrcDC, hOldBmp);

			DeleteObject(hBmp);
			DeleteDC(hSrcDC);
		}
//		else if(lpDrawItemStruct->itemID == 1)	// 通道
//		{

//		}
		else if(lpDrawItemStruct->itemID == 2)	// 步进
		{
			CopyBmp(hDC, IDB_BITMAP_STEP_R, rect);

			RECT textRect = rect;
			textRect.left += 50;
			textRect.top += 4;
			
			LPCWSTR pszText = (LPCWSTR)lpDrawItemStruct->itemData;
			
			SetBkMode(hDC, TRANSPARENT);

			LOGFONT lf;
			memset(&lf, 0, sizeof(lf));
			lf.lfHeight = TEXT_HEIGHT;        
			lf.lfWidth = 0;

			HFONT hFont = CreateFontIndirect(&lf);

			hFont = (HFONT) SelectObject (hDC, hFont) ;

			DrawText (hDC, pszText, -1, &textRect, 
						  DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			DeleteObject (SelectObject (hDC, hFont));

		}
		else if(lpDrawItemStruct->itemID == 3)	// 增益
		{
			CopyBmp(hDC, IDB_BITMAP_BASE_PLUS_R, rect);

			RECT textRect = rect; 
			textRect.left += 50;
			textRect.top += 15;

			LOGFONT lf;
			memset(&lf, 0, sizeof(lf));
			lf.lfHeight = TEXT_HEIGHT;        
            lf.lfWidth = 0;

			HFONT hFont = CreateFontIndirect(&lf);

			hFont = (HFONT) SelectObject (hDC, hFont) ;

			LPCWSTR pszText = (LPCWSTR)lpDrawItemStruct->itemData;
			SetBkMode(hDC, TRANSPARENT);

			if(gbHotDB && gDBIndex == 1) // 快捷
				FillRect(hDC, &textRect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
			else
				FillRect(hDC, &textRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

			DrawText (hDC, pszText, -1, &textRect, 
						  DT_CENTER | DT_BOTTOM | DT_SINGLELINE);	

			DeleteObject (SelectObject (hDC, hFont)) ;

		}
		else if(lpDrawItemStruct->itemID == 4)	// 参考增益
		{
			CopyBmp(hDC, IDB_BITMAP_REF_PLUS_R, rect);

			RECT textRect = rect;
			textRect.top += 15;

			LOGFONT lf;
			memset(&lf, 0, sizeof(lf));
			lf.lfHeight = TEXT_HEIGHT;        
            lf.lfWidth = 0;

			HFONT hFont = CreateFontIndirect(&lf);

			hFont = (HFONT) SelectObject (hDC, hFont) ;

			LPCWSTR pszText = (LPCWSTR)lpDrawItemStruct->itemData;
			SetBkMode(hDC, TRANSPARENT);

			if(gbHotDB && gDBIndex == 2) // 快捷
				FillRect(hDC, &textRect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
			else
				FillRect(hDC, &textRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
			
			DrawText (hDC, pszText, -1, &textRect, 
						  DT_CENTER | DT_BOTTOM | DT_SINGLELINE);

			DeleteObject (SelectObject (hDC, hFont)) ;

		}
		else if(lpDrawItemStruct->itemID == 5)	// 峰值
		{
			LPCWSTR pszText = (LPCWSTR)lpDrawItemStruct->itemData;

			HDC hSrcDC = CreateCompatibleDC(hDC);
			HBITMAP hBmpFreeze = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_DONGJIE_R));

			BITMAP bitmapFreeze;
			GetObject (hBmpFreeze, sizeof (BITMAP), &bitmapFreeze);

			HBITMAP hBmpYizhi = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_YIZHI_R));

			BITMAP bitmapYizhi;
			GetObject (hBmpYizhi, sizeof (BITMAP), &bitmapYizhi);

			HBITMAP hBmpGate = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_ZHAMENSHEZHI_R));

			BITMAP bitmapGate;
			GetObject (hBmpGate, sizeof (BITMAP), &bitmapGate);

			HBITMAP hBmpAlarm = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_LINGSHENG_R));

			BITMAP bitmapAlarm;
			GetObject (hBmpAlarm, sizeof (BITMAP), &bitmapAlarm);

			HBITMAP hBmpNoAlarm = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_LIGNGSHENGSTOP_R));

			BITMAP bitmapNoAlarm;
			GetObject (hBmpNoAlarm, sizeof (BITMAP), &bitmapNoAlarm);
			

			HBRUSH hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
			FillRect(hDC, &rect, hbr);

			if(bFreezeUp)
			{
				HBITMAP hOldBmp = (HBITMAP)SelectObject(hSrcDC, hBmpFreeze);
				BitBlt(hDC, rect.left, rect.top, bitmapFreeze.bmWidth, bitmapFreeze.bmHeight,
					hSrcDC, 0,0, SRCCOPY);
				SelectObject(hSrcDC, hOldBmp);
			}
			
			if(DisplaySet_ad[CurrentChannelNo].nBate > 0)
			{
				HBITMAP hOldBmp = (HBITMAP)SelectObject(hSrcDC, hBmpYizhi);
				BitBlt(hDC, (rect.left + bitmapFreeze.bmWidth), rect.top, bitmapYizhi.bmWidth, bitmapYizhi.bmHeight,
					hSrcDC, 0,0, SRCCOPY);
				SelectObject(hSrcDC, hOldBmp);
			}
		/*	if(gbHotGateFlag)
			{
				HBITMAP hOldBmp = (HBITMAP)SelectObject(hSrcDC, hBmpGate);
				BitBlt(hDC, (rect.left + bitmapFreeze.bmWidth + bitmapYizhi.bmWidth), rect.top, bitmapGate.bmWidth, bitmapGate.bmHeight,
					hSrcDC, 0,0, SRCCOPY);
				SelectObject(hSrcDC, hOldBmp);
			}*/
			if(gbAlarmFlag)
			{
				HBITMAP hOldBmp = (HBITMAP)SelectObject(hSrcDC, hBmpAlarm);
				BitBlt(hDC, (rect.left + bitmapFreeze.bmWidth + bitmapYizhi.bmWidth), rect.top, bitmapAlarm.bmWidth, bitmapAlarm.bmHeight,
					hSrcDC, 0,0, SRCCOPY);
				SelectObject(hSrcDC, hOldBmp);
			}
			else
			{
				HBITMAP hOldBmp = (HBITMAP)SelectObject(hSrcDC, hBmpNoAlarm);
				BitBlt(hDC, (rect.left + bitmapFreeze.bmWidth + bitmapYizhi.bmWidth), rect.top, bitmapNoAlarm.bmWidth, bitmapNoAlarm.bmHeight,
					hSrcDC, 0,0, SRCCOPY);
				SelectObject(hSrcDC, hOldBmp);
			}

			LOGFONT lf;
			memset(&lf, 0, sizeof(lf));
			lf.lfHeight = 20;        
            lf.lfWidth = 0;

			HFONT hFont = CreateFontIndirect(&lf);

			hFont = (HFONT) SelectObject (hDC, hFont) ;

			SetBkMode(hDC, TRANSPARENT);

			RECT textRect = rect;
			textRect.top += 20;
			textRect.left += 16;

			DrawText (hDC, pszText, -1, &textRect, 
						  DT_CENTER | DT_BOTTOM | DT_SINGLELINE);

			DeleteObject (SelectObject (hDC, hFont)) ;
			DeleteObject(hBmpFreeze);
			DeleteObject(hBmpYizhi);
			DeleteDC(hSrcDC);

		}
		else if(lpDrawItemStruct->itemID == 6)	// 电量+时间
		{
			HDC hSrcDC = CreateCompatibleDC(hDC);
			HBITMAP hBmp = GetPressBmp(gPressValue);
			
			HBITMAP hOldBmp = (HBITMAP)SelectObject(hSrcDC, hBmp);
			StretchBlt(hDC,rect.left, rect.top, (rect.right - rect.left),
				(rect.bottom -  rect.top), hSrcDC, 0,0, 
				(rect.right - rect.left),
				(rect.bottom -  rect.top),SRCCOPY);
			SelectObject(hSrcDC, hOldBmp);

			// 时间
			RECT textRect = rect;
			textRect.bottom -= 20;

			LOGFONT lf;
			memset(&lf, 0, sizeof(lf));
			lf.lfHeight = 28;        
            lf.lfWidth = 0;

			HFONT hFont = CreateFontIndirect(&lf);

			hFont = (HFONT) SelectObject (hDC, hFont) ;
			LPCWSTR pszText = (LPCWSTR)lpDrawItemStruct->itemData;
			SetBkMode(hDC, TRANSPARENT);
			DrawText (hDC, pszText, -1, &textRect, 
						  DT_LEFT | DT_BOTTOM | DT_SINGLELINE);
			DeleteObject (SelectObject (hDC, hFont)) ;

			DeleteObject(hBmp);
			DeleteDC(hSrcDC);
		}		
		else	// 通道
		{
			LOGFONT lf;
			memset(&lf, 0, sizeof(lf));
			lf.lfHeight = STATUSBAR_HEIGHT;        
            lf.lfWidth = 0;

			HFONT hFont = CreateFontIndirect(&lf);

			hFont = (HFONT) SelectObject (hDC, hFont) ;
			LPCWSTR pszText = (LPCWSTR)lpDrawItemStruct->itemData;
			SetBkMode(hDC, TRANSPARENT);
			DrawText (hDC, pszText, -1, &rect, 
						  DT_LEFT | DT_BOTTOM | DT_SINGLELINE);
			DeleteObject (SelectObject (hDC, hFont)) ;

		}
		
	}

	return 0;
}

HBITMAP GetPressBmp(WORD nPress)
{
	HBITMAP hBmp = NULL;
	if(nPress > 752)	// 充电
		hBmp = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_BATTERY3_R));
	else if(nPress>708)	// 满
		hBmp = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_BATTERY3_R));
	else if(nPress>664)	// 66%
		hBmp = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_BATTERY2_R));
	else if(nPress>620)	// 33%
		hBmp = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_BATTERY1_R));
	else	// 0%
		hBmp = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_BATTERY0_R));	

	return hBmp;
}

LRESULT DoPaintMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	// 创建工作线程
//	HANDLE hThread = CreateThread(NULL, 0, AECThreadProc1, NULL, CREATE_SUSPENDED, NULL);
//	HANDLE hThread = ::CreateThread(NULL, 0, AECThreadProc, (LPVOID)&thread_param, CREATE_SUSPENDED, &dwPutsThreadID);
//	ResumeThread(hThread);
//	::SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
	//if(hPutsThread == NULL)
	//{
	//	thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
	//	hPutsThread = ::CreateThread(NULL, 0, AECThreadProc, (LPVOID)&thread_param, CREATE_SUSPENDED, &dwPutsThreadID);
	//	::SetThreadPriority(hPutsThread, THREAD_PRIORITY_TIME_CRITICAL);
	//	hWorkEvent = CreateEvent(NULL, TRUE, TRUE, TEXT("WorkEvent0"));
	//	HANDLE hThread = CreateThread(NULL, 0, AECThreadProc1, (LPVOID)&thread_param, CREATE_SUSPENDED, NULL);
	//	ResumeThread(hThread);
	//	::SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
	//	
	//	ResumeThread(hPutsThread);

	//	SetTimer(hWnd, TIMER_PROCCESS, 10, TimerProcessProc);
	//}	
	// wireless wangbingfu add start at 2010-05-30
	//CTcpServer monitorServer(3333, 5);
	CTcpMonitor tcpMonitor;
	CTcpCScan tcpCScan;
	CTcpACScan tcpACScan;
	CTcpParam tcpParam;


//	CTcpParamString tcpParamString;
/*	if(hSocketThread == NULL)
	{
		hSocketThread = ::CreateThread(NULL, 0, SocketThreadProc, NULL, 0, &dwSocketThreadID);
		
	}*/
/*	if(hSocketThread2 == NULL)
	{
		hSocketThread2 = ::CreateThread(NULL, 0, SocketThreadProc2, NULL, 0, &dwSocket2ThreadID);
		
	}*/

/*	if(hSocketThread3 == NULL)
	{
		hSocketThread3 = ::CreateThread(NULL, 0, SocketThreadProc3, NULL, 0, &dwSocket3ThreadID);

	}*/
	//yachang.huang add start on 2013-12-19

/*	if(hSocketThread4 == NULL)
	{
		hSocketThread4 = ::CreateThread(NULL, 0, SocketThreadProc4, NULL, 0, &dwSocket4ThreadID);

	}*/


	//yachang.huang add end on 2013-12-19
	//wireless wangbingfu add end at 2010-05-30

	//wangbingfu add start at 2010-9-17
	if(gwDeviceType == 1)
	{
		if(hWndChDisplay == 0)
		{
			SetMenuStateFlag(TRUE);

			hWndChDisplay = CreateWindow(szChDisplayChild, TEXT(""), WS_CHILD,
						 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWnd, NULL, hInst, NULL);//创建测厚测量显示窗口
			ShowWindow(hWndChDisplay,SW_SHOW);
			UpdateWindow(hWndChDisplay);

			gCurrentWnd = hWndChDisplay;
		}
	}
	//wangbingfu add end at 2010-9-17
	//wangbingfu add start at 2011-1-19
	else if(gwDeviceType == 2)
	{
		if(hWndTlMenu == NULL)
		{
			SetMenuStateFlag(TRUE);

			hWndTlMenu = CreateWindow(szTlMenuChild, TEXT(""), WS_CHILD,
						 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWnd, NULL, hInst, NULL);//创建铁路专用菜单窗口
			ShowWindow(hWndTlMenu,SW_SHOW);
			UpdateWindow(hWndTlMenu);

			gCurrentWnd = hWndTlMenu;
		}
	}
	else
	{
		;//
	}
	//wangbingfu add end at 2011-1-19
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}


LRESULT DoCommandMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	WORD idItem, wNotifyCode;
	HWND hwndCtl;
	int i;

	idItem = (WORD)LOWORD(wParam);
	wNotifyCode = (WORD)HIWORD(wParam);
	hwndCtl = (HWND)lParam;

	for(i=0; i<dim(MainCommandItems); i++)
	{
		if(idItem == MainCommandItems[i].Code)
			return (*MainCommandItems[i].Fxn)(hWnd, idItem,
					hwndCtl, wNotifyCode);
	}

	return 0;
}

LRESULT DoDestoryMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	// 保存配置到默认文件...
	SaveParamFile(lpDefaultParamFile);

	KillTimer (hWnd, TIMER_PROCCESS);
//	KillTimer (hWnd, TIMER_PRESS);
	KillTimer (hWnd, TIMER_CLOCK);

	gbEndFlag = TRUE;

	BOOL bRet = ars200fpga_unalloc_io();
	cscan_ars200fpga_unalloc_io();
	if(hRes)
	{
		FreeLibrary(hRes);
	}

	InitializeCriticalSection(&csStop);
	EnterCriticalSection(&csStop);
	bTerminated = TRUE;
	LeaveCriticalSection(&csStop);
	DeleteCriticalSection(&csStop);
	
	PostQuitMessage(0);
	return 0;
}


LRESULT OnParamSet(HWND hwnd,WORD a,HWND hwndctl,WORD b)
{

	return 0;
}

LRESULT OnParamSet2(HWND hwnd,WORD a,HWND hwndctl,WORD b)
{
	return 0;
}

// 获取显示的矩形区域位置

void GetDisplayRect(const RECT& RctFull,
					BOOL bMenuShown,
					RECT& RctWave, 
					RECT& RctXScale, 
					RECT& RctYScale,
					RECT& RctRight)
{
	if(!bMenuShown)
	{
		// 波形区域
		RctWave.left = RctFull.left + SCALE_WIDTH;
		RctWave.right = RctFull.right - SCALE_WIDTH;
		RctWave.top = RctFull.top;
		RctWave.bottom = RctFull.bottom - SCALE_WIDTH;
	
		// Y坐标区域
		RctYScale.left = RctFull.left;
		RctYScale.right = RctWave.left;
		RctYScale.top = RctFull.top;
		RctYScale.bottom = RctWave.bottom;

		// X坐标区域
		RctXScale.left = RctFull.left;
		RctXScale.right = RctFull.right;
		RctXScale.top = RctWave.bottom;
		RctXScale.bottom = RctFull.bottom;

		// 右侧空白区
		RctRight.left = RctWave.right;
		RctRight.right = RctFull.right;
		RctRight.top = RctFull.top;
		RctRight.bottom = RctWave.bottom;
	}
	else
	{
		// 波形区域
		RctWave.left = RctFull.left + SCALE_WIDTH;
		RctWave.right = RctFull.right - SCALE_WIDTH - MENU_WIDTH;
		RctWave.top = RctFull.top;
		RctWave.bottom = RctFull.bottom - SCALE_WIDTH;
	
		// Y坐标区域
		RctYScale.left = RctFull.left;
		RctYScale.right = RctWave.left;
		RctYScale.top = RctFull.top;
		RctYScale.bottom = RctWave.bottom;

		// X坐标区域
		RctXScale.left = RctFull.left;
		RctXScale.right = RctFull.right - MENU_WIDTH;
		RctXScale.top = RctWave.bottom;
		RctXScale.bottom = RctFull.bottom;

		// 右侧空白区
		RctRight.left = RctWave.right;
		RctRight.right = RctFull.right - MENU_WIDTH;
		RctRight.top = RctFull.top;
		RctRight.bottom = RctWave.bottom;
	}

	InitializeCriticalSection(&csHanFengTanShang);
	EnterCriticalSection(&csHanFengTanShang);
	if(bHanFengTanShangSwitch)
	{
		// 波形区域
		RctWave.top = (RctWave.bottom - RctFull.top)/2 + RctFull.top;
	}
	LeaveCriticalSection(&csHanFengTanShang);
	DeleteCriticalSection(&csHanFengTanShang);
}

/** 
 * 生成固定范围内的一组随机数
 * @param     range_min [in]:	范围最小值.
 * @param     range_max [in]:	范围最大值.
 * @param     n [in]:			数组个数.
 * @param     pArray [out]:		数组指针.
 * @return    DWORD:
 * @since     1.00
 */
void RangedRandDemo( WORD range_min, WORD range_max, int n , WORD *pArray)
{
   // Generate random numbers in the half-closed interval
   // [range_min, range_max). In other words,
   // range_min <= random number < range_max
	WORD *p = pArray;
	
    int i;
    for ( i = 0; i < n; i++ )
    {
       WORD u = (double)rand() / (RAND_MAX + 1) * (range_max - range_min)
            + range_min;
	   // 画线
	   *p = u;
	   p++;
    }

	
}

// songchenguang add start 2010-06-29
// 回波区域信息
struct HUIBO_INFO
{
	int nRightPos;
	WORD nColor;
};
// songchenguang add end 2010-06-29
/** 
 * 画坐标文字
 * @param     hMemDC [in]:			屏幕缓存.
 * @param     rect [in]:			主窗口显示出来的范围.
 * @param     RctWave [in]:			波形范围.
 * @param     nLoopIndex [in]:		循环序号.
 * @return    DWORD:
 * @since     1.00
 */
void DrawReferenceFrame(HDC hMemDC,
						const RECT& rect,
						const RECT& RctWave,
						const RECT& RctXScale,
						const RECT& RctYScale,
						const int& nLoopIndex,
						HBRUSH hbrScaleBack)
{
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;

	RECT RctText;
	wchar_t szText[10];
	
	InitializeCriticalSection(&csZhanKuan);
	EnterCriticalSection(&csZhanKuan);

	BOOL bZhanKuan = gbZhanKuan;
	LeaveCriticalSection(&csZhanKuan);
	DeleteCriticalSection(&csZhanKuan);
	
	float fStart = 0.0f;
	float fEnd = 0.0f;

	if(bZhanKuan)
	{
		InitializeCriticalSection(&csZhanKuanValue);
		EnterCriticalSection(&csZhanKuanValue);
		fStart = gZhanKuanFront;
		fEnd = gZhanKuanBack;
		LeaveCriticalSection(&csZhanKuanValue);
		DeleteCriticalSection(&csZhanKuanValue);
	}
	else
	{
		InitializeCriticalSection(&csRange);
		EnterCriticalSection(&csRange);
		fStart = fRangeStart[CurrentChannelNo];
		fEnd = fRangeEnd[CurrentChannelNo];
		LeaveCriticalSection(&csRange);
		DeleteCriticalSection(&csRange);
	}

	InitializeCriticalSection(&csBScan);
	EnterCriticalSection(&csBScan);
	BOOL bScan = gbBscanSwitch;
	LeaveCriticalSection(&csBScan);
	DeleteCriticalSection(&csBScan);
#if 0
	if(nLoopIndex == 0)
	{
		if(bScan)
		{
			HBRUSH hbr = hbrScaleBack;
			FillRect(hMemDC, &RctXScale, hbr);

			FillRect(hMemDC, &RctYScale, hbr);

			for(int i=1; i<5; i++)
			{
				WORD y = RctWave.top + WORD(nWaveHeight/5*i);

				// 写坐标值
				RctText.left = RctWave.left - SCALE_WIDTH;
				RctText.right = RctWave.left;
				RctText.top = y - 15;
				RctText.bottom = y + 15;

				memset(szText, 0, sizeof(szText));
				swprintf(szText, _T("%d"), i*20);
				DrawText(hMemDC, szText, -1, &RctText, 
					DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
			}
		}
		else
		{
			for(int i=0; i<=5; i++)
			{
				WORD x = RctWave.left + WORD(nWaveWidth/5*i);

				// 写坐标值
				RctText.left = x - 60;
				RctText.right = x + 60;
				RctText.top = RctWave.bottom;
				RctText.bottom = rect.bottom; //RctWave.bottom + SCALE_WIDTH;

				memset(szText, 0, sizeof(szText));

				float xValue = fStart + (fEnd - fStart)/5.0f * i;

				swprintf(szText, _T("%.1f"), xValue);

				SetBkMode(hMemDC, TRANSPARENT); 
				DrawText(hMemDC, szText, -1, &RctText, 
					DT_TOP | DT_CENTER | DT_SINGLELINE);
			}
			InitializeCriticalSection(&cs110Display);
			EnterCriticalSection(&cs110Display);
			
			// Y轴
			if(!gb110Display)
			{
				for(int i=1; i<5; i++)
				{
					WORD y = RctWave.bottom - WORD(nWaveHeight/5*i);

					// 写坐标值
					RctText.left = RctWave.left - SCALE_WIDTH;
					RctText.right = RctWave.left;
					RctText.top = y - 15;
					RctText.bottom = y + 15;

					memset(szText, 0, sizeof(szText));
					swprintf(szText, _T("%d"), i*20);
					DrawText(hMemDC, szText, -1, &RctText, 
						DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
				}
			}
			else
			{
				LOGFONT lf;
				memset(&lf, 0, sizeof(lf));
				lf.lfHeight = 24;        
				lf.lfWidth = 0;

				HFONT hFont = CreateFontIndirect(&lf);

				hFont = (HFONT) SelectObject (hMemDC, hFont) ;

				for(int i=2; i<=10; i+=2)
				{
					WORD y = RctWave.bottom - WORD(nWaveHeight/11.0f*i);

					// 写坐标值
					RctText.left = RctWave.left - SCALE_WIDTH;
					RctText.right = RctWave.left;
					RctText.top = y - 15;
					RctText.bottom = y + 15;

					memset(szText, 0, sizeof(szText));
					swprintf(szText, _T("%d"), i*10);
					DrawText(hMemDC, szText, -1, &RctText, 
						DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
				}

				// 110%
				RctText.left = RctWave.left - SCALE_WIDTH;
				RctText.right = RctWave.left;
				RctText.top = RctWave.top;
				RctText.bottom = RctWave.top + 30;

				memset(szText, 0, sizeof(szText));
				swprintf(szText, _T("110"));
				DrawText(hMemDC, szText, -1, &RctText, 
						DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

				DeleteObject(SelectObject (hMemDC, hFont));
			}
			LeaveCriticalSection(&cs110Display);
			DeleteCriticalSection(&cs110Display);
		}
	}
#endif
	// X轴
	if(!bScan)
	{
		if(gbDrawHeadVCurveFlag)	//探头V型补偿
		{
			for(int i=1; i<5; i++)
			{
				WORD x = RctWave.left + WORD(nWaveWidth/5*i);

				// 写坐标值
				RctText.left = x - 60;
				RctText.right = x + 60;
				RctText.top = RctWave.bottom;
				RctText.bottom = rect.bottom; //RctWave.bottom + SCALE_WIDTH;

				memset(szText, 0, sizeof(szText));

				float xValue = fStart + (fEnd - fStart)/5.0f * i;

				swprintf(szText, _T("%d"), i*20);

				SetBkMode(hMemDC, TRANSPARENT); 
				DrawText(hMemDC, szText, -1, &RctText, 
					DT_TOP | DT_CENTER | DT_SINGLELINE);
				
			}
		}
		else if(gbDrawTempCurveCurveFlag)
		{
			for(int i=1; i<5; i++)
			{
				WORD x = RctWave.left + WORD(nWaveWidth/5*i);

				// 写坐标值
				RctText.left = x - 60;
				RctText.right = x + 60;
				RctText.top = RctWave.bottom;
				RctText.bottom = rect.bottom; //RctWave.bottom + SCALE_WIDTH;

				memset(szText, 0, sizeof(szText));

				float xValue = fStart + (fEnd - fStart)/5.0f * i;

				swprintf(szText, _T("%d"), i*200);

				SetBkMode(hMemDC, TRANSPARENT); 
				DrawText(hMemDC, szText, -1, &RctText, 
					DT_TOP | DT_CENTER | DT_SINGLELINE);
				
			}
		}
		else
		{
			for(int i=0; i<=5; i++)
			{
				WORD x = RctWave.left + WORD(nWaveWidth/5*i);

				// 写坐标值
				RctText.left = x - 60;
				RctText.right = x + 60;
				RctText.top = RctWave.bottom;
				RctText.bottom = rect.bottom; //RctWave.bottom + SCALE_WIDTH;

				memset(szText, 0, sizeof(szText));

				float xValue = fStart + (fEnd - fStart)/5.0f * i;
				if(xValue <= 100)
					swprintf(szText, _T("%.1f"), xValue);
				else
					swprintf(szText, _T("%d"), (WORD)xValue);

				SetBkMode(hMemDC, TRANSPARENT); 
				DrawText(hMemDC, szText, -1, &RctText, 
					DT_TOP | DT_CENTER | DT_SINGLELINE);
			}
		}
		if(nLoopIndex == 0)
		{
			InitializeCriticalSection(&cs110Display);
			EnterCriticalSection(&cs110Display);
			
			if(gb110Display)
			{
				LOGFONT lf;
				memset(&lf, 0, sizeof(lf));
				lf.lfHeight = 24;        
				lf.lfWidth = 0;

				HFONT hFont = CreateFontIndirect(&lf);

				hFont = (HFONT) SelectObject (hMemDC, hFont) ;

				for(int i=2; i<=10; i+=2)
				{
					WORD y = RctWave.bottom - WORD(nWaveHeight/11.0f*i);

					// 写坐标值
					RctText.left = RctWave.left - SCALE_WIDTH;
					RctText.right = RctWave.left;
					RctText.top = y - 15;
					RctText.bottom = y + 15;

					memset(szText, 0, sizeof(szText));
					swprintf(szText, _T("%d"), i*10);
					DrawText(hMemDC, szText, -1, &RctText, 
						DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
				}

				// 110%
				RctText.left = RctWave.left - SCALE_WIDTH;
				RctText.right = RctWave.left;
				RctText.top = RctWave.top;
				RctText.bottom = RctWave.top + 30;

				memset(szText, 0, sizeof(szText));
				swprintf(szText, _T("110"));
				DrawText(hMemDC, szText, -1, &RctText, 
						DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

				DeleteObject(SelectObject (hMemDC, hFont));
			}
			else if(gbDrawTempCurveCurveFlag)
			{
				for(int i=1; i<5; i++)
				{
					WORD y = RctWave.bottom - WORD(nWaveHeight/5*i);

					// 写坐标值
					RctText.left = RctWave.left - SCALE_WIDTH;
					RctText.right = RctWave.left;
					RctText.top = y - 10;
					RctText.bottom = y + 10;

					memset(szText, 0, sizeof(szText));
					
					if(i == 1)
					{
						swprintf(szText, _T(" -%d"), 12);
					}
					else if(i == 2)
					{
						swprintf(szText, _T(" -%d"), 4);
					}
					else if(i == 3)
					{
						swprintf(szText, _T(" %d"), 4);
					}
					else if(i == 4)
					{
						swprintf(szText, _T(" -%d"), 12);
					}
					DrawText(hMemDC, szText, -1, &RctText, 
						DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
				}
			}
			else
			{
				for(int i=1; i<5; i++)
				{
					WORD y = RctWave.bottom - WORD(nWaveHeight/5*i);

					// 写坐标值
					RctText.left = RctWave.left - SCALE_WIDTH;
					RctText.right = RctWave.left;
					RctText.top = y - 15;
					RctText.bottom = y + 15;

					memset(szText, 0, sizeof(szText));
					swprintf(szText, _T("%d"), i*20);
					DrawText(hMemDC, szText, -1, &RctText, 
						DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
				}
			}			
			LeaveCriticalSection(&cs110Display);
			DeleteCriticalSection(&cs110Display);
		}
	}
	else
	{
		HBRUSH hbr = hbrScaleBack;
		FillRect(hMemDC, &RctXScale, hbr);

		if(nLoopIndex == 0)
		{
			FillRect(hMemDC, &RctYScale, hbr);

			for(int i=1; i<5; i++)
			{
				WORD y = RctWave.top + WORD(nWaveHeight/5*i);

				// 写坐标值
				RctText.left = RctWave.left - SCALE_WIDTH;
				RctText.right = RctWave.left;
				RctText.top = y - 15;
				RctText.bottom = y + 15;

				memset(szText, 0, sizeof(szText));
				swprintf(szText, _T("%d"), i*20);
				DrawText(hMemDC, szText, -1, &RctText, 
					DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
			}
		}
	}
	
}

/** 
 * 画波形区域网格虚线
 * @param     RctWave [in]:		波形区域.
 * @return    void:
 * @since     1.00
 */
void DrawRectDashLine(const RECT& RctWave, const WORD& nColorBk)
{
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;

	// X轴wangbingfu modify start at 2011-5-23
	if((gwStandard[CurrentChannelNo] == 4 || gwStandard[CurrentChannelNo] == 5) && gwDeviceType == 2)
	{
		for(int i=1; i<10; i++)
		{
			WORD x = RctWave.left + WORD(nWaveWidth/10*i);

			DashLine(x, RctWave.top, x, RctWave.bottom, 65535, nColorBk);		
		}
	}
	else
	{
		for(int i=1; i<5; i++)
		{
			WORD x = RctWave.left + WORD(nWaveWidth/5*i);

			DashLine(x, RctWave.top, x, RctWave.bottom, 65535, nColorBk);		
		}
	}
	//wangbingfu modify end at 2011-5-23

	InitializeCriticalSection(&cs110Display);
	EnterCriticalSection(&cs110Display);	
		
	if(!gb110Display)
	{	
		// Y轴
		for(int i=1; i<5; i++)
		{
			WORD y = RctWave.bottom - WORD(nWaveHeight/5*i);

			DashLine(RctWave.left, y, RctWave.right, y, 65535, nColorBk);					
		}

		// 50%
		int yCenter = RctWave.top + (RctWave.bottom - RctWave.top)/2;
		DashLine(RctWave.left, yCenter, RctWave.right, yCenter, 63519, nColorBk);

		//wangbingfu add at 2010-7-9
		if(bHanFengTanShangSwitch)//焊缝探伤时 在波形区顶部画一条封顶线
		{
			DashLine(RctWave.left, RctWave.top, RctWave.right, RctWave.top, 65535, nColorBk);	
		}
		//wangbingfu add at 2010-7-9
	}
	else
	{
		// Y轴
		for(int i=2; i<=10; i+=2)
		{
			WORD y = RctWave.bottom - WORD(nWaveHeight/11.0f*i);

			DashLine(RctWave.left, y, RctWave.right, y, 65535, nColorBk);					
		}

		// 50%
		int yCenter = RctWave.bottom - WORD((RctWave.bottom - RctWave.top)/11.0f*5);
		DashLine(RctWave.left, yCenter, RctWave.right, yCenter, 63519, nColorBk);
	}

	LeaveCriticalSection(&cs110Display);
	DeleteCriticalSection(&cs110Display);
}

WORD GetCurrentBKColor(LONG x, vector<HUIBO_INFO>& HuiBoInfo)
{
	for(vector<HUIBO_INFO>::iterator it = HuiBoInfo.begin(); it != HuiBoInfo.end(); ++it)
	{
		if(x < it->nRightPos)
		{
			return it->nColor;
		}
	}

	return 0;
}

void EraseLine(const int& xStart, const int& yStart, const int& xEnd, const int& yEnd, const WORD& nBackColor, const WORD& nLineColor)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;

	WORD *pPixel = NULL;
	if(yStart < yEnd)	// 上升曲线
	{
		for(int i=xStart; i<xEnd; i++)
		{
			pPixel = pBaseAddress + yStart*LCD_XSIZE_TFT + i;
			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
		}

		for(int j = yStart; j<yEnd; j++)
		{
			pPixel = pBaseAddress + j*LCD_XSIZE_TFT + xEnd;

			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
		}
	}
	else	// 下降曲线
	{
		for(int j = yStart; j>yEnd; j--)
		{
			pPixel = pBaseAddress + j*LCD_XSIZE_TFT + xStart;

			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
		}

		for(int i=xStart; i<xEnd; i++)
		{
			pPixel = pBaseAddress + yEnd*LCD_XSIZE_TFT + i;

			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
		}
	}
	*/
}
/** 
 * 画波形曲线
 * @param     RctWave [in]:		波形区域.
 * @param     nLoopIndex [in]:	循环序号.
 * @param     preData [in]:		上次波形数据.
 * @param     preLeft [in]:		上次闸门前沿.
 * @param     preRight [in]:	上次闸门后沿.
 * @param     preTop [in]:		上次闸门上沿.
 * @param     prebOpen [in]:	上次闸门开关状态.
 * @return    void:
 * @since     1.00
 */
void DrawWave(HDC hdc,
			  const int& chNo, 
			  const RECT& RctWave,
			  const int& nLoopIndex,
			  POINT* preData,
			  int* preLeft,
			  int* preRight,
			  int* preTop,
			  BOOL* prebOpen,
			  BOOL bHuiBo,
			  vector<HUIBO_INFO>& HuiBoInfo,
			  int& preZhanKuanFront,
			  int& preZhanKuanBack, const int& nJiOu,
			  const SYS_COLOR_INFO& SysColor,
			  HFONT hFontInfo)
{
	// 把上一次描黑	
	int Left[3] = {0,0,0};
	int Right[3] = {0,0,0};
	int Top[3] = {0,0,0};
	BOOL bOpen[3] = {FALSE,FALSE,FALSE};

	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;
	
	float yb = nWaveHeight/MAX_HEIGHT;

	// 计算X方向步长
	float xb = 0.0f;
	InitializeCriticalSection(&csZhanKuan);
	EnterCriticalSection(&csZhanKuan);
	BOOL bZhanKuan = gbZhanKuan;
	LeaveCriticalSection(&csZhanKuan);
	DeleteCriticalSection(&csZhanKuan);

	InitializeCriticalSection(&csZhanKuanValue);
	EnterCriticalSection(&csZhanKuanValue);
	float fFront = gZhanKuanFront;
	float fBack = gZhanKuanBack;
	LeaveCriticalSection(&csZhanKuanValue);
	DeleteCriticalSection(&csZhanKuanValue);

	InitializeCriticalSection(&csMinRange);
	EnterCriticalSection(&csMinRange);

	float fMinRange = gfMinRange[chNo];

	LeaveCriticalSection(&csMinRange);
	DeleteCriticalSection(&csMinRange);

	
	// 若为当前通道且展宽为真
	WORD nNum = 0;
	if(bZhanKuan && chNo == CurrentChannelNo)
	{
		float fCha = fBack - fFront;
			
		if(fCha < fMinRange)
		{
			nNum = WORD((fCha/fMinRange)*DATA_SIZE);
			xb = nWaveWidth/float(nNum);
		}
		else
		{
			xb = nWaveWidth/512.0f;
			nNum = DATA_SIZE;
		}
	}
	else if(!bZhanKuan)
	{
		// 检查当前声程差
		if(DisplaySet_ad[CurrentChannelNo].fRange < fMinRange)
		{
			nNum = WORD((DisplaySet_ad[CurrentChannelNo].fRange/fMinRange)*DATA_SIZE);
			xb = nWaveWidth/float(nNum);
		}
		else
		{
			xb = nWaveWidth/512.0f;
			nNum = DATA_SIZE;
		}		
	}

	InitializeCriticalSection(&cs110Display);
	EnterCriticalSection(&cs110Display);
	
	BOOL b110Display = gb110Display;
	LeaveCriticalSection(&cs110Display);
	DeleteCriticalSection(&cs110Display);

	InitializeCriticalSection(&csDisplayParam);
	EnterCriticalSection(&csDisplayParam);

	BOOL bSFlag = DisplaySet_ad[chNo].bSFlag;
	float fAngle = DisplaySet_ad[chNo].fZheSheJiao/180.0f * PI;

	LeaveCriticalSection(&csDisplayParam);
	DeleteCriticalSection(&csDisplayParam);

	InitializeCriticalSection(&csSendParam);
	EnterCriticalSection(&csSendParam);
	float fQianYan = SendParam_ad[chNo].fQianYan;
	LeaveCriticalSection(&csSendParam);
	DeleteCriticalSection(&csSendParam);

	float fFrontGate = 0.0f;
	float fBackGate = 0.0f;

	for(int i = 0; i<3; i++)
	{
		InitializeCriticalSection(&csGate);
		EnterCriticalSection(&csGate);

		fFrontGate = GateInfos[chNo][i].fFrontGate;
		fBackGate = GateInfos[chNo][i].fBackGate;

		
		if(!bZhanKuan)
		{
			Left[i] = (fFrontGate - fRangeStart[chNo])/(fRangeEnd[chNo] - fRangeStart[chNo])*nWaveWidth + RctWave.left;
			Right[i] = (fBackGate - fRangeStart[chNo])/(fRangeEnd[chNo] - fRangeStart[chNo])*nWaveWidth + RctWave.left;
		}
		else
		{
			Left[i] = (fFrontGate - fFront)/(fBack - fFront)*nWaveWidth + RctWave.left;
			Right[i] = (fBackGate - fFront)/(fBack - fFront)*nWaveWidth + RctWave.left;
		}

		if(!b110Display)
			Top[i] = float(nWaveHeight)*(100-GateInfos[chNo][i].fPeakGate)/100.0f + RctWave.top; 
		else
			Top[i] = float(nWaveHeight)*(110-GateInfos[chNo][i].fPeakGate)/110.0f + RctWave.top;

		bOpen[i] = GateInfos[chNo][i].bOpen;
		LeaveCriticalSection(&csGate);
		DeleteCriticalSection(&csGate);
	}

	InitializeCriticalSection(&csBaoluo);
	EnterCriticalSection(&csBaoluo);
	BOOL bBaoluo = gbBaoluo;
	LeaveCriticalSection(&csBaoluo);
	DeleteCriticalSection(&csBaoluo);

	// 画波形曲线	
	WORD nWaveColor[2] = {0,0};
	WORD nZhanKuanColor = 0;


	if(nJiOu == 0)
	{
		nWaveColor[0] = RGBtoRGB565(SysColor.clrCh1Wave);
		nWaveColor[1] = RGBtoRGB565(SysColor.clrCh2Wave);

		nGateColor[0][0] = RGB888toRGB565(255,0,0);
		nGateColor[0][1] = RGB888toRGB565(0,255,0);
		nGateColor[0][2] = RGB888toRGB565(0,0,255); 
		nGateColor[1][0] = RGB888toRGB565(255,255,0);
		nGateColor[1][1] = RGB888toRGB565(0,255,255);
		nGateColor[1][2] = RGB888toRGB565(0,255,255); 

		nZhanKuanColor = RGB888toRGB565(255,0,0);
		
	}
	else
	{
		nWaveColor[0] = RGBtoRGB565(SysColor.clrCh1WaveLike);
		nWaveColor[1] = RGBtoRGB565(SysColor.clrCh2WaveLike);

		nGateColor[0][0] = RGB888toRGB565(255,31,0);
		nGateColor[0][1] = RGB888toRGB565(0,255,31);
		nGateColor[0][2] = RGB888toRGB565(31,0,255); 
		nGateColor[1][0] = RGB888toRGB565(255,255,31);
		nGateColor[1][1] = RGB888toRGB565(31,255,255);
		nGateColor[1][2] = RGB888toRGB565(31,255,255);

		nZhanKuanColor = RGB888toRGB565(255,31,0);

	}

	POINT tmpPreData[DATA_SIZE];
	memcpy(tmpPreData, preData, sizeof(tmpPreData));	

	for (int i = 0;i < nNum; i++)
	{
 		if (gPicData[chNo].data[i] > MAX_HEIGHT) 
 			gPicData[chNo].data[i] = MAX_HEIGHT;
		
		long x = long(RctWave.left+i*xb);
		long y = long((nWaveHeight)-gPicData[chNo].data[i]*yb) + RctWave.top;

		if(i>0)
			Line(tmpPreData[i-1].x, tmpPreData[i-1].y, x, y, nWaveColor[chNo]);

		tmpPreData[i].x = x;
		tmpPreData[i].y = y;
	}					

	int tmpPreLeft[3] = {0,0,0};
	int tmpPreRight[3] = {0,0,0};
	int tmpPreTop[3] = {0,0,0};
	BOOL tmpPrebOpen[3] = {FALSE, FALSE, FALSE};

	// 闸门线	
	for(int i=0; i<2; i++)
	{
		if(bOpen[i])
		{
			if(Left[i] >= RctWave.left && Right[i] <= RctWave.right)
			{
				// 前沿
//				Line(Left[i], RctWave.bottom, Left[i], Top[i], nGateColor[chNo][i]);
				
				// 峰值
				Line3(Left[i], Top[i], Right[i], Top[i], nGateColor[chNo][i]);
				
				// 后沿
//				Line(Right[i], Top[i], Right[i], RctWave.bottom, nGateColor[chNo][i]);
			}
			else if(Left[i] < RctWave.left && (Right[i] >= RctWave.left && Right[i] <= RctWave.right))
			{
				// 峰值
				Line3(RctWave.left, Top[i], Right[i], Top[i], nGateColor[chNo][i]);
				
				// 后沿
//				Line(Right[i], Top[i], Right[i], RctWave.bottom, nGateColor[chNo][i]);
			}
			else if(Right[i] > RctWave.right && (Left[i] >= RctWave.left && Left[i] <= RctWave.right))
			{
				// 前沿
//				Line(Left[i], RctWave.bottom, Left[i], Top[i], nGateColor[chNo][i]);
				
				// 峰值
				Line3(Left[i], Top[i], RctWave.right, Top[i], nGateColor[chNo][i]);
			}
			else if(Left[i] <= RctWave.left && Right[i] >= RctWave.right)
			{
				// 峰值
				Line3(RctWave.left, Top[i], RctWave.right, Top[i], nGateColor[chNo][i]);
			}
		}

		tmpPreLeft[i] = Left[i];
		tmpPreRight[i] = Right[i];
		tmpPreTop[i] = Top[i];
		tmpPrebOpen[i] = bOpen[i];
	}

	int tmpPreZhanKuanFront = 0;
	int tmpPreZhanKuanBack = 0;

	// 等待显存更新数据
	Sleep(5);
	// 擦线
	if(nLoopIndex > 0 && (!bBaoluo))
	{
		if(!bHuiBo)	// 非回波状态
		{
			for (int i = 0;i < nNum; i++)
			{
				if(i > 0)
				{
					EraseLine(preData[i-1].x, preData[i-1].y, preData[i].x, preData[i].y, RGBtoRGB565(SysColor.clrWaveBack), nWaveColor[chNo]);
				}
			}

			for(int i=0; i<2; i++)
			{
				// 闸门线
				if(bOpen[i] || (bOpen[i] == FALSE && prebOpen[i] == TRUE))
				{
					if(preLeft[i] >= RctWave.left && preRight[i] <= RctWave.right)
					{
						// 前沿
//						EraseLine(preLeft[i], RctWave.bottom, preLeft[i], preTop[i], 0, nGateColor[chNo][i]);
						// 峰值
						EraseLine3(preLeft[i], preTop[i], preRight[i], preTop[i], RGBtoRGB565(SysColor.clrWaveBack), nGateColor[chNo][i]);
						// 后沿
//						EraseLine(preRight[i], preTop[i], preRight[i], RctWave.bottom, 0, nGateColor[chNo][i]);
					}
					else if(preLeft[i] < RctWave.left && 
						(preRight[i] >= RctWave.left && preRight[i] <= RctWave.right))
					{
						// 峰值
						EraseLine3(RctWave.left, preTop[i], preRight[i], preTop[i], RGBtoRGB565(SysColor.clrWaveBack), nGateColor[chNo][i]);
						// 后沿
//						EraseLine(preRight[i], preTop[i], preRight[i], RctWave.bottom, 0, nGateColor[chNo][i]);
					}
					else if(preRight[i] > RctWave.right && 
						(preLeft[i] >= RctWave.left && preLeft[i] <= RctWave.right))
					{
						// 前沿
//						EraseLine(preLeft[i], RctWave.bottom, preLeft[i], preTop[i], 0, nGateColor[chNo][i]);
						// 峰值
						EraseLine3(preLeft[i], preTop[i], RctWave.right, preTop[i], RGBtoRGB565(SysColor.clrWaveBack), nGateColor[chNo][i]);
					}
					else if(preLeft[i] <= RctWave.left && preRight[i] >= RctWave.right)
					{
						// 峰值
						EraseLine3(RctWave.left, preTop[i], RctWave.right, preTop[i], RGBtoRGB565(SysColor.clrWaveBack), nGateColor[chNo][i]);
					}
				}							
			}

		}
		else // 回波状态
		{
			for (int i = 0;i < nNum; i++)
			{
				if(i > 0)
				{
					WORD nColor = GetCurrentBKColor(preData[i].x, HuiBoInfo);
					EraseLine(preData[i-1].x, preData[i-1].y, preData[i].x, preData[i].y, nColor, nWaveColor[chNo]);
				}
			}
		}
		
	}

	// 赋值前-帧数据点
	memcpy(preData, tmpPreData, sizeof(tmpPreData));

	memcpy(preLeft, tmpPreLeft, sizeof(tmpPreLeft));
	memcpy(preRight, tmpPreRight, sizeof(tmpPreRight));
	memcpy(preTop, tmpPreTop, sizeof(tmpPreTop));
	memcpy(prebOpen, tmpPrebOpen, sizeof(tmpPrebOpen));

	preZhanKuanFront = tmpPreZhanKuanFront;
	preZhanKuanBack = tmpPreZhanKuanBack;

	// wangbingfu add start 2010-06-04
	if(chNo == CurrentChannelNo)
	{
		if(gWaveDrawTcgFlag == TRUE)
		{
			POINT stTcgPoint[MAX_CURVE_POINT_NUM];
			for(int i = 0; i < gSelectedDacPointDataNum[CurrentChannelNo]; i++)
			{
				stTcgPoint[i] = dataToPoint1(gSelectedDacPointData[CurrentChannelNo][i], RctWave);
				if(i == gwDeleteStep && gbDeletePointFlag)
				{
					drawShiZhi1(stTcgPoint[i], RED_POINT, TRUE);//画选中点
					continue;
				}
				else
					drawShiZhi1(stTcgPoint[i], BLUE_POINT, TRUE);//画点
			}
			drawCurve3(gSelectedDacPointData[CurrentChannelNo], gSelectedDacPointDataNum[CurrentChannelNo], RED_POINT, TRUE, RctWave);
			gTcgClearFlag = FALSE;
		}
		else
		{
			if(gTcgClearFlag == FALSE)
			{
				POINT stTcgPoint[MAX_CURVE_POINT_NUM];
				for(int i = 0; i < gSelectedDacPointDataNum[CurrentChannelNo]; i++)
				{
					stTcgPoint[i] = dataToPoint1(gSelectedDacPointData[CurrentChannelNo][i], RctWave);
					drawShiZhi1(stTcgPoint[i], BLUE_POINT, FALSE);//画点
				}
				
				drawCurve3(gSelectedDacPointData[CurrentChannelNo], gSelectedDacPointDataNum[CurrentChannelNo], RED_POINT, FALSE, RctWave);
			
				gTcgClearFlag = TRUE;
			}
		}
	}
	// wangbingfu add start 2010-06-18
	if(chNo == CurrentChannelNo)
	{
		if(gWaveDrawDacFlag == TRUE)
		{
			POINT stDgsPoint[MAX_CURVE_POINT_NUM];
			for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
			{
				stDgsPoint[i] = dataToPoint1(gSelectedDgsPointData[CurrentChannelNo][3][i], RctWave);
				if(i == gwDeleteStep && gbDeletePointFlag)
				{
					drawShiZhi1(stDgsPoint[i], RED_POINT, TRUE);//画选中点
					continue;
				}
				else
					drawShiZhi1(stDgsPoint[i], BLUE_POINT, TRUE);//画点
			}
			drawCurve3(gSelectedDgsPointData[CurrentChannelNo][3], gSelectedDgsPointDataNum[CurrentChannelNo][3], RED_POINT, TRUE, RctWave);

			gDacClearFlag = FALSE;
		}
		else
		{
			if(gDacClearFlag == FALSE)
			{
				POINT stDgsPoint[MAX_CURVE_POINT_NUM];
				for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
				{
					stDgsPoint[i] = dataToPoint1(gSelectedDgsPointData[CurrentChannelNo][3][i], RctWave);
					drawShiZhi1(stDgsPoint[i], BLUE_POINT, FALSE);//画点
				}
				
				drawCurve3(gSelectedDgsPointData[CurrentChannelNo][3], gSelectedDgsPointDataNum[CurrentChannelNo][3], RED_POINT, FALSE, RctWave);
				
				gDacClearFlag = TRUE;
			}
		}
	}
	// wangbingfu add end at 2010-06-18
	if(chNo == CurrentChannelNo)
	{
		if(gWaveDrawAvgFlag == TRUE)
		{
			POINT stAvgPoint[MAX_CURVE_POINT_NUM];
			for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
			{
				stAvgPoint[i] = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][3][i], RctWave);
				if(i == gwDeleteStep && gbDeletePointFlag)
				{
					drawShiZhi1(stAvgPoint[i], RED_POINT, TRUE);//画选中点
					continue;
				}
				else
					drawShiZhi1(stAvgPoint[i], BLUE_POINT, TRUE);//画点
			}
			drawCurve3(gSelectedAvgPointData[CurrentChannelNo][3], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, TRUE, RctWave);

			gAvgClearFlag = FALSE;
		}
		else
		{
			if(gAvgClearFlag == FALSE)
			{
				POINT stAvgPoint[MAX_CURVE_POINT_NUM];
				for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
				{
					stAvgPoint[i] = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][3][i], RctWave);
					drawShiZhi1(stAvgPoint[i], BLUE_POINT, FALSE);//画点
				}
				
				drawCurve3(gSelectedAvgPointData[CurrentChannelNo][3], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE, RctWave);
			
				gAvgClearFlag = TRUE;
			}
		}
	}
	// wangbingfu add end at 2010-12-02
	// wangbingfu add start at 2010-12-03
	//画AVG曲线
	if(gbAvgCurveFlag[chNo])
	{
		if(gSelectedAvgPointDataNum[chNo] >= 2)
		{
			if(bgAvgChangeFlag == FALSE)
			{
				for(int j = 0; j < AVG_CURVE_NUM; j++)
				{
				/*	if(chNo == 0)//通道1
					{	
						if(j == 0 && gfDefectZhi1[chNo] != 0)					
							drawCurve3(gSelectedAvgPointData[chNo][j], gSelectedAvgPointDataNum[chNo], wAvgColor[j], TRUE, RctWave);
						else if(j == 1 && gfDefectZhi2[chNo] != 0)
							drawCurve3(gSelectedAvgPointData[chNo][j], gSelectedAvgPointDataNum[chNo], wAvgColor[j], TRUE, RctWave);
						else if(j == 2 && gfDefectZhi3[chNo] != 0)
							drawCurve3(gSelectedAvgPointData[chNo][j], gSelectedAvgPointDataNum[chNo], wAvgColor[j], TRUE, RctWave);
					}
					else if(chNo == 1)//通道2
					{
						if(j == 0 && gfDefectZhi1[chNo] != 0)					
							drawDashCurve3(gSelectedAvgPointData[chNo][j], gSelectedAvgPointDataNum[chNo], wAvgColor[j], TRUE, RctWave);
						else if(j == 1 && gfDefectZhi2[chNo] != 0)
							drawDashCurve3(gSelectedAvgPointData[chNo][j], gSelectedAvgPointDataNum[chNo], wAvgColor[j], TRUE, RctWave);
						else if(j == 2 && gfDefectZhi3[chNo] != 0)
							drawDashCurve3(gSelectedAvgPointData[chNo][j], gSelectedAvgPointDataNum[chNo], wAvgColor[j], TRUE, RctWave);
					}*/
					if(j == 0 && gfDefectZhi1[chNo] != 0)					
						drawCurve3(gSelectedAvgPointData[chNo][j], gSelectedAvgPointDataNum[chNo], wAvgColor[chNo][j], TRUE, RctWave);
					else if(j == 1 && gfDefectZhi2[chNo] != 0)
						drawCurve3(gSelectedAvgPointData[chNo][j], gSelectedAvgPointDataNum[chNo], wAvgColor[chNo][j], TRUE, RctWave);
					else if(j == 2 && gfDefectZhi3[chNo] != 0)
						drawCurve3(gSelectedAvgPointData[chNo][j], gSelectedAvgPointDataNum[chNo], wAvgColor[chNo][j], TRUE, RctWave);
					
					RECT stTextRect;
					stTextRect.left = RctWave.left + 2;
					stTextRect.right = RctWave.left + 60;
					POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[chNo][j][0], RctWave);
					stTextRect.top = stFirstPoint.y - 25;
					stTextRect.bottom = stFirstPoint.y;
					if(j == 0 && gfDefectZhi1[chNo] != 0)
					{
						wchar_t strText[20];
						memset(strText, 0, sizeof(strText));

						wsprintf(strText, _T("Φ%.1f"), gfDefectZhi1[chNo]);
						
						DrawWaveAreaText(hdc, RctWave, strText, hFontInfo, RGB(255,255,255), stTextRect.left, stTextRect.top  ,58, 25);
					}
					else if(j == 1 && gfDefectZhi2[chNo] != 0)
					{
						wchar_t strText[20];
						memset(strText, 0, sizeof(strText));

						wsprintf(strText, _T("Φ%.1f"), gfDefectZhi2[chNo]);

						DrawWaveAreaText(hdc, RctWave, strText, hFontInfo, RGB(255,255,255), stTextRect.left, stTextRect.top  ,58, 25);
					}
					else if(j == 2 && gfDefectZhi3[chNo] != 0)
					{
						wchar_t strText[20];
						memset(strText, 0, sizeof(strText));

						wsprintf(strText, _T("Φ%.1f"), gfDefectZhi3[chNo]);

						DrawWaveAreaText(hdc, RctWave, strText, hFontInfo, RGB(255,255,255), stTextRect.left, stTextRect.top  ,58, 25);
					}
				}
				gbAvgDrawFlag[chNo] = TRUE;
			}
		}
	}
	else
	{
		if(gbAvgDrawFlag[chNo] == TRUE)
		{
			for(int j = 0; j < AVG_CURVE_NUM; j++)
			{
				/*if(chNo == 0)//通道1
				{						
					drawCurve3(gSelectedAvgPointData[chNo][j], gSelectedAvgPointDataNum[chNo], RED_POINT, FALSE, RctWave);//擦掉曲线
				}
				else if(chNo == 1)//通道2
				{
					drawDashCurve3(gSelectedAvgPointData[chNo][j], gSelectedAvgPointDataNum[chNo], RED_POINT, FALSE, RctWave);//擦掉曲线
				}*/
				drawCurve3(gSelectedAvgPointData[chNo][j], gSelectedAvgPointDataNum[chNo], RED_POINT, FALSE, RctWave);//擦掉曲线

				RECT stTextRect;
				stTextRect.left = RctWave.left + 2;
				stTextRect.right = RctWave.left + 60;
				POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[chNo][j][0], RctWave);
				stTextRect.top = stFirstPoint.y - 25;
				stTextRect.bottom = stFirstPoint.y;
				
				WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
				HBRUSH hbr = CreateSolidBrush(wColorBack);
				FillRect(hdc, &stTextRect, hbr);
				DeleteObject(hbr);
			}
		}
		gbAvgDrawFlag[chNo] = FALSE;
	}
	
	// wangbingfu add end at 2010-12-03
	// wangbingfu add start 2010-07-08
	//画DAC闸门线
	if(gDgsSwitch[chNo][0]  == 1)
	{
		for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
		{
			
			if(gSelectedDgsPointDataNum[chNo][j] > 1)
			{						
				//重新画线
				if(gDacChangeFlag == FALSE)
				{
					/*if(chNo == 0)
						drawCurve3(gSelectedDgsPointData[chNo][j], gSelectedDgsPointDataNum[chNo][j], nDacColor[j], TRUE, RctWave);
					else if(chNo == 1)
						drawDashCurve3(gSelectedDgsPointData[chNo][j], gSelectedDgsPointDataNum[chNo][j], nDacColor[j], TRUE, RctWave);
						*/
					drawCurve3(gSelectedDgsPointData[chNo][j], gSelectedDgsPointDataNum[chNo][j], nDacColor[chNo][j], TRUE, RctWave);
				}
			}
			gDgsGateDrawFlag[chNo] = TRUE;
		}				
	}
	else
	{
		for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
		{
			if(gDgsGateDrawFlag[chNo] == TRUE)
			{ 
			/*	if(chNo == 0)
					drawCurve3(gSelectedDgsPointData[chNo][j], gSelectedDgsPointDataNum[chNo][j], nDacColor[j], FALSE, RctWave);
				else if(chNo == 1)
					drawDashCurve3(gSelectedDgsPointData[chNo][j], gSelectedDgsPointDataNum[chNo][j], nDacColor[j], FALSE, RctWave);
				*/
				drawCurve3(gSelectedDgsPointData[chNo][j], gSelectedDgsPointDataNum[chNo][j], nDacColor[chNo][j], FALSE, RctWave);
			}
		}
		gDgsGateDrawFlag[chNo] = FALSE;
	}		

	// wangbingfu add end 2010-07-08
	
	// 画参考波
	InitializeCriticalSection(&csRefWave);
	EnterCriticalSection(&csRefWave);
	
	BOOL bDisplayRefWave = g_bDisplayRefWave;
	LeaveCriticalSection(&csRefWave);
	DeleteCriticalSection(&csRefWave);

	if(bDisplayRefWave)
	{
		InitializeCriticalSection(&csRefWaveRange);
		EnterCriticalSection(&csRefWaveRange);
		float fStart = gfRefWaveStart;
		float fEnd = gfRefWaveEnd;
		LeaveCriticalSection(&csRefWaveRange);
		DeleteCriticalSection(&csRefWaveRange);

		// 计算512点的坐标，若超出波形显示区域则不画

		//float xStep = (fEnd - fStart) / (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * nWaveWidth / 512.0f;
		float xStep = (fEnd - fStart) / (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * nWaveWidth / nNum;//wangbingfu modify at2011-3-31要做展宽处理

		int xStart = int((fStart - fRangeStart[CurrentChannelNo])/ (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * nWaveWidth) + RctWave.left;
		
		long preX = 0;
		long preY = 0;
		for(int i=0; i<DATA_SIZE; i++)
		{
			long x = xStart + int(i*xStep);
			long y = long((nWaveHeight-1)-gRefWaveData[i]*yb) + RctWave.top;

			if(x > RctWave.right)
				break;

			if(i>0 && preX >= RctWave.left)
				Line(preX, preY, x, y, 31);

			preX = x;
			preY = y;

		}

	}
}

void InitializeSysColor(SYS_COLOR_INFO* ColorArr, const int& nSize)
{
	// 第一项为构造函数设置的默认颜色

	// 2
	ColorArr[1].clrScaleBack = RGB(228,230,235);
	ColorArr[1].clrWaveBack = RGB(56,179,213);
	ColorArr[1].clrCh1Wave = RGB(255,255,0);
	ColorArr[1].clrCh2Wave = RGB(255,0,0);
	ColorArr[1].clrCh1WaveLike = RGB(255,255,31);
	ColorArr[1].clrCh2WaveLike = RGB(255,0,31);

	// 3
	ColorArr[2].clrScaleBack = RGB(255,255,255);
	ColorArr[2].clrWaveBack = RGB(0,146,63);
	ColorArr[2].clrCh1Wave = RGB(255,0,0);
	ColorArr[2].clrCh2Wave = RGB(255,255,0);
	ColorArr[2].clrCh1WaveLike = RGB(255,0,31);
	ColorArr[2].clrCh2WaveLike = RGB(255,255,31);

	// 4
	ColorArr[3].clrScaleBack = RGB(222,233,237);
	ColorArr[3].clrWaveBack = RGB(0,106,172);
	ColorArr[3].clrCh1Wave = RGB(221,19,123);
	ColorArr[3].clrCh2Wave = RGB(188,167,82);
	ColorArr[3].clrCh1WaveLike = RGB(221,19,154);
	ColorArr[3].clrCh2WaveLike = RGB(188,167,113);

	// 5
	ColorArr[4].clrCh1Wave = RGB(189,154,80);
	ColorArr[4].clrCh2Wave = RGB(52,115,203);
	ColorArr[4].clrCh1WaveLike = RGB(189,154,111);
	ColorArr[4].clrCh2WaveLike = RGB(52,115,234);
}

// 计算伤时垂直方向值
float CalcFlawTimeVetical(const float& fFlawTime, const float& fPlateHeight, const float& fZheSheJiao)
{
	float fFlawTimeV = 0.0f;
	float fAngle = float(fZheSheJiao/180.0f * PI);	
	if(cos(fAngle) < 0.00001)
		return 0.0f;

	float fOneLength = fPlateHeight / cos(fAngle);
	
	int nCount = int(fFlawTime / fOneLength);
	int nJiOu = nCount % 2;
	if(nJiOu == 0)
		fFlawTimeV = (fFlawTime - (nCount*fOneLength)) * cos(fAngle);
	else
		fFlawTimeV = fPlateHeight - (fFlawTime - (nCount*fOneLength)) * cos(fAngle);

	return fFlawTimeV;
}

// 显示报警信息
void DrawAlarmInfo( HBRUSH hbrWaveBack, 
				   CRITICAL_SECTION& csTextInfo,
				   HDC hdc,
				   HDC hMemDC, 
				   HFONT hFontInfo,
				   const RECT& RctWave,
				   const WORKMODE& mode)
{
	HBRUSH hbr = hbrWaveBack;

	COLORREF clrOld =  SetTextColor( hMemDC,  RGB(255,255,255));

	EnterCriticalSection(&csTextInfo);

	HFONT hFont = (HFONT) SelectObject (hMemDC, hFontInfo) ;

	wchar_t szText[10];
	memset(szText, 0, sizeof(szText));

	// 获取要显示的PeakInfo
	PEAK_INFO stPeakInfo;
	InitializeCriticalSection(&csGate);
	EnterCriticalSection(&csGate);
	BOOL bOpen = FALSE;

	int nChannelNo = -1;
	InitializeCriticalSection(&csPeakDefectInfos);
	EnterCriticalSection(&csPeakDefectInfos);

	if(mode == DoubleChannels)
	{
		if(GateInfos[0][0].bOpen && !GateInfos[0][1].bOpen && 
		   !GateInfos[1][0].bOpen && !GateInfos[1][1].bOpen)
		{
			bOpen = TRUE;
			stPeakInfo.wDefectAmp = gPeakInfosRemeber[0][0].wDefectAmp;
			stPeakInfo.wDefectTime =gPeakInfosRemeber[0][0].wDefectTime;

			nChannelNo = 0;
		}
		else if(!GateInfos[0][0].bOpen && GateInfos[0][1].bOpen && 
		   !GateInfos[1][0].bOpen && !GateInfos[1][1].bOpen)
		{
			bOpen = TRUE;
			stPeakInfo.wDefectAmp = gPeakInfosRemeber[0][1].wDefectAmp;
			stPeakInfo.wDefectTime =gPeakInfosRemeber[0][1].wDefectTime;

			nChannelNo = 0;
		}
		else if(!GateInfos[0][0].bOpen && !GateInfos[0][1].bOpen && 
		   GateInfos[1][0].bOpen && !GateInfos[1][1].bOpen)
		{
			bOpen = TRUE;
			stPeakInfo.wDefectAmp = gPeakInfosRemeber[1][0].wDefectAmp;
			stPeakInfo.wDefectTime =gPeakInfosRemeber[1][0].wDefectTime;

			nChannelNo = 1;
		}
		else if(!GateInfos[0][0].bOpen && !GateInfos[0][1].bOpen && 
		   !GateInfos[1][0].bOpen && GateInfos[1][1].bOpen)
		{
			bOpen = TRUE;
			stPeakInfo.wDefectAmp = gPeakInfosRemeber[1][1].wDefectAmp;
			stPeakInfo.wDefectTime =gPeakInfosRemeber[1][1].wDefectTime;

			nChannelNo = 1;
		}
		else
			bOpen = FALSE;
	}
	else if(mode == SingleChannelA || mode == SingleChannelB)
	{
		if(GateInfos[CurrentChannelNo][0].bOpen && !GateInfos[CurrentChannelNo][1].bOpen)
		{
			bOpen = TRUE;
			stPeakInfo.wDefectAmp = gPeakInfosRemeber[CurrentChannelNo][0].wDefectAmp;
			stPeakInfo.wDefectTime =gPeakInfosRemeber[CurrentChannelNo][0].wDefectTime;
		}
		else if(!GateInfos[CurrentChannelNo][0].bOpen && GateInfos[CurrentChannelNo][1].bOpen )
		{
			bOpen = TRUE;
			stPeakInfo.wDefectAmp = gPeakInfosRemeber[CurrentChannelNo][1].wDefectAmp;
			stPeakInfo.wDefectTime =gPeakInfosRemeber[CurrentChannelNo][1].wDefectTime;
		}
		else
			bOpen = FALSE;
	}
	else
		bOpen = FALSE;
	
	LeaveCriticalSection(&csPeakDefectInfos);
	DeleteCriticalSection(&csPeakDefectInfos);

	LeaveCriticalSection(&csGate);
	DeleteCriticalSection(&csGate);
	
	RECT rect;
	rect.left = RctWave.right - 120;
	rect.right = RctWave.right;
	rect.top = RctWave.top;
	rect.bottom = RctWave.top + 80;

	RECT RctInfo;
	static BOOL bDrawed = FALSE;	// 标志是否画过报警

	InitializeCriticalSection(&csProbeType);
	EnterCriticalSection(&csProbeType);

	WORD bTanTouType = 2;
	if(mode == DoubleChannels && bOpen)
		bTanTouType = wTanTouLeiXing[nChannelNo];
	else
		bTanTouType = wTanTouLeiXing[CurrentChannelNo];
	
	LeaveCriticalSection(&csProbeType);
	DeleteCriticalSection(&csProbeType);

	if(bTanTouType == 0 && bOpen)	// 斜探头
	{
		FillRect(hMemDC, &rect, hbr);
		if(gbQuMianXiuZhengFlag)// 曲率修正打开
		{
			// h 深度				
			RctInfo.left = RctWave.right - 120;
			RctInfo.right = RctWave.right;
			RctInfo.top = RctWave.top;
			RctInfo.bottom = RctWave.top + 20;

			// 计算值
			QUMIANXIUZHENG_INFO info = quMianXiuZheng(stPeakInfo.wDefectTime, (gfWaiJing / 2), (gfNeiJing / 2), DisplaySet_ad[CurrentChannelNo].fZheSheJiao, SendParam_ad[CurrentChannelNo].fQianYan);

			FillRect(hMemDC, &RctInfo, hbr);
			swprintf(szText, _T("H: %.2f"),info.fShenDu );

			DrawText(hMemDC, szText, -1, &RctInfo, 
				DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			
			// L 弧长
			OffsetRect(&RctInfo, 0, (RctInfo.bottom - RctInfo.top));
			FillRect(hMemDC, &RctInfo, hbr);
			memset(szText, 0, sizeof(szText));
			swprintf(szText, _T("L: %.2f"), info.fLength);

			DrawText(hMemDC, szText, -1, &RctInfo, 
				DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			
			// A 
			OffsetRect(&RctInfo, 0, (RctInfo.bottom - RctInfo.top));
			FillRect(hMemDC, &RctInfo, hbr);
			memset(szText, 0, sizeof(szText));

			if(gDgsSwitch[CurrentChannelNo][0])	// dB
			{
				// 计算
				float fValue = dacDeltaDB(stPeakInfo.wDefectAmp, stPeakInfo.wDefectTime, gSelectedDgsPointData[CurrentChannelNo][1], gSelectedDgsPointDataNum[CurrentChannelNo][1]);
				// 显示
				swprintf( szText,_T("△dB: %.2f"),fValue);
			}
			else if(gbAvgCurveFlag[CurrentChannelNo])	// Φ
			{
				float fValue = calculateFaiZhi(stPeakInfo.wDefectAmp, stPeakInfo.wDefectTime,gSelectedAvgPointData[CurrentChannelNo][0], gSelectedAvgPointDataNum[CurrentChannelNo]);
				swprintf( szText,_T("Φ: %.2f"),fValue);
			}
			else	// %
			{
				InitializeCriticalSection(&cs110Display);
				EnterCriticalSection(&cs110Display);			
				BOOL b110Display = gb110Display;
				LeaveCriticalSection(&cs110Display);
				DeleteCriticalSection(&cs110Display);

				if(!b110Display)
					swprintf( szText,_T("A: %.2f"),  float(stPeakInfo.wDefectAmp)/MAX_HEIGHT*100.0f );
				else
					swprintf( szText,_T("A: %.2f"),  float(stPeakInfo.wDefectAmp)/MAX_HEIGHT*110.0f );
			}

			DrawText(hMemDC, szText, -1, &RctInfo, 
				DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			// 复制
			BitBlt(hdc, rect.left, rect.top, (rect.right - rect.left),
				(rect.bottom - rect.top), hMemDC, rect.left, rect.top, SRCCOPY);
		}
		else
		{			
			// 贴图，写字	
			if(gDgsSwitch[CurrentChannelNo][0])
			{
				CopyBmp(hMemDC, IDB_BITMAP_OBLIQUE_PROBE_ALARM_INFO_DB_R, rect);
			}
			else if(gbAvgCurveFlag[CurrentChannelNo])
			{
				CopyBmp(hMemDC, IDB_BITMAP_OBLIQUE_PROBE_ALARM_INFO_FAIZHI_R, rect);
			}
			else
			{
				CopyBmp(hMemDC, IDB_BITMAP_OBLIQUE_PROBE_ALARM_INFO_R, rect);
			}

			RctInfo.left = RctWave.right - 80;
			RctInfo.right = RctWave.right;
			RctInfo.top = RctWave.top;
			RctInfo.bottom = RctWave.top + 20;

			float fAngle = DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI;
			// 伤时
			FillRect(hMemDC, &RctInfo, hbr);
			float fFlawTime = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, stPeakInfo.wDefectTime);
			swprintf(szText, _T("%.2f"), fFlawTime);

			DrawText(hMemDC, szText, -1, &RctInfo, 
				DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			// 伤时(水平)				
			OffsetRect(&RctInfo, 0, (RctInfo.bottom - RctInfo.top));
			FillRect(hMemDC, &RctInfo, hbr);
			memset(szText, 0, sizeof(szText));
			swprintf(szText, _T("%.2f"), fFlawTime * sin(fAngle));
			DrawText(hMemDC, szText, -1, &RctInfo, 
				DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			// 伤时(垂直)				
			OffsetRect(&RctInfo, 0, (RctInfo.bottom - RctInfo.top));
			FillRect(hMemDC, &RctInfo, hbr);
			memset(szText, 0, sizeof(szText));

			// 计算伤时（垂直）
			float fFlawTimeV = CalcFlawTimeVetical(fFlawTime, gfPlateHeight, DisplaySet_ad[CurrentChannelNo].fZheSheJiao);

			swprintf(szText, _T("%.2f"), fFlawTimeV);
			DrawText(hMemDC, szText, -1, &RctInfo, 
				DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			// 伤幅				
			OffsetRect(&RctInfo, 0, (RctInfo.bottom - RctInfo.top));
			FillRect(hMemDC, &RctInfo, hbr);
			memset(szText, 0, sizeof(szText));

			if(gDgsSwitch[CurrentChannelNo][0])	// dB
			{
				// 计算
				float fValue = dacDeltaDB(stPeakInfo.wDefectAmp, stPeakInfo.wDefectTime, gSelectedDgsPointData[CurrentChannelNo][1], gSelectedDgsPointDataNum[CurrentChannelNo][1]);
				// 显示
				swprintf( szText,_T("%.2f"),fValue);
			}
			else if(gbAvgCurveFlag[CurrentChannelNo])	// Φ
			{
				float fValue = calculateFaiZhi(stPeakInfo.wDefectAmp, stPeakInfo.wDefectTime,gSelectedAvgPointData[CurrentChannelNo][0], gSelectedAvgPointDataNum[CurrentChannelNo]);
				swprintf( szText,_T("%.2f"),fValue);
			}
			else	// %
			{
				InitializeCriticalSection(&cs110Display);
				EnterCriticalSection(&cs110Display);			
				BOOL b110Display = gb110Display;
				LeaveCriticalSection(&cs110Display);
				DeleteCriticalSection(&cs110Display);

				if(!b110Display)
					swprintf( szText,_T("%.2f"),  float(stPeakInfo.wDefectAmp)/MAX_HEIGHT*100.0f );
				else
					swprintf( szText,_T("%.2f"),  float(stPeakInfo.wDefectAmp)/MAX_HEIGHT*110.0f );
			}				

			DrawText(hMemDC, szText, -1, &RctInfo, 
				DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			// 复制
			BitBlt(hdc, rect.left, rect.top, (rect.right - rect.left),
				(rect.bottom - rect.top), hMemDC, rect.left, rect.top, SRCCOPY);
		}

		bDrawed = TRUE;
	}
	else	
	{
		// 擦掉斜探头的报警信息
		if(bDrawed)
		{
			FillRect(hdc, &rect, hbr);
			bDrawed = FALSE;
		}

		TEXTMETRIC tm;
		GetTextMetrics(hMemDC, &tm);

		RctInfo.left = RctWave.right - 30 * tm.tmAveCharWidth;
		RctInfo.right = RctWave.right;
		RctInfo.top = RctWave.top;
		RctInfo.bottom = RctWave.top + (tm.tmHeight + tm.tmExternalLeading);

		for(int i=0; i<2; i++)
		{
			if((mode == SingleChannelB || mode == OneSendOneReceive) && i==0)
				continue;

			if(mode == SingleChannelA && i==1)
				continue;

			for(int j=0; j<2; j++)
			{
				//wangbingfu modify start at 2011-3-31
				if( gszInfo[i][j][0] != 0 )
				{
					FillRect(hMemDC, &RctInfo, hbr);

					DrawText (hMemDC, gszInfo[i][j], -1, &RctInfo, 
						DT_LEFT | DT_VCENTER | DT_SINGLELINE);

					BitBlt(hdc, RctInfo.left, RctInfo.top, (RctInfo.right - RctInfo.left),
							(RctInfo.bottom - RctInfo.top), hMemDC, RctInfo.left, RctInfo.top, SRCCOPY);

					OffsetRect(&RctInfo, 0, (RctInfo.bottom - RctInfo.top));
				}
				//wangbingfu modify end at 2011-3-31
			}				
		}
	}

	hFontInfo = (HFONT) SelectObject (hMemDC, hFont) ;
	LeaveCriticalSection(&csTextInfo);

	SetTextColor( hMemDC,  clrOld);

}

/** 
 * 画图线程
 * @param     lpParameter [in]:		线程参数.
 * @return    DWORD:
 * @since     1.00
 */
DWORD WINAPI DrawThreadProc(LPVOID lpParameter)
{
	DRAWTHREAD_PARAM* pParam = (DRAWTHREAD_PARAM*)lpParameter;
	wchar_t strZhenShu[20];		// 帧数字符串
	memset(strZhenShu, 0, sizeof(strZhenShu));

	static DWORD begin = 0;		// 计时开始值
	static int count = 0;		// 循环次数
	static int nDisplayCount = 0;
	HBITMAP hBitmap,hOldBitmap;
	HDC hdc = pParam->hDC;
	HDC hMemDC = CreateCompatibleDC(hdc);

	if(hMemDC == NULL)
		MessageBox(NULL, _T("hMemDC == NULL"), _T("error"), MB_OK);

	RECT rect;	// 主窗口显示出来的矩形
	GetClipBox(hdc, &rect); 

	int nWidth = rect.right - rect.left;
	int nHeight = rect.bottom;

	HBRUSH hbr;

	hBitmap = CreateCompatibleBitmap (hdc, nWidth, nHeight);
	
	RECT RctWave;	// 波形区域
	RECT RctYScale; // Y坐标区域
	RECT RctXScale; // X坐标区域
	RECT RctRight;  // 右侧空白区

	HPEN hOldPen = NULL;
	HPEN hLine1Pen = CreatePen(PS_SOLID, 2, RGB(255,255,0));
	HPEN hLine2Pen = CreatePen(PS_SOLID, 1, RGB(255,0,0));

	// 坐标用字体
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = TEXT_HEIGHT;        
    lf.lfWidth = 0;

	HFONT hFont = CreateFontIndirect(&lf);

	hFont = (HFONT) SelectObject (hMemDC, hFont) ;	
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
	
	POINT preData[2][DATA_SIZE];
//	memset(preData, 0, sizeof(preData));	
	float xb = (nWidth - 64)/DATA_SIZE;
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<DATA_SIZE; j++)
		{
			preData[i][j].x = long(32+j*xb);
			preData[i][j].y = 448;
		}
	}

	static int nLoopIndex = 0;
	BOOL bMenuShown = FALSE;
	BOOL bPreMenuState = bMenuShown;
	
	int preLeft[2][3] = {0,0,0,0,0,0};
	int preRight[2][3] = {0,0,0,0,0,0};
	int preTop[2][3] = {0,0,0,0,0,0};
	BOOL prebOpen[2][3] = {FALSE,FALSE,FALSE,FALSE,FALSE,FALSE};
	int preZhanKuanFront = 0;
	int preZhanKuanBack = 0;

	BOOL bHuiBo = FALSE;
	// 回波编码背景色
	const COLORREF clrHuiBoBack[10] = {RGB(95,206,234), RGB(32,85,138), RGB(26,39,145),RGB(0,99,190), RGB(0,38,100),
									RGB(95,206,234), RGB(32,85,138), RGB(26,39,145),RGB(0,99,190), RGB(0,38,100)};
	const WORD clrHuiBoLineBack[10] = {RGB888toRGB565(95,206,234), RGB888toRGB565(32,85,138), 
		RGB888toRGB565(26,39,145), RGB888toRGB565(0,99,190), RGB888toRGB565(0,38,100),
		RGB888toRGB565(95,206,234), RGB888toRGB565(32,85,138), 
		RGB888toRGB565(26,39,145), RGB888toRGB565(0,99,190), RGB888toRGB565(0,38,100)};
	vector<HUIBO_INFO> HuiBoInfo;
	HuiBoInfo.reserve( 10 );

	LOGFONT lfInfo;
	memset(&lfInfo, 0, sizeof(lfInfo));
	lfInfo.lfHeight = 24;        
	lfInfo.lfWidth = 0;

	HFONT hFontInfo = CreateFontIndirect(&lfInfo);

	// 初始化系统颜色
	SYS_COLOR_INFO SysColor[SYS_COLOR_NUM];

	InitializeSysColor(SysColor, SYS_COLOR_NUM);
	// 系统画刷， 波形颜色
	HBRUSH hbrWaveBack = NULL;
	HBRUSH hbrScaleBack = NULL;

	static WORD nColorIndex = 0;

	InitializeCriticalSection(&csTextInfo);
	InitializeCriticalSection(&csSysColor);

	while(1)
	{
		// 查询Menu状态
		bPreMenuState = bMenuShown;

		InitializeCriticalSection(&csMenuState);
		EnterCriticalSection(&csMenuState);
		bMenuShown = g_bMenuShown;
		LeaveCriticalSection(&csMenuState);
		DeleteCriticalSection(&csMenuState);

		if(bPreMenuState != bMenuShown)
			nLoopIndex = 0;

		InitializeCriticalSection(&csRedraw);
		EnterCriticalSection(&csRedraw);
		BOOL bRedraw = g_bRedraw;
		LeaveCriticalSection(&csRedraw);
		DeleteCriticalSection(&csRedraw);

		if(bRedraw)
		{
			nLoopIndex = 0;
			SetRedrawFlag(FALSE);
		}

		// 查询回波编码是否打开
		if(nLoopIndex == 0)
		{
			bHuiBo = FALSE;
			InitializeCriticalSection(&csHuiBoState);
			EnterCriticalSection(&csHuiBoState);
			
			for(int i=0; i<10; i++)
			{
				if(gbHuiBoOpen[i])
				{
					bHuiBo = TRUE;
					break;
				}

			}
			LeaveCriticalSection(&csHuiBoState);
			DeleteCriticalSection(&csHuiBoState);
		}

		// 初始化画刷
		EnterCriticalSection(&csSysColor);
		BOOL bInitColor = gbInitializeSysColor;
		LeaveCriticalSection(&csSysColor);
		
		if(bInitColor)
		{
			EnterCriticalSection(&csSysColor);
			nColorIndex = gCurrentSysColorIndex;
			gbInitializeSysColor = FALSE;
			LeaveCriticalSection(&csSysColor);

			DeleteObject(hbrWaveBack);
			hbrWaveBack = CreateSolidBrush(SysColor[nColorIndex].clrWaveBack);

			DeleteObject(hbrScaleBack);
			hbrScaleBack = CreateSolidBrush(SysColor[nColorIndex].clrScaleBack);			
		}

		DWORD ret = WaitForSingleObject(hDrawEvent, 0);

		#if 1
		if (ret == WAIT_OBJECT_0)
		{
		//	Sleep(5);
			ResetEvent(hDrawEvent);
			SetEvent(hWorkEvent);
			Sleep(20);//由100改为20   yachang.huang modify on 2013-12-2
		}
		#else
		if (ret == WAIT_OBJECT_0)
		{			
			if(count==60)
			{
				// 计算帧数
				swprintf(strZhenShu, _T("%.2f"), 60000.0f/float(GetTickCount() - begin));
				strZhenShu[19] = '\0';

				count = 0;
			}	

			// 画图
			// 填充各部分底色
			if(nLoopIndex == 0)
			{
				Sleep(100);
				GetDisplayRect(rect, bMenuShown, RctWave, RctXScale, RctYScale, RctRight);

				// 若回波编码未打开
				if(!bHuiBo)
				{

//					hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
					hbr = hbrWaveBack;
					FillRect(hMemDC, &RctWave, hbr);

				}
				else// 若打开
				{
					HuiBoInfo.erase( HuiBoInfo.begin(), HuiBoInfo.end() );
					// 计算各个区域的屏幕坐标，若在RctWave内的话，填对应背景色
					int nPerWidth = int(gfPlateHeight/(fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * (RctWave.right - RctWave.left)+0.5);

					for(int i=0; i<10; i++)
					{
						float fValue = (i+1)*gfPlateHeight;

						if(fValue < fRangeStart[CurrentChannelNo])
							continue;

						// 计算区域坐标范围，填充背景色
						RECT rtHuiBo;
						rtHuiBo.top = RctWave.top;
						rtHuiBo.bottom = RctWave.bottom;
						rtHuiBo.left = (i*gfPlateHeight - fRangeStart[CurrentChannelNo])/(fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * 
							(RctWave.right - RctWave.left) + RctWave.left;
						rtHuiBo.right = rtHuiBo.left + nPerWidth;

						if(rtHuiBo.left < RctWave.left)
							rtHuiBo.left = RctWave.left;

						if(rtHuiBo.right > RctWave.right)
							rtHuiBo.right = RctWave.right;

						// 将回波位置信息加入数组
						HUIBO_INFO info;
						info.nRightPos = rtHuiBo.right;						

						if(gbHuiBoOpen[i])
						{
							HBRUSH hHuiBoBrush = CreateSolidBrush(clrHuiBoBack[i]);
							FillRect(hMemDC, &rtHuiBo, hHuiBoBrush);
							DeleteObject(hHuiBoBrush);

							info.nColor = clrHuiBoLineBack[i];
						}
						else
						{
//							hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
							hbr = hbrWaveBack;
							FillRect(hMemDC, &rtHuiBo, hbr);

							info.nColor = 0;
						}

						HuiBoInfo.push_back(info);

						if(fValue > fRangeEnd[CurrentChannelNo])
							break;
					}

					// 10段距离小于声程显示的最大值时
					float fMaxValue = 10*gfPlateHeight;
					if(fMaxValue < fRangeEnd[CurrentChannelNo])
					{
						RECT rtHuiBo;
						rtHuiBo.top = RctWave.top;
						rtHuiBo.bottom = RctWave.bottom;
						rtHuiBo.left = (fMaxValue - fRangeStart[CurrentChannelNo])/(fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * (RctWave.right - RctWave.left) + RctWave.left;
						rtHuiBo.right = RctWave.right;

//						hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
						hbr = hbrWaveBack;
						FillRect(hMemDC, &rtHuiBo, hbr);
					}
				}

//				hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
				hbr = hbrScaleBack;
				
				FillRect(hMemDC, &RctYScale, hbr);
				FillRect(hMemDC, &RctRight, hbr);
			}
#if FRAME_COUNT_DISP
			//ZhenShu
			RECT zhenshuRct;
			zhenshuRct.left = SCALE_WIDTH;
			zhenshuRct.top = 48;
			zhenshuRct.right = (int)((nWidth - SCALE_WIDTH) * 2 / 5);
			zhenshuRct.bottom = 100;

			hbr = (HBRUSH)GetStockObject(BLACK_BRUSH);
			FillRect(hMemDC, &zhenshuRct, hbr);
#endif	
			//ZhenShu
			// 根据是否按了声程快捷键，显示不同背景色
			BOOL bHotRange = FALSE;
			InitializeCriticalSection(&csHotRange);
			EnterCriticalSection(&csHotRange);
			bHotRange = gbHotRange;
			LeaveCriticalSection(&csHotRange);
			DeleteCriticalSection(&csHotRange);

			if(!bHotRange)
//				hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
				hbr = hbrScaleBack;
			else
				hbr = (HBRUSH)GetStockObject(LTGRAY_BRUSH);

			FillRect(hMemDC, &RctXScale, hbr);
		
			// 画坐标轴
			DrawReferenceFrame(hMemDC, rect, RctWave, RctXScale, RctYScale,nLoopIndex, hbrScaleBack);
#if FRAME_COUNT_DISP
			//zhenShu
			COLORREF clrOld =  SetTextColor( hMemDC,  RGB(255,255,255));

			DrawText (hMemDC, strZhenShu, -1, &zhenshuRct, 
						  DT_CENTER | DT_TOP | DT_SINGLELINE);	

			SetTextColor( hMemDC,  clrOld);
#endif
			// 获取工作模式
			InitializeCriticalSection(&csSysInfo);
			EnterCriticalSection(&csSysInfo);
			WORKMODE mode = gSysInfo.wWorkMode;
			LeaveCriticalSection(&csSysInfo);
			DeleteCriticalSection(&csSysInfo);

			
			if(nLoopIndex == 0)
				BitBlt(hdc, 0, 48, nWidth, nHeight-48, hMemDC, 0, 48, SRCCOPY);
			else
			{
#if FRAME_COUNT_DISP
				BitBlt(hdc, zhenshuRct.left, zhenshuRct.top, (zhenshuRct.right - zhenshuRct.left), (zhenshuRct.bottom - zhenshuRct.top), 
					hMemDC, zhenshuRct.left, zhenshuRct.top, SRCCOPY);		//zhenShu
#endif
				BitBlt(hdc, RctXScale.left, RctXScale.top, (RctXScale.right - RctXScale.left), 
					(RctXScale.bottom - RctXScale.top), 
					hMemDC, RctXScale.left, RctXScale.top, SRCCOPY);
			}

			// songchenguang add start 2010-07-31
			// 在波形区域右上角显示报警信息
		//	if(GateInfos[0][0].bOpen || GateInfos[0][1].bOpen || GateInfos[1][0].bOpen || GateInfos[1][1].bOpen)
		//	{
				DrawAlarmInfo(hbrWaveBack, csTextInfo, hdc, hMemDC, hFontInfo,RctWave,mode);
		//	}
			// songchenguang add end 2010-07-31
			
			// 画虚线	
			DrawRectDashLine(RctWave, RGBtoRGB565(SysColor[nColorIndex].clrWaveBack));

			// 画波形曲线
			
			// 单通道A
	/*		if(mode == SingleChannelA)
			{
				if(!gPicData[0].bDrawed)
				{					
					DrawWave(hdc, 0, RctWave, nLoopIndex, preData[0], preLeft[0], preRight[0], preTop[0], prebOpen[0], bHuiBo, HuiBoInfo, preZhanKuanFront, preZhanKuanBack, nDisplayCount, SysColor[nColorIndex], hFontInfo);

					gPicData[0].bDrawed = TRUE;
				}
			}
			else if(mode == SingleChannelB ||
				    mode == OneSendOneReceive
					) // 单通道B、一发一收
			{
				if(!gPicData[1].bDrawed)
				{					
					DrawWave(hdc, 1,RctWave, nLoopIndex, preData[1], preLeft[1], preRight[1], preTop[1], prebOpen[1], bHuiBo, HuiBoInfo, preZhanKuanFront, preZhanKuanBack, nDisplayCount, SysColor[nColorIndex], hFontInfo);
					gPicData[1].bDrawed = TRUE;
				}
			}
			else // 双通道, 单晶关联, 双晶关联
			{
				for(int i=0; i<2; i++)
				{
					if(!gPicData[i].bDrawed)
					{					
						DrawWave(hdc, i,RctWave, nLoopIndex, preData[i], preLeft[i], preRight[i], preTop[i], prebOpen[i], bHuiBo, HuiBoInfo, preZhanKuanFront, preZhanKuanBack, nDisplayCount, SysColor[nColorIndex], hFontInfo);

						gPicData[i].bDrawed = TRUE;
					}
				}
			}
*/			
			if(!gPicData[CurrentChannelNo].bDrawed)
			{					
				DrawWave(hdc, 0, RctWave, nLoopIndex, preData[0], preLeft[0], preRight[0], preTop[0], prebOpen[0], bHuiBo, HuiBoInfo, preZhanKuanFront, preZhanKuanBack, nDisplayCount, SysColor[nColorIndex], hFontInfo);

				gPicData[CurrentChannelNo].bDrawed = TRUE;
			}

			if(count == 0)
				begin = GetTickCount();
			count++;			

			nDisplayCount++;

			nDisplayCount = nDisplayCount%2;

			ResetEvent(hDrawEvent); 

			if(gbReplayFlag == OFF)		//wangbingfu add at 2010-11-10
				SetEvent(hWorkEvent);

			InitializeCriticalSection(&csStop);
			EnterCriticalSection(&csStop);
			BOOL bStop = bTerminated;
			LeaveCriticalSection(&csStop);
			DeleteCriticalSection(&csStop);

			if (bStop) 
			{
				break;
			}

//			Sleep(10);	
		}
		#endif
		else
		{
			Sleep(1);
			continue;
		}
		
		if(nLoopIndex > 0xff)
			nLoopIndex = 1;
		nLoopIndex++;

	}

	DeleteCriticalSection(&csSysColor);
	DeleteCriticalSection(&csTextInfo);
	SelectObject(hMemDC,hOldBitmap);

	DeleteObject(hbrWaveBack);
	DeleteObject(hbrScaleBack);

	DeleteObject (SelectObject (hMemDC, hFont)) ;
	DeleteObject (hFontInfo);
	DeleteObject(hLine1Pen);
	DeleteObject(hLine2Pen);
	DeleteObject (hBitmap);
	DeleteDC (hMemDC);
	return 0;
}

int AECThreadIdle1(AECTHREAD_PARAM * pThreadParam, int& nCount, int& nFrames)
{
//	int nCount1 = GetTickCount();
//	int nFrames = 0;

    //MessageBox(NULL,_T( "AECThreadIdle"),NULL,0); 
//	DWORD dwWaitObjectIndex = WaitForSingleObject(hWorkEvent, 0);
	
//	static int nPeakCount = 0;	// 循环计数
//	static PEAK_INFO PeakInfoSum[2][2];
#if 0//_NDT_TEST
	static PEAK_INFO PeakInfoTest[100];		// 测试值,通道1，闸门1
	static PEAK_INFO PeakInfoTest2[100];		// 测试值,通道1，闸门2

	if(nPeakCount == 0)
	{
		memset(PeakInfoTest, 0, sizeof(PeakInfoTest));
		memset(PeakInfoTest2, 0, sizeof(PeakInfoTest2));
	}
#endif
/*	InitializeCriticalSection(&csAdjust);
	EnterCriticalSection(&csAdjust);
	
	BOOL bAdjustStart = gbAdjustStart;
	BOOL bAngleAdjustStart = gbAngleAdjustStart;
	BOOL bChZeroSingleStart = gbChZeroSingleStart;
	BOOL bChZeroDoubleStart = gbChZeroDoubleStart;
	BOOL bChZeroDoubleMinStart = gbChZeroDoubleMinStart;
	BOOL bChWaveSpeedAdjustStart = gbChWaveSpeedAdjustStart;
	LeaveCriticalSection(&csAdjust);
	DeleteCriticalSection(&csAdjust);
*/
/*	memset(gstMonitorData, 0, sizeof(gstMonitorData));
	CH_FEATURE_BUFFER_DATA_t  stFeatureData[CHANNEL_NUM][128];
	memset(stFeatureData, 0, sizeof(stFeatureData));
	int nPerFeatureSize = sizeof(CH_FEATURE_BUFFER_DATA_t);*/ 
	WORD *ch_feature_buffer_flag[2] = {cscan_ch1_feature_buffer_flag, cscan_ch2_feature_buffer_flag}; 
	WORD *ch_feature_buffer[2][2] = {{cscan_ch1_feature_buffer1, cscan_ch1_feature_buffer2},{cscan_ch2_feature_buffer1, cscan_ch2_feature_buffer2}};
	CSCAN_ARS200FPGA_CH_DATA_REG *ch_data_regs[2] = {cscan_ch1_data_reg, cscan_ch2_data_reg};
	WORD *ch_datas[2] = {cscan_ch1_data, cscan_ch2_data};
   
	
	if (1)	// 确认画图线程已经结束
	{
        if (gbAscanDataStoreSend)
        {
		//	int nCount1 = GetTickCount();
		/*	Bytef szSendData1[NETHEAD_SIZE + ASCAN_DATA_STORE_SIZE * 2 ];
			wchar_t strText[30];
			wsprintf(strText, _T("compress:%d\n"), 1);
			OutputDebugString(strText);*/
			char szSendData1[NETHEAD_SIZE + ASCAN_DATA_STORE_SIZE * 2 ];
			if (*cscan_ch1_ascan_data_store_monitor_flag == 1)
			{
				PACKET_HEAD_t stPacketHead;
				memset(&stPacketHead, 0, sizeof(stPacketHead));
				stPacketHead.dwCommand = COMMAND_ASCAN_DATA_STROE;
				stPacketHead.wChannelIndex = 0;
				stPacketHead.wGateIndex = 0;
				stPacketHead.dwSize = ASCAN_DATA_STORE_SIZE * 2;

				memcpy(szSendData1, &stPacketHead, sizeof(stPacketHead));
                
				if (*cscan_ch1_ascan_data_store_buffer_flag == 1)
				{
					memcpy(&szSendData1[NETHEAD_SIZE], cscan_ch1_ascan_data_store_buffer1, stPacketHead.dwSize);
				}
				else if (*cscan_ch1_ascan_data_store_buffer_flag == 2)
				{
					memcpy(&szSendData1[NETHEAD_SIZE], cscan_ch1_ascan_data_store_buffer2, stPacketHead.dwSize);
				}
			//	int count = GetTickCount();

			/*	Bytef SendData[NETHEAD_SIZE + ASCAN_DATA_STORE_SIZE * 2 + 200];
				//FAR unsigned char* szSendData  = reinterpret_cast<FAR unsigned char*>(SendData[0]);
				//FAR unsigned char* source = reinterp	ret_cast<FAR unsigned char*>(szSendData1[0]);
				//char* szSendData;
				uLongf destLen = NETHEAD_SIZE + ASCAN_DATA_STORE_SIZE * 2 + 200;
				uLongf sourceLen = NETHEAD_SIZE + ASCAN_DATA_STORE_SIZE * 2;
				compress(SendData, &destLen, szSendData1, sourceLen);
				wchar_t strText[30];
				wsprintf(strText, _T("compress:%d\n"), destLen);
				OutputDebugString(strText);*/
				//FAR unsigned char* dest;
// 				uLongf len = 0;
				/*uncompress(dest, &len, SendData, destLen);
				wsprintf(strText, _T("un compress:%d\n"), len);
				OutputDebugString(strText);*/
				if(send( gSockClient3, szSendData1, NETHEAD_SIZE + stPacketHead.dwSize, 0 )== SOCKET_ERROR)
				{
					
				}
				//flushall();
			/*	wchar_t strText[30];
				wsprintf(strText, _T("ascan send time:%d\n"), GetTickCount() - count);
				OutputDebugString(strText);
				/*for(int i = 0; i < gVecSockACScan.size(); i++)
				{
					send( gVecSockACScan[i], szSendData1, NETHEAD_SIZE + stPacketHead.dwSize, 0 );
				}*/

				*cscan_ch1_ascan_data_store_reset = 1;
				Sleep(1);
				/*for(int i = 0; i < 10; i++)
				{
					i++;
				}*/
				*cscan_ch1_ascan_data_store_reset = 0;

			/*	nFrames = nFrames + 30;
				int nCurCount = GetTickCount();
				if(nCurCount - nCount >= 1000)
				{
					wchar_t strText[30];
					wsprintf(strText, _T("ASCAN frames:%d\n"), nFrames);
					OutputDebugString(strText);

					nFrames = 0;
					nCount = nCurCount;
				}*/
			}
#if 1
			else if (*cscan_ch2_ascan_data_store_monitor_flag == 1)
			{
				PACKET_HEAD_t stPacketHead;
				memset(&stPacketHead, 0, sizeof(stPacketHead));
				stPacketHead.dwCommand = COMMAND_ASCAN_DATA_STROE;
				stPacketHead.wChannelIndex = 1;
				stPacketHead.wGateIndex = 0;
				stPacketHead.dwSize = ASCAN_DATA_STORE_SIZE * 2;

				memcpy(szSendData1, &stPacketHead, sizeof(stPacketHead));

				if (*cscan_ch2_ascan_data_store_buffer_flag == 1)
				{
					memcpy(&szSendData1[NETHEAD_SIZE], cscan_ch2_ascan_data_store_buffer1, stPacketHead.dwSize);
				}
				else if (*cscan_ch2_ascan_data_store_buffer_flag == 2)
				{
					memcpy(&szSendData1[NETHEAD_SIZE], cscan_ch2_ascan_data_store_buffer2, stPacketHead.dwSize);
				}

				if(send( gSockClient3, szSendData1, NETHEAD_SIZE + stPacketHead.dwSize, 0 )== SOCKET_ERROR)
				{

				}
			/*	for(int i = 0; i < gVecSockACScan.size(); i++)
				{
					send( gVecSockACScan[i], szSendData1, NETHEAD_SIZE + stPacketHead.dwSize, 0 );
				}*/
				*cscan_ch2_ascan_data_store_reset = 1;
				Sleep(1);
				*cscan_ch2_ascan_data_store_reset = 0;

			}
#endif
			else
			{

			}
        }
	}

	return 0;
}

int AECThreadIdle(AECTHREAD_PARAM * pThreadParam, int& nCount, int& nFrames)
{
//	int nCount1 = GetTickCount();
//	int nFrames = 0;

    //MessageBox(NULL,_T( "AECThreadIdle"),NULL,0); 
//	DWORD dwWaitObjectIndex = WaitForSingleObject(hWorkEvent, 0);
	
//	static int nPeakCount = 0;	// 循环计数
//	static PEAK_INFO PeakInfoSum[2][2];
#if 0//_NDT_TEST
	static PEAK_INFO PeakInfoTest[100];		// 测试值,通道1，闸门1
	static PEAK_INFO PeakInfoTest2[100];		// 测试值,通道1，闸门2

	if(nPeakCount == 0)
	{
		memset(PeakInfoTest, 0, sizeof(PeakInfoTest));
		memset(PeakInfoTest2, 0, sizeof(PeakInfoTest2));
	}
#endif
/*	InitializeCriticalSection(&csAdjust);
	EnterCriticalSection(&csAdjust);
	
	BOOL bAdjustStart = gbAdjustStart;
	BOOL bAngleAdjustStart = gbAngleAdjustStart;
	BOOL bChZeroSingleStart = gbChZeroSingleStart;
	BOOL bChZeroDoubleStart = gbChZeroDoubleStart;
	BOOL bChZeroDoubleMinStart = gbChZeroDoubleMinStart;
	BOOL bChWaveSpeedAdjustStart = gbChWaveSpeedAdjustStart;
	LeaveCriticalSection(&csAdjust);
	DeleteCriticalSection(&csAdjust);
*/
/*	memset(gstMonitorData, 0, sizeof(gstMonitorData));
	CH_FEATURE_BUFFER_DATA_t  stFeatureData[CHANNEL_NUM][128];
	memset(stFeatureData, 0, sizeof(stFeatureData));
	int nPerFeatureSize = sizeof(CH_FEATURE_BUFFER_DATA_t);*/ 
	WORD *ch_feature_buffer_flag[2] = {cscan_ch1_feature_buffer_flag, cscan_ch2_feature_buffer_flag}; 
	WORD *ch_feature_buffer[2][2] = {{cscan_ch1_feature_buffer1, cscan_ch1_feature_buffer2},{cscan_ch2_feature_buffer1, cscan_ch2_feature_buffer2}};
	CSCAN_ARS200FPGA_CH_DATA_REG *ch_data_regs[2] = {cscan_ch1_data_reg, cscan_ch2_data_reg};
	WORD *ch_datas[2] = {cscan_ch1_data, cscan_ch2_data};
   

//	double dX = 0;
//	double dY = 0;
//	double dZ = 0;

	char szSendData[MAX_BUFFER_SIZE];
	memset( szSendData, 0, sizeof(szSendData) );
	//if (dwWaitObjectIndex == WAIT_OBJECT_0)	// 确认画图线程已经结束
	
	if (1)	// 确认画图线程已经结束
	{
		char cDataFlag = 0;		//BIT0=1:通道1有特征数据;BIT1=1:通道1有监视数据;BIT2=1:通道2有特征数据;BIT3=1:通道2有监视数据
		for(int i=0; i<2; i++)
		{	
			if(ch_data_regs[i]->wMonitorFlag == 1)
			{	
				gPicData[i].bDrawed = FALSE;
				gnSendCount++;
				gnSendCount1++;
				// Song rem
				PutsReadDrawData(gPicData[i], ch_data_regs[i], ch_datas[i], i, NULL);

				memcpy(&gstMonitorData[i].wPicData, &gPicData[i].data, sizeof(WORD)*DATA_SIZE);

				ReadCodeAndFeature(&gstMonitorData[i], i);

				if ( gbChannelSend[0] )
				{
					//改成两通道都发送
					//if(i == CurrentChannelNo)
					{
						int nSendBytes = 0;
						PACKET_HEAD_t stPacketHead;
						memset(&stPacketHead, 0, sizeof(stPacketHead));
						stPacketHead.dwCommand = COMMAND_WAVE_DATA;
						stPacketHead.wChannelIndex = i;//CurrentChannelNo;
						stPacketHead.wGateIndex = 0;
						stPacketHead.dwSize = sizeof(CSCAN_CH_SEND_MONITOR_DATA_t);

						memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));

						memcpy(&szSendData[NETHEAD_SIZE], &gstMonitorData[i], stPacketHead.dwSize);
						
						for(int j = 0; j < gVecSockMonitor.size(); j++)
						{
							if(send( gVecSockMonitor[j], szSendData, NETHEAD_SIZE + stPacketHead.dwSize, 0 )== SOCKET_ERROR)
							{
								;
							}
						}
					/*	int counts = GetTickCount();
						int total = 0;
						for(int i = 0; i < 1000; i++)
						{
							for(int j = 0; j < gVecSockMonitor.size(); j++)
							{
								if(send( gVecSockMonitor[j], szSendData, NETHEAD_SIZE + stPacketHead.dwSize, 0 )== SOCKET_ERROR)
								{
									total++;
								}
								Sleep(10);
							}
						}
						wchar_t strText[30];
						wsprintf(strText, _T("mointor send time:%d\n"), GetTickCount() - counts);
						OutputDebugString(strText);
						wsprintf(strText, _T("mointor error:%d\n"), total);
						OutputDebugString(strText);*/
					}
				}
			}
		}
#if 0
        if (gbAscanDataStoreSend)
        {
		//	int nCount1 = GetTickCount();
		//	Bytef szSendData1[NETHEAD_SIZE + ASCAN_DATA_STORE_SIZE * 2 ];
			char szSendData1[NETHEAD_SIZE + ASCAN_DATA_STORE_SIZE * 2 ];
			PACKET_HEAD_t stPacketHead;
			memset(&stPacketHead, 0, sizeof(stPacketHead));
			stPacketHead.dwCommand = COMMAND_ASCAN_DATA_STROE;
			stPacketHead.wChannelIndex = 0;
			stPacketHead.wGateIndex = 0;
			stPacketHead.dwSize = ASCAN_DATA_STORE_SIZE * 2;
		/*	int counts = GetTickCount();
			int total = 0;
			for(int i = 0; i < 1000; i++)
			{
				if(send( gSockClient3, szSendData1, NETHEAD_SIZE + stPacketHead.dwSize, 0 )== SOCKET_ERROR)
				{
				//	OutputDebugString(_T("socket error\n"));
					total++;
				}
				Sleep(10);
				//flushall();
			}
			wchar_t strText[30];
			wsprintf(strText, _T("send time:%d\n"), GetTickCount() - counts);
			OutputDebugString(strText);
			wsprintf(strText, _T("error:%d\n"), total);
			OutputDebugString(strText);*/
			if (*cscan_ch1_ascan_data_store_monitor_flag == 1)
			{
				PACKET_HEAD_t stPacketHead;
				memset(&stPacketHead, 0, sizeof(stPacketHead));
				stPacketHead.dwCommand = COMMAND_ASCAN_DATA_STROE;
				stPacketHead.wChannelIndex = 0;
				stPacketHead.wGateIndex = 0;
				stPacketHead.dwSize = ASCAN_DATA_STORE_SIZE * 2;

				memcpy(szSendData1, &stPacketHead, sizeof(stPacketHead));
                
				if (*cscan_ch1_ascan_data_store_buffer_flag == 1)
				{
					memcpy(&szSendData1[NETHEAD_SIZE], cscan_ch1_ascan_data_store_buffer1, stPacketHead.dwSize);
				}
				else if (*cscan_ch1_ascan_data_store_buffer_flag == 2)
				{
					memcpy(&szSendData1[NETHEAD_SIZE], cscan_ch1_ascan_data_store_buffer2, stPacketHead.dwSize);
				}
			//	int count = GetTickCount();

			/*	Bytef SendData[NETHEAD_SIZE + ASCAN_DATA_STORE_SIZE * 2 + 200];
				//FAR unsigned char* szSendData  = reinterpret_cast<FAR unsigned char*>(SendData[0]);
				//FAR unsigned char* source = reinterp	ret_cast<FAR unsigned char*>(szSendData1[0]);
				//char* szSendData;
				uLongf destLen = NETHEAD_SIZE + ASCAN_DATA_STORE_SIZE * 2 + 200;
				uLongf sourceLen = NETHEAD_SIZE + ASCAN_DATA_STORE_SIZE * 2;
				compress(SendData, &destLen, szSendData1, sourceLen);
				wchar_t strText[30];
				wsprintf(strText, _T("compress:%d\n"), destLen);
				OutputDebugString(strText);
				//FAR unsigned char* dest;
				uLongf len = 0;
				/*uncompress(dest, &len, SendData, destLen);
				wsprintf(strText, _T("un compress:%d\n"), len);
				OutputDebugString(strText);*/
				/*int counts = GetTickCount();
				for(int i = 0; i < 1000; i++)
				{
					if(send( gSockClient3, szSendData1, NETHEAD_SIZE + stPacketHead.dwSize, 0 )== SOCKET_ERROR)
					{
						OutputDebugString(_T("socket error"));
					}
				}
				wchar_t strText[30];
				wsprintf(strText, _T("send time:%d\n"), GetTickCount() - counts);
				OutputDebugString(strText);*/
				if(send( gSockClient3, szSendData1, NETHEAD_SIZE + stPacketHead.dwSize, 0 )== SOCKET_ERROR)
				{
					OutputDebugString(_T("socket error"));
				}

				
				//flushall();
			/*	wchar_t strText[30];
				wsprintf(strText, _T("ascan send time:%d\n"), GetTickCount() - count);
				OutputDebugString(strText);
				/*for(int i = 0; i < gVecSockACScan.size(); i++)
				{
					send( gVecSockACScan[i], szSendData1, NETHEAD_SIZE + stPacketHead.dwSize, 0 );
				}*/

				*cscan_ch1_ascan_data_store_reset = 1;
				Sleep(1);
			/*	for(int i = 0; i < 10; i++)
				{
					i++;
				}*/
				*cscan_ch1_ascan_data_store_reset = 0;

		/*		nFrames = nFrames + 60;       
				int nCurCount = GetTickCount();
				if(nCurCount - nCount >= 1000)
				{
					wchar_t strText[30];
					wsprintf(strText, _T("ASCAN1 frames:%d\n"), nFrames);
					OutputDebugString(strText);

					nFrames = 0;
					nCount = nCurCount;
				}*/
			}
//#if 1
			else if (*cscan_ch2_ascan_data_store_monitor_flag == 1)
			{
				PACKET_HEAD_t stPacketHead;
				memset(&stPacketHead, 0, sizeof(stPacketHead));
				stPacketHead.dwCommand = COMMAND_ASCAN_DATA_STROE;
				stPacketHead.wChannelIndex = 1;
				stPacketHead.wGateIndex = 0;
				stPacketHead.dwSize = ASCAN_DATA_STORE_SIZE * 2;

				memcpy(szSendData1, &stPacketHead, sizeof(stPacketHead));

				if (*cscan_ch2_ascan_data_store_buffer_flag == 1)
				{
					memcpy(&szSendData1[NETHEAD_SIZE], cscan_ch2_ascan_data_store_buffer1, stPacketHead.dwSize);
				}
				else if (*cscan_ch2_ascan_data_store_buffer_flag == 2)
				{
					memcpy(&szSendData1[NETHEAD_SIZE], cscan_ch2_ascan_data_store_buffer2, stPacketHead.dwSize);
				}

				if(send( gSockClient3, szSendData1, NETHEAD_SIZE + stPacketHead.dwSize, 0 )== SOCKET_ERROR)
				{

				}
			/*	for(int i = 0; i < gVecSockACScan.size(); i++)
				{
					send( gVecSockACScan[i], szSendData1, NETHEAD_SIZE + stPacketHead.dwSize, 0 );
				}*/
				*cscan_ch2_ascan_data_store_reset = 1;
				Sleep(1);
				/*for(int i = 0; i < 10; i++)
				{
					i++;
				}*/
				*cscan_ch2_ascan_data_store_reset = 0;

			}

			else
			{

			}
//#endif
        }
#endif

		
		// 获取工作模式
		InitializeCriticalSection(&csSysInfo);
		EnterCriticalSection(&csSysInfo);
		WORKMODE mode = gSysInfo.wWorkMode;
		LeaveCriticalSection(&csSysInfo);
		DeleteCriticalSection(&csSysInfo);

		if ( gbChannelSend[1] )
		{
			int nSendBytes = 0;
			PACKET_HEAD_t stPacketHead;
			memset(&stPacketHead, 0, sizeof(stPacketHead));
			stPacketHead.dwCommand = COMMAND_WAVE_DATA;		
			stPacketHead.wGateIndex = 0;
			stPacketHead.dwSize = sizeof(CH_FEATURE_BUFFER_DATA_t)*128;
			BOOL bFeatureFlag[CHANNEL_NUM] = {FALSE, FALSE};	// 两个通中有任一通道有征特缓冲数据
			for(int i = 0; i < 2; i++)
			{	
				if(ch_data_regs[i]->wFeatureReadFlag == 1)
				{	
					if(*ch_feature_buffer_flag[i] == 1)	//缓冲区1可读
					{
						gnSendCount = 1;

						stPacketHead.wChannelIndex = i;
						memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));						
						memcpy(&szSendData[NETHEAD_SIZE], ch_feature_buffer[i][0], stPacketHead.dwSize);

						//send( gSockClient2, szSendData, NETHEAD_SIZE + stPacketHead.dwSize, 0 );
						int count = GetTickCount();
						for(int j = 0; j < gVecSockCScan.size(); j++)
						{
							send( gVecSockCScan[j], szSendData, NETHEAD_SIZE + stPacketHead.dwSize, 0 );
						}
						wchar_t strText[30];
					//	wsprintf(strText, _T("cscan send time:%d\n"), GetTickCount() - count);
					//	OutputDebugString(strText);

						ch_data_regs[i]->wFeatureReadReset = 1;
						Sleep(1);
					/*	for(int i = 0; i < 10000000; i++)
						{
							i++;
						}*/
						ch_data_regs[i]->wFeatureReadReset = 0;
					}
					else if(*ch_feature_buffer_flag[i] == 2)	//缓冲区2可读
					{
						gnSendCount = 2;


						stPacketHead.wChannelIndex = i;
						memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));						
						memcpy(&szSendData[NETHEAD_SIZE], ch_feature_buffer[i][1], stPacketHead.dwSize);

						//send( gSockClient2, szSendData, NETHEAD_SIZE + stPacketHead.dwSize, 0 );
						for(int j = 0; j < gVecSockCScan.size(); j++)
						{
							send( gVecSockCScan[j], szSendData, NETHEAD_SIZE + stPacketHead.dwSize, 0 );
						}

						ch_data_regs[i]->wFeatureReadReset = 1;
						Sleep(1);
						/*for(int i = 0; i < 10000000; i++)
						{
							i++;
						}*/
						ch_data_regs[i]->wFeatureReadReset = 0;
					}
					else
					{
					}
				/*	if(i == 0)
					{
						nFrames = nFrames + 128;
						int nCurCount = GetTickCount();
						if(nCurCount - nCount >= 1000)
						{
							wchar_t strText[30];
							wsprintf(strText, _T("CSCAN1 frames:%d\n"), nFrames);
							OutputDebugString(strText);

							nFrames = 0;
							nCount = nCurCount;
						}
					}	*/			
				}
			}
		}
		
		
	/*	gnDrawCount++;

		if(gnDrawCount > 0xFFFF)
			gnDrawCount = 1;

		int nTemp = gnDrawCount % 3;
		if(nTemp == 0)
		{
			ResetEvent(hWorkEvent);
			SetEvent(hDrawEvent);
		}*/
	/*	wchar_t strText[30];
		wsprintf(strText, _T("time:%d\n"), GetTickCount() - nCount1);
		OutputDebugString(strText);	*/
	}

	// 读取伤数据
//	BOOL bRet = ReadFlawResult(pThreadParam);

	return 0;
}

DWORD WINAPI AECThreadProc1(LPVOID lpParameter)
{
/*	int nCount = GetTickCount();
	int frame = 0;
	int nCount1 = GetTickCount();
	while(1)
	{
		frame++;
		int count = GetTickCount();
		wchar_t strText[30];
		wsprintf(strText, _T("thread time:%d\n"), count - nCount);
		nCount = count;
	//	OutputDebugString(strText);
		if(count - nCount1 >= 1000)
		{
			wchar_t strText[30];
			wsprintf(strText, _T("thread frames:%d\n"), frame);
			OutputDebugString(strText);
			frame = 0;
			nCount1 = count;
		}
		Sleep(1);
	}*/
	AECTHREAD_PARAM * pThreadParam = (AECTHREAD_PARAM *)lpParameter;
	BOOL bThreadExitFlag = FALSE;

//	hWorkMutex = CreateMutex(NULL, TRUE, TEXT("AEC_MUTEX"));

	int nCount = GetTickCount();
	int nFrames = 0;
	int frame = 0;
	int nCount1 = GetTickCount();

	while(1)
	{
		frame++;
	/*	int count1 = GetTickCount();
		wchar_t strText[30];
		wsprintf(strText, _T("thread time:%d\n"), count1 - nCount);
		nCount = count1;
		OutputDebugString(strText);*/

//		DWORD dwRet = ::WaitForSingleObject(hWorkMutex, 1);
//		if(dwRet == WAIT_OBJECT_0)
		{			
			WORD wCommand = pThreadParam->nMain2AECThreadCommand;

			BOOL bRet = FALSE;

			switch (wCommand)
			{
			case MAIN2AECTHREADCOMMAND_INIT:
				//MessageBox(NULL, _T("before AECThreadInit"), NULL, MB_OK);
				bRet = AECThreadInit(pThreadParam);
				if(bRet)
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
				break;
			case MAIN2AECTHREADCOMMAND_START:
				//MessageBox(NULL, _T("before AECThreadStart"), NULL, MB_OK);
				bRet = AECThreadStart(pThreadParam);
				if(bRet)
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
				break;
			case MAIN2AECTHREADCOMMAND_STOP:	// 停止读数据
				AECThreadStop(pThreadParam);
				thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
				break;
			case MAIN2AECTHREADCOMMAND_IDLE:
				//MessageBox(NULL, _T("before AECThreadIdle"), NULL, MB_OK);
				AECThreadIdle1(pThreadParam, nCount, nFrames);
				break;
			case MAIN2AECTHREADCOMMAND_THREAD_STOP:
				bThreadExitFlag = TRUE;
				thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
				MessageBox(NULL, _T("MAIN2AECTHREADCOMMAND_THREAD_STOP"), NULL, MB_OK);
				break;
			case MAIN2AECTHREADCOMMAND_NONE:
				//MessageBox(NULL, _T("MAIN2AECTHREADCOMMAND_NONE"), NULL, MB_OK);
				break;
			}

//			ReleaseMutex(hWorkMutex);
		}		

		InitializeCriticalSection(&csStop);
		EnterCriticalSection(&csStop);
		BOOL bStop = bTerminated;
		LeaveCriticalSection(&csStop);
		DeleteCriticalSection(&csStop);

		if (bStop) 
			break;

		Sleep(1);
		
		int count = GetTickCount();
	/*	wchar_t strText[30];
		wsprintf(strText, _T("thread time:%d\n"), count - count1);
		OutputDebugString(strText);*/
		if(count - nCount1 >= 1000)
		{
			wchar_t strText[30];
			wsprintf(strText, _T("thread1 frames:%d\n"), frame);
			OutputDebugString(strText);
			frame = 0;
			nCount1 = count;
		}
		
	}

//	CloseHandle(hWorkMutex);
	return 0;
}
DWORD WINAPI AECThreadProc(LPVOID lpParameter)
{
	AECTHREAD_PARAM * pThreadParam = (AECTHREAD_PARAM *)lpParameter;
	BOOL bThreadExitFlag = FALSE;

	hWorkMutex = CreateMutex(NULL, TRUE, TEXT("AEC_MUTEX"));

	int nCount = GetTickCount();
	int nFrames = 0;
	int frame = 0;
	int nCount1 = GetTickCount();

	while(1)
	{
		frame++;
	/*	int count1 = GetTickCount();
		wchar_t strText[30];
		wsprintf(strText, _T("thread time:%d\n"), count1 - nCount);
		nCount = count1;
		OutputDebugString(strText);*/

		DWORD dwRet = ::WaitForSingleObject(hWorkMutex, 1);
		if(dwRet == WAIT_OBJECT_0)
		{			
			WORD wCommand = pThreadParam->nMain2AECThreadCommand;

			BOOL bRet = FALSE;

			switch (wCommand)
			{
			case MAIN2AECTHREADCOMMAND_INIT:
				//MessageBox(NULL, _T("before AECThreadInit"), NULL, MB_OK);
				bRet = AECThreadInit(pThreadParam);
				if(bRet)
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
				break;
			case MAIN2AECTHREADCOMMAND_START:
				//MessageBox(NULL, _T("before AECThreadStart"), NULL, MB_OK);
				bRet = AECThreadStart(pThreadParam);
				if(bRet)
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
				break;
			case MAIN2AECTHREADCOMMAND_STOP:	// 停止读数据
				AECThreadStop(pThreadParam);
				thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
				break;
			case MAIN2AECTHREADCOMMAND_IDLE:
				//MessageBox(NULL, _T("before AECThreadIdle"), NULL, MB_OK);
				AECThreadIdle(pThreadParam, nCount, nFrames);
				break;
			case MAIN2AECTHREADCOMMAND_THREAD_STOP:
				bThreadExitFlag = TRUE;
				thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
				MessageBox(NULL, _T("MAIN2AECTHREADCOMMAND_THREAD_STOP"), NULL, MB_OK);
				break;
			case MAIN2AECTHREADCOMMAND_NONE:
				//MessageBox(NULL, _T("MAIN2AECTHREADCOMMAND_NONE"), NULL, MB_OK);
				break;
			}

			ReleaseMutex(hWorkMutex);
		}		

		InitializeCriticalSection(&csStop);
		EnterCriticalSection(&csStop);
		BOOL bStop = bTerminated;
		LeaveCriticalSection(&csStop);
		DeleteCriticalSection(&csStop);

		if (bStop) 
			break;

		Sleep(1);
		
		int count = GetTickCount();
	/*	wchar_t strText[30];
		wsprintf(strText, _T("thread time:%d\n"), count - count1);
		OutputDebugString(strText);*/
		if(count - nCount1 >= 1000)
		{
			wchar_t strText[30];
			wsprintf(strText, _T("thread frames:%d\n"), frame);
			OutputDebugString(strText);
			frame = 0;
			nCount1 = count;
		}
		
	}

	CloseHandle(hWorkMutex);
	return 0;
}


BOOL AECThreadInit(AECTHREAD_PARAM * pThreadParam)
{
	return TRUE;
}

BOOL AECThreadStart(AECTHREAD_PARAM * pThreadParam)
{
	return TRUE;
}

BOOL AECThreadStop(AECTHREAD_PARAM * pThreadParam)
{
/*
	BOOL bRet = ars200fpga_unalloc_io();

	return bRet;
*/
	return TRUE;
}

BOOL ReadFlawResult(AECTHREAD_PARAM * pThreadParam)
{
	return TRUE;
}

/** 
 * 查询报警标志，若有，显示第一个报警在状态栏	
 * @return    void:
 * @since     1.00
 */
void CheckAlarm(const WORD& chNo, PEAK_INFO PeakSum[2])
{
	InitializeCriticalSection(&csTextInfo);

	InitializeCriticalSection(&csPeakRemember);
	EnterCriticalSection(&csPeakRemember);

	BOOL bPeakRemeber = gbPeakRememberFlag;

	LeaveCriticalSection(&csPeakRemember);
	DeleteCriticalSection(&csPeakRemember);

	ARS200FPGA_CH_GATE_REG *ch_gate_reg = NULL;
	ARS200FPGA_CH_CON_REG *ch_con_reg = NULL;

	if(chNo==0)
	{
		ch_gate_reg = ch1_gate_reg;
		ch_con_reg = ch1_con_reg;
	}
	else if(chNo==1)
	{
		ch_gate_reg = ch2_gate_reg;
		ch_con_reg = ch2_con_reg;
	}

	for(int j=0; j<2; j++)
	{
		// 获取标志值
		InitializeCriticalSection(&csGate);
		EnterCriticalSection(&csGate);	

		BOOL bOpen = GateInfos[chNo][j].bOpen;
		alarm_modes mode = GateInfos[chNo][j].alarmMode;
		LeaveCriticalSection(&csGate);
		DeleteCriticalSection(&csGate);
	 //wangbingfu delete at 2010-09-07
/*		BOOL bOpen = FALSE;
		if(j == 0)
		{
			bOpen = ch_gate_reg->wGate1Enable;
		}
		else if(j == 1)
		{
			bOpen = ch_gate_reg->wGate2Enable;
		}
*/		// songchenguang add start 2010-08-11
		// 保存通道闸门波峰信息

		if(PeakSum != NULL)
		{
			if(j==0)
			{
				PeakSum[j].wDefectTime += ch_gate_reg->wGate1DefectTime;
				PeakSum[j].wDefectAmp += ch_gate_reg->wGate1DefectAmp;
			}
			else if(j == 1)
			{
				PeakSum[j].wDefectTime += ch_gate_reg->wGate2DefectTime;
				PeakSum[j].wDefectAmp += ch_gate_reg->wGate2DefectAmp;
			}
		}

		// songchenguang add end 2010-08-11
		WORD bitTest = 1<<j;
		InitializeCriticalSection(&csAlarmFlag);
		EnterCriticalSection(&csAlarmFlag);
		gbAlarmRegFlag[chNo][j] = ch_gate_reg->wAlarmReg & bitTest;
		gbChannelAlarmFlag[chNo] = ch_gate_reg->wAlarmFlag & 0x0001;
		LeaveCriticalSection(&csAlarmFlag);
		DeleteCriticalSection(&csAlarmFlag);

		float fAngle = DisplaySet_ad[chNo].fZheSheJiao/180.0f * PI;
		//wangbingfu add start at 2010-8-26
		if(j == 0)
		{
			InitializeCriticalSection(&csPeakDefectInfos);
			EnterCriticalSection(&csPeakDefectInfos);
			/* rem by songchenguang 2010-12-02
			if(DisplaySet_ad[chNo].bSFlag)
			{
				gPeakDefectInfos[chNo][j].wDefectTime =  ch_gate_reg->wGate1DefectTime;
			}
			else
			{
				gPeakDefectInfos[chNo][j].wDefectTime =  (WORD)(ch_gate_reg->wGate1DefectTime * sin(fAngle));
			}
			*/			

			// songchenguang add start 2010-12-02
			gPeakDefectInfos[chNo][j].wDefectTime =  ch_gate_reg->wGate1DefectTime;
			// songchenguang add end 2010-12-02
			gPeakDefectInfos[chNo][j].wDefectAmp = ch_gate_reg->wGate1DefectAmp;
			LeaveCriticalSection(&csPeakDefectInfos);
			DeleteCriticalSection(&csPeakDefectInfos);
		}
		else if(j == 1)
		{
			InitializeCriticalSection(&csPeakDefectInfos);
			EnterCriticalSection(&csPeakDefectInfos);
			/* rem by songchenguang 2010-12-02
			if(DisplaySet_ad[chNo].bSFlag)
			{
				gPeakDefectInfos[chNo][j].wDefectTime =  ch_gate_reg->wGate2DefectTime;
			}
			else
			{
				gPeakDefectInfos[chNo][j].wDefectTime =  (WORD)(ch_gate_reg->wGate2DefectTime * sin(fAngle));
			}
			*/

			// songchenguang add start 2010-12-02
			gPeakDefectInfos[chNo][j].wDefectTime =  ch_gate_reg->wGate2DefectTime;
			// songchenguang add end 2010-12-02
			gPeakDefectInfos[chNo][j].wDefectAmp = ch_gate_reg->wGate2DefectAmp;
			LeaveCriticalSection(&csPeakDefectInfos);
			DeleteCriticalSection(&csPeakDefectInfos);
		}
		//wangbingfu add end at 2010-8-26
		if(bOpen)
		{
			// 读取峰值信息			

			// 读取对应闸门的伤幅、伤时
			if(j==0)
			{
				if(bPeakRemeber)
				{
					if(ch_gate_reg->wGate1DefectAmp > gPeakInfosRemeber[chNo][j].wDefectAmp)
					{
						gPeakInfosRemeber[chNo][j].wDefectAmp = gPeakDefectInfos[chNo][j].wDefectAmp;
						gPeakInfosRemeber[chNo][j].wDefectTime = gPeakDefectInfos[chNo][j].wDefectTime;					
					}
				}
				else
				{
					gPeakInfosRemeber[chNo][j].wDefectTime =  gPeakDefectInfos[chNo][j].wDefectTime;
					gPeakInfosRemeber[chNo][j].wDefectAmp = gPeakDefectInfos[chNo][j].wDefectAmp;
				}			
			}
			else if(j==1)
			{
				if(bPeakRemeber)
				{
					if(ch_gate_reg->wGate2DefectAmp > gPeakInfosRemeber[chNo][j].wDefectAmp)
					{
						gPeakInfosRemeber[chNo][j].wDefectAmp = gPeakDefectInfos[chNo][j].wDefectAmp;
						gPeakInfosRemeber[chNo][j].wDefectTime =gPeakDefectInfos[chNo][j].wDefectTime;			
					}
				}
				else
				{
					gPeakInfosRemeber[chNo][j].wDefectTime =  gPeakDefectInfos[chNo][j].wDefectTime;
					gPeakInfosRemeber[chNo][j].wDefectAmp = gPeakDefectInfos[chNo][j].wDefectAmp;
				}
			}

			InitializeCriticalSection(&cs110Display);
			EnterCriticalSection(&cs110Display);
			
			BOOL b110Display = gb110Display;
			LeaveCriticalSection(&cs110Display);
			DeleteCriticalSection(&cs110Display);

			EnterCriticalSection(&csTextInfo);

			memset(gszInfo[chNo][j], 0, sizeof(wchar_t)*30);

		//	memset(&gstConvertPeakInfo, 0, sizeof(gstConvertPeakInfo) );
			gstConvertPeakInfo[chNo][j].wDefectTime = Dot2Range(DisplaySet_ad[chNo].nWaveSpeed, gPeakInfosRemeber[chNo][j].wDefectTime);
			if(gbAvgCurveFlag[chNo])
			{				
				gstConvertPeakInfo[chNo][j].wDefectAmp = calculateFaiZhi(gPeakInfosRemeber[chNo][j].wDefectAmp, gPeakInfosRemeber[chNo][j].wDefectTime,
											gSelectedAvgPointData[CurrentChannelNo][0], gSelectedAvgPointDataNum[CurrentChannelNo]);
				swprintf(gszInfo[chNo][j],_T("CH%dG%d: %.1f, Φ%.1f"),  chNo+1, j+1, gstConvertPeakInfo[chNo][j].wDefectTime, gstConvertPeakInfo[chNo][j].wDefectAmp);
			}
			else if(gDgsSwitch[chNo][0])
			{
				gstConvertPeakInfo[chNo][j].wDefectAmp = dacDeltaDB(gPeakInfosRemeber[chNo][j].wDefectAmp, gPeakInfosRemeber[chNo][j].wDefectTime, 
											gSelectedDgsPointData[CurrentChannelNo][1], gSelectedDgsPointDataNum[CurrentChannelNo][1]);
				swprintf(gszInfo[chNo][j],_T("CH%dG%d: %.1f, △dB%.1f"),  chNo+1, j+1,gstConvertPeakInfo[chNo][j].wDefectTime, gstConvertPeakInfo[chNo][j].wDefectAmp);
			}
			else
			{
				if(!b110Display)
				{
					gstConvertPeakInfo[chNo][j].wDefectAmp = gPeakInfosRemeber[chNo][j].wDefectAmp/MAX_HEIGHT*100.0f;
					swprintf(gszInfo[chNo][j],_T("CH%dG%d: %.1f, %.1f%%"),  chNo+1, j+1, gstConvertPeakInfo[chNo][j].wDefectTime, gstConvertPeakInfo[chNo][j].wDefectAmp);
				}
				else
				{
					gstConvertPeakInfo[chNo][j].wDefectAmp = gPeakInfosRemeber[chNo][j].wDefectAmp/MAX_HEIGHT*110.0f;
					swprintf(gszInfo[chNo][j],_T("CH%dG%d: %.1f, %.1f%%"),  chNo+1, j+1, gstConvertPeakInfo[chNo][j].wDefectTime, gstConvertPeakInfo[chNo][j].wDefectAmp);
				}
			}
			
			LeaveCriticalSection(&csTextInfo);

	/*		char szSendData[NETHEAD_SIZE + DATA_SIZE * 2];
			memset( szSendData, 0, sizeof(szSendData) );
				
			PACKET_HEAD_t stPacketHead;
			memset(&stPacketHead, 0, sizeof(stPacketHead));
			stPacketHead.dwCommand = COMMAND_DEFECT_DATA;
			stPacketHead.wChannelIndex = chNo;
			stPacketHead.wGateIndex = j;
			stPacketHead.dwSize = sizeof(PEAK_INFO);
			
			memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));
			memcpy(&szSendData[NETHEAD_SIZE], &stPeakInfo, stPacketHead.dwSize);
			send( gSockClient, szSendData, sizeof(szSendData), 0 );*/
			#if 0 
			if((ch_gate_reg->wAlarmReg & bitTest) && 
				mode == SuperHigh)	// 进波
			{
				sys_reg->wLedAlarm |= 0x1;

				MessageBeep(0xFFFFFFFF);
				
				ch_con_reg->wReset |= 0x1;
				ch_con_reg->wReset &= 0xFFFE;

			}
			else if((ch_gate_reg->wAlarmReg & bitTest) && 
					mode == LostWave) // 失波
			{

				sys_reg->wLedAlarm |= 0x1;

				MessageBeep(0xFFFFFFFF);
				
				ch_con_reg->wReset |= 0x1;
				ch_con_reg->wReset &= 0xFFFE;

			}
			#endif
		

		}		
		else
		{
		//	InitializeCriticalSection(&csGate);
			EnterCriticalSection(&csTextInfo);
			memset(gszInfo[chNo][j], 0, sizeof(wchar_t)*30);
			LeaveCriticalSection(&csTextInfo);
		//	DeleteCriticalSection(&csGate);
		}
	}

	DeleteCriticalSection(&csTextInfo);
	ch_con_reg->wReset |= 0x1;
	ch_con_reg->wReset &= 0xFFFE;
}

void alarmProcess(WORKMODE eWorkMode)
{
	ARS200FPGA_CH_GATE_REG *ch_gate_reg = NULL;
	ARS200FPGA_CH_CON_REG *ch_con_reg = NULL;
	
	BOOL bChannelAlarmFlag[CHANNEL_NUM];
	BOOL bAlarmRegFlag[CHANNEL_NUM][GATE_NUM];
	
	InitializeCriticalSection(&csAlarmFlag);
	EnterCriticalSection(&csAlarmFlag);
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		bChannelAlarmFlag[i] = gbChannelAlarmFlag[i];
		for(int j = 0; j < GATE_NUM; j++)
		{
			bAlarmRegFlag[i][j] = gbAlarmRegFlag[i][j];
		}
	}
	LeaveCriticalSection(&csAlarmFlag);
	DeleteCriticalSection(&csAlarmFlag);
		
	if(eWorkMode == SingleChannelA || eWorkMode == SingleChannelB || eWorkMode == OneSendOneReceive)
	{
		
		if(CurrentChannelNo==0)
		{
			ch_gate_reg = ch1_gate_reg;
			ch_con_reg = ch1_con_reg;
		}
		else if(CurrentChannelNo==1)
		{
			ch_gate_reg = ch2_gate_reg;
			ch_con_reg = ch2_con_reg;
		}
		if(bChannelAlarmFlag[CurrentChannelNo])
		{
			if(gbAlarmFlag)
			{					
				sys_reg->wLedAlarm |= 0x1;
				//MessageBeep(0xFFFFFFFF);
				PlaySound(_T("SystemStart"),NULL,SND_ALIAS|SND_ASYNC); 
			}
			else
			{
				sys_reg->wLedAlarm &= 0xFFFE;			
			}
			
		}
		else if(!bChannelAlarmFlag[CurrentChannelNo] && ((sys_reg->wLedAlarm & 0x1) == 0x1))
		{
			sys_reg->wLedAlarm &= 0xFFFE;
		}

	//	ch_con_reg->wReset |= 0x1;
	//	ch_con_reg->wReset &= 0xFFFE;
	}
	else // 双通道, 单晶关联, 双晶关联
	{
		if(bChannelAlarmFlag[0] || bChannelAlarmFlag[1])
		{
			if(gbAlarmFlag)
			{					
				sys_reg->wLedAlarm |= 0x1;
				//MessageBeep(0xFFFFFFFF);
				PlaySound(_T("SystemStart"),NULL,SND_ALIAS|SND_ASYNC);
			}
			else
			{
				sys_reg->wLedAlarm &= 0xFFFE;
			}

		}
		else if( !bChannelAlarmFlag[0] && !bChannelAlarmFlag[1] && ((sys_reg->wLedAlarm & 0x1) == 0x1))
		{
			sys_reg->wLedAlarm &= 0xFFFE;
			
		}

	//	ch1_con_reg->wReset |= 0x1;
	//	ch1_con_reg->wReset &= 0xFFFE;

	//	ch2_con_reg->wReset |= 0x1;
	//	ch2_con_reg->wReset &= 0xFFFE;
	}

}


/** 
 * 读波形数据
 * @param     picData [in]:	波形数据
 * @return    void:
 * @since     1.00
 */
void PutsReadDrawData(PIC_DATA& picData, 
					  CSCAN_ARS200FPGA_CH_DATA_REG * ch_data_reg,
					  WORD *ch_data,
					  const WORD& chNo,
					  PEAK_INFO PeakSum[2])
{	
	picData.bDrawed = FALSE;

	if(hWndSpecialDisplay != 0 && gCurrentWnd == hWndSpecialDisplay)
	{
		InitializeCriticalSection(&csZhanKuan);
		EnterCriticalSection(&csZhanKuan);
		BOOL bZhanKuan = gbZhanKuan;
		LeaveCriticalSection(&csZhanKuan);
		DeleteCriticalSection(&csZhanKuan);

		InitializeCriticalSection(&csZhanKuanValue);
		EnterCriticalSection(&csZhanKuanValue);
		float fFront = gZhanKuanFront;
		float fBack = gZhanKuanBack;
		LeaveCriticalSection(&csZhanKuanValue);
		DeleteCriticalSection(&csZhanKuanValue);

		InitializeCriticalSection(&csMinRange);
		EnterCriticalSection(&csMinRange);

		float fMinRange = gfMinRange[chNo];

		LeaveCriticalSection(&csMinRange);
		DeleteCriticalSection(&csMinRange);
		
		// 若为当前通道且展宽为真

		if(bZhanKuan && chNo == CurrentChannelNo)
		{
			float fCha = fBack - fFront;
			float fTemp = fFront;			
	//		chCurrent_data_reg->wMonitorDelayCounter = WORD(500 + Range2DotNew(DisplaySet_ad[chNo].nWaveSpeed, fTemp));
			chCurrent_data_reg->wMonitorDelayCounter = WORD(500 + Range2Dot(DisplaySet_ad[chNo].nWaveSpeed, fTemp));
			
			if(fCha < fMinRange)
			{
				WORD wInterval[DATA_SIZE];
				memset(wInterval, 0, sizeof(wInterval));

				RangeProcess(DisplaySet_ad[chNo].nWaveSpeed, fMinRange, wInterval);

				memcpy(chCurrent_interval, wInterval, sizeof(wInterval));

				if(gnDrawCount == 0)
				{
					memcpy(preDrawData[chNo], ch_data, sizeof(WORD)*DATA_SIZE);
					memcpy(picData.data, ch_data, sizeof(WORD)*DATA_SIZE);
				}
				else
				{
					WORD *pData = ch_data;
					for(int i=0; i<DATA_SIZE; i++)
					{
						picData.data[i] = (*pData + preDrawData[chNo][i])/2;	

						pData++;
					}
				}
				
			}
			else
			{
				// 将显示范围设为展宽前后沿
				WORD wInterval[DATA_SIZE];
				memset(wInterval, 0, sizeof(wInterval));

				RangeProcess(DisplaySet_ad[chNo].nWaveSpeed, fCha, wInterval);

				memcpy(chCurrent_interval, wInterval, sizeof(wInterval));		
			}
		}
		else if(!bZhanKuan)
		{
			// 设定正常情况下的延时和声程

			float fTemp = ReceiveParam_ad[chNo].fDelay;

			if(chNo == 0)

				ch1_data_reg->wMonitorDelayCounter = WORD(500 + MicroSecond2Dot(fTemp));
			else if(chNo == 1)

				ch2_data_reg->wMonitorDelayCounter = WORD(500 + MicroSecond2Dot(fTemp));

			WORD wInterval[DATA_SIZE];
			memset(wInterval, 0, sizeof(wInterval));

			RangeProcess(DisplaySet_ad[chNo].nWaveSpeed, DisplaySet_ad[chNo].fRange, wInterval);

			if(chNo == 0)
				memcpy(ch1_interval, wInterval, sizeof(wInterval));
			else if(chNo == 1)
				memcpy(ch2_interval, wInterval, sizeof(wInterval));
		}
		else
		{
		}
	}

	// 两次求平均

	if(gnDrawCount == 0)
	{
		memcpy(preDrawData[chNo], ch_data, sizeof(WORD)*DATA_SIZE);
		memcpy(picData.data, ch_data, sizeof(WORD)*DATA_SIZE);
	}
	else
	{
		WORD *pData = ch_data;
		for(int i=0; i<DATA_SIZE; i++)
		{
			picData.data[i] = (*pData + preDrawData[chNo][i])/2;	

			pData++;
		}

	}		

	memcpy(preDrawData[chNo], ch_data, sizeof(WORD)*DATA_SIZE);


    //MessageBox(NULL,_T( "PutsReadDrawData"),NULL,0); 
	WORD wLamp = 0;
	memcpy(lamp,&wLamp,2);//关灯

	// 获取闸门数据
	CheckAlarm(chNo, PeakSum);	


	ch_data_reg->wMonitorReset = 1;

	Sleep(1);

	ch_data_reg->wMonitorReset = 0;

	wLamp = 1;
	memcpy(lamp,&wLamp,2);//点灯

}

void CALLBACK TimerProcessProc( HWND hwnd, 
								UINT uMsg, 
								UINT idEvent, 
								DWORD dwTime 
							  )
{
	// 命令通讯
	InitializeCriticalSection(&csCommand);
	EnterCriticalSection(&csCommand);
	
	if (gbInitFlag == TRUE)	// 初始化
	{
		// Song test

		thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_INIT;

		gbInitFlag = FALSE;
		//MessageBox(NULL, _T("初始化了"), NULL, MB_OK);
	}
	else if(gbStartFlag == TRUE) // 开始
	{	
		thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;

		gbStartFlag = FALSE;
	}
	else if(gbEndFlag == TRUE ) // SetOne
	{
		thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;

		gbEndFlag = FALSE;
	}
	else if(gbSetOneFlag == TRUE) // 结束
	{
		thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_SETONE;

		gbSetOneFlag = FALSE;
	}
//	else
//		thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;

	LeaveCriticalSection(&csCommand);
	DeleteCriticalSection(&csCommand);
}

void CALLBACK TimerPressProc( HWND hwnd, 
								UINT uMsg, 
								UINT idEvent, 
								DWORD dwTime 
							  )
{
	if(hStatusBar == NULL)
		return;

	// 获取电压值
/*	if(gPressValue == 0)
		gPressValue = 10;
	gPressValue--;
*/
	gPressValue = (WORD)get_battery();

	//battery test
	char buffer[20];
	memset(buffer, 0, sizeof(buffer));
	_itoa(gPressValue, buffer, 10);
	buffer[18] = '\r';
	buffer[19] = '\n';
	DWORD dwWriteDataSize;
	if(!WriteFile(hFileWrite,buffer,20,&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写battery文件失败"));

		//CloseHandle(hFileWrite);
	}//battery test

	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (6 | SBT_OWNERDRAW | SBT_NOBORDERS ), (LPARAM)(LPWSTR)_T(""));
}

void CALLBACK TimerClockProc( HWND hwnd, 
							  UINT uMsg, 
							  UINT idEvent, 
							  DWORD dwTime 
							)
{
	if(hStatusBar == NULL)
		return;

	// 获取电压值
	gPressValue = (WORD)get_battery();

/*	// Song rem 20131106	
	if(gPressValue < 620)
	{
		MessageBeep(0xFFFFFFFF);

		memset(gszTextPeak, 0, sizeof(gszTextPeak));
		swprintf(gszTextPeak, _T("请及时充电"));

		SendMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	}
*/
	// 获取当前时间
	SYSTEMTIME time;
	GetLocalTime(&time);

	memset(gszTextTime, 0, sizeof(gszTextTime));
	swprintf(gszTextTime, _T("%2d:%02d"), time.wHour, time.wMinute);
	//battery test
#ifdef _NDT_TEST
	hFileWrite = CreateFile(_T("\\ResidentFlash\\battery.txt"),
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("创建battery文件失败"));
	}
	else
	{
		SetFilePointer(hFileWrite, 0, 0, FILE_END);
		char buffer1[5];
		memset(buffer1, 0, sizeof(buffer1));
		_itoa(gPressValue, buffer1, 10);
		buffer1[3] = ' ';
		buffer1[4] = ' ';

		char buffer2[3];
		memset(buffer2, 0, sizeof(buffer2));
		_itoa(time.wHour, buffer2, 10);
		buffer2[2] = ':';
		
		char buffer3[3];
		memset(buffer3, 0, sizeof(buffer3));
		_itoa(time.wMinute, buffer3, 10);
		buffer3[2] = ':';

		char buffer4[4];
		memset(buffer4, 0, sizeof(buffer4));
		_itoa(time.wSecond, buffer4, 10);
		buffer4[2] = '\r';
		buffer4[3] = '\n';
		DWORD dwWriteDataSize;
		if(!WriteFile(hFileWrite,buffer1,5,&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写battery文件失败"));

			//CloseHandle(hFileWrite);
		}
		if(!WriteFile(hFileWrite,buffer2,3,&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写battery文件失败"));

			//CloseHandle(hFileWrite);
		}
		if(!WriteFile(hFileWrite,buffer3,3,&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写battery文件失败"));

			//CloseHandle(hFileWrite);
		}
		if(!WriteFile(hFileWrite,buffer4,4,&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写battery文件失败"));

			//CloseHandle(hFileWrite);
		}
		CloseHandle(hFileWrite);
	}
#endif
	//battery test

	SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (6 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextTime);
}

LRESULT OnStart(HWND hwnd,WORD a,HWND hwndctl,WORD b)
{
	gbInitFlag = TRUE;
	return 0;
}

LRESULT OnStop(HWND hwnd,WORD a,HWND hwndctl,WORD b)
{
	gbEndFlag = TRUE;
	return 0;
}

LRESULT OnAlarmOpen(HWND hwnd,WORD a,HWND hwndctl,WORD b)
{
	sys_reg->wLedAlarm = 1;
	return 0;
}

LRESULT OnAlarmClose(HWND hwnd,WORD a,HWND hwndctl,WORD b)
{
	sys_reg->wLedAlarm = 0;
	return 0;
}

LRESULT OnSendSPIDelay(HWND hwnd,WORD a,HWND hwndctl,WORD b)
{
	send_mcp41010(128);

	chCurrent_data_reg->wMonitorZeroCounter = 10;
	return 0;
}


/*
int Gain2Data( float Gain)
{
	int nData = (int) ((Gain - 60)*255.0f/48.0f);

	return nData;
}
*/

WORD RangeProcess(float VSound, // VSound--------------m/s
				  float Range, // Range --------------m
				  WORD* pInterSpace) // pInterSpace ---------
{
	WORD nRangeDot = Range2Dot(VSound, Range);
	int nTotal= (int)(Range*2/1000.0/VSound*ADFREQUENCY*1000000.0);
	int nBeiShu = nTotal / 16384;//改成8192 2014-5-16
	int nYuShu = nTotal % 16384;
	if (nYuShu != 0)
	{
		nBeiShu++;
	}
	else
	{
		
	}
	int nTotalTemp = nTotal / nBeiShu;
	if(nTotalTemp < MONITOR_SIZE)
	{
         for (int i=0;i<512;i++)
		{		
			pInterSpace[i]=0;
		}
	}
	else
	{
		int nInter=nTotalTemp/512;
		int nReset=nTotalTemp%512;
		int k=1;
		for (int i=0;i<512;i++)
		{
			if(nInter < 1)
				nInter = 1;
			pInterSpace[i]=nInter-1;
		}
		for (int i=0;i<512;i++)
		{
			float temp = i*1.0*nReset/511;
			if(temp>= k )
			{
				pInterSpace[i] = pInterSpace[i] +1;
				k++;
			}
		}
	}
	return nRangeDot;
}

/*

WORD RangeProcess(float VSound, // VSound--------------m/s
				  float Range, // Range --------------m
				  WORD* pInterSpace) // pInterSpace ---------
{
	WORD nRangeDot = Range2Dot(VSound, Range);
	int nTotal= (int)(Range*2/1000.0/VSound*ADFREQUENCY*1000000.0);
	if(nTotal < MONITOR_SIZE)
	{
		for (int i=0;i<512;i++)
		{

			pInterSpace[i]=0;
		}
	}
	else
	{
		int nInter=nTotal/512;
		int nReset=nTotal%512;
		int k=1;
		for (int i=0;i<512;i++)
		{
			if(nInter < 1)
				nInter = 1;
			pInterSpace[i]=nInter-1;
		}



		for (int i=0;i<512;i++)
		{
			float temp = i*1.0*nReset/511;
			if(temp>= k )
			{
				pInterSpace[i] = pInterSpace[i] +1;
				k++;
			}
			else
			{
				;
			}
		}
	}
	return nRangeDot;
}
*/

WORD Range2Dot(float VSound, float Range)
{
	WORD temp;
	temp = WORD(Range*2/1000.0/VSound*ADFREQUENCY*1000000.0);
	
	if(temp == 0)
		temp = 1;
	return temp;
}

// songchenguang add start 2010-08-11
int Range2DotNew(float VSound, float Range)
{
	int temp;
	temp = static_cast<int>(Range*2/1000.0/VSound*ADFREQUENCY*1000000.0 + 0.5);
	
//	if(temp == 0)
//		temp = 1;

	return temp;
}
// songchenguang add end 2010-08-11

LRESULT DoMouseMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);

	switch(wMsg)
	{
	case WM_RBUTTONUP:
/*
		hMainMenu = LoadMenu(hRes, MAKEINTRESOURCE(IDR_MENU_MAIN));
			hMainMenu = GetSubMenu (hMainMenu, 0) ;
			if(hMainMenu)
			{
				TrackPopupMenu(hMainMenu, TPM_LEFTALIGN, rect.left, rect.top, 0, hWnd, NULL);	
			}
			else
				MessageBox(NULL, _T("hMainMenu == NULL"), _T("fail"), MB_OK);
*/
		break;
	}
	return 0;
}


LRESULT DoKeysMain(HWND hWnd, UINT wMsg, WPARAM wParam, LPARAM lParam)
{
	HMENU hMenu = NULL;
	wchar_t szText[20];
	memset(szText, 0, sizeof(szText));

	//wangbingfu add start at 2011-3-1
	BOOL bSaveFlag = TRUE; // 是否保存值到参数文件标志，如果是铁路专用则不保存

	switch(wMsg)
	{
	case WM_KEYUP:
		
		break;
	case WM_KEYDOWN:
		if(!bFreezeUp)	// 未冻结
		{
			//wangbingfu add at 2010-6-13
			if(wParam == VK_UP)
			{
				if(gCurrentFocusBtn)
				{
					InitializeCriticalSection(&csHotRange);
					EnterCriticalSection(&csHotRange);
					gbHotRange = FALSE;
					LeaveCriticalSection(&csHotRange);
					DeleteCriticalSection(&csHotRange);
					gbHotDB = FALSE;

					if(gbHotGateFlag)
					{
						SetFocus(currentGateBtn);
					}
					else if(gbHotCraftParamFlag)
					{
						SetFocus(currentCraftParamBtn);
					}
					else if(gbHotAutoGainFlag)
					{
						SetFocus(currentAutoGainPlusBtn);
					}
					else if(gbHotAutoAdjustFlag)
					{
						SetFocus(currentAdjustBtn);
					}
					else
					{
						if(gCurrentFocusBtn)
						{
							SetFocus(gCurrentFocusBtn);
						}
					}

					memset(gszTextPlus, 0, sizeof(gszTextPlus));
					swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

					memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
					swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
				}

			}
			else if(wParam == VK_DOWN)
			{
				if(gCurrentFocusBtn)
				{
					InitializeCriticalSection(&csHotRange);
					EnterCriticalSection(&csHotRange);
					gbHotRange = FALSE;
					LeaveCriticalSection(&csHotRange);
					DeleteCriticalSection(&csHotRange);
					gbHotDB = FALSE;

					if(gbHotGateFlag)
					{
						SetFocus(currentGateBtn);
					}
					else if(gbHotCraftParamFlag)
					{
						SetFocus(currentCraftParamBtn);
					}
					else if(gbHotAutoGainFlag)
					{
						SetFocus(currentAutoGainPlusBtn);
					}
					else if(gbHotAutoAdjustFlag)
					{
						SetFocus(currentAdjustBtn);
					}
					else
					{
						if(gCurrentFocusBtn)
						{
							SetFocus(gCurrentFocusBtn);
						}
					}
					
					memset(gszTextPlus, 0, sizeof(gszTextPlus));
					swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

					memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
					swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
				}
			}
				//wangbingfu add at 2010-6-13
			else if(wParam == VK_LEFT )
			{
				// 声程
				InitializeCriticalSection(&csHotRange);
				EnterCriticalSection(&csHotRange);
				BOOL bHotRange = gbHotRange;
				LeaveCriticalSection(&csHotRange);
				DeleteCriticalSection(&csHotRange);
				
				float fOldPlus = ReceiveParam_ad[CurrentChannelNo].fPlus;
				float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
				float fOldRange = DisplaySet_ad[CurrentChannelNo].fRange;
					
				if(bHotRange)
				{
					// 声程减小
					if((DisplaySet_ad[CurrentChannelNo].fRange - fRangeStep) > 1.0f)
						DisplaySet_ad[CurrentChannelNo].fRange -= fRangeStep;
					else
						DisplaySet_ad[CurrentChannelNo].fRange = 1.0f;

					// 若显示菜单正显示，更新声程信息
					if(hBtnDisplaySoundDistance != NULL && hWndDisplay != NULL)
						SetRangeLableAndExecute(hBtnDisplaySoundDistance, DisplaySet_ad[CurrentChannelNo].fRange, bSaveFlag);
					else
						SetRangeLableAndExecute(NULL, DisplaySet_ad[CurrentChannelNo].fRange, bSaveFlag);

					rangeChangeCurve(fOldRange, TRUE);
				}
				else if(gbHotDB) // 增益
				{
					if(gDBIndex == 1)
					{
						if((ReceiveParam_ad[CurrentChannelNo].fPlus - fPlusStep) > MIN_PLUS )
							ReceiveParam_ad[CurrentChannelNo].fPlus -= fPlusStep;
						else
							ReceiveParam_ad[CurrentChannelNo].fPlus = MIN_PLUS;

						memset(gszTextPlus, 0, sizeof(gszTextPlus));
						swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);
					}
					else if(gDBIndex == 2)
					{
						if((ReceiveParam_ad[CurrentChannelNo].fRefPlus - fPlusStep) > MIN_PLUS )
							ReceiveParam_ad[CurrentChannelNo].fRefPlus -= fPlusStep;
						else
							ReceiveParam_ad[CurrentChannelNo].fRefPlus = MIN_PLUS;

						if(hWndTlAdjust)
						{
							TlBiaoMianBuChangDisplay(hBtnTlAdjustBiaoMianBuChang, gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]]);
						}

						memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
						swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
					}
					
					if(hWndRecvParam != NULL && hBtnRecvParamGain != NULL && gDBIndex == 1)
						SetPlusLableAndExecute(hBtnRecvParamGain, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, bSaveFlag, CurrentChannelNo);
					else if(hWndRecvParam != NULL && hBtnRecvParamReferenceGain != NULL && gDBIndex == 2)
						SetRefPlusLableAndExecute(hBtnRecvParamReferenceGain, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, bSaveFlag, CurrentChannelNo);
					else if(hWndRecvParam == NULL && gDBIndex == 1)
						SetPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, bSaveFlag, CurrentChannelNo);
					else if(hWndRecvParam == NULL && gDBIndex == 2)
						SetRefPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, bSaveFlag, CurrentChannelNo);
					else
					{
						;//
					}
					if(hBtnZhongHouBanTanShangSensitive != NULL && gDBIndex == 2)
						SetFnRefPlusLableAndExecute(hBtnZhongHouBanTanShangSensitive);
					else if(hBtnFuHeCaiLiaoJianCeSensitive != NULL && gDBIndex == 2)
						SetFnRefPlusLableAndExecute(hBtnFuHeCaiLiaoJianCeSensitive);
					else if(hBtnZhuJianSensitive != NULL && gDBIndex == 2)
						SetFnRefPlusLableAndExecute(hBtnZhuJianSensitive);
					else
					{
					}


					gainChangeCurve(fOldPlus, TRUE);
					refGainChangeCurve(fOldRefPlus, TRUE);//wangbingfu add at 2011-03-09
				}				
			}
			else if(wParam == VK_RIGHT)
			{
				// 声程
				InitializeCriticalSection(&csHotRange);
				EnterCriticalSection(&csHotRange);
				BOOL bHotRange = gbHotRange;
				LeaveCriticalSection(&csHotRange);
				DeleteCriticalSection(&csHotRange);

				float fOldPlus = ReceiveParam_ad[CurrentChannelNo].fPlus;
				float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
				float fOldRange = DisplaySet_ad[CurrentChannelNo].fRange;



				if(bHotRange)
				{
					// 声程变大				
					if((DisplaySet_ad[CurrentChannelNo].fRange + fRangeStep)< MAX_RANGE)
						DisplaySet_ad[CurrentChannelNo].fRange += fRangeStep;
					else
						DisplaySet_ad[CurrentChannelNo].fRange = MAX_RANGE;

					// 若显示菜单正显示，更新声程信息
					if(hBtnDisplaySoundDistance != NULL && hWndDisplay != NULL)
						SetRangeLableAndExecute(hBtnDisplaySoundDistance, DisplaySet_ad[CurrentChannelNo].fRange, bSaveFlag);
					else
						SetRangeLableAndExecute(NULL, DisplaySet_ad[CurrentChannelNo].fRange, bSaveFlag);
					
					rangeChangeCurve(fOldRange, TRUE);
				}			
				else if(gbHotDB) // 增益
				{				
					if(gDBIndex == 1)
					{
						if((ReceiveParam_ad[CurrentChannelNo].fPlus + fPlusStep) < (MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fRefPlus))
							ReceiveParam_ad[CurrentChannelNo].fPlus += fPlusStep;
						else
							ReceiveParam_ad[CurrentChannelNo].fPlus = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fRefPlus;

						memset(gszTextPlus, 0, sizeof(gszTextPlus));
						swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);
					}
					else if(gDBIndex == 2)
					{
						if((ReceiveParam_ad[CurrentChannelNo].fRefPlus + fPlusStep) < (MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus))
							ReceiveParam_ad[CurrentChannelNo].fRefPlus += fPlusStep;
						else
							ReceiveParam_ad[CurrentChannelNo].fRefPlus = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus;

						if(hWndTlAdjust)
						{
							TlBiaoMianBuChangDisplay(hBtnTlAdjustBiaoMianBuChang, gfTlBuChangPlus[CurrentChannelNo][gwStandard[CurrentChannelNo]][gwPosition[CurrentChannelNo][gwStandard[CurrentChannelNo]]]);
						}
						
						memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
						swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
					}

					if(hWndRecvParam != NULL && hBtnRecvParamGain != NULL && gDBIndex == 1)
						SetPlusLableAndExecute(hBtnRecvParamGain, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, bSaveFlag, CurrentChannelNo);
					else if(hWndRecvParam != NULL && hBtnRecvParamReferenceGain != NULL && gDBIndex == 2)
						SetRefPlusLableAndExecute(hBtnRecvParamReferenceGain, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, bSaveFlag, CurrentChannelNo);
					else if(hWndRecvParam == NULL && gDBIndex == 1)
						SetPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, bSaveFlag, CurrentChannelNo);
					else if(hWndRecvParam == NULL && gDBIndex == 2)
						SetRefPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, bSaveFlag, CurrentChannelNo);

					//刷新检测灵敏度wangbingfu add start at 2010-01-10
					if(hBtnZhongHouBanTanShangSensitive != NULL && gDBIndex == 2)
						SetFnRefPlusLableAndExecute(hBtnZhongHouBanTanShangSensitive);
					else if(hBtnFuHeCaiLiaoJianCeSensitive != NULL && gDBIndex == 2)
						SetFnRefPlusLableAndExecute(hBtnFuHeCaiLiaoJianCeSensitive);
					else if(hBtnZhuJianSensitive != NULL && gDBIndex == 2)
						SetFnRefPlusLableAndExecute(hBtnZhuJianSensitive);
					else
					{
						;//
					}
					//wangbingfu add end at 2010-01-10

					gainChangeCurve(fOldPlus, TRUE);
					refGainChangeCurve(fOldRefPlus, TRUE);//wangbingfu add at 2011-03-09
				}
			}
			else if(wParam == 'D') // 增益
			{
				InitializeCriticalSection(&csHotRange);
				EnterCriticalSection(&csHotRange);
				gbHotRange = FALSE;
				LeaveCriticalSection(&csHotRange);
				DeleteCriticalSection(&csHotRange);

				gbHotDB = TRUE;
				gDBIndex++;
				//wangbingfu add at 2010-6-13
				SetFocus(hWndMain);
				//wangbingfu add at 2010-6-13
				if(gDBIndex == 1) // 基准增益
				{
				}
				else if(gDBIndex == 2) // 参考增益
				{
				}
				else if(gDBIndex == 3) // 关闭
				{
					gDBIndex = 1;
				}

				memset(gszTextPlus, 0, sizeof(gszTextPlus));
				swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

				PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

				memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
				swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

				PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
				
				// display db step
				memset(gszTextStep, 0, sizeof(gszTextStep));
				swprintf(gszTextStep, _T("%.1f"), fPlusStep);
				PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				
			}
			else if(wParam == 'E') // 声程
			{
				// close db hotkey
				gbHotDB = FALSE;
				gDBIndex = 0;
				//wangbingfu add at 2010-6-13
				SetFocus(hWndMain);
				//wangbingfu add at 2010-6-13
				memset(gszTextPlus, 0, sizeof(gszTextPlus));
				swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

				PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

				memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
				swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

				PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);

				// 
				InitializeCriticalSection(&csHotRange);
				EnterCriticalSection(&csHotRange);
				gbHotRange = TRUE;
				LeaveCriticalSection(&csHotRange);
				DeleteCriticalSection(&csHotRange);
				
				// display range step
				memset(gszTextStep, 0, sizeof(gszTextStep));
				swprintf(gszTextStep, _T("%.1f"), fRangeStep);
				PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			}
			else if(wParam == 'F')
			{
				BOOL bHotRange = FALSE;

				InitializeCriticalSection(&csHotRange);
				EnterCriticalSection(&csHotRange);
				bHotRange = gbHotRange;
				LeaveCriticalSection(&csHotRange);
				DeleteCriticalSection(&csHotRange);

				//
				memset(gszTextStep, 0, sizeof(gszTextStep));
				if(bHotRange)
				{
					if(nRangeStepIndex < 3)
						nRangeStepIndex++;
					else
						nRangeStepIndex = 0;

					fRangeStep = AMENDSTEPS[nRangeStepIndex];

					swprintf(gszTextStep, _T("%.1f"), fRangeStep);
					PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
				else if(gbHotDB)
				{
					if(nPlusStepIndex < 3)
						nPlusStepIndex++;
					else
						nPlusStepIndex = 0;

					fPlusStep = PLUSSTEPS[nPlusStepIndex];

					swprintf(gszTextStep, _T("%.1f"), fPlusStep);
					PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
				
			}
			else if(wParam == 'A' || wParam == VK_RETURN) // 参数
			{
				if(gwDeviceType == 0)
				{
					if(!hWndTsMenu && (hWndFnMenu == 0) && !bFreezeUp)
					{
						// songchenguang add start 2010-4-26
						// 修改菜单显示标志
						SetMenuStateFlag(TRUE);
						// songchenguang add end 2010-4-26
						Sleep(30);	//菜单先画后改变显示区，会出现图形画到菜单的情况 所以先等待30ms

						hWndTsMenu = CreateWindow(szTsMenuChild, TEXT(""), WS_CHILD,
							MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWnd, NULL, hInst, NULL);//创建探伤菜单窗口
						ShowWindow(hWndTsMenu,SW_SHOW);
						UpdateWindow(hWndTsMenu);

						gCurrentWnd = hWndTsMenu;
						// close other hotkey
						gbHotDB = FALSE;
						gDBIndex = 0;

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
					else if(hWndTsMenu)//按声程或增益快捷键,菜单失焦后再按参数设置键返回菜单的上次焦点
					{
						if(gCurrentFocusBtn && (gbHotRange == TRUE || gbHotDB == TRUE))
						{
							SetFocus(gCurrentFocusBtn);
							InitializeCriticalSection(&csHotRange);
							EnterCriticalSection(&csHotRange);
							gbHotRange = FALSE;
							LeaveCriticalSection(&csHotRange);
							DeleteCriticalSection(&csHotRange);

							gbHotDB = FALSE;

							memset(gszTextPlus, 0, sizeof(gszTextPlus));
							swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

							memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
							swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
						}
					}
				}
				else if(gwDeviceType == 1)
				{
					if(!hWndChMenu && !bFreezeUp)
					{
						ShowWindow(hWndChDisplay, SW_HIDE);


						hWndChMenu = CreateWindow(szChMenuChild, TEXT(""), WS_CHILD,
							MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWnd, NULL, hInst, NULL);//创建测厚菜单窗口
						ShowWindow(hWndChMenu,SW_SHOW);
						UpdateWindow(hWndChMenu);

						gCurrentWnd = hWndChMenu;

						// close other hotkey
						gbHotDB = FALSE;
						gDBIndex = 0;

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
			}			
			else if(wParam == 'C')	//Fn
			{
			/*	while(1)
				{
					DWORD ret = WaitForSingleObject(hWorkEvent, 0);
			
					if (ret == WAIT_OBJECT_0)
					{*/
						if(!hWndFnMenu && (hWndTsMenu == 0) && !bFreezeUp && !gbHotGateFlag && !gbHotAutoGainFlag && !gbHotAutoAdjustFlag)
						{
							if(gwDeviceType == 0)
							{
								SetMenuStateFlag(TRUE);
							}
							else if(gwDeviceType == 1)
							{
								ShowWindow(hWndChDisplay, SW_HIDE);
							}
							else if(gwDeviceType == 2)
							{
								ShowWindow(hWndTlMenu, SW_HIDE);
							}
							Sleep(30);	//菜单先画后改变显示区，会出现图形画到菜单的情况 所以先等待30ms

							hWndFnMenu = CreateWindow(szFnMenuChild, TEXT(""), WS_CHILD,
										 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWnd, NULL, hInst, NULL);//创建Fn功能菜单窗口
							ShowWindow(hWndFnMenu,SW_SHOW);
							UpdateWindow(hWndFnMenu);
							
							gCurrentWnd  = hWndFnMenu;
							// close other hotkey
							gbHotDB = FALSE;
							gDBIndex = 0;

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

//						break;
				/*	}
				//	else
				//		Sleep(10);
				}*/
			}			
			else if(wParam == VK_ESCAPE)
			{	
				
				InitializeCriticalSection(&csHotRange);
				EnterCriticalSection(&csHotRange);
				gbHotRange = FALSE;
				LeaveCriticalSection(&csHotRange);
				DeleteCriticalSection(&csHotRange);
				gbHotDB = FALSE;

				if(gbHotGateFlag)
				{
					SetFocus(currentGateBtn);
				}
				else if(gbHotCraftParamFlag)
				{
					SetFocus(currentCraftParamBtn);
				}
				else if(gbHotAutoGainFlag)
				{
					SetFocus(currentAutoGainPlusBtn);
				}
				else if(gbHotAutoAdjustFlag)
				{
					SetFocus(currentAdjustBtn);
				}
				else
				{
					if(gCurrentFocusBtn)
					{
						SetFocus(gCurrentFocusBtn);
					}
				}

				InitializeCriticalSection(&csHotRange);
				EnterCriticalSection(&csHotRange);
				gbHotRange = FALSE;
				LeaveCriticalSection(&csHotRange);
				DeleteCriticalSection(&csHotRange);

				gbHotDB = FALSE;

				memset(gszTextPlus, 0, sizeof(gszTextPlus));
				swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

				PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

				memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
				swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

				PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
			}
			// wangbingfu add end on 2010-04-19
			// wangbingfu add end on 2010-07-21
		/*	else if(wParam == 'P')
			{
				if(gwDeviceType == 0)
				{
					gwDeviceType = 1;

					if(hWndChDisplay == 0)
					{
					//	SetMenuStateFlag(TRUE);

						hWndChDisplay = CreateWindow(szChDisplayChild, TEXT(""), WS_CHILD,
									 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWnd, NULL, hInst, NULL);//创建测厚测量显示窗口
						ShowWindow(hWndChDisplay,SW_SHOW);
						UpdateWindow(hWndChDisplay);

						gCurrentWnd = hWndChDisplay;
					}
				}
				else if(gwDeviceType == 1)
				{
					gwDeviceType = 0;
					
					DestroyWindow(hWndChDisplay);
					hWndChDisplay = 0;

				//	gCurrentWnd = 0;
						
					SetMenuStateFlag(FALSE);
				}
			}*/
			
			// wangbingfu add end on 2010-07-21
			// wangbingfu add end on 2010-07-23
			//test保存波形数据
			else if(wParam == 'S')
			{
			/*	wchar_t strText[50];
				memset(strText, 0, sizeof(strText));
				wsprintf(strText, _T("\\ResidentFlash\\wave%d.bin"), gwWaveFileNum);

				saveWaveFile(strText, gPicData[CurrentChannelNo].data);
				gwWaveFileNum++;
				*/
				wchar_t strText[50];
				memset(strText, 0, sizeof(strText));
				wsprintf(strText, _T("\\ResidentFlash\\wave%d.txt"), gwWaveFileNum);

				saveWaveFile(strText, gPicData[CurrentChannelNo].data);
				gwWaveFileNum++;
			}
		}
		else	//冻结
		{
			if(wParam == VK_LEFT )
			{
				FreezeLeftMove();
				DrawPeakInfo();
			}
			else if(wParam == VK_RIGHT)
			{
				FreezeRightMove();
				DrawPeakInfo();
			}
		}
		
		// wangbingfu add end on 2010-07-23
		if(wParam == 'B')	// 冻结
		{
			if(!gbBscanSwitch)	//B扫时无法冻结
			{
				if(bFreezeUp)
				{
					bFreezeUp = FALSE;
					if(gbHotGateFlag)
					{
						ShowWindow(hWndGate, SW_SHOW);
						SetFocus(currentGateBtn);//上级菜单最后选中的按钮获得输入焦点
					}
					else if(gbHotCraftParamFlag)
					{
						ShowWindow(hWndCraftParam, SW_SHOW);
						SetFocus(currentCraftParamBtn);//上级菜单最后选中的按钮获得输入焦点
					}
					else if(gbHotAutoGainFlag)
					{
						SetFocus(currentAutoGainPlusBtn);
					}
					else if(gbHotAutoAdjustFlag)
					{
						SetFocus(currentAdjustBtn);
					}
					else
					{
						if(gCurrentFocusBtn)
						{
							SetFocus(gCurrentFocusBtn);
						}
					}
				}
				else
				{
					bFreezeUp = TRUE;
					SetFocus(hWndMain);
				}
				if(hBtnSpecialDisplayFreeze)
				{
					if(bFreezeUp)
						DrawLableText(hBtnSpecialDisplayFreeze, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					else
						DrawLableText(hBtnSpecialDisplayFreeze, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				if(bFreezeUp)
				{
					InitializeCriticalSection(&csHotRange);
					EnterCriticalSection(&csHotRange);
					gbHotRange = FALSE;
					LeaveCriticalSection(&csHotRange);
					DeleteCriticalSection(&csHotRange);

					gbHotDB = FALSE;

					memset(gszTextPeak, 0, sizeof(gszTextPeak));

					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);

					memset(gszTextPlus, 0, sizeof(gszTextPlus));
					swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

					memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
					swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (4 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
				}

				FreezeUp(hWnd, bFreezeUp);
			}
		}
		break;
	case WM_CHAR:
		if(!bFreezeUp)	// 未冻结
		{
			if((gFunctionDefine & WORKMODE1PLUS2_SWITCH) != 0)	//双通道功能打开时才能切换通道
			{
				// 切换通道
				InitializeCriticalSection(&csSysInfo);
				EnterCriticalSection(&csSysInfo);
				if( wParam == '\t' && gSysInfo.wWorkMode != OneSendOneReceive &&
					gSysInfo.wWorkMode != DanJingGuanLian && 
					gSysInfo.wWorkMode != ShuangJingGuanLian)
				{
					ChangeChannel(bSaveFlag, -1);
				}
				LeaveCriticalSection(&csSysInfo);
				DeleteCriticalSection(&csSysInfo);
			}
			//wangbingfu add at 2010-8-9
			if( wParam == '1')
			{
				if(!gbHotAutoGainFlag && !gbHotAutoAdjustFlag) //自动增益跟自动校准都没开快捷菜单
				{
					if(!gbHotGateFlag)
					{
						gbHotGateFlag = TRUE;
						if(!hWndGate)
						{
							if(!gCurrentWnd)//没有子菜单窗口
							{
								SetMenuStateFlag(TRUE);
							}
							hWndGate = CreateWindow(szGateChild, TEXT(""), WS_CHILD|WS_VISIBLE,
											 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
							if(gCurrentWnd)//有子菜单窗口
								ShowWindow(gCurrentWnd,SW_HIDE);
							//SetFocus(hWndGate);
							ShowWindow(hWndGate,SW_SHOW);
							UpdateWindow(hWndGate);

							InitializeCriticalSection(&csHotRange);
							EnterCriticalSection(&csHotRange);
							gbHotRange = FALSE;
							LeaveCriticalSection(&csHotRange);
							DeleteCriticalSection(&csHotRange);

							gbHotDB = FALSE;

							memset(gszTextPlus, 0, sizeof(gszTextPlus));
							swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

							memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
							swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (4 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);

					/*		memset(gszTextPeak, 0, sizeof(gszTextPeak));

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (5| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);*/
							
						}
					}
					else
					{
						gbHotGateFlag = FALSE;

					/*	memset(gszTextPeak, 0, sizeof(gszTextPeak));

						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
						*/
						if(gCurrentWnd)//有子菜单窗口
						{
							ShowWindow(gCurrentWnd, SW_SHOW);
							SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
						}
						
						DestroyWindow(hWndGate);
						hWndGate = 0;

						
						if(!gCurrentWnd)//没有子菜单窗口
						{
							SetMenuStateFlag(FALSE);
							SetFocus(hWndMain);//主窗口获得输入焦点
						}
					}
				}

			}		
			//wangbingfu add at 2010-8-9
			//wangbingfu add at 2010-8-14		
			else if( wParam == '2')
			{
				if(!gbHotGateFlag && !gbHotAutoAdjustFlag)	//闸门跟自动校准都没开快捷菜单
				{
					if(!gbHotAutoGainFlag)
					{
						gbHotAutoGainFlag = TRUE;
						if(!hWndAutoGainPlus)
						{
							if(!gCurrentWnd)//没有子菜单窗口
							{
								SetMenuStateFlag(TRUE);
							}
							hWndAutoGainPlus = CreateWindow(szAutoGainPlusChild, TEXT(""), WS_CHILD|WS_VISIBLE,
											 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
						

							if(gCurrentWnd)//有子菜单窗口
								ShowWindow(gCurrentWnd,SW_HIDE);
							
							ShowWindow(hWndAutoGainPlus,SW_SHOW);
							UpdateWindow(hWndAutoGainPlus);

							InitializeCriticalSection(&csHotRange);
							EnterCriticalSection(&csHotRange);
							gbHotRange = FALSE;
							LeaveCriticalSection(&csHotRange);
							DeleteCriticalSection(&csHotRange);

							gbHotDB = FALSE;

							memset(gszTextPlus, 0, sizeof(gszTextPlus));
							swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

							memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
							swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
							
						}
					}
					else
					{
						if(gCurrentWnd)//有子菜单窗口
						{
							ShowWindow(gCurrentWnd, SW_SHOW);
							SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
						}

						DestroyWindow(hWndAutoGainPlus);
						hWndAutoGainPlus = 0;
						if(!gCurrentWnd)//没有子菜单窗口
						{
							SetMenuStateFlag(FALSE);
							SetFocus(hWndMain);//主窗口获得输入焦点
						}
						
						gbHotAutoGainFlag = FALSE;
					}
				}
			}	
			//wangbingfu add at 2010-8-14
			//wangbingfu add at 2010-6-9
			else if( wParam == '3')
			{
				gbAlarmFlag = !gbAlarmFlag;

				memset(gszTextPeak, 0, sizeof(gszTextPeak));

				PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
			}
	/*		else if( wParam == '8')
			{
				TCHAR szFilePath[30];
				swprintf(szFilePath, _T("%s%d.bmp"), FileDirectory, pictureNum);
				ScreenCapture(szFilePath, 16, NULL);
				pictureNum++;
			}*/
			// songchenguang add start 2010-10-13
#ifdef _NDT_TEST
			// 存512点数据
			else if(wParam == '5')
			{
				// stop thread
				InitializeCriticalSection(&csCommand);
				EnterCriticalSection(&csCommand);
				thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;
				LeaveCriticalSection(&csCommand);
				DeleteCriticalSection(&csCommand);
				Sleep(20);
				// save data
				HANDLE hFileWrite;	//文件句柄
				DWORD dwWriteDataSize;
				hFileWrite = CreateFile(_T("\\ResidentFlash\\射频数据.txt"),
										GENERIC_READ|GENERIC_WRITE,
										0,
										NULL,
										CREATE_ALWAYS,
										FILE_ATTRIBUTE_NORMAL,
										NULL);
				if(hFileWrite == INVALID_HANDLE_VALUE)
				{
					PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("创建文件失败"));
					return -1;
				}
	
				for(int i=0; i<512; i++)
				{				
					char szText[10];
					memset(szText, 0, sizeof(szText));
					sprintf(szText, "%d\t\r\n", gPicData[CurrentChannelNo].data[i]);

					if(!WriteFile(hFileWrite,&szText, sizeof(szText),&dwWriteDataSize,NULL))
					{						
						CloseHandle(hFileWrite);
						return -1;
					}
				}

				CloseHandle(hFileWrite);

			}
			// songchenguang add end 2010-10-13
#endif
			else if( wParam == '4')
			{
				if(!gbHotGateFlag && !gbHotAutoGainFlag)	//闸门跟自动增益都没开快捷菜单
				{
					if(!gbHotAutoAdjustFlag)
					{
						gbHotAutoAdjustFlag = TRUE;
						if(!hWndAdjust)
						{
							if(!gCurrentWnd)//没有子菜单窗口
							{
								SetMenuStateFlag(TRUE);
							}
							hWndAdjust= CreateWindow(szAdjustChild, TEXT(""), WS_CHILD|WS_VISIBLE,
											 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
							if(gCurrentWnd)//有子菜单窗口
								ShowWindow(gCurrentWnd,SW_HIDE);
							//SetFocus(hWndAdjust);
							ShowWindow(hWndAdjust,SW_SHOW);
							UpdateWindow(hWndAdjust);

							InitializeCriticalSection(&csHotRange);
							EnterCriticalSection(&csHotRange);
							gbHotRange = FALSE;
							LeaveCriticalSection(&csHotRange);
							DeleteCriticalSection(&csHotRange);

							gbHotDB = FALSE;

							memset(gszTextPlus, 0, sizeof(gszTextPlus));
							swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

							memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
							swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
							
						}
					}

					else
					{
						if(gCurrentWnd)//有子菜单窗口
						{
							ShowWindow(gCurrentWnd, SW_SHOW);
							SetFocus(gCurrentFocusBtn);//上级菜单最后选中的按钮获得输入焦点
						}

						DestroyWindow(hWndAdjust);
						hWndAdjust = 0;
						if(!gCurrentWnd)//没有子菜单窗口
						{
							SetMenuStateFlag(FALSE);
							SetFocus(hWndMain);//主窗口获得输入焦点
						}
						
						gbHotAutoAdjustFlag = FALSE;
					}
				}
			}
			//wangbingfu add at 2010-6-9
			// songchenguang add start 2010-08-05
			else if(wParam == 'x' || wParam == 'X')	// 退出程序，启动explore
			{
				// 启动explore
				SHELLEXECUTEINFO ShellInfo;
				memset(&ShellInfo, 0, sizeof(ShellInfo)); 

				ShellInfo.cbSize = sizeof(SHELLEXECUTEINFO);
				ShellInfo.hwnd = NULL; 

				ShellInfo.lpVerb = _T("open"); 

				ShellInfo.lpFile = _T("\\Windows\\explorer.exe"); 

				ShellInfo.nShow = SW_SHOWNORMAL; 

				ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

				ShellExecuteEx(&ShellInfo);

				// 退出程序
	//			if(int(ShellInfo.hInstApp) > 32)
	//			{
					ExitProcess(0);
	//			}
			}	
			// songchenguang add end 2010-08-05
			else if(wParam == 'z')
			{
				// 递增颜色序号
				EnterCriticalSection(&csSysColor);
				gCurrentSysColorIndex++;
				gbInitializeSysColor = TRUE;

				if(gCurrentSysColorIndex >= SYS_COLOR_NUM)
					gCurrentSysColorIndex = 0;

				LeaveCriticalSection(&csSysColor);

				SetRedrawFlag(TRUE);
			}
		}
		break;
	}
	return 0;
}

//wangbingfu add start at 2010-12-08
LRESULT DoHotKey(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case 1001:
			{
				TCHAR szFilePath[30];
				swprintf(szFilePath, _T("%s%d.bmp"), FileDirectory, pictureNum);
				ScreenCapture(szFilePath, 16, NULL);
				pictureNum++;
			}
			break;
		
		case 1002:
			if(!gbHotCraftParamView)	// 显示参数预览窗口
			{	
				if(!hWndAllParamCfg)
				{
					if(ReadParamFile(lpDefaultParamFile))
					{
						gbHotCraftParamView = TRUE;
							
						InitializeCriticalSection(&csCommand);
						EnterCriticalSection(&csCommand);
						
						thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;
						LeaveCriticalSection(&csCommand);
						DeleteCriticalSection(&csCommand);
						Sleep(20); //等待线程完全停止

						gbBscanSwitch = FALSE;	//关闭BSCAN

						hWndAllParamCfgCaption = CreateWindow(szAllParamCfgCaptionChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								 0, 0, PARAM_WIDTH, 30, hWndMain, NULL, hInst, NULL);
						hWndAllParamCfg = CreateWindow(szAllParamCfgChild, TEXT(""), WS_CHILD|WS_VISIBLE|WS_VSCROLL,
								 PARAM_STRART_X, PARAM_STRART_Y, PARAM_WIDTH, PARAM_HEIGHT - 30, hWndMain, NULL, hInst, NULL);
						if(gbHotGateFlag)
						{
							ShowWindow(hWndGate,SW_HIDE);
						}
						else if(gbHotAutoGainFlag)
						{
							ShowWindow(hWndAutoGainPlus,SW_HIDE);
						}
						else if(gbHotAutoAdjustFlag)
						{
							ShowWindow(hWndAdjust,SW_HIDE);
						}
						else	//没有打开快捷菜单时
						{
							if(gCurrentWnd)
								ShowWindow(gCurrentWnd,SW_HIDE);
						}

						ShowWindow(hStatusBar,SW_HIDE);
						
						ShowWindow(hWndAllParamCfgCaption,SW_SHOW);
						UpdateWindow(hWndAllParamCfgCaption);
						ShowWindow(hWndAllParamCfg,SW_SHOW);
						UpdateWindow(hWndAllParamCfg);						
					}
				}
			}
			break;
	}
	return 0;
}

//wangbingfu add end at 2010-12-08

/** 
 * 显示windows错误信息
 * @return    void:
 * @since     1.00
 */
void ErroWindowMessage()
{
	DWORD dwError = GetLastError();
	HLOCAL hLocal = NULL;

	BOOL fOK = ::FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, dwError,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPTSTR)&hLocal,0,NULL);
	if (!fOK)
	{
		// Is it a network related error?
 		HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"),NULL,
 			DONT_RESOLVE_DLL_REFERENCES);

		if (hDll != NULL)
		{
			::FormatMessage(
				FORMAT_MESSAGE_FROM_HMODULE|FORMAT_MESSAGE_FROM_SYSTEM,
				hDll,dwError,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
				(LPTSTR)&hLocal,0,NULL);
			::FreeLibrary(hDll);
		}
	}

	if (hLocal != NULL)
	{
		MessageBox(NULL, (LPTSTR)hLocal, _T("Error"), MB_OK);
	}
	else
		MessageBox(NULL, _T("未找到对应的错误序号!"), _T("Error"), MB_OK);
}





/** 
 * 显示显示设置对话框
 * @param     hwnd [in]:		窗口句柄.
 * @param     wID [in]:			命令ID.
 * @param     hwndctl [in]:		控件句柄.
 * @param     wNotifyCode [in]: 通知码.
 * @return    LRESULT:
 * @since     1.00
 */
LRESULT OnDisplaySet(HWND hwnd,WORD wID,HWND hwndctl,WORD wNotifyCode)
{

	return 0;
}

/** 
 * 初始化Slider和Edit
 * @param     hSld [in]: Slider句柄.
 * @param     hEdit [in]: 对应的Edit句柄.
 * @param     nMaxRange [in]: 范围最大值.
 * @param     nMinRange [in]: 范围最小值.
 * @param     nPageSize [in]: 单次点击Slider增加值.
 * @param     nPos [in]: Slider位置值.
 * @return    void
 * @since     1.00
 */
void InitSldAndEit(HWND hSld, 
				   HWND hEdit, 
				   WORD nMaxRange, 
				   WORD nMinRange, 
				   WORD nPageSize,
				   WORD nPos)
{
	WCHAR szString[10];
	memset(szString, 0, sizeof(szString));

	// 设范围的上下限
	SendMessage(hSld,TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(nMinRange, nMaxRange));
	SendMessage(hSld, TBM_SETPAGESIZE, 0, (LPARAM) nPageSize);                  // new page size 

    SendMessage(hSld, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG(nMinRange, nMaxRange)); 
	//...
	SendMessage(hSld, TBM_SETPOS,  (WPARAM) TRUE, (LPARAM) nPos);

	swprintf(szString, _T("%d"), nPos);
	SendMessage(hEdit, EM_REPLACESEL, 0, (LPARAM) szString);
}

/** 
 * 滑杆通知处理函数
 * @param     wParam [in]: wParam of WM_HSCROLL message.
 * @param     hwndTrack [in]: handle of trackbar window.
 * @param     iSelMin [in]: minimum value of trackbar selection.
 * @param     iSelMax [in]: maximum value of trackbar selection.
 * @param     hEdit [in]: 对应的Edit控件句柄.
 * @return    void
 * @since     1.00
 */
VOID WINAPI TBNotifications( WPARAM wParam,  
							 HWND hwndTrack, 
							 UINT iSelMin,  
							 UINT iSelMax,  
							 HWND hEdit)
{
	DWORD dwPos; // current position of slider
	WCHAR szString[10];	

	switch (LOWORD(wParam)) 
	{
	case TB_ENDTRACK:
		dwPos = SendMessage(hwndTrack, TBM_GETPOS, 0, 0);
		if (dwPos > iSelMax)
			SendMessage(hwndTrack, TBM_SETPOS,
			(WPARAM) TRUE, // redraw flag
			(LPARAM) iSelMax);
		else if (dwPos < iSelMin)
			SendMessage(hwndTrack, TBM_SETPOS,
			(WPARAM) TRUE, // redraw flag
			(LPARAM) iSelMin);

		dwPos = SendMessage(hwndTrack, TBM_GETPOS, 0, 0);
		memset(szString, 0, sizeof(szString));
		swprintf(szString, _T("%d"), dwPos);

		SetWindowText(hEdit, szString);
		break;
	case TB_THUMBPOSITION:		

		break;
	default:
		break;
	}
}

/** 
 * 采集点转换为距离
 * @param     VSound [in]: 声速.
 * @param     DotNo [in]: 点的值.
 * @return    float: Range (mm).
 * @since     1.00
 */
float Dot2Range(float VSound, int DotNo)
{
	float temp;
	temp = DotNo/2.0*1000.0*VSound/float(ADFREQUENCY)/1000000.0; 
	return temp;
}

/** 
 * 初始化程序参数值
 * @return    void
 * @since     1.00
 */
BOOL InitParam()
{
	// 工作模式
	InitializeCriticalSection(&csSysInfo);
	EnterCriticalSection(&csSysInfo);

	CurrentChannelNo = gSysInfo.wCurrentChNo;//wangbingfu add at 2011-4-20
	
	sys_reg->wWorkMode = gSysInfo.wWorkMode;
	switch(gSysInfo.wWorkMode)
	{
	case SingleChannelA:
		gSysInfo.wRelay &= 0xFFFC;	
		gbGuanLian = FALSE; 
		SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)SingleChannelA);
		break;
	case SingleChannelB:
		gSysInfo.wRelay &= 0xFFFC;
		gbGuanLian = FALSE; 
		SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)SingleChannelB);
		break;
	case DoubleChannels:		
		gSysInfo.wRelay &= 0xFFFC;
		gbGuanLian = FALSE; 
		SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)DoubleChannels);
		break;
	case OneSendOneReceive:		
		gSysInfo.wRelay &= 0xFFFD;
		gSysInfo.wRelay |= 0x1;
		
		//if(gwDeviceType == 0)//测厚时一发一收不关联
			gbGuanLian = TRUE; 
		SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)OneSendOneReceive);
		break;
	case DanJingGuanLian:		
		gSysInfo.wRelay &= 0xFFFE;
		gSysInfo.wRelay |= 0x2;

		gbGuanLian = TRUE;
		SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)DanJingGuanLian);
		break;
	case ShuangJingGuanLian:		
		gSysInfo.wRelay |= 0x3;

		gbGuanLian = TRUE;
		SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)ShuangJingGuanLian);
		break;	
	default:
		break;
	}
    //yachang.huang rum on 2013-11-19
	//sys_reg->wRelay = gSysInfo.wRelay;

	LeaveCriticalSection(&csSysInfo);
	DeleteCriticalSection(&csSysInfo);

	InitParamDetail(); 
	
	// 配置DAC曲线：包括DAC使能、DAC前/后闸门（即开始位置和结束位置）以及TGC曲线
	ch1_dac_reg->wDACEnable = 0;
	ch2_dac_reg->wDACEnable = 0;

	memset(ch1_tgc, 0, 4096*sizeof(WORD));
	memset(ch2_tgc, 0, 4096*sizeof(WORD));
	
	ch1_con_reg->wValueMiddle = 511;	//ParamSet_ad.nMidValue;
	
	ch2_con_reg->wValueMiddle = 511;	//ParamSet_ad.nMidValue;

	// 打开通道
	ch1_con_reg->wEnable |= 0x1;
	ch2_con_reg->wEnable |= 0x1;

	return TRUE;
}

BOOL InitParamDetail()
{	
	// 配置发射参数：
	// 发射电压，、入射角
	SetPressLableAndExecute(NULL);

	// 发射脉宽
	ch1_con_reg->wMaiKuan = int(float(SendParam_ad[0].nMaiKuan)/20.0 +0.5);
	ch2_con_reg->wMaiKuan = int(float(SendParam_ad[1].nMaiKuan)/20.0 +0.5);

	// 重复频率
	//临时 2010-08-24 宋晨光
//	SendParam_ad[0].nRefrainFrequency = 50;
//	SendParam_ad[1].nRefrainFrequency = 50;

	// songchenguang moified start 2011-02-10
	// test
	SendParam_ad[0].nRefrainFrequency = 100;
	SendParam_ad[1].nRefrainFrequency = 100;
	// songchenguang moified end 2011-02-10

//	ch1_con_reg->wRepeatCounter = 200000 / SendParam_ad[0].nRefrainFrequency;//应该是2000 疑问待解决
//	ch2_con_reg->wRepeatCounter = 200000 / SendParam_ad[1].nRefrainFrequency;//应该是2000 疑问待解决
	// 修正
	float fTemp = SendParam_ad[0].nAmend;
	
	ch1_data_reg->wMonitorZeroCounter = WORD(MicroSecond2Dot(fTemp));//WORD(Range2Dot(DisplaySet_ad[0].nWaveSpeed, fTemp));

	fTemp = SendParam_ad[1].nAmend;
	
	ch2_data_reg->wMonitorZeroCounter = WORD(MicroSecond2Dot(fTemp));//WORD(Range2Dot(DisplaySet_ad[1].nWaveSpeed, fTemp));

	// 配置接收参数：初始增益、滤波频带、检波模式配置	
	// 延时
	fTemp = ReceiveParam_ad[0].fDelay;
	
//	ch1_data_reg->wMonitorDelayCounter = WORD(500 + Range2DotNew(DisplaySet_ad[0].nWaveSpeed, fTemp));
	ch1_data_reg->wMonitorDelayCounter = WORD(500 + MicroSecond2Dot(fTemp));

	fTemp = ReceiveParam_ad[1].fDelay;
	
//	ch2_data_reg->wMonitorDelayCounter = WORD(500 + Range2DotNew(DisplaySet_ad[1].nWaveSpeed, fTemp));
	ch2_data_reg->wMonitorDelayCounter = WORD(500 + MicroSecond2Dot(fTemp));


	// 增益
	if(!gbGuanLian)
	{
		WORD wTemp0 = 0;
		WORD wTemp1 = 0;
		WORD wTemp2 = 0;
		WORD wTemp3 = 0;

		float fTotalPlus = ReceiveParam_ad[0].fPlus + ReceiveParam_ad[0].fRefPlus;
		dBProcess(fTotalPlus, wTemp0, wTemp1, wTemp2, wTemp3);

		ch1_plus_reg->wdBRef = wTemp0;
		PlusChangeTurn(0);	

		ch1_plus_reg->wdBLevel1 = wTemp1;
		PlusChangeTurn(0);

		ch1_plus_reg->wdBLevel2 = wTemp2;
		PlusChangeTurn(0);

		ch1_plus_reg->wdBLevel3 = wTemp3;
		PlusChangeTurn(0);

		fTotalPlus = ReceiveParam_ad[1].fPlus + ReceiveParam_ad[1].fRefPlus;
		dBProcess(fTotalPlus, wTemp0, wTemp1, wTemp2, wTemp3);

		ch2_plus_reg->wdBRef = wTemp0;
		PlusChangeTurn(1);	

		ch2_plus_reg->wdBLevel1 = wTemp1;
		PlusChangeTurn(1);

		ch2_plus_reg->wdBLevel2 = wTemp2;
		PlusChangeTurn(1);

		ch2_plus_reg->wdBLevel3 = wTemp3;
		PlusChangeTurn(1);
	}
	else
	{
		DB_ITEM dB1;
		memset(&dB1, 0, sizeof(DB_ITEM));
		DB_ITEM dB2;
		memset(&dB2, 0, sizeof(DB_ITEM));

		GuanLianDBProcess(ReceiveParam_ad[0].fPlus,
						  ReceiveParam_ad[0].fRefPlus,
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

	float fBasePlus = CurrentChannelNo == 0 ? ReceiveParam_ad[0].fPlus : ReceiveParam_ad[1].fPlus;
	float fRefPlus = CurrentChannelNo == 0 ? ReceiveParam_ad[0].fRefPlus : ReceiveParam_ad[1].fRefPlus;



	memset(gszTextPlus, 0, sizeof(gszTextPlus));
	swprintf(gszTextPlus, _T("%.1f"), fBasePlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

	memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
	swprintf(gszTextRefPlus, _T("%.1f"), fRefPlus);


	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);


	//滤波频带...
	SetLvBoLableAndExecute(NULL, 0);
	Sleep(1);
	SetLvBoLableAndExecute(NULL, 1);

	InitializeCriticalSection(&csSysInfo);
	EnterCriticalSection(&csSysInfo);

	if(gSysInfo.wWorkMode == SingleChannelA)
	{
		if(ReceiveParam_ad[0].wImpedance == 0) //A高  查看继电器功能说明
		{
			gSysInfo.wRelay &= 0xFFFC;
			//yachang.huang rum on 2013-11-19
			//sys_reg->wRelay = gSysInfo.wRelay;
		}
		else if(ReceiveParam_ad[0].wImpedance == 1)//A低  查看继电器功能说明
		{
			gSysInfo.wRelay &= 0xFFFC;
			gSysInfo.wRelay |= 0x1;
			//yachang.huang rum on 2013-11-19
			//sys_reg->wRelay = gSysInfo.wRelay;
		}
	}
	else if(gSysInfo.wWorkMode == SingleChannelB)
	{
		if(ReceiveParam_ad[1].wImpedance == 2) //B高  查看继电器功能说明
		{
			gSysInfo.wRelay &= 0xFFFC;
			gSysInfo.wRelay |= 0x2;
			//yachang.huang rum on 2013-11-19
			//sys_reg->wRelay = gSysInfo.wRelay;
		}
		else if(ReceiveParam_ad[1].wImpedance == 0)//B低  查看继电器功能说明
		{
			gSysInfo.wRelay &= 0xFFFC;
			//yachang.huang rum on 2013-11-19
			//sys_reg->wRelay = gSysInfo.wRelay;
		}
	}


	// 检波模式配置(包括通道1和通道2)
	switch(ReceiveParam_ad[0].nJianBoMode)
	{
	case 0:
		gSysInfo.wRelay |= 0x1C; //ch1
		break;
	case 1:
		// ch1
		gSysInfo.wRelay |= 0x14;	
		gSysInfo.wRelay &= 0xFFF7;
		break;
	case 2:
		// ch1
		gSysInfo.wRelay |= 0x10;
		gSysInfo.wRelay &= 0xFFF3;
		break;
	case 3:
		// ch1
		gSysInfo.wRelay &= 0xFFEF;
		break;
	}

	switch(ReceiveParam_ad[1].nJianBoMode)
	{
	case 0:
		gSysInfo.wRelay |= 0xE00; // ch2;
		break;
	case 1:
		// ch2
		gSysInfo.wRelay |= 0xA00;
		gSysInfo.wRelay &= 0xFBFF;
		break;
	case 2:
		// ch2
		gSysInfo.wRelay |= 0x800;
		gSysInfo.wRelay &= 0xF9FF;
		break;
	case 3:
		// ch2
		gSysInfo.wRelay &= 0xF7FF;
		break;
	}
   //yachang.huang rum on 2013-11-19
	//sys_reg->wRelay = gSysInfo.wRelay;
/*
	// 微调
	WORD nMax500Value = 0;
	
	nMax500Value = (2<<8) + ReceiveParam_ad[0].nLvBo;
	send_max500(nMax500Value);

	Sleep(1);
	nMax500Value = (3<<8) + ReceiveParam_ad[1].nLvBo;
	send_max500(nMax500Value);
*/
	LeaveCriticalSection(&csSysInfo);
	DeleteCriticalSection(&csSysInfo);

	// 声程
	WORD wInterval[DATA_SIZE];
	memset(wInterval, 0, sizeof(wInterval));

	float fRange = DisplaySet_ad[0].fRange;
	RangeProcess(DisplaySet_ad[0].nWaveSpeed, fRange, wInterval);

	memcpy(ch1_interval, wInterval, sizeof(wInterval));

	memset(wInterval, 0, sizeof(wInterval));
	fRange = DisplaySet_ad[1].fRange;
	RangeProcess(DisplaySet_ad[1].nWaveSpeed, fRange, wInterval);
	memcpy(ch2_interval, wInterval, sizeof(wInterval));

	SetRangeLableAndExecute(NULL, DisplaySet_ad[CurrentChannelNo].fRange, TRUE);

	// 抑制
	//yachang.huang delete on 2013-12-10
	//ch1_con_reg->wSuspend = WORD((DisplaySet_ad[0].nBate/100.0f)*MAX_HEIGHT);
	//ch2_con_reg->wSuspend = WORD((DisplaySet_ad[1].nBate/100.0f)*MAX_HEIGHT);

	// 闸门设置
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<3; j++)
		{
			WORD nFront = (WORD)Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fFrontGate);
			WORD nBack = (WORD)Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fBackGate);
			WORD nRange = nBack - nFront;
			if(i==0)
			{			
				if(j == 0)
				{
					// 报警模式
					ch1_gate_reg->wGate1Alarm = GateInfos[i][j].alarmMode;
					// 开启标志
					ch1_gate_reg->wGate1Enable = GateInfos[i][j].bOpen;						
					// 前沿
					ch1_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fFrontGate));		
		
					// 后沿
					ch1_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fBackGate));
					
					// 峰沿
					//SetGateCurve(ch1_gate1curve, nRange, GateInfos[i][j].fPeakGate);
					ch1_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * GateInfos[i][j].fPeakGate / 100.0f);
		
				}
				else if(j == 1)
				{
					ch1_gate_reg->wGate2Alarm = GateInfos[i][j].alarmMode;
					ch1_gate_reg->wGate2Enable = GateInfos[i][j].bOpen;
					ch1_gate_reg->wGate2Front = WORD(Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fFrontGate));
					ch1_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fBackGate));
					//SetGateCurve(ch1_gate2curve, nRange, GateInfos[i][j].fPeakGate);
					ch1_gate_reg->wGate2Peak = WORD(MAX_HEIGHT * GateInfos[i][j].fPeakGate / 100.0f);
				}
				else if(j == 2)
				{
					ch1_gate_reg->wGate3Alarm = GateInfos[i][j].alarmMode;
					ch1_gate_reg->wGate3Enable = GateInfos[i][j].bOpen;
					ch1_gate_reg->wGate3Front = WORD(Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fFrontGate));
					ch1_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fBackGate));
				//	SetGateCurve(ch1_gate3curve, nRange, GateInfos[i][j].fPeakGate);
				}					
			}
			else 
			{
				if(j == 0)
				{
					// 报警模式
					ch2_gate_reg->wGate1Alarm = GateInfos[i][j].alarmMode;
					// 开启标志
					ch2_gate_reg->wGate1Enable = GateInfos[i][j].bOpen;						
					// 前沿
					ch2_gate_reg->wGate1Front = WORD(Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fFrontGate));		
		
					// 后沿
					ch2_gate_reg->wGate1Rear = WORD(Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fBackGate));
					
					// 峰沿
					//SetGateCurve(ch2_gate1curve, nRange, GateInfos[i][j].fPeakGate);
					ch2_gate_reg->wGate1Peak = WORD(MAX_HEIGHT * GateInfos[i][j].fPeakGate / 100.0f);
		
				}
				else if(j == 1)
				{
					ch2_gate_reg->wGate2Alarm = GateInfos[i][j].alarmMode;
					ch2_gate_reg->wGate2Enable = GateInfos[i][j].bOpen;
					ch2_gate_reg->wGate2Front = WORD(Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fFrontGate));
					ch2_gate_reg->wGate2Rear = WORD(Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fBackGate));
					//SetGateCurve(ch2_gate2curve, nRange, GateInfos[i][j].fPeakGate);
					ch2_gate_reg->wGate2Peak = WORD(MAX_HEIGHT * GateInfos[i][j].fPeakGate / 100.0f);
				}
				else if(j == 2)
				{
					ch2_gate_reg->wGate3Alarm = GateInfos[i][j].alarmMode;
					ch2_gate_reg->wGate3Enable = GateInfos[i][j].bOpen;
					ch2_gate_reg->wGate3Front = WORD(Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fFrontGate));
					ch2_gate_reg->wGate3Rear = WORD(Range2Dot(DisplaySet_ad[i].nWaveSpeed, GateInfos[i][j].fBackGate));
				//	SetGateCurve(ch2_gate3curve, nRange, GateInfos[i][j].fPeakGate);
				}
			}
		}
	}
	//wangbingfu add at 2011-4-20
	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)(WORD)gSysInfo.wWorkMode);

	// 通道
	memset(gszTextChannel, 0, sizeof(gszTextChannel));
	
	if(gSysInfo.wWorkMode != DanJingGuanLian && gSysInfo.wWorkMode != ShuangJingGuanLian)
	{
			swprintf(gszTextChannel, _T("%d"), CurrentChannelNo+1);
	}

	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
	return TRUE;	
}

/** 
 * 增益值改变后开关处理
 * @return    void
 * @since     1.00
 */
void PlusChangeTurn(const WORD& chNo)
{
	if(chNo == 0)
	{
		ch1_plus_reg->wdBControl = 0;
		ch1_plus_reg->wdBControl = 1;
		Sleep(1);
		ch1_plus_reg->wdBControl = 0;
	}
	else if(chNo == 1)
	{
		ch2_plus_reg->wdBControl = 0;
		ch2_plus_reg->wdBControl = 1;
		Sleep(1);
		ch2_plus_reg->wdBControl = 0;
	}	
}

void CopyBmp(HDC hDC, const WORD& wBmpID, const RECT& rect)
{
	HDC hSrcDC = CreateCompatibleDC(hDC);
	HBITMAP hBmp = LoadBitmap( hRes, MAKEINTRESOURCE(wBmpID));
	
	HBITMAP hOldBmp = (HBITMAP)SelectObject(hSrcDC, hBmp);
	StretchBlt(hDC,rect.left, rect.top, (rect.right - rect.left),
		(rect.bottom -  rect.top), hSrcDC, 0,0, 
		(rect.right - rect.left), (rect.bottom -  rect.top), SRCCOPY);
	SelectObject(hSrcDC, hOldBmp);
	DeleteObject(hBmp);
	DeleteDC(hSrcDC);
}
/** 
 * creates a status bar and divides it into
 * the specified number of parts.
 * @param     hwndParent [in]: parent window for the status bar.
 * @param     nStatusID [in]: child window identifier.
 * @param     hinst [in]: handle to the application instance.
 * @param     nParts [in]: number of parts into which to divide the status bar.
 * @return    HWND: the handle to the status bar.
 * @since     1.00
 */
HWND DoCreateStatusBar(HWND hwndParent, int nStatusID,
					   HINSTANCE hinst, int nParts)
{
	HWND hwndStatus;
	HLOCAL hloc;
	LPINT lpParts;
	
	// Ensure that the common control DLL is loaded.
	InitCommonControls();
	// Create the status bar.
	hwndStatus = CreateStatusWindow( WS_CHILD | WS_VISIBLE | CCS_TOP, 
		                _T(""),
						hwndParent, 
						nStatusID);

	// Allocate an array for holding the right edge coordinates.
	hloc = LocalAlloc(LHND, sizeof(int) * nParts);
	lpParts = (LPINT)LocalLock(hloc);
	// Calculate the right edge coordinate for each part, and
	// copy the coordinates to the array.
	lpParts[0] = 72;
	lpParts[1] = 112;
	lpParts[2] = 232;
	lpParts[3] = 368;
	lpParts[4] = 448;
	lpParts[5] = 572;
	lpParts[6] = 640;
	// Tell the status bar to create the window parts.
	SendMessage(hwndStatus, SB_SETPARTS, (WPARAM) nParts,
	(LPARAM) lpParts);

	SendMessage(hwndStatus, SB_SETMINHEIGHT, 
		(WPARAM) STATUSBAR_HEIGHT, 0);

	SendMessage(hwndStatus, WM_SIZE, 0, 0);

	memset(gszTextChannel, 0, sizeof(gszTextChannel));
	memset(gszTextPlus, 0, sizeof(gszTextPlus));
	memset(gszTextTime, 0, sizeof(gszTextTime));

	// Free the array, and return.
	LocalUnlock(hloc);
	LocalFree(hloc);
	return hwndStatus;
}

/** 
 * 修改菜单显示标志
 * @param     bValue [in]: Flag Value.
 * @return    void: 
 * @since     1.00
 */
void SetMenuStateFlag(BOOL bValue)
{
	InitializeCriticalSection(&csMenuState);
	EnterCriticalSection(&csMenuState);
	g_bMenuShown = bValue;
	LeaveCriticalSection(&csMenuState);
	DeleteCriticalSection(&csMenuState);
}

/** 
 * 修改重画标志
 * @param     bValue [in]: Flag Value.
 * @return    void: 
 * @since     1.00
 */
void SetRedrawFlag(BOOL bValue)
{
	InitializeCriticalSection(&csRedraw);
	EnterCriticalSection(&csRedraw);
	g_bRedraw = bValue;
	LeaveCriticalSection(&csRedraw);
	DeleteCriticalSection(&csRedraw);
}

/** 
 * 画线
 * the specified number of parts.
 * @param     xStart [in]: 起点X.
 * @param     yStart [in]: 起点Y.
 * @param     xEnd   [in]: 终点X.
 * @param     yEnd   [in]: 终点Y.
 * @param     nColor [in]: 颜色值.
 * @return    void: 
 * @since     1.00
 */
void Line(const int& xStart, const int& yStart, const int& xEnd, const int& yEnd, const WORD& nColor)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;

	WORD *pPixel = NULL;
	if(yStart < yEnd)	// 上升曲线
	{
		for(int i=xStart; i<xEnd; i++)
		{
			pPixel = pBaseAddress + yStart*LCD_XSIZE_TFT + i;
			*pPixel = nColor;
		}

		for(int j = yStart; j<yEnd; j++)
		{
			pPixel = pBaseAddress + j*LCD_XSIZE_TFT + xEnd;
			*pPixel = nColor;
		}
	}
	else	// 下降曲线
	{
		for(int j = yStart; j>yEnd; j--)
		{
			pPixel = pBaseAddress + j*LCD_XSIZE_TFT + xStart;
			*pPixel = nColor;
		}

		for(int i=xStart; i<xEnd; i++)
		{
			pPixel = pBaseAddress + yEnd*LCD_XSIZE_TFT + i;
			*pPixel = nColor;
		}
	}
	*/
}

/** 
 * 画三倍粗线
 * the specified number of parts.
 * @param     xStart [in]: 起点X.
 * @param     yStart [in]: 起点Y.
 * @param     xEnd   [in]: 终点X.
 * @param     yEnd   [in]: 终点Y.
 * @param     nColor [in]: 颜色值.
 * @return    void: 
 * @since     1.00
 */
void Line3(const int& xStart, const int& yStart, const int& xEnd, const int& yEnd, const WORD& nColor)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;

	WORD *pPixel = NULL;
	if(yStart < yEnd)	// 上升曲线
	{
		for(int i=xStart; i<xEnd; i++)
		{
			pPixel = pBaseAddress + (yStart-1)*LCD_XSIZE_TFT + i;
			*pPixel = nColor;
			pPixel = pBaseAddress + yStart*LCD_XSIZE_TFT + i;
			*pPixel = nColor;
			pPixel = pBaseAddress + (yStart+1)*LCD_XSIZE_TFT + i;
			*pPixel = nColor;
		}

		for(int j = yStart; j<yEnd; j++)
		{
			pPixel = pBaseAddress + (j-1)*LCD_XSIZE_TFT + xEnd;
			*pPixel = nColor;
			pPixel = pBaseAddress + j*LCD_XSIZE_TFT + xEnd;
			*pPixel = nColor;
			pPixel = pBaseAddress + (j+1)*LCD_XSIZE_TFT + xEnd;
			*pPixel = nColor;
		}
	}
	else	// 下降曲线
	{
		for(int j = yStart; j>yEnd; j--)
		{
			pPixel = pBaseAddress + (j-1)*LCD_XSIZE_TFT + xStart;
			*pPixel = nColor;
			pPixel = pBaseAddress + j*LCD_XSIZE_TFT + xStart;
			*pPixel = nColor;
			pPixel = pBaseAddress + (j+1)*LCD_XSIZE_TFT + xStart;
			*pPixel = nColor;
		}

		for(int i=xStart; i<xEnd; i++)
		{
			pPixel = pBaseAddress + (yEnd-1)*LCD_XSIZE_TFT + i;
			*pPixel = nColor;
			pPixel = pBaseAddress + yEnd*LCD_XSIZE_TFT + i;
			*pPixel = nColor;
			pPixel = pBaseAddress + (yEnd+1)*LCD_XSIZE_TFT + i;
			*pPixel = nColor;
		}
	}
	*/
}
/** 
 * 擦掉三倍粗线
 * the specified number of parts.
 * @param     xStart [in]: 起点X.
 * @param     yStart [in]: 起点Y.
 * @param     xEnd   [in]: 终点X.
 * @param     yEnd   [in]: 终点Y.
 * @param     nColor [in]: 颜色值.
 * @return    void: 
 * @since     1.00
 */

void EraseLine3(const int& xStart, const int& yStart, const int& xEnd, const int& yEnd, const WORD& nBackColor, const WORD& nLineColor)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;

	WORD *pPixel = NULL;
	if(yStart < yEnd)	// 上升曲线
	{
		for(int i=xStart; i<xEnd; i++)
		{
			pPixel = pBaseAddress + (yStart-1)*LCD_XSIZE_TFT + i;
			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
			pPixel = pBaseAddress + yStart*LCD_XSIZE_TFT + i;
			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
			pPixel = pBaseAddress + (yStart+1)*LCD_XSIZE_TFT + i;
			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
		}

		for(int j = yStart; j<yEnd; j++)
		{
			pPixel = pBaseAddress + (j-1)*LCD_XSIZE_TFT + xEnd;
			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
			pPixel = pBaseAddress + j*LCD_XSIZE_TFT + xEnd;
			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
			pPixel = pBaseAddress + (j+1)*LCD_XSIZE_TFT + xEnd;
			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
		}
	}
	else	// 下降曲线
	{
		for(int j = yStart; j>yEnd; j--)
		{
			pPixel = pBaseAddress + (j-1)*LCD_XSIZE_TFT + xStart;
			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
			pPixel = pBaseAddress + j*LCD_XSIZE_TFT + xStart;
			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
			pPixel = pBaseAddress + (j+1)*LCD_XSIZE_TFT + xStart;
			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
		}

		for(int i=xStart; i<xEnd; i++)
		{
			pPixel = pBaseAddress + (yEnd-1)*LCD_XSIZE_TFT + i;
			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
			pPixel = pBaseAddress + yEnd*LCD_XSIZE_TFT + i;
			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
			pPixel = pBaseAddress + (yEnd+1)*LCD_XSIZE_TFT + i;
			if(*pPixel != nLineColor)
				*pPixel = nBackColor;
		}
	}
	*/
}

/** 
 * 画虚线
 * the specified number of parts.
 * @param     xStart [in]: 起点X.
 * @param     yStart [in]: 起点Y.
 * @param     xEnd   [in]: 终点X.
 * @param     yEnd   [in]: 终点Y.
 * @param     nColor [in]: 颜色值.
 * @return    void: 
 * @since     1.00
 */
void DashLine(const int& xStart, const int& yStart, const int& xEnd, const int& yEnd, const WORD& nColor, const WORD& nColorBk)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;

	WORD *pPixel = NULL;
	if(yStart < yEnd)	// 上升曲线
	{
		int k = 0;
		for(int i=xStart; i<xEnd; i++)
		{
			pPixel = pBaseAddress + yStart*LCD_XSIZE_TFT + i;
			if(k < 7)
				*pPixel = nColor;
			else
				*pPixel = nColorBk;

			k++;
			if(k>10)
				k = 0;
		}

		k = 0;
		for(int j = yStart; j<yEnd; j++)
		{
			pPixel = pBaseAddress + j*LCD_XSIZE_TFT + xEnd;
			if(k < 7)
				*pPixel = nColor;
			else
				*pPixel = nColorBk;

			k++;
			if(k>10)
				k = 0;
		}
	}
	else	// 下降曲线
	{
		int k = 0;
		for(int j = yStart; j>yEnd; j--)
		{
			pPixel = pBaseAddress + j*LCD_XSIZE_TFT + xStart;
			if(k < 7)
				*pPixel = nColor;
			else
				*pPixel = nColorBk;

			k++;
			if(k>10)
				k = 0;
		}

		k = 0;
		for(int i=xStart; i<xEnd; i++)
		{
			pPixel = pBaseAddress + yEnd*LCD_XSIZE_TFT + i;
			if(k < 7)
				*pPixel = nColor;
			else
				*pPixel = nColorBk;

			k++;
			if(k>10)
				k = 0;
		}
	}
	*/
}

/** 
 * 获取工作模式图片句柄
 * @param     nMode [in]: 工作模式.
 * @return    HBITMAP: 模式图片句柄
 * @since     1.00
 */
HBITMAP GetModeBmp(const WORKMODE& nMode)
{
	HBITMAP hBmp = NULL;

	if(nMode == SingleChannelA || nMode == SingleChannelB)
		hBmp = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_SINGLE_CHANNEL_R));
	else if(nMode == DoubleChannels)
		hBmp = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_DOUBLE_CHANNEL_R));
	else if(nMode == OneSendOneReceive)
		hBmp = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_YIFAYISHOU_R));
	else if(nMode == DanJingGuanLian)
		hBmp = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_DANJING_R));
	else if(nMode == ShuangJingGuanLian)
		hBmp = LoadBitmap( hRes, MAKEINTRESOURCE(IDB_BITMAP_SHUANGJING_R));

	return hBmp;
}

/** 
 * dB转换为硬件增益对应的A,B,C,D值
 * @param     db [in]:		   增益.
 * @param     db_0_data [out]: 增益A.
 * @param     db_1_data [out]: 增益B.
 * @param     db_2_data [out]: 增益C.
 * @param     db_3_data [out]: 增益D.
 * @return    void: 
 * @since     1.00
 */

void GuanLianDBProcess(const float& fBaseDB, 
					   const float& fRefDB,
					   DB_ITEM& dbCh1,
					   DB_ITEM& dbCh2)
{
	float fTotal = fBaseDB + fRefDB;
	if(fTotal < 0)
		return;

	float fDbBaseChannel = fBaseDB * YIQI_ADJUST; 
    float db_in = fDbBaseChannel - 10.0;

	float db_1 = 0.0f;
	float db_2 = 0.0f;
	float db_3 = 0.0f;

	if(db_in < -10)
	{	
		db_1 = -10;
		db_2 = 1;
		db_3 = 1;
	}
	else if(db_in < 3)
	{	
		db_1 = db_in-2;
		db_2 = 1;
		db_3 = 1;
	}
	else if(db_in < 107)
	{
		db_1 = db_in/3.0;
		db_2 = db_in/3.0;
		db_3 = db_in/3.0;
	}
	else
	{	
		db_1 = 30;
		db_2 = 39;
		db_3 = 39;
	}

    dbCh1.wDB_A = 128;
	dbCh1.wDB_B = WORD((db_1 + 14.0) * 16.0 / 3.0);
	dbCh1.wDB_C = WORD(20.0 * (db_2 - 0.8) / 3.0);
	dbCh1.wDB_D = WORD(20.0 * (db_3 - 0.8) / 3.0);

    float fDbRefChannel = fBaseDB + fRefDB - db_1;
	float db_in_2 = fDbRefChannel - 10.0;

	float db_1_2 = db_1;
	float db_2_2 = db_2 + fRefDB/2*YIQI_ADJUST;
	float db_3_2 = db_3 + fRefDB/2*YIQI_ADJUST;

	dbCh2.wDB_A= 128;
	dbCh2.wDB_B= WORD((db_1_2 + 14.0) * 16.0 / 3.0);
	dbCh2.wDB_C= WORD(20.0 * (db_2_2 - 0.8) / 3.0);
	dbCh2.wDB_D= WORD(20.0 * (db_3_2 - 0.8) / 3.0);
}

void dBProcess(const float& db, 
			   WORD& db_0_data,
			   WORD& db_1_data,
			   WORD& db_2_data,
			   WORD& db_3_data)
{
	if(db < 0)
		return;

	float db_in = db*YIQI_ADJUST - 10.0;
	float db_1 = 0.0f;
	float db_2 = 0.0f;
	float db_3 = 0.0f;

	if(db_in < -10)
	{	
		db_1 = -10;
		db_2 = 1;
		db_3 = 1;
	}
	else if(db_in < 3)
	{	
		db_1 = db_in-2;
		db_2 = 1;
		db_3 = 1;
	}
	else if(db_in < 107)
	{
		db_1 = db_in/3.0;
		db_2 = db_in/3.0; 
		db_3 = db_in/3.0; 
	}
	else
	{	
		db_1 = 30;
		db_2 = 39;
		db_3 = 39;
	}

    db_0_data = 128;
	db_1_data = WORD((db_1 + 14.0) * 16.0 / 3.0);
	db_2_data = WORD(20.0 * (db_2 - 0.8) / 3.0);
	db_3_data = WORD(20.0 * (db_3 - 0.8) / 3.0);
	   
}

WORD DACdBProcess(const float& db)
{	
	if(db<0)
		return 0;

	//WORD db_data = WORD(20.0 * db*YIQI_ADJUST / 3.0);
	WORD db_data = WORD(db / 40 * 1023);

	return db_data;
}

int MicroSecond2Dot(const float& fMicroSecond)
{
	int ret = fMicroSecond * 1 * ADFREQUENCY;

	if(ret == 0)
		ret = 1;
	return ret;
}
/*曲线随增益改变*/
void gainChangeCurve(float fOldGain, BOOL bSave)
{
	BOOL bOldWaveDrawDacFlag = gWaveDrawDacFlag;	//保存波形是否画DAC标志
	BOOL bOldWaveDrawAvgFlag = gWaveDrawAvgFlag;	//保存波形是否画AVG标志
	
	if(fOldGain - ReceiveParam_ad[CurrentChannelNo].fPlus != 0)
	{
		if(gDgsSwitch[CurrentChannelNo][0])
		{
			gDacChangeFlag = TRUE;//停止画线
			Sleep(30);//等待停止画线
		/*	if(CurrentChannelNo == 0)
			{
				for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
				{
					drawCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
				}
			}
			else
			{
				for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
				{
					drawDashCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
				}
			}*/
			for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
			{
				drawCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
			}
			
		}
		if(bOldWaveDrawDacFlag)//正在制作DAC曲线时
		{
			//擦掉制作线
			gWaveDrawDacFlag = FALSE;
			Sleep(30);//等待停止画线
			POINT stDgsPoint[MAX_CURVE_POINT_NUM];
			for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
			{
				stDgsPoint[i] = dataToPoint(gSelectedDgsPointData[CurrentChannelNo][3][i]);
				drawShiZhi1(stDgsPoint[i], BLUE_POINT, FALSE);//画点
			}
			
			drawCurve2(gSelectedDgsPointData[CurrentChannelNo][3], gSelectedDgsPointDataNum[CurrentChannelNo][3], RED_POINT, FALSE);

			gfDacFirstPointChangeGain[CurrentChannelNo] = gfDacFirstPointChangeGain[CurrentChannelNo] + (fOldGain - ReceiveParam_ad[CurrentChannelNo].fPlus);
		}
		else if(bOldWaveDrawAvgFlag)//正在制作AVG曲线时
		{
			//擦掉制作线
			gWaveDrawAvgFlag = FALSE;
			Sleep(30);//等待停止画线
			POINT stAvgPoint[MAX_CURVE_POINT_NUM];
			for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
			{
				stAvgPoint[i] = dataToPoint(gSelectedAvgPointData[CurrentChannelNo][3][i]);
				drawShiZhi1(stAvgPoint[i], BLUE_POINT, FALSE);//画点
			}
			
			drawCurve2(gSelectedAvgPointData[CurrentChannelNo][3], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);

			gfAvgFirstPointChangeGain[CurrentChannelNo] = gfAvgFirstPointChangeGain[CurrentChannelNo] + (fOldGain - ReceiveParam_ad[CurrentChannelNo].fPlus);
		}

		if(!gbGuanLian) //非关联
		{
			for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
			{
				gSelectedDgsPointData[CurrentChannelNo][0][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][0][i].data, (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));

				gSelectedDgsPointData[CurrentChannelNo][1][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][1][i].data, (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));
				
				gSelectedDgsPointData[CurrentChannelNo][2][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][2][i].data, (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));

				gSelectedDgsPointData[CurrentChannelNo][3][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));
			}
		}
		else
		{
			for(int k = 0; k < CHANNEL_NUM; k++)
			{
				for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
				{
					gSelectedDgsPointData[k][0][i].data = curveDataChangeByGain(gSelectedDgsPointData[k][0][i].data , (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));

					gSelectedDgsPointData[k][1][i].data = curveDataChangeByGain(gSelectedDgsPointData[k][1][i].data , (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));
					
					gSelectedDgsPointData[k][2][i].data = curveDataChangeByGain(gSelectedDgsPointData[k][2][i].data , (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));

					gSelectedDgsPointData[k][3][i].data = curveDataChangeByGain(gSelectedDgsPointData[k][3][i].data , (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));
				}
			}
		}

		if(gDgsSwitch[CurrentChannelNo][0])
			gDacChangeFlag = FALSE;//dac曲线需要重画

		//AVG
		if(gbAvgCurveFlag[CurrentChannelNo] == ON)
		{
			bgAvgChangeFlag = TRUE;//停止画线
			Sleep(30);//等待停止画线 通道2 跟关联模式会有残影
			/*if(CurrentChannelNo == 0)
			{
				for(int i = 0; i < AVG_CURVE_NUM; i++)
				{
					drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo],RED_POINT, FALSE);
				}
			}
			else
			{
				for(int i = 0; i < AVG_CURVE_NUM; i++)
				{
					drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo],RED_POINT, FALSE);
				}
			}*/
			for(int i = 0; i < AVG_CURVE_NUM; i++)
			{
				drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo],RED_POINT, FALSE);
			}
			HDC hdc = GetDC(hWndMain);
			WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
			HBRUSH hbr = CreateSolidBrush(wColorBack);
			
			RECT rect;
			GetClipBox(hdc, &rect);
			
			RECT RctWave;// 波形区域
			RctWave.left = rect.left + SCALE_WIDTH;
			RctWave.right = rect.right - SCALE_WIDTH;
			RctWave.top = rect.top;
			RctWave.bottom = rect.bottom - SCALE_WIDTH;
			
			RECT stTextRect;
			stTextRect.left = RctWave.left + 2;
			stTextRect.right = RctWave.left + 60;
			for(int i = 0; i < AVG_CURVE_NUM; i++)
			{
				POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
				stTextRect.top = stFirstPoint.y - 25;
				stTextRect.bottom = stFirstPoint.y;
			
				FillRect(hdc, &stTextRect, hbr);
			}
			DeleteObject(hbr);
			ReleaseDC(hWndMain,hdc);

		}

		if(!gbGuanLian) //非关联
		{
			for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
			{
				gSelectedAvgPointData[CurrentChannelNo][0][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][0][i].data , (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));

				gSelectedAvgPointData[CurrentChannelNo][1][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][1][i].data , (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));
				
				gSelectedAvgPointData[CurrentChannelNo][2][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][2][i].data , (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));

				gSelectedAvgPointData[CurrentChannelNo][3][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data , (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));
			}
		}
		else
		{
			for(int k = 0; k < CHANNEL_NUM; k++)
			{
				for(int i = 0; i < gSelectedAvgPointDataNum[k]; i++)
				{
					gSelectedAvgPointData[k][0][i].data = curveDataChangeByGain(gSelectedAvgPointData[k][0][i].data  , (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));

					gSelectedAvgPointData[k][1][i].data = curveDataChangeByGain(gSelectedAvgPointData[k][1][i].data  , (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));
					
					gSelectedAvgPointData[k][2][i].data = curveDataChangeByGain(gSelectedAvgPointData[k][2][i].data  , (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));

					gSelectedAvgPointData[k][3][i].data = curveDataChangeByGain(gSelectedAvgPointData[k][3][i].data  , (ReceiveParam_ad[CurrentChannelNo].fPlus - fOldGain));
				}
			}
		}

		if(gbAvgCurveFlag[CurrentChannelNo] == ON)
			bgAvgChangeFlag = FALSE;//AVG曲线需要重画

		if(bOldWaveDrawDacFlag)//正在制作曲线时
			gWaveDrawDacFlag = TRUE;//重画制作线
		else if(bOldWaveDrawAvgFlag)//正在制作AVG曲线时
			gWaveDrawAvgFlag = TRUE;//重画制作线
		// 保存配置到默认文件
		if(bSave)
			SaveParamFile(lpDefaultParamFile);				
	}
}
/*曲线随参考增益改变*/
void refGainChangeCurve(float fOldRefGain, BOOL bSave)
{
	BOOL bOldWaveDrawDacFlag = gWaveDrawDacFlag;	//保存波形是否画DAC标志
	BOOL bOldWaveDrawAvgFlag = gWaveDrawAvgFlag;	//保存波形是否画AVG标志
	
	if((fOldRefGain - ReceiveParam_ad[CurrentChannelNo].fRefPlus != 0) 
		&& (sys_reg->wWorkMode == DanJingGuanLian || sys_reg->wWorkMode == ShuangJingGuanLian))//单双晶关联且不在制作曲线
	{
		if(gDgsSwitch[CurrentChannelNo][0])
		{
			gDacChangeFlag = TRUE;//停止画线
			Sleep(30);//等待停止画线
		/*	if(CurrentChannelNo == 0)
			{
				for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
				{
					drawCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
				}
			}
			else
			{
				for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
				{
					drawDashCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
				}
			}*/
			for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
			{
				drawCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
			}
			
		}
	/*	if(bOldWaveDrawDacFlag)//正在制作DAC曲线时
		{
			//擦掉制作线x
			gWaveDrawDacFlag = FALSE;
			Sleep(30);//等待停止画线
			POINT stDgsPoint[MAX_CURVE_POINT_NUM];
			for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
			{
				stDgsPoint[i] = dataToPoint(gSelectedDgsPointData[CurrentChannelNo][3][i]);
				drawShiZhi1(stDgsPoint[i], BLUE_POINT, FALSE);//画点
			}
			
			drawCurve2(gSelectedDgsPointData[CurrentChannelNo][3], gSelectedDgsPointDataNum[CurrentChannelNo][3], RED_POINT, FALSE);

			gfDacFirstPointChangeGain[CurrentChannelNo] = gfDacFirstPointChangeGain[CurrentChannelNo] + (fOldRefGain - ReceiveParam_ad[CurrentChannelNo].fRefPlus);
		}
		else if(bOldWaveDrawAvgFlag)//正在制作AVG曲线时
		{
			//擦掉制作线
			gWaveDrawAvgFlag = FALSE;
			Sleep(30);//等待停止画线
			POINT stAvgPoint[MAX_CURVE_POINT_NUM];
			for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
			{
				stAvgPoint[i] = dataToPoint(gSelectedAvgPointData[CurrentChannelNo][3][i]);
				drawShiZhi1(stAvgPoint[i], BLUE_POINT, FALSE);//画点
			}
			
			drawCurve2(gSelectedAvgPointData[CurrentChannelNo][3], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);

			gfAvgFirstPointChangeGain[CurrentChannelNo] = gfAvgFirstPointChangeGain[CurrentChannelNo] + (fOldRefGain - ReceiveParam_ad[CurrentChannelNo].fRefPlus);
		}*/
		if(!bOldWaveDrawDacFlag)//不在制作曲线
		{
			for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
			{
				gSelectedDgsPointData[CurrentChannelNo][0][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][0][i].data, (ReceiveParam_ad[CurrentChannelNo].fRefPlus - fOldRefGain));

				gSelectedDgsPointData[CurrentChannelNo][1][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][1][i].data, (ReceiveParam_ad[CurrentChannelNo].fRefPlus - fOldRefGain));
				
				gSelectedDgsPointData[CurrentChannelNo][2][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][2][i].data, (ReceiveParam_ad[CurrentChannelNo].fRefPlus - fOldRefGain));

				gSelectedDgsPointData[CurrentChannelNo][3][i].data = curveDataChangeByGain(gSelectedDgsPointData[CurrentChannelNo][3][i].data, (ReceiveParam_ad[CurrentChannelNo].fRefPlus - fOldRefGain));
			}
		}

		if(gDgsSwitch[CurrentChannelNo][0])
			gDacChangeFlag = FALSE;//dac曲线需要重画

		//AVG
		if(gbAvgCurveFlag[CurrentChannelNo] == ON)
		{
			bgAvgChangeFlag = TRUE;//停止画线
			Sleep(30);//等待停止画线
			/*if(CurrentChannelNo == 0)
			{
				for(int i = 0; i < AVG_CURVE_NUM; i++)
				{
					drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo],RED_POINT, FALSE);
				}
			}
			else
			{
				for(int i = 0; i < AVG_CURVE_NUM; i++)
				{
					drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo],RED_POINT, FALSE);
				}
			}*/
			if(CurrentChannelNo == 0)
			{
				for(int i = 0; i < AVG_CURVE_NUM; i++)
				{
					drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo],RED_POINT, FALSE);
				}
			}
			HDC hdc = GetDC(hWndMain);
			WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
			HBRUSH hbr = CreateSolidBrush(wColorBack);
			
			RECT rect;
			GetClipBox(hdc, &rect);
			
			RECT RctWave;// 波形区域
			RctWave.left = rect.left + SCALE_WIDTH;
			RctWave.right = rect.right - SCALE_WIDTH;
			RctWave.top = rect.top;
			RctWave.bottom = rect.bottom - SCALE_WIDTH;
			
			RECT stTextRect;
			stTextRect.left = RctWave.left + 2;
			stTextRect.right = RctWave.left + 60;
			for(int i = 0; i < AVG_CURVE_NUM; i++)
			{
				POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
				stTextRect.top = stFirstPoint.y - 25;
				stTextRect.bottom = stFirstPoint.y;
			
				FillRect(hdc, &stTextRect, hbr);
			}
			DeleteObject(hbr);
			ReleaseDC(hWndMain,hdc);

		}
		if(!bOldWaveDrawAvgFlag)//不在制作曲线
		{
			for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
			{
				gSelectedAvgPointData[CurrentChannelNo][0][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][0][i].data , (ReceiveParam_ad[CurrentChannelNo].fRefPlus - fOldRefGain));

				gSelectedAvgPointData[CurrentChannelNo][1][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][1][i].data , (ReceiveParam_ad[CurrentChannelNo].fRefPlus - fOldRefGain));
				
				gSelectedAvgPointData[CurrentChannelNo][2][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][2][i].data , (ReceiveParam_ad[CurrentChannelNo].fRefPlus - fOldRefGain));

				gSelectedAvgPointData[CurrentChannelNo][3][i].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][3][i].data , (ReceiveParam_ad[CurrentChannelNo].fRefPlus - fOldRefGain));
			}
		}

		if(gbAvgCurveFlag[CurrentChannelNo] == ON)
			bgAvgChangeFlag = FALSE;//AVG曲线需要重画

	/*	if(bOldWaveDrawDacFlag)//正在制作曲线时
			gWaveDrawDacFlag = TRUE;//重画制作线
		else if(bOldWaveDrawAvgFlag)//正在制作AVG曲线时
			gWaveDrawAvgFlag = TRUE;//重画制作线
	*/
		// 保存配置到默认文件
		if(bSave)
			SaveParamFile(lpDefaultParamFile);				
	}
}

/*曲线随声程改变*/
void rangeChangeCurve(float fOldRange, BOOL bSave)
{	
	BOOL bOldWaveDrawDacFlag = gWaveDrawDacFlag;	//保存波形是否画DAC标志
	BOOL bOldWaveDrawAvgFlag = gWaveDrawAvgFlag;	//保存波形是否画AVG标志
	BOOL bOldWaveDrawTcgFlag = gWaveDrawTcgFlag;	//保存是否画TCG标志
	if(fOldRange - DisplaySet_ad[CurrentChannelNo].fRange != 0)
	{
		if(gDgsSwitch[CurrentChannelNo][0])
		{
			gDacChangeFlag = TRUE;//停止画线
			Sleep(10);//等待停止画线
			for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
			{
				drawCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
			}
		}
		if(bOldWaveDrawDacFlag)//正在制作DAC曲线时
		{
			//擦掉制作线
			gWaveDrawDacFlag = FALSE;
			Sleep(30);//等待停止画线
			POINT stDgsPoint[MAX_CURVE_POINT_NUM];
			for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
			{
				stDgsPoint[i] = dataToPoint(gSelectedDgsPointData[CurrentChannelNo][3][i]);
				drawShiZhi1(stDgsPoint[i], BLUE_POINT, FALSE);//画点
			}
			
			drawCurve2(gSelectedDgsPointData[CurrentChannelNo][3], gSelectedDgsPointDataNum[CurrentChannelNo][3], RED_POINT, FALSE);
		}
		else if(bOldWaveDrawAvgFlag)//正在制作AVG曲线时
		{
			//擦掉制作线
			gWaveDrawAvgFlag = FALSE;
			Sleep(30);//等待停止画线
			POINT stAvgPoint[MAX_CURVE_POINT_NUM];
			for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
			{
				stAvgPoint[i] = dataToPoint(gSelectedAvgPointData[CurrentChannelNo][3][i]);
				drawShiZhi1(stAvgPoint[i], BLUE_POINT, FALSE);//画点
			}
			
			drawCurve2(gSelectedAvgPointData[CurrentChannelNo][3], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
		}
		else if(bOldWaveDrawTcgFlag)//正在制作DAC曲线时
		{
			//擦掉制作线
			gWaveDrawTcgFlag = FALSE;
			Sleep(30);//等待停止画线
		}
		else
		{
			;//
		}
		
		if(!gbGuanLian) //非关联
		{
			for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
			{
				gSelectedDgsPointData[CurrentChannelNo][0][i].serial = (short)(gwOldDacSerial[CurrentChannelNo][i] 
																				* gfOldDacRange[CurrentChannelNo] / DisplaySet_ad[CurrentChannelNo].fRange);
			//	gSelectedDgsPointData[CurrentChannelNo][0][i].serial = curveChangeByDelay(gSelectedDgsPointData[CurrentChannelNo][0][i].serial, 
			//																					fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
	
				gSelectedDgsPointData[CurrentChannelNo][1][i].serial = gSelectedDgsPointData[CurrentChannelNo][0][i].serial;
	
				gSelectedDgsPointData[CurrentChannelNo][2][i].serial = gSelectedDgsPointData[CurrentChannelNo][0][i].serial;
				
				gSelectedDgsPointData[CurrentChannelNo][3][i].serial = gSelectedDgsPointData[CurrentChannelNo][0][i].serial;
	
			 } 				
			if(gDgsSwitch[CurrentChannelNo][0])
				gDacChangeFlag = FALSE;//dac曲线需要重画
		}
		else
		{
			for(int k = 0; k < CHANNEL_NUM; k++)
			{
				for(int i = 0; i < gSelectedDgsPointDataNum[k][3]; i++)
				{
					gSelectedDgsPointData[k][0][i].serial = (short)(gwOldDacSerial[k][i] 
																					* (gfOldDacRange[k] / DisplaySet_ad[k].fRange));
			//		gSelectedDgsPointData[k][0][i].serial = curveChangeByDelay(gSelectedDgsPointData[k][0][i].serial, 
			//																					fRangeStart[k], fRangeEnd[k] - fRangeStart[k]);
	
					gSelectedDgsPointData[k][1][i].serial = gSelectedDgsPointData[k][0][i].serial;
	
					gSelectedDgsPointData[k][2][i].serial = gSelectedDgsPointData[k][0][i].serial;
	
					gSelectedDgsPointData[k][3][i].serial = gSelectedDgsPointData[k][0][i].serial;
				 } 				
				
				if(gDgsSwitch[k][0])
					gDacChangeFlag = FALSE;//dac曲线需要重画
			}
		}
		//AVG
		if(gbAvgCurveFlag[CurrentChannelNo] == ON)
		{
			bgAvgChangeFlag = TRUE;//停止画线
			Sleep(10);//等待停止画线
			for(int i = 0; i < AVG_CURVE_NUM; i++)
			{
				drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
			}
	
		}
		if(gWaveDrawAvgFlag == FALSE)//制作曲线时不跟着变化
		{
			if(!gbGuanLian) //非关联
			{
				for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
				{
					gSelectedAvgPointData[CurrentChannelNo][0][i].serial = (short)(gwOldAvgSerial[CurrentChannelNo][i] 
																					* gfOldAvgRange[CurrentChannelNo] / DisplaySet_ad[CurrentChannelNo].fRange);
					gSelectedAvgPointData[CurrentChannelNo][0][i].serial = curveChangeByDelay(gSelectedAvgPointData[CurrentChannelNo][0][i].serial, 
																								fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
	
					gSelectedAvgPointData[CurrentChannelNo][1][i].serial = gSelectedAvgPointData[CurrentChannelNo][0][i].serial;
	
					gSelectedAvgPointData[CurrentChannelNo][2][i].serial = gSelectedAvgPointData[CurrentChannelNo][0][i].serial;

					gSelectedAvgPointData[CurrentChannelNo][3][i].serial = gSelectedAvgPointData[CurrentChannelNo][0][i].serial;
				 }
			}
			else
			{
				for(int k = 0; k < CHANNEL_NUM; k++)
				{
					for(int i = 0; i < gSelectedAvgPointDataNum[k]; i++)
					{
						gSelectedAvgPointData[k][0][i].serial = (short)(gwOldAvgSerial[k][i] 
																						* gfOldAvgRange[k] / DisplaySet_ad[k].fRange);
						gSelectedAvgPointData[k][0][i].serial = curveChangeByDelay(gSelectedAvgPointData[k][0][i].serial, 
																								fRangeStart[k], fRangeEnd[k] - fRangeStart[k]);
						
						gSelectedAvgPointData[k][1][i].serial = gSelectedAvgPointData[k][0][i].serial;
	
						gSelectedAvgPointData[k][2][i].serial =	gSelectedAvgPointData[k][0][i].serial;

						gSelectedAvgPointData[k][3][i].serial = gSelectedAvgPointData[k][0][i].serial;
					 }
				}
			}
		}
		//TCG
		if(!gbGuanLian) //非关联
		{
			for(int i = 0; i < gSelectedDacPointDataNum[CurrentChannelNo]; i++)
			{
				gSelectedDacPointData[CurrentChannelNo][i].serial = (short)(gwOldTcgSerial[CurrentChannelNo][i] 
																				* gfOldTcgRange[CurrentChannelNo] / DisplaySet_ad[CurrentChannelNo].fRange);
			}
		}
		else
		{
			for(int k = 0; k < CHANNEL_NUM; k++)
			{
				for(int i = 0; i < gSelectedDacPointDataNum[k]; i++)
				{
					gSelectedDacPointData[k][i].serial = (short)(gwOldAvgSerial[k][i] 
																					* gfOldAvgRange[k] / DisplaySet_ad[k].fRange);
				}
			}
		}
		
		if(gbAvgCurveFlag[CurrentChannelNo] == ON)
			bgAvgChangeFlag = FALSE;//AVG曲线需要重画
			
		if(bOldWaveDrawDacFlag)//正在制作曲线时
		{
			//wangbingfu add start at 2011-4-1	制作结束时取gOldMakeDgsPointData的值,又要取改变后的值,所以这里必须把改变后的值赋给他
			for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
			{
				gOldMakeDgsPointData[i].serial = gSelectedDgsPointData[CurrentChannelNo][3][i].serial;
			}
			//wangbingfu add end at 2011-4-1
			gWaveDrawDacFlag = TRUE;//重画制作线
		}
		else if(bOldWaveDrawAvgFlag)//正在制作AVG曲线时 制作结束时取gOldMakeDgsPointData的值,又要取改变后的值,所以这里必须把改变后的值赋给他
		{
			for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
			{
				gOldMakeAvgPointData[i].serial = gSelectedAvgPointData[CurrentChannelNo][3][i].serial;
			}
			gWaveDrawAvgFlag = TRUE;//重画制作线
		}
		else if(bOldWaveDrawTcgFlag)//正在制作TCG曲线时
			gWaveDrawTcgFlag = TRUE;
		else
		{
			;//
		}
		// 保存配置到默认文件
		if(bSave)
			SaveParamFile(lpDefaultParamFile);				
	}	
}
void curve100To110Display()
{
	if(gDgsSwitch[CurrentChannelNo][0])
	{
		gDacChangeFlag = TRUE;//停止画线
		Sleep(10);//等待停止画线
	/*	if(CurrentChannelNo == 0)
		{
			for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
			{
				drawCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
			}
		}
		else
		{
			for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
			{
				drawDashCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
			}
		}*/
		if(CurrentChannelNo == 0)
		{
			for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
			{
				drawCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
			}
		}
		if(!gbGuanLian) //非关联
		{
			for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
			{
				gSelectedDgsPointData[CurrentChannelNo][0][i].data = gSelectedDgsPointData[CurrentChannelNo][0][i].data * 100 / 110;

				gSelectedDgsPointData[CurrentChannelNo][1][i].data = gSelectedDgsPointData[CurrentChannelNo][1][i].data * 100 / 110;
				gSelectedDgsPointData[CurrentChannelNo][2][i].data = gSelectedDgsPointData[CurrentChannelNo][2][i].data * 100 / 110;

				gSelectedDgsPointData[CurrentChannelNo][3][i].data = gSelectedDgsPointData[CurrentChannelNo][3][i].data * 100 / 110;
			}
		}
		else
		{
			for(int k = 0; k < CHANNEL_NUM; k++)
			{
				for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
				{
					gSelectedDgsPointData[k][0][i].data = gSelectedDgsPointData[k][0][i].data * 100 / 110;

					gSelectedDgsPointData[k][1][i].data = gSelectedDgsPointData[k][0][i].data * 100 / 110;
					
					gSelectedDgsPointData[k][2][i].data = gSelectedDgsPointData[k][0][i].data * 100 / 110;

					gSelectedDgsPointData[k][3][i].data = gSelectedDgsPointData[k][0][i].data * 100 / 110;
				}
			}
		}
		gDacChangeFlag = FALSE;//dac曲线需要重画
	}
	

	//AVG
	if(gbAvgCurveFlag[CurrentChannelNo] == ON)
	{
		bgAvgChangeFlag = TRUE;//停止画线
		Sleep(30);//等待停止画线
	/*	if(CurrentChannelNo == 0)
		{
			for(int i = 0; i < AVG_CURVE_NUM; i++)
			{
				drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo],RED_POINT, FALSE);
			}
		}
		else
		{
			for(int i = 0; i < AVG_CURVE_NUM; i++)
			{
				drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo],RED_POINT, FALSE);
			}
		}*/
		for(int i = 0; i < AVG_CURVE_NUM; i++)
		{
			drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo],RED_POINT, FALSE);
		}
		HDC hdc = GetDC(hWndMain);
		WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
		HBRUSH hbr = CreateSolidBrush(wColorBack);
		
		RECT rect;
		GetClipBox(hdc, &rect);
		
		RECT RctWave;// 波形区域
		RctWave.left = rect.left + SCALE_WIDTH;
		RctWave.right = rect.right - SCALE_WIDTH;
		RctWave.top = rect.top;
		RctWave.bottom = rect.bottom - SCALE_WIDTH;
		
		RECT stTextRect;
		stTextRect.left = RctWave.left + 2;
		stTextRect.right = RctWave.left + 60;
		for(int i = 0; i < AVG_CURVE_NUM; i++)
		{
			POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
			stTextRect.top = stFirstPoint.y - 25;
			stTextRect.bottom = stFirstPoint.y;
		
			FillRect(hdc, &stTextRect, hbr);
		}
		DeleteObject(hbr);
		ReleaseDC(hWndMain,hdc);

		if(!gbGuanLian) //非关联
		{
			for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
			{
				gSelectedAvgPointData[CurrentChannelNo][0][i].data = gSelectedAvgPointData[CurrentChannelNo][0][i].data * 100 /110;

				gSelectedAvgPointData[CurrentChannelNo][1][i].data = gSelectedAvgPointData[CurrentChannelNo][1][i].data * 100 /110;
				
				gSelectedAvgPointData[CurrentChannelNo][2][i].data = gSelectedAvgPointData[CurrentChannelNo][2][i].data * 100 /110;

				gSelectedAvgPointData[CurrentChannelNo][3][i].data = gSelectedAvgPointData[CurrentChannelNo][3][i].data * 100 /110;
			}
		}
		else
		{
			for(int k = 0; k < CHANNEL_NUM; k++)
			{
				for(int i = 0; i < gSelectedAvgPointDataNum[k]; i++)
				{
					gSelectedAvgPointData[k][0][i].data = gSelectedAvgPointData[k][0][i].data * 100 /110;

					gSelectedAvgPointData[k][1][i].data = gSelectedAvgPointData[k][1][i].data * 100 /110;
					
					gSelectedAvgPointData[k][2][i].data = gSelectedAvgPointData[k][2][i].data * 100 /110;

					gSelectedAvgPointData[k][3][i].data = gSelectedAvgPointData[k][3][i].data * 100 /110;
				}
			}
		}
		bgAvgChangeFlag = FALSE;//AVG曲线需要重画
	}	
}
void curve110To100Display()
{
	if(gDgsSwitch[CurrentChannelNo][0])
	{
		gDacChangeFlag = TRUE;//停止画线
		Sleep(10);//等待停止画线
		/*if(CurrentChannelNo == 0)
		{
			for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
			{
				drawCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
			}
		}
		else
		{
			for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
			{
				drawDashCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
			}
		}*/
		for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
		{
			drawCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
		}

		if(!gbGuanLian) //非关联
		{
			for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
			{
				gSelectedDgsPointData[CurrentChannelNo][0][i].data = gSelectedDgsPointData[CurrentChannelNo][0][i].data * 110 / 100;

				gSelectedDgsPointData[CurrentChannelNo][1][i].data = gSelectedDgsPointData[CurrentChannelNo][1][i].data * 110 / 100;
				gSelectedDgsPointData[CurrentChannelNo][2][i].data = gSelectedDgsPointData[CurrentChannelNo][2][i].data * 110 / 100;

				gSelectedDgsPointData[CurrentChannelNo][3][i].data = gSelectedDgsPointData[CurrentChannelNo][3][i].data * 110 / 100;
			}
		}
		else
		{
			for(int k = 0; k < CHANNEL_NUM; k++)
			{
				for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
				{
					gSelectedDgsPointData[k][0][i].data = gSelectedDgsPointData[k][0][i].data * 110 / 100;

					gSelectedDgsPointData[k][1][i].data = gSelectedDgsPointData[k][0][i].data * 110 / 100;
					
					gSelectedDgsPointData[k][2][i].data = gSelectedDgsPointData[k][0][i].data * 110 / 100;

					gSelectedDgsPointData[k][3][i].data = gSelectedDgsPointData[k][0][i].data * 110 / 100;
				}
			}
		}
		gDacChangeFlag = FALSE;//dac曲线需要重画
	}
	

	//AVG
	if(gbAvgCurveFlag[CurrentChannelNo] == ON)
	{
		bgAvgChangeFlag = TRUE;//停止画线
		Sleep(30);//等待停止画线
		/*if(CurrentChannelNo == 0)
		{
			for(int i = 0; i < AVG_CURVE_NUM; i++)
			{
				drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo],RED_POINT, FALSE);
			}
		}
		else
		{
			for(int i = 0; i < AVG_CURVE_NUM; i++)
			{
				drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo],RED_POINT, FALSE);
			}
		}*/
		for(int i = 0; i < AVG_CURVE_NUM; i++)
		{
			drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo],RED_POINT, FALSE);
		}
		HDC hdc = GetDC(hWndMain);
		WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
		HBRUSH hbr = CreateSolidBrush(wColorBack);
		
		RECT rect;
		GetClipBox(hdc, &rect);
		
		RECT RctWave;// 波形区域
		RctWave.left = rect.left + SCALE_WIDTH;
		RctWave.right = rect.right - SCALE_WIDTH;
		RctWave.top = rect.top;
		RctWave.bottom = rect.bottom - SCALE_WIDTH;
		
		RECT stTextRect;
		stTextRect.left = RctWave.left + 2;
		stTextRect.right = RctWave.left + 60;
		for(int i = 0; i < AVG_CURVE_NUM; i++)
		{
			POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
			stTextRect.top = stFirstPoint.y - 25;
			stTextRect.bottom = stFirstPoint.y;
		
			FillRect(hdc, &stTextRect, hbr);
		}
		DeleteObject(hbr);
		ReleaseDC(hWndMain,hdc);

		if(!gbGuanLian) //非关联
		{
			for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
			{
				gSelectedAvgPointData[CurrentChannelNo][0][i].data = gSelectedAvgPointData[CurrentChannelNo][0][i].data * 110 / 100;

				gSelectedAvgPointData[CurrentChannelNo][1][i].data = gSelectedAvgPointData[CurrentChannelNo][1][i].data * 110 / 100;
				
				gSelectedAvgPointData[CurrentChannelNo][2][i].data = gSelectedAvgPointData[CurrentChannelNo][2][i].data * 110 / 100;

				gSelectedAvgPointData[CurrentChannelNo][3][i].data = gSelectedAvgPointData[CurrentChannelNo][3][i].data * 110 / 100;
			}
		}
		else
		{
			for(int k = 0; k < CHANNEL_NUM; k++)
			{
				for(int i = 0; i < gSelectedAvgPointDataNum[k]; i++)
				{
					gSelectedAvgPointData[k][0][i].data = gSelectedAvgPointData[k][0][i].data * 110 / 100;

					gSelectedAvgPointData[k][1][i].data = gSelectedAvgPointData[k][1][i].data * 110 / 100;
					
					gSelectedAvgPointData[k][2][i].data = gSelectedAvgPointData[k][2][i].data * 110 / 100;

					gSelectedAvgPointData[k][3][i].data = gSelectedAvgPointData[k][3][i].data * 110 / 100;
				}
			}
		}
		bgAvgChangeFlag = FALSE;//AVG曲线需要重画
	}	
}
/*曲线随延迟改变*/
void delayChangeCurve(float fRangeStartChange, float fRange)
{	
	if(fRangeStartChange != 0)
	{
		if(gDgsSwitch[CurrentChannelNo][0])
		{
			gDacChangeFlag = TRUE;//停止画线
			Sleep(10);//等待停止画线
			for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	
			{
				drawCurve2(gSelectedDgsPointData[CurrentChannelNo][i], gSelectedDgsPointDataNum[CurrentChannelNo][i], RED_POINT, FALSE);//删除旧线
			}
		}
		
		if(!gbGuanLian) //非关联
		{
			for(int i = 0; i < gSelectedDgsPointDataNum[CurrentChannelNo][3]; i++)
			{
				gSelectedDgsPointData[CurrentChannelNo][0][i].serial = curveChangeByDelay(gSelectedDgsPointData[CurrentChannelNo][0][i].serial, fRangeStartChange, fRange);
	
				gSelectedDgsPointData[CurrentChannelNo][1][i].serial = curveChangeByDelay(gSelectedDgsPointData[CurrentChannelNo][1][i].serial, fRangeStartChange, fRange);
	
				gSelectedDgsPointData[CurrentChannelNo][2][i].serial = curveChangeByDelay(gSelectedDgsPointData[CurrentChannelNo][2][i].serial, fRangeStartChange, fRange);
				
				gSelectedDgsPointData[CurrentChannelNo][3][i].serial = curveChangeByDelay(gSelectedDgsPointData[CurrentChannelNo][3][i].serial, fRangeStartChange, fRange);
	
			 } 				
			if(gDgsSwitch[CurrentChannelNo][0])
				gDacChangeFlag = FALSE;//dac曲线需要重画
		}
		else
		{
			for(int k = 0; k < CHANNEL_NUM; k++)
			{
				for(int i = 0; i < gSelectedDgsPointDataNum[k][3]; i++)
				{
					gSelectedDgsPointData[k][0][i].serial = curveChangeByDelay(gSelectedDgsPointData[k][0][i].serial, fRangeStartChange, fRange);
	
					gSelectedDgsPointData[k][1][i].serial = curveChangeByDelay(gSelectedDgsPointData[k][1][i].serial, fRangeStartChange, fRange);
	
					gSelectedDgsPointData[k][2][i].serial = curveChangeByDelay(gSelectedDgsPointData[k][2][i].serial, fRangeStartChange, fRange);
	
					gSelectedDgsPointData[k][3][i].serial = curveChangeByDelay(gSelectedDgsPointData[k][3][i].serial, fRangeStartChange, fRange);
				 } 				
				
				if(gDgsSwitch[k][0])
					gDacChangeFlag = FALSE;//dac曲线需要重画
			}
		}
		//AVG
		if(gbAvgCurveFlag[CurrentChannelNo] == ON)
		{
			bgAvgChangeFlag = TRUE;//停止画线
			Sleep(10);//等待停止画线
			for(int i = 0; i < AVG_CURVE_NUM; i++)
			{
				drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
			}
	
		}

		if(!gbGuanLian) //非关联
		{
			for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
			{
				gSelectedAvgPointData[CurrentChannelNo][0][i].serial = curveChangeByDelay(gSelectedAvgPointData[CurrentChannelNo][0][i].serial, fRangeStartChange, fRange);

				gSelectedAvgPointData[CurrentChannelNo][1][i].serial = curveChangeByDelay(gSelectedAvgPointData[CurrentChannelNo][1][i].serial, fRangeStartChange, fRange);

				gSelectedAvgPointData[CurrentChannelNo][2][i].serial = curveChangeByDelay(gSelectedAvgPointData[CurrentChannelNo][2][i].serial, fRangeStartChange, fRange);

				gSelectedAvgPointData[CurrentChannelNo][3][i].serial = curveChangeByDelay(gSelectedAvgPointData[CurrentChannelNo][3][i].serial, fRangeStartChange, fRange);
			 }
		}
		else
		{
			for(int k = 0; k < CHANNEL_NUM; k++)
			{
				for(int i = 0; i < gSelectedAvgPointDataNum[k]; i++)
				{
					gSelectedAvgPointData[k][0][i].serial = curveChangeByDelay(gSelectedAvgPointData[k][0][i].serial, fRangeStartChange, fRange);

					gSelectedAvgPointData[k][1][i].serial = curveChangeByDelay(gSelectedAvgPointData[k][1][i].serial, fRangeStartChange, fRange);

					gSelectedAvgPointData[k][2][i].serial = curveChangeByDelay(gSelectedAvgPointData[k][2][i].serial, fRangeStartChange, fRange);

					gSelectedAvgPointData[k][3][i].serial = curveChangeByDelay(gSelectedAvgPointData[k][3][i].serial, fRangeStartChange, fRange);
				 }
			}
		}
		
		if(gbAvgCurveFlag[CurrentChannelNo] == ON)
			bgAvgChangeFlag = FALSE;//AVG曲线需要重画			
	}	
}
void wait400000()
{
	/*for(int i = 0; i < 4000000; i++)
	{
		;//
	}*/
	int i = 0;
	while(i < 4000000)
	{
		i++;
	}
}
//wireless 2011-5-30
/**
* SOCKET线程处理过程
* @param     lpParameter: 传入线程参数
* @return    0:结束退出线程
* @since      1.00
*/

DWORD WINAPI SocketThreadProc( LPVOID lpParameter )
{
#if 0
	WSADATA wsd;
	if(WSAStartup( MAKEWORD( 2, 2 ), &wsd ) != 0 )// 支持Windows Sockets 2.2版本
	{
		return 0;	
	}
	
	WSAEVENT eventList[MAX_SOCKETS  + 1];
	//////////////////////////////////////////////////////////////////////////
	for (int i=0; i<(MAX_SOCKETS  + 1); i++)
	{
		eventList[i] = NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	int nEventTotal = 0;
	eventList[nEventTotal] = WSACreateEvent();

	SOCKET sockList[MAX_SOCKETS + 1];
	//////////////////////////////////////////////////////////////////////////
	for (int i=0; i<(MAX_SOCKETS  + 1); i++)
	{
		sockList[i] = INVALID_SOCKET;
	}
	//////////////////////////////////////////////////////////////////////////
	SOCKET sockListen = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
//	unsigned long ulMode = 1;
//	ioctlsocket(sockListen, FIONBIO, (unsigned long*)&ulMode);
	if( INVALID_SOCKET == sockListen )
	{
		return 0;
	}

	
	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port= htons( 3333 ); // 保证字节顺序
	addr.sin_addr.s_addr= htonl( INADDR_ANY );

	
	int nResult = bind( sockListen, (sockaddr*)&addr, sizeof(sockaddr) );
	if ( nResult == SOCKET_ERROR )
	{
		return 0;
	}
	
	nResult = listen( sockListen, MAX_BACKLOG ); // 最多 MAX_BACKLOG 个 Pending 连接
	if ( nResult == SOCKET_ERROR )
	{
		return 0;
	}
	sockList[nEventTotal] = sockListen;

	WSAEventSelect( sockList[nEventTotal], eventList[nEventTotal], FD_ACCEPT | FD_CLOSE );

	int nIndex;
	nEventTotal++;
    
	while ((nIndex = WSAWaitForMultipleEvents( nEventTotal, eventList, FALSE, WSA_INFINITE, TRUE )) != WSA_WAIT_FAILED)
	{
		nIndex = nIndex - WSA_WAIT_EVENT_0;
		// Song add

		BOOL bRet = WSAResetEvent(eventList[nIndex]);
		if (bRet == FALSE) 
		{
			wchar_t strText[20];
			wsprintf(strText, _T("Reset%d"), nIndex);
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
		}	

		WSANETWORKEVENTS stNetworkEvents;
		WSAEnumNetworkEvents( sockList[nIndex], eventList[nIndex], &stNetworkEvents );
		switch(stNetworkEvents.lNetworkEvents)
		{
			case FD_ACCEPT://服务器端接收到连接请求后
				{
					while(true)
					{
						wchar_t strText[20];
						wsprintf(strText, _T("accept%d"), nIndex);
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)strText);
						if ( stNetworkEvents.iErrorCode[FD_ACCEPT_BIT] != 0 )  // 发生网络错误
						{
							break;
						}
						SOCKET sockAccept;
						if((sockAccept = accept(sockList[nIndex], NULL, NULL)) == INVALID_SOCKET )
						{
							break;
						}
						if ( nEventTotal < MAX_SOCKETS)
						{
							sockList[nEventTotal] = sockAccept;
							gSockClient[nEventTotal] = sockAccept;
							wchar_t strText[20];
							wsprintf(strText, _T("accept%d"), nIndex);
							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)strText);

							if ((eventList[nEventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT )
							{
								break;
							}
							if (WSAEventSelect( sockList[nEventTotal], eventList[nEventTotal], FD_READ | FD_CLOSE ) == SOCKET_ERROR)
							{
								break;
							}

							nEventTotal++;
						}
						else
						{
							nEventTotal = 1;
							sockList[nEventTotal] = sockAccept;
							if ((eventList[nEventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT )
							{
								break;
							}
							if (WSAEventSelect( sockList[nEventTotal], eventList[nEventTotal], FD_READ | FD_CLOSE ) == SOCKET_ERROR)
							{
								break;
							}
						}
					}
				}
				break;
			case FD_READ:
				{

					int nResult = 0;
					char szRecvData[NETHEAD_SIZE + 512];
					memset(szRecvData, 0, sizeof(szRecvData));
					
					if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
					{
						MessageBox(hWndMain, _T("recverror"), _T("recverror"), MB_OK);
					 	break;
					}

					nResult = recv( sockList[nIndex], szRecvData, NETHEAD_SIZE, 0 );
					
					PACKET_HEAD_t stPacketHead;
					memcpy( &stPacketHead, szRecvData, sizeof(stPacketHead));
	
					nResult = recv( sockList[nIndex], &szRecvData[NETHEAD_SIZE], stPacketHead.dwSize, 0 );
					SOCKET sockClient = sockList[nIndex];
					AnalyseRecvData( szRecvData, sockClient );
				}
				break;
			case FD_CLOSE:
				{
					wchar_t strText[20];
					wsprintf(strText, _T("close%d"), nIndex);
					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
				//	PostMessage(hStatusBar, SB_SETTEXT, 
				//		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("close11"));
				/*	if(gSockClient == sockList[nIndex])
					{
						gbChannelSend[0] = FALSE;
						gnSendCount = 0;
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("close11"));
					}
					if(gSockClient2 == sockList[nIndex])
					{
					//	gbChannelSend[1] = FALSE;
					//	gnSendCount = 0;
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("close12"));
					}*/
					if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
					{
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("Error7"));
					 	break;
					}				
					BOOL bRet = WSACloseEvent( eventList[nIndex] );
					if (!bRet)
					{
						MessageBox(NULL, _T("closeevent fail"),_T("ndt_ce"), MB_OK);
					}

					//shutdown(sockList[nIndex],SD_SEND);
					gSockClient[nIndex] = NULL;
					BOOL bRet1 = closesocket( sockList[nIndex] );
					if (bRet1 == 0)
					{
					}
					else
					{
						MessageBox(NULL, _T("关闭socket失败1"),_T("ndt_ce"), MB_OK);
					}

					for (int i = nIndex; i < nEventTotal - 1; i++)
					{						
						eventList[i] = eventList[i + 1];
						sockList[i] = sockList[i + 1];
					}
					nEventTotal--;
					//nIndex--;	// Song add
				}
				break;
			default:
				break;
		}

	}
	MessageBox(NULL, _T("Socket1 exit"),_T("ndt_ce"), MB_OK);
	return 0;
#else
	SOCKET_PROC_PARAM_t* pstSocketParam = (SOCKET_PROC_PARAM_t*)lpParameter;
	SOCKET sockClient = pstSocketParam->sockClient;
	int nIndex = pstSocketParam->nIndex;
//	SOCKET* pSocket = (SOCKET*)lpParameter;
//	SOCKET sockClient = *pSocket;
	
	WSAEVENT event;
	if ((event = WSACreateEvent()) == WSA_INVALID_EVENT )
	{
		wchar_t strText[20];
		wsprintf(strText, _T("event%d"), nIndex);
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
		return 0;
	}
	if (WSAEventSelect(sockClient, event, FD_READ | FD_CLOSE ) == SOCKET_ERROR)
	{
		wchar_t strText[20];
		wsprintf(strText, _T("select%d"), nIndex);
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
		return 0;
	}
	
	while (WSAWaitForMultipleEvents( 1, &event, FALSE, WSA_INFINITE, TRUE ) != WSA_WAIT_FAILED)
	{
		WSANETWORKEVENTS stNetworkEvents;
		WSAEnumNetworkEvents( sockClient, event, &stNetworkEvents );
		switch(stNetworkEvents.lNetworkEvents)
		{
			case FD_READ:
				{

					int nResult = 0;
					char szRecvData[NETHEAD_SIZE + 512];
					memset(szRecvData, 0, sizeof(szRecvData));

					if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
					{
						MessageBox(hWndMain, _T("recverror"), _T("recverror"), MB_OK);
						break;
					}

					nResult = recv( sockClient, szRecvData, NETHEAD_SIZE, 0 );

					PACKET_HEAD_t stPacketHead;
					memcpy( &stPacketHead, szRecvData, sizeof(stPacketHead));

					nResult = recv( sockClient, &szRecvData[NETHEAD_SIZE], stPacketHead.dwSize, 0 );
					AnalyseRecvData( szRecvData, sockClient );
				}
				break;
			case FD_CLOSE:
				{
					wchar_t strText[20];
					wsprintf(strText, _T("MonitorClose%d\n"), sockClient);
					OutputDebugString(strText);
				//	PostMessage(hStatusBar, SB_SETTEXT, 
				//		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
				
					if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
					{
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("Error7"));
					 	break;
					}				
					BOOL bRet = WSACloseEvent( event );
					if (!bRet)
					{
						MessageBox(NULL, _T("closeevent fail"),_T("ndt_ce"), MB_OK);
					}

					//shutdown(sockList[nIndex],SD_SEND);
					//gSockClient[nIndex] = NULL;
					for(vector<SOCKET>::iterator iter = gVecSockMonitor.begin(); iter!= gVecSockMonitor.end();)
					{
						if(*iter == sockClient)
						{
							wchar_t strText[20];
							wsprintf(strText, _T("MonitorDelete%d\n"), sockClient);
							PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
							iter = gVecSockMonitor.erase(iter);
							OutputDebugString(strText);
						}
						else
						{
							++iter;
						}
					}

					BOOL bRet1 = closesocket( sockClient );
					if (bRet1 == 0)
					{
					}
					else
					{
						MessageBox(NULL, _T("关闭socket失败1"),_T("ndt_ce"), MB_OK);
					}

					if(gVecSockMonitor.size() == 0)
					{
						gbChannelSend[0] = FALSE;
					}
					wsprintf(strText, _T("MonitorCloseOk%d\n"), sockClient);
					OutputDebugString(strText);
				}
				return 1;
			default:
				break;
		}
	}
#endif
}

DWORD WINAPI SocketThreadProc2( LPVOID lpParameter )
{
#if 0
	WSADATA wsd;
	if(WSAStartup( MAKEWORD( 2, 2 ), &wsd ) != 0 )// 支持Windows Sockets 2.2版本
	{
		return 0;	
	}
	
	WSAEVENT eventList[MAX_SOCKETS  + 1];	

	//////////////////////////////////////////////////////////////////////////
	for (int i=0; i<(MAX_SOCKETS  + 1); i++)
	{
		eventList[i] = NULL;
	}
	//////////////////////////////////////////////////////////////////////////

	int nEventTotal = 0;
	eventList[nEventTotal] = WSACreateEvent();

	SOCKET sockList[MAX_SOCKETS + 1];

	for (int i=0; i<(MAX_SOCKETS  + 1); i++)
	{
		sockList[i] = INVALID_SOCKET;
	}
	
	SOCKET sockListen = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );	
	if( INVALID_SOCKET == sockListen )
	{
		return 0;
	}

	
	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port= htons( 4444 ); // 保证字节顺序
	addr.sin_addr.s_addr= htonl( INADDR_ANY );

	
	int nResult = bind( sockListen, (sockaddr*)&addr, sizeof(sockaddr) );
	if ( nResult == SOCKET_ERROR )
	{
		return 0;
	}
	
	nResult = listen( sockListen, MAX_BACKLOG ); // 最多 MAX_BACKLOG 个 Pending 连接
	if ( nResult == SOCKET_ERROR )
	{
		return 0;
	}
	sockList[nEventTotal] = sockListen;

	WSAEventSelect( sockList[nEventTotal], eventList[nEventTotal], FD_ACCEPT | FD_CLOSE );
	int nIndex;
	nEventTotal++;
	
	while ((nIndex = WSAWaitForMultipleEvents( nEventTotal, eventList, FALSE, WSA_INFINITE, TRUE )) != WSA_WAIT_FAILED)
	{

		nIndex = nIndex - WSA_WAIT_EVENT_0;
		WSANETWORKEVENTS stNetworkEvents;
		WSAEnumNetworkEvents( sockList[nIndex], eventList[nIndex], &stNetworkEvents );
		switch(stNetworkEvents.lNetworkEvents)
		{
			case FD_ACCEPT://服务器端接收到连接请求后
				{	
					if ( stNetworkEvents.iErrorCode[FD_ACCEPT_BIT] != 0 )  // 发生网络错误
					{
						break;
					}
					SOCKET sockAccept;
					if((sockAccept = accept(sockList[nIndex], NULL, NULL)) == INVALID_SOCKET )
					{
						break;
					}

					int nBufLen;  
    				int nOptLen = sizeof(nBufLen);
					if(getsockopt(sockAccept,SOL_SOCKET,SO_SNDBUF,(char*)&nBufLen,&nOptLen) == SOCKET_ERROR)  
					{  
					}  
				  
					// 设置当前套接字s的接受数据缓冲区为原来的4倍	 
					nBufLen *= 4;	
					if(setsockopt(sockAccept,SOL_SOCKET,SO_SNDBUF,(char*)&nBufLen,nOptLen) == SOCKET_ERROR) 
					{   
					}  

					if ( nEventTotal < MAX_SOCKETS)
					{
						sockList[nEventTotal] = sockAccept;
						if ((eventList[nEventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT )
						{
							break;
						}
						if (WSAEventSelect( sockList[nEventTotal], eventList[nEventTotal], FD_READ | FD_CLOSE ) == SOCKET_ERROR)
						{
							break;
						}
						nEventTotal++;
					}
					else
					{
						nEventTotal = 1;
						sockList[nEventTotal] = sockAccept;
						if ((eventList[nEventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT )
						{
							break;
						}
						if (WSAEventSelect( sockList[nEventTotal], eventList[nEventTotal], FD_READ | FD_CLOSE ) == SOCKET_ERROR)
						{
							break;
						}
					}
				}
				break;
			case FD_READ:
				{
					int nResult = 0;
					char szRecvData[NETHEAD_SIZE + 512];
					memset(szRecvData, 0, sizeof(szRecvData));
					
					if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
					{
						MessageBox(hWndMain, _T("recverror"), _T("recverror"), MB_OK);
					 	break;
					}
					nResult = recv( sockList[nIndex], szRecvData, NETHEAD_SIZE, 0 );
	
					PACKET_HEAD_t stPacketHead;
					memcpy( &stPacketHead, szRecvData, sizeof(stPacketHead));
					nResult = recv( sockList[nIndex], &szRecvData[NETHEAD_SIZE], stPacketHead.dwSize, 0 );

					SOCKET sockClient = sockList[nIndex];
					AnalyseRecvData( szRecvData, sockClient );
				}
				break;
			case FD_CLOSE:
				{

				/*	if(gSockClient == sockList[nIndex])
					{
						gbChannelSend[0] = FALSE;
						gnSendCount = 0;
					}
					if(gSockClient2 == sockList[nIndex])
					{
						gbChannelSend[1] = FALSE;
						gnSendCount = 0;
					}*/
					if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
					{
					 	break;
					}				
					WSACloseEvent( eventList[nIndex] );
					shutdown(sockList[nIndex], SD_SEND);
					Sleep(50);

					BOOL bRet1 = closesocket( sockList[nIndex] );
					if (bRet1 == 0)
					{
						
					}
					else
					{
						MessageBox(NULL, _T("关闭socket失败2"),_T("ndt_ce"), MB_OK);
					}

					for (int i = nIndex; i < nEventTotal - 1; i++)
					{						
						eventList[i] = eventList[i + 1];
						sockList[i] = sockList[i + 1];
					}
					nEventTotal--;
				}
				break;
			default:
				break;
		}
	}
	
	MessageBox(NULL, _T("Socket2 exit"),_T("ndt_ce"), MB_OK);
	return 0;
#else
	SOCKET_PROC_PARAM_t* pstSocketParam = (SOCKET_PROC_PARAM_t*)lpParameter;
	SOCKET sockClient = pstSocketParam->sockClient;
	int nIndex = pstSocketParam->nIndex;
	WSAEVENT event;
	if ((event = WSACreateEvent()) == WSA_INVALID_EVENT )
	{
		wchar_t strText[20];
		wsprintf(strText, _T("event%d"), nIndex);
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
		return 0;
	}
	if (WSAEventSelect(sockClient, event, FD_READ | FD_CLOSE ) == SOCKET_ERROR)
	{
		wchar_t strText[20];
		wsprintf(strText, _T("select%d"), nIndex);
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
		return 0;
	}

	while (WSAWaitForMultipleEvents( 1, &event, FALSE, WSA_INFINITE, TRUE ) != WSA_WAIT_FAILED)
	{
		WSANETWORKEVENTS stNetworkEvents;
		WSAEnumNetworkEvents( sockClient, event, &stNetworkEvents );
		switch(stNetworkEvents.lNetworkEvents)
		{
		case FD_READ:
			{

				int nResult = 0;
				char szRecvData[NETHEAD_SIZE + 512];
				memset(szRecvData, 0, sizeof(szRecvData));

				if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
				{
					MessageBox(hWndMain, _T("recverror"), _T("recverror"), MB_OK);
					break;
				}

				nResult = recv( sockClient, szRecvData, NETHEAD_SIZE, 0 );

				PACKET_HEAD_t stPacketHead;
				memcpy( &stPacketHead, szRecvData, sizeof(stPacketHead));

				nResult = recv( sockClient, &szRecvData[NETHEAD_SIZE], stPacketHead.dwSize, 0 );
				AnalyseRecvData( szRecvData, sockClient );
			}
			break;
		case FD_CLOSE:
			{
				wchar_t strText[20];
				wsprintf(strText, _T("SocketThreadProc2-CScanClose%d\n"), sockClient);
				OutputDebugString(strText);
				//	PostMessage(hStatusBar, SB_SETTEXT, 
				//		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);

				if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
				{
					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("Error7"));
					break;
				}				
				BOOL bRet = WSACloseEvent( event );
				if (!bRet)
				{
					MessageBox(NULL, _T("closeevent fail"),_T("ndt_ce"), MB_OK);
				}

				for(vector<SOCKET>::iterator iter = gVecSockCScan.begin(); iter!= gVecSockCScan.end();)
				{
					if(*iter == sockClient)
					{
						wchar_t strText[20];
						wsprintf(strText, _T("CScanDelete%d"), sockClient);
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
						iter = gVecSockCScan.erase(iter);
						OutputDebugString(strText);
					}
					else
					{
						++iter;
					}
				}

				BOOL bRet1 = closesocket( sockClient );
				if (bRet1 == 0)
				{
				}
				else
				{
					MessageBox(NULL, _T("关闭socket失败1"),_T("ndt_ce"), MB_OK);
				}

				if(gVecSockCScan.size() == 0)
				{
					gbChannelSend[1] = FALSE;
				}

				wsprintf(strText, _T("SocketThreadProc2-CScanCloseOk%d\n"), sockClient);
				OutputDebugString(strText);
			}
			return 1;
		default:
			break;
		}
	}
#endif
}


// wireless 2011-5-31
/**
* 解析接收到的数据
* @param     pData: 接收到的数据
* @return    void
* @since      1.00
*/

void AnalyseRecvData( char *pData, SOCKET sockClient )
{
	BOOL bFlag = TRUE;
	PACKET_HEAD_t stPacketHead;
	memcpy(&stPacketHead, pData, sizeof(stPacketHead));

	DWORD dwOffset = 0;	// 指针偏移量
	dwOffset = sizeof(stPacketHead);
	pData = pData + dwOffset;

	stPacketHead.wGateIndex = stPacketHead.wGateIndex + 1;
	if (( stPacketHead.wChannelIndex < 0 )
		&& ( stPacketHead.wChannelIndex > 1 ))
	{
		bFlag = FALSE;
		RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		swprintf(strText, _T("通道%d"), stPacketHead.wChannelIndex);
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)strText);
		return;
	}



	if (( stPacketHead.wGateIndex < 1 )
		|| ( stPacketHead.wGateIndex > 2 ))
	{
		bFlag = FALSE;
		RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
		wchar_t strText[20];
		memset(strText, 0, sizeof(strText));
		swprintf(strText, _T("闸门%d"), stPacketHead.wGateIndex);
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)strText);
		return;
	}

	switch(stPacketHead.dwCommand)
	{
		case COMMAND_CURRENT_CHANNEL:	//当前通道
			{
				int nChnNo = 0;
				memcpy( &nChnNo, pData, stPacketHead.dwSize );
				if(nChnNo >= 0 && nChnNo <= 1)
				{
					ChangeChannel(TRUE, nChnNo);
				}
				else
				{
					bFlag = FALSE;
				}				
			//	PostMessage(hWndMain, WM_CHAR, '\t', 0);
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_WORKMODE:	//工作模式
			{
				WORKMODE emWorkMode = SingleChannelA;
				memcpy( &emWorkMode, pData, stPacketHead.dwSize );
				if(emWorkMode >= SingleChannelA && emWorkMode <= ShuangJingGuanLian)
				{
					ChangeWorkMode(emWorkMode, TRUE);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_SEND_VOLTAGE:	//电压
			{
				WORD wVoltage = 0;
				memcpy( &wVoltage, pData, stPacketHead.dwSize );
				if(wVoltage >= 0 && wVoltage <= 3)
				{
					SendParam_ad[0].nPress = typeToVoltage(wVoltage);
					SendParam_ad[1].nPress = SendParam_ad[0].nPress;
					SetPressLableAndExecute(hBtnSendParamVoltage);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_SEND_MAIKUAN:	//脉宽
			{
				WORD wMaiKuan = 0;
				memcpy( &wMaiKuan, pData, stPacketHead.dwSize );
				if (( wMaiKuan <= MAX_MAIKUAN ) || ( wMaiKuan >= MIN_MAIKUAN ))
				{
					SendParam_ad[stPacketHead.wChannelIndex].nMaiKuan = wMaiKuan;
					SetMaiKuanLableAndExecute( hBtnSendParamMaiKuan );					
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_SEND_REPEAT_FREQ:	//重复频率
			{
				memcpy( &SendParam_ad[stPacketHead.wChannelIndex].nRefrainFrequency, pData, stPacketHead.dwSize );
				SetReFreqLableAndExecute( hBtnSendParamRepeatFreq );
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_SEND_ZERO:	//零点
			{
				float fZero = 0;
				memcpy( &fZero, pData, stPacketHead.dwSize );
				if(fZero >= MIN_AMEND && fZero <= MAX_AMEND)
				{
					SendParam_ad[stPacketHead.wChannelIndex].nAmend = fZero;
					SetAmendLableAndExecute(hBtnSendParamModify);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_SEND_QIANYAN:	//前沿
			{
				float fQianYan = 0;
				memcpy( &fQianYan, pData, stPacketHead.dwSize );
				if(fQianYan >= 0 && fQianYan <= 100)
				{
					SendParam_ad[stPacketHead.wChannelIndex].fQianYan = fQianYan;
					SetObliqueProbeQianYan(hBtnSendParamHeadQianYan);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_RECV_JIANBO_MODE:	//检波模式
			{
				WORD wJianBoMode = 0;
				memcpy( &wJianBoMode, pData, stPacketHead.dwSize );
				if(wJianBoMode >= 0 && wJianBoMode <= 3)
				{
					ReceiveParam_ad[stPacketHead.wChannelIndex].nJianBoMode = wJianBoMode;
					SetJianBoModeLableAndExecute( hBtnRecvParamWaveMode );
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_RECV_GAIN:	//增益	
			{
				float fPlus = 0;
				memcpy( &fPlus, pData, stPacketHead.dwSize );
				if(fPlus >= MIN_PLUS && fPlus <= MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fRefPlus)
				{
					float fOldPlus = ReceiveParam_ad[CurrentChannelNo].fPlus;
					ReceiveParam_ad[stPacketHead.wChannelIndex].fPlus = fPlus;
					SetPlusLableAndExecute( hBtnRecvParamGain, ReceiveParam_ad[stPacketHead.wChannelIndex].fPlus, 
										ReceiveParam_ad[stPacketHead.wChannelIndex].fRefPlus, FALSE, stPacketHead.wChannelIndex );
					gainChangeCurve(fOldPlus, TRUE);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_RECV_REF_GAIN:	//参考增益
			{
				float fRefPlus = 0;
				memcpy( &fRefPlus, pData, stPacketHead.dwSize );
				float fPlusRange = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus;
				float fMaxValue = fPlusRange > 80.0f ? 80.0f : fPlusRange;
				if(fRefPlus >= MIN_PLUS && fRefPlus <= fMaxValue)
				{
					float fOldRefPlus = ReceiveParam_ad[CurrentChannelNo].fRefPlus;
					ReceiveParam_ad[stPacketHead.wChannelIndex].fPlus = fRefPlus;
					SetRefPlusLableAndExecute(hBtnRecvParamReferenceGain, ReceiveParam_ad[CurrentChannelNo].fPlus, 
												ReceiveParam_ad[CurrentChannelNo].fRefPlus,FALSE, stPacketHead.wChannelIndex);
					refGainChangeCurve(fOldRefPlus, TRUE);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_RECV_DELAY:	//延迟
			{
				float fDelay = 0;
				memcpy( &fDelay, pData, stPacketHead.dwSize );
				if(fDelay >= MIN_DELAY[CurrentChannelNo] && fDelay <= MAX_DELAY)
				{
					ReceiveParam_ad[stPacketHead.wChannelIndex].fDelay = fDelay;
					SetDelayLableAndExecute( hBtnRecvParamDelay, stPacketHead.wChannelIndex );
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_RECV_WORK_FREQ:	//工作频率
			{
				WORD wProbeFreq = 0;
				memcpy( &wProbeFreq, pData, stPacketHead.dwSize );
				if(wProbeFreq >= MIN_LVBO_TYPE && wProbeFreq <= MAX_LVBO_TYPE)
				{
					ReceiveParam_ad[stPacketHead.wChannelIndex].nProbeFreq = wProbeFreq;
					SetLvBoLableAndExecute(hBtnRecvParamWaveBand,  stPacketHead.wChannelIndex);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_RECV_ZUKANG:	//阻抗
			{
				WORD wImpedeance = 0;
				memcpy( &wImpedeance, pData, stPacketHead.dwSize );
				if(stPacketHead.wChannelIndex == 0)
				{
					if(wImpedeance == 0 || wImpedeance == 1)
					{
						ReceiveParam_ad[stPacketHead.wChannelIndex].wImpedance = wImpedeance;
						SetImpedanceLableAndExecute(hBtnRecvParamImpedance, wImpedeance, stPacketHead.wChannelIndex, TRUE);
					}
					else
					{
						bFlag = FALSE;
					}
				}
				else if(stPacketHead.wChannelIndex == 1)
				{
					if(wImpedeance == 0 || wImpedeance == 2)
					{
						ReceiveParam_ad[stPacketHead.wChannelIndex].wImpedance = wImpedeance;
						SetImpedanceLableAndExecute(hBtnRecvParamImpedance, wImpedeance, stPacketHead.wChannelIndex, TRUE);
					}
					else
					{
						bFlag = FALSE;
					}
				}
				
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_DISP_SX:	//S/X转换
			{
				BOOL bSXflag = TRUE;
				memcpy( &bSXflag, pData, stPacketHead.dwSize );
				if(bSXflag == TRUE || bSXflag == FALSE)
				{
					DisplaySet_ad[stPacketHead.wChannelIndex].bSFlag = bSXflag;
					SetAngleMode(hBtnDisplayRefractMode, bSXflag);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_DISP_VELOCITY:	//声速
			{
				WORD wVelocity = 0;
				memcpy( &wVelocity, pData, stPacketHead.dwSize );
				if(wVelocity >= MIN_WAVE_SPEED && wVelocity <= MAX_WAVE_SPEED)
				{
					DisplaySet_ad[stPacketHead.wChannelIndex].nWaveSpeed = wVelocity;
					SetWaveSpeedLabelAndExecute( hBtnWaveSpeedValue );
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_DISP_ANGLE:	//角度
			{
				float fAngle = 0;
				memcpy( &fAngle, pData, stPacketHead.dwSize );
				if(fAngle >= MIN_ANGLE && fAngle <= MAX_ANGLE)
				{
					DisplaySet_ad[CurrentChannelNo].fZheSheJiao = fAngle;

					InitializeCriticalSection(&csProbeType);
					EnterCriticalSection(&csProbeType);
					if(DisplaySet_ad[CurrentChannelNo].fZheSheJiao == 0)
					{		
						wTanTouLeiXing[CurrentChannelNo] = 1;//直探头
					}
					else
					{
						wTanTouLeiXing[CurrentChannelNo] = 0;//斜探头
					}
					LeaveCriticalSection(&csProbeType);
					DeleteCriticalSection(&csProbeType);
					DisplaySet_ad[CurrentChannelNo].nWaveSpeed = typeToWaveSpeed(gwMaterialType);
					SetWaveSpeedLabelAndExecute(NULL);
					
					displayAngle(hBtnRefractAngleJiaoDu, DisplaySet_ad[CurrentChannelNo].fZheSheJiao);

					double fAngle = DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI;
					fKValue = (float)tan(fAngle);
					displayKValue(hBtnRefractAngleKValue, fKValue);

					SetAngleLabelAndExecute();
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_DISP_RANGE:	//声程
			{
				float fRange = 0;				
				memcpy( &fRange, pData, stPacketHead.dwSize );
				if(fRange >= 1 && fRange <= MAX_RANGE)
				{
					DisplaySet_ad[stPacketHead.wChannelIndex].fRange = fRange;
					SetRangeLableAndExecute( hBtnDisplaySoundDistance, fRange, TRUE );
				}
				
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_DISP_RESTRAIN:	//抑制
			{
				WORD wBate = 0;
				memcpy( &wBate, pData, stPacketHead.dwSize );
				if(wBate >= MIN_BATE && wBate <= MAX_BATE)
				{
					DisplaySet_ad[stPacketHead.wChannelIndex].nBate = wBate;
					SetBateLabelAndExecute( hBtnDisplayRestraint );	
				}			
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_GATE_ALARM_MODE:		//闸门报警模式
			{
				alarm_modes emAlarmMode = SuperHigh;
				memcpy( &emAlarmMode, pData, stPacketHead.dwSize );
				if(emAlarmMode == SuperHigh || emAlarmMode == LostWave)
				{
					SetAlarmMode(hBtnGateAlarmMode, stPacketHead.wGateIndex, emAlarmMode);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_GATE_SWITCH:		//闸门开关
			{
				BOOL bGateSwitch = FALSE;
				memcpy( &bGateSwitch, pData, stPacketHead.dwSize );
				if(bGateSwitch == TRUE || bGateSwitch == FALSE)
				{
					SetOpenFlag(hBtnGateSwitch, bGateSwitch, stPacketHead.wGateIndex);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_GATE_QIDIAN:	//闸门起点
			{	
				float fFront = 0.0f;
				memcpy( &fFront, pData, stPacketHead.dwSize );
				float fBack = GateInfos[stPacketHead.wChannelIndex][stPacketHead.wGateIndex-1].fBackGate;
				float fKuanDu = fBack - fFront;
				if(fFront >= 0 && fFront <= MAX_RANGE - fKuanDu)
				{
					SetQiDian( hBtnGateQiDian, stPacketHead.wGateIndex, fFront, fBack, TRUE );
				}
				else
				{
					bFlag = FALSE;
				}			
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_GATE_WIDTH:	//闸门宽度
			{
				float fWidth = 0.0f;
				memcpy( &fWidth, pData, stPacketHead.dwSize );
				float fFront = GateInfos[stPacketHead.wChannelIndex][stPacketHead.wGateIndex-1].fFrontGate;
				if(fWidth >= 0 && fWidth <= MAX_RANGE - fFront)
				{
					SetKuanDu( hBtnGateKuanDu, stPacketHead.wGateIndex, fFront, fWidth, TRUE );
				}
				else
				{
					bFlag = FALSE;
				}				
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_GATE_PEAK:		//闸门幅值
			{
				float fPeakGate = 0.0f;
				memcpy( &fPeakGate, pData, stPacketHead.dwSize );
				if(fPeakGate >=MIN_PEAK && fPeakGate <= MAX_PEAK)
				{
					SetPeakGate(hBtnGateFuZhi, stPacketHead.wGateIndex, fPeakGate, TRUE);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_OTHER_COLOR:		//颜色配置
			{
				int nColorIndex = 0;
				memcpy( &nColorIndex, pData, stPacketHead.dwSize );
				if(nColorIndex >=0 && nColorIndex <= SYS_COLOR_NUM - 1)
				{
					wchar_t strText[20];
					memset(strText, 0, sizeof(strText));
					
					// 递增颜色序号
					EnterCriticalSection(&csSysColor);
					gCurrentSysColorIndex = nColorIndex;
					gbInitializeSysColor = TRUE;;

					swprintf(strText, _T("%d"), gCurrentSysColorIndex); 
					LeaveCriticalSection(&csSysColor);

					DrawLableText(hBtnOtherCfgPrint, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);	
					SetRedrawFlag(TRUE);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_OTHER_BACK_GRUOND:		//背光
			{
				int nBackGround = 0;
				memcpy( &nBackGround , pData, stPacketHead.dwSize );
				if(nBackGround >= MIN_LCD && nBackGround <= MAX_LCD)
				{
					nLCDValue = nBackGround;
					sys_reg->wLedBrightness = nLCDValue;
					InvalidateRect(hBtnOtherCfgLcd, NULL, TRUE);
					UpdateWindow(hBtnOtherCfgLcd);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_OTHER_VOLUMN:		//音量
			{
				int nVolumn= 0;
				memcpy( &nVolumn , pData, stPacketHead.dwSize );
				if(nVolumn >= MIN_VOLUMN && nVolumn <= MAX_VOLUMN)
				{
					gnVolumn = nVolumn;
					SetVolumn(nVolumn);
					InvalidateRect(hBtnOtherCfgVolume, NULL, TRUE);
					UpdateWindow(hBtnOtherCfgVolume);
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_OTHER_STORE_MODE:		//存储介质
			{
				int nStoreMode = 0;
				memcpy( &nStoreMode , pData, stPacketHead.dwSize );
				if(nStoreMode >= MIN_TYPE && nStoreMode <= MAX_TYPE)
				{
					if(nStoreMode == 0)
					{
						if(!gbSD)
						{
							bFlag = FALSE;
						}
					}
					else if(gwStoreType == 1)
					{
						if(!gbUSB)
						{
							bFlag = FALSE;
						}
					}
					else
					{
						gwStoreType = nStoreMode;
						SetDriveType(hBtnOtherCfgStoreMode, gwStoreType);
					}
				}
				else
				{
					bFlag = FALSE;
				}
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_WAVE_REQ:		//通道波形数据请求
			{
				WORD wSocketNum = stPacketHead.wChannelIndex;
				if(wSocketNum == 0)
				{
				/*	if(gSockClient != sockClient)
					{
						gSockClient = sockClient;
					}*/
					gbChannelSend[0] = TRUE;
				}
				if(wSocketNum == 1)
				{
					if(gSockClient2 != sockClient)
					{
						gSockClient2 = sockClient;
					}
					gbChannelSend[1] = TRUE;
				}	
					
				InitializeCriticalSection(&gCs);
				EnterCriticalSection(&gCs);
					
				LeaveCriticalSection(&gCs);
				DeleteCriticalSection(&gCs);


			}
			break;
		case COMMAND_ASCAN_DATA_STROE_REQ://A扫存储数据请求  yachang.huang add on 2013-12-19
			{
				if(gSockClient3 != sockClient)
				{
					gSockClient3 = sockClient;
				}
				gbAscanDataStoreSend = TRUE;
			}
			break;
		case COMMAND_RECV_PARAMS:	// 接收参数
			{
				wchar_t strText[20];
				memset(strText, 0, sizeof(strText));
				swprintf(strText, _T("%d"), stPacketHead.dwCommand);
				PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)strText);
				RECEIVE_PARAM_DATA recvParams;
				memcpy( &recvParams, pData, sizeof(RECEIVE_PARAM_DATA));

				ReceiveParam_ad[stPacketHead.wChannelIndex] = recvParams;

				SetPlusLableAndExecute( hBtnRecvParamGain, ReceiveParam_ad[stPacketHead.wChannelIndex].fPlus, 
										ReceiveParam_ad[stPacketHead.wChannelIndex].fRefPlus, TRUE, stPacketHead.wChannelIndex );			
			}
			break;
		case COMMAND_ALL_PARAM_REQ:	// 接收全部参数
			{
				
			}
		case COMMAND_ALL_PARAM_SET:	// 设置参数SETONE
			{
				// Song Chenguang test
//				MessageBox(NULL, _T("Enter COMMAND_ALL_PARAM_SET"), _T("ndt_ce"), MB_OK);
				///////////////////////////////////////////////////////////
				CSCAN_ARS200FPGA_SETONE stSetOne;
				memcpy( &stSetOne, pData, stPacketHead.dwSize );
//				stPacketHead.wChannelIndex = 1;//yachang.huang 2013-11-01  强制改通道为2通道
				CurrentChannelNo = stPacketHead.wChannelIndex;

				SetOne(stSetOne, stPacketHead.wChannelIndex);
				if(gbGuanLian)// 关联时由下位机分通道设置另一个通道
				{
					// Song Chenguang test
//					MessageBox(NULL, _T("Guanlian=true COMMAND_ALL_PARAM_SET"), _T("ndt_ce"), MB_OK);
					///////////////////////////////////////////////////////////

					if(stPacketHead.wChannelIndex == 0)
					{
						SetOne(stSetOne, 1);
						memcpy(&gstSetOne[1],&stSetOne, stPacketHead.dwSize);
					}
					else if(stPacketHead.wChannelIndex == 1)
					{
						SetOne(stSetOne, 0);
						memcpy(&gstSetOne[0],&stSetOne, stPacketHead.dwSize);
					}
					else
					{
						;//
					}
				}
				memcpy(&gstSetOne[CurrentChannelNo],&stSetOne, stPacketHead.dwSize);
		
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);	

				// Song Chenguang test
//				MessageBox(NULL, _T("Leave COMMAND_ALL_PARAM_SET"), _T("ndt_ce"), MB_OK);
				///////////////////////////////////////////////////////////
			}
			break;
		case COMMAND_WORK_MODE:
			{	
				// Song Chenguang test				
//				MessageBox(NULL, _T("Enter COMMAND_WOKE_MODE"), _T("ndt_ce"), MB_OK);
				///////////////////////////////////////////////////////////

				WORD wRelay = 0;
				memcpy( &gwCscanWorkMode, pData, stPacketHead.dwSize );

				// Song Chenguang test
//				wchar_t strText[10];
//				memset(strText, 0, sizeof(strText));	
//				wsprintf(strText, _T("%d"), gwCscanWorkMode);
//				MessageBox(NULL, strText, _T("ndt_ce"), MB_OK);
				////////////////////////////////////////////////////////////
				if(gwCscanWorkMode == 0)
				{
					cscan_sys_reg->wWorkMode = 0;	//关闭
					
					gbGuanLian = FALSE;
				}
				else if(gwCscanWorkMode == 1)	//单1
				{
					cscan_sys_reg->wWorkMode = SingleChannelA;

					*ch2_associate_detection = 0;//yachang.huang add on 2013-12-11
					gbGuanLian = FALSE;
				}
				else if(gwCscanWorkMode == 2)	//单2
				{
					cscan_sys_reg->wWorkMode = SingleChannelB;

					*ch2_associate_detection = 0;//yachang.huang add on 2013-12-11
					gbGuanLian = FALSE;
				}
				else if(gwCscanWorkMode == 3)	//双单步进/双单深度 单晶（双通道） 
				{
					cscan_sys_reg->wWorkMode = DoubleChannels;


					gSysInfo.wRelay &= 0xFFFC;
					//yachang.huang rum on 2013-11-19
					//sys_reg->wRelay = gSysInfo.wRelay;

					*ch2_associate_detection = 0;//yachang.huang add on 2013-12-11
					gbGuanLian = FALSE; 
				}
				else if(gwCscanWorkMode == 4)	//双晶反射（一发一收）
				{
					cscan_sys_reg->wWorkMode = OneSendOneReceive;
					
					gSysInfo.wRelay &= 0xFFFD;
					gSysInfo.wRelay |= 0x1;
					//yachang.huang rum on 2013-11-19
					//sys_reg->wRelay = gSysInfo.wRelay;
					*ch2_associate_detection = 0;//yachang.huang add on 2013-12-11
					gbGuanLian = TRUE;
				}
				else if(gwCscanWorkMode == 5)	//双晶对射（一发一收）
				{
					cscan_sys_reg->wWorkMode = OneSendOneReceive;

					gSysInfo.wRelay &= 0xFFFD;
					gSysInfo.wRelay |= 0x1;
					//yachang.huang rum on 2013-11-19
					//sys_reg->wRelay = gSysInfo.wRelay;

					*ch2_associate_detection = 0;//yachang.huang add on 2013-12-11
					
					gbGuanLian = TRUE;
				}
				else if(gwCscanWorkMode == 6)	//单晶关联
				{
					cscan_sys_reg->wWorkMode = DanJingGuanLian;

				
					gSysInfo.wRelay &= 0xFFFE;
					gSysInfo.wRelay |= 0x2;
					//yachang.huang rum on 2013-11-19
					//sys_reg->wRelay = gSysInfo.wRelay;

					*ch2_associate_detection = 1;//yachang.huang add on 2013-12-11
					gbGuanLian = TRUE;
				}
				else if(gwCscanWorkMode == 7)	//双晶反射关联（双晶关联）
				{					
					cscan_sys_reg->wWorkMode = ShuangJingGuanLian;


					gSysInfo.wRelay |= 0x3;
					//yachang.huang rum on 2013-11-19
					//sys_reg->wRelay = gSysInfo.wRelay;
					*ch2_associate_detection = 0;//yachang.huang add on 2013-12-11
					gbGuanLian = TRUE;
				}
				else if(gwCscanWorkMode == 8)	//双晶对射关联（双晶关联）
				{
					cscan_sys_reg->wWorkMode = ShuangJingGuanLian;

					
					gSysInfo.wRelay |= 0x3;
					//yachang.huang rum on 2013-11-19
					//sys_reg->wRelay = gSysInfo.wRelay;
					*ch2_associate_detection = 0;//yachang.huang add on 2013-12-11
					gbGuanLian = TRUE;
				}
				else
				{
					;//
				}
				
			/*	if(gbGuanLian) //另一通道的参数设置成与当前通道一样
				{
					if(CurrentChannelNo == 0)
					{
						SetOne(gstSetOne[1], 1);	
					}
					else
					{
						SetOne(gstSetOne[0], 0);	
					}
				}*/
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);

				// Song Chenguang test
//				MessageBox(NULL, _T("Leave COMMAND_WORK_MODE"), _T("ndt_ce"), MB_OK);
				///////////////////////////////////////////////////////////
			}
			break;
		case COMMAND_RESET:	// 复位
			{
				cscan_code_reg->wReset = 1;
				Sleep(1);
				cscan_code_reg->wReset = 0;
				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		case COMMAND_VERSION_REQ:
			{
				CSCAN_VERSION_DATA_t stVersion;
				WORD wHardWareVer = Sys_Hardware_Ver->wHardwareVer;
				float fHardWareVer = 0;
				float fDot2 = wHardWareVer & 0xF;			//小数点第二位
				float fDot1 = (wHardWareVer >> 4) & 0xF;	//小数点第一位
				int nGeWei	= (wHardWareVer >> 8) & 0xF;	//个位
				int nShiWei	= (wHardWareVer >> 12) & 0xF;	//十位
				fHardWareVer = nShiWei * 10 + nGeWei + fDot1 / 10 + fDot2 / 100;
				stVersion.fHardWareVer = fHardWareVer;
				stVersion.fSoftWareVer = SOFTWARE_VER;
				std::string version(SOFTWARE_VER_STR);
				memset(stVersion.versionStr, 0, sizeof(stVersion.versionStr));
				memcpy_s(stVersion.versionStr, sizeof(stVersion.versionStr), version.c_str(), version.length());

				PACKET_HEAD_t stPacketHead;
				memset(&stPacketHead, 0, sizeof(stPacketHead));
				stPacketHead.dwCommand = COMMAND_VERSION_REQ;
				stPacketHead.wChannelIndex = 0;
				stPacketHead.wGateIndex = 0;
				stPacketHead.dwSize = sizeof(CSCAN_VERSION_DATA_t);

				char szSendData[BUFFER_SIZE];
				memset( szSendData, 0, sizeof(szSendData) );
				memcpy(szSendData, &stPacketHead, NETHEAD_SIZE);
				memcpy(&szSendData[NETHEAD_SIZE], &stVersion, stPacketHead.dwSize);

				if ( send( sockClient, szSendData, NETHEAD_SIZE + stPacketHead.dwSize, 0 ) == SOCKET_ERROR )	
			 	{
			 		int nRt =  WSAGetLastError();
					wchar_t strText[20];
					memset(strText, 0, sizeof(strText));
					swprintf(strText, _T("%d"), nRt);
					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)strText);
			 	}
			}
			break;
		case COMMAND_TCG_SWICH:
			{
				TCG_COMMU_DATA_t stTcgRecv;
				int nSize = sizeof(stTcgRecv);
				wchar_t strText[20];
				wsprintf(strText, _T("stTcgRecv: %d\n"), nSize);
				OutputDebugString(strText);
				memset( &stTcgRecv, 0, sizeof(stTcgRecv));
				memcpy(&stTcgRecv, pData, stPacketHead.dwSize);

				WORD* cscan_chCurrent_dac_reg = NULL;
				WORD *cscan_chCurrent_tgc = NULL;
				WORD *cscan_chCurrent_tgc_sampling_interval = NULL;
				if(stPacketHead.wChannelIndex == 0)
				{
					cscan_chCurrent_dac_reg = cscan_ch1_dac_reg;
					cscan_chCurrent_tgc = cscan_ch1_tgc;
					cscan_chCurrent_tgc_sampling_interval = cscan_ch1_sampling_interval_tgc;
				}
				else
				{
					cscan_chCurrent_dac_reg = cscan_ch2_dac_reg;
					cscan_chCurrent_tgc = cscan_ch2_tgc;
					cscan_chCurrent_tgc_sampling_interval = cscan_ch2_sampling_interval_tgc;
				}
				
				if(stTcgRecv.bOpenFlag  == FALSE)
				{
					*cscan_chCurrent_dac_reg = 0;
				}
				else
				{
					WORD* pTcgData = cscan_chCurrent_tgc;
					for(int i = 0; i < 1024; i++)
					{
						*pTcgData = DACdBProcess(stTcgRecv.fSramGain[i]);
						pTcgData++;
					}
					Sleep(1);
					*cscan_chCurrent_dac_reg = 1;
					*cscan_chCurrent_tgc_sampling_interval = stTcgRecv.wSamplingInterval;
				}

				RequestRespond(sockClient, pData, stPacketHead.dwSize, bFlag);
			}
			break;
		default:
			break;
	}
		
	return;
}

BOOL CheckRecvParams(RECEIVE_PARAM_DATA recvParams)
{
	if(!(recvParams.nProbeFreq >= MIN_LVBO_TYPE && recvParams.nProbeFreq <= MAX_LVBO_TYPE) ||
	   !(recvParams.fDelay >= MIN_DELAY[0] && recvParams.fDelay <= MAX_DELAY) ||
	   !(recvParams.fPlus >= MIN_PLUS && recvParams.fPlus <= MAX_PLUS) ||
	   !(recvParams.fRefPlus >= MIN_PLUS && recvParams.fRefPlus <= MAX_PLUS) ||
	   !(recvParams.nJianBoMode >= 0 && recvParams.nJianBoMode <= 3) ||
	   !(recvParams.nLvBo >= 0 && recvParams.nLvBo <= 255))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void SetRecvParams()
{
}
//请求应答
void RequestRespond(SOCKET sockClient, char *pData,DWORD dwDataSize, BOOL bFlag)
{

//因为校验不全，暂时注掉弹框  yachang.huang  modify  on 2013-11-29

	if(bFlag)
	{
		// 应答
		char szSendData[BUFFER_SIZE];
		memset( szSendData, 0, sizeof(szSendData) );
		memcpy(szSendData, pData - NETHEAD_SIZE, NETHEAD_SIZE);
		memcpy(&szSendData[NETHEAD_SIZE], pData, dwDataSize);

		if ( send( sockClient, szSendData, NETHEAD_SIZE + dwDataSize, 0 ) == SOCKET_ERROR )	
	 	{
	 		int nRt =  WSAGetLastError();
			wchar_t strText[10];
			memset(strText, 0, sizeof(strText));
			swprintf(strText, _T("%d Send失败"), nRt);
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)strText);
			// Song test
//			MessageBox(NULL, _T("Send fail"), _T("ndt"), MB_OK);
			/////////////////////
	 	}
//		else
			// Song test
//			MessageBox(NULL, _T("Send SUCCESS"), _T("ndt"), MB_OK);
		/////////////////////
	}
	else //数值不在范围之内
	{
		// Song test
	//	MessageBox(NULL, _T("bFlag = fALSE"), _T("ndt"), MB_OK);
		/////////////////////
		char szSendData[BUFFER_SIZE];
		memset( szSendData, 0, sizeof(szSendData) );

		PACKET_HEAD_t stPacketHead;
		memset(&stPacketHead, 0, sizeof(stPacketHead));
		stPacketHead.dwCommand = COMMAND_DATA_FAULT;
		stPacketHead.wChannelIndex = 0;
		stPacketHead.wGateIndex = 0;
		stPacketHead.dwSize = 0;
		memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));

		if ( send( sockClient, szSendData, NETHEAD_SIZE, 0 ) == SOCKET_ERROR )	
	 	{
	 		int nRt =  WSAGetLastError();
			wchar_t strText[20];
			memset(strText, 0, sizeof(strText));
			swprintf(strText, _T("%d"), nRt);
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)strText);
	 	}
	}	
}
void ChangeChannel(BOOL bSaveFlag, int nChnNo)
{
	if( nChnNo >= 0 && nChnNo <= 1)		//切换成某通道,非按切换键
	{
		if(CurrentChannelNo != nChnNo)
		{
			if(nChnNo == 0)
			{
				CurrentChannelNo = 1;
			}
			else
			{
				CurrentChannelNo = 0;
			}
		}
		else
		{
			return;
		}
	}
		
	if(gwDeviceType == 0 || gwDeviceType == 2)//探伤有通道切换, 测厚无通道切换
	{
		if(CurrentChannelNo == 0)
		{
			CurrentChannelNo = 1;
			chCurrent_data_reg = ch2_data_reg;
			chCurrent_gate_reg = ch2_gate_reg;
			chCurrent_con_reg = ch2_con_reg;
			chCurrent_interval = ch2_interval;
			chCurrent_data = ch2_data;

			chCurrent_plus_reg = ch2_plus_reg;
			chCurrent_dac_reg = ch2_dac_reg;

			chCurrent_gate1curve = ch2_gate1curve;
			chCurrent_gate2curve = ch2_gate2curve;
			chCurrent_gate3curve = ch2_gate3curve;

			chCurrent_tgc = ch2_tgc;//wangbingfu add 2010-06-17

			//wangbingfu add start at 2010-09-08
			if(gSysInfo.wWorkMode != DoubleChannels)
			{
				if(ReceiveParam_ad[1].wImpedance == 0)
				{
					gSysInfo.wRelay &= 0xFFFC;
					//yachang.huang rum on 2013-11-19
					//sys_reg->wRelay = gSysInfo.wRelay;							
				}
				else if(ReceiveParam_ad[1].wImpedance == 2)
				{
					gSysInfo.wRelay &= 0xFFFC;
					gSysInfo.wRelay |= 0x2;
					//yachang.huang rum on 2013-11-19
					//sys_reg->wRelay = gSysInfo.wRelay;
				}
				if(hBtnRecvParamImpedance)
					displayImpedance(hBtnRecvParamImpedance, 1, ReceiveParam_ad[1].wImpedance);
			}
			//wangbingfu add end at 2010-09-08

			if(gSysInfo.wWorkMode == SingleChannelA)
			{
				gSysInfo.wWorkMode = SingleChannelB;
				sys_reg->wWorkMode = SingleChannelB;
			}

			gSysInfo.wCurrentChNo = 1;
		}
		else if(CurrentChannelNo == 1)
		{
			CurrentChannelNo = 0;
			chCurrent_data_reg = ch1_data_reg;
			chCurrent_gate_reg = ch1_gate_reg;
			chCurrent_con_reg = ch1_con_reg;
			chCurrent_interval = ch1_interval;
			chCurrent_data = ch1_data;

			chCurrent_plus_reg = ch1_plus_reg;
			chCurrent_dac_reg = ch1_dac_reg;

			chCurrent_gate1curve = ch1_gate1curve;
			chCurrent_gate2curve = ch1_gate2curve;
			chCurrent_gate3curve = ch1_gate3curve;

			chCurrent_tgc = ch1_tgc;//wangbingfu add 2010-06-17
			//wangbingfu add start at 2010-09-08
			if(gSysInfo.wWorkMode != DoubleChannels)
			{
				if(ReceiveParam_ad[0].wImpedance == 0)
				{
					gSysInfo.wRelay &= 0xFFFC;
					//yachang.huang rum on 2013-11-19
					//sys_reg->wRelay = gSysInfo.wRelay;							
				}
				else if(ReceiveParam_ad[0].wImpedance == 1)
				{
					gSysInfo.wRelay &= 0xFFFC;
					gSysInfo.wRelay |= 0x1;
					//yachang.huang rum on 2013-11-19
					//sys_reg->wRelay = gSysInfo.wRelay;
				}

				if(hBtnRecvParamImpedance)	
					displayImpedance(hBtnRecvParamImpedance, 0, ReceiveParam_ad[0].wImpedance);
			}
			//wangbingfu add end at 2010-09-08
			
			if(gSysInfo.wWorkMode == SingleChannelB)
			{
				gSysInfo.wWorkMode = SingleChannelA;
				sys_reg->wWorkMode = SingleChannelA;
			}

			gSysInfo.wCurrentChNo = 0;
		}

		memset(gszTextChannel, 0, sizeof(gszTextChannel));
		if(CurrentChannelNo == 0)
		{
			swprintf(gszTextChannel, _T("1"));
		}
		else if(CurrentChannelNo == 1)
		{
			swprintf(gszTextChannel, _T("2"));
		}

		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

		SetRedrawFlag(TRUE);			

		memset(gszTextPlus, 0, sizeof(gszTextPlus));
		swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

		memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
		swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
		
		//切换抑制图标
		memset(gszTextPeak, 0, sizeof(gszTextPeak));
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);

		SetRangeLableAndExecute(NULL, DisplaySet_ad[CurrentChannelNo].fRange, bSaveFlag);
	}
}
/*void SendAllParams()
{
	WIRELESS_ALL_PARAM_t stSendAllParam;
	memset(&stSendAllParam, 0, sizeof(stSendAllParam));
	stSendAllParam.wCurChannel = CurrentChannelNo;
	stSendAllParam.wWorkMode = gSysInfo.wWorkMode;
	stSendAllParam.stSendParam[0].fQianYan
}*/
void SetOne(CSCAN_ARS200FPGA_SETONE stSetOne , WORD wChannelNo)
{
	//电压
// 	WORD nPress = WORD(stSetOne.wVoltage / 400.0f * 255.0f);
// 	send_mcp41010(nPress);


	SetVoltage(stSetOne.wVoltage);//yachang.huang modify on 2013-11-22
	SetRevAndSendMode(stSetOne.wRevAndSendMode,wChannelNo);//yachang.huang modify on 2013-11-22
    SetSPI(stSetOne.wSPIAddr,stSetOne.wSPIData,wChannelNo);//yachang.huang modify on 2013-11-22
	SetEnerge(stSetOne.wEnerge,wChannelNo);//yachang.huang add on 2013-11-26
	WORD wInterval[DATA_SIZE];
	memset(wInterval, 0, sizeof(wInterval));
	if(gwCscanWorkMode == 5 || gwCscanWorkMode == 8)
	{
		RangeProcess(stSetOne.nVelocity, stSetOne.fRange / 2, wInterval);	//计算采样间隔
	}
	else
	{
		RangeProcess(stSetOne.nVelocity, stSetOne.fRange, wInterval);	//计算采样间隔
	}
	SetProbeBand(stSetOne.wProbeBand, wChannelNo);	//设置探头频率
	SetGain(stSetOne, wChannelNo);		//设置增益
	SetImpedance(stSetOne.wImpedance,wChannelNo);  //设置阻抗
	SetJianBoMode(stSetOne.stConfig.wDetectMode,wChannelNo);  //设置检波模式
	if(wChannelNo == 0)
	{
		cscan_ch1_con_reg->wRepeatCounter = stSetOne.stCon.wRepeatCounter;
		cscan_ch1_con_reg->wMaiKuan = stSetOne.stCon.wMaiKuan;
	//	cscan_ch1_config_reg->wDetectMode = stSetOne.stConfig.wDetectMode;
	//	cscan_ch1_config_reg->wValueMiddle = stSetOne.stConfig.wValueMiddle;
		cscan_ch1_config_reg->wValueMiddle = gMidValue[wChannelNo];
		cscan_ch1_config_reg->wSuspend = stSetOne.stConfig.wSuspend;
		cscan_ch1_config_reg->wMonitorZeroCounter = stSetOne.stConfig.wMonitorZeroCounter;
		cscan_ch1_gate_reg->wGate1Enable = stSetOne.stGate.wGate1Enable;
		cscan_ch1_gate_reg->wGate1SyncMode = stSetOne.stGate.wGate1SyncMode;
		cscan_ch1_gate_reg->wGate1SyncThreshold = stSetOne.stGate.wGate1SyncThreshold;
		cscan_ch1_gate_reg->wGate1Start = stSetOne.stGate.wGate1Start;
		cscan_ch1_gate_reg->wGate1Width = stSetOne.stGate.wGate1Width;
		cscan_ch1_gate_reg->wGate1Amp = stSetOne.stGate.wGate1Amp;
		cscan_ch1_gate_reg->wGate2Enable = stSetOne.stGate.wGate2Enable;
		cscan_ch1_gate_reg->wGate2SyncMode = stSetOne.stGate.wGate2SyncMode;
		cscan_ch1_gate_reg->wGate2SyncThreshold = stSetOne.stGate.wGate2SyncThreshold;
		cscan_ch1_gate_reg->wGate2Start = stSetOne.stGate.wGate2Start;
		cscan_ch1_gate_reg->wGate2Width = stSetOne.stGate.wGate2Width;
		cscan_ch1_gate_reg->wGate2Amp = stSetOne.stGate.wGate2Amp ;
		cscan_ch1_gate_reg->wGate3Enable = stSetOne.stGate.wGate3Enable;
		cscan_ch1_gate_reg->wGate3SyncMode = stSetOne.stGate.wGate3SyncMode;
		cscan_ch1_gate_reg->wGate3SyncThreshold = stSetOne.stGate.wGate3SyncThreshold;
		cscan_ch1_gate_reg->wGate3Start = stSetOne.stGate.wGate3Start;
		cscan_ch1_gate_reg->wGate3Width = stSetOne.stGate.wGate3Width;
		cscan_ch1_gate_reg->wGate3Amp = stSetOne.stGate.wGate3Amp;
		cscan_ch1_gate_reg->wGate4Enable = stSetOne.stGate.wGate4Enable;
		cscan_ch1_gate_reg->wGate4SyncMode = stSetOne.stGate.wGate4SyncMode;
		cscan_ch1_gate_reg->wGate4SyncThreshold = stSetOne.stGate.wGate4SyncThreshold;
		cscan_ch1_gate_reg->wGate4Start = stSetOne.stGate.wGate4Start;
		cscan_ch1_gate_reg->wGate4Width = stSetOne.stGate.wGate4Width;
		cscan_ch1_gate_reg->wGate4Amp = stSetOne.stGate.wGate4Amp;
		cscan_ch1_gate_reg->wSimpleInterval = stSetOne.stGate.wSimpleInterval;
		cscan_ch1_gate_reg->wSimpleAverage = stSetOne.stGate.wSimpleAverage;
		cscan_ch1_gate_reg->wDelay = stSetOne.stGate.wDelay;

		memcpy(cscan_ch1_interval, wInterval, sizeof(wInterval));		//采样间隔
	}
	else if(wChannelNo == 1)
	{
		cscan_ch2_con_reg->wRepeatCounter = stSetOne.stCon.wRepeatCounter;
		cscan_ch2_con_reg->wMaiKuan = stSetOne.stCon.wMaiKuan;
	//	cscan_ch2_config_reg->wDetectMode = stSetOne.stConfig.wDetectMode;	
	//	cscan_ch2_config_reg->wValueMiddle = stSetOne.stConfig.wValueMiddle;
		cscan_ch2_config_reg->wValueMiddle = gMidValue[wChannelNo];;
		cscan_ch2_config_reg->wSuspend = stSetOne.stConfig.wSuspend;
		cscan_ch2_config_reg->wMonitorZeroCounter = stSetOne.stConfig.wMonitorZeroCounter;
		cscan_ch2_gate_reg->wGate1Enable = stSetOne.stGate.wGate1Enable;
		cscan_ch2_gate_reg->wGate1SyncMode = stSetOne.stGate.wGate1SyncMode;
		cscan_ch2_gate_reg->wGate1SyncThreshold = stSetOne.stGate.wGate1SyncThreshold;
		cscan_ch2_gate_reg->wGate1Start = stSetOne.stGate.wGate1Start;
		cscan_ch2_gate_reg->wGate1Width = stSetOne.stGate.wGate1Width;
		cscan_ch2_gate_reg->wGate1Amp = stSetOne.stGate.wGate1Amp;
		cscan_ch2_gate_reg->wGate2Enable = stSetOne.stGate.wGate2Enable;
		cscan_ch2_gate_reg->wGate2SyncMode = stSetOne.stGate.wGate2SyncMode;
		cscan_ch2_gate_reg->wGate2SyncThreshold = stSetOne.stGate.wGate2SyncThreshold;
		cscan_ch2_gate_reg->wGate2Start = stSetOne.stGate.wGate2Start;
		cscan_ch2_gate_reg->wGate2Width = stSetOne.stGate.wGate2Width;
		cscan_ch2_gate_reg->wGate2Amp = stSetOne.stGate.wGate2Amp ;
		cscan_ch2_gate_reg->wGate3Enable = stSetOne.stGate.wGate3Enable;
		cscan_ch2_gate_reg->wGate3SyncMode = stSetOne.stGate.wGate3SyncMode;
		cscan_ch2_gate_reg->wGate3SyncThreshold = stSetOne.stGate.wGate3SyncThreshold;
		cscan_ch2_gate_reg->wGate3Start = stSetOne.stGate.wGate3Start;
		cscan_ch2_gate_reg->wGate3Width = stSetOne.stGate.wGate3Width;
		cscan_ch2_gate_reg->wGate3Amp = stSetOne.stGate.wGate3Amp;
		cscan_ch2_gate_reg->wGate4Enable = stSetOne.stGate.wGate4Enable;
		cscan_ch2_gate_reg->wGate4SyncMode = stSetOne.stGate.wGate4SyncMode;
		cscan_ch2_gate_reg->wGate4SyncThreshold = stSetOne.stGate.wGate4SyncThreshold;
		cscan_ch2_gate_reg->wGate4Start = stSetOne.stGate.wGate4Start;
		cscan_ch2_gate_reg->wGate4Width = stSetOne.stGate.wGate4Width;
		cscan_ch2_gate_reg->wGate4Amp = stSetOne.stGate.wGate4Amp;
		cscan_ch2_gate_reg->wSimpleInterval = stSetOne.stGate.wSimpleInterval;
		cscan_ch2_gate_reg->wSimpleAverage = stSetOne.stGate.wSimpleAverage;
		cscan_ch2_gate_reg->wDelay = stSetOne.stGate.wDelay;

		memcpy(cscan_ch2_interval, wInterval, sizeof(wInterval));	//采样间隔
	}
	else
	{
	}
}
void SetGain(CSCAN_ARS200FPGA_SETONE stSetOne , WORD wChannelNo)
{
	/*
	通道1： 

	0-31.9dB——0x18040006写入1，0x18060008写入值：(Gain + 4)/40×1023；（Gain为面板增益值，下同），0x18060006写入4/40 × 1023;
	32-63.9dB——0x18040006写入2，0x18060008写入值：(Gain+X11+ 4 - 32)/40×1023，0x18060006写入4/40 × 1023；
	64 - (83.9-X12)dB——0x18040006写入4，0x18060008写入值：(Gain +X12 + 4 - 48)/40×1023，0x18060006写入4/40 × 1023；
	(84 -X12)-110dB——0x18040006写入4，0x18060008写入值：1023，0x18060006写入(Gain + 4 –(84 -X12))/40×1023；

	通道2：  

	0-31.9dB——0x18080006写入1，0x180A000C写入值：(Gain + 4)/40×1023；（Gain为面板增益值，下同），0x180A000A写入4/40 × 1023;
	32-63.9dB——0x18080006写入2，0x180A000C写入值：(Gain+X21+ 4 - 32)/40×1023，0x180A000A写入4/40 × 1023；
	64 - (83.9-X22)dB——0x18080006写入4，0x180A000C写入值：(Gain +X22 + 4 - 48)/40×1023，0x180A000A写入4/40 × 1023；
	(84 -X22)-110dB——0x18080006写入4，0x180A000C写入值：1023，0x180A000A写入(Gain + 4 –(84 -X22))/40×1023；
	*/

	if(wChannelNo == 0)
	{
		float fTempPlus = 0;
		if (!gbGuanLian)
		{
			 fTempPlus = stSetOne.fPlus + stSetOne.fRefPlus;
		}
		else
		{
			 fTempPlus = stSetOne.fPlus;
		}
		if (fTempPlus >= 0 && fTempPlus <= 31.9)
		{
			*ch1_opa3692_simulate_switch = 1;
			Sleep(1);
			cscan_ch1_config_reg->wdBRef = 4  * 1023 / 40;
			cscan_ch1_config_reg->wdBLevel1 = (fTempPlus + 4)/ 40 * 1023;
		}
		else if (fTempPlus >= 32.0 && fTempPlus <= 63.9)
		{
			*ch1_opa3692_simulate_switch = 2;
			Sleep(1);
			cscan_ch1_config_reg->wdBRef =  4  * 1023 / 40;
		//	cscan_ch1_config_reg->wdBLevel1 = ((fTempPlus + stSetOne.fPlusAddX11 - 32 + 4)* 1023) / 40 ;
			cscan_ch1_config_reg->wdBLevel1 = ((fTempPlus + gfX11[stSetOne.wProbeBand] - 32 + 4)* 1023) / 40 ;
		}
		else if (fTempPlus >= 64.0 && fTempPlus <= (83.9 - stSetOne.fPlusAddX12))
		{
			*ch1_opa3692_simulate_switch = 4;
			cscan_ch1_config_reg->wdBRef =  4  * 1023 / 40;
		//	cscan_ch1_config_reg->wdBLevel1 = ((fTempPlus + stSetOne.fPlusAddX12 - 48 + 4) * 1023) / 40;
			cscan_ch1_config_reg->wdBLevel1 = ((fTempPlus + gfX12[stSetOne.wProbeBand] - 48 + 4) * 1023) / 40;
		}
		else if (fTempPlus >= (84.0 - stSetOne.fPlusAddX12) && fTempPlus <= 110.0)
		{
			*ch1_opa3692_simulate_switch = 4;
			Sleep(1);
		//	cscan_ch1_config_reg->wdBRef =  ((fTempPlus - (84.0 -stSetOne.fPlusAddX12) + 4)  * 1023) / 40;
			cscan_ch1_config_reg->wdBRef =  ((fTempPlus - (84.0 - gfX12[stSetOne.wProbeBand]) + 4)  * 1023) / 40;
			cscan_ch1_config_reg->wdBLevel1 =  1023;
		}
		else
		{

		}
	}
	else if(wChannelNo == 1)
	{
		if (gbGuanLian)
		{
			float fTempPlusGuanLian = stSetOne.fPlus;
			float fTempPlusTotal = stSetOne.fPlus + stSetOne.fRefPlus;
			if (fTempPlusGuanLian >= 0 && fTempPlusGuanLian <= 31.9)
			{
				float fTempPlusGuanLian0 = fTempPlusTotal + 4;

				if (fTempPlusGuanLian0 <= 40.0)
				{
					cscan_ch2_config_reg->wdBLevel1 = static_cast<WORD>(((fTempPlusTotal + 4) * 1023 ) / 40);
					cscan_ch2_config_reg->wdBRef = (4 * 1023)/ 40 ;	
				}
				else if (fTempPlusGuanLian0 > 40.0 && fTempPlusGuanLian0 <= 76.0)
				{
					cscan_ch2_config_reg->wdBLevel1 = 1023;
					cscan_ch2_config_reg->wdBRef = static_cast<WORD>(((fTempPlusTotal - 32) * 1023 ) / 40) ;	
				}
				else if (fTempPlusGuanLian0 > 76.0)
				{
					cscan_ch2_config_reg->wdBLevel1 = 1023;
					cscan_ch2_config_reg->wdBRef = 1023;	
				}


			}
			else if (fTempPlusGuanLian >= 32.0 && fTempPlusGuanLian <= 63.9)
			{
			//	float fTempPlusGuanLian32 = fTempPlusTotal + 4 + stSetOne.fPlusAddX21 - 32;
				float fTempPlusGuanLian32 = fTempPlusTotal + 4 + gfX21[stSetOne.wProbeBand] - 32;
				if (fTempPlusGuanLian32 <= 40.0)
				{
					cscan_ch2_config_reg->wdBLevel1 = static_cast<WORD>((fTempPlusGuanLian32 * 1023 ) / 40);
					cscan_ch2_config_reg->wdBRef = (4 * 1023) / 40 ;	
				}
				else if (fTempPlusGuanLian32 > 40.0 && fTempPlusGuanLian32 <= 76.0)
				{
					cscan_ch2_config_reg->wdBLevel1 = 1023 ;
					cscan_ch2_config_reg->wdBRef = static_cast<WORD>(((fTempPlusGuanLian32 - 36) * 1023 )/ 40);	
				}
				else if (fTempPlusGuanLian32 > 76.0)
				{
					cscan_ch2_config_reg->wdBLevel1 = 1023 ;
					cscan_ch2_config_reg->wdBRef = 1023;
				}

			}
			else if (fTempPlusGuanLian >= 64.0)
			{
				//float fTempPlusGuanLian64 = fTempPlusTotal + stSetOne.fPlusAddX22 + 4 - 48;
				float fTempPlusGuanLian64 = fTempPlusTotal + gfX22[stSetOne.wProbeBand] + 4 - 48;
				if (fTempPlusGuanLian64 <= 40)
				{
					cscan_ch2_config_reg->wdBLevel1 = static_cast<WORD>((fTempPlusGuanLian64 * 1023 ) / 40);
					cscan_ch2_config_reg->wdBRef = (4 * 1023) / 40 ;	
				}
				else if (fTempPlusGuanLian64 > 40 && fTempPlusGuanLian64 <= 76)
				{
					cscan_ch2_config_reg->wdBLevel1 = 1023 ;
					cscan_ch2_config_reg->wdBRef = static_cast<WORD>(((fTempPlusGuanLian64 - 36) * 1023 ) / 40);	
				}
				else if (fTempPlusGuanLian64 > 76)
				{
					cscan_ch2_config_reg->wdBLevel1 = 1023 ;
					cscan_ch2_config_reg->wdBRef = 1023;
				}
			}
		}
		else
		{
			float fTempPlus = stSetOne.fPlus + stSetOne.fRefPlus;

			if (fTempPlus >= 0 && fTempPlus <= 31.9)
			{
				*ch2_opa3692_simulate_switch = 1;
				Sleep(1);
				cscan_ch2_config_reg->wdBRef = 4  * 1023 / 40;;
				cscan_ch2_config_reg->wdBLevel1 = ((fTempPlus + 4) * 1023) / 40;
			}
			else if (fTempPlus >= 32 && fTempPlus <= 63.9)
			{
				*ch2_opa3692_simulate_switch = 2;
				Sleep(1);
				cscan_ch2_config_reg->wdBRef = 4  * 1023 / 40;;
			//	cscan_ch2_config_reg->wdBLevel1 = ((fTempPlus + stSetOne.fPlusAddX21 - 32  + 4) * 1023) / 40;
				cscan_ch2_config_reg->wdBLevel1 = ((fTempPlus + gfX21[stSetOne.wProbeBand] - 32  + 4) * 1023) / 40;
			}
			else if (fTempPlus >= 64 && fTempPlus <= (83.9 - stSetOne.fPlusAddX22))
			{
				*ch2_opa3692_simulate_switch = 4;
				Sleep(1);
				cscan_ch2_config_reg->wdBRef = 4  * 1023 / 40;;
			//	cscan_ch2_config_reg->wdBLevel1 = ((fTempPlus + stSetOne.fPlusAddX22 - 48 + 4) * 1023) / 40;
				cscan_ch2_config_reg->wdBLevel1 = ((fTempPlus + gfX22[stSetOne.wProbeBand]  - 48 + 4) * 1023) / 40;
			}
			else if (fTempPlus >= (84.0 - stSetOne.fPlusAddX22) && fTempPlus <= 110)
			{
				*ch2_opa3692_simulate_switch = 4;
				Sleep(1);
			//	cscan_ch2_config_reg->wdBRef =  ((fTempPlus - (84.0 - stSetOne.fPlusAddX22) + 4) * 1023) / 40;
				cscan_ch2_config_reg->wdBRef =  ((fTempPlus - (84.0 - gfX22[stSetOne.wProbeBand]) + 4) * 1023) / 40;
				cscan_ch2_config_reg->wdBLevel1 =  1023;
			}
			else
			{

			}
	
		}

	}
}
	


/*  旧版本设置增益函数 yachang.huang rum on 2013-11-21
void SetGain(CSCAN_ARS200FPGA_SETONE stSetOne , WORD wChannelNo)
{
	WORD wTemp0 = 0;
	WORD wTemp1 = 0;
	WORD wTemp2 = 0;
	WORD wTemp3 = 0;
	float fTotalPlus = 0;

	if(gbGuanLian)
	{
		DB_ITEM dB1;
		memset(&dB1, 0, sizeof(DB_ITEM));
		DB_ITEM dB2;
		memset(&dB2, 0, sizeof(DB_ITEM));

		GuanLianDBProcess(stSetOne.fPlus, stSetOne.fRefPlus, dB1, dB2);

		cscan_ch1_config_reg->wdBControl = 0;
		cscan_ch1_config_reg->wdBRef = dB1.wDB_A;
		cscan_ch1_config_reg->wdBControl = 1;
		cscan_ch1_config_reg->wdBControl = 0;
		Sleep(1);
		cscan_ch1_config_reg->wdBControl = 0;
		cscan_ch1_config_reg->wdBLevel1 = dB1.wDB_B;
		cscan_ch1_config_reg->wdBControl = 1;
		cscan_ch1_config_reg->wdBControl = 0;
		Sleep(1);
		cscan_ch1_config_reg->wdBControl = 0;
		cscan_ch1_config_reg->wdBLevel2 = dB1.wDB_C;
		cscan_ch1_config_reg->wdBControl = 1;
		cscan_ch1_config_reg->wdBControl = 0;
		Sleep(1);
		cscan_ch1_config_reg->wdBControl = 0;
		cscan_ch1_config_reg->wdBLevel3 = dB1.wDB_D;
		cscan_ch1_config_reg->wdBControl = 1;
		cscan_ch1_config_reg->wdBControl = 0;
		Sleep(1);

		cscan_ch2_config_reg->wdBControl = 0;
		cscan_ch2_config_reg->wdBRef = dB2.wDB_A;
		cscan_ch2_config_reg->wdBControl = 1;
		cscan_ch2_config_reg->wdBControl = 0;
		Sleep(1);
		cscan_ch2_config_reg->wdBControl = 0;
		cscan_ch2_config_reg->wdBLevel1 = dB2.wDB_B;
		cscan_ch2_config_reg->wdBControl = 1;
		cscan_ch2_config_reg->wdBControl = 0;
		Sleep(1);
		cscan_ch2_config_reg->wdBControl = 0;
		cscan_ch2_config_reg->wdBLevel2 = dB2.wDB_C;
		cscan_ch2_config_reg->wdBControl = 1;
		cscan_ch2_config_reg->wdBControl = 0;
		Sleep(1);
		cscan_ch2_config_reg->wdBControl = 0;
		cscan_ch2_config_reg->wdBLevel3 = dB2.wDB_D;
		cscan_ch2_config_reg->wdBControl = 1;
		cscan_ch2_config_reg->wdBControl = 0;
		Sleep(1);
	}
	else
	{
		fTotalPlus = stSetOne.fPlus + stSetOne.fRefPlus;
		dBProcess(fTotalPlus, wTemp0, wTemp1, wTemp2, wTemp3);

		if(wChannelNo == 0)
		{
			cscan_ch1_config_reg->wdBControl = 0;
			cscan_ch1_config_reg->wdBRef = wTemp0;
			cscan_ch1_config_reg->wdBControl = 1;
			cscan_ch1_config_reg->wdBControl = 0;
			Sleep(1);
			cscan_ch1_config_reg->wdBControl = 0;
			cscan_ch1_config_reg->wdBLevel1 = wTemp1;
			cscan_ch1_config_reg->wdBControl = 1;
			cscan_ch1_config_reg->wdBControl = 0;
			Sleep(1);
			cscan_ch1_config_reg->wdBControl = 0;
			cscan_ch1_config_reg->wdBLevel2 = wTemp2;
			cscan_ch1_config_reg->wdBControl = 1;
			cscan_ch1_config_reg->wdBControl = 0;
			Sleep(1);
			cscan_ch1_config_reg->wdBControl = 0;
			cscan_ch1_config_reg->wdBLevel3 = wTemp3;
			cscan_ch1_config_reg->wdBControl = 1;
			cscan_ch1_config_reg->wdBControl = 0;
			Sleep(1);
		}
		else if(wChannelNo == 1)
		{
			cscan_ch2_config_reg->wdBControl = 0;
			cscan_ch2_config_reg->wdBRef = wTemp0;
			cscan_ch2_config_reg->wdBControl = 1;
			cscan_ch1_config_reg->wdBControl = 0;
			Sleep(1);
			cscan_ch2_config_reg->wdBControl = 0;
			cscan_ch2_config_reg->wdBLevel1 = wTemp1;
			cscan_ch2_config_reg->wdBControl = 1;
			cscan_ch2_config_reg->wdBControl = 0;
			Sleep(1);
			cscan_ch2_config_reg->wdBControl = 0;
			cscan_ch2_config_reg->wdBLevel2 = wTemp2;
			cscan_ch2_config_reg->wdBControl = 1;
			cscan_ch2_config_reg->wdBControl = 0;
			Sleep(1);
			cscan_ch2_config_reg->wdBControl = 0;
			cscan_ch2_config_reg->wdBLevel3 = wTemp3;
			cscan_ch2_config_reg->wdBControl = 1;
			cscan_ch2_config_reg->wdBControl = 0;
		}
		else
		{
		}
	}
}
*/
void delay(int nDelayclock)
{                 
	int i;
	int j = 0;
                  
	for(i=0;i<nDelayclock;i++)
	{               
		j++;             
	}                                 
}
void SetInit()
{
	cscan_code_reg->wReset = 1;
	Sleep(1);
	cscan_code_reg->wReset = 0;
	
	cscan_code_reg->wEnable = 0xffff;
		
	WORD wTemp0 = 0;
	WORD wTemp1 = 0;
	WORD wTemp2 = 0;
	WORD wTemp3 = 0;

	dBProcess(60, wTemp0, wTemp1, wTemp2, wTemp3);

	// 工作模式
	cscan_sys_reg->wWorkMode = DoubleChannels;
	WORD wRelay = 0;
	wRelay &= 0xFFFC;
	//yachang.huang rum on 2013-11-19
	//cscan_sys_reg->wRelay = wRelay;
	gbGuanLian = FALSE; 

	CSCAN_ARS200FPGA_SETONE stSetInit[CHANNEL_NUM];
	stSetInit[0].stCon.wRepeatCounter = 110; 195 / 2;	//通过声速6000,声程100计算得出
	stSetInit[0].stCon.wMaiKuan = int(50.0 / (1000.0 / ADFREQUENCY) +0.5);	// 100
	stSetInit[0].stConfig.wDetectMode = 0;	// 全波
	stSetInit[0].stConfig.wValueMiddle = 512;
	stSetInit[0].stConfig.wSuspend = 0;
	stSetInit[0].stConfig.wMonitorZeroCounter = 0;
//	stSetInit[0].stConfig.wdBRef = Gain2Data(20);
//	stSetInit[0].stConfig.wdBLevel1 = Gain2Data(20);
//	stSetInit[0].stConfig.wdBLevel2 = Gain2Data(32);
//	stSetInit[0].stConfig.wdBLevel3 = Gain2Data(32);
	stSetInit[0].stConfig.wdBRef = wTemp0;
	stSetInit[0].stConfig.wdBLevel1 = wTemp1;
	stSetInit[0].stConfig.wdBLevel2 = wTemp2;
	stSetInit[0].stConfig.wdBLevel3 = wTemp3;
	stSetInit[0].stGate.wGate1Enable = 0;
	stSetInit[0].stGate.wGate1SyncMode = 0;
	stSetInit[0].stGate.wGate1SyncThreshold = 400;
	stSetInit[0].stGate.wGate1Start = 750;
	stSetInit[0].stGate.wGate1Width = 1050;
	stSetInit[0].stGate.wGate1Amp = 30;
	stSetInit[0].stGate.wGate2Enable = 0;
	stSetInit[0].stGate.wGate2SyncMode = 0;
	stSetInit[0].stGate.wGate2SyncThreshold = 400;
	stSetInit[0].stGate.wGate2Start = 750;
	stSetInit[0].stGate.wGate2Width = 1050;
	stSetInit[0].stGate.wGate2Amp = 30;
	stSetInit[0].stGate.wGate3Enable = 0;
	stSetInit[0].stGate.wGate3SyncMode = 0;
	stSetInit[0].stGate.wGate3SyncThreshold = 400;
	stSetInit[0].stGate.wGate3Start = 750;
	stSetInit[0].stGate.wGate3Width = 1050;
	stSetInit[0].stGate.wGate3Amp = 30;
	stSetInit[0].stGate.wGate4Enable = 0;
	stSetInit[0].stGate.wGate4SyncMode = 0;
	stSetInit[0].stGate.wGate4SyncThreshold = 400;
	stSetInit[0].stGate.wGate4Start = 750;
	stSetInit[0].stGate.wGate4Width = 1050;
	stSetInit[0].stGate.wGate4Amp = 30;
	stSetInit[0].stGate.wSimpleInterval = 0;
	stSetInit[0].stGate.wSimpleAverage = 3;
	stSetInit[0].stGate.wDelay = 0;
	stSetInit[0].fRange = 100;
	stSetInit[0].nVelocity = 6000;
	stSetInit[0].wProbeBand = 4;
	stSetInit[0].wVoltage = 50;
	stSetInit[0].wImpedance = 0;
	stSetInit[0].fPlus = 30; 
	stSetInit[0].fRefPlus = 0;
	stSetInit[0].wEnerge = 1; //yachang.huang add on 2013-11-26
	stSetInit[0].wRevAndSendMode = 1;//yachang.huang add on 2013-11-26
	stSetInit[0].wSPIAddr = 0;//yachang.huang add on 2013-11-26
	stSetInit[0].wSPIData = 0;//yachang.huang add on 2013-11-26
//	stSetInit[1] =  stSetInit[0];
	memcpy(&stSetInit[1], &stSetInit[0], sizeof(CSCAN_ARS200FPGA_SETONE));
	
	SetOne(stSetInit[0],0);
	SetOne(stSetInit[1],1);

	//通道1手动复位
	cscan_ch1_data_reg->wMReset = 1;
	Sleep(1);
	cscan_ch1_data_reg->wMReset = 0;
	//通道2手动复位
	cscan_ch2_data_reg->wMReset = 1;
	Sleep(1);
	cscan_ch2_data_reg->wMReset = 0;

/*	//通道1软复位
	cscan_ch1_con_reg->wReset |= 0x1;
	cscan_ch1_con_reg->wReset &= 0xFFFE;
	//通道2软复位
	cscan_ch2_con_reg->wReset |= 0x1;
	cscan_ch2_con_reg->wReset &= 0xFFFE;
*/
}
WORD Gain2Data( float fGain)
{                 
	WORD wData;    
	if(fGain <=0)
	{
		wData = GAIN_DATA_MIN; 
	}
	else if( fGain > GAIN_MAX)
	{
		wData = GAIN_DATA_MAX;
	}
	else
	{
		wData = (WORD) (fGain*20.0/3);
	}
	return wData;
}  

void ReadCodeAndFeature(CSCAN_CH_SEND_MONITOR_DATA_t *pstMonitorData, int nChNo)
{
	pstMonitorData->stFeatureData.wXLow16 = cscan_code_reg->wXLow16;
	pstMonitorData->stFeatureData.wXHigh16 = cscan_code_reg->wXHigh16;
	pstMonitorData->stFeatureData.wYLow16 = cscan_code_reg->wYLow16;
	pstMonitorData->stFeatureData.wYHigh16 = cscan_code_reg->wYHigh16;
	pstMonitorData->stFeatureData.wZLow16 = cscan_code_reg->wZLow16;
	pstMonitorData->stFeatureData.wZHigh16 = cscan_code_reg->wZHigh16;
	pstMonitorData->stFeatureData.wW1Low16 = cscan_code_reg->wW1Low16;
	pstMonitorData->stFeatureData.wW1High16 = cscan_code_reg->wW1High16;
	pstMonitorData->stFeatureData.wALow16 = cscan_code_reg->wALow16;
	pstMonitorData->stFeatureData.wAHigh16 = cscan_code_reg->wAHigh16;
	pstMonitorData->stFeatureData.wBLow16 = cscan_code_reg->wBLow16;
	pstMonitorData->stFeatureData.wBHigh16 = cscan_code_reg->wBHigh16;
	pstMonitorData->stFeatureData.wW2Low16 = cscan_code_reg->wW2Low16;
	pstMonitorData->stFeatureData.wW2High16 = cscan_code_reg->wW2High16;

	CSCAN_ARS200FPGA_CH_FEATURE_REG* cscan_ch_feature_reg;
	if(nChNo == 0)
	{
		cscan_ch_feature_reg = cscan_ch1_feature_reg;
	}
	else if(nChNo == 1)
	{
		cscan_ch_feature_reg = cscan_ch2_feature_reg;
	}
	else
	{
		;//
	}
	pstMonitorData->stFeatureData.wGate1SyncTime = cscan_ch_feature_reg->wGate1SyncTime;
//	pstMonitorData->stFeatureData.wGate1MPFirstTime = cscan_ch_feature_reg->wGate1MPFirstTime;
	pstMonitorData->stFeatureData.wGate1MPTime = cscan_ch_feature_reg->wGate1MPTime;
	pstMonitorData->stFeatureData.wGate1MPAmp = cscan_ch_feature_reg->wGate1MPAmp;
//	pstMonitorData->stFeatureData.wGate1FPFirstTime = cscan_ch_feature_reg->wGate1FPFirstTime;
//	pstMonitorData->stFeatureData.wGate1FPTime = cscan_ch_feature_reg->wGate1FPTime;
//	pstMonitorData->stFeatureData.wGate1FPAmp = cscan_ch_feature_reg->wGate1FPAmp;
//	pstMonitorData->stFeatureData.wGate2SyncTime = cscan_ch_feature_reg->wGate2SyncTime;
//	pstMonitorData->stFeatureData.wGate2MPFirstTime = cscan_ch_feature_reg->wGate2MPFirstTime;
	pstMonitorData->stFeatureData.wGate2MPTime = cscan_ch_feature_reg->wGate2MPTime;
	pstMonitorData->stFeatureData.wGate2MPAmp = cscan_ch_feature_reg->wGate2MPAmp;
	pstMonitorData->stFeatureData.wGate2FPFirstTime = cscan_ch_feature_reg->wGate2FPFirstTime;
	pstMonitorData->stFeatureData.wGate2FPTime = cscan_ch_feature_reg->wGate2FPTime;
	pstMonitorData->stFeatureData.wGate2FPAmp = cscan_ch_feature_reg->wGate2FPAmp;
	pstMonitorData->stFeatureData.wGate3MPTime = cscan_ch_feature_reg->wGate3MPTime;
	pstMonitorData->stFeatureData.wGate3MPAmp = cscan_ch_feature_reg->wGate3MPAmp;
	pstMonitorData->stFeatureData.wGate3FPFirstTime = cscan_ch_feature_reg->wGate3FPFirstTime;
	pstMonitorData->stFeatureData.wGate3FPTime = cscan_ch_feature_reg->wGate3FPTime;
	pstMonitorData->stFeatureData.wGate3FPAmp = cscan_ch_feature_reg->wGate3FPAmp;
	pstMonitorData->stFeatureData.wGate4MPTime = cscan_ch_feature_reg->wGate4MPTime;
	pstMonitorData->stFeatureData.wGate4MPAmp = cscan_ch_feature_reg->wGate4MPAmp;
	pstMonitorData->stFeatureData.wGate4FPFirstTime = cscan_ch_feature_reg->wGate4FPFirstTime;
	pstMonitorData->stFeatureData.wGate4FPTime = cscan_ch_feature_reg->wGate4FPTime;
	pstMonitorData->stFeatureData.wGate4FPAmp = cscan_ch_feature_reg->wGate4FPAmp;
}

// 设置探头频率
//按新版硬件程序修改  yachang.huang modify start on 2013-11-22
/*
“000” ---0.2~30MHz
“001” ---10~30MHz
“010” ---15MHz
“011” ---1~10MHz
“100” ---10MHz
“101” ---4MHz
“110” ---2MHz
“111” ---1MHz

*/
void SetProbeBand(WORD wBand, WORD wChannelNo)
{
	WORD wLvBo = 0;	// 滤波频带
	WORD wMax500Value = 0;
	if(!gbGuanLian)
	{
		if(wBand == 0)	//“000” ---0.2~30MHz
		{
			if(wChannelNo == 0)
			{
				*ch1_wave_filter_control = 0;
			}
			else if(wChannelNo == 1)
			{
				*ch2_wave_filter_control = 0;
			}
		}
		else if(wBand == 1)	//“001” ---10~30MHz
		{
			if(wChannelNo == 0)
			{
				*ch1_wave_filter_control = 1;
			}
			else if(wChannelNo == 1)
			{
				*ch2_wave_filter_control = 1;
			}
		}
		else if(wBand == 2)	//“010” ---15MHz
		{
			if(wChannelNo == 0)
			{
				*ch1_wave_filter_control = 2;
			}
			else if(wChannelNo == 1)
			{
				*ch2_wave_filter_control = 2;
			}
		}
		else if(wBand == 3)	//“011” ---1~10MHz
		{
			if(wChannelNo == 0)
			{
				*ch1_wave_filter_control = 3;
			}
			else if(wChannelNo == 1)
			{
				*ch2_wave_filter_control = 3;
			}
		}
		else if(wBand == 4)	//“100” ---10MHz
		{
			if(wChannelNo == 0)
			{
				*ch1_wave_filter_control = 4;
			}
			else if(wChannelNo == 1)
			{
				*ch2_wave_filter_control = 4;
			}
		}
		else if(wBand == 5) //“101” ---4MHz
		{
			if(wChannelNo == 0)
			{
				*ch1_wave_filter_control = 5;
			}
			else if(wChannelNo == 1)
			{
				*ch2_wave_filter_control = 5;
			}
		}
		else if(wBand == 6) //“110” ---2MHz
		{
			if(wChannelNo == 0)
			{
				*ch1_wave_filter_control = 6;
			}
			else if(wChannelNo == 1)
			{
				*ch2_wave_filter_control = 6;
			}
		}
		else if(wBand == 7)//“111” ---1MHz
		{
			if(wChannelNo == 0)
			{
				*ch1_wave_filter_control = 7;
			}
			else if(wChannelNo == 1)
			{
				*ch2_wave_filter_control = 7;
			}
		}
	}
	else
	{
		if(wBand == 0)	//“000” ---0.2~30MHz
		{
			*ch1_wave_filter_control = 0;
			*ch2_wave_filter_control = 0;
		}
		else if(wBand == 1)	//“001” ---10~30MHz
		{
			*ch1_wave_filter_control = 1;
			*ch2_wave_filter_control = 1;
		}
		else if(wBand == 2)	//“010” ---15MHz
		{
			*ch1_wave_filter_control = 2;
			*ch2_wave_filter_control = 2;
		}
		else if(wBand == 3)	//“011” ---1~10MHz
		{
			*ch1_wave_filter_control = 3;
			*ch2_wave_filter_control = 3;
		}
		else if(wBand == 4)	//“100” ---10MHz
		{
			*ch1_wave_filter_control = 4;
			*ch2_wave_filter_control = 4;
		}
		else if(wBand == 5) //“101” ---4MHz
		{
			*ch1_wave_filter_control = 5;
			*ch2_wave_filter_control = 5;
		}
		else if(wBand == 6) //“110” ---2MHz
		{
			*ch1_wave_filter_control = 6;
			*ch2_wave_filter_control = 6;
		}
		else if(wBand == 7)//“111” ---1MHz
		{
			*ch1_wave_filter_control = 7;
			*ch2_wave_filter_control = 7;
		}

	}
}
//yachang.huang modify start on 2013-11-22


//设置阻抗
//yachang.huang modify start on 2013-11-22
/*
0x18000002
Bit[1]:通道1高低阻抗切换，0为低阻抗（50Ω），1为高阻抗（1000Ω）

Bit[4]:通道2高低阻抗切换，0为低阻抗（50Ω），1为高阻抗（1000Ω）
*/
void SetImpedance(WORD wImpedance, WORD wChannelNo)
{
	//if(gwCscanWorkMode == 1 || gwCscanWorkMode == 2)	//单通道才参设置阻抗
	if(1)
	{
		if(wChannelNo == 0)
		{
			if(wImpedance == 0) //A高
			{
				sys_reg->wRelay |= 0x2;						
			}
			else if(wImpedance == 1)//A低
			{			
				sys_reg->wRelay &= 0xFFFD;
			}
		}
		else if(wChannelNo == 1)
		{
			if(wImpedance == 2) //B高
			{
				sys_reg->wRelay |= 0x10;
			}
			else if(wImpedance == 0)//B低
			{
				sys_reg->wRelay &= 0xFFEF;
			}
		}
		else
		{
			;//
		}
	}
}
//yachang.huang modify end on 2013-11-22


// 设置检波模式  
// yachang.huang modify start on 2013-11-22
// 按照新版本修改0---全波
// 1---正半波
// 2---负半波
// 3---射频
void SetJianBoMode(WORD wJianBoMode, WORD wChannelNo)
{
	if(!gbGuanLian)
	{
			switch(wJianBoMode)
			{
				case 0:	// 正半波
					if(wChannelNo == 0)
					{
						cscan_ch1_config_reg->wDetectMode = 1;
					}
					else if(wChannelNo == 1)
					{
						cscan_ch2_config_reg->wDetectMode = 1;
					}
					else
					{
					}
					break;
				case 1: // 负半波
					if(wChannelNo == 0)
					{
						cscan_ch1_config_reg->wDetectMode = 2;
					}
					else if(wChannelNo == 1)
					{
						cscan_ch2_config_reg->wDetectMode = 2;
					}
					else
					{
					}
					break;
				case 2: // 全波
					if(wChannelNo == 0)
					{
						cscan_ch1_config_reg->wDetectMode = 0;
					}
					else if(wChannelNo == 1)
					{
						cscan_ch2_config_reg->wDetectMode = 0;
					}
					else
					{
					}
					break;
				case 3: // 射频
					if(wChannelNo == 0)
					{
						cscan_ch1_config_reg->wDetectMode = 3;
					}
					else if(wChannelNo == 1)
					{
						cscan_ch2_config_reg->wDetectMode = 3;
					}
					else
					{
					}
					break;
			}
	}
	else
	{
		switch(wJianBoMode)
		{
			case 0:	// 正半波
				cscan_ch1_config_reg->wDetectMode = 1;
				cscan_ch2_config_reg->wDetectMode = 1;
				break;
			case 1: // 负半波
				cscan_ch1_config_reg->wDetectMode = 2;
				cscan_ch2_config_reg->wDetectMode = 2;
				break;
			case 2: // 全波
				cscan_ch1_config_reg->wDetectMode = 0;
				cscan_ch2_config_reg->wDetectMode = 0;
				break;
			case 3: // 射频
				cscan_ch1_config_reg->wDetectMode = 3;
				cscan_ch2_config_reg->wDetectMode = 3;
				break;
		}
	}
}
//yachang.huang modify end on 2013-11-22


BOOL SetIpAddress(LPCTSTR strDevice, LPCTSTR strIp, LPCTSTR strMask, LPCTSTR strGateWay)
{
	//获得网卡的设备名
/*	WCHAR Names[50];
	DWORD bytes;
	HANDLE m_hFileHandle = CreateFile(_T("NDS0:"), 0, 0, NULL,
	   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
	   (HANDLE) INVALID_HANDLE_VALUE);

	if( m_hFileHandle == INVALID_HANDLE_VALUE )
	{
	   MessageBox(NULL,_T("获得网卡名时打开设备错误"),NULL,0);
	   return false;
	}

	// Get list of adapter names
	if (!DeviceIoControl(m_hFileHandle, 
	   IOCTL_NDIS_GET_ADAPTER_NAMES,
	   NULL,0,
	   Names,MAX_PATH,&bytes,NULL))
	{
	   MessageBox(NULL,_T("获得网卡名错误"),NULL,0);
	   return false;
	}
	DWORD len = wcslen(Names);
	Names[len] = 0;
	Names[len+1] = 0;
	
	CString strKeyName;
//	strKeyName.Format(_T("Comm//%s//Parms//TcpIp"),Names);
	*/
//	WCHAR Names[50];
//	swprintf(Names, _T("RT2501USB1"));
//	swprintf(Names, _T("MOSUSBCE1"));
	wchar_t strKeyName[50];
	swprintf(strKeyName, _T("Comm\\%s\\Parms\\TCPIP"),strDevice);

#if 1
	//http://blog.csdn.net/li_guotao/article/details/3176066
	//http://www.newsmth.net/bbsanc.php?path=%2Fgroups%2Fsci.faq%2FEmbedded%2FRTOS%2FWinCE%2FM.979863316.A
	//http://hi.baidu.com/yzyseal/blog/item/fb02bc89427120a00e244467.html
	//http://anston.blog.sohu.com/93396551.html
	//打开注册表对网卡IP信息对应子健进行修改
	HKEY   hkey; 
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,strKeyName,0,KEY_WRITE,&hkey)   !=   ERROR_SUCCESS)   
	{
	   MessageBox(NULL,TEXT("打开注册表错误"),NULL,0);   
	   return false;
	}
	DWORD   value;   
	value   =   0;
	WCHAR buffer[50];

	//set   EnableDHCP   
	if(RegSetValueEx(hkey,TEXT("EnableDHCP"),0,REG_DWORD,(const BYTE *)&value,sizeof(DWORD))   !=   ERROR_SUCCESS)   
	   MessageBox(NULL,TEXT("关闭自动获得IP错误"),NULL,0);   
	//set   ip   address   
//	memset(buffer,0,100);
//	memcpy(buffer,m_strIp.GetBuffer(0),m_strIp.GetLength()*2);
	if(RegSetValueEx(hkey,TEXT("IpAddress"),0,REG_MULTI_SZ,(const BYTE *)strIp, (_tcslen(strIp)+1)*sizeof(WCHAR)) !=   ERROR_SUCCESS)   
	   MessageBox(NULL,TEXT("设置IP错误"),NULL,0);
	//set   subnet   mask 
//	memset(buffer,0,100);
//	memcpy(buffer,m_strMask.GetBuffer(0),m_strMask.GetLength()*2);
	if(RegSetValueEx(hkey,TEXT("SubnetMask"),0,REG_MULTI_SZ,(const BYTE *)strMask, (_tcslen(strMask)+1)*sizeof(WCHAR)) !=   ERROR_SUCCESS)   
	   MessageBox(NULL,TEXT("设置子网掩码错误"),NULL,0);   
	//set   gateway
//	memset(buffer,0,100);
//	memcpy(buffer,m_strGateWay.GetBuffer(0),m_strGateWay.GetLength()*2);
	if(RegSetValueEx(hkey,TEXT("DefaultGateway"),0,REG_MULTI_SZ,(const BYTE *)strGateWay, (_tcslen(strGateWay)+1)*sizeof(WCHAR)) !=   ERROR_SUCCESS)   
	   MessageBox(NULL,TEXT("设置网关错误"),NULL,0);
	RegFlushKey(hkey);
	RegCloseKey(hkey);
	
	// 重启网卡，不用机器热启动
/*	HANDLE hNdis = CreateFile(_T("NDS0:"), 0, 0, NULL,
	   OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
	   (HANDLE) INVALID_HANDLE_VALUE);
	if( hNdis == INVALID_HANDLE_VALUE )
	{
	   MessageBox(NULL,_T( "重启网络驱动时打开设备错误"),NULL,0);
	   return false;
	}

	// Send the device command.
	if (!DeviceIoControl( hNdis, IOCTL_NDIS_REBIND_ADAPTER,
	   Names, _tcslen( Names) * sizeof( WCHAR ),
	   NULL, 0, NULL, NULL ) )
	{
	   MessageBox(NULL,_T( "重启网络驱动错误"),NULL,0); 
	}*

	CloseHandle( hNdis );*/
#endif
	return true;
}

// 确保发送完
BOOL SendTo(SOCKET sockClient, char szSendData[], DWORD dwSize)
{
	int nSendBytes = 0;

	if((nSendBytes = send( sockClient, szSendData + gnTotalSendBytes, dwSize - gnTotalSendBytes, 0 )) == SOCKET_ERROR)
	{
	/*	if(GetLastError() == WSAEWOULDBLOCK)
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("WSAEWOULDBLOCK"));
		}
        else   
        {   
			wchar_t strText[20];
			memset(strText, 0, sizeof(strText));
			swprintf(strText, _T("send:%d"), GetLastError());
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)strText);
		//	closesocket( gSockClient );
        } */
        gnTotalSendBytes = 0;
        return FALSE;
	}
	else
	{
		gnTotalSendBytes += nSendBytes;
	}
	
	if(gnTotalSendBytes < dwSize)
	{
		SendTo(sockClient, szSendData, dwSize);
	}
	else
	{
		gnTotalSendBytes = 0;
		return TRUE;
	}
}


BOOL ArmSelfTest()
{
	WORD wWriteValue = 12;
	for (int i = 0;i < 512;i++)
	{
		memcpy(arm_selftest + i, &wWriteValue,2);
	}

	WORD wReadValue = 0;
	for (int i = 0;i < 512;i++)
	{
		memcpy(&wReadValue,arm_selftest + i,2);
		if (wReadValue != 12)
		{ 
			return FALSE;
		}
	}
	return TRUE;
}

void GetCurPath(WCHAR* strFilePath)
{
	int i = 0;
	int sLen = 0; 
	sLen = GetModuleFileName(NULL,strFilePath,MAX_PATH); 
	for(i=sLen;i!=0;i--)
	{ 
		if(strFilePath[i]==WCHAR('\\'))
		{ 
			break; 
		} 
		strFilePath[i] = WCHAR('\0');
	}
}

DWORD WINAPI SocketThreadProc3( LPVOID lpParameter )
{
#if 0
	WSADATA wsd;
	if(WSAStartup( MAKEWORD( 2, 2 ), &wsd ) != 0 )// 支持Windows Sockets 2.2版本
	{
		return 0;	
	}
	
	WSAEVENT eventList[MAX_SOCKETS  + 1];
	//////////////////////////////////////////////////////////////////////////
	for (int i=0; i<(MAX_SOCKETS  + 1); i++)
	{
		eventList[i] = NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	int nEventTotal = 0;
	eventList[nEventTotal] = WSACreateEvent();

	SOCKET sockList[MAX_SOCKETS + 1];

	for (int i=0; i<(MAX_SOCKETS  + 1); i++)
	{
		sockList[i] = INVALID_SOCKET;
	}

	SOCKET sockListen = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );	
	if( INVALID_SOCKET == sockListen )
	{
		return 0;
	}

	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port= htons( 5555 ); // 保证字节顺序
	addr.sin_addr.s_addr= htonl( INADDR_ANY );

	int nResult = bind( sockListen, (sockaddr*)&addr, sizeof(sockaddr) );
	if ( nResult == SOCKET_ERROR )
	{
		return 0;
	}
	
	nResult = listen( sockListen, MAX_BACKLOG ); // 最多 MAX_BACKLOG 个 Pending 连接
	if ( nResult == SOCKET_ERROR )
	{
		return 0;
	}
	sockList[nEventTotal] = sockListen;

	WSAEventSelect( sockList[nEventTotal], eventList[nEventTotal], FD_ACCEPT | FD_CLOSE );

	
	int nIndex;
	nEventTotal++;
    
	while ((nIndex = WSAWaitForMultipleEvents( nEventTotal, eventList, FALSE, WSA_INFINITE, TRUE )) != WSA_WAIT_FAILED)
	{
		nIndex = nIndex - WSA_WAIT_EVENT_0;
		BOOL bRet = WSAResetEvent(eventList[nIndex]);
		if (bRet == FALSE) 
		{
		}


		WSANETWORKEVENTS stNetworkEvents;
		WSAEnumNetworkEvents( sockList[nIndex], eventList[nIndex], &stNetworkEvents );
		switch(stNetworkEvents.lNetworkEvents)
		{
			case FD_ACCEPT://服务器端接收到连接请求后
				{	
					if ( stNetworkEvents.iErrorCode[FD_ACCEPT_BIT] != 0 )  // 发生网络错误
					{
						MessageBox(hWndMain, _T("send error"), _T("send error"), MB_OK);

					}
					SOCKET sockAccept;
					if((sockAccept = accept(sockList[nIndex], NULL, NULL)) == INVALID_SOCKET )
					{

					}
					if ( nEventTotal < MAX_SOCKETS)
					{
						sockList[nEventTotal] = sockAccept;
						if ((eventList[nEventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT )
						{
							break;
						}
						if (WSAEventSelect( sockList[nEventTotal], eventList[nEventTotal], FD_ACCEPT | FD_READ | FD_CLOSE ) == SOCKET_ERROR)
						{

							break;
						}
						nEventTotal++;
					}
					else
					{
						nEventTotal = 1;
						sockList[nEventTotal] = sockAccept;
						if ((eventList[nEventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT )
						{
							break;
						}
						if (WSAEventSelect( sockList[nEventTotal], eventList[nEventTotal], FD_READ | FD_CLOSE ) == SOCKET_ERROR)
						{

							break;
						}
						// Song test
						MessageBox(NULL, _T("accept"), _T("lu 2"), MB_OK);
					}
				}
				break;
			case FD_READ:
				{
					int nResult = 0;
					char szRecvData[NETHEAD_SIZE + 512];
					memset(szRecvData, 0, sizeof(szRecvData));
					
					if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
					{
						MessageBox(hWndMain, _T("recverror"), _T("recverror"), MB_OK);

					 	break;
					}

					nResult = recv( sockList[nIndex], szRecvData, NETHEAD_SIZE, 0 );
	
					PACKET_HEAD_t stPacketHead;
					memcpy( &stPacketHead, szRecvData, sizeof(stPacketHead));
	
					nResult = recv( sockList[nIndex], &szRecvData[NETHEAD_SIZE], stPacketHead.dwSize, 0 );

					SOCKET sockClient = sockList[nIndex];
					AnalyseRecvData( szRecvData, sockClient );

				}
				break;
			case FD_CLOSE:
				{
					if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
					{
						MessageBox(NULL, _T("发生网络错误"),_T("ndt_ce"), MB_OK);	
					}				
					BOOL bRet = WSACloseEvent( eventList[nIndex] );
					if (!bRet)
					{
						MessageBox(NULL, _T("closeevent fail"),_T("ndt_ce"), MB_OK);
					}
					shutdown(sockList[nIndex],SD_RECEIVE);
					BOOL bRet1 = closesocket( sockList[nIndex] );
					if (bRet1 == 0)
					{
						//MessageBox(NULL, _T("关闭socket成功3"),_T("ndt_ce"), MB_OK);
					}
					else
					{
						MessageBox(NULL, _T("关闭socket失败3"),_T("ndt_ce"), MB_OK);
					}

					for (int i = nIndex; i < nEventTotal - 1; i++)
					{						
						eventList[i] = eventList[i + 1];
						sockList[i] = sockList[i + 1];
					}
					nEventTotal--;
					nIndex--;	
				}
				break;
			default:
				break;
		}

		
	}
	MessageBox(NULL, _T("Socket3 exit"),_T("ndt_ce"), MB_OK);

	return 0;
#else
	SOCKET_PROC_PARAM_t* pstSocketParam = (SOCKET_PROC_PARAM_t*)lpParameter;
	SOCKET sockClient = pstSocketParam->sockClient;
	int nIndex = pstSocketParam->nIndex;
	//	SOCKET* pSocket = (SOCKET*)lpParameter;
	//	SOCKET sockClient = *pSocket;

	WSAEVENT event;
	if ((event = WSACreateEvent()) == WSA_INVALID_EVENT )
	{
		wchar_t strText[20];
		wsprintf(strText, _T("event%d"), nIndex);
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
		return 0;
	}
	if (WSAEventSelect(sockClient, event, FD_READ | FD_CLOSE ) == SOCKET_ERROR)
	{
		wchar_t strText[20];
		wsprintf(strText, _T("select%d"), nIndex);
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
		return 0;
	}

	while (WSAWaitForMultipleEvents( 1, &event, FALSE, WSA_INFINITE, TRUE ) != WSA_WAIT_FAILED)
	{
		WSANETWORKEVENTS stNetworkEvents;
		WSAEnumNetworkEvents( sockClient, event, &stNetworkEvents );
		switch(stNetworkEvents.lNetworkEvents)
		{
		case FD_READ:
			{

				int nResult = 0;
				char szRecvData[NETHEAD_SIZE + 5120];
				memset(szRecvData, 0, sizeof(szRecvData));

				if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
				{
					MessageBox(hWndMain, _T("recverror"), _T("recverror"), MB_OK);
					break;
				}

				nResult = recv( sockClient, szRecvData, NETHEAD_SIZE, 0 );

				PACKET_HEAD_t stPacketHead;
				memcpy( &stPacketHead, szRecvData, sizeof(stPacketHead));

				nResult = recv( sockClient, &szRecvData[NETHEAD_SIZE], stPacketHead.dwSize, 0 );
				AnalyseRecvData( szRecvData, sockClient );
			}
			break;
		case FD_CLOSE:
			{
				wchar_t strText[20];
				wsprintf(strText, _T("SocketThreadProc3-ParamClose%d\n"), sockClient);
				OutputDebugString(strText);
				//	PostMessage(hStatusBar, SB_SETTEXT, 
				//		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);

				if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
				{
					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("Error7"));
					break;
				}				
				BOOL bRet = WSACloseEvent( event );
				if (!bRet)
				{
					MessageBox(NULL, _T("closeevent fail"),_T("ndt_ce"), MB_OK);
				}

				//shutdown(sockList[nIndex],SD_SEND);
				//gSockClient[nIndex] = NULL;
				for(vector<SOCKET>::iterator iter = gVecSockParam.begin(); iter!= gVecSockParam.end();)
				{
					if(*iter == sockClient)
					{
						wchar_t strText[20];
						wsprintf(strText, _T("ParamDelete%d"), sockClient);
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
						iter = gVecSockParam.erase(iter);
						OutputDebugString(strText);
					}
					else
					{
						++iter;
					}
				}

				BOOL bRet1 = closesocket( sockClient );
				if (bRet1 == 0)
				{
				}
				else
				{
					MessageBox(NULL, _T("关闭socket失败1"),_T("ndt_ce"), MB_OK);
				}

				wsprintf(strText, _T("SocketThreadProc3-ParamCloseOk%d\n"), sockClient);
				OutputDebugString(strText);
			}
			return 1;
		default:
			break;
		}
	}
#endif
}

DWORD WINAPI SocketParamStringThreadProc( LPVOID lpParameter )
{
	SOCKET_PROC_PARAM_t* pstSocketParam = (SOCKET_PROC_PARAM_t*)lpParameter;
	SOCKET sockClient = pstSocketParam->sockClient;
	int nIndex = pstSocketParam->nIndex;

	WSAEVENT event;
	if ((event = WSACreateEvent()) == WSA_INVALID_EVENT )
	{
		wchar_t strText[20];
		wsprintf(strText, _T("event%d"), nIndex);
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
		return 0;
	}
	if (WSAEventSelect(sockClient, event, FD_READ | FD_CLOSE ) == SOCKET_ERROR)
	{
		wchar_t strText[20];
		wsprintf(strText, _T("select%d"), nIndex);
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
		return 0;
	}

	while (WSAWaitForMultipleEvents( 1, &event, FALSE, WSA_INFINITE, TRUE ) != WSA_WAIT_FAILED)
	{
		WSANETWORKEVENTS stNetworkEvents;
		WSAEnumNetworkEvents( sockClient, event, &stNetworkEvents );
		switch(stNetworkEvents.lNetworkEvents)
		{
		case FD_READ:
			{
				if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
				{
					MessageBox(hWndMain, _T("recverror"), _T("recverror"), MB_OK);
					break;
				}
				wchar_t strText[20];
				wsprintf(strText, _T("luaArrive"), nIndex);
				PostMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
				std::string &bufferStr = pstSocketParam->buffer;
				int bytesRead = 0;
				do
				{
					char buffer[BUFFER_SIZE];															/* 读取缓冲区 */
					bytesRead = recv(sockClient, buffer, BUFFER_SIZE, 0);								
					if (bytesRead > 0)
					{
						bufferStr.append(std::string(buffer, bytesRead));								/* 将读取到的内容加入缓冲区 */
						size_t pos = 0;
						while (true)																	/* 搜索整行文本 */
						{
							size_t nextPos = bufferStr.find_first_of('\n', pos);
							if (nextPos == string::npos)
							{
								break;
							}
							luaL_dostring(pstSocketParam->L, bufferStr.c_str() + pos, nextPos - pos);	/* 将其压入Lua引擎 */
							pos = nextPos + 1;
						}
						bufferStr = bufferStr.substr(pos, string::npos);
					}
				} while (bytesRead > 0);

			}
			break;
		case FD_CLOSE:
			{
				wchar_t strText[20];
				wsprintf(strText, _T("SocketParamStringThreadProc-ParamClose%d\n"), sockClient);
				OutputDebugString(strText);
				//	PostMessage(hStatusBar, SB_SETTEXT, 
				//		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);

				if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
				{
					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("Error7"));
					break;
				}				
				BOOL bRet = WSACloseEvent( event );
				if (!bRet)
				{
					MessageBox(NULL, _T("closeevent fail"),_T("ndt_ce"), MB_OK);
				}

				for(vector<SOCKET>::iterator iter = gVecSockParamString.begin(); iter!= gVecSockParamString.end();)
				{
					if(*iter == sockClient)
					{
						wchar_t strText[20];
						wsprintf(strText, _T("ParamDelete%d"), sockClient);
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
						iter = gVecSockParamString.erase(iter);
						OutputDebugString(strText);
					}
					else
					{
						++iter;
					}
				}

				BOOL bRet1 = closesocket( sockClient );
				if (bRet1 == 0)
				{
				}
				else
				{
					MessageBox(NULL, _T("关闭socket失败1"),_T("ndt_ce"), MB_OK);
				}

				wsprintf(strText, _T("SocketParamStringThreadProc-ParamCloseOk%d\n"), sockClient);
				OutputDebugString(strText);
			}
			return 1;
		default:
			break;
		}
	}
}

//yachang.huang add start on 2013-11-22
/*
0x18000002
Bit[0]: 高压切换K4，0为低压（50V），1为高压（400V）
*/
void SetVoltage(WORD wVoltage)
{
    if (wVoltage == 50)
    {
		sys_reg->wRelay &= 0xFFFE;	
    }
	else
	{
		sys_reg->wRelay |= 0x1;	
	}
}


/*
Bit[2]:通道1高低能量切换，0为低能量，1为高能量
Bit[5]:通道2高低能量切换，0为低能量，1为高能量
*/
void SetEnerge(WORD wEnerge,WORD wChannelNo)
{


	if (wChannelNo == 0)
	{
		if (wEnerge == 0)
		{
			sys_reg->wRelay &= 0xFFFB;	
		}
		else
		{
			sys_reg->wRelay |= 0x4;	
		}
	}
	else if(wChannelNo == 1)
	{
		if (wEnerge == 0)
		{
			sys_reg->wRelay &= 0xFFDF;	
		}
		else
		{
			sys_reg->wRelay |= 0x20;	
		}
	}



}

/*
Bit[3]:通道1收发模式切换，0为一发一收模式，1为自发自收模式
Bit[6]:通道2收发模式切换，0为一发一收模式，1为自发自收模式
*/
void SetRevAndSendMode(WORD wRevAndSendMode,WORD wChannelNo)
{

	if (wChannelNo == 0)
	{
		if (wRevAndSendMode == 0)
		{
			sys_reg->wRelay &= 0xFFF7;	
		}
		else
		{
			sys_reg->wRelay |= 0x8;	
		}
	}
	else if(wChannelNo == 1)
	{
		if (wRevAndSendMode == 0) 
		{
			sys_reg->wRelay &= 0xFFBF;	
		}
		else
		{
			sys_reg->wRelay |= 0x40;	
		}
	}

}

/*
“11010010”—RDA11
“11010011”—RDA12
“11010001”—RDA22
"11010000"—RDA21
"11001011"—FIL_DA2
"11001001"—FIL_DA1
"11001000"—DA_AMP1
"11001010"—DA_AMP2

*/
void SetSPI(WORD wSPIAddr,WORD wSPIData,WORD wChannelNo)
{
	switch(wSPIAddr)
	{
	case 0:
		sys_reg->wSPIAddr = 0xD2;
		break;
	case 1:
		sys_reg->wSPIAddr = 0xD3;
		break;
	case 2:
		sys_reg->wSPIAddr = 0xD1;
		break;
	case 3:
		sys_reg->wSPIAddr = 0xD0;
		break;
	case 4:
		sys_reg->wSPIAddr = 0xCB;
		break;
	case 5:
		sys_reg->wSPIAddr = 0xC9;
		break;
	case 6:
		sys_reg->wSPIAddr = 0xC8;
		break;
	case 7:
		sys_reg->wSPIAddr = 0xCA;
		break;
	default:
		sys_reg->wSPIAddr = 0xD2;
		break;
	}
	sys_reg->wSPIData = wSPIData;
	
	sys_reg->wSPIStart = 1;
	Sleep(1);
    sys_reg->wSPIStart = 0;
}

//yachang.huang add end on 2013-11-22


//yachang.huang add start on 2013-12-19
DWORD WINAPI SocketThreadProc4(LPVOID lpParameter)
{
#if 0
	WSADATA wsd;
	if(WSAStartup( MAKEWORD( 2, 2 ), &wsd ) != 0 )// 支持Windows Sockets 2.2版本
	{
		return 0;	
	}

	WSAEVENT eventList[MAX_SOCKETS  + 1];
	//////////////////////////////////////////////////////////////////////////
	for (int i=0; i<(MAX_SOCKETS  + 1); i++)
	{
		eventList[i] = NULL;
	}
	//////////////////////////////////////////////////////////////////////////
	int nEventTotal = 0;
	eventList[nEventTotal] = WSACreateEvent();

	SOCKET sockList[MAX_SOCKETS + 1];
	for (int i=0; i<(MAX_SOCKETS  + 1); i++)
	{
		sockList[i] = INVALID_SOCKET;
	}

	SOCKET sockListen = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );	
	if( INVALID_SOCKET == sockListen )
	{
		return 0;
	}

	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port= htons( 2222 ); // 保证字节顺序
	addr.sin_addr.s_addr= htonl( INADDR_ANY );

	int nResult = bind( sockListen, (sockaddr*)&addr, sizeof(sockaddr) );
	if ( nResult == SOCKET_ERROR )
	{
		return 0;
	}

	nResult = listen( sockListen, MAX_BACKLOG ); // 最多 MAX_BACKLOG 个 Pending 连接
	if ( nResult == SOCKET_ERROR )
	{
		return 0;
	}
	sockList[nEventTotal] = sockListen;

	WSAEventSelect( sockList[nEventTotal], eventList[nEventTotal], FD_ACCEPT | FD_CLOSE );
	int nIndex;
	nEventTotal++;

	while ((nIndex = WSAWaitForMultipleEvents( nEventTotal, eventList, FALSE, WSA_INFINITE, TRUE )) != WSA_WAIT_FAILED)
	{
		nIndex = nIndex - WSA_WAIT_EVENT_0;
		WSANETWORKEVENTS stNetworkEvents;
		WSAEnumNetworkEvents( sockList[nIndex], eventList[nIndex], &stNetworkEvents );
		switch(stNetworkEvents.lNetworkEvents)
		{
		case FD_ACCEPT://服务器端接收到连接请求后
			{	
				if ( stNetworkEvents.iErrorCode[FD_ACCEPT_BIT] != 0 )  // 发生网络错误
				{
					break;
				}
				SOCKET sockAccept;
				if((sockAccept = accept(sockList[nIndex], NULL, NULL)) == INVALID_SOCKET )
				{
					break;
				}

				int nBufLen;  
				int nOptLen = sizeof(nBufLen);
				if(getsockopt(sockAccept,SOL_SOCKET,SO_SNDBUF,(char*)&nBufLen,&nOptLen) == SOCKET_ERROR)  
				{  

				}  
				// 设置当前套接字s的接受数据缓冲区为原来的4倍	 
				nBufLen *= 4;	
				if(setsockopt(sockAccept,SOL_SOCKET,SO_SNDBUF,(char*)&nBufLen,nOptLen) == SOCKET_ERROR) 
				{   

				}  

				if ( nEventTotal < MAX_SOCKETS)
				{
					sockList[nEventTotal] = sockAccept;
					if ((eventList[nEventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT )
					{
						break;
					}
					if (WSAEventSelect( sockList[nEventTotal], eventList[nEventTotal], FD_READ | FD_CLOSE ) == SOCKET_ERROR)
					{
						break;
					}
					nEventTotal++;
				}
				else
				{
					nEventTotal = 1;
					sockList[nEventTotal] = sockAccept;
					if ((eventList[nEventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT )
					{
						break;
					}
					if (WSAEventSelect( sockList[nEventTotal], eventList[nEventTotal], FD_READ | FD_CLOSE ) == SOCKET_ERROR)
					{
						break;
					}
				}
			}
			break;
		case FD_READ:
			{
				int nResult = 0;
				char szRecvData[NETHEAD_SIZE + 512];
				memset(szRecvData, 0, sizeof(szRecvData));

				if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
				{
					MessageBox(hWndMain, _T("recverror"), _T("recverror"), MB_OK);
					break;
				}
				nResult = recv( sockList[nIndex], szRecvData, NETHEAD_SIZE, 0 );


				PACKET_HEAD_t stPacketHead;
				memcpy( &stPacketHead, szRecvData, sizeof(stPacketHead));
				nResult = recv( sockList[nIndex], &szRecvData[NETHEAD_SIZE], stPacketHead.dwSize, 0 );

				SOCKET sockClient = sockList[nIndex];
				AnalyseRecvData( szRecvData, sockClient );
			}
			break;
		case FD_CLOSE:
			{
				if(gSockClient3 == sockList[nIndex])
				{
					gbAscanDataStoreSend = FALSE;
				}
				if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
				{
					break;
				}				
				WSACloseEvent( eventList[nIndex] );
				shutdown(sockList[nIndex], SD_SEND);
				Sleep(50);

				BOOL bRet1 = closesocket( sockList[nIndex] );
				if (bRet1 == 0)
				{

				}
				else
				{
					MessageBox(NULL, _T("关闭socket失败2"),_T("ndt_ce"), MB_OK);
				}

				for (int i = nIndex; i < nEventTotal - 1; i++)
				{						
					eventList[i] = eventList[i + 1];
					sockList[i] = sockList[i + 1];
				}
				nEventTotal--;
			}
			break;
		default:
			break;
		}
	}

	MessageBox(NULL, _T("Socket2 exit"),_T("ndt_ce"), MB_OK);
	return 0;
#else
	SOCKET_PROC_PARAM_t* pstSocketParam = (SOCKET_PROC_PARAM_t*)lpParameter;
	SOCKET sockClient = pstSocketParam->sockClient;
	int nIndex = pstSocketParam->nIndex;
	WSAEVENT event;
	if ((event = WSACreateEvent()) == WSA_INVALID_EVENT )
	{
		wchar_t strText[20];
		wsprintf(strText, _T("event%d"), nIndex);
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
		return 0;
	}
	if (WSAEventSelect(sockClient, event, FD_READ | FD_CLOSE ) == SOCKET_ERROR)
	{
		wchar_t strText[20];
		wsprintf(strText, _T("select%d"), nIndex);
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
		return 0;
	}

	while (WSAWaitForMultipleEvents( 1, &event, FALSE, WSA_INFINITE, TRUE ) != WSA_WAIT_FAILED)
	{
		WSANETWORKEVENTS stNetworkEvents;
		WSAEnumNetworkEvents( sockClient, event, &stNetworkEvents );
		switch(stNetworkEvents.lNetworkEvents)
		{
		case FD_READ:
			{
				int nResult = 0;
				char szRecvData[NETHEAD_SIZE + 512];
				memset(szRecvData, 0, sizeof(szRecvData));

				if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
				{
					MessageBox(hWndMain, _T("recverror"), _T("recverror"), MB_OK);
					break;
				}

				nResult = recv( sockClient, szRecvData, NETHEAD_SIZE, 0 );

				PACKET_HEAD_t stPacketHead;
				memcpy( &stPacketHead, szRecvData, sizeof(stPacketHead));

				nResult = recv( sockClient, &szRecvData[NETHEAD_SIZE], stPacketHead.dwSize, 0 );
				AnalyseRecvData( szRecvData, sockClient );
			}
			break;
		case FD_CLOSE:
			{
				wchar_t strText[20];
				wsprintf(strText, _T("SocketThreadProc4-ACScanClose%d\n"), sockClient);
				OutputDebugString(strText);
				//	PostMessage(hStatusBar, SB_SETTEXT, 
				//		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);

				if ( stNetworkEvents.iErrorCode[FD_READ_BIT] != 0 )  // 发生网络错误
				{
					PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("Error7"));
					break;
				}				
				BOOL bRet = WSACloseEvent( event );
				if (!bRet)
				{
					MessageBox(NULL, _T("closeevent fail"),_T("ndt_ce"), MB_OK);
				}

				for(vector<SOCKET>::iterator iter = gVecSockACScan.begin(); iter!= gVecSockACScan.end();)
				{
					if(*iter == sockClient)
					{
						wchar_t strText[20];
						wsprintf(strText, _T("ACScanDelete%d"), sockClient);
						PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)strText);
						iter = gVecSockACScan.erase(iter);
						OutputDebugString(strText);
					}
					else
					{
						++iter;
					}
				}

				BOOL bRet1 = closesocket( sockClient );
				if (bRet1 == 0)
				{
				}
				else
				{
					MessageBox(NULL, _T("关闭socket失败1"),_T("ndt_ce"), MB_OK);
				}

				if(gVecSockACScan.size() == 0)
				{
					gbAscanDataStoreSend = FALSE;
				}
				wsprintf(strText, _T("SocketThreadProc4-ACScanCloseOk%d\n"), sockClient);
				OutputDebugString(strText);
			}
			return 1;
		default:
			break;
		}
	}
#endif
}
//yachang.huang add end on 2013-12-19

DWORD GetWorkingFolder(LPTSTR lpszName, DWORD nCapacity)
{
	if (lpszName != NULL && nCapacity > 0)
		lpszName[0] = _T('\0');
	TCHAR szExeName[MAX_PATH];
	DWORD nSize = ::GetModuleFileName(GetModuleHandle(NULL), szExeName, MAX_PATH);
	if (nSize == 0)
	{
		return 0;
	}
	do {
		nSize--;
	} while(szExeName[nSize] != TCHAR('\\') && nSize > 0);

	if (nSize > 0)
		nSize++;
	if (nSize > nCapacity - 1)
	{
		return nSize;
	}

	if (lpszName == NULL || nCapacity == 0)
	{
		return nSize;
	}

	//	wcsncpy(lpszName, szExeName, nSize);
	memcpy(lpszName, szExeName, nSize*sizeof(TCHAR));
	lpszName[nSize] = _T('\0');

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("GetWorkingFolder"),"Leave");

	return nSize;
}
