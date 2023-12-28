#pragma once


// CPartGainCompensateDlg dialog

class CPartGainCompensateDlg : public CDialog
{
	DECLARE_DYNAMIC(CPartGainCompensateDlg)

public:
	CPartGainCompensateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPartGainCompensateDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PART_GAIN_COMPENSATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void OnCancel();
	void PostNcDestroy();
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	float m_fWStart;
	float m_fWStop;
	float m_fZStart;
	float m_fZStop;
	float m_fdBValue;
	float m_fPreviousdBValue;
	int m_nSelectMode;	// machao add on 2013-08-23
public:
	afx_msg void OnBnClickedButtonReset();
public:
	afx_msg void OnBnClickedButtonInstall();
public:
	afx_msg void OnBnClickedButtonCompensate();
	// machao add start on 2013-08-23
public:
	afx_msg void OnBnClickedRadioManuallySelect();
public:
	afx_msg void OnBnClickedRadioMouseSelect();
	// machao add end on 2013-08-23
};
