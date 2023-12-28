#pragma once

#include "Winsock2.h"

class CTcpMonitor
{
public:
	CTcpMonitor(int nPort = 3333, int nMax = 5);
	~CTcpMonitor(void);

private:
	int m_nPort;
	int m_nMaxConnect;
	static SOCKET m_sockListen;

private:
	int CreateListenSocket();
	static DWORD WINAPI ListenProc(LPVOID lpParameter);

public:
	SOCKET GetListenSocket();
};
