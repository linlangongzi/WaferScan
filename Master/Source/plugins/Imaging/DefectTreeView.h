#pragma once

#include <QTreeView>
#include <QTableView>

class DefectItem;
class DefectItemModel;
class QGraphicsScene;
class QModelIndex;

class DefectTreeView : public QTableView
{
    Q_OBJECT

public:
    explicit DefectTreeView(QWidget *parent = nullptr);

public:
    void SetModel(int layerID);
    void setCurrentItem(int row, const QModelIndex &parent);
    void SetScene(QGraphicsScene *scene);
    DefectItemModel *defectModel() const;
    DefectItem *currentItem();

signals:
    void RequestLocate(QPointF location);
    void DefectRectUpdated(QRectF rect);

private slots:
    void OnCustomContextMenu(const QPoint &pos);
    void OnDoubleClicked(const QModelIndex &index);
    void Edit();
    void deleteSelectedItem();
    void Comment();
    void Clear();
    void LocateView();
    void Locate();
    void AddImage();

private:
    void Init();
    QModelIndexList selectItemsIndexes();
};
