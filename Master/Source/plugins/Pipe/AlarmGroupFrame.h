#pragma once

#include "AlarmLabel.h"
#include <QFrame>
#include <Plot/PlotConfig.h>

namespace Ui {
class AlarmGroupFrame;
}

class AlarmGroupFrame : public QFrame
{
    Q_OBJECT

    Q_PROPERTY(PlotAlarmGroup option READ option WRITE setOption NOTIFY optionChanged)

public:
    explicit AlarmGroupFrame(QWidget *parent = 0);
    ~AlarmGroupFrame();
    TestResultType JudgeResult();
    PlotAlarmGroup option() const;
    void ConnectTo(QObject *obj);

public slots:
    void setOption(const PlotAlarmGroup &option);
    void setAlarms(const QList<PlotAlarmItem> &alarms);

signals:
    void optionChanged(PlotAlarmGroup option);

protected:
    bool eventFilter(QObject *obj, QEvent *e);

private:
    Ui::AlarmGroupFrame *ui;
    PlotAlarmGroup m_option;
    QList<AlarmLabel *> m_labels;
};
