#pragma once


// CSetGrayValueDlg 对话框

class CSetGrayValueDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetGrayValueDlg)

public:
	CSetGrayValueDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetGrayValueDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_GRAYVALUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nGrayValue;
};
