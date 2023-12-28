//MovePathView.h : 头文件
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* 扫查架运动轨迹视图头文件
*Filename : MovePathView.h 
*
*@author       songchenguang zhaoli
*@version      1.0         Date: 2011-7-10
*/


#ifndef _MOVE_PATH_VIEW_H_
#define _MOVE_PATH_VIEW_H_
#include "mdsocx.h"


#pragma once

// CMovePathView form view
class CDGraphMove;
class CMechanicalControlPanel;

class CMovePathView : public CFormView
{
	DECLARE_DYNCREATE(CMovePathView)

protected:
	CMovePathView();           // protected constructor used by dynamic creation
	virtual ~CMovePathView();

public:
	enum { IDD = IDD_MOVEPATHVIEW };
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
//	CDGraphMove m_GraphMove;
	
public:
	afx_msg void OnEditUndo();
public:
	afx_msg void OnEditRedo();
public:
	afx_msg void OnEditSelectAll();
public:
	afx_msg void OnEditFind();
public:
	afx_msg void OnEditShai();
public:
	afx_msg void OnEditCut();
public:
	afx_msg void OnEditCopy();
public:
	afx_msg void OnEditPaste();
public:
	afx_msg void OnEditClear();
public:
	afx_msg void OnEditFan();
public:
	afx_msg void OnEditMirror();
public:
	afx_msg void OnEditRota();
public:
	afx_msg void OnDrawDao();
public:
	afx_msg void OnDrawSelect();
public:
	afx_msg void OnDrawHole();
public:
	afx_msg void OnDrawLine();
public:
	afx_msg void OnDrawArc();
public:
	afx_msg void OnDrawArcCenetr();
public:
	afx_msg void OnDrawCircle();
public:
	afx_msg void OnDrawCircleSan();
public:
	afx_msg void OnDrawArcline();
public:
	afx_msg void OnDrawRect();
public:
	afx_msg void OnDrawEllipse();
public:
	afx_msg void OnDrawPolygon();
public:
	afx_msg void OnDrawRunway();
public:
	afx_msg void OnToolCan();
public:
	afx_msg void OnToolColor();
public:
	afx_msg void OnToolJxian();
public:
	afx_msg void OnToolJpian();
public:
	afx_msg void OnToolEdit();
public:
	afx_msg void OnToolReset();
public:
	afx_msg void OnToolRun();
public:
	afx_msg void OnViewShi();
public:
	afx_msg void OnViewTu();
public:
	afx_msg void OnViewWindow();
public:
	//afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnDecompose();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileNew();
public:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnIpConfig();
public:
	DECLARE_EVENTSINK_MAP()
public:
	void AxisSelectGraphmovectrl(long AxisIndex);
public:
	int m_nPanelIndex;	// 平面切换
public:
	CMdsOcx m_GraphMove;
public:
	void OnGraphEnd();
public:
	void OnPathStop(short p_flag);
public:
	CMechanicalControlPanel* m_ParentWnd;
	BOOL m_bULDPtp;		// 向上-水平-向下Ptp标志
	BOOL m_bESC;		//按下ESC键标志
public:
	void OnJump(short p_flag);
	void OnReadFileMdsocx();
};


#endif