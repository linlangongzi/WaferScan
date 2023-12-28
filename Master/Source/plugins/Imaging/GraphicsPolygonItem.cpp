#include "GraphicsPolygonItem.h"
#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>

static QPen cosmaticPen(QColor color, qreal width)
{
    QPen pen(color, width);
    pen.setCosmetic(true);
    return pen;
}

static const QPen backgroundSelected = cosmaticPen(Qt::white, 4);
static const QPen foregroundSelected = cosmaticPen(Qt::black, 2);
static const QPen background = cosmaticPen(Qt::white, 3);
static const QPen foreground = cosmaticPen(Qt::black, 1);

GraphicsPolygonItem::GraphicsPolygonItem(QGraphicsItem *parent) :
    QGraphicsPolygonItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable);
    setPen(backgroundSelected);
}

/*!
 * \brief GraphicsPolygonItem::paint 重载的graphicsPolygonItem的重绘接口
 * \param painter
 * \param option
 * \param widget
 */
void GraphicsPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (option->state & QStyle::State_Selected) {
        painter->setPen(backgroundSelected);
        painter->drawPolygon(polygon());
        painter->setPen(foregroundSelected);
        painter->drawPolygon(polygon());
    } else {
        painter->setPen(background);
        painter->drawPolygon(polygon());
        painter->setPen(foreground);
        painter->drawPolygon(polygon());
    }
}
