#include "itemmodel.h"
#include <QDebug>

ItemModel::ItemModel(QObject *parent) :
    QAbstractItemModel(parent),
    root(NULL)
{
}

/*!
 * \brief   设置model 的根节点
 * \param   model 的根节点的值
*/
void ItemModel::SetRootItem(AbstractItem *item)
{
    if (item != NULL)
    {
        if (root != NULL)
        {
            delete root;
            root = NULL;
        }

        root = item;
    }
}

/*!
 * \brief ItemModel::GetRootItem 获得根节点
 * \return 根节点
 */
AbstractItem *ItemModel::GetRootItem()
{
    return root;
}

/*!
 * \brief   设置index 对应项的父节点的项标志 （必须重载函数）
 * \param   index 查询项的项标志
*/
QModelIndex ItemModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    AbstractItem *child = GetItemFromIndex(index);
    if (child == NULL)
    {
        return QModelIndex();
    }

    AbstractItem *parent = dynamic_cast<AbstractItem *>(child->parent());
    if (parent == NULL || parent == root)
    {
        return QModelIndex();
    }

    AbstractItem* grandParent = dynamic_cast<AbstractItem *>(parent->parent());
    if (grandParent == NULL)
    {
        return QModelIndex();
    }

    int row = grandParent->GetChildren().indexOf(parent);
    return createIndex(row, 0, parent);
}

/*!
 * \brief   根据index 获得对应的Item
 * \param   index     项标志
*/
AbstractItem *ItemModel::GetItemFromIndex(const QModelIndex &index) const
{
    if (index.isValid())
    {
        AbstractItem *curItem = static_cast<AbstractItem *>(index.internalPointer());
        return curItem;
    }
    return root;
}

/*!
 * \brief ItemModel::GetIndexByItem 通过Item和列号创建对应的Index
 * \param item 目标Item的指针
 * \param col  列号
 * \return 返回对应的Index
 */

QModelIndex ItemModel::GetIndexByItem(AbstractItem *item, int col)
{
    if (item == NULL || item == root)
    {
        return QModelIndex();
    }
    AbstractItem* parent = dynamic_cast<AbstractItem *>(item->parent());
    if (parent == NULL)
    {
        return QModelIndex();
    }
    int row = parent->GetChildren().indexOf(item);
    return createIndex(row, col, item);
}

int ItemModel::GetColumnCount()
{
    if (root != NULL)
    {
        return root->ColumnCount();
    }

    return -1;
}

int ItemModel::GetMaxDepth()
{
    return MAX_DEPTH;
}

/*!
 * \brief ItemModel::Reset 删除root下面所有节点
 */
void ItemModel::Reset()
{
    if (root == NULL)
    {
        return;
    }

    if (root->GetNumChildren() > 0)
    {
        beginResetModel();
        foreach (AbstractItem *pItem, root->GetChildren())
        {
            if (pItem != NULL)
            {
                root->RemoveChild(pItem);
            }
        }
        endResetModel();
    }
}

/*!
 * \brief   返回对应项的项标志index （ 为必须重载函数 ）
 * \param   row 行号（ 相对量，是在父节点中的行号 ）
 *          column 列号（ 相对量，是在父节点中的列号 ）
*/
QModelIndex ItemModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }
    if (!parent.isValid())
    {
        if (root == NULL)
        {
            return QModelIndex();
        }
        return createIndex(row, column, root->GetChild(row));
    }

    AbstractItem *curItem = GetItemFromIndex(parent);
    if (curItem != NULL )
    {
        return createIndex(row, column, curItem->GetChild(row));
    }
    return QModelIndex();
}

/*!
 * \brief   设置index 项对应role的数值
 * \param   index 设置项的项标志
 *          value 设置的数值
 *          role  设置的角色
*/
bool ItemModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
    {
        return false;
    }

    AbstractItem *curItem = GetItemFromIndex(index);
    if (curItem != NULL)
    {
        return curItem->SetData(index, value, role);
    }

    return QAbstractItemModel::setData(index, value, role);
}

/*!
 * \brief   设置index 项的标志（标志决定是否可以编辑、是否可选、是否可以拖动、是否可以放置等等）
 * \param   index 项标志
*/
Qt::ItemFlags ItemModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flag = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if (index.isValid())  // 本身不是root
    {
        if (index.column() == 0)    // 第0列可以设置edit
        {
            flag |= Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
        }

        AbstractItem* item = GetItemFromIndex(index);
        Q_ASSERT_X(item != NULL, "ItemModel::flags()", "item is null");

        if (item->GetDepth() < MAX_DEPTH)   // 只有小于最大深度 2 才允许放
        {
            flag |= Qt::ItemIsDropEnabled;
        }

        if (item->GetNumChildren() == 0)
        {
            return Qt::ItemIsDragEnabled | flag;
        }
        else
        {
            return Qt::ItemIsDropEnabled | flag;
        }
    }
    else
    {
        return Qt::ItemIsDropEnabled | flag;
    }
}

/*!
 * \brief   删除行（ removeRow 是通过对count 设置为1实现，这一点与其他类型中的实现方式相反，所以只要model涉及到删除项，必须重载）
 * \param   row 行号
 *          count 行数量
 *          parent 父节点标志
*/
bool ItemModel::removeRows(int row, int count, const QModelIndex & parent/* = QModelIndex()*/)
{
    AbstractItem *parentItem = GetItemFromIndex(parent);
    if ((parentItem != NULL) && parentItem->GetNumChildren() >= (row + count))
    {
        for (int i = 0; i < count; i++)
        {
            AbstractItem *child = parentItem->GetChild(row + i);
         //   qDebug() << "removeRow child" << child << child->Get("Name");
            if (child != NULL)
            {
                beginRemoveRows(parent, row, row + count - 1); // 实现model 的项删除时，必须以beginRemoveRows 开头，
                parentItem->RemoveChild(child);
                endRemoveRows();                               // endRemoveRows 结尾，这样model 就会自动刷新数据在界面上,最主要是内部更新index
            }
        }
        return true;
    }
    return false;
}

/*!
 * \brief   移动行（ moveRow 是通过对count 设置为1实现，这一点与其他类型中的实现方式相反，所以只要model涉及到移动项，必须重载）
 * \param   sourceParent 移动项的父节点项标志
 *          sourceRow 移动项的行号
 *          count 行数
 *          destinationParent 目标节点的父节点项标志
 *          destinationChild 目标节点位置
*/
bool ItemModel::moveRows(const QModelIndex & sourceParent, int sourceRow, int count, const QModelIndex & destinationParent, int destinationChild)
{
    AbstractItem *sourceParentItem = GetItemFromIndex(sourceParent);
    AbstractItem *destinationParentItem = GetItemFromIndex(destinationParent);
    if (sourceParentItem != NULL && destinationParentItem != NULL && sourceParentItem->GetNumChildren() >= sourceRow + count)
    {
        for (int i = 0; i < count; ++i)
        {
            AbstractItem *child = sourceParentItem->GetChild(sourceRow + i);
            if (child != NULL)
            {
                beginMoveRows(sourceParent, sourceRow + i, sourceRow + i, destinationParent, destinationChild);
                sourceParentItem->DetachChild(child);
                if (sourceParentItem == destinationParentItem)  // 同一个父节点下面的移动，位置要前移一个，因为chidlren 在detachchild 的时候被移除了一个
                {
                    if (sourceRow > destinationChild)
                    {
                        destinationParentItem->AttachChild(child, destinationChild + i);
                    }
                    else
                    {
                        destinationParentItem->AttachChild(child, destinationChild + i - 1);
                    }
                }
                else
                {
                    destinationParentItem->AttachChild(child, destinationChild + i);
                }
                endMoveRows();
            }
        }
        return true;
    }
    else
    {
        qWarning() << "ItemModel::moveRows(): input sourceParent or destinationParent is invalid";
        return false;
    }
}

/*!
 * \brief 插入新纪录
 * \param pAbstractItem 新纪录对应的Item指针
 * \param nRow 插入位置
 * \param parent 新纪录对应的父节点
 * \return  true: 插入成功
 *          false: 插入失败
 */
bool ItemModel::insertNewItem(AbstractItem *pAbstractItem, const QModelIndex &parent, int nRow/* = -1*/)
{
    AbstractItem* parentItem = GetItemFromIndex(parent);
    // wangbingfu modify on 2014-11-20 不判断parent.isValid()在没有节点时，只能传root的Index
    // root的GetIndexByItem返回的是QModelIndex(),parent.isValid()返回的是false
 //   if (parent.isValid() && parentItem != NULL && pAbstractItem != NULL)
    if (parentItem != NULL && pAbstractItem != NULL)
    {
        if (nRow < 0)
        {
            beginInsertRows(parent, parentItem->GetNumChildren(), parentItem->GetNumChildren());
            parentItem->AttachChild(pAbstractItem);
            endInsertRows();
        }
        else
        {
            beginInsertRows(parent, nRow, nRow);
            parentItem->AttachChild(pAbstractItem, nRow);
            endInsertRows();
        }
        return true;
    }
    return false;
}

/*!
 * \brief   设置parent 对应的项的列数( 此函数为必须重载函数 )
 * \param   parent 父节点对应的index
*/
int ItemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (root == NULL)
    {
        return 0;
    }
    return root->ColumnCount();
}

/*!
 * \brief   设置根据参数条件得到的表头
 * \param   section     列序号
 *          orientation 横竖标志
 *          role        角色（ 用来区分显示、图片、复选框等 ）
*/
QVariant ItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (root != NULL)
    {
        return root->HeaderData(section, orientation, role);
    }

    return QVariant();
}

/*!
 * \brief   设置index 对应项在 role 角色上的数据 （ 为必须重载函数 ）
 * \param   index     项标志
 *          role      角色
*/
QVariant ItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    AbstractItem *curItem = GetItemFromIndex(index);
    if (curItem != NULL && curItem != root)
    {
        return curItem->Data(index.column(), role);
    }
    return QVariant();
}

/*!
 * \brief   parent 对应项的子项行数 （ 为必须重载函数 ）
 * \param   parent     项标志
*/
int ItemModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        if (root == NULL)
        {
            return 0;
        }
        return root->GetNumChildren();
    }
    AbstractItem* parentItem = GetItemFromIndex(parent);
    if (parentItem != NULL)
    {
        return parentItem->GetNumChildren();
    }
    return 0;
}

/*!
 * \brief   设置支持drop 的Action
*/
Qt::DropActions	ItemModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

/*!
 * \brief   drag 的Action
*/
Qt::DropActions	ItemModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

/*!
 * \brief   指示由row/column/parent 指向的项的data 是否支持action 对应的拖拽（一般的简单拖拽处理，可以直接在次函数中实现）
 * \param   data 对应的mimedata数据
 *          action 对应的dropAction (与前面的supportedDropActions 的返回值一致)
 *          row 项的行号
 *          column 项的列号
 *          parent 项的父节点标志
*/
bool ItemModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    return QAbstractItemModel::dropMimeData(data, action, row, column, parent);
}

/*!
 * \brief   设置一行所有项的mimedata数据（dropMimeData 就是获取的这项数据，如果直接在这里实现拖拽功能，这数据就是拖拽处理的数据）
 * \param   indexes 某一行对用的所有列的index
*/
QMimeData *ItemModel::mimeData(const QModelIndexList &indexes) const
{
    return QAbstractItemModel::mimeData(indexes);
}

/*!
 * \brief   设置数据的多媒体类型（用于在dropMimeData 和mimeData 中的数据进行类型判断）
*/
QStringList ItemModel::mimeTypes() const
{
    return QAbstractItemModel::mimeTypes();
}

