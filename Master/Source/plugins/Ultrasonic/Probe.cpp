#include "Probe.h"
#include <Motion/EndPoint.h>
#include <Motion/MotionManager.h>
#include <QMatrix4x4>
#include <Ultrasonic/UTPose.h>
#include <allrising/Util.h>
#include <QLoggingCategory>
#include <utils/algorithm.h>

static Q_LOGGING_CATEGORY(debug, "ut.probe");

Q_DECL_UNUSED static bool typeRegistered = Probe::registerType();
REGISTER_JSON_CONVERTER(ProbeSize)

static bool dgsTableSorter(const DgsTable &t1, const DgsTable &t2)
{
    return t1.uuid < t2.uuid;
};

Probe Probe::fromJson(const QJsonValue &value)
{
    QJsonObject obj = value.toObject();
    Probe p;
    JsonToGadget(value, Probe::staticMetaObject, &p);
    if (p.uuid.isNull()) {
        p.uuid = QUuid::createUuid();
        qCDebug(debug) << "Probe has null uuid, create" << p.uuid;
    }
    return p;
}

qreal Probe::pulseDuration() const
{
    return 1e3 / frequency / 2;
}

qreal Probe::effectFocalPos(qreal materialSpeed, qreal waterPathLength, const QString &type, qreal waterSpeed) const
{
    qreal focusLength = InchToMillimetre(focalInch);
    if (focusLength <= waterPathLength || qIsNaN(waterPathLength) || qIsNaN(materialSpeed)) {
        /* 如果焦点在工件上边及刚好在上表面 */
        if (type == "time") {
            return TimeFromDistance(focusLength, waterSpeed);
        } else {
            return focusLength;
        }
    } else {
        /* 如果焦点在工件上表面以下 */
        qreal focalDepthInWater = focusLength - waterPathLength;
        qreal focalDepthInMaterial = focalDepthInWater / (materialSpeed / waterSpeed);
        if (type == "time") {
            return TimeFromDistance(waterPathLength, waterSpeed) + TimeFromDistance(focalDepthInMaterial, materialSpeed);
        } else {
            return waterPathLength + focalDepthInMaterial;
        }
    }
}

QString Probe::focusText() const
{
    return tr("%1 英寸 (%2 mm)").arg(focalInch).arg(InchToMillimetre(focalInch));
}

QString Probe::crystalSizeText() const
{
    return tr("%1 英寸 (%2 mm)").arg(crystalSize).arg(InchToMillimetre(crystalSize));
}

QString Probe::focalTypeText() const
{
    MetaType meta = MetaType::fromType<FocalType>();
    return meta.gadgetKeyText(EnumToKey(focalType));
}

bool Probe::registerType()
{
    bool ret = registerJsonConverter<Probe>();
    MetaType::setTr(FocusPoint, tr("点聚焦"));
    MetaType::setTr(FocusLine, tr("线聚焦"));
    MetaType::setTr(FocusNone, tr("平探头"));
    return ret;
}

DgsTable Probe::defaultDgsTable() const
{
    if (defaultDgsTableUuid.isNull()) {
        return {};
    }
    return Utils::findOrDefault(dgsTables, [this] (const auto &table) {
        return table.uuid == defaultDgsTableUuid;
    });
}

void Probe::updateTable(const DgsTable &table)
{
    auto it = std::find_if(dgsTables.begin(), dgsTables.end(), [&] (const DgsTable &t) {
        return t.uuid == table.uuid;
    });
    if (it != dgsTables.end()) {
        *it = table;
    } else {
        dgsTables.append(table);
        qSort(dgsTables.begin(), dgsTables.end(), dgsTableSorter);
    }
}

void Probe::addTransform(QMatrix4x4 &m, const UTPose &pose, qreal waterPath) const
{
    Q_UNUSED(pose);
    m *= transform;
    if (qIsFinite(waterPath)) {
        waterPath -= emissionDelay;
        if (waterPath < 0) {
            waterPath = Q_SNAN;
        }
        m.translate(0, 0, -waterPath);
    }
}

QList<QMatrix4x4> Probe::transformChain(const UTPose &pose, qreal waterPath) const
{
    Q_UNUSED(pose);
    QList<QMatrix4x4> chain;
    QMatrix4x4 m;
    m *= transform;
    chain << m;
    if (qIsFinite(waterPath)) {
        waterPath -= emissionDelay;
        if (waterPath < 0) {
            waterPath = Q_SNAN;
        }
        m.translate(0, 0, -waterPath);
        chain << m;
    }
    return chain;
}

////////////////////////////////////////////////////////////////////////////////

QVariant ProbeItem::data(int column, int role) const
{
    switch (column) {
    case ColUuid:
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return probe.uuid;
        }
        break;
    case ColName:
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return probe.name;
        }
        break;
    case ColManufacture:
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return probe.manufactureName;
        }
        break;
    case ColFrequency:
        if (role == Qt::DisplayRole) {
            return tr("%1 MHz").arg(probe.frequency);
        } else if (role == Qt::EditRole) {
            return probe.frequency;
        }
        break;
    case ColFocalType:
        if (role == Qt::DisplayRole) {
            return probe.focalTypeText();
        } else if (role == Qt::EditRole) {
            return QVariant::fromValue(probe.focalType);
        }
        break;
    case ColFocus:
        if (role == Qt::DisplayRole) {
            if (probe.focalType == Probe::FocusNone) {
                return {};
            } else {
                return probe.focusText();
            }
        } else if (role == Qt::EditRole) {
            if (probe.focalType == Probe::FocusNone) {
                return 0;
            } else {
                return probe.focalInch;
            }
        }
        break;
    case ColCrystalSize:
        if (role == Qt::DisplayRole) {
            return probe.crystalSizeText();
        } else if (role == Qt::EditRole) {
            return probe.crystalSize;
        }
        break;
    default:
        break;
    }
    return {};
}

bool ProbeItem::setData(int column, const QVariant &data, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }
    if (column == ColUuid) {
        probe.uuid = data.toUuid();
    }
    if (column == ColName) {
        probe.name = data.toString();
    } else if (column == ColManufacture) {
        probe.manufactureName = data.toString();
    } else if (column == ColFrequency) {
        probe.frequency = data.toReal();
    } else if (column == ColFocalType) {
        probe.focalType = Probe::FocalType(data.toInt());
    } else if (column == ColFocus) {
        probe.focalInch = data.toReal();
    } else if (column == ColCrystalSize) {
        probe.crystalSize = data.toReal();
    } else {
        return false;
    }
    return true;
}

ProbeModel::ProbeModel(QObject *parent) :
    Utils::TreeModel<ProbeRootItem, ProbeItem>(parent)
{
    setHeader({tr("UUID"), tr("名称"), tr("生产商"), tr("频率"), tr("聚焦类型"), tr("焦距(水)"), tr("晶片大小")});

    connect(this, &QAbstractItemModel::columnsInserted, this, &ProbeModel::probesChanged);
    connect(this, &QAbstractItemModel::columnsMoved,    this, &ProbeModel::probesChanged);
    connect(this, &QAbstractItemModel::columnsRemoved,  this, &ProbeModel::probesChanged);
    connect(this, &QAbstractItemModel::dataChanged,     this, &ProbeModel::probesChanged);
    connect(this, &QAbstractItemModel::modelReset,      this, &ProbeModel::probesChanged);
    connect(this, &QAbstractItemModel::rowsInserted,    this, &ProbeModel::probesChanged);
    connect(this, &QAbstractItemModel::rowsMoved,       this, &ProbeModel::probesChanged);
    connect(this, &QAbstractItemModel::rowsRemoved,     this, &ProbeModel::probesChanged);
}

void ProbeModel::setProbes(const QList<Probe> &probes)
{
    clear();
    for (const auto &probe : probes) {
        auto *item = new ProbeItem;
        item->probe = probe;
        rootItem()->appendChild(item);
    }
}

QList<Probe> ProbeModel::probes() const
{
    QList<Probe> data;
    forItemsAtLevel<1>([&] (ProbeItem *item) {
        data << item->probe;
    });
    return data;
}

Probe ProbeModel::probe(const QUuid &uuid)
{
    auto *item = probeItem(uuid);
    return (item != nullptr) ? item->probe : Probe();
}

bool ProbeModel::updateProbe(const Probe &newProbe)
{
    auto *item = probeItem(newProbe.uuid);
    if (item) {
        item->probe = newProbe;
        item->update();
    }
    return (item != nullptr);
}

ProbeItem *ProbeModel::probeItem(const QUuid &uuid)
{
    return findItemAtLevel<1>([uuid] (ProbeItem *probeItem){
        return probeItem->probe.uuid == uuid;
    });
}
