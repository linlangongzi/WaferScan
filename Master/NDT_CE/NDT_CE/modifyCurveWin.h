#ifndef __MODIFYCURVEWIN_H__
#define __MODIFYCURVEWIN_H__

ATOM registerModifyCurveChild(HINSTANCE hInstance);
LRESULT CALLBACK ModifyCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK btnModifyCurveAddPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnModifyCurveSelPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnModifyCurveModifyAmpProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnModifyCurveDeletePointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnModifyCurveEndProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
int ModifyCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);

#endif//__MODIFYCURVEWIN_H__
