#ifndef UTDATA_H
#define UTDATA_H

#include <QSharedPointer>
#include <QAtomicInt>
#include "UTPose.h"
#include "UTDataContext.h"

typedef qint16 ADataValue;
typedef float CDataValue;

typedef QVector<ADataValue> ADataFrame;
typedef QVector<CDataValue> CDataFrame;

typedef qint64 UTDataFileOffset;
class UTDataHeader;
class UTDataSet;

#define ARS_DUMP_CSV

/*!
 * \brief UTDataFrame 是一个超声扫描数据帧，包含在仪器采样的一个样本中所需的所有信息。
 * 帧中包含的数据有：
 * A扫描数据    (AData) 可能为空
 * C扫描数据    (CData)
 * 位姿数据     (Pose)
 * 上下文      (UTDataContext)
 *
 * 当这个数据结构作为参数传递时，应当使用UTSharedDataFrame和UTSharedDataSegment
 *  UTSharedDataSegment中的元素可能为NULL
 */
struct UTDataFrame;
typedef QSharedPointer<UTDataFrame> UTSharedDataFrame;

struct UTDataFrame
{
public:
    enum SpecialAxis {
        AXIS_X = -1,
        AXIS_Y = -2,
        AXIS_Z = -3,
        AXIS_DIR_X = -4,
        AXIS_DIR_Y = -5,
        AXIS_DIR_Z = -6,
    };

    enum DataSource {
        DataEmbedded,
        DataSetReference,
    };

public:
    UTDataFrame();
    UTDataFrame(const UTDataFrame &other);
    ~UTDataFrame();

    bool IsEmpty() const;
    bool IsADataValid() const;
    bool IsCDataValid() const;

    bool IsIGateTriggered() const;

    CDataValue GetSyncTime() const;
    float GetWaterPathLength(float waterSpeed = 1500) const;

    QVector3D GetWaterPathVector(float waterSpeed = 1500) const;
    QVector3D GetWaterPathPosition(float waterSpeed = 1500) const;

    float GetMaterialPathLength(float speed, uint CDataIndex) const;
    QVector3D GetMaterialPathVector(float speed, uint CDataIndex) const;

    float GetFullPathLength(float waterSpeed, float materialSpeed, uint CDataIndex) const;
    QVector3D GetFullPathVector(float waterSpeed, float materialSpeed, uint CDataIndex) const;
    QVector3D GetFullPathPosition(float waterSpeed, float materialSpeed, uint CDataIndex) const;

    void UpdatePosition();
    UTPoseValue GetPose(int axis) const;
    QVector3D GetPosition() const {return position;}
    QVector3D GetDirection() const {return direction;}

    float GetMPTime(int gateIndex) const;
    float GetMPPercent(int gateIndex) const;
    float GetFPTime(int gateIndex) const;
    float GetFPPercent(int gateIndex) const;
    QString GetGatePosInfo(int index) const;
    QString GetChannelName() const;

    QString GetGateDisplayInfo(int dataIndex, float materialSpeed, float waterSpeed = 1500) const;

    double GetMaterialSpeed() const;

    int GetNumGates() const;
    static int GetGateInfoNum(int nGateIndex);
    static int GetGateStartCDataIndex(int gateIndex);

    static int RegisterHeader(const UTDataHeader &header);
    static const UTDataHeader &GetHeaderById(int id);
    inline const UTDataHeader &GetHeader() const {return GetHeaderById(headerId);}

    QString GetGateInfo(int featureIndex, bool useTime = false, bool usePercent = true) const;
    QString GetPosInfo(int axisID) const;
    QString ExportCsv() const;

public:
    /*! A扫描数据，表达A扫描波形在各个时间上的幅值。 */
    ADataFrame AData;

    /*! C扫描特征值 */
    CDataFrame CData;
    UTPose Pose;

    UTDataContext Context;
    QVector3D position;
    QVector3D direction;
    int headerId;

    static QAtomicInt globalCount;
    static const int GATE_SYNC_TIME_INDEX = 0;
    static QList<UTDataHeader> headers;
};

typedef QList<UTSharedDataFrame> UTSharedDataSegment;

Q_DECLARE_METATYPE(UTSharedDataFrame)
Q_DECLARE_METATYPE(UTSharedDataSegment)

void DumpSegment(const QString &fileName, UTSharedDataSegment segment);

#endif                                                                          // UTDATA_H
