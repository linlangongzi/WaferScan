#pragma once
#include <vector>
using namespace std;
#include "Resource.h"

// CStaticDlg dialog

struct MCOLORR
{
	float fPercent;
	int nGradualColor;
};

class CStaticDlg : public CDialog
{
	DECLARE_DYNAMIC(CStaticDlg)

public:
	CStaticDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStaticDlg();

// Dialog Data
	enum { IDD = IDD_STATIC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void OnSize(UINT nType, int cx, int cy);
	BOOL OnInitDialog();
	BOOL PreTranslateMessage(MSG* pMsg);
	void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
private:
	int m_nCurHeight;
//	CRect m_rcOriginalRect;
	int	m_nScrollPos;
//	BOOL bScroll;

	COLORREF m_clrTop;	// static������ɫ
	COLORREF m_clrEnd;	// static�ײ���ɫ
public:
	int	m_nSite;	// �ڵڼ���Edit�����ɫ
	int m_nRectNum;	// ��ɫ���ĸ���
//	int m_nAddNum;	// ���n����ɫ
	vector<HWND> m_vEdit;
	vector<HWND> m_vGraduateEdit;
	vector<HWND> m_vStatic;
	vector<COLORREF> m_vColor;
	vector<MCOLORR> m_vmClrMain;
	vector<float> m_vfPercent;
	void AddItem(const int& n);
	void LayOutControl();
	void LayOutControlGraduate();
	void LayOutControlStatic();
	void DeleteLayoutControl();
	void OpenLayoutControl();
	void DeleteItem();
	void ChangeColor();
	void OnEnableButton();
	void OnMainColorNum(float fAddTop, float fAddBottom);
	BOOL OnIntoClr(float fPer);
	void OnDeleteMainColor(float fDelete, int nNum);
	void InitializeMColor();
//	void ChangeEditValue();	// machao add on 2013-09-2
	int m_nChangeEditNum;

	HWND m_hFocusEdit;
};
