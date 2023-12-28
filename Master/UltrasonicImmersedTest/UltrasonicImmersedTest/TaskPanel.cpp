// TaskPanel.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "TaskInfoDlg.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskItemDlg.h"
#include "MovePathView.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "SysParam.h"
#include "MechanicalControl.h"
#include "MechanicalControlPanel.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "CScanDlgBar.h" 
#include "TaskDockView.h"
#include "MainFrm.h"


// CTaskPanel

IMPLEMENT_DYNCREATE(CTaskPanel, CFormView)

CTaskPanel::CTaskPanel()
	: CFormView(CTaskPanel::IDD)
{

}

CTaskPanel::~CTaskPanel()
{
}

void CTaskPanel::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TASK_ITEMS, m_lstTaskItems);
}

BEGIN_MESSAGE_MAP(CTaskPanel, CFormView)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CTaskPanel::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_ITEM, &CTaskPanel::OnBnClickedButtonCreateItem)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CTaskPanel::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CTaskPanel::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CTaskPanel::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_ITEM, &CTaskPanel::OnBnClickedButtonSaveItem)
	ON_BN_CLICKED(IDC_BUTTON_DEL_ITEM, &CTaskPanel::OnBnClickedButtonDelItem)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_CUBE, &CTaskPanel::OnBnClickedButtonCreateCube)
END_MESSAGE_MAP()


// CTaskPanel diagnostics

#ifdef _DEBUG
void CTaskPanel::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTaskPanel::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CTaskPanel::CreateView(CWnd* pParent, CCreateContext* pContext)
{
	// Song Chenguang  add start on 2012-11-03
	if (pParent == NULL)
	{
		MessageBox(_T("CWnd* pParent 传参为空"), _T("错误"), MB_OK|MB_ICONERROR);

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTaskPanel::CreateView"),"Leave");

		return FALSE;
	}

	if (pContext == NULL)
	{
		MessageBox(_T("CWnd* pContext 传参为空"), _T("错误"), MB_OK|MB_ICONERROR);

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTaskPanel::CreateView"),"Leave");

		return FALSE;
	}
	// Song Chenguang  add end on 2012-11-03

	CRect rect(0,0,300,300);

	if (!Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		rect, pParent, AFX_IDW_PANE_FIRST, pContext))
	{
		TRACE0("Warning: couldn't create CMyFormView!\n");

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CTaskPanel::CreateView"),"Leave");

		return FALSE;
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CChannelPanel::CreateView"),"Leave");

	return TRUE;
}
// CTaskPanel message handlers

void CTaskPanel::OnBnClickedButtonCreate()
{
	// TODO: Add your control notification handler code here
	CreateTask();
}

BOOL CTaskPanel::CreateTask()
{
	CTaskInfoDlg dlg;
	dlg.m_strTaskName = m_strTaskName;

	if (dlg.DoModal() == IDOK)
	{
		// 任务创建
		// 设名字
		m_strTaskName = dlg.m_strTaskName;
		GetDlgItem(IDC_STATIC_TASK)->SetWindowText(m_strTaskName);

		// 清空列表项
		m_arrayMovePathItems.RemoveAll();
		m_lstTaskItems.DeleteAllItems();

		// 建立目录及任务列表文件
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

		if (pFrame->m_strDataFolder.Right(1) == _T("\\"))
		{
			m_strTaskPath = pFrame->m_strDataFolder + m_strTaskName;
		} 
		else
		{
			m_strTaskPath = pFrame->m_strDataFolder + _T("\\") + m_strTaskName;
		}
		

		if (!CreateDirectory(m_strTaskPath, NULL))
		{
			MessageBox(_T("创建任务目录失败！"));
			return FALSE;
		}
		// ?是否起作用
		SetCurrentDirectory(m_strTaskPath);

		m_strListFilePath = m_strTaskPath + _T("\\") +_T("任务列表.lst");
		CFile file;
		file.Open(m_strListFilePath, CFile::modeCreate);
		file.Close();

		return TRUE;
	}
	else
		return FALSE;
}

void CTaskPanel::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	// Items列表初始化
	m_lstTaskItems.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 40, 0);
	m_lstTaskItems.InsertColumn(1, _T("任务名称"), LVCFMT_CENTER, 150, 1);
//	m_lstTaskItems.InsertColumn(2, _T("保存"), CListCtrlEditable::CtrlType_Button, 0, true, LVCFMT_CENTER, 50);
	m_lstTaskItems.InsertColumn(2, _T("状态"), LVCFMT_CENTER, 50, 2);
	// Song Chenguang add start on 2013-09-13
#if _BEIFANG_ZHONGGONG
	GetDlgItem(IDC_BUTTON_CREATE_CUBE)->ShowWindow(TRUE);
#else
	GetDlgItem(IDC_BUTTON_CREATE_CUBE)->ShowWindow(FALSE);
#endif
	// Song Chenguang add end on 2013-09-13
}

void CTaskPanel::OnBnClickedButtonCreateItem()
{
	// TODO: Add your control notification handler code here
	CTaskItemDlg dlg;
	
	if (dlg.DoModal() == IDOK)
	{
		// GraphMove创建新文件
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
		CMovePathView* pView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0, 0);
		pView->m_GraphMove.NewFile();

		// 创建子项
		InsertItemToList(dlg.m_strTaskItemName);
	}
}

void CTaskPanel::InsertItemToList(const CString& strItemName)
{
	// 查询是否有此名称
	for (int i=0; i<m_lstTaskItems.GetItemCount(); i++)
	{
		CString strName = m_lstTaskItems.GetItemText(i,1);
		if (strName == strItemName)
		{
			CString str;
			str.Format(_T("%s 名称重复！"), strItemName);
			MessageBox(str, _T("提示"), MB_OK);
			return;
		}
	}
	// 创建子项
	LV_ITEM lvi;

	lvi.mask = LVIF_TEXT;// | LVIF_PARAM;
	lvi.iItem = m_lstTaskItems.GetItemCount();
	lvi.iSubItem = 0;
	CString strIndex;
	strIndex.Format(_T("%d"), lvi.iItem + 1);
	lvi.pszText = strIndex.GetBuffer(strIndex.GetLength());	// 序号
	int nRes = m_lstTaskItems.InsertItem(&lvi);

	int iSubItem = 0;
	m_lstTaskItems.SetItemText(lvi.iItem, ++iSubItem, strItemName);	// 名称
	//		++iSubItem;		//? 保存按钮
	m_lstTaskItems.SetItemText(lvi.iItem, ++iSubItem, _T("未保存"));			// 状态
}
void CTaskPanel::OnBnClickedButtonSave()
{
	// TODO: Add your control notification handler code here
	// 保存每项扫查文件路径到列表文件
	CStdioFile file;

	CFileException e;	
//	if(file.Open(m_strListFilePath, CFile::modeReadWrite | CFile::modeCreate | CFile::typeBinary, &e))
	if(file.Open(m_strListFilePath, CFile::modeReadWrite | CFile::modeCreate , &e))
	{
		
		int nCount = m_arrayMovePathItems.GetCount();
		for (int i=0; i< nCount; i++)
		{
//			file.Write("\377\376", 2);
			CString str = m_arrayMovePathItems[i] + _T("\r\n");
			file.WriteString(str);
		}
		
		file.Close();

		MessageBox(_T("任务保存成功！"));
	}
	else
	{
		e.ReportError();
	}

}

void CTaskPanel::OnBnClickedButtonLoad()
{
	// TODO: Add your control notification handler code here
	// 打开扫查文件路径到列表文件，填充面板	
	TCHAR szFilters[] =
		_T("NDT Task List files (*.lst)|*.lst|All files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("lst"), NULL,OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,	szFilters);

	if(dlg.DoModal() == IDOK)
	{
		m_arrayMovePathItems.RemoveAll();
		m_lstTaskItems.DeleteAllItems();

		m_strListFilePath = dlg.GetPathName();
		// 更新任务名称
		int nIndexR = m_strListFilePath.ReverseFind('\\');
		CString strTemp = m_strListFilePath.Left(nIndexR);
		int nIndexL = strTemp.ReverseFind('\\');
		m_strTaskName = strTemp.Right(strTemp.GetLength() - nIndexL - 1);
		GetDlgItem(IDC_STATIC_TASK)->SetWindowText(m_strTaskName);

		CStdioFile file;
		file.Open(m_strListFilePath, CFile::modeReadWrite);

		CString strItemPath;
		while (file.ReadString (strItemPath))
		{
			int index1 = strItemPath.ReverseFind('\\');
			int index2 = strItemPath.ReverseFind('.');
			CString strItemName = strItemPath.Mid(index1+1, (index2 - 1 - index1));

			// 创建子项
			LV_ITEM lvi;

			lvi.mask = LVIF_TEXT;// | LVIF_PARAM;
			lvi.iItem = m_lstTaskItems.GetItemCount();
			lvi.iSubItem = 0;
			CString strIndex;
			strIndex.Format(_T("%d"), lvi.iItem + 1);
			lvi.pszText = strIndex.GetBuffer(strIndex.GetLength());	// 序号
			int nRes = m_lstTaskItems.InsertItem(&lvi);

			int iSubItem = 0;
			m_lstTaskItems.SetItemText(lvi.iItem, ++iSubItem, strItemName);	// 名称
			m_lstTaskItems.SetItemText(lvi.iItem, ++iSubItem, _T("未运行"));			// 状态

			m_arrayMovePathItems.Add(strItemPath);	
		}

		file.Close();
	}

	// 打开第一个子项。。。
	
}

void CTaskPanel::OnBnClickedButtonRun()
{
	// TODO: Add your control notification handler code here
	CString strMessage;
	BOOL bRet = IsReady(strMessage);
	if (!bRet)
	{
		AfxMessageBox(strMessage);
		return;
	}

	GetDlgItem(IDC_BUTTON_RUN)->EnableWindow(FALSE);
	// 顺序执行路径文件
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMechanicalControlPanel* pPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;

	pPanel->m_wTaskItemCount = m_arrayMovePathItems.GetCount();
	pPanel->RunTask(m_strListFilePath);

}

void CTaskPanel::OnBnClickedButtonSaveItem()
{
	// TODO: Add your control notification handler code here
	

	// 保存扫描路径文件	
	int nIndex = m_lstTaskItems.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);

	BOOL bRet = SaveParaFile(nIndex, TRUE);

	CString str;
	if (!bRet)
		str.Format(_T("任务 %d 保存失败！"), nIndex+1);
	else	
		str.Format(_T("任务 %d 保存成功！"), nIndex+1);
	MessageBox(str);
}

BOOL CTaskPanel::SaveParaFile(const int& nIndex, const BOOL& bSaveMdsx)
{
	if (nIndex == -1)
	{
		return FALSE;
	}
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMovePathView* pView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0, 0);

	CString strItemName = m_lstTaskItems.GetItemText(nIndex, 1);
	
	CString strItemPath = m_strTaskPath + _T("\\") + strItemName + _T(".mdsx");

	if(bSaveMdsx)
	{		
		BOOL bRet = pView->m_GraphMove.SaveFileByPath(strItemPath);
		if (!bRet)
		{
//			MessageBox(_T("未画运行轨迹，保存文件失败！"));
			return FALSE;
		}
	}

	// 保存通道参数
	CString strChannelParaPath = m_strTaskPath + _T("\\") + strItemName + _T(".par");
	CChannelPanel* pChannel = pFrame->m_wndChannelInfo.m_pFormView;
	pChannel->SaveParam(strChannelParaPath);

	// 保存图像参数
	CString strCScanPara1 = m_strTaskPath + _T("\\") + strItemName + _T("1") + _T(".cpar");
	CCScanParamPanel* pCScanParaPanel1 = pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
	pCScanParaPanel1->SaveCScanPara(strCScanPara1);

	CString strCScanPara2 = m_strTaskPath + _T("\\") + strItemName + _T("2") + _T(".cpar");
	CCScanParamPanel* pCScanParaPanel2 = pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;
	pCScanParaPanel2->SaveCScanPara(strCScanPara2);

	// 保存路径到数组
	int nCount = m_arrayMovePathItems.GetCount();
	if (nCount > nIndex )
		m_arrayMovePathItems.SetAt(nIndex, strItemPath);
	else
		m_arrayMovePathItems.Add(strItemPath);

	m_lstTaskItems.SetItemText(nIndex, 2, _T("已保存"));

	return TRUE;
}

void CTaskPanel::OnBnClickedButtonDelItem()
{
	// TODO: Add your control notification handler code here
	int nIndex = m_lstTaskItems.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);

	if (nIndex == -1)
	{
		return;
	}

	// 删除
	int nRet = MessageBox(_T("此操作将删除相应的数据文件，是否删除？"), _T("提示"), MB_YESNO);

	if (nRet == IDYES)
	{
		CString strItemName = m_lstTaskItems.GetItemText(nIndex, 1);
		CString strItemPath = m_strTaskPath + _T("\\") + strItemName + _T(".mdsx");

		if (INVALID_HANDLE_VALUE != CreateFile(
			strItemPath, 0, 0, NULL, OPEN_EXISTING, 0, NULL))
		{
			DeleteFile(strItemPath);
		}

		CString strChannelParaPath = m_strTaskPath + _T("\\") + strItemName + _T(".par");
		if (INVALID_HANDLE_VALUE != CreateFile(
			strChannelParaPath, 0, 0, NULL, OPEN_EXISTING, 0, NULL))
		{
			DeleteFile(strChannelParaPath);
		}

		CString strCScanPara1 = m_strTaskPath + _T("\\") + strItemName + _T("1.cpar");
		if (INVALID_HANDLE_VALUE != CreateFile(
			strCScanPara1, 0, 0, NULL, OPEN_EXISTING, 0, NULL))
		{
			DeleteFile(strCScanPara1);
		}

		CString strCScanPara2 = m_strTaskPath + _T("\\") + strItemName + _T("2.cpar");
		if (INVALID_HANDLE_VALUE != CreateFile(
			strCScanPara2, 0, 0, NULL, OPEN_EXISTING, 0, NULL))
		{
			DeleteFile(strCScanPara2);
		}

		// List删除
		m_lstTaskItems.DeleteItem(nIndex);
		// 数组删除
		CString strItemState = m_lstTaskItems.GetItemText(nIndex, 2);
		if (strItemState != _T("未保存"))		
			m_arrayMovePathItems.RemoveAt(nIndex);
	}

}

void CTaskPanel::OnBnClickedButtonCreateCube()
{
	// TODO: Add your control notification handler code here
	BOOL bRetTask = CreateTask();
	if (bRetTask)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CMovePathView* pView =( CMovePathView*)pFrame->m_wndSplitter.GetPane(0, 0);

		pView->m_GraphMove.SetMdsxPath(m_strTaskPath);
		const int SURFACE_COUNT = 5;
		CString strSurfaceName[SURFACE_COUNT] = {_T("XY-Top"),_T("XZ-Right"),_T("YZ-Back"),_T("XZ-Left"), _T("YZ-Front")};
		pView->m_GraphMove.SetMdsxName(strSurfaceName[0], strSurfaceName[1], strSurfaceName[2], strSurfaceName[3], strSurfaceName[4]); 

		CMechanicalControlPanel* pPanel = (CMechanicalControlPanel*)pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
		pPanel->UpdateData(TRUE);
		pView->m_GraphMove.SetMdsxN(pPanel->m_dWaterDistance, pPanel->m_dABRadius, pPanel->m_dWRadius);
		BOOL bRet = pView->m_GraphMove.CubeShow();
		if (bRet)
		{
			for (int i=0; i<SURFACE_COUNT; i++)
			{
				InsertItemToList(strSurfaceName[i]);
				SaveParaFile(i, FALSE);
			}

			OnBnClickedButtonSave();
		}
	}
	
}

// Song Chenguang add start on 2013-05-
BOOL CTaskPanel::IsReady(CString& strMessage)
{
	// 任务未保存
	CFile file;
	file.Open(m_strListFilePath, CFile::modeReadWrite);
	ULONGLONG ullLength = file.GetLength();
	if (ullLength == 0)
	{
		strMessage = _T("任务未保存!");
		return FALSE;
	}
	// 未复位
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMovePathView* pView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
	int nStatus = pView->m_GraphMove.GetWorkStatus();
	if (!(nStatus == 10 || nStatus == 0))
	{
		strMessage = _T("设备未准备好！");
		return FALSE;
	}

	return TRUE;
}
// Song Chenguang add end on 2013-05-29
