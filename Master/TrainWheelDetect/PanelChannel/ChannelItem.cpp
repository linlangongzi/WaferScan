#include "ChannelItem.h"

QVariant ChannelItem::data(int column, int role) const
{
    if (column == ColChannel && role == Qt::DisplayRole) {
        return channelId;
    }

    return QVariant();
}

Qt::ItemFlags ChannelItem::flags(int column) const
{
    Q_UNUSED(column);
    return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void ChannelItem::setChannel(int channel)
{
    channelId = channel;
}
