#ifndef __FNZHONGHOUBANTANSHANGEWIN_H__
#define __FNZHONGHOUBANTANSHANGEWIN_H__


ATOM registerZhongHouBanTanShangChild(HINSTANCE hInstance);
LRESULT CALLBACK zhongHouBanTanShangChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int zhongHouBanTanShang_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnZhongHouBanTanShangThicknessProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnZhongHouBanTanShangHeadModelProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnZhongHouBanTanShangDacInputProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnZhongHouBanTanShangDacOnOrOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnZhongHouBanTanShangDelayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnZhongHouBanTanShangUpBlindAreaProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnZhongHouBanTanShangDownBlindAreaProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnZhongHouBanTanShangSensitiveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

void SetGuanLianDownBlindAreaGate();
#endif//__FNZHONGHOUBANTANSHANGEWIN_H__