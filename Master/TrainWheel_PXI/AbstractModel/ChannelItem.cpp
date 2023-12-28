#include "ChannelItem.h"
#include <QTextCodec>
AbstractItem *ChannelItem::New(QObject *parent) const
{
    return new ChannelItem(parent);
}

/*!
 * \brief 初始化Item每一列的头名称
 * \param parent
 */
ChannelItem::ChannelItem(QObject *parent) :
    AbstractItem(parent)
{
//    QTextCodec *gbkCode = QTextCodec::codecForName("GB2312");
    headers.insert(0, QString("通道管理"));
}

/*!
 * \brief  获取列数
 * \return
 */
int ChannelItem::ColumnCount()
{
    return headers.size();
}

Qt::ItemFlags ChannelItem::Flags(int column)
{
    Q_UNUSED(column);
    Qt::ItemFlags flags;
    return flags;
}

QVariant ChannelItem::Data(int column, int role)
{
    if (role == Qt::DisplayRole && column >= 0 && column < headers.size())
    {
        switch (column)
        {
        case 0:
            return channelData.GetName();

        default:
            return QVariant();
        }
    }

    return QVariant();
}

/*!
 * \brief 修改data,暂时没有修改功能
 * \param index
 * \param value
 * \param role
 * \return 暂时返回false
 */
bool ChannelItem::SetData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);

    return false;
}

/*!
 * \brief 返回 指定列 的头名称
 * \param section
 * \param orientation
 * \param role
 * \return
 */
QVariant ChannelItem::HeaderData(int section, Qt::Orientation orientation, int role)
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole    /* Role检查 */
        && section >=0 && section < headers.size())                 /* header表边界检查 */
    {
        return QVariant(headers.value(section));
    }

    return QVariant();
}

void ChannelItem::SetChannel(const Channel &channel)
{
    channelData = channel;
}

/*!
 * \brief ChannelItem::GetChannel 获得Chanel的指针
 * \return utchannel的指针
 */
const Channel &ChannelItem::GetChannel()
{
    return channelData;
}
