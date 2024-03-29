// ChannelPanel.cpp : 实现文件
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 通道面板窗体视图源文件
*Filename : ChannelPanel.cpp
*
*@author       songchenguang zhaoli 
*@version      1.0         Date: 2011-7-10
*/

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "UltrasonicImmersedTestDoc.h"
#include "CScanDlgBar.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "ExButton.h"
#include "SysParam.h"
#include "MechanicalControlPanel.h"
#include "MechanicalControl.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"
#include "MovePathView.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "UltrasonicImmersedTestView.h"
#include "UltrasonicImmersedTestDoc.h"
#include "GlobalFunction.h"
#include "FeatureInfoWnd.h"
#include "ProbeInfoDlg.h"
#include "DacCurveDlg.h"
#include <math.h>
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
#include "TcgCurveDlg.h"

// CChannelPanel
bool g_bReFreqFlag = FALSE;
extern DWORD g_dwActiveChannel;
short g_nWaveSpeed = 6000;			// 声速
const int DELAY_RANGE = 200;		// 延迟范围
const int ZERO_POINT_RANGE = 200;	// 零点范围
const int A_SCAN_MAX_RANGE = 1000;	// A扫描最大范围
const int GATE_MAX_RANGE = 1000;	// 闸门最大范围
const int GATE_PEAK_MAX_RANGE = 100;	// 闸门高度最大值
const int PLUS_RANGE = 110;			// 增益范围

extern BOOL g_bDrawRealTime;
IMPLEMENT_DYNCREATE(CChannelPanel, CFormView)

CChannelPanel::CChannelPanel()
	: CFormView(CChannelPanel::IDD)
	, m_nChannelNo(0)
	, m_fGateFront(0)
	, m_fGateBack(0)
	, m_fGatePeak(0)
	, m_bGateIsOpen(FALSE)
	, m_fDelay(0)
	, m_fRange(0)
	, m_fPlus(0)
	, m_fRefPlus(0)		 // Song Chenguang add on 2012-07-25
	, m_bMTPmode(FALSE)
	, m_fMTPPeak(0)
	, m_fZeroPoint(0)
	, m_nWaveSpeed(5980)
	, m_bMPFTimeDisplay(FALSE)
	, m_bMPMTimeDisplay(TRUE)
	, m_bMPMAltitudeDisplay(TRUE)
	, m_bFPFTimeDisplay(TRUE)
	, m_bFPMTimeDisplay(FALSE)
	, m_bFPMAltitudeDisplay(FALSE)
	, m_bGroup1Flag(TRUE)
	, m_bGroup2Flag(FALSE)
	, m_bGroup3Flag(FALSE)
	, m_bGroup4Flag(FALSE)
	, m_fRestrain(0)
	, m_nReFreq(500)
	, m_bLockParam(FALSE)
//	, m_wWorkMode(1)
	, m_bExtendFlag(FALSE)	// wangbingfu modify at 2011-11-19
	, m_bGateAdjust(FALSE)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::CChannelPanel"),"Enter");

	m_brush.CreateSolidBrush(RGB(255,0,0));
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		m_bRfReduced[i] = FALSE;
		m_fOldDelay[i] = 0;
		m_fOldRange[i] = 0;
		m_fOldPlus[i] = 0;
		m_bExtendArrayFlag[i] = FALSE;
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::CChannelPanel"),"Leave");
}

CChannelPanel::~CChannelPanel()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::~CChannelPanel"),"Enter");
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::~CChannelPanel"),"Leave");
}

void CChannelPanel::DoDataExchange(CDataExchange* pDX)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoDataExchange"),"Enter");

	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_GATE_NO, m_cmbGateNo);
	DDX_Radio(pDX, IDC_RADIO_CH1, m_nChannelNo);
	DDX_Control(pDX, IDC_SLIDER_GATE_FRONT, m_sliderGateFront);
	DDX_Control(pDX, IDC_SLIDER_GATE_BACK, m_sliderGateBack);
	DDX_Control(pDX, IDC_SLIDER_GATE_PEAK, m_sliderGatePeak);
	DDX_Text(pDX, IDC_EDIT_GATE_FRONT, m_fGateFront);
	DDX_Text(pDX, IDC_EDIT_GATE_BACK, m_fGateBack);
	DDX_Text(pDX, IDC_EDIT_GATE_PEAK, m_fGatePeak);
	DDX_Check(pDX, IDC_CHECK_GATE_ISOPEN, m_bGateIsOpen);
	DDX_Control(pDX, IDC_STATIC_DIPLAYCOLOR, m_staticDisplayColor);
	DDX_Control(pDX, IDC_SLIDER_DELAY, m_sliderDelay);
	DDX_Text(pDX, IDC_EDIT_DELAY, m_fDelay);
	DDX_Control(pDX, IDC_SLIDER_RANGE, m_sliderRange);
	DDX_Text(pDX, IDC_EDIT_RANGE, m_fRange);
	DDX_Control(pDX, IDC_SLIDER_PLUS, m_sliderPlus);
	DDX_Text(pDX, IDC_EDIT_PLUS, m_fPlus);
	DDX_Radio(pDX, IDC_RADIO_INITPULSE, m_bMTPmode);
	DDX_Control(pDX, IDC_SLIDER_MTP_PEAK, m_sliderMTPPeak);
	DDX_Text(pDX, IDC_EDIT_MTP_PEAK, m_fMTPPeak);
	DDX_Control(pDX, IDC_COMBO_CH_SI, m_comboChSI);
	DDX_Control(pDX, IDC_COMBO_CH_SAVG, m_comboChSavg);
	DDX_Control(pDX, IDC_SLIDER_ZEROPOINT, m_sliderZeroPoint);
	DDX_Text(pDX, IDC_EDIT_ZEROPOINT, m_fZeroPoint);
	DDX_Text(pDX, IDC_EDIT_WAVE_SPEED, m_nWaveSpeed);
	DDV_MinMaxInt(pDX, m_nWaveSpeed, 1, 10000);
	DDX_Check(pDX, IDC_CHECK_MP_F_TIME, m_bMPFTimeDisplay);
	DDX_Check(pDX, IDC_CHECK_MP_M_TIME, m_bMPMTimeDisplay);
	DDX_Check(pDX, IDC_CHECK_MP_M_ALTITUDE, m_bMPMAltitudeDisplay);
	DDX_Check(pDX, IDC_CHECK_FP_F_TIME, m_bFPFTimeDisplay);
	DDX_Check(pDX, IDC_CHECK_FP_M_TIME, m_bFPMTimeDisplay);
	DDX_Check(pDX, IDC_CHECK_FP_M_ALTITUDE, m_bFPMAltitudeDisplay);
	DDX_Control(pDX, IDC_COMBO_PROBE_FREQ, m_cmbProbeFreq);
	DDX_Check(pDX, IDC_CHECK_GROUP1_SWITCH, m_bGroup1Flag);
	DDX_Check(pDX, IDC_CHECK_GROUP2_SWITCH, m_bGroup2Flag);
	DDX_Check(pDX, IDC_CHECK_GROUP3_SWITCH, m_bGroup3Flag);
	DDX_Check(pDX, IDC_CHECK_GROUP4_SWITCH, m_bGroup4Flag);
	DDX_Text(pDX, IDC_EDIT_RESTRAIN, m_fRestrain);
	DDX_Control(pDX, IDC_SLIDER_RESTRAIN, m_sliderRestrain);
	DDX_Control(pDX, IDC_COMBO_MATERIAL, m_cmbMaterial);
	//	DDX_Control(pDX, IDC_COMBO_SYNC_FREQ, m_cmbSyncFreq);
	DDX_Control(pDX, IDC_SLIDER_RE_FREQ, m_sliderReFreq);
	DDX_Text(pDX, IDC_EDIT_RE_FREQ, m_nReFreq);
	DDV_MinMaxUInt(pDX, m_nReFreq, RE_FREQ_MIN, RE_FREQ_MAX);
	DDX_Check(pDX, IDC_CHECK_LOCK_PARAM, m_bLockParam);
	DDX_Control(pDX, IDC_SLIDER_REF_PLUS, m_sldRefPlus);
	DDX_Text(pDX, IDC_EDIT_REF_PLUS, m_fRefPlus);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoDataExchange"),"Leave");
	DDX_Check(pDX, IDC_CHECK_EXTENSION, m_bExtendFlag);	// wangbingfu modify at 2011-11-19
	DDX_Check(pDX, IDC_CHECK_GATE_ADJUST, m_bGateAdjust);
}

BEGIN_MESSAGE_MAP(CChannelPanel, CFormView)
	ON_BN_CLICKED(IDC_RADIO_CH1, &CChannelPanel::OnBnClickedRadioCh1)
	ON_BN_CLICKED(IDC_RADIO_CH2, &CChannelPanel::OnBnClickedRadioCh2)

	ON_CBN_SELCHANGE(IDC_COMBO_GATE_NO, &CChannelPanel::OnCbnSelchangeComboGateNo)
	ON_BN_CLICKED(IDC_CHECK_GATE_ISOPEN, &CChannelPanel::OnBnClickedCheckGateIsopen)
	ON_BN_CLICKED(IDC_BUTTON_GATE_COLOR, &CChannelPanel::OnBnClickedButtonGateColor)
	ON_BN_CLICKED(IDC_BUTTON_OTHER_PARAM, &CChannelPanel::OnBnChickedOtherParam)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_INITPULSE, &CChannelPanel::OnBnClickedRadioInitpulse)
	ON_BN_CLICKED(IDC_RADIO_FIRSTECHO, &CChannelPanel::OnBnClickedRadioFirstecho)
	ON_CBN_SELCHANGE(IDC_COMBO_CH_SI, &CChannelPanel::OnCbnSelchangeComboChSi)
	ON_CBN_SELCHANGE(IDC_COMBO_CH_SAVG, &CChannelPanel::OnCbnSelchangeComboChSavg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FRONT, &CChannelPanel::OnDeltaposSpinFront)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_WIDTH, &CChannelPanel::OnDeltaposSpinWidth)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GATE_PEAK, &CChannelPanel::OnDeltaposSpinGatePeak)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_MTP_PEAK, &CChannelPanel::OnDeltaposSpinMtpPeak)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ZEROPOINT, &CChannelPanel::OnDeltaposSpinZeropoint)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DELAY, &CChannelPanel::OnDeltaposSpinDelay)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RANGE, &CChannelPanel::OnDeltaposSpinRange)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_PLUS, &CChannelPanel::OnDeltaposSpinPlus)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_AS_PARAM, &CChannelPanel::OnBnClickedButtonSaveAsParam)
	ON_BN_CLICKED(IDC_BUTTON_READ_PARAM, &CChannelPanel::OnBnClickedButtonReadParam)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT_WAVE_SPEED, &CChannelPanel::OnEnKillfocusEditWaveSpeed)
	ON_CBN_SELCHANGE(IDC_COMBO_PROBE_FREQ, &CChannelPanel::OnCbnSelchangeComboProbeFreq)
	ON_BN_CLICKED(IDC_CHECK_MP_M_ALTITUDE, &CChannelPanel::OnBnClickedCheckMpMAltitude)
	ON_BN_CLICKED(IDC_CHECK_FP_M_ALTITUDE, &CChannelPanel::OnBnClickedCheckFpMAltitude)
	ON_BN_CLICKED(IDC_CHECK_MP_F_TIME, &CChannelPanel::OnBnClickedCheckMpFTime)
	ON_BN_CLICKED(IDC_CHECK_MP_M_TIME, &CChannelPanel::OnBnClickedCheckMpMTime)
	ON_BN_CLICKED(IDC_CHECK_FP_F_TIME, &CChannelPanel::OnBnClickedCheckFpFTime)
	ON_BN_CLICKED(IDC_CHECK_FP_M_TIME, &CChannelPanel::OnBnClickedCheckFpMTime)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_PARAM, &CChannelPanel::OnBnClickedButtonSaveParam)
	ON_BN_CLICKED(IDC_CHECK_GROUP1_SWITCH, &CChannelPanel::OnBnClickedCheckGroup1Switch)
	ON_BN_CLICKED(IDC_CHECK_GROUP2_SWITCH, &CChannelPanel::OnBnClickedCheckGroup2Switch)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RESTRAIN, &CChannelPanel::OnDeltaposSpinRestrain)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO_CH3, &CChannelPanel::OnBnClickedRadioCh3)
	ON_BN_CLICKED(IDC_RADIO_CH4, &CChannelPanel::OnBnClickedRadioCh4)
	ON_BN_CLICKED(IDC_RADIO_CH5, &CChannelPanel::OnBnClickedRadioCh5)
	ON_BN_CLICKED(IDC_RADIO_CH6, &CChannelPanel::OnBnClickedRadioCh6)
	ON_BN_CLICKED(IDC_RADIO_CH7, &CChannelPanel::OnBnClickedRadioCh7)
	ON_BN_CLICKED(IDC_RADIO_CH8, &CChannelPanel::OnBnClickedRadioCh8)
	ON_BN_CLICKED(IDC_CHECK_GROUP3_SWITCH, &CChannelPanel::OnBnClickedCheckGroup3Switch)
	ON_BN_CLICKED(IDC_CHECK_GROUP4_SWITCH, &CChannelPanel::OnBnClickedCheckGroup4Switch)
	ON_CBN_SELCHANGE(IDC_COMBO_MATERIAL, &CChannelPanel::OnCbnSelchangeComboMaterial)
	// DongQi delete start on 2013-06-27
// 	ON_EN_CHANGE(IDC_EDIT_WAVE_SPEED, &CChannelPanel::OnEnChangeEditWaveSpeed)
	// DongQi delete end on 2013-06-27
//	ON_CBN_SELCHANGE(IDC_COMBO_SYNC_FREQ, &CChannelPanel::OnCbnSelchangeComboSyncFreq)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RE_FREQ, &CChannelPanel::OnDeltaposSpinReFreq)
	ON_BN_CLICKED(IDC_CHECK_LOCK_PARAM, &CChannelPanel::OnBnClickedCheckLockParam)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_REF_PLUS, &CChannelPanel::OnDeltaposSpinRefPlus)
//	ON_CBN_SELCHANGE(IDC_COMBO_WORK_MODE, &CChannelPanel::OnCbnSelchangeComboWorkMode)
	ON_BN_CLICKED(IDC_CHECK_EXTENSION, &CChannelPanel::OnBnClickedCheckExtension)	// wangbingfu modify at 2011-11-19
	ON_BN_CLICKED(IDC_CHECK_GATE_ADJUST, &CChannelPanel::OnBnClickedCheckGateAdjust)
	ON_WM_LBUTTONDOWN()//yachang.huang add start on 2013-06-29
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_RE_FREQ, &CChannelPanel::OnReleasedcaptureSliderReFreq)
END_MESSAGE_MAP()


// CChannelPanel diagnostics

#ifdef _DEBUG
void CChannelPanel::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CChannelPanel::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CChannelPanel message handlers
/** 
* 在工具栏窗体中创建视图
* @param     pParent [in]:  父窗口指针
* @param     pContext [in]:  窗口上下文                                            
* @return    void
* @since     1.0
*/
BOOL CChannelPanel::CreateView(CWnd* pParent, CCreateContext* pContext)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::CreateView"),"Enter");

	// Song Chenguang  add start on 2012-11-03
	if (pParent == NULL)
	{
		MessageBox(_T("CWnd* pParent 传参为空"), _T("错误"), MB_OK|MB_ICONERROR);
		
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CChannelPanel::CreateView"),"Leave");

		return FALSE;
	}

	if (pContext == NULL)
	{
		MessageBox(_T("CWnd* pContext 传参为空"), _T("错误"), MB_OK|MB_ICONERROR);

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CChannelPanel::CreateView"),"Leave");

		return FALSE;
	}
	// Song Chenguang  add end on 2012-11-03

	CRect rect(0,0,300,300);

	if (!Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		rect, pParent, AFX_IDW_PANE_FIRST, pContext))
	{
		TRACE0("Warning: couldn't create CMyFormView!\n");

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CChannelPanel::CreateView"),"Leave");

		return FALSE;
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::CreateView"),"Leave");

	return TRUE;
}
void CChannelPanel::OnInitialUpdate()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnInitialUpdate"),"Enter");

	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pAScanView = (CUltrasonicImmersedTestView*)pFrame->m_wndSplitter.GetPane(1,0);
	if (m_pAScanView == NULL)
	{
		MessageBox(_T("CChannelPanel::OnInitialUpdate() m_pAScanView指针获取失败"), _T("错误"), MB_OK|MB_ICONERROR);

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CChannelPanel::OnInitialUpdate"),"Leave");
		return;
	}

	g_dwActiveChannel = m_nChannelNo;

	//设置每个闸门默认的颜色，无闸门：无色，1闸门：红色，2闸门：绿色，3闸门：粉色，4闸门：青色
	for (int i = 0; i<CHANNEL_NUM; i++)
	{
		m_ChannelInfo[i].m_GateInfo[0].m_colorGate = RGB(255,0,0);
		m_ChannelInfo[i].m_GateInfo[1].m_colorGate = RGB(0,255,0);
		m_ChannelInfo[i].m_GateInfo[2].m_colorGate = RGB(255,0,255);
		m_ChannelInfo[i].m_GateInfo[3].m_colorGate = RGB(0,255,255);
	}
 
	CString strProbeFreq[8] = {_T("0.2~30MHz"),_T("10~30MHz"), _T("15MHz"),_T("1~10MHz"), _T("10MHz"), _T("4MHz"), _T("2MHz"), _T("1MHz")};
	for (int i=0; i<8; i++)
	{
		m_cmbProbeFreq.AddString(strProbeFreq[i]);
	}
	int nIndexFreq = (int)m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eProbeFreq;
	m_cmbProbeFreq.SetCurSel(nIndexFreq);
	
//	OnCbnSelchangeComboProbeFreq();

	CString strGate[4] = {_T("I"), _T("A"), _T("B"), _T("C")};
	for (int i=0; i<4; i++)
	{
		m_cmbGateNo.AddString(strGate[i]);
	}
	m_cmbGateNo.SetCurSel(0);

	// 设置滑块的滚动范围
	m_sliderGateFront.SetRange(0, GATE_MAX_RANGE * 10);
	m_sliderGateBack.SetRange(0, GATE_MAX_RANGE * 10);
	m_sliderGatePeak.SetRange(0, GATE_PEAK_MAX_RANGE * 10);
	m_sliderMTPPeak.SetRange(0, GATE_PEAK_MAX_RANGE * 10);

	m_sliderDelay.SetRange(0, DELAY_RANGE * 10);
	m_sliderRange.SetRange(0, A_SCAN_MAX_RANGE * 10);
	m_sliderPlus.SetRange(0, PLUS_RANGE * 10);
	// Song Chenguang add start on 2012-07-25
	m_sldRefPlus.SetRange(0, PLUS_RANGE * 10);
	// Song Chenguang add end on 2012-07-25
	m_sliderZeroPoint.SetRange(0, ZERO_POINT_RANGE * 10);
	m_sliderRestrain.SetRange(0, 1000);
	m_sliderReFreq.SetRange(RE_FREQ_MIN, RE_FREQ_MAX);

	InitChannelInfo();	// 初始化通道控制面板控件

	//将修改过后的闸门信息同步显示到A扫视图区域
	for(int i=0; i<4; i++)
	{		
		m_pAScanView->m_pGateInfo[i] = m_ChannelInfo[m_nChannelNo].m_GateInfo[i];
	}

	SetGateInfo();		// 更新闸门参数
	SetProbeInfo();		// 更新探头参数	

	// 改变C扫面板控件状态
	ALLSCanItemEnableJudge();

	// 根据AEC卡禁止操作没对应卡的通道
	UINT uChannelID[CHANNEL_NUM] = {IDC_RADIO_CH1, IDC_RADIO_CH2, IDC_RADIO_CH3, IDC_RADIO_CH4,
		IDC_RADIO_CH5, IDC_RADIO_CH6, IDC_RADIO_CH7, IDC_RADIO_CH8};

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	WORD wAECNum = pApp->GetCardNum();

	for (int i=(sizeof(uChannelID) / sizeof(UINT) - 1); i>=0; i--)
	{
		if(i >= (wAECNum*2))
		{
			CWnd *pWnd = GetDlgItem(uChannelID[i]);
			// Song Chenguang modified start on 2012-11-03
			if (pWnd != NULL)
				pWnd->EnableWindow(FALSE);
			else
			{
				MessageBox(_T("CChannelPanel::OnInitialUpdate() GetDlgItem失败"), _T("错误"), MB_OK|MB_ICONERROR);
				
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CChannelPanel::OnInitialUpdate"),"Leave");

				return;
			}
			// Song Chenguang modified end on 2012-11-03
		}		
	}

	for (int j=0; j<(sizeof(uChannelID) / sizeof(UINT)); j++)
	{
		CWnd* pWnd = GetDlgItem(uChannelID[j]);
		// Song Chenguang add start on 2012-11-03
		if (pWnd == NULL)
		{
			MessageBox(_T("CChannelPanel::OnInitialUpdate() GetDlgItem失败"), _T("错误"), MB_OK|MB_ICONERROR);

			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CChannelPanel::OnInitialUpdate"),"Leave");

			return;
		}
		// Song Chenguang add end on 2012-11-03
		BOOL bEnable = pWnd->IsWindowEnabled();		

		if (bEnable)
		{
			m_nChannelNo = j;
			break;
		}
	}

	InitMaterialSoundSpeed();

	BOOL bFind = FALSE;
	for (int i=0; i<MATERIAL_NUM; i++)
	{
		if (m_nWaveSpeed == m_MaterialSoundSpeeds[i].m_wSoundSpeed)
		{
			m_cmbMaterial.SetCurSel(i);
			bFind = TRUE;
		}
	}

	if (!bFind)
		m_cmbMaterial.SetCurSel(11);	// 自定义

//	m_cmbSyncFreq.SetCurSel(3);

	// Song Chenguang modified start on 2012-07-12
//	DoModifyReFreq();		
	// Song Chenguang modified end on 2012-07-12
/*
	// Song Chenguang add start on 2012-07-12
	// 工作模式
	CComboBox* pcmbWorkMode = (CComboBox*)GetDlgItem(IDC_COMBO_WORK_MODE);
	pcmbWorkMode->SetCurSel(m_wWorkMode - 1);

	// 下发工作模式
	int nCardNum = pApp->GetCardNum();
	for (int i=0; i<nCardNum; i++)
	{
//		m_ChannelInfo[2 * i].m_ProbeInfo.m_wWorkMode = m_wWorkMode;
//		m_ChannelInfo[2 * i + 1].m_ProbeInfo.m_wWorkMode = m_wWorkMode;

		SendWorkMode(i, m_wWorkMode);
	}

	// 模式为单2、双晶, 切换到偶数通道,
	int nYu = m_nChannelNo % 2;
 	if (m_wWorkMode == 2 || m_wWorkMode == 4 || m_wWorkMode == 5 ||
 		m_wWorkMode == 7 || m_wWorkMode == 8)
//	if (m_wWorkMode == 2 || m_wWorkMode == 5 || m_wWorkMode == 6 ||
//		m_wWorkMode == 8 || m_wWorkMode == 9)
	{
		if (nYu == 0)
		{
			m_nChannelNo += 1;
		}

		DoChangeChannel();
	}
	else if (nYu == 1 && m_wWorkMode == 1)
	{
		m_nChannelNo -= 1;
		
		DoChangeChannel();
	}
*/	
	// Song Chenguang add end on 2012-07-12
//	SetMoveRule(m_wWorkMode);

	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnInitialUpdate"),"Leave");
}

/** 
* 闸门开关
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::OnBnClickedCheckGateIsopen()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckGateIsopen"),"Enter");

	UpdateData(TRUE);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();

	int n = m_cmbGateNo.GetCurSel();
	if (-1 == n)
	{
		MessageBox(_T("请选择闸门！"));
		return;
	}

	m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_bOpen = m_bGateIsOpen;
	if (!m_bGateIsOpen)
	{
		m_sliderGateFront.EnableWindow(FALSE);
		m_sliderGateBack.EnableWindow(FALSE);
		m_sliderGatePeak.EnableWindow(FALSE);

		GetDlgItem(IDC_SPIN_FRONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_GATE_PEAK)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_MTP_PEAK)->EnableWindow(FALSE);

	}
	else
	{
		m_sliderGateFront.EnableWindow(TRUE);
		m_sliderGateBack.EnableWindow(TRUE);
		m_sliderGatePeak.EnableWindow(TRUE);

		GetDlgItem(IDC_SPIN_FRONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_GATE_PEAK)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_MTP_PEAK)->EnableWindow(TRUE);
	}
	m_pAScanView->m_pGateInfo[n].m_bOpen = m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_bOpen;
	m_pAScanView->InvalidateRect(m_pAScanView->m_rectWave, FALSE);
	switch(n)
	{
	case 0:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1Enable = m_bGateIsOpen;
		break;
	case 1:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2Enable = m_bGateIsOpen;
		break;
	case 2:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3Enable = m_bGateIsOpen;
		break;
	case 3:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4Enable = m_bGateIsOpen;
		break;
	default:
		break;
	}

	// 改变C扫面板控件状态
	ALLSCanItemEnableJudge();

	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckGateIsopen"),"Leave");
}
/** 
* 一通道单选按钮消息
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::OnBnClickedRadioCh1()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("OnBnClickedRadioCh1"),"Enter");

//	UpdateData(TRUE);

	DoChangeChannel();

// 	if(m_bExtendArrayFlag[m_nChannelNo])
// 	{
// 		RfExtendOff();
// 	}
// 	m_bExtendFlag = FALSE;
// 
// 	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("OnBnClickedRadioCh1"),"Leave");
}
/** 
* 二通道单选按钮消息
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::OnBnClickedRadioCh2()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedRadioCh2"),"Enter");

//	UpdateData(TRUE);

	DoChangeChannel();

// 	if(m_bExtendArrayFlag[m_nChannelNo])
// 	{
// 		RfExtendOff();
// 	}
// 	m_bExtendFlag = FALSE;
// 	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedRadioCh2"),"Leave");
}


/** 
* 设置闸门控件信息
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::SetGateInfo(void)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::SetGateInfo"),"Enter");

//	UpdateData(TRUE);
	CString str;
	int n = m_cmbGateNo.GetCurSel();
	
	m_bGateIsOpen = m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_bOpen;

	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_GATE_ISOPEN);

	if (pButton == NULL)
	{
		MessageBox(_T("CChannelPanel::SetGateInfo GetDlgItem IDC_CHECK_GATE_ISOPEN 失败"), _T("错误"), MB_OK|MB_ICONERROR);
		return;
	}

	if (n == 0)
	{		
		pButton->SetCheck(BST_CHECKED);
		pButton->EnableWindow(FALSE);
	}
	else
	{
		if(!g_bDrawRealTime)
		{
			pButton->EnableWindow(TRUE);
		}
	}

	if (!m_bGateIsOpen || m_bLockParam)//如果闸门是关闭状态则让滑块呈不可用状态
	{
		m_sliderGateFront.EnableWindow(FALSE);
		m_sliderGateBack.EnableWindow(FALSE);
		m_sliderGatePeak.EnableWindow(FALSE);
		m_sliderMTPPeak.EnableWindow(FALSE);

		GetDlgItem(IDC_SPIN_FRONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_GATE_PEAK)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_MTP_PEAK)->EnableWindow(FALSE);
	}
	else
	{
		m_sliderGateFront.EnableWindow(TRUE);
		m_sliderGateBack.EnableWindow(TRUE);
		m_sliderGatePeak.EnableWindow(TRUE);

		GetDlgItem(IDC_SPIN_FRONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_WIDTH)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_GATE_PEAK)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_MTP_PEAK)->EnableWindow(TRUE);

		if (n != 0)
		{
			m_sliderMTPPeak.EnableWindow(FALSE);
		}
		else
			m_sliderMTPPeak.EnableWindow(TRUE);
	}

	// 同步模式
	m_bMTPmode = m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_nMtpMode;
	// 闸门前沿
	m_fGateFront = m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_fFrontGate;
	str.Format(_T("%.1f"),m_fGateFront);
	m_fGateFront = _tstof(str);
	m_sliderGateFront.SetPos(int(m_fGateFront*10));

	// 闸门宽度
	m_fGateBack = m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_fBackGate;
	str.Format(_T("%.1f"),m_fGateBack);
	m_fGateBack = _tstof(str);
	m_sliderGateBack.SetPos(int(m_fGateBack*10));

	// 闸门阈值
	m_fGatePeak = m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_fPeakGate;
	str.Format(_T("%.1f"),m_fGatePeak);
	m_fGatePeak = _tstof(str);
	m_sliderGatePeak.SetPos(int(m_fGatePeak*10));

	// 闸门同步阈值
	m_fMTPPeak = m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_fMTPPeak;
	str.Format(_T("%.1f"),m_fMTPPeak);
	m_fMTPPeak = _tstof(str);
	m_sliderMTPPeak.SetPos(int(m_fMTPPeak*10));

	SetStaticColor(m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_colorGate);

//	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::SetGateInfo"),"Leave");
}

/** 
* 闸门combox控件切换选项时，同步改变并显示当前闸门信息
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::OnCbnSelchangeComboGateNo()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnCbnSelchangeComboGateNo"),"Enter");

	SetGateInfo();

	UpdateData(FALSE);
	
	// wangbingfu add start at 2012-11-19
	if(m_bExtendFlag)
	{
		RfExtendOff();	// 关闭扩展
	}
	// wangbingfu add end at 2012-11-21

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnCbnSelchangeComboGateNo"),"Leave");
}


/** 
* 设置闸门颜色按钮消息响应
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::OnBnClickedButtonGateColor()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedButtonGateColor"),"Enter");

	CColorDialog colorDlg;
	int n = m_cmbGateNo.GetCurSel();
	if (IDOK == colorDlg.DoModal())
	{
		COLORREF color = colorDlg.GetColor();
		m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_colorGate = color;	

		m_pAScanView->m_pGateInfo[n] = m_ChannelInfo[m_nChannelNo].m_GateInfo[n];

		m_pAScanView->InvalidateRect(m_pAScanView->m_rectWave, FALSE);
		SetStaticColor(color);
	
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedButtonGateColor"),"Leave");
}


/** 
* 设置静态框的背景颜色，表示当前闸门的颜色
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::SetStaticColor(COLORREF color)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::SetStaticColor"),"Enter");

	m_brush.DeleteObject();			//?
	m_brush.CreateSolidBrush(color);
	CRect rect;
	GetDlgItem(IDC_DIPLAYCOLOR)->GetClientRect(rect);
	GetDlgItem(IDC_DIPLAYCOLOR)->InvalidateRect(&rect);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::SetStaticColor"),"Leave");
}

void CChannelPanel::OnSize(UINT nType, int cx, int cy)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnSize"),"Enter");

	if(m_cmbGateNo.GetSafeHwnd() != NULL)
	{
		int n = m_cmbGateNo.GetCurSel();
		SetStaticColor(m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_colorGate);
	}
	
	CFormView::OnSize(nType, cx, cy);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnSize"),"Leave");
}
/** 
* 设置探头参数
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::SetProbeInfo(void)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::SetProbeInfo"),"Enter");

//	UpdateData(TRUE);

	//更新延迟
	CString str;
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
	m_fDelay = m_ChannelInfo[m_nChannelNo].m_fChannelDelay;
	str.Format(_T("%.1f"),m_fDelay);
	m_fDelay = _tstof(str);
	m_sliderDelay.SetPos(int(m_fDelay*10));

	//更新 范围
	int nMin = Dot2Range(m_nWaveSpeed, MONITOR_SIZE);// yachang.huang modify on 2014-05-19
	m_sliderRange.SetRange(nMin*10, A_SCAN_MAX_RANGE*10);
 	if (m_ChannelInfo[m_nChannelNo].m_fChannelRange < nMin)
 	{
 		m_ChannelInfo[m_nChannelNo].m_fChannelRange = nMin;
 	}
	int nPos = int(m_ChannelInfo[m_nChannelNo].m_fChannelRange * 10);
	m_sliderRange.SetPos(nPos);
	m_fRange = m_ChannelInfo[m_nChannelNo].m_fChannelRange;
	str.Format(_T("%.1f"),m_fRange);//为了保留一位小数
	m_fRange = _tstof(str);

	DoModifyRange();

	//更新增益
	m_fPlus = m_ChannelInfo[m_nChannelNo].m_fChannelPlus;
	str.Format(_T("%.1f"), m_fPlus);
	m_fPlus = _tstof(str);
	m_sliderPlus.SetPos(int(m_fPlus*10));

	// 更新参考增益
	m_fRefPlus = m_ChannelInfo[m_nChannelNo].m_fChannelRefPlus;
	str.Format(_T("%.1f"), m_fRefPlus);
	m_fRefPlus = _tstof(str);
	m_sldRefPlus.SetPos(int(m_fRefPlus*10));

	//更新抑制
	m_fRestrain = m_ChannelInfo[m_nChannelNo].m_fRestrain;
	m_sliderRestrain.SetPos(int(m_fRestrain*10));
	
	//更新零点
	m_fZeroPoint = m_ChannelInfo[m_nChannelNo].m_fZeroPoint;
	str.Format(_T("%.1f"),m_fZeroPoint);
	m_fZeroPoint = _tstof(str);
	m_sliderZeroPoint.SetPos(int(m_fZeroPoint * 10));

	//更新采样间隔
	m_comboChSI.SetCurSel(m_ChannelInfo[m_nChannelNo].m_nChSI);

	//更新采样平均数
	m_comboChSavg.SetCurSel(m_ChannelInfo[m_nChannelNo].m_nChSavg);

	// 重复频率
	m_nReFreq = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_wReFreq;
	m_sliderReFreq.SetPos(m_nReFreq);

	// 声速
	m_nWaveSpeed = m_ChannelInfo[m_nChannelNo].m_nWaveSpeed;

	// 探头频率
	int nIndexFreq = (int)m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eProbeFreq;
	m_cmbProbeFreq.SetCurSel(nIndexFreq);

	DWORD dwDelay = MicroSecond2DotChannel(m_fDelay);// yachang.huang modify on 2014-05-19
	m_pAScanView->m_fXMinRuler = Dot2RangeChannel100(m_nWaveSpeed, dwDelay);// yachang.huang modify on 2014-05-19
	m_pAScanView->m_fXMaxRuler = m_fRange + m_pAScanView->m_fXMinRuler;
	m_pAScanView->InvalidateRect(m_pAScanView->m_rectScaleX);

	m_pAScanView->InvalidateRect(m_pAScanView->m_rectWave, FALSE);

	// 激发电压
	m_ProbeInfoDlg.m_nPressureType = FindVoltageType(WORD(m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eInterruptVoltage));

	// 检波模式
	m_ProbeInfoDlg.m_nOscillationMode = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eDisplayMode;
	// 阻抗
	m_ProbeInfoDlg.m_nImpedanceType = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eImpedance;

	// 脉宽 
	m_ProbeInfoDlg.m_nMaiKuan = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_wPlusWidth;

	m_ProbeInfoDlg.m_nEnergy = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eEnergy;//yachang.huang add on 2013-11-25
	m_ProbeInfoDlg.m_nRevAndSendMode = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eRevAndSendMode;//yachang.huang add on 2013-11-25
	m_ProbeInfoDlg.m_nSpiAddr = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eSpiAddr;//yachang.huang add on 2013-11-25
	m_ProbeInfoDlg.m_nSpiData = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_nSpiData;//yachang.huang add on 2013-11-25

//	m_ProbeInfoDlg.m_nMidValue = m_ChannelInfo[m_nChannelNo].m_nMidValue;//yachang.huang add on 2013-11-27

	
//	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::SetProbeInfo"),"Leave");
}
/** 
* 同步模式之初始脉冲
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::OnBnClickedRadioInitpulse()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedRadioInitpulse"),"Enter");

	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();

	m_bMTPmode = FALSE;
	int n = m_cmbGateNo.GetCurSel();
	m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_nMtpMode = m_bMTPmode;
	m_pAScanView->m_pGateInfo[n].m_nMtpMode = m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_nMtpMode;
	switch(n)
	{
	case 0:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1SyncMode = m_bMTPmode;
		break;
	case 1:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2SyncMode = m_bMTPmode;
		break;
	case 2:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3SyncMode = m_bMTPmode;
		break;
	case 3:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4SyncMode = m_bMTPmode;
		break;
	default:
		break;
	}
	pFrame->m_bSetOne = TRUE;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedRadioInitpulse"),"Leave");
}
/** 
* 同步模式之首次回波
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::OnBnClickedRadioFirstecho()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedRadioFirstecho"),"Enter");

	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();

	m_bMTPmode = TRUE;
	int n = m_cmbGateNo.GetCurSel();
	m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_nMtpMode = m_bMTPmode;
	m_pAScanView->m_pGateInfo[n].m_nMtpMode = m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_nMtpMode;
	switch(n)
	{
	case 0:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1SyncMode = m_bMTPmode;
		break;
	case 1:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2SyncMode = m_bMTPmode;
		break;
	case 2:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3SyncMode = m_bMTPmode;
		break;
	case 3:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4SyncMode = m_bMTPmode;
		break;
	default:
		break;
	}
	pFrame->m_bSetOne = TRUE;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedRadioFirstecho"),"Leave");
}

/** 
* 通道采样间隔
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::OnCbnSelchangeComboChSi()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnCbnSelchangeComboChSi"),"Enter");

	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
	m_ChannelInfo[m_nChannelNo].m_nChSI = m_comboChSI.GetCurSel();

	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wSimpleInterval = m_ChannelInfo[m_nChannelNo].m_nChSI;
	pFrame->m_bSetOne = TRUE;

	// wanbingfu modify start on  2013-01-09
	/*	if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
	{
	int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

	memcpy(&m_ChannelInfo[nOtherChannelIndex], &m_ChannelInfo[m_nChannelNo], sizeof(CChannelInfo));
	memcpy(&pDoc->m_stAllCH_CONFIG[nOtherChannelIndex], &pDoc->m_stAllCH_CONFIG[m_nChannelNo], sizeof(CSCAN_ARS200FPGA_SETONE));
	}*/
	SetGroupOtherChannel();
	// wanbingfu modify end  2013-01-09

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnCbnSelchangeComboChSi"),"Leave");
}
/** 
* 通道采样平均数
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::OnCbnSelchangeComboChSavg()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnCbnSelchangeComboChSavg"),"Enter");

	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
	m_ChannelInfo[m_nChannelNo].m_nChSavg = m_comboChSavg.GetCurSel();

	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wSimpleAverage = m_ChannelInfo[m_nChannelNo].m_nChSavg;
	pFrame->m_bSetOne = TRUE;

	// wanbingfu modify start on  2013-01-09
	/*	if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
	{
	int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

	memcpy(&m_ChannelInfo[nOtherChannelIndex], &m_ChannelInfo[m_nChannelNo], sizeof(CChannelInfo));
	memcpy(&pDoc->m_stAllCH_CONFIG[nOtherChannelIndex], &pDoc->m_stAllCH_CONFIG[m_nChannelNo], sizeof(CSCAN_ARS200FPGA_SETONE));
	}*/
	SetGroupOtherChannel();
	// wanbingfu modify end  2013-01-09
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnCbnSelchangeComboChSavg"),"Leave");
}

/** 
* 初始化通道数据
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::InitChannelInfo()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::InitChannelInfo"),"Enter");

	UpdateData(TRUE);

	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();

	for (int i=0; i<CHANNEL_NUM; i++)
	{
		//闸门开关
		m_ChannelInfo[i].m_GateInfo[0].m_bOpen = pDoc->m_stAllCH_CONFIG[i].stGate.wGate1Enable;
		m_ChannelInfo[i].m_GateInfo[1].m_bOpen = pDoc->m_stAllCH_CONFIG[i].stGate.wGate2Enable;
		m_ChannelInfo[i].m_GateInfo[2].m_bOpen = pDoc->m_stAllCH_CONFIG[i].stGate.wGate3Enable;
		m_ChannelInfo[i].m_GateInfo[3].m_bOpen = pDoc->m_stAllCH_CONFIG[i].stGate.wGate4Enable;
		//闸门同步模式
		m_ChannelInfo[i].m_GateInfo[0].m_nMtpMode = pDoc->m_stAllCH_CONFIG[i].stGate.wGate1SyncMode;
		m_ChannelInfo[i].m_GateInfo[1].m_nMtpMode = pDoc->m_stAllCH_CONFIG[i].stGate.wGate2SyncMode;
		m_ChannelInfo[i].m_GateInfo[2].m_nMtpMode = pDoc->m_stAllCH_CONFIG[i].stGate.wGate3SyncMode;
		m_ChannelInfo[i].m_GateInfo[3].m_nMtpMode = pDoc->m_stAllCH_CONFIG[i].stGate.wGate4SyncMode;

		//闸门前沿
		m_ChannelInfo[i].m_GateInfo[0].m_fFrontGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[i].stGate.wGate1Start);
		m_ChannelInfo[i].m_GateInfo[1].m_fFrontGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[i].stGate.wGate2Start);// yachang.huang modify on 2014-05-19
		m_ChannelInfo[i].m_GateInfo[2].m_fFrontGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[i].stGate.wGate3Start);// yachang.huang modify on 2014-05-19
		m_ChannelInfo[i].m_GateInfo[3].m_fFrontGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[i].stGate.wGate4Start);// yachang.huang modify on 2014-05-19
		//闸门宽度
		m_ChannelInfo[i].m_GateInfo[0].m_fBackGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[i].stGate.wGate1Width);// yachang.huang modify on 2014-05-19
		m_ChannelInfo[i].m_GateInfo[1].m_fBackGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[i].stGate.wGate2Width);// yachang.huang modify on 2014-05-19
		m_ChannelInfo[i].m_GateInfo[2].m_fBackGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[i].stGate.wGate3Width);// yachang.huang modify on 2014-05-19
		m_ChannelInfo[i].m_GateInfo[3].m_fBackGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[i].stGate.wGate4Width);// yachang.huang modify on 2014-05-19
		//闸门阈值
		m_ChannelInfo[i].m_GateInfo[0].m_fPeakGate = pDoc->m_stAllCH_CONFIG[i].stGate.wGate1Amp / MAX_HEIGHT * 100;
		m_ChannelInfo[i].m_GateInfo[1].m_fPeakGate = pDoc->m_stAllCH_CONFIG[i].stGate.wGate2Amp / MAX_HEIGHT * 100;
		m_ChannelInfo[i].m_GateInfo[2].m_fPeakGate = pDoc->m_stAllCH_CONFIG[i].stGate.wGate3Amp / MAX_HEIGHT * 100;
		m_ChannelInfo[i].m_GateInfo[3].m_fPeakGate = pDoc->m_stAllCH_CONFIG[i].stGate.wGate4Amp / MAX_HEIGHT * 100;
		//闸门同步阈值
		m_ChannelInfo[i].m_GateInfo[0].m_fMTPPeak = pDoc->m_stAllCH_CONFIG[i].stGate.wGate1SyncThreshold / MAX_HEIGHT * 100;
		m_ChannelInfo[i].m_GateInfo[1].m_fMTPPeak = pDoc->m_stAllCH_CONFIG[i].stGate.wGate2SyncThreshold / MAX_HEIGHT * 100;
		m_ChannelInfo[i].m_GateInfo[2].m_fMTPPeak = pDoc->m_stAllCH_CONFIG[i].stGate.wGate3SyncThreshold / MAX_HEIGHT * 100;
		m_ChannelInfo[i].m_GateInfo[3].m_fMTPPeak = pDoc->m_stAllCH_CONFIG[i].stGate.wGate4SyncThreshold / MAX_HEIGHT * 100;

		//延迟
		m_ChannelInfo[i].m_fChannelDelay = int(Dot2MicroSecondChannel(pDoc->m_stAllCH_CONFIG[i].stGate.wDelay) * 10 + 0.5)/10.0f;// yachang.huang modify on 2014-05-19

		//范围
		int nMin = Dot2RangeChannel(m_nWaveSpeed, MONITOR_SIZE);// yachang.huang modify on 2014-05-19
		if (m_ChannelInfo[i].m_fChannelRange < nMin)
		{
			m_ChannelInfo[i].m_fChannelRange = nMin;
		}

		// 增益
//		m_ChannelInfo[i].m_fChannelPlus = Data2Gain(pDoc->m_stAllCH_CONFIG[i].stConfig.wdBLevel2 + pDoc->m_stAllCH_CONFIG[i].stConfig.wdBLevel3);
		m_ChannelInfo[i].m_fChannelPlus = pDoc->m_stAllCH_CONFIG[i].fPlus;

		// 参考增益
		m_ChannelInfo[i].m_fChannelRefPlus = pDoc->m_stAllCH_CONFIG[i].fRefPlus;

		// 抑制
		m_ChannelInfo[i].m_fRestrain = pDoc->m_stAllCH_CONFIG[i].stConfig.wSuspend;
		// 零点
		m_ChannelInfo[i].m_fZeroPoint = int(Dot2MicroSecondChannel100(pDoc->m_stAllCH_CONFIG[i].stConfig.wMonitorZeroCounter) * 10 + 0.5)/10.0f;// yachang.huang modify on 2014-05-19

		// 采样间隔
		m_ChannelInfo[i].m_nChSI = pDoc->m_stAllCH_CONFIG[i].stGate.wSimpleInterval;

		// 采样平均数
		m_ChannelInfo[i].m_nChSavg = pDoc->m_stAllCH_CONFIG[i].stGate.wSimpleAverage;

		// 重复频率
		m_ChannelInfo[i].m_ProbeInfo.m_wReFreq = CalcRepeateCounterReverse(int(pDoc->m_stAllCH_CONFIG[i].stCon.wRepeatCounter));

		// 声速
		m_ChannelInfo[i].m_nWaveSpeed = pDoc->m_stAllCH_CONFIG[i].nVelocity;

		// 探头频率
		m_ChannelInfo[i].m_ProbeInfo.m_eProbeFreq = PROBE_FREQ_e(pDoc->m_stAllCH_CONFIG[i].wProbeBand);
		// 激发电压
		m_ChannelInfo[i].m_ProbeInfo.m_eInterruptVoltage = FindVoltageType(pDoc->m_stAllCH_CONFIG[i].wVoltage);
		// 检波模式
		m_ChannelInfo[i].m_ProbeInfo.m_eDisplayMode = DETECT_OSCILLATION_MODE_e(pDoc->m_stAllCH_CONFIG[i].stConfig.wDetectMode);
		// 阻抗
		m_ChannelInfo[i].m_ProbeInfo.m_eImpedance = IMPEDANCE_e(pDoc->m_stAllCH_CONFIG[i].wImpedance);
		// 脉宽
		m_ChannelInfo[i].m_ProbeInfo.m_wPlusWidth = pDoc->m_stAllCH_CONFIG[i].stCon.wMaiKuan * (1000 / ADFREQUENCY);
        //能量
        m_ChannelInfo[i].m_ProbeInfo.m_eEnergy =  (ENERGY_e)pDoc->m_stAllCH_CONFIG[i].wEnerge;//yachang.huang add on 2013-11-26
        //收发模式
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eRevAndSendMode = (RevAndSendMode_e)pDoc->m_stAllCH_CONFIG[i].wRevAndSendMode;//yachang.huang add on 2013-11-26
        //SPI地址
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eSpiAddr = (SPI_ADDR_e)pDoc->m_stAllCH_CONFIG[i].wSPIAddr;//yachang.huang add on 2013-11-26
        //SPI数据
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_nSpiData = (SPI_ADDR_e)pDoc->m_stAllCH_CONFIG[i].wSPIData;//yachang.huang add on 2013-11-26	
	}	

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::InitChannelInfo"),"Leave");
}

/** 
* 通道数据转换为向下位机发送的数据
* @param     void                                         
* @return    void
* @since     1.0
*/
void CChannelPanel::ChannelInfoToData()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::ChannelInfoToData"),"Enter");

	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
	CString str;

	for (int i=0; i<CHANNEL_NUM; i++)
	{
		//闸门开关
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate1Enable = m_ChannelInfo[i].m_GateInfo[0].m_bOpen;
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate2Enable = m_ChannelInfo[i].m_GateInfo[1].m_bOpen;
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate3Enable = m_ChannelInfo[i].m_GateInfo[2].m_bOpen;
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate4Enable = m_ChannelInfo[i].m_GateInfo[3].m_bOpen;
		//闸门同步模式
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate1SyncMode = m_ChannelInfo[i].m_GateInfo[0].m_nMtpMode;
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate2SyncMode = m_ChannelInfo[i].m_GateInfo[1].m_nMtpMode;
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate3SyncMode = m_ChannelInfo[i].m_GateInfo[2].m_nMtpMode;
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate4SyncMode = m_ChannelInfo[i].m_GateInfo[3].m_nMtpMode;

		//闸门前沿
		DWORD dwDelay[4] = {0, 0, 0, 0};
		for (int j=0; j<4; j++)
		{
			if (!m_ChannelInfo[i].m_GateInfo[j].m_nMtpMode)
			{
				dwDelay[j] = MicroSecond2DotChannel(m_ChannelInfo[i].m_fChannelDelay);// yachang.huang modify on 2014-05-19
			}
			else
				dwDelay[j] = 0;
		}
	
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate1Start = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[i].m_GateInfo[0].m_fFrontGate) - dwDelay[0];// yachang.huang modify on 2014-05-19
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate2Start = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[i].m_GateInfo[1].m_fFrontGate) - dwDelay[1];// yachang.huang modify on 2014-05-19
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate3Start = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[i].m_GateInfo[2].m_fFrontGate) - dwDelay[2];// yachang.huang modify on 2014-05-19
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate4Start = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[i].m_GateInfo[3].m_fFrontGate) - dwDelay[3];// yachang.huang modify on 2014-05-19

		//闸门宽度
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate1Width = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[i].m_GateInfo[0].m_fBackGate);// yachang.huang modify on 2014-05-19
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate2Width = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[i].m_GateInfo[1].m_fBackGate);// yachang.huang modify on 2014-05-19
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate3Width = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[i].m_GateInfo[2].m_fBackGate);// yachang.huang modify on 2014-05-19
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate4Width = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[i].m_GateInfo[3].m_fBackGate);// yachang.huang modify on 2014-05-19
		//闸门阈值
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate1Amp = m_ChannelInfo[i].m_GateInfo[0].m_fPeakGate / 100.0f * MAX_HEIGHT;
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate2Amp = m_ChannelInfo[i].m_GateInfo[1].m_fPeakGate / 100.0f * MAX_HEIGHT;
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate3Amp = m_ChannelInfo[i].m_GateInfo[2].m_fPeakGate / 100.0f * MAX_HEIGHT;
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate4Amp = m_ChannelInfo[i].m_GateInfo[3].m_fPeakGate / 100.0f * MAX_HEIGHT;
		//闸门同步阈值
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate1SyncThreshold = m_ChannelInfo[i].m_GateInfo[0].m_fMTPPeak / 100.0f * MAX_HEIGHT;
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate2SyncThreshold = m_ChannelInfo[i].m_GateInfo[1].m_fMTPPeak / 100.0f * MAX_HEIGHT;
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate3SyncThreshold = m_ChannelInfo[i].m_GateInfo[2].m_fMTPPeak / 100.0f * MAX_HEIGHT;
		pDoc->m_stAllCH_CONFIG[i].stGate.wGate4SyncThreshold = m_ChannelInfo[i].m_GateInfo[3].m_fMTPPeak / 100.0f * MAX_HEIGHT;

		//延迟
		pDoc->m_stAllCH_CONFIG[i].stGate.wDelay = MicroSecond2DotChannel(m_ChannelInfo[i].m_fChannelDelay);// yachang.huang modify on 2014-05-19

		//范围


		pDoc->m_stAllCH_CONFIG[i].fPlus = m_ChannelInfo[i].m_fChannelPlus;
		pDoc->m_stAllCH_CONFIG[i].fRefPlus = m_ChannelInfo[i].m_fChannelRefPlus;

		// 零点
		pDoc->m_stAllCH_CONFIG[i].stConfig.wMonitorZeroCounter = MicroSecond2DotChannel100(m_ChannelInfo[i].m_fZeroPoint);// yachang.huang modify on 2014-05-19

		//抑制
		pDoc->m_stAllCH_CONFIG[i].stConfig.wSuspend = m_ChannelInfo[i].m_fRestrain / 100.0f * MAX_HEIGHT;;

		// 采样间隔
		pDoc->m_stAllCH_CONFIG[i].stGate.wSimpleInterval = m_ChannelInfo[i].m_nChSI;

		// 采样平均数
		pDoc->m_stAllCH_CONFIG[i].stGate.wSimpleAverage = m_ChannelInfo[i].m_nChSavg;	

		// 重复频率
		pDoc->m_stAllCH_CONFIG[i].stCon.wRepeatCounter = CalcRepeateCounter(m_ChannelInfo[i].m_ProbeInfo.m_wReFreq);
		// 声速
		pDoc->m_stAllCH_CONFIG[i].nVelocity = m_ChannelInfo[i].m_nWaveSpeed;
		// 探头频率
		pDoc->m_stAllCH_CONFIG[i].wProbeBand = (WORD)m_ChannelInfo[i].m_ProbeInfo.m_eProbeFreq;	
		pDoc->m_stAllCH_CONFIG[i].fPlusAddX11 = pDoc->fPlusEqualize[pDoc->m_stAllCH_CONFIG[i].wProbeBand][0];
		pDoc->m_stAllCH_CONFIG[i].fPlusAddX12 = pDoc->fPlusEqualize[pDoc->m_stAllCH_CONFIG[i].wProbeBand][1];
		pDoc->m_stAllCH_CONFIG[i].fPlusAddX21 = pDoc->fPlusEqualize[pDoc->m_stAllCH_CONFIG[i].wProbeBand][2];
		pDoc->m_stAllCH_CONFIG[i].fPlusAddX22 = pDoc->fPlusEqualize[pDoc->m_stAllCH_CONFIG[i].wProbeBand][3];

		// 激发电压
		switch (m_ChannelInfo[i].m_ProbeInfo.m_eInterruptVoltage)
		{
		case TYPE_50:
			pDoc->m_stAllCH_CONFIG[i].wVoltage = 50;
			break;
		case TYPE_400:
			pDoc->m_stAllCH_CONFIG[i].wVoltage = 400;
			break;
		default:
			pDoc->m_stAllCH_CONFIG[i].wVoltage = 50;
			break;
		}		
		// 检波模式
		pDoc->m_stAllCH_CONFIG[i].stConfig.wDetectMode = m_ChannelInfo[i].m_ProbeInfo.m_eDisplayMode;
		// 阻抗
		pDoc->m_stAllCH_CONFIG[i].wImpedance = m_ChannelInfo[i].m_ProbeInfo.m_eImpedance;
		// 脉宽
		pDoc->m_stAllCH_CONFIG[i].stCon.wMaiKuan = WORD(m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_wPlusWidth / (1000.0 / ADFREQUENCY));

		pDoc->m_stAllCH_CONFIG[i].wEnerge = m_ChannelInfo[i].m_ProbeInfo.m_eEnergy;  //能量 yachang.huang add on 2013-11-25
		pDoc->m_stAllCH_CONFIG[i].wRevAndSendMode = m_ChannelInfo[i].m_ProbeInfo.m_eRevAndSendMode;//收发模式  yachang.huang add on 2013-11-25
		pDoc->m_stAllCH_CONFIG[i].wSPIAddr = m_ChannelInfo[i].m_ProbeInfo.m_eSpiAddr;//SPI地址 yachang.huang add on 2013-11-25
		pDoc->m_stAllCH_CONFIG[i].wSPIData = m_ChannelInfo[i].m_ProbeInfo.m_nSpiData;//SPI数据 yachang.huang add on 2013-11-25
//		pDoc->m_stAllCH_CONFIG[i].stConfig.wValueMiddle = m_ChannelInfo[i].m_nMidValue;//中值 yachang.huang add on 2013-11-27
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::ChannelInfoToData"),"Leave");
}

//前沿微调按钮
void CChannelPanel::OnDeltaposSpinFront(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinFront"),"Enter");

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int n = m_cmbGateNo.GetCurSel();
	m_fGateFront -= pNMUpDown->iDelta/10.0f;
	DoModifyGateFront();

	m_sliderGateFront.SetPos(int(m_fGateFront * 10));

	*pResult = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinFront"),"Leave");
}
//宽度微调按钮
void CChannelPanel::OnDeltaposSpinWidth(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinWidth"),"Enter");

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_fGateBack -= pNMUpDown->iDelta/10.0f;
	DoModifyGateWidth();

	m_sliderGateBack.SetPos(int(m_fGateBack * 10));
	*pResult = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinWidth"),"Leave");
}

//闸门阈值微调按钮
void CChannelPanel::OnDeltaposSpinGatePeak(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinGatePeak"),"Enter");

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_fGatePeak -= pNMUpDown->iDelta/10.0f;

	DoModifyGatePeak();

	m_sliderGatePeak.SetPos(int(m_fGatePeak * 10));

	*pResult = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinGatePeak"),"Leave");
}
//同步阈值
void CChannelPanel::OnDeltaposSpinMtpPeak(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinMtpPeak"),"Enter");

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_fMTPPeak -= pNMUpDown->iDelta/10.0f;

	DoModifyMTPPeak();
	m_sliderMTPPeak.SetPos(int(m_fMTPPeak * 10));
	*pResult = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinMtpPeak"),"Leave");
}
//零点微调按钮
void CChannelPanel::OnDeltaposSpinZeropoint(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinZeropoint"),"Enter");

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_fZeroPoint -= pNMUpDown->iDelta/10.0f;
	
	DoModifyZeroPoint();
	m_sliderZeroPoint.SetPos(int(m_fZeroPoint * 10));

	*pResult = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinZeropoint"),"Leave");
}

//延迟微调按钮
void CChannelPanel::OnDeltaposSpinDelay(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinDelay"),"Enter");

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_fDelay -= pNMUpDown->iDelta/10.0f;
	
	DoModifyDelay();

	m_sliderDelay.SetPos(int(m_fDelay * 10));

//	UpdateData(FALSE);
	*pResult = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinDelay"),"Leave");
}
//范围微调按钮
void CChannelPanel::OnDeltaposSpinRange(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinRange"),"Enter");

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_fRange -= pNMUpDown->iDelta/10.0f;
	
	DoModifyRange();	

	m_sliderRange.SetPos(int(m_fRange * 10));
	*pResult = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinRange"),"Leave");
}

//增益微调按钮
void CChannelPanel::OnDeltaposSpinPlus(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinPlus"),"Enter");

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_fPlus -= pNMUpDown->iDelta/10.0f;
	
	DoModifyPlus();

	m_sliderPlus.SetPos(int(m_fPlus * 10));
	*pResult = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinPlus"),"Leave");
}
//抑制微调按钮
void CChannelPanel::OnDeltaposSpinRestrain(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinRestrain"),"Enter");

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_fRestrain -= pNMUpDown->iDelta/10.0f;
	
	DoModifyRestain();
	m_sliderRestrain.SetPos(int(m_fRestrain * 10));

	*pResult = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinRestrain"),"Leave");
}

void CChannelPanel::OnBnChickedOtherParam()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnChickedOtherParam"),"Enter");

	m_ProbeInfoDlg.m_wChannelNo = m_nChannelNo;
	m_ProbeInfoDlg.m_nProbeFrequency = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eProbeFreq;
	m_ProbeInfoDlg.m_nPressureType = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eInterruptVoltage;
	m_ProbeInfoDlg.m_nOscillationMode = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eDisplayMode;
	m_ProbeInfoDlg.m_nImpedanceType = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eImpedance;
	m_ProbeInfoDlg.m_fLvBoHigh = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_fLvBoHigh;
	m_ProbeInfoDlg.m_fLvBoLow = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_fLvBoLow;
	m_ProbeInfoDlg.m_nLvBoType = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eLvBoType;
	m_ProbeInfoDlg.m_nMaiKuan = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_wPlusWidth;
	m_ProbeInfoDlg.m_wWorkMode = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_wWorkMode;
	m_ProbeInfoDlg.m_nEnergy = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eEnergy;//yachang.huang add on 2013-11-25
	m_ProbeInfoDlg.m_nRevAndSendMode = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eRevAndSendMode;//yachang.huang add on 2013-11-25
	m_ProbeInfoDlg.m_nSpiAddr = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eSpiAddr;//yachang.huang add on 2013-11-25
	m_ProbeInfoDlg.m_nSpiData = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_nSpiData;//yachang.huang add on 2013-11-25
//	m_ProbeInfoDlg.m_nMidValue = m_ChannelInfo[m_nChannelNo].m_nMidValue;//yachang.huang add on 2013-11-27
	int nChannelNo = m_nChannelNo % 2;
	// 下位机1通道时0为高1为低，2通道时2为高0为低
	if(nChannelNo == 0)
	{
		if(m_ProbeInfoDlg.m_nImpedanceType == 0)
		{
			m_ProbeInfoDlg.m_nImpedanceType = 1;
		}
		else if(m_ProbeInfoDlg.m_nImpedanceType == 1)
		{
			m_ProbeInfoDlg.m_nImpedanceType = 0;
		}
	}
	else if(nChannelNo == 1)
	{
		if(m_ProbeInfoDlg.m_nImpedanceType == 2)
		{
			m_ProbeInfoDlg.m_nImpedanceType = 1;
		}
	}
	else
	{
		;//
	}

	if(m_ProbeInfoDlg.DoModal() == IDOK)
	{
		// 通道参数赋值
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eProbeFreq = (PROBE_FREQ_e)m_ProbeInfoDlg.m_nProbeFrequency;
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eInterruptVoltage = (VOLTAGE_TYPE_e)m_ProbeInfoDlg.m_nPressureType;
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eDisplayMode = (DETECT_OSCILLATION_MODE_e)m_ProbeInfoDlg.m_nOscillationMode;
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eImpedance = (IMPEDANCE_e)m_ProbeInfoDlg.m_nImpedanceType;
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_fLvBoHigh = m_ProbeInfoDlg.m_fLvBoHigh;
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_fLvBoLow = m_ProbeInfoDlg.m_fLvBoLow;
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eLvBoType = (LVBO_TYPE_e)m_ProbeInfoDlg.m_nLvBoType;
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_wPlusWidth = m_ProbeInfoDlg.m_nMaiKuan;
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_wWorkMode = m_ProbeInfoDlg.m_wWorkMode;
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eEnergy = (ENERGY_e)m_ProbeInfoDlg.m_nEnergy;//yachang.huang add on 2013-11-25
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eRevAndSendMode = (RevAndSendMode_e)m_ProbeInfoDlg.m_nRevAndSendMode;//yachang.huang add on 2013-11-25
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eSpiAddr = (SPI_ADDR_e)m_ProbeInfoDlg.m_nSpiAddr;//yachang.huang add on 2013-11-25
		m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_nSpiData = (SPI_ADDR_e)m_ProbeInfoDlg.m_nSpiData;//yachang.huang add on 2013-11-25	
//		m_ChannelInfo[m_nChannelNo].m_nMidValue = m_ProbeInfoDlg.m_nMidValue;//yachang.huang add on 2013-11-27	
		// 下位机1通道时0为高1为低，2通道时2为高0为低
		if(nChannelNo == 0)
		{
			if(m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eImpedance == (IMPEDANCE_e)0)
			{
				m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eImpedance = (IMPEDANCE_e)1;
			}
			else if(m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eImpedance == (IMPEDANCE_e)1)
			{
				m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eImpedance = (IMPEDANCE_e)0;
			}
			else
			{
				;//
			}
		}
		else if(nChannelNo == 1)
		{
			if(m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eImpedance == (IMPEDANCE_e)1)
			{
				m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eImpedance = (IMPEDANCE_e)2;
			}
		}
		else
		{
			;//
		}

		// 将参数下发到硬件
		CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].wProbeBand = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eProbeFreq;		//探头频率
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].fPlusAddX11 = pDoc->fPlusEqualize[pDoc->m_stAllCH_CONFIG[m_nChannelNo].wProbeBand][0];
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].fPlusAddX12 = pDoc->fPlusEqualize[pDoc->m_stAllCH_CONFIG[m_nChannelNo].wProbeBand][1];
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].fPlusAddX21 = pDoc->fPlusEqualize[pDoc->m_stAllCH_CONFIG[m_nChannelNo].wProbeBand][2];
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].fPlusAddX22 = pDoc->fPlusEqualize[pDoc->m_stAllCH_CONFIG[m_nChannelNo].wProbeBand][3];
		
		switch (m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eInterruptVoltage)	// 电压
		{
		case TYPE_50:
			pDoc->m_stAllCH_CONFIG[m_nChannelNo].wVoltage = 50;
			break;
		case TYPE_400:
			pDoc->m_stAllCH_CONFIG[m_nChannelNo].wVoltage = 400;
			break;
		default:
			pDoc->m_stAllCH_CONFIG[m_nChannelNo].wVoltage = 50;
			break;
		}
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].wImpedance = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eImpedance;		// 阻抗
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stConfig.wDetectMode = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eDisplayMode;	// 检波模式
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stCon.wMaiKuan = WORD(m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_wPlusWidth / (1000.0 / ADFREQUENCY));	// 脉宽

		pDoc->m_stAllCH_CONFIG[m_nChannelNo].wEnerge = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eEnergy;  //能量 yachang.huang add on 2013-11-25
        pDoc->m_stAllCH_CONFIG[m_nChannelNo].wRevAndSendMode = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eRevAndSendMode;//收发模式  yachang.huang add on 2013-11-25
        pDoc->m_stAllCH_CONFIG[m_nChannelNo].wSPIAddr = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eSpiAddr;//SPI地址 yachang.huang add on 2013-11-25
        pDoc->m_stAllCH_CONFIG[m_nChannelNo].wSPIData = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_nSpiData;//SPI数据 yachang.huang add on 2013-11-25
//        pDoc->m_stAllCH_CONFIG[m_nChannelNo].stConfig.wValueMiddle = m_ChannelInfo[m_nChannelNo].m_nMidValue;//中值 yachang.huang add on 2013-11-27

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->m_bSetOne = TRUE;

		SetGroupOtherChannel();
		// wanbingfu modify end  2013-01-09
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnChickedOtherParam"),"Leave");
}

CChannelInfo CChannelPanel::GetChannelInfo(const WORD& wChannelNo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::GetChannelInfo"),"Enter");

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::GetChannelInfo"),"Leave");
	return m_ChannelInfo[wChannelNo];
}

// 保存工艺参数
void CChannelPanel::OnBnClickedButtonSaveAsParam()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedButtonSaveAsParam"),"Enter");

	// TODO: Add your control notification handler code here
	TCHAR szFilters[] =
		_T("NDT Instrument Parameter files (*.par)|*.par|All files (*.*)|*.*||");

	CFileDialog dlg(FALSE, _T("par"), NULL, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,	szFilters);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();

		SaveParam(strPath);

		m_strParamFilePath = strPath;
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedButtonSaveAsParam"),"Leave");
}

void CChannelPanel::SaveParam(const CString& strFilePath)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::SaveParam"),"Enter");

	CFile file;
	CFileException e;

	if (file.Open(strFilePath, CFile::modeReadWrite | CFile::modeCreate), &e)
	{			
		file.Write(m_ChannelInfo, sizeof(CChannelInfo)* CHANNEL_NUM);
		file.Close();
	}
	else
	{
		e.ReportError();
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::SaveParam"),"Leave");
}

void CChannelPanel::ReadParam(const CString& strFilePath)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::ReadParam"),"Enter");

	CFile file;
	CFileException e;

	if (file.Open(strFilePath, CFile::modeRead), &e)
	{
		CChannelInfo channelInfos[CHANNEL_NUM];
		file.Read(channelInfos, sizeof(CChannelInfo) * CHANNEL_NUM);
		file.Close();

		for (int i=0; i<CHANNEL_NUM; i++)
		{
			m_ChannelInfo[i] = channelInfos[i];
		}

		// 更新通道面板
		SetGateInfo();//更新闸门参数
		SetProbeInfo();//更新探头参数

		ChannelInfoToData();

		//将修改过后的闸门信息同步显示到A扫视图区域
		for(int i=0; i<4; i++)
		{		
			m_pAScanView->m_pGateInfo[i] = m_ChannelInfo[m_nChannelNo].m_GateInfo[i];
		}

		CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
		m_pAScanView->m_fXMinRuler = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wDelay);// yachang.huang modify on 2014-05-19
		m_pAScanView->m_fXMaxRuler = m_fRange + m_pAScanView->m_fXMinRuler;

		m_pAScanView->InvalidateRect(&m_pAScanView->m_rectScaleX);
		m_pAScanView->InvalidateRect(m_pAScanView->m_rectWave, FALSE);
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

		// 使参数生效
		CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
		int nAECCardNum = pApp->GetCardNum();
		const int ALL_PARAM_SIZE = sizeof(CSCAN_ARS200FPGA_SETONE);
		for (int i=0;i < nAECCardNum ;i++)
		{
			for (int j = 0; j<2; j++)
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

				pFrame->SendData( szSendData, sizeof(szSendData), nChannelIndex);
			}
		}

		pFrame->m_bSetOne = TRUE;

		UpdateData(FALSE);
		// 改变C扫面板控件状态
		ALLSCanItemEnableJudge();

		m_strParamFilePath = strFilePath;	
	}
	else
	{
		e.ReportError();
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::ReadParam"),"Leave");
}

// 读取工艺参数
void CChannelPanel::OnBnClickedButtonReadParam()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedButtonReadParam"),"Enter");

	// TODO: Add your control notification handler code here
	// 读文件
	TCHAR szFilters[] =
		_T("NDT Instrument Parameter files (*.par)|*.par|All files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("par"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();

		CFile file;
		CFileException e;
		
		if (file.Open(strPath, CFile::modeReadWrite), &e)
		{
			CChannelInfo channelInfos[CHANNEL_NUM];
			file.Read(channelInfos, sizeof(CChannelInfo) * CHANNEL_NUM);
			file.Close();

			int nRet = MessageBox(_T("当前数据将被覆盖，同意点“是”，否则点“否”。"), _T("提示"), MB_YESNO);

			if (nRet == IDYES)
			{
				for (int i=0; i<CHANNEL_NUM; i++)
				{
					m_ChannelInfo[i] = channelInfos[i];
				}

				// 更新通道面板
				SetGateInfo();//更新闸门参数
				SetProbeInfo();//更新探头参数
				
				ChannelInfoToData();

				//将修改过后的闸门信息同步显示到A扫视图区域
				for(int i=0; i<4; i++)
				{		
					m_pAScanView->m_pGateInfo[i] = m_ChannelInfo[m_nChannelNo].m_GateInfo[i];
				}

				CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
				m_pAScanView->m_fXMinRuler = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wDelay);// yachang.huang modify on 2014-05-19
				m_pAScanView->m_fXMaxRuler = m_fRange + m_pAScanView->m_fXMinRuler;

				m_pAScanView->InvalidateRect(&m_pAScanView->m_rectScaleX);
				m_pAScanView->InvalidateRect(m_pAScanView->m_rectWave, FALSE);
				CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			
				// 使参数生效
				CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
				int nAECCardNum = pApp->GetCardNum();
				const int ALL_PARAM_SIZE = sizeof(CSCAN_ARS200FPGA_SETONE);
				for (int i=0;i < nAECCardNum ;i++)
				{
					for (int j = 0; j<2; j++)
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
						
						pFrame->SendData( szSendData, sizeof(szSendData), nChannelIndex);
					}
				}
				
				pFrame->m_bSetOne = TRUE;

				UpdateData(FALSE);
				// 改变C扫面板控件状态
				ALLSCanItemEnableJudge();

				m_strParamFilePath = strPath;
			}
		}
		else
		{
			e.ReportError();
		}
	}
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedButtonReadParam"),"Leave");
}

void CChannelPanel::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnHScroll"),"Enter");

	// TODO: Add your message handler code here and/or call default
	if (nSBCode == TB_THUMBTRACK && pScrollBar != NULL)
	{
		UpdateData(TRUE);

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
		int nCtrlID = pScrollBar->GetDlgCtrlID();

		if (nCtrlID == IDC_SLIDER_GATE_FRONT)	// 闸门前沿
		{			
			m_fGateFront = m_sliderGateFront.GetPos()*0.1;
			DoModifyGateFront();
		}
		else if (nCtrlID == IDC_SLIDER_GATE_BACK)	// 闸门宽度
		{
			m_fGateBack = m_sliderGateBack.GetPos()*0.1;
			DoModifyGateWidth();
		} 
		else if (nCtrlID == IDC_SLIDER_GATE_PEAK)	// 闸门高度
		{			
			m_fGatePeak = m_sliderGatePeak.GetPos()*0.1;
			DoModifyGatePeak();
		} 
		else if (nCtrlID == IDC_SLIDER_MTP_PEAK)	// 同步阈值
		{
			m_fMTPPeak = m_sliderMTPPeak.GetPos()*0.1;
			DoModifyMTPPeak();
		} 
		else if (nCtrlID == IDC_SLIDER_ZEROPOINT)	// 零点
		{
			m_fZeroPoint = m_sliderZeroPoint.GetPos()*0.1;

			DoModifyZeroPoint();
		} 
		else if (nCtrlID == IDC_SLIDER_DELAY)	// 延迟
		{
			m_fDelay = m_sliderDelay.GetPos()*0.1;		
			
			DoModifyDelay();
		} 
		else if (nCtrlID == IDC_SLIDER_RANGE)	// 范围
		{
			m_fRange = m_sliderRange.GetPos()*0.1;

// 			BOOL bRet = IsInSafeRange(m_fZeroPoint, m_fDelay, m_fRange, 2);
// 
// 			if (!bRet)
//				m_sliderRange.SetPos(int(m_fRange * 10));

			DoModifyRange();
		} 
		else if (nCtrlID == IDC_SLIDER_PLUS)	// 增益
		{
			m_fPlus = m_sliderPlus.GetPos()*0.1;
			DoModifyPlus();
		} 
		// Song Chenguang add start on 2012-07-19
		else if (nCtrlID == IDC_SLIDER_REF_PLUS)
		{
			m_fRefPlus = m_sldRefPlus.GetPos() * 0.1;
			DoModifyRefPlus();
		}
		// Song Chenguang add end on 2012-07-19
		else if(nCtrlID == IDC_SLIDER_RESTRAIN)//抑制
		{
			m_fRestrain = m_sliderRestrain.GetPos()/10.0;

			DoModifyRestain();
		}
		else if (nCtrlID == IDC_SLIDER_RE_FREQ)
		{
			m_nReFreq = m_sliderReFreq.GetPos();

//			DoModifyReFreq();
		}

	}	

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnHScroll"),"Leave");
}

void CChannelPanel::OnEnKillfocusEditWaveSpeed()
{
	// TODO: Add your control notification handler code here
	// DongQi add start on 2013-06-27
	if (FALSE == UpdateData(TRUE))
	{
		return;
	}

	m_ChannelInfo[m_nChannelNo].m_nWaveSpeed = m_nWaveSpeed;
	g_nWaveSpeed = m_nWaveSpeed;

	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].nVelocity = m_nWaveSpeed;

	BOOL bFind = FALSE;
	for (int i=0; i<MATERIAL_NUM; i++)
	{
		if (m_nWaveSpeed == m_MaterialSoundSpeeds[i].m_wSoundSpeed)
		{
			m_cmbMaterial.SetCurSel(i);
			bFind = TRUE;
		}
	}

	if (!bFind)
		m_cmbMaterial.SetCurSel(11);	// 自定义
	// 重新设一下参数
	SetGateInfo();
	SetProbeInfo();
	m_pAScanView->InvalidateRect(m_pAScanView->m_rectWave, FALSE);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_bSetOne = TRUE;
	SetGroupOtherChannel();
	// DongQi add end on 2013-06-27
}

void CChannelPanel::OnCbnSelchangeComboProbeFreq()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnCbnSelchangeComboProbeFreq"),"Enter");

	// TODO: Add your control notification handler code here
	int nIndex = m_cmbProbeFreq.GetCurSel();

	// 切换到对应频率
	m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eProbeFreq = PROBE_FREQ_e(nIndex);

	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].wProbeBand = nIndex;
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].fPlusAddX11 = pDoc->fPlusEqualize[pDoc->m_stAllCH_CONFIG[m_nChannelNo].wProbeBand][0];
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].fPlusAddX12 = pDoc->fPlusEqualize[pDoc->m_stAllCH_CONFIG[m_nChannelNo].wProbeBand][1];
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].fPlusAddX21 = pDoc->fPlusEqualize[pDoc->m_stAllCH_CONFIG[m_nChannelNo].wProbeBand][2];
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].fPlusAddX22 = pDoc->fPlusEqualize[pDoc->m_stAllCH_CONFIG[m_nChannelNo].wProbeBand][3];


	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_bSetOne = TRUE;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnCbnSelchangeComboProbeFreq"),"Leave");
}

void CChannelPanel::OnBnClickedCheckMpMAltitude()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckMpMAltitude"),"Enter");

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ALLSCanItemEnableJudge();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckMpMAltitude"),"Leave");
}

void CChannelPanel::OnBnClickedCheckFpMAltitude()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckFpMAltitude"),"Enter");

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ALLSCanItemEnableJudge();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckFpMAltitude"),"Leave");
}

void CChannelPanel::ALLSCanItemEnableJudge()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::ALLSCanItemEnableJudge"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	// 闸门选择
	for(int i = 0; i < 4; i++)
	{
		if(m_ChannelInfo[m_nChannelNo].m_GateInfo[i].m_bOpen)
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_SELECT_GATE1 + i, TRUE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_SELECT_GATE1 + i, TRUE);
		}
		else
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_SELECT_GATE1 + i, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_SELECT_GATE1 + i, FALSE);
			if(pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nGateIndex == i) // 选中变灰，默认闸门I
			{
				pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_SELECT_GATE1, TRUE);
			}

			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_SELECT_GATE1 + i, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_SELECT_GATE1 + i, FALSE);
			if(pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->m_nGateIndex == i) // 选中变灰，默认闸门I
			{
				pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_SELECT_GATE1, TRUE);
			}
		}
	}
	//幅值特征,成像模式
	if(m_bMPMAltitudeDisplay)
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_RANGEDRAW, TRUE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_MAX_AMP, TRUE);

		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_RANGEDRAW, TRUE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_MAX_AMP, TRUE);
	}
	else
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_MAX_AMP, FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_MAX_AMP, FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_MAX_AMP, FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_MAX_AMP, FALSE);
		if(!m_bFPMAltitudeDisplay)
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_RANGEDRAW, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_RANGEDRAW, FALSE);

			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_RANGEDRAW, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_RANGEDRAW, FALSE);
		}
		else
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_RANGEDRAW, TRUE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_RANGEDRAW, TRUE);
		}
	}

	if(m_bFPMAltitudeDisplay)
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_RANGEDRAW, TRUE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_FIRST_AMP, TRUE);

		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_RANGEDRAW, TRUE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_FIRST_AMP, TRUE);
	}
	else
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_FIRST_AMP,FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_FIRST_AMP,FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_FIRST_AMP,FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_FIRST_AMP,FALSE);
		if(!m_bMPMAltitudeDisplay)
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_RANGEDRAW, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_RANGEDRAW, FALSE);

			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_RANGEDRAW, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_RANGEDRAW, FALSE);

		}
		else
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_RANGEDRAW, TRUE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_RANGEDRAW, TRUE);
		}
	}
	//时间特征,成像模式
	if(m_bMPFTimeDisplay)
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_MAX_THRESHOL_TIME, TRUE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);

		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_MAX_THRESHOL_TIME, TRUE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);
	}
	else
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_MAX_THRESHOL_TIME, FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_MAX_THRESHOL_TIME, FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_MAX_THRESHOL_TIME, FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_MAX_THRESHOL_TIME, FALSE);

		if(!m_bMPMTimeDisplay&&!m_bFPFTimeDisplay&&!m_bFPMTimeDisplay)
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_DEEPDRAW, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_DEEPDRAW, FALSE);

			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_DEEPDRAW, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_DEEPDRAW, FALSE);
		}
		else
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);
		}
	}

	if(m_bMPMTimeDisplay)
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_MAX_TIME, TRUE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_MAX_TIME, TRUE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);
	}
	else
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_MAX_TIME, FALSE);		
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_MAX_TIME, FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_MAX_TIME, FALSE);		
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_MAX_TIME, FALSE);

		if(!m_bMPFTimeDisplay&&!m_bFPFTimeDisplay&&!m_bFPMTimeDisplay)
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_DEEPDRAW, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_DEEPDRAW, FALSE);

			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_DEEPDRAW, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_DEEPDRAW, FALSE);			
		}
		else
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);
		}
	}

	if(m_bFPFTimeDisplay)
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_FIRST_THRESHOLD_TIME, TRUE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);

		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_FIRST_THRESHOLD_TIME, TRUE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);
	}
	else
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_FIRST_THRESHOLD_TIME, FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_FIRST_THRESHOLD_TIME, FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_FIRST_THRESHOLD_TIME, FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_FIRST_THRESHOLD_TIME, FALSE);

		if(!m_bMPMTimeDisplay&&!m_bMPFTimeDisplay&&!m_bFPMTimeDisplay)
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_DEEPDRAW, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_DEEPDRAW, FALSE);
			
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_DEEPDRAW, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_DEEPDRAW, FALSE);
		}
		else
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);
		}
	}

	if(m_bFPMTimeDisplay)
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_FIRST_TIME, TRUE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);

		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_FIRST_TIME, TRUE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);
	}
	else
	{
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_FIRST_TIME, FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_FIRST_TIME, FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_FIRST_TIME, FALSE);
		pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_FIRST_TIME, FALSE);
		
		if(!m_bMPFTimeDisplay&&!m_bFPFTimeDisplay&&!m_bMPMTimeDisplay)
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_DEEPDRAW, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSetCheck(IDC_RADIO_DEEPDRAW, FALSE);

			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_DEEPDRAW, FALSE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSetCheck(IDC_RADIO_DEEPDRAW, FALSE);
		}
		else
		{
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);
			pFrame->m_wndCScanParamDockBar.m_pCScanParamView2->OneItemSwitch(IDC_RADIO_DEEPDRAW, TRUE);
		}
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::ALLSCanItemEnableJudge"),"Leave");
}

void CChannelPanel::OnBnClickedCheckMpFTime()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckMpFTime"),"Enter");

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	// 时间特征选择

	ALLSCanItemEnableJudge();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckMpFTime"),"Leave");
}

void CChannelPanel::OnBnClickedCheckMpMTime()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckMpMTime"),"Enter");

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ALLSCanItemEnableJudge();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckMpMTime"),"Leave");
}

void CChannelPanel::OnBnClickedCheckFpFTime()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckFpFTime"),"Enter");

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ALLSCanItemEnableJudge();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckFpFTime"),"Leave");
}

void CChannelPanel::OnBnClickedCheckFpMTime()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckFpMTime"),"Enter");

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	ALLSCanItemEnableJudge();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckFpMTime"),"Leave");	
}

void CChannelPanel::OnBnClickedButtonSaveParam()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedButtonSaveParam"),"Enter");

	// TODO: Add your control notification handler code here
	if (m_strParamFilePath == _T(""))
	{
		OnBnClickedButtonSaveAsParam();
	} 
	else
	{
		SaveParam(m_strParamFilePath);
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedButtonSaveParam"),"Leave");
}

void CChannelPanel::OnBnClickedCheckGroup1Switch()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckGroup1Switch"),"Enter");

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CMainFrame* pFrame =(CMainFrame*)AfxGetMainWnd();

	if(!m_bGroup1Flag)
	{
		GetDlgItem(IDC_RADIO_CH1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CH2)->EnableWindow(FALSE);
		
		// 关闭通道
		m_ChannelInfo[0].m_bOpen = FALSE;
		m_ChannelInfo[1].m_bOpen = FALSE;

		SendWorkMode(0, 0);
	}
	else
	{
		GetDlgItem(IDC_RADIO_CH1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CH2)->EnableWindow(TRUE);

		// 开启通道
		m_ChannelInfo[0].m_bOpen = TRUE;
		m_ChannelInfo[1].m_bOpen = TRUE;
		
		SendWorkMode(0, pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode);
	}

	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckGroup1Switch"),"Leave");
}

void CChannelPanel::OnBnClickedCheckGroup2Switch()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckGroup2Switch"),"Enter");

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CMainFrame* pFrame =(CMainFrame*)AfxGetMainWnd();

	if(!m_bGroup2Flag)
	{
		GetDlgItem(IDC_RADIO_CH3)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CH4)->EnableWindow(FALSE);
		
		// 关闭通道
		m_ChannelInfo[2].m_bOpen = FALSE;
		m_ChannelInfo[3].m_bOpen = FALSE;
		
		SendWorkMode(1, 0);
	}
	else
	{
		GetDlgItem(IDC_RADIO_CH3)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CH4)->EnableWindow(TRUE);
		
		// 开启通道
		m_ChannelInfo[2].m_bOpen = TRUE;
		m_ChannelInfo[3].m_bOpen = TRUE;

		SendWorkMode(1, pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode);
	}

	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckGroup2Switch"),"Leave");
}

// 检测零点、延迟、范围值是否在合理范围
// 零点+延迟<80 && 零点+延迟+范围（转换为us）<250 us
BOOL CChannelPanel::IsInSafeRange(float& fZeroPoint, float& fDelay, float& fRange, const int& nInputIndex)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::IsInSafeRange"),"Enter");

	float fValue1 = fZeroPoint + fDelay;
	DWORD dwRangeDot = Range2DotChannel(m_nWaveSpeed, fRange);// yachang.huang modify on 2014-05-19
	float fRangeTime = float(dwRangeDot) / ADFREQUENCY;
	float fValue2 = fZeroPoint + fDelay + fRangeTime;

	const float MAXTIME1 = 75.0f;
	const float MAXTIME2 = 250.0f;
	if (nInputIndex == 0)	// 零点
	{
		if(fValue1 > MAXTIME1)
		{
			fZeroPoint = MAXTIME1 - fDelay;

			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CChannelPanel::IsInSafeRange"),"Leave");

			return FALSE;
		}

		if (fValue2 > MAXTIME2)
		{
			fZeroPoint = MAXTIME2 - fDelay - fRangeTime;
			
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CChannelPanel::IsInSafeRange"),"Leave");

			return FALSE;
		}
	}
	else if (nInputIndex == 1)	// 延迟
	{
		if(fValue1 > MAXTIME1 )
		{
			fDelay = MAXTIME1 - fZeroPoint;

			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CChannelPanel::IsInSafeRange"),"Leave");

			return FALSE;
		}

		if (fValue2 > MAXTIME2)
		{
			fDelay = MAXTIME2 - fZeroPoint - fRangeTime;

			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CChannelPanel::IsInSafeRange"),"Leave");

			return FALSE;
		}
	}
	else if (nInputIndex == 2)	// 范围
	{
		if (fValue2 > MAXTIME2)
		{
			fRangeTime = MAXTIME2 - fDelay - fZeroPoint;

			int nDot = (int)MicroSecond2DotChannel(fRangeTime);// yachang.huang modify on 2014-05-19
			fRange = Dot2RangeChannel(m_nWaveSpeed, nDot);// yachang.huang modify on 2014-05-19

			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CChannelPanel::IsInSafeRange"),"Leave");

			return FALSE;
		}
	}
	else
	{		
	}
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::IsInSafeRange"),"Leave");
	return TRUE;
}


HBRUSH CChannelPanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnCtlColor"),"Enter");

	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	if (pWnd->GetDlgCtrlID() == IDC_DIPLAYCOLOR)
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnCtlColor"),"Leave");

		return m_brush;
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnCtlColor"),"Leave");

	return hbr;
}

void CChannelPanel::SyncIGateThreshValue(const float fThreshValue)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::SyncIGateThreshValue"),"Enter");

	UpdateData(TRUE);
	int nIndex = m_cmbGateNo.GetCurSel();
	if (nIndex != 0)
		return;
	
	m_fGatePeak = fThreshValue;
	m_fMTPPeak = fThreshValue;
	m_ChannelInfo[m_nChannelNo].m_GateInfo[0].m_fPeakGate = fThreshValue;
	m_pAScanView->m_pGateInfo[0].m_fPeakGate = fThreshValue;
	m_pAScanView->m_pGateInfo[0].m_fMTPPeak = fThreshValue;

	m_sliderGatePeak.SetPos(fThreshValue * 10);	
	m_sliderMTPPeak.SetPos(fThreshValue * 10);

	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	for (int i=0; i<4; i++)
	{
		m_ChannelInfo[m_nChannelNo].m_GateInfo[i].m_fMTPPeak = fThreshValue;
		// 修改向下发的值
		if(i==0)
			pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1SyncThreshold = fThreshValue/100.0*MAX_HEIGHT;
		else if(i==1)
			pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2SyncThreshold = fThreshValue/100.0*MAX_HEIGHT;
		else if(i==2)
			pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3SyncThreshold = fThreshValue/100.0*MAX_HEIGHT;
		else if(i==3)
			pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4SyncThreshold = fThreshValue/100.0*MAX_HEIGHT;
		else
		{
		}

		m_pAScanView->m_pGateInfo[i].m_fMTPPeak = fThreshValue;
	}

	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::SyncIGateThreshValue"),"Leave");
}

void CChannelPanel::DoModifyGateFront()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyGateFront"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
	
	float fDelayDistance = m_fDelay * m_nWaveSpeed / 2 / 1000;
	int nGateIndex = m_cmbGateNo.GetCurSel();
	if ( m_fGateFront + 0.1 < fDelayDistance && m_ChannelInfo[m_nChannelNo].m_GateInfo[nGateIndex].m_nMtpMode == 0)
	{
		MessageBox(_T("闸门前沿不能小于延迟对应的声程"));
		m_fGateFront = fDelayDistance;
		CString str;
		str.Format(_T("%.1f"),m_fGateFront);
		m_fGateFront = _tstof(str);
		m_sliderGateFront.SetPos(m_fGateFront * 10);
	}
	if (m_fGateFront < 1E-6)
	{
		m_fGateFront = 0;
	}
	else if (m_fGateFront > GATE_MAX_RANGE)
	{
		m_fGateFront = GATE_MAX_RANGE;
	}
	int n = m_cmbGateNo.GetCurSel();
	m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_fFrontGate = m_fGateFront;
	// wangbingfu delete on 2015-06-12
/*	DWORD dwDelay = 0;
	if (!m_bMTPmode)
	{
		dwDelay = MicroSecond2Dot(m_fDelay);//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
	}
	else
		dwDelay = 0;
*/
	// wangbingfu delete on 2015-06-12
	//将前沿值写道硬件里面去
	/*
	switch (n)
	{
	case 0:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1Start = Range2DotChannel100(m_nWaveSpeed, m_fGateFront) - dwDelay;
		//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
		break;
	case 1:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2Start = Range2DotChannel100(m_nWaveSpeed, m_fGateFront) - dwDelay;
		//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
		break;
	case 2:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3Start = Range2DotChannel100(m_nWaveSpeed, m_fGateFront) - dwDelay;
		//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
		break;
	case 3:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4Start = Range2DotChannel100(m_nWaveSpeed, m_fGateFront) - dwDelay;
		//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
		break;
	default:
		break;
	}
	*/


     //改为现在所有的闸门信息都更新一遍  yachang.huang modify on 2014-05-29
	// wangbingfu delete on 2015-06-12
/*	
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1Start = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[m_nChannelNo].m_GateInfo[0].m_fFrontGate) - dwDelay;
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2Start = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[m_nChannelNo].m_GateInfo[1].m_fFrontGate) - dwDelay;
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3Start = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[m_nChannelNo].m_GateInfo[2].m_fFrontGate) - dwDelay;
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4Start = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[m_nChannelNo].m_GateInfo[3].m_fFrontGate) - dwDelay;


	m_ChannelInfo[m_nChannelNo].m_GateInfo[0].m_fFrontGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1Start);
	m_ChannelInfo[m_nChannelNo].m_GateInfo[1].m_fFrontGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2Start);// yachang.huang modify on 2014-05-19
	m_ChannelInfo[m_nChannelNo].m_GateInfo[2].m_fFrontGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3Start);// yachang.huang modify on 2014-05-19
	m_ChannelInfo[m_nChannelNo].m_GateInfo[3].m_fFrontGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4Start);// yachang.huang modify on 2014-05-19
*/
	// wangbingfu delete on 2015-06-12

	pFrame->m_bSetOne = TRUE;
	//刷新A扫波形视图区域闸门前沿
	for(int i=0;i<4;i++)
	{
		m_pAScanView->m_pGateInfo[i] = m_ChannelInfo[m_nChannelNo].m_GateInfo[i];
	}

	m_pAScanView->InvalidateRect(m_pAScanView->m_rectWave, FALSE);
	SetGroupOtherChannel();
	
	UpdateData(FALSE);

}
// wangbingfu add start on 2013-01-09
void CChannelPanel::SetGroupOtherChannel()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
	if(pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nAxisIndex == 4)
	{
		CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
		int nPipeGuanLianCard = pCScanView->GetPipeGuanLianCard();
		int nCardNum = m_nChannelNo % 2;
		if(nPipeGuanLianCard == nCardNum)
		{
			int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

			memcpy(&m_ChannelInfo[nOtherChannelIndex], &m_ChannelInfo[m_nChannelNo], sizeof(CChannelInfo));
			memcpy(&pDoc->m_stAllCH_CONFIG[nOtherChannelIndex], &pDoc->m_stAllCH_CONFIG[m_nChannelNo], sizeof(CSCAN_ARS200FPGA_SETONE));
		}
	}
	else
	{
		if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
		{
			int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

			memcpy(&m_ChannelInfo[nOtherChannelIndex], &m_ChannelInfo[m_nChannelNo], sizeof(CChannelInfo));
			memcpy(&pDoc->m_stAllCH_CONFIG[nOtherChannelIndex], &pDoc->m_stAllCH_CONFIG[m_nChannelNo], sizeof(CSCAN_ARS200FPGA_SETONE));
		}
	}
}
// wangbingfu add end on 2013-01-09
void CChannelPanel::DoModifyDelay()
{

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();

	float fDelayDistance = m_fDelay * m_nWaveSpeed / 2 / 1000;
	int nGateIndex = m_cmbGateNo.GetCurSel();

	// wangbingfu modify at 2011-11-19
	if( fDelayDistance - m_fGateFront > 0.1  && m_ChannelInfo[m_nChannelNo].m_GateInfo[nGateIndex].m_nMtpMode == 0)
	{
		MessageBox(_T("延迟对应的声程不能大于闸门前沿"));
		fDelayDistance = m_fGateFront;
		m_fDelay = (fDelayDistance * 1000 * 2) / m_nWaveSpeed;
		CString str;
		str.Format(_T("%.1f"),m_fDelay);
		m_fDelay = _tstof(str);
		m_sliderDelay.SetPos(int(m_fDelay * 10));

	}
	
	if (m_fDelay < 1E-6)
	{
		m_fDelay = 0;
	}
	if (m_fDelay > DELAY_RANGE)
	{
		m_fDelay = DELAY_RANGE;
	}

	m_ChannelInfo[m_nChannelNo].m_fChannelDelay = m_fDelay;	



	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wDelay = MicroSecond2DotChannel(m_fDelay);//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
	m_pAScanView->m_fXMinRuler = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wDelay);
	//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
	//将延迟值给A扫视图，用来画出正确的闸门
	m_pAScanView->m_fXMaxRuler = m_fRange + m_pAScanView->m_fXMinRuler;
	m_pAScanView->InvalidateRect(&m_pAScanView->m_rectScaleX);


	// Song Chenguang add start 0n 2013-12-11
	// 将前沿值写道硬件里面去
	for (int i=0; i<GATE_NUM; i++)
	{
		DWORD dwDelay = 0;
		if (!m_ChannelInfo[m_nChannelNo].m_GateInfo[i].m_nMtpMode)
		{
			dwDelay = MicroSecond2DotChannel(m_fDelay);//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
		}
		else
			continue;

		float fGateFront = m_ChannelInfo[m_nChannelNo].m_GateInfo[i].m_fFrontGate;
		if (i == 0)
			pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1Start = Range2DotChannel100(m_nWaveSpeed, fGateFront) - dwDelay;//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
		else if (i == 1)
			pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2Start = Range2DotChannel100(m_nWaveSpeed, fGateFront) - dwDelay;//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
		else if (i == 2)
			pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3Start = Range2DotChannel100(m_nWaveSpeed, fGateFront) - dwDelay;//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
		else if (i == 3)
			pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4Start = Range2DotChannel100(m_nWaveSpeed, fGateFront) - dwDelay;//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
		else
			;		
	}



	
	pFrame->m_bSetOne = TRUE;


	SetGroupOtherChannel();
	// wanbingfu modify end  2013-01-09
	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyDelay"),"Leave");
}

void CChannelPanel::DoModifyRange()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyRange"),"Enter");

	if (m_fRange < 1E-6)
	{
		m_fRange = 0;
	}
	if (m_fRange > 1000.0)
	{
		m_fRange = 1000.0;
	}
	m_sliderRange.SetPos(m_fRange*10);

	//保留一位小数点
	CString str;
	str.Format(_T("%.1f"),m_fRange);
	m_fRange = _tstof(str);

	m_ChannelInfo[m_nChannelNo].m_fChannelRange = m_fRange;

	WORD nInterSpace[FLAW_DOT_MAX_NO];
	RangeProcess(m_nWaveSpeed, m_fRange, nInterSpace);

	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].fRange = m_fRange;

	int nDotNum = int(m_fRange * 2 / (m_nWaveSpeed * 1000) * ADFREQUENCY * 1E6);
	int nBeiShu = nDotNum / 16384;//由于下位机调整改成8192 yachang.huang modify on 2014-5-16
	// 零点  延迟  范围  闸门前沿 闸门宽度  ADFREQUENCY = ADFREQUENCY/(nBeiShu + 1)
	m_ChannelInfo[m_nChannelNo].m_nChSI = nBeiShu;
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wSimpleInterval = m_ChannelInfo[m_nChannelNo].m_nChSI;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_bSetOne = TRUE;

	m_pAScanView->m_fXMaxRuler = m_fRange + m_pAScanView->m_fXMinRuler;

	m_pAScanView->InvalidateRect(&m_pAScanView->m_rectScaleX);


	DoModifyGateFront();
	DoModifyGateWidth();
	//DoModifyZeroPoint();
	SetGroupOtherChannel();
	

	UpdateData(FALSE);

	
}
void CChannelPanel::OnBnClickedRadioCh3()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedRadioCh3"),"Enter");

//	UpdateData(TRUE);

	DoChangeChannel();

// 	if(m_bExtendArrayFlag[m_nChannelNo])
// 	{
// 		RfExtendOff();
// 	}
// 	m_bExtendFlag = FALSE;
// 	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedRadioCh3"),"Leave");
}

void CChannelPanel::OnBnClickedRadioCh4()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedRadioCh4"),"Enter");

//	UpdateData(TRUE);

	DoChangeChannel();

// 	UpdateData(FALSE);
// 
// 	if(m_bExtendArrayFlag[m_nChannelNo])
// 	{
// 		RfExtendOff();
// 	}
// 	m_bExtendFlag = FALSE;
// 	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedRadioCh4"),"Leave");
}

void CChannelPanel::OnBnClickedRadioCh5()
{
//	UpdateData(TRUE);

	DoChangeChannel();

// 	if(m_bExtendArrayFlag[m_nChannelNo])
// 	{
// 		RfExtendOff();
// 	}
// 	m_bExtendFlag = FALSE;
// 	UpdateData(FALSE);
}

void CChannelPanel::OnBnClickedRadioCh6()
{
//	UpdateData(TRUE);

	DoChangeChannel();

// 	if(m_bExtendArrayFlag[m_nChannelNo])
// 	{
// 		RfExtendOff();
// 	}
// 	m_bExtendFlag = FALSE;
// 	UpdateData(FALSE);
}

void CChannelPanel::OnBnClickedRadioCh7()
{
//	UpdateData(TRUE);

	DoChangeChannel();

// 	if(m_bExtendArrayFlag[m_nChannelNo])
// 	{
// 		RfExtendOff();
// 	}
// 	m_bExtendFlag = FALSE;
// 	UpdateData(FALSE);
}

void CChannelPanel::OnBnClickedRadioCh8()
{
//	UpdateData(TRUE);

	DoChangeChannel();

// 	if(m_bExtendArrayFlag[m_nChannelNo])
// 	{
// 		RfExtendOff();
// 	}
// 	m_bExtendFlag = FALSE;
// 	UpdateData(FALSE);
}
void CChannelPanel::DoChangeChannel()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoChangeChannel"),"Enter");
	UpdateData(TRUE);	// Song Chenguang add on 2013-05-06

	g_dwActiveChannel = m_nChannelNo;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_bSelChannel = TRUE;

	SetGateInfo();
	SetProbeInfo();

	// 更新A扫闸门
	for(int i=0; i<4; i++)
		m_pAScanView->m_pGateInfo[i] = m_ChannelInfo[m_nChannelNo].m_GateInfo[i];

	m_pAScanView->InvalidateRect(m_pAScanView->m_rectWave, FALSE);

	// 更新探头频带
	OnCbnSelchangeComboProbeFreq();

	// 改变C扫面板控件状态
	ALLSCanItemEnableJudge();

	// Song Chenguang add start on 2013-05-06
	if(m_bExtendArrayFlag[m_nChannelNo])
	{
		RfExtendOff();
	}
	m_bExtendFlag = FALSE;
	// Song Chenguang add end on 2013-05-06

	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoChangeChannel"),"Leave");
}

void CChannelPanel::DoModifyGateWidth()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyGateWidth"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
	int n = m_cmbGateNo.GetCurSel();

	if (m_fGateBack < 1E-6)
	{
		m_fGateBack = 0;
	}
	else if (m_fGateBack > GATE_MAX_RANGE)
	{
		m_fGateBack = GATE_MAX_RANGE;
	}

	m_sliderGateBack.SetPos(m_fGateBack*10);
	m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_fBackGate = m_fGateBack;

	//将闸门后沿写道硬件里面去	
	/*
	switch (n)
	{
	case 0:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1Width = Range2DotChannel100(m_nWaveSpeed, m_fGateBack);
		//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
		break;
	case 1:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2Width = Range2DotChannel100(m_nWaveSpeed, m_fGateBack);
		//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
		break;
	case 2:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3Width = Range2DotChannel100(m_nWaveSpeed, m_fGateBack);
		//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
		break;
	case 3:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4Width = Range2DotChannel100(m_nWaveSpeed, m_fGateBack);
		//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19
		break;
	default:
		break;
	}
	*/

     //改为现在所有的闸门信息都更新一遍  yachang.huang modify on 2014-05-29
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1Width = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[m_nChannelNo].m_GateInfo[0].m_fBackGate);
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2Width = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[m_nChannelNo].m_GateInfo[1].m_fBackGate);
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3Width = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[m_nChannelNo].m_GateInfo[2].m_fBackGate);
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4Width = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[m_nChannelNo].m_GateInfo[3].m_fBackGate);

	m_ChannelInfo[m_nChannelNo].m_GateInfo[0].m_fBackGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1Width);// yachang.huang modify on 2014-05-19
	m_ChannelInfo[m_nChannelNo].m_GateInfo[1].m_fBackGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2Width);// yachang.huang modify on 2014-05-19
	m_ChannelInfo[m_nChannelNo].m_GateInfo[2].m_fBackGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3Width);// yachang.huang modify on 2014-05-19
	m_ChannelInfo[m_nChannelNo].m_GateInfo[3].m_fBackGate = Dot2RangeChannel100(m_nWaveSpeed, pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4Width);// yachang.huang modify on 2014-05-19

	pFrame->m_bSetOne = TRUE;

	//刷新A扫视图区域的闸门宽度
	for(int i=0;i<4;i++)
	{
		m_pAScanView->m_pGateInfo[i] = m_ChannelInfo[m_nChannelNo].m_GateInfo[i];
	}

	SetGroupOtherChannel();
	// wanbingfu modify end  2013-01-09

	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyGateWidth"),"Leave");
}

// wangbingfu add start at 2012-12-5
void CChannelPanel::DoModifyGateWidth(int nGateNum, float fBack)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyGateWidth"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
	int n = m_cmbGateNo.GetCurSel();
	if(n == nGateNum)
	{
		m_fGateBack = fBack;
		if (m_fGateBack < 1E-6)
		{
			m_fGateBack = 0;
		}
		else if (m_fGateBack > GATE_MAX_RANGE)
		{
			m_fGateBack = GATE_MAX_RANGE;
		}

		m_sliderGateBack.SetPos(m_fGateBack*10);
		UpdateData(FALSE);
	}
	
	m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_fBackGate = fBack;

	//将闸门后沿写道硬件里面去			
	switch (n)
	{
	case 0:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1Width = Range2DotChannel100(m_nWaveSpeed, fBack);// yachang.huang modify on 2014-05-19
		break;
	case 1:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2Width = Range2DotChannel100(m_nWaveSpeed, fBack);// yachang.huang modify on 2014-05-19
		break;
	case 2:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3Width = Range2DotChannel100(m_nWaveSpeed, fBack);// yachang.huang modify on 2014-05-19
		break;
	case 3:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4Width = Range2DotChannel100(m_nWaveSpeed, fBack);// yachang.huang modify on 2014-05-19
		break;
	default:
		break;
	}

	pFrame->m_bSetOne = TRUE;

	//刷新A扫视图区域的闸门宽度
	m_pAScanView->m_pGateInfo[n] = m_ChannelInfo[m_nChannelNo].m_GateInfo[n];

	// 关联处理
	if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
	{
		int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

		memcpy(&m_ChannelInfo[nOtherChannelIndex], &m_ChannelInfo[m_nChannelNo], sizeof(CChannelInfo));
		memcpy(&pDoc->m_stAllCH_CONFIG[nOtherChannelIndex], &pDoc->m_stAllCH_CONFIG[m_nChannelNo], sizeof(CSCAN_ARS200FPGA_SETONE));
	}

	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyGateWidth"),"Leave");
}
// wangbingfu add end at 2012-12-5

void CChannelPanel::DoModifyGatePeak()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyGatePeak"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
	int n = m_cmbGateNo.GetCurSel();

	if (m_fGatePeak < 1E-6)
	{
		m_fGatePeak = 0;
	}
	else if (m_fGatePeak > GATE_PEAK_MAX_RANGE)
	{
		m_fGatePeak = GATE_PEAK_MAX_RANGE;
	}

	m_sliderGatePeak.SetPos(m_fGatePeak * 10);
	m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_fPeakGate = m_fGatePeak;

	//将闸门阈值写道硬件里面去
	switch(n)
	{
	case 0:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1Amp = m_fGatePeak/100.0*MAX_HEIGHT;
		break;
	case 1:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2Amp = m_fGatePeak/100.0*MAX_HEIGHT;
		break;
	case 2:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3Amp = m_fGatePeak/100.0*MAX_HEIGHT;
		break;
	case 3:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4Amp = m_fGatePeak/100.0*MAX_HEIGHT;
		break;
	default:
		break;
	}
	pFrame->m_bSetOne = TRUE;

	//刷新A扫视图区域的闸门阈值

	m_pAScanView->m_pGateInfo[n] = m_ChannelInfo[m_nChannelNo].m_GateInfo[n];

	if (n == 0)
		SyncIGateThreshValue(m_fGatePeak);

	// 关联处理
	// wanbingfu modify start on  2013-01-09
	/*if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
	{
	int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

	memcpy(&m_ChannelInfo[nOtherChannelIndex], &m_ChannelInfo[m_nChannelNo], sizeof(CChannelInfo));
	memcpy(&pDoc->m_stAllCH_CONFIG[nOtherChannelIndex], &pDoc->m_stAllCH_CONFIG[m_nChannelNo], sizeof(CSCAN_ARS200FPGA_SETONE));
	}*/
	SetGroupOtherChannel();
	// wanbingfu modify end  2013-01-09
	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyGatePeak"),"Leave");
}

void CChannelPanel::DoModifyMTPPeak()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyMTPPeak"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
	int n = m_cmbGateNo.GetCurSel();
	m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_fMTPPeak = m_fMTPPeak;
	m_pAScanView->m_pGateInfo[n].m_fMTPPeak = m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_fMTPPeak;

	if (m_fMTPPeak < 1E-6)
	{
		m_fMTPPeak = 0;
	}
	if (m_fMTPPeak > 100.0)
	{
		m_fMTPPeak = 100.0;
	}
	m_sliderMTPPeak.SetPos(m_fMTPPeak*10);

	switch(n)
	{
	case 0:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate1SyncThreshold = m_fMTPPeak/100.0*MAX_HEIGHT;
		break;
	case 1:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate2SyncThreshold = m_fMTPPeak/100.0*MAX_HEIGHT;
		break;
	case 2:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate3SyncThreshold = m_fMTPPeak/100.0*MAX_HEIGHT;
		break;
	case 3:
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].stGate.wGate4SyncThreshold = m_fMTPPeak/100.0*MAX_HEIGHT;
		break;
	default:
		break;
	}
	pFrame->m_bSetOne = TRUE;

	if (n == 0)
		SyncIGateThreshValue(m_fMTPPeak);

	// 关联处理
	// wanbingfu modify start on  2013-01-09
	/*if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
	{
	int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

	memcpy(&m_ChannelInfo[nOtherChannelIndex], &m_ChannelInfo[m_nChannelNo], sizeof(CChannelInfo));
	memcpy(&pDoc->m_stAllCH_CONFIG[nOtherChannelIndex], &pDoc->m_stAllCH_CONFIG[m_nChannelNo], sizeof(CSCAN_ARS200FPGA_SETONE));
	}*/
	SetGroupOtherChannel();
	// wanbingfu modify end  2013-01-09
	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyMTPPeak"),"Leave");
}

void CChannelPanel::DoModifyZeroPoint()
{

	if (m_fZeroPoint < 1E-6)
	{
		m_fZeroPoint = 0;
	}
	if (m_fZeroPoint > ZERO_POINT_RANGE)
	{
		m_fZeroPoint = ZERO_POINT_RANGE;
	}

	m_sliderZeroPoint.SetPos(m_fZeroPoint*10);

	int n = m_cmbGateNo.GetCurSel();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();
	m_ChannelInfo[m_nChannelNo].m_fZeroPoint = m_fZeroPoint;


	//pDoc->m_stAllCH_CONFIG[m_nChannelNo].stConfig.wMonitorZeroCounter = MicroSecond2DotChannel(m_fZeroPoint);
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stConfig.wMonitorZeroCounter = MicroSecond2DotChannel100(m_fZeroPoint);;//应吴越要求修改，更改对A扫横坐标显示造成影响的数值计算方法  yachang.huang modify on 2014-05-19


	SetGroupOtherChannel();
	// wanbingfu modify end  2013-01-09

	pFrame->m_bSetOne = TRUE;

	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyZeroPoint"),"Leave");
}

void CChannelPanel::DoModifyPlus()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyPlus"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();

	if (m_fPlus < 1E-6)
	{
		m_fPlus = 0;
	}

	// Xinbo.Lin modified 2014-5-7 for limit total gain in range
	if (m_fPlus + m_fRefPlus > PLUS_RANGE) {
		m_fPlus = PLUS_RANGE - m_fRefPlus - 1E-6; // 额外减去一点，确保和小于限制
		CString strRangeWarning;
		strRangeWarning.Format(_T("增益与参考增益之和不得超过%idB"), PLUS_RANGE);
		AfxMessageBox(strRangeWarning);
	}

	m_sliderPlus.SetPos(m_fPlus*10);
	
	m_ChannelInfo[m_nChannelNo].m_fChannelPlus = m_fPlus;

/*
	WORD wRef = 0;
	WORD wLevel1 = 0;
	WORD wLevel2 = 0;
	WORD wLevel3 = 0;
	dBProcess(fTotalPlus, wRef, wLevel1, wLevel2, wLevel3);

	pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].stConfig.wdBRef = wRef;
	pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].stConfig.wdBLevel1 = wLevel1;
	pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].stConfig.wdBLevel2 = wLevel2;
	pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].stConfig.wdBLevel3 = wLevel3;
*/
	pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].fPlus = m_fPlus;
	pFrame->m_bSetOne = TRUE;

	// 刷新显示的值
	pFrame->m_pDacCurveDlg->DisplayValue(m_fPlus, m_fRefPlus, g_dwActiveChannel);
	pFrame->m_pTcgCurveDlg->DisplayValue(m_fPlus,m_fRefPlus, g_dwActiveChannel);

	// 关联处理
	// wanbingfu modify start on  2013-01-09
	/*if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
	{
	int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

	memcpy(&m_ChannelInfo[nOtherChannelIndex], &m_ChannelInfo[m_nChannelNo], sizeof(CChannelInfo));
	memcpy(&pDoc->m_stAllCH_CONFIG[nOtherChannelIndex], &pDoc->m_stAllCH_CONFIG[m_nChannelNo], sizeof(CSCAN_ARS200FPGA_SETONE));
	}*/
	SetGroupOtherChannel();
	// wanbingfu modify end  2013-01-09
	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyPlus"),"Leave");
}

void CChannelPanel::DoModifyRestain()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyRestain"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();

	if (m_fRestrain < 1E-6)
	{
		m_fRestrain = 0.0f;
	}
	if (m_fRestrain > 100.0f)
	{
		m_fRestrain = 100.0f;
	}
	m_sliderRestrain.SetPos(int(m_fRestrain*10));
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stConfig.wSuspend = m_fRestrain / 100.0 * MAX_HEIGHT;
	m_ChannelInfo[m_nChannelNo].m_fRestrain = m_fRestrain;

	pFrame->m_bSetOne = TRUE;

	// 关联处理
	// wanbingfu modify start on  2013-01-09
	/*if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
	{
	int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

	memcpy(&m_ChannelInfo[nOtherChannelIndex], &m_ChannelInfo[m_nChannelNo], sizeof(CChannelInfo));
	memcpy(&pDoc->m_stAllCH_CONFIG[nOtherChannelIndex], &pDoc->m_stAllCH_CONFIG[m_nChannelNo], sizeof(CSCAN_ARS200FPGA_SETONE));
	}*/
	SetGroupOtherChannel();
	// wanbingfu modify end  2013-01-09

	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyRestain"),"Leave");
}
void CChannelPanel::OnBnClickedCheckGroup3Switch()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckGroup3Switch"),"Enter");

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(!m_bGroup3Flag)
	{
		GetDlgItem(IDC_RADIO_CH5)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CH6)->EnableWindow(FALSE);

		// 关闭通道
		m_ChannelInfo[4].m_bOpen = FALSE;
		m_ChannelInfo[5].m_bOpen = FALSE;

		SendWorkMode(2, 0);
	}
	else
	{
		GetDlgItem(IDC_RADIO_CH5)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CH6)->EnableWindow(TRUE);

		// 开启通道
		m_ChannelInfo[4].m_bOpen = TRUE;
		m_ChannelInfo[5].m_bOpen = TRUE;

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		SendWorkMode(2, pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode);
	}

	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckGroup3Switch"),"Leave");
}

void CChannelPanel::OnBnClickedCheckGroup4Switch()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckGroup4Switch"),"Enter");

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(!m_bGroup4Flag)
	{
		GetDlgItem(IDC_RADIO_CH7)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_CH8)->EnableWindow(FALSE);

		// 关闭通道
		m_ChannelInfo[6].m_bOpen = FALSE;
		m_ChannelInfo[7].m_bOpen = FALSE;

		SendWorkMode(3, 0);
	}
	else
	{
		GetDlgItem(IDC_RADIO_CH7)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_CH8)->EnableWindow(TRUE);

		// 开启通道
		m_ChannelInfo[6].m_bOpen = TRUE;
		m_ChannelInfo[7].m_bOpen = TRUE;

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		SendWorkMode(3, pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode);
	}

	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckGroup4Switch"),"Leave");
}


void CChannelPanel::InitMaterialSoundSpeed()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::InitMaterialSoundSpeed"),"Enter");

	m_MaterialSoundSpeeds[0].m_strMaterial = _T("钢");
	m_MaterialSoundSpeeds[0].m_wSoundSpeed = 5920;
	m_MaterialSoundSpeeds[1].m_strMaterial = _T("铁");
	m_MaterialSoundSpeeds[1].m_wSoundSpeed = 5930;
	m_MaterialSoundSpeeds[2].m_strMaterial = _T("铜");
	m_MaterialSoundSpeeds[2].m_wSoundSpeed = 4720;
	m_MaterialSoundSpeeds[3].m_strMaterial = _T("陶瓷");
	m_MaterialSoundSpeeds[3].m_wSoundSpeed = 5842;
	m_MaterialSoundSpeeds[4].m_strMaterial = _T("不锈钢");
	m_MaterialSoundSpeeds[4].m_wSoundSpeed = 5740;
	m_MaterialSoundSpeeds[5].m_strMaterial = _T("铝");
	m_MaterialSoundSpeeds[5].m_wSoundSpeed = 6260;
	m_MaterialSoundSpeeds[6].m_strMaterial = _T("黄铜");
	m_MaterialSoundSpeeds[6].m_wSoundSpeed = 4399;
	m_MaterialSoundSpeeds[7].m_strMaterial = _T("锌");
	m_MaterialSoundSpeeds[7].m_wSoundSpeed = 4170;
	m_MaterialSoundSpeeds[8].m_strMaterial = _T("环氧树脂");
	m_MaterialSoundSpeeds[8].m_wSoundSpeed = 2540;
	m_MaterialSoundSpeeds[9].m_strMaterial = _T("聚氯乙烯");
	m_MaterialSoundSpeeds[9].m_wSoundSpeed = 2388;
	m_MaterialSoundSpeeds[10].m_strMaterial = _T("水");
	m_MaterialSoundSpeeds[10].m_wSoundSpeed = 1500;

	m_cmbMaterial.ResetContent();
	for (int i=0; i<MATERIAL_NUM; i++)
	{
		m_cmbMaterial.AddString(m_MaterialSoundSpeeds[i].m_strMaterial);
	}

	m_cmbMaterial.AddString(_T("自定义"));	

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::InitMaterialSoundSpeed"),"Leave");
}
void CChannelPanel::OnCbnSelchangeComboMaterial()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnCbnSelchangeComboMaterial"),"Enter");

	// TODO: Add your control notification handler code here
	int nIndex = m_cmbMaterial.GetCurSel();

	if (nIndex != -1 && nIndex != 11)
	{
		m_ChannelInfo[m_nChannelNo].m_nWaveSpeed = m_MaterialSoundSpeeds[nIndex].m_wSoundSpeed;
		m_nWaveSpeed = m_MaterialSoundSpeeds[nIndex].m_wSoundSpeed;
		g_nWaveSpeed = m_nWaveSpeed;

		CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
		pDoc->m_stAllCH_CONFIG[m_nChannelNo].nVelocity = m_MaterialSoundSpeeds[nIndex].m_wSoundSpeed;

		// 重新设一下参数
		SetGateInfo();
		SetProbeInfo();

		m_pAScanView->InvalidateRect(m_pAScanView->m_rectWave, FALSE);

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->m_bSetOne = TRUE;

		// 关联处理
		// wanbingfu modify start on  2013-01-09
		/*if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
		{
		int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

		memcpy(&m_ChannelInfo[nOtherChannelIndex], &m_ChannelInfo[m_nChannelNo], sizeof(CChannelInfo));
		memcpy(&pDoc->m_stAllCH_CONFIG[nOtherChannelIndex], &pDoc->m_stAllCH_CONFIG[m_nChannelNo], sizeof(CSCAN_ARS200FPGA_SETONE));
		}*/
		SetGroupOtherChannel();
		// wanbingfu modify end  2013-01-09

		UpdateData(FALSE);
		// DongQi add start on 2013-06-27
		OnEnKillfocusEditWaveSpeed();
		// DongQi add end on 2013-06-27
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnCbnSelchangeComboMaterial"),"Leave");
}
// DongQi delete start on 2013-06-27
// void CChannelPanel::OnEnChangeEditWaveSpeed()
// {
// 	// TODO:  If this is a RICHEDIT control, the control will not
// 	// send this notification unless you override the CFormView::OnInitDialog()
// 	// function and call CRichEditCtrl().SetEventMask()
// 	// with the ENM_CHANGE flag ORed into the mask.
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnEnChangeEditWaveSpeed"),"Enter");
// 
// 	// TODO:  Add your control notification handler code here
// 	UpdateData(TRUE);
// 
// 	m_ChannelInfo[m_nChannelNo].m_nWaveSpeed = m_nWaveSpeed;
// 	g_nWaveSpeed = m_nWaveSpeed;
// 
// 	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
// 	pDoc->m_stAllCH_CONFIG[m_nChannelNo].nVelocity = m_nWaveSpeed;
// 
// 	BOOL bFind = FALSE;
// 	for (int i=0; i<MATERIAL_NUM; i++)
// 	{
// 		if (m_nWaveSpeed == m_MaterialSoundSpeeds[i].m_wSoundSpeed)
// 		{
// 			m_cmbMaterial.SetCurSel(i);
// 			bFind = TRUE;
// 		}
// 	}
// 
// 	if (!bFind)
// 		m_cmbMaterial.SetCurSel(11);	// 自定义
// 
// 	// 重新设一下参数
// 	SetGateInfo();
// 	SetProbeInfo();
// 
// 	m_pAScanView->InvalidateRect(m_pAScanView->m_rectWave, FALSE);
// 
// 	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
// 	pFrame->m_bSetOne = TRUE;
// 
// 	// 关联处理
// 	// wanbingfu modify start on  2013-01-09
// 	/*if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
// 	{
// 		int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);
// 
// 		memcpy(&m_ChannelInfo[nOtherChannelIndex], &m_ChannelInfo[m_nChannelNo], sizeof(CChannelInfo));
// 		memcpy(&pDoc->m_stAllCH_CONFIG[nOtherChannelIndex], &pDoc->m_stAllCH_CONFIG[m_nChannelNo], sizeof(CSCAN_ARS200FPGA_SETONE));
// 	}*/
// 	SetGroupOtherChannel();
// 	// wanbingfu modify end  2013-01-09
// 
// 	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnEnChangeEditWaveSpeed"),"Leave");
// }
// DongQi delete end on 2013-06-27


int CChannelPanel::CalcRepeateCounter(const int& nSynFreq)
{
	//int nValue = (int)(float(ADFREQUENCY*1E6) / (256.0 * nSynFreq));
	int nValue = (int)(float(100*1E6) / (256.0 * nSynFreq));//yachang.huang modify on 2014-6-5
	return nValue;
}

int CChannelPanel::CalcRepeateCounterReverse(const int& nValue)
{
	//int nSyncFreq = (int)(float(ADFREQUENCY*1E6) / nValue / 256.0);
	int nSyncFreq = (int)(float(100*1E6) / nValue / 256.0);//yachang.huang modify on 2014-6-5
	return nSyncFreq;
}

void CChannelPanel::DoModifyReFreq()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyReFreq"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();

	int nValue = CalcRepeateCounter((int)m_nReFreq);
	int nCardNum = pApp->GetCardNum();

	for (int i=0; i<nCardNum; i++)
	{
		for (int j=0; j<2; j++)
		{
			int nChannelIndex = 2 * i + j;
			m_ChannelInfo[nChannelIndex].m_ProbeInfo.m_wReFreq = m_nReFreq;

			pDoc->m_stAllCH_CONFIG[nChannelIndex].stCon.wRepeatCounter = nValue;

			CSCAN_ARS200FPGA_SETONE *pCHX_INIT_CONFIG = &(pDoc->m_stAllCH_CONFIG[nChannelIndex]);

			// test 2013-09-12
			if (i == 0 && j == 0)
			{
				BOOL bRet = CheckSetOneParam(*pCHX_INIT_CONFIG);

				if (!bRet)
				{
					LOG4CXX_ERROR(log4cxx::Logger::getLogger("AEC_palte_setone"),"Leave");
					return;
				}

				const int ALL_PARAM_SIZE = sizeof(CSCAN_ARS200FPGA_SETONE);
				char szSendData[NETHEAD_SIZE+ALL_PARAM_SIZE];
				memset( szSendData, 0, sizeof(szSendData));

				PACKET_HEAD_t stPacketHead;
				stPacketHead.dwCommand = COMMAND_ALL_PARAM_SET;
				stPacketHead.wChannelIndex = j;
				stPacketHead.wGateIndex = 0;
				stPacketHead.dwSize = ALL_PARAM_SIZE;

				memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));
				memcpy( &szSendData[NETHEAD_SIZE], pCHX_INIT_CONFIG, ALL_PARAM_SIZE);
				pFrame->SendData( szSendData, sizeof(szSendData), nChannelIndex);
			}			
		}
	}
	

	// 关联处理
	// wanbingfu modify start on  2013-01-09	
	SetGroupOtherChannel();
	// wanbingfu modify end  2013-01-09

	// 做SetOne操作确保切换为当前通道
	pFrame->m_bSetOne = TRUE;	// Song Chenguang add on 2013-09-27
	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyReFreq"),"Leave");
}

// Song Chenguang modify end on 2013-07-17
void CChannelPanel::OnDeltaposSpinReFreq(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinReFreq"),"Enter");

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_nReFreq -= pNMUpDown->iDelta;	

	if (m_nReFreq < RE_FREQ_MIN)
	{
		m_nReFreq = RE_FREQ_MIN;
	}
	else if (m_nReFreq > RE_FREQ_MAX)
	{
		m_nReFreq = RE_FREQ_MAX;
	}
	
	DoModifyReFreq();

	m_sliderReFreq.SetPos(m_nReFreq);
	UpdateData(FALSE);

	*pResult = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinReFreq"),"Leave");
}

VOLTAGE_TYPE_e CChannelPanel::FindVoltageType(const WORD& wValue)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::FindVoltageType"),"Enter");

	VOLTAGE_TYPE_e eType = TYPE_ERROR;
	if (wValue == 50)
	{
		eType = TYPE_50;
	}
	else if (wValue == 400)
	{
		eType = TYPE_400;
	}
	else
	{}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::FindVoltageType"),"Leave");

	return eType;
}


void CChannelPanel::OnBnClickedCheckLockParam()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckLockParam"),"Enter");

	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	UINT itemIDs[] = {IDC_SLIDER_GATE_FRONT, IDC_SPIN_FRONT, IDC_SLIDER_GATE_BACK, IDC_SPIN_WIDTH,
	IDC_SLIDER_GATE_PEAK, IDC_SPIN_GATE_PEAK, IDC_SLIDER_MTP_PEAK, IDC_SPIN_MTP_PEAK,
	IDC_BUTTON_GATE_COLOR, IDC_SLIDER_ZEROPOINT, IDC_SPIN_ZEROPOINT, IDC_SLIDER_DELAY,
	// WangQianfeng add start on 2013-06-28
	IDC_SLIDER_REF_PLUS,IDC_SPIN_REF_PLUS,IDC_COMBO_GATE_NO,IDC_CHECK_EXTENSION,IDC_CHECK_GATE_ADJUST,
	IDC_EDIT_WAVE_SPEED,IDC_CHECK_MP_F_TIME,IDC_CHECK_MP_M_TIME,IDC_CHECK_MP_M_ALTITUDE,
	IDC_CHECK_FP_F_TIME,IDC_CHECK_FP_M_TIME,IDC_CHECK_FP_M_ALTITUDE,
	// WangQianfeng add end on 2013-06-28
	IDC_SPIN_DELAY, IDC_SLIDER_RANGE, IDC_SPIN_RANGE, IDC_SLIDER_PLUS, IDC_SPIN_PLUS,
	IDC_SLIDER_RESTRAIN, IDC_SPIN_RESTRAIN, IDC_SLIDER_RE_FREQ, IDC_SPIN_RE_FREQ, 
	IDC_COMBO_CH_SAVG, IDC_COMBO_MATERIAL, IDC_BUTTON_OTHER_PARAM, IDC_BUTTON_READ_PARAM,
	IDC_BUTTON_SAVE_PARAM, IDC_BUTTON_SAVE_AS_PARAM};

	for (int i=0; i < (sizeof(itemIDs)/sizeof(UINT)); i++)
	{
		GetDlgItem(itemIDs[i])->EnableWindow(!m_bLockParam);
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckLockParam"),"Leave");
}


BOOL CChannelPanel::SendWorkMode(const int& nCardIndex, const WORD& wWorkMode)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::SendWorkMode"),"Enter");
	
	UpdateData(TRUE);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	// wangbingfu add start on 2013-01-09
	int nWorkMode = wWorkMode;
	CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	if(pCScanView->m_nAxisSelect == 4)
	{
		int nPipeGuanLianCard = pCScanView->GetPipeGuanLianCard();
		if(nPipeGuanLianCard == nCardIndex)	// 所设卡为关联卡
		{
			nWorkMode = 6;
		}
	}
	// 发送工作模式
	const int WORKMODE_SIZE = sizeof(WORD);
	char szSendData[NETHEAD_SIZE+WORKMODE_SIZE];
	memset( szSendData, 0, sizeof(szSendData));

	PACKET_HEAD_t stPacketHead;
	stPacketHead.dwCommand = COMMAND_WORK_MODE;
	stPacketHead.wChannelIndex = m_nChannelNo;
	stPacketHead.wGateIndex = 0;
	stPacketHead.dwSize = WORKMODE_SIZE;

	memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));
	memcpy(&szSendData[NETHEAD_SIZE], &nWorkMode, WORKMODE_SIZE);
	pFrame->SendData( szSendData, sizeof(szSendData), nCardIndex*2);

	// 若为关联模式，一发一收，向每个通道发送数据， 双单以上值
	const int ALL_PARAM_SIZE = sizeof(CSCAN_ARS200FPGA_SETONE);
	if (nWorkMode > 3)
//	if (wWorkMode > 4)
	{
//		int nCardNum = pApp->GetCardNum();
//		for (int i=0; i<nCardNum; i++)
//		{
			m_ChannelInfo[2 * nCardIndex + 1] = m_ChannelInfo[2 * nCardIndex];

			int j = 2 * nCardIndex;
//			for (int j = 2 * nCardIndex; j < 2 * (nCardIndex + 1); j++)
//			{
				//闸门开关
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate1Enable = m_ChannelInfo[j].m_GateInfo[0].m_bOpen;
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate2Enable = m_ChannelInfo[j].m_GateInfo[1].m_bOpen;
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate3Enable = m_ChannelInfo[j].m_GateInfo[2].m_bOpen;
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate4Enable = m_ChannelInfo[j].m_GateInfo[3].m_bOpen;
				//闸门同步模式
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate1SyncMode = m_ChannelInfo[j].m_GateInfo[0].m_nMtpMode;
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate2SyncMode = m_ChannelInfo[j].m_GateInfo[1].m_nMtpMode;
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate3SyncMode = m_ChannelInfo[j].m_GateInfo[2].m_nMtpMode;
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate4SyncMode = m_ChannelInfo[j].m_GateInfo[3].m_nMtpMode;

				//闸门前沿
				DWORD dwDelay[4] = {0, 0, 0, 0};
				for (int k=0; k<4; k++)
				{
					if (!m_ChannelInfo[j].m_GateInfo[k].m_nMtpMode)
					{
						dwDelay[k] = MicroSecond2DotChannel(m_ChannelInfo[j].m_fChannelDelay);// yachang.huang modify on 2014-05-19
					}
					else
						dwDelay[k] = 0;
				}

				pDoc->m_stAllCH_CONFIG[j].stGate.wGate1Start = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[j].m_GateInfo[0].m_fFrontGate) - dwDelay[0];// yachang.huang modify on 2014-05-19
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate2Start = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[j].m_GateInfo[1].m_fFrontGate) - dwDelay[1];// yachang.huang modify on 2014-05-19
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate3Start = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[j].m_GateInfo[2].m_fFrontGate) - dwDelay[2];// yachang.huang modify on 2014-05-19
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate4Start = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[j].m_GateInfo[3].m_fFrontGate) - dwDelay[3];// yachang.huang modify on 2014-05-19

				//闸门宽度
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate1Width = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[j].m_GateInfo[0].m_fBackGate);// yachang.huang modify on 2014-05-19
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate2Width = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[j].m_GateInfo[1].m_fBackGate);// yachang.huang modify on 2014-05-19
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate3Width = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[j].m_GateInfo[2].m_fBackGate);// yachang.huang modify on 2014-05-19
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate4Width = Range2DotChannel100(m_nWaveSpeed, m_ChannelInfo[j].m_GateInfo[3].m_fBackGate);// yachang.huang modify on 2014-05-19
				//闸门阈值
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate1Amp = m_ChannelInfo[j].m_GateInfo[0].m_fPeakGate / 100.0f * MAX_HEIGHT;
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate2Amp = m_ChannelInfo[j].m_GateInfo[1].m_fPeakGate / 100.0f * MAX_HEIGHT;
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate3Amp = m_ChannelInfo[j].m_GateInfo[2].m_fPeakGate / 100.0f * MAX_HEIGHT;
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate4Amp = m_ChannelInfo[j].m_GateInfo[3].m_fPeakGate / 100.0f * MAX_HEIGHT;
				//闸门同步阈值
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate1SyncThreshold = m_ChannelInfo[j].m_GateInfo[0].m_fMTPPeak / 100.0f * MAX_HEIGHT;
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate2SyncThreshold = m_ChannelInfo[j].m_GateInfo[1].m_fMTPPeak / 100.0f * MAX_HEIGHT;
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate3SyncThreshold = m_ChannelInfo[j].m_GateInfo[2].m_fMTPPeak / 100.0f * MAX_HEIGHT;
				pDoc->m_stAllCH_CONFIG[j].stGate.wGate4SyncThreshold = m_ChannelInfo[j].m_GateInfo[3].m_fMTPPeak / 100.0f * MAX_HEIGHT;

				//延迟
				pDoc->m_stAllCH_CONFIG[j].stGate.wDelay = MicroSecond2DotChannel(m_ChannelInfo[j].m_fChannelDelay);// yachang.huang modify on 2014-05-19

				//范围
				// 		int nMin = Dot2Range(m_nWaveSpeed, MONITOR_SIZE);
				// 		if (m_ChannelInfo[i].m_fChannelRange < nMin)
				// 		{
				// 			m_ChannelInfo[i].m_fChannelRange = nMin;
				// 		}

				// 增益
				pDoc->m_stAllCH_CONFIG[j].fPlus = m_ChannelInfo[j].m_fChannelPlus;
				pDoc->m_stAllCH_CONFIG[j].fRefPlus = m_ChannelInfo[j].m_fChannelRefPlus;

				// 零点
				pDoc->m_stAllCH_CONFIG[j].stConfig.wMonitorZeroCounter = MicroSecond2DotChannel100(m_ChannelInfo[j].m_fZeroPoint);// yachang.huang modify on 2014-05-19

				//抑制
				pDoc->m_stAllCH_CONFIG[j].stConfig.wSuspend = m_ChannelInfo[j].m_fRestrain / 100.0f * MAX_HEIGHT;;

				// 采样间隔
				pDoc->m_stAllCH_CONFIG[j].stGate.wSimpleInterval = m_ChannelInfo[j].m_nChSI;

				// 采样平均数
				pDoc->m_stAllCH_CONFIG[j].stGate.wSimpleAverage = m_ChannelInfo[j].m_nChSavg;	

				// 重复频率
				pDoc->m_stAllCH_CONFIG[j].stCon.wRepeatCounter = CalcRepeateCounter(m_ChannelInfo[j].m_ProbeInfo.m_wReFreq);
				// 声速
				pDoc->m_stAllCH_CONFIG[j].nVelocity = m_ChannelInfo[j].m_nWaveSpeed;
				// 探头频率
				pDoc->m_stAllCH_CONFIG[j].wProbeBand = (WORD)m_ChannelInfo[j].m_ProbeInfo.m_eProbeFreq;	

				// 激发电压
				switch (m_ChannelInfo[j].m_ProbeInfo.m_eInterruptVoltage)
				{
				case TYPE_50:
					pDoc->m_stAllCH_CONFIG[j].wVoltage = 50;
					break;
				case TYPE_400:
					pDoc->m_stAllCH_CONFIG[j].wVoltage = 400;
					break;
				default:
					pDoc->m_stAllCH_CONFIG[j].wVoltage = 50;
					break;
				}		
				// 检波模式
				pDoc->m_stAllCH_CONFIG[j].stConfig.wDetectMode = m_ChannelInfo[j].m_ProbeInfo.m_eDisplayMode;
				// 阻抗
				pDoc->m_stAllCH_CONFIG[j].wImpedance = m_ChannelInfo[j].m_ProbeInfo.m_eImpedance;
				// 脉宽
				pDoc->m_stAllCH_CONFIG[j].stCon.wMaiKuan = WORD(m_ChannelInfo[j].m_ProbeInfo.m_wPlusWidth / (1000.0 / ADFREQUENCY));

				pDoc->m_stAllCH_CONFIG[j].wEnerge = m_ChannelInfo[j].m_ProbeInfo.m_eEnergy;  //能量 yachang.huang add on 2013-11-25
				pDoc->m_stAllCH_CONFIG[j].wRevAndSendMode = m_ChannelInfo[j].m_ProbeInfo.m_eRevAndSendMode;//收发模式  yachang.huang add on 2013-11-25
				pDoc->m_stAllCH_CONFIG[j].wSPIAddr = m_ChannelInfo[j].m_ProbeInfo.m_eSpiAddr;//SPI地址 yachang.huang add on 2013-11-25
				pDoc->m_stAllCH_CONFIG[j].wSPIData = m_ChannelInfo[j].m_ProbeInfo.m_nSpiData;//SPI数据 yachang.huang add on 2013-11-25

				char szSendData2[NETHEAD_SIZE+ALL_PARAM_SIZE];
				memset( szSendData2, 0, sizeof(szSendData2));

				PACKET_HEAD_t stPacketHead2;
				stPacketHead2.dwCommand = COMMAND_ALL_PARAM_SET;
				stPacketHead2.wChannelIndex = j;
				stPacketHead2.wGateIndex = 0;
				stPacketHead2.dwSize = ALL_PARAM_SIZE;

				memcpy(szSendData2, &stPacketHead2, sizeof(stPacketHead2));
				memcpy( &szSendData2[NETHEAD_SIZE], &pDoc->m_stAllCH_CONFIG[j], ALL_PARAM_SIZE);

				pFrame->SendData( szSendData, sizeof(szSendData), j);
//			}
//		}
	}
	// wangbingfu modify end on 2013-01-09
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::SendWorkMode"),"Leave");

	return TRUE;
}

void CChannelPanel::DoModifyRefPlus()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyRefPlus"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestDoc* pDoc = m_pAScanView->GetDocument();

	if (m_fRefPlus < 1E-6)
	{
		m_fRefPlus = 0;
	}

	// Xinbo.Lin modified on 2014-5-7 for limiting total gain in range
	if (m_fPlus + m_fRefPlus> PLUS_RANGE) {
		m_fRefPlus = PLUS_RANGE - m_fPlus - 1E-6; // 额外减去一点，确保和小于限制
		CString strRangeWarning;
		strRangeWarning.Format(_T("增益与参考增益之和不得超过%idB"), PLUS_RANGE);
		AfxMessageBox(strRangeWarning);
	}

	m_sldRefPlus.SetPos(m_fRefPlus * 10);

	m_ChannelInfo[m_nChannelNo].m_fChannelRefPlus = m_fRefPlus;

/*
	WORD wRef = 0;
	WORD wLevel1 = 0;
	WORD wLevel2 = 0;
	WORD wLevel3 = 0;
	dBProcess(fTotalPlus, wRef, wLevel1, wLevel2, wLevel3);

	pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].stConfig.wdBRef = wRef;
	pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].stConfig.wdBLevel1 = wLevel1;
	pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].stConfig.wdBLevel2 = wLevel2;
	pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].stConfig.wdBLevel3 = wLevel3;
*/
	pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].fRefPlus = m_fRefPlus;
	pFrame->m_bSetOne = TRUE;

	// 刷新显示的值
	pFrame->m_pDacCurveDlg->DisplayValue(m_fPlus, m_fRefPlus, g_dwActiveChannel);
	pFrame->m_pTcgCurveDlg->DisplayValue(m_fPlus,m_fRefPlus, g_dwActiveChannel);

	// 关联处理
	// wanbingfu modify start on  2013-01-09
	/*	if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
	{
	int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

	memcpy(&m_ChannelInfo[nOtherChannelIndex], &m_ChannelInfo[m_nChannelNo], sizeof(CChannelInfo));
	memcpy(&pDoc->m_stAllCH_CONFIG[nOtherChannelIndex], &pDoc->m_stAllCH_CONFIG[m_nChannelNo], sizeof(CSCAN_ARS200FPGA_SETONE));
	}*/
	SetGroupOtherChannel();
	// wanbingfu modify end  2013-01-09

	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::DoModifyRefPlus"),"Leave");
}
void CChannelPanel::OnDeltaposSpinRefPlus(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinRefPlus"),"Enter");

	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_fRefPlus -= pNMUpDown->iDelta/10.0f;

	DoModifyRefPlus();

	m_sldRefPlus.SetPos(int(m_fRefPlus * 10));
	*pResult = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnDeltaposSpinRefPlus"),"Leave");
}

/*
void CChannelPanel::OnCbnSelchangeComboWorkMode()
{
	CComboBox* pcmbWorkMode = (CComboBox*)GetDlgItem(IDC_COMBO_WORK_MODE);
	int nIndex = pcmbWorkMode->GetCurSel();
	m_wWorkMode = nIndex + 1;

	// 下发工作模式
	CUltrasonicImmersedTestApp* pApp =(CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();
	for (int i=0; i<nCardNum; i++)
	{
		// 模式设为一样的
//		m_ChannelInfo[2 * i].m_ProbeInfo.m_wWorkMode = m_wWorkMode;
//		m_ChannelInfo[2 * i + 1].m_ProbeInfo.m_wWorkMode = m_wWorkMode;			

		SendWorkMode(i, m_wWorkMode);
	}

	// 模式为单2、双晶, 切换到偶数通道,
	int nYu = m_nChannelNo % 2;
 	if (m_wWorkMode == 2 || m_wWorkMode == 4 || m_wWorkMode == 5 ||
 		m_wWorkMode == 7 || m_wWorkMode == 8)
//	if (m_wWorkMode == 2 || m_wWorkMode == 5 || m_wWorkMode == 6 ||
//		m_wWorkMode == 8 || m_wWorkMode == 9)
	{
		if (nYu == 0)
		{
			m_nChannelNo += 1;
		}

		DoChangeChannel();
	}
	else if (nYu == 1 && m_wWorkMode == 1)
	{
		m_nChannelNo -= 1;

		DoChangeChannel();
	}

	SetMoveRule(m_wWorkMode);
}
*/
int CChannelPanel::CalGroupOtherIndex(const int& nChannelIndex)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::CalGroupOtherIndex"),"Enter");

	int nYu = nChannelIndex % 2;

	int nOtherChannelIndex = 0;
	if (nYu == 0)
		nOtherChannelIndex = nChannelIndex + 1;
	else
		nOtherChannelIndex = nChannelIndex - 1;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::CalGroupOtherIndex"),"Leave");

	return nOtherChannelIndex;
}
/*
// 根据工作模式设定机械运动规则
// 目前只写一块卡的情况
void CChannelPanel::SetMoveRule(const WORD& wWorkMode)
{
	CUltrasonicImmersedTestApp* pApp =(CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();

	double dSysPara[40];//返回系统参数
	long   llogPara[10];//填充方式

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
	pMovePathView->m_GraphMove.GetPara(dSysPara, llogPara);//获取系统参数

	int nIndexOneProbeType = 0; 
	int	nIndexTwoProbeType = 0;
	int nIndexThreeProbeType = 0;
	if (wWorkMode == 1)	// 单1
	{
		CString str;
		str.Format(_T("%d"), 1);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeNum"), str);

		nIndexOneProbeType = 0;
		str.Format(_T("%d"), nIndexOneProbeType);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("OneProbeType"), str);

		llogPara[3] = 1;

		llogPara[5] = nIndexOneProbeType;
		// Song chenguang add end 2010-04-10
	}
	else if(wWorkMode == 2 || wWorkMode == 4 || wWorkMode == 5 || wWorkMode == 7 ||
			wWorkMode == 8)	// 单2
	{
		CString str;
		str.Format(_T("%d"), 1);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeNum"), str);

		nIndexOneProbeType = 1;
		str.Format(_T("%d"), nIndexOneProbeType);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("OneProbeType"), str);

		llogPara[3] = 1;

		llogPara[5] = nIndexOneProbeType;
	}
	else if (wWorkMode == 3)	// 双单
	{
		CString str;
		str.Format(_T("%d"), 2);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeNum"), str);

		nIndexTwoProbeType = 0;
		str.Format(_T("%d"), nIndexTwoProbeType);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("TwoProbeType"), str);

		llogPara[3] = 2;

		llogPara[5] = nIndexTwoProbeType;
	}
	else if (wWorkMode == 6)	// 单晶关联
	{
		CString str;
		str.Format(_T("%d"), 1);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeNum"), str);

		llogPara[3] = 1;

	}

	//根据控件变量设置系统参数
	pMovePathView->m_GraphMove.SetPara(dSysPara,llogPara);

	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.SetChannelState(llogPara[3], nIndexOneProbeType, 
		nIndexTwoProbeType, nIndexThreeProbeType);
}
*/
// wangbingfu add start at 2012-11-19
void CChannelPanel::OnBnClickedCheckExtension()
{
	// TODO: Add your control notification handler code here
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckExtension"),"Enter");
	UpdateData(TRUE);

	if(m_bExtendFlag)
	{
		RfExtendOn();
	}
	else
	{
		RfExtendOff();
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::OnBnClickedCheckExtension"),"Leave");
}

void CChannelPanel::RfExtendOn()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::RfExtendOn"),"Enter");
	
	m_bExtendArrayFlag[m_nChannelNo] = TRUE;
	int n = m_cmbGateNo.GetCurSel();
	float fGateFront = m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_fFrontGate;
	float fGateBack = m_ChannelInfo[m_nChannelNo].m_GateInfo[n].m_fBackGate;
	m_nOldOscillationMode[m_nChannelNo] = m_ProbeInfoDlg.m_nOscillationMode;
	m_ProbeInfoDlg.m_nOscillationMode = 3;
	m_fOldDelay[m_nChannelNo] = m_ChannelInfo[m_nChannelNo].m_fChannelDelay;
	m_fOldRange[m_nChannelNo] = m_ChannelInfo[m_nChannelNo].m_fChannelRange;
	if(n == 0)
	{
		m_fDelay = Range2MicroSecond(m_nWaveSpeed, fGateFront);
	}
	else
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		int nDotNum = int(pFrame->m_wndChannelInfo.m_pFormView->m_fRange * 2 / (pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed * 1000) * ADFREQUENCY * 1E6);
		int nBeiShu = nDotNum / 16384 + 1;
		float fSync = pFrame->m_pFeatureWnd->m_GateFeature.m_pfFeatures[0][0] * pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed / 1000.0f / 2.0f * nBeiShu;
		m_fDelay = Range2MicroSecond(m_nWaveSpeed, fGateFront + fSync);
	}
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eDisplayMode = (DETECT_OSCILLATION_MODE_e)m_ProbeInfoDlg.m_nOscillationMode;		
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stConfig.wDetectMode = m_ProbeInfoDlg.m_nOscillationMode;		// 阻抗

	// wanbingfu modify start on  2013-01-09
	/*	if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
	{
	int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);
	m_ChannelInfo[nOtherChannelIndex].m_ProbeInfo.m_eDisplayMode = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eDisplayMode;
	pDoc->m_stAllCH_CONFIG[nOtherChannelIndex].stConfig.wDetectMode = pDoc->m_stAllCH_CONFIG[m_nChannelNo].stConfig.wDetectMode;
	}*/
	if(pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nAxisIndex == 4)
	{
		CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
		int nPipeGuanLianCard = pCScanView->GetPipeGuanLianCard();
		int nCardNum = m_nChannelNo % 2;
		if(nPipeGuanLianCard == nCardNum)
		{
			int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

			m_ChannelInfo[nOtherChannelIndex].m_ProbeInfo.m_eDisplayMode = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eDisplayMode;
			pDoc->m_stAllCH_CONFIG[nOtherChannelIndex].stConfig.wDetectMode = pDoc->m_stAllCH_CONFIG[m_nChannelNo].stConfig.wDetectMode;
		}
	}
	else
	{
		if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
		{
			int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

			m_ChannelInfo[nOtherChannelIndex].m_ProbeInfo.m_eDisplayMode = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eDisplayMode;
			pDoc->m_stAllCH_CONFIG[nOtherChannelIndex].stConfig.wDetectMode = pDoc->m_stAllCH_CONFIG[m_nChannelNo].stConfig.wDetectMode;
		}
	}
	// wanbingfu modify end  2013-01-09
	DoModifyDelay();
	m_fRange = fGateBack;
	DoModifyRange();
	
	m_fOldPlus[m_nChannelNo] = m_ChannelInfo[m_nChannelNo].m_fChannelPlus;
	float fPlus = m_ChannelInfo[m_nChannelNo].m_fChannelPlus;

	if(fPlus >= RF_REDUCE_DB && !m_bRfReduced[m_nChannelNo])
	{
		fPlus -= RF_REDUCE_DB;
		 
	}

	if(fPlus > 0)
	{
		m_fPlus = fPlus;
		DoModifyPlus();
	}	
	m_bRfReduced[m_nChannelNo] = TRUE;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::RfExtendOn"),"Leave");
}

void CChannelPanel::RfExtendOff()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::RfExtendOff"),"Enter");
	
	m_bExtendArrayFlag[m_nChannelNo] = FALSE;
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)GetDocument();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_ProbeInfoDlg.m_nOscillationMode = m_nOldOscillationMode[m_nChannelNo];
	m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eDisplayMode = (DETECT_OSCILLATION_MODE_e)m_nOldOscillationMode[m_nChannelNo];
	pDoc->m_stAllCH_CONFIG[m_nChannelNo].stConfig.wDetectMode = m_nOldOscillationMode[m_nChannelNo];
	
	// wanbingfu modify start on  2013-01-09
/*	if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
	{
	int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);
	m_ChannelInfo[nOtherChannelIndex].m_ProbeInfo.m_eDisplayMode = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eDisplayMode;
	pDoc->m_stAllCH_CONFIG[nOtherChannelIndex].stConfig.wDetectMode = pDoc->m_stAllCH_CONFIG[m_nChannelNo].stConfig.wDetectMode;
	}*/
	if(pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nAxisIndex == 4)
	{
		CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
		int nPipeGuanLianCard = pCScanView->GetPipeGuanLianCard();
		int nCardNum = m_nChannelNo % 2;
		if(nPipeGuanLianCard == nCardNum)
		{
			int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

			m_ChannelInfo[nOtherChannelIndex].m_ProbeInfo.m_eDisplayMode = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eDisplayMode;
			pDoc->m_stAllCH_CONFIG[nOtherChannelIndex].stConfig.wDetectMode = pDoc->m_stAllCH_CONFIG[m_nChannelNo].stConfig.wDetectMode;
		}
	}
	else
	{
		if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
		{
			int nOtherChannelIndex = CalGroupOtherIndex(m_nChannelNo);

			m_ChannelInfo[nOtherChannelIndex].m_ProbeInfo.m_eDisplayMode = m_ChannelInfo[m_nChannelNo].m_ProbeInfo.m_eDisplayMode;
			pDoc->m_stAllCH_CONFIG[nOtherChannelIndex].stConfig.wDetectMode = pDoc->m_stAllCH_CONFIG[m_nChannelNo].stConfig.wDetectMode;
		}
	}
	// wanbingfu modify end  2013-01-09
	m_fDelay = m_fOldDelay[m_nChannelNo];
	DoModifyDelay();
	m_fRange = m_fOldRange[m_nChannelNo];
	DoModifyRange();

	if(m_bRfReduced[m_nChannelNo])
	{
		m_bRfReduced[m_nChannelNo] = FALSE;
		m_fPlus = m_fOldPlus[m_nChannelNo];
		DoModifyPlus();
	}
	
	m_bExtendFlag = FALSE;
	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::RfExtendOff"),"Leave");
}
// wangbingfu add end at 2012-11-21
void CChannelPanel::OnBnClickedCheckGateAdjust()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

//yachang.huang add start on 2013-06-29
void CChannelPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetDlgItem(IDC_COMBO_GATE_NO)->GetWindowRect(&rect);
	//ScreenToClient(&rect);
	if (!PtInRect(&rect,point))
	{
		GetDlgItem(IDC_BUTTON_READ_PARAM)->SetFocus();
	}
	else
	{
		GetDlgItem(IDC_COMBO_GATE_NO)->SetFocus();
	}
}
//yachang.huang add end on 2013-06-29

void CChannelPanel::OnReleasedcaptureSliderReFreq(NMHDR *,LRESULT *)
{
	m_nReFreq = m_sliderReFreq.GetPos();

	DoModifyReFreq();
}

int CChannelPanel::MicroSecond2DotChannel( const float& fMicroSecond )
{

	int nDotNum = int(m_fRange * 2 / (m_nWaveSpeed * 1000) * ADFREQUENCY * 1E6);
	int nBeiShu = nDotNum / 16384;
	int nADFREQUENCY = ADFREQUENCY / (nBeiShu + 1);

	int ret = static_cast<int>( fMicroSecond * 1 * nADFREQUENCY );

	if ( ret == 0 )
		ret = 1;
	return ret;
}

float CChannelPanel::Dot2RangeChannel( float VSound, int DotNo )
{
	int nDotNum = int(m_fRange * 2 / (m_nWaveSpeed * 1000) * ADFREQUENCY * 1E6);
	int nBeiShu = nDotNum / 16384;
	int nADFREQUENCY = ADFREQUENCY / (nBeiShu + 1); 

	float temp;
	temp = static_cast<float>( DotNo / 2.0 * 1000.0 * VSound / float( nADFREQUENCY ) / 1000000.0 ); 
	return temp;
}

DWORD CChannelPanel::Range2DotChannel( float VSound, float Range )
{
	int nDotNum = int(m_fRange * 2 / (m_nWaveSpeed * 1000) * ADFREQUENCY * 1E6);
	int nBeiShu = nDotNum / 16384;
	int nADFREQUENCY = ADFREQUENCY / (nBeiShu + 1); 

	DWORD temp;
	temp = static_cast<DWORD>( Range * 2 / 1000.0 / VSound * nADFREQUENCY * 1000000.0 );
	return temp;
}

float CChannelPanel::Dot2MicroSecondChannel( const int& nDot )
{
	int nDotNum = int(m_fRange * 2 / (m_nWaveSpeed * 1000) * ADFREQUENCY * 1E6);
	int nBeiShu = nDotNum / 16384;
	int nADFREQUENCY = ADFREQUENCY / (nBeiShu + 1); 

	return float( nDot ) / nADFREQUENCY;
}
int CChannelPanel::MicroSecond2DotChannel100( const float& fMicroSecond )
{

	int ret = static_cast<int>( fMicroSecond * 1 * 100 );

	if ( ret == 0 )
		ret = 1;
	return ret;
}

float CChannelPanel::Dot2RangeChannel100( float VSound, int DotNo )
{
	int nDotNum = int(m_fRange * 2 / (m_nWaveSpeed * 1000) * ADFREQUENCY * 1E6);
	int nBeiShu = nDotNum / 16384 + 1;
	//int nADFREQUENCY = ADFREQUENCY / (nBeiShu + 1); 

	float temp;
	temp = static_cast<float>(( DotNo / 2.0 * 1000.0 * VSound / float( ADFREQUENCY ) / 1000000.0 ) * nBeiShu); 
	return temp;
}

DWORD CChannelPanel::Range2DotChannel100( float VSound, float Range )
{
	int nDotNum = int(m_fRange * 2 / (m_nWaveSpeed * 1000) * ADFREQUENCY * 1E6);
	int nBeiShu = nDotNum / 16384 + 1;
	//int nADFREQUENCY = ADFREQUENCY / (nBeiShu + 1); 

	DWORD temp;
	//temp = static_cast<DWORD>( Range * 2 / 1000.0 / VSound * nADFREQUENCY * 1000000.0 );
	temp = static_cast<DWORD>((Range * 2 / (VSound * 1000) * ADFREQUENCY * 1E6) / nBeiShu);
	return temp;
}

float CChannelPanel::Dot2MicroSecondChannel100( const int& nDot )
{



	return float( nDot ) / 100;
}