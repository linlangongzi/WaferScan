// StaticDlg.cpp : implementation file
//

#include "stdafx.h"
#include "StaticDlg.h"
#include "PaletteDlg.h"
#include "CustumEdit.h"
#include "ColorStatic.h"
#include <math.h>


// CStaticDlg dialog

IMPLEMENT_DYNAMIC(CStaticDlg, CDialog)

CStaticDlg::CStaticDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStaticDlg::IDD, pParent)
	, m_nSite(0)
	, m_nRectNum(3)
	, m_hFocusEdit(NULL)
	, m_nChangeEditNum(0)
{
//	m_hFocusEdit = NULL;
	m_clrTop = RGB(255,255,255);
	m_clrEnd = RGB(0,0,0);
//	bScroll = FALSE;
	m_vfPercent.push_back(127.50);
	m_vfPercent.push_back(63.75);
	m_vfPercent.push_back(0.00);
	InitializeMColor();
}

CStaticDlg::~CStaticDlg()
{
}

void CStaticDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStaticDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

BOOL CStaticDlg ::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
 	return FALSE;
}

// CStaticDlg message handlers
BOOL CStaticDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// save the original size
//	GetWindowRect(m_rcOriginalRect);

	// 使用默认的窗口大小
	m_nScrollPos = 0; 
	m_nCurHeight = 0;

	CRect rect(80, 10, 140, 30);
	CRect rectGraduate(150, 10, 210, 30);
	CRect rectStatic(10, 20, 70, 40);

	for (int i=0; i<3; i++)
	{
		CCustumEdit* pEdit = new CCustumEdit;
		pEdit->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
			rect, this, i);
		CString str;
		str.Format(_T("%.2f"), m_vfPercent[i]);
		pEdit->SetWindowText(str);
		rect.OffsetRect(0, 25);

		CCustumEdit* pGraduateEdit = new CCustumEdit;
		pGraduateEdit->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
			rectGraduate, this, 400 + i);
		rectGraduate.OffsetRect(0, 25);

		CColorStatic* pStatic = new CColorStatic;
		BOOL bRet2 = pStatic->Create(NULL, SS_NOTIFY | SS_SUNKEN, 
			rectStatic, this, 800+i);
		pStatic->ShowWindow(true);
		rectStatic.OffsetRect(0, 25);

		if (i == 0)
		{
			pGraduateEdit->SetWindowText(_T("2.11"));
			pStatic->SetBackColor(m_clrTop);
			m_vColor.push_back(m_clrTop);
			pEdit->SetReadOnly(TRUE);
			pGraduateEdit->SetReadOnly(TRUE);
		}
		else if (i != 0)
		{
			pStatic->SetBackColor(m_clrEnd);
			m_vColor.push_back(m_clrEnd);
		}
		if (i == 1)
		{
			pGraduateEdit->SetWindowText(_T("-3.91"));
		}
		if (i == 2)
		{
			pGraduateEdit->SetWindowText(_T("-infinite"));
			pEdit->SetReadOnly(TRUE);
			pGraduateEdit->SetReadOnly(TRUE);
		}
		m_vStatic.push_back(pStatic->GetSafeHwnd());
		m_vEdit.push_back(pEdit->GetSafeHwnd());
		m_vGraduateEdit.push_back(pGraduateEdit->GetSafeHwnd());
	}

	return TRUE;
}

//*******************滚动条初始化*******************
void CStaticDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	m_nCurHeight = cy;

	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_ALL; 
	si.nMin = 0;
	if (20 + 25 * m_nRectNum <= 435)
	{
		si.nMax = 0;
		si.nPage = 0;
	}
	else if (20 + 25 * m_nRectNum > 435)
	{
		si.nMax = 20 + 25 * m_nRectNum;
		si.nPage = cy;
	}
	si.nPos = 0;
	SetScrollInfo(SB_VERT, &si, TRUE);
}

//******************控制滚动条*******************
void CStaticDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nScrollNum;
	int	nMinRange, nMaxRange;
	GetScrollRange(SB_VERT, &nMinRange, &nMaxRange);
	int nMaxPos = nMaxRange - m_nCurHeight;
	//	nScrollNum = GetScrollPos(SB_VERT);
	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		nScrollNum = 25;
		break;
	case SB_LINEUP:
		if (m_nScrollPos <= 0)
			return;
		nScrollNum = -25;
		break;
	case SB_PAGEDOWN:
		if (m_nScrollPos >= nMaxPos)
			return;
		if (m_nScrollPos + 200 > nMaxPos)
		{
			nScrollNum = nMaxPos - m_nScrollPos;
			break;
		}
		nScrollNum = 200;
		break;
	case SB_THUMBTRACK:
 	case SB_THUMBPOSITION:
		nScrollNum = (int)nPos - m_nScrollPos;
		break;

	case SB_PAGEUP:
		if (m_nScrollPos <= 0)
			return;
		if (m_nScrollPos - 200 < 0)
		{
			nScrollNum = -m_nScrollPos;
			break;
		}
		nScrollNum = -200;
		break;

	default:
		return;
	}
	m_nScrollPos += nScrollNum;
	SetScrollPos(SB_VERT,m_nScrollPos,TRUE);
	ScrollWindow(0,-nScrollNum);
	//	Invalidate(TRUE);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

//******************指定位置添加****************
void CStaticDlg::AddItem(const int& n)
{
	if (m_hFocusEdit == NULL)
		return;

	vector< HWND >::iterator iter;
	vector< HWND >::iterator iterGratuade = m_vGraduateEdit.begin();
	int nIndex = 0;
	for (iter = m_vEdit.begin(); iter != m_vEdit.end(); iter++)
	{
		HWND hwnd = *iter;
		HWND hwndGraduate = *iterGratuade;

		if (hwnd == m_hFocusEdit || hwndGraduate == m_hFocusEdit)
		{
			OnMainColorNum(m_vfPercent[nIndex - 1], m_vfPercent[nIndex]);
			m_nSite = nIndex;
			CRect rect(80, 10, 140, 30);
			CRect rectGraduate(150, 10, 210, 30);
			CRect rectStatic(10,20,70,40);

			// 插入n个edit控件
			COLORREF m_Nextcolor = m_vColor[nIndex - 1];
			COLORREF m_color;

			float nR, nG, nB;

			if (nIndex == m_vEdit.size() - 1)
			{
				nR = (float)(GetRValue(m_vColor[nIndex]) - GetRValue(m_vColor[nIndex - 1])) / (n);
				nG = (float)(GetGValue(m_vColor[nIndex]) - GetGValue(m_vColor[nIndex - 1])) / (n);
				nB = (float)(GetBValue(m_vColor[nIndex]) - GetBValue(m_vColor[nIndex - 1])) / (n);
				int i = 0;
				for (; i<n-1; i++)
				{
					int r = int(GetRValue(m_Nextcolor) + (i + 1) * nR);
					int g = int(GetGValue(m_Nextcolor) + (i + 1) * nG);
					int b = int(GetBValue(m_Nextcolor) + (i + 1) * nB);
					m_color = RGB(r, g, b);
					m_vColor.insert(m_vColor.begin() + nIndex + i, m_color);
				}
				m_vColor.insert(m_vColor.begin() + nIndex + i, m_vColor[m_vColor.size() - 1]);
			}
			else if (nIndex != m_vEdit.size() - 1)
			{
				nR = (float)(GetRValue(m_vColor[nIndex]) - GetRValue(m_vColor[nIndex - 1])) / (n + 1);
				nG = (float)(GetGValue(m_vColor[nIndex]) - GetGValue(m_vColor[nIndex - 1])) / (n + 1);
				nB = (float)(GetBValue(m_vColor[nIndex]) - GetBValue(m_vColor[nIndex - 1])) / (n + 1);
				for (int i = 0; i<n; i++)
				{
					int r = int(GetRValue(m_Nextcolor) + (i + 1) * nR);
					int g = int(GetGValue(m_Nextcolor) + (i + 1) * nG);
					int b = int(GetBValue(m_Nextcolor) + (i + 1) * nB);
					m_color = RGB(r, g, b);
					m_vColor.insert(m_vColor.begin() + nIndex + i, m_color);
				}
			}
			float fChange = (m_vfPercent[nIndex - 1] - m_vfPercent[nIndex]) / (n + 1);
			float fPercent = m_vfPercent[nIndex - 1];
			for(int i=0; i<n; i++)
			{
				m_vfPercent.insert(m_vfPercent.begin() + nIndex + i, fPercent - fChange * (i + 1));
				CCustumEdit* pEdit = new CCustumEdit;
				BOOL bRet = pEdit->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
					rect, this, m_vEdit.size());
				CString str;
				str.Format(_T("%.2f"), m_vfPercent[nIndex + i]);
				pEdit->SetWindowText(str);
				m_vEdit.insert(m_vEdit.begin() + nIndex + i, pEdit->GetSafeHwnd());

				CCustumEdit* pGraduateEdit = new CCustumEdit;
				BOOL bRet1 = pGraduateEdit->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
					rectGraduate, this, m_vGraduateEdit.size());
				CString str1;
				float fDb = 0 - 20 * log10f(100 / m_vfPercent[nIndex + i]); 
				str1.Format(_T("%.2f"), fDb);
				pGraduateEdit->SetWindowText(str1);
				m_vGraduateEdit.insert(m_vGraduateEdit.begin() + nIndex + i, pGraduateEdit->GetSafeHwnd());

				CColorStatic* pStatic = new CColorStatic;
				BOOL bRet2 = pStatic->Create(NULL, SS_NOTIFY | SS_SUNKEN, 
					rectStatic, this, 800+i);
				pStatic->ShowWindow(true);
				pStatic->SetBackColor(m_vColor[nIndex + i]);
				m_vStatic.insert(m_vStatic.begin() + nIndex + i, pStatic->GetSafeHwnd());
			}
			break;
		}
		iterGratuade++;
		nIndex++;
	}
}

//****************删除操作*****************
void CStaticDlg::DeleteItem()
{
	if (m_hFocusEdit == NULL)
		return;

	CPaletteDlg* pPattledlg = (CPaletteDlg*)GetParent();
	vector< HWND >::iterator iter = m_vEdit.begin();
	vector< HWND >::iterator iterGraduate = m_vGraduateEdit.begin();
	vector< HWND >::iterator iterStatic = m_vStatic.begin();
	vector< COLORREF >::iterator iterColor = m_vColor.begin();
	vector< int >::iterator itery = pPattledlg->m_vnyColorbar.begin();
	vector< float >::iterator iterPercent = m_vfPercent.begin();

	int nIndex = 0;
	for (; iter != m_vEdit.end(); iter++)
	{
		HWND hwnd = *iter;
		HWND hwndGraduate = *iterGraduate;

		if (hwnd == m_hFocusEdit || hwndGraduate == m_hFocusEdit)
		{
			OnDeleteMainColor(m_vfPercent[nIndex], nIndex);
			BOOL bEdit = FALSE;
			BOOL bGraduateEdit = FALSE;
			if (hwnd == m_hFocusEdit)
			{
				bEdit = TRUE;
			}
			else if (hwndGraduate == m_hFocusEdit)
			{
				bGraduateEdit = TRUE;
			}
			CCustumEdit* pEdit = (CCustumEdit*)CWnd::FromHandle(*iter);
			iter = m_vEdit.erase(m_vEdit.begin() + nIndex);
			//			pEdit->DestroyWindow();//销毁对象
			delete pEdit;
			pEdit = NULL;

			CCustumEdit* pGraduateEdit = (CCustumEdit*)CWnd::FromHandle(*iterGraduate);
			iterGraduate = m_vGraduateEdit.erase(m_vGraduateEdit.begin() + nIndex);
			//			pGraduateEdit->DestroyWindow();
			delete pGraduateEdit;
			pGraduateEdit = NULL;

			CColorStatic* pStatic = (CColorStatic*)CWnd::FromHandle(*iterStatic);
			iterStatic = m_vStatic.erase(m_vStatic.begin() + nIndex);
			//			pStatic->DestroyWindow();
			delete pStatic;
			pStatic = NULL;

			iterColor = m_vColor.erase(m_vColor.begin() + nIndex);

			itery = pPattledlg->m_vnyColorbar.erase(pPattledlg->m_vnyColorbar.begin() + nIndex);
			pPattledlg->ChangeColorRect(nIndex - 1);

			iterPercent = m_vfPercent.erase(m_vfPercent.begin() + nIndex);

			if (bEdit)
			{
				pEdit = (CCustumEdit*)CWnd::FromHandle(*(iter - 1));
				pEdit->SetFocus();
				pEdit->SetSel(0, -1);
			}
			else if(bGraduateEdit)
			{
				pGraduateEdit = (CCustumEdit*)CWnd::FromHandle(*(iterGraduate - 1));
				pGraduateEdit->SetFocus();
				pGraduateEdit->SetSel(0, -1);
			}
			m_nRectNum--;
			LayOutControl();
			LayOutControlGraduate();
			LayOutControlStatic();
			Invalidate(TRUE);
			return;
		}
		iterGraduate++;
		iterStatic++;
		iterColor++;
		nIndex++;
		itery++;
		iterPercent++;
	}
}

//************指定位置添加后控件重新排序**************
void CStaticDlg::LayOutControl()
{
	vector< HWND >::iterator iter;

	CRect rect(80, 10 - GetScrollPos(SB_VERT), 140, 30 - GetScrollPos(SB_VERT));
	for (iter = m_vEdit.begin(); iter != m_vEdit.end(); iter++)
	{
		HWND hwnd = *iter;
		CCustumEdit* pEdit = (CCustumEdit*)CWnd::FromHandle(hwnd);

		pEdit->MoveWindow(&rect);
		rect.OffsetRect(0, 25);
	}
}

void CStaticDlg::LayOutControlGraduate()
{
	vector< HWND >::iterator iter;

	CRect rect(150, 10 - GetScrollPos(SB_VERT), 210, 30 - GetScrollPos(SB_VERT));
	for (iter = m_vGraduateEdit.begin(); iter != m_vGraduateEdit.end(); iter++)
	{
		HWND hwnd = *iter;
		CCustumEdit* pGraduateEdit = (CCustumEdit*)CWnd::FromHandle(hwnd);

		pGraduateEdit->MoveWindow(&rect);
		rect.OffsetRect(0, 25);
	}
}

void CStaticDlg::LayOutControlStatic()
{
	vector< HWND >::iterator iter;

	CRect rect(10, 20 - GetScrollPos(SB_VERT), 70, 40 - GetScrollPos(SB_VERT));
	int nIndex = 0;
	for (iter = m_vStatic.begin(); iter != m_vStatic.end(); iter++)
	{
		HWND hwnd = *iter;
		CColorStatic* pStatic = (CColorStatic*)CWnd::FromHandle(hwnd);

		pStatic->MoveWindow(&rect);
		rect.OffsetRect(0, 25);
		nIndex++;
	}
	SetScrollRange(SB_VERT, 0, 20 + 25 * nIndex, TRUE);
}

void CStaticDlg::DeleteLayoutControl()
{
	vector< HWND >::iterator iter;
	vector< HWND >::iterator iterGraduate = m_vGraduateEdit.begin();
	vector< HWND >::iterator iterStatic = m_vStatic.begin();
	for (iter = m_vEdit.begin(); iter != m_vEdit.end(); iter++)
	{
		CCustumEdit* pEdit = (CCustumEdit*)CWnd::FromHandle(*iter);
		delete pEdit;
		pEdit = NULL;

		CCustumEdit* pGraduateEdit = (CCustumEdit*)CWnd::FromHandle(*iterGraduate);
		delete pGraduateEdit;
		pGraduateEdit = NULL;

		CCustumEdit* pStatic = (CCustumEdit*)CWnd::FromHandle(*iterStatic);
		delete pStatic;
		pStatic = NULL;

		iterGraduate++;
		iterStatic++;
	}
}

void CStaticDlg::OpenLayoutControl()
{
	vector<float>::iterator iter;
	m_nScrollPos = 0;
	int nIndex = 0;
	CRect rect(80, 10, 140, 30);
	CRect rectGraduate(150, 10, 210, 30);
	CRect rectStatic(10, 20, 70, 40);
	for (iter = m_vfPercent.begin(); iter != m_vfPercent.end(); iter++)
	{
		CCustumEdit* pEdit = new CCustumEdit;
		BOOL bRet = pEdit->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
			rect, this, m_vEdit.size());
		CString str;
		str.Format(_T("%.2f"), m_vfPercent[nIndex]);
		pEdit->SetWindowText(str);
		m_vEdit.push_back(pEdit->GetSafeHwnd());

		CCustumEdit* pGraduateEdit = new CCustumEdit;
		BOOL bRet1 = pGraduateEdit->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
			rectGraduate, this, 300 + m_vGraduateEdit.size());
		if (iter != m_vfPercent.end() - 1)
		{
			CString str1;
			float fDb = 0 - 20 * log10f(100 / m_vfPercent[nIndex]); 
			str1.Format(_T("%.2f"), fDb);
			pGraduateEdit->SetWindowText(str1);
		}
		else
		{
			pGraduateEdit->SetWindowText(_T("-infinite"));
		}
		m_vGraduateEdit.push_back(pGraduateEdit->GetSafeHwnd());

		CColorStatic* pStatic = new CColorStatic;
		BOOL bRet2 = pStatic->Create(NULL, SS_NOTIFY | SS_SUNKEN, 
			rectStatic, this, 600 + m_vStatic.size());
		pStatic->ShowWindow(true);
		pStatic->SetBackColor(m_vColor[nIndex]);
		m_vStatic.push_back(pStatic->GetSafeHwnd());
		nIndex++;
		rect.OffsetRect(0, 25);
		rectGraduate.OffsetRect(0, 25);
		rectStatic.OffsetRect(0, 25);
		if (iter == m_vfPercent.begin() || iter == m_vfPercent.end() - 1)
		{
			pEdit->SetReadOnly(TRUE);
			pGraduateEdit->SetReadOnly(TRUE);
		}
	}
	m_nRectNum = nIndex;
	if (20 + 25 * m_nRectNum <= 435)
	{
		SetScrollRange(SB_VERT, 0, 0, TRUE);
	}
	else if (20 + 25 * m_nRectNum > 435)
	{
		SetScrollRange(SB_VERT, 0, 20 + 25 * nIndex, TRUE);
	}
}

//****************鼠标单击static是之变色*******************
void CStaticDlg::ChangeColor()
{
	if (m_hFocusEdit == NULL)
		return;

	vector< HWND >::iterator iter;
	//	CWnd* pWnd = CWnd::GetFocus();
	int nIndex = 0;
	for (iter = m_vStatic.begin(); iter != m_vStatic.end(); iter++)
	{
		HWND hwnd = *iter;

		if (hwnd == m_hFocusEdit)
		{
			CColorStatic* pStatic = (CColorStatic*)CWnd::FromHandle(hwnd);
			if(pStatic->OpenSelectDialog())
			{
				pStatic->SetBackColor(pStatic->m_color);
				m_vColor[nIndex] = pStatic->m_color;
				if (nIndex + 1 != m_vStatic.size())
				{
					CPaletteDlg* pPattledlg = (CPaletteDlg*)GetParent();
					pPattledlg->ChangeColorRect(nIndex);
				}
			}
		}
		nIndex++;
	}
}

void CStaticDlg::OnEnableButton()
{
	CPaletteDlg* pPattledlg = (CPaletteDlg*)GetParent();
	vector<HWND>::iterator iter;
	int nIndex = 1;
	for (iter = m_vEdit.begin(); iter != m_vEdit.end(); iter++)
	{
		if (m_hFocusEdit == NULL)
			return;
		HWND hwnd = *iter;

		if (hwnd == m_hFocusEdit && nIndex != 1)
		{
			pPattledlg->m_WndToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_ADD,TRUE);
			pPattledlg->m_bMenuInsert = TRUE;
//			pPattledlg->m_bToolbarAdd = TRUE;
		}
		if (hwnd == m_hFocusEdit && nIndex == 1)
		{
			pPattledlg->m_WndToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_ADD,FALSE);
			pPattledlg->m_bMenuInsert = FALSE;
//			pPattledlg->m_bToolbarAdd = FALSE;
		}
		if (hwnd == m_hFocusEdit && nIndex != 1 && nIndex != m_vEdit.size())
		{
			pPattledlg->m_WndToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_DELETE,TRUE);
			pPattledlg->m_bMenuClear = TRUE;
//			pPattledlg->m_bToolbarDelete = TRUE;
		}
		if (hwnd == m_hFocusEdit)
		{
			if (nIndex == 1 || nIndex == m_vEdit.size())
			{
				pPattledlg->m_WndToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_DELETE,FALSE);
				pPattledlg->m_bMenuClear = FALSE;
//				pPattledlg->m_bToolbarDelete = FALSE;
			}
		}
		nIndex++;
	}
	nIndex = 1;
	for (iter = m_vGraduateEdit.begin(); iter != m_vGraduateEdit.end(); iter++)
	{
		if (m_hFocusEdit == NULL)
			return;
		HWND hwnd = *iter;

		if (hwnd == m_hFocusEdit && nIndex != 1)
		{
			pPattledlg->m_WndToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_ADD,TRUE);
			pPattledlg->m_bMenuInsert = TRUE;
//			pPattledlg->m_bToolbarAdd = TRUE;
		}
		if(hwnd == m_hFocusEdit && nIndex == 1)
		{
			pPattledlg->m_WndToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_ADD,FALSE);
			pPattledlg->m_bMenuInsert = FALSE;
//			pPattledlg->m_bToolbarAdd = FALSE;
		}
		if (hwnd == m_hFocusEdit && nIndex != 1 && nIndex != m_vGraduateEdit.size())
		{
			pPattledlg->m_WndToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_DELETE,TRUE);
			pPattledlg->m_bMenuClear = TRUE;
//			pPattledlg->m_bToolbarDelete = TRUE;
		}
		if(hwnd == m_hFocusEdit)
		{
			if(nIndex == 1 || nIndex == m_vGraduateEdit.size())
			{
				pPattledlg->m_WndToolBar.GetToolBarCtrl().EnableButton(ID_BUTTON_DELETE,FALSE);
				pPattledlg->m_bMenuClear = FALSE;
//				pPattledlg->m_bToolbarDelete = FALSE;
			}
		}
		nIndex++;
	}
}

void CStaticDlg::OnMainColorNum(float fAddTop, float fAddBottom)
{
	if (!OnIntoClr(fAddTop))
	{
		MCOLORR mclr;
		mclr.fPercent = fAddTop;
		mclr.nGradualColor = 0;
		m_vmClrMain.push_back(mclr);
	}
	if (!OnIntoClr(fAddBottom))
	{
		MCOLORR mclr;
		mclr.fPercent = fAddBottom;
		mclr.nGradualColor = 0;
		m_vmClrMain.push_back(mclr);
	}
}

BOOL CStaticDlg::OnIntoClr(float fPer)
{
	int nIndex = 0;
	vector<MCOLORR>::iterator iter;
	for (iter = m_vmClrMain.begin(); iter != m_vmClrMain.end(); iter++)
	{
		if (fPer == m_vmClrMain[nIndex].fPercent)
		{
			return TRUE;
		}
		nIndex++;
	}
	return FALSE;
}

void CStaticDlg::OnDeleteMainColor(float fDelete, int nNum)
{
	int nIndex = 0;
	int n = 1;
	vector<MCOLORR>::iterator iter;
	for (iter = m_vmClrMain.begin(); iter != m_vmClrMain.end(); iter++)
	{
		if (fDelete == m_vmClrMain[nIndex].fPercent)
		{
			nNum += 2;
			if (m_vfPercent.begin() + nNum == m_vfPercent.end())
			{
				m_vmClrMain.erase(m_vmClrMain.begin() + nIndex);
				break;
			}
			m_vmClrMain[nIndex].fPercent = m_vfPercent[nNum - 1];
			m_vmClrMain[nIndex].nGradualColor = 0;
		}
		nIndex++;
	}
}

void CStaticDlg::InitializeMColor()
{
	MCOLORR mclr;
	for (int i = 0; i < 3; i++)
	{
		mclr.fPercent = m_vfPercent[i];
		mclr.nGradualColor = 0;
		m_vmClrMain.push_back(mclr);
	}
}

// machao add start on 2013-09-2
// void CStaticDlg::ChangeEditValue()
// {
// 	vector< HWND >::iterator iter;
// 	vector< HWND >::iterator iterGratuade = m_vGraduateEdit.begin();
// 	int nIndex = 0;
// 	for (iter = m_vEdit.begin(); iter != m_vEdit.end(); iter++)
// 	{
// 		HWND hwnd = *iter;
// 		HWND hwndGraduate = *iterGratuade;
// 
// 		if (hwnd == m_hFocusEdit || hwndGraduate == m_hFocusEdit)
// 		{
// 			if (m_nChangeEditNum > 0)
// 			{
// 				
// 			}
// 			m_nChangeEditNum = nIndex;
// 			break;
// 		}
// 		nIndex++;
// 	}
// }
// machao add end on 2013-09-2