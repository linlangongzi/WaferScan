// UltrasonicImmersedTestView.cpp : CUltrasonicImmersedTestView ���ʵ��

/**
*  Copyright(c)2011 Allrising (BeiJing) TechnoLogy Co.,Ltd,AllRights Reserved.
*
* Aɨ��ͼ������ʾԴ�ļ�
* Filename: UltrasonicImmersedTestView.cpp
* 
* @author       songchenguang  zhaoli
* @version      1.0         Date: 2011-07-1
*/

#include "stdafx.h"
#include "ChannelInfoDockView.h"
#include "CScanDlgBar.h"
#include "MechanicalControl.h"
#include "SysParam.h"
#include "MechanicalControlPanel.h"
#include "CScanParamDockBar.h"
#include "UltrasonicImmersedTest.h"
#include "GlobalFunction.h"
#include "UltrasonicImmersedTestDoc.h"
#include "UltrasonicImmersedTestView.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "ChannelPanel.h"

#include "MemDC.h"
#include "DataDefine.h"
#include "FeatureInfoWnd.h"
#include "DacCurveDlg.h"
#include "TcgCurveDlg.h"
#include "RulerWnd.h"
#include "math.h"
#include "TaskPanel.h"
#include "TaskDockView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CUltrasonicImmersedTestView
extern DWORD g_dwActiveChannel;
extern short g_nWaveSpeed;

IMPLEMENT_DYNCREATE(CUltrasonicImmersedTestView, CView)

BEGIN_MESSAGE_MAP(CUltrasonicImmersedTestView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)

	ON_MESSAGE(WM_A_SCAN_DATA, &CUltrasonicImmersedTestView::OnRecieveAScanData)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// CUltrasonicImmersedTestView ����/����

CUltrasonicImmersedTestView::CUltrasonicImmersedTestView()
{
	// TODO: �ڴ˴���ӹ������
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView"),"Enter");
	m_fXMinRuler = 0;
	m_fXMaxRuler = 100.0f;
	m_pGateInfo = new CGateInfo[4];
	m_pGateLine = new CLine[4];  //բ��������ָ��   yachang.huang add start on 2012-6-19  
	m_pGateLeftLine = new CLine[4]; //  բ�������������ָ��   yachang.huang add start on 2012-6-19  
	m_pGateRightLine = new CLine[4]; //  բ�����ұ�������ָ��   yachang.huang add start on 2012-6-19  
	// yachang.huang  add start on 2012-11-02
	if (m_pGateInfo == NULL)
	{
		MessageBox(_T("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView m_pGateInfo"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView"),"Leave");
		return;
	}
	if (m_pGateLine == NULL)
	{
		MessageBox(_T("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView m_pGateLine"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView"),"Leave");
		return;
	}
	if (m_pGateLeftLine == NULL)
	{
		MessageBox(_T("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView m_pGateLeftLine"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView"),"Leave");
		return;
	}
	if (m_pGateRightLine == NULL)
	{
		MessageBox(_T("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView m_pGateRightLine"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	//m_ToolTip = new CToolTipCtrl;
	m_ptOrigin.x = 0;
	m_ptOrigin.y = 0;
	m_ptEnd.x = 0;
	m_ptEnd.y = 0;

	m_bMoifyDAC = FALSE;
	m_bDrawDAC = FALSE;
	
	::ZeroMemory(m_stDacCurve, sizeof(DAC_CURVE_t) * CHANNEL_NUM);
// 	m_stDacCurve[0].bSwitchFlag = TRUE;
// 	m_stDacCurve[0].nPointNum = 2;
// 	m_stDacCurve[0].fFirstGain = 30;
// 	m_stDacCurve[0].stPointData[0][0].fData = 50;
// 	m_stDacCurve[0].stPointData[0][0].fRange = 15;
// 	m_stDacCurve[0].stPointData[0][1].fData = 25;
// 	m_stDacCurve[0].stPointData[0][1].fRange = 30;
	m_colorDac[0] = RGB(119, 190, 93);
	m_colorDac[1] = RGB(221, 19, 123);
	m_colorDac[2] = RGB(0, 147, 221);
	m_colorDac[3] = RGB(255, 0, 0);
	m_bMakeDacFlag = FALSE;
	m_nSelectPoint = 0;
    //yachang.huang add start on 2012-6-19  
	m_StartPoint.x = 0;
	m_StartPoint.y = 0;
	m_nGateLineSelected = -1; 
	m_nGateLineRectSelected = -1;
	m_bLineMove = FALSE;
	m_bLeftExtend = FALSE;
	m_bRightExtend = FALSE;
	m_nDrawNum = 1;
	//yachang.huang add end on 2012-6-19  
    //yachang.huang add start on 2012-7-24  
	m_ptXScalePoint = 0;
	m_bChangeZeroPoint = FALSE;
	m_bChangeRange = FALSE;
    //yachang.huang add end on 2012-7-24  
	m_pHRuler = new CRulerWnd; //yachang.huang add on 2012-8-8
	m_pVRuler = new CRulerWnd; //yachang.huang add on 2012-8-8
	// yachang.huang  add start on 2012-11-02
	if (m_pHRuler == NULL)
	{
		MessageBox(_T("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView m_pHRuler"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView"),"Leave");
		return;
	}
	if (m_pVRuler == NULL)
	{
		MessageBox(_T("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView m_pVRuler"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView"),"Leave");
		return;
	}
    // yachang.huang  add end on 2012-11-02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::CUltrasonicImmersedTestView"),"Leave");

}

CUltrasonicImmersedTestView::~CUltrasonicImmersedTestView()
{
	//delete m_ToolTip;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::~CUltrasonicImmersedTestView"),"Enter");
   	//yachang.huang add start on 2012-8-8 
	if (m_pHRuler != NULL)
	{
		delete m_pHRuler;
		m_pHRuler = NULL;
	}
	if (m_pVRuler != NULL)
	{
		delete m_pVRuler;
		m_pVRuler = NULL;
	}

	//yachang.huang add end on 2012-8-8
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::~CUltrasonicImmersedTestView"),"Leave");
}

BOOL CUltrasonicImmersedTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::PreCreateWindow"),"Enter");
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::PreCreateWindow"),"Leave");
    return CView::PreCreateWindow(cs);

}

// CUltrasonicImmersedTestView ����

void CUltrasonicImmersedTestView::OnDraw(CDC* pDC)
{
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnDraw"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T(" CUltrasonicImmersedTestView::OnDraw pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnDraw"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	CUltrasonicImmersedTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnDraw"),"Leave");
		return;
	}

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	// ��Aɨ�貨��
	CRect rect;
	GetClientRect(&rect);
	DrawAScan(pDC, rect);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnDraw"),"Leave");
}


// CUltrasonicImmersedTestView ��ӡ

BOOL CUltrasonicImmersedTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnPreparePrinting"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pInfo == NULL)
	{
		MessageBox(_T(" CUltrasonicImmersedTestView::OnPreparePrinting pInfo"), _T("����"), MB_OK|MB_ICONERROR);
        LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnPreparePrinting"),"Leave");
		return FALSE;
	}
	// yachang.huang  add end on 2012-11-02
	// Ĭ��׼��
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnPreparePrinting"),"Leave");
	return DoPreparePrinting(pInfo);
}

void CUltrasonicImmersedTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CUltrasonicImmersedTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CUltrasonicImmersedTestView ���

#ifdef _DEBUG
void CUltrasonicImmersedTestView::AssertValid() const
{
	CView::AssertValid();
}

void CUltrasonicImmersedTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUltrasonicImmersedTestDoc* CUltrasonicImmersedTestView::GetDocument() const // �ǵ��԰汾��������
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::GetDocument"),"Enter");
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUltrasonicImmersedTestDoc)));
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::GetDocument"),"Leave");
	return (CUltrasonicImmersedTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CUltrasonicImmersedTestView ��Ϣ�������
/** 
* ��Aɨ��ͼ��Ա����(������������/������/������)
* @param     pDC [in]: �豸��������ָ�� 
* @param     rect[in]: Aɨ�ͻ�����ͼ����                                                         
* @return    void
* @since     1.0
*/
void CUltrasonicImmersedTestView::DrawAScan(CDC* pDC, CRect& rect)
{
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawAScan"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T(" CUltrasonicImmersedTestView::DrawAScan pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawAScan"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
 	// ��������߼���������
  	DistributeRegion(rect, m_rectScaleX, m_rectScaleY, m_rectWave);
	// �������(�˴����޸�)
//	float fStep = (m_fXMaxRuler - m_fXMinRuler)/10.0;//�̶ȳߵĲ���
    m_pHRuler->MoveWindow(&m_rectScaleX);     //yachang.huang add on 2012-8-8   
	m_pVRuler->MoveWindow(&m_rectScaleY);     //yachang.huang add on 2012-8-8 
// 	DrawScale(pDC, m_rectScaleX, m_fXMinRuler, m_fXMaxRuler, fStep, 3, TRUE);
// 	
/* 	DrawScale(pDC, m_rectScaleY, 0, 100, 10, 4, TRUE);*/
	
	// ��������
 	CMemDC dcMem(pDC, &m_rectWave);
 
 	// ����������
 	DrawGrid(&dcMem, m_rectWave);
 
 	// ������
 	DrawWave(&dcMem, m_rectWave);
 	
	//բ����ʾ
	int j = 0;
	int m = 0;  //	yachang.huang add on 2012-7-25  ����Ƿ�����갴�µ�������֮�仭�ߵı�־����
	for (int i=3; i>=0; i--)   //yachang.huang modify on 2012-7-27  ���Ļ�բ����˳��I����ʾ�������棬�Դ�����
	{
		if(m_pGateInfo[i].m_bOpen)
		{
			DisplayAScanGate(&dcMem,m_rectWave,m_pGateInfo[i],i,j);
			j++;
		}
        //	yachang.huang add start on 2012-7-25   ������ߵ��յ���բ�����ڣ���m++
        if ( PtInRegion(m_pGateLine[i].m_LineRgn, m_ptEnd.x, m_ptEnd.y) )	
		{
			m++;
		}
		//	yachang.huang add end on 2012-7-25
	}

    //	yachang.huang add start on 2012-7-25   ������ߵ��յ㲻��բ�����ڣ�����    
	if (-1 == m_nGateLineRectSelected)
	{
		DrawWidthLine(&dcMem,m_ptOrigin,m_ptEnd);
	}
    //	yachang.huang add end on 2012-7-25

	DrawDacCurve(&dcMem, m_rectWave);
	DrawTcgCurve(&dcMem, m_rectWave);
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_pDacCurveDlg->DisplayDefectInfo(&dcMem, m_rectWave);

	//��������̶ȳ�֮��Ŀհ�
	//FillSpace(pDC,CRect(rect.left,rect.bottom-CSCAN_SCALE_Y_LENGTH,CSCAN_SCALE_X_LENGTH,rect.bottom));  //yachang.huang modify on 2012-8-10   �����ô˺����� ��Aɨ��

	pDC->BitBlt(m_rectWave.left, m_rectWave.top, m_rectWave.Width(), m_rectWave.Height(),
 		&dcMem, 0, 0, SRCCOPY);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawAScan"),"Leave");
}
//
/** 
* ����̶��Լ���������
* @param     rectAll    [in]: Aɨ�ͻ�����ͼ����
* @param     rectXScale [out]: X��̶ȳ�����
* @param	 rectYScale [out]: Y��̶ȳ�����
* @param     rectWave   [in]: ������ʾ����                                             
* @return    void
* @since     1.0
*/
void CUltrasonicImmersedTestView::DistributeRegion(const CRect& rectAll, 
					  CRect& rectXScale,
					  CRect& rectYScale,
					  CRect& rectWave)
{	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DistributeRegion"),"Enter");
	POINT XScaleTop,XScaleBottom;
	//XScaleTop.x = 1.0/20.0*rectAll.Width();
	XScaleTop.x = 0;     //yachang.huang modify on 2012-8-8  �޸ĺ�����߳���
	//XScaleTop.y = rectAll.bottom-1.0/10.0*rectAll.Height();
	XScaleTop.y = rectAll.bottom - CSCAN_SCALE_Y_LENGTH;
	XScaleBottom.x = rectAll.right;
	XScaleBottom.y = rectAll.bottom;
	//x��������
	rectXScale.SetRect(XScaleTop,XScaleBottom);
	
	POINT YScaleTop,YScaleBottom;
	YScaleTop.x = rectAll.left;
	YScaleTop.y = rectAll.top;
	//YScaleBottom.x = 1.0/20.0*rectAll.Width();
	YScaleBottom.x = CSCAN_SCALE_X_LENGTH;
	//YScaleBottom.y = rectAll.bottom-1.0/10.0*rectAll.Height();
	YScaleBottom.y = rectAll.bottom - CSCAN_SCALE_Y_LENGTH;

	//y��������
	rectYScale.SetRect(YScaleTop,YScaleBottom);
	
	POINT WaveTop,WaveBottom;
	//WaveTop.x = 1.0/20.0*rectAll.Width();
	WaveTop.x = CSCAN_SCALE_X_LENGTH;
	WaveTop.y = rectAll.top;
	WaveBottom.x = rectAll.right;
	//WaveBottom.y = rectAll.bottom - 1.0/10.0*rectAll.Height();
	WaveBottom.y = rectAll.bottom - CSCAN_SCALE_Y_LENGTH;
	//������ʾ����
	rectWave.SetRect(WaveTop,WaveBottom);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DistributeRegion"),"Leave");


}
//
/** 
* ��Aɨ����ͼ
* @param     pDC  [in]: ��ͼ�豸��������ָ�� 
* @param     rect [in]: Aɨ������ʾ����                              
* @return    void
* @since     1.0
*/
void CUltrasonicImmersedTestView::DrawWave(CDC* pDC, const CRect& rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawWave"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T(" CUltrasonicImmersedTestView::DrawWave pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawWave"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	CPen pen(PS_SOLID,1,RGB(255,255,0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	int nSize = MONITOR_DATA_SIZE/AD_SIZE;
	int height = rect.Height() - 1;
	float yb = rect.Height() / MAX_HEIGHT * WAVE_ZOOM_SCALE;
	float xb = rect.Width()/float(nSize);
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	float fRange = pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo[g_dwActiveChannel].m_fChannelRange;
	
	if(fRange < 15.0 / (ADFREQUENCY / 100))
	{
		int nNum = 0;
		//nNum = WORD( fRange / 15.0 / (ADFREQUENCY / 100) * DATA_SIZE);
		nNum = fRange*2/1000.0/pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo[g_dwActiveChannel].m_nWaveSpeed*ADFREQUENCY*1000000.0;
		if(nNum != 0)
		{
			xb = rect.Width() / float(nNum);
		}		
	}
	//��ͼ��512����ĵ�һ�����ֵ���ɵڶ����ֵ  yachang.huang modify on 2014-06-26
	m_stData.m_wData[0] = m_stData.m_wData[1];
	pDC->MoveTo(0, static_cast<int>(height - m_stData.m_wData[0]*yb));
	for (int i=0; i < nSize; i++)
	{
		if (m_stData.m_wData[i] > MAX_HEIGHT) 
			m_stData.m_wData[i] = static_cast<WORD>(MAX_HEIGHT);
		
		pDC->LineTo(static_cast<int>(i*xb), static_cast<int>(height - m_stData.m_wData[i]*yb));
	}

	pDC->SelectObject(pOldPen);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawWave"),"Leave");

}


/** 
* ��Aɨ��ͼ����Y�̶ȱ��/����X�̶ȱ��
* @param     pDC       [in]: ��ͼ�豸����ָ��
* @param     rect      [in]: ����̶ȳ�����
* @param     nStart    [in]: ���̶�ֵ
* @param     nEnd      [in]:  ��С�̶�ֵ   
* @param     nStep     [in]: �̶Ȳ���
* @param     direction [in]: �̶ȳߵ�λ��
* @param     bReverse  [in]: ���̶ȳ߿���
* @return    void
* @since     1.0
*/
void CUltrasonicImmersedTestView::DrawScale(CDC* pDC, 
											const CRect& rect, 
											float fStart, 
											float fEnd, 
											float fStep, 
											int direction, 
											BOOL bReverse)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawScale"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T(" CUltrasonicImmersedTestView::DrawScale pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawScale"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	//	ASSERT(nStart != nEnd);

	if (fStep<1E-6)
	{
		// 		MessageBox(_T("����ֵ����!"), _T("����"), MB_OK|MB_ICONERROR);
		// 		return;
		fStep = 100.0;
	}
	pDC->SetBkMode(TRANSPARENT);

	//CBrush brush(RGB(0,192,0));
    CBrush brush(RGB(100,200,100));
	CBrush *pOldBrush = pDC->SelectObject(&brush);
	// ������
	pDC->Rectangle(rect);
	// ���ָ���
	int n = static_cast<int>((fEnd - fStart)/fStep);
	float fPerCoordinate = 0.0f;
	CString str;
	// ���±��
	if (direction == 1 || direction == 3)
	{
		fPerCoordinate = float(rect.Width())/float(fEnd - fStart);
		for (int i=0; i<=n; i++)
		{
			str.Format(_T("%.1f"), fStart+(fStep*i));
			// ������㣬�յ�
			CPoint ptStart, ptEnd;
			ptStart.x = rect.left + static_cast<LONG>(fPerCoordinate*fStep*i);
			ptEnd.x = ptStart.x;
			if (direction == 1)
			{
				ptStart.y = rect.bottom;
				ptEnd.y = rect.bottom - 8;

				pDC->SetTextAlign(TA_CENTER|TA_BOTTOM);
				pDC->TextOut(ptEnd.x, ptEnd.y, str);
			}
			else
			{
				ptStart.y = rect.top;
				ptEnd.y = rect.top + 8;

				pDC->SetTextAlign(TA_CENTER|TA_TOP);
				pDC->TextOut(ptEnd.x+20, ptEnd.y, str);//ptEnd.x����20����Ϊ��ʹ�̶ȿ���ƫ�ƣ�ʹ��һ���̶�ֵ�ܹ���ʾ������
			}

			pDC->MoveTo(ptStart);
			pDC->LineTo(ptEnd);
		}
	}
	// ���ұ��
	else if (direction == 2 || direction == 4)
	{
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);

		LONG lHalfHeight = tm.tmHeight/2;
		fPerCoordinate = float(rect.Height())/float(fEnd - fStart);
		for (int i=1; i<n; i++)
		{
			int nYData = static_cast<int>(fStart+(fStep*i));
			str.Format(_T("%d"), nYData);
			// ������㣬�յ�
			CPoint ptStart, ptEnd;
			if (!bReverse)
			{
				ptStart.y = rect.top + static_cast<LONG>(fPerCoordinate*fStep*i);
			}
			else
				ptStart.y = rect.bottom - static_cast<LONG>(fPerCoordinate*fStep*i);

			ptEnd.y = ptStart.y;
			if (direction == 2)
			{
				ptStart.x = rect.left;
				ptEnd.x = rect.left + 8;

				pDC->SetTextAlign(TA_LEFT);
				pDC->TextOut(ptEnd.x, ptEnd.y - lHalfHeight, str);
			}
			else
			{
				ptStart.x = rect.right;
				ptEnd.x = rect.right - 8;

				pDC->SetTextAlign(TA_RIGHT);
				pDC->TextOut(ptEnd.x, ptEnd.y - lHalfHeight, str);
			}

			pDC->MoveTo(ptStart);
			pDC->LineTo(ptEnd);
		}
	}
	else
	{
	}

	//�����������߽��㴦�Ŀհ״�����ɫ
// 	CRect rectbk;
// 	GetClientRect(&rectbk);
// 	pDC->FillRect(CRect(rectbk.left,rectbk.bottom-23,35,rectbk.bottom),&brush);


	pDC->SelectObject(pOldBrush);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawScale"),"Leave");
}
void CUltrasonicImmersedTestView::FillSpace(CDC* pDC,CRect& rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::FillSpace"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T(" CUltrasonicImmersedTestView::FillSpace pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::FillSpace"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	CBrush brush(RGB(0,192,0));
	pDC->FillRect(&rect,&brush);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(rect.left +13,rect.bottom-23,_T("Aɨ"));//yachang.huang modify on 2012-8-8  ������Aɨ����ʾλ��
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::FillSpace"),"Leave");
}
/** 
* ��Aɨ��ͼ������������
* @param     pDC  [in]: ��ͼ�豸����ָ��
* @param     rect [in]: Aɨ������ʾ����                                                    
* @return    void
* @since     1.0
*/
void CUltrasonicImmersedTestView::DrawGrid(CDC* pDC, const CRect& rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawGrid"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T(" CUltrasonicImmersedTestView::DrawGrid pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawGrid"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	CPen pen;

	pen.CreatePen(PS_SOLID,1,RGB(192,192,192));   //���Ļ�����ʽ  yachang.huang modify on 2012-7-26 
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->SetBkColor(TRANSPARENT);

	//�޸Ļ����񣬺ͺ�������ı仯����һ��  yachang.huang add start on 2012-11-22
	float fStep = static_cast<float>((m_fXMaxRuler - m_fXMinRuler)/10.0);
	if (fStep<1E-6)
	{
		fStep = 100.0;
	}
	int nStart = 0;
	int nEnd = 0;
	int nLinestart = 0;
	int nStep = 0;
	int iPos = 0;
	nStart = (int)m_fXMinRuler*10;
	nEnd = (int)(m_fXMinRuler +  fStep*10)*10;
	nStep = (nEnd - nStart)/100;
	nStep = (nStep/5 + 1)*5;

	nLinestart = (nStart/nStep + 1)*nStep; 
	if (nStart == 0)
	{
		nLinestart = 0;
	}
	//������������
	for (int i = nLinestart;i < nEnd; i+= nStep)
	{
		if (i % (nStep*10) == 0)
		{
			iPos = (rect.Width()*(i - nStart))/(nEnd - nStart);
			for(int j = 0;j<5;++j)
			{
				CPoint pt1;
				CPoint pt2;
				pt1.x = iPos;
				pt1.y = static_cast<LONG>(2.0/10.0*rect.Height()*j);
				pt2.x = iPos;
				pt2.y = static_cast<LONG>(2.0/10.0*rect.Height()*(j+1));
				DrawRegularLine(pDC,pt1,pt2,10);
			}
		}
	}
    //������������
	for(int i = 1;i<5;++i)
	{
		CPoint pt1;
		CPoint pt2;
		pt1.x = 0;
		pt1.y = static_cast<LONG>(2.0/10.0*rect.Height()*i);
		pt2.x = rect.Width();
		pt2.y = static_cast<LONG>(2.0/10.0*rect.Height()*i);
		DrawRegularLine(pDC,pt1,pt2,(nEnd - nStart)/nStep);

	}
	//  yachang.huang add end on 2012-11-22


	//  yachang.huang modify start on 2012-7-26
	//������������
// 	for(int i = 1;i<5;++i)
// 	{
// 		for(int j = 0;j< 10;++j)
// 		{
// 			CPoint pt1;
// 			CPoint pt2;
// 			pt1.x = static_cast<LONG>(1.0/10.0*rect.Width()*j);
// 			pt1.y = static_cast<LONG>(2.0/10.0*rect.Height()*i);
// 			pt2.x = static_cast<LONG>(1.0/10.0*rect.Width()*(j+1));
// 			pt2.y = static_cast<LONG>(2.0/10.0*rect.Height()*i);
// 			DrawRegularLine(pDC,pt1,pt2,10);
// 
// 		}
// 	}
	//������������
// 	for(int i = 1;i< 10;++i)
// 	{
// 	    for(int j = 0;j<5;++j)
// 		{
// 			CPoint pt1;
// 			CPoint pt2;
// 			pt1.x = static_cast<LONG>(1.0/10.0*rect.Width()*i);
// 			pt1.y = static_cast<LONG>(2.0/10.0*rect.Height()*j);
// 			pt2.x = static_cast<LONG>(1.0/10.0*rect.Width()*i);
// 			pt2.y = static_cast<LONG>(2.0/10.0*rect.Height()*(j+1));
// 			DrawRegularLine(pDC,pt1,pt2,10);
// 		}
// 	}
   //  yachang.huang modify end on 2012-7-26
	
	pDC->SelectObject(pOldPen);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawGrid"),"Leave");

}
/** 
* ���ղɼ�����
* @param     wParam [in]:  ���յ�������
* @param     lParam [in]:                                                   
* @return    void
* @since     1.0
*/
LRESULT CUltrasonicImmersedTestView::OnRecieveAScanData(WPARAM wParam, LPARAM lParam)
{
//	WORD* pData = (WORD*)wParam;
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnRecieveAScanData"),"Enter");
//	memcpy(m_stData.m_wData, pData, sizeof(WORD)*DATA_SIZE);
	// wangbingfu modify start at 2012-11-20
	CUltrasonicImmersedTestDoc* pDoc = GetDocument();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	CPicData * pData = (CPicData*)wParam;
	// Song Chenguang add start on 2013-11-26
	if (lParam != 0)
	{
		CH_FLAWDATA_CSCAN_S* pFlawData = (CH_FLAWDATA_CSCAN_S*)lParam;	
		CChannelInfo info = pFrame->GetChannelInfo(g_dwActiveChannel);
		int nDelayDot = MicroSecond2Dot(info.m_fChannelDelay);
		float fSyncRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, (pFlawData->wGate1SyncTime + nDelayDot));
		// Song Chenguang add end on 2013-11-26

		BOOL bTcgOnFlag = pFrame->m_pTcgCurveDlg->m_stTcgCurve[g_dwActiveChannel].bSwitchFlag;
		if(bTcgOnFlag)	// TCG��
		{
			if(pFrame->m_pTcgCurveDlg->m_nTcgOnMode == 0)	// ���������
			{
				float fRange = 0.0f;
				/*	for(int i = 0; i < DATA_SIZE; i++)
				{
				fRange = (i + 1) * (m_fXMaxRuler - m_fXMinRuler) / DATA_SIZE;
				pData->m_wData[i] = pFrame->m_pTcgCurveDlg->CalcNewAmp(pData->m_wData[i], fRange);
				}
				*/	 //������ CalcNewAmp()����������������512��ѭ������Щֻ��Ҫ����һ��

				TCG_CURVE_t stTcgCurve;
				memcpy(&stTcgCurve, &pFrame->m_pTcgCurveDlg->m_stTcgCurve[g_dwActiveChannel], sizeof(stTcgCurve));
				int nPointNum = stTcgCurve.nPointNum;
				CURVEPOINT_DATA_t stHeightestPoint = pFrame->m_pTcgCurveDlg->GetHeightestPoint(stTcgCurve.stPointData,
					nPointNum - 1);
				CURVEPOINT_DATA_t stFirstPoint = stTcgCurve.stPointData[0];
				CURVEPOINT_DATA_t stLastPoint = stTcgCurve.stPointData[nPointNum-1];
				float fFirstPointDeltaDB = 0;
				float fLastPointDeltaDB = 0;
				if(stFirstPoint.fData != 0)
				{
					fFirstPointDeltaDB = (float)(20 * log10((double)(stHeightestPoint.fData / stFirstPoint.fData)));
				}
				if(stLastPoint.fData != 0)
				{
					fLastPointDeltaDB = (float)(20 * log10((double)(stHeightestPoint.fData / stLastPoint.fData)));
				}

				//	float fHeightestData = stHeightestPoint.fData * MAX_HEIGHT / 100;
				for(int i = 0; i < DATA_SIZE; i++)
				{
					// Song Chenguang modfiy start on 2013-10-29
					//				fRange = (i + 1) * (m_fXMaxRuler - m_fXMinRuler) / DATA_SIZE;
					fRange = (i + 1) * (m_fXMaxRuler - m_fXMinRuler) / DATA_SIZE + m_fXMinRuler;
					// Song Chenguang modfiy end on 2013-10-29
					if(fRange < (stFirstPoint.fRange + fSyncRange))	// ��һ����֮ǰ	// Song Chenguang modify on 2013-11-26 
					{
						pData->m_wData[i] = pData->m_wData[i] * pow(10, fFirstPointDeltaDB / 20);
					}
					else if(fRange > (stLastPoint.fRange + fSyncRange))	// ���һ����֮�� // Song Chenguang modify on 2013-11-26
					{
						pData->m_wData[i] = pData->m_wData[i] * pow(10, fLastPointDeltaDB / 20);
					}
					else	// �м�
					{
						// Song Chenguang modify start on 2013-11-26
						int j = 0;
						float fRangeStart = 0.0f;
						float fRangeEnd = 0.0f;
						for(; j < nPointNum - 1; j++)
						{
							fRangeStart = stTcgCurve.stPointData[j].fRange + fSyncRange;
							fRangeEnd = stTcgCurve.stPointData[j+1].fRange + fSyncRange;
							if((fRange >= fRangeStart)&& (fRange <= fRangeEnd))
							{
								break;
							}
						}

						// ��ǰ���Ӧ�����ߵ��ֵ
						float fCurCurveAmp = stTcgCurve.stPointData[j].fData + (fRange - fRangeStart)
							/ (fRangeEnd - fRangeStart) 
							* (stTcgCurve.stPointData[j + 1].fData - stTcgCurve.stPointData[j].fData);					
						// Song Chenguang modify end on 2013-11-26
						if((fCurCurveAmp != 0) 
							&& (pData->m_wData[i] != 0))
						{
							float fDeltaDB = static_cast<float>(20 * log10(stHeightestPoint.fData / fCurCurveAmp));
							pData->m_wData[i] = pData->m_wData[i] * pow(10, fDeltaDB / 20);
							if(pData->m_wData[i] > MAX_HEIGHT)
							{
								pData->m_wData[i] = MAX_HEIGHT;
							}
						}						
					}
				}
			}
		}
		// Song Chenguang add start on 2013-11-26
		delete pFlawData;
		pFlawData = NULL;
		// Song Chenguang add end on 2013-11-26
	}

	m_stData = *pData;	
	// wangbingfu modify end at 2012-11-20
	InvalidateRect(&m_rectWave, FALSE);	
	// rem by Song Chenguang on 2013-12-19
/*
	if (pFrame->m_bSaveAScan)
	{
//		pDoc->m_lstAScanData[g_dwActiveChannel].AddTail(m_stData);	// rem by Song Chenguang on 2013-12-18
		pDoc->m_qAScanData[g_dwActiveChannel].push(m_stData);
	}
*/
	delete pData;
	pData = NULL;

    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnRecieveAScanData"),"Leave");
	return 0;
}
void CUltrasonicImmersedTestView::OnInitialUpdate()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnInitialUpdate"),"Enter");
	CView::OnInitialUpdate();

	m_ToolTip.Create(this,TTS_ALWAYSTIP);
	m_ToolTip.AddTool(this,LPSTR_TEXTCALLBACK);
	m_ToolTip.SetTipBkColor(RGB(0,255,0));
	m_ToolTip.SetDelayTime(2000);
	m_ToolTip.SetMaxTipWidth(200);

	m_WidthToolTip.Create(this,TTS_ALWAYSTIP);
	m_WidthToolTip.AddTool(this,LPSTR_TEXTCALLBACK);
	m_WidthToolTip.SetTipBkColor(RGB(255,0,255));
	m_WidthToolTip.SetDelayTime(2000);
	m_WidthToolTip.SetMaxTipWidth(200);

	// TODO: Add your specialized code here and/or call the base class
// 	m_pFeatureWnd = new CFeatureInfoWnd;
//  	m_pFeatureWnd->Create(NULL, _T("FeatureInfo"), WS_OVERLAPPEDWINDOW | WS_CHILD, CRect(0,0,300,300),
//  		this, ID_FEATURE_WND);
// //	m_pFeatureWnd->CreateEx(WS_EX_CLIENTEDGE, NULL, _T("FeatureInfo"), WS_OVERLAPPEDWINDOW, CRect(0,0,300,300),
// //		NULL, NULL, WS_EX_TOPMOST);
// 	m_bFeatureWndDisplayed = FALSE;
// 	m_pFeatureWnd->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnInitialUpdate"),"Leave");
}

void CUltrasonicImmersedTestView::OnSize(UINT nType, int cx, int cy)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnSize"),"Enter");
	CView::OnSize(nType, cx, cy);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnSize"),"Leave");

	// TODO: Add your message handler code here

}


/** 
* ��Aɨբ��
* @param     pDC [in]: ��ͼ�豸����ָ��
* @param     rect  [in]:բ����ʾ����
* @param     GateInfo [in]: բ�Ŷ�������
* @return    void
* @since     1.0
*/
void CUltrasonicImmersedTestView::DisplayAScanGate(CDC* pDC,const CRect& rect,CGateInfo& GateInfo,const int nGateNum, const int& nIndex)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DisplayAScanGate"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T(" CUltrasonicImmersedTestView::DisplayAScanGate pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DisplayAScanGate"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02

	if (GateInfo.m_bOpen)
	{	
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

		int nXStart = 0;
		int nXEnd = 0;
		int nHeight = static_cast<int>(rect.bottom - GateInfo.m_fPeakGate*(rect.Height()/100.0f));

		int nCount = 0;
		for (int i =0; i<4; i++)
		{
			if (m_pGateInfo[i].m_bOpen)
				nCount++;
		}

		if(GateInfo.m_nMtpMode == TRUE && pFrame->m_pFeatureWnd->m_GateFeature.m_nGateNum == nCount)
		{
			int nDotNum = int(pFrame->m_wndChannelInfo.m_pFormView->m_fRange * 2 / (pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed * 1000) * ADFREQUENCY * 1E6);
			int nBeiShu = nDotNum / 16384 + 1;

			float fSync = pFrame->m_pFeatureWnd->m_GateFeature.m_pfFeatures[0][0] * pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed / 1000.0f / 2.0f * nBeiShu;
			nXStart = int((GateInfo.m_fFrontGate - m_fXMinRuler + fSync)*(rect.Width()/(m_fXMaxRuler - m_fXMinRuler)));
			
			nXEnd = int(((GateInfo.m_fBackGate + GateInfo.m_fFrontGate) - m_fXMinRuler + fSync)*(rect.Width()/(m_fXMaxRuler - m_fXMinRuler)));

		}
		else
		{
			nXStart = int((GateInfo.m_fFrontGate - m_fXMinRuler)*(rect.Width()/(m_fXMaxRuler - m_fXMinRuler)));

			nXEnd = int(((GateInfo.m_fBackGate + GateInfo.m_fFrontGate) - m_fXMinRuler )*(rect.Width()/(m_fXMaxRuler - m_fXMinRuler)));
		}

		CPen penGate(PS_SOLID, 1, GateInfo.m_colorGate);
		CPen* pOldPen = pDC->SelectObject(&penGate);
		pDC->MoveTo(nXStart, nHeight);
		pDC->LineTo(nXEnd, nHeight);
		//yachang.huang add start on 2012-6-19  ��ȡֱ�ߵĶ˵㡢��ɫ���������˵�����򣬲������˵�Сֱ��
        m_pGateLine[nGateNum].SaveLine(nXStart,nHeight,nXEnd,nHeight,GateInfo.m_colorGate);
		
		m_pGateLeftLine[nGateNum].SaveLine(nXStart, nHeight + 10,nXStart,nHeight - 10,GateInfo.m_colorGate);
		pDC->MoveTo(m_pGateLeftLine[nGateNum].m_ptLeftPoint);
		pDC->LineTo(m_pGateLeftLine[nGateNum].m_ptRightPoint);

		m_pGateRightLine[nGateNum].SaveLine(nXEnd, nHeight + 10,nXEnd,nHeight - 10,GateInfo.m_colorGate);
		pDC->MoveTo(m_pGateRightLine[nGateNum].m_ptLeftPoint);
		pDC->LineTo(m_pGateRightLine[nGateNum].m_ptRightPoint);
	    //yachang.huang add end on 2012-6-19 
		pDC->SelectObject(pOldPen);
        //yachang.huang add start on 2012-7-2  ���ѡ����ʾ��  ���ⱻ�ػ��  
		if (m_nGateLineRectSelected != -1 && m_pGateInfo[m_nGateLineRectSelected].m_bOpen)  //yachang.huang modify on 2012-7-31  բ�Źرպ�������� 
		{
			//��ȡ֮ǰѡ�е�ֱ�ߵ���ɫ����������ֱ�ߵ�����С�ڵ㣬��ʾ���ѡ��
			CPen NewPen;
			NewPen.CreatePen(PS_SOLID, 1,m_pGateLine[m_nGateLineRectSelected].m_LineClr);
			CPen* pOldPen = pDC->SelectObject(&NewPen);

/*			pDC->SetROP2(R2_XORPEN); */  
			pDC->Rectangle(m_pGateLine[m_nGateLineRectSelected].m_ptLeftPoint.x - 3, m_pGateLine[m_nGateLineRectSelected].m_ptLeftPoint.y + 3,
				m_pGateLine[m_nGateLineRectSelected].m_ptLeftPoint.x + 3,m_pGateLine[m_nGateLineRectSelected].m_ptLeftPoint.y - 3);
			pDC->Rectangle(m_pGateLine[m_nGateLineRectSelected].m_ptRightPoint.x - 3, m_pGateLine[m_nGateLineRectSelected].m_ptRightPoint.y + 3,
				m_pGateLine[m_nGateLineRectSelected].m_ptRightPoint.x + 3,m_pGateLine[m_nGateLineRectSelected].m_ptRightPoint.y - 3);
			pDC->SelectObject(pOldPen);

		}
	    //yachang.huang add end on 2012-7-2 
	}
	else
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DisplayAScanGate"),"Leave");
		return;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DisplayAScanGate"),"Leave");
	
}

BOOL CUltrasonicImmersedTestView::DestroyWindow()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DestroyWindow"),"Enter");
	// TODO: Add your specialized code here and/or call the base class
	if(m_pGateInfo != NULL)
	{
		delete[] m_pGateInfo;
		m_pGateInfo = NULL;
	}
	//yachang.huang add start on 2012-6-19
	if (m_pGateLine != NULL)
	{
		delete[] m_pGateLine;
		m_pGateLine = NULL;
	}

	if (m_pGateLeftLine != NULL)
	{
		delete[] m_pGateLeftLine;
		m_pGateLeftLine = NULL;
	}

	if (m_pGateRightLine != NULL)
	{
		delete[] m_pGateRightLine;
		m_pGateRightLine = NULL;
	}
	//yachang.huang add start on 2012-6-19
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DestroyWindow"),"Leave");
	return CView::DestroyWindow();

}

BOOL CUltrasonicImmersedTestView::PreTranslateMessage(MSG* pMsg)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::PreTranslateMessage"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pMsg == NULL)
	{
		MessageBox(_T("CUltrasonicImmersedTestView::PreTranslateMessage(MSG* pMsg)"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::PreTranslateMessage"),"Leave");
		return FALSE;
	}
	// yachang.huang  add end on 2012-11-02
	// TODO: �ڴ����ר�ô����/����û���
	if (m_ToolTip.m_hWnd != NULL)
	{
		m_ToolTip.RelayEvent(pMsg);
	}
	if (m_WidthToolTip.m_hWnd != NULL)
	{
		m_WidthToolTip.RelayEvent(pMsg);
	}
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::PreTranslateMessage"),"Leave");
	return CView::PreTranslateMessage(pMsg);
}

/** 
* ��ʾ��ʱ�˷�tip
* @param     nFlags  
* @param     point  [out]:��굱ǰλ��
* @return    void
* @since     1.0
*/
void CUltrasonicImmersedTestView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

/*	ScreenToClient(&point); */    //yachang.huang modify on 2012-7-24  Ŀǰ���Ѿ��ǿͻ����꣬ȡ��ת��
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnMouseMove"),"Enter");
	if (point.x < m_rectScaleY.right)
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnMouseMove"),"Leave");
		return;
	}
	CString strTip;
	//�������λ�õ�����,��λmm
	float fSoundDistance = (m_fXMaxRuler - m_fXMinRuler)*(point.x - m_rectScaleY.right)/m_rectWave.Width()
							+ m_fXMinRuler;
	
	//�������λ�õĲɼ�������
	int n = int((fSoundDistance - m_fXMinRuler) / (m_fXMaxRuler - m_fXMinRuler) * (MONITOR_DATA_SIZE/AD_SIZE)) ;
	float fData = float(m_stData.m_wData[n]) / MAX_HEIGHT * 100.0f;
	strTip.Format(_T("���� = %.1f\n�˷�ֵ = %.1f"),fSoundDistance,fData);
	m_ToolTip.UpdateTipText(strTip,this);
    // yachang.huang add start on 2012-6-1
    /*ClientToScreen(&point);*/ //yachang.huang modify on 2012-7-2 �û�����ת������Ļ����
	point.x -= 45;  //yachang.huang modify on 2012-7-11  �޸��������ƫ�����������ڴ�DC�б���һ��
	CDC* pDC = GetDC();
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T(" CUltrasonicImmersedTestView::OnMouseMove GetDC()"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnMouseMove"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	CMemDC dcMem(pDC, &m_rectWave);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	if (m_nGateLineRectSelected != -1 && m_bChangeRange == FALSE && m_bChangeZeroPoint == FALSE && m_pGateInfo[m_nGateLineRectSelected].m_bOpen)//yachang.huang modify on 2012-7-31  բ�Źرպ�������� 
	{
		if (PtInRegion(m_pGateLine[m_nGateLineRectSelected].m_LeftPointRgn, point.x, point.y) ||
			PtInRegion(m_pGateLine[m_nGateLineRectSelected].m_RightPointRgn, point.x, point.y))
		{
			::SetCursor(::LoadCursor (NULL,IDC_SIZEWE)); 
		}
		//ֱ��������
		if ( TRUE == m_bLeftExtend )
		{
	        ::SetCursor(::LoadCursor (NULL,IDC_SIZEWE)); 
			if (m_nGateLineSelected != -1)
			{

				int x = point.x - m_pGateLine[m_nGateLineRectSelected].m_ptLeftPoint.x;
				if (point.x > 0)   //yachang.huang modify on 2012-7-11  ����ƽ�������жϣ���֤բ��ǰ�ش���0
				{
					m_pGateLine[m_nGateLineRectSelected].LeftExtend(&dcMem,x);
					m_pGateLeftLine[m_nGateLineRectSelected].Move(&dcMem,x,0);


				}

			}
		}
		//ֱ��������
		if ( TRUE == m_bRightExtend)
		{
			::SetCursor(::LoadCursor (NULL,IDC_SIZEWE)); 
			if (m_nGateLineSelected != -1)
			{

				int x = point.x - m_pGateLine[m_nGateLineRectSelected].m_ptRightPoint.x;
				m_pGateLine[m_nGateLineRectSelected].RightExtend(&dcMem,x);
				m_pGateRightLine[m_nGateLineRectSelected].Move(&dcMem,x,0);


			}
		}
		//ֱ���϶�
		else
		{

			if (m_nGateLineSelected != -1 && m_bLineMove == TRUE)
			{
                
				if ((m_pGateLine[m_nGateLineSelected].m_ptLeftPoint.x + point.x - m_StartPoint.x) > 0) //yachang.huang modify on 2012-7-11  ����ƽ�������жϣ���֤բ��ǰ�ش���0
				{
					m_pGateLine[m_nGateLineSelected].Move(&dcMem, point.x - m_StartPoint.x, point.y - m_StartPoint.y);
					m_pGateLeftLine[m_nGateLineRectSelected].Move(&dcMem,point.x - m_StartPoint.x, point.y - m_StartPoint.y);
					m_pGateRightLine[m_nGateLineRectSelected].Move(&dcMem,point.x - m_StartPoint.x, point.y - m_StartPoint.y);




					m_StartPoint = point;

				}



			}

		}
		if (m_nGateLineSelected != -1)   ////yachang.huang modify on 2012-7-24  �����жϣ�ȷ����ק������ʱ�Ŷ�բ����Ϣ���б༭
		{
			//int nHeight = m_rectWave.bottom - GateInfo.m_fPeakGate*(m_rectWave.Height()/100.0f);
			m_pGateInfo[m_nGateLineRectSelected].m_fPeakGate = double(m_rectWave.bottom - m_pGateLine[m_nGateLineRectSelected].m_ptLeftPoint.y)/(m_rectWave.Height()/100.0f);
			int nCount = 0;
			for (int i =0; i<4; i++)
			{
				if (m_pGateInfo[i].m_bOpen) 
					nCount++;
			}
			if(m_pGateInfo[m_nGateLineRectSelected].m_nMtpMode == TRUE && pFrame->m_pFeatureWnd->m_GateFeature.m_nGateNum == nCount)
			{
				int nDotNum = int(pFrame->m_wndChannelInfo.m_pFormView->m_fRange * 2 / (pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed * 1000) * ADFREQUENCY * 1E6);
				int nBeiShu = nDotNum / 16384 + 1;

				float fSync = pFrame->m_pFeatureWnd->m_GateFeature.m_pfFeatures[0][0] * pFrame->m_wndChannelInfo.m_pFormView->m_nWaveSpeed / 1000.0f / 2.0f * nBeiShu;

				m_pGateInfo[m_nGateLineRectSelected].m_fFrontGate = (double(m_pGateLine[m_nGateLineRectSelected].m_ptLeftPoint.x)/(m_rectWave.Width()/(m_fXMaxRuler - m_fXMinRuler)))
					- fSync + m_fXMinRuler;

				//nXEnd = int(((m_pGateInfo[m_nGateLineRectSelected].m_fBackGate + m_pGateInfo[m_nGateLineRectSelected].m_fFrontGate) - m_fXMinRuler + fSync)*(m_rectWave.Width()/(m_fXMaxRuler - m_fXMinRuler)));
				if (FALSE == m_bLineMove)
				{		
					m_pGateInfo[m_nGateLineRectSelected].m_fBackGate = (double(m_pGateLine[m_nGateLineRectSelected].m_ptRightPoint.x)/(m_rectWave.Width()/(m_fXMaxRuler - m_fXMinRuler))) - fSync + m_fXMinRuler -  m_pGateInfo[m_nGateLineRectSelected].m_fFrontGate;   
				}
			}
			else
			{
				//nXStart = int((m_pGateInfo[m_nGateLineRectSelected].m_fFrontGate - m_fXMinRuler)*(m_rectWave.Width()/(m_fXMaxRuler - m_fXMinRuler)));
				m_pGateInfo[m_nGateLineRectSelected].m_fFrontGate = (double(m_pGateLine[m_nGateLineRectSelected].m_ptLeftPoint.x)/(m_rectWave.Width()/(m_fXMaxRuler - m_fXMinRuler))) + m_fXMinRuler;

/*				nXEnd = int(((m_pGateInfo[m_nGateLineRectSelected].m_fBackGate + m_pGateInfo[m_nGateLineRectSelected].m_fFrontGate) - m_fXMinRuler )*(m_rectWave.Width()/(m_fXMaxRuler - m_fXMinRuler)));*/
				if (FALSE == m_bLineMove)
				{	
					m_pGateInfo[m_nGateLineRectSelected].m_fBackGate = double(double(m_pGateLine[m_nGateLineRectSelected].m_ptRightPoint.x/double(m_rectWave.Width()/(m_fXMaxRuler - m_fXMinRuler))) + m_fXMinRuler -  m_pGateInfo[m_nGateLineRectSelected].m_fFrontGate);
				}
			}

			/*		CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;*/
			for(int i=0; i<4; i++)
			{		
				pChannelPanel->m_ChannelInfo[pChannelPanel->m_nChannelNo].m_GateInfo[i] = m_pGateInfo[i]; 
			}
		// ��������
		if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode > 3)
		{
			int nOtherChannelIndex =  pChannelPanel->CalGroupOtherIndex(pChannelPanel->m_nChannelNo);

			memcpy(&(pChannelPanel->m_ChannelInfo[nOtherChannelIndex]), &(pChannelPanel->m_ChannelInfo[pChannelPanel->m_nChannelNo]), sizeof(CChannelInfo));
//			memcpy(&pDoc->m_stAllCH_CONFIG[nOtherChannelIndex], &pDoc->m_stAllCH_CONFIG[pChannelPanel->m_nChannelNo], sizeof(CSCAN_ARS200FPGA_SETONE));
		}

			pChannelPanel->SetGateInfo();  //����բ�ſؼ���Ϣ
			pChannelPanel->ChannelInfoToData();   //ͨ������ת��Ϊ����λ�����͵�����
			pChannelPanel->UpdateData(FALSE);
		}
	}

	//yachang.huang add start on 2012-7-24 
	if (point.x > m_rectScaleX.left && point.x < m_rectScaleX.left + 150 && point.y > m_rectScaleX.top && point.y < m_rectScaleX.bottom)
	{
		   ::SetCursor(::LoadCursor (NULL,IDC_HAND));
		   if (TRUE == m_bChangeZeroPoint)
           {
			   if (point.x - m_ptXScalePoint.x < 0)
			   {
				   pChannelPanel->m_fZeroPoint -= 0.1;
				   m_ptXScalePoint = point;
				   pChannelPanel->m_sliderZeroPoint.SetPos(int(pChannelPanel->m_fZeroPoint * 10));
			   }
				if(point.x - m_ptXScalePoint.x > 0)
			   {
				   pChannelPanel->m_fZeroPoint += 0.1;
				   m_ptXScalePoint = point;
				   pChannelPanel->m_sliderZeroPoint.SetPos(int(pChannelPanel->m_fZeroPoint * 10));
			   }
			   pChannelPanel->DoModifyZeroPoint();
           }
	}
	if (point.x > m_rectScaleX.right - 150  && point.x < m_rectScaleX.right && point.y > m_rectScaleX.top && point.y < m_rectScaleX.bottom)
	{
		::SetCursor(::LoadCursor (NULL,IDC_HAND));	
			if (TRUE == m_bChangeRange)
			{
			    ::SetCursor(::LoadCursor (NULL,IDC_HAND));
				if (point.x - m_ptXScalePoint.x < 0)
				{
					pChannelPanel->m_fRange -= 0.5;
					m_ptXScalePoint = point;
				    pChannelPanel->m_sliderRange.SetPos(int(pChannelPanel->m_fRange * 10));
				}
				if(point.x - m_ptXScalePoint.x > 0)
				{
					pChannelPanel->m_fRange += 0.5;
					m_ptXScalePoint = point;
				    pChannelPanel->m_sliderRange.SetPos(int(pChannelPanel->m_fRange * 10));
				}
				pChannelPanel->DoModifyRange();
			}
	}
	//yachang.huang add  end on 2012-7-24  
    ReleaseDC(pDC);
	 //yachang.huang add start on 2012-6-19
	CView::OnMouseMove(nFlags, point);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnMouseMove"),"Leave");
}
/** 
* ��갴����Ϣ��Ӧ
* @param     nFlags  
* @param     point  [out]:��굱ǰλ��
* @return    void
* @since     1.0
*/
void CUltrasonicImmersedTestView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnLButtonDown"),"Enter");
	m_ptOrigin = point;
	m_ptEnd = point;
	
	//yachang.huang add start on 2012-7-24 
	if (point.x > m_rectScaleX.left && point.x < m_rectScaleX.left + 150 && point.y > m_rectScaleX.top && point.y < m_rectScaleX.bottom)
	{
			m_ptXScalePoint = point;
			m_bChangeZeroPoint = TRUE;
	}
	else if (point.x > m_rectScaleX.right - 150  && point.x < m_rectScaleX.right && point.y > m_rectScaleX.top && point.y < m_rectScaleX.bottom)
	{
			m_ptXScalePoint = point;
			m_bChangeRange = TRUE;
	}
	else
	{

	}
    //yachang.huang add  end on 2012-7-24  
	//yachang.huang add start on 2012-6-19  
	CDC* pDC = GetDC();
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T(" CUltrasonicImmersedTestView::OnLButtonDown GetDC()"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnLButtonDown"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	point.x -= 45; //yachang.huang modify on 2012-7-11  �޸��������ƫ�����������ڴ�DC�б���һ��
	CMemDC dcMem(pDC, &m_rectWave);
    m_nGateLineRectSelected = -1;
	for(int i = 0; i < 4; i++)
	{
		if ( PtInRegion(m_pGateLine[i].m_LineRgn, point.x, point.y) )	

		{

			m_bLineMove = TRUE;

			m_StartPoint = point;  //  ��ȡ�ƶ�ԭʼ���
			m_nGateLineSelected = i; //  ��굥��δ�ͷ�ѡ�е�ֱ�߱��
			m_nGateLineRectSelected = i; //  ��ʾѡ�е�ֱ�߱��


			//�����굥����ֱ�����Ҷ˵�����ֻ���������죬�����϶�
            //yachang.huang modify start on 2012-7-25  �������������ж�
			if (PtInRegion(m_pGateLine[m_nGateLineRectSelected].m_LeftPointRgn, point.x, point.y))
			{
				m_bLineMove = FALSE;
				m_bRightExtend = FALSE;
				m_bLeftExtend = TRUE;
			}
			if (PtInRegion(m_pGateLine[m_nGateLineRectSelected].m_RightPointRgn, point.x, point.y))
			{
				m_bLineMove = FALSE;
				m_bLeftExtend = FALSE;
			    m_bRightExtend = TRUE;
			}
            //yachang.huang modify end on 2012-7-25  
			break;
		}

	}
    //yachang.huang add  end on 2012-6-19  
	dcMem->BitBlt(m_rectWave.left, m_rectWave.top, m_rectWave.Width(), m_rectWave.Height(),
		&dcMem, 0, 0, SRCCOPY);

    ReleaseDC(pDC);
	CView::OnLButtonDown(nFlags, point);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnLButtonDown"),"Leave");
}
/** 
* ��굯����Ϣ��Ӧ����tip��ʾ����֮�����̲�ֵ
* @param     nFlags  
* @param     point  [out]:��굱ǰλ��
* @return    void
* @since     1.0
*/
void CUltrasonicImmersedTestView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnLButtonUp"),"Enter");
	m_ptEnd = point;//���յ㸺ֵ�����ݳ�Ա

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame->m_bAScanIsOpen)//��Aɨ�򿪵�ʱ�򣬲ż��㲢��ʾTip
	{
		CString strWidth;
		//�������
		float fOriginDistance = (m_fXMaxRuler - m_fXMinRuler)*(m_ptOrigin.x-m_rectScaleY.Width())/m_rectWave.Width()
			+ m_fXMinRuler;
		//�յ������
		float fEndDistance = (m_fXMaxRuler - m_fXMinRuler)*(point.x-m_rectScaleY.Width())/m_rectWave.Width()
			+ m_fXMinRuler;
		//����֮��
		float temp = fEndDistance - fOriginDistance;
		if (m_ptOrigin != m_ptEnd)
		{
			strWidth.Format(_T("���̵Ĳ�ֵ��%.1f"),temp);
		}
		
		m_WidthToolTip.UpdateTipText(strWidth,this);
		m_ToolTip.EnableToolTips(FALSE);
	}
	if(m_nGateLineSelected >= 0)
	{
		m_nGateLineSelected = -1;  //  yachang.huang add start on 2012-6-19
		m_bLineMove = FALSE;  // yachang.huang add on 2012-7-25 
		m_bLeftExtend = FALSE;   // yachang.huang add on 2012-7-25 
		m_bRightExtend = FALSE;   //yachang.huang add on 2012-7-25 

		pFrame->m_bSetOne = TRUE;
	}
	m_bChangeZeroPoint = FALSE;   //  yachang.huang add on 2012-7-24
	m_bChangeRange = FALSE;    //  yachang.huang add on 2012-7-24

	CView::OnLButtonUp(nFlags, point);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnLButtonUp"),"Leave");
}
/** 
* ��Aɨ��ͼ��������갴�µ�������֮�仭��
* @param     pDC [in]�豸������  
* @param     ptOrigin  [in]:���
* @param     ptEnd  [in]:�յ�
* @return    void
* @since     1.0
*/
void CUltrasonicImmersedTestView::DrawWidthLine(CDC* pDC,CPoint ptOrigin,CPoint ptEnd)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawWidthLine"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T(" CUltrasonicImmersedTestView::DrawWidthLine pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawWidthLine"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	CPen pen(PS_SOLID,1,RGB(255,0,0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	ptOrigin.x -= m_rectScaleY.Width();
	ptEnd.x -= m_rectScaleY.Width();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (pFrame->m_bAScanIsOpen)//��Aɨ�򿪵�ʱ�򣬲ſ��Ի���
	{
		pDC->MoveTo(ptOrigin);
		pDC->LineTo(ptEnd);
	}
	pDC->SelectObject(pOldPen);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawWidthLine"),"Leave");
}

void CUltrasonicImmersedTestView::DrawDacCurve(CDC* pDC, CRect rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawDacCurve"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T("CUltrasonicImmersedTestView::DrawDacCurve pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawDacCurve"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
	int nCardNum = pApp->GetCardNum();
	
	if (g_dwActiveChannel >= (nCardNum*2))
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawDacCurve"),"Leave");
		return;
	}

	int nChannelNum = g_dwActiveChannel;
	int nPointNum = m_stDacCurve[nChannelNum].nPointNum;
	float fTotalPlus = 0;
	fTotalPlus = GetTotalPlus(pChannelPanel->m_fPlus, pChannelPanel->m_fRefPlus, g_dwActiveChannel);
	float fChangeGain = fTotalPlus - m_stDacCurve[nChannelNum].fFirstGain;	
	CURVEPOINT_DATA_t stDacCurveTemp[DAC_CURVE_NUM][DAC_CURVE_POINT_NUM];
	memset(stDacCurveTemp, 0, sizeof(stDacCurveTemp));
	

	for(int i = 0; i < DAC_CURVE_NUM; i++)
	{
		for(int j = 0; j< nPointNum; j++)
		{
			stDacCurveTemp[i][j].fData = CurveDataChangeByGain(m_stDacCurve[nChannelNum].stPointData[i][j].fData, fChangeGain);
			float fSyncRange = pChannelPanel->Dot2RangeChannel(pChannelPanel->m_nWaveSpeed, static_cast<int>(pFrame->m_pFeatureWnd->m_GateFeature.m_pfFeatures[0][0] * ADFREQUENCY));// ͬ������
			stDacCurveTemp[i][j].fRange = m_stDacCurve[nChannelNum].stPointData[i][j].fRange + fSyncRange;
		}
	}
	if(m_bMakeDacFlag)
	{
		drawCurve(pDC, rect, stDacCurveTemp[3], nPointNum, m_colorDac[3]);
		DrawAllCross(pDC, rect, stDacCurveTemp[3], nPointNum);
		DrawSelectCross(pDC, rect, stDacCurveTemp[3], m_nSelectPoint);
	}
	else
	{
		//if(m_stDacCurve[g_dwActiveChannel].bSwitchFlag)
		if(pFrame->m_pDacCurveDlg->m_nDacStatus  == 0)	//����
		{
			if(nPointNum >= 2)
			{
				for(int i = 0; i < DAC_CURVE_NUM - 1; i++)	// �����߲���
				{
					drawCurve(pDC, rect, stDacCurveTemp[i], nPointNum, m_colorDac[i]);
				}
			}
		}
	}
//	pFrame->m_pDacCurveDlg->DisplayValue(pFrame->m_wndChannelInfo.m_pFormView->m_fPlus, 
//				pFrame->m_wndChannelInfo.m_pFormView->m_fRefPlus, g_dwActiveChannel);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawDacCurve"),"Leave");
}

void CUltrasonicImmersedTestView::DrawTcgCurve(CDC* pDC, CRect rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawTcgCurve"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T("CUltrasonicImmersedTestView::DrawTcgCurve pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawTcgCurve"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CTcgCurveDlg* pTcgCurveDlg = (CTcgCurveDlg*)pFrame->m_pTcgCurveDlg;
	CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;

	int nCardNum = pApp->GetCardNum();

	if (g_dwActiveChannel >= (nCardNum*2))
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawTcgCurve"),"Leave");
		return;
	}

	int nChannelNum = g_dwActiveChannel;
	int nPointNum = pTcgCurveDlg->m_stTcgCurve[nChannelNum].nPointNum;	
	float fTotalPlus = 0;
	fTotalPlus = GetTotalPlus(pChannelPanel->m_fPlus, pChannelPanel->m_fRefPlus, g_dwActiveChannel);
	float fChangeGain = fTotalPlus - pTcgCurveDlg->m_stTcgCurve[nChannelNum].fFirstGain;	
	CURVEPOINT_DATA_t stTcgCurveTemp[TCG_CURVE_POINT_NUM];
	memset(stTcgCurveTemp, 0, sizeof(stTcgCurveTemp));

	for(int i = 0; i < nPointNum; i++)
	{
		stTcgCurveTemp[i].fData = CurveDataChangeByGain(pTcgCurveDlg->m_stTcgCurve[nChannelNum].stPointData[i].fData, fChangeGain);
		float fSyncRange = pChannelPanel->Dot2RangeChannel(pChannelPanel->m_nWaveSpeed, static_cast<int>(pFrame->m_pFeatureWnd->m_GateFeature.m_pfFeatures[0][0] * ADFREQUENCY));// ͬ������
		//		stTcgCurveTemp[i].fRange = CurveRangeChangeByVelocity(pTcgCurveDlg->m_stTcgCurve[nChannelNum].stPointData[i].fRange,
		//										pTcgCurveDlg->m_stTcgCurve[nChannelNum].stPointData[i].fVelocity,pChannelPanel->m_nWaveSpeed);
		stTcgCurveTemp[i].fRange = pTcgCurveDlg->m_stTcgCurve[nChannelNum].stPointData[i].fRange + fSyncRange;
	}

	if(pTcgCurveDlg->m_bMakeTcgFlag)
	{
		drawCurve(pDC, rect, stTcgCurveTemp, nPointNum, pTcgCurveDlg->m_colorTcg);
		DrawAllCross(pDC, rect, stTcgCurveTemp, nPointNum);
		DrawSelectCross(pDC, rect, stTcgCurveTemp, pTcgCurveDlg->m_nSelectPoint);
	}
/*	else
	{
		if(pFrame->m_pTcgCurveDlg->m_bSwitchFlag  == 0)	//����
		{
			if(nPointNum >= 2)
			{
				drawCurve(pDC, rect, stTcgCurveTemp, nPointNum, pTcgCurveDlg->m_colorTcg);
			}
		}
	}*/
//	pFrame->m_pTcgCurveDlg->DisplayValue(pFrame->m_wndChannelInfo.m_pFormView->m_fPlus, 
//		pFrame->m_wndChannelInfo.m_pFormView->m_fRefPlus, g_dwActiveChannel);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawTcgCurve"),"Leave");
}

void CUltrasonicImmersedTestView::drawCurve(CDC* pDC, CRect rect, CURVEPOINT_DATA_t stCurvePointData[],
											int nDataNum, COLORREF color)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::drawCurve"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T("CUltrasonicImmersedTestView::drawCurve pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::drawCurve"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	POINT point;
	memset(&point, 0 ,sizeof(POINT));

	if(nDataNum < 1)
		return;
	else
	{
		// wangbingfu modify start on 2013-03-19
		CPen pen;
		CPen* pOldPen;
		if(g_dwActiveChannel == 0)
		{
			pen.CreatePen(PS_SOLID,1,color);
			pOldPen = pDC->SelectObject(&pen);
		}
		else
		{
			pen.CreatePen(PS_DOT,1,color);
			pOldPen = pDC->SelectObject(&pen);
		}		

		POINT ptData[TCG_CURVE_POINT_NUM];
		memset(ptData, 0, sizeof(ptData));
		int nX[TCG_CURVE_POINT_NUM];
		int nY[TCG_CURVE_POINT_NUM];
		memset(nX, 0, sizeof(nX));
		memset(nY, 0, sizeof(nY));

		for(int i = 0; i < nDataNum; i++)
		{
			ptData[i] = CurveDataToPoint(stCurvePointData[i], rect);
			nX[i] = ptData[i].x;
			nY[i] = ptData[i].y;
		}

		POINT ptStart = ptData[0];
		POINT ptEnd = ptData[nDataNum - 1];
		pDC->MoveTo(0,ptStart.y);
		pDC->LineTo(ptStart.x, ptStart.y);

		// Song Chenguang modify start on 2013-10-22
		// ��Ϊֱ��
/*		
		int y = 0;
		for(LONG x = ptStart.x; x < ptEnd.x; x++)
		{
			y = (int)Lagrange((float)x, nX, nY, nDataNum);
			pDC->LineTo(x, y);
		}
		pDC->LineTo(ptEnd.x, ptEnd.y);
		pDC->LineTo(rect.Width(), ptEnd.y);
*/
		for(int i=1; i<nDataNum; i++)
		{
			pDC->LineTo(ptData[i].x, ptData[i].y);
		}

		pDC->LineTo(rect.Width(), ptData[nDataNum - 1].y);
		// Song Chenguang modify end on 2013-10-22
		pen.DeleteObject();

		pDC->SelectObject(pOldPen);
		// wangbingfu modify start on 2013-03-19
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::drawCurve"),"Leave");
}
POINT CUltrasonicImmersedTestView::CurveDataToPoint(CURVEPOINT_DATA_t stCurvePointData, CRect rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::CurveDataToPoint"),"Enter");
	POINT point;
	memset(&point, 0 ,sizeof(POINT));
	
	float fYStep = rect.Height() / 100.0f;
	float fXStep = rect.Width() / (m_fXMaxRuler - m_fXMinRuler);
	LONG lX = 0;
	LONG lY = 0;

	lX = LONG((stCurvePointData.fRange - m_fXMinRuler) * fXStep);
	lY = LONG(rect.bottom - stCurvePointData.fData * fYStep);

	point.x = lX;
	point.y = lY;

    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::CurveDataToPoint"),"Leave");
	return point;
}
/*
CURVEPOINT_DATA_t CUltrasonicImmersedTestView::FindMaxAmpPoint()
{
	CURVEPOINT_DATA_t stMaxAmpPoint;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	float fGateFront = pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo[g_dwActiveChannel].m_GateInfo[0].m_fFrontGate;
	float fGateB = pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo[g_dwActiveChannel].m_GateInfo[0].m_fBackGate;
	
	memset(&stMaxAmpPoint, 0, sizeof(CURVEPOINT_DATA_t));
	int nStartSize;	//���λ��
	int nEndSize;	//�յ�λ��
	for (int i=0; i < nSize; i++)
	{
		if (m_stData.m_wData[i] > MAX_HEIGHT) 
			m_stData.m_wData[i] = MAX_HEIGHT;

		pDC->LineTo(i*xb, height - m_stData.m_wData[i]*yb);
	}
}*/

// ��ѡ�е�ʮ��
void CUltrasonicImmersedTestView::DrawSelectCross(CDC* pDC, CRect rect,CURVEPOINT_DATA_t stCurvePointData[], int nSelectPoint)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawSelectCross"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T("CUltrasonicImmersedTestView::DrawSelectCross pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawSelectCross"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	POINT point = CurveDataToPoint(stCurvePointData[nSelectPoint], rect);
	CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);
	// ������
	pDC->MoveTo(point.x, point.y - 5);
	pDC->LineTo(point.x, point.y + 5);
	// ������
	pDC->MoveTo(point.x - 5, point.y);
	pDC->LineTo(point.x + 5, point.y);

	pDC->SelectObject(pOldPen);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawSelectCross"),"Leave");
}

// �����е�ʮ��
void CUltrasonicImmersedTestView::DrawAllCross(CDC* pDC, CRect rect, CURVEPOINT_DATA_t stCurvePointData[], WORD nDataNum)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawAllCross"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T("CUltrasonicImmersedTestView::DrawAllCross pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawAllCross"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	CPen pen(PS_SOLID, 2, RGB(0, 0, 255));
	CPen* pOldPen = pDC->SelectObject(&pen);

	for(int i = 0; i < nDataNum; i++)
	{
		POINT point = CurveDataToPoint(stCurvePointData[i], rect);

		// ������
		pDC->MoveTo(point.x, point.y - 5);
		pDC->LineTo(point.x, point.y + 5);
		// ������
		pDC->MoveTo(point.x - 5, point.y);
		pDC->LineTo(point.x + 5, point.y);
	}

	pDC->SelectObject(pOldPen);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawAllCross"),"Leave");
}

//����֮�仭����ΪnSegmentNum�ĺڰ׼���߶�  yachang.huang add start on 2012-7-25 
void CUltrasonicImmersedTestView::DrawRegularLine(CDC* pDC,CPoint ptOrigin,CPoint ptEnd,int nSegmentNum)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawRegularLine"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T(" CUltrasonicImmersedTestView::DrawRegularLine pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawRegularLine"),"Leave");
		return;
	}
	// yachang.huang  add end on 2012-11-02
	for (int i = 0;i < nSegmentNum;i++)
		{
			pDC->MoveTo(static_cast<int>(ptOrigin.x+1.0/nSegmentNum*(ptEnd.x - ptOrigin.x)*i), static_cast<int>(ptOrigin.y+1.0/nSegmentNum*(ptEnd.y - ptOrigin.y)*i));
			pDC->LineTo(static_cast<int>(ptOrigin.x+1.0/nSegmentNum*(ptEnd.x - ptOrigin.x)*(i+1)),static_cast<int>(ptOrigin.y+1.0/nSegmentNum*(ptEnd.y - ptOrigin.y)*(i+1)));
			i++;
		}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::DrawRegularLine"),"Leave");

}
// yachang.huang add end on 2012-7-25 
//yachang.huang add start on 2012-8-8  
int CUltrasonicImmersedTestView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnCreate"),"Enter");
	if (CView::OnCreate(lpCreateStruct) == -1)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnCreate"),"Leave");
		return -1;
	}

	// TODO:  �ڴ������ר�õĴ�������
	m_pHRuler->SetBackGroundColor( RGB(7,165,0) );
	m_pHRuler->SetStyle( RWSTYLE_AHORZ | RWSTYLE_TOPALIGN | RWSTYLE_DRAWEDGE );
	m_pHRuler->Create(WS_VISIBLE ,m_rectScaleX,this,WM_USER+100);
	m_pHRuler->ShowWindow(WM_USER+100);

	m_pVRuler->SetBackGroundColor( RGB(7,165,0) );
	m_pVRuler->SetStyle( RWSTYLE_AVERT | RWSTYLE_RIGHTALIGN | RWSTYLE_DRAWMODAL);
	m_pVRuler->Create(WS_VISIBLE,m_rectScaleY,this,WM_USER+101);
	m_pVRuler->ShowWindow(WM_USER+101);
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestView::OnCreate"),"Leave");
	return 0;
}
//yachang.huang add end on 2012-8-8 
