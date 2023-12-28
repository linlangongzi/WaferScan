#pragma once

#include <QObject>

namespace Motion {

class TrioDriver;

class TrioWarden : public QObject
{
    Q_OBJECT

public:
    explicit TrioWarden(TrioDriver *parent = 0);
    void start();
    void stop();

private:
    void timerEvent(QTimerEvent *event);

private:
    TrioDriver *p;
    int timerId = -1;
    int addr;
};

} // namespace Motion
