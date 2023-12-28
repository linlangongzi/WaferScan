// CScanParamDockBar.cpp : 实现文件
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* C扫参数面板ControlBar工具栏源文件
*Filename : CScanParamDockBar.cpp
*
*@author       zhaoli
*@version      1.0         Date: 2011-7-27 
*/

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "BScanParamPanel.h"
#include "CScanParamDockBar.h"


// CCScanParamDockBar

IMPLEMENT_DYNAMIC(CCScanParamDockBar, CBCGPDockingControlBar)

CCScanParamDockBar::CCScanParamDockBar()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamDockBar::CCScanParamDockBar"),"Enter");

	m_pContext = NULL;				//add by Wangqianfeng 20121101
	m_pCScanParamView = NULL;		//add by Wangqianfeng 20121101
	m_pCScanParamView2 = NULL;		//add by Wangqianfeng 20121101
	// WangQianfeng delete start on 2013-06-28
	m_pBScanParamView = NULL;	    //add by Wangqianfeng 20121101
	// WangQianfeng delete end on 2013-06-28
	CRuntimeClass* pFactory = RUNTIME_CLASS(CCScanParamPanel);
	m_pCScanParamView = ( CCScanParamPanel*)(pFactory->CreateObject());
	m_pCScanParamView2 = ( CCScanParamPanel*)(pFactory->CreateObject());
	// WangQianfeng delete start on 2013-06-28
	CRuntimeClass* pFactory1 = RUNTIME_CLASS(CBScanParamPanel);
	m_pBScanParamView = ( CBScanParamPanel*)(pFactory1->CreateObject());
	// WangQianfeng delete end on 2013-06-28
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamDockBar::CCScanParamDockBar"),"Leave");
}

CCScanParamDockBar::~CCScanParamDockBar()
{
}


BEGIN_MESSAGE_MAP(CCScanParamDockBar, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(BCGM_CHANGE_ACTIVE_TAB, OnChangeActiveTab)	// machao add on 2013-09-3
END_MESSAGE_MAP()



// CCScanParamDockBar 消息处理程序

int CCScanParamDockBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamDockBar::OnCreate"),"Enter");

	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CCScanParamDockBar::OnCreate"),"Leave");
		return -1;
	}
	// TODO:  在此添加您专用的创建代码
	CRect rect(0,0,1000,1000);
	// TODO:  Add your specialized creation code here
	if (m_pCScanParamView != NULL)
	{
		m_pCScanParamView->CreateView(this,m_pContext);
	}
	if (m_pCScanParamView2 != NULL)
	{
		m_pCScanParamView2->CreateView(this,m_pContext);
	}
	// WangQianfeng delete start on 2013-06-28
	if (m_pBScanParamView != NULL)
	{
		m_pBScanParamView->CreateView(this,m_pContext);
	}
	// WangQianfeng delete end on 2013-06-28
	CRect rectTab;
	GetClientRect(rectTab);
	rectTab.bottom = rectTab.bottom + 1000;
	rectTab.right = rectTab.right + 1000;
	m_wndTab.Create (CBCGPTabWnd::STYLE_3D_VS2005, rectTab, this, 1,
		CBCGPTabWnd::LOCATION_TOP);

	m_wndTab.AddTab (m_pCScanParamView, _T("C扫一参数"), 0, FALSE);
	m_wndTab.AddTab (m_pCScanParamView2, _T("C扫二参数"), 1, FALSE);
	// WangQianfeng delete start on 2013-06-28
	m_wndTab.AddTab (m_pBScanParamView, _T("B扫参数"), 2, FALSE);
	// WangQianfeng delete end on 2013-06-28

//	m_wndTab.EnableAutoColor (TRUE);
	m_wndTab.EnableTabSwap (TRUE);
	m_wndTab.RecalcLayout ();
	m_wndTab.RedrawWindow ();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamDockBar::OnCreate"),"Leave");
	return 0;
}

void CCScanParamDockBar::OnPaint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamDockBar::OnPaint"),"Enter");

	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CBCGPDockingControlBar::OnPaint()
	CRect rect;
	GetClientRect (rect);

	dc.FillSolidRect (rect, ::GetSysColor (COLOR_3DFACE));

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamDockBar::OnPaint"),"Leave");
}

void CCScanParamDockBar::OnSize(UINT nType, int cx, int cy)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamDockBar::OnSize"),"Enter");

	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_pCScanParamView->GetSafeHwnd ())
	{
		m_pCScanParamView->SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	if (m_pCScanParamView2->GetSafeHwnd ())
	{
		m_pCScanParamView2->SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	// WangQianfeng delete start on 2013-06-28
	if (m_pBScanParamView->GetSafeHwnd ())
	{
		m_pBScanParamView->SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	// WangQianfeng delete end on 2013-06-28
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CCScanParamDockBar::OnSize"),"Leave");
}
// machao add start on 2013-09-3
LRESULT CCScanParamDockBar::OnChangeActiveTab(WPARAM wp, LPARAM /*lp*/)
{
	int iTabIndex = (int)wp;//激活哪个tab的索引
	CRect rect;
	GetWindowRect(&rect);
	int nx = rect.Width();//宽度
	int ny = rect.Height();//高度
	if (m_pCScanParamView->GetSafeHwnd () && iTabIndex == 0)
	{
		m_pCScanParamView->SetWindowPos (NULL, -1, -1, nx, ny, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	if (m_pCScanParamView2->GetSafeHwnd () && iTabIndex == 1)
	{
		m_pCScanParamView2->SetWindowPos (NULL, -1, -1, nx, ny, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
	return 0;
}	
// machao add end on 2013-09-3