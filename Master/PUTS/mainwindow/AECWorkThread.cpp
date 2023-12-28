/**
* Copyright(c) 2013 Allrising (Beijing) TechnoLogy Co.,Ltd, All Rights Reserved. 
*
* 线程类
* Filename: AECWorkThread.cpp
*
* @author        Dong Qi
* @version       1.00         Date: 2013-06-06
*/

#include <QMutex>
#include <Windows.h>
#include <QMessageBox>
#include "GlobalFunction.h"
#include "DataDefine.h"
#include "mainwindow.h"
#include "PLXCommunication.h"
#include "EventDefine.h"
#include "AECWorkThread.h"

QVector<CPicData> g_Cpicdata;
DWORD g_AData[MONITOR_DATA_SIZE/AD_SIZE];

extern QMutex g_csCard;
extern short g_nWaveSpeed;
extern DWORD g_dwActiveChannel;
extern CIniParams g_IniParams;
extern map<WORD,CPicData*> m_MapCPicData;
const float INITIAL_RANGE = 38.7f;	   // Song Chenguang add on 2013-11-22
extern HANDLE g_hWaveReadyEvent;	   // Song Chenguang add on 2013-12-03
extern AScan *g_pAscan;                //A扫界面指针
extern int g_ADataAvgNum;

AECWorkThread::AECWorkThread(void)
{
	m_pResult = 0;
	m_pMainWindow = NULL;
	for (int i = 0; i < MONITOR_DATA_SIZE/AD_SIZE; ++i)
	{
		g_AData[i] = 0;
	}
	
}

AECWorkThread::~AECWorkThread(void)
{

}

void AECWorkThread::run()
{ 
	// 传递参数
	int iCardIndex = m_pAecThreadParam.index;
	BOOL  bThreadExitFlag = FALSE;
	// 命令调用
	while(1)
	{
		// 加锁
		if (g_csCard.tryLock())		
		{
			switch (m_pAecThreadParam.nMain2AECThreadCommand)
			{
			case MAIN2AECTHREADCOMMAND_SEL_CHANNEL:
			{
				AEC_selChannel(&m_pAecThreadParam);
				m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
				break;
			}
			case MAIN2AECTHREADCOMMAND_INIT:	//卡初始化
				{
					BOOL bRet = AECCardInit(&m_pAecThreadParam);
					if (bRet)
					{
						m_pAecThreadParam.nAEC2MainThreadFlag = AECTHREADREURN_INIT_OK;       //没做处理
						m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_PLATE_START;
					}
					else
					{
						m_pAecThreadParam.nAEC2MainThreadFlag = AECTHREADREURN_INIT_FALSE;    //没做处理
					}
					break;
				}
			case MAIN2AECTHREADCOMMAND_PLATE_START:	// 单张钢板检测开始
				{	
					//dong qi add start on 2013.11.21
					// 避免重复开始
					if (m_pAecThreadParam.bStarted == TRUE)
					{
						m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
						m_pAecThreadParam.nAEC2MainThreadFlag = AECTHREADREURN_START_OK;
					}
					else
					{
						BOOL nRet = AEC_plate_Start(&m_pAecThreadParam);
						if(nRet)
						{
							m_pAecThreadParam.bStarted = TRUE; 
							m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;;
							m_pAecThreadParam.nAEC2MainThreadFlag = AECTHREADREURN_START_OK;
						}
						else
						{
							m_pAecThreadParam.nAEC2MainThreadFlag = AECTHREADREURN_START_FALSE;
						}
					}
					break;
				   //dong qi add end on 2013.11.21
				}
			case MAIN2AECTHREADCOMMAND_PLATE_STOP:	// 单张钢板检测结束
				{
					BOOL bRet = AEC_plate_Stop(&m_pAecThreadParam);
					if(bRet)
					{
						m_pAecThreadParam.nAEC2MainThreadFlag = AECTHREADREURN_STOP_OK; 
						m_pAecThreadParam.bStarted = FALSE; 
						m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
					}
					else
					{
						m_pAecThreadParam.nAEC2MainThreadFlag = AECTHREADREURN_STOP_FALSE; 
					}
					break;
				}
			case MAIN2AECTHREADCOMMAND_SETONE:	// SetOne
				{		
					BOOL bRet = AEC_palte_setone(&m_pAecThreadParam);
					if (bRet)
					{
						m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
						m_pAecThreadParam.nAEC2MainThreadFlag = AECTHREADREURN_SETONE_OK;
					}
					else
						m_pAecThreadParam.nAEC2MainThreadFlag = AECTHREADREURN_SETONE_FALSE;

					break;
				}
			case MAIN2AECTHREADCOMMAND_SET_CHANNEL_SETONE: // wangwei 2016-1-1
				{
					AEC_selChannel(&m_pAecThreadParam);
					m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_SETONE;
					break;
				}
			case MAIN2AECTHREADCOMMAND_BIAODING:	// 标定
				{

					m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
					break;
				}
			case MAIN2AECTHREADCOMMAND_ZIJIAN:	// 卡自检
				{
					m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
					break;
				}
			case   MAIN2AECTHREADCOMMAND_IDLE: // 卡运行时的一般操作
				{
					AECThread_Idle(&m_pAecThreadParam);
					m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_IDLE;
					break;
				}
			case   MAIN2AECTHREADCOMMAND_THREAD_STOP: // 停止线程
				{				
					bThreadExitFlag = TRUE;
					if (m_pAecThreadParam.bStarted)
					{
						WORD wValue = 1;
						DWORD begin = GetTickCount();
						while(1)
						{
							wValue = P9054_ReadWord(m_pAecThreadParam.hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
							wValue = wValue >> 1;
							if ((wValue & 0x1 )== 0)
							{
								break;
							}
							DWORD cha = GetTickCount() - begin;
							if (cha > WAIT_TIME_SET)
							{
								QString str = QString("%1%2%3").arg(QObject::tr("第 ")).arg(m_pAecThreadParam.index+1).arg(QObject::tr(" 块卡线程结束时查询标志位失败，请检查卡是否正常或重新启动机器。"));
								QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
								m_pMainWindow->m_PLXCommunication.UnInit(m_pMainWindow->m_stThreadParam);
								ExitProcess(0);								
							}
						}

						wValue = 1; 
						P9054_WriteWord(m_pAecThreadParam.hDev, P9054_ADDR_SPACE0, PCI_CLEAR_COMMUNICATE_FLAG_ADDR, wValue);
						wValue = 0; 	
						P9054_WriteWord(m_pAecThreadParam.hDev, P9054_ADDR_SPACE0, PCI_CLEAR_COMMUNICATE_FLAG_ADDR, wValue);
						m_pAecThreadParam.pPLXCommunication->ResetHPI_INT(m_pAecThreadParam.hDev);//清除中断					
						// 给PLC写结束标志
						wValue= SIM_END_VALUE;
						P9054_WriteWord(m_pAecThreadParam.hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_CMD_NUM_ADDR, wValue);
						wValue = P9054_ReadWord(m_pAecThreadParam.hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
						wValue = (wValue >> 1) << 1; 	
						P9054_WriteWord(m_pAecThreadParam.hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);
						wValue += 1; 	
						P9054_WriteWord(m_pAecThreadParam.hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);
						// 查询PCI到DSP反馈信号
						begin = GetTickCount();
						while(1)
						{
							wValue = P9054_ReadWord(m_pAecThreadParam.hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
							wValue = wValue >> 1;
							if ((wValue & 0x1 )== 0)
							{
								break;
							}
							DWORD cha = GetTickCount() - begin;
							if (cha > WAIT_TIME_SET)

							{
								QString str = QString("%1%2%3").arg(QObject::tr("第 ")).arg(m_pAecThreadParam.index+1).arg(QObject::tr(" 块卡线程结束时查询反馈标志位失败，请检查卡是否正常或重新启动机器。"));

								QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
								m_pMainWindow->m_PLXCommunication.UnInit(m_pMainWindow->m_stThreadParam);
								ExitProcess(0);

							}
						}
						// 清除PCI到DSP通讯命令
						wValue = 0;
						P9054_WriteWord(m_pAecThreadParam.hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_CMD_NUM_ADDR, wValue);
						// 清除PCI到DSP信号标志位
						wValue = P9054_ReadWord(m_pAecThreadParam.hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
						wValue = (wValue >> 1) << 1;
						P9054_WriteWord(m_pAecThreadParam.hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);
						m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
					}
					break;
				}
			default:
				{
					m_pAecThreadParam.nMain2AECThreadCommand = MAIN2AECTHREADCOMMAND_NONE;
					break;
				}
			}
			// 解锁
			g_csCard.unlock();
			Sleep(5);
		}  
		else 
		{ 			
			Sleep(30);// 需要调整 
		}
		if(bThreadExitFlag)
		{
			m_pAecThreadParam.nAEC2MainThreadFlag = AECTHREADREURN_THREAD_STOP_OK;
			break; // stop the 
		}
	} 
	m_pResult = 0;   
	return;

}

BOOL AECWorkThread::AECCardInit(AECTHREAD_PARAM * pThreadParam)
{
	WORD wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, DSP_2_PCI_SRAM_CONTROL1_ADDR, wValue);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, DSP_2_PCI_SRAM_CONTROL2_ADDR, wValue);
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_SRAM_CONTROL_ADDR, wValue);
	wValue = 1;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_CLEAR_COMMUNICATE_FLAG_ADDR, wValue);
	wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_CLEAR_COMMUNICATE_FLAG_ADDR, wValue);
	// 清除中断
	pThreadParam->pPLXCommunication->ResetHPI_INT(pThreadParam->hDev);
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
	return TRUE;
}
BOOL AECWorkThread::AEC_plate_Start(AECTHREAD_PARAM * pThreadParam)
{
	try
	{
		/*通知dsp*/
		// 给卡写开始标志	
		// 申请SRAM控制权
		WORD wValue = 1;
		WORD ntimer = 0;
		// 启动硬件 	
		wValue = SIM_END_NO_EFFECT;
		P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_CMD_NUM_ADDR, wValue);
		// 初始化监视通道信息
		//chenyuzhu modify on 2014-10-28
		float XRange = (float)g_pAscan->spinBoxXRange->value();
		float fRangeCha = XRange - Dot2Range(VSOUND, MONITOR_SIZE);//INITIAL_RANGE 这个设置成了一个固定值不妥 chenyuzhu  on 2014-10-27
		//chenyuzhu modify on 2014-10-28
		WORD nInterSpace[FLAW_DOT_MAX_NO];
		RangeProcess(g_nWaveSpeed, fRangeCha, nInterSpace);
		pThreadParam->pPLXCommunication->SetMonitorRange(pThreadParam->hDev, nInterSpace);	//chenyuzhu 注释 on 2014-10-27
		// 设置程序开始了
		//pThreadParam->nAEC2MainThreadFlag = AECTHREADREURN_START_OK;
		return TRUE;
	}
	catch(QString exception)
	{
		QMessageBox::about(NULL,"钢板检测开始异常",exception);
		return FALSE;
	}
}
int AECWorkThread::AECThread_Idle(AECTHREAD_PARAM * pThreadParam)
{
	int iCardIndex = pThreadParam->index;
	//nIndex 用于计算是哪个卡
	int nIndex = g_dwActiveChannel / MAX_CARD_CHANNEL_NO;
	DWORD dwWaitObjectIndex = WaitForSingleObject(g_hWaveReadyEvent, 0);
	if (dwWaitObjectIndex != WAIT_OBJECT_0)
	{
		if (nIndex == iCardIndex)
		{		
		//	WORD wTempChannelNum = pThreadParam->wSelChannel;
			WORD wTempChannelNum = g_dwActiveChannel;
			map<WORD,CPicData*>::iterator iter = m_MapCPicData.find(wTempChannelNum);
			if(iter != m_MapCPicData.end())
			{
				//向map里添加数据
				CPicData * pData = iter->second;
				if (pData == NULL)
				{
					return 0;
				}
				if (pData->m_wData ==NULL)
				{
					return 0;
				}
				BOOL bRet = pThreadParam->pPLXCommunication->PutsReadDrawData(pThreadParam->hDev, pData);
				
				for (int k = 0; k < MONITOR_SIZE; ++k)
				{
					g_AData[k] = g_AData[k] + pData->m_wData[k];
				}
				
				g_Cpicdata.append(*pData);
				if (g_Cpicdata.size() <= 0)
				{
					return 0;
				}
				if (g_Cpicdata.size() < g_ADataAvgNum)
				{
					for (int i = 0; i < MONITOR_SIZE; ++i)
					{
						(iter->second)->m_wData[i] = g_AData[i] / g_Cpicdata.size();
					}
				}
				else if (g_Cpicdata.size() > g_ADataAvgNum)
				{
					for (int k = 0; k < MONITOR_SIZE; ++k)
					{
						g_AData[k] = g_AData[k]  - g_Cpicdata.value(0).m_wData[k];
					}
					g_Cpicdata.remove(0);
				}
				if (g_Cpicdata.size() == g_ADataAvgNum)
				{
					//int start = GetTickCount();
					for (int i = 0; i < MONITOR_SIZE; ++i)
					{
						(iter->second)->m_wData[i] = g_AData[i] / g_Cpicdata.size();
					}
					//int t = GetTickCount() - start;
					//qDebug() << "time:" << t;
				}
				else
				{
					return 0;		
				}
				//通知画图线程可以画图
			
				SetEvent(g_hWaveReadyEvent); 
			}		
		}
	}
	// 读取伤数据
	BOOL bRet = ReadFlawResult(pThreadParam, iCardIndex);
	return 0;
} 

BOOL AECWorkThread::AEC_plate_Stop(AECTHREAD_PARAM * pThreadParam)
{
	if (!pThreadParam) {
		QMessageBox::information(NULL, "Error", "AECWorkThread::AEC_plate_Stop(): pThreadParam is null", QMessageBox::Yes);
		return FALSE;
	}

	int iCardIndex = pThreadParam->index;
	BOOL nErrorFlag_sram_controller = FALSE; 
	BOOL nErrorFlag_card_process = FALSE; 
	int  nError =0;

	//chenyuzhu add start on 2014-10-30 
	// wangwei add on 20151120
	map<WORD,CPicData*>::iterator iter;
	if (!m_MapCPicData.empty()) {
		iter = m_MapCPicData.begin();
		::ZeroMemory((iter->second)->m_wData, sizeof((iter->second)->m_wData));
	}
	
	//chenyuzhu add end on 2014-10-30 
	// 等待卡结束处理
	BOOL bEnd = FALSE;
	int  nEndFlag = FALSE;
	nErrorFlag_sram_controller = FALSE;
	//申请sram控制权
	WORD wValue = 1;
	int timer=0;
	DWORD begin = GetTickCount();

	while(1)
	{
		wValue = 1;
		wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
		wValue = wValue >> 1;
		if ((wValue & 0x1 )== 0)
		{
			break;
		}	
		DWORD cha = GetTickCount() - begin;
		if (cha > WAIT_TIME_SET)
		{
			QString str = QString("%1%2%3").arg(QObject::tr("第 ")).arg(pThreadParam->index+1).arg(QObject::tr(" 块卡钢板结束时查询标志位失败，请检查卡是否正常或重新启动机器。"));
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
			m_pMainWindow->m_PLXCommunication.UnInit(m_pMainWindow->m_stThreadParam);
			ExitProcess(0);
		}
	}

	if (!pThreadParam->pPLXCommunication) {
		QMessageBox::information(NULL, "Error", "AECWorkThread::AEC_plate_Stop(): pThreadParam->pPLXCommunication is null", QMessageBox::Yes);
		return FALSE;
	}

	// 获取了控制权
	pThreadParam->pPLXCommunication->ResetHPI_INT(pThreadParam->hDev);
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
	// 查询PCI到DSP反馈信号
	begin = GetTickCount();
	int errCount = 0;
	int endCount = 1;
	while(1)
	{
		wValue = 1;
		wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
		wValue = wValue >> 1;

		if ((wValue & 0x1 )== 0)
		{
			break;
		}

		DWORD cha = GetTickCount() - begin;

		if ((errCount >= endCount) && (cha > WAIT_TIME_SET))
		{	// 武钢 崩溃报错 TODO
			QString str = QString("%1%2%3").arg(QObject::tr("第 ")).arg(pThreadParam->index + 1)
				.arg(QObject::tr(" 块卡钢板结束时查询PCI到DSP反馈信号失败，请检查卡是否正常或重新启动机器。 "));
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
			m_pMainWindow->m_PLXCommunication.UnInit(m_pMainWindow->m_stThreadParam);
			ExitProcess(0);
		}

		if (cha > WAIT_TIME_SET)	// 没有在规定时间内响应，试图重新写结束
		{
			// 给PLC写结束标志
			wValue= SIM_END_VALUE;
			P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_CMD_NUM_ADDR, wValue);	
			wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
			wValue = (wValue >> 1) << 1; 	
			P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);	
			wValue += 1; 	
			P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);	

			begin = cha;
			errCount++;
		}
	}
	// 清除PCI到DSP通讯命令
	wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_CMD_NUM_ADDR, wValue);
	// 清除PCI到DSP信号标志位
	wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);
	wValue = (wValue >> 1) << 1;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);


	BOOL bRet = ReadStopFlawResult(pThreadParam, iCardIndex);
	if (!bRet)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL AECWorkThread::AEC_selChannel(AECTHREAD_PARAM * pThreadParam) 
{
	WORD wValue = 1;
	WORD channelNO = pThreadParam->wSelChannel;  //272

	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_RESET_OFFSET, wValue);
	wValue = pThreadParam->pAllConfig[channelNO].CH_ZEROCORRECT;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_ZERO_OFFSET, wValue);

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
		if (cha > WAIT_TIME_SET)
		{
			QString str = QString("%1%2%3").arg(QObject::tr("第 ")).arg(pThreadParam->index+1)
				.arg(QObject::tr(" 块卡切换通道时查询标志位失败，请检查卡是否正常或重新启动机器。"));
			MessageBox(NULL,L"卡切换通道时查询标志位失败，请检查卡是否正常或重新启动机器",L"AECWorkThread::AEC_selChannel",MB_OK);

			m_pMainWindow->m_PLXCommunication.UnInit(m_pMainWindow->m_stThreadParam);
			ExitProcess(0);
		}
	}

	// 申请SRAM控制权
	begin = GetTickCount();
	QString strInfo = QString("%1%2%3").arg(QObject::tr("AEC_selChannel函数里执行SelChannel命令, CH_NO = ")).arg(channelNO)
		.arg(QObject::tr("\r\n"));
	// 写入数据
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, CHANNEL_SEL_ADDR, channelNO);
	// 释放SRAM控制权
	wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_SRAM_CONTROL_ADDR, wValue);

	wValue = SIM_SEL_CHANNEL;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_CMD_NUM_ADDR, wValue);

	wValue = P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR);

	wValue = (wValue >> 1) << 1; 	
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_2_DSP_COMMUNICATE_FLAG_ADDR, wValue);
	wValue = wValue + 1; 	
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
		if (cha > WAIT_TIME_SET)
		{
			QString str = QString("%1%2%3").arg(QObject::tr("线程 ")).arg(pThreadParam->index+1).arg(QObject::tr(" 的selChannel查询PCI到DSP反馈信号失败"));
//			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
			MessageBox(NULL,L"selChannel查询PCI到DSP反馈信号失败",L"AECWorkThread::AEC_selChannel",MB_OK);
			m_pMainWindow->m_PLXCommunication.UnInit(m_pMainWindow->m_stThreadParam);
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
	wValue = 1;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_RESET_OFFSET, wValue);
	wValue = 0;
	P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_RESET_OFFSET, wValue);
	pThreadParam->pPLXCommunication->WaveDelay(pThreadParam->hDev, pThreadParam->pAllConfig[g_dwActiveChannel].CH_ZEROCORRECT);
	return TRUE;
}

BOOL AECWorkThread::AEC_palte_setone(AECTHREAD_PARAM * pThreadParam)
{
	P9054_HANDLE hDev = pThreadParam->hDev;
	CH_CONFIG *pCHX_INIT_CONFIG = &pThreadParam->pAllConfig[pThreadParam->nSetOneChannel];
	pCHX_INIT_CONFIG->CH_NO = pThreadParam->nSetOneChannel;
	int chno = pCHX_INIT_CONFIG->CH_NO;
	// 获取控制权
	WORD wValue = 1;
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
		if (cha > WAIT_TIME_SET)
		{
			QString str = QString("%1").arg(QObject::tr("单通道设置查询PCI到DSP标志位失败，请检查卡是否正常或重新启动机器。"));
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
			m_pMainWindow->m_PLXCommunication.UnInit(m_pMainWindow->m_stThreadParam);
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
	wValue = 1;
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
		if (cha > WAIT_TIME_SET)
		{
			QString str = QString("%1").arg(QObject::tr("单通道设置查询PCI到DSP反馈信号失败，请检查卡是否正常或重新启动机器。"));
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
			m_pMainWindow->m_PLXCommunication.UnInit(m_pMainWindow->m_stThreadParam);
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
	return TRUE;
}