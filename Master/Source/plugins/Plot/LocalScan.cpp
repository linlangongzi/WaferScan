#include "LocalScan.h"
#include "ui_LocalScan.h"
#include <Core/Platform.h>
#include <Motion/MotionManager.h>
#include <Motion/TrioDriver.h>
#include <QGraphicsSceneMouseEvent>
#include <QLegendMarker>
#include <QLoggingCategory>
#include <QMessageBox>
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/ThrottleUpdate.h>

static const int DEFAULT_SIZE = 1000;
static Q_LOGGING_CATEGORY(debug, "plot.scan");

using namespace QtCharts;

LocalScan::LocalScan(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocalScan)
{
    ui->setupUi(this);

    const auto axes = MotionManager::axisList();
    for (Axis *axis: axes) {
        ui->axis->addItem(axis->name(), axis->config().axisId);
    }
    if (ui->axis->count()) {
        ui->axis->setCurrentIndex(0);
    }

    for (UTChannel *channel: UltrasonicManager::channels()) {
        ui->channel->addItem(channel->name(), channel->channelId());
    }
    if (ui->channel->count()) {
        ui->channel->setCurrentIndex(0);
    }

    QChart *chart = ui->feature->chart();
    auto *axisX = chart->axisX();
    m_axisAmp = chart->axisY();
    m_axisAmp->setRange(0, 100);

    m_axisTimeDisplay = new QValueAxis(chart);
    chart->addAxis(m_axisTimeDisplay, Qt::AlignRight);
    m_axisTimeDisplay->setRange(0, 100);

    m_axisTime = new QValueAxis(chart);
    chart->addAxis(m_axisTime, Qt::AlignRight);
    m_axisTime->hide();

    m_line = new QLineSeries(chart);
    chart->addSeries(m_line);
    m_line->attachAxis(axisX);
    m_line->attachAxis(m_axisAmp);

    m_ampSeries = new BScanSeries(chart);
    m_ampSeries->setName(tr("幅值"));
    chart->addSeries(m_ampSeries);
    m_ampSeries->attachAxis(axisX);
    m_ampSeries->attachAxis(m_axisAmp);

    m_timeSeries = new BScanSeries(chart);
    m_timeSeries->setName(tr("时间"));
    chart->addSeries(m_timeSeries);
    m_timeSeries->attachAxis(axisX);
    m_timeSeries->attachAxis(m_axisTime);
    ui->feature->setupSeries();

    for (QLegendMarker *marker: chart->legend()->markers(m_line)) {
        marker->setVisible(false);
    }

    chart->installEventFilter(this);
    resetSeries();
}

LocalScan::~LocalScan()
{
    delete ui;
}

int LocalScan::axisIndex()
{
    return ui->axis->currentIndex();
}

Axis *LocalScan::scanAxis() const
{
    const auto axisId = ui->axis->currentData(Qt::UserRole).toInt();
    const auto axes = MotionManager::axisList();
    for (Axis *axis: axes) {
        if (axis->config().axisId == axisId) {
            return axis;
        }
    }
    return nullptr;
}

UTChannel *LocalScan::channel() const
{
    const int channelIndex = ui->channel->currentData(Qt::UserRole).toInt();
    return UltrasonicManager::channel(channelIndex);
}

void LocalScan::onAxisStatusChanged(const AxisStatus &status)
{
    Axis *axis = scanAxis();
    if (!axis) {
        return;
    }

    auto type = status.moveType;
    const qreal pos = axis->position();
    QString errorString = tr("%1轴未停止在指定位置").arg(axis->name());

    if (m_status == PrepareLocate && type == AxisStatus::MOVEABS) {
        setStatus(Locate);
    } else if (m_status == Locate && type == AxisStatus::Idle) {
        if (isEqualWith(pos, m_startPos)) {
            setStatus(PrepareScan);
        } else {
            errorWarning(errorString.append(tr("(%1,%2").arg(pos).arg(m_startPos)));
        }
    } else if (m_status == PrepareScan && type == AxisStatus::MOVEABS) {
        setStatus(Scan);
    } else if (m_status == Scan && type == AxisStatus::Idle) {
        if (isEqualWith(pos, m_endPos) && qFuzzyCompare(m_endPos, m_originalPos)) {
            setStatus(Idle);
        } else if (isEqualWith(pos, m_endPos) && !qFuzzyCompare(m_endPos, m_originalPos)) {
            setStatus(PrepareRestore);
        } else {
            errorWarning(errorString.append(tr("(%1,%2").arg(pos).arg(m_endPos)));
        }
    } else if (m_status == PrepareRestore && type == AxisStatus::MOVEABS) {
        setStatus(Restore);
    } else if (m_status == Restore && type == AxisStatus::Idle) {
        if (isEqualWith(pos, m_originalPos)) {
            setStatus(Idle);
        } else {
            errorWarning(errorString.append(tr("(%1,%2").arg(pos).arg(m_originalPos)));
        }
    } else if (m_status == PrepareMoveTo && type == AxisStatus::MOVEABS) {
        setStatus(MoveTo);
    } else if (m_status == MoveTo && type == AxisStatus::Idle) {
        const qreal target = cursorX();
        if (isEqualWith(pos, target)) {
            setStatus(Idle);
        } else {
            errorWarning(errorString.append(tr("(%1,%2").arg(pos).arg(target)));
        }
    }
}

void LocalScan::onDataArrived(const UTDataSegment &segment)
{
    const int axisIndex = ui->axis->currentIndex();
    const qreal intervalMinimum = ui->range->value() / DEFAULT_SIZE;
    const qreal interval = intervalMinimum < ui->resolution->value() ? ui->resolution->value() : intervalMinimum;

    bool needSet = true;
    for (const auto &frame: segment) {
        const qreal pos = frame.GetPose(axisIndex);
        const int index = (pos - m_startPos) / interval;
        if (index >= 0 && index < DEFAULT_SIZE) {
            m_data.replace(index, frame);
            m_location.replace(index, pos);
        } else {
            UTChannel *c = channel();
            disconnect(c, &UTChannel::dataArrived, this, &LocalScan::onDataArrived);
        }

        if (needSet) {
            needSet = false;
            UTDataHeader header = frame.GetHeader();
            const int ampIndex = header.GetAmpIndex(ui->gate->currentData(Qt::UserRole).toInt(), GateMaxPeak);
            const int timeIndex = header.GetTimeIndex(ui->gate->currentData(Qt::UserRole).toInt(), GateMaxPeak);
            m_ampSeries->setFeatureIndex(ampIndex);
            m_timeSeries->setFeatureIndex(timeIndex);
        }
    }

    ThrottleUpdate::call(this, &LocalScan::refreshLines);
}

void LocalScan::resetSeries()
{
    m_data.fill(UTDataFrame(), DEFAULT_SIZE);
    m_location.fill(0, DEFAULT_SIZE);
    m_line->clear();
    refreshLines();
}

void LocalScan::refreshLines()
{
    const UTDataSegment s = m_data.toList();
    m_ampSeries->setData(s, m_location);
    m_timeSeries->setData(s, m_location);
}

void LocalScan::on_scan_clicked()
{
    Axis *axis = scanAxis();
    UTChannel *c = channel();
    UTSetup setup = c->setup();
    if (!axis || !c) {
        return;
    }
    if (!trioNormal()) {
        return;
    }

    m_originalPos = axis->position();

    // TODO: 同步编码器

    const qreal posReverse = m_originalPos - ui->range->value();
    const qreal posForward = m_originalPos + ui->range->value();

    m_startPos = posReverse;
    m_endPos = posForward;
    axis->SetSpeed(ui->speed->value());

    const auto direction = ScanDirection(ui->direction->currentIndex());
    if (direction == Forward && axis->withinLimit(posForward)) {
        m_startPos = m_originalPos;
        setStatus(PrepareScan);
    } else if (direction == Reverse && axis->withinLimit(posReverse)) {
        m_endPos = m_originalPos;
        setStatus(PrepareLocate);
    } else if (direction == Both && axis->withinLimit(posReverse) && axis->withinLimit(posForward)) {
        setStatus(PrepareLocate);
    }

    if (!qIsNaN(m_originalPos)) {
        ui->origin->setText(QString::number(m_originalPos));
    }

    QChart *chart = ui->feature->chart();
    chart->axisX()->setRange(m_startPos, m_endPos);

    const qreal rangeMax = setup.range();
    m_axisTime->setRange(0, rangeMax);
    m_axisTimeDisplay->setRange(0, setup.range());
}

void LocalScan::on_stop_clicked()
{
    Axis *axis = scanAxis();
    if (!axis) {
        return;
    }
    if (!trioNormal()) {
        setUiEnable(true);
        m_status = Idle;
    }
    axis->cancelMove();
    setStatus(Idle);
}

void LocalScan::on_reset_clicked()
{
    if (!trioNormal()) {
        return;
    }
    setStatus(PrepareRestore);
}

void LocalScan::on_moveTo_clicked()
{
    if (!trioNormal()) {
        return;
    }
    Axis *axis = scanAxis();
    if (!axis) {
        return;
    }
    const qreal target = cursorX();
    if (qIsNaN(target)) {
        return;
    }
    if (axis->withinLimit(target)) {
        setStatus(PrepareMoveTo);
    }
}

void LocalScan::on_channel_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    UTChannel *c = channel();
    if (c != nullptr) {
        ui->gate->clear();
        for (const auto &gate: c->setup().gates()) {
            ui->gate->addItem(gate.name(), gate.index());
        }
    }
}

bool LocalScan::eventFilter(QObject *watched, QEvent *event)
{
    QChart *chart = ui->feature->chart();
    if (qobject_cast<QChart *>(watched) == chart && event->type() == QEvent::GraphicsSceneMouseDoubleClick) {
        auto *e = static_cast<QGraphicsSceneMouseEvent *>(event);
        const QPointF pos = chart->mapToValue(e->pos(), m_ampSeries);
        const QPointF top(pos.x(), 100);
        const QPointF bottom(pos.x(), 0);
        m_line->clear();
        m_line->insert(0, top);
        m_line->insert(1, bottom);
    }
    return QWidget::eventFilter(watched, event);
}

bool LocalScan::trioNormal() const
{
    using namespace Motion;
    TrioDriver *driver = MotionManager::driver();
    if (driver->status().state != Motion::Normal) {
        return false;
    }
    return true;
}

bool LocalScan::isEqualWith(qreal value, qreal target)
{
    if (qAbs(value - target) > m_axisPresition) {
        return false;
    }
    return true;
}

/*!
 * \brief LocalScan::moveAxis
 * \param axis:不做空指针检查，使用时要注意先检查
 * \param pos
 */
void LocalScan::moveAxis(Axis *axis, qreal pos)
{
    axis->MoveAbs(pos);
    m_timeStamp = QTime::currentTime();
}

void LocalScan::setUiEnable(bool enable)
{
    ui->options->setEnabled(enable);
    ui->scan->setEnabled(enable);
    ui->reset->setEnabled(enable);
    ui->moveTo->setEnabled(enable);
}

void LocalScan::setStatus(LocalScan::ScanStatus status)
{
    if (m_status == status) {
        return;
    }

    Axis *axis = scanAxis();
    UTChannel *c = channel();
    if (!axis || !c) {
        return;
    }

    if (axis->status().forwardLimited()) {
        status = Idle;
        axis->cancelMove();
        errorWarning(tr("%1轴触发正向限位"));
    }
    if (axis->status().reverseLimited()) {
        status = Idle;
        axis->cancelMove();
        errorWarning(tr("%1轴触发反向限位"));
    }

    ScanStatus statusOld = m_status;
    m_status = status;

    if (statusOld == Idle && status == PrepareLocate) {
        setUiEnable(false);
        moveAxis(axis, m_startPos);
        connect(axis, &Axis::statusChanged, this, &LocalScan::onAxisStatusChanged);
    } else if (statusOld == Idle && status == PrepareScan) {
        setUiEnable(false);
        resetSeries();
        moveAxis(axis, m_endPos);
        connect(axis, &Axis::statusChanged, this, &LocalScan::onAxisStatusChanged);
        connect(c, &UTChannel::dataArrived, this, &LocalScan::onDataArrived);
    } else if (statusOld == Idle && status == PrepareRestore) {
        if (qIsNaN(m_originalPos) || isEqualWith(axis->position(), m_originalPos)) {
            m_status = statusOld;
            return;
        }
        setUiEnable(false);
        moveAxis(axis, m_originalPos);
        connect(axis, &Axis::statusChanged, this, &LocalScan::onAxisStatusChanged);
    } else if (statusOld == Idle && status == PrepareMoveTo) {
        const qreal target = cursorX();
        if (isEqualWith(axis->position(), target)) {
            m_status = statusOld;
            return;
        }
        setUiEnable(false);
        moveAxis(axis, target);
        connect(axis, &Axis::statusChanged, this, &LocalScan::onAxisStatusChanged);
    } else if (statusOld == PrepareLocate && status == Locate) {
        ;
    } else if (statusOld == Locate && status == Idle) {
        disconnect(axis, &Axis::statusChanged, this, &LocalScan::onAxisStatusChanged);
        setUiEnable(true);
    } else if (statusOld == Locate && status == PrepareScan) {
        resetSeries();
        moveAxis(axis, m_endPos);
        connect(c, &UTChannel::dataArrived, this, &LocalScan::onDataArrived);
    } else if (statusOld == PrepareScan && status == Scan) {
        ;
    } else if (statusOld == Scan && status == PrepareRestore) {
        moveAxis(axis, m_originalPos);
        disconnect(c, &UTChannel::dataArrived, this, &LocalScan::onDataArrived);
    } else if (statusOld == Scan && status == Idle) {
        disconnect(c, &UTChannel::dataArrived, this, &LocalScan::onDataArrived);
        disconnect(axis, &Axis::statusChanged, this, &LocalScan::onAxisStatusChanged);
        setUiEnable(true);
    } else if (statusOld == PrepareRestore && status == Restore) {
        ;
    } else if (statusOld == Restore && status == Idle) {
        disconnect(axis, &Axis::statusChanged, this, &LocalScan::onAxisStatusChanged);
        setUiEnable(true);
    } else if (statusOld == PrepareMoveTo && status == MoveTo) {
        ;
    } else if (statusOld == MoveTo && status == Idle) {
        disconnect(axis, &Axis::statusChanged, this, &LocalScan::onAxisStatusChanged);
        setUiEnable(true);
    }

    qCDebug(debug) << "Scan status" << statusOld << "to" << status;
}

void LocalScan::errorWarning(const QString &content)
{
    QMessageBox::warning(this, QString(), content, QMessageBox::Yes);
}

qreal LocalScan::cursorX() const
{
    const auto points = m_line->points();
    if (!points.size()) {
        return Q_SNAN;
    }
    return points.first().x();
}
