#pragma once

#include <QWidget>
#include <QDateTime>
#include <QSettings>

namespace Ui {
class TemperatureView;
}

class TemperatureView : public QWidget
{
    Q_OBJECT

public:
    explicit TemperatureView(QWidget *parent = 0);
    ~TemperatureView();

    void loadSettings(QSettings *settings);
    void saveSettings(QSettings *settings);

private slots:
    void appendData();
    void refreshTimeRange();
    void onSourcesChanged();

    void onSeriesHover(const QPointF &pos, bool state);
    void on_addSource_clicked();
    void on_removeSource_clicked();
    void on_tempMin_valueChanged(double arg1);
    void on_tempMax_valueChanged(double arg1);
    void on_range_valueChanged(double arg1);
    void on_exportBrowse_clicked();

private:
    Ui::TemperatureView *ui;

    QTimer *timer;
    int dataCount = 0;
    QDateTime startTime;
};
