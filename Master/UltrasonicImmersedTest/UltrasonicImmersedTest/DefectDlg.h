/**
* Copyright(c) 2012 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 缺陷标注实现窗口头文件
* Filename: DefectDlg.cpp
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2012-11-21
*/
#pragma once

#include <map>
using namespace std;
// CDefectDlg dialog

class CDefectDlg : public CDialog
{
	DECLARE_DYNAMIC(CDefectDlg)

public:
	CDefectDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDefectDlg();

	// Dialog Data
	enum { IDD = IDD_DIALOG_DEFECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
protected:
	HICON	m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckBadflag();
public:
	CComboBox m_cmbAreaSel;
	CComboBox m_cmbSymbolSel1;
	CComboBox m_cmbSymbolSel2;
	CComboBox m_cmbSymbolSel3;
public:
	BOOL m_bBadFlag;//使能开关
	int m_nDefectEnable;//配置文件中的使能和m_bBadFlag对应
	CString m_csSymbolSetFilePath;
	CString m_csfont;
	int m_nFontSize;
	COLORREF m_FontColor;
	int m_nlfUnderline;//下划线
	int m_nlfStrikeOut;

private:
	int m_nSymbol1;
	int m_nSymbol2;
	int m_nSymbol3;
	int m_nSymbol1Enable;
	int m_nSymbol2Enable;
	int m_nSymbol3Enable;
	BOOL m_bCheckFlag1;
	BOOL m_bCheckFlag2;
	BOOL m_bCheckFlag3;
	CString m_csFontColor;//用于将RGB拼成一个串写入配置文件。
public:
	void UpdateMapDrawSymbol();
	CString GetSetFilePath();
	void GetDataFromSetFile(CString csFilePath);
	void WriteDataToSetFile();
	void UpdateCtrlStatus();
	void UpdateCheckBoxSelect();
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnCbnSelchangeComboArea();
public:
	afx_msg void OnBnClickedButtonBadfont();
public:
	afx_msg void OnCbnSelchangeComboSymbol1();
public:
	afx_msg void OnCbnSelchangeComboSymbol2();
public:
	afx_msg void OnCbnSelchangeComboSymbol3();
public:
	afx_msg void OnBnClickedCancel();
public:
	// 最小面积
	double m_dMinArea;
public:
	// 分段面积值A
	double m_dAreaA;
public:
	// 分段面积值B
	double m_dAreaB;
public:
	afx_msg void OnBnClickedCheckDefine1();
public:
	afx_msg void OnBnClickedCheckDefine2();
public:
	afx_msg void OnBnClickedCheckDefine3();
};
