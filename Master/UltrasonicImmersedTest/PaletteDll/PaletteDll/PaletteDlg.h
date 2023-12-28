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
	void InitDisplayRect(int nNum); // ��ʼ��ʾ����
	void InitDiscolorRect();		// ѡ����ɫ
	afx_msg void OnBnClickedButton1();	// ���
	BOOL ChangeBackground();	// �ı�background��ɫ
	BOOL ChangeAcquisition();	// �ı�Acquisition��ɫ
	void OnLButtonDown(UINT nFlags, CPoint point);
	void SpecificColor();	// ������ɫ��ɫ
	BOOL DrawVRuler(CDC *pDC,CRect rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection,int ndB);	//���̶�
	BOOL OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult);
	void OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu);
private:
	COLORREF m_color;	// �û�ѡ�����ɫ
	int m_nNumRect;		// ������ɫ���εĶ���
	CDC* m_pDC;
//	CStatic test;
	CRect rc;

	int m_nMaxAdd;		// ָ����ɫ���ֵ
public:
	BOOL m_bPlatte;
	int m_nMainColorNum;	// ����ɫ��
	COLORREF m_clrBackground;	// background ��ɫ
	COLORREF m_clrBottom;		// Acquisition ��ɫ
	CStaticDlg *m_pStaticDlg;
	vector<int> m_vnyColorbar;	// ��ɫ��Y���С
	CToolBar m_WndToolBar;
	CFont fontAmp;			// AMP����

	BOOL m_bMenuInsert;		// ��������Ӱ�ť�Ƿ����
	BOOL m_bMenuClear;		// ������ɾ����ť�Ƿ����
// 	BOOL m_bToolbarAdd;		// ��������Ӱ�ť�Ƿ����
// 	BOOL m_bToolbarDelete;	// ������ɾ����ť�Ƿ����

	void OnSequence();		// ����ɫnGradualColorֵ����
	void OnCountGradualColor();

	afx_msg void OnBnClickedButton2();		// ɾ��
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
