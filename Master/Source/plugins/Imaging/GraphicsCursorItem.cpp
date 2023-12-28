#include <utility>

#include <utility>

#include <utility>

#include "GraphicsCursorItem.h"
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QLoggingCategory>
#include <QMenu>
#include <QtMath>

static Q_LOGGING_CATEGORY(debug, "image.cursorItem");

static const qreal CROSSPOINT_SIZE = 4;

template<typename T>
class CursorPosition : public T
{
public:
    CursorPosition(QGraphicsItem *parent = nullptr) :
        T(parent)
    {
        QGraphicsItem::setFlag(QGraphicsItem::ItemIgnoresTransformations);
    }

    void setPositionCallback(std::function<QPointF(QPointF)> callback)
    {
        m_positionCallback = callback;
        QGraphicsItem::setFlag(QGraphicsItem::ItemSendsGeometryChanges, bool(callback));
    }

protected:
    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override
    {
        if (change == QGraphicsItem::ItemPositionChange && m_positionCallback) {
            return m_positionCallback(value.toPointF());        //result can be changed and return is important
        }
        return QGraphicsItem::itemChange(change, value);
    }

private:
    std::function<QPointF(QPointF)> m_positionCallback;
};

using LineItem = CursorPosition<QGraphicsLineItem>;
using PointItem = CursorPosition<QGraphicsRectItem>;

GraphicsCursorItem::GraphicsCursorItem(QGraphicsItem *parent):
    QGraphicsItemGroup(parent)
{
    setHandlesChildEvents(false);
    m_cursorHorizonal = new LineItem(this);
    m_cursorHorizonal->installSceneEventFilter(this);
    m_cursorVertical = new LineItem(this);
    m_cursorVertical->installSceneEventFilter(this);
    m_cursorCrossPoint = new PointItem(this);
    m_cursorCrossPoint->installSceneEventFilter(this);
}

GraphicsCursorItem::~GraphicsCursorItem() = default;

QPointF GraphicsCursorItem::cursorPosition() const
{
    return m_cursorPosition;
}

Qt::Orientations GraphicsCursorItem::orientations() const
{
    return m_orientations;
}

QColor GraphicsCursorItem::cursorColor() const
{
    return m_cursorColor;
}

void GraphicsCursorItem::setCursorPosition(const QPointF &pos)
{
    if (m_cursorPosition.y() == pos.y() && m_cursorPosition.x() == pos.x()) {           //notice: if x or y == Q_INFINITY
        return;
    }
    m_cursorPosition = pos;

    if (m_positionCallback) {
        m_positionCallback(pos);
    }

    QRectF sceneRect = m_cursorHorizonal->scene()->sceneRect();
    if (qIsFinite(pos.y())) {
        m_cursorHorizonal->setPos(0, pos.y());                                          //set line pos
        m_cursorHorizonal->setLine(sceneRect.left(), 0, sceneRect.right(), 0);          //draw line where y = 0
    }
    if (qIsFinite(pos.x())) {
        m_cursorVertical->setPos(pos.x(), 0);
        m_cursorVertical->setLine(0, sceneRect.bottom(), 0, sceneRect.top());
    }
    if (qIsFinite(pos.x()) && qIsFinite(pos.y())) {
        m_cursorCrossPoint->setPos(pos.x(), pos.y());
        m_cursorCrossPoint->setRect(-CROSSPOINT_SIZE, -CROSSPOINT_SIZE, CROSSPOINT_SIZE * 2, CROSSPOINT_SIZE * 2);
        m_cursorCrossPoint->setPen(Qt::NoPen);
    }

    updateVisibility();     //position will affect visiblility
}

void GraphicsCursorItem::setOrientations(Qt::Orientations orientations)
{
    if (m_orientations == orientations) {
        return;
    }
    m_orientations = orientations;
    updateVisibility();
}

void GraphicsCursorItem::setCursorColor(const QColor &color)
{
    if (m_cursorColor == color) {
        return;
    }
    m_cursorColor = color;
    QPen pen(m_cursorColor);
    pen.setCosmetic(true);
    m_cursorHorizonal->setPen(pen);
    m_cursorVertical->setPen(pen);
}

void GraphicsCursorItem::updateVisibility()
{
    bool isHorVisible = m_orientations.testFlag(Qt::Horizontal) && qIsFinite(m_cursorPosition.y());
    bool isVerVisible = m_orientations.testFlag(Qt::Vertical) && qIsFinite(m_cursorPosition.x());
    m_cursorHorizonal->setVisible(isHorVisible);
    m_cursorVertical->setVisible(isVerVisible);
    m_cursorCrossPoint->setVisible(isHorVisible && isVerVisible);
}

bool GraphicsCursorItem::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    if (watched == m_cursorHorizonal || watched == m_cursorVertical || watched == m_cursorCrossPoint) {
        if (event->type() == QEvent::GraphicsSceneContextMenu) {
            if (m_menuCallback) {
                m_menuCallback();
            }
            return true;
        }
    }
    return QGraphicsItem::sceneEventFilter(watched, event);
}

void GraphicsCursorItem::setMenuCallback(const std::function<void ()> &callback)
{
    m_menuCallback = callback;
}

void GraphicsCursorItem::setPositionCallback(const std::function<void (QPointF)> &callback)
{
    m_positionCallback = callback;
}

void GraphicsCursorItem::setMovable(bool isMovable)
{
    if (isMovable) {
        m_cursorHorizonal->setCursor(Qt::SizeVerCursor);
        m_cursorVertical->setCursor(Qt::SizeHorCursor);
        m_cursorCrossPoint->setCursor(Qt::SizeAllCursor);
        m_cursorHorizonal->setPositionCallback([=](QPointF pos) {
            setCursorPosition({m_cursorVertical->x(), pos.y()});
            return QPointF(0, pos.y());                     //still keep x = 0
        });
        m_cursorVertical->setPositionCallback([=](QPointF pos) {
            setCursorPosition({pos.x(), m_cursorHorizonal->y()});
            return QPointF(pos.x(), 0);
        });
        m_cursorCrossPoint->setPositionCallback([=](QPointF pos) {
            setCursorPosition({pos.x(), pos.y()});
            return QPointF(pos.x(), pos.y());
        });
    } else {
        m_cursorHorizonal->setCursor(Qt::ArrowCursor);
        m_cursorVertical->setCursor(Qt::ArrowCursor);
        m_cursorCrossPoint->setCursor(Qt::ArrowCursor);
    }
    m_cursorHorizonal->setFlag(QGraphicsItem::ItemIsMovable, isMovable);
    m_cursorVertical->setFlag(QGraphicsItem::ItemIsMovable, isMovable);
    m_cursorCrossPoint->setFlag(QGraphicsItem::ItemIsMovable, isMovable);
}
