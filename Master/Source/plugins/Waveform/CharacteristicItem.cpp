#include "CharacteristicItem.h"
#include <QPainter>
#include <QDebug>

CharacteristicItem::CharacteristicItem(QGraphicsItem *parent) :
    QGraphicsItem(parent),
    pos(QPointF()),
    size(6),
    color(Qt::white)
{
    setFlag(ItemIgnoresTransformations);
}

void CharacteristicItem::SetColor(QColor value)
{
    if (color == value) {
        return;
    }
    color = value;
    update();
}

void CharacteristicItem::SetSize(qreal value)
{
    if (qFuzzyCompare(size, value)) {
        return;
    }
    prepareGeometryChange();
    size = value;
    update();
}

QRectF CharacteristicItem::boundingRect() const
{
    return QRectF(-size, -size, size * 2, size * 2);
}

void CharacteristicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setPen(QPen(color, 0));
    painter->drawLine(-size, 0, size, 0);
    painter->drawLine(0, -size, 0, size);
}
