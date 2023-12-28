#pragma once


// CLogOnDlg 对话框

class CLogOnDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogOnDlg)

public:
	CLogOnDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogOnDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LOG_ON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 用户名称
	CString m_strUserName;
public:
	// 密码
	CString m_strPassWord;
public:
	afx_msg void OnBnClickedOk();
private:
	CString PwdEncode(CString& strPwd);
	virtual BOOL OnInitDialog();
};
