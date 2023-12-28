#pragma once


// CSmoothDlg 对话框

class CSmoothDlg : public CDialog
{
	DECLARE_DYNAMIC(CSmoothDlg)

public:
	CSmoothDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSmoothDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SMOOTH };

public:
	FLOAT m_fpArray[25];//模板元素数组指针
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nTempH;
	int m_nTempW;
	int m_nTempMX;
	int m_nTempMY;
	float m_fTempC;
public:
	void UpdateEdit(void);
public:
	afx_msg void OnEnKillfocusEditTemph();
public:
	afx_msg void OnEnKillfocusEditTempw();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
};
