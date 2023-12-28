#pragma once


// CExButton
class CMechanicalControlPanel;

class CExButton : public CButton
{
	DECLARE_DYNAMIC(CExButton)

public:
	CExButton();
	virtual ~CExButton();

	short type;
	CMechanicalControlPanel* m_pParent;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


