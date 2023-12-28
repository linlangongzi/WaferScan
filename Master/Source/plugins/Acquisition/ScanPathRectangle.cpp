#include "ScanPathRectangle.h"
#include "ScanPathRectangleEdit.h"
#include "ScanPathUtilities.h"
#include "ScanPlan.h"
#include <Motion/EndPoint.h>
#include <Motion/MotionManager.h>
#include <QLoggingCategory>
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/Util.h>
#include <array>
#include <utils/algorithm.h>

static const qreal stepSpeedScale = 0.5;
static const qreal scanSpeedScale = 0.9;
static Q_LOGGING_CATEGORY(debug, "acquire.path.rect")
REGISTER_JSON_CONVERTER(ScanPathRectangleOptions)

ScanPathRectangle::ScanPathRectangle(QObject *parent) :
    ScanPathFactory(parent)
{
}

QString ScanPathRectangle::name() const
{
    return "Rectangle";
}

QString ScanPathRectangle::displayName() const
{
    return tr("平板");
}

QIcon ScanPathRectangle::icon() const
{
    return QIcon(tr(":/fatcow/16x16/card_chip_silver.png"));
}

MotionPath ScanPathRectangle::motionPath(const QVariant &options) const
{
    ScanPathRectangleOptions o = options.value<ScanPathRectangleOptions>();

    qCDebug(debug) << "Generating rectangle path";
    MotionPath path;
    path.interval = o.scanInterval;
    path.channelOffset = o.probeInterval;

    auto mainStepAxis = Qt::Axis(o.stepAxis());
    const qreal mainStep = (o.end - o.start)[mainStepAxis];
    if (mainStep < 0 && o.primaryChannelIdList().count() > 1) {
        qSwap(o.start, o.end);
    }
    path.elements = o.motionElements();
    return path;
}

QHash<int, UTSetup> ScanPathRectangle::setups(const QVariant &options) const
{
    const ScanPathRectangleOptions o = options.value<ScanPathRectangleOptions>();
    if (!o.setupEnable) {
        return QHash<int, UTSetup>();
    }
    auto setup = o.setup;
    setup.setGain(o.gainOffset);
    auto alterSetup = setup;
    alterSetup.setAlterSource(true);

    QHash<int, UTSetup> setups;
    for (int channelId: o.primaryChannelIdList()) {
        setups.insert(channelId, setup);
    }
    if (o.alterSourceEnable) {
        for (int channelId: o.secondaryChannelIdList()) {
            setups.insert(channelId, setup);
        }
    }
    if (!setups.contains(0)) {
        UTSetup setup = UltrasonicManager::channelSetup(0);
        setup.setPrf(o.setup.prf());
        setups[0] = setup;
    }
    return setups;
}

QList<ImageOptions> ScanPathRectangle::imageOptions(const QVariant &options) const
{
    const ScanPathRectangleOptions opt = options.value<ScanPathRectangleOptions>();
    const QList<int> &list = opt.elementAxisList().toList();
    const int scanAxisIndex = list.indexOf(opt.axisIndex(opt.scanAxisId));
    const int stepAxisIndex = opt.stepAxis();

    QSizeF gridSize = QSizeF(opt.scanInterval, opt.stepInterval);
    QRectF sourceRect = QRectF(QPointF(opt.start[scanAxisIndex], opt.start[stepAxisIndex]),
                               QPointF(opt.end[scanAxisIndex], opt.end[stepAxisIndex]));
    gridSize = QSizeF(gridSize.width(), gridSize.height());
    qreal minMargin = qMin(gridSize.width(), gridSize.height());
    sourceRect = sourceRect.normalized();
    sourceRect = sourceRect.marginsAdded(QMarginsF(10 * minMargin, 10 * minMargin, 10 * minMargin, 10 * minMargin));

    ImageOptions mainChannelImageOptions(sourceRect, gridSize);
    mainChannelImageOptions.setCoordinateType(ImageOptions::CoordNormal);
    mainChannelImageOptions.setCyclularCenter(0);
    mainChannelImageOptions.setAxisX(MotionManager::axisIndex(opt.scanAxisId));
    mainChannelImageOptions.setAxisY(opt.stepAxis());

    Axis *scanAxis = MotionManager::axis(opt.scanAxisId);
    mainChannelImageOptions.setBacklash(QPointF(scanAxis->config().backlash, 0));
    mainChannelImageOptions.setChannels(MotionAxisList(opt.primaryChannelIdList()));

    // 获取通道偏移量
    QVector<QVector<float>> channelOffsets;
    auto offset = channelOffset(opt);
    for (int key: offset.keys()) {
        if (key >= channelOffsets.size()) {
            channelOffsets.resize(key + 1);
        }
        channelOffsets.replace(key, offset.value(key));
    }
    mainChannelImageOptions.setChannelOffset(channelOffsets);

    if (opt.unidirection) {
        mainChannelImageOptions.setDirectionFilter(ImageOptions::DirectionForward);
    }
    if (opt.alterSourceEnable) {
        ImageOptions subChannelImageOptions = mainChannelImageOptions;
        subChannelImageOptions.setChannels(MotionAxisList(opt.secondaryChannelIdList()));
        subChannelImageOptions.setChannelOffset(channelOffsets);
        return QList<ImageOptions> ({mainChannelImageOptions, subChannelImageOptions});
    }
    return QList<ImageOptions> ({mainChannelImageOptions});
}

QPair<qreal, qreal> ScanPathRectangle::boundingRange(const QVariant &options, int axis) const
{
    const ScanPathRectangleOptions o = options.value<ScanPathRectangleOptions>();
    Axis *axisPointer = MotionManager::axis(axis);
    if (axisPointer == nullptr) {
        return qMakePair(Q_SNAN, Q_SNAN);
    }
    //! FIXME   下列使用名字存在弊端，需要替代
    if (axisPointer->name() == "X") {
        return qMakePair(o.start.x(), o.end.x());
    }
    if (axisPointer->name() == "Y") {
        return qMakePair(o.start.y(), o.end.y());
    }
    if (axisPointer->name() == "Z") {
        return qMakePair(o.start.z(), o.end.z());
    }
    return qMakePair(Q_SNAN, Q_SNAN);
}

ScanPathFactory::CheckResult ScanPathRectangle::checkOptions(const QVariant &options) const
{
    CheckResult result;
    auto o = options.value<ScanPathRectangleOptions>();
    if (qFuzzyIsNull(o.effectiveLinearSpeed())) {
        result.addError(tr("扫描线速度无效"));
    }
    if (qFuzzyIsNull(o.stepInterval)) {
        result.addError(tr("无效的步进间隔"));
    }
    if (o.axisIndex(o.scanAxisId) < 0) {
        result.addError(tr("无效的扫描轴"));
    }
    if (o.stepAxis() < 0) {
        result.addError(tr("无效的步进轴"));
    }
    if (o.stepAxis() == o.axisIndex(o.scanAxisId)) {
        result.addError(tr("步进轴与扫描轴相同"));
    }
    if (o.start == o.end) {
        result.addError(tr("起始点相同"));
    }
    if (qFuzzyCompare(QVector3D(), o.scanDirection())) {
        result.addError(tr("扫描方向没有指定"));
    }
    if (o.effectiveLinearSpeed() > o.calculatedAdvicedSpeed()) {
        result.addWarning(tr("超过推荐速度"));
    }
    const QList<int> &list = o .elementAxisList().toList();
    const int scanAxisIndex = list.indexOf(o.axisIndex(o.scanAxisId));
    const int stepAxisIndex = o.stepAxis();
    if (scanAxisIndex < 0) {
        result.addError(tr("扫描轴不存在"));
        return result;
    }
    if (o.stepInterval > (qAbs(o.end[stepAxisIndex] - o.start[stepAxisIndex]))) {
        result.addWarning(tr("步进间隔过大"));
    }
    if (o.scanInterval > (qAbs(o.end[scanAxisIndex] - o.start[scanAxisIndex]))) {
        result.addWarning(tr("扫描间隔过大"));
    }
    return result;
}

QWidget *ScanPathRectangle::widget(const QVariant &options) const
{
    auto *edit = new ScanPathRectangleEdit;
    edit->setOptions(options.value<ScanPathRectangleOptions>());
    return edit;
}

QVariant ScanPathRectangle::widgetOptions(QWidget *widget) const
{
    auto *edit = qobject_cast<ScanPathRectangleEdit *>(widget);
    if (edit != nullptr) {
        return QVariant::fromValue(edit->options());
    }
    return QVariant();
}

MotionPose ScanPathRectangleOptions::elementMotionPose(const QVector3D &vector) const
{
    EndPoint *endPoint = MotionManager::endPoint(endPointId);
    if (endPoint == nullptr) {
        return MotionPose();
    }
    return endPoint->motionPose(vector);
}

qreal ScanPathRectangleOptions::multiProbePatchSpan() const
{
    if (channelSelect == ChannelSelectPolicy::Parallel) {               // 等间距
        return probeInterval * primaryChannelIdList().count();
    } else {                                                            // 不等间距 或 单探头
        qreal sum = 0;
        for (int index = 1; index < primaryChannelIdList().count(); ++index) {        // 第一个通道的间距是固定0不变的
            sum += probeIntervalList.value(index);
        }
        return (sum + multiProbePatchStep());
    }
}

qreal ScanPathRectangleOptions::multiProbePatchStep() const
{
    if (channelSelect == ChannelSelectPolicy::Parallel) {               // 等间距
        return probeInterval;
    } else {                                                            // 不等间距 或 单探头
        qreal max = 0;
        for (int index = 1; index < primaryChannelIdList().count(); ++index) {        // 第一个通道的间距是固定0不变的
            max = qMax(max, probeIntervalList.value(index));
        }
        return max;
    }
}

QList<MotionElement> ScanPathRectangleOptions::motionElements() const
{
    const QVector3D direction = end - start;                            // 矢量方向
    auto mainStepAxis = Qt::Axis(stepAxis());                           // 步进轴
    const qreal mainStep = direction[mainStepAxis];                     // 步进距离在方向上的投影
    const int stepDirection = arsSign(mainStep / qAbs(mainStep));       // return 1 / -1 步进方向
    QList<MotionElement> elements;
    // patchSpan  : 一个扫描块能够覆盖的步进距离
    // patchStep  : 最大探头间距,机械臂的步进距离。
    // 探头的总步进距离 = 扫描块的步进 + 机械臂的步进
    // patchCount : 扫描需要的完整块数，无论mainStep为正或者负，count永远为正
    // patchLast  : 完整块之后, 最后一个扫描块的尺寸
    const qreal patchSpan = multiProbePatchSpan() * stepDirection;
    const qreal patchStep = multiProbePatchStep();
    if (patchSpan > 0 && patchStep > 0) {                               // 多通道
        const int patchCount = qFloor(mainStep / patchSpan);
        const qreal patchLast = mainStep - patchCount * patchSpan;
        QVector3D unit = ScanPathUtilities::unitVector(mainStepAxis);
        if (patchCount == 0) {
            QVector3D endOffset = (qAbs(patchLast) > patchStep ? patchStep : patchLast) * unit;
            addPatch(elements, {}, endOffset);
        } else {
            for (int patchIndex = 0; patchIndex < patchCount; ++patchIndex) {
                QVector3D startOffset = patchIndex * patchSpan * unit;
                QVector3D endOffset = (patchIndex * patchSpan + patchStep * stepDirection) * unit;
                addPatch(elements, startOffset, endOffset);
            }
            if (qFuzzyIsNull(patchLast)) {

            } else if (qAbs(patchLast) <= patchStep) {
                elements.last().addOffset(patchLast, stepAxis());
            } else if (qAbs(patchLast) > patchStep) {
                QVector3D startOffset = ((patchCount - 1) * patchSpan + patchLast) * unit;
                QVector3D endOffset = ((patchCount - 1) * patchSpan + patchLast + patchStep * stepDirection) * unit;
                addPatch(elements, startOffset, endOffset);
            }
        }
    } else {                                                            //单通道
        addPatch(elements, {}, stepVector());
    }
    return elements;
}

void ScanPathRectangleOptions::addPatch(QList<MotionElement> &elements, const QVector3D &startOffset, const QVector3D &endOffset) const
{
    MotionElement element;
    element.type = MotionElement::MoveLine;
    element.axis = elementAxisList();
    element.mainAxis = axisIndex(scanAxisId);
    const int stepAxisIndex = elementAxisList().toList().value(stepAxis());
    element.stepAxis = stepAxisIndex;
    element.speed = effectiveLinearSpeed();
    element.stepInterval = stepInterval;
    element.stepSpeed = element.speed * stepSpeedScale;
    element.interPolicy = unidirection ? MotionElement::InterpolateUnidirectional : MotionElement::InterpolateBidiretional;

    MotionElement startElement = element;
    startElement.pos << elementMotionPose(start + startOffset) << elementMotionPose(topRight() + startOffset);
    elements << startElement;

    MotionElement endElement = element;
    endElement.pos << elementMotionPose(start + endOffset) << elementMotionPose(topRight() + endOffset);
    endElement.interPolicy = MotionElement::InterpolateNone;
    elements << endElement;
}

QHash<int, QVector<float> > ScanPathRectangle::channelOffset(const ScanPathRectangleOptions &o) const
{
    QHash<int, QVector<float> > offsets;                                        /*!< <channel, <axisOffset> > */
    int axisCount = MotionManager::axisCount();
    int channelListCount = o.primaryChannelIdList().count();
    if (o.alterSourceEnable) {
        channelListCount = o.secondaryChannelIdList().count();
    }
    qreal sum = 0;
    for (int index = 0; index < channelListCount; ++index) {
        QVector<float> offset(axisCount, 0);
        if (o.channelSelect == o.ChannelSelectPolicy::ParallelList) {
            sum += o.probeIntervalList.value(index);
            offset.replace(o.stepAxis(), sum);
        } else {
            offset.replace(o.stepAxis(), o.probeInterval * index);
        }
        offsets.insert(o.primaryChannelIdList()[index], offset);
    }
    return offsets;
}

int ScanPathRectangleOptions::axisIndex(const QUuid &axisId) const
{
    EndPoint *endPoint = MotionManager::endPoint(endPointId);
    if (endPoint == nullptr) {
        return -1;
    }
    Axis *axis = endPoint->axis(axisId);
    if (axis == nullptr) {
        return -1;
    }
    return axis->index();
}

QList<int> ScanPathRectangleOptions::primaryChannelIdList() const
{
    if (channelSelect == Single) {
        return QList<int>({channel});
    }
    return channels;
}

QList<int> ScanPathRectangleOptions::secondaryChannelIdList() const
{
    return Utils::transform(primaryChannelIdList(), [] (int channelId) {
        UTChannel *channel = UltrasonicManager::channel(channelId);
        if (channel == nullptr) {
            return -1;
        }
        UTInstrument *instrument = channel->instrument();
        if (instrument == nullptr) {
            return -1;
        };
        UTChannel *secondaryChannel = instrument->channel(1);
        if (secondaryChannel == nullptr) {
            return -1;
        }
        return secondaryChannel->channelId();
    });
}

qreal ScanPathRectangleOptions::calculatedAdvicedSpeed() const
{
    int prf = 0;
    int sampleAverage = 0;
    if (setupEnable) {
        prf = setup.prf();
        sampleAverage = setup.sampleAverage();
    } else {
        const QList<UTChannel *> channels = UltrasonicManager::channels();
        if (channels.isEmpty()) {
            return 0;
        }
        UTSetup channelSetup = channels.first()->setup();
        prf = channelSetup.prf();
        sampleAverage = channelSetup.sampleAverage();

        if (channelSelect == Single && primaryChannelIdList().first() != 0) {   //采样平均次数与当前通道有关
            UTSetup channelSetupSecond = channels.value(primaryChannelIdList().first())->setup();
            sampleAverage = channelSetupSecond.sampleAverage();
        }
    }
    const qreal speed = scanInterval * prf / qPow(2, sampleAverage) * scanSpeedScale;
    Axis *axis = MotionManager::axis(scanAxisId);
    if (axis == nullptr) {
        return 0;
    }
    return qBound(0.0, speed, axis->config().speedMax);
}

qreal ScanPathRectangleOptions::effectiveLinearSpeed() const
{
    if (userLinearSpeedEnable) {
        return linearSpeed;
    } else {
        return calculatedAdvicedSpeed();
    }
}

MotionAxisList ScanPathRectangleOptions::elementAxisList() const
{
    EndPoint *endPoint = MotionManager::endPoint(endPointId);
    if (endPoint == nullptr) {
        return {};
    }
    MotionAxisList list;
    foreach (Axis *axis, endPoint->axisList().mid(0, 3)) {
        list.setAxis(axis->index(), true);
    }
    return list;
}

int ScanPathRectangleOptions::stepAxis() const
{
    std::array<float, 3> projections;
    const QVector3D stepV = stepVector();
    for (int index = 0; index < projections.size(); ++index) {
        QVector3D baseDirection;
        baseDirection[index] = 1;
        projections[index] = qAbs(QVector3D::dotProduct(stepV, baseDirection));
    }
    const auto max = std::max_element(projections.begin(), projections.end());
    return std::distance(projections.begin(), max);
}

QVector3D ScanPathRectangleOptions::topRight() const
{
    const QVector3D direction = scanDirection();
    return direction.normalized() * (QVector3D::dotProduct((end - start), direction) / direction.length()) + start;
}

QVector3D ScanPathRectangleOptions::stepVector() const
{
    return end - topRight();
}

QVector3D ScanPathRectangleOptions::scanDirection() const
{
    EndPoint *endPoint = MotionManager::endPoint(endPointId);
    if (endPoint == nullptr) {
        return {};
    }
    Axis *axis = endPoint->axis(scanAxisId);
    if (axis == nullptr) {
        return {};
    }
    return axis->config().baseDirection;
}
