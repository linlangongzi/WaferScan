#pragma once

class CLine : public CObject
{
public:
	CLine(void);
public:
	~CLine(void);
public:
	CPoint m_ptLeftPoint;   //直线左端点
	CPoint m_ptRightPoint;  //直线右端点
	CRgn  m_LineRgn;    //直线区域
	CRgn  m_LeftPointRgn;  //左端点区域
	CRgn  m_RightPointRgn;  //右端点区域
	BOOL m_bSelectPos; // 鼠标选中提示： 0为未选中，1为选中
	COLORREF m_LineClr;  //直线颜色
public:
	void Move(CDC* pDC,int x,int y);  //鼠标拖动
	void LeftExtend(CDC* pDC,int x);  //左延伸
	void RightExtend(CDC* pDC,int x);  //右延伸
	void GetRgn(CRgn* rgn,CPoint pt1,CPoint pt2); //获取区域值
	void SaveLine(int nLeftPointX,int nLeftPointY,int nRightPointX,int nRightPointY,COLORREF m_clrLineClr);  //获取保存直线、直线的端点、直线的颜色、直线的区域、两端点的区域


};
