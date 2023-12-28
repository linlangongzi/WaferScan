#pragma once

#include "UTDataContext.h"
#include "UTDataGlobals.h"
#include "UTDataHeader.h"
#include "UTPose.h"
#include <QAtomicInt>
#include <QCoreApplication>
#include <QSharedPointer>

typedef qint64 UTStreamFileOffset;
class UTDataHeader;

#define ARS_DUMP_CSV

/*!
 * \brief UTDataFrame 是一个超声扫描数据帧，包含在仪器采样的一个样本中所需的所有信息。
 * 帧中包含的数据有：
 * A扫描数据    (AData) 可能为空
 * C扫描数据    (CData)
 * 位姿数据     (Pose)
 * 上下文      (UTDataContext)
 *
 * 当这个数据结构作为参数传递时，应当使用UTDataFrame和UTDataSegment
 *  UTDataSegment中的元素可能为NULL
 */
struct UTDataFrame;
struct UTDataFrameData;

struct UTDataFrame
{
    Q_DECLARE_TR_FUNCTIONS(UTDataFrame)
public:
    enum SpecialAxis {
        AXIS_X = -1,
        AXIS_Y = -2,
        AXIS_Z = -3,
        AXIS_DIR_X = -4,
        AXIS_DIR_Y = -5,
        AXIS_DIR_Z = -6,
    };

    enum DataType {
        AllDataFrame,
        AScanDataFrame,
        CScanDataFrame,
        PoseDataFrame,
    };

public:
    UTDataFrame();
    UTDataFrame(const UTDataFrame &other);
    ~UTDataFrame();

    bool isEmpty() const;
    bool IsCDataValid() const;

    bool hasWaveform() const;
    int waveformLength() const;
    ADataValue waveformAt(int index) const;
    ADataFrame waveform() const;
    static ADataValue waveformMax();

    bool IsIGateTriggered() const;

    CDataValue syncTime() const;
    qreal waterPathLength(qreal waterSpeed = 1500) const;                       /* 水距mm */

    QVector3D GetWaterPathVector(float waterSpeed = 1500) const;                /* 水距向量：从探头出射点指向工件表面入射点的向量 */
    QVector3D GetWaterPathPosition(float waterSpeed = 1500) const;              /* 工件表面入射点空间位置 */

    float GetMaterialPathLength(float speed, uint CDataIndex) const;            /* 声束在工件内的行程mm */
    QVector3D GetMaterialPathVector(float speed, uint CDataIndex) const;        /* 声束在工件内的行程向量 */

    float GetFullPathLength(float waterSpeed, float materialSpeed, uint CDataIndex) const;          /* 声束在水中和工件中的总行程mm */
    QVector3D GetFullPathVector(float waterSpeed, float materialSpeed, uint CDataIndex) const;      /* 声束在水中和工件中的总行程向量 */
    QVector3D GetFullPathPosition(float waterSpeed, float materialSpeed, uint CDataIndex) const;    /* 工件中某C扫特征在空间中的位置 */

    QPair<QVector3D, QVector3D> UpdatePosition() const;
    UTPoseValue GetPose(int axis) const;                                        /* 轴axis的位置 */
//    QVector3D GetPosition() const {return position;}                          /* 探头末端(声束出射点）的空间位置 */
//    QVector3D GetDirection() const {return direction;}                        /* 声束的指向向量 */

    bool isGateTriggered(int gateIndex) const;
    float gateTime(int gateIndex, UTGateType gt = GateMaxPeak) const;           /* 最高波峰时间。最高波峰是指在闸门区间内的最高波峰，无须触发 */
    float gateAmp(int gateIndex) const;                                         /* 最高波峰幅值百分比 */
    float GetFPTime(int gateIndex) const;                                       /* 首次波峰时间。首次波峰是指闸门区间内被触发后的第一个波峰，需要触发 */
    float GetFPPercent(int gateIndex) const;                                    /* 首次波峰幅值百分比 */
    QString channelName() const;                                                /* 仪器通道名称 */
    int channelId() const;

    qreal materialSpeed() const;                                                /* 材料声速 */

    CDataValue GetFeature(int featureIndex) const;                              /* 由特征索引返回特征值 */
    CDataValue GetFeature(int gate, UTGateType gt, UTDataType dt) const;        /* 由闸门、闸门类型、触发时间*/
    CDataValue FeatureValue(int featureIndex) const;
    CDataValue FeatureValue(int gate, UTGateType gt, UTDataType dt) const;      /* 原生的特征 */
    void SetFeature(CDataValue value, int gate, UTGateType gt, UTDataType dt);  /* 设置特征：将原特征替换掉 */
    int GetNumGates() const;                                                    /* 数据帧中的闸门个数 */

    static int RegisterHeader(const UTDataHeader &header, int id);              /* 注册一个UTDataHeader到全局Headers中 */
    static const UTDataHeader &GetHeaderById(int id);                           /* 获取全局Headers中第id个Header */
    inline const UTDataHeader &GetHeader() const {return GetHeaderById(headerId);}  /* 获取全局Headers中headerID对应的Header */

    QString GetFeatureText(int featureIndex, bool useTime = false, bool usePercent = true, qreal ampReference = 80) const;  /* 闸门各特征文字信息 */
    QString GetPosInfo(int axisID) const;                                       /* 轴位置（文字） */
    QString exportCsv() const;                                                  /* 将一帧数据导出，分别是C扫数据、位置、A扫数据 */
    QString exportCsv(DataType dataType) const;

    bool operator ==(const UTDataFrame &other) const;
    void setContext(UTDataContext::ContextIndex index, qreal value);
public:
    /*! A扫描数据，表达A扫描波形在各个时间上的幅值。 */
    ADataFrame AData;
    /*! C扫描特征值 */
    CDataFrame CData;
    UTPose Pose;
    UTDataContext Context;
    int headerId = 0;

    static QAtomicInt globalCount;
    static const int GATE_SYNC_TIME_INDEX = 0;
};

using UTDataSegment = QList<UTDataFrame>;                                       /* 指向UTDataFrame的指针列表,可以理解为一个数据包，含有多个数据帧 */

Q_DECLARE_METATYPE(UTDataFrame)
Q_DECLARE_METATYPE(UTDataSegment)

void DumpSegment(const QString &fileName, UTDataSegment segment);
