// DataSocket.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
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
#include "WSocket.h"
#include "ARSSocket.h"
#include "DataSocket.h"
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "mdsocx.h"
#include "MovePathView.h"
#include "GlobalFunction.h"
#include "MainFrm.h"
#include "DataSocket.h"
#include "UltrasonicImmersedTestView.h"

extern BOOL g_bDrawRealTime;
// CDataSocket
int CDataSocket::m_nNo = -1;

CDataSocket::CDataSocket()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDataSocket::CDataSocket"),"Enter");

	InitParam();
	m_count = 0;
	m_begin = 0;
	m_nNo++;
	m_nObjectNum = m_nNo;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDataSocket::CDataSocket"),"Leave");
}

CDataSocket::~CDataSocket()
{
}



// CDataSocket member functions
void CDataSocket::InitParam( void) 
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDataSocket::InitParam"),"Enter");

	m_CmdLine = 0;
	CurRecLen = 0;
	SumRecLen = 0;
	memset( ReceivedData, 0, sizeof( ReceivedData));

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDataSocket::InitParam"),"Leave");
}



void CDataSocket::OnReceive(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDataSocket::OnReceive"),"Enter");

	if( nErrorCode != 0) 
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDataSocket::OnReceive"),"Leave");
		return;
	}

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	pFrame->m_nNetFeatureCount = 0;

	CurRecLen = this->Receive( &ReceivedData[SumRecLen], FEATURE_DATA_SIZE - SumRecLen);
	if( CurRecLen == SOCKET_ERROR || CurRecLen <=0) 
	{
		int nRet = GetLastError();

		TRACE(_T("socket error = %d\r\n"), nRet);
		Close();

		InitParam();
		pFrame->TransFeatureData();

		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDataSocket::OnReceive"),"Leave");

		return;
	}
	
	SumRecLen += CurRecLen;
	ASSERT(SumRecLen <= FEATURE_DATA_SIZE);
	if( SumRecLen < NETHEAD_SIZE+DATA_FEATURE_RET_LEN) 
	{
	} 
	else 
	{
		PACKET_HEAD_t stPacketHead;
		memset( &stPacketHead, 0, NETHEAD_SIZE); 
		memcpy( &stPacketHead, ReceivedData, NETHEAD_SIZE);
		DWORD cmdLine = stPacketHead.dwCommand;
		int dataSize = stPacketHead.dwSize;
		int nChannelIndex = stPacketHead.wChannelIndex;

		if( cmdLine == CDM_ASCAN_RET_WAVE) 
		{
			CMechanicalControlPanel* pView = pFrame->m_wndMechanicalControl.m_pMechanicalControlView;
			const int nOneChannelSize = sizeof(CH_FLAWDATA_CSCAN_S)*128;

			if (nChannelIndex == 0 || nChannelIndex == 1)
			{
				CH_FLAWDATA_CSCAN_S* pData = new CH_FLAWDATA_CSCAN_S[128];
				// Song Chenguang add start on 2012-11-05
				if (pData == NULL)
				{
					AfxMessageBox(_T("CDataSocket::OnReceive new CH_FLAWDATA_CSCAN_S[]失败！"));

					LOG4CXX_ERROR(log4cxx::Logger::getLogger("CDataSocket::OnReceive"),"Leave");
					return;
				}
				// Song Chenguang add end on 2012-11-05
				::ZeroMemory(pData, nOneChannelSize);
				memcpy(pData, ReceivedData + NETHEAD_SIZE, nOneChannelSize);
				
				pView->AddScanData(pData, NET_DATA_SIZE, m_nObjectNum, nChannelIndex);
#if 0
				//////////////////////////////////////////////////////////////////////////
				// test
				if(g_bDrawRealTime)
				{
					int nAxisIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();
					int nRealChannelNo = m_nObjectNum * 2 + stPacketHead.wChannelIndex;
/*					if(nRealChannelNo == 0)
					{
						fileTxtData.Open(_T("C:\\C扫数据记录1.txt"), CFile::modeReadWrite, NULL);
					}
					else if(nRealChannelNo == 1)
					{
						fileTxtData.Open(_T("C:\\C扫数据记录2.txt"), CFile::modeReadWrite, NULL);
					}
					else
					{
						;//
					}*/
					fileTxtData.Open(_T("C:\\C扫数据记录1.txt"), CFile::modeReadWrite, NULL);

					for (int i=0; i<NET_DATA_SIZE; i++)
					{
				/*		double dX = 0;
						double dY = 0;
						double dZ = 0;
						double dA = 0;
						double dB = 0;
						double dW = 0;
						double dR = 0;

						GetCScanDataCoordinate(pData[i], dX, dY, dZ, 
							dR, dB, dW, dA, 0, 
							nAxisIndex, m_nObjectNum, stPacketHead.wChannelIndex);
						CString str;
						str.Format(_T("%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%f\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\r\n"), 
							dX, dY, dZ, dA, dB, dW, dR,
							pData[i].wGate2MPTime, pData[i].wGate2MPAmp, pData[i].wGate2FPFirstTime, pData[i].wGate2FPTime, pData[i].wGate2FPAmp,
							pData[i].wGate3MPTime, pData[i].wGate3MPAmp, pData[i].wGate3FPFirstTime, pData[i].wGate3FPTime, pData[i].wGate3FPAmp,
							pData[i].wGate4MPTime, pData[i].wGate4MPAmp, pData[i].wGate4FPFirstTime, pData[i].wGate4FPTime, pData[i].wGate4FPAmp);*/
						CString str;
						str.Format(_T("%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\r\n"), pData[i].wXLow16, pData[i].wXHigh16, pData[i].wYLow16, pData[i].wYHigh16, pData[i].wZLow16, pData[i].wZHigh16, pData[i].wALow16, pData[i].wAHigh16, pData[i].wBLow16, pData[i].wBHigh16, pData[i].wW1Low16, pData[i].wW1High16, pData[i].wW2Low16, pData[i].wW2High16);
						fileTxtData.WriteString(str);
					}

					fileTxtData.Close();
				}
				//////////////////////////////////////////////////////////////////////////
#endif
				delete []pData;
				pData = NULL;
			} 
			else
			{
			}
		}

		InitParam();
#ifdef _DEBUG
		//帧test
		m_count++;
		if(m_count == 60)
		{
			CUltrasonicImmersedTestView* pView = (CUltrasonicImmersedTestView*)pFrame->m_wndSplitter.GetPane(1,0);
			int nCurrentTime = GetTickCount();
			int nFrame = 60000.0f / float(nCurrentTime - m_begin);
			m_begin = nCurrentTime;
			m_count = 0;

			CString strFrame;
			strFrame.Format(_T("%d"),nFrame);
			CDC *pDC = pView->GetDC();
			//	pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->TextOut(0, 50, strFrame);
			pView->ReleaseDC(pDC);
		}
#endif
	}


	CAsyncSocket::OnReceive(nErrorCode);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDataSocket::OnReceive"),"Leave");
}

void CDataSocket::OnConnect(int nErrorCode)
{
	// TODO: Add your specialized code here and/or call the base class
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDataSocket::OnConnect"),"Enter");

	char szSendData[NETHEAD_SIZE];
	memset( szSendData, 0, sizeof(szSendData));

	PACKET_HEAD_t stPacketHead;

	stPacketHead.dwCommand = CMD_ASCAN;	// 需改为接收特征值数据
	stPacketHead.wChannelIndex = 1;
	stPacketHead.wGateIndex = 0;
	stPacketHead.dwSize = 0;
	memcpy(szSendData, &stPacketHead, sizeof(stPacketHead));


	Send( szSendData, sizeof(stPacketHead), 0);
/*
	//////////////////////////////////////////////////////////////////////////
	// test

	fileTxtData.Open(_T("C:\\C扫数据记录.txt"), CFile::modeCreate | CFile::modeReadWrite, NULL);
	CString str;
	str.Format(_T("X\t\tY\t\tZ\t\tA\t\tB\t\tW\t\tR\t\tGate2MPTime\t\tGate2MPAmp\t\tGate2FPFirstTime\t\tGate2FPTime\t\tGate2FPAmp\t\tGate3MPTime\t\tGate3MPAmp\t\tGate3FPFirstTime\t\tGate3FPTime\t\tGate3FPAmp\t\tGate4MPTime\t\tGate4MPAmp\t\tGate4FPFirstTime\t\tGate4FPTime\t\tGate4FPAmp\r\n"));
	fileTxtData.WriteString(str);
	fileTxtData.Close();
	//////////////////////////////////////////////////////////////////////////
*/
	CAsyncSocket::OnConnect(nErrorCode);

	int nRecvBuf = 32 * 1024; //设置为32K
	setsockopt( m_hSocket, SOL_SOCKET, SO_RCVBUF, ( const char* )&nRecvBuf, sizeof( int ));

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CDataSocket::OnConnect"),"Leave");
}