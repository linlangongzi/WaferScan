#ifndef __PARAMWIN_H__
#define __PARAMWIN_H__


ATOM registerParamChild(HINSTANCE hInstance);
LRESULT CALLBACK paramChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int param_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnParamSendProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnParamRecvProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnParamDisplayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnParamCurveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnParamGateProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__PARAMWIN_H__