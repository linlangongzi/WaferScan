/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * �������ô���Դ�ļ�
 * Filename: otherConfigureWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "otherConfigureWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include <creg.hxx>
#include <Storemgr.h>
#include "NDT_CE.h"

HWND hWndOtherCfg = NULL;					//�������ô��ھ��
HWND hBtnOtherCfgCommunication = NULL;		//ͨ�����ð�ť���
HWND hBtnOtherCfgPrint = NULL;				//��ӡ���ð�ť���
HWND hBtnOtherCfgLcd = NULL;				//�������ð�ť���
HWND hBtnOtherCfgVolume = NULL;			//�������ð�ť���
HWND hBtnOtherCfgStoreMode = NULL;			//��ǰ�洢���ʰ�ť���
HWND otherCfgBtnList[5] ={NULL,NULL,NULL,NULL,NULL};			//�������ô��ڵİ�ť��
WNDPROC OldBtnOtherCfgCommunicationProc = NULL;	//ͨ�����ð�ťĬ�ϴ������
WNDPROC OldBtnOtherCfgPrintProc = NULL;			//��ӡ���ð�ťĬ�ϴ������
WNDPROC OldBtnOtherCfgLcdProc = NULL;			//�������ð�ťĬ�ϴ������
WNDPROC OldBtnOtherCfgVolumeProc = NULL;		//�������ð�ťĬ�ϴ������
WNDPROC OldBtnOtherCfgStoreModeProc = NULL;		//��ǰ�洢���ʰ�ťĬ�ϴ������
HWND currentOtherCfgBtn = NULL;						//�������ô��е�ǰѡ�еİ�ť
//static HWND hLablePrint;
//static HWND hLableLcd;
//static HWND hLableVolume;
//static HWND hLableStoreMode;
//static HBRUSH hStaticBrush = NULL;
static const int buttonNum = 5;					//��ť�ؼ���

extern HWND hWndTsMenu;							//̽�˲˵����ھ��
extern HWND hWndChMenu;							//���˵����ھ��

extern HWND currentTsMenuBtn;					//̽�˲˵������е�ǰѡ�еİ�ť
extern HWND currentChMenuBtn;					//���˵������е�ǰѡ�еİ�ť
extern HINSTANCE hInst;							//����ʵ�����

//songchenguang add start 2010-05-14
int nLCDValue = 1;
int gnVolumn = 3;

extern int gCurrentSysColorIndex;
extern BOOL gbInitializeSysColor;
extern CRITICAL_SECTION csSysColor;


WORD gwStoreType = 2;			//�洢����
TCHAR FileDirectory[MAX_PATH] = _T("\\ResidentFlash\\");
TCHAR SDDirectoy[MAX_PATH];
TCHAR USBDirectoy[MAX_PATH];
BOOL gbUSB = FALSE;			// USB�Ƿ��д洢����־
BOOL gbSD = FALSE;			// SD�����Ƿ��д洢����־
extern ARS200FPGA_SYS_REG* sys_reg;
extern HMODULE hRes;
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;

extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���

extern HWND hWndLogo;						//��˾ͼ�괰�ھ��
extern HWND hStatusBar;
extern HWND hWndCraftParam;
extern HWND gCurrentFocusBtn;
extern CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;			// �����̲߳���

//songchenguang add end 2010-05-14
/**************************
*ע���������ô���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerOtherCfgChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) otherCfgChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szOtherCfgChild;

	return RegisterClass(&wc);
}
void CALLBACK checkDeviceProc( HWND hwnd, 
							  UINT uMsg, 
							  UINT idEvent, 
							  DWORD dwTime 
							)
{
	GetSysDrivesInfo();
}

void SetDriveType(HWND hWnd, const WORD& nType)
{
	// ��ע
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	if(nType == 0)	// SD
	{
		wsprintf(strText, _T("SD"));
		StringCchCopy(FileDirectory, MAX_PATH, SDDirectoy);
		StringCchCat(FileDirectory, MAX_PATH, _T("\\"));
	}
	else if(nType == 1) // USB
	{
		wsprintf(strText, _T("USB"));
		StringCchCopy(FileDirectory, MAX_PATH, USBDirectoy);
		StringCchCat(FileDirectory, MAX_PATH, _T("\\"));
	}
	else if(nType == 2) // ����
	{
		wsprintf(strText, _T("����"));
		wsprintf(FileDirectory, _T("\\ResidentFlash\\"));
	}
	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	//SetWindowText(hLableStoreMode, strText);	
}

void GetSysDrivesInfo()
{
	STOREINFO info;
	info.cbSize=sizeof(STOREINFO);
	HANDLE hStore = ::FindFirstStore(&info);

	gbUSB = FALSE;
	gbSD = FALSE;
#ifdef _NDT_TEST
	HANDLE hFileWrite = CreateFile(_T("deviceInfo.txt"),
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
#endif

	if(hStore != INVALID_HANDLE_VALUE)
	{
		do
		{
#ifdef _NDT_TEST
			TCHAR szText[100];
			size_t cbDest = 100 * sizeof(TCHAR);

//			if(info.dwDeviceType & STORAGE_DEVICE_TYPE_SRAM)
//			{

				memset(szText, 0, sizeof(szText));
				StringCchPrintf(szText, cbDest, TEXT("info.dwDeviceType = %u, DeviceName = %s storeName = %s,info.sdi.szProfile = %s\r\n"), 
					info.dwDeviceType, info.szDeviceName, info.szStoreName, info.sdi.szProfile);

				DWORD num = 0;
				WriteFile(hFileWrite, szText, cbDest, &num, NULL);
#endif
				// ����Store�ĵ�һ��Partition
				// ��Store
				HANDLE hStore2 = OpenStore(info.szDeviceName);
				if(hStore2 != INVALID_HANDLE_VALUE)
				{
					PARTINFO PartInfo;
					PartInfo.cbSize = sizeof(PARTINFO);

					HANDLE hPartition = FindFirstPartition(hStore2, &PartInfo);
					

					if(hPartition != INVALID_HANDLE_VALUE)
					{
						do		//wangbingfu modify at 2010-11-18
						{
							if(lstrcmp(PartInfo.szVolumeName, _T("\\Ӳ��"))==0)
							{
								gbUSB = TRUE;

								StringCchCopy(USBDirectoy, MAX_PATH, PartInfo.szVolumeName );
							}
							if(lstrcmp(PartInfo.szVolumeName, _T("\\Storage Card"))==0)
							{
								gbSD = TRUE;
								StringCchCopy(SDDirectoy, MAX_PATH, PartInfo.szVolumeName);
							}
#ifdef _NDT_TEST
							memset(szText, 0, sizeof(szText));
							StringCchPrintf(szText, cbDest, TEXT("szVolumeName = %ls \r\n"), 
								PartInfo.szVolumeName);
							WriteFile(hFileWrite, szText, cbDest, &num, NULL);
#endif
						}while(FindNextPartition(hPartition, &PartInfo));
					}

					FindClosePartition(hPartition);	
					
					CloseHandle(hStore2);
				}
//			}

			 
		}while(::FindNextStore(hStore, &info));

		::FindCloseStore(hStore);

		CloseHandle(hStore);
		if(gwStoreType == 0 && (!gbSD))
		{
			gwStoreType = 2;
			
			SetDriveType(hBtnOtherCfgStoreMode, gwStoreType);
		}
		else if(gwStoreType == 1 && (!gbUSB))
		{
			gwStoreType = 2;
			SetDriveType(hBtnOtherCfgStoreMode, gwStoreType);
		}
#ifdef _NDT_TEST
		CloseHandle(hFileWrite);
#endif
	}
}


/**************************
*�������ô��ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK otherCfgChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			{
				SetTimer(hWnd, TIMER_CHECK_DEVICE, 20, checkDeviceProc);
			//	if(hStaticBrush == NULL)	
			//		hStaticBrush = CreateSolidBrush(BK_COLOR_WHITE);
				if(sys_reg->wLedBrightness == 0)
					nLCDValue = 6;
				else
					nLCDValue = sys_reg->wLedBrightness;
				// songchenguang add start 2010-05-17
				memset(FileDirectory, 0, sizeof(FileDirectory));
				memset(SDDirectoy, 0, sizeof(SDDirectoy));
				memset(USBDirectoy, 0, sizeof(USBDirectoy));
				
				// ��ѯĿǰ������״̬
				GetSysDrivesInfo();
				wchar_t szText[20];
				memset(szText, 0, sizeof(szText));
				wsprintf(szText, _T("bSD = %d, bUSB = %d"), gbSD, gbUSB);

//				MessageBox(NULL, szText, _T("test"), MB_OK);

				if((!gbSD)&&(!gbUSB))
				{
					wsprintf(FileDirectory, _T("\\ResidentFlash\\"));
					gwStoreType = 2;
				}
				// songchenguang add end 2010-05-17
				//����ť�ؼ�
				hBtnOtherCfgCommunication = CreateWindow(_T("BUTTON"),_T("Communication"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_OTHERCFG_COMMUNICATION,hInst,NULL);
				hBtnOtherCfgPrint = CreateWindow(_T("BUTTON"),_T("Print"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_OTHERCFG_PRINT,hInst,NULL);
				hBtnOtherCfgLcd = CreateWindow(_T("BUTTON"),_T("Lcd"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_OTHERCFG_LCD,hInst,NULL);
				hBtnOtherCfgVolume = CreateWindow(_T("BUTTON"),_T("Volume"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_OTHERCFG_VOLUME,hInst,NULL);
				hBtnOtherCfgStoreMode = CreateWindow(_T("BUTTON"),_T("StoreMode"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_OTHERCFG_STOREMODE,hInst,NULL);
				otherCfgBtnList[0] = hBtnOtherCfgCommunication;
				otherCfgBtnList[1] = hBtnOtherCfgPrint;
				otherCfgBtnList[2] = hBtnOtherCfgLcd;
				otherCfgBtnList[3] = hBtnOtherCfgVolume;
				otherCfgBtnList[4] = hBtnOtherCfgStoreMode;
				//��ť�ؼ����Զ��崦�����
				OldBtnOtherCfgCommunicationProc = (WNDPROC) GetWindowLong(hBtnOtherCfgCommunication,GWL_WNDPROC);
				SetWindowLong(hBtnOtherCfgCommunication,GWL_WNDPROC,(LONG)btnOtherCfgCommunicationProc);
				OldBtnOtherCfgPrintProc = (WNDPROC) GetWindowLong(hBtnOtherCfgPrint,GWL_WNDPROC);
				SetWindowLong(hBtnOtherCfgPrint,GWL_WNDPROC,(LONG)btnOtherCfgPrintProc);
				OldBtnOtherCfgLcdProc = (WNDPROC) GetWindowLong(hBtnOtherCfgLcd,GWL_WNDPROC);
				SetWindowLong(hBtnOtherCfgLcd,GWL_WNDPROC,(LONG)btnOtherCfgLcdProc);
				OldBtnOtherCfgVolumeProc = (WNDPROC) GetWindowLong(hBtnOtherCfgVolume,GWL_WNDPROC);
				SetWindowLong(hBtnOtherCfgVolume,GWL_WNDPROC,(LONG)btnOtherCfgVolumeProc);
				OldBtnOtherCfgStoreModeProc = (WNDPROC) GetWindowLong(hBtnOtherCfgStoreMode,GWL_WNDPROC);
				SetWindowLong(hBtnOtherCfgStoreMode,GWL_WNDPROC,(LONG)btnOtherCfgStoreModeProc);
				//����̬�ı���
			//	hLablePrint = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER|WS_BORDER,LABEL_START_X,
			//			LABEL_5_START_Y,LABEL_WIDTH,LABEL_HEIGHT,hBtnOtherCfgPrint,(HMENU)IDL_OTHERCFG_PRINT,hInst,NULL);
				//hLableLcd = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER|WS_BORDER,LABEL_START_X,
				//		LABEL_START_Y,LABEL_WIDTH,LABEL_HEIGHT,hBtnOtherCfgLcd,(HMENU)IDL_OTHERCFG_LCD,hInst,NULL);
				//hLableVolume = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER|WS_BORDER,LABEL_START_X,
				//		LABEL_START_Y,LABEL_WIDTH,LABEL_HEIGHT,hBtnOtherCfgVolume,(HMENU)IDL_OTHERCFG_VOLUME,hInst,NULL);
			//	hLableStoreMode = CreateWindow(_T("STATIC"),_T(""),WS_VISIBLE | WS_CHILD|SS_CENTER|WS_BORDER,LABEL_START_X,
			//			LABEL_5_START_Y,LABEL_WIDTH,LABEL_HEIGHT,hBtnOtherCfgStoreMode,(HMENU)IDL_OTHERCFG_STOREMODE,hInst,NULL);

				currentOtherCfgBtn = otherCfgBtnList[0];//Ĭ�ϵ�ǰѡ��ͨѶ���ð�ť
				SetFocus(currentOtherCfgBtn);//ѡ�а�ť������뽹��
				gCurrentFocusBtn = currentOtherCfgBtn;
				break;
			}
		case WM_PAINT:
			hdc = BeginPaint(hWndOtherCfg, &ps);
			
			EndPaint(hWndOtherCfg, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				if(hWndTsMenu)
				{
					ShowWindow(hWndTsMenu,SW_SHOW);
					SetFocus(currentTsMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndOtherCfg);
					hWndOtherCfg = 0;
					
					gCurrentFocusBtn = currentTsMenuBtn;

					gCurrentWnd = hWndTsMenu;
				}

				else if(hWndChMenu)
				{
					DestroyWindow(hWndOtherCfg);
					hWndOtherCfg = 0;
					ShowWindow(hWndChMenu,SW_SHOW);
					SetFocus(currentChMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					gCurrentFocusBtn = currentChMenuBtn;

					gCurrentWnd = hWndChMenu;
				}
				else
				{
					;//
				}
				KillTimer(hWnd, TIMER_CHECK_DEVICE);
			}
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			{
				HBITMAP hBitmap,hBitmapOld;
				BITMAP bm;
				HDC hdcBtn;
				int bitMapId;
				pdis=(LPDRAWITEMSTRUCT)lParam;
				otherCfg_DrawButtonBitmap(hInst,pdis);
				if(pdis->CtlID == IDC_OTHERCFG_LCD)
				{
					if(sys_reg->wLedBrightness == 0)
						bitMapId = IDB_BITMAP_BEIGUANG5_R;
					else
						bitMapId = IDB_BITMAP_BEIGUANG0_R + nLCDValue - 1;
					hBitmap=LoadBitmap(hRes,MAKEINTRESOURCE(bitMapId));
					hdcBtn=CreateCompatibleDC(pdis->hDC);
					hBitmapOld = (HBITMAP)SelectObject(hdcBtn,hBitmap);
					GetObject(hBitmap,sizeof(BITMAP),&bm);
				//	BitBlt(pdis->hDC, LABEL_START_X, LABEL_5_START_Y, LABEL_WIDTH, LABEL_HEIGHT, hdcBtn, 0, 0, SRCCOPY);
					StretchBlt(pdis->hDC,LABEL_START_X,LABEL_5_START_Y + 1,LABEL_WIDTH,LABEL_HEIGHT - 2,
									hdcBtn,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
					SelectObject(pdis->hDC,hBitmapOld);
					DeleteObject(hBitmap);
					DeleteDC(hdcBtn);
				}
				else if(pdis->CtlID == IDC_OTHERCFG_VOLUME)
				{
					
				
					bitMapId = IDB_BITMAP_YINLIANGBIAOJI_R;
					hBitmap=LoadBitmap(hRes,MAKEINTRESOURCE(bitMapId));
					hdcBtn=CreateCompatibleDC(pdis->hDC);
					hBitmapOld = (HBITMAP)SelectObject(hdcBtn,hBitmap);
					GetObject(hBitmap,sizeof(BITMAP),&bm);

					StretchBlt(pdis->hDC,LABEL_START_X + 13*gnVolumn + 2,LABEL_5_START_Y + 1,5,24,
									hdcBtn,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);
					
					SelectObject(pdis->hDC,hBitmapOld);
					DeleteObject(hBitmap);
					DeleteDC(hdcBtn);
				}
				if(pdis->hwndItem == hBtnOtherCfgStoreMode)
					SetDriveType(hBtnOtherCfgStoreMode, gwStoreType);
				else if(pdis->hwndItem == hBtnOtherCfgPrint)
				{
					wchar_t strText[20];
					memset(strText, 0, sizeof(strText));

					EnterCriticalSection(&csSysColor);
					swprintf(strText, _T("%d"), gCurrentSysColorIndex); 
					LeaveCriticalSection(&csSysColor);

					DrawLableText(pdis->hwndItem, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);	
				}
			}
			break;
		case WM_CLOSE:
			
			break;
		case WM_DESTROY:
		//	if(hStaticBrush)
		//	{
		//		DeleteObject(hStaticBrush);
		//		hStaticBrush = NULL;
		//	}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*�����������ô��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int otherCfg_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_otherCfgBtnBitmap[]=
	{
		{IDC_OTHERCFG_COMMUNICATION	,IDB_BITMAP_TONGXUNPEIZHI_NOSEL_R	 ,  IDB_BITMAP_TONGXUNPEIZHI_SELECT_R		},
		{IDC_OTHERCFG_PRINT	,IDB_BITMAP_COLORCHOSE_NOSEL_R	 ,  IDB_BITMAP_COLORCHOSE_SELECT_R		},
		{IDC_OTHERCFG_LCD	,IDB_BITMAP_BEIGUANGSHEZHI_NOSEL_R ,  IDB_BITMAP_BEIGUANGSHEZHI_SELECT_R	},
		{IDC_OTHERCFG_VOLUME	,IDB_BITMAP_YINLIANG_NOSEL_R	 ,  IDB_BITMAP_YINLIANG_SELECT_R		},
		{IDC_OTHERCFG_STOREMODE	,IDB_BITMAP_CUNCHUJIEZHI_NOSEL_R	 ,  IDB_BITMAP_CUNCHUJIEZHI_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_otherCfgBtnBitmap);
	return ReturnVal;
}
int otherCfgRefresh_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_otherCfgBtnBitmap[]=
	{
		{IDC_OTHERCFG_COMMUNICATION	,IDB_BITMAP_TONGXUNPEIZHI_NOSEL_R	 ,  IDB_BITMAP_TONGXUNPEIZHI_NOSEL_R		},
		{IDC_OTHERCFG_PRINT	,IDB_BITMAP_DAYINPEIZHI_NOSEL_R	 ,  IDB_BITMAP_DAYINPEIZHI_SELECT_R		},
		{IDC_OTHERCFG_LCD	,IDB_BITMAP_BEIGUANGSHEZHI_SELECT_R ,  IDB_BITMAP_BEIGUANGSHEZHI_SELECT_R	},
		{IDC_OTHERCFG_VOLUME	,IDB_BITMAP_YINLIANG_SELECT_R	 ,  IDB_BITMAP_YINLIANG_SELECT_R		},
		{IDC_OTHERCFG_STOREMODE	,IDB_BITMAP_CUNCHUJIEZHI_NOSEL_R	 ,  IDB_BITMAP_CUNCHUJIEZHI_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_otherCfgBtnBitmap);
	return ReturnVal;
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
LRESULT CALLBACK btnOtherCfgCommunicationProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			
			break;
		case WM_PAINT:
			{
			}
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
					while(otherCfgBtnList[i] != currentOtherCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentOtherCfgBtn = otherCfgBtnList[i];
					SetFocus(currentOtherCfgBtn);
					gCurrentFocusBtn = currentOtherCfgBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(otherCfgBtnList[i] != currentOtherCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentOtherCfgBtn = otherCfgBtnList[i];
					SetFocus(currentOtherCfgBtn);
					gCurrentFocusBtn = currentOtherCfgBtn;
				}
				else if(wParam == VK_RETURN)
				{
				/*	InitializeCriticalSection(&csCommand);
					EnterCriticalSection(&csCommand);
					//gbEndFlag =TRUE;
					thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;
					LeaveCriticalSection(&csCommand);
					DeleteCriticalSection(&csCommand);
					Sleep(20); //�ȴ��߳���ȫֹͣ
					hWndLogo = CreateWindow(szLogoChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 0, 0, 640, 480, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndOtherCfg,SW_HIDE);
					ShowWindow(hStatusBar,SW_HIDE);
					SetFocus(hWndLogo);
					ShowWindow(hWndLogo,SW_SHOW);
					UpdateWindow(hWndLogo);
				*/
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
	return CallWindowProc(OldBtnOtherCfgCommunicationProc,Wnd,Msg,wParam,lParam);
}
/**************************
*��ӡ���ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnOtherCfgPrintProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(otherCfgBtnList[i] != currentOtherCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentOtherCfgBtn = otherCfgBtnList[i];
					SetFocus(currentOtherCfgBtn);
					gCurrentFocusBtn = currentOtherCfgBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(otherCfgBtnList[i] != currentOtherCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentOtherCfgBtn = otherCfgBtnList[i];
					SetFocus(currentOtherCfgBtn);
					gCurrentFocusBtn = currentOtherCfgBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					wchar_t strText[20];
					memset(strText, 0, sizeof(strText));
					// ������ɫ���
					EnterCriticalSection(&csSysColor);
					gCurrentSysColorIndex--;
					gbInitializeSysColor = TRUE;

					if(gCurrentSysColorIndex < 0)
						gCurrentSysColorIndex = SYS_COLOR_NUM - 1;

					swprintf(strText, _T("%d"), gCurrentSysColorIndex); 
					LeaveCriticalSection(&csSysColor);

					DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);	
					SetRedrawFlag(TRUE);
				}
				else if(wParam == VK_RIGHT)
				{
					wchar_t strText[20];
					memset(strText, 0, sizeof(strText));

					// ������ɫ���
					EnterCriticalSection(&csSysColor);
					gCurrentSysColorIndex++;
					gbInitializeSysColor = TRUE;

					if(gCurrentSysColorIndex >= SYS_COLOR_NUM)
						gCurrentSysColorIndex = 0;

					swprintf(strText, _T("%d"), gCurrentSysColorIndex); 
					LeaveCriticalSection(&csSysColor);

					DrawLableText(Wnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);	
					SetRedrawFlag(TRUE);
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
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnOtherCfgPrintProc,Wnd,Msg,wParam,lParam);
}
/*
void SetLCDValue(const WORD& nLCDValue)
{
	// ���ǩ
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(nLCDValue == 0)
	{
		wsprintf(strText, _T("50%%"));
	}
	else
	{
		wsprintf(strText, _T("%d%%"), (nLCDValue-1)*10);
	}

//	SetWindowText(hLableLcd, strText);

	// ��ֵ
	sys_reg->wLedBrightness = nLCDValue;
}*/

/**************************
*�������ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnOtherCfgLcdProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_PAINT:
			{
				
			}
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
					while(otherCfgBtnList[i] != currentOtherCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentOtherCfgBtn = otherCfgBtnList[i];
					SetFocus(currentOtherCfgBtn);
					gCurrentFocusBtn = currentOtherCfgBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(otherCfgBtnList[i] != currentOtherCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentOtherCfgBtn = otherCfgBtnList[i];
					SetFocus(currentOtherCfgBtn);
					gCurrentFocusBtn = currentOtherCfgBtn;
				}
				else if(wParam == VK_LEFT)
				{
			//		HBITMAP hBitmap,hBitmapOld;
			//		BITMAP bm;
			//		HDC hdc,hdcBtn;
			//		LPDRAWITEMSTRUCT pdis;
			//		DRAWITEMSTRUCT  stDis;
					// songchenguang add start 2010-05-14
					//if(nLCDValue > MIN_LCD)
					//	nLCDValue--;
					//else
					//	nLCDValue = MAX_LCD;
					//SetLCDValue(nLCDValue);
					// songchenguang add end 2010-05-14
					nLCDValue--;
				//	hdc = GetWindowDC(hBtnOtherCfgCommunication);
					if(nLCDValue < MIN_LCD)
						nLCDValue = MIN_LCD;
					else
					{
						sys_reg->wLedBrightness = nLCDValue;
						InvalidateRect(hBtnOtherCfgLcd, NULL, TRUE);
						UpdateWindow(hBtnOtherCfgLcd);
					}
				/*	stDis.CtlID = IDC_OTHERCFG_LCD;
					stDis.itemState = ODS_DISABLED;//ODS_FOCUS;
					stDis.hDC = hdc;
					stDis.hwndItem = hBtnOtherCfgLcd;
					pdis = &stDis;
					//Invalidate(FALSE);
					
					PostMessage(hWndOtherCfg, WM_DRAWITEM,0,(LPARAM)pdis);
					hBitmap=LoadBitmap(hRes,MAKEINTRESOURCE(IDB_BITMAP_ZHAMENXUANZE_NOSEL_R));
					hBitmapOld = (HBITMAP)SelectObject(hdcBtn,hBitmap);
					hdcBtn=CreateCompatibleDC(hdc);
					GetObject(hBitmap,sizeof(BITMAP),&bm);
					BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcBtn, 0, 0, SRCCOPY);
					SelectObject(hdc,hBitmapOld);
					wsprintf(strText, _T("%d"), hdc);
					SetWindowText(hLableStoreMode, strText);
					DeleteObject(hBitmap);
					DeleteDC(hdcBtn);
					ReleaseDC(Wnd,hdc);
					UpdateWindow(hWndOtherCfg);*/
				}
				else if(wParam == VK_RIGHT)
				{
			//		TCHAR strText[20];
			//		memset(strText, 0, sizeof(strText));
			//		HBITMAP hBitmap,hBitmapOld;
			//		BITMAP bm;
			//		HDC hdc,hdcBtn;
			//		LPDRAWITEMSTRUCT pdis;
			//		DRAWITEMSTRUCT stDis;
					// songchenguang add start 2010-05-14
					//if(nLCDValue < MAX_LCD)
					//	nLCDValue--;
					//else
					//	nLCDValue = MIN_LCD;
					//SetLCDValue(nLCDValue);
					// songchenguang add end 2010-05-14
					nLCDValue++;
				//	hdc = GetWindowDC(NULL);
					if(nLCDValue > MAX_LCD)
						nLCDValue = MAX_LCD;
					else
					{	
						sys_reg->wLedBrightness = nLCDValue;
						InvalidateRect(hBtnOtherCfgLcd, NULL, TRUE);
						UpdateWindow(hBtnOtherCfgLcd);
					}
				/*	stDis.CtlID = IDC_OTHERCFG_LCD;
					stDis.itemState = ODS_FOCUS;
					stDis.hDC = hdc;
					stDis.hwndItem = hBtnOtherCfgLcd;
					pdis = &stDis;
				//	PostMessage(hWndOtherCfg, WM_DRAWITEM,0,(LPARAM)pdis);
					hBitmap=LoadBitmap(hRes,MAKEINTRESOURCE(IDB_BITMAP_ZHAMENXUANZE_NOSEL_R));
					hdcBtn=CreateCompatibleDC(hdc);
					GetObject(hBitmap,sizeof(BITMAP),&bm);
				//	BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdcBtn, 0, 0, SRCCOPY);
					StretchBlt(hdc,0, 0, 250,
				250, hdcBtn, 0,0, 250,
				250, SRCCOPY);
					wsprintf(strText, _T("%d"), hdc);
					SetWindowText(hLablePrint, strText);
					DeleteObject(hBitmap);
					DeleteDC(hdcBtn);
					ReleaseDC(hWndMain,hdc);
					UpdateWindow(hWndOtherCfg);*/
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
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnOtherCfgLcdProc,Wnd,Msg,wParam,lParam);
}

void SetVolumn(const WORD& nVolumn)
{
	// �޸�����ֵ
	DWORD dwVolume = 0;
	CReg* pVolumeReg = NULL;

	pVolumeReg = new CReg( HKEY_CURRENT_USER, TEXT("ControlPanel\\Volume") );

	dwVolume = pVolumeReg->ValueDW(TEXT("Volume"));
	// �������ľ�������
	dwVolume = DWORD(double(nVolumn)/MAX_VOLUMN * 0xFFFFFFFF);

	if( waveOutSetVolume(NULL, dwVolume) != MMSYSERR_NOERROR ) // ���������Ƿ�ɹ�
	{
//		DBGMSG(ZONE_1, (TEXT("waveOutSetVolume failed, [MainLayere.cpp, SetVolume]")));
	}
 
	 pVolumeReg->SetDW(TEXT("Volume"),dwVolume); //����ע���
	 delete pVolumeReg;
	 pVolumeReg = NULL; 

	// ��ͼƬ

}

/**************************
*�������ð�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnOtherCfgVolumeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(otherCfgBtnList[i] != currentOtherCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentOtherCfgBtn = otherCfgBtnList[i];
					SetFocus(currentOtherCfgBtn);
					gCurrentFocusBtn = currentOtherCfgBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(otherCfgBtnList[i] != currentOtherCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentOtherCfgBtn = otherCfgBtnList[i];
					SetFocus(currentOtherCfgBtn);
					gCurrentFocusBtn = currentOtherCfgBtn;
				}
				else if(wParam == VK_LEFT)
				{
					gnVolumn --;
					if(gnVolumn < MIN_VOLUMN)
						gnVolumn = MIN_VOLUMN;
					else
					{
						SetVolumn(gnVolumn);
						InvalidateRect(hBtnOtherCfgVolume, NULL, TRUE);
						UpdateWindow(hBtnOtherCfgVolume);
					}	
				}
				else if(wParam == VK_RIGHT)
				{
					gnVolumn ++;
					if(gnVolumn > MAX_VOLUMN)
						gnVolumn = MAX_VOLUMN;
					else
					{
						SetVolumn(gnVolumn);
						InvalidateRect(hBtnOtherCfgVolume, NULL, TRUE);
						UpdateWindow(hBtnOtherCfgVolume);
					}
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
	//	case WM_CTLCOLORSTATIC:
		//	SetBkMode((HDC)wParam,TRANSPARENT);
		//	SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
		//	return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnOtherCfgVolumeProc,Wnd,Msg,wParam,lParam);
}


/**************************
*��ǰ�洢���ʰ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnOtherCfgStoreModeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(otherCfgBtnList[i] != currentOtherCfgBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentOtherCfgBtn = otherCfgBtnList[i];
					SetFocus(currentOtherCfgBtn);
					gCurrentFocusBtn = currentOtherCfgBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(otherCfgBtnList[i] != currentOtherCfgBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentOtherCfgBtn = otherCfgBtnList[i];
					SetFocus(currentOtherCfgBtn);
					gCurrentFocusBtn = currentOtherCfgBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gwStoreType > MIN_TYPE)
						gwStoreType--;
					else
						gwStoreType = MAX_TYPE;
					
					if(gwStoreType==1 && (!gbUSB))
						gwStoreType-- ;
					if(gwStoreType==0 && (!gbSD))
						gwStoreType = MAX_TYPE;

					SetDriveType(Wnd, gwStoreType);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gwStoreType < MAX_TYPE)
						gwStoreType++;
					else
						gwStoreType = MIN_TYPE;

					if(gwStoreType==0 && (!gbSD))
						gwStoreType ++;

					if(gwStoreType==1 && (!gbUSB))
						gwStoreType ++;
					SetDriveType(Wnd, gwStoreType);
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
	//	case WM_CTLCOLORSTATIC:
		//	SetBkMode((HDC)wParam,TRANSPARENT);
		//	SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
		//	return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnOtherCfgStoreModeProc,Wnd,Msg,wParam,lParam);
}

