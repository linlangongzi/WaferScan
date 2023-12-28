#include <QMessageBox>
#include <Qstring>
#include <QWidget>
#include <QEvent>
#include <QApplication>
#include <tchar.h>
#include <windows.h>
#include "const.h"
#include "..\pci9054_dll\plx_lib_x.h"	// plx_dll.dll PCI卡操作动态链接库
#include "..\pci9054_dll\p9054_lib.h"
#include "DataDefine.h"
#include "GlobalFunction.h"
#include <math.h>
#include<QMutex>
#include "HardwareAddress.h"
#include "PLXCommunication.h"
#include "EventDefine.h"
#include "ascan.h"
extern map<WORD,CPicData*> m_MapCPicData;
//#include "FeatureInfoWnd.h"
//#include "ChannelPanel.h"
//#include "ChannelInfoDockView.h"
//#include "CScanDlgBar.h"
//#include "MechanicalControl.h"
//#include "CScanParamDockBar.h"
//#include "CDGraphMove.h"
//#include "MovePathView.h"
//#include "MainFrm.h"
//#include "UltrasonicImmersedTest.h"
//#include "ExButton.h"
//#include "SysParam.h"
//#include "MechanicalControlPanel.h"

//#include "dibapi.h"

extern QMutex g_csCard;
extern short g_nWaveSpeed; 

const DWORD MAX_FLAW_COUNT = 200000;
DWORD g_dwActiveChannel = 1;//DEFAULT_CHANNEL;
DWORD g_dwActiveFlawOrBottom = 1;//下发时的底波或伤波标识

//extern CMutex g_csCard;
//extern CDatabase g_sqlDB;

//WORD g_wDMABuffer[2][DMA_COUNT];
//CH_FLAWDATA_CSCAN_D g_stPrimitiveData[2][PER_CH_DATA_NUM*2];
 extern HANDLE hSaveDMADataEvent;
// extern short g_nWaveSpeed;
// extern BOOL g_bDrawRealTime; //实时成像
CH_FLAWDATA_CSCAN_D g_pFeatrueInfo[2][PER_CH_DATA_NUM*2]; // wangbingfu add at2011-9-27
FEATURE_DATA g_stFeatrueInfo[DATA_NUM];
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
	// Generate random numbers in the half-closed interval
	// [range_min, range_max). In other words,
	// range_min <= random number < range_max
	WORD *p = pArray;

	int i;
	for ( i = 0; i < n; i++ )
	{
		WORD u = (double)rand() / (RAND_MAX + 1) * (range_max - range_min)
			+ range_min;
		// 画线
		*p = u;
		p++;
	}


}

// 卡工作线程
unsigned __stdcall AECWorkThreadProc(LPVOID pParam)
{
	// 传递参数
	AECTHREAD_PARAM *pAECThreadProc =(AECTHREAD_PARAM *)pParam;
	int iCardIndex = pAECThreadProc->index;

	QString str;
	BOOL  bThreadExitFlag = FALSE;
	// 命令调用

	while(1)
	{
		// 加锁
		if (g_csCard.tryLock())		
		{

			switch (pAECThreadProc->wMain2AECThreadCommand)
			{
			case MAIN2AECTHREADCOMMAND_INIT:	//卡初始化
				{
					BOOL bRet = AECCardInit(pAECThreadProc);
					if (bRet)
					{
						pAECThreadProc->wAEC2MainThreadFlag = AECTHREADREURN_INIT_OK;
//						pAECThreadProc->wMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
						pAECThreadProc->wMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_PLATE_START;
					}
					else
					{
						pAECThreadProc->wAEC2MainThreadFlag = AECTHREADREURN_INIT_FALSE;
					}
					break;
				}
			case MAIN2AECTHREADCOMMAND_PLATE_START:	// 单张钢板检测开始
				{					
					// 避免重复开始
// 					if (pAECThreadProc->bStarted == TRUE)
// 					{
// 						pAECThreadProc->wMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
// 						pAECThreadProc->wAEC2MainThreadFlag = AECTHREADREURN_START_OK;
// 					}
// 					else
// 					{
						BOOL nRet = AEC_plate_Start(pAECThreadProc);
						if(nRet)
						{
							pAECThreadProc->bStarted = TRUE; 
							pAECThreadProc->wMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
							pAECThreadProc->wAEC2MainThreadFlag = AECTHREADREURN_START_OK;
						}
						else
						{
							pAECThreadProc->wAEC2MainThreadFlag = AECTHREADREURN_START_FALSE;
						}
//					} 
					break;
				}
			case MAIN2AECTHREADCOMMAND_PLATE_STOP:	// 单张钢板检测结束
				{
// 					if (pAECThreadProc->m_bThreadStopEn)
// 					{
// 						// 避免重复结束
// 						if (pAECThreadProc->bStarted == FALSE)
// 						{
// 							pAECThreadProc->wMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
// 							break;
// 						}
						
						BOOL bRet = AEC_plate_Stop(pAECThreadProc);

						if(bRet)
						{
							pAECThreadProc->wAEC2MainThreadFlag = AECTHREADREURN_STOP_OK; 
							pAECThreadProc->bStarted = FALSE; 
							
							pAECThreadProc->wMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
						}
						else
						{
							pAECThreadProc->wAEC2MainThreadFlag = AECTHREADREURN_STOP_FALSE; 
						}

//					}
					break;
				}
			case MAIN2AECTHREADCOMMAND_SETONE:	// SetOne
				{		
					BOOL bRet = AEC_palte_setone(pAECThreadProc);
					if (bRet)
					{
						pAECThreadProc->wMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
						pAECThreadProc->wAEC2MainThreadFlag = AECTHREADREURN_SETONE_OK;
					}
					else
						pAECThreadProc->wAEC2MainThreadFlag = AECTHREADREURN_SETONE_FALSE;

					break;
				}
			case MAIN2AECTHREADCOMMAND_BIAODING:	// 标定
				{
					break;
				}
			case MAIN2AECTHREADCOMMAND_ZIJIAN:	// 卡自检
				{
					break;
				}
			case   MAIN2AECTHREADCOMMAND_IDLE: // 卡运行时的一般操作
				{
					AECThread_Idle(pAECThreadProc);
					pAECThreadProc->wMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;

					break;
				}
			case   MAIN2AECTHREADCOMMAND_THREAD_STOP: // 停止线程
				{				
					bThreadExitFlag = TRUE;
					if (pAECThreadProc->bStarted)
					{
						pAECThreadProc->wMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
					}
					break;
				}
			case MAIN2AECTHREADCOMMAND_SEL_CHANNEL:
				{
					AEC_selChannel(pAECThreadProc);
					pAECThreadProc->wMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
					break;
				}
			case MAIN2AECTHREADCOMMAND_SETALL:
				{
					BOOL bRet = AEC_palte_setall(pAECThreadProc);
					if (bRet)
					{
						pAECThreadProc->wMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
						pAECThreadProc->wAEC2MainThreadFlag = AECTHREADREURN_SETONE_OK;
					}
					else
						pAECThreadProc->wAEC2MainThreadFlag = AECTHREADREURN_SETONE_FALSE;

					break;
				}
			default:
				{
					pAECThreadProc->wMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
					break;
				}
			}

			// 解锁
// 			BOOL bRet = FALSE;
// 			do 
// 			{
// 				bRet = sLock.unlock();
// 			} while (!bRet);
			g_csCard.unlock();

			Sleep(25);
		}  
		else 
		{ 			
			Sleep(30);// 需要调整 
		}
		if(bThreadExitFlag)
		{
			pAECThreadProc->wAEC2MainThreadFlag = AECTHREADREURN_THREAD_STOP_OK;

			break; // stop the 
		}
	} 

	return 0;
}

BOOL AECCardInit(AECTHREAD_PARAM * pThreadParam)
{
/*	// 
	WORD wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, DSP_2_PCI_SRAM_CONTROL1_ADDR, wValue);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, DSP_2_PCI_SRAM_CONTROL2_ADDR, wValue);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_SRAM_CONTROL_ADDR, wValue);
*/
	WORD wValue = 1;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, SYS_RESET_ADDRESS, wValue);

	wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, SYS_RESET_ADDRESS, wValue);

	wValue = 1;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_ADDRESS, wValue);

	wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_ADDRESS, wValue);

	wValue = 1;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);

	wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);
/*
	// 清除中断
	pThreadParam->pPLXCommunication->ResetHPI_INT(pThreadParam->hDev);
	// Song Chenguang add start on 2009-03-26


	// 清除dsp访问sram控制申和清除pci-〉dsp通讯信号
	WORD wHPIAddr_H = 0xa000;
	WORD wHPIAddr_L = 0x8000;
	WORD wValueWriteH =	0;
	WORD wValueWriteL =	0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wHPIAddr_L);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wHPIAddr_H);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L, wValueWriteL);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H, wValueWriteH);

	wHPIAddr_H = 0xa000;
	wHPIAddr_L = 0x8004;
	wValueWriteH =	0;
	wValueWriteL =	0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wHPIAddr_L);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wHPIAddr_H);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L, wValueWriteL);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H, wValueWriteH);

	wHPIAddr_H = 0xa000;
	wHPIAddr_L = 0x8008;
	wValueWriteH =	1;
	wValueWriteL =	0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wHPIAddr_L);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wHPIAddr_H);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L, wValueWriteL);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H, wValueWriteH);

	// 撤销Dsp清除Pci到dsp通讯标志
	wHPIAddr_H = 0xa000;
	wHPIAddr_L = 0x8008;
	wValueWriteH =	0;
	wValueWriteL =	0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wHPIAddr_L);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wHPIAddr_H);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L, wValueWriteL);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H, wValueWriteH);
	// Song Chenguang add end on 2009-03-26
*/

	return TRUE;
}

int AECThread_Idle(AECTHREAD_PARAM * pThreadParam)
{
	int iCardIndex = pThreadParam->index;

	if (g_dwActiveChannel / PER_CARD_CHANNEL_NUM == iCardIndex)
	{		
		// CPicData* pData = new CPicData;
		WORD wTempChannelNum = pThreadParam->wSelChannel;
		map<WORD,CPicData*>::iterator itPicData;
		BOOL bExist = FALSE;
		if (pThreadParam->nFlawOrBottom == 0)
		{
			wTempChannelNum += 6;  //wangwei modified on 2014-6-18
		}
		for (itPicData = m_MapCPicData.begin();itPicData != m_MapCPicData.end(); itPicData++)
		{
			if (wTempChannelNum == itPicData->first)
			{
				bExist = TRUE;
			}
		}
		if (bExist)
		{
			BOOL bRet = pThreadParam->pPLXCommunication->PutsReadDrawData(pThreadParam->hDev, m_MapCPicData[wTempChannelNum]);
			if(bRet)
			{
				ASCANEvent *AScanDataEvent = new ASCANEvent;
				AScanDataEvent->pData = NULL;
				AScanDataEvent->wChannelNo = pThreadParam->wSelChannel;
				AScanDataEvent->nFlawOrBottom = pThreadParam->nFlawOrBottom;
				QApplication::postEvent(pThreadParam->qObject,AScanDataEvent);
			}
		}


		CH_FLAWDATA_CSCAN_D* pData2 = new CH_FLAWDATA_CSCAN_D;

		::ZeroMemory(pData2,sizeof(CH_FLAWDATA_CSCAN_D));

		pData2->wXHigh16 = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0x14);
		pData2->wXLow16 = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0x12);
		pData2->wGate1SyncTime = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS);

		pData2->wGate1MPAmp = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0x2);

		pData2->wGate2MPTime = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0x4);

		pData2->wGate2MPAmp = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0x6);

		pData2->wGate3MPTime = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0x8);


		pData2->wGate3MPAmp = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0xA);

		pData2->wGate4SyncTime = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0xC);
		pData2->wGate4MPAmp = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_FEATURE_DATA_ADDRESS + 0xE);

		if (pData2 != NULL)
		{
			FEATUREINFOEvent *FeatuereInfoDataEvent = new FEATUREINFOEvent();
			FeatuereInfoDataEvent->pData = pData2;
			QApplication::postEvent(pThreadParam->qObject,FeatuereInfoDataEvent);
		}

		// 通道数据清零
		WORD wValue = PCI_SAMPLE_RESET_ON;
		P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);

		wValue = PCI_SAMPLE_RESET_OFF;
		P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);

		wValue = PCI_SAMPLE_RESET_ON;
		P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_ADDRESS, wValue);

		wValue = PCI_SAMPLE_RESET_OFF;
		P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_ADDRESS, wValue);
	}


	// 读取伤数据
	BOOL bRet = ReadFlawResult(pThreadParam, iCardIndex);

	return 0;
} 



BOOL ReadFlawResult(AECTHREAD_PARAM * p,int iCardIndex)
{
	WORD wInterruptType = 0;
	WORD wValueL =0, wValueH=0;

	DWORD dwWaitObjectIndex = WaitForSingleObject(hSaveDMADataEvent, 0);

	if (dwWaitObjectIndex != WAIT_OBJECT_0)
	{
		WORD wIsValid = P9054_ReadWord(p->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_EN_FLAG_ADDRESS);
		WORD wTemp = wIsValid & 0x1;
		if (wTemp == 1)
		{
			WORD wIndex = P9054_ReadWord(p->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_INDEX_ADDRESS);
			wIndex &= 0x3;

			DWORD dwAddress =0;
			if (wIndex == 1)
			{
				dwAddress = 0x20000;
			}
			else if (wIndex == 2)
			{
				dwAddress = 0x30000;
			}
			else
			{
				return FALSE;
			}

			int nSize = sizeof(FEATURE_DATA) /  sizeof(WORD);

			for (int j=0;j<DATA_NUM;j++)
			{
				FEATURE_DATA data;
				WORD* pwData = (WORD*)(&data);

				for (int i=0; i<nSize; i++)
				{
					*pwData = P9054_ReadWord(p->hDev, P9054_ADDR_SPACE0, dwAddress);
					pwData++;
					dwAddress += 2;
				}
				memcpy(&(g_stFeatrueInfo[j]), &data, sizeof(FEATURE_DATA));
			}


			int a = 0;

			WORD wValue = PCI_SAMPLE_RESET_ON;
			P9054_WriteWord(p->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_FLAG_ADDRESS, wValue);

			wValue = PCI_SAMPLE_RESET_OFF;
			P9054_WriteWord(p->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_FLAG_ADDRESS, wValue);

		}
		SetEvent(hSaveDMADataEvent);
	}

	return TRUE;
}


BOOL AEC_plate_Start(AECTHREAD_PARAM * pThreadParam)
{
	try
	{
		/*通知dsp*/
		// 给卡写开始标志	
		// 申请SRAM控制权
		WORD wValue = 1;
		WORD ntimer = 0;
		WORD nInterSpace[FLAW_DOT_MAX_NO];
		RangeProcess(g_nWaveSpeed, 300, nInterSpace);	// temp
		pThreadParam->pPLXCommunication->SetMonitorRange(pThreadParam->hDev, nInterSpace);	

		// 设置程序开始了
		pThreadParam->wAEC2MainThreadFlag = AECTHREADREURN_START_OK;
		return TRUE;
	}
	catch(QString exception)
	{
		QMessageBox::about(NULL,"钢板检测开始异常",exception);
	}
}


void RangeProcess(float VSound, // VSound--------------m/s
				  float Range, // Range --------------m
				  WORD* pInterSpace) // pInterSpace ---------
{
//	WORD nRangeDot = Range2Dot(VSound, Range);

	//	float fMinRange = Dot2Range(VSound, MONITOR_SIZE);
	//	if(Range < fMinRange)
	//		Range = fMinRange;

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
//	return nRangeDot;
}


BOOL AEC_plate_Stop(AECTHREAD_PARAM * pThreadParam)
{
	int iCardIndex = pThreadParam->index;

	BOOL nErrorFlag_sram_controller = FALSE; 
	BOOL nErrorFlag_card_process = FALSE; 
	int  nError =0;

	// WAIT FOR THE ENDFLAG OF EACH CARD

	// 等待卡结束处理
	BOOL bEnd = FALSE;
	int  nEndFlag = FALSE;

	nErrorFlag_sram_controller = FALSE;

	//申请sram控制权
	WORD wValue = 1;
	int timer=0;
	// REM temp
/*
	DWORD begin = GetTickCount();
	while(1)
	{
		wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
		wValue = wValue >> 1;
		if ((wValue & 0x1 )== 0)
		{
			break;
		}	

		DWORD cha = GetTickCount() - begin;
		if (cha > 30000)
		{
			CString str;
			str.Format(_T("第 %d 块卡钢板结束时查询标志位失败，请检查卡是否正常或重新启动机器。"), pThreadParam->index+1);
			AfxMessageBox(str);

			CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
			pApp->m_PLXCommunication.UnInit(pApp->m_stThreadParam);
			ExitProcess(0);
		}
	}

	// 获取了控制权
	// Song Chenguang add start on 2009-03-24
	pThreadParam->pPLXCommunication->ResetHPI_INT(pThreadParam->hDev);//

	wValue = 1; 	
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_CLEAR_COMMUNICATE_FLAG_ADDR, wValue);
	wValue = 0; 	
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_CLEAR_COMMUNICATE_FLAG_ADDR, wValue);

	// 给PLC写结束标志
	wValue= SIM_END_VALUE;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_CMD_NUM_ADDR, wValue);

	wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);

	wValue = (wValue >> 1) << 1; 	
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);
	wValue += 1; 	
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);

	// Song Chenguang add end on 2009-03-24

	// Song Chenguang add start on 2009-03-23
	// 查询PCI到DSP反馈信号
	begin = GetTickCount();
	while(1)
	{
		wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
		wValue = wValue >> 1;
		if ((wValue & 0x1 )== 0)
		{
			break;
		}
		DWORD cha = GetTickCount() - begin;
		if (cha > 30000)
		{
			CString str;
			str.Format(_T("第 %d 块卡钢板结束时查询PCI到DSP反馈信号失败，请检查卡是否正常或重新启动机器。"), pThreadParam->index+1);
			AfxMessageBox(str);

			CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
			pApp->m_PLXCommunication.UnInit(pApp->m_stThreadParam);
			ExitProcess(0);
		}
	}
	// 清除PCI到DSP通讯命令
	wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_CMD_NUM_ADDR, wValue);

	// 清除PCI到DSP信号标志位
	wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);

	wValue = (wValue >> 1) << 1;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);
	// Song Chenguang add end on 2009-03-23
*/
// 	BOOL bRet = ReadStopFlawResult(pThreadParam, iCardIndex);
// 	if (!bRet)
// 	{
// 		return FALSE;
// 	}

	return TRUE;
}

BOOL ReadStopFlawResult(AECTHREAD_PARAM * p,int iCardIndex)
{
	QString str;
	WORD wInterruptType=0xffff;
	WORD wValueL =0, wValueH=0;

	WORD wValue = 0;
	// REM temp
/*	while(1)
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
				CString str;
				str.Format(_T("第 %d 块卡读结束中断失败，请检查卡是否正常或重新启动机器。"), p->index+1);
				AfxMessageBox(str);

				CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
				pApp->m_PLXCommunication.UnInit(pApp->m_stThreadParam);
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
*/
	// 收到结束信号
	// 读取伤数据
//	int nRet = PutsHPIReadResult(p->hDev, flawdata_result, iCardIndex);

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

BOOL AEC_selChannel(AECTHREAD_PARAM * pThreadParam) //WDC_DEVICE_HANDLE hDev, WORD channelNO, int card)
{
	
	WORD channelNO = pThreadParam->wSelChannel;
	WORD wValueH = (channelNO - 1)/4* 2 + pThreadParam->nFlawOrBottom;
	//WORD wValueH = (channelNO - 1)/4* 2 + 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_CHANNEL_H_ADDRESS, wValueH);
	WORD wValueS = (channelNO - 1)%4 ;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_CHANNEL_S_ADDRESS, wValueS);


	//A扫数据清零
	WORD wValue = PCI_SAMPLE_RESET_ON;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);

	wValue = PCI_SAMPLE_RESET_OFF;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);

	wValue = PCI_SAMPLE_RESET_ON;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_ADDRESS, wValue);

	wValue = PCI_SAMPLE_RESET_OFF;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, FEATURE_BUFFER_RESET_ADDRESS, wValue);


	//P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_RESET_OFFSET, wValue);

	// set the delay of the pci sample 

	//wValue = pThreadParam->pAllConfig[channelNO].CH_ZEROCORRECT;
	//P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_ZERO_OFFSET, wValue);
/*		
	// Song Chenguang add start on 2009-03-24
	DWORD begin = GetTickCount();
	while(1)
	{
		wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
		wValue = wValue >> 1;
		if ((wValue & 0x1 )== 0x0)
		{
			break;
		}	

		DWORD cha = GetTickCount() - begin;
		if (cha > 30000)
		{
			CString str;
			str.Format(_T("第 %d 块卡切换通道时查询标志位失败，请检查卡是否正常或重新启动机器。"), pThreadParam->index+1);
			AfxMessageBox(str);


			CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
			pApp->m_PLXCommunication.UnInit(pApp->m_stThreadParam);
			ExitProcess(0);
		}
	}

	// 申请SRAM控制权
	begin = GetTickCount();
		
	CString strInfo;
	strInfo.Format(_T("AEC_selChannel函数里执行SelChannel命令, CH_NO = %d\r\n"), channelNO);
	//	WriteLog(_T("标定顺序记录.txt"), strInfo, CFile::modeWrite);
*/
	// 写入数据
	//P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, CHANNEL_SEL_ADDR, channelNO);
/*
	// 释放SRAM控制权
	wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_SRAM_CONTROL_ADDR, wValue);

	wValue = SIM_SEL_CHANNEL;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_CMD_NUM_ADDR, wValue);

	// Song Chenguang add start on 2009-04-08
	wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
	// Song Chenguang add end on 2009-04-08
	wValue = (wValue >> 1) << 1; 	
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);
	wValue = wValue + 1; 	
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);

	// 查询PCI到DSP反馈信号
	begin = GetTickCount();
	while(1)
	{
		//Sleep(10);
		wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
		wValue = wValue >> 1;
		if ((wValue & 0x1 )== 0)
		{
			break;
		}

		DWORD cha = GetTickCount() - begin;
		if (cha > 3000)
		{
			CString str;
			str.Format(_T("线程 %d 的selChannel查询PCI到DSP反馈信号失败"), pThreadParam->index+1);
			AfxMessageBox(str);

			CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
			pApp->m_PLXCommunication.UnInit(pApp->m_stThreadParam);
			ExitProcess(0);
		}

	}

	// 清除PCI到DSP通讯命令
	wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_CMD_NUM_ADDR, wValue);
	
	// 清除PCI到DSP信号标志位
	wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
	wValue = (wValue >> 1) << 1;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);

	// Song Chenguang add end on 2009-03-24
*/	
	//wValue = 1;
	//P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_RESET_OFFSET, wValue);
	//wValue = 0;
	//P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_RESET_OFFSET, wValue);

	//pThreadParam->pPLXCommunication->WaveDelay(pThreadParam->hDev, pThreadParam->pAllConfig[channelNO].CH_ZEROCORRECT);

	return TRUE;
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
	return temp;
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

BOOL AEC_palte_setone(AECTHREAD_PARAM * pThreadParam)
{
	// REM temp
	P9054_HANDLE hDev = pThreadParam->hDev;
	CH_CONFIG *pCHX_INIT_CONFIG = &pThreadParam->pAllConfig[pThreadParam->wSetOneChannel];

	// temp
	// 写入数据

	WORD channelNO = pThreadParam->wSetOneChannel;
	if (channelNO == 0)
	{
		channelNO = 1;
	}
	WORD wValueH = (channelNO - 1)/4 ;
	WORD wValueS = (channelNO - 1)%4 ;

	DWORD dwAddrStart = SOFT_CHANNEL_CONFIG_PARARM_ADDRESS + wValueH * 0x01000 + wValueS * 0x80;
	


	DWORD dwAddr = dwAddrStart;
	WORD *pData = (WORD *)pCHX_INIT_CONFIG;
	int i = 0;
	
	while(dwAddr < dwAddrStart + 0x5B) 
	{
		P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, dwAddr, pData[i]);
		dwAddr += 2;
		i++;
	}

	return TRUE;
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
// 	if (lpszName != NULL && nCapacity > 0)
// 		lpszName[0] = _T('\0');
// 	TCHAR szExeName[MAX_PATH];
// 	DWORD nSize = ::GetModuleFileName(GetModuleHandle(NULL), szExeName, MAX_PATH);
// 	if (nSize == 0)
// 		return 0;
// 	do {
// 		nSize--;
// 	} while(szExeName[nSize] != TCHAR('\\') && nSize > 0);
// 
// 	if (nSize > 0)
// 		nSize++;
// 	if (nSize > nCapacity - 1)
// 		return nSize;
// 
// 	if (lpszName == NULL || nCapacity == 0)
// 		return nSize;
// 
// 	//	wcsncpy(lpszName, szExeName, nSize);
// 	memcpy(lpszName, szExeName, nSize*sizeof(TCHAR));
// 	lpszName[nSize] = _T('\0');
// 	return nSize;
	return 0;
}

// 读取DMA数据
//BOOL ReadDMADate(AECTHREAD_PARAM * p, const DWORD& dwLocalAddress)
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
	/* yachang.huang
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMovePathView* pView = (CMovePathView*) pFrame->m_wndSplitter.GetPane(0,0);

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
//	pView->m_GraphMove.GetPara(dParam, lLogParam);
	if (lLogParam[0] == 0) // 横向
	{
		dY += dParam[19]*(data.wCardNum*2 + data.wChannelNo);
	} 
	else	// 纵向
	{
		dX += dParam[19]*(data.wCardNum*2 + data.wChannelNo);
	}
	*/
}

/*
BOOL PrimitiveDataChange(const CH_FLAWDATA_CSCAN_D& Cdata, CFeatureInfo& Fdata, const DWORD& dwChannelNo)
{
	GetCScanDataCoordinate(Cdata, Fdata.m_dX, Fdata.m_dY, Fdata.m_dZ);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	CChannelInfo info = pFrame->GetChannelInfo((WORD)dwChannelNo);
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
		if (bMTPmode[0])
		{
			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate1SyncTime) / ADFREQUENCY + fDelay;
		}
		else
			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate1SyncTime) / ADFREQUENCY;

		if (pFrame->m_pFeatureWnd->m_bFPFTimeDisplay)
		{
			if(bMTPmode[0])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate1FPFirstTime);
			else
			{
				if (Cdata.wGate1FPFirstTime <= 2)
				{
					return FALSE;
				}

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
		if (bMTPmode[1])
			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate2SyncTime) / ADFREQUENCY + fDelay;
		else
			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate2SyncTime) / ADFREQUENCY;

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
		if (bMTPmode[2])
			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate3SyncTime) / ADFREQUENCY + fDelay;
		else
			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate3SyncTime) / ADFREQUENCY;

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
		if (bMTPmode[3])
			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate4SyncTime) / ADFREQUENCY + fDelay;
		else
			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate4SyncTime) / ADFREQUENCY;

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
	}

	return TRUE;
}
*/
BOOL PrimitiveDataChange(const CH_FLAWDATA_CSCAN_D& Cdata, CFeatureInfo& Fdata)
{
	/*  yachang.huang
	GetCScanDataCoordinate(Cdata, Fdata.m_dX, Fdata.m_dY, Fdata.m_dZ, Fdata.m_dW1, Fdata.m_dA, Fdata.m_dB, Fdata.m_dW2);

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

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
// 		if (bMTPmode[0])
// 		{		
			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate1SyncTime) / ADFREQUENCY + fDelay;
// 		}
// 		else
// 			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate1SyncTime) / ADFREQUENCY;

		if (pFrame->m_pFeatureWnd->m_bFPFTimeDisplay)
		{
			if(bMTPmode[0])
				Fdata.m_pfFeatures[i][j] = Dot2Range(g_nWaveSpeed, Cdata.wGate1FPFirstTime);
			else
			{
// 				if (Cdata.wGate1FPFirstTime <= 2)
// 				{
// 					return FALSE;
// 				}

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

//		if (bMTPmode[1])
			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate2SyncTime) / ADFREQUENCY + fDelay;
// 		else
// 			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate2SyncTime) / ADFREQUENCY;

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
//		if (bMTPmode[2])
			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate3SyncTime) / ADFREQUENCY + fDelay;
// 		else
// 			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate3SyncTime) / ADFREQUENCY;

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
//		if (bMTPmode[3])
			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate4SyncTime) / ADFREQUENCY + fDelay;
// 		else
// 			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate4SyncTime) / ADFREQUENCY;

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
	}
     
    */
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
	/* yachang.huang

	HANDLE hbmFile = NULL;
	DWORD dwWritten = 0;
	DWORD dwBitmapArraySize;//位图数据大小
//	HDC hScrDC = NULL;
	HDC hMemDC = NULL;         
	int nScreenWidth = 0; 
	int nScreenHeight = 0;
	int ixStart = 0; 
	int iyStart = 0; 
	int ix = 0; 
	int iy = 0;

	BYTE *lpBitmapBits = NULL; 


	ixStart = rect.left;
	iyStart = rect.top;
	ix = rect.right - rect.left;
	iy = rect.bottom - rect.top;


	dwBitmapArraySize = ((((ix*bitCount) + 31) & ~31) >> 3) * iy;
	hMemDC = CreateCompatibleDC(hScrDC);


	BITMAPINFO bmInfo; 
	ZeroMemory(&bmInfo, sizeof(BITMAPINFO));
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = ix;
	bmInfo.bmiHeader.biHeight = iy;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = bitCount;


	HBITMAP directBmp = CreateDIBSection(hMemDC, (BITMAPINFO*)&bmInfo, 
		DIB_RGB_COLORS, (void **)&lpBitmapBits, NULL, 0);
	HGDIOBJ previousObject = SelectObject(hMemDC, directBmp);


	BitBlt(hMemDC, 0, 0, ix, iy, hScrDC, ixStart, iyStart, SRCCOPY);
	

	if(nType == 2)	// A扫
	{
		SetBkMode(hMemDC,TRANSPARENT);
		SetTextColor(hMemDC, RGB(255, 255, 255));
		CString strCoord;
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CMechanicalControlPanel* pMechanicalControlView = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
		strCoord.Format(_T("X=%.1f Y=%.1f Z=%.1f"),pMechanicalControlView->m_dCurrentCoord[0],pMechanicalControlView->m_dCurrentCoord[1],
			pMechanicalControlView->m_dCurrentCoord[2]);
		TextOut(hMemDC, ix - 160, iy - 65, strCoord, strCoord.GetLength());
	}
	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
	CString strTime;
	strTime.Format(_T("%d-%2d-%2d %2d：%2d：%2d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
									sysTime.wHour+8, sysTime.wMinute, sysTime.wSecond);
	TextOut(hMemDC, ix - 160, iy - 50, strTime, strTime.GetLength());

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
		AfxMessageBox(_T("创建图片失败！"));
		return FALSE;
	}

	if(!WriteFile(hbmFile, &bmFileHeader, sizeof(BITMAPFILEHEADER),&dwWritten, NULL))
	{
		AfxMessageBox(_T("写图片失败！"));
		CloseHandle(hbmFile);
		return FALSE;
	}
	if(!WriteFile(hbmFile, &(bmInfo.bmiHeader), sizeof(BITMAPINFOHEADER),&dwWritten, NULL))
	{
		AfxMessageBox(_T("写图片失败！"));
		CloseHandle(hbmFile);
		return FALSE;
	}
	if(!WriteFile(hbmFile, lpBitmapBits, dwBitmapArraySize,&dwWritten, NULL))
	{
		AfxMessageBox(_T("写图片失败！"));
		CloseHandle(hbmFile);
		return FALSE;
	}
	CloseHandle(hbmFile);
	
	AfxMessageBox(_T("图片保存成功！"));

	//删除
	DeleteObject(hMemDC);
	DeleteObject(hScrDC);
	DeleteObject(directBmp);
	DeleteObject(previousObject);

	
    */
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

	/* yachang.huang
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

	BYTE *lpBitmapBits = NULL; 

		
	ixStart = rect.left;
	iyStart = rect.top;
	ixStart2 = rect2.left;
	iyStart2 = rect2.top;

	iyStart = rect.top;
	ix = rect.right - rect.left;
	iy = (rect.bottom - rect.top) + (rect2.bottom - rect2.top);

	iy1 = rect.bottom - rect.top;

	dwBitmapArraySize = ((((ix*bitCount) + 31) & ~31) >> 3) * iy;
	hMemDC = CreateCompatibleDC(hScrDC);


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
	CString strCoord;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMechanicalControlPanel* pMechanicalControlView = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
	strCoord.Format(_T("X=%.1f Y=%.1f Z=%.1f"),pMechanicalControlView->m_dCurrentCoord[0],pMechanicalControlView->m_dCurrentCoord[1],
						pMechanicalControlView->m_dCurrentCoord[2]);
	TextOut(hMemDC, ix - 160, iy1 - 65, strCoord, strCoord.GetLength());

	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
	CString strTime;
	strTime.Format(_T("%d-%2d-%2d %2d：%2d：%2d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
						sysTime.wHour+8, sysTime.wMinute, sysTime.wSecond);
	TextOut(hMemDC, ix - 160, iy1 - 50, strTime, strTime.GetLength());
	SetTextColor(hMemDC, RGB(0, 0, 0));
	TextOut(hMemDC, ix - 160, iy - 50, strTime, strTime.GetLength());

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
		AfxMessageBox(_T("创建图片失败！"));
		return FALSE;
	}

	if(!WriteFile(hbmFile, &bmFileHeader, sizeof(BITMAPFILEHEADER),&dwWritten, NULL))
	{
		AfxMessageBox(_T("写图片失败！"));
		CloseHandle(hbmFile);
		return FALSE;
	}
	if(!WriteFile(hbmFile, &(bmInfo.bmiHeader), sizeof(BITMAPINFOHEADER),&dwWritten, NULL))
	{
		AfxMessageBox(_T("写图片失败！"));
		CloseHandle(hbmFile);
		return FALSE;
	}
	if(!WriteFile(hbmFile, lpBitmapBits, dwBitmapArraySize,&dwWritten, NULL))
	{
		AfxMessageBox(_T("写图片失败！"));
		CloseHandle(hbmFile);
		return FALSE;
	}
	CloseHandle(hbmFile);

	AfxMessageBox(_T("图片保存成功！"));

	//删除
	DeleteObject(hMemDC);
	DeleteObject(hScrDC);
	DeleteObject(directBmp);
	DeleteObject(previousObject);


	*/
	return TRUE;
}

BOOL AEC_palte_setall(AECTHREAD_PARAM * pThreadParam)
{
	P9054_HANDLE hDev = pThreadParam->hDev;
	for (int nTempChannelNum = 0;nTempChannelNum < CHANNEL_NUM;nTempChannelNum++)
	{
		CH_CONFIG *pCHX_INIT_CONFIG = &pThreadParam->pAllConfig[nTempChannelNum];

		// temp
		// 写入数据

		WORD channelNO = nTempChannelNum;
		if (channelNO == 0)
		{
			channelNO = 1;
		}
		WORD wValueH = (channelNO - 1)/4 ;
		WORD wValueS = (channelNO - 1)%4 ;

		DWORD dwAddrStart = SOFT_CHANNEL_CONFIG_PARARM_ADDRESS + wValueH * 0x01000 + wValueS * 0x80;



		DWORD dwAddr = dwAddrStart;
		WORD *pData = (WORD *)pCHX_INIT_CONFIG;
		int i = 0;

		while(dwAddr < dwAddrStart + 0x5B) 
		{
			P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, dwAddr, pData[i]);
			dwAddr += 2;
			i++;
		}

	}

	return TRUE;
}