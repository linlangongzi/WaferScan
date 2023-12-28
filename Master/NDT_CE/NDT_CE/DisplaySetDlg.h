#ifndef _DISPLAYSETDLG_H_
#define _DISPLAYSETDLG_H_

BOOL CALLBACK DisplaySetDlgProc(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);

BOOL OnCreateDisplaySetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL OnInitialDisplaySetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goCommandDisplaySetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goNotifyDisplaySetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);
BOOL goHScrollDisplaySetDlg(HWND hWnd, UINT msgCode, WPARAM wParam, LPARAM lParam);

#endif