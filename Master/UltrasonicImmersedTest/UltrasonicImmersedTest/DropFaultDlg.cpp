// DropFault.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "DropFaultDlg.h"


// CDropFaultDlg dialog

IMPLEMENT_DYNAMIC(CDropFaultDlg, CDialog)

CDropFaultDlg::CDropFaultDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDropFaultDlg::IDD, pParent)
,m_nJZNum(0)
,m_nTargetRange(0)
,m_nCZJJ(0)
{

}

CDropFaultDlg::~CDropFaultDlg()
{
}

void CDropFaultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDropFaultDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CDropFaultDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CDropFaultDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_pJZNum = new CEdit;
	m_pJZNum = (CEdit*)GetDlgItem(IDC_EDIT_JZNUM);
	CString strJZNum;
	strJZNum.Format(_T("%d"), 0);
	m_pJZNum->SetWindowText(strJZNum);
	m_pJZNum->SetFocus();
	m_pJZNum->SetSel(0, -1);

	m_pTargetRange = new CEdit;
	m_pTargetRange = (CEdit*)GetDlgItem(IDC_EDIT_TARGETRABGE);
	CString strTargetRange;
	strTargetRange.Format(_T("%d"), 0);
	m_pTargetRange->SetWindowText(strTargetRange);
	m_pTargetRange->SetFocus();
	m_pTargetRange->SetSel(0, -1);

	m_pCZJJ = new CEdit;
	m_pCZJJ = (CEdit*)GetDlgItem(IDC_EDIT_CCJJ);
	CString strCZJJ;
	strCZJJ.Format(_T("%d"), 0);
	m_pCZJJ->SetWindowText(strCZJJ);
	m_pCZJJ->SetFocus();
	m_pCZJJ->SetSel(0, -1);

	return FALSE;
}

// CDropFaultDlg message handlers
void CDropFaultDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString strJZNum;
	m_pJZNum->GetWindowText(strJZNum);
	m_nJZNum = _tstoi(strJZNum);

	CString strTargetRange;
	m_pTargetRange->GetWindowText(strTargetRange);
	m_nTargetRange = _tstoi(strTargetRange);

	CString strCZJJ;
	m_pCZJJ->GetWindowText(strCZJJ);
	m_nCZJJ = _tstoi(strCZJJ);

	OnOK();
}
