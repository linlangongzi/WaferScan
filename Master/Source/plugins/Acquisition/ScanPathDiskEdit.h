#pragma once

#include <QWidget>
#include <QVector3D>
#include "ScanPathDisk.h"

class Axis;

namespace Ui {
class ScanPathDiskEdit;
}

class ScanPathDiskEdit : public QWidget
{
    Q_OBJECT

public:
    explicit ScanPathDiskEdit(QWidget *parent = 0);
    ~ScanPathDiskEdit();
    void setOptions(const ScanPathDiskOptions &options);
    ScanPathDiskOptions options() const;
    void setSetupVisibility(bool enable);

private slots:
    void on_setup_clicked();
    void refreshSpeed();
    void refreshOffset();
    void on_coordinateSystem_currentIndexChanged(int index);
    void on_scanAxis_currentIndexChanged(int index);
    void on_useSuggestedSpeed_clicked();
    void on_axisRotationEnable_toggled(bool checked);
    void on_axesRotationLimited_currentIndexChanged(int index);
    void on_angularSpeed_valueChanged(double arg1);

private:
    qreal suggestedSpeed() const;
    Axis *liftingAxis() const;
    QVector3D calculateOffset() const;
    ScanPathDiskOptions::CoordinateSystem coordinateSystem() const;

private:
    Ui::ScanPathDiskEdit *ui;
    UTSetup m_setup;
};
