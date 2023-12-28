// TaskDockView.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"


// CTaskDockView

IMPLEMENT_DYNAMIC(CTaskDockView, CBCGPDockingControlBar)

CTaskDockView::CTaskDockView()
{
	CRuntimeClass* pFactory = RUNTIME_CLASS(CTaskPanel);
	m_pFormView = (CTaskPanel *)(pFactory->CreateObject());
}

CTaskDockView::~CTaskDockView()
{
}


BEGIN_MESSAGE_MAP(CTaskDockView, CBCGPDockingControlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTaskDockView message handlers



int CTaskDockView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void CTaskDockView::OnSize(UINT nType, int cx, int cy)
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_pFormView->GetSafeHwnd ())
	{
		m_pFormView->SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void CTaskDockView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CBCGPDockingControlBar::OnPaint() for painting messages
	CRect rect;
	GetClientRect (rect);

	dc.FillSolidRect (rect, ::GetSysColor (COLOR_3DFACE));
}
