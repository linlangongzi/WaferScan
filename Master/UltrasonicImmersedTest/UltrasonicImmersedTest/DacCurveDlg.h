#pragma once
class CUltrasonicImmersedTestView;
class CMainFrame;
class CUltrasonicImmersedTestDoc;
class CChannelPanel;
class CFeatureInfoWnd;
// CDacCurveDlg dialog

class CDacCurveDlg : public CDialog
{
	DECLARE_DYNAMIC(CDacCurveDlg)

public:
	CDacCurveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDacCurveDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DAC_CURVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	// 自动增益幅值
	float m_fAutoAmp;
	CMainFrame* m_pFrame;
	CUltrasonicImmersedTestView* m_pAScanView;//A扫视图指针
	CUltrasonicImmersedTestDoc* m_pDoc;
	CChannelPanel* m_pChannelPanel;
	CFeatureInfoWnd* m_pFeatureWnd;	// 特征窗口
public:
	void AutoGain();
//	void DisplayDefectInfo();
	void DisplayDefectInfo(CDC *pDC, const CRect& rect);
	void GainChangeDac();
	void DisplayValue(float fPlus, float fRefPlus, int nChnNo);
public:
	afx_msg void OnBnClickedButtonDacAutoGain();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnClose();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedButtonDacAddPoint();
public:
	float m_fPingDing;
public:
	float m_fDingLiang;
public:
	float m_fPanFei;
public:
	// 当前点声程
	float m_fCurrentPointRange;
public:
	// 当前点幅值
	float m_fCurrentPointAmp;
public:
	afx_msg void OnBnClickedButtonPrevDacPoint();
public:
	afx_msg void OnBnClickedButtonNextDacPoint();
public:
	// 当位点序号
	int m_nCurPointNum;
public:
	afx_msg void OnBnClickedButtonDacDeletePoint();
public:
	afx_msg void OnBnClickedButtonDacCreate();
public:
	int m_nDacStatus;
public:
	afx_msg void OnBnClickedRadioDacOn();
public:
	afx_msg void OnBnClickedRadioDacOff();
public:
	float m_fDefectSize;
public:
	// 声程位置
	float m_fDefectRange;
public:
	afx_msg void OnBnClickedDacFileOpen();
public:
	afx_msg void OnBnClickedButtonFileSave();
public:
	afx_msg void OnBnClickedButtonDacModifyConfirm();
};
//