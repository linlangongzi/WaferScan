/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ���̽ͷV�Ͳ�������Դ�ļ�
 * Filename: chHeadVCompensateWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"


HWND hWndChHeadVCompensate = NULL;						//���̽ͷV�Ͳ������ھ��
HWND currentChHeadVCompensateBtn = NULL;				//���̽ͷV�Ͳ������ڵ�ǰѡ�еİ�ť
WORD gwHeadVHeadNum = 0;						//̽ͷ�ͺ�
TCHAR strHeadVFileName[MAX_PATH] = _T("\\ResidentFlash\\headvCurve.dat");
BOOL gbHeadVCurveFlag = FALSE;					//̽ͷV�Ͳ��������Ƿ��

static HWND hBtnChHeadVCompensateHeadNum = NULL;				//̽ͷ�ͺ�/�Զ��尴ť���
static HWND hBtnChHeadVCompensateCurveMake = NULL;			//�Զ�������������ť���
static HWND hBtnChHeadVCompensateSwitch = NULL;				//��/�ذ�ť���
static HWND chHeadVCompensateBtnList[3] = {NULL,NULL,NULL};					//���̽ͷV�Ͳ������ڵİ�ť��
static WNDPROC OldBtnChHeadVCompensateHeadNumProc = NULL;	//̽ͷ�ͺ�/�Զ��尴ťĬ�ϴ������
static WNDPROC OldBtnChHeadVCompensateCurveMakeProc = NULL;		//�Զ�������������ťĬ�ϴ������
static WNDPROC OldBtnChHeadVCompensateSwitchProc = NULL;			//��/�ذ�ťĬ�ϴ������

static const int buttonNum = 3;		//��ť�ؼ���

ATOM registerChHeadVCompensateChild(HINSTANCE hInstance);
static LRESULT CALLBACK chHeadVCompensateChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chHeadVCompensate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChHeadVCompensateHeadNumProc( HWND Wnd, 
													UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChHeadVCompensateCurveMakeProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChHeadVCompensateSwitchProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//����ʵ�����
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndChAdjust;
extern HWND currentChAdjustBtn;
extern HWND hWndHeadVMake;//̽ͷV�Ͳ��������������ھ�
extern HWND gCurrentWnd;
extern TCHAR FileDirectory[MAX_PATH];
extern WORD gwHeadVPointNum[HEAD_NUM + 10];									//̽ͷV�Ͳ������ߵ�ĸ���
extern HEADVPOINT_DATA gstHeadVPointData[HEAD_NUM + 10][HEADV_POINT_MAX];		//�ɼ���̽ͷV�Ͳ������ߵ�
extern HWND hStatusBar;
extern BOOL saveHeadVFile(LPCTSTR lpszPathName);

/**************************
*��ȡ̽ͷV�Ͳ������ߵ��ļ�
* @param		hWnd[in]:���ھ��
* @param		wNum[in]:���
* @return   LRESULT :
* @since    1.00
***************************/
/*//�ֳɶ���ļ�
BOOL readHeadVFile(LPCTSTR lpszPathName)
{
	WORD wHeadVPointNum = 0;
	HEADVPOINT_DATA stHeadVpointData[HEADV_POINT_MAX];				

	HANDLE hFileWrite;	//�ļ����
	DWORD dwWriteDataSize;
	hFileWrite = CreateFile(lpszPathName,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��HEADV�ļ�ʧ��"));
		return FALSE;
	}

	if(!ReadFile(hFileWrite,&wHeadVPointNum, sizeof(WORD),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��HEADV�ļ�ʧ��"));
		
		CloseHandle(hFileWrite);
		return FALSE;
	}
	if(!ReadFile(hFileWrite,stHeadVpointData, sizeof(HEADVPOINT_DATA) * HEADV_POINT_MAX,&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��HEADV�ļ�ʧ��"));
		
		CloseHandle(hFileWrite);
		return FALSE;
	}

	CloseHandle(hFileWrite);

	gwHeadVPointNum[gwHeadVHeadNum] = wHeadVPointNum;
	memcpy(gstHeadVPointData[gwHeadVHeadNum], stHeadVpointData, sizeof(HEADVPOINT_DATA)*HEADV_POINT_MAX);
}
*/

BOOL readHeadVFile(LPCTSTR lpszPathName)
{
	WORD wHeadVPointNum = 0;
//	HEADVPOINT_DATA stHeadVpointData[HEADV_POINT_MAX];				

	HANDLE hFileWrite;	//�ļ����
	DWORD dwWriteDataSize;
	hFileWrite = CreateFile(lpszPathName,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_READONLY,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��HEADV�ļ�ʧ��"));
		return FALSE;
	}
	for(int i = 0; i < HEAD_NUM + 10; i++)
	{
		if(!ReadFile(hFileWrite,&gwHeadVPointNum[i], sizeof(WORD),&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��HEADV�ļ�ʧ��"));
			
			CloseHandle(hFileWrite);
			return FALSE;
		}
	}
	for(int i = 0; i < HEAD_NUM + 10; i++)
	{
		if(!ReadFile(hFileWrite,gstHeadVPointData[i], sizeof(HEADVPOINT_DATA) * HEADV_POINT_MAX,&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��HEADV�ļ�ʧ��"));
			
			CloseHandle(hFileWrite);
			return FALSE;
		}
	}

	CloseHandle(hFileWrite);

	return TRUE;
}
void getHeadVData()
{
	readHeadVFile(strHeadVFileName);
	gwHeadVPointNum[0] = 18;
	for(int i = 0; i < 18; i++)
	{
		gstHeadVPointData[0][i].fActualHouDu= float(i + 3);
	}
	gstHeadVPointData[0][0].fMeasureHouDu= 2.6;
	gstHeadVPointData[0][1].fMeasureHouDu= 3.71;
	gstHeadVPointData[0][2].fMeasureHouDu= 4.77;
	gstHeadVPointData[0][3].fMeasureHouDu= 5.81;
	gstHeadVPointData[0][4].fMeasureHouDu= 6.84;
	gstHeadVPointData[0][5].fMeasureHouDu= 7.86;
	gstHeadVPointData[0][6].fMeasureHouDu= 8.87;
	gstHeadVPointData[0][7].fMeasureHouDu= 9.89;
	gstHeadVPointData[0][8].fMeasureHouDu= 10.9;
	gstHeadVPointData[0][9].fMeasureHouDu= 11.91;
	gstHeadVPointData[0][10].fMeasureHouDu= 12.91;
	gstHeadVPointData[0][11].fMeasureHouDu= 13.92;
	gstHeadVPointData[0][12].fMeasureHouDu= 14.92;
	gstHeadVPointData[0][13].fMeasureHouDu= 15.93;
	gstHeadVPointData[0][14].fMeasureHouDu= 16.93;
	gstHeadVPointData[0][15].fMeasureHouDu= 17.94;
	gstHeadVPointData[0][16].fMeasureHouDu= 18.94;
	gstHeadVPointData[0][17].fMeasureHouDu= 19.94;

	gwHeadVPointNum[10] = 18;
	for(int i = 0; i < 18; i++)
	{
		gstHeadVPointData[10][i].fActualHouDu= float(i + 3);
	}
	gstHeadVPointData[10][0].fMeasureHouDu= 2.6;
	gstHeadVPointData[10][1].fMeasureHouDu= 3.71;
	gstHeadVPointData[10][2].fMeasureHouDu= 4.77;
	gstHeadVPointData[10][3].fMeasureHouDu= 5.81;
	gstHeadVPointData[10][4].fMeasureHouDu= 6.84;
	gstHeadVPointData[10][5].fMeasureHouDu= 7.86;
	gstHeadVPointData[10][6].fMeasureHouDu= 8.87;
	gstHeadVPointData[10][7].fMeasureHouDu= 9.89;
	gstHeadVPointData[10][8].fMeasureHouDu= 10.9;
	gstHeadVPointData[10][9].fMeasureHouDu= 11.91;
	gstHeadVPointData[10][10].fMeasureHouDu= 12.91;
	gstHeadVPointData[10][11].fMeasureHouDu= 13.92;
	gstHeadVPointData[10][12].fMeasureHouDu= 14.92;
	gstHeadVPointData[10][13].fMeasureHouDu= 15.93;
	gstHeadVPointData[10][14].fMeasureHouDu= 16.93;
	gstHeadVPointData[10][15].fMeasureHouDu= 17.94;
	gstHeadVPointData[10][16].fMeasureHouDu= 18.94;
	gstHeadVPointData[10][17].fMeasureHouDu= 19.94;
	
}

/**************************
*��ʾ̽ͷ�ͺ�
* @param		hWnd[in]:���ھ��
* @param		wNum[in]:�ͺ�
* @return   LRESULT :
* @since    1.00
***************************/
/*//�ֳɶ���ļ�
static void displayHeadNum(HWND hWnd ,WORD wNum)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wchar_t strFileName[20];
	memset(strFileName, 0, sizeof(strFileName));

	wsprintf(strFileName, _T("%d.headVCurve"), wNum);

	memcpy(strHeadVFileName, FileDirectory, sizeof(strHeadVFileName));
	StringCchCat(strHeadVFileName, MAX_PATH, strFileName);

	if(GetFileAttributes(strHeadVFileName)  == 0xFFFFFFFF)
	{
		if(wNum + 1 <= 10)
			wsprintf(strText, _T("*%d"), wNum + 1);
		else
			wsprintf(strText, _T("*�Զ���%d"), wNum - 9);
	}
	else
	{
		if(wNum + 1 <= 10)
			wsprintf(strText, _T("%d"),wNum + 1);
		else
			wsprintf(strText, _T("�Զ���%d"), wNum - 9);
	}
	DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);

}*/
static void displayHeadNum(HWND hWnd ,WORD wNum)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	readHeadVFile(strHeadVFileName);

	if(gwHeadVPointNum[wNum]  == 0)
	{
		if(wNum + 1 <= HEAD_NUM)
		{
			if(wNum == 0)
				wsprintf(strText, _T("7P��6"));
			else
				wsprintf(strText, _T("*%d"), wNum + 1);
			DrawLableText(hWnd, strText, 28, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else
		{
			wsprintf(strText, _T("*�Զ���%d"), wNum + 1 - HEAD_NUM);
			
			DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
	}
	else
	{
		if(wNum + 1 <= HEAD_NUM)
		{
			if(wNum == 0)
				wsprintf(strText, _T("7P��6"));
			else
				wsprintf(strText, _T("*%d"), wNum + 1);
			DrawLableText(hWnd, strText, 28, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
		else
		{
			wsprintf(strText, _T("�Զ���%d"), wNum + 1 - HEAD_NUM);
			DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
	}
}

/**************************
*��ʾ����
* @param		hWnd[in]:���ھ��
* @param		wNum[in]:�ͺ�
* @return   LRESULT :
* @since    1.00
***************************/
static void displaySwitch(HWND hWnd ,BOOL bSwitch)
{
	if(bSwitch)
		DrawLableText(hWnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	else
		DrawLableText(hWnd, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);		
}

/**************************
*ע����̽ͷV�Ͳ�������
* @chHeadVCompensate    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChHeadVCompensateChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chHeadVCompensateChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChHeadVCompensateChild;

	return RegisterClass(&wc);
}
/**************************
*���̽ͷV�Ͳ������ڴ������
* @chHeadVCompensate    hInstance[in]:
* @chHeadVCompensate		message[in]:
* @chHeadVCompensate		wParam[in]:
* @chHeadVCompensate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chHeadVCompensateChildProc(HWND hWnd, UINT message, 
											  WPARAM wParam, LPARAM lParam)
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
			getHeadVData();
			saveHeadVFile(strHeadVFileName);
			//readHeadVFile(strHeadVFileName);
			//����ť�ؼ�
			hBtnChHeadVCompensateHeadNum = CreateWindow(_T("BUTTON"),_T("HeadNumChHeadVCompensate"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHHEADVCOMPENSATE_HEADNUM,hInst,NULL);
			hBtnChHeadVCompensateCurveMake = CreateWindow(_T("BUTTON"),_T("curveMakeChHeadVCompensate"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 1*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHHEADVCOMPENSATE_CURVEMAKE,hInst,NULL);
			hBtnChHeadVCompensateSwitch = CreateWindow(_T("BUTTON"),_T("switchChHeadVCompensate"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHHEADVCOMPENSATE_SWITCH,hInst,NULL);
					
			chHeadVCompensateBtnList[0] = hBtnChHeadVCompensateHeadNum;
			chHeadVCompensateBtnList[1] = hBtnChHeadVCompensateCurveMake;
			chHeadVCompensateBtnList[2] = hBtnChHeadVCompensateSwitch;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnChHeadVCompensateHeadNumProc = (WNDPROC) GetWindowLong(hBtnChHeadVCompensateHeadNum,GWL_WNDPROC);
			SetWindowLong(hBtnChHeadVCompensateHeadNum,GWL_WNDPROC,(LONG)btnChHeadVCompensateHeadNumProc);
			
			OldBtnChHeadVCompensateCurveMakeProc = (WNDPROC) GetWindowLong(hBtnChHeadVCompensateCurveMake,GWL_WNDPROC);
			SetWindowLong(hBtnChHeadVCompensateCurveMake,GWL_WNDPROC,(LONG)btnChHeadVCompensateCurveMakeProc);
			
			OldBtnChHeadVCompensateSwitchProc = (WNDPROC) GetWindowLong(hBtnChHeadVCompensateSwitch,GWL_WNDPROC);
			SetWindowLong(hBtnChHeadVCompensateSwitch,GWL_WNDPROC,(LONG)btnChHeadVCompensateSwitchProc);
		
			currentChHeadVCompensateBtn = chHeadVCompensateBtnList[0];//Ĭ�ϵ�ǰѡ��̽ͷ�ͺ�/�Զ��尴ť
			SetFocus(currentChHeadVCompensateBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentChHeadVCompensateBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			chHeadVCompensate_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnChHeadVCompensateHeadNum)
			{
				displayHeadNum(pdis->hwndItem,gwHeadVHeadNum);
			}
			if(pdis->hwndItem == hBtnChHeadVCompensateSwitch)
			{
				displaySwitch(pdis->hwndItem, gbHeadVCurveFlag);
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
*���ز��̽ͷV�Ͳ������ڰ�ťͼƬ
* @chHeadVCompensate    hInstance[in]:
* @chHeadVCompensate		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chHeadVCompensate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chHeadVCompensateBtnBitmap[]=
	{
		{IDC_CHHEADVCOMPENSATE_HEADNUM	,IDB_BITMAP_TANTOUXINGHAO4_NOSEL_R	 ,  IDB_BITMAP_TANTOUXINGHAO4_SELECT_R		},
		{IDC_CHHEADVCOMPENSATE_CURVEMAKE	,IDB_BITMAP_ZIDINGYIQUXIANZHIZUO_NOSEL_R ,  IDB_BITMAP_ZIDINGYIQUXIANZHIZUO_SELECT_R	},
		{IDC_CHHEADVCOMPENSATE_SWITCH	,IDB_BITMAP_KAIQIGUANBI4_NOSEL_R ,  IDB_BITMAP_KAIQIGUANBI4_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chHeadVCompensateBtnBitmap);
	return ReturnVal;
}
/**************************
*̽ͷ�ͺ�/�Զ��尴ť�Զ��崦�����
* @chHeadVCompensate    hInstance[in]:
* @chHeadVCompensate		message[in]:
* @chHeadVCompensate		wParam[in]:
* @chHeadVCompensate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChHeadVCompensateHeadNumProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChAdjust,SW_SHOW);
					SetFocus(currentChAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					
					DestroyWindow(hWndChHeadVCompensate);
					hWndChHeadVCompensate = 0;

					gCurrentFocusBtn = currentChAdjustBtn;

					gCurrentWnd = hWndChAdjust;

					SetRedrawFlag(TRUE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chHeadVCompensateBtnList[i] != currentChHeadVCompensateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChHeadVCompensateBtn = chHeadVCompensateBtnList[i];
					SetFocus(currentChHeadVCompensateBtn);
					gCurrentFocusBtn = currentChHeadVCompensateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chHeadVCompensateBtnList[i] != currentChHeadVCompensateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChHeadVCompensateBtn = chHeadVCompensateBtnList[i];
					SetFocus(currentChHeadVCompensateBtn);
					gCurrentFocusBtn = currentChHeadVCompensateBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gwHeadVHeadNum == 0)
						gwHeadVHeadNum = HEAD_NUM +10 - 1;
					else
						gwHeadVHeadNum--;

					displayHeadNum(Wnd, gwHeadVHeadNum);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gwHeadVHeadNum == HEAD_NUM +10 - 1)
						gwHeadVHeadNum = 0;
					else
						gwHeadVHeadNum++;

					displayHeadNum(Wnd, gwHeadVHeadNum);
				}
				else if(wParam == VK_RETURN)
				{
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
	return CallWindowProc(OldBtnChHeadVCompensateHeadNumProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�Զ�������������ť�Զ��崦�����
* @chHeadVCompensate    hInstance[in]:
* @chHeadVCompensate		message[in]:
* @chHeadVCompensate		wParam[in]:
* @chHeadVCompensate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChHeadVCompensateCurveMakeProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChAdjust,SW_SHOW);
					SetFocus(currentChAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					
					DestroyWindow(hWndChHeadVCompensate);
					hWndChHeadVCompensate = 0;

					gCurrentFocusBtn = currentChAdjustBtn;

					gCurrentWnd = hWndChAdjust;

					SetRedrawFlag(TRUE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chHeadVCompensateBtnList[i] != currentChHeadVCompensateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChHeadVCompensateBtn = chHeadVCompensateBtnList[i];
					SetFocus(currentChHeadVCompensateBtn);
					gCurrentFocusBtn = currentChHeadVCompensateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chHeadVCompensateBtnList[i] != currentChHeadVCompensateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChHeadVCompensateBtn = chHeadVCompensateBtnList[i];
					SetFocus(currentChHeadVCompensateBtn);
					gCurrentFocusBtn = currentChHeadVCompensateBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndHeadVMake = CreateWindow(szChHeadVMakeChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChHeadVCompensate,SW_HIDE);
				//	SetFocus(hWndHeadVMake);
					ShowWindow(hWndHeadVMake,SW_SHOW);
					UpdateWindow(hWndHeadVMake);

					gCurrentWnd = hWndHeadVMake;
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
	return CallWindowProc(OldBtnChHeadVCompensateCurveMakeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*��/�ذ�ť�Զ��崦�����
* @chHeadVCompensate    hInstance[in]:
* @chHeadVCompensate		message[in]:
* @chHeadVCompensate		wParam[in]:
* @chHeadVCompensate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChHeadVCompensateSwitchProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChAdjust,SW_SHOW);
					SetFocus(currentChAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					
					DestroyWindow(hWndChHeadVCompensate);
					hWndChHeadVCompensate = 0;

					gCurrentFocusBtn = currentChAdjustBtn;

					gCurrentWnd = hWndChAdjust;

					SetRedrawFlag(TRUE);
				}
			
				else if(wParam == VK_UP)
				{
					while(chHeadVCompensateBtnList[i] != currentChHeadVCompensateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChHeadVCompensateBtn = chHeadVCompensateBtnList[i];
					SetFocus(currentChHeadVCompensateBtn);
					gCurrentFocusBtn = currentChHeadVCompensateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chHeadVCompensateBtnList[i] != currentChHeadVCompensateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChHeadVCompensateBtn = chHeadVCompensateBtnList[i];
					SetFocus(currentChHeadVCompensateBtn);
					gCurrentFocusBtn = currentChHeadVCompensateBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					gbHeadVCurveFlag = !gbHeadVCurveFlag;
					if(gbHeadVCurveFlag)
					{
						readHeadVFile(strHeadVFileName);
					}
					displaySwitch(Wnd, gbHeadVCurveFlag);
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
	return CallWindowProc(OldBtnChHeadVCompensateSwitchProc,Wnd,Msg,wParam,lParam);
}
