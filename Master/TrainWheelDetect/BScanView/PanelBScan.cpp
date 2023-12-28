#include "PanelBScan.h"
#include "ui_PanelBScan.h"
#include <QLineSeries>
#include <allrising/Util.h>
#include <utils/algorithm.h>
#include <Wheel/ControlPanel.h>
#include <allrising/ThrottleUpdate.h>
#include <WheelDataset/WheelDataset.h>

using namespace QtCharts;

PanelBScan::PanelBScan(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PanelBScan)
{
    ui->setupUi(this);

    for (auto value: EnumValueList<ControlPanel::Features>()) {
        ui->feature->addItem(EnumToKey(value), value);
    }
    ui->feature->setCurrentIndex(ui->feature->findData(ControlPanel::GATE_A_AMP));

    connect(controlPanel(), &ControlPanel::globalConfigChanged, this, &PanelBScan::setupSeries);
    connect(ui->adaptRange, &QCheckBox::toggled, this, &PanelBScan::refreshRange);
    connect(ui->timelineEnable, &QCheckBox::toggled, this, &PanelBScan::refreshLines);
    connect(ui->timelinePos, &QSlider::valueChanged, [this]() {
        ThrottleUpdate::call(this, &PanelBScan::refreshLines);
    });

    setupSeries();
}

PanelBScan::~PanelBScan()
{
    delete ui;
}

void PanelBScan::setupSeries()
{
    /* 清除所有数据序列 */
    for (auto series: seriesList) {
        if (series) {
            series->chart()->removeSeries(series);
        }
    }
    qDeleteAll(seriesList);
    seriesList.clear();

    auto groups = controlPanel()->getGlobalConfig().channelGroups;

    QList<BScanChart *> views = {ui->chart1, ui->chart2, ui->chart3, ui->chart4};
    for (const auto &group: groups) {
        BScanChart *view = views.value(group.view);
        if (!view) {
            seriesList << NULL;
            continue;
        }

        QChart *chart = view->chart();

        auto *series = new BScanSeries(chart);
        series->setName(group.name);
        series->setFeatureIndex(ui->feature->currentData().toInt());
        chart->addSeries(series);
        chart->createDefaultAxes();
        seriesList << series;

        connect(series, &BScanSeries::hovered, this, &PanelBScan::onSeriesHovered);
        connect(series, &BScanSeries::doubleClicked, this, &PanelBScan::onSeriesDoubleClicked);
        connect(series, &BScanSeries::rangeUpdated, [this]() {
            ThrottleUpdate::call(this, &PanelBScan::refreshRange);
        });

        view->setupSeries();
        chart->axisY()->setRange(0, 100);
    }

    ThrottleUpdate::call(this, &PanelBScan::refreshLines);
}

void PanelBScan::refreshLines()
{
    if (!ControlPanel::datasetValid(wheelDataset)) {
        return;
    }

    UTSharedDataSegment segment;
    ui->timelineContainer->setVisible(ui->timelineEnable->isChecked());

    if (ui->timelineEnable->isChecked()) {
        int minTime = 0;
        int maxTime = 0;
        int pos = ui->timelinePos->value();
        const auto allData = wheelDataset->allData();
        for (const auto &bin: allData) {
            for (const auto &frame: bin.data) {
                int time = ControlPanel::timestamp(frame);
                minTime = qMin(minTime, time);
                maxTime = qMax(maxTime, time);
                if ((time >= pos) && (time - pos <= ui->timelineRange->value())) {
                    segment << frame;
                }
            }
        }
        ui->timelinePos->setRange(minTime, maxTime);
        ui->timelineText->setText(tr("%1-%2").arg(pos).arg(pos + ui->timelineRange->value()));
    } else {
        int currentWheel = ControlPanel::getInstance()->currentWheel();
        setWindowTitle(tr("B扫描 - %1").arg(controlPanel()->wheelName(currentWheel)));
        segment = wheelDataset->wheelFrames(currentWheel);
    }

    auto groups = controlPanel()->getGlobalConfig().channelGroups;

    QList<UTSharedDataSegment> data = Utils::transform(groups, [](const ChannelGroup &group) {
        int size = group.end - group.start + 1;
        return UTSharedDataSegment::fromVector(QVector<UTSharedDataFrame>(size));
    });

    for (const auto &frame: segment) {
        if (frame.isNull()) {
            continue;
        }

        int channel = (ControlPanel::channelId(frame) % 10000) + 10000;
        for (int index = 0; index < groups.size(); ++index) {
            const auto &group = groups.at(index);
            if (channel >= group.start && channel <= group.end) {
                data[index][channel - group.start] = frame;
            }
        }
    }

    /* 将数值表变换为LineSeries */
    for (int seriesIndex = 0; seriesIndex < groups.size(); ++seriesIndex) {
        auto *series = seriesList[seriesIndex];
        if (series) {
            series->setData(data[seriesIndex]);
            series->chart()->zoomReset();
            series->chart()->axisX(series)->setRange(0, data[seriesIndex].size() - 1);
        }
    }
}

QImage PanelBScan::renderBScan()
{
    refreshRange();
    refreshLines();

    return ui->splitterAll->grab().toImage();
}

void PanelBScan::setWheelDataset(WheelDataset *dataset)
{
    if (wheelDataset == dataset) {
        return;
    }

    wheelDataset = dataset;
    disconnect(0, 0, this, SLOT(onChannelDataUpdated));

    if (wheelDataset) {
        connect(wheelDataset, &WheelDataset::channelDataUpdated, this, &PanelBScan::onChannelDataUpdated);
    }
}

void PanelBScan::refreshRange()
{
    for (auto *view: findChildren<BScanChart *>()) {
        QChart *chart = view->chart();
        if (!ui->adaptRange->isChecked()) {
            chart->axisY()->setRange(0, 100);
            continue;
        }
        qreal min = 0;
        qreal max = 0;
        for (auto *s: chart->series()) {
            auto *series = qobject_cast<BScanSeries *>(s);
            if (!series) {
                continue;
            }
            auto range = series->range();
            min = qMin(range.first, min);
            max = qMax(range.second, max);
        }
        chart->axisY()->setRange(min, max + 1e-3);
    }
}

void PanelBScan::setChartsOptions(QtCharts::QChart::AnimationOptions options)
{
    for (auto chart: findChildren<BScanChart *>()) {
        chart->chart()->setAnimationOptions(options);
    }
}

void PanelBScan::onSeriesHovered(QPointF point, bool state)
{
    BScanSeries *series = qobject_cast<BScanSeries *>(sender());
    if (!series) {
        return;
    }
    BScanChart *view = qobject_cast<BScanChart *>(series->chart()->parent());
    if (!view) {
        qDebug() << "Invalid parent:" << series->chart()->parent();
        return;
    }
    if (state) {
        const auto frame = series->frameAt(point);

        view->setToolTip(controlPanel()->frameText(frame));
        view->cursor->setPos(series->chart()->mapToPosition(series->pointAt(point), series));
    } else {
        view->setToolTip(QString());
    }

    view->cursor->setVisible(state);
}

void PanelBScan::onSeriesDoubleClicked()
{
    BScanSeries *series = qobject_cast<BScanSeries *>(sender());
    if (!series) {
        return;
    }
    BScanChart *chart = qobject_cast<BScanChart *>(series->chart()->parent());
    UTSharedDataFrame frame = series->frameAt(series->chart()->mapToValue(chart->cursor->pos(), series));
    if (frame) {
        ControlPanel::getInstance()->setCurrentWheel(ControlPanel::wheelId(frame));
        ControlPanel::getInstance()->setCurrentChannel(ControlPanel::channelId(frame));
    }
}

void PanelBScan::onChannelDataUpdated(const QSet<qint64> &channels)
{
    int currentWheel = ControlPanel::getInstance()->currentWheel();
    for (auto id: channels) {
        if (ControlPanel::channelIdToWheel(id) == currentWheel) {
            ThrottleUpdate::call(this, &PanelBScan::refreshLines);
            return;
        }
    }
}

void PanelBScan::on_feature_activated(int index)
{
    for (auto *series: seriesList) {
        series->setFeatureIndex(ui->feature->itemData(index).toInt());
    }
}
