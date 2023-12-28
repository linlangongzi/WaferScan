#pragma once

#include "PipeGlobalConfig.h"

#include <QDialog>
#include <QListWidget>

namespace Ui {
class PipeModeEdit;
}

class QComboBox;

class PipeModeEdit : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(PipeModeConfig config READ config WRITE setConfig)

public:
    explicit PipeModeEdit(QWidget *parent = 0);
    ~PipeModeEdit();

    PipeModeConfig config() const;

public slots:
    void setConfig(PipeModeConfig config);

private slots:
    void on_bscan_itemDoubleClicked(QListWidgetItem *item);
    void on_addPlot_clicked();
    void on_removePlot_clicked();
    void on_addImage_clicked();
    void on_removeImage_clicked();
    void on_images_itemDoubleClicked(QListWidgetItem *item);

private:
    void addPlot(const PlotView &view);

private:
    Ui::PipeModeEdit *ui;
    PipeModeConfig m_config;
};
