#pragma once

#include "ScanPlan.h"

#include <QDialog>

namespace Ui {
class DialogScanPlanEdit;
}

class DialogScanPlanEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogScanPlanEdit(QWidget *parent = 0);
    ~DialogScanPlanEdit();

    void setNameReadonly(bool readOnly);

    ScanPlan *scanPlan() const;
    void setScanPlan(ScanPlan *scanPlan);

private:
    Ui::DialogScanPlanEdit *ui;
    ScanPlan *m_scan = nullptr;
    QWidget *m_widget = nullptr;
};
