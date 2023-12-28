/* @author       yachang.huang
* @version      1.0         Date: 2012-06-12 
*/


#include "stdafx.h"
#include "Line.h"
#include "UltrasonicImmersedTest.h" //wangbingfu add at 2012-11-15

CLine::CLine(void)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::CLine"),"Enter");

	m_ptLeftPoint = 0;
	m_ptRightPoint = 0;
	m_LineRgn.CreateRectRgn(0,0,0,0);
	m_LeftPointRgn.CreateRectRgn(0,0,0,0);
	m_RightPointRgn.CreateRectRgn(0,0,0,0);
	m_bSelectPos = FALSE;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::CLine"),"Leave");
}

CLine::~CLine(void)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::~CLine"),"Enter");

	m_LineRgn.DeleteObject();
	m_LeftPointRgn.DeleteObject();
	m_RightPointRgn.DeleteObject();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::~CLine"),"Leave");
}


/*************************************************************************
* 函数名称：
*   Move()
* 参数:
*   CDC* pDC - 设备上下文对象的类的指针 
*   int x	- 横坐标偏移量
*   int y   - 纵坐标偏移量
* 返回值:
*  void
* 说明:
*   实现直线拖动功能
* 另注：
*   将要拖动的直线，画线前建议调用 SetROP2(R2_NOTXORPEN)
************************************************************************/
void CLine::Move(CDC* pDC,int x,int y)
{
	// WangQianfeng add start on 2012-11-02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::Move"),"Enter");

	if ( NULL == pDC )
	{
		AfxMessageBox(_T("CLine::Move,pDC = null"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CLine::Move"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	//重画一便，擦除原来的直线
	pDC->SetROP2(R2_NOTXORPEN);   
	pDC->MoveTo(m_ptLeftPoint);   
	pDC->LineTo(m_ptRightPoint);

	m_ptLeftPoint.x += x;
	m_ptLeftPoint.y += y;
	m_ptRightPoint.x += x;
	m_ptRightPoint.y += y;

	//画新的线
	pDC->SetROP2(R2_NOTXORPEN);   
	pDC->MoveTo(m_ptLeftPoint);
	pDC->LineTo(m_ptRightPoint);

	//获取新的直线，端点区域
	GetRgn(&m_LineRgn,m_ptLeftPoint,m_ptRightPoint);
	GetRgn(&m_LeftPointRgn,m_ptLeftPoint,m_ptLeftPoint);
	GetRgn(&m_RightPointRgn,m_ptRightPoint,m_ptRightPoint);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::Move"),"Leave");
}


/*************************************************************************
* 函数名称：
*   LeftExtend()
* 参数:
*   CDC* pDC - 设备上下文对象的类的指针 
*   int x	- 直线左端点横坐标偏移量
* 返回值:
*  void
* 说明:
*   实现直线左端伸缩功能
************************************************************************/
void CLine::LeftExtend(CDC* pDC,int x)
{
	// WangQianfeng add start on 2012-11-02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::LeftExtend"),"Enter");

	if ( NULL == pDC )
	{
		AfxMessageBox(_T("CLine::LeftExtend,pDC = null"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CLine::LeftExtend"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	pDC->SetROP2(R2_NOTXORPEN);  
	CPoint temp = m_ptLeftPoint;
	m_ptLeftPoint.x += x;

	//x值做正负区分处理，防止出现单独的点
	if (x > 0)
	{
		pDC->MoveTo(temp);
		pDC->LineTo(m_ptLeftPoint);
	}
	if(x < 0)
	{
		pDC->MoveTo(m_ptLeftPoint);
		pDC->LineTo(temp);
	}
	GetRgn(&m_LineRgn,m_ptLeftPoint,m_ptRightPoint);
	GetRgn(&m_LeftPointRgn,m_ptLeftPoint,m_ptLeftPoint);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::LeftExtend"),"Leave");
}


/*************************************************************************
* 函数名称：
*   RightExtend()
* 参数:
*   CDC* pDC - 设备上下文对象的类的指针 
*   int x	- 直线右端点横坐标偏移量
* 返回值:
*  void
* 说明:
*   实现直线右端伸缩功能
************************************************************************/
void CLine::RightExtend(CDC* pDC,int x)
{
	// WangQianfeng add start on 2012-11-02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::RightExtend"),"Enter");

	if ( NULL == pDC )
	{
		AfxMessageBox(_T("CLine::RightExtend,pDC = null"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CLine::RightExtend"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	pDC->SetROP2(R2_NOTXORPEN); 
	CPoint temp = m_ptRightPoint;
	m_ptRightPoint.x += x;


	//x值做正负区分处理，防止出现单独的点
	if (x > 0)
	{
		pDC->MoveTo(temp);
		pDC->LineTo(m_ptRightPoint);
	}
	if (x < 0)
	{
		pDC->MoveTo(m_ptRightPoint);
		pDC->LineTo(temp);
	}
	GetRgn(&m_LineRgn,m_ptLeftPoint,m_ptRightPoint);
	GetRgn(&m_RightPointRgn,m_ptRightPoint,m_ptRightPoint);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::RightExtend"),"Leave");
}



/*************************************************************************
* 函数名称：
*   GetRgn()
* 参数:
*   CPoint pt1 - 直线左端点
*   CPoint pt2 - 直线右端点
* 返回值:
*  void
* 说明:
*   实现获取HRGN功能，方便用PtInRegion()函数来判定落点是否在区域内
************************************************************************/
void CLine::GetRgn(CRgn* rgn,CPoint pt1,CPoint pt2)
{
	// WangQianfeng add start on 2012-11-02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::GetRgn"),"Enter");

	if ( NULL == rgn )
	{
		AfxMessageBox(_T("CLine::GetRgn,rgn = null"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CLine::GetRgn"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	rgn->SetRectRgn(pt1.x-5,pt1.y+5,pt2.x+5,pt2.y-5);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::GetRgn"),"Leave");
}

/*************************************************************************
* 函数名称：
*   SaveLine()
* 参数:
*   int nLeftPointX - 直线左端点横坐标值X
*   int nLeftPointY - 直线左端点纵坐标值Y
*   int nRightPointX - 直线右端点横坐标值X
*   int nRightPointY - 直线右端点横坐标值Y
*   COLORREF m_LineClr - 直线颜色
* 返回值:
*  void
* 说明:
*   获取保存直线的端点、直线的颜色、直线的区域、两端点的区域
************************************************************************/
void CLine::SaveLine(int nLeftPointX,int nLeftPointY,int nRightPointX,int nRightPointY,COLORREF m_clrLineClr)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::SaveLine"),"Enter");

	m_ptLeftPoint.x = nLeftPointX;
	m_ptLeftPoint.y = nLeftPointY;
	m_ptRightPoint.x = nRightPointX;
	m_ptRightPoint.y = nRightPointY;
	m_LineClr = m_clrLineClr;
	GetRgn(&m_LineRgn,m_ptLeftPoint,m_ptRightPoint);
	GetRgn(&m_LeftPointRgn,m_ptLeftPoint,m_ptLeftPoint);
	GetRgn(&m_RightPointRgn,m_ptRightPoint,m_ptRightPoint);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::SaveLine"),"Leave");
}