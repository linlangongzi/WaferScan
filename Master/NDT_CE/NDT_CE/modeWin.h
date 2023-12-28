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
void ChangeWorkMode(WORKMODE emWorkMode, BOOL bSaveFlag);	//�л�����ģʽ
void ChangeTo1or2();			//�л�Ϊ��ͨ��
void ChangeTo1plus2();			//�л�Ϊ˫ͨ��
void ChangeTo1to2();			//�л�Ϊһ��һ��
void ChangeToDanJing();			//�л�Ϊ����
void ChangeToShuangJing();		//�л�Ϊ˫��
#endif//__MODEWIN_H__