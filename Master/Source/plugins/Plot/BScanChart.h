#pragma once

#include <QChartView>
#include <QLineSeries>
#include <Ultrasonic/UTData.h>

class BScanChart : public QtCharts::QChartView
{
    Q_OBJECT

public:
    BScanChart(QWidget *parent);
    void setupSeries();

    QGraphicsEllipseItem *cursor = nullptr;

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
};

class BScanSeries : public QtCharts::QLineSeries
{
    Q_OBJECT

public:
    BScanSeries(QObject *parent);
    QPair<qreal, qreal> range() const;

signals:
    void rangeUpdated();

public slots:
    void setData(const UTDataSegment &data, const QVector<qreal> &horizontal = QVector<qreal>());
    void setFeatureIndex(int index);
    void refresh();

    int indexAt(qreal x) const;
    UTDataFrame frameAt(const QPointF &point) const;
    QPointF pointAt(const QPointF &point) const;

private:
    int m_featureIndex = -1;
    UTDataSegment m_data;
    QVector<qreal> m_horizontal;
    qreal m_min = 0;
    qreal m_max = 0;
};
