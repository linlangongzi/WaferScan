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
	// ���ݳ�ʼ��
	pad = (SEND_PARAM_DATA *)lParam;
        
	ad = *pad;

	// �ؼ���ʼ������ֵ
	// ��ѹ
	HWND hWndSldPressure = GetDlgItem(hWnd, IDC_SLIDER_PRESS_R);
	HWND hWndEdtPressure = GetDlgItem(hWnd, IDC_EDIT_PRESS_R);
	InitSldAndEit(hWndSldPressure, hWndEdtPressure, MAX_PRESSURE, MIN_PRESSURE, 4, ad.nPress);

	// ����
	HWND hWndSldMaiKuan = GetDlgItem(hWnd, IDC_SLIDER_MAIKUAN_R);
	HWND hWndEdtMaiKuan = GetDlgItem(hWnd, IDC_EDIT_MAIKUAN_R);	
	InitSldAndEit(hWndSldMaiKuan, hWndEdtMaiKuan, MAX_MAIKUAN, MIN_MAIKUAN, 4, ad.nMaiKuan);

	// �ظ�Ƶ��
	HWND hWndSldReFreq = GetDlgItem(hWnd, IDC_SLIDER_R_PINLV_R);
	HWND hWndEdtReFreq = GetDlgItem(hWnd, IDC_EDIT_R_PINLV_R);
	InitSldAndEit(hWndSldReFreq, hWndEdtReFreq, MAX_RE_FREQ, MIN_RE_FREQ, 4, ad.nRefrainFrequency);

	// ����
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
		// ��ȡ�Ի������
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

	// ���Ӧ��Edit���ڵ�ֵ
	HWND hEdit = NULL;
	if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_PRESS_R))	// ��ѹ
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_PRESS_R);	
		TBNotifications(wParam, hWndCtrl, MIN_PRESSURE, MAX_PRESSURE, hEdit);

		ad.nPress = GetDlgItemInt(hWnd, IDC_EDIT_PRESS_R, &bSuccess, FALSE);
		
		WORD nPress = WORD(ad.nPress/400.0f*255.0f);
		send_mcp41010(nPress);		
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_MAIKUAN_R))	// ����
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_MAIKUAN_R);	
		TBNotifications(wParam, hWndCtrl, MIN_MAIKUAN, MAX_MAIKUAN, hEdit);

		ad.nMaiKuan = GetDlgItemInt(hWnd, IDC_EDIT_MAIKUAN_R, &bSuccess, FALSE);
		// ����
		chCurrent_con_reg->wMaiKuan = int(float(ad.nMaiKuan)/20.0 +0.5);
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_R_PINLV_R))	// �ظ�Ƶ��
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_R_PINLV_R);
		TBNotifications(wParam, hWndCtrl, MIN_RE_FREQ, MAX_RE_FREQ, hEdit);

		ad.nRefrainFrequency = GetDlgItemInt(hWnd, IDC_EDIT_R_PINLV_R, &bSuccess, FALSE);
		// �ظ�Ƶ��
		chCurrent_con_reg->wRepeatCounter = 20000 / ad.nRefrainFrequency;
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_AMEND_R))	// ����
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_AMEND_R);
		TBNotifications(wParam, hWndCtrl, MIN_AMEND, MAX_AMEND, hEdit);

		ad.nAmend = GetDlgItemInt(hWnd, IDC_EDIT_AMEND_R, &bSuccess, FALSE);
		// ����
		float fTemp = ad.nAmend + ReceiveParam_ad[0].fDelay;
		chCurrent_data_reg->wMonitorZeroCounter = WORD(Range2Dot(WaveSpeed, fTemp));
	}

	return TRUE;
}
