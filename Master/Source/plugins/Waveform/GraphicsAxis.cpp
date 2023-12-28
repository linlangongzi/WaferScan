#include "GraphicsAxis.h"
#include "PlotterRuler.h"
#include <QPainter>
#include <QResizeEvent>
#include <QApplication>

GraphicsAxis::GraphicsAxis(QObject *parent) :
    QObject(parent),
    m_range(Range::infinite()),
    m_selection(0, 10),
    m_cursorPos(Q_INFINITY)
{
}

const Range &GraphicsAxis::range() const
{
    return m_range;
}

const Range &GraphicsAxis::selection() const
{
    return m_selection;                                                           /* DEPRECATED: selection可能大于range，将其限制在range当中。 */
}

Range GraphicsAxis::rulerSelection()
{
    return (m_userTransform * m_rulerTransform).mapInverted(m_selection);
}

qreal GraphicsAxis::selectionScale() const
{
    return m_selection.length() / pixelLength();
}

Qt::Orientation GraphicsAxis::orientation() const
{
    return m_orientation;
}

int GraphicsAxis::pixelLength() const
{
    if (bindWidget.isNull()) {
        return 0;
    }
    return pixelLength(bindWidget->size());
}

int GraphicsAxis::pixelLength(const QSize &size) const
{
    if (m_orientation == Qt::Vertical) {
        return size.height();
    } else {
        return size.width();
    }
}

/*! 获取每个像素代表的长度 */
double GraphicsAxis::lengthInPixel() const
{
    int pixel = pixelLength();
    if (pixel != 0) {
        return m_selection.length() / pixel;
    }
    return 0.00;
}

void GraphicsAxis::drawRuler(QPainter *painter, Qt::Edge edge, const QRect &rulerRect, const QRect &boundingRect)
{
    Range range = rulerSelection();
    PlotterRuler plotter;
    plotter.setUnitName(m_unitName);
    qreal pos = (m_userTransform * m_rulerTransform).mapInverted(m_cursorPos * direction());
    bool isReverse = m_reversed;
    plotter.drawRuler(painter, range, rulerRect, boundingRect, edge, isReverse, pos);
}

Transform1D GraphicsAxis::sceneTransform() const
{
    return Transform1D(pixelLength() / m_selection.length() * direction());
}

Transform1D GraphicsAxis::userTransform() const
{
    return m_userTransform;
}

Transform1D GraphicsAxis::GetTransform() const
{
    return sceneTransform() * m_userTransform;
}

Transform1D GraphicsAxis::rulerTransform() const
{
    return m_rulerTransform;
}

/*! 给当前slider设置关联的Widget */
void GraphicsAxis::setWidget(QWidget *widget)
{
    bindWidget = widget;
    if (bindWidget) {
        bindWidget->removeEventFilter(this);
    }
    if (widget) {
        widget->installEventFilter(this);
    }
    updateSelection();
}

/*! 设置可选的范围上限和下限 */
void GraphicsAxis::setRange(const Range &newRange)
{
    bool changed = !newRange.isEqual(m_range);
    m_range = newRange;
    if (changed) {
        emit rangeChanged(m_range);
    }

    updateSelection();                                                          /* 可选范围改变时，可能导致选择范围落在可选范围之外，使用SetSelection刷新可选范围。 */
}

void GraphicsAxis::setSelection(const Range &newSelection)
{
    if (m_locked) {
        return;
    }
    Range newConstrained = newSelection.constrainedTo(m_range);
    if (!newConstrained.isEqual(m_selection) && qIsFinite(newConstrained.length())) {
        m_selection = newConstrained;
        updateSelection();
        emit selectionChanged(m_selection);
        emit axisUpdated();
    }
}

void GraphicsAxis::setUserTransform(const Transform1D &newTransform)
{
    if (m_userTransform == newTransform) {
        return;
    }
    m_userTransform = newTransform;
    emit axisUpdated();
}

void GraphicsAxis::setRulerTransform(const Transform1D &newTransform)
{
    if (m_rulerTransform == newTransform) {
        return;
    }
    m_rulerTransform = newTransform;
    emit rulerTransformChanged();
}

void GraphicsAxis::updateSelection()
{
    if (m_resizePolicy == ResizeKeepScale) {
        setSelection(Range::fromCenterWidth(m_center, pixelLength() * m_scale));
        updateSceneTransform();
    } else if (m_resizePolicy == ResizeKeepSelection) {
        qreal newCenter = m_selection.center();
        qreal newScale = m_selection.length() / pixelLength();
        bool hasScaleChanged = (newScale != m_scale);
        bool hasCenterChanged = (newCenter != m_center);
        m_center = newCenter;
        m_scale = newScale;
        if (hasScaleChanged) {
            emit scaleChanged(m_scale);
        }
        if (hasCenterChanged) {
            emit centerChanged(m_center);
        }
        if (hasScaleChanged || hasCenterChanged) {
            emit axisUpdated();
        }
    }
}

void GraphicsAxis::updateSceneTransform()
{
    Transform1D newTransform = sceneTransform();
    if (newTransform == m_sceneTransform) {
        return;
    }
    emit axisUpdated();
}

bool GraphicsAxis::eventFilter(QObject *o, QEvent *e)
{
    bool ret = QObject::eventFilter(o, e);
    if (o == bindWidget && e->type() == QEvent::Resize) {
        updateSelection();
    }
    return ret;
}

void GraphicsAxis::setRulerOrigin(qreal origin)
{
    if (qFuzzyCompare(origin, rulerOrigin())) {
        return;
    }
    m_rulerTransform.setOffsetBefore(origin);
    emit rulerTransformChanged();
}

void GraphicsAxis::setUserScale(qreal scale)
{
    if (qFuzzyCompare(scale, userScale())) {
        return;
    }
    m_userTransform.setScale(scale);
    emit axisUpdated();
}

void GraphicsAxis::setRulerScale(qreal scale)
{
    if (qFuzzyCompare(scale, rulerScale())) {
        return;
    }
    m_rulerTransform.setScale(scale);
    emit rulerTransformChanged();
}

void GraphicsAxis::setScale(qreal newScale)
{
    if (qFuzzyCompare(newScale, m_scale)) {
        return;
    }
    m_scale = newScale;
    updateSelection();
    emit scaleChanged(newScale);
}

void GraphicsAxis::setCursorPos(qreal pos)
{
    if (qFuzzyCompare(m_cursorPos, pos)) {
        return;
    }
    m_cursorPos = pos;
    emit axisUpdated();
}

void GraphicsAxis::setLocked(bool arg)
{
    m_locked = arg;
}

void GraphicsAxis::setUnitName(const QString &name)
{
    if (m_unitName == name) {
        return;
    }
    m_unitName = name;
    emit axisUpdated();
}

/*! 设置反向 */
void GraphicsAxis::setReversed(bool enable)
{
    if (enable == m_reversed) {
        return;
    }
    m_reversed = enable;
    emit axisUpdated();
}

void GraphicsAxis::setCenter(qreal newCenter)
{
    if (qFuzzyCompare(m_center, newCenter)) {
        return;
    }
    m_center = newCenter;
    updateSelection();
    emit centerChanged(newCenter);
}

void GraphicsAxis::setOrientation(Qt::Orientation arg)
{
    if (arg == m_orientation) {
        return;
    }
    m_orientation = arg;
    emit axisUpdated();
}
