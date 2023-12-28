#pragma once

#include <Ultrasonic/UTData.h>
#include <QGraphicsPathItem>

class WaveformItem : public QGraphicsPathItem
{
public:
    WaveformItem(QGraphicsItem *parent);

    void setWaveform(const UTDataFrame &frame);
};
