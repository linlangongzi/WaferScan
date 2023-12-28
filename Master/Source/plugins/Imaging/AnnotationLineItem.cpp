#include "AnnotationLineItem.h"
#include <QCursor>
#include <QLoggingCategory>
#include <QPen>

static Q_LOGGING_CATEGORY(debug, "image.lineItem");

AnnotationLineItem::AnnotationLineItem(QGraphicsItem *parent) :
    QGraphicsItemGroup(parent)
{
    m_line = new QGraphicsLineItem(this);
    m_startPoint = new AnnotationPointItem(this);
    m_endPoint = new AnnotationPointItem(this);
    addToGroup(m_line);
    addToGroup(m_startPoint);
    addToGroup(m_endPoint);

    m_line->setPen(QPen(Qt::black, 0));

    m_line->setFlag(ItemIsMovable);
    m_startPoint->setFlag(ItemIsMovable);
    m_endPoint->setFlag(ItemIsMovable);
    setFlag(ItemIsMovable);
//    setFlag(ItemIsSelectable);
//    setFlag(ItemIsFocusable);

    setFiltersChildEvents(false);

    setCursor(Qt::SizeAllCursor);
}

AnnotationLineItem::~AnnotationLineItem() = default;

void AnnotationLineItem::setLine(const QPointF &start, const QPointF &end)
{
    updateLine(start, end);
    updatePoint(start, end);
}

void AnnotationLineItem::setStart(const QPointF &newStart)
{
    m_startPoint->setPos(newStart);
    updateLine(newStart, m_endPoint->pos());
}

void AnnotationLineItem::setEnd(const QPointF &newEnd)
{
    m_endPoint->setPos(newEnd);
    updateLine(newEnd, m_endPoint->pos());
}

void AnnotationLineItem::updateLine(const QPointF &start, const QPointF &end)
{
    m_line->setLine(QLineF(start, end));
}

void AnnotationLineItem::updatePoint(const QPointF &start, const QPointF &end)
{
    m_startPoint->setPos(start);
    m_endPoint->setPos(end);
}

QRectF AnnotationLineItem::boundingRect() const
{
    qCDebug(debug) << "x =" << -m_startPoint->x() - m_startPoint->diameter() / 2 << "y =" << -m_startPoint->y() <<
                      "length =" << m_line->line().length() + m_startPoint->diameter() << "width =" << m_startPoint->diameter();
    return (QRectF(-m_startPoint->x() - m_startPoint->diameter() / 2, -m_startPoint->y(),
            m_line->line().length() + m_startPoint->diameter(), m_startPoint->diameter()));
}

void AnnotationLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
//    QPointF pos = event->pos();
//    if (m_isSelected) {
//        // 拖动
//    } else {
//        // 画线
//        // 如果用户刚好把点放在(0, 0)位置上
//        if (m_state == ZeroPoint || m_state == TwoPoint) {
//            return;
//        }
//        m_line->setLine(QLineF(m_line->line().p1(), pos));
//    }
    QGraphicsItem::mouseMoveEvent(event);
}

void AnnotationLineItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
//    qCDebug(debug) << "mouseDoubleClickEvent m_state =" << m_state;
//    QPointF pos = event->pos();
//    if (m_state == ZeroPoint) {
//        //画起点
//        m_line->setLine(QLineF(pos, pos));
//        m_startPoint->setPos(pos);
//        m_state = OnePoint;
//    } else if (m_state == OnePoint) {
//        //画终点
//        m_line->setLine(QLineF(m_line->line().p1(), pos));
//        m_endPoint->setPos(pos);
//        m_state = TwoPoint;
//    }
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void AnnotationLineItem::keyPressEvent(QKeyEvent *event)
{
//    qCDebug(debug) << "keyPressEvent" << event->key();
    QGraphicsItem::keyPressEvent(event);
//    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete) {
//        delete this;
//    }
}

