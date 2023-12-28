#ifndef __GATEDISPLAYWIN_H__
#define __GATEDISPLAYWIN_H__


ATOM registerGateDisplayChild(HINSTANCE hInstance);
LRESULT CALLBACK gateDisplayChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int gateDisplay_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnGateDisplayGateCfgProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnGateDisplaySelectProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnGateDisplayQueXianDaXiaoProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnGateDisplayShengChengWeiZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnGateDisplayShenDuWeiZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnGateDisplayshuiPingWeiZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__GATEDISPLAYWIN_H__