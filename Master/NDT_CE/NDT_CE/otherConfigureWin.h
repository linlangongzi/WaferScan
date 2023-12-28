#ifndef __OTHERCONFIGUREWIN_H__
#define __OTHERCONFIGUREWIN_H__


ATOM registerOtherCfgChild(HINSTANCE hInstance);
LRESULT CALLBACK otherCfgChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK btnOtherCfgCommunicationProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnOtherCfgPrintProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnOtherCfgLcdProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnOtherCfgVolumeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnOtherCfgStoreModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
int otherCfg_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
int otherCfgRefresh_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);

void GetSysDrivesInfo();
void SetVolumn(const WORD& nVolumn);
void SetDriveType(HWND hWnd, const WORD& nType);
#endif//__OTHERCONFIGUREWIN_H__