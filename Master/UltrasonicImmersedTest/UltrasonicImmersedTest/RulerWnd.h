#if !defined(AFX_RULERWND_H__9D8CA79F_99BA_4075_A9CD_5FA8B55E26B8__INCLUDED_)
#define AFX_RULERWND_H__9D8CA79F_99BA_4075_A9CD_5FA8B55E26B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000
#pragma once
class CUltrasonicImmersedTestView;
class CUltrasonicImmersedTestDoc;
class CMainFrame;
class CCScanView;
class CBScanView;
			 
#define RULERWINDOW_CLASSNAME  _T( "CRulerWnd" )


//Ruler Window Style
#define RWSTYLE_AVERT         0x0001
#define RWSTYLE_AHORZ         0x0002
#define RWSTYLE_LEFTALIGN     0x0004
#define RWSTYLE_RIGHTALIGN    0x0008
#define RWSTYLE_TOPALIGN      RWSTYLE_LEFTALIGN
#define RWSTYLE_BOTTOMALIGN   RWSTYLE_RIGHTALIGN
#define RWSTYLE_DRAWMODAL     0x0010
#define RWSTYLE_DRAWEDGE      0x0020   
#define RWSTYLE_CVERT         0x0040
#define RWSTYLE_CHORZ         0x0080
#define RWSTYLE_BVERT         0x0100
#define RWSTYLE_BHORZ         0x0200

				   
//Notify Message
#define  NM_RULER_NOTIFICATIONMESSAGE   0x1112
#define  NMSUB_RULER_SEPERATORCHANGE    0x0001
#define  NMSUB_RULER_SEPERATORCHANGING  0x0002

#define SEPTYPE_NOPOSCHANGE    0x0001

class CRulerWnd : public CWnd
{			 
public:
	CRulerWnd(); 
	BOOL Create( DWORD dwStyle , const RECT& rect , CWnd* pParentWnd = NULL, UINT nID = 0);  //
	virtual ~CRulerWnd();   
	int m_nObjectCount;   //C扫编号
public:
	CMainFrame* m_pFrame;                        //Frame指针
	CUltrasonicImmersedTestView* m_pAScanView;   //A扫指针
	// WangQianfeng delete start on 2013-06-28
	CBScanView* m_pBScanView;                    //B扫指针
	// WangQianfeng delete end on 2013-06-28
	CCScanView* m_pCScanView;                    //C扫指针

	DWORD    GetStyle() { return m_dwStyle; }                             //获取标尺类型
	COLORREF GetBackGroundColor() { return m_clrBackGround; }             //获取标尺背景颜色
	COLORREF GetMilimeterLineColor() { return m_clrMilimeterLineColor; }  //获取标尺刻度线颜色
	COLORREF GetTextColor() { return m_clrTextColor; }                    // 获取标尺刻度颜色
	BOOL     SetStyle( DWORD dwStyle );              //设置标尺类型
	BOOL     SetBackGroundColor( COLORREF clr );     //设置标尺背景颜色
	BOOL     SetMilimeterLineColor( COLORREF clr );  //设置标尺刻度线颜色
	BOOL     SetTextColor( COLORREF clr );           //设置标尺刻度颜色
protected:
    virtual void PreSubclassWindow();
	BOOL DrawHRuler(CDC *pDC,float fStart, float fEnd, float fStep, int direction, BOOL bReverse,int ScanNum);  //画横坐标
	BOOL DrawVRuler(CDC *pDC,float fStart, float fEnd, float fStep, int direction, BOOL bReverse,int ScanNum);  //画纵坐标
	DWORD     m_dwStyle;                //标尺类型
	COLORREF  m_clrBackGround;          //标尺背景颜色
	COLORREF  m_clrMilimeterLineColor;  //标尺刻度线颜色
	COLORREF  m_clrTextColor;           //标尺刻度颜色
	BOOL RegisterWindowClass();         //注册窗口类
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);//控件刷新函数
	DECLARE_MESSAGE_MAP()
public:
	int m_nCStep;  //C扫刻度尺步长
	int m_nAStep;  //A扫刻度尺步长
};


#endif // !defined(AFX_RULERWND_H__9D8CA79F_99BA_4075_A9CD_5FA8B55E26B8__INCLUDED_)
