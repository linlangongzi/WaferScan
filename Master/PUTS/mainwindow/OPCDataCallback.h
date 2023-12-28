// OPCDataCallback.h: interface for the COPCDataCallback class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPCDATACALLBACK_H__A5EF1BE3_6EB2_4EB5_ADDA_14317A960675__INCLUDED_)
#define AFX_OPCDATACALLBACK_H__A5EF1BE3_6EB2_4EB5_ADDA_14317A960675__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "opcda.h"
#include "atlbase.h"
#include "atlcom.h"

class COPCDataCallback :public CComObjectRoot,public IOPCDataCallback 
{
public:
	COPCDataCallback();
	~COPCDataCallback();
	BEGIN_COM_MAP(COPCDataCallback)
		COM_INTERFACE_ENTRY(IOPCDataCallback)
	END_COM_MAP()

	// IOPCDataCallback
	STDMETHODIMP OnDataChange(
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup,
		/* [in] */ HRESULT hrMasterquality,
		/* [in] */ HRESULT hrMastererror,
		/* [in] */ DWORD dwCount,
		/* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
		/* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
		/* [size_is][in] */ WORD __RPC_FAR *pwQualities,
		/* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
		/* [size_is][in] */ HRESULT __RPC_FAR *pErrors);
	STDMETHODIMP OnReadComplete(
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup,
		/* [in] */ HRESULT hrMasterquality,
		/* [in] */ HRESULT hrMastererror,
		/* [in] */ DWORD dwCount,
		/* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
		/* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
		/* [size_is][in] */ WORD __RPC_FAR *pwQualities,
		/* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
		/* [size_is][in] */ HRESULT __RPC_FAR *pErrors);
	STDMETHODIMP OnWriteComplete(
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup,
		/* [in] */ HRESULT hrMastererr,
		/* [in] */ DWORD dwCount,
		/*[size_is][in]*/ OPCHANDLE __RPC_FAR *pClienthandles,
		/* [size_is][in] */ HRESULT __RPC_FAR *pErrors);
	STDMETHODIMP OnCancelComplete(
		/* [in] */ DWORD dwTransid,
		/* [in] */ OPCHANDLE hGroup)
	{
		return S_OK;
	};
	
};

#endif // !defined(AFX_OPCDATACALLBACK_H__A5EF1BE3_6EB2_4EB5_ADDA_14317A960675__INCLUDED_)
