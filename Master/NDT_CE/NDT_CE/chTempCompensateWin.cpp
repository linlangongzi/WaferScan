/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ����¶Ȳ�������Դ�ļ�
 * Filename: chTempCompensateWin.cpp
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


HWND hWndChTempCompensate = NULL;						//����¶Ȳ������ھ��
HWND currentChTempCompensateBtn = NULL;					//����¶Ȳ������ڵ�ǰѡ�еİ�ť
TCHAR strTempCurveFileName[MAX_PATH] = _T("\\ResidentFlash\\tempCurve.dat");
WORD gwTempCurveMaterial = 0;					//�¶Ȳ�������
BOOL gbTempCurveFlag = FALSE;					//�Ƿ���¶Ȳ�������
float gfCurrentTemperature = 20;				//��ǰ�¶�

static HWND hBtnChTempCompensateMaterial = NULL;				//���ϣ��������в�����ͬ��/�Զ��尴ť���
static HWND hBtnChTempCompensateTemperature = NULL ;			//��ǰ�¶Ȱ�ť���
static HWND hBtnChTempCompensateCurveMake = NULL;			//�Զ�������������ť���
static HWND hBtnChTempCompensateSwitch = NULL;				//��/�ذ�ť���
static HWND chTempCompensateBtnList[4] ={NULL,NULL,NULL,NULL};					//����¶Ȳ������ڵİ�ť��
static WNDPROC OldBtnChTempCompensateMaterialProc = NULL;	//���ϣ��������в�����ͬ��/�Զ��尴ťĬ�ϴ������
static WNDPROC OldBtnChTempCompensateTemperatureProc = NULL;		//��ǰ�¶Ȱ�ťĬ�ϴ������
static WNDPROC OldBtnChTempCompensateCurveMakeProc = NULL;		//�Զ�������������ťĬ�ϴ������
static WNDPROC OldBtnChTempCompensateSwitchProc = NULL;			//��/�ذ�ťĬ�ϴ������

static const int buttonNum = 4;		//��ť�ؼ���
static float fTemperatureStep = 1;		//�¶Ȳ���
static WORD wTemperatureStepIndex = 1;	//�¶Ȳ�������

ATOM registerChTempCompensateChild(HINSTANCE hInstance);
static LRESULT CALLBACK chTempCompensateChildProc(HWND hWnd, 
											  UINT message, WPARAM wParam, LPARAM lParam);
static int chTempCompensate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnChTempCompensateMaterialProc( HWND Wnd, 
													UINT Msg, WPARAM wParam, LPARAM lParam );
static LRESULT CALLBACK btnChTempCompensateTemperatureProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChTempCompensateCurveMakeProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnChTempCompensateSwitchProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
BOOL readTempCurveFile(LPCTSTR lpszPathName);

extern BOOL saveTempCurveFile(LPCTSTR lpszPathName);

extern HINSTANCE hInst;					//����ʵ�����
extern HWND hWndTempCurveMake;			//�¶Ȳ��������������ھ�
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern HWND hWndChAdjust;
extern HWND currentChAdjustBtn;
extern HWND gCurrentWnd;
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;						// ״̬�����
extern WORD gwTempCurvePointNum[MATERIAL_NUM + 10];									//�¶Ȳ������ߵ�ĸ���
extern TEMPCURVEPOINT_DATA gstTempCurvePointData[MATERIAL_NUM + 10][HEADV_POINT_MAX];		//�ɼ����¶Ȳ������ߵ�


void getTempCurveData()
{
	readTempCurveFile(strTempCurveFileName);
	gwTempCurvePointNum[0] = 20;
	gstTempCurvePointData[0][0].fTemperature = 0;
	gstTempCurvePointData[0][0].wWaveSpeed = 5920;
	for(int i = 1; i < 19; i++)
	{
		gstTempCurvePointData[0][i].fTemperature = (float)(25 + 55*i);
		gstTempCurvePointData[0][i].wWaveSpeed = (WORD)(5920 / (1 + ((float)i/100)));
	}
	gstTempCurvePointData[0][19].fTemperature = 1000;
	gstTempCurvePointData[0][19].wWaveSpeed = (WORD)(5920 / (1 + ((float)17.27/100)));
			
	gwTempCurvePointNum[MATERIAL_NUM] = 20;
	gstTempCurvePointData[MATERIAL_NUM][0].fTemperature = 0;
	gstTempCurvePointData[MATERIAL_NUM][0].wWaveSpeed = 5920;
	for(int i = 1; i < 19; i++)
	{
		gstTempCurvePointData[MATERIAL_NUM][i].fTemperature = (float)(25 + 55*i);
		gstTempCurvePointData[MATERIAL_NUM][i].wWaveSpeed = (WORD)(5920 / (1 + ((float)i/100)));
	}
	gstTempCurvePointData[MATERIAL_NUM][19].fTemperature = 1000;
	gstTempCurvePointData[MATERIAL_NUM][19].wWaveSpeed = (WORD)(5920 / (1 + ((float)17.27/100)));
}
/**************************
*��ȡ�¶Ȳ������ߵ��ļ�
* @param		hWnd[in]:���ھ��
* @param		wNum[in]:���
* @return   LRESULT :
* @since    1.00
***************************/
BOOL readTempCurveFile(LPCTSTR lpszPathName)
{			
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
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("���ļ�ʧ��"));
		return FALSE;
	}
	for(int i = 0; i < MATERIAL_NUM + 10; i++)
	{
		if(!ReadFile(hFileWrite,&gwTempCurvePointNum[i], sizeof(WORD),&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("���ļ�ʧ��"));
			
			CloseHandle(hFileWrite);
			return FALSE;
		}
	}
	for(int i = 0; i < MATERIAL_NUM + 10; i++)
	{
		if(!ReadFile(hFileWrite,gstTempCurvePointData[i], sizeof(TEMPCURVEPOINT_DATA) * TEMPCURVE_POINT_MAX,&dwWriteDataSize,NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("���ļ�ʧ��"));
			
			CloseHandle(hFileWrite);
			return FALSE;
		}
	}

	CloseHandle(hFileWrite);

	return TRUE;
}

/**************************
*��ʾ����
* @param		hWnd[in]:���ھ��
* @param		wMaterialType[in]:��������
* @return   LRESULT :
* @since    1.00
***************************/
static void displayMaterial(HWND hWnd ,WORD wMaterialType)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	readTempCurveFile(strTempCurveFileName);

	if(gwTempCurvePointNum[wMaterialType]  == 0)
	{
		if(wMaterialType <= MATERIAL_NUM - 1)
		{
			switch(wMaterialType)
			{
				case 0:
					DrawLableText(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					break;
				case 1:
					DrawLableText(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					break;
				case 2:
					DrawLableText(hWnd, _T("ͭ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					break;
				case 3:
					DrawLableText(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					break;
				case 4:
					DrawLableText(hWnd, _T("п"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					break;
				default:
					break;						
			}
		}
		else
		{
			wsprintf(strText, _T("*�Զ���%d"), wMaterialType + 1 - MATERIAL_NUM);
			
			DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
	}
	else
	{
		if(wMaterialType <= MATERIAL_NUM - 1)
		{
			switch(wMaterialType)
			{
				case 0:
					DrawLableText(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					break;
				case 1:
					DrawLableText(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					break;
				case 2:
					DrawLableText(hWnd, _T("ͭ"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					break;
				case 3:
					DrawLableText(hWnd, _T("��"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					break;
				case 4:
					DrawLableText(hWnd, _T("п"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
					break;

				default:
					break;						
			}
		}
		else
		{
			wsprintf(strText, _T("�Զ���%d"), wMaterialType + 1 - MATERIAL_NUM);
			DrawLableText(hWnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
		}
	}
}
/**************************
*��ʾ�¶�
* @param		hWnd[in]:���ھ��
* @param		fTemp[in]:�¶�
* @return   LRESULT :
* @since    1.00
***************************/
static void displayTemperature(HWND hWnd ,float fTemp)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	
	wsprintf(strText, _T("%.1f"), fTemp);	
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
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
*ע�����¶Ȳ�������
* @chTempCompensate    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerChTempCompensateChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) chTempCompensateChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szChTempCompensateChild;

	return RegisterClass(&wc);
}
/**************************
*����¶Ȳ������ڴ������
* @chTempCompensate    hInstance[in]:
* @chTempCompensate		message[in]:
* @chTempCompensate		wParam[in]:
* @chTempCompensate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK chTempCompensateChildProc(HWND hWnd, UINT message, 
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
			getTempCurveData();
		//	readTempCurveFile(strTempCurveFileName);
			saveTempCurveFile(strTempCurveFileName);
			//����ť�ؼ�
			hBtnChTempCompensateMaterial = CreateWindow(_T("BUTTON"),_T("materialChTempCompensate"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCOMPENSATE_MATERIAL,hInst,NULL);
			hBtnChTempCompensateTemperature  = CreateWindow(_T("BUTTON"),_T("temperature ChTempCompensate"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCOMPENSATE_TEMPERATURE ,hInst,NULL);
			hBtnChTempCompensateCurveMake = CreateWindow(_T("BUTTON"),_T("curveMakeChTempCompensate"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCOMPENSATE_CURVEMAKE,hInst,NULL);
			hBtnChTempCompensateSwitch = CreateWindow(_T("BUTTON"),_T("switchChTempCompensate"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 3*BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CHTEMPCOMPENSATE_SWITCH,hInst,NULL);
					
			chTempCompensateBtnList[0] = hBtnChTempCompensateMaterial;
			chTempCompensateBtnList[1] = hBtnChTempCompensateTemperature;
			chTempCompensateBtnList[2] = hBtnChTempCompensateCurveMake;
			chTempCompensateBtnList[3] = hBtnChTempCompensateSwitch;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnChTempCompensateMaterialProc = (WNDPROC) GetWindowLong(hBtnChTempCompensateMaterial,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCompensateMaterial,GWL_WNDPROC,(LONG)btnChTempCompensateMaterialProc);
			
			OldBtnChTempCompensateTemperatureProc = (WNDPROC) GetWindowLong(hBtnChTempCompensateTemperature,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCompensateTemperature,GWL_WNDPROC,(LONG)btnChTempCompensateTemperatureProc);
			
			OldBtnChTempCompensateCurveMakeProc = (WNDPROC) GetWindowLong(hBtnChTempCompensateCurveMake,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCompensateCurveMake,GWL_WNDPROC,(LONG)btnChTempCompensateCurveMakeProc);
			
			OldBtnChTempCompensateSwitchProc = (WNDPROC) GetWindowLong(hBtnChTempCompensateSwitch,GWL_WNDPROC);
			SetWindowLong(hBtnChTempCompensateSwitch,GWL_WNDPROC,(LONG)btnChTempCompensateSwitchProc);
		
			currentChTempCompensateBtn = chTempCompensateBtnList[0];//Ĭ�ϵ�ǰѡ�в��ϣ��������в�����ͬ��/�Զ��尴ť
			SetFocus(currentChTempCompensateBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentChTempCompensateBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			
			break;
		case WM_COMMAND:
		/*		if(LOWORD(wParam) == IDC_CHTEMPCOMPENSATE_CURVEMAKE)
				{
					currentChTempCompensateBtn = chTempCompensateBtnList[2]; 
					SetFocus(currentChTempCompensateBtn);
					gCurrentFocusBtn = currentChTempCompensateBtn;
					hWndCurveMake = CreateWindow(szCurveMakeChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChTempCompensate,SW_HIDE);
					//SetFocus(hWndCurveMake);
					ShowWindow(hWndCurveMake,SW_SHOW);
					UpdateWindow(hWndCurveMake);
				}
				else if(LOWORD(wParam) == IDC_CHTEMPCOMPENSATE_SWITCH)
				{
				//	currentChTempCompensateBtn = chTempCompensateBtnList[3]; 
				//	SetFocus(currentChTempCompensateBtn);
				//	gCurrentFocusBtn = currentChTempCompensateBtn;
				}
			*/
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			chTempCompensate_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnChTempCompensateMaterial)
			{
				displayMaterial(pdis->hwndItem, gwTempCurveMaterial);
			}
			if(pdis->hwndItem == hBtnChTempCompensateTemperature)
			{
				displayTemperature(pdis->hwndItem,gfCurrentTemperature);
			}
			if(pdis->hwndItem == hBtnChTempCompensateSwitch)
			{
				displaySwitch(pdis->hwndItem,gbTempCurveFlag);
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
*���ز���¶Ȳ������ڰ�ťͼƬ
* @chTempCompensate    hInstance[in]:
* @chTempCompensate		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int chTempCompensate_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_chTempCompensateBtnBitmap[]=
	{
		{IDC_CHTEMPCOMPENSATE_MATERIAL	,IDB_BITMAP_CAILIAO_NOSEL_R	 ,  IDB_BITMAP_CAILIAO_SELECT_R		},
		{IDC_CHTEMPCOMPENSATE_TEMPERATURE 	,IDB_BITMAP_DANGQIANWENDU_NOSEL_R	 ,  IDB_BITMAP_DANGQIANWENDU_SELECT_R		},
		{IDC_CHTEMPCOMPENSATE_CURVEMAKE	,IDB_BITMAP_ZIDINGYIQUXIANZHIZUO_NOSEL_R ,  IDB_BITMAP_ZIDINGYIQUXIANZHIZUO_SELECT_R	},
		{IDC_CHTEMPCOMPENSATE_SWITCH	,IDB_BITMAP_KAIQIGUANBI4_NOSEL_R ,  IDB_BITMAP_KAIQIGUANBI4_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_chTempCompensateBtnBitmap);
	return ReturnVal;
}
/**************************
*���ϣ��������в�����ͬ��/�Զ��尴ť�Զ��崦�����
* @chTempCompensate    hInstance[in]:
* @chTempCompensate		message[in]:
* @chTempCompensate		wParam[in]:
* @chTempCompensate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCompensateMaterialProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					
					DestroyWindow(hWndChTempCompensate);
					hWndChTempCompensate = 0;

					gCurrentFocusBtn = currentChAdjustBtn;

					gCurrentWnd = hWndChAdjust;
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCompensateBtnList[i] != currentChTempCompensateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCompensateBtn = chTempCompensateBtnList[i];
					SetFocus(currentChTempCompensateBtn);
					gCurrentFocusBtn = currentChTempCompensateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCompensateBtnList[i] != currentChTempCompensateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCompensateBtn = chTempCompensateBtnList[i];
					SetFocus(currentChTempCompensateBtn);
					gCurrentFocusBtn = currentChTempCompensateBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if(gwTempCurveMaterial == 0)
						gwTempCurveMaterial = MATERIAL_NUM + 10 -1;
					else
						gwTempCurveMaterial--;

					displayMaterial(Wnd, gwTempCurveMaterial);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gwTempCurveMaterial == MATERIAL_NUM + 10 -1)
						gwTempCurveMaterial = 0;
					else
						gwTempCurveMaterial++;

					displayMaterial(Wnd, gwTempCurveMaterial);
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
	return CallWindowProc(OldBtnChTempCompensateMaterialProc,Wnd,Msg,wParam,lParam);
}
/**************************
*��ǰ�¶Ȱ�ť�Զ��崦�����
* @chTempCompensate    hInstance[in]:
* @chTempCompensate		message[in]:
* @chTempCompensate		wParam[in]:
* @chTempCompensate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCompensateTemperatureProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
		case WM_SETFOCUS:
			swprintf(gszTextStep, _T("%.1f"), fTemperatureStep);
			PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			break;
		case WM_KILLFOCUS:
			swprintf(gszTextStep, _T(""));
			PostMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndChAdjust,SW_SHOW);
					SetFocus(currentChAdjustBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					
					DestroyWindow(hWndChTempCompensate);
					hWndChTempCompensate = 0;

					gCurrentFocusBtn = currentChAdjustBtn;

					gCurrentWnd = hWndChAdjust;
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCompensateBtnList[i] != currentChTempCompensateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCompensateBtn = chTempCompensateBtnList[i];
					SetFocus(currentChTempCompensateBtn);
					gCurrentFocusBtn = currentChTempCompensateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCompensateBtnList[i] != currentChTempCompensateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCompensateBtn = chTempCompensateBtnList[i];
					SetFocus(currentChTempCompensateBtn);
					gCurrentFocusBtn = currentChTempCompensateBtn;
				}
				else if(wParam == VK_LEFT)
				{
					if((gfCurrentTemperature - fTemperatureStep) >= 0)
						gfCurrentTemperature -= fTemperatureStep;
					else
						gfCurrentTemperature = 0;

					displayTemperature(Wnd, gfCurrentTemperature);
				}
				else if(wParam == VK_RIGHT)
				{
					if(gfCurrentTemperature += fTemperatureStep <= 2000)
					{
						gfCurrentTemperature += fTemperatureStep;
					}
					else
					{
						gfCurrentTemperature = 2000;
					}
					
					displayTemperature(Wnd, gfCurrentTemperature);
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == 'F')
				{
					if(wTemperatureStepIndex < 3)
						wTemperatureStepIndex++;
					else
						wTemperatureStepIndex = 0;

					fTemperatureStep = AMENDSTEPS[wTemperatureStepIndex];

					memset(gszTextStep, 0, sizeof(gszTextStep));
					swprintf(gszTextStep, _T("%.1f"), fTemperatureStep);
					SendMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (2 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextStep);
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
	return CallWindowProc(OldBtnChTempCompensateTemperatureProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�Զ�������������ť�Զ��崦�����
* @chTempCompensate    hInstance[in]:
* @chTempCompensate		message[in]:
* @chTempCompensate		wParam[in]:
* @chTempCompensate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCompensateCurveMakeProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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
					
					DestroyWindow(hWndChTempCompensate);
					hWndChTempCompensate = 0;

					gCurrentFocusBtn = currentChAdjustBtn;
					
					gCurrentWnd = hWndChAdjust;					
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCompensateBtnList[i] != currentChTempCompensateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCompensateBtn = chTempCompensateBtnList[i];
					SetFocus(currentChTempCompensateBtn);
					gCurrentFocusBtn = currentChTempCompensateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCompensateBtnList[i] != currentChTempCompensateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCompensateBtn = chTempCompensateBtnList[i];
					SetFocus(currentChTempCompensateBtn);
					gCurrentFocusBtn = currentChTempCompensateBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndTempCurveMake = CreateWindow(szChTempCurveMakeChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndChTempCompensate,SW_HIDE);
				//	SetFocus(hWndTempCurveMake);
					ShowWindow(hWndTempCurveMake,SW_SHOW);
					UpdateWindow(hWndTempCurveMake);

					gCurrentWnd = hWndTempCurveMake;
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
	return CallWindowProc(OldBtnChTempCompensateCurveMakeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*��/�ذ�ť�Զ��崦�����
* @chTempCompensate    hInstance[in]:
* @chTempCompensate		message[in]:
* @chTempCompensate		wParam[in]:
* @chTempCompensate		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnChTempCompensateSwitchProc(HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam)
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
					
					DestroyWindow(hWndChTempCompensate);
					hWndChTempCompensate = 0;

					gCurrentFocusBtn = currentChAdjustBtn;

					gCurrentWnd = hWndChAdjust;					
				}
			
				else if(wParam == VK_UP)
				{
					while(chTempCompensateBtnList[i] != currentChTempCompensateBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentChTempCompensateBtn = chTempCompensateBtnList[i];
					SetFocus(currentChTempCompensateBtn);
					gCurrentFocusBtn = currentChTempCompensateBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(chTempCompensateBtnList[i] != currentChTempCompensateBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentChTempCompensateBtn = chTempCompensateBtnList[i];
					SetFocus(currentChTempCompensateBtn);
					gCurrentFocusBtn = currentChTempCompensateBtn;
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					gbTempCurveFlag = !gbTempCurveFlag;
					if(gbTempCurveFlag)
					{
						readTempCurveFile(strTempCurveFileName);
					}
					displaySwitch(Wnd, gbTempCurveFlag);
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
	return CallWindowProc(OldBtnChTempCompensateSwitchProc,Wnd,Msg,wParam,lParam);
}
