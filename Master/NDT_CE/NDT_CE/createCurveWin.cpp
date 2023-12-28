/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * �½�TCG���ߴ���Դ�ļ�
 * Filename: createCurveWin.cpp
 *
 * Spec: Trace Tag write here. If has not, OnOff this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"
#include "createCurveWin.h"
#include "interfaceCmdID.h"
#include "const.h"
#include "resource.h"
#include "craftParamWin.h"
#include "gateWin.h"
#include "recvParamWin.h"

HWND hWndCreateCurve = NULL;//�½�DAC���ߴ��ھ��
HWND hBtnCreateCurveAddPoint = NULL;//�����㰴ť���
HWND hBtnCreateCurveDeletePoint = NULL;//ɾ���㰴ť���
HWND hBtnCreateCurveEnd = NULL;//������ť���
HWND createCurveBtnList[3] = {NULL,NULL,NULL};//�½����ߴ��ڵİ�ť��
WNDPROC OldBtnCreateCurveAddPointProc = NULL;//�����㰴ťĬ�ϴ������
WNDPROC OldBtnCreateCurveDeletePointProc = NULL;//ɾ���㰴ťĬ�ϴ������
WNDPROC OldBtnCreateCurveEndProc = NULL;//������ťĬ�ϴ������
HWND currentCreateCurveBtn = NULL;//�½����ߴ��е�ǰѡ�еİ�ť

CURVEPOINT_DATA gSelectedDacPointData[CHANNEL_NUM][MAX_CURVE_POINT_NUM];//��ӵ��������ݶ���
CURVEPOINT_DATA gCurrentSelectedDacPointData;//��ӵ������е�ǰѡ�е�DAC���ߵ�����
DWORD gSelectedDacPointDataNum[CHANNEL_NUM] = {0,0};//��ѡ�е�DAC������
float gfTcgFirstPointGain[CHANNEL_NUM] = {0,0};		//��ӵĵ��һ�����Ӧ��Dbֵ

float gfOldTcgRange[CHANNEL_NUM] = {0, 0};				//�������TCGʱ������
short gwOldTcgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//�������TCGʱ��ÿ�����λ��

BOOL gWaveDrawTcgFlag = FALSE;//�����Ƿ�TCG
static const int buttonNum = 3;//��ť�ؼ���

extern HWND hWndMakeCurve;//�������ߴ���
extern HWND currentMakeCurveBtn;//�������ߴ����е�ǰѡ�еİ�ť
extern HINSTANCE hInst;//����ʵ�����
extern HWND hWndMain;//�����ھ��
extern WORD CurrentChannelNo;
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern float fRangeEnd[2];
extern float fRangeStart[2];
extern HWND gCurrentFocusBtn;
extern ARS200FPGA_CH_DAC_REG *chCurrent_dac_reg;
extern HWND gCurrentWnd;		//��ǰ��ʾ���Ӵ���
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD gwDeleteStep;
extern BOOL gbDeletePointFlag;		//�Ƿ���ɾ�������
extern PEAK_INFO gPeakDefectInfos[2][2];	//����բ�ŷ�ֵ����Ϣ
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern HWND hBtnMakeCurveCreate;//������ť���
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];

/**************************
*ע���½����ߴ���
* @param    hInstance[in]:����ʵ�����
* @return   ATOM :
* @since    1.00
***************************/
ATOM registerCreateCurveChild(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HBRUSH hBrush;
	hBrush=CreateSolidBrush(RGB(222, 222, 221));

	wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (WNDPROC) createCurveChildProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = 0;
    wc.hCursor = 0;
    wc.hbrBackground = hBrush;//(HBRUSH) GetStockObject(GRAY_BRUSH);
    wc.lpszMenuName = 0;
    wc.lpszClassName = szCreateCurveChild;

	return RegisterClass(&wc);
}
/**************************
*�½����ߴ��ڴ������
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK createCurveChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			{	
				gwDeleteStep = 0;
				gbDeletePointFlag = FALSE;
				
				if(currentMakeCurveBtn == hBtnMakeCurveCreate)
				{
					gSelectedDacPointDataNum[CurrentChannelNo] = 0;
					memset(gSelectedDacPointData[CurrentChannelNo], 0,sizeof(CURVEPOINT_DATA) * MAX_CURVE_POINT_NUM);
					memset(&gCurrentSelectedDacPointData, 0, sizeof(CURVEPOINT_DATA));
				}

				gWaveDrawTcgFlag = TRUE;

				//����ť�ؼ�
				hBtnCreateCurveAddPoint = CreateWindow(_T("BUTTON"),_T("tcgAddPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATECURVE_ADDPOINT,hInst,NULL);
				hBtnCreateCurveDeletePoint = CreateWindow(_T("BUTTON"),_T("tcgDeletePoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATECURVE_DELETEPOINT,hInst,NULL);
				hBtnCreateCurveEnd = CreateWindow(_T("BUTTON"),_T("tcgEnd"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATECURVE_END,hInst,NULL);
			
				createCurveBtnList[0] = hBtnCreateCurveAddPoint;
				createCurveBtnList[1] = hBtnCreateCurveDeletePoint;
				createCurveBtnList[2] = hBtnCreateCurveEnd;
				//��ť�ؼ����Զ��崦�����
				OldBtnCreateCurveAddPointProc = (WNDPROC) GetWindowLong(hBtnCreateCurveAddPoint,GWL_WNDPROC);
				SetWindowLong(hBtnCreateCurveAddPoint,GWL_WNDPROC,(LONG)btnCreateCurveAddPointProc);
				OldBtnCreateCurveDeletePointProc = (WNDPROC) GetWindowLong(hBtnCreateCurveDeletePoint,GWL_WNDPROC);
				SetWindowLong(hBtnCreateCurveDeletePoint,GWL_WNDPROC,(LONG)btnCreateCurveDeletePointProc);
				OldBtnCreateCurveEndProc = (WNDPROC) GetWindowLong(hBtnCreateCurveEnd,GWL_WNDPROC);
				SetWindowLong(hBtnCreateCurveEnd,GWL_WNDPROC,(LONG)btnCreateCurveEndProc);
			
				currentCreateCurveBtn = createCurveBtnList[0];  //Ĭ�ϵ�ǰѡ�м����������ť
				SetFocus(currentCreateCurveBtn);//ѡ�а�ť������뽹��
				gCurrentFocusBtn = currentCreateCurveBtn;

			}
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
			createCurve_DrawButtonBitmap(hInst,pdis);

			break;
		case WM_CLOSE:		
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
/**************************
*�����½����ߴ��ڰ�ťͼƬ
* @param    hInstance[in]:
* @param		pdis[in]                                                 
* @return   int
* @since    1.00
***************************/
int createCurve_DrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis)
{
	int ReturnVal;
	const BTN_BMP_INFO tbl_createCurveBtnBitmap[]=
	{
		{IDC_CREATECURVE_ADDPOINT	,IDB_BITMAP_JIAYANGDIAN_NOSEL_R	 ,  IDB_BITMAP_JIAYANGDIAN_SELECT_R		},
		{IDC_CREATECURVE_DELETEPOINT	,IDB_BITMAP_SHANYANGDIAN_NOSEL_R	 ,  IDB_BITMAP_SHANYANGDIAN_SELECT_R		},
		{IDC_CREATECURVE_END	,IDB_BITMAP_JIESHU_NOSEL_R ,  IDB_BITMAP_JIESHU_SELECT_R	},
		{	-1			,	-1					 ,	-1							}    
	} ; 

	ReturnVal=WinDrawButtonBitmap(hInstance,pdis,tbl_createCurveBtnBitmap);
	return ReturnVal;
}
/**************************
*�����㰴ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCreateCurveAddPointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_CREATE:
			
			break;
		case WM_PAINT:
			 break;
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					DestroyWindow(hWndCreateCurve);
					hWndCreateCurve = 0;
					ShowWindow(hWndMakeCurve,SW_SHOW);
					SetFocus(currentMakeCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					gCurrentFocusBtn = currentMakeCurveBtn;

					gCurrentWnd = hWndMakeCurve;

					gWaveDrawTcgFlag = FALSE;
				}
			
				else if(wParam == VK_UP)
				{
					while(createCurveBtnList[i] != currentCreateCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateCurveBtn = createCurveBtnList[i];
					SetFocus(currentCreateCurveBtn);
					gCurrentFocusBtn = currentCreateCurveBtn;
				}
				else if(wParam == VK_DOWN)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					while(createCurveBtnList[i] != currentCreateCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateCurveBtn = createCurveBtnList[i];
					SetFocus(currentCreateCurveBtn);
					gCurrentFocusBtn = currentCreateCurveBtn;

					gbDeletePointFlag = TRUE;

					if(gSelectedDacPointDataNum[CurrentChannelNo])
					{
						gCurrentSelectedDacPointData = gSelectedDacPointData[CurrentChannelNo][gwDeleteStep];
						
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDacPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDacPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(hBtnCreateCurveDeletePoint, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
				}
				else if(wParam == VK_LEFT)
				{
				}
				else if(wParam == VK_RIGHT)
				{
				}
				else if(wParam == VK_RETURN)
				{
					gWaveDrawTcgFlag = FALSE;
					Sleep(100);//�ȴ���������

					if(gSelectedDacPointDataNum[CurrentChannelNo] < MAX_CURVE_POINT_NUM)
							gSelectedDacPointDataNum[CurrentChannelNo]++;

					if(gSelectedDacPointDataNum[CurrentChannelNo] == 1)
					{
						gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)gPeakDefectInfos[CurrentChannelNo][0].wDefectTime)
																										/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
						gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].serial = curveChangeByDelay(gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].serial, 
																								fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
						
						gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].data = (float)gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp;

						gfTcgFirstPointGain[CurrentChannelNo] = ReceiveParam_ad[CurrentChannelNo].fPlus;
					}
					else
					{
						gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].serial = (WORD)(Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed,(int)gPeakDefectInfos[CurrentChannelNo][0].wDefectTime)
																										/ DisplaySet_ad[CurrentChannelNo].fRange * DATA_SIZE);
						gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].serial = curveChangeByDelay(gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].serial, 
																								fRangeStart[CurrentChannelNo], fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
						
						gSelectedDacPointData[CurrentChannelNo][gSelectedDacPointDataNum[CurrentChannelNo] - 1].data = (float)(gPeakDefectInfos[CurrentChannelNo][0].wDefectAmp 
																														* pow(10, (double)(gfTcgFirstPointGain[CurrentChannelNo] - ReceiveParam_ad[CurrentChannelNo].fPlus) / 20));
					}
					
					sortSelectedPointDataBySerial(gSelectedDacPointData[CurrentChannelNo],gSelectedDacPointDataNum[CurrentChannelNo]);

					gWaveDrawTcgFlag = TRUE;

					gfOldTcgRange[CurrentChannelNo] = DisplaySet_ad[CurrentChannelNo].fRange;
							
					for(WORD i = 0; i < gSelectedDacPointDataNum[CurrentChannelNo]; i++)
					{	
						gwOldTcgSerial[CurrentChannelNo][i] = (short)gSelectedDacPointData[CurrentChannelNo][i].serial;
					}
				}
			//	else if(wParam == '\t')
			//	{
			//		if(gAddingFlag == FALSE)
			//			SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			//	}
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
				/*else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
			}
			break;
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnCreateCurveAddPointProc,Wnd,Msg,wParam,lParam);
}
/**************************
*ɾ���㰴ť�Զ��崦�����
* @param    hInstance[in]:
* @param		message[in]:
* @param		wParam[in]:
* @param		lParam[in]:
* @return   LRESULT :
* @since    1.00
***************************/
LRESULT CALLBACK btnCreateCurveDeletePointProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					DestroyWindow(hWndCreateCurve);
					hWndCreateCurve = 0;
					ShowWindow(hWndMakeCurve,SW_SHOW);
					SetFocus(currentMakeCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					gCurrentFocusBtn = currentMakeCurveBtn;

					gCurrentWnd = hWndMakeCurve;

					gWaveDrawTcgFlag = FALSE;
				}
			
				else if(wParam == VK_UP)
				{
					while(createCurveBtnList[i] != currentCreateCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateCurveBtn = createCurveBtnList[i];
					SetFocus(currentCreateCurveBtn);
					gCurrentFocusBtn = currentCreateCurveBtn;

					gbDeletePointFlag = FALSE;
				}
				else if(wParam == VK_DOWN)
				{
					while(createCurveBtnList[i] != currentCreateCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateCurveBtn = createCurveBtnList[i];
					SetFocus(currentCreateCurveBtn);
					gCurrentFocusBtn = currentCreateCurveBtn;

					gbDeletePointFlag = FALSE;
				}
				else if(wParam == VK_LEFT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					if(gSelectedDacPointDataNum[CurrentChannelNo])
					{
						
						if(gwDeleteStep == 0)
							gwDeleteStep = (WORD)gSelectedDacPointDataNum[CurrentChannelNo] - 1;
						else
							gwDeleteStep--;
						gCurrentSelectedDacPointData = gSelectedDacPointData[CurrentChannelNo][gwDeleteStep];
						
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDacPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDacPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
				}
				else if(wParam == VK_RIGHT)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					if(gSelectedDacPointDataNum[CurrentChannelNo])
					{						
						if(gwDeleteStep == gSelectedDacPointDataNum[CurrentChannelNo] - 1)
							gwDeleteStep = 0;
						else
							gwDeleteStep++;
						gCurrentSelectedDacPointData = gSelectedDacPointData[CurrentChannelNo][gwDeleteStep];
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDacPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDacPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
				}
				else if(wParam == VK_RETURN)
				{
					if(gSelectedDacPointDataNum[CurrentChannelNo])
					{
						gWaveDrawTcgFlag = FALSE;
						Sleep(100);//�ȴ���������
					 	if(gSelectedDacPointDataNum[CurrentChannelNo] == 1)
					 	{
					 		gSelectedDacPointDataNum[CurrentChannelNo]--;
					 	}
						else
						{
							if(gwDeleteStep == (gSelectedDacPointDataNum[CurrentChannelNo] - 1))//ɾ���һ����
							{
								gSelectedDacPointDataNum[CurrentChannelNo]--;
								gwDeleteStep--;
							}
							else
							{
								for(WORD i = gwDeleteStep; i < (WORD)gSelectedDacPointDataNum[CurrentChannelNo] - 1; i++)
								{
									gSelectedDacPointData[CurrentChannelNo][i] = gSelectedDacPointData[CurrentChannelNo][i + 1];//ѡ�е����ĵ�������ǰ��
								}
								gSelectedDacPointDataNum[CurrentChannelNo]--;
							}
						}
						gWaveDrawTcgFlag = TRUE;
					}
				}
			//	else if(wParam == '\t')
			//	{
			//		SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			//	}
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
				/*else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
			}
			break;
	//	case WM_CTLCOLORSTATIC:
	//		SetBkMode((HDC)wParam,TRANSPARENT);
	//		SetTextColor((HDC)wParam,STATIC_TEXT_COLOR);
	//		return (LRESULT)hStaticBrush;
		default:
			break;
	}
	return CallWindowProc(OldBtnCreateCurveDeletePointProc,Wnd,Msg,wParam,lParam);
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
LRESULT CALLBACK btnCreateCurveEndProc( HWND Wnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
	switch(Msg)
	{
		case WM_KEYDOWN:
			{
				int i = 0;
				if(wParam == VK_ESCAPE)
				{
					DestroyWindow(hWndCreateCurve);
					hWndCreateCurve = 0;
					ShowWindow(hWndMakeCurve,SW_SHOW);
					SetFocus(currentMakeCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					gCurrentFocusBtn = currentMakeCurveBtn;

					gCurrentWnd = hWndMakeCurve;

					gWaveDrawTcgFlag = FALSE;
				}
			
				else if(wParam == VK_UP)
				{
					TCHAR strText[20];
					memset(strText, 0, sizeof(strText));
					while(createCurveBtnList[i] != currentCreateCurveBtn)
					{
						i++;
					}
					i--;
					if(i < 0)
						i = buttonNum - 1;
					currentCreateCurveBtn = createCurveBtnList[i];
					SetFocus(currentCreateCurveBtn);
					gCurrentFocusBtn = currentCreateCurveBtn;

					gbDeletePointFlag = TRUE;

					if(gSelectedDacPointDataNum[CurrentChannelNo])
					{
						gCurrentSelectedDacPointData = gSelectedDacPointData[CurrentChannelNo][gwDeleteStep];
						
						swprintf(strText, _T("%.2f,%d"), (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentSelectedDacPointData.serial/DATA_SIZE, (int)(gCurrentSelectedDacPointData.data * 100 / MAX_HEIGHT));
					}
					else
					{
						swprintf(strText, _T("������"));
						
					}
					DrawLableText(hBtnCreateCurveDeletePoint, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
				}
				
				else if(wParam == VK_DOWN)
				{
					while(createCurveBtnList[i] != currentCreateCurveBtn)
					{
						i++;
					}
					i++;
					if(i > (buttonNum - 1))
						i = 0;
					currentCreateCurveBtn = createCurveBtnList[i];
					SetFocus(currentCreateCurveBtn);
					gCurrentFocusBtn = currentCreateCurveBtn;
				}
				else if(wParam == VK_RETURN)
				{
					ReceiveParam_ad[CurrentChannelNo].fPlus = gfTcgFirstPointGain[CurrentChannelNo];
					SetPlusLableAndExecute(Wnd, gfTcgFirstPointGain[CurrentChannelNo], ReceiveParam_ad[CurrentChannelNo].fRefPlus, TRUE, CurrentChannelNo);
					getGain2TgcSram(gSelectedDacPointData[CurrentChannelNo], gSelectedDacPointDataNum[CurrentChannelNo], CurrentChannelNo);
					chCurrent_dac_reg->wDACEnable = 1;

					// songchenguang add start 2010-06-13
					// �������õ�Ĭ���ļ�
					SaveParamFile(lpDefaultParamFile);
					// songchenguang add end 2010-06-13

					DestroyWindow(hWndCreateCurve);
					hWndCreateCurve = 0;
					ShowWindow(hWndMakeCurve,SW_SHOW);
					SetFocus(currentMakeCurveBtn);//�ϼ��˵����ѡ�еİ�ť������뽹��
					gCurrentFocusBtn = currentMakeCurveBtn;

					gCurrentWnd = hWndMakeCurve;

					gWaveDrawTcgFlag = FALSE;
				}
			//	else if(wParam == '\t')
			//	{
			//		SendMessage(hWndMain, WM_CHAR, wParam, lParam);
			//	}
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
				/*else if(wParam == 'B')	// ����
				{
					SendMessage(hWndMain, WM_KEYDOWN, wParam, lParam);
				}*/
			}
			break;
		default:
			break;
	}
	return CallWindowProc(OldBtnCreateCurveEndProc,Wnd,Msg,wParam,lParam);
}
