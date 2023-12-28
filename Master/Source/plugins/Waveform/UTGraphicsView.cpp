#include "UTGraphicsView.h"
#include <allrising/Util.h>
#include "PlotterRuler.h"
#include "UTGraphicsScene.h"
#include <QPinchGesture>
#include <QtMath>

const qreal ZOOM_SCALE_STEP = 1.1;

UTGraphicsView::UTGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    setDragMode(RubberBandDrag);
    setMouseTracking(true);
    viewport()->setCursor(Qt::ArrowCursor);
    setCacheMode(QGraphicsView::CacheBackground);
    grabGesture(Qt::PinchGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::TapGesture);
    grabGesture(Qt::TapAndHoldGesture);
    grabGesture(Qt::SwipeGesture);
}

UTGraphicsView::~UTGraphicsView() = default;

QPointF UTGraphicsView::center()
{
    QPointF center;
    if (axisX) {
        center.setX(axisX->selection().center());
    }
    if (axisY) {
        center.setY(axisY->selection().center());
    }
    return center;
}

qreal UTGraphicsView::scale() const
{
    bool validX = axisX && axisX->resizePolicy() == GraphicsAxis::ResizeKeepScale;
    bool validY = axisY && axisY->resizePolicy() == GraphicsAxis::ResizeKeepScale;
    if (validX && validY) {
        return qMax(qAbs(axisX->GetTransform().scale()), qAbs(axisY->GetTransform().scale()));
    } else if (validX) {
        return qAbs(axisX->GetTransform().scale());
    } else if (validY) {
        return qAbs(axisY->GetTransform().scale());
    }
    return 1;
}

void UTGraphicsView::setScale(qreal value)
{
    if (!qIsFinite(value) || axisX.isNull() || axisY.isNull()) {
        return;
    }
    if (axisX && axisX->resizePolicy() == GraphicsAxis::ResizeKeepScale) {
        qreal userScale = axisX->userTransform().scale();
        axisX->setScale(userScale / value);
    }
    if (axisY && axisY->resizePolicy() == GraphicsAxis::ResizeKeepScale) {
        qreal userScale = axisY->userTransform().scale();
        axisY->setScale(userScale / value);
    }
}

/*! 重载的鼠标移动事件 */
void UTGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    // 鼠标拖动平移，不调用Qt默认的mouseMoveEvent，完全依靠GraphicsAxis控制场景的移动
    if (isMousePressed) {
        if (axisX.isNull() || axisY.isNull()) {
            return;
        }
        QPoint offsetPixel = mousePressedPos - event->pos();
        QPointF offset = sceneTransform().inverted().map(QPointF(offsetPixel));  /* 鼠标自按下以后，在场景坐标系中移动的总位移 */
        QPointF center = mousePressedCenter + offset;                           /* 新的视口中心 */

        setCenter(center);
        viewport()->setCursor(Qt::ClosedHandCursor);
    } else {
        QGraphicsView::mouseMoveEvent(event);
        emit mouseMoved(mapToScene(event->pos()));
    }
}

/*! 重载的鼠标按下事件 */
void UTGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (dragMode() == RubberBandDrag && event->button() & Qt::RightButton) {
        isMousePressed = true;
        mousePressedPos = event->pos();
        mousePressedCenter = center();
    } else {
        QGraphicsView::mousePressEvent(event);
    }
    emit mousePressed();
}

/*! 重载的鼠标弹起事件 */
void UTGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (!isMousePressed || viewport()->cursor().shape() != Qt::ClosedHandCursor) {
        QGraphicsView::mouseReleaseEvent(event);
        emit mouseReleased();
    }
}

/*! 重载的鼠标右键菜单 */
void UTGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    if (!isMousePressed || viewport()->cursor().shape() != Qt::ClosedHandCursor) {
        QGraphicsView::contextMenuEvent(event);
        if (!event->isAccepted()) {
            emit contextMenuRequest( mapToGlobal( event->pos() ) );
        }
    }
    viewport()->unsetCursor();
    isMousePressed = false;
}

void UTGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
    emit mouseDoubleClicked( mapToScene( event->pos() ) );
}

void UTGraphicsView::leaveEvent(QEvent *event)
{
    QGraphicsView::leaveEvent(event);
    emit mouseMoved(QPointF(Q_INFINITY, Q_INFINITY));
}

void UTGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (axisX.isNull() || axisY.isNull()) {
        return;
    }
    if (event->modifiers() & Qt::ControlModifier) {
        onMouseScrolled(event->delta());
    } else {
        QPointF mouseDelta = QPointF(event->pixelDelta());
#ifndef Q_OS_MAC
        if (mouseDelta.isNull()) {
            qreal direction = 0.1 * arsSign(event->delta());
            if (event->orientation() == Qt::Horizontal) {
                mouseDelta.setX(width() * direction);
            } else {
                mouseDelta.setY(height() * direction);
            }
        }
#endif
        QPointF sceneDelta = mouseDelta / scale();
        sceneDelta.rx() *= axisX->direction();
        sceneDelta.ry() *= axisY->direction();
        setCenter(center() - sceneDelta);
    }
}

bool UTGraphicsView::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture || event->type() == QEvent::GestureOverride) {
        auto *gesture = static_cast<QGestureEvent *>(event);
        auto *pinch = static_cast<QPinchGesture *>(gesture->gesture(Qt::PinchGesture));
        if (pinch && isZoomEnabled()) {
            static qreal pinchBaseScale;
            if (pinch->state() == Qt::GestureStarted) {
                pinchBaseScale = scale();
            }
            setScale(pinch->scaleFactor() * pinchBaseScale);
            gesture->accept(Qt::PinchGesture);
            return true;
        }
        auto *pan = static_cast<QPanGesture *>(gesture->gesture(Qt::PanGesture));
        if (pan) {
            static QPointF panBasePosition;
            if (pan->state() == Qt::GestureStarted) {
                panBasePosition = center();
            }
            const QTransform t = sceneTransform().inverted();
            const QPointF offset = t.map(pan->offset()) - t.map(QPointF());
            setCenter(panBasePosition - offset);
            gesture->accept(Qt::PanGesture);
            return true;
        }
    }
    return QGraphicsView::event(event);
}

/*! 绘制背景网格 */
void UTGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);
    if (!gridEnable || axisX.isNull() || axisY.isNull()) {
        return;
    }
    painter->fillRect(rect, backgroundBrush());                                 /* 填充背景色 */
    painter->setRenderHints(QPainter::Antialiasing, false);
    painter->setPen(QPen(Qt::gray, 0));                                     // 设置网格颜色

    QTransform rulerTransform = Transform1D::to2D(axisX->rulerTransform(), axisY->rulerTransform());
    painter->save();
    painter->setTransform(rulerTransform, true);
    QRectF r = rulerTransform.inverted().mapRect(rect);

    const PlotterRuler plotter;
    plotter.DrawGrid(painter, r);
    painter->restore();
}

void UTGraphicsView::updateViewport()
{
    if (axisX.isNull() || axisY.isNull()) {
        return;
    }
    /* 对于两个轴均为KeepScale的情况，保持长宽比 */
    if (axisX->resizePolicy() == GraphicsAxis::ResizeKeepScale && axisY->resizePolicy() == GraphicsAxis::ResizeKeepScale) {
        setScale(scale());
//        scaleX = axisX->selectionScale() * axisX->GetUnitRatio();
//        scaleY = axisY->selectionScale() * axisY->GetUnitRatio();
    }

    /* 更新视口的缩放比例及位置 */
    setTransform(QTransform::fromScale(axisX->sceneTransform().scale() * axisX->direction(), axisY->sceneTransform().scale() * axisY->direction()));
    QPointF centerPos = center();
    centerPos.rx() *= axisX->direction();
    centerPos.ry() *= axisY->direction();
    centerOn(centerPos);
    updateBackground();
    emit viewportUpdated();
}

void UTGraphicsView::updateBackground()
{
    QPointF topleft = mapToScene(0, 0);
    QPointF bottomRiht = mapToScene(width(), height());
    invalidateScene(QRectF(topleft, bottomRiht), QGraphicsScene::BackgroundLayer);
}

/*! 放大 */
void UTGraphicsView::zoomIn()
{
    onMouseScrolled(1);
}

/*! 缩小 */
void UTGraphicsView::zoomOut()
{
    onMouseScrolled(-1);
}

void UTGraphicsView::setCenterAnimated(const QPointF &newCenter)
{
    const QPointF prevCenter = center();
    if (newCenter == prevCenter) {
        return;
    }
    centerAnimation.stop();
    centerAnimation.setTargetObject(this);
    centerAnimation.setPropertyName("center");
    centerAnimation.setEasingCurve(QEasingCurve::InOutSine);
    centerAnimation.setStartValue(prevCenter);
    centerAnimation.setEndValue(newCenter);
    centerAnimation.start();
}

void UTGraphicsView::setScaleAnimated(double newScale)
{
    qreal prevScale = scale();
    if (scaleAnimation.state() == QPropertyAnimation::Running) {
        prevScale = scaleAnimation.endValue().toReal();
    }
    if (qFuzzyCompare(prevScale, newScale)) {
        return;
    }
    scaleAnimation.stop();
    scaleAnimation.setTargetObject(this);
    scaleAnimation.setPropertyName("scale");
    scaleAnimation.setEasingCurve(QEasingCurve::InOutSine);
    scaleAnimation.setStartValue(prevScale);
    scaleAnimation.setEndValue(newScale);
    scaleAnimation.start();
}

QTransform UTGraphicsView::sceneTransform() const
{
    Transform1D transformX = axisX ? axisX->sceneTransform() : Transform1D();
    Transform1D transformY = axisY ? axisY->sceneTransform() : Transform1D();
    return Transform1D::to2D(transformX, transformY);
}

QTransform UTGraphicsView::userTransform() const
{
    Transform1D transformX = axisX ? axisX->userTransform() : Transform1D();
    Transform1D transformY = axisY ? axisY->userTransform() : Transform1D();
    return Transform1D::to2D(transformX, transformY);
}

void UTGraphicsView::onMouseScrolled(int direction, const QPointF &center)
{
    qreal oldScale = scale();
    oldScale = direction > 0 ? oldScale * ZOOM_SCALE_STEP : oldScale / ZOOM_SCALE_STEP;
    setScale(oldScale);
    if (!center.isNull()) {
        setCenter(center);
    }
}

void UTGraphicsView::setAxis(GraphicsAxis *x, GraphicsAxis *y)
{
    if (axisX) {
        disconnect(axisX, nullptr, this, nullptr);
    }
    if (axisY) {
        disconnect(axisY, nullptr, this, nullptr);
    }
    axisX = x;
    axisY = y;
    if (axisX) {
        connect(axisX, &GraphicsAxis::axisUpdated, this, &UTGraphicsView::updateViewport, Qt::QueuedConnection);
        connect(axisX, &GraphicsAxis::rulerTransformChanged, this, &UTGraphicsView::updateBackground, Qt::QueuedConnection);
    }
    if (axisY) {
        connect(axisY, &GraphicsAxis::axisUpdated, this, &UTGraphicsView::updateViewport, Qt::QueuedConnection);
        connect(axisY, &GraphicsAxis::rulerTransformChanged, this, &UTGraphicsView::updateBackground, Qt::QueuedConnection);
    }
}

void UTGraphicsView::setGridEnable(bool enable)
{
    if (gridEnable == enable) {
        return;
    }
    gridEnable = enable;
    invalidateScene(QRectF(), QGraphicsScene::BackgroundLayer);
}

void UTGraphicsView::setCenter(const QPointF &pos)
{
    if (axisX) {
        axisX->setCenter(pos.x());
    }
    if (axisY) {
        axisY->setCenter(pos.y());
    }
}
