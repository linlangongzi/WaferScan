/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 共通处理函数
* Filename: GlobalFunction.cpp
*
* @author        Wang Qianfeng
* @version       1.00         Date: 2013-07-02
*/

#include <QDebug>
#include <QObject>
#include <QMutexLocker>
#include <QMessageBox>
#include <QMetaObject>
#include <Qstring>
#include <math.h>
#include <tchar.h>
#include <QMutex>
#include <QPainter>
#include <QAxWidget>
#include <QVariant>
#include "windows.h"
#include "headerFile.h"
#include "mainwindow.h"
#include "DataDefine.h"
#include "GlobalFunction.h"
#include <time.h>

//const DWORD MAX_FLAW_COUNT = 200000;
DWORD g_dwActiveChannel = 0;//DEFAULT_CHANNEL;
DWORD g_dwActiveChannelLuoJi = 0;//chenyuzhu add on 2014-6-20 逻辑通道号全局变量;

QString g_strGongYi;

extern QMutex g_csCard;
CH_FLAWDATA_CSCAN_D g_stPrimitiveData[2][PER_CH_DATA_NUM*2];
short g_nWaveSpeed = 5980; 
QSqlDatabase *g_sqlDB = NULL;//数据库连接的指针以后都要改成这个全局变量，不再传递参数，因为用到的地方太多
QString g_MaxCheckId;
QList<CH_FLAWDATA>* g_LostWaveDatas[MAX_CHANNEL_NO];	// 需在保存前初始化
QMap<WORD,WORD>   m_LogicChannel2ArrayMap;               //逻辑通道号转换到物理通道号
QMap<WORD,WORD>   m_Array2LogicChannelMap;

QMap<WORD, float> LogicChannelQianMangQuMap;
QMap<WORD, float> LogicChannelHouMangQuMap;
QMap<WORD, float> LogicChannelMaxDiShiMap;
float be_distance = 0.0;
int g_nADCFreq;
int g_nADCaiYangShu;
int g_nSpeed;

CH_FLAWDATA flawdata[1024];
CH_FLAWDATA flawdata_result[1024 * 1024];
extern PLATE_INFO g_Info[];
extern int nPlate;
extern QMutex g_ItemMutex;
extern GlobalItemValue g_ItemValue;
extern NeiTanBoxStartNum g_NeiTanBoxStartNum;
extern OtherParameters g_OtherParameters;
extern const DWORD MAX_FLAW_COUNT;
extern QMutex g_CS;
extern DWORD g_dwFlawCount;
extern int nCardEndStauts;
extern QAxWidget *g_pPlateDraw;
extern QMutex g_ItemCS;
extern const WORD TYPE_LOST_WAVE;
extern const int CHANNEL_NUM;
extern CH_CONFIG allCH_CONFIG[];
extern CIniParams g_IniParams;
extern bool g_DepthFilterSwitch;
DWORD g_dwActiveFlawOrBottom = 0;//0伤波 1底波
extern AScan *g_pAscan;

CH_FLAWDATA g_MonitorFlaw[MAX_CARD_NO];
extern HANDLE hMonitorFlawEvent[MAX_CARD_NO]; //用于监视通道信息，报警
//BOOL bUpdateDatabase = FALSE;

extern bool g_pbBiaoding; //chenyuzhu addon 2014-11-7
extern float g_GainSale[500];

/** 
* 生成固定范围内的一组随机数
* @param     range_min [in]:	范围最小值.
* @param     range_max [in]:	范围最大值.
* @param     n [in]:			数组个数.
* @param     pArray [out]:		数组指针.
* @return    DWORD:
* @since     1.00
*/
void RangedRandDemo( WORD range_min, WORD range_max, int n , WORD *pArray)
{
	WORD *p = pArray;
	int i;
	for ( i = 0; i < n; i++ )
	{
		WORD u = (double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min;
		// 画线
		*p = u;
		p++;
	}
}


void ResetHPI_INT(P9054_HANDLE hDev)
{	
	WORD wValueL = HPI_RC_INIT_VALUE;
	WORD wValueH = HPI_RC_INIT_VALUE;

	wValueL = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_L);
	wValueH = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_H);

	wValueL |= HPI_HINT_RESET_VALUE;
	wValueH |= HPI_HINT_RESET_VALUE;

	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_L, wValueL);
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_H, wValueH);
}

bool ReadFlawResult(AECTHREAD_PARAM * p,int iCardIndex)
{
	QString str;
	WORD wInterruptType = 0;
	WORD wValueL =0, wValueH=0;	

	wValueL = P9054_ReadWord(p->hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_L);
	wValueH = P9054_ReadWord(p->hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_H);

	wValueL &= HPI_HINT_RESET_VALUE;
	int temp = 3;
	if( p->index == 0)
	{
		int a = 1;
	}
	if( p->index == 1)
	{
		int a = 1;
	}
	if( p->index == 2)
	{
		int a = 1;
	}
	if( p->index == 3)
	{
		int a = 1;
	}
	if( p->index == 4)
	{
		int a = 1;
	}
	if( wValueL == HPI_HINT_RESET_VALUE ) //有中断发生(探到伤点，报警和画C扫实时探伤图)
	{			
		WORD wValue = 0;
	    wInterruptType = P9054_ReadWord(p->hDev, P9054_ADDR_SPACE0, DSP_2_PCI_COMMUNICATE_CMD_NUM_ADDR);
		// 申请SRAM控制权	
		DWORD begin = GetTickCount();		
		if (wInterruptType == INTERRUPT_REAL_TIME_1)
		{		
			PutsReadPoints(p, flawdata, CH_FLAW_DATA_ADDR_1, iCardIndex);	
		}
//////
 	/*	if (wInterruptType != INTERRUPT_RESULT)
 		{
 			// 读0x0x123c6 INTERRUPT_TYPE_ADDR_AUTO_ZENGYI
 			WDC_DIAG_ReadWriteAddr(p->hDev, WDC_READ, BAR2, INTERRUPT_TYPE_ADDR_AUTO_ZENGYI, &wInterruptType, WDC_MODE_16);
			if (wInterruptType == AUTO_PLUS_FLAG)
 			{
 				// 读取地址INTERRUPT_TYPE_ADDR_AUTO_ZENGYI_FLAW (0x123c8) 开始的空间
 				AUTO_DATA_TYPE data[MAX_CARD_CHANNEL_NO];
 				::ZeroMemory(data, sizeof(AUTO_DATA_TYPE)*MAX_CARD_CHANNEL_NO);	
 				ReadAutoPlusData(p->hDev, data, MAX_CARD_CHANNEL_NO, INTERRUPT_TYPE_ADDR_AUTO_ZENGYI_FLAW);
 				// 用读取数据更新界面显示通道的闸门和增益信息
 				for (int i=0; i<MAX_CARD_CHANNEL_NO; i++)
				{
 					WORD nWuliNum = 0;
 					g_CS.Lock(1);
					BOOL bRet = m_LogicChannel2ArrayMap.Lookup(data[i].CH_NO, nWuliNum);
 
 					if (bRet)
 					{
 						allCH_CONFIG[nWuliNum].CH_FRONTTIME = data[i].CH_FRONTTIME;
 						allCH_CONFIG[nWuliNum].CH_REARTIME = data[i].CH_REARTIME;
 						allCH_CONFIG[nWuliNum].CH_GAIN_C = data[i].CH_GAIN / 2;
 						allCH_CONFIG[nWuliNum].CH_GAIN_D = data[i].CH_GAIN / 2;
 					}
 
 					g_CS.Unlock();
 
 					if (dwActiveChannel == nWuliNum)
					{
 						DWORD dwObjectIndex = WaitForSingleObject(hAutoPlusEvent, 0);
 						if (dwObjectIndex != WAIT_OBJECT_0)
 						{
 							SetEvent(hAutoPlusEvent);
 						}
 					}					
 				}
 			}	
 		}	*/
////
		wValue = 0;
		P9054_WriteWord(p->hDev,P9054_ADDR_SPACE0,DSP_2_PCI_SRAM_CONTROL1_ADDR,wValue);
		ResetHPI_INT(p->hDev);//清除中断
	}
	else
	{
	}
	return TRUE;
}


int PutsReadPoints( AECTHREAD_PARAM* pThread, CH_FLAWDATA* pFlawData, DWORD dwAddr, int iCardNo)
{	
	DWORD dwWaitObjectIndex = WaitForSingleObject(hMonitorFlawEvent[iCardNo], 0);
	P9054_HANDLE hDev= pThread->hDev;
	WORD Count = 0;
	if (dwWaitObjectIndex != WAIT_OBJECT_0)
	{		
		Count = P9054_ReadWord(hDev,P9054_ADDR_SPACE0,dwAddr);
		dwAddr += 0x2;

		WORD *pData = (WORD *)(&g_MonitorFlaw[iCardNo]);
		if (Count > 0)
		{
			CH_FLAWDATA tempData = g_MonitorFlaw[iCardNo];		
			for (int i = (Count-1)*12;i <  Count* 12;i++)// 只读最后一个伤			
			{
				pData[i-(Count-1)*12] = P9054_ReadWord(hDev,P9054_ADDR_SPACE0,dwAddr);
				dwAddr += 0x2;
			}	
			// 判断读伤前后的数据是否相同
			bool bRet = IsFlawIdentical(g_MonitorFlaw[iCardNo], tempData);
			SetEvent(hMonitorFlawEvent[iCardNo]);
		}		
	}
	return Count;
}
BOOL IsFlawIdentical(const CH_FLAWDATA& pFlawData1, const CH_FLAWDATA& pFlawData2)
{
	if (pFlawData1.CH_NO == pFlawData2.CH_NO &&
		pFlawData1.CH_FLAW_TIME == pFlawData2.CH_FLAW_TIME &&
		pFlawData1.CH_FLAW_PEAK == pFlawData2.CH_FLAW_PEAK &&
		pFlawData1.CH_BE_TIME == pFlawData2.CH_BE_TIME &&
		pFlawData1.CH_BE_PEAK == pFlawData2.CH_BE_PEAK &&
		pFlawData1.CH_HEIGHT == pFlawData2.CH_HEIGHT &&
		pFlawData1.CHX_GAIN == pFlawData2.CHX_GAIN &&
		pFlawData1.CH_FRONTTIME == pFlawData2.CH_FRONTTIME &&
		pFlawData1.CH_REARTIME == pFlawData2.CH_REARTIME &&
		pFlawData1.DETECT_STATE == pFlawData2.DETECT_STATE &&
		pFlawData1.CH_X == pFlawData2.CH_X )
	{
		return TRUE;
	}
	else
		return FALSE;
}









/*
void RangeProcess(float VSound, // VSound--------------m/s
				  float Range, // Range --------------m
				  WORD* pInterSpace) // pInterSpace ---------
{

	int nTotal= (int)(Range*2/1000.0/VSound*ADFREQUENCY*1000000.0);
	if(nTotal < MONITOR_SIZE)
	{
		for (int i=0;i<512;i++)
		{

			pInterSpace[i]=0;
		}
	}
	else
	{
		int nInter=nTotal/512;
		int nReset=nTotal%512;
		int k=1;
		for (int i=0;i<512;i++)
		{
			if(nInter < 1)
				nInter = 1;
			pInterSpace[i]=nInter-1;
		}



		for (int i=0;i<512;i++)
		{
			float temp = i*1.0*nReset/511;
			if(temp>= k )
			{
				pInterSpace[i] = pInterSpace[i] +1;
				k++;
			}
			else
			{
				;
			}
		}
	}
}
*/
// 参考总站修改 2013-11-29 Song Chenguang
void RangeProcess(float VSound, float Range, WORD* pInterSpace)
// VSound--------------m/s
// Range --------------m
// pInterSpace ---------
{
	int nTotal= (int)(Range*2/1000.0/VSound*ADFREQUENCY*1000000.0);
	int nInter=nTotal/512;
	int nReset=nTotal%512;
	int k=1;
	for (int i=0;i<512;i++)
	{
		pInterSpace[i]=nInter;
	}

	for (int i=0;i<512;i++)
	{
		float temp = i*1.0*nReset/511;
		if(temp>= k )
		{
			pInterSpace[i] = pInterSpace[i] +1;
			k++;
		}
		else
		{
			;
		}
	}



}

BOOL ReadStopFlawResult(AECTHREAD_PARAM * p,int iCardIndex)
{
	QString str;
	WORD wInterruptType=0xffff;
	WORD wValueL =0, wValueH=0;

	WORD wValue = 0;

	while(1)
	{
		DWORD begin = GetTickCount();
		do
		{	// 读中断信号
			wValueL = P9054_ReadWord(p->hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_L);
			wValueH = P9054_ReadWord(p->hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_H);
			wValueL &= HPI_HINT_RESET_VALUE;
			Delay(100);

			DWORD cha = GetTickCount() - begin;
			if (cha > 30000)
			{
				QString str = QString("%1%2%3").arg(QObject::tr("第 ")).arg(p->index+1).arg(QObject::tr(" 块卡读结束中断失败，请检查卡是否正常或重新启动机器。"));
				QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);

				g_IniParams.m_pMainWindow->m_PLXCommunication.UnInit(g_IniParams.m_pMainWindow->m_stThreadParam);
				ExitProcess(0);
			}
		} while (wValueL != HPI_HINT_RESET_VALUE );

		//获取控制命令
		wInterruptType = P9054_ReadWord(p->hDev, P9054_ADDR_SPACE0, DSP_2_PCI_COMMUNICATE_CMD_NUM_ADDR);

		if ( wInterruptType == INTERRUPT_RESULT)
		{
			break;
		}
		else
		{
			p->pPLXCommunication->ResetHPI_INT(p->hDev);
		}		 

	}	

	// 收到结束信号
	// 读取伤数据
	//chenyuzhu modify start on 2014-11-6
	if(g_pbBiaoding == false)
	{
		int nRet = PutsHPIReadResult(p->hDev, flawdata_result, iCardIndex);
	}
	//chenyuzhu modify end on 2014-11-6

	WORD nInterSpace[FLAW_DOT_MAX_NO];				
	RangeProcess(g_nWaveSpeed, 0, nInterSpace);
	p->pPLXCommunication->SetMonitorRange(p->hDev, nInterSpace);

	//清除中断
	p->pPLXCommunication->ResetHPI_INT(p->hDev);
	return TRUE;
}



float Data2Gain( WORD nData)
{
	float Gain;
	if(nData > GAIN_DATA_MAX)
	{
		//		Gain = GAIN_MAX;
		Gain = GAIN_MAX;
	}
	else
	{
		//		Gain =  (nData*3.0/20.0)/AEC_AMP_LINEAR_PARA_A;
		Gain =  (nData*3.0/20.0)/AEC_AMP_LINEAR_PARA_A;
	}

	return Gain;
}

WORD Gain2Data( float Gain)
{
	WORD nData;

	if(Gain <= GAIN_MIN)
	{
		nData = GAIN_DATA_MIN; 
	}
	else if( Gain > GAIN_MAX)
	{
		nData = GAIN_DATA_MAX;
	}
	else
	{
		int tmp = (int)((Gain*20.0/3.0* AEC_AMP_LINEAR_PARA_A)+0.5);
		nData = static_cast<WORD>(tmp);
	}


	return nData;
}

//////////////////////////////////////////////////////////////////////////
//
// 硬件配置初始化
//
//////////////////////////////////////////////////////////////////////////

DWORD Range2Dot(float VSound, float Range)
{
	DWORD temp;
	temp = Range*2/1000.0/VSound*ADFREQUENCY*1000000.0;
	return temp;
}
//////////////////////////////
//
//Dot -> nRange (mm) 
//
///////////////////////

float Dot2Range(float VSound, int DotNo)
{
	float temp;
	temp = DotNo/2.0*1000.0*VSound/float(ADFREQUENCY)/1000000.0; 
	float temp2 = (int)(temp * 10 + 0.5) / 10.0f;
	return temp2;
}

WORD Range2Dot_New(const float& VSound, const float& Range)
{
	WORD temp = 0;
	
	return temp;
}

float Dot2Range_New(const float& VSound, const WORD& DotNo)
{
	float temp = 0.0f;
	
	return temp;
}

WORD Distance2Data( float nDistance, WORD SoundVelocity)
{
	WORD nData;
	nData = (WORD )(nDistance *100000.0/SoundVelocity);
	return(nData);
}

void Delay(DWORD dwUs)
{
	DWORD dwUs1 = DELAY_TIME * dwUs;
	for (DWORD i = 0;i < dwUs1;i++) 
	{
	}
}



/** 
* 获取EXE的工作目录
* @param     lpszName[out]: 工作目录
* @param     nCapacity[in]: 字符串长度
* @return    void
* @since     1.00
*/
DWORD GetWorkingFolder(LPTSTR lpszName, DWORD nCapacity)
{
	if (lpszName != NULL && nCapacity > 0)
		lpszName[0] = _T('\0');
	TCHAR szExeName[MAX_PATH];
	DWORD nSize = ::GetModuleFileName(GetModuleHandle(NULL), LPWSTR(szExeName), MAX_PATH);
	if (nSize == 0)
		return 0;
	do {
		nSize--;
	} while(szExeName[nSize] != TCHAR('\\') && nSize > 0);

	if (nSize > 0)
		nSize++;
	if (nSize > nCapacity - 1)
		return nSize;

	if (lpszName == NULL || nCapacity == 0)
		return nSize;

	memcpy(lpszName, szExeName, nSize*sizeof(TCHAR));
	lpszName[nSize] = _T('\0');
	return nSize;
}

// 读取DMA数据
BOOL ReadDMADate(AECTHREAD_PARAM * p, WORD wSramFlag)
{	
	return TRUE;
}

int MicroSecond2Dot(const float& fMicroSecond)
{
	int ret = fMicroSecond * 1 * ADFREQUENCY;
	if(ret == 0)
		ret = 1;
	return ret;
}

float Dot2MicroSecond(const int& nDot)
{
	return float(nDot) / ADFREQUENCY;
}

// 由CH_FLAWDATA_CSCAN数据提取坐标
void GetCScanDataCoordinate(const CH_FLAWDATA_CSCAN_D& data, double& dX, double& dY, double& dZ, double& dW1, double& dA, double& dB,double& dW2)
{
// 	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
// 	CMovePathView* pView = (CMovePathView*) pFrame->m_wndSplitter.GetPane(0,0);

	DWORD dwXHigh = data.wXHigh16;
	dX = (data.wXLow16 + (dwXHigh << 16)) / CODER_PER_RING_XY;
	DWORD dwYHigh = data.wYHigh16;
	dY = (data.wYLow16 + (dwYHigh << 16)) / CODER_PER_RING_XY;
	DWORD dwZHigh = data.wZHigh16;
	dZ = (data.wZLow16 + (dwZHigh << 16)) / CODER_PER_RING_Z;

	DWORD dwW1High = data.wW1High16;
	dW1 = (data.wW1Low16 + (dwW1High << 16)) / CODER_PER_RING_XY;
	DWORD dwAHigh = data.wAHigh16;
	dA = (data.wALow16 + (dwAHigh << 16)) / CODER_PER_RING_XY;
	DWORD dwBHigh = data.wBHigh16;
	dB = (data.wBLow16 + (dwBHigh << 16)) / CODER_PER_RING_XY;
	DWORD dwW2High = data.wW2High16;
	dW2 = (data.wW2Low16 + (dwW2High << 16)) / CODER_PER_RING_XY;

	double dParam[30];
	::ZeroMemory(dParam, sizeof(dParam));
	long lLogParam[10];
	::ZeroMemory(lLogParam, sizeof(lLogParam));
// 	pView->m_GraphMove.GetPara(dParam, lLogParam);
// 	if (lLogParam[0] == 0) // 横向
// 	{
// 		dY += dParam[19]*(data.wCardNum*2 + data.wChannelNo);
// 	} 
// 	else	// 纵向
// 	{
// 		dX += dParam[19]*(data.wCardNum*2 + data.wChannelNo);
// 	}
}


BOOL PrimitiveDataChange(const CH_FLAWDATA_CSCAN_D& Cdata, CFeatureInfo& Fdata)
{
	GetCScanDataCoordinate(Cdata, Fdata.m_dX, Fdata.m_dY, Fdata.m_dZ, Fdata.m_dW1, Fdata.m_dA, Fdata.m_dB, Fdata.m_dW2);

	/*CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	WORD wChannelNo = Cdata.wCardNum*2 + Cdata.wChannelNo;

	CChannelInfo info = pFrame->GetChannelInfo((WORD)wChannelNo);
	BOOL bMTPmode[4] = {FALSE, FALSE, FALSE, FALSE};
	float fDelay = 0.0f;
	DWORD dwDelayDot = 0;

	for (int i=0; i<4; i++)
	{
		bMTPmode[i] = info.m_GateInfo[i].m_nMtpMode;		
	}	
	fDelay = info.m_fChannelDelay;
	dwDelayDot = MicroSecond2Dot(fDelay);

	int i = 0;

	if(pFrame->m_pFeatureWnd->m_bGateOpen[0])
	{
		int j = 1;

		if (Cdata.wGate1SyncTime <= 2)
		{
			return FALSE;
		}

		Fdata.m_pfFeatures[i][0] = float(Cdata.wGate1SyncTime) / ADFREQUENCY + fDelay;

		if (pFrame->m_pFeatureWnd->m_bFPFTimeDisplay)
		{
			if(bMTPmode[0])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate1FPFirstTime);
			else
			{
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate1FPFirstTime + dwDelayDot);
			}

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMTimeDisplay)
		{
			if(bMTPmode[0])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate1FPTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate1FPTime + dwDelayDot);

			j++;
		}

		if(pFrame->m_pFeatureWnd->m_bMPFTimeDisplay)
		{
			if(bMTPmode[0])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate1MPFirstTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate1MPFirstTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMTimeDisplay)
		{
			if(bMTPmode[0])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate1MPTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate1MPTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = Cdata.wGate1FPAmp / MAX_HEIGHT *100.0f;
			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = Cdata.wGate1MPAmp / MAX_HEIGHT *100.0f;
			j++;
		}

		i++;
	}

	if (pFrame->m_pFeatureWnd->m_bGateOpen[1])
	{
		int j = 1;		

		Fdata.m_pfFeatures[i][0] = float(Cdata.wGate2SyncTime) / ADFREQUENCY + fDelay;

		if (pFrame->m_pFeatureWnd->m_bFPFTimeDisplay)
		{
			if (bMTPmode[1])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate2FPFirstTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate2FPFirstTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMTimeDisplay)
		{
			if (bMTPmode[1])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate2FPTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate2FPTime + dwDelayDot);

			j++;
		}

		if(pFrame->m_pFeatureWnd->m_bMPFTimeDisplay)
		{
			if(bMTPmode[1])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate2MPFirstTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate2MPFirstTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMTimeDisplay)
		{
			if(bMTPmode[1])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate2MPTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate2MPTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = Cdata.wGate2FPAmp / MAX_HEIGHT *100.0f;
			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = Cdata.wGate2MPAmp / MAX_HEIGHT *100.0f;
			j++;
		}

		i++;
	}

	if(pFrame->m_pFeatureWnd->m_bGateOpen[2])
	{
		int j = 1;
		Fdata.m_pfFeatures[i][0] = float(Cdata.wGate3SyncTime) / ADFREQUENCY + fDelay;

		if (pFrame->m_pFeatureWnd->m_bFPFTimeDisplay)
		{
			if(bMTPmode[2])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate3FPFirstTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate3FPFirstTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMTimeDisplay)
		{
			if(bMTPmode[2])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate3FPTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate3FPTime + dwDelayDot);

			j++;
		}

		if(pFrame->m_pFeatureWnd->m_bMPFTimeDisplay)
		{
			if(bMTPmode[2])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate3MPFirstTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate3MPFirstTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMTimeDisplay)
		{
			if(bMTPmode[2])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate3MPTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate3MPTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = Cdata.wGate3FPAmp / MAX_HEIGHT *100.0f;
			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = Cdata.wGate3MPAmp / MAX_HEIGHT *100.0f;
			j++;
		}

		i++;
	}

	if (pFrame->m_pFeatureWnd->m_bGateOpen[3])
	{
		int j = 1;
		Fdata.m_pfFeatures[i][0] = float(Cdata.wGate4SyncTime) / ADFREQUENCY + fDelay;

		if (pFrame->m_pFeatureWnd->m_bFPFTimeDisplay)
		{
			if(bMTPmode[3])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate4FPFirstTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate4FPFirstTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMTimeDisplay)
		{
			if(bMTPmode[3])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate4FPTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate4FPTime + dwDelayDot);

			j++;
		}

		if(pFrame->m_pFeatureWnd->m_bMPFTimeDisplay)
		{
			if(bMTPmode[3])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate4MPFirstTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate4MPFirstTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMTimeDisplay)
		{
			if(bMTPmode[3])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate4MPTime);
			else
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate4MPTime +dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = Cdata.wGate4FPAmp / MAX_HEIGHT *100.0f;
			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = Cdata.wGate4MPAmp / MAX_HEIGHT *100.0f;
			j++;
		}

		i++;
	}*/

	return TRUE;
}

/**************************
*截取指定区域的屏幕，并保存文件
* @param	fileName[in]:保存位图文件的文件路径，如果为NULL则不保存
* @param	bitCount[in]:bit深度，用于表示一个像素点所使用的数据长度
* @param	hScrDC[HDC]:要截取的DC
* @param	lpRect[in]:所要截取的DC的区域
* @param	nType[in]:图像类型　１：Ｃ扫　２：Ａ扫
* @return   BITMAP
* @since    1.00
***************************/
BOOL ScreenCapture(LPCWSTR fileName, WORD bitCount, HDC hScrDC, RECT rect, int nType)
{
	HANDLE hbmFile = NULL;
	DWORD dwWritten = 0;
	DWORD dwBitmapArraySize;//位图数据大小
	HDC hMemDC = NULL;         
	int nScreenWidth = 0; 
	int nScreenHeight = 0;
	int ixStart = 0; 
	int iyStart = 0; 
	int ix = 0; 
	int iy = 0;
	//the pointer will save all pixel point's color value
	BYTE *lpBitmapBits = NULL; 

	ixStart = rect.left;
	iyStart = rect.top;
	ix = rect.right - rect.left;
	iy = rect.bottom - rect.top;

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
	
	if(nType == 2)	// A扫
	{
		SetBkMode(hMemDC,TRANSPARENT);
		SetTextColor(hMemDC, RGB(255, 255, 255));
//		CString strCoord;
// 		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
// 		CMechanicalControlPanel* pMechanicalControlView = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
// 		strCoord.Format(_T("X=%.1f Y=%.1f Z=%.1f"),pMechanicalControlView->m_dCurrentCoord[0],pMechanicalControlView->m_dCurrentCoord[1],
// 			pMechanicalControlView->m_dCurrentCoord[2]);
//		TextOut(hMemDC, ix - 160, iy - 65, strCoord, strCoord.GetLength());
	}
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
// 	CString strTime;
// 	strTime.Format(_T("%d-%2d-%2d %2d：%2d：%2d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
// 									sysTime.wHour+8, sysTime.wMinute, sysTime.wSecond);
	QString strTime = QString("%d-%2d-%2d %2d: %2d: %2d").arg(sysTime.wYear).arg(sysTime.wMonth).arg(sysTime.wDay).arg(sysTime.wHour+8).arg(sysTime.wMinute).arg(sysTime.wSecond);

	TextOut(hMemDC, ix - 160, iy - 50, strTime.toStdWString().c_str(), strTime.length());
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
		QMessageBox::information(NULL, "Error", QObject::tr("创建图片失败！"), QMessageBox::Yes, QMessageBox::Yes);
		return FALSE;
	}

	if(!WriteFile(hbmFile, &bmFileHeader, sizeof(BITMAPFILEHEADER),&dwWritten, NULL))
	{
		QMessageBox::information(NULL, "Error", QObject::tr("写图片失败！"), QMessageBox::Yes, QMessageBox::Yes);
		CloseHandle(hbmFile);
		return FALSE;
	}
	if(!WriteFile(hbmFile, &(bmInfo.bmiHeader), sizeof(BITMAPINFOHEADER),&dwWritten, NULL))
	{
		QMessageBox::information(NULL, "Error", QObject::tr("写图片失败！"), QMessageBox::Yes, QMessageBox::Yes);
		CloseHandle(hbmFile);
		return FALSE;
	}
	if(!WriteFile(hbmFile, lpBitmapBits, dwBitmapArraySize,&dwWritten, NULL))
	{
		QMessageBox::information(NULL, "Error", QObject::tr("写图片失败！"), QMessageBox::Yes, QMessageBox::Yes);
		CloseHandle(hbmFile);
		return FALSE;
	}
	CloseHandle(hbmFile);
	
	QMessageBox::information(NULL, "Error", QObject::tr("图片保存成功！"), QMessageBox::Yes, QMessageBox::Yes);

	//删除
	DeleteObject(hMemDC);
	DeleteObject(hScrDC);
	DeleteObject(directBmp);
	DeleteObject(previousObject);

	return TRUE;
}

/**************************
*截取指定区域的屏幕，并保存文件
* @param	fileName[in]:保存位图文件的文件路径，如果为NULL则不保存
* @param	bitCount[in]:bit深度，用于表示一个像素点所使用的数据长度
* @param	hScrDC[HDC]:要截取的DC
* @param	lpRect[in]:所要截取的DC的区域
* @return   BITMAP
* @since    1.00
***************************/
BOOL ScreenCapture2(LPCWSTR fileName, WORD bitCount, HDC hScrDC, RECT rect, HDC hScrDC2, RECT rect2)
{
	HANDLE hbmFile = NULL;
	DWORD dwWritten = 0;
	DWORD dwBitmapArraySize;//位图数据大小
	//	HDC hScrDC = NULL;
	HDC hMemDC = NULL;         
	int nScreenWidth = 0; 
	int nScreenHeight = 0;
	int ixStart = 0; 
	int iyStart = 0; 
	int ixStart2 = 0; 
	int iyStart2 = 0; 
	int ix = 0; 
	int iy = 0;
	int iy1 = 0;
	int iy2 = 0;
	//the pointer will save all pixel point's color value
	BYTE *lpBitmapBits = NULL; 

	//creates a device context for the screen device
	/*	hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

	//get the screen point size
	nScreenWidth = GetDeviceCaps(hScrDC, HORZRES);
	nScreenHeight = GetDeviceCaps(hScrDC, VERTRES);
	*/
	/*	ixStart = rect.left;
	iyStart = rect.top;
	ix = rect.right - rect.left;
	iy = rect.bottom - rect.top;
	*/	
	ixStart = rect.left;
	iyStart = rect.top;
	ixStart2 = rect2.left;
	iyStart2 = rect2.top;

	iyStart = rect.top;
	ix = rect.right - rect.left;
	iy = (rect.bottom - rect.top) + (rect2.bottom - rect2.top);

	iy1 = rect.bottom - rect.top;
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
	BitBlt(hMemDC, 0, 0, ix, iy1, hScrDC, ixStart, iyStart, SRCCOPY);
	BitBlt(hMemDC, 0, iy1, ix, iy, hScrDC2, ixStart2, iyStart2, SRCCOPY);

	SetBkMode(hMemDC,TRANSPARENT);
	SetTextColor(hMemDC, RGB(255, 255, 255));
// 	CString strCoord;
// 	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
// 	CMechanicalControlPanel* pMechanicalControlView = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
// 	strCoord.Format(_T("X=%.1f Y=%.1f Z=%.1f"),pMechanicalControlView->m_dCurrentCoord[0],pMechanicalControlView->m_dCurrentCoord[1],
// 						pMechanicalControlView->m_dCurrentCoord[2]);
//	TextOut(hMemDC, ix - 160, iy1 - 65, strCoord, strCoord.GetLength());

	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
// 	CString strTime;
// 	strTime.Format(_T("%d-%2d-%2d %2d：%2d：%2d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
// 						sysTime.wHour+8, sysTime.wMinute, sysTime.wSecond);

	QString strTime = QString("%d-%2d-%2d %2d: %2d: %2d").arg(sysTime.wYear).arg(sysTime.wMonth).arg(sysTime.wDay).arg(sysTime.wHour+8).arg(sysTime.wMinute).arg(sysTime.wSecond);

	TextOut(hMemDC, ix - 160, iy1 - 50, strTime.toStdWString().c_str(), strTime.length());
	SetTextColor(hMemDC, RGB(0, 0, 0));
	TextOut(hMemDC, ix - 160, iy - 50, strTime.toStdWString().c_str(), strTime.length());
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
		QMessageBox::information(NULL, "Error", QObject::tr("创建图片失败！"), QMessageBox::Yes, QMessageBox::Yes);
		return FALSE;
	}

	if(!WriteFile(hbmFile, &bmFileHeader, sizeof(BITMAPFILEHEADER),&dwWritten, NULL))
	{
		QMessageBox::information(NULL, "Error", QObject::tr("写图片失败！"), QMessageBox::Yes, QMessageBox::Yes);
		CloseHandle(hbmFile);
		return FALSE;
	}
	if(!WriteFile(hbmFile, &(bmInfo.bmiHeader), sizeof(BITMAPINFOHEADER),&dwWritten, NULL))
	{
		QMessageBox::information(NULL, "Error", QObject::tr("写图片失败！"), QMessageBox::Yes, QMessageBox::Yes);
		CloseHandle(hbmFile);
		return FALSE;
	}
	if(!WriteFile(hbmFile, lpBitmapBits, dwBitmapArraySize,&dwWritten, NULL))
	{
		QMessageBox::information(NULL, "Error", QObject::tr("写图片失败！"), QMessageBox::Yes, QMessageBox::Yes);
		CloseHandle(hbmFile);
		return FALSE;
	}
	CloseHandle(hbmFile);

	QMessageBox::information(NULL, "Error", QObject::tr("图片保存成功！"), QMessageBox::Yes, QMessageBox::Yes);
	//删除
	DeleteObject(hMemDC);
	DeleteObject(hScrDC);
	DeleteObject(directBmp);
	DeleteObject(previousObject);

	return TRUE;
}

QString GetYiQiName()
{
	QString strFileName = QDir::currentPath() + ("/YiQiConfig.ini");
	QFile file(strFileName);
	file.open(QIODevice::ReadOnly);
	QString str;
	while (!file.atEnd())
	{
		QByteArray byte = file.readLine(); 
		QString strPath = QString::fromLocal8Bit(byte.data());
		if (strPath.left(2) == QObject::tr("名称"))
		{
			file.close();
			return strPath.right(strPath.length() - 3);
		}
	}
	file.close();
	return "";
}
//chenyuzhu modify start on 2014-7-8
void INIT_CONFIG(CH_CONFIG *pCHX_INIT_CONFIG, CH_CONFIG_GLOBAL *pCHX_CONFIG_GLOBAL)  //初始化通道信息（替换原来的）
{
	QString strYiQiName = GetYiQiName();
	strYiQiName = strYiQiName.trimmed();
	
	//chenyuzhu add start on 2014-7-8
	QString strFileName = QDir::currentPath()+ "/SheBeifile.dbfl";

	QFile file1(strFileName);
	bool bRet1 = file1.open(QIODevice::ReadOnly);

	if (!bRet1)
	{
		QMessageBox::warning(NULL,"InitConfig",QObject::tr("SheBeifile.dbfl打开失败"));
		return ;
	}
	file1.seek(0);
	SHEBEICANSHU SheBeiCanShu;
	quint64 nBytesRead = file1.read((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));
	file1.close();
	//chenyuzhu add end on 2014-7-8
	
/*		//chenzyuzhu add start 2014-7-10 给数据文件初始值用的
	//////////////////////////////////ChannelParam////////////////////////////////////////
	for (UINT i=0; i < MAX_CHANNEL_NO; i++)
	{	

		SheBeiCanShu.ChannelParam[i].nNum = i;
		if(i >= 0 && i < 8)
			SheBeiCanShu.ChannelParam[i].nRefNum = i +1000;
		if(i >= 8 && i < 16)
			SheBeiCanShu.ChannelParam[i].nRefNum = i +2000;
		if(i >=16 && i < 88)
			SheBeiCanShu.ChannelParam[i].nRefNum = i +3000;
		if(i >=88 )
			SheBeiCanShu.ChannelParam[i].nRefNum = i +4000;

		SheBeiCanShu.ChannelParam[i].nZhongZhiDian = 511;		//中值点
		strcpy_s(SheBeiCanShu.ChannelParam[i].strIsOpen, "开");				//通道开启
		SheBeiCanShu.ChannelParam[i].fICIV = 26;				//底波初始增益值(db)
		SheBeiCanShu.ChannelParam[i].fTCIV = 12;				//底波初始增益值(db)
		SheBeiCanShu.ChannelParam[i].fDelay = 33.1;		//延迟(mm)
		SheBeiCanShu.ChannelParam[i].fHuaMenQianYan = 2;		//闸门前沿(mm)
		SheBeiCanShu.ChannelParam[i].fHuaMenHouYan = 23.5;		//闸门后沿(mm)
		SheBeiCanShu.ChannelParam[i].fDiBoHuaMenHouYan = 29.15;		//底波闸门后沿(mm)
		SheBeiCanShu.ChannelParam[i].fDiBoYuZhi = 20;		//底波阈值(%)
		SheBeiCanShu.ChannelParam[i].fYiZhi = 0.1;	//抑制(%)


	}
	//////////////////////////////////TanShangGongYi////////////////////////////////////////

	strcpy_s(SheBeiCanShu.TanShangGongYi.strJianBoMoShi, "全波");//检波模式
	SheBeiCanShu.TanShangGongYi.fShangMangQu = 2;//上盲区
	SheBeiCanShu.TanShangGongYi.fXiaMangQu = 3;	//下盲区
	strcpy_s(SheBeiCanShu.TanShangGongYi.strDACOpen, "关");		//DAC开启
	strcpy_s(SheBeiCanShu.TanShangGongYi.strAuToZengYiOpen, "关");		//自动增益开启
	strcpy_s(SheBeiCanShu.TanShangGongYi.strAutoHuaMenOpen, "关");	//自动闸门跟踪
	strcpy_s(SheBeiCanShu.TanShangGongYi.strBaoJing, "开");


	////////////////////////////////////Plate_Type//////////////////////////////////////
	SheBeiCanShu.Plate_Type.fLWaveSpeed = 5970;

	//////////////////////////////////////////////////////////////////////////
	strcpy_s(SheBeiCanShu.YiQiParam.strName, "PUTS-ARS144");
	SheBeiCanShu.YiQiParam.fTanTouZhongXinJuLi = 20;
	SheBeiCanShu.YiQiParam.fADCFreq = 50;
	SheBeiCanShu.YiQiParam.nADCaiYangShu = 10;
	SheBeiCanShu.YiQiParam.nBianTan1TongDaoShu = 3;        //边探1通道数
	SheBeiCanShu.YiQiParam.nBianTan2TongDaoShu = 3;		//边探2通道数
	SheBeiCanShu.YiQiParam.nNeiTan1TongDaoShu = 69;		//内探1通道数
	SheBeiCanShu.YiQiParam.nNeiTan2TongDaoShu = 66;			//内探2通道数

	SheBeiCanShu.YiQiParam.nBianTan1WuLiBegin = 16;			//边探1物理开始通道号
	SheBeiCanShu.YiQiParam.nBianTan2WuLiBegin = 88;          //边探2物理开始通道号
	SheBeiCanShu.YiQiParam.nNeiTan1WuLiBegin = 0;          //内探1物理开始通道号
	SheBeiCanShu.YiQiParam.nNeiTan2WuLiBegin = 8;          //内探2物理开始通道号


	//////////////////////////////////////////////////////////////////////////

	if (!file1.open(QIODevice::ReadWrite | QIODevice::Truncate))
	{
		QMessageBox::warning(NULL,"AScanSave",QObject::tr("SheBeiFile.dbfl打开失败"));
		return ;
	}
	quint64 nBytesWrite = file1.write((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));
	//////////////////////////////////////////////////////////////////////////
//chenzyuzhu add end on 2014-7-10 给数据文件初始值用的

*/

	//chenyuzhu modify start on 2014-7-8
	//QString strSQL = QString("%1%2%3").arg("select ADCFreq,ADCaiYangShu from t_YiQi where name= '").arg(strYiQiName).arg("'");
/*	QSqlQuery query(*g_sqlDB);  //创建QSqlQuery
	query.exec(strSQL);
	query.next();
	QString strADCFreq = query.value(0).toString();      //ADCFreq
	QString strADCaiYangShu = query.value(1).toString(); //ADCaiYangShu
	
	g_nADCFreq = strADCFreq.toInt()*1000000;				//strADCFreq以MHz为单位
	g_nADCaiYangShu = strADCaiYangShu.toInt();				//采样数
	query.clear();*/
	g_nADCFreq = SheBeiCanShu.YiQiParam.fADCFreq * 1000000;
	g_nADCaiYangShu = SheBeiCanShu.YiQiParam.nADCaiYangShu;
	//chenyuzhu modify end on 2014-7-8

/*	QString strID;
	QString strCH_DEM_MODE;
	QString strMIDDLE_VALUE;
	QString strCH_FRONTTIME;
	QString strCH_REARTIME;
	QString strCH_MAXBACKTIME;
	QString strCH_BEMINDATA;
	QString strCH_TGC_DA_START;
	QString strFEGain;
	QString strBEGain;
	QString strCH_GAIN_A;
	QString strCH_GAIN_B;
	QString strCH_GAIN_C;
	QString strCH_GAIN_D;
	QString strCH_REJECT_VALUE;
	QString strCH_ZEROCORRECT;
	QString strCH_FLAW_GAIN_MUX;
	QString strCH_BE_GAIN_MUX;
	QString strCH_TGC_DAC_EN;
	QString strCH_FLAW_SURF_DISTANCE;
	QString strCH_FLAW_BE_DISTANCE;
	QString strCH_EN;
	QString strAUTO_GAIN_EN;
	QString strAUTO_GATE_EN;
	QString strALARM_EN;
	QString strCH_REF_NO;
	QString strCH_FLAW_DEF;
	QString strCH_SELFCHECK;
	QString strCH_MONITOR_RANGE;
	QString strCH_NO;
	QString strSpeed;

	strSQL = QString("%1%2%3%4%5").arg("select t_Channel.id as id,t_TanShangGongYi.JianBoMoShi as JianBoMoShi,\
									   t_TanShangGongYi.ShangMangQu as ShangMangQu,\
									   t_TanShangGongYi.XiaMangQu as XiaMangQu,\
									   t_TanShangGongYi.DACOpen as DACOpen,\
									   t_TanShangGongYi.AutoZengYiOpen as AutoZengYiOpen,\
									   t_TanShangGongYi.AutoHuaMenOpen as AutoHuaMenOpen,\
									   t_TanShangGongYi.BaoJing as BaoJing,\
									   t_Channel.Num as Num,\
									   t_Channel.RefNum as RefNum,\
									   t_Channel.ZhongZhiDian as ZhongZhiDian,\
									   t_Channel.IsOpen as IsOpen,\
									   t_Channel.ICIV as ICIV,\
									   t_Channel.TCIV as TCIV,\
									   t_Channel.Delay as Delay,\
									   t_Channel.HuaMenQianYan as HuaMenQianYan,\
									   t_Channel.HuaMenHouYan as HuaMenHouYan,\
									   t_Channel.DiBoHuaMenHouYan as DiBoHuaMenHouYan,\
									   t_Channel.DiBoYuZhi as DiBoYuZhi,\
									   t_Channel.YiZhi as YiZhi,\
									   t_PlateType.LWaveSpeed as LWaveSpeed,\
									   t_PlateType.TWaveSpeed as TWaveSpeed \
									   from t_Channel join t_TanShangGongYi on t_Channel.TanShangGongYi=t_TanShangGongYi.name \
									   join t_PlateType on t_PlateType.name=t_TanShangGongYi.PlateType \
									   where t_TanShangGongYi.name=").arg("'").arg(g_strGongYi).arg("'").arg("order by t_Channel.Num");
	query.exec(strSQL);*/
/*	while (query.next())
	{
		strID = query.value(0).toString();//
		strCH_DEM_MODE = query.value(1).toString();//检波模式
		strCH_FLAW_SURF_DISTANCE = query.value(2).toString();//上盲区
		strCH_FLAW_BE_DISTANCE = query.value(3).toString();	//下盲区
		strCH_TGC_DAC_EN = query.value(4).toString();		//DAC开启
		strAUTO_GAIN_EN = query.value(5).toString();		//自动增益开启
		strAUTO_GATE_EN = query.value(6).toString();		//自动闸门跟踪
		strALARM_EN = query.value(7).toString();			//报警开关
		strCH_NO = query.value(8).toString();				//物理通道号
		strCH_REF_NO = query.value(9).toString();			//逻辑通道号
		strMIDDLE_VALUE = query.value(10).toString();		//中值点
		strCH_EN = query.value(11).toString();				//通道开启
		strFEGain = query.value(12).toString();				//底波初始增益值(db)
		strBEGain = query.value(13).toString();				//底波初始增益值(db)
		strCH_ZEROCORRECT = query.value(14).toString();		//延迟(mm)
		strCH_FRONTTIME = query.value(15).toString();		//闸门前沿(mm)
		strCH_REARTIME = query.value(16).toString();		//闸门后沿(mm)
		strCH_MAXBACKTIME = query.value(17).toString();		//底波闸门后沿(mm)
		strCH_BEMINDATA = query.value(18).toString();		//底波阈值(%)
		strCH_REJECT_VALUE = query.value(19).toString();	//抑制(%)
		strSpeed = query.value(20).toString();				//纵波声速(m/s)

		g_nSpeed = strSpeed.toInt()*1000; //应该是全局变量暂且这样写
		strCH_DEM_MODE = strCH_DEM_MODE.trimmed();
		int nWuLiNum = strCH_NO.toInt();

		pCHX_INIT_CONFIG[nWuLiNum].CH_DEM_MODE = 0;
		pCHX_INIT_CONFIG[nWuLiNum].id = strID.toInt();
		pCHX_INIT_CONFIG[nWuLiNum].MIDDLE_VALUE = strMIDDLE_VALUE.toInt();

		pCHX_INIT_CONFIG[nWuLiNum].CH_FRONTTIME = strCH_FRONTTIME.toFloat() / g_nSpeed * g_nADCFreq * 2;	
		pCHX_INIT_CONFIG[nWuLiNum].CH_REARTIME = (g_Info[nPlate].height - strCH_REARTIME.toFloat())/g_nSpeed*g_nADCFreq*2;
		pCHX_INIT_CONFIG[nWuLiNum].CH_MAXBACKTIME = (g_Info[nPlate].height + strCH_MAXBACKTIME.toFloat())/g_nSpeed*g_nADCFreq*2;


		pCHX_INIT_CONFIG[nWuLiNum].CH_BEMINDATA=100;
		float fGainDiff =  strFEGain.toFloat() - strBEGain.toFloat();
		if (fGainDiff > 17)
		{
			QString str = QString("%1%2%3%4").arg(strCH_REF_NO.toInt()).arg(QObject::tr(" 通道伤波底波增益差为 ")).arg(fGainDiff).arg(QObject::tr("，超出限制"));
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
		}
		
		pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_DIFF = Gain2Data(strFEGain.toFloat() - strBEGain.toFloat());
		pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_A = Gain2Data(REF_GAIN);
		pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_B = Gain2Data(REF_GAIN);
		pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_C = Gain2Data(strBEGain.toFloat() / 2.0);
		pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_D = Gain2Data(strBEGain.toFloat() / 2.0);
		float fRejectValue = strCH_REJECT_VALUE.toFloat();
		pCHX_INIT_CONFIG[nWuLiNum].CH_REJECT_VALUE = fRejectValue * pow(2.0,g_nADCaiYangShu - 1) / 100.0;
		pCHX_INIT_CONFIG[nWuLiNum].CH_ZEROCORRECT = strCH_ZEROCORRECT.toFloat() / g_nSpeed * g_nADCFreq * 2;
		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_GAIN_MUX = 0;
		pCHX_INIT_CONFIG[nWuLiNum].CH_BE_GAIN_MUX = 0;
		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_DEF = Gain2Data(10.0);              //需要用数据库参数修改

		strCH_TGC_DAC_EN = strCH_TGC_DAC_EN.trimmed();

		pCHX_INIT_CONFIG[nWuLiNum].CH_TGC_DAC_EN = ((strCH_TGC_DAC_EN==QObject::tr("开"))?1:0);//判断通道是否是开
		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_SURF_DISTANCE = strCH_FLAW_SURF_DISTANCE.toFloat() / g_nSpeed * g_nADCFreq * 2;
		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_BE_DISTANCE = strCH_FLAW_BE_DISTANCE.toFloat() / g_nSpeed * g_nADCFreq * 2;
		strCH_EN = strCH_EN.trimmed();
		pCHX_INIT_CONFIG[nWuLiNum].CH_EN = ((strCH_EN == QObject::tr("开")) ? 1:0 );

		strAUTO_GAIN_EN = strAUTO_GAIN_EN.trimmed();
		pCHX_INIT_CONFIG[nWuLiNum].AUTO_GAIN_EN = ((strAUTO_GAIN_EN == QObject::tr("开")) ? 1:0 );
		strAUTO_GATE_EN = strAUTO_GATE_EN.trimmed();
		pCHX_INIT_CONFIG[nWuLiNum].AUTO_GATE_EN=((strAUTO_GATE_EN == QObject::tr("开")) ? 1:0 );
		strALARM_EN = strALARM_EN.trimmed();
		pCHX_INIT_CONFIG[nWuLiNum].ALARM_EN = ((strALARM_EN == QObject::tr("开")) ? 1:0 );
		pCHX_INIT_CONFIG[nWuLiNum].CH_REF_NO = strCH_REF_NO.toInt();

		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_DEF = 0;
		pCHX_INIT_CONFIG[nWuLiNum].CH_SELFCHECK = 0;
		pCHX_INIT_CONFIG[nWuLiNum].CH_MONITOR_RANGE = MONITOR_SIZE;
		pCHX_INIT_CONFIG[nWuLiNum].CH_NO = nWuLiNum; 
	   //给一些后面要用的map赋值
 		LogicChannelQianMangQuMap[pCHX_INIT_CONFIG[nWuLiNum].CH_REF_NO] = strCH_FRONTTIME.toFloat();
 		LogicChannelHouMangQuMap[pCHX_INIT_CONFIG[nWuLiNum].CH_REF_NO] = strCH_REARTIME.toFloat();
 		LogicChannelMaxDiShiMap[pCHX_INIT_CONFIG[nWuLiNum].CH_REF_NO] = g_Info[nPlate].height + 
 			strCH_MAXBACKTIME.toFloat();
		int RefMun = strCH_REF_NO.toInt();
		m_Array2LogicChannelMap[nWuLiNum] = RefMun;
		m_LogicChannel2ArrayMap[RefMun] = nWuLiNum;
	}*/

	//chenyuzhu add start on 2014-7-7
	int nID;
	QString strCH_DEM_MODE;
	int nMIDDLE_VALUE;
	float fCH_FRONTTIME;
	float fCH_REARTIME;
	float fCH_MAXBACKTIME;
	float fCH_BEMINDATA;
	QString strCH_TGC_DA_START;
	float fFEGain;
	float fBEGain;
	QString strCH_GAIN_A;
	QString strCH_GAIN_B;
	QString strCH_GAIN_C;
	QString strCH_GAIN_D;
	float fCH_REJECT_VALUE;
	float fCH_ZEROCORRECT;
	QString strCH_FLAW_GAIN_MUX;
	QString strCH_BE_GAIN_MUX;
	QString strCH_TGC_DAC_EN;
	float fCH_FLAW_SURF_DISTANCE;	//上盲区
	float fCH_FLAW_BE_DISTANCE;		//下盲区
	QString strCH_EN;
	QString strAUTO_GAIN_EN;
	QString strAUTO_GATE_EN;
	QString strALARM_EN;
	int nCH_REF_NO;
	QString strCH_FLAW_DEF;
	QString strCH_SELFCHECK;
	QString strCH_MONITOR_RANGE;
	int nCH_NO;
	float fSpeed;
	int nBian1Logicalstart = g_OtherParameters.edgeCar1LogicalChannelStart;
	int nBian2Logicalstart = g_OtherParameters.edgeCar2LogicalChannelStart;
	int nNei1Logicalstart = g_OtherParameters.insideCar1LogicalChannelStart;
	int nNei2Logicalstart = g_OtherParameters.insideCar2LogicalChannelStart;
	int nBianProbeNum = g_OtherParameters.edgeCarEachBoxChannelCount;
	int nNei1ProbeNum = g_OtherParameters.insideCar1BoxCount * g_OtherParameters.insideCarEachBoxChannelCount;
	int nNei2ProbeNum = g_OtherParameters.insideCar2BoxCount * g_OtherParameters.insideCarEachBoxChannelCount;
	
	for (UINT i=0; i < MAX_CHANNEL_NO; i++)
	{
		nID = SheBeiCanShu.ChannelParam[i].nId;//
		strCH_DEM_MODE = SheBeiCanShu.TanShangGongYi.strJianBoMoShi;//检波模式
		fCH_FLAW_SURF_DISTANCE = SheBeiCanShu.TanShangGongYi.fShangMangQu;//上盲区
		fCH_FLAW_BE_DISTANCE = SheBeiCanShu.TanShangGongYi.fXiaMangQu;	//下盲区
		strCH_TGC_DAC_EN = SheBeiCanShu.TanShangGongYi.strDACOpen;		//DAC开启
		strAUTO_GAIN_EN = SheBeiCanShu.TanShangGongYi.strAuToZengYiOpen;		//自动增益开启
		strAUTO_GATE_EN = SheBeiCanShu.TanShangGongYi.strAutoHuaMenOpen;		//自动闸门跟踪
		strALARM_EN = SheBeiCanShu.TanShangGongYi.strBaoJing;			//报警开关
		nCH_NO = SheBeiCanShu.ChannelParam[i].nNum;				//物理通道号
		nCH_REF_NO = SheBeiCanShu.ChannelParam[i].nRefNum;			//逻辑通道号
		nMIDDLE_VALUE = SheBeiCanShu.ChannelParam[i].nZhongZhiDian;		//中值点
		strCH_EN = SheBeiCanShu.ChannelParam[i].strIsOpen;				//通道开启
		fFEGain = SheBeiCanShu.ChannelParam[i].fICIV;				//伤波初始增益值(db)
		fBEGain = SheBeiCanShu.ChannelParam[i].fTCIV;				//底波初始增益值(db)
		fCH_ZEROCORRECT = SheBeiCanShu.ChannelParam[i].fDelay;		//延迟(mm)
		fCH_FRONTTIME = SheBeiCanShu.ChannelParam[i].fShangmangqu;		//闸门前沿(mm)
		fCH_REARTIME = SheBeiCanShu.ChannelParam[i].fXiamangqu;		//闸门后沿(mm)
		fCH_MAXBACKTIME = SheBeiCanShu.ChannelParam[i].fDibozhamenkuandu;		//底波闸门后沿(mm)
		fCH_BEMINDATA = SheBeiCanShu.ChannelParam[i].fDiBoYuZhi;		//底波阈值(%)
		fCH_REJECT_VALUE = SheBeiCanShu.ChannelParam[i].fYiZhi;	//抑制(%)
		fSpeed = SheBeiCanShu.Plate_Type.fLWaveSpeed;				//纵波声速(m/s)
		g_nSpeed = fSpeed * 1000; //应该是全局变量暂且这样写
		strCH_DEM_MODE = strCH_DEM_MODE.trimmed();
		int nWuLiNum = nCH_NO;

		//chenyuzhu add start on 2014-10-22
		if (nCH_REF_NO < nBian1Logicalstart)
		{
			continue;
		}
		if ( (nCH_REF_NO > nBian1Logicalstart + nBianProbeNum - 1) && (nCH_REF_NO < nBian2Logicalstart) )
		{
			continue;
		}
		if ( (nCH_REF_NO > nBian2Logicalstart + nBianProbeNum - 1) && (nCH_REF_NO < nNei1Logicalstart) )
		{
			continue;
		}
		if ( (nCH_REF_NO > nNei1Logicalstart + nNei1ProbeNum - 1) && (nCH_REF_NO < nNei2Logicalstart) )
		{
			continue;
		}
		if (nCH_REF_NO > nNei2Logicalstart + nNei2ProbeNum - 1)
		{
			continue;
		}
		//chenyuzhu add end on 2014-10-22

		g_GainSale[nWuLiNum] = SheBeiCanShu.ChannelParam[i].fZengyixishu;
		fBEGain = fBEGain * g_GainSale[nWuLiNum];
		fFEGain = fFEGain * g_GainSale[nWuLiNum];

		pCHX_INIT_CONFIG[nWuLiNum].CH_DEM_MODE = 0;
		pCHX_INIT_CONFIG[nWuLiNum].id = nID;
		pCHX_INIT_CONFIG[nWuLiNum].MIDDLE_VALUE = nMIDDLE_VALUE;

	//	pCHX_INIT_CONFIG[nWuLiNum].CH_FRONTTIME = Distance2Data(SheBeiCanShu.ChannelParam[i].fShangmangqu, VSOUND);//闸门前沿chenyuzhu
		//闸门前沿
		pCHX_INIT_CONFIG[nWuLiNum].CH_FRONTTIME = SheBeiCanShu.ChannelParam[i].fShangmangqu / g_nSpeed * g_nADCFreq * 2;	
		//闸门后沿
		pCHX_INIT_CONFIG[nWuLiNum].CH_REARTIME = (g_Info[nPlate].height - SheBeiCanShu.ChannelParam[i].fXiamangqu)/g_nSpeed*g_nADCFreq*2;
		//底波闸门后沿
		pCHX_INIT_CONFIG[nWuLiNum].CH_MAXBACKTIME = (g_Info[nPlate].height -SheBeiCanShu.ChannelParam[i].fXiamangqu + SheBeiCanShu.ChannelParam[i].fDibozhamenkuandu)/g_nSpeed*g_nADCFreq*2;


		pCHX_INIT_CONFIG[nWuLiNum].CH_BEMINDATA=100;
		float fGainDiff =  fFEGain - fBEGain;
		if (fGainDiff > 17)
		{
			QString str = QString("%1%2%3%4").arg(nCH_REF_NO).arg(QObject::tr(" 通道伤波底波增益差为 ")).arg(fGainDiff).arg(QObject::tr("，超出限制"));
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
		}

		//pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_DIFF = Gain2Data(fFEGain - fBEGain);
		pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_DIFF = Gain2Data(14.0f);
		pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_A = Gain2Data(REF_GAIN);
		pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_B = Gain2Data(REF_GAIN);

		float gain = fBEGain;

		float gain_c,gain_d;
		float nOffset = 2.1f;
		float nSection = 29.7f;

		if( gain < nSection)
		{
			gain_c = gain + nOffset;
			gain_d = nOffset;
		}
		else
		{
			gain_c = nSection + nOffset;
			gain_d = gain - nSection + nOffset;
		}

		allCH_CONFIG[nWuLiNum].CH_GAIN_C = Gain2Data(gain_c);
		allCH_CONFIG[nWuLiNum].CH_GAIN_D = Gain2Data(gain_d);
		//allCH_CONFIG[nWuLiNum].CH_GAIN_C = wGainData / 2;
		//allCH_CONFIG[nWuLiNum].CH_GAIN_D = wGainData - wGainData / 2;

		//pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_C = Gain2Data(fBEGain / 2.0);
		//pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_D = Gain2Data(fBEGain / 2.0);
		float fRejectValue = fCH_REJECT_VALUE;
		pCHX_INIT_CONFIG[nWuLiNum].CH_REJECT_VALUE = fRejectValue * pow(2.0,g_nADCaiYangShu - 1) / 100.0;
		
		//pCHX_INIT_CONFIG[nWuLiNum].CH_ZEROCORRECT = fCH_ZEROCORRECT / g_nSpeed * g_nADCFreq * 2;
		pCHX_INIT_CONFIG[nWuLiNum].CH_ZEROCORRECT = fCH_ZEROCORRECT * g_nADCFreq * 0.000001; //chenyuzhu modify on 2014-10-10
		
		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_GAIN_MUX = 0;
		pCHX_INIT_CONFIG[nWuLiNum].CH_BE_GAIN_MUX = 0;
		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_DEF = Gain2Data(10.0);              /* 需要用数据库参数修改*/

		strCH_TGC_DAC_EN = strCH_TGC_DAC_EN.trimmed();

		pCHX_INIT_CONFIG[nWuLiNum].CH_TGC_DAC_EN = ((strCH_TGC_DAC_EN==QObject::tr("开"))?1:0);//判断通道是否是开
		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_SURF_DISTANCE = fCH_FLAW_SURF_DISTANCE / g_nSpeed * g_nADCFreq * 2;
		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_BE_DISTANCE = fCH_FLAW_BE_DISTANCE / g_nSpeed * g_nADCFreq * 2;
		strCH_EN = strCH_EN.trimmed();
		pCHX_INIT_CONFIG[nWuLiNum].CH_EN = ((strCH_EN == QObject::tr("开")) ? 1:0 );

		strAUTO_GAIN_EN = strAUTO_GAIN_EN.trimmed();
		pCHX_INIT_CONFIG[nWuLiNum].AUTO_GAIN_EN = ((strAUTO_GAIN_EN == QObject::tr("开")) ? 1:0 );
		strAUTO_GATE_EN = strAUTO_GATE_EN.trimmed();
		pCHX_INIT_CONFIG[nWuLiNum].AUTO_GATE_EN=((strAUTO_GATE_EN == QObject::tr("开")) ? 1:0 );
		strALARM_EN = strALARM_EN.trimmed();
		pCHX_INIT_CONFIG[nWuLiNum].ALARM_EN = ((strALARM_EN == QObject::tr("开")) ? 1:0 );
		pCHX_INIT_CONFIG[nWuLiNum].CH_REF_NO = nCH_REF_NO;

		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_DEF = 0;
		pCHX_INIT_CONFIG[nWuLiNum].CH_SELFCHECK = 0;
		pCHX_INIT_CONFIG[nWuLiNum].CH_MONITOR_RANGE = MONITOR_SIZE;
		pCHX_INIT_CONFIG[nWuLiNum].CH_MONITOR_RANGE = 1024;
		pCHX_INIT_CONFIG[nWuLiNum].CH_NO = nWuLiNum; 
		//给一些后面要用的map赋值  ??????????
		LogicChannelQianMangQuMap[pCHX_INIT_CONFIG[nWuLiNum].CH_REF_NO] = fCH_FRONTTIME;
		LogicChannelHouMangQuMap[pCHX_INIT_CONFIG[nWuLiNum].CH_REF_NO] = fCH_REARTIME;
		LogicChannelMaxDiShiMap[pCHX_INIT_CONFIG[nWuLiNum].CH_REF_NO] = g_Info[nPlate].height + fCH_MAXBACKTIME;
		int RefMun = nCH_REF_NO;
		m_Array2LogicChannelMap[nWuLiNum] = RefMun;
		m_LogicChannel2ArrayMap[RefMun] = nWuLiNum;	
	}
	//chenyuzhu add end on 2014-7-7

	// 其余未赋值的，参数拷贝0通道，通道开关设为关
	for (UINT i=0; i < MAX_CHANNEL_NO; i++)
	{
		if (pCHX_INIT_CONFIG[i].CH_REF_NO == 0)
		{
			memcpy(&(pCHX_INIT_CONFIG[i]), &(pCHX_INIT_CONFIG[0]), sizeof(CH_CONFIG));
			pCHX_INIT_CONFIG[i].CH_EN = 0;
		}

		// 根据配置文件DAC开关,设置DAC开关项
		pCHX_INIT_CONFIG[i].CH_TGC_DAC_EN = g_IniParams.m_nDACOpen;
	}
	QString strGongYiName = g_strGongYi;
	strGongYiName = strGongYiName.trimmed();
	
	//chenyuzhu modify start on 2014-7-8
	//strSQL = QString("%1").arg("select t_Detector.Max_P_Distance as Max_P_Distance,t_Detector.Max_N_Distance as Max_N_Distance from t_Detector join t_TanShangGongYi on t_Detector.name = t_TanShangGongYi.Detector");
	//query.clear();
	//query.exec(strSQL);
	//query.next();
	//float fMaxPDistance = query.value(0).toFloat();
	//float fMaxNDistance = query.value(1).toFloat();
	float fMaxPDistance = SheBeiCanShu.DetectorParam.fMax_P_Distance;
	float fMaxNDistance = SheBeiCanShu.DetectorParam.fMax_N_Distance;
	//chenyuzhu modify end on 2014-7-8
	
	pCHX_CONFIG_GLOBAL->DETECT_STANDARD = 1;					//1代表某种标准
	pCHX_CONFIG_GLOBAL->ALARM_VALUE = 1;
	pCHX_CONFIG_GLOBAL->K_VALUE = 0;
//	pCHX_CONFIG_GLOBAL->COMPENSATE_VALUE = g_nCeHouBuChang;  在工艺配置完成后g_nCeHouBuChang赋值，现在还没有
	pCHX_CONFIG_GLOBAL->SOUND_VELOCITY = fSpeed;
	pCHX_CONFIG_GLOBAL->THICKNESS_POS_DIFF_RANGE = Distance2Data(fMaxPDistance, VSOUND);
	pCHX_CONFIG_GLOBAL->THICKNESS_NEG_DIFF_RANGE = Distance2Data(fMaxNDistance, VSOUND);
	pCHX_CONFIG_GLOBAL->THICKNESS_STANDARD = Distance2Data(g_Info[nPlate].height, VSOUND); //50mm
	pCHX_CONFIG_GLOBAL->CHIP1_SAMPLE_SELECT = 0x0;					//CH2 FLAW DATA
	pCHX_CONFIG_GLOBAL->CHIP1_SAMPLE_ZEROCORRECT = 400;				//单位mm
	pCHX_CONFIG_GLOBAL->CHIP1_SYN_SIG_SELECT = 1;					//extern signal
	pCHX_CONFIG_GLOBAL->CHIP1_SYN_4K = 1;
	pCHX_CONFIG_GLOBAL->CHIP1_SYN_500 = 1;	                
	pCHX_CONFIG_GLOBAL->CHIP2_SAMPLE_SELECT = 0x0;					//CH2 FLAW DATA
	pCHX_CONFIG_GLOBAL->CHIP2_SAMPLE_ZEROCORRECT = 400;
	pCHX_CONFIG_GLOBAL->CHIP2_SYN_SIG_SELECT = 1;					//extern signal
	pCHX_CONFIG_GLOBAL->CHIP2_SYN_4K = 1;
	pCHX_CONFIG_GLOBAL->CHIP2_SYN_500 =1;
	pCHX_CONFIG_GLOBAL->LOCAL_BUS_CARD_NO = 1;
	pCHX_CONFIG_GLOBAL->BE_ALARM_VALUE = pCHX_CONFIG_GLOBAL->ALARM_VALUE;

	QString strCurrentPath = QDir::currentPath() + "/channelglobal.txt";
	QFile file(strCurrentPath);
	bool bRet = file.open(QIODevice::ReadOnly);
	if (!bRet)
	{
		MessageBox(NULL,L"打开channelglobal.txt文件失败",L"INIT_CONFIG",MB_OK);
		return;
	}
	while (!file.atEnd())
	{
		QByteArray line = file.readLine();
		QString strTemp = QString::fromLocal8Bit(line.data());
		int index = strTemp.indexOf(" ");
		if (index == -1)
		{
			break;
		}
		QString strTitle = strTemp.left(index);
		QString strValue = strTemp.right(strTemp.length()-index-1);
		strValue = strValue.trimmed();
		int nValue = strValue.toInt();

		if (strTitle == QObject::tr("AUTO_GAIN_RANGE_MIN"))
		{
			pCHX_CONFIG_GLOBAL->AUTO_GAIN_RANGE_MIN = nValue;
		}
		else if (strTitle == QObject::tr("AUTO_GAIN_RANGE_MAX"))
		{
			pCHX_CONFIG_GLOBAL->AUTO_GAIN_RANGE_MAX = nValue;
		}
		else if (strTitle == QObject::tr("BE_MIDDLE_VALUE"))
		{
			pCHX_CONFIG_GLOBAL->BE_MIDDLE_VALUE = nValue;
		}
		else if (strTitle == QObject::tr("AUTO_GAIN_Threshold"))
		{
			pCHX_CONFIG_GLOBAL->AUTO_GAIN_Threshold = nValue;
		}
		else if (strTitle == QObject::tr("AUTO_GATE_RANGE_MIN"))
		{
			pCHX_CONFIG_GLOBAL->AUTO_GATE_RANGE_MIN = nValue;
		}
		else if (strTitle == QObject::tr("AUTO_GATE_RANGE_MAX"))
		{
			pCHX_CONFIG_GLOBAL->AUTO_GATE_RANGE_MAX = nValue - pCHX_INIT_CONFIG[0].CH_BEMINDATA;
		}
		else if (strTitle == QObject::tr("AUTO_GATE_Threshold"))
		{
			pCHX_CONFIG_GLOBAL->AUTO_GATE_Threshold = nValue;
		}
		else if (strTitle == QObject::tr("FLAW_JUDGE_Threshold"))
		{
			pCHX_CONFIG_GLOBAL->FLAW_JUDGE_Threshold = nValue;
		}
		else if (strTitle == QObject::tr("START_DETECT_TIME"))
		{
			pCHX_CONFIG_GLOBAL->START_DETECT_TIME = nValue;
		}
		else if (strTitle == QObject::tr("AUTO_GAIN_RANGE_NEG_MIN"))
		{
			pCHX_CONFIG_GLOBAL->AUTO_GAIN_RANGE_NEG_MIN = nValue;
		}
		else if (strTitle == QObject::tr("AUTO_GAIN_RANGE_NEG_MAX"))
		{
			pCHX_CONFIG_GLOBAL->AUTO_GAIN_RANGE_NEG_MAX = nValue;
		}
		else if (strTitle == QObject::tr("AUTO_GAIN_DB_RANGE_SUB"))
		{
			pCHX_CONFIG_GLOBAL->AUTO_GAIN_DB_RANGE_SUB = nValue;
		}
		else if (strTitle == QObject::tr("AUTO_GAIN_DB_RANGE_ADD"))
		{
			pCHX_CONFIG_GLOBAL->AUTO_GAIN_DB_RANGE_ADD = nValue;
		}
		else if (strTitle == QObject::tr("AD_SAMPLE_VALUE_MAX"))
		{
			pCHX_CONFIG_GLOBAL->AD_SAMPLE_VALUE_MAX = nValue;
		}
		else if (strTitle == QObject::tr("AD_SAMPLE_MIDDLE_VALUE"))
		{
			pCHX_CONFIG_GLOBAL->AD_SAMPLE_MIDDLE_VALUE = nValue;
		}
		else if (strTitle == QObject::tr("FLAWTIME_RANGE"))
		{
			pCHX_CONFIG_GLOBAL->FLAWTIME_RANGE = nValue;
		}
		else if (strTitle == QObject::tr("BE_JUDGE_Threshold"))
		{
			pCHX_CONFIG_GLOBAL->BE_JUDGE_Threshold = nValue;
		}
		else if (strTitle == QObject::tr("HEIGHT_JUDGE_Threshold"))
		{
			pCHX_CONFIG_GLOBAL->HEIGHT_JUDGE_Threshold = nValue;
		}
		else
		{
		}					
	}
	file.close();
}
//chenyuzhu modify end on 2014-7-8

void InitConfig(CH_CONFIG *pCHX_INIT_CONFIG,QSqlDatabase *&pDatabaseConnection,CPlateInfo *&pPlateInfo)
{
	QString strID;
	QString strCH_DEM_MODE;
	QString strMIDDLE_VALUE;
	QString strCH_FRONTTIME;
	QString strCH_REARTIME;
	QString strCH_MAXBACKTIME;
	QString strCH_BEMINDATA;
	QString strCH_TGC_DA_START;
	QString strFEGain;
	QString strBEGain;
	QString strCH_GAIN_A;
	QString strCH_GAIN_B;
	QString strCH_GAIN_C;
	QString strCH_GAIN_D;
	QString strCH_REJECT_VALUE;
	QString strCH_ZEROCORRECT;
	QString strCH_FLAW_GAIN_MUX;
	QString strCH_BE_GAIN_MUX;
	QString strCH_TGC_DAC_EN;
	QString strCH_FLAW_SURF_DISTANCE;
	QString strCH_FLAW_BE_DISTANCE;
	QString strCH_EN;
	QString strAUTO_GAIN_EN;
	QString strAUTO_GATE_EN;
	QString strALARM_EN;
	QString strCH_REF_NO;
	QString strCH_FLAW_DEF;
	QString strCH_SELFCHECK;
	QString strCH_MONITOR_RANGE;
	QString strCH_NO;
	QString strSpeed;

	int nADCFreq = 0;
	int nADCaiYangShu = 0;
	int nSpeed = 0;	

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	QSqlQuery sqlQuery(*pDatabaseConnection);
	QString strQuery = QString("%1").arg("select ADCFreq,ADCaiYangShu from t_YiQi where name='PUTS-ARS144'");
	sqlQuery.exec(strQuery);

	QString strADCFreq;
	QString strADCaiYangShu;

	while(sqlQuery.next())
	{
		strADCFreq = sqlQuery.value(0).toString();
		strADCaiYangShu = sqlQuery.value(1).toString();
		//qDebug()<<strADCFreq<<strADCaiYangShu;
	}

	nADCFreq = strADCFreq.toInt()*1000000;	// 现在strADCFreq以MHz为单位
	nADCaiYangShu = strADCaiYangShu.toInt();

	strQuery = QString("%1%2%3%4").arg("select t_Channel.id as id,t_TanShangGongYi.JianBoMoShi as JianBoMoShi,\
									   t_TanShangGongYi.ShangMangQu as ShangMangQu,\
									   t_TanShangGongYi.XiaMangQu as XiaMangQu,\
									   t_TanShangGongYi.DACOpen as DACOpen,\
									   t_TanShangGongYi.AutoZengYiOpen as AutoZengYiOpen,\
									   t_TanShangGongYi.AutoHuaMenOpen as AutoHuaMenOpen,\
									   t_TanShangGongYi.BaoJing as BaoJing,\
									   t_Channel.Num as Num,\
									   t_Channel.RefNum as RefNum,\
									   t_Channel.ZhongZhiDian as ZhongZhiDian,\
									   t_Channel.IsOpen as IsOpen,\
									   t_Channel.ICIV as ICIV,\
									   t_Channel.TCIV as TCIV,\
									   t_Channel.Delay as Delay,\
									   t_Channel.HuaMenQianYan as HuaMenQianYan,\
									   t_Channel.HuaMenHouYan as HuaMenHouYan,\
									   t_Channel.DiBoHuaMenHouYan as DiBoHuaMenHouYan,\
									   t_Channel.DiBoYuZhi as DiBoYuZhi,\
									   t_Channel.YiZhi as YiZhi,\
									   t_PlateType.LWaveSpeed as LWaveSpeed,\
									   t_PlateType.TWaveSpeed as TWaveSpeed \
									   from t_Channel join t_TanShangGongYi on t_Channel.TanShangGongYi=t_TanShangGongYi.name \
									   join t_PlateType on t_PlateType.name=t_TanShangGongYi.PlateType \
									   where t_TanShangGongYi.name=").arg("'").arg(pPlateInfo->m_strArtName).arg("'").arg("order by t_Channel.Num");
	sqlQuery.exec(strQuery);
	while (sqlQuery.next())
	{
		strID = sqlQuery.value(0).toString();
		strCH_DEM_MODE = sqlQuery.value(1).toString();
		strCH_FLAW_SURF_DISTANCE = sqlQuery.value(2).toString();
		strCH_FLAW_BE_DISTANCE = sqlQuery.value(3).toString();
		strCH_TGC_DAC_EN = sqlQuery.value(4).toString();
		strAUTO_GAIN_EN = sqlQuery.value(5).toString();
		strAUTO_GATE_EN = sqlQuery.value(6).toString();
		strALARM_EN = sqlQuery.value(7).toString();
		strCH_NO = sqlQuery.value(8).toString();
		strCH_REF_NO = sqlQuery.value(9).toString();
		strMIDDLE_VALUE = sqlQuery.value(10).toString();
		strCH_EN = sqlQuery.value(11).toString();
		strFEGain = sqlQuery.value(12).toString();
		strBEGain = sqlQuery.value(13).toString();
		strCH_ZEROCORRECT = sqlQuery.value(14).toString();
		strCH_FRONTTIME = sqlQuery.value(15).toString();
		strCH_REARTIME = sqlQuery.value(16).toString();
		strCH_MAXBACKTIME = sqlQuery.value(17).toString();
		strCH_BEMINDATA = sqlQuery.value(18).toString();
		strCH_REJECT_VALUE = sqlQuery.value(19).toString();
		strSpeed = sqlQuery.value(20).toString();
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		strCH_DEM_MODE = strCH_DEM_MODE.trimmed();
		int nWuLiNum = strCH_NO.toInt();
		//qDebug()<<nWuLiNum;
		nSpeed = strSpeed.toInt()*1000; //应该是全局变量暂且这样写
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		pCHX_INIT_CONFIG[nWuLiNum].CH_DEM_MODE = 0;
		pCHX_INIT_CONFIG[nWuLiNum].id = strID.toInt();
		pCHX_INIT_CONFIG[nWuLiNum].MIDDLE_VALUE = strMIDDLE_VALUE.toInt();
		pCHX_INIT_CONFIG[nWuLiNum].CH_FRONTTIME = strCH_FRONTTIME.toFloat() / nSpeed * nADCFreq * 2;	
		pCHX_INIT_CONFIG[nWuLiNum].CH_BEMINDATA = strCH_BEMINDATA.toFloat() * pow(2.0,nADCaiYangShu - 1) / 100.0;

		pCHX_INIT_CONFIG[nWuLiNum].CH_REARTIME = (g_Info[nPlate].height - strCH_REARTIME.toFloat())/nSpeed*nADCFreq*2;
		//底波闸门后沿
		pCHX_INIT_CONFIG[nWuLiNum].CH_MAXBACKTIME = (g_Info[nPlate].height + strCH_MAXBACKTIME.toFloat())/nSpeed*nADCFreq*2;

		float fGainDiff =  strFEGain.toFloat() - strBEGain.toFloat();
		if (fGainDiff > 17)
		{
			QString str = QString("%1%2%3%4").arg(strCH_REF_NO.toInt()).arg(QObject::tr(" 通道伤波底波增益差为 ")).arg(fGainDiff).arg(QObject::tr("，超出限制"));
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
		}
		
		pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_DIFF = Gain2Data(strFEGain.toFloat() - strBEGain.toFloat());

		pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_A = Gain2Data(REF_GAIN);
		pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_B = Gain2Data(REF_GAIN);
		pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_C = Gain2Data(strBEGain.toFloat() / 2.0);
		pCHX_INIT_CONFIG[nWuLiNum].CH_GAIN_D = Gain2Data(strBEGain.toFloat() / 2.0);
		float fRejectValue = strCH_REJECT_VALUE.toFloat();
		pCHX_INIT_CONFIG[nWuLiNum].CH_REJECT_VALUE = fRejectValue * pow(2.0,nADCaiYangShu - 1) / 100.0;
		pCHX_INIT_CONFIG[nWuLiNum].CH_ZEROCORRECT = strCH_ZEROCORRECT.toFloat() / nSpeed * nADCFreq * 2;
		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_GAIN_MUX = 0;//atoi(strCH_FLAW_GAIN_MUX.GetBuffer(1024));
		pCHX_INIT_CONFIG[nWuLiNum].CH_BE_GAIN_MUX = 0;//atoi(strCH_BE_GAIN_MUX.GetBuffer(1024));
		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_DEF = Gain2Data(10.0);  /* 需要用数据库参数修改*/

		strCH_TGC_DAC_EN.trimmed();

		pCHX_INIT_CONFIG[nWuLiNum].CH_TGC_DAC_EN = ((strCH_TGC_DAC_EN==QObject::tr("开"))?1:0);//判断通道是否是开
		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_SURF_DISTANCE = strCH_FLAW_SURF_DISTANCE.toFloat() / nSpeed * nADCFreq * 2;
		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_BE_DISTANCE = strCH_FLAW_BE_DISTANCE.toFloat() / nSpeed * nADCFreq * 2;
		strCH_EN = strCH_EN.trimmed();
		pCHX_INIT_CONFIG[nWuLiNum].CH_EN = ((strCH_EN == QObject::tr("开")) ? 1:0 );

		strAUTO_GAIN_EN.trimmed();
		pCHX_INIT_CONFIG[nWuLiNum].AUTO_GAIN_EN = ((strAUTO_GAIN_EN == QObject::tr("开")) ? 1:0 );
		strAUTO_GATE_EN.trimmed();
		pCHX_INIT_CONFIG[nWuLiNum].AUTO_GATE_EN=((strAUTO_GATE_EN == QObject::tr("开")) ? 1:0 );
		strALARM_EN.trimmed();
		pCHX_INIT_CONFIG[nWuLiNum].ALARM_EN = ((strALARM_EN == QObject::tr("开")) ? 1:0 );
		pCHX_INIT_CONFIG[nWuLiNum].CH_REF_NO = strCH_REF_NO.toInt();

		pCHX_INIT_CONFIG[nWuLiNum].CH_FLAW_DEF = 0;//
		pCHX_INIT_CONFIG[nWuLiNum].CH_SELFCHECK = 0;//atoi(strCH_SELFCHECK.GetBuffer(1024));
		pCHX_INIT_CONFIG[nWuLiNum].CH_MONITOR_RANGE = MONITOR_SIZE;
		pCHX_INIT_CONFIG[nWuLiNum].CH_NO = nWuLiNum;//atoi(strCH_NO.GetBuffer(1024));  

// 		LogicChannelQianMangQuMap[pCHX_INIT_CONFIG[nWuLiNum].CH_REF_NO] = strCH_FRONTTIME.toFloat();
// 		LogicChannelHouMangQuMap[pCHX_INIT_CONFIG[nWuLiNum].CH_REF_NO] = strCH_REARTIME.toFloat();
// 		LogicChannelMaxDiShiMap[pCHX_INIT_CONFIG[nWuLiNum].CH_REF_NO] = g_Info[nPlate].height + 
// 			strCH_MAXBACKTIME.toFloat();
		//qDebug()<<nWuLiNum<<pCHX_INIT_CONFIG[nWuLiNum].CH_REF_NO<<pCHX_INIT_CONFIG[nWuLiNum].CH_TGC_DAC_EN;
		//qDebug()<<pCHX_INIT_CONFIG[nWuLiNum].CH_EN;
	}

	

	// 其余未赋值的，参数拷贝0通道，通道开关设为关
	for (UINT i=0; i < MAX_CHANNEL_NO; i++)
	{
		if (pCHX_INIT_CONFIG[i].CH_REF_NO == 0)
		{
			memcpy(&(pCHX_INIT_CONFIG[i]), &(pCHX_INIT_CONFIG[0]), sizeof(CH_CONFIG));
			pCHX_INIT_CONFIG[i].CH_EN = 0;
		}

		// 根据配置文件DAC开关,设置DAC开关项
		//pCHX_INIT_CONFIG[i].CH_TGC_DAC_EN = g_IniParams.m_nDACOpen;
	}

}

// 获取 Z方向过滤深度
float GetGuoLvZ(const float& fX, const float& fY)
{
	if (g_DepthFilterSwitch) {
		if (fX < g_IniParams.m_fFrontLength) {	// 头部伤点过滤Z方向最大值
			return g_IniParams.m_fFrontMinZ;
		} else if (fX >  (g_Info[nPlate].length - g_IniParams.m_fBackLength)) {
			return g_IniParams.m_fBackMinZ;
		} else if ((fY < g_IniParams.m_fEdgeLength) || (fY > (g_Info[nPlate].width - g_IniParams.m_fEdgeLength))) {
			return g_IniParams.m_fEdgeMinZ;
		} else {
			return g_IniParams.m_fminZ;
		}
	}

	return 0.0f;
}

bool IsValidData(const float &fX, const float &fY, const float &fZ)
{
	if (g_DepthFilterSwitch) {
		if (fX < g_IniParams.m_fFrontLength) {	// 头部伤点过滤Z方向最大值
			if (fZ >= g_IniParams.m_fFrontMinZ && fZ <= g_IniParams.m_fFrontMinZEnd) {
				return TRUE;
			}
		} else if (fX >  (g_Info[nPlate].length - g_IniParams.m_fBackLength)) {
			if (fZ >= g_IniParams.m_fBackMinZ && fZ <= g_IniParams.m_fBackMinZEnd) {
				return TRUE;
			}
		} else if ((fY < g_IniParams.m_fEdgeLength) || (fY > (g_Info[nPlate].width - g_IniParams.m_fEdgeLength))) {
			if (fZ >= g_IniParams.m_fEdgeMinZ && fZ <= g_IniParams.m_fEdgeMinZEnd) {
				return TRUE;
			}
		} else {
			if (fZ >= g_IniParams.m_fminZ && fZ <= g_IniParams.m_fminZEnd) {
				return TRUE;
			}
		}
	} else {
		return TRUE;
	}

	return FALSE;
}

/** 
* 画标尺
* @since     1.0
*/
//nTextLocation为1时，刻度画在区域的底部；为0时，刻度画在区域的顶部
void DrawHRuler(QPaintDevice *device,const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection)
{

	QPainter painter(device);
	painter.setPen(Qt::black);

	int iStartPos = 0;  

	int iLLineStart,iLLineEnd;
	int iMLineStart,iMLineEnd;
	int iSLineStart,iSLineEnd;

	QRect rectText;
	if( nTextLocation == 1 ) 
	{

		rectText.setTop(rect.bottom()- 25);
		rectText.setBottom(rect.bottom()- 10);

		iLLineStart = rect.bottom() - 9;
		iLLineEnd	 = rect.bottom() - 2;

		iMLineStart = rect.bottom() - 6;
		iMLineEnd	 = rect.bottom() - 2;

		iSLineStart = rect.bottom() - 4;
		iSLineEnd	 = rect.bottom() - 2;

	}
	else if(nTextLocation == 0)
	{

		rectText.setTop(rect.top()+20);
		rectText.setBottom(rect.top()+ 35);

		iLLineStart = rect.top() + 2;
		iLLineEnd = rect.top() + 18;

		iMLineStart = rect.top() + 2;
		iMLineEnd = rect.top() + 12;

		iSLineStart = rect.top() + 2;
		iSLineEnd = rect.top() + 8;
	}

	QString str;
	int iPos;
	int iSel = 0;
	int nLinestart = 0;

	bool bXiaoyu = FALSE;
	int  i = 0;
	int  iSayac = 0;

	if (nStep < 10)
	{
		nStart = nStart*100;
		nEnd = nEnd*100;
		nStep = nStep*100;
		bXiaoyu = TRUE;
	}

	nStep = nStep/10;

	if (nStep%5 != 0)
	{
		nStep = (nStep/5 + 1)*5;
	}

	nLinestart = (nStart/nStep + 1)*nStep; 
	if (nStart == 0)
	{
		nLinestart = 0;
	}

	for (i = nLinestart;i < nEnd; i+=nStep)
	{
		if (i %nStep == 0)
		{
			if (nTextDirection == 0)
			{
				iPos = rect.left() + ((rect.width())*(i - nStart))/(nEnd - nStart);
				rectText.setLeft(iPos - 50);
			}
			else
			{
				iPos = rect.right() - ((rect.width())*(i - nStart))/(nEnd - nStart);
				rectText.setLeft(iPos-50);
			}

			if (i % (nStep*5) == 0)
			{
				painter.drawLine(iPos,iMLineStart,iPos,iMLineEnd);	
				if (i % (nStep*10) == 0)
				{
					painter.drawLine(iPos,iLLineStart,iPos,iLLineEnd);	

					rectText.setRight(iPos+60);  
					if (rectText.right() > rect.right())
					{
						rectText.setRight(rect.right()+25);
					}
					if (bXiaoyu)
					{
						str.setNum(i/100);
					}
					else	
					{
						str.setNum(i);
					}
					if(   nTextLocation == 1 ) 
					{
						painter.drawText( rectText, Qt::AlignHCenter ,str);
					}
					else
					{
						painter.drawText( rectText, Qt::AlignHCenter ,str);
					}

					continue;
				}
				continue;
			}
			painter.drawLine(iPos,iSLineStart,iPos,iSLineEnd);
		}
	}
	return;
}


/** 
* 画标尺
* @since     1.0
*/
void DrawVRuler(QPaintDevice *device,const QRect& rect,int nStart, int nEnd, int nStep, int nTextLocation,int nTextDirection)
{
	QPainter painter(device);
	painter.setPen(Qt::black);
	if (nEnd == 20)
	{
		int test = 0;
	}
	QRect rectText;
	int nNumber = 0;

	int iLLineStart , iLLineEnd;
	int iMLineStart , iMLineEnd;
	int iSLineStart , iSLineEnd;

	if( nTextLocation == 1 ) 
	{
		rectText.setRight(rect.right()-10);

		iLLineStart = rect.right() - 18;
		iLLineEnd	 = rect.right() - 2;

		iMLineStart = rect.right() - 12;
		iMLineEnd	 = rect.right() - 2;

		iSLineStart = rect.right() - 8;
		iSLineEnd	 = rect.right() - 2;

	}
	else if( nTextLocation == 0 ) 
	{
		rectText.setLeft(rect.left()+10);

		iLLineStart = rect.left()  + 2;
		iLLineEnd	 = rect.left()  + 18;

		iMLineStart = rect.left()  + 2;
		iMLineEnd	 = rect.left()  + 12;

		iSLineStart = rect.left() + 2;
		iSLineEnd	 = rect.left() + 8;
	}

	QString str;

	int iPos;


	bool bXiaoyu = FALSE;
	if (nStep < 10)
	{
		nStart = nStart*100;
		nEnd = nEnd*100;
		nStep = nStep*100;
		bXiaoyu = TRUE;
	}
	nStep = nStep/10;
	if (nStep%5 != 0)
	{
		if (nStep > 3)
		{
			nStep = (nStep/5 + 1)*5;
		}
		else
		{

		}
	}


	int nLinestart = (nStart/nStep + 1)*nStep;
	if (nStart == 0)
	{
		nLinestart = 0;
	}

	if (nEnd == nStart)
	{
		nEnd = nStart + 100;
	}

	for (int i = nLinestart;i<=nEnd; i+=nStep)
	{
		if (i %nStep == 0)
		{
			if( nTextDirection == 0 )  
			{
				// TODO 点伤文件解压缩失败会崩溃
				iPos = rect.top() + (rect.height()*(i - nStart))/(nEnd - nStart);				
				rectText.setTop(iPos-10);
			}
			else if(nTextDirection == 1 )
			{
				iPos = rect.bottom() - (rect.height()*(i - nStart))/(nEnd - nStart);				
				rectText.setTop(iPos-13);
			}
			rectText.setBottom(rectText.top()+20);

			if (i % (nStep*5) == 0)
			{
				painter.drawLine(iMLineStart,iPos,iMLineEnd,iPos);		
				if (i % (nStep*10) == 0)
				{
					painter.drawLine(iLLineStart,iPos,iLLineEnd,iPos);	
					if( nTextLocation == 0 )
					{
						rectText.setRight(rectText.left()+32);
					}
					else
					{
						rectText.setLeft(rectText.right()-30);
					}

					if (rectText.top() < rect.top())
					{
						rectText.setTop(rect.top()+1);
					}
					if (rectText.bottom() > rect.bottom())
					{
						rectText.setBottom(rect.bottom()+10);
					}
					if (bXiaoyu)
					{
						nNumber=i/100;
						// 						if (nNumber<0)
						// 						{
						// 							nNumber=-nNumber;
						// 						}
						str.setNum(nNumber);
					}
					else	
					{
						str.setNum(i);
					}
					if( nTextLocation == 0 )
					{
						painter.drawText( rectText,Qt::AlignHCenter,str  );
					}
					else
					{
						painter.drawText( rectText,Qt::AlignHCenter,str );
					}

					continue;
				}
			}
			painter.drawLine(iSLineStart,iPos,iSLineEnd,iPos);		

		}
	}
	return;
}


void CalcGridStep(const PlaneFlawHeader& PFH, float& fGridXStep, float& fGridYStep)
{
	if (PFH.max_x < g_IniParams.m_fNeiFrontLength || 
	PFH.min_x >(g_Info[nPlate].length - g_IniParams.m_fNeiTailLength))
	{
		fGridXStep = g_IniParams.m_minYDistance;
		fGridYStep = g_IniParams.m_minXDistance;
	}
	else
	{
		fGridXStep = g_IniParams.m_minXDistance;
		fGridYStep = g_IniParams.m_minYDistance;
	}
}

void FindBian(QList<HPOINT>& listResource,QList<HPOINT>& listResult,float fXStep, float fYStep )
{
	if (listResource.size() == 0  ) 
	{
		return;
	}

	float m_DistanceX = fXStep*2;
	float m_DistanceY = fYStep*2;

	float m_PlateWidth = g_Info[nPlate].length;
	float m_PlateHeight = g_Info[nPlate].width;

	if (listResource.size() == 1  )
	{
		QList<HPOINT>::iterator itPoint1 = listResource.begin();
		HPOINT p = *itPoint1,pTemp = *itPoint1;

		pTemp.x = (p.x + m_DistanceX/2)<m_PlateWidth?(p.x + m_DistanceX/2):m_PlateWidth;
		pTemp.y = (p.y +  m_DistanceY/2)<m_PlateHeight?(p.y +  m_DistanceY/2):m_PlateHeight;
		listResult.push_back(pTemp);
		pTemp.x = (p.x - m_DistanceX/2)>0?(p.x - m_DistanceX/2):0;
		pTemp.y = (p.y + m_DistanceY/2)<m_PlateHeight?(p.y +  m_DistanceY/2):m_PlateHeight;
		listResult.push_back(pTemp);
		pTemp.x = (p.x - m_DistanceX/2)>0?(p.x - m_DistanceX/2):0;
		pTemp.y = (p.y -  m_DistanceY/2)>0?(p.y -  m_DistanceY/2):0;
		listResult.push_back(pTemp);
		pTemp.x = (p.x + m_DistanceX/2)<m_PlateWidth?(p.x + m_DistanceX/2):m_PlateWidth;
		pTemp.y = (p.y -  m_DistanceY/2)>0?(p.y -  m_DistanceY/2):0;
		listResult.push_back(pTemp);
		return;
	}

	if (listResource.size() == 2  )
	{
		QList<HPOINT>::iterator itPoint1 = listResource.begin();
		HPOINT p1 = *itPoint1,p2 = *itPoint1,pTemp = *itPoint1;
		p1 = *itPoint1++;
		p2 = *itPoint1;

		pTemp.type = p1.type;
		pTemp.z      = p1.z;
		if (p1.x == p2.x)
		{
			pTemp.x = (p1.x - m_DistanceX/2)>0?(p1.x - m_DistanceX/2):0;
			pTemp.y = (p1.y -  m_DistanceY/2)>0?(p1.y -  m_DistanceY/2):0;
			listResult.push_back(pTemp);
			pTemp.x = (p1.x + m_DistanceX/2)<m_PlateWidth?(p1.x + m_DistanceX/2):m_PlateWidth;
			pTemp.y = (p1.y -  m_DistanceY/2)>0?(p1.y -  m_DistanceY/2):0;
			listResult.push_back(pTemp);
			pTemp.x = (p2.x + m_DistanceX/2)<m_PlateWidth?(p2.x + m_DistanceX/2):m_PlateWidth;
			pTemp.y = (p2.y +  m_DistanceY/2)<m_PlateHeight?(p2.y +  m_DistanceY/2):m_PlateHeight;
			listResult.push_back(pTemp);
			pTemp.x = (p2.x - m_DistanceX/2)>0?(p2.x - m_DistanceX/2):0;
			pTemp.y = (p2.y + m_DistanceY/2)<m_PlateHeight?(p2.y +  m_DistanceY/2):m_PlateHeight;
			listResult.push_back(pTemp);	
		}
		else if (p1.y == p2.y)
		{
			pTemp.x = (p1.x - m_DistanceX/2)>0?(p1.x - m_DistanceX/2):0;
			pTemp.y = (p1.y -  m_DistanceY/2)>0?(p1.y -  m_DistanceY/2):0;
			listResult.push_back(pTemp);
			pTemp.x = (p2.x + m_DistanceX/2)<m_PlateWidth?(p2.x + m_DistanceX/2):m_PlateWidth;
			pTemp.y = (p2.y -  m_DistanceY/2)>0?(p2.y -  m_DistanceY/2):0;
			listResult.push_back(pTemp);
			pTemp.x =  (p2.x + m_DistanceX/2)<m_PlateWidth?(p2.x + m_DistanceX/2):m_PlateWidth;
			pTemp.y = (p2.y + m_DistanceY/2)<m_PlateHeight?(p2.y +  m_DistanceY/2):m_PlateHeight;
			listResult.push_back(pTemp);
			pTemp.x = (p1.x - m_DistanceX/2)>0?(p1.x - m_DistanceX/2):0;
			pTemp.y = (p1.y + m_DistanceY/2)<m_PlateHeight?(p1.y +  m_DistanceY/2):m_PlateHeight;
			listResult.push_back(pTemp);	
		}
		else
		{
			//			ASSERT(0);
		}

		return;
	}

	int nXiangXian1,nXiangXian2;
	HPOINT p1,p2,p3,pFirst,pSecond,pTemp = {0};
	QList<HPOINT>::iterator itPoint1,itPoint2,itPoint3;

	itPoint1 = listResource.begin();
	p1  = p2 = p3 = pFirst = pSecond = pTemp = *itPoint1;

	itPoint2 = 	itPoint1;
	itPoint2++;
	pSecond = *itPoint2;
	int i = 0;
	for (;itPoint1 != listResource.end();itPoint1  = itPoint2)
	{
		i++;
		p1 = *itPoint1;
		itPoint2 = 	itPoint1;
		itPoint2++;
		if (itPoint2 == listResource.end())
		{
			p2 = pFirst;
			p3 = pSecond;
		}
		else
		{
			p2 = *itPoint2;
			itPoint3 = itPoint2;
			itPoint3++;
			if (itPoint3 == listResource.end())
			{
				p3 = pFirst;
			}
			else
			{
				p3 = *itPoint3;
			}
		}

		if (p1.x == p2.x) 
		{
			nXiangXian1 = (p2.y > p1.y)?3:1;
		}
		else if (p1.y == p2.y)
		{
			nXiangXian1 = (p2.x > p1.x)?0:2;
		}
		else
		{
			//			ASSERT(0);
		}

		if (p3.x == p2.x) 
		{
			nXiangXian2 = (p2.y > p3.y)?1:3;
		}
		else if (p3.y == p2.y)
		{
			nXiangXian2 = (p2.x > p3.x)?2:0;
		}
		else
		{
			//			ASSERT(0);
		}

		switch(nXiangXian1)
		{
		case 0:
			switch(nXiangXian2)
			{
			case 0:
				//				ASSERT(0);
				break;
			case 1:
				pTemp.x = (p2.x - m_DistanceX/2)>0?(p2.x - m_DistanceX/2):0;
				pTemp.y = (p2.y -  m_DistanceY/2)>0?(p2.y -  m_DistanceY/2):0;
				break;
			case 2:
				pTemp.x = (p2.x + m_DistanceX/2)<m_PlateWidth?(p2.x + m_DistanceX/2):m_PlateWidth;
				pTemp.y = (p2.y -  m_DistanceY/2)>0?(p2.y -  m_DistanceY/2):0;
				listResult.push_back(pTemp);
				pTemp.x = (p2.x + m_DistanceX/2)<m_PlateWidth?(p2.x + m_DistanceX/2):m_PlateWidth;
				pTemp.y = (p2.y +  m_DistanceY/2)<m_PlateHeight?(p2.y +  m_DistanceY/2):m_PlateHeight;
				break;
			case 3:
				pTemp.x = (p2.x + m_DistanceX/2)<m_PlateWidth?(p2.x + m_DistanceX/2):m_PlateWidth;
				pTemp.y = (p2.y -  m_DistanceY/2)>0?(p2.y -  m_DistanceY/2):0;
				break;
			default:
				//				ASSERT(0);
				break;
			}

			break;
		case 1:
			switch(nXiangXian2)
			{
			case 0:
				pTemp.x = (p2.x - m_DistanceX/2)>0?(p2.x - m_DistanceX/2):0;
				pTemp.y = (p2.y -  m_DistanceY/2)>0?(p2.y -  m_DistanceY/2):0;
				break;
			case 1:
				//				ASSERT(0);
				break;
			case 2:
				pTemp.x = (p2.x - m_DistanceX/2)>0?(p2.x - m_DistanceX/2):0;
				pTemp.y = (p2.y + m_DistanceY/2)<m_PlateHeight?(p2.y +  m_DistanceY/2):m_PlateHeight;
				break;
			case 3:
				pTemp.x = (p2.x - m_DistanceX/2)>0?(p2.x - m_DistanceX/2):0;
				pTemp.y = (p2.y -  m_DistanceY/2)>0?(p2.y -  m_DistanceY/2):0;
				listResult.push_back(pTemp);
				pTemp.x = (p2.x + m_DistanceX/2)<m_PlateWidth?(p2.x + m_DistanceX/2):m_PlateWidth;
				pTemp.y = (p2.y -  m_DistanceY/2)>0?(p2.y -  m_DistanceY/2):0;
				break;
			default:
				//				ASSERT(0);
				break;
			}

			break;
		case 2:
			switch(nXiangXian2)
			{
			case 0:
				pTemp.x = (p2.x - m_DistanceX/2)>0?(p2.x - m_DistanceX/2):0;
				pTemp.y = (p2.y + m_DistanceY/2)<m_PlateHeight?(p2.y +  m_DistanceY/2):m_PlateHeight;
				listResult.push_back(pTemp);
				pTemp.x = (p2.x - m_DistanceX/2)>0?(p2.x - m_DistanceX/2):0;
				pTemp.y = (p2.y -  m_DistanceY/2)>0?(p2.y -  m_DistanceY/2):0;
				break;
			case 1:
				pTemp.x = (p2.x - m_DistanceX/2)>0?(p2.x - m_DistanceX/2):0;
				pTemp.y = (p2.y + m_DistanceY/2)<m_PlateHeight?(p2.y +  m_DistanceY/2):m_PlateHeight;
				break;
			case 2:
				//				ASSERT(0);
				break;
			case 3:
				pTemp.x = (p2.x + m_DistanceX/2)<m_PlateWidth?(p2.x + m_DistanceX/2):m_PlateWidth;
				pTemp.y = (p2.y +  m_DistanceY/2)<m_PlateHeight?(p2.y +  m_DistanceY/2):m_PlateHeight;
				break;
			default:
				//				ASSERT(0);
				break;
			}

			break;
		case 3:
			switch(nXiangXian2)
			{
			case 0:
				pTemp.x = (p2.x + m_DistanceX/2)<m_PlateWidth?(p2.x + m_DistanceX/2):m_PlateWidth;
				pTemp.y = (p2.y -  m_DistanceY/2)>0?(p2.y -  m_DistanceY/2):0;
				break;
			case 1:
				pTemp.x = (p2.x + m_DistanceX/2)<m_PlateWidth?(p2.x + m_DistanceX/2):m_PlateWidth;
				pTemp.y = (p2.y +  m_DistanceY/2)<m_PlateHeight?(p2.y +  m_DistanceY/2):m_PlateHeight;
				listResult.push_back(pTemp);
				pTemp.x = (p2.x - m_DistanceX/2)>0?(p2.x - m_DistanceX/2):0;
				pTemp.y = (p2.y + m_DistanceY/2)<m_PlateHeight?(p2.y +  m_DistanceY/2):m_PlateHeight;
				break;
			case 2:
				pTemp.x = (p2.x + m_DistanceX/2)<m_PlateWidth?(p2.x + m_DistanceX/2):m_PlateWidth;
				pTemp.y = (p2.y +  m_DistanceY/2)<m_PlateHeight?(p2.y +  m_DistanceY/2):m_PlateHeight;
				break;
			case 3:
				//				ASSERT(0);
				break;
			default:
				//				ASSERT(0);
				break;
			}

			break;
		default:
			//			ASSERT(0);
			break;
		}

		listResult.push_back(pTemp);
	}
}

bool CalculateFlawXY(const CH_FLAWDATA& FlawInfo, float& fX, float& fY, const DeltaX& deltaX)
{
	unsigned short nH8 = 0;
	unsigned int nL24 = 0;

	nH8 = (FlawInfo.CH_X >> 24) & 0xFF;
	nL24 = FlawInfo.CH_X & 0xFFFFFF;

	g_ItemMutex.lock();
	qint8 tmpCommand = g_ItemValue.dwPLC2WSCommand;
	g_ItemMutex.unlock();

	if (nH8 > 3 || nH8 == 0)
	{
		return FALSE;
	}

	int nBoxIndex = 0;
	int nProbeIndex = 0;
	int nJiOu = 0;
	if (FlawInfo.CH_NO >= 1000 && FlawInfo.CH_NO < 5000)
	{
		nBoxIndex = (FlawInfo.CH_NO%1000) / 3;
		nProbeIndex = (FlawInfo.CH_NO%1000) % 3;		
	}

	nJiOu = (nProbeIndex + 1) % 2;


	if (FlawInfo.CH_NO >= 1000 && FlawInfo.CH_NO < 2000)	// 边探1
	{
		fX = nL24 * (3.1415926 * g_IniParams.m_fCoder2Diameter / g_IniParams.m_nCoder2PerRingNum) - 
		 			deltaX.m_fCar1_Coder1_Distance - nJiOu * g_IniParams.m_fDistanceInBox;
		fY = (FlawInfo.CH_NO - 1000) * 20 + 10 + deltaX.m_fBianTan1DeltaX;
	}
	else if (FlawInfo.CH_NO>=2000 && FlawInfo.CH_NO<3000)	// 边探2-- 非参考边
	{	

		if (nH8 == 1)	// 头探
		{
			fX = (FlawInfo.CH_NO - 2000) * 20 + 10 + deltaX.m_fBianTan2DeltaX;//?20为一个探头的扫查宽度，后面需从数据库来


			fY = nL24 * (3.1415926 * g_IniParams.m_fCoder1Diameter / g_IniParams.m_nCoder1PerRingNum) + g_IniParams.m_fTouTan_RefEdge_Distance - nJiOu * g_IniParams.m_fDistanceInBox;	
		}
		else if (nH8 == 2)	// 边探2
		{
			fX = nL24 * (3.1415926 * g_IniParams.m_fCoder2Diameter / g_IniParams.m_nCoder2PerRingNum) - 
			 				deltaX.m_fCar2_Coder1_Distance - nJiOu * g_IniParams.m_fDistanceInBox;
			fY = g_Info[nPlate].width - (FlawInfo.CH_NO - 2000) * 20 - 10 - deltaX.m_fBianTan2DeltaX;
		}
		else if (nH8 == 3)	// 尾探
		{
			fX = g_Info[nPlate].length - (FlawInfo.CH_NO - 2000) * 20
				- 10 - deltaX.m_fBianTan2DeltaX;

			fY = nL24 * (3.1415926 * g_IniParams.m_fCoder1Diameter / g_IniParams.m_nCoder1PerRingNum) - g_IniParams.m_fWeiTan_FreeEdge_Distance + nJiOu * g_IniParams.m_fDistanceInBox;	
		}
		else
		{
		}
	}
	else if (FlawInfo.CH_NO >= 3000 && FlawInfo.CH_NO < 4000)	// 内探1
	{
		fX = nL24 * (3.1415926 * g_IniParams.m_fCoder2Diameter / g_IniParams.m_nCoder2PerRingNum) - 
		 			deltaX.m_fNeitan1_Coder1_Distance - nJiOu * g_IniParams.m_fDistanceInBox;
		fY = (nBoxIndex-g_NeiTanBoxStartNum.m_nNeitan1StartNum) * 120 + nProbeIndex * 20 + deltaX.m_fNeiTan1DeltaX;
		bool bRet = IsNeiTanRegion(fX, fY);
		if(!bRet && (tmpCommand != 2 && tmpCommand != 3))
		{
			return bRet;
		}
	}
	else if (FlawInfo.CH_NO >= 4000 && FlawInfo.CH_NO < 5000)	// 内探2
	{
		fX = nL24 * (3.1415926 * g_IniParams.m_fCoder2Diameter / g_IniParams.m_nCoder2PerRingNum) - 
					deltaX.m_fNeitan2_Coder1_Distance - nJiOu * g_IniParams.m_fDistanceInBox;
		fY = (nBoxIndex - g_NeiTanBoxStartNum.m_nNeitan2StartNum) * 120 + 
			nProbeIndex * 20 + deltaX.m_fNeiTan2DeltaX;

		bool bRet = IsNeiTanRegion(fX, fY);

		if(!bRet && (tmpCommand != 2 && tmpCommand != 3))
		{
			return bRet;
		}
	}
	else
	{
		return FALSE;
	}

	if((fX <= -0.0001 || fY <= -0.0001)  && (tmpCommand != 2 && tmpCommand != 3))
	{
		return FALSE;
	}

	if ((fX > g_Info[nPlate].length || fY > g_Info[nPlate].width) && (tmpCommand != 2 && tmpCommand != 3))
	{
		return FALSE;
	}

/*
	const float WUCHA = 50.0f;
	if (fabs(g_Info[nPlate].length - 3800) < 5 &&
		fabs(g_Info[nPlate].width - 2800) < 5 &&
		fabs(g_Info[nPlate].height - 30) < 2 )
	{
		// 边探小车2
		if ((fabs(fX - 1100) < WUCHA || fabs(fX - 2000) < WUCHA || fabs(fX - 2900) < WUCHA)  &&
			FlawInfo.CH_NO == 2000)
		{
			return FALSE;
		}
		if (fabs(fX - 400) < WUCHA)
		{				
			if(FlawInfo.CH_NO >= 3000 && FlawInfo.CH_NO < 4000 && FlawInfo.DETECT_STATE != 0)
				return FALSE;		
		}

		if (fabs(fX - 700) < WUCHA)
		{	
			if(FlawInfo.CH_NO >= 4000 && FlawInfo.CH_NO < 5000 && FlawInfo.DETECT_STATE != 0)
				return FALSE;			
		}
	}
*/
	// 超出实际板长的伤就不算
	if(fX > g_Info[nPlate].fRealLength && g_Info[nPlate].fRealLength > 1)
	{
		return FALSE;
	}
	return TRUE;
}

bool IsNeiTanRegion(const float& fX, const float& fY)
{

	float fLength = 0.0f;
	if (g_Info[nPlate].fRealLength > 1)
	{
		fLength = g_Info[nPlate].fRealLength;
	}
	else
	{
		fLength =  g_Info[nPlate].length;
	}
	if( (fX > g_IniParams.m_fNeiFrontLength && fX < (fLength - g_IniParams.m_fNeiTailLength)) && 
		(fY > g_IniParams.m_fNeiRefEdgeLength && fY < (g_Info[nPlate].width - g_IniParams.m_fNeiUnRefEdgeLength)))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

} 

int PutsHPIReadResult(P9054_HANDLE hDev, CH_FLAWDATA *pFlawData, int icardno)
{
 
	QString str = QString("%1%2%3%4").arg(QObject::tr("第 ")).arg(QObject::tr(" 块卡进入PutsHPIReadResult：tickcount = ")).arg(icardno).arg(GetTickCount());

	QString strMessage;
	DWORD begin2 = GetTickCount();

	WORD Count_H;
	WORD Count_L;
	DWORD Count;
	// HPI 方式读取伤点数量	
	DWORD dwAddr = CH_FLAW_RESULT_ADDR;
	WORD wAddr_L = dwAddr & 0xFFFF;
	WORD wAddr_H = dwAddr >> 16;

	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wAddr_L);
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wAddr_H);


	Count_L = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L);
	Count_H = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H);
	// 高16位与低16位组合
	Count = (Count_H << 16) | Count_L;

	if (Count>MAX_FLAW_COUNT)
	{
		Count = MAX_FLAW_COUNT;
	}

	g_CS.lock();
	g_dwFlawCount += Count;
	g_CS.unlock();

	dwAddr += 0x4;
	WORD *pData = (WORD *)pFlawData;

	int loop = Count * 12 / 2;
	int j = 0;
	strMessage = QString("%1%2%3").arg(QObject::tr("Part1: 读伤个数耗时 ")).arg(GetTickCount() - begin2).arg(QObject::tr(" ms"));
	begin2 = GetTickCount();
	//CH_FLAWDATA temp;

	// 
	for (int i = 0;i < loop;i++)
	{
		WORD wAddr_L = dwAddr & 0xFFFF;
		WORD wAddr_H = dwAddr >> 16;

		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wAddr_L);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wAddr_H);

		pData[j] = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L);
		pData[j+1] = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H);
//		temp = *((CH_FLAWDATA*)pData[]);
		dwAddr += 0x4;
		j += 2;
		Delay(1);
	}
	strMessage = QString("%1%2%3").arg(QObject::tr("Part2.1: 读伤数据耗时 ")).arg(GetTickCount() - begin2).arg(QObject::tr(" ms"));
	begin2 = GetTickCount();
	// 保存到数据库	
	QString strSQL, strTemp;
	// 增加探伤流水表
	QString strGongYi = g_strGongYi;
	strGongYi.trimmed();
	//chenyuzhu modify start on 2014-7-14
	/*// 第一块卡存数据库时，插一条记录，其他卡不插记录
	QSqlQuery initQuery(*g_sqlDB);
	if (nCardEndStauts == 0)
	{
		strSQL = QString("%1%2%3").arg(QObject::tr("insert t_PlateCheckInfo(Plate_id,CheckClass,CheckName,CheckMethod) values('12','甲','system','")).arg(strGongYi).arg("')");
		initQuery.exec(strSQL);
		initQuery.clear();
	}
	strSQL = QString("%1").arg(QObject::tr("select max(id) as max_id from t_PlateCheckInfo"));
	*/
	// 第一块卡存数据库时，插一条记录，其他卡不插记录
	QSqlQuery initQuery(*g_sqlDB);
	if (nCardEndStauts == 0)
	{
		strSQL = QString("%1%2%3").arg(QObject::tr("insert T_PlateCheckInfo(Check_Class,Check_People,Device_ID,Part_ID,Material) values('甲','system','1','1','")).arg(strGongYi).arg("')");
		initQuery.exec(strSQL);
		initQuery.clear();
	}
	strSQL = QString("%1").arg(QObject::tr("select max(id) as max_id from T_PlateCheckInfo"));

	//chenyuzhu modify end on 2014-7-14
	QString strID;
	initQuery.exec(strSQL);
	initQuery.next();
	strID = initQuery.value(0).toString(); //获得t_PlateCheckInfo表的当前最大id值
	g_CS.lock();
	g_MaxCheckId = strID;
	g_CS.unlock();
	strMessage = QString("%1%2%3").arg(QObject::tr("Part2: 读伤数据耗时 ")).arg(GetTickCount() - begin2).arg(QObject::tr(" ms"));
	begin2 = GetTickCount();
	if (g_pPlateDraw == NULL) return Count;
	// 创建文件
	QString strFileName = QDir::currentPath() +  "/DianShang.txt";
	QFile file(strFileName);
	// asc码文件
	QString strFileName2 = QDir::currentPath() + "/DianShangASC.txt";
	QFile file2(strFileName2);

	static int jishu = 0;
	static DWORD begin = 0;
	static DWORD sumCount = 0;
	if (nCardEndStauts == 0) //第一张板子结束时，是覆盖
	{
		if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
		{
			QMessageBox::warning(NULL,"PutsHPIReadResult",QObject::tr("DianShang.txt打开失败"));
			return -1;
		}
		begin = GetTickCount();
		jishu = 0;
		sumCount = 0;
		if (!file2.open(QIODevice::ReadWrite | QIODevice::Truncate))
		{
			QMessageBox::warning(NULL,"PutsHPIReadResult",QObject::tr("DianShangASC.txt打开失败"));
			return -1;
		}
	}
	else  //以后是继续写入文件
	{
		if (!file.open(QIODevice::ReadWrite))
		{
			QMessageBox::warning(NULL,"PutsHPIReadResult",QObject::tr("DianShang.txt打开失败"));
			return -1;
		}
		if (!file2.open(QIODevice::ReadWrite))
		{
			QMessageBox::warning(NULL,"PutsHPIReadResult",QObject::tr("DianShangASC.txt打开失败"));
			return -1;
		}
	}

	int size1 = file.size();
	file.seek(size1);
	int size2 = file2.size();
	file2.seek(size2);
	strMessage = QString("%1%2%3").arg(QObject::tr("Part3.1: 数据库操作耗时")).arg(GetTickCount() - begin2).arg("ms");
	begin2 = GetTickCount();
	g_ItemCS.lock();
	float fRefBody_Start = g_ItemValue.fRefBody_Start;
	float fFreeBody_Start = g_ItemValue.fFreeBody_Start;
	g_ItemCS.unlock();
	DeltaX dx;
	dx.m_fBianTan1DeltaX = g_IniParams.m_fCar1_RefEdge_Distance;
	dx.m_fBianTan2DeltaX = g_IniParams.m_fCar2_FreeEdge_Distance;
	dx.m_fNeiTan1DeltaX = g_IniParams.m_fNeitan1_RefEdge_Distance;
	dx.m_fNeiTan2DeltaX = g_IniParams.m_fNeitan2_RefEdge_Distance;
	//	dx.m_fNeiTan3DeltaX = dx.m_fNeiTan4DeltaX = g_IniParams.m_fNeitan34_FreeEdge_Distance;
 	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
 	dx.m_fNeitan1_Coder1_Distance = g_IniParams.m_fNeitan1_Coder1_Distance;
 	dx.m_fNeitan2_Coder1_Distance = g_IniParams.m_fNeitan2_Coder1_Distance;
 	dx.m_fNeitan3_Coder1_Distance = g_IniParams.m_fNeitan3_Coder1_Distance;
 	dx.m_fNeitan4_Coder1_Distance = g_IniParams.m_fNeitan4_Coder1_Distance;
 	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
 	dx.m_fCar2_Coder1_Distance = g_IniParams.m_fCar2_Coder1_Distance;
#if 1
//	short old; //
	for (int i = 0;i < Count ;i++)// Count
	{
		// 提取有用的失波数据
		if (pFlawData[i].DETECT_STATE == TYPE_LOST_WAVE &&  g_IniParams.m_nDisplayShiBo == 1)
		{
			BOOL bRet = AddLostWaveFlawData(g_LostWaveDatas, pFlawData[i]);
			// 失波数据此时不存入点伤文件
			continue;
		}
		if (pFlawData[i].DETECT_STATE == TYPE_LOST_WAVE && g_IniParams.m_nDisplayShiBo != 1)
		{
			continue;
		}
		
		float fX = 0.0f;
		float fY = 0.0f;
		float fZ = 0.0f;
		//
	/*	int ii = pFlawData[i].DETECT_STATE;//chenyuzhu 
		if(old != pFlawData[i].CH_NO )
		{
			old = pFlawData[i].CH_NO;
		}
		ii = pFlawData[i].CH_NO;// 逻辑通道号
		ii = pFlawData[i].CH_FLAW_TIME;  // z 坐标
		ii = pFlawData[i].CH_FLAW_PEAK;	// 伤波幅值
		ii = pFlawData[i].CH_BE_TIME;	// 底波时间
		ii = pFlawData[i].CH_BE_PEAK;	// 底波幅度
		ii = pFlawData[i].CH_HEIGHT;    // 钢板厚度
		ii = pFlawData[i].CHX_GAIN;		// 增益
		ii = pFlawData[i].CH_FRONTTIME;	// 前沿
		ii = pFlawData[i].CH_REARTIME;	// 后沿
		ii = pFlawData[i].DETECT_STATE;	// 0失波，1 缺陷，2：超厚, 3:失耦；4：报警
		ii = pFlawData[i].CH_X;         // 编码器值
	*/	       
		//
			
	//	MessageBox(NULL,L"CalculateFlawXY",L"PutsHPIReadResult",MB_OK);
		BOOL bRet = CalculateFlawXY(pFlawData[i], fX, fY, dx); //计算出伤点在钢板中的位置

		
		if (!bRet || (fX > g_Info[nPlate].length) || (fY >g_Info[nPlate].width))
		{
			continue;
		}
		fZ = Dot2Range(VSOUND,pFlawData[i].CH_FLAW_TIME);
		bRet = IsYunXuFlaw(pFlawData[i]);
		if (!bRet && g_IniParams.m_bNewGuoLvEn)
		{
			continue;
		}
		// 向控件里加伤点
		// 读取伤点高八位，若为头尾探，再加6个点
		WORD nH8 = (pFlawData[i].CH_X >> 24) & 0xFF;
		if (pFlawData[i].CH_NO >= 2000 && pFlawData[i].CH_NO < 3000 && (nH8 == 1 || nH8 == 3))
		{
			stDianShang tmp;
			tmp.nCheckId = strID.toLong();
			tmp.nDianShangTeZheng = pFlawData[i].DETECT_STATE;
			tmp.fX = fX;
			tmp.fY = fY;
			tmp.fZ = fZ;

			tmp.fShangBoFuZhi = pFlawData[i].CH_FLAW_PEAK*100.0/(pow(2.0,g_nADCaiYangShu-1)-1); 
			tmp.fDiBoZ = g_nSpeed/(2.0*g_nADCFreq)*(float)pFlawData[i].CH_BE_TIME;
			tmp.fDiBoFuZhi = pFlawData[i].CH_BE_PEAK*100.0/(pow(2.0,g_nADCaiYangShu-1)-1);
			tmp.fDiBoZengYi = Data2Gain(pFlawData[i].CHX_GAIN);
			WORD nWuLiNum = 0;
			g_CS.lock();

			nWuLiNum = m_LogicChannel2ArrayMap[pFlawData[i].CH_NO];
			g_CS.unlock();
			tmp.fShangBoZengYi = Data2Gain(pFlawData[i].CHX_GAIN + allCH_CONFIG[nWuLiNum].CH_GAIN_DIFF);

			file.write((char*)&tmp,sizeof(stDianShang));
			QString str = QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15%16%17%18").arg(tmp.nCheckId).arg("\t").arg(tmp.nDianShangTeZheng).arg("\t").arg(pFlawData[i].CH_NO).arg("\t").arg(tmp.fX).arg("\t")
				.arg(tmp.fY).arg("\t").arg(tmp.fZ).arg("\t").arg(tmp.fShangBoFuZhi).arg("\t").arg(tmp.fDiBoZ).arg("\t").arg(tmp.fDiBoFuZhi).arg("\t")
				.arg(tmp.fShangBoZengYi).arg("\t").arg(tmp.fDiBoZengYi).arg("\t\r\n");

			QByteArray strTemp = str.toLatin1();
			const char* strChar = strTemp.data();
			file2.write(strChar,strlen(strChar));

			// 根据长度选择过滤需要的Z值
			//chenyuzhu modify start on 2014-10-22注释掉应为绘图时已有加点到ocx 
		/*	float fGuoLvZ = GetGuoLvZ(fX, fY);
			if (fZ < fGuoLvZ)
			{
				continue;
			}
			// 只加头尾的点
			if(fX < g_IniParams.m_fNeiFrontLength || fX > (g_Info[nPlate].length - g_IniParams.m_fNeiTailLength))
			{
				//加伤点数据到控件中
				double x = fX;
				double y = fY;
				double z = fZ;
				short type = pFlawData[i].DETECT_STATE;
				QList<QVariant> listVariant;
				listVariant.clear();
				listVariant<<static_cast<double>(tmp.fDiBoFuZhi);
				listVariant<<static_cast<double>(tmp.fShangBoFuZhi);
				listVariant<<static_cast<double>(tmp.fDiBoZ);
				listVariant<<static_cast<double>(tmp.fShangBoZengYi);
				listVariant<<static_cast<double>(tmp.fDiBoZengYi);
				g_pPlateDraw->dynamicCall("AddPointQT(Double,Double,Double,SHORT,QVariant)",x,y,z,type,listVariant);
			}
			*/
		}
		else
		{
			// 写文件
			stDianShang tmp;
			tmp.nCheckId = strID.toInt();
			tmp.nDianShangTeZheng = pFlawData[i].DETECT_STATE;
			tmp.fX = fX;
			tmp.fY = fY;
			tmp.fZ = fZ;
			tmp.fShangBoFuZhi = pFlawData[i].CH_FLAW_PEAK*100.0/(pow(2.0,g_nADCaiYangShu-1)-1); 
			tmp.fDiBoZ = g_nSpeed/(2.0*g_nADCFreq)*(float)pFlawData[i].CH_BE_TIME;
			tmp.fDiBoFuZhi = pFlawData[i].CH_BE_PEAK*100.0/(pow(2.0,g_nADCaiYangShu-1)-1);
			tmp.fDiBoZengYi = Data2Gain(pFlawData[i].CHX_GAIN);
			WORD nWuLiNum = 0;
			g_CS.lock();
			//		    m_LogicChannel2ArrayMap.Lookup(pFlawData[i].CH_NO, nWuLiNum);
			nWuLiNum = m_LogicChannel2ArrayMap[pFlawData[i].CH_NO];
			g_CS.unlock();
			tmp.fShangBoZengYi = Data2Gain(pFlawData[i].CHX_GAIN + allCH_CONFIG[nWuLiNum].CH_GAIN_DIFF);

			//			file.Write(&tmp, sizeof(stDianShang));
			file.write((char*)&tmp,sizeof(stDianShang));
			QString str = QString("%1%2%3%4%5%6%7%8%9%10%11%12%13%14%15%16%17%18").arg(tmp.nCheckId).arg("\t").arg(tmp.nDianShangTeZheng).arg("\t").arg(pFlawData[i].CH_NO).arg("\t").arg(tmp.fX).arg("\t")
				.arg(tmp.fY).arg("\t").arg(tmp.fZ).arg("\t").arg(tmp.fShangBoFuZhi).arg("\t").arg(tmp.fDiBoZ).arg("\t").arg(tmp.fDiBoFuZhi).arg("\t")
				.arg(tmp.fShangBoZengYi).arg("\t").arg(tmp.fDiBoZengYi).arg("\t\r\n");
			//			file2.WriteString(str);
			QByteArray temp = str.toLatin1();
			char* tempChar = temp.data();
			file2.write((char*)tempChar,strlen(tempChar));
			// 根据长度选择过滤需要的Z值
			//chenyuzhu modify start on 2014-10-22注释掉应为绘图时已有加点到ocx 
			/*float fGuoLvZ = GetGuoLvZ(fX, fY);
			if (fZ < fGuoLvZ)
			{
				continue;
			}
			// 只加头尾的点
			if(fX < g_IniParams.m_fNeiFrontLength || fX > (g_Info[nPlate].length - g_IniParams.m_fNeiTailLength))
			{
				double x = fX;
				double y = fY;
				double z = fZ;
				short type = pFlawData[i].DETECT_STATE;
				QList<QVariant> listVariant;
				listVariant.clear();
				listVariant<<static_cast<double>(tmp.fDiBoFuZhi);
				listVariant<<static_cast<double>(tmp.fShangBoFuZhi);
				listVariant<<static_cast<double>(tmp.fDiBoZ);
				listVariant<<static_cast<double>(tmp.fShangBoZengYi);
				listVariant<<static_cast<double>(tmp.fDiBoZengYi);
				g_pPlateDraw->dynamicCall("AddPointQT(Double,Double,Double,SHORT,QVariant)",x,y,z,type,listVariant);
			}
			*/
		}
	}
#else
// 读点伤文件，将数据加到控件里
if (nCardEndStauts == 0)
{
	QString filename = QDir::currentPath() + "/DianShang2.txt";
	QFile file3(filename);

	if (file3.open(QIODevice::ReadOnly))
	{
		DWORD dwBytesRemaining = file3.size();

		stDianShang tmp;
		WORD size = sizeof(stDianShang);

		while (dwBytesRemaining) 
		{
			UINT nBytesRead = file3.read((char*)&tmp,size);

			// 只加头尾的点
			if(tmp.fX < g_IniParams.m_fNeiFrontLength || tmp.fX > (g_Info[nPlate].length - g_IniParams.m_fNeiTailLength))
			{
				double x = fX;
				double y = fY;
				double z = fZ;
				short type = tmp.nDianShangTeZheng;
				QList<QVariant> listVariant;
				listVariant.clear();
				listVariant<<static_cast<double>(tmp.fDiBoFuZhi);
				listVariant<<static_cast<double>(tmp.fShangBoFuZhi);
				listVariant<<static_cast<double>(tmp.fDiBoZ);
				listVariant<<static_cast<double>(tmp.fShangBoZengYi);
				listVariant<<static_cast<double>(tmp.fDiBoZengYi);
				g_pPlateDraw->dynamicCall("AddPointQT(Double,Double,Double,SHORT,QVariant)",x,y,z,type,listVariant);
			}
			dwBytesRemaining -= nBytesRead;
		}
		file3.close();
	}
	else
	{
		//		e.ReportError();
	}
}

#endif
	file.close();
	file2.close();
	DWORD cha = GetTickCount() - begin2;
	strMessage = QString("%1%2%3").arg(QObject::tr("Part3: 插伤点循环耗时 ")).arg(GetTickCount() - begin2).arg("ms");
	begin2 = GetTickCount();
	sumCount += Count;
	jishu++;
	const int nMaxLen = 32;
	char szText[nMaxLen];
	::ZeroMemory(szText, sizeof(char)*nMaxLen);

	strMessage = QString("%1%2%3").arg(QObject::tr("Part4: 余下的耗时 ")).arg(GetTickCount() - begin2).arg("ms");
	begin2 = GetTickCount();
	strMessage = QString("%1%2%3%4").arg(QObject::tr("第")).arg(icardno).arg(QObject::tr("块卡离开PutsHPIReadResult：tickcount = ")).arg(GetTickCount());
	return Count;

}

BOOL AddLostWaveFlawData(QList<CH_FLAWDATA>** pLostWaveDatas, CH_FLAWDATA& flawData)
{
	
	if(pLostWaveDatas == NULL)
		return FALSE;
	// 查找伤对应的物理通道号
	WORD wWuliNum = 0;
	//	m_LogicChannel2ArrayMap.Lookup(flawData.CH_NO, wWuliNum);
	wWuliNum = m_LogicChannel2ArrayMap[flawData.CH_NO];
	// 底波幅值大于某值认为不是失波
	float fDiboScale = float(flawData.CH_BE_PEAK) / MAX_HEIGHT * 100.0f;
	if (fDiboScale > g_IniParams.m_fDiboGuolv)
	{
		return FALSE;
	} 
	// 将伤按编码器值大小插入链表
	BOOL bAdd = FALSE;
	if (!pLostWaveDatas[wWuliNum]->isEmpty())
	{
		for (int i=0; i<pLostWaveDatas[wWuliNum]->size();++i)
		{
			CH_FLAWDATA data = pLostWaveDatas[wWuliNum]->at(i);	
			if (flawData.CH_X < data.CH_X)
			{
				pLostWaveDatas[wWuliNum]->insert(i,flawData);
				bAdd = TRUE;
				break;
			}
		}
	}
	if (!bAdd)
	{
		pLostWaveDatas[wWuliNum]->append(flawData);
	}
	
	return TRUE;
  
}
void SearchUsefulLostWaveData(QList<CH_FLAWDATA>& lostWavedata)
{
	DeltaX dx;
	dx.m_fBianTan1DeltaX = g_IniParams.m_fCar1_RefEdge_Distance;
	dx.m_fBianTan2DeltaX = g_IniParams.m_fCar2_FreeEdge_Distance;
	dx.m_fNeiTan1DeltaX = g_IniParams.m_fNeitan1_RefEdge_Distance;
	dx.m_fNeiTan2DeltaX = g_IniParams.m_fNeitan2_RefEdge_Distance;
	//	dx.m_fNeiTan3DeltaX = dx.m_fNeiTan4DeltaX = g_IniParams.m_fNeitan34_FreeEdge_Distance;
	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
	dx.m_fNeitan1_Coder1_Distance = g_IniParams.m_fNeitan1_Coder1_Distance;
	dx.m_fNeitan2_Coder1_Distance = g_IniParams.m_fNeitan2_Coder1_Distance;
	dx.m_fNeitan3_Coder1_Distance = g_IniParams.m_fNeitan3_Coder1_Distance;
	dx.m_fNeitan4_Coder1_Distance = g_IniParams.m_fNeitan4_Coder1_Distance;
	dx.m_fCar1_Coder1_Distance = g_IniParams.m_fCar1_Coder1_Distance;
	dx.m_fCar2_Coder1_Distance = g_IniParams.m_fCar2_Coder1_Distance;


	QString strFileName = QDir::currentPath() + "\DianShang.txt";
	//POSITION pos = lostWavedata.GetHeadPosition();

	QList<CH_FLAWDATA> tempList;
	int i = 0;
	for (UINT j = 0;j < tempList.count();j++)
	{
		CH_FLAWDATA data = lostWavedata[j];

		WORD nH8 = 0;
		DWORD nL24 = 0;

		nH8 = (data.CH_X >> 24) & 0xFF;
		nL24 = data.CH_X & 0xFFFFFF;

		if (nH8 > 3 || nH8 == 0)
		{
			continue;
		}

		// 不同通道对应不同的编码器比例
		float fXiShu = 0.0f;
		if ((data.CH_NO >=1000 && data.CH_NO < 2000) ||
			(data.CH_NO >=3000 && data.CH_NO < 7000))
		{
			fXiShu = g_IniParams.m_fCoder2Diameter/g_IniParams.m_nCoder2PerRingNum;
		}
		else if (data.CH_NO >=2000 && data.CH_NO < 3000)
		{
			if (nH8 == 1 || nH8 == 3)
			{
				fXiShu = g_IniParams.m_fCoder1Diameter/g_IniParams.m_nCoder1PerRingNum;
			}
			else if (nH8 == 2)
			{
				fXiShu = g_IniParams.m_fCoder2Diameter/g_IniParams.m_nCoder2PerRingNum;
			}
		}
		else
		{
		}

		if (i==0)
		{
			tempList.push_back(data);
		}
		else
		{
			CH_FLAWDATA tail = tempList[tempList.count()];
			float fDistance = fabs((data.CH_X - tail.CH_X) * fXiShu);

			if (fDistance < g_IniParams.m_fLianMaxDistance)
			{
				tempList.push_back(data);
			}
			else
			{
				CH_FLAWDATA head = tempList[0];
				float fLength = fabs(static_cast<float>(tail.CH_X - head.CH_X)) * fXiShu;

				if (fLength >= g_IniParams.m_fZhongduanMinDistance)
				{
					// 计算tempList内数据 X，Y，Z坐标，加入控件
// 					CFile file;
// 					file.Open(strFileName, CFile::modeWrite);
// 					file.SeekToEnd();
					QFile file(strFileName);
					if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
					{
						QMessageBox::warning(NULL,"",QObject::tr("文件打开失败"));
						return;
					}
					//POSITION tmpPos = tempList.GetHeadPosition();
					for(UINT i = 0;i < tempList.count();i++)
					//while(tmpPos)
					{
						CH_FLAWDATA tmpData = tempList[i];
						float fX = 0.0f;
						float fY = 0.0f;
						BOOL bRet = CalculateFlawXY(tmpData, fX, fY, dx);
						if (!bRet || (fX > g_Info[nPlate].length) || (fY >g_Info[nPlate].width))
						{
							continue;
						}

						stDianShang tmp;
						tmp.nCheckId = g_MaxCheckId.toInt();
						tmp.nDianShangTeZheng = tmpData.DETECT_STATE;
						tmp.fX = fX;
						tmp.fY = fY;
						tmp.fZ = 0;
						tmp.fShangBoFuZhi = tmpData.CH_FLAW_PEAK*100.0/(pow(2.0,g_nADCaiYangShu-1)-1); 
						tmp.fDiBoZ = g_nSpeed/(2.0*g_nADCFreq)*(float)tmpData.CH_BE_TIME;
						tmp.fDiBoFuZhi = tmpData.CH_BE_PEAK*100.0/(pow(2.0,g_nADCaiYangShu-1)-1);
						tmp.fDiBoZengYi = Data2Gain(tmpData.CHX_GAIN);
						WORD nWuLiNum = 0;
						g_CS.lock();
						//m_LogicChannel2ArrayMap.Lookup(tmpData.CH_NO, nWuLiNum);
						nWuLiNum = m_LogicChannel2ArrayMap[tmpData.CH_NO];
						g_CS.unlock();
						tmp.fShangBoZengYi = Data2Gain(tmpData.CHX_GAIN + allCH_CONFIG[nWuLiNum].CH_GAIN_DIFF);

						double x = fX;
						double y = fY;
						double z = 0;
						short type = tmp.nDianShangTeZheng;
						QList<QVariant> listVariant;
						listVariant.clear();
						listVariant<<static_cast<double>(tmp.fDiBoFuZhi);
						listVariant<<static_cast<double>(tmp.fShangBoFuZhi);
						listVariant<<static_cast<double>(tmp.fDiBoZ);
						listVariant<<static_cast<double>(tmp.fShangBoZengYi);
						listVariant<<static_cast<double>(tmp.fDiBoZengYi);
						g_pPlateDraw->dynamicCall("AddPointQT(Double,Double,Double,SHORT,QVariant)",x,y,z,type,listVariant);
						file.write((char*)&tmp,sizeof(stDianShang));
					}

					file.close();
				}

				tempList.clear();
				tempList.push_back(data);				
			}
		}
		i++;
	}

	// 将最后一个失波链表存入文件
	if (tempList.count() > 0)
	{
		CH_FLAWDATA tail = tempList[tempList.count()];
		CH_FLAWDATA head = tempList[0];

		WORD nH8 = 0;
		DWORD nL24 = 0;

		nH8 = (tail.CH_X >> 24) & 0xFF;
		nL24 = tail.CH_X & 0xFFFFFF;

		// 不同通道对应不同的编码器比例
		float fXiShu = 0.0f;
		if ((tail.CH_NO >=1000 && tail.CH_NO < 2000) ||
			(tail.CH_NO >=3000 && tail.CH_NO < 7000))
		{
			fXiShu = g_IniParams.m_fCoder2Diameter/g_IniParams.m_nCoder2PerRingNum;
		}
		else if (tail.CH_NO >=2000 && tail.CH_NO < 3000)
		{
			if (nH8 == 1 || nH8 == 3)
			{
				fXiShu = g_IniParams.m_fCoder1Diameter/g_IniParams.m_nCoder1PerRingNum;
			}
			else if (nH8 == 2)
			{
				fXiShu = g_IniParams.m_fCoder2Diameter/g_IniParams.m_nCoder2PerRingNum;
			}
			else
			{}
		}
		else
		{
		}

		float fLength = qAbs(tail.CH_X - head.CH_X) * fXiShu;

		if (fLength >= g_IniParams.m_fZhongduanMinDistance)
		{
// 			CFile file;
// 			file.Open(strFileName, CFile::modeWrite);
// 			file.SeekToEnd();
			QFile file(strFileName);
			if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
			{
				QMessageBox::warning(NULL,"",QObject::tr("文件打开失败"));
				return;
			}

			//POSITION tmpPos = tempList.GetHeadPosition();
			for(UINT j=0;j < tempList.count();j++)
			//while(tmpPos)
			{
				CH_FLAWDATA tmpData = tempList[j];

				float fX = 0.0f;
				float fY = 0.0f;
				BOOL bRet = CalculateFlawXY(tmpData, fX, fY, dx);
				if (!bRet || (fX > g_Info[nPlate].length) || (fY >g_Info[nPlate].width))
				{
					continue;
				}

				stDianShang tmp;
				tmp.nCheckId = g_MaxCheckId.toInt();
				tmp.nDianShangTeZheng = tmpData.DETECT_STATE;
				tmp.fX = fX;
				tmp.fY = fY;
				tmp.fZ = 0;
				tmp.fShangBoFuZhi = tmpData.CH_FLAW_PEAK*100.0/(pow(2.0,g_nADCaiYangShu-1)-1); 
				tmp.fDiBoZ = g_nSpeed/(2.0*g_nADCFreq)*static_cast<float>(tmpData.CH_BE_TIME);
				tmp.fDiBoFuZhi = tmpData.CH_BE_PEAK*100.0/(pow(2.0,g_nADCaiYangShu-1)-1);
				tmp.fDiBoZengYi = Data2Gain(tmpData.CHX_GAIN);
				WORD nWuLiNum = 0;
				g_CS.lock();
				//m_LogicChannel2ArrayMap.Lookup(tmpData.CH_NO, nWuLiNum);
				nWuLiNum = m_LogicChannel2ArrayMap[tmpData.CH_NO];
				g_CS.unlock();
				tmp.fShangBoZengYi = Data2Gain(tmpData.CHX_GAIN + allCH_CONFIG[nWuLiNum].CH_GAIN_DIFF);

				double x = fX;
				double y = fY;
				double z = 0;
				short type = tmp.nDianShangTeZheng;
				QList<QVariant> listVariant;
				listVariant.clear();
				listVariant<<static_cast<double>(tmp.fDiBoFuZhi);
				listVariant<<static_cast<double>(tmp.fShangBoFuZhi);
				listVariant<<static_cast<double>(tmp.fDiBoZ);
				listVariant<<static_cast<double>(tmp.fShangBoZengYi);
				listVariant<<static_cast<double>(tmp.fDiBoZengYi);

				//g_pPlateDraw->AddPoint2(fX, fY, 0, tmp.nDianShangTeZheng,
				//	tmp.fShangBoFuZhi, tmp.fDiBoZ, tmp.fDiBoFuZhi, tmp.fDiBoZengYi, tmp.fShangBoZengYi);
				g_pPlateDraw->dynamicCall("AddPointQT(Double,Double,Double,SHORT,QVariant)",x,y,z,type,listVariant);
				file.write((char*)&tmp,sizeof(stDianShang));
			}
			file.close();
		}

	}

}
// 根据底波过滤
BOOL IsYunXuFlaw(CH_FLAWDATA& flawData)
{
	// 根据逻辑通道号查询对应的前盲区和后盲区值
	float fQianMangQu = 0.0f;
	float fHouMangQu = 0.0f;
	float fMaxDiShi = 0.0f;
	BOOL bRet = LogicChannelQianMangQuMap.contains(flawData.CH_NO);	
	if (!bRet)
	{
		return FALSE;
	}
	fQianMangQu = LogicChannelQianMangQuMap[flawData.CH_NO];

	bRet = LogicChannelHouMangQuMap.contains(flawData.CH_NO);
	if (!bRet)
	{
		return FALSE;
	}
	fHouMangQu = LogicChannelHouMangQuMap[flawData.CH_NO];
	bRet = LogicChannelMaxDiShiMap.contains(flawData.CH_NO);
	if (!bRet)
	{
		return FALSE;
	}
	fMaxDiShi = LogicChannelMaxDiShiMap[flawData.CH_NO];
	//若底波幅值大于0
	if (flawData.CH_BE_PEAK > 0)
	{
		float fDiShi = Dot2Range(VSOUND, flawData.CH_BE_TIME);
		float fShangShi = Dot2Range(VSOUND, flawData.CH_FLAW_TIME);
		if (fDiShi <= g_Info[nPlate].height && fDiShi > (g_Info[nPlate].height - fHouMangQu))
		{
			float cha = fDiShi - fShangShi;
			if (cha < fHouMangQu)
			{
				WORD wWuLiNum = 0;
				BOOL bRet2 = m_LogicChannel2ArrayMap.contains(flawData.CH_NO);
				if (!bRet2)
				{
					return FALSE;
				}
				wWuLiNum = m_LogicChannel2ArrayMap[flawData.CH_NO];
				float fBackGate = Dot2Range(VSOUND, allCH_CONFIG[wWuLiNum].CH_REARTIME);
				if (fDiShi < fBackGate - 0.1 || fDiShi > fBackGate +0.1)
				{
					return FALSE;
				}
			}			 
		}
		else if(fDiShi > g_Info[nPlate].height && fDiShi <  fMaxDiShi)
		{
			float cha2 = (fDiShi - g_Info[nPlate].height) / 4;
			if (cha2 > g_IniParams.m_fDiShiJudgeValue)
			{
				fQianMangQu = (fDiShi - g_Info[nPlate].height) * 2;
			}

			float fCompareValue = fDiShi - g_Info[nPlate].height + fQianMangQu;
			if (fShangShi < fCompareValue)
			{
				if (fDiShi < be_distance - 0.1 || fDiShi > be_distance + 0.1)
 				{
 					return FALSE;
 				}				
			}
		}
		else
		{}
	}
	return TRUE;
}
void GetNeitanBoxStartNum2(NeiTanBoxStartNum& NeitanBoxStartNum)
{
	g_ItemCS.lock();
	DWORD dwLiLun_NT1 = g_ItemValue.dwLiLun_NT1;
	DWORD dwLiLun_NT2 = g_ItemValue.dwLiLun_NT2;
	g_ItemCS.unlock();

	//内探1
	WORD nNei1Temp = dwLiLun_NT1;
	for(int i= 0; i<ITEM_NEI1_RESULT_NUM; i++)
	{
		int value = nNei1Temp%2;
		if (value == 1)
		{
			NeitanBoxStartNum.m_nNeitan1StartNum = i;
			break;
		}
		nNei1Temp /= 2;
	}

	//内探2
	DWORD nNei2Temp = dwLiLun_NT2;
	for(UINT i= 0; i<ITEM_NEI2_RESULT_NUM; i++)
	{
		int value = nNei2Temp%2;
		if (value == 1)
		{
			NeitanBoxStartNum.m_nNeitan2StartNum = i;
			break;
		}
		nNei2Temp /= 2;
	}	
}
//计算当前活动探头所属哪个探头盒
int GetCurrentProbeBoxNum()
{
	UINT j = 0;//存储探头盒号
	if (g_dwActiveChannel < 16)
	{
		j = g_dwActiveChannel/8;
	}
	else if (g_dwActiveChannel < 84)
	{
		j = (g_dwActiveChannel - 1)/3 - 3;
	}
	else //if (m_nWuLiTongDaoHao < 153)
	{
		j = (g_dwActiveChannel - 1)/3 - 4;
	}
	return j;
}

//计算当前活动探头所属哪块卡
int GetCurrentProbeFromCardNum()
{
	int nIndex = g_dwActiveChannel / MAX_CARD_CHANNEL_NO;
	if ((g_dwActiveChannel > 95 && g_dwActiveChannel < 99)
		||(g_dwActiveChannel > 127 && g_dwActiveChannel < 131))
	{
		nIndex = nIndex - 1;
	}
	return nIndex;
}

void ControlCodec(P9054_HANDLE hDev, WORD codecState)
{
	 P9054_WriteWord(hDev, P9054_ADDR_SPACE0, CODEC_ADDR, codecState);	
	// P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0x122, 0x001F);	
}

void WaveDelay(P9054_HANDLE hDev, WORD nDelay)
{
	P9054_WriteWord(hDev,P9054_ADDR_SPACE0, PCI_SAMPLE_ZERO_OFFSET, nDelay);
}

void SetMonitorRange(P9054_HANDLE hDev, WORD *pInterSpace)
{
	DWORD dwAddr = MONITOR_RANGE_ADDR;
	for (int i = 0;i < FLAW_DOT_MAX_NO;i++)
	{
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, dwAddr, pInterSpace[i]);
		dwAddr += 2;
	}
}

void SelChannel(WORD channelNO)
{
	WORD channelNOtmp;
	if (channelNO >= 256)
	{
	//	g_dwActiveChannel = channelNO - 256;
		g_pAscan->m_bSelChannel= TRUE;
	}
	else
	{
	//	g_dwActiveChannel = channelNO;
		g_pAscan->m_bSelChannel = TRUE;
	}
}
void DrawRuler(QPainter &painter, QRectF rect, double start, double end, int separate, bool isHori, bool topToBottom)
{
    painter.setPen(Qt::black);

    int majorCount = separate + 1;
    double majorHeight = 6.0;
    double minorHeight = 3.0;

    double majorSpace = (rect.right() - rect.left()) / separate;   // 主要刻度
    if (!isHori)
    {
        majorSpace = (rect.bottom() - rect.top()) / separate;   // 主要刻度
    }

    double minorSpace = majorSpace / separate;      // 子刻度

    QVector<QPointF> vecMajorsStart;
    vecMajorsStart.resize(majorCount);
    QVector<QPointF> vecMajorsEnd;
    vecMajorsEnd.resize(majorCount);

    int minorCount = (separate - 1) * separate;
    QVector<QPointF> vecMinorsStart;
    vecMinorsStart.resize(minorCount);
    QVector<QPointF> vecMinorsEnd;
    vecMinorsEnd.resize(minorCount);

    QVector<double> vecTextsShow;                   // 要显示的刻度数vector
    vecTextsShow.resize(majorCount);
    double majorSpaceShow = (end - start) / separate;
    for (int i = 0; i < majorCount; i++)
    {
        vecTextsShow[i] = i * majorSpaceShow + start;
    }

    QVector<QRectF> vecTextsShowRect;
    vecTextsShowRect.resize(majorCount);
    double textWidth = 50.0;
    double textHeight = 10.0;
    double textSpace = 0.0;

    if (isHori)
    {
        if (topToBottom)    // 水平 上到下
        {
            QPointF ptStart(rect.topLeft());
            vecMajorsStart[0] = ptStart;
            QPointF ptEnd(rect.topLeft().x(), rect.topLeft().y() + majorHeight);
            vecMajorsEnd[0] = ptEnd;

            for (int i = 1; i < majorCount; ++i)
            {
                ptStart.setX(ptStart.x() + majorSpace);
                ptEnd.setX(ptEnd.x() + majorSpace);
                vecMajorsStart[i] = ptStart;
                vecMajorsEnd[i] = ptEnd;
            }

            for (int i = 0; i < separate; ++i)
            {
                QPointF ptMinorStart(vecMajorsStart[i]);
                QPointF ptMinorEnd(vecMajorsEnd[i].x(), vecMajorsEnd[i].y() - majorHeight + minorHeight);

                for (int j = 0; j < separate - 1; j++)
                {
                    ptMinorStart.setX(ptMinorStart.x() + minorSpace);
                    ptMinorEnd.setX(ptMinorEnd.x() + minorSpace);
                    vecMinorsStart[i * (separate - 1) + j] = ptMinorStart;
                    vecMinorsEnd[i * (separate - 1) + j] = ptMinorEnd;
                }
            }

            for (int i = 0; i < majorCount; ++i)
            {
                if (i == 0)
                {
                    vecTextsShowRect[i].setRect(vecMajorsEnd[i].x(),
                                                vecMajorsEnd[i].y() + textSpace,
                                                textWidth,
                                                textHeight);
                }
                else if (i == majorCount - 1)
                {
                    vecTextsShowRect[i].setRect(vecMajorsEnd[i].x() - textWidth,
                                                vecMajorsEnd[i].y() + textSpace,
                                                textWidth,
                                                textHeight);
                }
                else
                {
                    vecTextsShowRect[i].setRect(vecMajorsEnd[i].x() - textWidth / 2,
                                                vecMajorsEnd[i].y() + textSpace,
                                                textWidth,
                                                textHeight);
                }
            }
        }
        else                // 水平 下到上
        {
            QPointF ptStart(rect.bottomLeft());
            vecMajorsStart[0] = ptStart;
            QPointF ptEnd(rect.bottomLeft().x(), rect.bottomLeft().y() - majorHeight);
            vecMajorsEnd[0] = ptEnd;

            for (int i = 1; i < majorCount; ++i)
            {
                ptStart.setX(ptStart.x() + majorSpace);
                ptEnd.setX(ptEnd.x() + majorSpace);
                vecMajorsStart[i] = ptStart;
                vecMajorsEnd[i] = ptEnd;
            }

            for (int i = 0; i < separate; ++i)
            {
                QPointF ptMinorStart(vecMajorsStart[i]);
                QPointF ptMinorEnd(vecMajorsEnd[i].x(), vecMajorsEnd[i].y() + majorHeight - minorHeight);

                for (int j = 0; j < separate - 1; j++)
                {
                    ptMinorStart.setX(ptMinorStart.x() + minorSpace);
                    ptMinorEnd.setX(ptMinorEnd.x() + minorSpace);
                    vecMinorsStart[i * (separate - 1) + j] = ptMinorStart;
                    vecMinorsEnd[i * (separate - 1) + j] = ptMinorEnd;
                }
            }

            for (int i = 0; i < majorCount; ++i)
            {
                if (i == 0)
                {
                    vecTextsShowRect[i].setRect(vecMajorsEnd[i].x(),
                                                vecMajorsEnd[i].y() - textSpace - textHeight,
                                                textWidth,
                                                textHeight);
                }
                else if (i == majorCount - 1)
                {
                    vecTextsShowRect[i].setRect(vecMajorsEnd[i].x() - textWidth,
                                                vecMajorsEnd[i].y() - textSpace - textHeight,
                                                textWidth,
                                                textHeight);
                }
                else
                {
                    vecTextsShowRect[i].setRect(vecMajorsEnd[i].x() - textWidth / 2,
                                                vecMajorsEnd[i].y() - textSpace - textHeight,
                                                textWidth,
                                                textHeight);
                }
            }
        }
    }
    else                    // 竖直 上到下
    {
        if (topToBottom)
        {
            QPointF ptStart(rect.topRight());
            vecMajorsStart[0] = ptStart;
            QPointF ptEnd(rect.topRight().x() - majorHeight, rect.topRight().y());
            vecMajorsEnd[0] = ptEnd;

            for (int i = 1; i < majorCount; ++i)
            {
                ptStart.setY(ptStart.y() + majorSpace);
                ptEnd.setY(ptEnd.y() + majorSpace);
                vecMajorsStart[i] = ptStart;
                vecMajorsEnd[i] = ptEnd;
            }

            for (int i = 0; i < separate; ++i)
            {
                QPointF ptMinorStart(vecMajorsStart[i]);
                QPointF ptMinorEnd(vecMajorsEnd[i].x() + majorHeight - minorHeight, vecMajorsEnd[i].y());

                for (int j = 0; j < separate - 1; j++)
                {
                    ptMinorStart.setY(ptMinorStart.y() + minorSpace);
                    ptMinorEnd.setY(ptMinorEnd.y() + minorSpace);
                    vecMinorsStart[i * (separate - 1) + j] = ptMinorStart;
                    vecMinorsEnd[i * (separate - 1) + j] = ptMinorEnd;
                }
            }

            for (int i = 0; i < majorCount; ++i)
            {
                if (i == 0)
                {
                    vecTextsShowRect[i].setRect(vecMajorsEnd[i].x() - textWidth - textSpace,
                                                vecMajorsEnd[i].y(),
                                                textWidth,
                                                textHeight);
                }
                else if (i == majorCount - 1)
                {
                    vecTextsShowRect[i].setRect(vecMajorsEnd[i].x() - textWidth - textSpace,
                                                vecMajorsEnd[i].y() - textHeight,
                                                textWidth,
                                                textHeight);
                }
                else
                {
                    vecTextsShowRect[i].setRect(vecMajorsEnd[i].x() - textWidth - textSpace,
                                                vecMajorsEnd[i].y() - textHeight / 2,
                                                textWidth,
                                                textHeight);
                }
            }
        }
        else                // 竖直 下到上
        {
            QPointF ptStart(rect.topLeft());
            vecMajorsStart[0] = ptStart;
            QPointF ptEnd(rect.topLeft().x() + majorHeight, rect.topLeft().y());
            vecMajorsEnd[0] = ptEnd;

            for (int i = 1; i < majorCount; ++i)
            {
                ptStart.setY(ptStart.y() + majorSpace);
                ptEnd.setY(ptEnd.y() + majorSpace);
                vecMajorsStart[i] = ptStart;
                vecMajorsEnd[i] = ptEnd;
            }

            for (int i = 0; i < separate; ++i)
            {
                QPointF ptMinorStart(vecMajorsStart[i]);
                QPointF ptMinorEnd(vecMajorsEnd[i].x() - majorHeight + minorHeight, vecMajorsEnd[i].y());

                for (int j = 0; j < separate - 1; j++)
                {
                    ptMinorStart.setY(ptMinorStart.y() + minorSpace);
                    ptMinorEnd.setY(ptMinorEnd.y() + minorSpace);
                    vecMinorsStart[i * (separate - 1) + j] = ptMinorStart;
                    vecMinorsEnd[i * (separate - 1) + j] = ptMinorEnd;
                }
            }

            for (int i = 0; i < majorCount; ++i)
            {
                if (i == 0)
                {
                    vecTextsShowRect[i].setRect(vecMajorsEnd[i].x() + textSpace,
                                                vecMajorsEnd[i].y(),
                                                textWidth,
                                                textHeight);
                }
                else if (i == majorCount - 1)
                {
                    vecTextsShowRect[i].setRect(vecMajorsEnd[i].x() + textSpace,
                                                vecMajorsEnd[i].y() - textHeight,
                                                textWidth,
                                                textHeight);
                }
                else
                {
                    vecTextsShowRect[i].setRect(vecMajorsEnd[i].x() + textSpace,
                                                vecMajorsEnd[i].y() - textHeight / 2,
                                                textWidth,
                                                textHeight);
                }
            }
        }
    }

    // 绘图
    painter.drawLine(vecMajorsStart[0], vecMajorsStart[separate]);
	int temp = 1;

    for (int i = 0; i < minorCount; ++i)
    {
        painter.drawLine(vecMinorsStart[i], vecMinorsEnd[i]);
    }

    if (isHori)
    {
        for (int i = 0; i < majorCount; ++i)
        {
            painter.drawLine(vecMajorsStart[i], vecMajorsEnd[i]);

            if (i == 0)
            {
                painter.drawText(vecTextsShowRect[i], Qt::AlignLeft, QString::number(vecTextsShow[i], 'd', temp));
            }
            else if (i == majorCount - 1)
            {
                painter.drawText(vecTextsShowRect[i], Qt::AlignRight, QString::number(vecTextsShow[i], 'd', temp));
            }
            else
            {
                painter.drawText(vecTextsShowRect[i], Qt::AlignHCenter, QString::number(vecTextsShow[i], 'd', temp));
            }
        }
    }
    else
    {
        if (topToBottom)
        {
            for (int i = 0; i < majorCount; ++i)
            {
                painter.drawLine(vecMajorsStart[i], vecMajorsEnd[i]);
                painter.drawText(vecTextsShowRect[i], Qt::AlignRight, QString::number(vecTextsShow[i], 'd', temp));
            }
        }
        else
        {
            for (int i = 0; i < majorCount; ++i)
            {
                painter.drawLine(vecMajorsStart[i], vecMajorsEnd[i]);
                painter.drawText(vecTextsShowRect[i], Qt::AlignLeft, QString::number(vecTextsShow[i], 'd', temp));
            }
        }
    }
}

void ReplaceOldToNew()
{
	//从数据库中读取文件/SheBeifile.dbfl
	QString strFileName = QDir::currentPath()+ "/SheBeifile.dbfl";

	QFile fileold(strFileName);
	bool bRet1 = fileold.open(QIODevice::ReadOnly);

	if (!bRet1)
	{
		QMessageBox::warning(NULL,"InitConfig",QObject::tr("SheBeifile.dbfl打开失败"));
		return ;
	}

	fileold.seek(0);
	SHEBEICANSHU_OLD SheBeiCanShu_old;
	quint64 nBytesRead = fileold.read((char*)&SheBeiCanShu_old,sizeof(SHEBEICANSHU_OLD));
	fileold.close();

	// wangwei modified start on 2016-01-15
	int nNei1Num = g_OtherParameters.insideCar1BoxCount * g_OtherParameters.insideCarEachBoxChannelCount;	// 内探1通道数
	int nNei2Num = g_OtherParameters.insideCar2BoxCount * g_OtherParameters.insideCarEachBoxChannelCount;	// 内探2通道数
	int nBianAllNum = g_OtherParameters.insideCar1PhysicalChannelStart;										// 边探1+2通道数
	int nBian1Begin = g_OtherParameters.edgeCar1PhysicalChannelStart;										// 边探1物理通道号起始 一般是0
	int nBian2Begin = g_OtherParameters.edgeCar2PhysicalChannelStart;										// 边探2物理通道号起始 一般是8
	int nNei1Begin = g_OtherParameters.insideCar1PhysicalChannelStart;										// 内探1物理通道号起始
	int nNei2Begin = g_OtherParameters.insideCar2PhysicalChannelStart;										// 内探2物理通道号起始
	int nNei2LogicalStart = g_OtherParameters.insideCar2LogicalChannelStart;								// 内探2逻辑通道号起始
	// wangwei modified end on 2016-01-15

	//chenyuzhu add end on 2014-7-8
	SHEBEICANSHU SheBeiCanShu;
	//chenzyuzhu add start 2014-7-10 给数据文件初始值用的

	//////////////////////////////////ChannelParam////////////////////////////////////////
	for (UINT i=0; i < MAX_CHANNEL_NO; i++)
	{	
		int nPhysicsNo = SheBeiCanShu_old.ChannelParam[i].nNum;
		SheBeiCanShu.ChannelParam[i].nNum = nPhysicsNo;
		SheBeiCanShu.ChannelParam[i].nRefNum = SheBeiCanShu_old.ChannelParam[i].nRefNum; 
		SheBeiCanShu.ChannelParam[i].nZhongZhiDian = SheBeiCanShu_old.ChannelParam[i].nZhongZhiDian;			// 中值点
		strcpy_s(SheBeiCanShu.ChannelParam[i].strIsOpen, SheBeiCanShu_old.ChannelParam[i].strIsOpen);			// 通道开启
		SheBeiCanShu.ChannelParam[i].fICIV = SheBeiCanShu_old.ChannelParam[i].fICIV;							// 底波初始增益值(db)
		SheBeiCanShu.ChannelParam[i].fTCIV = SheBeiCanShu_old.ChannelParam[i].fTCIV;							// 伤波初始增益值(db)
		SheBeiCanShu.ChannelParam[i].fDelay = SheBeiCanShu_old.ChannelParam[i].fDelay;							// 延迟(mm)
		SheBeiCanShu.ChannelParam[i].fShangmangqu = SheBeiCanShu_old.ChannelParam[i].fShangmangqu;				// 上盲区(mm)
		SheBeiCanShu.ChannelParam[i].fXiamangqu = SheBeiCanShu_old.ChannelParam[i].fXiamangqu;					// 下盲区(mm)
		SheBeiCanShu.ChannelParam[i].fDibozhamenkuandu = SheBeiCanShu_old.ChannelParam[i].fDibozhamenkuandu;	// 底波闸门宽度(mm)
		SheBeiCanShu.ChannelParam[i].fDiBoYuZhi = SheBeiCanShu_old.ChannelParam[i].fDiBoYuZhi;					// 底波阈值(%)
		SheBeiCanShu.ChannelParam[i].fYiZhi = SheBeiCanShu_old.ChannelParam[i].fYiZhi;							// 抑制(%)
		SheBeiCanShu.ChannelParam[i].fShangDiCha = SheBeiCanShu_old.ChannelParam[i].fShangDiCha;				// 伤波增益差值(db)
		SheBeiCanShu.ChannelParam[i].nDemarcateResult = SheBeiCanShu_old.ChannelParam[i].nDemarcateResult;
		SheBeiCanShu.ChannelParam[i].fZengyixishu = 1.0;

		if((nPhysicsNo >= nNei2Begin)
			&& (nPhysicsNo < nNei2Begin + nNei2Num)) 
		{
			SheBeiCanShu.ChannelParam[i].nRefNum = nNei2LogicalStart + nPhysicsNo - nNei2Begin;
		}

		// 边探1和2都只有3个通道开启，其他关闭，否则检测中边探会爆失波
		if ((i > 2 && i < 8 ) || (i > 10 && i < 16))
		{
			strcpy_s(SheBeiCanShu.ChannelParam[i].strIsOpen, "关");				//通道开启
		}
	}
	//////////////////////////////////TanShangGongYi////////////////////////////////////////

	strcpy_s(SheBeiCanShu.TanShangGongYi.strJianBoMoShi, SheBeiCanShu_old.TanShangGongYi.strJianBoMoShi);//检波模式
	SheBeiCanShu.TanShangGongYi.fShangMangQu = SheBeiCanShu_old.TanShangGongYi.fShangMangQu;//上盲区
	SheBeiCanShu.TanShangGongYi.fXiaMangQu = SheBeiCanShu_old.TanShangGongYi.fXiaMangQu;	//下盲区
	strcpy_s(SheBeiCanShu.TanShangGongYi.strDACOpen, SheBeiCanShu_old.TanShangGongYi.strDACOpen);		//DAC开启
	strcpy_s(SheBeiCanShu.TanShangGongYi.strAuToZengYiOpen, SheBeiCanShu_old.TanShangGongYi.strAuToZengYiOpen);		//自动增益开启
	strcpy_s(SheBeiCanShu.TanShangGongYi.strAutoHuaMenOpen, SheBeiCanShu_old.TanShangGongYi.strAutoHuaMenOpen);	//自动闸门跟踪
	strcpy_s(SheBeiCanShu.TanShangGongYi.strBaoJing, SheBeiCanShu_old.TanShangGongYi.strBaoJing);
	strcpy_s(SheBeiCanShu.TanShangGongYi.strYiQiXingHao, SheBeiCanShu_old.TanShangGongYi.strYiQiXingHao);
	strcpy_s(SheBeiCanShu.TanShangGongYi.strName, SheBeiCanShu_old.TanShangGongYi.strName);
	strcpy_s(SheBeiCanShu.TanShangGongYi.strPlateType, SheBeiCanShu_old.TanShangGongYi.strPlateType);
	strcpy_s(SheBeiCanShu.TanShangGongYi.strDetector, SheBeiCanShu_old.TanShangGongYi.strDetector);
	strcpy_s(SheBeiCanShu.TanShangGongYi.strTanShangMethod, SheBeiCanShu_old.TanShangGongYi.strTanShangMethod);
	strcpy_s(SheBeiCanShu.TanShangGongYi.strTangShangBiaoZhun, SheBeiCanShu_old.TanShangGongYi.strTangShangBiaoZhun);
	strcpy_s(SheBeiCanShu.TanShangGongYi.strTanShangJiBieBody, SheBeiCanShu_old.TanShangGongYi.strTanShangJiBieBody);
	strcpy_s(SheBeiCanShu.TanShangGongYi.strTanShangJiBieEdge, SheBeiCanShu_old.TanShangGongYi.strTanShangJiBieEdge);
	strcpy_s(SheBeiCanShu.TanShangGongYi.strTanShangLinMingDu, SheBeiCanShu_old.TanShangGongYi.strTanShangLinMingDu);
	strcpy_s(SheBeiCanShu.TanShangGongYi.strTanShangJiBieEdge, SheBeiCanShu_old.TanShangGongYi.strTanShangJiBieEdge);
	SheBeiCanShu.TanShangGongYi.fTouWeiMangQu = SheBeiCanShu_old.TanShangGongYi.fTouWeiMangQu;
	SheBeiCanShu.TanShangGongYi.fCeMangQu = SheBeiCanShu_old.TanShangGongYi.fCeMangQu;
	SheBeiCanShu.TanShangGongYi.nNetTanTongDaoShu = SheBeiCanShu_old.TanShangGongYi.nNetTanTongDaoShu;
	strcpy_s(SheBeiCanShu.TanShangGongYi.strOuHeJi, SheBeiCanShu_old.TanShangGongYi.strOuHeJi);
	SheBeiCanShu.TanShangGongYi.fHouDuChaoCha = SheBeiCanShu_old.TanShangGongYi.fHouDuChaoCha;
	strcpy_s(SheBeiCanShu.TanShangGongYi.strHouDuAutoRead, SheBeiCanShu_old.TanShangGongYi.strHouDuAutoRead);
	strcpy_s(SheBeiCanShu.TanShangGongYi.strDuiBiShiYang, SheBeiCanShu_old.TanShangGongYi.strDuiBiShiYang);
	SheBeiCanShu.TanShangGongYi.fChuShiZengYi = SheBeiCanShu_old.TanShangGongYi.fChuShiZengYi;
	SheBeiCanShu.TanShangGongYi.fDiBoYuZhi = SheBeiCanShu_old.TanShangGongYi.fDiBoYuZhi;
	SheBeiCanShu.TanShangGongYi.fChuShiYiZhi = SheBeiCanShu_old.TanShangGongYi.fChuShiYiZhi;
	SheBeiCanShu.TanShangGongYi.fShaoMiaoJianGe = SheBeiCanShu_old.TanShangGongYi.fShaoMiaoJianGe;
	strcpy_s(SheBeiCanShu.TanShangGongYi.strModel, SheBeiCanShu_old.TanShangGongYi.strModel);
	SheBeiCanShu.TanShangGongYi.nflag = SheBeiCanShu_old.TanShangGongYi.nflag;

	////////////////////////////////////Plate_Type//////////////////////////////////////
	SheBeiCanShu.Plate_Type.fLWaveSpeed = SheBeiCanShu_old.Plate_Type.fLWaveSpeed;
	SheBeiCanShu.Plate_Type.fTWaveSpeed = SheBeiCanShu_old.Plate_Type.fTWaveSpeed;

	///////////////////////////////////////YiQi///////////////////////////////////
	strcpy_s(SheBeiCanShu.YiQiParam.strName, SheBeiCanShu_old.YiQiParam.strName);
	SheBeiCanShu.YiQiParam.nTouWeiTanTongDaoShu = SheBeiCanShu_old.YiQiParam.nTouWeiTanTongDaoShu;
	SheBeiCanShu.YiQiParam.nTouWeiTanLogicBegin = SheBeiCanShu_old.YiQiParam.nTouWeiTanLogicBegin;

	SheBeiCanShu.YiQiParam.fTanTouZhongXinJuLi = SheBeiCanShu_old.YiQiParam.fTanTouZhongXinJuLi;
	SheBeiCanShu.YiQiParam.fADCFreq = SheBeiCanShu_old.YiQiParam.fADCFreq;
	SheBeiCanShu.YiQiParam.nADCaiYangShu = SheBeiCanShu_old.YiQiParam.nADCaiYangShu;
	SheBeiCanShu.YiQiParam.nBianTan1TongDaoShu = SheBeiCanShu_old.YiQiParam.nBianTan1TongDaoShu;        // 边探1通道数
	SheBeiCanShu.YiQiParam.nBianTan2TongDaoShu = SheBeiCanShu_old.YiQiParam.nBianTan2TongDaoShu;		// 边探2通道数
	SheBeiCanShu.YiQiParam.nNeiTan1TongDaoShu = SheBeiCanShu_old.YiQiParam.nNeiTan1TongDaoShu;			// 内探1通道数
	SheBeiCanShu.YiQiParam.nNeiTan2TongDaoShu = SheBeiCanShu_old.YiQiParam.nNeiTan2TongDaoShu;			// 内探2通道数

	// 避免数据库配置文件有问题，此处改为从OtherParameters.ini文件中配置
	SheBeiCanShu.YiQiParam.nNeiTan1TongDaoShu = nNei1Num;												//内探1通道数
	SheBeiCanShu.YiQiParam.nNeiTan2TongDaoShu = nNei2Num;												//内探2通道数	

	SheBeiCanShu.YiQiParam.nBianTan1WuLiBegin = SheBeiCanShu_old.YiQiParam.nBianTan1WuLiBegin ;			// 边探1物理开始通道号
	SheBeiCanShu.YiQiParam.nBianTan2WuLiBegin = SheBeiCanShu_old.YiQiParam.nBianTan2WuLiBegin;          // 边探2物理开始通道号
	SheBeiCanShu.YiQiParam.nNeiTan1WuLiBegin = SheBeiCanShu_old.YiQiParam.nNeiTan1WuLiBegin;			// 内探1物理开始通道号
	SheBeiCanShu.YiQiParam.nNeiTan2WuLiBegin = SheBeiCanShu_old.YiQiParam.nNeiTan2WuLiBegin;			// 内探2物理开始通道号
	SheBeiCanShu.YiQiParam.nBianTan1LogicBegin = SheBeiCanShu_old.YiQiParam.nBianTan1LogicBegin;
	SheBeiCanShu.YiQiParam.nBianTan2LogicBegin = SheBeiCanShu_old.YiQiParam.nBianTan2LogicBegin;
	SheBeiCanShu.YiQiParam.nNeiTan1LogicBegin = SheBeiCanShu_old.YiQiParam.nNeiTan1LogicBegin;
	SheBeiCanShu.YiQiParam.nNeiTan2LogicBegin = SheBeiCanShu_old.YiQiParam.nNeiTan2LogicBegin;
	SheBeiCanShu.YiQiParam.fDistanceTanCha2NeiTanYi1 = SheBeiCanShu_old.YiQiParam.fDistanceTanCha2NeiTanYi1;
	SheBeiCanShu.YiQiParam.fDistanceTanCha2NeiTanYi2 = SheBeiCanShu_old.YiQiParam.fDistanceTanCha2NeiTanYi2;
	SheBeiCanShu.YiQiParam.fDistanceTanCha2BianTan = SheBeiCanShu_old.YiQiParam.fDistanceTanCha2BianTan;
	SheBeiCanShu.YiQiParam.fDistanceTanCha2DaoLun = SheBeiCanShu_old.YiQiParam.fDistanceTanCha2DaoLun;
	SheBeiCanShu.YiQiParam.nWuLiTongDaoShu = SheBeiCanShu_old.YiQiParam.nWuLiTongDaoShu;
	SheBeiCanShu.YiQiParam.fCengXiangSaoChaMaxDistance = SheBeiCanShu_old.YiQiParam.fCengXiangSaoChaMaxDistance;
	SheBeiCanShu.YiQiParam.nTouWeiTanWuLiBegin = SheBeiCanShu_old.YiQiParam.nTouWeiTanWuLiBegin;
	SheBeiCanShu.YiQiParam.nBianTan1WuLiBegin = SheBeiCanShu_old.YiQiParam.nBianTan1WuLiBegin;
	SheBeiCanShu.YiQiParam.fCeChang1 = SheBeiCanShu_old.YiQiParam.fCeChang1;
	SheBeiCanShu.YiQiParam.fCeChang2 = SheBeiCanShu_old.YiQiParam.fCeChang2;
	SheBeiCanShu.YiQiParam.fCeChang3 = SheBeiCanShu_old.YiQiParam.fCeChang3;
	SheBeiCanShu.YiQiParam.fBianTanXiaoChe1 = SheBeiCanShu_old.YiQiParam.fBianTanXiaoChe1;
	SheBeiCanShu.YiQiParam.fBianTanXiaoChe2 = SheBeiCanShu_old.YiQiParam.fBianTanXiaoChe2;
	SheBeiCanShu.YiQiParam.fTouTanMangQu = SheBeiCanShu_old.YiQiParam.fTouTanMangQu;
	SheBeiCanShu.YiQiParam.fWeiTanMangQu = SheBeiCanShu_old.YiQiParam.fWeiTanMangQu;
	SheBeiCanShu.YiQiParam.fLeftTanMangQu = SheBeiCanShu_old.YiQiParam.fLeftTanMangQu;
	SheBeiCanShu.YiQiParam.fRightTanMangQu = SheBeiCanShu_old.YiQiParam.fRightTanMangQu;
	SheBeiCanShu.YiQiParam.fLeftTanNeiMangQu = SheBeiCanShu_old.YiQiParam.fLeftTanNeiMangQu;
	SheBeiCanShu.YiQiParam.fRightTanNeiMangQu = SheBeiCanShu_old.YiQiParam.fRightTanNeiMangQu;
	strcpy_s(SheBeiCanShu.YiQiParam.strTanShangMethod, SheBeiCanShu_old.YiQiParam.strTanShangMethod);
	strcpy_s(SheBeiCanShu.YiQiParam.strOuHeJi, SheBeiCanShu_old.YiQiParam.strOuHeJi);



	//////////////////////////////////////////////////////////////////////////

	QFile file1(strFileName);
	if (!file1.open(QIODevice::ReadWrite | QIODevice::Truncate))
	{
		QMessageBox::warning(NULL,"AScanSave",QObject::tr("SheBeiFile.dbfl打开失败"));
		return ;
	}
	quint64 nBytesWrite = file1.write((char*)&SheBeiCanShu,sizeof(SHEBEICANSHU));
	file1.close();
	//////////////////////////////////////////////////////////////////////////
	//chenzyuzhu add end on 2014-7-10 给数据文件初始值用的
}