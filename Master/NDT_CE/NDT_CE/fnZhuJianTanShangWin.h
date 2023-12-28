#ifndef __ZHUJIANTANSHANGWIN_H__
#define __ZHUJIANTANSHANGWIN_H__


ATOM registerZhuJianChild(HINSTANCE hInstance);
LRESULT CALLBACK zhuJianChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int zhuJian_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnZhuJianHouDuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnZhuJianUpBlindAreaProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnZhuJianDownBlindAreaProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnZhuJianSensitiveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnZhuJianDefectZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__ZHUJIANTANSHANGWIN_H__

