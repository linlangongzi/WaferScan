﻿#ifndef _PLX_COMMUNICATION_H_
#define _PLX_COMMUNICATION_H_
#include "plx_lib_x.h"	// plx_dll.dll PCI卡操作动态链接库
#include "p9054_lib.h"
#include "CardDataDefine.h"

enum BusSlotOrder {ZBusZSlot = 0, ZBusNSlot, NBusZSlot, NBusNSlot};

class CPLXCommunication
{
public:
    CPLXCommunication(void);
public:
    ~CPLXCommunication(void);
public:
    enum E_InitResult
    {
        SUCCESS = 1,
        ERROR_INIT,         // 初始化失败
        ERROR_FIND,         // 找不到卡
        ERROR_SELTEST,      // 自检失败不对
        ERROR_AEC_CARD_NUM, // 卡数量不对
        ERROR_PXI_VERSION   // 下位机版本
    };
public:
    E_InitResult Init(SharedCardParamVec pstThreadParam, int nInsertAecNum);
    DWORD UnInit(SharedCardParamVec pstThreadParam);
    // 清除HPI中断
    void ResetHPI_INT(P9054_HANDLE hDev);
    // 设定监视范围
    void SetMonitorRange(P9054_HANDLE hDev, WORD *pInterSpace, quint32 unAddr);
    // 读取画图数据
    //BOOL PutsReadDrawData(P9054_HANDLE hDev, unsigned short *pData);
    bool ReadMonitorData(P9054_HANDLE hDev, unsigned short *pData, quint32 unEnAddr, quint32 unAddr, quint32 bufferIndex = 0);

//	void ConfigDSP(P9054_HANDLE hDev, CH_CONFIG *pallCH_CONFIG, CH_CONFIG_GLOBAL *pch_config_global, int icardno);
//	void ConfigDSP_DAC(P9054_HANDLE hDev);
//	void InterruptDSP(P9054_HANDLE hDev, int nCardNo);
    void WaveDelay(P9054_HANDLE hDev, WORD nDelay);
    // 延时
//	static void Delay(DWORD dwUs);
    //void SetMonitorRange(WDC_DEVICE_HANDLE hDev, WORD *pInterSpace);//设置范围
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
    QString m_strVersion;
private:
    // 卡自检（多功能卡和工作卡）
    BOOL AllCardsSelfTest(SharedCardParamVec pstThreadParam);
    // AEC卡自检
    BOOL CardSelfTest(P9054_HANDLE hDev);
    // 操作LED灯
    BOOL AEC_OperationLamp(P9054_HANDLE hDev, WORD nValue);

    // 多功能卡自检
    BOOL MultiCardTest(P9054_HANDLE hDev);
    // 多功能卡初始化
    BOOL MultiCardInit(P9054_HANDLE hDev);
    // 配件配置发现与排序
    BOOL BusSlotFindAndSort(SharedCardParamVec pstThreadParam);
    // 查询卡Bus、Slot号
    int FindCardBusSlot(WDC_PCI_SCAN_RESULT *pscanResult, WD_PCI_SLOT *pBusSlotIndex);
    // Slot排序
    void SortSlot(WD_PCI_SLOT *pBusSlotIndex,int size);
    // 获取Slot排序标志
    int GetSlotFlag();
    // 使用HPI方式写数据
//	void WriteDataHPI(P9054_HANDLE hDev, DWORD dwAddr, void *pdata, int size);
    void Delay(DWORD dwUs);
private:
    E_InitResult m_eInitResult;
public:
    E_InitResult GetInitResult();

};


#endif
