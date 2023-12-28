#include "ChannelItemModel.h"
#include "ChannelItem.h"
#include <QItemSelectionModel>

ChannelItemModel::ChannelItemModel(QObject *parent) :
    ItemModel(parent),
    selectionModel(new QItemSelectionModel(this, this)),
    m_pProxyModel(new ChannelFilterProxyModel(this))

{
    SetRootItem(new ChannelItem(this));

    if (m_pProxyModel != NULL)
    {
        m_pProxyModel->setSourceModel(this);
    }
}

/*!
 * \brief ChannelItemModel::FromChannelList 实现从UTChanelList中取数据，按照数据序列进行新建各级Item
 * \param list Channel数据列表
 */
void ChannelItemModel::FromChannelList(ChannelList list)
{
    ChannelItem *root = dynamic_cast<ChannelItem *>(GetRootItem());
    if (root != NULL)
    {
        if (list.count() <= 0)
        {
            return;
        }
        beginResetModel();
        foreach (Channel channel, list)
        {
            ChannelItem *pItem = new ChannelItem();
            pItem->SetChannel(channel);

            InsertItem(pItem, root);
        }
        endResetModel();
    }
    else
    {
        qWarning() << "ChannelItemModel::SetChannelList model's root is NULL";
        return;
    }
}

/*!
 * \brief ChannelItemModel::InsertItem 递归检测root是否有children，如果有新建子节点
 * \param child 子节点
 * \param parent 父节点
 */
void ChannelItemModel::InsertItem(ChannelItem *child, ChannelItem *parent)
{
    parent->AttachChild(child);
    foreach (Channel channel, child->GetChannel().GetChildren())
    {
        ChannelItem *pItem = new ChannelItem();
        pItem->SetChannel(channel);

        InsertItem(pItem, child);
    }
}

/*!
 * \brief ChannelItemModel::FromJson 从JSON中取值并设置Item的值
 * \param value JSON值
 */
void ChannelItemModel::FromJson(const QJsonValue &value)
{
    ChannelList list;
     list = ChannelList::FromJson(value);

     FromChannelList(list);
}
/*!
 * \brief QItemSelectionModel::GetSelectionModel
 * \return 返回当前ItemModel创建的SelectionMode
 */
QItemSelectionModel *ChannelItemModel::GetSelectionModel()
{
    return selectionModel;
}

/*!
 * \brief QItemSelectionModel::ToJson 反序列化成json
 * \return
 */
QJsonArray ChannelItemModel::ToJson()
{
    if (root != NULL)
    {
        ChannelList list = GetChannelList(root);
        return list.ToJson();
    }

    return QJsonArray();
}

ChannelList ChannelItemModel::GetChannelList(AbstractItem* parent)
{
    ChannelList list;

    foreach(AbstractItem* child, parent->GetChildren())
    {
        ChannelItem *childChannel = dynamic_cast<ChannelItem *>(child);
        list.append(childChannel->GetChannel());
    }

    return list;
}

ChannelFilterProxyModel *ChannelItemModel::GetFilterProxyModel()
{
    return m_pProxyModel;
}
