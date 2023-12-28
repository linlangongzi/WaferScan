#include "AbstractItem.h"

AbstractItem::AbstractItem(QObject *parent) :
    QObject(parent)
{
}

void AbstractItem::FromJson(const QJsonObject &source)
{
    data = source;
    if (source.value("Children").isArray())
    {
        foreach (QJsonValue value, source.value("Children").toArray())
        {
            if (value.isObject())
            {
                AbstractItem *item = New(this);
                if (item != NULL)
                {
                    item->FromJson(value.toObject());
                    AttachChild(item);
                }
            }
        }
    }

}

const QJsonObject AbstractItem::ToJson()
{
    return data;
}

AbstractItem *AbstractItem::GetChild(int row)
{
    if (Q_LIKELY(row >= 0 && row < children.count()))
    {
        return children.value(row);
    }

    return NULL;
}

int AbstractItem::GetNumChildren()
{
    return children.count();
}

AbstractItem::ItemVector AbstractItem::GetChildren()
{
    return children;
}

void AbstractItem::SetDepth(int newDepth)
{
    data.insert(QString("Depth"), QJsonValue(newDepth));
}

int AbstractItem::GetDepth() const
{
    AbstractItem *parent = GetParent();

    if (parent == NULL)
    {
        return 0;
    }

    return parent->GetDepth() + 1;
}

void AbstractItem::RemoveChild(AbstractItem *item)
{
    if (item != NULL)
    {
        int index = children.indexOf(item);
        foreach(AbstractItem* child, item->GetChildren()) // 一旦删除节点，其子节点也需要删除
        {
            item->RemoveChild(child);
        }
        children.removeAt(index);
        delete item;                                // 从列表中移除基点之后，还需要将此节点析构
        item = NULL;
    }
}

void AbstractItem::AttachChild(AbstractItem *item, int index)
{
    if (item != NULL)
    {
        item->setParent(this);

        if (index < GetNumChildren() && index >= 0)
        {
            children.insert(index, item);    // 附加节点时，需要指定附加位置
        }
        else if (index >= GetNumChildren() || index < 0)
        {
            children.append(item);
        }
    }
}

void AbstractItem::DetachChild(AbstractItem *item)
{
    if (item != NULL)
    {
        int index = children.indexOf(item);
        children.removeAt(index);               // 分离节点只需要从列表中移除，不需要析构
    }
}

AbstractItem *AbstractItem::GetParent() const
{
    return dynamic_cast<AbstractItem *>(parent());
}

QVariant AbstractItem::Get(const QString &key)
{
    return data.value(key).toVariant();
}

QVariant AbstractItem::Get(char *key)
{
    return data.value(QString(key)).toVariant();
}

void AbstractItem::Set(const QString &key, const QVariant &value)
{
    data.insert(key, value.toJsonValue());
}

void AbstractItem::Set(char *key, const QVariant &value)
{
    data.insert(QString(key), value.toJsonValue());
}
