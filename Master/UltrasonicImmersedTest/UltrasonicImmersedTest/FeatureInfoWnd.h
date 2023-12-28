#pragma once


// CFeatureInfoWnd

class CFeatureInfoWnd : public CWnd
{
	DECLARE_DYNAMIC(CFeatureInfoWnd)

public:
	CFeatureInfoWnd();
	virtual ~CFeatureInfoWnd();
private:
	void DrawInfo(CDC* pDC);
	int CountGateOpenNum();
	int CountFeatureOpenNum();
public:
	BOOL m_bMPFTimeDisplay;
	BOOL m_bMPMTimeDisplay;
	BOOL m_bMPMAltitudeDisplay;
	BOOL m_bFPFTimeDisplay;
	BOOL m_bFPMTimeDisplay;
	BOOL m_bFPMAltitudeDisplay;
	
	int m_nGateOpenNum;		// 闸门开的个数
	int m_nFeatureOpenNum;	// 特征值开的个数
	BOOL m_bGateOpen[4];
	CFeatureInfo m_GateFeature;	// 闸门特征值
private:	
	CBitmap m_bmpBack;			// 背景图
	CBitmap* m_pOldBmp;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
//Machao add start on 2013-07-1
	afx_msg void OnClose();
//Machao add end on 2013-07-1
	LRESULT OnReceiveFeatureInfo(WPARAM wParam, LPARAM lParam);
};


