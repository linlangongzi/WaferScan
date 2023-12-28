/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * �Զ����洰��Դ�ļ�
 * Filename: autoGainPlusWin.cpp
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
#include "gateWin.h"
#include "NDT_CE.h"
#include "recvParamWin.h"
#include "craftParamWin.h"

HWND hWndAutoGainPlus = NULL;//�Զ����洰�ھ��
HWND hBtnAutoGainPlusStart = NULL;//��ʼ��ť���
HWND hBtnAutoGainPlusWaveHeight = NULL;//���߰�ť���

HWND autoGainPlusBtnList[2] = {NULL, NULL};//�Զ����洰�ڵİ�ť��
WNDPROC OldBtnAutoGainPlusStartProc = NULL;//��ʼ��ťĬ�ϴ������
WNDPROC OldBtnAutoGainPlusWaveHeightProc = NULL;//���߰�ťĬ�ϴ������
HWND currentAutoGainPlusBtn = NULL;//�Զ����洰�е�ǰѡ�еİ�ť

ATOM registerAutoGainPlusChild(HINSTANCE hInstance);
LRESULT CALLBACK autoGainPlusChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int autoGainPlus_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
LRESULT CALLBACK btnAutoGainPlusStartProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK btnAutoGainPlusWaveHeightProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam );

static const int buttonNum = 2;//��ť�ؼ���

static float fPeakSheDing = 80.0f;
static float PeakSheDingStep = 1;
static float fOldPlus = 0;				//�ı�����֮ǰ������

HANDLE hDrawAutoGainThread = NULL;				// �Զ�����̾��
DWORD dwDrawAutoGainThreadID = 0;				// �Զ������߳�ID
THREAD_PARAM stAutoGainThread_param;		// �Զ������̲߳���

extern HWND hWndQuantitativeAnalysis;			//�����������ھ��
extern HWND currentQuantitativeAnalysisBtn;		//�����������е�ǰѡ�еİ�ť
extern HINSTANCE hInst;//����ʵ�����
extern HWND hWndMain;//������
extern HWND gCurrentFocusBtn;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern WORD CurrentChannelNo;
extern HWND hStatusBar;
extern wchar_t gszTextPeak[20];
extern GATE_INFO GateInfos[2][3];
extern BOOL gbPeakRememberFlag;  	//��ֵ�����Ƿ���
extern CRITICAL_SECTION csPeakRemember;
extern CRITICAL_SECTION csPeakInfo;
extern PEAK_INFO gPeakInfosRemeber[2][2];
extern BOOL gbHotAutoGainFlag;	//�Ƿ���Զ������ݷ�ʽ 
extern HWND gCurrentWnd;
extern wchar_t lpDefaultParamFile[MAX_PATH];

extern BOOL gWaveDrawDacFlag;		//�����Ƿ�DAC
extern BOOL gWaveDrawAvgFlag;		//�����Ƿ�AVG
extern HWND hWndTlAdjust;	//��У���ھ��
// songchenguang add end 2011-06-22
extern WORD MAX_PLUS;
// songchenguang add end 2011-06-22

/**************************
*ע���Զ����洰��
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerAutoGainPlusChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) autoGainPlusChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szAutoGainPlusChild;

	return RegisterClass(&wc);
}

void SetPeakSheDing(HWND hWnd, const float& fPeak)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));

	wsprintf(strText, _T("%.1f"), fPeak);
	DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
}

/**************************
*�Զ����洰�ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK autoGainPlusChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LPDRAWITEMSTRUCT pdis;

	switch (message) 
	{
		case WM_CREATE:
			//����ť�ؼ�
			hBtnAutoGainPlusStart = CreateWindow(_T("BUTTON"),_T("Start"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_AUTOGAINPLUS_START,hInst,NULL);
			hBtnAutoGainPlusWaveHeight = CreateWindow(_T("BUTTON"),_T("WaveHeight"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_AUTOGAINPLUS_WAVEHEIGHT,hInst,NULL);
			
			autoGainPlusBtnList[0] = hBtnAutoGainPlusStart;
			autoGainPlusBtnList[1] = hBtnAutoGainPlusWaveHeight;
			//��ť�ؼ����Զ��崦�����
			OldBtnAutoGainPlusStartProc = (WNDPROC) GetWindowLong(hBtnAutoGainPlusStart,GWL_WNDPROC);
			SetWindowLong(hBtnAutoGainPlusStart,GWL_WNDPROC,(LONG)btnAutoGainPlusStartProc);
			OldBtnAutoGainPlusWaveHeightProc = (WNDPROC) GetWindowLong(hBtnAutoGainPlusWaveHeight,GWL_WNDPROC);
			SetWindowLong(hBtnAutoGainPlusWaveHeight,GWL_WNDPROC,(LONG)btnAutoGainPlusWaveHeightProc);

			currentAutoGainPlusBtn = autoGainPlusBtnList[0];//Ĭ�ϵ�ǰѡ�п�ʼ��ť
			SetFocus(currentAutoGainPlusBtn);//ѡ�а�ť������뽹��

			if(!gbHotAutoGainFlag)
				gCurrentFocusBtn = currentAutoGainPlusBtn;

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
			autoGainPlus_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnAutoGainPlusWaveHeight)
			{
				SetPeakSheDing(pdis->hwndItem, fPeakSheDing);
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
*�����Զ����洰�ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int autoGainPlus_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	const BTN_BMP_INFO tbl_autoGainPlusBtnBitmap[]=
	{
		{IDC_AUTOGAINPLUS_START	,IDB_BITMAP_DBKAISHI_NOSEL_R	 ,  IDB_BITMAP_DBKAISHI_SELECT_R		},
		{IDC_AUTOGAINPLUS_WAVEHEIGHT	,IDB_BITMAP_BOGAO_NOSEL_R	 ,  IDB_BITMAP_BOGAO_SELECT_R		},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	int ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_autoGainPlusBtnBitmap);
	return ReturnVal;
}

DWORD WINAPI autoGainProc(LPVOID lpParameter)
{
	THREAD_PARAM* pParam = (THREAD_PARAM*)lpParameter;
	InitializeCriticalSection(&csPeakInfo);

	float fPeakValue = 0;
	do
	{
		// ��鵱ǰ����ֵ
		EnterCriticalSection(&csPeakInfo);
		fPeakValue = float(gPeakInfosRemeber[CurrentChannelNo][0].wDefectAmp) / MAX_HEIGHT*100.0f;
		LeaveCriticalSection(&csPeakInfo);

		float fStep = 0.3;
		double fCha = fabs(fPeakValue - pParam->fValue);
		if(fCha > 5.0f)
			fStep = 0.3f;
		else
			fStep = 0.05f;

		// �������趨ֵ������׼����
		if(fPeakValue > pParam->fValue)
		{
			if((ReceiveParam_ad[CurrentChannelNo].fPlus - fStep) > MIN_PLUS)
				ReceiveParam_ad[CurrentChannelNo].fPlus -= fStep;
			else
				ReceiveParam_ad[CurrentChannelNo].fPlus = MIN_PLUS;
		}
		// ��������׼����
		else
		{
			if(ReceiveParam_ad[CurrentChannelNo].fPlus  + ReceiveParam_ad[CurrentChannelNo].fRefPlus + fStep > MAX_PLUS)
				ReceiveParam_ad[CurrentChannelNo].fPlus = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fRefPlus;
			else
				ReceiveParam_ad[CurrentChannelNo].fPlus += fStep;
		}
		float fCompare = (float)fabs(ReceiveParam_ad[CurrentChannelNo].fPlus  + ReceiveParam_ad[CurrentChannelNo].fRefPlus - MAX_PLUS);
		if((fabs(ReceiveParam_ad[CurrentChannelNo].fPlus - MIN_PLUS) < 1E-3 
			|| fCompare < 1E-3) && fabs(fPeakValue - pParam->fValue) > 1E-3)
		{
			DeleteCriticalSection(&csPeakInfo);

			memset(gszTextPeak, 0, sizeof(gszTextPeak));
			swprintf(gszTextPeak, _T("�Զ�����ʧ��"));

			SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
			if((hWndTlAdjust != NULL) && (!gbHotAutoGainFlag))//��ִ����·ר�õ��������趨�����Զ�����ʱ
				PostMessage(pParam->hWnd, WM_KEYDOWN, (WPARAM)'8', (LPARAM)0);
			else
				PostMessage(pParam->hWnd, WM_KEYDOWN, (WPARAM)'9', (LPARAM)0);
			
			return 0;
		}

		// ���·�ֵ
		SetPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, FALSE, CurrentChannelNo);
		
		Sleep(50);
	}while(fabs(fPeakValue - pParam->fValue) > 1);// ����ֵ���趨ֵ������1E-1

	DeleteCriticalSection(&csPeakInfo);

	SaveParamFile(lpDefaultParamFile);

	memset(gszTextPeak, 0, sizeof(gszTextPeak));
	swprintf(gszTextPeak, _T("�Զ��������"));

	SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	
	PostMessage(pParam->hWnd, WM_KEYDOWN, (WPARAM)'9', (LPARAM)0);

	return 0;
}
/**************************
*��ʼ��ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAutoGainPlusStartProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					if(!gbHotAutoGainFlag)
					{
						ShowWindow(hWndQuantitativeAnalysis,SW_SHOW);
						SetFocus(currentQuantitativeAnalysisBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

						DestroyWindow(hWndAutoGainPlus);
						hWndAutoGainPlus = 0;

						gCurrentFocusBtn = currentQuantitativeAnalysisBtn;

						gCurrentWnd = hWndQuantitativeAnalysis;
					}
					else
					{
						if(gCurrentWnd)//���Ӳ˵�����
						{
							ShowWindow(gCurrentWnd, SW_SHOW);
							SetFocus(gCurrentFocusBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
						}
						else //û���Ӳ˵�����
						{
							SetMenuStateFlag(FALSE);
							SetFocus(hWndMain);//�����ڻ�����뽹��
						}

						DestroyWindow(hWndAutoGainPlus);
						hWndAutoGainPlus = 0;						
						
						gbHotAutoGainFlag = FALSE;
					}
				}
			
				else if(wParam == VK_UP)
				{
					while(autoGainPlusBtnList[i] != currentAutoGainPlusBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAutoGainPlusBtn = autoGainPlusBtnList[i];
					SetFocus(currentAutoGainPlusBtn);
					if(!gbHotAutoGainFlag)
						gCurrentFocusBtn = currentAutoGainPlusBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(autoGainPlusBtnList[i] != currentAutoGainPlusBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAutoGainPlusBtn = autoGainPlusBtnList[i];
					SetFocus(currentAutoGainPlusBtn);
					if(!gbHotAutoGainFlag)
						gCurrentFocusBtn = currentAutoGainPlusBtn;
				}
				else if(wParam == VK_RETURN)
				{
					SetFocus(NULL);
					//wangbingfu add start at 2010-12-27
					fOldPlus = ReceiveParam_ad[CurrentChannelNo].fPlus;
					//wangbingfu add start end 2010-12-27

					memset(gszTextPeak, 0, sizeof(gszTextPeak));
					swprintf(gszTextPeak, _T("���������..."));

					SendMessage(hStatusBar, SB_SETTEXT, 
								(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
				
					// �رշ�ֵ����
					InitializeCriticalSection(&csPeakRemember);
					EnterCriticalSection(&csPeakRemember);

					gbPeakRememberFlag = FALSE;

					LeaveCriticalSection(&csPeakRemember);
					DeleteCriticalSection(&csPeakRemember);

					// �򿪵�ǰͨ����һ��բ��
					GateInfos[CurrentChannelNo][0].bOpen = TRUE;
					SetOpenFlag(NULL, GateInfos[CurrentChannelNo][0].bOpen, 1);
					stAutoGainThread_param.hWnd = Wnd;
					stAutoGainThread_param.fValue = fPeakSheDing;
					// �����Զ������߳�
					if(hDrawAutoGainThread == NULL)
					{
						// �����߳�		
						hDrawAutoGainThread = ::CreateThread(NULL, 0, autoGainProc, (LPVOID)&stAutoGainThread_param, 0, &dwDrawAutoGainThreadID);
					}
				}
				else if(wParam == VK_LEFT)
				{
				}
				else if(wParam == VK_RIGHT)
				{
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					if(!gbHotAutoGainFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(gbHotAutoGainFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(!gbHotAutoGainFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '9')
				{
					if(currentAutoGainPlusBtn == hBtnAutoGainPlusStart)
					{
						SetFocus(currentAutoGainPlusBtn);
						SetPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);//wangbingfu add  at 2010-2-25������ɺ�Ҫ����,��ʧ��ҲҪˢ�»�׼�����������ֵ
						if(!gWaveDrawDacFlag && !gWaveDrawAvgFlag)	//������������
							gainChangeCurve(fOldPlus, TRUE); //wangbingfu add start at 2010-12-27
					}

					CloseHandle(hDrawAutoGainThread);
					hDrawAutoGainThread = NULL;
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
	return CallWindowProc(OldBtnAutoGainPlusStartProc,Wnd,Msg,wParam,lParam);
}



/**************************
*���߰�ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnAutoGainPlusWaveHeightProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					if(!gbHotAutoGainFlag)
					{
						ShowWindow(hWndQuantitativeAnalysis,SW_SHOW);
						SetFocus(currentQuantitativeAnalysisBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

						DestroyWindow(hWndAutoGainPlus);
						hWndAutoGainPlus = 0;

						gCurrentFocusBtn = currentQuantitativeAnalysisBtn;

						gCurrentWnd = hWndQuantitativeAnalysis;
					}
					else
					{
						if(gCurrentWnd)	//���Ӳ˵�����
						{
							ShowWindow(gCurrentWnd, SW_SHOW);
							SetFocus(gCurrentFocusBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
						}
						else	//û���Ӳ˵�����
						{
							SetMenuStateFlag(FALSE);
							SetFocus(hWndMain);//�����ڻ�����뽹��
						}

						DestroyWindow(hWndAutoGainPlus);
						hWndAutoGainPlus = 0;					
						
						gbHotAutoGainFlag = FALSE;
					}
				}			
				else if(wParam == VK_UP)
				{
					while(autoGainPlusBtnList[i] != currentAutoGainPlusBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentAutoGainPlusBtn = autoGainPlusBtnList[i];
					SetFocus(currentAutoGainPlusBtn);
					if(!gbHotAutoGainFlag)
						gCurrentFocusBtn = currentAutoGainPlusBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(autoGainPlusBtnList[i] != currentAutoGainPlusBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentAutoGainPlusBtn = autoGainPlusBtnList[i];
					SetFocus(currentAutoGainPlusBtn);
					if(!gbHotAutoGainFlag)
						gCurrentFocusBtn = currentAutoGainPlusBtn;
				}
				else if(wParam == VK_RETURN)
				{
				}
				else if(wParam == VK_LEFT)
				{
					if(fPeakSheDing > 0 && fPeakSheDing > PeakSheDingStep)
						fPeakSheDing -= PeakSheDingStep;
					else 
						fPeakSheDing = 0;		

					SetPeakSheDing(Wnd, fPeakSheDing);
				}
				else if(wParam == VK_RIGHT)
				{
					if((fPeakSheDing + PeakSheDingStep) < 100)
						fPeakSheDing += PeakSheDingStep;
					else 
						fPeakSheDing = 100;

					SetPeakSheDing(Wnd, fPeakSheDing);
				}
				else if(wParam == '\t')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '1')
				{
					if(!gbHotAutoGainFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '2')
				{
					if(gbHotAutoGainFlag)
						SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '3')
				{
					SendMessage(hWndMain, WM_CHAR, wParam, lParam);
				}
				else if(wParam == '4')
				{
					if(!gbHotAutoGainFlag)
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
				{}
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnAutoGainPlusWaveHeightProc,Wnd,Msg,wParam,lParam);
}

DWORD WINAPI autoRefGainProc(LPVOID lpParameter)
{
	THREAD_PARAM* pParam = (THREAD_PARAM*)lpParameter;
	InitializeCriticalSection(&csPeakInfo);

	float fPeakValue = 0;
	do
	{
		// ��鵱ǰ����ֵ
		EnterCriticalSection(&csPeakInfo);
		fPeakValue = float(gPeakInfosRemeber[CurrentChannelNo][0].wDefectAmp) / MAX_HEIGHT*100.0f;
		LeaveCriticalSection(&csPeakInfo);

		float fStep = 0.3;
		double fCha = fabs(fPeakValue - pParam->fValue);
		if(fCha > 5.0f)
			fStep = 0.3f;
		else
			fStep = 0.05f;

		// �������趨ֵ������׼����
		if(fPeakValue > pParam->fValue)
		{
			if((ReceiveParam_ad[CurrentChannelNo].fRefPlus - fStep) > MIN_PLUS)
				ReceiveParam_ad[CurrentChannelNo].fRefPlus -= fStep;
			else
				ReceiveParam_ad[CurrentChannelNo].fRefPlus = MIN_PLUS;
		}
		// ��������׼����
		else
		{
			ReceiveParam_ad[CurrentChannelNo].fRefPlus += fStep;
		}
		
		if(fabs(ReceiveParam_ad[CurrentChannelNo].fRefPlus) < 1E-3 
			|| ReceiveParam_ad[CurrentChannelNo].fPlus  + ReceiveParam_ad[CurrentChannelNo].fRefPlus > MAX_PLUS)
		{
			DeleteCriticalSection(&csPeakInfo);

			memset(gszTextPeak, 0, sizeof(gszTextPeak));
			swprintf(gszTextPeak, _T("ʧ��"));

			SendMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);

			PostMessage(pParam->hWnd, WM_KEYDOWN, (WPARAM)'8', (LPARAM)0);

			return 0;
		}

		// ���·�ֵ
		if((ReceiveParam_ad[CurrentChannelNo].fPlus  + ReceiveParam_ad[CurrentChannelNo].fRefPlus) > MAX_PLUS)
		{
			ReceiveParam_ad[CurrentChannelNo].fRefPlus = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fPlus;
		}
		SetRefPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);
		
		Sleep(50);
	}while(fabs(fPeakValue - pParam->fValue) > 1);// ����ֵ���趨ֵ������1E-1

	DeleteCriticalSection(&csPeakInfo);

	SaveParamFile(lpDefaultParamFile);

//	memset(gszTextPeak, 0, sizeof(gszTextPeak));
//	swprintf(gszTextPeak, _T("�Զ��������"));

	SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	
	PostMessage(pParam->hWnd, WM_KEYDOWN, (WPARAM)'9', (LPARAM)0);

	return 0;
}
