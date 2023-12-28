#pragma once

#include "UTChannel.h"
#include "UltrasonicGlobal.h"
#include <QAbstractTableModel>

class ULTRASONIC_EXPORT ChannelModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Columns {
        ColId,
        ColName,
        ColInstrument,
        ColChannel,
        ColCycle,
    };
    Q_ENUM(Columns)

    enum DataRole {
        ChannelIdRole = Qt::EditRole,
    };

    explicit ChannelModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    UTChannel *channel(const QModelIndex &index) const;

private slots:
    void resetModel();
    void updateChannelInfo();
};
