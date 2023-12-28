#ifndef __FNHANFENGTANSHANGWIN_H__
#define __FNHANFENGTANSHANGWIN_H__


ATOM registerHanFengTanShangChild(HINSTANCE hInstance);
LRESULT CALLBACK hanFengTanShangChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int hanFengTanShang_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnHanFengTanShangPoKouSheZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnHanFengTanShangGangBanHouDuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnHanFengTanShangDistanceProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnHanFengTanShangRefractAngleProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnHanFengTanShangOnOrOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__FNHANFENGTANSHANGWIN_H__