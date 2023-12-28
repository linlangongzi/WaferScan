#pragma once

#include <Ultrasonic/UTData.h>
#include <QGraphicsItemGroup>

class GraphicsDotItem;
class GraphicsClearTextItem;

class CScanProbeIndicator : public QGraphicsItemGroup
{
public:
    explicit CScanProbeIndicator(QGraphicsItem *parent = 0);
    void SetName(const QString &name);

private:
    GraphicsDotItem *dot;
    QGraphicsTextItem *text;
};
