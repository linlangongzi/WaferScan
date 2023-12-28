#pragma once
#include "Resource.h"

// CAddDlg dialog

class CAddDlg : public CDialog
{
	DECLARE_DYNAMIC(CAddDlg)

public:
	CAddDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddDlg();

	// Dialog Data
	enum { IDD = IDD_ADD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void OnPaint();
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

public:
	int nAddRect;
	CEdit *pAddEdit;
};
