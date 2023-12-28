// TailDecreasePipeParam.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "TailDecreasePipeParam.h"


// CTailDecreasePipeParam dialog

IMPLEMENT_DYNAMIC(CTailDecreasePipeParam, CPropertyPage)

CTailDecreasePipeParam::CTailDecreasePipeParam(/*CWnd* pParent *//*=NULL*/)
	: CPropertyPage(CTailDecreasePipeParam::IDD/*, pParent*/)
	, m_f11LayerMin(0)
	, m_f11LayerMax(0)
	, m_f14LayerMin(0)
	, m_f14LayerMax(0)
	, m_f17LayerMin(0)
	, m_f17LayerMax(0)
	, m_f18LayerMin(0)
	, m_f18LayerMax(0)
	, m_fWeiJianW1(40) 
{

}

CTailDecreasePipeParam::~CTailDecreasePipeParam()
{
}

void CTailDecreasePipeParam::DoDataExchange(CDataExchange* pDX)
{
	// machao modify start on 2013-08-8
	//CDialog::DoDataExchange(pDX);
	CPropertyPage::DoDataExchange(pDX);
	// machao modify end on 2013-08-8
	DDX_Text(pDX, IDC_EDIT_11_MIN, m_f11LayerMin);
	DDX_Text(pDX, IDC_EDIT_11_MAX, m_f11LayerMax);
	DDX_Text(pDX, IDC_EDIT_14_MIN, m_f14LayerMin);
	DDX_Text(pDX, IDC_EDIT_14_MAX, m_f14LayerMax);
	DDX_Text(pDX, IDC_EDIT_17_MIN, m_f17LayerMin);
	DDX_Text(pDX, IDC_EDIT_17_MAX, m_f17LayerMax);
	DDX_Text(pDX, IDC_EDIT_18_MIN, m_f18LayerMin);
	DDX_Text(pDX, IDC_EDIT_18_MAX, m_f18LayerMax);
	DDX_Text(pDX, IDC_EDIT_WEIJIAN_W1, m_fWeiJianW1);
}


BEGIN_MESSAGE_MAP(CTailDecreasePipeParam, CPropertyPage)
END_MESSAGE_MAP()


// CTailDecreasePipeParam message handlers
