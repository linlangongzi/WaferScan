/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * Fn���ܴ���Դ�ļ�
 * Filename: fnMenuWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnMenuWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "fnSpecialDisplayWin.h"
#include "fnQuantitativeAnalysisWin.h"
#include "fnSpecialFunctionWin.h"
#include "fnVideoRecordWin.h"


HWND hWndFnMenu;						//Fn���ܴ��ھ��
HWND hBtnFnMenuQuantitativeAnalysis;	//����������ť���
HWND hBtnFnMenuSpecialDisplay;			//������ʾ��ť���
HWND hBtnFnMenuSpecialFunction;			//ר�ù��ܰ�ť���
HWND hBtnFnMenuVideoRecord;				//¼��ť���
HWND hBtnFnMenuChangeType;				//�л��豸���Ͱ�ť���

HWND fnMenuBtnList[5];					//Fn���ܴ��ڵİ�ť��
WNDPROC OldBtnFnMenuQuantitativeAnalysisProc = NULL;//����������ťĬ�ϴ������
WNDPROC OldBtnFnMenuSpecialDisplayProc = NULL;		//������ʾ��ťĬ�ϴ������
WNDPROC OldBtnFnMenuSpecialFunctionProc = NULL;		//ר�ù��ܰ�ťĬ�ϴ������
WNDPROC OldBtnFnMenuVideoRecordProc = NULL;			//¼��ťĬ�ϴ������
WNDPROC OldBtnFnMenuChangeTypeProc = NULL;			//�л��豸���Ͱ�ťĬ�ϴ������
HWND currentFnMenuBtn;					//Fn���ܴ��е�ǰѡ�еİ�ť

static WORD buttonNum;					//��ť�ؼ���
static WORD wReducedButtonNum = 0;		//���ܿ��ؼ��ٰ�Ť����
static BOOL bOldDgsSwitch[CHANNEL_NUM][GATE_NUM] = {{FALSE,FALSE,FALSE},{FALSE,FALSE,FALSE}};	//DGSͨ������
static BOOL bOldAvgCurveFlag[CHANNEL_NUM] = {FALSE, FALSE};		//AVG���߿��ر�־

LRESULT CALLBACK btnFnMenuChangeTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

extern HINSTANCE hInst;					//����ʵ�����
extern HWND hWndSpecialDisplay;			//������ʾ���ھ��
extern HWND hWndQuantitativeAnalysis;	//�����������ھ��
extern HWND hWndVideoRecord;			//¼�񴰿ھ��
extern HWND hWndSpecialFunction;		//ר�ù��ܴ��ھ�� 
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;
extern WORD gwDeviceType;				//�豸���� 0:̽����	1:�����	
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;			//��ǰ��ʾ���Ӵ���	

extern WORD gwDeviceType;			//�豸���� 0:̽����	1:�����
extern HWND hWndChDisplay;			//��������ʾ���ھ��
extern HWND currentChDisplayBtn;	// ��������ʾ���ڵ�ǰѡ�еİ�ť
extern DWORD gFunctionDefine;		//���ܶ���
extern HWND hWndTsMenu;				//̽�˲˵����ھ��
extern WORD gwChHeadType;			//̽ͷ���� 0:���� 1:˫��

extern ARS200FPGA_CH_DAC_REG* ch1_dac_reg;
extern ARS200FPGA_CH_DAC_REG* ch2_dac_reg;
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];	//DGSͨ������
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG���߿��ر�־
extern HWND hWndTlMenu;//��·ר�ò˵����ھ��
extern HWND currentTlMenuBtn;// ��·ר�ò˵����ڵ�ǰѡ�еİ�ť

extern void chChangeHeadType(WORD wHeadType);
/**************************
*ע��Fn���ܴ���
* @fnMenu    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerFnMenuChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) fnMenuChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szFnMenuChild;

	return RegisterClass(&wc);
}
/**************************
*Fn���ܴ��ڴ������
* @fnMenu    hInstance[in]:
* @fnMenu		message[in]:
* @fnMenu		wParam[in]:
* @fnMenu		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK fnMenuChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			if(gwDeviceType == 0 || gwDeviceType == 2) //̽�˻���·
			{
				buttonNum = 5;
				wReducedButtonNum = 0;
				if((gFunctionDefine & DINGLIANGFENXI_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & SPECIALDISPLAY_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & SPECIALFUNCTION_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & RECORD_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) == 0)
					wReducedButtonNum ++;

				buttonNum = buttonNum - wReducedButtonNum;
				
				hBtnFnMenuQuantitativeAnalysis = CreateWindow(_T("BUTTON"),_T("quantitativeAnalysisFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_QUANTITATIVEANALYSIS,hInst,NULL);
				hBtnFnMenuSpecialDisplay = CreateWindow(_T("BUTTON"),_T("specialDisplayFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_SPECIALDISPLAY,hInst,NULL);
				hBtnFnMenuSpecialFunction = CreateWindow(_T("BUTTON"),_T("specialFunctionFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_SPECIALFUNCTION,hInst,NULL);
				hBtnFnMenuChangeType = CreateWindow(_T("BUTTON"),_T("changeTypeFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_CHANGETYPE,hInst,NULL);
				hBtnFnMenuVideoRecord = CreateWindow(_T("BUTTON"),_T("videoRecordFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_VIDEORECORD,hInst,NULL);

				if((gFunctionDefine & DINGLIANGFENXI_SWITCH) != 0)	//dgs���߹��ܿ�
				{
					fnMenuBtnList[0] = hBtnFnMenuQuantitativeAnalysis;
					if((gFunctionDefine & SPECIALDISPLAY_SWITCH) != 0)
					{
						fnMenuBtnList[1] = hBtnFnMenuSpecialDisplay;
						if((gFunctionDefine & SPECIALFUNCTION_SWITCH) != 0)
						{
							fnMenuBtnList[2] = hBtnFnMenuSpecialFunction;
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[3] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[4] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[3] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[2] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[3] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
						}
					}
					else 
					{
						if((gFunctionDefine & SPECIALFUNCTION_SWITCH) != 0)
						{
							fnMenuBtnList[1] = hBtnFnMenuSpecialFunction;
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[2] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[3] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[1] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
						}
					}
				}
				else
				{
					if((gFunctionDefine & SPECIALDISPLAY_SWITCH) != 0)
					{
						fnMenuBtnList[0] = hBtnFnMenuSpecialDisplay;
						if((gFunctionDefine & SPECIALFUNCTION_SWITCH) != 0)
						{
							fnMenuBtnList[1] = hBtnFnMenuSpecialFunction;
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[2] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[3] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[1] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
						}
					}
					else 
					{
						if((gFunctionDefine & SPECIALFUNCTION_SWITCH) != 0)
						{
							fnMenuBtnList[0] = hBtnFnMenuSpecialFunction;
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[1] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
							{
								fnMenuBtnList[0] = hBtnFnMenuChangeType;
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
							else
							{
								if((gFunctionDefine & RECORD_SWITCH) != 0)
									fnMenuBtnList[0] = hBtnFnMenuVideoRecord;
							}
						}
					}
				}
			}
			
			else if(gwDeviceType == 1)	//���
			{
				buttonNum = 4;
				wReducedButtonNum = 0;
				if((gFunctionDefine & SPECIALDISPLAY_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & SPECIALFUNCTION_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & RECORD_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) == 0)
					wReducedButtonNum ++;

				buttonNum = buttonNum - wReducedButtonNum;
				
				hBtnFnMenuSpecialDisplay = CreateWindow(_T("BUTTON"),_T("specialDisplayFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_SPECIALDISPLAY,hInst,NULL);
				hBtnFnMenuSpecialFunction = CreateWindow(_T("BUTTON"),_T("specialFunctionFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 1*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_SPECIALFUNCTION,hInst,NULL);
				hBtnFnMenuChangeType = CreateWindow(_T("BUTTON"),_T("changeTypeFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_CHANGETYPE,hInst,NULL);
				hBtnFnMenuVideoRecord = CreateWindow(_T("BUTTON"),_T("videoRecordFnMenu"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_FNMENU_VIDEORECORD,hInst,NULL);
			
				if((gFunctionDefine & SPECIALDISPLAY_SWITCH) != 0)	//dgs���߹��ܿ�
				{
					fnMenuBtnList[0] = hBtnFnMenuSpecialDisplay;
					if((gFunctionDefine & SPECIALFUNCTION_SWITCH) != 0)
					{
						fnMenuBtnList[1] = hBtnFnMenuSpecialFunction;
						if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
						{
							fnMenuBtnList[2] = hBtnFnMenuChangeType;
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[3] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
						}
					}
					else 
					{
						if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
						{
							fnMenuBtnList[1] = hBtnFnMenuChangeType;
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
						}					
					}
				}
				else
				{
					if((gFunctionDefine & SPECIALFUNCTION_SWITCH) != 0)
					{
						fnMenuBtnList[0] = hBtnFnMenuSpecialFunction;
						if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
						{
							fnMenuBtnList[1] = hBtnFnMenuChangeType;
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[2] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
						}
					}
					else 
					{
						if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) != 0)
						{
							fnMenuBtnList[0] = hBtnFnMenuChangeType;
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[1] = hBtnFnMenuVideoRecord;
							}
						}
						else
						{
							if((gFunctionDefine & RECORD_SWITCH) != 0)
							{
								fnMenuBtnList[0] = hBtnFnMenuVideoRecord;
							}
						}					
					}
				}
			}
			
			//��ť�ؼ����Զ��崦�����
			OldBtnFnMenuQuantitativeAnalysisProc = (WNDPROC) GetWindowLong(hBtnFnMenuQuantitativeAnalysis,GWL_WNDPROC);
			SetWindowLong(hBtnFnMenuQuantitativeAnalysis,GWL_WNDPROC,(LONG)btnFnMenuQuantitativeAnalysisProc);
			
			OldBtnFnMenuSpecialDisplayProc = (WNDPROC) GetWindowLong(hBtnFnMenuSpecialDisplay,GWL_WNDPROC);
			SetWindowLong(hBtnFnMenuSpecialDisplay,GWL_WNDPROC,(LONG)btnFnMenuSpecialDisplayProc);
			
			OldBtnFnMenuSpecialFunctionProc = (WNDPROC) GetWindowLong(hBtnFnMenuSpecialFunction,GWL_WNDPROC);
			SetWindowLong(hBtnFnMenuSpecialFunction,GWL_WNDPROC,(LONG)btnFnMenuSpecialFunctionProc);
			
			OldBtnFnMenuVideoRecordProc = (WNDPROC) GetWindowLong(hBtnFnMenuVideoRecord,GWL_WNDPROC);
			SetWindowLong(hBtnFnMenuVideoRecord,GWL_WNDPROC,(LONG)btnFnMenuVideoRecordProc);

			OldBtnFnMenuChangeTypeProc = (WNDPROC) GetWindowLong(hBtnFnMenuChangeType,GWL_WNDPROC);
			SetWindowLong(hBtnFnMenuChangeType,GWL_WNDPROC,(LONG)btnFnMenuChangeTypeProc);
		
			currentFnMenuBtn = fnMenuBtnList[0];//Ĭ�ϵ�ǰѡ�ж���������ť
			if(currentFnMenuBtn)
			{
				SetFocus(currentFnMenuBtn);//ѡ�а�ť������뽹��
				gCurrentFocusBtn = currentFnMenuBtn;
			}
			else			//���ܶ��رհ�ť�ĸ����ڻ�����뽹��
				SetFocus(hWnd);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				if(gwDeviceType == 0) //̽��
				{
					DestroyWindow(hWndFnMenu);
					hWndFnMenu = 0;
				
					SetMenuStateFlag(FALSE);
					SetFocus(hWndMain);//�����ڻ�����뽹��
				
					gCurrentWnd = 0;
				}
				else if(gwDeviceType == 1)
				{
					ShowWindow(hWndChDisplay,SW_SHOW);
					gCurrentWnd = hWndChDisplay;

					DestroyWindow(hWndFnMenu);
					hWndFnMenu = 0;

					SetFocus(currentChDisplayBtn);
					gCurrentFocusBtn = currentTlMenuBtn;	//2011-9-1 wangbingfu add at 2011-9-1
				}
				else if(gwDeviceType == 2)	//��·
				{
					ShowWindow(hWndTlMenu,SW_SHOW);
					gCurrentWnd = hWndTlMenu;

					DestroyWindow(hWndFnMenu);
					hWndFnMenu = 0;

					SetFocus(currentTlMenuBtn);
					gCurrentFocusBtn = currentTlMenuBtn;	//2011-9-1 wangbingfu add at 2011-9-1
				}
			}
			break;
		case WM_COMMAND:
				if(LOWORD(wParam) == IDC_FNMENU_QUANTITATIVEANALYSIS)
				{
					currentFnMenuBtn = fnMenuBtnList[0]; 
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
					hWndQuantitativeAnalysis = CreateWindow(szQuantitativeAnalysisChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);

					ShowWindow(hWndFnMenu,SW_HIDE);
					//SetFocus(hWndQuantitativeAnalysis);
					ShowWindow(hWndQuantitativeAnalysis,SW_SHOW);
					UpdateWindow(hWndQuantitativeAnalysis);
				}
				else if(LOWORD(wParam) == IDC_FNMENU_SPECIALDISPLAY)
				{
					currentFnMenuBtn = fnMenuBtnList[1]; 
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
					hWndSpecialDisplay = CreateWindow(szSpecialDisplayChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFnMenu,SW_HIDE);
					//SetFocus(hWndSpecialDisplay);
					ShowWindow(hWndSpecialDisplay,SW_SHOW);
					UpdateWindow(hWndSpecialDisplay);
				}
				else if(LOWORD(wParam) == IDC_FNMENU_SPECIALFUNCTION)
				{
					currentFnMenuBtn = fnMenuBtnList[2]; 
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
					hWndSpecialFunction = CreateWindow(szSpecialFunctionChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFnMenu,SW_HIDE);
					//SetFocus(hWndSpecialFunction);
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					UpdateWindow(hWndSpecialFunction);
				}
				else if(LOWORD(wParam) == IDC_FNMENU_VIDEORECORD)
				{
				//	currentFnMenuBtn = fnMenuBtnList[3]; 
				//	SetFocus(currentFnMenuBtn);
				//	gCurrentFocusBtn = currentFnMenuBtn;
				}
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			fnMenu_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_CLOSE:
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*����Fn���ܴ��ڰ�ťͼƬ
* @fnMenu    hInstance[in]:
* @fnMenu		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int fnMenu_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	if(gwDeviceType == 0 || gwDeviceType == 2) //̽�˻���·
	{
		BTN_BMP_INFO tbl_fnMenuBtnBitmap[]=
		{
			{IDC_FNMENU_QUANTITATIVEANALYSIS	,IDB_BITMAP_DINGLIANGFENXI_NOSEL_R	 ,  IDB_BITMAP_DINGLIANGFENXI_SELECT_R		},
			{IDC_FNMENU_SPECIALDISPLAY	,IDB_BITMAP_TESHUXIANSHI_NOSEL_R	 ,  IDB_BITMAP_TESHUXIANSHI_SELECT_R		},
			{IDC_FNMENU_SPECIALFUNCTION	,IDB_BITMAP_ZHUANYONGGONGNENG_NOSEL_R ,  IDB_BITMAP_ZHUANYONGGONGNENG_SELECT_R	},
			{IDC_FNMENU_CHANGETYPE	,IDB_BITMAP_CEHOUMOSHI_NOSEL_R ,  IDB_BITMAP_CEHOUMOSHI_SELECT_R	},
			{IDC_FNMENU_VIDEORECORD	,IDB_BITMAP_LUXIANG_NOSEL_R ,  IDB_BITMAP_LUXIANG_SELECT_R	},
			{	-1			,	-1					 ,	-1							}    
		};
		if((gFunctionDefine & DINGLIANGFENXI_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[0].nBtnNoFocID = IDB_BITMAP_DINGLIANGFENXI_DISABLE_R;
		if((gFunctionDefine & SPECIALDISPLAY_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[1].nBtnNoFocID = IDB_BITMAP_TESHUXIANSHI_DISABLE_R;
		if((gFunctionDefine & SPECIALFUNCTION_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[2].nBtnNoFocID = IDB_BITMAP_ZHUANYONGGONGNENG_DISABLE_R;
		if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[3].nBtnNoFocID = IDB_BITMAP_CEHOUMOSHI_DISABLE_R;
		if((gFunctionDefine & RECORD_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[4].nBtnNoFocID = IDB_BITMAP_LUXIANG_DISABLE_R;
		
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_fnMenuBtnBitmap);
	}
	else if(gwDeviceType == 1)	//���
	{
		BTN_BMP_INFO tbl_fnMenuBtnBitmap[]=
		{
			{IDC_FNMENU_SPECIALDISPLAY	,IDB_BITMAP_TESHUXIANSHI_NOSEL_R	 ,  IDB_BITMAP_TESHUXIANSHI_SELECT_R		},
			{IDC_FNMENU_SPECIALFUNCTION	,IDB_BITMAP_ZHUANYONGGONGNENG_NOSEL_R ,  IDB_BITMAP_ZHUANYONGGONGNENG_SELECT_R	},
			{IDC_FNMENU_CHANGETYPE	,IDB_BITMAP_TANSHANGMOSHI_NOSEL_R ,  IDB_BITMAP_TANSHANGMOSHI_SELECT_R	},
			{IDC_FNMENU_VIDEORECORD	,IDB_BITMAP_LUXIANG_NOSEL_R ,  IDB_BITMAP_LUXIANG_SELECT_R	},			
			{	-1			,	-1					 ,	-1							}    
		};
		if((gFunctionDefine & SPECIALDISPLAY_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[0].nBtnNoFocID = IDB_BITMAP_TESHUXIANSHI_DISABLE_R;
		if((gFunctionDefine & SPECIALFUNCTION_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[1].nBtnNoFocID = IDB_BITMAP_ZHUANYONGGONGNENG_DISABLE_R;
		if((gFunctionDefine & DEVICETYPECHANGE_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[2].nBtnNoFocID = IDB_BITMAP_TANSHANGMOSHI_DISABLE_R;
		if((gFunctionDefine & RECORD_SWITCH) == 0)
			tbl_fnMenuBtnBitmap[3].nBtnNoFocID = IDB_BITMAP_LUXIANG_DISABLE_R;
		
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_fnMenuBtnBitmap);
	}
	
	return ReturnVal;
}
/**************************
*����������ť�Զ��崦�����
* @fnMenu    hInstance[in]:
* @fnMenu		message[in]:
* @fnMenu		wParam[in]:
* @fnMenu		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFnMenuQuantitativeAnalysisProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndQuantitativeAnalysis = CreateWindow(szQuantitativeAnalysisChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFnMenu,SW_HIDE);
				//	SetFocus(hWndQuantitativeAnalysis);
					ShowWindow(hWndQuantitativeAnalysis,SW_SHOW);
					UpdateWindow(hWndQuantitativeAnalysis);

					gCurrentWnd = hWndQuantitativeAnalysis;
					
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
	return CallWindowProc(OldBtnFnMenuQuantitativeAnalysisProc,Wnd,Msg,wParam,lParam);
}
/**************************
*������ʾ��ť�Զ��崦�����
* @fnMenu    hInstance[in]:
* @fnMenu		message[in]:
* @fnMenu		wParam[in]:
* @fnMenu		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFnMenuSpecialDisplayProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndSpecialDisplay = CreateWindow(szSpecialDisplayChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFnMenu,SW_HIDE);
					//SetFocus(hWndSpecialDisplay);
					ShowWindow(hWndSpecialDisplay,SW_SHOW);
					UpdateWindow(hWndSpecialDisplay);

					gCurrentWnd = hWndSpecialDisplay;
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
	return CallWindowProc(OldBtnFnMenuSpecialDisplayProc,Wnd,Msg,wParam,lParam);
}
/**************************
*ר�ù��ܰ�ť�Զ��崦�����
* @fnMenu    hInstance[in]:
* @fnMenu		message[in]:
* @fnMenu		wParam[in]:
* @fnMenu		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFnMenuSpecialFunctionProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndSpecialFunction = CreateWindow(szSpecialFunctionChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFnMenu,SW_HIDE);
					//SetFocus(hWndSpecialFunction);
					ShowWindow(hWndSpecialFunction,SW_SHOW);
					UpdateWindow(hWndSpecialFunction);

					gCurrentWnd = hWndSpecialFunction;
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
	return CallWindowProc(OldBtnFnMenuSpecialFunctionProc,Wnd,Msg,wParam,lParam);
}
/**************************
*¼��ť�Զ��崦�����
* @fnMenu    hInstance[in]:
* @fnMenu		message[in]:
* @fnMenu		wParam[in]:
* @fnMenu		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFnMenuVideoRecordProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndVideoRecord= CreateWindow(szVideoRecordChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndFnMenu,SW_HIDE);
					//SetFocus(hWndVideoRecord);
					ShowWindow(hWndVideoRecord,SW_SHOW);
					UpdateWindow(hWndVideoRecord);

					gCurrentWnd = hWndVideoRecord;
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
	return CallWindowProc(OldBtnFnMenuVideoRecordProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�豸ת����ť�Զ��崦�����
* @fnMenu    hInstance[in]:
* @fnMenu		message[in]:
* @fnMenu		wParam[in]:
* @fnMenu		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnFnMenuChangeTypeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(fnMenuBtnList[i] != currentFnMenuBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentFnMenuBtn = fnMenuBtnList[i];
					SetFocus(currentFnMenuBtn);
					gCurrentFocusBtn = currentFnMenuBtn;
				}
				else if(wParam == VK_RETURN)
				{
					if(gwDeviceType == 0)	//̽��
					{
						gwDeviceType = 1;

					//	if(hWndChDisplay == 0)
					//	{
							DestroyWindow(hWndFnMenu);
							hWndFnMenu = 0;

							hWndChDisplay = CreateWindow(szChDisplayChild, TEXT(""), WS_CHILD,
										 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);//������������ʾ����
							ShowWindow(hWndChDisplay,SW_SHOW);
							UpdateWindow(hWndChDisplay);

							gCurrentWnd = hWndChDisplay;
					//	}
						chChangeHeadType(gwChHeadType); //�л�����̽ͷ����

						ch1_dac_reg->wDACEnable = 0; //�ر�TCG
						//���濪��DAC״̬
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							for(int j = 0; j < GATE_NUM; j++)
							{
								bOldDgsSwitch[i][j] = gDgsSwitch[i][j];
							}
						}
						//���濪��AVG״̬
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							bOldAvgCurveFlag[i] = gbAvgCurveFlag[i];
						}
						//�ر�DAC
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							for(int j = 0; j < GATE_NUM; j++)
							{
								gDgsSwitch[i][j] = FALSE;
							}
						}
						//�ر�AVG
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							gbAvgCurveFlag[i] = OFF;
						}
					}
					else if(gwDeviceType == 1)	//���
					{
						gwDeviceType = 0;

						DestroyWindow(hWndFnMenu);
						hWndFnMenu = 0;
						
						DestroyWindow(hWndChDisplay);
						hWndChDisplay = 0;

						hWndTsMenu = CreateWindow(szTsMenuChild, TEXT(""), WS_CHILD,
									 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);//����̽�˲˵�����
						ShowWindow(hWndTsMenu,SW_SHOW);
						UpdateWindow(hWndTsMenu);

						gCurrentWnd = hWndTsMenu;

						//��ԭ����DAC״̬
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							for(int j = 0; j < GATE_NUM; j++)
							{
								gDgsSwitch[i][j] = bOldDgsSwitch[i][j];
							}
						}
						//��ԭ����AVG״̬
						for(int i = 0; i < CHANNEL_NUM; i++)
						{
							gbAvgCurveFlag[i] = bOldAvgCurveFlag[i];
						}
							
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
		default:
			break;
	}
	return CallWindowProc(OldBtnFnMenuVideoRecordProc,Wnd,Msg,wParam,lParam);
}
