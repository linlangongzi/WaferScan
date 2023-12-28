/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 显微镜放大窗口执行文件
* Filename : MicroscopeWnd.cpp
*
* @author       Song Chenguang
* @version      1.0         Date: 2011-09-13 
*/

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "MicroscopeWnd.h"


// CMicroscopeWnd

IMPLEMENT_DYNAMIC(CMicroscopeWnd, CWnd)

CMicroscopeWnd::CMicroscopeWnd()
{
	m_pOldBmp = NULL;
}

CMicroscopeWnd::~CMicroscopeWnd()
{
}


BEGIN_MESSAGE_MAP(CMicroscopeWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CMicroscopeWnd message handlers



int CMicroscopeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CProbeLocationDlg::OnCreate"),"Enter");
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	// 禁止关闭操作
	CMenu* pMenu = GetSystemMenu(FALSE);
	if (pMenu != NULL)
	{
		pMenu->EnableMenuItem(SC_CLOSE, MF_BYCOMMAND | MF_DISABLED);
	}

	// 设置显示区域
	CRgn rgnClient;
	rgnClient.CreateEllipticRgn(0, 0, 100, 100);
	SetWindowRgn((HRGN)rgnClient.GetSafeHandle(), TRUE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CProbeLocationDlg::OnCreate"),"Leave");
	return 0;
}

BOOL CMicroscopeWnd::OnEraseBkgnd(CDC* pDC)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CProbeLocationDlg::OnEraseBkgnd"),"Enter");
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

	pDC->FillSolidRect(&rect, RGB(35, 184, 43));
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CProbeLocationDlg::OnEraseBkgnd"),"Leave");
	return TRUE;
}



LRESULT CMicroscopeWnd::OnNcHitTest(CPoint point)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CProbeLocationDlg::OnNcHitTest"),"Enter");
	// TODO: Add your message handler code here and/or call default
	// 使客户区可拖动
	UINT nHitTest = CWnd::OnNcHitTest(point);

	if (nHitTest == HTCLIENT)
	{
		nHitTest = HTCAPTION;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CProbeLocationDlg::OnNcHitTest"),"Leave");
	return nHitTest;
}

void CMicroscopeWnd::OnDestroy()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CProbeLocationDlg::OnDestroy"),"Enter");
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
	// Song Chenguang add start on 2012-11-05
	CDC* pDC = GetDC();
	pDC->SelectObject(m_pOldBmp);
	DeleteObject(m_bmpBack);
	// Song Chenguang add end on 2012-11-05
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CProbeLocationDlg::OnDestroy"),"Leave");
}
