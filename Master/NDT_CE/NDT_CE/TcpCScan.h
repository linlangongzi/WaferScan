#pragma once

#include "Winsock2.h"

class CTcpCScan
{
public:
	CTcpCScan(int nPort = 4444, int nMax = 5);
	~CTcpCScan(void);

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
