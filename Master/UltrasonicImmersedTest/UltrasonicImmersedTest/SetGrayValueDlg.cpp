// SetGrayValueDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "SetGrayValueDlg.h"


// CSetGrayValueDlg 对话框

IMPLEMENT_DYNAMIC(CSetGrayValueDlg, CDialog)

CSetGrayValueDlg::CSetGrayValueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetGrayValueDlg::IDD, pParent)
	, m_nGrayValue(0)
{

}

CSetGrayValueDlg::~CSetGrayValueDlg()
{
}

void CSetGrayValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GRAYVALUE, m_nGrayValue);
	DDV_MinMaxInt(pDX, m_nGrayValue, 0, 255);
}


BEGIN_MESSAGE_MAP(CSetGrayValueDlg, CDialog)
END_MESSAGE_MAP()


// CSetGrayValueDlg 消息处理程序
