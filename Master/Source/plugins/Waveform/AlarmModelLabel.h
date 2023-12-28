#pragma once

#include <QLabel>
#include <QMovie>
#include <QTimer>

class AlarmModelLabel : public QLabel
{
    Q_OBJECT

public:
    explicit AlarmModelLabel(QWidget *parent = 0);
    ~AlarmModelLabel();

    void alarmShow();

public slots:
    void slotAlarmHide();
    void OnPlaySound();

private:
    QMovie *m_movie;
    QTimer *m_timer;
    QTimer *m_timerSound;

};
