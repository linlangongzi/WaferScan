// OPCDataCallback.cpp: implementation of the COPCDataCallback class.
//
//////////////////////////////////////////////////////////////////////
#include <QMutex>

#include "SimpleFree.h"
#include "OPCDataCallback.h"
#include "resource.h"
#include "const.h"
#include "DataDefine.h"
#include "mainwindow.h"

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
		if (phClientItems[i] == 1)	// PLCStatus
		{
			g_ItemValue.PLCSelfResult = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 4)	// PLC2WSCommand
		{
			g_ItemValue.dwPLC2WSCommand = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 13)	//PlateTestResult
		{
			g_ItemValue.byPlateTestResult = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 
			ITEM_GENERAL_NUM+1) // 内探落下状态 
		{
			g_ItemValue.byA = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 
			ITEM_GENERAL_NUM+2)	// 
		{
			g_ItemValue.byB = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 
			ITEM_GENERAL_NUM+3)	// 
		{
			g_ItemValue.byC = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 
			ITEM_GENERAL_NUM+4)	// 
		{
			g_ItemValue.byD = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 
			ITEM_GENERAL_NUM+5)	// 
		{
			g_ItemValue.byE = pvValues[i].bVal;
		}
		else if (phClientItems[i] == 
			ITEM_GENERAL_NUM+6)	// 
		{
			g_ItemValue.byF = pvValues[i].bVal;
		}
		else if(phClientItems[i] == 11)	// RefBodyStart
		{
			g_ItemValue.fRefBody_Start = pvValues[i].lVal/10.0f;
		}
		else if(phClientItems[i] == 12)	// FreeBodyStart
		{
			g_ItemValue.fFreeBody_Start = pvValues[i].lVal/10.0f;
		}
		else if (phClientItems[i] == 
			72)	// 
		{
			g_ItemValue.dwLiLun_NT1 = pvValues[i].ulVal;
		}
		else if (phClientItems[i] == 
			73)	// 
		{
			g_ItemValue.dwLiLun_NT2 = pvValues[i].ulVal;
		}
/*		else if(phClientItems[i] == 74)	// RefBodyStart
		{
			g_ItemValue.wLiLun_NT2_A = pvValues[i].uiVal;
		}
		else if(phClientItems[i] == 75)	// FreeBodyStart
		{
			g_ItemValue.wLiLun_NT2_B = pvValues[i].uiVal;
		}
*/		// Song Chenguang add start on 2010-04-01
		else if (phClientItems[i] == 10)	// TestMode
		{
			g_ItemValue.ulTestMode = pvValues[i].ulVal;
		}
		// Song Chenguang add end on 2010-04-01

	}
	g_ItemCS.unlock();
/*
	DWORD ret = ::WaitForSingleObject(hOPCInitEvent,0);
	
	if (ret == WAIT_OBJECT_0)
	{
		// 标定结果
		for(i=0; i<dwCount; i++)
		{
			if (phClientItems[i] == ITEM_GENERAL_NUM + ITEM_NEIBOX_STATUS_NUM +1)	// BianTan1Status
			{
				g_ItemValue.wBiantan1Status = pvValues[i].uiVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ ITEM_NEIBOX_STATUS_NUM+2)	// BianTan2Status
			{
				g_ItemValue.wBiantan2Status = pvValues[i].uiVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+1)	// Neitan1box1
			{
				g_ItemValue.byNeitan1Box1Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+2)	// 
			{
				g_ItemValue.byNeitan1Box2Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+3)	// 
			{
				g_ItemValue.byNeitan1Box3Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+4)	// 
			{
				g_ItemValue.byNeitan1Box4Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+5)	// 
			{
				g_ItemValue.byNeitan1Box5Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+6)	// 
			{
				g_ItemValue.byNeitan1Box6Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+7)	// 
			{
				g_ItemValue.byNeitan1Box7Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+8)	// 
			{
				g_ItemValue.byNeitan1Box8Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+9)	// 
			{
				g_ItemValue.byNeitan1Box9Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+10)	// 
			{
				g_ItemValue.byNeitan1Box10Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+11)	// 
			{
				g_ItemValue.byNeitan1Box11Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+12)	// 
			{
				g_ItemValue.byNeitan1Box12Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+1)	// Neitan1box2
			{
				g_ItemValue.byNeitan2Box1Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+2)	// 
			{
				g_ItemValue.byNeitan2Box2Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+3)	// 
			{
				g_ItemValue.byNeitan2Box3Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+4)	// 
			{
				g_ItemValue.byNeitan2Box4Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+5)	// 
			{
				g_ItemValue.byNeitan2Box5Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+6)	// 
			{
				g_ItemValue.byNeitan2Box6Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+7)	// 
			{
				g_ItemValue.byNeitan2Box7Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+8)	// 
			{
				g_ItemValue.byNeitan2Box8Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+9)	// 
			{
				g_ItemValue.byNeitan2Box9Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+10)	// 
			{
				g_ItemValue.byNeitan2Box10Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+11)	// 
			{
				g_ItemValue.byNeitan2Box11Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+12)	// 
			{
				g_ItemValue.byNeitan2Box12Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+1)	// Neitan1box3
			{
				g_ItemValue.byNeitan3Box1Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+2)	// 
			{
				g_ItemValue.byNeitan3Box2Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+3)	// 
			{
				g_ItemValue.byNeitan3Box3Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+4)	// 
			{
				g_ItemValue.byNeitan3Box4Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+5)	// 
			{
				g_ItemValue.byNeitan3Box5Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+6)	// 
			{
				g_ItemValue.byNeitan3Box6Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+7)	// 
			{
				g_ItemValue.byNeitan3Box7Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+8)	// 
			{
				g_ItemValue.byNeitan3Box8Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+9)	// 
			{
				g_ItemValue.byNeitan3Box9Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+10)	// 
			{
				g_ItemValue.byNeitan3Box10Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+11)	// 
			{
				g_ItemValue.byNeitan3Box11Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+12)	// 
			{
				g_ItemValue.byNeitan3Box12Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+1)	// Neitan4box1
			{
				g_ItemValue.byNeitan4Box1Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+2)	// 
			{
				g_ItemValue.byNeitan4Box2Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+3)	// 
			{
				g_ItemValue.byNeitan4Box3Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+4)	// 
			{
				g_ItemValue.byNeitan4Box4Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+5)	// 
			{
				g_ItemValue.byNeitan4Box5Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+6)	// 
			{
				g_ItemValue.byNeitan4Box6Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+7)	// 
			{
				g_ItemValue.byNeitan4Box7Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+8)	// 
			{
				g_ItemValue.byNeitan4Box8Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+9)	// 
			{
				g_ItemValue.byNeitan4Box9Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+10)	// 
			{
				g_ItemValue.byNeitan4Box10Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+11)	// 
			{
				g_ItemValue.byNeitan4Box11Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+12)	// 
			{
				g_ItemValue.byNeitan4Box12Status = pvValues[i].bVal;
			}			
		}
		ResetEvent(hOPCInitEvent);
	}
*/
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
//	g_ItemCS.Lock(1);
	DWORD dwWaitObjectIndex = WaitForSingleObject(hOPCReadEvent, 0);
	
	if (dwWaitObjectIndex != WAIT_OBJECT_0)
	{
		for (int i=0; i<dwCount; i++)
		{
			if (phClientItems[i] == 1)	// PLCStatus
			{
				g_ItemValue.PLCSelfResult = pvValues[i].bVal;
			}
			else if (phClientItems[i] == 4)	// PLC2WSCommand
			{
				g_ItemValue.dwPLC2WSCommand = pvValues[i].bVal;
			}
			else if (phClientItems[i] == 7)	// Length
			{
				g_ItemValue.ulPlateLength = pvValues[i].ulVal;
			}
			else if (phClientItems[i] == 8)	// Width
			{
				g_ItemValue.ulPlateWidth = pvValues[i].ulVal;
			}
			else if (phClientItems[i] == 9)	// Height
			{
				g_ItemValue.ulPlateHeight = pvValues[i].ulVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM + ITEM_NEIBOX_STATUS_NUM +1)	// BianTan1Status
			{
				g_ItemValue.wBiantan1Status = pvValues[i].uiVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ ITEM_NEIBOX_STATUS_NUM+2)	// BianTan2Status
			{
				g_ItemValue.wBiantan2Status = pvValues[i].uiVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+1)	// Neitan1box1
			{
				g_ItemValue.byNeitan1Box1Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+2)	// 
			{
				g_ItemValue.byNeitan1Box2Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+3)	// 
			{
				g_ItemValue.byNeitan1Box3Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+4)	// 
			{
				g_ItemValue.byNeitan1Box4Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+5)	// 
			{
				g_ItemValue.byNeitan1Box5Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+6)	// 
			{
				g_ItemValue.byNeitan1Box6Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+7)	// 
			{
				g_ItemValue.byNeitan1Box7Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+8)	// 
			{
				g_ItemValue.byNeitan1Box8Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+9)	// 
			{
				g_ItemValue.byNeitan1Box9Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+10)	// 
			{
				g_ItemValue.byNeitan1Box10Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+11)	// 
			{
				g_ItemValue.byNeitan1Box11Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+12)	// 
			{
				g_ItemValue.byNeitan1Box12Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+1)	// Neitan1box2
			{
				g_ItemValue.byNeitan2Box1Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+2)	// 
			{
				g_ItemValue.byNeitan2Box2Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+3)	// 
			{
				g_ItemValue.byNeitan2Box3Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+4)	// 
			{
				g_ItemValue.byNeitan2Box4Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+5)	// 
			{
				g_ItemValue.byNeitan2Box5Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+6)	// 
			{
				g_ItemValue.byNeitan2Box6Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+7)	// 
			{
				g_ItemValue.byNeitan2Box7Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+8)	// 
			{
				g_ItemValue.byNeitan2Box8Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+9)	// 
			{
				g_ItemValue.byNeitan2Box9Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+10)	// 
			{
				g_ItemValue.byNeitan2Box10Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+11)	// 
			{
				g_ItemValue.byNeitan2Box11Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+12)	// 
			{
				g_ItemValue.byNeitan2Box12Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+1)	// Neitan1box3
			{
				g_ItemValue.byNeitan3Box1Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+2)	// 
			{
				g_ItemValue.byNeitan3Box2Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+3)	// 
			{
				g_ItemValue.byNeitan3Box3Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+4)	// 
			{
				g_ItemValue.byNeitan3Box4Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+5)	// 
			{
				g_ItemValue.byNeitan3Box5Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+6)	// 
			{
				g_ItemValue.byNeitan3Box6Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+7)	// 
			{
				g_ItemValue.byNeitan3Box7Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+8)	// 
			{
				g_ItemValue.byNeitan3Box8Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+9)	// 
			{
				g_ItemValue.byNeitan3Box9Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+10)	// 
			{
				g_ItemValue.byNeitan3Box10Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+11)	// 
			{
				g_ItemValue.byNeitan3Box11Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+12)	// 
			{
				g_ItemValue.byNeitan3Box12Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+1)	// Neitan4box1
			{
				g_ItemValue.byNeitan4Box1Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+2)	// 
			{
				g_ItemValue.byNeitan4Box2Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+3)	// 
			{
				g_ItemValue.byNeitan4Box3Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+4)	// 
			{
				g_ItemValue.byNeitan4Box4Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+5)	// 
			{
				g_ItemValue.byNeitan4Box5Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+6)	// 
			{
				g_ItemValue.byNeitan4Box6Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+7)	// 
			{
				g_ItemValue.byNeitan4Box7Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+8)	// 
			{
				g_ItemValue.byNeitan4Box8Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+9)	// 
			{
				g_ItemValue.byNeitan4Box9Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+10)	// 
			{
				g_ItemValue.byNeitan4Box10Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+11)	// 
			{
				g_ItemValue.byNeitan4Box11Status = pvValues[i].bVal;
			}
			else if (phClientItems[i] == ITEM_GENERAL_NUM+ITEM_BIAN_RESULT_NUM+ITEM_NEI1_RESULT_NUM+ITEM_NEI2_RESULT_NUM+ITEM_NEI3_RESULT_NUM+ ITEM_NEIBOX_STATUS_NUM+12)	// 
			{
				g_ItemValue.byNeitan4Box12Status = pvValues[i].bVal;
			}		
		}

		SetEvent(hOPCReadEvent);
	}
	

//	g_ItemCS.Unlock();

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
