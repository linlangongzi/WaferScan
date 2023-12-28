#pragma once

#include <QDialog>
#include <Plot/PlotConfig.h>

namespace Ui {
class PlotCurveEdit;
}

class PlotCurveEdit : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(PlotCurve option READ option WRITE setOption NOTIFY optionChanged)

public:
    explicit PlotCurveEdit(QWidget *parent = 0);
    ~PlotCurveEdit();

    PlotCurve option() const;

public slots:
    void setOption(PlotCurve option);

signals:
    void optionChanged(PlotCurve option);

private:
    Ui::PlotCurveEdit *ui;
    PlotCurve m_option;
};
