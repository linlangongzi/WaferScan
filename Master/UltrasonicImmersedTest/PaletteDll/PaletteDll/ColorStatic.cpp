// ColorStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "ColorStatic.h"
#include "StaticDlg.h"

#define TRANS_BACK -1

// CColorStatic

IMPLEMENT_DYNAMIC(CColorStatic, CStatic)

CColorStatic::CColorStatic()
{
	m_color = RGB(255,76,9);
	clrText = RGB(0, 0, 0);
	clrBack = TRANS_BACK;
}

CColorStatic::~CColorStatic()
{
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()

// CColorStatic 消息处理程序

void CColorStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	CStaticDlg* pParent = (CStaticDlg*)GetParent();
	pParent->m_hFocusEdit = this->GetSafeHwnd();
	pParent->ChangeColor();
	CStatic::OnLButtonDown(nFlags, point);
}

bool CColorStatic::OpenSelectDialog()
{
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN;
	dlg.m_cc.rgbResult = m_color;
	if(dlg.DoModal() == IDOK)
	{
		m_color = dlg.m_cc.rgbResult;
		return true;
	}
	else
	{
		return false;
	}
}

BOOL CColorStatic::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID /* = 0xffff */)
{
	if (!CStatic::Create( lpszText , dwStyle, rect, pParentWnd, nID ) )
		return FALSE;

	return TRUE;
}

void CColorStatic::SetBackColor(COLORREF col)
{ 
	clrBack = col;
	UpdateCtrl();
}

HBRUSH CColorStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	m_Brush.DeleteObject();

	if (clrBack == TRANS_BACK) {
		m_Brush.CreateStockObject(HOLLOW_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
	}
	else {
		m_Brush.CreateSolidBrush(clrBack);
		pDC->SetBkColor(clrBack);
	}

	pDC->SetTextColor(clrText);

	return (HBRUSH)m_Brush;
}

void CColorStatic::UpdateCtrl()
{
	CWnd* pParent = GetParent();
	CRect rect;

	GetWindowRect(rect);
	pParent->ScreenToClient(rect);

	pParent->InvalidateRect(rect, true);
}
