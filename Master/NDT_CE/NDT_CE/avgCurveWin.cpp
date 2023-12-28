/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * AVG���ߴ���Դ�ļ�
 * Filename: curveWin.cpp
 *
 * Spec: Trace Tag write here. If has not, OnOff this line. If have mutil-TraceTag, write one Tag per line
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
#include "craftParamWin.h"


HWND hWndAvgCurve = NULL;//AVG���ߴ��ھ��
HWND hBtnAvgCurveMake = NULL;//������ť���
HWND hBtnAvgCurveOnOff = NULL;//����/�رհ�ť���
HWND hBtnAvgCurveDefectZhi1 = NULL;//��1ֵ��ť���
HWND hBtnAvgCurveDefectZhi2 = NULL;//��2ֵ��ť���
HWND hBtnAvgCurveDefectZhi3 = NULL;//��3ֵ��ť���

HWND avgCurveBtnList[5] ={NULL,NULL,NULL,NULL,NULL};//AVG���ߴ��ڵİ�ť��
WNDPROC OldBtnAvgCurveMakeProc = NULL;//������ťĬ�ϴ������
WNDPROC OldBtnAvgCurveOnOffProc = NULL;//����/�رհ�ťĬ�ϴ������
WNDPROC OldBtnAvgCurveDefectZhi1Proc = NULL;//��1ֵ��ťĬ�ϴ������
WNDPROC OldBtnAvgCurveDefectZhi2Proc = NULL;//��2ֵ��ťĬ�ϴ������
WNDPROC OldBtnAvgCurveDefectZhi3Proc = NULL;//��3ֵ��ťĬ�ϴ������

HWND currentAvgCurveBtn = NULL;				//AVG���ߴ��е�ǰѡ�еİ�ť
BOOL gbAvgCurveFlag[CHANNEL_NUM] = {FALSE, FALSE};		//AVG���߿��ر�־
float gfDefectZhi1[CHANNEL_NUM] = {2, 2};			//����׾���1
float gfDefectZhi2[CHANNEL_NUM] = {0, 0};			//��2
float gfDefectZhi3[CHANNEL_NUM] = {0, 0};			//��3

static const int buttonNum = 5;//��ť�ؼ���
static float fFaiZhiStep = 1;	//��ֵ����
static WORD wFaiZhiStepIndex = 1;	//��ֵ��������
static float fOldSelectedAvgPointData[MAX_CURVE_POINT_NUM] = {0};	//���榵1�ķ�ֵ����Ϣ
static float fOldDefectZhi1 = 0;			//���榵1�׾�

ATOM registerAvgCurveChild(HINSTANCE hInstance);
static LRESULT CALLBACK avgCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int avgCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnAvgCurveMakeProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnAvgCurveOnOffProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnAvgCurveDefectZhi1Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnAvgCurveDefectZhi2Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnAvgCurveDefectZhi3Proc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);


extern HWND hWndCurve;//�������ô��ھ��
extern HWND hWndMakeCurve;//����AVG���ߴ���
extern HWND hWndMain;//������
extern HWND currentCurveBtn;//AVG���ߴ����е�ǰѡ�еİ�ť
extern HINSTANCE hInst;//����ʵ�����

extern ARS200FPGA_CH_DAC_REG *chCurrent_dac_reg;

extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;			//��ǰ��ʾ���Ӵ���
extern WORD CurrentChannelNo;		// ��ǰͨ����
extern HWND hWndMakeAvgCurve;//����AVG���ߴ��ھ��
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG���߿��ر�־
extern BOOL bgAvgChangeFlag;					//AVG����λ���Ƿ�ı��־
extern int gCurrentSysColorIndex;
extern SYS_COLOR_INFO gstSysColor[SYS_COLOR_NUM];
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//��ѡ�е�AVG������
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern CRITICAL_SECTION csSysInfo;
extern SYS_INFO gSysInfo;
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;
extern BOOL gbGuanLian;
extern BOOL gbHotDB;			// ��ݼ�����
extern BOOL gbHotRange;			// ��ݼ�����

/**************************
*��ʾ����
* @param		hWnd[in]:���ھ��
* @param		bSwitch[in]:����
* @return   LRESULT :
* @since    1.00
***************************/
static void displaySwitch(HWND hWnd ,BOOL bSwitch)
{
	if(bSwitch)
		DrawLableText(hWnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
	else
		DrawLableText(hWnd, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);		
}

/**************************
*��ʾ��ֵ
* @param		hWnd[in]:���ھ��
* @param		fWaiJing[in]:�⾶ֵ
* @return   LRESULT :
* @since    1.00
***************************/
static void displayDefectZhi(HWND hWnd ,float fDefectZhi)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fDefectZhi);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
}

static void UpdateAvgMenuInfo(const WORD& chNo)
{
	displaySwitch(hBtnAvgCurveOnOff, gbAvgCurveFlag[chNo]);
	displayDefectZhi(hBtnAvgCurveDefectZhi1, gfDefectZhi1[chNo]);
	displayDefectZhi(hBtnAvgCurveDefectZhi2, gfDefectZhi2[chNo]);
	displayDefectZhi(hBtnAvgCurveDefectZhi3, gfDefectZhi3[chNo]);
}
/**************************
*ע��AVG���ߴ���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerAvgCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));

	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) avgCurveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szAvgCurveChild;

	return RegisterClass(&wc);
}

/**************************
*AVG���ߴ��ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK avgCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:				
			//����ť�ؼ�
			hBtnAvgCurveMake = CreateWindow(_T("BUTTON"),_T("Make"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 ,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_AVGCURVE_MAKE,hInst,NULL);
			hBtnAvgCurveOnOff = CreateWindow(_T("BUTTON"),_T("OnOff"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_AVGCURVE_SWITCH,hInst,NULL);
			hBtnAvgCurveDefectZhi1 = CreateWindow(_T("BUTTON"),_T("DefectZhi1"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_AVGCURVE_DEFECTZHI1,hInst,NULL);
			hBtnAvgCurveDefectZhi2 = CreateWindow(_T("BUTTON"),_T("DefectZhi2"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_AVGCURVE_DEFECTZHI2,hInst,NULL);
			hBtnAvgCurveDefectZhi3 = CreateWindow(_T("BUTTON"),_T("DefectZhi3"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_AVGCURVE_DEFECTZHI3,hInst,NULL);
			
			avgCurveBtnList[0] = hBtnAvgCurveMake;
			avgCurveBtnList[1] = hBtnAvgCurveOnOff;
			avgCurveBtnList[2] = hBtnAvgCurveDefectZhi1;
			avgCurveBtnList[3] = hBtnAvgCurveDefectZhi2;
			avgCurveBtnList[4] = hBtnAvgCurveDefectZhi3;
			//��ť�ؼ����Զ��崦�����
			OldBtnAvgCurveMakeProc = (WNDPROC) GetWindowLong(hBtnAvgCurveMake,GWL_WNDPROC);
			SetWindowLong(hBtnAvgCurveMake,GWL_WNDPROC,(LONG)btnAvgCurveMakeProc);
			OldBtnAvgCurveOnOffProc = (WNDPROC) GetWindowLong(hBtnAvgCurveOnOff,GWL_WNDPROC);
		    SetWindowLong(hBtnAvgCurveOnOff,GWL_WNDPROC,(LONG)btnAvgCurveOnOffProc);
			OldBtnAvgCurveDefectZhi1Proc = (WNDPROC) GetWindowLong(hBtnAvgCurveDefectZhi1,GWL_WNDPROC);
		    SetWindowLong(hBtnAvgCurveDefectZhi1,GWL_WNDPROC,(LONG)btnAvgCurveDefectZhi1Proc);
			OldBtnAvgCurveDefectZhi2Proc = (WNDPROC) GetWindowLong(hBtnAvgCurveDefectZhi2,GWL_WNDPROC);
		    SetWindowLong(hBtnAvgCurveDefectZhi2,GWL_WNDPROC,(LONG)btnAvgCurveDefectZhi2Proc);
			OldBtnAvgCurveDefectZhi3Proc = (WNDPROC) GetWindowLong(hBtnAvgCurveDefectZhi3,GWL_WNDPROC);
		    SetWindowLong(hBtnAvgCurveDefectZhi3,GWL_WNDPROC,(LONG)btnAvgCurveDefectZhi3Proc);
			
			
			currentAvgCurveBtn = avgCurveBtnList[0];  //Ĭ�ϵ�ǰѡ��������ť
			SetFocus(currentAvgCurveBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentAvgCurveBtn;
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
			avgCurve_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnAvgCurveOnOff)
			{
				displaySwitch(pdis->hwndItem, gbAvgCurveFlag[CurrentChannelNo]);
			}
			else if(pdis->hwndItem == hBtnAvgCurveDefectZhi1)
			{
				displayDefectZhi(pdis->hwndItem, gfDefectZhi1[CurrentChannelNo]);
				//���榵1�ķ�ֵ����Ϣ
				for(int i = 0; i < gSelectedAvgPointDataNum[CurrentChannelNo]; i++)
				{
					fOldSelectedAvgPointData[i] = gSelectedAvgPointData[CurrentChannelNo][0][i].data;
				}
				//���榵1�׾�
				 fOldDefectZhi1 = gfDefectZhi1[CurrentChannelNo];
			}
			else if(pdis->hwndItem == hBtnAvgCurveDefectZhi2)
			{
				displayDefectZhi(pdis->hwndItem, gfDefectZhi2[CurrentChannelNo]);
			}
			else if(pdis->hwndItem == hBtnAvgCurveDefectZhi3)
			{
				displayDefectZhi(pdis->hwndItem, gfDefectZhi3[CurrentChannelNo]);
			}
			else
			{
				;//
			}

			// ��ⰴť�������ʾ��Ӧ�Ĳ���
			memset(gszTextStep, 0, sizeof(gszTextStep));

			if(pdis->hwndItem == currentAvgCurveBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnAvgCurveDefectZhi1 || pdis->hwndItem == hBtnAvgCurveDefectZhi2 || pdis->hwndItem == hBtnAvgCurveDefectZhi3)
						swprintf(gszTextStep, _T("%.1f"), fFaiZhiStep);
					else
						swprintf(gszTextStep, _T(""));

					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}
			break;
		case WM_CHAR:
			InitializeCriticalSection(&csSysInfo);
			EnterCriticalSection(&csSysInfo);
			if( wParam == '\t' && gSysInfo.wWorkMode != OneSendOneReceive)
			{
				// ���°�ť������
				if(CurrentChannelNo == 0)
				{
					UpdateAvgMenuInfo(1);
				}
				else
				{
					UpdateAvgMenuInfo(0);
				}
			}

			LeaveCriticalSection(&csSysInfo);
			DeleteCriticalSection(&csSysInfo);

			SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			break;
		case WM_CLOSE:		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*����AVG���ߴ��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int avgCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_avgCurveBtnBitmap[]=
	{
		{IDC_AVGCURVE_MAKE	,IDB_BITMAP_ZHIZUO_NOSEL_R	 ,  IDB_BITMAP_ZHIZUO_SELECT_R		},
		{IDC_AVGCURVE_SWITCH	,IDB_BITMAP_KAIQIGUANBI5_NOSEL_R ,  IDB_BITMAP_KAIQIGUANBI5_SELECT_R	},
		{IDC_AVGCURVE_DEFECTZHI1	,IDB_BITMAP_FAI1_NOSEL_R ,  IDB_BITMAP_FAI1_SELECT_R	},
		{IDC_AVGCURVE_DEFECTZHI2	,IDB_BITMAP_FAI2_NOSEL_R ,  IDB_BITMAP_FAI2_SELECT_R	},
		{IDC_AVGCURVE_DEFECTZHI3	,IDB_BITMAP_FAI3_NOSEL_R ,  IDB_BITMAP_FAI3_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_avgCurveBtnBitmap);
	return ReturnVal;
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
static LRESULT CALLBACK btnAvgCurveMakeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndCurve,SW_SHOW);
					SetFocus(currentCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndAvgCurve);
					hWndAvgCurve = 0;
					
					gCurrentFocusBtn = currentCurveBtn;

					gCurrentWnd = hWndCurve;
				}
			
				else if(wParam == VK_UP)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndMakeAvgCurve = CreateWindow(szMakeAvgCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndAvgCurve,SW_HIDE);
					ShowWindow(hWndMakeAvgCurve,SW_SHOW);
					UpdateWindow(hWndMakeAvgCurve);

					gCurrentWnd = hWndMakeAvgCurve;
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAvgCurveMakeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����/�رհ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAvgCurveOnOffProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_PAINT:
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndCurve,SW_SHOW);
					SetFocus(currentCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndAvgCurve);
					hWndAvgCurve = 0;
					
					gCurrentFocusBtn = currentCurveBtn;

					gCurrentWnd = hWndCurve;
				}
			
				else if(wParam == VK_UP)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
				/*	if(!gbGuanLian) //�ǹ���
					{
						gbAvgCurveFlag[CurrentChannelNo] = !gbAvgCurveFlag[CurrentChannelNo];
					}
					else
					{
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							gbAvgCurveFlag[i] = !gbAvgCurveFlag[i];
						}
					}

					displaySwitch(Wnd, gbAvgCurveFlag[CurrentChannelNo]);*/

					if(gbAvgCurveFlag[CurrentChannelNo] == OFF)
					{
						if(!gbGuanLian) //�ǹ���
						{
							if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
							{
								gbAvgCurveFlag[CurrentChannelNo] = ON;
								DrawLableText(Wnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
							}
							else
							{
								if(CurrentChannelNo == 0)
								{
									PostMessage(hStatusBar, SB_SETTEXT, 
										(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("ͨ��1��AVG"));
								}
								else
								{
									PostMessage(hStatusBar, SB_SETTEXT, 
										(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("ͨ��2��AVG"));
								}
							}
						}
						else
						{
							for(int i = 0; i < CHANNEL_NUM; i++)
							{
								if(gSelectedAvgPointDataNum[i] > 1)
								{
									gbAvgCurveFlag[i] = ON;
									DrawLableText(Wnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
								}
								else
								{
									if(i == 0) //ͨ��1������
									{
										PostMessage(hStatusBar, SB_SETTEXT, 
											(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("ͨ��1��AVG"));
									}
									else if(i == 1)	//ͨ��2������
									{
										if(gSelectedAvgPointDataNum[0] <= 1)//ͨ��1Ҳ������
										{
											PostMessage(hStatusBar, SB_SETTEXT, 
												(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("��ͨ������AVG"));
										}
										else//ͨ��1������
										{
											PostMessage(hStatusBar, SB_SETTEXT, 
											(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("ͨ��2��AVG"));
										}
									}
									else
									{
										;//
									}
								}
							}
						}
					}
					else
					{
						if(!gbGuanLian) //�ǹ���
						{
							gbAvgCurveFlag[CurrentChannelNo] = OFF;
						}
						else
						{
							for(int i = 0; i < CHANNEL_NUM; i++)
							{
								gbAvgCurveFlag[i] = OFF;
							}
						}
						DrawLableText(Wnd, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
					}

					// �������õ�Ĭ���ļ�
					SaveParamFile(lpDefaultParamFile);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
				else
				{
					;//
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
	return CallWindowProc(OldBtnAvgCurveOnOffProc,Wnd,Msg,wParam,lParam);
}
/**************************
*��1ֵ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAvgCurveDefectZhi1Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_PAINT:
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndCurve,SW_SHOW);
					SetFocus(currentCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndAvgCurve);
					hWndAvgCurve = 0;
					
					gCurrentFocusBtn = currentCurveBtn;

					gCurrentWnd = hWndCurve;
				}
			
				else if(wParam == VK_UP)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfDefectZhi1[CurrentChannelNo] - fFaiZhiStep >= MIN_FAIZHI + 0.1)
					{
						gfDefectZhi1[CurrentChannelNo] = gfDefectZhi1[CurrentChannelNo] - fFaiZhiStep;

						if(gbAvgCurveFlag[CurrentChannelNo] == ON) //������
						{
							bgAvgChangeFlag = TRUE;//ֹͣ����
							Sleep(30);//�ȴ�ֹͣ����
							/*if(CurrentChannelNo == 0)
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}
							else
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}*/
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
							}
							//������ֵ
							HDC hdc = GetDC(hWndMain);
							WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
							HBRUSH hbr = CreateSolidBrush(wColorBack);
							
							RECT rect;
							GetClipBox(hdc, &rect);
							
							RECT RctWave;// ��������
							RctWave.left = rect.left + SCALE_WIDTH;
							RctWave.right = rect.right - SCALE_WIDTH;
							RctWave.top = rect.top;
							RctWave.bottom = rect.bottom - SCALE_WIDTH;
							
							RECT stTextRect;
							stTextRect.left = RctWave.left + 2;
							stTextRect.right = RctWave.left + 60;
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
								stTextRect.top = stFirstPoint.y - 25;
								stTextRect.bottom = stFirstPoint.y;
							
								FillRect(hdc, &stTextRect, hbr);
							}
							DeleteObject(hbr);
							ReleaseDC(hWndMain,hdc);
						}
						
						if(gbAvgCurveFlag[CurrentChannelNo] == ON)
							bgAvgChangeFlag = FALSE;//AVG������Ҫ�ػ�

						if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
						{
							float fDeltaDB = (float)(40 * log10(gfDefectZhi1[CurrentChannelNo] / fOldDefectZhi1));
							for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
							{
								gSelectedAvgPointData[CurrentChannelNo][0][j].data = curveDataChangeByGain(fOldSelectedAvgPointData[j], fDeltaDB);
							}

							SaveParamFile(lpDefaultParamFile);
						}
					}
				/*	else
					{
						gfDefectZhi1[CurrentChannelNo] = 0;
					}*/

					displayDefectZhi(Wnd, gfDefectZhi1[CurrentChannelNo]);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfDefectZhi1[CurrentChannelNo] + fFaiZhiStep <= MAX_FAIZHI)
					{
						gfDefectZhi1[CurrentChannelNo] = gfDefectZhi1[CurrentChannelNo] + fFaiZhiStep;

						if(gbAvgCurveFlag[CurrentChannelNo] == ON) //������
						{
							bgAvgChangeFlag = TRUE;//ֹͣ����
							Sleep(30);//�ȴ�ֹͣ����
						/*	if(CurrentChannelNo == 0)
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}
							else
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}*/
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
							}
							//������ֵ
							HDC hdc = GetDC(hWndMain);
							WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
							HBRUSH hbr = CreateSolidBrush(wColorBack);
							
							RECT rect;
							GetClipBox(hdc, &rect);
							
							RECT RctWave;// ��������
							RctWave.left = rect.left + SCALE_WIDTH;
							RctWave.right = rect.right - SCALE_WIDTH;
							RctWave.top = rect.top;
							RctWave.bottom = rect.bottom - SCALE_WIDTH;
							
							RECT stTextRect;
							stTextRect.left = RctWave.left + 2;
							stTextRect.right = RctWave.left + 60;
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
								stTextRect.top = stFirstPoint.y - 25;
								stTextRect.bottom = stFirstPoint.y;
							
								FillRect(hdc, &stTextRect, hbr);
							}
							DeleteObject(hbr);
							ReleaseDC(hWndMain,hdc);
						}
						
						if(gbAvgCurveFlag[CurrentChannelNo] == ON)
							bgAvgChangeFlag = FALSE;//AVG������Ҫ�ػ�

						if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
						{
							float fDeltaDB = (float)(40 * log10(gfDefectZhi1[CurrentChannelNo] / fOldDefectZhi1));
							for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
							{
								gSelectedAvgPointData[CurrentChannelNo][0][j].data = curveDataChangeByGain(fOldSelectedAvgPointData[j], fDeltaDB);
							}

							SaveParamFile(lpDefaultParamFile);
						}
					}
					else
					{
						gfDefectZhi1[CurrentChannelNo] = 20;
					}

					displayDefectZhi(Wnd, gfDefectZhi1[CurrentChannelNo]);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
				else if(wParam == 'F')
				{
					if(wFaiZhiStepIndex < 1)
						wFaiZhiStepIndex++;
					else
						wFaiZhiStepIndex = 0;

					fFaiZhiStep = FAIZHISTEPS[wFaiZhiStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fFaiZhiStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAvgCurveDefectZhi1Proc,Wnd,Msg,wParam,lParam);
}
/**************************
*��2ֵ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAvgCurveDefectZhi2Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_PAINT:
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndCurve,SW_SHOW);
					SetFocus(currentCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndAvgCurve);
					hWndAvgCurve = 0;
					
					gCurrentFocusBtn = currentCurveBtn;

					gCurrentWnd = hWndCurve;
				}
			
				else if(wParam == VK_UP)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfDefectZhi2[CurrentChannelNo] - fFaiZhiStep >= MIN_FAIZHI)
					{
						gfDefectZhi2[CurrentChannelNo] = gfDefectZhi2[CurrentChannelNo] - fFaiZhiStep;

						if(gbAvgCurveFlag[CurrentChannelNo] == ON) //������
						{
							bgAvgChangeFlag = TRUE;//ֹͣ����
							Sleep(30);//�ȴ�ֹͣ����
						/*	if(CurrentChannelNo == 0)
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}
							else
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}*/
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
							}
							//������ֵ
							HDC hdc = GetDC(hWndMain);
							WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
							HBRUSH hbr = CreateSolidBrush(wColorBack);
							
							RECT rect;
							GetClipBox(hdc, &rect);
							
							RECT RctWave;// ��������
							RctWave.left = rect.left + SCALE_WIDTH;
							RctWave.right = rect.right - SCALE_WIDTH;
							RctWave.top = rect.top;
							RctWave.bottom = rect.bottom - SCALE_WIDTH;
							
							RECT stTextRect;
							stTextRect.left = RctWave.left + 2;
							stTextRect.right = RctWave.left + 60;
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
								stTextRect.top = stFirstPoint.y - 25;
								stTextRect.bottom = stFirstPoint.y;
							
								FillRect(hdc, &stTextRect, hbr);
							}
							DeleteObject(hbr);
							ReleaseDC(hWndMain,hdc);
						}
						
						if(gbAvgCurveFlag[CurrentChannelNo] == ON)
							bgAvgChangeFlag = FALSE;//AVG������Ҫ�ػ�

						if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
						{
							float fDeltaDB = (float)(40 * log10((float)gfDefectZhi2[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
							for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
							{
								gSelectedAvgPointData[CurrentChannelNo][1][j].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][0][j].data, fDeltaDB);
							}

							SaveParamFile(lpDefaultParamFile);
						}
					}
					else
					{
						gfDefectZhi2[CurrentChannelNo] = 0;
					}

					displayDefectZhi(Wnd, gfDefectZhi2[CurrentChannelNo]);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfDefectZhi2[CurrentChannelNo] + fFaiZhiStep <= MAX_FAIZHI)
					{
						gfDefectZhi2[CurrentChannelNo] = gfDefectZhi2[CurrentChannelNo] + fFaiZhiStep;

						if(gbAvgCurveFlag[CurrentChannelNo] == ON) //������
						{
							bgAvgChangeFlag = TRUE;//ֹͣ����
							Sleep(30);//�ȴ�ֹͣ����
						/*	if(CurrentChannelNo == 0)
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}
							else
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}*/
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
							}
							//������ֵ
							HDC hdc = GetDC(hWndMain);
							WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
							HBRUSH hbr = CreateSolidBrush(wColorBack);
							
							RECT rect;
							GetClipBox(hdc, &rect);
							
							RECT RctWave;// ��������
							RctWave.left = rect.left + SCALE_WIDTH;
							RctWave.right = rect.right - SCALE_WIDTH;
							RctWave.top = rect.top;
							RctWave.bottom = rect.bottom - SCALE_WIDTH;
							
							RECT stTextRect;
							stTextRect.left = RctWave.left + 2;
							stTextRect.right = RctWave.left + 60;
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
								stTextRect.top = stFirstPoint.y - 25;
								stTextRect.bottom = stFirstPoint.y;
							
								FillRect(hdc, &stTextRect, hbr);
							}
							DeleteObject(hbr);
							ReleaseDC(hWndMain,hdc);
						}
						
						if(gbAvgCurveFlag[CurrentChannelNo] == ON)
							bgAvgChangeFlag = FALSE;//AVG������Ҫ�ػ�

						if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
						{
							float fDeltaDB = (float)(40 * log10((float)gfDefectZhi2[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
							for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
							{
								gSelectedAvgPointData[CurrentChannelNo][1][j].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][0][j].data, fDeltaDB);
							}

							SaveParamFile(lpDefaultParamFile);
						}
					}
					else
					{
						gfDefectZhi2[CurrentChannelNo] = 20;
					}

					displayDefectZhi(Wnd, gfDefectZhi2[CurrentChannelNo]);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
				else if(wParam == 'F')
				{
					if(wFaiZhiStepIndex < 1)
						wFaiZhiStepIndex++;
					else
						wFaiZhiStepIndex = 0;

					fFaiZhiStep = FAIZHISTEPS[wFaiZhiStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fFaiZhiStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAvgCurveDefectZhi2Proc,Wnd,Msg,wParam,lParam);
}
/**************************
*��3ֵ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnAvgCurveDefectZhi3Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_PAINT:
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndCurve,SW_SHOW);
					SetFocus(currentCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndAvgCurve);
					hWndAvgCurve = 0;
					
					gCurrentFocusBtn = currentCurveBtn;

					gCurrentWnd = hWndCurve;
				}
			
				else if(wParam == VK_UP)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(avgCurveBtnList[i] != currentAvgCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAvgCurveBtn = avgCurveBtnList[i];
					SetFocus(currentAvgCurveBtn);
					gCurrentFocusBtn = currentAvgCurveBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gfDefectZhi3[CurrentChannelNo] - fFaiZhiStep >= MIN_FAIZHI)
					{
						gfDefectZhi3[CurrentChannelNo] = gfDefectZhi3[CurrentChannelNo] - fFaiZhiStep;

						if(gbAvgCurveFlag[CurrentChannelNo] == ON) //������
						{
							bgAvgChangeFlag = TRUE;//ֹͣ����
							Sleep(30);//�ȴ�ֹͣ����
						/*	if(CurrentChannelNo == 0)
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}
							else
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}*/
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
							}
							//������ֵ
							HDC hdc = GetDC(hWndMain);
							WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
							HBRUSH hbr = CreateSolidBrush(wColorBack);
							
							RECT rect;
							GetClipBox(hdc, &rect);
							
							RECT RctWave;// ��������
							RctWave.left = rect.left + SCALE_WIDTH;
							RctWave.right = rect.right - SCALE_WIDTH;
							RctWave.top = rect.top;
							RctWave.bottom = rect.bottom - SCALE_WIDTH;
							
							RECT stTextRect;
							stTextRect.left = RctWave.left + 2;
							stTextRect.right = RctWave.left + 60;
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
								stTextRect.top = stFirstPoint.y - 25;
								stTextRect.bottom = stFirstPoint.y;
							
								FillRect(hdc, &stTextRect, hbr);
							}
							DeleteObject(hbr);
							ReleaseDC(hWndMain,hdc);
						}
						
						if(gbAvgCurveFlag[CurrentChannelNo] == ON)
							bgAvgChangeFlag = FALSE;//AVG������Ҫ�ػ�

						if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
						{
							float fDeltaDB = (float)(40 * log10((float)gfDefectZhi3[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
							for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
							{
								gSelectedAvgPointData[CurrentChannelNo][2][j].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][0][j].data, fDeltaDB);
							}

							SaveParamFile(lpDefaultParamFile);
						}
					}
					else
					{
						gfDefectZhi3[CurrentChannelNo] = 0;
					}

					displayDefectZhi(Wnd, gfDefectZhi3[CurrentChannelNo]);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfDefectZhi3[CurrentChannelNo] + fFaiZhiStep <= MAX_FAIZHI)
					{
						gfDefectZhi3[CurrentChannelNo] = gfDefectZhi3[CurrentChannelNo] + fFaiZhiStep;

						if(gbAvgCurveFlag[CurrentChannelNo] == ON) //������
						{
							bgAvgChangeFlag = TRUE;//ֹͣ����
							Sleep(30);//�ȴ�ֹͣ����
						/*	if(CurrentChannelNo == 0)
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}
							else
							{
								for(int i = 0; i < AVG_CURVE_NUM; i++)
								{
									drawDashCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
								}
							}*/
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								drawCurve2(gSelectedAvgPointData[CurrentChannelNo][i], gSelectedAvgPointDataNum[CurrentChannelNo], RED_POINT, FALSE);
							}
							//������ֵ
							HDC hdc = GetDC(hWndMain);
							WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
							HBRUSH hbr = CreateSolidBrush(wColorBack);
							
							RECT rect;
							GetClipBox(hdc, &rect);
							
							RECT RctWave;// ��������
							RctWave.left = rect.left + SCALE_WIDTH;
							RctWave.right = rect.right - SCALE_WIDTH;
							RctWave.top = rect.top;
							RctWave.bottom = rect.bottom - SCALE_WIDTH;
							
							RECT stTextRect;
							stTextRect.left = RctWave.left + 2;
							stTextRect.right = RctWave.left + 60;
							for(int i = 0; i < AVG_CURVE_NUM; i++)
							{
								POINT stFirstPoint = dataToPoint1(gSelectedAvgPointData[CurrentChannelNo][i][0], RctWave);
								stTextRect.top = stFirstPoint.y - 25;
								stTextRect.bottom = stFirstPoint.y;
							
								FillRect(hdc, &stTextRect, hbr);
							}
							DeleteObject(hbr);
							ReleaseDC(hWndMain,hdc);
						}
						
						if(gbAvgCurveFlag[CurrentChannelNo] == ON)
							bgAvgChangeFlag = FALSE;//AVG������Ҫ�ػ�

						if(gSelectedAvgPointDataNum[CurrentChannelNo] > 1)
						{
							float fDeltaDB = (float)(40 * log10(gfDefectZhi3[CurrentChannelNo] / gfDefectZhi1[CurrentChannelNo]));
							for(int j = 0; j < gSelectedAvgPointDataNum[CurrentChannelNo]; j++)
							{
								gSelectedAvgPointData[CurrentChannelNo][2][j].data = curveDataChangeByGain(gSelectedAvgPointData[CurrentChannelNo][0][j].data, fDeltaDB);
							}
						}

						SaveParamFile(lpDefaultParamFile);
					}
					else
					{
						gfDefectZhi3[CurrentChannelNo] = 20;
					}

					displayDefectZhi(Wnd, gfDefectZhi3[CurrentChannelNo]);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == '\t')
				{
					HWND hParentWnd = GetParent(Wnd);

					if(hParentWnd != NULL)
						SendMessage(hParentWnd, WM_CHAR, wParam, lParam);
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
				else if(wParam == 'F')
				{
					if(wFaiZhiStepIndex < 1)
						wFaiZhiStepIndex++;
					else
						wFaiZhiStepIndex = 0;

					fFaiZhiStep = FAIZHISTEPS[wFaiZhiStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fFaiZhiStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
				else
				{
					;//
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAvgCurveDefectZhi3Proc,Wnd,Msg,wParam,lParam);
}


