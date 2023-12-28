#ifndef PANELDEFECT_H
#define PANELDEFECT_H

#include <QDockWidget>
#include <utils/treemodel.h>
#include <Defect/DefectItem.h>
#include <QGraphicsEllipseItem>
#include <Wheel/ControlPanel.h>
#include <QSortFilterProxyModel>

class WheelDataset;

namespace Ui {
class PanelDefect;
}

class PanelDefect : public QDockWidget
{
    Q_OBJECT

public:
    explicit PanelDefect(QWidget *parent = 0);
    ~PanelDefect();
    QImage renderWheels(qreal width);
    void resizeColumnWidth();
    Utils::TreeModel<DefectItem, DefectItem> *model();
    void setWheelDataset(WheelDataset *dataset);

public slots:
    void refreshDefects();
    void refreshWheelsText();
    void onWheelSetMaxChanged(int wheelSetMax);
    void treeCurrentItemChanged(const QModelIndex &index);
    void treeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void sceneSelectionChanged();
    void actionMenu();
    void actionDelete();
    void onTrainInfoChanged(const TrainInfo &info);
    void setCurrentWheel(int wheel);

private slots:
    void on_resize_clicked();
    void on_wheel_currentIndexChanged(int index);

private:
    void resizeEvent(QResizeEvent *);
    void resizeWheels();

private:
    Ui::PanelDefect *ui;
    Utils::TreeModel<DefectItem, DefectItem> defectModel;
    QSortFilterProxyModel *proxyModel;
    QGraphicsItemGroup *root = NULL;
    QList<QGraphicsEllipseItem *> wheelItems;
    bool isUpdatingSelection = false;
    WheelDataset *wheelDataset = NULL;
};

#endif // PANELDEFECT_H
