#include "StdAfx.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "ReceiveParamDlg.h"
#include "SendParamDlg.h"

// extern
extern ARS200FPGA_SYS_REG* sys_reg;
extern ARS200FPGA_CH_DATA_REG *chCurrent_data_reg;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern ARS200FPGA_CH_PLUS_REG* chCurrent_plus_reg;
extern WORD WaveSpeed;
extern WORD CurrentChannelNo;
extern SEND_PARAM_DATA SendParam_ad[2];
extern HWND hStatusBar;
extern float gCh1Plus;
extern float gCh2Plus;
extern wchar_t gszTextPlus[10];

int  MIN_DELAY1 = 0;
extern float MAX_DELAY;
extern WORD MAX_PLUS;

const dlgMessageProc ReceiveParamDlgMessages[] = {
	WM_CREATE, OnCreateReceiveParamDlg,
	WM_INITDIALOG, OnInitialReceiveParamDlg,
	WM_COMMAND, goCommandReceiveParamDlg,
	WM_NOTIFY, goNotifyReceiveParamDlg,
	WM_HSCROLL, goHScrollReceiveParamDlg,
};

static RECEIVE_PARAM_DATA ad, *pad;

BOOL CALLBACK ReceiveParamDlgProc(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	for(int i=0; i< dim(ReceiveParamDlgMessages); i++)
	{
		if(msgCode == ReceiveParamDlgMessages[i].uCode)
			return (*ReceiveParamDlgMessages[i].functionName)(hWnd, msgCode, wParam, lParam);
	}

	return FALSE;
}

BOOL OnCreateReceiveParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BOOL OnInitialReceiveParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	// 数据初始化
	pad = (RECEIVE_PARAM_DATA *)lParam;
        
	ad = *pad;

	// 控件初始化及赋值
	// 频率
	HWND hPinLv[7];
	for(int i=0; i<7;i++)
	{
		hPinLv[i] = GetDlgItem(hWnd, IDC_RADIO_PINLV1_R+i);
		SendMessage(hPinLv[i], BM_SETCHECK, 0, 0);
	}

	HWND hPinLvSet = GetDlgItem(hWnd, IDC_RADIO_PINLV1_R + ad.nProbeFreq);
	SendMessage(hPinLvSet, BM_SETCHECK, 1, 0);

	// 检波模式
	HWND hJianBoMode[4];
	for(int i=0; i<4;i++)
	{
		hJianBoMode[i] = GetDlgItem(hWnd, IDC_RADIO_JIANBO1_R+i);
		SendMessage(hJianBoMode[i], BM_SETCHECK, 0, 0);
	}

	HWND hJianBoSet = GetDlgItem(hWnd, IDC_RADIO_JIANBO1_R+ad.nJianBoMode);
	SendMessage(hJianBoSet, BM_SETCHECK, 1, 0);

	// 增益
	HWND hWndSldPlus = GetDlgItem(hWnd, IDC_SLIDER_PLUS_R);
	HWND hWndEdtPlus = GetDlgItem(hWnd, IDC_EDIT_PLUS_R);
	InitSldAndEit(hWndSldPlus, hWndEdtPlus, MAX_PLUS, MIN_PLUS, 4, ad.fPlus);

	// 延迟
	HWND hWndSldDelay = GetDlgItem(hWnd, IDC_SLIDER_DELAY_R);
	HWND hWndEdtDelay = GetDlgItem(hWnd, IDC_EDIT_DELAY_R);	
	InitSldAndEit(hWndSldDelay, hWndEdtDelay, MAX_DELAY, MIN_DELAY1, 4, ad.fDelay);

	// 滤波频带
	HWND hWndSldLvBo = GetDlgItem(hWnd, IDC_SLIDER_LVBO_R);
	HWND hWndEdtLvBo = GetDlgItem(hWnd, IDC_EDIT_LVBO_R);	
	InitSldAndEit(hWndSldLvBo, hWndEdtLvBo, 255, 0, 1, ad.nLvBo);

	return FALSE;
}

BOOL goCommandReceiveParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	BOOL bSuccess = FALSE;	

	switch(LOWORD(wParam))
	{
	case IDOK:
		// 获取对话框参数
		ad.fPlus = GetDlgItemInt(hWnd, IDC_EDIT_PLUS_R, &bSuccess, FALSE);
		ad.fDelay = GetDlgItemInt(hWnd, IDC_EDIT_DELAY_R, &bSuccess, FALSE);
		ad.nLvBo = GetDlgItemInt(hWnd, IDC_EDIT_LVBO_R, &bSuccess, FALSE);

		*pad = ad;

		EndDialog(hWnd, 1);

		return TRUE;
	case IDCANCEL:
		EndDialog(hWnd, 2);

		return TRUE;
		break;	
	case IDC_RADIO_PINLV1_R:
		ad.nProbeFreq = 0;
		return TRUE;
		break;
	case IDC_RADIO_PINLV2_R:
		ad.nProbeFreq = 1;
		return TRUE;
		break;
	case IDC_RADIO_PINLV3_R:
		ad.nProbeFreq = 2;
		return TRUE;
		break;
	case IDC_RADIO_PINLV4_R:
		ad.nProbeFreq = 3;
		return TRUE;
		break;
	case IDC_RADIO_PINLV5_R:
		ad.nProbeFreq = 4;
		return TRUE;
		break;
	case IDC_RADIO_PINLV6_R:
		ad.nProbeFreq = 5;
		return TRUE;
		break;
	case IDC_RADIO_PINLV7_R:
		ad.nProbeFreq = 6;
		return TRUE;
		break;
	case IDC_RADIO_JIANBO1_R:
		ad.nJianBoMode = 0;

		if(CurrentChannelNo == 0)
		{
			sys_reg->wRelay &= 0xFFE1;
		}
		else if(CurrentChannelNo == 1)
		{
			sys_reg->wRelay &= 0xFE1F;
		}
		else
		{
		}
		return TRUE;
		break;
	case IDC_RADIO_JIANBO2_R:
		ad.nJianBoMode = 1;

		if(CurrentChannelNo == 0)
		{
			sys_reg->wRelay |= 0x12;
			sys_reg->wRelay &= 0xFFF3;
		}
		else if(CurrentChannelNo == 1)
		{
			sys_reg->wRelay |= 0x120;
			sys_reg->wRelay &= 0xFF3F;
		}
		else
		{
		}
		return TRUE;
		break;
	case IDC_RADIO_JIANBO3_R:
		ad.nJianBoMode = 2;

		if(CurrentChannelNo == 0)
		{
			sys_reg->wRelay |= 0x2;
			sys_reg->wRelay &= 0xFFE3;
		}
		else if(CurrentChannelNo == 1)
		{
			sys_reg->wRelay |= 0x20;
			sys_reg->wRelay &= 0xFE3F;
		}
		else
		{
		}
		return TRUE;
		break;
	case IDC_RADIO_JIANBO4_R:
		ad.nJianBoMode = 3;

		if(CurrentChannelNo == 0)
		{
			sys_reg->wRelay |= 0x4;
			sys_reg->wRelay &= 0xFFE5;
		}
		else if(CurrentChannelNo == 1)
		{
			sys_reg->wRelay |= 0x40;
			sys_reg->wRelay &= 0xFE5F;
		}
		else
		{
		}
		return TRUE;
		break;
	}
	return FALSE;
}

BOOL goNotifyReceiveParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BOOL goHScrollReceiveParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	HWND hWndCtrl = (HWND)lParam;
	BOOL bSuccess = FALSE;

	// 设对应的Edit窗口的值
	HWND hEdit = NULL;
	if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_PLUS_R))	// 增益
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_PLUS_R);	
		TBNotifications(wParam, hWndCtrl, MIN_PLUS, MAX_PLUS, hEdit);

		ad.fPlus = GetDlgItemInt(hWnd, IDC_EDIT_PLUS_R, &bSuccess, FALSE);

	/*	chCurrent_plus_reg->wdBRef = 128;
		PlusChangeTurn();

		int wTemp1 = Gain2Data(ad.fPlus) / 3;

		chCurrent_plus_reg->wdBLevel1 = 128 + wTemp1;
		PlusChangeTurn();

		int wTemp2 = (Gain2Data(ad.fPlus) - wTemp1) / 2;

		chCurrent_plus_reg->wdBLevel2 = 128 + wTemp2;
		PlusChangeTurn();

		int wTemp3 = Gain2Data(ad.fPlus) - wTemp1 - wTemp2;

		chCurrent_plus_reg->wdBLevel3 = 128 + wTemp3;
		PlusChangeTurn();
	*/
		if(CurrentChannelNo == 0)
			gCh1Plus = ad.fPlus;
		else if(CurrentChannelNo == 1)
			gCh2Plus = ad.fPlus;

//		wchar_t strInfo[20];
		memset(gszTextPlus, 0, sizeof(gszTextPlus));
		swprintf(gszTextPlus, _T("增益: %.1f dB"), ad.fPlus);

		SendMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_DELAY_R))	// 延迟
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_DELAY_R);	
		TBNotifications(wParam, hWndCtrl, MIN_DELAY1, MAX_DELAY, hEdit);

		ad.fDelay = GetDlgItemInt(hWnd, IDC_EDIT_DELAY_R, &bSuccess, FALSE);

		float fTemp = SendParam_ad[0].nAmend + ad.fDelay;
		chCurrent_data_reg->wMonitorZeroCounter = WORD(Range2Dot(WaveSpeed, fTemp));
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_LVBO_R))	// 滤波
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_LVBO_R);	
		TBNotifications(wParam, hWndCtrl, 0, 255, hEdit);

		ad.nLvBo = GetDlgItemInt(hWnd, IDC_EDIT_LVBO_R, &bSuccess, FALSE);
		WORD nMax500Value = 0;
		if(CurrentChannelNo == 0)
		{
//			nMax500Value = 1<<8 + WORD(ReceiveParam_ad.nLvBo/10.0f*255.0f);
			nMax500Value = (1<<8) + ad.nLvBo;
		}
		else if(CurrentChannelNo == 1)
		{
//			nMax500Value = WORD(ReceiveParam_ad.nLvBo/10.0f*255.0f);
			nMax500Value = ad.nLvBo;
		}

		send_max500(nMax500Value);
	}

	return TRUE;
}