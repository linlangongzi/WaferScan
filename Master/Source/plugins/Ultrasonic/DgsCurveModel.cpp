#include "DgsCurveModel.h"
#include <QLoggingCategory>
#include <QMessageBox>

static Q_LOGGING_CATEGORY(debug, "ut.dgsmodel")

QVariant DgsCurvePointItem::data(int column, int role) const
{
    if (column == ColDepth) {
        if (role == Qt::DisplayRole) {
            return tr("%1 mm").arg(depth);
        } else if (role == Qt::EditRole) {
            return depth;
        }
    }
    if (column == ColGain) {
        if (role == Qt::DisplayRole) {
            return tr("%1 dB").arg(gain);
        } else if (role == Qt::EditRole) {
            return gain;
        }
    }
    return {};
}

bool DgsCurvePointItem::setData(int column, const QVariant &data, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }
    if (column == ColDepth) {
        qreal newDepth = qMax<qreal>(0.1, data.toReal());
        int row = parent()->indexOf(this);
        if (row - 1 >= 0) {
            auto *previous = parent()->childAt(row - 1);
            if (previous && previous->depth > newDepth) {
                return false;
            }
        }
        auto *next = parent()->childAt(row + 1);
        if (next && next->depth < newDepth) {
            return false;
        }

        depth = newDepth;
        parent()->notifyModel();
        return true;
    }
    if (column == ColGain) {
        gain = data.toReal();
        parent()->notifyModel();
        return true;
    }
    return false;
}

Qt::ItemFlags DgsCurvePointItem::flags(int column) const
{
    Q_UNUSED(column);
    auto flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    return flags;
}

QVariant DgsCurveItem::data(int column, int role) const
{
    if (column == ColDiameter) {
        if (role == Qt::DisplayRole) {
            return tr("Φ%1").arg(dgsCurve().diameter);
        } else if (role == Qt::EditRole) {
            return dgsCurve().diameter;
        }
    }
    return {};
}

bool DgsCurveItem::setData(int column, const QVariant &data, int role)
{
    if (role != Qt::EditRole) {
        return false;
    }
    if (column == ColDiameter) {
        auto dgs = dgsCurve();
        dgs.diameter = data.toReal();
        setDgsCurve(dgs);
        return true;
    }
    return false;
}

Qt::ItemFlags DgsCurveItem::flags(int column) const
{
    Q_UNUSED(column)
    auto flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
//    if (column == ColDiameter) {
//        flags = flags | Qt::ItemIsEditable;
//    }
    return flags;
}

DgsCurve DgsCurveItem::dgsCurve() const
{
    DgsCurve data;
    data.diameter = m_diameter;
    forAllChildren([&] (DgsCurvePointItem *child) {
        data.interpolateTable.insert(child->depth, child->gain);
    });
    return data;
}

void DgsCurveItem::notifyModel()
{
    auto *m = qobject_cast<DgsCurveModel *>(model());
    if (m) {
        m->notifyDgsCurvesChanged();
    }
}

void DgsCurveItem::setDgsCurve(const DgsCurve &curve)
{
    if (dgsCurve() == curve) {
        return;
    }
    m_diameter = curve.diameter;

    const auto &table = curve.interpolateTable;
    while (childCount() > table.count()) {
        removeChildAt(childCount() - 1);
    }
    while (childCount() < table.count()) {
        appendChild(new DgsCurvePointItem);
    }
    Q_ASSERT(childCount() == table.count());
    const auto &keyList = table.keys();
    const auto &valueList = table.values();
    for (int index = 0; index < childCount(); ++index) {
        childAt(index)->depth = keyList.value(index);
        childAt(index)->gain = valueList.value(index);
    }

    notifyModel();
}

DgsCurveModel::DgsCurveModel(QObject *parent) :
    Utils::TreeModel<DgsCurveRootItem, DgsCurveItem, DgsCurvePointItem>(parent)
{
    setHeader({tr("深度"), tr("增益")});
}

QList<DgsCurve> DgsCurveModel::dgsCurves() const
{
    QList<DgsCurve> data;
    forItemsAtLevel<1>([&] (DgsCurveItem *item) {
        data << item->dgsCurve();
    });
    return data;
}

void DgsCurveModel::notifyDgsCurvesChanged()
{
    emit dgsCurvesChanged(dgsCurves());
}

void DgsCurveModel::setDgsCurves(const QList<DgsCurve> &curves)
{
    if (dgsCurves() == curves) {
        return;
    }

    // Create insufficient items.
    while (rootItem()->childCount() < curves.count()) {
        rootItem()->appendChild(new DgsCurveItem);
    }
    // Remove abundant items.
    while (rootItem()->childCount() > curves.count()) {
        rootItem()->removeChildAt(rootItem()->childCount() - 1);
    }
    Q_ASSERT(rootItem()->childCount() == curves.count());
    // Update all items.
    for (int index = 0; index < rootItem()->childCount(); ++index) {
        rootItem()->childAt(index)->setDgsCurve(curves[index]);
    }
    emit dgsCurvesChanged(curves);
}
