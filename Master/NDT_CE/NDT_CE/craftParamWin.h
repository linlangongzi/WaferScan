#ifndef _CRAFT_PARAM_WIN_H_
#define _CRAFT_PARAM_WIN_H_


ATOM registerCraftParamChild(HINSTANCE hInstance);
LRESULT CALLBACK craftParamChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int craftParam_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnCraftParamNumberProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCraftParamReviewProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCraftParamSaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCraftParamGetProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCraftParamDeleteProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

BOOL SaveParamFile(LPCTSTR lpszPathName);
BOOL ReadParamFile(LPCTSTR lpszPathName);
#endif