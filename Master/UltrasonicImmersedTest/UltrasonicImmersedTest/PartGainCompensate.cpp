// PartGainCompensate.cpp : implementation file
//

#include "stdafx.h"
#include "CScanDlgBar.h" 
#include "ExButton.h"
#include "UltrasonicImmersedTest.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "MovePathView.h"
#include "SysParam.h"
#include "MechanicalControl.h"
#include "MechanicalControlPanel.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"
#include "MainFrm.h"
#include "PartGainCompensate.h"
#include "CScanView.h"
// CPartGainCompensateDlg dialog
IMPLEMENT_DYNAMIC(CPartGainCompensateDlg, CDialog)

CPartGainCompensateDlg::CPartGainCompensateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPartGainCompensateDlg::IDD, pParent)
	, m_fWStart(0)
	, m_fWStop(360)
	, m_fZStart(0)
	, m_fZStop(360)
	, m_fdBValue(0)
	, m_fPreviousdBValue(0)
	, m_nSelectMode(0)	// machao add on 2013-08-23
{

}

CPartGainCompensateDlg::~CPartGainCompensateDlg()
{
}

void CPartGainCompensateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_W_START, m_fWStart);
	DDX_Text(pDX, IDC_EDIT_W_STOP, m_fWStop);
	DDX_Text(pDX, IDC_EDIT_Z_START, m_fZStart);
	DDX_Text(pDX, IDC_EDIT_Z_STOP, m_fZStop);
	DDX_Text(pDX, IDC_EDIT_DB_COMPENSATE_VALUE, m_fdBValue);
	DDX_Radio(pDX, IDC_RADIO_MANUALLY_SELECT, m_nSelectMode);	// machao add on 2013-08-23
}


BEGIN_MESSAGE_MAP(CPartGainCompensateDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CPartGainCompensateDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_INSTALL, &CPartGainCompensateDlg::OnBnClickedButtonInstall)
	ON_BN_CLICKED(IDC_BUTTON_COMPENSATE, &CPartGainCompensateDlg::OnBnClickedButtonCompensate)
	// machao add start on 2013-08-23
	ON_BN_CLICKED(IDC_RADIO_MANUALLY_SELECT, &CPartGainCompensateDlg::OnBnClickedRadioManuallySelect)
	ON_BN_CLICKED(IDC_RADIO_MOUSE_SELECT, &CPartGainCompensateDlg::OnBnClickedRadioMouseSelect)
	// machao add end on 2013-08-23
END_MESSAGE_MAP()

// CPartGainCompensateDlg message handlers

BOOL CPartGainCompensateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	int nAxisIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();
	switch(nAxisIndex)
	{
	case 0:
		GetDlgItem(IDC_STATIC_TRANSVERSE_START)->SetWindowText(_T(" X起点："));
		GetDlgItem(IDC_STATIC_TRANSVERSE_STOP)->SetWindowText(_T(" X终点："));
		GetDlgItem(IDC_STATIC_VERTICAL_START)->SetWindowText(_T(" Y起点："));
		GetDlgItem(IDC_STATIC_VERTICAL_STOP)->SetWindowText(_T(" Y终点："));
		break;
	case 1:
		GetDlgItem(IDC_STATIC_TRANSVERSE_START)->SetWindowText(_T(" X起点："));
		GetDlgItem(IDC_STATIC_TRANSVERSE_STOP)->SetWindowText(_T(" X终点："));
		GetDlgItem(IDC_STATIC_VERTICAL_START)->SetWindowText(_T(" Z起点："));
		GetDlgItem(IDC_STATIC_VERTICAL_STOP)->SetWindowText(_T(" Z终点："));
		break;
	case 2:
		GetDlgItem(IDC_STATIC_TRANSVERSE_START)->SetWindowText(_T(" Y起点："));
		GetDlgItem(IDC_STATIC_TRANSVERSE_STOP)->SetWindowText(_T(" Y终点："));
		GetDlgItem(IDC_STATIC_VERTICAL_START)->SetWindowText(_T(" Z起点："));
		GetDlgItem(IDC_STATIC_VERTICAL_STOP)->SetWindowText(_T(" Z终点：")); 
		break;
	case 3:
	case 4:
		GetDlgItem(IDC_STATIC_TRANSVERSE_START)->SetWindowText(_T(" X起点："));
		GetDlgItem(IDC_STATIC_TRANSVERSE_STOP)->SetWindowText(_T(" X终点："));
		GetDlgItem(IDC_STATIC_VERTICAL_START)->SetWindowText(_T(" R起点："));
		GetDlgItem(IDC_STATIC_VERTICAL_STOP)->SetWindowText(_T(" R终点："));
		break;
	case 5:
		GetDlgItem(IDC_STATIC_TRANSVERSE_START)->SetWindowText(_T(" W起点："));
		GetDlgItem(IDC_STATIC_TRANSVERSE_STOP)->SetWindowText(_T(" W终点："));
		GetDlgItem(IDC_STATIC_VERTICAL_START)->SetWindowText(_T(" Z起点："));
		GetDlgItem(IDC_STATIC_VERTICAL_STOP)->SetWindowText(_T(" Z终点："));
		break;
	default:
		GetDlgItem(IDC_STATIC_TRANSVERSE_START)->SetWindowText(_T(" X起点："));
		GetDlgItem(IDC_STATIC_TRANSVERSE_STOP)->SetWindowText(_T(" X终点："));
		GetDlgItem(IDC_STATIC_VERTICAL_START)->SetWindowText(_T(" Y起点："));
		GetDlgItem(IDC_STATIC_VERTICAL_STOP)->SetWindowText(_T(" Y终点："));
		break;
	}

	return TRUE;
}

void CPartGainCompensateDlg::OnCancel()
{
	((CCScanView*)GetParent())->m_pPartGainCompensateDlg = NULL;
	DestroyWindow();
}

void CPartGainCompensateDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	AfxGetMainWnd()->SendMessage(WM_USER_DIALOG_DESTROYED, 0, 0);
	delete this;
}
void CPartGainCompensateDlg::OnBnClickedButtonReset()
{
	// TODO: Add your control notification handler code here
	CEdit *pEdit = new CEdit;
	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_DB_COMPENSATE_VALUE);
	CString strdBValue;
	strdBValue.Format(_T("%d"), 0);
	pEdit->SetWindowText(strdBValue);
	m_fdBValue = _tstoi(strdBValue);
	m_fPreviousdBValue = 0;

	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pPanel = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	if (pPanel->m_pCurCScanView->m_bImage == TRUE && pPanel->m_pCurCScanView->m_bCompensateRange == TRUE)
	{
		pPanel->OnBnClickedButtonDrawPicture();
	}
	CCScanParamPanel* pPanel2 = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;
	if (pPanel2->m_pCurCScanView->m_bImage == TRUE && pPanel2->m_pCurCScanView->m_bCompensateRange == TRUE)
	{
		pPanel2->OnBnClickedButtonDrawPicture();
	}
}

void CPartGainCompensateDlg::OnBnClickedButtonInstall()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

 	CCScanView* pView = (CCScanView*)GetParent();
 	pView->Invalidate(TRUE);
}

void CPartGainCompensateDlg::OnBnClickedButtonCompensate()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	if (pView->m_bImage == TRUE && pView->m_bCompensateRange == TRUE)
	{
		pView->DrawDBCompensatesite();
	}
	CCScanView* pView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
	if (pView2->m_bImage == TRUE && pView2->m_bCompensateRange == TRUE)
	{
		pView2->DrawDBCompensatesite();
	}
}
// machao add start on 2013-08-23
void CPartGainCompensateDlg::OnBnClickedRadioManuallySelect()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_W_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_W_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_Z_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_Z_STOP)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_INSTALL)->EnableWindow(TRUE);
	UpdateData(TRUE);
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	if (pView->m_bImage == TRUE && pView->m_bCompensateRange == TRUE)
	{
		pView->m_bMouseGainRangeSelect = FALSE;
	}
	CCScanView* pView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
	if (pView2->m_bImage == TRUE && pView2->m_bCompensateRange == TRUE)
	{
		pView2->m_bMouseGainRangeSelect = FALSE;
	}
}

void CPartGainCompensateDlg::OnBnClickedRadioMouseSelect()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_W_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_W_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Z_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_Z_STOP)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_INSTALL)->EnableWindow(FALSE);
	UpdateData(TRUE);
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
	if (pView->m_bImage == TRUE && pView->m_bCompensateRange == TRUE)
	{
		pView->m_bMouseGainRangeSelect = TRUE;
	}
	CCScanView* pView2 = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
	if (pView2->m_bImage == TRUE && pView2->m_bCompensateRange == TRUE)
	{
		pView2->m_bMouseGainRangeSelect = TRUE;
	}
}
// machao add end on 2013-08-23