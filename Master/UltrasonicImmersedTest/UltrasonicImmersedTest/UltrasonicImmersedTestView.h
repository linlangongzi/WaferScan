// UltrasonicImmersedTestView.h : CUltrasonicImmersedTestView 类的接口
//
/**
* Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* A扫视图波形显示头文件
*Filename : CUltrasonicImmersedTestView.h
*
*@author       zhaoli
*@version      1.0         Date: 2011-7-1 
*/
#pragma once
class CChannelPanel;
class CRulerWnd;
#include "Line.h"

// 曲线上单点信息
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
protected: // 仅从序列化创建
	CUltrasonicImmersedTestView();
	DECLARE_DYNCREATE(CUltrasonicImmersedTestView)

// 属性
public:
	CUltrasonicImmersedTestDoc* GetDocument() const;
public:
	CRect m_rectScaleX;
 	CRect m_rectScaleY;
 	CRect m_rectWave;	//波形显示区域
//	CFeatureInfoWnd* m_pFeatureWnd;	// 特征窗口
private:
//	CCurvePoint	m_DACPonits[CHANNEL_NUM][DAC_CURVE_POINT_NUM];
	BOOL m_bMoifyDAC;
	BOOL m_bDrawDAC;
	
	BOOL m_bFeatureWndDisplayed;
	CH_FLAWDATA_CSCAN  m_FeatureInfoData;
// 操作
private:
	void DrawAScan(CDC* pDC, CRect& rect);
	void DistributeRegion(const CRect& rectAll, 
		                  CRect& rectXScale,
						  CRect& rectYScale,
						  CRect& rectWave);
	void DrawWave(CDC* pDC, const CRect& rect);
	// 画标尺
	// direction 1, _|_,  2,|-, 3为1反方向, 4为2反方向
	void DrawScale(CDC* pDC, const CRect& rect, float fStart, float fEnd, float fStep, int direction, BOOL bReverse);
	void DrawGrid(CDC* pDC, const CRect& rect);

	void DisplayAScanGate(CDC* pDC,const CRect& rect,CGateInfo& GateInfo,const int nGateNum, const int& nIndex);// 显示A扫闸门  yachang.huang modify start on 2012-6-19,添加nGateNum 闸门线
	void DrawWidthLine(CDC* pDC,CPoint ptOrigin,CPoint ptEnd);

	void FillSpace(CDC* pDC,CRect& rect);
	void DrawRegularLine(CDC* pDC,CPoint ptOrigin,CPoint ptEnd,int nSegmentNum); //两点之间画数量为nSegmentNum的黑白间隔线段    yachang.huang add on 2012-7-25 
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	LRESULT OnRecieveAScanData(WPARAM wParam, LPARAM lParam);

// 实现
public:
	virtual ~CUltrasonicImmersedTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	
public:
	float m_fXMinRuler;		// 刻度尺最小值，即最小声程
	float m_fXMaxRuler;		// 刻度尺最大值，即最大声程	
	CGateInfo* m_pGateInfo;	// A扫闸门类对象数组指针

    //yachang.huang add start on 2012-6-19  
	CLine* m_pGateLine;  //  闸门线数组指针    
	CLine* m_pGateLeftLine; //  闸门线左边线数组指针    
	CLine* m_pGateRightLine; //  闸门线右边线数组指针   
	int m_nGateLineSelected;  //选中的直线编号，-1为未选中    
	int m_nGateLineRectSelected; //点击显示小矩形的直线编号 
	CPoint m_StartPoint;      //鼠标单击获取移动原始点
	BOOL m_bLineMove;  //直线是否允许移动   
	BOOL m_bLeftExtend; //直线是否允许左延伸    yachang.huang add on 2012-7-25 
	BOOL m_bRightExtend; //直线是否允许右延伸   yachang.huang add on 2012-7-25 
	int m_nDrawNum; 
	//yachang.huang add end on 2012-6-19  
	CPicData m_stData;	
	CToolTipCtrl m_ToolTip;	// 伤时伤幅提示框
	CToolTipCtrl m_WidthToolTip;	// 宽度提示框
	CPoint m_ptOrigin;		// 测量厚度起始点
	CPoint m_ptEnd;			// 测量厚度终点
	CChannelPanel* m_pChannelPanel; //通道面板窗体指针  yachang.huang add start on 2012-7-2 

    //yachang.huang add start on 2012-7-24 
    CPoint m_ptXScalePoint;  //保存鼠标点击X坐标轴位置，用于判断是否改变延迟或范围数值
	BOOL m_bChangeZeroPoint;   //是否改变延迟数值  
	BOOL m_bChangeRange;   //是否改变范围数值 
	//yachang.huang add end on 2012-7-24  
	CRulerWnd *m_pHRuler;  //yachang.huang add on 2012-8-8   水平标尺
	CRulerWnd *m_pVRuler;  //yachang.huang add on 2012-8-8   竖直标尺
//	int m_nGateNum;			//当前闸门索引
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
	DAC_CURVE_t m_stDacCurve[CHANNEL_NUM];	// DAC曲线
	//BOOL m_bDacChangeFlag;				// DAC曲线位置是否改变标志
	COLORREF m_colorDac[DAC_CURVE_NUM];		// DAC曲线颜色
	BOOL m_bMakeDacFlag;					//是否正在制作或修改DAC曲线
//	BOOL m_bMakeTcgFlag;					//是否正在制作或修改TCG曲线
	int m_nSelectPoint;						//  选中的DAC点
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

#ifndef _DEBUG  // UltrasonicImmersedTestView.cpp 中的调试版本
inline CUltrasonicImmersedTestDoc* CUltrasonicImmersedTestView::GetDocument() const
   { return reinterpret_cast<CUltrasonicImmersedTestDoc*>(m_pDocument); }
#endif

