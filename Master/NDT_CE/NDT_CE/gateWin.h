#ifndef __GATEWIN_H__
#define __GATEWIN_H__


ATOM registerGateChild(HINSTANCE hInstance);
LRESULT CALLBACK gateChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int gate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnGateSelectProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnGateAlarmModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnGateSwitchProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnGateQiDianProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnGateKuanDuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnGateFuZhiProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

// songchenguang add start 2010-05-25
void DisplayGateIndex();
void SetAlarmMode(HWND hWnd, const WORD& nGateIndex, const	WORD& nIndex);
void SetOpenFlag(HWND hWnd, const BOOL& bOpen, const WORD& nGateIndex);
void SetQianYan(HWND hWnd, const WORD& nGateIndex, const float& fFront, const BOOL& bSave);
void SetHouYan(HWND hWnd, const WORD& nGateIndex, const float& fBack, const BOOL& bSave);
void SetQiDian(HWND hWnd, const WORD& nGateIndex, const float& fFront, const float& fBack, BOOL bSave);
void SetKuanDu(HWND hWnd, const WORD& nGateIndex, const float& fFront, const float& fKuanDu, BOOL bSave);
void SetPeakGate(HWND hwnd, const WORD& nGateIndex, const float& fFuzhi, const BOOL& bSave);

void SetGateCurve(WORD* nCurves, 
				  const WORD& nRange,
				  const float& fFuzhi);

// songchenguang add end 2010-05-25
#endif//__GATEWIN_H__