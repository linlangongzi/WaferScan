// BScanParamPanel.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "BScanParamPanel.h"
#include "CScanDlgBar.h"
#include "ChannelInfoDockView.h"
#include "MechanicalControl.h"
#include "CScanParamDockBar.h"
#include "WSocket.h"
#include "ARSSocket.h"
#include "DataSocket.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "BScanView.h"
// CBScanParamPanel

IMPLEMENT_DYNCREATE(CBScanParamPanel, CFormView)

CBScanParamPanel::CBScanParamPanel()
	: CFormView(CBScanParamPanel::IDD)
	, m_fBScanOrigin(0)
	, m_fBScanWidth(600)
{

}

CBScanParamPanel::~CBScanParamPanel()
{
}

void CBScanParamPanel::DoDataExchange(CDataExchange* pDX)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanParamPanel::DoDataExchange"),"Enter");

	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_BSCAN_ORIGIN, m_sliderBScanOrigin);
	DDX_Control(pDX, IDC_SLIDER_BSCAN_WIDTH, m_sliderBScanWidth);
	DDX_Text(pDX, IDC_EDIT_BSCAN_ORIGIN, m_fBScanOrigin);
	DDX_Text(pDX, IDC_EDIT_BSCAN_WIDTH, m_fBScanWidth);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanParamPanel::DoDataExchange"),"Leave");
}

BEGIN_MESSAGE_MAP(CBScanParamPanel, CFormView)
	ON_WM_HSCROLL()
	ON_EN_CHANGE(IDC_EDIT_BSCAN_ORIGIN, &CBScanParamPanel::OnEnChangeEditBscanOrigin)
	ON_EN_CHANGE(IDC_EDIT_BSCAN_WIDTH, &CBScanParamPanel::OnEnChangeEditBscanWidth)
END_MESSAGE_MAP()


// CBScanParamPanel diagnostics

#ifdef _DEBUG
void CBScanParamPanel::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CBScanParamPanel::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

/** 
* 在工具栏窗体中创建视图
* @param     pParent [in]:  父窗口指针
* @param     pContext [in]:  窗口上下文                                            
* @return    void
* @since     1.0
*/
BOOL CBScanParamPanel::CreateView(CWnd* pParent, CCreateContext* pContext)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanParamPanel::CreateView"),"Enter");

	CRect rect(0,0,300,300);

	if (!Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		rect, pParent, AFX_IDW_PANE_FIRST, pContext))
	{
		TRACE0("Warning: couldn't create CMyFormView!\n");
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CBScanParamPanel::CreateView"),"Leave");
		return FALSE;
	}
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanParamPanel::CreateView"),"Leave");
	return TRUE;
}
// CBScanParamPanel message handlers

void CBScanParamPanel::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanParamPanel::OnHScroll"),"Enter");

	if (nSBCode == TB_THUMBTRACK && pScrollBar != NULL)
	{
		UpdateData(TRUE);

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CBScanView* pBScanView = (CBScanView*)pFrame->m_wndSplitter.GetPane(4,0);
		int nCtrlID = pScrollBar->GetDlgCtrlID();
		if (nCtrlID == IDC_SLIDER_BSCAN_ORIGIN)
		{
			m_fBScanOrigin = m_sliderBScanOrigin.GetPos()/10.0f;

			pBScanView->InvalidateRect(pBScanView->m_BScanScaleX);

		}
		else if (nCtrlID == IDC_SLIDER_BSCAN_WIDTH)
		{
			m_fBScanWidth =m_sliderBScanWidth.GetPos()/10.0f;

			pBScanView->InvalidateRect(pBScanView->m_BScanScaleX);
		}
	}
	UpdateData(FALSE);
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanParamPanel::OnHScroll"),"Leave");
}

void CBScanParamPanel::OnInitialUpdate()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanParamPanel::OnInitialUpdate"),"Enter");

	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	//初始化B扫滑竿
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	
	m_sliderBScanOrigin.SetRange(0, int(pApp->m_IniParams.m_info.fXRange*10), TRUE);
	m_sliderBScanWidth.SetRange(10, int(pApp->m_IniParams.m_info.fXRange*10), TRUE);
	m_sliderBScanOrigin.SetPos(int(m_fBScanOrigin*10));
	m_sliderBScanWidth.SetPos(int(m_fBScanWidth*10));

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanParamPanel::OnInitialUpdate"),"Leave");
}

void CBScanParamPanel::OnEnChangeEditBscanOrigin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (FALSE == UpdateData(TRUE))
	{
		return;
	}

	m_sliderBScanOrigin.SetPos(int(m_fBScanOrigin*10));

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CBScanView* pBScanView = (CBScanView*)pFrame->m_wndSplitter.GetPane(4,0);
	pBScanView->InvalidateRect(pBScanView->m_BScanScaleX);
}

void CBScanParamPanel::OnEnChangeEditBscanWidth()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (FALSE == UpdateData(TRUE))
	{
		return;
	}

	m_sliderBScanWidth.SetPos(int(m_fBScanWidth*10));

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CBScanView* pBScanView = (CBScanView*)pFrame->m_wndSplitter.GetPane(4,0);
	pBScanView->InvalidateRect(pBScanView->m_BScanScaleX);
}
