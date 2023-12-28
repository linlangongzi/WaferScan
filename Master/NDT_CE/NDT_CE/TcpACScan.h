#pragma once

#include "Winsock2.h"

class CTcpACScan
{
public:
	CTcpACScan(int nPort = 2222, int nMax = 5);
	~CTcpACScan(void);

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
