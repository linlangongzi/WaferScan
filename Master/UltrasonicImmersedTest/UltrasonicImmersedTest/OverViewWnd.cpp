// OverViewWnd.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "OverViewWnd.h"

#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
#include "UltrasonicImmersedTestDoc.h"

// COverViewWnd

IMPLEMENT_DYNAMIC(COverViewWnd, CWnd)

COverViewWnd::COverViewWnd()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::COverViewWnd"),"Enter");
	m_pDCMemory = NULL;
	m_SourceSize = CSize(100, 100);
	m_rcMonitorRect = CRect(0, 0, 0, 0);
	m_ptCursor = CPoint(0, 0);

	m_pOldBmp = NULL;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::COverViewWnd"),"Leave");
}

COverViewWnd::~COverViewWnd()
{
}


BEGIN_MESSAGE_MAP(COverViewWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// COverViewWnd message handlers

void COverViewWnd::OnPaint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::OnPaint"),"Enter");
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	if (m_pDCMemory != NULL)
	{
		CRect rect;
		GetClientRect(&rect);
		dc.StretchBlt(0, 0, rect.Width(), rect.Height(), m_pDCMemory, 
 			0, 0, m_SourceSize.cx, m_SourceSize.cy, SRCCOPY);

		CPen pen(PS_SOLID, 1, RGB(128, 128, 128));
		CPen * pOldPen = dc.SelectObject(&pen);

		dc.Polyline(m_ptsRegion, 5);
		dc.SelectObject(pOldPen);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::OnPaint"),"Leave");
}

BOOL COverViewWnd::OnEraseBkgnd(CDC* pDC)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::OnEraseBkgnd"),"Enter");
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(&rect);
	static int nCount = 0;
	if(nCount == 0)
	{
		BOOL bRet = m_bmpBack.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
		
		if (bRet)
		{
			m_pOldBmp = pDC->SelectObject(&m_bmpBack);
			nCount++;
		}
		
	}

	pDC->FillSolidRect(&rect, RGB(255, 255, 255));
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::OnEraseBkgnd"),"Leave");
	return TRUE;
}

int COverViewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::OnCreate"),"Enter");
	if (CWnd::OnCreate(lpCreateStruct) == -1)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("COverViewWnd::OnCreate"),"Leave");
		return -1;
	}

	// TODO:  Add your specialized creation code here
	// 禁止关闭操作
	CMenu* pMenu = GetSystemMenu(FALSE);
	if (pMenu != NULL)
	{
		pMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_DISABLED);
	}

	CRect rect;
	GetClientRect(&rect);
	int nWidth = rect.Width() / 4;
	int nHeight = rect.Height() / 4;

	m_rcMonitorRect = CRect(0, 0, nWidth, nHeight);
	m_ptsRegion[0] = CPoint(m_rcMonitorRect.left, m_rcMonitorRect.top);
	m_ptsRegion[1] = CPoint(m_rcMonitorRect.right, m_rcMonitorRect.top);
	m_ptsRegion[2] = CPoint(m_rcMonitorRect.right, m_rcMonitorRect.bottom);
	m_ptsRegion[3] = CPoint(m_rcMonitorRect.left, m_rcMonitorRect.bottom);
	m_ptsRegion[4] = CPoint(m_rcMonitorRect.left, m_rcMonitorRect.top);

	for(int i=0; i<5; i++)
		m_ptsOldRegion[i] = m_ptsRegion[i];
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::OnCreate"),"Leave");
	return 0;
}

void COverViewWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::OnMouseMove"),"Enter");
	// TODO: Add your message handler code here and/or call default
	if (nFlags & MK_LBUTTON)
	{		
		m_rcMonitorRect.MoveToXY(point.x, point.y);

		m_ptCursor = point;
		Invalidate(FALSE);

		m_ptsRegion[0] = CPoint(m_rcMonitorRect.left, m_rcMonitorRect.top);
		m_ptsRegion[1] = CPoint(m_rcMonitorRect.right, m_rcMonitorRect.top);
		m_ptsRegion[2] = CPoint(m_rcMonitorRect.right, m_rcMonitorRect.bottom);
		m_ptsRegion[3] = CPoint(m_rcMonitorRect.left, m_rcMonitorRect.bottom);
		m_ptsRegion[4] = CPoint(m_rcMonitorRect.left, m_rcMonitorRect.top);

 		CCScanView * pWnd = (CCScanView*)GetParent();
		if (pWnd != NULL)
		{
			CSize size = pWnd->getDisplaySize();
			int nX = 0;
			int nY = 0;
			GetScrollPos(size, nX, nY);

			POINT pt;
			pt.x = nX;
			pt.y = nY;
			pWnd->ScrollToPosition(pt);

			// 移动自身到父窗口右上角
			pWnd->SendMessage(WM_MOVE_OVERVIEW, 0, 0);
		}
		else
		{
			AfxMessageBox(_T("COverViewWnd::OnMouseMove GetParent()获取指针失败！"));
		}
		
	}

	CWnd::OnMouseMove(nFlags, point);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::OnMouseMove"),"Leave");
}

BOOL COverViewWnd::GetScrollPos(CSize DisplaySize, int& nX, int& nY)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::GetScrollPos"),"Enter");
	CRect rect;
	GetClientRect(&rect);

	nX = int(float(m_ptCursor.x) / float(rect.Width()) * DisplaySize.cx);
	nY = int(float(m_ptCursor.y) / float(rect.Height()) * DisplaySize.cy);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::GetScrollPos"),"Leave");
	return TRUE;
}
void COverViewWnd::OnDestroy()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::OnDestroy"),"Enter");
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
	// Song Chenguang add start on 2012-11-05
	CDC* pDC = GetDC();
	pDC->SelectObject(m_pOldBmp);
	DeleteObject(m_bmpBack);
	// Song Chenguang add end on 2012-11-05
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("COverViewWnd::OnDestroy"),"Leave");
}
