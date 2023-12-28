#include "DgsTable.h"
#include "UltrasonicManager.h"
#include <QLoggingCategory>
#include <QtMath>
#include <utils/algorithm.h>

static Q_LOGGING_CATEGORY(debug, "ut.dgs");

REGISTER_JSON_CONVERTER(DgsCurve)
REGISTER_JSON_CONVERTER(DgsTable)

static bool dgsCurveSorter(const DgsCurve &c1, const DgsCurve &c2)
{
    return c1.diameter < c2.diameter;
}

QString DgsTable::materialName() const
{
    return UltrasonicManager::materialModel()->material(materialUuid).name;
}

QString DgsTable::waterPathLengthText() const
{
    return tr("%1 mm").arg(waterPathLength);
}

QString DgsTable::depthRangeText() const
{
    if (dgsCurves.isEmpty()) {
        return QString();
    }
    qreal minDepth = std::numeric_limits<qreal>::max();
    qreal maxDepth = std::numeric_limits<qreal>::min();
    for (const auto &dgs : dgsCurves) {
        const auto &table = dgs.interpolateTable;
        for (auto iter = table.constBegin(); iter != table.constEnd(); ++iter) {
            minDepth = qMin(minDepth, iter.key());
            maxDepth = qMax(maxDepth, iter.key());
        }
    }
    return tr("%1 - %2").arg(minDepth).arg(maxDepth);
}

QString DgsTable::diameterRangeText() const
{
    if (dgsCurves.isEmpty()) {
        return QString();
    }
    qreal min = std::numeric_limits<qreal>::max();
    qreal max = std::numeric_limits<qreal>::min();
    for (const auto &dgs : dgsCurves) {
        min = qMin(min, dgs.diameter);
        max = qMax(max, dgs.diameter);
    }
    return tr("%1 - %2").arg(min).arg(max);
}

bool DgsTable::hasDgsCurve(qreal diameter) const
{
    return Utils::anyOf(dgsCurves, [&] (const auto &c) {
        return qFuzzyCompare(c.diameter, diameter);
    });
}

void DgsTable::updateDgsCurve(const DgsCurve &curve)
{
    auto it = std::find_if(dgsCurves.begin(), dgsCurves.end(), [&] (const auto &c) {
        return qFuzzyCompare(c.diameter, curve.diameter);
    });
    if (it != dgsCurves.end()) {
        *it = curve;
    } else {
        dgsCurves.append(curve);
        qSort(dgsCurves.begin(), dgsCurves.end(), dgsCurveSorter);
    }
}

QVariant DgsTableItem::data(int column, int role) const
{
    if (role == Qt::FontRole && table.uuid == dynamic_cast<DgsTableModel *>(this->model())->defaultTableUuid()) {
        QFont font;
        font.setBold(true);
        return font;
    }
    if (role != Qt::DisplayRole) {
        return {};
    }
    if (column == ColMaterialName) {
        return table.materialName();
    } else if (column == ColWaterPathLength) {
        return table.waterPathLengthText();
    } else if (column == ColDepthRange) {
        return table.depthRangeText();
    } else if (column == ColDiameter) {
        return table.diameterRangeText();
    }
    return {};
}

DgsTableModel::DgsTableModel(QObject *parent) :
    Utils::TreeModel<DgsTableRootItem, DgsTableItem> (parent)
{
    setHeader({tr("材料"), tr("水距"), tr("深度范围"), tr("平底孔范围")});
}

void DgsTableModel::setDgsTables(const QList<DgsTable> &tables)
{
    if (dgsTables() == tables) {
        return;
    }

    while (rootItem()->childCount() > tables.count()) {
        rootItem()->removeChildAt(rootItem()->childCount() - 1);
    }
    while (rootItem()->childCount() < tables.count()) {
        rootItem()->appendChild(new DgsTableItem);
    }
    Q_ASSERT(rootItem()->childCount() == tables.count());
    for (int index = 0; index < rootItem()->childCount(); ++index) {
        auto *item = rootItem()->childAt(index);
        item->table = tables[index];
        item->update();
    }
    emit dgsTablesChanged(tables);
}

QList<DgsTable> DgsTableModel::dgsTables() const
{
    QList<DgsTable> data;
    forItemsAtLevel<1>([&] (DgsTableItem *item) {
        data << item->table;
    });
    return data;
}

QUuid DgsTableModel::defaultTableUuid() const
{
    return m_defaultTableUuid;
}

DgsTable DgsTableModel::dgsTable(const QUuid &uuid)
{
    auto *item = findItemAtLevel<1>([&](DgsTableItem *dgsItem) {
        return dgsItem->table.uuid == uuid;
    });
    if (!item) {
        return {};
    }
    return item->table;
}

void DgsTableModel::setDefaultTableUuid(const QUuid &defaultTableUuid)
{
    if (m_defaultTableUuid == defaultTableUuid) {
        return;
    }

    m_defaultTableUuid = defaultTableUuid;
    rootItem()->updateAll();
    emit defaultTableUuidChanged(m_defaultTableUuid);
}
