#ifndef __FILEMANAGEWIN_H__
#define __FILEMANAGEWIN_H__


ATOM registerFileManageChild(HINSTANCE hInstance);
LRESULT CALLBACK fileManageChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK btnFileManageCraftParamProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnFileManageReportFormProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnFileManageScreenCopyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
int fileManage_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);

#endif//__FILEMANAGEWIN_H__