#pragma once

// CDataSocket command target
//struct CH_FLAWDATA_CSCAN;
const WORD DATA_FEATURE_RET_LEN = sizeof(CH_FLAWDATA_CSCAN_S) * 128;
const int FEATURE_DATA_SIZE = NETHEAD_SIZE+DATA_FEATURE_RET_LEN;

class CDataSocket : public CAsyncSocket
{
public:
	CDataSocket();
	virtual ~CDataSocket();
public:
	virtual void OnConnect(int nErrorCode);
public:
	virtual void OnReceive(int nErrorCode);
	void InitParam( void);
	WORD m_CmdLine;
public:
	int CurRecLen,SumRecLen;
	
	char ReceivedData[FEATURE_DATA_SIZE];

	int m_count;
	int m_begin;
public:
	static int m_nNo;
	int m_nObjectNum;

	// test
	CStdioFile fileTxtData;
	//////////////////////////////////////////////////////////////////////////

};


