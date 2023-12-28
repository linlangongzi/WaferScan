#ifndef _PARAMSETDLG_H_
#define _PARAMSETDLG_H_

typedef struct{
	WORD nRange;
	WORD nDelay;
	WORD nPlusA;
	WORD nPlusB;
	WORD nPlusC;
	WORD nPlusD;
	WORD nMaiKuan;
	WORD nPress;
	WORD nMidValue;
} PARAM_SET_DATA;

BOOL CALLBACK ParamSetDlgProc(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);

BOOL OnCreateParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL OnInitialParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goCommandParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goNotifyParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goHScrollParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);

#endif