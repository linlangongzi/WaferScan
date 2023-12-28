#pragma once

// CAScanDataSocket command target
const int A_SCAN_PACKAGE_SIZE = sizeof(PACKET_HEAD_t) + sizeof(CSCAN_CH_SEND_MONITOR_DATA_t)*30;

class CAScanDataSocket : public CAsyncSocket
{
public:
	CAScanDataSocket();
	virtual ~CAScanDataSocket();
public:
	int CurRecLen;	// 当前接收大小
	int SumRecLen;	// 接收总大小
	char ReceivedData[A_SCAN_PACKAGE_SIZE];
public:
	static int m_nNo;
	int m_nObjectNum;
public:
	virtual void OnReceive(int nErrorCode);
	void InitParam();
public:
	virtual void OnConnect(int nErrorCode);

	// test
	CStdioFile fileTxtData;
	//////////////////////////////////////////////////////////////////////////

};


