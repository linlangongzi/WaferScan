#pragma once

#include <QLabel>
#include "PanelStatusFrame.h"
#include <Plot/PlotConfig.h>

class AlarmLabel : public QLabel
{
    Q_OBJECT

public:
    explicit AlarmLabel(QWidget *parent = 0);
    TestResultType JudgeResult();

public slots:
    void setOption(const PlotAlarmIndicator &option);
    void setAlarms(const QList<PlotAlarmItem> &alarms);
    void setColor(const QColor &inactive, const QColor &active);

signals:
    void CurrentChannelIndex(int index);

private:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void refresh();

private:
    PlotAlarmIndicator m_option;
    QList<PlotAlarmItem> m_alarms;
    QColor activeColor;
    QColor inactiveColor;
};
