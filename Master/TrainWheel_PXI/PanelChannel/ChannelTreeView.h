#ifndef CHANNELTREEVIEW_H
#define CHANNELTREEVIEW_H

#include <QTreeView>
#include <QPointer>
#include <QGraphicsScene>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include "AbstractModel/ChannelItem.h"

class ChannelItemModel;
class QModelIndex;
class QGraphicsScene;
class ChannelFilterProxyModel;


class ChannelTreeView : public QTreeView
{
    Q_OBJECT

public:
    enum WaveType
    {
        WaveSurface,
        WaveFlaw
    };

public:
    explicit ChannelTreeView(QWidget *parent = 0);

public:
    void SetModel();
    void SetCurrentItem(int row, const QModelIndex &parent);
    ChannelFilterProxyModel *GetModel() const;
    int GetBindChannel(int channelNO, bool flawOrSurface);

protected:
    void selectionChanged(const QItemSelection &selected,
                          const QItemSelection &deselected);
signals:
    void SendSelectedChannelNo(int channelNo, WaveType type);

private:
    void Init();

private:
    int m_ChannelNo;
    bool m_IsFlaw;

};

#endif // CHANNELTREEVIEW_H
