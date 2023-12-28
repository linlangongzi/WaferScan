// TaskItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "TaskItemDlg.h"


// CTaskItemDlg dialog

IMPLEMENT_DYNAMIC(CTaskItemDlg, CDialog)

CTaskItemDlg::CTaskItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTaskItemDlg::IDD, pParent)
	, m_strTaskItemName(_T(""))
{

}

CTaskItemDlg::~CTaskItemDlg()
{
}

void CTaskItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ITEM_NAME, m_strTaskItemName);
}


BEGIN_MESSAGE_MAP(CTaskItemDlg, CDialog)
END_MESSAGE_MAP()


// CTaskItemDlg message handlers
