#ifndef __FNREFERENCEWAVEWIN_H__
#define __FNREFERENCEWAVEWIN_H__


ATOM registerReferenceWaveChild(HINSTANCE hInstance);
LRESULT CALLBACK referenceWaveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int referenceWave_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnReferenceWaveSelectProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnReferenceWaveSaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnReferenceWaveOnOrOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__FNREFERENCEWAVEWIN_H__