#include "ChannelModel.h"
#include "UltrasonicManager.h"

ChannelModel::ChannelModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    connect(UltrasonicManager::instance(), &UltrasonicManager::channelsChanged, this, &ChannelModel::resetModel);
    resetModel();
}

int ChannelModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return UltrasonicManager::channelCount();
}

int ChannelModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return QMetaEnum::fromType<Columns>().keyCount();
}

QVariant ChannelModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case ColId:         return tr("通道号");
        case ColName:       return tr("名称");
        case ColInstrument: return tr("仪器");
        case ColChannel:    return tr("物理通道");
        case ColCycle:      return tr("时间槽");
        }
    }
    return QVariant();
}

QVariant ChannelModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const QList<UTChannel *> channels = UltrasonicManager::channels();
    UTChannel *channel = channels.value(index.row());
    if (!channel) {
        return QVariant();
    }
    int col = index.column();

    /* 未绑定的通道显示为红色 */
    if (role == Qt::ForegroundRole) {
        return channel->instrument() ? QVariant() : QColor(Qt::red);
    }
    if (role == Qt::EditRole) {
        return channel->channelId();
    }
    if (col == ColId && role == Qt::DisplayRole) {
        return channel->channelId();
    } else if (col == ColName && role == Qt::DisplayRole) {
        return channel->name();
    } else if (col == ColInstrument && role == Qt::DisplayRole) {
        if (channel->instrument()) {
            return channel->instrument()->name();
        } else {
            return tr("%1 (不存在)").arg(channel->config().identifier.instrument);
        }
    } else if (col == ColChannel && role == Qt::DisplayRole) {
        return channel->config().identifier.channel;
    } else if (col == ColCycle && role == Qt::DisplayRole) {
        return channel->config().identifier.cycle;
    }
    return QVariant();
}

UTChannel *ChannelModel::channel(const QModelIndex &index) const
{
    if (index.model() != this || !index.isValid()) {
        return Q_NULLPTR;
    }
    const auto &channels = UltrasonicManager::channels();
    return channels.value(index.row());
}

void ChannelModel::resetModel()
{
    beginResetModel();
    for (auto *channel: UltrasonicManager::channels()) {
        connect(channel, &UTChannel::configChanged, this, &ChannelModel::updateChannelInfo, Qt::UniqueConnection);
    }
    endResetModel();
}

void ChannelModel::updateChannelInfo()
{
    auto *channel = qobject_cast<UTChannel *>(sender());
    if (!channel) {
        return;
    }
    int row = UltrasonicManager::channels().indexOf(channel);
    emit dataChanged(index(row, 0, QModelIndex()), index(row, columnCount() - 1, QModelIndex()));
}
