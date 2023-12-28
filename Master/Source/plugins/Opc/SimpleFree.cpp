#include "SimpleFree.h"
#include "OPCDataCallback.h"
#include <QDir>
#include <QString>
#include <QSettings>
#include <QMessageBox>
#include <QLoggingCategory>
#include <QLibrary>

CLSID clsOPCServer = {0x75D00BBB,0xDDA5,0x11D1,{0xB9,0x44,0x9E,0x61,0x4D,0x00,0x00,0x00}};
extern HANDLE hOPCInitEvent;
CComModule _Module;
extern __declspec(selectany) CAtlModule* _pAtlModule=&_Module;//yachang.huang add on 2013-08-20 解决下方CComObject<COPCDataCallback>::CreateInstance(&pCOPCDataCallback)崩溃bug
///////////////////////////////////////////////////////////
//
// Constructor
//
CSimpleFree::CSimpleFree()
{
    m_ThreadId = 0;
    m_hThread = NULL;
    m_hCreateComponentEvent = NULL;
    m_hComponentReadyEvent = NULL;
    m_hStopThreadEvent = NULL;
    m_pIStream = NULL;
    m_hr = S_OK;
    m_piid = NULL;
    m_pclsid = NULL;
    m_WaitTime = 500;

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
//	assert(m_hThread == NULL);
//	delete []m_pItemResult;
//	delete []m_pErrors;
}

///////////////////////////////////////////////////////////
//
// StartThread
//   - Create and start the thread.
//
bool CSimpleFree::StartThread(DWORD WaitTime)
{
    if (IsThreadStarted()) {
        return false;
    }

    // Create the thread.
    m_hThread = ::CreateThread(NULL, 0, RealThreadProc, (void*)this, CREATE_SUSPENDED, &m_ThreadId);    // Get the Thread ID.
    QString str = QString("%1%2").arg("OPC-Free Thread ID = ").arg(m_ThreadId);

    if (m_hThread == NULL) {
//		trace("StartThread failed to create thread.", GetLastError());
        return false;
    }
//	trace("StartThread successfully created thread.");

    // Create an event to signal the thread to create the component.
    m_hCreateComponentEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if (m_hCreateComponentEvent == NULL) {
        return false;
    }

    // Create an event for the thread to signal when it is finished.
    m_hComponentReadyEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if (m_hComponentReadyEvent == NULL) {
        return false;
    }

    // Create an event to tell the free thread to stop.
    m_hStopThreadEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if (m_hStopThreadEvent == NULL) {
        return false;
    }

    // Song Chenguang add start on 2009-02-25
    m_hAsynIO2ComponentEvent= ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if (m_hAsynIO2ComponentEvent == NULL) {
        return false;
    }

//	trace("StartThread successfully created the events.");

    // Initialize the wait time.
    m_WaitTime = WaitTime;

    // Thread was created suspended; start the thread.
    ::ResumeThread(m_hThread);
//	assert(r != 0xffffffff);

    // Wait for the thread to start up before we continue.
    WaitWithMessageLoop(m_hComponentReadyEvent);

    // Make sure that thread is still running and that
    // there isn't an error.
    DWORD dwReturn = 0;
    ::GetExitCodeThread(m_hThread, &dwReturn);
//	assert(b);
    if (dwReturn != STILL_ACTIVE) {
//		trace("There was an error. Thread is not running.");
        m_hThread = NULL;
        return FALSE;
    }

    return TRUE;
}

///////////////////////////////////////////////////////////
//
// StopThread
//
void CSimpleFree::StopThread()
{
    if (m_hThread == NULL)
    {
        return;
    }
    // Stop the thread.
    // PostThreadMessage(m_ThreadId, WM_QUIT, 0,0);


    // Song Chenguang add start on 2009-02-25
    HRESULT r1;
    OPCHANDLE *phServer;
    HRESULT *pErrors;
    // 删除Item...
    phServer = new OPCHANDLE[ITEM_NUM];
    for (int i=0; i<ITEM_NUM; i++)
    {
        phServer[i] = m_pItemResult[i].hServer;
    }

    r1 = m_IOPCItemMgt->RemoveItems(ITEM_NUM, // [in] 删除1 个item
        phServer, // [in] 服务器句柄
        &pErrors);// [out] 服务器返回的错误码
    if ( (r1 != S_OK) && (r1 != S_FALSE) )
    {
//			AfxMessageBox("RemoveItems 失败!");
    }
    else if(r1==S_OK)
    {
        QMessageBox::information(NULL, "Error", QObject::tr("RemoveItems()成功"), QMessageBox::Yes, QMessageBox::Yes);
    }
    delete[] phServer;
    CoTaskMemFree(pErrors);
    CoTaskMemFree(m_pItemResult);
    m_pItemResult=NULL;
    CoTaskMemFree(m_pErrors);
    m_pErrors = NULL;

    // 释放同步接口
    m_IOPCAsyncIO2->Release();
    m_IOPCAsyncIO2 = NULL;

    // 释放item 管理接口
    m_IOPCItemMgt->Release();
    m_IOPCItemMgt = NULL;
    // 删除group 对象
    r1=m_IOPCServer->RemoveGroup(m_GrpSrvHandle, TRUE);
    if (r1 != S_OK)
    {
//			AfxMessageBox("RemoveGroup 失败!");
    }
    else
    {
        QMessageBox::information(NULL, "Error", QObject::tr("RemoveGroup 成功!"), QMessageBox::Yes, QMessageBox::Yes);
    }
    m_GrpSrvHandle = NULL;
    // 释放 OPC 服务器
    m_IOPCServer->Release();
    m_IOPCServer = NULL;
    //关闭COM 库
//		CoUninitialize();
    // Song Chenguang add end on 2009-02-25

    SetEvent(m_hStopThreadEvent);
    // Wait for thread to stop.
    WaitWithMessageLoop(m_hComponentReadyEvent);

    m_hThread = NULL;
}

///////////////////////////////////////////////////////////
//
// Current thread status
//
bool CSimpleFree::IsThreadStarted()
{
    return (m_hThread != NULL);
}

///////////////////////////////////////////////////////////
//
// Thread procedure
//
DWORD WINAPI CSimpleFree::RealThreadProc(void* pv)
{
    CSimpleFree* pFree = reinterpret_cast<CSimpleFree *>(pv);
    return pFree->ClassThreadProc();
}

///////////////////////////////////////////////////////////
//
// Thread procedure
//
BOOL CSimpleFree::ClassThreadProc()
{
    BOOL bReturn = FALSE;

    // Check for the existence of CoInitializeEx.
    typedef HRESULT (__stdcall *FPCOMINITIALIZE)(void*, DWORD);
//    FPCOMINITIALIZE pCoInitializeEx =;
//        reinterpret_cast<FPCOMINITIALIZE>(
//            ::GetProcAddress(::GetModuleHandle(_T("ole32")),
//                             "CoInitializeEx"));
    FPCOMINITIALIZE pCoInitializeEx = reinterpret_cast<FPCOMINITIALIZE>(QLibrary::resolve("ole32", "CoInitializeEx"));
    if (pCoInitializeEx == NULL) {
        qDebug() << "CoInitializeEx not available. This program requires the free-thread support in DCOM.";
        SetEvent(m_hComponentReadyEvent);
        return FALSE;
    }

    // Initialize the COM Library.
    HRESULT hr = pCoInitializeEx(0, COINIT_MULTITHREADED); //@
    if (SUCCEEDED(hr))
    {
        // Signal that we are starting.
        SetEvent(m_hComponentReadyEvent);

        // Set up array of events.
        HANDLE hEventArray[3] = { m_hCreateComponentEvent,
                                  m_hStopThreadEvent,
                                    m_hAsynIO2ComponentEvent};

        // Wait for the signal to create a component.
        BOOL bContinue = TRUE;
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
                CreateComponentOnThread();
                break;

            // Stop the thread.
            case (WAIT_OBJECT_0 +1):
                bContinue = FALSE;
                bReturn = TRUE;
                break;

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
                WorkerFunction();
                break;

            default:
//				trace("Wait failed.", GetLastError());
                break;
            }
        }
        // Uninitialize the COM Library.
        CoUninitialize();
    }
    else

    {
        //ErrorMessage(hr);//delete by wqf 20130702
    }

    // Signal that we have finished.
    SetEvent(m_hComponentReadyEvent);
    return bReturn;
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
    m_pIStream = NULL;
    m_piid = &iid;
    m_pclsid = &clsid;

    // Signal the thread to create a component.
    SetEvent(m_hCreateComponentEvent);

    // Wait for the component to be created.
//	trace("Wait for the component to be created.");
    if (WaitWithMessageLoop(m_hComponentReadyEvent))
    {
//		trace("The wait succeeded.");

        if (FAILED(m_hr))        // Did GetClassFactory fail?
        {
            return m_hr;
        }

        if (m_pIStream == NULL)  // Did the marshaling fail?
        {
            return E_FAIL;
        }

//		trace("Unmarshal the interface pointer.");
        // Unmarshal the interface.
        HRESULT hr = ::CoGetInterfaceAndReleaseStream(m_pIStream,
                                                      iid,
                                                      (void**)ppI);
        m_pIStream = NULL;
        if (FAILED(hr))
        {
            //ErrorMessage(hr);//wqf delete on 20130702
//			trace("CoGetInterfaceAndReleaseStream failed.", hr);
            return E_FAIL;
        }
        return S_OK;
    }
    return E_FAIL;
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
    m_hr = CreateComponentOnThread(*m_pclsid, *m_piid, &pI);
    if (SUCCEEDED(m_hr))
    {
//      trace("Successfully created component.");
        // Marshal the interface pointer to the server.
        ::CoMarshalInterThreadInterfaceInStream(*m_piid, pI, &m_pIStream);
//		ASSERT(SUCCEEDED(hr));

        // Release the pI Pointer.
        pI->Release();
    }
    else
    {
//		trace("CreateComponentOnThread failed.", m_hr);
        CoUninitialize();
        ExitProcess(0);
    }

//	trace("Signal the main thread that the component is ready.");
    SetEvent(m_hComponentReadyEvent);

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
                                                     QS_ALLINPUT);
        if (dwReturn == WAIT_OBJECT_0)
        {
            // Our event happened.
            return TRUE;
        }
        else if (dwReturn == WAIT_OBJECT_0 + 1)
        {
            // Handle message to keep client alive.
            MSG msg;
            while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                ::DispatchMessage(&msg);
            }
        }
        else
        {
//			trace("WaitWithMessageLoop failed.", GetLastError());
            return FALSE;
        }
    }
}

void CSimpleFree::WorkerFunction()
{
    Sleep(10);
}

HRESULT CSimpleFree::CreateComponentOnThread(const CLSID& clsid, const IID& iid, IUnknown** ppI)
{
    Q_UNUSED(iid);
    LONG TimeBias = 0;
    FLOAT PercentDeadband = 0.0;
    DWORD RevisedUpdateRate;

    memset(m_arrMultiQI,0,sizeof(m_arrMultiQI));
    m_arrMultiQI [0].pIID = &IID_IOPCServer;
    m_arrMultiQI [1].pIID = &IID_IConnectionPointContainer;
    m_arrMultiQI [2].pIID = &IID_IOPCItemProperties;
    m_arrMultiQI [3].pIID = &IID_IOPCBrowseServerAddressSpace;
    m_arrMultiQI [4].pIID = &IID_IOPCServerPublicGroups;
    m_arrMultiQI [5].pIID = &IID_IPersistFile;
    COSERVERINFO tCoServerInfo;
    ZeroMemory (&tCoServerInfo, sizeof (tCoServerInfo));

    // 读配置文件获取OPC服务器IP地址
    //QString m_strRemoteMachine = GetOPCServerIP();
    QString m_strRemoteMachine = "192.168.0.10";
    tCoServerInfo.pwszName = (LPWSTR)m_strRemoteMachine.utf16();

    HRESULT hr = CoCreateInstanceEx(clsid,                  // CLSID
                                    NULL,                   // No aggregation
                                    CLSCTX_REMOTE_SERVER,   // connect to local, inproc and remote servers
                                    &tCoServerInfo,
                                    sizeof(m_arrMultiQI) / sizeof (MULTI_QI), // numberof IIDS to query
                                    m_arrMultiQI);          // array of	IID pointers to query

    // COM requires us to free memory allocated for [out] and [in/out]
    // arguments (i.e. name string).
    //delete [] tCoServerInfo.pwszName;

    if (FAILED(hr)) {
        QMessageBox::information(NULL, QString(QObject::tr("错误")), QString(QObject::tr("创建远程OPC 服务器对象失败,请确认OPC通讯环境是否完备")), QMessageBox::Yes);
        m_IOPCServer = NULL;
        return hr;
    }

    if (SUCCEEDED (m_arrMultiQI [0].hr)) {
        m_IOPCServer = (IOPCServer *)m_arrMultiQI [0].pItf;
        (*ppI) = m_IOPCServer;
        m_IOPCServer->AddRef();
    }

    //添加一个group 对象，并查询IOPCItemMgt 接口
    hr = m_IOPCServer->AddGroup(L"wincc",
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
    if (hr == OPC_S_UNSUPPORTEDRATE) {
        QMessageBox::information(NULL, "Error", QObject::tr("请求的刷新速率与实际的刷新速率不一致"), QMessageBox::Yes, QMessageBox::Yes);
    } else if (FAILED(hr)) {
        QMessageBox::information(NULL, "Error", QObject::tr("不能为服务器添加group 对象"), QMessageBox::Yes, QMessageBox::Yes);
        m_IOPCServer->Release();
        m_IOPCServer = NULL;
        return hr;
    }

    // 为AddItem 定义item 表的参数
    InitItemsValue();
    hr = m_IOPCItemMgt->AddItems(ITEM_NUM,      // 调试的时候可以修改数量 ITEM_NUM
                                m_Items,
                                &m_pItemResult,
                                &m_pErrors);
    if ( (hr != S_OK) && (hr != S_FALSE) ) {
        QMessageBox::information(NULL, "Error", QObject::tr("AddItems 失败"), QMessageBox::Yes, QMessageBox::Yes);
        m_IOPCItemMgt->Release();
        m_IOPCItemMgt = NULL;
        m_GrpSrvHandle = NULL;
        m_IOPCServer->Release();
        m_IOPCServer = NULL;
        return S_FALSE;
    } else if(hr==S_OK) {

    } else {
        QMessageBox::information(NULL, "Error", QObject::tr("OPC Items未加全！"), QMessageBox::Yes, QMessageBox::Yes);
    }

    // 检测Item 的可读写性
    if (m_pItemResult[0].dwAccessRights != (OPC_READABLE + OPC_WRITEABLE)) {
        QMessageBox::information(NULL, "Error", QObject::tr("Item 不可读，也不可写,请检查服务器配置"), QMessageBox::Yes, QMessageBox::Yes);
    }

    //查询group 对象的异步接口
    hr = m_IOPCItemMgt->QueryInterface(IID_IOPCAsyncIO2, (void**)&m_IOPCAsyncIO2);
    if (hr < 0) {
        QMessageBox::information(NULL, "Error", QObject::tr("IOPCAsyncIO2 没有发现，错误的查询!"), QMessageBox::Yes, QMessageBox::Yes);
        CoTaskMemFree(m_pItemResult);
        m_IOPCItemMgt->Release();
        m_IOPCItemMgt = NULL;
        m_GrpSrvHandle = NULL;
        m_IOPCServer->Release();
        m_IOPCServer = NULL;
        return S_FALSE;
    }

    //获得IOPCGroupStateMgt 接口
    hr=m_IOPCItemMgt->QueryInterface(IID_IOPCGroupStateMgt, (void**) &m_IOPCGroupStateMgt);
    if (hr != S_OK) {
        QMessageBox::information(NULL, "Error", QObject::tr("IOPCGroupStateMgt 接口没有找到"), QMessageBox::Yes, QMessageBox::Yes);
        CoTaskMemFree(m_pItemResult);
        m_IOPCItemMgt->Release();
        m_IOPCItemMgt = 0;
        m_GrpSrvHandle = 0;
        m_IOPCServer->Release();
        m_IOPCServer = NULL;
        return S_FALSE;
    }

    // 建立异步回调
    CComObject<COPCDataCallback>* pCOPCDataCallback; // 回调对象的指针

    //通过ATL 模板创建回调对象的实例
    HRESULT hrTemp;
    hrTemp = CComObject<COPCDataCallback>::CreateInstance(&pCOPCDataCallback);

    // 查询IUnknown 接口
    LPUNKNOWN pCbUnk;
    pCbUnk = pCOPCDataCallback->GetUnknown();

    // 建立一个服务器的连接点与客户程序接收器之间的连接
    HRESULT hRes = AtlAdvise( m_IOPCGroupStateMgt, // [in] IUnknown Interface of the Connection Point
        pCbUnk, // [in]	IUnknown Interface of the Callback object
        IID_IOPCDataCallback,// [in] Connection	Point ID: The OPC Data Callback
        &m_dwAdvise // [out] Cookie	that that uniquely identifies the connection
        );
    //yachang.huang add on 2013-8-21 与OPC服务器用户名/密码保持一致时才会成功
    if (hRes != S_OK) {
        QMessageBox::information(NULL, "Error", QObject::tr("Advise 失败!"), QMessageBox::Yes, QMessageBox::Yes);
        CoTaskMemFree(m_pItemResult);
        m_IOPCItemMgt->Release();
        m_IOPCItemMgt = 0;
        m_GrpSrvHandle = 0;
        m_IOPCServer->Release();
        m_IOPCServer = NULL;
        CoUninitialize();
        ExitProcess(0);
        return S_FALSE;
    }

    SetEvent(hOPCInitEvent);
    return hr;
}

/**
* 获取OPC服务器地址
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
        QString str =QString("%1").arg(QObject::tr("没有YiQiConfig.ini配置文件!!!"));
        QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
        return -1;
    }
    QSettings *setting = new QSettings(strIniFlieName, QSettings::IniFormat);
    QString strOpcIP = setting->value("OPC-Server:","").toString();
    return strOpcIP;
}


/*
// 实验室的
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

    LPWSTR szItems1[40] = {	L"PLCSelfTestResult",	// PLC自检结果
                            L"WSSelfTestResult",	// 仪器系统自检结果
                            L"SysSelfTestResult",	// 系统自检结果
                            L"Start_End_Flag",		// 探伤开始结束标志，同时也是所有编码器复位信号
                            L"PLC2WSCommand",		// 电气到仪器的命令
                            L"PlateID",				// 钢板ID
                            L"PlateLength",			// 钢板参数---长度
                            L"PlateWidth",			// 钢板参数---宽度
                            L"PlateHeight",			// 钢板参数---厚度
                            L"TestMode",			// 探伤模式
                            L"RefBody_Start",		// 参考边内探探架离板边距离
                            L"RefBody_1BoxStartNo",	// 参考边内探第一组起始探头盒序号
                            L"RefBody_1BoxEndNo",	// 参考边内探第一组结束探头盒序号
                            L"RefBody_2BoxStartNo",	// 参考边内探第二组起始探头盒序号
                            L"RefBody_2BoxEndNo",	// 参考边内探第二组结束探头盒序号
                            L"FreeBody_Start",		// 非参考边内探探架离板边距离
                            L"FreeBody_1BoxStartNo",	// 非参考边内探第一组起始探头盒序号
                            L"FreeBody_1BoxEndNo",	// 非参考边内探第二组结束探头盒序号
                            L"FreeBody_2BoxStartNo",	// 非参考边内探第一组起始探头盒序号
                            L"FreeBody_2BoxEndNo",	// 非参考边内探第二组结束探头盒序号
                            L"WSState",				// 仪器系统工作状态
                            L"Coder_1_en",			// 非参考边小车编码器使能信号
                            L"Coder_2_en",			// 测长编码器1使能信号
                            L"Coder_3_en",			// 测长编码器2使能信号
                            L"Coder_4_en",			// 测长编码器3使能信号
                            L"Coder_5_en",			// 导向轮编码器使能信号
                            L"TestState",			// 设备检查状态
                            L"RefBody_1_BoxState",	// 参考边第一组内探探头盒状态
                            L"RefBody_2_BoxState",	// 参考边第二组内探探头盒状态
                            L"FreeBody_1_BoxState",	// 非参考边第一组内探探头盒状态
                            L"FreeBody_2_BoxState",	// 非参考边第二组内探探头盒状态
                            L"RefBody_1_State",		// 参考边第一组内探架状态
                            L"RefBody_2_State",		// 参考边第二组内探架状态
                            L"FreeBody_1_State",	// 非参考边第一组内探架状态
                            L"FreeBody_2_State",	// 非参考边第二组内探架状态
                            L"RefEdge_BoxState",	// 参考边边探探头盒状态
                            L"RefEdge_State",		// 参考边边探状态
                            L"FreeEdge_BoxState",	// 非参考边边探探头盒状态
                            L"FreeEdge_State",		// 非参考边边探状态
                            L"PlateTestResult"		// 钢板探伤结果

    };

    for (i=0; i<40; i++)
    {
        m_Items[i].szItemID = szItems1[i];
        if (i>5 && i<9)
        {
            m_Items[i].vtRequestedDataType = 4;
        }
    }
    //? 浮点的单独赋值


    return TRUE;
}
*/
// Song Chenguang add start on 2009-04-11
// 实际的
/*
BOOL CSimpleFree::InitItemsValue()
{
    DWORD startNum = 0;
    // General
    LPWSTR szItems1[ITEM_GENERAL_NUM]={	L"PLCState",			// PLC状态
        L"WSState",				// 仪器状态
        L"SysState",			// 系统状态
        L"PLC2WSCommand",		// 电气到仪器的命令
        L"WS2PLCCommand",		// 仪器到电气的命令
        L"PlateID",				// 钢板ID
        L"PlateLength",			// 钢板参数---长度
        L"PlateWidth",			// 钢板参数---宽度
        L"PlateHeight",			// 钢板参数---厚度
        L"TestMode",			// 探伤模式
        L"RefBody_Start",		// 参考边内探探架离板边距离
        L"FreeBody_Start",		// 非参考边内探探架离板边距离
        L"PlateTestResult",		// 钢板探伤结果
        L"G",					// 参考边、非参考边内探等有效标志
        L"H",					// 编码器、参考边、非参考边，全局开始有效标志
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

    // 内探盒下落状态
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


    // 边探Result
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

    //? 浮点的单独赋值

    return TRUE;
}
*/
// Song Chenguang add end on 2009-04-11

// Song Chenguang add start on 2009-05-16
// 实际的
BOOL CSimpleFree::InitItemsValue()
{
    for (int i = 0; i < ITEM_NUM; i++)
    {
        m_Items[i].szAccessPath = LPWSTR(L"");
        m_Items[i].bActive = TRUE;
        m_Items[i].hClient = i + 1;
        m_Items[i].dwBlobSize = 0;
        m_Items[i].pBlob = NULL;
    }

    m_Items[0].szItemID = LPWSTR(L"PLCState");          //读 电气状态
    m_Items[0].vtRequestedDataType = VT_UI1;
    m_Items[1].szItemID = LPWSTR(L"WSState");           //写 仪器状态
    m_Items[1].vtRequestedDataType = VT_UI1;
    m_Items[2].szItemID = LPWSTR(L"PipeLength");        //写 管棒长度
    m_Items[2].vtRequestedDataType = VT_UI4;
    m_Items[3].szItemID = LPWSTR(L"PipeDiameter");      //写 管棒直径
    m_Items[3].vtRequestedDataType = VT_UI4;
    m_Items[4].szItemID = LPWSTR(L"PipeTestResult");    //写 探伤结果
    m_Items[4].vtRequestedDataType = VT_UI1;
    m_Items[5].szItemID = LPWSTR(L"SWTestMode");        //读 探伤模式
    m_Items[5].vtRequestedDataType = VT_UI4;
    m_Items[6].szItemID = LPWSTR(L"ProbeBoxStatus");    //写 检测所需要下落的探头盒序列
    m_Items[6].vtRequestedDataType = VT_UI4;
    m_Items[7].szItemID = LPWSTR(L"KeepTesting");       //读 续扫标志
    m_Items[7].vtRequestedDataType = VT_UI1;
    m_Items[8].szItemID = LPWSTR(L"KTPosition");        //写 续扫位置坐标
    m_Items[8].vtRequestedDataType = VT_UI4;
    m_Items[9].szItemID = LPWSTR(L"ReturnMark");        //读 缺陷回位模式开关
    m_Items[9].vtRequestedDataType = VT_UI1;
    m_Items[10].szItemID = LPWSTR(L"ReturnBoxIndex");   //写 回位探头盒序号
    m_Items[10].vtRequestedDataType = VT_UI4;
    m_Items[11].szItemID = LPWSTR(L"ReturnPosition");   //写 回位位置-X方向坐标
    m_Items[11].vtRequestedDataType = VT_UI4;

    LPWSTR strProbeFlawFlag[PROBE_FLAG] = {
        LPWSTR(L"ProbeInFlawFlag1"), LPWSTR(L"ProbeInFlawFlag2"), LPWSTR(L"ProbeInFlawFlag3"), LPWSTR(L"ProbeInFlawFlag4"), LPWSTR(L"ProbeInFlawFlag5"),
        LPWSTR(L"ProbeOutFlawFlag1"), LPWSTR(L"ProbeOutFlawFlag2"), LPWSTR(L"ProbeOutFlawFlag3"), LPWSTR(L"ProbeOutFlawFlag4"), LPWSTR(L"ProbeOutFlawFlag5"),
        LPWSTR(L"ProbeWallFlawFlag1"), LPWSTR(L"ProbeWallFlawFlag2"), LPWSTR(L"ProbeWallFlawFlag3"), LPWSTR(L"ProbeWallFlawFlag4"), LPWSTR(L"ProbeWallFlawFlag5")};//写各通道探伤结果

    for (int i = 12;i < 12 + PROBE_FLAG;i++)
    {
        m_Items[i].szItemID = strProbeFlawFlag[i - 12];
        m_Items[i].vtRequestedDataType = VT_UI1;
    }

    m_Items[27].szItemID = LPWSTR(L"TestStartFlag");        //读 缺陷回位标志
    m_Items[27].vtRequestedDataType = VT_UI1;
    m_Items[28].szItemID = LPWSTR(L"PaintPosition");        //写 喷标位置
    m_Items[28].vtRequestedDataType = VT_UI4;

    return TRUE;
}



// 获取AsynIO2指针
HRESULT CSimpleFree::GetIO2Interface(const IID& iid,
                            IUnknown** ppI)
{
    // Initialize the shared data.
    m_pIStream = NULL;
    m_piid = &iid;
//	m_pclsid = &clsid;

    // Signal the thread to create a component.
    SetEvent(m_hAsynIO2ComponentEvent);

    // Wait for the component to be created.
    //	trace("Wait for the component to be created.");
    if (WaitWithMessageLoop(m_hComponentReadyEvent)) {
        //		trace("The wait succeeded.");

        if (FAILED(m_hr)) {       // Did GetClassFactory fail?
            return m_hr;
        }

        if (m_pIStream == NULL) { // Did the marshaling fail?
            return E_FAIL;
        }

        //		trace("Unmarshal the interface pointer.");
        // Unmarshal the interface.
        HRESULT hr = ::CoGetInterfaceAndReleaseStream(m_pIStream,
            iid,
            (void**)ppI);
        m_pIStream = NULL;
        if (FAILED(hr)) {
            //			trace("CoGetInterfaceAndReleaseStream failed.", hr);
            return E_FAIL;
        }

        return S_OK;
    }
    //	trace("What happened here?");
    return E_FAIL;
}

void CSimpleFree::GetIO2InterfaceOnThread()
{
    IUnknown* pI = m_IOPCAsyncIO2;
    // Song Chenguang add start on 2009-02-27
    // 指针赋值后AddRef
    m_IOPCAsyncIO2->AddRef();
    // Song Chenguang add end on 2009-02-27

    // Call the derived class to actually create the component.
    // Marshal the interface pointer to the server.
    ::CoMarshalInterThreadInterfaceInStream(*m_piid, pI, &m_pIStream);
    //ASSERT(SUCCEEDED(hr));
    // Release the pI Pointer.
    pI->Release();

    //	trace("Signal the main thread that the component is ready.");
    SetEvent(m_hComponentReadyEvent);
}

/*
// 获取syncIO指针
HRESULT CSimpleFree::GetIOInterface(const IID& iid,
                            IUnknown** ppI)
{
    // Initialize the shared data.
    m_pIStream = NULL;
    m_piid = &iid;
    //	m_pclsid = &clsid;

    // Signal the thread to create a component.
//	SetEvent(m_hbbComponentEvent);

    // Wait for the component to be created.
    //	trace("Wait for the component to be created.");
    if (WaitWithMessageLoop(m_hComponentReadyEvent))
    {
        //		trace("The wait succeeded.");

        if (FAILED(m_hr))        // Did GetClassFactory fail?
        {
            return m_hr;
        }

        if (m_pIStream == NULL)  // Did the marshaling fail?
        {
            return E_FAIL;
        }

        //		trace("Unmarshal the interface pointer.");
        // Unmarshal the interface.
        HRESULT hr = ::CoGetInterfaceAndReleaseStream(m_pIStream,
            iid,
            (void**)ppI);
        m_pIStream = NULL;
        if (FAILED(hr))
        {
            //			trace("CoGetInterfaceAndReleaseStream failed.", hr);
            return E_FAIL;
        }

        // Song Chenguang add start on 2009-02-27
//		(*ppI)->AddRef();
        // Song Chenguang add end on 2009-02-27


        return S_OK;
    }
    //	trace("What happened here?");
    return E_FAIL;
}

void CSimpleFree::GetIOInterfaceOnThread()
{
    IUnknown* pI = m_IOPCSyncIO;
    // Call the derived class to actually create the component.
    //	m_hr = CreateComponentOnThread(*m_pclsid, *m_piid, &pI);

    //	if (SUCCEEDED(m_hr))
    //	{
    //      trace("Successfully created component.");
    // Marshal the interface pointer to the server.
    HRESULT hr = ::CoMarshalInterThreadInterfaceInStream(*m_piid,
        pI,
        &m_pIStream);
    //		assert(SUCCEEDED(hr));
    ASSERT(SUCCEEDED(hr));

    // Release the pI Pointer.
    pI->Release();
    //	}
    //	else
    //	{
    //		trace("CreateComponentOnThread failed.", m_hr);
    //	}

    //	trace("Signal the main thread that the component is ready.");
    SetEvent(m_hComponentReadyEvent);
}
*/
