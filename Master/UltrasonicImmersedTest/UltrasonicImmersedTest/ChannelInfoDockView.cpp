//ChannelInfoDockView.cpp : 实现文件
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 通道面板窗体工具栏源文件
*Filename : ChannelInfoDockView.cpp
*
*@author       songchenguang zhaoli 
*@version      1.0         Date: 2011-7-10
*/

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"


// CChannelInfoDockView

IMPLEMENT_DYNAMIC(CChannelInfoDockView, CBCGPDockingControlBar)

CChannelInfoDockView::CChannelInfoDockView()
{
	CRuntimeClass* pFactory = RUNTIME_CLASS(CChannelPanel);
	m_pFormView = (CChannelPanel *)(pFactory->CreateObject());
}

CChannelInfoDockView::~CChannelInfoDockView()
{
}


BEGIN_MESSAGE_MAP(CChannelInfoDockView, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CChannelInfoDockView message handlers



int CChannelInfoDockView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (m_pFormView != NULL)
	{
		m_pFormView->CreateView(this, m_pContext);
	}
	return 0;
}

void CChannelInfoDockView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CBCGPDockingControlBar::OnPaint() for painting messages

	CRect rect;
	GetClientRect (rect);

	dc.FillSolidRect (rect, ::GetSysColor (COLOR_3DFACE));
}

void CChannelInfoDockView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here

	if (m_pFormView->GetSafeHwnd ())
	{
		m_pFormView->SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

// 需要改进
CChannelInfo CChannelInfoDockView::GetChannelInfo(const WORD& wChannelNo)
{
	CChannelInfo info;
	if (m_pFormView != NULL)
	{
		info = m_pFormView->GetChannelInfo(wChannelNo);		
	}
	 
	return info;
}