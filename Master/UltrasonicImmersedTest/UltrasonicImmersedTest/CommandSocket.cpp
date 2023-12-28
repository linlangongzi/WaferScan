// CommandSocket.cpp : implementation file
//

#include "stdafx.h"
#include "UltrasonicImmersedTest.h"
#include "CommandSocket.h"
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


// CCommandSocket

CCommandSocket::CCommandSocket()
{
}

CCommandSocket::~CCommandSocket()
{
}


// CCommandSocket member functions
void CCommandSocket::OnConnect(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CARSSocket::OnConnect"),"Enter");
/*
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
*/
	CAsyncSocket::OnConnect(nErrorCode);

	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CARSSocket::OnConnect"),"Leave");
}

void CCommandSocket::OnReceive(int nErrorCode)
{

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (	pFrame->m_bPlateStart[0] )
	{
		char tempBuf[MAX_BUF_LEN];
		memset( tempBuf, 0, MAX_BUF_LEN);
		if(Receive( tempBuf,MAX_BUF_LEN, 100)==SOCKET_ERROR) 
		{

		}


		//取包头，看是否返回异常
		PACKET_HEAD_t stPacketHead;
		memset( &stPacketHead, 0, sizeof( stPacketHead));
		memcpy( &stPacketHead, tempBuf, NETHEAD_SIZE);
		DWORD cmdLine = stPacketHead.dwCommand;

		if( cmdLine == COMMAND_VERSION_REQ) 
		{
			memcpy(&pFrame->m_stYiQiVer[0], tempBuf+sizeof(PACKET_HEAD_t), sizeof(CSCAN_VERSION_DATA_t));
		}
// wangbingfu modify stat on 2014-10-14	 Receive(NULL, 0); CPU占用率高很多
		CAsyncSocket::OnReceive(nErrorCode);
	}
/*	else
	{
	     Receive(NULL, 0);
	}*/
	
//	CAsyncSocket::OnReceive(nErrorCode);
	// wangbingfu modify end on 2014-10-14
}