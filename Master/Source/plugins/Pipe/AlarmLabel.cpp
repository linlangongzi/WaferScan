#include "AlarmLabel.h"
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UltrasonicManager.h>

AlarmLabel::AlarmLabel(QWidget *parent) :
    QLabel(parent)
{

}

TestResultType AlarmLabel::JudgeResult()
{
    for (const auto &item : m_alarms) {
        if (item.featureIndex != 7) {       // 排除耦合不良报警
            return TestResultType::Reject;
        }
    }

    return TestResultType::Accept;
}

void AlarmLabel::setOption(const PlotAlarmIndicator &option)
{
    m_option = option;
    refresh();
}

void AlarmLabel::setAlarms(const QList<PlotAlarmItem> &alarms)
{
    m_alarms.clear();
    for (const auto &alarm: alarms) {
        if (alarm.channel.hasAxis(m_option.channel)) {
            m_alarms << alarm;
        }
    }

    refresh();
}

void AlarmLabel::setColor(const QColor &inactive, const QColor &active)
{
    inactiveColor = inactive;
    activeColor = active;

    refresh();
}

void AlarmLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit CurrentChannelIndex(m_option.channel);
}

void AlarmLabel::refresh()
{
    QColor color = m_alarms.isEmpty() ? inactiveColor : activeColor;
    setStyleSheet(QString("background-color: %1").arg(color.name()));

    QString tooltip;
    QTextStream s(&tooltip);

    if (!m_option.name.isEmpty()) {
        s << m_option.name;
        setText(m_option.name);
        setAlignment(Qt::AlignCenter);
    }
    UTChannel *channel = UltrasonicManager::channel(m_option.channel);
    if (channel) {
        s << endl << channel->description();
    }

    if (!m_alarms.isEmpty()) {
        s << endl << tr("当前触发的报警 (共 %1 个):").arg(m_alarms.count());
        for (const auto &alarm: m_alarms) {
            s << endl << "  " << alarm.reason;
        }
    }

    setToolTip(tooltip);
}
