// CScanDlgBar.cpp : 实现文件
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "CScanDlgBar.h"


// CCScanDlgBar 对话框

IMPLEMENT_DYNAMIC(CCScanDlgBar, CBCGPDialogBar)

CCScanDlgBar::CCScanDlgBar(CWnd* pParent /*=NULL*/)
	//: CDialog(CCScanDlgBar::IDD, pParent)
{

}

CCScanDlgBar::~CCScanDlgBar()
{
}

void CCScanDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCScanDlgBar, CBCGPDialogBar)
END_MESSAGE_MAP()


// CCScanDlgBar 消息处理程序

BOOL CCScanDlgBar::OnInitDialog()
{
	//CDialogBar::OnInitDialog();
	UpdateData(FALSE);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
