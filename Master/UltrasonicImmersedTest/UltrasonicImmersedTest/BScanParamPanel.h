#pragma once
#include "afxcmn.h"



// CBScanParamPanel form view

class CBScanParamPanel : public CFormView
{
	DECLARE_DYNCREATE(CBScanParamPanel)

protected:
	CBScanParamPanel();           // protected constructor used by dynamic creation
	virtual ~CBScanParamPanel();

public:
	enum { IDD = IDD_BSCANPARAM_PANEL };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL CreateView(CWnd* pParent, CCreateContext* pContext);
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
public:
	virtual void OnInitialUpdate();
public:
	CSliderCtrl m_sliderBScanOrigin;
public:
	CSliderCtrl m_sliderBScanWidth;
public:
	float m_fBScanOrigin;
public:
	float m_fBScanWidth;
public:
	afx_msg void OnEnChangeEditBscanOrigin();
public:
	afx_msg void OnEnChangeEditBscanWidth();
};


