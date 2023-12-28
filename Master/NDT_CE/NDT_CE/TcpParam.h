#pragma once

#include "Winsock2.h"

class CTcpParam
{
public:
	CTcpParam(int nPort = 5555, int nMax = 5);
	~CTcpParam(void);

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
