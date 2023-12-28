// PaletteDog.cpp : implementation file
//

#include "stdafx.h"
#include "PaletteDog.h"
#include "PaletteDlg.h"


// CPaletteDog

CPaletteDog::CPaletteDog()
{
}

CPaletteDog::~CPaletteDog()
{
}


// CPaletteDog member functions

// void CPaletteDog::PlatteConfig()
// {
// 	CPaletteDlg dlg;
// 	if (dlg.DoModal() == IDOK)
// 	{
// 	}
// }
BOOL CPaletteDog::GetPlatteData(PLATTE * array, int& nSize, vector<int> &ColorNums, COLORREF& clrBack1, COLORREF& clrBack2)
{
	CPaletteDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		dlg.OnSequence();
		dlg.OnCountGradualColor();
		vector<float>::iterator iter = dlg.m_pStaticDlg->m_vPercent.begin();
		int nIndex = 0;
		int nMainColorNum = dlg.m_nMainColorNum;
		clrBack1 = dlg.m_clrBackground;
		clrBack2 = dlg.m_clrBottom;
		for (int i = 0; i < dlg.m_pStaticDlg->m_vMClrMain.size(); i++)
		{
			ColorNums.push_back(dlg.m_pStaticDlg->m_vMClrMain[i].nGradualColor);
		}
		for (;iter != dlg.m_pStaticDlg->m_vPercent.end() - 1; iter++)
		{
			array[nIndex].color = dlg.m_pStaticDlg->m_vColor[nIndex];
			array[nIndex].fMaxPercent = dlg.m_pStaticDlg->m_vPercent[nIndex];
			array[nIndex].fMinPercent = dlg.m_pStaticDlg->m_vPercent[nIndex + 1];
			nIndex++;
		}
		array[nIndex].color = dlg.m_pStaticDlg->m_vColor[nIndex];
		array[nIndex].fMaxPercent = dlg.m_pStaticDlg->m_vPercent[nIndex];
		array[nIndex].fMinPercent = -1;
		nSize = nIndex;
	}
	return dlg.m_bPlatte;
}