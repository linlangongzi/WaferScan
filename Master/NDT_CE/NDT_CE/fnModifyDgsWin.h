#ifndef __FNMODIFYDGSWIN_H__
#define __FNMODIFYDGSWIN_H__


ATOM registerModifyDgsCurveChild(HINSTANCE hInstance);
LRESULT CALLBACK ModifyDgsCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK btnModifyDgsCurveAddPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnModifyDgsCurveSelPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnModifyDgsCurveModifyAmpProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnModifyDgsCurveDeletePointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnModifyDgsCurvePingDingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnModifyDgsCurveDingLiangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnModifyDgsCurvePanHuiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnModifyDgsCurveEndProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

int ModifyDgsCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);


#endif//__FNMODIFYDGSWIN_H__