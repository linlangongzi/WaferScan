// UltrasonicImmersedTest.cpp : ����Ӧ�ó��������Ϊ��
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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	// HuangYaChang add start on 2012-11-24
	ON_COMMAND(ID_APP_HELP, &CUltrasonicImmersedTestApp::OnAppHelp)
	// HuangYaChang add end on 2012-11-24
END_MESSAGE_MAP()



CUltrasonicImmersedTestApp::CUltrasonicImmersedTestApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
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


// Ψһ��һ�� CUltrasonicImmersedTestApp ����

CUltrasonicImmersedTestApp theApp;


// CUltrasonicImmersedTestApp ��ʼ��

BOOL CUltrasonicImmersedTestApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Enter");	
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
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

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
		return FALSE;
	}
	AfxEnableControlContainer();

	if (!AfxSocketInit())
	{
		AfxMessageBox(_T("Sock��ʼ��ʧ�ܣ�"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
		return FALSE;
	}

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("�����ӣ��������Ƽ����޹�˾"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)
#if !_OUTLINE	
	// �򿪼��ܹ�
	// 30303030-aa55aa55-55aa55aa-30303030
	//unsigned long RetVal = ElepOpenFirst(0, 0, 0x303030, 0xaa55aa55, 0x55aa55aa, 0x303030, &m_ElepHandle);
	//30303030-55aa55aa-aa55aa55-30303030
	unsigned long RetVal = ElepOpenFirst(0, 0, 0x30303030, 0x55aa55aa, 0xaa55aa55, 0x30303030, &m_ElepHandle);
	if(RetVal != ELEP_SUCCESS)
	{
		RetVal = ElepOpenFirst(0, 0, 0x30303030, 0xaa55aa55, 0x55aa55aa, 0x30303030, &m_ElepHandle);
		if(RetVal != ELEP_SUCCESS)
		{
			AfxMessageBox(_T("δ��Ȩ,�����Զ��ر�!"));
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
			return FALSE;
		}

		//AfxMessageBox(_T("δ��Ȩ,�����Զ��ر�!"));
		//LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
		//return FALSE;
	}

	// ��ȡ���ò���
	// �������ļ�
	BOOL bRet = m_IniParams.ReadIniData();
//	m_IniParams.m_info.nCardNum = 1; // ֻ֧��2ͨ��
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
		AfxMessageBox(_T("���ڣ�δ��Ȩ!"));
		return FALSE;
	}
	// wangbingfu add end on 2014-12-23
#endif
	// �̲߳�����ʼ��
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

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CUltrasonicImmersedTestDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CUltrasonicImmersedTestView));
	if (!pDocTemplate)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
		return FALSE;
	}
	AddDocTemplate(pDocTemplate);

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
		return FALSE;
	}

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	gbCScanOnSizeFlag = TRUE;
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);	
	m_pMainWnd->UpdateWindow();
	
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand  ֮����

	SetRegistryBase (_T("Settings"));

	// Initialize customization managers:
	InitMouseManager();
	InitContextMenuManager();
	InitKeyboardManager(); 

	// ��ѯAɨ��ͼָ�� 		
	CUltrasonicImmersedTestView* pView = GetAScanView();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);

	for (int i=0; i < (m_IniParams.m_info.nCardNum); i++)
	{
		// �������߳�
		// ��ֵ
		m_stThreadParam[i + 1].hWnd = pView->GetSafeHwnd();	
		m_stThreadParam[i + 1].pAllConfig = pView->GetDocument()->m_stAllCH_CONFIG;
		m_stThreadParam[i + 1].index = i;
		m_stThreadParam[i + 1].hWndFeatureInfo = pFrame->m_pFeatureWnd->GetSafeHwnd();
		m_stThreadParam[i + 1].hWndCScan = pCScanView->GetSafeHwnd();	// wangbingfu add at 2011-9-27
		m_stThreadParam[i + 1].pFrame = pFrame;
		m_pAECWorkThread[i] = AfxBeginThread(AECWorkThreadProc, &m_stThreadParam[i + 1], THREAD_PRIORITY_NORMAL);
	}
#if !_OUTLINE	
	// ����Aɨ��
	WPARAM wParam = MAKEWPARAM(ID_START_A_SCAN, 0);
	pFrame->SendMessage(WM_COMMAND, wParam, 0);

	// ������������
	wParam = MAKEWPARAM(ID_START_MONITOR_DATA, 0);
	pFrame->SendMessage(WM_COMMAND, wParam, 0);

	// ������������
	wParam = MAKEWPARAM(ID_START_FEATURE_DATA, 0);
	pFrame->SendMessage(WM_COMMAND, wParam, 0);


	// ��������ͨѶ
	wParam = MAKEWPARAM(ID_CMD_S_CONNECT, 0);//yachang.huang add on 2013-11-26
	pFrame->SendMessage(WM_COMMAND, wParam, 0);//yachang.huang add on 2013-11-26
	
	// wangbingfu open start on 2014-10-16
	// Song Chenguang add start on 2013-12-17
	// ����Aɨ����ͨѶ
	wParam = MAKEWPARAM(ID_A_SCAN_DATA_START, 0);
	pFrame->SendMessage(WM_COMMAND, wParam, 0);
	// Song Chenguang add end on 2013-12-17
	// wangbingfu open end on 2014-10-16

#endif
	// Song Chenguang add start on 2012-07-26
	// �·�����ģʽ
	int nCardNum = GetCardNum();
	for (int i=0; i<nCardNum; i++)
	{
		pFrame->m_wndChannelInfo.m_pFormView->SendWorkMode(i, pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode);
	}

	CString strProbeNum = GetProfileString(_T("ˮ������̽��ϵͳ"), _T("ProbeNum"), _T("1"));
	int nProbeNum = _wtoi(strProbeNum.GetBuffer(strProbeNum.GetLength()));
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nIndexProbeNum = nProbeNum - 1;

	CString strOneProbeType = GetProfileString(_T("ˮ������̽��ϵͳ"), _T("OneProbeType"), _T("1"));
	int nIndexOneProbeType = _wtoi(strOneProbeType.GetBuffer(strOneProbeType.GetLength()));
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nIndexOneProbeType = nIndexOneProbeType;

	CString strTwoProbeType = GetProfileString(_T("ˮ������̽��ϵͳ"), _T("TwoProbeType"), _T("1"));
	int nIndexTwoProbeType = _wtoi(strTwoProbeType.GetBuffer(strTwoProbeType.GetLength()));
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nIndexTwoProbeType = nIndexOneProbeType;

	CString strThreeProbeType = GetProfileString(_T("ˮ������̽��ϵͳ"), _T("ThreeProbeType"), _T("1"));
	int nIndexThreeProbeType = _wtoi(strThreeProbeType.GetBuffer(strThreeProbeType.GetLength()));
	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nIndexThreeProbeType = nIndexThreeProbeType;

	pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.SetMoveRule(nProbeNum, nIndexOneProbeType,
		nIndexTwoProbeType, nIndexThreeProbeType, pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode);	
	// Song Chenguang add end on 2012-07-26
	// ��ȡע���
	CString strFolder = GetProfileString(_T("ˮ������̽��ϵͳ"), _T("Data Folder"));
	
	if(strFolder == _T(""))// ��ֵ
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

		// д��ע���
		WriteProfileString(_T("ˮ������̽��ϵͳ"), _T("Data Folder"), pFrame->m_strDataFolder);
	}
	else
	{
		pFrame->m_strDataFolder = strFolder.Left(strFolder.GetLength()); // Song Chenguang modified on 2013-03-28
	}

	// ���Ŀ¼�Ƿ����
	DWORD   dwAttr   =   GetFileAttributes(pFrame->m_strDataFolder); 
	if(dwAttr == INVALID_FILE_ATTRIBUTES || (dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0 )//Ŀ¼������ 
	{ 
		CString strFolder = pFrame->m_strDataFolder;
		if (strFolder.Right(1) == _T("\\"))
		{
			strFolder = strFolder.Left(strFolder.GetLength() - 1);
		}
		if(!CreateDirectory(strFolder, NULL)) 
		{ 
			AfxMessageBox(_T("���ܴ���Ŀ¼ "));
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
			return FALSE; 
		}
	}
	
	// Song Chenguang add start on 2013-06-17
	TCHAR strExeFolder[MAX_PATH];
	GetWorkingFolder(strExeFolder, MAX_PATH);
	CString strPath = CString(strExeFolder) + _T("ɨ��������ʱ�ļ�λ��.txt");
	// Song Chenguang add start on 2013-06-17
	BOOL bRet3 = ::PathFileExists(strPath);
	if (bRet3)	// �������ϴ�����ʱ����
	{
		int nRet = MessageBox(NULL,_T("����֮ǰ��δ֪ԭ����ɵı������Ƿ�ָ��ϴ����ݣ�"), _T("��ʾ"), MB_YESNO | MB_ICONQUESTION);

		// �ָ���ָ��·��
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
			filenamemonth = day.GetMonth();	// mm�·�
			filenameyear = day.GetYear();	// yyyy
			filehour = day.GetHour();		// hh
			filemin = day.GetMinute();		// mm����
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
					
					// ��ȡ�ļ�����ʾ
					pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_filename = strPath2;
					pFrame->m_wndCScanParamDockBar.m_pCScanParamView->ReadFile();
					// machao modify start on 2013-08-14
//					CCScanView *pCScanView = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_pCScanView;
					CCScanView *pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2, 0);
					// machao modify end on 2013-08-14
					pCScanView->DrawPicture(&pCScanView->m_MemDC);
					
					AfxMessageBox(_T("������ϣ�"));

					pCScanView->bFreePictureMemoy = FALSE;
					// �ָ�·���ļ��͹켣�ļ�
					int nIndex = str.ReverseFind('.');
					CString strMdsPath = str.Left(nIndex) + _T(".apf");
					CMovePathView* pMoveView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0, 0);
					AfxMessageBox(strMdsPath);
					pMoveView->m_GraphMove.OpenFileByPath(strMdsPath);

					CString strChannelPath = str.Left(nIndex) + _T(".asf");
					AfxMessageBox(strChannelPath);
					pFrame->m_wndChannelInfo.m_pFormView->ReadParam(strChannelPath);

					// ��ʾ���һ�������ݵ���е�������PTP��
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

					AfxMessageBox(_T("����������ʾ���"));
					// ��ȡ�켣�յ㣬��ʾ��ȡ���λ��
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

					// ��ע�������ʱ������
					UINT nLineNum = GetProfileInt(_T("ˮ������̽��ϵͳ"), _T("HaltLineNum"), 0);
					
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
	if (IDOK == AfxMessageBox(_T("�Ƿ�ԭ�ϴ����ã�"),MB_OKCANCEL | MB_ICONQUESTION))
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

		GetProfileString(_T("ˮ������̽��ϵͳ"), _T("apfFilePath"));
		CChannelPanel* pChannelPanel = pFrame->m_wndChannelInfo.m_pFormView;
		if (PathFileExists(GetProfileString(_T("ˮ������̽��ϵͳ"), _T("apfFilePath"))))
		{
			CMovePathView* mdsocx = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
			mdsocx->m_GraphMove.OpenFileByPath(GetProfileString(_T("ˮ������̽��ϵͳ"), _T("apfFilePath")));
		}

		if (PathFileExists(GetProfileString(_T("ˮ������̽��ϵͳ"), _T("asfFilePath"))))
		{
			pChannelPanel->ReadParam(GetProfileString(_T("ˮ������̽��ϵͳ"), _T("asfFilePath")));
			pChannelPanel->DoChangeChannel();
			pChannelPanel->UpdateData(FALSE);
		}
	}
	// machao add end on 2013-11-11
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::InitInstance"),"Leave");
	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CUltrasonicImmersedTestApp::OnAppAbout()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::OnAppAbout"),"Enter");

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::OnAppAbout"),"Leave");
}


// CUltrasonicImmersedTestApp ��Ϣ�������


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
	// �رռ��ܹ�
	ElepClose(m_ElepHandle);
#endif	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CUltrasonicImmersedTestApp::ExitInstance"),"Leave");
	return __super::ExitInstance();
}

/** 
* ��ȡAɨ����ͼָ��                                        
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
	// ��ȡ�汾��Ϣ
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	// songchenguang add start 2012-02-28
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMovePathView* pView = (CMovePathView*)pFrame->m_wndSplitter.GetPane(0,0);
	CString strVer;
	CString strMachine;
	strMachine.Format(_T("�豸�ͺ�: %s\r\n"), pApp->m_IniParams.m_info.wcMechineType);
	CString strSaleTime;
	strSaleTime.Format(_T("��������: %d��%d��%d��\r\n"), pApp->m_IniParams.m_info.SaleTime.GetYear(), 
		pApp->m_IniParams.m_info.SaleTime.GetMonth(), pApp->m_IniParams.m_info.SaleTime.GetDay());
	strVer = strVer + strMachine + strSaleTime;

	CString strVerOcx;
	strVerOcx.Format(_T("�˶��ؼ��汾: %f \r\n"), pView->m_GraphMove.Ver());

	strVer = strVer + strVerOcx;
	int nAECNum = pApp->GetCardNum();
	for(int i=0; i<nAECNum; i++)
	{
		CString strVerDown;
		strVerDown.Format(_T("��λ��Ӳ���汾: %.2f \r\n��λ������汾: %.2f \r\n"), pFrame->m_stYiQiVer[i].wHardWareVer, pFrame->m_stYiQiVer[i].wSoftWareVer);
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
* ���ÿ�����IP                                        
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
* ���㳤���������ֵ                                        
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
	CString sHelp = strFileName + _T("\\ARS����Cɨ��ϵͳ˵����.chm");
	if (PathFileExists(sHelp))
	{
		::ShellExecute(NULL,_T("open"),sHelp,NULL,NULL,SW_SHOW   );
		return;
	}
	else
	{
		AfxMessageBox(_T("ARS����Cɨ��ϵͳ˵����.chm������"),MB_OK);
	}
}
// HuangYaChang add end on 2012-11-24
