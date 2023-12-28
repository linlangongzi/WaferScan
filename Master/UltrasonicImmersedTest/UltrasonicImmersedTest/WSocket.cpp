//////////////////////////////////////////////////////////////////////
//WYD's Socket
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WSocket.h"
#include <TIME.H>
#include <stdio.h> 
#include "UltrasonicImmersedTest.h"
#include "GlobalFunction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWSocket::CWSocket()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::CWSocket"),"Enter");
	WSADATA wsaData;

	m_sSocket = NULL;
	m_bConnected = FALSE;
	m_sServer = NULL;
	m_bListened = FALSE;
	m_nServerPort = -1;
	m_hServerThread = NULL;

	m_bNeedClose = FALSE;
	m_bNeedCloseServer = FALSE;
	
	if(WSAStartup(0x202, &wsaData) == 0)
	{
		m_bWSAStartup = TRUE;
		err = 0;
	}
	else
	{
		m_bWSAStartup = FALSE;
		err = WSAGetLastError();
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::CWSocket"),"Leave");
}

CWSocket::CWSocket(SOCKET s)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::CWSocket(SOCKET s)"),"Enter");
	m_sSocket = NULL;
	m_bWSAStartup = FALSE;
	m_sServer = NULL;
	m_bListened = FALSE;
	m_nServerPort = -1;
	m_hServerThread = NULL;

	m_bNeedClose = FALSE;
	m_bNeedCloseServer = FALSE;
	
	m_bConnected = IsSockConnected(s);
	if(m_bConnected) m_sSocket = s;

	if(!m_bConnected)
	{
		WSADATA wsaData;
		if(WSAStartup(0x202, &wsaData) == 0)
		{
			m_bWSAStartup = TRUE;
			err = 0;
		}
		else
		{
			m_bWSAStartup = FALSE;
			err = WSAGetLastError();
		}
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::CWSocket(SOCKET s)"),"Leave");
}

CWSocket& CWSocket::operator=(SOCKET s)
{	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::operator="),"Enter");
	if(IsSockConnected(s))
	{
		if(m_bConnected) Close();
		m_sSocket = s;
		m_bConnected = TRUE;
		m_bNeedClose = FALSE;
	}
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::operator="),"Leave");
	return *this;
}

CWSocket::~CWSocket()
{	
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::~CWSocket"),"Enter");
	if(m_bNeedClose) Close();
	if(m_bNeedCloseServer) CloseServer();
	if(m_bWSAStartup) WSACleanup();
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::~CWSocket"),"Leave");
}

BOOL CWSocket::Connection(LPCSTR pstrHost, int nPort)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::Connection"),"Enter");
	if(m_bConnected) Close();

	LPHOSTENT lpHost;	
	struct sockaddr_in server;	

	//Lookup host
	lpHost = gethostbyname(pstrHost);
	if(lpHost == NULL) 
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::Connection"),"Leave");
		return FALSE;
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = *((u_long FAR *)(lpHost->h_addr));
	server.sin_port=htons(nPort);

	m_sSocket = socket(AF_INET, SOCK_STREAM, 0);

	if(m_sSocket <= 0)
	{
		err = WSAGetLastError();
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::Connection"),"Leave");
		return FALSE;
	}

	if(connect(m_sSocket, (LPSOCKADDR)&server, sizeof(SOCKADDR)) == SOCKET_ERROR) 
	{
		err = WSAGetLastError();
		closesocket(m_sSocket);
		m_sSocket = NULL;
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::Connection"),"Leave");
		return FALSE;
	}
	
	m_bNeedClose = TRUE;
	m_bConnected = TRUE;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::Connection"),"Leave");
	return TRUE;
}

void CWSocket::Close()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::Close"),"Enter");
	if(!m_bConnected) 
	{
	    LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::Close"),"Leave");
		return;
	}
	if(m_sSocket == NULL)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::Close"),"Leave");
		return;
	}

	shutdown(m_sSocket, SD_RECEIVE);
	Sleep(50);
	closesocket(m_sSocket);
	m_sSocket = NULL;
	m_bConnected = FALSE;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::Close"),"Leave");
}

void CWSocket::CloseServer()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::CloseServer"),"Enter");
	if(!m_bListened) 
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::CloseServer"),"Leave");
		return;
	}
	if(m_sServer == NULL)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::CloseServer"),"Leave");
		return;
	}

	shutdown(m_sServer, SD_RECEIVE);
	Sleep(50);
	closesocket(m_sServer);
	m_sServer = NULL;
	m_bListened = FALSE;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::CloseServer"),"Leave");
}


DWORD CALLBACK CWSocket::TimeoutControl(LPVOID lpParm)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::TimeoutControl"),"Enter");
	TPARA* para = (TPARA*)lpParm;
	time_t stime = time(NULL);
	BOOL bTimeover = FALSE;

	while(!bTimeover)
	{
		if(para->bExit)
		{
			para->IsExit = TRUE;
			//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::TimeoutControl"),"Leave");
			return 0;
		}

		Sleep(1);

		time_t ntime = time(NULL);
		if((ntime - stime) > para->OutTime) bTimeover = TRUE;
	}

	if(para->bExit)
	{
		para->IsExit = TRUE;
		//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::TimeoutControl"),"Leave");
		return 0;
	}
	
	if(para->s != NULL)
	{
		para->pbConnected[0] = FALSE;
		shutdown(para->s, SD_RECEIVE);
		Sleep(5);
		closesocket(para->s);
	}

	para->IsExit = TRUE;
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::TimeoutControl"),"Leave");
	return 0;
}

int CWSocket::ReadData(char FAR* buf, int len, int timeout)
{
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::ReadData"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (buf == NULL)
	{
		AfxMessageBox(_T("CWSocket::ReadData buf"), MB_OK|MB_ICONERROR, -1);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::ReadData"),"Leave");
		return -1;
	}
	// yachang.huang  add end on 2012-11-02

	if(!m_bConnected || m_sSocket == NULL)
	{
		err = -1; 
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::ReadData"),"Leave");
		return -1;
	}

	HANDLE hThread;
	DWORD dwThreadId;
	TPARA para;

	para.OutTime = timeout;
	para.s = m_sSocket;
	para.bExit = FALSE;
	para.IsExit = FALSE;
	para.pbConnected = &m_bConnected;
	hThread = CreateThread(NULL, NULL, TimeoutControl, (LPVOID)(&para), 0, &dwThreadId);
	if (hThread == NULL) 
	{
        LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::ReadData"),"Leave");
		return -1;
	}

	int nRet = recv(m_sSocket, buf, len, 0);
	if(nRet == SOCKET_ERROR) 
	{
		err = WSAGetLastError();
		ErroWindowMessage();
	}
	

	para.bExit = TRUE;
	while(!para.IsExit) Sleep(1);
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::ReadData"),"Leave");
	return nRet;
}

int CWSocket::SendData(const char FAR *buf, int len, int timeout)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::SendData"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (buf == NULL)
	{
	    AfxMessageBox(_T("CWSocket::SendData buf"), MB_OK|MB_ICONERROR, -1);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::SendData"),"Leave");
		return -1;
	}
	// yachang.huang  add end on 2012-11-02
	if(!m_bConnected || m_sSocket == NULL)
	{
		err = -1;
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::SendData"),"Leave");
		return -1;
	}

	if(!IsSockConnected(m_sSocket))
	{
		m_bConnected = FALSE;
		err = -1;
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::SendData"),"Leave");
		return -1;
	}

	int nRet = send(m_sSocket, buf, len, 0);

	if(nRet == SOCKET_ERROR) err = WSAGetLastError();
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::SendData"),"Leave");
	return nRet;
}

BOOL CWSocket::IsSockConnected(SOCKET s)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::IsSockConnected"),"Enter");
	int nRet = 0;
	struct fd_set Fd_Recv;
    struct timeval Time_Recv;

	memset(&Fd_Recv, 0, sizeof(struct fd_set));
    FD_CLR(s, &Fd_Recv); 
    FD_SET(s, &Fd_Recv); 
    Time_Recv.tv_sec = 0;
    Time_Recv.tv_usec = 0;

	nRet = select(s, &Fd_Recv, NULL, NULL, &Time_Recv);
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::IsSockConnected"),"Leave");
    return (nRet == 0);
}

BOOL CWSocket::CreateServer(int port, int backlog)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::CreateServer"),"Enter");
	if(m_bListened == TRUE) CloseServer();

	struct	sockaddr_in local;

	//创建监听socket
    m_sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if(m_sServer == SOCKET_ERROR)
    {
        err = WSAGetLastError();
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::CreateServer"),"Leave");
        return FALSE;
    }

	//配置Server-TAX参数	
	local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_family = AF_INET;
    local.sin_port = htons(port);

	if(bind(m_sServer, (struct sockaddr *)&local, sizeof(local)) == SOCKET_ERROR)
    {
		err = WSAGetLastError();
		closesocket(m_sServer);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::CreateServer"),"Leave");
        return FALSE;
    }

	//开始侦听
    if(listen(m_sServer, backlog) != 0)
	{
		err = WSAGetLastError();
		closesocket(m_sServer);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::CreateServer"),"Leave");
        return FALSE;
    }

	m_nServerPort = port;
	m_bListened = TRUE;
	m_bNeedCloseServer = TRUE;
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::CreateServer"),"Leave");
	return TRUE;
}

SOCKET CWSocket::Listen(char *ClientIP)
{
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::Listen"),"Enter");
	// yachang.huang  add start on 2012-11-02
	if (ClientIP == NULL)
	{
	    AfxMessageBox(_T("CWSocket::Listen ClientIP"), MB_OK|MB_ICONERROR, -1);
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::Listen"),"Leave");
		return -1;
	}
	// yachang.huang  add end on 2012-11-02

	if(!m_bListened) 
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::Listen"),"Leave");
		return -1;
	}
	if(m_sServer == NULL) 
	{	
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::Listen"),"Leave");
		return -1;
	}

	SOCKET	sClient;	
	int		iAddrSize;
	struct	sockaddr_in addr;

	iAddrSize = sizeof(addr);

	sClient = accept(m_sServer, (struct sockaddr *)&addr, &iAddrSize);
	
	if(sClient == SOCKET_ERROR)
	{
		err = WSAGetLastError();
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::Listen"),"Leave");
        return SOCKET_ERROR;
    }

	if(ClientIP != NULL)
	{
		sprintf(ClientIP, "%d.%d.%d.%d", addr.sin_addr.S_un.S_un_b.s_b1, 
										 addr.sin_addr.S_un.S_un_b.s_b2,
										 addr.sin_addr.S_un.S_un_b.s_b3,
										 addr.sin_addr.S_un.S_un_b.s_b4);
	}
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::Listen"),"Leave");
	return sClient;
}

BOOL CWSocket::StartServer(DEALPROC lpDealFunc)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::StartServer"),"Enter");
	if(!m_bListened) 
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::StartServer"),"Leave");
		return -1;
	}
	if(m_sServer == NULL) 
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::StartServer"),"Leave");
		return -1;
	}

	DWORD dwThreadId;
	SERVERPARA *para;

	para = new SERVERPARA;
	// WangQianfeng add start on 2012-11-06
	if ( NULL == para )
	{
		AfxMessageBox(_T(" CWSocket::StartServer,para = null"));
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::StartServer"),"Leave");
		return FALSE;
	}
	// WangQianfeng add end on 2012-11-06
	para->s = m_sServer;
	para->port = m_nServerPort;
	para->lpDealFunc = lpDealFunc;

	m_hServerThread = CreateThread(NULL, 0, ServerProc, (LPVOID)(para), 0, &dwThreadId);

	if(m_hServerThread == NULL)
	{
		delete para;
		err = WSAGetLastError();
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::StartServer"),"Leave");
        return FALSE;
	}
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::StartServer"),"Leave");
	return TRUE;
}

BOOL CWSocket::StopServer()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::StopServer"),"Enter");
	CloseServer();
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::StopServer"),"Leave");
	return TRUE;
}

DWORD CALLBACK CWSocket::ServerProc(LPVOID lpParm)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::ServerProc"),"Enter");
	SERVERPARA *para = (SERVERPARA*)lpParm;
	
	if(para == NULL) 
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::ServerProc"),"Leave");
		return -1;
	}

	SOCKET		s = para->s;
	int			port = para->port;
	DEALPROC	lpDealFunc = para->lpDealFunc;
	delete para;

	SOCKET	sClient;	
	int		iAddrSize;
	struct	sockaddr_in addr;
	char	IP[32];
	HANDLE	hThread;
    DWORD	dwThreadId;
	DEALPARA *parac;

	iAddrSize = sizeof(addr);

	while(1)
	{
		sClient = accept(s, (struct sockaddr *)&addr, &iAddrSize);
		
		if(sClient == SOCKET_ERROR) break;

		sprintf(IP, "%d.%d.%d.%d", addr.sin_addr.S_un.S_un_b.s_b1, 
									   addr.sin_addr.S_un.S_un_b.s_b2,
									   addr.sin_addr.S_un.S_un_b.s_b3,
									   addr.sin_addr.S_un.S_un_b.s_b4);

		parac = new DEALPARA;
		// WangQianfeng add start on 2012-11-06
		if ( NULL == parac )
		{
			AfxMessageBox(_T("CWSocket::ServerProc,parac = null"));
			LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::ServerProc"),"Leave");
			return NULL;
		}
		// WangQianfeng add end on 2012-11-06
		memset(parac->IP, 0, sizeof(parac->IP));
		parac->s = sClient;
		parac->port = port;
		parac->lpDealFunc = lpDealFunc;
		memcpy(parac->IP, IP, strlen(IP));	

		//侦听到连接，开一个线程		
        hThread = CreateThread(NULL, 0, DealProc, (LPVOID)(parac), 0, &dwThreadId);

		if(hThread == NULL) delete parac;
	}
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::ServerProc"),"Leave");
	return 0;
}

DWORD CALLBACK CWSocket::DealProc(LPVOID lpParm)
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::DealProc"),"Enter");
	DEALPARA *para = (DEALPARA*)lpParm;
	
	if(para == NULL) 
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::DealProc"),"Leave");
		return -1;
	}

	SOCKET		s = para->s;
	int			port = para->port;
	DEALPROC	lpDealFunc = para->lpDealFunc;
	char		IP[32];
	memcpy(IP, para->IP, sizeof(IP));
	delete para;

	try{lpDealFunc(s, port, IP);}
	catch(...)
	{
		LOG4CXX_ERROR(log4cxx::Logger::getLogger("CWSocket::DealProc"),"Leave");
		return -1;
	}
    //LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::DealProc"),"Leave");
	return 0;
}

SOCKET CWSocket::GetSocket()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::GetSocket"),"Enter");
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::GetSocket"),"Leave");
	return m_sSocket;

}

SOCKET CWSocket::GetServerSocket()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::GetServerSocket"),"Enter");
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::GetServerSocket"),"Leave");
	return m_sServer;
}

int CWSocket::GetError()
{
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::GetError"),"Enter");
	//LOG4CXX_INFO(log4cxx::Logger::getLogger("CWSocket::GetError"),"Leave");
	return err;
}