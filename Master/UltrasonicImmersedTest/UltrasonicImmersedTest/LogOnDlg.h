#pragma once


// CLogOnDlg �Ի���

class CLogOnDlg : public CDialog
{
	DECLARE_DYNAMIC(CLogOnDlg)

public:
	CLogOnDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogOnDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_LOG_ON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// �û�����
	CString m_strUserName;
public:
	// ����
	CString m_strPassWord;
public:
	afx_msg void OnBnClickedOk();
private:
	CString PwdEncode(CString& strPwd);
	virtual BOOL OnInitDialog();
};
