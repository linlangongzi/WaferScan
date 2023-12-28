#ifndef _PLX_COMMUNICATION_H_
#define _ PLX_COMMUNICATION_H_

#pragma once

enum BusSlotOrder {ZBusZSlot = 0, ZBusNSlot, NBusZSlot, NBusNSlot};

class CPLXCommunication
{
public:
	CPLXCommunication(void);
public:
	~CPLXCommunication(void);
public:
	BOOL Init(AECTHREAD_PARAM* pstThreadParam);
	DWORD UnInit(AECTHREAD_PARAM* pstThreadParam);
	// ���HPI�ж�
	void ResetHPI_INT(P9054_HANDLE hDev);
	// �趨���ӷ�Χ
	void SetMonitorRange(P9054_HANDLE hDev, WORD *pInterSpace);
	// ��ȡ��ͼ����
	void PutsReadDrawData(P9054_HANDLE hDev, CPicData *pData);

	void ConfigDSP(P9054_HANDLE hDev, CH_CONFIG *pallCH_CONFIG, CH_CONFIG_GLOBAL *pch_config_global, int icardno);
//	void ConfigDSP_DAC(P9054_HANDLE hDev);
	void InterruptDSP(P9054_HANDLE hDev, int nCardNo);
	void WaveDelay(P9054_HANDLE hDev, WORD nDelay);
	// ��ʱ
//	static void Delay(DWORD dwUs);
	//void SetMonitorRange(WDC_DEVICE_HANDLE hDev, WORD *pInterSpace);//���÷�Χ
public:
	inline WORD GetAECCardNum(){ return m_wAECCardNum;}
private:
	WD_PCI_SLOT m_BusSlotIndex[MAX_CARD_NO];
	WDC_PCI_SCAN_RESULT m_scanResult;
	WORD m_wTotalCardNum;
	WORD m_wMFCCardNum;
public:
	WORD m_wAECCardNum;
public:
	CString m_strVersion;
private:
	// ���Լ죨�๦�ܿ��͹�������
	BOOL AllCardsSelfTest(AECTHREAD_PARAM* pstThreadParam);
	// AEC���Լ�
	BOOL CardSelfTest(P9054_HANDLE hDev);
	// ����LED��
	BOOL AEC_OperationLamp(P9054_HANDLE hDev, WORD nValue);
	
	// �๦�ܿ��Լ�
	BOOL MultiCardTest(P9054_HANDLE hDev);
	// �๦�ܿ���ʼ��
	BOOL MultiCardInit(P9054_HANDLE hDev);
	// ������÷���������
	BOOL BusSlotFindAndSort(AECTHREAD_PARAM* pstThreadParam);
	// ��ѯ��Bus��Slot��
	int FindCardBusSlot(WDC_PCI_SCAN_RESULT *pscanResult, WD_PCI_SLOT *pBusSlotIndex);
	// Slot����
	void SortSlot(WD_PCI_SLOT *pBusSlotIndex,int size);
	// ��ȡSlot�����־
	int GetSlotFlag();
	// ʹ��HPI��ʽд����
	void WriteDataHPI(P9054_HANDLE hDev, DWORD dwAddr, void *pdata, int size);

};


#endif