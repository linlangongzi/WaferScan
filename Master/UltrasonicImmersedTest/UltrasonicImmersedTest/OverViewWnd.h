#pragma once


// COverViewWnd

class COverViewWnd : public CWnd
{
	DECLARE_DYNAMIC(COverViewWnd)

public:
	COverViewWnd();
	virtual ~COverViewWnd();
private:
	CBitmap m_bmpBack;			// ����ͼ	
	CBitmap* m_pOldBmp;
	CRect m_rcMonitorRect;
	CPoint m_ptsRegion[5];
	CPoint m_ptsOldRegion[5];
public:
	CDC* m_pDCMemory;	// ͼ������Դ
	CSize m_SourceSize;	// ����Դ��С
	CPoint m_ptCursor;
public:
	BOOL GetScrollPos(CSize DisplaySize, int& nX, int& nY);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
};


