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
		// �����ߵĵ�
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][i].fData = CurveDataChangeByGain(fData, m_fPingDing);
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][i].fRange = fRange;
		// �����ߵĵ�
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][i].fData = CurveDataChangeByGain(fData, m_fDingLiang);
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][i].fRange = fRange;
		// �з��ߵĵ�
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
	if(!m_pFeatureWnd->m_bGateOpen[1])	// բ��1û��
	{
		KillTimer(TIMER_AUTO_GAIN);
		AfxMessageBox(_T("����ʧ�ܣ����ȿ���բ��1"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::AutoGain"),"Leave");
		return;
	}

	if(!m_pFeatureWnd->m_bMPMAltitudeDisplay)	// ��󲨷��ֵû�д�
	{
		KillTimer(TIMER_AUTO_GAIN);
		AfxMessageBox(_T("����ʧ�ܣ����ȴ���󲨷��ֵ"));
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

	if(fabs(fPeakValue - m_fAutoAmp) > 1)	// ����ֵ���趨ֵ������1E-1
	{
		float fStep = 0.3f;
		double fCha = fabs(fPeakValue - m_fAutoAmp);
		if(fCha > 5.0f)
			fStep = 0.3f;
		else
			fStep = 0.05f;

		if(fPeakValue > m_fAutoAmp)	// �������趨ֵ��������
		{
			if((m_pChannelPanel->m_fPlus - fStep) > MIN_PLUS)
			{
				m_pChannelPanel->m_fPlus -= fStep;
			}
			else
			{
				m_pChannelPanel->m_fPlus = MIN_PLUS;
				KillTimer(TIMER_AUTO_GAIN);
				AfxMessageBox(_T("����ʧ�ܣ��Ѿ�������С����"));
			}
		}
		else if(fPeakValue < m_fAutoAmp)// ����������
		{
			if((m_pChannelPanel->m_fPlus + fStep) < MAX_PLUS)
			{
				m_pChannelPanel->m_fPlus += fStep;
			}
			else
			{
				m_pChannelPanel->m_fPlus = MAX_PLUS;
				KillTimer(TIMER_AUTO_GAIN);
				AfxMessageBox(_T("����ʧ�ܣ��Ѿ������������"));
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
	//û�мӵ��򲻳������������̼���ֵ
	GetDlgItem(IDC_EDIT_DAC_RANGE)->EnableWindow(FALSE);//����
	GetDlgItem(IDC_EDIT_DAC_AMP)->EnableWindow(FALSE);//��ֵ
	GetDlgItem(IDC_BUTTON_DAC_MODIFY_CONFIRM)->EnableWindow(FALSE);//ȷ���޸�
	GetDlgItem(IDC_BUTTON_PREV_DAC_POINT)->EnableWindow(FALSE);//��һ��
	GetDlgItem(IDC_BUTTON_NEXT_DAC_POINT)->EnableWindow(FALSE);//��һ��
	GetDlgItem(IDC_BUTTON_DAC_DELETE_POINT)->EnableWindow(FALSE);//ɾ����

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDacCurveDlg::OnBnClickedButtonDacAddPoint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacAddPoint"),"Enter");
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(!m_pFeatureWnd->m_bGateOpen[0])	// բ��1û��
	{
		KillTimer(TIMER_AUTO_GAIN);
		AfxMessageBox(_T("�ӵ�ʧ�ܣ����ȿ���բ��1"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacAddPoint"),"Leave");
		return;
	}

	if(!m_pFeatureWnd->m_bMPMAltitudeDisplay)	// ��󲨷��ֵû�д�
	{
		KillTimer(TIMER_AUTO_GAIN);
		AfxMessageBox(_T("�ӵ�ʧ�ܣ����ȴ���󲨷��ֵ"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacAddPoint"),"Leave");
		return;
	}
	if(!m_pFeatureWnd->m_bMPMTimeDisplay)	// ��󲨷��ֵû�д�
	{
		KillTimer(TIMER_AUTO_GAIN);
		AfxMessageBox(_T("�ӵ�ʧ�ܣ����ȴ���󲨷�ʱ��"));
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

	int nTimeIndex = 0;			//��󲨷�ʱ������
	for(int i = 0; i < 4; i++)
	{
		if(bFeatureFlag[i])
		{
			nTimeIndex++;
		}

	}
	int nAmpIndex = 0;			//��󲨷��ֵ����
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
		if(nPointNum == 1)	// ��һ�㣬���´�ʱ����
		{
			fTotalPlus = GetTotalPlus(m_pChannelPanel->m_fPlus, m_pChannelPanel->m_fRefPlus, g_dwActiveChannel);
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].fFirstGain = fTotalPlus;
		}
		
		fTotalPlus = GetTotalPlus(m_pChannelPanel->m_fPlus, m_pChannelPanel->m_fRefPlus, g_dwActiveChannel);
		// �����ߵĵ�
		float fChangeGain = m_pAScanView->m_stDacCurve[g_dwActiveChannel].fFirstGain - fTotalPlus;
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][nPointNum - 1].fData =  CurveDataChangeByGain(m_pFeatureWnd->m_GateFeature.m_pfFeatures[1][nAmpIndex], fChangeGain);
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][nPointNum - 1].fRange = m_pFeatureWnd->m_GateFeature.m_pfFeatures[1][nTimeIndex];// + fSyncRange;
		// wangbingfu modify end at 2012-8-16

		float fData = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][nPointNum - 1].fData;
		float fRange = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][nPointNum - 1].fRange;
		// �����ߵĵ�
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][nPointNum - 1].fData = CurveDataChangeByGain(fData, m_fPingDing);
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][nPointNum - 1].fRange = fRange;
		// �����ߵĵ�
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][nPointNum - 1].fData = CurveDataChangeByGain(fData, m_fDingLiang);
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][nPointNum - 1].fRange = fRange;
		// �з��ߵĵ�
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[2][nPointNum - 1].fData = CurveDataChangeByGain(fData, m_fPanFei);
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[2][nPointNum - 1].fRange = fRange;

		m_pAScanView->m_stDacCurve[g_dwActiveChannel].fPingDing = m_fPingDing;
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].fDingLiang = m_fDingLiang;
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].fPanFei = m_fPanFei;

		SortCurvePointByRange(m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3], nPointNum);

		m_pAScanView->m_nSelectPoint = nPointNum - 1;
		DisplayValue(m_pChannelPanel->m_fPlus, m_pChannelPanel->m_fRefPlus, g_dwActiveChannel);
		// WangQianfeng add start on 2013-06-26
		//�ӵ�ɹ���������������̼���ֵ
		GetDlgItem(IDC_EDIT_DAC_RANGE)->EnableWindow(TRUE);//����
		GetDlgItem(IDC_EDIT_DAC_AMP)->EnableWindow(TRUE);//��ֵ
		GetDlgItem(IDC_BUTTON_DAC_MODIFY_CONFIRM)->EnableWindow(TRUE);//ȷ���޸�
		if (m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum > 1)//�ӵĵ㳬��1��ʱ
		{
			GetDlgItem(IDC_BUTTON_PREV_DAC_POINT)->EnableWindow(TRUE);//��һ��
			GetDlgItem(IDC_BUTTON_NEXT_DAC_POINT)->EnableWindow(TRUE);//��һ��
		}
		GetDlgItem(IDC_BUTTON_DAC_DELETE_POINT)->EnableWindow(TRUE);//ɾ����
		// WangQianfeng add start on 2013-06-26
	}
	else
	{
		AfxMessageBox(_T("�ӵ�ʧ��,������20����"));
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
	if(m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum < 1)  // ������
	{
		AfxMessageBox(_T("������"));
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
	if(m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum < 1)  // ������
	{
		AfxMessageBox(_T("������"));
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
	if(m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum < 1)  // ������
	{
		AfxMessageBox(_T("�������ɾ"));
	}
	else
	{
		if(m_pAScanView->m_nSelectPoint == nPointNum - 1)	// ɾ���һ��,ѡ�е���ǰ��
		{
			m_pAScanView->m_nSelectPoint --;
		}
		else
		{
			for(int i = m_pAScanView->m_nSelectPoint; i < nPointNum - 1; i++)	// ѡ�е����ĵ�������ǰ��
			{
				 m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i] =  m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i + 1];
			}
		}
		
		m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum--;
		nPointNum = m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum;

		// ˢ�¿ؼ���ʾ
		DisplayValue(m_pChannelPanel->m_fPlus, m_pChannelPanel->m_fRefPlus, g_dwActiveChannel);
		
		for(int i = m_pAScanView->m_nSelectPoint; i < nPointNum; i++)
		{
			float fData = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i].fData;
			float fRange = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i].fRange;
			// �����ߵĵ�
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][i].fData = CurveDataChangeByGain(fData, m_fPingDing);
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][i].fRange = fRange;
			// �����ߵĵ�
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][i].fData = CurveDataChangeByGain(fData, m_fDingLiang);
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][i].fRange = fRange;
			// �з��ߵĵ�
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
	if (IDCANCEL == MessageBox(_T("����ɾ��ԭ�����ߣ��Ƿ����"),_T("��ʾ"), MB_OKCANCEL))
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::OnBnClickedButtonDacCreate"),"Leave");
		return;
	}
	::ZeroMemory(m_pAScanView->m_stDacCurve, sizeof(DAC_CURVE_t) * CHANNEL_NUM);
	// ˢ�¿ؼ���ʾ
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

		float fFirstPointRange = stDacCurveTemp[0].fRange;				//���ߵ�һ������
		float fLastPointRange = stDacCurveTemp[nPointNum - 1].fRange;	//�������һ������
		if(!m_pFeatureWnd->m_bGateOpen[0])	// բ��1û��
		{
			m_fDefectRange = 0;
			m_fDefectSize = 0;
		//	UpdateData(FALSE);
			return;
		}

		if(!m_pFeatureWnd->m_bMPMAltitudeDisplay)	// ��󲨷��ֵû�д�
		{
			m_fDefectRange = 0;
			m_fDefectSize = 0;
		//	UpdateData(FALSE);
			return;
		}
		if(!m_pFeatureWnd->m_bMPMTimeDisplay)	// ��󲨷�ʱ��û�д�
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

		int nTimeIndex = 0;			//��󲨷�ʱ������
		for(int i = 0; i < 4; i++)
		{
			if(bFeatureFlag[i])
			{
				nTimeIndex++;
			}

		}
		int nAmpIndex = 0;			//��󲨷��ֵ����
		for(int i = 0; i < 6; i++)
		{
			if(bFeatureFlag[i])
			{
				nAmpIndex++;
			}

		}
		m_fDefectRange = m_pFeatureWnd->m_GateFeature.m_pfFeatures[0][nTimeIndex];
		float fDefectAmp = m_pFeatureWnd->m_GateFeature.m_pfFeatures[0][nAmpIndex];
		if(m_fDefectRange <= fFirstPointRange)	// ��ȡ���������ߵ�һ��֮ǰ
		{		
			if(fDefectAmp && stDacCurveTemp[0].fData)
			{
				m_fDefectSize = (float)(20 * log10((float)fDefectAmp / stDacCurveTemp[0].fData));
			}
		}
		if(m_fDefectRange >= fLastPointRange)	// ��ȡ�������������һ��֮��
		{		
			if(fDefectAmp && stDacCurveTemp[nPointNum - 1].fData)
			{
				m_fDefectSize = (float)(20 * log10((float)fDefectAmp / stDacCurveTemp[nPointNum - 1].fData));
			}
		}
		else
		{
			int i = 0;
			for(; i < nPointNum - 1; i++)	// �ҳ�λ����������֮��
			{
				if(stDacCurveTemp[i].fData < m_fDefectRange && stDacCurveTemp[i+ 1].fData > m_fDefectRange)
				{
					break;
				}
			}
			float fCurCurveAmp = 0;	// ��ǰȱ�ݶ�Ӧ�����ߵ��ֵ
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
		MessageBox(_T("CDacCurveDlg::DisplayDefectInfo,pDC = null"), _T("����"), MB_OK|MB_ICONERROR);
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
				//Dot2Range��ΪDot2RangeChannel  yachang.huang modify on 2014-6-5
				float fSyncRange = m_pChannelPanel->Dot2RangeChannel(m_pChannelPanel->m_nWaveSpeed, static_cast<int>(m_pFeatureWnd->m_GateFeature.m_pfFeatures[0][0] * ADFREQUENCY));// ͬ������
				stDacCurveTemp[i].fRange = m_pAScanView->m_stDacCurve[nChannelNum].stPointData[1][i].fRange + fSyncRange;
			}

			float fFirstPointRange = stDacCurveTemp[0].fRange;				//���ߵ�һ������
			float fLastPointRange = stDacCurveTemp[nPointNum - 1].fRange;	//�������һ������
			if(!m_pFeatureWnd->m_bGateOpen[1])	// բ��Aû��
			{
				m_fDefectRange = 0;
				m_fDefectSize = 0;
				//	UpdateData(FALSE);
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::DisplayDefectInfo"),"Leave");
				return;
			}

			if(!m_pFeatureWnd->m_bMPMAltitudeDisplay)	// ��󲨷��ֵû�д�
			{
				m_fDefectRange = 0;
				m_fDefectSize = 0;
				//	UpdateData(FALSE);
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDacCurveDlg::DisplayDefectInfo"),"Leave");
				return;
			}
			if(!m_pFeatureWnd->m_bMPMTimeDisplay)	// ��󲨷�ʱ��û�д�
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

			int nTimeIndex = 0;			//��󲨷�ʱ������
			for(int i = 0; i < 4; i++)
			{
				if(bFeatureFlag[i])
				{
					nTimeIndex++;
				}

			}
			int nAmpIndex = 0;			//��󲨷��ֵ����
			for(int i = 0; i < 6; i++)
			{
				if(bFeatureFlag[i])
				{
					nAmpIndex++;
				}

			}

			m_fDefectRange = m_pFeatureWnd->m_GateFeature.m_pfFeatures[1][nTimeIndex];
			float fDefectAmp = m_pFeatureWnd->m_GateFeature.m_pfFeatures[1][nAmpIndex];
			if(m_fDefectRange <= fFirstPointRange)	// ��ȡ���������ߵ�һ��֮ǰ
			{		
				if(fDefectAmp && stDacCurveTemp[0].fData)
				{
					m_fDefectSize = (float)(20 * log10((float)fDefectAmp / stDacCurveTemp[0].fData));
				}
			}
			if(m_fDefectRange >= fLastPointRange)	// ��ȡ�������������һ��֮��
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
				for(; i < nPointNum - 1; i++)	// �ҳ�λ����������֮��
				{
					if((m_fDefectRange >= stDacCurveTemp[i].fRange)
						&& (stDacCurveTemp[i+ 1].fRange >= m_fDefectRange))
					{
						break;
					}
				}
				float fCurCurveAmp = 0;	// ��ǰȱ�ݶ�Ӧ�����ߵ��ֵ
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
		_T("DAC�ļ� (*.dac)|*.dac||");

	CFileDialog dlg(TRUE, _T("dac"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();

		CFile file;
		CFileException e;

		if (file.Open(strPath, CFile::modeReadWrite), &e)
		{		
			int nRet = MessageBox(_T("��ǰ���ݽ������ǣ�ͬ��㡰�ǡ�������㡰�񡱡�"), _T("��ʾ"), MB_YESNO);

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
		_T("DAC�ļ� (*.dac)|*.dac||");

	CFileDialog dlg(FALSE, _T("dac"), NULL, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,	szFilters);

	if (dlg.DoModal() == IDOK)
	{
		UpdateData(TRUE);
		int nPointNum = m_pAScanView->m_stDacCurve[g_dwActiveChannel].nPointNum;
		for(int i = 0; i < nPointNum; i++)
		{
			float fData = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i].fData;
			float fRange = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][i].fRange;
			// �����ߵĵ�
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][i].fData = CurveDataChangeByGain(fData, m_fPingDing);
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[0][i].fRange = fRange;
			// �����ߵĵ�
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][i].fData = CurveDataChangeByGain(fData, m_fDingLiang);
			m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[1][i].fRange = fRange;
			// �з��ߵĵ�
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
//���¶Ի�����ʾ��ֵ
void CDacCurveDlg::DisplayValue(float fPlus, float fRefPlus, int nChnNo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDacCurveDlg::DisplayValue"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	CFeatureInfoWnd* pFeatureWnd = pFrame->m_pFeatureWnd;
	float fTotalPlus = 0;
	fTotalPlus = GetTotalPlus(fPlus, fRefPlus, nChnNo);


	float fCurrentPointRange = m_pAScanView->m_stDacCurve[g_dwActiveChannel].stPointData[3][m_pAScanView->m_nSelectPoint].fRange;// + fSyncRange; //�ĳɲ���ͬ�����̱仯
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