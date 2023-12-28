#ifndef AECWORK_H
#define AECWORK_H

#include <QObject>
#include "CardWork/CardDataDefine.h"
#include <QMap>
#include "TcpServer/DataDef.h"
#include "Platform/Platform.h"
#include "ThreadController/ThreadWork.h"
#include "CardWork/CardWorkBase.h"

const int AEC_HARD_CHN_NUM = 8;

class AECWork : public CardWorkBase
{
    Q_OBJECT

public:
    typedef NS_PXI100::SIMULATION_DATA_t SIMULATION_DATA_t;

public:
    explicit AECWork(QObject *parent = 0);
    static QVector<int> vecSet;

private:
//    sharedProbeNoManager m_probeNoManager;
    quint16 m_usEyeArea;    //光眼区域0-7(16个光眼一个区域)
    qint16 m_curEyeArea;   // 0-15
    quint16 m_usSensor;
    quint16 m_usSensorSingle[AEC_HARD_CHN_NUM];
    quint16 m_curEyeAreaSingle[AEC_HARD_CHN_NUM];
    qint16 m_usSoftIdxSingle[AEC_HARD_CHN_NUM];
    qint16 m_usSoftIdxSingle1[AEC_HARD_CHN_NUM];
    bool m_bMaxFrameFlag;
    UTSharedFeatureFrame m_frameMax;
    UTSharedFeatureFrame m_frameMax1;
    bool m_bEyesRead;
    static const int m_frameBytes;
    qint16 m_usSoftIdx;
    qint16 m_usSoftIdx1;
    bool m_bDeleteFrames;
    int m_nDeleteNum;
    QVector<quint16> m_vecAvgAll;
    int m_avgCount;
    int m_sensorCardNum;
    int m_straightFlangeCardNum;
    int m_21DegreeCardNum;
    bool m_isStraightFlangeTwoSensor;
    bool m_is21DegreeTwoSensor;

signals:
    void MonitorDataRead(UTSharedMonitorFrame data, qint32 nIdx);
    void CscanDataRead(UTSharedFeatureSegment data, qint32 nFrameNum);
    void CscanDataRead(UTSharedCommuSegment data);
    void EyesDataRead(UTSharedDataSimulation simulation);
    void TriggerTimeStamp(quint16 usChnNo, quint32 unTimeStamp);
    void FlangeDataRead(UTSharedFeatureSegment, qint32 nFrameNum);

public slots:
    virtual void SetSelectChannel(qint32 nChnNo, qint32 nSurOrFlaw);
    void OnSetOne(UTSharedDataParam param, qint32 nChnNo);
    void SetEyesStatusReadFlag(bool isOpen);
    void OnSetScanStutas(qint32 nStataus);
    void SetSimulationData(UTSharedDataSimulation simulation);
    void OnSetRepeatFrequence(qint32 nValue);
    void OnSetSensorRoll(quint16 nValue);
    void SetTCG(qint32 chnNo, TCGCommuData data, TCGData tcgData);

private:
    virtual bool InitCard();
    virtual bool CardStart();
    virtual bool ReadFlawResult();
    virtual bool CardReading();
    void Params2HardParam(UTSharedDataParam param, NS_PXI100::CH_CONFIG_HARD &stConfig);
    void SendAllFeatureFrame(UTSharedFeatureSegment  segmentIn, qint32 nFrameNum);
//#if SENSOR_TWO_WORK
#if FLANGE_ANALYSIS
    void CalcMaxFrameTwoSensor(UTSharedFeatureSegment segmentIn, int idx, int chnIdx,
                               UTSharedFeatureFrame frameMax, UTSharedFeatureFrame frameMax1);
    void EmitMaxFrameTwoSensor(UTSharedFeatureSegment segmentIn, int idx, int chnIdx, int nFrameRead, int nFrameNum,
                               UTSharedFeatureFrame frameMax, UTSharedFeatureFrame frameMax1);
#endif
    void CalcMaxFrameTwoSensor(UTSharedFeatureSegment segmentIn, int idx,
                               UTSharedFeatureFrame frameMax, UTSharedFeatureFrame frameMax1);
    void EmitMaxFrameTwoSensor(UTSharedFeatureSegment segmentIn, int idx, int nFrameRead, int nFrameNum,
                               UTSharedFeatureFrame frameMax, UTSharedFeatureFrame frameMax1);
    void GetMaxFeatureFrame(UTSharedFeatureSegment segmentIn, qint32 nFrameNum,
                            qint32 &nFrameRead, UTSharedFeatureFrame frameMax, UTSharedFeatureFrame frameMax1);
//#else
    bool GetMaxFeatureFrame(UTSharedFeatureSegment segmentIn, qint32 nFrameNum,
                            qint32 &nFrameRead, UTSharedFeatureFrame frameMax);
//#endif
    void CalcAvgFrameTwoSensor(UTSharedFeatureSegment segmentIn, int idx,
                                 UTSharedFeatureFrame frameAvg, UTSharedFeatureFrame frameAvg1);
    void SetMaxFrameFlag(bool bFlag);
    UTSharedDataSimulation ReadEyesStatus();
    bool ReadMonitorData(quint16* pData);
    void SetMonitorRange(quint16* pInterSpace);
    void OnSetSurfaceTrigger(bool bFlag);
    void InitHardChn();
    quint16 GetEyeLowOrHi(quint16 usEye, int nChnNum);
    quint16 GetSoftChnIdx(quint16 usSoftIdx, int nChnNum);
    bool IsStraightFlangeCard();
    bool Is21DegreeCard();
    bool IsSensorCard();
    bool isTwoSensorWork();
    quint32 GetTCGAddress(qint32 chnNo);
    qint32 GetSoftChnNo(qint32 chnNo);
};

#endif // AECWORK_H
