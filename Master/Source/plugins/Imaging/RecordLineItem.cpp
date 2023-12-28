#include "RecordLineItem.h"
#include <QDebug>
#include <QCursor>


RecordLineItem::RecordLineItem(QGraphicsItem *parent) :
//    QObject(parent),
    QGraphicsLineItem(parent),
    inColor(QColor(Qt::black)),
    backLineShow(true)
{
    setFlags(/*QGraphicsItem::ItemIsFocusable | QGraphicsItem::ItemIsMovable | */QGraphicsItem::ItemIsSelectable);
//    this->setCursor( QCursor(Qt::OpenHandCursor) );
    update();
}

void RecordLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHints(QPainter::Antialiasing);
    worldTrans = painter->worldTransform();

    QPen pen(Qt::white);
    if (backLineShow)
    {
        pen.setWidth(3);
        pen.setCosmetic(true);
        painter->setPen(pen);
        painter->drawLine(this->line());
    }
    pen.setColor(inColor);
    pen.setWidth(0);

    painter->setPen(pen);
    painter->drawLine(this->line());
}

QPainterPath RecordLineItem::shape() const
{
    QLineF normalLine = line().normalVector();
    qreal angel = normalLine.angle();

    QPointF leftPoint = line().p1();
    QLineF leftBaseLine(leftPoint, QPointF(leftPoint.x() + qAbs(4 / worldTrans.m11()), leftPoint.y()));
    leftBaseLine.setAngle(angel);
    QPointF topleft = leftBaseLine.p2();
    leftBaseLine.setAngle(180 + angel);
    QPointF bottomleft = leftBaseLine.p2();

    QPointF rightPoint = line().p2();
    QLineF rightBaseLine(rightPoint, QPointF(rightPoint.x() + qAbs(4 / worldTrans.m11()), rightPoint.y()));
    rightBaseLine.setAngle(180 + angel);
    QPointF bottomright = rightBaseLine.p2();
    rightBaseLine.setAngle(angel);
    QPointF topright = rightBaseLine.p2();

    QPolygonF polygon;
    polygon.append(topleft);
    polygon.append(bottomleft);
    polygon.append(bottomright);
    polygon.append(topright);

    QPainterPath path;
    path.addPolygon(polygon);
    return path;
}

void RecordLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

/*!
 * \brief RecordLineItem::contextMenuEvent
 * \param event
 */
void RecordLineItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event);
    setSelected(true);
    emit contextMenuRequest(event);
}

void RecordLineItem::SetInColor(QColor color)
{
    if(inColor == color)
    {
        return;
    }
    inColor = color;
    update();
}

void RecordLineItem::SetBackLineShow(bool enabled)
{
    if (backLineShow == enabled)
    {
        return;
    }
    backLineShow = enabled;
    update();
}

