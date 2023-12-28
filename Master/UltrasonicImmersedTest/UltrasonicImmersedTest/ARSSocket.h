#pragma once

// CARSSocket ÃüÁîÄ¿±ê
const int SIZE1 = sizeof(CH_FLAWDATA_CSCAN_S) * 128;	// 0x1
const int SIZE2 = sizeof(CSCAN_CH_SEND_MONITOR_DATA_t);	// 0x10
const int SIZE3 = sizeof(CH_FLAWDATA_CSCAN_S) * 128;	// 0x100
const int SIZE4 = sizeof(CSCAN_CH_SEND_MONITOR_DATA_t);	// 0x1000
const int SIZE5 = sizeof(CH_FLAWDATA_CSCAN_S) * 128 + sizeof(CSCAN_CH_SEND_MONITOR_DATA_t);	// 0x11
const int SIZE6 = sizeof(CH_FLAWDATA_CSCAN_S) * 256;	// 0x101
const int SIZE7 = sizeof(CH_FLAWDATA_CSCAN_S) * 128 + sizeof(CSCAN_CH_SEND_MONITOR_DATA_t);	// 0x110
const int SIZE8 = sizeof(CH_FLAWDATA_CSCAN_S) * 256 + sizeof(CSCAN_CH_SEND_MONITOR_DATA_t);	// 0x111
const int SIZE9 = sizeof(CH_FLAWDATA_CSCAN_S) * 128 + sizeof(CSCAN_CH_SEND_MONITOR_DATA_t);	// 0x1001
const int SIZE10 = sizeof(CSCAN_CH_SEND_MONITOR_DATA_t) * 2;	// 0x1010
const int SIZE11 = sizeof(CH_FLAWDATA_CSCAN_S) * 128 + sizeof(CSCAN_CH_SEND_MONITOR_DATA_t) * 2;	// 0x1011
const int SIZE12 = sizeof(CH_FLAWDATA_CSCAN_S) * 128 + sizeof(CSCAN_CH_SEND_MONITOR_DATA_t);	// 0x1100
const int SIZE13 = sizeof(CH_FLAWDATA_CSCAN_S) * 256 + sizeof(CSCAN_CH_SEND_MONITOR_DATA_t);	// 0x1101
const int SIZE14 = sizeof(CH_FLAWDATA_CSCAN_S) * 128 + sizeof(CSCAN_CH_SEND_MONITOR_DATA_t) * 2;	// 0x1110
const int SIZE15 = sizeof(CH_FLAWDATA_CSCAN_S) * 256 + sizeof(CSCAN_CH_SEND_MONITOR_DATA_t) * 2;	// 0x1111
const int S_MONITORSIZE = 12 + sizeof(CSCAN_CH_SEND_MONITOR_DATA_t);

class CARSSocket : public CAsyncSocket
{
public:
	CARSSocket();
	~CARSSocket();
	void OnReceive(int nErrorCode);
	void OnConnect(int nErrorCode);
	void InitParam( void);
	WORD m_CmdLine;
public:
	int CurRecLen;
	int SumRecLen;
	char ReceivedData[S_MONITORSIZE];
	int m_count;
	int m_begin;
public:
	static int m_nNo;
	int m_nObjectNum;
};


