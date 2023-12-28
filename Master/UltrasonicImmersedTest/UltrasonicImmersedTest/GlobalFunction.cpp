#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "GlobalFunction.h"
#include <math.h>
#include "FeatureInfoWnd.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "CScanDlgBar.h"
#include "MechanicalControl.h"
#include "CScanParamDockBar.h"
#include "ARSSocket.h"
#include "WSocket.h"
#include "DataSocket.h"
#include "MovePathView.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "MainFrm.h"
#include "ExButton.h"
#include "SysParam.h"
#include "MechanicalControlPanel.h"
#include "PartGainCompensate.h"		// machao add on 2013-08-15
#include "CScanView.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"

//#include "dibapi.h"
const DWORD MAX_FLAW_COUNT = 200000;
DWORD g_dwActiveChannel = 0;//DEFAULT_CHANNEL;

extern CMutex g_csCard;
//extern CDatabase g_sqlDB;

//WORD g_wDMABuffer[2][DMA_COUNT];
//CH_FLAWDATA_CSCAN_D g_stPrimitiveData[2][PER_CH_DATA_NUM*2];
extern HANDLE *hSaveDMADataEvent;
extern short g_nWaveSpeed;
extern BOOL g_bDrawRealTime; //实时成像
CH_FLAWDATA_CSCAN_S *g_pFeatrueInfo; // wangbingfu add at2011-9-27
BYTE *lpBitmapBits = NULL;
extern bool g_bReFreqFlag;
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
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("RangedRandDemo"),"Enter");

	// Generate random numbers in the half-closed interval
	// [range_min, range_max). In other words,
	// range_min <= random number < range_max
	if(pArray == NULL)
	{
		AfxMessageBox(_T("RangedRandDemo pArray参数为空"));
		return;
	}

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

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("RangedRandDemo"),"Leave");
}

// 卡工作线程
UINT AECWorkThreadProc(LPVOID pParam)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("AECWorkThreadProc"),"Enter");

	if(pParam == NULL)
	{
		AfxMessageBox(_T("AECWorkThreadProc pParam参数为空"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("AECWorkThreadProc"),"Leave");

		return 0;
	}
	// 传递参数
	AECTHREAD_PARAM *pAECThreadProc =(AECTHREAD_PARAM *)pParam;
	int iCardIndex = pAECThreadProc->index;

	CString str;
	BOOL  bThreadExitFlag = FALSE;
	// 命令调用
	// test wangbingfu add at 2011-9-27
//	g_pFeatrueInfo = new CH_FLAWDATA_CSCAN_D[2][PER_CH_DATA_NUM];
//	memset(g_pFeatrueInfo, 0 ,sizeof(CH_FLAWDATA_CSCAN)*60);
	CSingleLock sLock(&g_csCard);

	while(1)
	{
		// 加锁
		if (sLock.Lock(1))
		{

			switch (pAECThreadProc->nMain2AECThreadCommand)
			{
			case MAIN2AECTHREADCOMMAND_INIT:	//卡初始化
				{
					BOOL bRet = TRUE;	//AECCardInit(pAECThreadProc);
					if (bRet)
					{
						pAECThreadProc->nAEC2MainThreadFlag = AECTHREADREURN_INIT_OK;
//						pAECThreadProc->nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
						pAECThreadProc->nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_PLATE_START;
					}
					else
					{
						pAECThreadProc->nAEC2MainThreadFlag = AECTHREADREURN_INIT_FALSE;
					}
					break;
				}
			case MAIN2AECTHREADCOMMAND_PLATE_START:	// 单张钢板检测开始
				{					
					// 避免重复开始
// 					if (pAECThreadProc->bStarted == TRUE)
// 					{
// 						pAECThreadProc->nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
// 						pAECThreadProc->nAEC2MainThreadFlag = AECTHREADREURN_START_OK;
// 					}
// 					else
// 					{
						BOOL nRet = TRUE;	//AEC_plate_Start(pAECThreadProc);
						if(nRet)
						{
							pAECThreadProc->bStarted = TRUE; 
							pAECThreadProc->nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
							pAECThreadProc->nAEC2MainThreadFlag = AECTHREADREURN_START_OK;
						}
						else
						{
							pAECThreadProc->nAEC2MainThreadFlag = AECTHREADREURN_START_FALSE;
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
// 							pAECThreadProc->nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
// 							break;
// 						}
						
						BOOL bRet = TRUE;//AEC_plate_Stop(pAECThreadProc);

						if(bRet)
						{
							pAECThreadProc->nAEC2MainThreadFlag = AECTHREADREURN_STOP_OK; 
							pAECThreadProc->bStarted = FALSE; 
							
							pAECThreadProc->nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
						}
						else
						{
							pAECThreadProc->nAEC2MainThreadFlag = AECTHREADREURN_STOP_FALSE; 
						}

//					}
					break;
				}
			case MAIN2AECTHREADCOMMAND_SETONE:	// SetOne
				{		
					BOOL bRet = AEC_palte_setone(pAECThreadProc);
					if (bRet)
					{
						pAECThreadProc->nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
						pAECThreadProc->nAEC2MainThreadFlag = AECTHREADREURN_SETONE_OK;
					}
					else
						pAECThreadProc->nAEC2MainThreadFlag = AECTHREADREURN_SETONE_FALSE;

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
//					AECThread_Idle(pAECThreadProc);
					pAECThreadProc->nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;

					break;
				}
			case   MAIN2AECTHREADCOMMAND_THREAD_STOP: // 停止线程
				{				
					bThreadExitFlag = TRUE;
					if (pAECThreadProc->bStarted)
					{
					}
					break;
				}
			case MAIN2AECTHREADCOMMAND_SEL_CHANNEL:
				{
//					AEC_selChannel(pAECThreadProc);
					pAECThreadProc->nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
					break;
				}
			default:
				{
					pAECThreadProc->nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
					break;
				}
			}

			// 解锁
 			BOOL bRet = FALSE;
 			do 
 			{
 				bRet = sLock.Unlock();
 			} while (!bRet);
			// test
			/*if(g_bDrawRealTime)
			{
 				g_pFeatrueInfo->wGate1MPAmp = 10;
 				(g_pFeatrueInfo + 1)->wGate1MPAmp = 20;
 				(g_pFeatrueInfo + 2)->wGate1MPAmp = 30;
				PostMessage(pAECThreadProc->hWndCScan, WM_CSCAN_DRAW, (WPARAM)g_pFeatrueInfo, 0);	// wangbingfu add at 2011-9-27
			}*/
			Sleep(25);
		}  
		else 
		{ 			
			Sleep(30);// 需要调整 
		}
		if(bThreadExitFlag)
		{
			pAECThreadProc->nAEC2MainThreadFlag = AECTHREADREURN_THREAD_STOP_OK;

			break; // stop the 
		}
	} 

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("AECWorkThreadProc"),"Leave");
	return 0;
}


int AECThread_Idle(AECTHREAD_PARAM * pThreadParam)
{

	return 0;
} 


BOOL AEC_selChannel(AECTHREAD_PARAM * pThreadParam) //WDC_DEVICE_HANDLE hDev, WORD channelNO, int card)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("AEC_selChannel"),"Enter");

	if (pThreadParam == NULL)
	{
		AfxMessageBox(_T("AEC_selChannel pThreadParam为空！"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("AEC_selChannel"),"Leave");

		return FALSE;
	}

	WORD wValue = 1;
	WORD channelNO = pThreadParam->nSelChannel;

	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_RESET_OFFSET, wValue);

	// set the delay of the pci sample 

	wValue = pThreadParam->pAllConfig[channelNO].stConfig.wMonitorZeroCounter;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_ZERO_OFFSET, wValue);
		
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
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("AEC_selChannel"),"Leave");
//			pApp->m_PLXCommunication.UnInit(pApp->m_stThreadParam);
			ExitProcess(0);
		}
	}

	// 申请SRAM控制权
	begin = GetTickCount();
		
	CString strInfo;
	strInfo.Format(_T("AEC_selChannel函数里执行SelChannel命令, CH_NO = %d\r\n"), channelNO);
	//	WriteLog(_T("标定顺序记录.txt"), strInfo, CFile::modeWrite);

	// 写入数据
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, CHANNEL_SEL_ADDR, channelNO);

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
//			pApp->m_PLXCommunication.UnInit(pApp->m_stThreadParam);
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("AEC_selChannel"),"Leave");
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
	
	wValue = 1;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_RESET_OFFSET, wValue);
	wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_RESET_OFFSET, wValue);



	return TRUE;
}



void Delay(DWORD dwUs)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("Delay"),"Enter");

	DWORD dwUs1 = DELAY_TIME * dwUs;
	for (DWORD i = 0;i < dwUs1;i++) 
	{
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("Delay"),"Leave");
}
/*
BOOL AEC_palte_setone(AECTHREAD_PARAM * pThreadParam)
{
	P9054_HANDLE hDev = pThreadParam->hDev;
	CH_CONFIG *pCHX_INIT_CONFIG = &pThreadParam->pAllConfig[pThreadParam->nSetOneChannel];

	pCHX_INIT_CONFIG->CH_NO = pThreadParam->nSelChannel;
	int chno = pCHX_INIT_CONFIG->CH_NO;

	// 获取控制权
	WORD wValue = 1;

	// Song Chenguang add start on 2009-03-23
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
		if (cha > 3000)
		{
			CString str;
			str.Format(_T("单通道设置查询PCI到DSP标志位失败，请检查卡是否正常或重新启动机器。"));
			AfxMessageBox(str);

			CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
			pApp->m_PLXCommunication.UnInit(pApp->m_stThreadParam);
			ExitProcess(0);
		}
	}

	// 申请SRAM控制权

	// 写入数据
	DWORD dwAddr = SETONE_DATA_ADDR;
	WORD *pData = (WORD *)pCHX_INIT_CONFIG;
	int i = 0;
	while(dwAddr < SETONE_DATA_ADDR_END) 
	{
		P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, dwAddr, pData[i]);
		dwAddr += 2;
		i++;
	}

	pCHX_INIT_CONFIG->CH_NO = chno;

	// 释放SRAM控制权
	wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_SRAM_CONTROL_ADDR, wValue);

	// Song Chenguang add end on 2009-03-23	

	wValue = 1;
	// Song Chenguang add start on 2009-03-24
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_CMD_NUM_ADDR, wValue);

	wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);	

	wValue = (wValue >> 1) << 1; 	
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);
	Sleep(1);
	wValue += 1; 	
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);

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
		if (cha > 3000)
		{
			CString str;
			str.Format(_T("单通道设置查询PCI到DSP反馈信号失败，请检查卡是否正常或重新启动机器。"));
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

// 检查SetOne参数合法性
BOOL CheckSetOneParam(CSCAN_ARS200FPGA_SETONE& stPar)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CheckSetOneParam"),"Enter");

	CString strResult = _T("错误值：\r\n");
	float fGate1SyncThreshold = stPar.stGate.wGate1SyncThreshold / MAX_HEIGHT * 100.0f;
	float fGate2SyncThreshold = stPar.stGate.wGate2SyncThreshold / MAX_HEIGHT * 100.0f;
	float fGate3SyncThreshold = stPar.stGate.wGate3SyncThreshold / MAX_HEIGHT * 100.0f;
	float fGate4SyncThreshold = stPar.stGate.wGate4SyncThreshold / MAX_HEIGHT * 100.0f;

	float fGate1Threshold = stPar.stGate.wGate1Amp / MAX_HEIGHT * 100.0f;
	float fGate2Threshold = stPar.stGate.wGate2Amp / MAX_HEIGHT * 100.0f;
	float fGate3Threshold = stPar.stGate.wGate3Amp / MAX_HEIGHT * 100.0f;
	float fGate4Threshold = stPar.stGate.wGate4Amp / MAX_HEIGHT * 100.0f;
	WORD wRepeaterCount = (int)(float(ADFREQUENCY*1E6) / stPar.stCon.wRepeatCounter / 256.0); 

	BOOL bRet = TRUE;
	if (wRepeaterCount < RE_FREQ_MIN || wRepeaterCount > RE_FREQ_MAX * 8 + 150)	// 重复频率计数器 ( 500-4k)
	{
		CString strError;
		strError.Format(_T("重复频率计数器：  %d\r\n"), wRepeaterCount);
		strResult += strError;

		bRet = FALSE;
	} 
	if (stPar.stCon.wMaiKuan < 2 || stPar.stCon.wMaiKuan > 50) //脉宽 (2-50)
	{
		CString strError;
		strError.Format(_T("脉宽：  %d\r\n"), stPar.stCon.wMaiKuan);
		strResult += strError;

		bRet = FALSE;
	}
	if (fGate1SyncThreshold < 0.0 || fGate1SyncThreshold> 100.0f)	// 闸门1同步阈值  0-100%
	{
		CString strError;
		strError.Format(_T("闸门1同步阈值：  %.1f\r\n"), fGate1SyncThreshold);
		strResult += strError;

		bRet = FALSE;
	}
	if (stPar.stGate.wGate1Start < 3)	//闸门1起点	   >3
	{
		stPar.stGate.wGate1Start=2;
/*
		CString strError;
		strError.Format(_T("闸门1起点：  %d\r\n"), stPar.stGate.wGate1Start);
		strResult += strError;
		bRet = FALSE;
*/

	}
	if (stPar.stGate.wGate1Width < 3)	//闸门1宽度    >3
	{
		stPar.stGate.wGate1Width=2;
/*
		CString strError;
		strError.Format(_T("闸门1宽度：  %d\r\n"), stPar.stGate.wGate1Width);
		strResult += strError;
		bRet = FALSE;
*/

	}
	if (fGate1Threshold < 0.0f || fGate1Threshold > 100.0f)	//闸门1幅值 0-100%
	{
		CString strError;
		strError.Format(_T("闸门1幅值：  %.1f\r\n"), fGate1Threshold);
		strResult += strError;
		bRet = FALSE;
	}
	if (fGate2SyncThreshold < 0.0 || fGate2SyncThreshold> 100.0f)	// 闸门2同步阈值  0-100%
	{
		CString strError;
		strError.Format(_T("闸门2同步阈值：  %1f.\r\n"), fGate2SyncThreshold);
		strResult += strError;
		bRet = FALSE;
	}
	if (stPar.stGate.wGate2Start < 3)	//闸门2起点	   >3
	{
		stPar.stGate.wGate2Start=2;
/*
		CString strError;
		strError.Format(_T("闸门2起点：  %d\r\n"), stPar.stGate.wGate2Start);
		strResult += strError;
		bRet = FALSE;
*/

	}
	if (stPar.stGate.wGate2Width < 3)	//闸门2宽度    >3
	{
		stPar.stGate.wGate2Width=2;
/*
		CString strError;
		strError.Format(_T("闸门2宽度：  %d\r\n"), stPar.stGate.wGate2Width);
		strResult += strError;
		bRet = FALSE;
*/

	}
	if (fGate2Threshold < 0.0f || fGate2Threshold > 100.0f)	//闸门2幅值 0-100%
	{
		CString strError;
		strError.Format(_T("闸门2幅值：  %.1f\r\n"), fGate2Threshold);
		strResult += strError;
		bRet = FALSE;
	}
	if (fGate3SyncThreshold < 0.0 || fGate3SyncThreshold> 100.0f)	// 闸门3同步阈值  0-100% 
	{
		CString strError;
		strError.Format(_T("闸门3同步阈值：  %.1f\r\n"), fGate3SyncThreshold);
		strResult += strError;
		bRet = FALSE;
	}
	if (stPar.stGate.wGate3Start < 3)	//闸门3起点	   >3
	{
		stPar.stGate.wGate3Start=2;
/*
		CString strError;
		strError.Format(_T("闸门3起点：  %d\r\n"), stPar.stGate.wGate3Start);
		strResult += strError;
		bRet = FALSE;
*/

	}
	if (stPar.stGate.wGate3Width < 3)	//闸门3宽度    >3
	{
		stPar.stGate.wGate3Width=2;
/*
		CString strError;
		strError.Format(_T("闸门3宽度：  %d\r\n"), stPar.stGate.wGate3Width);
		strResult += strError;
		bRet = FALSE;
*/

	}
	if (fGate3Threshold < 0.0f || fGate3Threshold > 100.0f)	//闸门3幅值 0-100%
	{
		CString strError;
		strError.Format(_T("闸门3幅值：  %.1f\r\n"), fGate3Threshold);
		strResult += strError;
		bRet = FALSE;
	}
	if (fGate4SyncThreshold < 0.0 || fGate4SyncThreshold> 100.0f)	// 闸门4同步阈值  0-100%
	{
		CString strError;
		strError.Format(_T("闸门4同步阈值：  %.1f\r\n"), fGate4SyncThreshold);
		strResult += strError;
		bRet = FALSE;
	}
	if (stPar.stGate.wGate4Start < 3)	//闸门4起点	   >3
	{
		stPar.stGate.wGate4Start=2;
/*
		CString strError;
		strError.Format(_T("闸门4起点：  %d\r\n"), stPar.stGate.wGate4Start);
		strResult += strError;
		bRet = FALSE;
*/

	}
	if (stPar.stGate.wGate4Width < 3)	//闸门4宽度    >3
	{
		stPar.stGate.wGate4Width=2;
/*
		CString strError;
		strError.Format(_T("闸门4宽度：  %d\r\n"), stPar.stGate.wGate4Width);
		strResult += strError;
		bRet = FALSE;
*/

	}
	if (fGate4Threshold < 0.0f || fGate4Threshold > 100.0f)	//闸门4幅值 0-100% 
	{
		CString strError;
		strError.Format(_T("闸门1幅值：  %.1f\r\n"), fGate4Threshold);
		strResult += strError;
		bRet = FALSE;
	}
	// Song Chenguang modify start on 2013-07-17
	int nAverage = powf(2, stPar.stGate.wSimpleAverage);
	if (nAverage < 1 || nAverage > 64 * ADFREQUENCY / 100)	//通道采样平均数 1-64	因为ADFREQUENCY变成200，所以最大值翻倍 yachang.huang modify on 2013-11-26	
	{
		CString strError;
		strError.Format(_T("通道采样平均数：  %d\r\n"), nAverage);
		strResult += strError;
		bRet = FALSE;
	}
	// Song Chenguang modify end on 2013-07-17
	if (stPar.stGate.wDelay < 1)	//通道延迟时间  >3
	{
		CString strError;
		strError.Format(_T("通道延迟时间：  %d\r\n"), stPar.stGate.wDelay);
		strResult += strError;
		stPar.stGate.wDelay = 3;
		bRet = FALSE;
	}
	if (stPar.stGate.wSimpleInterval > 255)	//通道采样间隔 (0 - 255)	//machao modify on 2013-11-20
	{
		CString strError;
		strError.Format(_T("通道采样间隔 ：  %d\r\n"), stPar.stGate.wSimpleInterval);
		strResult += strError;
		bRet = FALSE;
	}
	if (stPar.fRange < 1 || stPar.fRange > 1000 * ADFREQUENCY / 100)	//声程 1-1000  因为ADFREQUENCY变成200，所以最大值翻倍 yachang.huang modify on 2013-11-26
	{
		CString strError;
		strError.Format(_T("声程 ：  %.1f\r\n"), stPar.fRange);
		strResult += strError;
		bRet = FALSE;
	}
	if (stPar.nVelocity < 1 || stPar.nVelocity > 10000 * ADFREQUENCY / 100)	//声速 1-10000 因为ADFREQUENCY变成200，所以最大值翻倍 yachang.huang modify on 2013-11-26
	{
		CString strError;
		strError.Format(_T("声速 ：  %d\r\n"), stPar.nVelocity);
		strResult += strError;
		bRet = FALSE;
	}	
	
	if(!bRet)
		AfxMessageBox(strResult);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CheckSetOneParam"),"Leave");

	return bRet;
}

BOOL AEC_palte_setone(AECTHREAD_PARAM * pThreadParam)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("AEC_palte_setone"),"Enter");

	if (pThreadParam == NULL)
	{
		AfxMessageBox(_T("AEC_palte_setone pThreadParam参数为空！"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("AEC_palte_setone"),"Leave");

		return FALSE;
	}
// Song Chenguang modify start on 2013-07-17
/*	if(g_bReFreqFlag)	//重复频率 先设卡的1通道 再设2通道 因为硬件改成只有1通道起作用，通道2与通道1一致
	{
		g_bReFreqFlag = FALSE;
		int nChannelIndex = pThreadParam->index * 2 + pThreadParam->nSetOneChannel - 1;
		int chno = 0;
		CSCAN_ARS200FPGA_SETONE *pCHX_INIT_CONFIG = &pThreadParam->pAllConfig[nChannelIndex];

		BOOL bRet = CheckSetOneParam(*pCHX_INIT_CONFIG);

		if (!bRet)
		{
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("AEC_palte_setone"),"Leave");
			return TRUE;
		}

		const int ALL_PARAM_SIZE = sizeof(CSCAN_ARS200FPGA_SETONE);
		char szSendData[NETHEAD_SIZE+ALL_PARAM_SIZE];
		memset( szSendData, 0, sizeof(szSendData));

		PACKET_HEAD_t stPacketHead;
		stPacketHead.dwCommand = COMMAND_ALL_PARAM_SET;
		stPacketHead.wChannelIndex = chno;
		stPacketHead.wGateIndex = 0;
		stPacketHead.dwSize = ALL_PARAM_SIZE;

		memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));
		memcpy( &szSendData[NETHEAD_SIZE], pCHX_INIT_CONFIG, ALL_PARAM_SIZE);
		pThreadParam->pFrame->SendData( szSendData, sizeof(szSendData), nChannelIndex);

		nChannelIndex = pThreadParam->index * 2 + pThreadParam->nSetOneChannel;
		chno = 1;
		pCHX_INIT_CONFIG = &pThreadParam->pAllConfig[nChannelIndex];

		bRet = CheckSetOneParam(*pCHX_INIT_CONFIG);

		if (!bRet)
		{
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("AEC_palte_setone"),"Leave");
			return TRUE;
		}

		memset( szSendData, 0, sizeof(szSendData));
		memset(&stPacketHead, 0, sizeof(stPacketHead));

		stPacketHead.dwCommand = COMMAND_ALL_PARAM_SET;
		stPacketHead.wChannelIndex = chno;
		stPacketHead.wGateIndex = 0;
		stPacketHead.dwSize = ALL_PARAM_SIZE;

		memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));
		memcpy( &szSendData[NETHEAD_SIZE], pCHX_INIT_CONFIG, ALL_PARAM_SIZE);
		pThreadParam->pFrame->SendData( szSendData, sizeof(szSendData), nChannelIndex);
	}
	else
	{
*/
		int nChannelIndex = pThreadParam->index * 2 + pThreadParam->nSetOneChannel;
		int chno = pThreadParam->nSetOneChannel;
		CSCAN_ARS200FPGA_SETONE *pCHX_INIT_CONFIG = &pThreadParam->pAllConfig[nChannelIndex];

		BOOL bRet = CheckSetOneParam(*pCHX_INIT_CONFIG);

		if (!bRet)
		{
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("AEC_palte_setone"),"Leave");
			return TRUE;
		}

		const int ALL_PARAM_SIZE = sizeof(CSCAN_ARS200FPGA_SETONE);
		char szSendData[NETHEAD_SIZE+ALL_PARAM_SIZE];
		memset( szSendData, 0, sizeof(szSendData));

		PACKET_HEAD_t stPacketHead;
		stPacketHead.dwCommand = COMMAND_ALL_PARAM_SET;
		stPacketHead.wChannelIndex = chno;
		stPacketHead.wGateIndex = 0;
		stPacketHead.dwSize = ALL_PARAM_SIZE;

		memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));
		memcpy( &szSendData[NETHEAD_SIZE], pCHX_INIT_CONFIG, ALL_PARAM_SIZE);
		pThreadParam->pFrame->SendData( szSendData, sizeof(szSendData), nChannelIndex);
	
//	}
// Song Chenguang modify end on 2013-07-17
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("AEC_palte_setone"),"Leave");

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
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("GetWorkingFolder"),"Enter");

	if (lpszName != NULL && nCapacity > 0)
		lpszName[0] = _T('\0');
	TCHAR szExeName[MAX_PATH];
	DWORD nSize = ::GetModuleFileName(GetModuleHandle(NULL), szExeName, MAX_PATH);
	if (nSize == 0)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("GetWorkingFolder"),"Leave");

		return 0;
	}
	do {
		nSize--;
	} while(szExeName[nSize] != TCHAR('\\') && nSize > 0);

	if (nSize > 0)
		nSize++;
	if (nSize > nCapacity - 1)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("GetWorkingFolder"),"Leave");
		return nSize;
	}

	if (lpszName == NULL || nCapacity == 0)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("GetWorkingFolder"),"Leave");
		return nSize;
	}

	//	wcsncpy(lpszName, szExeName, nSize);
	memcpy(lpszName, szExeName, nSize*sizeof(TCHAR));
	lpszName[nSize] = _T('\0');

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("GetWorkingFolder"),"Leave");

	return nSize;
}

/*
// 读取DMA数据
//BOOL ReadDMADate(AECTHREAD_PARAM * p, const DWORD& dwLocalAddress)
BOOL ReadDMADate(AECTHREAD_PARAM * p, WORD wSramFlag)
{	
	DWORD dwWaitObjectIndex = WaitForSingleObject(hSaveDMADataEvent[p->index], 0);

	CH_FLAWDATA_CSCAN_D * pData = NULL;
	if (dwWaitObjectIndex != WAIT_OBJECT_0)
	{
		int nIndex1 = g_dwActiveChannel / 2;
		int nIndex2 = g_dwActiveChannel % 2;
		// 读DMA数据
		WORD wDMABuffer[DMA_COUNT];
		::ZeroMemory(wDMABuffer, sizeof(wDMABuffer));
//		P9054_DMAReadWriteBlock(p->hDev, dwLocalAddress, wDMABuffer, sizeof(wDMABuffer), TRUE, P9054_MODE_WORD,P9054_DMA_CHANNEL_0);
		
		if(wSramFlag == 0)
		{
			for(int i = 0; i < DMA_COUNT; i++)
			{
				wDMABuffer[i] = P9054_ReadWord(p->hDev, P9054_ADDR_SPACE0, DSP_2_PCI_DATA_ADRESS1 + 2 + i*2);
			}
		}
		else if(wSramFlag == 1)
		{
			for(int i = 0; i < DMA_COUNT; i++)
			{ 
				wDMABuffer[i] = P9054_ReadWord(p->hDev, P9054_ADDR_SPACE0, DSP_2_PCI_DATA_ADRESS2 + 2 + i*2);
			}
		}
		else
		{
			;//
		}
// 		WORD *pDMA = wDMABuffer;
		WORD *pDMA = wDMABuffer + 1;
		for (int i=0; i<PER_CH_DATA_NUM*2; i++)
		{
			memcpy(&(g_stPrimitiveData[p->index][i]), pDMA, sizeof(CH_FLAWDATA_CSCAN));

			g_stPrimitiveData[p->index][i].wCardNum = p->index;
			if(i < PER_CH_DATA_NUM)
				g_stPrimitiveData[p->index][i].wChannelNo = 0;
			else
				g_stPrimitiveData[p->index][i].wChannelNo = 1;

			pDMA += (sizeof(CH_FLAWDATA_CSCAN) / sizeof(WORD));
		}
		// 通知A扫视图
		
		if(nIndex1 == p->index)
		{
			pData = new CH_FLAWDATA_CSCAN_D;
			if(nIndex2 == 0)
			{
				memcpy(pData, &(g_stPrimitiveData[p->index][0]), sizeof(CH_FLAWDATA_CSCAN_D));
//				memcpy(g_pFeatrueInfo, g_wDMABuffer[p->index] + 1, sizeof(CH_FLAWDATA_CSCAN) * PER_CH_DATA_NUM);
			}	
			else if(nIndex2 == 1)
			{
				memcpy(pData, &(g_stPrimitiveData[p->index][PER_CH_DATA_NUM]), sizeof(CH_FLAWDATA_CSCAN_D));
//				memcpy(g_pFeatrueInfo, g_wDMABuffer[p->index] + 1 + (sizeof(CH_FLAWDATA_CSCAN)*PER_CH_DATA_NUM / 2), sizeof(CH_FLAWDATA_CSCAN) * PER_CH_DATA_NUM);
			}
		}

		SetEvent(hSaveDMADataEvent[p->index]);
	}

	if (pData != NULL)
	{
		PostMessage(p->hWndFeatureInfo, WM_FEATURE_INFO, (WPARAM)pData, 0);

	}

	return TRUE;
}
*/
/*
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
*/
// 由CH_FLAWDATA_CSCAN数据提取坐标
void GetCScanDataCoordinate(const CH_FLAWDATA_CSCAN_S& data, double& dX, double& dY, double& dZ, double& dW1, double& dA, double& dB,double& dW2,
							 int nDw1Flag,	// 0=dW1存管材棒周长; 1=驱动轮角度
							 int nAxisIndex, //平面
							 const int& nCardNo, const DWORD& dwChannelNo
							 )
{
	CUltrasonicImmersedTestApp* pApp = (CUltrasonicImmersedTestApp*)AfxGetApp();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMovePathView* pView = (CMovePathView*) pFrame->m_wndSplitter.GetPane(0,0);
	if (pView == NULL)
	{
		AfxMessageBox(_T("GetCScanDataCoordinate CMovePathView指针获取失败！"));
		return;
	}
	CCScanView* pCscanView = (CCScanView*) pFrame->m_wndSplitter.GetPane(2,0);
	if (pCscanView == NULL)
	{
		AfxMessageBox(_T("GetCScanDataCoordinate CCScanView指针获取失败！"));
		return;
	}
//	double dPipeDiameter =	pCscanView->m_dPipeDiameter;	// 管材棒直径
	double dPipeDiameter = pView->m_GraphMove.GetPipR();
//	DWORD dwXHigh = data.wXHigh16;
	DWORD dwXHigh = data.wXHigh16 & 0x7FFF;
	// Song chenguang add start on 2013-04-03
	WORD  wXFlag2 = (data.wXHigh16 & 0x7FFF) >> 14;
	if (wXFlag2 == 1)
	{
		dwXHigh += 0x8000;
	}
	// Song chenguang add end on 2013-04-03
	
	dX = int(data.wXLow16 + (dwXHigh << 16)) / pApp->m_fCODER_PER_RING_X;
	WORD wXFlag = data.wXHigh16 >> 15;
	if(wXFlag == 0)
		dX += pApp->m_IniParams.m_info.fXCorrect;
//	DWORD dwYHigh = data.wYHigh16;
	DWORD dwYHigh = data.wYHigh16 & 0x7FFF;
	// Song chenguang add start on 2013-04-03
	WORD  wYFlag2 = (data.wYHigh16 & 0x7FFF) >> 14;
	if (wYFlag2 == 1)
	{
		dwYHigh += 0x8000;
	}
	// Song chenguang add end on 2013-04-03
	dY = int(data.wYLow16 + (dwYHigh << 16)) / pApp->m_fCODER_PER_RING_Y;
	// Song chenguang add start on 2013-03-28
	WORD wYFlag = data.wYHigh16 >> 15;
	if(wYFlag == 0)
		dY += pApp->m_IniParams.m_info.fYCorrect;
	// Song chenguang add end on 2013-03-28
//	DWORD dwZHigh = data.wZHigh16;
	// Song Chenguang modify start on 2013-09-13
	if(!pFrame->m_bChangeZWAixs)
	{
		DWORD dwZHigh = data.wZHigh16 & 0x7FFF;
		// Song chenguang add start on 2013-04-03
		WORD  wZFlag2 = (data.wZHigh16 & 0x7FFF) >> 14;
		if (wZFlag2 == 1)
		{
			dwZHigh += 0x8000;
		}
		// Song chenguang add end on 2013-04-03
		dZ = int(data.wZLow16 + (dwZHigh << 16)) / pApp->m_fCODER_PER_RING_Z;

		// Song chenguang add start on 2013-03-28
		WORD wZFlag = data.wZHigh16 >> 15;
		if(wZFlag == 0)
			dZ += pApp->m_IniParams.m_info.fZCorrect;
		// Song chenguang add end on 2013-03-28
	}
	else	// 北方重工Z1占用R，W1占用A
	{
		DWORD dwZHigh = data.wW1High16 & 0x7FFF;
		// Song chenguang add start on 2013-04-03
		WORD  wZFlag2 = (data.wW1High16 & 0x7FFF) >> 14;
		if (wZFlag2 == 1)
		{
			dwZHigh += 0x8000;
		}
		// Song chenguang add end on 2013-04-03
		dZ = int(data.wW1Low16 + (dwZHigh << 16)) / pApp->m_fCODER_PER_RING_Z;

		// Song chenguang add start on 2013-03-28
		WORD wZFlag = data.wW1High16 >> 15;
		if(wZFlag == 0)
			dZ += pApp->m_IniParams.m_info.fZCorrect;
		// Song chenguang add end on 2013-03-28
	}	
	// Song Chenguang modify end on 2013-09-13

	DWORD dwW1High = data.wW1High16 & 0x7FFF;
	// Song chenguang add start on 2013-04-03
	WORD  wW1Flag2 = (data.wW1High16 & 0x7FFF) >> 14;
	if (wW1Flag2 == 1)
	{
		dwW1High += 0x8000;
	}
	// Song chenguang add end on 2013-04-03
	if(nDw1Flag == 0)
	{
		if(dPipeDiameter > 0)
		{
			double dW1Drive = int(data.wW1Low16 + (dwW1High << 16)) / pApp->m_fCODER_PER_RING_W1; // 驱动轮角度
// wangbingfu modify start on 2014-11-03
#if _HAFEI
			float fW1Diameter = pApp->m_IniParams.m_info.fW1Diameter;
			CMechanicalControlPanel* pMechanicalPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
			if (pMechanicalPanel->m_SysParamDlg.m_bTailDecreasePipe)
			{
				fW1Diameter = pFrame->m_fWeiJianW1;
			}
			double dW1Pipe = dW1Drive *( fW1Diameter / dPipeDiameter );
#else
			double dW1Pipe = dW1Drive *( pApp->m_IniParams.m_info.fW1Diameter / dPipeDiameter );
#endif
// wangbingfu modify end on 2014-11-03
			// Song chenguang add start on 2013-03-28
			WORD wW1Flag = data.wW1High16 >> 15;
			if(wW1Flag == 0)
				dW1Pipe += pApp->m_IniParams.m_info.fW1Correct;
			// Song chenguang add end on 2013-03-28
			dW1 = dW1Pipe / 360.0f * PI * dPipeDiameter;
		}	
	}
	else
	{
		double dW1Drive = int(data.wW1Low16 + (dwW1High << 16)) / pApp->m_fCODER_PER_RING_W1; // 驱动轮角度
// wangbingfu add start on 2014-11-03
#if _HAFEI
		float fW1Diameter = pApp->m_IniParams.m_info.fW1Diameter;
		CMechanicalControlPanel* pMechanicalPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
		if (pMechanicalPanel->m_SysParamDlg.m_bTailDecreasePipe)
		{
			fW1Diameter = pFrame->m_fWeiJianW1;
		}
		dW1 = dW1Drive *( fW1Diameter / dPipeDiameter );	//工件角度
#else
		dW1 = dW1Drive *( pApp->m_IniParams.m_info.fW1Diameter / dPipeDiameter );	//工件角度
#endif
// wangbingfu add end on 2014-11-03
		// Song chenguang add start on 2013-03-28
		WORD wW1Flag = data.wW1High16 >> 15;
		if(wW1Flag == 0)
			dW1 += pApp->m_IniParams.m_info.fW1Correct;
		// Song chenguang add end on 2013-03-28
	}

	if(nAxisIndex == 1)	// XZ
	{
		double dTemp = dZ;
		dZ = dY;
		dY = dTemp;
	}
	else if(nAxisIndex == 2)	// YZ
	{
		double dTempX = dX;
		dX = dY;
		dY = dZ;
		dZ = dTempX;
	}
	else
	{
		;//
	}

	DWORD dwAHigh = data.wAHigh16 & 0x7FFF;
	// Song chenguang add start on 2013-04-03
	WORD  wAFlag2 = (data.wAHigh16 & 0x7FFF) >> 14;
	if (wAFlag2 == 1)
	{
		dwAHigh += 0x8000;
	}

	dA = int(data.wALow16 + (dwAHigh << 16)) / pApp->m_fCODER_PER_RING_A;
	WORD wAFlag = data.wAHigh16 >> 15;
	if(wAFlag == 0)
		dA += pApp->m_IniParams.m_info.fACorrect;

	if (!pFrame->m_bChangeZWAixs)
	{
		DWORD dwBHigh = data.wBHigh16 & 0x7FFF;

		WORD  wBFlag2 = (data.wBHigh16 & 0x7FFF) >> 14;
		if (wBFlag2 == 1)
		{
			dwBHigh += 0x8000;
		}
		dB = int(data.wBLow16 + (dwBHigh << 16)) / pApp->m_fCODER_PER_RING_B;
		WORD wBFlag = data.wBHigh16 >> 15;
		if(wBFlag == 0)
			dB += pApp->m_IniParams.m_info.fBCorrect;

	} 
	else	// 北方重工Z1占用R，W1占用A
	{
		DWORD dwBHigh = data.wW2High16 & 0x7FFF;
		// Song chenguang add start on 2013-04-03
		WORD  wBFlag2 = (data.wW2High16 & 0x7FFF) >> 14;
		if (wBFlag2 == 1)
		{
			dwBHigh += 0x8000;
		}
		// Song chenguang add end on 2013-04-03
		dB = int(data.wW2Low16 + (dwBHigh << 16)) / pApp->m_fCODER_PER_RING_B;
		// Song chenguang add start on 2013-03-28
		WORD wBFlag = data.wW2High16 >> 15;
		if(wBFlag == 0)
			dB += pApp->m_IniParams.m_info.fBCorrect;
		// Song chenguang add end on 2013-03-28
	}
	// Song Chenguang modify end on 2013-09-13
	
	DWORD dwW2High = data.wW2High16 & 0x7FFF;
	// Song chenguang add start on 2013-04-03
	WORD  wW2Flag2 = (data.wW2High16 & 0x7FFF) >> 14;
	if (wW2Flag2 == 1)
	{
		dwW2High += 0x8000;
	}
	// Song chenguang add end on 2013-04-03
	dW2 = int(data.wW2Low16 + (dwW2High << 16)) / pApp->m_fCODER_PER_RING_W2;
	// Song chenguang add start on 2013-03-28
	WORD wW2Flag = data.wW2High16 >> 15;
	if(wW2Flag == 0)
		dW2 += pApp->m_IniParams.m_info.fW2Correct;
	// Song chenguang add end on 2013-03-28
	
	double dParam[SYSPARAM1_NUM];
	::ZeroMemory(dParam, sizeof(dParam));
	long lLogParam[10];
	::ZeroMemory(lLogParam, sizeof(lLogParam));
	pView->m_GraphMove.GetPara(dParam, lLogParam);

	CString strProbeNum = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("ProbeNum"), _T("1"));
	int nProbeNum = _wtoi(strProbeNum.GetBuffer(strProbeNum.GetLength()));
	float fDistance = 0.0f;
	
/*	if (nProbeNum == 2)
	{
		CString strTwoProbeType = pApp->GetProfileString(_T("水浸超声探伤系统"), _T("TwoProbeType"), _T("1"));
		int nTwoProbeType = _wtoi(strTwoProbeType.GetBuffer(strTwoProbeType.GetLength()));

		if (nTwoProbeType > 2 && nTwoProbeType < 5)
		{
			fDistance = dParam[19] / 2;
		}
		else if (nTwoProbeType == 5)
		{
			fDistance = dParam[19] / 3;
		}
		else
			fDistance = dParam[19];
	}*/
/*
	fDistance = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_fProbeDistance;
	if (lLogParam[0] == 0) // 横向
	{
		dY += fDistance*(nCardNo*2 + dwChannelNo);
	} 
	else	// 纵向
	{
		dX += fDistance*(nCardNo*2 + dwChannelNo);
	}
*/
	// rem by Song Chenguang on 2013-10-18
	// 深度扫查时也要计算探头偏移
//	if(pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_nScanType == 0)
	{
		int nReakChannelIndex = 0;
		if (pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_wWorkMode <= 3)
		{
			nReakChannelIndex = nCardNo * 2 + dwChannelNo;
		}
		else
		{
			nReakChannelIndex = nCardNo*2 + 1; // wangbingfu modify at 2012-9-19 ,2,4,6,8 关联都是2通道接收
		}

		dX += pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_fProbeLocation[nReakChannelIndex][0];
		dY += pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_fProbeLocation[nReakChannelIndex][1];
		dZ += pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_SysParamDlg.m_fProbeLocation[nReakChannelIndex][2];	
	}

	// Song Chenguang add start on 2013-09-13
#if _BEIFANG_ZHONGGONG
	// 北方重工无R轴，预防出现非法值
	dW1 = 0;
	dW2 = 0;
#endif
	// Song Chenguang add end on 2013-09-13
	// 轴开关 wangbingfu add start on 2014-09-09
	if(!pApp->m_IniParams.m_info.bXEnable)
	{
		dX = 0.0f;
	}
	
	if(!pApp->m_IniParams.m_info.bYEnable)
	{
		dY = 0.0f;
	}

	if(!pApp->m_IniParams.m_info.bZEnable)
	{
		dZ = 0.0f;
	}

	if(!pApp->m_IniParams.m_info.bAEnable)
	{
		dA = 0.0f;
	}

	if(!pApp->m_IniParams.m_info.bBEnable)
	{
		dB = 0.0f;
	}

	if(!pApp->m_IniParams.m_info.bW1Enable)
	{
		dW1 = 0.0f;
	}

	if(!pApp->m_IniParams.m_info.bW2Enable)
	{
		dW2 = 0.0f;
	}
	// 轴开关 wangbingfu add end on 2014-09-09
}

BOOL PrimitiveDataChange(const CH_FLAWDATA_CSCAN_S& Cdata, CFeatureInfo& Fdata, const int& nCardNo, const DWORD& dwChannelNo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("PrimitiveDataChange"),"Enter");

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	int nAxisIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();
	// Song Chenguang add start on 2013-09-06
//	int nRMode = pFrame->m_wndCScanParamDockBar.m_pCScanParamView->m_nPipeXMode;
	// Song Chenguang add end on 2013-09-06
	GetCScanDataCoordinate(Cdata, Fdata.m_dX, Fdata.m_dY, Fdata.m_dZ, Fdata.m_dR, Fdata.m_dB, Fdata.m_dW, Fdata.m_dA, 0, nAxisIndex, nCardNo, dwChannelNo);

//	WORD wChannelNo = Cdata.wCardNum*2 + Cdata.wChannelNo;
	WORD wChannelNo = 0;

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

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMTimeDisplay)
		{

			j++;
		}

		if(pFrame->m_pFeatureWnd->m_bMPFTimeDisplay)
		{


			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMTimeDisplay)
		{
			if(bMTPmode[0])
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate1MPTime);
			else
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate1MPTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMAltitudeDisplay)
		{
/*			Fdata.m_pfFeatures[i][j] = Cdata.wGate1FPAmp / MAX_HEIGHT *100.0f;*/
			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = (Cdata.wGate1MPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}

		i++;
	}

	if (pFrame->m_pFeatureWnd->m_bGateOpen[1])
	{
		int j = 1;		

//		if (bMTPmode[1])
// 			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate2SyncTime) / ADFREQUENCY + fDelay;
// 		else
// 			Fdata.m_pfFeatures[i][0] = float(Cdata.wGate2SyncTime) / ADFREQUENCY;

		if (pFrame->m_pFeatureWnd->m_bFPFTimeDisplay)
		{
			if (bMTPmode[1])
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate2FPFirstTime);
			else
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate2FPFirstTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMTimeDisplay)
		{
			if (bMTPmode[1])
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate2FPTime);
			else
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate2FPTime + dwDelayDot);

			j++;
		}

		if(pFrame->m_pFeatureWnd->m_bMPFTimeDisplay)
		{


			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMTimeDisplay)
		{
			if(bMTPmode[1])
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate2MPTime);
			else
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate2MPTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = (Cdata.wGate2FPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = (Cdata.wGate2MPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}

		i++;
	}

	if(pFrame->m_pFeatureWnd->m_bGateOpen[2])
	{
		int j = 1;


		if (pFrame->m_pFeatureWnd->m_bFPFTimeDisplay)
		{
			if(bMTPmode[2])
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate3FPFirstTime);
			else
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate3FPFirstTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMTimeDisplay)
		{
			if(bMTPmode[2])
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate3FPTime);
			else
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate3FPTime + dwDelayDot);

			j++;
		}

		if(pFrame->m_pFeatureWnd->m_bMPFTimeDisplay)
		{


			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMTimeDisplay)
		{
			if(bMTPmode[2])
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate3MPTime);
			else
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate3MPTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = (Cdata.wGate3FPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = (Cdata.wGate3MPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}

		i++;
	}

	if (pFrame->m_pFeatureWnd->m_bGateOpen[3])
	{
		int j = 1;


		if (pFrame->m_pFeatureWnd->m_bFPFTimeDisplay)
		{
			if(bMTPmode[3])
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate4FPFirstTime);
			else
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate4FPFirstTime + dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMTimeDisplay)
		{
			if(bMTPmode[3])
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate4FPTime);
			else
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate4FPTime + dwDelayDot);

			j++;
		}

		if(pFrame->m_pFeatureWnd->m_bMPFTimeDisplay)
		{

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMTimeDisplay)
		{
			if(bMTPmode[3])
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate4MPTime);
			else
				Fdata.m_pfFeatures[i][j] = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, Cdata.wGate4MPTime +dwDelayDot);

			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bFPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = (Cdata.wGate4FPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}

		if (pFrame->m_pFeatureWnd->m_bMPMAltitudeDisplay)
		{
			Fdata.m_pfFeatures[i][j] = (Cdata.wGate4MPAmp & 0x7FF) / MAX_HEIGHT *100.0f;
			j++;
		}

		i++;
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("PrimitiveDataChange"),"Leave");

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
#if 0
BOOL ScreenCapture(LPCWSTR fileName, WORD bitCount, HDC hScrDC, RECT rect, int nType)
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
	int ix = 0; 
	int iy = 0;
	//the pointer will save all pixel point's color value
	BYTE *lpBitmapBits = NULL; 

	//creates a device context for the screen device
	/*	hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

	//get the screen point size
	nScreenWidth = GetDeviceCaps(hScrDC, HORZRES);
	nScreenHeight = GetDeviceCaps(hScrDC, VERTRES);
	*/
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
	
//	::MoveToEx(hMemDC, 0, 0, NULL);
//	::LineTo(hMemDC, ix, iy);
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

	return TRUE;
}
#else		//不保存图片 后面再保存
BOOL ScreenCapture(HBITMAP directBmp, BITMAPINFO *bmInfo, BITMAPFILEHEADER *bmFileHeader,
				   WORD bitCount, HDC hScrDC, RECT rect, int nType, int nObjectNum, int nPictureMode)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("ScreenCapture"),"Enter");

	// wangbingfu modify start at 2012-11-19
	// Song Chenguang add start on 2012-11-03
	if (bmInfo == NULL)
	{
		AfxMessageBox(_T("ScreenCapture bmInfo参数为空值"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("ScreenCapture"),"Leave");

		return FALSE;
	}
	if (bmFileHeader == NULL)
	{
		AfxMessageBox(_T("ScreenCapture bmFileHeader参数为空值！"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("ScreenCapture"),"Leave");

		return FALSE;
	}
	// Song Chenguang add end on 2012-11-03
	int nColorMode = 0;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParam = NULL;
	CCScanView* pCScanView = NULL;

	if(nObjectNum == 1)
	{
		pCScanParam = (CCScanParamPanel*)pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);		
		nColorMode = pCScanParam->m_nColorMode;
	}
	else
	{
		pCScanParam = (CCScanParamPanel*)pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
		nColorMode = pCScanParam->m_nColorMode;
	}

	// Song Chenguang add start on 2012-11-03
	if (pCScanView == NULL)
	{
		AfxMessageBox(_T("ScreenCapture C扫1视图指针获取失败！"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("ScreenCapture"),"Leave");

		return FALSE;
	}
	// Song Chenguang add end on 2012-11-03

	HANDLE hbmFile = NULL;
	DWORD dwWritten = 0;
	DWORD dwBitmapArraySize;//位图数据大小
	//	HDC hScrDC = NULL;
	HDC hMemDC = NULL;         
	int nScreenWidth = 0; 
	int nScreenHeight = 0;
	int ixStart = 0; 
	int iyStart = 0; 
	int ix = 0;		// 整个图X长度
	int iy = 0;		// 整个图Y长度
	int ix1 = 0;	// C扫区域X长度
	int iy1 = 0;	// C扫区域Y长度

	ixStart = rect.left;
	iyStart = rect.top;
	int nVLength = 100;//增加的色条区域长度
	if(nType == 1)
	{
		if(nPictureMode == 3)	// 画坐标轴
		{
			ix = rect.right - rect.left + nVLength + CSCAN_SCALE_X_LENGTH;
			iy = rect.bottom - rect.top + CSCAN_SCALE_Y_LENGTH;
		}
		else
		{
			ix = rect.right - rect.left + nVLength;
			iy = rect.bottom - rect.top;
		}
	}
	else
	{
		ix = rect.right - rect.left;
		iy = rect.bottom - rect.top;
	}
	iy1 = rect.bottom - rect.top;
	ix1 = rect.right - rect.left;
	// wangbingfu add start on 2012-12-7
	if(nPictureMode == 3)	// 原尺寸
	{
		ix1 = ix1 + CSCAN_SCALE_X_LENGTH;
	}
	// wangbingfu add start on 2012-12-7

	//creates a memory device context (DC) compatible with the screen device(hScrDC) 
	dwBitmapArraySize = ((((ix*bitCount) + 31) & ~31) >> 3) * iy;
	hMemDC = CreateCompatibleDC(hScrDC);


	bmInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo->bmiHeader.biWidth = ix;
	bmInfo->bmiHeader.biHeight = iy;
	bmInfo->bmiHeader.biPlanes = 1;
	bmInfo->bmiHeader.biBitCount = bitCount;

	//use the function CreateDIBSection and SelectObject 
	//in order to get the bimap pointer : lpBitmapBits 
	directBmp = CreateDIBSection(hMemDC, bmInfo, 
		DIB_RGB_COLORS, (void **)&lpBitmapBits, NULL, 0);
	HGDIOBJ previousObject = SelectObject(hMemDC, directBmp);
	
	// wangbingfu add start at 2012-11-19
	TEXTMETRIC tm;
	GetTextMetrics(hMemDC, &tm);
	LONG lCharLength = 0;
	// wangbingfu add end at 2012-11-19
	// copy the screen dc to the memory dc
	if(nPictureMode == 3)	// 原尺寸需要画坐标轴
	{
		BitBlt(hMemDC, CSCAN_SCALE_X_LENGTH, 0, ix1, iy1, hScrDC, ixStart, iyStart, SRCCOPY);
	}
	else
	{
		BitBlt(hMemDC, 0, 0, ix1, iy1, hScrDC, ixStart, iyStart, SRCCOPY);
	}

	if(nType == 1)
	{
		CRect rectOther(ix1, 0, ix, iy1);
		CBrush brush(RGB(255,255,255));
		FillRect(hMemDC, &rectOther, brush);

		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
		HPEN hOldPen = (HPEN)SelectObject(hMemDC, hPen);	
		MoveToEx(hMemDC, ix1, 0, NULL);
		LineTo(hMemDC, ix1, iy1);
		SelectObject(hMemDC, hOldPen);
		DeleteObject(hPen);

		CRect rectV(ix1, 0 + 10, ix - 50, iy1 - 10);

		DrawPattleRule( hMemDC, pCScanParam->m_fBoundMin, pCScanParam->m_fBoundMax, rectV);

		if(nColorMode == 1)
		{
			DrawPattle(hMemDC, rectV, pCScanView, pCScanParam);
		}
		else
		{
			DrawPattle(hMemDC,pCScanParam->m_fBeginGrayValue, pCScanParam->m_fEndGrayValue, rectV);
		}
		
		SetBkMode(hMemDC,TRANSPARENT);
		SetTextColor(hMemDC, RGB(0, 0, 0));

		if(nPictureMode == 3)	// 画坐标轴
		{		
			// wangbingfu modify start on 2012-12-7
			//CRect rectAll(0, 0, ix - nVLength - CSCAN_SCALE_X_LENGTH, iy);	//除了色条区域
			CRect rectAll(0, 0, ix - nVLength, iy);	//除了色条区域
			// wangbingfu modify end on 2012-12-7

			// X坐标区域
			CRect rectScaleX;
			POINT XScaleTop;
			POINT XScaleBottom;
			XScaleTop.x = CSCAN_SCALE_X_LENGTH;
			XScaleTop.y = rectAll.bottom - CSCAN_SCALE_Y_LENGTH;
			XScaleBottom.x = rectAll.right;
			XScaleBottom.y = rectAll.bottom;
			rectScaleX.SetRect(XScaleTop,XScaleBottom);

			// Y坐标区域
			CRect rectScaleY;
			POINT YScaleTop;
			POINT YScaleBottom;
			YScaleTop.x = rectAll.left;
			YScaleTop.y = rectAll.top;
			YScaleBottom.x = CSCAN_SCALE_X_LENGTH - 1;
			YScaleBottom.y = rectAll.bottom - CSCAN_SCALE_Y_LENGTH;
			rectScaleY.SetRect(YScaleTop,YScaleBottom);

			float fXmin = pCScanView->m_dXStart;
			float fXmax = pCScanView->m_dXEnd;
			float fYmax = pCScanView->m_dYEnd;
			float fYmin = pCScanView->m_dYStart;
			// wangbingfu add start at 2012-12-5
			if(pCScanView->m_nAxisSelect == 3
				|| pCScanView->m_nAxisSelect == 4)	// wangbingfu add on 2013-01-09
			{
				if(pCScanView->m_nPipeXMode == 0) // 角度
				{
					fYmax = 360;
				}
			}
			// wangbingfu add end at 2012-12-5
			// Song Chenguang add start at 2012-12-5
			else if (pCScanView->m_nAxisSelect == 5)
			{
				if(pCScanView->m_nPipeXMode == 0) // 角度
				{
					fXmax = 360;
				}
			}
			// Song Chenguang add end at 2012-12-5
			
			CRect rectBottom(0,rectAll.bottom-CSCAN_SCALE_Y_LENGTH,ix,iy);
			HBRUSH hBrushWhite = CreateSolidBrush(RGB(255,255,255));
			FillRect(hMemDC, &rectBottom, hBrushWhite);
			// 画X坐标
			float fStep  = (fXmax - fXmin) / 10;
		//	pCScanView->DrawScale((CDC *)&hMemDC, rectScaleX, fXmin, fXmax, fStep, 3, TRUE);
			GlobalDrawScale(hMemDC, rectScaleX, fXmin, fXmax, fStep, 3, TRUE);

			// 画Y坐标
			fStep  = (fYmax - fYmin) / 10;
		//	pCScanView->DrawScale((CDC *)&hMemDC, rectScaleY, fYmin, fYmax, fStep, 4, TRUE);
			GlobalDrawScale(hMemDC, rectScaleY, fYmin, fYmax, fStep, 4, TRUE);

			CRect rectSpace(rectAll.left,rectAll.bottom-CSCAN_SCALE_Y_LENGTH,CSCAN_SCALE_X_LENGTH,rectAll.bottom);
			HBRUSH hBrushScale = CreateSolidBrush(RGB(0,192,0));
			FillRect(hMemDC, &rectSpace, hBrushScale);
			CString str;
			str.Format(_T("C扫%d"),nObjectNum);
			TextOut(hMemDC, rectSpace.left +5, rectSpace.bottom-20, str, str.GetLength());
			DeleteObject(hBrushWhite);
			DeleteObject(hBrushScale);
		}
	}	
	else if(nType == 2)	// A扫
	{
		SetBkMode(hMemDC,TRANSPARENT);
		SetTextColor(hMemDC, RGB(255, 255, 255));
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();		
		CMechanicalControlPanel* pMechanicalControlView = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
		CString strCoordX = _T("");	
		CString strCoordY = _T("");
		CString strCoordZ = _T("");
		pMechanicalControlView->m_staticXCoord.GetWindowText(strCoordX);	
		pMechanicalControlView->m_staticYCoord.GetWindowText(strCoordY);
		pMechanicalControlView->m_staticZCoord.GetWindowText(strCoordZ);

		CString strCoord = _T("");
		strCoord = strCoordX + _T(" ") + strCoordY + _T(" ") +strCoordZ;
		lCharLength = tm.tmAveCharWidth * strCoord.GetLength();
		TextOut(hMemDC, ix1 - lCharLength, iy1 - 65, strCoord, strCoord.GetLength());
	}
	else
	{
		;//
	}

	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
	CString strTime;
	strTime.Format(_T("%d-%2d-%2d %2d：%2d：%2d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
		sysTime.wHour+8, sysTime.wMinute, sysTime.wSecond);
	if(lCharLength == 0)
	{
		TextOut(hMemDC, ix1 - 160, iy1 - 50, strTime, strTime.GetLength());
	}
	else
	{
		TextOut(hMemDC, ix1 - lCharLength, iy1 - 50, strTime, strTime.GetLength());
	}
	//if you only want to get the every pixel color value, 
	//you can begin here and the following part of this function will be unuseful;
	//the following part is in order to write file;

	//bimap file header in order to write bmp file 
//	BITMAPFILEHEADER bmFileHeader;
//	ZeroMemory(&bmFileHeader, sizeof(BITMAPFILEHEADER));
	bmFileHeader->bfType = 0x4d42; //bmp 
	bmFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmFileHeader->bfSize = bmFileHeader->bfOffBits + dwBitmapArraySize;

	// 删除
	// Song Chenguang modified start on 2012-11-03
	SelectObject(hMemDC, previousObject);
//	DeleteObject(directBmp);
	DeleteDC(hMemDC);
	// Song Chenguang modified end on 2012-11-03
	// wangbingfu modify end at 2012-11-19
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("ScreenCapture"),"Leave");

	return dwBitmapArraySize;
}
#endif
/**************************
*截取指定区域的屏幕，并保存文件
* @param	fileName[in]:保存位图文件的文件路径，如果为NULL则不保存
* @param	bitCount[in]:bit深度，用于表示一个像素点所使用的数据长度
* @param	hScrDC[HDC]:要截取的DC
* @param	lpRect[in]:所要截取的DC的区域
* @return   BITMAP
* @since    1.00
***************************/
#if 0
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

	return TRUE;
}
#else	//不保存图片 后面再保存
DWORD ScreenCapture2(HBITMAP directBmp, BITMAPINFO *bmInfo, BITMAPFILEHEADER *bmFileHeader, 
					 WORD bitCount, HDC hScrDC, RECT rect, HDC hScrDC2, RECT rect2, int nObjectNum)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("ScreenCapture2"),"Enter");
	// wangbingfu modify start at 2012-11-19
	// Song Chenguang add start on 2012-11-03	
	if (bmInfo == NULL)
	{
		AfxMessageBox(_T("ScreenCapture2 bmInfo句柄为空！"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("ScreenCapture2"),"Leave");

		return 0;
	}
	if (bmFileHeader == NULL)
	{
		AfxMessageBox(_T("ScreenCapture2 bmFileHeader句柄为空！"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("ScreenCapture2"),"Leave");

		return 0;
	}
	if (hScrDC == NULL)
	{
		AfxMessageBox(_T("ScreenCapture2 hScrDC句柄为空！"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("ScreenCapture2"),"Leave");

		return 0;
	}
	if (hScrDC2 == NULL)
	{
		AfxMessageBox(_T("ScreenCapture2 hScrDC2句柄为空！"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("ScreenCapture2"),"Leave");

		return 0;
	}
	// Song Chenguang add end on 2012-11-03
	int nColorMode = 0;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParam = NULL;
	CCScanView* pCScanView = NULL;

	if(nObjectNum == 1)
	{
		pCScanParam = (CCScanParamPanel*)pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);

		if (pCScanView == NULL)
		{
			AfxMessageBox(_T("获取C扫1视图指针失败！"));

			LOG4CXX_ERROR(log4cxx::Logger::getLogger("ScreenCapture2"),"Leave");

			return 0;
		}
		nColorMode = pCScanParam->m_nColorMode;
	}
	else
	{
		pCScanParam = (CCScanParamPanel*)pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);

		if(pCScanView == NULL)
		{
			AfxMessageBox(_T("获取C扫2视图指针失败！"));

			LOG4CXX_ERROR(log4cxx::Logger::getLogger("ScreenCapture2"),"Leave");

			return 0;
		}

		nColorMode = pCScanParam->m_nColorMode;
	}

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
	int ix1 = 0;
	int iy = 0;
	int iy1 = 0;
	int iy2 = 0;
	//the pointer will save all pixel point's color value
//	BYTE *lpBitmapBits = NULL; 

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
	ix = rect.right - rect.left + 100;
	iy = (rect.bottom - rect.top) + (rect2.bottom - rect2.top);
	
	ix1 = rect.right - rect.left;
	iy1 = rect.bottom - rect.top;
	//creates a memory device context (DC) compatible with the screen device(hScrDC) 
	dwBitmapArraySize = ((((ix*bitCount) + 31) & ~31) >> 3) * iy;
	hMemDC = CreateCompatibleDC(hScrDC);

	//initialise the struct BITMAPINFO for the bimap infomation， 
	//in order to use the function CreateDIBSection
	// on wince os, each pixel stored by 24 bits(biBitCount=24) 
	//and no compressing(biCompression=0) 
//	BITMAPINFO bmInfo; 
//	ZeroMemory(bmInfo, sizeof(BITMAPINFO));
	bmInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo->bmiHeader.biWidth = ix;
	bmInfo->bmiHeader.biHeight = iy;
	bmInfo->bmiHeader.biPlanes = 1;
	bmInfo->bmiHeader.biBitCount = bitCount;

	//use the function CreateDIBSection and SelectObject 
	//in order to get the bimap pointer : lpBitmapBits 
	directBmp = CreateDIBSection(hMemDC, bmInfo, 
		DIB_RGB_COLORS, (void **)&lpBitmapBits, NULL, 0);
	HGDIOBJ previousObject = SelectObject(hMemDC, directBmp);

	// copy the screen dc to the memory dc
	BitBlt(hMemDC, 0, 0, ix1, iy1, hScrDC, ixStart, iyStart, SRCCOPY);
	BitBlt(hMemDC, 0, iy1, ix1, iy, hScrDC2, ixStart2, iyStart2, SRCCOPY);

	CRect rectOther(ix1, 0, ix, iy);
	CBrush brush(RGB(255,255,255));
	FillRect(hMemDC, &rectOther, brush);

	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
	HPEN hOldPen = (HPEN)SelectObject(hMemDC, hPen);	
	MoveToEx(hMemDC, ix1, 0, NULL);
	LineTo(hMemDC, ix1, iy);
	SelectObject(hMemDC, hOldPen);
	DeleteObject(hPen);

	CRect rectV(ix1, 0 + 10, ix - 50, iy - 10);

	DrawPattleRule( hMemDC, 0, 100, rectV);

	if(nColorMode == 1)
	{
		DrawPattle(hMemDC, rectV, pCScanView, pCScanParam);
	}
	else
	{
		DrawPattle(hMemDC,pCScanParam->m_fBeginGrayValue, pCScanParam->m_fEndGrayValue, rectV);
	}


	SetBkMode(hMemDC,TRANSPARENT);
	SetTextColor(hMemDC, RGB(255, 255, 255));

	CMechanicalControlPanel* pMechanicalControlView = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
	CString strCoordX = _T("");	
	CString strCoordY = _T("");
	CString strCoordZ = _T("");
	pMechanicalControlView->m_staticXCoord.GetWindowText(strCoordX);	
	pMechanicalControlView->m_staticYCoord.GetWindowText(strCoordY);
	pMechanicalControlView->m_staticZCoord.GetWindowText(strCoordZ);

	CString strCoord = _T("");
	strCoord = strCoordX + _T(" ") + strCoordY + _T(" ") +strCoordZ;

	TEXTMETRIC tm;
	GetTextMetrics(hMemDC, &tm);
	LONG lCharLength = 0;
	lCharLength = tm.tmAveCharWidth * strCoord.GetLength();
	
	TextOut(hMemDC, ix1 - lCharLength, iy1 - 65, strCoord, strCoord.GetLength());

	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
	CString strTime;
	strTime.Format(_T("%d-%2d-%2d %2d：%2d：%2d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
		sysTime.wHour+8, sysTime.wMinute, sysTime.wSecond);
	TextOut(hMemDC, ix1 - lCharLength, iy1 - 50, strTime, strTime.GetLength());
	SetTextColor(hMemDC, RGB(0, 0, 0));
	TextOut(hMemDC, ix1 - 160, iy - 50, strTime, strTime.GetLength());
	//if you only want to get the every pixel color value, 
	//you can begin here and the following part of this function will be unuseful;
	//the following part is in order to write file;

	//bimap file header in order to write bmp file 
//	BITMAPFILEHEADER bmFileHeader;
//	ZeroMemory(bmFileHeader, sizeof(BITMAPFILEHEADER));
	bmFileHeader->bfType = 0x4d42; //bmp 
	bmFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmFileHeader->bfSize = bmFileHeader->bfOffBits + dwBitmapArraySize;
	
	//删除
	// Song Chenguang add start on 2012-11-03
	SelectObject(hMemDC, previousObject);
//	DeleteObject(directBmp);
	DeleteDC(hMemDC);
	// Song Chenguang add end on 2012-11-03
	// wangbingfu modify end at 2012-11-19
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("ScreenCapture2"),"Leave");

	return dwBitmapArraySize;
}
#endif

//存C扫图，加伤点信息
BOOL SaveCscanWithInfo(HBITMAP directBmp, BITMAPINFO *bmInfo, BITMAPFILEHEADER *bmFileHeader,
				   WORD bitCount, HDC hScrDC, RECT rect, int nObjectNum, int nPictureMode)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("SaveCscanWithInfo"),"Enter");

	// Song Chenguang add start on 2012-11-03
	if (bmInfo == NULL)
	{
		AfxMessageBox(_T("SaveCscanWithInfo bmInfo参数为空"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("SaveCscanWithInfo"),"Leave");

		return FALSE; 
	}
	if (bmFileHeader == NULL)
	{
		AfxMessageBox(_T("SaveCscanWithInfo bmFileHeader参数为空"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("SaveCscanWithInfo"),"Leave");

		return FALSE;
	}
	if (hScrDC == NULL)
	{
		AfxMessageBox(_T("SaveCscanWithInfo hScrDC参数为空"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("SaveCscanWithInfo"),"Leave");

		return FALSE;
	}
	// Song Chenguang add end on 2012-11-03
	int nColorMode = 0;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanParamPanel* pCScanParam = NULL;
	CCScanView* pCScanView = NULL;

	if(nObjectNum == 1)
	{
		pCScanParam = (CCScanParamPanel*)pFrame->m_wndCScanParamDockBar.m_pCScanParamView;
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);
		if (pCScanView == NULL)
		{
			AfxMessageBox(_T("SaveCscanWithInfo 获取C扫1视图指针失败！"));

			LOG4CXX_ERROR(log4cxx::Logger::getLogger("SaveCscanWithInfo"),"Leave");

			return FALSE;
		}
		nColorMode = pCScanParam->m_nColorMode;
	}
	else
	{
		pCScanParam = (CCScanParamPanel*)pFrame->m_wndCScanParamDockBar.m_pCScanParamView2;
		pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(3,0);
		if (pCScanView == NULL)
		{
			AfxMessageBox(_T("SaveCscanWithInfo 获取C扫2视图指针失败！"));

			LOG4CXX_ERROR(log4cxx::Logger::getLogger("SaveCscanWithInfo"),"Leave");

			return FALSE;
		}
		nColorMode = pCScanParam->m_nColorMode;
	}

	HANDLE hbmFile = NULL;
	DWORD dwWritten = 0;
	DWORD dwBitmapArraySize;//位图数据大小
	//	HDC hScrDC = NULL;
	HDC hMemDC = NULL;         
	int nScreenWidth = 0; 
	int nScreenHeight = 0;
	int ixStart = 0; 
	int iyStart = 0; 
	int ix = 0;		// 整个图X长度
	int iy = 0;		// 整个图Y长度
	int ix1 = 0;	// C扫区域X长度
	int iy1 = 0;	// C扫区域Y长度
	//the pointer will save all pixel point's color value
	//	BYTE *lpBitmapBits = NULL; 

	//creates a device context for the screen device
	/*	hScrDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);

	//get the screen point size
	nScreenWidth = GetDeviceCaps(hScrDC, HORZRES);
	nScreenHeight = GetDeviceCaps(hScrDC, VERTRES);
	*/
	ixStart = rect.left;
	iyStart = rect.top;
	int nVLength = 100;//增加的色条区域长度

	if(nPictureMode == 3)	// 画坐标轴
	{
		ix = rect.right - rect.left + nVLength + CSCAN_SCALE_X_LENGTH;
		iy = rect.bottom - rect.top + CSCAN_SCALE_Y_LENGTH;
	}
	else
	{
		ix = rect.right - rect.left + nVLength;
		iy = rect.bottom - rect.top;
	}

	iy1 = rect.bottom - rect.top;
	ix1 = rect.right - rect.left;

	//creates a memory device context (DC) compatible with the screen device(hScrDC) 
	dwBitmapArraySize = ((((ix*bitCount) + 31) & ~31) >> 3) * iy;
	hMemDC = CreateCompatibleDC(hScrDC);

	//initialise the struct BITMAPINFO for the bimap infomation， 
	//in order to use the function CreateDIBSection
	// on wince os, each pixel stored by 24 bits(biBitCount=24) 
	//and no compressing(biCompression=0) 
	//	BITMAPINFO bmInfo; 
	//	ZeroMemory(&bmInfo, sizeof(BITMAPINFO));
	bmInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo->bmiHeader.biWidth = ix;
	bmInfo->bmiHeader.biHeight = iy;
	bmInfo->bmiHeader.biPlanes = 1;
	bmInfo->bmiHeader.biBitCount = bitCount;

	//use the function CreateDIBSection and SelectObject 
	//in order to get the bimap pointer : lpBitmapBits 
	directBmp = CreateDIBSection(hMemDC, bmInfo, 
		DIB_RGB_COLORS, (void **)&lpBitmapBits, NULL, 0);
	HGDIOBJ previousObject = SelectObject(hMemDC, directBmp);

	// copy the screen dc to the memory dc
	if(nPictureMode == 3)	// 原尺寸需要画坐标轴
	{
		BitBlt(hMemDC, CSCAN_SCALE_X_LENGTH, 0, ix1, iy1, hScrDC, ixStart, iyStart, SRCCOPY);
	}
	else
	{
		BitBlt(hMemDC, 0, 0, ix1, iy1, hScrDC, ixStart, iyStart, SRCCOPY);
	}

	CRect rectOther(ix1, 0, ix, iy1);
	CBrush brush(RGB(255,255,255));
	FillRect(hMemDC, &rectOther, brush);

	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
	HPEN hOldPen = (HPEN)SelectObject(hMemDC, hPen);	
	MoveToEx(hMemDC, ix1, 0, NULL);
	LineTo(hMemDC, ix1, iy1);
	SelectObject(hMemDC, hOldPen);
	DeleteObject(hPen);

	CRect rectV(ix1, 0 + 10, ix - 50, iy1 - 10);

	DrawPattleRule( hMemDC, pCScanParam->m_fBoundMin, pCScanParam->m_fBoundMax, rectV);

	if(nColorMode == 1)
	{
		DrawPattle(hMemDC, rectV, pCScanView, pCScanParam);
	}
	else
	{
		DrawPattle(hMemDC,pCScanParam->m_fBeginGrayValue, pCScanParam->m_fEndGrayValue, rectV);
	}

	if(nPictureMode == 3)	// 画坐标轴
	{		
		CRect rectAll(0, 0, ix - nVLength - CSCAN_SCALE_X_LENGTH, iy);	//除了色条区域

		// X坐标区域
		CRect rectScaleX;
		POINT XScaleTop;
		POINT XScaleBottom;
		XScaleTop.x = CSCAN_SCALE_X_LENGTH;
		XScaleTop.y = rectAll.bottom - CSCAN_SCALE_Y_LENGTH;
		XScaleBottom.x = rectAll.right;
		XScaleBottom.y = rectAll.bottom;
		rectScaleX.SetRect(XScaleTop,XScaleBottom);

		// Y坐标区域
		CRect rectScaleY;
		POINT YScaleTop;
		POINT YScaleBottom;
		YScaleTop.x = rectAll.left;
		YScaleTop.y = rectAll.top;
		YScaleBottom.x = CSCAN_SCALE_X_LENGTH - 1;
		YScaleBottom.y = rectAll.bottom - CSCAN_SCALE_Y_LENGTH;
		rectScaleY.SetRect(YScaleTop,YScaleBottom);

		float fXmin = pCScanView->m_dXStart;
		float fXmax = pCScanView->m_dXEnd;
		float fYmax = pCScanView->m_dYEnd;
		float fYmin = pCScanView->m_dYStart;

		CRect rectBottom(0,rectAll.bottom-CSCAN_SCALE_Y_LENGTH,ix,iy);
		HBRUSH hBrushWhite = CreateSolidBrush(RGB(255,255,255));
		FillRect(hMemDC, &rectBottom, hBrushWhite);
		// 画X坐标
		float fStep  = (fXmax - fXmin) / 10;
		//	pCScanView->DrawScale((CDC *)&hMemDC, rectScaleX, fXmin, fXmax, fStep, 3, TRUE);
		GlobalDrawScale(hMemDC, rectScaleX, fXmin, fXmax, fStep, 3, TRUE);

		// 画Y坐标
		fStep  = (fYmax - fYmin) / 10;
		//	pCScanView->DrawScale((CDC *)&hMemDC, rectScaleY, fYmin, fYmax, fStep, 4, TRUE);
		GlobalDrawScale(hMemDC, rectScaleY, fYmin, fYmax, fStep, 4, TRUE);

		CRect rectSpace(rectAll.left,rectAll.bottom-CSCAN_SCALE_Y_LENGTH,CSCAN_SCALE_X_LENGTH,rectAll.bottom);
		HBRUSH hBrushScale = CreateSolidBrush(RGB(0,192,0));
		FillRect(hMemDC, &rectSpace, hBrushScale);
		CString str;
		str.Format(_T("C扫%d"),nObjectNum);
		TextOut(hMemDC, rectSpace.left +5, rectSpace.bottom-20, str, str.GetLength());
		DeleteObject(hBrushWhite);
		DeleteObject(hBrushScale);
	}

	for(POSITION pos = pCScanParam->m_lstSurfaceDetail.GetHeadPosition(); pos; )
	{
		//获取数据点，并自动进行到下一个点
		FlawNode* node = &pCScanParam->m_lstSurfaceDetail.GetNext(pos);
		LOGICAL_POINT_t stLogicalPoint;
		stLogicalPoint.x = (node->m_fMinX + node->m_fMaxX) / 2;
		stLogicalPoint.y = (node->m_fMinY + node->m_fMaxY) / 2;
		
		CString str;
		str.Format(_T("%.1f,%.1f"),stLogicalPoint.x, stLogicalPoint.y);

		CPoint point = pCScanView->Logical2Client(stLogicalPoint);
		TextOut(hMemDC, stLogicalPoint.x, stLogicalPoint.y, str, str.GetLength());
	}

	SYSTEMTIME sysTime;
	GetSystemTime(&sysTime);
	CString strTime;
	strTime.Format(_T("%d-%2d-%2d %2d：%2d：%2d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
		sysTime.wHour+8, sysTime.wMinute, sysTime.wSecond);
	TextOut(hMemDC, ix1 - 160, iy1 - 50, strTime, strTime.GetLength());
	//if you only want to get the every pixel color value, 
	//you can begin here and the following part of this function will be unuseful;
	//the following part is in order to write file;

	//bimap file header in order to write bmp file 
	//	BITMAPFILEHEADER bmFileHeader;
	//	ZeroMemory(&bmFileHeader, sizeof(BITMAPFILEHEADER));
	bmFileHeader->bfType = 0x4d42; //bmp 
	bmFileHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmFileHeader->bfSize = bmFileHeader->bfOffBits + dwBitmapArraySize;

	//删除
	// Song Chenguang modified start on 2012-11-03
	SelectObject(hMemDC, previousObject);
//	DeleteObject(directBmp);
	DeleteDC(hMemDC);
	// Song Chenguang modified end on 2012-11-03

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("SaveCscanWithInfo"),"Leave");

	return dwBitmapArraySize;
}

void GlobalDrawScale(HDC hDC, 
					 const CRect& rect, 
					 float fStart, 
					 float fEnd, 
					 float fStep, 
					 int direction, 
					 BOOL bReverse)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("GlobalDrawScale"),"Enter");

	ASSERT(fabs(fStart - fEnd) > 1E-6);

	if (fStep<1E-6)
	{
		fStep = 100.0;
	}
	SetBkMode(hDC, TRANSPARENT);

	HBRUSH hBrush = CreateSolidBrush(RGB(0,192,0));
	// 填充矩形
//	Rectangle(hDC, rect.left, rect.right, rect.top, rect.bottom);
	FillRect(hDC, &rect, hBrush);
	// 画分隔线
	int n = 10;//(fEnd - fStart)/fStep;
	float fPerCoordinate = 0.0f;
	CString str;

	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
	HPEN hOldPen = (HPEN)SelectObject(hDC, hPen);
	UINT nOldAlign = 0;

	if (direction == 1)
	{
		nOldAlign = SetTextAlign(hDC, TA_CENTER|TA_BOTTOM);
	}
	else if (direction == 2)
	{
		nOldAlign = SetTextAlign(hDC, TA_LEFT);
	}
	else if (direction == 3)
	{
		nOldAlign = SetTextAlign(hDC, TA_CENTER|TA_TOP);
	}
	else if (direction == 4)
	{
		nOldAlign = SetTextAlign(hDC, TA_RIGHT);
	}
	else
	{
	}

	// 左右标尺
	if (direction == 1 || direction == 3)
	{
		fPerCoordinate = float(rect.Width())/float(fEnd - fStart);
		for (int i=1; i<n; i++)
		{
			str.Format(_T("%d"), (int)(fStart+(fStep*i)));
			// 定义起点，终点
			CPoint ptStart, ptEnd;
			ptStart.x = rect.left + fPerCoordinate*fStep*i;
			ptEnd.x = ptStart.x;
			if (direction == 1)
			{
				ptStart.y = rect.bottom;
				ptEnd.y = rect.bottom - 9;

				TextOut(hDC, ptEnd.x, ptEnd.y, str, str.GetLength());
			}
			else
			{
				ptStart.y = rect.top;
				ptEnd.y = rect.top + 9;

				TextOut(hDC, ptEnd.x+20, ptEnd.y, str, str.GetLength());//ptEnd.x加上20，是为了使刻度靠右偏移，使第一个刻度值能够显示出来。

			}

			MoveToEx(hDC, ptStart.x, ptStart.y, NULL);
			LineTo(hDC, ptEnd.x,ptEnd.y);
		}

		for(int i = 0; i < n; i++)
		{
			CPoint ptStart, ptEnd;
			for(int j = 0; j < 5; j++)
			{
				ptStart.x = rect.left + fPerCoordinate*fStep*i + fPerCoordinate*fStep * j / 5;
				ptEnd.x = ptStart.x;

				if (direction == 1)
				{
					ptStart.y = rect.bottom;
					ptEnd.y = rect.bottom - 4;
				}
				else
				{
					ptStart.y = rect.top;
					ptEnd.y = rect.top + 4;
				}

				MoveToEx(hDC, ptStart.x, ptStart.y, NULL);
				LineTo(hDC, ptEnd.x,ptEnd.y);
			}
		}
	}
	// 上下标尺
	else if (direction == 2 || direction == 4)
	{
		TEXTMETRIC tm;
		GetTextMetrics(hDC, &tm);

		LONG lHalfHeight = tm.tmHeight/2;
		fPerCoordinate = float(rect.Height())/float(fEnd - fStart);
		for (int i=1; i<n; i++)
		{
			int nYData = fStart+(fStep*i);
			str.Format(_T("%d"), nYData);
			// 定义起点，终点
			CPoint ptStart, ptEnd;
			if (!bReverse)
			{
				ptStart.y = rect.top + fPerCoordinate*fStep*i;
			}
			else
				ptStart.y = rect.bottom - fPerCoordinate*fStep*i;

			ptEnd.y = ptStart.y;
			if (direction == 2)
			{
				ptStart.x = rect.left;
				ptEnd.x = rect.left + 8;

				TextOut(hDC, ptEnd.x, ptEnd.y - lHalfHeight, str, str.GetLength());
			}
			else
			{
				ptStart.x = rect.right;
				ptEnd.x = rect.right - 8;

				TextOut(hDC, ptEnd.x, ptEnd.y - lHalfHeight, str, str.GetLength());
			}

			MoveToEx(hDC, ptStart.x, ptStart.y, NULL);
			LineTo(hDC, ptEnd.x,ptEnd.y);
		}

		for(int i = 0; i < n; i++)
		{
			CPoint ptStart, ptEnd;
			for(int j = 0; j < 5; j++)
			{
				if (!bReverse)
				{
					ptStart.y = rect.top + fPerCoordinate*fStep*i + fPerCoordinate*fStep * j / 5;
				}
				else
					ptStart.y = rect.bottom - fPerCoordinate*fStep*i - fPerCoordinate*fStep * j / 5;
				ptEnd.y = ptStart.y;

				if (direction == 2)
				{
					ptStart.x = rect.left;
					ptEnd.x = rect.left + 5;
				}
				else
				{
					ptStart.x = rect.right;
					ptEnd.x = rect.right - 5;
				}

				MoveToEx(hDC, ptStart.x, ptStart.y, NULL);
				LineTo(hDC, ptEnd.x,ptEnd.y);
			}
		}
	}
	else
	{
	}

	SetTextAlign(hDC, nOldAlign);
	SelectObject(hDC, hOldPen);
	DeleteObject(hBrush);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("GlobalDrawScale"),"Leave");
}
//画色条
void DrawPattle( HDC hDC, CRect& DrawRect, CCScanView* pCScanView, CCScanParamPanel* pCScanParam)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("DrawPattle"),"Enter");

	// Song Chenguang add start on 2012-11-03
	if (pCScanView == NULL)
	{
		AfxMessageBox(_T("DrawPattle pCScanView参数为空！"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("DrawPattle"),"Leave");

		return;
	}

	if (pCScanParam == NULL)
	{
		AfxMessageBox(_T("DrawPattle pCScanParam参数为空！"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("DrawPattle"),"Leave");

		return;
	}
	// Song Chenguang add end on 2012-11-03
	CRect rectV1 = DrawRect;
	int DrawRectWidth=rectV1.Width();
	int DrawRectHeight=rectV1.Height();
	if(pCScanParam->m_nColorDefine == 0)	// 自定义颜色
	{
		//Machao modify start on 2013-07-4
// 		float fPerHeight = DrawRectHeight/193.0;//193等分渐变条，每份的高度
// 		COLORREF clrPlatte[193];
// 		for(int i = 0; i < 193; i++)
// 		{
// 			clrPlatte[i] = pCScanView->m_clrDefine[i];
// 		}
// 		for(int i = 193; i > 1; --i ) 
// 		{
// 			//创建刷子
// 			CBrush brush( clrPlatte[193-i] );
// 			FillRect(hDC, &rectV1, brush);
// 
// 			//改变小正方体的位置
// 			//	rectV1.top -= fPerHeight;
// 			rectV1.bottom -= fPerHeight;
// 
// 			//判断小正方体是否超界
// 			if( rectV1.bottom < DrawRect.top )
// 				rectV1.bottom = DrawRect.top;
// 		}
		float fNum = MAXFVALUE;
		rectV1.right -= 4;
		rectV1.bottom -= 5;
		for (int i = 0; i < pCScanParam->m_nSize; i++)
		{
			int nTop = (int)((fNum - pCScanParam->m_plaPalette[i].fMaxPercent) * (float)(rectV1.bottom - rectV1.top) / fNum);
			int nBottom = (int)((fNum - pCScanParam->m_plaPalette[i].fMinPercent) * (float)(rectV1.bottom - rectV1.top) / fNum);
			CBrush brush(pCScanParam->m_plaPalette[i].color);
			FillRect(hDC,&CRect(rectV1.left,rectV1.top + nTop,rectV1.right, rectV1.top + nBottom),brush);
		}
		CBrush brush1(pCScanParam->m_clrBack1);
		FillRect(hDC,&CRect(rectV1.right,rectV1.top,rectV1.right + 4,rectV1.bottom + 5),brush1);
		CBrush brush2(pCScanParam->m_clrBack2);
		FillRect(hDC,&CRect(rectV1.left,rectV1.bottom,rectV1.right,rectV1.bottom + 5),brush2);
		//Machao modify end on 2013-07-4
	}
	else
	{
		float fPerHeight = DrawRectHeight/20.0;//20等分渐变条，每份的高度
		COLORREF clrPlatte[21];
		for(int i = 0; i < 21; i++)
		{
			clrPlatte[i] = pCScanView->m_clrPlatte[i];
		}
		for(int i = 21; i > 1; --i ) 
		{
			//创建刷子
			CBrush brush( clrPlatte[21-i] );
			FillRect(hDC, &rectV1, brush);


			//改变小正方体的位置
			//	rectV1.top -= fPerHeight;
			rectV1.bottom -= fPerHeight;

			//判断小正方体是否超界
			if( rectV1.bottom < DrawRect.top )
				rectV1.bottom = DrawRect.top;
		}		
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("DrawPattle"),"Leave");
}

void DrawPattle( HDC hDC, float fGrayBegin, float fGrayEnd, CRect& DrawRect)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("DrawPattle2"),"Enter");

	int DrawRectWidth=DrawRect.Width();
	int DrawRectHeight=DrawRect.Height();

	float fPerHeight = DrawRectHeight/20.0;//20等分渐变条，每份的高度

	CRect rectV1 = DrawRect;


	float fDValue = (fGrayEnd - fGrayBegin)/20.0;

	//HBRUSH hbr;
	for(int i = 20; i >0; --i ) 
	{
		//创建刷子
		CBrush brush( RGB( fGrayBegin,fGrayBegin,fGrayBegin));
		FillRect(hDC, &rectV1, brush);

		//改变小正方体的位置
		//		rectV1.top -= fPerHeight;
		rectV1.bottom -= fPerHeight;

		//判断小正方体是否超界
		if( rectV1.bottom < DrawRect.top )
			rectV1.bottom = DrawRect.top;

		fGrayBegin += fDValue;

	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("DrawPattle2"),"Leave");
}

void DrawPattleRule( HDC hDC, float fStart, float fEnd, CRect& rectV)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("DrawPattleRule"),"Enter");

	CString str;
	CPoint StartPoint,EndPoint;
	int n = 10;
	float fStep = (fEnd - fStart)/n;
	TEXTMETRIC tm;
	GetTextMetrics(hDC, &tm);
	LONG lHalfHeight = tm.tmHeight/2;
	float fPerCoordinate = float(rectV.Height())/float(fEnd - fStart);
	StartPoint.y = EndPoint.y;

	for (int i = 0;i<= n;i++)
	{
		float fYData = fStart + (fStep*i);
		str.Format(_T("%.0f"),fYData);
		StartPoint.x = rectV.right;
		EndPoint.x = rectV.right + 8;

		//	SetBkMode(hMemDC, TRANSPARENT);
		TextOut(hDC, EndPoint.x+8,rectV.bottom - fPerCoordinate*fStep*i -lHalfHeight,str, str.GetLength());

		MoveToEx(hDC, rectV.right,rectV.bottom - fPerCoordinate*fStep*i, NULL);
		LineTo(hDC, rectV.right +8 ,rectV.bottom - fPerCoordinate*fStep*i);
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("DrawPattleRule"),"Leave");
}
/*
void dBProcess(const float& db, 
			   WORD& db_0_data,
			   WORD& db_1_data,
			   WORD& db_2_data,
			   WORD& db_3_data)
{
	if(db < 0)
		return;

	float db_in = db*YIQI_ADJUST - 10.0;
	float db_1 = 0.0f;
	float db_2 = 0.0f;
	float db_3 = 0.0f;

	if(db_in < -10)
	{	
		db_1 = -10;
		db_2 = 1;
		db_3 = 1;
	}
	else if(db_in < 3)
	{	
		db_1 = db_in-2;
		db_2 = 1;
		db_3 = 1;
	}
	else if(db_in < 107)
	{
		db_1 = db_in/3.0;
		db_2 = db_in/3.0; 
		db_3 = db_in/3.0; 
	}
	else
	{	
		db_1 = 30;
		db_2 = 39;
		db_3 = 39;
	}

	db_0_data = 128;
	db_1_data = WORD((db_1 + 14.0) * 16.0 / 3.0);
	db_2_data = WORD(20.0 * (db_2 - 0.8) / 3.0);
	db_3_data = WORD(20.0 * (db_3 - 0.8) / 3.0);

}
*/
float CurveDataChangeByGain(float fDataToChange, float fChangeGain)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CurveDataChangeByGain"),"Enter");
	float fDataAfterChange = 0.0f;
	fDataAfterChange = (float)(fDataToChange / pow(10, (double)fChangeGain / (-20)));

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CurveDataChangeByGain"),"Leave");

	return fDataAfterChange;
}
float CurveRangeChangeByVelocity(float fRangeToChange, float fCurveVelocity, float fCurrentVelocity)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CurveRangeChangeByVelocity"),"Enter");
	float fDataAfterChange = 0.0f;
	fDataAfterChange = fRangeToChange * (fCurrentVelocity / fCurveVelocity);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CurveRangeChangeByVelocity"),"Leave");

	return fDataAfterChange;
}

void SortCurvePointByRange(CURVEPOINT_DATA_t stCurveToSort[], int nPointNum)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("SortCurvePointByRange"),"Enter");

	// Song Chenguang add start on 2012-11-03
	if (stCurveToSort == NULL || ::IsBadWritePtr(stCurveToSort, sizeof(CURVEPOINT_DATA_t) * nPointNum))
	{
		AfxMessageBox(_T("SortCurvePointByRange stCurveToSort参数错误！"));

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("SortCurvePointByRange"),"Leave");

		return;
	}
	// Song Chenguang add end on 2012-11-03
	CURVEPOINT_DATA_t stTempData;
	memset(&stTempData, 0, sizeof(CURVEPOINT_DATA_t));
	if(nPointNum >= 2)
	{
		for(WORD i = 0; i < nPointNum - 1; i++)
		{
			for(WORD j = i + 1; j < nPointNum; j++)
			{
				if(stCurveToSort[i].fRange > stCurveToSort[j].fRange)
				{
					stTempData = stCurveToSort[j];
					stCurveToSort[j] = stCurveToSort[i];
					stCurveToSort[i] = stTempData;
				}
			}
		}
	}

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("SortCurvePointByRange"),"Leave");

}
float GetTotalPlus(float fPlus, float fRefPlus, int nChnNo)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("GetTotalPlus"),"Enter");

	nChnNo = nChnNo % 2;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCScanView* pCScanView = (CCScanView*)pFrame->m_wndSplitter.GetPane(2,0);

	float fTotalPlus = 0;
	if(pCScanView->m_bGuanLianFlag && nChnNo == 0)	//关联 奇通道
	{
		fRefPlus = 0;
	}

	fTotalPlus = fPlus + fRefPlus;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("GetTotalPlus"),"Leave");

	return fTotalPlus;
}

// wangbingfu add start at 2012-11-19
float Range2MicroSecond(float fVSound, float fRange)
{
	float temp;
	temp = 2 * fRange * 1000000 / (fVSound * 1000);
	return temp;	
}
// wangbingfu add end at 2012-11-19

// wangbingfu add start on 2013-03-19
// 拉格朗日插值中一个函数
float Lagrange_lk(int k,float x, int nX[], int nCount)
{
	float l = 1;
	for(int i = 0; i< nCount; i++)
	{
		if(i != k)
		{
			l *= (x - nX[i]) / (nX[k] - nX[i]);

		}
	}
	return l;
}

//拉格朗日插值最终的函数
float Lagrange(float x, int nX[], int nY[], int nCount)
{
	float y = 0;
	for(int i = 0;i < nCount; i++)
	{
		y += Lagrange_lk(i, x, nX, nCount) * nY[i];
	}
	return y;
}
// wangbingfu add end on 2013-03-19

// Song Chenguang add start on 2013-05-29
void ErroWindowMessage()
{
	DWORD dwError = GetLastError();
	HLOCAL hLocal = NULL;

	BOOL fOK = ::FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, dwError,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPTSTR)&hLocal,0,NULL);
	if (!fOK)
	{
		// Is it a network related error?
		HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"),NULL,
			DONT_RESOLVE_DLL_REFERENCES);

		if (hDll != NULL)
		{
			::FormatMessage(
				FORMAT_MESSAGE_FROM_HMODULE|FORMAT_MESSAGE_FROM_SYSTEM,
				hDll,dwError,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
				(LPTSTR)&hLocal,0,NULL);
			::FreeLibrary(hDll);
		}
	}

	if (hLocal != NULL)
	{
		AfxMessageBox((LPTSTR)hLocal);
	}
	else
		AfxMessageBox(_T("未找到对应的错误序号!"));
}
// Song Chenguang add end on 2013-05-29
// machao add start on 2013-08-13
float Percent100ToDB(float fDBZeroAmp, float fValue)
{
	float fdB = 0 - 20 * log10(fDBZeroAmp / fValue);
	return fdB;
}

float DBTo100Percent(float fDBZeroAmp, float fdB)
{
	float fValue = fDBZeroAmp / powf(10, (0 - fdB) / 20);
	return fValue;
}
// machao add end on 2013-08-13