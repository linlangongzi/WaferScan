#include "GraphicsGateItem.h"
#include <QDebug>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include <QtMath>
#include <Ultrasonic/UltrasonicManager.h>

static const int ENDING_LENGTH = 5;
static const int TEXT_MARGIN = 7;
static const QMarginsF interactMargin(3, 2, 3, 2);

static QPolygonF upArrow = QVector<QPointF>{QPointF(0, 0), QPointF(-3, 9), QPointF(3, 9)};

class LineEndingItem : public QGraphicsLineItem
{
public:
    LineEndingItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem *parent) :
        QGraphicsLineItem(x1, y1, x2, y2, parent)
    {
        prepareGeometryChange();
    }

    /*! 重载LineItem的shape。由于默认的LineItem::shape()为了处理线宽，做了部分特殊处理，并不
     * 会调用boundingRect计算碰撞模型。为了保证鼠标光标正常显示，此处将其结果扩大若干像素。
     * 由于boundingRect会调用shape()计算尺寸，所以不需要重载boundingRect() */
    QPainterPath shape() const override
    {
        QPainterPath path;
        path.addRect(QGraphicsLineItem::shape().controlPointRect().marginsAdded(interactMargin));
        return path;
    }
};

GraphicsGateItem::GraphicsGateItem(QGraphicsItem *parent) :
    QGraphicsObject(parent),
    mainLine(new QGraphicsLineItem(this)),
    leftEnding(new LineEndingItem(0, -ENDING_LENGTH, 0, ENDING_LENGTH, this)),
    rightEnding(new LineEndingItem(0, -ENDING_LENGTH, 0, ENDING_LENGTH, this)),
    mark(new GraphicsMarkItem(this))
{
    setFlag(ItemIsSelectable, true);

    QPen pen(m_gate.color(), 0);
    leftEnding->setPen(pen);
    leftEnding->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    rightEnding->setPen(pen);
    rightEnding->setFlag(QGraphicsItem::ItemIgnoresTransformations);

    mainLine->setPen(pen);

    mark->setPen(pen);
    mark->hide();

    setGeometry(QLineF(100, 95, 200, 95));

    setAcceptHoverEvents(true);
    updateCursor();
    prepareGeometryChange();
    update();
}

qreal GraphicsGateItem::syncPos() const
{
    return pos().x();
}

bool GraphicsGateItem::horizontalLock() const
{
    return m_horizontalLock;
}

bool GraphicsGateItem::verticalLock() const
{
    return m_verticalLock;
}

bool GraphicsGateItem::lengthLock() const
{
    return m_lengthLock;
}

QRectF GraphicsGateItem::boundingRect() const
{
    return QRectF(-200000, -100, 400000, 200);
}

QPainterPath GraphicsGateItem::shape() const
{
    if (!scene()) {
        return QPainterPath();
    }
    QRectF left = mapRectFromItem(leftEnding, leftEnding->boundingRect());      /*!< 左边线在本图元中的几何位置 */
    QRectF right = mapRectFromItem(rightEnding, rightEnding->boundingRect());   /*!< 右边线在本图元中的几何位置 */

    /* 遍历所有视口，计算左右边缘线的像素高度，在当前图元中高度的最大值 */
    qreal height = 0;
    qreal width = 0;
    foreach (QGraphicsView *view, scene()->views()) {
        QTransform dev = deviceTransform(view->viewportTransform()).inverted();
        const QRectF devRect = dev.mapRect(left);
        height = qMax(devRect.height(), height);
        width = qMax(devRect.width(), width);
    }
    /* 调整左右边缘线的高度 */
    qreal heightExpand = (height - left.height()) / 2;
    qreal widthExpand = (width - left.width()) / 2;
    left.adjust(-widthExpand, -heightExpand, widthExpand, heightExpand);        /* 由于此尺寸的单位是像素，保持其中心位置不变，将尺寸调整为图元坐标系中的尺寸 */
    right.adjust(-widthExpand, -heightExpand, widthExpand, heightExpand);

    QPainterPath path;
    path.addRect(left.united(right));
    return path;
}

void GraphicsGateItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(m_gate.color(), 0));
    painter->setBrush(m_gate.color());
    const QTransform &worldTransform = painter->worldTransform();

    QString text = m_gate.name();
    if (qIsFinite(mark->x()) && !qFuzzyIsNull(mark->x())) {
        text += tr("\n%2 (%3)").arg(UltrasonicManager::ampText(mark->y(), false), timebase.timebaseText(mark->x()));
    }
    /* 视口和前后沿坐标，所有单位均为像素 */
    QRectF viewport = painter->viewport();
    QPointF leftPos = worldTransform.map(leftEnding->pos());
    QPointF rightPos = worldTransform.map(rightEnding->pos());
    QSizeF textSize = option->fontMetrics.size(0, text);
    qreal halfTextHeight = textSize.height() / 2;

    /* 计算基线位置，即文字水平中线 */
    qreal baselineY = leftPos.y() - TEXT_MARGIN - halfTextHeight;
    if (rightPos.x() < viewport.left() || leftPos.x() > viewport.right()) {
        baselineY = leftPos.y();
    }
    if (baselineY - halfTextHeight <= viewport.top()) {
        baselineY = leftPos.y() + TEXT_MARGIN + halfTextHeight;
    }
    qreal baselineX = leftPos.x();

    painter->setWorldTransform(QTransform());
    /* 如果后沿超出视口后沿，画一个指向后面的箭头 */
    if (rightPos.x() > viewport.right()) {
        painter->translate(viewport.right() - 1, baselineY);
        qreal radians = qAtan2(rightPos.x() - viewport.right(), baselineY - rightPos.y());
        painter->rotate(qRadiansToDegrees(radians));
        painter->drawPolygon(upArrow);
        painter->resetTransform();
    }
    qreal arrowWidth = upArrow.boundingRect().height();
    /* 如果前沿超出视口前沿，画一个指向前面的箭头 */
    if (leftPos.x() < viewport.left()) {
        painter->translate(viewport.left() + 1, baselineY);
        qreal radians = qAtan2(viewport.left() - leftPos.x(), baselineY - leftPos.y());
        painter->rotate(-qRadiansToDegrees(radians));
        painter->drawPolygon(upArrow);
        painter->resetTransform();
        baselineX = viewport.left() + TEXT_MARGIN + arrowWidth;
    }
    /* 文字如果超出右边界，将其画在右箭头的左边 */
    qreal rightBound = viewport.right() - arrowWidth - TEXT_MARGIN - textSize.width();
    baselineX = qMin(baselineX, rightBound);
    /* 绘制闸门文字 */
    QRectF textRect(QPointF(baselineX, baselineY - halfTextHeight), textSize);
    painter->drawText(textRect, text);
}

void GraphicsGateItem::setGate(const UTSetupGate &newGate)
{
    QPen pen(newGate.color(), 0);
    mainLine->setPen(pen);
    leftEnding->setPen(pen);
    rightEnding->setPen(pen);
    mark->setPen(pen);

    m_gate = newGate;

    setVisible(newGate.isEnabled());
    setGeometry(QLineF(newGate.start(), newGate.threshold(), newGate.end(), newGate.threshold()));
}

void GraphicsGateItem::setTimebase(const UTSetupTimebase &newTimebase)
{
    timebase = newTimebase;
}

void GraphicsGateItem::setGeometry(const QLineF &line)
{
    mainLine->setLine(line);
    leftEnding->setPos(line.p1());
    rightEnding->setPos(line.p2());
    update();
}

void GraphicsGateItem::setHorizontalLock(bool locked)
{
    m_horizontalLock = locked;
    updateCursor();
}

void GraphicsGateItem::setVerticalLock(bool locked)
{
    m_verticalLock = locked;
    updateCursor();
}

void GraphicsGateItem::setLengthLock(bool locked)
{
    m_lengthLock = locked;
    updateCursor();
}

void GraphicsGateItem::setMarkPos(qreal time, qreal amp)
{
    if (qIsFinite(time) && qIsFinite(amp)) {
        mark->show();
        mark->setPos(time, amp);
    } else {
        mark->hide();
    }
    update();
}

void GraphicsGateItem::setMarkVisible(bool visible)
{
    mark->setVisible(visible);
}

void GraphicsGateItem::updateCursor()
{
    leftEnding->setCursor(m_lengthLock ? Qt::ArrowCursor : Qt::SizeHorCursor);
    rightEnding->setCursor(m_lengthLock ? Qt::ArrowCursor : Qt::SizeHorCursor);
    if (m_verticalLock && m_horizontalLock) {
        setCursor(Qt::ArrowCursor);
    } else if (m_verticalLock) {
        setCursor(Qt::SizeHorCursor);
    } else if (m_horizontalLock) {
        setCursor(Qt::SizeVerCursor);
    } else {
        setCursor(Qt::SizeAllCursor);
    }
}

void GraphicsGateItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        dragType = DragNone;
        dragStart = mainLine->line();
        QPointF rightPos = mapToItem(rightEnding, event->pos());
        QPointF leftPos = mapToItem(leftEnding, event->pos());

        QGraphicsView *view = event->widget() ? qobject_cast<QGraphicsView *>(event->widget()->parentWidget()) : nullptr;
        if (view) {
            QTransform viewTransform = view->viewportTransform();
            QPointF devicePos = deviceTransform(viewTransform).map(event->pos());
            rightPos = rightEnding->deviceTransform(viewTransform).inverted().map(devicePos);
            leftPos = leftEnding->deviceTransform(viewTransform).inverted().map(devicePos);
        }
        if (!m_lengthLock && rightEnding->contains(rightPos)) {
            dragType = DragRight;
        } else if (!m_lengthLock && leftEnding->contains(leftPos)) {
            dragType = DragLeft;
        } else if (contains(mapFromScene(event->scenePos()))) {
            dragType = DragMainLine;
        }
        event->accept();
    } else {
        QGraphicsObject::mousePressEvent(event);
    }
}

void GraphicsGateItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        dragType = DragNone;
        event->accept();
    }
    QGraphicsObject::mouseReleaseEvent(event);
}

void GraphicsGateItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton)) {
        return event->ignore();
    }
    QPointF diff = event->scenePos() - event->buttonDownScenePos(Qt::LeftButton);
    diff.ry() = -diff.y();
    QLineF moved = dragStart;
    if (dragType == DragMainLine) {
        if (m_verticalLock) {
            diff.ry() = 0;
        }
        if (m_horizontalLock) {
            diff.rx() = 0;
        }
        moved.translate(diff);
    } else if (dragType == DragLeft) {
        qreal x = qMin(moved.p1().x() + diff.x(), moved.p2().x());
        moved.setP1(QPointF(x, moved.p1().y()));
    } else if (dragType == DragRight) {
        qreal x = qMax(moved.p2().x() + diff.x(), moved.p1().x());
        moved.setP2(QPointF(x, moved.p1().y()));
    }
    event->accept();

    emit lineDragged(moved);
}

void GraphicsGateItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsObject::contextMenuEvent(event);
    QGraphicsScene *scene = this->scene();
    if (scene) {
        scene->clearSelection();
    }
    setSelected(true);

    emit gateItemMenuRequested();
}

void GraphicsGateItem::onUnlock()
{
    setHorizontalLock(false);
    setVerticalLock(false);
    setLengthLock(false);
}
