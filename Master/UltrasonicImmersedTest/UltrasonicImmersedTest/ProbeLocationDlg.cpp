// ProbeLocationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "ProbeLocationDlg.h"


// CProbeLocationDlg dialog

IMPLEMENT_DYNAMIC(CProbeLocationDlg, CDialog)

CProbeLocationDlg::CProbeLocationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProbeLocationDlg::IDD, pParent)
	, m_fX1(0)
	, m_fX2(0)
	, m_fX3(0)
	, m_fX4(0)
	, m_fX5(0)
	, m_fX6(0)
	, m_fX7(0)
	, m_fX8(0)
	, m_fY1(0)
	, m_fY2(50.0f)
	, m_fY3(100.0f)
	, m_fY4(150.0f)
	, m_fY5(200.0f)
	, m_fY6(250.0f)
	, m_fY7(300.0f)
	, m_fY8(350.0f)
	, m_fZ1(0)
	, m_fZ2(0)
	, m_fZ3(0)
	, m_fZ4(0)
	, m_fZ5(0)
	, m_fZ6(0)
	, m_fZ7(0)
	, m_fZ8(0)
{

}

CProbeLocationDlg::~CProbeLocationDlg()
{
}

void CProbeLocationDlg::DoDataExchange(CDataExchange* pDX)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CProbeLocationDlg::DoDataExchange"),"Enter");
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X1, m_fX1);
	DDX_Text(pDX, IDC_EDIT_X2, m_fX2);
	DDX_Text(pDX, IDC_EDIT_X3, m_fX3);
	DDX_Text(pDX, IDC_EDIT_X4, m_fX4);
	DDX_Text(pDX, IDC_EDIT_X5, m_fX5);
	DDX_Text(pDX, IDC_EDIT_X6, m_fX6);
	DDX_Text(pDX, IDC_EDIT_X7, m_fX7);
	DDX_Text(pDX, IDC_EDIT_X8, m_fX8);
	DDX_Text(pDX, IDC_EDIT_Y1, m_fY1);
	DDX_Text(pDX, IDC_EDIT_Y2, m_fY2);
	DDX_Text(pDX, IDC_EDIT_Y3, m_fY3);
	DDX_Text(pDX, IDC_EDIT_Y4, m_fY4);
	DDX_Text(pDX, IDC_EDIT_Y5, m_fY5);
	DDX_Text(pDX, IDC_EDIT_Y6, m_fY6);
	DDX_Text(pDX, IDC_EDIT_Y7, m_fY7);
	DDX_Text(pDX, IDC_EDIT_Y8, m_fY8);
	DDX_Text(pDX, IDC_EDIT_Z1, m_fZ1);
	DDX_Text(pDX, IDC_EDIT_Z2, m_fZ2);
	DDX_Text(pDX, IDC_EDIT_Z3, m_fZ3);
	DDX_Text(pDX, IDC_EDIT_Z4, m_fZ4);
	DDX_Text(pDX, IDC_EDIT_Z5, m_fZ5);
	DDX_Text(pDX, IDC_EDIT_Z6, m_fZ6);
	DDX_Text(pDX, IDC_EDIT_Z7, m_fZ7);
	DDX_Text(pDX, IDC_EDIT_Z8, m_fZ8);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CProbeLocationDlg::DoDataExchange"),"Leave");
}


BEGIN_MESSAGE_MAP(CProbeLocationDlg, CDialog)
END_MESSAGE_MAP()


// CProbeLocationDlg message handlers
