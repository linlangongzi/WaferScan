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
	int m_nObjectCount;   //Cɨ���
public:
	CMainFrame* m_pFrame;                        //Frameָ��
	CUltrasonicImmersedTestView* m_pAScanView;   //Aɨָ��
	// WangQianfeng delete start on 2013-06-28
	CBScanView* m_pBScanView;                    //Bɨָ��
	// WangQianfeng delete end on 2013-06-28
	CCScanView* m_pCScanView;                    //Cɨָ��

	DWORD    GetStyle() { return m_dwStyle; }                             //��ȡ�������
	COLORREF GetBackGroundColor() { return m_clrBackGround; }             //��ȡ��߱�����ɫ
	COLORREF GetMilimeterLineColor() { return m_clrMilimeterLineColor; }  //��ȡ��߿̶�����ɫ
	COLORREF GetTextColor() { return m_clrTextColor; }                    // ��ȡ��߿̶���ɫ
	BOOL     SetStyle( DWORD dwStyle );              //���ñ������
	BOOL     SetBackGroundColor( COLORREF clr );     //���ñ�߱�����ɫ
	BOOL     SetMilimeterLineColor( COLORREF clr );  //���ñ�߿̶�����ɫ
	BOOL     SetTextColor( COLORREF clr );           //���ñ�߿̶���ɫ
protected:
    virtual void PreSubclassWindow();
	BOOL DrawHRuler(CDC *pDC,float fStart, float fEnd, float fStep, int direction, BOOL bReverse,int ScanNum);  //��������
	BOOL DrawVRuler(CDC *pDC,float fStart, float fEnd, float fStep, int direction, BOOL bReverse,int ScanNum);  //��������
	DWORD     m_dwStyle;                //�������
	COLORREF  m_clrBackGround;          //��߱�����ɫ
	COLORREF  m_clrMilimeterLineColor;  //��߿̶�����ɫ
	COLORREF  m_clrTextColor;           //��߿̶���ɫ
	BOOL RegisterWindowClass();         //ע�ᴰ����
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);//�ؼ�ˢ�º���
	DECLARE_MESSAGE_MAP()
public:
	int m_nCStep;  //Cɨ�̶ȳ߲���
	int m_nAStep;  //Aɨ�̶ȳ߲���
};


#endif // !defined(AFX_RULERWND_H__9D8CA79F_99BA_4075_A9CD_5FA8B55E26B8__INCLUDED_)
