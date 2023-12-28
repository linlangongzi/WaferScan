#ifndef __FNSPECIALDISPLAYWIN_H__
#define __FNSPECIALDISPLAYWIN_H__


ATOM registerSpecialDisplayChild(HINSTANCE hInstance);
LRESULT CALLBACK specialDisplayChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int specialDisplay_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnSpecialDisplayDynamicIncludeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSpecialDisplayFreezeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSpecialDisplayReferenceWaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSpecialDisplayHuiBoBianMaProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSpecialDisplayMenMeiZhanKuanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSpecialDisplay110DisplayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );


#endif//__FNSPECIALDISPLAYWIN_H__