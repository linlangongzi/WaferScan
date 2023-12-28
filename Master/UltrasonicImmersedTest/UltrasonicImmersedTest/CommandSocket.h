#pragma once

// CCommandSocket command target

class CCommandSocket : public CAsyncSocket
{
public:
	CCommandSocket();
	virtual ~CCommandSocket();
	void OnConnect(int nErrorCode);
	void OnReceive(int nErrorCode);
};


