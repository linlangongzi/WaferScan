/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ̽�˲˵�����Դ�ļ�
 * Filename: tsMenuWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "tsMenuWin.h"

#include "resource.h"
#include "interfaceCmdID.h"

#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"

HWND hWndTsMenu;//̽�˲˵����ھ��
static HWND hBtnMode;//ģʽ��ť���
static HWND hBtnParam;//������ť���
static HWND hBtnOtherCfg;//ͨ�����ð�ť���
static HWND hBtnFile;//�ļ����ð�ť���
static HWND hBtnHotkey;//��ݼ���ť���

static WNDPROC OldBtnModeProc = NULL;//ģʽ��ťĬ�ϴ������
static WNDPROC OldBtnParamProc = NULL;//������ťĬ�ϴ������
static WNDPROC OldBtnOtherCfgProc = NULL;//ͨ�����ð�ťĬ�ϴ������
static WNDPROC OldBtnFileProc = NULL;//�ļ����ð�ťĬ�ϴ������
static WNDPROC OldBtnHotkeyProc = NULL;//��ݼ���ťĬ�ϴ������
static HWND tsMenuBtnList[5];//̽�˲˵����ڵİ�ť��
HWND currentTsMenuBtn;//̽�˲˵������е�ǰѡ�еİ�ť

static const int buttonNum = 5;//��ť�ؼ���

extern HWND hWndMode;//ģʽ���ھ��
extern HWND hWndParam;//�������ھ��
extern HINSTANCE hInst;//����ʵ�����
extern HWND hWndMain;//�����ھ��
extern HWND hWndHotKey;//��ݼ����ھ��
extern HWND hWndFileManage;//�ļ������ھ��
extern HWND hWndOtherCfg;//�������ô��ھ��
extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���

extern HWND hWndTlAdjust;	//��У���ھ��
extern HWND currentTlAdjustBtn;	//��У���ڵ�ǰѡ�а�ť

// songchenguang add start 2011-02-09
extern HWND hWndTlMenu;	//��·ר�ò˵����ھ��
extern HWND currentTlMenuBtn;  	// ��·ר�ò˵����ڵ�ǰѡ�еİ�ť

// songchenguang add end 2011-02-09

extern WORD gwDeviceType;	//�豸���� 0:̽����	1:����� 2:��·ר��
extern WORD CurrentChannelNo;// ��ǰͨ����

extern float gfTsOldRange[CHANNEL_NUM];		//����ͨ��̽�˵�����
extern float gfTsOldPlus[CHANNEL_NUM];		//����ͨ��̽�˵Ļ�׼����
extern float gfTsOldRefPlus[CHANNEL_NUM];	//����ͨ��̽�˵Ĳο�����
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];

//extern void setRailRoadParam(WORD wChannelNo, BOOL bCurveChangeFlag);//wangbingfu delete at 2011-5-27
/**************************
*ע����̽�˲˵�����
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerTsMenuChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = (WNDPROC) tsMenuChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szTsMenuChild;

	return RegisterClass(&wc);
}
/**************************
*̽�˲˵����ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK tsMenuChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
	switch (message) 
	{
		case WM_CREATE:
			hBtnMode = CreateWindow(_T("BUTTON"),_T("mode"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TSMENU_MODE,hInst,NULL);
			hBtnParam = CreateWindow(_T("BUTTON"),_T("param"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TSMENU_PARAM,hInst,NULL);
			hBtnOtherCfg = CreateWindow(_T("BUTTON"),_T("othercfg"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TSMENU_OTHERCFG,hInst,NULL);
			hBtnFile = CreateWindow(_T("BUTTON"),_T("fileMange"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TSMENU_FILE,hInst,NULL);
			hBtnHotkey = CreateWindow(_T("BUTTON"),_T("hotkey"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_TSMENU_HOTKEY,hInst,NULL);
			tsMenuBtnList[0] = hBtnMode;
			tsMenuBtnList[1] = hBtnParam;
			tsMenuBtnList[2] = hBtnOtherCfg;
			tsMenuBtnList[3] = hBtnFile;
			tsMenuBtnList[4] = hBtnHotkey;
			//��ť�ؼ����Զ��崦�����
			OldBtnModeProc = (WNDPROC) GetWindowLong(hBtnMode,GWL_WNDPROC);
			SetWindowLong(hBtnMode,GWL_WNDPROC,(LONG)btnModeProc);
			OldBtnParamProc = (WNDPROC) GetWindowLong(hBtnParam,GWL_WNDPROC);
			SetWindowLong(hBtnParam,GWL_WNDPROC,(LONG)btnParamProc);
			OldBtnOtherCfgProc = (WNDPROC) GetWindowLong(hBtnOtherCfg,GWL_WNDPROC);
		    SetWindowLong(hBtnOtherCfg,GWL_WNDPROC,(LONG)btnOtherCfgProc);
			OldBtnFileProc = (WNDPROC) GetWindowLong(hBtnFile,GWL_WNDPROC);
			SetWindowLong(hBtnFile,GWL_WNDPROC,(LONG)btnFileProc);
			OldBtnHotkeyProc = (WNDPROC) GetWindowLong(hBtnHotkey,GWL_WNDPROC);
			SetWindowLong(hBtnHotkey,GWL_WNDPROC,(LONG)btnHotkeyProc);
			
			currentTsMenuBtn = tsMenuBtnList[0];//Ĭ�ϵ�ǰѡ��ģʽ��ť
			SetFocus(currentTsMenuBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentTsMenuBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndTsMenu, &ps);
			// TODO: Add any drawing code here...
		//	RECT rt;
		//	GetClientRect(hWnd, &rt);
		//	DrawText(hdc, szHello, _tcslen(szHello), &rt, DT_CENTER);
			//����ť�ؼ�
			
			
			EndPaint(hWndTsMenu, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				if(!bFreezeUp)
				{
					DestroyWindow(hWndTsMenu);
					hWndTsMenu = 0;
					
					if(gwDeviceType == 2)	//��·ר��
					{
						if(hWndTlAdjust != NULL)
						{
							ShowWindow(hWndTlAdjust,SW_SHOW);
							SetFocus(currentTlAdjustBtn);//�����ڻ�����뽹��
							
							gCurrentFocusBtn = currentTlAdjustBtn;
							gCurrentWnd = hWndTlAdjust;
						}
						else
						{
							ShowWindow(hWndTlMenu,SW_SHOW);
							SetFocus(currentTlMenuBtn);//�����ڻ�����뽹��
							
							gCurrentFocusBtn = currentTlMenuBtn;
							gCurrentWnd = hWndTlMenu;
						}
						/*	//wangbingfu delete at 2011-5-27
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							gfTsOldRange[i] = DisplaySet_ad[i].fRange;
							gfTsOldPlus[i] = ReceiveParam_ad[i].fPlus;
							gfTsOldRefPlus[i] = ReceiveParam_ad[i].fRefPlus;
						}
						setRailRoadParam(CurrentChannelNo, TRUE);//���øñ�׼�ò�λ���̣���׼���棬��������
						*/
					}
					else
					{
						// songchenguang add start 2010-4-26
						SetMenuStateFlag(FALSE);
						// songchenguang add end 2010-4-26
						gCurrentFocusBtn = 0;//û���κΰ�ť�н���

						gCurrentWnd = 0; //û���Ӳ˵�����
						SetFocus(hWndMain);//�����ڻ�����뽹��
					}
				}
			}
			break;
		case WM_DRAWITEM://�ػ���ť
			pdis=(LPDRAWITEMSTRUCT)lParam;
			tsMenu_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_COMMAND:
				if(LOWORD(wParam) == IDC_TSMENU_MODE)
				{
					currentTsMenuBtn = tsMenuBtnList[0]; 
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
					hWndMode = CreateWindow(szModeChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndMode);
					ShowWindow(hWndMode,SW_SHOW);
					UpdateWindow(hWndMode);
				}
				else if(LOWORD(wParam) == IDC_TSMENU_PARAM)
				{
					currentTsMenuBtn = tsMenuBtnList[1]; 
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
					hWndParam = CreateWindow(szParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndParam);
					ShowWindow(hWndParam,SW_SHOW);
					UpdateWindow(hWndParam);
				}
				else if(LOWORD(wParam) == IDC_TSMENU_OTHERCFG)
				{
					currentTsMenuBtn = tsMenuBtnList[2]; 
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
					hWndOtherCfg = CreateWindow(szOtherCfgChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndOtherCfg);
					ShowWindow(hWndOtherCfg,SW_SHOW);
					UpdateWindow(hWndOtherCfg);
				}
				else if(LOWORD(wParam) == IDC_TSMENU_FILE)
				{
					currentTsMenuBtn = tsMenuBtnList[3]; 
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
					hWndFileManage = CreateWindow(szFileManageChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndFileManage);
					ShowWindow(hWndFileManage,SW_SHOW);
					UpdateWindow(hWndFileManage);
				}
				else if(LOWORD(wParam) == IDC_TSMENU_HOTKEY)
				{
					currentTsMenuBtn = tsMenuBtnList[4]; 
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
					hWndHotKey = CreateWindow(szHotKeyChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndHotKey);
					ShowWindow(hWndHotKey,SW_SHOW);
					UpdateWindow(hWndHotKey);
				}
			break;
		case WM_CLOSE:
		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/**************************
*����̽�˲˵����ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int tsMenu_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_smBtnBitmap[]=
	{
		{IDC_TSMENU_MODE	,IDB_BITMAP_MODE_NOSEL_R	 ,  IDB_BITMAP_MODE_SELECT_R		},
		{IDC_TSMENU_PARAM	,IDB_BITMAP_PARAMETER_NOSEL_R	 ,  IDB_BITMAP_PARAMETER_SELECT_R		},
		{IDC_TSMENU_OTHERCFG	,IDB_BITMAP_OTHERCONFIGURE_NOSEL_R ,  IDB_BITMAP_OTHERCONFIGURE_SELECT_R	},
		{IDC_TSMENU_FILE	,IDB_BITMAP_FILE_NOSEL_R	 ,  IDB_BITMAP_FILE_SELECT_R		},
		{IDC_TSMENU_HOTKEY	,IDB_BITMAP_HOTKEY_NOSEL_R ,  IDB_BITMAP_HOTKEY_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_smBtnBitmap);
	return ReturnVal;
}
/**************************
*ģʽ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndMode = CreateWindow(szModeChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndMode);
					ShowWindow(hWndMode,SW_SHOW);
					UpdateWindow(hWndMode);

					gCurrentWnd = hWndMode;
				//	SetWindowPos(hWndMode,hWndTsMenu,0,0,120,450,
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
	return CallWindowProc(OldBtnModeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*������ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnParamProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndParam = CreateWindow(szParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndParam);
					ShowWindow(hWndParam,SW_SHOW);
					UpdateWindow(hWndParam);

					gCurrentWnd = hWndParam;
				//	SetWindowPos(hWndParam,HWND_TOPMOST,0,0,120,450,
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
	return CallWindowProc(OldBtnParamProc,Wnd,Msg,wParam,lParam);
}
/**************************
*ͨ�����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnOtherCfgProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndOtherCfg = CreateWindow(szOtherCfgChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndOtherCfg);
					ShowWindow(hWndOtherCfg,SW_SHOW);
					UpdateWindow(hWndOtherCfg);

					gCurrentWnd = hWndOtherCfg;
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
	return CallWindowProc(OldBtnOtherCfgProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�ļ����ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFileProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndFileManage = CreateWindow(szFileManageChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndFileManage);
					ShowWindow(hWndFileManage,SW_SHOW);
					UpdateWindow(hWndFileManage);

					gCurrentWnd = hWndFileManage;
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
	return CallWindowProc(OldBtnFileProc,Wnd,Msg,wParam,lParam);
}
/**************************
*��ݼ���ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnHotkeyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam ==VK_DOWN)
				{
					while(tsMenuBtnList[i] != currentTsMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentTsMenuBtn = tsMenuBtnList[i];
					SetFocus(currentTsMenuBtn);
					gCurrentFocusBtn = currentTsMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndHotKey = CreateWindow(szHotKeyChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndTsMenu,SW_HIDE);
					//SetFocus(hWndHotKey);
					ShowWindow(hWndHotKey,SW_SHOW);
					UpdateWindow(hWndHotKey);

					gCurrentWnd = hWndHotKey;
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
	return CallWindowProc(OldBtnHotkeyProc,Wnd,Msg,wParam,lParam);
}
