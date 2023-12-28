#ifndef __FNPOKOUSHEZHI_H__
#define __FNPOKOUSHEZHI_H__


ATOM registerPoKouSheZhiChild(HINSTANCE hInstance);
LRESULT CALLBACK poKouSheZhiChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK btnPoKouSheZhiLeiXingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnPoKouSheZhiPoKouJiaoDuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnPoKouSheZhiPoKouShenDuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

int poKouSheZhi_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);


#endif//__FNPOKOUSHEZHI_H__