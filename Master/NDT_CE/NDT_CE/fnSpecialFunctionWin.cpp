/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ר�ù��ܴ���Դ�ļ�
 * Filename: fnSpecialFunctionWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "fnSpecialFunctionWin.h"
#include "interfaceCmdID.h"
#include "win_com.h"
#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"


#include "fnBScanWin.h"
#include "fnZhongHouBanTanShangWin.h"
#include "fnHanFengTanShangWin.h"
//#include "fnZhuJianTanShangWin.h"

HWND hWndSpecialFunction;						//ר�ù��ܴ��ھ��
HWND hBtnSpecialFunctionBScan;					//Bɨ�谴ť���
HWND hBtnSpecialFunctionZhongHouBanTanShang;	//�к��̽�˰�ť���
HWND hBtnSpecialFunctionHanFengTanShang;		//����̽�˰�ť���
HWND hBtnSpecialFunctionFuHeCaiLiaoJianCe;		//���ϲ��ϼ�ⰴť���
HWND hBtnSpecialFunctionZhuJianTanShang;		//������ť���
HWND hBtnSpecialFunctionLieWenCeGao;			//���Ʋ�߰�ť���

HWND specialFunctionBtnList[6];	//ר�ù��ܴ��ڵİ�ť��
WNDPROC OldBtnSpecialFunctionBScanProc = NULL;						//Bɨ�谴ťĬ�ϴ������
WNDPROC OldBtnSpecialFunctionZhongHouBanTanShangProc = NULL;		//�к��̽�˰�ťĬ�ϴ������
WNDPROC OldBtnSpecialFunctionHanFengTanShangProc = NULL;			//����̽�˰�ťĬ�ϴ������
WNDPROC OldBtnSpecialFunctionFuHeCaiLiaoJianCeProc = NULL;			//���ϲ��ϼ�ⰴťĬ�ϴ������
WNDPROC OldBtnSpecialFunctionZhuJianTanShangProc = NULL;			//������ťĬ�ϴ������
WNDPROC OldBtnSpecialFunctionLieWenCeGaoProc = NULL;				//���Ʋ�߰�ťĬ�ϴ������

 
HWND currentSpecialFunctionBtn;					//ר�ù��ܴ��е�ǰѡ�еİ�ť

static WORD buttonNum = 0;		//��ť�ؼ���
static WORD wReducedButtonNum = 0;			//���ܿ��ؼ��ٰ�Ť����

extern HINSTANCE hInst;		//����ʵ�����

extern HWND hWndBScan; 						//Bɨ�贰�ھ��
extern HWND hWndZhongHouBanTanShang; 		//�к��̽�˴��ھ��
extern HWND hWndZhuJianTanShang;			//�������ھ��
extern HWND hWndHanFengTanShang;			//����̽�˴��ھ�� 
extern HWND hWndFuHeCaiLiaoJianCe;			//���ϲ��ϼ�ⴰ�ھ��
extern HWND hWndLieWenCeGao;				//���Ʋ�ߴ��ھ��

extern HWND hWndMain;
extern HWND hWndFnMenu;
extern HWND currentFnMenuBtn;
extern HWND gCurrentFocusBtn;

extern WORD gwDeviceType;				//�豸���� 0:̽����	1:�����
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���
extern DWORD gFunctionDefine;		//���ܶ���

/**************************
*ע��ר�ù��ܴ���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerSpecialFunctionChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) specialFunctionChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szSpecialFunctionChild;

	return RegisterClass(&wc);
}
/**************************
*ר�ù��ܴ��ڴ������
* @param		Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK specialFunctionChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
				buttonNum = 6;
				wReducedButtonNum = 0;
				if((gFunctionDefine & BSCAN_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & ZHONGHOUBANTANSHANG_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & HANFENGTANSHANG_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) == 0)
					wReducedButtonNum ++;
				if((gFunctionDefine & LIEWENCEGAO_SWITCH) == 0)
					wReducedButtonNum ++;

				buttonNum = buttonNum - wReducedButtonNum;

				//����ť�ؼ�
				hBtnSpecialFunctionBScan = CreateWindow(_T("BUTTON"),_T("BScanSpecialFunction"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALFUNCTION_BSCAN,hInst,NULL);
				hBtnSpecialFunctionZhongHouBanTanShang = CreateWindow(_T("BUTTON"),_T("zhongHouBanTanShangSpecialFunction"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALFUNCTION_ZHONGHOUBANTANSHANG,hInst,NULL);
				hBtnSpecialFunctionHanFengTanShang = CreateWindow(_T("BUTTON"),_T("hanFengTanShangSpecialFunction"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 2*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALFUNCTION_HANFENGTANSHANG,hInst,NULL);
				hBtnSpecialFunctionFuHeCaiLiaoJianCe = CreateWindow(_T("BUTTON"),_T("fuHeCaiLiaoJianCeSpecialFunction"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 3*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALFUNCTION_FUHECAILIAOJIANCE,hInst,NULL);
				hBtnSpecialFunctionZhuJianTanShang = CreateWindow(_T("BUTTON"),_T("ZhuJianTanShangSpecialFunction"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 4*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALFUNCTION_ZHUJIANTANSHANG,hInst,NULL);
				hBtnSpecialFunctionLieWenCeGao = CreateWindow(_T("BUTTON"),_T("lieWenCeGaoSpecialFunction"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0 + 5*BUTTON_6_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 5*BUTTON_6_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALFUNCTION_LIEWENCEGAO,hInst,NULL);
				/*
				specialFunctionBtnList[0] = hBtnSpecialFunctionBScan;
				specialFunctionBtnList[1] = hBtnSpecialFunctionZhongHouBanTanShang;
				specialFunctionBtnList[2] = hBtnSpecialFunctionHanFengTanShang;
				specialFunctionBtnList[3] = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
				specialFunctionBtnList[4] = hBtnSpecialFunctionZhuJianTanShang;
				*/

				if((gFunctionDefine & BSCAN_SWITCH) != 0)	//��̬���繦�ܴ�
				{
					specialFunctionBtnList[0] = hBtnSpecialFunctionBScan;
					if((gFunctionDefine & ZHONGHOUBANTANSHANG_SWITCH) != 0)	//���Ṧ�ܴ�
					{
						specialFunctionBtnList[1] = hBtnSpecialFunctionZhongHouBanTanShang;
						if((gFunctionDefine & HANFENGTANSHANG_SWITCH) != 0)
						{
							specialFunctionBtnList[2] = hBtnSpecialFunctionHanFengTanShang;
							if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
							{
								specialFunctionBtnList[3] = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[4] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[5] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[4] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
							else
							{
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[3] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[4] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[3] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
						}
						else
						{
							if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
							{
								specialFunctionBtnList[2] = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[3] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[4] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[3] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
							else
							{
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[2] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[3] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[2] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
						}
					}
					else
					{
						if((gFunctionDefine & HANFENGTANSHANG_SWITCH) != 0)
						{
							specialFunctionBtnList[1] = hBtnSpecialFunctionHanFengTanShang;
							if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
							{
								specialFunctionBtnList[2] = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[3] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[4] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[3] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
							else
							{
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[2] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[3] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[2] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
						}
						else
						{
							if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
							{
								specialFunctionBtnList[1] = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[2] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[3] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[2] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
							else
							{
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[1] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[2] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[1] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
						}
					}
				}
				else
				{
					if((gFunctionDefine & ZHONGHOUBANTANSHANG_SWITCH) != 0)	//���Ṧ�ܴ�
					{
						specialFunctionBtnList[0] = hBtnSpecialFunctionZhongHouBanTanShang;
						if((gFunctionDefine & HANFENGTANSHANG_SWITCH) != 0)
						{
							specialFunctionBtnList[1] = hBtnSpecialFunctionHanFengTanShang;
							if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
							{
								specialFunctionBtnList[2] = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[3] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[4] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[3] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
							else
							{
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[2] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[3] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[2] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
						}
						else
						{
							if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
							{
								specialFunctionBtnList[1] = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[2] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[3] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[2] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
							else
							{
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[1] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[2] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[1] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
						}
					}
					else
					{
						if((gFunctionDefine & HANFENGTANSHANG_SWITCH) != 0)
						{
							specialFunctionBtnList[0] = hBtnSpecialFunctionHanFengTanShang;
							if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
							{
								specialFunctionBtnList[1] = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[2] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[3] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[2] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
							else
							{
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[1] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[2] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[1] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
						}
						else
						{
							if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
							{
								specialFunctionBtnList[0] = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[1] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[2] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[1] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
							else
							{
								if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
								{
									specialFunctionBtnList[0] = hBtnSpecialFunctionZhuJianTanShang;
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[1] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
								else
								{
									if((gFunctionDefine & LIEWENCEGAO_SWITCH) != 0)
									{
										specialFunctionBtnList[0] = hBtnSpecialFunctionLieWenCeGao;
									}
								}
							}
						}
					}
				}
				
				//��ť�ؼ����Զ��崦�����
				OldBtnSpecialFunctionBScanProc = (WNDPROC) GetWindowLong(hBtnSpecialFunctionBScan,GWL_WNDPROC);
				SetWindowLong(hBtnSpecialFunctionBScan,GWL_WNDPROC,(LONG)btnSpecialFunctionBScanProc);
				
				OldBtnSpecialFunctionZhongHouBanTanShangProc = (WNDPROC) GetWindowLong(hBtnSpecialFunctionZhongHouBanTanShang,GWL_WNDPROC);
				SetWindowLong(hBtnSpecialFunctionZhongHouBanTanShang,GWL_WNDPROC,(LONG)btnSpecialFunctionZhongHouBanTanShangProc);
				
				OldBtnSpecialFunctionHanFengTanShangProc = (WNDPROC) GetWindowLong(hBtnSpecialFunctionHanFengTanShang,GWL_WNDPROC);
				SetWindowLong(hBtnSpecialFunctionHanFengTanShang,GWL_WNDPROC,(LONG)btnSpecialFunctionHanFengTanShangProc);
				
				OldBtnSpecialFunctionFuHeCaiLiaoJianCeProc = (WNDPROC) GetWindowLong(hBtnSpecialFunctionFuHeCaiLiaoJianCe,GWL_WNDPROC);
				SetWindowLong(hBtnSpecialFunctionFuHeCaiLiaoJianCe,GWL_WNDPROC,(LONG)btnSpecialFunctionFuHeCaiLiaoJianCeProc);

				OldBtnSpecialFunctionZhuJianTanShangProc = (WNDPROC) GetWindowLong(hBtnSpecialFunctionZhuJianTanShang,GWL_WNDPROC);
				SetWindowLong(hBtnSpecialFunctionZhuJianTanShang,GWL_WNDPROC,(LONG)btnSpecialFunctionZhuJianTanShangProc);

				OldBtnSpecialFunctionLieWenCeGaoProc = (WNDPROC) GetWindowLong(hBtnSpecialFunctionLieWenCeGao,GWL_WNDPROC);
				SetWindowLong(hBtnSpecialFunctionLieWenCeGao,GWL_WNDPROC,(LONG)btnSpecialFunctionLieWenCeGaoProc);
			
				currentSpecialFunctionBtn = specialFunctionBtnList[0];
				if(currentSpecialFunctionBtn)
				{
					SetFocus(currentSpecialFunctionBtn);//ѡ�а�ť������뽹��
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else			//���ܶ��رհ�ť�ĸ����ڻ�����뽹��
					SetFocus(hWnd);
			}
			if(gwDeviceType == 1)//���
			{
				buttonNum = 1;
				wReducedButtonNum = 0;
				if((gFunctionDefine & BSCAN_SWITCH) == 0)
					wReducedButtonNum ++;

				buttonNum = buttonNum - wReducedButtonNum;
				//����ť�ؼ�
				hBtnSpecialFunctionBScan = CreateWindow(_T("BUTTON"),_T("BScanSpecialFunction"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
						0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_SPECIALFUNCTION_BSCAN,hInst,NULL);	
				
				specialFunctionBtnList[0] = hBtnSpecialFunctionBScan;				
				
				//��ť�ؼ����Զ��崦�����
				OldBtnSpecialFunctionBScanProc = (WNDPROC) GetWindowLong(hBtnSpecialFunctionBScan,GWL_WNDPROC);
				SetWindowLong(hBtnSpecialFunctionBScan,GWL_WNDPROC,(LONG)btnSpecialFunctionBScanProc);
			
				currentSpecialFunctionBtn = specialFunctionBtnList[0];//Ĭ�ϵ�ǰѡ��Bɨ�谴ť

				currentSpecialFunctionBtn = specialFunctionBtnList[0];
				if(currentSpecialFunctionBtn)
				{
					SetFocus(currentSpecialFunctionBtn);//ѡ�а�ť������뽹��
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else			//���ܶ��رհ�ť�ĸ����ڻ�����뽹��
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
				SetFocus(currentFnMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

				DestroyWindow(hWndSpecialFunction);
				hWndSpecialFunction = 0;
				
				gCurrentFocusBtn = currentFnMenuBtn;

				gCurrentWnd = hWndFnMenu;
			}
			break;
		case WM_COMMAND:
			/*	if(LOWORD(wParam) == IDC_SPECIALFUNCTION_BSCAN)
				{
	//				currentSpecialFunctionBtn = specialFunctionBtnList[0]; 
	//				SetFocus(currentSpecialFunctionBtn);
	//				hWndBScan = CreateWindow(szBScanChild, TEXT(""), WS_CHILD|WS_VISIBLE,
	//						 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);

	//				ShowWindow(hWndSpecialFunction,SW_HIDE);
					//SetFocus(hWndBScan);
	//				ShowWindow(hWndBScan,SW_SHOW);
	//				UpdateWindow(hWndBScan);
				}
				else if(LOWORD(wParam) == IDC_SPECIALFUNCTION_ZHONGHOUBANTANSHANG)
				{
					currentSpecialFunctionBtn = specialFunctionBtnList[1]; 
					SetFocus(currentSpecialFunctionBtn);
					hWndZhongHouBanTanShang = CreateWindow(szZhongHouBanTanShangChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndSpecialFunction,SW_HIDE);
					//SetFocus(hWndZhongHouBanTanShang);
					ShowWindow(hWndZhongHouBanTanShang,SW_SHOW);
					UpdateWindow(hWndZhongHouBanTanShang);
				}
				else if(LOWORD(wParam) == IDC_SPECIALFUNCTION_HANFENGTANSHANG)
				{
	//				currentSpecialFunctionBtn = specialFunctionBtnList[2]; 
	//				SetFocus(currentSpecialFunctionBtn);
	//				hWndHanFengTanShang = CreateWindow(szHanFengTanShangChild, TEXT(""), WS_CHILD|WS_VISIBLE,
	//						MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
	//				ShowWindow(hWndSpecialFunction,SW_HIDE);
					//SetFocus(hWndHanFengTanShang);
	//				ShowWindow(hWndHanFengTanShang,SW_SHOW);
	//				UpdateWindow(hWndHanFengTanShang);
				}
				else if(LOWORD(wParam) == IDC_SPECIALFUNCTION_ZHUJIANTANSHANG)
				{
	//				currentSpecialFunctionBtn = specialFunctionBtnList[3]; 
	//				SetFocus(currentSpecialFunctionBtn);
				}*/
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			specialFunction_DrawButtonBitmap(hInst,pdis);
			break;
		case WM_CLOSE:
		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*����ר�ù��ܴ��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int specialFunction_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	if(gwDeviceType == 0 || gwDeviceType == 2) //̽�˻���·
	{
		BTN_BMP_INFO tbl_specialFunctionBtnBitmap[]=
		{
			{IDC_SPECIALFUNCTION_BSCAN	,IDB_BITMAP_BSCAN_NOSEL_R	 ,  IDB_BITMAP_BSCAN_SELECT_R		},
			{IDC_SPECIALFUNCTION_ZHONGHOUBANTANSHANG	,IDB_BITMAP_ZHONGHOUBANTANSHANG_NOSEL_R	 ,  IDB_BITMAP_ZHONGHOUBANTANSHANG_SELECT_R		},
			{IDC_SPECIALFUNCTION_HANFENGTANSHANG	,IDB_BITMAP_HANFENGTANSHANG_NOSEL_R ,  IDB_BITMAP_HANFENGTANSHANG_SELECT_R	},
			{IDC_SPECIALFUNCTION_FUHECAILIAOJIANCE	,IDB_BITMAP_FUHECAILIAOJIANCE_NOSEL_R ,  IDB_BITMAP_FUHECAILIAOJIANCE_SELECT_R	},
			{IDC_SPECIALFUNCTION_ZHUJIANTANSHANG	,IDB_BITMAP_ZHUJIAN_NOSEL_R ,  IDB_BITMAP_ZHUJIAN_SELECT_R	},
			{IDC_SPECIALFUNCTION_LIEWENCEGAO	,IDB_BITMAP_LIEWENCEGAO_NOSEL_R ,  IDB_BITMAP_LIEWENCEGAO_SELECT_R	},
			{	-1			,	-1					 ,	-1							}    
		} ; 
		if((gFunctionDefine & BSCAN_SWITCH) == 0)
			tbl_specialFunctionBtnBitmap[0].nBtnNoFocID = IDB_BITMAP_BSAOMIAO_DISABLE_R;
		if((gFunctionDefine & ZHONGHOUBANTANSHANG_SWITCH) == 0)
			tbl_specialFunctionBtnBitmap[1].nBtnNoFocID = IDB_BITMAP_ZHONGHOUBANTANSHANG_DISABLE_R;
		if((gFunctionDefine & HANFENGTANSHANG_SWITCH) == 0)
			tbl_specialFunctionBtnBitmap[2].nBtnNoFocID = IDB_BITMAP_HANFENGTANSHANG_DISABLE_R;
		if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) == 0)
			tbl_specialFunctionBtnBitmap[3].nBtnNoFocID = IDB_BITMAP_FUHECAILIAOJIANCE_DISABLE_R;
		if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) == 0)
			tbl_specialFunctionBtnBitmap[4].nBtnNoFocID = IDB_BITMAP_DUANJIANTANSHANG_DISABLE_R;
		if((gFunctionDefine & LIEWENCEGAO_SWITCH) == 0)
			tbl_specialFunctionBtnBitmap[5].nBtnNoFocID = IDB_BITMAP_LIEWENCEGAO_DISABLE_R;
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_specialFunctionBtnBitmap);
	}
	if(gwDeviceType == 1)//���
	{
		BTN_BMP_INFO tbl_specialFunctionBtnBitmap[]=
		{
			{IDC_SPECIALFUNCTION_BSCAN	,IDB_BITMAP_BSCAN_NOSEL_R	 ,  IDB_BITMAP_BSCAN_SELECT_R		},
			{	-1			,	-1					 ,	-1							}    
		} ; 
		if((gFunctionDefine & BSCAN_SWITCH) == 0)
			tbl_specialFunctionBtnBitmap[0].nBtnNoFocID = IDB_BITMAP_BSAOMIAO_DISABLE_R;

		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_specialFunctionBtnBitmap);
	}
	return ReturnVal;
}
/**************************
*Bɨ�谴ť�Զ��崦�����
* @param		Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnSpecialFunctionBScanProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndSpecialFunction);
					hWndSpecialFunction = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(specialFunctionBtnList[i] != currentSpecialFunctionBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSpecialFunctionBtn = specialFunctionBtnList[i];
					
				
				/*	if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhuJianTanShang;
					else if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
					else if((gFunctionDefine & HANFENGTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionHanFengTanShang;
					else if((gFunctionDefine & ZHONGHOUBANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhongHouBanTanShang;
								
					*/
					SetFocus(currentSpecialFunctionBtn);
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(specialFunctionBtnList[i] != currentSpecialFunctionBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSpecialFunctionBtn = specialFunctionBtnList[i];
									
				/*	if((gFunctionDefine & ZHONGHOUBANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhongHouBanTanShang;
					else if((gFunctionDefine & HANFENGTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionHanFengTanShang;
					else if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
					else if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhuJianTanShang;
				*/	
					SetFocus(currentSpecialFunctionBtn);
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndBScan = CreateWindow(szBScanChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndSpecialFunction,SW_HIDE);
					//SetFocus(hWndBScan);
					ShowWindow(hWndBScan,SW_SHOW);
					UpdateWindow(hWndBScan);

					gCurrentWnd = hWndBScan;
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
	return CallWindowProc(OldBtnSpecialFunctionBScanProc,Wnd,Msg,wParam,lParam);
}
/**************************
*�к��̽�˰�ť�Զ��崦�����
* @param		Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnSpecialFunctionZhongHouBanTanShangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndSpecialFunction);
					hWndSpecialFunction = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(specialFunctionBtnList[i] != currentSpecialFunctionBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSpecialFunctionBtn = specialFunctionBtnList[i];
				/*	
					if((gFunctionDefine & BSCAN_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionBScan;
					else if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhuJianTanShang;
					else if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
					else if((gFunctionDefine & HANFENGTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionHanFengTanShang;
				*/	
					SetFocus(currentSpecialFunctionBtn);
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(specialFunctionBtnList[i] != currentSpecialFunctionBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSpecialFunctionBtn = specialFunctionBtnList[i];
				/*	
					if((gFunctionDefine & HANFENGTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionHanFengTanShang;
					else if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
					else if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhuJianTanShang;
					else if((gFunctionDefine & BSCAN_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionBScan;
				*/	
					SetFocus(currentSpecialFunctionBtn);
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndZhongHouBanTanShang = CreateWindow(szZhongHouBanTanShangChild, TEXT(""), WS_CHILD|WS_VISIBLE,
								MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndSpecialFunction,SW_HIDE);
					//SetFocus(hWndZhongHouBanTanShang);
					ShowWindow(hWndZhongHouBanTanShang,SW_SHOW);
					UpdateWindow(hWndZhongHouBanTanShang);

					gCurrentWnd = hWndZhongHouBanTanShang;
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
	return CallWindowProc(OldBtnSpecialFunctionZhongHouBanTanShangProc,Wnd,Msg,wParam,lParam);
}
/**************************
*����̽�˰�ť�Զ��崦�����
* @param		Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnSpecialFunctionHanFengTanShangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndSpecialFunction);
					hWndSpecialFunction = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(specialFunctionBtnList[i] != currentSpecialFunctionBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSpecialFunctionBtn = specialFunctionBtnList[i];
					/*
					if((gFunctionDefine & ZHONGHOUBANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhongHouBanTanShang;
					else if((gFunctionDefine & BSCAN_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionBScan;
					else if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhuJianTanShang;
					else if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
					*/
					SetFocus(currentSpecialFunctionBtn);
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(specialFunctionBtnList[i] != currentSpecialFunctionBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSpecialFunctionBtn = specialFunctionBtnList[i];
					/*
					
					if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
					else if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhuJianTanShang;
					else if((gFunctionDefine & BSCAN_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionBScan;
					else if((gFunctionDefine & ZHONGHOUBANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhongHouBanTanShang;
					*/
					SetFocus(currentSpecialFunctionBtn);
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndHanFengTanShang= CreateWindow(szHanFengTanShangChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndSpecialFunction,SW_HIDE);
					//SetFocus(hWndHanFengTanShang);
					ShowWindow(hWndHanFengTanShang,SW_SHOW);
					UpdateWindow(hWndHanFengTanShang);

					gCurrentWnd = hWndHanFengTanShang;
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
	return CallWindowProc(OldBtnSpecialFunctionHanFengTanShangProc,Wnd,Msg,wParam,lParam);
}
/**************************
*���ϲ��ϼ�ⰴť�Զ��崦�����
* @param		Wnd[in]:
* @param		Msg[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnSpecialFunctionFuHeCaiLiaoJianCeProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndSpecialFunction);
					hWndSpecialFunction = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(specialFunctionBtnList[i] != currentSpecialFunctionBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSpecialFunctionBtn = specialFunctionBtnList[i];
					/*
					if((gFunctionDefine & HANFENGTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionHanFengTanShang;
					else if((gFunctionDefine & ZHONGHOUBANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhongHouBanTanShang;
					else if((gFunctionDefine & BSCAN_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionBScan;
					else if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhuJianTanShang;
					*/
					SetFocus(currentSpecialFunctionBtn);
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(specialFunctionBtnList[i] != currentSpecialFunctionBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSpecialFunctionBtn = specialFunctionBtnList[i];
					/*					
					if((gFunctionDefine & ZHUJIANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhuJianTanShang;
					else if((gFunctionDefine & BSCAN_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionBScan;
					else if((gFunctionDefine & ZHONGHOUBANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhongHouBanTanShang;
					else if((gFunctionDefine & HANFENGTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionHanFengTanShang;
					*/
					SetFocus(currentSpecialFunctionBtn);
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndFuHeCaiLiaoJianCe= CreateWindow(szFuHeCaiLiaoJianCeChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndSpecialFunction,SW_HIDE);
					//SetFocus(hWndFuHeCaiLiaoJianCe);
					ShowWindow(hWndFuHeCaiLiaoJianCe,SW_SHOW);
					UpdateWindow(hWndFuHeCaiLiaoJianCe);
					
					gCurrentWnd = hWndFuHeCaiLiaoJianCe;
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
	return CallWindowProc(OldBtnSpecialFunctionFuHeCaiLiaoJianCeProc,Wnd,Msg,wParam,lParam);
}
/**************************
*������ť�Զ��崦�����
* @specialFunction    hInstance[in]:
* @specialFunction		message[in]:
* @specialFunction		wParam[in]:
* @specialFunction		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnSpecialFunctionZhuJianTanShangProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndSpecialFunction);
					hWndSpecialFunction = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(specialFunctionBtnList[i] != currentSpecialFunctionBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSpecialFunctionBtn = specialFunctionBtnList[i];
				/*											
					if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
					else if((gFunctionDefine & HANFENGTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionHanFengTanShang;
					else if((gFunctionDefine & ZHONGHOUBANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhongHouBanTanShang;
					else if((gFunctionDefine & BSCAN_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionBScan;
					*/
					SetFocus(currentSpecialFunctionBtn);
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(specialFunctionBtnList[i] != currentSpecialFunctionBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSpecialFunctionBtn = specialFunctionBtnList[i];
					
					
				/*	if((gFunctionDefine & BSCAN_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionBScan;
					else if((gFunctionDefine & ZHONGHOUBANTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionZhongHouBanTanShang;
					else if((gFunctionDefine & HANFENGTANSHANG_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionHanFengTanShang;
					else if((gFunctionDefine & FUHECAILIAOJIANCE_SWITCH) != 0)
						currentSpecialFunctionBtn = hBtnSpecialFunctionFuHeCaiLiaoJianCe;
				*/	
					SetFocus(currentSpecialFunctionBtn);
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndZhuJianTanShang = CreateWindow(szZhuJianChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndSpecialFunction,SW_HIDE);
					//SetFocus(hWndZhuJianTanShang);
					ShowWindow(hWndZhuJianTanShang,SW_SHOW);
					UpdateWindow(hWndZhuJianTanShang);

					gCurrentWnd = hWndZhuJianTanShang;
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
	return CallWindowProc(OldBtnSpecialFunctionZhuJianTanShangProc,Wnd,Msg,wParam,lParam);
}
/**************************
*���Ʋ�߰�ť�Զ��崦�����
* @specialFunction    hInstance[in]:
* @specialFunction		message[in]:
* @specialFunction		wParam[in]:
* @specialFunction		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnSpecialFunctionLieWenCeGaoProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndFnMenu,SW_SHOW);
					SetFocus(currentFnMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndSpecialFunction);
					hWndSpecialFunction = 0;
					
					gCurrentFocusBtn = currentFnMenuBtn;

					gCurrentWnd = hWndFnMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(specialFunctionBtnList[i] != currentSpecialFunctionBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentSpecialFunctionBtn = specialFunctionBtnList[i];

					SetFocus(currentSpecialFunctionBtn);
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(specialFunctionBtnList[i] != currentSpecialFunctionBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentSpecialFunctionBtn = specialFunctionBtnList[i];

					SetFocus(currentSpecialFunctionBtn);
					gCurrentFocusBtn = currentSpecialFunctionBtn;
				}
				else if(wParam == VK_RETURN)
				{
					hWndLieWenCeGao = CreateWindow(szLieWenCeGaoChild, TEXT(""), WS_CHILD|WS_VISIBLE,
							 MENU_START_X, MENU_START_Y, MENU_WIDTH, MENU_HEIGHT, hWndMain, NULL, hInst, NULL);
					ShowWindow(hWndSpecialFunction,SW_HIDE);
					//SetFocus(hWndZhuJianTanShang);
					ShowWindow(hWndLieWenCeGao,SW_SHOW);
					UpdateWindow(hWndLieWenCeGao);

					gCurrentWnd = hWndLieWenCeGao;
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
	return CallWindowProc(OldBtnSpecialFunctionLieWenCeGaoProc,Wnd,Msg,wParam,lParam);
}

