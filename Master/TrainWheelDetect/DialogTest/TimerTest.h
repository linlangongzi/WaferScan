#ifndef TIMERTEST_H
#define TIMERTEST_H

#include <QTimer>

#include <Ultrasonic/UTInstrument.h>
#include "UTInstrumentDriver/PXI100/DataDef.h"

class TimerTest : public QTimer
{
    Q_OBJECT

public:
    explicit TimerTest(QObject *parent = 0);
    void Start(int nInterval);

private:
    int m_nIdx;
    UTInstrument::ChannelList list;
private:
    virtual void timerEvent(QTimerEvent *);

signals:
    void ChannelChanged(int probeNo, WaveType type);
};

#endif // TIMERTEST_H
