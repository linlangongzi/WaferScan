#pragma once


// CSmoothDlg �Ի���

class CSmoothDlg : public CDialog
{
	DECLARE_DYNAMIC(CSmoothDlg)

public:
	CSmoothDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSmoothDlg();

// �Ի�������
	enum { IDD = IDD_DLG_SMOOTH };

public:
	FLOAT m_fpArray[25];//ģ��Ԫ������ָ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
