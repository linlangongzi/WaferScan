#pragma once


// CMidFilterDlg 对话框

class CMidFilterDlg : public CDialog
{
	DECLARE_DYNAMIC(CMidFilterDlg)

public:
	CMidFilterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMidFilterDlg();

// 对话框数据
	enum { IDD = IDD_DIG_MIDFILTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nFilterH;//滤波器的高度
	int m_nFilterW;//滤波器的宽度
	int m_nFilterMX;//滤波中心点的X坐标
	int m_nFilterMY;//滤波中心点的Y坐标
public:
	afx_msg void OnBnClickedOk();
};
