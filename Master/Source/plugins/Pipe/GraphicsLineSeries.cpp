#include "GraphicsLineSeries.h"
#include "PipePanelBScan.h"
#include <QBoxLayout>
#include <QGraphicsScene>
#include <QPen>
#include <QtMath>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/ThrottleUpdate.h>

class MovingAverage
{
public:
    inline void add(qreal value)
    {
        if (qIsFinite(value)) {
            sum += value;
            count++;
        }
        window.append(value);
    }

    inline qreal next(qreal value)
    {
        if (window.size() == 1) {
            sum = value;
            qSwap(value, window.first());
            return value;
        }
        add(value);
        const qreal average = sum / count;

        const qreal dequeue = window.takeFirst();
        if (qIsFinite(dequeue)) {
            sum -= dequeue;
            count--;
        }
        return average;
    }

private:
    QList<qreal> window;
    qreal sum = 0;
    int count = 0;
};

GraphicsLineSeries::GraphicsLineSeries(QWidget *parent) :
    QWidget(parent),
    path(new QGraphicsPathItem),
    title(new QCheckBox(this)),
    legend(new QLabel(this))
{
    title->setChecked(true);
    connect(title, &QCheckBox::toggled, this, [=](bool checked) {
        path->setVisible(checked);
    });

    auto *layout = new QHBoxLayout(this);
    layout->addWidget(title);
    layout->addWidget(legend);

    legend->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    legend->setMinimumWidth(20);
}

GraphicsLineSeries::~GraphicsLineSeries()
{
    if (!path->parentItem()) {
        delete path;
    }
}

void GraphicsLineSeries::setItemParents(QGraphicsItem *parentItem)
{
    path->setParentItem(parentItem);
}

void GraphicsLineSeries::setOption(const PlotCurve &option)
{
    QPen pen(option.color, option.lineWidth);
    pen.setCosmetic(true);
    path->setPen(pen);
    path->setTransform(QTransform::fromScale(option.interval, 1));

    title->setText(option.name);

    legend->setFixedHeight(qMax(2, option.lineWidth));
    legend->setStyleSheet("background-color: " + option.color.name());

    curve = option;
}

bool GraphicsLineSeries::add(qreal pos, int position, qreal value, int channelId, bool noAlarm)
{
    /* 边界检查 */
    if (uint(position) > uint(data.size())) {
        return false;
    }

    if (!qIsFinite(value)) {
        return false;
    }

    /* 整体幅值修正处理 */
    value += curve.correct;

    /* 不触发：0无效；触发：基底噪声最低也不会趋近于0 */
    if (qIsNaN(data[position])) {
        data[position] = value;
    } else {
        switch (curve.reduce) {
        case PlotCurve::ReduceNone:
            data[position] = value;
            count[position] = 1;
            break;
        case PlotCurve::ReduceByMax:
            data[position] = qMax(value, data[position]);
            count[position] = 1;
            break;
        case PlotCurve::ReduceByMin:
            data[position] = qMin(data[position], value);
            count[position] = 1;
            break;
        case PlotCurve::ReduceByAverage:
            data[position] += value;
            count[position]++;
            break;
        }
    }

    /* 盲区外的报警、极值处理 */
    bool alarmChanged = false;
    if (!noAlarm) {                                     /* 盲区过滤 */
        for (auto &alarm: alarms) {
            if (alarm.condition.shouldAlarm(value)) {
                emit FoundFeature(pos);

                if (alarm.allTriggered) {
                    continue;
                }

                if (!alarm.alarm.channel.hasAxis(channelId)) {
                    alarm.alarm.channel.setAxis(channelId);
                    alarm.alarm.reason = tr("B扫描曲线 %1 超出警报线 %2 的限制 (%3)").arg(curve.name, alarm.condition.name).arg(alarm.condition.value);
                    alarm.allTriggered = (alarm.alarm.channel.toList() == curve.channels.toList());
                    alarmChanged = true;
                }
            }
        }

        calculateExtreme(channelId, value, pos);
    }

    ThrottleUpdate::call(this, &GraphicsLineSeries::refreshPath);

    return alarmChanged;
}

void GraphicsLineSeries::setSize(int size)
{
    QVector<qreal> vector;
    vector.clear();

    data.fill(Q_SNAN, size);
    count.fill(0, size);
    path->setPath(QPainterPath());

    maxima = PlotFeature();
    minima = PlotFeature();
}

void GraphicsLineSeries::setAlarms(const QList<PlotLine> &lines)
{
    alarms.clear();
    for (const auto &line: lines) {
        AlarmItem item;
        item.condition = line;
        alarms << item;
    }
}

void GraphicsLineSeries::reset()
{
    setSize(-1);
}

void GraphicsLineSeries::refreshPath()
{
    if (data.isEmpty() || (data.size() != count.size())) {
        return;
    }
    QPainterPath p;

    bool prevDrawn = false;
    MovingAverage window;
    for (int index = 0; index < curve.average; ++index) {                       /* 将前n个数字加入移动平均池 */
        if (!qIsNaN(data.at(index))) {
            const qreal value = data.at(index) / count.at(index);
            window.add(value);
        }
    }

    int indexOffset = curve.average / 2;
    for (int index = curve.average - 1; index < data.count(); ++index) {
        if (!qIsNaN(data.at(index))) {
            const qreal nextValue = data.at(index) / count.at(index);
            qreal v = window.next(nextValue);                                       /* 将当前数值推入移动平均池, 并获取平均值 */
            bool isValid = qIsFinite(v);

            if (isValid) {
                if (prevDrawn) {
                    p.lineTo(index - indexOffset, v);
                } else {
                    p.moveTo(index - indexOffset, v);
                }
            }

            prevDrawn = isValid;
        } else {
            prevDrawn = false;
        }
    }

    path->setPath(p);
    refreshTooltips();
}

void GraphicsLineSeries::calculateExtreme(int channelId, qreal value, int pos)
{
    if (qIsNaN(maxima.feature) || value > maxima.feature) {
        maxima.pos = pos;
        maxima.feature = value;
        maxima.channel = channelId;
    }

    if (qIsNaN(minima.feature) || value < minima.feature) {
        minima.pos = pos;
        minima.feature = value;
        minima.channel = channelId;
    }
}

void GraphicsLineSeries::refreshTooltips()
{
    QString unitH = "";
    QString unitV = "";
    auto *bscan = qobject_cast<PipePanelBScan *>(parent());
    if (bscan) {
        unitH = bscan->viewOptions().unitNameH;
        unitV = bscan->viewOptions().unitNameV;
    }

    bool isByMax = curve.reduce == PlotCurve::ReduceByMax;
    PlotFeature extremum = isByMax ? maxima : minima;
    QString type = isByMax ? tr("最大值") : tr("最小值");

    QString tooltip = tr("%1: %2 %3\n" "位置: %4 %5")
            .arg(type).arg(extremum.feature).arg(unitV)
            .arg(extremum.pos).arg(unitH);
    if (extremum.channel >= 0) {
        tooltip += tr("\n通道: %1").arg(UltrasonicManager::channelName(extremum.channel));
    }
    legend->setToolTip(tooltip);
    title->setToolTip(tooltip);
}
