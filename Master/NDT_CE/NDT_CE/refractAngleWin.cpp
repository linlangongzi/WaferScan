/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ����Ǵ���Դ�ļ�
 * Filename: refractAngleWin.cpp
 *
 * Spec: Trace Tag write here. If has not, ScreenCopy this line. If have mutil-TraceTag, write one Tag per line
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
#include "recvParamWin.h"

HWND hWndRefractAngle = NULL;//����Ǵ��ھ��
HWND hBtnRefractAngleKValue = NULL;//Kֵ��ť���
HWND hBtnRefractAngleJiaoDu = NULL;//�ǶȰ�ť���

HWND refractAngleBtnList[2] = {NULL,NULL};//����Ǵ��ڵİ�ť��
WNDPROC OldBtnRefractAngleKValueProc = NULL;//Kֵ��ťĬ�ϴ������
WNDPROC OldBtnRefractAngleJiaoDuProc = NULL;//�ǶȰ�ťĬ�ϴ������
HWND currentRefractAngleBtn = NULL;//����Ǵ��е�ǰѡ�еİ�ť

ATOM registerRefractAngleChild(HINSTANCE hInstance);
LRESULT CALLBACK refractAngleChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int refractAngle_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnRefractAngleKValueProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnRefractAngleJiaoDuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

float fZheSheAngleStep = 1;
float fKValueStep = 1;
WORD nZheSheAngleStepIndex = 0;
static WORD nKValueStepIndex = 0;
static float fKValue = 0;
static const int buttonNum = 2;//��ť�ؼ���

extern HWND hWndDisplay;						//��ʾ���ھ��
extern HWND currentDisplayBtn;					//��ʾ���е�ǰѡ�еİ�ť
extern HINSTANCE hInst;//����ʵ�����
extern HWND hWndMain;//������
extern HWND gCurrentFocusBtn;

extern BOOL bFreezeUp;

extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern SEND_PARAM_DATA SendParam_ad[2];
extern HWND hStatusBar;
extern WORD CurrentChannelNo;
extern BOOL gbGuanLian;
extern SYS_INFO gSysInfo;
extern CRITICAL_SECTION csSysInfo;
extern wchar_t gszTextStep[10];
extern CRITICAL_SECTION csRange;
extern float fRangeStart[2];
extern float fRangeEnd[2];
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���
extern BOOL gbHotDB;			// ��ݼ�����
extern BOOL gbHotRange;			// ��ݼ�����
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern CRITICAL_SECTION csProbeType;
extern WORD wTanTouLeiXing[CHANNEL_NUM];	//̽ͷ����
extern WORD gwMaterialType;				//��������
extern WORD gwUserDefineWaveSpeed;  		//�Զ�������	

extern WORD waveSpeedToType(WORD wWaveSpeed);
extern WORD typeToWaveSpeed(WORD wType);
extern void SetWaveSpeedLabelAndExecute(HWND hWnd);

/**************************
*ע������Ǵ���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerRefractAngleChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) refractAngleChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szRefractAngleChild;

	return RegisterClass(&wc);
}
void displayKValue(HWND hWnd, float fKValue)
{
	wchar_t strText[20];

	memset(strText, 0, sizeof(strText));
	swprintf(strText, _T("%.1f"), fKValue);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}
void displayAngle(HWND hWnd, float fAngle)
{
	wchar_t strText[20];

	memset(strText, 0, sizeof(strText));
	swprintf(strText, _T("%.1f"), fAngle);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

void SetAngleLabelAndExecute()
{
	if(gbGuanLian)
	{
		DisplaySet_ad[0].fZheSheJiao = DisplaySet_ad[CurrentChannelNo].fZheSheJiao;
		DisplaySet_ad[1].fZheSheJiao = DisplaySet_ad[CurrentChannelNo].fZheSheJiao;
	}

	// ���������ֵ������ֵ��Ӧ���仯...
	float fRange = DisplaySet_ad[CurrentChannelNo].fRange;
	float fAngle = (float)DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * (float)PI;
	InitializeCriticalSection(&csRange);
	EnterCriticalSection(&csRange);

	if(!gbGuanLian)
	{
		if(!DisplaySet_ad[CurrentChannelNo].bSFlag)
		{
			if(sin(fAngle) > 0)
			{
				fRangeStart[CurrentChannelNo] = (float)(fRangeStart[CurrentChannelNo] * sin(fAngle) - SendParam_ad[CurrentChannelNo].fQianYan);
				fRangeEnd[CurrentChannelNo] = (float)((fRangeStart[CurrentChannelNo] + fRange) * sin(fAngle) - SendParam_ad[CurrentChannelNo].fQianYan);
			}
			else
			{
				fRangeStart[CurrentChannelNo] = float(ReceiveParam_ad[CurrentChannelNo].fDelay/2*DisplaySet_ad[CurrentChannelNo].nWaveSpeed*1E-3);
				fRangeEnd[CurrentChannelNo] = fRangeStart[CurrentChannelNo] + DisplaySet_ad[CurrentChannelNo].fRange;
			}
		}
	}
	else
	{
		if(!DisplaySet_ad[CurrentChannelNo].bSFlag)
		{
			if(sin(fAngle) > 0)
			{
				fRangeStart[0] = (float)(fRangeStart[0] * sin(fAngle) - SendParam_ad[0].fQianYan);
				fRangeEnd[0] = (float)((fRangeStart[0] + fRange) * sin(fAngle) - SendParam_ad[0].fQianYan);
			}
			else
			{
				fRangeStart[0] = float(ReceiveParam_ad[0].fDelay/2*DisplaySet_ad[0].nWaveSpeed*1E-3);
				fRangeEnd[0] = fRangeStart[0] + DisplaySet_ad[0].fRange;
			}
		}
		fRangeStart[1] = fRangeStart[0];
		fRangeEnd[1] = fRangeEnd[0];
	}

	LeaveCriticalSection(&csRange);
	DeleteCriticalSection(&csRange);

	// �������õ�Ĭ���ļ�
	SaveParamFile(lpDefaultParamFile);
}

/**************************
*����Ǵ��ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK refractAngleChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			gwMaterialType = waveSpeedToType(DisplaySet_ad[CurrentChannelNo].nWaveSpeed);
			if(gwMaterialType == 1)
				gwUserDefineWaveSpeed = DisplaySet_ad[CurrentChannelNo].nWaveSpeed;
			//����ť�ؼ�
			hBtnRefractAngleKValue = CreateWindow(_T("BUTTON"),_T("KValue"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_REFRACTANGLE_KVALUE,hInst,NULL);
			hBtnRefractAngleJiaoDu = CreateWindow(_T("BUTTON"),_T("JiaoDu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_REFRACTANGLE_JIAODU,hInst,NULL);
			
			refractAngleBtnList[0] = hBtnRefractAngleKValue;
			refractAngleBtnList[1] = hBtnRefractAngleJiaoDu;
			//��ť�ؼ����Զ��崦�����
			OldBtnRefractAngleKValueProc = (WNDPROC) GetWindowLong(hBtnRefractAngleKValue,GWL_WNDPROC);
			SetWindowLong(hBtnRefractAngleKValue,GWL_WNDPROC,(LONG)btnRefractAngleKValueProc);

			OldBtnRefractAngleJiaoDuProc = (WNDPROC) GetWindowLong(hBtnRefractAngleJiaoDu,GWL_WNDPROC);
			SetWindowLong(hBtnRefractAngleJiaoDu,GWL_WNDPROC,(LONG)btnRefractAngleJiaoDuProc);

			currentRefractAngleBtn = refractAngleBtnList[0];//Ĭ�ϵ�ǰѡ��Kֵ��ť
			SetFocus(currentRefractAngleBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentRefractAngleBtn;

			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndRefractAngle, &ps);
			
			EndPaint(hWndRefractAngle, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
			if(LOWORD(wParam) == IDC_REFRACTANGLE_KVALUE)
			{
			/*	currentRefractAngleBtn = refractAngleBtnList[0]; 
				SetFocus(currentRefractAngleBtn);
				gCurrentFocusBtn = currentRefractAngleBtn;
				hWndKValue = CreateWindow(szKValueChild, TEXT(""), WS_CHILD|WS_VISIBLE,
						MENU_KVALUE_X, MENU_KVALUE_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
				ShowWindow(hWndRefractAngle,SW_HIDE);
			//	SetFocus(hWndKValue);
				ShowWindow(hWndKValue,SW_SHOW);
				UpdateWindow(hWndKValue);
				*/
			}
			if(LOWORD(wParam) == IDC_REFRACTANGLE_JIAODU)
			{
			/*	currentRefractAngleBtn = refractAngleBtnList[0]; 
				SetFocus(currentRefractAngleBtn);
				gCurrentFocusBtn = currentRefractAngleBtn;
				hWndJiaoDu = CreateWindow(szJiaoDuChild, TEXT(""), WS_CHILD|WS_VISIBLE,
						MENU_KVALUE_X, MENU_KVALUE_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
				ShowWindow(hWndRefractAngle,SW_HIDE);
			//	SetFocus(hWndJiaoDu);
				ShowWindow(hWndJiaoDu,SW_SHOW);
				UpdateWindow(hWndJiaoDu);
				*/
			}
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			refractAngle_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnRefractAngleKValue)
			{
				double fAngle = DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI;
				fKValue = (float)tan(fAngle);
				displayKValue(pdis->hwndItem, fKValue);
			}
			if(pdis->hwndItem == hBtnRefractAngleJiaoDu)
			{
				displayAngle(pdis->hwndItem, DisplaySet_ad[CurrentChannelNo].fZheSheJiao);
			}
			
		/*	// ��ⰴť�������ʾ��Ӧ�Ĳ���
			memset(gszTextStep, 0, sizeof(gszTextStep));
			if(pdis->hwndItem == currentRefractAngleBtn)
			{
				if(!gbHotDB && !gbHotRange)
				{
					if(pdis->hwndItem == hBtnRefractAngleKValue)
						swprintf(gszTextStep, _T("%.1f"), fKValueStep);
					else if(pdis->hwndItem == hBtnRefractAngleJiaoDu)
						swprintf(gszTextStep,_T("%.1f"), fZheSheAngleStep);
				
					PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
				}
			}*/
			break;
		case WM_CHAR:			
			if( wParam == '\t')
			{
				// ���°�ť������
				if(CurrentChannelNo == 0)
				{
					displayAngle(hBtnRefractAngleJiaoDu, DisplaySet_ad[1].fZheSheJiao);

					double fAngle = DisplaySet_ad[1].fZheSheJiao/180.0f * PI;
					fKValue = (float)tan(fAngle);
					displayKValue(hBtnRefractAngleKValue, fKValue);
				}
				else
				{
					displayAngle(hBtnRefractAngleJiaoDu, DisplaySet_ad[0].fZheSheJiao);

					double fAngle = DisplaySet_ad[0].fZheSheJiao/180.0f * PI;
					fKValue = (float)tan(fAngle);
					displayKValue(hBtnRefractAngleKValue, fKValue);
				}
			}
			SendMessage(hWndMain, WM_CHAR, wParam, lParam);
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
*��������Ǵ��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int refractAngle_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_refractAngleBtnBitmap[]=
	{
		{IDC_REFRACTANGLE_KVALUE	,IDB_BITMAP_KZHI_NOSEL_R	 ,  IDB_BITMAP_KZHI_SELECT_R		},
		{IDC_REFRACTANGLE_JIAODU	,IDB_BITMAP_JIAODU_NOSEL_R	 ,  IDB_BITMAP_JIAODU_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_refractAngleBtnBitmap);
	return ReturnVal;
}
/**************************
*Kֵ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnRefractAngleKValueProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			fKValueStep = AMENDSTEPS[nKValueStepIndex];
			break;
		case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), fKValueStep);
			PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{					
					ShowWindow(hWndDisplay,SW_SHOW);
					SetFocus(currentDisplayBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndRefractAngle);
					hWndRefractAngle = 0;

					gCurrentFocusBtn = currentDisplayBtn;

					gCurrentWnd = hWndDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(refractAngleBtnList[i] != currentRefractAngleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentRefractAngleBtn = refractAngleBtnList[i];
					SetFocus(currentRefractAngleBtn);
					gCurrentFocusBtn = currentRefractAngleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(refractAngleBtnList[i] != currentRefractAngleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentRefractAngleBtn = refractAngleBtnList[i];
					SetFocus(currentRefractAngleBtn);
					gCurrentFocusBtn = currentRefractAngleBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(fKValue - fKValueStep >= 0)
					{
						fKValue = fKValue - fKValueStep;
					}
					else
					{
						fKValue = 0;
					}
					if(fKValue == 0)
					{
						InitializeCriticalSection(&csProbeType);
						EnterCriticalSection(&csProbeType);

						wTanTouLeiXing[CurrentChannelNo] = 1;//ֱ̽ͷ
						LeaveCriticalSection(&csProbeType);
						DeleteCriticalSection(&csProbeType);

						DisplaySet_ad[CurrentChannelNo].nWaveSpeed = typeToWaveSpeed(gwMaterialType);
						SetWaveSpeedLabelAndExecute(NULL);
					}

					displayKValue(Wnd, fKValue);

					DisplaySet_ad[CurrentChannelNo].fZheSheJiao = (float)(atan((double)fKValue) / PI * 180);
					displayAngle(hBtnRefractAngleJiaoDu, DisplaySet_ad[CurrentChannelNo].fZheSheJiao);

					SetAngleLabelAndExecute();
				}
				else if(wParam == VK_RIGHT)
				{
					if(fKValue + fKValueStep <= 573)
					{
						fKValue = fKValue + fKValueStep;

						InitializeCriticalSection(&csProbeType);
						EnterCriticalSection(&csProbeType);

						wTanTouLeiXing[CurrentChannelNo] = 0;//б̽ͷ
						LeaveCriticalSection(&csProbeType);
						DeleteCriticalSection(&csProbeType);
						
						DisplaySet_ad[CurrentChannelNo].nWaveSpeed = typeToWaveSpeed(gwMaterialType);
						SetWaveSpeedLabelAndExecute(Wnd);
					}
					else
						fKValue = 573;

					displayKValue(Wnd, fKValue);

					DisplaySet_ad[CurrentChannelNo].fZheSheJiao = (float)(atan((double)fKValue) / PI * 180);
					displayAngle(hBtnRefractAngleJiaoDu, DisplaySet_ad[CurrentChannelNo].fZheSheJiao);

					SetAngleLabelAndExecute();
				}
				else if(wParam == 'F')
				{
					if(nKValueStepIndex < 3)
						nKValueStepIndex++;
					else
						nKValueStepIndex = 0;

					fKValueStep = AMENDSTEPS[nKValueStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fKValueStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnRefractAngleKValueProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�ǶȰ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnRefractAngleJiaoDuProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			fZheSheAngleStep = ANGLESTEPS[nZheSheAngleStepIndex];
			break;
		case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), fZheSheAngleStep);
			PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndDisplay,SW_SHOW);
					SetFocus(currentDisplayBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndRefractAngle);
					hWndRefractAngle = 0;

					gCurrentFocusBtn = currentDisplayBtn;

					gCurrentWnd = hWndDisplay;
				}
			
				else if(wParam == VK_UP)
				{
					while(refractAngleBtnList[i] != currentRefractAngleBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentRefractAngleBtn = refractAngleBtnList[i];
					SetFocus(currentRefractAngleBtn);
					gCurrentFocusBtn = currentRefractAngleBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(refractAngleBtnList[i] != currentRefractAngleBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentRefractAngleBtn = refractAngleBtnList[i];
					SetFocus(currentRefractAngleBtn);
					gCurrentFocusBtn = currentRefractAngleBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(DisplaySet_ad[CurrentChannelNo].fZheSheJiao - fZheSheAngleStep >= MIN_ANGLE)
					{
						DisplaySet_ad[CurrentChannelNo].fZheSheJiao -= fZheSheAngleStep;
					}
					else
					{
						DisplaySet_ad[CurrentChannelNo].fZheSheJiao = MIN_ANGLE;

					}
					if(DisplaySet_ad[CurrentChannelNo].fZheSheJiao == 0)
					{
						InitializeCriticalSection(&csProbeType);
						EnterCriticalSection(&csProbeType);

						wTanTouLeiXing[CurrentChannelNo] = 1;//ֱ̽ͷ
						LeaveCriticalSection(&csProbeType);
						DeleteCriticalSection(&csProbeType);

						DisplaySet_ad[CurrentChannelNo].nWaveSpeed = typeToWaveSpeed(gwMaterialType);
						SetWaveSpeedLabelAndExecute(NULL);
					}
					
					displayAngle(Wnd, DisplaySet_ad[CurrentChannelNo].fZheSheJiao);

					double fAngle = DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI;
					fKValue = (float)tan(fAngle);
					displayKValue(hBtnRefractAngleKValue, fKValue);

					SetAngleLabelAndExecute();
				}
				else if(wParam == VK_RIGHT)
				{
					if(DisplaySet_ad[CurrentChannelNo].fZheSheJiao + fZheSheAngleStep <= MAX_ANGLE)
					{
						DisplaySet_ad[CurrentChannelNo].fZheSheJiao += fZheSheAngleStep;

						InitializeCriticalSection(&csProbeType);
						EnterCriticalSection(&csProbeType);

						wTanTouLeiXing[CurrentChannelNo] = 0;//б̽ͷ
						LeaveCriticalSection(&csProbeType);
						DeleteCriticalSection(&csProbeType);

						DisplaySet_ad[CurrentChannelNo].nWaveSpeed = typeToWaveSpeed(gwMaterialType);
						SetWaveSpeedLabelAndExecute(Wnd);
					}
					else
						DisplaySet_ad[CurrentChannelNo].fZheSheJiao = MAX_ANGLE;

					displayAngle(hBtnRefractAngleJiaoDu, DisplaySet_ad[CurrentChannelNo].fZheSheJiao);

					double fAngle = DisplaySet_ad[CurrentChannelNo].fZheSheJiao/180.0f * PI;
					fKValue = (float)tan(fAngle);
					displayKValue(hBtnRefractAngleKValue, fKValue);

					SetAngleLabelAndExecute();
				}
				else if(wParam == 'F')
				{
					if(nZheSheAngleStepIndex < 1)
						nZheSheAngleStepIndex++;
					else
						nZheSheAngleStepIndex = 0;

					fZheSheAngleStep = ANGLESTEPS[nZheSheAngleStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fZheSheAngleStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnRefractAngleJiaoDuProc,Wnd,Msg,wParam,lParam);
}

