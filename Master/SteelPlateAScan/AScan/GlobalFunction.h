#ifndef _GLOBAL_FUNCTION_H_
#define _GLOBAL_FUNCTION_H_

class CPicData;

//UINT AECWorkThreadProc(LPVOID pParam);
unsigned __stdcall AECWorkThreadProc(LPVOID pParam);

BOOL AECCardInit(AECTHREAD_PARAM * pThreadParam);
BOOL AEC_plate_Start(AECTHREAD_PARAM * pThreadParam);
int AECThread_Idle(AECTHREAD_PARAM * pThreadParam);
BOOL AEC_plate_Stop(AECTHREAD_PARAM * pThreadParam);
BOOL AEC_selChannel(AECTHREAD_PARAM * pThreadParam);
BOOL AEC_palte_setone(AECTHREAD_PARAM * pThreadParam);
void RangeProcess(float VSound, float Range, WORD* pInterSpace);
WORD Gain2Data( float Gain);
float Data2Gain( WORD nData);

BOOL ReadStopFlawResult(AECTHREAD_PARAM * p,int iCardIndex);

WORD Distance2Data( float nDistance, WORD SoundVelocity);
DWORD Range2Dot(float VSound, float Range);
float Dot2Range(float VSound, int DotNo);

WORD Range2Dot_New(const float& VSound, const float& Range);
float Dot2Range_New(const float& VSound, const WORD& DotNo);


BOOL ReadFlawResult(AECTHREAD_PARAM * p, int iCardIndex);

void Delay(DWORD dwUs);

// 获取EXE的工作目录
DWORD GetWorkingFolder(LPTSTR lpszName, DWORD nCapacity);

// 读取DMA数据
//BOOL ReadDMADate(AECTHREAD_PARAM * p, const DWORD& dwLocalAddress);
BOOL ReadDMADate(AECTHREAD_PARAM * p, WORD wSramFlag);
int MicroSecond2Dot(const float& fMicroSecond);

float Dot2MicroSecond(const int& nDot);

// 由CH_FLAWDATA_CSCAN数据提取坐标
void GetCScanDataCoordinate(const CH_FLAWDATA_CSCAN_D& data, double& dX, double& dY, double& dZ, double& dW1, double& dA, double& dB, double& dW2);

//BOOL PrimitiveDataChange(const CH_FLAWDATA_CSCAN_D& Cdata, CFeatureInfo& Fdata, const DWORD& dwChannelNo);
BOOL PrimitiveDataChange(const CH_FLAWDATA_CSCAN_D& Cdata, CFeatureInfo& Fdata);

BOOL ScreenCapture(LPCWSTR fileName, WORD bitCount, HDC hScrDC, RECT rect, int nType);
BOOL ScreenCapture2(LPCWSTR fileName, WORD bitCount, HDC hScrDC, RECT rect, HDC hScrDC2, RECT rect2);
#endif