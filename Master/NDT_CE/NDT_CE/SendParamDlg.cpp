#include "StdAfx.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "SendParamDlg.h"
#include "ReceiveParamDlg.h"

const dlgMessageProc SendParamDlgMessages[] = {
	WM_CREATE, OnCreateSendParamDlg,
	WM_INITDIALOG, OnInitialSendParamDlg,
	WM_COMMAND, goCommandSendParamDlg,
	WM_NOTIFY, goNotifySendParamDlg,
	WM_HSCROLL, goHScrollSendParamDlg,
};

// extern
extern ARS200FPGA_CH_DATA_REG *chCurrent_data_reg;
extern ARS200FPGA_CH_CON_REG *chCurrent_con_reg;
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern WORD WaveSpeed;

static SEND_PARAM_DATA ad, *pad;

BOOL CALLBACK SendParamDlgProc(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	for(int i=0; i< dim(SendParamDlgMessages); i++)
	{
		if(msgCode == SendParamDlgMessages[i].uCode)
			return (*SendParamDlgMessages[i].functionName)(hWnd, msgCode, wParam, lParam);
	}
	return FALSE;
}

BOOL OnCreateSendParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BOOL OnInitialSendParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	// 数据初始化
	pad = (SEND_PARAM_DATA *)lParam;
        
	ad = *pad;

	// 控件初始化及赋值
	// 电压
	HWND hWndSldPressure = GetDlgItem(hWnd, IDC_SLIDER_PRESS_R);
	HWND hWndEdtPressure = GetDlgItem(hWnd, IDC_EDIT_PRESS_R);
	InitSldAndEit(hWndSldPressure, hWndEdtPressure, MAX_PRESSURE, MIN_PRESSURE, 4, ad.nPress);

	// 脉宽
	HWND hWndSldMaiKuan = GetDlgItem(hWnd, IDC_SLIDER_MAIKUAN_R);
	HWND hWndEdtMaiKuan = GetDlgItem(hWnd, IDC_EDIT_MAIKUAN_R);	
	InitSldAndEit(hWndSldMaiKuan, hWndEdtMaiKuan, MAX_MAIKUAN, MIN_MAIKUAN, 4, ad.nMaiKuan);

	// 重复频率
	HWND hWndSldReFreq = GetDlgItem(hWnd, IDC_SLIDER_R_PINLV_R);
	HWND hWndEdtReFreq = GetDlgItem(hWnd, IDC_EDIT_R_PINLV_R);
	InitSldAndEit(hWndSldReFreq, hWndEdtReFreq, MAX_RE_FREQ, MIN_RE_FREQ, 4, ad.nRefrainFrequency);

	// 修正
	HWND hWndSldAmend = GetDlgItem(hWnd, IDC_SLIDER_AMEND_R);
	HWND hWndEdtAmend = GetDlgItem(hWnd, IDC_EDIT_AMEND_R);
	InitSldAndEit(hWndSldAmend, hWndEdtAmend, MAX_AMEND, MIN_AMEND, 4, ad.nAmend);

	return FALSE;
}

BOOL goCommandSendParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	BOOL bSuccess = FALSE;	

	switch(LOWORD(wParam))
	{
	case IDOK:
		// 获取对话框参数
		ad.nPress = GetDlgItemInt(hWnd, IDC_EDIT_PRESS_R, &bSuccess, FALSE);
		ad.nMaiKuan = GetDlgItemInt(hWnd, IDC_EDIT_MAIKUAN_R, &bSuccess, FALSE);
		ad.nRefrainFrequency = GetDlgItemInt(hWnd, IDC_EDIT_R_PINLV_R, &bSuccess, FALSE);
		ad.nAmend = GetDlgItemInt(hWnd, IDC_EDIT_AMEND_R, &bSuccess, FALSE);

		*pad = ad;

		EndDialog(hWnd, 1);

		return TRUE;
	case IDCANCEL:
		EndDialog(hWnd, 2);

		return TRUE;
		break;	
	}

	return FALSE;
}

BOOL goNotifySendParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BOOL goHScrollSendParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	HWND hWndCtrl = (HWND)lParam;
	BOOL bSuccess = FALSE;

	// 设对应的Edit窗口的值
	HWND hEdit = NULL;
	if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_PRESS_R))	// 电压
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_PRESS_R);	
		TBNotifications(wParam, hWndCtrl, MIN_PRESSURE, MAX_PRESSURE, hEdit);

		ad.nPress = GetDlgItemInt(hWnd, IDC_EDIT_PRESS_R, &bSuccess, FALSE);
		
		WORD nPress = WORD(ad.nPress/400.0f*255.0f);
		send_mcp41010(nPress);		
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_MAIKUAN_R))	// 脉宽
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_MAIKUAN_R);	
		TBNotifications(wParam, hWndCtrl, MIN_MAIKUAN, MAX_MAIKUAN, hEdit);

		ad.nMaiKuan = GetDlgItemInt(hWnd, IDC_EDIT_MAIKUAN_R, &bSuccess, FALSE);
		// 脉宽
		chCurrent_con_reg->wMaiKuan = int(float(ad.nMaiKuan)/20.0 +0.5);
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_R_PINLV_R))	// 重复频率
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_R_PINLV_R);
		TBNotifications(wParam, hWndCtrl, MIN_RE_FREQ, MAX_RE_FREQ, hEdit);

		ad.nRefrainFrequency = GetDlgItemInt(hWnd, IDC_EDIT_R_PINLV_R, &bSuccess, FALSE);
		// 重复频率
		chCurrent_con_reg->wRepeatCounter = 20000 / ad.nRefrainFrequency;
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_AMEND_R))	// 修正
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_AMEND_R);
		TBNotifications(wParam, hWndCtrl, MIN_AMEND, MAX_AMEND, hEdit);

		ad.nAmend = GetDlgItemInt(hWnd, IDC_EDIT_AMEND_R, &bSuccess, FALSE);
		// 修正
		float fTemp = ad.nAmend + ReceiveParam_ad[0].fDelay;
		chCurrent_data_reg->wMonitorZeroCounter = WORD(Range2Dot(WaveSpeed, fTemp));
	}

	return TRUE;
}
