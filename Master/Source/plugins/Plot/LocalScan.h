#pragma once

#include "BScanChart.h"
#include <Motion/Axis.h>
#include <QtCharts/QLineSeries>
#include <QTime>
#include <QValueAxis>
#include <QWidget>

namespace Ui {
class LocalScan;
}

class LocalScan : public QWidget
{
    Q_OBJECT

public:
    enum ScanDirection {
        Forward,
        Reverse,
        Both
    };
    Q_ENUM(ScanDirection)

    enum ScanStatus {
        Idle,
        PrepareLocate,
        Locate,
        PrepareScan,
        Scan,
        PrepareRestore,
        Restore,
        PrepareMoveTo,
        MoveTo
    };
    Q_ENUM(ScanStatus)

    explicit LocalScan(QWidget *parent = 0);
    ~LocalScan();

    int axisIndex();
    Axis *scanAxis() const;
    UTChannel *channel() const;

public slots:
    void onAxisStatusChanged(const AxisStatus &status);
    void onDataArrived(const UTDataSegment &segment);

private slots:
    void resetSeries();
    void refreshLines();
    void on_scan_clicked();
    void on_stop_clicked();
    void on_reset_clicked();
    void on_moveTo_clicked();
    void on_channel_currentIndexChanged(int index);

private:
    bool eventFilter(QObject *watched, QEvent *event) override;
    bool trioNormal() const;
    bool isEqualWith(qreal value, qreal target);
    void moveAxis(Axis *axis, qreal pos);
    void setUiEnable(bool enable);
    void setStatus(ScanStatus status);
    void errorWarning(const QString &content);
    qreal cursorX() const;

    Ui::LocalScan *ui;
    qreal m_startPos = 0;
    qreal m_endPos = 0;
    qreal m_originalPos = Q_SNAN;
    ScanStatus m_status = Idle;
    QTime m_timeStamp = QTime::currentTime();
    qreal m_axisPresition = 0.01;
    const int m_timeSpace = 100;

    BScanSeries *m_ampSeries = nullptr;
    BScanSeries *m_timeSeries = nullptr;
    QtCharts::QLineSeries  *m_line = nullptr;
    QtCharts::QAbstractAxis *m_axisAmp = nullptr;
    QtCharts::QAbstractAxis *m_axisTime = nullptr;
    QtCharts::QAbstractAxis *m_axisTimeDisplay = nullptr;
    QVector<UTDataFrame> m_data;
    QVector<qreal> m_location;
};
