#include "PanelProbeBScan.h"
#include "ui_PanelProbeBScan.h"
#include <allrising/ThrottleUpdate.h>
#include <arsCore/Platform.h>
#include <QChart>
#include <QInputDialog>
#include <QLineSeries>
#include <QLoggingCategory>
#include <QtMath>
#include <Ultrasonic/UTSetup.h>
#include <Wheel/ControlPanel.h>
#include <Wheel/DialogCheating.h>
#include <WheelDataset/WheelDataset.h>

using namespace QtCharts;
static const int defaultIndex = ControlPanel::GATE_A_AMP;

static Q_LOGGING_CATEGORY(debug, "wheel.probe");

PanelProbeBScan::PanelProbeBScan(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PanelProbeBScan)
{
    ui->setupUi(this);

    for (auto value: EnumValueList<ControlPanel::Features>()) {
        ui->feature->addItem(EnumToKey(value), value);
    }
    ui->feature->setCurrentIndex(defaultIndex);

    QChart *chart = ui->chart->chart();
    chart->legend()->setAlignment(Qt::AlignRight);

    extremeSeries = new QScatterSeries(chart);
    extremeSeries->setColor(Qt::red);
    chart->addSeries(extremeSeries);
    chart->createDefaultAxes();

    ui->chart->setupSeries();
    ui->chart->installEventFilter(this);
    chart->axisY()->setMax(100);
    chart->axisY()->setLabelsAngle(-90);

    connect(ui->plotAll, &QCheckBox::toggled, this, &PanelProbeBScan::refresh);
    connect(ui->chart, &QChartView::rubberBandChanged, [chart](QRect rect, QPointF from, QPointF to) {
        static QRectF rubberBand;
        if (!rect.isNull()) {
            rubberBand = QRectF(chart->mapFromScene(from), chart->mapFromScene(to)).normalized();
        } else {
            chart->zoomIn(rubberBand);
            chart->axisY()->setRange(0, 100);
        }
    });

    connect(ui->adaptRange, &QCheckBox::toggled, [this](){
        ThrottleUpdate::call(this, &PanelProbeBScan::refreshRange);
    });

    setContextMenuPolicy(Qt::ActionsContextMenu);
}

PanelProbeBScan::~PanelProbeBScan()
{
    delete ui;
}

void PanelProbeBScan::setWheelDataset(WheelDataset *dataset)
{
    if (wheelDataset == dataset) {
        return;
    }

    wheelDataset = dataset;
}

void PanelProbeBScan::showTooltip(QPointF point, bool state)
{
    if (state) {
        BScanSeries *series = qobject_cast<BScanSeries *>(sender());
        const auto frame = series->frameAt(point);

        ui->chart->setToolTip(controlPanel()->frameText(frame));
        ui->chart->cursor->setPos(ui->chart->chart()->mapToPosition(series->pointAt(point), series));
    } else {
        ui->chart->setToolTip(QString());
    }

    ui->chart->cursor->setVisible(state);
}

void PanelProbeBScan::askModifyData(const QPointF &pos)
{
    const qreal peak = pos.y();
    const qreal center = pos.x();
    const qreal span = QInputDialog::getDouble(this, QString(), "Span:", 5);
    const qreal base = QInputDialog::getDouble(this, QString(), "Base:", 12);
    const qreal noise = QInputDialog::getDouble(this, QString(), "Noise:", 5);
    const qreal scale = peak - base;
    qCDebug(debug) << "Asking for data with peak" << peak << "scale" << scale << "span" << span << "center" << center;

    const int channel = controlPanel()->currentChannel();
    const int wheel = controlPanel()->currentWheel();
    const qint64 channelId = ControlPanel::makeChannelId(wheel, channel);

    UTSharedDataSegment segment = wheelDataset->channelFrames(channelId);
    for (UTSharedDataFrame &frame: segment) {
        qreal time = ControlPanel::timestamp(frame);
        if (qAbs(time - center) > span) {
            continue;
        }
        qreal x = (time - center) / span * 2;
        const qreal currentNoise = qreal(qrand() - (RAND_MAX / 2)) / (RAND_MAX / 2) * noise;
        const qreal result = qBound<qreal>(0, scale * qExp(-0.5 * x * x) + currentNoise + base, 100);
        frame->CData[ui->feature->currentIndex()] = result;
        qCDebug(debug) << "Data at" << time << "x" << x << "normal" << qExp(-0.5 * x * x) << "result" << result;
    }
    BScanSeries *series = lineSeries.first();
    series->refresh();
    wheelDataset->modifyData(channelId);
}

bool PanelProbeBScan::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->chart && event->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent *e = static_cast<QMouseEvent *>(event);
        if (e->button() == Qt::MiddleButton && platform()->Get(K_CHEAT_EDIT).toBool()) {
            auto scenePos = ui->chart->mapToScene(e->pos());
            if (!lineSeries.isEmpty()) {
                askModifyData(ui->chart->chart()->mapToValue(scenePos, lineSeries.first()));
            }
        }
    }
    return false;
}

void PanelProbeBScan::refresh()
{
    if (!ControlPanel::datasetValid(wheelDataset)) {
        return;
    }

    int channel = ControlPanel::getInstance()->currentChannel();
    int wheelCount = wheelDataset->wheelSetCount() * 2;
    if (!controlPanel()->isValidChannel(channel)) {
        return;
    }

    QChart *chart = ui->chart->chart();
    for (BScanSeries *series: lineSeries) {
        chart->removeSeries(series);
        delete series;
    }

    lineSeries.clear();
    locations.clear();

    qreal minX = Q_INFINITY;
    qreal maxX = -Q_INFINITY;
    for (int wheelIndex = 0; wheelIndex < wheelCount; ++wheelIndex) {
        if (!ui->plotAll->isChecked() && wheelIndex != ControlPanel::getInstance()->currentWheel()) {
            continue;
        }

        qint64 channelId = ControlPanel::makeChannelId(wheelIndex, channel);
        UTSharedDataSegment segment = wheelDataset->channelFrames(channelId);
        if (segment.isEmpty()) {
            continue;
        }

        QVector<qreal> times;
        for (const auto &frame: segment) {
            qreal time = ControlPanel::timestamp(frame);
            times << time;

            minX = qMin(minX, time);
            maxX = qMax(maxX, time);
        };

        BScanSeries *series = createSeries(controlPanel()->wheelName(wheelIndex));
        series->setData(segment, times);
        series->setFeatureIndex(ui->feature->currentIndex());

        auto extremeFrame = wheelDataset->extremeFrame(channelId);
        if (extremeFrame) {
            locations << QPointF(ControlPanel::timestamp(extremeFrame), 1e-3);
        }
    }

    chart->createDefaultAxes();
    chart->zoomReset();
    ui->chart->setupSeries();
    chart->axisX()->setRange(minX, maxX);
    extremeSeries->replace(locations);
    setWindowTitle(tr("探头B扫图-%1通道）").arg(channel));

    if (ui->channel->value() != channel) {
        ui->channel->setValue(channel);
    }
}

void PanelProbeBScan::refreshRange()
{
    bool adaptRange = ui->adaptRange->isChecked();
    QChart *chart = ui->chart->chart();
    if (!adaptRange) {
        chart->axisY()->setRange(0, 100);
        replaceScatterSeries(0);

        return;
    }

    qreal min = Q_INFINITY;
    qreal max = -Q_INFINITY;
    for (auto *s: chart->series()) {
        auto *series = qobject_cast<BScanSeries *>(s);
        if (!series) {
            continue;
        }

        auto range = series->range();
        min = qMin(range.first,  min);
        max = qMax(range.second, max);
    }

    chart->axisY()->setRange(min, max + 1e-3);
    replaceScatterSeries(min);
}

void PanelProbeBScan::replaceScatterSeries(qreal y)
{
    for (auto &pt: locations) {
        pt.setY(y);
    }

    extremeSeries->replace(locations);
}

void PanelProbeBScan::on_feature_currentIndexChanged(int index)
{
    for (auto *series: lineSeries) {
        series->setFeatureIndex(index);
    }
}

void PanelProbeBScan::on_channel_valueChanged(int arg1)
{
    if (!ControlPanel::getInstance()->isValidChannel(arg1)) {
        return;
    }

    ControlPanel::getInstance()->setCurrentChannel(arg1);
}

BScanSeries *PanelProbeBScan::createSeries(const QString &name)
{
    QChart *chart = ui->chart->chart();
    BScanSeries *series = new BScanSeries(chart);
    series->setPointsVisible(true);
    series->setName(name);
    series->setFeatureIndex(defaultIndex);
    chart->addSeries(series);

    connect(series, &QLineSeries::hovered, this, &PanelProbeBScan::showTooltip);
    connect(series, &BScanSeries::rangeUpdated, [this](){
        ThrottleUpdate::call(this, &PanelProbeBScan::refreshRange);
    });
    lineSeries << series;

    return series;
}
