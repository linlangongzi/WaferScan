// TailDecreasePipe.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "TailDecreasePipeParam.h"
#include "TailDecreasePipeSketch.h"
#include "TailDecreasePipeUnfold.h"
#include "TailDecreasePipe.h"


// CTailDecreasePipe

IMPLEMENT_DYNAMIC(CTailDecreasePipe, CPropertySheet)

CTailDecreasePipe::CTailDecreasePipe(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_Param);
	AddPage(&m_Sketch);
	AddPage(&m_Unfold);
	this->m_psh.dwFlags |= PSH_NOAPPLYNOW;
}

CTailDecreasePipe::CTailDecreasePipe(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_Param);
	AddPage(&m_Sketch);
	AddPage(&m_Unfold);
	this->m_psh.dwFlags |= PSH_NOAPPLYNOW;
}

CTailDecreasePipe::~CTailDecreasePipe()
{
}


BEGIN_MESSAGE_MAP(CTailDecreasePipe, CPropertySheet)
END_MESSAGE_MAP()


// CTailDecreasePipe message handlers
