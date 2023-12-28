#ifndef UTSETUPTOJSON_H
#define UTSETUPTOJSON_H

#include <QJsonArray>
#include <QMainWindow>
#include <QMap>
#include "ProbeNoManager/ProbeNoManager.h"

#define PER_AEC_CHN_NUM 8       // 每块AEC卡的硬通道数
#define PER_TX_CARD_CHN_NUM  8  // 每个前置卡通道数
#define PER_TX_CHN_PROBE_NUM 2   // 每个前置通道接的探头数

#define NEW_PROBE_CONNECT 0     // 最新探头接线（AEC硬通道不闲置，顺序接满）
#define PROBE_SENSOR 1          // 超声探头传感器

#if PROBE_SENSOR
#define AEC_CARD_NUM 11          // AEC卡的数量
#else
#define AEC_CARD_NUM 9          // AEC卡的数量
#endif
#define ALL_AEC_CHN_NUM (AEC_CARD_NUM * PER_AEC_CHN_NUM)

#define PUS_DOUBLE 1    // 双口前置
#define LANGFA_TEST 1   // 廊坊实验系统

namespace Ui {
class UTSetupToJson;
}

class UTSetupToJson : public QMainWindow
{
    Q_OBJECT

public:
    enum FileType
    {
        UpperComputer,
        LowerComputer1,
        LowerComputer2,
        None
    };
    enum TxType     //前置卡类型
    {
        SINGLE = 0,
        DOUBLE
    };
    typedef QMap<qint32, qint32> MapProbeLightEye;
public:
    explicit UTSetupToJson(QWidget *parent = 0);
    ~UTSetupToJson();

private slots:
    void DoJsonToFileUTSetup();
    void DoJsonToFileChannelList();
    void DoJsonToFileUTInstrument();

    void on_GenProbeConnect_clicked();
    void on_GenProbeEyeArea_clicked();
    void on_GenProbeNoInfoJson_clicked();   //ProbeNoInfo.json

private:
    void InitUI();
    void Connect();
    FileType GetFileType();
    QJsonArray MakeArray(int start, int end);
    void GenProbeNoInfoJson();
    void GenProbeConnectJson();
    void SaveJsonConfig(const QJsonDocument &jsonDoc, const QJsonValue &path, const QString &defaultPath);
    void InsertMapEye(qint32 nTxProbe0Or1, qint32 nAecChnNo, qint32 nDectectArea,
                      qint32 nSection, qint32 nProbeNo);
    void InsertMapEyeDouble(bool bEyeFlag, qint32 nTxProbe0Or1, qint32 nAecChnNo,
                            qint32 nSection, qint32 nProbeNo);
    void InsertMapEyeSingle(bool bEyeFlag, qint32 nTxProbe0Or1, qint32 nAecChnNo,
                                     qint32 nSection, qint32 nProbeNo);
    void SetProbeConnect(qint32 nStart);
    void SetProbeConnectDouble(ProbeNoInfo::Track track);
    void GenProbeEyeAreaJson();
    void SetProbeNo(bool bEyeFlag, int nLR, int nAecChannel, int nBegin,
                    int iStart, int iNum,
                    int nSection, int nProbeStart, int jStart = 0, int jEnd = PER_TX_CARD_CHN_NUM, int iNumReal = 0);
    void SetProbeNoSingle(bool bEyeFlag, int nLR, int nAecIdx, int nBegin,
                    int iStart, int iNum,
                    int nSection, int nProbeStart, int jStart = 0, int jEnd = PER_TX_CARD_CHN_NUM, int iNumReal = 0);
    void SetProbeNull(int nLR, int nAecChannel, int iStart, int iNum);
    void SetProbeArmRimAxial(int nLR, int& nAecIdx);
    void SetProbeFlangeRadial(int nLR, int& nAecChannel);
    //void SetProbeRimRadial(int nLR, int& nAecChannel);
    //void SetProbeArmRadialPositive(int nLR, int& nAecChannel);
    //void SetProbeArmRadialNegative(int nLR, int& nAecChannel);
    void SetProbeSensor(int nLR, int& nAecChannel);
    void SetProbe21Positive(int nLR, int& nAecChannel);
    void SetProbe21PositiveAndNegative(int nLR, int& nAecChannel);
    void SetProbe21Negative(int nLR, int& nAecChannel);
    void LoadProbeNoInfoJsonFile();

private:
    Ui::UTSetupToJson *ui;
    QString filePath;
    QString fileNameUTSetup;    
    QString fileNameChannelList;
    QString fileNameInstrument;
    QString m_strProbeConnectFile;    // 探头接线JSON文件名
    MapProbeLightEye m_mapEye;
    const qint32 AREA_EYE_NUM = 32;   // 一块检测区域的光眼传感器数量
    const qint32 SECTION_EYE_NUM = 8;      // 检测区域里的一段的光眼传感器数量
    const qint32 METER2_EYE_NUM = 16;  // 2米区域的传感器数量
    qint32 AecChannel[2][AEC_CARD_NUM][PER_AEC_CHN_NUM][PER_TX_CARD_CHN_NUM][PER_TX_CHN_PROBE_NUM];
    ProbeNoManager *m_probeNoManager;
};

#endif // UTSETUPTOJSON_H
