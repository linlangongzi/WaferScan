/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ���ƽ��ֵ����Դ�ļ�
 * Filename: chAverageWin.cpp
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


HWND hWndChAverage = NULL;									//���ƽ��ֵ���ھ��
HWND currentChAverageBtn = NULL;							//���ƽ��ֵ���ڵ�ǰѡ�еİ�ť
static HWND hBtnChAverageCurrent1 = NULL;					//��ǰֵ1��ť���
static HWND hBtnChAverageCurrent2 = NULL;					//��ǰֵ2��ť���
static HWND hBtnChAverageCurrent3 = NULL;					//��ǰֵ3��ť���
static HWND hBtnChAverageCurrent4 = NULL;					//��ǰֵ4��ť���
static HWND hBtnChAverageCurrent5 = NULL;					//��ǰֵ5��ť���
static HWND hBtnChAverageCurrent6 = NULL;					//��ǰֵ6��ť���

static HWND chAverageBtnList[6]={NULL,NULL,NULL,NULL,NULL,NULL};						//���ƽ��ֵ���ڵİ�ť��
static WNDPROC OldBtnChAverageCurrent1Proc = NULL;	//��ǰֵ1��ťĬ�ϴ������
static WNDPROC OldBtnChAverageCurrent2Proc = NULL;		//��ǰֵ2��ťĬ�ϴ������
static WNDPROC OldBtnChAverageCurrent3Proc = NULL;	//��ǰֵ3��ťĬ�ϴ������
static WNDPROC OldBtnChAverageCurrent4Proc = NULL;		//��ǰֵ4��ťĬ�ϴ������
static WNDPROC OldBtnChAverageCurrent5Proc = NULL;		//��ǰֵ5��ťĬ�ϴ������
static WNDPROC OldBtnChAverageCurrent6Proc = NULL;		//��ǰֵ6��ťĬ�ϴ������

static WORD buttonNum = 6;		//��ť�ؼ���
float fCurrentValue1 = 0;		//��ǰֵ1
float fCurrentValue2 = 0;		//��ǰֵ2
float fCurrentValue3 = 0;		//��ǰֵ3
float fCurrentValue4 = 0;		//��ǰֵ4
float fCurrentValue5 = 0;		//��ǰֵ5
float fCurrentValue6 = 0;		//��ǰֵ6

ATOM registerChAverageChild(HINSTANCE hInstance);
static LRESULT CALLBACK chAverageChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chAverage_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChAverageCurrent1Proc( HWND Wnd, 
													UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChAverageCurrent2Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChAverageCurrent3Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChAverageCurrent4Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChAverageCurrent5Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChAverageCurrent6Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//����ʵ�����

//extern HWND hWndMode;			//��ǰֵ4���ھ��
//extern HWND hWndStandard;		//��ǰֵ3���ھ�� 
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndChDisplay;//��������ʾ���ھ��
extern HWND currentChDisplayBtn;// ��������ʾ���ڵ�ǰѡ�еİ�ť
extern HWND gCurrentWnd;
extern MEASURE_DISPLAY_ALL gstMeasureDisplay;
extern HWND hWndChDisplay;//��������ʾ���ھ��
extern HWND currentChDisplayBtn;// ��������ʾ���ڵ�ǰѡ�еİ�ť

/**************************
*�����м���ƽ��ֵ
* @param    hWnd[in]:���ھ��
* @param    fValue[in]:ֵ
* @return   ATOM :
* @since    1.00
***************************/
WORD getAverageNum()
{
	WORD wNum = 0;
	if(fCurrentValue1 != 0)
	{
		if(fCurrentValue2 != 0)
		{
			if(fCurrentValue3 != 0)
			{
				if(fCurrentValue4 != 0)
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 6;
						}
						else
						{
							wNum = 5;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 5;
						}
						else
						{
							wNum = 4;
						}
					}
				}
				else
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 5;
						}
						else
						{
							wNum = 4;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 4;
						}
						else
						{
							wNum = 3;
						}
					}
				}
			}
			else
			{
				if(fCurrentValue4 != 0)
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 5;
						}
						else
						{
							wNum = 4;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 4;
						}
						else
						{
							wNum = 3;
						}
					}
				}
				else
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 4;
						}
						else
						{
							wNum = 3;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 3;
						}
						else
						{
							wNum = 2;
						}
					}
				}
			}
		}
		else
		{
			if(fCurrentValue3 != 0)
			{
				if(fCurrentValue4 != 0)
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 5;
						}
						else
						{
							wNum = 4;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 4;
						}
						else
						{
							wNum = 3;
						}
					}
				}
				else
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 4;
						}
						else
						{
							wNum = 3;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 3;
						}
						else
						{
							wNum = 2;
						}
					}
				}
			}
			else
			{
				if(fCurrentValue4 != 0)
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 4;
						}
						else
						{
							wNum = 3;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 3;
						}
						else
						{
							wNum = 2;
						}
					}
				}
				else
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 3;
						}
						else
						{
							wNum = 2;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 2;
						}
						else
						{
							wNum = 1;
						}
					}
				}
			}
		}
	}
	else
	{
		if(fCurrentValue2 != 0)
		{
			if(fCurrentValue3 != 0)
			{
				if(fCurrentValue4 != 0)
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 5;
						}
						else
						{
							wNum = 4;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 4;
						}
						else
						{
							wNum = 3;
						}
					}
				}
				else
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 4;
						}
						else
						{
							wNum = 3;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 3;
						}
						else
						{
							wNum = 2;
						}
					}
				}
			}
			else
			{
				if(fCurrentValue4 != 0)
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 4;
						}
						else
						{
							wNum = 3;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 3;
						}
						else
						{
							wNum = 2;
						}
					}
				}
				else
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 3;
						}
						else
						{
							wNum = 2;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 2;
						}
						else
						{
							wNum = 1;
						}
					}
				}
			}
		}
		else
		{
			if(fCurrentValue3 != 0)
			{
				if(fCurrentValue4 != 0)
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 4;
						}
						else
						{
							wNum = 3;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 3;
						}
						else
						{
							wNum = 2;
						}
					}
				}
				else
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 3;
						}
						else
						{
							wNum = 2;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 2;
						}
						else
						{
							wNum = 1;
						}
					}
				}
			}
			else
			{
				if(fCurrentValue4 != 0)
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 3;
						}
						else
						{
							wNum = 2;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 2;
						}
						else
						{
							wNum = 1;
						}
					}
				}
				else
				{
					if(fCurrentValue5 != 0)
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 2;
						}
						else
						{
							wNum = 1;
						}
					}
					else
					{
						if(fCurrentValue6 != 0)
						{
							wNum = 1;
						}
						else
						{
							wNum = 0;
						}
					}
				}
			}
		}
	}
	return wNum;
}
/**************************
*��ʾ��ǰֵ
* @param    hWnd[in]:���ھ��
* @param    fValue[in]:ֵ
* @return   ATOM :
* @since    1.00
***************************/
static void displayCurrentValue(HWND hWnd, float fValue)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(fValue == 0)
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	else
	{
		wsprintf(strText, _T("%.2f"), fValue);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
	}
}

/**************************
*ע����ƽ��ֵ����
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChAverageChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chAverageChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChAverageChild;

	return RegisterClass(&wc);
}
/**************************
*���ƽ��ֵ���ڴ������
* @param    	hWnd[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chAverageChildProc(HWND hWnd, UINT message, 
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
			//����ť�ؼ�
			hBtnChAverageCurrent1 = CreateWindow(_T("BUTTON"),_T("ChAverageCurrent1"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHAVERAGE_CURRENT1,hInst,NULL);
			hBtnChAverageCurrent2 = CreateWindow(_T("BUTTON"),_T("ChAverageCurrent2"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHAVERAGE_CURRENT2,hInst,NULL);
			hBtnChAverageCurrent3 = CreateWindow(_T("BUTTON"),_T("ChAverageCurrent3"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHAVERAGE_CURRENT3,hInst,NULL);
			hBtnChAverageCurrent4 = CreateWindow(_T("BUTTON"),_T("ChAverageCurrent4"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHAVERAGE_CURRENT4,hInst,NULL);
			hBtnChAverageCurrent5 = CreateWindow(_T("BUTTON"),_T("ChAverageCurrent5"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHAVERAGE_CURRENT5,hInst,NULL);
			hBtnChAverageCurrent6 = CreateWindow(_T("BUTTON"),_T("ChAverageCurrent6"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 5*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 5*BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_CHAVERAGE_CURRENT6,hInst,NULL);

			chAverageBtnList[0] = hBtnChAverageCurrent1;
			chAverageBtnList[1] = hBtnChAverageCurrent2;
			chAverageBtnList[2] = hBtnChAverageCurrent3;
			chAverageBtnList[3] = hBtnChAverageCurrent4;
			chAverageBtnList[4] = hBtnChAverageCurrent5;
			chAverageBtnList[5] = hBtnChAverageCurrent6;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnChAverageCurrent1Proc = (WNDPROC) GetWindowLong(hBtnChAverageCurrent1,GWL_WNDPROC);
			SetWindowLong(hBtnChAverageCurrent1,GWL_WNDPROC,(LONG)btnChAverageCurrent1Proc);
			
			OldBtnChAverageCurrent2Proc = (WNDPROC) GetWindowLong(hBtnChAverageCurrent2,GWL_WNDPROC);
			SetWindowLong(hBtnChAverageCurrent2,GWL_WNDPROC,(LONG)btnChAverageCurrent2Proc);
			
			OldBtnChAverageCurrent3Proc = (WNDPROC) GetWindowLong(hBtnChAverageCurrent3,GWL_WNDPROC);
			SetWindowLong(hBtnChAverageCurrent3,GWL_WNDPROC,(LONG)btnChAverageCurrent3Proc);
			
			OldBtnChAverageCurrent4Proc = (WNDPROC) GetWindowLong(hBtnChAverageCurrent4,GWL_WNDPROC);
			SetWindowLong(hBtnChAverageCurrent4,GWL_WNDPROC,(LONG)btnChAverageCurrent4Proc)
				;
			OldBtnChAverageCurrent5Proc = (WNDPROC) GetWindowLong(hBtnChAverageCurrent5,GWL_WNDPROC);
			SetWindowLong(hBtnChAverageCurrent5,GWL_WNDPROC,(LONG)btnChAverageCurrent5Proc);
			
			OldBtnChAverageCurrent6Proc = (WNDPROC) GetWindowLong(hBtnChAverageCurrent6,GWL_WNDPROC);
			SetWindowLong(hBtnChAverageCurrent6,GWL_WNDPROC,(LONG)btnChAverageCurrent6Proc);
		
			currentChAverageBtn = chAverageBtnList[0];//Ĭ�ϵ�ǰѡ�е�ǰֵ1��ť
			SetFocus(currentChAverageBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentChAverageBtn;
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
			chAverage_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnChAverageCurrent1)
			{
				displayCurrentValue(pdis->hwndItem, fCurrentValue1);
			}
			if(pdis->hwndItem == hBtnChAverageCurrent2)
			{
				displayCurrentValue(pdis->hwndItem, fCurrentValue2);
			}
			if(pdis->hwndItem == hBtnChAverageCurrent3)
			{
				displayCurrentValue(pdis->hwndItem, fCurrentValue3);
			}
			if(pdis->hwndItem == hBtnChAverageCurrent4)
			{
				displayCurrentValue(pdis->hwndItem, fCurrentValue4);
			}
			if(pdis->hwndItem == hBtnChAverageCurrent5)
			{
				displayCurrentValue(pdis->hwndItem, fCurrentValue5);
			}
			if(pdis->hwndItem == hBtnChAverageCurrent6)
			{
				displayCurrentValue(pdis->hwndItem, fCurrentValue6);
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
*���ز��ƽ��ֵ���ڰ�ťͼƬ
* @param    	hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chAverage_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chAverageBtnBitmap[]=
	{
		{IDC_CHAVERAGE_CURRENT1	,IDB_BITMAP_DANGQIANZHI1_NOSEL_R	 ,  IDB_BITMAP_DANGQIANZHI1_SELECT_R		},
		{IDC_CHAVERAGE_CURRENT2	,IDB_BITMAP_DANGQIANZHI2_NOSEL_R	 ,  IDB_BITMAP_DANGQIANZHI2_SELECT_R		},
		{IDC_CHAVERAGE_CURRENT3	,IDB_BITMAP_DANGQIANZHI3_NOSEL_R ,  IDB_BITMAP_DANGQIANZHI3_SELECT_R	},
		{IDC_CHAVERAGE_CURRENT4	,IDB_BITMAP_DANGQIANZHI4_NOSEL_R ,  IDB_BITMAP_DANGQIANZHI4_SELECT_R	},
		{IDC_CHAVERAGE_CURRENT5 ,IDB_BITMAP_DANGQIANZHI5_NOSEL_R ,  IDB_BITMAP_DANGQIANZHI5_SELECT_R	},
		{IDC_CHAVERAGE_CURRENT6 ,IDB_BITMAP_DANGQIANZHI6_NOSEL_R ,  IDB_BITMAP_DANGQIANZHI6_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chAverageBtnBitmap);
	return ReturnVal;
}
/**************************
*��ǰֵ1��ť�Զ��崦�����
* @param    	Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAverageCurrent1Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);
					SetFocus(currentChDisplayBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChAverage);
					currentChAverageBtn = 0;

					gCurrentFocusBtn = currentChDisplayBtn;

					gCurrentWnd = hWndChDisplay;

					WORD wNum = getAverageNum();				
					if(wNum != 0)
					{
						gstMeasureDisplay.stAverage.fValue = (fCurrentValue1 + fCurrentValue2 + fCurrentValue3 + fCurrentValue4 +
																fCurrentValue5 + fCurrentValue6) / wNum;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(chAverageBtnList[i] != currentChAverageBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAverageBtn = chAverageBtnList[i];
					SetFocus(currentChAverageBtn);
					gCurrentFocusBtn = currentChAverageBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAverageBtnList[i] != currentChAverageBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAverageBtn = chAverageBtnList[i];
					SetFocus(currentChAverageBtn);
					gCurrentFocusBtn = currentChAverageBtn;
				}
				else if(wParam == VK_LEFT)
				{
				 	fCurrentValue1 = 0;
					displayCurrentValue(Wnd, fCurrentValue1);
				}
				
				else if(wParam == VK_RIGHT)
				{
				 	fCurrentValue1 = gstMeasureDisplay.stCurrent.fValue;
					displayCurrentValue(Wnd, fCurrentValue1);
				}
				else if(wParam == VK_RETURN)
				{
				 	fCurrentValue1 = gstMeasureDisplay.stCurrent.fValue;
					displayCurrentValue(Wnd, fCurrentValue1);
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
	return CallWindowProc(OldBtnChAverageCurrent1Proc,Wnd,Msg,wParam,lParam);
}
/**************************
*��ǰֵ2��ť�Զ��崦�����
* @param    	Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAverageCurrent2Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);
					SetFocus(currentChDisplayBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChAverage);
					currentChAverageBtn = 0;

					gCurrentFocusBtn = currentChDisplayBtn;

					gCurrentWnd = hWndChDisplay;

					WORD wNum = getAverageNum();				
					if(wNum != 0)
					{
						gstMeasureDisplay.stAverage.fValue = (fCurrentValue1 + fCurrentValue2 + fCurrentValue3 + fCurrentValue4 +
																fCurrentValue5 + fCurrentValue6) / wNum;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(chAverageBtnList[i] != currentChAverageBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAverageBtn = chAverageBtnList[i];
					SetFocus(currentChAverageBtn);
					gCurrentFocusBtn = currentChAverageBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAverageBtnList[i] != currentChAverageBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAverageBtn = chAverageBtnList[i];
					SetFocus(currentChAverageBtn);
					gCurrentFocusBtn = currentChAverageBtn;
				}
				else if(wParam == VK_LEFT)
				{
				 	fCurrentValue2 = 0;
					displayCurrentValue(Wnd, fCurrentValue2);
				}
				
				else if(wParam == VK_RIGHT)
				{
				 	fCurrentValue2 = gstMeasureDisplay.stCurrent.fValue;
					displayCurrentValue(Wnd, fCurrentValue2);
				}
				else if(wParam == VK_RETURN)
				{
					fCurrentValue2 = gstMeasureDisplay.stCurrent.fValue;
					displayCurrentValue(Wnd, fCurrentValue2);
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
	return CallWindowProc(OldBtnChAverageCurrent2Proc,Wnd,Msg,wParam,lParam);
}
/**************************
*��ǰֵ3��ť�Զ��崦�����
* @param    	Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAverageCurrent3Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);
					SetFocus(currentChDisplayBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChAverage);
					currentChAverageBtn = 0;

					gCurrentFocusBtn = currentChDisplayBtn;

					gCurrentWnd = hWndChDisplay;
					
					WORD wNum = getAverageNum();				
					if(wNum != 0)
					{
						gstMeasureDisplay.stAverage.fValue = (fCurrentValue1 + fCurrentValue2 + fCurrentValue3 + fCurrentValue4 +
																fCurrentValue5 + fCurrentValue6) / wNum;
					}
					
				}
			
				else if(wParam == VK_UP)
				{
					while(chAverageBtnList[i] != currentChAverageBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAverageBtn = chAverageBtnList[i];
					SetFocus(currentChAverageBtn);
					gCurrentFocusBtn = currentChAverageBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAverageBtnList[i] != currentChAverageBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAverageBtn = chAverageBtnList[i];
					SetFocus(currentChAverageBtn);
					gCurrentFocusBtn = currentChAverageBtn;
				}
				else if(wParam == VK_LEFT)
				{
				 	fCurrentValue3 = 0;
					displayCurrentValue(Wnd, fCurrentValue3);
				}
				
				else if(wParam == VK_RIGHT)
				{
				 	fCurrentValue3 = gstMeasureDisplay.stCurrent.fValue;
					displayCurrentValue(Wnd, fCurrentValue3);
				}
				else if(wParam == VK_RETURN)
				{
					fCurrentValue3 = gstMeasureDisplay.stCurrent.fValue;
					displayCurrentValue(Wnd, fCurrentValue3);
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
	return CallWindowProc(OldBtnChAverageCurrent3Proc,Wnd,Msg,wParam,lParam);
}
/**************************
*��ǰֵ4��ť�Զ��崦�����
* @param    	Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAverageCurrent4Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);
					SetFocus(currentChDisplayBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChAverage);
					currentChAverageBtn = 0;

					gCurrentFocusBtn = currentChDisplayBtn;

					gCurrentWnd = hWndChDisplay;

					WORD wNum = getAverageNum();				
					if(wNum != 0)
					{
						gstMeasureDisplay.stAverage.fValue = (fCurrentValue1 + fCurrentValue2 + fCurrentValue3 + fCurrentValue4 +
																fCurrentValue5 + fCurrentValue6) / wNum;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(chAverageBtnList[i] != currentChAverageBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAverageBtn = chAverageBtnList[i];
					SetFocus(currentChAverageBtn);
					gCurrentFocusBtn = currentChAverageBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAverageBtnList[i] != currentChAverageBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAverageBtn = chAverageBtnList[i];
					SetFocus(currentChAverageBtn);
					gCurrentFocusBtn = currentChAverageBtn;
				}
				else if(wParam == VK_LEFT)
				{
				 	fCurrentValue4 = 0;
					displayCurrentValue(Wnd, fCurrentValue4);
				}
				
				else if(wParam == VK_RIGHT)
				{
				 	fCurrentValue4 = gstMeasureDisplay.stCurrent.fValue;
					displayCurrentValue(Wnd, fCurrentValue4);
				}
				else if(wParam == VK_RETURN)
				{
					fCurrentValue4 = gstMeasureDisplay.stCurrent.fValue;
					displayCurrentValue(Wnd, fCurrentValue4);
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
	return CallWindowProc(OldBtnChAverageCurrent4Proc,Wnd,Msg,wParam,lParam);
}
/**************************
*��ǰֵ5��ť�Զ��崦�����
* @param    	Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAverageCurrent5Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);
					SetFocus(currentChDisplayBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChAverage);
					currentChAverageBtn = 0;

					gCurrentFocusBtn = currentChDisplayBtn;

					gCurrentWnd = hWndChDisplay;

					WORD wNum = getAverageNum();				
					if(wNum != 0)
					{
						gstMeasureDisplay.stAverage.fValue = (fCurrentValue1 + fCurrentValue2 + fCurrentValue3 + fCurrentValue4 +
																fCurrentValue5 + fCurrentValue6) / wNum;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(chAverageBtnList[i] != currentChAverageBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAverageBtn = chAverageBtnList[i];
					SetFocus(currentChAverageBtn);
					gCurrentFocusBtn = currentChAverageBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAverageBtnList[i] != currentChAverageBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAverageBtn = chAverageBtnList[i];
					SetFocus(currentChAverageBtn);
					gCurrentFocusBtn = currentChAverageBtn;
				}
				else if(wParam == VK_LEFT)
				{
				 	fCurrentValue5 = 0;
					displayCurrentValue(Wnd, fCurrentValue5);
				}
				
				else if(wParam == VK_RIGHT)
				{
				 	fCurrentValue5 = gstMeasureDisplay.stCurrent.fValue;
					displayCurrentValue(Wnd, fCurrentValue5);
				}
				else if(wParam == VK_RETURN)
				{
					fCurrentValue5 = gstMeasureDisplay.stCurrent.fValue;
					displayCurrentValue(Wnd, fCurrentValue5);
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
	return CallWindowProc(OldBtnChAverageCurrent5Proc,Wnd,Msg,wParam,lParam);
}

/**************************
*��ǰֵ6��ť�Զ��崦�����
* @param    	Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChAverageCurrent6Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);
					SetFocus(currentChDisplayBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndChAverage);
					currentChAverageBtn = 0;

					gCurrentFocusBtn = currentChDisplayBtn;

					gCurrentWnd = hWndChDisplay;

					WORD wNum = getAverageNum();				
					if(wNum != 0)
					{
						gstMeasureDisplay.stAverage.fValue = (fCurrentValue1 + fCurrentValue2 + fCurrentValue3 + fCurrentValue4 +
																fCurrentValue5 + fCurrentValue6) / wNum;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(chAverageBtnList[i] != currentChAverageBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChAverageBtn = chAverageBtnList[i];
					SetFocus(currentChAverageBtn);
					gCurrentFocusBtn = currentChAverageBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chAverageBtnList[i] != currentChAverageBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChAverageBtn = chAverageBtnList[i];
					SetFocus(currentChAverageBtn);
					gCurrentFocusBtn = currentChAverageBtn;
				}
				else if(wParam == VK_LEFT)
				{
				 	fCurrentValue6 = 0;
					displayCurrentValue(Wnd, fCurrentValue6);
				}
				
				else if(wParam == VK_RIGHT)
				{
				 	fCurrentValue6 = gstMeasureDisplay.stCurrent.fValue;
					displayCurrentValue(Wnd, fCurrentValue6);
				}
				else if(wParam == VK_RETURN)
				{
					fCurrentValue6 = gstMeasureDisplay.stCurrent.fValue;
					displayCurrentValue(Wnd, fCurrentValue6);
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
	return CallWindowProc(OldBtnChAverageCurrent6Proc,Wnd,Msg,wParam,lParam);
}


