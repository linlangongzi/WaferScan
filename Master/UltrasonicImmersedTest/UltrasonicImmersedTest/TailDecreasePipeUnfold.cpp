// TailDecreasePipeUnfold.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "TailDecreasePipeUnfold.h"


// TailDecreasePipeUnfold dialog

IMPLEMENT_DYNAMIC(CTailDecreasePipeUnfold, CPropertyPage)

CTailDecreasePipeUnfold::CTailDecreasePipeUnfold()
	: CPropertyPage(CTailDecreasePipeUnfold::IDD)
	, m_fLeft1Start(100)
	, m_fLeft1Stop(200)
	, m_fLeft2Start(200)
	, m_fLeft2Stop(250)
	, m_fRight1Start(1160)
	, m_fRight1Stop(1060)
	, m_fRight2Start(1060)
	, m_fRight2Stop(1010)
{

}

CTailDecreasePipeUnfold::~CTailDecreasePipeUnfold()
{
}

void CTailDecreasePipeUnfold::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_LEFT_1_START, m_fLeft1Start);
	DDX_Text(pDX, IDC_EDIT_LEFT_1_STOP, m_fLeft1Stop);
	DDX_Text(pDX, IDC_EDIT_LEFT_2_START, m_fLeft2Start);
	DDX_Text(pDX, IDC_EDIT_LEFT_2_STOP, m_fLeft2Stop);
	DDX_Text(pDX, IDC_EDIT_RIGHT_1_START, m_fRight1Start);
	DDX_Text(pDX, IDC_EDIT_RIGHT_1_STOP, m_fRight1Stop);
	DDX_Text(pDX, IDC_EDIT_RIGHT_2_START, m_fRight2Start);
	DDX_Text(pDX, IDC_EDIT_RIGHT_2_STOP, m_fRight2Stop);
}


BEGIN_MESSAGE_MAP(CTailDecreasePipeUnfold, CPropertyPage)
END_MESSAGE_MAP()


// TailDecreasePipeUnfold message handlers
