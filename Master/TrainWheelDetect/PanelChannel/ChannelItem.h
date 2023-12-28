#ifndef CHANNELITEM_H
#define CHANNELITEM_H

#include <utils/treemodel.h>

class ChannelItem : public Utils::TypedTreeItem<ChannelItem>
{
    enum Columns {
        ColChannel
    };

public:
    QVariant data(int column, int role) const;
    Qt::ItemFlags flags(int column) const;

    int channel() const {return channelId;}
    void setChannel(int channel);

private:
    int channelId = 0;
};

#endif // CHANNELITEM_H
