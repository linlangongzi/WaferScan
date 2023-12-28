
#include "windows.h"
#include "headerFile.h"
#include "GlobalFunction.h"

#include <QMessageBox>
#include <QString>
#include <QList>
#include <QDir>
#include <QSettings>


const DWORD MULTI_CARD_FLAG = 0x0101;
const DWORD AEC_CARD_FLAG = 0x0201;
extern CIniParams g_IniParams;

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
	//QMessageBox::warning(NULL,"AScanSave",QObject::tr("开始PLX初始化")); //chenyuzhu add on 2015-1-26
	DWORD dwRet = PLX_LibInit();
	if (dwRet != WD_STATUS_SUCCESS)
	{
		QMessageBox::information(NULL, "Error", QObject::tr("PLX初始化失败,请检查任务管理器中是否有总站程序运行!"), QMessageBox::Yes, QMessageBox::Yes);
		return FALSE;
	}
	//QMessageBox::warning(NULL,"AScanSave",QObject::tr("完成PLX初始化")); //chenyuzhu add on 2015-1-26
	BOOL bRet = BusSlotFindAndSort(pstThreadParam);
	//QMessageBox::warning(NULL,"AScanSave",QObject::tr("查找卡完成")); //chenyuzhu add on 2015-1-26
	if (!bRet)
	{
		//QMessageBox::information(NULL, "Error", QObject::tr("PLX Bus、Slot查询失败!"), QMessageBox::Yes, QMessageBox::Yes);
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
		//QMessageBox::information(NULL, "Error", QObject::tr("卡自检失败!"), QMessageBox::Yes, QMessageBox::Yes);
		return FALSE;
	}
	else
	{
		//卡自检成功
		//		WriteLog(_T("NDT-EventLog.txt"), _T("卡自检成功!"),CFile::modeReadWrite|CFile::modeCreate);
		//QMessageBox::warning(NULL,"AScanSave",QObject::tr("卡自检成功")); //chenyuzhu add on 2015-1-26
		return TRUE;
	}
	return TRUE;//chenyuzhu tiaoshi 2014-10-8
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
//	QMessageBox::warning(NULL,"AScanSave",QObject::tr("开始aec卡自检")); //chenyuzhu add on 2015-1-26
	for (int i = 1;i <= m_wAECCardNum;i++)  //aec卡自检，从thread_param[1]开始 <dwCardCount ----><=g_nAECCardNum
	{
		bselftest = CardSelfTest(pstThreadParam[i].hDev);
		if(!bselftest)  // self test fail
		{
			QString str = QString("%1%2%3").arg(QObject::tr("第 ")).arg(i).arg(QObject::tr(" 块卡自检失败！！！！！"));
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
			return bselftest;
		}
		else
		{
			/* board pass the test, set ok*/
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
			QString strDSPVer = QString("%1%2%3%4%5%6%7%8").arg(QObject::tr("AEC卡")).arg(i).arg(QObject::tr(" DSP版本："))
				.arg((wDSPVer / 0x1000 * 10) + (wDSPVer % 0x1000) / 0x100).arg(".").arg((wDSPVer % 0x100) / 0x10)
				.arg(wDSPVer % 0x10).arg("\r\n");

			
			// FPGA1
			wHPIAddr_L = FPGA1_VER_ADDRESS_L;
			wHPIAddr_H = FPGA1_VER_ADDRESS_H;
			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wHPIAddr_L);
			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wHPIAddr_H);

			WORD wFPGA1Low = P9054_ReadWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L);
			WORD wFPGA1High = P9054_ReadWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H);

			Delay(2);
			WORD wFPGA1Ver = wFPGA1High;

			QString strFPGA1Ver = QString("%1%2%3%4%5%6%7%8").arg(QObject::tr("AEC卡")).arg(i).arg(QObject::tr("  FPGA1版本："))
										.arg((wFPGA1Ver / 0x1000 * 10) + (wFPGA1Ver % 0x1000) / 0x100).arg(".").arg((wFPGA1Ver % 0x100) / 0x10)
										.arg(wFPGA1Ver % 0x10).arg("\r\n");

			
			// FPGA2
			wHPIAddr_L = FPGA2_VER_ADDRESS_L;
			wHPIAddr_H = FPGA2_VER_ADDRESS_H;
			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, wHPIAddr_L);
			P9054_WriteWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, wHPIAddr_H);

			WORD wFPGA2Low = P9054_ReadWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L);
			WORD wFPGA2High = P9054_ReadWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H);

			Delay(2);
			WORD wFPGA2Ver = wFPGA2High;
			QString strFPGA2Ver = QString("%1%2%3%4%5%6%7%8").arg(QObject::tr("AEC卡")).arg(i).arg(QObject::tr("  FPGA2版本："))
				.arg((wFPGA2Ver / 0x1000 * 10) + (wFPGA2Ver % 0x1000) / 0x100).arg(".").arg((wFPGA2Ver % 0x100) / 0x10)
				.arg(wFPGA2Ver % 0x10).arg("\r\n");
			// FPGA3
			WORD wFPGA3Ver = P9054_ReadWord(pstThreadParam[i].hDev, P9054_ADDR_SPACE0, FPGA3_VER_ADDRESS);

			QString strFPGA3Ver = QString("%1%2%3%4%5%6%7%8").arg(QObject::tr("AEC卡")).arg(i).arg(QObject::tr("  FPGA3版本："))
				.arg((wFPGA3Ver / 0x1000 * 10) + (wFPGA3Ver % 0x1000) / 0x100).arg(".").arg((wFPGA3Ver % 0x100) / 0x10)
				.arg(wFPGA3Ver % 0x10).arg("\r\n");
			m_strVersion = m_strVersion + strDSPVer + strFPGA1Ver + strFPGA2Ver + strFPGA3Ver;
		}

	}
//	QMessageBox::warning(NULL,"AScanSave",QObject::tr("aec自检完成")); //chenyuzhu add on 2015-1-26
	bselftest = MultiCardTest(pstThreadParam[0].hDev);   //多功能卡自检，应该用g_nMFCNum;
	if (!bselftest)
	{
		QString str =QString("%1").arg(QObject::tr("多功能卡自检失败，需关机，重新启动机器!"));
		QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
	}
	else
	{
		WORD wMPBVer = P9054_ReadWord(pstThreadParam[0].hDev, P9054_ADDR_SPACE0, 0x00);

		QString strMPBVer = QString("%1%2%3%4%5").arg(QObject::tr("MPB卡版本：")).arg((wMPBVer / 0x1000 * 10) + (wMPBVer % 0x1000) / 0x100)
			.arg(".").arg((wMPBVer % 0x100) / 0x10).arg(wMPBVer % 0x10).arg("\r\n");

		m_strVersion = m_strVersion + strMPBVer;
		bselftest = MultiCardInit(pstThreadParam[0].hDev);

		//卡自检成功写入PLC的工作模式
	//	P9054_WriteWord(pstThreadParam[0].hDev, P9054_ADDR_SPACE0, 0x0c, );

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
	AEC_OperationLamp(hDev, AEC_SYS_BOARD_STATUS_ERROR);

	dwAddr = SELF_TEST_ADDR_L;
	while (dwAddr < SELF_TEST_ADDR_H)
		//	for (int i = 0; i < 32;i++)
	{
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, dwAddr, wValueWrite);
		dwAddr += 0x2;
		Delay(2);
	}

	// Song Chenguang add start on 2009-03-25
	int nResetCount = 0;
	DWORD dwRestAddr = 0x6c;

	dwAddr = SELF_TEST_ADDR_L;
	while (dwAddr < SELF_TEST_ADDR_H)
		//	for (i = 0; i < 32;i++)
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
			dwAddr = SELF_TEST_ADDR_L;
			while (dwAddr < SELF_TEST_ADDR_H)
			{
				P9054_WriteWord(hDev, P9054_ADDR_SPACE0, dwAddr, wValueWrite);
				dwAddr += 0x2;
				Delay(2);
			}

			dwAddr = SELF_TEST_ADDR_L;
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
			//AfxMessageBox(_T("HPI 自检出错 1！！！！！"));
			QString str =QString("%1").arg(QObject::tr("HPI 自检出错 1！！！！！"));
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
			return FALSE;
		}

		wValueRead = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H);
		if (wValueRead != wValueWrite) 
		{
			//AfxMessageBox(_T("HPI 自检出错 2！！！！！"));
			QString str =QString("%1").arg(QObject::tr("HPI 自检出错 2！！！！！"));
			QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
			return FALSE;
		}

		wHPIAddr_L += 0x4;
		Delay(2);
	}

	// 使工作卡系统指示灯变绿
	AEC_OperationLamp(hDev, AEC_SYS_BOARD_STATUS_OK);

	return TRUE;
}

BOOL CPLXCommunication::AEC_OperationLamp(P9054_HANDLE hDev, WORD nValue)
{
	WORD dwHPIAddr_L = AEC_SYS_BOARD_STATUS_ADDR_L;
	WORD dwHPIAddr_H = AEC_SYS_BOARD_STATUS_ADDR_H;
	WORD nHighValue = 0x0;

	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_L, dwHPIAddr_L);
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_RA_ADDR_H, dwHPIAddr_H);

	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_L, nValue);
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, HPI_R_SD_ADDR_H, nHighValue);

	return TRUE;
}


////////////////////////////////////////////////////////////////
//
// 多功能卡自检
//
////////////////////////////////////////////////////////////////

BOOL CPLXCommunication::MultiCardTest(P9054_HANDLE hDev)
{
//	QMessageBox::warning(NULL,"AScanSave",QObject::tr("开始多功能卡自检")); //chenyuzhu add on 2015-1-26
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
//	QMessageBox::warning(NULL,"AScanSave",QObject::tr("多功能卡自检11")); //chenyuzhu add on 2015-1-26
	for (int i = 0;i < MULTI_CARD_SELF_TEST_ADDR_SIZE / 2;i++)
	{
		P9054_WriteWord(hDev, P9054_ADDR_SPACE0, dwAddr, wValueWrite);
		dwAddr += 2;
		// songchenguang add start
		Delay(1);
		// songchenguang add end
	}
//	QMessageBox::warning(NULL,"AScanSave",QObject::tr("多功能卡自检22")); //chenyuzhu add on 2015-1-26
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
//	QMessageBox::warning(NULL,"AScanSave",QObject::tr("多功能卡自检22")); //chenyuzhu add on 2015-1-26
	// 板卡ready  变绿
	nValue = MPB_CARD_READY_OK;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_CARD_READY_ADDR, nValue);
    Delay(1);

	// 多功能卡ReadyOK标志
	nValue = MPB_SYS_READY_OK;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_SYS_READY_ADDR, nValue);
//	QMessageBox::warning(NULL,"AScanSave",QObject::tr("多功能卡自检完成")); //chenyuzhu add on 2015-1-26
	return TRUE;
}

// 配件配置发现与排序
BOOL CPLXCommunication::BusSlotFindAndSort(AECTHREAD_PARAM* pstThreadParam)
{
	// 计算板卡数量
	QList<WD_PCI_SLOT> sortlist;
	QList<WD_PCI_SLOT>::iterator inter;

	for (int j = 0; j< MAX_CARD_NO; j++)
	{
		m_BusSlotIndex[j].dwSlot = 0;
	}

	m_wTotalCardNum = P9054_CountCards (0x10b5, 0x9054);
//	QMessageBox::warning(NULL,"AScanSave",QObject::tr("计算板卡数量为")); //chenyuzhu add on 2015-1-26
	if(m_wTotalCardNum <= 0) //chenyuzhu add on 2015-1-26
	//if(m_wTotalCardNum == 0) //chenyuzhu modif on 2015-1-26
	{ 
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
				
				QString str = QString("%1%2%3").arg(QObject::tr("P9054_Open ")).arg(i).arg(QObject::tr(" 没有发现采集卡和接收卡，请关机后安装板卡! !") );
  				QMessageBox::information(NULL, "Error", str, QMessageBox::Yes, QMessageBox::Yes);
				return FALSE;
			} 
			else 
			{
				DWORD  dwSeriNumb = P9054_ReadPCIReg( hPlx, 0x2E);
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
	QList<WD_PCI_SLOT> sortlist;
	QList<WD_PCI_SLOT>::iterator inter;

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
}

/** 
* 多功能卡初始化
* @param     hDev:
* @return    void
* @since     1.00
*/
BOOL CPLXCommunication::MultiCardInit(P9054_HANDLE hDev)
{

	WORD wValue = 1;	
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_LSMP2_DPC31_SWITCH_ADDR, wValue);

	wValue = MPB_CODER1_NUM_PER_RING;
	//wValue = 14400;//chenyuzhu add on 2014-8-8
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_CODER1_NUM_PER_RING_ADDR, wValue);

	wValue = MPB_CODER2_NUM_PER_RING;
	//wValue = 14400;//chenyuzhu add on 2014-8-8
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_CODER2_NUM_PER_RING_ADDR, wValue);

	wValue = MPB_CODER3_NUM_PER_RING;
	//wValue = 14400;//chenyuzhu add on 2014-8-8
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_CODER3_NUM_PER_RING_ADDR, wValue);

	wValue = MPB_CODER4_NUM_PER_RING;
	//wValue = 14400;//chenyuzhu add on 2014-8-8
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_CODER4_NUM_PER_RING_ADDR, wValue);

	//chenyuzhu add start on 2014-8-8
//	wValue = MPB_CODER5_NUM_PER_RING;
//	wValue = 14400;//chenyuzhu add on 2014-8-8
//	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, MPB_CODER5_NUM_PER_RING_ADDR, wValue);
	//chenyuzhu add end on 2014-8-8
	wValue = 1;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0xc, wValue);

#if PLC_DEBUG
	wValue = 1;//改为1测试，正常为0 wangwei on 2014-10-8
#else
	wValue = 0;//改为1测试，正常为0 wangwei on 2014-10-8
#endif

	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, 0xb0, wValue);

	return TRUE;
}

void CPLXCommunication::ResetHPI_INT(P9054_HANDLE hDev)
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

////////////////////////////////////////////////////////////////
//
// 设置范围
//
////////////////////////////////////////////////////////////////
void CPLXCommunication::SetMonitorRange(P9054_HANDLE hDev, WORD *pInterSpace)
{
	DWORD dwAddr = MONITOR_RANGE_ADDR;

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
	if(pData == NULL)
		return FALSE;

	WORD wEnd = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, FLAW_END_ADDR);
	if (wEnd == PCI_SAMPLE_NO_END)
		return FALSE;

	DWORD dwAddr = DEFECT_REAL_TIME_DATA_ADDR;
	for (int i = 0;i < MONITOR_DATA_SIZE/AD_SIZE;i++)
	{
		pData->m_wData[i] = P9054_ReadWord(hDev, P9054_ADDR_SPACE0, dwAddr);
		dwAddr += 2;
	}

	// 去掉前端的异常值
	pData->m_wData[0] = 2;
	pData->m_wData[1] = 2;
	WORD wValue = PCI_SAMPLE_RESET_ON;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_RESET_OFFSET, wValue);

	wValue = PCI_SAMPLE_RESET_OFF;
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_RESET_OFFSET, wValue);
	return TRUE;
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

//给硬件下发的参数
typedef struct {
	WORD CH_DEM_MODE;   // 检波模式  全波0，正半波1，负半波2
	WORD MIDDLE_VALUE;  // AD中值
	WORD CH_FRONTTIME;	//闸门前沿	mm			
	WORD CH_REARTIME;   //闸门后沿  mm          
	WORD CH_MAXBACKTIME;	//最大底波位置 mm
	WORD CH_BEMINDATA;		//底波阈值
	WORD CH_GAIN_DIFF;		// 伤波通道与底波通道增益差值
	WORD CH_GAIN_A;			
	WORD CH_GAIN_B;
	WORD CH_GAIN_C;
	WORD CH_GAIN_D;			//增益值		dB
	WORD CH_REJECT_VALUE;	//抑制
	WORD CH_ZEROCORRECT;	//延迟		mm
	WORD CH_FLAW_GAIN_MUX;	//伤波衰减档	0dB：0；-6dB：1；-12dB:2，....-42dB：7
	WORD CH_BE_GAIN_MUX;	//底波衰减档	0dB：0；-6dB：1；-12dB:2，....-42dB：7
	WORD CH_TGC_DAC_EN;		//DAC开启		关闭：0；开启：1	
	WORD CH_FLAW_SURF_DISTANCE;				//上盲区		mm
	WORD CH_FLAW_BE_DISTANCE;				//下盲区		mm
	WORD CH_EN;								//通道开启 关闭：0；开启：1
	WORD AUTO_GAIN_EN;						//自动增益 关闭：0；开启：1
	WORD AUTO_GATE_EN;						//自动闸门 关闭：0；开启：1
	WORD ALARM_EN;							//报警开关 关闭：0；开启：1
	WORD CH_REF_NO;							//起始通道号
	WORD CH_FLAW_DEF;						
	WORD CH_SELFCHECK;						//自检开关 关闭：0；开启：1
	WORD CH_MONITOR_RANGE;					
	WORD CH_NO;								//通道号
} CH_CONFIG_HD;


void CPLXCommunication::ConfigDSP(P9054_HANDLE hDev, CH_CONFIG *pallCH_CONFIG, CH_CONFIG_GLOBAL *pch_config_global, int icardno)
{
	int i;
	int cnt = sizeof(CH_CONFIG_HD)*8;
	WORD wValueL	=	UST_RAM_ADDR & 0xFFFF;
	WORD wValueH	=	UST_RAM_ADDR >> 0x10;
	CH_CONFIG_HD temp[32];

 	for (i = 0; i<32; i++)
 	{
		temp[i].ALARM_EN = pallCH_CONFIG[icardno * 32+i].ALARM_EN;
		temp[i].AUTO_GAIN_EN = pallCH_CONFIG[icardno * 32+i].AUTO_GAIN_EN;
		temp[i].AUTO_GATE_EN = pallCH_CONFIG[icardno * 32+i].AUTO_GATE_EN;
		temp[i].CH_BE_GAIN_MUX = pallCH_CONFIG[icardno * 32+i].CH_BE_GAIN_MUX;
		temp[i].CH_BEMINDATA = pallCH_CONFIG[icardno * 32+i].CH_BEMINDATA;
		temp[i].CH_DEM_MODE = pallCH_CONFIG[icardno * 32+i].CH_DEM_MODE;
		temp[i].CH_EN = pallCH_CONFIG[icardno * 32+i].CH_EN;
		temp[i].CH_FLAW_BE_DISTANCE = pallCH_CONFIG[icardno * 32+i].CH_FLAW_BE_DISTANCE;
		temp[i].CH_FLAW_DEF = pallCH_CONFIG[icardno * 32+i].CH_FLAW_DEF;
		temp[i].CH_FLAW_GAIN_MUX = pallCH_CONFIG[icardno * 32+i].CH_FLAW_GAIN_MUX;
		temp[i].CH_FLAW_SURF_DISTANCE = pallCH_CONFIG[icardno * 32+i].CH_FLAW_SURF_DISTANCE;
		temp[i].CH_FRONTTIME = pallCH_CONFIG[icardno * 32+i].CH_FRONTTIME; //闸门前沿chenyuzhu
		temp[i].CH_GAIN_A = pallCH_CONFIG[icardno * 32+i].CH_GAIN_A;
		temp[i].CH_GAIN_B = pallCH_CONFIG[icardno * 32+i].CH_GAIN_B;
		temp[i].CH_GAIN_C = pallCH_CONFIG[icardno * 32+i].CH_GAIN_C;
		temp[i].CH_GAIN_D = pallCH_CONFIG[icardno * 32+i].CH_GAIN_D;
		temp[i].CH_MAXBACKTIME = pallCH_CONFIG[icardno * 32+i].CH_MAXBACKTIME; //底波闸门后沿chenyuzhu
		temp[i].CH_MONITOR_RANGE = pallCH_CONFIG[icardno * 32+i].CH_MONITOR_RANGE;
		temp[i].CH_NO = pallCH_CONFIG[icardno * 32+i].CH_NO;
		temp[i].CH_REARTIME = pallCH_CONFIG[icardno * 32+i].CH_REARTIME; //闸门后沿chenyuzhu
		temp[i].CH_REF_NO = pallCH_CONFIG[icardno * 32+i].CH_REF_NO;
		temp[i].CH_REJECT_VALUE = pallCH_CONFIG[icardno * 32+i].CH_REJECT_VALUE;
		temp[i].CH_SELFCHECK = pallCH_CONFIG[icardno * 32+i].CH_SELFCHECK;
		temp[i].CH_GAIN_DIFF = pallCH_CONFIG[icardno * 32+i].CH_GAIN_DIFF;
		temp[i].CH_TGC_DAC_EN = pallCH_CONFIG[icardno * 32+i].CH_TGC_DAC_EN;
		temp[i].CH_ZEROCORRECT = pallCH_CONFIG[icardno * 32+i].CH_ZEROCORRECT; //延迟chenyuzhu
		temp[i].MIDDLE_VALUE = pallCH_CONFIG[icardno * 32+i].MIDDLE_VALUE;
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
 	cnt = sizeof(CH_CONFIG_GLOBAL)/4;
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
 
/** 
 * 中断DSP
 * @param     hDev:
 * @return    void
 * @since     1.00
 */
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
 
/** 
 * 主机写入DSP DAC数据结构
 * @param     hDev:
 * @return    void
 * @since     1.00
 */

void CPLXCommunication::ConfigDSP_DAC(P9054_HANDLE hDev,QSqlDatabase *databaseConnection)
{
 	// 凯达探头dac
 	WORD PHY_DAC[8*128];       // = {Gain2Data(10.0)}; //20/3*gain
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
 
 	//try
 	//{
 		//CDBVariant tmp;
 		//CRecordset set(&g_sqlDB);
 		//QString strSQL = _T("select * from t_DAC");
		//set.Open(AFX_DB_USE_DEFAULT_TYPE, strSQL);
		// 只读了一行
		//for (int i=0; i<128; i++)
		//{
		//	// 读DAC值
		//	set.GetFieldValue(2+i, tmp);
		//	float dacValue = (float)tmp.m_dblVal;
		//	dacDatas[i] = Gain2Data(dacValue);
		//}
		//set.Close();


		QSqlQuery initQuery(*databaseConnection);
		initQuery.exec("select * from t_DAC");
		for (int i=0; i<128; i++)
		{
			float dacValue = initQuery.value(i+2).toFloat();
			dacDatas[i] = Gain2Data(dacValue);
		}

 		
//  	}
//  	catch(CDBException* e)
//  	{
//  		e->ReportError();
//  		e->Delete();
//  	}
 
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

//////////////////////////////////////////////////////////////////////////
//
// 使用HPI方式写数据
//
//////////////////////////////////////////////////////////////////////////
 
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

////////////////////////////////////////////////////////////////////////
//
// 延迟
//
////////////////////////////////////////////////////////////////////////
void CPLXCommunication::WaveDelay(P9054_HANDLE hDev, WORD nDelay)
{
	P9054_WriteWord(hDev, P9054_ADDR_SPACE0, PCI_SAMPLE_ZERO_OFFSET, nDelay);
}


//						P9054_WriteWord(pThreadParam->hDev, P9054_ADDR_SPACE0, , );
//                      P9054_ReadWord(pThreadParam->hDev, P9054_ADDR_SPACE0, );	