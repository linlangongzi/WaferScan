// GeomRotaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "GeomRotaDlg.h"


// CGeomRotaDlg 对话框

IMPLEMENT_DYNAMIC(CGeomRotaDlg, CDialog)

CGeomRotaDlg::CGeomRotaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeomRotaDlg::IDD, pParent)
	, m_nRotateAngle(0)
{

}

CGeomRotaDlg::~CGeomRotaDlg()
{
}

void CGeomRotaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROTATE, m_nRotateAngle);
	DDV_MinMaxInt(pDX, m_nRotateAngle, 0, 360);
}


BEGIN_MESSAGE_MAP(CGeomRotaDlg, CDialog)
END_MESSAGE_MAP()


// CGeomRotaDlg 消息处理程序
