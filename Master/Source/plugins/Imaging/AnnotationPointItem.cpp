#include "AnnotationPointItem.h"
#include <QPainterPath>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "image.pointItem");

static const qreal RADIUS = 5;

AnnotationPointItem::AnnotationPointItem(QGraphicsItem *parent) :
    QGraphicsPathItem (parent)
{
    setFlag(ItemIgnoresTransformations);

    QPainterPath path;
    path.addEllipse(-RADIUS, -RADIUS, diameter(), diameter());
    setPath(path);
}

void AnnotationPointItem::setPositionCallback(const AnnotationPointItem::PositionCallback &callback)
{
    m_positionCallback = callback;
    setFlag(ItemSendsGeometryChanges, bool(callback));
}

qreal AnnotationPointItem::diameter() const
{
    return RADIUS * 2;
}

QVariant AnnotationPointItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && m_positionCallback) {
        QPointF newPos = value.toPointF();
        newPos = m_positionCallback(pos(), newPos);
        return newPos;
    }
    return QGraphicsPathItem::itemChange(change, value);
}
