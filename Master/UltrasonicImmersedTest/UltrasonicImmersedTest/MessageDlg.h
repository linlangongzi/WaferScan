#pragma once


// CMessageDlg dialog

class CMessageDlg : public CDialog
{
	DECLARE_DYNAMIC(CMessageDlg)

public:
	CMessageDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMessageDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
