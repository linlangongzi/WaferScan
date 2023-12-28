// DacCurveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "DacCurveDlg.h"
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
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "FeatureInfoWnd.h"
#include <math.h>
#include "UltrasonicImmersedTestView.h"
#include "UltrasonicImmersedTestDoc.h"
#include "GlobalFunction.h"
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
// CDacCurveDlg dialog
extern DWORD g_dwActiveChannel;
extern short g_nWaveSpeed;
IMPLEMENT_DYNAMIC(CDacCurveDlg, CDialog)

CDacCurveDlg::CDacCurveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDacCurveDlg::IDD, pParent)
	, m_fAutoAmp(80)
	, m_fPingDing(3)
	, m_fDingLiang(0)
	, m_fPanFei(-3)
	, m_fCurrentPointRange(0)
	, m_fCurrentPointAmp(0)
	, m_nCurPointNum(0)
	, m_nDacStatus(1)
	, m_fDefectSize(0)
	, m_fDefectRange(0)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::CDacCurveDlg"),"Enter");
	// WangQianfeng add start on 2012-11-02
	m_pFrame = NULL;
	m_pAScanView = NULL;
	m_pDoc = NULL;
	m_pChannelPanel = NULL;
	m_pFeatureWnd = NULL;
	// WangQianfeng add end on 2012-11-02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::CDacCurveDlg"),"Leave");
}

CDacCurveDlg::~CDacCurveDlg()
{
}

void CDacCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::DoDataExchange"),"Enter");
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_AUTOGAIN_AMP, m_fAutoAmp);
	DDX_Text(pDX, IDC_EDIT_PINGDING, m_fPingDing);
	DDX_Text(pDX, IDC_EDIT_DINGLIANG, m_fDingLiang);
	DDX_Text(pDX, IDC_EDIT_PANHUI, m_fPanFei);
	DDX_Text(pDX, IDC_EDIT_DAC_RANGE, m_fCurrentPointRange);
	DDX_Text(pDX, IDC_EDIT_DAC_AMP, m_fCurrentPointAmp);
	DDX_Text(pDX, IDC_EDIT_CURRENT_POINT_NUM, m_nCurPointNum);
	DDX_Radio(pDX, IDC_RADIO_DAC_ON, m_nDacStatus);
	DDX_Text(pDX, IDC_EDIT_DEFECT_SIZE, m_fDefectSize);
	DDX_Text(pDX, IDC_EDIT__DEFECT_RANGE, m_fDefectRange);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::DoDataExchange"),"Leave");
}


BEGIN_MESSAGE_MAP(CDacCurveDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DAC_AUTO_GAIN, &CDacCurveDlg::OnBnClickedButtonDacAutoGain)
	ON_WM_TIMER()
//	ON_WM_CLOSE()
ON_WM_CLOSE()
ON_BN_CLICKED(IDC_BUTTON_DAC_ADD_POINT, &CDacCurveDlg::OnBnClickedButtonDacAddPoint)
ON_BN_CLICKED(IDC_BUTTON_PREV_DAC_POINT, &CDacCurveDlg::OnBnClickedButtonPrevDacPoint)
ON_BN_CLICKED(IDC_BUTTON_NEXT_DAC_POINT, &CDacCurveDlg::OnBnClickedButtonNextDacPoint)
ON_BN_CLICKED(IDC_BUTTON_DAC_DELETE_POINT, &CDacCurveDlg::OnBnClickedButtonDacDeletePoint)
ON_BN_CLICKED(IDC_BUTTON_DAC_CREATE, &CDacCurveDlg::OnBnClickedButtonDacCreate)
ON_BN_CLICKED(IDC_RADIO_DAC_ON, &CDacCurveDlg::OnBnClickedRadioDacOn)
ON_BN_CLICKED(IDC_RADIO_DAC_OFF, &CDacCurveDlg::OnBnClickedRadioDacOff)
ON_BN_CLICKED(IDC_DAC_FILE_OPEN, &CDacCurveDlg::OnBnClickedDacFileOpen)
ON_BN_CLICKED(IDC_BUTTON_FILE_SAVE, &CDacCurveDlg::OnBnClickedButtonFileSave)
ON_BN_CLICKED(IDC_BUTTON_DAC_MODIFY_CONFIRM, &CDacCurveDlg::OnBnClickedButtonDacModifyConfirm)
END_MESSAGE_MAP()


// CDacCurveDlg message handlers

void CDacCurveDlg::OnBnClickedButtonDacAutoGain()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacAutoGain"),"Enter");
	// TODO: Add your control notification handler code here
	SetTimer(TIMER_AUTO_GAIN, 50, NULL);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacAutoGain"),"Leave");
}

void CDacCurveDlg::OnTimer(UINT_PTR nIDEvent)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnTimer"),"Enter");
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TIMER_AUTO_GAIN)
	{
		AutoGain();
	}

// 	if(nIDEvent == TIMER_DEFECT_INFO)
// 	{
// 		DisplayDefectInfo();
// 	}

	CDialog::OnTimer(nIDEvent);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnTimer"),"Leave");
}

void CDacCurveDlg::OnClose()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnClose"),"Enter");
	// TODO: Add your message handler code here and/or call default
	UpdateData(TRUE);
	KillTimer(TIMER_AUTO_GAIN);
	m_pAScanView->m_bMakeDacFlag = FALSE;
	ShowWindow(SW_HIDE);
	m_pFrame->m_bDacWndDisplayed = !m_pFrame->m_bDacWndDisplayed;

	int nPointNum = m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum;
	for(int i = 0; i < nPointNum; i++)
	{
		float fData = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i].fData;
		float fRange = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i].fRange;
		// 评定线的点
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][i].fData = CurveDataChangeByGain(fData, m_fPingDing);
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][i].fRange = fRange;
		// 定量线的点
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][i].fData = CurveDataChangeByGain(fData, m_fDingLiang);
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][i].fRange = fRange;
		// 判废线的点
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[2][i].fData = CurveDataChangeByGain(fData, m_fPanFei);
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[2][i].fRange = fRange;

		m_pAScanView->m_stDacCurve[g_dwActiveChannel].fPingDing = m_fPingDing;
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].fDingLiang = m_fDingLiang;
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].fPanFei = m_fPanFei;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnClose"),"Leave");
//	CDialog::OnClose();
}

void CDacCurveDlg::AutoGain()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::AutoGain"),"Enter");
	UpdateData(TRUE);
	if(!m_pFeatureWnd->m_bGateOpen[1])	// 闸门1没开
	{
		KillTimer(TIMER_AUTO_GAIN);
		AfxMessageBox(_T("增益失败，请先开打开闸门1"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::AutoGain"),"Leave");
		return;
	}

	if(!m_pFeatureWnd->m_bMPMAltitudeDisplay)	// 最大波峰幅值没有打开
	{
		KillTimer(TIMER_AUTO_GAIN);
		AfxMessageBox(_T("增益失败，请先打开最大波峰幅值"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::AutoGain"),"Leave");
		return;
	}

	int nGateNum = m_pFeatureWnd->m_nGateOpenNum;
	int nFeatureNum =m_pFeatureWnd->m_nFeatureOpenNum + 1;
	CFeatureInfo gateFeature = CFeatureInfo(nGateNum, nFeatureNum);
//	gateFeature.m_pfFeatures = m_pFeatureWnd->m_GateFeature.m_pfFeatures;

	BOOL bFeatureFlag[7];
	bFeatureFlag[0] = TRUE;
	bFeatureFlag[1] = m_pFeatureWnd->m_bFPFTimeDisplay;
	bFeatureFlag[2] = m_pFeatureWnd->m_bFPMTimeDisplay;
	bFeatureFlag[3] = m_pFeatureWnd->m_bMPFTimeDisplay;
	bFeatureFlag[4] = m_pFeatureWnd->m_bMPMTimeDisplay;
	bFeatureFlag[5] = m_pFeatureWnd->m_bFPMAltitudeDisplay;
	bFeatureFlag[6] = m_pFeatureWnd->m_bMPMAltitudeDisplay;

	int nFeatureIndex = 0;
	for(int i = 0; i < 6; i++)
	{
		if(bFeatureFlag[i])
		{
			nFeatureIndex++;
		}

	}
	gateFeature.m_pfFeatures[1][nFeatureIndex] = m_pFeatureWnd->m_GateFeature.m_pfFeatures[1][nFeatureIndex];
	float fPeakValue = gateFeature.m_pfFeatures[1][nFeatureIndex];

	if(fabs(fPeakValue - m_fAutoAmp) > 1)	// 波峰值和设定值误差大于1E-1
	{
		float fStep = 0.3f;
		double fCha = fabs(fPeakValue - m_fAutoAmp);
		if(fCha > 5.0f)
			fStep = 0.3f;
		else
			fStep = 0.05f;

		if(fPeakValue > m_fAutoAmp)	// 若大于设定值，减增益
		{
			if((m_pChannelPanel->m_fPlus - fStep) > MIN_PLUS)
			{
				m_pChannelPanel->m_fPlus -= fStep;
			}
			else
			{
				m_pChannelPanel->m_fPlus = MIN_PLUS;
				KillTimer(TIMER_AUTO_GAIN);
				AfxMessageBox(_T("增益失败，已经到达最小增益"));
			}
		}
		else if(fPeakValue < m_fAutoAmp)// 否则增增益
		{
			if((m_pChannelPanel->m_fPlus + fStep) < MAX_PLUS)
			{
				m_pChannelPanel->m_fPlus += fStep;
			}
			else
			{
				m_pChannelPanel->m_fPlus = MAX_PLUS;
				KillTimer(TIMER_AUTO_GAIN);
				AfxMessageBox(_T("增益失败，已经到达最大增益"));
			}
		}
/*
		WORD wRef = 0;
		WORD wLevel1 = 0;
		WORD wLevel2 = 0;
		WORD wLevel3 = 0;
		dBProcess(m_pChannelPanel->m_fPlus, wRef, wLevel1, wLevel2, wLevel3);
		m_pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].stConfig.wdBRef = wRef;
		m_pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].stConfig.wdBLevel1 = wLevel1;
		m_pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].stConfig.wdBLevel2 = wLevel2;
		m_pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].stConfig.wdBLevel3 = wLevel3;
		*/
		m_pDoc->m_stAllCH_CONFIG[g_dwActiveChannel].fPlus = m_pChannelPanel->m_fPlus;
		m_pFrame->m_bSetOne = TRUE;	
		m_pChannelPanel->UpdateData(FALSE);
	}
	else
	{
		KillTimer(TIMER_AUTO_GAIN);
		m_pChannelPanel->UpdateData(FALSE);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::AutoGain"),"Leave");
}

BOOL CDacCurveDlg::OnInitDialog()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnInitDialog"),"Enter");
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pAScanView = (CUltrasonicImmersedTestView*)m_pFrame->m_wndSplitter.GetPane(1,0);
	m_pChannelPanel = m_pFrame->m_wndChannelInfo.m_pFormView;
	m_pFeatureWnd = m_pFrame->m_pFeatureWnd;
	m_pDoc = m_pAScanView->GetDocument();
	// WangQianfeng add start on 2013-06-26
	//没有加点则不充许修正点声程及幅值
	GetDlgItem(IDC_EDIT_DAC_RANGE)->EnableWindow(FALSE);//声程
	GetDlgItem(IDC_EDIT_DAC_AMP)->EnableWindow(FALSE);//幅值
	GetDlgItem(IDC_BUTTON_DAC_MODIFY_CONFIRM)->EnableWindow(FALSE);//确认修改
	GetDlgItem(IDC_BUTTON_PREV_DAC_POINT)->EnableWindow(FALSE);//上一点
	GetDlgItem(IDC_BUTTON_NEXT_DAC_POINT)->EnableWindow(FALSE);//下一点
	GetDlgItem(IDC_BUTTON_DAC_DELETE_POINT)->EnableWindow(FALSE);//删除点

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDacCurveDlg::OnBnClickedButtonDacAddPoint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacAddPoint"),"Enter");
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(!m_pFeatureWnd->m_bGateOpen[0])	// 闸门1没开
	{
		KillTimer(TIMER_AUTO_GAIN);
		AfxMessageBox(_T("加点失败，请先开打开闸门1"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacAddPoint"),"Leave");
		return;
	}

	if(!m_pFeatureWnd->m_bMPMAltitudeDisplay)	// 最大波峰幅值没有打开
	{
		KillTimer(TIMER_AUTO_GAIN);
		AfxMessageBox(_T("加点失败，请先打开最大波峰幅值"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacAddPoint"),"Leave");
		return;
	}
	if(!m_pFeatureWnd->m_bMPMTimeDisplay)	// 最大波峰幅值没有打开
	{
		KillTimer(TIMER_AUTO_GAIN);
		AfxMessageBox(_T("加点失败，请先打开最大波峰时间"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacAddPoint"),"Leave");
		return;
	}

	BOOL bFeatureFlag[7];
	bFeatureFlag[0] = TRUE;
	bFeatureFlag[1] = m_pFeatureWnd->m_bFPFTimeDisplay;
	bFeatureFlag[2] = m_pFeatureWnd->m_bFPMTimeDisplay;
	bFeatureFlag[3] = m_pFeatureWnd->m_bMPFTimeDisplay;
	bFeatureFlag[4] = m_pFeatureWnd->m_bMPMTimeDisplay;
	bFeatureFlag[5] = m_pFeatureWnd->m_bFPMAltitudeDisplay;
	bFeatureFlag[6] = m_pFeatureWnd->m_bMPMAltitudeDisplay;

	int nTimeIndex = 0;			//最大波峰时间索引
	for(int i = 0; i < 4; i++)
	{
		if(bFeatureFlag[i])
		{
			nTimeIndex++;
		}

	}
	int nAmpIndex = 0;			//最大波峰幅值索引
	for(int i = 0; i < 6; i++)
	{
		if(bFeatureFlag[i])
		{
			nAmpIndex++;
		}

	}

	if(m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum < DAC_CURVE_POINT_NUM)
	{
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum++;
		int nPointNum = m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum;
		
		float fTotalPlus = 0;
		if(nPointNum == 1)	// 第一点，记下此时增益
		{
			fTotalPlus = GetTotalPlus(m_pChannelPanel->m_fPlus, m_pChannelPanel->m_fRefPlus, g_dwActiveChannel);
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].fFirstGain = fTotalPlus;
		}
		
		fTotalPlus = GetTotalPlus(m_pChannelPanel->m_fPlus, m_pChannelPanel->m_fRefPlus, g_dwActiveChannel);
		// 制作线的点
		float fChangeGain = m_pAScanView->m_stDacCurve[g_dwActiveChannel].fFirstGain - fTotalPlus;
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][nPointNum - 1].fData =  CurveDataChangeByGain(m_pFeatureWnd->m_GateFeature.m_pfFeatures[1][nAmpIndex], fChangeGain);
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][nPointNum - 1].fRange = m_pFeatureWnd->m_GateFeature.m_pfFeatures[1][nTimeIndex];// + fSyncRange;
		// wangbingfu modify end at 2012-8-16

		float fData = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][nPointNum - 1].fData;
		float fRange = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][nPointNum - 1].fRange;
		// 评定线的点
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][nPointNum - 1].fData = CurveDataChangeByGain(fData, m_fPingDing);
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][nPointNum - 1].fRange = fRange;
		// 定量线的点
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][nPointNum - 1].fData = CurveDataChangeByGain(fData, m_fDingLiang);
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][nPointNum - 1].fRange = fRange;
		// 判废线的点
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[2][nPointNum - 1].fData = CurveDataChangeByGain(fData, m_fPanFei);
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[2][nPointNum - 1].fRange = fRange;

		m_pAScanView->m_stDacCurve[g_dwActiveChannel].fPingDing = m_fPingDing;
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].fDingLiang = m_fDingLiang;
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].fPanFei = m_fPanFei;

		SortCurvePointByRange(m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3], nPointNum);

		m_pAScanView->m_nSelectPoint = nPointNum - 1;
		DisplayValue(m_pChannelPanel->m_fPlus, m_pChannelPanel->m_fRefPlus, g_dwActiveChannel);
		// WangQianfeng add start on 2013-06-26
		//加点成功则充许修正点声程及幅值
		GetDlgItem(IDC_EDIT_DAC_RANGE)->EnableWindow(TRUE);//声程
		GetDlgItem(IDC_EDIT_DAC_AMP)->EnableWindow(TRUE);//幅值
		GetDlgItem(IDC_BUTTON_DAC_MODIFY_CONFIRM)->EnableWindow(TRUE);//确认修改
		if (m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum > 1)//加的点超过1点时
		{
			GetDlgItem(IDC_BUTTON_PREV_DAC_POINT)->EnableWindow(TRUE);//上一点
			GetDlgItem(IDC_BUTTON_NEXT_DAC_POINT)->EnableWindow(TRUE);//下一点
		}
		GetDlgItem(IDC_BUTTON_DAC_DELETE_POINT)->EnableWindow(TRUE);//删除点
		// WangQianfeng add start on 2013-06-26
	}
	else
	{
		AfxMessageBox(_T("加点失败,最多添加20个点"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacAddPoint"),"Leave");
		return;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacAddPoint"),"Leave");
}

void CDacCurveDlg::GainChangeDac()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::GainChangeDac"),"Enter");
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		int nPointNum = m_pAScanView->m_stDacCurve[i].nPointNum;

		if(nPointNum > 0)
		{
			float fChangeGain = m_pChannelPanel->m_fPlus - m_pAScanView->m_stDacCurve[i].fFirstGain;

			for(int j = 0; j < nPointNum; j++)
			{
				m_pAScanView->m_stDacCurve[i].stPointData[3][j].fData = CurveDataChangeByGain(m_pAScanView->m_stDacCurve[i].stPointData[3][j].fData, fChangeGain);
				float fData = m_pAScanView->m_stDacCurve[i].stPointData[3][j].fData;
				m_pAScanView->m_stDacCurve[i].stPointData[0][j].fData = CurveDataChangeByGain(fData, m_pAScanView->m_stDacCurve[i].fPingDing);
				m_pAScanView->m_stDacCurve[i].stPointData[1][j].fData = CurveDataChangeByGain(fData, m_pAScanView->m_stDacCurve[i].fDingLiang);
				m_pAScanView->m_stDacCurve[i].stPointData[1][j].fData = CurveDataChangeByGain(fData, m_pAScanView->m_stDacCurve[i].fPanFei);
			}
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::GainChangeDac"),"Leave");
}
void CDacCurveDlg::OnBnClickedButtonPrevDacPoint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonPrevDacPoint"),"Enter");
	// TODO: Add your control notification handler code here
	if(m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum < 1)  // 无样点
	{
		AfxMessageBox(_T("无样点"));
	}
	else
	{
		if(m_pAScanView->m_nSelectPoint == 0)
		{
			m_pAScanView->m_nSelectPoint = m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum - 1;
		}
		else
		{
			m_pAScanView->m_nSelectPoint--;
		}
		DisplayValue(m_pChannelPanel->m_fPlus, m_pChannelPanel->m_fRefPlus, g_dwActiveChannel);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonPrevDacPoint"),"Leave");
}

void CDacCurveDlg::OnBnClickedButtonNextDacPoint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonNextDacPoint"),"Enter");
	// TODO: Add your control notification handler code here
	if(m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum < 1)  // 无样点
	{
		AfxMessageBox(_T("无样点"));
	}
	else
	{
		if(m_pAScanView->m_nSelectPoint + 1 == m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum)
		{
			m_pAScanView->m_nSelectPoint = 0;
		}
		else
		{
			m_pAScanView->m_nSelectPoint++;
		}
		DisplayValue(m_pChannelPanel->m_fPlus, m_pChannelPanel->m_fRefPlus, g_dwActiveChannel);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonNextDacPoint"),"Leave");
}

void CDacCurveDlg::OnBnClickedButtonDacDeletePoint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacDeletePoint"),"Enter");
	// TODO: Add your control notification handler code here
	int nPointNum = m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum;
	if(m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum < 1)  // 无样点
	{
		AfxMessageBox(_T("无样点可删"));
	}
	else
	{
		if(m_pAScanView->m_nSelectPoint == nPointNum - 1)	// 删最后一点,选中点往前移
		{
			m_pAScanView->m_nSelectPoint --;
		}
		else
		{
			for(int i = m_pAScanView->m_nSelectPoint; i < nPointNum - 1; i++)	// 选中点后面的点数据往前移
			{
				 m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i] =  m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i + 1];
			}
		}
		
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum--;
		nPointNum = m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum;

		// 刷新控件显示
		DisplayValue(m_pChannelPanel->m_fPlus, m_pChannelPanel->m_fRefPlus, g_dwActiveChannel);
		
		for(int i = m_pAScanView->m_nSelectPoint; i < nPointNum; i++)
		{
			float fData = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i].fData;
			float fRange = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i].fRange;
			// 评定线的点
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][i].fData = CurveDataChangeByGain(fData, m_fPingDing);
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][i].fRange = fRange;
			// 定量线的点
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][i].fData = CurveDataChangeByGain(fData, m_fDingLiang);
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][i].fRange = fRange;
			// 判废线的点
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[2][i].fData = CurveDataChangeByGain(fData, m_fPanFei);
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[2][i].fRange = fRange;
		}
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].fPingDing = m_fPingDing;
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].fDingLiang = m_fDingLiang;
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].fPanFei = m_fPanFei;
		// WangQianfeng add start on 2013-06-26
		if (nPointNum <= 1)
		{
			if (nPointNum < 1)
			{
				GetDlgItem(IDC_EDIT_DAC_RANGE)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_DAC_AMP)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_DAC_MODIFY_CONFIRM)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_DAC_DELETE_POINT)->EnableWindow(FALSE);
			}
			GetDlgItem(IDC_BUTTON_PREV_DAC_POINT)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_NEXT_DAC_POINT)->EnableWindow(FALSE);
		}
		// WangQianfeng add end on 2013-06-26
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacDeletePoint"),"Leave");
}

void CDacCurveDlg::OnBnClickedButtonDacCreate()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacCreate"),"Enter");
	// TODO: Add your control notification handler code here
	if (IDCANCEL == MessageBox(_T("将会删除原有曲线，是否继续"),_T("提示"), MB_OKCANCEL))
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacCreate"),"Leave");
		return;
	}
	::ZeroMemory(m_pAScanView->m_stDacCurve, sizeof(DAC_CURVE_t) * CHANNEL_NUM);
	// 刷新控件显示
	float fCurrentPointRange = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][m_pAScanView->m_nSelectPoint].fRange;
	float fCurrentPointAmp = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][m_pAScanView->m_nSelectPoint].fData / MAX_HEIGHT * 100;
	m_nCurPointNum = 0;
	CString str;
	str.Format(_T("%.1f"),fCurrentPointRange);
	m_fCurrentPointRange = static_cast<float>(_tstof(str));
	str.Format(_T("%.1f"),fCurrentPointAmp);
	m_fCurrentPointAmp = static_cast<float>(_tstof(str));
	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacCreate"),"Leave");
}

void CDacCurveDlg::OnBnClickedRadioDacOn()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedRadioDacOn"),"Enter");
	// TODO: Add your control notification handler code here
	m_nDacStatus = 0;
	m_pAScanView->m_stDacCurve[g_dwActiveChannel].bSwitchFlag = TRUE;
	//SetTimer(TIMER_DEFECT_INFO, 200, NULL);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedRadioDacOn"),"Leave");
}

void CDacCurveDlg::OnBnClickedRadioDacOff()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedRadioDacOff"),"Enter");
	// TODO: Add your control notification handler code here
	m_nDacStatus = 1;
	m_pAScanView->m_stDacCurve[g_dwActiveChannel].bSwitchFlag = FALSE;
	//KillTimer(TIMER_DEFECT_INFO);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedRadioDacOff"),"Leave");
}

/*
void CDacCurveDlg::DisplayDefectInfo()
{
	int nChannelNum = g_dwActiveChannel;
	int nPointNum =  m_pAScanView->m_stDacCurve[nChannelNum].nPointNum;
	if(nPointNum > 0)
	{
		CURVEPOINT_DATA_t stDacCurveTemp[DAC_CURVE_POINT_NUM];
		memset(stDacCurveTemp, 0, sizeof(stDacCurveTemp));
		float fChangeGain = m_pChannelPanel->m_fPlus - m_pAScanView->m_stDacCurve[nChannelNum].fFirstGain;
		for(int i = 0; i< nPointNum; i++)
		{
			stDacCurveTemp[i].fData = CurveDataChangeByGain(m_pAScanView->m_stDacCurve[nChannelNum].stPointData[1][i].fData, fChangeGain);
			stDacCurveTemp[i].fRange = m_pAScanView->m_stDacCurve[nChannelNum].stPointData[1][i].fRange;
		}

		float fFirstPointRange = stDacCurveTemp[0].fRange;				//曲线第一点声程
		float fLastPointRange = stDacCurveTemp[nPointNum - 1].fRange;	//曲线最后一点声程
		if(!m_pFeatureWnd->m_bGateOpen[0])	// 闸门1没开
		{
			m_fDefectRange = 0;
			m_fDefectSize = 0;
		//	UpdateData(FALSE);
			return;
		}

		if(!m_pFeatureWnd->m_bMPMAltitudeDisplay)	// 最大波峰幅值没有打开
		{
			m_fDefectRange = 0;
			m_fDefectSize = 0;
		//	UpdateData(FALSE);
			return;
		}
		if(!m_pFeatureWnd->m_bMPMTimeDisplay)	// 最大波峰时间没有打开
		{
			m_fDefectRange = 0;
			m_fDefectSize = 0;
		//	UpdateData(FALSE);
			return;
		}

		BOOL bFeatureFlag[7];
		bFeatureFlag[0] = TRUE;
		bFeatureFlag[1] = m_pFeatureWnd->m_bFPFTimeDisplay;
		bFeatureFlag[2] = m_pFeatureWnd->m_bFPMTimeDisplay;
		bFeatureFlag[3] = m_pFeatureWnd->m_bMPFTimeDisplay;
		bFeatureFlag[4] = m_pFeatureWnd->m_bMPMTimeDisplay;
		bFeatureFlag[5] = m_pFeatureWnd->m_bFPMAltitudeDisplay;
		bFeatureFlag[6] = m_pFeatureWnd->m_bMPMAltitudeDisplay;

		int nTimeIndex = 0;			//最大波峰时间索引
		for(int i = 0; i < 4; i++)
		{
			if(bFeatureFlag[i])
			{
				nTimeIndex++;
			}

		}
		int nAmpIndex = 0;			//最大波峰幅值索引
		for(int i = 0; i < 6; i++)
		{
			if(bFeatureFlag[i])
			{
				nAmpIndex++;
			}

		}
		m_fDefectRange = m_pFeatureWnd->m_GateFeature.m_pfFeatures[0][nTimeIndex];
		float fDefectAmp = m_pFeatureWnd->m_GateFeature.m_pfFeatures[0][nAmpIndex];
		if(m_fDefectRange <= fFirstPointRange)	// 读取数据在曲线第一点之前
		{		
			if(fDefectAmp && stDacCurveTemp[0].fData)
			{
				m_fDefectSize = (float)(20 * log10((float)fDefectAmp / stDacCurveTemp[0].fData));
			}
		}
		if(m_fDefectRange >= fLastPointRange)	// 读取数据在曲线最后一点之后
		{		
			if(fDefectAmp && stDacCurveTemp[nPointNum - 1].fData)
			{
				m_fDefectSize = (float)(20 * log10((float)fDefectAmp / stDacCurveTemp[nPointNum - 1].fData));
			}
		}
		else
		{
			int i = 0;
			for(; i < nPointNum - 1; i++)	// 找出位于哪两个点之间
			{
				if(stDacCurveTemp[i].fData < m_fDefectRange && stDacCurveTemp[i+ 1].fData > m_fDefectRange)
				{
					break;
				}
			}
			float fCurCurveAmp = 0;	// 当前缺陷对应的曲线点幅值
			fCurCurveAmp = stDacCurveTemp[i].fData + (m_fDefectRange - stDacCurveTemp[i].fRange) / (stDacCurveTemp[i + 1].fRange - stDacCurveTemp[i].fRange) 
								* (stDacCurveTemp[i + 1].fData - stDacCurveTemp[i].fData);

			if(fCurCurveAmp && fDefectAmp)
			{
				m_fDefectSize = (float)(20 * log10(fDefectAmp / fCurCurveAmp));
			}
		}
	}
	CString str;
	str.Format(_T("%.1f"),m_fDefectSize);
	m_fDefectSize = _tstof(str);
	str.Format(_T("%.1f"),m_fDefectRange);
	m_fDefectRange = _tstof(str);
//	UpdateData(FALSE);
}*/

void CDacCurveDlg::DisplayDefectInfo(CDC *pDC, const CRect& rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::DisplayDefectInfo"),"Enter");
	// WangQianfeng add start on 2012-11-02
	if ( NULL == pDC )
	{
		MessageBox(_T("CDacCurveDlg::DisplayDefectInfo,pDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::DisplayDefectInfo"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	if(m_nDacStatus == 0)
	{
		int nChannelNum = g_dwActiveChannel;
		int nPointNum =  m_pAScanView->m_stDacCurve[nChannelNum].nPointNum;
		if(nPointNum > 0)
		{
			CURVEPOINT_DATA_t stDacCurveTemp[DAC_CURVE_POINT_NUM];
			memset(stDacCurveTemp, 0, sizeof(stDacCurveTemp));
			float fChangeGain = m_pChannelPanel->m_fPlus - m_pAScanView->m_stDacCurve[nChannelNum].fFirstGain;
		
			for(int i = 0; i< nPointNum; i++)
			{
				stDacCurveTemp[i].fData = CurveDataChangeByGain(m_pAScanView->m_stDacCurve[nChannelNum].stPointData[1][i].fData, fChangeGain);
				//Dot2Range改为Dot2RangeChannel  yachang.huang modify on 2014-6-5
				float fSyncRange = m_pChannelPanel->Dot2RangeChannel(m_pChannelPanel->m_nWaveSpeed, static_cast<int>(m_pFeatureWnd->m_GateFeature.m_pfFeatures[0][0] * ADFREQUENCY));// 同步声程
				stDacCurveTemp[i].fRange = m_pAScanView->m_stDacCurve[nChannelNum].stPointData[1][i].fRange + fSyncRange;
			}

			float fFirstPointRange = stDacCurveTemp[0].fRange;				//曲线第一点声程
			float fLastPointRange = stDacCurveTemp[nPointNum - 1].fRange;	//曲线最后一点声程
			if(!m_pFeatureWnd->m_bGateOpen[1])	// 闸门A没开
			{
				m_fDefectRange = 0;
				m_fDefectSize = 0;
				//	UpdateData(FALSE);
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::DisplayDefectInfo"),"Leave");
				return;
			}

			if(!m_pFeatureWnd->m_bMPMAltitudeDisplay)	// 最大波峰幅值没有打开
			{
				m_fDefectRange = 0;
				m_fDefectSize = 0;
				//	UpdateData(FALSE);
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::DisplayDefectInfo"),"Leave");
				return;
			}
			if(!m_pFeatureWnd->m_bMPMTimeDisplay)	// 最大波峰时间没有打开
			{
				m_fDefectRange = 0;
				m_fDefectSize = 0;
				//	UpdateData(FALSE);
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::DisplayDefectInfo"),"Leave");
				return;
			}

			BOOL bFeatureFlag[7];
			bFeatureFlag[0] = TRUE;
			bFeatureFlag[1] = m_pFeatureWnd->m_bFPFTimeDisplay;
			bFeatureFlag[2] = m_pFeatureWnd->m_bFPMTimeDisplay;
			bFeatureFlag[3] = m_pFeatureWnd->m_bMPFTimeDisplay;
			bFeatureFlag[4] = m_pFeatureWnd->m_bMPMTimeDisplay;
			bFeatureFlag[5] = m_pFeatureWnd->m_bFPMAltitudeDisplay;
			bFeatureFlag[6] = m_pFeatureWnd->m_bMPMAltitudeDisplay;

			int nTimeIndex = 0;			//最大波峰时间索引
			for(int i = 0; i < 4; i++)
			{
				if(bFeatureFlag[i])
				{
					nTimeIndex++;
				}

			}
			int nAmpIndex = 0;			//最大波峰幅值索引
			for(int i = 0; i < 6; i++)
			{
				if(bFeatureFlag[i])
				{
					nAmpIndex++;
				}

			}

			m_fDefectRange = m_pFeatureWnd->m_GateFeature.m_pfFeatures[1][nTimeIndex];
			float fDefectAmp = m_pFeatureWnd->m_GateFeature.m_pfFeatures[1][nAmpIndex];
			if(m_fDefectRange <= fFirstPointRange)	// 读取数据在曲线第一点之前
			{		
				if(fDefectAmp && stDacCurveTemp[0].fData)
				{
					m_fDefectSize = (float)(20 * log10((float)fDefectAmp / stDacCurveTemp[0].fData));
				}
			}
			if(m_fDefectRange >= fLastPointRange)	// 读取数据在曲线最后一点之后
			{		
				if(fDefectAmp && stDacCurveTemp[nPointNum - 1].fData)
				{
					m_fDefectSize = (float)(20 * log10((float)fDefectAmp / stDacCurveTemp[nPointNum - 1].fData));
				}
			}
			else
			{
				// wangbingfu add start at 2012-11-20
				int i = 0;
				for(; i < nPointNum - 1; i++)	// 找出位于哪两个点之间
				{
					if((m_fDefectRange >= stDacCurveTemp[i].fRange)
						&& (stDacCurveTemp[i+ 1].fRange >= m_fDefectRange))
					{
						break;
					}
				}
				float fCurCurveAmp = 0;	// 当前缺陷对应的曲线点幅值
				if(stDacCurveTemp[i].fData - stDacCurveTemp[i+1].fData > 1E-6 )
				{
					fCurCurveAmp = stDacCurveTemp[i].fData + (m_fDefectRange - stDacCurveTemp[i].fRange) / (stDacCurveTemp[i + 1].fRange - stDacCurveTemp[i].fRange) 
						* (stDacCurveTemp[i + 1].fData - stDacCurveTemp[i].fData);
				}
				else
				{
					fCurCurveAmp = stDacCurveTemp[i].fData - (m_fDefectRange - stDacCurveTemp[i].fRange) / (stDacCurveTemp[i + 1].fRange - stDacCurveTemp[i].fRange) 
						* (stDacCurveTemp[i].fData - stDacCurveTemp[i + 1].fData);
				}
				// wangbingfu add end at 2012-11-20

				if(fCurCurveAmp && fDefectAmp)
				{
					m_fDefectSize = (float)(20 * log10(fDefectAmp / fCurCurveAmp));
				}
			}
		}
		// 	TEXTMETRIC tm;
		// 	pDC->GetTextMetrics(&tm);
		// 
		// 	LONG lHalfHeight = tm.tmHeight/2;
		pDC->SetTextColor(RGB(255, 255, 255));
		CString str;
		str.Format(_T("Size:%.1f"),m_fDefectSize);
		pDC->TextOut(rect.right - 140, rect.top, str);
		str.Format(_T("Range:%.1f"),m_fDefectRange);
		pDC->TextOut(rect.right - 140, rect.top + 20, str);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::DisplayDefectInfo"),"Leave");
}

void CDacCurveDlg::OnBnClickedDacFileOpen()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedDacFileOpen"),"Enter");
	// TODO: Add your control notification handler code here
	TCHAR szFilters[] =
		_T("DAC文件 (*.dac)|*.dac||");

	CFileDialog dlg(TRUE, _T("dac"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();

		CFile file;
		CFileException e;

		if (file.Open(strPath, CFile::modeReadWrite), &e)
		{		
			int nRet = MessageBox(_T("当前数据将被覆盖，同意点“是”，否则点“否”。"), _T("提示"), MB_YESNO);

			if (nRet == IDYES)
			{		
				for(int i = 0; i < CHANNEL_NUM; i++)
				{
					file.Read(&m_pAScanView->m_stDacCurve[i], sizeof(DAC_CURVE_t));
				}
				
				int nCurChannel = g_dwActiveChannel;
				m_pAScanView->m_nSelectPoint = 0;
				m_nCurPointNum = m_pAScanView->m_nSelectPoint + 1;
				DisplayValue(m_pChannelPanel->m_fPlus, m_pChannelPanel->m_fRefPlus, g_dwActiveChannel);

				m_fPingDing = m_pAScanView->m_stDacCurve[nCurChannel].fPingDing;
				m_fDingLiang = m_pAScanView->m_stDacCurve[nCurChannel].fDingLiang;
				m_fPanFei = m_pAScanView->m_stDacCurve[nCurChannel].fPanFei;
				UpdateData(FALSE);
			}
			file.Close();
		}
		else
		{
			e.ReportError();
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedDacFileOpen"),"Leave");
}

void CDacCurveDlg::OnBnClickedButtonFileSave()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonFileSave"),"Enter");
	// TODO: Add your control notification handler code here
	TCHAR szFilters[] =
		_T("DAC文件 (*.dac)|*.dac||");

	CFileDialog dlg(FALSE, _T("dac"), NULL, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,	szFilters);

	if (dlg.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		int nPointNum = m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum;
		for(int i = 0; i < nPointNum; i++)
		{
			float fData = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i].fData;
			float fRange = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i].fRange;
			// 评定线的点
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][i].fData = CurveDataChangeByGain(fData, m_fPingDing);
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][i].fRange = fRange;
			// 定量线的点
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][i].fData = CurveDataChangeByGain(fData, m_fDingLiang);
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][i].fRange = fRange;
			// 判废线的点
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[2][i].fData = CurveDataChangeByGain(fData, m_fPanFei);
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[2][i].fRange = fRange;

			m_pAScanView->m_stDacCurve[g_dwActiveChannel].fPingDing = m_fPingDing;
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].fDingLiang = m_fDingLiang;
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].fPanFei = m_fPanFei;
		}

		CString strPath = dlg.GetPathName();

		CFile file;
		CFileException e;

		if (file.Open(strPath, CFile::modeReadWrite | CFile::modeCreate), &e)
		{		
			for(int i = 0; i < CHANNEL_NUM; i++)
			{
				file.Write(&m_pAScanView->m_stDacCurve[i], sizeof(DAC_CURVE_t));
			}
			file.Close();
		}
		else
		{
			e.ReportError();
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonFileSave"),"Leave");
}

void CDacCurveDlg::OnBnClickedButtonDacModifyConfirm()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacModifyConfirm"),"Enter");
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int nCurPoint = m_pAScanView->m_nSelectPoint;
	m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][nCurPoint].fData = m_fCurrentPointAmp;

	m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][nCurPoint].fRange = m_fCurrentPointRange;// - fSyncRange;
	SortCurvePointByRange(m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3], m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum);

	DisplayValue(m_pChannelPanel->m_fPlus, m_pChannelPanel->m_fRefPlus, g_dwActiveChannel);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacModifyConfirm"),"Leave");	
}
//更新对话框显示的值
void CDacCurveDlg::DisplayValue(float fPlus, float fRefPlus, int nChnNo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::DisplayValue"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	CFeatureInfoWnd* pFeatureWnd = pFrame->m_pFeatureWnd;
	float fTotalPlus = 0;
	fTotalPlus = GetTotalPlus(fPlus, fRefPlus, nChnNo);


	float fCurrentPointRange = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][m_pAScanView->m_nSelectPoint].fRange;// + fSyncRange; //改成不随同步声程变化
	float fCurrentPointAmp = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][m_pAScanView->m_nSelectPoint].fData;
	float fChangeGain = fTotalPlus - m_pAScanView->m_stDacCurve[g_dwActiveChannel].fFirstGain;
	fCurrentPointAmp = CurveDataChangeByGain(fCurrentPointAmp, fChangeGain);
	CString str;
	str.Format(_T("%.1f"),fCurrentPointRange);
	m_fCurrentPointRange = static_cast<float>(_tstof(str));
	str.Format(_T("%.1f"),fCurrentPointAmp);
	m_fCurrentPointAmp = static_cast<float>(_tstof(str));
	m_nCurPointNum = m_pAScanView->m_nSelectPoint + 1;
	UpdateData(FALSE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::DisplayValue"),"Leave");
}