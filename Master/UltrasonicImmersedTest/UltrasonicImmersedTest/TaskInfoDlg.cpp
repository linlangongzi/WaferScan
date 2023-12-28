// TaskInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "TaskInfoDlg.h"


// CTaskInfoDlg dialog

IMPLEMENT_DYNAMIC(CTaskInfoDlg, CDialog)

CTaskInfoDlg::CTaskInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTaskInfoDlg::IDD, pParent)
	, m_strTaskName(_T(""))
{

}

CTaskInfoDlg::~CTaskInfoDlg()
{
}

void CTaskInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TASK_NAME, m_strTaskName);
}


BEGIN_MESSAGE_MAP(CTaskInfoDlg, CDialog)
END_MESSAGE_MAP()


// CTaskInfoDlg message handlers
