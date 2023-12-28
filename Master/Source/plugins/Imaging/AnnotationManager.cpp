#include "AnnotationManager.h"
#include <Waveform/UTGraphicsScene.h>
#include <QGraphicsWidget>
#include <QToolTip>
#include <QGraphicsView>


static const int MARK_LINE_LENGTH = 5;

AnnotationManager::AnnotationManager(QGraphicsItem *item, QObject *parent) : QObject(parent)
{
    parentItem = item;
    line = new GraphicsInvertedShape<QGraphicsLineItem>(parentItem);
    line->setPen(QPen(Qt::black, 0));

    leftEndLine = new GraphicsInvertedShape<QGraphicsLineItem>(line);
    leftEndLine->setPen(QPen(Qt::black, 0));
    leftEndLine->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    rightEndLine = new GraphicsInvertedShape<QGraphicsLineItem>(line);
    rightEndLine->setPen(QPen(Qt::black, 0));
    rightEndLine->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    circle = new GraphicsInvertedShape<QGraphicsEllipseItem>(parentItem);
    circle->setPen(QPen(Qt::black, 0));

    rectangle = new GraphicsInvertedShape<QGraphicsRectItem>(parentItem);
    rectangle->setPen(QPen(Qt::black, 0));

    setMode(None);
}

AnnotationManager::~AnnotationManager() = default;

bool AnnotationManager::mouseMoveHandler(const QPointF &pos)
{
    if (!qIsFinite(pos.x()) || !qIsFinite(pos.y())) {
        return false;
    }
    QPointF geometryPos = parentItem->mapFromScene(pos);
    switch (mode) {
    case Line:
    case Circle:
        if (state == OnePoint) {
            updateShapes(geometryPos);
        }
        break;
    case CustomCircle: setCirclePos(geometryPos); break;
    case Rect: setRectPos(geometryPos); break;
    default: return false;
    }
    return true;
}

bool AnnotationManager::doubleClickHandler(const QPointF &pos)
{
    QPointF geometryPos = parentItem->mapFromScene(pos);
    switch (mode) {
    case Line:
    case Circle: toggleShapeState(geometryPos); break;
    case CustomCircle: setCirclePos(geometryPos); break;
    case Rect: setRectPos(geometryPos); break;
    default: return false;
    }
    return true;
}

void AnnotationManager::setMode(AnnotationManager::Mode customMode)
{
    if (mode == customMode) {
        return;
    }
    mode = customMode;
    state = ZeroPoint;

    line->setLine(QLineF());
    leftEndLine->setLine(QLineF());
    rightEndLine->setLine(QLineF());
    circle->setRect(QRectF());
    rectangle->setRect(QRectF());

    line->setVisible(mode == Line || mode == Circle);
    leftEndLine->setVisible(mode == Line || mode == Circle);
    rightEndLine->setVisible(mode == Line || mode == Circle);
    circle->setVisible(mode == Circle || mode == CustomCircle);
    rectangle->setVisible(mode == Rect);
}

void AnnotationManager::setDiameter(qreal custom)
{
    diameter = custom;
}

QRectF AnnotationManager::rectAt(const QPointF &pos)
{
    qreal radius = diameter / 2;
    QRectF rect(pos, QSizeF(0, 0));
    rect.adjust(-radius, -radius, radius, radius);
    return rect;
}

void AnnotationManager::setCirclePos(const QPointF &pos)
{
    circle->setRect(rectAt(pos));
    tip(tr("直径: %1").arg(diameter), pos);
}

void AnnotationManager::tip(const QString &text, const QPointF &pos) const
{
    if (parentItem == nullptr) {
        return;
    }
    if (parentItem->scene() == nullptr) {
        return;
    }
    if (parentItem->scene()->views().isEmpty()) {
        return;
    }
    QGraphicsView *view = parentItem->scene()->views().first();
    QPointF posInScene = parentItem->mapToScene(pos);
    QPoint posInView = view->mapFromScene(posInScene);
    QPoint posInGlobal = view->mapToGlobal(posInView);
    QToolTip::showText(posInGlobal, text, view);
}

void AnnotationManager::setRectPos(const QPointF &pos)
{
    rectangle->setRect(rectAt(pos));
    tip(tr("边长: %1").arg(diameter), pos);
}

void AnnotationManager::updateShapes(const QPointF &pos)
{
    QLineF newLine(line->line().p1(), pos);
    qreal lineAngle = line->line().angle();
    line->setLine(newLine);
    leftEndLine->setRotation(lineAngle);
    leftEndLine->setVisible(mode == Line);
    rightEndLine->setLine(QLineF(0, -MARK_LINE_LENGTH, 0, MARK_LINE_LENGTH));
    rightEndLine->setPos(pos);
    rightEndLine->setRotation(lineAngle);
    rightEndLine->setVisible(mode == Line);
    tip(tr("%1 mm").arg(newLine.length()), pos);
    if (mode == Line) {
        return;
    }
    QPointF center = newLine.center();
    qreal radius = newLine.length() / 2;
    QRectF rect(center, QSizeF(0, 0));
    rect.adjust(-radius, -radius, radius, radius);
    circle->setRect(rect);
}

void AnnotationManager::toggleShapeState(const QPointF &pos)
{
    if (state == ZeroPoint || state == TwoPoint) {
        line->setLine(QLineF(pos, pos));
        leftEndLine->setLine(QLineF(0, -MARK_LINE_LENGTH, 0, MARK_LINE_LENGTH));
        leftEndLine->setPos(pos);
        leftEndLine->setRotation(-line->line().angle());
        state = OnePoint;
    } else if (state == OnePoint) {
        updateShapes(pos);
        state = TwoPoint;
    }
}
