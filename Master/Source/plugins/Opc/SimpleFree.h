#pragma once

#include <qt_windows.h> // Windows.h 需要在所有 Windows #include 之前.
#include <QString>
//#include <istream>
#include "opcerrors.h"
#include "opcda.h"

#define LOCALE_ID  0x409 // Code 0x409 = ENGLISH  0x804	// Chinese

const WORD ITEM_NUM = 29;   // 修改前为20，北方重工新加了7个
const WORD PROBE_FLAG = 15;

class CSimpleFree
{
public:
    // Constructor
    CSimpleFree() ;

    // Destructor
    virtual ~CSimpleFree() ;

    // Create and start the thread.
    bool StartThread(DWORD WaitTime = 1000);

    // Stop the thread.
    void StopThread() ;

    // Current thread status
    bool IsThreadStarted() ;

    // Signal other thread to create the component.
    HRESULT CreateComponent(const CLSID& clsid,
                            const IID& iid,
                            IUnknown** ppI) ;

    // 获取syncIO指针
    HRESULT GetIOInterface(const IID& iid,
                            IUnknown** ppI);

    // 获取AsynIO2指针
    HRESULT GetIO2Interface(const IID& iid,
                            IUnknown** ppI);
// Pure virtual functions
protected:

    // Create the component on this thread.
    HRESULT CreateComponentOnThread(const CLSID& clsid,
                                            const IID& iid,
                                            IUnknown** ppI);// = 0 ;

    // Function for doing background processing
    void WorkerFunction();// = 0 ;

// Member variables
protected:
    // ID of thread
    DWORD m_ThreadId ;

    // Handle to thread
    HANDLE m_hThread ;

    // Event tells free thread to create a component.
    HANDLE m_hCreateComponentEvent ;

    // Event signals main thread to continue.
    HANDLE m_hComponentReadyEvent ;

    // Event used to stop thread
    HANDLE m_hStopThreadEvent ;

    // Pointer to the stream for marshaling the interface
    IStream* m_pIStream ;

    // HRESULT returned when creating component
    HRESULT m_hr ;

    // Pointer to CLSID to create
    const CLSID* m_pclsid ;

    // Pointer to IID of interface to get
    const IID* m_piid ;

    // Time to wait before calling WorkerFunction
    DWORD m_WaitTime ;
public:
    // OPC server
    IOPCServer *m_IOPCServer;
    IOPCItemMgt *m_IOPCItemMgt;
    IOPCSyncIO *m_IOPCSyncIO;
    OPCITEMDEF m_Items[ITEM_NUM];

    OPCITEMRESULT *m_pItemResult;
    OPCHANDLE m_GrpSrvHandle;
    HRESULT *m_pErrors;
    IOPCGroupStateMgt *m_IOPCGroupStateMgt;
    DWORD m_dwAdvise;

    MULTI_QI m_arrMultiQI [6];

    // 接口定义
    //IConnectionPointContainer* pCPC;
    IOPCAsyncIO2* m_IOPCAsyncIO2;
    IOPCDataCallback* pDCB;

    //IConnectionPoint* pCP;
    // 定义接收器接口
    IOPCDataCallback* myDCB;

    // Event tells free thread to create a component.
    HANDLE m_hAsynIO2ComponentEvent ;
//	HANDLE m_hbbComponentEvent ;
// Internal helper functions
private:
    // Thread procedure
    static DWORD WINAPI RealThreadProc(void* pv) ;

    // Member thread procedure
    BOOL ClassThreadProc() ;

    // Helper function to simplify CreateComponentOnThread
    void CreateComponentOnThread() ;

    // Wait for an event, but process window messages.
    BOOL WaitWithMessageLoop(HANDLE hEvent) ;

    // Song Chenguang add start on 2009-02-24
    // 初始化Item参数
    BOOL InitItemsValue();
    // Song Chenguang add end on 2009-02-24

    void GetIO2InterfaceOnThread();
    void GetIOInterfaceOnThread();
    QString GetOPCServerIP();
};
