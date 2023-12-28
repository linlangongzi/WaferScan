#include "GraphicsMarkItem.h"
#include <QPainter>
#include <QDebug>

GraphicsMarkItem::GraphicsMarkItem(QGraphicsItem *parent) :
    QGraphicsPathItem(parent)
{
    setFlag(ItemIgnoresTransformations);
    setShape(5, Plus);
}

void GraphicsMarkItem::setShape(qreal size, Style style)
{
    QPainterPath path;
    switch (style) {
    case Plus:
        path.moveTo(0, -size);
        path.lineTo(0, size);
        path.moveTo(-size, 0);
        path.lineTo(size, 0);
        break;
    case Cross:
        path.moveTo(-size, -size);
        path.lineTo(size, size);
        path.moveTo(-size, size);
        path.lineTo(size, -size);
        break;
    case Circle:
        path.addEllipse(-size, -size, size * 2, size * 2);
        break;
    case Rectangle:
        path.addRect(-size, -size, size * 2, size * 2);
        break;
    case RightArrowTriangle:
        path.addPolygon(QPolygonF({QPointF(0, -0.35 * size), QPointF(size, 0), QPointF(0, 0.35 * size), QPointF(0, -0.35 * size)}));
        break;
    case UpArrowTriangle:
        path.addPolygon(QPolygonF({QPointF(-0.35 * size, 0), QPointF(0.35 * size, 0), QPointF(0, -size), QPointF(-0.35 * size, 0)}));
        break;
    default:
        break;
    }
    setPath(path);
}

void GraphicsMarkItem::setPositionCallback(GraphicsMarkItem::PositionCallback callback)
{
    positionCallback = callback;
    setFlag(ItemSendsGeometryChanges, bool(callback));
}

QVariant GraphicsMarkItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && positionCallback) {
        QPointF newPos = value.toPointF();
        newPos = positionCallback(pos(), newPos);
        return newPos;
    }
    return QGraphicsPathItem::itemChange(change, value);
}
