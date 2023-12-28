#include "WaveformItem.h"
#include <QGraphicsPathItem>

WaveformItem::WaveformItem(QGraphicsItem *parent) :
    QGraphicsPathItem(parent)
{
}

void WaveformItem::setWaveform(const UTDataFrame &frame)
{
    if (frame.hasWaveform()) {
        double bottom = -100;
        double scaleY = 100.0f / 1023;

        QPainterPath path;
        int length = frame.waveformLength();
        qreal start = frame.Context.GetDelay();
        qreal range = frame.Context.GetRange();

        if (length > 0) {
            double scaleX = range / length;
            path.moveTo(start, bottom);                                         // 画笔放到原点开始绘制

            for (int index = 0; index < length; ++index) {
                double x = index * scaleX + start;
                double y = frame.AData[index] * scaleY;
                path.lineTo(x, y);
            }
            path.lineTo(range + start, bottom);
        }
        setPath(path);
    } else {
        setPath(QPainterPath());
    }
}
