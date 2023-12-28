#ifndef __FNDGSCURVEWIN_H__
#define __FNDGSCURVEWIN_H__


ATOM registerDgsCurveChild(HINSTANCE hInstance);
LRESULT CALLBACK dgsCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int dgsCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);

LRESULT CALLBACK btnDgsCurveMakeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDgsCurveOnOrOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDgsCurveQueXianDaXiaoProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDgsCurveShengChengWeiZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDgsCurveShenDuWeiZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDgsCurveshuiPingWeiZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__FNDGSCURVEWIN_H__