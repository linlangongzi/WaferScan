#include "StdAfx.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "ParamSetDlg.h"

// extern
extern HINSTANCE hInst;
extern ARS200FPGA_CH_DATA_REG *ch1_data_reg;
extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
extern WORD *ch1_interval;
extern ARS200FPGA_CH_CON_REG *ch1_con_reg;
extern ARS200FPGA_CH_PLUS_REG* ch1_plus_reg;

int MIN_DELAY = 0;
extern float MAX_DELAY;
extern WORD MAX_PLUS;

const dlgMessageProc ParamSetDlgMessages[] = {
	WM_CREATE, OnCreateParamSetDlg,
	WM_INITDIALOG, OnInitialParamSetDlg,
	WM_COMMAND, goCommandParamSetDlg,
	WM_NOTIFY, goNotifyParamSetDlg,
	WM_HSCROLL, goHScrollParamSetDlg,
};




static PARAM_SET_DATA ad, *pad;


BOOL CALLBACK ParamSetDlgProc(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	for(int i=0; i< dim(ParamSetDlgMessages); i++)
	{
		if(msgCode == ParamSetDlgMessages[i].uCode)
			return (*ParamSetDlgMessages[i].functionName)(hWnd, msgCode, wParam, lParam);
	}

	return FALSE;
}

BOOL OnCreateParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BOOL OnInitialParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	// 数据初始化
	pad = (PARAM_SET_DATA *)lParam;
        
	ad = *pad;

	// 控件初始化及赋值
	HWND hWndSldRange = GetDlgItem(hWnd, IDC_SLIDER_RANGE);
	HWND hWndEdtRange = GetDlgItem(hWnd, IDC_EDIT_RANGE);

	HWND hWndSldDelay = GetDlgItem(hWnd, IDC_SLIDER_DELAY);
	HWND hWndEdtDelay = GetDlgItem(hWnd, IDC_EDIT_DELAY);

	HWND hWndSldPlusA = GetDlgItem(hWnd, IDC_SLIDER_PLUS_A);
	HWND hWndEdtPlusA = GetDlgItem(hWnd, IDC_EDIT_PLUS_A);

	HWND hWndSldPlusB = GetDlgItem(hWnd, IDC_SLIDER_PLUS_B);
	HWND hWndEdtPlusB = GetDlgItem(hWnd, IDC_EDIT_PLUS_B);

	HWND hWndSldPlusC = GetDlgItem(hWnd, IDC_SLIDER_PLUS_C);
	HWND hWndEdtPlusC = GetDlgItem(hWnd, IDC_EDIT_PLUS_C);

	HWND hWndSldPlusD = GetDlgItem(hWnd, IDC_SLIDER_PLUS_D);
	HWND hWndEdtPlusD = GetDlgItem(hWnd, IDC_EDIT_PLUS_D);

	HWND hWndSldMaiKuan = GetDlgItem(hWnd, IDC_SLIDER_MAIKUAN);
	HWND hWndEdtMaiKuan = GetDlgItem(hWnd, IDC_EDIT_MAIKUAN);

	HWND hWndSldPressure = GetDlgItem(hWnd, IDC_SLIDER_PRESS);
	HWND hWndEdtPressure = GetDlgItem(hWnd, IDC_EDIT_PRESS);

	HWND hWndSldMidValue = GetDlgItem(hWnd, IDC_SLIDER_MID_VALUE);
	HWND hWndEdtMidValue = GetDlgItem(hWnd, IDC_EDIT_MID_VALUE);

	WCHAR szString[10];
	memset(szString, 0, sizeof(szString));
	

	// 设范围的上下限
	SendMessage(hWndSldRange,TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(MIN_RANGE, MAX_RANGE));
	SendMessage(hWndSldRange, TBM_SETPAGESIZE, 0, (LPARAM) 1);                  // new page size 

    SendMessage(hWndSldRange, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG(MIN_RANGE, MAX_RANGE)); 
	//...
	SendMessage(hWndSldRange, TBM_SETPOS,  (WPARAM) TRUE, (LPARAM) ad.nRange);

	swprintf(szString, _T("%d"), ad.nRange);
	SendMessage(hWndEdtRange, EM_REPLACESEL, 0, (LPARAM) szString);

	// Delay
	SendMessage(hWndSldDelay,TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(MIN_DELAY, MAX_DELAY));
	SendMessage(hWndSldDelay, TBM_SETPAGESIZE, 0, (LPARAM) 1);                  // new page size 

    SendMessage(hWndSldDelay, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG(MIN_DELAY, MAX_DELAY)); 
	//...
	SendMessage(hWndSldDelay, TBM_SETPOS,  (WPARAM) TRUE, (LPARAM) ad.nDelay);

	memset(szString, 0, sizeof(szString));
	swprintf(szString, _T("%d"), ad.nDelay);
	SendMessage(hWndEdtDelay, EM_REPLACESEL, 0, (LPARAM) szString);

	// 设增益的上下限
	SendMessage(hWndSldPlusA,TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(MIN_PLUS, MAX_PLUS));
	SendMessage(hWndSldPlusA, TBM_SETPAGESIZE, 0, (LPARAM) 4);                  // new page size 
    SendMessage(hWndSldPlusA, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG(MIN_PLUS, MAX_PLUS)); 
	//...
	SendMessage(hWndSldPlusA, TBM_SETPOS,  (WPARAM) TRUE, (LPARAM) ad.nPlusA);

	memset(szString, 0, sizeof(szString));
	swprintf(szString, _T("%d"), ad.nPlusA);
	SendMessage(hWndEdtPlusA, EM_REPLACESEL, 0, (LPARAM) szString);

	// B
	SendMessage(hWndSldPlusB,TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(MIN_PLUS, MAX_PLUS));
	SendMessage(hWndSldPlusB, TBM_SETPAGESIZE, 0, (LPARAM) 4);                  // new page size 
    SendMessage(hWndSldPlusB, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG(MIN_PLUS, MAX_PLUS)); 
	//...
	SendMessage(hWndSldPlusB, TBM_SETPOS,  (WPARAM) TRUE, (LPARAM) ad.nPlusB);

	memset(szString, 0, sizeof(szString));
	swprintf(szString, _T("%d"), ad.nPlusB);
	SendMessage(hWndEdtPlusB, EM_REPLACESEL, 0, (LPARAM) szString);

	// C
	SendMessage(hWndSldPlusC,TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(MIN_PLUS, MAX_PLUS));
	SendMessage(hWndSldPlusC, TBM_SETPAGESIZE, 0, (LPARAM) 4);                  // new page size 
    SendMessage(hWndSldPlusC, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG(MIN_PLUS, MAX_PLUS));
	//...
	SendMessage(hWndSldPlusC, TBM_SETPOS,  (WPARAM) TRUE, (LPARAM) ad.nPlusC);

	memset(szString, 0, sizeof(szString));
	swprintf(szString, _T("%d"), ad.nPlusC);
	SendMessage(hWndEdtPlusC, EM_REPLACESEL, 0, (LPARAM) szString);

	// D
	SendMessage(hWndSldPlusD,TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(MIN_PLUS, MAX_PLUS));
	SendMessage(hWndSldPlusD, TBM_SETPAGESIZE, 0, (LPARAM) 4);                  // new page size 
    SendMessage(hWndSldPlusD, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG(MIN_PLUS, MAX_PLUS));
	//...
	SendMessage(hWndSldPlusD, TBM_SETPOS,  (WPARAM) TRUE, (LPARAM) ad.nPlusD);

	memset(szString, 0, sizeof(szString));
	swprintf(szString, _T("%d"), ad.nPlusD);
	SendMessage(hWndEdtPlusD, EM_REPLACESEL, 0, (LPARAM) szString);

	// MaiKuan
	SendMessage(hWndSldMaiKuan,TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(MIN_MAIKUAN, MAX_MAIKUAN));
	SendMessage(hWndSldMaiKuan, TBM_SETPAGESIZE, 0, (LPARAM) 10);                  // new page size 
    SendMessage(hWndSldMaiKuan, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG(MIN_MAIKUAN, MAX_MAIKUAN));
	//...
	SendMessage(hWndSldMaiKuan, TBM_SETPOS,  (WPARAM) TRUE, (LPARAM) ad.nMaiKuan);

	memset(szString, 0, sizeof(szString));
	swprintf(szString, _T("%d"), ad.nMaiKuan);
	SendMessage(hWndEdtMaiKuan, EM_REPLACESEL, 0, (LPARAM) szString);

	// Pressure
	SendMessage(hWndSldPressure, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(MIN_PRESSURE, MAX_PRESSURE));
	SendMessage(hWndSldPressure, TBM_SETPAGESIZE, 0, (LPARAM) 4);                  // new page size 
    SendMessage(hWndSldPressure, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG(MIN_PRESSURE, MAX_PRESSURE));
	//...
	SendMessage(hWndSldPressure, TBM_SETPOS,  (WPARAM) TRUE, (LPARAM) ad.nPress);

	memset(szString, 0, sizeof(szString));
	swprintf(szString, _T("%d"), ad.nPress);
	SendMessage(hWndEdtPressure, EM_REPLACESEL, 0, (LPARAM) szString);	

	// Middle Value
	SendMessage(hWndSldMidValue, TBM_SETRANGE, (WPARAM) TRUE, (LPARAM) MAKELONG(MIN_MID_VALUE, MAX_MID_VALUE));
	SendMessage(hWndSldMidValue, TBM_SETPAGESIZE, 0, (LPARAM) 4);                  // new page size 
    SendMessage(hWndSldMidValue, TBM_SETSEL, (WPARAM) FALSE, (LPARAM) MAKELONG(MIN_MID_VALUE, MAX_MID_VALUE));
	//...
	SendMessage(hWndSldMidValue, TBM_SETPOS,  (WPARAM) TRUE, (LPARAM) ad.nMidValue);

	memset(szString, 0, sizeof(szString));
	swprintf(szString, _T("%d"), ad.nMidValue);
	SendMessage(hWndEdtMidValue, EM_REPLACESEL, 0, (LPARAM) szString);	

	return FALSE;
}

BOOL goCommandParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	BOOL bSuccess = FALSE;
	float fRange = MAX_RANGE;
	float fDelay = 0;
	WORD wInterval[DATA_SIZE];
	memset(wInterval, 0, sizeof(wInterval));
	WORD tmpGain = 0;
	WORD tmpMaiKuan = MIN_MAIKUAN/20;
	WORD tmpPresure = MIN_PRESSURE;

	switch(LOWORD(wParam))
	{
	case IDOK:
		// 获取对话框参数
		ad.nRange = GetDlgItemInt(hWnd, IDC_EDIT_RANGE, &bSuccess, FALSE);
		ad.nDelay = GetDlgItemInt(hWnd, IDC_EDIT_DELAY, &bSuccess, FALSE);
		ad.nPlusA = GetDlgItemInt(hWnd, IDC_EDIT_PLUS_A, &bSuccess, FALSE);
		ad.nPlusB = GetDlgItemInt(hWnd, IDC_EDIT_PLUS_B, &bSuccess, FALSE);
		ad.nPlusC = GetDlgItemInt(hWnd, IDC_EDIT_PLUS_C, &bSuccess, FALSE);
		ad.nPlusD = GetDlgItemInt(hWnd, IDC_EDIT_PLUS_D, &bSuccess, FALSE);
		ad.nMaiKuan = GetDlgItemInt(hWnd, IDC_EDIT_MAIKUAN, &bSuccess, FALSE);
		ad.nPress = GetDlgItemInt(hWnd, IDC_EDIT_PRESS, &bSuccess, FALSE);

		*pad = ad;

	case IDCANCEL:
		EndDialog(hWnd, 0);

		return TRUE;
		break;	
	case IDC_BUTTON_RANGE:	// 范围

		fRange = float(GetDlgItemInt(hWnd, IDC_EDIT_RANGE, &bSuccess, FALSE));
		RangeProcess(VSOUND, fRange, wInterval);

		memcpy(ch1_interval, wInterval, sizeof(wInterval));

		return TRUE;
		break;
	case IDC_BUTTON_DELAY:	// 延迟
		
		fDelay = float(GetDlgItemInt(hWnd, IDC_EDIT_DELAY, &bSuccess, FALSE));
		ch1_data_reg->wMonitorZeroCounter = Range2Dot(VSOUND, fDelay);
		return TRUE;
		break;
	case IDC_BUTTON_PLUS_A:	// 增益A

		tmpGain = GetDlgItemInt(hWnd, IDC_EDIT_PLUS_A, &bSuccess, FALSE);
	//	ch1_plus_reg->wdBRef = Gain2Data(tmpGain);	
		PlusChangeTurn(0);
	
		return TRUE;
		break;
	case IDC_BUTTON_PLUS_B: // 增益B

		tmpGain = GetDlgItemInt(hWnd, IDC_EDIT_PLUS_B, &bSuccess, FALSE);
	//	ch1_plus_reg->wdBLevel1 = Gain2Data(tmpGain);
		PlusChangeTurn(0);
		return TRUE;
		break;
	case IDC_BUTTON_PLUS_C:	// 增益C
		
		tmpGain = GetDlgItemInt(hWnd, IDC_EDIT_PLUS_C, &bSuccess, FALSE);
//		ch1_plus_reg->wdBLevel2 = Gain2Data(tmpGain);
		PlusChangeTurn(0);
		return TRUE;	
		break;
	case IDC_BUTTON_PLUS_D: // 增益D
		
		tmpGain = GetDlgItemInt(hWnd, IDC_EDIT_PLUS_D, &bSuccess, FALSE);
//		ch1_plus_reg->wdBLevel3 = Gain2Data(tmpGain);
		PlusChangeTurn(0);
		return TRUE;
		break;
	case IDC_BUTTON_MAIKUAN:

		tmpMaiKuan = GetDlgItemInt(hWnd, IDC_EDIT_MAIKUAN, &bSuccess, FALSE);
		ch1_con_reg->wMaiKuan = int(float(tmpMaiKuan)/20.0 +0.5);
		return TRUE;
		break;
	case IDC_BUTTON_PRESS:

		tmpPresure = GetDlgItemInt(hWnd, IDC_EDIT_PRESS, &bSuccess, FALSE);
		send_mcp41010(tmpPresure);
		return TRUE;
		break;
	case IDC_BUTTON_MID_VALUE:

		ch1_con_reg->wValueMiddle = GetDlgItemInt(hWnd, IDC_EDIT_MID_VALUE, &bSuccess, FALSE);
		return TRUE;
		break;
	
	}

	return FALSE;
}

BOOL goNotifyParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	LPNMHDR lpnmh = (LPNMHDR) lParam;
/*
	int a = lpnmh->code - NM_FIRST;
	WCHAR TMP[10];
	memset(TMP, 0 , sizeof(TMP));
	swprintf(TMP, _T("%d"), a);
	MessageBox(hWnd, TMP, TEXT("CHA"), MB_OK);
*/
/*	switch(LOWORD(wParam))
	{
	case IDC_SLIDER_RANGE:	// 范围
		if(lpnmh->code == NM_CLICK)
		{
			MessageBox(hWnd, TEXT("NM_CLICK"), TEXT("AA"), MB_OK);
		}
		else if(lpnmh->code == NM_DBLCLK)
		{
			MessageBox(hWnd, TEXT("NM_DBLCLK"), TEXT("AA"), MB_OK);
		}
		else if(lpnmh->code == NM_SETFOCUS)
		{
			MessageBox(hWnd, TEXT("NM_SETFOCOUS"), TEXT("AA"), MB_OK);
		}
		else if(lpnmh->code == NM_RELEASEDCAPTURE)
		{
			MessageBox(hWnd, TEXT("NM_RELEASEDCAPTURE"), TEXT("AA"), MB_OK);
		}
		else
			MessageBox(hWnd, TEXT("NM_OTHER"), TEXT("AA"), MB_OK);
		return FALSE;
		break;
	case IDC_SLIDER_PLUS:	// 增益
	
		return FALSE;
		break;
	}
*/
	return FALSE;
}

BOOL goHScrollParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	HWND hWndCtrl = (HWND)lParam;

	// 设对应的Edit窗口的值
	HWND hEdit = NULL;
	if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_RANGE))
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_RANGE);	
		TBNotifications(wParam, hWndCtrl, MIN_RANGE, MAX_RANGE, hEdit);
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_DELAY))
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_DELAY);	
		TBNotifications(wParam, hWndCtrl, MIN_DELAY, MAX_DELAY, hEdit);
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_PLUS_A))
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_PLUS_A);
		TBNotifications(wParam, hWndCtrl, MIN_PLUS, MAX_PLUS, hEdit);
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_PLUS_B))
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_PLUS_B);
		TBNotifications(wParam, hWndCtrl, MIN_PLUS, MAX_PLUS, hEdit);
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_PLUS_C))
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_PLUS_C);
		TBNotifications(wParam, hWndCtrl, MIN_PLUS, MAX_PLUS, hEdit);
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_PLUS_D))
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_PLUS_D);
		TBNotifications(wParam, hWndCtrl, MIN_PLUS, MAX_PLUS, hEdit);
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_MAIKUAN))
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_MAIKUAN);
		TBNotifications(wParam, hWndCtrl, MIN_MAIKUAN, MAX_MAIKUAN, hEdit);
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_PRESS))
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_PRESS);
		TBNotifications(wParam, hWndCtrl, MIN_PRESSURE, MAX_PRESSURE, hEdit);
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_MID_VALUE))
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_MID_VALUE);
		TBNotifications(wParam, hWndCtrl, MIN_MID_VALUE, MAX_MID_VALUE, hEdit);
	}

	return TRUE;
}

