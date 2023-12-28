#pragma once

#include <QDialog>
#include <Plot/PlotConfig.h>

namespace Ui {
class PlotLineEdit;
}

class PlotLineEdit : public QDialog
{
    Q_OBJECT

    Q_PROPERTY(PlotLine option READ option WRITE setOption)

public:
    explicit PlotLineEdit(QWidget *parent = 0);
    ~PlotLineEdit();

    PlotLine option() const;

public slots:
    void setOption(const PlotLine &option);

private:
    Ui::PlotLineEdit *ui;
};
