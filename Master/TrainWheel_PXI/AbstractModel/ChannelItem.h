#ifndef CHANNELITEM_H
#define CHANNELITEM_H

#include <QMap>
#include <QObject>
#include <QVariant>
#include <QModelIndex>
#include "AbstractModel/AbstractItem.h"
#include "ARS3D/Channel.h"

class ChannelItem : public AbstractItem
{
    Q_OBJECT

    friend class ChannelItemModel;

public:
    typedef QMap<int, QString> Headers;

public:
    virtual AbstractItem *New(QObject *parent) const;

protected:
    explicit ChannelItem(QObject *parent = 0);

public:
    virtual int ColumnCount();
    virtual Qt::ItemFlags Flags(int column);
    virtual QVariant Data(int column, int role);
    virtual bool SetData(const QModelIndex &index, const QVariant &value, int role);
    virtual QVariant HeaderData(int section, Qt::Orientation orientation, int role);
    void SetChannel(const Channel &channel);
    const Channel &GetChannel();

private:
    Headers headers;
    Channel channelData;
};

#endif // CHANNELITEM_H
