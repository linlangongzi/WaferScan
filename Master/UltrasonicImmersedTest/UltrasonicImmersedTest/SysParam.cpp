// SysParam.cpp : 实现文件
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "UltrasonicImmersedTestDoc.h"
#include "CScanDlgBar.h"
#include "SysParam.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "ChannelInfoDockView.h"
#include "ProbeInfoDlg.h"
#include "DataDefine.h"
#include "ChannelPanel.h"
#include "MechanicalControl.h"
#include "CScanParamDockBar.h"
#include "MechanicalControlPanel.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "MovePathView.h"
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
#include "ProbeLocationDlg.h"
// CSysParam 对话框

IMPLEMENT_DYNAMIC(CSysParam, CDialog)

CSysParam::CSysParam(CWnd* pParent /*=NULL*/)
	: CDialog(CSysParam::IDD, pParent)
	, m_dxl(0)
	, m_dyl(0)
	, m_dzl(0)
	, m_dxacc(0)
	, m_dyacc(0)
	, m_dzacc(0)
	, m_dXResetVel(0)
	, m_dYResetVel(0)
	, m_dZResetVel(0)
	, m_dXMoveVel(0)
	, m_dYMoveVel(0)
	, m_dZMoveVel(0)
	, m_dXKongVel(0)
	, m_dYKongVel(0)
	, m_dZKongVel(0)
	, m_dZHigh(0)
	, m_Time(0)
	, m_bUse(FALSE)
	, m_bKeDu(FALSE)
	, m_bPath(TRUE)
	, m_bCatch(FALSE)
	, m_nFillType(0)
	, m_dFillSpace(1.0)
	, m_dFillSpace2(1.0)
	, m_bUseSafeArea(FALSE)
	, m_fPerLength(10000)
	, m_fProbeDistance(0)
	, m_nProbeNum(0)
	, m_fSafeMinX(0)
	, m_fSafeMaxX(1000)
	, m_fSafeMinY(0)
	, m_fSafeMaxY(2000)
	, m_fSafeMaxZ(300)
	, m_nScanType(0)
	, m_nIndexProbeNum(0)
	, m_nIndexOneProbeType(0)
	, m_nIndexTwoProbeType(0)
	, m_nIndexThreeProbeType(0)
	, m_nAxisIndex(0)
	, m_fChongDie(0)	
	, m_nStepDirect(0)
	, m_nScanDirect(1)
	, m_nRSpeed(10)
	, m_fShangLiaoX(0)
	, m_fShangLiaoY(0)
	, m_wWorkMode(1)
	, m_nStepMode(0)
	, m_nABSpeed(5)
	, m_nWSpeed(5)
	, m_bTailDecreasePipe(FALSE)
	, m_bChangeZWAxis(FALSE)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::CSysParam"),"Enter");
	::ZeroMemory(m_pbChannelOpen, sizeof(m_pbChannelOpen));
	::ZeroMemory(m_fProbeLocation, sizeof(float)*CHANNEL_NUM*3);

	for (int i=1; i<4; i++)
	{
		m_fProbeLocation[i][1] = static_cast<float>(i*50);//modify by wangqianfeng 20121105
	}

	m_nAxisIndexBack = 0;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::CSysParam"),"Leave");
}

CSysParam::~CSysParam()
{
	
}

void CSysParam::DoDataExchange(CDataExchange* pDX)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::DoDataExchange"),"Enter");
	CDialog::DoDataExchange(pDX);	
	// Song chenguang add start on 2013-08-15 
	CUltrasonicImmersedTestApp* pApp =(CUltrasonicImmersedTestApp*)AfxGetApp();
	// Song chenguang add end on 2013-08-15 
	DDX_Text(pDX, IDC_EDIT_XL, m_dxl);
//	DDV_MinMaxDouble(pDX, m_dxl, 1.0, 6000.0);
	DDX_Text(pDX, IDC_EDIT_YL, m_dyl);
//	DDV_MinMaxDouble(pDX, m_dyl, 1.0, 4000.0);
	DDX_Text(pDX, IDC_EDIT_ZL, m_dzl);
//	DDV_MinMaxDouble(pDX, m_dzl, 1.0, 4000.0);
	DDX_Text(pDX, IDC_EDIT_XA, m_dxacc);
	DDV_MinMaxDouble(pDX, m_dxacc, 1.0, 800.0);
	DDX_Text(pDX, IDC_EDIT_YA, m_dyacc);
	DDV_MinMaxDouble(pDX, m_dyacc, 1.0, 800.0);
	DDX_Text(pDX, IDC_EDIT_ZA, m_dzacc);
	DDV_MinMaxDouble(pDX, m_dzacc, 1.0, 800.0);
	DDX_Text(pDX, IDC_EDIT_XREV, m_dXResetVel);
	DDV_MinMaxDouble(pDX, m_dXResetVel, 1.0, 300.0);
	DDX_Text(pDX, IDC_EDIT_YREV, m_dYResetVel);
	DDV_MinMaxDouble(pDX, m_dYResetVel, 1.0, 300.0);
	DDX_Text(pDX, IDC_EDIT_ZREV, m_dZResetVel);
	DDV_MinMaxDouble(pDX, m_dZResetVel, 1.0, 100.0);
	DDX_Text(pDX, IDC_EDIT_XMV, m_dXMoveVel);
	DDV_MinMaxDouble(pDX, m_dXMoveVel, 1.0, 400.0);
	DDX_Text(pDX, IDC_EDIT_YMV, m_dYMoveVel);
	DDV_MinMaxDouble(pDX, m_dYMoveVel, 1.0, 400.0);
	DDX_Text(pDX, IDC_EDIT_ZMV, m_dZMoveVel);
	DDV_MinMaxDouble(pDX, m_dZMoveVel, 0.0, 400.0);
	DDX_Text(pDX, IDC_EDIT_XKV, m_dXKongVel);
	DDV_MinMaxDouble(pDX, m_dXKongVel, 1.0, 400.0);
	DDX_Text(pDX, IDC_EDIT_YKV, m_dYKongVel);
	DDV_MinMaxDouble(pDX, m_dYKongVel, 1.0, 400.0);
	DDX_Text(pDX, IDC_EDIT_ZKV, m_dZKongVel);
	DDV_MinMaxDouble(pDX, m_dZKongVel, 1.0, 400.0);	
	DDX_Text(pDX, IDC_EDIT_TIME, m_Time);
	DDV_MinMaxDouble(pDX, m_Time, 0.0, 10);
	DDX_Check(pDX, IDC_CHECK_USE, m_bUse);
	DDX_Check(pDX, IDC_CHECK_KEDU, m_bKeDu);
	DDX_Check(pDX, IDC_CHECK_PATH, m_bPath);
	DDX_Check(pDX, IDC_CHECK_CATCH, m_bCatch);
	DDX_Text(pDX, IDC_EDIT_JIAN, m_dFillSpace);
	DDV_MinMaxDouble(pDX, m_dFillSpace, 0.1, 50.0);
	DDX_Text(pDX, IDC_EDIT_JIAN2, m_dFillSpace2);
	DDV_MinMaxDouble(pDX, m_dFillSpace, 0.1, 50.0);
	DDX_Check(pDX, IDC_CHECK_SAFE_AREA, m_bUseSafeArea);
	DDX_Text(pDX, IDC_EDIT_PER_LENGTH, m_fPerLength);
	DDV_MinMaxDouble(pDX, m_fPerLength, 1, 10000);
	DDX_Text(pDX, IDC_EDIT_PROBE_DISTANCE, m_fProbeDistance);
	DDV_MinMaxDouble(pDX, m_fProbeDistance, 0, 300);
	DDX_Text(pDX, IDC_EDIT_SAFE_MINX, m_fSafeMinX);
	DDV_MinMaxDouble(pDX, m_fSafeMinX, 1, 4000);
	DDX_Text(pDX, IDC_EDIT_SAFE_MAXX, m_fSafeMaxX);
	DDV_MinMaxDouble(pDX, m_fSafeMaxX, m_fSafeMinX+1, pApp->m_IniParams.m_info.fXRange);	// Song Chenguang modify on 2013-08-15
	DDX_Text(pDX, IDC_EDIT_SAFE_MINY, m_fSafeMinY);
	DDV_MinMaxDouble(pDX, m_fSafeMinY, 1, 4000);
	DDX_Text(pDX, IDC_EDIT_SAFE_MAXY, m_fSafeMaxY);
	DDV_MinMaxDouble(pDX, m_fSafeMaxY, m_fSafeMinY+1, pApp->m_IniParams.m_info.fYRange);	// Song Chenguang modify on 2013-08-15
	DDX_Text(pDX, IDC_EDIT_SAFE_MAXZ, m_fSafeMaxZ);
	DDV_MinMaxDouble(pDX, m_fSafeMaxZ, 0, pApp->m_IniParams.m_info.fZRange);		// Song Chenguang modify on 2013-08-15
	DDX_Text(pDX, IDC_EDIT_ZHIGH, m_dZHigh);
	DDV_MinMaxDouble(pDX, m_dZHigh, 0.0, m_fSafeMaxZ);		// Song Chenguang modify on 2013-08-15
	DDX_Text(pDX, IDC_EDIT_R_SPEED, m_nRSpeed);
	DDV_MinMaxInt(pDX, m_nRSpeed, 1, 55);
	DDX_Radio(pDX, IDC_RADIO_SCAN_TYPE_STEP, m_nScanType);
	DDX_Control(pDX, IDC_COMBO_PROBE_NUM, m_cmbProbeNum);
	DDX_Control(pDX, IDC_COMBO_ONE_PROBE_TYPE, m_cmbOneProbeType);
	DDX_Control(pDX, IDC_COMBO_TWO_PROBE_TYPE, m_cmbTwoProbeType);
	DDX_Control(pDX, IDC_COMBO_THREE_PROBE_TYPE, m_cmbThreeProbeType);
	DDX_Control(pDX, IDC_COMBO_FOUR_PROBE_TYPE, m_cmbFourProbeType);
	DDX_Control(pDX, IDC_COMBO_AXIS, m_cmbAxis);
	DDX_Text(pDX, IDC_EDIT_CHONGDIE_LENGTH, m_fChongDie);
	DDV_MinMaxDouble(pDX, m_fChongDie, 0, 100);
	DDX_Radio(pDX, IDC_RADIO_STEP_X, m_nStepDirect);
	DDX_Radio(pDX, IDC_RADIO_SCAN_X, m_nScanDirect);
	DDX_Text(pDX, IDC_EDIT_SHANGLIAO_X, m_fShangLiaoX);
	DDX_Text(pDX, IDC_EDIT_SHANGLIAO_Y, m_fShangLiaoY);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::DoDataExchange"),"Leave");
	DDX_Control(pDX, IDC_COMBO_STEP_MODE, m_cmbStepMode);
	DDX_Text(pDX, IDC_EDIT_AB_SPEED, m_nABSpeed);
	DDV_MinMaxInt(pDX, m_nABSpeed, 1, 15);
	DDX_Text(pDX, IDC_EDIT_W_SPEED, m_nWSpeed);
	DDV_MinMaxInt(pDX, m_nWSpeed, 1, 15);
	DDX_Check(pDX, IDC_CHECK_TAILDECREASEPIPE, m_bTailDecreasePipe);
	DDX_Check(pDX, IDC_CHECK_CHANGE_Z_W, m_bChangeZWAxis);
}


BEGIN_MESSAGE_MAP(CSysParam, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_PROBE_NUM, &CSysParam::OnCbnSelchangeComboProbeNum)
	ON_BN_CLICKED(IDOK, &CSysParam::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_SCAN_TYPE_STEP, &CSysParam::OnBnClickedScanTypeStep)
	ON_BN_CLICKED(IDC_RADIO_SCAN_TYPE_DEPTH, &CSysParam::OnBnClickedScanTypeDepth)
	ON_BN_CLICKED(IDCANCEL, &CSysParam::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_AXIS, &CSysParam::OnCbnSelchangeComboAxis)
	ON_BN_CLICKED(IDC_RADIO_STEP_X, &CSysParam::OnBnClickedRadioStepX)
	ON_BN_CLICKED(IDC_RADIO_STEP_Y, &CSysParam::OnBnClickedRadioStepY)
	ON_BN_CLICKED(IDC_RADIO_STEP_Z, &CSysParam::OnBnClickedRadioStepZ)
	ON_BN_CLICKED(IDC_RADIO_SCAN_X, &CSysParam::OnBnClickedRadioScanX)
	ON_BN_CLICKED(IDC_RADIO_SCAN_Y, &CSysParam::OnBnClickedRadioScanY)
	ON_BN_CLICKED(IDC_RADIO_SACN_Z, &CSysParam::OnBnClickedRadioSacnZ)
	ON_BN_CLICKED(IDC_BUTTON_PROBE_LOCATION, &CSysParam::OnBnClickedProbeLocation)
	ON_CBN_SELCHANGE(IDC_COMBO_WORK_MODE, &CSysParam::OnCbnSelchangeComboWorkMode)
	ON_CBN_SELCHANGE(IDC_COMBO_STEP_MODE, &CSysParam::OnCbnSelchangeComboStepMode)
	ON_BN_CLICKED(IDC_BUTTON_PIPE_SET, &CSysParam::OnBnClickedButtonPipeSet)
	// machao add start on 2013-08-27
	ON_BN_CLICKED(IDC_RADIO_STEP_W, &CSysParam::OnBnClickedRadioStepW)
	ON_BN_CLICKED(IDC_RADIO_SCAN_W, &CSysParam::OnBnClickedRadioScanW)
	// machao add end on 2013-08-27
END_MESSAGE_MAP()


// CSysParam 消息处理程序

BOOL CSysParam::OnInitDialog()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnInitDialog"),"Enter");
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_cmbProbeNum.ResetContent();

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();

	for (int i=0; i<nCardNum; i++)
	{
		CString str;
		str.Format(_T("%d"), i+1);
		m_cmbProbeNum.AddString(str);
		m_cmbOneProbeType.AddString(str);
	}
/*	for (int i=0; i<2; i++)
	{
		CString str;
		str.Format(_T("%d"), i+1);
		m_cmbProbeNum.AddString(str);
		m_cmbOneProbeType.AddString(str);
	}*/
	m_cmbProbeNum.SetCurSel(m_nIndexProbeNum);
	m_cmbOneProbeType.SetCurSel(m_nIndexOneProbeType);
	
	m_cmbTwoProbeType.ResetContent();
	// wangbingfu modify start at 2012-9-24
/*	CString strTwoProbeType[6] = {_T("1, 2"), _T("2, 3"), _T("3, 4"), _T("1, 3"), _T("2, 4"), _T("1, 4")};
	for (int i=0; i<6; i++)
	{
		m_cmbTwoProbeType.AddString(strTwoProbeType[i]);
	}*/
	m_cmbTwoProbeType.AddString(_T("1, 2"));
	// wangbingfu modify end at 2012-9-24

	m_cmbTwoProbeType.SetCurSel(m_nIndexTwoProbeType);

	m_cmbThreeProbeType.ResetContent();
	CString strThreeProbeType1[2] = {_T("1, 2, 3"), _T("2, 3, 4")};
	CString strThreeProbeType2[4] = {_T("1, 2, 3"), _T("1, 2, 4"), _T("1, 3, 4"), _T("2, 3, 4")};

	if (m_nScanType == 0)
	{
		m_cmbThreeProbeType.AddString(strThreeProbeType1[0]);
		m_cmbThreeProbeType.AddString(strThreeProbeType1[1]);
	}
	else
	{
		for (int i=0; i<4; i++)
		{
			m_cmbThreeProbeType.AddString(strThreeProbeType2[i]);
		}
	}
	m_cmbThreeProbeType.SetCurSel(m_nIndexThreeProbeType);

	m_cmbFourProbeType.ResetContent();
	m_cmbFourProbeType.AddString(_T("1, 2, 3, 4"));
	m_cmbFourProbeType.SetCurSel(0);

	OnCbnSelchangeComboProbeNum();

	m_cmbAxis.ResetContent();

	CString strAxis[6] = {_T("XY"), _T("XZ"), _T("YZ"),_T("R棒"), _T("R管"), _T("W")};
	for (int i=0; i<6; i++)
	{
		m_cmbAxis.AddString(strAxis[i]);
	}
	
	m_cmbAxis.SetCurSel(m_nAxisIndex);
	m_nAxisIndexBack = m_nAxisIndex;	// Song Chenguang add on 2013-04-12

	if(m_nAxisIndex == 0)
	{
		if(m_nFillType == 0) // 横向
		{
			m_nStepDirect = 1;
			m_nScanDirect = 0;
		}
		else	// 纵向
		{
			m_nStepDirect = 0;
			m_nScanDirect = 1;
		}
		GetDlgItem(IDC_RADIO_STEP_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STEP_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STEP_Y)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_SCAN_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_Y)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_R_SPEED)->EnableWindow(FALSE);
		// machao add start on 2013-08-26
		GetDlgItem(IDC_RADIO_STEP_W)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_W)->EnableWindow(FALSE);
		// machao add end on 2013-08-26
	}
	else if(m_nAxisIndex == 1)
	{
		if(m_nFillType == 0) // 横向
		{
			m_nStepDirect = 2;
			m_nScanDirect = 0;
		}
		else	// 纵向
		{
			m_nStepDirect = 0;
			m_nScanDirect = 2;
		}
		GetDlgItem(IDC_RADIO_STEP_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STEP_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STEP_Z)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_SCAN_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_Z)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_R_SPEED)->EnableWindow(FALSE);
		// machao add start on 2013-08-26
		GetDlgItem(IDC_RADIO_STEP_W)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_W)->EnableWindow(FALSE);
		// machao add end on 2013-08-26
	}
	else if(m_nAxisIndex == 2)
	{
		if(m_nFillType == 0) // 横向
		{
			m_nStepDirect = 2;
			m_nScanDirect = 1;
		}
		else	// 纵向
		{
			m_nStepDirect = 1;
			m_nScanDirect = 2;
		}
		GetDlgItem(IDC_RADIO_STEP_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STEP_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STEP_Z)->EnableWindow(TRUE);

		GetDlgItem(IDC_RADIO_SCAN_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_Z)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_R_SPEED)->EnableWindow(FALSE);
		// machao add start on 2013-08-26
		GetDlgItem(IDC_RADIO_STEP_W)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_W)->EnableWindow(FALSE);
		// machao add end on 2013-08-26
	}
	// machao modify start on 2013-08-26
//	else if(m_nAxisIndex == 3)
	else if(m_nAxisIndex == 3
		|| m_nAxisIndex == 4)
	// machao modify end on 2013-08-26
	{
		GetDlgItem(IDC_RADIO_STEP_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STEP_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STEP_Z)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_SCAN_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_Z)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_R_SPEED)->EnableWindow(TRUE);
		// machao add start on 2013-08-26
		GetDlgItem(IDC_RADIO_STEP_W)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_W)->EnableWindow(FALSE);
		// machao add end on 2013-08-26
	}
	// machao add start on 2013-08-26
	else if (m_nAxisIndex == 5)
	{
		if(m_nFillType == 0) // 横向
		{
			m_nStepDirect = 2;
			m_nScanDirect = 3;
		}
		else	// 纵向
		{
			m_nStepDirect = 3;
			m_nScanDirect = 2;
		}
		GetDlgItem(IDC_RADIO_STEP_W)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_W)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STEP_Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STEP_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STEP_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_R_SPEED)->EnableWindow(FALSE);
	}
	// machao add end on 2013-08-26
	else
	{
		;//
	}

	// 工作模式
	CComboBox* pcmbWorkMode = (CComboBox*)GetDlgItem(IDC_COMBO_WORK_MODE);
	pcmbWorkMode->SetCurSel(int(m_wWorkMode - 1));
	//wangbingfu add start on 2012-12-26
	m_cmbStepMode.SetCurSel(m_nStepMode);
	//wangbingfu add end on 2012-12-26
	// Song Chenguang add start on 2013-09-13
#if _HAFEI
	GetDlgItem(IDC_CHECK_TAILDECREASEPIPE)->ShowWindow(TRUE);
#else
	GetDlgItem(IDC_CHECK_TAILDECREASEPIPE)->ShowWindow(FALSE);
#endif

#if _BEIFANG_ZHONGGONG
	GetDlgItem(IDC_CHECK_CHANGE_Z_W)->ShowWindow(TRUE);
#else
	GetDlgItem(IDC_CHECK_CHANGE_Z_W)->ShowWindow(FALSE);
#endif
	// Song Chenguang add end on 2013-09-13
	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnInitDialog"),"Leave");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSysParam::OnCbnSelchangeComboProbeNum()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnCbnSelchangeComboProbeNum"),"Enter");
	// TODO: Add your control notification handler code here
	int nIndex = m_cmbProbeNum.GetCurSel();

	if (nIndex == 0)
	{
		m_cmbOneProbeType.EnableWindow(TRUE);
		m_cmbTwoProbeType.EnableWindow(FALSE);
		m_cmbThreeProbeType.EnableWindow(FALSE);
		m_cmbFourProbeType.EnableWindow(FALSE);
	}
	else if (nIndex == 1)
	{
		m_cmbOneProbeType.EnableWindow(FALSE);
		m_cmbTwoProbeType.EnableWindow(TRUE);
		m_cmbThreeProbeType.EnableWindow(FALSE);
		m_cmbFourProbeType.EnableWindow(FALSE);
	}
	else if (nIndex == 2)
	{
		m_cmbOneProbeType.EnableWindow(FALSE);
		m_cmbTwoProbeType.EnableWindow(FALSE);
		m_cmbThreeProbeType.EnableWindow(TRUE);
		m_cmbFourProbeType.EnableWindow(FALSE);
	}
	else if (nIndex == 3)
	{
		m_cmbOneProbeType.EnableWindow(FALSE);
		m_cmbTwoProbeType.EnableWindow(FALSE);
		m_cmbThreeProbeType.EnableWindow(FALSE);
		m_cmbFourProbeType.EnableWindow(TRUE);
	}
	else
	{
	}

	m_nProbeNum = nIndex + 1;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnCbnSelchangeComboProbeNum"),"Leave");
}


void CSysParam::OnBnClickedOk()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedOk"),"Enter");
	// WangQianfeng modify start on 2013-06-26
	//UpdateData方法对控件值进行CHECK，如果不合法则不应该继续执行下面的代码
	BOOL bRet = UpdateData(TRUE);
	if (!bRet)
	{
		return;
	}
	// WangQianfeng modify end on 2013-06-26
	// DongQi add start on 2013-06-27
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_nAxisIndex = m_cmbAxis.GetCurSel();
	int nAxisIndex = m_nAxisIndex;

	CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
	pMovePathView->m_GraphMove.SetAxisSelect(nAxisIndex);

	CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CCScanView* pCScanView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
	pCScanView->m_nAxisSelect = nAxisIndex;
	pCScanView2->m_nAxisSelect = nAxisIndex;
	// DongQi add end on 2013-06-27
	// 下发工作模式
	CUltrasonicImmersedTestApp* pApp =(CUltrasonicImmersedTestApp*)AfxGetApp();
	// DongQi delete start on 2013-06-27
//	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	// DongQi delete end on 2013-06-27
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)pFrame->GetActiveDocument();
	// wangbingfu add start on 2013-01-09
/*
	// rem by Song Chenguang on 2013-04-12
	if(m_nAxisIndex == 4)	// W1管
	{
		m_wWorkMode = 3; // 双单
	}
*/
	// DongQi delete start on 2013-06-27
//	CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	// DongQi delete end on 2013-06-27
	// wangbingfu add end on 2013-01-09
	int nCardNum = pApp->GetCardNum();
	for (int i=0; i<nCardNum; i++)
	{
		// 若为关联模式，将奇数通道参数赋给偶数通道
		if (m_wWorkMode > 3)
		{
			memcpy(&(pChannelPanel->m_ChannelInfo[2*i+1]), &(pChannelPanel->m_ChannelInfo[2*i]), sizeof(CChannelInfo));
			memcpy(&pDoc->m_stAllCH_CONFIG[2*i+1], &pDoc->m_stAllCH_CONFIG[2*i], sizeof(CSCAN_ARS200FPGA_SETONE));
		}
		
		// wangbingfu add start on 2013-01-09
		if(m_nAxisIndex == 4)	// W1管
		{
			int nPipeGuanLianCard = pCScanView->GetPipeGuanLianCard();
			if(nPipeGuanLianCard == i)
			{
				memcpy(&(pChannelPanel->m_ChannelInfo[2*i+1]), &(pChannelPanel->m_ChannelInfo[2*i]), sizeof(CChannelInfo));
				memcpy(&pDoc->m_stAllCH_CONFIG[2*i+1], &pDoc->m_stAllCH_CONFIG[2*i], sizeof(CSCAN_ARS200FPGA_SETONE));
			}
		}
		// wangbingfu add end on 2013-01-09
		pChannelPanel->SendWorkMode(i, m_wWorkMode);	// Song Chenguang rem on 2013-07-19 后面设定
	}
	
	pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	if(m_wWorkMode == 4 || m_wWorkMode == 5 ||
		m_wWorkMode == 6 ||m_wWorkMode == 7 ||
		m_wWorkMode == 8) // 关联
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
/*
	// 模式为单2、双晶, 切换到偶数通道,
	int nYu = m_nChannelNo % 2;
	if (m_wWorkMode == 2 || m_wWorkMode == 4 || m_wWorkMode == 5 ||
		m_wWorkMode == 7 || m_wWorkMode == 8)
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
	m_nIndexProbeNum = m_cmbProbeNum.GetCurSel();
	m_nIndexOneProbeType = m_cmbOneProbeType.GetCurSel();
	m_nIndexTwoProbeType = m_cmbTwoProbeType.GetCurSel();
	m_nIndexThreeProbeType = m_cmbThreeProbeType.GetCurSel();
	m_nStepMode = m_cmbStepMode.GetCurSel();
//	SetChannelState(m_nIndexProbeNum+1, m_nIndexOneProbeType, 
//		m_nIndexTwoProbeType, m_nIndexThreeProbeType);
	SetChannelStateNew(m_nIndexProbeNum+1, m_nIndexOneProbeType, 
				m_nIndexTwoProbeType, m_nIndexThreeProbeType, m_wWorkMode);

//	SetMoveRule(m_nIndexProbeNum+1, m_nIndexOneProbeType, 
//		m_nIndexTwoProbeType, m_nIndexThreeProbeType, m_wWorkMode);
	// DongQi delete start on 2013-06-27
//	int nAxisIndex = m_nAxisIndex;

//	CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
	// DongQi delete end on 2013-06-27
	pMovePathView->m_GraphMove.SetAxisSelect(nAxisIndex);

	if(nAxisIndex == 3
		|| nAxisIndex == 4) // wangbingfu add on 2013-01-09
	{
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_R_POSITIVE)->EnableWindow(TRUE);
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_R_REVERSE)->EnableWindow(TRUE);
	}
	else
	{
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_R_POSITIVE)->EnableWindow(FALSE);
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_R_REVERSE)->EnableWindow(FALSE);;
	}
/*
	// rem by Song Chenguang om 2013-10-22
	// 不控制了
	// Song Chenguang add start on 2013-07-02
	if (nAxisIndex == 5)
	{
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W_POSITIVE)->EnableWindow(TRUE);
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W_REVERSE)->EnableWindow(TRUE);
	} 
	else
	{
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W_POSITIVE)->EnableWindow(FALSE);
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->GetDlgItem(IDC_BUTTON_W_REVERSE)->EnableWindow(FALSE);
	}
*/
	// Song Chenguang add end on 2013-07-02
	// 设定机械面板采点画矩形处，列表列名
	if (nAxisIndex == 0)
	{
		LVCOLUMN col;

		col.mask = LVCF_TEXT;
		col.pszText = _T("X");		
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetColumn(1, &col);
		col.pszText = _T("Y");		
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetColumn(2, &col);
		col.pszText = _T("Z");		
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetColumn(3, &col);
	} 
	else if (nAxisIndex == 1)
	{
		LVCOLUMN col;

		col.mask = LVCF_TEXT;
		col.pszText = _T("X");		
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetColumn(1, &col);
		col.pszText = _T("Z");		
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetColumn(2, &col);
		col.pszText = _T("Y");		
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetColumn(3, &col);
	}
	else if (nAxisIndex == 2)
	{
		LVCOLUMN col;

		col.mask = LVCF_TEXT;
		col.pszText = _T("Y");		
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetColumn(1, &col);
		col.pszText = _T("Z");		
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetColumn(2, &col);
		col.pszText = _T("X");		
		pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetColumn(3, &col);
	}
	else
	{
	}
/*
	for (int i=0; i<2; i++)
	{
		int iSubItem = 0;
		BOOL bRet = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetItemText(i, ++iSubItem, _T("0.0"));	// X
		bRet = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetItemText(i, ++iSubItem, _T("0.0"));	// Y
		bRet = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetItemText(i, ++iSubItem, _T("0.0"));	// Z
	}
*/
	// Song Chenguang add start on 2013-04-06
	if (m_fSafeMaxX > m_dxl)
	{
		MessageBox(_T("X方向安全区最大值超过行程，请重新设置！"));
		return;
	}

	if (m_fSafeMaxY > m_dyl)
	{
		MessageBox(_T("Y方向安全区最大值超过行程，请重新设置！"));
		return;
	}

	if (m_fSafeMaxZ > m_dzl)
	{
		MessageBox(_T("Z方向安全区最大值超过行程，请重新设置！"));
		return;
	}

	// Song Chenguang add start on 2013-04-13
	// 换平面的情况下，空走高度设为0
	if (m_nAxisIndexBack != m_nAxisIndex)
		m_dZHigh = 0;
	// Song Chenguang add end on 2013-04-13

	OnOK();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedOk"),"Leave");
}

void CSysParam::OnBnClickedScanTypeStep()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedScanTypeStep"),"Enter");
	UpdateData(TRUE);

	CString strThreeProbeType1[2] = {_T("1, 2, 3"), _T("2, 3, 4")};
	CString strThreeProbeType2[4] = {_T("1, 2, 3"), _T("1, 2, 4"), _T("1, 3, 4"), _T("2, 3, 4")};

	m_cmbThreeProbeType.ResetContent();
	if (m_nScanType == 0)
	{
		m_cmbThreeProbeType.AddString(strThreeProbeType1[0]);
		m_cmbThreeProbeType.AddString(strThreeProbeType1[1]);
	}
	else
	{
		for (int i=0; i<4; i++)
		{
			m_cmbThreeProbeType.AddString(strThreeProbeType2[i]);
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedScanTypeStep"),"Leave");
}

void CSysParam::OnBnClickedScanTypeDepth()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedScanTypeDepth"),"Enter");
	UpdateData(TRUE);


	CString strThreeProbeType1[2] = {_T("1, 2, 3"), _T("2, 3, 4")};
	CString strThreeProbeType2[4] = {_T("1, 2, 3"), _T("1, 2, 4"), _T("1, 3, 4"), _T("2, 3, 4")};

	m_cmbThreeProbeType.ResetContent();
	if (m_nScanType == 0)
	{
		m_cmbThreeProbeType.AddString(strThreeProbeType1[0]);
		m_cmbThreeProbeType.AddString(strThreeProbeType1[1]);
	}
	else
	{
		for (int i=0; i<4; i++)
		{
			m_cmbThreeProbeType.AddString(strThreeProbeType2[i]);
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedScanTypeDepth"),"Leave");
}


void CSysParam::OnBnClickedCancel()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedCancel"),"Enter");
	OnCancel();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedCancel"),"Leave");
}

void CSysParam::SetChannelState(const int& nProbeNum, 
					 const int& nOneProbeIndex,
					 const int& nTwoProbeIndex,
					 const int& nThreeProbeIndex)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::SetChannelState"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;

	UINT uChannelID[4] = {IDC_RADIO_CH1, IDC_RADIO_CH2, IDC_RADIO_CH3, IDC_RADIO_CH4};

	if (nProbeNum == 1)
	{				
		for (int i = 0; i<4; i++)
		{
			if (i == nOneProbeIndex)
			{
				pChannelPanel->GetDlgItem(uChannelID[i])->EnableWindow(TRUE);
				m_pbChannelOpen[i] = TRUE;
			}
			else
			{
				pChannelPanel->GetDlgItem(uChannelID[i])->EnableWindow(FALSE);
				m_pbChannelOpen[i] = FALSE;
			}
		}
	}
	else if (nProbeNum == 2)
	{
		if (nTwoProbeIndex == 0)
		{
			pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(FALSE);

			m_pbChannelOpen[0] = TRUE;
			m_pbChannelOpen[1] = TRUE;
			m_pbChannelOpen[2] = FALSE;
			m_pbChannelOpen[3] = FALSE;
		} 
		else if (nTwoProbeIndex == 1)
		{
			pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(FALSE);

			m_pbChannelOpen[0] = FALSE;
			m_pbChannelOpen[1] = TRUE;
			m_pbChannelOpen[2] = TRUE;
			m_pbChannelOpen[3] = FALSE;
		}
		else if (nTwoProbeIndex == 2)
		{
			pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(TRUE);

			m_pbChannelOpen[0] = FALSE;
			m_pbChannelOpen[1] = FALSE;
			m_pbChannelOpen[2] = TRUE;
			m_pbChannelOpen[3] = TRUE;
		}
		else if (nTwoProbeIndex == 3)
		{
			pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(FALSE);

			m_pbChannelOpen[0] = TRUE;
			m_pbChannelOpen[1] = FALSE;
			m_pbChannelOpen[2] = TRUE;
			m_pbChannelOpen[3] = FALSE;
		}
		else if (nTwoProbeIndex == 4)
		{
			pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(TRUE);

			m_pbChannelOpen[0] = FALSE;
			m_pbChannelOpen[1] = TRUE;
			m_pbChannelOpen[2] = FALSE;
			m_pbChannelOpen[3] = TRUE;
		}
		else if (nTwoProbeIndex == 5)
		{
			pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(TRUE);

			m_pbChannelOpen[0] = TRUE;
			m_pbChannelOpen[1] = FALSE;
			m_pbChannelOpen[2] = FALSE;
			m_pbChannelOpen[3] = TRUE;
		}
		else
		{
		}
	}
	else if (nProbeNum == 3)
	{
		if (m_nScanType == 0)
		{
			if (nThreeProbeIndex == 0)
			{
				pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(FALSE);

				m_pbChannelOpen[0] = TRUE;
				m_pbChannelOpen[1] = TRUE;
				m_pbChannelOpen[2] = TRUE;
				m_pbChannelOpen[3] = FALSE;
			} 
			else if (nThreeProbeIndex == 1)
			{
				pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(FALSE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(TRUE);

				m_pbChannelOpen[0] = FALSE;
				m_pbChannelOpen[1] = TRUE;
				m_pbChannelOpen[2] = TRUE;
				m_pbChannelOpen[3] = TRUE;
			}		
			else
			{
			}
		} 
		else	// {_T("1, 2, 3"), _T("1, 2, 4"), _T("1, 3, 4"), _T("2, 3, 4")};
		{
			if (nThreeProbeIndex == 0)
			{
				pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(FALSE);

				m_pbChannelOpen[0] = TRUE;
				m_pbChannelOpen[1] = TRUE;
				m_pbChannelOpen[2] = TRUE;
				m_pbChannelOpen[3] = FALSE;
			} 
			else if (nThreeProbeIndex == 1)
			{
				pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(FALSE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(TRUE);

				m_pbChannelOpen[0] = TRUE;
				m_pbChannelOpen[1] = TRUE;
				m_pbChannelOpen[2] = FALSE;
				m_pbChannelOpen[3] = TRUE;
			}
			else if (nThreeProbeIndex == 2)
			{
				pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(FALSE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(TRUE);

				m_pbChannelOpen[0] = TRUE;
				m_pbChannelOpen[1] = FALSE;
				m_pbChannelOpen[2] = TRUE;
				m_pbChannelOpen[3] = TRUE;
			}
			else if (nThreeProbeIndex == 3)
			{
				pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(FALSE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(TRUE);
				pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(TRUE);

				m_pbChannelOpen[0] = FALSE;
				m_pbChannelOpen[1] = TRUE;
				m_pbChannelOpen[2] = TRUE;
				m_pbChannelOpen[3] = TRUE;
			}		
			else
			{
			}
		}
	}
	else if (nProbeNum == 4)
	{
		pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(TRUE);
		pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(TRUE);
		pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(TRUE);
		pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(TRUE);

		m_pbChannelOpen[0] = TRUE;
		m_pbChannelOpen[1] = TRUE;
		m_pbChannelOpen[2] = TRUE;
		m_pbChannelOpen[3] = TRUE;
	}
	else
	{}
	
	//同步闸门开关 2012-4-1
	for (int j=0;j<4;j++)
	{
		if (m_pbChannelOpen[j])
		{
			for (int k=j+1;k<4;k++)
			{
				if (m_pbChannelOpen[k])
				{
					for (int i = 0;i<4;i++)
					{
						pChannelPanel->m_ChannelInfo[k].m_GateInfo[i].m_bOpen=pChannelPanel->m_ChannelInfo[j].m_GateInfo[i].m_bOpen;
					}
				}
			}
			j=4;
		}
	}//end

	for (int j=0; j<4; j++)
	{
		CWnd* pWnd = pChannelPanel->GetDlgItem(uChannelID[j]);
		BOOL bEnable = pWnd->IsWindowEnabled();

		if (bEnable)
		{
			pChannelPanel->m_nChannelNo = j;
			pChannelPanel->UpdateData(FALSE);
			// Domodal 情况下才调用
			if(this->GetSafeHwnd() != NULL)
				pChannelPanel->DoChangeChannel();
			break;
		}
	}

	if (m_wWorkMode == 6 || m_wWorkMode == 7 || m_wWorkMode == 8)	// 单晶关联,双晶反射关联，双晶对射关联
	{
		int nGroupIndex = pChannelPanel->m_nChannelNo / 2;
		if(nGroupIndex == 0)
		{
			pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH5)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH6)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH7)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH8)->EnableWindow(FALSE);
			m_pbChannelOpen[0] = TRUE;
			m_pbChannelOpen[1] = TRUE;
			m_pbChannelOpen[2] = FALSE;
			m_pbChannelOpen[3] = FALSE;
			m_pbChannelOpen[4] = FALSE;
			m_pbChannelOpen[5] = FALSE;
			m_pbChannelOpen[6] = FALSE;
			m_pbChannelOpen[7] = FALSE;
		}
		else if(nGroupIndex == 1)
		{
			pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH5)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH6)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH7)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH8)->EnableWindow(FALSE);
			m_pbChannelOpen[0] = FALSE;
			m_pbChannelOpen[1] = FALSE;
			m_pbChannelOpen[2] = TRUE;
			m_pbChannelOpen[3] = TRUE;
			m_pbChannelOpen[4] = FALSE;
			m_pbChannelOpen[5] = FALSE;
			m_pbChannelOpen[6] = FALSE;
			m_pbChannelOpen[7] = FALSE;


		}
		else if (nGroupIndex == 2)
		{
			pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH5)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH6)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH7)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH8)->EnableWindow(FALSE);
			m_pbChannelOpen[0] = FALSE;
			m_pbChannelOpen[1] = FALSE;
			m_pbChannelOpen[2] = FALSE;
			m_pbChannelOpen[3] = FALSE;
			m_pbChannelOpen[4] = TRUE;
			m_pbChannelOpen[5] = TRUE;
			m_pbChannelOpen[6] = FALSE;
			m_pbChannelOpen[7] = FALSE;

		}			
		else if (nGroupIndex == 3)
		{
			pChannelPanel->GetDlgItem(IDC_RADIO_CH1)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH2)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH3)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH4)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH5)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH6)->EnableWindow(FALSE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH7)->EnableWindow(TRUE);
			pChannelPanel->GetDlgItem(IDC_RADIO_CH8)->EnableWindow(TRUE);
			m_pbChannelOpen[0] = FALSE;
			m_pbChannelOpen[1] = FALSE;
			m_pbChannelOpen[2] = FALSE;
			m_pbChannelOpen[3] = FALSE;
			m_pbChannelOpen[4] = FALSE;
			m_pbChannelOpen[5] = FALSE;
			m_pbChannelOpen[6] = TRUE;
			m_pbChannelOpen[7] = TRUE;
		}
		else
		{
			;//
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::SetChannelState"),"Leave");
}
void CSysParam::OnCbnSelchangeComboAxis()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnCbnSelchangeComboAxis"),"Enter");
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_nAxisIndex = m_cmbAxis.GetCurSel();
	int nAxisIndex = m_nAxisIndex;
	if(nAxisIndex == 0)
	{
		GetDlgItem(IDC_RADIO_STEP_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STEP_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STEP_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_R_SPEED)->EnableWindow(FALSE);
		// machao add start on 2013-08-26
		GetDlgItem(IDC_RADIO_STEP_W)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_W)->EnableWindow(FALSE);
		// machao add end on 2013-08-26
		
		CButton *pb;
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_X);
		pb->SetCheck(TRUE);
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_Y);
		pb->SetCheck(FALSE);
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_Z);
		pb->SetCheck(FALSE);
		// machao add start on 2013-08-27
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_W);
		pb->SetCheck(FALSE);
		// machao add end on 2013-08-27
		OnBnClickedRadioStepY();
	}
	else if(nAxisIndex == 1)
	{
		GetDlgItem(IDC_RADIO_STEP_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STEP_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STEP_Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_R_SPEED)->EnableWindow(FALSE);
		// machao add start on 2013-08-26
		GetDlgItem(IDC_RADIO_STEP_W)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_W)->EnableWindow(FALSE);
		// machao add end on 2013-08-26


		CButton *pb;
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_X);
		pb->SetCheck(TRUE);
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_Y);
		pb->SetCheck(FALSE);
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_Z);
		pb->SetCheck(FALSE);
		// machao add start on 2013-08-27
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_W);
		pb->SetCheck(FALSE);
		// machao add end on 2013-08-27
		OnBnClickedRadioStepX();
	}
	else if(nAxisIndex == 2)
	{
		GetDlgItem(IDC_RADIO_STEP_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STEP_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STEP_Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_R_SPEED)->EnableWindow(FALSE);
		// machao add start on 2013-08-26
		GetDlgItem(IDC_RADIO_STEP_W)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_W)->EnableWindow(FALSE);
		// machao add end on 2013-08-26


		CButton *pb;
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_Y);
		pb->SetCheck(TRUE);
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_X);
		pb->SetCheck(FALSE);
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_Z);
		pb->SetCheck(FALSE);
		// machao add start on 2013-08-27
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_W);
		pb->SetCheck(FALSE);
		// machao add end on 2013-08-27
		OnBnClickedRadioStepY();
	}
	else if(nAxisIndex == 3 
		|| nAxisIndex == 4)	//W1管 // wangbingfu add on 2013-01-09
	{
		m_nFillType = 1; //纵向
		GetDlgItem(IDC_RADIO_STEP_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STEP_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STEP_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_Z)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_R_SPEED)->EnableWindow(TRUE);
		// machao add start on 2013-08-26
		GetDlgItem(IDC_RADIO_STEP_W)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_W)->EnableWindow(FALSE);
		// machao add end on 2013-08-26


		CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
		CCScanView* pCScanView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
		pCScanView->Invalidate(TRUE);
		pCScanView2->Invalidate(TRUE);
	}
	// machao add start on 2013-08-26
	else if (nAxisIndex == 5)
	{
		GetDlgItem(IDC_RADIO_STEP_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STEP_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_STEP_Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STEP_W)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_X)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_Y)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SCAN_Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_W)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_R_SPEED)->EnableWindow(FALSE);
		CButton *pb;
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_Z);
		pb->SetCheck(TRUE);
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_W);
		pb->SetCheck(FALSE);
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_X);
		pb->SetCheck(FALSE);
		pb = (CButton *)GetDlgItem(IDC_RADIO_STEP_Y);
		pb->SetCheck(FALSE);
		OnBnClickedRadioStepW();
	}
	// machao add end on 2013-08-26
	else
	{
		GetDlgItem(IDC_RADIO_STEP_Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STEP_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_STEP_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_Z)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_Y)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_R_SPEED)->EnableWindow(TRUE);
		// machao add start on 2013-08-27
		GetDlgItem(IDC_RADIO_STEP_W)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SCAN_W)->EnableWindow(TRUE);
		// machao add end on 2013-08-27
	}
	// DongQi delete start on 2013-06-27
// 	CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
// 	pMovePathView->m_GraphMove.SetAxisSelect(nAxisIndex);
// 
// 	CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
// 	CCScanView* pCScanView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
// 	pCScanView->m_nAxisSelect = nAxisIndex;
// 	pCScanView2->m_nAxisSelect = nAxisIndex;
	// DongQi delete end on 2013-06-27
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnCbnSelchangeComboAxis"),"Leave");
}

void CSysParam::OnBnClickedRadioStepX()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioStepX"),"Enter");
	// TODO: Add your control notification handler code here
	m_nStepDirect = 0;
	m_nFillType = 1;
	
	int nAxisIndex = m_cmbAxis.GetCurSel();

	if(nAxisIndex == 0)		//XY
	{
		m_nScanDirect = 1;
	}
	else if(nAxisIndex == 1)  //XZ
	{
		m_nScanDirect = 2;
	}	
	else
	{
		MessageBox(_T("坐标系选择错误！"), _T("提示"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioStepX"),"Leave");
		return;
	}

	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioStepX"),"Leave");
}

void CSysParam::OnBnClickedRadioStepY()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioStepY"),"Enter");
	// TODO: Add your control notification handler code here
	m_nStepDirect = 1;
	
	int nAxisIndex = m_cmbAxis.GetCurSel();

	if(nAxisIndex == 0)		//XY
	{
		m_nFillType = 0;
		m_nScanDirect = 0;

	}
	else if(nAxisIndex == 2)  //YZ
	{
		m_nFillType = 1;
		m_nScanDirect = 2;
	}
	else
	{
		MessageBox(_T("坐标系选择错误!"), _T("提示"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioStepY"),"Leave");
	}

	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioStepY"),"Leave");
}

void CSysParam::OnBnClickedRadioStepZ()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioStepZ"),"Enter");
	// TODO: Add your control notification handler code here
	m_nStepDirect = 2;
//	m_nFillType = 0;	// machao delete on 2013-09-2
	
	int nAxisIndex = m_cmbAxis.GetCurSel();

	if(nAxisIndex == 1)		//XZ
	{
		m_nFillType = 0;	// machao add on 2013-09-2
		m_nScanDirect = 0;
	}
	else if(nAxisIndex == 2)  //YZ
	{
		m_nFillType = 0;	// machao add on 2013-09-2
		m_nScanDirect = 1;
	}
	// machao add start on 2013-08-27
	else if (nAxisIndex == 5)	//W面
	{
		m_nFillType = 0;	// machao add on 2013-09-2
		m_nScanDirect = 3;
	}
	// machao add end on 2013-08-27
	else
	{
		MessageBox(_T("坐标系选择错误！"), _T("提示"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioStepZ"),"Leave");
		return;
	}

	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioStepZ"),"Leave");
}

void CSysParam::OnBnClickedRadioScanX()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioScanX"),"Enter");
	// TODO: Add your control notification handler code here
	m_nScanDirect = 0;
	
	m_nFillType = 0;

	int nAxisIndex = m_cmbAxis.GetCurSel();
	if(nAxisIndex == 0)		//XY
	{
		m_nStepDirect = 1;
	}
	else if(nAxisIndex == 1)  //XZ
	{
		m_nStepDirect = 2;
	}	
	else
	{
		MessageBox(_T("坐标系选择错误！"), _T("提示"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioScanX"),"Leave");
		return;
	}

	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioScanX"),"Leave");
}

void CSysParam::OnBnClickedRadioScanY()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioScanY"),"Enter");
	// TODO: Add your control notification handler code here
	m_nScanDirect = 1;

	int nAxisIndex = m_cmbAxis.GetCurSel();

	if(nAxisIndex == 0)		//XY
	{
		m_nFillType = 1;
		m_nStepDirect = 0;
	}
	else if(nAxisIndex == 2)  //YZ
	{
		m_nFillType = 0;
		m_nStepDirect = 2;
	}
	else
	{
		MessageBox(_T("坐标系选择错误！"), _T("提示"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioScanY"),"Leave");
		return;
	}

	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioScanY"),"Leave");
}

void CSysParam::OnBnClickedRadioSacnZ()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioSacnZ"),"Enter");
	// TODO: Add your control notification handler code here
	m_nScanDirect = 2;
//	m_nFillType = 1;	// machao delete on 2013-09-2

	int nAxisIndex = m_cmbAxis.GetCurSel();
	if(nAxisIndex == 1)		//XZ
	{
		m_nFillType = 1;	// machao add on 2013-09-2
		m_nStepDirect = 0;
	}
	else if(nAxisIndex == 2)  //YZ
	{
		m_nFillType = 1;	// machao add on 2013-09-2
		m_nStepDirect = 1;
	}
	// machao add start on 2013-08-27
	else if (nAxisIndex == 5)	//W面
	{
		m_nFillType = 1;	// machao add on 2013-09-2
		m_nStepDirect = 3;
	}
	// machao add end on 2013-08-27
	else
	{
		MessageBox(_T("坐标系选择错误！"), _T("提示"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioSacnZ"),"Leave");
		return;
	}

	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioSacnZ"),"Leave");
}

void CSysParam::OnBnClickedProbeLocation()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedProbeLocation"),"Enter");
	CProbeLocationDlg dlg;
	dlg.m_fX1 = m_fProbeLocation[0][0];
	dlg.m_fX2 = m_fProbeLocation[1][0];
	dlg.m_fX3 = m_fProbeLocation[2][0];
	dlg.m_fX4 = m_fProbeLocation[3][0];
	dlg.m_fX5 = m_fProbeLocation[4][0];
	dlg.m_fX6 = m_fProbeLocation[5][0];
	dlg.m_fX7 = m_fProbeLocation[6][0];
	dlg.m_fX8 = m_fProbeLocation[7][0];

	dlg.m_fY1 = m_fProbeLocation[0][1];
	dlg.m_fY2 = m_fProbeLocation[1][1];
	dlg.m_fY3 = m_fProbeLocation[2][1];
	dlg.m_fY4 = m_fProbeLocation[3][1];
	dlg.m_fY5 = m_fProbeLocation[4][1];
	dlg.m_fY6 = m_fProbeLocation[5][1];
	dlg.m_fY7 = m_fProbeLocation[6][1];
	dlg.m_fY8 = m_fProbeLocation[7][1];

	dlg.m_fZ1 = m_fProbeLocation[0][2];
	dlg.m_fZ2 = m_fProbeLocation[1][2];
	dlg.m_fZ3 = m_fProbeLocation[2][2];
	dlg.m_fZ4 = m_fProbeLocation[3][2];
	dlg.m_fZ5 = m_fProbeLocation[4][2];
	dlg.m_fZ6 = m_fProbeLocation[5][2];
	dlg.m_fZ7 = m_fProbeLocation[6][2];
	dlg.m_fZ8 = m_fProbeLocation[7][2];

	if (dlg.DoModal() == IDOK)
	{
		m_fProbeLocation[0][0] = dlg.m_fX1;
		m_fProbeLocation[1][0] = dlg.m_fX2;
		m_fProbeLocation[2][0] = dlg.m_fX3;
		m_fProbeLocation[3][0] = dlg.m_fX4;
		m_fProbeLocation[4][0] = dlg.m_fX5;
		m_fProbeLocation[5][0] = dlg.m_fX6;
		m_fProbeLocation[6][0] = dlg.m_fX7;
		m_fProbeLocation[7][0] = dlg.m_fX8;

		m_fProbeLocation[0][1] = dlg.m_fY1;
		m_fProbeLocation[1][1] = dlg.m_fY2;
		m_fProbeLocation[2][1] = dlg.m_fY3;
		m_fProbeLocation[3][1] = dlg.m_fY4;
		m_fProbeLocation[4][1] = dlg.m_fY5;
		m_fProbeLocation[5][1] = dlg.m_fY6;
		m_fProbeLocation[6][1] = dlg.m_fY7;
		m_fProbeLocation[7][1] = dlg.m_fY8;

		m_fProbeLocation[0][2] = dlg.m_fZ1;
		m_fProbeLocation[1][2] = dlg.m_fZ2;
		m_fProbeLocation[2][2] = dlg.m_fZ3;
		m_fProbeLocation[3][2] = dlg.m_fZ4;
		m_fProbeLocation[4][2] = dlg.m_fZ5;
		m_fProbeLocation[5][2] = dlg.m_fZ6;
		m_fProbeLocation[6][2] = dlg.m_fZ7;
		m_fProbeLocation[7][2] = dlg.m_fZ8;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnBnClickedProbeLocation"),"Leave");
}

void CSysParam::SetChannelStateNew(const int& nGroupNum, 
						const int& nOneGroupIndex,
						const int& nTwoGroupIndex,
						const int& nThreeGroupIndex, const WORD& wWorkMode)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::SetChannelStateNew"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;

	UINT uChannelID[8] = {IDC_RADIO_CH1, IDC_RADIO_CH2, IDC_RADIO_CH3, IDC_RADIO_CH4,
						IDC_RADIO_CH5, IDC_RADIO_CH6, IDC_RADIO_CH7, IDC_RADIO_CH8};
	
	if (nGroupNum == 1)	// 组1
	{				
		for (int i = 0; i<4; i++)
		{
			if (i == nOneGroupIndex)
			{
				SetGroupState(i, wWorkMode, TRUE);
			}
			else
			{
				SetGroupState(i, wWorkMode, FALSE);
			}
		}
	}
	else if (nGroupNum == 2) // 组2
	{
		if (nTwoGroupIndex == 0)
		{
			SetGroupState(0, wWorkMode, TRUE);
			SetGroupState(1, wWorkMode, TRUE);
			SetGroupState(2, wWorkMode, FALSE);
			SetGroupState(3, wWorkMode, FALSE);
		} 
		else if (nTwoGroupIndex == 1)
		{
			SetGroupState(0, wWorkMode, FALSE);
			SetGroupState(1, wWorkMode, TRUE);
			SetGroupState(2, wWorkMode, TRUE);
			SetGroupState(3, wWorkMode, FALSE);
		}
		else if (nTwoGroupIndex == 2)
		{
			SetGroupState(0, wWorkMode, FALSE);
			SetGroupState(1, wWorkMode, FALSE);
			SetGroupState(2, wWorkMode, TRUE);
			SetGroupState(3, wWorkMode, TRUE);
		}
		else if (nTwoGroupIndex == 3)
		{
			SetGroupState(0, wWorkMode, TRUE);
			SetGroupState(1, wWorkMode, FALSE);
			SetGroupState(2, wWorkMode, TRUE);
			SetGroupState(3, wWorkMode, FALSE);
		}
		else if (nTwoGroupIndex == 4)
		{
			SetGroupState(0, wWorkMode, FALSE);
			SetGroupState(1, wWorkMode, TRUE);
			SetGroupState(2, wWorkMode, FALSE);
			SetGroupState(3, wWorkMode, TRUE);
		}
		else if (nTwoGroupIndex == 5)
		{
			SetGroupState(0, wWorkMode, TRUE);
			SetGroupState(1, wWorkMode, FALSE);
			SetGroupState(2, wWorkMode, FALSE);
			SetGroupState(3, wWorkMode, TRUE);
		}
		else
		{
		}
	}
	else if (nGroupNum == 3) // 组3
	{
		if (m_nScanType == 0)
		{
			if (nThreeGroupIndex == 0)
			{
				SetGroupState(0, wWorkMode, TRUE);
				SetGroupState(1, wWorkMode, TRUE);
				SetGroupState(2, wWorkMode, TRUE);
				SetGroupState(3, wWorkMode, FALSE);
			} 
			else if (nThreeGroupIndex == 1)
			{
				SetGroupState(0, wWorkMode, FALSE);
				SetGroupState(1, wWorkMode, TRUE);
				SetGroupState(2, wWorkMode, TRUE);
				SetGroupState(3, wWorkMode, TRUE);
			}		
			else
			{
			}
		} 
		else	// {_T("1, 2, 3"), _T("1, 2, 4"), _T("1, 3, 4"), _T("2, 3, 4")};
		{
			if (nThreeGroupIndex == 0)
			{
				SetGroupState(0, wWorkMode, TRUE);
				SetGroupState(1, wWorkMode, TRUE);
				SetGroupState(2, wWorkMode, TRUE);
				SetGroupState(3, wWorkMode, FALSE);
			} 
			else if (nThreeGroupIndex == 1)
			{
				SetGroupState(0, wWorkMode, TRUE);
				SetGroupState(1, wWorkMode, TRUE);
				SetGroupState(2, wWorkMode, FALSE);
				SetGroupState(3, wWorkMode, TRUE);
			}
			else if (nThreeGroupIndex == 2)
			{
				SetGroupState(0, wWorkMode, TRUE);
				SetGroupState(1, wWorkMode, FALSE);
				SetGroupState(2, wWorkMode, TRUE);
				SetGroupState(3, wWorkMode, TRUE);
			}
			else if (nThreeGroupIndex == 3)
			{
				SetGroupState(0, wWorkMode, FALSE);
				SetGroupState(1, wWorkMode, TRUE);
				SetGroupState(2, wWorkMode, TRUE);
				SetGroupState(3, wWorkMode, TRUE);
			}		
			else
			{
			}
		}
	}
	else if (nGroupNum == 4) // 组4
	{
		SetGroupState(0, wWorkMode, TRUE);
		SetGroupState(1, wWorkMode, TRUE);
		SetGroupState(2, wWorkMode, TRUE);
		SetGroupState(3, wWorkMode, TRUE);
	}
	else
	{}

	// 同步闸门开关 2012-4-1
	for (int j=0;j<CHANNEL_NUM;j++)	// 通道
	{
		if (m_pbChannelOpen[j])
		{
			for (int k=j+1;k<CHANNEL_NUM;k++)	// 后续通道
			{
				if (m_pbChannelOpen[k])
				{
					for (int i = 0;i<GATE_NUM;i++)
					{
						pChannelPanel->m_ChannelInfo[k].m_GateInfo[i].m_bOpen = pChannelPanel->m_ChannelInfo[j].m_GateInfo[i].m_bOpen;
					}
				}
			}

			break;
		}
	}//end

	// 切换到当前打开的第一个通道
	for (int j=0; j<CHANNEL_NUM; j++)
	{
		CWnd* pWnd = pChannelPanel->GetDlgItem(uChannelID[j]);
		BOOL bEnable = pWnd->IsWindowEnabled();

		if (bEnable)
		{
			pChannelPanel->m_nChannelNo = j;
			pChannelPanel->UpdateData(FALSE);
			// Domodal 情况下才调用
			if(this->GetSafeHwnd() != NULL)
				pChannelPanel->DoChangeChannel();
			break;
		}
	}
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::SetChannelStateNew"),"Leave");
}

void CSysParam::SetGroupState(const int& nGroupIndex, const WORD& wWorkMode, const BOOL& bOPen)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::SetGroupState"),"Enter");
	UINT uChannelID[8] = {IDC_RADIO_CH1, IDC_RADIO_CH2, IDC_RADIO_CH3, IDC_RADIO_CH4,
		IDC_RADIO_CH5, IDC_RADIO_CH6, IDC_RADIO_CH7, IDC_RADIO_CH8};
	int nFirstChannelIndex = nGroupIndex * 2;
	int nSecondChannelIndex = nGroupIndex * 2 + 1;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pPanel = pFrame->m_wndChannelInfo.m_pFormView;
	// Song Chenguang modify start on 2013-07-19
//	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
//	int nCardNum = pApp->GetCardNum();
	
	if (bOPen)
	{
		if (wWorkMode == 1)	// 单1
		{
			pPanel->GetDlgItem(uChannelID[nFirstChannelIndex])->EnableWindow(TRUE);
			pPanel->GetDlgItem(uChannelID[nSecondChannelIndex])->EnableWindow(FALSE);

			m_pbChannelOpen[nFirstChannelIndex] = TRUE;
			m_pbChannelOpen[nSecondChannelIndex] = FALSE;
		}
		else if (wWorkMode == 2 || wWorkMode == 4 || wWorkMode == 5)	// 单2、双晶反射、双晶对射
		{
			pPanel->GetDlgItem(uChannelID[nFirstChannelIndex])->EnableWindow(FALSE);
			pPanel->GetDlgItem(uChannelID[nSecondChannelIndex])->EnableWindow(TRUE);

			m_pbChannelOpen[nFirstChannelIndex] = FALSE;
			m_pbChannelOpen[nSecondChannelIndex] = TRUE;
		}
		else if (wWorkMode == 3 || wWorkMode == 6 || wWorkMode == 7 || wWorkMode == 8)	// 双单、单晶关联、双晶反射关联、双晶对射关联
		{
			pPanel->GetDlgItem(uChannelID[nFirstChannelIndex])->EnableWindow(TRUE);
			pPanel->GetDlgItem(uChannelID[nSecondChannelIndex])->EnableWindow(TRUE);

			m_pbChannelOpen[nFirstChannelIndex] = TRUE;
			m_pbChannelOpen[nSecondChannelIndex] = TRUE;
		}
		else
		{

		}

//		if(nGroupIndex < nCardNum)
//			pPanel->SendWorkMode(nGroupIndex, wWorkMode);	// Song Chenguang add on 2013-07-19
	}
	else
	{
		pPanel->GetDlgItem(uChannelID[nFirstChannelIndex])->EnableWindow(FALSE);
		pPanel->GetDlgItem(uChannelID[nSecondChannelIndex])->EnableWindow(FALSE);

		m_pbChannelOpen[nFirstChannelIndex] = FALSE;
		m_pbChannelOpen[nSecondChannelIndex] = FALSE;

//		if(nGroupIndex < nCardNum)
//			pPanel->SendWorkMode(nGroupIndex, 0);	// Song Chenguang add on 2013-07-19
	}

	// Song Chenguang modify end on 2013-07-19
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::SetGroupState"),"Leave");
}

// 改变工作模式响应
void CSysParam::OnCbnSelchangeComboWorkMode()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnCbnSelchangeComboWorkMode"),"Enter");
	// TODO: Add your control notification handler code here
	CComboBox* pcmbWorkMode = (CComboBox*)GetDlgItem(IDC_COMBO_WORK_MODE);
	int nIndex = pcmbWorkMode->GetCurSel();
	m_wWorkMode = nIndex + 1;	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::OnCbnSelchangeComboWorkMode"),"Leave");
}

// 根据工作模式设定机械运动规则
// 需根据组合情况设定扫查控件探头间距...
/*
void CSysParam::SetMoveRule(const int& nGroupNum, 
							const int& nOneGroupIndex,
							const int& nTwoGroupIndex,
							const int& nThreeGroupIndex,
							const WORD& wWorkMode)
{
	if (wWorkMode < 1 || wWorkMode > 8)	
	{
		MessageBox(_T("非法的工作模式值"), _T("提示"), MB_OK);
		return;
	}

	CUltrasonicImmersedTestApp* pApp =(CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();

	double dSysPara[40];//返回系统参数
	long   llogPara[10];//填充方式

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
	pMovePathView->m_GraphMove.GetPara(dSysPara, llogPara);//获取系统参数

	//////////////////////////////////////////////////////////////////////////
	// 设定探头个数
	CString strNum;
	int nProbeNum = 0;
	
	if (m_nScanType == 0)	// 步进
	{
		if(wWorkMode > 3)
			nProbeNum = nGroupNum;
		else
			nProbeNum = nGroupNum * 2;
	} 
	else if(m_nScanType == 1)	// 深度
		nProbeNum = 1;
	else
	{
	}

	llogPara[3] = nProbeNum;

	strNum.Format(_T("%d"), nProbeNum);
	pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeNum"), strNum);

	// 设定排列索引号
	if (nGroupNum == 1)
	{
		CString str;
		str.Format(_T("%d"), nOneGroupIndex);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("OneProbeType"), str);		

		llogPara[5] = nOneGroupIndex;		
	}
	else if (nGroupNum == 2)
	{
		CString str;
		str.Format(_T("%d"), nTwoGroupIndex);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("TwoProbeType"), str);		

		llogPara[5] = nTwoGroupIndex;
	}
	else if (nGroupNum == 3)
	{
		CString str;
		str.Format(_T("%d"), nThreeGroupIndex);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ThreeProbeType"), str);		

		llogPara[5] = nThreeGroupIndex;
	}
	else
	{
		llogPara[5] = 0;
	}
	//////////////////////////////////////////////////////////////////////////
	

	//根据控件变量设置系统参数
	pMovePathView->m_GraphMove.SetPara(dSysPara,llogPara);
}
*/
// wangbingfu add start at 2012-9-18
void CSysParam::SetMoveRule(const int& nGroupNum, 
							const int& nOneGroupIndex,
							const int& nTwoGroupIndex,
							const int& nThreeGroupIndex,
							const WORD& wWorkMode)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::SetMoveRule"),"Enter");
	if (wWorkMode < 1 || wWorkMode > 8)	
	{
		MessageBox(_T("非法的工作模式值"), _T("提示"), MB_OK);
		return;
	}

	CUltrasonicImmersedTestApp* pApp =(CUltrasonicImmersedTestApp*)AfxGetApp();
	int nCardNum = pApp->GetCardNum();

	double dSysPara[SYSPARAM1_NUM];//返回系统参数
	long   llogPara[10];//填充方式

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
	pMovePathView->m_GraphMove.GetPara(dSysPara, llogPara);//获取系统参数

	//////////////////////////////////////////////////////////////////////////
	// 转换成探头个数模式
	int nProbeNum = 0;
	int nOneProbeIndex = 0;
	int nTwoProbeIndex = 0;
	int nThreeProbeIndex = 0;
	Group2ProbeValue(nProbeNum, nOneProbeIndex, nTwoProbeIndex, nThreeProbeIndex);
	if(m_nScanType == 1)	// 深度
	{
		nProbeNum = 1;
	}

	llogPara[3] = nProbeNum;
	
	CString strNum;
	strNum.Format(_T("%d"), nGroupNum);
	pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ProbeNum"), strNum);
	if (nGroupNum == 1)
	{
		CString str;
		str.Format(_T("%d"), nOneGroupIndex);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("OneProbeType"), str);				
	}
	else if (nGroupNum == 2)
	{
		CString str;
		str.Format(_T("%d"), nTwoGroupIndex);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("TwoProbeType"), str);				
	}
	else if (nGroupNum == 3)
	{
		CString str;
		str.Format(_T("%d"), nThreeGroupIndex);
		pApp->WriteProfileString(_T("水浸超声探伤系统"), _T("ThreeProbeType"), str);		
	}
	else
	{
		;//
	}
	// 只有两块卡
	if(nProbeNum == 1)
	{
		llogPara[5] = nOneProbeIndex;
	}
	else if(nProbeNum == 2)
	{
		llogPara[5] = nTwoProbeIndex;
	}
	else if(nProbeNum == 4)
	{
		llogPara[5] = 0;
	}
	else
	{
		;//
	}
	//////////////////////////////////////////////////////////////////////////


	//根据控件变量设置系统参数
	pMovePathView->m_GraphMove.SetPara(dSysPara,llogPara);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::SetMoveRule"),"Leave");
}

void CSysParam::Group2ProbeValue(int &nProbeNum, int &nOneProbeIndex, int &nTwoProbeIndex, int &nThreeProbeIndex)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::Group2ProbeValue"),"Enter");
	// 只有两块卡
	int nGroupNum = m_nIndexProbeNum + 1;
	if(nGroupNum == 1)
	{
		if(m_wWorkMode == 1)	// 单1
		{
			nProbeNum = 1;
			if(m_nIndexOneProbeType == 0)
			{
				nOneProbeIndex = 0;	// 1
			}
			else if(m_nIndexOneProbeType == 1)
			{
				nOneProbeIndex = 2;	// 3
			}

		}
		else if(m_wWorkMode == 2)	// 单2
		{
			nProbeNum = 1;
			
			if(m_nIndexOneProbeType == 0)
			{
				nOneProbeIndex = 1;	// 2 
			}
			else if(m_nIndexOneProbeType == 1)
			{
				nOneProbeIndex = 3;	// 4
			}

		}
		else if(m_wWorkMode == 3)	// 双单
		{
			nProbeNum = 2;
			// 也可以不分，因为不管哪个探头起始
			if(m_nIndexOneProbeType == 0)
			{
				nTwoProbeIndex = 0; // 1,2
			}
			else if(m_nIndexOneProbeType == 1)
			{
				nTwoProbeIndex = 2; // 3,4
			}
			else
			{
				
			}

		}
		else	// 关联
		{
			nProbeNum = 1;
			if(m_nIndexOneProbeType == 0)
			{
				nOneProbeIndex = 1;	// 2
			}
			else if(m_nIndexOneProbeType == 1)
			{
				nOneProbeIndex = 3;	// 4
			}
			else
			{
				
			}
			
		}
	}
	else if(nGroupNum == 2)
	{
		if(m_wWorkMode == 1)	// 单1
		{
			nProbeNum = 2;
			nTwoProbeIndex = 3; // 1, 3
		}
		else if(m_wWorkMode == 2)	// 单2
		{
			nProbeNum = 2;
			nTwoProbeIndex = 4; // 2, 4
		}
		else if(m_wWorkMode == 3)	// 双单
		{
			nProbeNum = 4;	// 1,2,3,4
		}
		else	// 关联
		{
			nProbeNum = 2;
			nTwoProbeIndex = 4; //2, 4 也可以=3即1,3 因为不管哪个探头起始
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSysParam::Group2ProbeValue"),"Leave");
}
// wangbingfu add end at 2012-9-18
// wangbingfu add start at 2012-12-26
void CSysParam::OnCbnSelchangeComboStepMode()
{
	// TODO: Add your control notification handler code here
	m_nStepMode = m_cmbStepMode.GetCurSel();
}
// wangbingfu add end at 2012-12-26
void CSysParam::OnBnClickedButtonPipeSet()
{
	// TODO: Add your control notification handler code here
	if (m_pipeParamDlg.DoModal() == IDOK)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->m_nPipeChannelSelect[0]= m_pipeParamDlg.m_nG11Index;
		pFrame->m_nPipeChannelSelect[1]= m_pipeParamDlg.m_nG12Index;
		pFrame->m_nPipeChannelSelect[2]= m_pipeParamDlg.m_nG21Index;
		pFrame->m_nPipeChannelSelect[3]= m_pipeParamDlg.m_nG22Index;
		pFrame->m_nPipeChannelSelect[4]= m_pipeParamDlg.m_nG31Index;
		pFrame->m_nPipeChannelSelect[5]= m_pipeParamDlg.m_nG32Index;
		pFrame->m_nPipeChannelSelect[6]= m_pipeParamDlg.m_nG41Index;
		pFrame->m_nPipeChannelSelect[7]= m_pipeParamDlg.m_nG42Index;
		pFrame->m_nCScanSelect[0] = m_pipeParamDlg.m_nCscan1Index;
		pFrame->m_nCScanSelect[1] = m_pipeParamDlg.m_nCscan2Index;
		
		CCScanView* pCScanView1 = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
		CCScanView* pCScanView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
		for(int i = 0; i < CHANNEL_NUM; i++)
		{
			pCScanView1->m_nPipeChannelSelect[i] = pFrame->m_nPipeChannelSelect[i];
			pCScanView2->m_nPipeChannelSelect[i] = pFrame->m_nPipeChannelSelect[i];
		}
		pCScanView1->m_nCScanSelect = pFrame->m_nCScanSelect[0];
		pCScanView2->m_nCScanSelect = pFrame->m_nCScanSelect[1];
		pFrame->WritePipeIni();
	}
}

// machao add start on 2013-08-27
void CSysParam::OnBnClickedRadioStepW()
{
	// TODO: Add your control notification handler code here
	
	m_nStepDirect = 3;
	m_nFillType = 1;	// machao modify on 2013-09-2
	int nAxisIndex = m_cmbAxis.GetCurSel();

	if(nAxisIndex == 5)		//W面
	{
		m_nScanDirect = 2;
	}	
	else
	{
		MessageBox(_T("坐标系选择错误！"), _T("提示"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioStepX"),"Leave");
		return;
	}

	UpdateData(FALSE);
}

void CSysParam::OnBnClickedRadioScanW()
{
	// TODO: Add your control notification handler code here

	m_nScanDirect = 3;
	m_nFillType = 0;	// machao modify on 2013-09-2
	int nAxisIndex = m_cmbAxis.GetCurSel();
	if(nAxisIndex == 5)		//W面
	{
		m_nStepDirect = 2;
	}
	else
	{
		MessageBox(_T("坐标系选择错误！"), _T("提示"), MB_OK);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSysParam::OnBnClickedRadioScanX"),"Leave");
		return;
	}

	UpdateData(FALSE);
}
// machao add end on 2013-08-27