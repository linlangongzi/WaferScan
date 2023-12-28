// OPCDataCallback.cpp: implementation of the COPCDataCallback class.
//
//////////////////////////////////////////////////////////////////////
#include <QMutex>

#include "SimpleFree.h"
#include "OPCDataCallback.h"
//#include "resource.h"
#include "const.h"
#include <QtGui/QDialog>
#include "..\pci9054_dll\plx_lib_x.h"	// plx_dll.dll PCI¿¨²Ù×÷¶¯Ì¬Á´½Ó¿â
#include "..\pci9054_dll\p9054_lib.h"
#include "DataDefine.h"
//#include "mainwindow.h"

extern GlobalItemValue g_ItemValue;
extern QMutex g_ItemCS;
extern HANDLE hOPCInitEvent;
extern HANDLE hOPCReadEvent;
extern HANDLE hOPCWriteEvent;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COPCDataCallback::COPCDataCallback()
{
}

COPCDataCallback::~COPCDataCallback()
{

}

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
	g_ItemCS.lock();
	for (int i=0; i<dwCount; i++)
	{
		if (phClientItems[i] == 1)	// PLCState
		{
			g_ItemValue.PLCState = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 3)	// PipeLength
		{
			g_ItemValue.PipeLength = pvValues[i].ulVal;
		}
		else if (phClientItems[i] == 6)	// ProbeInFlawFlag1
		{
			g_ItemValue.ProbeInFlawFlag1 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 7)	// ProbeInFlawFlag2
		{
			g_ItemValue.ProbeInFlawFlag2 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 8)	// ProbeInFlawFlag3
		{
			g_ItemValue.ProbeInFlawFlag3 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 9)	// ProbeInFlawFlag4
		{
			g_ItemValue.ProbeInFlawFlag4 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 10)	// ProbeInFlawFlag5
		{
			g_ItemValue.ProbeInFlawFlag5 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 11)	// ProbeOutFlawFlag1
		{
			g_ItemValue.ProbeOutFlawFlag1 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 12)	// ProbeOutFlawFlag2
		{
			g_ItemValue.ProbeOutFlawFlag2 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 13)	// ProbeOutFlawFlag3
		{
			g_ItemValue.ProbeOutFlawFlag3 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 14)	// ProbeOutFlawFlag4
		{
			g_ItemValue.ProbeOutFlawFlag4 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 15)	// ProbeOutFlawFlag5
		{
			g_ItemValue.ProbeOutFlawFlag5 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 16)	// ProbeWallFlawFlag1
		{
			g_ItemValue.ProbeWallFlawFlag1 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 17)	// ProbeWallFlawFlag2
		{
			g_ItemValue.ProbeWallFlawFlag2 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 18)	// ProbeWallFlawFlag3
		{
			g_ItemValue.ProbeWallFlawFlag3 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 19)	// ProbeWallFlawFlag4
		{
			g_ItemValue.ProbeWallFlawFlag4 = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 20)	// ProbeWallFlawFlag5
		{
			g_ItemValue.ProbeWallFlawFlag5 = pvValues[i].bVal;
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
	DWORD dwWaitObjectIndex = WaitForSingleObject(hOPCReadEvent, 0);
	if (dwWaitObjectIndex != WAIT_OBJECT_0)
	{
		for (int i=0; i<dwCount; i++)
		{
			if (phClientItems[i] == 1)	// PLCState
			{
				g_ItemValue.PLCState = pvValues[i].bVal;
			}
			if (phClientItems[i] == 3)	// PipeLength
			{
				g_ItemValue.PipeLength = pvValues[i].ulVal;
			}
			if (phClientItems[i] == 6)	// ProbeInFlawFlag1
			{
				g_ItemValue.ProbeInFlawFlag1 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 7)	// ProbeInFlawFlag2
			{
				g_ItemValue.ProbeInFlawFlag2 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 8)	// ProbeInFlawFlag3
			{
				g_ItemValue.ProbeInFlawFlag3 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 9)	// ProbeInFlawFlag4
			{
				g_ItemValue.ProbeInFlawFlag4 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 10)	// ProbeInFlawFlag5
			{
				g_ItemValue.ProbeInFlawFlag5 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 11)	// ProbeOutFlawFlag1
			{
				g_ItemValue.ProbeOutFlawFlag1 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 12)	// ProbeOutFlawFlag2
			{
				g_ItemValue.ProbeOutFlawFlag2 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 13)	// ProbeOutFlawFlag3
			{
				g_ItemValue.ProbeOutFlawFlag3 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 14)	// ProbeOutFlawFlag4
			{
				g_ItemValue.ProbeOutFlawFlag4 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 15)	// ProbeOutFlawFlag5
			{
				g_ItemValue.ProbeOutFlawFlag5 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 16)	// ProbeWallFlawFlag1
			{
				g_ItemValue.ProbeWallFlawFlag1 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 17)	// ProbeWallFlawFlag2
			{
				g_ItemValue.ProbeWallFlawFlag2 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 18)	// ProbeWallFlawFlag3
			{
				g_ItemValue.ProbeWallFlawFlag3 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 19)	// ProbeWallFlawFlag4
			{
				g_ItemValue.ProbeWallFlawFlag4 = pvValues[i].bVal;
			}
			if (phClientItems[i] == 20)	// ProbeWallFlawFlag5
			{
				g_ItemValue.ProbeWallFlawFlag5 = pvValues[i].bVal;
			}
		}

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
	DWORD dwWaitObjectIndex = WaitForSingleObject(hOPCWriteEvent, 0);

	if (dwWaitObjectIndex != WAIT_OBJECT_0)
	{
		SetEvent(hOPCWriteEvent);
	}
	return S_OK;
}
