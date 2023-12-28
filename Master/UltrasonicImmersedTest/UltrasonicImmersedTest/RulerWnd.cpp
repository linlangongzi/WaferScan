// RulerWnd.cpp : implementation file
#include "stdafx.h"
#include "RulerWnd.h"
#include <math.h>
#include "MemDCForRuler.h"
#include "UltrasonicImmersedTestView.h"
#include "UltrasonicImmersedTestDoc.h"
#include "UltrasonicImmersedTest.h"
#include "CScanDlgBar.h"
#include "ChannelInfoDockView.h"
#include "MechanicalControl.h"
#include "CScanParamDockBar.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "const.h"
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
#include "BScanView.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "BScanParamPanel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE 
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRulerWnd
CRulerWnd::CRulerWnd()
{
   ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::CRulerWnd"),"Enter");
   m_dwStyle         = 0;
   m_clrBackGround = RGB( 0 , 192 , 0 );
   m_clrMilimeterLineColor = RGB( 0 , 0 , 0 );
   m_clrTextColor          = RGB( 0 , 0 , 0 );
   m_nObjectCount = 0;
   RegisterWindowClass();
   m_nCStep = 65;
   ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::CRulerWnd"),"Leave");
}

CRulerWnd::~CRulerWnd()
{
   ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::~CRulerWnd"),"Enter");
   DestroyWindow();
   WNDCLASS wndcls = { 0 };
   if( ( ::GetClassInfo( AfxGetResourceHandle() , RULERWINDOW_CLASSNAME , &wndcls ) ) ) 
   {
      ::UnregisterClass( wndcls.lpszClassName , AfxGetResourceHandle() );
   }
   ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::~CRulerWnd"),"Leave");
}


BEGIN_MESSAGE_MAP(CRulerWnd, CWnd)
	//{{AFX_MSG_MAP(CRulerWnd)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRulerWnd message handlers
//ע�ᴰ����
BOOL CRulerWnd::RegisterWindowClass()
{
    ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::RegisterWindowClass"),"Enter");
    WNDCLASS wndcls;

    HINSTANCE hInst = AfxGetResourceHandle();

    if ( !( ::GetClassInfo( hInst, RULERWINDOW_CLASSNAME , &wndcls ) ) )
    {

       // otherwise we need to register a new class
       wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
       wndcls.lpfnWndProc      = ::DefWindowProc;
       wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
       wndcls.hInstance        = hInst;
       wndcls.hIcon            = NULL;
#ifndef _WIN32_WCE_NO_CURSOR
       wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
#else
       wndcls.hCursor          = 0;
#endif										
       wndcls.hbrBackground    = (HBRUSH)( GetStockObject(WHITE_BRUSH)) ;
       wndcls.lpszMenuName     = NULL;
       wndcls.lpszClassName    = RULERWINDOW_CLASSNAME;
        
       if( !AfxRegisterClass( &wndcls ) )
       {

          AfxThrowResourceException();
		  LOG4CXX_ERROR(log4cxx::Logger::getLogger("CRulerWnd::RegisterWindowClass"),"Leave");
          return FALSE;

       }

    }
    ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::RegisterWindowClass"),"Leave");
    return TRUE;

}


/*************************************************************************
* �������ƣ�
*   Create����
* ����:
*   DWORD dwStyle - ���ڷ��
*   const RECT &rect - ������ռλ��
*   CWnd *pParentWnd - ������ָ��
*   UINT nID - �Ӵ���ID��
* ����ֵ:
*   BOOL
* ˵��:
*   ������߿ؼ�  
* ��ע��
*   ������ߺ���������λ�õĻ��������MoveWindow��������
************************************************************************/
BOOL CRulerWnd::Create( DWORD dwStyle, const RECT &rect, CWnd *pParentWnd , UINT nID )
{
   ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::Create"),"Enter");
   ASSERT(pParentWnd->GetSafeHwnd());

   if (!CWnd::Create( RULERWINDOW_CLASSNAME , NULL, dwStyle, rect, pParentWnd, nID ) )
   {
	    LOG4CXX_ERROR(log4cxx::Logger::getLogger("CRulerWnd::Create"),"Leave");
        return FALSE;
   }
   ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::Create"),"Leave");
   return TRUE;

}



/*************************************************************************
* �������ƣ�
*   OnEraseBkgnd����
* ����:
*   CDC* pDC - �豸�����Ķ�������ָ�� 
* ����ֵ:
*   BOOL
* ˵��:
*   �ؼ�ˢ�º������������С�仯������  
* ��ע��
*   ���̶Ⱥ���������������ã�����ʵʱ�ı�̶�����
************************************************************************/
BOOL CRulerWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::OnEraseBkgnd"),"Enter");
	m_pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pAScanView = (CUltrasonicImmersedTestView*)m_pFrame->m_wndSplitter.GetPane(1,0);
	CMemDCForRuler memDC( pDC );
    float fAStep = static_cast<float>((m_pAScanView->m_fXMaxRuler - m_pAScanView->m_fXMinRuler)/10.0);
	CRect rect;
	GetClientRect( &rect );
	if(m_nObjectCount == 1)
	{
		m_pCScanView = (CCScanView*)m_pFrame->m_wndSplitter.GetPane(2,0);	
	}
	else
	{
		m_pCScanView = (CCScanView*)m_pFrame->m_wndSplitter.GetPane(3,0);
	}
	// WangQianfeng delete start on 2013-06-28
	m_pBScanView = (CBScanView*)m_pFrame->m_wndSplitter.GetPane(4,0);
	// WangQianfeng  delete end on 2013-06-28
	memDC.FillSolidRect( &rect , m_clrBackGround );
	float fXmin;
	float fXmax;
	float fYmax;
	float fYmin;
	// ��X����
	if (m_pCScanView->m_Offset_Coordinate.y>=0)
	{
		fXmin = m_pCScanView->m_fZuobiaoXmin - m_pCScanView->m_Offset_Coordinate.x;
		fXmax = m_pCScanView->m_fZuobiaoXmax - m_pCScanView->m_Offset_Coordinate.x;
		fYmax = m_pCScanView->m_fZuobiaoYmax - m_pCScanView->m_Offset_Coordinate.y;
		fYmin = m_pCScanView->m_fZuobiaoYmin - m_pCScanView->m_Offset_Coordinate.y;
	} 
	else
	{
		fXmin = m_pCScanView->m_fZuobiaoXmin;
		fXmax = m_pCScanView->m_fZuobiaoXmax;
		fYmax = m_pCScanView->m_fZuobiaoYmax;
		fYmin = m_pCScanView->m_fZuobiaoYmin;
	}
	float fCXStep  = (fXmax - fXmin) / 10;
	float fCYStep  = (fYmax - fYmin) / 10;

	// WangQianfeng delete start on 2013-06-28
	m_pBScanView->m_fXMinScale = m_pFrame->m_wndCScanParamDockBar.m_pBScanParamView->m_fBScanOrigin;
	m_pBScanView->m_fXMaxScale = m_pFrame->m_wndCScanParamDockBar.m_pBScanParamView->m_fBScanWidth + m_pBScanView->m_fXMinScale;

	// �������(�˴����޸�)
	float fBXStep = static_cast<float>((m_pBScanView->m_fXMaxScale - m_pBScanView->m_fXMinScale)/10.0);//�̶ȳߵĲ���

	m_pBScanView->m_fGongJianHeight = m_pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_fThick;
	float fBYStep = static_cast<float>((m_pBScanView->m_fGongJianHeight - 0)/10.0);
	// WangQianfeng delete end on 2013-06-28
	//Drawing Ruler


	if( m_dwStyle & RWSTYLE_AHORZ )
        DrawHRuler( &memDC,m_pAScanView->m_fXMinRuler, m_pAScanView->m_fXMaxRuler, fAStep, 3, TRUE,1);
	// WangQianfeng delete start on 2013-06-28
	else if ( m_dwStyle & RWSTYLE_BHORZ)
		DrawHRuler(&memDC,m_pBScanView->m_fXMinScale,m_pBScanView->m_fXMaxScale,fBXStep,1,TRUE,2);
	// WangQianfeng delete end on 2013-06-28
 	else if (m_dwStyle & RWSTYLE_CHORZ)
 		DrawHRuler(&memDC,fXmin, fXmax, fCXStep, 3, TRUE,3);
	// WangQianfeng delete start on 2013-06-28
	else if (m_dwStyle & RWSTYLE_BVERT)
		DrawVRuler(&memDC,0,m_pBScanView->m_fGongJianHeight, fBYStep, 4, FALSE,2);
	// WangQianfeng delete end on 2013-06-28
	else if ( m_dwStyle & RWSTYLE_CVERT)
		DrawVRuler(&memDC,fYmin, fYmax, fCYStep, 4, TRUE,3);
	else if( m_dwStyle & RWSTYLE_AVERT )
		DrawVRuler( &memDC, m_pAScanView->m_fXMinRuler, m_pAScanView->m_fXMaxRuler, fAStep, 3, TRUE,1);

    ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::OnEraseBkgnd"),"Leave");
	return TRUE;//CWnd::OnEraseBkgnd(pDC);
}

void CRulerWnd::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::PreSubclassWindow"),"Enter");
	CWnd::PreSubclassWindow();
    ModifyStyleEx( 0,  /*WS_EX_DLGMODALFRAME |*/ SWP_DRAWFRAME  );
	SetWindowPos( NULL ,  0 ,0 , 0 , 0 , SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER );
	////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::PreSubclassWindow"),"Leave");

}

/*************************************************************************
* �������ƣ�
*   DrawHRuler����
* ����:
*  CDC *pDC,        [in]: ��ͼ�豸����ָ��
*  float fStart     [in]: ����̶ȳ�����
*  float fEnd       [in]: ���̶�ֵ
*  float fEnd       [in]:  ��С�̶�ֵ   
*  float fStep      [in]: �̶Ȳ���
*  int direction    [in]: �̶ȳߵ�λ��
*  BOOL bReverse    [in]: ���̶ȳ߿���
*  int ScanNum      [in]: ɨͼ���
* ����ֵ:
*   BOOL
* ˵��:
*   �����߿̶�  
* ��ע��
*   ������Դ��ԭ����DrawScale()���������ֲ���Ŀǰû���õ��������Ժ���Ӹ��Ĺ���
************************************************************************/
BOOL CRulerWnd::DrawHRuler(CDC *pDC,float fStart, float fEnd, float fStep, int direction, BOOL bReverse,int ScanNum)
{
    ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::DrawHRuler"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T("CRulerWnd::DrawHRuler pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CRulerWnd::DrawHRuler"),"Leave");
		return FALSE;
	}
	// yachang.huang  add end on 2012-11-02
	CRect rectRuler;
	GetClientRect( &rectRuler);
	CPen pen;
	pen.CreatePen( PS_SOLID , 1 , m_clrMilimeterLineColor );
	CPen *pOldPen = pDC->SelectObject( &pen );
	if (fStep<1E-6)
	{
		fStep = 100.0;
	}

	int iStartPos = 0;  

	LOGFONT lf;

	memset( &lf , 0 , sizeof( lf ) );
	CFont font;
	font.CreatePointFont( 70, _T( "Arial") );
	font.GetLogFont( &lf );
	font.DeleteObject();

	CRect rectText;

	int iLLineStart , iLLineEnd;
	int iMLineStart , iMLineEnd;
	int iSLineStart , iSLineEnd;

	if( m_dwStyle & RWSTYLE_BOTTOMALIGN ) 
	{
		rectText.top     = rectRuler.bottom - 30;
		rectText.bottom  = rectRuler.bottom - 10;

		iLLineStart = rectRuler.bottom - 9;
		iLLineEnd	 = rectRuler.bottom - 2;

		iMLineStart = rectRuler.bottom - 6;
		iMLineEnd	 = rectRuler.bottom - 2;

		iSLineStart = rectRuler.bottom - 4;
		iSLineEnd	 = rectRuler.bottom - 2;

	}
	else if( m_dwStyle & RWSTYLE_TOPALIGN ) 
	{

		rectText.top     = rectRuler.top + 10;
		rectText.bottom  = rectRuler.top + 30;

		iLLineStart = rectRuler.top + 2;
		iLLineEnd = rectRuler.top + 9;

		iMLineStart = rectRuler.top + 2;
		iMLineEnd = rectRuler.top + 6;

		iSLineStart = rectRuler.top + 2;
		iSLineEnd = rectRuler.top + 4;

	}
	CRect rectCScan;
	rectCScan.top = rectRuler.top;
	rectCScan.bottom = rectRuler.bottom;
	rectCScan.left = 0;
	rectCScan.right = CSCAN_SCALE_X_LENGTH;
	CString strCScan;
	switch(ScanNum)
	{
	case 1:
		strCScan.Format(_T("Aɨ"));
		pDC->MoveTo(0,0);
		pDC->LineTo(CSCAN_SCALE_X_LENGTH,0);
		break;
	case 2:
		strCScan.Format(_T("Bɨ"));
		pDC->MoveTo(0,CSCAN_SCALE_Y_LENGTH);
		pDC->LineTo(CSCAN_SCALE_X_LENGTH,CSCAN_SCALE_Y_LENGTH);
		break;
	case 3:
		// wangbingfu add start on 2013-01-11
		{	
			int nCsanMode = 0;
			m_pCScanView = (CCScanView*)m_pFrame->m_wndSplitter.GetPane(2,0);
			if(m_pCScanView->m_nAxisSelect == 4)	//W1��
			{
				if(m_nObjectCount == 1)
				{
					//	m_pCScanView = (CCScanView*)m_pFrame->m_wndSplitter.GetPane(2,0);
					CCScanView* pCScanView2 = (CCScanView*)m_pFrame->m_wndSplitter.GetPane(3,0);
					nCsanMode = pCScanView2->m_nPipeChannelSelect[pCScanView2->m_nCScanSelect];
					if(nCsanMode == 3)	// A���+�ֲ�
					{
						strCScan.Format(_T("A���"));
					}
					else
					{
						nCsanMode = m_pCScanView->m_nPipeChannelSelect[m_pCScanView->m_nCScanSelect];
						if(nCsanMode == 0)	// ��
						{
							strCScan.Format(_T("��"));
						}
						else if(nCsanMode == 1)	// ���
						{
							strCScan.Format(_T("A���"));
						}
						else if(nCsanMode == 2)	// �ֲ�
						{
							strCScan.Format(_T("A�ֲ�"));
						}
						else if(nCsanMode == 3)	// ����B1
						{
							strCScan.Format(_T("����B1"));
						}
						else if(nCsanMode == 4)	// ����B2
						{
							strCScan.Format(_T("����B2"));
						}
						else if(nCsanMode == 5)	// ����C1
						{
							strCScan.Format(_T("����C1"));
						}
						else if(nCsanMode == 6)	// ����C2
						{
							strCScan.Format(_T("����C2"));
						}
						else
						{
							;//
						}
					}
					

				}
				else if(m_nObjectCount == 2)
				{
					m_pCScanView = (CCScanView*)m_pFrame->m_wndSplitter.GetPane(3,0);
					nCsanMode = m_pCScanView->m_nPipeChannelSelect[m_pCScanView->m_nCScanSelect];
					if(nCsanMode == 3)	// A���+�ֲ�
					{
						strCScan.Format(_T("A�ֲ�"));
					}
					else if(nCsanMode == 0)	// ��
					{
						strCScan.Format(_T("��"));
					}
					else if(nCsanMode == 1)	// ���
					{
						strCScan.Format(_T("A���"));
					}
					else if(nCsanMode == 2)	// �ֲ�
					{
						strCScan.Format(_T("A�ֲ�"));
					}
					else if(nCsanMode == 4)	// ����B1
					{
						strCScan.Format(_T("����B1"));
					}
					else if(nCsanMode == 5)	// ����B2
					{
						strCScan.Format(_T("����B2"));
					}
					else if(nCsanMode == 6)	// ����C1
					{
						strCScan.Format(_T("����C1"));
					}
					else if(nCsanMode == 7)	// ����C2
					{
						strCScan.Format(_T("����C2"));
					}
					else
					{
						;//
					}
				}
				else
				{
					;//
				}
			}
			else
			{
				strCScan.Format(_T("Cɨ%d"),m_nObjectCount);
			}
			// wangbingfu add end on 2013-01-11		
			pDC->MoveTo(0,0);
			pDC->LineTo(CSCAN_SCALE_X_LENGTH,0);
		}
		break;
	}
	pDC->SetBkMode( TRANSPARENT );
	pDC->DrawText( strCScan , &rectCScan , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	CFont *pOldFont = pDC->SelectObject( &font );
	CString str;
	int iPos;
	int iSel = 0;
	pDC->SetBkMode( TRANSPARENT );
	pDC->SetTextColor( m_clrTextColor );
    int nStart = 0;
    int nEnd = 0;
	int nLinestart = 0;
	CPen newPen; 
	newPen.CreatePen(PS_SOLID,1,RGB(0,0,0)); 
	CPen* poldpen=pDC->SelectObject(&newPen);  

	switch(ScanNum)
	{
	case 1: 
		nStart = (int)fStart*10;
		nEnd = (int)(fStart +  fStep*10)*10;
		m_nCStep = (nEnd - nStart)/100;
		m_nCStep = (m_nCStep/5 + 1)*5;
		nLinestart = (nStart/m_nCStep + 1)*m_nCStep; 
		if (nStart == 0)
		{
			nLinestart = 0;
		}
		pDC->MoveTo(  CSCAN_SCALE_X_LENGTH , 0 );
		pDC->LineTo(  CSCAN_SCALE_X_LENGTH , rectRuler.bottom  );
		pDC->MoveTo(0,rectRuler.bottom );
		pDC->LineTo(CSCAN_SCALE_X_LENGTH,rectRuler.bottom);

		for (int i = nLinestart;i < nEnd; i+=m_nCStep)
		{
			if (i %m_nCStep == 0)
			{
				iPos = (iStartPos + ((rectRuler.right - CSCAN_SCALE_X_LENGTH)*(i - nStart))/(nEnd - nStart));
				iPos += CSCAN_SCALE_X_LENGTH;
				if (i % (m_nCStep*5) == 0)
				{
					pDC->MoveTo( iPos , iMLineStart ); 
					pDC->LineTo( iPos , iMLineEnd   );	
					if (i % (m_nCStep*10) == 0)
					{
						pDC->MoveTo( iPos , iLLineStart );
						pDC->LineTo( iPos , iLLineEnd   );
						rectText.left = iPos;
						rectText.right = iPos + 60; 
						str.Format(_T("%d"), i/10);
						pDC->DrawText( str , &rectText , DT_LEFT /*| DT_VCENTER | DT_SINGLELINE*/ );
						continue;
					}
					continue;
				}
				pDC->MoveTo( iPos , iSLineStart );
				pDC->LineTo( iPos , iSLineEnd   );		
			}
		}
// 		for( int iSayac = 0 ; iSayac <= 100; iSayac++ ) 
// 		{
// 			iPos = iStartPos + ( iSayac * (rectRuler.right - CSCAN_SCALE_X_LENGTH) /100 );
// 			iPos -= 0;
// 			iPos += CSCAN_SCALE_X_LENGTH;
// 			if (iSayac % 10 == 0)
// 			{
// 				rectText.left     = iPos;
// 				rectText.right    = iPos + 60;
// 				str.Format(_T("%.1f"), fStart+(fStep*iSayac/10));
// 				pDC->DrawText( str , &rectText , DT_LEFT /*| DT_VCENTER | DT_SINGLELINE*/ );
// 			}
// 			if( iPos > rectRuler.right )
// 				break;
// 			if( iSel == 0 ) 
// 			{
// 				if (0 == iSayac)
// 				{
// 					CPen newPen; 
// 					newPen.CreatePen(PS_SOLID,1,RGB(0,0,0)); 
// 					CPen* poldpen=pDC->SelectObject(&newPen); 
// 					pDC->MoveTo(  iPos , 0 );
// 					pDC->LineTo(  iPos , rectRuler.bottom   );
// 					pDC->MoveTo(0,rectRuler.bottom );
// 					pDC->LineTo(iPos,rectRuler.bottom);
// 					pDC->SelectObject(poldpen);
// 					newPen.DeleteObject();
// 				}
// 				else
// 				{
// 					pDC->MoveTo(  iPos , iLLineStart );
// 					pDC->LineTo(  iPos , iLLineEnd   );		
// 				}	
// 			}
// 			else
// 				if( iSel == 5 ) {
// 					pDC->MoveTo( iPos , iMLineStart );
// 					pDC->LineTo( iPos , iMLineEnd   );
// 				}
// 				else {
// 					pDC->MoveTo( iPos , iSLineStart );
// 					pDC->LineTo( iPos , iSLineEnd   );
// 				}
// 				iSel++;
// 				if( iSel == 10 )
// 					iSel = 0;
// 		}
		break;
	case 2:
		for( int iSayac = 0 ; iSayac <= 100; iSayac++ ) 
		{
			iPos = iStartPos + ( iSayac * (rectRuler.right - CSCAN_SCALE_X_LENGTH) /100 );
			iPos -= 0;
			iPos += CSCAN_SCALE_X_LENGTH;
			if (iSayac % 10 == 0)
			{
				rectText.left     = iPos;
				rectText.right    = iPos + 60;
				str.Format(_T("%d"), (int)(fStart+(fStep*iSayac / 10)));
				pDC->DrawText( str , &rectText , DT_LEFT /*| DT_VCENTER | DT_SINGLELINE*/ );
			}
			if( iPos > rectRuler.right )
				break;
			if( iSel == 0 ) 
			{
				if (0 == iSayac)
				{
// 					CPen newPen; 
// 					newPen.CreatePen(PS_SOLID,1,RGB(0,0,0)); 
// 					CPen* poldpen=pDC->SelectObject(&newPen); 
					pDC->MoveTo(  iPos , 0 );
					pDC->LineTo(  iPos , rectRuler.bottom   );
					pDC->MoveTo(0,rectRuler.bottom );
					pDC->LineTo(iPos,rectRuler.bottom);
// 					pDC->SelectObject(poldpen);
// 					newPen.DeleteObject();
				}
				else
				{
					pDC->MoveTo(  iPos , iLLineStart );
					pDC->LineTo(  iPos , iLLineEnd   );		
				}	
			}
			else
				if( iSel == 5 ) {
					pDC->MoveTo( iPos , iMLineStart );
					pDC->LineTo( iPos , iMLineEnd   );
				}
				else {
					pDC->MoveTo( iPos , iSLineStart );
					pDC->LineTo( iPos , iSLineEnd   );
				}
				iSel++;
				if( iSel == 10 )
					iSel = 0;
		}
		break;
	case 3:
		nStart = (int)fStart;
		nEnd = (int)(fStart +  fStep*10);
		m_nCStep = (nEnd - nStart)/100;
		if (m_nCStep < 3)
		{
			m_nCStep = 2;
		}
		else
		{
		   m_nCStep = (m_nCStep/5 + 1)*5;
		}


		nLinestart = (nStart/m_nCStep + 1)*m_nCStep;
		if (nStart == 0)
		{
			nLinestart = 0;
		}
// 			CPen newPen; 
// 			newPen.CreatePen(PS_SOLID,1,RGB(0,0,0)); 
// 			CPen* poldpen=pDC->SelectObject(&newPen); 
			pDC->MoveTo(  CSCAN_SCALE_X_LENGTH , 0 );
			pDC->LineTo(  CSCAN_SCALE_X_LENGTH , rectRuler.bottom   );
			pDC->MoveTo(0,rectRuler.bottom );
			pDC->LineTo(CSCAN_SCALE_X_LENGTH,rectRuler.bottom);
// 			pDC->SelectObject(poldpen);
// 			newPen.DeleteObject();

		for (int i = nLinestart;i < nEnd; i+=m_nCStep)
		{
			if (i %m_nCStep == 0)
			{
				iPos = iStartPos + ((rectRuler.right - CSCAN_SCALE_X_LENGTH)*(i - nStart))/(nEnd - nStart);
				iPos += CSCAN_SCALE_X_LENGTH;
				if (i % (m_nCStep*5) == 0)
				{
					pDC->MoveTo( iPos , iMLineStart );
					pDC->LineTo( iPos , iMLineEnd   );	
					if (i % (m_nCStep*10) == 0)
					{
						pDC->MoveTo( iPos , iLLineStart );
						pDC->LineTo( iPos , iLLineEnd   );
						rectText.left = iPos;
						rectText.right = iPos + 60;
						str.Format(_T("%d"), i);
						pDC->DrawText( str , &rectText , DT_LEFT /*| DT_VCENTER | DT_SINGLELINE*/ );
						continue;
					}
					continue;
				}
				pDC->MoveTo( iPos , iSLineStart );
				pDC->LineTo( iPos , iSLineEnd   );		
			}
		}
		break;
	}


// 	for( int iSayac = 0 ; iSayac <= 100; iSayac++ ) 
// 	{
// 		iPos = iStartPos + ( iSayac * (rectRuler.right - CSCAN_SCALE_X_LENGTH) /100 );
// 		iPos -= 0;
// 		iPos += CSCAN_SCALE_X_LENGTH;
// 		if (iSayac % 10 == 0)
// 		{
// 			rectText.left     = iPos;
// 			rectText.right    = iPos + 60;
// 			switch(ScanNum)
// 			{
// 			case 1:
// 				str.Format(_T("%.1f"), fStart+(fStep*iSayac/10));
// 				break;
// 			case 2:
// 				str.Format(_T("%.1f"), fStart+(fStep*iSayac / 10));
// 				break;
// 			case 3:
// 			    str.Format(_T("%d"), (int)(fStart + ( fStep*iSayac / 10)));	
// 				break;
// 			}
// 			pDC->DrawText( str , &rectText , DT_LEFT /*| DT_VCENTER | DT_SINGLELINE*/ );
// 		}
// 		if( iPos > rectRuler.right )
// 			break;
// 		if( iSel == 0 ) 
// 		{
// 			if (0 == iSayac)
// 			{
// 				CPen newPen; 
// 				newPen.CreatePen(PS_SOLID,1,RGB(0,0,0)); 
// 				CPen* poldpen=pDC->SelectObject(&newPen); 
// 				pDC->MoveTo(  iPos , 0 );
// 				pDC->LineTo(  iPos , rectRuler.bottom   );
// 				pDC->MoveTo(0,rectRuler.bottom );
// 				pDC->LineTo(iPos,rectRuler.bottom);
// 				pDC->SelectObject(poldpen);
// 				newPen.DeleteObject();
// 			}
// 			else
// 			{
// 				pDC->MoveTo(  iPos , iLLineStart );
// 				pDC->LineTo(  iPos , iLLineEnd   );		
// 			}	
// 		}
// 		else
// 			if( iSel == 5 ) {
// 				pDC->MoveTo( iPos , iMLineStart );
// 				pDC->LineTo( iPos , iMLineEnd   );
// 			}
// 			else {
// 				pDC->MoveTo( iPos , iSLineStart );
// 				pDC->LineTo( iPos , iSLineEnd   );
// 			}
// 			iSel++;
// 			if( iSel == 10 )
// 				iSel = 0;
// 	};

	pDC->SelectObject( pOldFont );
	font.DeleteObject();
	pDC->SelectObject( pOldPen );
	pen.DeleteObject();
	////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::DrawHRuler"),"Leave");
	return TRUE;
}

/*************************************************************************
* �������ƣ�
*   DrawVRuler����
* ����:
*  CDC *pDC,        [in]: ��ͼ�豸����ָ��
*  float fStart     [in]: ����̶ȳ�����
*  float fEnd       [in]: ���̶�ֵ
*  float fEnd       [in]:  ��С�̶�ֵ   
*  float fStep      [in]: �̶Ȳ���
*  int direction    [in]: �̶ȳߵ�λ��
*  BOOL bReverse    [in]: ���̶ȳ߿���
*  int ScanNum      [in]: ɨͼ���
* ����ֵ:
*   BOOL
* ˵��:
*   ���ݱ�߿̶�  
* ��ע��
*   ������Դ��ԭ����DrawScale()���������ֲ���Ŀǰû���õ��������Ժ���Ӹ��Ĺ���
************************************************************************/
BOOL CRulerWnd::DrawVRuler(CDC *pDC,float fStart, float fEnd, float fStep, int direction, BOOL bReverse,int ScanNum)
{
	////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::DrawVRuler"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T("CRulerWnd::DrawHRuler pDC"), _T("����"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CRulerWnd::DrawVRuler"),"Leave");
		return FALSE;
	}
	// yachang.huang  add end on 2012-11-02
	CRect rectRuler;
	GetClientRect( &rectRuler );

	CPen pen;
	pen.CreatePen( PS_SOLID , 1 , m_clrMilimeterLineColor );
	CPen *pOldPen = pDC->SelectObject( &pen );
	int iStartPos = 0;  

	LOGFONT lf;

	memset( &lf , 0 , sizeof( lf ) );

	CFont font;
	font.CreatePointFont(70 , _T( "Arial" ) );
	font.GetLogFont( &lf );
	font.DeleteObject();

	CRect rectText;

	int iLLineStart , iLLineEnd;
	int iMLineStart , iMLineEnd;
	int iSLineStart , iSLineEnd;

	if( m_dwStyle & RWSTYLE_RIGHTALIGN ) 
	{
		lf.lfOrientation = 900;
		lf.lfEscapement  = 900;

		rectText.left  = rectRuler.right - 40;

		iLLineStart = rectRuler.right - 9;
		iLLineEnd	 = rectRuler.right - 2;

		iMLineStart = rectRuler.right - 6;
		iMLineEnd	 = rectRuler.right - 2;

		iSLineStart = rectRuler.right - 4;
		iSLineEnd	 = rectRuler.right - 2;

	}
	else if( m_dwStyle & RWSTYLE_LEFTALIGN ) 
	{
		lf.lfOrientation = -900;
		lf.lfEscapement  = -900;

		rectText.left  = rectRuler.left + 7;

		iLLineStart = rectRuler.left  + 2;
		iLLineEnd	 = rectRuler.left  + 9;

		iMLineStart = rectRuler.left  + 2;
		iMLineEnd	 = rectRuler.left  + 6;

		iSLineStart = rectRuler.left + 2;
		iSLineEnd	 = rectRuler.left + 4;
	}

	CFont *pOldFont = pDC->SelectObject( &font );

	CString str;

	int iPos;
	int iSel = 0;

	pDC->SetBkMode( TRANSPARENT );
	pDC->SetTextColor( m_clrTextColor );

	pDC->MoveTo(0 , 0 );
	pDC->LineTo(CSCAN_SCALE_X_LENGTH ,  0);
	switch(ScanNum)
	{
	case 1:
		for( int iSayac = 0 ; iSayac <= 100; iSayac++ ) 
		{
			iPos = iStartPos + rectRuler.bottom - ( iSayac * rectRuler.bottom/100);
			str.Format( _T( "%ld" ) , iSayac / 1 );
			iPos -= 0;
			if( iPos < rectRuler.top ) 
			{
				iSel++;
				if( iSel == 20 )
					iSel = 0;
				continue;
			}
			if( iPos > rectRuler.bottom )
				break;

			if( iSel == 0 ) 
			{
				if( m_dwStyle & RWSTYLE_LEFTALIGN )
					rectText.top    = iPos - 2;
				else
					rectText.top    = iPos + 3;
				rectText.top -= 16;
				rectText.bottom = rectText.top + 20;
				rectText.right = rectText.left + 30; 
				pDC->DrawText( str , &rectText , DT_RIGHT );
				pDC->MoveTo( iLLineStart     , iPos );
				pDC->LineTo( iLLineEnd       , iPos );
			}
			else if( iSel == 10 ) 
			{
				pDC->MoveTo( iMLineStart      , iPos );
				pDC->LineTo( iMLineEnd        , iPos );
			}
			else if(iSel % 2 == 0)
			{
				pDC->MoveTo( iSLineStart     , iPos );
				pDC->LineTo( iSLineEnd       , iPos );
			}
			iSel++;
			if( iSel == 10 )
				iSel = 0;
			
		}
		break;
	case 2:
		for( int iSayac = 0 ; iSayac <= 100; iSayac++ ) 
		{
			iPos = iStartPos + ( iSayac * rectRuler.bottom/100);
			str.Format(_T("%d"), (int)(fStart+(fStep*iSayac / 10)));
			iPos -= 0;
			if( iPos < rectRuler.top ) 
			{
				iSel++;
				if( iSel == 20 )
					iSel = 0;
				continue;
			}
			if( iPos > rectRuler.bottom )
				break;

			if( iSel == 0 ) 
			{
				if( m_dwStyle & RWSTYLE_LEFTALIGN )
					rectText.top    = iPos - 2;
				else
					rectText.top    = iPos + 3;
				rectText.top -= 16;
				rectText.bottom = rectText.top + 20;
				rectText.right = rectText.left + 30; 
				pDC->DrawText( str , &rectText , DT_RIGHT );
				pDC->MoveTo( iLLineStart     , iPos );
				pDC->LineTo( iLLineEnd       , iPos );
			}
			else if( iSel == 10 ) 
			{
				pDC->MoveTo( iMLineStart      , iPos );
				pDC->LineTo( iMLineEnd        , iPos );
			}
			else if(iSel % 2 == 0)
			{
				pDC->MoveTo( iSLineStart     , iPos );
				pDC->LineTo( iSLineEnd       , iPos );
			}
			iSel++;
			if( iSel == 10 )
				iSel = 0;
		}
		break;
	case 3:
		int nStart = (int)fStart;
		int nEnd = (int)(fStart +  fStep*10);
		if (nEnd == 360 )
		{
			m_nCStep = (nEnd - nStart)/100;    
		}
		else
		{
			m_nCStep = (m_pCScanView->m_fZuobiaoXmax - m_pCScanView->m_fZuobiaoXmin)/100;
		}

		if (m_nCStep < 3)
		{
			m_nCStep = 2;
		}
		else
		{
			m_nCStep = (m_nCStep/5 + 1)*5;
		}
		;
		int nLinestart = (nStart/m_nCStep + 1)*m_nCStep;
		if (nStart == 0)
		{
			nLinestart = 0;
		}
        rectText.left -= 7;
		for (int i = nLinestart;i < nEnd; i+=m_nCStep)
		{
			if (i %m_nCStep == 0)
			{
				iPos = iStartPos + rectRuler.bottom - (rectRuler.bottom*(i - nStart))/(nEnd - nStart);
				if (i % (m_nCStep*5) == 0)
				{
					pDC->MoveTo(iMLineStart,iPos);
					pDC->LineTo(iMLineEnd,iPos);	
					if (i % (m_nCStep*10) == 0)
					{
						pDC->MoveTo(iLLineStart,iPos);
						pDC->LineTo( iLLineEnd,iPos);
						if( m_dwStyle & RWSTYLE_LEFTALIGN )
							rectText.top  = iPos - 2;
						else
							rectText.top  = iPos + 3;
						rectText.top -= 16;
						rectText.bottom = rectText.top + 20;
						rectText.right = rectText.left + 33;
						str.Format(_T("%d"), i);
						pDC->DrawText( str , &rectText , DT_RIGHT /*| DT_VCENTER | DT_SINGLELINE*/ );
						continue;
					}
				}
				pDC->MoveTo(iSLineStart,iPos);
				pDC->LineTo( iSLineEnd,iPos );	
	            if ((rectRuler.bottom*m_nCStep)/(nEnd - nStart) > rectRuler.bottom/10 )
				{
					if( m_dwStyle & RWSTYLE_LEFTALIGN )
						rectText.top  = iPos - 2; 
					else
						rectText.top  = iPos + 3;
					rectText.top -= 16;
					rectText.bottom = rectText.top + 20;
					rectText.right = rectText.left + 33;
					str.Format(_T("%d"), i);
					pDC->DrawText( str , &rectText , DT_RIGHT /*| DT_VCENTER | DT_SINGLELINE*/ );

				}
			}
		}
		break;
	}



// 	//���̶ȣ�д����
// 	for(int iSayac = 0;iSayac <= 100; iSayac++ ) 
// 	{
// 		switch(ScanNum)
// 		{
// 		case 1:
// 			iPos = iStartPos + rectRuler.bottom - ( iSayac * rectRuler.bottom/100);
// 		    str.Format( _T( "%ld" ) , iSayac / 1 );
// 			break;
// 		case 2:
// 			iPos = iStartPos + ( iSayac * rectRuler.bottom/100);
// 			str.Format(_T("%.1f"), fStart+(fStep*iSayac / 10));
// 			break;
// 		case 3:
// 			iPos = iStartPos + rectRuler.bottom - ( iSayac * rectRuler.bottom/100);
// 			int nYData = static_cast<int>(fStart+(fStep * iSayac / 10));
// 			str.Format(_T("%d"), nYData);
// 			break;
// 		}
// 		iPos -= 0;
// 		if( iPos < rectRuler.top ) 
// 		{
// 			iSel++;
// 			if( iSel == 20 )
// 				iSel = 0;
// 			continue;
// 		}
// 		if( iPos > rectRuler.bottom )
// 			break;
// 
// 		if( iSel == 0 ) 
// 		{
// 			if( m_dwStyle & RWSTYLE_LEFTALIGN )
// 				rectText.top    = iPos - 2;
// 			else
// 				rectText.top    = iPos + 3;
// 			rectText.top -= 16;
// 			rectText.bottom = rectText.top + 20;
// 			rectText.right = rectText.left + 30; 
// 			pDC->DrawText( str , &rectText , DT_RIGHT );
// 			pDC->MoveTo( iLLineStart     , iPos );
// 			pDC->LineTo( iLLineEnd       , iPos );
// 		}
// 		else if( iSel == 10 ) 
// 		{
// 			pDC->MoveTo( iMLineStart      , iPos );
// 			pDC->LineTo( iMLineEnd        , iPos );
// 		}
// 		else if(iSel % 2 == 0)
// 		{
// 			pDC->MoveTo( iSLineStart     , iPos );
// 			pDC->LineTo( iSLineEnd       , iPos );
// 		}
// 		iSel++;
// 		if( iSel == 10 )
// 			iSel = 0;
// 	};
	pDC->SelectObject( pOldFont );
	font.DeleteObject();
	pDC->SelectObject( pOldPen );
	pen.DeleteObject();
	////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::DrawVRuler"),"Leave");
	return TRUE;
}



//��������
BOOL CRulerWnd::SetStyle( DWORD dwStyle ) 
{ 
  ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::SetStyle"),"Enter");
  m_dwStyle = dwStyle;
  ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::SetStyle"),"Leave");
  return TRUE;

}

BOOL CRulerWnd::SetBackGroundColor( COLORREF clr ) 
{ 
  ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::SetBackGroundColor"),"Enter");
  m_clrBackGround = clr;
  ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::SetBackGroundColor"),"Leave");
  return TRUE;
}

BOOL CRulerWnd::SetMilimeterLineColor( COLORREF clr ) 
{ 
  ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::SetMilimeterLineColor"),"Enter");
  m_clrMilimeterLineColor = clr;
  ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::SetMilimeterLineColor"),"Leave");
  return TRUE;
}

BOOL CRulerWnd::SetTextColor( COLORREF clr ) 
{ 
  ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::SetTextColor"),"Enter");
  m_clrTextColor = clr; 
  ////LOG4CXX_INFO(log4cxx::Logger::getLogger("CRulerWnd::SetTextColor"),"Leave");
  return TRUE;
}












