#pragma once
#include "afxwin.h"
//#include "CScanView.h"

// CPipeParam dialog

class CPipeParam : public CDialog
{
	DECLARE_DYNAMIC(CPipeParam)

public:
	CPipeParam(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPipeParam();

// Dialog Data
	enum { IDD = IDD_DIALOG_PIPE_PARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbPipeG11;
public:
	CComboBox m_cmbPipeG12;
public:
	CComboBox m_cmbPipeG21;
public:
	CComboBox m_cmbPipeG22;
public:
	CComboBox m_cmbPipeG31;
public:
	CComboBox m_cmbPipeG32;
public:
	CComboBox m_cmbPipeG41;
public:
	CComboBox m_cmbPipeG42;
public:
	CComboBox m_cmbPipeCscan1;
public:
	CComboBox m_cmbPipeCscan2;
public:
	virtual BOOL OnInitDialog();
public:
	int m_nG11Index;
	int m_nG12Index;
	int m_nG21Index;
	int m_nG22Index;
	int m_nG31Index;
	int m_nG32Index;
	int m_nG41Index;
	int m_nG42Index;
	int m_nCscan1Index;
	int m_nCscan2Index;
//	CCScanView* m_pCScanView;
public:
	afx_msg void OnCbnSelchangeComboG11();
public:
	afx_msg void OnCbnSelchangeComboG12();
public:
	afx_msg void OnCbnSelchangeComboG21();
public:
	afx_msg void OnCbnSelchangeComboG22();
public:
	afx_msg void OnCbnSelchangeComboG31();
public:
	afx_msg void OnCbnSelchangeComboG32();
public:
	afx_msg void OnCbnSelchangeComboG41();
public:
	afx_msg void OnCbnSelchangeComboG42();
public:
	afx_msg void OnCbnSelchangeComboPipeCsan1();
public:
	afx_msg void OnCbnSelchangeComboPipeCscan2();
};
