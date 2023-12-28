#include "CScanProbeIndicator.h"
#include <Ultrasonic/UTChannel.h>
#include "GraphicsDotItem.h"

CScanProbeIndicator::CScanProbeIndicator(QGraphicsItem *parent) :
    QGraphicsItemGroup(parent),
    dot( new GraphicsDotItem(this) ),
    text( new QGraphicsTextItem(this) )
{
    dot->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
    dot->setPos(0, 0);
    text->setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
    text->setPos(0, 0);
}

void CScanProbeIndicator::SetName(const QString &name)
{
    text->setPlainText(name);
}
