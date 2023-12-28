#pragma once


// CTaskItemDlg dialog

class CTaskItemDlg : public CDialog
{
	DECLARE_DYNAMIC(CTaskItemDlg)

public:
	CTaskItemDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTaskItemDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TASK_ITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strTaskItemName;
};
