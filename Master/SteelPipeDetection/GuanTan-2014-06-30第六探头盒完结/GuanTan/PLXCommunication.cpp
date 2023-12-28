#include <Windows.h>
#include <tchar.h>
#include "..\pci9054_dll\plx_lib_x.h"	// plx_dll.dll PCI卡操作动态链接库
#include "..\pci9054_dll\p9054_lib.h"
#include "const.h"
#include <qobject.h>
#include <QEvent>
#include "DataDefine.h"
#include <QString>
#include "PLXCommunication.h"
#include "GlobalFunction.h"
#include <list>
#include "HardwareAddress.h"
#include <QtGui>
#include <QSettings>
#include <QMessageBox>


using namespace std;

//extern CDatabase g_sqlDB;
const DWORD MULTI_CARD_FLAG = 0x0101;
const DWORD AEC_CARD_FLAG = 0x0201;

CPLXCommunication::CPLXCommunication(void)
{
	m_wTotalCardNum = 0;
	m_wMFCCardNum = 0;
	m_wAECCardNum = 0;
}

CPLXCommunication::~CPLXCommunication(void)
{
}

BOOL CPLXCommunication::Init(AECTHREAD_PARAM* pstThreadParam)
{
	DWORD dwRet = PLX_LibInit();
	if (dwRet != WD_STATUS_SUCCESS)
	{
		QMessageBox::information(NULL, "Error", QObject::tr("PLX初始化失败,请检查任务管理器中是否有管探程序打开!"), QMessageBox::Yes, QMessageBox::Yes);
		return FALSE;
	}

	BOOL bRet = BusSlotFindAndSort(pstThreadParam);

	if (!bRet)
	{
		QMessageBox::information(NULL, "Error", QObject::tr("PLX Bus、Slot查询失败!"), QMessageBox::Yes, QMessageBox::Yes);
		return FALSE;
	}


	bRet = AllCardsSelfTest(pstThreadParam);
	if (!bRet)
	{
		// 关闭hDev
		for (int i = 0;i < m_wTotalCardNum;i ++)   // dwCardCount->g_nTotalCardNum,20081202
		{	
			PLX_DeviceClose(pstThreadParam[i].hDev);
		}

		//		WriteLog(_T("NDT-EventLog.txt"), _T("卡自检失败!"),CFile::modeReadWrite|CFile::modeCreate);
		return bRet;
	}
	else
	{
		//		WriteLog(_T("NDT-EventLog.txt"), _T("卡自检成功!"),CFile::modeReadWrite|CFile::modeCreate);
		return TRUE;
	}
}

DWORD CPLXCommunication::UnInit(AECTHREAD_PARAM* pstThreadParam)
{
	for(int i=0; i< m_wTotalCardNum; i++)
	{
		P9054_Close(pstThreadParam[i].hDev);
	}
	
	return PLX_LibUninit();
}

BOOL CPLXCommunication::AllCardsSelfTest(AECTHREAD_PARAM* pstThreadParam)
{
	BOOL bselftest = FALSE;

	for (int i = 1;i <= m_wAECCardNum;i++)  //aec卡自检，从thread_param[1]开始 <dwCardCount ----><=g_nAECCardNum
	{
		bselftest = CardSelfTest(pstThreadParam[i].hDev);
		if(!bselftest)  // self test fail
		{
			QString str = QString("%1%2%3").arg(QObject::tr("第 ")).arg(i).arg(QObject::tr(" 块卡自检失败！！！！！"));
			
			//str.Format(_T("第 %d 块卡自检失败！！！！！"), i);
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
			return bselftest;
		}
		else
		{
			/* board pass the test, set ok*/
	/*
			DWORD dwHPIAddr_L = SYS_STATUS_FLAG_ADDR_L;
			DWORD dwHPIAddr_H = SYS_STATUS_FLAG_ADDR_H;
			WORD  wValueWrite_L = SYS_STATUS_FLAG_DATA_L_OK;
			WORD wValueWrite_H = SYS_STATUS_FLAG_DATA_H_OK;

			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, dwHPIAddr_L);
			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, dwHPIAddr_H);

			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L, wValueWrite_L);
			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H, wValueWrite_H);

			Delay(2);

			// 1
			// DSP
			WORD wHPIAddr_L = DSP_VER_ADDRESS_L;
			WORD wHPIAddr_H = DSP_VER_ADDRESS_H;
			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wHPIAddr_L);
			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wHPIAddr_H);

			WORD wDSPLow = P9054_ReadWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L);
			WORD wDSPHigh = P9054_ReadWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H);

			Delay(2);
			WORD wDSPVer = wDSPHigh;
			CString strDSPVer;
			strDSPVer.Format(_T("AEC卡%d  DSP版本：%d.%d%d \r\n"), i, (wDSPVer / 0x1000 * 10) + (wDSPVer % 0x1000) / 0x100,
				(wDSPVer % 0x100) / 0x10, wDSPVer % 0x10);

			
			// FPGA1
			wHPIAddr_L = FPGA1_VER_ADDRESS_L;
			wHPIAddr_H = FPGA1_VER_ADDRESS_H;
			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wHPIAddr_L);
			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wHPIAddr_H);

			WORD wFPGA1Low = P9054_ReadWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L);
			WORD wFPGA1High = P9054_ReadWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H);

			Delay(2);
			WORD wFPGA1Ver = wFPGA1High;
			CString strFPGA1Ver;
			strFPGA1Ver.Format(_T("AEC卡%d  FPGA1版本：%d.%d%d \r\n"), i, (wFPGA1Ver / 0x1000 * 10) + (wFPGA1Ver % 0x1000) / 0x100,
				(wFPGA1Ver % 0x100) / 0x10, wFPGA1Ver % 0x10);

			
			// FPGA2
			wHPIAddr_L = FPGA2_VER_ADDRESS_L;
			wHPIAddr_H = FPGA2_VER_ADDRESS_H;
			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wHPIAddr_L);
			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wHPIAddr_H);

			WORD wFPGA2Low = P9054_ReadWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L);
			WORD wFPGA2High = P9054_ReadWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H);

			Delay(2);
			WORD wFPGA2Ver = wFPGA2High;
			CString strFPGA2Ver;
			strFPGA2Ver.Format(_T("AEC卡%d  FPGA2版本：%d.%d%d \r\n"), i, (wFPGA2Ver / 0x1000 * 10) + (wFPGA2Ver % 0x1000) / 0x100,
				(wFPGA2Ver % 0x100) / 0x10, wFPGA2Ver % 0x10);
			// FPGA3
			WORD wFPGA3Ver = P9054_ReadWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, FPGA3_VER_ADDRESS);

			CString strFPGA3Ver;
			strFPGA3Ver.Format(_T("AEC卡%d  FPGA3版本：%d.%d%d \r\n"), i, (wFPGA3Ver / 0x1000 * 10) + (wFPGA3Ver % 0x1000) / 0x100,
				(wFPGA3Ver % 0x100) / 0x10, wFPGA3Ver % 0x10);

			m_strVersion = m_strVersion + strDSPVer + strFPGA1Ver + strFPGA2Ver + strFPGA3Ver;
			*/
		}

	}

	bselftest = MultiCardTest(pstThreadParam[0].hDev);   //多功能卡自检，应该用g_nMFCNum;
	// songchenguang add start
	if (!bselftest)
	{
		//AfxMessageBox(_T("多功能卡自检失败，需关机，重新启动机器!"));
		QString str =QString("%1").arg(QObject::tr("多功能卡自检失败，需关机，重新启动机器!"));
		QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
	}
	else
	{
		WORD wMPBVer = P9054_ReadWord(pstThreadParam[0].hDev, P9054_ADDR_SPACE0, 0x00);
		/*
		CString strMPBVer;
		strMPBVer.Format(_T("MPB卡版本：%d.%d%d \r\n"), (wMPBVer / 0x1000 * 10) + (wMPBVer % 0x1000) / 0x100,
			(wMPBVer % 0x100) / 0x10, wMPBVer % 0x10);
		*/

		QString strMPBVer = QString("%1%2%3%4%5").arg(QObject::tr("MPB卡版本：")).arg((wMPBVer / 0x1000 * 10) + (wMPBVer % 0x1000) / 0x100)
			.arg(".").arg((wMPBVer % 0x100) / 0x10).arg(wMPBVer % 0x10).arg("\r\n");
		m_strVersion = m_strVersion + strMPBVer;
		// REM TEMP 2013-05-23
//		bselftest = MultiCardInit(pstThreadParam[0].hDev);
	}

	return bselftest;
}

//////////////////////////////////////////////////////////////////////////
//
// 卡自检 
//
//////////////////////////////////////////////////////////////////////////

BOOL CPLXCommunication::CardSelfTest(P9054_HANDLE hDev)
{
	DWORD dwAddr;
	WORD wHPIAddr_H, wHPIAddr_L;
	WORD wValueRead = 0;
	WORD wValueWrite = SELF_TEST_VALUE;

	// 使工作卡系统指示灯变红
//	AEC_OperationLamp(hDev, AEC_SYS_BOARD_STATUS_ERROR);
	WORD wSelfTestResult = 0;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, AEC_CARD_SELFTEST_RESULT_ADDRESS, wSelfTestResult);

	dwAddr = SYS_SELF_TEST_ADDRESS;
	while (dwAddr < SYS_SELF_TEST_ADDRESS_END)
	{
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, dwAddr, wValueWrite);
		dwAddr += 0x2;
		Delay(2);
	}

	// Song Chenguang add start on 2009-03-25
	int nResetCount = 0;
	DWORD dwRestAddr = 0x6c;

	dwAddr = SYS_SELF_TEST_ADDRESS;
	while (dwAddr < SYS_SELF_TEST_ADDRESS_END)
	{
		wValueRead = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, dwAddr);	
		if (wValueRead != wValueWrite) 
		{  
			// 软复位
			DWORD dwResetValue = P9054_ReadDWord(hDev, P9054_ADDR_REG, dwRestAddr);	

			DWORD temp = 1<<30;
			dwResetValue |= temp;
			P9054_WriteDWord(hDev, P9054_ADDR_REG, dwRestAddr, dwResetValue);
			Sleep(5000);

			temp = 0xbfffffff;
			dwResetValue &= temp;
			P9054_WriteDWord(hDev, P9054_ADDR_REG, dwRestAddr, dwResetValue);

			// 写数据
			dwAddr = SYS_SELF_TEST_ADDRESS;
			while (dwAddr < SYS_SELF_TEST_ADDRESS_END)
			{
				P9054_WriteWord(hDev, P9054_ADDR_SPACE0, dwAddr, wValueWrite);
				dwAddr += 0x2;
				Delay(2);
			}

			dwAddr = SYS_SELF_TEST_ADDRESS;
			nResetCount++;

			continue;
		}
		dwAddr += 0x2;
		Delay(2);

		if (nResetCount == 3)
		{
			//AfxMessageBox(_T("PCI 自检出错！！！！！"));
			QString str =QString("%1").arg(QObject::tr("PCI 自检出错！！！！！"));
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
			return FALSE;
		}
	}
	// Song Chenguang add end on 2009-03-25
/*
	// HPI 初始化
	WORD wValueL = HPI_RC_INIT_VALUE;
	WORD wValueH = HPI_RC_INIT_VALUE;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_L, wValueL);
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_H, wValueH);

	wValueL = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_L);	
	wValueH = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_H);	

	// HPI 方式读写
	wHPIAddr_L = SELF_TEST_HPI_L_BEGIN;
	wHPIAddr_H = SELF_TEST_HPI_H;
	//	while(dwHPIAddr_L < SELF_TEST_HPI_L_END)
	for (int j = 0;j < 32; j ++)
	{
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wHPIAddr_L);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wHPIAddr_H);

		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L, wValueWrite);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H, wValueWrite);

		wHPIAddr_L += 0x4;
		Delay(2);
	}

	wHPIAddr_L = SELF_TEST_HPI_L_BEGIN;
	wHPIAddr_H = SELF_TEST_HPI_H;
	//	while(dwHPIAddr_L < SELF_TEST_HPI_L_END)
	for (int j = 0;j < 32;j++)
	{
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wHPIAddr_L);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wHPIAddr_H);

		wValueRead = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L);	
		if (wValueRead != wValueWrite) 
		{
			wValueRead = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H);	
			AfxMessageBox(_T("HPI 自检出错 1！！！！！"));
			return FALSE;
		}

		wValueRead = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H);
		if (wValueRead != wValueWrite) 
		{
			AfxMessageBox(_T("HPI 自检出错 2！！！！！"));
			return FALSE;
		}

		wHPIAddr_L += 0x4;
		Delay(2);
	}
*/
	// 使工作卡系统指示灯变绿
//	AEC_OperationLamp(hDev, AEC_SYS_BOARD_STATUS_OK);
	wSelfTestResult = 1;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, AEC_CARD_SELFTEST_RESULT_ADDRESS, wSelfTestResult);

	return TRUE;
}

BOOL CPLXCommunication::AEC_OperationLamp(P9054_HANDLE hDev, WORD nValue)
{
/*	WORD dwHPIAddr_L = AEC_SYS_BOARD_STATUS_ADDR_L;
	WORD dwHPIAddr_H = AEC_SYS_BOARD_STATUS_ADDR_H;
	WORD nHighValue = 0x0;

	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, dwHPIAddr_L);
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, dwHPIAddr_H);

	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L, nValue);
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H, nHighValue);
*/
	return TRUE;
}

// void CPLXCommunication::Delay(DWORD dwUs)
// {
// 	DWORD dwUs1 = DELAY_TIME * dwUs;
// 	for (DWORD i = 0;i < dwUs1;i++) 
// 	{
// 	}
// }

////////////////////////////////////////////////////////////////
//
// 多功能卡自检
//
////////////////////////////////////////////////////////////////

BOOL CPLXCommunication::MultiCardTest(P9054_HANDLE hDev)
{

	// 板卡ready 变红
	WORD nValue = MPB_CARD_READY_ERROR;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_CARD_READY_ADDR, nValue);
	Delay(1);

	nValue = MPB_SYS_READY_ERROR;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_SYS_READY_ADDR, nValue);
	Delay(1);
	DWORD dwAddr = MULTI_CARD_SELF_TEST_ADDR;
	WORD wValueWrite = 0xAAAA;
	WORD wValueRead = 0;

	for (int i = 0;i < MULTI_CARD_SELF_TEST_ADDR_SIZE / 2;i++)
	{
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, dwAddr, wValueWrite);
		dwAddr += 2;
		// songchenguang add start
		Delay(1);
		// songchenguang add end
	}

	dwAddr = MULTI_CARD_SELF_TEST_ADDR;
	for (int i = 0;i < MULTI_CARD_SELF_TEST_ADDR_SIZE / 2;i++)
	{
		wValueRead = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, dwAddr);	
		if (wValueRead != wValueWrite) 
			return FALSE;
		dwAddr += 2;
		// songchenguang add start
		Delay(1);
		// songchenguang add end
	}

	// 板卡ready  变绿
	nValue = MPB_CARD_READY_OK;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_CARD_READY_ADDR, nValue);
    Delay(1);

	// 多功能卡ReadyOK标志
	nValue = MPB_SYS_READY_OK;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_SYS_READY_ADDR, nValue);

	return TRUE;
}

// 配件配置发现与排序
/*
BOOL CPLXCommunication::BusSlotFindAndSort(AECTHREAD_PARAM* pstThreadParam)
{
	for (int j = 0; j< MAX_CARD_NO; j++)
	{
		m_BusSlotIndex[j].dwSlot = 0;
	}

	m_wTotalCardNum = FindCardBusSlot(&m_scanResult, m_BusSlotIndex);  // 总卡数

	if (m_wTotalCardNum == 0)
	{
		AfxMessageBox(_T("工作卡数目为0!!!"));

		return FALSE;
	}

	if (m_BusSlotIndex[0].dwSlot == 0) // 如果没有多功能卡,
	{
		m_wMFCCardNum = 0;
		AfxMessageBox(_T("没有多功能能卡!!!"));

		return FALSE;
	}
	else
	{	
		m_wMFCCardNum = 1; // 目前只支持1块多功能卡
		m_wAECCardNum = m_wTotalCardNum -1; 		
	}

	for (int i = 0;i < m_wTotalCardNum; i++)   // dwCardCount->g_nTotalCardNum,20081202
	{
		int j = 0;
		for (j = 0;j < MAX_CARD_NO;j++)
		{
			if ((m_scanResult.deviceSlot[j].dwSlot == m_BusSlotIndex[i].dwSlot) &&
				(m_scanResult.deviceSlot[j].dwBus == m_BusSlotIndex[i].dwBus))
				break;
		}
		PLX_DeviceOpenBySlot(&(pstThreadParam[i].hDev ), &m_scanResult.deviceSlot[j], TRUE);		
	}

	return TRUE;
}
*/
BOOL CPLXCommunication::BusSlotFindAndSort(AECTHREAD_PARAM* pstThreadParam)
{
	// 计算板卡数量
	list<WD_PCI_SLOT> sortlist;
	list<WD_PCI_SLOT>::iterator inter;

	for (int j = 0; j< MAX_CARD_NO; j++)
	{
		m_BusSlotIndex[j].dwSlot = 0;
	}

	m_wTotalCardNum = P9054_CountCards (0x10b5, 0x9054);
	
	if(m_wTotalCardNum == 0) 
	{ 
		//AfxMessageBox(_T("没有发现采集卡和接收卡，请关机后安装板卡! "));
		QString str =QString("%1").arg(QObject::tr("没有发现采集卡和接收卡，请关机后安装板卡! "));
		QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
		return FALSE;
	}	
	else 
	{
		int j = 1;
		for(int i = 0; i < m_wTotalCardNum; i++)  //查找卡
		{
			P9054_HANDLE hPlx = NULL;
			P9054_Open (&hPlx, 0x10b5, 0x9054, i, 0);//P9054_OPEN_USE_INT);

			if(hPlx == NULL) 
			{
				/*
				CString str;
				str.Format(_T("P9054_Open %d 没有发现采集卡和接收卡，请关机后安装板卡! ! "), i);
				*/
				QString str = QString("%1%2%3").arg(QObject::tr("P9054_Open ")).arg(i).arg(QObject::tr(" 没有发现采集卡和接收卡，请关机后安装板卡! !") );

				//AfxMessageBox(str);
				//QString str =QString("%1").arg(QObject::tr("没有发现采集卡和接收卡，请关机后安装板卡! "));
				QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
				return FALSE;
			} 
			else 
			{
				DWORD  dwSeriNumb = P9054_ReadPCIReg( hPlx, 0x2E);
				//char m_SeriNumb=(char)(m_reg && 0xff);
				switch(dwSeriNumb)
				{
				case MULTI_CARD_FLAG:
					pstThreadParam[0].hDev = hPlx;
					m_BusSlotIndex[0] = (*hPlx).pciSlot;
					break;
				case AEC_CARD_FLAG:
					pstThreadParam[j].hDev = hPlx;
					sortlist.push_back((*hPlx).pciSlot);
					j++;
					break;
				default:
					break;
				}

				hPlx = NULL;
			}
		}

		// AEC排序
		int m = 1;
		for (inter = sortlist.begin(); inter != sortlist.end(); inter++)
		{
			m_BusSlotIndex[m] = *inter;
			m++;
		}
		int nSize = sortlist.size();
		SortSlot(m_BusSlotIndex, nSize);

		for(int k=1; k<m_wTotalCardNum; k++)
		{
			P9054_HANDLE hPlx = NULL;

			for(int l = k; l< m_wTotalCardNum; l++)
			{
				if(l != k && 
					(m_BusSlotIndex[k].dwBus == pstThreadParam[l].hDev->pciSlot.dwBus) &&
					(m_BusSlotIndex[k].dwFunction == pstThreadParam[l].hDev->pciSlot.dwFunction) &&
					(m_BusSlotIndex[k].dwSlot == pstThreadParam[l].hDev->pciSlot.dwSlot))
				{
					hPlx = pstThreadParam[k].hDev;
					pstThreadParam[k].hDev = pstThreadParam[l].hDev;
					pstThreadParam[l].hDev = hPlx;
				}
			}
		}

		if (m_BusSlotIndex[0].dwSlot == 0) // 如果没有多功能卡,
		{
			m_wMFCCardNum = 0;
			//AfxMessageBox(_T("没有多功能能卡!!!"));
			QString str =QString("%1").arg(QObject::tr("没有多功能能卡!!!"));
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);

			return FALSE;
		}
		else
		{	
			m_wMFCCardNum = 1; // 目前只支持1块多功能卡
			m_wAECCardNum = m_wTotalCardNum - 1; 		
		}
	}

	return TRUE;
}

int CPLXCommunication::FindCardBusSlot(WDC_PCI_SCAN_RESULT *pscanResult, WD_PCI_SLOT *pBusSlotIndex)
{
	int dwMinSlot = 0;
	WORD wSubdeviceID;
	WDC_DEVICE_HANDLE   hDev;
	list<WD_PCI_SLOT> sortlist;
	list<WD_PCI_SLOT>::iterator inter;

	// Make sure parameter is valid
	if (pscanResult == NULL || pBusSlotIndex == NULL) return 0;

	PLX_DeviceFind(0x10b5, 0x9054, pscanResult);

	// Find slot of Mutli-Function Card, others are UT cards
	for (DWORD i = 0;i < pscanResult->dwNumDevices;i++)
	{
		// 		CString str;
		// 		str.Format(_T("i = %d, Bus = %d, dwSlot = %d"),i, pscanResult->deviceSlot[i].dwBus, pscanResult->deviceSlot[i].dwSlot);
		// 		AfxMessageBox(str);

		PLX_DeviceOpenBySlot(&hDev, &pscanResult->deviceSlot[i], TRUE);

		Plx_PciReadCfg16(hDev, SUB_DEVICE_ID_H,&wSubdeviceID);

		if (wSubdeviceID == MUTLI_FUNCTION_CARD_ID) {
			pBusSlotIndex[0].dwBus = pscanResult->deviceSlot[i].dwBus;
			pBusSlotIndex[0].dwFunction = pscanResult->deviceSlot[i].dwFunction;
			pBusSlotIndex[0].dwSlot = pscanResult->deviceSlot[i].dwSlot;
		} else {
			sortlist.push_back(pscanResult->deviceSlot[i]);
		}

		PLX_DeviceClose(hDev);
	}

	int j = 1;
	for (inter = sortlist.begin(); inter != sortlist.end(); inter++)
	{
		pBusSlotIndex[j] = *inter;
		j++;
	}
	int nSize = sortlist.size();
	SortSlot(pBusSlotIndex, nSize);

	return pscanResult->dwNumDevices;
}

void CPLXCommunication::SortSlot(WD_PCI_SLOT *pBusSlotIndex, int size)
{
	// songchenguang add start
	// Read SlotConfig.ini

	// Song Chenguang add start on 2008-08-15
	BusSlotOrder flag = ZBusNSlot;	//(BusSlotOrder)GetSlotFlag();
	// Song Chenguang add end on 2008-08-15

	// sort dwslot make the sequence is same as pSlotIndex
	WD_PCI_SLOT tempSlot;
	::ZeroMemory(&tempSlot, sizeof(tempSlot));

	// Bus 正序 Slot 正序
	if (flag == ZBusZSlot)
	{
		for(int i = 2; i<= size; ++i)
		{
			// sort Bus and Slot
			if (!((pBusSlotIndex[i].dwBus > pBusSlotIndex[i-1].dwBus) || 
				((pBusSlotIndex[i].dwBus == pBusSlotIndex[i-1].dwBus)
				&&(pBusSlotIndex[i].dwSlot > pBusSlotIndex[i-1].dwSlot))))
			{
				tempSlot = pBusSlotIndex[i];
				pBusSlotIndex[i] = pBusSlotIndex[i-1];

				int j = 0;
				for (j = i-2;
					!((tempSlot.dwBus > pBusSlotIndex[j].dwBus) || 
					((tempSlot.dwBus == pBusSlotIndex[j].dwBus) &&
					(tempSlot.dwSlot > pBusSlotIndex[j].dwSlot)));
				--j)
				{
					pBusSlotIndex[j+1] = pBusSlotIndex[j];
				}
				pBusSlotIndex[j+1] = tempSlot;
			}
		}
	} 
	// Bus 正序 Slot 逆序
	else if(flag == ZBusNSlot)
	{
		for(int i = 2; i<= size; ++i)
		{
			// sort Bus and Slot
			if (!((pBusSlotIndex[i].dwBus > pBusSlotIndex[i-1].dwBus) || 
				((pBusSlotIndex[i].dwBus == pBusSlotIndex[i-1].dwBus)
				&&(pBusSlotIndex[i].dwSlot < pBusSlotIndex[i-1].dwSlot))))
			{
				tempSlot = pBusSlotIndex[i];
				pBusSlotIndex[i] = pBusSlotIndex[i-1];

				int j = 1;
				for (j = i-2;
					!((tempSlot.dwBus > pBusSlotIndex[j].dwBus) || 
					((tempSlot.dwBus == pBusSlotIndex[j].dwBus) &&
					(tempSlot.dwSlot < pBusSlotIndex[j].dwSlot)));
				--j)
				{
					if (j == 0)
						break;
					pBusSlotIndex[j+1] = pBusSlotIndex[j];

				}
				pBusSlotIndex[j+1] = tempSlot;
			}
		}
	}
	// Bus 逆序 Slot 正序
	else if(flag == NBusZSlot)
	{
		for(int i = 2; i<= size; ++i)
		{
			// sort Bus and Slot
			if (!((pBusSlotIndex[i].dwBus < pBusSlotIndex[i-1].dwBus) || 
				((pBusSlotIndex[i].dwBus == pBusSlotIndex[i-1].dwBus)
				&&(pBusSlotIndex[i].dwSlot > pBusSlotIndex[i-1].dwSlot))))
			{
				tempSlot = pBusSlotIndex[i];
				pBusSlotIndex[i] = pBusSlotIndex[i-1];

				int j = 0;
				for (j = i-2;
					!((tempSlot.dwBus < pBusSlotIndex[j].dwBus) || 
					((tempSlot.dwBus == pBusSlotIndex[j].dwBus) &&
					(tempSlot.dwSlot > pBusSlotIndex[j].dwSlot)));
				--j)
				{
					pBusSlotIndex[j+1] = pBusSlotIndex[j];
				}
				pBusSlotIndex[j+1] = tempSlot;
			}
		}
	}
	// // Bus 逆序 Slot 逆序
	else 
	{
		for(int i = 2; i<= size; ++i)
		{
			// sort Bus and Slot
			if (!((pBusSlotIndex[i].dwBus < pBusSlotIndex[i-1].dwBus) || 
				((pBusSlotIndex[i].dwBus == pBusSlotIndex[i-1].dwBus)
				&&(pBusSlotIndex[i].dwSlot < pBusSlotIndex[i-1].dwSlot))))
			{
				tempSlot = pBusSlotIndex[i];
				pBusSlotIndex[i] = pBusSlotIndex[i-1];

				int j = 0;
				for (j = i-2;
					!((tempSlot.dwBus < pBusSlotIndex[j].dwBus) || 
					((tempSlot.dwBus == pBusSlotIndex[j].dwBus) &&
					(tempSlot.dwSlot < pBusSlotIndex[j].dwSlot)));
				--j)
				{
					pBusSlotIndex[j+1] = pBusSlotIndex[j];
				}
				pBusSlotIndex[j+1] = tempSlot;
			}
		}
	}

}

/** 
* 获取Slot排序标志
* @param     :
* @return    void
* @since     1.00
*/
int CPLXCommunication::GetSlotFlag()
{

	QString strIniFlieName = QDir::currentPath() += "/YiQiConfig.ini";

	bool ret =   QFile::exists(strIniFlieName);
	if (!ret)
	{
		QString str =QString("%1").arg(QObject::tr("没有多功能能卡!!!"));
		QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
		return -1;
	}
	QSettings *setting = new QSettings(strIniFlieName, QSettings::IniFormat);
	int nFlag = setting->value("Slot排序","").toInt();
	return nFlag;

	/*
	CStdioFile file;
	CFileException ex;
	TCHAR strFolder[MAX_PATH];
	GetWorkingFolder(strFolder, MAX_PATH);
	
	//CString strFileName = CString(strFolder) + _T("YiQiConfig.ini");
    
	QString strFileName = QString("%1%2").arg(strFolder).arg(QObject::tr("YiQiConfig.ini"));

	BOOL bRet = file.Open(strFileName, CFile::modeRead|CFile::typeText);
	if (!bRet)
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		//CString str(szError);
		QString str(szError);
		//AfxMessageBox(str);
		//QString str =QString("%1").arg(QObject::tr("没有多功能能卡!!!"));
		QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
		return -1;
	}

	//CString str;
	QString str;
	while (file.ReadString(str))
	{
		if (str.Left(10) == _T("Slot排序："))
		{
			//CString strFlag = str.Right(str.GetLength() - 10);
			QString  strFlag = str.right(str.length() - 10);
			int nFlag = _wtoi(strFlag.GetBuffer(str.GetLength()));
			return nFlag;
		}
	}
	file.Close();
	return -1;
	*/
}

/** 
* 多功能卡初始化
* @param     hDev:
* @return    void
* @since     1.00
*/
BOOL CPLXCommunication::MultiCardInit(P9054_HANDLE hDev)
{
// 	if (g_IniParams.m_nLSMP2_OR_DPC31 > MFC_DPC31_LSMP2_SELECTOR_LSMP2)
// 	{
// 		AfxMessageBox(_T("非法的LSMP2_DPC31值"));
// 		return FALSE;
// 	}
// 	if (g_IniParams.m_nCoder1PerRingNum <= 0 ||
// 		g_IniParams.m_nCoder2PerRingNum <= 0 ||
// 		g_IniParams.m_nCoder3PerRingNum <= 0 ||
// 		g_IniParams.m_nCoder4PerRingNum <= 0 ||
// 		g_IniParams.m_nCoder5PerRingNum <= 0)
// 	{
// 		AfxMessageBox(_T("非法的Coder1PerRingNum值"));
// 		return FALSE;
// 	}

	// REM temp
/*	//	WORD wValue = MFC_DPC31_LSMP2_SELECTOR_LSMP2;
	WORD wValue = 1;	//g_IniParams.m_nLSMP2_OR_DPC31;	
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_LSMP2_DPC31_SWITCH_ADDR, wValue);

	wValue = MPB_CODER1_NUM_PER_RING;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_CODER1_NUM_PER_RING_ADDR, wValue);
 
	wValue = MPB_CODER2_NUM_PER_RING;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_CODER2_NUM_PER_RING_ADDR, wValue);
 
	wValue = MPB_CODER3_NUM_PER_RING;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_CODER3_NUM_PER_RING_ADDR, wValue);
 
	wValue = MPB_CODER4_NUM_PER_RING;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_CODER4_NUM_PER_RING_ADDR, wValue);
 
// 	//	wValue = MPB_CODER5_NUM_PER_RING;
// 	wValue = g_IniParams.m_nCoder5PerRingNum;
// 	WDC_DIAG_ReadWriteAddr(hDev, WDC_WRITE, P9054_ADDR_SPACE0, MPB_CODER5_NUM_PER_RING_ADDR, &wValue, WDC_MODE_16);
	// 测试
	wValue = 1;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0xc, wValue);

	wValue = 1;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0xb0, wValue);

*/
	return TRUE;
}

void CPLXCommunication::ResetHPI_INT(P9054_HANDLE hDev)
{	
/*	WORD wValueL = HPI_RC_INIT_VALUE;
	WORD wValueH = HPI_RC_INIT_VALUE;

	wValueL = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_L);
	wValueH = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_H);

	wValueL |= HPI_HINT_RESET_VALUE;
	wValueH |= HPI_HINT_RESET_VALUE;

	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_L, wValueL);
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_H, wValueH);
*/
}

////////////////////////////////////////////////////////////////
//
// 设置范围
//
////////////////////////////////////////////////////////////////
void CPLXCommunication::SetMonitorRange(P9054_HANDLE hDev, WORD *pInterSpace)
{
	DWORD dwAddr = MONITOR_RANGE_ADDRESS;

	for (int i = 0;i < FLAW_DOT_MAX_NO;i++)
	{
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, dwAddr, pInterSpace[i]);
		dwAddr += 2;
	}
}


//////////////////////////////////////////////////////////////////////////
//
// 读取画图数据
//
//////////////////////////////////////////////////////////////////////////


BOOL CPLXCommunication::PutsReadDrawData(P9054_HANDLE hDev, CPicData *pData)
{
//	pData->m_bDrawed = FALSE;
	// REM temp
	//?
	WORD wEnd = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, MONITOR_CHANNEL_DATA_EN);
	if (wEnd == 0)
		return FALSE;

	DWORD dwAddr = MONITOR_WAVE_DATA_ADDRESS;
	for (int i = 0;i < MONITOR_SIZE;i++)
	{
		pData->m_wData[i] = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, dwAddr);
		dwAddr += 2;
	}

	// 去掉前端的异常值
	pData->m_wData[0] = 2;
	pData->m_wData[1] = 2;
/*
	WORD wValue = PCI_SAMPLE_RESET_ON;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);

	wValue = PCI_SAMPLE_RESET_OFF;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MONITOR_DATA_RESET_ADDRESS, wValue);

	wValue = PCI_SAMPLE_RESET_ON;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MONITOR_CHANNEL_DATA_EN, wValue);

	wValue = PCI_SAMPLE_RESET_OFF;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MONITOR_CHANNEL_DATA_EN, wValue);
*/
}

/** 
* 向DSP发送通道数据
* @param     hDev [in]: AEC卡句柄
* @param     *pallCH_CONFIG [in]: 通道数据
* @param     *pch_config_global [in]: 通道全局数据
* @param     icardno [in]: 卡序号，从0计数
* @return    void
* @since     1.00
*/
/*
void CPLXCommunication::ConfigDSP(P9054_HANDLE hDev, CH_CONFIG *pallCH_CONFIG, CH_CONFIG_GLOBAL *pch_config_global, int icardno)
{
 	int i;
 	// Song Chenguang modify start on 2008-11-11
 	int cnt = sizeof(CH_CONFIG_HD) / (sizeof(WORD)*2) * PER_CARD_CHANNEL_NUM;
 	// Song Chenguang modify end on 2008-11-11
 
 	WORD wValueL = UST_RAM_ADDR & 0xFFFF;
 	WORD wValueH = UST_RAM_ADDR >> 0x10;
 
 	CH_CONFIG_HD temp[PER_CARD_CHANNEL_NUM];
 	for (i = 0; i< PER_CARD_CHANNEL_NUM; i++)
 	{
 		temp[i].ALARM_EN = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].ALARM_EN;
 		temp[i].AUTO_GAIN_EN = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].AUTO_GAIN_EN;
 		temp[i].AUTO_GATE_EN = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].AUTO_GATE_EN;
 		temp[i].CH_BE_GAIN_MUX = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_BE_GAIN_MUX;
 		temp[i].CH_BEMINDATA = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_BEMINDATA;
 		temp[i].CH_DEM_MODE = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_DEM_MODE;
 		temp[i].CH_EN = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_EN;
 		temp[i].CH_FLAW_BE_DISTANCE = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_FLAW_BE_DISTANCE;
 		temp[i].CH_FLAW_DEF = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_FLAW_DEF;
 		temp[i].CH_FLAW_GAIN_MUX = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_FLAW_GAIN_MUX;
 		temp[i].CH_FLAW_SURF_DISTANCE = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_FLAW_SURF_DISTANCE;
 		temp[i].CH_FRONTTIME = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_FRONTTIME;
 		temp[i].CH_GAIN_A = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_GAIN_A;
 		temp[i].CH_GAIN_B = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_GAIN_B;
 		temp[i].CH_GAIN_C = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_GAIN_C;
 		temp[i].CH_GAIN_D = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_GAIN_D;
 		temp[i].CH_MAXBACKTIME = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_MAXBACKTIME;
 		temp[i].CH_MONITOR_RANGE = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_MONITOR_RANGE;
 		temp[i].CH_NO = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_NO;
 		temp[i].CH_REARTIME = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_REARTIME;
 		temp[i].CH_REF_NO = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_REF_NO;
 		temp[i].CH_REJECT_VALUE = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_REJECT_VALUE;
		temp[i].CH_SELFCHECK = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_SELFCHECK;
 		//		temp[i].CH_TGC_DA_START = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_TGC_DA_START;
 		temp[i].CH_GAIN_DIFF = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_GAIN_DIFF;
 		temp[i].CH_TGC_DAC_EN = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_TGC_DAC_EN;
 		temp[i].CH_ZEROCORRECT = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].CH_ZEROCORRECT;
 		temp[i].MIDDLE_VALUE = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].MIDDLE_VALUE;

		temp[i].wGate1Enable = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate1Enable;
		temp[i].wGate1SyncMode = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate1SyncMode;
		temp[i].wGate1SyncThreshold = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate1SyncThreshold;
		temp[i].wGate1Start = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate1Start;
		temp[i].wGate1Width = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate1Width;
		temp[i].wGate1Amp = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate1Amp;
		temp[i].wGate2Enable = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate2Enable;
		temp[i].wGate2SyncMode = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate2SyncMode;
		temp[i].wGate2SyncThreshold = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate2SyncThreshold;
		temp[i].wGate2Start = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate2Start;
		temp[i].wGate2Width = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate2Width;
		temp[i].wGate2Amp = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate2Amp;
		temp[i].wGate3Enable = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate3Enable;
		temp[i].wGate3SyncMode = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate3SyncMode;
		temp[i].wGate3SyncThreshold = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate3SyncThreshold;
		temp[i].wGate3Start = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate3Start;
		temp[i].wGate3Width = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate3Width;
		temp[i].wGate3Amp = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate3Amp;
		temp[i].wGate4Enable = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate4Enable;
		temp[i].wGate4SyncMode = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate4SyncMode;
		temp[i].wGate4SyncThreshold = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate4SyncThreshold;
		temp[i].wGate4Start = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate4Start;
		temp[i].wGate4Width = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate4Width;
		temp[i].wGate4Amp = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wGate4Amp;
		temp[i].wSimpleInterval = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wSimpleInterval;
		temp[i].wSimpleAverage = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wSimpleAverage;
		temp[i].wDelay = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wDelay;
		temp[i].wProbeFreq = pallCH_CONFIG[icardno * PER_CARD_CHANNEL_NUM+i].wProbeFreq;
 	}
 	
 	WORD *p = (WORD *)temp;
 	int j = 0;
 
 	// Write CH_CONFIG DATA  
 	for (i = 0;i < cnt;i ++)
 	{
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wValueL);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wValueH);
 
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L, p[j]);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H, p[j+1]);
 		wValueL += 4;
 		j += 2;
 		Delay(1);
 	}

 	// Write CH_CONFIG_GLOBAL DATA
 	p = (WORD *)pch_config_global;

 	// Song Chenguang modify start on 2008-11-11
 	cnt = sizeof(CH_CONFIG_GLOBAL) / (sizeof(WORD) * 2);
 	// Song Chenguang modify end on 2008-11-11
 	j = 0;
 	for (i = 0;i < cnt;i++)
 	{
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wValueL);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wValueH);
 
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L, p[j]);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H, p[j+1]);
 		wValueL += 4;
 		j += 2;
 		Delay(1);
 	}
}
*/ 
/** 
 * 中断DSP
 * @param     hDev:
 * @return    void
 * @since     1.00
 */
/*
void CPLXCommunication::InterruptDSP(P9054_HANDLE hDev, int nCardNo)
{
 	WORD wAddr_L = DSP_INTERRUPT_L;
 	WORD wAddr_H = DSP_INTERRUPT_H;
 	WORD wValue_L, wValue_H;
 	int n=0;

	wValue_L = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_L);	
	wValue_H = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_H);	
 
 	while (((wValue_L & 0x2)>>1))
 	{
		wValue_L = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_L);
		wValue_H = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_H);	
 
 		Sleep(5);
 		n++;
 		if (n>500)
 		{
 			break;
 		}
 
 	}
 
 	//wValue_L &= 0xfffd;
 	wValue_H &= 0xfffd;
 
 
 	wValue_L |= PCI_DSP_INT_MASK;
 	//wValue_H |= PCI_DSP_INT_MASK;
 
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_L, wValue_L);
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_H, wValue_H);
 
 	wValue_L &= 0xfffd;
 	wValue_H &= 0xfffd;
 
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_L, wValue_L);
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RC_ADDR_H, wValue_H);

}
 */ 
/** 
 * 主机写入DSP DAC数据结构
 * @param     hDev:
 * @return    void
 * @since     1.00
 */
/*
void CPLXCommunication::ConfigDSP_DAC(P9054_HANDLE hDev)
{
 	// 凯达探头dac
 
 	WORD PHY_DAC[8*128];// = {Gain2Data(10.0)}; //20/3*gain
 
 	for (int i=0; i<8*128; i++)
	{
 		PHY_DAC[i] = Gain2Data(1.0);
 	}
 
 	WORD SOFT_DAC[512*8];// = {256};
 
 	for(int i=0; i<512*8; i++)
 	{
 		SOFT_DAC[i] = 256;
 	}
 
 	// Song Chenguang add start on 2009-04-10
 	// 读取数据库DAC数据
 	WORD dacDatas[128];
 	::ZeroMemory(dacDatas, sizeof(WORD)*128);
 
 	try
 	{
 		CDBVariant tmp;
 		CRecordset set(&g_sqlDB);
 		CString strSQL = _T("select * from t_DAC");
 		set.Open(AFX_DB_USE_DEFAULT_TYPE, strSQL);
 
 		// 只读了一行
		for (int i=0; i<128; i++)
 		{
 			// 读DAC值
 			set.GetFieldValue(2+i, tmp);
 			float dacValue = (float)tmp.m_dblVal;
 			dacDatas[i] = Gain2Data(dacValue);
 		}
 		set.Close();
 	}
 	catch(CDBException* e)
 	{
 		e->ReportError();
 		e->Delete();
 	}
 
 	for (int i=0; i<8; i++)
 	{
 		for (int j=0; j<128; j++)
 		{
 			PHY_DAC[i*128 + j] = dacDatas[j];
 		}
 	}
 	// Song Chenguang add end on 2009-04-10
 
 	//testWriteDataHPI
 	WORD ntemp[2]={67,0};
 	WriteDataHPI(hDev, AEC_CH1_JIANBOMOSHI_ADDR+AEC_CHX_FLAW_BE_DIFF_OFFSET, (void *)ntemp, 4);
 	WriteDataHPI(hDev, AEC_CH2_JIANBOMOSHI_ADDR+AEC_CHX_FLAW_BE_DIFF_OFFSET, (void *)ntemp, 4);
 	WriteDataHPI(hDev, AEC_CH3_JIANBOMOSHI_ADDR+AEC_CHX_FLAW_BE_DIFF_OFFSET, (void *)ntemp, 4);
 	WriteDataHPI(hDev, AEC_CH4_JIANBOMOSHI_ADDR+AEC_CHX_FLAW_BE_DIFF_OFFSET, (void *)ntemp, 4);
 	/////////////////////////////
 
 	WriteDataHPI(hDev, FPGA_CH1_PHY_DAC_BASE, (void *)PHY_DAC, 128*8*2);
 	WriteDataHPI(hDev, FPGA_CH2_PHY_DAC_BASE, (void *)PHY_DAC, 128*8*2);
 	WriteDataHPI(hDev, FPGA_CH3_PHY_DAC_BASE, (void *)PHY_DAC, 128*8*2);
 	WriteDataHPI(hDev, FPGA_CH4_PHY_DAC_BASE, (void *)PHY_DAC, 128*8*2);
 
 	WriteDataHPI(hDev, FPGA_CH1_SOFT_DAC_BASE, (void *)SOFT_DAC, 512*8*2);
 	WriteDataHPI(hDev, FPGA_CH2_SOFT_DAC_BASE, (void *)SOFT_DAC, 512*8*2);
 	WriteDataHPI(hDev, FPGA_CH3_SOFT_DAC_BASE, (void *)SOFT_DAC, 512*8*2);
 	WriteDataHPI(hDev, FPGA_CH4_SOFT_DAC_BASE, (void *)SOFT_DAC, 512*8*2);
 
 	WORD wValue = 0;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, SETONE_OR_ALL, wValue);
}
*/
//////////////////////////////////////////////////////////////////////////
//
// 使用HPI方式写数据
//
//////////////////////////////////////////////////////////////////////////
/* 
void CPLXCommunication::WriteDataHPI(P9054_HANDLE hDev, DWORD dwAddr, void *pdata, int size)
{
 	// parameter valid
 	if (pdata == NULL) return;
 
 	WORD wAddr_L = dwAddr & MONITOR_CHANNEL_ADDR_MASK;
 	WORD wAddr_H = dwAddr >> MONITOR_CHANNEL_ADDR_SHF;
 
 	WORD *pwValue = (WORD *)pdata;
 
 	DWORD addr = dwAddr;
 
 	WORD ss = 0;
 
 	int cnt = size / 4;
	for (int i = 0;i < cnt;i++)
 	{
 		wAddr_L = addr & MONITOR_CHANNEL_ADDR_MASK;
 		wAddr_H = addr >> MONITOR_CHANNEL_ADDR_SHF;
 
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wAddr_L);
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wAddr_H);
 
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L, *pwValue);
 		pwValue++;
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H, *pwValue);
 		pwValue++;
 
 		addr += 4;
 		Delay(1);
 	}
 	while(0);
}
*/
////////////////////////////////////////////////////////////////////////
//
// 延迟
//
////////////////////////////////////////////////////////////////////////
void CPLXCommunication::WaveDelay(P9054_HANDLE hDev, WORD nDelay)
{
	// REM temp
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0x122C, nDelay);
}


//						P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, , );
//                      P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, );	