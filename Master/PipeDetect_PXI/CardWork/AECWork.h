#ifndef AECWORK_H
#define AECWORK_H

#include <QObject>
#include "CardWork/CardDataDefine.h"
#include <QMap>
#include "TcpServer/DataDef.h"
#include "Platform/Platform.h"
#include "ThreadController/ThreadWork.h"
#include "CardWork/CardWorkBase.h"

class AECWork : public CardWorkBase
{
    Q_OBJECT

public:
    typedef NS_PXI100::SIMULATION_DATA_t SIMULATION_DATA_t;
    typedef QVector<quint16> VectorValidSoftChannel;
    typedef QVector<UTSharedMonitorFrame> UTSharedMonitorFrameVector;
    typedef QMap<qint32, qint32> ProbeAndChannelNoMap;

public:
    explicit AECWork(QObject *parent = 0);
    quint32 HardChannelDataAddress(quint32 hardIndex = 0);
    quint32 HardChannelRangeAddress(quint32 hardIndex = 0);
    quint32 GateEigenvalueAddressStart(quint32 bufferIndex = 0);
    quint32 GetTCGAddress(quint32 channel);
    void InitChannelInfo();

private:
    UTSharedFeatureFrame m_frameMax;
    UTSharedMonitorFrameVector m_vectorMonitorframe;
    quint16 m_mark;
    ProbeAndChannelNoMap mapChannel2Probe;
    ProbeAndChannelNoMap mapProbe2Channel;
    const int MONITOR_BUFFER_COUNT = 8;
    NS_PXI100::CH_CONFIG config;
    qint32 channel;
    bool isSetOne;
    qint32 setAllCount;

signals:
    void MonitorDataRead(UTSharedMonitorFrame data, qint32 nIdx);
    void CscanDataRead(UTSharedFeatureSegment data, qint32 nFrameNum);

public slots:
    virtual void SetSelectChannel(qint32 nChnNo, qint32 nSurOrFlaw);
    void OnSetOne(UTSharedDataParam param, qint32 nChnNo);
    void SetOne();
    void SetWaveType(quint16 mark);
    void SetProbeBoxSimulationData(bool isOpen = 0);
    void SetTCG(quint32 channel, NS_PXI100::TCGCommuData data1, NS_PXI100::TCGData data2);
    void OnConnectionChanged(bool isConnect);

private:
    virtual bool InitCard();
    virtual bool CardStart();
    virtual bool ReadFlawResult();
    virtual bool CardReading();
    void Params2HardParam(NS_PXI100::CH_CONFIG_HARD &stConfig, quint16 probeNo);
    bool ReadMonitorData(quint16* pData, unsigned int address, quint32 bufferIndex);
    void SetMonitorRange(quint16* pInterSpace, quint32 bufferIndex = 0);
    void ValidSoftChannelVector(VectorValidSoftChannel &vector, quint16 mark);
    bool InitChannelMap();
    void CalChannelIndex(quint16 &probeNo, quint16 hardChannel);
    void CalChannelIndexForCData(quint16 &probeNo, quint16 hardChannel);

};

#endif // AECWORK_H
