#pragma once

#include <QMouseEvent>
#include <QTreeView>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QHeaderView>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include "ScanPlanModel.h"
#include "PanelScanPlanControl.h"
#include "DialogScanPlanEdit.h"

/*!
 * \brief ScanPlanTreeView 是Model-View模型中与ScanPlanModel对应的View控件
 *
 * 警告: 这个控件会向MainWindow中导入一个Dialog对象。受QObject的析构顺序限制，此控件析构时
 * 将不会析构此Dialog，重复构造这个控件会导致内存溢出
 */
class ScanPlanTreeView : public QTreeView
{
    Q_OBJECT

    enum DialogMode {
        Edit,
        New,
    };

public:
    explicit ScanPlanTreeView(QWidget *parent = 0);
    ~ScanPlanTreeView();

public:
    ScanPlan *currentScanPlan();
    ScanPlan *scanPlanFromIndex(const QModelIndex &index) const;
    void initActions(PanelScanPlanControl *control);

public slots:
    void refreshActionsEnable(const QModelIndex &current, const QModelIndex &previous);

protected:
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dropEvent(QDropEvent *event);

private slots:
    void OnCustomContextMenu(const QPoint &);
    void OnDoubleClicked(const QModelIndex &index);
    void addFolder();
    void addRectangle();
    void addLegacyRectangle();
    void addLegacyCylinder();
    void addCylinder();
    void addCylinderRotation();
    void addDisk();
    void addLegacyDisk();
    void addDiskRotation();
    void addRing();
    void ImportCam();
    void RenameScanPlan();
    void EditScanPlan();
    void DelScanPlan();
    void OnDialogScanPlanAccepted(int result);

private:
    void initUi();
    void addLegacyScanPlan(ScanPlan::MotionType motionType);
    void addScanPlan(const QString &typeName);
    void setCurrentItem(int row, const QModelIndex &parent);
    void setActionsEnable(bool enabled);
    void resizeDialog();

private:
    static const int MARGIN = 2;                                                // 用来判断差距的常量
    DialogScanPlanEdit *dialogScanPlan = nullptr;
    DialogMode dialogMode = Edit;
    PanelScanPlanControl *m_control;
};
