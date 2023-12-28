

#include <QMessageBox>
#include <QString>
#include <QDir>
#include <QSettings>

#include "SimpleFree.h"
#include "OPCDataCallback.h"


CLSID clsOPCServer = {0x75D00BBB,0xDDA5,0x11D1,{0xB9,0x44,0x9E,0x61,0x4D,0x00,0x00,0x00}};
extern HANDLE hOPCInitEvent;
CComModule _Module;
extern __declspec(selectany) CAtlModule* _pAtlModule=&_Module;//yachang.huang add on 2013-08-20 ����·�CComObject<COPCDataCallback>::CreateInstance(&pCOPCDataCallback)����bug
///////////////////////////////////////////////////////////
//
// Constructor
//
CSimpleFree::CSimpleFree() 
{
	m_ThreadId = 0 ;	
	m_hThread = NULL ;
	m_hCreateComponentEvent = NULL ;
	m_hComponentReadyEvent = NULL ;
	m_hStopThreadEvent = NULL ;
	m_pIStream = NULL ;
	m_hr = S_OK ;
	m_piid = NULL ;
	m_pclsid = NULL ;
	m_WaitTime = 500 ;

	m_dwAdvise = 0;
	m_pItemResult = (OPCITEMRESULT*)CoTaskMemAlloc(sizeof(OPCITEMRESULT)*ITEM_NUM);
	m_pErrors = (HRESULT*)CoTaskMemAlloc(sizeof(HRESULT)*ITEM_NUM);
} 

///////////////////////////////////////////////////////////
//
// Destructor
//
CSimpleFree::~CSimpleFree()
{
	// The thread must be stopped before we are deleted
	// because the WorkerFunction is in the derived class.
//	assert(m_hThread == NULL) ;
//	delete []m_pItemResult;
//	delete []m_pErrors;
}

///////////////////////////////////////////////////////////
//
// StartThread
//   - Create and start the thread.
//
BOOL CSimpleFree::StartThread(DWORD WaitTime) 
{
	if (IsThreadStarted())
	{
		return FALSE ;
	}

	// Create the thread.
	m_hThread = ::CreateThread(NULL,             // Default security
		                 0,                // Default stack size
		                 RealThreadProc, 
		                 (void*)this,
		                 CREATE_SUSPENDED, // Create the thread suspended.
		                 &m_ThreadId) ;    // Get the Thread ID.
	QString str = QString("%1%2").arg("OPC-Free Thread ID = ").arg(m_ThreadId);

	if (m_hThread == NULL)
	{
//		trace("StartThread failed to create thread.", GetLastError()) ; 
		return FALSE ;
	}
//	trace("StartThread successfully created thread.") ;

	// Create an event to signal the thread to create the component. 
	m_hCreateComponentEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL) ;
	if (m_hCreateComponentEvent == NULL)
	{
		return FALSE ;
	}

	// Create an event for the thread to signal when it is finished. 
	m_hComponentReadyEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL) ;
	if (m_hComponentReadyEvent == NULL)
	{
		return FALSE ;
	}

	// Create an event to tell the free thread to stop.
	m_hStopThreadEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL) ;
	if (m_hStopThreadEvent == NULL)
	{
		return FALSE ;
	}

	// Song Chenguang add start on 2009-02-25
	m_hAsynIO2ComponentEvent= ::CreateEvent(NULL, FALSE, FALSE, NULL) ;
	if (m_hAsynIO2ComponentEvent == NULL)
	{
		return FALSE ;
	}
/*
	m_hbbComponentEvent= ::CreateEvent(NULL, FALSE, FALSE, NULL) ;
	if (m_hbbComponentEvent == NULL)
	{
		return FALSE ;
	}
*/
	// Song Chenguang add end on 2009-02-25


//	trace("StartThread successfully created the events.") ;

	// Initialize the wait time.
	m_WaitTime = WaitTime ;

	// Thread was created suspended; start the thread.
	DWORD r = ::ResumeThread(m_hThread) ;
//	assert(r != 0xffffffff) ;

	// Wait for the thread to start up before we continue.
	WaitWithMessageLoop(m_hComponentReadyEvent) ;

	// Make sure that thread is still running and that
	// there isn't an error.
	DWORD dwReturn = 0 ;
	BOOL b = ::GetExitCodeThread(m_hThread, &dwReturn) ;
//	assert(b) ;
	if (dwReturn != STILL_ACTIVE)
	{
//		trace("There was an error. Thread is not running.") ;
		m_hThread = NULL ;
		return FALSE ;
	}

	return TRUE ;
}

///////////////////////////////////////////////////////////
//
// StopThread
//
void CSimpleFree::StopThread()
{
	if (m_hThread != NULL)
	{
		// Stop the thread.
		// PostThreadMessage(m_ThreadId, WM_QUIT, 0,0) ;
		

		// Song Chenguang add start on 2009-02-25
		HRESULT r1;
		OPCHANDLE *phServer;
		HRESULT *pErrors;
		// ɾ��Item...
		phServer = new OPCHANDLE[ITEM_NUM];
		for (int i=0; i<ITEM_NUM; i++)
		{
			phServer[i] = m_pItemResult[i].hServer;
		}
		
		r1 = m_IOPCItemMgt->RemoveItems(ITEM_NUM, // [in] ɾ��1 ��item
			phServer, // [in] ���������
			&pErrors);// [out] ���������صĴ�����
		if ( (r1 != S_OK) && (r1 != S_FALSE) )
		{
//			AfxMessageBox("RemoveItems ʧ��!");
		}
		else if(r1==S_OK)
		{
			QMessageBox::information(NULL, "Error", QObject::tr("RemoveItems()�ɹ�"), QMessageBox::Yes, QMessageBox::Yes);
		}
		delete[] phServer;
		CoTaskMemFree(pErrors);
		CoTaskMemFree(m_pItemResult);
		m_pItemResult=NULL;
		CoTaskMemFree(m_pErrors);
		m_pErrors = NULL;

		// �ͷ�ͬ���ӿ�
		m_IOPCAsyncIO2->Release();
		m_IOPCAsyncIO2 = NULL;

		// �ͷ�item ����ӿ�
		m_IOPCItemMgt->Release();
		m_IOPCItemMgt = NULL;
		// ɾ��group ����
		r1=m_IOPCServer->RemoveGroup(m_GrpSrvHandle, TRUE);
		if (r1 != S_OK)
		{
//			AfxMessageBox("RemoveGroup ʧ��!");
		}
		else
		{
			QMessageBox::information(NULL, "Error", QObject::tr("RemoveGroup �ɹ�!"), QMessageBox::Yes, QMessageBox::Yes);
		}
		m_GrpSrvHandle = NULL;
		// �ͷ� OPC ������
		m_IOPCServer->Release();
		m_IOPCServer = NULL;
		//�ر�COM ��
//		CoUninitialize();
		// Song Chenguang add end on 2009-02-25
	
		SetEvent(m_hStopThreadEvent) ;
		// Wait for thread to stop.
		WaitWithMessageLoop(m_hComponentReadyEvent) ;

		m_hThread = NULL ;
	}
}
	
///////////////////////////////////////////////////////////
//
// Current thread status
//
BOOL CSimpleFree::IsThreadStarted()
{
	return (m_hThread != NULL) ;
}

///////////////////////////////////////////////////////////
//
// Thread procedure
//
DWORD WINAPI CSimpleFree::RealThreadProc(void* pv) 
{
	CSimpleFree* pFree = reinterpret_cast<CSimpleFree*>(pv) ;
	return pFree->ClassThreadProc() ;
}

///////////////////////////////////////////////////////////
//
// Thread procedure
//
BOOL CSimpleFree::ClassThreadProc()
{
	BOOL bReturn = FALSE ;

	// Check for the existence of CoInitializeEx.
	typedef HRESULT (__stdcall *FPCOMINITIALIZE)(void*, DWORD) ;
	FPCOMINITIALIZE pCoInitializeEx = 
		reinterpret_cast<FPCOMINITIALIZE>(
			::GetProcAddress(::GetModuleHandle(_T("ole32")),
			                 "CoInitializeEx")) ;
	if (pCoInitializeEx == NULL)
	{
//		trace("This program requires the free-thread support in DCOM.") ;
		SetEvent(m_hComponentReadyEvent) ;
		return FALSE ;
	}

	// Initialize the COM Library.
	HRESULT hr = pCoInitializeEx(0, COINIT_MULTITHREADED) ; //@
	if (SUCCEEDED(hr))
	{
		// Signal that we are starting.
		SetEvent(m_hComponentReadyEvent) ;

		// Set up array of events.
		HANDLE hEventArray[3] = { m_hCreateComponentEvent,
		                          m_hStopThreadEvent,
									m_hAsynIO2ComponentEvent} ;

		// Wait for the signal to create a component.
		BOOL bContinue = TRUE ;
		while (bContinue)
		{
			//@
			switch(::WaitForMultipleObjects(3,
			                                hEventArray,
			                                FALSE,
			                                m_WaitTime)) 
			{
			// Create the component.
			case WAIT_OBJECT_0:			
				CreateComponentOnThread() ;	
				break ;

			// Stop the thread.
			case (WAIT_OBJECT_0 +1):
				bContinue = FALSE ;
				bReturn = TRUE ;
				break ;

			case (WAIT_OBJECT_0 +2):
				GetIO2InterfaceOnThread();
				break;
/*
			case (WAIT_OBJECT_0 +3):
				GetIOInterfaceOnThread();
				break;
*/
			// Do background processing.
			case WAIT_TIMEOUT:
				WorkerFunction() ; 
				break ;

			default:
//				trace("Wait failed.", GetLastError()) ; 
				break;
			}
		}
		// Uninitialize the COM Library.
		CoUninitialize() ;
	}
	else
		
	{
		//ErrorMessage(hr);//delete by wqf 20130702
	}

	// Signal that we have finished.
	SetEvent(m_hComponentReadyEvent) ;
	return bReturn ;
}


///////////////////////////////////////////////////////////
//
// CreateComponent helper function
//
HRESULT CSimpleFree::CreateComponent(const CLSID& clsid,
                                     const IID& iid,
                                     IUnknown** ppI)
{
	// Initialize the shared data.
	m_pIStream = NULL ;
	m_piid = &iid ;
	m_pclsid = &clsid ;

	// Signal the thread to create a component.
	SetEvent(m_hCreateComponentEvent) ;

	// Wait for the component to be created.
//	trace("Wait for the component to be created.") ;
	if (WaitWithMessageLoop(m_hComponentReadyEvent))
	{
//		trace("The wait succeeded.") ;

		if (FAILED(m_hr))        // Did GetClassFactory fail?
		{
			return m_hr ;
		}

		if (m_pIStream == NULL)  // Did the marshaling fail?
		{
			return E_FAIL ;
		}

//		trace("Unmarshal the interface pointer.") ;
		// Unmarshal the interface.
		HRESULT hr = ::CoGetInterfaceAndReleaseStream(m_pIStream,
		                                              iid,
		                                              (void**)ppI) ;
		m_pIStream = NULL ;
		if (FAILED(hr))
		{
			//ErrorMessage(hr);//wqf delete on 20130702
//			trace("CoGetInterfaceAndReleaseStream failed.", hr) ; 
			return E_FAIL ;
		}
		return S_OK ;
	}
	return E_FAIL ;
}

///////////////////////////////////////////////////////////
//
// CreateComponentOnThread helper function
//   - This function packages the parameters for the
//     CoCreateComponentOnThread function.
//
void CSimpleFree::CreateComponentOnThread()
{
	IUnknown* pI = NULL;
	// Call the derived class to actually create the component.
	m_hr = CreateComponentOnThread(*m_pclsid, *m_piid, &pI) ;
	if (SUCCEEDED(m_hr))
	{
//      trace("Successfully created component.") ;
		// Marshal the interface pointer to the server.
		HRESULT hr = ::CoMarshalInterThreadInterfaceInStream(*m_piid,
		                                                     pI,
		                                                     &m_pIStream) ;
//		ASSERT(SUCCEEDED(hr));

		// Release the pI Pointer.
		pI->Release() ;
	}
	else
	{
//		trace("CreateComponentOnThread failed.", m_hr);
		CoUninitialize();
		ExitProcess(0);
	}

//	trace("Signal the main thread that the component is ready.") ;
	SetEvent(m_hComponentReadyEvent) ;

}
///////////////////////////////////////////////////////////
//
// BOOL WaitWithMessageLoop(HANDLE hEvent)
//

BOOL CSimpleFree::WaitWithMessageLoop(HANDLE hEvent)
{
	while (TRUE)
	{
		// Wait for the event and for messages.
		DWORD dwReturn = ::MsgWaitForMultipleObjects(1,
		                                             &hEvent,
		                                             FALSE,
		                                             INFINITE,
		                                             QS_ALLINPUT) ;
		if (dwReturn == WAIT_OBJECT_0)
		{
			// Our event happened.
			return TRUE ;
		}
		else if (dwReturn == WAIT_OBJECT_0 + 1)
		{
			// Handle message to keep client alive.
			MSG msg ;
			while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::DispatchMessage(&msg) ;
			}
		}
		else
		{
//			trace("WaitWithMessageLoop failed.", GetLastError()) ; 
			return FALSE ;
		}
	}
}

void CSimpleFree::WorkerFunction()
{
	Sleep(10);
}

HRESULT CSimpleFree::CreateComponentOnThread(const CLSID& clsid,
											const IID& iid,
	                                        IUnknown** ppI)
{
	LONG TimeBias = 0;
	FLOAT PercentDeadband = 0.0;
	DWORD RevisedUpdateRate;

	memset(m_arrMultiQI,0,sizeof(m_arrMultiQI));
	m_arrMultiQI [0].pIID = &IID_IOPCServer;
	m_arrMultiQI [1].pIID = &IID_IConnectionPointContainer;
	m_arrMultiQI [2].pIID = &IID_IOPCItemProperties;
	m_arrMultiQI [3].pIID =
		&IID_IOPCBrowseServerAddressSpace;
	m_arrMultiQI [4].pIID = &IID_IOPCServerPublicGroups;
	m_arrMultiQI [5].pIID = &IID_IPersistFile;
	COSERVERINFO tCoServerInfo;
	ZeroMemory (&tCoServerInfo, sizeof (tCoServerInfo));
	// �������ļ���ȡOPC������IP��ַ
	//QString m_strRemoteMachine = GetOPCServerIP(); 
	QString m_strRemoteMachine = "192.168.1.122";
	int nSize = m_strRemoteMachine.length() * sizeof (WCHAR);
// 	tCoServerInfo.pwszName = new WCHAR [nSize];
	tCoServerInfo.pwszName = (LPWSTR)m_strRemoteMachine.utf16();
// 	mbstowcs (tCoServerInfo.pwszName, (char*)m_strRemoteMachine,
// 		nSize);
 	//mbstowcs_s(tCoServerInfo.pwszName, charStr,nSize);

	HRESULT hr = CoCreateInstanceEx (
		clsid, // CLSID
		NULL, // No aggregation
		CLSCTX_REMOTE_SERVER, // connect to local, inproc and remote servers
		&tCoServerInfo,
		sizeof (m_arrMultiQI) / sizeof (MULTI_QI), // numberof IIDS to query
		m_arrMultiQI); // array of	IID pointers to query

	// COM requires us to free memory allocated for [out] and [in/out]
	// arguments (i.e. name string).
	//delete [] tCoServerInfo.pwszName;

	if (FAILED(hr))
	{
		//ErrorMessage(hr);//wqf delete on 20130702
		QMessageBox::information(NULL, "Error", QObject::tr("����Զ��OPC ����������ʧ��,������������Ƿ�����!"), QMessageBox::Yes, QMessageBox::Yes);
		m_IOPCServer = NULL;
//		CoUninitialize();
		return hr;
	}
	if (SUCCEEDED (m_arrMultiQI [0].hr))
	{
		m_IOPCServer = (IOPCServer *)m_arrMultiQI [0].pItf;

		(*ppI) = m_IOPCServer;
		m_IOPCServer->AddRef();
	}

	//���һ��group ���󣬲���ѯIOPCItemMgt �ӿ�
	hr = m_IOPCServer->AddGroup(L"grp1",
								TRUE,
								500,
								1,
								&TimeBias,
								&PercentDeadband,
								LOCALE_ID,
								&m_GrpSrvHandle,
								&RevisedUpdateRate,
								IID_IOPCItemMgt,
								(LPUNKNOWN*)&m_IOPCItemMgt);
	if (hr == OPC_S_UNSUPPORTEDRATE)
	{
		QMessageBox::information(NULL, "Error", QObject::tr("�����ˢ��������ʵ�ʵ�ˢ�����ʲ�һ��"), QMessageBox::Yes, QMessageBox::Yes);
	}
	else if (FAILED(hr))
	{
		//ErrorMessage(hr);//delete by wqf 20130702
		QMessageBox::information(NULL, "Error", QObject::tr("����Ϊ���������group ����"), QMessageBox::Yes, QMessageBox::Yes);
		m_IOPCServer->Release();
		m_IOPCServer = NULL;
//		CoUninitialize();
		return hr;
	}

	
	// ΪAddItem ����item ��Ĳ���
	InitItemsValue();
	WCHAR* ErrorStr = new WCHAR[256];
	char str[256];

//	OPCITEMRESULT* tmpResults = &m_ItemResult[0];
//	HRESULT* tmpErrors = &m_pErrors[0];
	hr = m_IOPCItemMgt->AddItems(ITEM_NUM,
								m_Items,
								&m_pItemResult,
								&m_pErrors);
	if ( (hr != S_OK) && (hr != S_FALSE) )
	{
		//ErrorMessage(hr);//delete by wqf on 20130702
		QMessageBox::information(NULL, "Error", QObject::tr("AddItems ʧ��"), QMessageBox::Yes, QMessageBox::Yes);
		m_IOPCItemMgt->Release();
		m_IOPCItemMgt = NULL;
		m_GrpSrvHandle = NULL;
		m_IOPCServer->Release();
		m_IOPCServer = NULL;
//		CoUninitialize();
		return S_FALSE;
	}
	else if(hr==S_OK)
	{
//		AfxMessageBox("AddItems()�ɹ�");
//		AfxMessageBox("����OPC�������ɹ�!");
	}
	else
	{
		QMessageBox::information(NULL, "Error", QObject::tr("OPC Itemsδ��ȫ��"), QMessageBox::Yes, QMessageBox::Yes);
	}

	// Song Chenguang add start on 2009-05-17
	delete []ErrorStr;
	// Song Chenguang add end on 2009-05-17


	// ���Item �Ŀɶ�д��
	if (m_pItemResult[0].dwAccessRights != (OPC_READABLE + OPC_WRITEABLE))
	{
		QMessageBox::information(NULL, "Error", QObject::tr("Item ���ɶ���Ҳ����д,�������������"), QMessageBox::Yes, QMessageBox::Yes);
	}

	//��ѯgroup ������첽�ӿ�
	hr = m_IOPCItemMgt->QueryInterface(IID_IOPCAsyncIO2,
		(void**)&m_IOPCAsyncIO2);
	if (hr < 0)
	{
		QMessageBox::information(NULL, "Error", QObject::tr("IOPCAsyncIO2 û�з��֣�����Ĳ�ѯ!"), QMessageBox::Yes, QMessageBox::Yes);
		CoTaskMemFree(m_pItemResult);
		m_IOPCItemMgt->Release();
		m_IOPCItemMgt = NULL;
		m_GrpSrvHandle = NULL;
		m_IOPCServer->Release();
		m_IOPCServer = NULL;
//		CoUninitialize();
		return S_FALSE;
	}

	//���IOPCGroupStateMgt �ӿ�
	hr=m_IOPCItemMgt->QueryInterface(IID_IOPCGroupStateMgt,
		(void**) &m_IOPCGroupStateMgt);
	if (hr != S_OK)
	{
		QMessageBox::information(NULL, "Error", QObject::tr("IOPCGroupStateMgt �ӿ�û���ҵ�"), QMessageBox::Yes, QMessageBox::Yes);
		CoTaskMemFree(m_pItemResult);
		m_IOPCItemMgt->Release();
		m_IOPCItemMgt = 0;
		m_GrpSrvHandle = 0;
		m_IOPCServer->Release();
		m_IOPCServer = NULL;
//		CoUninitialize();
		return S_FALSE;
	}

	
//	OnCheckActivategroup();

	// �����첽�ص�
	CComObject<COPCDataCallback>* pCOPCDataCallback; // �ص������ָ��
	//ͨ��ATL ģ�崴���ص������ʵ��
	HRESULT hrTemp;
	hrTemp = CComObject<COPCDataCallback>::CreateInstance(&pCOPCDataCallback);
	// Song Chenguang add start on 2009-04-13
	// ��������ص���������ϵ
//	pCOPCDataCallback->InformAboutFree(this);
	// Song Chenguang add end on 2009-04-13


	// ��ѯIUnknown �ӿ�
	LPUNKNOWN pCbUnk;
	pCbUnk = pCOPCDataCallback->GetUnknown();
	// ����һ�������������ӵ���ͻ����������֮�������

	HRESULT hRes = AtlAdvise( m_IOPCGroupStateMgt, // [in] IUnknown Interface of the Connection Point
		pCbUnk, // [in]	IUnknown Interface of the Callback object
		IID_IOPCDataCallback,// [in] Connection	Point ID: The OPC Data Callback
		&m_dwAdvise // [out] Cookie	that that uniquely identifies the connection
		);
	//yachang.huang add on 2013-8-21 ��OPC�������û���/���뱣��һ��ʱ�Ż�ɹ�
	if (hRes != S_OK)
	{
		//ErrorMessage(hRes);//delete by wqf 20130702
		QMessageBox::information(NULL, "Error", QObject::tr("Advise ʧ��!"), QMessageBox::Yes, QMessageBox::Yes);
		CoTaskMemFree(m_pItemResult);
		m_IOPCItemMgt->Release();
		m_IOPCItemMgt = 0;
		m_GrpSrvHandle = 0;
		m_IOPCServer->Release();
		m_IOPCServer = NULL;
		CoUninitialize();
		// Song Chenguang add start on 2009-04-11
		ExitProcess(0);
		// Song Chenguang add end on 2009-04-11

		return S_FALSE; 
	}

/*
	pCOPCDataCallback->Release();
	m_IOPCItemMgt->Release();
	m_IOPCAsyncIO2->Release();
	m_IOPCServer->Release();
*/	//////////////////////////////////////////////////////////////////////////

	SetEvent(hOPCInitEvent);
	return hr ;
}

/** 
* ��ȡOPC��������ַ
* @param     :
* @return    void
* @since     1.00
*/
QString CSimpleFree::GetOPCServerIP()
{
	QString strIniFlieName = QDir::currentPath() += "/YiQiConfig.ini";

	bool ret =   QFile::exists(strIniFlieName);
	if (!ret)
	{
		QString str =QString("%1").arg(QObject::tr("û��YiQiConfig.ini�����ļ�!!!"));
		QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
		return -1;
	}
	QSettings *setting = new QSettings(strIniFlieName, QSettings::IniFormat);
	QString strOpcIP = setting->value("OPC-Server:","").toString();
	return strOpcIP;
}


/*
// ʵ���ҵ�
BOOL CSimpleFree::InitItemsValue()
{
	for (int i=0; i<ITEM_NUM; i++)
	{
		m_Items[i].szAccessPath = L"";
//		m_Items[i].szItemID = L"PLCSelfTestResult";
		m_Items[i].bActive = TRUE;
		m_Items[i].hClient = i+1;
		m_Items[i].dwBlobSize = 0;
		m_Items[i].pBlob = NULL;
		m_Items[i].vtRequestedDataType = 2;
	}

	LPWSTR szItems1[40] = {	L"PLCSelfTestResult",	// PLC�Լ���
							L"WSSelfTestResult",	// ����ϵͳ�Լ���
							L"SysSelfTestResult",	// ϵͳ�Լ���
							L"Start_End_Flag",		// ̽�˿�ʼ������־��ͬʱҲ�����б�������λ�ź�
							L"PLC2WSCommand",		// ����������������
							L"PlateID",				// �ְ�ID
							L"PlateLength",			// �ְ����---����
							L"PlateWidth",			// �ְ����---���
							L"PlateHeight",			// �ְ����---���
							L"TestMode",			// ̽��ģʽ
							L"RefBody_Start",		// �ο�����̽̽�����߾���
							L"RefBody_1BoxStartNo",	// �ο�����̽��һ����ʼ̽ͷ�����
							L"RefBody_1BoxEndNo",	// �ο�����̽��һ�����̽ͷ�����
							L"RefBody_2BoxStartNo",	// �ο�����̽�ڶ�����ʼ̽ͷ�����
							L"RefBody_2BoxEndNo",	// �ο�����̽�ڶ������̽ͷ�����
							L"FreeBody_Start",		// �ǲο�����̽̽�����߾���
							L"FreeBody_1BoxStartNo",	// �ǲο�����̽��һ����ʼ̽ͷ�����
							L"FreeBody_1BoxEndNo",	// �ǲο�����̽�ڶ������̽ͷ�����
							L"FreeBody_2BoxStartNo",	// �ǲο�����̽��һ����ʼ̽ͷ�����
							L"FreeBody_2BoxEndNo",	// �ǲο�����̽�ڶ������̽ͷ�����
							L"WSState",				// ����ϵͳ����״̬
							L"Coder_1_en",			// �ǲο���С��������ʹ���ź�
							L"Coder_2_en",			// �ⳤ������1ʹ���ź�
							L"Coder_3_en",			// �ⳤ������2ʹ���ź�
							L"Coder_4_en",			// �ⳤ������3ʹ���ź�
							L"Coder_5_en",			// �����ֱ�����ʹ���ź�
							L"TestState",			// �豸���״̬
							L"RefBody_1_BoxState",	// �ο��ߵ�һ����̽̽ͷ��״̬
							L"RefBody_2_BoxState",	// �ο��ߵڶ�����̽̽ͷ��״̬
							L"FreeBody_1_BoxState",	// �ǲο��ߵ�һ����̽̽ͷ��״̬
							L"FreeBody_2_BoxState",	// �ǲο��ߵڶ�����̽̽ͷ��״̬
							L"RefBody_1_State",		// �ο��ߵ�һ����̽��״̬
							L"RefBody_2_State",		// �ο��ߵڶ�����̽��״̬
							L"FreeBody_1_State",	// �ǲο��ߵ�һ����̽��״̬
							L"FreeBody_2_State",	// �ǲο��ߵڶ�����̽��״̬
							L"RefEdge_BoxState",	// �ο��߱�̽̽ͷ��״̬
							L"RefEdge_State",		// �ο��߱�̽״̬
							L"FreeEdge_BoxState",	// �ǲο��߱�̽̽ͷ��״̬
							L"FreeEdge_State",		// �ǲο��߱�̽״̬
							L"PlateTestResult"		// �ְ�̽�˽��

	};

	for (i=0; i<40; i++)
	{
		m_Items[i].szItemID = szItems1[i];
		if (i>5 && i<9)
		{
			m_Items[i].vtRequestedDataType = 4;
		}
	}
	//? ����ĵ�����ֵ

	
	return TRUE;
}
*/
// Song Chenguang add start on 2009-04-11
// ʵ�ʵ�
/*
BOOL CSimpleFree::InitItemsValue()
{
	DWORD startNum = 0;
	// General
	LPWSTR szItems1[ITEM_GENERAL_NUM]={	L"PLCState",			// PLC״̬
		L"WSState",				// ����״̬
		L"SysState",			// ϵͳ״̬
		L"PLC2WSCommand",		// ����������������
		L"WS2PLCCommand",		// ����������������
		L"PlateID",				// �ְ�ID
		L"PlateLength",			// �ְ����---����
		L"PlateWidth",			// �ְ����---���
		L"PlateHeight",			// �ְ����---���
		L"TestMode",			// ̽��ģʽ
		L"RefBody_Start",		// �ο�����̽̽�����߾���
		L"FreeBody_Start",		// �ǲο�����̽̽�����߾���
		L"PlateTestResult",		// �ְ�̽�˽��
		L"G",					// �ο��ߡ��ǲο�����̽����Ч��־
		L"H",					// ���������ο��ߡ��ǲο��ߣ�ȫ�ֿ�ʼ��Ч��־
	};

	for (int i=startNum; i<(startNum+ITEM_GENERAL_NUM); i++)
	{
		m_Items[i].szAccessPath = L"";
		m_Items[i].szItemID = szItems1[i];
		m_Items[i].bActive = TRUE;
		m_Items[i].hClient = startNum + i+1;
		m_Items[i].dwBlobSize = 0;
		m_Items[i].pBlob = NULL;

		if (i==5)
		{
			m_Items[i].vtRequestedDataType = VT_BSTR;	// PlateID	
		}
		else if (i>5 && i<12)	// Lenght, width, height, testmode, refbody_start, freebodystart
		{
			m_Items[i].vtRequestedDataType = VT_UI4;
		}
		else
			m_Items[i].vtRequestedDataType = VT_UI1;
	}

	startNum += ITEM_GENERAL_NUM;

	CString strName;
	WCHAR wszItemID[64];

	// ��̽������״̬
	LPWSTR strStatus[ITEM_NEIBOX_STATUS_NUM] = {L"A",L"B",L"C",L"D",L"E",L"F"};
	::ZeroMemory(wszItemID, sizeof(WCHAR)*64);
	for (i=0; i<ITEM_NEIBOX_STATUS_NUM; i++)
	{
		m_Items[startNum + i].szAccessPath = L"";	
//		int ret = MultiByteToWideChar(CP_ACP, 0, strName, strName.GetLength()+1,
//			wszItemID, sizeof(wszItemID)/sizeof(wszItemID[0]));
		m_Items[startNum + i].szItemID = strStatus[i];	//wszItemID;
		m_Items[startNum + i].bActive = TRUE;
		m_Items[startNum + i].hClient = startNum + i+1;
		m_Items[startNum + i].dwBlobSize = 0;
		m_Items[startNum + i].pBlob = NULL;
		m_Items[startNum + i].vtRequestedDataType = VT_UI1;
	}

	startNum += ITEM_NEIBOX_STATUS_NUM;

	
	// ��̽Result
	::ZeroMemory(wszItemID, sizeof(WCHAR)*64);
	for (i=0; i<ITEM_BIAN_RESULT_NUM; i++)
	{
		m_Items[startNum + i].szAccessPath = L"";
		strName.Format("ProbeStatusBian_%d", i);
		int ret = MultiByteToWideChar(CP_ACP, 0, strName, strName.GetLength()+1,
			wszItemID, sizeof(wszItemID)/sizeof(wszItemID[0]));
		m_Items[startNum + i].szItemID = wszItemID;
		m_Items[startNum + i].bActive = TRUE;
		m_Items[startNum + i].hClient = startNum + i+1;
		m_Items[startNum + i].dwBlobSize = 0;
		m_Items[startNum + i].pBlob = NULL;
		m_Items[startNum + i].vtRequestedDataType = VT_UI2;
	}

	startNum += ITEM_BIAN_RESULT_NUM;

	// Nei1Result
	 
	// Song Chenguang add start on 2009-05-16
	m_Items[startNum].szAccessPath = L"";	
	m_Items[startNum].szItemID = L"ProbeStatusNei1_0";
	m_Items[startNum].bActive = TRUE;
	m_Items[startNum].hClient = startNum +1;
	m_Items[startNum].dwBlobSize = 0;
	m_Items[startNum].pBlob = NULL;
		m_Items[startNum].vtRequestedDataType = VT_UI1;
	// Song Chenguang add end on 2009-05-16


	::ZeroMemory(wszItemID, sizeof(WCHAR)*64);
	for (i=1; i<ITEM_NEI1_RESULT_NUM; i++)
	{
		m_Items[startNum + i].szAccessPath = L"";
		strName.Format(_T("ProbeStatusNei1_%d"), i);
		// ansi -> unicode		
		int ret = MultiByteToWideChar(CP_ACP, 0, strName, strName.GetLength()+1,
			wszItemID, sizeof(wszItemID)/sizeof(wszItemID[0]));
		m_Items[startNum + i].szItemID = wszItemID;
		m_Items[startNum + i].bActive = TRUE;
		m_Items[startNum + i].hClient = startNum + i+1;
		m_Items[startNum + i].dwBlobSize = 0;
		m_Items[startNum + i].pBlob = NULL;
		m_Items[startNum + i].vtRequestedDataType = VT_UI1;
	}
	
	startNum += ITEM_NEI1_RESULT_NUM;

	// Nei2Result
	::ZeroMemory(wszItemID, sizeof(WCHAR)*64);
	for (i=0; i<ITEM_NEI2_RESULT_NUM; i++)
	{
		m_Items[startNum + i].szAccessPath = L"";
		strName.Format(_T("ProbeStatusNei2_%d"), i);
		int ret = MultiByteToWideChar(CP_ACP, 0, strName, strName.GetLength()+1,
			wszItemID, sizeof(wszItemID)/sizeof(wszItemID[0]));
		m_Items[startNum + i].szItemID = wszItemID;
		m_Items[startNum + i].bActive = TRUE;
		m_Items[startNum + i].hClient = startNum + i+1;
		m_Items[startNum + i].dwBlobSize = 0;
		m_Items[startNum + i].pBlob = NULL;
		m_Items[startNum + i].vtRequestedDataType = VT_UI1;
	}

	startNum += ITEM_NEI2_RESULT_NUM;
	// Nei3Result
	::ZeroMemory(wszItemID, sizeof(WCHAR)*64);
	for (i=0; i<ITEM_NEI3_RESULT_NUM; i++)
	{
		m_Items[startNum + i].szAccessPath = L"";
		strName.Format(_T("ProbeStatusNei3_%d"), i);
		int ret = MultiByteToWideChar(CP_ACP, 0, strName, strName.GetLength()+1,
							  wszItemID, sizeof(wszItemID)/sizeof(wszItemID[0]));
		m_Items[startNum + i].szItemID = wszItemID;
		m_Items[startNum + i].bActive = TRUE;
		m_Items[startNum + i].hClient = startNum + i+1;
		m_Items[startNum + i].dwBlobSize = 0;
		m_Items[startNum + i].pBlob = NULL;
		m_Items[startNum + i].vtRequestedDataType = VT_UI1;
	}

	startNum += ITEM_NEI3_RESULT_NUM;
	// Nei4Result
	::ZeroMemory(wszItemID, sizeof(WCHAR)*64);
	for (i=0; i<ITEM_NEI4_RESULT_NUM; i++)
	{
		m_Items[startNum + i].szAccessPath = L"";
		strName.Format(_T("ProbeStatusNei4_%d"), i);
		int ret = MultiByteToWideChar(CP_ACP, 0, strName, strName.GetLength()+1,
			wszItemID, sizeof(wszItemID)/sizeof(wszItemID[0]));
		m_Items[startNum + i].szItemID = wszItemID;
		m_Items[startNum + i].bActive = TRUE;
		m_Items[startNum + i].hClient = startNum + i+1;
		m_Items[startNum + i].dwBlobSize = 0;
		m_Items[startNum + i].pBlob = NULL;
		m_Items[startNum + i].vtRequestedDataType = VT_UI1;
	}

	startNum += ITEM_NEI4_RESULT_NUM;
		
	//? ����ĵ�����ֵ
	
	return TRUE;
}
*/
// Song Chenguang add end on 2009-04-11

// Song Chenguang add start on 2009-05-16
// ʵ�ʵ�
BOOL CSimpleFree::InitItemsValue()
{
	for (int i = 0;i < ITEM_NUM;i++)
	{
		m_Items[i].szAccessPath = L"";
		m_Items[i].bActive = TRUE;
		m_Items[i].hClient = i+1;
		m_Items[i].dwBlobSize = 0;
		m_Items[i].pBlob = NULL;
	}

	m_Items[0].szItemID = L"PLCState";//�� ����״̬
	m_Items[0].vtRequestedDataType = VT_UI1;
	m_Items[1].szItemID = L"WSState";//�� ����״̬  ������û�ã�
	m_Items[1].vtRequestedDataType = VT_UI1;
	m_Items[2].szItemID = L"PipeLength";//д �ֹܳ���
	m_Items[2].vtRequestedDataType = VT_UI4;
	m_Items[3].szItemID = L"PipeDiameter";//д �ֹ�ֱ��
	m_Items[3].vtRequestedDataType = VT_UI4;
	m_Items[4].szItemID = L"PipeTestResult";//д ̽�˽��
	m_Items[4].vtRequestedDataType = VT_UI1;

	LPWSTR strProbeFlawFlag[25] = { 
		L"ProbeInFlawFlag1", L"ProbeInFlawFlag2",L"ProbeInFlawFlag3", L"ProbeInFlawFlag4",L"ProbeInFlawFlag5", 
		L"ProbeOutFlawFlag1",L"ProbeOutFlawFlag2", L"ProbeOutFlawFlag3",L"ProbeOutFlawFlag4", L"ProbeOutFlawFlag5",
		L"ProbeWallFlawFlag1", L"ProbeWallFlawFlag2",L"ProbeWallFlawFlag3", L"ProbeWallFlawFlag4",L"ProbeWallFlawFlag5" };//д��ͨ��̽�˽��

	for (int i = 5;i < ITEM_NUM;i++)
	{
		m_Items[i].szItemID = strProbeFlawFlag[i - 5];
		m_Items[i].vtRequestedDataType = VT_UI1;
	}

	return TRUE;
}



// ��ȡAsynIO2ָ��
HRESULT CSimpleFree::GetIO2Interface(const IID& iid,
	                        IUnknown** ppI)
{
	// Initialize the shared data.
	m_pIStream = NULL ;
	m_piid = &iid ;
//	m_pclsid = &clsid ;
	
	// Signal the thread to create a component.
	SetEvent(m_hAsynIO2ComponentEvent) ;
	
	// Wait for the component to be created.
	//	trace("Wait for the component to be created.") ;
	if (WaitWithMessageLoop(m_hComponentReadyEvent))
	{
		//		trace("The wait succeeded.") ;
		
		if (FAILED(m_hr))        // Did GetClassFactory fail?
		{
			return m_hr ;
		}
		
		if (m_pIStream == NULL)  // Did the marshaling fail?
		{
			return E_FAIL ;
		}
		
		//		trace("Unmarshal the interface pointer.") ;
		// Unmarshal the interface.
		HRESULT hr = ::CoGetInterfaceAndReleaseStream(m_pIStream,
			iid,
			(void**)ppI) ;
		m_pIStream = NULL ;
		if (FAILED(hr))
		{
			//			trace("CoGetInterfaceAndReleaseStream failed.", hr) ; 
			return E_FAIL ;
		}
		
		return S_OK ;
	}
	//	trace("What happened here?") ;
	return E_FAIL ;
	
}

void CSimpleFree::GetIO2InterfaceOnThread()
{
	IUnknown* pI = m_IOPCAsyncIO2;
	// Song Chenguang add start on 2009-02-27
	// ָ�븳ֵ��AddRef
	m_IOPCAsyncIO2->AddRef();
	// Song Chenguang add end on 2009-02-27

	// Call the derived class to actually create the component.
	// Marshal the interface pointer to the server.
	HRESULT hr = ::CoMarshalInterThreadInterfaceInStream(*m_piid,
		pI,
		&m_pIStream) ;
	//ASSERT(SUCCEEDED(hr));	
	// Release the pI Pointer.
	pI->Release() ;

	//	trace("Signal the main thread that the component is ready.") ;
	SetEvent(m_hComponentReadyEvent); 
}

/*
// ��ȡsyncIOָ��
HRESULT CSimpleFree::GetIOInterface(const IID& iid,
	                        IUnknown** ppI)
{
	// Initialize the shared data.
	m_pIStream = NULL ;
	m_piid = &iid ;
	//	m_pclsid = &clsid ;
	
	// Signal the thread to create a component.
//	SetEvent(m_hbbComponentEvent) ;
	
	// Wait for the component to be created.
	//	trace("Wait for the component to be created.") ;
	if (WaitWithMessageLoop(m_hComponentReadyEvent))
	{
		//		trace("The wait succeeded.") ;
		
		if (FAILED(m_hr))        // Did GetClassFactory fail?
		{
			return m_hr ;
		}
		
		if (m_pIStream == NULL)  // Did the marshaling fail?
		{
			return E_FAIL ;
		}
		
		//		trace("Unmarshal the interface pointer.") ;
		// Unmarshal the interface.
		HRESULT hr = ::CoGetInterfaceAndReleaseStream(m_pIStream,
			iid,
			(void**)ppI) ;
		m_pIStream = NULL ;
		if (FAILED(hr))
		{
			//			trace("CoGetInterfaceAndReleaseStream failed.", hr) ; 
			return E_FAIL ;
		}

		// Song Chenguang add start on 2009-02-27
//		(*ppI)->AddRef();
		// Song Chenguang add end on 2009-02-27


		return S_OK ;
	}
	//	trace("What happened here?") ;
	return E_FAIL ;
}

void CSimpleFree::GetIOInterfaceOnThread()
{
	IUnknown* pI = m_IOPCSyncIO;
	// Call the derived class to actually create the component.
	//	m_hr = CreateComponentOnThread(*m_pclsid, *m_piid, &pI) ;
	
	//	if (SUCCEEDED(m_hr))
	//	{
	//      trace("Successfully created component.") ;
	// Marshal the interface pointer to the server.
	HRESULT hr = ::CoMarshalInterThreadInterfaceInStream(*m_piid,
		pI,
		&m_pIStream) ;
	//		assert(SUCCEEDED(hr)) ;
	ASSERT(SUCCEEDED(hr));
	
	// Release the pI Pointer.
	pI->Release() ;
	//	}
	//	else
	//	{
	//		trace("CreateComponentOnThread failed.", m_hr);
	//	}
	
	//	trace("Signal the main thread that the component is ready.") ;
	SetEvent(m_hComponentReadyEvent);
}
*/
