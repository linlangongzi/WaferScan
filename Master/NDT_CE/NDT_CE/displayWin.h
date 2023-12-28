#ifndef __DISPLAYWIN_H__
#define __DISPLAYWIN_H__


ATOM registerDisplayChild(HINSTANCE hInstance);
LRESULT CALLBACK displayChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int display_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnDisplayRefractModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDisplaySoundDistanceProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDisplaySoundSpeedProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDisplayRefractAngleProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnDisplayRestraintProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
void SetRangeLableAndExecute(HWND hWnd, const float& fRange, const BOOL& bSave);
//void SetWaveSpeedLabelAndExecute(HWND hWnd);
void SetAngleLabelAndExecute(HWND Wnd);
void SetBateLabelAndExecute(HWND hWnd);
void SetAngleMode(HWND hWnd, BOOL bFlag);
#endif//__DISPLAYWIN_H__