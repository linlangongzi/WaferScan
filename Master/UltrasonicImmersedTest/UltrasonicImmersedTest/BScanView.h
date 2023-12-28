#pragma once


// CBScanView view
#include <vector>
using namespace std;

class CRulerWnd;
class CBScanView : public CView
{
	DECLARE_DYNCREATE(CBScanView)
public:
	CRulerWnd *m_pHRuler;  //yachang.huang add on 2012-8-15   水平标尺
	CRulerWnd *m_pVRuler;  //yachang.huang add on 2012-8-15   竖直标尺

protected:
	CBScanView();           // protected constructor used by dynamic creation
	virtual ~CBScanView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
protected:
	void DrawBScan(CDC* pDC, CRect& rect);
	void DistributeRegion(const CRect& rectAll, 
		CRect& rectXScale,
		CRect& rectYScale,
		CRect& rectWave);
	void DrawScale(CDC* pDC, 
		const CRect& rect, 
		float fStart, 
		float fEnd, 
		float fStep, 
		int direction, 
		BOOL bReverse);
	//void DrawBScan(CDC* pDC, const CRect& rect);//B扫画图函数
	LRESULT ReceiveBScanData(WPARAM wParam, LPARAM lParam);

	void DrawBScanPicture(CDC* pDC,const CRect& rect,CFeatureInfo& featureInfo);

	void FillSpace(CDC* pDC,CRect &rect);
public:
	CRect m_BScanScaleX;//x坐标轴区域
	CRect m_BScanScaleY;//y坐标轴区域
	CRect m_BScanArea;//B扫成像区域

	float m_fXMinScale;
	float m_fXMaxScale;

	float m_fGongJianHeight;

	
	float m_fGongJianLong;
	
	vector<CFeatureInfo> m_vfeature;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


