#pragma once


// CEdgeDBSetDlg dialog

class CEdgeDBSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CEdgeDBSetDlg)

public:
	CEdgeDBSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEdgeDBSetDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_EDGE_DB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_fEdgeDB;
	float m_fCenterDB;
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
};
