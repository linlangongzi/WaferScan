#ifndef __HOTKEYWIN_H__
#define __HOTKEYWIN_H__



ATOM registerHotKeyChild(HINSTANCE hInstance);
LRESULT CALLBACK hotKeyChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK btnHotKeySelectProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnHotKeySettingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnHotKeyDeleteProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
int hotKey_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);

#endif//__HOTKEYWIN_H__