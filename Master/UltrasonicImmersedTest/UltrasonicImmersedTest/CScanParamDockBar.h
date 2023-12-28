// CScanParamDockBar .h : 实现文件
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* C扫参数面板ControlBar工具栏头文件
*Filename : CScanParamDockBar.h
*
*@author       zhaoli
*@version      1.0         Date: 2011-7-27 
*/

#pragma once


// CCScanParamDockBar
class CCScanParamPanel;
class CBScanParamPanel;
class CCScanParamDockBar : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CCScanParamDockBar)

public:
	CCScanParamDockBar();
	virtual ~CCScanParamDockBar();

protected:
	DECLARE_MESSAGE_MAP()
private:
	CCreateContext* m_pContext;
public:
	CCScanParamPanel* m_pCScanParamView;
	CCScanParamPanel* m_pCScanParamView2;
	CBScanParamPanel* m_pBScanParamView;//CBScanView类中引用了下面的变量如果删除，则要改CBScanView这个类
public:
	void SetContext (CCreateContext* pContext)
	{
		m_pContext = pContext;
	}

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnChangeActiveTab(WPARAM wp, LPARAM lp);	// machao add on 2013-09-3
public:
	CBCGPTabWnd	m_wndTab;
};


