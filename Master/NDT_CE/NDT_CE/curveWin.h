#ifndef __CURVEWIN_H__
#define __CURVEWIN_H__


ATOM registerCurveChild(HINSTANCE hInstance);
LRESULT CALLBACK curveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK btnCurveTcgProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCurveDacProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCurveAvgProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCurveQuMianXiuZhengProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
int curve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);

#endif//__CURVEWIN_H__