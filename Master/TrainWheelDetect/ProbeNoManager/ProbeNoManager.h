#ifndef PROBENOMANAGER_H
#define PROBENOMANAGER_H

#include <QMap>
#include <QObject>
#include <QMultiMap>
#include "ProbeNoManager/ProbeNoInfo.h"

#define INVALID_VALUE -1

enum PROBE_DEFECT_TYPE_e{
    LEFT_RIM_TREAD = 0,      // 轮辋轴向、踏面(外侧一排)(轮辐没有外侧)
    LEFT_RIM_ARM_TREAD,      // 轮辋、轮幅轴向、踏面(内侧一排)
    LEFT_FLANGE_RADIAL,      // 轮缘径向
    LEFT_RIM_RADIAL_PLUS1,   // 轮辋径向正向(外侧)
    LEFT_RIM_RADIAL_PLUS2,   // 轮辋径向正向(内侧)
    LEFT_RIM_RADIAL_MINUS1,  // 轮辋径向负向(外侧)
    LEFT_RIM_RADIAL_MINUS2,  // 轮辋径向负向(内侧)
    LEFT_SENSOR,             // 传感器
    RIGHT_RIM_TREAD,         // 轮辋轴向、踏面(外侧一排)(轮辐没有外侧)
    RIGHT_RIM_ARM_TREAD,     // 轮辋、轮幅轴向、踏面(内侧一排)
    RIGHT_FLANGE_RADIAL,     // 轮缘径向
    RIGHT_RIM_RADIAL_PLUS1,  // 轮辋径向正向(外侧)
    RIGHT_RIM_RADIAL_PLUS2,  // 轮辋径向正向(内侧)
    RIGHT_RIM_RADIAL_MINUS1, // 轮辋径向负向(外侧)
    RIGHT_RIM_RADIAL_MINUS2, // 轮辋径向负向(内侧)
    RIGHT_SENSOR
};

class ProbeNoManager : public QObject
{
    Q_OBJECT

public:
    typedef QMap<qint32, PROBE_DEFECT_TYPE_e> MapType;
    typedef QMap<PROBE_DEFECT_TYPE_e, qint32> MapProbeNum;
    typedef QMap<PROBE_DEFECT_TYPE_e, qint32> MapProbeNo;
    typedef QMap<qint32, qint32> MapEyeArea;
    typedef QMultiMap<qint32, qint32> MultiMapEyeArea;
    typedef QList<qint32> ProbeNoList;

public:
    explicit ProbeNoManager(QObject *parent = 0);

private:
    MapType m_mapType;                  // 探头组-缺陷MAP
    MapProbeNo m_mapTypeProbeMin;       // 每个大类缺陷的最小探头号
    MapProbeNo m_mapTypeProbeMax;       // 每个大类缺陷的最大探头号
    MapEyeArea m_mapEyeArea;            // 每个探头号对应的光眼区域
    MultiMapEyeArea m_multiMapEyeArea;  // 每个传感器区域对应的探头编号List
    QString m_strEyeAreaFile;
    ProbeNoInfo* m_probeNoInfoLeft;
    ProbeNoInfo* m_probeNoInfoRight;
    ProbeNoList m_ProbeNoList;

private:
    void InitMap();
    void InsertTypeMap(qint32 nMin, qint32 qMax, PROBE_DEFECT_TYPE_e type);
    ProbeNoList GetProbeNoList();
    int GetTypeOffsetByTrack(ProbeNoInfo::Track track);
    void InitMapByProbeNoInfo(ProbeNoInfo* info);
    void InitMapProbeNoType(ProbeNoInfo* info);
    void InitMapTypeProbeMin(ProbeNoInfo* info);
    void InitMapTypeProbeMax(ProbeNoInfo* info);

public:
    static ProbeNoManager *instance();
    MapType GetProbeTypeMap();
    PROBE_DEFECT_TYPE_e ProbeNo2Type(qint32 nProbeNo);
    qint32 ProbeNo2TypeProbeNum(qint32 nProbeNo);
    qint32 ProbeNo2EyeArea(qint32 nProbeNo);
    ProbeNoList EyeArea2ProbeNoList(qint32 area);
    qint32 GetMinimumProbeNo(ProbeNoList list);
    void InitMapEyeArea();
    ProbeNoInfo* GetProbeNoInfoLeft();
    ProbeNoInfo* GetProbeNoInfoRight();
    bool IsProbeFlange(int probeNo);
    bool IsProbeRadialPositive(int probeNo);
    bool IsProbeSensor(int probeNo);
    bool IsProbeStraightOutside(int probeNo);
    int GetProbeMin();
    int GetProbeMax();
    int GetNextProbeNo(int probeNo);
    int GetIndexInProbeType(int probeNo);
    bool IsProbeValid(int probeNo);
    int GetProbeMinLeft();
    int GetProbeMinRight();
    int GetProbeMaxLeft();
    int GetProbeMaxRight();
    bool isProbeNoLeft(int probeNo);
    ProbeNoInfo::Track GetTrack(int probeNo);
    int GetTrackStartProbeNo(ProbeNoInfo::Track track);
    int GetProbeMinByType(PROBE_DEFECT_TYPE_e type);
    int GetProbeMaxByType(PROBE_DEFECT_TYPE_e type);
    int GetProbeNumByType(PROBE_DEFECT_TYPE_e type);
    int GetSensorMinIdx();
    int GetSensorMaxIdx();
    bool IsProbeStraightFlange(int probeNo);
    bool IsProbe21Degree(int probeNo);
};

#endif // PROBENOMANAGER_H
