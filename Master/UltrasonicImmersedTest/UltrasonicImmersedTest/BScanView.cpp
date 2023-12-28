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
	// ��������
	//CMemDC dcMem(pDC, &m_BScanArea);

	// ������
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
* ��Bɨ��ͼ��Ա����(������������/������/������)
* @param     pDC [in]: �豸��������ָ�� 
* @param     rect[in]: Aɨ�ͻ�����ͼ����                                                         
* @return    void
* @since     1.0
*/
void CBScanView::DrawBScan(CDC* pDC, CRect& rect)
{
	// ��������߼���������
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
// 	// �������(�˴����޸�)
// 	float fStep = (m_fXMaxScale - m_fXMinScale)/10.0;//�̶ȳߵĲ���
// 	//��X���
// 	DrawScale(pDC, m_BScanScaleX, m_fXMinScale, m_fXMaxScale, fStep, 1, TRUE);
// 
// 
// 	//��ȡ�������
// 	m_fGongJianHeight = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_fThick;
// 	float fYStep = (m_fGongJianHeight - 0)/10.0;
// 	//��Y���
// 	DrawScale(pDC, m_BScanScaleY, 0, m_fGongJianHeight, fYStep, 4, FALSE);

	//ˢ��Bɨ��ͼ����ʱ��������ʾBɨ
	vector<CFeatureInfo>::iterator iter;
	for (iter = m_vfeature.begin();iter != m_vfeature.end();++iter)
	{
		DrawBScanPicture(pDC,m_BScanArea,*iter);
	}
	
	//������������֮��հ�����  
	FillSpace(pDC,CRect(rect.left,rect.top,CSCAN_SCALE_X_LENGTH,rect.top+CSCAN_SCALE_Y_LENGTH));
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::DrawBScan"),"Leave");
	
}
/** 
* ����̶��Լ���������
* @param     rectAll    [in]: Aɨ�ͻ�����ͼ����
* @param     rectXScale [out]: X��̶ȳ�����
* @param	 rectYScale [out]: Y��̶ȳ�����
* @param     rectWave   [in]: ������ʾ����                                             
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
	//x��������
	rectXScale.SetRect(XScaleTop,XScaleBottom);

	POINT YScaleTop,YScaleBottom;
	YScaleTop.x = rectAll.left;
	YScaleTop.y = rectAll.top + CSCAN_SCALE_Y_LENGTH;
	//YScaleBottom.x = 1.0/20.0*rectAll.Width();
	YScaleBottom.x = CSCAN_SCALE_X_LENGTH;
	//YScaleBottom.y = rectAll.bottom-1.0/10.0*rectAll.Height();
	YScaleBottom.y = rectAll.bottom;

	//y��������
	rectYScale.SetRect(YScaleTop,YScaleBottom);

	POINT WaveTop,WaveBottom;
	//WaveTop.x = 1.0/20.0*rectAll.Width();
	WaveTop.x = CSCAN_SCALE_X_LENGTH;
	WaveTop.y = rectAll.top + CSCAN_SCALE_Y_LENGTH;
	WaveBottom.x = rectAll.right;
	//WaveBottom.y = rectAll.bottom - 1.0/10.0*rectAll.Height();
	WaveBottom.y = rectAll.bottom;
	//������ʾ����
	rectWave.SetRect(WaveTop,WaveBottom);
	rectXScale.left = 0; //yachang.huang add on 2012-8-15   ˮƽ����������Ϊ0
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CBScanView::DistributeRegion"),"Leave");





}
/** 
* ��Bɨ��ͼ����Y�̶ȱ��/����X�̶ȱ��
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
		// 		MessageBox(_T("����ֵ����!"), _T("����"), MB_OK|MB_ICONERROR);
		// 		return;
		fStep = 100.0;
	}
	pDC->SetBkMode(TRANSPARENT);

	CBrush brush(RGB(0,192,0));
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
			float fYData = fStart+(fStep*i);
			str.Format(_T("%.1f"), fYData);
			// ������㣬�յ�
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
	pDC->TextOut(rect.left + 30,rect.top+5,_T("Bɨ"));
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
//Bɨ��ͼ����
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
	//���բ��һ�رջ�������ֵ�����ڣ�ʱ��δ����ʱ������Bɨͼ��
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (!pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo->m_GateInfo[0].m_bOpen||
		!pFrame->m_wndChannelInfo.m_pFormView->IsDlgButtonChecked(IDC_CHECK_MP_M_TIME))
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CBScanView::DrawBScanPicture"),"Leave");
		return;
	}

	CPoint BScanPoint;//�ɼ�������ת�����Bɨ�ͻ�������

	double dParam[SYSPARAM1_NUM];
	::ZeroMemory(dParam, sizeof(dParam));
	long lLogParam[10];
	::ZeroMemory(lLogParam, sizeof(lLogParam));

	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetPara(dParam,lLogParam);
	double dCrood = 0;
	if (lLogParam[0] == 0)//����ɨ��
	{
		dCrood = featureInfo.m_dX;
	}
	else if (lLogParam[0] == 1)//����ɨ��
	{
		dCrood = featureInfo.m_dY;
	}

	//i��ʾբ������󲨷���7������ֵ��������
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

	//����ĺ�����ﵽ�̶ȳߵ������ʱ�����ٻ���
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

	// TODO:  �ڴ������ר�õĴ�������




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
