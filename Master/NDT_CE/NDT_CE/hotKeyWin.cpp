/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ��ݼ�����Դ�ļ�
 * Filename: hotKeyWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "hotKeyWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"

HWND hWndHotKey = NULL;//��ݼ����ھ��
HWND hBtnHotKeySelect = NULL;//��ݼ�ѡ��ť���
HWND hBtnHotKeySetting = NULL;//�趨��ť���
HWND hBtnHotKeyDelete = NULL;//ɾ����ť���
HWND hotKeyBtnList[3] ={NULL,NULL,NULL};//��ݼ����ڵİ�ť��
WNDPROC OldBtnHotKeySelectProc = NULL;//��ݼ�ѡ��ťĬ�ϴ������
WNDPROC OldBtnHotKeySettingProc = NULL;//�趨��ťĬ�ϴ������
WNDPROC OldBtnHotKeyDeleteProc = NULL;//һɾ����ťĬ�ϴ������
HWND currentHotKeyBtn = NULL;//ģʽ���е�ǰѡ�еİ�ť

static const int buttonNum = 3;//��ť�ؼ���
static HWND hLableSelect = NULL;//��ݼ�ѡ��ť�еľ�̬�ı���
static HBRUSH hStaticBrush = NULL;
extern HWND hWndTsMenu;							//̽�˲˵����ھ��
extern HWND hWndChMenu;							//���˵����ھ��
extern HWND currentTsMenuBtn;					//̽�˲˵������е�ǰѡ�еİ�ť
extern HWND currentChMenuBtn;					//���˵������е�ǰѡ�еİ�ť

extern HINSTANCE hInst;//����ʵ�����
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���

/**************************
*ע���ݼ�����
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerHotKeyChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) hotKeyChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szHotKeyChild;

	return RegisterClass(&wc);
}
/**************************
*��ݼ����ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK hotKeyChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;
//	LPDRAWITEMSTRUCT pdis;
//	TCHAR szHello[MAX_LOADSTRING];
//	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	switch (message) 
	{
		case WM_CREATE:
			if(hStaticBrush == NULL)	
				hStaticBrush = CreateSolidBrush(BK_COLOR_WHITE);
			//����ť�ؼ�
			hBtnHotKeySelect = CreateWindow(_T("BUTTON"),_T("Select"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_HOTKEY_SELECT,hInst,NULL);
			hBtnHotKeySetting = CreateWindow(_T("BUTTON"),_T("Setting"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_HOTKEY_SETTING,hInst,NULL);
			hBtnHotKeyDelete = CreateWindow(_T("BUTTON"),_T("Delete"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_HOTKEY_DELETE,hInst,NULL);
			
			hotKeyBtnList[0] = hBtnHotKeySelect;
			hotKeyBtnList[1] = hBtnHotKeySetting;
			hotKeyBtnList[2] = hBtnHotKeyDelete;
			//��ť�ؼ����Զ��崦�����
			OldBtnHotKeySelectProc = (WNDPROC) GetWindowLong(hBtnHotKeySelect,GWL_WNDPROC);
			SetWindowLong(hBtnHotKeySelect,GWL_WNDPROC,(LONG)btnHotKeySelectProc);
			OldBtnHotKeySettingProc = (WNDPROC) GetWindowLong(hBtnHotKeySetting,GWL_WNDPROC);
			SetWindowLong(hBtnHotKeySetting,GWL_WNDPROC,(LONG)btnHotKeySettingProc);
			OldBtnHotKeyDeleteProc = (WNDPROC) GetWindowLong(hBtnHotKeyDelete,GWL_WNDPROC);
		    SetWindowLong(hBtnHotKeyDelete,GWL_WNDPROC,(LONG)btnHotKeyDeleteProc);
		    
		//	hLableSelect = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER|WS_BORDER,LABEL_START_X,
		//			LABEL_4_START_Y,LABEL_WIDTH,LABEL_HEIGHT,hBtnHotKeySelect,(HMENU)IDL_HOTKEY_SELECT,hInst,NULL);
			
			currentHotKeyBtn = hotKeyBtnList[0];//Ĭ�ϵ�ǰѡ��ѡ��ť
			SetFocus(currentHotKeyBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentHotKeyBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndHotKey, &ps);
			
			EndPaint(hWndHotKey, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				if(hWndTsMenu)
				{
					ShowWindow(hWndTsMenu,SW_SHOW);
					SetFocus(currentTsMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndHotKey);
					hWndHotKey = 0;
					
					gCurrentFocusBtn = currentTsMenuBtn;

					gCurrentWnd = hWndTsMenu;
				}
				
				else if(hWndChMenu)
				{
					DestroyWindow(hWndHotKey);
					hWndHotKey = 0;
					ShowWindow(hWndChMenu,SW_SHOW);
					SetFocus(currentChMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					gCurrentFocusBtn = currentChMenuBtn;
					
					gCurrentWnd = hWndChMenu;
				}
				else
				{
					;//
				}
			}
			
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			hotKey_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_CLOSE:
		
			break;
		case WM_DESTROY:
			if(hStaticBrush)
			{
				DeleteObject(hStaticBrush);
				hStaticBrush = NULL;
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*���ؿ�ݼ����ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int hotKey_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_hotKeyBtnBitmap[]=
	{
		{IDC_HOTKEY_SELECT	,IDB_BITMAP_KUAIJIEJIANXUANZE_NOSEL_R	 ,  IDB_BITMAP_KUAIJIEJIANXUANZE_SELECT_R		},
		{IDC_HOTKEY_SETTING	,IDB_BITMAP_SHEDING_NOSEL_R	 ,  IDB_BITMAP_SHEDING_SELECT_R		},
		{IDC_HOTKEY_DELETE	,IDB_BITMAP_HOTKEY_SHANCHU_NOSEL_R ,  IDB_BITMAP_HOTKEY_SHANCHU_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_hotKeyBtnBitmap);
	return ReturnVal;
}
/**************************
*��ݼ�ѡ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHotKeySelectProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(hotKeyBtnList[i] != currentHotKeyBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentHotKeyBtn = hotKeyBtnList[i];
					SetFocus(currentHotKeyBtn);
					gCurrentFocusBtn = currentHotKeyBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hotKeyBtnList[i] != currentHotKeyBtn)
					{
						i++;
					}
					i++;
					if(i > 2)
						i = 0;
					currentHotKeyBtn = hotKeyBtnList[i];
					SetFocus(currentHotKeyBtn);
					gCurrentFocusBtn = currentHotKeyBtn;
				}
				else if(wParam == VK_LEFT)
				{
				}
				else if(wParam == VK_RIGHT)
				{
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == 'D')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		case WM_CTLCOLORSTATIC:
			SetBkMode((HDC)wParam,TRANSPARENT);
			SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
			return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnHotKeySelectProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�趨��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHotKeySettingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(hotKeyBtnList[i] != currentHotKeyBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = 2;
					currentHotKeyBtn = hotKeyBtnList[i];
					SetFocus(currentHotKeyBtn);
					gCurrentFocusBtn = currentHotKeyBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hotKeyBtnList[i] != currentHotKeyBtn)
					{
						i++;
					}
					i++;
					if(i > 2)
						i = 0;
					currentHotKeyBtn = hotKeyBtnList[i];
					SetFocus(currentHotKeyBtn);
					gCurrentFocusBtn = currentHotKeyBtn;
				}
				else if(wParam == VK_RETURN)
				{
				/*	hWndHotKey = CreateWindow(szHotKeyChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 0, 0, 120, 450, hWnd, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					SetFocus(hWndHotKey);
					ShowWindow(hWndHotKey,SW_SHOW);
					UpdateWindow(hWndHotKey);*/
				//	SetWindowPos(hWndHotKey,HWND_TOPMOST,0,0,120,450,
				//	SWP_NOMOVE|SWP_SHOWWINDOW);
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == 'D')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnHotKeySettingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*ɾ����ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHotKeyDeleteProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_KEYDOWN, wParam, lParam);
				}
			
				else if(wParam == VK_UP)
				{
					while(hotKeyBtnList[i] != currentHotKeyBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = 2;
					currentHotKeyBtn = hotKeyBtnList[i];
					SetFocus(currentHotKeyBtn);
					gCurrentFocusBtn = currentHotKeyBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(hotKeyBtnList[i] != currentHotKeyBtn)
					{
						i++;
					}
					i++;
					if(i > 2)
						i = 0;
					currentHotKeyBtn = hotKeyBtnList[i];
					SetFocus(currentHotKeyBtn);
					gCurrentFocusBtn = currentHotKeyBtn;
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == 'D')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'E')
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
				else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnHotKeyDeleteProc,Wnd,Msg,wParam,lParam);
}
