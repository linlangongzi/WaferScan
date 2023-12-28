#ifndef CHANNELITEMMODEL_H
#define CHANNELITEMMODEL_H

#include "ItemModel.h"
#include <QDebug>
#include <QItemSelectionModel>
#include "ARS3D/Channel.h"
#include "AbstractModel/ChannelFilterProxyModel.h"
class ChannelItem;
class ChannelItemModel : public ItemModel
{
    Q_OBJECT
public:
    explicit ChannelItemModel(QObject *parent = 0);

signals:
    void InsertDefectItem(ChannelItem *pItem, ChannelItem *pParent);

public slots:

public:
    void FromChannelList(ChannelList list);
    void FromJson(const QJsonValue &value);
    QItemSelectionModel *GetSelectionModel();
    QJsonArray ToJson();
    ChannelList GetChannelList(AbstractItem* parent);

    ChannelFilterProxyModel *GetFilterProxyModel();
private:
    void InsertItem(ChannelItem *child, ChannelItem *parent);
    QItemSelectionModel *selectionModel;
    ChannelFilterProxyModel *m_pProxyModel;
};
#endif // CHANNELITEMMODEL_H
