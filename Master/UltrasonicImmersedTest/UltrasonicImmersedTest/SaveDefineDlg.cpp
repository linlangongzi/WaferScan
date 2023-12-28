// SaveDefineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "SaveDefineDlg.h"


// CSaveDefineDlg dialog

IMPLEMENT_DYNAMIC(CSaveDefineDlg, CDialog)

CSaveDefineDlg::CSaveDefineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveDefineDlg::IDD, pParent)
	, m_bSaveAScan(FALSE)
	, m_bSaveGateI(TRUE)
	, m_bSaveGateA(TRUE)
	, m_bSaveGateB(TRUE)
	, m_bSaveGateC(TRUE)
	, m_nGateIType(0)
	, m_nGateAType(0)
	, m_nGateBType(0)
	, m_nGateCType(0)
{

}

CSaveDefineDlg::~CSaveDefineDlg()
{
}

void CSaveDefineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_A_SCAN_SAVE, m_bSaveAScan);
	DDX_Check(pDX, IDC_CHECK_GATE_I, m_bSaveGateI);
	DDX_Check(pDX, IDC_CHECK_GATE_A, m_bSaveGateA);
	DDX_Check(pDX, IDC_CHECK_GATE_B, m_bSaveGateB);
	DDX_Check(pDX, IDC_CHECK_GATE_C, m_bSaveGateC);
	DDX_Radio(pDX, IDC_RADIO_GATE_I_H, m_nGateIType);
	DDX_Radio(pDX, IDC_RADIO_GATE_A_H, m_nGateAType);
	DDX_Radio(pDX, IDC_RADIO_GATE_B_H, m_nGateBType);
	DDX_Radio(pDX, IDC_RADIO_GATE_C_H, m_nGateCType);
}


BEGIN_MESSAGE_MAP(CSaveDefineDlg, CDialog)
END_MESSAGE_MAP()


// CSaveDefineDlg message handlers
