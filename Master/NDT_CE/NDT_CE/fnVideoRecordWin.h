#ifndef __FNVIDEORECORDWIN_H__
#define __FNVIDEORECORDWIN_H__


ATOM registerVideoRecordChild(HINSTANCE hInstance);
LRESULT CALLBACK videoRecordChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int videoRecord_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnVideoRecordFileNumberProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnVideoRecordRecordOnOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnVideoRecordPlayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__FNVIDEORECORDWIN_H__