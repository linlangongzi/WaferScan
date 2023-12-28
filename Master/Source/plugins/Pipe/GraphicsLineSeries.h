#pragma once

#include <QGraphicsPathItem>
#include <Plot/PlotConfig.h>
#include <QCheckBox>
#include <QLabel>

class GraphicsLineSeries : public QWidget
{
    Q_OBJECT

signals:
    void FoundFeature(qreal pos);

public:
    GraphicsLineSeries(QWidget *parent);
    ~GraphicsLineSeries();

    void setItemParents(QGraphicsItem *parentItem);
    void setOption(const PlotCurve &option);
    bool add(qreal pos, int position, qreal value, int channelId, bool noAlarm);
    void setSize(int size);
    void setAlarms(const QList<PlotLine> &lines);
    void reset();

    void refreshPath();
    void calculateExtreme(int channelId, qreal value, int pos);
    void refreshTooltips();

    QGraphicsPathItem *path;
    QCheckBox *title;
    QLabel *legend;

    PlotCurve curve;
    QVector<qreal> data;        // 刷新线条所用数据
    QVector<int> count;
    PlotFeature maxima;
    PlotFeature minima;

    struct AlarmItem
    {
        PlotLine condition;
        PlotAlarmItem alarm;
        bool allTriggered = false;
    };
    QList<AlarmItem> alarms;
    bool dirty = false;
};
