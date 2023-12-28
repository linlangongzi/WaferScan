#include "ChannelFilterProxyModel.h"
#include "AbstractModel/ChannelItemModel.h"
ChannelFilterProxyModel::ChannelFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}
/*!
 * \brief ChannelFilterProxyModel::filterAcceptsRow 实现过滤，没有子节点则进行过滤
 * \param source_row
 * \param source_parent
 * \return
 */
bool ChannelFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
   bool bRet = isExistChildren(source_row, source_parent);
   if (!bRet)
   {
       bool filter = QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
       if (filter)
       {
            return true;
       }
       else
       {
           QModelIndex source_index =sourceModel()->index(source_row, 0, source_parent);
           for (int k = 0; k < sourceModel()->rowCount(source_index); ++k)
           {
               if (filterAcceptsRow(k, source_index))
               {
                   return true;
               }
           }
       }

       return false;
   }

   return true;
}
/*!
 * \brief ChannelFilterProxyModel::isExistChildren 判断所选Item是否有Children
 * \param source_row
 * \param index
 * \return
 */
bool ChannelFilterProxyModel::isExistChildren(int source_row, const QModelIndex &index) const
{
    QModelIndex source_index = sourceModel()->index(source_row, 0, index);
    if (sourceModel()->hasChildren(source_index))
    {
        return true;
    }

    return false;
}

