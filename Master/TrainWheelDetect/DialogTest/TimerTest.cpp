#include "TimerTest.h"

TimerTest::TimerTest(QObject *parent)
    :QTimer(parent)
{
    list = UTChannel::GetGlobalChannels();
}

void TimerTest::timerEvent(QTimerEvent *)
{
    if(m_nIdx < list.size())
    {
        emit ChannelChanged(list.at(m_nIdx)->GetChnNo(), WaveType::WaveSurface);
        m_nIdx++;
    }
    else
    {
        m_nIdx = 0;
        emit ChannelChanged(list.at(m_nIdx)->GetChnNo(), WaveType::WaveSurface);
    }

    return;
}

void TimerTest::Start(int nInterval)
{
    m_nIdx = 0;
    start(nInterval);
    return;
}
