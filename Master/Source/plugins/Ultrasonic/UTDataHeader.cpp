#include "UTDataHeader.h"
#include <QJsonObject>
#include <QObject>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "ut.data.header")

inline int MakeSignature(int gate, UTGateType gt, UTDataType dt)
{
    return gate + (gt << 16) + (dt << 24);
}

UTDataHeader::UTDataHeader(const QList<UTDataHeaderItem> &list)
{
    items = list.toVector();
    RefreshCache();
}

bool UTDataHeader::IsValid() const
{
    return !items.isEmpty();
}

/*!
 * \brief UTDataHeader::SetType     设置(添加）新闸门数据，即C扫特征
 * \param index                     特征索引号
 * \param gate                      闸门ID
 * \param gt                        闸门类型
 * \param dt                        特征数据类型
 * \param name                      特征名称
 * \param syncWith                  时间同步的基准闸门ID
 */
void UTDataHeader::SetType(int index, int gate, UTGateType gt, UTDataType dt, const QString &name, int syncWith)
{
    if (items.size() <= index) {
        items.resize(index + 1);
    }
    UTDataHeaderItem &entry = items[index];
    entry.gate = gate;
    entry.gt = gt;
    entry.dt = dt;
    entry.syncWith = syncWith;
    entry.name = name;

    RefreshCache();
}

int UTDataHeader::gateId(int index) const
{
    if (uint(index) < uint(items.size())) {
        return items[index].gate;
    }
    return invalidGate;
}

UTDataType UTDataHeader::GetDataType(int index) const
{
    if (uint(index) < uint(items.size())) {
        return items[index].dt;
    }
    return DataInvalid;
}

UTGateType UTDataHeader::GetGateType(int index) const
{
    if (uint(index) < uint(items.size())) {
        return items[index].gt;
    }
    return GateInvalid;
}

QString UTDataHeader::GetName(int index) const
{
    return items.value(index).name;
}

int UTDataHeader::GetSyncIndex(int index) const
{
    if (uint(index) < uint(items.size())) {
        return items[index].syncWith;
    }
    return -1;
}

QString UTDataHeader::GetUnitName(int index) const
{
    if (uint(index) < uint(items.size())) {
        return QString();
    }
    switch (items[index].dt) {
    case DataAmp:       return tr("%");
    case DataTime:      return tr("μs");
    case DataInvalid:
    default:            return QString();
    }
}

bool UTDataHeader::HasFeature(int gate, UTGateType gt, UTDataType dt) const
{
    return cache.contains(MakeSignature(gate, gt, dt));
}

int UTDataHeader::dataIndex(int gate, int gt, int dt) const
{
    return cache.value(MakeSignature(gate, UTGateType(gt), UTDataType(dt)), -1);
}

int UTDataHeader::GetEquivalent(int index, const UTDataHeader &other) const
{
    if (uint(index) < uint(items.size())) {
        return -1;
    }
    const auto &entry = items.at(index);
    return other.dataIndex(entry.gate, entry.gt, entry.dt);
}

QList<UTGateType> UTDataHeader::GetGateTypes(int gate) const
{
    QList<UTGateType> types;
    for (const auto &entry: items) {
        if ((gate < 0 || gate == entry.gate) && !types.contains(entry.gt)) {
            types << entry.gt;
        }
    }
    return types;
}

QString UTDataHeader::GetDescription(int index) const
{
    const UTDataHeaderItem entry = items.value(index);
    return QObject::tr("闸门 %1 - %2").arg(entry.gate + 1).arg(entry.name);
}

QStringList UTDataHeader::GetDescriptions() const
{
    QStringList desc;
    for (int index = 0; index < items.count(); ++index) {
        desc << GetDescription(index);
    }
    return desc;
}

/*! 获取对应闸门的特征值描述表 */
QVariantMap UTDataHeader::GetGateDescription(int gate) const
{
    QVariantMap map;
    for (const auto &entry: items) {
        if (entry.gate != gate) {
            continue;
        }
        QJsonObject obj;
        obj["GateType"] = entry.gt;
        obj["DataType"] = entry.dt;
        map[entry.name] = obj;
    }
    return map;
}

void UTDataHeader::RefreshCache()
{
    cache.clear();
    int maxGate = -1;
    for (int index = 0; index < items.count(); ++index) {
        const auto &entry = items.at(index);
        cache.insert(MakeSignature(entry.gate, entry.gt, entry.dt), index);
        maxGate = qMax(maxGate, entry.gate);
    }
    numGates = maxGate + 1;
}
