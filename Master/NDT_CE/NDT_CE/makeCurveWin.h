#ifndef __MAKECURVEWIN_H__
#define __MAKECURVEWIN_H__


ATOM registerMakeCurveChild(HINSTANCE hInstance);
LRESULT CALLBACK makeCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK btnMakeCurveCreateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnMakeCurveModifyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
int makeCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);

#endif//__MAKECurveWIN_H__