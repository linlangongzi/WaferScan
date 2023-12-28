// ListCtrlEditable.cpp : 实现文件
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "ListCtrlEditable.h"
#include ".\listctrleditable.h"

// CComboxNoBorder

IMPLEMENT_DYNAMIC(CComboxNoBorder, CComboBox)
CComboxNoBorder::CComboxNoBorder()
{
}

CComboxNoBorder::~CComboxNoBorder()
{
}


BEGIN_MESSAGE_MAP(CComboxNoBorder, CComboBox)
	//ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()



// CComboxNoBorder 消息处理程序


void CComboxNoBorder::OnPaint()
{
	CPaintDC dc(this); // device context for painting

}

HBRUSH CComboxNoBorder::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnCtlColor"),"Enter");

	// WangQianfeng add start on 2012-11-02
	if ( NULL == pDC )
	{
		MessageBox(_T("CComboxNoBorder::OnCtlColor,pDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CComboxNoBorder::OnCtlColor"),"Leave");
		return NULL;
	}
	if ( NULL == pWnd )
	{
		MessageBox(_T("CComboxNoBorder::OnCtlColor,pWnd = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CComboxNoBorder::OnCtlColor"),"Leave");
		return NULL;
	}
	// WangQianfeng add end on 2012-11-02
	CRect rc;
	if (nCtlColor == CTLCOLOR_EDIT)
	{	
		CEdit *pEdit = (CEdit*)pWnd;
		if (pEdit)
		{
			GetClientRect(&rc);
			rc.right -= GetSystemMetrics(SM_CXVSCROLL) + 2;
			pEdit->MoveWindow(&rc);
			pEdit->GetClientRect(&rc);
			pEdit->SetRect(&rc);
		}
	}

	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnCtlColor"),"Leave");
	return hbr;
}

void CComboxNoBorder::OnShowWindow(BOOL bShow, UINT nStatus)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnShowWindow"),"Enter");

	if (bShow)
	{
		LOGFONT lf;
		GetFont()->GetLogFont(&lf);
		SendMessage(CB_SETITEMHEIGHT, -1, abs(lf.lfHeight));
	}
	CComboBox::OnShowWindow(bShow, nStatus);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnShowWindow"),"Leave");
}

////////////////////////////////////////////////////////////////////////////////////

// CListCtrlEditable

IMPLEMENT_DYNAMIC(CListCtrlEditable, CListCtrl)
CListCtrlEditable::CListCtrlEditable()
{
	// machao add start on 2013-12-16
	m_bSorting = false;
	m_lColumnSortStates = 0;
	EmptyArray( m_aCombinedSortedColumns, -1 );
	EmptyArray( (int*)m_aDefaultColumnsSort, (int)DESCENDING1);
	InitColumnType( (int*)m_aColumnType, (int)TYPE_NUMERIC );
	m_nSortRow = -1;
	// machao add end on 2013-12-16
}

CListCtrlEditable::~CListCtrlEditable()
{
}


BEGIN_MESSAGE_MAP(CListCtrlEditable, CListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)	
	ON_EN_KILLFOCUS(501, OnCtrlKillFocus)
	ON_CBN_KILLFOCUS(502, OnCtrlKillFocus)
	ON_LBN_KILLFOCUS(503, OnCtrlKillFocus)
	ON_NOTIFY(NM_KILLFOCUS, 504, OnCtrlKillFocus)
//	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlEditable::OnNMCustomdraw)
	// machao add start on 2013-12-16
	ON_NOTIFY(HDN_ITEMCLICKA, 0, OnHeaderClicked)
	ON_NOTIFY(HDN_ITEMCLICKW, 0, OnHeaderClicked)
	ON_NOTIFY(HDN_DIVIDERDBLCLICKA, 0, OnHeaderDBClicked)
	ON_NOTIFY(HDN_DIVIDERDBLCLICKW, 0, OnHeaderDBClicked)
	// machao add end on 2013-12-16
END_MESSAGE_MAP()



// CListCtrlEditable 消息处理程序
BOOL CListCtrlEditable::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.dwExStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	return CListCtrl::PreCreateWindow(cs);
}

int CListCtrlEditable::InsertColumn(int nCol, LPCTSTR lpszColumnHeading, ControlType ctrltype, DWORD style, bool bEditable, int nFormat, int nWidth1, int nSubItem)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::InsertColumn"),"Enter");

	int ival = CListCtrl::InsertColumn(nCol, lpszColumnHeading, nFormat, nWidth1, nSubItem);
	if (ival != -1)
		m_ctMap.insert(std::pair<int, CColumType>((int)m_ctMap.size(),  CColumType(nCol, nSubItem, ctrltype, style, bEditable, this)));
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::InsertColumn"),"Leave");
	return ival;
}

CWnd* CListCtrlEditable::GetColumControl(int col /* = -1 */, int subitem /* = -1 */)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::GetColumControl"),"Enter");

	CWnd *pwnd = NULL;
	for (int i = 0; i < (int)m_ctMap.size(); i++)
	{
		CColumType &ct = m_ctMap[i];
		if (ct.iCol == col && ct.iSubItem == subitem)
		{
			pwnd = ct.pCtrlWnd;
			break;
		}
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::GetColumControl"),"Leave");
	return pwnd;
}

void CListCtrlEditable::SetColumCtrlText(CWnd *pColumCtrl)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::SetColumCtrlText"),"Enter");

	// WangQianfeng add start on 2012-11-02
	if ( NULL == pColumCtrl )
	{
		MessageBox(_T("CListCtrlEditable::SetColumCtrlText,pColumCtrl = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CComboxNoBorder::SetColumCtrlText"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	if (pColumCtrl)
	{
		for (int i = 0; i < (int)m_ctMap.size(); i++)
		{
			CColumType &ct = m_ctMap[i];
			if (ct.pCtrlWnd == pColumCtrl)
			{
				if (ct.ctCntrlType == CtrlType_Edit)
				{
					pColumCtrl->SetWindowText(m_CurCell.sText);
				}
				// Song Chenguang add start on 2013-04-19
// 				else if (ct.ctCntrlType == CtrlType_Button)
// 				{
// 					pColumCtrl->SetWindowText(m_CurCell.sText);
// 				}
				// Song Chenguang add end on 2013-04-19
				else if (ct.ctCntrlType == CtrlType_ComboBox)
				{
					CComboBox *pcmb = (CComboBox*)pColumCtrl;
					CString sitem;
					int i = 0;
					for (i = 0; i < pcmb->GetCount(); i++)
					{
						pcmb->GetLBText(i, sitem);
						if (sitem == m_CurCell.sText)
							break;
					}
					if (i < pcmb->GetCount())
						pcmb->SetCurSel(i);
				}
				else if (ct.ctCntrlType == CtrlType_ListBox)
				{
					CListBox *plb = (CListBox*)pColumCtrl;
					CString sitem;
					int i = 0;
					for (i; i < plb->GetCount(); i++)
					{
						plb->GetText(i, sitem);
						if (sitem == m_CurCell.sText)
							break;
					}
					if (i < plb->GetCount())
						plb->SetCurSel(i);
				}
				else if (ct.ctCntrlType == CtrlType_DateTimePicker)
				{
					pColumCtrl->SetWindowText(m_CurCell.sText);
				}
				//else if (ct.ctCntrlType == CtrlType_Ip)
				//{
				//	pColumCtrl->SetWindowText(m_CurCell.sText);
				//}
				break;
			}
		}
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::SetColumCtrlText"),"Leave");
}

CString CListCtrlEditable::GetColumCtrlText(int col /* = -1 */, int subitem /* = -1 */)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::GetColumCtrlText"),"Enter");

	CString sVal = m_CurCell.sText;
	CWnd *pColumCtrl = GetColumControl(col, subitem);
	if (pColumCtrl)
	{
		for (int i = 0; i < (int)m_ctMap.size(); i++)
		{
			CColumType &ct = m_ctMap[i];
			if (ct.pCtrlWnd == pColumCtrl)
			{
				if (ct.ctCntrlType == CtrlType_Edit)
				{
					pColumCtrl->GetWindowText(sVal);
				}
				// Song Chenguang add start on 2013-04-19
// 				else if (ct.ctCntrlType == CtrlType_Button)
// 				{
// 					pColumCtrl->GetWindowText(sVal);
// 				}
				// Song Chenguang add end on 2013-04-19
				else if (ct.ctCntrlType == CtrlType_ComboBox)
				{
					CComboBox *pcmb = (CComboBox*)pColumCtrl;
					if (pcmb->GetCurSel() > -1)
						pcmb->GetLBText(pcmb->GetCurSel(), sVal);
				}
				else if (ct.ctCntrlType == CtrlType_ListBox)
				{
					CListBox *plb = (CListBox*)pColumCtrl;
					if (plb->GetCurSel() > -1)
						plb->GetText(plb->GetCurSel(), sVal);
				}
				else if (ct.ctCntrlType == CtrlType_DateTimePicker)
				{
					CIPAddressCtrl *pIp = (CIPAddressCtrl*)pColumCtrl;
					pIp->GetWindowText(sVal);
				}
				//else if (ct.ctCntrlType == CtrlType_Ip)
				//{
				//	pColumCtrl->SetWindowText(sVal);
				//}
				break;
			}
		}
	}
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::GetColumCtrlText"),"Leave");
	return sVal;
}

BOOL CListCtrlEditable::ShowColumCtrl(int nItem /* = -1 */, int nSubItem /* = -1 */)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::ShowColumCtrl"),"Enter");

	BOOL ret = EnsureVisible(nItem, FALSE);

	// 保存单元格数据
	m_CurCell.iRow = nItem;
	m_CurCell.iCol = nSubItem;
	m_CurCell.sText = GetItemText(nItem, nSubItem);

	CWnd *pColCtrl = GetColumControl(nSubItem, -1);
	if (pColCtrl)
	{
		CRect rect;
		GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rect);
		CRect rtList;
		GetClientRect(&rtList);
		if(rect.right > rtList.Width()) 
			Scroll(CSize( rect.Width() > rtList.Width()?rect.left : rect.right - rtList.Width(), 0));
		if(rect.left < 0) 
			Scroll(CSize(rect.left));

		GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rect);
		rect.right = min(rect.right, rtList.Width()-4);
		pColCtrl->MoveWindow(&rect);
		pColCtrl->ShowWindow(SW_SHOW);		
		pColCtrl->SendMessage(WM_SHOWWINDOW, TRUE);
		pColCtrl->SetFocus();

		SetColumCtrlText(pColCtrl);
	}
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::ShowColumCtrl"),"Leave");
	return ret;
}

void CListCtrlEditable::HideAllColumCtrl()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::HideAllColumCtrl"),"Enter");

	//SetCurCellText(m_CurCell);

	for (int i = 0; i < (int)m_ctMap.size(); i++)
	{
		CColumType &ct = m_ctMap[i];
		if (ct.pCtrlWnd)
		{
			if (ct.pCtrlWnd->IsWindowVisible())
				ct.pCtrlWnd->ShowWindow(SW_HIDE);
		}
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::HideAllColumCtrl"),"Leave");
}

void CListCtrlEditable::SetCurCellText(CCell &cell)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::SetCurCellText"),"Enter");

	if (cell.iCol >= 0 && cell.iRow >= 0)
	{
		CWnd *pColCtrl = GetColumControl(cell.iCol, -1);
		if (pColCtrl)
		{
			cell.sText = GetColumCtrlText(cell.iCol, -1);
			CListCtrl::SetItemText(cell.iRow, cell.iCol, cell.sText);
		}
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::SetCurCellText"),"Leave");
}

void CListCtrlEditable::OnCtrlKillFocus()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnCtrlKillFocus"),"Enter");

	SetCurCellText(m_CurCell);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnCtrlKillFocus"),"Leave");
}

void CListCtrlEditable::OnCtrlKillFocus( NMHDR * pNotifyStruct, LRESULT * result )
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnCtrlKillFocus"),"Enter");

	SetCurCellText(m_CurCell);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnCtrlKillFocus"),"Leave");
}

int CListCtrlEditable::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnCreate"),"Enter");

	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CComboxNoBorder::OnCtlColor"),"Leave");
		return -1;
	}

	// TODO:  在此添加您专用的创建代码
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnCreate"),"Leave");
	return 0;
}

void CListCtrlEditable::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnLvnItemchanged"),"Enter");

	// WangQianfeng add start on 2012-11-02
	if ( NULL == pNMHDR )
	{
		MessageBox(_T("CListCtrlEditable::OnLvnItemchanged,pNMHDR = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CComboxNoBorder::OnLvnItemchanged"),"Leave");
		return;
	}
	if ( NULL == pResult )
	{
		MessageBox(_T("CListCtrlEditable::OnLvnItemchanged,pResult = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CComboxNoBorder::OnLvnItemchanged"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	HideAllColumCtrl();

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnLvnItemchanged"),"Leave");
}

void CListCtrlEditable::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnNMDblclk"),"Enter");

	// WangQianfeng add start on 2012-11-02
	if ( NULL == pNMHDR )
	{
		MessageBox(_T("CListCtrlEditable::OnNMDblclk,pNMHDR = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CComboxNoBorder::OnCtlColor"),"Leave");
		return;
	}
	if ( NULL == pResult )
	{
		MessageBox(_T("CListCtrlEditable::OnNMDblclk,pResult = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CComboxNoBorder::OnCtlColor"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	HideAllColumCtrl();

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if(!pNMLV) 
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CComboxNoBorder::OnNMDblclk"),"Leave");
		return;
	}
	int nItem = pNMLV->iItem, 
		nSubItem = pNMLV->iSubItem;
	// 显示控件
	ShowColumCtrl(nItem, nSubItem);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CComboxNoBorder::OnNMDblclk"),"Leave");
}

/*
void CListCtrlEditable::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = CDRF_DODEFAULT;

	switch (pLVCD->nmcd.dwDrawStage)
	{
	case CDDS_ITEMPREPAINT:
		*pResult = ItemPrePaint(pLVCD);
		break;
	case CDDS_SUBITEM | CDDS_ITEMPREPAINT: 
		*pResult = SubItemPrePaint(pLVCD); 
		break;
	default: 
		break;
	}
}

DWORD CListCtrlEditable::SubItemPrePaint(NMLVCUSTOMDRAW* pLVCD)
{
	// 画图片
	CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);

	return CDRF_NOTIFYPOSTPAINT;
}

DWORD CListCtrlEditable::ItemPrePaint(NMLVCUSTOMDRAW* pLVCD)
{
	// 画图片
	CDC* pDc = CDC::FromHandle(pLVCD->nmcd.hdc);
	return CDRF_NOTIFYPOSTPAINT;
}
*/
// machao add start on 2013-12-16
void CListCtrlEditable::OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *pHDN = (HD_NOTIFY *) pNMHDR;
	if( pHDN->iButton == 0 )
	{
		//前7列和第9列（最大幅值）可以排序，其他列不提供排序
		if ((pHDN->iItem >= 0 && pHDN->iItem < 7 ) || 9 == pHDN->iItem )
		{

			//将上一次选择排序的列宽恢复
			if ( -1 != m_nSortRow )
			{
				SetColumnWidth(m_nSortRow, LVSCW_AUTOSIZE_USEHEADER);
				int nHeaderWidth = GetColumnWidth(m_nSortRow);
				SetColumnWidth(m_nSortRow, nHeaderWidth);
			}
			CDC *pDC = GetDC();
			NM_LISTVIEW* pHDN1 = (NM_LISTVIEW*)pNMHDR;
			m_nSortRow = pHDN1->iItem;

			if( IsControlPressed() )
				SortColumn( pHDN->iItem, MULTI_COLUMN_SORT );
			else
			{
				SortColumn( pHDN->iItem, SINGLE_COLUMN_SORT );

				//因为排序后，会加上排序的图标，所以下面重新设置选择排序列的列宽
				SetColumnWidth(pHDN->iItem, LVSCW_AUTOSIZE);
				int nColumnWidth = GetColumnWidth(pHDN->iItem);
				SetColumnWidth(pHDN->iItem, LVSCW_AUTOSIZE_USEHEADER);
				int nHeaderWidth = GetColumnWidth(pHDN->iItem); 
				SetColumnWidth(pHDN->iItem, max(nColumnWidth, nHeaderWidth) + 25);
			}
		}
		else
		{
			return;
		}
	}
	*pResult = 0;
}
void CListCtrlEditable::OnHeaderDBClicked(NMHDR* pNMHDR, LRESULT* pResult) 
{
	AdjustColumnWidth();
}
void CListCtrlEditable::AdjustColumnWidth()
{
	int nColumnCount = GetColumnCount();

	for (int i = 0; i < nColumnCount; i++)
	{
		SetColumnWidth(i, LVSCW_AUTOSIZE);
		int nColumnWidth = GetColumnWidth(i);
		SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);
		int nHeaderWidth = GetColumnWidth(i); 
		if ( m_nSortRow == i)
		{
			SetColumnWidth(i, max(nColumnWidth, nHeaderWidth) + 30);
		}
		else
		{
			SetColumnWidth(i, max(nColumnWidth, nHeaderWidth));
		}

	}
} 
int CListCtrlEditable::GetColumnCount()
{
	CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();
	return (pHeaderCtrl->GetItemCount());
}
const int CListCtrlEditable::IsControlPressed() const
{
	return (::GetKeyState( VK_CONTROL ) < 0 );
}
void CListCtrlEditable::SortColumn( int iSubItem, bool bSortingMultipleColumns )
{	
	m_bSorting = true;

	if( bSortingMultipleColumns )
	{
		int iNumCombinedSortedCols = GetNumCombinedSortedColumns();

		if( NotInCombinedSortedColumnList( iSubItem ) )
		{
			m_aCombinedSortedColumns[ iNumCombinedSortedCols++ ] = iSubItem;
			SetItemSortState( iSubItem, GetDefaultItemSortState(iSubItem) );
		}
		else
		{
			SORT_STATE1 ssEachItem = GetItemSortState( iSubItem );
			SetItemSortState( iSubItem, (SORT_STATE1)!ssEachItem );
			MoveItemInCombinedSortedListToEnd( iSubItem );
		}

		//sort it
		SortCombinedColumns();
	}
	else
	{
		SORT_STATE1 ssEachItem = GetItemSortState( iSubItem );
		bool bNotInList = NotInCombinedSortedColumnList( iSubItem );

		m_ctlHeaderCtrl.RemoveAllSortImages();
		EmptyArray(m_aCombinedSortedColumns, -1);
		m_lColumnSortStates = 0;

		m_aCombinedSortedColumns[ 0 ] = iSubItem;
		SetItemSortState( iSubItem, 
			bNotInList ? GetDefaultItemSortState(iSubItem) : (SORT_STATE1)!ssEachItem );

		//sort it
		SortCombinedColumns();
	}

	m_bSorting = false;
}
const int CListCtrlEditable::GetNumCombinedSortedColumns() const
{
	for( int i = 0; i < MAX_COLUMNS; i++ )
		if( m_aCombinedSortedColumns[i] == -1 )
			return i;
	return MAX_COLUMNS;
}
bool CListCtrlEditable::NotInCombinedSortedColumnList(int iItem) const
{
	int iNumCombinedSortedColumns = GetNumCombinedSortedColumns();
	for( int i = 0; i < iNumCombinedSortedColumns; i++ )
	{
		if( m_aCombinedSortedColumns[i] == iItem )
			return false;
	}
	return true;
}
void CListCtrlEditable::SetItemSortState(int iItem, SORT_STATE1 bSortState)
{
	if( bSortState != GetItemSortState( iItem ) )
	{
		m_lColumnSortStates ^= (1 << iItem);
	}
}
const SORT_STATE1 CListCtrlEditable::GetItemSortState( int iItem ) const
{
	return (SORT_STATE1)((m_lColumnSortStates) & ( 1 << iItem ));
}
const SORT_STATE1 CListCtrlEditable::GetDefaultItemSortState( int iItem ) const
{
	SORT_STATE1 nRes = DESCENDING1;

	if( iItem >= 0 && iItem < MAX_COLUMNS )
	{
		nRes = (SORT_STATE1)m_aDefaultColumnsSort[iItem];
	}

	return nRes;
}
void CListCtrlEditable::MoveItemInCombinedSortedListToEnd(int iItem)
{
	int iNumCombinedSortedColumns = GetNumCombinedSortedColumns();
	int aCombinedSortedColumns[MAX_COLUMNS];
	memset( aCombinedSortedColumns, -1, MAX_COLUMNS );
	int iItemIndex = FindItemInCombinedSortedList( iItem );
	if( iItemIndex != -1 )
	{
		if( iItemIndex >= 0 )	//Frank Chen <mkchen@iii.org.tw>
		{
			memcpy( aCombinedSortedColumns, m_aCombinedSortedColumns, iItemIndex * sizeof( int ) );
			memcpy( &aCombinedSortedColumns[iItemIndex], &m_aCombinedSortedColumns[iItemIndex + 1], (iNumCombinedSortedColumns - iItemIndex - 1) * sizeof(int) );
		}
	}
	aCombinedSortedColumns[ iNumCombinedSortedColumns - 1 ] = iItem;
	memcpy( m_aCombinedSortedColumns, aCombinedSortedColumns, MAX_COLUMNS * sizeof(int) );
	for( int i = 0; i < MAX_COLUMNS ; i++ )
	{
		if( aCombinedSortedColumns[i] == -1 )
			break;
	}
}
int CListCtrlEditable::FindItemInCombinedSortedList( int iItem )
{
	int iNumCombinedSortedColumns = GetNumCombinedSortedColumns();
	for( int i = 0; i < iNumCombinedSortedColumns; i++ )
	{
		if(m_aCombinedSortedColumns[i] == iItem )
			return i;
	}
	return -1;
}
void CListCtrlEditable::SortCombinedColumns(bool bSetSort /*= false*/)
{
	if( bSetSort )
	{
		m_bSorting = true;
	}

	int iNumCombinedSortedCols = GetNumCombinedSortedColumns();

	for( int nIndex = iNumCombinedSortedCols - 1; nIndex >= 0 ; nIndex-- )
	{
		SORT_STATE1 ssEachItem = GetItemSortState( m_aCombinedSortedColumns[nIndex] );
		SORT_TYPE sortType = GetColumnType(m_aCombinedSortedColumns[nIndex]);

		CSortClass csc( this, m_aCombinedSortedColumns[nIndex], sortType );	
		csc.Sort( DESCENDING1 == ssEachItem );	//Ariel Benary <Ariel.Benary@ness.com>

		//refresh the sort image
		m_ctlHeaderCtrl.SetSortImage( m_aCombinedSortedColumns[nIndex], ssEachItem );
	}

	if( bSetSort )
	{
		m_bSorting = false;
	}
}
void CListCtrlEditable::EmptyArray( int *pArray, int nVal )
{
	memset( pArray, nVal, MAX_COLUMNS * sizeof(int) );
}
void CListCtrlEditable::InitColumnType( int *pArray, int nVal )
{
	for (int i = 0;i < MAX_COLUMNS;++i ) 
	{
		pArray[i] = nVal;
	}
}
const SORT_TYPE CListCtrlEditable::GetColumnType( int iCol ) const
{
	return (iCol >= 0 && iCol < MAX_COLUMNS) ? m_aColumnType[iCol] : TYPE_TEXT;
}
void CListCtrlEditable::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_ctlHeaderCtrl.SubclassWindow( GetDlgItem(0)->m_hWnd );

	CListCtrl::PreSubclassWindow();
}
// machao add end on 2013-12-16
