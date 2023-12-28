#ifndef _ALLPARAMSETDLG_H_
#define _ALLPARAMSETDLG_H_


BOOL CALLBACK AllParamSetDlgProc(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);

BOOL OnCreateAllParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL OnInitialAllParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goCommandAllParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goNotifyAllParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goHScrollAllParamSetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);

#endif