#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QAbstractItemModel>
#include <QMouseEvent>
#include "AbstractItem.h"

class ItemModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ItemModel(QObject *parent = 0);

public:
    virtual QModelIndex parent(const QModelIndex &index) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    virtual bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
    virtual bool moveRows(const QModelIndex & sourceParent, int sourceRow, int count, const QModelIndex & destinationParent, int destinationChild);
    bool insertNewItem(AbstractItem *pAbstractItem, const QModelIndex & parent = QModelIndex(), int nRow = -1);
    AbstractItem *GetItemFromIndex(const QModelIndex &index) const;
    QModelIndex GetIndexByItem(AbstractItem *item, int col = 0);
    void SetRootItem(AbstractItem *item);
    AbstractItem *GetRootItem();
    int GetColumnCount();
    int GetMaxDepth();
    void Reset();

private:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual Qt::DropActions	supportedDragActions() const;
    virtual Qt::DropActions	supportedDropActions() const;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
    virtual QStringList mimeTypes() const;

protected:
    AbstractItem *root;
    const int MAX_DEPTH = 2;

};

#endif // ITEMMODEL_H
