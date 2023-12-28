// OriginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "CScanDlgBar.h"
#include "ChannelInfoDockView.h"
#include "MechanicalControl.h"
#include "CScanParamDockBar.h"
#include "OriginDlg.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"

// COriginDlg 对话框

IMPLEMENT_DYNAMIC(COriginDlg, CDialog)

COriginDlg::COriginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COriginDlg::IDD, pParent)
{

}

COriginDlg::~COriginDlg()
{
}

void COriginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_OffSET_X, m_offset_x);
	DDX_Text(pDX, IDC_EDIT_OffSET_Y, m_offset_y); 
}


BEGIN_MESSAGE_MAP(COriginDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &COriginDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &COriginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// COriginDlg 消息处理程序


BOOL COriginDlg::OnInitDialog()
{
	//
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("COriginDlg::OnInitDialog"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pCScanView = NULL;
	if(m_pParentWnd == pFrame->m_wndCScanParamDockBar.m_pCScanParamView)
	{
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	}
	else if(m_pParentWnd == pFrame->m_wndCScanParamDockBar.m_pCScanParamView2)
	{
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
	}
	else
	{
		;//
	}

	CWnd* pWndX = GetDlgItem(IDC_STATIC_ORIGIN_X);
	CWnd* pWndY = GetDlgItem(IDC_STATIC_ORIGIN_Y);
	
	if(pCScanView->m_nAxisSelect == 1)	// XZ
	{
		pWndX->SetWindowText(_T("X坐标"));
		pWndY->SetWindowText(_T("Z坐标"));
	}
	else if(pCScanView->m_nAxisSelect == 2)	// YZ
	{
		pWndX->SetWindowText(_T("Y坐标"));
		pWndY->SetWindowText(_T("Z坐标"));
	}
	else if(pCScanView->m_nAxisSelect == 3
		|| pCScanView->m_nAxisSelect == 4)	//W wangbingfu add on 2013-01-09
	{
		pWndX->SetWindowText(_T("X坐标"));
		pWndY->SetWindowText(_T("R坐标"));
	}
	// Song Chenguang add start on 2013-05-04
	else if(pCScanView->m_nAxisSelect == 5)	//
	{
		pWndX->SetWindowText(_T("W坐标"));
		pWndY->SetWindowText(_T("Z坐标"));
	}
	// Song Chenguang add end on 2013-05-04
	else
	{
		pWndX->SetWindowText(_T("X坐标"));
		pWndY->SetWindowText(_T("Y坐标"));
	}
	CString strOffsetCoordinateX;
	CString strOffsetCoordinateY;
	strOffsetCoordinateX.Format(_T("%.2f"),pCScanView->m_Offset_Coordinate.x);
	strOffsetCoordinateY.Format(_T("%.2f"),pCScanView->m_Offset_Coordinate.y);
	m_offset_x = _tstof(strOffsetCoordinateX);
    m_offset_y = _tstof(strOffsetCoordinateY);


//     CEdit* pWndXValue = (CEdit*)GetDlgItem(IDC_EDIT_OffSET_X);
// 	CEdit* pWndYValue = (CEdit*)GetDlgItem(IDC_EDIT_OffSET_Y);
// 	CString strOffsetCoordinateX;
// 	CString strOffsetCoordinateY;
// 	strOffsetCoordinateX.Format(_T("%.2f"),m_offset_x);
//     strOffsetCoordinateY.Format(_T("%.2f"),m_offset_y);
// 	pWndXValue->SetWindowText(strOffsetCoordinateX);
// 	pWndYValue->SetWindowText(strOffsetCoordinateY);


	UpdateData(FALSE);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COriginDlg::OnInitDialog"),"Leave");

	m_NumEditX.SubclassDlgItem(IDC_EDIT_OffSET_X,this);  // yachang.huang  add on 2012-11-19
	m_NumEditY.SubclassDlgItem(IDC_EDIT_OffSET_Y,this);  // yachang.huang  add on 2012-11-19

	return TRUE;
}


void COriginDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void COriginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COriginDlg::OnBnClickedOk"),"Enter");
	UpdateData(TRUE);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* m_pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	CCScanView* m_pCScanView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);

	m_pCScanView->m_Offset_Coordinate.x = static_cast<float>(m_offset_x);
	m_pCScanView->m_Offset_Coordinate.y = static_cast<float>(m_offset_y);
	m_pCScanView2->m_Offset_Coordinate.x = static_cast<float>(m_offset_x);
	m_pCScanView2->m_Offset_Coordinate.y = static_cast<float>(m_offset_y);

	m_pCScanView->Invalidate(TRUE);
	m_pCScanView2->Invalidate(TRUE);
	OnOK();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COriginDlg::OnBnClickedOk"),"Leave");
}

