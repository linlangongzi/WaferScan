#ifndef __RECVPARAMWIN_H__
#define __RECVPARAMWIN_H__


ATOM registerRecvParamChild(HINSTANCE hInstance);
LRESULT CALLBACK recvParamChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int recvParam_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
//LRESULT CALLBACK btnRecvParamHeadFreqProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnRecvParamWaveModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnRecvParamGainProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnRecvParamDelayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnRecvParamWaveBandProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnRecvParamReferenceGainProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnRecvParamImpedanceProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

void SetJianBoModeLableAndExecute(HWND hWnd);
void SetPlusLableAndExecute(HWND hWnd, const float& fBasePlus, const float& fRefPlus, const BOOL& bSave, const WORD& wChNo );
void SetDelayLableAndExecute(HWND hWnd, const WORD& nChNo);
void SetLvBoLableAndExecute(HWND hWnd, const WORD& chNo);
void SetRefPlusLableAndExecute(HWND hWnd, const float& fBasePlus, const float& fRefPlus, const BOOL& bSave, const WORD& chNo);
//void SetLittleModifyLableAndExecute(HWND hWnd);
void displayImpedance(HWND hWnd,WORD wChNo, WORD wImpedance);
void SetImpedanceLableAndExecute(HWND hWnd, WORD wImpedeance, WORD wChNo, BOOL bSaveFlag);
#endif//__RECVPARAMWIN_H__