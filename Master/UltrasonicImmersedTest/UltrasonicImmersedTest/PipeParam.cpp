// PipeParam.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "PipeParam.h"
#include "CScanDlgBar.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "ChannelInfoDockView.h"
#include "MechanicalControl.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"

// CPipeParam dialog

IMPLEMENT_DYNAMIC(CPipeParam, CDialog)

CPipeParam::CPipeParam(CWnd* pParent /*=NULL*/)
	: CDialog(CPipeParam::IDD, pParent)
{
	m_nG11Index = 0;
	m_nG12Index = 0;
	m_nG21Index = 0;
	m_nG22Index = 0;
	m_nG31Index = 0;
	m_nG32Index = 0;
	m_nG41Index = 0;
	m_nG42Index = 0;
	m_nCscan1Index = 0;
	m_nCscan2Index = 0;
}

CPipeParam::~CPipeParam()
{
}

void CPipeParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_G11, m_cmbPipeG11);
	DDX_Control(pDX, IDC_COMBO_G12, m_cmbPipeG12);
	DDX_Control(pDX, IDC_COMBO_G21, m_cmbPipeG21);
	DDX_Control(pDX, IDC_COMBO_G22, m_cmbPipeG22);
	DDX_Control(pDX, IDC_COMBO_G31, m_cmbPipeG31);
	DDX_Control(pDX, IDC_COMBO_G32, m_cmbPipeG32);
	DDX_Control(pDX, IDC_COMBO_G41, m_cmbPipeG41);
	DDX_Control(pDX, IDC_COMBO_G42, m_cmbPipeG42);
	DDX_Control(pDX, IDC_COMBO_PIPE_CSAN1, m_cmbPipeCscan1);
	DDX_Control(pDX, IDC_COMBO_PIPE_CSCAN2, m_cmbPipeCscan2);
}


BEGIN_MESSAGE_MAP(CPipeParam, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_G11, &CPipeParam::OnCbnSelchangeComboG11)
	ON_CBN_SELCHANGE(IDC_COMBO_G12, &CPipeParam::OnCbnSelchangeComboG12)
	ON_CBN_SELCHANGE(IDC_COMBO_G21, &CPipeParam::OnCbnSelchangeComboG21)
	ON_CBN_SELCHANGE(IDC_COMBO_G22, &CPipeParam::OnCbnSelchangeComboG22)
	ON_CBN_SELCHANGE(IDC_COMBO_G31, &CPipeParam::OnCbnSelchangeComboG31)
	ON_CBN_SELCHANGE(IDC_COMBO_G32, &CPipeParam::OnCbnSelchangeComboG32)
	ON_CBN_SELCHANGE(IDC_COMBO_G41, &CPipeParam::OnCbnSelchangeComboG41)
	ON_CBN_SELCHANGE(IDC_COMBO_G42, &CPipeParam::OnCbnSelchangeComboG42)
	ON_CBN_SELCHANGE(IDC_COMBO_PIPE_CSAN1, &CPipeParam::OnCbnSelchangeComboPipeCsan1)
	ON_CBN_SELCHANGE(IDC_COMBO_PIPE_CSCAN2, &CPipeParam::OnCbnSelchangeComboPipeCscan2)
END_MESSAGE_MAP()


// CPipeParam message handlers

BOOL CPipeParam::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	m_pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);

	m_nG11Index = pFrame->m_nPipeChannelSelect[0];
	m_nG12Index = pFrame->m_nPipeChannelSelect[1];
	m_nG21Index = pFrame->m_nPipeChannelSelect[2];
	m_nG22Index = pFrame->m_nPipeChannelSelect[3];
	m_nG31Index = pFrame->m_nPipeChannelSelect[4];
	m_nG32Index = pFrame->m_nPipeChannelSelect[5];
	m_nG41Index = pFrame->m_nPipeChannelSelect[6];
	m_nG42Index = pFrame->m_nPipeChannelSelect[7];
	m_nCscan1Index = pFrame->m_nCScanSelect[0];
	m_nCscan2Index = pFrame->m_nCScanSelect[1];

	m_cmbPipeG11.SetCurSel(m_nG11Index);
	m_cmbPipeG12.SetCurSel(m_nG12Index);
	m_cmbPipeG21.SetCurSel(m_nG21Index);
	m_cmbPipeG22.SetCurSel(m_nG22Index);
	m_cmbPipeG31.SetCurSel(m_nG31Index);
	m_cmbPipeG32.SetCurSel(m_nG32Index);
	m_cmbPipeG41.SetCurSel(m_nG41Index);
	m_cmbPipeG42.SetCurSel(m_nG42Index);
	m_cmbPipeCscan1.SetCurSel(m_nCscan1Index);
	m_cmbPipeCscan2.SetCurSel(m_nCscan2Index);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPipeParam::OnCbnSelchangeComboG11()
{
	// TODO: Add your control notification handler code here
	m_nG11Index = m_cmbPipeG11.GetCurSel();
}

void CPipeParam::OnCbnSelchangeComboG12()
{
	// TODO: Add your control notification handler code here
	m_nG12Index = m_cmbPipeG12.GetCurSel();
}

void CPipeParam::OnCbnSelchangeComboG21()
{
	// TODO: Add your control notification handler code here
	m_nG21Index = m_cmbPipeG21.GetCurSel();
}

void CPipeParam::OnCbnSelchangeComboG22()
{
	// TODO: Add your control notification handler code here
	m_nG22Index = m_cmbPipeG22.GetCurSel();
}

void CPipeParam::OnCbnSelchangeComboG31()
{
	// TODO: Add your control notification handler code here
	m_nG31Index = m_cmbPipeG31.GetCurSel();
}

void CPipeParam::OnCbnSelchangeComboG32()
{
	// TODO: Add your control notification handler code here
	m_nG32Index = m_cmbPipeG32.GetCurSel();
}

void CPipeParam::OnCbnSelchangeComboG41()
{
	// TODO: Add your control notification handler code here
	m_nG41Index = m_cmbPipeG41.GetCurSel();
}

void CPipeParam::OnCbnSelchangeComboG42()
{
	// TODO: Add your control notification handler code here
	m_nG42Index = m_cmbPipeG42.GetCurSel();
}

void CPipeParam::OnCbnSelchangeComboPipeCsan1()
{
	// TODO: Add your control notification handler code here
	m_nCscan1Index = m_cmbPipeCscan1.GetCurSel();
}

void CPipeParam::OnCbnSelchangeComboPipeCscan2()
{
	// TODO: Add your control notification handler code here
	m_nCscan2Index = m_cmbPipeCscan2.GetCurSel();
}
