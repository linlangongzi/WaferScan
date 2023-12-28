#include "DefectGraphicsItem.h"
#include "GraphicsPolygonItem.h"
#include <QBitmap>
#include <QGraphicsTextItem>
#include <QPainter>
#include <QtMath>

QPropertyAnimation DefectGraphicsItem::animation;

qreal GraphicsFlashingEffect::GetProgress() const
{
    return 0;
}

void GraphicsFlashingEffect::SetProgress(qreal progress)
{
    setOpacity(0.25 * (qSin(progress) + 1));
}

DefectGraphicsItem::DefectGraphicsItem(DefectItem *item, QGraphicsItem *parent) :
    QGraphicsItemGroup(parent),
    minRect(new GraphicsPolygonItem(this)),
    pixmap(new QGraphicsPixmapItem(this))
{
    pixmap->hide();
    setFlag(ItemDoesntPropagateOpacityToChildren);
    setFlag(ItemIsSelectable);
    SetDefectItem(item);
    setAcceptHoverEvents(true);
    addToGroup(minRect);
}

void DefectGraphicsItem::SetDefectItem(DefectItem *item)
{
    defectItem = item;
    prepareGeometryChange();
    rect = item->defect().markRect;
    minRect->setPolygon(rect);
    pixmap->setOffset(rect.topLeft());
    pixmap->setPixmap(QBitmap::fromImage(item->defect().image));
}

bool DefectGraphicsItem::contains(const QPointF &point) const
{
    if (defectItem == nullptr) {
        return false;
    }
    bool ret = defectItem->defect().image.pixel(point.toPoint());
    return ret;
}

void DefectGraphicsItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event);
    if (!isSelected()) {
        setSelected(true);
    }
}

void DefectGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItemGroup::hoverEnterEvent(event);

    auto effect = new GraphicsFlashingEffect;
    animation.setTargetObject(effect);
    animation.setPropertyName("progress");
    animation.setLoopCount(-1);
    animation.setStartValue(qreal(0));
    animation.setEndValue(qreal(M_PI));
    animation.setDuration(500);
    animation.start();
    pixmap->show();
    pixmap->setGraphicsEffect(effect);
}

void DefectGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItemGroup::hoverLeaveEvent(event);

    pixmap->hide();
    animation.stop();
    pixmap->setGraphicsEffect(nullptr);
}

void DefectGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPen pen(Qt::red, 5);
    pen.setCosmetic(true);
    if (isSelected()) {
        painter->setPen(pen);
        painter->drawRect(rect);
    }
}
