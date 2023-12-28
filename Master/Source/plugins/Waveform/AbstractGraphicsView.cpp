#include "AbstractGraphicsView.h"
#include "ui_AbstractGraphicsView.h"
#include "UTGraphicsScene.h"
#include <QGraphicsItemGroup>
#include <QGraphicsSceneContextMenuEvent>
#include <QLabel>
#include <QMenu>
#include <QPaintEvent>
#include <QLoggingCategory>

static Q_LOGGING_CATEGORY(debug, "waveform.AGraphicsView")

const int edgeTypeId = qRegisterMetaType<Qt::Edge>();
const char widgetEdgeKey[] = "WidgetEdge";
Qt::Edge widgetEdge(QWidget *widget)
{
    return Qt::Edge(widget->property(widgetEdgeKey).value<int>());
}

const char rulerEdgeKey[] = "RulerEdge";
Qt::Edge rulerEdges(QWidget *widget)
{
    return Qt::Edge(widget->property(rulerEdgeKey).value<int>());
}

AbstractGraphicsView::AbstractGraphicsView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AbstractGraphicsView)
{
    ui->setupUi(this);
    initView();
}

AbstractGraphicsView::~AbstractGraphicsView()
{
    delete ui;
}

void AbstractGraphicsView::initView()
{
    // 设置场景
    m_scene = new UTGraphicsScene(ui->view);
    ui->view->setScene(m_scene);
    connect(m_scene, &UTGraphicsScene::contextMenuTriggered, this, &AbstractGraphicsView::onContextMenuTriggered);

    m_sceneRoot = new QGraphicsItemGroup;
    /* QGraphicsItemGroup 的构造中默认将HandlesChildEvents设置为true了,不符合我们的使用需求,
     * 此处需要在构造结束后将其设置为false,才能实现childItem各自处理自己的鼠标事件 */
    m_sceneRoot->setHandlesChildEvents(false);
    m_scene->addItem(m_sceneRoot);

    // 设置横纵方向轴
    m_horAxis = new GraphicsAxis(this);
    m_verAxis = new GraphicsAxis(this);
    m_horAxis->setOrientation(Qt::Horizontal);
    m_verAxis->setOrientation(Qt::Vertical);
    m_horAxis->setWidget(ui->view);
    m_verAxis->setWidget(ui->view);
    ui->view->setAxis(m_horAxis, m_verAxis);

    connect(m_horAxis, &GraphicsAxis::rangeChanged, this, &AbstractGraphicsView::onAxisRangeChanged);
    connect(m_verAxis, &GraphicsAxis::rangeChanged, this, &AbstractGraphicsView::onAxisRangeChanged);

    connect(m_horAxis, &GraphicsAxis::axisUpdated, this, &AbstractGraphicsView::onAxisUpdated);
    connect(m_verAxis, &GraphicsAxis::axisUpdated, this, &AbstractGraphicsView::onAxisUpdated);

    connect(ui->view, &UTGraphicsView::viewportUpdated, this, &AbstractGraphicsView::refreshRuler);
    connect(m_horAxis, &GraphicsAxis::rulerTransformChanged, this, &AbstractGraphicsView::refreshRuler);
    connect(m_verAxis, &GraphicsAxis::rulerTransformChanged, this, &AbstractGraphicsView::refreshRuler);
    // 设置轴的范围
    setRange(Qt::Horizontal, Range(-1e6, 1e6));
    setRulerSelection(Qt::Horizontal, Range(0, 200));
    setRange(Qt::Vertical, Range(-1e6, 1e6));

    ui->leftRuler->setProperty(widgetEdgeKey, Qt::LeftEdge);                    /* 标尺控件所在位置 */
    ui->rightRuler->setProperty(widgetEdgeKey, Qt::RightEdge);
    ui->topRuler->setProperty(widgetEdgeKey, Qt::TopEdge);
    ui->bottomRuler->setProperty(widgetEdgeKey, Qt::BottomEdge);

    ui->leftRuler->setProperty(rulerEdgeKey, Qt::RightEdge);                    /* 标尺底线在控件中的位置 */
    ui->rightRuler->setProperty(rulerEdgeKey, Qt::LeftEdge);
    ui->topRuler->setProperty(rulerEdgeKey, Qt::BottomEdge);
    ui->bottomRuler->setProperty(rulerEdgeKey, Qt::TopEdge);

    m_rulerWidgets << ui->leftRuler << ui->rightRuler << ui->topRuler << ui->bottomRuler;
    for (QWidget *widget: m_rulerWidgets) {
        widget->setUpdatesEnabled(false);                                       /* 标尺绘图由paintEvent完成，禁用所有标尺的绘图 */
    }

    // 设置默认标尺原点
    setRulerPos(Qt::LeftEdge | Qt::BottomEdge);
    setReversed(Qt::Vertical, true);
    m_sceneRoot->setTransform(QTransform::fromScale(1, -1));

    // 连接view与scene的信号
    connect(ui->view, &QGraphicsView::rubberBandChanged, this, &AbstractGraphicsView::rubberBandChanged);
    connect(ui->view, &UTGraphicsView::mouseMoved, this, &AbstractGraphicsView::mouseMoved);
    connect(ui->view, &UTGraphicsView::contextMenuRequest, this, &AbstractGraphicsView::contextMenuRequest);
    connect(ui->view, &UTGraphicsView::mouseDoubleClicked, this, &AbstractGraphicsView::mouseDoubleClicked);
    connect(m_horAxis, &GraphicsAxis::selectionChanged, this, [this](const Range &range){emit selectionChanged(Qt::Horizontal, range);} );
    connect(m_horAxis, &GraphicsAxis::rangeChanged, this, [this](const Range &range){emit rangeChanged(Qt::Horizontal, range);} );

    connect(m_verAxis, &GraphicsAxis::selectionChanged, this, [this](const Range &range){emit selectionChanged(Qt::Vertical, range);} );
    connect(m_verAxis, &GraphicsAxis::rangeChanged, this, [this](const Range &range){emit rangeChanged(Qt::Vertical, range);} );

    ui->view->installEventFilter(this);
}

/*! 设置四边标尺的开关 */
void AbstractGraphicsView::setRulerPos(Qt::Edges edges)
{
    ui->leftRuler->setVisible(edges & Qt::LeftEdge);
    ui->rightRuler->setVisible(edges & Qt::RightEdge);
    ui->topRuler->setVisible(edges & Qt::TopEdge);
    ui->bottomRuler->setVisible(edges & Qt::BottomEdge);
}

QList<GraphicsAxis *> AbstractGraphicsView::axes(Qt::Edges edge)
{
    QList<GraphicsAxis *> list;
    if ((edge & Qt::LeftEdge) || (edge & Qt::RightEdge)) {
        list << m_verAxis;
    }
    if ((edge & Qt::TopEdge) || (edge & Qt::BottomEdge)) {
        list << m_horAxis;
    }
    return list;
}

GraphicsAxis *AbstractGraphicsView::axis(Qt::Edge edge)
{
    if (edge == Qt::BottomEdge || edge == Qt::TopEdge) {
        return m_horAxis;
    }
    return m_verAxis;
}

QList<QWidget *> AbstractGraphicsView::rulerWidgets(Qt::Edges edges) const
{
    QList<QWidget *> list;
    if (edges & Qt::LeftEdge) {
        list << ui->leftRuler;
    }
    if (edges & Qt::RightEdge) {
        list << ui->rightRuler;
    }
    if (edges & Qt::TopEdge) {
        list << ui->topRuler;
    }
    if (edges & Qt::BottomEdge) {
        list << ui->bottomRuler;
    }
    return list;
}

QWidget *AbstractGraphicsView::rulerWidget(Qt::Edge edge) const
{
    if (edge == Qt::LeftEdge) {
        return ui->leftRuler;
    }
    if (edge == Qt::RightEdge) {
        return ui->rightRuler;
    }
    if (edge == Qt::TopEdge) {
        return ui->topRuler;
    }
    return ui->bottomRuler;
}

/*! 设置反向 */
void AbstractGraphicsView::setReversed(Qt::Orientations arg, bool isReverse)
{
    if (arg == Qt::Vertical) {
        m_verAxis->setReversed(isReverse);
    } else {
        m_horAxis->setReversed(isReverse);
    }
}

/*! 获取指定方向是否是反向 */
bool AbstractGraphicsView::isReversed(Qt::Orientation arg)
{
    return arg == Qt::Horizontal ? m_horAxis->isReversed() : m_verAxis->isReversed();
}

void AbstractGraphicsView::fitInViewAnimated(const QRectF &rect, Qt::AspectRatioMode aspectRatioMode)
{
    Q_UNUSED(aspectRatioMode);
    if (!m_scene) {
        return;
    }

    const QRectF viewRect = ui->view->rect();
    if (rect.isEmpty() || viewRect.isEmpty()) {
        return;
    }
    QTransform userTransform = ui->view->userTransform();
    QRectF userRect = userTransform.inverted().mapRect(rect);
    qreal ratioX = viewRect.width() / userRect.width();
    qreal ratioY = viewRect.height() / userRect.height();

    ui->view->setScaleAnimated(qMin(ratioX, ratioY));
    ui->view->setCenterAnimated(rect.center());
}

void AbstractGraphicsView::onAxisUpdated()
{
    QTransform transform;
    if (m_horAxis->isReversed()) {
        transform.scale(-1, 1);
    }
    if (m_verAxis->isReversed()) {
        transform.scale(1, -1);
    }
    m_sceneRoot->setTransform(transform);
}

/*! 根据global位置获取scene中对应的位置(对外显示的位置) */
QPointF AbstractGraphicsView::mapScenePosFromGlobal(const QPoint &pos)
{
    QPoint viewPos = ui->view->mapFromGlobal(pos);
    QPointF realPos = ui->view->mapToScene(viewPos);
    return m_sceneRoot->mapFromScene(realPos);
}

/*! 将Scene中的坐标映射到全局坐标 */
QPoint AbstractGraphicsView::mapToGlobalFromScene(const QPointF &pos)
{
    QPoint viewPoint = ui->view->mapFromScene(pos);
    return ui->view->mapToGlobal(viewPoint);
}

/*! 正常坐标系的rect转换到当前的scene中的视觉坐标系 */
QRectF AbstractGraphicsView::mapFromScene(const QRectF &rect)
{
    return m_sceneRoot->mapRectFromScene(rect);
}

QPointF AbstractGraphicsView::mapFromScene(const QPointF &pos)
{
    return m_sceneRoot->mapFromScene(pos);
}

QPoint AbstractGraphicsView::mapFromScene(const QPoint &pos)
{
    return m_sceneRoot->mapFromScene(pos).toPoint();
}

QGraphicsScene *AbstractGraphicsView::graphicsScene()
{
    return qobject_cast<QGraphicsScene *>(m_scene);
}

/*! 缩放开关 */
void AbstractGraphicsView::setZoomEnable(bool enable)
{
    ui->view->setZoomEnable(enable);
}

/*! 背景网格的绘制开关 */
void AbstractGraphicsView::setGridEnable(bool enable)
{
    ui->view->setGridEnable(enable);
}

/*! 设置绘图区的背景颜色 */
void AbstractGraphicsView::setBackgroundColor(const QColor &color)
{
    ui->view->setBackgroundBrush(color);
}

/*! 设置单位名称 */
void AbstractGraphicsView::setRulerUnitName(Qt::Edges edges, const QString &arg)
{
    for (GraphicsAxis *axis: axes(edges)) {
        axis->setUnitName(arg);
    }
}

/*! 设置范围 */
void AbstractGraphicsView::setRange(Qt::Orientations orientation, const Range &range)
{
    if (orientation & Qt::Horizontal) {
        m_horAxis->setRange(range);
    }
    if (orientation & Qt::Vertical) {
        m_verAxis->setRange(range);
    }
}

/*! 设置graphicsview的center（同步用直接设置而不用动画） */
void AbstractGraphicsView::setCenter(const QPointF &center)
{
    ui->view->setCenter(center);
}

/*! 设置显示范围 */
void AbstractGraphicsView::setRulerSelection(Qt::Orientations org, const Range &range)
{
    if (org & Qt::Horizontal) {
        m_horAxis->setSelection(range);
    }
    if (org & Qt::Vertical) {
        m_verAxis->setSelection(range);
    }
}

/*! 设置坐标轴显示的原点 */
void AbstractGraphicsView::setOrigin(const QPointF &origin)
{
    m_horAxis->setRulerOrigin(origin.x());
    m_verAxis->setRulerOrigin(origin.y());
}

/*! 增加获取场景矩形区接口 */
QRectF AbstractGraphicsView::sceneRect()
{
    return m_scene->sceneRect();
}

/*! 获取方向org上的范围 */
Range AbstractGraphicsView::selection(Qt::Orientation org)
{
    if (org == Qt::Horizontal) {
        return m_horAxis->selection();
    }
    return m_verAxis->selection();
}

/*! 右键菜单的响应接口 */
void AbstractGraphicsView::onContextMenuTriggered(const QList<QAction *> &actionList)
{
    if (actions.isEmpty() && actionList.isEmpty()) {
        return;
    }
    QMenu menu;
    menu.addActions(actions);
    menu.addSeparator();
    menu.addActions(actionList);
    menu.exec( QCursor::pos() );
}

void AbstractGraphicsView::onAxisRangeChanged()
{
    QRectF rect = Range::toRect(m_horAxis->range(), m_verAxis->range());
    m_scene->setSceneRect(mapFromScene(rect));
}

void AbstractGraphicsView::onZoomIn()
{
    ui->view->zoomIn();
}

void AbstractGraphicsView::onZoomOut()
{
    ui->view->zoomOut();
}

/*! 获取原点位置 */
double AbstractGraphicsView::origin(Qt::Orientation orientation)
{
    if (orientation == Qt::Horizontal) {
        return m_horAxis->rulerOrigin();
    } else {
        return m_verAxis->rulerOrigin();
    }
}

/*! 刷新标尺 */
void AbstractGraphicsView::refreshRuler()
{
    for (QWidget *widget: m_rulerWidgets) {
        update(widget->geometry());
    }
}

void AbstractGraphicsView::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    for (QWidget *widget: m_rulerWidgets) {
        if (widget->isVisible() && e->region().intersects(widget->geometry())) {
            QPainter painter(this);
            Qt::Edge edge = widgetEdge(widget);
            GraphicsAxis *axis = this->axis(edge);
            if (!axis) {
                continue;
            }
            QRect g = widget->geometry();
            Qt::Edge edges = rulerEdges(widget);
//            painter.translate(g.x(), g.y());
            axis->drawRuler(&painter, edges, g, g);
        }
    }
}

/*! 设置各方向上显示坐标系与场景坐标系的比例 */
void AbstractGraphicsView::setRulerScale(Qt::Edges edges, qreal ratio)
{
    for (GraphicsAxis *axis: axes(edges)) {
        axis->setRulerScale(ratio);
    }
}

void AbstractGraphicsView::ensureVisible(const QPointF &pos, int xmargin, int ymargin)
{
//  view coordinate         margin must is pixel
    QPoint posInView = ui->view->mapFromScene({pos.x(), pos.y()});
    QRect oldViewRect = ui->view->rect();
    QRect idealViewRect = oldViewRect.marginsRemoved({xmargin, ymargin, xmargin, ymargin});
    if (!idealViewRect.contains(posInView)) {
        QPointF posInScene = ui->view->mapToScene(posInView);
        ui->view->setCenterAnimated({posInScene.x(), -posInScene.y()});
    }
}

UTGraphicsView *AbstractGraphicsView::view() const
{
    return ui->view;
}

UTGraphicsScene *AbstractGraphicsView::scene() const
{
    return m_scene;
}

QGraphicsItemGroup *AbstractGraphicsView::rootItem() const
{
    return m_sceneRoot;
}
