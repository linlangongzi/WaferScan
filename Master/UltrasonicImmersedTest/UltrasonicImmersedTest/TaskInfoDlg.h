#pragma once


// CTaskInfoDlg dialog

class CTaskInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CTaskInfoDlg)

public:
	CTaskInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTaskInfoDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TASK_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTaskName;
};
