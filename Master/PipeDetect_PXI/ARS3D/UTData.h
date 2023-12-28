#ifndef UTDATA_H
#define UTDATA_H

#include <QtGlobal>
#include <QSharedPointer>
#include <QAtomicInt>
#include "UTPose.h"
#include "UTDataContext.h"
//#include <QVector3D>                                                          /* UTPose.h 包含了QVector3D的定义 */

typedef qint16 ADataValue;
typedef float CDataValue;

//typedef QVector<UTPoseValue> UTPose;
typedef QVector<ADataValue> ADataFrame;
typedef QVector<CDataValue> CDataFrame;

typedef qint64 UTDataFileOffset;

/*!
 * \macro ARS3D_SHARED_POINTER_LOCAL_ALLOCATION
 * \brief 编译器优化开关，当开启时，共享指针的分配将与数据对象分配在同一个内存区域，这可能有助于
 * 提高内存局部性，从而提高在长时间运行时，对指针的访问效率。
 */
#define ARS3D_SHARED_POINTER_LOCAL_ALLOCATION 1

#ifndef Q_ATOMIC_INT32_IS_SUPPORTED
#   error("Atomic int32 not supported.")
#endif

/*!
 * \brief UTDataFrame 是一个超声扫描数据帧，包含在仪器采样的一个样本中所需的所有信息。
 * \inmodule ARS3D
 * 帧中包含的数据有：
 * A扫描数据 (可选)
 * C扫描数据
 * 位姿数据
 * 上下文(参见UTDataContext)
 */
struct UTDataFrame;
typedef QSharedPointer<UTDataFrame> UTSharedDataFrame;

struct UTDataFrame
{
public:
    UTDataFrame();
    UTDataFrame(const UTDataFrame &other);
    ~UTDataFrame();

    float GetWaterPathLength(float waterSpeed);
    QVector3D GetWaterPathVector(float waterSpeed);
    QVector3D GetWaterPathPosition(float waterSpeed);
    float GetMaterialPathLength(float speed, uint CDataIndex);
    QVector3D GetMaterialPathVector(float speed, uint CDataIndex);
    float GetFullPathLength(float waterSpeed, float materialSpeed, uint CDataIndex);
    QVector3D GetFullPathVector(float waterSpeed, float materialSpeed, uint CDataIndex);
    QVector3D GetFullPathPosition(float waterSpeed, float materialSpeed, uint CDataIndex);

public:
    /*! \brief A扫描数据Vector，表达A扫描波形在各个时间上的幅值。 */
    ADataFrame AData;

    /*! \brief C扫描特征值 */
    CDataFrame CData;
    UTPose Pose;
    UTDataContext Context;
    QPoint point;
    UTDataFileOffset FileOffset;

    static QAtomicInt count;
    static const int GATE_SYNC_TIME_INDEX = 0;
};

typedef QList<UTSharedDataFrame> UTDataSegment;
typedef QSharedPointer<UTDataSegment> UTSharedDataSegment;

#endif // UTDATA_H
