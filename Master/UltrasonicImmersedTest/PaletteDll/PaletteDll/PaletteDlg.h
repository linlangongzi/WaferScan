#pragma once

#include "StaticDlg.h"
// CPaletteDlg dialog

struct PALETTE
{
	COLORREF color;
	float fPercent;
	int ncolorbar;
};

class CPaletteDlg : public CDialog
{
	DECLARE_DYNAMIC(CPaletteDlg)

public:
	CPaletteDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPaletteDlg();

// Dialog Data
	enum { IDD = IDD_PALETTE_DIALOG };

protected:
	HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
//	afx_msg BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
private:
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	int IsClickColorRect(POINT point);
	void InitDisplayRect(int nNum); // 初始显示矩形
	void InitDiscolorRect();		// 选择颜色
	afx_msg void OnBnClickedButton1();	// 添加
	BOOL ChangeBackground();	// 改变background颜色
	BOOL ChangeAcquisition();	// 改变Acquisition颜色
	void OnLButtonDown(UINT nFlags, CPoint point);
	void SpecificColor();	// 画背景色底色
	BOOL DrawVRuler(CDC *pDC,CRect rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection,int ndB);	//画刻度
	BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu);
private:
	COLORREF m_color;	// 用户选择的颜色
	int m_nNumRect;		// 所画颜色矩形的多少
	CDC* m_pDC;
//	CStatic test;
	CRect rc;

	int m_nMaxAdd;		// 指定颜色最大值
public:
	BOOL m_bPlatte;
	int m_nMainColorNum;	// 主颜色数
	COLORREF m_clrBackground;	// background 颜色
	COLORREF m_clrBottom;		// Acquisition 颜色
	CStaticDlg *m_pStaticDlg;
	vector<int> m_vnyColorbar;	// 颜色条Y轴大小
	CToolBar m_WndToolBar;
	CFont fontAmp;			// AMP字体

	BOOL m_bMenuInsert;		// 标题栏添加按钮是否可用
	BOOL m_bMenuClear;		// 标题栏删除按钮是否可用
// 	BOOL m_bToolbarAdd;		// 工具栏添加按钮是否可用
// 	BOOL m_bToolbarDelete;	// 工具栏删除按钮是否可用

	void OnSequence();		// 主颜色nGradualColor值排序
	void OnCountGradualColor();

	afx_msg void OnBnClickedButton2();		// 删除
	afx_msg void ChangeColorRect(int nIndex);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileQuit();
	afx_msg void OnFileDefine();
	afx_msg void OnUpdateLevelInsert(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLevelClear(CCmdUI *pCmdUI);
// 	afx_msg void OnUpdateBttonAdd(CCmdUI *pCmdUI);
// 	afx_msg void OnUpdateBttonDelete(CCmdUI *pCmdUI);
};
