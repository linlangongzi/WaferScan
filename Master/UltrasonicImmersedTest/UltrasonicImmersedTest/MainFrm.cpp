/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 应用程序MainFrame框架程序的实现
*Filename : MainFrm.cpp
*
*@author       Song Chenguang zhaoli
*@version      1.0         Date: 2011-7-1 
*/

#include "stdafx.h"
#include "CScanDlgBar.h" 
#include "ExButton.h"
#include "UltrasonicImmersedTest.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "MovePathView.h"
#include "SysParam.h"
#include "MechanicalControl.h"
#include "MechanicalControlPanel.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"

#include "MainFrm.h"
#include "UltrasonicImmersedTestDoc.h"
#include "UltrasonicImmersedTestView.h"
#include "ZoomView.h"

#include "TailDecreasePipeParam.h"
#include "TailDecreasePipeSketch.h"		// machao add on 2013-08-8
#include "TailDecreasePipeUnfold.h"		// machao add on 2013-08-8
#include "TailDecreasePipe.h"			// machao add on 2013-08-8
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
#include "GlobalFunction.h"
#include "FeatureInfoWnd.h"
#include "SaveDefineDlg.h"
#include "DacCurveDlg.h"
#include "BScanView.h"
#include <dbt.h>
#include "elepopular.h"
#include "TcgCurveDlg.h"
#include "CPrintView.h"	// temp
#include "AScanDataSocket.h"
#include "MsgDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern DWORD g_dwActiveChannel;
extern CMutex g_csCard;
const UINT ID_FEATURE_WND = 1234;
extern HANDLE* hSaveDMADataEvent;

// Song chenguang add start 2012-03-06
// 加密狗插拔判断
// function prototype for getting HID GUID
typedef void (__stdcall *FUNC_TYPE_HIDGUID) (
	OUT LPGUID HidGuid
	);
FUNC_TYPE_HIDGUID _HidD_GetHidGuid = NULL;
// EliteE USB version GUID
static GUID GUID_CLASS_ITOKEN2 =
	{0x3c1afcf8, 0x4a1b, 0x4ba7, {0xbd, 0x8c, 0x91, 0xe3, 0x63, 0xc8, 0xae,0x57}};
static GUID GUID_CLASS_ITOKEN2_HID;
// Song chenguang add end 2012-03-06
// CMainFrame


IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_PATH_VIEW, &CMainFrame::OnPathView)
	ON_UPDATE_COMMAND_UI(ID_PATH_VIEW, &CMainFrame::OnUpdatePathView)
	ON_COMMAND(ID_A_SCAN_VIEW, &CMainFrame::OnAScanView)
	ON_UPDATE_COMMAND_UI(ID_A_SCAN_VIEW, &CMainFrame::OnUpdateAScanView)
	ON_COMMAND(ID_C_SCAN_VIEW, &CMainFrame::OnCScanView)
	ON_UPDATE_COMMAND_UI(ID_C_SCAN_VIEW, &CMainFrame::OnUpdateCScanView)
	// WangQianfeng delete start on 2013-06-28
	ON_COMMAND(ID_B_SCAN_VIEW, &CMainFrame::OnBScanView)
	ON_UPDATE_COMMAND_UI(ID_B_SCAN_VIEW, &CMainFrame::OnUpdateBScanView)
	// WangQianfeng delete end on 2013-06-28
	ON_COMMAND(ID_START_A_SCAN, &CMainFrame::OnStartAScan)
	ON_COMMAND(ID_STOP_A_SCAN, &CMainFrame::OnStopAScan)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(IDM_CHANNEL_PANEL, &CMainFrame::OnChannelPanel)
	ON_UPDATE_COMMAND_UI(IDM_CHANNEL_PANEL, &CMainFrame::OnUpdateChannelPanel)
	ON_COMMAND(IDM_MECHANICAL_CONTROL, &CMainFrame::OnMechanicalControl)
	ON_UPDATE_COMMAND_UI(IDM_MECHANICAL_CONTROL, &CMainFrame::OnUpdateMechanicalControl)
	// WangQianfeng add start on 2013-06-28
	ON_COMMAND(IDM_CSCAN_PARAM, &CMainFrame::OnPictrueParam)
	ON_UPDATE_COMMAND_UI(IDM_CSCAN_PARAM, &CMainFrame::OnUpdatePictrueParam)
	// WangQianfeng add end on 2013-06-28
	ON_COMMAND(ID_VIEW_TOOLBAR, &CMainFrame::OnViewToolbar)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBAR, &CMainFrame::OnUpdateViewToolbar)
	ON_COMMAND(IDM_VIEW_STANDARD_BAR, &CMainFrame::OnViewStandardBar)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_STANDARD_BAR, &CMainFrame::OnUpdateViewStandardBar)
	ON_COMMAND(IDM_VIEW_DRAW_BAR, &CMainFrame::OnViewDrawBar)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_DRAW_BAR, &CMainFrame::OnUpdateViewDrawBar)
	ON_COMMAND(IDM_VIEW_BAR, &CMainFrame::OnViewBar)
	ON_UPDATE_COMMAND_UI(IDM_VIEW_BAR, &CMainFrame::OnUpdateViewBar)
	ON_UPDATE_COMMAND_UI(ID_START_A_SCAN, &CMainFrame::OnUpdateStartAScan)
	ON_UPDATE_COMMAND_UI(ID_STOP_A_SCAN, &CMainFrame::OnUpdateStopAScan)
	ON_COMMAND(ID_CSCAN_PARAM, &CMainFrame::OnCscanParam)
	ON_UPDATE_COMMAND_UI(ID_CSCAN_PARAM, &CMainFrame::OnUpdateCscanParam)

	ON_UPDATE_COMMAND_UI(ID_FEATURE_INFO, &CMainFrame::OnUpdateFeatureInfo)
	ON_COMMAND(ID_FEATURE_INFO, &CMainFrame::OnFeatureInfo)
	ON_COMMAND(ID_DAC_STANDARD, &CMainFrame::OnDacStandard)
	ON_UPDATE_COMMAND_UI(ID_DAC_STANDARD, &CMainFrame::OnUpdateDacStandard)
	ON_UPDATE_COMMAND_UI(ID_DAC_USER_DEFINE, &CMainFrame::OnUpdateDacUserDefine)
	ON_MESSAGE(WM_HIDE_FEATURE, &CMainFrame::OnCloseFeatureInfoWnd)
	ON_COMMAND(ID_SAVE_DATA_DEFINE, &CMainFrame::OnSaveDataDefine)
	ON_UPDATE_COMMAND_UI(ID_SAVE_A_SCAN, &CMainFrame::OnUpdateSaveAScan)
	ON_COMMAND(ID_CSCAN_RULER_MEASURE, &CMainFrame::OnCscanRulerMeasure)
	ON_UPDATE_COMMAND_UI(ID_CSCAN_RULER_MEASURE, &CMainFrame::OnUpdateCscanRulerMeasure)
	ON_COMMAND(ID_DAC_CURVE, &CMainFrame::OnDacCurve)
	ON_UPDATE_COMMAND_UI(ID_DAC_CURVE, &CMainFrame::OnUpdateDacCurve)
	ON_COMMAND(ID_DRAW_ARC, &CMainFrame::OnDrawArc)
	ON_COMMAND(ID_DRAW_DAO, &CMainFrame::OnDrawDao)
	ON_COMMAND(ID_DRAW_SELECT, &CMainFrame::OnDrawSelect)
	ON_COMMAND(ID_DRAW_HOLE, &CMainFrame::OnDrawHole)
	ON_COMMAND(ID_DRAW_LINE, &CMainFrame::OnDrawLine)
	ON_COMMAND(ID_DRAW_ARC_CENETR, &CMainFrame::OnDrawArcCenetr)
	ON_COMMAND(ID_DRAW_CIRCLE, &CMainFrame::OnDrawCircle)
	ON_COMMAND(ID_DRAW_CIRCLE_SAN, &CMainFrame::OnDrawCircleSan)
	ON_COMMAND(ID_DRAW_ARCLINE, &CMainFrame::OnDrawArcline)
	ON_COMMAND(ID_DRAW_RECT, &CMainFrame::OnDrawRect)
	ON_COMMAND(ID_DRAW_ELLIPSE, &CMainFrame::OnDrawEllipse)
	ON_COMMAND(ID_DRAW_POLYGON, &CMainFrame::OnDrawPolygon)
	ON_COMMAND(ID_DRAW_RUNWAY, &CMainFrame::OnDrawRunway)
	ON_COMMAND(ID_DECOMPOSE, &CMainFrame::OnDecompose)
	ON_COMMAND(ID_EDIT_UNDO, &CMainFrame::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CMainFrame::OnEditRedo)
	ON_COMMAND(ID_EDIT_SELECT_ALL, &CMainFrame::OnEditSelectAll)
	ON_COMMAND(ID_EDIT_FIND, &CMainFrame::OnEditFind)
	ON_COMMAND(ID_EDIT_SHAI, &CMainFrame::OnEditShai)
	ON_COMMAND(ID_EDIT_CUT, &CMainFrame::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CMainFrame::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CMainFrame::OnEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, &CMainFrame::OnEditClear)
	ON_COMMAND(ID_EDIT_FAN, &CMainFrame::OnEditFan)
	ON_COMMAND(ID_EDIT_MIRROR, &CMainFrame::OnEditMirror)
	ON_COMMAND(ID_EDIT_ROTA, &CMainFrame::OnEditRota)
	ON_COMMAND(ID_TOOL_CAN, &CMainFrame::OnToolCan)
	ON_COMMAND(ID_TOOL_COLOR, &CMainFrame::OnToolColor)
	ON_COMMAND(ID_TOOL_JXIAN, &CMainFrame::OnToolJxian)
	ON_COMMAND(ID_TOOL_JPIAN, &CMainFrame::OnToolJpian)
	ON_COMMAND(ID_TOOL_EDIT, &CMainFrame::OnToolEdit)
	ON_COMMAND(ID_TOOL_RESET, &CMainFrame::OnButtonReposition)
	ON_COMMAND(ID_TOOL_RUN, &CMainFrame::OnToolRun)
	ON_COMMAND(ID_IP_CONFIG, &CMainFrame::OnIpConfig)
	ON_COMMAND(ID_VIEW_SHI, &CMainFrame::OnViewShi)
	ON_COMMAND(ID_VIEW_TU, &CMainFrame::OnViewTu)
	ON_COMMAND(ID_VIEW_WINDOW, &CMainFrame::OnViewWindow)
	ON_COMMAND(ID_ROTATE_IMAGE, &CMainFrame::OnRotateImage)
	ON_COMMAND(ID_CSCAN_SMOOTH, &CMainFrame::OnCscanSmooth)
	ON_COMMAND(ID_CSCAN_MF, &CMainFrame::OnCscanMf)
	ON_COMMAND(ID_SET_UNIT, &CMainFrame::OnSetUnit)
	ON_COMMAND(ID_RULER_MEASURE, &CMainFrame::OnRulerMeasure)
	ON_COMMAND(ID_TRIANGLE_MEASURE, &CMainFrame::OnTriangleMeasure)
	ON_COMMAND(ID_ELLIPSE_MEASURE, &CMainFrame::OnEllipseMeasure)
	ON_COMMAND(ID_POLYGON_MEASURE, &CMainFrame::OnPolygonMeasure)	 //yachang.huang add start on 2012-5-31
	ON_COMMAND(ID_MEASURE_ON_OFF, &CMainFrame::OnMeasureOnOff)
	ON_COMMAND(IDC_BUTTON_REPOSITION, &CMainFrame::OnButtonReposition)
	ON_COMMAND(IDC_BUTTON_RUN, &CMainFrame::OnButtonRun)
	ON_COMMAND(IDC_BUTTON_SYSPARAM, &CMainFrame::OnButtonSysparam)
	ON_COMMAND(ID_PROBE_SET, &CMainFrame::OnProbeSet)

	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	// WangQianfeng add start on 2013-06-04
	ON_WM_ACTIVATE()
	// WangQianfeng add end on 2013-06-04

	ON_COMMAND(ID_DATA_DIRECTORY_SET, &CMainFrame::OnDataDirectorySet)
	ON_UPDATE_COMMAND_UI(ID_MEASURE_ON_OFF, &CMainFrame::OnUpdateMeasureOnOff)
	ON_COMMAND(ID_C_SCAN_VIEW2, &CMainFrame::OnCScanView2)
	ON_UPDATE_COMMAND_UI(ID_C_SCAN_VIEW2, &CMainFrame::OnUpdateCScanView2)
	ON_COMMAND(ID_START_MONITOR_DATA, &CMainFrame::TransAScanData)
	ON_COMMAND(ID_STOP_MONITOR_DATA, &CMainFrame::StopTransAscanData)
	ON_COMMAND(ID_START_FEATURE_DATA, &CMainFrame::TransFeatureData)
	ON_COMMAND(ID_STOP_FEATURE_DATA, &CMainFrame::StopTransFeatureData)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMainFrame::OnFilePrintPreview)
	// WangQianfeng add start on 2012-08-22
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_COMMAND(ID_CHANNEL_TOOLBAR, &CMainFrame::OnChannelToolBarShow)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_TOOLBAR, &CMainFrame::OnUpdateChannelToolBarShow)

	ON_COMMAND(ID_MECHANICAL_TOOLBAR, &CMainFrame::OnMechanicalToolBarShow)
	ON_UPDATE_COMMAND_UI(ID_MECHANICAL_TOOLBAR, &CMainFrame::OnUpdateMechanicalToolBarShow)

	ON_COMMAND(ID_CSCANPARAM_TOOLBAR, &CMainFrame::OnCScanparamToolBarShow)
	ON_UPDATE_COMMAND_UI(ID_CSCANPARAM_TOOLBAR, &CMainFrame::OnUpdateCScanparamToolBarShow)


	ON_COMMAND(ID_BTN_READ_PARAM, &CMainFrame::OnReadParam)
	ON_COMMAND(ID_BTN_SAVE_PARAM, &CMainFrame::OnSaveParam)

	ON_COMMAND(ID_BTN_OPEN_PATHFILE, &CMainFrame::OnOpenPathFile)
	ON_COMMAND(ID_BTN_SAVE_PATHFILE, &CMainFrame::OnSavePathFile)

	ON_COMMAND(ID_BTN_SAVE_DATA, &CMainFrame::OnSaveData)
	ON_COMMAND(ID_BTN_READ_FILE, &CMainFrame::OnReadData)
	ON_COMMAND(ID_BTN_SAVE_CPARAM, &CMainFrame::OnSaveCParam)
	ON_COMMAND(ID_BTN_READ_CPARAM, &CMainFrame::OnReadCParam)
	// WangQianfeng add end on 2012-08-22
	ON_WM_DEVICECHANGE() //注册消息映射
	ON_COMMAND(ID_TCG_CURVE, &CMainFrame::OnTcgCurve)
	ON_UPDATE_COMMAND_UI(ID_TCG_CURVE, &CMainFrame::OnUpdateTcgCurve)
	ON_COMMAND(IDM_TASK_PANEL, &CMainFrame::OnTaskToolbar)
	ON_UPDATE_COMMAND_UI(IDM_TASK_PANEL, &CMainFrame::OnUpdateTaskToolbar)
#if _HAFEI
	ON_COMMAND(IDC_TAIL_DECREASE_PIPE_PARAM, &CMainFrame::OnTailDecreasePipeParam)
#endif
	ON_MESSAGE (WM_USER_DIALOG_DESTROYED, OnDialogDestroyed)	// machao add on 2013-08-19
	ON_COMMAND(ID_DUPLICATE_DRAW_ONCE, &CMainFrame::OnDuplicateDrawOnce)
	ON_UPDATE_COMMAND_UI(ID_DUPLICATE_DRAW_ONCE, &CMainFrame::OnUpdateDuplicateDrawOnce)
	ON_COMMAND(ID_CMD_CONNECT, &CMainFrame::CommandConnect)
	ON_COMMAND(ID_CMD_CLOSE, &CMainFrame::CommandClose)
	ON_COMMAND(ID_CMD_S_CONNECT, &CMainFrame::OnCommandSConnect)
	ON_COMMAND(ID_CMD_S_CLOSE, &CMainFrame::OnCommandSClose)
	ON_COMMAND(ID_A_SCAN_DATA_START, &CMainFrame::OnAScanDataStart)
	ON_COMMAND(ID_A_SCAN_DATA_STOP, &CMainFrame::OnAScanDataStop)
	END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构
CMainFrame::CMainFrame()
// machao add start on 2013-08-13
#if _HAFEI
: m_nASketch(96)
, m_nABSketch(400)
, m_nBSketch(68)
, m_nBCSketch(132)
, m_nCSketch(68)
, m_nCDSketch(400)
, m_nDSketch(96)
, m_nLeft1Start(100)
, m_nLeft1Stop(200)
, m_nLeft2Start(200)
, m_nLeft2Stop(250)
, m_nRight1Start(1160)
, m_nRight1Stop(1060)
, m_nRight2Start(1060)
, m_nRight2Stop(1010)
#endif
// machao add start on 2013-08-13
{
	// TODO: 在此添加成员初始化代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::CMainFrame"),"Enter");
	m_bPathDisplay = TRUE;
	m_bAScanDisplay = TRUE;
	m_bCScanDisplay = TRUE;
	m_bCScanDisplay2 = TRUE;
	// WangQianfeng delete start on 2013-06-28
	m_bBScanDisplay = FALSE;
	// WangQianfeng delete start on 2013-06-28

	m_bSplitterCreated = FALSE;

	m_bCardInit = FALSE;	

	m_bSelChannel = FALSE;
	m_bSetOne = FALSE;
	m_bIsRunning = FALSE;

	m_bAScanIsOpen = FALSE;

	m_pFeatureWnd = NULL;
	m_bFeatureWndDisplayed = FALSE;

	m_pDacCurveDlg = NULL;
	m_pTcgCurveDlg = NULL;
	m_bDacWndDisplayed = FALSE;
	m_bTcgWndDisplayed = FALSE;
	m_bSaveAScan = FALSE;

	m_strIP = _T("192.168.1.80");
	m_nPort = 3333;
	m_ReSendNum = 0;
	m_ReSendMaxNum = 10;

	m_nNetMonitorCount = 0;
	m_nNetFeatureCount = 0;

	CLSID clsid;
	if(CLSIDFromString(CComBSTR("ActiveXExtension.ProbeSet"), &clsid)==S_OK)
	{
		m_ProbeSet.CreateDispatch(clsid,NULL);
	}

	m_hDevNotify1 = NULL;
	m_hDevNotify2 = NULL;

	// wangbingfu add start on 2013-01-09
	memset(m_nPipeChannelSelect, 0, sizeof(m_nPipeChannelSelect));
	memset(m_nCScanSelect, 0, sizeof(m_nCScanSelect));
	m_nAxisSelect = 0;
	// wangbingfu add end on 2013-01-09
	m_bMoinitorDataRun = FALSE;		// Song Chenguang add on 2013-04-03
	// WangQianfeng add start on 2013-06-04
	m_bPressDirectKeys = FALSE;		// wangqianfeng add on 2013-06-03
	// WangQianfeng add end on 2013-06-04
	// Song Chenguang add start on 2013-06-04
#if _HAFEI
	m_f11LayerMin = 5;
	m_f11LayerMax = 5.5;
	m_f14LayerMin = 6;
	m_f14LayerMax = 6.5;
	m_f17LayerMin = 7;
	m_f17LayerMax = 7.5;
	m_f18LayerMin = 8;
	m_f18LayerMax = 8.5;
	m_fWeiJianW1 = 40;	// wangbingfu add on 2014-11-03
#endif
	// Song Chenguang add end on 2013-06-04
	m_bDuplicateDrawOnce = FALSE;
	m_bDuplicateDrawOnceEn = FALSE;
	m_dwDuplicateDrawCount = 0;		// Song Chenguang add on 2013-09-26
	m_bChangeZWAixs = FALSE;
//	m_bReceiveAScan = TRUE;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::CMainFrame"),"Leave");
}

CMainFrame::~CMainFrame()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::~CMainFrame"),"Enter");
	if(m_pFeatureWnd != NULL)
		delete m_pFeatureWnd;

 	if(m_pDacCurveDlg != NULL)
		delete m_pDacCurveDlg;

	if(m_pTcgCurveDlg != NULL)
		delete m_pTcgCurveDlg;	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::~CMainFrame"),"Leave");

}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Enter");
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave"); 
		return -1;
	}
	
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	WORD nAECNum = pApp->GetCardNum();
	m_bPlateStart = new BOOL[nAECNum];
	// yachang.huang  add start on 2012-11-02
	if (m_bPlateStart == NULL)
	{
		MessageBox(_T("CMainFrame::OnCreate m_bPlateStart"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;
	}
	// yachang.huang  add end on 2012-11-02

	m_bPlateEnd = new BOOL[nAECNum];
	// yachang.huang  add start on 2012-11-02
	if (m_bPlateEnd == NULL)
	{
		MessageBox(_T("CMainFrame::OnCreate m_bPlateEnd"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;
	}
	// yachang.huang  add end on 2012-11-02



	::ZeroMemory(m_bPlateStart, sizeof(BOOL) * nAECNum);
	::ZeroMemory(m_bPlateEnd, sizeof(BOOL) * nAECNum);

	m_pFeatureWnd = new CFeatureInfoWnd;

	// yachang.huang  add start on 2012-11-02
	if (m_pFeatureWnd == NULL)
	{
		MessageBox(_T("CMainFrame::OnCreate m_pFeatureWnd"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;
	}
	// yachang.huang  add end on 2012-11-02

	//Machao modified start on 2013-07-1
//	BOOL bRet2 = m_pFeatureWnd->Create(NULL, _T("闸门特征值"), WS_CHILD | WS_BORDER | WS_CAPTION | WS_THICKFRAME, CRect(0,0,300,300),
//		this, ID_FEATURE_WND);
	BOOL bRet2 = m_pFeatureWnd->Create(NULL, _T("闸门特征值"), WS_SYSMENU | WS_CHILD | WS_BORDER | WS_CAPTION | WS_THICKFRAME, CRect(0,0,300,300),
		this, ID_FEATURE_WND);
	//Machao modified end on 2013-07-1
	m_pFeatureWnd->ShowWindow(SW_HIDE);

	//创建直尺测量工具栏
	if (!m_wndCScanDlgBar.Create(this,IDD_CSCAN_DLGBAR,WS_CHILD|WS_VISIBLE|CBRS_ALIGN_BOTTOM|
		CBRS_GRIPPER|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC,IDD_CSCAN_DLGBAR))
	{
		TRACE0("Failed to create Dlgbar\n");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;      // fail to create
	}
	m_wndCScanDlgBar.SetWindowText(_T("测量信息栏"));
	//创建之后隐藏
	ShowControlBar(&m_wndCScanDlgBar,FALSE,FALSE,TRUE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;      // 未能创建
	}
	// WangQianfeng add start on 2012-08-22
	if (!m_wndChannelToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndChannelToolBar.LoadToolBar(IDR_TOOL_CHANNEL))
	{
		TRACE0("未能创建工具栏\n");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;      // 未能创建
	}

	if (!m_wndMechanicalToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndMechanicalToolBar.LoadToolBar(IDR_TOOL_MECHANICAL))
	{
		TRACE0("未能创建工具栏\n");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;      // 未能创建
	}

	if (!m_wndCScanParamToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndCScanParamToolBar.LoadToolBar(IDR_TOOL_CSCANPARAM))
	{
		TRACE0("未能创建工具栏\n");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;      // 未能创建
	}
	// WangQianfeng add end on 2012-08-22
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetPaneTextColor(0, RGB(128, 0, 64));

	int nStatusNum = sizeof(indicators)/sizeof(UINT);
	for (int i=0; i<nStatusNum; i++)
	{
		m_wndStatusBar.SetPaneWidth(i, 300);
	}
	// WangQianfeng add start on 2012-08-22
	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	m_wndToolBar.SetWindowText (strMainToolbarTitle);

	CString strChannelToolBarTitle;
	strChannelToolBarTitle.LoadString (IDS_CHANNEL_TOOLBAR);
	m_wndChannelToolBar.SetWindowText (strChannelToolBarTitle);

	CString strCScanParamToolBarTitle;
	strCScanParamToolBarTitle.LoadString (IDS_CSCANPARAM_TOOLBAR);
	m_wndCScanParamToolBar.SetWindowText (strCScanParamToolBarTitle);

	CString strMechanicalToolBarTitle;
	strMechanicalToolBarTitle.LoadString (IDS_MECHANICAL_TOOLBAR);
	m_wndMechanicalToolBar.SetWindowText (strMechanicalToolBarTitle);
	// WangQianfeng add end on 2012-08-22


	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndCScanDlgBar);

	// 通道信息框
	if (!m_wndChannelInfo.Create (_T("仪器参数"), this, CRect (0, 0, 500, 500),
		TRUE, ID_VIEW_CHANNEL_INFO,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar\n");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return FALSE;      // fail to create
	}

	//机械控制
	if (!m_wndMechanicalControl.Create(_T("机械控制"),this,CRect(0,0,500,500),
		TRUE,ID_VIEW_MECHANICALCONTROL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar\n");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return FALSE;      // fail to create
	}
	//C扫参数
	if (!m_wndCScanParamDockBar.Create(_T("图像参数"),this,CRect(0,0,800,800),
		TRUE,ID_VIEW_MECHANICALCONTROL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar\n");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return FALSE;      // fail to create
	}

	// Song Chenguang add start on 2013-04-16
	// 任务面板
	if (!m_wndTaskDockBar.Create(_T("任务设置"),this,CRect(0,0,800,800),
		TRUE,ID_VIEW_MECHANICALCONTROL,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar\n");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return FALSE;      // fail to create
	}
	// Song Chenguang add end on 2013-04-16
	
	// TODO: 如果不需要工具栏可停靠，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	// WangQianfeng add start on 2012-08-22
	m_wndChannelToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMechanicalToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndCScanParamToolBar.EnableDocking(CBRS_ALIGN_ANY);
	// WangQianfeng add end on 2012-08-22


	m_wndChannelInfo.EnableDocking(CBRS_ALIGN_RIGHT);
	m_wndMechanicalControl.EnableDocking(CBRS_ALIGN_RIGHT);
	m_wndCScanParamDockBar.EnableDocking(CBRS_ALIGN_RIGHT);
	m_wndTaskDockBar.EnableDocking(CBRS_ALIGN_ANY);		// Song Chenguang add on 2013-04-16

	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);	// 自动隐藏
	DockControlBar(&m_wndToolBar);
	// WangQianfeng add start on 2012-08-22
	DockControlBar(&m_wndChannelToolBar);
	DockControlBar(&m_wndMechanicalToolBar);
	DockControlBar(&m_wndCScanParamToolBar);
	// WangQianfeng add end on 2012-08-22

	DockControlBar(&m_wndChannelInfo);
	m_wndMechanicalControl.AttachToTabWnd(&m_wndChannelInfo,BCGP_DM_STANDARD, FALSE, NULL);
	m_wndCScanParamDockBar.AttachToTabWnd(&m_wndChannelInfo,BCGP_DM_STANDARD, FALSE, NULL);
	m_wndTaskDockBar.AttachToTabWnd(&m_wndChannelInfo,BCGP_DM_STANDARD, FALSE, NULL);	// Song Chenguang add on 2013-04-16

	SetTimer(TIMER_THREAD_PROCESS, 100, NULL);
	SetTimer(TIMER_NET_JUDGE, NET_JUDGE_TIME, NULL);
	
	// wangbingfu add start on 2014-12-31
	m_dwTimeCount = GetTickCount();
	UINT unOneHour = 60*60*1000;//60*60*1000;
	SetTimer(TIMER_TIME_COUNT, unOneHour, NULL);
	// wangbingfu add end on 2014-12-31

	m_bFeatureWndDisplayed = FALSE;
	
	
	//自定义DAC曲线窗口
	m_pDacCurveDlg = new CDacCurveDlg;
	// yachang.huang  add start on 2012-11-02
	if (m_pDacCurveDlg == NULL)
	{
		MessageBox(_T(" CMainFrame::OnCreate m_pDacCurveDlg"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;
	}
	// yachang.huang  add end on 2012-11-02

	if(m_pDacCurveDlg->GetSafeHwnd() == NULL)
	{
		m_pDacCurveDlg->Create(IDD_DIALOG_DAC_CURVE, this);
		m_pDacCurveDlg->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}

	//自定义TCG曲线窗口
	m_pTcgCurveDlg = new CTcgCurveDlg;

	// yachang.huang  add start on 2012-11-02
	if (m_pTcgCurveDlg == NULL)
	{
		MessageBox(_T("CMainFrame::OnCreate m_pTcgCurveDlg"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;
	}
	// yachang.huang  add end on 2012-11-02

	if(m_pTcgCurveDlg->GetSafeHwnd() == NULL)
	{
		m_pTcgCurveDlg->Create(IDD_DIALOG_TCG_CURVE, this);
		m_pTcgCurveDlg->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}
	m_pTcgCurveDlg->ShowWindow(SW_HIDE);
	

	// 修改菜单
	CMenu* pMenu = GetMenu();
	pMenu->DeleteMenu(8, MF_BYPOSITION);	// Song Chenguang add on 2013-12-21
	CMenu* pSubMenu = pMenu->GetSubMenu(3);	// 操作

	pSubMenu->DeleteMenu(1, MF_BYPOSITION);
	pSubMenu->DeleteMenu(0, MF_BYPOSITION);

	CMenu* pSubMenu2 = pSubMenu->GetSubMenu(7);	// 操作->测量
	pSubMenu2->DeleteMenu(0, MF_BYPOSITION);

	
	pMenu->DeleteMenu(6, MF_BYPOSITION);	// 查看
	pSubMenu = pMenu->GetSubMenu(5);		// 工具
	pSubMenu->DeleteMenu(5, MF_BYPOSITION);
	pSubMenu->DeleteMenu(4, MF_BYPOSITION);
	pSubMenu->DeleteMenu(3, MF_BYPOSITION);
	pSubMenu->DeleteMenu(2, MF_BYPOSITION);
	pSubMenu->DeleteMenu(1, MF_BYPOSITION);
	pSubMenu->DeleteMenu(0, MF_BYPOSITION);
	pMenu->DeleteMenu(4, MF_BYPOSITION);	// 绘图

	CMenu* pSubMenu5 = pMenu->GetSubMenu(2); // 视图
	pSubMenu5->DeleteMenu(5, MF_BYPOSITION);
	// WangQianfeng delete start on 2012-08-22
	//pSubMenu5->DeleteMenu(3, MF_BYPOSITION);
	// WangQianfeng delete end on 2012-08-22
	pSubMenu5->DeleteMenu(2, MF_BYPOSITION);	//? 应该可以打开
	pSubMenu5->DeleteMenu(1, MF_BYPOSITION);
	pSubMenu5->DeleteMenu(0, MF_BYPOSITION);

	CMenu* pSubMenu6 = pMenu->GetSubMenu(0); 
	pSubMenu6->DeleteMenu(5, MF_BYPOSITION);
	pSubMenu6->DeleteMenu(4, MF_BYPOSITION);
	pSubMenu6->DeleteMenu(2, MF_BYPOSITION);
	pSubMenu6->DeleteMenu(1, MF_BYPOSITION);
	pSubMenu6->DeleteMenu(0, MF_BYPOSITION);

	pMenu->DeleteMenu(1, MF_BYPOSITION); 

	// Song Chenguang add start on 2013-07-11
	// 哈飞菜单
#if _HAFEI
	m_menuHafei.LoadMenu(IDR_MENU_HA_FEI);
	CMenu* pHafeiSubMenu = m_menuHafei.GetSubMenu(0);
	pMenu->InsertMenu(4, MF_BYPOSITION | MF_POPUP | MF_STRING, UINT_PTR(pHafeiSubMenu->GetSafeHmenu()), _T("哈飞"));
#endif
	// Song Chenguang add end on 2013-07-11

	// WangQianfeng delete start on 2013-06-28
	//删除视图菜单下“显示B扫”菜单项
	//CMenu* pSubViewMenu = pMenu->GetSubMenu(1);
	//pSubViewMenu->GetSubMenu(2)->DeleteMenu(4, MF_BYPOSITION);
	// WangQianfeng delete end on 2013-06-28

	m_AscanSocket = new CARSSocket[nAECNum];
	// yachang.huang  add start on 2012-11-02
	if (m_AscanSocket == NULL)
	{
		MessageBox(_T("CMainFrame::OnCreate m_AscanSocket"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;
	}
	// yachang.huang  add end on 2012-11-02

	m_CommandSocket = new CCommandSocket[nAECNum];
	// yachang.huang  add start on 2012-11-02
	if (m_CommandSocket == NULL)
	{
		MessageBox(_T("CMainFrame::OnCreate m_CommandSocket"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;
	}
	// yachang.huang  add end on 2012-11-02
	m_DataSocket = new CDataSocket[nAECNum];
	// yachang.huang  add start on 2012-11-02
	if (m_DataSocket == NULL)
	{
		MessageBox(_T("CMainFrame::OnCreate m_DataSocket"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;
	}
	// yachang.huang  add end on 2012-11-02
	m_stYiQiVer = new CSCAN_VERSION_DATA_t[nAECNum];
	// yachang.huang  add start on 2012-11-02
	if (m_stYiQiVer == NULL)
	{
		MessageBox(_T("CMainFrame::OnCreate m_stYiQiVer"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
		return -1;
	}
	// yachang.huang  add end on 2012-11-02
	::ZeroMemory(m_stYiQiVer, sizeof(CSCAN_VERSION_DATA_t) * nAECNum);

	// Song Chenguang add start on 2013-12-17
	m_AScanDataSocket = new CAScanDataSocket[nAECNum];
	if (m_AScanDataSocket == NULL)
	{
		MessageBox(_T("CMainFrame::OnCreate m_AScanDataSocket"), _T("警告"), MB_OK|MB_ICONERROR);
		return -1;
	}
	// Song Chenguang add end on 2013-12-17

	// 加密狗
	InitDogNotify();
	

	ReadPipeIni();
	// wangbingfu add end on 2013-01-09
	
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCreate"),"Leave");
	return 0;
}

void CMainFrame::InitDogNotify()
{
	// Get HID device GUID	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::InitDogNotify"),"Leave");
	if (_HidD_GetHidGuid == NULL)
	{
		HINSTANCE hMod = LoadLibrary(_T("hid.dll"));
		_HidD_GetHidGuid = (FUNC_TYPE_HIDGUID)GetProcAddress(hMod, "HidD_GetHidGuid");
		ASSERT(_HidD_GetHidGuid != NULL);

		// Get HID device GUID 
		_HidD_GetHidGuid(&GUID_CLASS_ITOKEN2_HID);
		FreeLibrary(hMod);
	}
	// Register ITOKEN2 device notification
	DEV_BROADCAST_DEVICEINTERFACE dbf;
	::ZeroMemory(&dbf, sizeof(DEV_BROADCAST_DEVICEINTERFACE));
	dbf.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	dbf.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	dbf.dbcc_classguid = GUID_CLASS_ITOKEN2;

	m_hDevNotify2 = RegisterDeviceNotification( this->GetSafeHwnd(),
		&dbf,DEVICE_NOTIFY_WINDOW_HANDLE);
	ASSERT(m_hDevNotify2 != NULL);

	::ZeroMemory(&dbf, sizeof(DEV_BROADCAST_DEVICEINTERFACE));
	dbf.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	dbf.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	dbf.dbcc_classguid = GUID_CLASS_ITOKEN2_HID;
	// Register ITOKEN2 HID device notification
	m_hDevNotify1 = RegisterDeviceNotification( this->GetSafeHwnd(),
		&dbf,DEVICE_NOTIFY_WINDOW_HANDLE);

	ASSERT(m_hDevNotify1 != NULL);
	// WangQianfeng add start on 2012-08-22
	//右键显示隐藏工具栏
	EnableControlBarMenu (	
		TRUE,				// Enable
		ID_VIEW_CUSTOMIZE, 	// Customize command ID
		_T("Customize..."),	// Customize command text
		NULL);	// Menu items with this ID will be replaced by
	// WangQianfeng add end on 2012-08-22
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::InitDogNotify"),"Leave");

}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCreateClient"),"Leave");
	// yachang.huang  add start on 2012-11-02
	if (pContext == NULL)
	{
		MessageBox(_T("CMainFrame::OnCreateClient pContext"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnCreateClient"),"Leave");
		return FALSE;
	}
	// yachang.huang  add end on 2012-11-02

	CRect rect;
	GetClientRect(&rect);
	// WangQianfeng modify start on 2013-06-28
	int nHeight = rect.Height() / SPLITTER_VIEW_NUM;
	VERIFY(m_wndSplitter.CreateStatic(this, SPLITTER_VIEW_NUM, 1));
	// WangQianfeng modify start on 2013-06-28
	VERIFY(m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMovePathView), CSize(100, nHeight), pContext));
	VERIFY(m_wndSplitter.CreateView(1, 0, RUNTIME_CLASS(CUltrasonicImmersedTestView), CSize(100, nHeight), pContext));
	VERIFY(m_wndSplitter.CreateView(2, 0, RUNTIME_CLASS(CCScanView), CSize(100, nHeight), pContext));
	VERIFY(m_wndSplitter.CreateView(3, 0, RUNTIME_CLASS(CCScanView), CSize(100, nHeight), pContext));
	// WangQianfeng delete start on 2013-06-28
	VERIFY(m_wndSplitter.CreateView(4, 0, RUNTIME_CLASS(CBScanView), CSize(100, nHeight), pContext));
	// WangQianfeng delete start on 2013-06-28

	m_bSplitterCreated = TRUE;

	m_wndChannelInfo.SetContext(pContext);
	m_wndMechanicalControl.SetContext(pContext);
	m_wndCScanParamDockBar.SetContext(pContext);
	m_wndTaskDockBar.SetContext(pContext);	// Song Chenguang add on 2013-04-16
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCreateClient"),"Leave");
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreCreateWindow"),"Enter");
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::PreCreateWindow"),"Leave");
		return FALSE;
	}
	//  CREATESTRUCT cs 来修改窗口类或样式 
	
	m_strTitle = _T("水浸超声检测系统");
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreCreateWindow"),"Leave");
	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序




void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CBCGPFrameWnd::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// 使窗口大小变化时，相应改变视图大小，使它们保持等分
	ResetView();
}

int CMainFrame::GetDisplayedViewNum()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::GetDisplayedViewNum"),"Enter");
	int nNum = 0;
	if (m_bPathDisplay)
		nNum++;

	if (m_bAScanDisplay)
		nNum++;

	if (m_bCScanDisplay)
		nNum++;

	if (m_bCScanDisplay2)
		nNum++;
	// wangbingfu add start on 2015-01-19
	if (m_bBScanDisplay)
		nNum++;
	// wangbingfu add end on 2015-01-19


	return nNum;
}
void CMainFrame::OnPathView()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnPathView"),"Enter");
	// WangQianfeng add start on 2012-06-27
	if (1 == GetDisplayedViewNum() && TRUE == m_bPathDisplay)
	{
		return;
	}
	// WangQianfeng add end on 2012-06-27
	m_bPathDisplay = !m_bPathDisplay;

	ResetView();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnPathView"),"Leave");
}

void CMainFrame::OnUpdatePathView(CCmdUI *pCmdUI)
{

	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdatePathView pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdatePathView"),"Leave");
		return;
	}


	if (m_bPathDisplay)
	{
		pCmdUI->SetCheck(BST_CHECKED);
	} 
	else
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}

}

void CMainFrame::OnAScanView()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnAScanView"),"Enter");
	// WangQianfeng add start on 2012-06-27
	if (1 == GetDisplayedViewNum() && TRUE == m_bAScanDisplay)
	{
		return;
	}
	// WangQianfeng add end on 2012-06-27
	m_bAScanDisplay = !m_bAScanDisplay;

	ResetView();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnAScanView"),"Leave");
}

void CMainFrame::OnUpdateAScanView(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	// yachang.huang  add start on 2012-11-02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateAScanView"),"Enter");
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateAScanView pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateAScanView"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	if (m_bAScanDisplay)
	{
		pCmdUI->SetCheck(BST_CHECKED);
	} 
	else
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateAScanView"),"Leave");
}

void CMainFrame::OnCScanView()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCScanView"),"Enter");
	// WangQianfeng add start on 2012-06-27
	if (1 == GetDisplayedViewNum() && TRUE == m_bCScanDisplay)
	{
		return;
	}
	// WangQianfeng add end on 2012-06-27
	m_bCScanDisplay = !m_bCScanDisplay;
	ResetView();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCScanView"),"Leave");
}

void CMainFrame::OnUpdateCScanView(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateCScanView"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateCScanView pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateCScanView"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02

	if (m_bCScanDisplay)
	{
		pCmdUI->SetCheck(BST_CHECKED);
	} 
	else
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateCScanView"),"Leave");
}

void CMainFrame::ResetView()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::ResetView"),"Enter");
	int nNum = GetDisplayedViewNum();	

	CRect rect;
	GetClientRect(&rect);
	int nHeight = 0;

	if (nNum > 0)
	{
		nHeight = rect.Height() / nNum;
	}	

	if(m_bSplitterCreated)
	{ 
		if(m_bPathDisplay)
			m_wndSplitter.SetRowInfo(0,   nHeight,   10); 
		else
			m_wndSplitter.SetRowInfo(0,   0,   0); 

		if(m_bAScanDisplay)
			m_wndSplitter.SetRowInfo(1,   nHeight,   10); 
		else
			m_wndSplitter.SetRowInfo(1,   0,   0); 

		if(m_bCScanDisplay)
			m_wndSplitter.SetRowInfo(2,   nHeight,   10); 
		else
			m_wndSplitter.SetRowInfo(2,   0,   0); 

		if(m_bCScanDisplay2)
			m_wndSplitter.SetRowInfo(3,   nHeight,   10); 
		else
			m_wndSplitter.SetRowInfo(3,   0,   0); 
		// WangQianfeng delete start on 2013-06-28
		if(m_bBScanDisplay)
			m_wndSplitter.SetRowInfo(4,   nHeight,   10); 
		else
			m_wndSplitter.SetRowInfo(4,   0,   0); 
		// WangQianfeng delete end on 2013-06-28

		m_wndSplitter.RecalcLayout(); 
	} 
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::ResetView"),"Leave");
}
void CMainFrame::OnStartAScan()
{
	// TODO: Add your command handler code here	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnStartAScan"),"Enter");
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	WORD nAECNum = pApp->GetCardNum();
	for (int i=0; i<nAECNum; i++)
	{
		m_bPlateStart[i] = TRUE;
	}	

	m_bAScanIsOpen = TRUE;//zhaoli add 2011 08 02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnStartAScan"),"Leave");

}

void CMainFrame::OnStopAScan()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnStopAScan"),"Enter");
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	WORD nAECNum = pApp->GetCardNum();
	for (int i=0; i<nAECNum; i++)
	{
		m_bPlateEnd[i] = TRUE;
	}

	m_bAScanIsOpen = FALSE;//zhaoli add 2011 08 02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnStopAScan"),"Leave");
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{	
	// TODO: Add your message handler code here and/or call default
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnTimer"),"Enter");
	if (!m_bSplitterCreated)
	{
        LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnTimer"),"Leave");
		return;
	}

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CUltrasonicImmersedTestView* pAScanView = (CUltrasonicImmersedTestView*)m_wndSplitter.GetPane(1,0);
	pAScanView->AssertValid();
	CUltrasonicImmersedTestDoc* pDoc = pAScanView->GetDocument();
	static int nCount = 0;
	
	if (nIDEvent == TIMER_THREAD_PROCESS)
	{
		// 启动的准备工作
		if(m_bPlateStart[0] && nCount == 0)
		{
			KillTimer(TIMER_THREAD_PROCESS);	// Song Chenguang add on 2013-06-06 防止多次弹框

			int nAECCardNum = pApp->GetCardNum();

			// 初始化通道
			const int ALL_PARAM_SIZE = sizeof(CSCAN_ARS200FPGA_SETONE);
			for (int i=0; i< nAECCardNum; i++)
			{	
				for (int j=0; j<2; j++)
				{
					char szSendData[NETHEAD_SIZE+ALL_PARAM_SIZE];
					memset( szSendData, 0, sizeof(szSendData));

					PACKET_HEAD_t stPacketHead;
					stPacketHead.dwCommand = COMMAND_ALL_PARAM_SET;
					stPacketHead.wChannelIndex = j;
					stPacketHead.wGateIndex = 0;
					stPacketHead.dwSize = ALL_PARAM_SIZE;

					int nChannelIndex = i * 2 + j;
					memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));
					memcpy( &szSendData[NETHEAD_SIZE], &pDoc->m_stAllCH_CONFIG[nChannelIndex], ALL_PARAM_SIZE);
				
					// Song Chenguang modified start on 2013-06-06
					// 防止多次弹框
					BOOL bRet = SendData( szSendData, sizeof(szSendData), nChannelIndex);

					if (!bRet)
					{						
						int nRet = MessageBox(_T("请检查下位机是否启动，程序是否关闭？"), _T("提示"), MB_YESNO | MB_ICONQUESTION);

						if(nRet == IDYES)
							::ExitProcess(0);							
					}
					// Song Chenguang modified end on 2013-06-06

				}
				
			}

			// 版本信息
			for (int i=0; i< nAECCardNum; i++)
			{	
				for (int j=0; j<2; j++)
				{
					char szSendData[NETHEAD_SIZE+ALL_PARAM_SIZE];
					memset( szSendData, 0, sizeof(szSendData));

					PACKET_HEAD_t stPacketHead;
					stPacketHead.dwCommand = COMMAND_VERSION_REQ;
					stPacketHead.wChannelIndex = j;
					stPacketHead.wGateIndex = 0;
					stPacketHead.dwSize = ALL_PARAM_SIZE;

					memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));
					int nChannelIndex = i * 2 + j;
					SendData( szSendData, sizeof(szSendData), nChannelIndex);
					m_CommandSocket[0].OnReceive(0);
				}

			}
	
			nCount = 1;
			SetTimer(TIMER_THREAD_PROCESS, 100, NULL);	// Song Chenguang add on 2013-06-06 防止多次弹框
		}

		int nAECNum = pApp->GetCardNum();

		for (int i=0; i<nAECNum; i++)
		{
			CSingleLock lock(&g_csCard);

			if(lock.Lock(1))
			{
				if (m_bPlateStart[i])
				{
					m_bPlateStart[i] = FALSE;
					pApp->m_stThreadParam[i+1].nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_INIT;

					m_bIsRunning = TRUE;
				} 
				else if(m_bPlateEnd[i])
				{
					m_bPlateEnd[i] = FALSE;
					pApp->m_stThreadParam[i+1].nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_PLATE_STOP;
				}
				else if (m_bSelChannel)
				{
					m_bSelChannel = FALSE;
					int nIndex = g_dwActiveChannel/2;
					int nIndex2 = g_dwActiveChannel%2;
					pApp->m_stThreadParam[nIndex+1].nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_SEL_CHANNEL;
					pApp->m_stThreadParam[nIndex+1].nSelChannel = nIndex2;

					lock.Unlock();
					break;
				}
				else if (m_bSetOne)
				{
					m_bSetOne = FALSE;
					int nIndex = g_dwActiveChannel/2;
					int nIndex2 = g_dwActiveChannel % 2;
					pApp->m_stThreadParam[nIndex+1].nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_SETONE;
					pApp->m_stThreadParam[nIndex+1].nSetOneChannel = nIndex2;

					lock.Unlock();
					break;
				}
				else
				{
				}

				// 启动完毕后切换到默认通道
				if (pApp->m_stThreadParam[i+1].nAEC2MainThreadFlag == AECTHREADREURN_START_OK)
				{
					pApp->m_stThreadParam[i+1].nAEC2MainThreadFlag = AECTHREADREURN_IDLE;

					m_bSelChannel = TRUE;
					m_bSetOne = TRUE;	// add by songchenguang 20110927
				}

				// 结束后的收尾工作
				if(pApp->m_stThreadParam[i+1].nAEC2MainThreadFlag == AECTHREADREURN_STOP_OK && nCount == 1)
				{
					m_bIsRunning = FALSE;

					pApp->m_stThreadParam[i+1].nAEC2MainThreadFlag = AECTHREADREURN_INIT_FALSE;
					nCount = 0;
				}

				lock.Unlock(); 
			}	
		}
			
	}

	else if(nIDEvent == TIMER_NET_JUDGE)
//	else if(0)
	{

#if !_OUTLINE
		m_nNetMonitorCount++;
		m_nNetFeatureCount++;
		if(m_nNetMonitorCount > 5)
		{
			MessageBox(_T("A扫数据传输中断，请检查网络是否异常"), _T("提示"),MB_OK);
		}

		// Song Chenguang add start on 2013-09-26
		if(m_nNetFeatureCount > 5)
		{
			MessageBox(_T("C扫数据传输中断，请检查网络是否异常"), _T("提示"),MB_OK);
		}
		// Song Chenguang add end on 2013-09-26
#endif
	}

	// wangbingfu add start on 2014-12-31
	else if(nIDEvent == TIMER_TIME_COUNT)
	{
		DWORD dwTimeCount = GetTickCount();
		DWORD dwCount = dwTimeCount - m_dwTimeCount;

		CUltrasonicImmersedTestApp *pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
		pApp->m_IniParams.WriteIniData();
		pApp->CalcCoderScale();
	}
	// wangbingfu add end on 2014-12-31

	else
	{
		;//
	}

	CBCGPFrameWnd::OnTimer(nIDEvent);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnTimer"),"Leave");
}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnClose"),"Enter");


	int nRet = MessageBox(_T("是否关闭程序？"),_T("提示"),MB_YESNO | MB_DEFBUTTON2);

	if (IDYES == nRet)
	{
		if(!m_bPlateEnd[0])
			OnStopAScan();

		StopTransAscanData();
		StopTransFeatureData();
		OnCommandSClose();

		delete[] m_AscanSocket;
		m_AscanSocket = NULL;
		delete[] m_CommandSocket;
		m_CommandSocket = NULL;
		delete[] m_DataSocket;
		m_DataSocket = NULL;
		// yachang.huang  add start on 2012-11-02
		delete[] m_stYiQiVer;
		m_stYiQiVer = NULL;
		// yachang.huang  add end on 2012-11-02
		// Song Chenguang add start on 2013-12-17
		delete[] m_AscanSocket;
		m_AScanDataSocket = NULL;
		// Song Chenguang add end on 2013-12-17

		KillTimer(TIMER_THREAD_PROCESS);
		KillTimer(TIMER_NET_JUDGE);
		//在退出系统时保存扫查架系统参数
		CMovePathView* m_pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);
		m_pView->m_GraphMove.quit();

		if (m_hDevNotify1)
			// Unregister ITOKEN2 device notification
		{
			BOOL bRes = UnregisterDeviceNotification(m_hDevNotify1);
			ASSERT(bRes);
		} 
		if (m_hDevNotify2)
			// Unregister ITOKEN2 HID device notification
		{
			BOOL bRes = UnregisterDeviceNotification(m_hDevNotify2);
			ASSERT(bRes);
		}

		if (m_bPlateStart != NULL)
		{
			delete[] m_bPlateStart; 
			m_bPlateStart = NULL;
		}

		if (m_bPlateEnd != NULL)
		{
			delete[] m_bPlateEnd; 
			m_bPlateEnd = NULL;
		}
		CBCGPFrameWnd::OnClose();

	}
	else
	{
		return;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnClose"),"Leave");
}

BOOL CMainFrame::Login2Database()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::Login2Database"),"Enter");
	try
	{
		CStdioFile oFile;

		TCHAR strCurDir[256];
		GetModuleFileName(GetModuleHandle(NULL), strCurDir, 256);
		CString strPath = strCurDir;
		int pos = strPath.ReverseFind('\\');
		strPath = strPath.Left(pos);
		strPath += "\\database.cfg";
		/*************************************需要********************************************/
		if (!oFile.Open(strPath.GetBuffer(1024), CFile::modeRead)) 
		{
			AfxMessageBox(_T("配置文件不存在，请先配置服务器"));
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::Login2Database"),"Leave");
			return FALSE;
		}
		
		long cfgFileLen = oFile.GetLength();

		CString strCon;
		oFile.ReadString(strCon);
		oFile.Close();
		return TRUE;
	}
	catch(CDBException* e)
	{
		e->ReportError();
		e->Delete();
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::Login2Database"),"Leave");
		return FALSE;
	}
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::Login2Database"),"Leave");

}
void CMainFrame::OnChannelPanel()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnChannelPanel"),"Enter");
	m_wndChannelInfo.ShowControlBar(!m_wndChannelInfo.IsVisible(),FALSE,TRUE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnChannelPanel"),"Leave");
}

void CMainFrame::OnUpdateChannelPanel(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateChannelPanel"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateChannelPanel pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateChannelPanel"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	pCmdUI->SetCheck(m_wndChannelInfo.IsVisible());
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateChannelPanel"),"Leave");
}

void CMainFrame::OnMechanicalControl()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnMechanicalControl"),"Enter");
	m_wndMechanicalControl.ShowControlBar(!m_wndMechanicalControl.IsVisible(),FALSE,TRUE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnMechanicalControl"),"Leave");

}

void CMainFrame::OnUpdateMechanicalControl(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateMechanicalControl"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateMechanicalControl pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateMechanicalControl"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	pCmdUI->SetCheck(m_wndMechanicalControl.IsVisible());
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateMechanicalControl"),"Leave");
}
// WangQianfeng add start on 2013-06-28
void CMainFrame::OnPictrueParam()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnPictrueParam"),"Enter");
	m_wndCScanParamDockBar.ShowControlBar(!m_wndCScanParamDockBar.IsVisible(),FALSE,TRUE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnPictrueParam"),"Leave");

}

void CMainFrame::OnUpdatePictrueParam(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdatePictrueParam"),"Enter");
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateMechanicalControl pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateMechanicalControl"),"Leave");
		return;
	}
	pCmdUI->SetCheck(m_wndCScanParamDockBar.IsVisible());
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdatePictrueParam"),"Leave");
}
// WangQianfeng add end on 2013-06-28
// 需做处理，避免当前视图非CMovePathView
void CMainFrame::HandStart(int type)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::HandStart"),"Enter");
	CMechanicalControlPanel * pView = m_wndMechanicalControl.m_pMechanicalControlView;
	
	if (pView == NULL)
	{
		MessageBox(_T("error"));
	}

	pView->HandStart(type);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::HandStart"),"Leave");
}

void CMainFrame::HandStop()
{
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::HandStop"),"Enter");
	CMechanicalControlPanel * pView = m_wndMechanicalControl.m_pMechanicalControlView;

	pView->HandStop();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::HandStop"),"Leave");
}

void CMainFrame::OnViewToolbar()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnViewToolbar"),"Enter");
	ShowControlBar(&m_wndToolBar,!m_wndToolBar.IsWindowVisible(),FALSE,TRUE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnViewToolbar"),"Leave");
}

void CMainFrame::OnUpdateViewToolbar(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateViewToolbar"),"Leave");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateViewToolbar pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateViewToolbar"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	pCmdUI->SetCheck(m_wndToolBar.IsWindowVisible());
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateViewToolbar"),"Leave");
}

void CMainFrame::OnViewStandardBar()
{
	// TODO: Add your command handler code here
//	ShowControlBar(&m_ToolBar,!m_ToolBar.IsWindowVisible(),FALSE,TRUE);
}

void CMainFrame::OnUpdateViewStandardBar(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->SetCheck(m_ToolBar.IsWindowVisible());
}

void CMainFrame::OnViewDrawBar()
{
	// TODO: Add your command handler code here
//	ShowControlBar(&m_tuToolBar,!m_tuToolBar.IsWindowVisible(),FALSE,TRUE);
}

void CMainFrame::OnUpdateViewDrawBar(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->SetCheck(m_tuToolBar.IsWindowVisible());
}

void CMainFrame::OnViewBar()
{
	// TODO: Add your command handler code here
//	ShowControlBar(&m_viewToolBar,!m_viewToolBar.IsWindowVisible(),FALSE,TRUE);
}

void CMainFrame::OnUpdateViewBar(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
//	pCmdUI->SetCheck(m_viewToolBar.IsWindowVisible());
}

void CMainFrame::OnUpdateStartAScan(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateStartAScan"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateStartAScan pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateStartAScan"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	pCmdUI->Enable(!m_bIsRunning);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateStartAScan"),"Leave");
}

void CMainFrame::OnUpdateStopAScan(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateStopAScan"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateStopAScan pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateStopAScan"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	pCmdUI->Enable(m_bIsRunning);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateStopAScan"),"Leave");
}

void CMainFrame::OnCscanParam()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCscanParam"),"Enter");
	m_wndCScanParamDockBar.ShowControlBar(!m_wndCScanParamDockBar.IsVisible(),FALSE,TRUE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCscanParam"),"Leave");
}

void CMainFrame::OnUpdateCscanParam(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateCscanParam"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateCscanParam pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateCscanParam"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	pCmdUI->SetCheck(m_wndCScanParamDockBar.IsVisible());
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateCscanParam"),"Leave");
}

void CMainFrame::OnUpdateFeatureInfo(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateFeatureInfo"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateFeatureInfo pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateFeatureInfo"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	if(!m_bFeatureWndDisplayed)
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}
	else
	{
		pCmdUI->SetCheck(BST_CHECKED);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateFeatureInfo"),"Leave");
}

void CMainFrame::OnFeatureInfo()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnFeatureInfo"),"Enter");
	if(m_pFeatureWnd->GetSafeHwnd() == NULL)
	{
		//Machao modified start on 2013-07-1
//		BOOL bRet = m_pFeatureWnd->Create(NULL, _T("FeatureInfo"), WS_CHILD, CRect(0,0,300,300),
//			this, ID_FEATURE_WND);
		BOOL bRet = m_pFeatureWnd->Create(NULL, _T("FeatureInfo"), WS_SYSMENU | WS_CHILD, CRect(0,0,300,300),
			this, ID_FEATURE_WND);
		//Machao modified end on 2013-07-1
	}

	if(!m_bFeatureWndDisplayed)
	{
		m_pFeatureWnd->ShowWindow(TRUE);
	}
	else
	{
		m_pFeatureWnd->ShowWindow(FALSE);
	}

	m_bFeatureWndDisplayed = !m_bFeatureWndDisplayed;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnFeatureInfo"),"Leave");
}
void CMainFrame::OnDacUserDefine()
{
	// TODO: Add your command handler code here
}

void CMainFrame::OnDacStandard()
{
	// TODO: Add your command handler code here
}

void CMainFrame::OnUpdateDacStandard(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CMainFrame::OnUpdateDacUserDefine(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

CChannelInfo CMainFrame::GetChannelInfo(const WORD& wChannelNo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::GetChannelInfo"),"Enter");
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::GetChannelInfo"),"Leave");
	return m_wndChannelInfo.GetChannelInfo(wChannelNo);
}

LRESULT CMainFrame::OnCloseFeatureInfoWnd(WPARAM wParam, LPARAM lParam)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCloseFeatureInfoWnd"),"Enter");
	m_bFeatureWndDisplayed = FALSE;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCloseFeatureInfoWnd"),"Leave");
	return 0;
}
void CMainFrame::OnSaveDataDefine()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnSaveDataDefine"),"Enter");
	CSaveDefineDlg Dlg;
	Dlg.m_bSaveAScan = m_bSaveAScan;

	if (Dlg.DoModal() == IDOK)
	{
		m_bSaveAScan = Dlg.m_bSaveAScan;

		// Song Chenguang add start on 2013-12-19
		// wangbing delete start on 2014-10-16	// 下位机不支持断开再连
	/*	if (m_bSaveAScan)
			OnAScanDataStart();
		else
			OnAScanDataStop();*/
		// wangbing delete start on 2014-10-16
		// Song Chenguang add end on 2013-12-19
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnSaveDataDefine"),"Leave");
}

void CMainFrame::OnUpdateSaveAScan(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateSaveAScan"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateSaveAScan pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateSaveAScan"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	if (m_bSaveAScan)
	{
		pCmdUI->Enable(TRUE);
	} 
	else
	{
		pCmdUI->Enable(FALSE);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateSaveAScan"),"Leave");
}

void CMainFrame::OnCscanRulerMeasure()
{
	// TODO: 在此添加命令处理程序代码
// 	CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2, 0);
// 	if(pView->m_bMeasureOn)
// 		ShowControlBar(&m_wndCScanDlgBar,!m_wndCScanDlgBar.IsWindowVisible(),FALSE,TRUE);
}

void CMainFrame::OnUpdateCscanRulerMeasure(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
//pCmdUI->SetCheck(m_wndCScanDlgBar.IsWindowVisible());
}

void CMainFrame::OnDacCurve()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDacCurve"),"Enter");
	CUltrasonicImmersedTestView* pAScanView = (CUltrasonicImmersedTestView*)m_wndSplitter.GetPane(1,0);
	if(!m_bDacWndDisplayed)
	{
		m_pDacCurveDlg->ShowWindow(SW_SHOW);
		pAScanView->m_bMakeDacFlag = TRUE;
		pAScanView->m_nSelectPoint = 0;
	}
	else
	{
		m_pDacCurveDlg->ShowWindow(SW_HIDE);
		pAScanView->m_bMakeDacFlag = FALSE;
	}

	m_bDacWndDisplayed = !m_bDacWndDisplayed;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDacCurve"),"Leave");
}

void CMainFrame::OnUpdateDacCurve(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateDacCurve"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateDacCurve pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateDacCurve"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	if(!m_bDacWndDisplayed)
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}
	else
	{
		pCmdUI->SetCheck(BST_CHECKED);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateDacCurve"),"Leave");
}
// WangQianfeng delete start on 2013-06-28
void CMainFrame::OnBScanView()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnBScanView"),"Enter");
	// WangQianfeng add start on 2012-06-27
	if (1 == GetDisplayedViewNum() && TRUE == m_bBScanDisplay)
	{
		return;
	}
	// WangQianfeng add end on 2012-06-27
	m_bBScanDisplay = !m_bBScanDisplay;

	ResetView();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnBScanView"),"Leave");
}

void CMainFrame::OnUpdateBScanView(CCmdUI *pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateBScanView"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateBScanView pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
       LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateBScanView"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	if (m_bBScanDisplay)
	{
		pCmdUI->SetCheck(BST_CHECKED);
	} 
	else
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateBScanView"),"Leave");
}
// WangQianfeng delete end on 2013-06-28
void CMainFrame::OnDrawArc()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawArc"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DRAW_ARC, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawArc"),"Leave");
}

void CMainFrame::OnDrawDao()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawDao"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DRAW_DAO, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawDao"),"Leave");
}

void CMainFrame::OnDrawSelect()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawSelect"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DRAW_SELECT, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawSelect"),"Leave");
}

void CMainFrame::OnDrawHole()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawHole"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DRAW_HOLE, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawHole"),"Leave");
}

void CMainFrame::OnDrawLine()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawLine"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DRAW_LINE, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawLine"),"Leave");
}

void CMainFrame::OnDrawArcCenetr()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawArcCenetr"),"Enter");
	
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DRAW_ARC_CENETR, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawArcCenetr"),"Leave");
}

void CMainFrame::OnDrawCircle()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawCircle"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DRAW_CIRCLE, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawCircle"),"Leave");
}

void CMainFrame::OnDrawCircleSan()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawCircleSan"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DRAW_CIRCLE_SAN, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawCircleSan"),"Leave");
}

void CMainFrame::OnDrawArcline()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawArcline"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DRAW_ARCLINE, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawArcline"),"Leave");
}

void CMainFrame::OnDrawRect()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawRect"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DRAW_RECT, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawRect"),"Leave");
}

void CMainFrame::OnDrawEllipse()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawEllipse"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DRAW_ELLIPSE, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawEllipse"),"Leave");
}

void CMainFrame::OnDrawPolygon()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawPolygon"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DRAW_POLYGON, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawPolygon"),"Leave");
}

void CMainFrame::OnDrawRunway()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawRunway"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DRAW_RUNWAY, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDrawRunway"),"Leave");

}

void CMainFrame::OnDecompose()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDecompose"),"Enter");
	
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_DECOMPOSE, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDecompose"),"Leave");
}

void CMainFrame::OnEditUndo()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditUndo"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_EDIT_UNDO, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditUndo"),"Leave");
}

void CMainFrame::OnEditRedo()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditUndo"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_EDIT_REDO, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditUndo"),"Leave");
}

void CMainFrame::OnEditSelectAll()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditSelectAll"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_EDIT_SELECT_ALL, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditSelectAll"),"Leave");
}

void CMainFrame::OnEditFind()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditFind"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_EDIT_FIND, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditFind"),"Leave");
}

void CMainFrame::OnEditShai()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditShai"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_EDIT_SHAI, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditShai"),"Leave");
}

void CMainFrame::OnEditCut()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditCut"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_EDIT_CUT, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditCut"),"Leave");
}

void CMainFrame::OnEditCopy()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditCopy"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_EDIT_COPY, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditCopy"),"Leave");
}

void CMainFrame::OnEditPaste()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditPaste"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_EDIT_PASTE, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditPaste"),"Leave");
}

void CMainFrame::OnEditClear()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditClear"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_EDIT_CLEAR, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditClear"),"Leave");
}

void CMainFrame::OnEditFan()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditFan"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_EDIT_FAN, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditFan"),"Leave");
}

void CMainFrame::OnEditMirror()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditMirror"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_EDIT_MIRROR, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditMirror"),"Leave");
}

void CMainFrame::OnEditRota()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditRota"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_EDIT_ROTA, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEditRota"),"Leave");

}

void CMainFrame::OnToolCan()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnToolCan"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_TOOL_CAN, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnToolCan"),"Leave");
}

void CMainFrame::OnToolColor()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnToolColor"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_TOOL_COLOR, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnToolColor"),"Leave");
}

void CMainFrame::OnToolJxian()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnToolJxian"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_TOOL_JXIAN, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnToolJxian"),"Leave");
}

void CMainFrame::OnToolJpian()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnToolJpian"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_TOOL_JPIAN, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnToolJpian"),"Leave");
}

void CMainFrame::OnToolEdit()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnToolEdit"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_TOOL_EDIT, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnToolEdit"),"Leave");
}
/*
void CMainFrame::OnToolReset()
{
	// TODO: 在此添加命令处理程序代码
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_TOOL_RESET, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
}
*/
void CMainFrame::OnToolRun()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnToolRun"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_TOOL_RUN, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnToolRun"),"Leave");
}

void CMainFrame::OnIpConfig()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnIpConfig"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_IP_CONFIG, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnIpConfig"),"Leave");
}

void CMainFrame::OnViewShi()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnViewShi"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_VIEW_SHI, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnViewShi"),"Leave");
}

void CMainFrame::OnViewTu()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnViewTu"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_VIEW_TU, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnViewTu"),"Leave");
}

void CMainFrame::OnViewWindow()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnViewWindow"),"Enter");
	CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);

	WPARAM wParam = MAKEWPARAM(ID_VIEW_WINDOW, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnViewWindow"),"Leave");
}

void CMainFrame::OnRotateImage()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnRotateImage"),"Enter");
	CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2,0);

	WPARAM wParam = MAKEWPARAM(ID_ROTATE_IMAGE, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnRotateImage"),"Leave");
}

void CMainFrame::OnCscanSmooth()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCscanSmooth"),"Enter");
	CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2,0);

	WPARAM wParam = MAKEWPARAM(ID_CSCAN_SMOOTH, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCscanSmooth"),"Leave");
}

void CMainFrame::OnCscanMf()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCscanMf"),"Enter");
	CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2,0);

	WPARAM wParam = MAKEWPARAM(ID_CSCAN_MF, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCscanMf"),"Leave");
}

void CMainFrame::OnSetUnit()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnSetUnit"),"Enter");
	CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2,0);

	WPARAM wParam = MAKEWPARAM(ID_SET_UNIT, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnSetUnit"),"Leave");
}

void CMainFrame::OnRulerMeasure()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnRulerMeasure"),"Enter");
	CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2,0);

	WPARAM wParam = MAKEWPARAM(ID_RULER_MEASURE, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnRulerMeasure"),"Leave");
}

void CMainFrame::OnTriangleMeasure()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnTriangleMeasure"),"Enter");
	CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2,0);

	WPARAM wParam = MAKEWPARAM(ID_TRIANGLE_MEASURE, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnTriangleMeasure"),"Leave");
}

void CMainFrame::OnEllipseMeasure()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEllipseMeasure"),"Enter");
	CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2,0);
	
	WPARAM wParam = MAKEWPARAM(ID_ELLIPSE_MEASURE, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnEllipseMeasure"),"Leave");
}

//yachang.huang add start on 2012-5-31
void CMainFrame::OnPolygonMeasure()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnPolygonMeasure"),"Enter");
	CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2,0);

	WPARAM wParam = MAKEWPARAM(ID_POLYGON_MEASURE, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnPolygonMeasure"),"Leave");
}
//yachang.huang add end on 2012-5-31

void CMainFrame::OnMeasureOnOff()
{
	// TODO: 在此添加命令处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnMeasureOnOff"),"Enter");
	CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2,0);

//	ShowControlBar(&m_wndCScanDlgBar,!m_wndCScanDlgBar.IsWindowVisible(),FALSE,TRUE);
	WPARAM wParam = MAKEWPARAM(ID_MEASURE_ON_OFF, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnMeasureOnOff"),"Leave");
}

void CMainFrame::OnButtonReposition()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnButtonReposition"),"Enter");

	WPARAM wParam = MAKEWPARAM(IDC_BUTTON_REPOSITION, 0);
	m_wndMechanicalControl.m_pMechanicalControlView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnButtonReposition"),"Leave");

}

void CMainFrame::OnButtonRun()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnButtonRun"),"Enter");
	WPARAM wParam = MAKEWPARAM(IDC_BUTTON_RUN, 0);
	m_wndMechanicalControl.m_pMechanicalControlView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnButtonRun"),"Leave");
}
void CMainFrame::OnButtonSysparam()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnButtonSysparam"),"Enter");
	WPARAM wParam = MAKEWPARAM(IDC_BUTTON_SYSPARAM, 0);
	m_wndMechanicalControl.m_pMechanicalControlView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnButtonSysparam"),"Leave");
}



void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default


	CBCGPFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default


	CBCGPFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnProbeSet()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnProbeSet"),"Enter");
	m_ProbeSet.show();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnProbeSet"),"Leave");
/*
	// test
	CPrintView temp;
	CString str = CString(temp.GetVariable());
	//////////////////////////////////////////////////////////////////////////
	CLSID clsid;
	HRESULT hresult = CLSIDFromProgID(OLESTR("CPrintView"),&clsid);
	if(FAILED(hresult))

	{
		//		AfxMessageBox("Creation Failed");
		return;
	}

	_PrintView *t;

	hresult=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(_PrintView),(LPVOID *) &t);

	if(FAILED(hresult))

	{
//		AfxMessageBox("Creation Failed");
		return;
	}


	_bstr_t str2;

	str2 = t->GetVariable();

//	MessageBox(str2,"",MB_OK);

//	t->fnCallDll ();

	t->Release ();

	//////////////////////////////////////////////////////////////////////////
*/
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Enter");
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_LEFT)
		{
			WPARAM wParam = MAKEWPARAM(IDC_BUTTON_X_LEFT, 0);
			m_wndMechanicalControl.m_pMechanicalControlView->m_btnXLeft.SendMessage(WM_LBUTTONDOWN, wParam, 0);
			m_bPressDirectKeys = TRUE;//add by wangqianfeng 2013-06-03
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Leave");
			return TRUE;
		}
		else if (pMsg->wParam == VK_RIGHT)
		{
			WPARAM wParam = MAKEWPARAM(IDC_BUTTON_X_RIGHT, 0);
			m_wndMechanicalControl.m_pMechanicalControlView->m_btnXRight.SendMessage(WM_LBUTTONDOWN, wParam, 0);
			m_bPressDirectKeys = TRUE;//add by wangqianfeng 2013-06-03
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Leave");
			return TRUE;
		}
		else if (pMsg->wParam == VK_DOWN)
		{
			WPARAM wParam = MAKEWPARAM(IDC_BUTTON_Y_BACK, 0);
			m_wndMechanicalControl.m_pMechanicalControlView->m_btnYBack.SendMessage(WM_LBUTTONDOWN, wParam, 0);
			m_bPressDirectKeys = TRUE;//add by wangqianfeng 2013-06-03
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Leave");
			return TRUE;
		}
		else if (pMsg->wParam == VK_UP)
		{
			WPARAM wParam = MAKEWPARAM(IDC_BUTTON_Y_FRONT, 0);
			m_wndMechanicalControl.m_pMechanicalControlView->m_btnYFront.SendMessage(WM_LBUTTONDOWN, wParam, 0);
			m_bPressDirectKeys = TRUE;//add by wangqianfeng 2013-06-03
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Leave");
			return TRUE;
		}
		else if (pMsg->wParam == VK_PRIOR)
		{
			WPARAM wParam = MAKEWPARAM(IDC_BUTTON_UP_Z, 0);
			m_wndMechanicalControl.m_pMechanicalControlView->m_btnZUp.SendMessage(WM_LBUTTONDOWN, wParam, 0);
			m_bPressDirectKeys = TRUE;//add by wangqianfeng 2013-06-03
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Leave");
			return TRUE;
		}
		else if (pMsg->wParam == VK_NEXT)
		{
			WPARAM wParam = MAKEWPARAM(IDC_BUTTON_Z_DOWN, 0);
			m_wndMechanicalControl.m_pMechanicalControlView->m_btnZDown.SendMessage(WM_LBUTTONDOWN, wParam, 0);
			m_bPressDirectKeys = TRUE;//add by wangqianfeng 2013-06-03
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Leave");
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);
			if ( IDYES == MessageBox(_T("是否中止运行？"),_T("暂停提示"),MB_YESNO|MB_ICONQUESTION| MB_DEFBUTTON2))
			{
				pView->m_bESC = TRUE;
				pView->m_GraphMove.CancelCmd();

				if(m_wndMechanicalControl.m_pMechanicalControlView->m_bTaskRun)
				{
					m_wndMechanicalControl.m_pMechanicalControlView->m_bTaskRun = FALSE;
					m_wndMechanicalControl.m_pMechanicalControlView->m_strCurrentMovePath = _T("");

					m_wndTaskDockBar.m_pFormView->GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(TRUE);
					m_wndTaskDockBar.m_pFormView->GetDlgItem(IDC_STATIC_RUN_STATE)->SetWindowText(_T("运行状态：运行终止"));
				}
			}
		}
	}
	if (pMsg->message == WM_KEYUP)
	{
		if (pMsg->wParam == VK_LEFT)
		{
			WPARAM wParam = MAKEWPARAM(IDC_BUTTON_X_LEFT, 0);
			m_wndMechanicalControl.m_pMechanicalControlView->m_btnXLeft.SendMessage(WM_LBUTTONUP, wParam, 0);
			m_bPressDirectKeys = FALSE;//add by wangqianfeng 2013-06-03
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Leave");
			return TRUE;
		}
		else if (pMsg->wParam == VK_RIGHT)
		{
			WPARAM wParam = MAKEWPARAM(IDC_BUTTON_X_RIGHT, 0);
			m_wndMechanicalControl.m_pMechanicalControlView->m_btnXRight.SendMessage(WM_LBUTTONUP, wParam, 0);
			m_bPressDirectKeys = FALSE;//add by wangqianfeng 2013-06-03
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Leave");
			return TRUE;
		}
		else if (pMsg->wParam == VK_DOWN)
		{
			WPARAM wParam = MAKEWPARAM(IDC_BUTTON_Y_BACK, 0);
			m_wndMechanicalControl.m_pMechanicalControlView->m_btnYBack.SendMessage(WM_LBUTTONUP, wParam, 0);
			m_bPressDirectKeys = FALSE;//add by wangqianfeng 2013-06-03
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Leave");
			return TRUE;
		}
		else if (pMsg->wParam == VK_UP)
		{
			WPARAM wParam = MAKEWPARAM(IDC_BUTTON_Y_FRONT, 0);
			m_wndMechanicalControl.m_pMechanicalControlView->m_btnYFront.SendMessage(WM_LBUTTONUP, wParam, 0);
			m_bPressDirectKeys = FALSE;//add by wangqianfeng 2013-06-03
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Leave");
			return TRUE;
		}
		else if (pMsg->wParam == VK_PRIOR)
		{
			WPARAM wParam = MAKEWPARAM(IDC_BUTTON_UP_Z, 0);
			m_wndMechanicalControl.m_pMechanicalControlView->m_btnZUp.SendMessage(WM_LBUTTONUP, wParam, 0);
			m_bPressDirectKeys = FALSE;//add by wangqianfeng 2013-06-03
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Leave");
			return TRUE;
		}
		else if (pMsg->wParam == VK_NEXT)
		{
			WPARAM wParam = MAKEWPARAM(IDC_BUTTON_Z_DOWN, 0);
			m_wndMechanicalControl.m_pMechanicalControlView->m_btnZDown.SendMessage(WM_LBUTTONUP, wParam, 0);
			m_bPressDirectKeys = FALSE;//add by wangqianfeng 2013-06-03
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Leave");
			return TRUE;
		}
	}
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::PreTranslateMessage"),"Leave");
	return CBCGPFrameWnd::PreTranslateMessage(pMsg);
}
// WangQianfeng add start on 2013-06-03
void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized )
{
 	if (nState == WA_INACTIVE)//窗体失去焦点
 	{

 		if (m_bPressDirectKeys)//点击过方向键
 		{
			CMovePathView* pView = (CMovePathView*)m_wndSplitter.GetPane(0,0);
			pView->m_GraphMove.AxisStop();//发送停止命令
			m_bPressDirectKeys = false;
			AfxMessageBox(_T("已发送Trio停止命令！"));
 		}
 	}
 	CBCGPFrameWnd::OnActivate(nState, pWndOther, bMinimized);  
}
// WangQianfeng add end on 2013-06-03

void CMainFrame::OnDataDirectorySet()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDataDirectorySet"),"Enter");
	TCHAR szPath[MAX_PATH];     //存放选择的目录路径 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = _T("请选择数据存放目录："); 
	bi.ulFlags = 0;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))
	{
		str.Format(_T("选择的目录为 %s"),  szPath);
		AfxMessageBox(str); 
		m_strDataFolder = CString(szPath) + _T("\\"); //? 是否需要加此符号
		SetCurrentDirectory(m_strDataFolder);

		// 写入注册表
		BOOL bRet = AfxGetApp()->WriteProfileString(_T("水浸超声探伤系统"), _T("Data Folder"), m_strDataFolder);	// Song Chenguang add on 2013-04-10
	}
	//Machao modify start on 2013-06-26
//	else 
//		AfxMessageBox(_T("无效的目录，请重新选择")); 
	else if (lp && !SHGetPathFromIDList(lp, szPath))
	{
		AfxMessageBox(_T("无效的目录，请重新选择"));
	}
	//Machao modify end on 2013-06-27
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDataDirectorySet"),"Leave");

}

void CMainFrame::OnUpdateMeasureOnOff(CCmdUI *pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateMeasureOnOff"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateMeasureOnOff pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateMeasureOnOff"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	// TODO: Add your command update UI handler code here
	CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2,0);

	//	ShowControlBar(&m_wndCScanDlgBar,!m_wndCScanDlgBar.IsWindowVisible(),FALSE,TRUE);
	if (m_wndCScanDlgBar.IsWindowVisible())
	{
		pCmdUI->SetCheck(BST_CHECKED);
	}
	else
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateMeasureOnOff"),"Leave");
}

void CMainFrame::OnCScanView2()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCScanView2"),"Enter");
	// TODO: Add your command handler code here
	// WangQianfeng add start on 2013-06-27
	if (1 == GetDisplayedViewNum() && TRUE == m_bCScanDisplay2)
	{
		return;
	}
	// WangQianfeng add end on 2012-06-27
	m_bCScanDisplay2 = !m_bCScanDisplay2;

	ResetView();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCScanView2"),"Leave");
}

void CMainFrame::OnUpdateCScanView2(CCmdUI *pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateCScanView2"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateCScanView2 pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateCScanView2"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	// TODO: Add your command update UI handler code here
	if (m_bCScanDisplay2)
	{
		pCmdUI->SetCheck(BST_CHECKED);
	} 
	else
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateCScanView2"),"Leave");
}

void CMainFrame::TransAScanData(void)
{
	//	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::TransAScanData"),"Enter");
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();
	for (int i = 0; i< nCardNum; i++)
	{
//		m_AscanSocket[i].Close();
		m_AscanSocket[i].m_CmdLine = CMD_ASCAN;
		bool bRet = m_AscanSocket[i].Create();
		CString strIP = pApp->GetStrIP(i);
		m_AscanSocket[i].Connect( strIP, m_nPort);
	}

	m_bMoinitorDataRun = TRUE;	// Song Chenguang add on 2013-04-03
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::TransAScanData"),"Leave");
}

void CMainFrame::StopTransAscanData(void)
{
	//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    
	// TODO: 在此添加调度处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::StopTransAscanData"),"Enter");
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();
	for (int i = 0; i< nCardNum; i++)
	{
		m_AscanSocket[i].Close();
	}

	m_bMoinitorDataRun = FALSE;	// Song Chenguang add on 2013-04-03
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::StopTransAscanData"),"Leave");
}

void CMainFrame::TransFeatureData()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::TransFeatureData"),"Enter");
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();
	for (int i = 0; i< nCardNum; i++)
	{
//		m_DataSocket[i].Close();
		m_DataSocket[i].m_CmdLine = CMD_ASCAN;
		m_DataSocket[i].m_hSocket = INVALID_SOCKET;
		m_DataSocket[i].Create();
		CString strIP = pApp->GetStrIP(i);
		m_DataSocket[i].Connect(strIP, 4444);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::TransFeatureData"),"Leave");
}

void CMainFrame::StopTransFeatureData()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::StopTransFeatureData"),"Enter");
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();
	for (int i = 0; i< nCardNum; i++)
	{
		m_DataSocket[i].Close();
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::StopTransFeatureData"),"Leave");
}

/*
BOOL CMainFrame::SendData( const char* data, const int& len, const int& nChannelIndex)
{
//#if !_OUTLINE
#if 0
	// TODO: 在此添加调度处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Enter");
	int nCardIndex = nChannelIndex / 2;
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();

	CString strIP = pApp->GetStrIP(nCardIndex);

	char cIP[MAX_PATH]; 
	::ZeroMemory(cIP, sizeof(cIP));
	BOOL bRet = FALSE;
	int nRet = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, strIP,
		strIP.GetLength(), cIP, MAX_PATH, "a", &bRet);

	if (nRet == 0)
	{
		AfxMessageBox(_T("WideCharToMultiByte转换失败！发送失败！"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Leave");

		ErroWindowMessage();

		return FALSE;
	}

	cIP[nRet] = '\0';

	if(!m_CommandSocket[nCardIndex].Connection(cIP, m_nPort)) 
	{
		m_CommandSocket[nCardIndex].Close();
		CString str;
		str.Format(_T("未连接到下位机设备！序号：%d"), nCardIndex);
		MessageBox(str, _T("提示"),MB_OK);
//		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Leave");

		ErroWindowMessage();

		return FALSE;
	}

	if(m_CommandSocket[nCardIndex].SendData( data, len, 10)==SOCKET_ERROR) 
	{
		m_CommandSocket[nCardIndex].Close();
		::MessageBox(NULL, _T("发送数据失败！"), _T("提示"),MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Leave");
		return FALSE;
	}

	char tempBuf[MAX_BUF_LEN];
	memset( tempBuf, 0, MAX_BUF_LEN);
	if(m_CommandSocket[nCardIndex].ReadData( tempBuf,MAX_BUF_LEN, 100)==SOCKET_ERROR) 
	{
		m_CommandSocket[nCardIndex].Close();
		::MessageBox(NULL,_T("接收返回结果失败！"), _T("提示"),MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Leave");
		return FALSE;
	}

	m_CommandSocket[nCardIndex].Close();

	//取包头，看是否返回异常
	PACKET_HEAD_t stPacketHead;
	memset( &stPacketHead, 0, sizeof( stPacketHead));
	memcpy( &stPacketHead, tempBuf, NETHEAD_SIZE);
	DWORD cmdLine = stPacketHead.dwCommand;

	if( cmdLine == COMMAND_VERSION_REQ) 
	{
		memcpy(&m_stYiQiVer[nCardIndex], tempBuf+sizeof(PACKET_HEAD_t), sizeof(CSCAN_VERSION_DATA_t));
	}
	else
	{
		if( cmdLine == CMD_SET_PARAM_RET) 
		{
			AfxMessageBox( _T("数据超出范围，设置失败。"));
			m_ReSendNum = 0;
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Leave");
			return FALSE;
		}
		if(cmdLine != COMMAND_TCG_SWICH) // TCG曲线命令不比较,4K多的数据
		{
			//比较接收和发送的字符串是否相等
			if( memcmp( data, tempBuf, len)!=0) 
			{
				if(  m_ReSendNum < m_ReSendMaxNum) 
				{
					m_ReSendNum += 1;
					this->SendData( data, len, nChannelIndex);
					//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Leave");
					return FALSE;
				} 
				else 
				{
					//如果发送最大次还不成功，则报错误
					AfxMessageBox( _T("设置失败，网络可能异常。"));
				}
			}

			m_ReSendNum = 0;
		}

	}	
#endif
	return TRUE;
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Leave");
}
*/
BOOL CMainFrame::SendData( const char* data, const int& len, const int& nChannelIndex)
{

	//#if !_OUTLINE
	// TODO: 在此添加调度处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Enter");
	int nCardIndex = nChannelIndex / 2;
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();

	CString strIP = pApp->GetStrIP(nCardIndex);

	char cIP[MAX_PATH]; 
	::ZeroMemory(cIP, sizeof(cIP));
	BOOL bRet = FALSE;
	int nRet = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, strIP,
		strIP.GetLength(), cIP, MAX_PATH, "a", &bRet);

	if (nRet == 0)
	{
		AfxMessageBox(_T("WideCharToMultiByte转换失败！发送失败！"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Leave");

		ErroWindowMessage();

		return FALSE;
	}

	cIP[nRet] = '\0';


	if(m_CommandSocket[nCardIndex].Send(data,len,0)==SOCKET_ERROR) 
	{
		DWORD dwError = GetLastError();

		m_CommandSocket[nCardIndex].Close();
		MsgDlg *msgbox = new MsgDlg;
		msgbox->Create(IDD_MSGDLG, this);;
		msgbox->ShowWindow(SW_SHOW); 
		Sleep(1000);
		msgbox->DestroyWindow();
		delete msgbox;

		//MessageBox(str, _T("提示"),MB_OK);
		//::MessageBox(NULL,str,_T("提示"),MB_OK);
		PostMessage( WM_CANCELMODE, 0, 0);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Leave");
		return FALSE;
	}





	return TRUE;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Leave");
}

BOOL CMainFrame::OnDeviceChange(UINT nEventType,DWORD_PTR dwData)
{
//#if !_OUTLINE
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDeviceChange"),"Enter");
	DEV_BROADCAST_DEVICEINTERFACE *pDbt = (DEV_BROADCAST_DEVICEINTERFACE*)dwData;
	const TCHAR *hidDevPath = _T("\\\\?\\HID#Vid_1bc0&Pid_8013");
	
	switch(nEventType)
	{
		case DBT_DEVICEARRIVAL:
		/* Check GUID to see whether it's a EliteE device.
		Notice:the GUIDs of HID devices are of the same,so
		we have to make further check on PID and VID */
		if (!memcmp(&pDbt->dbcc_classguid, &GUID_CLASS_ITOKEN2, sizeof(GUID)) ||
			(!memcmp(&pDbt->dbcc_classguid,&GUID_CLASS_ITOKEN2_HID, sizeof(GUID)) &&
			!memicmp(&pDbt->dbcc_name, hidDevPath, wcslen(hidDevPath))))
		{
			// TODO: Add your handler code here
			MessageBox(_T("加密狗已插入"), NULL, MB_OK);
		} 
		break;
		case DBT_DEVICEREMOVECOMPLETE :
			if (!memcmp(&pDbt->dbcc_classguid, &GUID_CLASS_ITOKEN2, sizeof(GUID)) ||
				(!memcmp(&pDbt->dbcc_classguid, &GUID_CLASS_ITOKEN2_HID, sizeof(GUID)) &&
				!memicmp(&pDbt->dbcc_name, hidDevPath, wcslen(hidDevPath))))
			{
				// TODO: Add your handler code here
				MessageBox(_T("加密狗已拔出，程序将退出！"), NULL, MB_OK);

				CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
				::BCGCBProCleanUp ();

				if (hSaveDMADataEvent != NULL)
				{
					delete[] hSaveDMADataEvent;
					hSaveDMADataEvent = NULL;
				}

				if (pApp->m_pAECWorkThread != NULL)
				{
					delete[] pApp->m_pAECWorkThread;
					pApp->m_pAECWorkThread = NULL;
				}

				if (pApp->m_strIPArray != NULL)
				{
					delete []pApp->m_strIPArray;
					pApp->m_strIPArray = NULL;
				}

				// 关闭加密狗
				ElepClose(pApp->m_ElepHandle);

				ExitProcess(0);
			} 
			break;
		default:
			break;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnDeviceChange"),"Leave");
//#endif
	return TRUE;
}

void CMainFrame::OnFilePrintPreview()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnFilePrintPreview"),"Enter");
	CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2,0);

	WPARAM wParam = MAKEWPARAM(ID_FILE_PRINT_PREVIEW, 0);
	pView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnFilePrintPreview"),"Leave");
}

void CMainFrame::OnTcgCurve()
{
	// TODO: Add your command handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnTcgCurve"),"Enter");
	CUltrasonicImmersedTestView* pAScanView = (CUltrasonicImmersedTestView*)m_wndSplitter.GetPane(1,0);
	if(!m_bTcgWndDisplayed)
	{
		m_pTcgCurveDlg->ShowWindow(SW_SHOW);
		m_pTcgCurveDlg->m_bMakeTcgFlag = TRUE;
		m_pTcgCurveDlg->m_nSelectPoint = 0;
	}
	else
	{
		m_pTcgCurveDlg->ShowWindow(SW_HIDE);
		m_pTcgCurveDlg->m_bMakeTcgFlag = FALSE;
	}

	m_bTcgWndDisplayed = !m_bTcgWndDisplayed;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnTcgCurve"),"Leave");
}

void CMainFrame::OnUpdateTcgCurve(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateTcgCurve"),"Enter");
	if(!m_bTcgWndDisplayed)
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}
	else
	{
		pCmdUI->SetCheck(BST_CHECKED);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateTcgCurve"),"Leave");
}
// WangQianfeng add start on 2012-08-22
/** 
* 创建customize工具栏
* @return    void
* @since     1.0
*/
void CMainFrame::OnViewCustomize()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnViewCustomize"),"Enter");
	CBCGPToolbarCustomize* pDlgCust = new CBCGPToolbarCustomize( this,TRUE );
	// WangQianfeng add start on 2012-11-06
	if (NULL == pDlgCust)
	{
		MessageBox(_T("CMainFrame::OnViewCustomize(),pDlgCust = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnViewCustomize"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-06
	pDlgCust->RemovePage(2);	//删除多余的属性页
	pDlgCust->RemovePage(2);	//删除多余的属性页
	pDlgCust->RemovePage(2);	//删除多余的属性页
	pDlgCust->RemovePage(2);	//删除多余的属性页
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnViewCustomize"),"Leave");

}
/** 
* 显示Channel工具栏
* @return    void
* @since     1.0
*/
void CMainFrame::OnChannelToolBarShow()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnChannelToolBarShow"),"Enter");
	m_wndChannelToolBar.ShowControlBar(!m_wndChannelToolBar.IsVisible(),FALSE,TRUE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnChannelToolBarShow"),"Leave");
}
/** 
* Channel工具栏选择更新
* @return    void
* @since     1.0
*/
void CMainFrame::OnUpdateChannelToolBarShow(CCmdUI *pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateChannelToolBarShow"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateChannelToolBarShow pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateChannelToolBarShow"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	// TODO: Add your command update UI handler code here
	if (m_wndChannelToolBar.IsVisible())
	{
		pCmdUI->SetCheck(BST_CHECKED);
	} 
	else
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateChannelToolBarShow"),"Leave");
}
/** 
* 显示Mechanical工具栏
* @return    void
* @since     1.0
*/
void CMainFrame::OnMechanicalToolBarShow()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnMechanicalToolBarShow"),"Enter");
	m_wndMechanicalToolBar.ShowControlBar(!m_wndMechanicalToolBar.IsVisible(),FALSE,TRUE);
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnMechanicalToolBarShow"),"Leave");
}
/** 
* Mechanical工具栏选择更新
* @return    void
* @since     1.0
*/
void CMainFrame::OnUpdateMechanicalToolBarShow(CCmdUI *pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateMechanicalToolBarShow"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateMechanicalToolBarShow pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateMechanicalToolBarShow"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	if (m_wndMechanicalToolBar.IsVisible())
	{
		pCmdUI->SetCheck(BST_CHECKED);
	} 
	else
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateMechanicalToolBarShow"),"Leave");
}
/** 
* 显示CScanparam工具栏
* @return    void
* @since     1.0
*/
void CMainFrame::OnCScanparamToolBarShow()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCScanparamToolBarShow"),"Enter");
	m_wndCScanParamToolBar.ShowControlBar(!m_wndCScanParamToolBar.IsVisible(),FALSE,TRUE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnCScanparamToolBarShow"),"Leave");

}
/** 
* CScanparam工具栏选择更新
* @return    void
* @since     1.0
*/
void CMainFrame::OnUpdateCScanparamToolBarShow(CCmdUI *pCmdUI)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateCScanparamToolBarShow"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pCmdUI == NULL)
	{
		MessageBox(_T("CMainFrame::OnUpdateCScanparamToolBarShow pCmdUI"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::OnUpdateCScanparamToolBarShow"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	if (m_wndCScanParamToolBar.IsVisible())
	{
		pCmdUI->SetCheck(BST_CHECKED);
	} 
	else
	{
		pCmdUI->SetCheck(BST_UNCHECKED);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnUpdateCScanparamToolBarShow"),"Leave");
}
/** 
* Channel工具栏读取参数
* @return    void
* @since     1.0
*/
void CMainFrame::OnReadParam()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnReadParam"),"Enter");
	WPARAM wParam = MAKEWPARAM(IDC_BUTTON_READ_PARAM, 0);
	m_wndChannelInfo.m_pFormView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnReadParam"),"Leave");
}
/** 
* Channel工具栏保存参数
* @return    void
* @since     1.0
*/
void CMainFrame::OnSaveParam()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnSaveParam"),"Enter");
	WPARAM wParam = MAKEWPARAM(IDC_BUTTON_SAVE_PARAM, 0);
	m_wndChannelInfo.m_pFormView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnSaveParam"),"Leave");
}
/** 
* Mechanical工具栏打开路径文件
* @return    void
* @since     1.0
*/
void CMainFrame::OnOpenPathFile()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnOpenPathFile"),"Enter");
	WPARAM wParam = MAKEWPARAM(IDC_OPEN_PATH_FILE, 0);
	m_wndMechanicalControl.m_pMechanicalControlView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnOpenPathFile"),"Leave");

}
/** 
* Mechanical工具栏保存路径文件
* @return    void
* @since     1.0
*/
void CMainFrame::OnSavePathFile()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnSavePathFile"),"Enter");
	WPARAM wParam = MAKEWPARAM(IDC_SAVE_PATH_FILE, 0);
	m_wndMechanicalControl.m_pMechanicalControlView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnSavePathFile"),"Leave");
}
/** 
* CScanparam工具栏保存探伤数据
* @return    void
* @since     1.0
*/
void CMainFrame::OnSaveData()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnSaveData"),"Enter");
	WPARAM wParam = MAKEWPARAM(IDC_BUTTON_SAVE_DATA, 0);
	m_wndCScanParamDockBar.m_pCScanParamView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnSaveData"),"Leave");
}
/** 
* CScanparam工具栏读文件
* @return    void
* @since     1.0
*/
void CMainFrame::OnReadData()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnReadData"),"Enter");
	WPARAM wParam = MAKEWPARAM(IDC_BUTTON_READ_FILE, 0);
	m_wndCScanParamDockBar.m_pCScanParamView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnReadData"),"Leave");

}
/** 
* CScanparam工具栏读取C扫参数
* @return    void
* @since     1.0
*/
void CMainFrame::OnReadCParam()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnReadCParam"),"Enter");
	WPARAM wParam = MAKEWPARAM(IDC_BUTTON_CSCAN_PARAM_READ, 0);
	m_wndCScanParamDockBar.m_pCScanParamView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnReadCParam"),"Leave");

}
/** 
* CScanparam工具栏保存C扫参数
* @return    void
* @since     1.0
*/
void CMainFrame::OnSaveCParam()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnSaveCParam"),"Enter");
	WPARAM wParam = MAKEWPARAM(IDC_BUTTON_CSCAN_PARAM_SAVE, 0);
	m_wndCScanParamDockBar.m_pCScanParamView->SendMessage(WM_COMMAND, wParam, 0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CMainFrame::OnSaveCParam"),"Leave");
}
// WangQianfeng add end on 2012-08-22

// wangbingfu add start on 2013-01-09
void CMainFrame::ReadPipeIni()//const CString& csFilePath)
{
	TCHAR strFolder[MAX_PATH];
	GetWorkingFolder(strFolder, MAX_PATH);
	CString csFilePath = CString(strFolder) + _T("PipeSet.ini");//取得配置文件路径

	CString strSection;
	CString strKey;
	
	int nValue = 0;
	strSection = _T("OtherParams");
//	strKey = _T("AxisSelect");
//	int nAxisSelect = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);

//	if(nAxisSelect == 4)
	{
//		m_nAxisSelect = nAxisSelect;
		strKey = _T("Angle");
		int nAngle = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		m_fPipeProbeAngle = (float)nAngle;

		int nPipeChannelSelect[CHANNEL_NUM];
		memset(nPipeChannelSelect, 0, sizeof(nPipeChannelSelect));

		// 0=无,1=A测厚,2=A分层,3=A测厚+分层,4=轴向B1,5=轴向B2,6=径向C1,7=径向C2
		strSection = _T("GroupSelect");
		strKey = _T("G11");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[0] = nValue;
		strKey = _T("G12");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[1] = nValue;
		strKey = _T("G21");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[2] = nValue;
		strKey = _T("G22");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[3] = nValue;
		strKey = _T("G31");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[4] = nValue;
		strKey = _T("G32");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[5] = nValue;
		strKey = _T("G41");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[6] = nValue;
		strKey = _T("G42");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		nPipeChannelSelect[7] = nValue;

		for(int i = 0; i < CHANNEL_NUM; i++)
		{
			m_nPipeChannelSelect[i] = nPipeChannelSelect[i];
		}

		// 0=通道1,1=通道2,2=通道3,3=通道4,4=通道5,5=通道6,6=通道7,7=通道8
		strSection = _T("CScanSelect");
		strKey = _T("C1");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		m_nCScanSelect[0] = nValue - 1;
		strKey = _T("C2");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		m_nCScanSelect[1] = nValue - 1;
	}
}

void CMainFrame::WritePipeIni()//const CString& csFilePath)
{
	CString strKey;
	CString strValue;
	CString cstr = _T("");
	CString strSection = _T("");

	TCHAR strFolder[MAX_PATH];
	GetWorkingFolder(strFolder, MAX_PATH);
	CString csFilePath = CString(strFolder) + _T("PipeSet.ini");//取得配置文件路径
	//如果文件为只读的，将只读属性去掉
	SetFileAttributes(csFilePath,GetFileAttributes(csFilePath) & ~FILE_ATTRIBUTE_READONLY);

	strSection = _T("OtherParams");
// 	strKey = _T("AxisSelect");
// 	strValue = _T("");
// 	strValue.Format(_T("%d"),m_nAxisSelect);
// 	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("Angle");
	strValue = _T("");
	strValue.Format(_T("%d"),m_fPipeProbeAngle);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);

	strSection = _T("GroupSelect");
	strKey = _T("G11");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[0]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G12");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[1]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G21");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[2]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G22");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[3]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G31");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[4]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G32");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[5]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G41");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[6]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strKey = _T("G42");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nPipeChannelSelect[7]);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);

	strSection = _T("CScanSelect");
	strKey = _T("C1");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nCScanSelect[0] + 1);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strSection = _T("CScanSelect");
	strKey = _T("C2");
	strValue = _T("");
	strValue.Format(_T("%d"), m_nCScanSelect[1] + 1);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);	
}
// wangbingfu add end on 2013-01-09
// Song Chenguang add start on 2013-04-16
void CMainFrame::OnTaskToolbar()
{
	// TODO: Add your command handler code here
	m_wndTaskDockBar.ShowControlBar(!m_wndTaskDockBar.IsVisible(),FALSE,TRUE);
}

void CMainFrame::OnUpdateTaskToolbar(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_wndTaskDockBar.IsVisible());
}
// Song Chenguang add end on 2013-04-16
#if _HAFEI
void CMainFrame::OnTailDecreasePipeParam()
{
	// TODO: Add your command handler code here
	CTailDecreasePipe pipe(_T("尾减管设置"));
	
	pipe.m_Param.m_f11LayerMin = m_f11LayerMin;
	pipe.m_Param.m_f11LayerMax = m_f11LayerMax;
	pipe.m_Param.m_f14LayerMin = m_f14LayerMin;
	pipe.m_Param.m_f14LayerMax = m_f14LayerMax;
	pipe.m_Param.m_f17LayerMin = m_f17LayerMin;
	pipe.m_Param.m_f17LayerMax = m_f17LayerMax;
	pipe.m_Param.m_f18LayerMin = m_f18LayerMin;
	pipe.m_Param.m_f18LayerMax = m_f18LayerMax;
	pipe.m_Param.m_fWeiJianW1 = m_fWeiJianW1;	// wangbingfu add on 2014-11-03
	// machao add start on 2013-08-13
	pipe.m_Unfold.m_fLeft1Start = m_nLeft1Start;
	pipe.m_Unfold.m_fLeft1Stop = m_nLeft1Stop;
	pipe.m_Unfold.m_fLeft2Start = m_nLeft2Start;
	pipe.m_Unfold.m_fLeft2Stop = m_nLeft2Stop;
	pipe.m_Unfold.m_fRight1Start = m_nRight1Start;
	pipe.m_Unfold.m_fRight1Stop = m_nRight1Stop;
	pipe.m_Unfold.m_fRight2Start = m_nRight2Start;
	pipe.m_Unfold.m_fRight2Stop = m_nRight2Stop;
	pipe.m_Sketch.m_fASketch = m_nASketch;
	pipe.m_Sketch.m_fABSketch = m_nABSketch;
	pipe.m_Sketch.m_fBSketch = m_nBSketch;
	pipe.m_Sketch.m_fBCSketch = m_nBCSketch;
	pipe.m_Sketch.m_fCSketch = m_nCSketch;
	pipe.m_Sketch.m_fCDSketch = m_nCDSketch;
	pipe.m_Sketch.m_fDSketch = m_nDSketch;
	// machao add end on 2013-08-13

	// machao modify start on 2013-08-8 
	if (pipe.DoModal() == IDOK)
	// machao modify end on 2013-08-8
	{
		m_f11LayerMin = pipe.m_Param.m_f11LayerMin;
		m_f11LayerMax = pipe.m_Param.m_f11LayerMax;
		m_f14LayerMin = pipe.m_Param.m_f14LayerMin;
		m_f14LayerMax = pipe.m_Param.m_f14LayerMax;
		m_f17LayerMin = pipe.m_Param.m_f17LayerMin;
		m_f17LayerMax = pipe.m_Param.m_f17LayerMax;
		m_f18LayerMin = pipe.m_Param.m_f18LayerMin;
		m_f18LayerMax = pipe.m_Param.m_f18LayerMax;
		m_fWeiJianW1 = pipe.m_Param.m_fWeiJianW1;	// wangbingfu add on 2014-11-03
		// machao add start on 2013-08-9
		m_nLeft1Start = pipe.m_Unfold.m_fLeft1Start;
		m_nLeft1Stop = pipe.m_Unfold.m_fLeft1Stop;
		m_nLeft2Start = pipe.m_Unfold.m_fLeft2Start;
		m_nLeft2Stop = pipe.m_Unfold.m_fLeft2Stop;
		m_nRight1Start = pipe.m_Unfold.m_fRight1Start;
		m_nRight1Stop = pipe.m_Unfold.m_fRight1Stop;
		m_nRight2Start = pipe.m_Unfold.m_fRight2Start;
		m_nRight2Stop = pipe.m_Unfold.m_fRight2Stop;
		m_nASketch = pipe.m_Sketch.m_fASketch;
		m_nABSketch = pipe.m_Sketch.m_fABSketch;
		m_nBSketch = pipe.m_Sketch.m_fBSketch;
		m_nBCSketch = pipe.m_Sketch.m_fBCSketch;
		m_nCSketch = pipe.m_Sketch.m_fCSketch;
		m_nCDSketch = pipe.m_Sketch.m_fCDSketch;
		m_nDSketch = pipe.m_Sketch.m_fDSketch;
		// machao add start on 2013-08-9
		// machao add start on 2013-08-13
		CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2,0);  
	  	pView->Invalidate(TRUE);
		CCScanView* pView2 = (CCScanView*)m_wndSplitter.GetPane(3,0);  
	  	pView2->Invalidate(TRUE);
		// machao add end on 2013-08-13
	}
}
#endif
// machao add start on 2013-08-16
LRESULT CMainFrame::OnDialogDestroyed (WPARAM wParam, LPARAM lParam)
{
	CCScanView* pView = (CCScanView*)m_wndSplitter.GetPane(2,0);
	pView->SendMessage (WM_USER_DIALOG_DESTROYED, wParam, lParam);
	CCScanView* pView2 = (CCScanView*)m_wndSplitter.GetPane(3,0);
	pView2->SendMessage (WM_USER_DIALOG_DESTROYED, wParam, lParam);
	return 0;
}
// machao add end on 2013-08-16

void CMainFrame::OnDuplicateDrawOnce()
{
	// TODO: Add your command handler code here
	m_bDuplicateDrawOnceEn = !m_bDuplicateDrawOnceEn;
}

void CMainFrame::OnUpdateDuplicateDrawOnce(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (m_bDuplicateDrawOnceEn)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CMainFrame::CommandConnect()
{
	CString strIP = _T("192.168.1.80");

	char cIP[MAX_PATH]; 
	::ZeroMemory(cIP, sizeof(cIP));
	BOOL bRet = FALSE;
	int nRet = WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, strIP,
		strIP.GetLength(), cIP, MAX_PATH, "a", &bRet);

	if (nRet == 0)
	{
		AfxMessageBox(_T("WideCharToMultiByte转换失败！发送失败！"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Leave");

		ErroWindowMessage();

		return;
	}

	cIP[nRet] = '\0';
	if(!m_CommandSocket[0].Connect(_T("192.168.1.80"), 5555))
	{
		m_CommandSocket[0].Close();		
		//		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CMainFrame::SendData"),"Leave");

		ErroWindowMessage();		
	}
}

void CMainFrame::CommandClose()
{
	m_CommandSocket[0].Close();
}

void CMainFrame::OnCommandSConnect()
{
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();
	for (int i = 0; i< nCardNum; i++)
	{
//		m_CommandSocket[i].Close();
		bool bRet = m_CommandSocket[i].Create();
		CString strIP = pApp->GetStrIP(i);
		BOOL bRet2 = m_CommandSocket[i].Connect( strIP, 5555);
		int a = 0;
	}

}

void CMainFrame::OnCommandSClose()
{
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();
	for (int i = 0; i< nCardNum; i++)
	{
		m_CommandSocket[i].Close();
	}
}
void CMainFrame::OnAScanDataStart()
{
	// TODO: Add your command handler code here
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();
	for (int i = 0; i< nCardNum; i++)
	{
		m_AScanDataSocket[i].Close();
		bool bRet = m_AScanDataSocket[i].Create();
		CString strIP = pApp->GetStrIP(i);
		BOOL bRet2 = m_AScanDataSocket[i].Connect( strIP, 2222);
		int a = 0;
	}
}

void CMainFrame::OnAScanDataStop()
{
	// TODO: Add your command handler code here
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();
	for (int i = 0; i< nCardNum; i++)
	{
		m_AScanDataSocket[i].Close();
	}
}

// Song Chenguang add start on 2013-12-19
void CMainFrame::SetGateFromFile(const int& nCardIndex, const int& nChannelIndex)
{

	int nRealChannelIndex = nCardIndex * 2 + nChannelIndex;
	int hLength = sizeof(double) * 5 + sizeof(float) * 2 + sizeof(int) * 2 + sizeof(ULONG) 
		+ nRealChannelIndex * sizeof(CGateInfo) *4;

	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetActiveDocument();
	CFile file;
	file.Open(pDoc->m_FilePath,CFile::modeRead);
	CGateInfo info[GATE_NUM];
	file.Seek(hLength, CFile::begin);
	file.Read(&info, sizeof(CGateInfo)*GATE_NUM);
	file.Close();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestView* pView = (CUltrasonicImmersedTestView*)pFrame->m_wndSplitter.GetPane(1, 0);
	for(int i=0; i<GATE_NUM; i++)
	{		
		pView->m_pGateInfo[i] = info[i];
	}

}
// Song Chenguang add end on 2013-12-19