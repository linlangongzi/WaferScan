#pragma once


// CSetGrayValueDlg �Ի���

class CSetGrayValueDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetGrayValueDlg)

public:
	CSetGrayValueDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetGrayValueDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_GRAYVALUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nGrayValue;
};
