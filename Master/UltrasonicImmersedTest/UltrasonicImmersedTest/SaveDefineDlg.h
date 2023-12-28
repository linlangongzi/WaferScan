#pragma once


// CSaveDefineDlg dialog

class CSaveDefineDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaveDefineDlg)

public:
	CSaveDefineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSaveDefineDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_SAVE_PARAM_DEFINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bSaveAScan;
	BOOL m_bSaveGateI;
	BOOL m_bSaveGateA;
	BOOL m_bSaveGateB;
	BOOL m_bSaveGateC;
	int m_nGateIType;
	int m_nGateAType;
	int m_nGateBType;
	int m_nGateCType;
};
