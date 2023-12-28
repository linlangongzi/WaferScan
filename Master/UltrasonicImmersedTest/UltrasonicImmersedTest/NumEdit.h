#pragma once
#include "afxwin.h"

class CNumEdit :
	public CEdit
{
public:
	CNumEdit(void);
public:
	~CNumEdit(void);
protected:
	afx_msg void  OnChar (UINT nChar , UINT nRepCnt , UINT nFlags);
	DECLARE_MESSAGE_MAP()
};
