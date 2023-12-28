#pragma once


// CDropFaultDlg dialog

class CDropFaultDlg : public CDialog
{
	DECLARE_DYNAMIC(CDropFaultDlg)

public:
	CDropFaultDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDropFaultDlg();

	// Dialog Data
	enum { IDD = IDD_DIALOG_DROPFAULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	int m_nJZNum;
	int m_nTargetRange;
	int m_nCZJJ;
	CEdit *m_pJZNum;
	CEdit *m_pTargetRange;
	CEdit *m_pCZJJ;
public:
	afx_msg void OnBnClickedOk();
};
