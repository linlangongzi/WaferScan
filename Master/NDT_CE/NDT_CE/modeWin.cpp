/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ģʽ����Դ�ļ�
 * Filename: modeWin.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "modeWin.h"
#include "interfaceCmdID.h"

#include "resource.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"
#include "craftParamWin.h"
#include "recvParamWin.h"

HWND hWndMode = NULL;//ģʽ���ھ��
HWND hBtnMode1or2 = NULL;//��ͨ����ť���
HWND hBtnMode1plus2 = NULL;//˫ͨ����ť���
HWND hBtnMode1to2 = NULL;//һ��һ�հ�ť���
HWND hBtnModeDanjing = NULL;//����������ť���
HWND hBtnModeShuangjing = NULL;//˫��������ť���
HWND modeBtnList[5] ={NULL,NULL,NULL,NULL,NULL};//ģʽ���ڵİ�ť��
WNDPROC OldBtnMode1or2Proc = NULL;//��ͨ����ťĬ�ϴ������
WNDPROC OldBtnMode1plus2Proc = NULL;//˫ͨ����ťĬ�ϴ������
WNDPROC OldBtnMode1to2Proc = NULL;//һ��һ�հ�ťĬ�ϴ������
WNDPROC OldBtnModeDanjingProc = NULL;//����������ťĬ�ϴ������
WNDPROC OldBtnModeShuangjingProc = NULL;//˫��������ťĬ�ϴ������
HWND currentModeBtn = NULL;//ģʽ���е�ǰѡ�еİ�ť
//HWND hLableTest;
//static HBRUSH hStaticBrush = NULL;
static int buttonNum = 5;//��ť�ؼ���
static WORD wReducedButtonNum = 0;			//���ܿ��ؼ��ٰ�Ť����

extern HWND hWndTsMenu;//̽�˲˵����ھ��
extern HWND currentTsMenuBtn;//̽�˲˵������е�ǰѡ�еİ�ť
extern HINSTANCE hInst;//����ʵ�����
extern HWND gCurrentFocusBtn;
// songchenguang add start 2010-04-23
extern HWND hStatusBar;
extern ARS200FPGA_SYS_REG* sys_reg;
extern WORD CurrentChannelNo;
extern HWND hWndMain;					//�����ھ��
extern SYS_INFO gSysInfo;
extern CRITICAL_SECTION csSysInfo;
extern wchar_t gszTextChannel[10];
extern wchar_t gszTextPlus[10];
extern wchar_t gszTextRefPlus[10];

extern ARS200FPGA_CH_DATA_REG *chCurrent_data_reg;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern ARS200FPGA_CH_CON_REG *chCurrent_con_reg;
extern ARS200FPGA_CH_PLUS_REG* chCurrent_plus_reg;
extern ARS200FPGA_CH_DAC_REG* chCurrent_dac_reg;
extern ARS200FPGA_CH_PLUS_REG* ch1_plus_reg;

extern WORD *chCurrent_interval;
extern WORD *chCurrent_data;

extern WORD *chCurrent_gate1curve;
extern WORD *chCurrent_gate2curve;
extern WORD *chCurrent_gate3curve;
extern WORD *chCurrent_tgc;
//extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];

extern WORD *ch2_interval;
extern WORD *ch2_data;
extern ARS200FPGA_CH_DATA_REG *ch2_data_reg;
extern ARS200FPGA_CH_GATE_REG *ch2_gate_reg;
extern ARS200FPGA_CH_CON_REG *ch2_con_reg;
extern WORD *ch2_tgc;

extern ARS200FPGA_CH_PLUS_REG* ch2_plus_reg;
extern ARS200FPGA_CH_DAC_REG* ch2_dac_reg;

extern WORD *ch2_gate1curve;
extern WORD *ch2_gate2curve;
extern WORD *ch2_gate3curve;

extern WORD *ch1_interval;
extern WORD *ch1_data;
extern ARS200FPGA_CH_DATA_REG *ch1_data_reg;
extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
extern ARS200FPGA_CH_CON_REG *ch1_con_reg;
extern WORD *ch1_tgc;

extern ARS200FPGA_CH_PLUS_REG* ch1_plus_reg;
extern ARS200FPGA_CH_DAC_REG* ch1_dac_reg;

extern WORD *ch1_gate1curve;
extern WORD *ch1_gate2curve;
extern WORD *ch1_gate3curve;

BOOL gbGuanLian = FALSE;

extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern SEND_PARAM_DATA SendParam_ad[2];
extern GATE_INFO GateInfos[2][3];
// songchenguang add start 2010-04-23
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;	//��ǰ��ʾ���Ӵ���
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern DWORD gFunctionDefine;		//���ܶ���
extern void chChangeHeadType(WORD wHeadType);

extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];				//DGSͨ������
extern float gfOldDacRange[CHANNEL_NUM];				//�������DACʱ������
extern short gwOldDacSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������DACʱ��ÿ�����λ��
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];//��ѡ�е�DGS������
extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
extern BOOL gbAvgCurveFlag[CHANNEL_NUM];		//AVG���߿��ر�־
extern float gfOldAvgRange[CHANNEL_NUM];				//�������AVGʱ������
extern short gwOldAvgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������AVGʱ��ÿ�����λ��
extern CURVEPOINT_DATA gSelectedAvgPointData[CHANNEL_NUM][AVG_CURVE_NUM + 1][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
extern WORD gSelectedAvgPointDataNum[CHANNEL_NUM];//��ѡ�е�AVG������
extern float gfTcgFirstPointGain[CHANNEL_NUM];		//��ӵĵ��һ�����Ӧ��Dbֵ
extern float gfDacFirstPointGain[CHANNEL_NUM];		//��ӵĵ��һ�����Ӧ��Dbֵ
extern float gfAvgFirstPointGain[CHANNEL_NUM];		//��ӵĵ��һ�����Ӧ��Dbֵ
extern float gfDefectZhi1[CHANNEL_NUM];			//����׾���1
extern float gfDefectZhi2[CHANNEL_NUM];			//��2
extern float gfDefectZhi3[CHANNEL_NUM];			//��3
extern float gfPingDingGain[CHANNEL_NUM];			//��������
extern float gfDingLiangGain[CHANNEL_NUM];			//��������
extern float gfPanHuiGain[CHANNEL_NUM];				//�з�����

/**************************
*ע��ģʽ����
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerModeChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) modeChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szModeChild;

	return RegisterClass(&wc);
}
/**************************
*ģʽ���ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK modeChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			//	if(hStaticBrush == NULL)	
			//		hStaticBrush = CreateSolidBrush(BK_COLOR_WHITE);
			buttonNum = 5;
		//	gFunctionDefine = 	0x000002AA;
			wReducedButtonNum = 0;
			if((gFunctionDefine & WORKMODE1OR2_SWITCH) == 0)
				wReducedButtonNum ++;
			if((gFunctionDefine & WORKMODE1PLUS2_SWITCH) == 0)
				wReducedButtonNum ++;
			if((gFunctionDefine & WORKMODE1TO2_SWITCH) == 0)
				wReducedButtonNum ++;
			if((gFunctionDefine & WORKMODEDANJING_SWITCH) == 0)
				wReducedButtonNum ++;
			if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) == 0)
				wReducedButtonNum ++;

			buttonNum = buttonNum - wReducedButtonNum;
				
			//����ť�ؼ�
			hBtnMode1or2 = CreateWindow(_T("BUTTON"),_T("1or2"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_MODE_1OR2,hInst,NULL);
			hBtnMode1plus2 = CreateWindow(_T("BUTTON"),_T("1plus2"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_MODE_1PLUS2,hInst,NULL);
			hBtnMode1to2 = CreateWindow(_T("BUTTON"),_T("1to2"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_MODE_1TO2,hInst,NULL);
			hBtnModeDanjing = CreateWindow(_T("BUTTON"),_T("Danjing"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 3*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_MODE_DANJING,hInst,NULL);
			hBtnModeShuangjing = CreateWindow(_T("BUTTON"),_T("Shuangjing"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 4*BUTTON_5_EACH_HEIGHT,MENU_WIDTH,MENU_HEIGHT - 4*BUTTON_5_EACH_HEIGHT,hWnd,(HMENU)IDC_MODE_SHUANGJING,hInst,NULL);
		/*	modeBtnList[0] = hBtnMode1or2;
			modeBtnList[1] = hBtnMode1plus2;
			modeBtnList[2] = hBtnMode1to2;
			modeBtnList[3] = hBtnModeDanjing;
			modeBtnList[4] = hBtnModeShuangjing;
			*/
			if((gFunctionDefine & WORKMODE1OR2_SWITCH) != 0)
			{
				modeBtnList[0] = hBtnMode1or2;
				if((gFunctionDefine & WORKMODE1PLUS2_SWITCH) != 0)
				{
					modeBtnList[1] = hBtnMode1plus2;
					if((gFunctionDefine & WORKMODE1TO2_SWITCH) != 0)
					{
						modeBtnList[2] = hBtnMode1to2;
						if((gFunctionDefine & WORKMODEDANJING_SWITCH) != 0)
						{
							modeBtnList[3] = hBtnModeDanjing;
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[4] = hBtnModeShuangjing;
							}
						}
						else
						{
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[3] = hBtnModeShuangjing;
							}
						}
					}
					else
					{
						if((gFunctionDefine & WORKMODEDANJING_SWITCH) != 0)
						{
							modeBtnList[2] = hBtnModeDanjing;
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[3] = hBtnModeShuangjing;
							}
						}
						else
						{
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[2] = hBtnModeShuangjing;
							}
						}
					}
				}
				else
				{
					if((gFunctionDefine & WORKMODE1TO2_SWITCH) != 0)
					{
						modeBtnList[1] = hBtnMode1to2;
						if((gFunctionDefine & WORKMODEDANJING_SWITCH) != 0)
						{
							modeBtnList[2] = hBtnModeDanjing;
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[3] = hBtnModeShuangjing;
							}
						}
						else
						{
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[2] = hBtnModeShuangjing;
							}
						}
					}
					else
					{
						if((gFunctionDefine & WORKMODEDANJING_SWITCH) != 0)
						{
							modeBtnList[1] = hBtnModeDanjing;
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[2] = hBtnModeShuangjing;
							}
						}
						else
						{
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[1] = hBtnModeShuangjing;
							}
						}
					}
				}				
			}
			else
			{
				if((gFunctionDefine & WORKMODE1PLUS2_SWITCH) != 0)
				{
					modeBtnList[0] = hBtnMode1plus2;
					if((gFunctionDefine & WORKMODE1TO2_SWITCH) != 0)
					{
						modeBtnList[1] = hBtnMode1to2;
						if((gFunctionDefine & WORKMODEDANJING_SWITCH) != 0)
						{
							modeBtnList[2] = hBtnModeDanjing;
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[3] = hBtnModeShuangjing;
							}
						}
						else
						{
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[2] = hBtnModeShuangjing;
							}
						}
					}
					else
					{
						if((gFunctionDefine & WORKMODEDANJING_SWITCH) != 0)
						{
							modeBtnList[1] = hBtnModeDanjing;
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[2] = hBtnModeShuangjing;
							}
						}
						else
						{
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[1] = hBtnModeShuangjing;
							}
						}
					}
				}
				else
				{
					if((gFunctionDefine & WORKMODE1TO2_SWITCH) != 0)
					{
						modeBtnList[0] = hBtnMode1to2;
						if((gFunctionDefine & WORKMODEDANJING_SWITCH) != 0)
						{
							modeBtnList[1] = hBtnModeDanjing;
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[2] = hBtnModeShuangjing;
							}
						}
						else
						{
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[1] = hBtnModeShuangjing;
							}
						}
					}
					else
					{
						if((gFunctionDefine & WORKMODEDANJING_SWITCH) != 0)
						{
							modeBtnList[0] = hBtnModeDanjing;
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[1] = hBtnModeShuangjing;
							}
						}
						else
						{
							if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) != 0)
							{
								modeBtnList[0] = hBtnModeShuangjing;
							}
						}
					}
				}
			}
			//��ť�ؼ����Զ��崦�����
			OldBtnMode1or2Proc = (WNDPROC) GetWindowLong(hBtnMode1or2,GWL_WNDPROC);
			SetWindowLong(hBtnMode1or2,GWL_WNDPROC,(LONG)btnMode1or2Proc);
			OldBtnMode1plus2Proc = (WNDPROC) GetWindowLong(hBtnMode1plus2,GWL_WNDPROC);
			SetWindowLong(hBtnMode1plus2,GWL_WNDPROC,(LONG)btnMode1plus2Proc);
			OldBtnMode1to2Proc = (WNDPROC) GetWindowLong(hBtnMode1to2,GWL_WNDPROC);
		    SetWindowLong(hBtnMode1to2,GWL_WNDPROC,(LONG)btnMode1to2Proc);
			OldBtnModeDanjingProc = (WNDPROC) GetWindowLong(hBtnModeDanjing,GWL_WNDPROC);
			SetWindowLong(hBtnModeDanjing,GWL_WNDPROC,(LONG)btnModeDanjingProc);
			OldBtnModeShuangjingProc = (WNDPROC) GetWindowLong(hBtnModeShuangjing,GWL_WNDPROC);
			SetWindowLong(hBtnModeShuangjing,GWL_WNDPROC,(LONG)btnModeShuangjingProc);
			
			currentModeBtn = modeBtnList[0];//Ĭ�ϵ�ǰѡ�е�ģʽ��ť
			if(currentModeBtn)
			{
				SetFocus(currentModeBtn);//ѡ�а�ť������뽹��
				gCurrentFocusBtn = currentModeBtn;
			}
			else			//���ܶ��رհ�ť�ĸ����ڻ�����뽹��
				SetFocus(hWnd);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWndMode, &ps);
			
			EndPaint(hWndMode, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndTsMenu,SW_SHOW);
				SetFocus(currentTsMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

				DestroyWindow(hWndMode);
				hWndMode = 0;
				
				gCurrentFocusBtn = currentTsMenuBtn;

				gCurrentWnd = hWndTsMenu;				
			}
			break;

		case WM_COMMAND:
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			mode_DrawButtonBitmap(hInst,pdis);
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
*����ģʽ���ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int mode_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	BTN_BMP_INFO tbl_modeBtnBitmap[]=
	{
		{IDC_MODE_1OR2	,IDB_BITMAP_DANTONGDAO_NOSEL_R	 ,  IDB_BITMAP_DANTONGDAO_SELECT_R		},
		{IDC_MODE_1PLUS2	,IDB_BITMAP_SHUANGTONGDAO_NOSEL_R	 ,  IDB_BITMAP_SHUANGTONGDAO_SELECT_R		},
		{IDC_MODE_1TO2	,IDB_BITMAP_YIFAYISHOU_NOSEL_R ,  IDB_BITMAP_YIFAYISHOU_SELECT_R	},
		{IDC_MODE_DANJING	,IDB_BITMAP_DANJING_NOSEL_R	 ,  IDB_BITMAP_DANJING_SELECT_R		},
		{IDC_MODE_SHUANGJING	,IDB_BITMAP_SHUANGJING_NOSEL_R	 ,  IDB_BITMAP_SHUANGJING_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	if((gFunctionDefine & WORKMODE1OR2_SWITCH) == 0)
		tbl_modeBtnBitmap[0].nBtnNoFocID = IDB_BITMAP_DANTONGDAO_DISABLE_R;
	if((gFunctionDefine & WORKMODE1PLUS2_SWITCH) == 0)
		tbl_modeBtnBitmap[1].nBtnNoFocID = IDB_BITMAP_SHUANGTONGDAO_DISABLE_R;
	if((gFunctionDefine & WORKMODE1TO2_SWITCH) == 0)
		tbl_modeBtnBitmap[2].nBtnNoFocID = IDB_BITMAP_YIFAYISHOU_DISABLE_R;
	if((gFunctionDefine & WORKMODEDANJING_SWITCH) == 0)
		tbl_modeBtnBitmap[3].nBtnNoFocID = IDB_BITMAP_DANJINGGUANLIAN_DISABLE_R;
	if((gFunctionDefine & WORKMODESHUANGJING_SWITCH) == 0)
		tbl_modeBtnBitmap[4].nBtnNoFocID = IDB_BITMAP_SHUANGJINGGUANLIAN_DISABLE_R;
			
	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_modeBtnBitmap);
	return ReturnVal;
}
/**************************
*��ͨ����ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnMode1or2Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			
			break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTsMenu,SW_SHOW);
					SetFocus(currentTsMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndMode);
					hWndMode = 0;
					
					gCurrentFocusBtn = currentTsMenuBtn;

					gCurrentWnd = hWndTsMenu;
					
				}
			
				else if(wParam == VK_UP)
				{
					while(modeBtnList[i] != currentModeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModeBtn = modeBtnList[i];
					SetFocus(currentModeBtn);
					gCurrentFocusBtn = currentModeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(modeBtnList[i] != currentModeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModeBtn = modeBtnList[i];
					SetFocus(currentModeBtn);
					gCurrentFocusBtn = currentModeBtn;
				}
				else if(wParam == VK_RETURN)
				{
					ChangeWorkMode(SingleChannelA, TRUE);
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
		case WM_CTLCOLORSTATIC:
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnMode1or2Proc,Wnd,Msg,wParam,lParam);
}
/**************************
*˫ͨ����ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnMode1plus2Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTsMenu,SW_SHOW);
					SetFocus(currentTsMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndMode);
					hWndMode = 0;
					
					gCurrentFocusBtn = currentTsMenuBtn;

					gCurrentWnd = hWndTsMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(modeBtnList[i] != currentModeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModeBtn = modeBtnList[i];
					SetFocus(currentModeBtn);
					gCurrentFocusBtn = currentModeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(modeBtnList[i] != currentModeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModeBtn = modeBtnList[i];
					SetFocus(currentModeBtn);
					gCurrentFocusBtn = currentModeBtn;
				}
				else if(wParam == VK_RETURN)
				{
					ChangeWorkMode(DoubleChannels, TRUE);
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
	return CallWindowProc(OldBtnMode1plus2Proc,Wnd,Msg,wParam,lParam);
}

void Ch1Data2Ch2Data()
{
	// �������
	memcpy(&SendParam_ad[1], &SendParam_ad[0], sizeof(SEND_PARAM_DATA));
	// ���ղ���
//	memcpy(&ReceiveParam_ad[1], &ReceiveParam_ad[0], sizeof(RECEIVE_PARAM_DATA));//�迹������wangbingfu modify at 2010-11-19
	ReceiveParam_ad[1].fDelay = ReceiveParam_ad[0].fDelay;
	ReceiveParam_ad[1].fPlus = ReceiveParam_ad[0].fPlus;
	ReceiveParam_ad[1].fRefPlus = ReceiveParam_ad[0].fRefPlus;
	ReceiveParam_ad[1].nJianBoMode = ReceiveParam_ad[0].nJianBoMode;
	ReceiveParam_ad[1].nLvBo = ReceiveParam_ad[0].nLvBo;
	ReceiveParam_ad[1].nProbeFreq = ReceiveParam_ad[0].nProbeFreq;
	// ��ʾ����
	memcpy(&DisplaySet_ad[1], &DisplaySet_ad[0], sizeof(DISPLAY_SET_DLG_DATA));
	// DAC����
//	memcpy();
	// բ������
	memcpy(&GateInfos[1], &GateInfos[0], sizeof(GATE_INFO));
	//DAC
	for(int j = 0; j < DAC_CURVE_NUM - 1; j++)
	{
		gDgsSwitch[1][j] = gDgsSwitch[0][j];
	}
	
	gfPingDingGain[1] = gfPingDingGain[0];
	gfDingLiangGain[1] = gfDingLiangGain[0];
	gfPanHuiGain[1] = gfPanHuiGain[0];
	
	gfDacFirstPointGain[1] = gfDacFirstPointGain[0];
	gfOldDacRange[1] = gfOldDacRange[0];
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		 gSelectedDgsPointDataNum[1][j] = gSelectedDgsPointDataNum[0][j];
	}
	for(int j = 0; j < gSelectedDgsPointDataNum[0][1]; j++)
	{
		gwOldDacSerial[1][j] = gwOldDacSerial[0][j];
	}
	for(int j = 0; j < DAC_CURVE_NUM; j++)
	{
		memcpy(gSelectedDgsPointData[1][j], gSelectedDgsPointData[0][j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
	//AVG
	gbAvgCurveFlag[1] = gbAvgCurveFlag[0];
	gfDefectZhi1[1] = gfDefectZhi1[0];
	gfDefectZhi2[1] = gfDefectZhi2[0];
	gfDefectZhi3[1] = gfDefectZhi3[0];
	gfAvgFirstPointGain[1] = gfAvgFirstPointGain[0];
	gfOldAvgRange[1] = gfOldAvgRange[0];
	for(int j = 0; j < gSelectedAvgPointDataNum[0]; j++)
	{
		gwOldAvgSerial[1][j] = gwOldAvgSerial[0][j];
	}
	gSelectedAvgPointDataNum[1] = gSelectedAvgPointDataNum[0];
	for(int j = 0; j < AVG_CURVE_NUM + 1; j++)
	{
		memcpy(gSelectedAvgPointData[1][j], gSelectedAvgPointData[0][j], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	}
}

/**************************
*һ��һ�հ�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnMode1to2Proc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTsMenu,SW_SHOW);
					SetFocus(currentTsMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndMode);
					hWndMode = 0;
					
					gCurrentFocusBtn = currentTsMenuBtn;

					gCurrentWnd = hWndTsMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(modeBtnList[i] != currentModeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModeBtn = modeBtnList[i];
					SetFocus(currentModeBtn);
					gCurrentFocusBtn = currentModeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(modeBtnList[i] != currentModeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModeBtn = modeBtnList[i];
					SetFocus(currentModeBtn);
					gCurrentFocusBtn = currentModeBtn;
				}
				else if(wParam == VK_RETURN)
				{
					ChangeWorkMode(OneSendOneReceive, TRUE);
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
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnMode1to2Proc,Wnd,Msg,wParam,lParam);
}
/*
void Switch2Ch1()
{
	CurrentChannelNo = 0;
	chCurrent_data_reg = ch1_data_reg;
	chCurrent_gate_reg = ch1_gate_reg;
	chCurrent_con_reg = ch1_con_reg;
	chCurrent_interval = ch1_interval;
	chCurrent_data = ch1_data;

	chCurrent_plus_reg = ch1_plus_reg;
	chCurrent_dac_reg = ch1_dac_reg;

	chCurrent_gate1curve = ch1_gate1curve;
	chCurrent_gate2curve = ch1_gate2curve;
	chCurrent_gate3curve = ch1_gate3curve;

	memset(gszTextChannel, 0, sizeof(gszTextChannel));
	
	swprintf(gszTextChannel, _T("I"));

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

	memset(gszTextPlus, 0, sizeof(gszTextPlus));
	swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

	memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
	swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
}
*/

/**************************
*����������ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnModeDanjingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTsMenu,SW_SHOW);
					SetFocus(currentTsMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndMode);
					hWndMode = 0;
					
					gCurrentFocusBtn = currentTsMenuBtn;

					gCurrentWnd = hWndTsMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(modeBtnList[i] != currentModeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModeBtn = modeBtnList[i];
					SetFocus(currentModeBtn);
					gCurrentFocusBtn = currentModeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(modeBtnList[i] != currentModeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModeBtn = modeBtnList[i];
					SetFocus(currentModeBtn);
					gCurrentFocusBtn = currentModeBtn;
				}
				else if(wParam == VK_RETURN)
				{
					ChangeWorkMode(DanJingGuanLian, TRUE);
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
	return CallWindowProc(OldBtnModeDanjingProc,Wnd,Msg,wParam,lParam);
}
/**************************
*˫��������ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnModeShuangjingProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					ShowWindow(hWndTsMenu,SW_SHOW);
					SetFocus(currentTsMenuBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

					DestroyWindow(hWndMode);
					hWndMode = 0;
					
					gCurrentFocusBtn = currentTsMenuBtn;

					gCurrentWnd = hWndTsMenu;
				}
			
				else if(wParam == VK_UP)
				{
					while(modeBtnList[i] != currentModeBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentModeBtn = modeBtnList[i];
					SetFocus(currentModeBtn);
					gCurrentFocusBtn = currentModeBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(modeBtnList[i] != currentModeBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentModeBtn = modeBtnList[i];
					SetFocus(currentModeBtn);
					gCurrentFocusBtn = currentModeBtn;
				}
				else if(wParam == VK_RETURN)
				{
					ChangeWorkMode(ShuangJingGuanLian, TRUE);
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
	return CallWindowProc(OldBtnModeShuangjingProc,Wnd,Msg,wParam,lParam);
}

/**************************
*�л�����ģʽ
* @param    emWorkMode[in]:
* @return   void
* @since    1.00
***************************/
void ChangeWorkMode(WORKMODE emWorkMode, BOOL bSaveFlag)
{
	if(emWorkMode == SingleChannelA)
	{
		ChangeTo1or2();
	}
	else if(emWorkMode == DoubleChannels)
	{
		ChangeTo1plus2();
	}
	else if(emWorkMode == OneSendOneReceive)
	{
		ChangeTo1to2();
	}
	else if(emWorkMode == DanJingGuanLian)
	{
		ChangeToDanJing();
	}
	else if(emWorkMode == ShuangJingGuanLian)
	{
		ChangeToShuangJing();
	}
	else
	{
	}
	// �������õ�Ĭ���ļ�
	if(bSaveFlag)
	{
		SaveParamFile(lpDefaultParamFile);
	}
}

/**************************
*�л�Ϊ��ͨ��
* @return   void
* @since    1.00
***************************/
void ChangeTo1or2()
{
	if((gFunctionDefine & WORKMODE1PLUS2_SWITCH) == 0)
	{
		chChangeHeadType(0);
	}
	else
	{						
		InitializeCriticalSection(&csSysInfo);
		EnterCriticalSection(&csSysInfo);
		if(CurrentChannelNo == 0)
		{
			sys_reg->wWorkMode = SingleChannelA;
			gSysInfo.wWorkMode = SingleChannelA;

			SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)SingleChannelA);
			//wangbingfu add start at 2010-10-19
			swprintf(gszTextChannel, _T("1"));

			SendMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
			//wangbingfu add end at 2010-10-19
			//wangbingfu add start at 2010-11-19
			if(ReceiveParam_ad[0].wImpedance == 0)
			{
				gSysInfo.wRelay &= 0xFFFC;
				//yachang.huang rum on 2013-11-19
				//sys_reg->wRelay = gSysInfo.wRelay;							
			}
			else if(ReceiveParam_ad[0].wImpedance == 1)
			{
				gSysInfo.wRelay &= 0xFFFC;
				gSysInfo.wRelay |= 0x1;
				//yachang.huang rum on 2013-11-19
				//sys_reg->wRelay = gSysInfo.wRelay;
			}
			//wangbingfu add end at 2010-11-19
		}
		else if(CurrentChannelNo == 1)
		{
			sys_reg->wWorkMode = SingleChannelB;
			gSysInfo.wWorkMode = SingleChannelB;

			SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)SingleChannelB);
			//wangbingfu add start at 2010-10-19
			swprintf(gszTextChannel, _T("2"));

			SendMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);
			//wangbingfu add end at 2010-10-19
			//wangbingfu add start at 2010-11-19
			if(ReceiveParam_ad[1].wImpedance == 0)
			{
				gSysInfo.wRelay &= 0xFFFC;
				//yachang.huang rum on 2013-11-19
				//sys_reg->wRelay = gSysInfo.wRelay;							
			}
			else if(ReceiveParam_ad[1].wImpedance == 2)
			{
				gSysInfo.wRelay &= 0xFFFC;
				gSysInfo.wRelay |= 0x2;
				//yachang.huang rum on 2013-11-19
				//sys_reg->wRelay = gSysInfo.wRelay;
			}
			//wangbingfu add end at 2010-11-19
		}
		LeaveCriticalSection(&csSysInfo);
		DeleteCriticalSection(&csSysInfo);
		
		//�����е���ͨ��ʱ��Ҫ��ͨ��1��RefPlus�ӽ�ȥwangbingfu add start at 2010-11-25						
		WORD wTemp0 = 0;
		WORD wTemp1 = 0;
		WORD wTemp2 = 0;
		WORD wTemp3 = 0;

		float fTotalPlus = ReceiveParam_ad[0].fPlus+ ReceiveParam_ad[0].fRefPlus;
		dBProcess(fTotalPlus, wTemp0, wTemp1, wTemp2, wTemp3);

		ch1_plus_reg->wdBRef = wTemp0;
		PlusChangeTurn(0);	

		ch1_plus_reg->wdBLevel1 = wTemp1;
		PlusChangeTurn(0);

		ch1_plus_reg->wdBLevel2 = wTemp2;
		PlusChangeTurn(0);

		ch1_plus_reg->wdBLevel3 = wTemp3;
		PlusChangeTurn(0);
		//�����е���ͨ��ʱ��Ҫ��ͨ��1��RefPlus�ӽ�ȥwangbingfu add end at 2010-11-25
		
		SetRedrawFlag(TRUE);

		gbGuanLian = FALSE; 

		// �������õ�Ĭ���ļ�
	//	SaveParamFile(lpDefaultParamFile);
	}
}
/**************************
*�л�Ϊ˫ͨ��
* @return   void
* @since    1.00
***************************/
void ChangeTo1plus2()
{
	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)DoubleChannels);	

	
	if(CurrentChannelNo == 0)
	{
		swprintf(gszTextChannel, _T("1"));
	}
	else if(CurrentChannelNo == 1)
	{
		swprintf(gszTextChannel, _T("2"));
	}

	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

	sys_reg->wWorkMode = DoubleChannels;

	InitializeCriticalSection(&csSysInfo);
	EnterCriticalSection(&csSysInfo);

	gSysInfo.wWorkMode = DoubleChannels;
	gSysInfo.wRelay &= 0xFFFC;
	//yachang.huang rum on 2013-11-19
	//sys_reg->wRelay = gSysInfo.wRelay;
	LeaveCriticalSection(&csSysInfo);
	DeleteCriticalSection(&csSysInfo);

	SetRedrawFlag(TRUE);
	gbGuanLian = FALSE;

	// �������õ�Ĭ���ļ�
//	SaveParamFile(lpDefaultParamFile);
}

/**************************
*�л�Ϊһ��һ��
* @return   void
* @since    1.00
***************************/
void ChangeTo1to2()
{
	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)OneSendOneReceive);

	sys_reg->wWorkMode = OneSendOneReceive;

	InitializeCriticalSection(&csSysInfo);
	EnterCriticalSection(&csSysInfo);

	gSysInfo.wCurrentChNo = 1;				//wangbingfu add at 2010-10-21
	gSysInfo.wWorkMode = OneSendOneReceive;
	gSysInfo.wRelay &= 0xFFFD;
	gSysInfo.wRelay |= 0x1;
	//yachang.huang rum on 2013-11-19
	//sys_reg->wRelay = gSysInfo.wRelay;
	LeaveCriticalSection(&csSysInfo);
	DeleteCriticalSection(&csSysInfo);

	////////////					
	CurrentChannelNo = 1;
	chCurrent_data_reg = ch2_data_reg;
	chCurrent_gate_reg = ch2_gate_reg;
	chCurrent_con_reg = ch2_con_reg;
	chCurrent_interval = ch2_interval;
	chCurrent_data = ch2_data;

	chCurrent_plus_reg = ch2_plus_reg;
	chCurrent_dac_reg = ch2_dac_reg;

	chCurrent_gate1curve = ch2_gate1curve;
	chCurrent_gate2curve = ch2_gate2curve;
	chCurrent_gate3curve = ch2_gate3curve;

	chCurrent_tgc = ch2_tgc;

	memset(gszTextChannel, 0, sizeof(gszTextChannel));
	
//			swprintf(gszTextChannel, _T("%d"), CurrentChannelNo+1);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

	memset(gszTextPlus, 0, sizeof(gszTextPlus));
	swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

	memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
	swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
	//////////////

	SetRedrawFlag(TRUE);
	
	gbGuanLian = TRUE;

	Ch1Data2Ch2Data();

	InitParamDetail();

	// �������õ�Ĭ���ļ�
//	SaveParamFile(lpDefaultParamFile);
}

/**************************
*�л�Ϊ����
* @return   void
* @since    1.00
***************************/
void ChangeToDanJing()
{
	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)DanJingGuanLian);
	
	sys_reg->wWorkMode = DanJingGuanLian;

	InitializeCriticalSection(&csSysInfo);
	EnterCriticalSection(&csSysInfo);

	gSysInfo.wRelay &= 0xFFFE;
	gSysInfo.wRelay |= 0x2;
	//yachang.huang rum on 2013-11-19
	//sys_reg->wRelay = gSysInfo.wRelay;

	gSysInfo.wWorkMode = DanJingGuanLian;
	LeaveCriticalSection(&csSysInfo);
	DeleteCriticalSection(&csSysInfo);
	
	SetRedrawFlag(TRUE);
	gbGuanLian = TRUE;

//					Switch2Ch1();
	////////////					
	CurrentChannelNo = 1;
	chCurrent_data_reg = ch2_data_reg;
	chCurrent_gate_reg = ch2_gate_reg;
	chCurrent_con_reg = ch2_con_reg;
	chCurrent_interval = ch2_interval;
	chCurrent_data = ch2_data;

	chCurrent_plus_reg = ch2_plus_reg;
	chCurrent_dac_reg = ch2_dac_reg;

	chCurrent_gate1curve = ch2_gate1curve;
	chCurrent_gate2curve = ch2_gate2curve;
	chCurrent_gate3curve = ch2_gate3curve;

	memset(gszTextChannel, 0, sizeof(gszTextChannel));

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

	memset(gszTextPlus, 0, sizeof(gszTextPlus));
	swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

	memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
	swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
	////

	Ch1Data2Ch2Data();
	InitParamDetail();

	// �������õ�Ĭ���ļ�
//	SaveParamFile(lpDefaultParamFile);
}

/**************************
*�л�Ϊ˫��
* @return   void
* @since    1.00
***************************/
void ChangeToShuangJing()
{
	SendMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (0 | (SBT_OWNERDRAW| SBT_NOBORDERS)), (LPARAM)ShuangJingGuanLian);					

	sys_reg->wWorkMode = ShuangJingGuanLian;

	InitializeCriticalSection(&csSysInfo);
	EnterCriticalSection(&csSysInfo);

	gSysInfo.wRelay |= 0x3;
	//yachang.huang rum on 2013-11-19
	//sys_reg->wRelay = gSysInfo.wRelay;
	gSysInfo.wWorkMode = ShuangJingGuanLian;

	LeaveCriticalSection(&csSysInfo);
	DeleteCriticalSection(&csSysInfo);

	SetRedrawFlag(TRUE);
	gbGuanLian = TRUE;

//					Switch2Ch1();
	////////////					
	CurrentChannelNo = 1;
	chCurrent_data_reg = ch2_data_reg;
	chCurrent_gate_reg = ch2_gate_reg;
	chCurrent_con_reg = ch2_con_reg;
	chCurrent_interval = ch2_interval;
	chCurrent_data = ch2_data;

	chCurrent_plus_reg = ch2_plus_reg;
	chCurrent_dac_reg = ch2_dac_reg;

	chCurrent_gate1curve = ch2_gate1curve;
	chCurrent_gate2curve = ch2_gate2curve;
	chCurrent_gate3curve = ch2_gate3curve;

	memset(gszTextChannel, 0, sizeof(gszTextChannel));

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (1 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextChannel);

	memset(gszTextPlus, 0, sizeof(gszTextPlus));
	swprintf(gszTextPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fPlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (3 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPlus);

	memset(gszTextRefPlus, 0, sizeof(gszTextRefPlus));
	swprintf(gszTextRefPlus, _T("%.1f"), ReceiveParam_ad[CurrentChannelNo].fRefPlus);

	PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (4| (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextRefPlus);
	////
	// ��Ch1���ݸ��Ƶ�Ch2
	Ch1Data2Ch2Data();
	InitParamDetail();

	// �������õ�Ĭ���ļ�
//	SaveParamFile(lpDefaultParamFile);
}

