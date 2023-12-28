#include "AScanGraphicsView.h"
#include "ui_AbstractGraphicsView.h"
#include "GraphicsEditablePolygon.h"
#include "GraphicsGateItem.h"
#include "UTGraphicsScene.h"
#include <Core/ICore.h>
#include <QElapsedTimer>
#include <QInputDialog>
#include <QLoggingCategory>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QtMath>
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/Util.h>
#include <utils/algorithm.h>

static qreal MAX_DAC = 200;
static const int COUNT_MAX_AUTOTCG = 10;
static const int INTERVAL_TIMER_AUTOTCG = 600;
static const double INTERVAL_GAIN_TCG = 0.1;

static Q_LOGGING_CATEGORY(debug, "waveform.ascan")

/*! GraphicsWaveformItem 是波形显示专用的图元类, 可以将波形数据显示出来.
 * 当波形数据较短时, 绘制效果类似 QGraphicsPolygonItem.
 * 当波形数据极长时, 会将其归并为(min, max)对, 降低绘图量.
 *
 * 绘图的座标系, 在X方向上为0-waveform.size, Y方向上为0-waveformMax.
 * 选用量纲是为了对性能进行优化, 使用时应当给此图元设置缩放矩阵, 将其缩放到实际尺寸.
 */
class GraphicsWaveformItem : public QAbstractGraphicsShapeItem
{
public:
    GraphicsWaveformItem(QGraphicsItem *parent) : QAbstractGraphicsShapeItem(parent) {}

    QRectF boundingRect() const override
    {
        return QRectF(0, 0, waveform.size(), UTDataFrame::waveformMax());
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
    {
//        QElapsedTimer timer;
//        timer.start();
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->setPen(pen());
        painter->setBrush(brush());

        /*! 波形点的密度, 即水平方向上每像素可容纳的波形点数 */
        qreal pointPerPixel = 1 / painter->transform().m11();

        /* 无缓存, 或已缓存的点密度与正在绘制的密度不一致 */
        if (!path || !qFuzzyCompare(pointPerPixel, pointDensity)) {
            delete path;                                                        /* 析构空指针安全 */
            path = new QPainterPath;
            pointDensity = pointPerPixel;

            int length = waveform.length();

            /* 每个像素超过4个点时, 进行归并 */
            if (pointPerPixel > 4) {
                int step = qFloor(pointPerPixel);                               /*!< 归并的步长, 为了提高性能使用int类型. */
                for (int start = 0, end = step; end < length; start += step, end += step) {
                    ADataValue min = waveform[start];
                    ADataValue max = min;
                    for (int index = start + 1; index < end; ++index) {
                        min = qMin(min, waveform[index]);
                        max = qMax(max, waveform[index]);
                    }
                    path->lineTo(start, min);
                    path->lineTo(start, max);
                }
            } else {
                for (int index = 0; index < length; ++index) {
                    path->lineTo(index, waveform.at(index));
                }
                path->lineTo(length - 1, 0);
            }
        }
        painter->drawPath(*path);
//        qDebug() << "Draw took time" << timer.elapsed();
    }

    void setWaveform(const ADataFrame &w)
    {
        if (waveform.size() != w.size()) {
            prepareGeometryChange();
        }
        waveform = w;
        delete path;
        path = nullptr;
        update();
    }

private:
    ADataFrame waveform;
    QPainterPath *path = nullptr;                                             /*!< 绘图路径缓存 */
    qreal pointDensity = Q_SNAN;                                                /*!< 缓存中的绘图点密度 */
};

AScanGraphicsView::AScanGraphicsView(QWidget *parent) :
    AbstractGraphicsView(parent),
    handler(this, &AScanGraphicsView::setup, &AScanGraphicsView::setSetup)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &AScanGraphicsView::onTimeOut);

    auto *root = rootItem();

    waveItem = new GraphicsWaveformItem(root);

    cursorH = new GraphicsInvertedLine(root);
    cursorV = new GraphicsInvertedLine(root);

    dacItem = new GraphicsEditablePolygon(root);
    dacItem->setPen(QPen(Qt::green, 0));
    dacItem->setEditable(true);
    dacItem->setMoveValidator([](QPointF newPos, const QPolygonF &polygon, int index) {
        qreal left = 0;
        if (index > 0) {
            left = qMax<qreal>(0, polygon[index - 1].x());
        }
        qreal right = qInf();
        if (index < polygon.count() - 1) {
            right = polygon[index + 1].x();
        }
        newPos.rx() = qBound(left + 1e-6, newPos.x(), right - 1e-6);
        newPos.ry() = qMax<qreal>(0, newPos.y());
        return newPos;
    });
    connect(dacItem, &GraphicsEditablePolygon::polygonChanged, this, [=](QPolygonF polygon) {
        InterpolateTable dac;
        for (const auto &point: polygon) {
            qreal amp = qMin(UltrasonicManager::decibelToAmp(MAX_DAC), point.y());
            qreal db = UltrasonicManager::ampToDecibel(amp, 80);
            dac[qMax<qreal>(0, point.x())] = qIsFinite(db) ? db : -MAX_DAC;
        }
        handler.set(&UTSetup::setDac, dac);
    });

    ui->view->setDragMode(QGraphicsView::NoDrag);
    initUi();

    updateControls();
    updatePlotStyle();
}

AScanGraphicsView::~AScanGraphicsView() = default;

/*! 处理各标尺事件 */
bool AScanGraphicsView::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->bottomRuler) {
        if (event->type() == QEvent::MouseButtonDblClick) {                     // 双击事件需要弹出时基界面
            emit onHorizontalRulerMouseDoubleClicked();
        } else if (event->type() == QEvent::MouseButtonPress) {
            auto *mouseEvent = static_cast<QMouseEvent *>(event);
            isBottomRulerPressed = true;
            mouseStartPos = mouseEvent->pos();
        } else if (event->type() == QEvent::MouseButtonRelease) {
            isBottomRulerPressed = false;
        } else if (event->type() == QEvent::MouseMove) {                        // 拖动标尺
            if (isBottomRulerPressed && m_horAxis->isDragEnabled()) {
                auto *mouseEvent = static_cast<QMouseEvent *>(event);
                Range selection = m_horAxis->selection();
                QPoint pointOffset = mouseStartPos - mouseEvent->pos();
                qreal offset = pointOffset.x();
                offset = Range( 0, ui->bottomRuler->width() ).scaleValue( offset, Range(0, selection.length()) );
                const int direction = isReversed(Qt::Horizontal) ? -1 : 1;
                Range newRange = selection.movedBy(direction * offset);         // 需要判断是递减还是递增

                UTSetup setup = m_setup;
                setup.setDelay(qMax<qreal>(newRange.min(), 0));
                emit setupChanged(setup);
                mouseStartPos = mouseEvent->pos();
            }
        } else if (event->type() == QEvent::ContextMenu) {
            auto *mouseEvent = static_cast<QContextMenuEvent *>(event);
            QMenu menu;
            menu.addAction(tr("设置为零点"), [=]() {
                Transform1D transform = m_horAxis->sceneTransform();
                qreal zero = transform.mapInverted(mouseEvent->pos().x()) + m_horAxis->selection().min();
                handler.set(&UTSetup::setZero, zero);
            });
            menu.exec(QCursor::pos());
        }
    }
    return AbstractGraphicsView::eventFilter(watched, event);
}

UTSetup AScanGraphicsView::setup() const
{
    return m_setup;
}

bool AScanGraphicsView::isSetupLocked() const
{
    return m_setupLocked;
}

UTDataFrame AScanGraphicsView::lastFrame() const
{
    return m_lastFrame;
}

qreal AScanGraphicsView::tcgSyncPos(const int gateIndex)
{
    if (gateIndex > m_gateItems.count()) {
        return 0;
    }

    if (m_lastFrame.IsIGateTriggered()) {
        if (gateIndex != 0) {
            return m_lastFrame.gateTime(gateIndex);
        } else {
            return m_lastFrame.syncTime() + m_setup.delay();
        }
    }
    if (m_gateItems.value(gateIndex)) {
        UTSetupGate gate = m_setup.gate(gateIndex);
        return gate.start();
    }
    return 0;
}

void AScanGraphicsView::refreshFocalPos()
{
    if (!m_channel) {
        return;
    }
    Probe curProbe = m_channel->probe();
    if (curProbe.uuid.isNull()) {
        m_probeFocalPosCross->setVisible(false);
        return;
    }
    m_probeFocalPosCross->setVisible(true);
    qreal xPosTime = curProbe.effectFocalPos(m_setup.materialSpeed(), m_lastFrame.waterPathLength(), QString("time"));
    qreal xPosDistance = curProbe.effectFocalPos(m_setup.materialSpeed(), m_lastFrame.waterPathLength(), QString("distance"));
    m_probeFocalPosCross->setToolTip(tr("探头焦点在%1mm的位置").arg(xPosDistance));
    m_probeFocalPosCross->setX(xPosTime);
    m_probeFocalPosCross->setY(40);
}

AScanStyle AScanGraphicsView::plotStyle() const
{
    return m_plotStyle;
}

void AScanGraphicsView::setSetup(const UTSetup &setup)
{
    if (m_setup == setup) {
        return;
    }
    m_setup = setup;
    updateControls();
    refreshFocalPos();
    emit setupChanged(setup);
}

void AScanGraphicsView::setSetupLocked(bool setupLocked)
{
    m_setupLocked = setupLocked;
}

void AScanGraphicsView::setPlotStyle(AScanStyle plotStyle)
{
    m_plotStyle = plotStyle;
    updatePlotStyle();
}

void AScanGraphicsView::initUi()
{
    setRulerPos(Qt::LeftEdge | Qt::BottomEdge);
    m_verAxis->setResizePolicy(GraphicsAxis::ResizeKeepSelection);
    m_horAxis->setResizePolicy(GraphicsAxis::ResizeKeepSelection);

    // 加载触发阈值标记
    m_triangleRoot = new QGraphicsPathItem(rootItem());
    m_verticalTriangle = new GraphicsMarkItem(m_triangleRoot);
    m_verticalTriangle->setShape(10, GraphicsMarkItem::RightArrowTriangle);
    m_verticalTriangle->setVisible(false);
    m_verticalTriangle->setBrush(Qt::blue);
    m_verticalTriangle->setPen(QPen(Qt::blue));
    m_verticalTriangle->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_verticalTriangle->setPositionCallback([=](QPointF, QPointF pos) -> QPointF {
        auto setup = m_setup;
        UTSetupSurfaceTracing surface = setup.surfaceTracing();
        if (pos.y() < 0) {
            pos.setY(0);
        } else if (pos.y() > 100) {
            pos.setY(100);
        }
        surface.threshold = pos.y();
        pos.setX(0);
        setup.setSurfaceTracing(surface);
        setSetup(setup);
        return pos;
    });
    m_horizontalTriangle = new GraphicsMarkItem(m_triangleRoot);
    m_horizontalTriangle->setShape(10, GraphicsMarkItem::UpArrowTriangle);
    m_horizontalTriangle->setVisible(false);
    m_horizontalTriangle->setBrush(Qt::red);
    m_horizontalTriangle->setPen(QPen(Qt::red));
    m_horizontalTriangle->setFlag(QGraphicsItem::ItemIsMovable, true);
    m_horizontalTriangle->setPositionCallback([=](QPointF, QPointF pos) -> QPointF {
        auto setup = m_setup;
        UTSetupSurfaceTracing surface = setup.surfaceTracing();
        if (pos.x() < 0) {
            pos.setX(0);
        }
        surface.offset = pos.x();
        pos.setY(0);
        setup.setSurfaceTracing(surface);
        setSetup(setup);
        return pos;
    });

    // 加载探头焦点在工件中的位置
    m_probeFocalPosCross = new GraphicsMarkItem(rootItem());
    m_probeFocalPosCross->setShape(5, GraphicsMarkItem::Cross);
    m_probeFocalPosCross->setVisible(false);
    m_probeFocalPosCross->setBrush(Qt::red);
    m_probeFocalPosCross->setPen(QPen(Qt::red));
    m_probeFocalPosCross->setFlag(QGraphicsItem::ItemIsMovable, false);

    // 设置场景
    setGridEnable(true);
    setBackgroundColor(Qt::black);
    setRulerUnitName(Qt::LeftEdge, "%");
    setRulerUnitName(Qt::BottomEdge, "mm");
    // 设置标尺
    // 对于A扫面板来说，纵向方向的Range与selection必须保持一致，因为在闸门绘制时，需要用到scene的纵向显示范围，只能通过sceneRect获取
    setRange(Qt::Vertical, Range(0, 100));
    setRulerSelection(Qt::Vertical, Range(0, 100));
    setRange(Qt::Horizontal, Range(0, std::numeric_limits<short>::max()));

    m_horAxis->setDragEnable(true);

    connect(ui->view, &UTGraphicsView::mouseMoved, this, &AScanGraphicsView::onMouseMoved);

    ui->bottomRuler->installEventFilter(this);
}

int AScanGraphicsView::currentGateIndex() const
{
    GraphicsGateItem *gate = Utils::findOrDefault(m_gateItems, [] (GraphicsGateItem *item) {
        return item->isSelected();
    });
    return m_gateItems.indexOf(gate);
}

void AScanGraphicsView::setMonitorData(UTDataFrame data)
{
    m_lastFrame = data;
    ThrottleUpdate::call(this, &AScanGraphicsView::refreshFocalPos);
    ThrottleUpdate::call(this, &AScanGraphicsView::updateWaveform);
}

void AScanGraphicsView::setFeatureMarkVisible(bool visible)
{
    for (GraphicsGateItem *gate: m_gateItems) {
        gate->setMarkVisible(visible);
    }
}

void AScanGraphicsView::setWaveformFill(bool enabled)
{
    if (enabled) {
        waveItem->setBrush(waveItem->pen().color());
    } else {
        waveItem->setBrush(QBrush());
    }
}

void AScanGraphicsView::onMouseMoved(const QPointF &scenePos)
{
    cursorPos = mapFromScene(scenePos);
    updateCursor();
}

void AScanGraphicsView::updateWaveform()
{
    if (!m_lastFrame.isEmpty()) {
        /* 更新波形图元 */
        if (m_lastFrame.hasWaveform()) {
            const ADataFrame waveform = m_lastFrame.waveform();
            waveItem->setWaveform(waveform);

            QTransform transform = QTransform::fromScale(m_setup.range() / waveform.size(), 100.0 / UTDataFrame::waveformMax());
            transform *= QTransform::fromTranslate(m_setup.delay(), 0);
            waveItem->setTransform(transform);
        } else {
            waveItem->setWaveform(ADataFrame());
        }

        /* 更新闸门特征值 */
        for (int index = 0; index < m_gateItems.count(); ++index) {
            auto *gateItem = m_gateItems[index];

            qreal time = m_lastFrame.gateTime(index);
            if (index == 0) {
                time += m_lastFrame.gateTime(0, GateTrigger) + m_lastFrame.Context.GetDelay();
            }
            qreal amp = m_lastFrame.gateAmp(index);
            gateItem->setMarkPos(time, amp);

            /* 读取基准闸门的触发时间, 配置图元偏移 */
            const auto &gate = m_setup.gate(index);
            int sync = gate.syncIndex();
            if (sync >= 0 && sync < index) {
                qreal triggerTime = m_lastFrame.gateTime(sync, GateTrigger) + m_lastFrame.Context.GetDelay();
                gateItem->setPos(triggerTime, 0);
            } else {
                gateItem->setPos(0, 0);
            }
        }

        int index = m_setup.dacTimebase();
        qreal offset = m_lastFrame.gateTime(index, GateTrigger) + m_lastFrame.Context.GetDelay();
        dacItem->setPos(offset, 0);
    } else {
        for (auto item: m_gateItems) {
            item->setMarkPos(Q_SNAN, Q_SNAN);
            item->setPos(0, 0);
        }
        dacItem->setPos(0, 0);
        waveItem->setWaveform(ADataFrame());
    }

    updateCursor();
}

void AScanGraphicsView::setCursorEnabled(bool enabled)
{
    isCursorEnable = enabled;
    updateCursor();
}

void AScanGraphicsView::setGatesVisible(bool visible)
{
    for (GraphicsGateItem *gate: m_gateItems) {
        gate->setVisible(visible);
    }
}

void AScanGraphicsView::updateTimebaseScale()
{
    if (m_setup.timebaseUnit() == UTSetupTimebase::Distance) {
        qreal speed = m_setup.materialSpeed();
        setRulerScale(Qt::BottomEdge, 2e3 / speed);
    } else {
        setRulerScale(Qt::BottomEdge, 1.0);
    }
    // 距离显示模式的时候，需要重新画
    setRulerUnitName(Qt::BottomEdge, m_setup.timebaseSuffix());
}

void AScanGraphicsView::setChannel(const QPointer<UTChannel> &channel)
{
    if (m_channel == channel)
        return;

    m_channel = channel;
    refreshFocalPos();
    emit channelChanged(m_channel);
}

void AScanGraphicsView::updateControls()
{
    updateTimebaseScale();
    UTSetupSurfaceTracing surfaceTracing = m_setup.surfaceTracing();
    qreal delay = m_setup.delay();
    qreal range = m_setup.range();
    qreal zero = m_setup.zero();
    if (surfaceTracing.enable) {
        delay = 0;
        zero = surfaceTracing.offset;
    }
    m_horAxis->setSelection(Range(delay, delay + range));
    m_horAxis->setRulerOrigin(zero);
    m_triangleRoot->setPos(delay, 0);
    m_verticalTriangle->setY(surfaceTracing.threshold);
    m_horizontalTriangle->setX(surfaceTracing.offset);
    m_verticalTriangle->setVisible(surfaceTracing.enable);
    m_horizontalTriangle->setVisible(surfaceTracing.enable);

    /* 移除多余的闸门图元 */
    const QList<UTSetupGate> gates = m_setup.gates();
    while (m_gateItems.count() > gates.count()) {
        auto *item = m_gateItems.takeLast();
        if (item->isAncestorOf(dacItem)) {
            dacItem->setParentItem(m_sceneRoot);
        }
        delete item;
    }

    /* 添加不足的闸门图元 */
    while (m_gateItems.count() < gates.count()) {
        auto *item = new GraphicsGateItem(m_sceneRoot);
        m_gateItems << item;

        /* 监听闸门拖动信号 */
        connect(item, &GraphicsGateItem::lineDragged, this, [=](QLineF line) {
            if (m_setupLocked) {
                return;
            }
            int index = m_gateItems.indexOf(item);
            UTSetup setup = m_setup;
            auto gate = setup.gate(index);

            gate.setStart(qMax<qreal>(0, line.x1()));
            gate.setWidth(qMax<qreal>(0, line.x2() - line.x1()));
            gate.setThreshold(qBound<qreal>(0, line.y1(), 100));

            setup.setGate(index, gate);
            setSetup(setup);
        });
        connect(item, &GraphicsGateItem::gateItemMenuRequested, this, &AScanGraphicsView::onGateItemMenu);
    }
    Q_ASSERT(gates.count() == m_gateItems.count());
    /* 刷新闸门图元 */
    for (int index = 0; index < gates.count(); ++index) {
        const UTSetupGate &gate = gates[index];
        GraphicsGateItem *item = m_gateItems[index];
        item->setGate(gate);
        item->setTimebase(m_setup.timebase());
        QGraphicsItem *parent = m_gateItems.value(gate.syncIndex());
        if (!parent || item->isAncestorOf(parent)) {
            parent = m_sceneRoot;
        }
        if (parent != item) {
            item->setParentItem(parent);
        }
    }

    QPolygonF dacPlot;
    for (const auto &dacItem: m_setup.dacList()) {
        dacPlot << QPointF(dacItem.first, UltrasonicManager::decibelToAmp(qMin(MAX_DAC, dacItem.second), 80));
    }

    QSignalBlocker blocker(dacItem);
    dacItem->setPolygon(dacPlot);
    blocker.unblock();

    int dacSync = 0;
    if (dacSync < m_gateItems.count()) {
        dacItem->setParentItem(m_gateItems[dacSync]);
    } else {
        dacItem->setParentItem(m_sceneRoot);
    }
    dacItem->setTransform(QTransform::fromScale(1, UltrasonicManager::decibelToAmp(m_setup.gain(), 1)));
}

void AScanGraphicsView::updatePlotStyle()
{
    const auto &style = m_plotStyle;
    view()->setBackgroundBrush(style.background);
    view()->setGridEnable(style.gridEnable);
    waveItem->setPen(QPen(style.waveformOutline, 0));
    waveItem->setBrush(style.waveformFillEnable ? QBrush() : QBrush(style.waveformFill));
}

void AScanGraphicsView::updateCursor()
{
    QPointF waveformPos = cursorPos;

    if (isCursorEnable && !m_lastFrame.isEmpty() && qIsFinite(waveformPos)) {
        QRectF sceneRect = m_scene->sceneRect();
        qreal delay = m_setup.delay();
        qreal range = m_setup.range();
        double scale = m_lastFrame.waveformLength() / range;
        int num = scale * (waveformPos.x() - delay);
        int direction = m_verAxis->isReversed() ? -1 : 1;
        waveformPos.ry() = 100.0f / UTDataFrame::waveformMax() * m_lastFrame.waveformAt(num);

        m_horAxis->setCursorPos(waveformPos.x());
        m_verAxis->setCursorPos(waveformPos.y() * direction);
        cursorH->setLine(sceneRect.left(), waveformPos.y(), sceneRect.right(), waveformPos.y());
        cursorV->setLine(waveformPos.x(), -sceneRect.top(), waveformPos.x(), sceneRect.bottom());
        cursorH->show();
        cursorV->show();
    } else {
        cursorH->hide();
        cursorV->hide();
        m_horAxis->setCursorPos(Q_INFINITY);
        m_verAxis->setCursorPos(Q_INFINITY);
    }
}

void AScanGraphicsView::onGateItemMenu()
{
    int gateIndex = currentGateIndex();
    GraphicsGateItem *gateItem = m_gateItems.at(gateIndex);
    auto *menu = new QMenu(this);
    QAction *zoomTo = menu->addAction(tr("查看此闸门内波形"), this, &AScanGraphicsView::onZoomTo);
    if (m_setup.surfaceTracing().enable || gateIndex != 0) {
        zoomTo->setEnabled(false);
    }
    QAction *autoGain = menu->addAction(tr("自动增益"), this, &AScanGraphicsView::onAutoGain);
    autoGain->setEnabled(m_lastFrame.IsIGateTriggered());
    if (m_timer->isActive()) {
        autoGain->setEnabled(false);
    }
    QAction *addDac = menu->addAction(tr("添加DAC样点"), this, &AScanGraphicsView::onAddDac);
    if (m_setup.dacMode() != UTSetup::DacEnabled || gateIndex <= m_setup.dacTimebase()) {
        addDac->setEnabled(false);
    }
    menu->addSeparator();
    QAction *soundVelocity = menu->addAction(tr("自动计算声速"), this, &AScanGraphicsView::onAutoCalculateSoundVelocity);
    if (gateIndex == 0) {
        soundVelocity->setEnabled(false);
    }
    QAction *hLock = menu->addAction(tr("水平锁定"), gateItem, &GraphicsGateItem::setHorizontalLock);
    hLock->setCheckable(true);
    hLock->setChecked(gateItem->horizontalLock());
    QAction *vLock = menu->addAction(tr("垂直锁定"), gateItem, &GraphicsGateItem::setVerticalLock);
    vLock->setCheckable(true);
    vLock->setChecked(gateItem->verticalLock());
    QAction *lengthLock = menu->addAction(tr("拉伸锁定"), gateItem, &GraphicsGateItem::setLengthLock);
    lengthLock->setCheckable(true);
    lengthLock->setChecked(gateItem->lengthLock());
    menu->addAction(tr("解锁"), gateItem, &GraphicsGateItem::onUnlock);
    menu->exec(QCursor::pos());
}

void AScanGraphicsView::onAddDac()
{
    qreal syncPos = 0;
    int gateIndex = currentGateIndex();
    if (gateIndex != 0) {
        syncPos = tcgSyncPos(gateIndex);
    }
    UTSetup setup = m_setup;
    UTSetupGate gate = setup.gate(gateIndex);
    qreal percent = m_lastFrame.gateAmp(gateIndex);
    InterpolateTable dacTable = setup.dac();
    qreal gain = setup.gain();
    qreal dacGain = -(gain - UltrasonicManager::ampToDecibel(percent, 80));
    qreal gateStart = gate.start();
    qreal gateEnd = gateStart + gate.width();

    int count = dacTable.countInRange(gateStart, gateEnd);
    if (count != 0) {
        QString message = tr("是否删除闸门内已有的%1个样点\n确定删除吗").arg(count);
        int ret = QMessageBox::warning(nullptr, QString(),
                                               message,
                                               QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                               QMessageBox::Yes);
        if (ret == QMessageBox::Yes) {
            dacTable.removeInRange(gateStart, gateEnd);
        } else if (ret == QMessageBox::Cancel) {
            return;
        }
    }
    if (setup.dacMode() != UTSetup::DacEnabled) {
        QMessageBox::warning(nullptr, tr("警告"), tr("DAC未开启显示"));
    }
    dacTable.insert(syncPos, dacGain);
    handler.set(&UTSetup::setDac,dacTable);
}

void AScanGraphicsView::onZoomTo()
{
    int gateIndex = currentGateIndex();
    const GraphicsGateItem *gateItem = m_gateItems.at(gateIndex);
    const UTSetupGate gate = m_setup.gate(gateIndex);
    qreal start = gate.start() + gateItem->syncPos();
    qreal width = gate.width();
    qCDebug(debug) << "gate.start(): " << gate.start() << "ZoomTo gateItem->syncPos(): " << gateItem->syncPos();
    qCDebug(debug) << "ZoomTo gateIndex: " << gateIndex << "start: " << start << "width" << width;

    qreal delay = start - width * 0.1;
    qreal range = width * 1.2;
    handler.set(&UTSetup::setDelay, delay);
    handler.set(&UTSetup::setRange, range);
}

void AScanGraphicsView::onAutoGain()
{
    bool ok = false;
    qreal gain = QInputDialog::getDouble(this, tr("自动增益"), tr("目标波高"), 80, 0, 100, 1, &ok);
    if (!ok) {
        return;
    }
    m_gateIndex = currentGateIndex();
    m_referenceGain = gain;
    m_timeCount = 0;
    m_timer->start(INTERVAL_TIMER_AUTOTCG);
}

void AScanGraphicsView::onAutoCalculateSoundVelocity()
{
    int gateIndex = currentGateIndex();
    bool ok = false;
    qreal thickness = QInputDialog::getDouble(this, QString(), tr("工件厚度(mm):"), 0, 0, std::numeric_limits<qreal>::max(), 5, &ok);
    if (!ok) {
        return;
    }
    qreal time = m_lastFrame.gateTime(gateIndex) - m_lastFrame.gateTime(0);
    qCDebug(debug) << "Auto SoundVelocity: gateIndex:" << gateIndex << "thickness:" << thickness << "time:" << time;
    UTSetup setup = m_setup;
    auto meterial = setup.material();
    meterial.longitudinalVelocity = thickness / (time / 1000 / 2);
    setup.setMaterial(meterial);
    setSetup(setup);
}

void AScanGraphicsView::onTimeOut()
{
    //距离目标差距的增益
    qreal percent = m_lastFrame.gateAmp(m_gateIndex);
    qreal gainDiff = UltrasonicManager::ampToDecibel(percent, m_referenceGain);
    gainDiff = qBound<qreal>(-5, gainDiff, 5);
    if (qAbs(gainDiff) <= INTERVAL_GAIN_TCG) {                                  // 已经调整到了逼近增益调整的最小分辨率，不需要调整
        qCDebug(debug) << "Auto gain: stopped at attempt" << m_timeCount << "goal" << INTERVAL_GAIN_TCG << "achieved" << gainDiff;
        m_timeCount = 0;
        m_timer->stop();
        return;
    }
    // 获取当前通道
    int channelID = m_lastFrame.Context.GetChannelID();
    UTChannel *channel = UltrasonicManager::channel(channelID);
    if (!channel) {
        qCWarning(debug) << "Auto gain: stopped since channel is invalid";
        m_timeCount = 0;
        m_timer->stop();
        return;
    }
    qCDebug(debug) << "Auto gain: attempt" << m_timeCount << "amp" << percent << "gainDiff:" << gainDiff;
    if (channel->instrumentIndex() != 0 && channel->setup().alterSource()) { // 不是0通道，并且开启关联检测，这时候调节的是参考增益
        handler.set(&UTSetup::setGainRef,m_setup.gainRef() + gainDiff);
    } else {
        handler.set(&UTSetup::setGain,m_setup.gain() + gainDiff);
    }
    m_timeCount++;
    if (m_timeCount > COUNT_MAX_AUTOTCG) {
        qCDebug(debug) << "Auto gain: stopped at attempt" << m_timeCount << "maximum attempt reached";
        m_timeCount = 0;
        m_timer->stop();
    }
}
