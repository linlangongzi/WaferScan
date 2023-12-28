#include "AxisModel.h"
#include "MotionManager.h"

AxisModel::AxisModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    connect(MotionManager::instance(), &MotionManager::axisListChanged, this, &AxisModel::resetModel);
    resetModel();
}

QVariant AxisModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case ColUuid:   return tr("ID");
        case ColName:   return tr("名称");
        case ColType:   return tr("类型");
        default:        return QString();
        }
    }
    return QVariant();
}

int AxisModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return MotionManager::axisCount();
}

int AxisModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return QMetaEnum::fromType<Columns>().keyCount();
}

QVariant AxisModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    Axis *axis = MotionManager::axis(index.row());
    if (!axis) {
        return QVariant();
    }

    if (role == UuidRole) {
        return axis->uuid();
    } else if (role == AxisObjectRole) {
        return QVariant::fromValue(axis);
    }
    if (index.column() == ColUuid && role == Qt::DisplayRole) {
        return axis->uuid();
    } else if (index.column() == ColName && role == Qt::DisplayRole) {
        return axis->config().name;
    } else if (index.column() == ColType && role == Qt::DisplayRole) {
        return EnumToKey(axis->config().axisType);
    }
    return QVariant();
}

Axis *AxisModel::axis(const QModelIndex &index) const
{
    if (!index.isValid() || index.parent().isValid()) {
        return nullptr;
    }
    return MotionManager::axis(index.row());
}

void AxisModel::resetModel()
{
    beginResetModel();
    for (auto *axis: MotionManager::axisList()) {
        connect(axis, &Axis::configChanged, this, &AxisModel::updateAxisInfo, Qt::UniqueConnection);
    }
    endResetModel();
}

void AxisModel::updateAxisInfo()
{
    auto *axis = qobject_cast<Axis *>(sender());
    if (!axis) {
        return;
    }
    int row = MotionManager::axisList().indexOf(axis);
    emit dataChanged(index(row, 0, QModelIndex()), index(row, columnCount() - 1, QModelIndex()));
}
