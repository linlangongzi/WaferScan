#pragma once

#include "CScanGraphicsItemManager.h"
#include <QGraphicsItem>
#include <QItemSelectionModel>

class DefectGraphicsItem;
class DefectItem;
class QGraphicsItem;
class QGraphicsSceneContextMenuEvent;
class AbstractItem;
class DefectItemModel;
class UTGraphicsScene;

class DefectManager : public CScanGraphicsItemManager
{
    Q_OBJECT

public:
    explicit DefectManager(QGraphicsItem *root, QObject *parent = nullptr);

public:
    void clear();

public slots:
    void setVisible(bool visible);
    void updateScene();
    void onItemSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onRowsInserted(const QModelIndex &parent, int first, int last);
    void onItemRemoved(const QModelIndex &parent, int first, int last);

protected slots:
    virtual void ConnectToLayer(int layerID);
    virtual void DisconnectToLayer(int layerID);

private:
    QModelIndexList selectedItems();

private slots:
    void onGraphicsSelectionChanged();

private:
    QMap<DefectItem *, DefectGraphicsItem *> itemMap;
    DefectItemModel *model = nullptr;
    QGraphicsItem *root = nullptr;
    bool isVisible = true;

};
