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
	
	int m_nGateOpenNum;		// բ�ſ��ĸ���
	int m_nFeatureOpenNum;	// ����ֵ���ĸ���
	BOOL m_bGateOpen[4];
	CFeatureInfo m_GateFeature;	// բ������ֵ
private:	
	CBitmap m_bmpBack;			// ����ͼ
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


