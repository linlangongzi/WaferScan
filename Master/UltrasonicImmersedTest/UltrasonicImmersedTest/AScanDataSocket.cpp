// AScanDataSocket.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "AScanDataSocket.h"
#include "ExButton.h"
#include "SysParam.h"
#include "CScanDlgBar.h"
#include "palettedog.h"		//machao add on 2013-07-25
#include "CScanParamPanel.h"
#include "CScanParamDockBar.h"
#include "ProbeInfoDlg.h"
#include "ChannelPanel.h"
#include "ChannelInfoDockView.h"
#include "MechanicalControl.h"
#include "MechanicalControlPanel.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "WSocket.h"
#include "ARSSocket.h"
#include "DataSocket.h"
#include "FeatureInfoWnd.h"
#include "MainFrm.h"
#include "mdsocx.h"
#include "MovePathView.h"
#include "GlobalFunction.h"
#include "UltrasonicImmersedTestDoc.h"

extern BOOL g_bDrawRealTime;
const int ASCAN_DATA_NUM = 30;
int CAScanDataSocket::m_nNo = -1;
// CAScanDataSocket

CAScanDataSocket::CAScanDataSocket()
{
	InitParam();
	m_nNo++;
	m_nObjectNum = m_nNo;
}

CAScanDataSocket::~CAScanDataSocket()
{
}


// CAScanDataSocket member functions

void CAScanDataSocket::OnReceive(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	if( nErrorCode != 0) 
		return;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

//	pFrame->m_nNetMonitorCount = 0;

	CurRecLen = Receive( &ReceivedData[SumRecLen], A_SCAN_PACKAGE_SIZE - SumRecLen);
	if( CurRecLen == SOCKET_ERROR || CurRecLen <=0) 
	{
		int nRet = GetLastError();			

		CString str;
		str.Format(_T("监视数据socket error = %d\n"), nRet);
		pFrame->m_wndStatusBar.SetPaneText(1, str);
		TRACE(_T("socket error = %d\n"), nRet);

		this->Close();
		this->InitParam();
		pFrame->OnAScanDataStart();

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CARSSocket::OnReceive"),"Leave");

		return;
	}

	SumRecLen += CurRecLen;

	if( SumRecLen < A_SCAN_PACKAGE_SIZE) 
	{
	}
	else 
	{
		if(pFrame)
		{
			if(pFrame->m_bSaveAScan) // wangbingu add on 2014-10-16 由m_bSaveAScan来决定发送数据出
			{
				PACKET_HEAD_t stPacketHead;
				memset( &stPacketHead, 0, NETHEAD_SIZE);
				memcpy( &stPacketHead, ReceivedData, NETHEAD_SIZE);
				DWORD cmdLine = stPacketHead.dwCommand;
				int dataSize = stPacketHead.dwSize;

				// 获取数据
				if(cmdLine == CMD_ASCAN_RET_DATA_STORE)
				{
					// wangbingfu modify start on 2014-10-14 加上指针判断
					CMechanicalControlPanel* pPanel = NULL;
					if(pFrame)
					{
						pPanel = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
					}
					else
					{
						return;
					}
					// wangbingfu modify start on 2014-10-14
					
					int nRealChannelNo = m_nObjectNum * 2 + stPacketHead.wChannelIndex;
					// wangbingfu modify start on 2014-10-14 在轨迹运行中时才把A扫数据加入队列,暂停则不				
					// m_bPathExecute 没有暂停概念
					short status = 0;
					if(pPanel)
					{
						if(pPanel->m_pMovePathView)
						{
							status = pPanel->m_pMovePathView->m_GraphMove.GetWorkStatus();
						}
						else
						{
							return;
						}
					}
					else
					{
						return;
					}
					
				//	if (pPanel->m_bPathExecute && pPanel->m_SysParamDlg.m_pbChannelOpen[nRealChannelNo])
					if (status == 1 && pPanel->m_bPathExecute && pPanel->m_SysParamDlg.m_pbChannelOpen[nRealChannelNo])
					// wangbingfu modify end on 2014-10-14
					{
						CPicData_L picData[ASCAN_DATA_NUM];
						char* pData = ReceivedData + NETHEAD_SIZE;
						int nWaveDataSize = sizeof(WORD) * DATA_SIZE;
						int nFeatureDataSize = sizeof(CH_FLAWDATA_CSCAN_S);
						int nAxisIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();

						for (int i=0; i<ASCAN_DATA_NUM; i++)
						{
							memcpy(&(picData[i].m_wData), pData, nWaveDataSize);
							memcpy(&(picData[i].m_stFlawData), pData + nWaveDataSize, nFeatureDataSize);
							double dX = 0;
							double dY = 0;
							double dZ = 0;
							double dA = 0;
							double dB = 0;
							double dW = 0;
							double dR = 0;

							GetCScanDataCoordinate(picData[i].m_stFlawData, dX, dY, dZ, 
								dR, dB, dW, dA, 0, 
								nAxisIndex, m_nObjectNum, stPacketHead.wChannelIndex);

							picData[i].m_fX = dX;
							picData[i].m_fY = dY;
							picData[i].m_fZ = dZ;
							picData[i].m_fA = dA;
							picData[i].m_fB = dB;
							picData[i].m_fW = dW;
							picData[i].m_fR = dR;

							picData[i].m_nCardIndex = m_nObjectNum;
							picData[i].m_nChannelIndex = stPacketHead.wChannelIndex;

							pData += sizeof(CSCAN_CH_SEND_MONITOR_DATA_t);
						}

						CUltrasonicImmersedTestDoc* pDoc = (CUltrasonicImmersedTestDoc*)pFrame->GetActiveDocument();
						for (int i=0; i<ASCAN_DATA_NUM; i++)
							pDoc->m_qAScanData[nRealChannelNo].push(picData[i]);
		/*
						//////////////////////////////////////////////////////////////////////////
						// test
						fileTxtData.Open(_T("C:\\A扫数据记录.txt"), CFile::modeReadWrite, NULL);
						for (int i=0; i<ASCAN_DATA_NUM; i++)
						{
							CString str;
							str.Format(_T("%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\r\n"), 
								picData[i].m_fX, picData[i].m_fY, picData[i].m_fZ, picData[i].m_fA, picData[i].m_fB, picData[i].m_fW, picData[i].m_fR,
								picData[i].m_stFlawData.wGate2MPTime, picData[i].m_stFlawData.wGate2MPAmp, picData[i].m_stFlawData.wGate2FPFirstTime, picData[i].m_stFlawData.wGate2FPTime, picData[i].m_stFlawData.wGate2FPAmp,
								picData[i].m_stFlawData.wGate3MPTime, picData[i].m_stFlawData.wGate3MPAmp, picData[i].m_stFlawData.wGate3FPFirstTime, picData[i].m_stFlawData.wGate3FPTime, picData[i].m_stFlawData.wGate3FPAmp,
								picData[i].m_stFlawData.wGate4MPTime, picData[i].m_stFlawData.wGate4MPAmp, picData[i].m_stFlawData.wGate4FPFirstTime, picData[i].m_stFlawData.wGate4FPTime, picData[i].m_stFlawData.wGate4FPAmp);
							fileTxtData.WriteString(str);
						}
						
						fileTxtData.Close();
						//////////////////////////////////////////////////////////////////////////
		*/			
		#if 0
						if(g_bDrawRealTime)
						{
							/*if(nRealChannelNo == 0)
							{
								fileTxtData.Open(_T("C:\\A扫数据记录1.txt"), CFile::modeReadWrite, NULL);
							}
							else if(nRealChannelNo == 1)
							{
								fileTxtData.Open(_T("C:\\A扫数据记录2.txt"), CFile::modeReadWrite, NULL);
							}
							else
							{
								;//
							}*/
							fileTxtData.Open(_T("C:\\A扫数据记录.txt"), CFile::modeReadWrite, NULL);
							
							for (int i=0; i<ASCAN_DATA_NUM; i++)
							{
								CString str;
								str.Format(_T("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\r\n"), picData[i].m_stFlawData.wXLow16, picData[i].m_stFlawData.wXHigh16, picData[i].m_stFlawData.wYLow16, picData[i].m_stFlawData.wYHigh16, picData[i].m_stFlawData.wZLow16, picData[i].m_stFlawData.wZHigh16, picData[i].m_stFlawData.wALow16, picData[i].m_stFlawData.wAHigh16, picData[i].m_stFlawData.wBLow16, picData[i].m_stFlawData.wBHigh16, picData[i].m_stFlawData.wW1Low16, picData[i].m_stFlawData.wW1High16, picData[i].m_stFlawData.wW2Low16, picData[i].m_stFlawData.wW2High16);
								fileTxtData.WriteString(str);
							}

							fileTxtData.Close();
						}
		#endif
					}
					
				}
			}
		}

		InitParam();
	}


	CAsyncSocket::OnReceive(nErrorCode);
}

void CAScanDataSocket::InitParam() 
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CARSSocket::InitParam"),"Enter");
	CurRecLen = 0;
	SumRecLen = 0;
	memset( ReceivedData, 0, sizeof( ReceivedData));

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CARSSocket::InitParam"),"Leave");
}

void CAScanDataSocket::OnConnect(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	char szSendData[NETHEAD_SIZE];
	memset( szSendData, 0, sizeof(szSendData));

	PACKET_HEAD_t stPacketHead;

	stPacketHead.dwCommand = CMD_ASCAN_DATA_STORE;
	stPacketHead.wChannelIndex = 0;
	stPacketHead.wGateIndex = 0;
	stPacketHead.dwSize = 0;
	memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));

	int nRet = Send( szSendData, sizeof(stPacketHead), 0);
	if (nRet == SOCKET_ERROR )
	{
		AfxMessageBox(_T("向服务器发送数据失败！"), MB_OK);
		CAsyncSocket::OnConnect(nErrorCode);
		return;
	}
/*
	//////////////////////////////////////////////////////////////////////////
	// test

	fileTxtData.Open(_T("C:\\A扫数据记录.txt"), CFile::modeCreate | CFile::modeReadWrite, NULL);
	CString str;
	str.Format(_T("X\t\tY\t\tZ\t\tA\t\tB\t\tW\t\tR\t\tGate2MPTime\t\tGate2MPAmp\t\tGate2FPFirstTime\t\tGate2FPTime\t\tGate2FPAmp\t\tGate3MPTime\t\tGate3MPAmp\t\tGate3FPFirstTime\t\tGate3FPTime\t\tGate3FPAmp\t\tGate4MPTime\t\tGate4MPAmp\t\tGate4FPFirstTime\t\tGate4FPTime\t\tGate4FPAmp\r\n"));
	fileTxtData.WriteString(str);
	fileTxtData.Close();
	//////////////////////////////////////////////////////////////////////////
*/
	CAsyncSocket::OnConnect(nErrorCode);
}
