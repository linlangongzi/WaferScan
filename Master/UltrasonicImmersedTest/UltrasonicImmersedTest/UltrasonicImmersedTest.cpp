// UltrasonicImmersedTest.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "CScanDlgBar.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "ExButton.h"
#include "SysParam.h"
#include "MechanicalControlPanel.h"
#include "MechanicalControl.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"
#include "MovePathView.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"

#include "UltrasonicImmersedTestDoc.h"
#include "FeatureInfoWnd.h"
#include "UltrasonicImmersedTestView.h"
#include "GlobalFunction.h"

#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
#include "LogOnDlg.h"
#include "elepopular.h"
#include "IniParams.h"
#include "platedrawctrl.h"
#include "platedrawctrl.h"
#include "mdsocx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMutex g_csCard;
HANDLE *hSaveDMADataEvent;
extern BOOL gbCScanOnSizeFlag;
BEGIN_MESSAGE_MAP(CUltrasonicImmersedTestApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CUltrasonicImmersedTestApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	// HuangYaChang add start on 2012-11-24
	ON_COMMAND(ID_APP_HELP, &CUltrasonicImmersedTestApp::OnAppHelp)
	// HuangYaChang add end on 2012-11-24
END_MESSAGE_MAP()



CUltrasonicImmersedTestApp::CUltrasonicImmersedTestApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::CUltrasonicImmersedTestApp"),"Enter");
	m_pAECWorkThread = NULL;
	m_strIPArray = NULL;
	m_ElepHandle = 0;
	m_fCODER_PER_RING_X = 1.0f;
	m_fCODER_PER_RING_Y = 1.0f;
	m_fCODER_PER_RING_Z = 1.0f;
	m_fCODER_PER_RING_W1 = 1.0f;
	m_fCODER_PER_RING_A = 1.0f;
	m_fCODER_PER_RING_B = 1.0f;
	m_fCODER_PER_RING_W2 = 1.0f;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::CUltrasonicImmersedTestApp"),"Leave");
}


// 唯一的一个 CUltrasonicImmersedTestApp 对象

CUltrasonicImmersedTestApp theApp;


// CUltrasonicImmersedTestApp 初始化

BOOL CUltrasonicImmersedTestApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Enter");	
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	// WangQianfeng add start on 2012-11-15
	char msg[] = ".\\log4cxx.properties";
	LoggerPtr rootLogger = Logger::getRootLogger();
	log4cxx::PropertyConfigurator::configure(msg);
	rootLogger->info("Entering application start.");
	//LOG4CXX_INFO(rootLogger, "The log4cxx call is successfully");
	// WangQianfeng add end on 2012-11-15

	CWinApp::InitInstance();

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
		return FALSE;
	}
	AfxEnableControlContainer();

	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("Sock初始化失败！"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
		return FALSE;
	}

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("奥瑞视（北京）科技有限公司"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)
#if !_OUTLINE	
	// 打开加密狗
	// 30303030-aa55aa55-55aa55aa-30303030
	//unsigned long RetVal = ElepOpenFirst(0, 0, 0x303030, 0xaa55aa55, 0x55aa55aa, 0x303030, &m_ElepHandle);
	//30303030-55aa55aa-aa55aa55-30303030
	unsigned long RetVal = ElepOpenFirst(0, 0, 0x30303030, 0x55aa55aa, 0xaa55aa55, 0x30303030, &m_ElepHandle);
	if(RetVal != ELEP_SUCCESS)
	{
		RetVal = ElepOpenFirst(0, 0, 0x30303030, 0xaa55aa55, 0x55aa55aa, 0x30303030, &m_ElepHandle);
		if(RetVal != ELEP_SUCCESS)
		{
			AfxMessageBox(_T("未授权,程序自动关闭!"));
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
			return FALSE;
		}

		//AfxMessageBox(_T("未授权,程序自动关闭!"));
		//LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
		//return FALSE;
	}

	// 读取配置参数
	// 读配置文件
	BOOL bRet = m_IniParams.ReadIniData();
//	m_IniParams.m_info.nCardNum = 1; // 只支持2通道
	if (!bRet)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
		return bRet;
	}
	else
	{
#endif
		CalcCoderScale();
#if !_OUTLINE
	}
	// wangbingfu add start on 2014-12-23
	bRet = m_IniParams.WriteIniData();
	if(!bRet)
	{
		AfxMessageBox(_T("过期，未授权!"));
		return FALSE;
	}
	// wangbingfu add end on 2014-12-23
#endif
	// 线程参数初始化
	::ZeroMemory(m_stThreadParam, sizeof(AECTHREAD_PARAM)*MAX_CARD_NO);

	for (int i=0; i<MAX_CARD_NO; i++)
	{
		m_stThreadParam[i].nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
	}

	InitHardwareConfig();
	m_pAECWorkThread = new CWinThread*[m_nCardNum];
	// WangQianfeng add start on 2012-11-02
	if ( NULL == m_pAECWorkThread )
	{
		AfxMessageBox(_T("CUltrasonicImmersedTestApp::InitInstance(),m_pAECWorkThread = null"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
		return FALSE;
	}
	// WangQianfeng add end on 2012-11-02
	for (int i=0; i<m_nCardNum; i++)
	{
		m_pAECWorkThread[i] = NULL;
	}

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CUltrasonicImmersedTestDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CUltrasonicImmersedTestView));
	if (!pDocTemplate)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
		return FALSE;
	}
	AddDocTemplate(pDocTemplate);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
		return FALSE;
	}

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	gbCScanOnSizeFlag = TRUE;
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);	
	m_pMainWnd->UpdateWindow();
	
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand  之后发生

	SetRegistryBase (_T("Settings"));

	// Initialize customization managers:
	InitMouseManager();
	InitContextMenuManager();
	InitKeyboardManager(); 

	// 查询A扫视图指针 		
	CUltrasonicImmersedTestView* pView = GetAScanView();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);

	for (int i=0; i < (m_IniParams.m_info.nCardNum); i++)
	{
		// 创建卡线程
		// 赋值
		m_stThreadParam[i + 1].hWnd = pView->GetSafeHwnd();	
		m_stThreadParam[i + 1].pAllConfig = pView->GetDocument()->m_stAllCH_CONFIG;
		m_stThreadParam[i + 1].index = i;
		m_stThreadParam[i + 1].hWndFeatureInfo = pFrame->m_pFeatureWnd->GetSafeHwnd();
		m_stThreadParam[i + 1].hWndCScan = pCScanView->GetSafeHwnd();	// wangbingfu add at 2011-9-27
		m_stThreadParam[i + 1].pFrame = pFrame;
		m_pAECWorkThread[i] = AfxBeginThread(AECWorkThreadProc, &m_stThreadParam[i + 1], THREAD_PRIORITY_NORMAL);
	}
#if !_OUTLINE	
	// 启动A扫描
	WPARAM wParam = MAKEWPARAM(ID_START_A_SCAN, 0);
	pFrame->SendMessage(WM_COMMAND, wParam, 0);

	// 启动监视数据
	wParam = MAKEWPARAM(ID_START_MONITOR_DATA, 0);
	pFrame->SendMessage(WM_COMMAND, wParam, 0);

	// 启动特征数据
	wParam = MAKEWPARAM(ID_START_FEATURE_DATA, 0);
	pFrame->SendMessage(WM_COMMAND, wParam, 0);


	// 启动命令通讯
	wParam = MAKEWPARAM(ID_CMD_S_CONNECT, 0);//yachang.huang add on 2013-11-26
	pFrame->SendMessage(WM_COMMAND, wParam, 0);//yachang.huang add on 2013-11-26
	
	// wangbingfu open start on 2014-10-16
	// Song Chenguang add start on 2013-12-17
	// 启动A扫数据通讯
	wParam = MAKEWPARAM(ID_A_SCAN_DATA_START, 0);
	pFrame->SendMessage(WM_COMMAND, wParam, 0);
	// Song Chenguang add end on 2013-12-17
	// wangbingfu open end on 2014-10-16

#endif
	// Song Chenguang add start on 2012-07-26
	// 下发工作模式
	int nCardNum = GetCardNum();
	for (int i=0; i<nCardNum; i++)
	{
		pFrame->m_wndChannelInfo.m_pFormView->SendWorkMode(i, pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode);
	}

	CString strProbeNum = GetProfileString(_T("水浸超声探伤系统"), _T("ProbeNum"), _T("1"));
	int nProbeNum = _wtoi(strProbeNum.GetBuffer(strProbeNum.GetLength()));
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nIndexProbeNum = nProbeNum - 1;

	CString strOneProbeType = GetProfileString(_T("水浸超声探伤系统"), _T("OneProbeType"), _T("1"));
	int nIndexOneProbeType = _wtoi(strOneProbeType.GetBuffer(strOneProbeType.GetLength()));
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nIndexOneProbeType = nIndexOneProbeType;

	CString strTwoProbeType = GetProfileString(_T("水浸超声探伤系统"), _T("TwoProbeType"), _T("1"));
	int nIndexTwoProbeType = _wtoi(strTwoProbeType.GetBuffer(strTwoProbeType.GetLength()));
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nIndexTwoProbeType = nIndexOneProbeType;

	CString strThreeProbeType = GetProfileString(_T("水浸超声探伤系统"), _T("ThreeProbeType"), _T("1"));
	int nIndexThreeProbeType = _wtoi(strThreeProbeType.GetBuffer(strThreeProbeType.GetLength()));
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nIndexThreeProbeType = nIndexThreeProbeType;

	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.SetMoveRule(nProbeNum, nIndexOneProbeType,
		nIndexTwoProbeType, nIndexThreeProbeType, pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode);	
	// Song Chenguang add end on 2012-07-26
	// 读取注册表
	CString strFolder = GetProfileString(_T("水浸超声探伤系统"), _T("Data Folder"));
	
	if(strFolder == _T(""))// 无值
	{
		TCHAR strFolder[MAX_PATH];
		DWORD dwSize = GetWorkingFolder(strFolder, MAX_PATH);
		dwSize -= 2;

		do {
			dwSize--;
		} while(strFolder[dwSize] != TCHAR('\\') && dwSize > 0);

		if (dwSize > 0)
			dwSize++;
		strFolder[dwSize] = _T('\0');

		pFrame->m_strDataFolder = CString(strFolder) + _T("Result\\");		

		// 写入注册表
		WriteProfileString(_T("水浸超声探伤系统"), _T("Data Folder"), pFrame->m_strDataFolder);
	}
	else
	{
		pFrame->m_strDataFolder = strFolder.Left(strFolder.GetLength()); // Song Chenguang modified on 2013-03-28
	}

	// 检查目录是否存在
	DWORD   dwAttr   =   GetFileAttributes(pFrame->m_strDataFolder); 
	if(dwAttr == INVALID_FILE_ATTRIBUTES || (dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0 )//目录不存在 
	{ 
		CString strFolder = pFrame->m_strDataFolder;
		if (strFolder.Right(1) == _T("\\"))
		{
			strFolder = strFolder.Left(strFolder.GetLength() - 1);
		}
		if(!CreateDirectory(strFolder, NULL)) 
		{ 
			AfxMessageBox(_T("不能创建目录 "));
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
			return FALSE; 
		}
	}
	
	// Song Chenguang add start on 2013-06-17
	TCHAR strExeFolder[MAX_PATH];
	GetWorkingFolder(strExeFolder, MAX_PATH);
	CString strPath = CString(strExeFolder) + _T("扫描数据临时文件位置.txt");
	// Song Chenguang add start on 2013-06-17
	BOOL bRet3 = ::PathFileExists(strPath);
	if (bRet3)	// 程序在上次运行时崩溃
	{
		int nRet = MessageBox(NULL,_T("程序之前有未知原因造成的崩溃，是否恢复上次数据？"), _T("提示"), MB_YESNO | MB_ICONQUESTION);

		// 恢复到指定路径
		if (nRet == IDYES)
		{
			TCHAR szFilters[] =
				_T ("NDT Test Result files (*.uit)|*.uit||");

			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

			CString stroMaterial=pFrame->m_ProbeSet.getVal(_T("oMaterial"));
			CString stroId=pFrame->m_ProbeSet.getVal(_T("oId"));
			CString stroType=pFrame->m_ProbeSet.getVal(_T("oType"));

			CString timeStr;
			CTime day;
			day = CTime::GetCurrentTime();
			int filenameday, filenamemonth, filenameyear, filehour, filemin, filesec;
			filenameday = day.GetDay();		// dd
			filenamemonth = day.GetMonth();	// mm月份
			filenameyear = day.GetYear();	// yyyy
			filehour = day.GetHour();		// hh
			filemin = day.GetMinute();		// mm分钟
			filesec = day.GetSecond();		// ss
			timeStr.Format(_T("%s_%s_%s_%04d%02d%02d%02d%02d%02d"),stroMaterial,stroId,stroType,filenameyear,filenamemonth,filenameday,filehour,filemin,filesec);

			CFileDialog dlg(FALSE, _T("uit"), timeStr, OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,	szFilters);
			dlg.m_ofn.lpstrInitialDir = pFrame->m_strDataFolder;

			if (dlg.DoModal() == IDOK)
			{
				try
				{
					CString strPath2 = dlg.GetPathName();

					CStdioFile file;
					file.Open(strPath, CFile::modeReadWrite);
					CString str;
					file.ReadString(str);
					file.Close();

					BOOL bRet2 = CopyFile(str, strPath2, TRUE);

					if (!bRet2)
						return FALSE;
					
					// 读取文件，显示
					pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_filename = strPath2;
					pFrame->m_wndCScanParamDockBar.m_pCScanParamView->ReadFile();
					// machao modify start on 2013-08-14
//					CCScanView *pCScanView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_pCScanView;
					CCScanView *pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2, 0);
					// machao modify end on 2013-08-14
					pCScanView->DrawPicture(&pCScanView->m_MemDC);
					
					AfxMessageBox(_T("成像完毕！"));

					pCScanView->bFreePictureMemoy = FALSE;
					// 恢复路径文件和轨迹文件
					int nIndex = str.ReverseFind('.');
					CString strMdsPath = str.Left(nIndex) + _T(".apf");
					CMovePathView* pMoveView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0, 0);
					AfxMessageBox(strMdsPath);
					pMoveView->m_GraphMove.OpenFileByPath(strMdsPath);

					CString strChannelPath = str.Left(nIndex) + _T(".asf");
					AfxMessageBox(strChannelPath);
					pFrame->m_wndChannelInfo.m_pFormView->ReadParam(strChannelPath);

					// 显示最后一个点数据到机械参数面板PTP处
					CUltrasonicImmersedTestDoc* pDoc = pCScanView->GetDocument();

					int nLastIndex = CHANNEL_NUM-1;
					for (int i = CHANNEL_NUM-1; i>=0; i--)
					{
						if (pDoc->m_lstFlawData[i].GetCount() > 0)
						{
							nLastIndex = i;
							break;
						}
					}

					CFeatureInfo feature = pDoc->m_lstFlawData[nLastIndex].GetTail();

					CString strInfo;
					strInfo.Format(_T("%.1f"), feature.m_dX);
					pFrame->m_wndMechanicalControl.m_pMechanicalControlView->SetDlgItemText(IDC_EDIT_XCOORD,strInfo);

					strInfo.Format(_T("%.1f"), feature.m_dY);
					pFrame->m_wndMechanicalControl.m_pMechanicalControlView->SetDlgItemText(IDC_EDIT_YCOORD,strInfo);

					strInfo.Format(_T("%.1f"), feature.m_dZ);
					pFrame->m_wndMechanicalControl.m_pMechanicalControlView->SetDlgItemText(IDC_EDIT_ZCOORD,strInfo);

					strInfo.Format(_T("%.1f"), feature.m_dB);
					pFrame->m_wndMechanicalControl.m_pMechanicalControlView->SetDlgItemText(IDC_EDIT_ABCOORD,strInfo);

					strInfo.Format(_T("%.1f"), feature.m_dW);
					pFrame->m_wndMechanicalControl.m_pMechanicalControlView->SetDlgItemText(IDC_EDIT_WCOORD,strInfo);

					strInfo.Format(_T("%.1f"), feature.m_dR);
					pFrame->m_wndMechanicalControl.m_pMechanicalControlView->SetDlgItemText(IDC_EDIT_RCOORD,strInfo);		

					AfxMessageBox(_T("最后点坐标显示完毕"));
					// 获取轨迹终点，显示在取点的位置
					double dXEnd = 0.0f;
					double dYEnd = 0.0f;
					double dZEnd = 0.0f;
					double dABEnd = 0.0f;	
					double dWEnd = 0.0f;
					double dREnd = 0.0f;
					pMoveView->m_GraphMove.GraphEndPoint(&dXEnd, &dYEnd, &dZEnd, 
						&dABEnd, &dWEnd, &dREnd);

					CString strX;
					strX.Format(_T("%.2f"), dXEnd);
					CString strY;
					strY.Format(_T("%.2f"), dYEnd);
					CString strZ;
					strZ.Format(_T("%.2f"), dZEnd);

					int iSubItem = 0;
					int nAxisIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();
					BOOL bRet = FALSE;
					if (nAxisIndex == 0)
					{
						bRet = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetItemText(1, ++iSubItem, strX);	// X
						bRet = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetItemText(1, ++iSubItem, strY);	// Y
						bRet = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetItemText(1, ++iSubItem, strZ);	// Z
					}
					else if (nAxisIndex == 1)
					{
						bRet = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetItemText(1, ++iSubItem, strX);	// X		
						bRet = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetItemText(1, ++iSubItem, strZ);	// Z
						bRet = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetItemText(1, ++iSubItem, strY);	// Y
					}
					else if (nAxisIndex == 2)
					{		
						bRet = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetItemText(1, ++iSubItem, strY);	// Y
						bRet = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetItemText(1, ++iSubItem, strZ);	// Z
						bRet = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_lstRectPoints.SetItemText(1, ++iSubItem, strX);	// X
					}
					else
					{
					}

					// 读注册表，崩溃时的行数
					UINT nLineNum = GetProfileInt(_T("水浸超声探伤系统"), _T("HaltLineNum"), 0);
					
					pMoveView->m_GraphMove.SetBreakPoint(long(nLineNum - 2));					
				}
				catch (CMemoryException* e)
				{
					ErroWindowMessage();
					e->ReportError();
				}
				catch (CFileException* e)
				{
					ErroWindowMessage();
					e->ReportError();
				}
				catch (CException* e)
				{
					ErroWindowMessage();
					e->ReportError();
				}
				
			}

		}
	}
	// Song Chenguang add end on 2013-06-17

	// machao add start on 2013-11-11
	if (IDOK == AfxMessageBox(_T("是否还原上次配置？"),MB_OKCANCEL | MB_ICONQUESTION))
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

		GetProfileString(_T("水浸超声探伤系统"), _T("apfFilePath"));
		CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
		if (PathFileExists(GetProfileString(_T("水浸超声探伤系统"), _T("apfFilePath"))))
		{
			CMovePathView* mdsocx = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
			mdsocx->m_GraphMove.OpenFileByPath(GetProfileString(_T("水浸超声探伤系统"), _T("apfFilePath")));
		}

		if (PathFileExists(GetProfileString(_T("水浸超声探伤系统"), _T("asfFilePath"))))
		{
			pChannelPanel->ReadParam(GetProfileString(_T("水浸超声探伤系统"), _T("asfFilePath")));
			pChannelPanel->DoChangeChannel();
			pChannelPanel->UpdateData(FALSE);
		}
	}
	// machao add end on 2013-11-11
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::DoDataExchange"),"Enter");

	CDialog::DoDataExchange(pDX);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::DoDataExchange"),"Leave");
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CUltrasonicImmersedTestApp::OnAppAbout()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::OnAppAbout"),"Enter");

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::OnAppAbout"),"Leave");
}


// CUltrasonicImmersedTestApp 消息处理程序


int CUltrasonicImmersedTestApp::ExitInstance()
{
	// TODO: Add your specialized code here and/or call the base class
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::ExitInstance"),"Enter");

	::BCGCBProCleanUp ();

	if (hSaveDMADataEvent != NULL)
	{
		delete[] hSaveDMADataEvent;
		hSaveDMADataEvent = NULL;
	}

	if (m_pAECWorkThread != NULL)
	{
		delete[] m_pAECWorkThread;
		m_pAECWorkThread = NULL;
	}

	if (m_strIPArray != NULL)
	{
		delete []m_strIPArray;
		m_strIPArray = NULL;
	}
#if !_OUTLINE
	// 关闭加密狗
	ElepClose(m_ElepHandle);
#endif	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::ExitInstance"),"Leave");
	return __super::ExitInstance();
}

/** 
* 获取A扫描视图指针                                        
* @return    void
* @since     1.0
*/ 
CUltrasonicImmersedTestView* CUltrasonicImmersedTestApp::GetAScanView()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::GetAScanView"),"Enter");

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	CUltrasonicImmersedTestView* pView = (CUltrasonicImmersedTestView*)pMainFrame->m_wndSplitter.GetPane(1, 0);

	if (pView == NULL)
	{
		AfxMessageBox(_T("error"));
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::GetAScanView"),"Leave");
	return pView;
}

BOOL CAboutDlg::OnInitDialog()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::OnInitDialog"),"Enter");

	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	// 读取版本信息
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	// songchenguang add start 2012-02-28
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMovePathView* pView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
	CString strVer;
	CString strMachine;
	strMachine.Format(_T("设备型号: %s\r\n"), pApp->m_IniParams.m_info.wcMechineType);
	CString strSaleTime;
	strSaleTime.Format(_T("出厂日期: %d年%d月%d日\r\n"), pApp->m_IniParams.m_info.SaleTime.GetYear(), 
		pApp->m_IniParams.m_info.SaleTime.GetMonth(), pApp->m_IniParams.m_info.SaleTime.GetDay());
	strVer = strVer + strMachine + strSaleTime;

	CString strVerOcx;
	strVerOcx.Format(_T("运动控件版本: %f \r\n"), pView->m_GraphMove.Ver());

	strVer = strVer + strVerOcx;
	int nAECNum = pApp->GetCardNum();
	for(int i=0; i<nAECNum; i++)
	{
		CString strVerDown;
		strVerDown.Format(_T("下位机硬件版本: %.2f \r\n下位机软件版本: %.2f \r\n"), pFrame->m_stYiQiVer[i].wHardWareVer, pFrame->m_stYiQiVer[i].wSoftWareVer);
		strVer = strVer + strVerDown;
	}	

	// songchenguang add end 2012-02-28
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_HARDWARE_VER);
	pEdit->SetWindowText(strVer);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::OnInitDialog"),"Leave");
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/** 
* 配置卡数和IP                                        
* @return    void
* @since     1.0
*/
void CUltrasonicImmersedTestApp::InitHardwareConfig()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitHardwareConfig"),"Enter");
//	m_IniParams.m_info.nCardNum = 1;	// test
	m_nCardNum = m_IniParams.m_info.nCardNum;
	m_strIPArray = new CString[m_nCardNum];
	// WangQianfeng add start on 2012-11-02
	if ( NULL == m_strIPArray )
	{
		AfxMessageBox(_T("CUltrasonicImmersedTestApp::InitHardwareConfig(),m_strIPArray = null"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
		return;
	}
	// WangQianfeng add end on 2012-11-02
	for (int i=0; i<m_nCardNum; i++)
	{
		m_strIPArray[i].Format(_T("%d.%d.%d.%d"), m_IniParams.m_info.FieldValue1[i], 
			m_IniParams.m_info.FieldValue2[i], m_IniParams.m_info.FieldValue3[i], m_IniParams.m_info.FieldValue4[i]);
	}

//	m_strIPArray[0] = _T("192.168.1.80");
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitHardwareConfig"),"Leave");
}

/** 
* 计算长度与脉冲比值                                        
* @return    void
* @since     1.0
*/
void CUltrasonicImmersedTestApp::CalcCoderScale()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::CalcCoderScale"),"Enter");
#if !_OUTLINE
	m_fCODER_PER_RING_X = m_IniParams.m_info.nXCoderPerRingNum * m_IniParams.m_info.fXJianSuBi / m_IniParams.m_info.fXDiameter;
	m_fCODER_PER_RING_Y = m_IniParams.m_info.nYCoderPerRingNum * m_IniParams.m_info.fYJianSuBi / m_IniParams.m_info.fYDiameter;
	m_fCODER_PER_RING_Z = m_IniParams.m_info.nZCoderPerRingNum * m_IniParams.m_info.fZJianSuBi / m_IniParams.m_info.fZDiameter;
	m_fCODER_PER_RING_W1 = m_IniParams.m_info.nW1CoderPerRingNum * m_IniParams.m_info.fW1JianSuBi / 360.0f;
	m_fCODER_PER_RING_A = m_IniParams.m_info.nACoderPerRingNum * m_IniParams.m_info.fAJianSuBi  / 360.0f; // Song chenguang modified on 2013-03-29
	m_fCODER_PER_RING_B = m_IniParams.m_info.nBCoderPerRingNum * m_IniParams.m_info.fBJianSuBi  / 360.0f; // Song chenguang modified on 2013-03-29
	m_fCODER_PER_RING_W2 = m_IniParams.m_info.nW2CoderPerRingNum * m_IniParams.m_info.fW2JianSuBi / 360.0f;
#else
	m_fCODER_PER_RING_X = 1;
	m_fCODER_PER_RING_Y = 1;
	m_fCODER_PER_RING_Z = 1;
	m_fCODER_PER_RING_W1 = 1;
	m_fCODER_PER_RING_A = 1; 
	m_fCODER_PER_RING_B = 1; 
	m_fCODER_PER_RING_W2 = 1;
#endif
}	
// HuangYaChang add start on 2012-11-24
void CUltrasonicImmersedTestApp::OnAppHelp()
{
	// TODO: Add your command handler code here

	TCHAR strFolder[MAX_PATH];
	GetWorkingFolder(strFolder, MAX_PATH); 
	CString strFileName = CString(strFolder);
	CString sHelp = strFileName + _T("\\ARS超声C扫描系统说明书.chm");
	if (PathFileExists(sHelp))
	{
		::ShellExecute(NULL,_T("open"),sHelp,NULL,NULL,SW_SHOW   );
		return;
	}
	else
	{
		AfxMessageBox(_T("ARS超声C扫描系统说明书.chm不存在"),MB_OK);
	}
}
// HuangYaChang add end on 2012-11-24
