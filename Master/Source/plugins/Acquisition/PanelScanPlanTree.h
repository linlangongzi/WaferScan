#pragma once

#include <QDockWidget>
#include <QMenu>
#include <QMessageBox>
#include "ScanPlan/ScanPlanModel.h"
#include "DialogScanPlan.h"
#include "ProgressBarDelegate.h"
#include "EditDelegate.h"

namespace Ui {
class PanelScanPlanTree;
}

class PanelScanPlanTree : public QDockWidget
{
    Q_OBJECT

    typedef ConfigManager::Registry Registry;

public:
    explicit PanelScanPlanTree(QWidget *parent = 0);
    ~PanelScanPlanTree();

public:
    void SetModel();
    ScanPlan *GetCurrentScanPlan();

private:
    void InitUI();
    void AddScanPlan(const QString &motiontype);

private slots:
    void slotCustomContextMenu(const QPoint &);
    void OnDoubleClicked(const QModelIndex &index);
    void AddFolder();
    void AddLine();
    void AddRectangle();
    void AddCylinder();
    void AddDisk();
    void AddLocate();
    void RenameScanPlan();
    void EditScanPlan();
    void DelScanPlan();

private:
    Ui::PanelScanPlanTree *ui;
    const int NUM_COLUMN;
};
