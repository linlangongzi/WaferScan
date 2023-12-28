#ifndef __REPORTFORMWIN_H__
#define __REPORTFORMWIN_H__


ATOM registerReportFormChild(HINSTANCE hInstance);
LRESULT CALLBACK reportFormChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int reportForm_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnReportFormNumberProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnReportFormSaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnReportFormReviewProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnReportFormPrintProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

#endif//__REPORTFORMWIN_H__


