/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ������������Դ�ļ�
 * Filename: fnQuantitativeAnalysisWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnQuantitativeAnalysisWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"


HWND hWndQuantitativeAnalysis = NULL;			//�����������ھ��

HWND hBtnQuantitativeAnalysisDgsCurve = NULL;		//DGS���߰�ť���
HWND hBtnQuantitativeAnalysisAutoGainPlus = NULL;//�Զ����水ť���
HWND hBtnQuantitativeAnalysisAdjust = NULL;//У׼��ť���
HWND hBtnQuantitativeAnalysisPeakRemember = NULL;//��ֵ���䰴ť���

HWND quantitativeAnalysisBtnList[4] ={NULL,NULL,NULL,NULL};	//�����������ڵİ�ť��

WNDPROC OldBtnQuantitativeAnalysisDgsCurveProc = NULL;		//DGS���߰�ťĬ�ϴ������
WNDPROC OldBtnQuantitativeAnalysisAutoGainPlusProc = NULL;//�Զ����水ťĬ�ϴ������
WNDPROC OldBtnQuantitativeAnalysisAdjustProc = NULL;//У׼��ťĬ�ϴ������
WNDPROC OldBtnQuantitativeAnalysisPeakRememberProc = NULL;//У׼��ťĬ�ϴ������


HWND currentQuantitativeAnalysisBtn = NULL;					//�����������е�ǰѡ�еİ�ť

BOOL gbPeakRememberFlag = FALSE;  	//��ֵ�����Ƿ���
CRITICAL_SECTION csPeakRemember;

static WORD wReducedButtonNum = 0;	//���ܿ��ؼ��ٰ�Ť����

static int  buttonNum = 4;			//��ť�ؼ���

extern HINSTANCE hInst;				//����ʵ�����
extern HWND hWndFnMenu;				//Fn���ܴ��ھ��
extern HWND currentFnMenuBtn;		//Fn���ܴ��ڵ�ǰѡ�о��
extern HWND hWndMain;
//extern HWND hWndDgsCurve;			//DGS���ߴ��ھ��
extern HWND hWndAutoGainPlus;		//�Զ����洰�ھ��
extern HWND hWndAdjust;				//У׼���ھ��
extern HWND gCurrentFocusBtn;
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;			//��ǰ��ʾ���Ӵ���
extern DWORD gFunctionDefine;		//���ܶ���

/**************************
*ע�ᶨ����������
* @quantitativeAnalysis    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerQuantitativeAnalysisChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) quantitativeAnalysisChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szQuantitativeAnalysisChild;

	return RegisterClass(&wc);
}

void displayPeakRemember(HWND hWnd ,BOOL bSwitch)
{
	if(bSwitch)
		DrawLableText(hWnd, _T("����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	else
		DrawLableText(hWnd, _T("�ر�"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

/**************************
*�����������ڴ������
* @quantitativeAnalysis    hInstance[in]:
* @quantitativeAnalysis		message[in]:
* @quantitativeAnalysis		wParam[in]:
* @quantitativeAnalysis		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK quantitativeAnalysisChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;
	
	switch (message) 
	{
		case WM_CREATE:
		//	gFunctionDefine = 	0x0000000A;
			buttonNum = 3;
			wReducedButtonNum = 0;
		//	if((gFunctionDefine & DGSCURVE_SWITCH) == 0)
		//		wReducedButtonNum ++;
			if((gFunctionDefine & AUTOGAIN_SWITCH) == 0)
				wReducedButtonNum ++;
			if((gFunctionDefine & AUTOADJUST_SWITCH) == 0)
				wReducedButtonNum ++;
			if((gFunctionDefine & PEAKMEMORY_SWITCH) == 0)
				wReducedButtonNum ++;

			buttonNum = buttonNum - wReducedButtonNum;
			//����ť�ؼ�
		//	hBtnQuantitativeAnalysisDgsCurve = CreateWindow(_T("BUTTON"),_T("dgsCurveQuantitativeAnalysis"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
		//			0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUANTITATIVEANALYSIS_DGSCURVE,hInst,NULL);
			hBtnQuantitativeAnalysisAutoGainPlus = CreateWindow(_T("BUTTON"),_T("AutoGainPlusQuantitativeAnalysis"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUANTITATIVEANALYSIS_AUTOGAINPLUS,hInst,NULL);
			hBtnQuantitativeAnalysisAdjust = CreateWindow(_T("BUTTON"),_T("AdjustQuantitativeAnalysis"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUANTITATIVEANALYSIS_ADJUST,hInst,NULL);
			hBtnQuantitativeAnalysisPeakRemember = CreateWindow(_T("BUTTON"),_T("peakRememberQuantitativeAnalysis"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_QUANTITATIVEANALYSIS_PEAKREMEMBER,hInst,NULL);

		/*	if((gFunctionDefine & DGSCURVE_SWITCH) != 0)	//dgs���߹��ܿ�
			{
				quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisDgsCurve;
				if((gFunctionDefine & AUTOGAIN_SWITCH) != 0)
				{
					quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisAutoGainPlus;
					if((gFunctionDefine & AUTOADJUST_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[2] = hBtnQuantitativeAnalysisAdjust;
						if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
						{
							quantitativeAnalysisBtnList[3] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
					else
					{
						if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
						{
							quantitativeAnalysisBtnList[2] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
				}
				else 
				{
					if((gFunctionDefine & AUTOADJUST_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisAdjust;
						if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
						{
							quantitativeAnalysisBtnList[2] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
					else
					{
						if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
						{
							quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
					
				}
			}
			else					//dgs���߹��ܹ�
			{	
				if((gFunctionDefine & AUTOGAIN_SWITCH) != 0)
				{
					quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisAutoGainPlus;
					if((gFunctionDefine & AUTOADJUST_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisAdjust;
						if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
						{
							quantitativeAnalysisBtnList[2] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
					else
					{
						if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
						{
							quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
				}
				else 
				{
					if((gFunctionDefine & 0x0004) != 0)
					{
						quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisAdjust;
						if((gFunctionDefine & 0x0008) != 0)
						{
							quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}
					else
					{
						if((gFunctionDefine & 0x0008) != 0)
						{
							quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisPeakRemember;
						}
					}					
				}
			}*/

			if((gFunctionDefine & AUTOGAIN_SWITCH) != 0)
			{
				quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisAutoGainPlus;
				if((gFunctionDefine & AUTOADJUST_SWITCH) != 0)
				{
					quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisAdjust;
					if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[2] = hBtnQuantitativeAnalysisPeakRemember;
					}
				}
				else
				{
					if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisPeakRemember;
					}
				}
			}
			else 
			{
				if((gFunctionDefine & AUTOADJUST_SWITCH) != 0)
				{
					quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisAdjust;
					if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisPeakRemember;
					}
				}
				else
				{
					if((gFunctionDefine & PEAKMEMORY_SWITCH) != 0)
					{
						quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisPeakRemember;
					}
				}
				
			}
			
			/*
			quantitativeAnalysisBtnList[0] = hBtnQuantitativeAnalysisDgsCurve;
			quantitativeAnalysisBtnList[1] = hBtnQuantitativeAnalysisAutoGainPlus;
			quantitativeAnalysisBtnList[2] = hBtnQuantitativeAnalysisAdjust;
			quantitativeAnalysisBtnList[3] = hBtnQuantitativeAnalysisPeakRemember;
			*/

			//��ť�ؼ����Զ��崦�����
	//		OldBtnQuantitativeAnalysisDgsCurveProc = (WNDPROC) GetWindowLong(hBtnQuantitativeAnalysisDgsCurve,GWL_WNDPROC);
	//		SetWindowLong(hBtnQuantitativeAnalysisDgsCurve,GWL_WNDPROC,(LONG)btnQuantitativeAnalysisDgsCurveProc);
			
			OldBtnQuantitativeAnalysisAutoGainPlusProc = (WNDPROC) GetWindowLong(hBtnQuantitativeAnalysisAutoGainPlus,GWL_WNDPROC);
			SetWindowLong(hBtnQuantitativeAnalysisAutoGainPlus,GWL_WNDPROC,(LONG)btnQuantitativeAnalysisAutoGainPlusProc);

			OldBtnQuantitativeAnalysisAdjustProc = (WNDPROC) GetWindowLong(hBtnQuantitativeAnalysisAdjust,GWL_WNDPROC);
			SetWindowLong(hBtnQuantitativeAnalysisAdjust,GWL_WNDPROC,(LONG)btnQuantitativeAnalysisAdjustProc);

			
			OldBtnQuantitativeAnalysisPeakRememberProc = (WNDPROC) GetWindowLong(hBtnQuantitativeAnalysisPeakRemember,GWL_WNDPROC);
			SetWindowLong(hBtnQuantitativeAnalysisPeakRemember,GWL_WNDPROC,(LONG)btnQuantitativeAnalysisPeakRememberProc);

			currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[0];
			if(currentQuantitativeAnalysisBtn)
			{
				SetFocus(currentQuantitativeAnalysisBtn);//ѡ�а�ť������뽹��
				gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
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
				ShowWindow(hWndFnMenu,SW_SHOW);
				SetFocus(currentFnMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

				DestroyWindow(hWndQuantitativeAnalysis);
				hWndQuantitativeAnalysis = 0;
				
				gCurrentFocusBtn = currentFnMenuBtn;

				gCurrentWnd = hWndFnMenu;
			}
			break;
		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			quantitativeAnalysis_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnQuantitativeAnalysisAutoGainPlus)
			{
//				DrawLableText(pdis->hwndItem, _T("�Զ�����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			}
			if(pdis->hwndItem == hBtnQuantitativeAnalysisAdjust)
			{
//				DrawLableText(pdis->hwndItem, _T("У׼"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
			}
			if(pdis->hwndItem == hBtnQuantitativeAnalysisPeakRemember)
			{
//				DrawLableText(pdis->hwndItem, _T("��ֵ����"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y - 20);
				displayPeakRemember(pdis->hwndItem, gbPeakRememberFlag);
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
*���ض����������ڰ�ťͼƬ
* @quantitativeAnalysis    hInstance[in]:
* @quantitativeAnalysis		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int quantitativeAnalysis_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	BTN_BMP_INFO tbl_quantitativeAnalysisBtnBitmap[]=
	{
		{IDC_QUANTITATIVEANALYSIS_DGSCURVE	,IDB_BITMAP_DGSQUXIAN_NOSEL_R	 ,  IDB_BITMAP_DGSQUXIAN_SELECT_R		},
		{IDC_QUANTITATIVEANALYSIS_AUTOGAINPLUS	,IDB_BITMAP_ZIDONGZENGYI_NOSEL_R	 ,  IDB_BITMAP_ZIDONGZENGYI_SELECT_R		},
		{IDC_QUANTITATIVEANALYSIS_ADJUST	,IDB_BITMAP_JIAOZHUN_NOSEL_R	 ,  IDB_BITMAP_JIAOZHUN_SELECT_R		},
		{IDC_QUANTITATIVEANALYSIS_PEAKREMEMBER	,IDB_BITMAP_FENGZHIJIYI_NOSEL_R	 ,  IDB_BITMAP_FENGZHIJIYI_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 
	//���ù��ܹرյ�ͼƬ
	if((gFunctionDefine & DGSCURVE_SWITCH) == 0)
		tbl_quantitativeAnalysisBtnBitmap[0].nBtnNoFocID = IDB_BITMAP_DGSQUXIAN_DISABLE_R;
	if((gFunctionDefine & AUTOGAIN_SWITCH) == 0)
		tbl_quantitativeAnalysisBtnBitmap[1].nBtnNoFocID = IDB_BITMAP_ZIDONGZENGYI_DISABLE_R;
	if((gFunctionDefine & AUTOADJUST_SWITCH) == 0)
		tbl_quantitativeAnalysisBtnBitmap[2].nBtnNoFocID = IDB_BITMAP_ZIDONGJIAOZHUN_DISABLE_R;
	if((gFunctionDefine & PEAKMEMORY_SWITCH) == 0)
		tbl_quantitativeAnalysisBtnBitmap[3].nBtnNoFocID = IDB_BITMAP_FENGZHIJIYI_DISABLE_R;
	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_quantitativeAnalysisBtnBitmap);
	return ReturnVal;
}

/**************************
*DGS���߰�ť�Զ��崦�����
* @quantitativeAnalysis    hInstance[in]:
* @quantitativeAnalysis		message[in]:
* @quantitativeAnalysis		wParam[in]:
* @quantitativeAnalysis		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
#if 0
LRESULT CALLBACK btnQuantitativeAnalysisDgsCurveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
									
				/*	if((wFunctionControl & 0x0008) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisPeakRemember;
					else if((wFunctionControl & 0x0004) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAdjust;
					else if((wFunctionControl & 0x0002) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAutoGainPlus;
			
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
				*/
				}
				else if(wParam == VK_DOWN)
				{
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
	
				/*
					if((wFunctionControl & 0x0002) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAutoGainPlus;
					else if((wFunctionControl & 0x0004) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAdjust;
					else if((wFunctionControl & 0x0008) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisPeakRemember;
				*/
				}
				else if(wParam == VK_RETURN)
				{
				/*	hWndDgsCurve = CreateWindow(szDgsCurveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndQuantitativeAnalysis,SW_HIDE);
					//SetFocus(hWndDgsCurve);
					ShowWindow(hWndDgsCurve,SW_SHOW);
					UpdateWindow(hWndDgsCurve);
					*/
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
	return CallWindowProc(OldBtnQuantitativeAnalysisDgsCurveProc,Wnd,Msg,wParam,lParam);
}
#endif
/**************************
*�Զ����水ť�Զ��崦�����
* @quantitativeAnalysis    hInstance[in]:
* @quantitativeAnalysis		message[in]:
* @quantitativeAnalysis		wParam[in]:
* @quantitativeAnalysis		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnQuantitativeAnalysisAutoGainPlusProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
		
				/*	if((wFunctionControl & 0x0001) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisDgsCurve;
					else if((wFunctionControl & 0x0008) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisPeakRemember;
					else if((wFunctionControl & 0x0004) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAdjust;
				*/
				}
				else if(wParam == VK_DOWN)
				{
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
			
				/*	if((wFunctionControl & 0x0004) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAdjust;
					else if((wFunctionControl & 0x0008) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisPeakRemember;
					if((wFunctionControl & 0x0001) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisDgsCurve;
				*/
				}
				else if(wParam == VK_RETURN)
				{
					hWndAutoGainPlus = CreateWindow(szAutoGainPlusChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndQuantitativeAnalysis,SW_HIDE);
					//SetFocus(hWndAutoGainPlus);
					ShowWindow(hWndAutoGainPlus,SW_SHOW);
					UpdateWindow(hWndAutoGainPlus);

					gCurrentWnd = hWndAutoGainPlus;
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
	return CallWindowProc(OldBtnQuantitativeAnalysisAutoGainPlusProc,Wnd,Msg,wParam,lParam);
}
/**************************
*У׼��ť�Զ��崦�����
* @quantitativeAnalysis    hInstance[in]:
* @quantitativeAnalysis		message[in]:
* @quantitativeAnalysis		wParam[in]:
* @quantitativeAnalysis		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnQuantitativeAnalysisAdjustProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
				
				/*	if((wFunctionControl & 0x0008) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisPeakRemember;
					else if((wFunctionControl & 0x0001) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisDgsCurve;
					else if((wFunctionControl & 0x0002) == 1)
						currentQuantitativeAnalysisBtn = hBtnQuantitativeAnalysisAutoGainPlus;
				*/
				}
				else if(wParam == VK_DOWN)
				{
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndAdjust = CreateWindow(szAdjustChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndQuantitativeAnalysis,SW_HIDE);
					//SetFocus(hWndAdjust);
					ShowWindow(hWndAdjust,SW_SHOW);
					UpdateWindow(hWndAdjust);

					gCurrentWnd = hWndAdjust;
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
	return CallWindowProc(OldBtnQuantitativeAnalysisAdjustProc,Wnd,Msg,wParam,lParam);
}

/**************************
*У׼��ť�Զ��崦�����
* @quantitativeAnalysis    hInstance[in]:
* @quantitativeAnalysis		message[in]:
* @quantitativeAnalysis		wParam[in]:
* @quantitativeAnalysis		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnQuantitativeAnalysisPeakRememberProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(quantitativeAnalysisBtnList[i] != currentQuantitativeAnalysisBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentQuantitativeAnalysisBtn = quantitativeAnalysisBtnList[i];
					SetFocus(currentQuantitativeAnalysisBtn);
					gCurrentFocusBtn = currentQuantitativeAnalysisBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					InitializeCriticalSection(&csPeakRemember);
					EnterCriticalSection(&csPeakRemember);

					gbPeakRememberFlag = !gbPeakRememberFlag;

					LeaveCriticalSection(&csPeakRemember);
					DeleteCriticalSection(&csPeakRemember);

					displayPeakRemember(Wnd, gbPeakRememberFlag);					
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
	return CallWindowProc(OldBtnQuantitativeAnalysisPeakRememberProc,Wnd,Msg,wParam,lParam);
}

