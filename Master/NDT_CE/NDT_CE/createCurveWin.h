#ifndef __CREATECURVEWIN_H__
#define __CREATECURVEWIN_H__


ATOM registerCreateCurveChild(HINSTANCE hInstance);
LRESULT CALLBACK createCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK btnCreateCurveAddPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCreateCurveDeletePointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCreateCurveEndProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

int createCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);


#endif//__CREATECURVEWIN_H__