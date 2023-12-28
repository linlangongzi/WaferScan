#ifndef __SENDPARAMWIN_H__
#define __SENDPARAMWIN_H__


ATOM registerSendParamChild(HINSTANCE hInstance);
LRESULT CALLBACK sendParamChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int sendParam_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnSendParamVoltageProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSendParamMaiKuanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSendParamRepeatFreqProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSendParamModifyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnSendParamHeadQianYanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

void SetPressLableAndExecute(HWND hWnd);
void SetAmendLableAndExecute(HWND hWnd);
void SetReFreqLableAndExecute(HWND hWnd);
void SetMaiKuanLableAndExecute(HWND hWnd);
void SetObliqueProbeQianYan(HWND hWnd);
WORD typeToVoltage(WORD wType);

#endif//__SENDPARAMWIN_H__
