/**
 * Copyright(c) 2010 allrising Corporation , All Rights Reserved.
 *
 * 通用接口文件
 * Filename: win_com.cpp
 *
 * Spec: Trace Tag write here. If has not, delete this line. If have mutil-TraceTag, write one Tag per line
 *
 * @author       wangbf
 * @version      1.00         Date: 2010-04-15
 */
#include "stdafx.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include "win_com.h"
/*
#include "tsMenuWin.h"
#include "modeWin.h"
#include "paramWin.h"
#include "hotKeyWin.h"
#include "fileManageWin.h"
#include "craftParamWin.h"
#include "screenCopyWin.h"
#include "reportFormWin.h"
#include "otherConfigureWin.h"
#include "recvParamWin.h"
#include "sendParamWin.h"
#include "displayWin.h"
#include "curveWin.h"
#include "gateWin.h"
#include "makeCurveWin.h"
#include "createCurveWin.h"
#include "modifyCurveWin.h"
#include "fnMenuWin.h"
#include "fnSpecialDisplayWin.h"
#include "fnReferenceWaveWin.h"
#include "fnHuiBoBianMaWin.h"
#include "fnMenMeiZhanKuanWin.h"
#include "fnSpecialFunctionWin.h"
#include "fnQuantitativeAnalysisWin.h"
#include "fnDgsCurveWin.h"
#include "fnMenMeiZhanKuanWin.h"
#include "fnBScanWin.h"
#include "fnZhongHouBanTanShangWin.h"
#include "fnHanFengTanShangWin.h"
#include "fnVideoRecordWin.h"
#include "fnDgsMakeWin.h"
#include "fnCreateDgsWin.h"
#include "fnModifyDgsWin.h"
#include "fnPoKouSheZhiWin.h"
*/
//#include "bsp.h"


HWND gCurrentFocusBtn = NULL;

extern HMODULE hRes;
extern PIC_DATA gPicData[2];			// 图像数据
extern HWND hWndMain;
extern WORD CurrentChannelNo;
extern WORD *chCurrent_tgc;
//extern WORD *ch1_tgc;
//extern WORD *ch2_tgc;
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern SEND_PARAM_DATA SendParam_ad[2];
extern ARS200FPGA_CH_DAC_REG *chCurrent_dac_reg;
extern ARS200FPGA_CH_GATE_REG *chCurrent_gate_reg;
extern float fRangeEnd[2];
extern float fRangeStart[2];

extern WORD *chCurrent_gate1curve;
extern WORD *chCurrent_gate2curve;
extern WORD *chCurrent_gate3curve;
extern ARS200FPGA_CH_GATE_REG *ch1_gate_reg;
extern ARS200FPGA_CH_GATE_REG *ch2_gate_reg;
extern CURVEPOINT_DATA gSelectedDgsPointData[CHANNEL_NUM][DAC_CURVE_NUM][MAX_CURVE_POINT_NUM];//添加的样点数据队列
extern WORD gSelectedDgsPointDataNum[CHANNEL_NUM][DAC_CURVE_NUM];//已选中的DGS点数量
extern HWND hStatusBar;
extern GATE_INFO GateInfos[2][3];
extern float gfMinRange[2];
extern DISPLAY_SET_DLG_DATA DisplaySet_ad[2];
extern BOOL gDgsSwitch[CHANNEL_NUM][GATE_NUM];		//DAC通道开关
extern float gfOldDacRange[CHANNEL_NUM];
extern short gwOldDacSerial[CHANNEL_NUM][MAX_CURVE_POINT_NUM];

CURVEPOINT_DATA gGetAllPeakPointData[DATA_SIZE];
CURVEPOINT_DATA gCurrentPeakPointData;	//当前选中的峰值点数据
WORD nAddStep = 0;
static CURVEPOINT_DATA gPrePeakPointData;//前一个选中的峰值点数据
WORD nAllPeakPointDataNum = 0;
extern CRITICAL_SECTION csRange;
extern wchar_t gszTextPeak[20];
extern HWND hStatusBar;
extern CRITICAL_SECTION csCommand;
extern AECTHREAD_PARAM thread_param;
extern PEAK_INFO gPeakInfos[2][2];
extern CRITICAL_SECTION csPeakInfo;
extern int gCurrentSysColorIndex;
extern SYS_COLOR_INFO gstSysColor[SYS_COLOR_NUM];

extern TCHAR FileDirectory[MAX_PATH];
extern ATOM registerTsMenuChild(HINSTANCE hInstance);
extern ATOM registerModeChild(HINSTANCE hInstance);
extern ATOM registerParamChild(HINSTANCE hInstance);
extern ATOM registerHotKeyChild(HINSTANCE hInstance);
extern ATOM registerFileManageChild(HINSTANCE hInstance);
extern ATOM registerCraftParamChild(HINSTANCE hInstance);
extern ATOM registerReportFormChild(HINSTANCE hInstance);
extern ATOM registerScreenCopyChild(HINSTANCE hInstance);
extern ATOM registerOtherCfgChild(HINSTANCE hInstance);
extern ATOM registerRecvParamChild(HINSTANCE hInstance);
extern ATOM registerSendParamChild(HINSTANCE hInstance);
extern ATOM registerDisplayChild(HINSTANCE hInstance);
extern ATOM registerCurveChild(HINSTANCE hInstance);
extern ATOM registerGateDisplayChild(HINSTANCE hInstance);
extern ATOM registerGateChild(HINSTANCE hInstance);
extern ATOM registerMakeCurveChild(HINSTANCE hInstance);
extern ATOM registerCreateCurveChild(HINSTANCE hInstance);
extern ATOM registerModifyCurveChild(HINSTANCE hInstance);
extern ATOM registerFnMenuChild(HINSTANCE hInstance);
extern ATOM registerSpecialDisplayChild(HINSTANCE hInstance);
extern ATOM registerReferenceWaveChild(HINSTANCE hInstance);
extern ATOM registerHuiBoBianMaChild(HINSTANCE hInstance);
extern ATOM registerMenMeiZhanKuanChild(HINSTANCE hInstance);
extern ATOM registerBScanChild(HINSTANCE hInstance);
extern ATOM registerSpecialFunctionChild(HINSTANCE hInstance);
extern ATOM registerDgsCurveChild(HINSTANCE hInstance);
extern ATOM registerQuantitativeAnalysisChild(HINSTANCE hInstance);
extern ATOM registerAutoGainPlusChild(HINSTANCE hInstance);
extern ATOM registerAdjustChild(HINSTANCE hInstance);
extern ATOM registerZhongHouBanTanShangChild(HINSTANCE hInstance);
extern ATOM registerHanFengTanShangChild(HINSTANCE hInstance);
extern ATOM registerFuHeCaiLiaoJianCeChild(HINSTANCE hInstance);
extern ATOM registerLieWenCeGaoChild(HINSTANCE hInstance);
extern ATOM registerVideoRecordChild(HINSTANCE hInstance);
extern ATOM registerDgsMakeChild(HINSTANCE hInstance);
extern ATOM registerCreateDgsCurveChild(HINSTANCE hInstance);
extern ATOM registerModifyDgsCurveChild(HINSTANCE hInstance);
extern ATOM registerPoKouSheZhiChild(HINSTANCE hInstance);
extern ATOM registerTcgCurveChild(HINSTANCE hInstance);
extern ATOM registerWaveSpeedChild(HINSTANCE hInstance);
extern ATOM registerRefractAngleChild(HINSTANCE hInstance);
extern ATOM registerAllParamCfgChild(HINSTANCE hInstance);
extern ATOM registerAllParamCfgCaptionChild(HINSTANCE hInstance);
extern ATOM registerAllParamConfirmChild(HINSTANCE hInstance);
extern ATOM registerLogoChild(HINSTANCE hInstance);
extern ATOM registerZhuJianChild(HINSTANCE hInstance);
extern ATOM registerQuMianXiuZhengChild(HINSTANCE hInstance);
extern ATOM registerAvgCurveChild(HINSTANCE hInstance);
extern ATOM registerMakeAvgCurveChild(HINSTANCE hInstance);
extern ATOM registerCreateAvgCurveChild(HINSTANCE hInstance);
extern ATOM registerModifyAvgCurveChild(HINSTANCE hInstance);

extern ATOM registerChMenuChild(HINSTANCE hInstance);
extern ATOM registerChParamChild(HINSTANCE hInstance);
extern ATOM registerChMeasureCfgChild(HINSTANCE hInstance);
extern ATOM registerChAdjustChild(HINSTANCE hInstance);
extern ATOM registerChAlarmCfgChild(HINSTANCE hInstance);
extern ATOM registerChWaveSpeedAdjustChild(HINSTANCE hInstance);
extern ATOM registerChZeroAdjustChild(HINSTANCE hInstance);
extern ATOM registerChTempCompensateChild(HINSTANCE hInstance);
extern ATOM registerChTempCurveMakeChild(HINSTANCE hInstance);
extern ATOM registerChTempCurveCreateChild(HINSTANCE hInstance);
extern ATOM registerChHeadVCompensateChild(HINSTANCE hInstance);
extern ATOM registerChHeadVMakeChild(HINSTANCE hInstance);
extern ATOM registerChHeadVCreateChild(HINSTANCE hInstance);
extern ATOM registerChDisplayChild(HINSTANCE hInstance);
extern ATOM registerChMeasureContentChild(HINSTANCE hInstance);
extern ATOM registerChZeroSingleChild(HINSTANCE hInstance);
extern ATOM registerChZeroDoubleChild(HINSTANCE hInstance);
extern ATOM registerChAverageChild(HINSTANCE hInstance);

extern ATOM registerTlMenuChild(HINSTANCE hInstance);
extern ATOM registerTlAdjustChild(HINSTANCE hInstance);

extern HWND hBtnCreateDgsCurvePingDing;//TEST
extern HWND hBtnCreateDgsCurvePanHui;

extern float gfDefectZhi1[CHANNEL_NUM];			//所测孔径Φ1

extern HWND hBtnMakeAvgCurveType;//AVG类型按钮句柄
extern HWND hBtnMakeAvgCurveCreate;//AVG类型按钮句柄
extern HWND hBtnMakeAvgCurveModify;//AVG类型按钮句柄

/**************************
*注册所有的窗口
* @param    hInstance:程序实例句柄
* @return   void
* @since    1.00
***************************/
void registerAll(HINSTANCE hInstance)
{
	registerTsMenuChild(hInstance);
	registerModeChild(hInstance);
	registerParamChild(hInstance);
	registerHotKeyChild(hInstance);
	registerFileManageChild(hInstance);
	registerCraftParamChild(hInstance);
	registerReportFormChild(hInstance);
	registerScreenCopyChild(hInstance);
	registerOtherCfgChild(hInstance);
	registerRecvParamChild(hInstance);
	registerSendParamChild(hInstance);
	registerDisplayChild(hInstance);
	registerCurveChild(hInstance);
	registerGateDisplayChild(hInstance);
	registerGateChild(hInstance);
	registerMakeCurveChild(hInstance);
	registerCreateCurveChild(hInstance);
	registerModifyCurveChild(hInstance);
	registerFnMenuChild(hInstance);
	registerSpecialDisplayChild(hInstance);
	registerReferenceWaveChild(hInstance);
	registerHuiBoBianMaChild(hInstance);	
	registerMenMeiZhanKuanChild(hInstance);
	registerBScanChild(hInstance);
	registerSpecialFunctionChild(hInstance);
	registerDgsCurveChild(hInstance);
	registerQuantitativeAnalysisChild(hInstance);
	registerAutoGainPlusChild(hInstance);
	registerAdjustChild(hInstance);
	registerZhongHouBanTanShangChild(hInstance);
	registerHanFengTanShangChild(hInstance);
	registerFuHeCaiLiaoJianCeChild(hInstance);
	registerLieWenCeGaoChild(hInstance);
	registerVideoRecordChild(hInstance);
	registerDgsMakeChild(hInstance);
	registerCreateDgsCurveChild(hInstance);
	registerModifyDgsCurveChild(hInstance);
	registerPoKouSheZhiChild(hInstance);
	registerTcgCurveChild(hInstance);
	registerWaveSpeedChild(hInstance);
	registerRefractAngleChild(hInstance);
	registerAllParamCfgChild(hInstance);
	registerAllParamCfgCaptionChild(hInstance);
	registerAllParamConfirmChild(hInstance);
	registerLogoChild(hInstance);
	registerZhuJianChild(hInstance);
	registerQuMianXiuZhengChild(hInstance);
	registerAvgCurveChild(hInstance);
	registerMakeAvgCurveChild(hInstance);
	registerCreateAvgCurveChild(hInstance);
	registerModifyAvgCurveChild(hInstance);
	//测厚
	registerChMenuChild(hInstance);
	registerChParamChild(hInstance);
	registerChMeasureCfgChild(hInstance);
	registerChAdjustChild(hInstance);
	registerChAlarmCfgChild(hInstance);
	registerChWaveSpeedAdjustChild(hInstance);
	registerChZeroAdjustChild(hInstance);
	registerChTempCompensateChild(hInstance);
	registerChTempCurveMakeChild(hInstance);
	registerChTempCurveCreateChild(hInstance);
	registerChHeadVCompensateChild(hInstance);
	registerChHeadVMakeChild(hInstance);
	registerChHeadVCreateChild(hInstance);
	registerChDisplayChild(hInstance);
	registerChMeasureContentChild(hInstance);
	registerChZeroSingleChild(hInstance);
	registerChZeroDoubleChild(hInstance);
	registerChAverageChild(hInstance);	
	//铁路
	registerTlMenuChild(hInstance);
	registerTlAdjustChild(hInstance);
}
int com_GetButtonBitmapID(const BTN_BMP_INFO *p,int buttonID,int ButtonState)
{
	BTN_BMP_INFO btn_info;
	int ReturnValue=0;

	memset(&btn_info,0,sizeof(BTN_BMP_INFO));
	while((p->nBtnID!=E_TBL_END)&&(p->nBtnID!=buttonID)) 
		p++;
	if(p->nBtnID!=E_TBL_END)
	{
		memcpy(&btn_info,p,sizeof(BTN_BMP_INFO));
		if(ButtonState&ODS_SELECTED)
			ReturnValue=btn_info.nBtnFocID;
		else if(ButtonState&ODS_FOCUS)
			ReturnValue=btn_info.nBtnFocID;
		else
			ReturnValue=btn_info.nBtnNoFocID;
	}
	return ReturnValue;
}
/**************************
*加载按钮图片
* @param    hInstance[in]:程序实例句柄
* @param		pdis[in]:
* @param		pBtnBmpInfo[in]:指向BTN_BMP_INFO结构的指针  
* @return   int
* @since    1.00
***************************/
int WinDrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis,const BTN_BMP_INFO *pBtnBmpInfo)
{
	HBITMAP hbm = NULL	;
	BITMAP bm;
	int nIDBitmap = 0;
	HDC hdcBtn = NULL;
//	LONG Style = 0;
	int nButtonID = 0;
	int nState = 0;
	nButtonID=pdis->CtlID;
	nState=pdis->itemState;
//	Style=GetWindowLong(pdis->hwndItem,GWL_STYLE);
	nIDBitmap=com_GetButtonBitmapID(pBtnBmpInfo,nButtonID,nState);
	if(nIDBitmap!=0)
		hbm=LoadBitmap(hRes,MAKEINTRESOURCE(nIDBitmap));
	else
		return FALSE;
	hdcBtn=CreateCompatibleDC(pdis->hDC);
	
	SelectObject(hdcBtn,hbm);
	GetObject(hbm,sizeof(BITMAP),&bm);
//	BitBlt(pdis->hDC,0,0,pdis->rcItem.right-pdis->rcItem.left,pdis->rcItem.bottom-pdis->rcItem.top,
//			hdcBtn,0,0,SRCCOPY);
	StretchBlt(pdis->hDC,0,0,pdis->rcItem.right-pdis->rcItem.left,pdis->rcItem.bottom-pdis->rcItem.top,
				hdcBtn,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);

	DeleteObject(hbm);
	DeleteDC(hdcBtn);
//	ReleaseDC(pdis->hwndItem,hdcBtn);
	
	return TRUE;
}
/*
int WinDrawButtonBitmap(HINSTANCE hInstance,LPDRAWITEMSTRUCT pdis,const BTN_BMP_INFO *pBtnBmpInfo)
{
	HBITMAP hbm;
	BITMAP bm;
	int nIDBitmap;
	HDC hdcBtn;
	LONG Style;
	int ButtonID;
	int State;
	ButtonID=pdis->CtlID;
	State=pdis->itemState;
//	DebugMessage(TEXT(" Sub Button WM_DRAWITEM"));
	Style=GetWindowLong(pdis->hwndItem,GWL_STYLE);
	nIDBitmap=com_GetButtonBitmapID(pBtnBmpInfo,ButtonID,State);
	if(nIDBitmap!=0)
		hbm=LoadBitmap(hInstance,MAKEINTRESOURCE(nIDBitmap));
	else
		return FALSE;

	hdcBtn=CreateCompatibleDC(pdis->hDC);
	
	SelectObject(hdcBtn,hbm);
	GetObject(hbm,sizeof(BITMAP),&bm);
//	BitBlt(pdis->hDC,0,0,pdis->rcItem.right-pdis->rcItem.left,pdis->rcItem.bottom-pdis->rcItem.top,
//			hdcBtn,0,0,SRCCOPY);
	StretchBlt(pdis->hDC,0,0,pdis->rcItem.right-pdis->rcItem.left,pdis->rcItem.bottom-pdis->rcItem.top,
				hdcBtn,0,0,bm.bmWidth,bm.bmHeight,SRCCOPY);

	DeleteObject(hbm);
	DeleteDC(hdcBtn);
//	ReleaseDC(pdis->hwndItem,hdcBtn);
	
	return TRUE;
}
*/
/**************************
*截取指定区域的屏幕，并保存文件
* @param	fileName[in]:保存位图文件的文件路径，如果为NULL则不保存
* @param	bitCount[in]:bit深度，用于表示一个像素点所使用的数据长度
* @param	lpRect[in]:所要截取的屏幕区域，如果为NULL，则获取全屏
* @return   BITMAP
* @since    1.00
***************************/
BOOL ScreenCapture(LPCWSTR fileName, WORD bitCount, LPRECT lpRect)
{
	HANDLE hbmFile = NULL;
	DWORD dwWritten = 0;
	DWORD dwBitmapArraySize;//位图数据大小
	HDC hScrDC = NULL;
	HDC hMemDC = NULL;         
    int nScreenWidth = 0; 
	int nScreenHeight = 0;
	int ixStart = 0; 
	int iyStart = 0; 
	int ix = 0; 
	int iy = 0;
	//the pointer will save all pixel point's color value
	BYTE *lpBitmapBits = NULL; 
       
	//creates a device context for the screen device
    hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

	//get the screen point size
    nScreenWidth = GetDeviceCaps(hScrDC, HORZRES);
    nScreenHeight = GetDeviceCaps(hScrDC, VERTRES);
	if(lpRect == NULL)
	{
		ixStart = 0;
		iyStart = 0;
		ix = nScreenWidth;
		iy = nScreenHeight;
	}
	else
	{
		ixStart = lpRect->left;
		iyStart = lpRect->top;
		ix = lpRect->right - lpRect->left;
		iy = lpRect->bottom - lpRect -> top;
	}
    //creates a memory device context (DC) compatible with the screen device(hScrDC) 
	dwBitmapArraySize = ((((ix*bitCount) + 31) & ~31) >> 3) * iy;
    hMemDC = CreateCompatibleDC(hScrDC);

	//initialise the struct BITMAPINFO for the bimap infomation， 
	//in order to use the function CreateDIBSection
	// on wince os, each pixel stored by 24 bits(biBitCount=24) 
	//and no compressing(biCompression=0) 
    BITMAPINFO bmInfo; 
    ZeroMemory(&bmInfo, sizeof(BITMAPINFO));
    bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmInfo.bmiHeader.biWidth = ix;
    bmInfo.bmiHeader.biHeight = iy;
    bmInfo.bmiHeader.biPlanes = 1;
    bmInfo.bmiHeader.biBitCount = bitCount;
   
	//use the function CreateDIBSection and SelectObject 
	//in order to get the bimap pointer : lpBitmapBits 
    HBITMAP directBmp = CreateDIBSection(hMemDC, (BITMAPINFO*)&bmInfo, 
        DIB_RGB_COLORS, (void **)&lpBitmapBits, NULL, 0);
	HGDIOBJ previousObject = SelectObject(hMemDC, directBmp);
	
	// copy the screen dc to the memory dc
	BitBlt(hMemDC, 0, 0, ix, iy, hScrDC, ixStart, iyStart, SRCCOPY);
	//if you only want to get the every pixel color value, 
	//you can begin here and the following part of this function will be unuseful;
	//the following part is in order to write file;

	//bimap file header in order to write bmp file 
	BITMAPFILEHEADER bmFileHeader;
	ZeroMemory(&bmFileHeader, sizeof(BITMAPFILEHEADER));
	bmFileHeader.bfType = 0x4d42; //bmp 
    bmFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmFileHeader.bfSize = bmFileHeader.bfOffBits + dwBitmapArraySize;

	//写进文件
	hbmFile = CreateFile(fileName,
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	if(hbmFile == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("创建图片失败"));
		return FALSE;
	}

	if(!WriteFile(hbmFile, &bmFileHeader, sizeof(BITMAPFILEHEADER),&dwWritten, NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写图片失败"));
		
		CloseHandle(hbmFile);
		return FALSE;
	}
	if(!WriteFile(hbmFile, &(bmInfo.bmiHeader), sizeof(BITMAPINFOHEADER),&dwWritten, NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写图片失败"));
		
		CloseHandle(hbmFile);
		return FALSE;
	}
	if(!WriteFile(hbmFile, lpBitmapBits, dwBitmapArraySize,&dwWritten, NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写图片失败"));
		
		CloseHandle(hbmFile);
		return FALSE;
	}
	CloseHandle(hbmFile);
	
	PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("图片保存成功"));
	//删除
	DeleteObject(hMemDC);
	DeleteObject(hScrDC);
	DeleteObject(directBmp);
	DeleteObject(previousObject);

	return TRUE;
}

int saveDataToFile(LPCTSTR szFilePath,LPVOID lpData,DWORD dwDataSize)
{
	HANDLE hFileWrite = NULL;//文件句柄
	DWORD dwWriteDataSize = 0;
	hFileWrite = CreateFile(szFilePath,
								GENERIC_READ|GENERIC_WRITE,
								0,
								NULL,
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
//		MessageBox (hWndMain, TEXT ("open or creat file fail!"), TEXT("save picture"), 0) ;
		return -1;
	}
	if(!WriteFile(hFileWrite,lpData,dwDataSize,&dwWriteDataSize,NULL))
	{
//		MessageBox (hWndMain, TEXT ("write file fail!"), TEXT("writeFile"), 0) ;
		return -1;
	}
	CloseHandle(hFileWrite);
	return 0;
}
/*写字的范围固定为长度73，宽度25*/
void DrawLableText(HWND hLable, LPCTSTR lpString, int fontSize, COLORREF fontColour,int xStart, int yStart)
{
	if(hLable)
	{
		RECT rect; 
		RECT textRect;
		HBRUSH hbr = NULL;
		HFONT hOldFont = NULL;
		HFONT hFont = NULL;
		LOGFONT logFont;
		memset(&logFont, 0, sizeof(logFont));
		logFont.lfHeight = fontSize;        
	    logFont.lfWidth = 0;
		HDC hDC = GetDC(hLable);
		GetClientRect(hLable, &rect);
	//	textRect = rect;
		textRect.left = rect.left +xStart;
		textRect.top = rect.top + yStart;
		textRect.right = textRect.left + LABEL_WIDTH;
		textRect.bottom = textRect.top + LABEL_HEIGHT;

		hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
		FillRect(hDC, &textRect, hbr);

		hFont = CreateFontIndirect(&logFont);

		hOldFont = (HFONT) SelectObject (hDC, hFont) ;
		SetBkMode(hDC,TRANSPARENT);
		SetTextColor(hDC,fontColour);
		
	//	SetWindowText(hLable, lpString);
		DrawText(hDC,lpString,-1,&textRect,DT_CENTER|DT_VCENTER);

		SelectObject(hDC,hOldFont);	
		DeleteObject(hFont);
		ReleaseDC(hLable, hDC);
	}
}

/*写字的范围长度跟宽度由参数输入*/
void DrawLableText1(HWND hLable, LPCTSTR lpString, int fontSize, COLORREF fontColour,int xStart, int yStart,WORD wWidth, WORD wHeight)
{
	if(hLable)
	{
		RECT rect;
		RECT textRect;
		HBRUSH hbr = NULL;
		HFONT hOldFont = NULL;
		HFONT hFont = NULL;
		LOGFONT logFont;
		memset(&logFont, 0, sizeof(logFont));
		logFont.lfHeight = fontSize;        
	    logFont.lfWidth = 0;
		HDC hDC = GetDC(hLable);
		GetClientRect(hLable, &rect);
	//	textRect = rect;
		textRect.left = rect.left +xStart;
		textRect.top = rect.top + yStart;
		textRect.right = textRect.left + wWidth;
		textRect.bottom = textRect.top + wHeight;

		hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
		FillRect(hDC, &textRect, hbr);

		hFont = CreateFontIndirect(&logFont);

		hOldFont = (HFONT) SelectObject (hDC, hFont) ;
		SetBkMode(hDC,TRANSPARENT);
		SetTextColor(hDC,fontColour);
		
	//	SetWindowText(hLable, lpString);
		DrawText(hDC,lpString,-1,&textRect,DT_CENTER|DT_VCENTER);

		SelectObject(hDC,hOldFont);
		DeleteObject(hFont);
		ReleaseDC(hLable, hDC);
	}
}

/*写字的范围长度跟宽度由参数输入,不刷底色*/
void DrawLableText2(HWND hLable, LPCTSTR lpString, int fontSize, COLORREF fontColour,int xStart, int yStart,WORD wWidth, WORD wHeight)
{
	if(hLable)
	{
	//	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
		RECT rect;
		RECT textRect;
	//	HBRUSH hbr;
		HFONT hOldFont = NULL;
		HFONT hFont = NULL;
		LOGFONT logFont;
		memset(&logFont, 0, sizeof(logFont));
		logFont.lfHeight = fontSize;        
	    logFont.lfWidth = 0;
		HDC hDC = GetDC(hLable);
		GetClientRect(hLable, &rect);
	//	textRect = rect;
		textRect.left = rect.left +xStart;
		textRect.top = rect.top + yStart;
		textRect.right = textRect.left + wWidth;
		textRect.bottom = textRect.top + wHeight;

	//	hbr = CreateSolidBrush(wColorBack);
	//	FillRect(hDC, &textRect, hbr);

		hFont = CreateFontIndirect(&logFont);

		hOldFont = (HFONT) SelectObject (hDC, hFont) ;
		SetBkMode(hDC,TRANSPARENT);
		SetTextColor(hDC,fontColour);
		
		DrawText(hDC,lpString,-1,&textRect,DT_CENTER|DT_VCENTER);

		SelectObject(hDC,hOldFont);
		DeleteObject(hFont);
		ReleaseDC(hLable, hDC);
	}
}

/*写字的范围长度跟宽度由参数输入,自动换行*/
void DrawLableText3(HWND hLable, LPCTSTR lpString, int fontSize, COLORREF fontColour,int xStart, int yStart,WORD wWidth, WORD wHeight)
{
	if(hLable)
	{
		RECT rect;
		RECT textRect;
		HBRUSH hbr = NULL;
		HFONT hOldFont = NULL;
		HFONT hFont = NULL;
		LOGFONT logFont;
		memset(&logFont, 0, sizeof(logFont));
		logFont.lfHeight = fontSize;        
	    logFont.lfWidth = 0;
		HDC hDC = GetDC(hLable);
		GetClientRect(hLable, &rect);
	//	textRect = rect;
		textRect.left = rect.left +xStart;
		textRect.top = rect.top + yStart;
		textRect.right = textRect.left + wWidth;
		textRect.bottom = textRect.top + wHeight;

		hbr = (HBRUSH)GetStockObject(WHITE_BRUSH);
		FillRect(hDC, &textRect, hbr);

		hFont = CreateFontIndirect(&logFont);

		hOldFont = (HFONT) SelectObject (hDC, hFont) ;
		SetBkMode(hDC,TRANSPARENT);
		SetTextColor(hDC,fontColour);
		
	//	SetWindowText(hLable, lpString);
		DrawText(hDC,lpString,-1,&textRect,DT_CENTER | DT_WORDBREAK | DT_EDITCONTROL);

		SelectObject(hDC,hOldFont);
		DeleteObject(hFont);
		ReleaseDC(hLable, hDC);
	}
}

void DrawWaveAreaText(HDC hDC, RECT rect, LPCTSTR lpString, HFONT hFontInfo, COLORREF fontColour,int xStart, int yStart,WORD wWidth, WORD wHeight)
{
//	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
//	RECT rect;
	RECT textRect;
//	HBRUSH hbr;
//	HFONT hOldFont = NULL;
//	GetClientRect(hLable, &rect);	
//	textRect = rect;
/*	textRect.left = rect.left +xStart;
	textRect.top = rect.top + yStart;
	textRect.right = textRect.left + wWidth;
	textRect.bottom = textRect.top + wHeight;*/
	textRect.left = xStart;
	textRect.top = yStart;
	textRect.right = textRect.left + wWidth;
	textRect.bottom = textRect.top + wHeight;

//	hbr = CreateSolidBrush(wColorBack);
//	FillRect(hDC, &textRect, hbr);

	HFONT hFont = (HFONT) SelectObject (hDC, hFontInfo);
	SetBkMode(hDC,TRANSPARENT);
	COLORREF clrOld = SetTextColor(hDC,fontColour);
	
	DrawText(hDC,lpString,-1,&textRect,DT_CENTER|DT_VCENTER);

	hFontInfo = (HFONT) SelectObject (hDC, hFont) ;
	SetTextColor( hDC,  clrOld);

}

void drawSrting(HWND hLable, LPCTSTR lpString, int fontSize, COLORREF fontColour,int xStart, int yStart,int nWidth, int nHeight)
{
	RECT rect;
	RECT textRect;

	HFONT hOldFont = NULL;
	HFONT hFont = NULL;
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(logFont));
	logFont.lfHeight = fontSize;        
    logFont.lfWidth = 0;
	HDC hDC = GetDC(hLable);
	GetClientRect(hLable, &rect);
//	textRect = rect;
	textRect.left = rect.left +xStart;
	textRect.top = rect.top + yStart;
	textRect.right = textRect.left + nWidth;
	textRect.bottom = textRect.top + nHeight;

	hFont = CreateFontIndirect(&logFont);

	hOldFont = (HFONT) SelectObject (hDC, hFont) ;
	SetBkMode(hDC,TRANSPARENT);
	SetTextColor(hDC,fontColour);
	
//	SetWindowText(hLable, lpString);
	DrawText(hDC,lpString,-1,&textRect,DT_RIGHT|DT_VCENTER);

	SelectObject(hDC,hOldFont);
	ReleaseDC(hLable, hDC);
}

//有关画DAC、DGS曲线的函数
/*
void drawCurve(CURVEPOINT_DATA selectPointData[MAX_CURVE_POINT_NUM], DWORD dataNum, BOOL drawFlag, WORD wColor)
{
//	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
//	WORD *pPixel = NULL;
	POINT point1, point2;
//	int x, y;
	memset(&point1, 0 ,sizeof(POINT));
	memset(&point2, 0 ,sizeof(POINT));
	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	if(dataNum < 2)
		return;
	else
	{
		for(DWORD i = 0; i < dataNum - 1; i++)
		{
			point1 = dataToPoint(selectPointData[i]);
			point2 = dataToPoint(selectPointData[i + 1]);
			if(drawFlag == TRUE)
				drawLine(point1, point2, wColor);
			else
				drawLine(point1, point2, wColorBack);
		}
	}
}
*/
void drawCurve(RECT RctWave, CURVEPOINT_DATA selectPointData[MAX_CURVE_POINT_NUM], DWORD dwDataNum, WORD wColorBack, WORD wColor, BOOL bDrawFlag)
{
//	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
//	WORD *pPixel = NULL;
	POINT point1;
	POINT point2;

	memset(&point1, 0 ,sizeof(POINT));
	memset(&point2, 0 ,sizeof(POINT));
//	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	if(dwDataNum < 2)
		return;
	else
	{
		for(DWORD i = 0; i < dwDataNum - 1; i++)
		{
			if(selectPointData[i].data < 1000000)
			{
				point1 = dataToPoint1(selectPointData[i], RctWave);
				point2 = dataToPoint1(selectPointData[i + 1], RctWave);
				if(bDrawFlag == TRUE)
					drawLine1(point1, point2, wColor, RctWave);
				else
					drawLine1(point1, point2, wColorBack, RctWave);
			}
		}
	}
}
/*
void drawDacCurve(CURVEPOINT_DATA selectPointData[MAX_CURVE_POINT_NUM], DWORD dataNum, BOOL drawFlag, WORD wColor)
{
//	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
//	WORD *pPixel = NULL;
	POINT point1, point2;
	POINT pointLeft;		//最左边点
	POINT pointRight;		//最右边点
	POINT pointCurveLeft;	//曲线最左边点
	POINT pointCurveRight;	//曲线最右边点

	HDC hdc = GetDC(hWndMain);

	RECT rect;
	GetClipBox(hdc, &rect);

	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
//	int x, y;
	memset(&point1, 0 ,sizeof(POINT));
	memset(&point2, 0 ,sizeof(POINT));

	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	
	if(dataNum < 2)
		return;
	else
	{
		for(DWORD i = 0; i < dataNum - 1; i++)
		{
			point1 = dataToPoint(selectPointData[i]);
			point2 = dataToPoint(selectPointData[i + 1]);
			if(drawFlag == TRUE)
				drawLine(point1, point2, wColor);
			else
				drawLine(point1, point2, wColorBack);
		}
	}
	pointCurveLeft = dataToPoint(selectPointData[0]);
	pointCurveRight = dataToPoint(selectPointData[dataNum - 1]);
	
	pointLeft.x = RctWave.left;
	pointLeft.y = pointCurveLeft.y;

	pointRight.x = RctWave.right - 1;
	pointRight.y = pointCurveRight.y;

	if(drawFlag == TRUE)
	{
		drawLine(pointLeft, pointCurveLeft, wColor);
		drawLine(pointCurveRight, pointRight, wColor);
	}
	else
	{
		drawLine(pointLeft, pointCurveLeft, wColorBack);
		drawLine(pointCurveRight, pointRight, wColorBack);
	}
	
	ReleaseDC(hWndMain,hdc);
}
*/
void drawDacDashCurve(CURVEPOINT_DATA selectPointData[MAX_CURVE_POINT_NUM], DWORD dataNum, BOOL drawFlag, WORD wColor)
{
//	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
//	WORD *pPixel = NULL;
	POINT point1;
	POINT point2;
	POINT pointLeft;		//最左边点
	POINT pointRight;		//最右边点
	POINT pointCurveLeft;	//曲线最左边点
	POINT pointCurveRight;	//曲线最右边点

	HDC hdc = GetDC(hWndMain);

	RECT rect;
	GetClipBox(hdc, &rect);

	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;

	memset(&point1, 0 ,sizeof(POINT));
	memset(&point2, 0 ,sizeof(POINT));
	memset(&pointLeft, 0 ,sizeof(POINT));
	memset(&pointRight, 0 ,sizeof(POINT));
	memset(&pointCurveLeft, 0 ,sizeof(POINT));
	memset(&pointCurveRight, 0 ,sizeof(POINT));

	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	
	if(dataNum < 2)
		return;
	else
	{
		for(DWORD i = 0; i < dataNum - 1; i++)
		{
			point1 = dataToPoint(selectPointData[i]);
			point2 = dataToPoint(selectPointData[i + 1]);
			if(drawFlag == TRUE)
				drawDashLine(point1, point2, wColor);
			else
				drawDashLine(point1, point2, wColorBack);
		}
	}
	pointCurveLeft = dataToPoint(selectPointData[0]);
	pointCurveRight = dataToPoint(selectPointData[dataNum - 1]);
	
	pointLeft.x = RctWave.left;
	pointLeft.y = pointCurveLeft.y;

	pointRight.x = RctWave.right - 1;
	pointRight.y = pointCurveRight.y;

	if(drawFlag == TRUE)
	{
		drawDashLine(pointLeft, pointCurveLeft, wColor);
		drawDashLine(pointCurveRight, pointRight, wColor);
	}
	else
	{
		drawDashLine(pointLeft, pointCurveLeft, wColorBack);
		drawDashLine(pointCurveRight, pointRight, wColorBack);
	}
	
	ReleaseDC(hWndMain,hdc);
}
//没用
void eraseDrawCurve(CURVEPOINT_DATA selectDacPointData[MAX_CURVE_POINT_NUM], DWORD dataNum, WORD nBackColor,WORD wColor)
{
//	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
//	WORD *pPixel = NULL;
	POINT point1;
	POINT point2;
//	int x, y;
	memset(&point1, 0 ,sizeof(POINT));
	memset(&point2, 0 ,sizeof(POINT));
	if(dataNum < 2)
		return;
	else
	{
		for(DWORD i = 0; i < dataNum - 1; i++)
		{
			point1 = dataToPoint(selectDacPointData[i]);
			point2 = dataToPoint(selectDacPointData[i + 1]);
			
			eraseDrawLine(point1, point2, nBackColor, wColor);
		}
	}
}
void drawDashLine(POINT point1, POINT point2, WORD wColor)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;
	int x = 0;
	int y = 0;
	DWORD dwUnitLength = 10;	//间隔长度
	DWORD dwLineLength; //线的长度
	dwLineLength = (DWORD)sqrt(pow((double)(abs(point1.x - point2.x)), 2) + pow((double)(abs(point1.y - point2.y)), 2));
	DWORD dwNum; //线分成的等分数
	dwNum = dwLineLength / dwUnitLength;
	
	HDC hdc = GetDC(hWndMain);
	RECT rect;
	GetClipBox(hdc, &rect);
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	
	//if(dwNum == 0)
	//	dwNum = 1;
	//if((dwNum & 1) == 0)//为偶数
	//	dwNum = dwNum - 1; //变成奇数
	if(point1.x < point2.x)
	{
		int nNum = 1;
		int nLenth = 0;
		for(int j = point1.x; j < point2.x; j++)
		{
			y = point1.y + (j - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
			if((y >= RctWave.top) && (y <= RctWave.bottom) && (j >= RctWave.left) && (j <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + y*LCD_XSIZE_TFT + j;

				nLenth = (int)sqrt(pow((double)(abs(j - point1.x)), 2) + pow((double)(abs(y - point1.y)), 2));
				if(nLenth / (nNum * dwUnitLength) == 1)//超过一等分的虚线长度
					nNum++;
				if(((nNum & 1) == 1) || (nNum == dwNum))
					*pPixel = wColor;
			}
		}
	}
	else if(point1.x > point2.x)
	{
		int nNum = 1;
		int nLenth = 0;
		for(int j = point1.x; j > point2.x; j--)
		{
			y = point1.y + (j - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
			if((y >= RctWave.top) && (y <= RctWave.bottom) && (j >= RctWave.left) && (j <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + y*LCD_XSIZE_TFT + j;

				nLenth = (int)sqrt(pow((double)(abs(j - point1.x)), 2) + pow((double)(abs(y - point1.y)), 2));
				if(nLenth / (nNum * dwUnitLength) == 1)
					nNum++;
				if(((nNum & 1) == 1) || (nNum == dwNum))
					*pPixel = wColor;
			}
		}
	}
	if(point1.y < point2.y)
	{
		int nNum = 1;
		int nLenth = 0;
		for(int k = point1.y; k < point2.y; k++)
		{
			x = point1.x + (point2.x - point1.x) * (k - point1.y) / (point2.y - point1.y);
			if((k >= RctWave.top) && (k <= RctWave.bottom) && (x >= RctWave.left) && (x <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + k*LCD_XSIZE_TFT + x;

				nLenth = (int)sqrt(pow((double)(abs(x - point1.x)), 2) + pow((double)(abs(k - point1.y)), 2));
				if(nLenth / (nNum * dwUnitLength) == 1)
					nNum++;
				if(((nNum & 1) == 1) || (nNum == dwNum))
					*pPixel = wColor;
			}
		}
	}
	else if(point1.y > point2.y)
	{
		int nNum = 1;
		int nLenth = 0;
		for(int k = point1.y; k > point2.y; k--)
		{
			x = point1.x + (point2.x - point1.x) * (k - point1.y) / (point2.y - point1.y);
			if((k >= RctWave.top) && (k <= RctWave.bottom) && (x >= RctWave.left) && (x <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + k*LCD_XSIZE_TFT + x;

				nLenth = (int)sqrt(pow((double)(abs(x - point1.x)), 2) + pow((double)(abs(k - point1.y)), 2));
				if(nLenth / (nNum * dwUnitLength) == 1)
					nNum++;
				if(((nNum & 1) == 1) || (nNum == dwNum))
					*pPixel = wColor;
			}
		}
	}
	ReleaseDC(hWndMain,hdc);
	*/
}
void drawDashLine1(POINT point1, POINT point2, WORD wColor, RECT RctWave)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;
	int x = 0;
	int y = 0;
	DWORD dwUnitLength = 10;	//间隔长度
	DWORD dwLineLength; //线的长度
	dwLineLength = (DWORD)sqrt(pow((double)(abs(point1.x - point2.x)), 2) + pow((double)(abs(point1.y - point2.y)), 2));
	DWORD dwNum; //线分成的等分数
	dwNum = dwLineLength / dwUnitLength;
	
	//if(dwNum == 0)
	//	dwNum = 1;
	//if((dwNum & 1) == 0)//为偶数
	//	dwNum = dwNum - 1; //变成奇数
	if(point1.x < point2.x)
	{
		int nNum = 1;
		int nLenth = 0;
		for(int j = point1.x; j < point2.x; j++)
		{
			y = point1.y + (j - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
			if((y >= RctWave.top) && (y <= RctWave.bottom) && (j >= RctWave.left) && (j <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + y*LCD_XSIZE_TFT + j;

				nLenth = (int)sqrt(pow((double)(abs(j - point1.x)), 2) + pow((double)(abs(y - point1.y)), 2));
				if(nLenth / (nNum * dwUnitLength) == 1)//超过一等分的虚线长度
					nNum++;
				if(((nNum & 1) == 1) || (nNum == dwNum))
					*pPixel = wColor;
			}
		}
	}
	else if(point1.x > point2.x)
	{
		int nNum = 1;
		int nLenth = 0;
		for(int j = point1.x; j > point2.x; j--)
		{
			y = point1.y + (j - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
			if((y >= RctWave.top) && (y <= RctWave.bottom) && (j >= RctWave.left) && (j <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + y*LCD_XSIZE_TFT + j;

				nLenth = (int)sqrt(pow((double)(abs(j - point1.x)), 2) + pow((double)(abs(y - point1.y)), 2));
				if(nLenth / (nNum * dwUnitLength) == 1)
					nNum++;
				if(((nNum & 1) == 1) || (nNum == dwNum))
					*pPixel = wColor;
			}
		}
	}
	if(point1.y < point2.y)
	{
		int nNum = 1;
		int nLenth = 0;
		for(int k = point1.y; k < point2.y; k++)
		{
			x = point1.x + (point2.x - point1.x) * (k - point1.y) / (point2.y - point1.y);
			if((k >= RctWave.top) && (k <= RctWave.bottom) && (x >= RctWave.left) && (x <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + k*LCD_XSIZE_TFT + x;

				nLenth = (int)sqrt(pow((double)(abs(x - point1.x)), 2) + pow((double)(abs(k - point1.y)), 2));
				if(nLenth / (nNum * dwUnitLength) == 1)
					nNum++;
				if(((nNum & 1) == 1) || (nNum == dwNum))
					*pPixel = wColor;
			}
		}
	}
	else if(point1.y > point2.y)
	{
		int nNum = 1;
		int nLenth = 0;
		for(int k = point1.y; k > point2.y; k--)
		{
			x = point1.x + (point2.x - point1.x) * (k - point1.y) / (point2.y - point1.y);
			if((k >= RctWave.top) && (k <= RctWave.bottom) && (x >= RctWave.left) && (x <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + k*LCD_XSIZE_TFT + x;

				nLenth = (int)sqrt(pow((double)(abs(x - point1.x)), 2) + pow((double)(abs(k - point1.y)), 2));
				if(nLenth / (nNum * dwUnitLength) == 1)
					nNum++;
				if(((nNum & 1) == 1) || (nNum == dwNum))
					*pPixel = wColor;
			}
		}
	}
	*/
}

//没用
void drawDashCurve(RECT RctWave, CURVEPOINT_DATA selectPointData[MAX_CURVE_POINT_NUM], DWORD dwDataNum, WORD wColorBack, WORD wColor, BOOL bDrawFlag)
{
//	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
//	WORD *pPixel = NULL;
	POINT point1;
	POINT point2;

	memset(&point1, 0 ,sizeof(POINT));
	memset(&point2, 0 ,sizeof(POINT));
	if(dwDataNum < 2)
		return;
	else
	{
		for(DWORD i = 0; i < dwDataNum - 1; i++)
		{
			if(selectPointData[i].data < 1000000)
			{
				point1 = dataToPoint1(selectPointData[i], RctWave);
				point2 = dataToPoint1(selectPointData[i + 1], RctWave);
				if(bDrawFlag == TRUE)
					drawDashLine1(point1, point2, wColor, RctWave);
				else
					drawDashLine1(point1, point2, wColorBack, RctWave);
			}
		}
	}
}


void drawLine(POINT point1, POINT point2, WORD wColor)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;

	HDC hdc = GetDC(hWndMain);

	RECT rect;
	GetClipBox(hdc, &rect);
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;

	int x = 0;
	int y = 0;
	if(point1.x < point2.x)
	{
		for(int j = point1.x; j < point2.x; j++)
		{
			y = point1.y + (j - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
			if((y >= RctWave.top) && (y <= RctWave.bottom) && (j >= RctWave.left) && (j <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + y*LCD_XSIZE_TFT + j;
				*pPixel = wColor;
			}
		}
	}
	else if(point1.x > point2.x)
	{
		for(int j = point1.x; j > point2.x; j--)
		{
			y = point1.y + (j - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
			if((y >= RctWave.top) && (y <= RctWave.bottom) && (j >= RctWave.left) && (j <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + y*LCD_XSIZE_TFT + j;
				*pPixel = wColor;
			}
		}
	}
	if(point1.y < point2.y)
	{
		for(int k = point1.y; k < point2.y; k++)
		{
			x = point1.x + (point2.x - point1.x) * (k - point1.y) / (point2.y - point1.y);
			if((k >= RctWave.top) && (k <= RctWave.bottom) && (x >= RctWave.left) && (x <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + k*LCD_XSIZE_TFT + x;
				*pPixel = wColor;
			}
		}
	}
	else if(point1.y > point2.y)
	{
		for(int k = point1.y; k > point2.y; k--)
		{
			x = point1.x + (point2.x - point1.x) * (k - point1.y) / (point2.y - point1.y);
			if((k >= RctWave.top) && (k <= RctWave.bottom) && (x >= RctWave.left) && (x <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + k*LCD_XSIZE_TFT + x;
				*pPixel = wColor;
			}
		}
	}
	ReleaseDC(hWndMain,hdc);
	*/
}
void drawLine1(POINT point1, POINT point2, WORD wColor, RECT RctWave)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;

	int x = 0;
	int y = 0;
	if(point1.x < point2.x)
	{
		for(int j = point1.x; j < point2.x; j++)
		{
			y = point1.y + (j - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
			if((y >= RctWave.top) && (y <= RctWave.bottom) && (j >= RctWave.left) && (j <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + y*LCD_XSIZE_TFT + j;
				*pPixel = wColor;
			}
		}
	}
	else if(point1.x > point2.x)
	{
		for(int j = point1.x; j > point2.x; j--)
		{
			y = point1.y + (j - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
			if((y >= RctWave.top) && (y <= RctWave.bottom) && (j >= RctWave.left) && (j <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + y*LCD_XSIZE_TFT + j;
				*pPixel = wColor;
			}
		}
	}
	if(point1.y < point2.y)
	{
		for(int k = point1.y; k < point2.y; k++)
		{
			x = point1.x + (point2.x - point1.x) * (k - point1.y) / (point2.y - point1.y);
			if((k >= RctWave.top) && (k <= RctWave.bottom) && (x >= RctWave.left) && (x <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + k*LCD_XSIZE_TFT + x;
				*pPixel = wColor;
			}
		}
	}
	else if(point1.y > point2.y)
	{
		for(int k = point1.y; k > point2.y; k--)
		{
			x = point1.x + (point2.x - point1.x) * (k - point1.y) / (point2.y - point1.y);
			if((k >= RctWave.top) && (k <= RctWave.bottom) && (x >= RctWave.left) && (x <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + k*LCD_XSIZE_TFT + x;
				*pPixel = wColor;
			}
		}
	}
	*/
}

/*
void eraseDrawLine(POINT point1, POINT point2, WORD nBackColor, WORD wColor)
{
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;

	HDC hdc = GetDC(hWndMain);

	RECT rect;
	GetClipBox(hdc, &rect);

	int nWidth = rect.right - rect.left;
	int nHeight = rect.bottom;
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	
	int x, y;
	if(point1.x < point2.x)
	{
		for(int j = point1.x; j < point2.x; j++)
		{
			y = point1.y + (j - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
			
			if((y >= RctWave.top) && (y <= RctWave.bottom) && (j >= RctWave.left) && (j <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + y*LCD_XSIZE_TFT + j;
				if(*pPixel != wColor)
					*pPixel = nBackColor;
			}
		}
	}
	else if(point1.x > point2.x)
	{
		for(int j = point1.x; j > point2.x; j--)
		{
			y = point1.y + (j - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
			
			if((y >= RctWave.top) && (y <= RctWave.bottom) && (j >= RctWave.left) && (j <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + y*LCD_XSIZE_TFT + j;
				if(*pPixel != wColor)
					*pPixel = nBackColor;
			}
		}
	}
	if(point1.y < point2.y)
	{
		for(int k = point1.y; k < point2.y; k++)
		{
			x = point1.x + (point2.x - point1.x) * (k - point1.y) / (point2.y - point1.y);
			
			if((k >= RctWave.top) && (k <= RctWave.bottom) && (x >= RctWave.left) && (x <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + k*LCD_XSIZE_TFT + x;
				if(*pPixel != wColor)
					*pPixel = nBackColor;
			}
		}
	}
	else if(point1.y > point2.y)
	{
		for(int k = point1.y; k > point2.y; k--)
		{
			x = point1.x + (point2.x - point1.x) * (k - point1.y) / (point2.y - point1.y);
			
			if((k >= RctWave.top) && (k <= RctWave.bottom) && (x >= RctWave.left) && (x <= RctWave.right))//确保在图形区里
			{
				pPixel = pBaseAddress + k*LCD_XSIZE_TFT + x;
				if(*pPixel != wColor)
					*pPixel = nBackColor;
			}
		}
	}
	ReleaseDC(hWndMain,hdc);
}
*/
void eraseDrawLine(POINT point1, POINT point2, WORD nBackColor, WORD wColor)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;
	int x = 0;
	int y = 0;
	if(point1.x < point2.x)
	{
		for(int j = point1.x; j < point2.x; j++)
		{
			y = point1.y + (j - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
			pPixel = pBaseAddress + y*LCD_XSIZE_TFT + j;
			if(*pPixel != wColor)
				*pPixel = nBackColor;
		}
	}
	else if(point1.x > point2.x)
	{
		for(int j = point1.x; j > point2.x; j--)
		{
			x = point1.x + (j - point1.x) * (point2.y - point1.y) / (point2.x - point1.x);
			pPixel = pBaseAddress + j*LCD_XSIZE_TFT + x;
			if(*pPixel != wColor)
				*pPixel = nBackColor;
		}
	}
	if(point1.y < point2.y)
	{
		for(int k = point1.y; k < point2.y; k++)
		{
			x = point1.x + (point2.x - point1.x) * (k - point1.y) / (point2.y - point1.y);
			pPixel = pBaseAddress + k*LCD_XSIZE_TFT + x;
			if(*pPixel != wColor)
				*pPixel = nBackColor;
		}
	}
	else if(point1.y > point2.y)
	{
		for(int k = point1.y; k > point2.y; k--)
		{
			x = point1.x + (point2.x - point1.x) * (k - point1.y) / (point2.y - point1.y);
			pPixel = pBaseAddress + k*LCD_XSIZE_TFT + x;
			if(*pPixel != wColor)
				*pPixel = nBackColor;
		}
	}
	*/
}

CURVEPOINT_DATA getHeightestPoinData(CURVEPOINT_DATA dataForSearch[MAX_CURVE_POINT_NUM], DWORD dataNum)
{
	CURVEPOINT_DATA tempData;
	memset(&tempData, 0, sizeof(CURVEPOINT_DATA));
	tempData = dataForSearch[0];
	for(DWORD i = 0; i < dataNum; i++)
	{
		if(tempData.data < dataForSearch[i].data)
		{
			tempData = dataForSearch[i];
		}
	}
	return tempData;
}

POINT dataToPoint(CURVEPOINT_DATA curve_point_data)
{
	POINT curve_point;
	memset(&curve_point, 0 ,sizeof(POINT));
	
	HDC hdc = GetDC(hWndMain);

	RECT rect;
	GetClipBox(hdc, &rect);

	int nWidth = rect.right - rect.left;
	int nHeight = rect.bottom;
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	// 波形区域宽度、高度
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;
	float yb = nWaveHeight/MAX_HEIGHT;
	float xb = nWaveWidth/512.0f;
	long x = 0;
	long y = 0;
	x = long(RctWave.left+curve_point_data.serial*xb);
/*	if(x < RctWave.left)
		x = RctWave.left;
	if(x > RctWave.right)
		x = RctWave.right;*/
	y = long((nWaveHeight-1)-curve_point_data.data*yb) + RctWave.top;
/*	if(y < RctWave.top)
		y = RctWave.top;
	if(y > RctWave.bottom)
		y = RctWave.bottom;*/
	curve_point.x = (DWORD)x;
	curve_point.y = (DWORD)y;
	ReleaseDC(hWndMain,hdc);
	return curve_point;
}
POINT dataToPoint1(CURVEPOINT_DATA curve_point_data, RECT RctWave)
{
	POINT curve_point;
	memset(&curve_point, 0 ,sizeof(POINT));
	// 波形区域宽度、高度
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;
	float yb = nWaveHeight/MAX_HEIGHT;
	float xb = nWaveWidth/512.0f;
	long x = 0;
	long y = 0;
	x = long(RctWave.left+curve_point_data.serial*xb);
/*	if(x < RctWave.left)
		x = RctWave.left;
	if(x > RctWave.right)
		x = RctWave.right;*/
	y = long((nWaveHeight-1)-curve_point_data.data*yb) + RctWave.top;
/*	if(y < RctWave.top)
		y = RctWave.top;
	if(y > RctWave.bottom)
		y = RctWave.bottom;*/
	curve_point.x = (DWORD)x;
	curve_point.y = (DWORD)y;
	
	return curve_point;
}


BOOL buDian(CURVEPOINT_DATA queueToBuDian[MAX_CURVE_POINT_NUM],CURVEPOINT_DATA queueAfterBuDian[MAX_CURVE_POINT_NUM], DWORD dataNum)
{
	CURVEPOINT_DATA temp[MAX_CURVE_POINT_NUM];
	memset(temp, 0, sizeof(temp));
	if(dataNum < 2)
		return FALSE;
	else
	{
		for(DWORD i = 0; i < dataNum; i++)
		{
			temp[i + 1] = queueToBuDian[i];
		}
		temp[0] =  queueToBuDian[0];
		temp[dataNum + 1] = queueToBuDian[dataNum - 1];
		for(DWORD i = 0; i < dataNum + 2; i++)
		{
			queueAfterBuDian[i] = temp[i];
		}
	}

	return TRUE;
}//前后沿水平延长3个采样点的声程，前后各补两个点

void getGain2TgcSram(CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dataNum,WORD channelNumber)
{
	float fSampleCalc = ADFREQUENCY * 1000000 / 50.0;
	CURVEPOINT_DATA tempPointData1;
	CURVEPOINT_DATA tempPointData2;
	CURVEPOINT_DATA heightestPointData;
	CURVEPOINT_DATA afterBuDianData[MAX_CURVE_POINT_NUM];
	memset(afterBuDianData, 0, sizeof(afterBuDianData));
	memset(&tempPointData1, 0 ,sizeof(CURVEPOINT_DATA));
	memset(&tempPointData2, 0 ,sizeof(CURVEPOINT_DATA));
	memset(&heightestPointData, 0 ,sizeof(CURVEPOINT_DATA));
	
	float soundDistance = 0.0f;//两个DAC点之间的声程
	float firstSoundDistance = 0.0f;
	float lastSoundDistance = 0.0f;
//	float perSampleDistance;//每个采样点之间间隔的声程
	double y = 0;
	float gain = 0.0f;
	float qianHouYanAdd  = 0.0f;
	int smpleNum = 0;//两个DAC点之间的声程的采样点数
	WORD* pData = NULL;
//	char buffer[20];
//	wchar_t szFilePath1[40] = _T("\\ResidentFlash\\y.txt");
//	wchar_t szFilePath2[40] = _T("\\ResidentFlash\\gainBefore.txt");
//	wchar_t szFilePath3[40] = _T("\\ResidentFlash\\gainAfter.txt");
//	swprintf(szFilePath1, _T("%s\\y.txt"), FileDirectory);
//	swprintf(szFilePath2, _T("%s\\gainBefore.txt"), FileDirectory);
//	swprintf(szFilePath3, _T("%s\\gainAfter.txt"), FileDirectory);
	heightestPointData = getHeightestPoinData(pointData, dataNum);
//	HANDLE hFileWrite1, hFileWrite2, hFileWrite3;//文件句柄
//	DWORD dwWriteDataSize;
/*	hFileWrite2 = CreateFile(szFilePath2,
								GENERIC_READ|GENERIC_WRITE,
								0,
								NULL,
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	hFileWrite1 = CreateFile(szFilePath1,
								GENERIC_READ|GENERIC_WRITE,
								0,
								NULL,
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	hFileWrite3 = CreateFile(szFilePath3,
								GENERIC_READ|GENERIC_WRITE,
								0,
								NULL,
								CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
*/
//	if(channelNumber == 0)
//		pData = ch1_tgc;
//	else if(channelNumber == 1)
//		pData = ch2_tgc; 
	pData = chCurrent_tgc;
	
	qianHouYanAdd = float((8 * DisplaySet_ad[channelNumber].nWaveSpeed / fSampleCalc / 2) * 1000);
	BOOL bResult = buDian(pointData, afterBuDianData, dataNum);
	if(bResult)
		dataNum = dataNum + 2;
	if(dataNum < 2)
		return;
	else
	{
		for(DWORD i = 0; i < dataNum - 1; i++)
		{
			tempPointData1 = afterBuDianData[i];
			tempPointData2 = afterBuDianData[i + 1];

			soundDistance = (fRangeEnd[channelNumber] - fRangeStart[channelNumber])*(afterBuDianData[i + 1].serial - afterBuDianData[i].serial)/DATA_SIZE;
			smpleNum = (int)((soundDistance * 2 / 1000) * fSampleCalc / DisplaySet_ad[channelNumber].nWaveSpeed) ;

			if(i == 0)
			{
				soundDistance = qianHouYanAdd;
				smpleNum = (int)((soundDistance * 2 / 1000) * fSampleCalc / DisplaySet_ad[channelNumber].nWaveSpeed) ;
				for(int j = 0; j <= smpleNum; j++)
				{					
				/*	char buffer1[20];
					char buffer2[20];
					char buffer3[20];
					memset(buffer1, 0, sizeof(buffer1));
					memset(buffer2, 0, sizeof(buffer2));
					memset(buffer3, 0, sizeof(buffer3));
					*/
					y = WORD(tempPointData1.data + ((double)tempPointData2.data - (double)tempPointData1.data) / smpleNum * j);
					
					gain = (float)(20 * log10((double)(heightestPointData.data / y)));
				

					
				/*	_itoa((WORD)y, buffer1, 10);
					buffer1[18] = '\r';
					buffer1[19] = '\n';
					WriteFile(hFileWrite1,buffer1,20,&dwWriteDataSize,NULL);

					_itoa((WORD)gain, buffer2, 10);
					buffer2[18] = '\r';
					buffer2[19] = '\n';
					WriteFile(hFileWrite2,buffer2,20,&dwWriteDataSize,NULL);					
				*/	
					*pData = DACdBProcess(gain);
				/*
					_itoa(DACdBProcess(gain), buffer3, 10);
					buffer3[18] = '\r';
					buffer3[19] = '\n';
					WriteFile(hFileWrite3,buffer3,20,&dwWriteDataSize,NULL);
				*/	

					pData++;
				}
			}
			else if(i == dataNum - 2)
			{
				soundDistance = qianHouYanAdd;
				smpleNum = (int)((soundDistance * 2 / 1000) * fSampleCalc / DisplaySet_ad[channelNumber].nWaveSpeed);
				for(int j = 1; j <= smpleNum; j++)
				{
				/*	char buffer1[20];
					char buffer2[20];
					char buffer3[20];
					memset(buffer1, 0, sizeof(buffer1));
					memset(buffer2, 0, sizeof(buffer2));
					memset(buffer3, 0, sizeof(buffer3));
					*/
					y = tempPointData1.data + ((double)tempPointData2.data - (double)tempPointData1.data) / smpleNum * j;
					gain = (float)(20 * log10((double)(heightestPointData.data / y)));
				
					
				/*	_itoa((WORD)y, buffer1, 10);
					buffer1[18] = '\r';
					buffer1[19] = '\n';
					WriteFile(hFileWrite1,buffer1,20,&dwWriteDataSize,NULL);

					_itoa((WORD)gain, buffer2, 10);
					buffer3[18] = '\r';
					buffer3[19] = '\n';
					WriteFile(hFileWrite2,buffer2,20,&dwWriteDataSize,NULL);
				*/
					*pData = DACdBProcess(gain);
				/*
					_itoa(DACdBProcess(gain), buffer3, 10);
					buffer3[18] = '\r';
					buffer3[19] = '\n';
					WriteFile(hFileWrite3,buffer3,20,&dwWriteDataSize,NULL);
					*/
					pData++;
				}
			}
			else
			{
				for(int j = 1; j <= smpleNum; j++)
				{
				/*	char buffer1[20];
					char buffer2[20];
					char buffer3[20];
					memset(buffer1, 0, sizeof(buffer1));
					memset(buffer2, 0, sizeof(buffer2));
					memset(buffer3, 0, sizeof(buffer3));
					*/
					y = tempPointData1.data + ((double)tempPointData2.data - (double)tempPointData1.data) / smpleNum * j;
					
					gain = (float)(20 * log10((double)(heightestPointData.data / y)));
		
				/*	_itoa((WORD)y, buffer1, 10);
					buffer1[18] = '\r';
					buffer1[19] = '\n';
					WriteFile(hFileWrite1,buffer1,20,&dwWriteDataSize,NULL);

					_itoa((WORD)gain, buffer2, 10);
					buffer3[18] = '\r';
					buffer3[19] = '\n';
					WriteFile(hFileWrite2,buffer2,20,&dwWriteDataSize,NULL);	
				*/
					*pData = DACdBProcess(gain);
				/*
					_itoa(DACdBProcess(gain), buffer3, 10);
					buffer3[18] = '\r';
					buffer3[19] = '\n';
					WriteFile(hFileWrite3,buffer3,20,&dwWriteDataSize,NULL);
					*/
					pData++;
				}
			}
		}
	
/*		memset(buffer, 0, sizeof(buffer));
		_itoa((WORD)firstSoundDistance, buffer, 10);
		sprintf(buffer, "%0.2f", qianHouYanAdd);
		buffer[18] = '\r';
		buffer[19] = '\n';
		WriteFile(hFileWrite1,buffer,20,&dwWriteDataSize,NULL);
*/
		firstSoundDistance = fRangeStart[channelNumber] + (fRangeEnd[channelNumber] - fRangeStart[channelNumber])*pointData[0].serial/DATA_SIZE;
/*		
		memset(buffer, 0, sizeof(buffer));
	//	_itoa((WORD)firstSoundDistance, buffer, 10);
		sprintf(buffer, "%0.2f", firstSoundDistance);
		buffer[18] = '\r';
		buffer[19] = '\n';
		WriteFile(hFileWrite1,buffer,20,&dwWriteDataSize,NULL);
*/		
		if((firstSoundDistance -  qianHouYanAdd)> 0.5)
			firstSoundDistance = firstSoundDistance - qianHouYanAdd;
		
		lastSoundDistance = fRangeStart[channelNumber] + (fRangeEnd[channelNumber] - fRangeStart[channelNumber])*pointData[dataNum - 3].serial/DATA_SIZE;		
/*		memset(buffer, 0, sizeof(buffer));
	//	_itoa((WORD)firstSoundDistance, buffer, 10);
		sprintf(buffer, "%0.2f", lastSoundDistance);
		buffer[18] = '\r';
		buffer[19] = '\n';
		WriteFile(hFileWrite1,buffer,20,&dwWriteDataSize,NULL);
*/
		lastSoundDistance = lastSoundDistance + qianHouYanAdd;
		
/*
		memset(buffer, 0, sizeof(buffer));
	//	_itoa((WORD)firstSoundDistance, buffer, 10);
		sprintf(buffer, "%0.2f", firstSoundDistance);
		buffer[18] = '\r';
		buffer[19] = '\n';
		WriteFile(hFileWrite1,buffer,20,&dwWriteDataSize,NULL);

		memset(buffer, 0, sizeof(buffer));
	//	_itoa((WORD)firstSoundDistance, buffer, 10);
		sprintf(buffer, "%0.2f", lastSoundDistance);
		buffer[18] = '\r';
		buffer[19] = '\n';
		WriteFile(hFileWrite1,buffer,20,&dwWriteDataSize,NULL);
*/
		chCurrent_dac_reg->wDACFront = (WORD)Range2Dot(DisplaySet_ad[channelNumber].nWaveSpeed, firstSoundDistance);
		chCurrent_dac_reg->wDACRear = (WORD)Range2Dot(DisplaySet_ad[channelNumber].nWaveSpeed, lastSoundDistance);
	}
//	CloseHandle(hFileWrite1);
//	CloseHandle(hFileWrite2);
}
/*
void drawCurve(CURVEPOINT_DATA dataToDraw[MAX_CURVE_POINT_NUM], DWORD dataNum, BOOL drawFlag)
{
	HPEN hOldPen, hLinePen;
	HDC hdc = GetDC(hWndMain);
	RECT rect;
	GetClipBox(hdc, &rect);
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	// 波形区域宽度、高度
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;
	float yb = nWaveHeight/MAX_HEIGHT;
	float xb = nWaveWidth/512.0f;
	float x, y;
	if(drawFlag == TRUE)
		hLinePen = CreatePen(PS_SOLID, 1, RGB(255,0,0));
	if(drawFlag == FALSE)
		hLinePen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
	
	hOldPen = (HPEN)SelectObject(hdc, hLinePen);
	x = long(RctWave.left+gSelectedDacPointData[CurrentChannelNo][0].serial*xb);
	y = long((nWaveHeight-1)-gSelectedDacPointData[CurrentChannelNo][0].data*yb) + RctWave.top;
	MoveToEx(hdc, (int)x, (int)y, NULL);
					
	for (int i = 1;i < dataNum; i++)
	{
 		if (gSelectedDacPointData[CurrentChannelNo][i].data > MAX_HEIGHT) 
 			gSelectedDacPointData[CurrentChannelNo][i].data = MAX_HEIGHT;
		x = long(RctWave.left+gSelectedDacPointData[CurrentChannelNo][i].serial*xb);
		y = long((nWaveHeight-1)-gSelectedDacPointData[CurrentChannelNo][i].data*yb) + RctWave.top;				
		LineTo(hdc, (int)x, (int)y);
	}
	SelectObject(hdc, hOldPen);
	ReleaseDC(hWndMain,hdc);
}
*/
void sortSelectedPointDataBySerial(CURVEPOINT_DATA queueToSort[MAX_CURVE_POINT_NUM], DWORD selectedDacPointDataNum)
{	
	CURVEPOINT_DATA tempData;
	memset(&tempData, 0, sizeof(CURVEPOINT_DATA));
	if(selectedDacPointDataNum >= 2)
	{
		for(DWORD i = 0; i < selectedDacPointDataNum - 1; i++)
		{
			for(DWORD j = i + 1; j < selectedDacPointDataNum; j++)
			{
				if(queueToSort[i].serial > queueToSort[j].serial)
				{
					tempData = queueToSort[j];
					queueToSort[j] = queueToSort[i];
					queueToSort[i] = tempData;
				}
			}
		}
	}
}
/*
DWORD insertNewPointBySerial(CURVEPOINT_DATA newPointData,DACPOINT_DATA queueToInsert[MAX_CURVE_POINT_NUM], DWORD dataNum)
{
	if(dataNum >= 2)
	{
		CURVEPOINT_DATA tempData1, tempData2[MAX_CURVE_POINT_NUM];
		memset(&tempData1, 0, sizeof(CURVEPOINT_DATA));
		memset(tempData2, 0, sizeof(tempData2));
		for(int i = 0; i < dataNum - 1; i++)
		{
			if(newPointData.serial < queueToInsert[i].serial)
			{
				tempData1 = queueToInsert[i];				
				for(int j = i; j < dataNum - 1; j++)
				{
					tempData2[j] = queueToInsert[j];
				}
				for(int k = i; j < dataNum - 1; k++)
				{
					queueToInsert[k + 1] = tempData2[k];
				}
				queueToInsert[i] = newPointData;
			}
		}
		dataNum++;
	}
	else if(dataNum == 1)
	{
		if(queueToInsert[dataNum -1].serial > newPointData.serial)
		{
			queueToInsert[dataNum] = queueToInsert[dataNum -1];
			queueToInsert[dataNum -1] = newPointData;
		}
		else
		{
			queueToInsert[dataNum] = newPointData;
		}
		dataNum++;
	}
	else//无任何添加的样点
	{	
		dataNum++;
		queueToInsert[dataNum - 1] = newPointData;
		
	}
	return dataNum;
}*/
DWORD drawShiZiPoint(CURVEPOINT_DATA unDrawPointData, CURVEPOINT_DATA drawPointData,
				   CURVEPOINT_DATA DrawSelectedPointData[MAX_CURVE_POINT_NUM],DWORD selectedPointNum, 
				   BOOL drawWaveFlag, BOOL drawDacPoinFlag)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;
	HDC hdc = GetDC(hWndMain);
	//HDC hMemDC = CreateCompatibleDC(hdc);
	//if(hMemDC == NULL)
	//	MessageBox(NULL, _T("hMemDC == NULL"), _T("error"), MB_OK);
	RECT rect;
	GetClipBox(hdc, &rect);
	POINT preData[DATA_SIZE];
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	// 波形区域宽度、高度
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;
	float yb = nWaveHeight/MAX_HEIGHT;
	float xb = 0;
	long x = 0;
	long y = 0;
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
	if(unDrawPointData.data)//删除上一次选中的点
	{
		if (unDrawPointData.data > MAX_HEIGHT) 
 			unDrawPointData.data = MAX_HEIGHT;
		x = long(RctWave.left+unDrawPointData.serial*xb);
		y = long((nWaveHeight-1)-unDrawPointData.data*yb) + RctWave.top;

		for(int i = (int)(x - 10); i < (int)(x + 10); i++)
		{
			if((i > RctWave.left) && ((i < RctWave.right)))//超过左右波形区不画
			{
				pPixel = pBaseAddress + (int)y*LCD_XSIZE_TFT + i;
				*pPixel = wColorBack;
			}
			if((i > RctWave.left) && ((i < RctWave.right)))//超过左右波形区不画
			{
				if((y - 1) >= MIN_STATUS_HEIGHT)
				{
					pPixel = pBaseAddress + (int)(y - 1)*LCD_XSIZE_TFT + i;
					*pPixel = wColorBack;
				}
			}
			if((i > RctWave.left) && ((i < RctWave.right)))//超过左右波形区不画
			{
				if((y + 1) <= 480- SCALE_WIDTH)
				{
					pPixel = pBaseAddress + (int)(y + 1)*LCD_XSIZE_TFT + i;
					*pPixel = wColorBack;
				}
			}
		}
		for(int j = (int)(y - 10); j < (int)(y + 10); j++)
		{
			if((j > MIN_STATUS_HEIGHT) && (j < 480- SCALE_WIDTH))
			{
				pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)x;
				*pPixel = wColorBack;
			}
			if((j > MIN_STATUS_HEIGHT) && (j < 480- SCALE_WIDTH))
			{
				if((x - 1) >= RctWave.left)
				{
					pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)(x - 1);
					*pPixel = wColorBack;
				}
			}
			if((j > MIN_STATUS_HEIGHT) && (j < 480- SCALE_WIDTH))
			{
				if((x + 1) <= RctWave.right)
				{
					pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)(x + 1);
					*pPixel = wColorBack;
				}
			}
		}
	}
	for(int j = 0; j < 2; j++)//删除波形图
	{
		if(gPicData[j].bDrawed)
		{
			for (int i = 0;i < wNum; i++)
			{
 				if (gPicData[j].data[i] > MAX_HEIGHT) 
 					gPicData[j].data[i] = (WORD)MAX_HEIGHT;
									
				x = long(RctWave.left+i*xb);
				y = long((nWaveHeight-1)-gPicData[j].data[i]*yb) + RctWave.top;
				if(drawWaveFlag == TRUE)
				{
					if(i>0)
					{
						if(j == 0)
							Line(preData[i-1].x, preData[i-1].y, x, y, RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrCh1Wave));
						else
							Line(preData[i-1].x, preData[i-1].y, x, y, RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrCh2Wave));
					}
				}
				else if(drawWaveFlag == FALSE)
				{
					if(i>0)
						Line(preData[i-1].x, preData[i-1].y, x, y, wColorBack);
				}
				preData[i].x = x;
				preData[i].y = y;
			}
		}
	}
	if(selectedPointNum)
	{
		for (DWORD k = 0;k < selectedPointNum; k++)
		{
 			if (DrawSelectedPointData[k].data > MAX_HEIGHT) 
 				DrawSelectedPointData[k].data = MAX_HEIGHT;
						
			x = long(RctWave.left+DrawSelectedPointData[k].serial*xb);
			y = long((nWaveHeight-1)-DrawSelectedPointData[k].data*yb) + RctWave.top;
			if(drawDacPoinFlag == TRUE)//画所有选择的DAC点
			{
				for(int i = (int)(x - 10); i < (int)(x + 10); i++)
				{
					if((i > RctWave.left) && ((i < RctWave.right)))//超过左右波形区不画
					{
						pPixel = pBaseAddress + (int)y*LCD_XSIZE_TFT + i;
						*pPixel = RED_POINT;
					}
					if((i > RctWave.left) && ((i < RctWave.right)))//超过左右波形区不画
					{
						if((y - 1) >= MIN_STATUS_HEIGHT)
						{
							pPixel = pBaseAddress + (int)(y - 1)*LCD_XSIZE_TFT + i;
							*pPixel = RED_POINT;
						}
					}
					if((i > RctWave.left) && ((i < RctWave.right)))//超过左右波形区不画
					{
						if((y + 1) <= 480- SCALE_WIDTH)
						{
							pPixel = pBaseAddress + (int)(y + 1)*LCD_XSIZE_TFT + i;
							*pPixel = RED_POINT;
						}
					}
				}
				for(int j = (int)(y - 10); j < (int)(y + 10); j++)
				{
					if((j > MIN_STATUS_HEIGHT) && (j < 480- SCALE_WIDTH))
					{
						pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)x;
						*pPixel = RED_POINT;
					}
					if((j > MIN_STATUS_HEIGHT) && (j < 480- SCALE_WIDTH))
					{
						if((x - 1) >= RctWave.left)
						{
							pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)(x - 1);
							*pPixel = RED_POINT;
						}
					}
					if((j > MIN_STATUS_HEIGHT) && (j < 480- SCALE_WIDTH))
					{
						if((x + 1) <= RctWave.right)
						{
							pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)(x + 1);
							*pPixel = RED_POINT;
						}
					}
				}
			}
			else//不画所有选择的DAC点
			{
				for(int i = (int)(x - 10); i < (int)(x + 10); i++)
				{
					if((i > RctWave.left) && ((i < RctWave.right)))//超过左右波形区不画
					{
						pPixel = pBaseAddress + (int)y*LCD_XSIZE_TFT + i;
						*pPixel = wColorBack;
					}
					if((i > RctWave.left) && ((i < RctWave.right)))//超过左右波形区不画
					{
						if((y - 1) >= MIN_STATUS_HEIGHT)
						{
							pPixel = pBaseAddress + (int)(y - 1)*LCD_XSIZE_TFT + i;
							*pPixel = wColorBack;
						}
					}
					if((i > RctWave.left) && ((i < RctWave.right)))//超过左右波形区不画
					{
						if((y + 1) <= 480- SCALE_WIDTH)
						{
							pPixel = pBaseAddress + (int)(y + 1)*LCD_XSIZE_TFT + i;
							*pPixel = wColorBack;
						}
					}
				}
				for(int j = (int)(y - 10); j < (int)(y + 10); j++)
				{
					if((j > MIN_STATUS_HEIGHT) && (j < 480- SCALE_WIDTH))
					{
						pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)x;
						*pPixel = wColorBack;
					}
					if((j > MIN_STATUS_HEIGHT) && (j < 480- SCALE_WIDTH))
					{
						if((x - 1) >= RctWave.left)
						{
							pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)(x - 1);
							*pPixel = wColorBack;
						}
					}
					if((j > MIN_STATUS_HEIGHT) && (j < 480- SCALE_WIDTH))
					{
						if((x + 1) <= RctWave.right)
						{
							pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)(x + 1);
							*pPixel = wColorBack;
						}
					}
				}
			}
		}
	}
	if(drawPointData.data)
	{
		if (drawPointData.data > MAX_HEIGHT) 
 			drawPointData.data = MAX_HEIGHT;
		x = long(RctWave.left+drawPointData.serial*xb);
		y = long((nWaveHeight-1)-drawPointData.data*yb) + RctWave.top;
		for(int i = (int)(x - 10); i < (int)(x + 10); i++)
		{
			if((i > RctWave.left) && ((i < RctWave.right)))//超过左右波形区不画
			{
				pPixel = pBaseAddress + (int)y*LCD_XSIZE_TFT + i;
				*pPixel = BLUE_POINT;
			}
			if((i > RctWave.left) && ((i < RctWave.right)))//超过左右波形区不画
			{
				if((y - 1) >= MIN_STATUS_HEIGHT)
				{
					pPixel = pBaseAddress + (int)(y - 1)*LCD_XSIZE_TFT + i;
					*pPixel = BLUE_POINT;
				}
			}
			if((i > RctWave.left) && ((i < RctWave.right)))//超过左右波形区不画
			{
				if((y + 1) <= 480- SCALE_WIDTH)
				{
					pPixel = pBaseAddress + (int)(y + 1)*LCD_XSIZE_TFT + i;
					*pPixel = BLUE_POINT;
				}
			}
		}
		for(int j = (int)(y - 10); j < (int)(y + 10); j++)
		{
			if((j > MIN_STATUS_HEIGHT) && (j < 480- SCALE_WIDTH))
			{
				pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)x;
				*pPixel = BLUE_POINT;
			}
			if((j > MIN_STATUS_HEIGHT) && (j < 480- SCALE_WIDTH))
			{
				if((x - 1) >= RctWave.left)
				{
					pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)(x - 1);
					*pPixel = BLUE_POINT;
				}
			}
			if((j > MIN_STATUS_HEIGHT) && (j < 480- SCALE_WIDTH))
			{
				if((x + 1) <= RctWave.right)
				{
					pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)(x + 1);
					*pPixel = BLUE_POINT;
				}
			}
		}
	}
	ReleaseDC(hWndMain,hdc);
	*/
	return 0;
}
void write2GateSram(CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dataNum,WORD channelNumber,WORD dgsNumber)
{
	CURVEPOINT_DATA tempPointData1;
	CURVEPOINT_DATA tempPointData2;
	memset(&tempPointData1, 0, sizeof(CURVEPOINT_DATA));
	memset(&tempPointData1, 0, sizeof(CURVEPOINT_DATA));
	
	float soundDistance = 0.0f;//两个曲线点之间的声程
	float firstSoundDistance = 0.0f;		//闸门前沿
	float lastSoundDistance = 0.0f;		//闸门后沿

	WORD y = 0;

	DWORD smpleNum = 0;//两个曲线点之间的声程的采样点数
	WORD* pData = NULL;

	WORD wCount = 0; //采样点超过4096个点，超过4096个点不写进去 SRAM只有4096 WORD
	switch(dgsNumber)
	{
		case 0:
			pData = chCurrent_gate1curve;
			break;
		case 1:
			pData = chCurrent_gate2curve;
			break;
		case 2:
			pData = chCurrent_gate3curve;
			break;
		default:
			break;
	}
	if(dataNum < 2)
		return;
	else
	{
		for(DWORD i = 0; i < dataNum - 1; i++)
		{
			tempPointData1 = pointData[i];
			tempPointData2 = pointData[i + 1];

			soundDistance = (fRangeEnd[channelNumber] - fRangeStart[channelNumber])*(pointData[i + 1].serial - pointData[i].serial)/DATA_SIZE;
			//smpleNum = (soundDistance * 2 / 1000) * HEAD_FREQUENCE_50M / DisplaySet_ad[channelNumber].nWaveSpeed ;
			smpleNum = Range2Dot(DisplaySet_ad[channelNumber].nWaveSpeed, soundDistance);

			//test
				TCHAR strText[20];
				memset(strText, 0, sizeof(strText));
				swprintf(strText, _T("%d"),(DWORD)smpleNum);

				DrawLableText(hBtnCreateDgsCurvePingDing, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);

				memset(strText, 0, sizeof(strText));
				
				swprintf(strText, _T("%d"),tempPointData2.data);
				
				DrawLableText(hBtnCreateDgsCurvePanHui, strText, 20, STATIC_TEXT_COLOR, LABEL_START_X, LABEL_5_START_Y);
			//test
			if(i == 0)
			{
				for(DWORD j = 0; j <= smpleNum; j++)
				{
					y = WORD(tempPointData1.data + ((float)tempPointData2.data - (float)tempPointData1.data) * j / smpleNum);

					if(wCount < 4096)
					{
						*pData = y;					
						pData++;
					}	
					wCount++;
				}
			}
			else
			{
				for(DWORD j = 1; j <= smpleNum; j++)
				{
					y = WORD(tempPointData1.data + ((float)tempPointData2.data - (float)tempPointData1.data) * j / smpleNum);
						
					if(wCount < 4096)
					{
						*pData = y;					
						pData++;
					}	
					wCount++;
				}
			}
			
		}
		firstSoundDistance = fRangeStart[channelNumber] + (fRangeEnd[channelNumber] - fRangeStart[channelNumber])*(pointData[0].serial/DATA_SIZE);
		
		lastSoundDistance = fRangeStart[channelNumber] + (fRangeEnd[channelNumber] - fRangeStart[channelNumber])*(pointData[dataNum - 1].serial/DATA_SIZE);		
		
		switch(dgsNumber)
		{
			case 0:
				chCurrent_gate_reg ->wGate1Alarm = 0;
				chCurrent_gate_reg->wGate1Front = (WORD)Range2Dot(DisplaySet_ad[channelNumber].nWaveSpeed, firstSoundDistance);
				chCurrent_gate_reg->wGate1Rear = (WORD)Range2Dot(DisplaySet_ad[channelNumber].nWaveSpeed, lastSoundDistance);
				break;
			case 1:
				chCurrent_gate_reg ->wGate2Alarm = 0;
				chCurrent_gate_reg->wGate2Front = (WORD)Range2Dot(DisplaySet_ad[channelNumber].nWaveSpeed, firstSoundDistance);
				chCurrent_gate_reg->wGate2Rear = (WORD)Range2Dot(DisplaySet_ad[channelNumber].nWaveSpeed, lastSoundDistance);
				break;
			case 2:
				chCurrent_gate_reg ->wGate3Alarm = 0;
				chCurrent_gate_reg->wGate3Front = (WORD)Range2Dot(DisplaySet_ad[channelNumber].nWaveSpeed, firstSoundDistance);
				chCurrent_gate_reg->wGate3Rear = (WORD)Range2Dot(DisplaySet_ad[channelNumber].nWaveSpeed, lastSoundDistance);
				break;
			default:
				break;
		}
		GateInfos[channelNumber][dgsNumber].alarmMode = (alarm_modes)0; //报警提示根据GateInfos里的变量来判断
	}
}
//有关画DAC、DGS曲线的函数

void drawShiZhi(POINT point, WORD wColor, BOOL bDrawFlag, RECT RctWave)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;
	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	
	for(int i = (int)(point.x - 10); i < (int)(point.x + 10); i++)
	{
		if(i > RctWave.left && i < RctWave.right)//超过左右波形区不画
		{
			if(point.y > RctWave.top && point.y < RctWave.bottom)
			{
				pPixel = pBaseAddress + (int)point.y*LCD_XSIZE_TFT + i;
				if(bDrawFlag ==TRUE)
					*pPixel = wColor;
				else
					*pPixel = wColorBack;
			}
		}
		if((i > RctWave.left) && (i < RctWave.right))//超过左右波形区不画
		{
			if((point.y - 1) > RctWave.top  && (point.y - 1) < RctWave.bottom)
			{
				pPixel = pBaseAddress + (int)(point.y - 1)*LCD_XSIZE_TFT + i;
				if(bDrawFlag ==TRUE)
					*pPixel = wColor;
				else
					*pPixel = wColorBack;
			}
		}
		if((i > RctWave.left) && (i < RctWave.right))//超过左右波形区不画
		{
			if((point.y + 1) > RctWave.top && (point.y + 1) < RctWave.bottom)
			{
				pPixel = pBaseAddress + (int)(point.y + 1)*LCD_XSIZE_TFT + i;
				if(bDrawFlag ==TRUE)
					*pPixel = wColor;
				else
					*pPixel = wColorBack;
			}
		}
	}
	for(int j = (int)(point.y - 10); j < (int)(point.y + 10); j++)
	{
		if((j > RctWave.top) && (j < RctWave.bottom))
		{
			if(point.x > RctWave.left && point.x < RctWave.right)
			{
				pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)point.x;
				if(bDrawFlag ==TRUE)
					*pPixel = wColor;
				else
					*pPixel = wColorBack;
			}
		}
		if((j > RctWave.top) && (j < RctWave.bottom))
		{
			if((point.x - 1) > RctWave.left && (point.x - 1) < RctWave.right)
			{
				pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)(point.x - 1);
				if(bDrawFlag ==TRUE)
					*pPixel = wColor;
				else
					*pPixel = wColorBack;
			}
		}
		if((j > RctWave.top) && (j < RctWave.bottom))
		{
			if((point.x + 1) > RctWave.left && (point.x + 1) < RctWave.right)
			{
				pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)(point.x + 1);
				if(bDrawFlag ==TRUE)
					*pPixel = wColor;
				else
					*pPixel = wColorBack;
			}
		}
	}
	*/
}


void DisplayClear(BOOL bMenuShown)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;
	
	HDC hDC = GetDC(hWndMain);

	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	
	RECT rect;	// 主窗口显示出来的矩形
	GetClipBox(hDC, &rect); 
	RECT RctWave;	// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
//	RECT RctYScale; // Y坐标区域
//	RECT RctXScale; // X坐标区域
//	RECT RctRight;  // 右侧空白区

//	GetDisplayRect(rect, bMenuShown, RctWave, RctXScale, RctYScale, RctRight);

	Sleep(200);

	for(int i = RctWave.left; i < RctWave.right; i++)
	{
		for(int j = RctWave.top; j < RctWave.bottom; j++)
		{
			pPixel = pBaseAddress + j*LCD_XSIZE_TFT + i;
			*pPixel = wColorBack;
		}
	}
	*/
}
//关闭所有闸门
void closeAllGate()
{
	ch1_gate_reg->wGate1Enable = 0;
	ch1_gate_reg->wGate2Enable = 0;
	ch1_gate_reg->wGate3Enable = 0;
	ch2_gate_reg->wGate1Enable = 0;
	ch2_gate_reg->wGate2Enable = 0;
	ch2_gate_reg->wGate3Enable = 0;

	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			GateInfos[i][j].bOpen = OFF;
		}
	}

	SetRedrawFlag(TRUE);
}
/*
BOOL saveDgsFile(LPCTSTR lpszPathName)
{
	DGS_CURVE dgsCurve;
	memset(&dgsCurve, 0 , sizeof(DGS_CURVE));
	
	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		for(int j = 0; j < GATE_NUM; j++)
		{
			dgsCurve.bSwitch[i][j] = gDgsSwitch[i][j];
		}
	}

	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		dgsCurve.fOldRang[i] = gfOldDacRange[i];
	}


	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		for(int j = 0; j < gSelectedDgsPointDataNum[i][1]; j++)
		{
			dgsCurve.wOldSerial[i][j] = gwOldDacSerial[i][j];
		}
	}

	
	dgsCurve.selectedDgsPointDataNum[0][0] = gSelectedDgsPointDataNum[0][0];
	dgsCurve.selectedDgsPointDataNum[0][1] = gSelectedDgsPointDataNum[0][1];
	dgsCurve.selectedDgsPointDataNum[0][2] = gSelectedDgsPointDataNum[0][2];
	dgsCurve.selectedDgsPointDataNum[0][3] = gSelectedDgsPointDataNum[0][3];

	dgsCurve.selectedDgsPointDataNum[1][0] = gSelectedDgsPointDataNum[1][0];
	dgsCurve.selectedDgsPointDataNum[1][1] = gSelectedDgsPointDataNum[1][1];
	dgsCurve.selectedDgsPointDataNum[1][2] = gSelectedDgsPointDataNum[1][2];
	dgsCurve.selectedDgsPointDataNum[1][3] = gSelectedDgsPointDataNum[1][3];
	
	memcpy(dgsCurve.selectedDgsPointData[0][0], gSelectedDgsPointData[0][0], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	memcpy(dgsCurve.selectedDgsPointData[0][1], gSelectedDgsPointData[0][1], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	memcpy(dgsCurve.selectedDgsPointData[0][2], gSelectedDgsPointData[0][2], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	memcpy(dgsCurve.selectedDgsPointData[0][3], gSelectedDgsPointData[0][3], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);

	memcpy(dgsCurve.selectedDgsPointData[1][0], gSelectedDgsPointData[1][0], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	memcpy(dgsCurve.selectedDgsPointData[1][1], gSelectedDgsPointData[1][1], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	memcpy(dgsCurve.selectedDgsPointData[1][2], gSelectedDgsPointData[1][2], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	memcpy(dgsCurve.selectedDgsPointData[1][3], gSelectedDgsPointData[1][3], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	
	HANDLE hFileWrite;	//文件句柄
	DWORD dwWriteDataSize;
	hFileWrite = CreateFile(lpszPathName,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("创建DGS文件失败"));
		return -1;
	}


	if(!WriteFile(hFileWrite,&dgsCurve, sizeof(DGS_CURVE),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写DGS文件失败"));

		CloseHandle(hFileWrite);
		return FALSE;
	}

	CloseHandle(hFileWrite);

//	PostMessage(hStatusBar, SB_SETTEXT, 
//		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("文件保存成功"));

	return TRUE;	
}

BOOL readDgsFile(LPCTSTR lpszPathName)
{
	DGS_CURVE dgsCurve;					
	memset(&dgsCurve, 0 , sizeof(DGS_CURVE));
	
	HANDLE hFileWrite;	//文件句柄
	DWORD dwWriteDataSize;
	hFileWrite = CreateFile(lpszPathName,
							GENERIC_READ,
							FILE_SHARE_READ,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("打开DGS文件失败"));
		return FALSE;
	}

	if(!ReadFile(hFileWrite,&dgsCurve, sizeof(DGS_CURVE),&dwWriteDataSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("读DGS文件失败"));
		
		CloseHandle(hFileWrite);
		return FALSE;
	}

	CloseHandle(hFileWrite);

	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		for(int j = 0; j < GATE_NUM; j++)
		{
			gDgsSwitch[i][j] = dgsCurve.bSwitch[i][j];
		}
	}

	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		gfOldDacRange[i] = dgsCurve.fOldRang[i];
	}

	for(int i = 0; i < CHANNEL_NUM; i++)
	{
		for(int j = 0; j < dgsCurve.selectedDgsPointDataNum[i][1]; j++)
		{
			gwOldDacSerial[i][j] = dgsCurve.wOldSerial[i][j];
		}
	}

	gSelectedDgsPointDataNum[0][0] = dgsCurve.selectedDgsPointDataNum[0][0]; 
	gSelectedDgsPointDataNum[0][1] = dgsCurve.selectedDgsPointDataNum[0][1];
	gSelectedDgsPointDataNum[0][2] = dgsCurve.selectedDgsPointDataNum[0][2]; 
	gSelectedDgsPointDataNum[0][3] = dgsCurve.selectedDgsPointDataNum[0][3];

	gSelectedDgsPointDataNum[1][0] = dgsCurve.selectedDgsPointDataNum[1][0]; 
	gSelectedDgsPointDataNum[1][1] = dgsCurve.selectedDgsPointDataNum[1][1];
	gSelectedDgsPointDataNum[1][2] = dgsCurve.selectedDgsPointDataNum[1][2]; 
	gSelectedDgsPointDataNum[1][3] = dgsCurve.selectedDgsPointDataNum[1][3];
	
	memcpy(gSelectedDgsPointData[0][0], dgsCurve.selectedDgsPointData[0][0], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	memcpy(gSelectedDgsPointData[0][1], dgsCurve.selectedDgsPointData[0][1], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	memcpy(gSelectedDgsPointData[0][2], dgsCurve.selectedDgsPointData[0][2], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	memcpy(gSelectedDgsPointData[0][3], dgsCurve.selectedDgsPointData[0][3], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);

	memcpy(gSelectedDgsPointData[1][0], dgsCurve.selectedDgsPointData[1][0], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	memcpy(gSelectedDgsPointData[1][1], dgsCurve.selectedDgsPointData[1][1], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	memcpy(gSelectedDgsPointData[1][2], dgsCurve.selectedDgsPointData[1][2], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);
	memcpy(gSelectedDgsPointData[1][3], dgsCurve.selectedDgsPointData[1][3], sizeof(CURVEPOINT_DATA)*MAX_CURVE_POINT_NUM);

	return TRUE;
}
*/
BOOL ReadWaveFile(LPCTSTR szFilePath,LPVOID lpFileDataBuffer)
{
	HANDLE hFileRead = NULL;
	DWORD dwReadSize = 0;
	hFileRead = CreateFile(szFilePath,
								GENERIC_READ,
								FILE_SHARE_READ,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	if(hFileRead == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("打开wave文件失败"));
		return FALSE;
	}
	if(!ReadFile(hFileRead,lpFileDataBuffer,1024,&dwReadSize,NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("读wave文件失败"));

		CloseHandle(hFileRead);
		return FALSE;
	}
	CloseHandle(hFileRead);
	return TRUE;
}
void drawWaveFromWaveFile(LPCTSTR szFilePath)
{
	HDC hMemDC = GetDC(hWndMain);
	WORD buffer[512];
	RECT rect;
	RECT RctWave;
	GetClipBox(hMemDC, &rect);
	HPEN hLine1Pen = CreatePen(PS_SOLID, 1, RGB(255,0,0));
	HPEN hOldPen = NULL;

	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;

	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;
	float yb = nWaveHeight/MAX_HEIGHT;
	float xb = nWaveWidth/512.0f;
	
	if(ReadWaveFile(szFilePath, buffer))
	{

		hOldPen = (HPEN)SelectObject(hMemDC, hLine1Pen);
		MoveToEx(hMemDC, RctWave.left, int((nWaveHeight-1) - buffer[0]*yb), NULL);
		for (int i = 0;i < DATA_SIZE; i++)
		{
			if (buffer[i] > MAX_HEIGHT) 
 				buffer[i] = (WORD)MAX_HEIGHT;
			LineTo(hMemDC, int(RctWave.left+i*xb), int((nWaveHeight-1)-buffer[i]*yb));
		}				
	}
	
	SelectObject(hMemDC, hOldPen);
	DeleteObject(hLine1Pen);
	ReleaseDC(hWndMain,hMemDC);
}
void clearWave()
{
	/*
	WORD wColorBack = WORD(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;
	HDC hdc = GetDC(hWndMain);
	//HDC hMemDC = CreateCompatibleDC(hdc);
	//if(hMemDC == NULL)
	//	MessageBox(NULL, _T("hMemDC == NULL"), _T("error"), MB_OK);
	RECT rect;
	GetClipBox(hdc, &rect);
	POINT preData[DATA_SIZE];
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	// 波形区域宽度、高度
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;
	float yb = nWaveHeight/MAX_HEIGHT;
	float xb = nWaveWidth/512.0f;
	long x = 0;
	long y = 0;
	for(int j = 0; j < 2; j++)//擦掉波形图
	{
		if(gPicData[j].bDrawed)
		{
			for (int i = 0;i < DATA_SIZE; i++)
			{
 				if (gPicData[j].data[i] > MAX_HEIGHT) 
 					gPicData[j].data[i] = (WORD)MAX_HEIGHT;
									
				x = long(RctWave.left+i*xb);
				y = long((nWaveHeight-1)-gPicData[j].data[i]*yb) + RctWave.top;

				if(i>0)
					Line(preData[i-1].x, preData[i-1].y, x, y, wColorBack);
				preData[i].x = x;
				preData[i].y = y;
			}
		}
	}
	ReleaseDC(hWndMain,hdc);
	*/
}
/*
BOOL saveWaveFile(LPCTSTR lpszFilePath, WORD wWaveData[DATA_SIZE])
{
	HANDLE hFileWrite;	//文件句柄
	DWORD dwWriteDataSize;
	hFileWrite = CreateFile(lpszFilePath,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("创建wave文件失败"));
		return FALSE;
	}
	if(!WriteFile(hFileWrite,wWaveData, sizeof(WORD) * DATA_SIZE, &dwWriteDataSize, NULL))
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写wave文件失败"));

		CloseHandle(hFileWrite);
		return FALSE;
	}

	CloseHandle(hFileWrite);
	return TRUE;

}
*/
//txt 0-1024 waveData
BOOL saveWaveFile(LPCTSTR lpszFilePath, WORD wWaveData[DATA_SIZE])
{
	HANDLE hFileWrite = NULL;	//文件句柄
	DWORD dwWriteDataSize = 0;
	hFileWrite = CreateFile(lpszFilePath,
							GENERIC_READ|GENERIC_WRITE,
							0,
							NULL,
							CREATE_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if(hFileWrite == INVALID_HANDLE_VALUE)
	{
		PostMessage(hStatusBar, SB_SETTEXT, 
		(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("创建wave文件失败"));
		return FALSE;
	}
	for(int i = 0; i < DATA_SIZE; i++)
	{
		char buffer1[20];
		memset(buffer1, 0, sizeof(buffer1));

		_itoa(wWaveData[i], buffer1, 10);
		buffer1[18] = '\r';
		buffer1[19] = '\n';

		if(!WriteFile(hFileWrite,buffer1, 20, &dwWriteDataSize, NULL))
		{
			PostMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)_T("写wave文件失败"));

			CloseHandle(hFileWrite);
			return FALSE;
		}
	}

	CloseHandle(hFileWrite);
	return TRUE;

}

void FreezeUp(HWND hWnd, const BOOL& bOpen)
{
	InitializeCriticalSection(&csCommand);
	EnterCriticalSection(&csCommand);
	if(bOpen)
	{
		InitializeCriticalSection(&csCommand);
		EnterCriticalSection(&csCommand);
		thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_STOP;
		LeaveCriticalSection(&csCommand);
		DeleteCriticalSection(&csCommand);
		Sleep(100);

		// 查询所有波峰点
		nAddStep = 0;
		int j = 0;
		
		if(DisplaySet_ad[CurrentChannelNo].fRange < gfMinRange[CurrentChannelNo])
		{
			WORD wNum = 0;
			wNum = WORD((DisplaySet_ad[CurrentChannelNo].fRange/gfMinRange[CurrentChannelNo])*DATA_SIZE);

			for(int i = 0; i < wNum; i++)
			{
				if(i == 0)
				{
					if(gPicData[CurrentChannelNo].data[i] > gPicData[CurrentChannelNo].data[i + 1])
					{
						gGetAllPeakPointData[j].data = gPicData[CurrentChannelNo].data[i];
						gGetAllPeakPointData[j].serial = (float)i;
						j++;
					}
				}
				else if(0 < i && i< DATA_SIZE - 1)
				{
					if((gPicData[CurrentChannelNo].data[i] >= gPicData[CurrentChannelNo].data[i - 1]) && 
					   (gPicData[CurrentChannelNo].data[i] > gPicData[CurrentChannelNo].data[i + 1]))
					{
						gGetAllPeakPointData[j].data = gPicData[CurrentChannelNo].data[i];
						gGetAllPeakPointData[j].serial = (float)i;
						j++;
					}
				}
				else
				{
					if(gPicData[CurrentChannelNo].data[i] > gPicData[CurrentChannelNo].data[i - 1])
					{
						gGetAllPeakPointData[j].data = gPicData[CurrentChannelNo].data[i];
						gGetAllPeakPointData[j].serial = (float)i;
						j++;
					}
				}
			}
		}
		else
		{
			for(int i = 0; i < DATA_SIZE; i++)
			{
				if(i == 0)
				{
					if(gPicData[CurrentChannelNo].data[i] > gPicData[CurrentChannelNo].data[i + 1])
					{
						gGetAllPeakPointData[j].data = gPicData[CurrentChannelNo].data[i];
						gGetAllPeakPointData[j].serial = (float)i;
						j++;
					}
				}
				else if(0 < i && i< DATA_SIZE - 1)
				{
					if((gPicData[CurrentChannelNo].data[i] >= gPicData[CurrentChannelNo].data[i - 1]) && 
					   (gPicData[CurrentChannelNo].data[i] > gPicData[CurrentChannelNo].data[i + 1]))
					{
						gGetAllPeakPointData[j].data = gPicData[CurrentChannelNo].data[i];
						gGetAllPeakPointData[j].serial = (float)i;
						j++;
					}
				}
				else
				{
					if(gPicData[CurrentChannelNo].data[i] > gPicData[CurrentChannelNo].data[i - 1])
					{
						gGetAllPeakPointData[j].data = gPicData[CurrentChannelNo].data[i];
						gGetAllPeakPointData[j].serial = (float)i;
						j++;
					}
				}
			}
		}
		// 在第一个波峰点画十字
		nAllPeakPointDataNum = j;
		gCurrentPeakPointData = gGetAllPeakPointData[nAddStep];
		
		drawShiZiPoint(gPrePeakPointData, gCurrentPeakPointData, NULL, 0, TRUE, FALSE);

		DrawPeakInfo();
	}
	else
	{	
		InitializeCriticalSection(&csCommand);
		EnterCriticalSection(&csCommand);
		thread_param.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_START;
		LeaveCriticalSection(&csCommand);
		DeleteCriticalSection(&csCommand);

		// 重画
		SetRedrawFlag(TRUE);

		// 清空状态栏
		memset(gszTextPeak, 0, sizeof(gszTextPeak));

		PostMessage(hStatusBar, SB_SETTEXT, 
							(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	}
	LeaveCriticalSection(&csCommand);
	DeleteCriticalSection(&csCommand);
}

void DrawPeakInfo()
{
	memset(gszTextPeak, 0, sizeof(gszTextPeak));	

	InitializeCriticalSection(&csRange);
	EnterCriticalSection(&csRange);
	swprintf(gszTextPeak, _T("%.2f,%d"), fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo])*gCurrentPeakPointData.serial/DATA_SIZE, 
		(int)(gCurrentPeakPointData.data * 100 / MAX_HEIGHT));
	LeaveCriticalSection(&csRange);
	DeleteCriticalSection(&csRange);

	PostMessage(hStatusBar, SB_SETTEXT, 
						(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
}

void FreezeLeftMove()
{
	gPrePeakPointData = gGetAllPeakPointData[nAddStep];
					
	if(nAddStep == 0)
	{
		gPrePeakPointData = gGetAllPeakPointData[0];
		nAddStep = nAllPeakPointDataNum - 1;
	}
	else
		nAddStep--;
	
	gCurrentPeakPointData = gGetAllPeakPointData[nAddStep];
	drawShiZiPoint(gPrePeakPointData, gCurrentPeakPointData, NULL,0, TRUE, FALSE);
}

void FreezeRightMove()
{
	gPrePeakPointData = gGetAllPeakPointData[nAddStep];
	if(nAddStep == nAllPeakPointDataNum - 1)
	{
		gPrePeakPointData = gGetAllPeakPointData[nAllPeakPointDataNum - 1];
		nAddStep = 0;
	}
	else
		nAddStep++;

	gCurrentPeakPointData = gGetAllPeakPointData[nAddStep];

	drawShiZiPoint(gPrePeakPointData, gCurrentPeakPointData, NULL, 0, TRUE, FALSE);
}
/**************************
*第一个回波声程
* @return   fDistance : 声程
* @since    1.00
***************************/
float firstHuiBoDistance()
{
	int i = 21;
	int nSerial = 0;
	WORD nCurrenData = 0;
	WORD wNum = DATA_SIZE;
	float fDistance = 0;

	if(DisplaySet_ad[CurrentChannelNo].fRange < gfMinRange[CurrentChannelNo])//小于最小声程
	{
		wNum = WORD((DisplaySet_ad[CurrentChannelNo].fRange / gfMinRange[CurrentChannelNo])*DATA_SIZE);
	}
	while(gPicData[CurrentChannelNo].data[i] > 50)
	{
		i++;
	}
	
	for(int j = i + 5; j < DATA_SIZE - 1; j++)
	{
		if(nCurrenData < gPicData[CurrentChannelNo].data[j])
		{
			nCurrenData = gPicData[CurrentChannelNo].data[j];
			nSerial = j;
		}
	}
	fDistance = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * nSerial / wNum;

	return fDistance;
}

/**************************
*第二个回波声程
* @return   fDistance : 声程
* @since    1.00
***************************/
float secondHuiBoDistance()
{
	int i = 21;
	int nSerial = 0;
	WORD nCurrenData = 0;
	WORD wNum = DATA_SIZE;
	float fDistance = 0;

	if(DisplaySet_ad[CurrentChannelNo].fRange < gfMinRange[CurrentChannelNo])//小于最小声程
	{
		wNum = WORD((DisplaySet_ad[CurrentChannelNo].fRange / gfMinRange[CurrentChannelNo])*DATA_SIZE);
	}
	while(gPicData[CurrentChannelNo].data[i] > 50)
	{
		i++;
	}
	//第一个回波最高点
	for(int k = i + 5; k < DATA_SIZE - 1; k++)
	{
		if(nCurrenData < gPicData[CurrentChannelNo].data[k])
		{
			nCurrenData = gPicData[CurrentChannelNo].data[k];
			nSerial = k;
		}
	}
	nCurrenData = 0;
	//第一个回波之后的点
	while(gPicData[CurrentChannelNo].data[nSerial] > 150)
	{
		nSerial++;
	}
	//第二个回波最高点
	for(int j = nSerial + 10; j < DATA_SIZE - 1; j++)
	{
		if(nCurrenData < gPicData[CurrentChannelNo].data[j])
		{
			nCurrenData = gPicData[CurrentChannelNo].data[j];
			nSerial = j;
		}
	}
	fDistance = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * nSerial / wNum;

	return fDistance;
}

float zeroAdjust(float fThickness)
{
	float fAmend = 0;
	float fTime = 2 * fThickness / (DisplaySet_ad[CurrentChannelNo].nWaveSpeed * 1000 ) *1000000;

	InitializeCriticalSection(&csPeakInfo);
	EnterCriticalSection(&csPeakInfo);
	
    fAmend = SendParam_ad[CurrentChannelNo].nAmend + (float)(gPeakInfos[CurrentChannelNo][0].wDefectTime / float(ADFREQUENCY) - fTime);

	LeaveCriticalSection(&csPeakInfo);
	DeleteCriticalSection(&csPeakInfo);
	if(fAmend < 0)
		fAmend = 0;

	// 显示校准完成
	MessageBeep(0xFFFFFFFF);

	memset(gszTextPeak, 0, sizeof(gszTextPeak));

	swprintf(gszTextPeak, _T("零点校准完成"));

	SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	
	return fAmend;
}
WORD waveSpeedAdjust(float fThickness)
{
	WORD nWaveSpeed = 1;

	InitializeCriticalSection(&csPeakInfo);
	EnterCriticalSection(&csPeakInfo);
	
	nWaveSpeed = 
		WORD( fThickness * ((ADFREQUENCY*1000000) / 1000) / ((gPeakInfos[CurrentChannelNo][1].wDefectTime - gPeakInfos[CurrentChannelNo][0].wDefectTime)/2.0) );

	LeaveCriticalSection(&csPeakInfo);
	DeleteCriticalSection(&csPeakInfo);
	if(nWaveSpeed > 0)
	{
		
	}
	else
	{
		memset(gszTextPeak, 0, sizeof(gszTextPeak));
			swprintf(gszTextPeak, _T("校准失败"));

		SendMessage(hStatusBar, SB_SETTEXT, 
					(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
		
		return nWaveSpeed;
	}

	// 显示校准完成
	MessageBeep(0xFFFFFFFF);

	memset(gszTextPeak, 0, sizeof(gszTextPeak));

	swprintf(gszTextPeak, _T("声速校准完成"));

	SendMessage(hStatusBar, SB_SETTEXT, 
				(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)gszTextPeak);
	return nWaveSpeed;
}

POINT headVDataToPoint(HEADVPOINT_DATA stHeadVPoint_data)
{
	POINT headV_point;
	memset(&headV_point, 0 ,sizeof(POINT));
	HDC hdc = GetDC(hWndMain);

	RECT rect;
	GetClipBox(hdc, &rect);

	int nWidth = rect.right - rect.left;
	int nHeight = rect.bottom;
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	// 波形区域宽度、高度
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;
	
	long x,y;
	x = long(RctWave.left + stHeadVPoint_data.fActualHouDu / 100 * nWaveWidth);
	y = long((nWaveHeight-1) - stHeadVPoint_data.fMeasureHouDu/ 100 * nWaveHeight) + RctWave.top;

	headV_point.x = (DWORD)x;
	headV_point.y = (DWORD)y;
	ReleaseDC(hWndMain,hdc);
	return headV_point;
}

void sortSelectedPointDataByHoudu(HEADVPOINT_DATA queueToSort[], DWORD selectedPointNum)
{	
	HEADVPOINT_DATA tempData;
	memset(&tempData, 0, sizeof(HEADVPOINT_DATA));
	if(selectedPointNum >= 2)
	{
		for(DWORD i = 0; i < selectedPointNum - 1; i++)
		{
			for(DWORD j = i + 1; j < selectedPointNum; j++)
			{
				if(queueToSort[i].fActualHouDu> queueToSort[j].fActualHouDu)
				{
					tempData = queueToSort[j];
					queueToSort[j] = queueToSort[i];
					queueToSort[i] = tempData;
				}
			}
		}
	}
}

void drawCurve1(POINT selectPoint[], DWORD dwPointNum, BOOL bDrawFlag)
{
	WORD wColor = RED_POINT;
	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	
	if(dwPointNum < 2)
		return;
	else
	{
		for(DWORD i = 0; i < dwPointNum - 1; i++)
		{
			if(bDrawFlag == TRUE)
				drawLine(selectPoint[i], selectPoint[i + 1], wColor);
			else
				drawLine(selectPoint[i], selectPoint[i + 1], wColorBack);
		}
	}
}
void drawDashCurve1(POINT selectPoint[], DWORD dwPointNum, BOOL bDrawFlag)
{
	WORD wColor = RED_POINT;
	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	
	if(dwPointNum < 2)
		return;
	else
	{
		for(DWORD i = 0; i < dwPointNum - 1; i++)
		{
			if(bDrawFlag == TRUE)
				drawDashLine(selectPoint[i], selectPoint[i + 1], wColor);
			else
				drawDashLine(selectPoint[i], selectPoint[i + 1], wColorBack);
		}
	}
}

void drawAfaCurve(RECT RctWave, CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dwPointNum, WORD wColorBack, WORD wColor, BOOL bDrawFlag)
{

	/*
	POINT stPoint[MAX_CURVE_POINT_NUM];
	memset(stPoint, 0, sizeof(stPoint));

	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;

	float fRange = 0.0f;	//声程
	double fAmp = 0;		//幅值
	

	// 波形区域宽度、高度
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;
	float yb = nWaveHeight/MAX_HEIGHT;
	float xb = nWaveWidth/512.0f;
//	long x,y;
	POINT stTempPoint;
	CURVEPOINT_DATA stTempPointData;
	memset(&stTempPoint, 0, sizeof(POINT));
	memset(&stTempPointData, 0, sizeof(CURVEPOINT_DATA));
	
	if(dwPointNum < 2)
		return;
	else
	{
		for(DWORD i = 0; i < dwPointNum - 1; i++)
		{
			if(pointData[i].data < 1000000)
			{
				stPoint[i] = dataToPoint1(pointData[i], RctWave);
				stPoint[i + 1] = dataToPoint1(pointData[i + 1], RctWave);
				double fAfa = log(((float)pointData[i].data) / pointData[i + 1].data) / ((fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * (pointData[i + 1].serial - pointData[i].serial) / DATA_SIZE);
				if(bDrawFlag == TRUE)
				{
					for(int j = stPoint[i].x; j < stPoint[i + 1].x; j++)
					{
						if(j == stPoint[i].x)
						{
							pPixel = pBaseAddress + stPoint[i].y * LCD_XSIZE_TFT + j;
							*pPixel = wColor;
						}
						else
						{
							fRange = (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * (j - stPoint[i].x) / nWaveWidth;

								fAmp = pointData[i].data / exp(fAfa * fRange);
								
								stTempPointData.data = (float)fAmp;
							//	stTempPointData.serial = fRange * 512 / (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
							//	stTempPoint.x = long(RctWave.left+stTempPointData.serial*xb);
								stTempPoint.x = j;
								stTempPoint.y = long((nWaveHeight-1)-stTempPointData.data*yb) + RctWave.top;
								if(stTempPoint.x < RctWave.right && stTempPoint.y > RctWave.top)
								{
									pPixel = pBaseAddress + stTempPoint.y * LCD_XSIZE_TFT + j;
									*pPixel = wColor;
								}
						}
					}
					if(stPoint[i].y < stPoint[i + 1].y)
					{
						for(int j = stPoint[i].y; j < stPoint[i + 1].y; j++)
						{
							if(j == stPoint[i].y)
							{
								pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stPoint[i].x;
								*pPixel = wColor;
							}
							else
							{
								fAmp = (nWaveHeight + RctWave.top - j) * MAX_HEIGHT / nWaveHeight;
								fRange = (float)(log((float)pointData[i].data / fAmp) / fAfa + pointData[i].serial * DisplaySet_ad[CurrentChannelNo].fRange / DATA_SIZE);
								stTempPointData.data = (float)fAmp;
								stTempPointData.serial = (WORD)(fRange * DATA_SIZE / (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]));// + pointData[i].serial;
								stTempPoint.x = long(RctWave.left+stTempPointData.serial*xb);
								stTempPoint.y = j;
								if(stTempPoint.x < RctWave.right && stTempPoint.y > RctWave.top)
								{
									pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stTempPoint.x;
									*pPixel = wColor;
								}
							}
						}
					}
					else
					{
						for(int j = stPoint[i].y; j > stPoint[i + 1].y; j--)
						{
							if(j == stPoint[i].y)
							{
								pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stPoint[i].x;
								*pPixel = wColor;
							}
							else
							{
								fAmp = (nWaveHeight + RctWave.top - j) * MAX_HEIGHT / nWaveHeight;
								fRange = (float)(log((float)pointData[i].data / fAmp) / fAfa + pointData[i].serial * DisplaySet_ad[CurrentChannelNo].fRange / DATA_SIZE);
								stTempPointData.data = (float)fAmp;
								stTempPointData.serial = (WORD)(fRange * DATA_SIZE / (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]));// + pointData[i].serial;
								stTempPoint.x = long(RctWave.left+stTempPointData.serial*xb);
								stTempPoint.y = j;
								if(stTempPoint.x < RctWave.right && stTempPoint.y > RctWave.top)
								{
									pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stTempPoint.x;
									*pPixel = wColor;
								}
							}
						}
					}
				}
				else
				{
					for(int j = stPoint[i].x; j < stPoint[i + 1].x; j++)
					{
						if(j == stPoint[i].x)
						{
							pPixel = pBaseAddress + stPoint[i].y * LCD_XSIZE_TFT + j;
							*pPixel = wColorBack;
						}
						else
						{
							fRange = (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * (j - stPoint[i].x) / nWaveWidth;
							fAmp = pointData[i].data / exp(fAfa * fRange);
							
							stTempPointData.data = (float)fAmp;
							stTempPoint.x = j;
							stTempPoint.y = long((nWaveHeight-1)-stTempPointData.data*yb) + RctWave.top;
							if(stTempPoint.x < RctWave.right && stTempPoint.y > RctWave.top)
							{
								pPixel = pBaseAddress + stTempPoint.y * LCD_XSIZE_TFT + j;
								*pPixel = wColorBack;
							}
						}
					}
					if(stPoint[i].y < stPoint[i + 1].y)
					{
						for(int j = stPoint[i].y; j < stPoint[i + 1].y; j++)
						{
							if(j == stPoint[i].y)
							{
								pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stPoint[i].x;
								*pPixel = wColorBack;
							}
							else
							{
								fAmp = (nWaveHeight + RctWave.top -j) * MAX_HEIGHT / nWaveHeight;
								fRange = float(log((float)pointData[i].data / fAmp) / fAfa + pointData[i].serial * DisplaySet_ad[CurrentChannelNo].fRange / DATA_SIZE);
								stTempPointData.data = (float)fAmp;
								stTempPointData.serial = (WORD)(fRange * 512 / (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]));// + pointData[i].serial;
								stTempPoint.x = long(RctWave.left+stTempPointData.serial*xb);
								stTempPoint.y = j;
								if(stTempPoint.x < RctWave.right && stTempPoint.y > RctWave.top)
								{
									pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stTempPoint.x;
									*pPixel = wColorBack;
								}
							}
						}
					}
					else
					{
						for(int j = stPoint[i].y; j > stPoint[i + 1].y; j--)
						{
							if(j == stPoint[i].y)
							{
								pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stPoint[i].x;
								*pPixel = wColorBack;
							}
							else
							{
								fAmp = (nWaveHeight + RctWave.top -j) * MAX_HEIGHT / nWaveHeight;
								fRange = (float)(log((float)pointData[i].data / fAmp) / fAfa + pointData[i].serial * DisplaySet_ad[CurrentChannelNo].fRange / DATA_SIZE);
								stTempPointData.data = (float)fAmp;
								stTempPointData.serial = (WORD)(fRange * 512 / (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]));// + pointData[i].serial;
								stTempPoint.x = long(RctWave.left+stTempPointData.serial*xb);
								stTempPoint.y = j;
								if(stTempPoint.x < RctWave.right && stTempPoint.y > RctWave.top)
								{
									pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stTempPoint.x;
									*pPixel = wColorBack;
								}
							}
						}
					}
				}
			}
		}
	}
//	ReleaseDC(hWndMain,hdc);
*/
}

void drawDashAfaCurve(RECT RctWave, CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dwPointNum, WORD wColorBack, WORD wColor, BOOL bDrawFlag)

{
	/*
	POINT stPoint[MAX_CURVE_POINT_NUM];
	memset(&stPoint, 0, sizeof(stPoint));

	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;

	float fRange = 0.0f;	//声程
	float fAmp = 0.0f;		//幅值
	
	// 波形区域宽度、高度
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;
	float yb = nWaveHeight/MAX_HEIGHT;
	float xb = nWaveWidth/512.0f;
//	long x,y;
	POINT stTempPoint;
	CURVEPOINT_DATA stTempPointData;
	memset(&stTempPoint, 0, sizeof(POINT));
	memset(&stTempPointData, 0, sizeof(CURVEPOINT_DATA));
	
	DWORD dwUnitLength = 10;	//间隔长度
	DWORD dwLineLength = 10; //线的长度
	
	for(DWORD i = 0; i < dwPointNum; i++)
	{
		stPoint[i] = dataToPoint1(pointData[i], RctWave);
	}
	
	if(dwPointNum < 2)
		return;
	else
	{
		if(bDrawFlag == TRUE)
		{
			for(DWORD i = 0; i < dwPointNum - 1; i++)
			{
			 	int nLenth = 0;
				int nNum = 1;
				dwLineLength = (DWORD)sqrt(pow((double)(abs(stPoint[i].x - stPoint[i + 1].x)), 2) + pow((double)(abs(stPoint[i].y - stPoint[i + 1].y)), 2));
				DWORD dwNum = dwLineLength / dwUnitLength;; //线分成的等分数
				float fAfa = (float)(log(((float)pointData[i].data) / pointData[i + 1].data) / ((fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * (pointData[i + 1].serial - pointData[i].serial) / DATA_SIZE));
				for(int j = stPoint[i].x; j < stPoint[i + 1].x; j++)
				{
					if(j == stPoint[i].x)
					{
						pPixel = pBaseAddress + stPoint[i].y * LCD_XSIZE_TFT + j;
						*pPixel = wColor;
					}
					else
					{
						fRange = (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * (j - stPoint[i].x) / nWaveWidth;
						fAmp = (float)(pointData[i].data / exp(fAfa * fRange));
						
						stTempPointData.data = (float)fAmp;
					//	stTempPointData.serial = fRange * 512 / (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
					//	stTempPoint.x = long(RctWave.left+stTempPointData.serial*xb);
						stTempPoint.x = j;
						stTempPoint.y = long((nWaveHeight-1)-stTempPointData.data*yb) + RctWave.top;
						pPixel = pBaseAddress + stTempPoint.y * LCD_XSIZE_TFT + j;
						nLenth = (int)sqrt(pow((double)(abs(j - stPoint[i].x)), 2) + pow((double)(abs(stTempPoint.y - stPoint[i].y)), 2));
						if(nLenth / (nNum * dwUnitLength) == 1)//超过一等分的虚线长度
							nNum++;
						if(((nNum & 1) == 1) || (nNum == dwNum))
						{
							if(stTempPoint.x < RctWave.right && stTempPoint.y > RctWave.top)
							{
								pPixel = pBaseAddress + stTempPoint.y * LCD_XSIZE_TFT + j;
								*pPixel = wColor;
							}
						}
					}
				}
				if(stPoint[i].y < stPoint[i + 1].y)
				{
					int nNum = 1;
					int nLenth = 0;
					for(int j = stPoint[i].y; j < stPoint[i + 1].y; j++)
					{
						if(j == stPoint[i].y)
						{
							pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stPoint[i].x;
							*pPixel = wColor;
						}
						else
						{
							fAmp = (nWaveHeight + RctWave.top - j) * MAX_HEIGHT / nWaveHeight;
							fRange = (float)(log((float)pointData[i].data / fAmp) / fAfa + pointData[i].serial * DisplaySet_ad[CurrentChannelNo].fRange / DATA_SIZE);
							stTempPointData.data = (float)fAmp;
							stTempPointData.serial = (WORD)(fRange * DATA_SIZE / (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]));// + pointData[i].serial;
							stTempPoint.x = long(RctWave.left+stTempPointData.serial*xb);
							stTempPoint.y = j;
							
							nLenth = (int)sqrt(pow((double)(abs(stTempPoint.x - stPoint[i].x)), 2) + pow((double)(abs(j - stPoint[i].y)), 2));
							if(nLenth / (nNum * dwUnitLength) == 1)//超过一等分的虚线长度
								nNum++;
							if(((nNum & 1) == 1) || (nNum == dwNum))
							{
								if(stTempPoint.x < RctWave.right && stTempPoint.y > RctWave.top)
								{
									pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stTempPoint.x;
									*pPixel = wColor;
								}
							}
						}
					}
				}
				else
				{
					int nNum = 1;
					int nLenth = 0;
					for(int j = stPoint[i].y; j > stPoint[i + 1].y; j--)
					{
						if(j == stPoint[i].y)
						{
							pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stPoint[i].x;
							*pPixel = wColor;
						}
						else
						{
							fAmp = (nWaveHeight + RctWave.top - j) * MAX_HEIGHT / nWaveHeight;
							fRange = (float)(log((float)pointData[i].data / fAmp) / fAfa + pointData[i].serial * DisplaySet_ad[CurrentChannelNo].fRange / DATA_SIZE);
							stTempPointData.data = (float)fAmp;
							stTempPointData.serial = (WORD)(fRange * DATA_SIZE / (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]));// + pointData[i].serial;
							stTempPoint.x = long(RctWave.left+stTempPointData.serial*xb);
							stTempPoint.y = j;
							
							nLenth = (int)sqrt(pow((double)(abs(stTempPoint.x - stPoint[i].x)), 2) + pow((double)(abs(j - stPoint[i].y)), 2));
							if(nLenth / (nNum * dwUnitLength) == 1)//超过一等分的虚线长度
								nNum++;
							if(((nNum & 1) == 1) || (nNum == dwNum))
							{
								if(stTempPoint.x < RctWave.right && stTempPoint.y > RctWave.top)
								{
									pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stTempPoint.x;
									*pPixel = wColor;
								}
							}
						}
					}
				}
			}
		}
		else
		{
			for(DWORD i = 0; i < dwPointNum - 1; i++)
			{
			 	int nLenth = 0;
				int nNum = 1;
				dwLineLength = (DWORD)sqrt(pow((double)(abs(stPoint[i].x - stPoint[i + 1].x)), 2) + pow((double)(abs(stPoint[i].y - stPoint[i + 1].y)), 2));
				DWORD dwNum = dwLineLength / dwUnitLength;; //线分成的等分数
				float fAfa = (float)(log(((float)pointData[i].data) / pointData[i + 1].data) / ((fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * (pointData[i + 1].serial - pointData[i].serial) / DATA_SIZE));
				for(int j = stPoint[i].x; j < stPoint[i + 1].x; j++)
				{
					if(j == stPoint[i].x)
					{
						pPixel = pBaseAddress + stPoint[i].y * LCD_XSIZE_TFT + j;
						*pPixel = wColorBack;
					}
					else
					{
						fRange = (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * (j - stPoint[i].x) / nWaveWidth;
						fAmp = (float)(pointData[i].data / exp(fAfa * fRange));
						
						stTempPointData.data = (float)fAmp;
					//	stTempPointData.serial = fRange * 512 / (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]);
					//	stTempPoint.x = long(RctWave.left+stTempPointData.serial*xb);
						stTempPoint.x = j;
						stTempPoint.y = long((nWaveHeight-1)-stTempPointData.data*yb) + RctWave.top;
						pPixel = pBaseAddress + stTempPoint.y * LCD_XSIZE_TFT + j;
						nLenth = (int)sqrt(pow((double)(abs(j - stPoint[i].x)), 2) + pow((double)(abs(stTempPoint.y - stPoint[i].y)), 2));
						if(nLenth / (nNum * dwUnitLength) == 1)//超过一等分的虚线长度
							nNum++;
						if(((nNum & 1) == 1) || (nNum == dwNum))
						{
							if(stTempPoint.x < RctWave.right && stTempPoint.y > RctWave.top)
							{
								pPixel = pBaseAddress + stTempPoint.y * LCD_XSIZE_TFT + j;
								*pPixel = wColorBack;
							}
						}
					}
				}
				if(stPoint[i].y < stPoint[i + 1].y)
				{
					int nNum = 1;
					int nLenth = 0;
					for(int j = stPoint[i].y; j < stPoint[i + 1].y; j++)
					{
						if(j == stPoint[i].y)
						{
							pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stPoint[i].x;
							*pPixel = wColorBack;
						}
						else
						{
							fAmp = (nWaveHeight + RctWave.top - j) * MAX_HEIGHT / nWaveHeight;
							fRange = (float)(log((float)pointData[i].data / fAmp) / fAfa + pointData[i].serial * DisplaySet_ad[CurrentChannelNo].fRange / DATA_SIZE);
							stTempPointData.data = (float)fAmp;
							stTempPointData.serial = (WORD)(fRange * DATA_SIZE / (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]));// + pointData[i].serial;
							stTempPoint.x = long(RctWave.left+stTempPointData.serial*xb);
							stTempPoint.y = j;
							
							nLenth = (int)sqrt(pow((double)(abs(stTempPoint.x - stPoint[i].x)), 2) + pow((double)(abs(j - stPoint[i].y)), 2));
							if(nLenth / (nNum * dwUnitLength) == 1)//超过一等分的虚线长度
								nNum++;
							if(((nNum & 1) == 1) || (nNum == dwNum))
							{
								if(stTempPoint.x < RctWave.right && stTempPoint.y > RctWave.top)
								{
									pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stTempPoint.x;
									*pPixel = wColorBack;
								}
							}
						}
					}
				}
				else
				{
					int nNum = 1;
					int nLenth = 0;
					for(int j = stPoint[i].y; j > stPoint[i + 1].y; j--)
					{
						if(j == stPoint[i].y)
						{
							pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stPoint[i].x;
							*pPixel = wColorBack;
						}
						else
						{
							fAmp = (nWaveHeight + RctWave.top - j) * MAX_HEIGHT / nWaveHeight;
							fRange = (float)(log((float)pointData[i].data / fAmp) / fAfa + pointData[i].serial * DisplaySet_ad[CurrentChannelNo].fRange / DATA_SIZE);
							stTempPointData.data = (float)fAmp;
							stTempPointData.serial = (WORD)(fRange * DATA_SIZE / (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]));// + pointData[i].serial;
							stTempPoint.x = long(RctWave.left+stTempPointData.serial*xb);
							stTempPoint.y = j;
							
							nLenth = (int)sqrt(pow((double)(abs(stTempPoint.x - stPoint[i].x)), 2) + pow((double)(abs(j - stPoint[i].y)), 2));
							if(nLenth / (nNum * dwUnitLength) == 1)//超过一等分的虚线长度
								nNum++;
							if(((nNum & 1) == 1) || (nNum == dwNum))
							{
								if(stTempPoint.x < RctWave.right && stTempPoint.y > RctWave.top)
								{
									pPixel = pBaseAddress + j * LCD_XSIZE_TFT + stTempPoint.x;
									*pPixel = wColorBack;
								}
							}
						}
					}
				}
			}
		}
	}
	*/
}

/*
void calculateAvgPixels(POINT selectPoint[], DWORD dwPointNum, BOOL bDrawFlag)
{
	WORD wColor = RED_POINT;
	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);

	HDC hdc = GetDC(hWndMain);

	RECT rect;
	GetClipBox(hdc, &rect);
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	
	if(dwPointNum < 2)
		return;
	else
	{
		for(DWORD i = 0; i < dwPointNum - 1; i++)
		{
			for(int j = selectPoint[i].x; j < selectPoint[i + 1].x; j++)
			{
			}
			if(bDrawFlag == TRUE)
				drawLine(selectPoint[i], selectPoint[i + 1], wColor);
			else
				drawLine(selectPoint[i], selectPoint[i + 1], wColorBack);
		}
	}
}
*/
void drawShiZhi1(POINT point, WORD wColor, BOOL bDrawFlag)
{
	/*
	WORD *pBaseAddress = (WORD *)IMAGE_FRAMEBUFFER_UA_BASE;
	WORD *pPixel = NULL;
	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	
	for(int i = (int)(point.x - 10); i < (int)(point.x + 10); i++)
	{
		if((i > SCALE_WIDTH) && ((i < 640 - SCALE_WIDTH - MENU_WIDTH)))//超过左右波形区不画
		{
			if(point.y > MIN_STATUS_HEIGHT && point.y < 480- SCALE_WIDTH)
			{
				pPixel = pBaseAddress + (int)point.y*LCD_XSIZE_TFT + i;
				if(bDrawFlag ==TRUE)
					*pPixel = wColor;
				else
					*pPixel = wColorBack;
			}
		
			if((point.y - 1) > MIN_STATUS_HEIGHT  && (point.y - 1) < 480- SCALE_WIDTH)
			{
				pPixel = pBaseAddress + (int)(point.y - 1)*LCD_XSIZE_TFT + i;
				if(bDrawFlag ==TRUE)
					*pPixel = wColor;
				else
					*pPixel = wColorBack;
			}

			if((point.y + 1) > MIN_STATUS_HEIGHT && (point.y + 1) < 480- SCALE_WIDTH)
			{
				pPixel = pBaseAddress + (int)(point.y + 1)*LCD_XSIZE_TFT + i;
				if(bDrawFlag ==TRUE)
					*pPixel = wColor;
				else
					*pPixel = wColorBack;
			}
		}
	}
	for(int j = (int)(point.y - 10); j < (int)(point.y + 10); j++)
	{
		if((j > MIN_STATUS_HEIGHT) && (j < 480- SCALE_WIDTH))
		{
			if(point.x > SCALE_WIDTH && point.x < (640 - SCALE_WIDTH - MENU_WIDTH))
			{
				pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)point.x;
				if(bDrawFlag ==TRUE)
					*pPixel = wColor;
				else
					*pPixel = wColorBack;
			}

			if((point.x - 1) > SCALE_WIDTH && (point.x - 1) <(640 - SCALE_WIDTH - MENU_WIDTH))
			{
				pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)(point.x - 1);
				if(bDrawFlag ==TRUE)
					*pPixel = wColor;
				else
					*pPixel = wColorBack;
			}

			if((point.x + 1) > SCALE_WIDTH && (point.x + 1) <(640 - SCALE_WIDTH - MENU_WIDTH))
			{
				pPixel = pBaseAddress + j*LCD_XSIZE_TFT + (int)(point.x + 1);
				if(bDrawFlag ==TRUE)
					*pPixel = wColor;
				else
					*pPixel = wColorBack;
			}
		}
	}
	*/
}


POINT tempCurveDataToPoint(TEMPCURVEPOINT_DATA stTempCurvePoint_data)
{
	POINT tempCurve_point;	
	memset(&tempCurve_point, 0, sizeof(POINT));
	
	HDC hdc = GetDC(hWndMain);

	RECT rect;
	GetClipBox(hdc, &rect);
	float fRate = 0.0f;		//相对VO的百分比
	
	int nWidth = rect.right - rect.left;
	int nHeight = rect.bottom;
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	// 波形区域宽度、高度
	int nWaveWidth = RctWave.right - RctWave.left;
	int nWaveHeight = RctWave.bottom - RctWave.top;

	fRate = 100 * ((float)stTempCurvePoint_data.wWaveSpeed - (float)DisplaySet_ad[CurrentChannelNo].nWaveSpeed) / DisplaySet_ad[CurrentChannelNo].nWaveSpeed;
		
	long x = 0;
	long y = 0;
	x = long(RctWave.left + stTempCurvePoint_data.fTemperature / 1000 * nWaveWidth);
//	y = long((nWaveHeight-1) - stTempCurvePoint_data.wWaveSpeed / 40 * nWaveHeight) + RctWave.top;
//	if(fRate > 0)
		y = long((nWaveHeight-1) / 2 - fRate / 20 * (nWaveHeight / 2)) + RctWave.top;
//	else
//		y = long((nWaveHeight-1) / 2 - fRate / 20 * (nWaveHeight / 2)) + RctWave.top;
	tempCurve_point.x = (DWORD)x;
	tempCurve_point.y = (DWORD)y;
	ReleaseDC(hWndMain,hdc);
	return tempCurve_point;
}
void sortSelectedPointDataByTemp(TEMPCURVEPOINT_DATA queueToSort[], DWORD selectedPointNum)
{	
	TEMPCURVEPOINT_DATA tempData;
	memset(&tempData, 0, sizeof(TEMPCURVEPOINT_DATA));
	if(selectedPointNum >= 2)
	{
		for(DWORD i = 0; i < selectedPointNum - 1; i++)
		{
			for(DWORD j = i + 1; j < selectedPointNum; j++)
			{
				if(queueToSort[i].fTemperature> queueToSort[j].fTemperature)
				{
					tempData = queueToSort[j];
					queueToSort[j] = queueToSort[i];
					queueToSort[i] = tempData;
				}
			}
		}
	}
}
void waveAreaDrawText(LPCTSTR szText, RECT RctInfo)//波形显示区内写文字
{
	HDC hdc = GetDC(hWndMain);
	HDC hMemDC = CreateCompatibleDC(hdc);
	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	HBRUSH hbrWaveBack = CreateSolidBrush(wColorBack);
	COLORREF clrOld =  SetTextColor( hMemDC,  RGB(255,255,255));
	LOGFONT lfInfo;
	memset(&lfInfo, 0, sizeof(lfInfo));
	lfInfo.lfHeight = 24;        
	lfInfo.lfWidth = 0;

	HFONT hFont = CreateFontIndirect(&lfInfo);
	
	HFONT hOldFont = (HFONT) SelectObject (hMemDC, hFont);

	FillRect(hMemDC, &RctInfo, hbrWaveBack);

	DrawText(hMemDC, szText, -1, &RctInfo, 
			  DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	// 复制
	BitBlt(hdc, RctInfo.left, RctInfo.top, (RctInfo.right - RctInfo.left),
				(RctInfo.bottom - RctInfo.top), hMemDC, RctInfo.left, RctInfo.top, SRCCOPY);

	hFont = (HFONT)SelectObject (hMemDC, hOldFont);
	DeleteObject(hFont);
	DeleteObject(hbrWaveBack);
	DeleteDC (hMemDC);
	ReleaseDC(hWndMain,hdc);
}
void drawCurve2(CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dwPointNum,WORD wColor, BOOL bDrawFlag)
{
	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	HDC hdc = GetDC(hWndMain);

	RECT rect;
	GetClipBox(hdc, &rect);
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	if(bDrawFlag)
	{
	//	drawAfaCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		drawCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		
		if(dwPointNum > 0)
		{
			POINT pointLeft;//最左边点
			POINT stFirstPoint;
			stFirstPoint = dataToPoint1(pointData[0], RctWave);
			pointLeft.x = RctWave.left;
			pointLeft.y = stFirstPoint.y;
			drawLine1(pointLeft, stFirstPoint, wColor, RctWave);//擦掉曲线第一点到最左边的直线
		}
		if(dwPointNum > 1)
		{
			POINT pointRight;//最右边点
			POINT stLastPoint;
			stLastPoint = dataToPoint1(pointData[dwPointNum - 1], RctWave);
			pointRight.x = RctWave.right - 1;
			pointRight.y = stLastPoint.y;
			drawLine1(pointRight, stLastPoint, wColor, RctWave);//擦掉曲线最后一点到最右边的直线
		}
	}
	else
	{
	//	drawAfaCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		drawCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		
		if(dwPointNum > 0)
		{
			POINT pointLeft;//最左边点
			POINT stFirstPoint;
			stFirstPoint = dataToPoint1(pointData[0], RctWave);
			pointLeft.x = RctWave.left;
			pointLeft.y = stFirstPoint.y;
			drawLine1(pointLeft, stFirstPoint, wColorBack, RctWave);//擦掉曲线第一点到最左边的直线
		}
		if(dwPointNum > 1)
		{
			POINT pointRight;//最右边点
			POINT stLastPoint;
			stLastPoint = dataToPoint1(pointData[dwPointNum - 1], RctWave);
			pointRight.x = RctWave.right - 1;
			pointRight.y = stLastPoint.y;
			drawLine1(pointRight, stLastPoint, wColorBack, RctWave);//擦掉曲线最后一点到最右边的直线
		}
	}
	ReleaseDC(hWndMain,hdc);
}

void drawCurve3(CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dwPointNum,const WORD& wColor, BOOL bDrawFlag, RECT RctWave)
{
	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
/*	HDC hdc = GetDC(hWndMain);

	RECT rect;
	GetClipBox(hdc, &rect);
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;*/
	if(bDrawFlag)
	{
	//	drawAfaCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		drawCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		
		if(dwPointNum > 0)
		{
			POINT pointLeft;//最左边点
			POINT stFirstPoint;
			stFirstPoint = dataToPoint1(pointData[0], RctWave);
			pointLeft.x = RctWave.left;
			pointLeft.y = stFirstPoint.y;
			drawLine1(pointLeft, stFirstPoint, wColor, RctWave);//擦掉曲线第一点到最左边的直线
		}
		if(dwPointNum > 1)
		{
			POINT pointRight;//最右边点
			POINT stLastPoint;
			stLastPoint = dataToPoint1(pointData[dwPointNum - 1], RctWave);
			pointRight.x = RctWave.right - 1;
			pointRight.y = stLastPoint.y;
			drawLine1(pointRight, stLastPoint, wColor, RctWave);//擦掉曲线最后一点到最右边的直线
		}
	}
	else
	{
	//	drawAfaCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		drawCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		
		if(dwPointNum > 0)
		{
			POINT pointLeft;//最左边点
			POINT stFirstPoint;
			stFirstPoint = dataToPoint1(pointData[0], RctWave);
			pointLeft.x = RctWave.left;
			pointLeft.y = stFirstPoint.y;
			drawLine1(pointLeft, stFirstPoint, wColorBack, RctWave);//擦掉曲线第一点到最左边的直线
		}
		if(dwPointNum > 1)
		{
			POINT pointRight;//最右边点
			POINT stLastPoint;
			stLastPoint = dataToPoint1(pointData[dwPointNum - 1], RctWave);
			pointRight.x = RctWave.right - 1;
			pointRight.y = stLastPoint.y;
			drawLine1(pointRight, stLastPoint, wColorBack, RctWave);//擦掉曲线最后一点到最右边的直线
		}
	}
//	ReleaseDC(hWndMain,hdc);
}
void drawDashCurve2(CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dwPointNum,WORD wColor, BOOL bDrawFlag)
{
	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
	HDC hdc = GetDC(hWndMain);

	RECT rect;
	GetClipBox(hdc, &rect);
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;
	if(bDrawFlag)
	{
	//	drawDashAfaCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		drawDashCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		
		if(dwPointNum > 0)
		{
			POINT pointLeft;//最左边点
			POINT stFirstPoint;
			stFirstPoint = dataToPoint1(pointData[0], RctWave);
			pointLeft.x = RctWave.left;
			pointLeft.y = stFirstPoint.y;
			drawDashLine1(pointLeft, stFirstPoint, wColor, RctWave);//擦掉曲线第一点到最左边的直线
		}
		if(dwPointNum > 1)
		{
			POINT pointRight;//最右边点
			POINT stLastPoint;
			stLastPoint = dataToPoint1(pointData[dwPointNum - 1], RctWave);
			pointRight.x = RctWave.right - 1;
			pointRight.y = stLastPoint.y;
			drawDashLine1(stLastPoint, pointRight, wColor, RctWave);//擦掉曲线最后一点到最右边的直线
		}
	}
	else
	{
	//	drawDashAfaCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		drawDashCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);

		if(dwPointNum > 0)
		{
			POINT pointLeft;//最左边点
			POINT stFirstPoint;
			stFirstPoint = dataToPoint1(pointData[0], RctWave);
			pointLeft.x = RctWave.left;
			pointLeft.y = stFirstPoint.y;
			drawDashLine1(pointLeft, stFirstPoint, wColorBack, RctWave);//擦掉曲线第一点到最左边的直线
		}
		if(dwPointNum > 1)
		{
			POINT pointRight;//最右边点
			POINT stLastPoint;
			stLastPoint = dataToPoint1(pointData[dwPointNum - 1], RctWave);
			pointRight.x = RctWave.right - 1;
			pointRight.y = stLastPoint.y;
			drawDashLine1(stLastPoint, pointRight, wColorBack, RctWave);//擦掉曲线最后一点到最右边的直线
		}
	}
	ReleaseDC(hWndMain,hdc);
}

void drawDashCurve3(CURVEPOINT_DATA pointData[MAX_CURVE_POINT_NUM], DWORD dwPointNum,WORD wColor, BOOL bDrawFlag, RECT RctWave)
{
	WORD wColorBack = RGBtoRGB565(gstSysColor[gCurrentSysColorIndex].clrWaveBack);
/*	HDC hdc = GetDC(hWndMain);

	RECT rect;
	GetClipBox(hdc, &rect);
	
	RECT RctWave;// 波形区域
	RctWave.left = rect.left + SCALE_WIDTH;
	RctWave.right = rect.right - SCALE_WIDTH;
	RctWave.top = rect.top;
	RctWave.bottom = rect.bottom - SCALE_WIDTH;*/
	if(bDrawFlag)
	{
	//	drawDashAfaCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		drawDashCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		
		if(dwPointNum > 0)
		{
			POINT pointLeft;//最左边点
			POINT stFirstPoint;
			stFirstPoint = dataToPoint1(pointData[0], RctWave);
			pointLeft.x = RctWave.left;
			pointLeft.y = stFirstPoint.y;
			drawDashLine1(pointLeft, stFirstPoint, wColor, RctWave);//擦掉曲线第一点到最左边的直线
		}
		if(dwPointNum > 1)
		{
			POINT pointRight;//最右边点
			POINT stLastPoint;
			stLastPoint = dataToPoint1(pointData[dwPointNum - 1], RctWave);
			pointRight.x = RctWave.right - 1;
			pointRight.y = stLastPoint.y;
			drawDashLine1(stLastPoint, pointRight, wColor, RctWave);//擦掉曲线最后一点到最右边的直线
		}
	}
	else
	{
	//	drawDashAfaCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);
		drawDashCurve(RctWave, pointData, dwPointNum, wColorBack, wColor, bDrawFlag);

		if(dwPointNum > 0)
		{
			POINT pointLeft;//最左边点
			POINT stFirstPoint;
			stFirstPoint = dataToPoint1(pointData[0], RctWave);
			pointLeft.x = RctWave.left;
			pointLeft.y = stFirstPoint.y;
			drawDashLine1(pointLeft, stFirstPoint, wColorBack, RctWave);//擦掉曲线第一点到最左边的直线
		}
		if(dwPointNum > 1)
		{
			POINT pointRight;//最右边点
			POINT stLastPoint;
			stLastPoint = dataToPoint1(pointData[dwPointNum - 1], RctWave);
			pointRight.x = RctWave.right - 1;
			pointRight.y = stLastPoint.y;
			drawDashLine1(stLastPoint, pointRight, wColorBack, RctWave);//擦掉曲线最后一点到最右边的直线
		}
	}
//	ReleaseDC(hWndMain,hdc);
}

/**************************
*计算曲面修正值
* @param    gateDefectTime[in]:		伤时
* @param		fWaiJing[in]:管材外径
* @param		fZheSheJiao[in]:折射角
* @param		fQianYan[in]:前沿
* @return   LRESULT :
* @since    1.00
***************************/

QUMIANXIUZHENG_INFO quMianXiuZheng(DWORD dwGateDefectTime, float fWaiBanJing, float fNeiBanJing, float fZheSheJiao, float fQianYan)
{
	QUMIANXIUZHENG_INFO stXiuZhengZhi = {0, 0};
	double dbAllAngle = 0;		//弧长所对应的弧度
	double dbAngle = 0;			//计算的角度对应的弧度
	float fZheSheAngle = fZheSheJiao/180.0f * (float)PI;
	float fRange = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, dwGateDefectTime);
	
	if(tan(fZheSheAngle) < (fNeiBanJing / sqrt(fWaiBanJing * fWaiBanJing - fNeiBanJing * fNeiBanJing))) //第一次在内壁上反射
	{
		double dbSingleZheSheAngle = PI - (fZheSheAngle + (PI - asin(fNeiBanJing / fWaiBanJing / sin(fZheSheAngle))));//一次折射对应的圆心角
		float fSingleZheSheRange =(float)(fNeiBanJing * sin(dbSingleZheSheAngle) / sin(fZheSheAngle));//一次折射距离
		int iZheSheNum = (int)(fRange / fSingleZheSheRange);//折射次数

		if((iZheSheNum & 0x01) == 0)	//偶数
		{
			if(fWaiBanJing - (fRange - iZheSheNum * fSingleZheSheRange) * cos(fZheSheAngle) > 0)
			{
				
				dbAngle = atan((fRange - iZheSheNum * fSingleZheSheRange) * sin(fZheSheAngle) / (fWaiBanJing - (fRange - iZheSheNum * fSingleZheSheRange) * cos(fZheSheAngle)));
				if(cos(fZheSheAngle) != 0)
				{
					stXiuZhengZhi.fShenDu = (float)(fWaiBanJing - ((fRange - iZheSheNum * fSingleZheSheRange) * sin(fZheSheAngle) / sin(dbAngle)));//正弦定律
				}
				dbAllAngle = dbAngle + dbSingleZheSheAngle * iZheSheNum;
				stXiuZhengZhi.fLength = (float)(fWaiBanJing * dbAllAngle  - fQianYan);
			}
		}
		else	//奇数
		{
			double dbZheSheDianAngle = asin(fNeiBanJing / fWaiBanJing / sin(fZheSheAngle)); //折射点所在角度的弧度
			double dbDefectAngle = atan(((fRange - iZheSheNum * fSingleZheSheRange) + fNeiBanJing * cos(dbZheSheDianAngle))/(fNeiBanJing * sin(dbZheSheDianAngle)));//缺陷点所在角度的弧度
			dbAngle = PI - dbZheSheDianAngle - dbDefectAngle;

			stXiuZhengZhi.fShenDu = (float)(fWaiBanJing - (fRange - iZheSheNum * fSingleZheSheRange) * sin(dbZheSheDianAngle) / sin(dbDefectAngle));
			dbAllAngle = dbAngle + dbSingleZheSheAngle * iZheSheNum;
			stXiuZhengZhi.fLength = (float)(fWaiBanJing * dbAllAngle  - fQianYan);
		}
	}
	else//第一次在外壁上反射
	{
		double dbSingleZheSheAngle = PI - 2 * fZheSheAngle;//一次折射对应的圆心角
		float fSingleZheSheRange =(float)(fWaiBanJing * sin(dbSingleZheSheAngle) / sin(fZheSheAngle));//一次折射距离
		int iZheSheNum = (int)(fRange / fSingleZheSheRange);//折射次数

		if(fWaiBanJing - (fRange - iZheSheNum * fSingleZheSheRange) * cos(fZheSheAngle) > 0)
		{
			
			dbAngle = atan((fRange - iZheSheNum * fSingleZheSheRange) * sin(fZheSheAngle) / (fWaiBanJing - (fRange - iZheSheNum * fSingleZheSheRange) * cos(fZheSheAngle)));
			if(cos(fZheSheAngle) != 0)
			{
				stXiuZhengZhi.fShenDu = (float)(fWaiBanJing - ((fRange - iZheSheNum * fSingleZheSheRange) * sin(fZheSheAngle) / sin(dbAngle)));//正弦定律
			}
			dbAllAngle = dbAngle + dbSingleZheSheAngle * iZheSheNum;
			stXiuZhengZhi.fLength = (float)(fWaiBanJing * dbAllAngle  - fQianYan);
		}
	}

	return stXiuZhengZhi;
}
/**************************
*计算DAC曲线开启时波峰对应定量线的DB差
* @param    wGateDefectAmp[in]:		伤幅
* @param    dwGateDefectTime[in]:	伤时
* @param		stDacDingLiang[in]:定量线数据
* @param		wPointNum[in]:定量线的点数量
* @return   LRESULT :
* @since    1.00
***************************/
float dacDeltaDB(WORD wGateDefectAmp, DWORD dwGateDefectTime, CURVEPOINT_DATA stDacDingLiang[MAX_CURVE_POINT_NUM], WORD wPointNum)
{
	float fDeltaDB = 0;
	float firstSoundDistance = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * stDacDingLiang[0].serial/(float)DATA_SIZE;
	float lastSoundDistance = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * stDacDingLiang[wPointNum - 1].serial/(float)DATA_SIZE;
	float fRange = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, dwGateDefectTime);
	float fRange1 = 0;
	float fRange2 = 0;
	float currentPointData = 0;			//当前伤时的曲线点幅值
	
	if(fRange < firstSoundDistance) //读取数据在曲线第一点之前
	{
		if(wGateDefectAmp && stDacDingLiang[0].data)
			fDeltaDB = (float)(20 * log10((float)wGateDefectAmp / stDacDingLiang[0].data));
	}
	else if(fRange > lastSoundDistance) //读取数据在曲线最后点之后
	{
		if(wGateDefectAmp && stDacDingLiang[wPointNum - 1].data)
			fDeltaDB = float(20 * log10((float)wGateDefectAmp / stDacDingLiang[wPointNum - 1].data));
	}
	else
	{
		WORD i;
		for( i = 0; i < wPointNum - 1; i++)
		{
			fRange1 = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * stDacDingLiang[i].serial / (float)DATA_SIZE;
			fRange2 = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * stDacDingLiang[i + 1].serial / (float)DATA_SIZE;
			if((fRange1 <= fRange) && (fRange2 >= fRange))//报警点是否在当前两个曲线点之间
				break;
		}//判断声程位置在哪两个DGS曲线点之间
		//当前伤时的曲线点幅值
		CURVEPOINT_DATA tempPointData1 = stDacDingLiang[i];
		CURVEPOINT_DATA tempPointData2 = stDacDingLiang[i + 1];
		if((fRange2 -fRange1) != 0)
			currentPointData = tempPointData1.data + (fRange - fRange1) / (fRange2 - fRange1) * ((float)tempPointData2.data - (float)tempPointData1.data);
	
		if(currentPointData)
		{
			if(wGateDefectAmp)
				fDeltaDB = (float)(20 * log10(wGateDefectAmp / currentPointData));

			if(fabs(fDeltaDB) < 0.1)
				fDeltaDB = 0;
		}
	}
	return fDeltaDB;
}
/**************************
*计算AVG曲线开启时选中波峰的Φ值
* @param    wGateDefectAmp[in]:		伤幅
* @param    dwGateDefectTime[in]:	伤时
* @param		stCurveData[in]:Φ1对应的曲线
* @param		wPointNum[in]:AVG曲线的点数量
* @return   LRESULT :
* @since    1.00
***************************/
float calculateFaiZhi(WORD wGateDefectAmp, DWORD dwGateDefectTime, CURVEPOINT_DATA stCurveData[MAX_CURVE_POINT_NUM], WORD wPointNum)
{
	float fFaiZhi = 0;
	float fDeltaDB = 0;
	float firstSoundDistance = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * stCurveData[0].serial/(float)DATA_SIZE;
	float lastSoundDistance = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * stCurveData[wPointNum - 1].serial/(float)DATA_SIZE;
	float fRange = Dot2Range(DisplaySet_ad[CurrentChannelNo].nWaveSpeed, dwGateDefectTime);
	float fRange1 = 0;
	float fRange2 = 0;
	float currentPointData = 0;			//当前伤时的曲线点幅值
	
	if(fRange < firstSoundDistance) //读取数据在曲线第一点之前
	{
		if(wGateDefectAmp && stCurveData[0].data)
			fDeltaDB = (float)(20 * log10((float)wGateDefectAmp / stCurveData[0].data));
	}
	else if(fRange > lastSoundDistance) //读取数据在曲线最后点之后
	{
		if(wGateDefectAmp && stCurveData[wPointNum - 1].data)
			fDeltaDB = (float)(20 * log10((float)wGateDefectAmp / stCurveData[wPointNum - 1].data));
	}
	else
	{
		WORD i;
		for( i = 0; i < wPointNum - 1; i++)
		{
			fRange1 = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * stCurveData[i].serial / (float)DATA_SIZE;
			fRange2 = fRangeStart[CurrentChannelNo] + (fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * stCurveData[i + 1].serial / (float)DATA_SIZE;
			if((fRange1 <= fRange) && (fRange2 >= fRange))//报警点是否在当前两个曲线点之间
				break;
		}//判断声程位置在哪两个AVG曲线点之间
		float fAfa = (float)(log(((float)stCurveData[i].data) / stCurveData[i + 1].data) / ((fRangeEnd[CurrentChannelNo] - fRangeStart[CurrentChannelNo]) * (stCurveData[i + 1].serial - stCurveData[i].serial) / DATA_SIZE));
		currentPointData = (float)(stCurveData[i].data / exp(fAfa * (fRange - fRange1)));
		
		if(currentPointData)
		{
			if(wGateDefectAmp)
				fDeltaDB = (float)(20 * log10(wGateDefectAmp / currentPointData));

			if(fabs(fDeltaDB) < 0.1)
				fDeltaDB = 0;
		}
	}
	fFaiZhi = (float)(gfDefectZhi1[CurrentChannelNo] * pow(10, (fDeltaDB / 40)));
	return fFaiZhi;
}
float curveDataChangeByGain(float fDataToChange, float fGain)
{
	float fDataAfterChange = 0.0f;
	fDataAfterChange = (float)(fDataToChange / pow(10, (double)fGain / (-20)));
	return fDataAfterChange;
}
//曲线数据随延迟改变
float curveChangeByDelay(float fDataToChange, float fRangeStartChange, float fRange)
{
	fDataToChange -=  (float)(fRangeStartChange /fRange * DATA_SIZE);
	
	return fDataToChange;
}

