#ifndef __SCREENCOPYWIN_H__
#define __SCREENCOPYWIN_H__


ATOM registerScreenCopyChild(HINSTANCE hInstance);
LRESULT CALLBACK screenCopyChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int screenCopy_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnScreenCopyNumberProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnScreenCopySaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnScreenCopyPrintProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnScreenCopyDeleteProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__SCREENCOPYWIN_H__