#pragma once

#include <map>
// machao add start on 2013-12-16
#include "SortableHeaderCtrl.h"
#include "SortClass.h"

#define MAX_COLUMNS	       14
#define MULTI_COLUMN_SORT  1
#define SINGLE_COLUMN_SORT 0

enum SORT_STATE1{ DESCENDING1, ASCENDING1 };
// machao add end on 2013-12-16

// CComboxNoBorder
class CComboxNoBorder : public CComboBox
{
	/*
 * Combobox information
 */
	DECLARE_DYNAMIC(CComboxNoBorder)

public:
	CComboxNoBorder();
	virtual ~CComboxNoBorder();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

//////////////////////////////////////////////////////////////////////////////////////////

// CListCtrlEditable

class CListCtrlEditable : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEditable)
public:
	// 颜色
	typedef enum mycolor{
		Color_White = RGB(255, 255, 255),
		Color_Black = RGB(0, 0, 0),
	};

	// 支持的编辑控件
	typedef enum controltype{
		CtrlType_Static = 0,			// 静态文本框
//		CtrlType_Button,				// 按钮
		CtrlType_Edit,					// 可编辑框
		CtrlType_ComboBox,				// 组合框
		CtrlType_ListBox,				// 列表框
		CtrlType_DateTimePicker,		// 日期控件
		//CtrlType_Ip,					// IP控件
	} ControlType;

	// 记录当前单元格信息
	class CCell
	{
	public:
		CCell() : sText(""), iRow(-1), iCol(-1), dwBkColor(Color_White), dwFontColor(Color_Black)
		{

		}
		CCell(CString text, int row = -1, int col = -1, DWORD bkcolor = Color_White, DWORD fontcolor = Color_Black) : 
			  sText(text)
			, iRow(row)
			, iCol(col)
			, dwBkColor(bkcolor)
			, dwFontColor(fontcolor)
		{

		}

		CString sText;
		int		iCol,
				iRow;
		DWORD	dwBkColor,
				dwFontColor;
	};

	// 列的类型信息
	class CColumType
	{
	public:
		CColumType(int col = 0, int subitem = -1, ControlType ct = CtrlType_Static, DWORD style = 0, BOOL editable = false, CWnd *pparent = NULL) : 
		  iCol(col), iSubItem(subitem), ctCntrlType(ct), dwStyle(style), bEditable(editable), pCtrlWnd(NULL), pParent(pparent)
		{
			CreateControl();
		}

		CColumType(const CColumType &ct)
		{
			iCol = ct.iCol;
			iSubItem = ct.iSubItem;
			ctCntrlType = ct.ctCntrlType;
			dwStyle = ct.dwStyle;
			bEditable = ct.bEditable;
			pParent = ct.pParent;
			pCtrlWnd = NULL;
			CreateControl();
		}

		virtual ~CColumType()
		{
			if (pCtrlWnd != NULL)
			{
				delete pCtrlWnd;
				pCtrlWnd = NULL;
			}
		}

		CColumType& operator=(const CColumType &ct)
		{
			iCol = ct.iCol;
			iSubItem = ct.iSubItem;
			ctCntrlType = ct.ctCntrlType;
			dwStyle = ct.dwStyle;
			bEditable = ct.bEditable;
			pParent = ct.pParent;
			if (pCtrlWnd)
			{
				delete pCtrlWnd;
				pCtrlWnd = NULL;
			}
			CreateControl();
		}

		int			iCol,		// 列号
					iSubItem;	// 子列号
	
		ControlType ctCntrlType;
		DWORD		dwStyle;	// 控件样式
		BOOL		bEditable;	// 是否可编辑
		CWnd	    *pCtrlWnd;	// 控件指针
		CWnd		*pParent;

	private:
		void CreateControl()
		{
			// 根据类型创建相应的控件
			if (bEditable == TRUE)
			{
				if (ctCntrlType == CtrlType_Edit)
				{
					CEdit *pCtrl = new CEdit();
					if ( NULL != pCtrl ) //add by wangqianfeng on 20121106
					{	
						pCtrl->Create(dwStyle | WS_CHILD | WS_TABSTOP, CRect(0, 0, 0, 0), pParent, 501);
						pCtrl->SetFont(pParent->GetFont());
						pCtrlWnd = pCtrl;
					}								
				}
				else if (ctCntrlType == CtrlType_ComboBox)
				{
					CComboxNoBorder *pCtrl = new CComboxNoBorder();
					if ( NULL != pCtrl ) //add by wangqianfeng on 20121106
					{
						pCtrl->Create(dwStyle | WS_CHILD | WS_TABSTOP | CBS_DROPDOWNLIST, CRect(10,10,400,400), pParent, 502);
						pCtrl->SetFont(pParent->GetFont());
						pCtrlWnd = pCtrl;
					}
				}
				else if (ctCntrlType == CtrlType_ListBox)
				{
					CListBox *pCtrl = new CListBox();
					if ( NULL != pCtrl ) //add by wangqianfeng on 20121106
					{
						pCtrl->Create(dwStyle | WS_CHILD | WS_TABSTOP, CRect(0,0,200,200), pParent, 503);
						pCtrl->SetFont(pParent->GetFont());
						pCtrlWnd = pCtrl;
					}
				}
				else if (ctCntrlType == CtrlType_DateTimePicker)
				{
					CDateTimeCtrl *pCtrl = new CDateTimeCtrl();
					if ( NULL != pCtrl ) //add by wangqianfeng on 20121106
					{
						pCtrl->Create(dwStyle | WS_CHILD | WS_TABSTOP,	CRect(0, 0, 0, 0), pParent, 504);
						pCtrl->SetFont(pParent->GetFont());
						pCtrlWnd = pCtrl;
					}
				}
				//else if (ctCntrlType == CtrlType_Ip)
				//{
				//	CIPAddressCtrl *pCtrl = new CIPAddressCtrl();
				//	pCtrl->Create(dwStyle | WS_CHILD | WS_TABSTOP,	CRect(0, 0, 0, 0), pParent, 505);
				//	pCtrl->SetFont(pParent->GetFont());
				//	pCtrlWnd = pCtrl;
				//}
				// Song Chenguang add start on 2013-04-19
// 				else if (ctCntrlType == CtrlType_Button)
// 				{
// 					
// 					CButton* pCtrl = new CButton();
// 					if (NULL != pCtrl )
// 					{
// 						pCtrl->Create(_T("保存"), dwStyle | WS_CHILD | WS_TABSTOP | WS_EX_TOOLWINDOW, CRect(0,0,200,200), pParent, 505);
// 						pCtrl->SetFont(pParent->GetFont());
// 						pCtrlWnd = pCtrl;
// 					}
// 				}
				// Song Chenguang add end on 2013-04-19
			}
		} 
	};

protected:
	typedef std::map<int, CColumType>		mapColumType;
	mapColumType		m_ctMap;
	CCell				m_CurCell;

public:
	CListCtrlEditable();
	virtual ~CListCtrlEditable();

	// 插入列 ctrltype指定列的类型, style指定控件的样式(WS_CHILD等),其它参数同MSDN中clistctrl::InsertColumn的说明
	int InsertColumn(int nCol,	
					 LPCTSTR lpszColumnHeading,	
					 ControlType ctrltype = CtrlType_Static,  // 列控件类型
					 DWORD style = 0,						  // 列控件样式
					 bool bEditable = false,				  // 列是否可编辑
					 int nFormat = LVCFMT_LEFT,	
					 int nWidth = -1, 
					 int nSubItem = -1);
	// 得到相应列头的控件指针，不存在时返回NULL
	CWnd* GetColumControl(int col = -1, int subitem = -1);

protected:
	// machao add start on 2013-12-16
	afx_msg void OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHeaderDBClicked(NMHDR* pNMHDR, LRESULT* pResult);	
	// machao add end on 2013-12-16
	DECLARE_MESSAGE_MAP()

	BOOL ShowColumCtrl(int item = -1, int subitem = -1);
	void HideAllColumCtrl();
	void SetColumCtrlText(CWnd *pColumCtrl = NULL);
	CString GetColumCtrlText(int col = -1, int subitem = -1);
	void SetCurCellText(CCell &cell);

	afx_msg void OnCtrlKillFocus();
	afx_msg void OnCtrlKillFocus(NMHDR * pNotifyStruct=NULL, LRESULT * result = NULL);

	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	// machao add start on 2013-12-16
	virtual void PreSubclassWindow();
	void AdjustColumnWidth();
	int GetColumnCount();
public:
	int m_nSortRow;
	int m_nTextWidth;
public:
	const int IsControlPressed() const;
	void SortColumn( int, bool = false );
	const int GetNumCombinedSortedColumns() const;
	bool NotInCombinedSortedColumnList( int iItem ) const;
	void SetItemSortState( int iItem, SORT_STATE1 bSortState );
	const SORT_STATE1 GetItemSortState( int iItem ) const;
	void MoveItemInCombinedSortedListToEnd( int );
	void SortCombinedColumns(bool bSetSort = false);
	void EmptyArray( int* pArray, int nVal );
	void InitColumnType( int* pArray, int nVal );
	int FindItemInCombinedSortedList( int );
	const SORT_STATE1 GetDefaultItemSortState( int iItem ) const;
	const SORT_TYPE GetColumnType( int iCol ) const;
private:
	bool m_bSorting;
	SORT_TYPE m_aColumnType[MAX_COLUMNS];
	int m_aCombinedSortedColumns[MAX_COLUMNS];
	__int64 m_lColumnSortStates;
	SORT_STATE1 m_aDefaultColumnsSort[MAX_COLUMNS];
protected:
	CSortableHeaderCtrl m_ctlHeaderCtrl;
	// machao add end on 2013-12-16
// public:
// 	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
// public:
// 	DWORD SubItemPrePaint(NMLVCUSTOMDRAW* pLVCD);
// 	DWORD ItemPrePaint(NMLVCUSTOMDRAW* pLVCD);
};



