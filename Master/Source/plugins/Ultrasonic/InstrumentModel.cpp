#include "InstrumentModel.h"
#include "UltrasonicManager.h"

InstrumentModel::InstrumentModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    connect(UltrasonicManager::instance(), &UltrasonicManager::instrumentsChanged, this, &InstrumentModel::resetModel);
    resetModel();
}

QVariant InstrumentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case ColName:       return tr("名称");
        case ColDevice:     return tr("仪器类型");
        case ColConnection: return tr("连接状态");
        }
    }
    return QVariant();
}

int InstrumentModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return UltrasonicManager::instruments().count();
}

int InstrumentModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return QMetaEnum::fromType<Columns>().keyCount();
}

QVariant InstrumentModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    UTInstrument *ins = instrument(index);
    if (ins) {
        if (index.column() == ColName) {
            if (role == Qt::DisplayRole) {
                return ins->name();
            } else if (role == Qt::EditRole) {
                return ins->config().name;
            }
        } else if (index.column() == ColDevice && role == Qt::DisplayRole) {
            return ins->config().deviceType;
        } else if (index.column() == ColConnection && role == Qt::DisplayRole) {
            return ins->instrumentStatus().text();
        }
    }
    return QVariant();
}

UTInstrument *InstrumentModel::instrument(const QModelIndex &index) const
{
    if (index.model() != this || !index.isValid()) {
        return nullptr;
    }
    const auto instruments = UltrasonicManager::instruments();
    return instruments.value(index.row());
}

void InstrumentModel::resetModel()
{
    beginResetModel();
    for (auto *instrument: UltrasonicManager::instruments()) {
        connect(instrument, &UTInstrument::configChanged, this, &InstrumentModel::updateInstrumentInfo, Qt::UniqueConnection);
        connect(instrument, &UTInstrument::instrumentStatusChanged, this, &InstrumentModel::updateInstrumentInfo, Qt::UniqueConnection);
    }
    endResetModel();
}

void InstrumentModel::updateInstrumentInfo()
{
    auto *instrument = qobject_cast<UTInstrument *>(sender());
    if (!instrument) {
        return;
    }
    int row = UltrasonicManager::instruments().indexOf(instrument);
    emit dataChanged(index(row, 0, QModelIndex()), index(row, columnCount() - 1, QModelIndex()));
}
