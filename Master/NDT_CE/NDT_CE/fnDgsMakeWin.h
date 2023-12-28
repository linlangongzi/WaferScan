#ifndef __FNDGSMAKEWIN_H__
#define __FNDGSMAKEWIN_H__


ATOM registerDgsMakeChild(HINSTANCE hInstance);
LRESULT CALLBACK dgsMakeChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//LRESULT CALLBACK btnDgsMakeTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDgsMakeCreateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDgsMakeModifyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDgsMakePingDingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDgsMakeDingLiangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDgsMakePanHuiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
int dgsMake_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);

#endif//__FNDGSMAKEWIN_H__