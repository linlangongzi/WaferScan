#pragma once


// CColorStatic

class CColorStatic : public CStatic
{
	DECLARE_DYNAMIC(CColorStatic)

public:
	CColorStatic();
	virtual ~CColorStatic();
	BOOL Create(LPCTSTR lpszText, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);
	void SetBackColor(COLORREF col);

protected:
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

private:
	void OnLButtonDown(UINT nFlags, CPoint point);
	void UpdateCtrl();

private:
	COLORREF clrText;
	COLORREF clrBack;
	CBrush   m_Brush;

public:
	COLORREF m_color;            //用户选择的颜色
	CColorDialog dlg;
	bool OpenSelectDialog();
};
