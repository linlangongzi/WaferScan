#include "GraphicsEditablePolygon.h"
#include <QGraphicsSceneContextMenuEvent>
#include <QKeyEvent>
#include <QLoggingCategory>
#include <QMenu>
#include <QPen>

static Q_LOGGING_CATEGORY(debug, "wave.edit.polygon")

static bool isClosed(const QPolygonF &polygon)
{
    return polygon.count() > 2 && polygon.isClosed();
}

GraphicsEditablePolygon::GraphicsEditablePolygon(QGraphicsItem *parent) :
    QGraphicsItemGroup(parent),
    polygonItem(new QGraphicsPathItem(this))
{
    setHandlesChildEvents(false);
}

QPolygonF GraphicsEditablePolygon::polygon() const
{
    return m_polygon;
}

bool GraphicsEditablePolygon::isEditable() const
{
    return m_editable;
}

QPen GraphicsEditablePolygon::pen() const
{
    return polygonItem->pen();
}

void GraphicsEditablePolygon::setPolygon(const QPolygonF &polygon)
{
    if (m_polygon == polygon) {
        return;
    }

    m_polygon = polygon;

    /* 计算控制点数量 */
    int pointCount = polygon.count();
    if (isClosed(polygon)) {
        pointCount -= 1;
    }

    /* 移除多余的控制点 */
    while (points.count() > pointCount) {
        delete points.takeLast();
    }
    /* 添加不足的控制点 */
    while (points.count() < pointCount) {
        createPoint();
    }
    /* 刷新所有图元 */
    for (int index = 0; index < pointCount; ++index) {
        points[index]->setPos(polygon[index]);
    }

    int lineCount = qMax(0, polygon.count() - 1);
    while (lines.count() > lineCount) {
        delete lines.takeLast();
    }
    while (lines.count() < lineCount) {
        createLine();
    }
    for (int index = 1; index < polygon.count(); ++index) {
        lines[index - 1]->setLine(QLineF(polygon[index - 1], polygon[index]));
    }

    QPainterPath path;
    path.addPolygon(polygon);
    polygonItem->setPath(path);

    emit polygonChanged(polygon);
}

void GraphicsEditablePolygon::setEditable(bool editable)
{
    if (m_editable == editable) {
        return;
    }

    m_editable = editable;
    for (auto *item: points) {
        item->setVisible(editable);
    }
    emit editableChanged(editable);
}

void GraphicsEditablePolygon::setPen(const QPen &pen)
{
    polygonItem->setPen(pen);
    for (auto *item: points) {
        item->setPen(pen);
        item->setBrush(pen.color());
    }
}

void GraphicsEditablePolygon::setMoveValidator(GraphicsEditablePolygon::MoveValidator validator)
{
    m_validator = validator;
}

void GraphicsEditablePolygon::startRecord()
{
    grabMouse();
    grabKeyboard();
    setFocus(Qt::OtherFocusReason);
}

void GraphicsEditablePolygon::reset()
{
    ungrabMouse();
    ungrabKeyboard();
    setPolygon(QPolygonF());
}

void GraphicsEditablePolygon::keyReleaseEvent(QKeyEvent *event)
{
    if (m_polygon.isEmpty() || m_polygon.isClosed()) {
        return;
    }
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Space) {
        if (m_polygon.count() < 3) {
            qCWarning(debug) << "PolygonRecordManager: can't finish polygon, insufficient points";
            return;
        }
        if (!m_polygon.isClosed()) {
            QPolygonF polygon = m_polygon;
            polygon.append(polygon.value(0));
            setPolygon(polygon);
        }
        ungrabMouse();
        ungrabKeyboard();
    }
}

void GraphicsEditablePolygon::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
    setFocus(Qt::MouseFocusReason);
}

void GraphicsEditablePolygon::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton) {
        QPolygonF polygon = m_polygon;
        polygon.append(event->pos());
        setPolygon(polygon);
    }
}

bool GraphicsEditablePolygon::sceneEventFilter(QGraphicsItem *watched, QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneContextMenu) {
        auto *e = static_cast<QGraphicsSceneContextMenuEvent *>(event);
        int markIndex = points.indexOf(static_cast<GraphicsMarkItem *>(watched));
        if (markIndex >= 0) {
            QMenu menu;
            menu.addAction(tr("删除"), [=]() {
                QPolygonF polygon = m_polygon;
                polygon.removeAt(markIndex);
                setPolygon(polygon);
            });
            menu.exec(QCursor::pos());
            event->accept();
            return true;
        }
        int lineIndex = lines.indexOf(static_cast<QGraphicsLineItem *>(watched));
        if (lineIndex >= 0) {
            QMenu menu;
            menu.addAction(tr("添加"), [=]() {
                QPolygonF polygon = m_polygon;
                polygon.insert(lineIndex + 1, e->pos());
                setPolygon(polygon);
            });
            menu.exec(QCursor::pos());
            event->accept();
            return true;
        }
    }
    return QGraphicsItemGroup::sceneEventFilter(watched, event);
}

void GraphicsEditablePolygon::createPoint()
{
    auto *item = new GraphicsMarkItem(this);
    item->setShape(3, GraphicsMarkItem::Rectangle);
    item->setVisible(m_editable);
    item->setBrush(polygonItem->pen().color());
    item->setPen(polygonItem->pen());
    item->setFlag(ItemIsMovable);

    /* 位置改变时刷新polygon */
    item->setPositionCallback([=](QPointF oldPos, QPointF newPos) -> QPointF {
        int index = points.indexOf(item);
        if (m_validator) {
            newPos = m_validator(newPos, m_polygon, index);
        }
        if (oldPos == newPos || !qIsFinite(newPos.x()) || !qIsFinite(newPos.y())) {
            return newPos;
        }
        if (uint(index) < uint(m_polygon.count())) {
            QPolygonF polygon = m_polygon;
            if (index == 0 && polygon.isClosed()) {
                polygon.last()= newPos;
            }
            polygon[index] = newPos;
            setPolygon(polygon);
        }
        return newPos;
    });

    /* 拦截处理contextMenu */
    item->installSceneEventFilter(this);

    points << item;
}

void GraphicsEditablePolygon::createLine()
{
    auto *item = new QGraphicsLineItem(this);
    item->setPen(polygonItem->pen());
    item->installSceneEventFilter(this);

    lines << item;
}
