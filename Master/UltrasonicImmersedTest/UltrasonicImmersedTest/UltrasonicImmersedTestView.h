// UltrasonicImmersedTestView.h : CUltrasonicImmersedTestView ��Ľӿ�
//
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* Aɨ��ͼ������ʾͷ�ļ�
*Filename : CUltrasonicImmersedTestView.h
*
*@author       zhaoli
*@version      1.0         Date: 2011-7-1 
*/
#pragma once
class CChannelPanel;
class CRulerWnd;
#include "Line.h"

// �����ϵ�����Ϣ
class CCurvePoint
{
public:
	float m_fTime;
	float m_fAmplitudeRatio;
	BOOL m_bEnable;
public:
	CCurvePoint():m_fTime(0.0f),m_fAmplitudeRatio(0.0f), m_bEnable(FALSE)
	{		
	}

	~CCurvePoint()
	{		
	}
};

class CFeatureInfoWnd;

class CUltrasonicImmersedTestView : public CView
{
protected: // �������л�����
	CUltrasonicImmersedTestView();
	DECLARE_DYNCREATE(CUltrasonicImmersedTestView)

// ����
public:
	CUltrasonicImmersedTestDoc* GetDocument() const;
public:
	CRect m_rectScaleX;
 	CRect m_rectScaleY;
 	CRect m_rectWave;	//������ʾ����
//	CFeatureInfoWnd* m_pFeatureWnd;	// ��������
private:
//	CCurvePoint	m_DACPonits[CHANNEL_NUM][DAC_CURVE_POINT_NUM];
	BOOL m_bMoifyDAC;
	BOOL m_bDrawDAC;
	
	BOOL m_bFeatureWndDisplayed;
	CH_FLAWDATA_CSCAN  m_FeatureInfoData;
// ����
private:
	void DrawAScan(CDC* pDC, CRect& rect);
	void DistributeRegion(const CRect& rectAll, 
		                  CRect& rectXScale,
						  CRect& rectYScale,
						  CRect& rectWave);
	void DrawWave(CDC* pDC, const CRect& rect);
	// �����
	// direction 1, _|_,  2,|-, 3Ϊ1������, 4Ϊ2������
	void DrawScale(CDC* pDC, const CRect& rect, float fStart, float fEnd, float fStep, int direction, BOOL bReverse);
	void DrawGrid(CDC* pDC, const CRect& rect);

	void DisplayAScanGate(CDC* pDC,const CRect& rect,CGateInfo& GateInfo,const int nGateNum, const int& nIndex);// ��ʾAɨբ��  yachang.huang modify start on 2012-6-19,���nGateNum բ����
	void DrawWidthLine(CDC* pDC,CPoint ptOrigin,CPoint ptEnd);

	void FillSpace(CDC* pDC,CRect& rect);
	void DrawRegularLine(CDC* pDC,CPoint ptOrigin,CPoint ptEnd,int nSegmentNum); //����֮�仭����ΪnSegmentNum�ĺڰ׼���߶�    yachang.huang add on 2012-7-25 
// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	LRESULT OnRecieveAScanData(WPARAM wParam, LPARAM lParam);

// ʵ��
public:
	virtual ~CUltrasonicImmersedTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	
public:
	float m_fXMinRuler;		// �̶ȳ���Сֵ������С����
	float m_fXMaxRuler;		// �̶ȳ����ֵ�����������	
	CGateInfo* m_pGateInfo;	// Aɨբ�����������ָ��

    //yachang.huang add start on 2012-6-19  
	CLine* m_pGateLine;  //  բ��������ָ��    
	CLine* m_pGateLeftLine; //  բ�������������ָ��    
	CLine* m_pGateRightLine; //  բ�����ұ�������ָ��   
	int m_nGateLineSelected;  //ѡ�е�ֱ�߱�ţ�-1Ϊδѡ��    
	int m_nGateLineRectSelected; //�����ʾС���ε�ֱ�߱�� 
	CPoint m_StartPoint;      //��굥����ȡ�ƶ�ԭʼ��
	BOOL m_bLineMove;  //ֱ���Ƿ������ƶ�   
	BOOL m_bLeftExtend; //ֱ���Ƿ�����������    yachang.huang add on 2012-7-25 
	BOOL m_bRightExtend; //ֱ���Ƿ�����������   yachang.huang add on 2012-7-25 
	int m_nDrawNum; 
	//yachang.huang add end on 2012-6-19  
	CPicData m_stData;	
	CToolTipCtrl m_ToolTip;	// ��ʱ�˷���ʾ��
	CToolTipCtrl m_WidthToolTip;	// �����ʾ��
	CPoint m_ptOrigin;		// ���������ʼ��
	CPoint m_ptEnd;			// ��������յ�
	CChannelPanel* m_pChannelPanel; //ͨ����崰��ָ��  yachang.huang add start on 2012-7-2 

    //yachang.huang add start on 2012-7-24 
    CPoint m_ptXScalePoint;  //���������X������λ�ã������ж��Ƿ�ı��ӳٻ�Χ��ֵ
	BOOL m_bChangeZeroPoint;   //�Ƿ�ı��ӳ���ֵ  
	BOOL m_bChangeRange;   //�Ƿ�ı䷶Χ��ֵ 
	//yachang.huang add end on 2012-7-24  
	CRulerWnd *m_pHRuler;  //yachang.huang add on 2012-8-8   ˮƽ���
	CRulerWnd *m_pVRuler;  //yachang.huang add on 2012-8-8   ��ֱ���
//	int m_nGateNum;			//��ǰբ������
public:
	virtual BOOL DestroyWindow();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	DAC_CURVE_t m_stDacCurve[CHANNEL_NUM];	// DAC����
	//BOOL m_bDacChangeFlag;				// DAC����λ���Ƿ�ı��־
	COLORREF m_colorDac[DAC_CURVE_NUM];		// DAC������ɫ
	BOOL m_bMakeDacFlag;					//�Ƿ������������޸�DAC����
//	BOOL m_bMakeTcgFlag;					//�Ƿ������������޸�TCG����
	int m_nSelectPoint;						//  ѡ�е�DAC��
public:
	void DrawDacCurve(CDC* pDC, CRect rect);
	void drawCurve(CDC* pDC, CRect rect, CURVEPOINT_DATA_t stCurvePointData[], int nDataNum, COLORREF color);
	POINT CurveDataToPoint(CURVEPOINT_DATA_t stCurvePointData, CRect rect);
	void DrawSelectCross(CDC* pDC, CRect rect,CURVEPOINT_DATA_t stCurvePointData[], int nSelectPoint);
	void DrawAllCross(CDC* pDC, CRect rect, CURVEPOINT_DATA_t stCurvePointData[], WORD nDataNum);
	void DrawTcgCurve(CDC* pDC, CRect rect);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // UltrasonicImmersedTestView.cpp �еĵ��԰汾
inline CUltrasonicImmersedTestDoc* CUltrasonicImmersedTestView::GetDocument() const
   { return reinterpret_cast<CUltrasonicImmersedTestDoc*>(m_pDocument); }
#endif

