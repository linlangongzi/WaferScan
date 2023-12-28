#ifndef _SENDPARAMDLG_H_
#define _SENDPARAMDLG_H_


BOOL CALLBACK SendParamDlgProc(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);

BOOL OnCreateSendParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL OnInitialSendParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goCommandSendParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goNotifySendParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goHScrollSendParamDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);


#endif