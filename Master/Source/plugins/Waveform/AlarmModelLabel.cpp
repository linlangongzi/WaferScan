#include "AlarmModelLabel.h"
#include <QApplication>

AlarmModelLabel::AlarmModelLabel(QWidget *parent):
    QLabel(parent)
{
    m_movie = new QMovie(":/alarm.gif", "gif", this);
    setMovie(m_movie);
    setScaledContents(true);
    m_movie->setCacheMode(QMovie::CacheAll);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &AlarmModelLabel::slotAlarmHide);
    m_timer->start(1000);

    m_timerSound = new QTimer(this);
    connect(m_timerSound, SIGNAL(timeout()), this, SLOT(OnPlaySound()));
    m_timerSound->setInterval(100);
}

AlarmModelLabel::~AlarmModelLabel() = default;

void AlarmModelLabel::OnPlaySound()
{
    if (isVisible()) {
        QApplication::beep();
    }
}

void AlarmModelLabel::alarmShow()
{
    m_timer->start();
    m_timerSound->start();
    m_movie->start();
    setVisible(true);
}

void AlarmModelLabel::slotAlarmHide()
{
    m_timer->stop();
    m_timerSound->stop();
    m_movie->stop();
    setVisible(false);
}
