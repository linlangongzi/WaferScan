#ifndef __FNMENUWIN_H__
#define __FNMENUWIN_H__


ATOM registerFnMenuChild(HINSTANCE hInstance);
LRESULT CALLBACK fnMenuChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int fnMenu_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnFnMenuQuantitativeAnalysisProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnFnMenuSpecialDisplayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnFnMenuSpecialFunctionProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnFnMenuVideoRecordProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__FNMENUWIN_H__