#pragma once


// CCustumEdit

class CCustumEdit : public CEdit
{
	DECLARE_DYNAMIC(CCustumEdit)

public:
	CCustumEdit();
	virtual ~CCustumEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnSetfocus();
public:
	afx_msg void OnUpdateLevelInsert(CCmdUI *pCmdUI);
};
