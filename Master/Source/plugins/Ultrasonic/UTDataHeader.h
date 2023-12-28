#pragma once

#include <QCoreApplication>
#include <QHash>
#include <QStringList>
#include <QVariantMap>
#include <QVector>

enum UTDataType {
    DataInvalid,
    DataTime,
    DataAmp,
    DataPosition,
    DataIndex,
};
//Q_ENUM(DataType)

enum UTGateType {
    GateInvalid,
    GateTrigger,
    GateMaxPeak,
    GateFirstPeak,
    FeatureContext,
    FeaturePose,
};
//Q_ENUM(GateType)

struct UTDataHeaderItem;

/*!
 * UTDataHeader 是为 UTDataFrame 等数据存储类服务的类，负责记录每个特征值的具体含义
 * 受限于存储空间和处理效率， UTDataFrame 中的特征值，使用一个数字索引的列表存储，其中并不包含
 * 这些特征值的具体含义。
 *
 * 在 UTDataHeader 中记录了C扫数据每个特征值索引，及其对应的数据的属性：闸门号、闸门特征、数据
 * 类型。
 * 同时， UTDataHeader 还提供高速反向查找，用于按特征值类型来访问 UTDataFrame 。
 */
class UTDataHeader
{
    Q_GADGET
    Q_DECLARE_TR_FUNCTIONS(UTDataHeader)

public:
    UTDataHeader() {}
    UTDataHeader(const QList<UTDataHeaderItem> &list);
    UTDataHeader(const QString &name) : name(name) {}

    bool IsValid() const;
    QList<UTDataHeaderItem> GetItems() const;
    QString GetName() const {return name;}
    void SetType(int index, int gate, UTGateType gt, UTDataType dt, const QString &name, int syncWith = -1);    /* 设置(添加）新闸门数据，即C扫特征 */
    void SetPoseOffset(int offset);
    void SetPoseCount(int count);

    int GetNumFeatures() const {return items.count();}                          /* 获取C扫特征数量 */

    int gateId(int index) const;                                             /* 获取闸门ID */
    UTDataType GetDataType(int index) const;                                    /* 获取特征的数据类型 */
    UTGateType GetGateType(int index) const;                                    /* 获取特征的闸门类型 */
    QString GetName(int index) const;                                           /* 获取特征名称 */
    int GetSyncIndex(int index) const;                                          /* 获取特征同步标志 */
    QString GetUnitName(int index) const;                                       /* 获取特征值的单位 */

    int GetNumGates() const {return numGates;}                                  /* 获取闸门总数量 */
    bool HasFeature(int gate, UTGateType gt, UTDataType dt) const;              /* 判断该特征是否被记录 */
    int dataIndex(int gate, int gt, int dt) const;                           /* 获取特征索引号*/
    int GetEquivalent(int index, const UTDataHeader &other) const;
    QList<UTGateType> GetGateTypes(int gate) const;

    inline int GetAmpIndex(int gate, UTGateType gt) const {return dataIndex(gate, gt, DataAmp);}      /* 获取幅值特征的索引号，在CData中的索引号对应 */
    inline int GetTimeIndex(int gate, UTGateType gt) const {return dataIndex(gate, gt, DataTime);}    /* 获取时间特征的索引号，在CData中的索引号对应 */
    QString GetDescription(int index) const;                                    /* 获取特征描述 */
    QStringList GetDescriptions() const;                                        /* 获取所有特征的描述表 */
    QVariantMap GetGateDescription(int gate) const;                             /* 获取对应闸门的特征值描述表 */

    static const int invalidGate = -1;

private:
    void RefreshCache();

private:
    QVector<UTDataHeaderItem> items;                                            /*!< 特征值描述表 */
    QHash<int, int> cache;                                                      /*!< 特征值描述表的反向查询表，作缓存使用 */
    QString name;
    int numGates = 0;
};

/*! 特征值描述 */
struct UTDataHeaderItem {
    QString name;
    int gate = UTDataHeader::invalidGate;
    UTDataType dt = DataInvalid;
    UTGateType gt = GateInvalid;
    int syncWith = -1;
};
