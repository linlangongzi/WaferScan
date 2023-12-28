// MechanicalControl.h : 实现文件
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 机械控制工具栏头文件
*Filename : MechanicalControl.h
*
*@author       zhaoli
*@version      1.0         Date: 2011-7-10 
*/

#pragma once


// CMechanicalControl
class CMechanicalControlPanel;

class CMechanicalControl : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CMechanicalControl)

public:
	CMechanicalControl();
	virtual ~CMechanicalControl();
	CCreateContext* m_pContext;
	CMechanicalControlPanel* m_pMechanicalControlView;
public:
	void SetContext (CCreateContext* pContext)
	{
		m_pContext = pContext;
	}
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


