#pragma once

#include "AlarmGroupFrame.h"

#include <QMap>
#include <QLabel>
#include <QDockWidget>
#include <Plot/PlotConfig.h>

namespace Ui {
class PanelAlarm;
}

class PanelAlarm : public QDockWidget
{
    Q_OBJECT

    Q_PROPERTY(QList<PlotAlarmGroup> alarmGroups READ alarmGroups WRITE setAlarmGroups)

public:
    typedef QList<QLabel *> QLabelList;
    typedef QMap<QLabel *, bool> AlarmMap;

public:
    explicit PanelAlarm(QWidget *parent = 0);
    ~PanelAlarm();
    void ResetAlarmMaps();
    void AddAScan(QString name);
    void RemoveAScan(int index);
    qint32 GetCurAScanIndex() const;
    qint32 GetAScanCount() const;
    void ComboBoxClear();
    void setAlarms(const QList<PlotAlarmItem> &alarms);
    QList<PlotAlarmGroup> alarmGroups() const;
    TestResultType JudgeResult();

signals:
    void CurChannelChanged(qint32 ascanIndex, qint32 channelIndex);

public slots:
    void OnCurChannelChanged(qint32 index);
    void setAlarmGroups(const QList<PlotAlarmGroup> &alarmGroups);

private slots:
    void on_add_clicked();

private:
    void refreshControls();

private:
    Ui::PanelAlarm *ui;

    QList<PlotAlarmGroup> m_alarmGroups;
    QList<AlarmGroupFrame *> m_frames;

    AlarmMap m_DefectMap;
    AlarmMap m_CouplingMap;
    QList<QLabel *> labels;
};
