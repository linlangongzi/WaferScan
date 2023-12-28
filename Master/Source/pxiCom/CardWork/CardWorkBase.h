#pragma once

#include <QObject>
#include "CardDataDefine.h"
#include "ThreadController/ThreadWork.h"
#include "ProbeConnectManager/ProbeConnectManager.h"

class CardWorkBase : public ThreadWork
{
    Q_OBJECT

public:
    enum eWorkStatus
    {
        INIT = 0,   // 初始化
        START,      // 开始
        STOP,       // 结束
        READING,    // 采集数据中
        NONE,       // 空闲
    };

    typedef QMap<quint32, quint16> MFCDataMap;

public:
    explicit CardWorkBase(QObject *parent = 0);

protected:
    eWorkStatus m_nStatus;   // 卡工作状态
    SharedCardParam m_pParam;
    sharedProbeNoManager m_probeNoManager;

public:
    void Init();
    void Start();
    void Stop();
    void SetCardParam(SharedCardParam cardParam);
    SharedCardParam GetCardParam();
    virtual quint16 Gain2Data(float Gain);
    virtual quint32 Range2Dot(float VSound, float Range);
    virtual quint16 MicroSecond2Dot(float fMicroSecond);
    virtual void RangeProcess(float VSound, float Range, unsigned short *pInterSpace);

signals:
    void SendStartFlag(quint16 startFlag);

public slots:
    virtual void Run();
    virtual void SetSelectChannel(qint32 nChnNo, qint32 nSurOrFlaw);
    virtual void OnFinish();
    virtual bool SetPRF(quint16 prf);
    virtual void SetSimulationData(bool isOpen = 0);
    virtual void SetProbeBoxSimulationData(bool isOpen = 0);
    virtual void SetData(quint32 address, quint16 data);
    virtual void SetDataMap(MFCDataMap &dataMap);
    virtual void SetCoderMark(quint16 mark);

protected:
    virtual void ConnectTo();
    virtual bool InitCard();
    virtual bool CardStart();
    virtual bool CardStop();
    virtual bool ReadFlawResult();
    virtual bool CardReading();
    bool SelectChnInCard(int nSelectChn);
};
