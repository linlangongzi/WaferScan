#pragma once


// CMidFilterDlg �Ի���

class CMidFilterDlg : public CDialog
{
	DECLARE_DYNAMIC(CMidFilterDlg)

public:
	CMidFilterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMidFilterDlg();

// �Ի�������
	enum { IDD = IDD_DIG_MIDFILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nFilterH;//�˲����ĸ߶�
	int m_nFilterW;//�˲����Ŀ��
	int m_nFilterMX;//�˲����ĵ��X����
	int m_nFilterMY;//�˲����ĵ��Y����
public:
	afx_msg void OnBnClickedOk();
};
