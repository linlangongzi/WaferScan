// ARSSocket.cpp : 实现文件
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
#include "ListCtrlEditable.h"
#include "TaskPanel.h"
#include "TaskDockView.h"
#include "WSocket.h"
#include "ARSSocket.h"
#include "DataSocket.h"
#include "FeatureInfoWnd.h"
#include "MainFrm.h"
#include "UltrasonicImmersedTestDoc.h"
#include "UltrasonicImmersedTestView.h"
#include "GlobalFunction.h"
#include "MovePathView.h"
#include "TcgCurveDlg.h"	// wangbingfu add at 2012-11-20

int CARSSocket::m_nNo = -1;
extern DWORD g_dwActiveChannel;
extern short g_nWaveSpeed; // wangbingfu add at 2012-11-20
// CARSSocket
CARSSocket::CARSSocket()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CARSSocket::CARSSocket"),"Enter");

	InitParam();
	m_count = 0;
	m_begin = 0;
	m_nNo++;
	m_nObjectNum = m_nNo;

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CARSSocket::CARSSocket"),"Leave");
}

CARSSocket::~CARSSocket()
{
}


// CARSSocket 成员函数
void CARSSocket::InitParam( void) 
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CARSSocket::InitParam"),"Enter");

	m_CmdLine = 0;
	CurRecLen = 0;
	SumRecLen = 0;
	memset( ReceivedData, 0, sizeof( ReceivedData));

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CARSSocket::InitParam"),"Leave");
}

void CARSSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CARSSocket::OnReceive"),"Enter");

	if( nErrorCode != 0) 
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CARSSocket::OnReceive"),"Leave");
		return;
	}

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	pFrame->m_nNetMonitorCount = 0;

	CurRecLen = Receive( &ReceivedData[SumRecLen], S_MONITORSIZE - SumRecLen);
	if( CurRecLen == SOCKET_ERROR || CurRecLen <=0) 
	{
		int nRet = GetLastError();			
		
		CString str;
		str.Format(_T("监视数据socket error = %d\n"), nRet);
		pFrame->m_wndStatusBar.SetPaneText(1, str);
		TRACE(_T("socket error = %d\n"), nRet);

		this->Close();
		this->InitParam();
		pFrame->TransAScanData();

		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CARSSocket::OnReceive"),"Leave");

		return;
	}

	SumRecLen += CurRecLen;
	
  	if( SumRecLen < S_MONITORSIZE) 
  	{
  	}
  	else 
 	{
 		PACKET_HEAD_t stPacketHead;
 		memset( &stPacketHead, 0, NETHEAD_SIZE);
 		memcpy( &stPacketHead, ReceivedData, NETHEAD_SIZE);
 		DWORD cmdLine = stPacketHead.dwCommand;
 		int dataSize = stPacketHead.dwSize;

		// Song Chenguang add start on 2013-12-21
		if (!pFrame->m_bMoinitorDataRun)
		{
			InitParam();
			CAsyncSocket::OnReceive(nErrorCode);
			return;
		}
		// Song Chenguang add end on 2013-12-21

		if (stPacketHead.dwCommand != CDM_ASCAN_RET_WAVE)
		{
			WORD* pHead = (WORD*)ReceivedData;
			int nCount = 0;

			while (*pHead != CDM_ASCAN_RET_WAVE)
			{
				nCount += sizeof(WORD);
				pHead++;
			}

			memcpy(ReceivedData, pHead, S_MONITORSIZE - nCount);
			SumRecLen -= nCount;

			CAsyncSocket::OnReceive(nErrorCode);

			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CARSSocket::OnReceive"),"Leave");
			return;
		}
		

		CUltrasonicImmersedTestView* pView = (CUltrasonicImmersedTestView*)pFrame->m_wndSplitter.GetPane(1,0);

		if(g_dwActiveChannel == (m_nObjectNum * 2 + stPacketHead.wChannelIndex))
		{
			CH_FLAWDATA_CSCAN_S * pData = new CH_FLAWDATA_CSCAN_S;
			CARD_INFO_t * pCardInfo = new CARD_INFO_t;
			// Song Chenguang add start on 2012-11-05
			if (pData == NULL)
			{
				AfxMessageBox(_T("CARSSocket::OnReceive new CH_FLAWDATA_CSCAN_S失败！"));
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CARSSocket::OnReceive"),"Leave");
				return;
			}

			if (pCardInfo == NULL)
			{
				AfxMessageBox(_T("CARSSocket::OnReceive new CARD_INFO_t失败！"));
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CARSSocket::OnReceive"),"Leave");
				return;
			}
			// Song Chenguang add end on 2012-11-05			
			
			pCardInfo->nCardIndex = m_nObjectNum;
			pCardInfo->nChannelIndex = stPacketHead.wChannelIndex;

			memcpy(pData, ReceivedData + NETHEAD_SIZE + sizeof(WORD)*DATA_SIZE, sizeof(CH_FLAWDATA_CSCAN_S));
			
			// wangbingfu add start at 2012-11-20
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			BOOL bTcgOnFlag = pFrame->m_pTcgCurveDlg->m_stTcgCurve[g_dwActiveChannel].bSwitchFlag;
			if(bTcgOnFlag)
			{
				if(pFrame->m_pTcgCurveDlg->m_nTcgOnMode == 0)
				{
					float fRange = 0;
					if(pData->wGate1MPTime > 0)
					{
						//Dot2Range改为Dot2RangeChannel  yachang.huang modify on 2014-6-5
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate1MPTime);
						pData->wGate1MPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(pData->wGate1MPAmp, fRange);
					}
					if(pData->wGate2MPTime > 0)
					{
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate2MPTime);
						pData->wGate2MPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(pData->wGate2MPAmp, fRange);
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate2FPTime);
						pData->wGate2FPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(pData->wGate2FPAmp, fRange);
					}
					if(pData->wGate3MPTime > 0)
					{
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate3MPTime);
						pData->wGate3MPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(pData->wGate3MPAmp, fRange);
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate3FPTime);
						pData->wGate3FPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(pData->wGate3FPAmp, fRange);
					}
					if(pData->wGate4MPTime > 0)
					{
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate4MPTime);
						pData->wGate4MPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(pData->wGate4MPAmp, fRange);
						fRange = pFrame->m_wndChannelInfo.m_pFormView->Dot2RangeChannel(g_nWaveSpeed, pData->wGate4FPTime);
						pData->wGate4FPAmp = pFrame->m_pTcgCurveDlg->CalcNewAmp(pData->wGate4FPAmp, fRange);
					}
				}
			}
			// wangbingfu add end at 2012-11-20
			double dX = 0.0f;
			double dY = 0.0f;
			double dZ = 0.0f;
			double dA = 0.0f;
			double dB = 0.0f;
			double dW = 0.0f;
			double dR = 0.0f;
			int nAxisIndex = pFrame->m_wndMechanicalControl.m_pMechanicalControlView->m_pMovePathView->m_GraphMove.GetAxisSelect();
			GetCScanDataCoordinate(*pData, dX, dY, dZ, dR, dB, dW, dA, 0, nAxisIndex, m_nObjectNum, stPacketHead.wChannelIndex);
			pFrame->m_pFeatureWnd->PostMessage(WM_FEATURE_INFO, (WPARAM)pData, (LPARAM)pCardInfo);

			CPicData* pPicData = new CPicData;
			// Song Chenguang add start on 2012-11-05
			if (pPicData == NULL)
			{
				AfxMessageBox(_T("CARSSocket::OnReceive new CPicData失败！"));

				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CARSSocket::OnReceive"),"Leave");

				return;
			}
			// Song Chenguang add end on 2012-11-05
			
			memcpy(pPicData->m_wData, ReceivedData + NETHEAD_SIZE, sizeof(WORD)*DATA_SIZE);
			pPicData->m_fX = dX;
			pPicData->m_fY = dY;
			pPicData->m_fZ = dZ;
			// machao modify start on 2013-11-26
			pPicData->m_fA = dA;
			pPicData->m_fB = dB;
			pPicData->m_fW = dW;
			pPicData->m_fR = dR;
			// machao modify end on 2013-11-26
			// Song Chenguang add start on 2013-11-26
			CH_FLAWDATA_CSCAN_S * pData3 = new CH_FLAWDATA_CSCAN_S;
	//		CARD_INFO_t * pCardInfo3 = new CARD_INFO_t;	// 内存泄漏 wangbingfu delete on 2014-10-15

			if (pData3 == NULL)
			{
				AfxMessageBox(_T("CARSSocket::OnReceive new pData2失败！"));
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CARSSocket::OnReceive"),"Leave");
				return;
			}
			
			// 内存泄漏 wangbingfu delete start on 2014-10-15
	/*		if (pCardInfo3 == NULL)
			{
				AfxMessageBox(_T("CARSSocket::OnReceive new pCardInfo2失败！"));
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CARSSocket::OnReceive"),"Leave");
				return;
			}

			pCardInfo3->nCardIndex = m_nObjectNum;
			pCardInfo3->nChannelIndex = stPacketHead.wChannelIndex;*/
			// 内存泄漏 wangbingfu delete end on 2014-10-15

			memcpy(pData3, ReceivedData + NETHEAD_SIZE + sizeof(WORD)*DATA_SIZE, sizeof(CH_FLAWDATA_CSCAN_S));
			// Song Chenguang add end on 2013-11-26
			
			pView->PostMessage(WM_A_SCAN_DATA, (WPARAM)pPicData, (LPARAM)pData3);	// Song Chenguang modify on 2013-11-26

			CH_FLAWDATA_CSCAN_S * pData2 = new CH_FLAWDATA_CSCAN_S;
			CARD_INFO_t * pCardInfo2 = new CARD_INFO_t;

			// Song Chenguang add start on 2012-11-05
			if (pData2 == NULL)
			{
				AfxMessageBox(_T("CARSSocket::OnReceive new pData2失败！"));
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CARSSocket::OnReceive"),"Leave");
				return;
			}

			if (pCardInfo2 == NULL)
			{
				AfxMessageBox(_T("CARSSocket::OnReceive new pCardInfo2失败！"));
				LOG4CXX_ERROR(log4cxx::Logger::getLogger("CARSSocket::OnReceive"),"Leave");
				return;
			}
			// Song Chenguang add end on 2012-11-05

			pCardInfo2->nCardIndex = m_nObjectNum;
			pCardInfo2->nChannelIndex = stPacketHead.wChannelIndex;

			memcpy(pData2, ReceivedData + NETHEAD_SIZE + sizeof(WORD)*DATA_SIZE, sizeof(CH_FLAWDATA_CSCAN_S));
			pFrame->m_wndMechanicalControl.m_pMechanicalControlView->PostMessage(WM_FEATURE_INFO, (WPARAM)pData2, (LPARAM)pCardInfo2);
		}

		InitParam();
#ifdef _DEBUG
		//帧test
		m_count++;
		if(m_count == 60)
		{
			int nCurrentTime = GetTickCount();
			int nFrame = 60000.0f / float(nCurrentTime - m_begin);
			m_begin = nCurrentTime;
			m_count = 0;

			CString strFrame;
			strFrame.Format(_T("%d"),nFrame);
			CDC *pDC = pView->GetDC();
			pDC->SetTextColor(RGB(0, 0, 0));
			pDC->TextOut(0, 0, strFrame);
			pView->ReleaseDC(pDC);
		}
#endif
	}

	CAsyncSocket::OnReceive(nErrorCode);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CARSSocket::OnReceive"),"Leave");
}

void CARSSocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CARSSocket::OnConnect"),"Enter");

	char szSendData[NETHEAD_SIZE];
	memset( szSendData, 0, sizeof(szSendData));

	PACKET_HEAD_t stPacketHead;

	stPacketHead.dwCommand = CMD_ASCAN;
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

	CAsyncSocket::OnConnect(nErrorCode);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CARSSocket::OnConnect"),"Leave");
}
