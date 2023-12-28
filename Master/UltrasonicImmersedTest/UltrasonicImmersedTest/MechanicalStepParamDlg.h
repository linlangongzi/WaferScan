#pragma once


// CMechanicalStepParamDlg 对话框

class CMechanicalStepParamDlg : public CDialog
{
	DECLARE_DYNAMIC(CMechanicalStepParamDlg)

public:
	CMechanicalStepParamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMechanicalStepParamDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETSTEP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// x轴单步运动步进值
	float m_fXStep;
public:
	// y轴单步运动步进
	float m_fYStep;
public:
	// z轴单步运动步进
	float m_fZStep;
public:
	afx_msg void OnBnClickedOk();
public:
	float m_fABStep;		// A轴单步运动步进
	float m_fWStep;		// W轴单步运动步进
	float m_fRStep;		// R轴单步运动步进
};
