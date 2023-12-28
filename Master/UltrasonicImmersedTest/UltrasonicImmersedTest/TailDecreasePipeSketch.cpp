// TailDecreasePipeSketch.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "TailDecreasePipeSketch.h"


// TailDecreasePipeSketch dialog

IMPLEMENT_DYNAMIC(CTailDecreasePipeSketch, CPropertyPage)

CTailDecreasePipeSketch::CTailDecreasePipeSketch()
	: CPropertyPage(CTailDecreasePipeSketch::IDD)
	, m_fASketch(96)
	, m_fABSketch(400)
	, m_fBSketch(68)
	, m_fBCSketch(132)
	, m_fCSketch(68)
	, m_fCDSketch(400)
	, m_fDSketch(96)
{

}

CTailDecreasePipeSketch::~CTailDecreasePipeSketch()
{
}

void CTailDecreasePipeSketch::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

  	DDX_Text(pDX, IDC_EDIT_A_SKETCH, m_fASketch);
  	DDX_Text(pDX, IDC_EDIT_AB_SKETCH, m_fABSketch);
  	DDX_Text(pDX, IDC_EDIT_B_SKETCH, m_fBSketch);
  	DDX_Text(pDX, IDC_EDIT_BC_SKETCH, m_fBCSketch);
  	DDX_Text(pDX, IDC_EDIT_C_SKETCH, m_fCSketch);
  	DDX_Text(pDX, IDC_EDIT_CD_SKETCH, m_fCDSketch);
  	DDX_Text(pDX, IDC_EDIT_D_SKETCH, m_fDSketch);
}


BEGIN_MESSAGE_MAP(CTailDecreasePipeSketch, CPropertyPage)
	ON_WM_PAINT()	// machao add on 2013-09-23
END_MESSAGE_MAP()


// TailDecreasePipeSketch message handlers

// machao add start on 2013-09-23
void CTailDecreasePipeSketch::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetDlgItem(IDC_STATIC_SKETCH)->GetClientRect(&rect);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpWeiJianGuanSketch;
	bmpWeiJianGuanSketch.LoadBitmap(IDB_WEIJIANGUAN_SKETCH);
	BITMAP bitmap;
	bmpWeiJianGuanSketch.GetBitmap(&bitmap);
	CBitmap *pbmpOld = dcMem.SelectObject(&bmpWeiJianGuanSketch);
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,
		bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);
}
// machao add end on 2013-09-23