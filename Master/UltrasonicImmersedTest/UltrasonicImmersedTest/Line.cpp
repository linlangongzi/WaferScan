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
* �������ƣ�
*   Move()
* ����:
*   CDC* pDC - �豸�����Ķ�������ָ�� 
*   int x	- ������ƫ����
*   int y   - ������ƫ����
* ����ֵ:
*  void
* ˵��:
*   ʵ��ֱ���϶�����
* ��ע��
*   ��Ҫ�϶���ֱ�ߣ�����ǰ������� SetROP2(R2_NOTXORPEN)
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
	//�ػ�һ�㣬����ԭ����ֱ��
	pDC->SetROP2(R2_NOTXORPEN);   
	pDC->MoveTo(m_ptLeftPoint);   
	pDC->LineTo(m_ptRightPoint);

	m_ptLeftPoint.x += x;
	m_ptLeftPoint.y += y;
	m_ptRightPoint.x += x;
	m_ptRightPoint.y += y;

	//���µ���
	pDC->SetROP2(R2_NOTXORPEN);   
	pDC->MoveTo(m_ptLeftPoint);
	pDC->LineTo(m_ptRightPoint);

	//��ȡ�µ�ֱ�ߣ��˵�����
	GetRgn(&m_LineRgn,m_ptLeftPoint,m_ptRightPoint);
	GetRgn(&m_LeftPointRgn,m_ptLeftPoint,m_ptLeftPoint);
	GetRgn(&m_RightPointRgn,m_ptRightPoint,m_ptRightPoint);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CLine::Move"),"Leave");
}


/*************************************************************************
* �������ƣ�
*   LeftExtend()
* ����:
*   CDC* pDC - �豸�����Ķ�������ָ�� 
*   int x	- ֱ����˵������ƫ����
* ����ֵ:
*  void
* ˵��:
*   ʵ��ֱ�������������
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

	//xֵ���������ִ�����ֹ���ֵ����ĵ�
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
* �������ƣ�
*   RightExtend()
* ����:
*   CDC* pDC - �豸�����Ķ�������ָ�� 
*   int x	- ֱ���Ҷ˵������ƫ����
* ����ֵ:
*  void
* ˵��:
*   ʵ��ֱ���Ҷ���������
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


	//xֵ���������ִ�����ֹ���ֵ����ĵ�
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
* �������ƣ�
*   GetRgn()
* ����:
*   CPoint pt1 - ֱ����˵�
*   CPoint pt2 - ֱ���Ҷ˵�
* ����ֵ:
*  void
* ˵��:
*   ʵ�ֻ�ȡHRGN���ܣ�������PtInRegion()�������ж�����Ƿ���������
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
* �������ƣ�
*   SaveLine()
* ����:
*   int nLeftPointX - ֱ����˵������ֵX
*   int nLeftPointY - ֱ����˵�������ֵY
*   int nRightPointX - ֱ���Ҷ˵������ֵX
*   int nRightPointY - ֱ���Ҷ˵������ֵY
*   COLORREF m_LineClr - ֱ����ɫ
* ����ֵ:
*  void
* ˵��:
*   ��ȡ����ֱ�ߵĶ˵㡢ֱ�ߵ���ɫ��ֱ�ߵ��������˵������
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