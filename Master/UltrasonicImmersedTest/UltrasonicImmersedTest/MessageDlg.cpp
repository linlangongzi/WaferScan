// MessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "MessageDlg.h"


// CMessageDlg dialog

IMPLEMENT_DYNAMIC(CMessageDlg, CDialog)

CMessageDlg::CMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageDlg::IDD, pParent)
{

}

CMessageDlg::~CMessageDlg()
{
}

void CMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMessageDlg, CDialog)
END_MESSAGE_MAP()


// CMessageDlg message handlers
