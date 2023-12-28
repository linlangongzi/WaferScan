#ifndef _RECEIVEPARAMDLG_H_
#define _RECEIVEPARAMDLG_H_


BOOL CALLBACK ReceiveParamDlgProc(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);

BOOL OnCreateReceiveParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL OnInitialReceiveParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goCommandReceiveParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goNotifyReceiveParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goHScrollReceiveParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);

#endif