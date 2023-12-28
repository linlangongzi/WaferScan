#pragma once

#include <QAbstractTableModel>
#include "Axis.h"
#include "MotionGlobal.h"

class MOTION_EXPORT AxisModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Columns {
        ColUuid,
        ColName,
        ColType,
    };
    Q_ENUM(Columns)

    enum DataRole {
        UuidRole = Qt::EditRole,
        AxisObjectRole = Qt::UserRole,
    };

    explicit AxisModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Axis *axis(const QModelIndex &index) const;

private:
    void resetModel();
    void updateAxisInfo();
};
