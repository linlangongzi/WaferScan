/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * ���Ʋ�ߴ���Դ�ļ�
 * Filename: lieWenCeGaoWin.cpp
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
#include "recvParamWin.h"

HWND hWndLieWenCeGao = NULL;						//���Ʋ�ߴ��ھ��
HWND hBtnLieWenCeGaoUpDownPoint = NULL;			//���¶˵㰴ť���
HWND hBtnLieWenCeGaoCaculateAgain = NULL;			//���¼��㰴ť���
HWND hBtnLieWenCeGaoHeight = NULL;					//���Ƹ߶Ȱ�ť���

HWND lieWenCeGaoBtnList[2] = {NULL,NULL};					//���Ʋ�ߴ��ڵİ�ť��
WNDPROC OldBtnLieWenCeGaoUpDownPointProc = NULL;	//���¶˵㰴ťĬ�ϴ������
WNDPROC OldBtnLieWenCeGaoCaculateAgainProc = NULL;		//���¼��㰴ťĬ�ϴ������
HWND currentLieWenCeGaoBtn  = NULL;					//���Ʋ�ߴ��е�ǰѡ�еİ�ť

static WORD buttonNum = 2;				//��ť�ؼ���
static float fLieWenGaoDu = -1;			//���Ƹ߶�
static WORD wAddPointType = 0;			//�Ӷ˵�����0���϶˵� 1���¶˵� 2:���ܼӶ˵�
static float fUpPointRang = 0;			//�϶˵�����
static float fDownPointRang = 0;		//�¶˵�����
static float fOldPlus = 0;				//�ı�����֮ǰ������
static BOOL bUpPointFlag = FALSE;		//�����϶˵��־
static BOOL bDownPointFlag = FALSE;		//�����¶˵��־

CURVEPOINT_DATA gstUpPointData = {0, 0};		//�϶˵�
CURVEPOINT_DATA gstDownPointData = {0, 0};		//�¶˵�
WORD gwUpPointFreezeWave[DATA_SIZE];			//�϶˵��ס�Ļز�
WORD gwDownPointFreezeWave[DATA_SIZE];			//�¶˵��ס�Ļز�

ATOM registerLieWenCeGaoChild(HINSTANCE hInstance);
static LRESULT CALLBACK lieWenCeGaoChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
static int lieWenCeGao_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis);
static LRESULT CALLBACK btnLieWenCeGaoUpDownPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);
static LRESULT CALLBACK btnLieWenCeGaoCaculateAgainProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam);

extern HINSTANCE hInst;					//����ʵ�����
extern HWND hWndMain;
extern HWND gCurrentFocusBtn;	
extern BOOL bFreezeUp;
extern HWND gCurrentWnd;				//��ǰ��ʾ���Ӵ���	
extern HWND hWndSpecialFunction;		//ר�ù��ܴ��ھ��
extern HWND currentSpecialFunctionBtn;	//ר�ù��ܴ��е�ǰѡ�еİ�ť
extern wchar_t gszTextStep[10];
extern HWND hStatusBar;					// ״̬�����
extern HWND hWndCurve;//���ߴ��ھ��
extern HWND currentCurveBtn;//���ߴ����е�ǰѡ�еİ�ť

extern HANDLE hDrawAutoGainThread;				// �Զ�����̾��
extern DWORD dwDrawAutoGainThreadID;			// �Զ������߳�ID
extern THREAD_PARAM stAutoGainThread_param;		// �Զ������̲߳���
extern DWORD WINAPI autoGainProc(LPVOID lpParameter);
extern CRITICAL_SECTION csPeakDefectInfos;
extern PEAK_INFO gPeakDefectInfos[2][2];
extern WORD CurrentChannelNo;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern PIC_DATA gPicData[2];					// ͼ������
extern SYS_COLOR_INFO gstSysColor[SYS_COLOR_NUM];
extern int gCurrentSysColorIndex;
extern float gfMinRange[2];	// ��С����
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern BOOL gbPeakRememberFlag;  	//��ֵ�����Ƿ���
extern CRITICAL_SECTION csPeakRemember;
// songchenguang add end 2011-06-22
extern WORD MAX_PLUS;
// songchenguang add end 2011-06-22
/**************************
*��ʾ���Ƹ߶�
* @param		hWnd[in]:���ھ��
* @param		fHeight[in]:���Ƹ߶�
* @return   LRESULT :
* @since    1.00
***************************/
static void displayLieWenGaoDu(HWND hWnd ,float fHeight)
{
	wchar_t strText[20];
	memset(strText, 0, sizeof(strText));
	if(fHeight == -1)
	{
		DrawLableText(hWnd, _T("N/A"), DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
	else
	{
		wsprintf(strText, _T("%.1f"), fHeight);
		DrawLableText(hWnd, strText, DEFAULT_LABLE_TEXT_HEIGHT, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
	}
}
void CALLBACK lieWenCeGaoDisplayProc( HWND hwnd, 
							  UINT uMsg, 
							  UINT idEvent, 
							  DWORD dwTime 
							)
{
/*	POINT preData[DATA_SIZE];
	HDC hdc = GetDC(hWndMain);

	RECT rect;
	GetClipBox(hdc, &rect);
	
	RECT RctWave;// ��������
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	// ���������ȡ��߶�
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;
	float yb = nWaveHeight/MAX_HEIGHT;
	float xb;
	long x,y;
	WORD wNum = 0;

	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);

	if(DisplaySet_ad[CurrentChannelNo].fRange < gfMinRange[CurrentChannelNo])
	{
		wNum = WORD((DisplaySet_ad[CurrentChannelNo].fRange/gfMinRange[CurrentChannelNo])*DATA_SIZE);
		xb = nWaveWidth/float(wNum);
	}
	else		
	{
		xb = nWaveWidth/512.0f;
		wNum = DATA_SIZE;
	}

	for (int i = 0;i < wNum; i++)
	{
			if (gwUpPointFreezeWave[i] > MAX_HEIGHT) 
				gwUpPointFreezeWave[i] = MAX_HEIGHT;
							
		x = long(RctWave.left+i*xb);
		y = long((nWaveHeight-1)-gwUpPointFreezeWave[i]*yb) + RctWave.top;

		if(i>0)
		{
			if(CurrentChannelNo == 0)
				Line(preData[i-1].x, preData[i-1].y, x, y, RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrCh1Wave));
			else
				Line(preData[i-1].x, preData[i-1].y, x, y, RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrCh2Wave));
		}

		preData[i].x = x;
		preData[i].y = y;
	}
	
	
	ReleaseDC(hWndMain,hdc);
*/
	if(bUpPointFlag)
		drawShiZhi1(dataToPoint(gstUpPointData), BLUE_POINT, TRUE);
	if(bDownPointFlag)
		drawShiZhi1(dataToPoint(gstDownPointData), RED_POINT, TRUE);
}

/**************************
*ע�����Ʋ�ߴ���
* @LieWenCeGao    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerLieWenCeGaoChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));
	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) lieWenCeGaoChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szLieWenCeGaoChild;

	return RegisterClass(&wc);
}
/**************************
*���Ʋ�ߴ��ڴ������
* @LieWenCeGao    hInstance[in]:
* @LieWenCeGao		message[in]:
* @LieWenCeGao		wParam[in]:
* @LieWenCeGao		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK lieWenCeGaoChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			buttonNum = 2;
			
			hBtnLieWenCeGaoUpDownPoint = CreateWindow(_T("BUTTON"),_T("upDownPointLieWenCeGao"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_LIEWENCEGAO_UPDOWNPOINT,hInst,NULL);
			hBtnLieWenCeGaoCaculateAgain = CreateWindow(_T("BUTTON"),_T("caculateAgainLieWenCeGao"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_LIEWENCEGAO_CACULATEAGAIN,hInst,NULL);
			hBtnLieWenCeGaoHeight = CreateWindow(_T("BUTTON"),_T("caculateAgainLieWenCeGao"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_LIEWENCEGAO_HEIGHT,hInst,NULL);
			
			lieWenCeGaoBtnList[0] = hBtnLieWenCeGaoUpDownPoint;
			lieWenCeGaoBtnList[1] = hBtnLieWenCeGaoCaculateAgain;
			
			//��ť�ؼ����Զ��崦�����
			OldBtnLieWenCeGaoUpDownPointProc = (WNDPROC) GetWindowLong(hBtnLieWenCeGaoUpDownPoint,GWL_WNDPROC);
			SetWindowLong(hBtnLieWenCeGaoUpDownPoint,GWL_WNDPROC,(LONG)btnLieWenCeGaoUpDownPointProc);
			
			OldBtnLieWenCeGaoCaculateAgainProc = (WNDPROC) GetWindowLong(hBtnLieWenCeGaoCaculateAgain,GWL_WNDPROC);
			SetWindowLong(hBtnLieWenCeGaoCaculateAgain,GWL_WNDPROC,(LONG)btnLieWenCeGaoCaculateAgainProc);
		
		
			currentLieWenCeGaoBtn = lieWenCeGaoBtnList[0];//Ĭ�ϵ�ǰѡ�����¶˵㰴ť			
			SetFocus(currentLieWenCeGaoBtn);//ѡ�а�ť������뽹��
			gCurrentFocusBtn = currentLieWenCeGaoBtn;
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);
			break;
		case WM_KEYDOWN:
			if(wParam == VK_ESCAPE)
			{
				ShowWindow(hWndSpecialFunction,SW_SHOW);
				SetFocus(currentSpecialFunctionBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��

				DestroyWindow(hWndLieWenCeGao);
				hWndLieWenCeGao = 0;
				
				gCurrentFocusBtn = currentSpecialFunctionBtn;

				gCurrentWnd = hWndSpecialFunction;

				fLieWenGaoDu = -1;
				wAddPointType = 0;
				bUpPointFlag = FALSE;
				bDownPointFlag = FALSE;
				SetRedrawFlag(TRUE);//ˢ�£������ʱ����������
			}
			break;
		case WM_COMMAND:
				
			break;
		case WM_DRAWITEM:
			pdis=(LPDRAWITEMSTRUCT)lParam;
			lieWenCeGao_DrawButtonBitmap(hInst,pdis);

			if(pdis->hwndItem == hBtnLieWenCeGaoHeight)
			{
				displayLieWenGaoDu(pdis->hwndItem, fLieWenGaoDu);
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
*�������Ʋ�ߴ��ڰ�ťͼƬ
* @LieWenCeGao    hInstance[in]:
* @LieWenCeGao		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
static int lieWenCeGao_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;

	if(wAddPointType == 0)
	{
		BTN_BMP_INFO tbl_LieWenCeGaoBtnBitmap[]=
		{
			{IDC_LIEWENCEGAO_UPDOWNPOINT	,IDB_BITMAP_SHANGDUANDIAN_NOSEL_R	 ,  IDB_BITMAP_SHANGDUANDIAN_SELECT_R		},
			{IDC_LIEWENCEGAO_CACULATEAGAIN	,IDB_BITMAP_CHONGXINJISUAN_NOSEL_R	 ,  IDB_BITMAP_CHONGXINJISUAN_SELECT_R		},
			{IDC_LIEWENCEGAO_HEIGHT	,		 IDB_BITMAP_LIEWENGAODU_NOSEL_R	 ,  IDB_BITMAP_LIEWENGAODU_NOSEL_R		},
			{	-1			,	-1					 ,	-1							}    
		};
		
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_LieWenCeGaoBtnBitmap);
	}
	else if(wAddPointType == 1)
	{
		BTN_BMP_INFO tbl_LieWenCeGaoBtnBitmap[]=
		{
			{IDC_LIEWENCEGAO_UPDOWNPOINT	,IDB_BITMAP_XIADUANDIAN_NOSEL_R	 ,  IDB_BITMAP_XIADUANDIAN_SELECT_R		},
			{IDC_LIEWENCEGAO_CACULATEAGAIN	,IDB_BITMAP_CHONGXINJISUAN_NOSEL_R	 ,  IDB_BITMAP_CHONGXINJISUAN_SELECT_R		},
			{IDC_LIEWENCEGAO_HEIGHT	,		 IDB_BITMAP_LIEWENGAODU_NOSEL_R	 ,  IDB_BITMAP_LIEWENGAODU_NOSEL_R		},
			{	-1			,	-1					 ,	-1							}    
		};
		
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_LieWenCeGaoBtnBitmap);
	}
	else if(wAddPointType == 2)
	{
		BTN_BMP_INFO tbl_LieWenCeGaoBtnBitmap[]=
		{
			{IDC_LIEWENCEGAO_UPDOWNPOINT	,IDB_BITMAP_SHANGDUANDIAN_DISABLE_R	 ,  IDB_BITMAP_SHANGDUANDIAN_DISABLE_R		},
			{IDC_LIEWENCEGAO_CACULATEAGAIN	,IDB_BITMAP_CHONGXINJISUAN_NOSEL_R	 ,  IDB_BITMAP_CHONGXINJISUAN_SELECT_R		},
			{IDC_LIEWENCEGAO_HEIGHT	,		 IDB_BITMAP_LIEWENGAODU_NOSEL_R	 ,  IDB_BITMAP_LIEWENGAODU_NOSEL_R		},
			{	-1			,	-1					 ,	-1							}    
		};
		
		ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_LieWenCeGaoBtnBitmap);
	}
		
	return ReturnVal;
}
/**************************
*���¶˵㰴ť�Զ��崦�����
* @LieWenCeGao    hInstance[in]:
* @LieWenCeGao		message[in]:
* @LieWenCeGao		wParam[in]:
* @LieWenCeGao		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnLieWenCeGaoUpDownPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(lieWenCeGaoBtnList[i] != currentLieWenCeGaoBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentLieWenCeGaoBtn = lieWenCeGaoBtnList[i];
					SetFocus(currentLieWenCeGaoBtn);
					gCurrentFocusBtn = currentLieWenCeGaoBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(lieWenCeGaoBtnList[i] != currentLieWenCeGaoBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentLieWenCeGaoBtn = lieWenCeGaoBtnList[i];
					SetFocus(currentLieWenCeGaoBtn);
					gCurrentFocusBtn = currentLieWenCeGaoBtn;
				}
				else if(wParam == VK_RETURN)
				{
					SetFocus(NULL);
					fOldPlus = ReceiveParam_ad[CurrentChannelNo].fPlus;

					// �رշ�ֵ����
					InitializeCriticalSection(&csPeakRemember);
					EnterCriticalSection(&csPeakRemember);

					gbPeakRememberFlag = FALSE;

					LeaveCriticalSection(&csPeakRemember);
					DeleteCriticalSection(&csPeakRemember);
					
					if(wAddPointType == 0)
					{
						stAutoGainThread_param.hWnd = Wnd;
						stAutoGainThread_param.fValue = 80;					
					}
					else if(wAddPointType == 1)
					{
						stAutoGainThread_param.hWnd = Wnd;
						stAutoGainThread_param.fValue = 60;						
					}
					else
					{
						;//
					}
					// �����Զ������߳�
					if(hDrawAutoGainThread == NULL)
					{
						// �����߳�		
						hDrawAutoGainThread = ::CreateThread(NULL, 0, autoGainProc, (LPVOID)&stAutoGainThread_param, 0, &dwDrawAutoGainThreadID);
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
				else if(wParam == '9')
				{
					PEAK_INFO peakDefectInfos[2][2];
					InitializeCriticalSection(&csPeakDefectInfos);
					EnterCriticalSection(&csPeakDefectInfos);
					for(int i = 0 ;i < CHANNEL_NUM; i++)
					{
						for(int j = 0; j < 2; j++)
						{
							peakDefectInfos[i][j] = gPeakDefectInfos[i][j];
						}
					}
					LeaveCriticalSection(&csPeakDefectInfos);
					DeleteCriticalSection(&csPeakDefectInfos);
					
					if(currentLieWenCeGaoBtn == hBtnLieWenCeGaoUpDownPoint)
					{
						if(wAddPointType == 0)
						{
							wAddPointType = 1;
							SetFocus(currentLieWenCeGaoBtn);
							
							gstUpPointData.serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)peakDefectInfos[CurrentChannelNo][0].wDefectTime)
																												/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
							gstUpPointData.data = (float)peakDefectInfos[CurrentChannelNo][0].wDefectAmp;

							fUpPointRang = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)peakDefectInfos[CurrentChannelNo][0].wDefectTime);

							for(int i = 0; i < DATA_SIZE; i++)
							{
								gwUpPointFreezeWave[i] = gPicData[CurrentChannelNo].data[i];
							}//��ס�϶˵�ز�

							SetTimer(Wnd, TIMER_LIEWENCEGAO, 20, lieWenCeGaoDisplayProc);

							if(ReceiveParam_ad[CurrentChannelNo].fPlus + 20 < (MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fRefPlus))
								ReceiveParam_ad[CurrentChannelNo].fPlus += 20;
							else
								ReceiveParam_ad[CurrentChannelNo].fPlus = MAX_PLUS - ReceiveParam_ad[CurrentChannelNo].fRefPlus;

							SetPlusLableAndExecute(NULL, ReceiveParam_ad[CurrentChannelNo].fPlus, ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);

							bUpPointFlag = TRUE;
								
							gainChangeCurve(fOldPlus, TRUE);							
						}
						else if(wAddPointType == 1)
						{							
							wAddPointType = 2;
							SetFocus(hBtnLieWenCeGaoUpDownPoint);//����ͼƬΪdisable
							lieWenCeGaoBtnList[0] = hBtnLieWenCeGaoCaculateAgain;
							buttonNum = 1;
							currentLieWenCeGaoBtn = lieWenCeGaoBtnList[0];//Ĭ�ϵ�ǰѡ�����¶˵㰴ť			
							SetFocus(currentLieWenCeGaoBtn);//ѡ�а�ť������뽹��
							gCurrentFocusBtn = currentLieWenCeGaoBtn;

							gstDownPointData.serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)peakDefectInfos[CurrentChannelNo][0].wDefectTime)
																												/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
							gstDownPointData.data = (float)peakDefectInfos[CurrentChannelNo][0].wDefectAmp;

							fDownPointRang = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)peakDefectInfos[CurrentChannelNo][0].wDefectTime);

							for(int i = 0; i < DATA_SIZE; i++)
							{
								gwDownPointFreezeWave[i] = gPicData[CurrentChannelNo].data[i];
							}//��ס�¶˵�ز�

							bDownPointFlag = TRUE;
							
							gainChangeCurve(fOldPlus, TRUE);

							float fAngle = (90 - DisplaySet_ad[CurrentChannelNo].fZheSheJiao)/180.0f * (float)PI;

							fLieWenGaoDu = (float)fabs((fDownPointRang * sin((double)fAngle) - fUpPointRang * sin((double)fAngle)));
							displayLieWenGaoDu(hBtnLieWenCeGaoHeight, fLieWenGaoDu);
						}
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
	return CallWindowProc(OldBtnLieWenCeGaoUpDownPointProc,Wnd,Msg,wParam,lParam);
}
/**************************
*���¼��㰴ť�Զ��崦�����
* @LieWenCeGao    hInstance[in]:
* @LieWenCeGao		message[in]:
* @LieWenCeGao		wParam[in]:
* @LieWenCeGao		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
static LRESULT CALLBACK btnLieWenCeGaoCaculateAgainProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
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
					while(lieWenCeGaoBtnList[i] != currentLieWenCeGaoBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentLieWenCeGaoBtn = lieWenCeGaoBtnList[i];
					SetFocus(currentLieWenCeGaoBtn);
					gCurrentFocusBtn = currentLieWenCeGaoBtn;
				}
				else if(wParam == VK_DOWN)
				{
					while(lieWenCeGaoBtnList[i] != currentLieWenCeGaoBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentLieWenCeGaoBtn = lieWenCeGaoBtnList[i];
					SetFocus(currentLieWenCeGaoBtn);
					gCurrentFocusBtn = currentLieWenCeGaoBtn;
				}
				else if(wParam == VK_RETURN)
				{
					fLieWenGaoDu = -1;
				//	displayLieWenGaoDu(pdis->hwndItem, fLieWenGaoDu);
					
					wAddPointType = 0;
					lieWenCeGaoBtnList[0] = hBtnLieWenCeGaoUpDownPoint;
					lieWenCeGaoBtnList[1] = hBtnLieWenCeGaoCaculateAgain;
					buttonNum = 2;
					currentLieWenCeGaoBtn = lieWenCeGaoBtnList[0];//Ĭ�ϵ�ǰѡ�����¶˵㰴ť			
					SetFocus(currentLieWenCeGaoBtn);//ѡ�а�ť������뽹��
					gCurrentFocusBtn = currentLieWenCeGaoBtn;

					bUpPointFlag = FALSE;
					bDownPointFlag = FALSE;
					SetRedrawFlag(TRUE);//ˢ�£������ʱ����������
					displayLieWenGaoDu(hBtnLieWenCeGaoHeight, fLieWenGaoDu);
				}
				else if(wParam == 'F')
				{
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
	return CallWindowProc(OldBtnLieWenCeGaoCaculateAgainProc,Wnd,Msg,wParam,lParam);
}

