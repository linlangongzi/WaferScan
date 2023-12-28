#ifndef __FNBSCANEWIN_H__
#define __FNBSCANEWIN_H__


ATOM registerBScanChild(HINSTANCE hInstance);
LRESULT CALLBACK BScanChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int BScan_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnBScanGangBanHouDuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnBScanOnOrOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnBScanScanModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnBScanGateQianYanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnBScanGateHouYanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnBScanGateFuZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

void setBScanSwitch(HWND hWnd, BOOL bSwitch);

#endif//__FNBSCANEWIN_H__