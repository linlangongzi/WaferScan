/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 新建TCG曲线窗口源文件
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

HWND hWndCreateCurve = NULL;//新建DAC曲线窗口句柄
HWND hBtnCreateCurveAddPoint = NULL;//加样点按钮句柄
HWND hBtnCreateCurveDeletePoint = NULL;//删样点按钮句柄
HWND hBtnCreateCurveEnd = NULL;//结束按钮句柄
HWND createCurveBtnList[3] = {NULL,NULL,NULL};//新建曲线窗口的按钮表
WNDPROC OldBtnCreateCurveAddPointProc = NULL;//加样点按钮默认处理过程
WNDPROC OldBtnCreateCurveDeletePointProc = NULL;//删样点按钮默认处理过程
WNDPROC OldBtnCreateCurveEndProc = NULL;//结束按钮默认处理过程
HWND currentCreateCurveBtn = NULL;//新建曲线窗中当前选中的按钮

CURVEPOINT_DATA gSelectedDacPointData[CHANNEL_NUM][MAX_CURVE_POINT_NUM];//添加的样点数据队列
CURVEPOINT_DATA gCurrentSelectedDacPointData;//添加的样点中当前选中的DAC曲线点数据
DWORD gSelectedDacPointDataNum[CHANNEL_NUM] = {0,0};//已选中的DAC点数量
float gfTcgFirstPointGain[CHANNEL_NUM] = {0,0};		//添加的点第一个点对应的Db值

float gfOldTcgRange[CHANNEL_NUM] = {0, 0};				//最初生成TCG时的声程
short gwOldTcgSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];	//最初生成TCG时的每个点的位置

BOOL gWaveDrawTcgFlag = FALSE;//波形是否画TCG
static const int buttonNum = 3;//按钮控件数

extern HWND hWndMakeCurve;//制作曲线窗口
extern HWND currentMakeCurveBtn;//制作曲线窗口中当前选中的按钮
extern HINSTANCE hInst;//程序实例句柄
extern HWND hWndMain;//主窗口句柄
extern WORD CurrentChannelNo;
extern wchar_t lpDefaultParamFile[MAX_PATH];
extern float fRangeEnd[2];
extern float fRangeStart[2];
extern HWND gCurrentFocusBtn;
extern ARS200FPGA_CH_DAC_REG *chCurrent_dac_reg;
extern HWND gCurrentWnd;		//当前显示的子窗口
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern WORD gwDeleteStep;
extern BOOL gbDeletePointFlag;		//是否在删样点操作
extern PEAK_INFO gPeakDefectInfos[2][2];	//保存闸门峰值点信息
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];
extern HWND hBtnMakeCurveCreate;//创建按钮句柄
extern RECEIVE_PARAM_DATA ReceiveParam_ad[2];

/**************************
*注册新建曲线窗口
* @param    hInstance[in]:程序实例句柄
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
*新建曲线窗口处理过程
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

				//画按钮控件
				hBtnCreateCurveAddPoint = CreateWindow(_T("BUTTON"),_T("tcgAddPoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATECURVE_ADDPOINT,hInst,NULL);
				hBtnCreateCurveDeletePoint = CreateWindow(_T("BUTTON"),_T("tcgDeletePoint"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATECURVE_DELETEPOINT,hInst,NULL);
				hBtnCreateCurveEnd = CreateWindow(_T("BUTTON"),_T("tcgEnd"),WS_VISIBLE | WS_CHILD|BS_OWNERDRAW,0,
					0 + 2*BUTTON_4_EACH_HEIGHT,MENU_WIDTH,BUTTON_4_EACH_HEIGHT,hWnd,(HMENU)IDC_CREATECURVE_END,hInst,NULL);
			
				createCurveBtnList[0] = hBtnCreateCurveAddPoint;
				createCurveBtnList[1] = hBtnCreateCurveDeletePoint;
				createCurveBtnList[2] = hBtnCreateCurveEnd;
				//按钮控件的自定义处理过程
				OldBtnCreateCurveAddPointProc = (WNDPROC) GetWindowLong(hBtnCreateCurveAddPoint,GWL_WNDPROC);
				SetWindowLong(hBtnCreateCurveAddPoint,GWL_WNDPROC,(LONG)btnCreateCurveAddPointProc);
				OldBtnCreateCurveDeletePointProc = (WNDPROC) GetWindowLong(hBtnCreateCurveDeletePoint,GWL_WNDPROC);
				SetWindowLong(hBtnCreateCurveDeletePoint,GWL_WNDPROC,(LONG)btnCreateCurveDeletePointProc);
				OldBtnCreateCurveEndProc = (WNDPROC) GetWindowLong(hBtnCreateCurveEnd,GWL_WNDPROC);
				SetWindowLong(hBtnCreateCurveEnd,GWL_WNDPROC,(LONG)btnCreateCurveEndProc);
			
				currentCreateCurveBtn = createCurveBtnList[0];  //默认当前选中加样点参数按钮
				SetFocus(currentCreateCurveBtn);//选中按钮获得输入焦点
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
*加载新建曲线窗口按钮图片
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
*加样点按钮自定义处理过程
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
					SetFocus(currentMakeCurveBtn);//上级菜单最后选中的按钮获得输入焦点
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
						swprintf(strText, _T("无样点"));
						
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
					Sleep(100);//等待擦掉波形

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
				/*else if(wParam == 'B')	// 冻结
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
*删样点按钮自定义处理过程
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
					SetFocus(currentMakeCurveBtn);//上级菜单最后选中的按钮获得输入焦点
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
						swprintf(strText, _T("无样点"));
						
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
						swprintf(strText, _T("无样点"));
						
					}
					DrawLableText(Wnd, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_4_START_Y);
				}
				else if(wParam == VK_RETURN)
				{
					if(gSelectedDacPointDataNum[CurrentChannelNo])
					{
						gWaveDrawTcgFlag = FALSE;
						Sleep(100);//等待擦掉波形
					 	if(gSelectedDacPointDataNum[CurrentChannelNo] == 1)
					 	{
					 		gSelectedDacPointDataNum[CurrentChannelNo]--;
					 	}
						else
						{
							if(gwDeleteStep == (gSelectedDacPointDataNum[CurrentChannelNo] - 1))//删最后一个点
							{
								gSelectedDacPointDataNum[CurrentChannelNo]--;
								gwDeleteStep--;
							}
							else
							{
								for(WORD i = gwDeleteStep; i < (WORD)gSelectedDacPointDataNum[CurrentChannelNo] - 1; i++)
								{
									gSelectedDacPointData[CurrentChannelNo][i] = gSelectedDacPointData[CurrentChannelNo][i + 1];//选中点后面的点数据往前移
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
				/*else if(wParam == 'B')	// 冻结
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
*结束按钮自定义处理过程
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
					SetFocus(currentMakeCurveBtn);//上级菜单最后选中的按钮获得输入焦点
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
						swprintf(strText, _T("无样点"));
						
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
					// 保存配置到默认文件
					SaveParamFile(lpDefaultParamFile);
					// songchenguang add end 2010-06-13

					DestroyWindow(hWndCreateCurve);
					hWndCreateCurve = 0;
					ShowWindow(hWndMakeCurve,SW_SHOW);
					SetFocus(currentMakeCurveBtn);//上级菜单最后选中的按钮获得输入焦点
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
				/*else if(wParam == 'B')	// 冻结
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
