#include "BScanChart.h"

#include <QValueAxis>
#include <QLegendMarker>

using namespace QtCharts;

BScanChart::BScanChart(QWidget *parent) :
    QChartView(parent)
{
    QChart *c = chart();
    c->setParent(this);
    c->setAnimationOptions(QChart::NoAnimation);

    cursor = new QGraphicsEllipseItem(c);
    cursor->setRect(QRectF(-3, -3, 6, 6));
    cursor->setBrush(QBrush(Qt::black));
    cursor->setZValue(12);
    cursor->hide();

//    c->legend()->hide();
    c->setBackgroundRoundness(0);
    c->setMargins(QMargins(0, 0, 0, 0));

    auto *axisX = new QValueAxis(c);
    c->addAxis(axisX, Qt::AlignLeft);
    auto *axisY = new QValueAxis(c);
    axisY->setLabelsAngle(-90);
    c->addAxis(axisY, Qt::AlignBottom);

    setRubberBand(HorizontalRubberBand);
    setRenderHint(QPainter::Antialiasing);
}

void BScanChart::setupSeries()
{
    QChart *c = chart();
    QLegend *legend = c->legend();
    for (QLegendMarker *marker: legend->markers()) {
        connect(marker, &QLegendMarker::hovered, this, [=](bool state) {
            for (auto *series: c->series()) {
                bool isHovered = !state || (series == marker->series());
                series->setOpacity(isHovered ? 1 : 0.1);
            }
        });
    }
}

void BScanChart::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        event->accept();
        chart()->zoomReset();
    } else {
        QChartView::mouseReleaseEvent(event);
    }
}

BScanSeries::BScanSeries(QObject *parent) :
    QLineSeries(parent)
{
}

QPair<qreal, qreal> BScanSeries::range() const
{
    return qMakePair(m_min, m_max);
}

void BScanSeries::setData(const UTDataSegment &data, const QVector<qreal> &horizontal)
{
    if (m_data == data && m_horizontal == horizontal) {
        return;
    }
    m_data = data;
    m_horizontal = horizontal;
    refresh();
}

void BScanSeries::setFeatureIndex(int index)
{
    m_featureIndex = index;
    refresh();
}

void BScanSeries::refresh()
{
    qreal min = Q_INFINITY;
    qreal max = -Q_INFINITY;
    QVector<QPointF> points(m_data.size());
    for (int frameIndex = 0; frameIndex < m_data.size(); ++frameIndex) {
        const UTDataFrame &frame = m_data[frameIndex];
        points[frameIndex].setX(m_horizontal.isEmpty() ? frameIndex : m_horizontal.value(frameIndex));
        if (!frame.isEmpty()) {
            qreal v = frame.FeatureValue(m_featureIndex);
            min = qMin(v, min);
            max = qMax(v, max);
            points[frameIndex].setY(v);
        } else {
            points[frameIndex].setY(0);
        }
    }
    replace(points);
    if (!qFuzzyCompare(min, m_min) || !qFuzzyCompare(max, m_max)) {
        m_min = min;
        m_max = max;
        emit rangeUpdated();
    }
}

int BScanSeries::indexAt(qreal x) const
{
    if (m_horizontal.isEmpty()) {
        return qRound(x);
    }
    for (int index = 1; index < m_horizontal.count(); ++index) {
        if (x >= m_horizontal[index - 1] && x < m_horizontal[index]) {
            return index - 1;
        }
    }
    return m_horizontal.count() - 1;
}

UTDataFrame BScanSeries::frameAt(const QPointF &point) const
{
    int index = indexAt(point.x());
    return m_data.value(index);
}

QPointF BScanSeries::pointAt(const QPointF &point) const
{
    int index = indexAt(point.x());
    const auto frame = m_data.value(index);
    if (m_featureIndex < frame.CData.count()) {
        return point;
    }
    qreal x = m_horizontal.value(index, index);
    return QPointF(x, frame.FeatureValue(m_featureIndex));
}
