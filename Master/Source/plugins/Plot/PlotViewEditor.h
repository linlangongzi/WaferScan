#pragma once

#include <QDialog>
#include <Plot/PlotConfig.h>

class QListWidgetItem;

namespace Ui {
class PlotViewEditor;
}

class PLOT_EXPORT PlotViewEditor : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(PlotView option READ option WRITE setOption NOTIFY optionChanged)

public:
    explicit PlotViewEditor(QWidget *parent = 0);
    ~PlotViewEditor();

    PlotView option() const;

public slots:
    void setOption(PlotView option);

signals:
    void optionChanged(PlotView option);

private slots:
    void on_curves_itemDoubleClicked(QListWidgetItem *item);
    void on_addCurve_clicked();
    void on_removeCurve_clicked();
    void on_unitNameH_textChanged(const QString &arg1);
    void on_unitNameV_textChanged(const QString &arg1);
    void on_lines_itemDoubleClicked(QListWidgetItem *item);
    void on_addLine_clicked();
    void on_removeLine_clicked();

private:
    Ui::PlotViewEditor *ui;
    PlotView m_option;
};
