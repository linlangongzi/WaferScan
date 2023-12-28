#include "NamedPosition.h"

#include <QFont>

enum PositionColumn {
    ColName,
    ColPosition,
};

QVariant NamedPosition::data(int column, int role) const
{
    if (role == Qt::FontRole && isLoadingPosition) {
        QFont font;
        font.setBold(true);
        return font;
    }
    if (column == ColName) {
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return name;
        }
    } else if (column >= ColPosition) {
        int axisId = column - ColPosition;
        if (role == Qt::DisplayRole) {
            return pos.value(axisId);
        } else if (role == Qt::EditRole) {
            return pos.value(axisId);
        } else if (role == Qt::CheckStateRole) {
            return axis.hasAxis(axisId) ? Qt::Checked : Qt::Unchecked;
        }
    }
    return QVariant();
}

bool NamedPosition::setData(int column, const QVariant &data, int role)
{
    if (column == ColName && role == Qt::EditRole) {
        name = data.toString();
    } else if (column >= ColPosition) {
        int axisId = column - ColPosition;
        if (role == Qt::EditRole) {
            if (pos.count() < axisId) {
                pos.resize(axisId + 1);
            }
            pos[axisId] = data.toFloat();
        } else if (role == Qt::CheckStateRole) {
            axis.setAxis(axisId, data.toInt() == Qt::Checked);
        } else {
            return false;
        }
    } else {
        return false;
    }
    return true;
}

Qt::ItemFlags NamedPosition::flags(int column) const
{
    auto flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    if (column == ColName) {
        return flags;
    } else {
        return flags | Qt::ItemIsUserCheckable;
    }
}

NamedPosition *NamedPositionModel::findByName(const QString &name) const
{
    return findItemAtLevel<1>([&](NamedPosition *item) {
        return item->name == name;
    });
}

NamedPosition *NamedPositionModel::loadingPosition() const
{
    return findItemAtLevel<1>([](NamedPosition *item) {
        return item->isLoadingPosition;
    });
}
