#pragma once

#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QPainter>

class GraphicsInvertedLine : public QGraphicsLineItem
{
public:
    GraphicsInvertedLine(QGraphicsItem *parent = nullptr) :
        QGraphicsLineItem(parent)
    {
        setPen(QPen(Qt::black, 0));
        setZValue(1);
    }

    GraphicsInvertedLine(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent = nullptr) :
        QGraphicsLineItem(x1, y1, x2, y2, parent)
    {
        setPen(QPen(Qt::black, 0));
        setZValue(1);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        painter->setCompositionMode(QPainter::RasterOp_NotDestination);
        painter->setRenderHint(QPainter::Antialiasing, false);
        QGraphicsLineItem::paint(painter, option, widget);
    }
};

template <typename T = QGraphicsLineItem>
class GraphicsInvertedShape : public T
{
public:
    GraphicsInvertedShape(QGraphicsItem *parent = nullptr) :
        T(parent)
    {}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
        painter->setCompositionMode(QPainter::RasterOp_NotDestination);
        painter->setRenderHint(QPainter::Antialiasing, false);
        T::paint(painter, option, widget);
    }
};
