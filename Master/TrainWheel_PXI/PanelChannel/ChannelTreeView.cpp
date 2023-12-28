#include "ChannelTreeView.h"
#include <QTextCodec>
#include <QDirModel>
#include <QSortFilterProxyModel>
#include "Platform/Platform.h"
#include "ChannelManager/ChannelManager.h"
#include "AbstractModel/ChannelItemModel.h"
#include "ChannelManager/ChannelManager.h"
#include "Platform/Platform.h"
#include "AbstractModel/AbstractItem.h"

#include "AbstractModel/ChannelItem.h"
#include "AbstractModel/ItemModel.h"
#include <QApplication>
#include <QMimeData>
#include <QHeaderView>
#include <QDragMoveEvent>
#include <QDrag>
#include <QDebug>
#include <QTextCodec>

ChannelTreeView::ChannelTreeView(QWidget *parent) :
    QTreeView(parent),
    m_ChannelNo(10000),
    m_IsFlaw(0)
{
    platform()->GetChannelManager()->OnReadFile("./ChannelList");
    Init();
    SetModel();
    expandAll();
}

/*!
 * \brief ChannelTreeView::Init 初始化
 */
void ChannelTreeView::Init()
{
//   setHeaderHidden(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);  // 设置行选择
    setEditTriggers(QAbstractItemView::NoEditTriggers);

}
/*!
 * \brief ChannelTreeView::SetModel 设置当前TreeView的Model，使用代理Model实现动态过滤，设置SelectModel以便获取其选中数据
 */
void ChannelTreeView::SetModel()
{
    ChannelItemModel *pChannelModel = platform()->GetChannelManager()->GetModel();

    if (pChannelModel == NULL)
    {
        qDebug() << "ChannelTreeView::SetModel()" << "ChannelModel is null";
        return;
    }

    setModel(pChannelModel->GetFilterProxyModel());
    setSelectionModel(pChannelModel->GetSelectionModel());
    int columnCount = pChannelModel->GetColumnCount();

    for (int index = 0; index < columnCount; ++index)
    {
        setColumnWidth(index, 120);
    }
}
/*!
 * \brief ChannelTreeView::GetModel 获取TreeView的Model时返回其代理Model
 * \return
 */
ChannelFilterProxyModel *ChannelTreeView::GetModel() const
{
    return dynamic_cast<ChannelFilterProxyModel *>(model());
}

int ChannelTreeView::GetBindChannel(int channelNO, bool flawOrSurface)
{
    Q_UNUSED(channelNO);
    Q_UNUSED(flawOrSurface);
    return 0;
}
/*!
 * \brief ChannelTreeView::selectionChanged 选中Item时将所选通道号发出
 * \param selected
 * \param deselected
 */
void ChannelTreeView::selectionChanged(const QItemSelection &selected,
                      const QItemSelection &deselected)
{
    Q_UNUSED(deselected);

    QModelIndexList listSelected = selected.indexes();
    ChannelFilterProxyModel *pModel = GetModel();
    if (NULL == pModel)
    {
        return;
    }
    foreach (QModelIndex indexSelected, listSelected)
    {
        ChannelItemModel *pChannelModel = platform()->GetChannelManager()->GetModel();

        if (pChannelModel == NULL)
        {
            qDebug() << "ChannelTreeView::SetModel()" << "ChannelModel is null";
            return;
        }

        ChannelItem *pItem = dynamic_cast<ChannelItem *>(pChannelModel->GetItemFromIndex(pModel->mapToSource(indexSelected)));
        if (pItem != NULL)
        {
            Channel channel = pItem->GetChannel();
            QString strName = channel.GetName();

            bool bOk = true;
            m_ChannelNo = strName.toInt(&bOk);
            if (bOk)
            {
              //  QTextCodec *gbkCode = QTextCodec::codecForName("GB2312");
                ChannelItem *item = dynamic_cast<ChannelItem *>(pItem->GetParent());
                Channel cha = item->GetChannel();
                QString str = cha.GetName();
                WaveType type(WaveSurface);

                if (str == "界面波")
                {
                    type = WaveSurface;
                }
                else if (str == "伤波")
                {
                    type = WaveFlaw;
                }
                else
                {
                    qDebug() << "ChannelTreeView::selectionChanged(): WaveType error !";
                }

                emit SendSelectedChannelNo(m_ChannelNo, type);
            }

            return;
        }
    }

}
