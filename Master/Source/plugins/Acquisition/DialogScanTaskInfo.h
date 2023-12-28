#pragma once

#include <QDialog>
#include <QPointer>
#include <Acquisition/ScanTask.h>

namespace Ui {
class DialogScanTaskInfo;
}

class QLineEdit;
class ScanPlan;

class DialogScanTaskInfo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogScanTaskInfo(QWidget *parent = 0);
    ~DialogScanTaskInfo();

    void setTask(ScanTask *task);
    void setEditable(bool enable = false);

private slots:
    void on_buttonBox_accepted();
    void on_exportPlan_clicked();

private:
    void setScanPlan(const ScanPlan *plan);
    void setCustomInfo(QMap<QString, QString> customInfo);
    void addCustomInfo(const QString &name, const QString &key, const QString &value);
    ScanTaskData scanTaskData() const;
    void setScanTaskData(const ScanTaskData &data);

private:
    Ui::DialogScanTaskInfo *ui;
    QList<QLineEdit *> m_customInfoEdits;
    QWidget *m_widget = nullptr;
    QPointer<ScanTask> m_task;
};
