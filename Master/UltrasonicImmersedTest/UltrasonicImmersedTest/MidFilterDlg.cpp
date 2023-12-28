// MidFilterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "MidFilterDlg.h"


// CMidFilterDlg 对话框

IMPLEMENT_DYNAMIC(CMidFilterDlg, CDialog)

CMidFilterDlg::CMidFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMidFilterDlg::IDD, pParent)
	, m_nFilterH(0)
	, m_nFilterW(0)
	, m_nFilterMX(0)
	, m_nFilterMY(0)
{

}

CMidFilterDlg::~CMidFilterDlg()
{
}

void CMidFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FH, m_nFilterH);
	DDV_MinMaxInt(pDX, m_nFilterH, 1, 8);
	DDX_Text(pDX, IDC_EDIT_FW, m_nFilterW);
	DDV_MinMaxInt(pDX, m_nFilterW, 1, 8);
	DDX_Text(pDX, IDC_EDIT_FMX, m_nFilterMX);
	DDX_Text(pDX, IDC_EDIT_FMY, m_nFilterMY);
}


BEGIN_MESSAGE_MAP(CMidFilterDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMidFilterDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMidFilterDlg 消息处理程序

void CMidFilterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//判断设置是否有效
	if ((m_nFilterH < 0)||(m_nFilterW< 0)||(m_nFilterMX > m_nFilterW - 1) ||
						(m_nFilterMY>m_nFilterH -1))
	{
		MessageBox(_T("滤波参数设置有误！"));
		return;
	}
	CDialog::OnOK();
}
