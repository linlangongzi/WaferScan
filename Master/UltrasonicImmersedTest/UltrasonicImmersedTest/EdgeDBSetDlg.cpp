// EdgeDBSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "EdgeDBSetDlg.h"


// CEdgeDBSetDlg dialog

IMPLEMENT_DYNAMIC(CEdgeDBSetDlg, CDialog)

CEdgeDBSetDlg::CEdgeDBSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEdgeDBSetDlg::IDD, pParent)
	, m_fEdgeDB(0)
	, m_fCenterDB(0)
{

}

CEdgeDBSetDlg::~CEdgeDBSetDlg()
{
}

void CEdgeDBSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DB, m_fEdgeDB);
}


BEGIN_MESSAGE_MAP(CEdgeDBSetDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CEdgeDBSetDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEdgeDBSetDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CEdgeDBSetDlg message handlers

void CEdgeDBSetDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (m_fEdgeDB >= m_fCenterDB)
	{
		MessageBox(_T("边缘点dB须小于中心点dB，请重新输入！"), _T("提示"), MB_OK);
		return;
	}
	OnOK();
}

void CEdgeDBSetDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
