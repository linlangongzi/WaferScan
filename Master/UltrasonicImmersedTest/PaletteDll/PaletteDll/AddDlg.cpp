// AddDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AddDlg.h"


// CAddDlg dialog

IMPLEMENT_DYNAMIC(CAddDlg, CDialog)

CAddDlg::CAddDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAddDlg::IDD, pParent)
{
	nAddRect = 0;
}

CAddDlg::~CAddDlg()
{

}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAddDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CAddDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CAddDlg message handlers

void CAddDlg::OnPaint()
{

}

BOOL CAddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	pAddEdit = (CEdit*)GetDlgItem(IDC_ADD_EDIT);
	CString str;
	str.Format(_T("%d"), 1);
	pAddEdit->SetWindowText(str);
	pAddEdit->SetFocus();
	pAddEdit->SetSel(0, -1);

	return FALSE;
}

void CAddDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString str;
	pAddEdit->GetWindowText(str);
	nAddRect = _tstoi(str);
	OnOK();
}
