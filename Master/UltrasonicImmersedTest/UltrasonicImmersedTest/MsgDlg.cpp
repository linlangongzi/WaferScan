// MsgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "MsgDlg.h"


// MsgDlg dialog

IMPLEMENT_DYNAMIC(MsgDlg, CDialog)

MsgDlg::MsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MsgDlg::IDD, pParent)
{

}

MsgDlg::~MsgDlg()
{
}

void MsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MsgDlg, CDialog)
END_MESSAGE_MAP()


// MsgDlg message handlers


BOOL MsgDlg::OnInitDialog()
{

	SetWindowText(_T("²Ù×÷¹ýÓÚÆµ·±£¡"));
	SetWindowPos(NULL, -1, -1, 150, 100, SWP_NOMOVE);
	CenterWindow();

	return TRUE;
}