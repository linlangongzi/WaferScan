// HanHeRateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "HanHeRateDlg.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
#include "CScanParamDockBar.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "ExButton.h"
#include "SysParam.h"
#include "MechanicalControl.h"
#include "MechanicalControlPanel.h"
#include "CScanDlgBar.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
// CHanHeRateDlg dialog

IMPLEMENT_DYNAMIC(CHanHeRateDlg, CDialog)

CHanHeRateDlg::CHanHeRateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHanHeRateDlg::IDD, pParent)
{

}

CHanHeRateDlg::~CHanHeRateDlg()
{
}

void CHanHeRateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHanHeRateDlg, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


/** 
* 画焊合率
* @param                                              
* @return    void
* @version   1.0
* @date		 2012-04-24
* @author	 wangbingfu
* @modify
*/
void CHanHeRateDlg::DrawColorRate()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::DrawColorRate"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

//	CCScanParamPanel* pCScanParam = (CCScanParamPanel*)GetParent();
	CCScanParamPanel* pCScanParam = m_pParentWnd;
	CCScanView* pCScanView = NULL;
	if(pCScanParam == pFrame->m_wndCScanParamDockBar.m_pCScanParamView)
	{
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	}
	else if(pCScanParam == pFrame->m_wndCScanParamDockBar.m_pCScanParamView2)
	{
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
	}
	else
	{
		;//
	}
	CWnd *wnd = GetDlgItem(IDC_STATIC_HANHE_STATI); // 获取显示控件的句柄
	// Song Chenguang modified start on 2012-11-05
	CDC *pDC = wnd->GetDC();
	if (pDC == NULL)
	{
		MessageBox(_T("CHanHeRateDlg::DrawColorRate IDC_STATIC_HANHE_STATI GetDC()失败！"), _T("警告"), MB_OK|MB_ICONERROR);

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CHanHeRateDlg::DrawColorRate"),"Leave");
		return;
	}
	// Song Chenguang modified end on 2012-11-05
	int nRectLength = 32;
	int nRectHeight = 20;
	int nClrLength = 600;	// machao add on 2013-09-5

// machao delete start on 2013-09-5
//	pDC->SetViewportOrg(10, 150);

// 	CPen cpen;
// 	cpen.CreatePen(PS_SOLID,1,RGB(0,0,0));//黑色 画线用
// 	pDC->SelectObject(&cpen);
// 	for (int i = 0; i < 20; i++)
// 	{
// 		pDC->Rectangle(i*nRectLength,0,nRectLength+i*nRectLength,nRectHeight);
// 	}
// 
// 	pDC->SetViewportOrg(10, 152);
// 	CString string;//字符，显示数字用
// 
// 	for(int i = 0; i < 20; i++)
// 	{
// 		string.Format (_T ("%.1f"), pCScanView->m_fHanHeRate[i]);
// 		pDC->TextOut (i*nRectLength, 0, string);
// 	}

// 	string = _T("%");
// 	pDC->SetBkMode(TRANSPARENT);
// 	pDC->TextOut (20*nRectLength + 5, 0, string);

	//画出调色板
	pDC->SetViewportOrg(51, 148);	// machao modify on 2013-09-5
	CBrush* pOldBrush;
	if (pCScanParam-> m_nSurfceColorMode == 0)
	{
		for (int i = 0;i < 16; i++)
		{
			int nGrayValue = (15-i)*17;
			CBrush	brush;
			brush.CreateSolidBrush(RGB(nGrayValue, nGrayValue, nGrayValue));
			// Song Chenguang modified start on 2012-11-05 
			if(i == 0)
			{
				pOldBrush = pDC->SelectObject(&brush);
			}
			else
			{
				pDC->SelectObject(&brush);
			}
			// Song Chenguang modified end on 2012-11-05
			pDC->Rectangle(i*nRectLength, 0, nRectLength+i*nRectLength, nRectHeight);

		}
	} 
	else
	{
		// machao modify start on2013-09-5
		if (pCScanView->m_nColorDefine == 0)
		{
			//machao modify start on 2013-07-16
			// machao modify start on 2013-11-19
			int nClrStartNum = (int)(27.5 * pCScanParam->m_nSize / 127.5);
			if (100 > pCScanParam->m_plaPalette[nClrStartNum].fMaxPercent)
			{
				nClrStartNum--;
			}
			int nClrNum = pCScanParam->m_nSize - nClrStartNum;
			// machao modify end on 2013-11-19
			float fRectLength = (float)nClrLength / nClrNum;
			for (int i = 0; i < nClrNum; i++)
			{
				CBrush brush(pCScanParam->m_plaPalette[nClrStartNum + i].color);
				// Song Chenguang modified start on 2012-11-05 
				if(i == 0)
				{
					pOldBrush = pDC->SelectObject(&brush);
				}
				else
				{
					pDC->SelectObject(&brush);
				}
				// Song Chenguang modified end on 2012-11-05
				pDC->FillRect(CRect(i*fRectLength, 0, (i + 1)*fRectLength, nRectHeight),&brush);
			}
		}
		else if(pCScanView->m_nColorDefine == 1)
		{
			for (int i = 0; i < 20; i++)
			{
				float fRectLength = (float)nClrLength / 20;
				CBrush brush(pCScanView->m_clrPlatte[19 - i]);
				// Song Chenguang modified start on 2012-11-05 
				if(i == 0)
				{
					pOldBrush = pDC->SelectObject(&brush);
				}
				else
				{
					pDC->SelectObject(&brush);
				}
				// Song Chenguang modified end on 2012-11-05
				pDC->FillRect(CRect(i*fRectLength, 0, (i + 1)*fRectLength, nRectHeight),&brush);
			}
// 			CBrush brush(pCScanView->m_clrPlatte[i]);
// 			// Song Chenguang modified start on 2012-11-05 
// 			if(i == 0)
// 			{
// 				pOldBrush = pDC->SelectObject(&brush);
// 			}
// 			else
// 			{
// 				pDC->SelectObject(&brush);
// 			}
// 			// Song Chenguang modified end on 2012-11-05
// 			pDC->Rectangle(i*nRectLength, 0, nRectLength+i*nRectLength, nRectHeight);
			//machao modify end on 2013-07-16
		}
		// machao modify end on 2013-09-5
		// 底色 无伤
		// 		CBrush brush(RGB(245, 245, 245));
		// 		pOldBrush = pDC->SelectObject(&brush);
		// 		pDC->Rectangle(20*nRectLength, 0, nRectLength + 20*nRectLength, nRectHeight);
	}
	
	float fHanHeRate = 0;
/*	for (int i = 0; i < 20; i++)
	{	
		fHanHeRate += pCScanView->m_fHanHeRate[i];
	}

	pDC->SetBkMode(TRANSPARENT);
	float fFlawArea = pCScanView->m_SelArea * fHanHeRate / 100;
	CString strFlawArea;
	strFlawArea.Format (_T ("未焊合面积 = %.1f mm^2"), fFlawArea);
	pDC->TextOut (15 * nRectLength, 65, strFlawArea);

	fHanHeRate = 100 - fHanHeRate;
	CString strRate;
	strRate.Format (_T ("焊合率 = %.1f%%"), fHanHeRate);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut (15 * nRectLength, 50, strRate);
*/
	pDC->SetBkMode(TRANSPARENT);
	CString strFlawArea;
	strFlawArea.Format (_T ("未焊合面积 = %.1f mm^2"), pCScanView->m_dbFlawArea);
//	strFlawArea.Format (_T ("未焊合面积 = %.1f mm^2"), (pCScanView->m_SelArea - pCScanView->m_dbFlawArea));
	pDC->TextOut (15 * nRectLength, 45, strFlawArea);

	fHanHeRate = 100 - pCScanView->m_dbFlawArea / pCScanView->m_SelArea * 100;
//	fHanHeRate = pCScanView->m_dbFlawArea / pCScanView->m_SelArea * 100;	// modified by Song Chenguang 2012-11-24
	if(fHanHeRate < 1E-6)
	{
		fHanHeRate = 0;
	}
	CString strRate;
	strRate.Format (_T ("焊合率 = %.2f%%"), fHanHeRate);
	pDC->SetBkMode(TRANSPARENT);
	pDC->TextOut (15 * nRectLength, 28, strRate);

	pDC->SelectObject(pOldBrush);
	wnd->ReleaseDC(pDC);	// Song Chenguang modified on 2012-11-05

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::DrawColorRate"),"Leave");
}
// CHanHeRateDlg message handlers

BOOL CHanHeRateDlg::OnInitDialog()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::OnInitDialog"),"Enter");

	CDialog::OnInitDialog();
	DrawColorRate();
	// TODO:  Add extra initialization here

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::OnInitDialog"),"Leave");

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
// 画柱状图
void CHanHeRateDlg::DrawHistogram()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::DrawHistogram"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
//	CCScanParamPanel* pCScanParam = (CCScanParamPanel*)GetParent();
	CCScanParamPanel* pCScanParam = m_pParentWnd;
	CCScanView* pCScanView = NULL;
	if(pCScanParam == pFrame->m_wndCScanParamDockBar.m_pCScanParamView)
	{
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	}
	else if(pCScanParam == pFrame->m_wndCScanParamDockBar.m_pCScanParamView2)
	{
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
	}
	else
	{
		;//
	}

	CWnd *wnd = GetDlgItem(IDC_STATIC_HANHE_STATI); // 获取显示控件的句柄

	// Song Chenguang modified start on 2012-11-05
	CDC *pDC = wnd->GetDC();
	if (pDC == NULL)
	{
		MessageBox(_T("CHanHeRateDlg::DrawHistogram IDC_STATIC_HANHE_STATI GetDC失败！"), _T("警告"), MB_OK|MB_ICONERROR);

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CHanHeRateDlg::DrawHistogram"),"Leave");

		return;
	}
	// Song Chenguang modified end on 2012-11-05
	int nRectLength = 600;
	pDC->SetViewportOrg(20, 20);
	DrawVRuler(pDC,CRect(30,0,30,125),0,100,20,1,1);	// machao add on 2013-09-5
	// machao modify start on 2013-09-5
	pDC->SetViewportOrg(50, 20);
	CRect rectALLHistogram(0, 0, nRectLength + 2, 125);
	// machao modify end on 2013-09-5

	CPen pen;
	//矩形框
	pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	CPen *pOldPen = pDC->SelectObject(&pen);
	pDC->Rectangle(&rectALLHistogram);

	//网格虚线
	CPen penGrid;
	penGrid.CreatePen(PS_DOT ,1,RGB(192,192,192));
	pDC->SelectObject(&penGrid);
	for(int i = 1; i < 10; i++)
	{
		pDC->MoveTo(rectALLHistogram.left + 1, i * 12.5);	// machao modify on 2013-09-5
		pDC->LineTo(rectALLHistogram.right - 1, i * 12.5);	// machao modify on 2013-09-5
	}
	pDC->SelectObject(pOldPen);

	//柱状图
	CBrush* pOldBrush;
	// machao modify start on 2013-09-5
	pDC->SetViewportOrg(51,19);
	if (pCScanView->m_nColorDefine == 0)
	{
		// machao modify start on 2013-11-19
		int nClrStartNum = (int)(27.5 * pCScanParam->m_nSize / 127.5);
		if (100 > pCScanParam->m_plaPalette[nClrStartNum].fMaxPercent)
		{
			nClrStartNum--;
		}
		int nClrNum = pCScanParam->m_nSize - nClrStartNum;
		// machao modify end on 2013-11-19
		float fRectLength = (float)nRectLength / nClrNum;
		//machao modify start on 2013-07-16
		for (int i = 0; i < nClrNum; i++)
		{
			CBrush brush(pCScanParam->m_plaPalette[nClrStartNum + i].color);
			// Song Chenguang modified start on 2012-11-05 
			if(i == 0)
			{
				pOldBrush = pDC->SelectObject(&brush);
			}
			else
			{
				pDC->SelectObject(&brush);
			}
			// Song Chenguang modified end on 2012-11-05
			pDC->FillRect(CRect(i*fRectLength, rectALLHistogram.bottom-(int)(pCScanView->m_pfCustomHanheRate[i]), (i + 1)*fRectLength, rectALLHistogram.bottom),&brush);
//			pDC->Rectangle(i*fRectLength + 6, rectALLHistogram.bottom-(int)(pCScanView->m_fHanHeRate[i]), i*nRectLength + 20, rectALLHistogram.bottom);
		}
	}
		// machao modify start on 2013-08-28
//		else if(pCScanView->m_nColorDefine = 0)
	else if(pCScanView->m_nColorDefine = 1)
	{
		float fRectLength = (float)nRectLength / 20;
		// machao modify end on 2013-08-28
		for (int i = 0; i < 20; i++)
		{
			CBrush brush(pCScanView->m_clrPlatte[19 - i]);
			// Song Chenguang modified start on 2012-11-05
			if(i == 0)
			{
				pOldBrush = pDC->SelectObject(&brush);
			}
			else
			{
				pDC->SelectObject(&brush);
			}
			// Song Chenguang modified end on 2012-11-05
			pDC->FillRect(CRect(i*fRectLength, rectALLHistogram.bottom-(int)(pCScanView->m_fHanHeRate[i]), (i + 1)*fRectLength, rectALLHistogram.bottom),&brush);
		}
	// machao modify start on 2013-09-5
// 		CBrush brush(pCScanView->m_clrPlatte[i]);
// 		// Song Chenguang modified start on 2012-11-05
// 		if(i == 0)
// 		{
// 			pOldBrush = pDC->SelectObject(&brush);
// 		}
// 		else
// 		{
// 			pDC->SelectObject(&brush);
// 		}
// 		// Song Chenguang modified end on 2012-11-05
// 		pDC->Rectangle(i*nRectLength + 6, rectALLHistogram.bottom-(int)(pCScanView->m_fHanHeRate[i]), i*nRectLength + 20, rectALLHistogram.bottom);
		//machao modify end on 2013-07-16
	}
	//空白
	// 	CBrush brush(RGB(245, 245, 245));
	// 	pOldBrush = pDC->SelectObject(&brush);
	// 	pDC->Rectangle(20*nRectLength + 6, rectALLHistogram.bottom-(int)(pCScanView->m_fSurfacePixeRate[20]), 20*nRectLength + 20, rectALLHistogram.bottom);
	// 	pDC->SelectObject(pOldBrush);	
	// Song Chenguang modified start on 2012-11-05
	pDC->SelectObject(pOldBrush);
	wnd->ReleaseDC(pDC);	
	// Song Chenguang modified end on 2012-11-05

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::DrawHistogram"),"Leave");
}
void CHanHeRateDlg::OnPaint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::OnPaint"),"Enter");

	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	DrawColorRate();
	DrawHistogram();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::OnPaint"),"Leave");
}

void CHanHeRateDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::OnOK"),"Enter");
	CDialog::OnOK();
	CDialog::DestroyWindow();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::OnOK"),"Leave");
}

void CHanHeRateDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::OnCancel"),"Enter");
	CDialog::OnCancel();
	CDialog::DestroyWindow();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::OnCancel"),"Leave");
}

void CHanHeRateDlg::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::PostNcDestroy"),"Enter");

	CDialog::PostNcDestroy();
	m_pParentWnd->m_pSHanHeRateDlg = NULL;
	delete this;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CHanHeRateDlg::PostNcDestroy"),"Leave");
}

// machao add start on 2013-09-5
BOOL CHanHeRateDlg::DrawVRuler(CDC *pDC,CRect rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection)
{
	////LOG4CXX_INFO(//LOG4CXX::Logger::getLogger("CRulerWnd::DrawVRuler"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (pDC == NULL)
	{
		MessageBox(_T("CRulerWnd::DrawHRuler pDC"), _T("警告"), MB_OK|MB_ICONERROR);
		//LOG4CXX_ERROR(//LOG4CXX::Logger::getLogger("CRulerWnd::DrawVRuler"),"Leave");
		return FALSE;
	}
	// yachang.huang  add end on 2012-11-02

	CPen pen;
	pen.CreatePen( PS_SOLID , 1 , RGB( 0 , 0 , 0 ) );
	CPen *pOldPen = pDC->SelectObject( &pen ); 

	LOGFONT lf;

	memset( &lf , 0 , sizeof( lf ) );

	CFont font;
	font.CreatePointFont(70 , _T( "Arial" ) );
	font.GetLogFont( &lf );
	font.DeleteObject();
	if (nEnd == 20)
	{
		int test = 0;
	}

	CRect rectText;
	int iSayac = 0;

	int iLLineStart , iLLineEnd;
	int iMLineStart , iMLineEnd;
	int iSLineStart , iSLineEnd;

	if( nTextLocation == 1 ) 
	{
		lf.lfOrientation = 900;
		lf.lfEscapement  = 900;

		rectText.right  = rect.right - 10;

		iLLineStart = rect.right - 9;
		iLLineEnd	 = rect.right - 2;

		iMLineStart = rect.right - 6;
		iMLineEnd	 = rect.right - 2;

		iSLineStart = rect.right - 4;
		iSLineEnd	 = rect.right - 2;

	}
	else if( nTextLocation == 0 ) 
	{
		lf.lfOrientation = -900;
		lf.lfEscapement  = -900;

		rectText.left  = rect.left + 10;

		iLLineStart = rect.left  + 2;
		iLLineEnd	 = rect.left  + 9;

		iMLineStart = rect.left  + 2;
		iMLineEnd	 = rect.left  + 6;

		iSLineStart = rect.left + 2;
		iSLineEnd	 = rect.left + 4;
	}
	pDC->FillSolidRect( &rect , RGB(7,165,0) );
	CFont *pOldFont = pDC->SelectObject( &font );

	CString str;

	int iPos;
	int iSel = 0;

	pDC->SetBkMode( TRANSPARENT );
	pDC->SetTextColor( RGB( 0 , 0 , 0 ) );



	bool bXiaoyu = FALSE;
	if (nStep < 10)
	{
		nStart = nStart*100;
		nEnd = nEnd*100;
		nStep = nStep*100;
		bXiaoyu = TRUE;
	}
	nStep = nStep/10;
	if (nStep%5 != 0)
	{
		if (nStep > 3)
		{
			nStep = (nStep/5 + 1)*5;
		}
		else
		{

		}
	}


	int nLinestart = (nStart/nStep + 1)*nStep;
	if (nStart == 0)
	{
		nLinestart = 0;
	}
	for (int i = nLinestart;i < nEnd; i+=nStep)
	{
		if (i %nStep == 0)
		{
			if( nTextDirection == 0 )  
			{
				iPos = rect.top + (rect.Height()*(i - nStart))/(nEnd - nStart);
				rectText.top  = iPos;
			}
			else if(nTextDirection == 1 )
			{
				iPos = rect.bottom - (rect.Height()*(i - nStart))/(nEnd - nStart);
				rectText.top  = iPos - 13;
			}
			rectText.bottom = rectText.top + 20;

			if (i % (nStep*5) == 0)
			{
				pDC->MoveTo(iMLineStart,iPos);
				pDC->LineTo(iMLineEnd,iPos);	
				if (i % (nStep*10) == 0)
				{
					pDC->MoveTo(iLLineStart,iPos);
					pDC->LineTo( iLLineEnd,iPos);
					if( nTextLocation == 0 )
					{
						rectText.right = rectText.left + 43;
					}
					else
					{
						rectText.left = rectText.right - 43;
					}

					if (rectText.top < rect.top)
					{
						rectText.top = rect.top;
					}
					if (rectText.bottom > rect.bottom)
					{
						rectText.bottom = rect.bottom;
					}
					if (bXiaoyu)
					{
						str.Format(_T("%d"), i/100);
					}
					else	
					{
						str.Format(_T("%d"), i);
					}
					if( nTextLocation == 0 )
					{
						pDC->DrawText( str , &rectText , DT_LEFT  );
					}
					else
					{
						pDC->DrawText( str , &rectText , DT_RIGHT );
					}

					continue;
				}
			}
			pDC->MoveTo(iSLineStart,iPos);
			pDC->LineTo( iSLineEnd,iPos );	
			if ((rect.Height()*nStep)/(nEnd - nStart) > rect.Height()/10 )
			{
				if( nTextLocation == 0 )
				{
					rectText.right = rectText.left + 43;
				}
				else
				{
					rectText.left = rectText.right - 43;
				}
				str.Format(_T("%d"), i);   
				if( nTextLocation == 0 )
				{
					pDC->DrawText( str , &rectText , DT_LEFT  );
				}
				else
				{
					pDC->DrawText( str , &rectText , DT_RIGHT );
				}

			}
		}
	}

	pDC->SelectObject( pOldFont );
	font.DeleteObject();
	pDC->SelectObject( pOldPen );
	pen.DeleteObject();
	////LOG4CXX_INFO(//LOG4CXX::Logger::getLogger("CRulerWnd::DrawVRuler"),"Leave");
	return TRUE;
}
// machao add end on 2013-09-5