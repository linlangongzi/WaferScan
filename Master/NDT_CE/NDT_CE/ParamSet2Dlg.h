#ifndef _PARAMSETDLG2_H_
#define _PARAMSETDLG2_H_


typedef struct{
	WORD nAddress;
	WORD nValue;
	WORD nMcp41010;
	WORD nMax500_value1;
	WORD nMax500_value2;
} PARAM_SET_DATA2;

BOOL CALLBACK ParamSetDlgProc2(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);

BOOL OnCreateParamSetDlg2(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL OnInitialParamSetDlg2(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goCommandParamSetDlg2(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goNotifyParamSetDlg2(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goHScrollParamSetDlg2(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);

unsigned long HexToInt(WCHAR* pch , int at);
#endif