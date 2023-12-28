#pragma once

#include <QGraphicsPolygonItem>

class GraphicsPolygonItem : public QGraphicsPolygonItem
{
public:
    GraphicsPolygonItem(QGraphicsItem *parent = 0);

private:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
};
