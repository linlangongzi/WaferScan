/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * �ļ�������Դ�ļ�
 * Filename: fileManageWin.cpp
 *
 * Spec: Trace Tag write here. If has not, ScreenCopy this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fileManageWin.h"

#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"

HWND hWndFileManage = NULL;//�ļ������ھ��
HWND hBtnFileManageCraftParam = NULL;//���ղ�����ť���
HWND hBtnFileManageReportForm = NULL;//����ť���
HWND hBtnFileManageScreenCopy = NULL;//������ť���

HWND fileManageBtnList[3] = {NULL,NULL,NULL};//�ļ������ڵİ�ť��
WNDPROC OldBtnFileManageCraftParamProc = NULL;//���ղ�����ťĬ�ϴ������
WNDPROC OldBtnFileManageReportFormProc = NULL;//����ťĬ�ϴ������
WNDPROC OldBtnFileManageScreenCopyProc = NULL;//������ťĬ�ϴ������
HWND currentFileManageBtn = NULL;//�ļ������е�ǰѡ�еİ�ť

static const int buttonNum = 3;//��ť�ؼ���

extern HWND hWndTsMenu;							//̽�˲˵����ھ��
extern HWND hWndChMenu;							//���˵����ھ��
extern HWND currentTsMenuBtn;					//̽�˲˵������е�ǰѡ�еİ�ť
extern HWND currentChMenuBtn;					//���˵������е�ǰѡ�еİ�ť

extern HINSTANCE hInst;//����ʵ�����
extern HWND hWndCraftParam;//���ղ������ھ��
extern HWND hWndReportForm;//�����ھ��
extern HWND hWndScreenCopy;//�������ھ��
extern HWND hWndMain;//������
extern HWND gCurrentFocusBtn;

extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���
/**************************
*ע���ļ�������
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerFileManageChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) fileManageChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szFileManageChild;

	return RegisterClass(&wc);
}
/**************************
*�ļ������ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK fileManageChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			//����ť�ؼ�
			hBtnFileManageCraftParam = CreateWindow(_T("BUTTON"),_T("CraftParam"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_FILEMANAGE_CRAFTPARAM,hInst,NULL);
			hBtnFileManageReportForm = CreateWindow(_T("BUTTON"),_T("ReportForm"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_FILEMANAGE_REPORTFORM,hInst,NULL);
			hBtnFileManageScreenCopy = CreateWindow(_T("BUTTON"),_T("ScreenCopy"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_FILEMANAGE_SCREENCOPY,hInst,NULL);
			
			fileManageBtnList[0] = hBtnFileManageCraftParam;
			fileManageBtnList[1] = hBtnFileManageReportForm;
			fileManageBtnList[2] = hBtnFileManageScreenCopy;
			//��ť�ؼ����Զ��崦�����
			OldBtnFileManageCraftParamProc = (WNDPROC) GetWindowLong(hBtnFileManageCraftParam,GWL_WNDPROC);
			SetWindowLong(hBtnFileManageCraftParam,GWL_WNDPROC,(LONG)btnFileManageCraftParamProc);
			OldBtnFileManageReportFormProc = (WNDPROC) GetWindowLong(hBtnFileManageReportForm,GWL_WNDPROC);
			SetWindowLong(hBtnFileManageReportForm,GWL_WNDPROC,(LONG)btnFileManageReportFormProc);
			OldBtnFileManageScreenCopyProc = (WNDPROC) GetWindowLong(hBtnFileManageScreenCopy,GWL_WNDPROC);
		    SetWindowLong(hBtnFileManageScreenCopy,GWL_WNDPROC,(LONG)btnFileManageScreenCopyProc);

			currentFileManageBtn = fileManageBtnList[0];//Ĭ�ϵ�ǰѡ�й��ղ�����ť
			SetFocus(currentFileManageBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentFileManageBtn;

			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndFileManage, &ps);
			
			EndPaint(hWndFileManage, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				if(hWndTsMenu)
				{						
					ShowWindow(hWndTsMenu,SW_SHOW);
					SetFocus(currentTsMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndFileManage);
					hWndFileManage = 0;

					gCurrentFocusBtn = currentTsMenuBtn;
					
					gCurrentWnd = hWndTsMenu;
				}

				else if(hWndChMenu)
				{
					DestroyWindow(hWndFileManage);
					hWndFileManage = 0;
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
			if(LOWORD(wParam) == IDC_FILEMANAGE_CRAFTPARAM)
			{
				currentFileManageBtn = fileManageBtnList[0]; 
				SetFocus(currentFileManageBtn);
				gCurrentFocusBtn = currentFileManageBtn;
				hWndCraftParam = CreateWindow(szCraftParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
						MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
				ShowWindow(hWndFileManage,SW_HIDE);
			//	SetFocus(hWndCraftParam);
				ShowWindow(hWndCraftParam,SW_SHOW);
				UpdateWindow(hWndCraftParam);
			}
			if(LOWORD(wParam) == IDC_FILEMANAGE_REPORTFORM)
			{
				currentFileManageBtn = fileManageBtnList[0]; 
				SetFocus(currentFileManageBtn);
				gCurrentFocusBtn = currentFileManageBtn;
				hWndReportForm = CreateWindow(szReportFormChild, TEXT(""), WS_CHILD|WS_VISIBLE,
						MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
				ShowWindow(hWndFileManage,SW_HIDE);
			//	SetFocus(hWndReportForm);
				ShowWindow(hWndReportForm,SW_SHOW);
				UpdateWindow(hWndReportForm);
			}
			else if(LOWORD(wParam) == IDC_FILEMANAGE_SCREENCOPY)
			{
				currentFileManageBtn = fileManageBtnList[0]; 
				SetFocus(currentFileManageBtn);
				gCurrentFocusBtn = currentFileManageBtn;
				hWndScreenCopy = CreateWindow(szScreenCopyChild, TEXT(""), WS_CHILD|WS_VISIBLE,
						MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
				ShowWindow(hWndFileManage,SW_HIDE);
			//	SetFocus(hWndScreenCopy);
				ShowWindow(hWndScreenCopy,SW_SHOW);
				UpdateWindow(hWndScreenCopy);
			}
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			fileManage_DrawButtonBitmap(hInst,pdis);
			break;
	//	case WM_CTLCOLORSTATIC:
	//		return (LRESULT)hStaticBrush;
		case WM_CLOSE:		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*�����ļ������ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int fileManage_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_fileManageBtnBitmap[]=
	{
		{IDC_FILEMANAGE_CRAFTPARAM	,IDB_BITMAP_GONGYICANSHU_NOSEL_R	 ,  IDB_BITMAP_GONGYICANSHU_SELECT_R		},
		{IDC_FILEMANAGE_REPORTFORM	,IDB_BITMAP_BAOBIAO_NOSEL_R	 ,  IDB_BITMAP_BAOBIAO_SELECT_R		},
		{IDC_FILEMANAGE_SCREENCOPY	,IDB_BITMAP_KAOPIN_NOSEL_R ,  IDB_BITMAP_KAOPIN_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_fileManageBtnBitmap);
	return ReturnVal;
}
/**************************
*���ղ�����ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFileManageCraftParamProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fileManageBtnList[i] != currentFileManageBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFileManageBtn = fileManageBtnList[i];
					SetFocus(currentFileManageBtn);
					gCurrentFocusBtn = currentFileManageBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fileManageBtnList[i] != currentFileManageBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFileManageBtn = fileManageBtnList[i];
					SetFocus(currentFileManageBtn);
					gCurrentFocusBtn = currentFileManageBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndCraftParam = CreateWindow(szCraftParamChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFileManage,SW_HIDE);
					//SetFocus(hWndCraftParam);
					ShowWindow(hWndCraftParam,SW_SHOW);
					UpdateWindow(hWndCraftParam);

					gCurrentWnd = hWndCraftParam;
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
		default:
			break;
	}
	return CallWindowProc(OldBtnFileManageCraftParamProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFileManageReportFormProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fileManageBtnList[i] != currentFileManageBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFileManageBtn = fileManageBtnList[i];
					SetFocus(currentFileManageBtn);
					gCurrentFocusBtn = currentFileManageBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fileManageBtnList[i] != currentFileManageBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFileManageBtn = fileManageBtnList[i];
					SetFocus(currentFileManageBtn);
					gCurrentFocusBtn = currentFileManageBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndReportForm = CreateWindow(szReportFormChild, TEXT(""), WS_CHILD|WS_VISIBLE,
						MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFileManage,SW_HIDE);
					//SetFocus(hWndReportForm);
					ShowWindow(hWndReportForm,SW_SHOW);
					UpdateWindow(hWndReportForm);

					gCurrentWnd = hWndReportForm;
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
	return CallWindowProc(OldBtnFileManageReportFormProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnFileManageScreenCopyProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fileManageBtnList[i] != currentFileManageBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFileManageBtn = fileManageBtnList[i];
					SetFocus(currentFileManageBtn);
					gCurrentFocusBtn = currentFileManageBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fileManageBtnList[i] != currentFileManageBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFileManageBtn = fileManageBtnList[i];
					SetFocus(currentFileManageBtn);
					gCurrentFocusBtn = currentFileManageBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndScreenCopy = CreateWindow(szScreenCopyChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFileManage,SW_HIDE);
					//SetFocus(hWndScreenCopy);
					ShowWindow(hWndScreenCopy,SW_SHOW);
					UpdateWindow(hWndScreenCopy);

					gCurrentWnd = hWndScreenCopy;
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
	return CallWindowProc(OldBtnFileManageScreenCopyProc,Wnd,Msg,wParam,lParam);
}
