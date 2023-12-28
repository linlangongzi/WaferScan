#ifndef __FNCREATEDGSWIN_H__
#define __FNCREATEDGSWIN_H__


ATOM registerCreateDgsCurveChild(HINSTANCE hInstance);
LRESULT CALLBACK createDgsCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK btnCreateDgsCurveAddPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCreateDgsCurveDeletePointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCreateDgsCurvePingDingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCreateDgsCurveDingLiangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCreateDgsCurvePanHuiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnCreateDgsCurveEndProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

int createDgsCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);


#endif//__FNCREATEDGSWIN_H__