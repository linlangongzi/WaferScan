#ifndef PANELPROBEBSCAN_H
#define PANELPROBEBSCAN_H

#include <Ultrasonic/UTData.h>
#include <QDockWidget>
#include <QLineSeries>
#include <QScatterSeries>
#include <QGraphicsEllipseItem>
#include <Plot/BScanChart.h>

class WheelDataset;

namespace Ui {
class PanelProbeBScan;
}

class Callout;

class PanelProbeBScan : public QDockWidget
{
    Q_OBJECT

public:
    explicit PanelProbeBScan(QWidget *parent = 0);
    ~PanelProbeBScan();
    void setWheelDataset(WheelDataset *dataset);

public slots:
    void refresh();
    void refreshRange();
    void replaceScatterSeries(qreal y);
    void showTooltip(QPointF point, bool state);
    void askModifyData(const QPointF &pos);

private:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void on_feature_currentIndexChanged(int index);
    void on_channel_valueChanged(int arg1);
    BScanSeries *createSeries(const QString &name);

private:
    Ui::PanelProbeBScan *ui;
    QList<BScanSeries *> lineSeries;
    QList<QPointF> locations;
    WheelDataset *wheelDataset = NULL;
    QtCharts::QScatterSeries *extremeSeries = NULL;
};

#endif // PANELPROBEBSCAN_H
