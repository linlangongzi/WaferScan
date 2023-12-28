#ifndef __CURVEWIN_H__
#define __CURVEWIN_H__


ATOM registerTcgCurveChild(HINSTANCE hInstance);
LRESULT CALLBACK tcgCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK btnTcgCurveMakeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnTcgCurveOnOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
int tcgCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);

#endif//__CURVEWIN_H__