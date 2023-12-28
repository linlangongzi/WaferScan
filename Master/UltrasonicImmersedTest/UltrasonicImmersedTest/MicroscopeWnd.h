/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 显微镜放大窗口头文件
* Filename : MicroscopeWnd.h
*
* @author       Song Chenguang
* @version      1.0         Date: 2011-09-13 
*/
#pragma once


// CMicroscopeWnd

class CMicroscopeWnd : public CWnd
{
	DECLARE_DYNAMIC(CMicroscopeWnd)

public:
	CMicroscopeWnd();
	virtual ~CMicroscopeWnd();
private:
	CBitmap m_bmpBack;			// 背景图	
	CBitmap* m_pOldBmp;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnDestroy();
};


