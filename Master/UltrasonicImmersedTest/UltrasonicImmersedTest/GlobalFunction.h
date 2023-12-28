#ifndef _GLOBAL_FUNCTION_H_
#define _GLOBAL_FUNCTION_H_

class CPicData;
class CCScanView;
class CCScanParamPanel;

UINT AECWorkThreadProc(LPVOID pParam);
int AECThread_Idle(AECTHREAD_PARAM * pThreadParam);
BOOL AEC_selChannel(AECTHREAD_PARAM * pThreadParam);
BOOL AEC_palte_setone(AECTHREAD_PARAM * pThreadParam);


void Delay(DWORD dwUs);

// 获取EXE的工作目录
DWORD GetWorkingFolder(LPTSTR lpszName, DWORD nCapacity);



// 由CH_FLAWDATA_CSCAN数据提取坐标
void GetCScanDataCoordinate(const CH_FLAWDATA_CSCAN_S& data, double& dX, double& dY, double& dZ, 
							double& dW1, double& dA, double& dB, double& dW2, int nDw1Flag,  int nAxisIndex,
							const int& nCardNo, const DWORD& dwChannelNo);

BOOL PrimitiveDataChange(const CH_FLAWDATA_CSCAN_S& Cdata, CFeatureInfo& Fdata, const int& nCardNo, const DWORD& dwChannelNo);


BOOL ScreenCapture(HBITMAP directBmp, BITMAPINFO *bmInfo, BITMAPFILEHEADER *bmFileHeader,
				   WORD bitCount, HDC hScrDC, RECT rect, int nType, int nObjectNum, int nPictureMode);
DWORD ScreenCapture2(HBITMAP directBmp, BITMAPINFO *bmInfo, BITMAPFILEHEADER *bmFileHeader, 
					 WORD bitCount, HDC hScrDC, RECT rect, HDC hScrDC2, RECT rect2, int nObjectNum);
BOOL SaveCscanWithInfo(HBITMAP directBmp, BITMAPINFO *bmInfo, BITMAPFILEHEADER *bmFileHeader,
					   WORD bitCount, HDC hScrDC, RECT rect, int nObjectNum, int nPictureMode);
void GlobalDrawScale(HDC hDC, 
					 const CRect& rect, 
					 float fStart, 
					 float fEnd, 
					 float fStep, 
					 int direction, 
					 BOOL bReverse);
void DrawPattle( HDC hDC, CRect& DrawRect, CCScanView* pCScanView, CCScanParamPanel* pCScanParam);
void DrawPattle( HDC hDC, float fGrayBegin, float fGrayEnd, CRect& DrawRect);
void DrawPattleRule( HDC hDC, float fStart, float fEnd, CRect& rectV);
// void dBProcess(const float& db, 
// 			   WORD& db_0_data,
// 			   WORD& db_1_data,
// 			   WORD& db_2_data,
// 			   WORD& db_3_data);

// 检查SetOne参数合法性
BOOL CheckSetOneParam(CSCAN_ARS200FPGA_SETONE& stPar);
float CurveDataChangeByGain(float fDataToChange, float fChangeGain);
void SortCurvePointByRange(CURVEPOINT_DATA_t stCurveToSort[], int nPointNum);
float GetTotalPlus(float fPlus, float fRefPlus, int nChnNo);
float Range2MicroSecond(float fVSound, float fRange); // wangbingfu add  at 2012-11-19
// wangbingfu add start at 2013-03-19
float Lagrange_lk(int k,float x, int nX[], int nCount);
float Lagrange(float x, int nX[], int nY[], int nCount);
// wangbingfu add end at 2013-03-19
void ErroWindowMessage();	// 显示当前错误信息  Song Chenguang add on 2013-05-29
// machao add start on 2013-08-13
float Percent100ToDB(float fDBZeroAmp, float fValue);
float DBTo100Percent(float fDBZeroAmp, float fdB);
// machao add end on 2013-08-13
#endif