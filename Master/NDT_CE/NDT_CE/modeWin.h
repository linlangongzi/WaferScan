#ifndef __MODEWIN_H__
#define __MODEWIN_H__
extern enum WORKMODE;


ATOM registerModeChild(HINSTANCE hInstance);
LRESULT CALLBACK modeChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK btnMode1or2Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnMode1plus2Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnMode1to2Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnModeDanjingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnModeShuangjingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
int mode_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);

void Ch1Data2Ch2Data();
void ChangeWorkMode(WORKMODE emWorkMode, BOOL bSaveFlag);	//切换工作模式
void ChangeTo1or2();			//切换为单通道
void ChangeTo1plus2();			//切换为双通道
void ChangeTo1to2();			//切换为一发一收
void ChangeToDanJing();			//切换为单晶
void ChangeToShuangJing();		//切换为双晶
#endif//__MODEWIN_H__