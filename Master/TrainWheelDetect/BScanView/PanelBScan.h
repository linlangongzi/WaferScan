#ifndef PANELBSCAN_H
#define PANELBSCAN_H

#include <QDockWidget>
#include <Plot/BScanChart.h>

class WheelDataset;
class AScanPlotterData;

namespace Ui {
class PanelBScan;
}

class PanelBScan : public QDockWidget
{
    Q_OBJECT

public:
    explicit PanelBScan(QWidget *parent = 0);
    ~PanelBScan();
    void setupSeries();
    void refreshLines();
    QImage renderBScan();
    void setWheelDataset(WheelDataset *dataset);

private slots:
    void refreshRange();
    void setChartsOptions(QtCharts::QChart::AnimationOptions options);
    void onSeriesHovered(QPointF point, bool state);
    void onSeriesDoubleClicked();
    void onChannelDataUpdated(const QSet<qint64> &channels);
    void on_feature_activated(int index);

private:
    Ui::PanelBScan *ui;
    QList<BScanSeries *> seriesList;
    WheelDataset *wheelDataset = NULL;
};

#endif // PANELBSCAN_H
