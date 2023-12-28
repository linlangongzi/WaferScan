#ifndef __TSMENUWIN_H__
#define __SETWIN_H__

#define MAX_LOADSTRING 100


ATOM registerTsMenuChild(HINSTANCE hInstance);

LRESULT CALLBACK tsMenuChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int tsMenu_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnParamProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnOtherCfgProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnFileProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnHotkeyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
//int WinDrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis,const BTN_BMP_INFO *pBtnBmpInfo,int nIDBitmap,int ButtonID);

#endif//__TSMENUWIN_H__