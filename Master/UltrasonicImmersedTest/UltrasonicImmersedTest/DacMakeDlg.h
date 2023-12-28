#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDacMakeDlg dialog

class CDacMakeDlg : public CDialog
{
	DECLARE_DYNAMIC(CDacMakeDlg)

public:
	CDacMakeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDacMakeDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DAC_MAKE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	CListCtrl m_DACList;
	CScrollBar m_scrollAmpLevel;
	CScrollBar m_scrollCurPos;
public:
	CEdit m_AmpLevel;	//选择期望的幅值
public:
	CEdit m_curPos;		//当前位置
public:
	CEdit m_curAmp;		//当前幅值
public:
	afx_msg void OnBnClickedRadioDacStatOn();
public:
	afx_msg void OnBnClickedRadioDacStatOff();
public:
	afx_msg void OnBnClickedRadioDacShowOn();
public:
	afx_msg void OnBnClickedRadioDacShowOff();
};
