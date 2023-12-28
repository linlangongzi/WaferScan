#ifndef __FNHUIBOBIANMAWIN_H__
#define __FNHUIBOBIANMAWIN_H__


ATOM registerHuiBoBianMaChild(HINSTANCE hInstance);
LRESULT CALLBACK huiBoBianMaChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int huiBoBianMa_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnHuiBoBianMaSelectProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnHuiBoBianMaGongJianHouDuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnHuiBoBianMaOnOrOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__FNHUIBOBIANMAWIN_H__