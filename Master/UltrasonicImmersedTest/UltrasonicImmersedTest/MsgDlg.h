#pragma once


// MsgDlg dialog

class MsgDlg : public CDialog
{
	DECLARE_DYNAMIC(MsgDlg)

public:
	MsgDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MsgDlg();

// Dialog Data
	enum { IDD = IDD_MSGDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
};
