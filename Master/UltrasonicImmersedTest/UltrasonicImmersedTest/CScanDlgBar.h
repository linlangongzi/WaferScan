#pragma once


// CCScanDlgBar �Ի���

class CCScanDlgBar : public CBCGPDialogBar
{
	DECLARE_DYNAMIC(CCScanDlgBar)

public:
	CCScanDlgBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCScanDlgBar();

// �Ի�������
	enum { IDD = IDD_CSCAN_DLGBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
