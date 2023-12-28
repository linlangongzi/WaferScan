#ifndef __FNMENMEIZHANKUANWIN_H__
#define __FNMENMEIZHANKUANWIN_H__

const TCHAR szMenMeiZhanKuanChild[] = TEXT("menMeiZhanKuanChild");
ATOM registerMenMeiZhanKuanChild(HINSTANCE hInstance);
LRESULT CALLBACK menMeiZhanKuanChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int menMeiZhanKuan_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnMenMeiZhanKuanQianYanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnMenMeiZhanKuanHouYanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnMenMeiZhanKuanOnOrOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__FNMENMEIZHANKUANWIN_H__
