#include "GraphicsDotItem.h"
#include <QPen>
#include <QPainter>
#include <QDebug>
#include <QCursor>
#include <QGraphicsScene>

GraphicsDotItem::GraphicsDotItem(QGraphicsItem *parent) :
    QGraphicsObject(parent),
    point(),
    size(5),
    dragable(false),
    inColor(QColor(Qt::black)),
    outColor(QColor(Qt::white)),
    backLineShow(true),
    fillColor(false)
{
    shape = CIRCLE;
    update();
}

QPointF GraphicsDotItem::GetPoint() const
{
    return point;
}

double GraphicsDotItem::GetSize() const
{
    return size;
}

void GraphicsDotItem::SetPoint(QPointF arg)
{
    if (point == arg) {
        return;
    }
    prepareGeometryChange();
    point = arg;
}

void GraphicsDotItem::SetPoint(float x, float y)
{
    SetPoint(QPointF(x, y));
}

void GraphicsDotItem::SetSize(double arg)
{
    if (size == arg) {
        return;
    }
    prepareGeometryChange();
    size = arg;
}

void GraphicsDotItem::SetInColor(QColor color)
{
    if (inColor == color) {
        return;
    }
    inColor = color;
    update();
}

void GraphicsDotItem::SetOutColor(QColor color)
{
    if (outColor == color) {
        return;
    }
    outColor = color;
    update();
}

QRectF GraphicsDotItem::boundingRect() const
{
    QRectF rect;
    switch (shape) {
    case CIRCLE:
        rect =  QRectF(point.x() - qAbs( (size + 1) / worldTrans.m11() ), point.y() - qAbs( (size + 1) / worldTrans.m22() ),
                          qAbs( (size + size + 2) / worldTrans.m11() ), qAbs( (size + size + 2) / worldTrans.m22() ));
        break;
    case SQUARE:
        rect =  QRectF(point.x() - qAbs((size / 2.0 + 1) / worldTrans.m11() ), point.y() - qAbs((size / 2.0 + 1) / worldTrans.m22()),
                          qAbs((size + 2) / worldTrans.m11() ), qAbs((size + 2) / worldTrans.m22() ));
        break;
    default:
        break;
    }
    return rect;
}

/*!
 * \brief GraphicsDotItem::SetDragEnabled 设置为可移动状态
 * \param enable
 */
void GraphicsDotItem::SetDragEnabled(bool enable)
{
    GraphicsItemFlags flag = flags();
    if (enable) {
        setCursor(Qt::OpenHandCursor);
        flag |= QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable;
    } else {
        setCursor(Qt::ArrowCursor);
        flag &= ~QGraphicsItem::ItemIsSelectable & ~QGraphicsItem::ItemIsFocusable;
    }
    dragable = enable;
    setFlags(flag);
}

void GraphicsDotItem::SetShape(GraphicsDotItem::Shape shape)
{
    if (this->shape == shape)
    {
        return;
    }
    prepareGeometryChange();
    this->shape = shape;
}

void GraphicsDotItem::SetBackLineShow(bool enabled)
{
    if (backLineShow == enabled)
    {
        return;
    }
    backLineShow = enabled;
    update();
}

void GraphicsDotItem::SetIfFillColor(bool value)
{
    if (fillColor == value)
    {
        return;
    }
    fillColor = value;
    update();
}

void GraphicsDotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (worldTrans != painter->worldTransform()) {
        prepareGeometryChange();
        worldTrans = painter->worldTransform();
    }
    QPen penOut(outColor);
    penOut.setWidth(3);
    penOut.setCosmetic(true);
    QPen penIn(inColor);
    penIn.setWidth(0);
    QBrush brush(inColor);
    switch (shape) {
    case CIRCLE:
        painter->setPen(penOut);
        if (backLineShow) {painter->drawEllipse(point, qAbs(size / worldTrans.m11()), qAbs(size / worldTrans.m22()));}
        painter->setPen(penIn);
        if (fillColor) {painter->setBrush(brush);}
        painter->drawEllipse(point, qAbs(size / worldTrans.m11()), qAbs(size / worldTrans.m22()));
        break;
    case SQUARE:
        painter->setPen(penOut);
        if (backLineShow) {painter->drawRect(QRectF(point.x() - qAbs(size/ 2.0 / worldTrans.m11()), point.y() - qAbs(size/ 2.0 / worldTrans.m22()), qAbs(size / worldTrans.m11()), qAbs(size / worldTrans.m22())));}
        painter->setPen(penIn);
        if (fillColor) {painter->setBrush(brush);};
        painter->drawRect(QRectF(point.x() - qAbs(size/ 2.0 / worldTrans.m11()), point.y() - qAbs(size/ 2.0 / worldTrans.m22()), qAbs(size / worldTrans.m11()), qAbs(size / worldTrans.m22())));
        break;
    default:
        break;
    }
}

/*!
 * \brief GraphicsDotItem::mouseMoveEvent 鼠标移动事件
 * \param event
 */
void GraphicsDotItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!isSelected()) {
        return;
    }
    emit DotMoved(event->scenePos());
}

void GraphicsDotItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mousePressEvent(event);
    if (dragable) {
        setCursor(Qt::ClosedHandCursor);
    }
}

void GraphicsDotItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseReleaseEvent(event);
    if (dragable) {
        setCursor(Qt::OpenHandCursor);
    }
}

void GraphicsDotItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsObject::contextMenuEvent(event);
    QGraphicsScene *scene = this->scene();
    if (scene) {
        scene->clearSelection();
    }
    setSelected(true);
}
