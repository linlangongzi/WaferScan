#pragma once


// CHanHeRateDlg dialog
class CCScanParamPanel;

class CHanHeRateDlg : public CDialog
{
	DECLARE_DYNAMIC(CHanHeRateDlg)

public:
	CHanHeRateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHanHeRateDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_HANHE_RATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void DrawColorRate();
	void DrawHistogram();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnPaint();
protected:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	// machao add start on 2013-09-5
	BOOL DrawVRuler(CDC *pDC,CRect rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);
	// machao add end on 2013-09-5
public:
	CCScanParamPanel* m_pParentWnd;
};
