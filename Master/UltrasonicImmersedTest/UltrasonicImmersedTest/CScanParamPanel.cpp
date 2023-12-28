// CScanParamPanel.cpp : 实现文件
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* C扫参数面板窗体视图源文件
*Filename : CScanParamPanel.cpp
*
*@author       zhaoli
*@version      1.0         Date: 2011-7-27 
*/
#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "CScanDlgBar.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "ChannelInfoDockView.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "MechanicalControl.h"
#include "SysParam.h"
#include "MechanicalControlPanel.h"
#include "CScanParamDockBar.h"
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
#include "MovePathView.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "SortClass.h"
#include "MultiColumnSortListView.h"
#include "ListCtrlEditable.h"
#include "SurfaceDetailDlg.h"
#include "HanHeRateDlg.h"
#include "UltrasonicImmersedTestDoc.h"
#include "GlobalFunction.h"
#include "FeatureInfoWnd.h"
#include "math.h"
#include "BScanView.h"
#include "SetGrayValueDlg.h"
#include "UltrasonicImmersedTestView.h"
#include "MessageDlg.h"
#include "OriginDlg.h"
// WangQianfeng add start on 2012-11-21
#include "DefectDlg.h"
// WangQianfeng add end on 2012-11-21
// CCScanParamPanel
BOOL g_bDrawRealTime = FALSE; //实时成像
//int **g_pPixelCount = NULL;	//像素点描绘次数矩阵 //delete by wangqianfeng 20121102
BOOL g_bPictureReadFile = FALSE;	//读取文件成像
extern DWORD g_dwActiveChannel;
int CCScanParamPanel::m_nObjectCount = 0;
extern BYTE *lpBitmapBits;
extern float g_fDBZeroAmp;	// Song Chenguang add on 2013-08-13
IMPLEMENT_DYNCREATE(CCScanParamPanel, CFormView)

// TEST 转存新格式数据 wangbingfu add on 2014-10-21
typedef struct
{
	double dX;
	double dY;
	float fValue;
}DAC_CONVERT_t;

CCScanParamPanel::CCScanParamPanel()
	: CFormView(CCScanParamPanel::IDD)

	, m_nDrawType(1)
// 	, m_fMinXDistance(0.5)
// 	, m_fMaxXDistance(0.5)
// 	, m_fMinYDistance(0.5)
// 	, m_fMaxYDistance(0.5)
	, m_nZoomScale(10)
	, m_fThick(20)
	, m_nColorMode(1)
	, m_nValueMode(0)
	, m_nGateIndex(1)
	, m_fGate1Threshold(0)
	, m_fGate2Threshold(0)
	, m_fGate3Threshold(0)
	, m_fGate4Threshold(0)
	, m_fBoundMin(0)
	, m_fBoundMax(100)
	, m_nAmpFeatureIndex(0)
	, m_nTimeFeatureIndex(0)
	// WangQianfeng delete start on 2013-06-28
	, m_fBScanOrigin(0)
	, m_fBScanWidth(100)
	// WangQianfeng delete end on 2013-06-28
	, m_bSurfaceFlag(FALSE)
	, m_nFeatureIndex(5)
	, m_nPictureModeIndex(0)
	, m_nPipeXMode(0)
	, m_fAveVelocity(0)
	, m_fAveShuaiJian(0)
	, m_fAveSzk(0)
	, m_fAveMidu(0)
	, m_fMinSurfaceArea(0)
	, m_bCscanSync(false)
	, m_bDisplayProbe(FALSE)
	, m_bNoteFlag(FALSE)
	, m_nNoteType(0)
	, m_nGateType(0)
	, m_nSurfaceSelType(1)
	//Machao add start on 2013-06-27
	,m_bChangeOrigin(FALSE)
	//Machao add end on 2013-06-27
	,m_nMainColorNum(3)	//machao add on 2013-07-22
	, m_bRealScale(TRUE)
	// machao delete start on 2013-09-12
//	, m_bInitializationColor(TRUE)	// machao add on 2013-08-13
	// machao delete end on 2013-09-12
	, m_nMinAMP(0)
	, m_nMaxAMP(100)
	, m_fTestAmp(80)
	, m_fSurComp(4)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::CCScanParamPanel"),"Enter");

	m_fMinRule = 0.0f;
	m_fMaxRule = 120.0f;
	m_colorBegin = RGB(0,255,0);
	m_colorEnd = RGB(255,0,0);
	m_nSurfceColorMode = 1;
	m_fBeginGrayValue = 0;
	m_fEndGrayValue = 255;
	m_bLianXuShuJu = FALSE;
	m_nObjectCount++;
	m_nObjectNum = m_nObjectCount;
	// machao modify start on 2013-08-13
//	m_nColorDefine = 1;
	m_nColorDefine = 0;
	// machao modify end on 2013-08-13
	m_fLogicalFromX = 0.0f;
	m_fLogicalToX = 0.0f;
	m_fLogicalFromY = 0.0f;
	m_fLogicalToY = 0.0f;
	m_pSHanHeRateDlg = NULL;
	m_pSurfaceDeatailDlg = NULL;
	m_nSurfaceType = 0;
	m_pMessageDlg = NULL;
	bHanHeFlag = FALSE;
	m_csPipeSetPath = _T(""); // wangbingfu add on 2013-01-09

	m_bLockParam = FALSE;//yachang.huang add start on 2014-3-7

	m_brushBackColor.CreateSolidBrush(RGB(255,255,255));
	m_clrBackColor = RGB(255,255,255);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::CCScanParamPanel"),"Leave");
}

CCScanParamPanel::~CCScanParamPanel()
{
}

void CCScanParamPanel::DoDataExchange(CDataExchange* pDX)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::DoDataExchange"),"Enter");

	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PLATEDRAWCTRL, m_PlateDrawCtrl);
	// 	DDX_Text(pDX, IDC_EDIT_MIN_X, m_fMinXDistance);
	// 	DDX_Text(pDX, IDC_EDIT_MAX_X, m_fMaxXDistance);
	// 	DDX_Text(pDX, IDC_EDIT_MIN_Y, m_fMinYDistance);
	// 	DDX_Text(pDX, IDC_EDIT_MAX_Y, m_fMaxYDistance);
	DDX_Text(pDX, IDC_EDIT_THICK, m_fThick);
	//machao delete on 2013-07-12
//	DDX_Radio(pDX, IDC_RADIO_GRAY, m_nColorMode);
	DDX_Radio(pDX, IDC_RADIO_MAX_VALUE, m_nValueMode);
	DDX_Radio(pDX, IDC_RADIO_SELECT_GATE1, m_nGateIndex);
	DDX_Text(pDX, IDC_EDIT_GATE1_THRESHOLD, m_fGate1Threshold);
	DDV_MinMaxFloat(pDX,m_fGate1Threshold,0,100);
	DDX_Text(pDX, IDC_EDIT_GATE2_THRESHOLD, m_fGate2Threshold);
	DDV_MinMaxFloat(pDX,m_fGate2Threshold,0,100);
	DDX_Text(pDX, IDC_EDIT_GATE3_THRESHOLD, m_fGate3Threshold);
	DDV_MinMaxFloat(pDX,m_fGate3Threshold,0,100);
	DDX_Text(pDX, IDC_EDIT_GATE4_THRESHOLD, m_fGate4Threshold);
	DDV_MinMaxFloat(pDX,m_fGate4Threshold,0,100);
	DDX_Text(pDX, IDC_EDIT_BOUND_MIN, m_fBoundMin);
	//	DDV_MinMaxFloat(pDX,m_fBoundMin,0,100);
	DDX_Text(pDX, IDC_EDIT_BOUND_MAX, m_fBoundMax);
	//	DDV_MinMaxFloat(pDX,m_fBoundMax,0,100);
	//  	DDX_Control(pDX, IDC_SLIDER_BSCAN_ORIGIN, m_sliderBScanOrigin);
	//  	DDX_Text(pDX, IDC_EDIT_BSCAN_ORIGIN, m_fBScanOrigin);
	//  	DDX_Control(pDX, IDC_SLIDER_BSCAN_WIDTH, m_sliderBScanWidth);
	//  	DDX_Text(pDX, IDC_EDIT_BSCAN_WIDTH, m_fBScanWidth);
	DDX_Check(pDX, IDC_CHECK_SURFACE_STATISTICS, m_bSurfaceFlag);
	DDX_Radio(pDX, IDC_RADIO_FIRST_THRESHOLD_TIME, m_nFeatureIndex);
	DDX_Control(pDX, IDC_COMBO_PICTURE_MODE, m_cmbPictureMode);
	DDX_Radio(pDX, IDC_RADIO_XMODE_ANGLE, m_nPipeXMode);
	DDX_Text(pDX, IDC_EDIT_AVE_VELOCITY, m_fAveVelocity);
	DDX_Text(pDX, IDC_EDIT_AVE_SHUAIJIAN, m_fAveShuaiJian);
	DDX_Text(pDX, IDC_EDIT_AVE_SZK, m_fAveSzk);
	DDX_Text(pDX, IDC_EDIT_AVE_MIDU, m_fAveMidu);
	DDX_Text(pDX, IDC_EDIT_MIN_AREA, m_fMinSurfaceArea);
	DDV_MinMaxFloat(pDX, m_fMinSurfaceArea, 0.0f, 1000000.0f);
	//DDX_Text(pDX, IDC_RADIO_RECT, m_nSurfaceType);
	DDX_Radio(pDX, IDC_RADIO_RECT, m_nSurfaceType);
	DDX_Check(pDX, IDC_CHECK_DISPLAY_PROBE, m_bDisplayProbe);
	DDX_Check(pDX, IDC_CHECK_ZHUSHI, m_bNoteFlag);
	DDX_Radio(pDX, IDC_RADIO_ZHUSHI_RECT, m_nNoteType);
	DDX_Radio(pDX, IDC_RADIO_GATE_ABOVE, m_nGateType);
	DDX_Radio(pDX, IDC_RADIO_CUSTUM_SURFACE, m_nSurfaceSelType);
	DDX_Control(pDX, IDC_LIST_CUSTUM_FLAW, m_lstctrlCustumFlaw);
	DDX_Check(pDX, IDC_CHECK_REAL_SCALE, m_bRealScale);
	DDX_Check(pDX, IDC_CHECK_ORIGIN, m_bChangeOrigin);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::DoDataExchange"),"Leave");
	DDX_Text(pDX, IDC_EDIT_MIN_AMP, m_nMinAMP);
	DDX_Text(pDX, IDC_EDIT_MAX_AMP, m_nMaxAMP);
	DDX_Text(pDX, IDC_EDIT_TEST_HOLE_AMP, m_fTestAmp);
	DDX_Text(pDX, IDC_EDIT_SURFACE_COMP, m_fSurComp);
}

BEGIN_MESSAGE_MAP(CCScanParamPanel, CFormView)
	//machao delete start on 2013-07-12
//	ON_BN_CLICKED(IDC_BUTTON_BEGIN_COLOR, &CCScanParamPanel::OnBnClickedButtonBeginColor)
//	ON_BN_CLICKED(IDC_BUTTON_END_COLOR, &CCScanParamPanel::OnBnClickedButtonEndColor)
	//machao delete end on 2013-07-12
	ON_BN_CLICKED(IDC_RADIO_DEEPDRAW, &CCScanParamPanel::OnBnClickedRadioDeepdraw)
	ON_BN_CLICKED(IDC_RADIO_RANGEDRAW, &CCScanParamPanel::OnBnClickedRadioRangedraw)
	ON_BN_CLICKED(IDC_BUTTON_READ_FILE, &CCScanParamPanel::OnBnClickedButtonReadFile)
	ON_BN_CLICKED(IDC_BUTTON_SURFACE_CALC, &CCScanParamPanel::OnBnClickedButtonSurfaceCalc)
	ON_BN_CLICKED(IDC_BUTTON_SURFACE_DETAIL, &CCScanParamPanel::OnBnClickedButtonSurfaceDetail)
	//machao delete start on 2013-07-12
//	ON_BN_CLICKED(IDC_RADIO_COLOR, &CCScanParamPanel::OnBnClickedRadioColor)
//	ON_BN_CLICKED(IDC_RADIO_GRAY, &CCScanParamPanel::OnBnClickedRadioGray)
	//machao delete end on 2013-07-12
	ON_BN_CLICKED(IDC_RADIO_MAX_VALUE, &CCScanParamPanel::OnBnClickedRadioMaxValue)
	ON_BN_CLICKED(IDC_RADIO_MIN_VALUE, &CCScanParamPanel::OnBnClickedRadioMinValue)
	ON_BN_CLICKED(IDC_RADIO_AVG_VALUE, &CCScanParamPanel::OnBnClickedRadioAvgValue)
	ON_BN_CLICKED(IDC_RADIO_SELECT_GATE1, &CCScanParamPanel::OnBnClickedRadioGate1)
	ON_BN_CLICKED(IDC_RADIO_SELECT_GATE2, &CCScanParamPanel::OnBnClickedRadioGate2)
	ON_BN_CLICKED(IDC_RADIO_SELECT_GATE3, &CCScanParamPanel::OnBnClickedRadioGate3)
	ON_BN_CLICKED(IDC_RADIO_SELECT_GATE4, &CCScanParamPanel::OnBnClickedRadioGate4)
	ON_BN_CLICKED(IDC_BUTTON_DRAW_PICTURE, &CCScanParamPanel::OnBnClickedButtonDrawPicture)
	ON_BN_CLICKED(IDC_BUTTON_CSCAN_REALTIME, &CCScanParamPanel::OnBnClickedButtonCscanRealtime)
	ON_BN_CLICKED(IDC_BUTTON_STOP_CSCAN, &CCScanParamPanel::OnBnClickedButtonStopCscan)
	ON_BN_CLICKED(IDC_RADIO_MAX_AMP, &CCScanParamPanel::OnBnClickedRadioMaxAmp)
	ON_BN_CLICKED(IDC_RADIO_FIRST_AMP, &CCScanParamPanel::OnBnClickedRadioFirstAmp)
	ON_BN_CLICKED(IDC_RADIO_MAX_TIME, &CCScanParamPanel::OnBnClickedRadioMaxTime)
	ON_BN_CLICKED(IDC_RADIO_MAX_THRESHOL_TIME, &CCScanParamPanel::OnBnClickedRadioMaxThresholTime)
	ON_BN_CLICKED(IDC_RADIO_FIRST_TIME, &CCScanParamPanel::OnBnClickedRadioFirstTime)
	ON_BN_CLICKED(IDC_RADIO_FIRST_THRESHOLD_TIME, &CCScanParamPanel::OnBnClickedRadioFirstThresholdTime)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_CSCAN_PICTURE, &CCScanParamPanel::OnBnClickedButtonSaveCscanPicture)
	ON_BN_CLICKED(IDC_BUTTON_CSCAN_PARAM_SAVE, &CCScanParamPanel::OnBnClickedButtonCscanParamSave)
	ON_BN_CLICKED(IDC_BUTTON_CSCAN_PARAM_READ, &CCScanParamPanel::OnBnClickedButtonCscanParamRead)
	// WangQianfeng delete start on 2013-06-28
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_THICK, &CCScanParamPanel::OnEnChangeEditThick)
	// WangQianfeng delete end on 2013-06-28
	ON_BN_CLICKED(IDC_CHECK_SURFACE_STATISTICS, &CCScanParamPanel::OnBnClickedCheckSurfaceStatistics)
	ON_EN_KILLFOCUS(IDC_EDIT_BOUND_MIN, &CCScanParamPanel::OnEnKillfocusEditBoundMin)
	ON_EN_KILLFOCUS(IDC_EDIT_BOUND_MAX, &CCScanParamPanel::OnEnKillfocusEditBoundMax)
	ON_EN_KILLFOCUS(IDC_EDIT_GATE1_THRESHOLD, &CCScanParamPanel::OnEnKillfocusEditGate1Threshold)
	ON_EN_KILLFOCUS(IDC_EDIT_GATE2_THRESHOLD, &CCScanParamPanel::OnEnKillfocusEditGate2Threshold)
	ON_EN_KILLFOCUS(IDC_EDIT_GATE3_THRESHOLD, &CCScanParamPanel::OnEnKillfocusEditGate3Threshold)
	ON_EN_KILLFOCUS(IDC_EDIT_GATE4_THRESHOLD, &CCScanParamPanel::OnEnKillfocusEditGate4Threshold)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_DATA, &CCScanParamPanel::OnBnClickedButtonSaveData)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RADIO_VELOCITY_PICTURE, &CCScanParamPanel::OnBnClickedRadioVelocityPicture)
	ON_BN_CLICKED(IDC_RADIO_VELOCITY_REDUCE_PICTURE, &CCScanParamPanel::OnBnClickedRadioVelocityReducePicture)
	ON_BN_CLICKED(IDC_RADIO_SZK_PICTURE, &CCScanParamPanel::OnBnClickedRadioSzkPicture)
	ON_BN_CLICKED(IDC_RADIO_MIDU_PICTURE, &CCScanParamPanel::OnBnClickedRadioMiduPicture)
	ON_BN_CLICKED(IDC_BUTTON_CACULATE_VELOCITY, &CCScanParamPanel::OnBnClickedButtonCaculateVelocity)
	ON_BN_CLICKED(IDC_BUTTON_CALCUATE_SHUAIJIAN, &CCScanParamPanel::OnBnClickedButtonCalcuateShuaijian)
	ON_BN_CLICKED(IDC_BUTTON_CACULATE_SZK, &CCScanParamPanel::OnBnClickedButtonCaculateSzk)
	ON_BN_CLICKED(IDC_BUTTON_CACULATE_MIDU, &CCScanParamPanel::OnBnClickedButtonCaculateMidu)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_COLOR1, &CCScanParamPanel::OnBnClickedButtonDefaultColor1)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_COLOR2, &CCScanParamPanel::OnBnClickedButtonDefaultColor2)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT_COLOR3, &CCScanParamPanel::OnBnClickedButtonDefaultColor3)		//machao add on 2013-08-5
	ON_BN_CLICKED(IDC_BUTTON_HANHE_RATE, &CCScanParamPanel::OnBnClickedButtonHanheRate)
	ON_BN_CLICKED(IDC_BUTTON_HANHE_CACULATE, &CCScanParamPanel::OnBnClickedButtonHanheCaculate)
	ON_BN_CLICKED(IDC_CHECK_CSCAN_SYNC, &CCScanParamPanel::OnBnClickedCheckCscanSync)
	//Machao add start on 2013-06-27
 	ON_BN_CLICKED(IDC_CHECK_ORIGIN, &CCScanParamPanel::OnBnClickedCheckOrigin)
	ON_BN_CLICKED(IDC_RADIO_XMODE_ANGLE, &CCScanParamPanel::OnBnRadioXmodeAngle)
	ON_BN_CLICKED(IDC_RADIO_XMODE_PERIMETER, &CCScanParamPanel::OnBnRadioXmodePerimeter)
	ON_BN_CLICKED(IDC_RADIO_GATE_ABOVE, &CCScanParamPanel::OnBnRadioGateAbove)
	ON_BN_CLICKED(IDC_RADIO_GATE_BELOW, &CCScanParamPanel::OnBnRadioGateBelow)
	//Machao add end on 2013-06-27
	ON_BN_CLICKED(IDC_RADIO_RECT, &CCScanParamPanel::OnBnClickedRadioRect)
	ON_BN_CLICKED(IDC_RADIO_CIRCLE, &CCScanParamPanel::OnBnClickedRadioCircle)
	ON_BN_CLICKED(IDC_RADIO_ELLIPSE, &CCScanParamPanel::OnBnClickedRadioEllipse)
	ON_BN_CLICKED(IDC_CHECK_DISPLAY_PROBE, &CCScanParamPanel::OnBnClickedCheckDisplayProbe)
	ON_BN_CLICKED(IDC_RADIO_POLYGON, &CCScanParamPanel::OnBnClickedRadioPolygon)
	ON_EN_CHANGE(IDC_EDIT_BOUND_MIN, &CCScanParamPanel::OnEnChangeEditBoundMin)
	ON_EN_CHANGE(IDC_EDIT_BOUND_MAX, &CCScanParamPanel::OnEnChangeEditBoundMax)
	ON_BN_CLICKED(IDC_BUTTON_ORIGIN, &CCScanParamPanel::OnBnClickedButtonOrigin)
	//Machao delete start on 2013-07-3
// 	ON_BN_CLICKED(IDC_BUTTON_DEF_COLOR0, &CCScanParamPanel::OnBnClickedButtonDefColor0)
// 	ON_BN_CLICKED(IDC_BUTTON_DEF_COLOR1, &CCScanParamPanel::OnBnClickedButtonDefColor1)
// 	ON_BN_CLICKED(IDC_BUTTON_DEF_COLOR2, &CCScanParamPanel::OnBnClickedButtonDefColor2)
// 	ON_BN_CLICKED(IDC_BUTTON_DEF_COLOR3, &CCScanParamPanel::OnBnClickedButtonDefColor3)
// 	ON_BN_CLICKED(IDC_BUTTON_DEF_COLOR4, &CCScanParamPanel::OnBnClickedButtonDefColor4)
// 	ON_BN_CLICKED(IDC_BUTTON_DEF_COLOR5, &CCScanParamPanel::OnBnClickedButtonDefColor5)
	//Machao delete end on 2013-07-3
	ON_BN_CLICKED(IDC_CHECK_ZHUSHI, &CCScanParamPanel::OnBnClickedCheckZhushi)
	ON_BN_CLICKED(IDC_RADIO_ZHUSHI_RECT, &CCScanParamPanel::OnBnClickedRadioZhushiRect)
	ON_BN_CLICKED(IDC_RADIO_ZHUSHI_CIRCLE, &CCScanParamPanel::OnBnClickedRadioZhushiCircle)
	ON_BN_CLICKED(IDC_RADIO_ZHUSHI_ELLIPSE, &CCScanParamPanel::OnBnClickedRadioZhushiEllipse)
	// WangQianfeng add start on 2012-11-21
	ON_BN_CLICKED(IDC_BUTTON_DEFECT, &CCScanParamPanel::OnBnClickedDefect)
	ON_BN_CLICKED(IDC_CHECK_REAL_SCALE, &CCScanParamPanel::OnBnClickedRealScale)
	// WangQianfeng add end on 2012-11-21
	//Machao add start on 2013-07-1
	ON_BN_CLICKED(IDC_BUTTON_PALETTE, &CCScanParamPanel::OnBnClickedButtonPalette)
	//Machao add end on 2013-07-1
	ON_BN_CLICKED(IDC_BUTTON_IMPORT_PALETTE, &CCScanParamPanel::OnBnClickedButtonImportPalette)
	ON_BN_CLICKED(IDC_LOCK_PARAM, &CCScanParamPanel::OnBnClickedLockParam)
	ON_BN_CLICKED(IDC_BUTTON_BACKGROUND_COLOR, &CCScanParamPanel::OnBnClickedButtonBackgroundColor)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCScanParamPanel 诊断

#ifdef _DEBUG
void CCScanParamPanel::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCScanParamPanel::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCScanParamPanel 消息处理程序

//
/** 
* 在工具栏窗体中创建视图
* @param     pParent [in]:  父窗口指针
* @param     pContext [in]:  窗口上下文                                            
* @return    void
* @since     1.0
*/
BOOL CCScanParamPanel::CreateView(CWnd* pParent, CCreateContext* pContext)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::CreateView"),"Enter");

	// WangQianfeng add start on 2012-11-02
	if ( NULL == pParent )
	{
		MessageBox(_T("CCScanParamPanel::CreateView,pParent = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::CreateView"),"Leave");
		return FALSE;
	}
	if ( NULL == pContext )
	{
		MessageBox(_T("CCScanParamPanel::CreateView,pContext = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::CreateView"),"Leave");
		return FALSE;
	}
	// WangQianfeng add end on 2012-11-02
	CRect rect(0,0,300,300);

	if (!Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		rect, pParent, AFX_IDW_PANE_FIRST, pContext))
	{
		TRACE0("Warning: couldn't create CMyFormView!\n");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::CreateView"),"Leave");
		return FALSE;
	}
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::CreateView"),"Leave");
	return TRUE;
}
//灰度模式下绘制颜色渐变条
void CCScanParamPanel::DrawGradientV( CDC* pDC,float fGrayBegin,float fGrayEnd,CRect& DrawRect )
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::DrawGradientV"),"Enter");

	// WangQianfeng add start on 2012-11-02
	if ( NULL == pDC )
	{
		MessageBox(_T("CCScanParamPanel::DrawGradientV,pDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::DrawGradientV"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02

	UpdateData(TRUE);

	int DrawRectWidth=DrawRect.Width();
	int DrawRectHeight=DrawRect.Height();

	CRect rect(0,DrawRectHeight - GRADLEVEL,DrawRectWidth,DrawRectHeight);

	CDC MemDC;

	CBitmap Bitmap;
	pDC->FillSolidRect(&DrawRect,RGB(255,0,0));
	Bitmap.CreateCompatibleBitmap(pDC,DrawRectWidth,DrawRectHeight);//创建位图

	MemDC.CreateCompatibleDC(pDC);     //创建内存DC
	CBitmap* pOldBmp = MemDC.SelectObject(&Bitmap);   //把位图选进内存DC

	float fDValue = (fGrayEnd - fGrayBegin)/DrawRectHeight;

	//HBRUSH hbr;
	for(int i = DrawRect.bottom; i > DrawRect.top; i -= GRADLEVEL ) 
	{
		// machao modify start on 2013-08-13
// 		if(m_nObjectNum == 1)
// 		{
// 			m_pCScanView->m_grayDefine[DrawRect.bottom - i] = RGB( fGrayBegin, fGrayBegin, fGrayBegin );
// 		}
// 		else
// 		{
// 			m_pCScanView2->m_grayDefine[DrawRect.bottom - i] = RGB( fGrayBegin, fGrayBegin, fGrayBegin );
// 		}
		m_pCurCScanView->m_grayDefine[DrawRect.bottom - i] = RGB( fGrayBegin, fGrayBegin, fGrayBegin );
		// machao modify end on 2013-08-13
		//创建刷子
		CBrush brush( RGB( fGrayBegin,fGrayBegin,fGrayBegin));
		MemDC.FillRect(&rect, &brush );

		//改变小正方体的位置
		rect.top -= GRADLEVEL;
		rect.bottom -= GRADLEVEL;

		//判断小正方体是否超界
		if( rect.bottom > DrawRect.bottom )
			rect.bottom = DrawRect.bottom;

		fGrayBegin += fDValue;
	}

	//内存DC映射到屏幕DC
	pDC->BitBlt(DrawRect.left,DrawRect.top,DrawRectWidth,DrawRectHeight,&MemDC,0,0,SRCCOPY);
	// Song Chenguang add start on 2012-11-05
	MemDC.SelectObject(pOldBmp);	
	DeleteObject(Bitmap);
	// Song Chenguang add end on 2012-11-05
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::DrawGradientV"),"Leave");
}
//彩色模式下绘制颜色渐变条
void CCScanParamPanel::DrawGradientV( CDC* pDC,COLORREF co1,COLORREF co2,CRect& DrawRect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::DrawGradientV"),"Enter");

	pDC = GetDC();	// machao add on 2013-09-10
	// WangQianfeng add start on 2012-11-02
	if ( NULL == pDC )
	{
		MessageBox(_T("CCScanParamPanel::DrawGradientV,pDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::DrawGradientV"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	UpdateData(TRUE);

	int DrawRectWidth=DrawRect.Width();
	int DrawRectHeight=DrawRect.Height();

	if ( DrawRectWidth<=0)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::DrawGradientV"),"Leave");
		return;
	}

	// machao delete start on 2013-09-10
// 	CDC MemDC;
// 
// 	CBitmap Bitmap;
// 	pDC->FillSolidRect(&DrawRect,RGB(255,0,0));
// 	Bitmap.CreateCompatibleBitmap(pDC,DrawRectWidth,DrawRectHeight);//创建位图
// 
// 	MemDC.CreateCompatibleDC(pDC);     //创建内存DC
// 	CBitmap* pOldBmp = MemDC.SelectObject(&Bitmap);   //把位图选进内存DC
	// machao delete end on 2013-09-10
	if(m_nColorDefine == 0)	// 自定义颜色
	{
		//Machao modify start on 2013-07-02
		// machao delete start on 2013-09-12
// 		// machao add start on 2013-08-13
// 		if (m_bInitializationColor)
// 		{
// 			OnBnClickedButtonDefaultColor1();
// 			m_bInitializationColor = FALSE;
// 		}
// 		// machao add end on 2013-08-13
		// machao delete end on 2013-09-12
		DrawColorBar();
// 		CRect rect(0,DrawRectHeight - GRADLEVEL,DrawRectWidth,DrawRectHeight);
// 
// 		float H1 = 0.0f;
// 		float S1 = 0.0f;
// 		float V1 = 0.0f;
// 
// 		float r = GetRValue(co1);
// 		float g = GetGValue(co1);
// 		float b = GetBValue(co1);
// 
// 		//将rgb转换成HSV
// 		Rgb2Hsv(r,g,b,H1,S1,V1);
// 
// 		float H2 = 0.0f;
// 		float S2 = 0.0f;
// 		float V2 = 0.0f;
// 
// 		float r2 = GetRValue(co2);
// 		float g2 = GetGValue(co2);
// 		float b2 = GetBValue(co2);
// 
// 		//将rgb转换成HSV
// 		Rgb2Hsv(r2,g2,b2,H2,S2,V2);
// 
// 		float ftempH = 0.0f;
// 		if (H2>H1)
// 		{
// 			ftempH = (H2 - H1)/DrawRectHeight;
// 		}
// 		else
// 		{
// 			ftempH = (360 + H2 - H1)/DrawRectHeight; 
// 
// 		}
// 
// 		float ftempS = fabs(S2 - S1)/DrawRectHeight;
// 		float ftempV = fabs(V2 - V1)/DrawRectHeight;
// 		//HBRUSH hbr;
// 		for(int i = DrawRect.bottom; i > DrawRect.top; i -= GRADLEVEL ) 
// 		{
// 			if(m_nObjectNum == 1)
// 			{
// 				m_pCScanView->m_clrDefine[DrawRect.bottom - i] = RGB( r, g, b );
// 			}
// 			else
// 			{
// 				m_pCScanView2->m_clrDefine[DrawRect.bottom - i] = RGB( r, g, b );
// 			}
// 			//创建刷子
// 			CBrush brush( RGB( r, g, b ) );
// 			MemDC.FillRect(&rect, &brush );
// 
// 
// 			//改变小正方体的位置
// 			rect.top -= GRADLEVEL;
// 			rect.bottom -= GRADLEVEL;
// 
// 			//判断小正方体是否超界
// 			if( rect.bottom > DrawRect.bottom )
// 				rect.bottom = DrawRect.bottom;
// 			// 		if( rect.top < DrawRect.top )
// 			// 			rect.top = DrawRect.bottom;
// 
// 			H1 += ftempH;
// 			if (H1>360)
// 			{
// 				H1 = 0;
// 			}
// 			S1 += ftempS;
// 			V1 += ftempV;
// 
// 			//将HSV转换成rgb
// 			Hsv2Rgb(H1,S1,V1,r,g,b);
// 		}
		//Machao modify end on 2013-07-02
	}
	else
	{
		// machao add start on 2013-09-10
		CDC MemDC;

		CBitmap Bitmap;
		pDC->FillSolidRect(&DrawRect,RGB(255,0,0));
		Bitmap.CreateCompatibleBitmap(pDC,DrawRectWidth,DrawRectHeight);//创建位图

		MemDC.CreateCompatibleDC(pDC);     //创建内存DC
		CBitmap* pOldBmp = MemDC.SelectObject(&Bitmap);   //把位图选进内存DC
		// machao add end on 2013-09-10
		COLORREF clrPlatte[21];
		// machao modify start on 2013-08-13
// 		if(m_nObjectNum == 1)
// 		{
// 			for(int i = 0; i < 21; i++)
// 			{
// 				clrPlatte[i] = m_pCScanView->m_clrPlatte[i];
// 			}
// 		}
// 		else
// 		{
// 			for(int i = 0; i < 21; i++)
// 			{
// 				clrPlatte[i] = m_pCScanView2->m_clrPlatte[i];
// 			}
// 		}
		for (int i = 0; i < 21; i++)
		{
			clrPlatte[i] = m_pCurCScanView->m_clrPlatte[i];
		}
		// machao modify end on 2013-08-13

		float fPerHeight = static_cast<float>(DrawRectHeight/20.0);//20等分渐变条，每份的高度
		CRect rect(0, static_cast<int>(DrawRectHeight - fPerHeight),DrawRectWidth,DrawRectHeight);

		for(int i = 21; i > 0; --i ) 
		{
			//创建刷子
			CBrush brush( clrPlatte[21-i] );
			MemDC.FillRect(&rect, &brush );


			//改变小正方体的位置
			rect.top -=  static_cast<LONG>(fPerHeight);
			rect.bottom -= static_cast<LONG>(fPerHeight);

			//判断小正方体是否超界
			// machao delete start on 2013-09-10
//			if( rect.bottom > DrawRect.bottom )
//			rect.bottom = DrawRect.bottom;
			// machao delete end on 2013-09-10
		}
	//Machao modify start on 2013-07-4
		//内存DC映射到屏幕DC
		pDC->BitBlt(DrawRect.left,DrawRect.top,DrawRectWidth,DrawRectHeight,&MemDC,0,0,SRCCOPY);	
 		// Song Chenguang add start on 2012-11-05
		MemDC.SelectObject(pOldBmp);
		DeleteObject(Bitmap);
		// Song Chenguang add end on 2012-11-05
	}

// 	//内存DC映射到屏幕DC
// 	pDC->BitBlt(DrawRect.left,DrawRect.top,DrawRectWidth,DrawRectHeight,&MemDC,0,0,SRCCOPY);	
// 	// Song Chenguang add start on 2012-11-05
// 	MemDC.SelectObject(pOldBmp);
// 	DeleteObject(Bitmap);
// 	// Song Chenguang add end on 2012-11-05
	//Machao modify end on 2013-07-4
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::DrawGradientV"),"Leave");
}

void CCScanParamPanel::OnDraw(CDC* pDC)
{
	// TODO: 在此添加专用代码和/或调用基类
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnDraw"),"Enter");

	// WangQianfeng add start on 2012-11-02
	if ( NULL == pDC )
	{
		MessageBox(_T("CCScanParamPanel::OnDraw,pDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::OnDraw"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	CRect rect;
	GetDlgItem(IDC_STATIC_GRADIENT)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	if (m_nColorMode == 1)
	{
		DrawGradientV(pDC,m_colorBegin,m_colorEnd,rect);
	}
	else
	{
		DrawGradientV(pDC,m_fBeginGrayValue,m_fEndGrayValue,rect);
	}
	
	
	if(m_nDrawType == 0)
	{
		m_fMinRule = 0;
		m_fMaxRule = m_fThick;
	}
	else
	{
		m_fMinRule = m_fBoundMin;
		m_fMaxRule = m_fBoundMax;
	}
	DrawRule(pDC,m_fMinRule,m_fMaxRule);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnDraw"),"Leave");
}

//machao delete start on 2013-07-12
// void CCScanParamPanel::OnBnClickedButtonBeginColor()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonBeginColor"),"Enter");
// 
// 	CColorDialog BeginColor;
// 	CClientDC dc(this);
// 	CRect rect;
// 	GetDlgItem(IDC_STATIC_GRADIENT)->GetWindowRect(&rect);
// 	ScreenToClient(&rect);
// /*	if (m_nColorMode == 1)//彩色模式下的颜色渐变条
// 	{
// 		if (IDOK == BeginColor.DoModal())
// 		{
// 			m_nColorDefine = 0;
// 			COLORREF color = BeginColor.GetColor();
// 			m_colorBegin = color;
// 			DrawGradientV(&dc,m_colorBegin,m_colorEnd,rect);
// 
// 		}
// 	}
// 	else //灰度模式下的颜色渐变*/
// 	{
// 		CSetGrayValueDlg GrayValueDlg;
// 
// 		GrayValueDlg.m_nGrayValue = static_cast<int>(m_fBeginGrayValue);
// 		if (IDOK == GrayValueDlg.DoModal())
// 		{
// 			m_fBeginGrayValue = static_cast<float>(GrayValueDlg.m_nGrayValue);
// 			DrawGradientV(&dc,m_fBeginGrayValue,m_fEndGrayValue,rect);
// 		}
// 		
// 	}
// 
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonBeginColor"),"Leave");
// }
// 
// void CCScanParamPanel::OnBnClickedButtonEndColor()
// {
// 	// TODO: 在此添加控件通知处理程序代码
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonEndColor"),"Enter");
// 
// 	CColorDialog EndColor;
// 	CClientDC dc(this);
// 	CRect rect;
// 	GetDlgItem(IDC_STATIC_GRADIENT)->GetWindowRect(&rect);
// 	ScreenToClient(&rect);
// /*	if (m_nColorMode == 1)
// 	{
// 		if (IDOK == EndColor.DoModal())
// 		{
// 			m_nColorDefine = 0;
// 			COLORREF color = EndColor.GetColor();
// 			m_colorEnd = color;
// 			DrawGradientV(&dc,m_colorBegin,m_colorEnd,rect);
// 
// 		}
// 	}
// 	else //灰度模式下的颜色渐变*/
// 	{
// 		CSetGrayValueDlg GrayValueDlg;
// 		GrayValueDlg.m_nGrayValue = static_cast<int>(m_fEndGrayValue);
// 		if (IDOK == GrayValueDlg.DoModal())
// 		{
// 			m_fEndGrayValue = static_cast<float>(GrayValueDlg.m_nGrayValue);
// 			DrawGradientV(&dc,m_fBeginGrayValue,m_fEndGrayValue,rect);
// 		}	
// 	}
// 	
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonEndColor"),"Leave");
// }
//machao delete end on 2013-07-12

//画刻度尺
void CCScanParamPanel::DrawRule(CDC* pDC,float fStart, float fEnd)
{
	// WangQianfeng add start on 2012-11-02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::DrawRule"),"Enter");

	pDC = GetDC();	// machao add on 2013-09-10
	if ( NULL == pDC )
	{
		MessageBox(_T("CCScanParamPanel::DrawRule,pDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::DrawRule"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02

	CString str;
	CRect rect;
	CPoint StartPoint,EndPoint;
	GetDlgItem(IDC_STATIC_GRADIENT)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	int n = 10;
	float fStep = (fEnd - fStart)/n;
	//CClientDC dc(this);
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	//int n = (fEnd - fStart)/fStep;
	LONG lHalfHeight = tm.tmHeight/2;
	float fPerCoordinate = float(rect.Height())/float(fEnd - fStart);

	StartPoint.y = EndPoint.y;
	for (int i = 0;i<= n;i++)
	{
		float fYData = fStart + (fStep*i);
		str.Format(_T("%.0f"),fYData);
		StartPoint.x = rect.right;
		EndPoint.x = rect.right + 8;
	
		
		//dc.SetTextAlign(TA_LEFT);
		pDC->SetBkMode(TRANSPARENT);
		pDC->TextOut(EndPoint.x+8,static_cast<int>(rect.bottom - fPerCoordinate*fStep*i -lHalfHeight),str);

		pDC->MoveTo(rect.right,static_cast<int>(rect.bottom - fPerCoordinate*fStep*i));
		pDC->LineTo(rect.right +8 ,static_cast<int>(rect.bottom - fPerCoordinate*fStep*i));
	}
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::DrawRule"),"Leave");
}
//深度成像
void CCScanParamPanel::OnBnClickedRadioDeepdraw()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioDeepdraw"),"Enter");

	m_nDrawType = 0;
	CClientDC dc(this);
	float fMinDeep = 0;
	float fMaxDeep = m_fThick;
	float fDeepStep = static_cast<float>((fMaxDeep-fMinDeep)/10.0);
	//DrawRule(&dc,fMinDeep,fMaxDeep,fDeepStep);
	m_fMinRule = fMinDeep;
	m_fMaxRule = fMaxDeep;
	//m_fStepRule = fDeepStep;
	CDC *pDC = GetDC();
	// WangQianfeng add start on 2012-11-02
	if ( NULL == pDC )
	{
		MessageBox(_T("CCScanParamPanel::OnBnClickedRadioDeepdraw,pDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioDeepdraw"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	DrawRule(pDC,m_fMinRule,m_fMaxRule);
	Invalidate(TRUE);
	// WangQianfeng add start on 2012-11-02
	ReleaseDC(pDC);
	// WangQianfeng add end on 2012-11-02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioDeepdraw"),"Leave");
}
//幅度成像
void CCScanParamPanel::OnBnClickedRadioRangedraw()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioRangedraw"),"Enter");

	m_nDrawType = 1;
	CClientDC dc(this);
	float fMinRange = m_fBoundMin;
	float fMaxRange = m_fBoundMax;
	//float fRangeStep = (fMaxRange - fMinRange)/10.0; 

	m_fMinRule = fMinRange;
	m_fMaxRule = fMaxRange;
	//m_fStepRule = fRangeStep;
	CDC *pDC = GetDC();
	// WangQianfeng add start on 2012-11-02
	if ( NULL == pDC )
	{
		MessageBox(_T("CCScanParamPanel::OnBnClickedRadioRangedraw,pDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioRangedraw"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	DrawRule(pDC,m_fMinRule,m_fMaxRule);
	Invalidate(TRUE);
	// WangQianfeng add start on 2012-11-02
	ReleaseDC(pDC);
	// WangQianfeng add end on 2012-11-02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioRangedraw"),"Leave");
}
//读文件
void CCScanParamPanel::ReadFile()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::ReadFile"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;

	// TODO: Add your control notification handler code here

		// 	if (m_bLianXuShuJu)
		// 	{
		for (int i = 0; i<CHANNEL_NUM; i++)
		{
			pDoc->m_lstFlawData[i].RemoveAll();
			pDoc->m_lstFlawDataOriginal[i].RemoveAll();	// 2011-11-18
		}
		//}

		CFile file;
		CCScanFileHeader fileHeader;
		DWORD dwChnCount[CHANNEL_NUM];	//通道采集点数
		memset(dwChnCount, 0 ,sizeof(dwChnCount));

		file.Open(m_filename,CFile::modeRead);
		//读取设定参数
		file.Read(&fileHeader,sizeof(CCScanFileHeader));

		if (fileHeader.m_nFillType == 0) // 横向
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace2 = (double)fileHeader.m_fGridX;
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace = (double)fileHeader.m_fGridY;
		} 
		else	// 纵向
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace = (double)fileHeader.m_fGridX;
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace2 = (double)fileHeader.m_fGridY;
		}
		CString str;
		str.Format(_T("%.2f"), pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace2);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("FillSpace2"), str);
		// Song Chenguang add start on 2013-08-27
		// 保存步进间距、平面到系统参数
		double sysPara[SYSPARAM1_NUM];
		long logPara[10];
		CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0, 0);
		pMovePathView->m_GraphMove.GetPara(sysPara, logPara);
		sysPara[17] = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_dFillSpace;
		pMovePathView->m_GraphMove.SetPara(sysPara, logPara);
		pMovePathView->m_GraphMove.SetAxisSelect(fileHeader.m_nAxisSelect);
		// Song Chenguang add end on 2013-08-27

		//闸门开关
		for(int i = 0; i < CHANNEL_NUM; i++)
		{
			for(int j = 0; j < GATE_NUM; j++)
			{
				float fGatePeak = static_cast<float>(fileHeader.m_GateInfo[i][j].m_fPeakGate);			
				fileHeader.m_GateInfo[i][j].m_fPeakGate = int(fGatePeak * 10) / 10.0f;
				pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo[i].m_GateInfo[j] = fileHeader.m_GateInfo[i][j];
			}
		}

		// 更新闸门
		pFrame->m_wndChannelInfo.m_pFormView->SetGateInfo();
		pFrame->m_wndChannelInfo.m_pFormView->UpdateData(FALSE);
		int nChannelNo = pFrame->m_wndChannelInfo.m_pFormView->m_nChannelNo;
		for(int i = 0; i < GATE_NUM; i++)
		{
			m_pAScanView->m_pGateInfo[i] = pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo[nChannelNo].m_GateInfo[i];
		}
		m_pAScanView->InvalidateRect(m_pAScanView->m_rectWave, FALSE);

		DWORD i_cc=0;
		for(int i = 0; i < fileHeader.m_wChannelNum; i++)
		{
			file.Read(&dwChnCount[i], sizeof(DWORD));
			i_cc+=dwChnCount[i];
		}

		// 特征值开关
		pFrame->m_wndChannelInfo.m_pFormView->m_bFPFTimeDisplay = fileHeader.m_bFPFTimeDisplay;
		pFrame->m_wndChannelInfo.m_pFormView->m_bFPMTimeDisplay = fileHeader.m_bFPMTimeDisplay;
		pFrame->m_wndChannelInfo.m_pFormView->m_bFPMAltitudeDisplay = fileHeader.m_bFPMAltitudeDisplay;
		pFrame->m_wndChannelInfo.m_pFormView->m_bMPFTimeDisplay = fileHeader.m_bMPFTimeDisplay;
		pFrame->m_wndChannelInfo.m_pFormView->m_bMPMTimeDisplay = fileHeader.m_bMPMTimeDisplay;
		pFrame->m_wndChannelInfo.m_pFormView->m_bMPMAltitudeDisplay = fileHeader.m_bMPMAltitudeDisplay;
		pFrame->m_wndChannelInfo.m_pFormView->UpdateData(FALSE);
		pFrame->m_wndChannelInfo.m_pFormView->ALLSCanItemEnableJudge();	
		// 声速
		pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed = fileHeader.m_wWaveSpeed;
		// 工件厚度
		m_fThick = fileHeader.m_fThick;

		//根据平面选择改变W1正转和W1反转按钮状态
	//	fileHeader.m_nAxisSelect = 4; // test wangbingfu add on 2013-01-09
		if(fileHeader.m_nAxisSelect == 3
			|| fileHeader.m_nAxisSelect == 4)	//W1 wangbingfu add on 2013-01-09
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_R_POSITIVE)->EnableWindow(TRUE);
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_R_REVERSE)->EnableWindow(TRUE);
		}		
		else
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_R_POSITIVE)->EnableWindow(FALSE);
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_R_REVERSE)->EnableWindow(FALSE);
		}

		// rem by Song Chenguang om 2013-10-22
		// 不控制了
/*
		// Song Chenguang add start on 2013-05-04
		if(fileHeader.m_nAxisSelect == 5)	// W平面
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W_POSITIVE)->EnableWindow(TRUE);
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W_REVERSE)->EnableWindow(TRUE);
		}		
		else
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W_POSITIVE)->EnableWindow(FALSE);
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W_REVERSE)->EnableWindow(FALSE);
		}
		// Song Chenguang add end on 2013-05-04
*/
		// 网格间距(暂时用默认值)
		UpdateData(FALSE);	// 更新控件显示的值
		// 相关控件状态更新

		// 统计闸门开的个数
		// 统计特征值开的个数
		int nFeatureNum = 0;
		if(fileHeader.m_bMPFTimeDisplay)
			nFeatureNum++;
		if(fileHeader.m_bMPMTimeDisplay)
			nFeatureNum++;
		if(fileHeader.m_bMPMAltitudeDisplay)
			nFeatureNum++;
		if(fileHeader.m_bFPFTimeDisplay)
			nFeatureNum++;
		if(fileHeader.m_bFPMTimeDisplay)
			nFeatureNum++;
		if(fileHeader.m_bFPMAltitudeDisplay)
			nFeatureNum++;
		nFeatureNum = nFeatureNum + 1;

		int nGateNum[CHANNEL_NUM] = {0};

		// test 转存新格式数据 wangbingfu add on 2014-10-21
		//CFile fileConvert;
		//fileConvert.Open(_T("D:/convertData.txt"), CFile::modeReadWrite | CFile::modeCreate);
		// test 

		if(i_cc==0)
		{
			for(int m = 0; m < fileHeader.m_wChannelNum; m++)
			{
				for (int i=0; i< 4; i++)
				{
					if (fileHeader.m_GateInfo[m][i].m_bOpen)
						nGateNum[m]++;
				}
			}
			ULONGLONG FileSize=file.GetLength();
			while(file.GetPosition()<FileSize)
			{

				int m;
				ULONG dlength;

				file.Read(&m,sizeof(int));
				CFeatureInfo featureInfo(nGateNum[m], nFeatureNum);

				file.Read(&dlength,sizeof(ULONG));
				//				file.Read(&featureInfo,dlength);
				file.Read(&featureInfo.m_dX, sizeof(double));
				file.Read(&featureInfo.m_dY, sizeof(double));
				file.Read(&featureInfo.m_dZ, sizeof(double));
				file.Read(&featureInfo.m_dR, sizeof(double));

				file.Read(&featureInfo.m_dA, sizeof(double));
				file.Read(&featureInfo.m_dB, sizeof(double));
				file.Read(&featureInfo.m_dW, sizeof(double));

				file.Read(&featureInfo.m_nGateNum, sizeof(int));
				file.Read(&featureInfo.m_nFeatureNum, sizeof(int));
				file.Read(&featureInfo.m_nCount, sizeof(int));

				for (int j=0; j<featureInfo.m_nGateNum; j++)
				{
					file.Read(featureInfo.m_pfFeatures[j], sizeof(float)*featureInfo.m_nFeatureNum);
				}

				//////////////////////////////////////////////////////////////////////////
				if (featureInfo.m_dW > 175)
				{
					int a = 0;
				}
				//////////////////////////////////////////////////////////////////////////
				pDoc->m_lstFlawData[m].AddTail(featureInfo);
				
				// test 转存新格式数据 wangbingfu add on 2014-10-21
// 				if(m == 0)
// 				{
// 					DAC_CONVERT_t stData;
// 					stData.dX = featureInfo.m_dX;
// 					stData.dY = featureInfo.m_dY;
// 					stData.fValue = featureInfo.m_pfFeatures[1][3];
// 					fileConvert.Write(&stData, sizeof(DAC_CONVERT_t));
// 				}
				// test
				//TRACE( "%s\n",);
			}
		}
		else
		{
			for(int m = 0; m < fileHeader.m_wChannelNum; m++)
			{
				for (int i=0; i< 4; i++)
				{
					if (fileHeader.m_GateInfo[m][i].m_bOpen)
						nGateNum[m]++;
				}

				CFeatureInfo featureInfo(nGateNum[m], nFeatureNum);	// 特征数据
				float fFeatureData[28];
				int nSize = 4 * sizeof(double) + 2 * sizeof(int);
				int nFeatureSize = sizeof(float) * nGateNum[m] * nFeatureNum;
				int nGateSize = sizeof(float) * nFeatureNum;

				for(UINT k = 0; k < dwChnCount[m]; k++)
				{
					file.Read(&featureInfo, nSize);

					file.Read(&fFeatureData, nFeatureSize);
					for(int i = 0; i < nGateNum[m]; i++)
					{
						for(int j = 0; j < nFeatureNum; j++)
						{
							featureInfo.m_pfFeatures[i][j] = fFeatureData[i * nFeatureNum + j];
						}
					}
					pDoc->m_lstFlawData[m].AddTail(featureInfo);
					//pDoc->m_qCScanData[m].push(featureInfo);
				}

			}
		}
		// wangbingfu add start on 2013-01-09
		if(fileHeader.m_nAxisSelect == 4)
		{
			fileHeader.m_nWorkMode = 3; // 双单
		}
		// wangbingfu add end on 2013-01-09
		// machao modify start on 2013-08-14
//		SetCScanDrawPicturePara(m_pCScanView, fileHeader, nGateNum, nFeatureNum);
//		SetCScanDrawPicturePara(m_pCScanView2, fileHeader, nGateNum, nFeatureNum);

		CCScanView *pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
		CCScanView *pCScanView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
		SetCScanDrawPicturePara(pCScanView, fileHeader, nGateNum, nFeatureNum);
		SetCScanDrawPicturePara(pCScanView2, fileHeader, nGateNum, nFeatureNum);		
		// machao modify end on 2013-08-14

		// 设置扫查方式 wangbingfu add start at 2012-9-25
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode = (WORD)fileHeader.m_nWorkMode;
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nIndexProbeNum = fileHeader.m_nProbeNum;
		if(fileHeader.m_nProbeNum == 0)		// 1块卡
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nIndexOneProbeType = fileHeader.m_nTwoProbeIndex;
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.SetMoveRule(fileHeader.m_nProbeNum+1, fileHeader.m_nTwoProbeIndex, 
				0, 0, fileHeader.m_nWorkMode);
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.SetChannelStateNew(fileHeader.m_nProbeNum+1, fileHeader.m_nTwoProbeIndex, 
				0, 0, fileHeader.m_nWorkMode);
		}
		else if(fileHeader.m_nProbeNum == 1)	// 2块卡
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nIndexTwoProbeType = fileHeader.m_nTwoProbeIndex;
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.SetMoveRule(fileHeader.m_nProbeNum+1, 0, 
				fileHeader.m_nTwoProbeIndex, 0, fileHeader.m_nWorkMode);
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.SetChannelStateNew(fileHeader.m_nProbeNum+1, 0, 
				fileHeader.m_nTwoProbeIndex, 0, fileHeader.m_nWorkMode);
		}
		else if(fileHeader.m_nProbeNum == 2)	// 3块卡
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nIndexThreeProbeType = fileHeader.m_nTwoProbeIndex;
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.SetMoveRule(fileHeader.m_nProbeNum+1, 0, 
				fileHeader.m_nTwoProbeIndex, 0, fileHeader.m_nWorkMode);
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.SetChannelStateNew(fileHeader.m_nProbeNum+1, 0, 
				fileHeader.m_nTwoProbeIndex, 0, fileHeader.m_nWorkMode);
		}
		else	// 4块卡
		{
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.SetMoveRule(fileHeader.m_nProbeNum+1, 0, 
				0, 0, fileHeader.m_nWorkMode);
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.SetChannelStateNew(fileHeader.m_nProbeNum+1, 0, 
				0, 0, fileHeader.m_nWorkMode);
		}
		//wangbingfu add end at 2012-9-25
		int nCardNum = pApp->GetCardNum();
		for (int i=0; i<nCardNum; i++)
		{
			pChannelPanel->SendWorkMode(i, fileHeader.m_nWorkMode);
		}
		
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
			if(fileHeader.m_nWorkMode == 4 || fileHeader.m_nWorkMode == 5 ||
				fileHeader.m_nWorkMode == 6 ||fileHeader.m_nWorkMode == 7 ||
				fileHeader.m_nWorkMode == 8) // 关联
			{
				pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nGateType = 1;
				pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->m_nGateType = 0;
			}
			else
			{
				pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nGateType = 0;
				pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->m_nGateType = 1;
			}

		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->UpdateData(FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->UpdateData(FALSE);

		
		CString strOtherPath;
		strOtherPath = m_filename.Mid(0, m_filename.GetLength() - 4);

		if (PathFileExists(strOtherPath+L".apf"))
		{
			CMovePathView* mdsocx = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
			mdsocx->m_GraphMove.OpenFileByPath(strOtherPath+L".apf");
		}

		pDoc->m_FilePath=strOtherPath+L".aaf";
		pDoc->m_FilePath1 = strOtherPath+L"1.aaf"; // wangbingfu add on 2014-09-10

		if (PathFileExists(strOtherPath+L".asf"))
		{
			pChannelPanel->ReadParam(strOtherPath+L".asf");
			pChannelPanel->DoChangeChannel();
			pChannelPanel->UpdateData(FALSE);
		}

	//	g_bPictureReadFile = TRUE;

		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::ReadFile"),"Leave");
}
void CCScanParamPanel::OnBnClickedButtonReadFile()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonReadFile"),"Enter");

	CFileDialog dlg(TRUE,_T(".uit"),_T("*.uit"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("文本文件(*.uit)|*.uit||"));

	if(IDOK == dlg.DoModal())
	{
		m_filename = dlg.GetPathName();	// Song Chenguang modify on 2014-01-16

		CString strPath = dlg.GetPathName();
		CString strNotePath = _T("");
		strNotePath = strPath.Mid(0, strPath.GetLength() - 4);
		strNotePath = strNotePath + L".note";
		// machao modify start on 2013-08-14
// 		m_pCScanView->m_strNotePath = strNotePath;
 //		m_pCScanView2->m_strNotePath = strNotePath;

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		// machao add start on 2013-11-22
		CString strTitle = _T("奥瑞视 - 水浸超声检测系统 - ") + strPath;
		pFrame->SetWindowText(strTitle);
		// machao add end on 2013-11-22
		CCScanView *pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
		CCScanView *pCScanView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
		pCScanView->m_strNotePath = strNotePath;
		pCScanView2->m_strNotePath = strNotePath;
		// machao modify end on 2013-08-14
	//	Note_SetFilePath(m_strNotePath);
		// wangbingfu add start on 2013-01-09
		m_csPipeSetPath = strPath.Mid(0, strPath.GetLength() - m_filename.GetLength());
		m_csPipeSetPath = m_csPipeSetPath + _T("PipeSet.ini");
		// wangbingfu add end on 2013-01-09

		m_pMessageDlg = new CMessageDlg;
		if(m_pMessageDlg->GetSafeHwnd() == NULL)
		{
			m_pMessageDlg->Create(IDD_DIALOG_MESSAGE, this);
			m_pMessageDlg->SetWindowPos(&wndTop,0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		}
		m_pMessageDlg->ShowWindow(SW_SHOW);
		SetTimer(TIMER_WAITT_FOR_READ, 50, NULL);
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonReadFile"),"Leave");
}

void CCScanParamPanel::SurfaceCalc()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::SurfaceCalc"),"Enter");

	LOGICAL_POINT_t* ptArray = NULL;
	if (m_nFeatureIndex > 5)
	{
		AfxMessageBox(_T("面伤计算不支持特征成像！"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::SurfaceCalc"),"Leave");
		return;
	}
	
	CCScanView* pCScanView = NULL;
	// machao modify start on 2013-08-14
// 	if(m_nObjectNum == 1)
// 		pCScanView = m_pCScanView;
// 	else
// 		pCScanView = m_pCScanView2;
	pCScanView = m_pCurCScanView;
	// machao modify end on 2013-08-14

	UpdateData(TRUE);

	if (m_nSurfaceType == 1)	// 圆
	{
		// machao modify start on 2013-11-6
		CPoint ptRightTop;
		ptRightTop.x = pCScanView->m_ptFrom.x > pCScanView->m_ptTo.x ? pCScanView->m_ptFrom.x : pCScanView->m_ptTo.x;
		CPoint ptLeftBottom;
		ptLeftBottom.x = pCScanView->m_ptFrom.x < pCScanView->m_ptTo.x ? pCScanView->m_ptFrom.x : pCScanView->m_ptTo.x;

		ptRightTop.y = (pCScanView->m_ptFrom.y + pCScanView->m_ptTo.y) / 2 - abs(pCScanView->m_ptTo.x - pCScanView->m_ptFrom.x) / 2;
		ptLeftBottom.y = (pCScanView->m_ptFrom.y + pCScanView->m_ptTo.y) / 2 + abs(pCScanView->m_ptTo.x - pCScanView->m_ptFrom.x) / 2;
		// machao modify end on 2013-11-6

		pCScanView->m_ptFrom = ptLeftBottom;
		pCScanView->m_ptTo = ptRightTop;
	} 
	else if (m_nSurfaceType == 3)	// 多边形
	{
		// 获取最大的X和最小的Y
		int nMinX = pCScanView->m_Poly[0].x;
		int nMaxX = pCScanView->m_Poly[0].x;
		int nMinY = pCScanView->m_Poly[0].y;
		int nMaxY = pCScanView->m_Poly[0].y;

		for (int i=1; i<pCScanView->m_nPolyNum+1; i++)
		{
			if (pCScanView->m_Poly[i].x < nMinX)
			{
				nMinX = pCScanView->m_Poly[i].x;
			}

			if (pCScanView->m_Poly[i].x > nMaxX)
			{
				nMaxX = pCScanView->m_Poly[i].x;
			}

			if (pCScanView->m_Poly[i].y < nMinY)
			{
				nMinY = pCScanView->m_Poly[i].y;
			}

			if (pCScanView->m_Poly[i].y > nMaxY)
			{
				nMaxY = pCScanView->m_Poly[i].y;
			}
		}
		pCScanView->m_ptFrom = CPoint(nMinX, nMaxY);
		pCScanView->m_ptTo = CPoint(nMaxX, nMinY);
	}
	else
	{
	}

	LOGICAL_POINT_t ptLogicalFrom;
	LOGICAL_POINT_t ptLogicalTo;
	ptLogicalFrom = pCScanView->Client2Logical(pCScanView->m_ptFrom);
	ptLogicalTo = pCScanView->Client2Logical(pCScanView->m_ptTo);

	// 获取扫查范围
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	m_PlateDrawCtrl.clear();

	if (m_nSurfaceType == 1)	// 圆
	{
		LOGICAL_POINT_t l_ptRightTop = pCScanView->Client2Logical(pCScanView->m_ptTo);

		m_PlateDrawCtrl.SetPlateInfo(l_ptRightTop.x * m_nZoomScale, 
			l_ptRightTop.y * m_nZoomScale, m_fThick);
	}
	else if (m_nSurfaceType == 3)	// 多边形
	{

		LOGICAL_POINT_t l_ptRightTop = pCScanView->Client2Logical(pCScanView->m_ptTo);

		m_PlateDrawCtrl.SetPlateInfo(l_ptRightTop.x * m_nZoomScale, 
			l_ptRightTop.y * m_nZoomScale, m_fThick);

		//		m_PlateDrawCtrl.SetPlateInfo(pCScanView->m_dXEnd * m_nZoomScale, 
		//			pCScanView->m_dYEnd * m_nZoomScale, m_fThick);
		ptArray = new LOGICAL_POINT_t[pCScanView->m_nPolyNum+1];
		if (NULL != ptArray)  //add by wangqianfeng on 20121106
		{
			for (int i=0; i<pCScanView->m_nPolyNum+1; i++)
			{
				ptArray[i] = pCScanView->Client2Logical(pCScanView->m_Poly[i]);
			}
		}
	}
	else
	{
		float fLength = ptLogicalTo.x > ptLogicalFrom.x ? ptLogicalTo.x : ptLogicalFrom.x;
		float fWidth = ptLogicalTo.y > ptLogicalFrom.y ? ptLogicalTo.y : ptLogicalFrom.y;

		m_PlateDrawCtrl.SetPlateInfo(fLength * m_nZoomScale, 
			fWidth * m_nZoomScale, m_fThick);
	}


	// 获取扫描间隔
	CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
	float fDiameter = pMovePathView->m_GraphMove.GetPipR();	// Song Chenguang add on 2013-09-06
	// machao add start on 2013-12-4
	if (pCScanView->m_nAxisSelect == 5)
	{
		fDiameter = pMovePathView->m_GraphMove.Get_WallDiameter();
	}
	// machao add end on 2013-12-4

	float fDistanceX = pCScanView->m_fGridX;
	float fDistanceY = pCScanView->m_fGridY;
	m_PlateDrawCtrl.SetDistanceX(fDistanceX * m_nZoomScale, fDistanceX * m_nZoomScale);
	m_PlateDrawCtrl.SetDistanceY(fDistanceY * m_nZoomScale, fDistanceY * m_nZoomScale);
	m_PlateDrawCtrl.SetMaxZDistance(1);

	m_PlateDrawCtrl.SetZhiShiLengthType(1);
	m_PlateDrawCtrl.SetShangWidthType(2);
	m_PlateDrawCtrl.SetAreaType(1);
	m_PlateDrawCtrl.SetMergeShangType(2, 1.0);
	m_PlateDrawCtrl.SetEdgeWidth(0);

	m_PlateDrawCtrl.SetNotUseBodyMinArea(m_fMinSurfaceArea * m_nZoomScale * m_nZoomScale);
	m_PlateDrawCtrl.SetNotUseBodyMinShangZhiShiLength(0);
	m_PlateDrawCtrl.SetNotUseBodyMinShangLength(0);
	m_PlateDrawCtrl.SetNotUseBodyMinShangWidth(0);
	m_PlateDrawCtrl.SetNotUseEdgeMinArea(m_fMinSurfaceArea * m_nZoomScale * m_nZoomScale);
	m_PlateDrawCtrl.SetNotUseEdgeMinShangZhiShiLength(0);
	m_PlateDrawCtrl.SetNotUseEdgeMinShangLength(0);
	m_PlateDrawCtrl.SetNotUseEdgeMinShangWidth(0);

	// 加点
	int nFeatureIndex = 4;	//特征选择,选择最大峰值时间
	int nMaxAmpIndex = 6;	//门内最大峰值幅度
	int nMaxAmpIndexTemp = 0;	//门内最大峰值幅度
	int nGateIndexTemp = 0;
	int nFeatureIndexTemp = 0;
	BOOL bFeatureFlag[7];
	bFeatureFlag[0] = FALSE;
	for(int i= 1; i < 7; i++)
	{
		bFeatureFlag[i] = pCScanView->m_bFileFeatureFlag[i];
	}

	nFeatureIndexTemp = nFeatureIndex;
	nMaxAmpIndexTemp = nMaxAmpIndex;

	for(int j = 1; j < nFeatureIndex; j++)
	{
		if (!bFeatureFlag[j])
		{
			nFeatureIndexTemp--;
		}
	}
	for(int j = 1; j < nMaxAmpIndex; j++)
	{
		if (!bFeatureFlag[j])
		{
			nMaxAmpIndexTemp--;
		}
	}

	CUltrasonicImmersedTestDoc* pDoc = GetDocument();


	if(ptLogicalFrom.x < ptLogicalTo.x)
	{
		m_fLogicalFromX = ptLogicalFrom.x;
		m_fLogicalToX = ptLogicalTo.x;
	}
	else
	{
		m_fLogicalFromX = ptLogicalTo.x;
		m_fLogicalToX = ptLogicalFrom.x;
	}

	if(ptLogicalFrom.y < ptLogicalTo.y)
	{
		m_fLogicalFromY = ptLogicalFrom.y;
		m_fLogicalToY = ptLogicalTo.y;
	}
	else
	{
		m_fLogicalFromY = ptLogicalTo.y;
		m_fLogicalToY = ptLogicalFrom.y;
	}

	//wangbingfu add start on 2015-07-08 test
/*	float fXStep = pCScanView->m_fGridX; // X间距
	float fYStep = pCScanView->m_fGridY; // Y间距

	float fXLength = static_cast<float>(fabs(pCScanView->m_dXEnd - pCScanView->m_dXStart));	// 工件X轴长度
	float fYLength = static_cast<float>(fabs(pCScanView->m_dYEnd - pCScanView->m_dYStart)); // 工件Y轴长度

	int nXLength = int(fXLength / fXStep);	//画工件的X像素长度
	int nYLength = int(fYLength / fYStep);	//画工件的Y像素长度
	for(int i = 0; i < nXLength; i++)
	{
		float x = i * pCScanView->m_fGridX + pCScanView->m_dXStart;
		for(int j = 0; j < nYLength; j++)
		{
			float y = j * pCScanView->m_fGridY + pCScanView->m_dYStart;
			if(pCScanView->m_pPixel[i][j] >= 0)
			{
				if((x >= m_fLogicalFromX) && (x <= m_fLogicalToX)
					&& (y >= m_fLogicalFromY) && (y <= m_fLogicalToY))
				{
					if(m_nGateType == 0)
					{
						if(pCScanView->m_pPixel[i][j] >= pCScanView->m_fGateThreshold[nGateIndexTemp])
						{
							m_PlateDrawCtrl.AddPoint2(x * m_nZoomScale, y * m_nZoomScale, 
								9, 2, pCScanView->m_pPixel[i][j] , 0, 0, 0, 0);
						}
					}
					else
					{
						if(pCScanView->m_pPixel[i][j] < pCScanView->m_fGateThreshold[nGateIndexTemp])
						{
							m_PlateDrawCtrl.AddPoint2(x * m_nZoomScale, y * m_nZoomScale, 
								9, 2, pCScanView->m_pPixel[i][j] , 0, 0, 0, 0);
						}
					}
					
				}
			}		
		}
	}*/
	//wangbingfu add end on 2015-07-08 test

	long lCount = 0;
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		if(pCScanView->m_bGuanLianFlag)	// 关联模式
		{
			if(pCScanView->m_nObjectNum == 1)	// 只画底波 单通道底波 双通道伤波
			{
				if(i == 1 || i == 3 || i == 5 || i == 7)
				{
					continue;
				}
			}
			else if(pCScanView->m_nObjectNum == 2)	// 只画伤波
			{
				if(i == 0 || i == 2 || i == 4 || i == 6)
				{
					continue;
				}
			}
			else
			{
				;//
			}
		}

		nGateIndexTemp = m_nGateIndex;

		for(int j = 0; j < m_nGateIndex; j++)
		{
			if(!pCScanView->m_bFileGateFlag[i][j])
			{
				nGateIndexTemp--;
			}
		}
	//	int num = 0;
		for(POSITION pos = pDoc->m_lstFlawData[i].GetHeadPosition();pos; )
		{
		/*	if(lCount >= 1000000)
			{
				int nCount = (lCount - 1000000) / (100 * (i+1));
				if(nCount > num)
				{
					CString str;
					str.Format(_T("add point num %d\n"), lCount);
					TRACE0(str);
					OutputDebugString(str);
					num++;
				}
			}*/
			
			if (lCount > MAX_POINTS_NUM)
			{
				MessageBox(_T("伤点个数超出500,000！"));
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::SurfaceCalc"),"Leave");
				return;
			}
			CFeatureInfo ndata = pDoc->m_lstFlawData[i].GetNext(pos); 

			if(pCScanView->m_nAxisSelect == 3
				|| pCScanView->m_nAxisSelect == 4)	//管材 wangbingfu add on 2013-01-09
			{
// 				if (m_nPipeXMode == 0)
// 				{
// 					ndata.m_dY = ndata.m_dR * 360.0f / (PI * fDiameter);
// 				} 
// 				else
// 				{
					ndata.m_dY = ndata.m_dR;
//				}
				
			}
			// Song Chenguang add start on 2013-05-04
			else if (pCScanView->m_nAxisSelect == 5)
			{
				ndata.m_dX = ndata.m_dW;
				ndata.m_dY = ndata.m_dZ;
			}
			// Song Chenguang add end on 2013-05-04

			// songchenguang modify start on 2012-05-22
			// wangbingfu modify stat on 2012-9-13 简化代码并增加高于或低于阈值的判断
			if (m_nSurfaceType == 0 || m_nSurfaceType == 2)	// 矩形、椭圆
			{
				if((ndata.m_dX >= m_fLogicalFromX) && (ndata.m_dX <= m_fLogicalToX)
					&& (ndata.m_dY >= m_fLogicalFromY) && (ndata.m_dY <= m_fLogicalToY))
				{
					;//
				}
				else
				{
					continue;
				}
			}
			else if(m_nSurfaceType == 2)
			{
				BOOL bRet = IsInEllipse(ndata, m_fLogicalFromX, m_fLogicalToX, m_fLogicalFromY, m_fLogicalToY);
				if (!bRet)
					continue;
			}
			else if (m_nSurfaceType == 1)	// 圆
			{
				BOOL bRet = IsInCircle(ndata, m_fLogicalFromX, m_fLogicalToX, m_fLogicalFromY, m_fLogicalToY);
				if (!bRet)
					continue;

			}		
			else if (m_nSurfaceType == 3)	// 多边形
			{				
				BOOL bRet = IsInPolygon(ndata, ptArray, pCScanView->m_nPolyNum + 1);				

				if (!bRet)
					continue;
			}
			
			if(m_nGateType == 0)
			{
				if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] >= pCScanView->m_fGateThreshold[nGateIndexTemp])
				{
					m_PlateDrawCtrl.AddPoint2(ndata.m_dX * m_nZoomScale, ndata.m_dY * m_nZoomScale, 
						ndata.m_pfFeatures[nGateIndexTemp][nFeatureIndexTemp], 2,ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] , 0, 0, 0, 0);

					lCount++;
				}
			}
			else
			{
				if(ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp] < pCScanView->m_fGateThreshold[nGateIndexTemp])
				{
					m_PlateDrawCtrl.AddPoint2(ndata.m_dX * m_nZoomScale, ndata.m_dY * m_nZoomScale, 
						ndata.m_pfFeatures[nGateIndexTemp][nFeatureIndexTemp], 2, ndata.m_pfFeatures[nGateIndexTemp][nMaxAmpIndexTemp], 0, 0, 0, 0);

					lCount++;
				}
			}
		}
		// wangbingfu modify end on 2012-9-13
		// songchenguang modify end on 2012-05-22

		if (lCount > MAX_POINTS_NUM)
		{			
			break;
		}
	}
	if (m_nSurfaceType == 3)
	{
		delete []ptArray;
		ptArray = NULL;
	}

	// 运算
	m_PlateDrawCtrl.go();
	// 画图
	m_nSurfceColorMode = m_nColorMode;
	pCScanView->m_bDrawSurfaceFlag = TRUE;
//	pCScanView->m_MemDC.FillSolidRect(0, 0, pCScanView->m_CScanSize.cx, pCScanView->m_CScanSize.cy, pCScanView->m_cscan_back_color);
	pCScanView->DrawPicture(&pCScanView->m_MemDC);

	pCScanView->DrawSurfaceFlaw(&pCScanView->m_MemDC);
	pCScanView->Invalidate(TRUE);	// 画面伤
	pCScanView->CalcSurfaceColorRate();	// 颜色比率，焊合率
	//消除选中范围
	CPoint point(0, 0);
	pCScanView->m_ptFrom = point;
	pCScanView->m_ptTo = point;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::SurfaceCalc"),"Leave");
}
// 计算面伤数据
void CCScanParamPanel::OnBnClickedButtonSurfaceCalc()
{	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSurfaceCalc"),"Enter");
// machao modify start on 2013-08-14
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
 	CCScanView* pCScanView = NULL;
	if(m_nObjectNum == 1)
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
// 		pCScanView = m_pCScanView;
	else
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
// 		pCScanView = m_pCScanView2;
// machao modify end on 2013-08-14

	if((pCScanView->m_ptFrom == pCScanView->m_ptTo && (m_nSurfaceType >= 0 && m_nSurfaceType <= 2)) ||
		(m_nSurfaceType == 3 && pCScanView->m_nPolyNum == 0))
	{
		if (IDOK == MessageBox(_T("请先在C扫窗口中选择区域"),_T("提示"),MB_OK))
		{
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel:://LOG4CXX_INFO"),"Leave");
			return;
		}
	}

	m_pMessageDlg = new CMessageDlg;
	if(m_pMessageDlg->GetSafeHwnd() == NULL)
	{
		m_pMessageDlg->Create(IDD_DIALOG_MESSAGE, this);
		m_pMessageDlg->SetWindowPos(&wndTop,0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}
	m_pMessageDlg->ShowWindow(SW_SHOW);

	SetTimer(TIMER_WAITT_FOR_FLAW_CALC, 50, NULL);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSurfaceCalc"),"Leave");
	// TODO: Add your control notification handler code here
}

void CCScanParamPanel::OnBnClickedButtonSurfaceDetail()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSurfaceDetail"),"Enter");

/*	CSurfaceDetailDlg dlg;
	dlg.m_pParentWnd = this;
	dlg.DoModal();*/

//	if(m_pSurfaceDeatailDlg->GetSafeHwnd() == NULL)
	if(m_pSurfaceDeatailDlg == NULL)
	{
		m_pSurfaceDeatailDlg = new CSurfaceDetailDlg;
		if (NULL != m_pSurfaceDeatailDlg) //add by wangqianfeng on 20121106
		{
			m_pSurfaceDeatailDlg->m_pParentWnd = this;
			m_pSurfaceDeatailDlg->Create(IDD_DIALOG_SURFACE_DETAIL, this);
			m_pSurfaceDeatailDlg->ShowWindow(SW_SHOW);
		}
		//m_lstSurfaceDetail.RemoveAll();
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSurfaceDetail"),"Leave");
}

CUltrasonicImmersedTestDoc* CCScanParamPanel::GetDocument() const // 非调试版本是内联的
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::GetDocument"),"Enter");

	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUltrasonicImmersedTestDoc)));

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::GetDocument"),"Leave");
	return (CUltrasonicImmersedTestDoc*)m_pDocument;
}
//machao delete start on 2013-07-12
// void CCScanParamPanel::OnBnClickedRadioColor()
// {
// 	// TODO: Add your control notification handler code here
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioColor"),"Enter");
// 
// 	m_nColorMode = 1;
// 	Invalidate(TRUE);
// 
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioColor"),"Leave");
// }

// void CCScanParamPanel::OnBnClickedRadioGray()
// {
// 	// TODO: Add your control notification handler code here
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioGray"),"Enter");
// 
// 	m_nColorMode = 0;
// 	Invalidate(TRUE);
// 
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioGray"),"Leave");
// }
//machao delete on end on 2013-07-12


void CCScanParamPanel::OnBnClickedRadioMaxValue()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioMaxValue"),"Enter");

	m_nValueMode = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioMaxValue"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioMinValue()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioMinValue"),"Enter");

	m_nValueMode = 1;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioMinValue"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioAvgValue()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioAvgValue"),"Enter");

	m_nValueMode = 2;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioAvgValue"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioGate1()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioGate1"),"Enter");

	m_nGateIndex = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioGate1"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioGate2()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioGate2"),"Enter");

	m_nGateIndex = 1;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioGate2"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioGate3()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioGate3"),"Enter");

	m_nGateIndex = 2;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioGate3"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioGate4()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioGate4"),"Enter");

	m_nGateIndex = 3;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioGate4"),"Leave");
}

void CCScanParamPanel::OnBnClickedButtonDrawPicture()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonDrawPicture"),"Enter");
	// WangQianfeng add start on 2012-11-21
	m_pCurCScanView->m_nDefectNum = -1;
	// WangQianfeng add end on 2012-11-21
	m_pCurCScanView->m_vecLine.clear();

	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();	
	
	m_pMessageDlg = new CMessageDlg;
	if(m_pMessageDlg->GetSafeHwnd() == NULL)
	{
		m_pMessageDlg->Create(IDD_DIALOG_MESSAGE, this);
		m_pMessageDlg->SetWindowPos(&wndTop,0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}
	m_pMessageDlg->ShowWindow(SW_SHOW);

	SetTimer(TIMER_WAITT_FOR_PICTURE, 50, NULL);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonDrawPicture"),"Leave");
}

void CCScanParamPanel::OnInitialUpdate()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnInitialUpdate"),"Enter");

	CFormView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	// machao delete start on 2013-08-14
//	m_pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
//	m_pCScanView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
	// machao delete end on 2013-08-14
	m_pAScanView = (CUltrasonicImmersedTestView*)pFrame->m_wndSplitter.GetPane(1,0);

	if(m_nObjectNum == 1) //note
	{
		// machao modify start on 2013-08-14
//		m_pCurCScanView = m_pCScanView;
		m_pCurCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	}
	else
	{
//		m_pCurCScanView = m_pCScanView2;
		m_pCurCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
		// machao modify end on 2013-08-14
	}

/*
	m_sliderBScanOrigin.SetRange(0,40000);
	m_sliderBScanWidth.SetRange(0,40000);

	//初始化B扫滑竿
	m_sliderBScanOrigin.SetPos(int(m_fBScanOrigin*10));
	m_sliderBScanWidth.SetPos(int(m_fBScanWidth*10));
*/
	m_cmbPictureMode.SetCurSel(m_nPictureModeIndex);

	if(m_nObjectNum == 2)
	{
		CWnd *pWnd = GetDlgItem(IDC_CHECK_CSCAN_SYNC);
		pWnd->ShowWindow(FALSE);
	}

	// Song Chenguang add start on 2013-06-28
	// 初始化自选伤列表
	m_lstctrlCustumFlaw.InsertColumn(0, _T("序号"));
	m_lstctrlCustumFlaw.SetColumnWidth(0, 40);
	m_lstctrlCustumFlaw.InsertColumn(1, _T("中心点坐标"));
	m_lstctrlCustumFlaw.SetColumnWidth(1, 100);
	m_lstctrlCustumFlaw.InsertColumn(2, _T("中心点DB"));
	m_lstctrlCustumFlaw.SetColumnWidth(2, 80);
	m_lstctrlCustumFlaw.InsertColumn(3, _T("边缘点DB"));
	m_lstctrlCustumFlaw.SetColumnWidth(3, 80);
	// Song Chenguang add end on 2013-06-28
	DefaultColor1();	// machao add on 2013-09-12
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnInitialUpdate"),"Leave");
}

void CCScanParamPanel::OnBnClickedButtonCscanRealtime()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCscanRealtime"),"Enter");

	UpdateData(TRUE);
	// machao modify start on 2013-08-14
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView *pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	pCScanView->m_fGateThreshold[0] = m_fGate1Threshold;
	pCScanView->m_fGateThreshold[1] = m_fGate2Threshold;
	pCScanView->m_fGateThreshold[2] = m_fGate3Threshold;
	pCScanView->m_fGateThreshold[3] = m_fGate4Threshold;
	pCScanView->m_nColorMode = m_nColorMode;
	pCScanView->m_nValueMode = m_nValueMode;
	pCScanView->m_nGateIndex = m_nGateIndex;
	pCScanView->m_nDrawType = m_nDrawType;
	pCScanView->m_fBoundMin = m_fBoundMin;
	pCScanView->m_fBoundMax = m_fBoundMax;
// 	m_pCScanView->m_fGateThreshold[0] = m_fGate1Threshold;
// 	m_pCScanView->m_fGateThreshold[1] = m_fGate2Threshold;
// 	m_pCScanView->m_fGateThreshold[2] = m_fGate3Threshold;
// 	m_pCScanView->m_fGateThreshold[3] = m_fGate4Threshold;
// 	m_pCScanView->m_nColorMode = m_nColorMode;
// 	m_pCScanView->m_nValueMode = m_nValueMode;
// 	m_pCScanView->m_nGateIndex = m_nGateIndex;
// 	m_pCScanView->m_nDrawType = m_nDrawType;
// 	m_pCScanView->m_fBoundMin = m_fBoundMin;
// 	m_pCScanView->m_fBoundMax = m_fBoundMax;
	// machao modify start on 2013-08-14

/*	//三维分配内存
	float ***pPixel = new float**[nXShuZuLength];
	for(int i = 0; i < nXShuZuLength; i++)
	{
		pPixel[i] = new float*[nYShuZuLentgh];
		//memset(pPixel[i], 0, nYShuZuLentgh*sizeof(float));
		for(int j = 0; j < nYShuZuLentgh; j++)
		{
			pPixel[i][j] = new float[50];
		}
	}	*/

	g_bDrawRealTime = TRUE;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCscanRealtime"),"Leave");
}

void CCScanParamPanel::OnBnClickedButtonStopCscan()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonStopCscan"),"Enter");

	g_bDrawRealTime = FALSE;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonStopCscan"),"Leave");
}
void CCScanParamPanel::Rgb2Hsv(float R, float G, float B, float& H, float& S, float&V)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::Rgb2Hsv"),"Enter");

	// r,g,b values are from 0 to 1
	// h = [0,360], s = [0,1], v = [0,1]
	// if s == 0, then h = -1 (undefined)

	float min, max, delta,tmp;
	tmp = min(R, G);
	min = min( tmp, B );
	tmp = max( R, G);
	max = max(tmp, B );
	V = max; // v

	delta = max - min;

	if( max != 0 )
		S = delta / max; // s
	else
	{

		// r = g = b = 0 // s = 0, v is undefined
		S = 0;
		H = -1;
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::Rgb2Hsv"),"Leave");
		return;
	}
	if( R == max )
		H = ( G - B ) / delta; // between yellow & magenta
	else if( G == max )
		H = 2 + ( B - R ) / delta; // between cyan & yellow
	else
		H = 4 + ( R - G ) / delta; // between magenta & cyan

	H *= 60; // degrees
	if( H < 0 )
		H += 360;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::Rgb2Hsv"),"Leave");
}

void CCScanParamPanel::Hsv2Rgb(float H, float S, float V, float &R, float &G, float &B)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::Hsv2Rgb"),"Enter");

	int i;
	float f, p, q, t;

	if( S == 0 ) 
	{
		// achromatic (grey)
		R = G = B = V;
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::Hsv2Rgb"),"Leave");
		return;
	}

	H /= 60; // sector 0 to 5
	i = static_cast<int>(floor( H ));
	f = H - i; // factorial part of h
	p = V * ( 1 - S );
	q = V * ( 1 - S * f );
	t = V * ( 1 - S * ( 1 - f ) );

	switch( i ) 
	{
	case 0: 
		R = V;
		G = t;
		B = p;
		break;
	case 1:
		R = q;
		G = V;
		B = p;
		break;
	case 2:
		R = p;
		G = V;
		B = t;
		break;
	case 3:
		R = p;
		G = q;
		B = V;
		break;
	case 4:
		R = t;
		G = p;
		B = V;
		break;
	default: // case 5:
		R = V;
		G = p;
		B = q;
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::Hsv2Rgb"),"Leave");
}


int CCScanParamPanel::IsShowProbe()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::IsShowProbe"),"Enter");

	if ( m_pSurfaceDeatailDlg == NULL )
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::IsShowProbe"),"Leave");
		return 0;
	} 
	else
	{
		CButton* i_button = (CButton*)m_pSurfaceDeatailDlg->GetDlgItem(IDC_CHECK_SHOW_PROBE);
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::IsShowProbe"),"Leave");
		return i_button->GetCheck();
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::IsShowProbe"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioFirstThresholdTime()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioFirstThresholdTime"),"Enter");

	m_nFeatureIndex = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioFirstThresholdTime"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioFirstTime()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioFirstTime"),"Enter");

	m_nFeatureIndex =1;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioFirstTime"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioMaxThresholTime()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioMaxThresholTime"),"Enter");

	m_nFeatureIndex = 2;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioMaxThresholTime"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioMaxTime()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioMaxTime"),"Enter");

	m_nFeatureIndex = 3;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioMaxTime"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioFirstAmp()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioFirstAmp"),"Enter");

	m_nFeatureIndex = 4;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioFirstAmp"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioMaxAmp()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioMaxAmp"),"Enter");

	m_nFeatureIndex = 5;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioMaxAmp"),"Leave");
}

void CCScanParamPanel::OnBnClickedButtonSaveCscanPicture()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSaveCscanPicture"),"Enter");

	TCHAR szFilters[] =
		_T("BMP图片 (*.bmp)|*.bmp||");

	CFileDialog dlg(FALSE, _T("bmp"), NULL, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,	szFilters);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();	// machao modify start on 2013-08-14
	CCScanView* pCScanView;
	if(m_nObjectNum == 1)
	{
		// machao modify start on 2013-08-14
//		pCScanView = m_pCScanView;
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2, 0);
	}
	else if(m_nObjectNum == 2)
	{
//		pCScanView = m_pCScanView2;
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3, 0);
		// machao end start on 2013-08-14
	}
	else
	{
		;//
	}
	// A扫视图
	CDC *pAScanDC = m_pAScanView->GetDC();
	// WangQianfeng add start on 2012-11-02
	if ( NULL == pAScanDC )
	{
		MessageBox(_T("CCScanParamPanel::OnBnClickedButtonSaveCscanPicture,pAScanDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSaveCscanPicture"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	RECT rectAScan;
	m_pAScanView->GetClientRect(&rectAScan);
	HWND hAScanWnd = m_pAScanView->GetSafeHwnd();
	// C扫视图
	CDC *pCscanDC = pCScanView->GetDC();
	// WangQianfeng add start on 2012-11-02
	if ( NULL == pCscanDC )
	{
		MessageBox(_T("CCScanParamPanel::OnBnClickedButtonSaveCscanPicture,pCscanDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSaveCscanPicture"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	RECT rectCscan;
	pCScanView->GetClientRect(&rectCscan);
	HWND hCScanWnd = pCScanView->GetSafeHwnd();
	
//	BYTE *lpBitmapBits = NULL;
	HBITMAP directBmp = NULL;
	BITMAPINFO *bmInfo = NULL;
	bmInfo = (BITMAPINFO *)calloc(1, sizeof(BITMAPINFO));
	BITMAPFILEHEADER *bmFileHeader = NULL;
	bmFileHeader = (BITMAPFILEHEADER *)calloc(1, sizeof(BITMAPFILEHEADER));
	DWORD dwBitmapArraySize = 0;
	int nPictureModeIndex = m_cmbPictureMode.GetCurSel();
	if(nPictureModeIndex == 0)		// A+C扫
	{
		dwBitmapArraySize = ScreenCapture2(directBmp, bmInfo, bmFileHeader, 16, (HDC)*pAScanDC, rectAScan, (HDC)*pCscanDC, rectCscan, m_nObjectNum);
	}
	if(nPictureModeIndex == 1)		// C扫
	{
		dwBitmapArraySize = ScreenCapture(directBmp, bmInfo, bmFileHeader, 16, (HDC)*pCscanDC, 
											rectCscan, 1, m_nObjectNum, nPictureModeIndex);
	}
	else if(nPictureModeIndex == 2)		// A扫
	{
		dwBitmapArraySize = ScreenCapture(directBmp, bmInfo, bmFileHeader, 16, (HDC)*pAScanDC, 
											rectAScan, 2, m_nObjectNum, nPictureModeIndex);
	}
	else if(nPictureModeIndex == 3)		// C扫无拉伸
	{
		RECT rectMemDC;
		rectMemDC.left = 0;
		// wangbingfu modify start at 2012-12-5
		rectMemDC.top = ADD_MEM_Y_LENGTH;
		rectMemDC.right = pCScanView->m_nMemXLENGTH - ADD_MEM_X_LENGTH;
		// wangbingfu modify end at 2012-12-5
		rectMemDC.bottom = pCScanView->m_nMemYLENGTH;
		dwBitmapArraySize = ScreenCapture(directBmp, bmInfo, bmFileHeader, 16, (HDC)(pCScanView->m_MemDC),
											rectMemDC, 1, m_nObjectNum, nPictureModeIndex);
	}
	else
	{
		;//
	}
	ReleaseDC(pAScanDC);
	ReleaseDC(pCscanDC);


	if (dlg.DoModal() == IDOK)
	{
		m_strPicturePath = dlg.GetPathName();
		CFile file;
	/*	CFileException e;

		if (file.Open(strPath, CFile::modeReadWrite | CFile::modeCreate), &e)
		{			
	//		file.Write(m_ChannelInfo, sizeof(CChannelInfo)* CHANNEL_NUM);
			file.Close();
		}
		else
		{
			e.ReportError();
		}*/

//		SetTimer(TIMER_SAVE_PICTURE,10,NULL);

		//写进文件
		DWORD dwWritten = 0;
		HANDLE hbmFile = NULL;
		hbmFile = CreateFile(m_strPicturePath,
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if(hbmFile == INVALID_HANDLE_VALUE)
		{
			AfxMessageBox(_T("创建图片失败！"));
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSaveCscanPicture"),"Leave");
			return;
		}

		if(!WriteFile(hbmFile, bmFileHeader, sizeof(BITMAPFILEHEADER),&dwWritten, NULL))
		{
			AfxMessageBox(_T("写图片失败！"));
			CloseHandle(hbmFile);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSaveCscanPicture"),"Leave");
			return;
		}
		if(!WriteFile(hbmFile, &(bmInfo->bmiHeader), sizeof(BITMAPINFOHEADER),&dwWritten, NULL))
		{
			AfxMessageBox(_T("写图片失败！"));
			CloseHandle(hbmFile);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSaveCscanPicture"),"Leave");
			return;
		}
		
		// WangQianfeng add start on 2012-08-22
		if (NULL == lpBitmapBits)
		{
			AfxMessageBox(_T("lpBitmapBits = null"));
			CloseHandle(hbmFile);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSaveCscanPicture"),"Leave");
			return;
		}
		// WangQianfeng add end on 2012-08-22

		if(!WriteFile(hbmFile, lpBitmapBits, dwBitmapArraySize,&dwWritten, NULL))
		{
			AfxMessageBox(_T("写图片失败！"));
			CloseHandle(hbmFile);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSaveCscanPicture"),"Leave");
			return;
		}
		CloseHandle(hbmFile);

		AfxMessageBox(_T("图片保存成功！"));

		pCScanView->Invalidate(TRUE);
	}
	DeleteObject(directBmp);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSaveCscanPicture"),"Leave");
}

BOOL CCScanParamPanel::SavePicByPath( CString p_path, BOOL p_flag )
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::SavePicByPath"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();	// machao modify on 2013-08-14
	CCScanView* pCScanView;
	if(m_nObjectNum == 1)
	{
		// machao modify start on 2013-08-14
//		pCScanView = m_pCScanView;
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2, 0);
	}
	else if(m_nObjectNum == 2)
	{
//		pCScanView = m_pCScanView2;
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3, 0);
	}
	else
	{
		;//
	}
	pCScanView->Invalidate(TRUE);
	m_pAScanView->Invalidate(TRUE);
	// A扫视图
	CDC *pAScanDC = m_pAScanView->GetDC();
	// WangQianfeng add start on 2012-11-02
	if ( NULL == pAScanDC )
	{
		MessageBox(_T("CCScanParamPanel::SavePicByPath,pAScanDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::SavePicByPath"),"Leave");
		return FALSE;
	}
	// WangQianfeng add end on 2012-11-02
	RECT rectAScan;
	m_pAScanView->GetClientRect(&rectAScan);
	HWND hAScanWnd = m_pAScanView->GetSafeHwnd();
	// C扫视图
	CDC *pCscanDC = pCScanView->GetDC();
	// WangQianfeng add start on 2012-11-02
	if ( NULL == pCscanDC )
	{
		MessageBox(_T("CCScanParamPanel::SavePicByPath,pCscanDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::SavePicByPath"),"Leave");
		return FALSE;
	}
	// WangQianfeng add end on 2012-11-02
	/*
	float fXStart = pCScanView->m_dXStart - pCScanView->m_fXMove;
	float fYStart = pCScanView->m_dYStart - pCScanView->m_fYMove;
	int nXStart = fXStart / (pCScanView->m_fZuobiaoXmax - pCScanView->m_fZuobiaoXmin) * pCScanView->m_rectGrid.Width();
	int nYStart = fYStart / (pCScanView->m_fZuobiaoYmax - pCScanView->m_fZuobiaoYmin) * pCScanView->m_rectGrid.Height();
	int nRectWidth  = pCScanView->m_rectCScan.Width();
	int nRectHeight = pCScanView->m_rectCScan.Height();

	float fXLength = fabs(pCScanView->m_dXEnd - pCScanView->m_dXStart);	// 工件X轴长度
	float fYLength = fabs(pCScanView->m_dYEnd - pCScanView->m_dYStart); // 工件Y轴长度
	int nXLength = nRectWidth * pCScanView->m_fScaleFactor;	// 工件X像素长度
	int nYLength = nRectHeight * pCScanView->m_fScaleFactor;	// 工件Y像素长度

	int nMemDCXLength = int(fXLength / pCScanView->m_fGridX);	//MemDC画工件的X像素长度
	int nMemDCYLength = int(fYLength / pCScanView->m_fGridY);	//MemDC画工件的Y像素长度
	pCscanDC->StretchBlt(nXStart, nYStart, nXLength, nYLength, &pCScanView->m_MemDC, 
		0, 0, nMemDCXLength, nMemDCYLength, SRCCOPY);
	*/
	//pCScanView->m_MemDC
	RECT rectCscan;
	pCScanView->GetClientRect(&rectCscan);
	HWND hCScanWnd = pCScanView->GetSafeHwnd();

	//	BYTE *lpBitmapBits = NULL;
	HBITMAP directBmp = NULL;
	BITMAPINFO *bmInfo = NULL;
	bmInfo = (BITMAPINFO *)calloc(1, sizeof(BITMAPINFO));
	BITMAPFILEHEADER *bmFileHeader = NULL;
	bmFileHeader = (BITMAPFILEHEADER *)calloc(1, sizeof(BITMAPFILEHEADER));
	DWORD dwBitmapArraySize = 0;
	int nPictureModeIndex = m_cmbPictureMode.GetCurSel();

	if(p_flag)		// C扫
	{
		//dwBitmapArraySize = SaveCscanWithInfo(directBmp, bmInfo, bmFileHeader, 16, (HDC)*pCscanDC, 
		//	rectCscan, m_nObjectNum, 1);

		dwBitmapArraySize = ScreenCapture(directBmp, bmInfo, bmFileHeader, 16, (HDC)*pCscanDC, 
			rectCscan, 1, m_nObjectNum, 2);//nPictureModeIndex);
	}
	else 		// A扫
	{
		dwBitmapArraySize = ScreenCapture(directBmp, bmInfo, bmFileHeader, 16, (HDC)*pAScanDC, 
			rectAScan, 2, m_nObjectNum, 2);
	}

	ReleaseDC(pAScanDC);
	ReleaseDC(pCscanDC);


		CFile file;


		//写进文件
		DWORD dwWritten = 0;
		HANDLE hbmFile = NULL;
		hbmFile = CreateFile(p_path,
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if(hbmFile == INVALID_HANDLE_VALUE)
		{
			//AfxMessageBox(_T("创建图片失败！"));
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::SavePicByPath"),"Leave");
			return false;
		}

		if(!WriteFile(hbmFile, bmFileHeader, sizeof(BITMAPFILEHEADER),&dwWritten, NULL))
		{
			//AfxMessageBox(_T("写图片失败！"));
			CloseHandle(hbmFile);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::SavePicByPath"),"Leave");
			return false;
		}
		if(!WriteFile(hbmFile, &(bmInfo->bmiHeader), sizeof(BITMAPINFOHEADER),&dwWritten, NULL))
		{
			//AfxMessageBox(_T("写图片失败！"));
			CloseHandle(hbmFile);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::SavePicByPath"),"Leave");
			return false;
		}
		if(!WriteFile(hbmFile, lpBitmapBits, dwBitmapArraySize,&dwWritten, NULL))
		{
			//AfxMessageBox(_T("写图片失败！"));
			CloseHandle(hbmFile);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::SavePicByPath"),"Leave");
			return false;
		}
		CloseHandle(hbmFile);

		//AfxMessageBox(_T("图片保存成功！"));

		pCScanView->Invalidate(TRUE);
	
	DeleteObject(directBmp);
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::SavePicByPath"),"Leave");
	return true;
}

void CCScanParamPanel::OnBnClickedButtonCscanParamSave()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCscanParamSave"),"Enter");
	TCHAR szFilters[] =
		_T("C扫参数文件 (*.cpar)|*.cpar||");

	CFileDialog dlg(FALSE, _T("cpar"), NULL, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,	szFilters);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();

		SaveCScanPara(strPath);
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCscanParamSave"),"Leave");
}

// Song Chenguang add start on 2013-04-26
void CCScanParamPanel::SaveCScanPara(CString strPath)
{
	CSCAN_PARAM_t stCScanParm;
	stCScanParm.nColor = m_nColorMode;
	stCScanParm.nFeatureAmp = m_nAmpFeatureIndex;
	stCScanParm.nFeatureTime = m_nTimeFeatureIndex;
	stCScanParm.nGate1Threshold = static_cast<int>(m_fGate1Threshold);
	stCScanParm.nGate2Threshold = static_cast<int>(m_fGate2Threshold);
	stCScanParm.nGate3Threshold = static_cast<int>(m_fGate3Threshold);
	stCScanParm.nGate4Threshold = static_cast<int>(m_fGate4Threshold);
	stCScanParm.nGateIndex = m_nGateIndex;
	stCScanParm.nDrawMode = m_nDrawType;
	stCScanParm.nValueMode = m_nValueMode;
	//Machao add start on 2013-06-27
	stCScanParm.nImageMode = m_nFeatureIndex;
	stCScanParm.nWorkpieceThickness = static_cast<int>(m_fThick);
	stCScanParm.nMinAcreage = static_cast<int>(m_fMinSurfaceArea);
	stCScanParm.nMinDisplayRange = static_cast<int>(m_fBoundMin);
	stCScanParm.nMaxDisplayRange = static_cast<int>(m_fBoundMax);
	stCScanParm.nTubeBarYMode = m_nPipeXMode;
	stCScanParm.nSizeThreshold = m_nGateType;
	stCScanParm.nProbePosition = m_bDisplayProbe;
	stCScanParm.nCBroomMap = m_bCscanSync;
	stCScanParm.nChangeOrigin = m_bChangeOrigin;
	//Machao add end on 2013-06-27

	CFile file;
	CFileException e;

	if (file.Open(strPath, CFile::modeReadWrite | CFile::modeCreate), &e)
	{			
		file.Write(&stCScanParm, sizeof(CSCAN_PARAM_t));
		file.Close();
	}
	else
	{
		e.ReportError();
	}
}
// Song Chenguang add end on 2013-04-26

void CCScanParamPanel::OnBnClickedButtonCscanParamRead()
{
	// TODO: Add your control notification handler code here
	//CSCAN_PARAM_t stCScanParm;// wangqianfeng delete on 2012-11-03
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCscanParamRead"),"Enter");

	TCHAR szFilters[] =
		_T("C扫参数文件 (*.cpar)|*.cpar||");

	CFileDialog dlg(TRUE, _T("cpar"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();

		ReadCScanPara(strPath, FALSE);
	}
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCscanParamRead"),"Leave");
}

// Song Chenguang add start on 2013-04-26
void CCScanParamPanel::ReadCScanPara(CString strPath, const BOOL& bTaskRun)
{
	CFile file;
	CFileException e;

	if (file.Open(strPath, CFile::modeReadWrite), &e)
	{
		CSCAN_PARAM_t stCScanParm;
		file.Read(&stCScanParm, sizeof(CSCAN_PARAM_t));
		file.Close();

		int nRet = IDNO;
		if (!bTaskRun)
			nRet = MessageBox(_T("当前数据将被覆盖，同意点“是”，否则点“否”。"), _T("提示"), MB_YESNO);
		else
			nRet = IDYES;		

		if (nRet == IDYES)
		{
			m_nColorMode = stCScanParm.nColor;
			m_nAmpFeatureIndex = stCScanParm.nFeatureAmp;
			m_nTimeFeatureIndex = stCScanParm.nFeatureTime;
			m_fGate1Threshold = static_cast<float>(stCScanParm.nGate1Threshold);
			m_fGate2Threshold = static_cast<float>(stCScanParm.nGate2Threshold);
			m_fGate3Threshold = static_cast<float>(stCScanParm.nGate3Threshold);
			m_fGate4Threshold = static_cast<float>(stCScanParm.nGate4Threshold);
			m_nGateIndex = stCScanParm.nGateIndex;
			m_nDrawType = stCScanParm.nDrawMode;
			m_nValueMode = stCScanParm.nValueMode;
			//Machao add start on 2013-06-27
			m_nFeatureIndex = stCScanParm.nImageMode;
			m_fThick = stCScanParm.nWorkpieceThickness;
			m_fMinSurfaceArea = stCScanParm.nMinAcreage;
			m_fBoundMin = stCScanParm.nMinDisplayRange;
			m_fBoundMax = stCScanParm.nMaxDisplayRange;
			m_nPipeXMode = stCScanParm.nTubeBarYMode;
			m_nGateType = stCScanParm.nSizeThreshold;
			m_bDisplayProbe = stCScanParm.nProbePosition;
			m_bCscanSync = stCScanParm.nCBroomMap;
			m_bChangeOrigin = stCScanParm.nChangeOrigin;
			//Machao add end on 2013-06-27

			AllItemSetCheck(FALSE);	//设置所有控件未选中

			//Machao add start on 2013-06-27
			if (m_bCscanSync)
			{
				OneItemSetCheck(IDC_CHECK_CSCAN_SYNC,TRUE);
			}
			else 
			{
				OneItemSetCheck(IDC_CHECK_CSCAN_SYNC,FALSE);
			}
			if (m_bChangeOrigin)
			{
				OneItemSetCheck(IDC_CHECK_ORIGIN,TRUE);
			} 
			else 
			{
				OneItemSetCheck(IDC_CHECK_ORIGIN,FALSE);
			}
			//Machao add end on 2013-06-27
			//颜色控件选中设置
			//machao delete start on 2013-07-12
// 			if(m_nColorMode == 0)
// 			{
// 				OneItemSetCheck(IDC_RADIO_GRAY, TRUE);
// 			}
// 			else
// 			{
// 				OneItemSetCheck(IDC_RADIO_GRAY + 1, TRUE);
// 			}
			//machao delete end on 2013-07-12
			//幅值特征选择选中设置
			if(m_nAmpFeatureIndex == 0)
			{
				OneItemSetCheck(IDC_RADIO_FIRST_AMP, TRUE);
			}
			else
			{
				OneItemSetCheck(IDC_RADIO_FIRST_AMP + 1, TRUE);
			}
			//时间特征选择选中设置
			if(m_nTimeFeatureIndex == 0)
			{
				OneItemSetCheck(IDC_RADIO_MAX_TIME, TRUE);
			}
			else if(m_nTimeFeatureIndex == 1)
			{
				OneItemSetCheck(IDC_RADIO_MAX_TIME + 1, TRUE);
			}
			else if(m_nTimeFeatureIndex == 2)
			{
				OneItemSetCheck(IDC_RADIO_MAX_TIME + 2, TRUE);
			}
			else
			{
				OneItemSetCheck(IDC_RADIO_MAX_TIME + 3, TRUE);
			}
			//闸门选择控件选中设置
			if(m_nGateIndex == 0)
			{
				OneItemSetCheck(IDC_RADIO_SELECT_GATE1, TRUE);
			}
			else if(m_nGateIndex == 1)
			{
				OneItemSetCheck(IDC_RADIO_SELECT_GATE1 + 1, TRUE);
			}
			else if(m_nGateIndex == 2)
			{
				OneItemSetCheck(IDC_RADIO_SELECT_GATE1 + 2, TRUE);
			}
			else
			{
				OneItemSetCheck(IDC_RADIO_SELECT_GATE1 + 3, TRUE);
			}
			//成像模式控件选中设置
			if(m_nDrawType == 0)
			{
				OneItemSetCheck(IDC_RADIO_DEEPDRAW, TRUE);
				m_fMinRule = 0;
				m_fMaxRule = m_fThick;

				CDC *pDC = GetDC();
				// WangQianfeng add start on 2012-11-02
				if ( NULL == pDC )
				{
					MessageBox(_T("CCScanParamPanel::OnBnClickedButtonCscanParamRead,pDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
					LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCscanParamRead"),"Leave");
					return;
				}
				// WangQianfeng add end on 2012-11-02
				DrawRule(pDC,m_fMinRule,m_fMaxRule);
				// WangQianfeng add start on 2012-11-02
				ReleaseDC(pDC);
				// WangQianfeng add end on 2012-11-02
			}
			else
			{
				OneItemSetCheck(IDC_RADIO_RANGEDRAW, TRUE);
				m_fMinRule = m_fBoundMin;
				m_fMaxRule = m_fBoundMax;

				CDC *pDC = GetDC();
				// WangQianfeng add start on 2012-11-02
				if ( NULL == pDC )
				{
					MessageBox(_T("CCScanParamPanel::OnBnClickedButtonCscanParamRead,pDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
					LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCscanParamRead"),"Leave");
					return;
				}
				// WangQianfeng add end on 2012-11-02
				DrawRule(pDC,m_fMinRule,m_fMaxRule);
				// WangQianfeng add start on 2012-11-02
				ReleaseDC(pDC);
				// WangQianfeng add end on 2012-11-02
			}
			//取值模式控件选中设置
			if(m_nValueMode == 0)
			{
				OneItemSetCheck(IDC_RADIO_MAX_VALUE, TRUE);
			}
			else if(m_nValueMode == 1)
			{
				OneItemSetCheck(IDC_RADIO_MIN_VALUE, TRUE);
			}
			else if(m_nValueMode == 2)
			{
				OneItemSetCheck(IDC_RADIO_AVG_VALUE, TRUE);
			}

			UpdateData(FALSE);
		}
	}
	else
	{
		e.ReportError();
	}
}
// Song Chenguang add end on 2013-04-26
// 某个控件选中设置
void CCScanParamPanel::OneItemSetCheck(int nID, BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OneItemSetCheck"),"Enter");

	CButton *pb;
	pb = (CButton *)GetDlgItem(nID);
	pb->SetCheck(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OneItemSetCheck"),"Leave");
}
// 全部颜色控件选中设置
//machao delete start on 2013-07-12
// void CCScanParamPanel::AllColorItemSetCheck(BOOL bFlag)
// {
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllColorItemSetCheck"),"Enter");
// 
// 	CButton *pb;
// 	pb = (CButton *)GetDlgItem(IDC_RADIO_GRAY);
// 	pb->SetCheck(bFlag);
// 	pb = (CButton *)GetDlgItem(IDC_RADIO_COLOR);
// 	pb->SetCheck(bFlag);
// 
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllColorItemSetCheck"),"Leave");
// }
//machao delete end on 2013-07-12

// 全部成像模式控件选中设置
void CCScanParamPanel::AllDrawModeItemSetCheck(BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllDrawModeItemSetCheck"),"Enter");

	CButton *pb;
	pb = (CButton *)GetDlgItem(IDC_RADIO_DEEPDRAW);
	pb->SetCheck(bFlag);
	pb = (CButton *)GetDlgItem(IDC_RADIO_RANGEDRAW);
	pb->SetCheck(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllDrawModeItemSetCheck"),"Leave");
}
// 全部闸门选择控件选中设置
void CCScanParamPanel::AllGateSelectItemSetCheck(BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllGateSelectItemSetCheck"),"Enter");

	CButton *pb;
	pb = (CButton *)GetDlgItem(IDC_RADIO_SELECT_GATE1);
	pb->SetCheck(bFlag);
	pb = (CButton *)GetDlgItem(IDC_RADIO_SELECT_GATE2);
	pb->SetCheck(bFlag);
	pb = (CButton *)GetDlgItem(IDC_RADIO_SELECT_GATE3);
	pb->SetCheck(bFlag);
	pb = (CButton *)GetDlgItem(IDC_RADIO_SELECT_GATE4);
	pb->SetCheck(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllGateSelectItemSetCheck"),"Leave");
}
// 全部取值模式控件选中设置
void CCScanParamPanel::AllValueModeItemSetCheck(BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllValueModeItemSetCheck"),"Enter");

	CButton *pb;
	pb = (CButton *)GetDlgItem(IDC_RADIO_MAX_VALUE);
	pb->SetCheck(bFlag);
	pb = (CButton *)GetDlgItem(IDC_RADIO_MIN_VALUE);
	pb->SetCheck(bFlag);
	pb = (CButton *)GetDlgItem(IDC_RADIO_AVG_VALUE);
	pb->SetCheck(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllValueModeItemSetCheck"),"Leave");
}
// 全部幅值特征选择控件选中设置
void CCScanParamPanel::AllAmpSelectItemSetCheck(BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllAmpSelectItemSetCheck"),"Enter");

	CButton *pb;
	pb = (CButton *)GetDlgItem(IDC_RADIO_MAX_AMP);
	pb->SetCheck(bFlag);
	pb = (CButton *)GetDlgItem(IDC_RADIO_FIRST_AMP);
	pb->SetCheck(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllAmpSelectItemSetCheck"),"Leave");
}
// 全部时间特征选择控件选中设置
void CCScanParamPanel::AllTimeSelectItemSetCheck(BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllTimeSelectItemSetCheck"),"Enter");

	CButton *pb;
	pb = (CButton *)GetDlgItem(IDC_RADIO_MAX_TIME);
	pb->SetCheck(bFlag);
	pb = (CButton *)GetDlgItem(IDC_RADIO_MAX_THRESHOL_TIME);
	pb->SetCheck(bFlag);
	pb = (CButton *)GetDlgItem(IDC_RADIO_FIRST_TIME);
	pb->SetCheck(bFlag);
	pb = (CButton *)GetDlgItem(IDC_RADIO_FIRST_THRESHOLD_TIME);
	pb->SetCheck(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllTimeSelectItemSetCheck"),"Leave");
}
// 全部控件选中设置
void CCScanParamPanel::AllItemSetCheck(BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllItemSetCheck"),"Enter");

//	AllColorItemSetCheck(bFlag);	//machao delete on 2013-07-12
	AllDrawModeItemSetCheck(bFlag);
	AllGateSelectItemSetCheck(bFlag);
	AllValueModeItemSetCheck(bFlag);
	AllAmpSelectItemSetCheck(bFlag);
	AllTimeSelectItemSetCheck(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllItemSetCheck"),"Leave");
}

// 某个控件使能设置
void CCScanParamPanel::OneItemSwitch(int nID, BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OneItemSwitch"),"Enter");

	GetDlgItem(nID)->EnableWindow(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OneItemSwitch"),"Leave");
}

// 全部颜色控件使能设置
//machao delete start on 2013-07-12
// void CCScanParamPanel::AllColorItemSwitch(BOOL bFlag)
// {
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllColorItemSwitch"),"Enter");
// 
// 	GetDlgItem(IDC_RADIO_GRAY)->EnableWindow(bFlag);
// 	GetDlgItem(IDC_RADIO_COLOR)->EnableWindow(bFlag);
// 
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllColorItemSwitch"),"Leave");
// }
//machao delete end on 2013-07-12

// 全部成像模式控件使能设置
void CCScanParamPanel::AllDrawModeItemSwitch(BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllDrawModeItemSwitch"),"Enter");

	GetDlgItem(IDC_RADIO_DEEPDRAW)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_RANGEDRAW)->EnableWindow(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllDrawModeItemSwitch"),"Leave");
}
// 全部闸门选择控件使能设置
void CCScanParamPanel::AllGateSelectItemSwitch(BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllGateSelectItemSwitch"),"Enter");

	GetDlgItem(IDC_RADIO_SELECT_GATE1)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_SELECT_GATE2)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_SELECT_GATE3)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_SELECT_GATE4)->EnableWindow(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllGateSelectItemSwitch"),"Leave");
}
// 全部取值模式控件使能设置
void CCScanParamPanel::AllValueModeItemSwitch(BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllValueModeItemSwitch"),"Enter");

	GetDlgItem(IDC_RADIO_MAX_VALUE)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_MIN_VALUE)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_AVG_VALUE)->EnableWindow(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllValueModeItemSwitch"),"Leave");
}
// 全部幅值特征选择控件使能设置
void CCScanParamPanel::AllAmpSelectItemSwitch(BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllAmpSelectItemSwitch"),"Enter");

	GetDlgItem(IDC_RADIO_MAX_AMP)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_FIRST_AMP)->EnableWindow(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllAmpSelectItemSwitch"),"Leave");
}
// 全部时间特征选择控件使能设置
void CCScanParamPanel::AllTimeSelectItemSwitch(BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllTimeSelectItemSwitch"),"Enter");

	GetDlgItem(IDC_RADIO_MAX_TIME)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_MAX_THRESHOL_TIME)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_FIRST_TIME)->EnableWindow(bFlag);
	GetDlgItem(IDC_RADIO_FIRST_THRESHOLD_TIME)->EnableWindow(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllTimeSelectItemSwitch"),"Leave");
}
// 全部控件使能设置
void CCScanParamPanel::AllItemSwitch(BOOL bFlag)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllItemSwitch"),"Enter");

//	AllColorItemSwitch(bFlag);	//machao delete on 2013-07-12
	AllDrawModeItemSwitch(bFlag);
	AllGateSelectItemSwitch(bFlag);
	AllValueModeItemSwitch(bFlag);
	AllAmpSelectItemSwitch(bFlag);
	AllTimeSelectItemSwitch(bFlag);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::AllItemSwitch"),"Leave");
}
// WangQianfeng delete start on 2013-06-28
void CCScanParamPanel::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnHScroll"),"Enter");

	if (nSBCode == TB_THUMBTRACK && pScrollBar != NULL)
	{
		UpdateData(TRUE);
		
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CBScanView* pBScanView = (CBScanView*)pFrame->m_wndSplitter.GetPane(4,0);
		int nCtrlID = pScrollBar->GetDlgCtrlID();
		if (nCtrlID == IDC_SLIDER_BSCAN_ORIGIN)
		{
			m_fBScanOrigin = m_sliderBScanOrigin.GetPos()/10.0f;
			
			pBScanView->InvalidateRect(pBScanView->m_BScanScaleX);

		}
		else if (nCtrlID == IDC_SLIDER_BSCAN_WIDTH)
		{
			m_fBScanWidth =m_sliderBScanWidth.GetPos()/10.0f;

			pBScanView->InvalidateRect(pBScanView->m_BScanScaleX);
		}
	}
	UpdateData(FALSE);

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnHScroll"),"Leave");
}

void CCScanParamPanel::OnEnChangeEditThick()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnEnChangeEditThick"),"Enter");

	UpdateData(TRUE);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CBScanView* pBScanView = (CBScanView*)pFrame->m_wndSplitter.GetPane(4,0);
	pBScanView->InvalidateRect(pBScanView->m_BScanScaleY);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnEnChangeEditThick"),"Leave");
}
// WangQianfeng delete end on 2013-06-28
void CCScanParamPanel::OnBnClickedCheckSurfaceStatistics()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedCheckSurfaceStatistics"),"Enter");

	m_bSurfaceFlag = !m_bSurfaceFlag;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView *pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	if(m_bSurfaceFlag)
	{
		/*	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if(m_pCScanView->m_bMeasureOn || m_pCScanView2->m_bMeasureOn)
		{
		WPARAM wParam = MAKEWPARAM(ID_MEASURE_ON_OFF, 0);
		pFrame->SendMessage(WM_COMMAND, wParam, 0);
		}*/
//		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();	// machao delete on 2013-08-14
		// machao modify start on 2013-08-13
//		if(m_pCScanView->m_bMeasureOn || m_pCScanView2->m_bMeasureOn)
		if(m_pCurCScanView->m_bMeasureOn)
		// machao modify end on 2013-08-13
		{
			pFrame->ShowControlBar(&pFrame->m_wndCScanDlgBar,!pFrame->m_wndCScanDlgBar.IsWindowVisible(),FALSE,FALSE);
			// machao modify start on 2013-08-13
// 			m_pCScanView->m_bMeasureOn = pFrame->m_wndCScanDlgBar.IsWindowVisible();
// 			m_pCScanView2->m_bMeasureOn = pFrame->m_wndCScanDlgBar.IsWindowVisible();
			m_pCurCScanView->m_bMeasureOn = pFrame->m_wndCScanDlgBar.IsWindowVisible();
			// machao modify end on 2013-08-13
		}
		CPoint point(0, 0);
		// machao modify start on 2013-08-14
		pCScanView->m_ptFrom = point;
		pCScanView->m_ptTo = point;

// 		m_pCScanView->m_ptFrom = point;
// 		m_pCScanView->m_ptTo = point;
		// machao modify start on 2013-08-14
		GetDlgItem(IDC_BUTTON_SURFACE_CALC)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SURFACE_DETAIL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_HANHE_RATE)->EnableWindow(TRUE);

		// Song Chenguang add start on 2013-06-27
		m_pCurCScanView->m_lstCustumSurfaceRegion.RemoveAll();
		// machao add start on 2013-09-17
		m_pCurCScanView->m_lstCustumFlawBufferSize.RemoveAll();
		m_pCurCScanView->m_bEdgePoint = TRUE;
		// machao add end on 2013-09-17
		if (m_pCurCScanView->m_pcCustumFlawBuffer != NULL)
		{
			delete []m_pCurCScanView->m_pcCustumFlawBuffer;
			m_pCurCScanView->m_pcCustumFlawBuffer = NULL;
			m_pCurCScanView->m_lCustumFlawBufferSize = 0;
		}
		m_lstctrlCustumFlaw.DeleteAllItems();
		// Song Chenguang add end on 2013-06-27
	}
	else
	{
		// machao modify start on 2013-08-14
		CClientDC dc(pCScanView);
		// machao modify end on 2013-08-14

		MEASURE_TYPE type = eRECT;
		if (m_nSurfaceType == 0)
		{
			type = eRECT;
		}
		else if (m_nSurfaceType == 1)
		{
			type = eCIRCLE;
		}
		else if (m_nSurfaceType == 2)
		{
			type = eELLIPSE;
		}
		else if (m_nSurfaceType == 3)
		{
			type = ePolygon;
		}
		// machao modify start on 2013-08-14
		pCScanView->InvertRegion(&dc, pCScanView->m_ptFrom, pCScanView->m_ptTo, type);
//		m_pCScanView->InvertRegion(&dc, m_pCScanView->m_ptFrom, m_pCScanView->m_ptTo, type);
		// machao modify end on 2013-08-14
		GetDlgItem(IDC_BUTTON_SURFACE_CALC)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SURFACE_DETAIL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_HANHE_RATE)->EnableWindow(FALSE);
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedCheckSurfaceStatistics"),"Leave");
}
//失去焦点判断当前edit控件输入数据是否正确
void CCScanParamPanel::OnEnKillfocusEditBoundMin()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}
//失去焦点判断当前edit控件输入数据是否正确
void CCScanParamPanel::OnEnKillfocusEditBoundMax()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}
//失去焦点判断当前edit控件输入数据是否正确
void CCScanParamPanel::OnEnKillfocusEditGate1Threshold()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}
//失去焦点判断当前edit控件输入数据是否正确
void CCScanParamPanel::OnEnKillfocusEditGate2Threshold()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}
//失去焦点判断当前edit控件输入数据是否正确
void CCScanParamPanel::OnEnKillfocusEditGate3Threshold()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}
//失去焦点判断当前edit控件输入数据是否正确
void CCScanParamPanel::OnEnKillfocusEditGate4Threshold()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

// Song Chenguang add start on 2014-01-16
// 提取原始A扫文件为某种特定情况下的A扫文件 
//wangbingfu add on 2014-09-10 没办法知道扫查当时哪几个通道有数据
void CCScanParamPanel::ExtractAScanFile()
{
	CString strAscanFilePrimitive = m_filename.Left(m_filename.GetLength() - 4) + _T(".paaf");
	CString strAscanFile = m_filename.Left(m_filename.GetLength() - 4) + _T(".aaf");
	CFile file;
	file.Open(strAscanFilePrimitive, CFile::modeRead);
	CFile file2;
	file2.Open(strAscanFile, CFile::modeReadWrite);
	//wangbingfu add start on 2014-09-10
	CString strAscanFile1 = m_filename.Left(m_filename.GetLength() - 4) + _T("1.aaf");
	CFile file3;
	file3.Open(strAscanFile1, CFile::modeReadWrite);
	//wangbingfu add end on 2014-09-10
	int nAscanHeadSize = sizeof(double)*5+sizeof(float)*2+sizeof(int)*2+sizeof(ULONG) + sizeof(CGateInfo) * GATE_NUM * CHANNEL_NUM; // A扫文件头大小
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMechanicalControlPanel* pPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
	int nValueMode = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nValueMode;
	int nGateIndex = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nGateIndex;
	int nFeatureIndex = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nFeatureIndex;

	// 获取平面序号	
	
	// 获取间距等	
	float fXStep = 0; // X间距
	float fYStep = 0; // Y间
	double dXStart = 0;
	double dYStart = 0;
	int nAxisIndex = 0;
	int nGridXSize = 0;
	ULONG ulGridSize = 0;
	file2.Read(&dXStart, sizeof(double));
	file2.Seek(sizeof(double), CFile::begin);
	file2.Read(&dYStart, sizeof(double));
	file2.Seek(sizeof(double)*5, CFile::begin);
	file2.Read(&nAxisIndex, sizeof(int));
	file2.Seek(sizeof(double)*5 + sizeof(int), CFile::begin);
	file2.Read(&fXStep, sizeof(float));
	file2.Seek(sizeof(double)*5 + sizeof(int) + sizeof(float), CFile::begin);
	file2.Read(&fYStep, sizeof(float));
	file2.Seek(sizeof(double)*5 + sizeof(int) + sizeof(float)*2, CFile::begin);
	file2.Read(&nGridXSize, sizeof(int));
	file2.Seek(sizeof(double)*5 + sizeof(int) + sizeof(float)*2 + sizeof(int), CFile::begin);
	file2.Read(&ulGridSize, sizeof(ULONG));
	
	DWORD dwBytesRemaining = file.GetLength ();

	int i = 0;
	while (dwBytesRemaining) 
	{
		CPicData_L data;
		UINT nBytesRead = file.Read (&data, sizeof (CPicData_L));

		ULONG i_grid_sn;

		if (nAxisIndex == 3 || nAxisIndex == 4)
		{
			// wangbingfu modify start on 2014-11-04
			//i_grid_sn=static_cast<ULONG>(data.m_fR / fYStep + 0.5);	// 改为四舍五入
			i_grid_sn = static_cast<ULONG>(ULONG(fabs(data.m_fX - dXStart) / fXStep + 0.5) + 
				ULONG(data.m_fR / fYStep + 0.5) * nGridXSize);	
			// wangbingfu modify end on 2014-11-04
		} 
		else if (nAxisIndex == 5)
		{
			i_grid_sn = static_cast<ULONG>(ULONG(fabs(data.m_fW - dXStart)/fXStep + 0.5) + 
				ULONG(fabs(data.m_fZ - dYStart) / fYStep + 0.5) * nGridXSize);
		}
		else
		{
			i_grid_sn = static_cast<ULONG>(ULONG(fabs(data.m_fX - dXStart) / fXStep + 0.5) + 
				ULONG(fabs(data.m_fY - dYStart) / fYStep + 0.5) * nGridXSize);						
		}
		// wangbingfu modify start on 2014-09-10 
		int nChnl = 2 * data.m_nCardIndex + data.m_nChannelIndex;
		int nNum = nChnl % 2;

		if(!m_pCurCScanView->m_bGuanLianFlag)
		{
			nNum = 0; // 非关联只有一个A扫文件
		}

		if(nNum == 0)
		{
			if (i_grid_sn < ulGridSize)
			{
				// 读取原值
				file2.Seek(i_grid_sn*sizeof(ULONG) + nAscanHeadSize + sizeof(int), CFile::begin);
				CPicData_L tempData;
				file2.Read(&tempData, sizeof(CPicData_L));

				// 与当前值比较，决定用哪个
				CPicData_L DataToWrite;
				pPanel->CompareTwoAscanData(tempData, data, DataToWrite, nValueMode, nFeatureIndex, nValueMode);

				// 写值
				file2.Seek(i_grid_sn*sizeof(ULONG) + nAscanHeadSize, CFile::begin);
				file2.Write(&i, sizeof(int));
				file2.Write(&DataToWrite, sizeof(CPicData_L));	
			}
		}
		else if(nNum == 1)
		{
			if (i_grid_sn < ulGridSize)
			{
				// 读取原值
				file3.Seek(i_grid_sn*sizeof(ULONG) + nAscanHeadSize + sizeof(int), CFile::begin);
				CPicData_L tempData;
				file3.Read(&tempData, sizeof(CPicData_L));

				// 与当前值比较，决定用哪个
				CPicData_L DataToWrite;
				pPanel->CompareTwoAscanData(tempData, data, DataToWrite, nValueMode, nFeatureIndex, nValueMode);

				// 写值
				file3.Seek(i_grid_sn*sizeof(ULONG) + nAscanHeadSize, CFile::begin);
				file3.Write(&i, sizeof(int));
				file3.Write(&DataToWrite, sizeof(CPicData_L));	
			}
		}
		// wangbingfu modify end on 2014-09-10
		dwBytesRemaining -= nBytesRead;
	}

	file.Close();
	file2.Close();
}
// Song Chenguang add end on 2014-01-16
void CCScanParamPanel::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnTimer"),"Enter");

	KillTimer(nIDEvent);
	if(nIDEvent == TIMER_WAITT_FOR_READ)
	{
		ReadFile();		
	}
	else if(nIDEvent == TIMER_WAITT_FOR_PICTURE)
	{
		// machao modify start on 2013-08-13
// 		if(m_nObjectNum == 1)
// 		{
// 			m_pCScanView->DrawPicture(&m_pCScanView->m_MemDC);
// 			//machao add start on 2013-07-15
// 			m_pCScanView->m_bNoiseReduction = FALSE;
// 			m_pCScanView->m_bImage = TRUE;
// 			//machao add end on 2013-07-15
// 		}
// 		else
// 		{
// 			m_pCScanView2->DrawPicture(&m_pCScanView2->m_MemDC);
// 			//machao add start on 2013-07-15
// 			m_pCScanView2->m_bNoiseReduction = FALSE;
// 			m_pCScanView2->m_bImage = TRUE;
// 			//machao add end on 2013-07-15
// 		}
		// wangbing 注释 on 2014-09-10 根据参数和原始A扫数据文件，重新生成A扫数据文件
		// Song Chenguang add start on 2014-01-16
		// 重新生成A扫提取后的数据文件
		CString strAscanFilePrimitive = m_filename.Left(m_filename.GetLength() - 4) + _T(".paaf");
		if (PathFileExists(strAscanFilePrimitive))
		{
			ExtractAScanFile();
		}
		// Song Chenguang add end on 2014-01-16
		// wangbing 注释 end on 2014-09-10
		m_pCurCScanView->DrawPicture(&m_pCurCScanView->m_MemDC);
		m_pCurCScanView->m_bNoiseReduction = FALSE;
		m_pCurCScanView->m_bImage = TRUE;
		// machao modify end on 2013-08-13
	}
	else if(nIDEvent == TIMER_WAITT_FOR_FLAW_CALC)
	{
		SurfaceCalc();
	}
	else if(nIDEvent == TIMER_WAITT_FOR_HANHE_CALC)
	{		
		if(m_pSHanHeRateDlg == NULL)
		{	
			OnBnClickedButtonHanheCaculate();
			// machao add start on 2013-09-3
			if (m_pCurCScanView->m_nAllCount == 0)
			{
				delete m_pMessageDlg;
				m_pMessageDlg = NULL;
				return;
			}
			// machao add end on 2013-09-3
			m_pSHanHeRateDlg = new CHanHeRateDlg;
			// WangQianfeng add start on 2012-11-02
			if ( NULL == m_pSHanHeRateDlg )
			{
				MessageBox(_T("CCScanParamPanel::OnTimer,m_pSHanHeRateDlg = null"), _T("警告"), MB_OK|MB_ICONERROR);
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::OnTimer"),"Leave");
				return;
			}
			// WangQianfeng add end on 2012-11-02
			m_pSHanHeRateDlg->m_pParentWnd = this;
			m_pSHanHeRateDlg->Create(IDD_DIALOG_HANHE_RATE, this);
			m_pSHanHeRateDlg->ShowWindow(SW_SHOW);
		}
	}
	delete m_pMessageDlg;
	m_pMessageDlg = NULL;

	CFormView::OnTimer(nIDEvent);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnTimer"),"Leave");
}

void CCScanParamPanel::OnBnClickedButtonSaveData()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSaveData"),"Enter");

	// 保存数据文件
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMechanicalControlPanel* pPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
	pPanel->SaveTestResult();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonSaveData"),"Leave");
}
void CCScanParamPanel::SetCScanDrawPicturePara(CCScanView* pCScanView, CCScanFileHeader fileHeader, int nGateNum[], int nFeatureNum)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::SetCScanDrawPicturePara"),"Enter");

	// WangQianfeng add start on 2012-11-02
	if ( NULL == pCScanView )
	{
		MessageBox(_T("CCScanParamPanel::SetCScanDrawPicturePara,pCScanView = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel::SetCScanDrawPicturePara"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		pCScanView->m_nFileGateNum[i] = nGateNum[i];
		for(int j = 0; j < GATE_NUM; j++)
		{
			pCScanView->m_bFileGateFlag[i][j] = fileHeader.m_GateInfo[i][j].m_bOpen;
		}
	}
	pCScanView->m_bFileFeatureFlag[1] = fileHeader.m_bFPFTimeDisplay;
	pCScanView->m_bFileFeatureFlag[2] = fileHeader.m_bFPMTimeDisplay;
	pCScanView->m_bFileFeatureFlag[3] = fileHeader.m_bMPFTimeDisplay;
	pCScanView->m_bFileFeatureFlag[4] = fileHeader.m_bMPMTimeDisplay;;
	pCScanView->m_bFileFeatureFlag[5] = fileHeader.m_bFPMAltitudeDisplay;
	pCScanView->m_bFileFeatureFlag[6] = fileHeader.m_bMPMAltitudeDisplay;
	
	pCScanView->m_fThick = fileHeader.m_fThick;
	pCScanView->m_dXStart = fileHeader.m_dXStart;
	pCScanView->m_dYStart = fileHeader.m_dYStart;
	pCScanView->m_dZStart = fileHeader.m_dZStart;
	pCScanView->m_dXEnd = fileHeader.m_dXEnd;
	pCScanView->m_dYEnd = fileHeader.m_dYEnd;
	pCScanView->m_dZEnd = fileHeader.m_dZEnd;
	pCScanView->m_fGridX = fileHeader.m_fGridX;
	pCScanView->m_fGridY = fileHeader.m_fGridY;
	pCScanView->m_fXMove = fileHeader.m_dXStart;
	pCScanView->m_fYMove = fileHeader.m_dYStart;
	pCScanView->m_fYMoveForZuoBiao = fileHeader.m_dYStart;
	pCScanView->m_nAxisSelect = fileHeader.m_nAxisSelect;
	pCScanView->m_dPipeDiameter = fileHeader.m_dPipeDiameter;

	pCScanView->m_nFileFeatureNum = nFeatureNum;

	pCScanView->m_nProbeNum = fileHeader.m_nProbeNum;
	pCScanView->m_nScanType = fileHeader.m_nScanType;
	pCScanView->m_nFillType = fileHeader.m_nFillType;
	pCScanView->m_nTwoProbeIndex = fileHeader.m_nTwoProbeIndex;
	pCScanView->m_fProbeDistance = fileHeader.m_fProbeDistance;
	pCScanView->m_fChongDie = fileHeader.m_fChongDie;

	m_fGate1Threshold = static_cast<float>(fileHeader.m_GateInfo[0][1].m_fPeakGate);
	m_fGate2Threshold = static_cast<float>(fileHeader.m_GateInfo[0][2].m_fPeakGate);
	m_fGate3Threshold = static_cast<float>(fileHeader.m_GateInfo[0][3].m_fPeakGate);
	m_fGate4Threshold = static_cast<float>(fileHeader.m_GateInfo[0][4].m_fPeakGate);
	
	if(fileHeader.m_nWorkMode == 4 || fileHeader.m_nWorkMode == 5 ||
		fileHeader.m_nWorkMode == 6 ||fileHeader.m_nWorkMode == 7 ||
		fileHeader.m_nWorkMode == 8)
	{
		pCScanView->m_bGuanLianFlag = TRUE;
	}
	else
	{
		pCScanView->m_bGuanLianFlag = FALSE;
	}

	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::SetCScanDrawPicturePara"),"Leave");
}
//Machao modify start on 2013-07-12
//void CCScanParamPanel::OnBnClickedButtonDefaultColor1()
void CCScanParamPanel::OnBnClickedButtonDefaultColor2()
//Machao modify end on 2013-07-12
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonDefaultColor1"),"Enter");
	// machao add start on 2013-11-22
	TCHAR strFolder[MAX_PATH];
	GetWorkingFolder(strFolder, MAX_PATH);
	CString csFilePath = CString(strFolder) + _T("DefaultColor2.prt");//取得配置文件路径
	if (PathFileExists(csFilePath))
	{
		ReadColorFile(csFilePath);
		Invalidate(TRUE);
		return;
	}
	// machao add end on 2013-11-22
// machao modify start on 2013-08-13
	// machao add start on 2013-09-4
#if _HAFEI
	m_nSize = 6;
	m_vnMainColorNum.clear();
	m_plaPalette[0].color = RGB(255,255,0);
	m_plaPalette[0].fMaxPercent = 100;
	m_plaPalette[0].fMinPercent = 80;
	m_plaPalette[1].color = RGB(255, 0, 0);
	m_plaPalette[1].fMaxPercent = 80;
	m_plaPalette[1].fMinPercent = 60;
	m_plaPalette[2].color = RGB(0, 255, 0);
	m_plaPalette[2].fMaxPercent = 60;
	m_plaPalette[2].fMinPercent = 40;
	m_plaPalette[3].color = RGB(0, 0, 255);
	m_plaPalette[3].fMaxPercent = 40;
	m_plaPalette[3].fMinPercent = 20;
	m_plaPalette[4].color = RGB(0, 0, 0);
	m_plaPalette[4].fMaxPercent = 20;
	m_plaPalette[4].fMinPercent = 0;
	m_plaPalette[5].color = RGB(0, 0, 0);
	m_plaPalette[5].fMaxPercent = 0;
	m_plaPalette[5].fMinPercent = -1;
	m_clrBack1 = RGB(180,180,180);
	m_clrBack2 = RGB(75,140,255);
	m_nMainColorNum = 2;
	m_vnMainColorNum.push_back(3);
	m_vnMainColorNum.push_back(3);
	m_pCurCScanView->m_nColorDefine = 0;
	m_nColorDefine = 0;
#else
	// machao add end on 2013-09-4
	m_pCurCScanView->m_clrPlatte[0] = RGB(0,255,0);//
	m_pCurCScanView->m_clrPlatte[1] = RGB(0,240,14);//
	m_pCurCScanView->m_clrPlatte[2] = RGB(0,209,41);//
	m_pCurCScanView->m_clrPlatte[3] = RGB(0,191,68);//
	m_pCurCScanView->m_clrPlatte[4] = RGB(0,160,91);//
	m_pCurCScanView->m_clrPlatte[5] = RGB(0,132,118);//
	m_pCurCScanView->m_clrPlatte[6] = RGB(0,107,135);//
	m_pCurCScanView->m_clrPlatte[7] = RGB(0,74,179);//
	m_pCurCScanView->m_clrPlatte[8] = RGB(0,48,218);//
	m_pCurCScanView->m_clrPlatte[9] = RGB(0,35,213);//
	m_pCurCScanView->m_clrPlatte[10] = RGB(0,0,255);//
	m_pCurCScanView->m_clrPlatte[11] = RGB(20,0,235);//
	m_pCurCScanView->m_clrPlatte[12] = RGB(32,0,210);//
	m_pCurCScanView->m_clrPlatte[13] = RGB(70,0,186);//
	m_pCurCScanView->m_clrPlatte[14] = RGB(97,0,167);//
	m_pCurCScanView->m_clrPlatte[15] = RGB(130,0,124);//
	m_pCurCScanView->m_clrPlatte[16] = RGB(143,0,106);//
	m_pCurCScanView->m_clrPlatte[17] = RGB(170,0,88);//
	m_pCurCScanView->m_clrPlatte[18] = RGB(197,0,46);//
	m_pCurCScanView->m_clrPlatte[19] = RGB(226,0,38);//
	m_pCurCScanView->m_clrPlatte[20] = RGB(255,0,0);//
	//machao add start on 2013-07-31
	m_nMainColorNum = 3;
	m_vnMainColorNum.clear();
	m_vnMainColorNum.push_back(5);
	m_vnMainColorNum.push_back(11);
	m_vnMainColorNum.push_back(5);
	//machao add end on 2013-07-31

	m_nSize = 21;	//machao add on 2013-07-22
	m_pCurCScanView->m_nColorDefine = 1;
// 	if(m_nObjectNum == 1)
// 	{
// 		m_pCScanView->m_clrPlatte[0] = RGB(0,255,0);//
// 		m_pCScanView->m_clrPlatte[1] = RGB(0,240,14);//
// 		m_pCScanView->m_clrPlatte[2] = RGB(0,209,41);//
// 		m_pCScanView->m_clrPlatte[3] = RGB(0,191,68);//
// 		m_pCScanView->m_clrPlatte[4] = RGB(0,160,91);//
// 		m_pCScanView->m_clrPlatte[5] = RGB(0,132,118);//
// 		m_pCScanView->m_clrPlatte[6] = RGB(0,107,135);//
// 		m_pCScanView->m_clrPlatte[7] = RGB(0,74,179);//
// 		m_pCScanView->m_clrPlatte[8] = RGB(0,48,218);//
// 		m_pCScanView->m_clrPlatte[9] = RGB(0,35,213);//
// 		m_pCScanView->m_clrPlatte[10] = RGB(0,0,255);//
// 		m_pCScanView->m_clrPlatte[11] = RGB(20,0,235);//
// 		m_pCScanView->m_clrPlatte[12] = RGB(32,0,210);//
// 		m_pCScanView->m_clrPlatte[13] = RGB(70,0,186);//
// 		m_pCScanView->m_clrPlatte[14] = RGB(97,0,167);//
// 		m_pCScanView->m_clrPlatte[15] = RGB(130,0,124);//
// 		m_pCScanView->m_clrPlatte[16] = RGB(143,0,106);//
// 		m_pCScanView->m_clrPlatte[17] = RGB(170,0,88);//
// 		m_pCScanView->m_clrPlatte[18] = RGB(197,0,46);//
// 		m_pCScanView->m_clrPlatte[19] = RGB(226,0,38);//
// 		m_pCScanView->m_clrPlatte[20] = RGB(255,0,0);//
// 		//machao add start on 2013-07-31
// 		m_nMainColorNum = 3;
// 		m_vnMainColorNum.clear();
// 		m_vnMainColorNum.push_back(5);
// 		m_vnMainColorNum.push_back(11);
// 		m_vnMainColorNum.push_back(5);
// 		//machao add end on 2013-07-31
// 
// 		m_nSize = 21;	//machao add on 2013-07-22
// 		m_pCScanView->m_nColorDefine = 1;
// 	}
// 	else
// 	{
// 		m_pCScanView2->m_clrPlatte[0] = RGB(0,255,0);//
// 		m_pCScanView2->m_clrPlatte[1] = RGB(0,240,14);//
// 		m_pCScanView2->m_clrPlatte[2] = RGB(0,209,41);//
// 		m_pCScanView2->m_clrPlatte[3] = RGB(0,191,68);//
// 		m_pCScanView2->m_clrPlatte[4] = RGB(0,160,91);//
// 		m_pCScanView2->m_clrPlatte[5] = RGB(0,132,118);//
// 		m_pCScanView2->m_clrPlatte[6] = RGB(0,107,135);//
// 		m_pCScanView2->m_clrPlatte[7] = RGB(0,74,179);//
// 		m_pCScanView2->m_clrPlatte[8] = RGB(0,48,218);//
// 		m_pCScanView2->m_clrPlatte[9] = RGB(0,35,213);//
// 		m_pCScanView2->m_clrPlatte[10] = RGB(0,0,255);//
// 		m_pCScanView2->m_clrPlatte[11] = RGB(20,0,235);//
// 		m_pCScanView2->m_clrPlatte[12] = RGB(32,0,210);//
// 		m_pCScanView2->m_clrPlatte[13] = RGB(70,0,186);//
// 		m_pCScanView2->m_clrPlatte[14] = RGB(97,0,167);//
// 		m_pCScanView2->m_clrPlatte[15] = RGB(130,0,124);//
// 		m_pCScanView2->m_clrPlatte[16] = RGB(143,0,106);//
// 		m_pCScanView2->m_clrPlatte[17] = RGB(170,0,88);//
// 		m_pCScanView2->m_clrPlatte[18] = RGB(197,0,46);//
// 		m_pCScanView2->m_clrPlatte[19] = RGB(226,0,38);//
// 		m_pCScanView2->m_clrPlatte[20] = RGB(255,0,0);//
// 		//machao add start on 2013-07-31
// 		m_nMainColorNum = 3;
// 		m_vnMainColorNum.clear();
// 		m_vnMainColorNum.push_back(5);
// 		m_vnMainColorNum.push_back(11);
// 		m_vnMainColorNum.push_back(5);
// 		//machao add start on 2013-07-31
// 
// 		m_nSize = 21;	//machao add on 2013-07-22
// 		m_pCScanView2->m_nColorDefine = 1;
// 	}
// machao modify end on 2013-08-13
	m_nColorDefine = 1;
#endif	// machao add on 2013-09-4
//	m_colorBegin = RGB(0,255,0);
//	m_colorEnd = RGB(255,0,0);
	Invalidate(TRUE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonDefaultColor1"),"Leave");
}
//Machao modify start on 2013-07-12
//void CCScanParamPanel::OnBnClickedButtonDefaultColor2()
void CCScanParamPanel::OnBnClickedButtonDefaultColor1()
//Machao modify end on 2013-07-12
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonDefaultColor2"),"Enter");
	
	DefaultColor1();	// machao add on 2013-09-12
	Invalidate(TRUE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonDefaultColor2"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioVelocityPicture()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioVelocityPicture"),"Enter");

	m_nFeatureIndex = 6;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioVelocityPicture"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioVelocityReducePicture()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioVelocityReducePicture"),"Enter");

	m_nFeatureIndex = 7;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioVelocityReducePicture"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioSzkPicture()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioSzkPicture"),"Enter");

	m_nFeatureIndex = 8;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioSzkPicture"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioMiduPicture()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioMiduPicture"),"Enter");

	m_nFeatureIndex = 9;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioMiduPicture"),"Leave");
}

void CCScanParamPanel::OnBnClickedButtonCaculateVelocity()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCaculateVelocity"),"Enter");
// machao modify start on 2013-08-13
// 	if(m_nObjectNum == 1)
// 	{
// 		m_pCScanView->CaculateVelocity();
// 	}
// 	if(m_nObjectNum == 2)
// 	{
// 		m_pCScanView2->CaculateVelocity();
// 	}
	m_pCurCScanView->CaculateVelocity();
// machao modify end on 2013-08-13

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCaculateVelocity"),"Leave");
}

void CCScanParamPanel::OnBnClickedButtonCalcuateShuaijian()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCalcuateShuaijian"),"Enter");
// machao modify start on 2013-08-13
// 	if(m_nObjectNum == 1)
// 	{
// 		m_pCScanView->CalcuateShuaijian();
// 	}
// 	else
// 	{
// 		m_pCScanView2->CalcuateShuaijian();	
// 	}
	m_pCurCScanView->CalcuateShuaijian();
// machao modify end on 2013-08-13
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCalcuateShuaijian"),"Leave");
}

void CCScanParamPanel::OnBnClickedButtonCaculateSzk()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCaculateSzk"),"Enter");
// machao modify start on 2013-08-13
// 	if(m_nObjectNum == 1)
// 	{	
// 		m_pCScanView->CaculateSzk();
// 	}
// 	else
// 	{
// 		m_pCScanView2->CaculateSzk();
// 	}
	m_pCurCScanView->CaculateSzk();
// machao modify end on 2013-08-13
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCaculateSzk"),"Leave");
}
void CCScanParamPanel::OnBnClickedButtonCaculateMidu()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCaculateMidu"),"Enter");
// machao modify start on 2013-08-13
// 	if(m_nObjectNum == 1)
// 	{	
// 		m_pCScanView->CaculateMidu();
// 	}
// 	else
// 	{
// 		m_pCScanView2->CaculateMidu();
// 	}
	m_pCurCScanView->CaculateMidu();
// machao modify end on 2013-08-13
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonCaculateMidu"),"Leave");
}

void CCScanParamPanel::OnBnClickedButtonHanheRate()
{
	// TODO: Add your control notification handler code here
	
//	if(m_pSHanHeRateDlg->GetSafeHwnd() == NULL)
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonHanheRate"),"Enter");

	UpdateData();//获取界面控件数据
	
	if((m_pCurCScanView->m_ptFrom == m_pCurCScanView->m_ptTo && (m_nSurfaceType >= 0 && m_nSurfaceType <= 2)) ||
		(m_nSurfaceType == 3 && m_pCurCScanView->m_nPolyNum == 0))
	{
		if (IDOK == MessageBox(_T("请先在C扫窗口中选择区域"),_T("提示"),MB_OK))
		{
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamPanel:://LOG4CXX_INFO"),"Leave");
			return;
		}
	}

	m_pMessageDlg = new CMessageDlg;
	if(m_pMessageDlg->GetSafeHwnd() == NULL)
	{
		m_pMessageDlg->Create(IDD_DIALOG_MESSAGE, this);
		m_pMessageDlg->SetWindowPos(&wndTop,0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}
	m_pMessageDlg->ShowWindow(SW_SHOW);

	SetTimer(TIMER_WAITT_FOR_HANHE_CALC, 50, NULL);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonHanheRate"),"Leave");
}

void CCScanParamPanel::OnBnClickedButtonHanheCaculate()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonHanheCaculate"),"Enter");

	bHanHeFlag = TRUE;
// machao modify start on 2013-08-13
// 	if(m_nObjectNum == 1)
// 	{	
// 		m_pCScanView->CalcHanHeColorRate();
// 		if(m_nFeatureIndex != 5)
// 		{
// 			m_nFeatureIndex = 5;
// 			UpdateData(FALSE);
// 		}
// 		m_pCScanView->DrawPicture(&m_pCScanView->m_MemDC);
// 	}
// 	else
// 	{
// 		m_pCScanView2->CalcHanHeColorRate();
// 		if(m_nFeatureIndex != 5)
// 		{
// 			m_nFeatureIndex = 5;
// 			UpdateData(FALSE);
// 		}
// 		m_pCScanView2->DrawPicture(&m_pCScanView2->m_MemDC);
// 	}
	m_pCurCScanView->CalcHanHeColorRate();
	if(m_nFeatureIndex != 5)
	{
		m_nFeatureIndex = 5;
		UpdateData(FALSE);
	}
	m_pCurCScanView->DrawPicture(&m_pCurCScanView->m_MemDC);
// machao modify end on 2013-08-13
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonHanheCaculate"),"Leave");
}

void CCScanParamPanel::OnBnClickedCheckCscanSync()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedCheckCscanSync"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView *pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CCScanView *pCScanView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
	m_bCscanSync = !m_bCscanSync;
	if(m_bCscanSync)
	{
		// machao modify start on 2013-08-14
		if (m_nObjectNum == 1)
		{
	 		CRect rect;
	 		CSize csize;
	 		pCScanView2->GetClientRect(&rect);
	 		csize.cx =  rect.right - rect.left;
	 		csize.cy =  rect.bottom - rect.top;
	 		csize.cx = static_cast<LONG>(csize.cx * m_pCurCScanView->m_fScaleFactor);
	 		csize.cy = static_cast<LONG>(csize.cy * m_pCurCScanView->m_fScaleFactor);
	 
	 		pCScanView2->m_fScaleFactor = m_pCurCScanView->m_fScaleFactor;
	 		pCScanView2->SetScrollSizes(MM_TEXT, csize);
	 		pCScanView2->m_displaySize =m_pCurCScanView-> m_displaySize;
	 
	 		int curpos = m_pCurCScanView->GetScrollPos(SB_HORZ);
	 		pCScanView2->SetScrollPos(SB_HORZ, curpos, TRUE);
	 		pCScanView2->m_fXMove = m_pCurCScanView->m_fXMove;
	 		
	 		curpos = m_pCurCScanView->GetScrollPos(SB_VERT);
	 		pCScanView2->SetScrollPos(SB_VERT, curpos, TRUE);
	 		pCScanView2->m_fYMove = m_pCurCScanView->m_fYMove;
	 		pCScanView2->m_fYMoveForZuoBiao = m_pCurCScanView->m_fYMoveForZuoBiao;
	 
	 		pCScanView2->Invalidate(TRUE);
		}
		else
		{
			CRect rect;
			CSize csize;
			pCScanView->GetClientRect(&rect);
			csize.cx =  rect.right - rect.left;
			csize.cy =  rect.bottom - rect.top;
			csize.cx = static_cast<LONG>(csize.cx * m_pCurCScanView->m_fScaleFactor);
			csize.cy = static_cast<LONG>(csize.cy * m_pCurCScanView->m_fScaleFactor);

			pCScanView->m_fScaleFactor = m_pCurCScanView->m_fScaleFactor;
			pCScanView->SetScrollSizes(MM_TEXT, csize);
			pCScanView->m_displaySize =m_pCurCScanView-> m_displaySize;

			int curpos = m_pCurCScanView->GetScrollPos(SB_HORZ);
			pCScanView->SetScrollPos(SB_HORZ, curpos, TRUE);
			pCScanView->m_fXMove = m_pCurCScanView->m_fXMove;

			curpos = m_pCurCScanView->GetScrollPos(SB_VERT);
			pCScanView->SetScrollPos(SB_VERT, curpos, TRUE);
			pCScanView->m_fYMove = m_pCurCScanView->m_fYMove;
			pCScanView->m_fYMoveForZuoBiao = m_pCurCScanView->m_fYMoveForZuoBiao;

			pCScanView->Invalidate(TRUE);
		}
// 		CRect rect;
// 		CSize csize;
// 		m_pCScanView2->GetClientRect(&rect);
// 		csize.cx =  rect.right - rect.left;
// 		csize.cy =  rect.bottom - rect.top;
// 		csize.cx = static_cast<LONG>(csize.cx * m_pCScanView->m_fScaleFactor);
// 		csize.cy = static_cast<LONG>(csize.cy * m_pCScanView->m_fScaleFactor);
// 
// 		m_pCScanView2->m_fScaleFactor = m_pCScanView->m_fScaleFactor;
// 		m_pCScanView2->SetScrollSizes(MM_TEXT, csize);
// 		m_pCScanView2->m_displaySize =m_pCScanView-> m_displaySize;
// 
// 		int curpos = m_pCScanView->GetScrollPos(SB_HORZ);
// 		m_pCScanView2->SetScrollPos(SB_HORZ, curpos, TRUE);
// 		m_pCScanView2->m_fXMove = m_pCScanView->m_fXMove;
// 		
// 		curpos = m_pCScanView->GetScrollPos(SB_VERT);
// 		m_pCScanView2->SetScrollPos(SB_VERT, curpos, TRUE);
// 		m_pCScanView2->m_fYMove = m_pCScanView->m_fYMove;
// 		m_pCScanView2->m_fYMoveForZuoBiao = m_pCScanView->m_fYMoveForZuoBiao;
// 
// 		m_pCScanView2->Invalidate(TRUE);
		//machao modify end on 2013-08-14
	}
	else
	{
		pCScanView->Invalidate(TRUE);
		pCScanView2->Invalidate(TRUE);
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedCheckCscanSync"),"Leave");
}

BOOL CCScanParamPanel::IsInEllipse(const CFeatureInfo& feature, const float& fXmin, const float& fXMax,
				 const float& fYMin, const float& fYMax)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::IsInEllipse"),"Enter");

	BOOL bRet = FALSE;

	float fXCenter = (fXMax + fXmin) / 2.0f;
	float fYCenter = (fYMax + fYMin) / 2.0f;

	float fA = (fXMax - fXmin) / 2.0f;
	float fB = (fYMax - fYMin) / 2.0f;
	// machao modify start on 2013-09-4
//	float fValue = static_cast<float>(pow((feature.m_dX - fXCenter)/fA, 2) + pow((feature.m_dY - fYCenter)/fB, 2));
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int nAxisIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();
	float fValue = 0.0f;
	switch(nAxisIndex)
	{
	case 0:
		fValue = static_cast<float>(pow((feature.m_dX - fXCenter)/fA, 2) + pow((feature.m_dY - fYCenter)/fB, 2));
		break;
	case 1:
		fValue = static_cast<float>(pow((feature.m_dX - fXCenter)/fA, 2) + pow((feature.m_dZ - fYCenter)/fB, 2));
		break;
	case 2:
		fValue = static_cast<float>(pow((feature.m_dY - fXCenter)/fA, 2) + pow((feature.m_dZ - fYCenter)/fB, 2));
		break;
	case 3:
	case 4:
		fValue = static_cast<float>(pow((feature.m_dX - fXCenter)/fA, 2) + pow((feature.m_dR - fYCenter)/fB, 2));
		break;
	case 5:
		fValue = static_cast<float>(pow((feature.m_dW - fXCenter)/fA, 2) + pow((feature.m_dZ - fYCenter)/fB, 2));
		break;
	}
	// machao modify end on 2013-09-4

	if (fValue < 1)
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::IsInEllipse"),"Leave");
		return TRUE;
	}
	else
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::IsInEllipse"),"Leave");
		return FALSE;
	}
}

BOOL CCScanParamPanel::IsInCircle(const CFeatureInfo& feature, const float& fXmin, const float& fXMax,
				const float& fYMin, const float& fYMax)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::IsInCircle"),"Enter");

	float fCenterX = (fXmin + fXMax) / 2.0f;
	float fCenterY = (fYMin + fYMax) / 2.0f;
	float fRadius = (fXMax - fXmin) / 2.0f;
	
	// machao modify start on 2013-09-3
//	float fDistance = static_cast<float>(sqrt((feature.m_dX - fCenterX)*(feature.m_dX - fCenterX) + (feature.m_dY - fCenterY)*(feature.m_dY - fCenterY)));
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int nAxisIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();
	float fDistance = 0.0f;
	switch(nAxisIndex)
	{
	case 0:
		fDistance = static_cast<float>(sqrt((feature.m_dX - fCenterX)*(feature.m_dX - fCenterX) + (feature.m_dY - fCenterY)*(feature.m_dY - fCenterY)));
		break;
	case 1:
		fDistance = static_cast<float>(sqrt((feature.m_dX - fCenterX)*(feature.m_dX - fCenterX) + (feature.m_dZ - fCenterY)*(feature.m_dZ - fCenterY)));
		break;
	case 2:
		fDistance = static_cast<float>(sqrt((feature.m_dY - fCenterX)*(feature.m_dY - fCenterX) + (feature.m_dZ - fCenterY)*(feature.m_dZ - fCenterY)));
		break;
	case 3:
	case 4:
		fDistance = static_cast<float>(sqrt((feature.m_dX - fCenterX)*(feature.m_dX - fCenterX) + (feature.m_dR - fCenterY)*(feature.m_dR - fCenterY)));
		break;
	case 5:
		fDistance = static_cast<float>(sqrt((feature.m_dW - fCenterX)*(feature.m_dW - fCenterX) + (feature.m_dZ - fCenterY)*(feature.m_dZ - fCenterY)));
		break;
	}
	// machao modify end on 2013-09-3

	if (fDistance < fRadius)
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::IsInCircle"),"Leave");
	    return TRUE;
	}
	else
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::IsInCircle"),"Leave");
		return FALSE;
	}
}

void CCScanParamPanel::CleanSurfaceDisplay()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::CleanSurfaceDisplay"),"Enter");

//	if (!m_bSurfaceFlag)
//		return;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd(); //machao add on 2013-08-14
	CCScanView * pView = NULL;
	if (m_nObjectNum == 1)
	{
		// machao modify start on 2013-08-14
		pView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2, 0);
//		pView = m_pCScanView;
	}
	else
		pView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3, 0);
//		pView = m_pCScanView2;
		// machao modify end on 2013-08-14

	pView->m_ptFrom = pView->m_ptTo;

	pView->m_nPolyNum = -1;    //按下右键，多边形顶点重新获取  yachang.huang add start on 2012-6-4
	pView->m_fPolygonArea = 0.0;  //按下右键，多边形面积清零  yachang.huang add start on 2012-6-4
	for (int i=0; i<POLY_VERTEX_NUM; i++)
	{
		pView->m_Poly[i] = CPoint(0, 0);
	}

	pView->Invalidate(TRUE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::CleanSurfaceDisplay"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioRect()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioRect"),"Enter");

	CleanSurfaceDisplay();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioRect"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioCircle()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioCircle"),"Enter");

	CleanSurfaceDisplay();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioCircle"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioEllipse()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioEllipse"),"Enter");

	CleanSurfaceDisplay();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioEllipse"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioPolygon()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioPolygon"),"Enter");

	CleanSurfaceDisplay();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioPolygon"),"Leave");
}

BOOL CCScanParamPanel::IsInPolygon(const CFeatureInfo& feature, LOGICAL_POINT_t* ptPolygon, const int& nCount)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::IsInPolygon"),"Enter");

	int nCross = 0; 

	for (int i = 0; i < nCount; i++) 
	{
		LOGICAL_POINT_t p1;
		p1.x = ptPolygon[i].x;
		p1.y = ptPolygon[i].y;

		LOGICAL_POINT_t p2;
		p2.x = ptPolygon[(i + 1) % nCount].x;
		p2.y = ptPolygon[(i + 1) % nCount].y;

		// 求解 y=p.y 与 p1p2 的交点 

		// machao modify start on 2013-09-4
// 		if ( fabs(p1.y - p2.y) < 1E-3 ) // p1p2 与 y=p0.y平行 
// 			continue; 
// 
// 		if ( feature.m_dY < min(p1.y, p2.y) ) // 交点在p1p2延长线上 
// 			continue; 
// 
// 		if ( feature.m_dY >= max(p1.y, p2.y) ) // 交点在p1p2延长线上 
// 			continue; 
// 
// 		// 求交点的 X 坐标 -------------------------------------------------------------- 
// 		double x = (double)(feature.m_dY - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x; 
// 		if ( x > feature.m_dX ) 
// 			nCross++; // 只统计单边交点 
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		int nAxisIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();
		double x = 0;
		if (nAxisIndex == 0)
		{
			if ( fabs(p1.y - p2.y) < 1E-3 ) // p1p2 与 y=p0.y平行 
				continue; 

			if ( feature.m_dY < min(p1.y, p2.y) ) // 交点在p1p2延长线上 
				continue; 

			if ( feature.m_dY >= max(p1.y, p2.y) ) // 交点在p1p2延长线上 
				continue; 

			// 求交点的 X 坐标 -------------------------------------------------------------- 
			x = (double)(feature.m_dY - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x; 

			if ( x > feature.m_dX ) 
				nCross++; // 只统计单边交点 
		}
		else if (nAxisIndex == 1)
		{
			if ( fabs(p1.y - p2.y) < 1E-3 ) // p1p2 与 y=p0.y平行 
				continue; 

			if ( feature.m_dZ < min(p1.y, p2.y) ) // 交点在p1p2延长线上 
				continue; 

			if ( feature.m_dZ >= max(p1.y, p2.y) ) // 交点在p1p2延长线上 
				continue; 

			// 求交点的 X 坐标 -------------------------------------------------------------- 
			x = (double)(feature.m_dZ - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x; 

			if ( x > feature.m_dX ) 
				nCross++; // 只统计单边交点 
		}
		else if (nAxisIndex == 2)
		{
			if ( fabs(p1.y - p2.y) < 1E-3 ) // p1p2 与 y=p0.y平行 
				continue; 

			if ( feature.m_dZ < min(p1.y, p2.y) ) // 交点在p1p2延长线上 
				continue; 

			if ( feature.m_dZ >= max(p1.y, p2.y) ) // 交点在p1p2延长线上 
				continue; 

			// 求交点的 X 坐标 -------------------------------------------------------------- 
			x = (double)(feature.m_dZ - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x; 

			if ( x > feature.m_dY ) 
				nCross++; // 只统计单边交点 
		}
		else if (nAxisIndex == 3 || nAxisIndex == 4)
		{
			if ( fabs(p1.y - p2.y) < 1E-3 ) // p1p2 与 y=p0.y平行 
				continue; 

			if ( feature.m_dR < min(p1.y, p2.y) ) // 交点在p1p2延长线上 
				continue; 

			if ( feature.m_dR >= max(p1.y, p2.y) ) // 交点在p1p2延长线上 
				continue; 

			// 求交点的 X 坐标 -------------------------------------------------------------- 
			x = (double)(feature.m_dR - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x; 

			if ( x > feature.m_dX ) 
				nCross++; // 只统计单边交点 
		}
		else if (nAxisIndex == 5)
		{
			if ( fabs(p1.y - p2.y) < 1E-3 ) // p1p2 与 y=p0.y平行 
				continue; 

			if ( feature.m_dZ < min(p1.y, p2.y) ) // 交点在p1p2延长线上 
				continue; 

			if ( feature.m_dZ >= max(p1.y, p2.y) ) // 交点在p1p2延长线上 
				continue; 

			// 求交点的 X 坐标 -------------------------------------------------------------- 
			x = (double)(feature.m_dZ - p1.y) * (double)(p2.x - p1.x) / (double)(p2.y - p1.y) + p1.x; 

			if ( x > feature.m_dW ) 
				nCross++; // 只统计单边交点 
		}
		// machao modify end on 2013-09-4
	} 

	// 单边交点为偶数，点在多边形之外 --- 

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::IsInPolygon"),"Leave");
	return (nCross % 2 == 1); 
}

void CCScanParamPanel::OnBnClickedCheckDisplayProbe()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedCheckDisplayProbe"),"Enter");
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();	// machao modify on 2013-08-14
	CCScanView* pCScanView;
	if(m_nObjectNum == 1)
	{
		// machao modify start on 2013-08-14
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2, 0);
//		pCScanView = m_pCScanView;
	}
	else if(m_nObjectNum == 2)
	{
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3, 0);
//		pCScanView = m_pCScanView2;
		// machao modify end on 2013-08-14
	}
	else
	{
		;//
	}
	pCScanView->Invalidate(TRUE);
	pCScanView->m_ptProbeCurPos.x = -1;
	pCScanView->m_ptProbeCurPos.y = -1;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedCheckDisplayProbe"),"Leave");
}

void CCScanParamPanel::OnEnChangeEditBoundMin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	Invalidate(TRUE);
}

void CCScanParamPanel::OnEnChangeEditBoundMax()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	Invalidate(TRUE);
}

void CCScanParamPanel::OnBnClickedButtonOrigin()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonOrigin"),"Enter");

	COriginDlg dlg;
	dlg.m_pParentWnd = this;
	dlg.DoModal();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedButtonOrigin"),"Leave");
}


void CCScanParamPanel::OnBnClickedCheckZhushi()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedCheckZhushi"),"Enter");

	UpdateData(TRUE);
	if(m_bNoteFlag)
	{
		Note_ReadFile(m_pCurCScanView->m_strNotePath);
		// rem by Song Chenguang on 2013-07-18 避免GDI对象泄漏
//		m_pCurCScanView->m_MemDC.RestoreDC(m_pCurCScanView->m_nSavedDC); // note
//		m_pCurCScanView->m_nSavedDC = m_pCurCScanView->m_MemDC.SaveDC();
		Note_DrawAllNote(&m_pCurCScanView->m_MemDC, FALSE, m_nObjectNum);
	}
	else
	{
		m_pCurCScanView->m_MemDC.SetViewportOrg(0, 0);
		m_pCurCScanView->m_MemDC.FillSolidRect(0, 0, m_pCurCScanView->m_CScanSize.cx, m_pCurCScanView->m_CScanSize.cy, m_pCurCScanView->m_cscan_back_color);
		// rem by Song Chenguang on 2013-07-18 避免GDI对象泄漏
//		m_pCurCScanView->m_MemDC.RestoreDC(m_pCurCScanView->m_nSavedDC); // note
//		m_pCurCScanView->m_nSavedDC = m_pCurCScanView->m_MemDC.SaveDC();
		for(int i = 0; i < m_pCurCScanView->m_nMemXLENGTH; i++)
		{
			for(int j = 0; j < m_pCurCScanView->m_nMemYLENGTH; j++)
			{
				if(m_pCurCScanView->m_pPixelCount[i][j] != 0)
				{
					if(m_nGateType == 0)
					{
						if(m_pCurCScanView->m_pPixel[i][j] >= m_pCurCScanView->m_fGateThreshold[m_nGateIndex])
						{
							m_pCurCScanView->draw2DC(&m_pCurCScanView->m_MemDC, m_pCurCScanView->m_pPixel[i][j], i, j, 0, 0);
						}
						else
						{
							m_pCurCScanView->draw2DC(&m_pCurCScanView->m_MemDC, -1, i, j, 0, 0);
						}
					}
					else
					{
						if(m_pCurCScanView->m_pPixel[i][j] < m_pCurCScanView->m_fGateThreshold[m_nGateIndex])
						{
							m_pCurCScanView->draw2DC(&m_pCurCScanView->m_MemDC, m_pCurCScanView->m_pPixel[i][j], i, j, 0, 0);
						}
						else
						{
							m_pCurCScanView->draw2DC(&m_pCurCScanView->m_MemDC, -1, i, j, 0, 0);
						}
					}
				}		
			}
		}
	}
	m_pCurCScanView->Invalidate(TRUE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedCheckZhushi"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioZhushiRect()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioZhushiRect"),"Enter");

	CleanSurfaceDisplay();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioZhushiRect"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioZhushiCircle()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioZhushiCircle"),"Enter");

	CleanSurfaceDisplay();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioZhushiCircle"),"Leave");
}

void CCScanParamPanel::OnBnClickedRadioZhushiEllipse()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioZhushiEllipse"),"Enter");

	CleanSurfaceDisplay();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamPanel::OnBnClickedRadioZhushiEllipse"),"Leave");
}
// WangQianfeng add start on 2012-11-21
void CCScanParamPanel::OnBnClickedDefect()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CDefectDlg dlg;
	dlg.m_dMinArea = m_fMinSurfaceArea;
	if(IDOK == dlg.DoModal())
	{
		//m_bDefectFlag = TRUE;	
	}
}
// WangQianfeng add end on 2012-11-21

// wangbingfu add start on 2013-01-09
void CCScanParamPanel::ReadPipeIni(const CString& csFilePath)
{
	// 	TCHAR strFolder[MAX_PATH];
	// 	GetWorkingFolder(strFolder, MAX_PATH);
	// 	CString csFilePath = CString(strFolder) + _T("PipeSet.ini");//取得配置文件路径

	CString strSection;
	CString strKey;
	// machao add start on 2013-08-14
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView *pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CCScanView *pCScanView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
	// machao add end on 2013-08-14
	int nValue = 0;
	strSection = _T("OtherParams");
	//	strKey = _T("AxisSelect");
	//	int nAxisSelect = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);

	//	if(nAxisSelect == 4)
	{
		//		m_nAxisSelect = nAxisSelect;
		strKey = _T("Angle");
		int nAngle = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
		// machao modify start on 2013-08-14
		pCScanView->m_fPipeProbeAngle = (float)nAngle;
		pCScanView2->m_fPipeProbeAngle = (float)nAngle;

// 		m_pCScanView->m_fPipeProbeAngle = (float)nAngle;
// 		m_pCScanView2->m_fPipeProbeAngle = (float)nAngle;
		// machao modify end on 2013-08-14

		int nPipeChannelSelect[CHANNEL_NUM];
		memset(nPipeChannelSelect, 0, sizeof(nPipeChannelSelect));

		// 0=无,1=A测厚,2=A分层,3=A测厚+分层,4=轴向B1,5=轴向B2,6=径向C1,7=径向C2
		strSection = _T("GroupSelect");
		// machao modify start on 2013-09-25
		CString str[8] = {_T("G11"),_T("G12"),_T("G21"),_T("G22"),_T("G31"),_T("G32"),_T("G41"),_T("G42")};
		for (int i = 0; i < 8; i++)
		{
			strKey = str[i];
			nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
			nPipeChannelSelect[i] = nValue;
		}
		// machao moidfy end on 2013-09-25

		for(int i = 0; i < CHANNEL_NUM; i++)
		{
			// machao modify start on 2013-08-14
			pCScanView->m_nPipeChannelSelect[i] = nPipeChannelSelect[i];
			pCScanView2->m_nPipeChannelSelect[i] = nPipeChannelSelect[i];

// 			m_pCScanView->m_nPipeChannelSelect[i] = nPipeChannelSelect[i];
// 			m_pCScanView2->m_nPipeChannelSelect[i] = nPipeChannelSelect[i];
			// machao modify end on 2013-08-14
		}

		// 0=通道1,1=通道2,2=通道3,3=通道4,4=通道5,5=通道6,6=通道7,7=通道8
		// machao modify start on 2013-08-14
		strSection = _T("CScanSelect");
		strKey = _T("C1");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
//		m_pCScanView->m_nCScanSelect = nValue - 1;
		pCScanView->m_nCScanSelect = nValue - 1;
		strKey = _T("C2");
		nValue = GetPrivateProfileInt(strSection, strKey, NULL, csFilePath);
//		m_pCScanView2->m_nCScanSelect = nValue - 1;
		pCScanView2->m_nCScanSelect = nValue - 1;
		// machao modify end on 2013-08-14
	}
}

void CCScanParamPanel::WritePipeIni(const CString& csFilePath)
{
	CString strKey;
	CString strValue;
	CString cstr = _T("");
	CString strSection = _T("");
	// machao add start on 2013-08-14
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView *pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CCScanView *pCScanView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);

	// 	TCHAR strFolder[MAX_PATH];
	// 	GetWorkingFolder(strFolder, MAX_PATH);
	// 	CString csFilePath = CString(strFolder) + _T("PipeSet.ini");//取得配置文件路径
	//如果文件为只读的，将只读属性去掉
	SetFileAttributes(csFilePath,GetFileAttributes(csFilePath) & ~FILE_ATTRIBUTE_READONLY);

	strSection = _T("OtherParams");
	// 	strKey = _T("AxisSelect");
	// 	strValue = _T("");
	// 	strValue.Format(_T("%d"),m_nAxisSelect);
	// 	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	// machao modify start on 2013-08-14
	strKey = _T("Angle");
	strValue = _T("");
//	strValue.Format(_T("%d"),m_pCScanView->m_fPipeProbeAngle);
	strValue.Format(_T("%d"),pCScanView->m_fPipeProbeAngle);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);

	strSection = _T("GroupSelect");
	// machao modify start on 2013-09-25
	CString str[8] = {_T("G11"),_T("G12"),_T("G21"),_T("G22"),_T("G31"),_T("G32"),_T("G41"),_T("G42")};
	for (int i = 0; i < 8; i++)
	{
		strKey = str[i];
		strValue = _T("");
		//	strValue.Format(_T("%d"), m_pCScanView->m_nPipeChannelSelect[0]);
		strValue.Format(_T("%d"), pCScanView->m_nPipeChannelSelect[i]);
		WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	}
	// machao modify end on 2013-09-25

	strSection = _T("CScanSelect");
	strKey = _T("C1");
	strValue = _T("");
//	strValue.Format(_T("%d"), m_pCScanView->m_nCScanSelect + 1);
	strValue.Format(_T("%d"), pCScanView->m_nCScanSelect + 1);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	strSection = _T("CScanSelect");
	strKey = _T("C2");
	strValue = _T("");
//	strValue.Format(_T("%d"), m_pCScanView2->m_nCScanSelect + 1);
	strValue.Format(_T("%d"), pCScanView2->m_nCScanSelect + 1);
	WritePrivateProfileString(strSection, strKey, strValue, csFilePath);
	// machao modify end on 2013-08-14
}
// wangbingfu add end on 2013-01-09
//Machao add start on 2013-06-27
void CCScanParamPanel::OnBnClickedCheckOrigin()
{
	m_bChangeOrigin = !m_bChangeOrigin;
}
void CCScanParamPanel::OnBnRadioXmodeAngle()
{
	m_nPipeXMode = 0;
	OnBnClickedButtonDrawPicture();		// machao add on 2013-11-27
}
void CCScanParamPanel::OnBnRadioXmodePerimeter()
{
	m_nPipeXMode = 1;
	OnBnClickedButtonDrawPicture();		// machao add on 2013-11-27
}
void CCScanParamPanel::OnBnRadioGateAbove()
{
	m_nGateType = 0;
}
void CCScanParamPanel::OnBnRadioGateBelow()
{
	m_nGateType = 1;
}
//Machao add end on 2013-06-27
// Song Chenguang add start on 2013-07-15
void CCScanParamPanel::OnBnClickedRealScale()
{
	UpdateData(TRUE);
	m_pCurCScanView->Invalidate(TRUE);
}
// Song Chenguang add end on 2013-07-15//Machao add start on 2013-07-1 
void CCScanParamPanel::OnBnClickedButtonPalette()
{
	// TODO: Add your control notification handler code here
	CPaletteDog dog;
	// machao modify start on 2013-07-31
	m_vnMainColorNum.clear();
//	bool b = dog.GetPlatteData(m_plaPalette,m_nSize,m_clrBack1,m_clrBack2);
	bool b = dog.GetPlatteData(m_plaPalette,m_nSize,m_vnMainColorNum,m_clrBack1,m_clrBack2);
	// machao modify end on 2013-07-31
	if (b)
	{
		m_nMainColorNum = m_vnMainColorNum.size();
		m_nColorDefine = 0;
		Invalidate(TRUE);
	}
}

void CCScanParamPanel::DrawColorBar()
{
	CRect rc;
	GetDlgItem(IDC_STATIC_GRADIENT)->GetWindowRect(rc);
	ScreenToClient(&rc);
	CDC *pDC = GetDC();
	CDC MemDC;
	CBitmap Bitmap;
	pDC->FillSolidRect(&rc,RGB(255,0,0));
	Bitmap.CreateCompatibleBitmap(pDC,30,193);//创建位图

	MemDC.CreateCompatibleDC(pDC);     //创建内存DC
	CBitmap* pOldBmp = MemDC.SelectObject(&Bitmap);   //把位图选进内存DC

	float fNum = MAXFVALUE;
	rc.right -= 4;
	rc.bottom -= 5;
	for (int i = 0; i < m_nSize; i++)
	{
		int nTop = (int)((fNum - m_plaPalette[i].fMaxPercent) * (float)(rc.bottom - rc.top) / fNum);
		int nBottom = (int)((fNum - m_plaPalette[i].fMinPercent) * (float)(rc.bottom - rc.top) / fNum);
		CBrush brush(m_plaPalette[i].color);
		MemDC.FillRect(CRect(0,nTop,rc.right - rc.left,nBottom),&brush);
	}
	CBrush brush1(m_clrBack1);
	MemDC.FillRect(CRect(rc.right - rc.left,0,rc.right - rc.left + 4,rc.bottom - rc.top + 5),&brush1);
	CBrush brush2(m_clrBack2);
	MemDC.FillRect(CRect(0,rc.bottom - rc.top,rc.right - rc.left,rc.bottom - rc.top + 5),&brush2);
	pDC->BitBlt(rc.left,rc.top,30,193,&MemDC,0,0,SRCCOPY);
	MemDC.SelectObject(pOldBmp);
	DeleteObject(Bitmap);
}
//Machao add end on 2013-07-1
//Machao add start on 2013-07-11

void CCScanParamPanel::OnBnClickedButtonImportPalette()
{
	// TODO: Add your control notification handler code here
	CString strExt = _T(".prt");
	CString strFilePath;
	CString strFilter;

	strFilter.Format(_T("Prt Files (*prt)|*prt|"));
	CFileDialog dlg(TRUE, NULL, NULL, NULL, strFilter);
	if (dlg.DoModal() == IDOK)
	{
		strFilePath = dlg.GetPathName();
		if (strFilePath.Find(strExt) == -1)
		{
			MessageBox(_T("找不到文件，请检查文件名是否正确"));
		}
		ReadColorFile(strFilePath);	// machao modify on 2013-11-21
		Invalidate(TRUE);
	}
}
//Machao add end on 2013-07-11
//machao add start on 2013-07-12
void CCScanParamPanel::CountRect(int nNum, int nIndex, float fTopPercent, float fBottomPercent, COLORREF clrTop, COLORREF clrBottom)
{
	float fR,fG,fB;
	fR = (float)(GetRValue(clrBottom) - GetRValue(clrTop)) / (nNum + 1);
	fG = (float)(GetGValue(clrBottom) - GetGValue(clrTop)) / (nNum + 1);
	fB = (float)(GetBValue(clrBottom) - GetBValue(clrTop)) / (nNum + 1);
	float fchange = (fTopPercent - fBottomPercent) / nNum;
	for (int i = 0; i < nNum; i++)
	{
		int nR = int(GetRValue(clrTop) + (i + 1) * fR);
		int nG = int(GetGValue(clrTop) + (i + 1) * fG);
		int nB = int(GetBValue(clrTop) + (i + 1) * fB);
		m_plaPalette[nIndex].color = RGB(nR, nG, nB);
		m_plaPalette[nIndex].fMaxPercent = fTopPercent - i * fchange;
		m_plaPalette[nIndex].fMinPercent = fTopPercent - (i + 1) * fchange;
		nIndex++;
	}
}
//machao add end on 2013-07-12

//machao add start on 2013-08-5
void CCScanParamPanel::OnBnClickedButtonDefaultColor3()
{
	// machao add start on 2013-11-22
	TCHAR strFolder[MAX_PATH];
	GetWorkingFolder(strFolder, MAX_PATH);
	CString csFilePath = CString(strFolder) + _T("DefaultColor3.prt");//取得配置文件路径
	if (PathFileExists(csFilePath))
	{
		ReadColorFile(csFilePath);
		Invalidate(TRUE);
		return;
	}
	// machao add end on 2013-11-22
	
	m_nMainColorNum = 2;
	m_nSize = 5;	// machao modify on 2013-11-19
	m_vnMainColorNum.clear();
#if _HAFEI
	m_plaPalette[0].color = RGB(0,0,0);
	m_plaPalette[0].fMaxPercent = 100;//DBTo100Percent(g_fDBZeroAmp, 0);
	m_plaPalette[0].fMinPercent = DBTo100Percent(g_fDBZeroAmp, -20);
	m_plaPalette[1].color = RGB(63,63,63);
	m_plaPalette[1].fMaxPercent = DBTo100Percent(g_fDBZeroAmp, -20);
	m_plaPalette[1].fMinPercent = DBTo100Percent(g_fDBZeroAmp, -22.5);
	m_plaPalette[2].color = RGB(127,127,127);
	m_plaPalette[2].fMaxPercent = DBTo100Percent(g_fDBZeroAmp, -22.5);
	m_plaPalette[2].fMinPercent = DBTo100Percent(g_fDBZeroAmp, -26);
	m_plaPalette[3].color = RGB(191,191,191);
	m_plaPalette[3].fMaxPercent = DBTo100Percent(g_fDBZeroAmp, -26);
	m_plaPalette[3].fMinPercent = DBTo100Percent(g_fDBZeroAmp, -32);
	m_plaPalette[4].color = RGB(255,255,255);
	m_plaPalette[4].fMaxPercent = DBTo100Percent(g_fDBZeroAmp, -32);
	m_plaPalette[4].fMinPercent = 0;
	m_plaPalette[5].color = RGB(255,255,255);
	m_plaPalette[5].fMaxPercent = 0;
	m_plaPalette[5].fMinPercent = -1;
#else
	m_plaPalette[0].color = RGB(0,0,0);
	m_plaPalette[0].fMaxPercent = 127.5;//DBTo100Percent(g_fDBZeroAmp, 0);	// machao modify on 2013-11-19
	m_plaPalette[0].fMinPercent = 80;	// machao modify on 2013-09-5
	m_plaPalette[1].color = RGB(63,63,63);
	m_plaPalette[1].fMaxPercent = 80;
	m_plaPalette[1].fMinPercent = 60;
	m_plaPalette[2].color = RGB(127,127,127);
	m_plaPalette[2].fMaxPercent = 60;
	m_plaPalette[2].fMinPercent = 40;
	m_plaPalette[3].color = RGB(191,191,191);
	m_plaPalette[3].fMaxPercent = 40;
	m_plaPalette[3].fMinPercent = 20;
	m_plaPalette[4].color = RGB(255,255,255);
	m_plaPalette[4].fMaxPercent = 20;
	m_plaPalette[4].fMinPercent = 0;
	m_plaPalette[5].color = RGB(255,255,255);
	m_plaPalette[5].fMaxPercent = 0;
	m_plaPalette[5].fMinPercent = -1;
#endif
	m_clrBack1 = RGB(180,180,180);
	// machao modify start on 2013-11-19
	m_clrBack2 = RGB(180,180,180);
//	m_clrBack2 = RGB(75,140,255);
	// machao modify end on 2013-11-19
	m_nMainColorNum = 2;
	m_vnMainColorNum.push_back(3);
	m_vnMainColorNum.push_back(3);
	m_pCurCScanView->m_nColorDefine = 0;

	m_nColorDefine = 0;
	Invalidate(TRUE);
}
//machao add start on 2013-08-5
// machao add start on 2013-09-12
void CCScanParamPanel::DefaultColor1()
{
	// machao add start on 2013-11-21
	TCHAR strFolder[MAX_PATH];
	GetWorkingFolder(strFolder, MAX_PATH);
	CString csFilePath = CString(strFolder) + _T("DefaultColor1.prt");//取得配置文件路径
	if (PathFileExists(csFilePath))
	{
		ReadColorFile(csFilePath);
		return;
	}
	// machao add end on 2013-11-21
	m_nMainColorNum = 6;	//machao add on 2013-07-22
	m_nSize = 256;
	m_vnMainColorNum.clear();
	m_plaPalette[0].color = RGB(255,255,255);
	m_plaPalette[0].fMaxPercent = 127.5;
	m_plaPalette[0].fMinPercent = 127.0;
	CountRect(50,1,127.0,102.0,RGB(255,255,255),RGB(255,255,0));
	m_vnMainColorNum.push_back(26);			//machao add on 2013-07-31
	m_plaPalette[51].color = RGB(255,255,0);
	m_plaPalette[51].fMaxPercent = 102;
	m_plaPalette[51].fMinPercent = 101.5;
	CountRect(50,52,101.5,76.5,RGB(255,255,0),RGB(255,0,0));
	m_vnMainColorNum.push_back(51);			//machao add on 2013-07-31
	m_plaPalette[102].color = RGB(255,0,0);
	m_plaPalette[102].fMaxPercent = 76.5;
	m_plaPalette[102].fMinPercent = 76;
	CountRect(50,103,76,51,RGB(255,0,0),RGB(0,255,0));
	m_vnMainColorNum.push_back(51);			//machao add on 2013-07-31
	m_plaPalette[153].color = RGB(0,255,0);
	m_plaPalette[153].fMaxPercent = 51;
	m_plaPalette[153].fMinPercent = 50.5;
	CountRect(50,154,50.5,25.5,RGB(0,255,0),RGB(0,0,255));
	m_vnMainColorNum.push_back(51);			//machao add on 2013-07-31
	m_plaPalette[204].color = RGB(0,0,255);
	m_plaPalette[204].fMaxPercent = 25.5;
	m_plaPalette[204].fMinPercent = 25;
	CountRect(50,205,25,0,RGB(0,0,255),RGB(0,0,0));
	m_vnMainColorNum.push_back(51);			//machao add on 2013-07-31
	m_plaPalette[255].color = RGB(0,0,0);
	m_plaPalette[255].fMaxPercent = 0;
	m_plaPalette[255].fMinPercent = -1;
	m_vnMainColorNum.push_back(26);			//machao add on 2013-07-31
	m_clrBack1 = RGB(180,180,180);
//	m_clrBack2 = RGB(75,140,255);
	m_clrBack2 = RGB(180,180,180);	// Song Chenguang modify on 2013-09-13 临时修改，使底色一致

	m_pCurCScanView->m_nColorDefine = 0;	// machao add on 2013-08-13
	m_nColorDefine = 0;
}
// machao add end on 2013-09-12

// machao add start on 2013-11-21
void CCScanParamPanel::ReadColorFile(CString strPath)
{
	CFile file(strPath,CFile::modeRead);
	file.Read(&m_nMainColorNum,sizeof(m_nMainColorNum));	//machao add on 2013-07-22
	//machao add start on 2013-07-25
	m_vnMainColorNum.clear();
	for (int i = 0; i < m_nMainColorNum; i++)
	{
		MCOLOR mclr;
		file.Read(&mclr,sizeof(mclr));
		m_vnMainColorNum.push_back(mclr.nGradualColor);
	}
	//machao add end on 2013-07-25
	file.Read(&m_clrBack1,sizeof(m_clrBack1));
	file.Read(&m_clrBack2,sizeof(m_clrBack2));
	COLORREF clrColor[PLATTE_NUM] = {NULL};
	float fPercent[PLATTE_NUM] = {NULL};
	for (int i = 0;i < PLATTE_NUM; i++)
	{
		int n = file.Read(&clrColor[i],sizeof(clrColor[i]));
		file.Read(&fPercent[i],sizeof(fPercent[i]));
		if(n == 0)
			break;
	}
	file.Close();
	int nIndex = 0;
	for (int i = 0; i < PLATTE_NUM - 1; i++)
	{
		if (NULL == fPercent[i])
		{
			break;
		}
		m_plaPalette[i].color = clrColor[i];
		m_plaPalette[i].fMaxPercent = fPercent[i];
		m_plaPalette[i].fMinPercent = fPercent[i + 1];
		nIndex++;
	}
	m_plaPalette[nIndex].color =clrColor[nIndex];
	m_plaPalette[nIndex].fMaxPercent = fPercent[nIndex];
	m_plaPalette[nIndex].fMinPercent = -1;
	m_nSize = nIndex;
	m_nColorDefine = 0;
	m_pCurCScanView->m_nColorDefine = 0;
}
// machao add end on 2013-11-21


//yachang.huang add start on 2014-3-7
void CCScanParamPanel::OnBnClickedLockParam()
{
	if (m_bLockParam)
	{
		GetDlgItem(IDC_RADIO_MIN_VALUE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_MAX_VALUE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_AVG_VALUE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_GATE_ABOVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_GATE_BELOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SELECT_GATE1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SELECT_GATE2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GATE1_THRESHOLD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GATE2_THRESHOLD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GATE3_THRESHOLD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GATE4_THRESHOLD)->EnableWindow(TRUE);
		m_bLockParam = FALSE;
	}
	else
	{
		GetDlgItem(IDC_RADIO_MIN_VALUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_MAX_VALUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_AVG_VALUE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_GATE_ABOVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_GATE_BELOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SELECT_GATE1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SELECT_GATE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GATE1_THRESHOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GATE2_THRESHOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GATE3_THRESHOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GATE4_THRESHOLD)->EnableWindow(FALSE);
		m_bLockParam = TRUE;
	}
}
//yachang.huang add end on 2014-3-7
void CCScanParamPanel::OnBnClickedButtonBackgroundColor()
{
	// TODO: Add your control notification handler code here


	CColorDialog colorDlg;
	if (IDOK == colorDlg.DoModal())
	{
		COLORREF color = colorDlg.GetColor();

		m_clrBackColor = color;
		m_brushBackColor.DeleteObject();			
		m_brushBackColor.CreateSolidBrush(color);
		CRect rect;
		GetDlgItem(IDC_STATIC_BACK_COLOR)->GetClientRect(rect);
		GetDlgItem(IDC_STATIC_BACK_COLOR)->InvalidateRect(&rect);

	}

}

HBRUSH CCScanParamPanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{


	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);



	//设置控件IDC_STATIC_BACK_COLOR的颜色
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_BACK_COLOR)
	{
		return m_brushBackColor;
	}


	return hbr;
}
