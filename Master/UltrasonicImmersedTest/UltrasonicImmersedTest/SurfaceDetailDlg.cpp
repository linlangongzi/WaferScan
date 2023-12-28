// SurfaceDetailDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "SortClass.h"
#include "MultiColumnSortListView.h"
#include "ListCtrlEditable.h"
#include "SurfaceDetailDlg.h" 
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "ExButton.h"
#include "SysParam.h"
#include "MechanicalControl.h"
#include "MechanicalControlPanel.h"
#include "CScanDlgBar.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "UltrasonicImmersedTestDoc.h"
#include "MovePathView.h"
#include <io.h> 
#include <locale.h> 
#include <odbcinst.h>
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
#include "UltrasonicImmersedTestView.h"
#include "CPrintView.h"
#include "MessageDlg.h"
#include <math.h>
#include <algorithm> 
#include "excel.h"
// CSurfaceDetailDlg dialog

//生成报表格式：0（通用），1（株洲时代）
int g_nReportFormat = 0;	//machao add on 2013-07-31

static ColumnInfo columns[] =
{
	{ _T("序号"),  				TYPE_NUMERIC, DESCENDING,   40, LVCFMT_CENTER },
	{ _T("minX"),				TYPE_NUMERIC, DESCENDING,   40, LVCFMT_CENTER },
	{ _T("minY"),  				TYPE_NUMERIC, DESCENDING,   40, LVCFMT_CENTER },
	{ _T("maxX"),       		TYPE_NUMERIC, DESCENDING,   40, LVCFMT_CENTER },
	{ _T("maxY"),       		TYPE_NUMERIC, DESCENDING,   40, LVCFMT_CENTER },
	{ _T("深度"),       		TYPE_NUMERIC, DESCENDING,   40, LVCFMT_CENTER },
	{ _T("面积"),       		TYPE_NUMERIC, DESCENDING,   40, LVCFMT_CENTER }
};

IMPLEMENT_DYNAMIC(CSurfaceDetailDlg, CDialog)

CSurfaceDetailDlg::CSurfaceDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSurfaceDetailDlg::IDD, pParent)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::CSurfaceDetailDlg"),"Enter");
	m_pParentWnd = NULL;
	m_pMessageDlg = NULL;

	m_Offset_Coordinate.x = 0.0f;
	m_Offset_Coordinate.y = 0.0f;
	PlaneFlawHeader m_combineDefect= {0};	// wangbingfu add on 2014-11-04
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::CSurfaceDetailDlg"),"Leave");
}

CSurfaceDetailDlg::~CSurfaceDetailDlg()
{
}

void CSurfaceDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::DoDataExchange"),"Enter");
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SURFACE_DETAIL, m_lstSurfaceResult);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::DoDataExchange"),"Leave");
}


BEGIN_MESSAGE_MAP(CSurfaceDetailDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_TO_FLAW, &CSurfaceDetailDlg::OnBnClickedButtonMoveToFlaw)//移动到伤位置
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_DATE_SAVE, &CSurfaceDetailDlg::OnBnClickedButtonDateSave)//保存数据
	ON_BN_CLICKED(IDC_BUTTON_SAVETO_EXCEL, &CSurfaceDetailDlg::OnTransExcel)//导出到Excel
	ON_BN_CLICKED(IDC_BUTTON_BINNARYSAVE, &CSurfaceDetailDlg::OnBnClickedButtonBinarysave)//二进制保存
	ON_BN_CLICKED(IDC_BUTTON_TABLE, &CSurfaceDetailDlg::OnBnClickedButtonTable)
	ON_BN_CLICKED(IDC_BUTTON_GETASCAN, &CSurfaceDetailDlg::OnBnClickedButtonGetascan)
//	ON_NOTIFY(HDN_ITEMCHANGED, 0, &CSurfaceDetailDlg::OnHdnItemchangedListSurfaceDetail)
//ON_NOTIFY(NM_SETFOCUS, IDC_LIST_SURFACE_DETAIL, &CSurfaceDetailDlg::OnNMSetfocusListSurfaceDetail)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SURFACE_DETAIL, &CSurfaceDetailDlg::OnNMClickListSurfaceDetail)
	ON_WM_TIMER()
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_SURFACE_DETAIL, &CSurfaceDetailDlg::OnLvnKeydownListSurfaceDetail)
END_MESSAGE_MAP()


// CSurfaceDetailDlg message handlers
void CSurfaceDetailDlg::InitResultList(CListCtrlEditable* pListCtrl)
{
	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::InitResultList"),"Enter");
	// WangQianfeng add start on 2012-11-02
	if (pListCtrl == NULL)
	{
		MessageBox(_T("CSurfaceDetailDlg::InitResultList,pListCtrl = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSurfaceDetailDlg::InitResultList"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	pListCtrl->SetFont(this->GetFont());

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	//	CCScanParamPanel* m_pCScanParam = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
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
	pListCtrl->InsertColumn(0, _T("序号"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 40);
	if(pCScanView->m_nAxisSelect == 1)	// XZ
	{
		pListCtrl->InsertColumn(1, _T("minX"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(2, _T("minZ"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(3, _T("maxX"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(4, _T("maxZ"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
	}
	else if(pCScanView->m_nAxisSelect == 2)	// YZ
	{
		pListCtrl->InsertColumn(1, _T("minY"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(2, _T("minZ"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(3, _T("maxY"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(4, _T("maxZ"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
	}
	else if(pCScanView->m_nAxisSelect == 3
		|| pCScanView->m_nAxisSelect == 4)	// R 
	{
		pListCtrl->InsertColumn(1, _T("minX"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(2, _T("minW"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(3, _T("maxX"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(4, _T("maxW"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
	}
	// Song Chenguang add start on 2013-05-04
	else if(pCScanView->m_nAxisSelect == 5)	// W 
	{
		pListCtrl->InsertColumn(1, _T("minW"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(2, _T("minZ"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(3, _T("maxW"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(4, _T("maxZ"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
	}
	// Song Chenguang add end on 2013-05-04
	else
	{
		pListCtrl->InsertColumn(1, _T("minX"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(2, _T("minY"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(3, _T("maxX"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
		pListCtrl->InsertColumn(4, _T("maxY"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
	}

	pListCtrl->InsertColumn(5, _T("深度"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
	pListCtrl->InsertColumn(6, _T("面积"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 50);
	pListCtrl->InsertColumn(7, _T("A扫设置"), CListCtrlEditable::CtrlType_ComboBox, CBS_DROPDOWN, true, LVCFMT_CENTER, 100);
	pListCtrl->InsertColumn(8, _T("导出设置"), CListCtrlEditable::CtrlType_ComboBox, CBS_DROPDOWN, true, LVCFMT_CENTER, 100);
	// songchenguang add start 2012-05-21
	pListCtrl->InsertColumn(9, _T("最大幅值"), CListCtrlEditable::CtrlType_Static, 0, true, LVCFMT_CENTER, 100);
	// songchenguang add end 2012-05-21
	pListCtrl->InsertColumn(10, _T("位置危险性"), CListCtrlEditable::CtrlType_Edit, 0, true, LVCFMT_CENTER, 100);
	pListCtrl->InsertColumn(11, _T("分析结果"), CListCtrlEditable::CtrlType_Edit, 0, true, LVCFMT_CENTER, 100);
	pListCtrl->InsertColumn(12, _T("处理意见"), CListCtrlEditable::CtrlType_Edit, 0, true, LVCFMT_CENTER, 100);
	pListCtrl->InsertColumn(13, _T("备注"), CListCtrlEditable::CtrlType_Edit, 0, true, LVCFMT_CENTER, 100);
		
	CComboBox *pcmb2 = (CComboBox *)pListCtrl->GetColumControl(7);
	if (pcmb2)
	{
		pcmb2->AddString(_T("保存"));
		pcmb2->AddString(_T("不保存"));
	}

	CComboBox *pcmb = (CComboBox *)pListCtrl->GetColumControl(8);
	if (pcmb)
	{
		pcmb->AddString(_T("导出"));
		pcmb->AddString(_T("不导出"));
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::InitResultList"),"Leave");
}

BOOL CSurfaceDetailDlg::OnInitDialog()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnInitDialog"),"Enter");
	CDialog::OnInitDialog();

	if (m_pParentWnd->m_nFeatureIndex > 5)
	{
		AfxMessageBox(_T("面伤计算不支持特征成像！"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnInitDialog"),"Leave");
		return FALSE;
	}
	
	m_pMessageDlg = new CMessageDlg;
	if(m_pMessageDlg->GetSafeHwnd() == NULL)
	{
		m_pMessageDlg->Create(IDD_DIALOG_MESSAGE, this);
		m_pMessageDlg->SetWindowPos(&wndTop,0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}
	m_pMessageDlg->ShowWindow(SW_SHOW);
	
	// WangQianfeng add start on 2013-06-27
	//默认不能获取A扫波形
	GetDlgItem(IDC_BUTTON_GETASCAN)->EnableWindow(FALSE);
	// WangQianfeng add end on 2013-06-27
	SetTimer(TIMER_WAITT_FOR_DETAIL, 50, NULL);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnInitDialog"),"Leave");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSurfaceDetailDlg::InitResultList(CMultiColumnSortListCtrl* pListCtrl)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::InitResultList"),"Enter");
	// WangQianfeng add start on 2012-11-02
	if (pListCtrl == NULL)
	{
		MessageBox(_T("CSurfaceDetailDlg::InitResultList,pListCtrl = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSurfaceDetailDlg::InitResultList"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02

	int         j;
	LVCOLUMN    lvc;

	pListCtrl->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM) 
		LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_FULLROWSELECT |
		pListCtrl->SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE));

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for(j = 0; j < countof(columns); j++)
	{
		lvc.iSubItem = j;
		lvc.pszText = (TCHAR *)columns[j].name;
		lvc.cx = pListCtrl->GetStringWidth(lvc.pszText) + 
			columns[j].lengthExtension + 15;
		if (lvc.cx < 40) 
			lvc.cx = 40;
		lvc.fmt = columns[j].fmt;
		pListCtrl->InsertColumn(j, columns[j].sortType, columns[j].sortState, 
			&lvc);
	}

	pListCtrl->SortColumn( 2, false );
	pListCtrl->SortColumn( 1, true );
	pListCtrl->SortColumn( 0, true );

	pListCtrl->SetUniqueName(_T("Parameter Result List"));
	pListCtrl->LoadColumnWidths();
	pListCtrl->LoadColumnOrder();
	pListCtrl->LoadColumnSort();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::InitResultList"),"Leave");
}

// 移动到列表当前项的伤位置
void CSurfaceDetailDlg::OnBnClickedButtonMoveToFlaw()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnBnClickedButtonMoveToFlaw"),"Enter");
	// TODO: Add your control notification handler code here
	// 获取当前列表项数据
	POSITION   pos   =   m_lstSurfaceResult.GetFirstSelectedItemPosition(); 
	if   (pos   ==   NULL) 
		TRACE0( "No   items   were   selected!\n "); 
	else 
	{ 
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		int   nItem   =   m_lstSurfaceResult.GetNextSelectedItem(pos); 
		TRACE1( "Item   %d   was   selected!\n ",   nItem); 
		//   you   could   do   your   own   processing   on   nItem   here 
		// 获取选中行minX, minY, maxX, maxY
		TCHAR szBufXMin[256];
		LVITEM itemMinX;
		itemMinX.iItem = nItem;
		itemMinX.iSubItem = 1;
		itemMinX.mask = LVIF_TEXT;
		itemMinX.pszText = szBufXMin;
		itemMinX.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemMinX);

		TCHAR szBufYMin[256];
		LVITEM itemMinY;
		itemMinY.iItem = nItem;
		itemMinY.iSubItem = 2;
		itemMinY.mask = LVIF_TEXT;
		itemMinY.pszText = szBufYMin;
		itemMinY.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemMinY);

		TCHAR szBufXMax[256];
		LVITEM itemMaxX;
		itemMaxX.iItem = nItem;
		itemMaxX.iSubItem = 3;
		itemMaxX.mask = LVIF_TEXT;
		itemMaxX.pszText = szBufXMax;
		itemMaxX.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemMaxX);

		TCHAR szBufYMax[256];
		LVITEM itemMaxY;
		itemMaxY.iItem = nItem;
		itemMaxY.iSubItem = 4;
		itemMaxY.mask = LVIF_TEXT;
		itemMaxY.pszText = szBufYMax;
		itemMaxY.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemMaxY);

		double dXMin = _wtof(szBufXMin);
		double dYMin = _wtof(szBufYMin);
		double dXMax = _wtof(szBufXMax);
		double dYMax = _wtof(szBufYMax);
		
		double dXCenter = (dXMin + dXMax) / 2.0;
		double dYCenter = (dYMin + dYMax) / 2.0;
	/*	double dZCenter = 0.0;
		double dW1Center = 0.0;

		// 获取当前Z,W1坐标
		CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
		CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)m_pParentWnd->GetDocument();

		WORD wAECNum = pApp->GetCardNum();

		for (int i=0; i<wAECNum*2; i++)
		{
			if (pDoc->m_lstFlawData[i].GetCount() > 0)
			{
				CFeatureInfo pFeatureInfo = pDoc->m_lstFlawData[i].GetTail();
				dZCenter = pFeatureInfo.m_dZ;
				dW1Center = pFeatureInfo.m_dW1;

				break;
			}
		}

		// 移动到伤位置
		
		CMovePathView* pView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);

		pView->m_bULDPtp = TRUE;
*/		
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

		LOGICAL_POINT_t i_point;
		i_point.x=0;
		i_point.y=0;
		// machao modify start on 2013-08-14
// 		if ( m_pParentWnd->m_nObjectNum == 1 )
// 		{
// 			if (m_pParentWnd->m_pCScanView->m_Offset_Coordinate.y > 0 || m_pParentWnd->m_pCScanView->m_Offset_Coordinate.x > 0)
// 			{
// 				i_point=m_pParentWnd->m_pCScanView->m_Offset_Coordinate;
// 			} 
// 
// 		}
// 		else
// 		{
// 			if (m_pParentWnd->m_pCScanView2->m_Offset_Coordinate.y > 0 || m_pParentWnd->m_pCScanView2->m_Offset_Coordinate.x > 0)
// 			{
// 				i_point=m_pParentWnd->m_pCScanView2->m_Offset_Coordinate;
// 			} 
// 		}
		if (m_pParentWnd->m_pCurCScanView->m_Offset_Coordinate.y > 0 || m_pParentWnd->m_pCurCScanView->m_Offset_Coordinate.x > 0)
		{
			i_point=m_pParentWnd->m_pCurCScanView->m_Offset_Coordinate;
		} 
		// machao modify end on 2013-08-14
		dXCenter += i_point.x;
		dYCenter += i_point.y;

		LOGICAL_POINT_t stLogicalPoint;
		// Song Chenguang modify start on 2013-09-10
		CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0, 0);
		int nAxisIndex = pMovePathView->m_GraphMove.GetAxisSelect();
		float fDiameter = pMovePathView->m_GraphMove.GetPipR();
		if (nAxisIndex == 3 || nAxisIndex == 4)
		{
			if (m_pParentWnd->m_nPipeXMode == 0)
			{
				stLogicalPoint.x = (float)dXCenter;
				stLogicalPoint.y = (float)dYCenter / 360.0f * PI * fDiameter;
			}
		}
		// machao add start on 2013-12-4
		else if (nAxisIndex == 5 && m_pParentWnd->m_nPipeXMode == 0)
		{
			fDiameter = pMovePathView->m_GraphMove.Get_WallDiameter();
			pCScanView->m_stLogicalFlaw.x = (float)dXCenter / 360.0f * PI * fDiameter;
			pCScanView->m_stLogicalFlaw.y = (float)dYCenter;
		}
		// machao add end on 2013-12-4
		else
		{
			stLogicalPoint.x = (float)dXCenter;
			stLogicalPoint.y = (float)dYCenter;
		}		
		// Song Chenguang modify end on 2013-09-10

		CPoint point = pCScanView->Logical2Client(stLogicalPoint);
		pCScanView->RepositionProcess(point, stLogicalPoint);
		
		//回位
		CMechanicalControlPanel* pMechanicalCtrlPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
		pMechanicalCtrlPanel->OnBnClickedButtonDefineReposition();
	} 
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnBnClickedButtonMoveToFlaw"),"Leave");
}

void CSurfaceDetailDlg::DrawColorRate()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::DrawColorRate"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
//	CCScanParamPanel* m_pCScanParam = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	CCScanParamPanel* m_pCScanParam = m_pParentWnd;
	CCScanView* m_pCScanView = NULL;
	if(m_pCScanParam == pFrame->m_wndCScanParamDockBar.m_pCScanParamView)
	{
		m_pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	}
	else if(m_pCScanParam == pFrame->m_wndCScanParamDockBar.m_pCScanParamView2)
	{
		m_pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
	}
	else
	{
		;//
	}
	CWnd *wnd = GetDlgItem(IDC_STATIC_HANHE_RATE); // 获取显示控件的句柄
	CDC *pDC;
	pDC = wnd->GetDC();
	// WangQianfeng add start on 2012-11-02
	if (NULL == pDC)
	{
		MessageBox(_T("CSurfaceDetailDlg::DrawColorRate,pDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		return;
	}
	// WangQianfeng add end on 2012-11-02
	int nRectLength = 32;
	int nRectHeight = 20;

	pDC->SetViewportOrg(10, 150);

	CPen cpen;
	cpen.CreatePen(PS_SOLID,1,RGB(0,0,0));//黑色 画线用
	pDC->SelectObject(&cpen);
	for (int i = 0; i < 20; i++)
	{
		pDC->Rectangle(i*nRectLength,0,nRectLength+i*nRectLength,nRectHeight);
	}

	pDC->SetViewportOrg(10, 152);
	CString string;//字符，显示数字用

	for(int i = 0; i < 20; i++)
	{
		string.Format (_T ("%.1f"), m_pCScanView->m_fSurfacePixeRate[i]);
		pDC->TextOut (i*nRectLength, 0, string);
	}

	string = _T("%");
	pDC->TextOut (20*nRectLength + 5, 0, string);

	//画出调色板
	pDC->SetViewportOrg(10, 130);
	CBrush* pOldBrush;
	if (m_pCScanParam-> m_nSurfceColorMode == 0)
	{
		for (int i = 0;i < 16; i++)
		{
			int nGrayValue = (15-i)*17;
			CBrush	brush;
			brush.CreateSolidBrush(RGB(nGrayValue, nGrayValue, nGrayValue));
			pOldBrush = pDC->SelectObject(&brush);
			pDC->Rectangle(i*nRectLength, 0, nRectLength+i*nRectLength, nRectHeight);
		}
	} 
	else
	{
		for (int i = 0; i < 20; i++)
		{
			//machao modify start on 2013-07-16
			if (m_pCScanView->m_nColorDefine == 0)
			{
				int nColorIndex = m_pCScanView->GetColorNumber(i * 5 + 3, m_pCScanView->m_fBoundMin, m_pCScanView->m_fBoundMax);
				CBrush brush(m_pCScanParam->m_plaPalette[nColorIndex].color);
				pOldBrush = pDC->SelectObject(&brush);
				pDC->Rectangle(i*nRectLength, 0, nRectLength+i*nRectLength, nRectHeight);
			}
			else if (m_pCScanView->m_nColorDefine == 1)
			{
				CBrush brush(m_pCScanView->m_clrPlatte[i]);
				pOldBrush = pDC->SelectObject(&brush);
				pDC->Rectangle(i*nRectLength, 0, nRectLength+i*nRectLength, nRectHeight);
			}
// 			CBrush brush(m_pCScanView->m_clrPlatte[i]);
// 			pOldBrush = pDC->SelectObject(&brush);
// 			pDC->Rectangle(i*nRectLength, 0, nRectLength+i*nRectLength, nRectHeight);
			//machao modify end on 2013-07-16
		}
		// 底色 无伤
// 		CBrush brush(RGB(245, 245, 245));
// 		pOldBrush = pDC->SelectObject(&brush);
// 		pDC->Rectangle(20*nRectLength, 0, nRectLength + 20*nRectLength, nRectHeight);
	}

	pDC->SelectObject(pOldBrush);
	// WangQianfeng add start on 2012-11-02
	ReleaseDC(pDC);
	// WangQianfeng add end on 2012-11-02
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::DrawColorRate"),"Leave");	
}

void CSurfaceDetailDlg::OnPaint()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnPaint"),"Enter");
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	DrawColorRate();
	DrawHistogram();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnPaint"),"Leave");
}

// 画柱状图
void CSurfaceDetailDlg::DrawHistogram()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::DrawHistogram"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//CCScanParamPanel* m_pCScanParam = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	CCScanParamPanel* m_pCScanParam = m_pParentWnd;
	CCScanView* m_pCScanView = NULL;
	if(m_pCScanParam == pFrame->m_wndCScanParamDockBar.m_pCScanParamView)
	{
		m_pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	}
	else if(m_pCScanParam == pFrame->m_wndCScanParamDockBar.m_pCScanParamView2)
	{
		m_pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
	}
	else
	{
		;//
	}
	CWnd *wnd = GetDlgItem(IDC_STATIC_HANHE_RATE); // 获取显示控件的句柄
	CDC *pDC;
	pDC = wnd->GetDC();
	// WangQianfeng add start on 2012-11-02
	if (NULL == pDC)
	{
		MessageBox(_T("CSurfaceDetailDlg::DrawHistogram,pDC = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSurfaceDetailDlg::DrawHistogram"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	int nRectLength = 32;
	pDC->SetViewportOrg(10, 20);
	CRect rectALLHistogram(0, 0, 20*nRectLength, 100);

	CPen pen;
	//矩形框
	pen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	CPen *pOldPen;
	pOldPen = pDC->SelectObject(&pen);
	pDC->Rectangle(&rectALLHistogram);

	//网格虚线
	CPen penGrid;
	penGrid.CreatePen(PS_DOT ,1,RGB(192,192,192));
	pOldPen = pDC->SelectObject(&penGrid);
	for(int i = 1; i < 10; i++)
	{
		pDC->MoveTo(rectALLHistogram.left, i * 10);
		pDC->LineTo(rectALLHistogram.right, i * 10);
	}
	pDC->SelectObject(pOldPen);

	//柱状图
	CBrush* pOldBrush;
	for (int i = 0; i < 20; i++)
	{
		if (m_pCScanView->m_nColorDefine == 0)
		{
			int nColorIndex = m_pCScanView->GetColorNumber(i * 5 + 3, m_pCScanView->m_fBoundMin, m_pCScanView->m_fBoundMax);
			CBrush brush(m_pCScanParam->m_plaPalette[nColorIndex].color);
			pOldBrush = pDC->SelectObject(&brush);
			pDC->Rectangle(i*nRectLength + 6, rectALLHistogram.bottom-(int)(m_pCScanView->m_fSurfacePixeRate[i]), i*nRectLength + 20, rectALLHistogram.bottom);
		}
		else if (m_pCScanView->m_nColorDefine == 1)
		{
			CBrush brush(m_pCScanView->m_clrPlatte[i]);
			pOldBrush = pDC->SelectObject(&brush);
			pDC->Rectangle(i*nRectLength + 6, rectALLHistogram.bottom-(int)(m_pCScanView->m_fSurfacePixeRate[i]), i*nRectLength + 20, rectALLHistogram.bottom);
		}
// 		CBrush brush(m_pCScanView->m_clrPlatte[i]);
// 		pOldBrush = pDC->SelectObject(&brush);
// 		pDC->Rectangle(i*nRectLength + 6, rectALLHistogram.bottom-(int)(m_pCScanView->m_fSurfacePixeRate[i]), i*nRectLength + 20, rectALLHistogram.bottom);
	}
	// WangQianfeng add start on 2012-11-02
	ReleaseDC(pDC);
	// WangQianfeng add end on 2012-11-02
	//空白
// 	CBrush brush(RGB(245, 245, 245));
// 	pOldBrush = pDC->SelectObject(&brush);
// 	pDC->Rectangle(20*nRectLength + 6, rectALLHistogram.bottom-(int)(m_pCScanView->m_fSurfacePixeRate[20]), 20*nRectLength + 20, rectALLHistogram.bottom);
// 	pDC->SelectObject(pOldBrush);	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::DrawHistogram"),"Leave");
}

/*
void CSurfaceDetailDlg::OnBnClickedButtonDateSave()
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[] = _T("Cscan Result files (*.txt)|*.txt||");

	CFileDialog dlg(FALSE, _T("txt"), NULL, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,	szFilters);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();
		//结果输出
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CCScanParamPanel* pCScanParam = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
		VARIANT Result = m_pParentWnd->m_PlateDrawCtrl.GetPlaneFlawPoints2();

		PlaneFlawHeader stPFH = {0};
		long lLBound = 0;
		long lUBound = 0;
		::SafeArrayGetLBound(Result.parray, 1, &lLBound);		//一维数组下标
		::SafeArrayGetUBound(Result.parray, 1, &lUBound);		//一维数组上标

		char* buf = NULL;
		SafeArrayAccessData(Result.parray,   (void   **)&buf);   

		UINT Size = (lUBound-lLBound+1);	

		int nZoomScale = m_pParentWnd->m_nZoomScale;

		int nPos = 0;
		int i = 0;

		CStdioFile file; 
		file.Open(strPath, CFile::modeReadWrite|CFile::modeCreate);
		char* old_locale = _strdup(setlocale(LC_CTYPE,NULL)); 
		CList<LOGICAL_POINT_t, LOGICAL_POINT_t&> lstSurfacePoint;	// 面伤最小点坐标
        float fMinSurfaceArea = pCScanParam->m_fMinSurfaceArea * nZoomScale * nZoomScale;
		setlocale( LC_CTYPE,("chs"));//设定 
		while(nPos+sizeof(PlaneFlawHeader) < Size)
		{	
			memcpy(&stPFH, buf + nPos, sizeof(PlaneFlawHeader));
			if (stPFH.area > fMinSurfaceArea)
			{				
				CString strMinX;
				strMinX.Format(_T("%.2f"), stPFH.min_x / nZoomScale);
				CString strMinY;
				strMinY.Format(_T("%.2f"), stPFH.min_y / nZoomScale);
				CString strMaxX;
				strMaxX.Format(_T("%.2f"), stPFH.max_x / nZoomScale);
				CString strMaxY;
				strMaxY.Format(_T("%.2f"), stPFH.max_y / nZoomScale);
				CString strZ;
				strZ.Format(_T("%.2f"), stPFH.z);
				CString strArea;
				strArea.Format(_T("%.2f"), stPFH.area / (nZoomScale * nZoomScale));

				file.WriteString(_T("minX: "));//正常写入 
				file.WriteString(	strMinX	);
				file.WriteString(_T("     minY: "));
				file.WriteString(	strMinY	);
				file.WriteString(_T("     maxX: "));
				file.WriteString(	strMaxX	);
				file.WriteString(_T("     maxY: "));
				file.WriteString(	strMaxY	);
				file.WriteString( _T("    深度: "));
				file.WriteString(	strZ	);
				file.WriteString(_T("     面积: "));
				file.WriteString(	strArea	);
				CString test = _T("\r\n");
				file.WriteString(test);			
			}
			int sizetmp = sizeof(PlaneFlawHeader) + sizeof(HPOINT)*stPFH.PointsCount;

			nPos += sizetmp;
			i++;
		}

		setlocale( LC_CTYPE, old_locale ); 
		free( old_locale );//还原区域设定 

		SafeArrayUnaccessData(Result.parray);
		SafeArrayDestroy(Result.parray);

		file.Close();
	
//	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) ); 
//	setlocale( LC_CTYPE, old_locale ); 

	}
}
*/
void CSurfaceDetailDlg::OnBnClickedButtonDateSave()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnBnClickedButtonDateSave"),"Enter");
	// TODO: Add your control notification handler code here
	TCHAR szFilters[] = _T("Cscan Result files (*.txt)|*.txt||");

	CFileDialog dlg(FALSE, _T("txt"), NULL, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,	szFilters);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();
		//结果输出
		CStdioFile file; 
		file.Open(strPath, CFile::modeReadWrite|CFile::modeCreate);
		char* old_locale = _strdup(setlocale(LC_CTYPE,NULL)); 
		setlocale( LC_CTYPE,("chs"));//设定 
		int nCount = m_lstSurfaceResult.GetItemCount();

		for (int i=0; i<nCount; i++)
		{
			CString strExport = m_lstSurfaceResult.GetItemText(i, 12);  
			if (strExport == _T("不导出"))
			{
				continue;
			}
			CString strTitles[13] = {_T("序号:"), _T("MinX:"),_T("MinY"),_T("MaxX:"),_T("MaxY:"),_T("深度:"),_T("面积:"),
				_T("位置危险性"),_T("分析结果"),_T("处理意见"),_T("备注"),_T("A扫设置"),_T("导出设置")};
			CString str;
			for (int j=0; j<10; j++)
			{
				// 打印字符串  
				if (j == 11 && j == 12)  
					continue;
				
				CString strItem = m_lstSurfaceResult.GetItemText(i, j);  
/*

				if (i == 0)
				{
					str = str + strItem + _T("\t");
				}
				else
*/

					str = str + strTitles[j] + strItem + _T("\t");
			}

			str = str + _T("\r\n");

			file.WriteString(str);
		}

		setlocale( LC_CTYPE, old_locale ); 
		free( old_locale );//还原区域设定 

		file.Close();
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnBnClickedButtonDateSave"),"Leave");	
}


/////////////////////////////////////////////////////////////////////////////////////////////
void CSurfaceDetailDlg::OnTransExcel() 
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnTransExcel"),"Enter");	
	UpdateData(TRUE);

	/*	int nIndex = m_cmbFenLiang.GetCurSel();

	CString strTitle;
	if (nIndex == 0)
	{
	strTitle = _T("X分量");
	}
	else if (nIndex == 1)
	{
	strTitle = _T("Y分量");
	}
	else if (nIndex == 2)
	{
	strTitle = _T("Z分量");
	}
	else
	{
	}

	if (nIndex != -1)
	{
	*/		ExportListToExcel(&m_lstSurfaceResult, _T("面伤数据"));

	//	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnTransExcel"),"Leave");		
}

///////////////////////////////////////////////////////////////////////////////
//	void GetExcelDriver(CListCtrl* pList, CString strTitle)
//	参数：
//		pList		需要导出的List控件指针
//		strTitle	导出的数据表标题
//	说明:
//		导出CListCtrl控件的全部数据到Excel文件。Excel文件名由用户通过“另存为”
//		对话框输入指定。创建名为strTitle的工作表，将List控件内的所有数据（包括
//		列名和数据项）以文本的形式保存到Excel工作表中。保持行列关系。
//	
//	edit by [r]@dotlive.cnblogs.com
///////////////////////////////////////////////////////////////////////////////
void CSurfaceDetailDlg::ExportListToExcel(CListCtrl* pList, CString strTitle)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::ExportListToExcel"),"Enter");
	// WangQianfeng add start on 2012-11-02
	if (pList == NULL)
	{
		MessageBox(_T("CSurfaceDetailDlg::ExportListToExcel,pList = null"), _T("警告"), MB_OK|MB_ICONERROR);
		return;
	}
	// WangQianfeng add end on 2012-11-02
	CString warningStr;
	if (pList->GetItemCount ()>0) {	
		CDatabase database;
		CString sDriver;
		CString sExcelFile; 
		CString sSql;
		CString tableName = strTitle;
		// 检索是否安装有Excel驱动 "Microsoft Excel Driver (*.xls)" 
		sDriver = GetExcelDriver();
		if (sDriver.IsEmpty())
		{
			// 没有发现Excel驱动
			AfxMessageBox(_T("没有安装Excel!\n请先安装Excel软件才能使用导出功能!"));
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSurfaceDetailDlg::ExportListToExcel"),"Leave");
			return;
		}

		///默认文件名
		if (!GetDefaultXlsFileName(sExcelFile))
		{
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSurfaceDetailDlg::ExportListToExcel"),"Leave");
			return;
		}


		// 创建进行存取的字符串
		sSql.Format(_T("DRIVER={%s};DSN='''';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s"),
			sDriver, sExcelFile, sExcelFile);

		// 创建数据库 (既Excel表格文件)
		if( database.OpenEx(sSql,CDatabase::noOdbcDialog) )
		{
			// 创建表结构
			int i;
			LVCOLUMN columnData;
			CString columnName;
			int columnNum = 0;
			CString strH;
			CString strV;

			sSql = "";
			strH = "";
			columnData.mask = LVCF_TEXT; 
			columnData.cchTextMax =100;
			columnData.pszText = columnName.GetBuffer (100);
			for(i=0;pList->GetColumn(i,&columnData);i++)
			{
				if (i!=0)
				{
					sSql = sSql + _T(", ") ;
					strH = strH + _T(", ") ;
				}
				sSql = sSql + _T(" ") + columnData.pszText +_T(" TEXT");
				strH = strH + _T(" ") + columnData.pszText +_T(" ");
			}
			columnName.ReleaseBuffer ();
			columnNum = i;

			sSql = _T("CREATE TABLE ") + tableName + _T(" ( ") + sSql +  _T(" ) ");
			try
			{
				database.ExecuteSQL(sSql);
			}
			catch (CDBException* e)
			{
				e->ReportError();
				e->Delete();
			}


			// 插入数据项
			int nItemIndex;

			for (nItemIndex=0;nItemIndex<pList->GetItemCount (); nItemIndex++)
			{
				strV = "";
				for(i=0;i<columnNum;i++)
				{
					// 若为不导出，不输出
					CString strExport = pList->GetItemText(nItemIndex,columnNum - 1);
					if (strExport == _T("不导出"))
					{
						continue;
					}


					if (i!=0)
					{
						strV = strV + _T(", ") ;
					}
/*
					if(i == 7)
						strV = strV + _T(" '' ");
					if(i != 7)
*/

						strV = strV + _T(" '") + pList->GetItemText(nItemIndex,i) +_T("' ");
				}

				sSql = _T("INSERT INTO ")+ tableName 
					+_T(" (")+ strH + _T(")")
					+_T(" VALUES(")+ strV + _T(")");
				database.ExecuteSQL(sSql);
			}

		}      

		// 关闭数据库
		database.Close();

		warningStr.Format(_T("导出文件保存于%s!"),sExcelFile);
		AfxMessageBox(warningStr);
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::ExportListToExcel"),"Leave");
}

//////////////////////////////////////////////////////////////////////////////
//名称：GetExcelDriver
//功能：获取ODBC中Excel驱动
//作者：徐景周(jingzhou_xu@163.net)
//组织：未来工作室(Future Studio)
//日期：2002.9.1
/////////////////////////////////////////////////////////////////////////////
CString CSurfaceDetailDlg::GetExcelDriver()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::GetExcelDriver"),"Enter");
	TCHAR szBuf[2001];
	WORD cbBufMax = 2000;
	WORD cbBufOut;
	TCHAR *pszBuf = szBuf;
	CString sDriver;

	// 获取已安装驱动的名称(涵数在odbcinst.h里)
	if (!SQLGetInstalledDrivers(szBuf, cbBufMax, &cbBufOut))
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSurfaceDetailDlg::GetExcelDriver"),"Leave");
		return _T("");
	}

	// 检索已安装的驱动是否有Excel...
	do
	{
		if (wcsstr(pszBuf, _T("Excel")) != 0)
		{
			//发现 !
			sDriver = CString(pszBuf);
			break;
		}
		pszBuf = wcschr(pszBuf, '\0') + 1;
	}
	while (pszBuf[1] != '\0');
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::GetExcelDriver"),"Leave");
	return sDriver;
}

//获得默认的文件名
BOOL CSurfaceDetailDlg::GetDefaultXlsFileName(CString& sExcelFile)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::GetDefaultXlsFileName"),"Enter");
	///默认文件名：yyyymmddhhmmss.xls
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	CString stroMaterial=pFrame->m_ProbeSet.getVal(_T("oMaterial"));
	CString stroId=pFrame->m_ProbeSet.getVal(_T("oId"));
	CString stroType=pFrame->m_ProbeSet.getVal(_T("oType"));

	CString timeStr;
	CTime day;
	day=CTime::GetCurrentTime();
	int filenameday,filenamemonth,filenameyear,filehour,filemin,filesec;
	filenameday=day.GetDay();//dd
	filenamemonth=day.GetMonth();//mm月份
	filenameyear=day.GetYear();//yyyy
	filehour=day.GetHour();//hh
	filemin=day.GetMinute();//mm分钟
	filesec=day.GetSecond();//ss
	timeStr.Format(_T("%s_%s_%s_%04d%02d%02d%02d%02d%02d"),stroMaterial,stroId,stroType,filenameyear,filenamemonth,filenameday,filehour,filemin,filesec);

	sExcelFile =  timeStr + _T(".xls");
	// prompt the user (with all document templates)
	CFileDialog dlgFile(FALSE,_T(".xls"),sExcelFile);
	CString title;
	CString strFilter;

	title = "导出";
	strFilter = "Excel文件(*.xls)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.xls");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;
	dlgFile.m_ofn.nFilterIndex = 1;
	// append the "*.*" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;

	if (dlgFile.DoModal()==IDCANCEL)
	{
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::GetDefaultXlsFileName"),"Leave");
		return FALSE; // open cancelled
	}

	CString strPath = dlgFile.GetPathName();
	sExcelFile=strPath;

	sExcelFile.ReleaseBuffer();
	if(MakeSurePathExists(sExcelFile,true)) {
		if(!DeleteFile(sExcelFile)) {    // delete the file
			AfxMessageBox(_T("覆盖文件时出错！"));
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSurfaceDetailDlg::GetDefaultXlsFileName"),"Leave");
			return FALSE;
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::GetDefaultXlsFileName"),"Leave");
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
//	BOOL MakeSurePathExists( CString &Path,bool FilenameIncluded)
//	参数：
//		Path				路径
//		FilenameIncluded	路径是否包含文件名
//	返回值:
//		文件是否存在
//	说明:
//		判断Path文件(FilenameIncluded=true)是否存在,存在返回TURE，不存在返回FALSE
//		自动创建目录
//
///////////////////////////////////////////////////////////////////////////////
BOOL CSurfaceDetailDlg::MakeSurePathExists( CString &Path,bool FilenameIncluded)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::MakeSurePathExists"),"Enter");
	int Pos=0;
	while((Pos=Path.Find(_T('\\'),Pos+1))!=-1)
		CreateDirectory(Path.Left(Pos),NULL);
	if(!FilenameIncluded)
		CreateDirectory(Path,NULL);
	//	return ((!FilenameIncluded)?!_access(Path,0):
	//	!_access(Path.Left(Path.ReverseFind('\\')),0));
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::MakeSurePathExists"),"Leave");
	return !_waccess(Path,0);
}

void CSurfaceDetailDlg::OnBnClickedButtonBinarysave()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnBnClickedButtonBinarysave"),"Enter");
	// TODO: Add your control notification handler code here
	TCHAR szFilters[] = _T("scan Result files (*.txt)|*.txt||");

	CFileDialog dlg(FALSE, _T("txt"), NULL, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,	szFilters);

	if (dlg.DoModal() == IDOK)
	{
		CString strPath = dlg.GetPathName();
		////////////////////////////////////////////////////////////////////////////////////
		VARIANT Result = m_pParentWnd->m_PlateDrawCtrl.GetPlaneFlawPoints2();

		PlaneFlawHeader stPFH = {0};
		long lLBound = 0;
		long lUBound = 0;
		::SafeArrayGetLBound(Result.parray, 1, &lLBound);		//一维数组下标
		::SafeArrayGetUBound(Result.parray, 1, &lUBound);		//一维数组上标

		char* buf = NULL;
		SafeArrayAccessData(Result.parray,   (void   **)&buf);   

		UINT Size = (lUBound-lLBound+1);	

		int nZoomScale = m_pParentWnd->m_nZoomScale;

		int nPos = 0;
		int i = 0;

		CStdioFile file; 
		file.Open(strPath, CFile::modeReadWrite|CFile::modeCreate|CFile::typeBinary);
//		char* old_locale = _strdup(setlocale(LC_CTYPE,NULL)); 
		// wangbingfu add start on 2014-11-04
/*		if(m_pParentWnd->m_nSurfaceSelType == 0)
		{
			memcpy(&stPFH, &m_combineDefect, sizeof(PlaneFlawHeader));
			CString strMinX;
			strMinX.Format(_T("%.2f"), stPFH.min_x / nZoomScale);
			CString strMinY;
			strMinY.Format(_T("%.2f"), stPFH.min_y / nZoomScale);
			CString strMaxX;
			strMaxX.Format(_T("%.2f"), stPFH.max_x / nZoomScale);
			CString strMaxY;
			strMaxY.Format(_T("%.2f"), stPFH.max_y / nZoomScale);
			CString strZ;
			strZ.Format(_T("%.2f"), stPFH.z);
			CString strArea;
			strArea.Format(_T("%.2f"), stPFH.area / (nZoomScale * nZoomScale));

			file.Write(buf,Size);//正常写入 
		}
		else*/	// wangbingfu add end on 2014-11-04
		{
			while(nPos+sizeof(PlaneFlawHeader) < Size)
			{	
				memcpy(&stPFH, buf + nPos, sizeof(PlaneFlawHeader));
				CString strMinX;
				strMinX.Format(_T("%.2f"), stPFH.min_x / nZoomScale);
				CString strMinY;
				strMinY.Format(_T("%.2f"), stPFH.min_y / nZoomScale);
				CString strMaxX;
				strMaxX.Format(_T("%.2f"), stPFH.max_x / nZoomScale);
				CString strMaxY;
				strMaxY.Format(_T("%.2f"), stPFH.max_y / nZoomScale);
				CString strZ;
				strZ.Format(_T("%.2f"), stPFH.z);
				CString strArea;
				strArea.Format(_T("%.2f"), stPFH.area / (nZoomScale * nZoomScale));

				file.Write(buf,Size);//正常写入 
				int sizetmp = sizeof(PlaneFlawHeader) + sizeof(HPOINT)*stPFH.PointsCount;
				nPos += sizetmp;
				i++;
			}
		}		

		SafeArrayUnaccessData(Result.parray);
		SafeArrayDestroy(Result.parray);

		file.Close();
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnBnClickedButtonBinarysave"),"Leave");
}


void CSurfaceDetailDlg::OnOK()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnOK"),"Enter");
	// TODO: Add your specialized code here and/or call the base class

	CButton* i_button = (CButton*)GetDlgItem(IDC_CHECK_SHOW_PROBE);
	i_button->SetCheck(0);
	setList();
	//CDialog::OnOK();
	CDialog::DestroyWindow();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnOK"),"Leave");
}

void CSurfaceDetailDlg::OnCancel()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnCancel"),"Enter");
	// TODO: Add your specialized code here and/or call the base class

	CButton* i_button = (CButton*)GetDlgItem(IDC_CHECK_SHOW_PROBE);
	i_button->SetCheck(0);
	setList();
	//CDialog::OnCancel();
	CDialog::DestroyWindow();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnCancel"),"Leave");
}

void CSurfaceDetailDlg::PostNcDestroy()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::PostNcDestroy"),"Enter");
	// TODO: Add your specialized code here and/or call the base class
	// Song chenguang add start 2012-05-03
	// 将列表数据中显示导出的保存
	/*
	int nCount = m_lstSurfaceResult.GetItemCount();
	
		for(int i=0; i<nCount; i++)
		{
			CString strOutput = m_lstSurfaceResult.GetItemText(i, 7);
			if (strOutput == _T("不导出"))
				continue;
	
			FlawNode node;
			for (int j=1; j<10; j++)
			{
				CString str = m_lstSurfaceResult.GetItemText(i, j);
				
				
				if (j==1)
					node.m_fMinX = _wtof(str.GetBuffer(str.GetLength()));
				else if (j==2)
					node.m_fMinY = _wtof(str.GetBuffer(str.GetLength()));
				else if (j==3)
					node.m_fMaxX = _wtof(str.GetBuffer(str.GetLength()));
				else if (j==4)
					node.m_fMaxY = _wtof(str.GetBuffer(str.GetLength()));
				else if (j==5)
					node.m_fZ = _wtof(str.GetBuffer(str.GetLength()));
				else if (j==6)
					node.m_fArea = _wtof(str.GetBuffer(str.GetLength()));
				else if (j==9)
					node.m_strAscan = str;
				else
				{
				}
			}
			m_pParentWnd->m_lstSurfaceDetail.AddTail(node);
		}*/
	
	// Song chenguang add end 2012-05-03
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
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

	CDialog::PostNcDestroy();
	m_pParentWnd->m_pSurfaceDeatailDlg = NULL;
	delete this;

	pCScanView->Invalidate(TRUE);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::PostNcDestroy"),"Leave");
}

void CSurfaceDetailDlg::setList()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::setList"),"Enter");
	// Song chenguang add start 2012-05-03
	// 将列表数据中显示导出的保存
	//m_pParentWnd->m_lstSurfaceDetail.RemoveAll();
	int nCount = m_lstSurfaceResult.GetItemCount();
	//for(POSITION pos = pDoc->m_lstFlawData[0].GetHeadPosition();pos; )


	POSITION pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();

	for(int i=0; i<nCount; i++)
	{
		CString strOutput = m_lstSurfaceResult.GetItemText(i, 13);
		if (strOutput == _T("不导出"))
			continue;

		FlawNode *node = &m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);

		for (int j=1; j<14; j++)
		{
			CString str = m_lstSurfaceResult.GetItemText(i, j);


			if (j==1)
				node->m_fMinX = static_cast<float>(_wtof(str.GetBuffer(str.GetLength())));
			else if (j==2)
				node->m_fMinY = static_cast<float>(_wtof(str.GetBuffer(str.GetLength())));
			else if (j==3)
				node->m_fMaxX = static_cast<float>(_wtof(str.GetBuffer(str.GetLength())));
			else if (j==4)
				node->m_fMaxY = static_cast<float>(_wtof(str.GetBuffer(str.GetLength())));
			else if (j==5)
				node->m_fZ = static_cast<float>(_wtof(str.GetBuffer(str.GetLength())));
			else if (j==6)
				node->m_fArea = static_cast<float>(_wtof(str.GetBuffer(str.GetLength())));
			else if (j == 9)
				node->m_fFuzhi = static_cast<float>(_wtof(str.GetBuffer(str.GetLength())));
			else if (j==10)
				node->m_strPosition = str;
			else if (j==11)
				node->m_strResult = str;
			else if (j==12)
				node->m_strProc = str;
			else if (j==13)
				node->m_strMemo = str;
			else if (j==7)
				node->m_strAscan = str;
			else if (j==8)
				node->m_strSave = str;
			//else if (j==9)
			//	node.m_strAscan = str;
			else
			{
			}
		}
		//m_pParentWnd->m_lstSurfaceDetail.AddTail(node);
	}
	// Song chenguang add end 2012-05-03
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::setList"),"Leave");
}

BOOL CSurfaceDetailDlg::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DestroyWindow();
}

void CSurfaceDetailDlg::OnBnClickedButtonTable()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnBnClickedButtonTable"),"Enter");
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
	SetTimer(TIMER_CREATE_TABLE,50,NULL);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnBnClickedButtonTable"),"Leave");
}

void CSurfaceDetailDlg::TableCreate()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::TableCreate"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	setList();

	CString strTempPath;
	TCHAR szTempPath [100];
	DWORD dwSize=100;
	DWORD dwResult=GetTempPath(dwSize,szTempPath);//获取临时文件夹路径 

	CString timeStr;
	CTime day;
	day=CTime::GetCurrentTime();
	int filenameday,filenamemonth,filenameyear,filehour,filemin,filesec;
	filenameday=day.GetDay();//dd
	filenamemonth=day.GetMonth();//mm月份
	filenameyear=day.GetYear();//yyyy
	filehour=day.GetHour();//hh
	filemin=day.GetMinute();//mm分钟
	filesec=day.GetSecond();//ss
	timeStr.Format(_T("%s%04d%02d%02d%02d%02d%02d"),szTempPath,filenameyear,filenamemonth,filenameday,filehour,filemin,filesec);
	CString strSCANDate;
	strSCANDate.Format(_T("%04d-%02d-%02d"),filenameyear,filenamemonth,filenameday);
	strTempPath=timeStr+_T(".c.bmp");

	//	ShowWindow(SW_HIDE);
	//	CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	//	pCScanView->ShowWindow(SW_SHOW);
	//pCScanView->SetForegroundWindow();
	//	pCScanView->Invalidate();
	//Invalidate();

	BOOL bGetCPic=m_pParentWnd->SavePicByPath(strTempPath,true);

	//Invalidate();
	//	ShowWindow(SW_SHOW);
	//Invalidate();

	CPrintView pv;
	CLSID clsid;
	if(CLSIDFromString(CComBSTR("ActiveXExtension.PrintView"), &clsid)==S_OK)
	{
		pv.CreateDispatch(clsid,NULL);
	}
	CMechanicalControlPanel* pMechanicalCtrlPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;



	;
	CProbeSet probeset1;
  	if(CLSIDFromString(CComBSTR("ActiveXExtension.ProbeSet"), &clsid)==S_OK)
  	{
  		probeset1.CreateDispatch(clsid,NULL);
  	}
    probeset1.SetDir(pMechanicalCtrlPanel->m_strDataFolder);
//    	CString str;
//    	BSTR b = str.AllocSysString() ;
//    	probeset1.GetMaterialID(&b);
//    	str = b;
//    	SysFreeString(b);


// 	BOOL bFind = TRUE; 
// 
// 	CString strDir = pFrame->m_strDataFolder+_T("fjkd");
// 	bFind =PathFileExists(strDir);
// 	if (!bFind)
// 	{
// 		if(!CreateDirectory(strDir, NULL)) 
// 		{ 
// 			AfxMessageBox(_T("不能创建目录：") + strDir); 
// 			return; 
// 		}
// 	}
// 
// 	strDir = pFrame->m_strDataFolder+_T("fjkd\\")+str;
// 	bFind = PathFileExists(strDir);
// 	if (!bFind)
// 	{
// 		if(!CreateDirectory(strDir, NULL)) 
// 	     { 
// 		    AfxMessageBox(_T("不能创建目录：") + strDir); 
// 		    return; 
// 	     }
// 	}
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

	/*int nGateIndexTemp = pCScanView->m_nGateIndex;

	for(int j = 0; j < pCScanView->m_nGateIndex; j++)
	{
		if(!pCScanView->m_bFileGateFlag[i][j])
		{
			nGateIndexTemp--;
		}
	}*/
    
	pv.ExLoad(); 

	if(bGetCPic)
	{
		pv.ExSetPic(_T("A18:O37"),strTempPath);  
	}

	CString strList[6] = {pFrame->m_ProbeSet.getVal(_T("uName")),
		pFrame->m_ProbeSet.getVal(_T("uClass")),
		pFrame->m_ProbeSet.getVal(_T("oMaterial")),
		pFrame->m_ProbeSet.getVal(_T("oType")),
		pFrame->m_ProbeSet.getVal(_T("oId")),
		pFrame->m_ProbeSet.getVal(_T("oSize"))};

	//machao modify start on 2013-07-30 //?
	pv.ExSetTxt(_T("O11"),strList[0]);
	//machao modify end on 2013-07-30
	pv.ExSetTxt(_T("D11:J11"),strList[1]);
	pv.ExSetTxt(_T("D4:J4"),strList[2]);
	pv.ExSetTxt(_T("D9:J9"),strList[3]);
	pv.ExSetTxt(_T("O3"),strList[4]);
	pv.ExSetTxt(_T("O4"),strList[5]);
	pv.ExSetTxt(_T("D10:J10"),strSCANDate);

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CString strMachine;
	strMachine.Format(_T("%s"), pApp->m_IniParams.m_info.wcMechineType);
	pv.ExSetTxt(_T("D7:J7"),strMachine);
	CString strUserCompany;
	strUserCompany.Format(_T("%s"), pApp->m_IniParams.m_info.wcUserCompany);
	pv.ExSetTxt(_T("O8"),strUserCompany);

	//{_T("序号:"), _T("MinX:"),_T("MinY"),_T("MaxX:"),_T("MaxY:"),_T("深度:"),_T("面积:"),
	//	_T("位置危险性"),_T("分析结果"),_T("处理意见"),_T("备注"),_T("A扫设置"),_T("导出设置")};
	POSITION pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();
	int cc=1;
	int ccc=0;
	float ccMianji=0.0;
	float fMaxArea=0.0;
	float fMaxLength=0.0;
	int maxRow=41;
	char cell[11][2]={{'A','A'},{'B','B'},{'C','D'},{'E','E'},{'F','F'},{'G','G'},{'H','H'},{'I','I'},{'J','K'},{'L','M'},{'N','O'}};
	int nCount = m_lstSurfaceResult.GetItemCount();
	for(int i=0; i<nCount; i++)
	{
		CString strOutput = m_lstSurfaceResult.GetItemText(i, 8);
		if (strOutput == _T("不导出"))
			continue;
		ccc++;
		FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);
		ccMianji+=node.m_fArea;

		if (node.m_fArea>fMaxArea)
		{
			fMaxArea=node.m_fArea;
		}

		float fTmpLength=sqrtf(pow(node.m_fMaxX-node.m_fMinX,2)+pow(node.m_fMaxY-node.m_fMinY,2));
		if (fTmpLength>fMaxLength)
		{
			fMaxLength=fTmpLength;
		}

		LOGICAL_POINT_t pt;
		pt.x = (node.m_fMinX + node.m_fMaxX) / 2 ;
		pt.y = (node.m_fMinY + node.m_fMaxY) / 2 ;

		CString strMinX;
		strMinX.Format(_T("%.2f"), pt.x);//node.m_fMinX);
		CString strMinY;
		strMinY.Format(_T("%.2f"), pt.y);//node.m_fMinY);
		//添加序号  yachang.huang add start on 2012-11-19
        CString strNum;
		strNum.Format(_T("%d"),maxRow-41);
		CString i_sn_num;
		i_sn_num.Format(_T("%s%d:%s%d"),(CString)cell[0][0],maxRow,(CString)cell[0][1],maxRow);
		pv.ExSetTxt(i_sn_num,strNum);
		//yachang.huang add end on 2012-11-19
		//“幅值”前面的几列都向右移一列，去掉原来的“A扫图”列  yachang.huang modify start on 2012-11-19
		CString i_sn;
		i_sn.Format(_T("%s%d:%s%d"),(CString)cell[1][0],maxRow,(CString)cell[1][1],maxRow);
		pv.ExSetTxt(i_sn,strMinX);
		CString i_sn2;
		i_sn2.Format(_T("%s%d:%s%d"),(CString)cell[2][0],maxRow,(CString)cell[2][1],maxRow);
		pv.ExSetTxt(i_sn2,strMinY);

		CString i_sn4;
		i_sn4.Format(_T("%s%d:%s%d"),(CString)cell[3][0],maxRow,(CString)cell[3][1],maxRow);
		pv.ExSetTxt(i_sn4,_T(""));
// 		CString i_sn5;
// 		i_sn5.Format(_T("%s%d:%s%d"),(CString)cell[5][0],maxRow,(CString)cell[5][1],maxRow);
// 		pv.ExSetTxt(i_sn5,_T(""));

// 		CString strAIndex;
// 		if (node.m_strAscan == _T("保存") && node.m_boolHasAscan && node.m_strAscanPath!=""){
// 			strAIndex.Format(_T("%d"), cc);
// 			cc++;
// 			CString i_sn3;
// 			i_sn3.Format(_T("%s%d:%s%d"),(CString)cell[5][0],maxRow,(CString)cell[5][1],maxRow);
// 			pv.ExSetTxt(i_sn3,strAIndex);
// 		}

		for (int j=6; j<8; j++)
		{
			CString str = m_lstSurfaceResult.GetItemText(i, j - 1);

			CString i_sn;
			i_sn.Format(_T("%s%d:%s%d"),(CString)cell[j-2][0],maxRow,(CString)cell[j-2][1],maxRow);
			pv.ExSetTxt(i_sn,str);
		}

		//yachang.huang modify end on 2012-11-19


		// modify by songchenguang 2012-07-01
		for (int j=9; j<14; j++)
		{
			CString str = m_lstSurfaceResult.GetItemText(i, j);

			CString i_sn;
			i_sn.Format(_T("%s%d:%s%d"),(CString)cell[j-3][0],maxRow,(CString)cell[j-3][1],maxRow);
			pv.ExSetTxt(i_sn,str);
		}

		maxRow++;
	}

	CString strQuexian;
	strQuexian.Format(_T("%d"),ccc);
	pv.ExSetTxt(_T("D14:J14"),strQuexian);

	CString strMianji;
	//wangbingfu modify start on 2015-07-08
	BOOL bUseHanHeData = FALSE;
	if(pCScanParam->m_nGateType == 0) // 高于
	{
		if(pCScanParam->m_nMinAMP == pCScanView->m_fGateThreshold[pCScanView->m_nGateIndex]
		&& pCScanParam->m_nMaxAMP == 100)
		{
			bUseHanHeData = TRUE;
		}
	}
	else
	{
		if(pCScanParam->m_nMaxAMP == pCScanView->m_fGateThreshold[pCScanView->m_nGateIndex]
		&& pCScanParam->m_nMinAMP == 0)
		{
			bUseHanHeData = TRUE;
		}
	}

	if(!bUseHanHeData)
	{
		strMianji.Format(_T("%f"),ccMianji);
	}
	else
	{
		strMianji.Format(_T("%f"), pCScanView->m_SelArea - pCScanView->m_dbFlawArea);
	}
	//wangbingfu modify end on 2015-07-08
	pv.ExSetTxt(_T("O14"),strMianji);

	CString strMaxMianji;
	strMaxMianji.Format(_T("%f"),fMaxArea);
	pv.ExSetTxt(_T("D16:J16"),strMaxMianji);

	CString strLength;
	strLength.Format(_T("%f"),fMaxLength);
	pv.ExSetTxt(_T("O15"),strLength);


	//增加原点偏移输出  yachang.huang add start on 2012-11-19
	CString strOffsetCoordinate;
	strOffsetCoordinate.Format(_T("(%.2f,%.2f)"),pCScanView->m_Offset_Coordinate.x, pCScanView->m_Offset_Coordinate.y);
	pv.ExSetTxt(_T("O16"),strOffsetCoordinate); 
	//yachang.huang add end on 2012-11-19

	maxRow++;

	pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();
	cc=0;
	for(int i=0; i<nCount; i++)
	{
		FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);
		CString strOutput = m_lstSurfaceResult.GetItemText(i, 8);
		
		if (strOutput == _T("不导出"))
		{
			continue;
		}

		//FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);

		if (node.m_strAscan == _T("保存") && node.m_boolHasAscan && node.m_strAscanPath!="")
		{
            CString strTempAscanPath;
            strTempAscanPath = node.m_strAscanPath;
			int num = std::count((LPCTSTR)strTempAscanPath,(LPCTSTR)strTempAscanPath+strTempAscanPath.GetLength(), _T('@'));//获取m_strAscanPath里面包含几个路径
			int nSinglePathLength = strTempAscanPath.GetLength()/num;
			for (int j = 0;j<num;j++)
			{
				CString strTempPath = strTempAscanPath.Mid(j*nSinglePathLength,nSinglePathLength);
				strTempAscanPath.TrimLeft(nSinglePathLength);
				strTempPath.TrimLeft(_T("@"));
				CString strAIndex;
				strAIndex.Format(_T("A扫图序号：%d-%d"), cc,j);
				CString i_sn;
				i_sn.Format(_T("A%d:D%d"),maxRow,maxRow);
				pv.ExSetTxt(i_sn,strAIndex);
				maxRow++;
				CString i_sn2;
				i_sn2.Format(_T("A%d:O%d"),maxRow,maxRow+18);
				pv.ExSetPic(i_sn2,strTempPath);
				maxRow += 20;
				continue;
			}
		}
		cc++;
	}


	//	CCScanParamPanel* m_pCScanParam = pFrame->m_wndCScanParamDockBar.m_pCScanParamView
	if(pCScanView->m_nAxisSelect == 3
		|| pCScanView->m_nAxisSelect == 4)	// wangbingfu add on 2013-01-09
	{
		pv.ExSetTxt(_T("C40:D40"),_T("R角度"));	// machao modify on 2013-09-10
	}
	// Song Chenguang add start on 2013-05-04
	else if (pCScanView->m_nAxisSelect == 5)
	{
		pv.ExSetTxt(_T("B40"),_T("W角度"));
		pv.ExSetTxt(_T("C40:D40"),_T("Z坐标"));
	}
	// Song Chenguang add end on 2013-05-04

	if (pCScanView->m_SelArea>0)
	{
		// wangbingfu modify start on 2015-07-08
		float fZhanJilv = 0;
		if(!bUseHanHeData)
		{
			fZhanJilv = ccMianji * 100 / pCScanView->m_SelArea;
		}
		else
		{
			fZhanJilv = 100 - pCScanView->m_dbFlawArea * 100 / pCScanView->m_SelArea;
		}
		// wangbingfu modify end on 2015-07-08
		CString strZhanJiLv;
		strZhanJiLv.Format(_T("%f"),fZhanJilv);
		pv.ExSetTxt(_T("D15:J15"),strZhanJiLv);
	}

	pv.ExShow();
	return;
	WPARAM wParam = MAKEWPARAM(ID_FILE_PRINT_PREVIEW, 0);
	// machao modify start on 2013-08-14
// 	if ( m_pParentWnd->m_nObjectNum == 1 )
// 	{
// 		m_pParentWnd->m_pCScanView->SendMessage(WM_COMMAND, wParam, 0) ;
// 	}
// 	else
// 	{
// 		m_pParentWnd->m_pCScanView2->SendMessage(WM_COMMAND, wParam, 0);
// 	}
	m_pParentWnd->m_pCurCScanView->SendMessage(WM_COMMAND, wParam, 0);
	// machao modify end on 2013-08-14
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::TableCreate"),"Leave");
}
//machao add start on 2013-07-31
void CSurfaceDetailDlg::TableCreateZhuZhouShiDai()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::TableCreate"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	setList();

	CString strTempPath;
	TCHAR szTempPath [100];
	DWORD dwSize=100;
	DWORD dwResult=GetTempPath(dwSize,szTempPath);//获取临时文件夹路径 

	CString timeStr;
	CTime day;
	day=CTime::GetCurrentTime();
	int filenameday,filenamemonth,filenameyear,filehour,filemin,filesec;
	filenameday=day.GetDay();//dd
	filenamemonth=day.GetMonth();//mm月份
	filenameyear=day.GetYear();//yyyy
	filehour=day.GetHour();//hh
	filemin=day.GetMinute();//mm分钟
	filesec=day.GetSecond();//ss
	timeStr.Format(_T("%s%04d%02d%02d%02d%02d%02d"),szTempPath,filenameyear,filenamemonth,filenameday,filehour,filemin,filesec);
	CString strSCANDate;
	strSCANDate.Format(_T("%04d-%02d-%02d"),filenameyear,filenamemonth,filenameday);
	strTempPath=timeStr+_T(".c.bmp");


	BOOL bGetCPic=m_pParentWnd->SavePicByPath(strTempPath,true);

	CPrintView pv;
	CLSID clsid;
	if(CLSIDFromString(CComBSTR("ActiveXExtension.PrintView"), &clsid)==S_OK)
	{
		pv.CreateDispatch(clsid,NULL);
	}
	CMechanicalControlPanel* pMechanicalCtrlPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;



	;
	CProbeSet probeset1;
	if(CLSIDFromString(CComBSTR("ActiveXExtension.ProbeSet"), &clsid)==S_OK)
	{
		probeset1.CreateDispatch(clsid,NULL);
	}
	probeset1.SetDir(pMechanicalCtrlPanel->m_strDataFolder);


	pv.ExLoad(); 

	if(bGetCPic)
	{
		pv.ExSetPic(_T("A18:O37"),strTempPath);  
	}

	CString strList[6] = {pFrame->m_ProbeSet.getVal(_T("uName")),
		pFrame->m_ProbeSet.getVal(_T("uClass")),
		pFrame->m_ProbeSet.getVal(_T("oMaterial")),
		pFrame->m_ProbeSet.getVal(_T("oType")),
		pFrame->m_ProbeSet.getVal(_T("oId")),
		pFrame->m_ProbeSet.getVal(_T("oSize"))};

	//machao modify start on 2013-07-31
//	pv.ExSetTxt(_T("O11"),strList[0]);
	pv.ExSetTxt(_T("O10"),strList[0]);
	pv.ExSetTxt(_T("D11:J11"),strList[1]);
	pv.ExSetTxt(_T("D4:J4"),strList[2]);
	pv.ExSetTxt(_T("D9:J9"),strList[3]);
	pv.ExSetTxt(_T("O3"),strList[4]);
	pv.ExSetTxt(_T("O4"),strList[5]);
	pv.ExSetTxt(_T("O9"),strSCANDate);
//	pv.ExSetTxt(_T("D10:J10"),strSCANDate);
	//machao modify end on 2013-07-31

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CString strMachine;
	strMachine.Format(_T("%s"), pApp->m_IniParams.m_info.wcMechineType);
	pv.ExSetTxt(_T("D7:J7"),strMachine);
	CString strUserCompany;
	strUserCompany.Format(_T("%s"), pApp->m_IniParams.m_info.wcUserCompany);
	pv.ExSetTxt(_T("O8"),strUserCompany);

	//{_T("序号:"), _T("MinX:"),_T("MinY"),_T("MaxX:"),_T("MaxY:"),_T("深度:"),_T("面积:"),
	//	_T("位置危险性"),_T("分析结果"),_T("处理意见"),_T("备注"),_T("A扫设置"),_T("导出设置")};
	POSITION pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();
	int cc=1;
	int ccc=0;
	float ccMianji=0.0;
	float fMaxArea=0.0;
	float fMaxLength=0.0;
	int maxRow=41;

	//增加原点偏移输出  yachang.huang add start on 2012-11-19
	CCScanParamPanel* m_pCScanParam = m_pParentWnd;
	CCScanView* m_pCScanView = NULL;
	if(m_pCScanParam == pFrame->m_wndCScanParamDockBar.m_pCScanParamView)
	{
		m_pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	}
	else if(m_pCScanParam == pFrame->m_wndCScanParamDockBar.m_pCScanParamView2)
	{
		m_pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
	}
	else
	{
		;//
	}


	maxRow++;

	pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();
	cc=0;

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

	if(pCScanView->m_nAxisSelect == 3
		|| pCScanView->m_nAxisSelect == 4)	// wangbingfu add on 2013-01-09
	{
		pv.ExSetTxt(_T("B40"),_T("R角度"));
	}
	// Song Chenguang add start on 2013-05-04
	else if (pCScanView->m_nAxisSelect == 5)
	{
		pv.ExSetTxt(_T("B40"),_T("W角度"));
	}

	pv.ExShow();
	return;
	WPARAM wParam = MAKEWPARAM(ID_FILE_PRINT_PREVIEW, 0);

	m_pParentWnd->m_pCurCScanView->SendMessage(WM_COMMAND, wParam, 0);

}
//machao add end on 2013-07-31

void CSurfaceDetailDlg::OnBnClickedButtonGetascan()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnBnClickedButtonGetascan"),"Enter");
	ShowWindow(SW_HIDE);
	SetTimer(TIMER_GET_ASCAN,50,NULL);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnBnClickedButtonGetascan"),"Leave");
}

void CSurfaceDetailDlg::GetAScan()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::GetAScan"),"Enter");
	setList();

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CUltrasonicImmersedTestView* i_pAScanView = (CUltrasonicImmersedTestView*)pFrame->m_wndSplitter.GetPane(1,0);
	CPicData i_wave = i_pAScanView->m_stData;

	CString strTempPath;
	TCHAR szTempPath [100];
	DWORD dwSize=100;
	DWORD dwResult=GetTempPath(dwSize,szTempPath);//获取临时文件夹路径 

	CString timeStr;
	CTime day;
	day=CTime::GetCurrentTime();
	int filenameday,filenamemonth,filenameyear,filehour,filemin,filesec;
	filenameday=day.GetDay();//dd
	filenamemonth=day.GetMonth();//mm月份
	filenameyear=day.GetYear();//yyyy
	filehour=day.GetHour();//hh
	filemin=day.GetMinute();//mm分钟
	filesec=day.GetSecond();//ss
	timeStr.Format(_T("%s%04d%02d%02d%02d%02d%02d"),szTempPath,filenameyear,filenamemonth,filenameday,filehour,filemin,filesec);

	strTempPath=timeStr+_T(".a.bmp");

	POSITION pos2 = m_lstSurfaceResult.GetFirstSelectedItemPosition(); 
	if(pos2   ==   NULL) 
		TRACE0( "No   items   were   selected!\n "); 
	else 
	{ 
		int   nItem   =   m_lstSurfaceResult.GetNextSelectedItem(pos2); 
		int i=0;
		for(POSITION pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();pos;i++)
		{
			FlawNode* node = &m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);
			if (i==nItem)
			{
				ShowWindow(SW_HIDE);
				Invalidate();
				if(m_pParentWnd->SavePicByPath(strTempPath,false))
				{

					node->m_strAscanPath += _T("@")+strTempPath;  //把临时路径保存到伤点成员变量m_strAscanPath，多次获取时路径相加以“@”分割   yachang.huang  modify  on  2012-11-23
				}
				ShowWindow(SW_SHOW);
				Invalidate();

				memcpy(node->m_aScan->m_wData, &i_wave.m_wData , sizeof(WORD) * MONITOR_DATA_SIZE/AD_SIZE);

				node->m_boolHasAscan = true;
				MessageBox(_T("获取成功！"), _T("提示"), MB_OK);
				break;
			}
		}
	} 
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::GetAScan"),"Leave");
}
//void CSurfaceDetailDlg::OnHdnItemchangedListSurfaceDetail(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

//void CSurfaceDetailDlg::OnNMSetfocusListSurfaceDetail(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

void CSurfaceDetailDlg::OnNMClickListSurfaceDetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnNMClickListSurfaceDetail"),"Enter");
	// WangQianfeng add start on 2012-11-02
	if (pNMHDR == NULL)
	{
		MessageBox(_T("CSurfaceDetailDlg::OnNMClickListSurfaceDetail,pNMHDR = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnNMClickListSurfaceDetail"),"Leave");
		return;
	}
	if (pResult == NULL)
	{
		MessageBox(_T("CSurfaceDetailDlg::OnNMClickListSurfaceDetail,pResult = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnNMClickListSurfaceDetail"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	// TODO: Add your control notification handler code here
	POSITION pos2 = m_lstSurfaceResult.GetFirstSelectedItemPosition(); 
	if(pos2   ==   NULL) 
	{
		TRACE0( "No   items   were   selected!\n "); 
		GetDlgItem(IDC_STATIC_SURFACE_TITLE)->SetWindowText(_T("面伤列表："));
	}
	else 
	{ 
		int   nItem   =   m_lstSurfaceResult.GetNextSelectedItem(pos2); 
		TRACE("selected index : %d \n",nItem);
		//IDC_STATIC
		CString str;
		str.Format(_T("面伤列表：  当前选择序号为 %d"), nItem);
		GetDlgItem(IDC_STATIC_SURFACE_TITLE)->SetWindowText(str);

		// WangQianfeng add start on 2013-06-27
		TCHAR szBufSave[256];
		LVITEM itemSave;
		itemSave.iItem = nItem;
		itemSave.iSubItem = 7;
		itemSave.mask = LVIF_TEXT;
		itemSave.pszText = szBufSave;
		itemSave.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemSave);

		if (0 == _tcscmp(szBufSave,_T("保存")))
		{
			GetDlgItem(IDC_BUTTON_GETASCAN)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_GETASCAN)->EnableWindow(FALSE);
		}
		// WangQianfeng add end on 2013-06-27
		
		TCHAR szBufXMin[256];
		LVITEM itemMinX;
		itemMinX.iItem = nItem;
		itemMinX.iSubItem = 1;
		itemMinX.mask = LVIF_TEXT;
		itemMinX.pszText = szBufXMin;
		itemMinX.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemMinX);

		TCHAR szBufYMin[256];
		LVITEM itemMinY;
		itemMinY.iItem = nItem;
		itemMinY.iSubItem = 2;
		itemMinY.mask = LVIF_TEXT;
		itemMinY.pszText = szBufYMin;
		itemMinY.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemMinY);

		TCHAR szBufXMax[256];
		LVITEM itemMaxX;
		itemMaxX.iItem = nItem;
		itemMaxX.iSubItem = 3;
		itemMaxX.mask = LVIF_TEXT;
		itemMaxX.pszText = szBufXMax;
		itemMaxX.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemMaxX);

		TCHAR szBufYMax[256];
		LVITEM itemMaxY;
		itemMaxY.iItem = nItem;
		itemMaxY.iSubItem = 4;
		itemMaxY.mask = LVIF_TEXT;
		itemMaxY.pszText = szBufYMax;
		itemMaxY.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemMaxY);

		double dXMin = _wtof(szBufXMin);
		double dYMin = _wtof(szBufYMin);
		double dXMax = _wtof(szBufXMax);
		double dYMax = _wtof(szBufYMax);
		
		// wangbingfu modify start at 20012-11-12
		if (m_Offset_Coordinate.y >= 0)
		{
			dYMin = dYMin + m_Offset_Coordinate.y;
			dYMax = dYMax + m_Offset_Coordinate.y;
		} 
		if (m_Offset_Coordinate.x >= 0)
		{
			dXMin = dXMin + m_Offset_Coordinate.x;
			dXMax = dXMax + m_Offset_Coordinate.x;
		} 
		// wangbingfu modify end at 20012-11-12

		double dXCenter = (dXMin + dXMax) / 2.0;
		double dYCenter = (dYMin + dYMax) / 2.0;

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
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
		
		// Song Chenguang modfiy start on 2013-09-10
		CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0, 0);
		int nAxisIndex = pMovePathView->m_GraphMove.GetAxisSelect();
		float fDiameter = pMovePathView->m_GraphMove.GetPipR();
		if (nAxisIndex == 3 || nAxisIndex == 4)
		{
			if (m_pParentWnd->m_nPipeXMode == 0)
			{
				pCScanView->m_stLogicalFlaw.x = (float)dXCenter;
				pCScanView->m_stLogicalFlaw.y = (float)dYCenter / 360.0f * PI * fDiameter;
			}
		}
		// machao add start on 2013-12-4
		else if (nAxisIndex == 5 && m_pParentWnd->m_nPipeXMode == 0)
		{
			fDiameter = pMovePathView->m_GraphMove.Get_WallDiameter();
			pCScanView->m_stLogicalFlaw.x = (float)dXCenter / 360.0f * PI * fDiameter;
			pCScanView->m_stLogicalFlaw.y = (float)dYCenter;
		}
		// machao add end on 2013-12-4
		else
		{
			pCScanView->m_stLogicalFlaw.x = (float)dXCenter;
			pCScanView->m_stLogicalFlaw.y = (float)dYCenter;
		}	
		// Song Chenguang modfiy end on 2013-09-10
		pCScanView->Invalidate(TRUE);
	}
	*pResult = 0;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnNMClickListSurfaceDetail"),"Leave");
}

// 查找指定范围内伤点最大幅值
float CSurfaceDetailDlg::SearchMaxAmp(const float& fXMin, const float& fXMax, const float& fYMin, const float& fYMax,
				   const int& nGateIndex, const int& nFeatureIndex)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::SearchMaxAmp"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
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

	float fValue = 0.0f;

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)m_pParentWnd->GetDocument();
	WORD wAECNum = pApp->GetCardNum();

	for (int i=0; i<wAECNum*2; i++)
	{
		if(pCScanView->m_bGuanLianFlag)	// 关联模式
		{
			if(pCScanView->m_nObjectNum == 1)	// 只画底波 单通道底波 双通道伤波
			{
				if(i == 1 || i == 3 || i == 5 || i == 7)
				{
					continue;
				}
			}
			else if(pCScanView->m_nObjectNum == 2)	// 只画伤波
			{
				if(i == 0 || i == 2 || i == 4 || i == 6)
				{
					continue;
				}
			}
			else
			{
				;//
			}
		}

		if (pDoc->m_lstFlawData[i].GetCount() > 0)
		{
			POSITION pos = pDoc->m_lstFlawData[i].GetHeadPosition();

			while (pos)
			{
				CFeatureInfo info = pDoc->m_lstFlawData[i].GetNext(pos);
				if (info.m_dX > fXMin && info.m_dX < fXMax && info.m_dY > fYMin && info.m_dY < fYMax &&
					info.m_pfFeatures[nGateIndex][nFeatureIndex] > fValue)
					fValue = info.m_pfFeatures[nGateIndex][nFeatureIndex];
			}
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::SearchMaxAmp"),"Leave");
	return fValue;
}

void CSurfaceDetailDlg::OnTimer(UINT_PTR nIDEvent)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnTimer"),"Enter");
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);
	if(nIDEvent == TIMER_CREATE_TABLE)
	{
		//machao modify start on 2013-07-31
//		TableCreate();
		if(g_nReportFormat == 1)
		{
			TableCreateZhuZhouShiDai();
		}
		else
		{
#if _BEIFANG_ZHONGGONG
			// wangbingfu modify start on 2014-08-27
	//		TableCreateBeiFangZhongGong();
			TableCreateBeiFangZhongGong1();
			// wangbingfu modify end on 2014-08-27
#elif _HAFEI	// wangbingfu add strat on 2014-11-03
			TableCreateHaFei1();
#else
			TableCreate();
#endif
		}
		//machao modify end on 2013-07-31
	}
	else if(nIDEvent == TIMER_GET_ASCAN)
	{
		GetAScan();
	}
	else if(nIDEvent == TIMER_WAITT_FOR_DETAIL)
	{
		InitDialog();
		delete m_pMessageDlg;
		m_pMessageDlg = NULL;
	}
	ShowWindow(SW_SHOW);
	CDialog::OnTimer(nIDEvent);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnTimer"),"Leave");
}

/*
void CSurfaceDetailDlg::CombineDefect(PlaneFlawHeader& defect)
{
	;
}
*/
void CSurfaceDetailDlg::InitDialog()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::InitDialog"),"Enter");
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	// wangbingfu modify start at 20012-11-12
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

	if (pCScanView->m_Offset_Coordinate.y >= 0 || pCScanView->m_Offset_Coordinate.x >= 0)
	{
		m_Offset_Coordinate =  pCScanView->m_Offset_Coordinate;
	} 
	// wangbingfu modify end at 20012-11-12
	// 初始化结果列表
	InitResultList(&m_lstSurfaceResult);

	//清空链表
	m_pParentWnd->m_lstSurfaceDetail.RemoveAll();

	// TODO:  Add extra initialization here
	if (m_pParentWnd != NULL)
	{
		// 初始化结果列表
		//		InitResultList(&m_lstSurfaceResult);
		m_lstSurfaceResult.DeleteAllItems();

		DrawColorRate(); // 焊合率
		// 获取面伤信息
		VARIANT Result = m_pParentWnd->m_PlateDrawCtrl.GetPlaneFlawPoints2();

		PlaneFlawHeader stPFH = {0};
		long lLBound = 0;
		long lUBound = 0;
		::SafeArrayGetLBound(Result.parray, 1, &lLBound);		//一维数组下标
		::SafeArrayGetUBound(Result.parray, 1, &lUBound);		//一维数组上标

		char* buf = NULL;
		UINT Size = 0;
		// Song Chenguang modify start on 2013-06-29
		if (m_pParentWnd->m_nSurfaceSelType == 0)
		{
			if (m_pParentWnd->m_pCurCScanView->m_pcCustumFlawBuffer != NULL)
			{
				buf = m_pParentWnd->m_pCurCScanView->m_pcCustumFlawBuffer;
				Size = m_pParentWnd->m_pCurCScanView->m_lCustumFlawBufferSize;
			}
			
		}
		else if (m_pParentWnd->m_nSurfaceSelType == 1)
		{
			SafeArrayAccessData(Result.parray,   (void   **)&buf); 
			Size = (lUBound-lLBound+1);	
		}
		  
		// Song Chenguang modify start on 2013-06-29		

		int nZoomScale = m_pParentWnd->m_nZoomScale;

		int nPos = 0;
		int i = 0;
		float fMinArea = m_pParentWnd->m_fMinSurfaceArea * m_pParentWnd->m_nZoomScale * m_pParentWnd->m_nZoomScale;

		// songchenguang add start 2012-05-21
		BOOL bMaxAmp = pFrame->m_wndChannelInfo.m_pFormView->m_bMPMAltitudeDisplay;
		
		int nMaxIndex = m_pParentWnd->m_nFeatureIndex + 1;	// 特征值序号
		int nMaxAmpIndex = nMaxIndex;
		if (bMaxAmp)
		{
			BOOL bOpen[6] = {TRUE, pFrame->m_wndChannelInfo.m_pFormView->m_bFPFTimeDisplay,
				pFrame->m_wndChannelInfo.m_pFormView->m_bFPMTimeDisplay, pFrame->m_wndChannelInfo.m_pFormView->m_bMPFTimeDisplay,
				pFrame->m_wndChannelInfo.m_pFormView->m_bMPMTimeDisplay, pFrame->m_wndChannelInfo.m_pFormView->m_bFPMAltitudeDisplay};

			for (int i=0; i<nMaxIndex; i++)
			{
				if (!bOpen[i])
					nMaxAmpIndex--;
			}
		}

		int nGateIndex = m_pParentWnd->m_nGateIndex;	// 当前闸门序号
		for (int i=0; i<m_pParentWnd->m_nGateIndex; i++)
		{
			if (!pFrame->m_wndChannelInfo.m_pFormView->m_ChannelInfo->m_GateInfo[i].m_bOpen)
			{
				nGateIndex--;
			}
		}
		
		// wangbingfu add start on 2014-11-04 自选伤时，面伤合并，时间急不考虑代码结构
/*		CCScanParamPanel* pCScanParam = m_pParentWnd;
		if(false)//if(pCScanParam->m_nSurfaceSelType == 0)
		{
			int nPos1 = 0;
			double dMinX = 0.0f;
			double dMinY = 0.0f;
			double dMaxX = 0.0f;
			double dMaxY = 0.0f;
			double dZ = 0.0f;
			double dArea = 0.0f;
			double dMaxAmp =  0.0f;
			memset(&m_combineDefect, 0 ,sizeof(PlaneFlawHeader));

			while(nPos+sizeof(PlaneFlawHeader) < Size)
			{	
				memcpy(&stPFH, buf + nPos, sizeof(PlaneFlawHeader));		
				if(stPFH.area > fMinArea)
				{
					if(nPos1 == 0)
					{
						dMinX = stPFH.min_x;
						dMinY = stPFH.min_y;
						dMaxX = stPFH.max_x;
						dMaxY = stPFH.max_y;
						dZ = stPFH.z;
						dArea = stPFH.area;
						dMaxAmp = stPFH.fShangBoFuZhiMax;
					}
					else
					{
						if(dMinX > stPFH.min_x)
						{
							dMinX = stPFH.min_x;
						}
						if(dMinY > stPFH.min_y)
						{
							dMinY = stPFH.min_y;
						}
						if(dMaxX < stPFH.max_x)
						{
							dMaxX = stPFH.max_x;
						}
						if(dMaxY < stPFH.max_y)
						{
							dMaxY = stPFH.max_y;
						}
						if(dZ < stPFH.z)
						{
							dZ = stPFH.z;
						}
						if(dMaxAmp < stPFH.fShangBoFuZhiMax)
						{
							dMaxAmp = stPFH.fShangBoFuZhiMax;
						}
						dArea = dArea + stPFH.area;
					}
				}
				int sizetmp = sizeof(PlaneFlawHeader) + sizeof(HPOINT)*stPFH.PointsCount;

				nPos1 += sizetmp;
			}

			m_combineDefect.min_x = dMinX;
			m_combineDefect.min_y = dMinY;
			m_combineDefect.max_x = dMaxX;
			m_combineDefect.max_y = dMaxY;
			m_combineDefect.z = dZ;
			m_combineDefect.fShangBoFuZhiMax = dMaxAmp;
			m_combineDefect.area = dArea;
			
			PlaneFlawHeader stPFH;
			memcpy(&stPFH, &m_combineDefect, sizeof(PlaneFlawHeader));
			{
				// 加面伤到列表框
				LV_ITEM lvi;

				lvi.mask = LVIF_TEXT;// | LVIF_PARAM;
				lvi.iItem = m_lstSurfaceResult.GetItemCount();
				//   lvi.lParam = (LPARAM)&fi;

				lvi.iSubItem = 0;
				CString strIndex;
				strIndex.Format(_T("%d"), i);
				lvi.pszText = strIndex.GetBuffer(strIndex.GetLength());	// 序号
				int nRes = m_lstSurfaceResult.InsertItem(&lvi);

				CString strMinX;
				//strMinX.Format(_T("%.2f"), stPFH.min_x / nZoomScale + m_pParentWnd->m_fLogicalFromX);
				CString strMinY;
				//strMinY.Format(_T("%.2f"), stPFH.min_y / nZoomScale + m_pParentWnd->m_fLogicalFromY);
				CString strMaxX;
				//strMaxX.Format(_T("%.2f"), stPFH.max_x / nZoomScale + m_pParentWnd->m_fLogicalFromX);
				CString strMaxY;
				//strMaxY.Format(_T("%.2f"), stPFH.max_y / nZoomScale + m_pParentWnd->m_fLogicalFromY);
				CString strZ;
				strZ.Format(_T("%.2f"), stPFH.z);
				CString strArea;
				strArea.Format(_T("%.2f"), stPFH.area / (nZoomScale * nZoomScale));

				LOGICAL_POINT_t i_point;
				i_point.x=0;
				i_point.y=0;
				int nPipeXMode = 0;
				int nAxisSelect = 0;
				double dPipeDiameter = 0;

				// wangbingfu modify start at 20012-11-12
				if (pCScanView->m_Offset_Coordinate.y >= 0 || pCScanView->m_Offset_Coordinate.x >= 0)
				{
					i_point=pCScanView->m_Offset_Coordinate;
				} 
				nPipeXMode = pCScanView->m_nPipeXMode;
				dPipeDiameter = pCScanView->m_dPipeDiameter;
				nAxisSelect = pCScanView->m_nAxisSelect;
				// wangbingfu modify end at 20012-11-12

				float fLogicalFromY = m_pParentWnd->m_fLogicalFromY;
				float fLogicalFromX = m_pParentWnd->m_fLogicalFromX;	//Song Chenguang add on 2013-05-04
				if(nAxisSelect == 3 ||
					nAxisSelect == 4)	//管材 Song Chenguang modified on 2013-05-04
				{
					if(nPipeXMode == 0)	// 角度
					{
						stPFH.min_y = stPFH.min_y * (360.0f / (PI * dPipeDiameter));
						stPFH.max_y	= stPFH.max_y * (360.0f / (PI * dPipeDiameter));
						fLogicalFromY = static_cast<float>(fLogicalFromY * (360.0f / (PI * dPipeDiameter)));
					}
				}
				// Song Chenguang add start on 2013-05-04
				else if (nAxisSelect == 5)
				{
					if(nPipeXMode == 0)	// 角度
					{
						stPFH.min_x = stPFH.min_x * (360.0f / (PI * dPipeDiameter));
						stPFH.max_x	= stPFH.max_x * (360.0f / (PI * dPipeDiameter));
						fLogicalFromX = static_cast<float>(fLogicalFromX * (360.0f / (PI * dPipeDiameter)));
					}
				}
				// Song Chenguang add end on 2013-05-04

				// Song Chenguang modify start on 2013-07-01
				// 				strMinX.Format(_T("%.2f"), stPFH.min_x / nZoomScale + fLogicalFromX - i_point .x );
				// 				strMinY.Format(_T("%.2f"), stPFH.min_y / nZoomScale + fLogicalFromY - i_point .y );
				// 				strMaxX.Format(_T("%.2f"), stPFH.max_x / nZoomScale + fLogicalFromX - i_point .x );
				// 				strMaxY.Format(_T("%.2f"), stPFH.max_y / nZoomScale + fLogicalFromY - i_point .y );
				strMinX.Format(_T("%.2f"), stPFH.min_x / nZoomScale - i_point .x );
				strMinY.Format(_T("%.2f"), stPFH.min_y / nZoomScale - i_point .y );
				strMaxX.Format(_T("%.2f"), stPFH.max_x / nZoomScale - i_point .x );
				strMaxY.Format(_T("%.2f"), stPFH.max_y / nZoomScale - i_point .y );
				// Song Chenguang modify end on 2013-07-01

				int iSubItem = 0;
				m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strMinX);	// minX
				m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strMinY);	// minY
				m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strMaxX);	// maxX
				m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strMaxY);	// maxY
				m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strZ);	// 深度	
				m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strArea);	// 面积	
				m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, _T("不保存"));	// A扫存储设置	
				m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, _T("导出"));	// 导出设置	

				// yachang.huang add start on 2012-11-26
				CString strFuzhi;
				strFuzhi.Format(_T("%.2f"),stPFH.fShangBoFuZhiMax );
				m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strFuzhi); 

				// yachang.huang add end on 2012-11-26



				// 				// songchenguang add start on 2012-05-21
				// 				// 若面积小于2mm~2,在最大赋值列显示此伤范围内最大幅值
				// 				float fArea = static_cast<float>(stPFH.area / (nZoomScale * nZoomScale));
				// 				if (fArea < 2 && bMaxAmp)
				// 				{
				// 					float fKuo = 0.5f;
				// 					// 查找伤范围内最大幅值
				// 					float fMaxAmp = SearchMaxAmp(static_cast<float>(stPFH.min_x / nZoomScale + m_pParentWnd->m_fLogicalFromX - fKuo), static_cast<float>(stPFH.max_x / nZoomScale + m_pParentWnd->m_fLogicalFromX + fKuo),
				// 						static_cast<float>(stPFH.min_y / nZoomScale + m_pParentWnd->m_fLogicalFromY - fKuo), static_cast<float>(stPFH.max_y / nZoomScale + m_pParentWnd->m_fLogicalFromY + fKuo), nGateIndex, nMaxAmpIndex);
				// 					CString strFuzhi;
				// 					strFuzhi.Format(_T("%.2f"), fMaxAmp);
				// 					m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strFuzhi);	// 导出设置	
				// 				}
				// 				else
				// 				{
				// 					CString strFuzhi = _T("100.0");
				// 					m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strFuzhi);	// 导出设置	
				// 				}
				// 
				// 				// songchenguang add end on 2012-05-21
				++iSubItem;	// 说明
				++iSubItem;	// 说明
				++iSubItem;	// 说明
				++iSubItem;	// 说明

				i++;

				FlawNode node;
				m_pParentWnd->m_lstSurfaceDetail.AddTail(node);
			}
		}
		else*/	// wangbingfu add end on 2014-11-04 
		{
			// songchenguang add end 2012-05-21
			while(nPos+sizeof(PlaneFlawHeader) < Size)
			{	
				memcpy(&stPFH, buf + nPos, sizeof(PlaneFlawHeader));		
				if(stPFH.area > fMinArea)
				{
					// 加面伤到列表框
					LV_ITEM lvi;

					lvi.mask = LVIF_TEXT;// | LVIF_PARAM;
					lvi.iItem = m_lstSurfaceResult.GetItemCount();
					//   lvi.lParam = (LPARAM)&fi;

					lvi.iSubItem = 0;
					CString strIndex;
					strIndex.Format(_T("%d"), i);
					lvi.pszText = strIndex.GetBuffer(strIndex.GetLength());	// 序号
					int nRes = m_lstSurfaceResult.InsertItem(&lvi);

					CString strMinX;
					//strMinX.Format(_T("%.2f"), stPFH.min_x / nZoomScale + m_pParentWnd->m_fLogicalFromX);
					CString strMinY;
					//strMinY.Format(_T("%.2f"), stPFH.min_y / nZoomScale + m_pParentWnd->m_fLogicalFromY);
					CString strMaxX;
					//strMaxX.Format(_T("%.2f"), stPFH.max_x / nZoomScale + m_pParentWnd->m_fLogicalFromX);
					CString strMaxY;
					//strMaxY.Format(_T("%.2f"), stPFH.max_y / nZoomScale + m_pParentWnd->m_fLogicalFromY);
					CString strZ;
					strZ.Format(_T("%.2f"), stPFH.z);
					CString strArea;
					strArea.Format(_T("%.2f"), stPFH.area / (nZoomScale * nZoomScale));

					LOGICAL_POINT_t i_point;
					i_point.x=0;
					i_point.y=0;
					int nPipeXMode = 0;
					int nAxisSelect = 0;
					double dPipeDiameter = 0;
					
					// wangbingfu modify start at 20012-11-12
					if (pCScanView->m_Offset_Coordinate.y >= 0 || pCScanView->m_Offset_Coordinate.x >= 0)
					{
						i_point=pCScanView->m_Offset_Coordinate;
					} 
					nPipeXMode = pCScanView->m_nPipeXMode;
					dPipeDiameter = pCScanView->m_dPipeDiameter;
					nAxisSelect = pCScanView->m_nAxisSelect;
					// wangbingfu modify end at 20012-11-12
					
					float fLogicalFromY = m_pParentWnd->m_fLogicalFromY;
					float fLogicalFromX = m_pParentWnd->m_fLogicalFromX;	//Song Chenguang add on 2013-05-04
					if(nAxisSelect == 3 ||
						nAxisSelect == 4)	//管材 Song Chenguang modified on 2013-05-04
					{
						if(nPipeXMode == 0)	// 角度
						{
							stPFH.min_y = stPFH.min_y * (360.0f / (PI * dPipeDiameter));
							stPFH.max_y	= stPFH.max_y * (360.0f / (PI * dPipeDiameter));
							fLogicalFromY = static_cast<float>(fLogicalFromY * (360.0f / (PI * dPipeDiameter)));
						}
					}
					// Song Chenguang add start on 2013-05-04
					else if (nAxisSelect == 5)
					{
						if(nPipeXMode == 0)	// 角度
						{
							stPFH.min_x = stPFH.min_x * (360.0f / (PI * dPipeDiameter));
							stPFH.max_x	= stPFH.max_x * (360.0f / (PI * dPipeDiameter));
							fLogicalFromX = static_cast<float>(fLogicalFromX * (360.0f / (PI * dPipeDiameter)));
						}
					}
					// Song Chenguang add end on 2013-05-04

					// Song Chenguang modify start on 2013-07-01
	// 				strMinX.Format(_T("%.2f"), stPFH.min_x / nZoomScale + fLogicalFromX - i_point .x );
	// 				strMinY.Format(_T("%.2f"), stPFH.min_y / nZoomScale + fLogicalFromY - i_point .y );
	// 				strMaxX.Format(_T("%.2f"), stPFH.max_x / nZoomScale + fLogicalFromX - i_point .x );
	// 				strMaxY.Format(_T("%.2f"), stPFH.max_y / nZoomScale + fLogicalFromY - i_point .y );
					strMinX.Format(_T("%.2f"), stPFH.min_x / nZoomScale - i_point .x );
					strMinY.Format(_T("%.2f"), stPFH.min_y / nZoomScale - i_point .y );
					strMaxX.Format(_T("%.2f"), stPFH.max_x / nZoomScale - i_point .x );
					strMaxY.Format(_T("%.2f"), stPFH.max_y / nZoomScale - i_point .y );
					// Song Chenguang modify end on 2013-07-01

					int iSubItem = 0;
					m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strMinX);	// minX
					m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strMinY);	// minY
					m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strMaxX);	// maxX
					m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strMaxY);	// maxY
					m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strZ);	// 深度	
					m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strArea);	// 面积	
					m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, _T("不保存"));	// A扫存储设置	
					m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, _T("导出"));	// 导出设置	

					// yachang.huang add start on 2012-11-26
					CString strFuzhi;
					strFuzhi.Format(_T("%.2f"),stPFH.fShangBoFuZhiMax );
					m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strFuzhi); 

					// yachang.huang add end on 2012-11-26



	// 				// songchenguang add start on 2012-05-21
	// 				// 若面积小于2mm~2,在最大赋值列显示此伤范围内最大幅值
	// 				float fArea = static_cast<float>(stPFH.area / (nZoomScale * nZoomScale));
	// 				if (fArea < 2 && bMaxAmp)
	// 				{
	// 					float fKuo = 0.5f;
	// 					// 查找伤范围内最大幅值
	// 					float fMaxAmp = SearchMaxAmp(static_cast<float>(stPFH.min_x / nZoomScale + m_pParentWnd->m_fLogicalFromX - fKuo), static_cast<float>(stPFH.max_x / nZoomScale + m_pParentWnd->m_fLogicalFromX + fKuo),
	// 						static_cast<float>(stPFH.min_y / nZoomScale + m_pParentWnd->m_fLogicalFromY - fKuo), static_cast<float>(stPFH.max_y / nZoomScale + m_pParentWnd->m_fLogicalFromY + fKuo), nGateIndex, nMaxAmpIndex);
	// 					CString strFuzhi;
	// 					strFuzhi.Format(_T("%.2f"), fMaxAmp);
	// 					m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strFuzhi);	// 导出设置	
	// 				}
	// 				else
	// 				{
	// 					CString strFuzhi = _T("100.0");
	// 					m_lstSurfaceResult.SetItemText(lvi.iItem, ++iSubItem, strFuzhi);	// 导出设置	
	// 				}
	// 
	// 				// songchenguang add end on 2012-05-21
					++iSubItem;	// 说明
					++iSubItem;	// 说明
					++iSubItem;	// 说明
					++iSubItem;	// 说明
					
					i++;

					FlawNode node;
					m_pParentWnd->m_lstSurfaceDetail.AddTail(node);
				}
				int sizetmp = sizeof(PlaneFlawHeader) + sizeof(HPOINT)*stPFH.PointsCount;

				nPos += sizetmp;
			}
		}

		// Song Chenguang  modify start on 2013-06-29
		if (m_pParentWnd->m_nSurfaceSelType == 1)
		{
			SafeArrayUnaccessData(Result.parray);
			SafeArrayDestroy(Result.parray);
		}		
		// Song Chenguang  modify end on 2013-06-29
	}

	CButton* i_button = (CButton*)GetDlgItem(IDC_CHECK_SHOW_PROBE);
	i_button->SetCheck(0);
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::InitDialog"),"Leave");
}

void CSurfaceDetailDlg::OnLvnKeydownListSurfaceDetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnLvnKeydownListSurfaceDetail"),"Enter");
	// WangQianfeng add start on 2012-11-02
	if (pNMHDR == NULL)
	{
		MessageBox(_T("CSurfaceDetailDlg::OnLvnKeydownListSurfaceDetail,pNMHDR = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnLvnKeydownListSurfaceDetail"),"Leave");
		return;
	}
	if (pResult == NULL)
	{
		MessageBox(_T("CSurfaceDetailDlg::OnLvnKeydownListSurfaceDetail,pResult = null"), _T("警告"), MB_OK|MB_ICONERROR);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnLvnKeydownListSurfaceDetail"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02

	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	POSITION pos2 = m_lstSurfaceResult.GetFirstSelectedItemPosition(); 
	if(pos2 == NULL) 
	{
		TRACE0( "No   items   were   selected!\n "); 
		GetDlgItem(IDC_STATIC_SURFACE_TITLE)->SetWindowText(_T("面伤列表："));
	}
	else 
	{ 
		int   nItem   =   m_lstSurfaceResult.GetNextSelectedItem(pos2); 
		if(pLVKeyDow->wVKey==VK_UP)
		{
			nItem--;
		}
		else if(pLVKeyDow->wVKey==VK_DOWN)
		{
			nItem++;
		}		
		TRACE("selected index : %d \n",nItem);
		//IDC_STATIC
		CString str;
		str.Format(_T("面伤列表：  当前选择序号为 %d"), nItem);
		GetDlgItem(IDC_STATIC_SURFACE_TITLE)->SetWindowText(str);

		// WangQianfeng add start on 2013-06-27
		TCHAR szBufSave[256];
		LVITEM itemSave;
		itemSave.iItem = nItem;
		itemSave.iSubItem = 7;
		itemSave.mask = LVIF_TEXT;
		itemSave.pszText = szBufSave;
		itemSave.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemSave);

		if (0 == _tcscmp(szBufSave,_T("保存")))
		{
			GetDlgItem(IDC_BUTTON_GETASCAN)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_GETASCAN)->EnableWindow(FALSE);
		}
		// WangQianfeng add end on 2013-06-27
		TCHAR szBufXMin[256];
		LVITEM itemMinX;
		itemMinX.iItem = nItem;
		itemMinX.iSubItem = 1;
		itemMinX.mask = LVIF_TEXT;
		itemMinX.pszText = szBufXMin;
		itemMinX.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemMinX);

		TCHAR szBufYMin[256];
		LVITEM itemMinY;
		itemMinY.iItem = nItem;
		itemMinY.iSubItem = 2;
		itemMinY.mask = LVIF_TEXT;
		itemMinY.pszText = szBufYMin;
		itemMinY.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemMinY);

		TCHAR szBufXMax[256];
		LVITEM itemMaxX;
		itemMaxX.iItem = nItem;
		itemMaxX.iSubItem = 3;
		itemMaxX.mask = LVIF_TEXT;
		itemMaxX.pszText = szBufXMax;
		itemMaxX.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemMaxX);

		TCHAR szBufYMax[256];
		LVITEM itemMaxY;
		itemMaxY.iItem = nItem;
		itemMaxY.iSubItem = 4;
		itemMaxY.mask = LVIF_TEXT;
		itemMaxY.pszText = szBufYMax;
		itemMaxY.cchTextMax = 256;
		m_lstSurfaceResult.GetItem(&itemMaxY);

		double dXMin = _wtof(szBufXMin);
		double dYMin = _wtof(szBufYMin);
		double dXMax = _wtof(szBufXMax);
		double dYMax = _wtof(szBufYMax);

		double dXCenter = (dXMin + dXMax) / 2.0;
		double dYCenter = (dYMin + dYMax) / 2.0;

		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
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

		// Song Chenguang modfiy start on 2013-09-10
		CMovePathView* pMovePathView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0, 0);
		int nAxisIndex = pMovePathView->m_GraphMove.GetAxisSelect();
		float fDiameter = pMovePathView->m_GraphMove.GetPipR();
		if (nAxisIndex == 3 || nAxisIndex == 4)
		{
			if (m_pParentWnd->m_nPipeXMode == 0)
			{
				pCScanView->m_stLogicalFlaw.x = (float)dXCenter;
				pCScanView->m_stLogicalFlaw.y = (float)dYCenter / 360.0f * PI * fDiameter;
			}
		}
		// machao add start on 2013-12-4
		else if (nAxisIndex == 5 && m_pParentWnd->m_nPipeXMode == 0)
		{
			fDiameter = pMovePathView->m_GraphMove.Get_WallDiameter();
			pCScanView->m_stLogicalFlaw.x = (float)dXCenter / 360.0f * PI * fDiameter;
			pCScanView->m_stLogicalFlaw.y = (float)dYCenter;
		}
		// machao add end on 2013-12-4
		else
		{
			pCScanView->m_stLogicalFlaw.x = (float)dXCenter;
			pCScanView->m_stLogicalFlaw.y = (float)dYCenter;
		}	
		// Song Chenguang modfiy end on 2013-09-10
		pCScanView->Invalidate(TRUE);
	}
	*pResult = 0;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CSurfaceDetailDlg::OnLvnKeydownListSurfaceDetail"),"Leave");
}


CString CSurfaceDetailDlg::VariantToCString(VARIANT var)
{
	CString strValue;
	_variant_t var_t;
	_bstr_t bst_t;
	time_t cur_time;
	CTime time_value;
	COleCurrency var_currency;
	switch(var.vt)
	{
	case VT_EMPTY:
		strValue=_T("");
		break;
	case VT_UI1:
		strValue.Format(_T("%d"),var.bVal);
		break;
	case VT_I2:
		strValue.Format(_T("%d"),var.iVal);
		break;
	case VT_I4:
		strValue.Format(_T("%d"),var.lVal);
		break;
	case VT_R4:
		strValue.Format(_T("%f"),var.fltVal);
		break;
	case VT_R8:
		strValue.Format(_T("%f"),var.dblVal);
		break;
	case VT_CY:
		var_currency=var;
		strValue=var_currency.Format(0);
		break;
	case VT_BSTR:
		var_t=var;
		bst_t=var_t;
		strValue.Format (_T("%s"),(const char*)bst_t);
		break;
	case VT_NULL:
		strValue=_T("");
		break;
	case VT_DATE:
		cur_time = (long)var.date;
		time_value=cur_time;
		strValue=time_value.Format("%A,%B%d,%Y");
		break;
	case VT_BOOL:
		strValue.Format(_T("%d"),var.boolVal );
		break;
	default: 
		strValue=_T("");
		break;
	}
	return strValue;

}


void CSurfaceDetailDlg::TableCreateBeiFangZhongGong()
{

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	setList();
	CString strTempPath;
	TCHAR szTempPath [100];
	DWORD dwSize=100;
	DWORD dwResult=GetTempPath(dwSize,szTempPath);//获取临时文件夹路径 

	CString timeStr;
	CTime day;
	day=CTime::GetCurrentTime();
	int filenameday,filenamemonth,filenameyear,filehour,filemin,filesec;
	filenameday=day.GetDay();//dd
	filenamemonth=day.GetMonth();//mm月份
	filenameyear=day.GetYear();//yyyy
	filehour=day.GetHour();//hh
	filemin=day.GetMinute();//mm分钟
	filesec=day.GetSecond();//ss
	timeStr.Format(_T("%s%04d%02d%02d%02d%02d%02d"),szTempPath,filenameyear,filenamemonth,filenameday,filehour,filemin,filesec);
	CString strSCANDate;
	strSCANDate.Format(_T("%04d-%02d-%02d"),filenameyear,filenamemonth,filenameday);
	strTempPath=timeStr+_T(".c.bmp");


	BOOL bGetCPic=m_pParentWnd->SavePicByPath(strTempPath,true);


	CPrintView pv;
	CLSID clsid;
	if(CLSIDFromString(CComBSTR("ActiveXExtension.PrintView"), &clsid)==S_OK)
	{
		pv.CreateDispatch(clsid,NULL);
	}
	CMechanicalControlPanel* pMechanicalCtrlPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;


	CProbeSet probeset1;
	if(CLSIDFromString(CComBSTR("ActiveXExtension.ProbeSet"), &clsid)==S_OK)
	{
		probeset1.CreateDispatch(clsid,NULL);
	}
	probeset1.SetDir(pMechanicalCtrlPanel->m_strDataFolder);

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

	pv.ExLoad(); 

	if(bGetCPic)
	{
		pv.ExSetPic(_T("A18:O37"),strTempPath);  
	}

	CString strList[6] = {pFrame->m_ProbeSet.getVal(_T("uName")),
		pFrame->m_ProbeSet.getVal(_T("uClass")),
		pFrame->m_ProbeSet.getVal(_T("oMaterial")),
		pFrame->m_ProbeSet.getVal(_T("oType")),
		pFrame->m_ProbeSet.getVal(_T("oId")),
		pFrame->m_ProbeSet.getVal(_T("oSize"))};
/*
	//machao modify start on 2013-07-30 //?
	pv.ExSetTxt(_T("O11"),strList[0]);
	//machao modify end on 2013-07-30
	pv.ExSetTxt(_T("D11:J11"),strList[1]);
	pv.ExSetTxt(_T("D4:J4"),strList[2]);
	pv.ExSetTxt(_T("D9:J9"),strList[3]);
	pv.ExSetTxt(_T("O3"),strList[4]);
	pv.ExSetTxt(_T("O4"),strList[5]);
	pv.ExSetTxt(_T("D10:J10"),strSCANDate);

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CString strMachine;
	strMachine.Format(_T("%s"), pApp->m_IniParams.m_info.wcMechineType);
	pv.ExSetTxt(_T("D7:J7"),strMachine);
	CString strUserCompany;
	strUserCompany.Format(_T("%s"), pApp->m_IniParams.m_info.wcUserCompany);
	pv.ExSetTxt(_T("O8"),strUserCompany);
*/
	//{_T("序号:"), _T("MinX:"),_T("MinY"),_T("MaxX:"),_T("MaxY:"),_T("深度:"),_T("面积:"),
	//	_T("位置危险性"),_T("分析结果"),_T("处理意见"),_T("备注"),_T("A扫设置"),_T("导出设置")};
	POSITION pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();
	int cc=1;
	int ccc=0;
	float ccMianji=0.0;
	float fMaxArea=0.0;
	float fMaxLength=0.0;
	int maxRow=41;
	char cell[11][2]={{'A','A'},{'B','B'},{'C','D'},{'E','E'},{'F','F'},{'G','G'},{'H','H'},{'I','I'},{'J','K'},{'L','M'},{'N','O'}};
	int nCount = m_lstSurfaceResult.GetItemCount();
	for(int i=0; i<nCount; i++)
	{
		CString strOutput = m_lstSurfaceResult.GetItemText(i, 8);
		if (strOutput == _T("不导出"))
			continue;
		ccc++;
		FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);
		ccMianji+=node.m_fArea;

		if (node.m_fArea>fMaxArea)
		{
			fMaxArea=node.m_fArea;
		}

		float fTmpLength=sqrtf(pow(node.m_fMaxX-node.m_fMinX,2)+pow(node.m_fMaxY-node.m_fMinY,2));
		if (fTmpLength>fMaxLength)
		{
			fMaxLength=fTmpLength;
		}

		LOGICAL_POINT_t pt;
		pt.x = (node.m_fMinX + node.m_fMaxX) / 2 ;
		pt.y = (node.m_fMinY + node.m_fMaxY) / 2 ;

		CString strMinX;
		strMinX.Format(_T("%.2f"), pt.x);//node.m_fMinX);
		CString strMinY;
		strMinY.Format(_T("%.2f"), pt.y);//node.m_fMinY);
		//添加序号  yachang.huang add start on 2012-11-19
		CString strNum;
		strNum.Format(_T("%d"),maxRow-41);
		CString i_sn_num;
		i_sn_num.Format(_T("%s%d:%s%d"),(CString)cell[0][0],maxRow,(CString)cell[0][1],maxRow);
		pv.ExSetTxt(i_sn_num,strNum);
		//yachang.huang add end on 2012-11-19
		//“幅值”前面的几列都向右移一列，去掉原来的“A扫图”列  yachang.huang modify start on 2012-11-19
		CString i_sn;
		i_sn.Format(_T("%s%d:%s%d"),(CString)cell[1][0],maxRow,(CString)cell[1][1],maxRow);
		pv.ExSetTxt(i_sn,strMinX);
		CString i_sn2;
		i_sn2.Format(_T("%s%d:%s%d"),(CString)cell[2][0],maxRow,(CString)cell[2][1],maxRow);
		pv.ExSetTxt(i_sn2,strMinY);

		CString i_sn4;
		i_sn4.Format(_T("%s%d:%s%d"),(CString)cell[3][0],maxRow,(CString)cell[3][1],maxRow);
		pv.ExSetTxt(i_sn4,_T(""));
		for (int j=6; j<8; j++)
		{
			CString str = m_lstSurfaceResult.GetItemText(i, j - 1);

			if (7 == j)
			{
				CString strTemp = m_lstSurfaceResult.GetItemText(i, 9);
				float fTemp = _tstof(strTemp);
				float fTempDB = 20 * log10(fTemp/80);
				strTemp.Format(_T("%.2f"),fTempDB);
				strTemp += _T("dB");
				CString i_sn;
				i_sn.Format(_T("%s%d:%s%d"),(CString)cell[j-2][0],maxRow,(CString)cell[j-2][1],maxRow);
				pv.ExSetTxt(i_sn,strTemp);

			}
			else
			{
				CString i_sn;
				i_sn.Format(_T("%s%d:%s%d"),(CString)cell[j-2][0],maxRow,(CString)cell[j-2][1],maxRow);
				pv.ExSetTxt(i_sn,str);
			}
			

		}

		//yachang.huang modify end on 2012-11-19


		// modify by songchenguang 2012-07-01
		for (int j=9; j<14; j++)
		{
			CString str = m_lstSurfaceResult.GetItemText(i, j);

			CString i_sn;
			i_sn.Format(_T("%s%d:%s%d"),(CString)cell[j-3][0],maxRow,(CString)cell[j-3][1],maxRow);
			pv.ExSetTxt(i_sn,str);
		}

		maxRow++;
	}

	CString strQuexian;
	strQuexian.Format(_T("%d"),ccc);
	pv.ExSetTxt(_T("D14:J14"),strQuexian);

	CString strMianji;
	//wangbingfu modify start on 2015-07-08
	BOOL bUseHanHeData = FALSE;
	if(pCScanParam->m_nGateType == 0) // 高于
	{
		if(pCScanParam->m_nMinAMP == pCScanView->m_fGateThreshold[pCScanView->m_nGateIndex]
		&& pCScanParam->m_nMaxAMP == 100)
		{
			bUseHanHeData = TRUE;
		}
	}
	else
	{
		if(pCScanParam->m_nMaxAMP == pCScanView->m_fGateThreshold[pCScanView->m_nGateIndex]
		&& pCScanParam->m_nMinAMP == 0)
		{
			bUseHanHeData = TRUE;
		}
	}

	if(!bUseHanHeData)
	{
		strMianji.Format(_T("%f"),ccMianji);
	}
	else
	{
		strMianji.Format(_T("%f"), pCScanView->m_SelArea - pCScanView->m_dbFlawArea);
	}
	//wangbingfu modify end on 2015-07-08
	pv.ExSetTxt(_T("O14"),strMianji);

	CString strMaxMianji;
	strMaxMianji.Format(_T("%f"),fMaxArea);
	pv.ExSetTxt(_T("D16:J16"),strMaxMianji);

	CString strLength;
	strLength.Format(_T("%f"),fMaxLength);
	pv.ExSetTxt(_T("O15"),strLength);


	//增加原点偏移输出  yachang.huang add start on 2012-11-19
	CString strOffsetCoordinate;
	strOffsetCoordinate.Format(_T("(%.2f,%.2f)"), pCScanView->m_Offset_Coordinate.x, pCScanView->m_Offset_Coordinate.y);
	pv.ExSetTxt(_T("O16"),strOffsetCoordinate); 
	//yachang.huang add end on 2012-11-19

	maxRow++;

	pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();
	cc=0;
	for(int i=0; i<nCount; i++)
	{
		FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);
		CString strOutput = m_lstSurfaceResult.GetItemText(i, 8);

		if (strOutput == _T("不导出"))
		{
			continue;
		}

		//FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);

		if (node.m_strAscan == _T("保存") && node.m_boolHasAscan && node.m_strAscanPath!="")
		{
			CString strTempAscanPath;
			strTempAscanPath = node.m_strAscanPath;
			int num = std::count((LPCTSTR)strTempAscanPath,(LPCTSTR)strTempAscanPath+strTempAscanPath.GetLength(), _T('@'));//获取m_strAscanPath里面包含几个路径
			int nSinglePathLength = strTempAscanPath.GetLength()/num;
			for (int j = 0;j<num;j++)
			{
				CString strTempPath = strTempAscanPath.Mid(j*nSinglePathLength,nSinglePathLength);
				strTempAscanPath.TrimLeft(nSinglePathLength);
				strTempPath.TrimLeft(_T("@"));
				CString strAIndex;
				strAIndex.Format(_T("A扫图序号：%d-%d"), cc,j);
				CString i_sn;
				i_sn.Format(_T("A%d:D%d"),maxRow,maxRow);
				pv.ExSetTxt(i_sn,strAIndex);
				maxRow++;
				CString i_sn2;
				i_sn2.Format(_T("A%d:O%d"),maxRow,maxRow+18);
				pv.ExSetPic(i_sn2,strTempPath);
				maxRow += 20;
				continue;
			}
		}
		cc++;
	}

	if(pCScanView->m_nAxisSelect == 3
		|| pCScanView->m_nAxisSelect == 4)	
	{
		pv.ExSetTxt(_T("C40:D40"),_T("R角度"));	
	}

	else if (pCScanView->m_nAxisSelect == 5)
	{
		pv.ExSetTxt(_T("B40"),_T("W角度"));
		pv.ExSetTxt(_T("C40:D40"),_T("Z坐标"));
	}

	if (pCScanView->m_SelArea>0)
	{
		// wangbingfu modify start on 2015-07-08
		float fZhanJilv = 0;
		if(!bUseHanHeData)
		{
			fZhanJilv = ccMianji * 100 / pCScanView->m_SelArea;
		}
		else
		{
			fZhanJilv = 100 - pCScanView->m_dbFlawArea * 100 / pCScanView->m_SelArea;
		}
		// wangbingfu modify end on 2015-07-08
		CString strZhanJiLv;
		strZhanJiLv.Format(_T("%f"),fZhanJilv);
		pv.ExSetTxt(_T("D15:J15"),strZhanJiLv);
	}

	pv.ExShow();
	return;
	WPARAM wParam = MAKEWPARAM(ID_FILE_PRINT_PREVIEW, 0);
	m_pParentWnd->m_pCurCScanView->SendMessage(WM_COMMAND, wParam, 0);

}

// wangbingfu add start on 2014-08-27 北方重工报表格式20140630
void CSurfaceDetailDlg::TableCreateBeiFangZhongGong1()
{
/*	float fTestAmp = 0.0f; // 试块孔波高
	float fSurfaceComp = 0.0f; // 表面补偿
	ReadExcelValue(fTestAmp, fSurfaceComp);*/
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	setList();
	CString strTempPath;
	TCHAR szTempPath [100];
	DWORD dwSize=100;
	DWORD dwResult=GetTempPath(dwSize,szTempPath);//获取临时文件夹路径 

	CString timeStr;
	CTime day;
	day=CTime::GetCurrentTime();
	int filenameday,filenamemonth,filenameyear,filehour,filemin,filesec;
	filenameday=day.GetDay();//dd
	filenamemonth=day.GetMonth();//mm月份
	filenameyear=day.GetYear();//yyyy
	filehour=day.GetHour();//hh
	filemin=day.GetMinute();//mm分钟
	filesec=day.GetSecond();//ss
	timeStr.Format(_T("%s%04d%02d%02d%02d%02d%02d"),szTempPath,filenameyear,filenamemonth,filenameday,filehour,filemin,filesec);
	CString strSCANDate;
	strSCANDate.Format(_T("%04d-%02d-%02d"),filenameyear,filenamemonth,filenameday);
	strTempPath=timeStr+_T(".c.bmp");


	BOOL bGetCPic=m_pParentWnd->SavePicByPath(strTempPath,true);


	CPrintView pv;
	CLSID clsid;
	if(CLSIDFromString(CComBSTR("ActiveXExtension.PrintView"), &clsid)==S_OK)
	{
		pv.CreateDispatch(clsid,NULL);
	}
	CMechanicalControlPanel* pMechanicalCtrlPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;



	;
	CProbeSet probeset1;
	if(CLSIDFromString(CComBSTR("ActiveXExtension.ProbeSet"), &clsid)==S_OK)
	{
		probeset1.CreateDispatch(clsid,NULL);
	}
	probeset1.SetDir(pMechanicalCtrlPanel->m_strDataFolder);



	pv.ExLoad(); 

	if(bGetCPic)
	{
		pv.ExSetPic(_T("A18:K18"),strTempPath);  
	}

	CString strList[6] = {pFrame->m_ProbeSet.getVal(_T("uName")),
		pFrame->m_ProbeSet.getVal(_T("uClass")),
		pFrame->m_ProbeSet.getVal(_T("oMaterial")),
		pFrame->m_ProbeSet.getVal(_T("oType")),
		pFrame->m_ProbeSet.getVal(_T("oId")),
		pFrame->m_ProbeSet.getVal(_T("oSize"))};
/*
	//machao modify start on 2013-07-30 //?
	pv.ExSetTxt(_T("O11"),strList[0]);
	//machao modify end on 2013-07-30
	pv.ExSetTxt(_T("D11:J11"),strList[1]);
	pv.ExSetTxt(_T("D4:J4"),strList[2]);
	pv.ExSetTxt(_T("D9:J9"),strList[3]);
	pv.ExSetTxt(_T("O3"),strList[4]);
	pv.ExSetTxt(_T("O4"),strList[5]);
	pv.ExSetTxt(_T("D10:J10"),strSCANDate);

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CString strMachine;
	strMachine.Format(_T("%s"), pApp->m_IniParams.m_info.wcMechineType);
	pv.ExSetTxt(_T("D7:J7"),strMachine);
	CString strUserCompany;
	strUserCompany.Format(_T("%s"), pApp->m_IniParams.m_info.wcUserCompany);
	pv.ExSetTxt(_T("O8"),strUserCompany);
*/
	//{_T("序号:"), _T("MinX:"),_T("MinY"),_T("MaxX:"),_T("MaxY:"),_T("深度:"),_T("面积:"),
	//	_T("位置危险性"),_T("分析结果"),_T("处理意见"),_T("备注"),_T("A扫设置"),_T("导出设置")};
	CCScanParamPanel* pCScanParam = m_pParentWnd;
	CString cstrVal;
	cstrVal.Format(_T("%.3f"), pCScanParam->m_fTestAmp / 100);
	pv.ExSetTxt(_T("J7:K7"),cstrVal);
	cstrVal.Format(_T("%.0f"), pCScanParam->m_fSurComp);
	pv.ExSetTxt(_T("J10:K10"),cstrVal);
	POSITION pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();
	int cc=1;
	int ccc=0;
	float ccMianji=0.0;
	float fMaxArea=0.0;
	float fMaxLength=0.0;
	int maxRow=21;
	int nStartRow = 21;	// 开始行号
	char cell[11][2]={{'A','A'},{'B','B'},{'C','C'},{'D','D'},{'E','E'},{'F','F'},{'G','G'},
						{'H','H'},{'I','I'},{'J','J'},{'K','K'}};
	// wangbingfu add on 2014-11-03 先写空值 使得表格可写入
	pv.ExSetTxt(_T("C2:F2"),_T(""));
	for(int i = 3; i < 13; i++)
	{
		CString cstrPostion;
		cstrPostion.Format(_T("C%d:D%d"),i, i);
		pv.ExSetTxt(cstrPostion,_T(""));
		cstrPostion.Format(_T("G%d:H%d"),i, i);
		pv.ExSetTxt(cstrPostion,_T(""));
		cstrPostion.Format(_T("J%d:K%d"),i, i);
		pv.ExSetTxt(cstrPostion,_T(""));
	}
	// wangbingfu add end on 2014-11-03 
	
	int nCount = m_lstSurfaceResult.GetItemCount();
	for(int i=0; i<nCount; i++)
	{
		CString strOutput = m_lstSurfaceResult.GetItemText(i, 8);
		if (strOutput == _T("不导出"))
			continue;

		FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);
	/*	ccc++;
		ccMianji+=node.m_fArea;

		if (node.m_fArea>fMaxArea)
		{
			fMaxArea=node.m_fArea;
		}

		float fTmpLength=sqrtf(pow(node.m_fMaxX-node.m_fMinX,2)+pow(node.m_fMaxY-node.m_fMinY,2));
		if (fTmpLength>fMaxLength)
		{
			fMaxLength=fTmpLength;
		}
		*/
	/*	CString strMinX;
		strMinX.Format(_T("%.2f"), node.m_fMinX);
		CString strMinY;
		strMinY.Format(_T("%.2f"), node.m_fMinY);
		CString strMaxX;
		strMinX.Format(_T("%.2f"), node.m_fMaxX);
		CString strMaxY;
		strMinY.Format(_T("%.2f"), node.m_fMaxY);
		//添加序号  yachang.huang add start on 2012-11-19
		CString strNum;
		strNum.Format(_T("%d"), (maxRow - nStartRow));
		CString i_sn_num;
		i_sn_num.Format(_T("%s%d:%s%d"),(CString)cell[0][0],maxRow,(CString)cell[0][1],maxRow);
		pv.ExSetTxt(i_sn_num,strNum);
		//yachang.huang add end on 2012-11-19
		//“幅值”前面的几列都向右移一列，去掉原来的“A扫图”列  yachang.huang modify start on 2012-11-19
		CString cstrPostion;
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[1][0],maxRow,(CString)cell[1][1],maxRow);
		pv.ExSetTxt(cstrPostion,strMinX);
		
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[2][0],maxRow,(CString)cell[2][1],maxRow);
		pv.ExSetTxt(cstrPostion,strMinY);

		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[3][0],maxRow,(CString)cell[3][1],maxRow);
		pv.ExSetTxt(cstrPostion,strMaxX);
	
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[4][0],maxRow,(CString)cell[4][1],maxRow);
		pv.ExSetTxt(cstrPostion,strMaxY);
	*/
		for(int j = 0; j < 6; j++)	// 序号，minX, maxX, minY, maxY, 高度
		{
			CString cstrVal = m_lstSurfaceResult.GetItemText(i, j);
			CString cstrPostion;
			cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[j][0],maxRow,(CString)cell[j][1],										maxRow);
			pv.ExSetTxt(cstrPostion,cstrVal);
		}
		
		// 最大幅值
		CString cstrVal = m_lstSurfaceResult.GetItemText(i, 9);	
		CString cstrPostion;
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[7][0],maxRow,(CString)cell[7][1],maxRow);
		pv.ExSetTxt(cstrPostion,cstrVal);
		
		// 缺陷大小
		float fAmp = _tstof(cstrVal);
		float fTempDB = 0.0f;
		float fAmpabs = fabs(fAmp);
		float fTestAmpabs = fabs(pCScanParam->m_fTestAmp);
		if(fAmpabs > 1E-6 && fTestAmpabs > 1E-6)
		{
			fTempDB = 20 * log10(fAmpabs / fTestAmpabs ) - pCScanParam->m_fSurComp;
		}
			
		cstrVal.Format(_T("%.2f"),fTempDB);
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[6][0],maxRow,(CString)cell[6][1],maxRow);
		pv.ExSetTxt(cstrPostion,cstrVal);

		// 缺陷面积
		cstrVal = m_lstSurfaceResult.GetItemText(i, 6);
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[8][0],maxRow,(CString)cell[8][1],maxRow);
		pv.ExSetTxt(cstrPostion,cstrVal);
		
		// 缺陷长度
		float fLength = sqrtf(pow(node.m_fMaxX-node.m_fMinX,2) + pow(node.m_fMaxY-node.m_fMinY,2));
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[9][0],maxRow,(CString)cell[9][1],maxRow);
		cstrVal.Format(_T("%.1f"), fLength);
		pv.ExSetTxt(cstrPostion,cstrVal);

		// 备注
		cstrPostion.Format(_T("K%d"), maxRow);
		pv.ExSetTxt(cstrPostion, _T(""));

		maxRow++;
	}

	maxRow++;

	pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();
	cc=0;
	for(int i=0; i<nCount; i++)
	{
		FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);
		CString strOutput = m_lstSurfaceResult.GetItemText(i, 8);

		if (strOutput == _T("不导出"))
		{
			continue;
		}

		//FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);

		if (node.m_strAscan == _T("保存") && node.m_boolHasAscan && node.m_strAscanPath!="")
		{
			CString strTempAscanPath;
			strTempAscanPath = node.m_strAscanPath;
			int num = std::count((LPCTSTR)strTempAscanPath,(LPCTSTR)strTempAscanPath+strTempAscanPath.GetLength(), _T('@'));//获取m_strAscanPath里面包含几个路径
			int nSinglePathLength = strTempAscanPath.GetLength()/num;
			for (int j = 0;j<num;j++)
			{
				CString strTempPath = strTempAscanPath.Mid(j*nSinglePathLength,nSinglePathLength);
				strTempAscanPath.TrimLeft(nSinglePathLength);
				strTempPath.TrimLeft(_T("@"));
				CString strAIndex;
				strAIndex.Format(_T("A扫图序号：%d-%d"), cc,j);
				CString i_sn;
				i_sn.Format(_T("A%d:D%d"),maxRow,maxRow);
				pv.ExSetTxt(i_sn,strAIndex);
				maxRow++;
				CString i_sn2;
				i_sn2.Format(_T("A%d:K%d"),maxRow,maxRow+18);
				pv.ExSetPic(i_sn2,strTempPath);
				maxRow += 20;
				continue;
			}
		}
		cc++;
	}
	
	maxRow++;
	
	CString cstrPostion;
	cstrPostion.Format(_T("A%d:D%d"), maxRow, maxRow+1);
	pv.ExSetTxt(cstrPostion, _T("检测："));
	cstrPostion.Format(_T("E%d:H%d"), maxRow, maxRow+1);
	pv.ExSetTxt(cstrPostion, _T("审核："));
	cstrPostion.Format(_T("I%d:K%d"), maxRow, maxRow+1);
	pv.ExSetTxt(cstrPostion, _T("批准："));
	
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

	if(pCScanView->m_nAxisSelect == 3
		|| pCScanView->m_nAxisSelect == 4)	
	{
		pv.ExSetTxt(_T("B20"),_T("minR"));
		pv.ExSetTxt(_T("D20"),_T("maxR"));
	}

	else if (pCScanView->m_nAxisSelect == 5)
	{
		pv.ExSetTxt(_T("B20"),_T("minW"));
		pv.ExSetTxt(_T("D20"),_T("maxW"));
		pv.ExSetTxt(_T("C20"),_T("minZ"));
		pv.ExSetTxt(_T("E20"),_T("maxZ"));
	}

	pv.ExShow();
	return;
	WPARAM wParam = MAKEWPARAM(ID_FILE_PRINT_PREVIEW, 0);
	m_pParentWnd->m_pCurCScanView->SendMessage(WM_COMMAND, wParam, 0);

}

bool CSurfaceDetailDlg::ReadExcelValue(float &fAmp,					// 试块孔波高
					float &fSurfaceComp)			// 表面补偿
{
	CString cstrPath = _T("\\printModel.xls");
	//	GetCurrentDirectory( _MAX_PATH, cstrPath.GetBuffer());   //获取当前路径
	GetModuleFileName(NULL, cstrPath.GetBuffer(_MAX_PATH), _MAX_PATH);
	cstrPath.ReleaseBuffer();
	int nLength = cstrPath.GetLength();
	cstrPath = cstrPath.Mid(0, nLength-26);
	cstrPath = cstrPath + _T("\\printModel.xls");
	COleVariant VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	CString sValue = _T("");
	_Application objApp;
	Workbooks objBooks;
	_Workbook objBook;
	Worksheets objSheets;
	_Worksheet objSheet;
	Range objRange;
	VARIANT vRet;
	//_T("../mdsocx/printModel.xls")
	objApp.CreateDispatch(_T("Excel.Application"));
	objBooks = objApp.GetWorkbooks(); 
	cstrPath = _T("c:\\Program Files\\奥瑞视（北京）科技有限公司\\三维水浸探伤系统\\mdsocx\\printModel.xls");
	objBook = objBooks.Open(cstrPath,  VOptional, VOptional, VOptional, VOptional,  VOptional,			VOptional, VOptional, VOptional,  VOptional, VOptional, VOptional, VOptional,  VOptional,			VOptional);
	objSheets = objBook.GetWorksheets();
	objSheet = objSheets.GetItem(COleVariant((short)1));
	objRange = objSheet.GetUsedRange();
	// 取行列数
	vRet = objRange.GetValue(VOptional);

	COleSafeArray oleSA(vRet); 
	long lNumRows; 
	long lNumCols;
	oleSA.GetUBound(1, &lNumRows);
	oleSA.GetUBound(2, &lNumCols); 

	if (lNumRows <= 1)
	{
		//AfxMessageBox("没有数据！");
		return false;
	}
	long index[2]; 
	index[0] = 7;    
	index[1] = 10;
	VARIANT vValue;
	oleSA.GetElement(index, &vValue); 
	fAmp = vValue.fltVal;
	index[0] = 10;    
	index[1] = 10;
	oleSA.GetElement(index, &vValue); 
	fSurfaceComp = vValue.fltVal;


	objRange.Clear();
	objBook.Close(COleVariant((short)FALSE),VOptional,VOptional);
	objBooks.Close();
	objApp.Quit();

	return true;
}
// wangbingfu add end on 2014-08-28 北方重工报表格式20140630
void CSurfaceDetailDlg::TableCreateHaFei()
{

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	setList();
	CString strTempPath;
	TCHAR szTempPath [100];
	DWORD dwSize=100;
	DWORD dwResult=GetTempPath(dwSize,szTempPath);//获取临时文件夹路径 

	CString timeStr;
	CTime day;
	day=CTime::GetCurrentTime();
	int filenameday,filenamemonth,filenameyear,filehour,filemin,filesec;
	filenameday=day.GetDay();//dd
	filenamemonth=day.GetMonth();//mm月份
	filenameyear=day.GetYear();//yyyy
	filehour=day.GetHour();//hh
	filemin=day.GetMinute();//mm分钟
	filesec=day.GetSecond();//ss
	timeStr.Format(_T("%s%04d%02d%02d%02d%02d%02d"),szTempPath,filenameyear,filenamemonth,filenameday,filehour,filemin,filesec);
	CString strSCANDate;
	strSCANDate.Format(_T("%04d-%02d-%02d"),filenameyear,filenamemonth,filenameday);
	strTempPath=timeStr+_T(".c.bmp");


	BOOL bGetCPic=m_pParentWnd->SavePicByPath(strTempPath,true);


	CPrintView pv;
	CLSID clsid;
	if(CLSIDFromString(CComBSTR("ActiveXExtension.PrintView"), &clsid)==S_OK)
	{
		pv.CreateDispatch(clsid,NULL);
	}
	CMechanicalControlPanel* pMechanicalCtrlPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;



	;
	CProbeSet probeset1;
	if(CLSIDFromString(CComBSTR("ActiveXExtension.ProbeSet"), &clsid)==S_OK)
	{
		probeset1.CreateDispatch(clsid,NULL);
	}
	probeset1.SetDir(pMechanicalCtrlPanel->m_strDataFolder);

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

	pv.ExLoad(); 

	if(bGetCPic)
	{
		pv.ExSetPic(_T("A18:O37"),strTempPath);  
	}

	CString strList[6] = {pFrame->m_ProbeSet.getVal(_T("uName")),
		pFrame->m_ProbeSet.getVal(_T("uClass")),
		pFrame->m_ProbeSet.getVal(_T("oMaterial")),
		pFrame->m_ProbeSet.getVal(_T("oType")),
		pFrame->m_ProbeSet.getVal(_T("oId")),
		pFrame->m_ProbeSet.getVal(_T("oSize"))};
/*
	//machao modify start on 2013-07-30 //?
	pv.ExSetTxt(_T("O11"),strList[0]);
	//machao modify end on 2013-07-30
	pv.ExSetTxt(_T("D11:J11"),strList[1]);
	pv.ExSetTxt(_T("D4:J4"),strList[2]);
	pv.ExSetTxt(_T("D9:J9"),strList[3]);
	pv.ExSetTxt(_T("O3"),strList[4]);
	pv.ExSetTxt(_T("O4"),strList[5]);
	pv.ExSetTxt(_T("D10:J10"),strSCANDate);

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CString strMachine;
	strMachine.Format(_T("%s"), pApp->m_IniParams.m_info.wcMechineType);
	pv.ExSetTxt(_T("D7:J7"),strMachine);
	CString strUserCompany;
	strUserCompany.Format(_T("%s"), pApp->m_IniParams.m_info.wcUserCompany);
	pv.ExSetTxt(_T("O8"),strUserCompany);
*/
	//{_T("序号:"), _T("MinX:"),_T("MinY"),_T("MaxX:"),_T("MaxY:"),_T("深度:"),_T("面积:"),
	//	_T("位置危险性"),_T("分析结果"),_T("处理意见"),_T("备注"),_T("A扫设置"),_T("导出设置")};
	POSITION pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();
	int cc=1;
	int ccc=0;
	float ccMianji=0.0;
	float fMaxArea=0.0;
	float fMaxLength=0.0;
	int maxRow=41;
	char cell[11][2]={{'A','A'},{'B','B'},{'C','D'},{'E','E'},{'F','F'},{'G','G'},{'H','H'},{'I','I'},{'J','K'},{'L','M'},{'N','O'}};
	int nCount = m_lstSurfaceResult.GetItemCount();
	for(int i=0; i<nCount; i++)
	{
		CString strOutput = m_lstSurfaceResult.GetItemText(i, 8);
		if (strOutput == _T("不导出"))
			continue;
		ccc++;
		FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);
		ccMianji+=node.m_fArea;

		if (node.m_fArea>fMaxArea)
		{
			fMaxArea=node.m_fArea;
		}

		float fTmpLength=sqrtf(pow(node.m_fMaxX-node.m_fMinX,2)+pow(node.m_fMaxY-node.m_fMinY,2));
		if (fTmpLength>fMaxLength)
		{
			fMaxLength=fTmpLength;
		}

		LOGICAL_POINT_t pt;
		pt.x = (node.m_fMinX + node.m_fMaxX) / 2 ;
		pt.y = (node.m_fMinY + node.m_fMaxY) / 2 ;

		CString strMinX;
		strMinX.Format(_T("%.2f"), pt.x);//node.m_fMinX);
		CString strMinY;
		strMinY.Format(_T("%.2f"), pt.y);//node.m_fMinY);
		//添加序号  yachang.huang add start on 2012-11-19
		CString strNum;
		strNum.Format(_T("%d"),maxRow-41);
		CString i_sn_num;
		i_sn_num.Format(_T("%s%d:%s%d"),(CString)cell[0][0],maxRow,(CString)cell[0][1],maxRow);
		pv.ExSetTxt(i_sn_num,strNum);
		//yachang.huang add end on 2012-11-19
		//“幅值”前面的几列都向右移一列，去掉原来的“A扫图”列  yachang.huang modify start on 2012-11-19
		CString i_sn;
		i_sn.Format(_T("%s%d:%s%d"),(CString)cell[1][0],maxRow,(CString)cell[1][1],maxRow);
		pv.ExSetTxt(i_sn,strMinX);
		CString i_sn2;
		i_sn2.Format(_T("%s%d:%s%d"),(CString)cell[2][0],maxRow,(CString)cell[2][1],maxRow);
		pv.ExSetTxt(i_sn2,strMinY);

		CString i_sn4;
		i_sn4.Format(_T("%s%d:%s%d"),(CString)cell[3][0],maxRow,(CString)cell[3][1],maxRow);
		pv.ExSetTxt(i_sn4,_T(""));
		for (int j=6; j<8; j++)
		{
			CString str = m_lstSurfaceResult.GetItemText(i, j - 1);


			CString i_sn;
			i_sn.Format(_T("%s%d:%s%d"),(CString)cell[j-2][0],maxRow,(CString)cell[j-2][1],maxRow);
			pv.ExSetTxt(i_sn,str);



		}

		//yachang.huang modify end on 2012-11-19


		// modify by songchenguang 2012-07-01
		for (int j=9; j<14; j++)
		{
			CString str = m_lstSurfaceResult.GetItemText(i, j);

			CString i_sn;
			i_sn.Format(_T("%s%d:%s%d"),(CString)cell[j-3][0],maxRow,(CString)cell[j-3][1],maxRow);
			pv.ExSetTxt(i_sn,str);
		}

		maxRow++;
	}

	CString strQuexian;
	strQuexian.Format(_T("%d"),ccc);
	pv.ExSetTxt(_T("D14:J14"),strQuexian);

	CString strMianji;
	//wangbingfu modify start on 2015-07-08
	BOOL bUseHanHeData = FALSE;
	if(pCScanParam->m_nGateType == 0) // 高于
	{
		if(pCScanParam->m_nMinAMP == pCScanView->m_fGateThreshold[pCScanView->m_nGateIndex]
		&& pCScanParam->m_nMaxAMP == 100)
		{
			bUseHanHeData = TRUE;
		}
	}
	else
	{
		if(pCScanParam->m_nMaxAMP == pCScanView->m_fGateThreshold[pCScanView->m_nGateIndex]
		&& pCScanParam->m_nMinAMP == 0)
		{
			bUseHanHeData = TRUE;
		}
	}

	if(!bUseHanHeData)
	{
		strMianji.Format(_T("%f"),ccMianji);
	}
	else
	{
		strMianji.Format(_T("%f"), pCScanView->m_SelArea - pCScanView->m_dbFlawArea);
	}
	//wangbingfu modify end on 2015-07-08
	pv.ExSetTxt(_T("O14"),strMianji);

	CString strMaxMianji;
	strMaxMianji.Format(_T("%f"),fMaxArea);
	pv.ExSetTxt(_T("D16:J16"),strMaxMianji);

	CString strLength;
	strLength.Format(_T("%f"),fMaxLength);
	pv.ExSetTxt(_T("O15"),strLength);


	//增加原点偏移输出  yachang.huang add start on 2012-11-19
	CString strOffsetCoordinate;
	strOffsetCoordinate.Format(_T("(%.2f,%.2f)"), pCScanView->m_Offset_Coordinate.x, pCScanView->m_Offset_Coordinate.y);
	pv.ExSetTxt(_T("O16"),strOffsetCoordinate); 
	//yachang.huang add end on 2012-11-19

	maxRow++;

	pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();
	cc=0;
	for(int i=0; i<nCount; i++)
	{
		FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);
		CString strOutput = m_lstSurfaceResult.GetItemText(i, 8);

		if (strOutput == _T("不导出"))
		{
			continue;
		}

		//FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);

		if (node.m_strAscan == _T("保存") && node.m_boolHasAscan && node.m_strAscanPath!="")
		{
			CString strTempAscanPath;
			strTempAscanPath = node.m_strAscanPath;
			int num = std::count((LPCTSTR)strTempAscanPath,(LPCTSTR)strTempAscanPath+strTempAscanPath.GetLength(), _T('@'));//获取m_strAscanPath里面包含几个路径
			int nSinglePathLength = strTempAscanPath.GetLength()/num;
			for (int j = 0;j<num;j++)
			{
				CString strTempPath = strTempAscanPath.Mid(j*nSinglePathLength,nSinglePathLength);
				strTempAscanPath.TrimLeft(nSinglePathLength);
				strTempPath.TrimLeft(_T("@"));
				CString strAIndex;
				strAIndex.Format(_T("A扫图序号：%d-%d"), cc,j);
				CString i_sn;
				i_sn.Format(_T("A%d:D%d"),maxRow,maxRow);
				pv.ExSetTxt(i_sn,strAIndex);
				maxRow++;
				CString i_sn2;
				i_sn2.Format(_T("A%d:O%d"),maxRow,maxRow+18);
				pv.ExSetPic(i_sn2,strTempPath);
				maxRow += 20;
				continue;
			}
		}
		cc++;
	}

	if(pCScanView->m_nAxisSelect == 3
		|| pCScanView->m_nAxisSelect == 4)	
	{
		pv.ExSetTxt(_T("C40:D40"),_T("R角度"));	
	}

	else if (pCScanView->m_nAxisSelect == 5)
	{
		pv.ExSetTxt(_T("B40"),_T("W角度"));
		pv.ExSetTxt(_T("C40:D40"),_T("Z坐标"));
	}

	if (pCScanView->m_SelArea>0)
	{
		// wangbingfu modify start on 2015-07-08
		float fZhanJilv = 0;
		if(!bUseHanHeData)
		{
			fZhanJilv = ccMianji * 100 / pCScanView->m_SelArea;
		}
		else
		{
			fZhanJilv = 100 - pCScanView->m_dbFlawArea * 100 / pCScanView->m_SelArea;
		}
		// wangbingfu modify end on 2015-07-08
		CString strZhanJiLv;
		strZhanJiLv.Format(_T("%f"),fZhanJilv);
		pv.ExSetTxt(_T("D15:J15"),strZhanJiLv);
	}

	pv.ExShow();
	return;
	WPARAM wParam = MAKEWPARAM(ID_FILE_PRINT_PREVIEW, 0);
	m_pParentWnd->m_pCurCScanView->SendMessage(WM_COMMAND, wParam, 0);

}

// wangbingfu add start on 2014-11-03 哈飞报表格式
void CSurfaceDetailDlg::TableCreateHaFei1()
{

	/*	float fTestAmp = 0.0f; // 试块孔波高
	float fSurfaceComp = 0.0f; // 表面补偿
	ReadExcelValue(fTestAmp, fSurfaceComp);*/
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	setList();
	CString strTempPath;
	TCHAR szTempPath [100];
	DWORD dwSize=100;
	DWORD dwResult=GetTempPath(dwSize,szTempPath);//获取临时文件夹路径 

	CString timeStr;
	CTime day;
	day=CTime::GetCurrentTime();
	int filenameday,filenamemonth,filenameyear,filehour,filemin,filesec;
	filenameday=day.GetDay();//dd
	filenamemonth=day.GetMonth();//mm月份
	filenameyear=day.GetYear();//yyyy
	filehour=day.GetHour();//hh
	filemin=day.GetMinute();//mm分钟
	filesec=day.GetSecond();//ss
	timeStr.Format(_T("%s%04d%02d%02d%02d%02d%02d"),szTempPath,filenameyear,filenamemonth,filenameday,filehour,filemin,filesec);
	CString strSCANDate;
	strSCANDate.Format(_T("%04d-%02d-%02d"),filenameyear,filenamemonth,filenameday);
	strTempPath=timeStr+_T(".c.bmp");


	BOOL bGetCPic=m_pParentWnd->SavePicByPath(strTempPath,true);


	CPrintView pv;
	CLSID clsid;
	if(CLSIDFromString(CComBSTR("ActiveXExtension.PrintView"), &clsid)==S_OK)
	{
		pv.CreateDispatch(clsid,NULL);
	}
	CMechanicalControlPanel* pMechanicalCtrlPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;



	;
	CProbeSet probeset1;
	if(CLSIDFromString(CComBSTR("ActiveXExtension.ProbeSet"), &clsid)==S_OK)
	{
		probeset1.CreateDispatch(clsid,NULL);
	}
	probeset1.SetDir(pMechanicalCtrlPanel->m_strDataFolder);



	pv.ExLoad(); 

	if(bGetCPic)
	{
		pv.ExSetPic(_T("A12:K12"),strTempPath);  
	}

	CString strList[6] = {pFrame->m_ProbeSet.getVal(_T("uName")),
		pFrame->m_ProbeSet.getVal(_T("uClass")),
		pFrame->m_ProbeSet.getVal(_T("oMaterial")),
		pFrame->m_ProbeSet.getVal(_T("oType")),
		pFrame->m_ProbeSet.getVal(_T("oId")),
		pFrame->m_ProbeSet.getVal(_T("oSize"))};
/*
	//machao modify start on 2013-07-30 //?
	pv.ExSetTxt(_T("O11"),strList[0]);
	//machao modify end on 2013-07-30
	pv.ExSetTxt(_T("D11:J11"),strList[1]);
	pv.ExSetTxt(_T("D4:J4"),strList[2]);
	pv.ExSetTxt(_T("D9:J9"),strList[3]);
	pv.ExSetTxt(_T("O3"),strList[4]);
	pv.ExSetTxt(_T("O4"),strList[5]);
	pv.ExSetTxt(_T("D10:J10"),strSCANDate);

	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CString strMachine;
	strMachine.Format(_T("%s"), pApp->m_IniParams.m_info.wcMechineType);
	pv.ExSetTxt(_T("D7:J7"),strMachine);
	CString strUserCompany;
	strUserCompany.Format(_T("%s"), pApp->m_IniParams.m_info.wcUserCompany);
	pv.ExSetTxt(_T("O8"),strUserCompany);
*/
	//{_T("序号:"), _T("MinX:"),_T("MinY"),_T("MaxX:"),_T("MaxY:"),_T("深度:"),_T("面积:"),
	//	_T("位置危险性"),_T("分析结果"),_T("处理意见"),_T("备注"),_T("A扫设置"),_T("导出设置")};
	CCScanParamPanel* pCScanParam = m_pParentWnd;
// 	CString cstrVal;
// 	cstrVal.Format(_T("%.3f"), pCScanParam->m_fTestAmp / 100);
// 	pv.ExSetTxt(_T("J7:K7"),cstrVal);
// 	cstrVal.Format(_T("%.0f"), pCScanParam->m_fSurComp);
// 	pv.ExSetTxt(_T("J10:K10"),cstrVal);
	POSITION pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();
	int cc=1;
	int ccc=0;
	float ccMianji=0.0;
	float fMaxArea=0.0;
	float fMaxLength=0.0;
	int maxRow=15;
	int nStartRow = 15;	// 开始行号
	char cell[11][2]={{'A','A'},{'B','B'},{'C','C'},{'D','D'},{'E','E'},{'F','F'},{'G','G'},
						{'H','H'},{'I','I'},{'J','J'},{'K','K'}};

	for(int i = 2; i < 5; i++)
	{
		CString cstrPostion;
		cstrPostion.Format(_T("C%d:D%d"),i, i);
		pv.ExSetTxt(cstrPostion,_T(""));
		cstrPostion.Format(_T("G%d:H%d"),i, i);
		pv.ExSetTxt(cstrPostion,_T(""));
		cstrPostion.Format(_T("J%d:K%d"),i, i);
		pv.ExSetTxt(cstrPostion,_T(""));
	}
	pv.ExSetTxt(_T("C5:K5"),_T(""));

	int nCount = m_lstSurfaceResult.GetItemCount();
	for(int i=0; i<nCount; i++)
	{
		CString strOutput = m_lstSurfaceResult.GetItemText(i, 8);
		if (strOutput == _T("不导出"))
			continue;

		FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);
	/*	ccc++;
		ccMianji+=node.m_fArea;

		if (node.m_fArea>fMaxArea)
		{
			fMaxArea=node.m_fArea;
		}

		float fTmpLength=sqrtf(pow(node.m_fMaxX-node.m_fMinX,2)+pow(node.m_fMaxY-node.m_fMinY,2));
		if (fTmpLength>fMaxLength)
		{
			fMaxLength=fTmpLength;
		}
		*/
	/*	CString strMinX;
		strMinX.Format(_T("%.2f"), node.m_fMinX);
		CString strMinY;
		strMinY.Format(_T("%.2f"), node.m_fMinY);
		CString strMaxX;
		strMinX.Format(_T("%.2f"), node.m_fMaxX);
		CString strMaxY;
		strMinY.Format(_T("%.2f"), node.m_fMaxY);
		//添加序号  yachang.huang add start on 2012-11-19
		CString strNum;
		strNum.Format(_T("%d"), (maxRow - nStartRow));
		CString i_sn_num;
		i_sn_num.Format(_T("%s%d:%s%d"),(CString)cell[0][0],maxRow,(CString)cell[0][1],maxRow);
		pv.ExSetTxt(i_sn_num,strNum);
		//yachang.huang add end on 2012-11-19
		//“幅值”前面的几列都向右移一列，去掉原来的“A扫图”列  yachang.huang modify start on 2012-11-19
		CString cstrPostion;
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[1][0],maxRow,(CString)cell[1][1],maxRow);
		pv.ExSetTxt(cstrPostion,strMinX);
		
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[2][0],maxRow,(CString)cell[2][1],maxRow);
		pv.ExSetTxt(cstrPostion,strMinY);

		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[3][0],maxRow,(CString)cell[3][1],maxRow);
		pv.ExSetTxt(cstrPostion,strMaxX);
	
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[4][0],maxRow,(CString)cell[4][1],maxRow);
		pv.ExSetTxt(cstrPostion,strMaxY);
	*/
		for(int j = 0; j < 6; j++)	// 序号，minX, maxX, minY, maxY, 高度
		{
			CString cstrVal = m_lstSurfaceResult.GetItemText(i, j);
			CString cstrPostion;
			cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[j][0],maxRow,(CString)cell[j][1],										maxRow);
			pv.ExSetTxt(cstrPostion,cstrVal);
		}
		
		// 最大幅值
		CString cstrVal = m_lstSurfaceResult.GetItemText(i, 9);	
		CString cstrPostion;
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[7][0],maxRow,(CString)cell[7][1],maxRow);
		pv.ExSetTxt(cstrPostion,cstrVal);
		
		// 缺陷大小
		float fAmp = _tstof(cstrVal);
		float fTempDB = 0.0f;
		float fAmpabs = fabs(fAmp);
		float fTestAmpabs = fabs(pCScanParam->m_fTestAmp);
		if(fAmpabs > 1E-6 && fTestAmpabs > 1E-6)
		{
			fTempDB = 20 * log10(fAmpabs / fTestAmpabs ) - pCScanParam->m_fSurComp;
		}
			
		cstrVal.Format(_T("%.2f"),fTempDB);
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[6][0],maxRow,(CString)cell[6][1],maxRow);
		pv.ExSetTxt(cstrPostion,cstrVal);

		// 缺陷面积
		cstrVal = m_lstSurfaceResult.GetItemText(i, 6);
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[8][0],maxRow,(CString)cell[8][1],maxRow);
		pv.ExSetTxt(cstrPostion,cstrVal);
		
		// 缺陷长度
		float fLength = sqrtf(pow(node.m_fMaxX-node.m_fMinX,2) + pow(node.m_fMaxY-node.m_fMinY,2));
		cstrPostion.Format(_T("%s%d:%s%d"),(CString)cell[9][0],maxRow,(CString)cell[9][1],maxRow);
		cstrVal.Format(_T("%.1f"), fLength);
		pv.ExSetTxt(cstrPostion,cstrVal);

		// 备注
		cstrPostion.Format(_T("K%d"), maxRow);
		pv.ExSetTxt(cstrPostion, _T(""));

		maxRow++;
	}

	maxRow++;

	pos = m_pParentWnd->m_lstSurfaceDetail.GetHeadPosition();
	cc=0;
	for(int i=0; i<nCount; i++)
	{
		FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);
		CString strOutput = m_lstSurfaceResult.GetItemText(i, 8);

		if (strOutput == _T("不导出"))
		{
			continue;
		}

		//FlawNode node = m_pParentWnd->m_lstSurfaceDetail.GetNext(pos);

		if (node.m_strAscan == _T("保存") && node.m_boolHasAscan && node.m_strAscanPath!="")
		{
			CString strTempAscanPath;
			strTempAscanPath = node.m_strAscanPath;
			int num = std::count((LPCTSTR)strTempAscanPath,(LPCTSTR)strTempAscanPath+strTempAscanPath.GetLength(), _T('@'));//获取m_strAscanPath里面包含几个路径
			int nSinglePathLength = strTempAscanPath.GetLength()/num;
			for (int j = 0;j<num;j++)
			{
				CString strTempPath = strTempAscanPath.Mid(j*nSinglePathLength,nSinglePathLength);
				strTempAscanPath.TrimLeft(nSinglePathLength);
				strTempPath.TrimLeft(_T("@"));
				CString strAIndex;
				strAIndex.Format(_T("A扫图序号：%d-%d"), cc,j);
				CString i_sn;
				i_sn.Format(_T("A%d:D%d"),maxRow,maxRow);
				pv.ExSetTxt(i_sn,strAIndex);
				maxRow++;
				CString i_sn2;
				i_sn2.Format(_T("A%d:K%d"),maxRow,maxRow+18);
				pv.ExSetPic(i_sn2,strTempPath);
				maxRow += 20;
				continue;
			}
		}
		cc++;
	}
	
	maxRow++;
	
	CString cstrPostion;
	cstrPostion.Format(_T("A%d:D%d"), maxRow, maxRow+1);
	pv.ExSetTxt(cstrPostion, _T("检测："));
	cstrPostion.Format(_T("E%d:H%d"), maxRow, maxRow+1);
	pv.ExSetTxt(cstrPostion, _T("审核："));
	cstrPostion.Format(_T("I%d:K%d"), maxRow, maxRow+1);
	pv.ExSetTxt(cstrPostion, _T("批准："));
	
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

	if(pCScanView->m_nAxisSelect == 3
		|| pCScanView->m_nAxisSelect == 4)	
	{
		pv.ExSetTxt(_T("B20"),_T("minR"));
		pv.ExSetTxt(_T("D20"),_T("maxR"));
	}

	else if (pCScanView->m_nAxisSelect == 5)
	{
		pv.ExSetTxt(_T("B20"),_T("minW"));
		pv.ExSetTxt(_T("D20"),_T("maxW"));
		pv.ExSetTxt(_T("C20"),_T("minZ"));
		pv.ExSetTxt(_T("E20"),_T("maxZ"));
	}

	pv.ExShow();
	return;
	WPARAM wParam = MAKEWPARAM(ID_FILE_PRINT_PREVIEW, 0);
	m_pParentWnd->m_pCurCScanView->SendMessage(WM_COMMAND, wParam, 0);
}