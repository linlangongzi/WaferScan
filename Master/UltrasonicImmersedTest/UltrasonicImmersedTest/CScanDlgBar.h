#pragma once


// CCScanDlgBar 对话框

class CCScanDlgBar : public CBCGPDialogBar
{
	DECLARE_DYNAMIC(CCScanDlgBar)

public:
	CCScanDlgBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCScanDlgBar();

// 对话框数据
	enum { IDD = IDD_CSCAN_DLGBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
