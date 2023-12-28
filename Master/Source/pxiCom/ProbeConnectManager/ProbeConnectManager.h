#pragma once

#include <QObject>
#include <QMap>

class ProbeConnectManager : public QObject
{
    Q_OBJECT
public:
    typedef QMap<qint32, qint32> MapProbeAecChn;    //探头号对应的Aec通道号
    typedef QMap<qint32, qint32> MapProbeTxChn0or1; //探头号对应的同一个前置通道的前或后
    typedef QMap<qint32, qint32> MapProbeTxChn;     //探头号对应的前置通道号（0-7）
    typedef QMap<qint32, qint32> MapEyeArea;
    typedef QMultiMap<qint32, qint32> MultiMapEyeArea;

    enum PUTS_TYPE_e
    {
        SINGLE = 0,     // 单口
        DOUBLE,         // 双口
        HALF_SINGLE     //半单口，管探上面4个和下面4个各自输出
    };
    static const int MAX_TX_CHN_NUM;    // 一块前置卡的通道数（双口只算一列）

public:
    explicit ProbeConnectManager(QObject *parent = 0);

private:
    MapProbeAecChn m_mapProbeAecChn;
    MapProbeTxChn0or1 m_mapProbeTxChn0or1;
    MapProbeTxChn m_mapProbeTxChn;
    QString m_strProbeConnectFile;    // 探头接线JSON文件名
    qint32 m_nBasicNo;                // 起始探头编号
    MapEyeArea m_mapEyeArea;            // 每个探头号对应的光眼区域
    MultiMapEyeArea m_multiMapEyeArea;  // 每个传感器区域对应的探头编号List
    QString m_strEyeAreaFile;

private:
    void InitMapProbeAecChn();

public:
    qint32 ProbNo2AecChnNo(qint32 nProbeNo);
    qint32 ProbNo2CardNo(qint32 nProbeNo);
    qint32 ProbNo2CardChnNo(qint32 nProbeNo);
    qint32 ProbNo2TxChn0or1(qint32 nProbeNo);
    qint32 ProbNo2TxChnNo(qint32 nProbeNo);
    qint32 ProbNo2SoftChnNo(qint32 nProbeNo, PUTS_TYPE_e putsType);
    qint32 GetBasicNo();
    qint32 ProbeNo2EyeArea(qint32 nProbeNo);
    QList<qint32> EyeArea2ProbeNoList(qint32 area);
    void InitMapEyeArea();
    void InitMultiMapEyeArea();
    QString GetProbeConnectFileName();
signals:

public slots:

};

typedef QSharedPointer<ProbeConnectManager> sharedProbeNoManager;
typedef QSharedPointer<ProbeConnectManager> sharedProbeConnectManager;
