#include "Channel.h"

Channel::Channel():
    m_strName("")
{
}

Channel::Channel(const Channel &other) :
    children(other.children),
    m_strName(other.m_strName)
{

}

/*!
 * \brief 获取通道列表
 * \return 通道列表
 */
ChannelList Channel::GetChildren() const
{
    return children;
}

QString Channel::GetName()
{
    return m_strName;
}
/*!
 * \brief 将通道对象转换为Json对象
 * \return Json对象
 */
QJsonObject Channel::ToJson() const
{
    QJsonObject obj;
    obj["Name"] = m_strName;

    return obj;
}

/*!
 * \brief 由Json对象构造通道对象
 * \param obj Json对象
 * \return 构造得到的通道对象
 */
Channel Channel::FromJson(const QJsonObject &obj)
{
    Channel channel;
    channel.children = ChannelList::FromJson(obj["Children"]);
    channel.m_strName = obj["Name"].toString();

    return channel;
}

QJsonArray ChannelList::ToJson() const
{
    QJsonArray array;
    for (int index = 0; index < count(); ++index)
    {
        array.append(at(index).ToJson());
    }

    QJsonObject obj;
    obj["ChannelList"] = array;
    QJsonArray arrayChannel;
    arrayChannel.append(obj);
    return arrayChannel;
}

ChannelList ChannelList::FromJson(const QJsonValue &value)
{
    QJsonArray array = value.toArray();
    ChannelList list;
    list.reserve(array.count());
    for (int index = 0; index < array.count(); ++index)
    {
        Channel channel = Channel::FromJson(array[index].toObject());
        list.append(channel);
    }
    return list;
}

