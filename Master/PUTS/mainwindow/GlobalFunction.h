/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 共通处理函数
* Filename: GlobalFunction.h
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2013-07-02
*/

#ifndef _GLOBAL_FUNCTION_H_
#define _GLOBAL_FUNCTION_H_


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <math.h>

#include "DataDefine.h"
#include "mainwindow.h"
#include "PlateInfoPanel.h"
class CPicData;
 
void InitConfig(CH_CONFIG *pCHX_INIT_CONFIG,QSqlDatabase *&pDatabaseConnection,CPlateInfo *&pPlateInfo);  //初始化通道信息
void INIT_CONFIG(CH_CONFIG *pCHX_INIT_CONFIG, CH_CONFIG_GLOBAL *pCHX_CONFIG_GLOBAL);  //和总站一样的接口，用于替换上面那个
QString GetYiQiName();

void RangeProcess(float VSound, float Range, WORD* pInterSpace);
WORD Gain2Data( float Gain);
float Data2Gain( WORD nData);

BOOL ReadStopFlawResult(AECTHREAD_PARAM * p,int iCardIndex);

WORD Distance2Data( float nDistance, WORD SoundVelocity);
DWORD Range2Dot(float VSound, float Range);
float Dot2Range(float VSound, int DotNo);

WORD Range2Dot_New(const float& VSound, const float& Range);
float Dot2Range_New(const float& VSound, const WORD& DotNo);


bool ReadFlawResult(AECTHREAD_PARAM * p, int iCardIndex);

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


float GetGuoLvZ(const float& fX, const float& fY);
bool IsValidData(const float &fX, const float &fY, const float &fZ);
//void InitConfig(CH_CONFIG *pCHX_INIT_CONFIG,QSqlDatabase *&pDatabaseConnection,CPlateInfo *&pPlateInfo);  //初始化通道信息
void DrawVRuler(QPaintDevice *device,const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);
void DrawHRuler(QPaintDevice *device,const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection);
void CalcGridStep(const PlaneFlawHeader& PFH, float& fGridXStep, float& fGridYStep);
void FindBian(QList<HPOINT>& listResource,QList<HPOINT>& listResult,float fXStep, float fYStep );
// 根据伤点数据计算伤的X,Y坐标
bool CalculateFlawXY(const CH_FLAWDATA& FlawInfo, float& fX, float& fY, const DeltaX& deltaX);
bool IsNeiTanRegion(const float& fX, const float& fY);
int PutsHPIReadResult(P9054_HANDLE hDev, CH_FLAWDATA *pFlawData, int icardno);
BOOL AddLostWaveFlawData(QList<CH_FLAWDATA>** pLostWaveDatas, CH_FLAWDATA& flawData);
void SearchUsefulLostWaveData(QList<CH_FLAWDATA>& lostWavedata);
// 根据底波过滤
BOOL IsYunXuFlaw(CH_FLAWDATA& flawData);
void GetNeitanBoxStartNum2(NeiTanBoxStartNum& NeitanBoxStartNum);
int GetCurrentProbeBoxNum();//计算当前活动探头所属哪个探头盒
int GetCurrentProbeFromCardNum();//计算当前活动探头所属哪块卡
void ControlCodec(P9054_HANDLE hDev, WORD codecState);
//void WaveDelay(WDC_DEVICE_HANDLE hDev, WORD nDelay);
void WaveDelay(P9054_HANDLE hDev, WORD nDelay);
void SetMonitorRange(P9054_HANDLE hDev, WORD *pInterSpace);
void SelChannel(WORD channelNO); //底波伤波切换
int PutsReadPoints(AECTHREAD_PARAM* pThread, CH_FLAWDATA* pFlawData, DWORD dwAddr, int iCardNo);
BOOL IsFlawIdentical(const CH_FLAWDATA& pFlawData1, const CH_FLAWDATA& pFlawData2);
void ResetHPI_INT(P9054_HANDLE hDev);
void DrawRuler(QPainter &painter, QRectF rect, double start, double end, int separate, bool isHori, bool topToBottom);
void ReplaceOldToNew();
#endif
