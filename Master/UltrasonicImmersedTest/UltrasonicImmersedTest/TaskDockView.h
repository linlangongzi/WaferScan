#pragma once


// CTaskDockView

class CTaskDockView : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CTaskDockView)

public:
	CTaskDockView();
	virtual ~CTaskDockView();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	CCreateContext* m_pContext;
public:
	CTaskPanel* m_pFormView;
	// Operations
public:
	void SetContext (CCreateContext* pContext)
	{
		m_pContext = pContext;
	}
public:
	afx_msg void OnPaint();
};


