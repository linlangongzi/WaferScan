#ifndef CHANNELFILTERPROXYMODEL_H
#define CHANNELFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class ChannelFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ChannelFilterProxyModel(QObject *parent = 0);

signals:

public slots:

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool isExistChildren(int source_row, const QModelIndex &index) const;
};

#endif // CHANNELFILTERPROXYMODEL_H
