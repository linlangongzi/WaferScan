#pragma once

#include <QWidget>
#include "ScanPlanModel.h"

namespace Ui {
class PanelScanPlanControl;
}

class ScanPlan;
class ScanTask;
class ReportDlg;
class QWinTaskbarButton;

class PanelScanPlanControl : public QWidget
{
    Q_OBJECT

public:
    explicit PanelScanPlanControl(QWidget *parent = nullptr);
    ~PanelScanPlanControl() override;

public:
    void SetScanControllerEnable(bool enable);

private slots:
    bool checkController(ScanPlan *plan);
    bool checkPlan(ScanPlan *plan);
    ScanTask *createTask(ScanPlan *plan, ReportDlg *report);
    QList<ScanTask *> createTasks(ScanPlan *plan, ReportDlg *report);
    bool createDocument(ScanTask *task);
    void refreshScanTask();
    void OnControllerStatusChanged(int status);
    void on_actionQuickStart_triggered();
    void on_actionPause_triggered();
    void on_actionStop_triggered();
    void createActions();

//    void on_actionSetCurrent_triggered();
    void on_actionStart_triggered();
    void on_actionCreateTask_triggered();
    void on_actionEditTask_triggered();
    void on_actionRemoveTask_triggered();
    void refreshTaskButtons();
    void onTaskContextMenu(const QPoint &pos);
    void on_actionOpenSelectedFile_triggered();
    void on_actionClearTasks_triggered();

public:
    Ui::PanelScanPlanControl *ui;

private:
    int currentStatus;
#ifdef QT_WINEXTRAS_LIB
    QWinTaskbarButton *taskbar = nullptr;
#endif
};
