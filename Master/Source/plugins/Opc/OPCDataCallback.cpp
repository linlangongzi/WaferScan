// OPCDataCallback.cpp: implementation of the COPCDataCallback class.
//
//////////////////////////////////////////////////////////////////////
#include <QMutex>
#include <QDialog>

#include "SimpleFree.h"
#include "DataDefine.h"
#include "OPCDataCallback.h"

extern GlobalItemValue g_ItemValue;
extern QMutex g_ItemCS;
extern HANDLE hOPCInitEvent;
extern HANDLE hOPCReadEvent;
extern HANDLE hOPCWriteEvent;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COPCDataCallback::COPCDataCallback() = default;

COPCDataCallback::~COPCDataCallback() = default;

// IOPCDataCallback
STDMETHODIMP COPCDataCallback::OnDataChange(
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup,
    /* [in] */ HRESULT hrMasterquality,
    /* [in] */ HRESULT hrMastererror,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
    /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
    /* [size_is][in] */ WORD __RPC_FAR *pwQualities,
    /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
    /* [size_is][in] */ HRESULT __RPC_FAR *pErrors)
{
    Q_UNUSED(dwTransid);
    Q_UNUSED(hGroup);
    Q_UNUSED(hrMasterquality);
    Q_UNUSED(hrMastererror);
    Q_UNUSED(pwQualities);
    Q_UNUSED(pftTimeStamps);
    Q_UNUSED(pErrors);

    g_ItemCS.lock();
    for (DWORD i = 0; i < dwCount; i++) {
        if (phClientItems[i] == 1) {   // PLCState
            g_ItemValue.PLCState = pvValues[i].bVal;
        } else if (phClientItems[i] == 6) {     //
            g_ItemValue.SWTestMode = pvValues[i].ulVal;
        } else if (phClientItems[i] == 8) {
            g_ItemValue.KeepTesting = pvValues[i].bVal;
        } else if (phClientItems[i] == 10) {
            g_ItemValue.ReturnMark = pvValues[i].bVal;
        } else if (phClientItems[i] == 7) {
            g_ItemValue.ProbeBoxStatus = pvValues[i].ulVal;
        } else if (phClientItems[i] == 28) {
            g_ItemValue.TestStartFlag = pvValues[i].bVal;
        }
    }
    g_ItemCS.unlock();

    return S_OK;
}

STDMETHODIMP COPCDataCallback::OnReadComplete(
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup,
    /* [in] */ HRESULT hrMasterquality,
    /* [in] */ HRESULT hrMastererror,
    /* [in] */ DWORD dwCount,
    /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
    /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
    /* [size_is][in] */ WORD __RPC_FAR *pwQualities,
    /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
    /* [size_is][in] */ HRESULT __RPC_FAR *pErrors)
{
//	g_ItemCS.lock();	// rem by Song Chenguang  on 2013-12-03
    Q_UNUSED(dwTransid);
    Q_UNUSED(hGroup);
    Q_UNUSED(hrMasterquality);
    Q_UNUSED(hrMastererror);
    Q_UNUSED(pwQualities);
    Q_UNUSED(pftTimeStamps);
    Q_UNUSED(pErrors);

    DWORD dwWaitObjectIndex = WaitForSingleObject(hOPCReadEvent, 0);
    if (dwWaitObjectIndex != WAIT_OBJECT_0) {
        g_ItemCS.lock();
        for (DWORD i = 0; i < dwCount; i++) {
            if (phClientItems[i] == 1) {   // PLCState
                g_ItemValue.PLCState = pvValues[i].bVal;
            } else if (phClientItems[i] == 6) {     //
                g_ItemValue.SWTestMode = pvValues[i].ulVal;
            } else if (phClientItems[i] == 8) {
                g_ItemValue.KeepTesting = pvValues[i].bVal;
            } else if (phClientItems[i] == 10) {
                g_ItemValue.ReturnMark = pvValues[i].bVal;
            } else if (phClientItems[i] == 7) {
                g_ItemValue.ProbeBoxStatus = pvValues[i].ulVal;
            } else if (phClientItems[i] == 28) {
                g_ItemValue.TestStartFlag = pvValues[i].bVal;
            }
        }
        g_ItemCS.unlock();
        SetEvent(hOPCReadEvent);
    }

//	g_ItemCS.unlock();   // rem by Song Chenguang  on 2013-12-03

    return S_OK;
}
STDMETHODIMP COPCDataCallback::OnWriteComplete(
    /* [in] */ DWORD dwTransid,
    /* [in] */ OPCHANDLE hGroup,
    /* [in] */ HRESULT hrMastererr,
    /* [in] */ DWORD dwCount,
    /*[size_is][in]*/ OPCHANDLE __RPC_FAR *pClienthandles,
    /* [size_is][in] */ HRESULT __RPC_FAR *pErrors)
{
    Q_UNUSED(dwTransid);
    Q_UNUSED(hGroup);
    Q_UNUSED(hrMastererr);
    Q_UNUSED(dwCount);
    Q_UNUSED(pClienthandles);
    Q_UNUSED(pErrors);
    DWORD dwWaitObjectIndex = WaitForSingleObject(hOPCWriteEvent, 0);

    if (dwWaitObjectIndex != WAIT_OBJECT_0)
    {
        SetEvent(hOPCWriteEvent);
    }
    return S_OK;
}
