#include "StdAfx.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include <pkfuncs.h>
#include <ceddk.h>
#include "ParamSetDlg.h"
#include "ParamSet2Dlg.h"

// extern
extern HINSTANCE hInst;
extern ARS200FPGA_CH_DATA_REG *ch1_data_reg;
extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
extern WORD *ch1_interval;
extern ARS200FPGA_CH_CON_REG *ch1_con_reg;

const dlgMessageProc ParamSetDlgMessages2[] = {
	WM_INITDIALOG, OnInitialParamSetDlg2,
	WM_COMMAND, goCommandParamSetDlg2,
	WM_HSCROLL, goHScrollParamSetDlg2,
};

static PARAM_SET_DATA2 ad, *pad;


BOOL CALLBACK ParamSetDlgProc2(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	for(int i=0; i< dim(ParamSetDlgMessages2); i++)
	{
		if(msgCode == ParamSetDlgMessages2[i].uCode)
			return (*ParamSetDlgMessages2[i].functionName)(hWnd, msgCode, wParam, lParam);
	}

	return FALSE;
}


BOOL OnInitialParamSetDlg2(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	// 数据初始化
	pad = (PARAM_SET_DATA2 *)lParam;
        
	ad = *pad;

	// 控件初始化及赋值
	HWND hWndSldMcp41010 = GetDlgItem(hWnd, IDC_SLIDER_MCP41010);
	HWND hWndEdtMcp41010 = GetDlgItem(hWnd, IDC_EDIT_MCP41010);

	HWND hWndSldMax500 = GetDlgItem(hWnd, IDC_SLIDER_MAX500);
	HWND hWndEdtMax500 = GetDlgItem(hWnd, IDC_EDIT_MAX500);

	WCHAR szString[10];
	memset(szString, 0, sizeof(szString));
	

	// Mcp41010
	SendMessage(hWndSldMcp41010,TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(0, 255));
	SendMessage(hWndSldMcp41010, TBM_SETPAGESIZE, 0, (LPARAM) 1);                  // new page size 

    SendMessage(hWndSldMcp41010, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG(0, 255)); 
	//...
	SendMessage(hWndSldMcp41010, TBM_SETPOS,  (WPARAM) TRUE, (LPARAM) ad.nMcp41010);

	swprintf(szString, _T("%d"), ad.nMcp41010);
	SendMessage(hWndEdtMcp41010, EM_REPLACESEL, 0, (LPARAM) szString);

	// Max500
	SendMessage(hWndSldMax500,TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(0, 255));
	SendMessage(hWndSldMax500, TBM_SETPAGESIZE, 0, (LPARAM) 1);                  // new page size 

    SendMessage(hWndSldMax500, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG(0, 255)); 
	//...
	SendMessage(hWndSldMax500, TBM_SETPOS,  (WPARAM) TRUE, (LPARAM) ad.nMax500_value1);

	memset(szString, 0, sizeof(szString));
	swprintf(szString, _T("%d"), ad.nMax500_value1);
	SendMessage(hWndEdtMax500, EM_REPLACESEL, 0, (LPARAM) szString);
	
	return FALSE;
}

BOOL goCommandParamSetDlg2(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	BOOL bSuccess = FALSE;
	WORD Mcp41010Value = GetDlgItemInt(hWnd, IDC_EDIT_MCP41010, &bSuccess, FALSE);
	WORD Max500Value1 = GetDlgItemInt(hWnd, IDC_EDIT_MAX500, &bSuccess, FALSE);
	WORD Max500Value2 = GetDlgItemInt(hWnd, IDC_EDIT_MAX500_2, &bSuccess, FALSE);
	WORD Max500Sum = (Max500Value2<<8) + Max500Value1;
	HWND hwndCtrl = NULL;
	DWORD wAddress = 0; //GetDlgItemInt(hWnd, IDC_EDIT_ADDRESS, &bSuccess, FALSE);
	WCHAR szAddress[10];
	memset(szAddress, 0, sizeof(szAddress));
	GetDlgItemText(hWnd, IDC_EDIT_ADDRESS, szAddress, 10);
//	swscanf(szAddress, _T("%x"), wAddress);
	wAddress = HexToInt(szAddress, wcslen(szAddress));

	WORD wValue = 0;
//	PHYSICAL_ADDRESS address = {wAddress,0};
	WORD *pValue = NULL;
	WCHAR szString[10];
	memset(szString, 0, sizeof(szString));
	WORD tmp = 0;
	
	switch(LOWORD(wParam))
	{
	case IDOK:
		// 获取对话框参数
		ad.nMcp41010 = GetDlgItemInt(hWnd, IDC_EDIT_MCP41010, &bSuccess, FALSE);
		ad.nMax500_value1 = Max500Value1;
		ad.nMax500_value2 = Max500Value2;
		
		*pad = ad;

	case IDCANCEL:
		EndDialog(hWnd, 0);

		return TRUE;
		break;	
	case IDC_BUTTON_MCP41010:	// MCP41010

		send_mcp41010(Mcp41010Value);

		return TRUE;
		break;
	case IDC_BUTTON_MAX500:	// MAX500

		send_max500(Max500Sum);
		return TRUE;
		break;
	case IDC_BUTTON_READ:	// Read

		swprintf(szString, _T("%x"), wAddress);
		MessageBox(NULL, szString, _T("Test"), MB_OK);

		pValue = (WORD*)wAddress;
		hwndCtrl = GetDlgItem( hWnd, IDC_EDIT_VALUE);
		memset(szString, 0, sizeof(szString));

		SetKMode(TRUE);
//		swprintf(szString, _T("%d"), *pValue);
		tmp = *pValue;
		SetKMode(FALSE);

		memset(szString, 0, sizeof(szString));
		swprintf(szString, _T("%d"), tmp);
		MessageBox(NULL, szString, _T("Value"), MB_OK);
		SendMessage(hwndCtrl, EM_REPLACESEL, 0, (LPARAM) szString);

		return TRUE;
		break;	
	case IDC_BUTTON_WRITE:	// Write
		
		pValue = (WORD*)wAddress;
		SetKMode(TRUE);
		*pValue = GetDlgItemInt(hWnd, IDC_EDIT_VALUE, &bSuccess, FALSE);
		SetKMode(FALSE);
		return TRUE;
		break;	
	}

	return FALSE;
}

BOOL goHScrollParamSetDlg2(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	HWND hWndCtrl = (HWND)lParam;

	// 设对应的Edit窗口的值
	HWND hEdit = NULL;
	if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_MCP41010))
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_MCP41010);	
		TBNotifications(wParam, hWndCtrl, 0, 255, hEdit);
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_MAX500))
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_MAX500);	
		TBNotifications(wParam, hWndCtrl, 0, 255, hEdit);
	}
	
	return TRUE;
}

unsigned long HexToInt(WCHAR* pch , int at)
{
	if( at <= 0 )
		return 0;
	WCHAR a = pch[--at];
	if(a>='0'&&a<='9')
	{
		a-='0';
	}
	else if(a>='a'&&a<='f')
	{
		a-='a';
		a+=0xa;
	}
	else if(a>='A'&&a<='F')
	{
		a-='A';
		a+=0xa;
	}
	else
	{
		return 0;
	}
	unsigned long b = HexToInt( pch , at );
//	return b==-1 ? -1 : (unsigned long(a) + 16 * b);
	return (unsigned long(a) + 16 * b);
} 