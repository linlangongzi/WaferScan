#include "StdAfx.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "DisplaySetDlg.h"
#include "resource.h"


// extern
extern WORD WaveSpeed;
extern WORD *chCurrent_interval;
extern ARS200FPGA_CH_CON_REG *chCurrent_con_reg;
extern float gCh1Range;
extern float gCh2Range;
extern WORD CurrentChannelNo;
extern HWND hStatusBar;
//extern wchar_t gszTextRange[10];

const dlgMessageProc DisplaySetDlgMessages[] = {
	WM_CREATE, OnCreateDisplaySetDlg,
	WM_INITDIALOG, OnInitialDisplaySetDlg,
	WM_COMMAND, goCommandDisplaySetDlg,
	WM_NOTIFY, goNotifyDisplaySetDlg,
	WM_HSCROLL, goHScrollDisplaySetDlg,
};

static DISPLAY_SET_DLG_DATA ad, *pad;

BOOL CALLBACK DisplaySetDlgProc(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	for(int i=0; i< dim(DisplaySetDlgMessages); i++)
	{
		if(msgCode == DisplaySetDlgMessages[i].uCode)
			return (*DisplaySetDlgMessages[i].functionName)(hWnd, msgCode, wParam, lParam);
	}

	return FALSE;
}

BOOL OnCreateDisplaySetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BOOL OnInitialDisplaySetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	WCHAR szString[10];
	memset(szString, 0, sizeof(szString));	
	
	// ���ݳ�ʼ��
	pad = (DISPLAY_SET_DLG_DATA *)lParam;
        
	ad = *pad;

	// �ؼ���ʼ������ֵ
	// ����
	HWND hWndSldRange = GetDlgItem(hWnd, IDC_SLIDER_RANGE_R);
	HWND hWndEdtRange = GetDlgItem(hWnd, IDC_EDIT_RANGE_R);
	InitSldAndEit(hWndSldRange, hWndEdtRange, MAX_RANGE, MIN_RANGE, 20, ad.fRange);

	// ����
	HWND hEditSpeed = GetDlgItem(hWnd, IDC_EDIT_WAVE_SPEED_R);
	swprintf(szString, _T("%d"), ad.nWaveSpeed);
	SendMessage(hEditSpeed, EM_REPLACESEL, 0, (LPARAM) szString);

	// �����		
	HWND hEditZheSheJiao = GetDlgItem(hWnd, IDC_EDIT_ANGLE_R);
	memset(szString, 0, sizeof(szString));
	swprintf(szString, _T("%.1f"), ad.fZheSheJiao);
	SendMessage(hEditZheSheJiao, EM_REPLACESEL, 0, (LPARAM) szString);

	// ����
	HWND hWndSldBate = GetDlgItem(hWnd, IDC_SLIDER_BATE_R);
	HWND hWndEdtBate = GetDlgItem(hWnd, IDC_EDIT_BATE_R);	
	InitSldAndEit(hWndSldBate, hWndEdtBate, MAX_BATE, MIN_BATE, 5, ad.nBate);

	return FALSE;
}

BOOL goCommandDisplaySetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	BOOL bSuccess = FALSE;	

	switch(LOWORD(wParam))
	{
	case IDOK:
		// ��ȡ�Ի������
		ad.fRange = GetDlgItemInt(hWnd, IDC_EDIT_RANGE_R, &bSuccess, FALSE);
		ad.nWaveSpeed = GetDlgItemInt(hWnd, IDC_EDIT_WAVE_SPEED_R, &bSuccess, FALSE);
		ad.fZheSheJiao = GetDlgItemInt(hWnd, IDC_EDIT_ANGLE_R, &bSuccess, FALSE);
		ad.nBate = GetDlgItemInt(hWnd, IDC_EDIT_BATE_R, &bSuccess, FALSE);

		*pad = ad;

		EndDialog(hWnd, IDOK);

		return TRUE;
	case IDCANCEL:
		EndDialog(hWnd, IDCANCEL);

		return TRUE;
		break;	
	}
	return FALSE;
}

BOOL goNotifyDisplaySetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	return FALSE;
}

BOOL goHScrollDisplaySetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam)
{
	HWND hWndCtrl = (HWND)lParam;
	BOOL bSuccess = FALSE;	

	// ���Ӧ��Edit���ڵ�ֵ
	HWND hEdit = NULL;
	if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_RANGE_R))	// ����
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_RANGE_R);	
		TBNotifications(wParam, hWndCtrl, MIN_RANGE, MAX_RANGE, hEdit);

		ad.fRange = GetDlgItemInt(hWnd, IDC_EDIT_RANGE_R, &bSuccess, FALSE);
		WORD wInterval[DATA_SIZE];
		memset(wInterval, 0, sizeof(wInterval));

		float fRange = ad.fRange;
		RangeProcess(WaveSpeed, fRange, wInterval);

		memcpy(chCurrent_interval, wInterval, sizeof(wInterval));

		if(CurrentChannelNo == 0)
		{
			gCh1Range = fRange;
		}
		else if(CurrentChannelNo == 1)
		{
			gCh2Range = fRange;
		}
/*		
//		wchar_t szText[20];
		memset(gszTextRange, 0, sizeof(gszTextRange));
		swprintf(gszTextRange, _T("���̣�%.2f"), fRange);
		SendMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRange);
*/
	}
	else if(hWndCtrl == GetDlgItem(hWnd, IDC_SLIDER_BATE_R))	// ����
	{
		hEdit = GetDlgItem(hWnd, IDC_EDIT_BATE_R);	
		TBNotifications(wParam, hWndCtrl, MIN_BATE, MAX_BATE, hEdit);

		ad.nBate = GetDlgItemInt(hWnd, IDC_EDIT_BATE_R, &bSuccess, FALSE);
		//yachang.huang delete on 2013-12-10
		//chCurrent_con_reg->wSuspend = WORD((ad.nBate/100.0f)*MAX_HEIGHT);
	}

	return TRUE;
}