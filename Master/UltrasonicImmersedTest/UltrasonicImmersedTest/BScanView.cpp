// BScanView.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "ExButton.h"
#include "FeatureInfoWnd.h"
#include "MovePathView.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "SysParam.h"
#include "MechanicalControlPanel.h"
#include "MechanicalControl.h"
#include "CScanDlgBar.h"
#include "ChannelInfoDockView.h"
#include "BScanView.h"
#include "MemDC.h"
#include "GlobalFunction.h"
#include "WSocket.h"
#include "ARSSocket.h"
#include "DataSocket.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "BScanParamPanel.h"
#include "RulerWnd.h"
// #include<vector>
// using namespace std;


extern DWORD g_dwActiveChannel;
// CBScanView

IMPLEMENT_DYNCREATE(CBScanView, CView)

CBScanView::CBScanView()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::CBScanView"),"Enter");
	m_fXMaxScale = 100.0f;
	m_fXMinScale = 0.0f;
	m_fGongJianHeight = 10.0f;
	m_fGongJianLong = 600.0f;
	m_pHRuler = new CRulerWnd; //yachang.huang add on 2012-8-15
	m_pVRuler = new CRulerWnd; //yachang.huang add on 2012-8-15
	// yachang.huang  add start on 2012-11-02
	if (m_pHRuler == NULL)
	{
		AfxMessageBox(_T("CBScanView::CBScanView m_pHRuler"), MB_OK|MB_ICONERROR, -1);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CBScanView::CBScanView"),"Leave");
		return ;
	}
	if (m_pVRuler == NULL)
	{
		AfxMessageBox(_T("CBScanView::CBScanView m_pVRuler"), MB_OK|MB_ICONERROR, -1);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CBScanView::CBScanView"),"Leave");
		return ;
	}
	// yachang.huang  add end on 2012-11-02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::CBScanView"),"Leave");

}

CBScanView::~CBScanView()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::~CBScanView"),"Enter");
	//yachang.huang add start on 2012-8-15
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

	//yachang.huang add end on 2012-8-15
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::~CBScanView"),"Leave");
}

BEGIN_MESSAGE_MAP(CBScanView, CView)
	ON_MESSAGE(WM_BSCAN_DRAW, &CBScanView::ReceiveBScanData)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CBScanView drawing

void CBScanView::OnDraw(CDC* pDC)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::OnDraw"),"Enter");
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	CRect rect;
	GetClientRect(rect);
	// 波形区域
	//CMemDC dcMem(pDC, &m_BScanArea);

	// 画波形
	DrawBScan(pDC, rect);

	//pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),&dcMem, 0, 0, SRCCOPY);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::OnDraw"),"Leave");
}


// CBScanView diagnostics

#ifdef _DEBUG
void CBScanView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBScanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBScanView message handlers
/** 
* 画B扫视图成员函数(包括分配区域/画网格/画波形)
* @param     pDC [in]: 设备环境对象指针 
* @param     rect[in]: A扫客户端视图区域                                                         
* @return    void
* @since     1.0
*/
void CBScanView::DrawBScan(CDC* pDC, CRect& rect)
{
	// 分配坐标尺及波形区域
	//	CRect rectXScale;
	//CRect rectYScale;
	//	CRect rectWave;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::DrawBScan"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		AfxMessageBox(_T("CBScanView::DrawBScan pDC"), MB_OK|MB_ICONERROR, -1);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CBScanView::DrawBScan"),"Leave");
		return ;
	}
	// yachang.huang  add end on 2012-11-02
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	DistributeRegion(rect, m_BScanScaleX, m_BScanScaleY, m_BScanArea);
    m_pVRuler->MoveWindow(&m_BScanScaleY);     //yachang.huang add on 2012-8-15 
	m_pHRuler->MoveWindow(&m_BScanScaleX);     //yachang.huang add on 2012-8-15   

// 	m_fXMinScale = pFrame->m_wndCScanParamDockBar.m_pBScanParamView->m_fBScanOrigin;
// 	m_fXMaxScale = pFrame->m_wndCScanParamDockBar.m_pBScanParamView->m_fBScanWidth + m_fXMinScale;
// 	// 画坐标尺(此处需修改)
// 	float fStep = (m_fXMaxScale - m_fXMinScale)/10.0;//刻度尺的步长
// 	//画X标尺
// 	DrawScale(pDC, m_BScanScaleX, m_fXMinScale, m_fXMaxScale, fStep, 1, TRUE);
// 
// 
// 	//获取工件厚度
// 	m_fGongJianHeight = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_fThick;
// 	float fYStep = (m_fGongJianHeight - 0)/10.0;
// 	//画Y标尺
// 	DrawScale(pDC, m_BScanScaleY, 0, m_fGongJianHeight, fYStep, 4, FALSE);

	//刷新B扫视图区域时，从新显示B扫
	vector<CFeatureInfo>::iterator iter;
	for (iter = m_vfeature.begin();iter != m_vfeature.end();++iter)
	{
		DrawBScanPicture(pDC,m_BScanArea,*iter);
	}
	
	//填充两个坐标尺之间空白区域  
	FillSpace(pDC,CRect(rect.left,rect.top,CSCAN_SCALE_X_LENGTH,rect.top+CSCAN_SCALE_Y_LENGTH));
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::DrawBScan"),"Leave");
	
}
/** 
* 分配刻度以及波形区域
* @param     rectAll    [in]: A扫客户端视图区域
* @param     rectXScale [out]: X轴刻度尺区域
* @param	 rectYScale [out]: Y轴刻度尺区域
* @param     rectWave   [in]: 波形显示区域                                             
* @return    void
* @since     1.0
*/
void CBScanView::DistributeRegion(const CRect& rectAll, 
								   CRect& rectXScale,
								   CRect& rectYScale,
								   CRect& rectWave)
{	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::DistributeRegion"),"Enter");
	POINT XScaleTop,XScaleBottom;
	//XScaleTop.x = 1.0/20.0*rectAll.Width();
	XScaleTop.x = CSCAN_SCALE_X_LENGTH;
	//XScaleTop.y = rectAll.bottom-1.0/10.0*rectAll.Height();
	XScaleTop.y = rectAll.top;
	XScaleBottom.x = rectAll.right;
	XScaleBottom.y = rectAll.top + CSCAN_SCALE_Y_LENGTH;
	//x坐标区域
	rectXScale.SetRect(XScaleTop,XScaleBottom);

	POINT YScaleTop,YScaleBottom;
	YScaleTop.x = rectAll.left;
	YScaleTop.y = rectAll.top + CSCAN_SCALE_Y_LENGTH;
	//YScaleBottom.x = 1.0/20.0*rectAll.Width();
	YScaleBottom.x = CSCAN_SCALE_X_LENGTH;
	//YScaleBottom.y = rectAll.bottom-1.0/10.0*rectAll.Height();
	YScaleBottom.y = rectAll.bottom;

	//y坐标区域
	rectYScale.SetRect(YScaleTop,YScaleBottom);

	POINT WaveTop,WaveBottom;
	//WaveTop.x = 1.0/20.0*rectAll.Width();
	WaveTop.x = CSCAN_SCALE_X_LENGTH;
	WaveTop.y = rectAll.top + CSCAN_SCALE_Y_LENGTH;
	WaveBottom.x = rectAll.right;
	//WaveBottom.y = rectAll.bottom - 1.0/10.0*rectAll.Height();
	WaveBottom.y = rectAll.bottom;
	//波形显示区域
	rectWave.SetRect(WaveTop,WaveBottom);
	rectXScale.left = 0; //yachang.huang add on 2012-8-15   水平标尺左边坐标为0
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::DistributeRegion"),"Leave");





}
/** 
* 画B扫视图竖向Y刻度标尺/横向X刻度标尺
* @param     pDC       [in]: 画图设备环境指针
* @param     rect      [in]: 坐标刻度尺区域
* @param     nStart    [in]: 最大刻度值
* @param     nEnd      [in]:  最小刻度值   
* @param     nStep     [in]: 刻度步进
* @param     direction [in]: 刻度尺的位置
* @param     bReverse  [in]: 画刻度尺开关
* @return    void
* @since     1.0
*/
void CBScanView::DrawScale(CDC* pDC, 
							const CRect& rect, 
							float fStart, 
							float fEnd, 
							float fStep, 
							int direction, 
							BOOL bReverse)
{
	//	ASSERT(nStart != nEnd);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::DrawScale"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		AfxMessageBox(_T("CBScanView::DrawScale pDC"), MB_OK|MB_ICONERROR, -1);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CBScanView::DrawScale"),"Leave");
		return ;
	}
	// yachang.huang  add end on 2012-11-02
	if (fStep<1E-6)
	{
		// 		MessageBox(_T("步长值错误!"), _T("警告"), MB_OK|MB_ICONERROR);
		// 		return;
		fStep = 100.0;
	}
	pDC->SetBkMode(TRANSPARENT);

	CBrush brush(RGB(0,192,0));
	CBrush *pOldBrush = pDC->SelectObject(&brush);
	// 填充矩形
	pDC->Rectangle(rect);
	// 画分隔线
	int n = static_cast<int>((fEnd - fStart)/fStep);
	float fPerCoordinate = 0.0f;
	CString str;
	// 上下标尺
	if (direction == 1 || direction == 3)
	{
		fPerCoordinate = float(rect.Width())/float(fEnd - fStart);
		for (int i=0; i<=n; i++)
		{
			str.Format(_T("%.1f"), fStart+(fStep*i));
			// 定义起点，终点
			CPoint ptStart, ptEnd;
			ptStart.x = static_cast<LONG>(rect.left + fPerCoordinate*fStep*i);
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
				pDC->TextOut(ptEnd.x+20, ptEnd.y, str);//ptEnd.x加上20，是为了使刻度靠右偏移，使第一个刻度值能够显示出来。
			}

			pDC->MoveTo(ptStart);
			pDC->LineTo(ptEnd);
		}
	}
	// 左右标尺
	else if (direction == 2 || direction == 4)
	{
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);

		LONG lHalfHeight = tm.tmHeight/2;
		fPerCoordinate = float(rect.Height())/float(fEnd - fStart);
		for (int i=1; i<n; i++)
		{
			float fYData = fStart+(fStep*i);
			str.Format(_T("%.1f"), fYData);
			// 定义起点，终点
			CPoint ptStart, ptEnd;
			if (!bReverse)
			{
				ptStart.y = static_cast<LONG>(rect.top + fPerCoordinate*fStep*i);
			}
			else
				ptStart.y = static_cast<LONG>(rect.bottom - fPerCoordinate*fStep*i);

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

	pDC->SelectObject(pOldBrush);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::DrawScale"),"Leave");
}
void CBScanView::FillSpace(CDC* pDC,CRect &rect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::FillSpace"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		AfxMessageBox(_T("CBScanView::FillSpace pDC"), MB_OK|MB_ICONERROR, -1);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CBScanView::FillSpace"),"Leave");
		return ;
	}
	// yachang.huang  add end on 2012-11-02
	CBrush brush(RGB(0,192,0));
	pDC->FillRect(&rect,&brush);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut(rect.left + 30,rect.top+5,_T("B扫"));
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::FillSpace"),"Leave");

}

LRESULT CBScanView::ReceiveBScanData(WPARAM wParam, LPARAM lParam)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::ReceiveBScanData"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	int nGateNum = pFrame->m_pFeatureWnd->m_nGateOpenNum;
	int nFeatureNum = pFrame->m_pFeatureWnd->m_nFeatureOpenNum+1;

	CFeatureInfo *pData = (CFeatureInfo*)wParam;
		
	m_vfeature.push_back(*pData);

	CDC* pDC = this->GetDC();
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		AfxMessageBox(_T("CBScanView::ReceiveBScanData GetDC"), MB_OK|MB_ICONERROR, -1);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CBScanView::ReceiveBScanData"),"Leave");
		return -1 ;
	}
	// yachang.huang  add end on 2012-11-02


	DrawBScanPicture(pDC,m_BScanArea,*pData);

	delete pData;
	pData = NULL;
	ReleaseDC(pDC);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::ReceiveBScanData"),"Leave");
	return 0;
}
//B扫画图函数
void CBScanView::DrawBScanPicture(CDC* pDC,const CRect& rect,CFeatureInfo& featureInfo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::DrawBScanPicture"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		AfxMessageBox(_T("CBScanView::DrawBScanPicture pDC"), MB_OK|MB_ICONERROR, -1);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CBScanView::DrawBScanPicture"),"Leave");
		return ;
	}
	// yachang.huang  add end on 2012-11-02
	//如果闸门一关闭或者最大峰值（门内）时间未开启时，不画B扫图形
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (!pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo->m_GateInfo[0].m_bOpen||
		!pFrame->m_wndChannelInfo.m_pFormView->IsDlgButtonChecked(IDC_CHECK_MP_M_TIME))
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CBScanView::DrawBScanPicture"),"Leave");
		return;
	}

	CPoint BScanPoint;//采集点坐标转换后的B扫客户区坐标

	double dParam[SYSPARAM1_NUM];
	::ZeroMemory(dParam, sizeof(dParam));
	long lLogParam[10];
	::ZeroMemory(lLogParam, sizeof(lLogParam));

	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetPara(dParam,lLogParam);
	double dCrood = 0;
	if (lLogParam[0] == 0)//横向扫描
	{
		dCrood = featureInfo.m_dX;
	}
	else if (lLogParam[0] == 1)//纵向扫描
	{
		dCrood = featureInfo.m_dY;
	}

	//i表示闸门内最大波幅在7个特征值里面的序号
	int nTimeIdx = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_nMPMTimeIndex;
	int MPMPeakIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_nMPMPeakIndex;

	int width =m_BScanScaleY.Width();
	int width1 =rect.Width();
	BScanPoint.x = static_cast<LONG>(m_BScanScaleY.Width() + (rect.Width()*(dCrood - m_fXMinScale)/pFrame->m_wndCScanParamDockBar.m_pBScanParamView->m_fBScanWidth));
	CComboBox* pcmbGate = (CComboBox*)pFrame->m_wndChannelInfo.m_pFormView->GetDlgItem(IDC_COMBO_GATE_NO);
	float fYuzhi = pFrame->m_wndChannelInfo.m_pFormView->m_fGatePeak;
	int nIndex = pcmbGate->GetCurSel();
	
	if (!(pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo[g_dwActiveChannel].m_GateInfo[nIndex].m_bOpen))
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CBScanView::DrawBScanPicture"),"Leave");
		return;
	}
	
	for (int i = nIndex; i>0; i--)
	{
		if (!(pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo[g_dwActiveChannel].m_GateInfo[i].m_bOpen))
		{
			nIndex--;
		}
	}

	float value = featureInfo.m_pfFeatures[nIndex][MPMPeakIndex];
	if (featureInfo.m_pfFeatures[nIndex][MPMPeakIndex] >= fYuzhi)
	{
		BScanPoint.y = static_cast<LONG>(m_BScanScaleX.Height()+ featureInfo.m_pfFeatures[nIndex][nTimeIdx]*(rect.Height() - m_BScanScaleX.Height())/m_fGongJianHeight);
	} 
	else
	{
		BScanPoint.y = m_BScanScaleX.Height();
	}

	//当点的横坐标达到刻度尺的最左端时，不再划线
	if (BScanPoint.x < m_BScanScaleY.Width())
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::DrawBScanPicture"),"Leave");
		return;
	}
	
	CPen pen(PS_SOLID,1,RGB(0,0,255));
	CPen* pPen = pDC->SelectObject(&pen);
	pDC->MoveTo(BScanPoint.x,m_BScanScaleX.Height());
	pDC->LineTo(BScanPoint.x,BScanPoint.y);

	pDC->SelectObject(pPen);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::DrawBScanPicture"),"Leave");

}
int CBScanView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::OnCreate"),"Enter");
	if (CView::OnCreate(lpCreateStruct) == -1)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CBScanView::OnCreate"),"Leave");
		return -1;
	}

	// TODO:  在此添加您专用的创建代码




	m_pVRuler->SetBackGroundColor( RGB(7,165,0) );
	m_pVRuler->SetStyle( RWSTYLE_BVERT | RWSTYLE_RIGHTALIGN | RWSTYLE_DRAWMODAL);
	m_pVRuler->Create(WS_VISIBLE,m_BScanScaleY,this,WM_USER+106);
	m_pVRuler->ShowWindow(WM_USER+106);

	m_pHRuler->SetBackGroundColor( RGB(7,165,0) );
	m_pHRuler->SetStyle( RWSTYLE_BHORZ | RWSTYLE_BOTTOMALIGN | RWSTYLE_DRAWEDGE );
	m_pHRuler->Create(WS_VISIBLE ,m_BScanScaleX,this,WM_USER+105);
	m_pHRuler->ShowWindow(WM_USER+105);
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::OnCreate"),"Leave");
	return 0;
}
