#ifndef __FNSPECIALFUNCTIONWIN_H__
#define __FNSPECIALDISPLAYWIN_H__


ATOM registerSpecialFunctionChild(HINSTANCE hInstance);
LRESULT CALLBACK specialFunctionChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int specialFunction_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnSpecialFunctionBScanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSpecialFunctionZhongHouBanTanShangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSpecialFunctionHanFengTanShangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSpecialFunctionFuHeCaiLiaoJianCeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSpecialFunctionZhuJianTanShangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSpecialFunctionLieWenCeGaoProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__FNSPECIALFUNCTIONWIN_H__