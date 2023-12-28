#include "StdAfx.h"
#include "TcpServer.h"
#include "ars200fpga.h"
#include "NDT_CE.h"
#include <vector> 

using namespace std;

SOCKET CTcpServer::m_sockListen = NULL;
//int CTcpServer::m_nPort = NULL;
//extern SOCKET gSockClient[MAX_SOCKETS + 1];
extern vector<SOCKET> gVecSockMonitor;
extern HWND hStatusBar;

CTcpServer::CTcpServer(int nPort, int nMax):
	m_nPort(nPort),
	m_nMaxConnect(nMax)
{
	CreateListenSocket();
	::CreateThread(NULL, 0, ListenProc, (LPVOID)this, 0, NULL);
}

CTcpServer::~CTcpServer(void)
{
}

int CTcpServer::CreateListenSocket()
{
	WSADATA wsd;
	if(WSAStartup( MAKEWORD( 2, 2 ), &wsd ) != 0 )// 支持Windows Sockets 2.2版本
	{
		return 0;	
	}
/*
	SOCKET sockList[MAX_SOCKETS + 1];

	for (int i=0; i<(MAX_SOCKETS  + 1); i++)
	{
		sockList[i] = INVALID_SOCKET;
	}
*/
	m_sockListen = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );	
	if( INVALID_SOCKET == m_sockListen )
	{
		return 0;
	}


	sockaddr_in addr;
	addr.sin_family=AF_INET;
	addr.sin_port= htons( m_nPort ); // 保证字节顺序
	addr.sin_addr.s_addr= htonl( INADDR_ANY );


	int nResult = bind( m_sockListen, (sockaddr*)&addr, sizeof(sockaddr) );
	if ( nResult == SOCKET_ERROR )
	{
		return 0;
	}

	nResult = listen( m_sockListen, m_nMaxConnect ); // 最多 MAX_BACKLOG 个 Pending 连接
	if ( nResult == SOCKET_ERROR )
	{
		return 0;
	}

	return 1;
}

SOCKET CTcpServer::GetListenSocket()
{
	return m_sockListen;
}


DWORD WINAPI CTcpServer::ListenProc(LPVOID lpParameter)
//DWORD WINAPI ListenProc(LPVOID lpParameter)
{
	//CTcpServer* pTcpServer = (CTcpServer*)lpParameter;
	while(TRUE)
	{
		SOCKET sockClient;
		//if((sockClient = accept(pTcpServer->GetListenSocket(), NULL, NULL)) == INVALID_SOCKET )
		if((sockClient = accept(m_sockListen, NULL, NULL)) == INVALID_SOCKET )
		{
			break;
		}
		
		int nSocketTotal = gVecSockMonitor.size();
		SOCKET_PROC_PARAM_t stSocketParam;
		stSocketParam.sockClient = sockClient;
		stSocketParam.nIndex = nSocketTotal;
		::CreateThread(NULL, 0, SocketThreadProc, (LPVOID)&stSocketParam, 0, NULL);
		gVecSockMonitor.push_back(sockClient);
		//gSockClient[nSocketTotal] = sockClient;
		wchar_t strText[20];
		wsprintf(strText, _T("accept%d"), nSocketTotal);
		PostMessage(hStatusBar, SB_SETTEXT, 
			(WPARAM) (5 | (SBT_OWNERDRAW| SBT_NOBORDERS) ), (LPARAM)(LPWSTR)strText);
	}

	return 0;
}