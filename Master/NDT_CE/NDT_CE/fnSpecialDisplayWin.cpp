/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * Fn特殊显示窗口源文件
 * Filename: specialDisplayWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnSpecialDisplayWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"

#include "fnReferenceWaveWin.h"
#include "fnMenMeiZhanKuanWin.h"
#include "fnHuiBoBianMaWin.h"

HWND hWndSpecialDisplay;			//Fn特殊显示窗口句柄
HWND hBtnSpecialDisplayDynamicInclude;		//动态包络按钮句柄
HWND hBtnSpecialDisplayFreeze;		//冻结按钮句柄
HWND hBtnSpecialDisplayRefrenceWave;	//参考波按钮句柄
HWND hBtnSpecialDisplayHuiBoBianMa;	//回波编码按钮句柄
HWND hBtnSpecialDisplayMenNeiZhanKuan;	//门内展宽按钮句柄
HWND hBtnSpecialDisplay110Display;	// 110%显示按钮句柄

HWND specialDisplayBtnList[6];	//Fn特殊显示窗口的按钮表
WNDPROC OldBtnSpecialDisplayDynamicIncludeProc = NULL;//动态包络按钮默认处理过程
WNDPROC OldBtnSpecialDisplayFreezeProc = NULL;		//冻结按钮默认处理过程
WNDPROC OldBtnSpecialDisplayReferenceWaveProc = NULL;	//参考波按钮默认处理过程
WNDPROC OldBtnSpecialDisplayHuiBoBianMaProc = NULL;	//回波编码按钮默认处理过程
WNDPROC OldBtnSpecialDisplayMenMeiZhanKuanProc = NULL;	//门内展宽按钮默认处理过程
WNDPROC OldBtnSpecialDisplay110DisplayProc = NULL;	//110%显示按钮默认处理过程

 
HWND currentSpecialDisplayBtn;					//Fn特殊显示窗中当前选中的按钮

static WORD buttonNum = 0;						//按钮控件数
static WORD wReducedButtonNum = 0;			//因功能开关减少按扭数量

extern HINSTANCE hInst;						//程序实例句柄

extern HWND hWndHuiBoBianMa;				//回波编码窗口句柄
extern HWND hWndReferenceWave;				//参考波窗口句柄 
extern HWND hWndMenMeiZhanKuan;				//门内展宽波窗口句柄 
extern HWND hWndMain;
extern HWND hWndFnMenu;
extern HWND currentFnMenuBtn;
extern HWND gCurrentFocusBtn;
//songchenguang add start 2010-06-08
BOOL gbBaoluo = FALSE;		// 包络打开标志
extern CRITICAL_SECTION csBaoluo;
BOOL bFreezeUp = FALSE;
BOOL gb110Display = FALSE;
CRITICAL_SECTION cs110Display;
extern CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;
extern PIC_DATA gPicData[2];			// 图像数据

extern WORD CurrentChannelNo;


extern float fRangeStart[2];
extern float fRangeEnd[2];
extern CRITICAL_SECTION csZhanKuan;
extern BOOL gbZhanKuan;
extern float gZhanKuanFront;
extern float gZhanKuanBack;
extern CRITICAL_SECTION csZhanKuan;
extern CRITICAL_SECTION csZhanKuanValue;
extern GATE_INFO GateInfos[2][3];
//extern BOOL gbDisplayZhanKuanLine;
extern CRITICAL_SECTION csGate;

extern void SetZhuanKuanOpenFlag(HWND hWnd);
//songchenguang add end 2010-06-08
extern WORD gwDeviceType;				//设备类型 0:探伤仪	1:测厚仪
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern float gfMinRange[2];
extern HWND gCurrentWnd;		//当前显示的子窗口
extern DWORD gFunctionDefine;		//功能定义
/**************************
*注册Fn特殊显示窗口
* @specialDisplay    hInstance[in]:程序实例句柄
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerSpecialDisplayChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) specialDisplayChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szSpecialDisplayChild;

	return RegisterClass(&wc);
}
/**************************
*Fn特殊显示窗口处理过程
* @specialDisplay    hInstance[in]:
* @specialDisplay		message[in]:
* @specialDisplay		wParam[in]:
* @specialDisplay		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK specialDisplayChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			if(gwDeviceType == 0 || gwDeviceType == 2) //探伤或铁路
			{
				buttonNum = 6;
			//	gFunctionDefine = 	0x000002AA;
				wReducedButtonNum = 0;
				if((gFunctionDefine & DYNAMICINCLUDE_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & FREEZE_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & REFRENCEWAVE_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & HUIBOBIANMA_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & DISPLAY110_SWITCH) == 0)
					wReducedButtonNum ++;

				buttonNum = buttonNum - wReducedButtonNum;
				//画按钮控件
				hBtnSpecialDisplayDynamicInclude = CreateWindow(_T("BUTTON"),_T("dynamicIncludeSpecialDisplay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALDISPLAY_DYNAMICINCLUDE,hInst,NULL);
				hBtnSpecialDisplayFreeze = CreateWindow(_T("BUTTON"),_T("freezeSpecialDisplay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALDISPLAY_FREEZE,hInst,NULL);
				hBtnSpecialDisplayRefrenceWave = CreateWindow(_T("BUTTON"),_T("refrenceWaveSpecialDisplay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALDISPLAY_REFRENCEWAVE,hInst,NULL);
				hBtnSpecialDisplayHuiBoBianMa = CreateWindow(_T("BUTTON"),_T("HuiBoBianMaSpecialDisplay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALDISPLAY_HUIBOBIANMA,hInst,NULL);
				hBtnSpecialDisplayMenNeiZhanKuan = CreateWindow(_T("BUTTON"),_T("meiZhanKuanSpecialDisplay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALDISPLAY_MENMEIZHANKUAN,hInst,NULL);
				hBtnSpecialDisplay110Display = CreateWindow(_T("BUTTON"),_T("110DisplaySpecialDisplay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 5*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 5*BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALDISPLAY_110DISPLAY,hInst,NULL);
						
			/*	specialDisplayBtnList[0] = hBtnSpecialDisplayDynamicInclude;
				specialDisplayBtnList[1] = hBtnSpecialDisplayFreeze;
				specialDisplayBtnList[2] = hBtnSpecialDisplayRefrenceWave;
				specialDisplayBtnList[3] = hBtnSpecialDisplayHuiBoBianMa;
				specialDisplayBtnList[4] = hBtnSpecialDisplayMenNeiZhanKuan;
				specialDisplayBtnList[5] = hBtnSpecialDisplay110Display;
			*/	
				if((gFunctionDefine & DYNAMICINCLUDE_SWITCH) != 0)	//动态包络功能打开
				{
					specialDisplayBtnList[0] = hBtnSpecialDisplayDynamicInclude;
					if((gFunctionDefine & FREEZE_SWITCH) != 0)	//冻结功能打开
					{
						specialDisplayBtnList[1] = hBtnSpecialDisplayFreeze;
						if((gFunctionDefine & REFRENCEWAVE_SWITCH) != 0)
						{
							specialDisplayBtnList[2] = hBtnSpecialDisplayRefrenceWave;
							if((gFunctionDefine & HUIBOBIANMA_SWITCH) != 0)
							{
								specialDisplayBtnList[3] = hBtnSpecialDisplayHuiBoBianMa;
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[4] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[5] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[4] = hBtnSpecialDisplay110Display;
									}									
								}
							}
							else
							{
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[3] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[4] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[3] = hBtnSpecialDisplay110Display;
									}	
								}
							}
						}
						else
						{
							if((gFunctionDefine & HUIBOBIANMA_SWITCH) != 0)
							{
								specialDisplayBtnList[2] = hBtnSpecialDisplayHuiBoBianMa;
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[3] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[4] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[3] = hBtnSpecialDisplay110Display;
									}									
								}
							}
							else
							{
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[2] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[3] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[2] = hBtnSpecialDisplay110Display;
									}	
								}
							}
						}
					}
					else
					{
						if((gFunctionDefine & REFRENCEWAVE_SWITCH) != 0)
						{
							specialDisplayBtnList[1] = hBtnSpecialDisplayRefrenceWave;
							if((gFunctionDefine & HUIBOBIANMA_SWITCH) != 0)
							{
								specialDisplayBtnList[2] = hBtnSpecialDisplayHuiBoBianMa;
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[3] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[4] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[3] = hBtnSpecialDisplay110Display;
									}									
								}
							}
							else
							{
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[2] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[3] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[2] = hBtnSpecialDisplay110Display;
									}	
								}
							}
						}
						else
						{
							if((gFunctionDefine & HUIBOBIANMA_SWITCH) != 0)
							{
								specialDisplayBtnList[1] = hBtnSpecialDisplayHuiBoBianMa;
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[2] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[3] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[2] = hBtnSpecialDisplay110Display;
									}									
								}
							}
							else
							{
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[1] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[2] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[1] = hBtnSpecialDisplay110Display;
									}	
								}
							}
						}
					}
				}
				else	//动态包络功能关闭
				{
					if((gFunctionDefine & FREEZE_SWITCH) != 0)	//冻结功能打开
					{
						specialDisplayBtnList[0] = hBtnSpecialDisplayFreeze;
						if((gFunctionDefine & REFRENCEWAVE_SWITCH) != 0)
						{
							specialDisplayBtnList[1] = hBtnSpecialDisplayRefrenceWave;
							if((gFunctionDefine & HUIBOBIANMA_SWITCH) != 0)
							{
								specialDisplayBtnList[2] = hBtnSpecialDisplayHuiBoBianMa;
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[3] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[4] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[3] = hBtnSpecialDisplay110Display;
									}									
								}
							}
							else
							{
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[2] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[3] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[2] = hBtnSpecialDisplay110Display;
									}	
								}
							}
						}
						else
						{
							if((gFunctionDefine & HUIBOBIANMA_SWITCH) != 0)
							{
								specialDisplayBtnList[1] = hBtnSpecialDisplayHuiBoBianMa;
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[2] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[3] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[2] = hBtnSpecialDisplay110Display;
									}									
								}
							}
							else
							{
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[1] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[2] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[1] = hBtnSpecialDisplay110Display;
									}	
								}
							}
						}
					}
					else
					{
						if((gFunctionDefine & REFRENCEWAVE_SWITCH) != 0)
						{
							specialDisplayBtnList[0] = hBtnSpecialDisplayRefrenceWave;
							if((gFunctionDefine & HUIBOBIANMA_SWITCH) != 0)
							{
								specialDisplayBtnList[1] = hBtnSpecialDisplayHuiBoBianMa;
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[2] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[3] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[2] = hBtnSpecialDisplay110Display;
									}									
								}
							}
							else
							{
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[1] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[2] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[1] = hBtnSpecialDisplay110Display;
									}	
								}
							}
						}
						else
						{
							if((gFunctionDefine & HUIBOBIANMA_SWITCH) != 0)
							{
								specialDisplayBtnList[0] = hBtnSpecialDisplayHuiBoBianMa;
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[1] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[2] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[1] = hBtnSpecialDisplay110Display;
									}									
								}
							}
							else
							{
								if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) != 0)
								{
									specialDisplayBtnList[0] = hBtnSpecialDisplayMenNeiZhanKuan;
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[1] = hBtnSpecialDisplay110Display;
									}
								}
								else
								{
									if((gFunctionDefine & DISPLAY110_SWITCH) != 0)
									{
										specialDisplayBtnList[0] = hBtnSpecialDisplay110Display;
									}	
								}
							}
						}
					}			
				}
				//按钮控件的自定义处理过程
				OldBtnSpecialDisplayDynamicIncludeProc = (WNDPROC) GetWindowLong(hBtnSpecialDisplayDynamicInclude,GWL_WNDPROC);
				SetWindowLong(hBtnSpecialDisplayDynamicInclude,GWL_WNDPROC,(LONG)btnSpecialDisplayDynamicIncludeProc);
				
				OldBtnSpecialDisplayFreezeProc = (WNDPROC) GetWindowLong(hBtnSpecialDisplayFreeze,GWL_WNDPROC);
				SetWindowLong(hBtnSpecialDisplayFreeze,GWL_WNDPROC,(LONG)btnSpecialDisplayFreezeProc);
				
				OldBtnSpecialDisplayReferenceWaveProc = (WNDPROC) GetWindowLong(hBtnSpecialDisplayRefrenceWave,GWL_WNDPROC);
				SetWindowLong(hBtnSpecialDisplayRefrenceWave,GWL_WNDPROC,(LONG)btnSpecialDisplayReferenceWaveProc);
				
				OldBtnSpecialDisplayHuiBoBianMaProc = (WNDPROC) GetWindowLong(hBtnSpecialDisplayHuiBoBianMa,GWL_WNDPROC);
				SetWindowLong(hBtnSpecialDisplayHuiBoBianMa,GWL_WNDPROC,(LONG)btnSpecialDisplayHuiBoBianMaProc);

				OldBtnSpecialDisplayMenMeiZhanKuanProc = (WNDPROC) GetWindowLong(hBtnSpecialDisplayMenNeiZhanKuan, GWL_WNDPROC);
				SetWindowLong(hBtnSpecialDisplayMenNeiZhanKuan,GWL_WNDPROC,(LONG)btnSpecialDisplayMenMeiZhanKuanProc);
				
				OldBtnSpecialDisplay110DisplayProc = (WNDPROC) GetWindowLong(hBtnSpecialDisplay110Display,GWL_WNDPROC);
				SetWindowLong(hBtnSpecialDisplay110Display,GWL_WNDPROC,(LONG)btnSpecialDisplay110DisplayProc);
			
				currentSpecialDisplayBtn = specialDisplayBtnList[0];
				if(currentSpecialDisplayBtn)
				{
					SetFocus(currentSpecialDisplayBtn);//选中按钮获得输入焦点
					gCurrentFocusBtn = currentSpecialDisplayBtn;
				}
				else			//功能都关闭按钮的父窗口获得输入焦点
					SetFocus(hWnd);
			}
			if(gwDeviceType == 1)//测厚
			{
				buttonNum = 1;
				wReducedButtonNum = 0;
				if((gFunctionDefine & FREEZE_SWITCH) == 0)
					wReducedButtonNum ++;

				buttonNum = buttonNum - wReducedButtonNum;
				//画按钮控件
				hBtnSpecialDisplayFreeze = CreateWindow(_T("BUTTON"),_T("freezeSpecialDisplay"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALDISPLAY_FREEZE,hInst,NULL);
				if((gFunctionDefine & FREEZE_SWITCH) != 0)		
					specialDisplayBtnList[0] = hBtnSpecialDisplayFreeze;
				
				//按钮控件的自定义处理过程
				OldBtnSpecialDisplayFreezeProc = (WNDPROC) GetWindowLong(hBtnSpecialDisplayFreeze,GWL_WNDPROC);
				SetWindowLong(hBtnSpecialDisplayFreeze,GWL_WNDPROC,(LONG)btnSpecialDisplayFreezeProc);

				currentSpecialDisplayBtn = specialDisplayBtnList[0];
				if(currentSpecialDisplayBtn)
				{
					SetFocus(currentSpecialDisplayBtn);//选中按钮获得输入焦点
					gCurrentFocusBtn = currentSpecialDisplayBtn;
				}
				else			//功能都关闭按钮的父窗口获得输入焦点
					SetFocus(hWnd);
			}		
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndFnMenu,SW_SHOW);
				SetFocus(currentFnMenuBtn);//上级菜单最后选中的按钮获得输入焦点

				DestroyWindow(hWndSpecialDisplay);
				hWndSpecialDisplay = 0;
				
				gCurrentFocusBtn = currentFnMenuBtn;

				gCurrentWnd = hWndFnMenu;
			}
			break;
		case WM_COMMAND:
			/*	if(LOWORD(wParam) == IDC_SPECIALDISPLAY_DYNAMICINCLUDE)
				{
	//				currentSpecialDisplayBtn = specialDisplayBtnList[0]; 
	//				SetFocus(currentSpecialDisplayBtn);
	//				hWndDynamicIncludeSpecialDisplay = CreateWindow(szDynamicIncludeSpecialDisplayChild, TEXT(""), WS_CHILD|WS_VISIBLE,
	//						 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);

	//				ShowWindow(hWndSpecialDisplay,SW_HIDE);
					//SetFocus(hWndDynamicIncludeSpecialDisplay);
	//				ShowWindow(hWndDynamicIncludeSpecialDisplay,SW_SHOW);
	//				UpdateWindow(hWndDynamicIncludeSpecialDisplay);
				}
				else if(LOWORD(wParam) == IDC_SPECIALDISPLAY_FREEZE)
				{
					currentSpecialDisplayBtn = specialDisplayBtnList[1]; 
					SetFocus(currentSpecialDisplayBtn);
					hWndFreezeSpecialDisplay = CreateWindow(szFreezeSpecialDisplayChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndSpecialDisplay,SW_HIDE);
					//SetFocus(hWndFreezeSpecialDisplay);
					ShowWindow(hWndFreezeSpecialDisplay,SW_SHOW);
					UpdateWindow(hWndFreezeSpecialDisplay);
				}
				else if(LOWORD(wParam) == IDC_SPECIALDISPLAY_REFRENCEWAVE)
				{
	//				currentSpecialDisplayBtn = specialDisplayBtnList[2]; 
	//				SetFocus(currentSpecialDisplayBtn);
	//				hWndReferenceWave= CreateWindow(szRefrenceWaveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
	//						MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
	//				ShowWindow(hWndSpecialDisplay,SW_HIDE);
					//SetFocus(hWndReferenceWave);
	//				ShowWindow(hWndReferenceWave,SW_SHOW);
	//				UpdateWindow(hWndReferenceWave);
				}
				else if(LOWORD(wParam) == IDC_SPECIALDISPLAY_HUIBOBIANMA)
				{
	//				currentSpecialDisplayBtn = specialDisplayBtnList[3]; 
	//				SetFocus(currentSpecialDisplayBtn);
				}*/
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			specialDisplay_DrawButtonBitmap(hInst,pdis);
			if(pdis->hwndItem == hBtnSpecialDisplayDynamicInclude)
			{
				if(gbBaoluo)
					DrawLableText(pdis->hwndItem, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				else
				{
					DrawLableText(pdis->hwndItem, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
			}
			else if(pdis->hwndItem == hBtnSpecialDisplayFreeze)
			{					
				if(bFreezeUp)
				{
					DrawLableText(pdis->hwndItem, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else
				{
					DrawLableText(pdis->hwndItem, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
			}
			else if(pdis->hwndItem == hBtnSpecialDisplay110Display)
			{
				if(gb110Display)
					{
						DrawLableText(pdis->hwndItem, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					}
					else
					{
						DrawLableText(pdis->hwndItem, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					}
			}
			else if(pdis->hwndItem == hBtnSpecialDisplayMenNeiZhanKuan)
				SetZhuanKuanOpenFlag(hBtnSpecialDisplayMenNeiZhanKuan);
			break;
		case WM_CLOSE:
		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*加载Fn特殊显示窗口按钮图片
* @specialDisplay    hInstance[in]:
* @specialDisplay		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int specialDisplay_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	if(gwDeviceType == 0 || gwDeviceType == 2) //探伤或铁路
	{
		BTN_BMP_INFO tbl_specialDisplayBtnBitmap[]=
		{
			{IDC_SPECIALDISPLAY_DYNAMICINCLUDE	,IDB_BITMAP_DONGTAIBAOLUO_NOSEL_R	 ,  IDB_BITMAP_DONGTAIBAOLUO_SELECT_R		},
			{IDC_SPECIALDISPLAY_FREEZE	,IDB_BITMAP_DONGJIE_NOSEL_R	 ,  IDB_BITMAP_DONGJIE_SELECT_R		},
			{IDC_SPECIALDISPLAY_REFRENCEWAVE	,IDB_BITMAP_CANKAOBO_NOSEL_R ,  IDB_BITMAP_CANKAOBO_SELECT_R	},
			{IDC_SPECIALDISPLAY_HUIBOBIANMA	,IDB_BITMAP_HUIBOBIANMA_NOSEL_R ,  IDB_BITMAP_HUIBOBIANMA_SELECT_R	},
			{IDC_SPECIALDISPLAY_MENMEIZHANKUAN	,IDB_BITMAP_MENMEIZHANKUAN_NOSEL_R ,  IDB_BITMAP_MENMEIZHANKUAN_SELECT_R	},
			{IDC_SPECIALDISPLAY_110DISPLAY	,IDB_BITMAP_110DISPLAY_NOSEL_R ,  IDB_BITMAP_110DISPLAY_SELECT_R	},
			{	-1			,	-1					 ,	-1							}    
		};
		//设置功能关闭的图片
		if((gFunctionDefine & DYNAMICINCLUDE_SWITCH) == 0)
			tbl_specialDisplayBtnBitmap[0].nBtnNoFocID = IDB_BITMAP_DONGTAIBAOLUO_DISABLE_R;
		if((gFunctionDefine & FREEZE_SWITCH) == 0)
			tbl_specialDisplayBtnBitmap[1].nBtnNoFocID = IDB_BITMAP_DONGJIE_DISABLE_R;
		if((gFunctionDefine & REFRENCEWAVE_SWITCH) == 0)
			tbl_specialDisplayBtnBitmap[2].nBtnNoFocID = IDB_BITMAP_CANKAOBO_DISABLE_R;
		if((gFunctionDefine & HUIBOBIANMA_SWITCH) == 0)
			tbl_specialDisplayBtnBitmap[3].nBtnNoFocID = IDB_BITMAP_HUIBOBIANMA_DISABLE_R;
		if((gFunctionDefine & MENNEIZHANKUAN_SWITCH) == 0)
			tbl_specialDisplayBtnBitmap[4].nBtnNoFocID = IDB_BITMAP_MENNEIZHANKUAN_DISABLE_R;
		if((gFunctionDefine & DISPLAY110_SWITCH) == 0)
			tbl_specialDisplayBtnBitmap[5].nBtnNoFocID = IDB_BITMAP_110DISPLAY_DISABLE_R;
				
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_specialDisplayBtnBitmap);
	}
	if(gwDeviceType == 1)//测厚
	{
		BTN_BMP_INFO tbl_specialDisplayBtnBitmap[]=
		{
			{IDC_SPECIALDISPLAY_FREEZE	,IDB_BITMAP_DONGJIE_NOSEL_R	 ,  IDB_BITMAP_DONGJIE_SELECT_R		},
			{	-1			,	-1					 ,	-1							}    
		};
		if((gFunctionDefine & FREEZE_SWITCH) == 0)
			tbl_specialDisplayBtnBitmap[0].nBtnNoFocID = IDB_BITMAP_DONGJIE_DISABLE_R;
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_specialDisplayBtnBitmap);
	}
	return ReturnVal;
}

void CloseDynamicInclude()
{
	InitializeCriticalSection(&csBaoluo);
	EnterCriticalSection(&csBaoluo);

	gbBaoluo = FALSE;
	
	LeaveCriticalSection(&csBaoluo);
	DeleteCriticalSection(&csBaoluo);
	
	SetRedrawFlag(TRUE);
}

/**************************
*动态包络按钮自定义处理过程
* @specialDisplay    hInstance[in]:
* @specialDisplay		message[in]:
* @specialDisplay		wParam[in]:
* @specialDisplay		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnSpecialDisplayDynamicIncludeProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndSpecialDisplay);
					hWndSpecialDisplay = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;

					if(gb110Display)
						curve110To100Display();

					CloseDynamicInclude();

					bFreezeUp = FALSE;
					FreezeUp(hWnd, bFreezeUp);
				}
			
				else if(wParam == VK_UP)
				{
					while(specialDisplayBtnList[i] != currentSpecialDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSpecialDisplayBtn = specialDisplayBtnList[i];
					SetFocus(currentSpecialDisplayBtn);
					gCurrentFocusBtn = currentSpecialDisplayBtn;

					
					DrawLableText(hWnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					CloseDynamicInclude();
				}
				else if(wParam == VK_DOWN)
				{
					while(specialDisplayBtnList[i] != currentSpecialDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSpecialDisplayBtn = specialDisplayBtnList[i];
					SetFocus(currentSpecialDisplayBtn);
					gCurrentFocusBtn = currentSpecialDisplayBtn;

					DrawLableText(hWnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					CloseDynamicInclude();
				}
				else if(wParam == VK_RETURN)
				{
			//		hWndDynamicIncludeSpecialDisplay = CreateWindow(szDynamicIncludeSpecialDisplayChild, TEXT(""), WS_CHILD|WS_VISIBLE,
			//				 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
			//		ShowWindow(hWndSpecialDisplay,SW_HIDE);
			//		//SetFocus(hWndDynamicIncludeSpecialDisplay);
			//		ShowWindow(hWndDynamicIncludeSpecialDisplay,SW_SHOW);
			//		UpdateWindow(hWndDynamicIncludeSpecialDisplay);
				}
				//songchenguang add start 2010-06-08
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					InitializeCriticalSection(&csBaoluo);
					EnterCriticalSection(&csBaoluo);

					gbBaoluo = !gbBaoluo;
					
					if(gbBaoluo)
						DrawLableText(hWnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					else
					{
						DrawLableText(hWnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
						SetRedrawFlag(TRUE);
					}

					LeaveCriticalSection(&csBaoluo);
					DeleteCriticalSection(&csBaoluo);
					
				}
				//songchenguang add end 2010-06-08
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
				else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnSpecialDisplayDynamicIncludeProc,hWnd,Msg,wParam,lParam);
}


/**************************
*冻结按钮自定义处理过程
* @specialDisplay    hInstance[in]:
* @specialDisplay		message[in]:
* @specialDisplay		wParam[in]:
* @specialDisplay		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnSpecialDisplayFreezeProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
	case WM_CREATE:
	//	nAddStep = 0;
	//	memset(gGetAllPeakPointData, 0, sizeof(gGetAllPeakPointData));
	//	memset(&gCurrentPeakPointData, 0, sizeof(CURVEPOINT_DATA));
	//	memset(&gPrePeakPointData, 0, sizeof(gPrePeakPointData));
	//	nAllPeakPointDataNum = 0;
		break;
	case WM_KEYDOWN:
		{
			int i = 0;
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndFnMenu,SW_SHOW);
				SetFocus(currentFnMenuBtn);//上级菜单最后选中的按钮获得输入焦点

				DestroyWindow(hWndSpecialDisplay);
				hWndSpecialDisplay = 0;
				
				gCurrentFocusBtn = currentFnMenuBtn;

				gCurrentWnd = hWndFnMenu;

				InitializeCriticalSection(&csCommand);
				EnterCriticalSection(&csCommand);

				thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;

				LeaveCriticalSection(&csCommand);
				DeleteCriticalSection(&csCommand);

				if(gb110Display)
					curve110To100Display();

				bFreezeUp = FALSE;
				FreezeUp(hWnd, bFreezeUp);
			}		
			else if(wParam == VK_UP)
			{
				if(gwDeviceType == 0 || gwDeviceType == 2) //探伤或铁路
				{
					while(specialDisplayBtnList[i] != currentSpecialDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSpecialDisplayBtn = specialDisplayBtnList[i];
					SetFocus(currentSpecialDisplayBtn);
					gCurrentFocusBtn = currentSpecialDisplayBtn;

					bFreezeUp = FALSE;
					FreezeUp(hWnd, bFreezeUp);
				}
			}
			else if(wParam == VK_DOWN)
			{
				if(gwDeviceType == 0 || gwDeviceType == 2) //探伤或铁路
				{
					while(specialDisplayBtnList[i] != currentSpecialDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSpecialDisplayBtn = specialDisplayBtnList[i];
					SetFocus(currentSpecialDisplayBtn);
					gCurrentFocusBtn = currentSpecialDisplayBtn;

					bFreezeUp = FALSE;
					FreezeUp(hWnd, bFreezeUp);
				}
			}
			// songchenguang add start 2010-06-18
			else if(wParam == VK_RETURN)
			{
				bFreezeUp = !bFreezeUp;

				if(bFreezeUp)
				{
				/*	if(gwDeviceType == 0)
						DrawLableText(hWnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					else if(gwDeviceType == 1)
						DrawLableText(hWnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
						*/
					DrawLableText(hWnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}
				else
				{
				/*	if(gwDeviceType == 0)
						DrawLableText(hWnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
					else if(gwDeviceType == 1)
						DrawLableText(hWnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
						*/
					DrawLableText(hWnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
				}

				FreezeUp(hWnd, bFreezeUp);
			}
			else if(wParam == VK_LEFT)
			{
				// 左移波峰点
				if(bFreezeUp)
				{
					FreezeLeftMove();
					DrawPeakInfo();
				}
			}
			else if(wParam == VK_RIGHT)
			{
				// 右移波峰点
				if(bFreezeUp)
				{
					FreezeRightMove();
					DrawPeakInfo();
				}
			}
			// songchenguang add end 2010-06-18
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
			else if(wParam == 'B')	// 冻结
			{
				SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
			}
		}
		break;
	default:
		break;
	}
	return CallWindowProc(OldBtnSpecialDisplayFreezeProc,hWnd,Msg,wParam,lParam);
}
/**************************
*参考波按钮自定义处理过程
* @specialDisplay    hInstance[in]:
* @specialDisplay		message[in]:
* @specialDisplay		wParam[in]:
* @specialDisplay		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnSpecialDisplayReferenceWaveProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndSpecialDisplay);
					hWndSpecialDisplay = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;

					if(gb110Display)
						curve110To100Display();

					bFreezeUp = FALSE;
					FreezeUp(Wnd, bFreezeUp);

				}
			
				else if(wParam == VK_UP)
				{
					while(specialDisplayBtnList[i] != currentSpecialDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSpecialDisplayBtn = specialDisplayBtnList[i];
					SetFocus(currentSpecialDisplayBtn);
					gCurrentFocusBtn = currentSpecialDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(specialDisplayBtnList[i] != currentSpecialDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSpecialDisplayBtn = specialDisplayBtnList[i];
					SetFocus(currentSpecialDisplayBtn);
					gCurrentFocusBtn = currentSpecialDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndReferenceWave= CreateWindow(szReferenceWaveChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndSpecialDisplay,SW_HIDE);
					//SetFocus(hWndReferenceWave);
					ShowWindow(hWndReferenceWave,SW_SHOW);
					UpdateWindow(hWndReferenceWave);

					gCurrentWnd = hWndReferenceWave;
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
				else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnSpecialDisplayReferenceWaveProc,Wnd,Msg,wParam,lParam);
}
/**************************
*回波编码按钮自定义处理过程
* @specialDisplay    hInstance[in]:
* @specialDisplay		message[in]:
* @specialDisplay		wParam[in]:
* @specialDisplay		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnSpecialDisplayHuiBoBianMaProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndSpecialDisplay);
					hWndSpecialDisplay = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;

					if(gb110Display)
						curve110To100Display();

					bFreezeUp = FALSE;
					FreezeUp(Wnd, bFreezeUp);

				}
			
				else if(wParam == VK_UP)
				{
					while(specialDisplayBtnList[i] != currentSpecialDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSpecialDisplayBtn = specialDisplayBtnList[i];
					SetFocus(currentSpecialDisplayBtn);
					gCurrentFocusBtn = currentSpecialDisplayBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(specialDisplayBtnList[i] != currentSpecialDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSpecialDisplayBtn = specialDisplayBtnList[i];
					SetFocus(currentSpecialDisplayBtn);
					gCurrentFocusBtn = currentSpecialDisplayBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndHuiBoBianMa= CreateWindow(szHuiBoBianMaChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndSpecialDisplay,SW_HIDE);
					//SetFocus(hWndHuiBoBianMa);
					ShowWindow(hWndHuiBoBianMa,SW_SHOW);
					UpdateWindow(hWndHuiBoBianMa);

					gCurrentWnd = hWndHuiBoBianMa;
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
				else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnSpecialDisplayHuiBoBianMaProc,Wnd,Msg,wParam,lParam);
}

void CloseMenNeiZhanKuan()
{
	// songchenguang add start 2010-07-09
	InitializeCriticalSection(&csZhanKuan);
	EnterCriticalSection(&csZhanKuan);

	gbZhanKuan = FALSE;
	LeaveCriticalSection(&csZhanKuan);
	DeleteCriticalSection(&csZhanKuan);
/*
	InitializeCriticalSection(&csZhanKuanValue);
	EnterCriticalSection(&csZhanKuanValue);
	
	gbDisplayZhanKuanLine = FALSE;
	LeaveCriticalSection(&csZhanKuanValue);
	DeleteCriticalSection(&csZhanKuanValue);
*/	
	SetRedrawFlag(TRUE);
	// songchenguang add end 2010-07-09
}
/**************************
* 门内展宽按钮自定义处理过程
* @specialDisplay    hInstance[in]:
* @specialDisplay		message[in]:
* @specialDisplay		wParam[in]:
* @specialDisplay		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnSpecialDisplayMenMeiZhanKuanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	float fFront = 0.0f;
	float fBack = 0.0f;
	switch(Msg)
	{
	case WM_CREATE:

		
		break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndSpecialDisplay);
					hWndSpecialDisplay = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;

					if(gb110Display)
						curve110To100Display();

					bFreezeUp = FALSE;
					FreezeUp(Wnd, bFreezeUp);

					CloseMenNeiZhanKuan();
				}
			
				else if(wParam == VK_UP)
				{
					while(specialDisplayBtnList[i] != currentSpecialDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSpecialDisplayBtn = specialDisplayBtnList[i];
					SetFocus(currentSpecialDisplayBtn);
					gCurrentFocusBtn = currentSpecialDisplayBtn;

					CloseMenNeiZhanKuan();
				}
				else if(wParam == VK_DOWN)
				{
					while(specialDisplayBtnList[i] != currentSpecialDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSpecialDisplayBtn = specialDisplayBtnList[i];
					SetFocus(currentSpecialDisplayBtn);
					gCurrentFocusBtn = currentSpecialDisplayBtn;

					CloseMenNeiZhanKuan();
				}
				else if(wParam == VK_RETURN)
				{
/*
					hWndMenMeiZhanKuan= CreateWindow(szMenMeiZhanKuanChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndSpecialDisplay,SW_HIDE);
					//SetFocus(hWndMenMeiZhanKuan);
					ShowWindow(hWndMenMeiZhanKuan,SW_SHOW);
					UpdateWindow(hWndMenMeiZhanKuan);

					gCurrentWnd = hWndMenMeiZhanKuan;
*/
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					// songchenguang add start 2010-06-18
					InitializeCriticalSection(&csGate);
					EnterCriticalSection(&csGate);
					fFront = GateInfos[CurrentChannelNo][0].fFrontGate;
					fBack = GateInfos[CurrentChannelNo][0].fBackGate;
					LeaveCriticalSection(&csGate);
					DeleteCriticalSection(&csGate);

					InitializeCriticalSection(&csZhanKuanValue);
					EnterCriticalSection(&csZhanKuanValue);
					gZhanKuanFront = fFront;
					gZhanKuanBack = fBack;

					LeaveCriticalSection(&csZhanKuanValue);
					DeleteCriticalSection(&csZhanKuanValue);

					InitializeCriticalSection(&csZhanKuan);
					EnterCriticalSection(&csZhanKuan);
					gbZhanKuan = !gbZhanKuan;

					LeaveCriticalSection(&csZhanKuan);
					DeleteCriticalSection(&csZhanKuan);

					SetZhuanKuanOpenFlag(Wnd);

					SetRedrawFlag(TRUE);
					// songchenguang add end 2010-06-18
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
				else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnSpecialDisplayMenMeiZhanKuanProc,Wnd,Msg,wParam,lParam);
}

void Close110Display()
{
	InitializeCriticalSection(&cs110Display);
	EnterCriticalSection(&cs110Display);

	gb110Display = FALSE;

	LeaveCriticalSection(&cs110Display);
	DeleteCriticalSection(&cs110Display);

	SetRedrawFlag(TRUE);
}

/**************************
* 110%显示按钮自定义处理过程
* @specialDisplay    hInstance[in]:
* @specialDisplay		message[in]:
* @specialDisplay		wParam[in]:
* @specialDisplay		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnSpecialDisplay110DisplayProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//上级菜单最后选中的按钮获得输入焦点

					DestroyWindow(hWndSpecialDisplay);
					hWndSpecialDisplay = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;
					
					if(gb110Display)
						curve110To100Display();

					Close110Display();

					bFreezeUp = FALSE;
					FreezeUp(hWnd, bFreezeUp);
				}
			
				else if(wParam == VK_UP)
				{
					while(specialDisplayBtnList[i] != currentSpecialDisplayBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSpecialDisplayBtn = specialDisplayBtnList[i];
					SetFocus(currentSpecialDisplayBtn);
					gCurrentFocusBtn = currentSpecialDisplayBtn;

					Close110Display();
				}
				else if(wParam == VK_DOWN)
				{
					while(specialDisplayBtnList[i] != currentSpecialDisplayBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSpecialDisplayBtn = specialDisplayBtnList[i];
					SetFocus(currentSpecialDisplayBtn);
					gCurrentFocusBtn = currentSpecialDisplayBtn;

					Close110Display();
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT || wParam == VK_RIGHT)
				{
					InitializeCriticalSection(&cs110Display);
					EnterCriticalSection(&cs110Display);

					gb110Display = !gb110Display;

					if(gb110Display)
					{
						DrawLableText(hWnd, _T("开启"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);

						curve100To110Display();						
					}
					else
					{
						DrawLableText(hWnd, _T("关闭"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_6_START_Y);
						
						curve110To100Display();
					}

					LeaveCriticalSection(&cs110Display);
					DeleteCriticalSection(&cs110Display);

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
				else if(wParam == 'B')	// 冻结
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnSpecialDisplay110DisplayProc,hWnd,Msg,wParam,lParam);
}