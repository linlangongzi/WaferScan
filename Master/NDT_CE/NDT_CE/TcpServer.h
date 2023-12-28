#pragma once
#include "Winsock2.h"

class CTcpServer
{
public:
	CTcpServer(int nPort, int nMax);
	~CTcpServer(void);

private:
	int m_nPort;
	int m_nMaxConnect;
//	SOCKET m_sockListen;
	static SOCKET m_sockListen;

private:
	int CreateListenSocket();
	static DWORD WINAPI ListenProc(LPVOID lpParameter);

public:
	SOCKET GetListenSocket();
};

//DWORD WINAPI ListenProc(LPVOID lpParameter);
