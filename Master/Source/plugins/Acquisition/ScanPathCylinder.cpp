#include "ScanPathCylinder.h"
#include "ScanPathCylinderEdit.h"
#include "ScanPathUtilities.h"
#include <Motion/MotionManager.h>
#include <QLoggingCategory>
#include <QtMath>
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/Util.h>
#include <utils/algorithm.h>

static Q_LOGGING_CATEGORY(debug, "acquire.path.cylinder")

static const qreal stepSpeedScale = 0.5;
static const qreal scanSpeedScale = 0.9;

REGISTER_JSON_CONVERTER(ScanPathCylinderOptions)

ScanPathCylinder::ScanPathCylinder(QObject *parent) :
    ScanPathFactory (parent)
{
}

QString ScanPathCylinder::name() const
{
    return "Cylinder";
}

QString ScanPathCylinder::displayName() const
{
    return tr("管棒");
}

QIcon ScanPathCylinder::icon() const
{
    return QIcon(tr(":/fatcow/16x16/database_green.png"));
}

QHash<int, UTSetup> ScanPathCylinder::setups(const QVariant &options) const
{
    const ScanPathCylinderOptions o = options.value<ScanPathCylinderOptions>();
    if (!o.setupEnable) {
        return {};
    }
    UTSetup setup = o.setup;
    setup.setGainRef(o.gainOffset);
    UTSetup alterSetup = setup;
    alterSetup.setAlterSource(true);

    QHash<int, UTSetup> setups;
    for (int channelId: o.primaryChannelIdList()) {
        setups[channelId] = setup;
    }
    if (o.alterSourceEnable) {
        for (int channelId: o.secondaryChannelIdList()) {
            setups[channelId] = alterSetup;
        }
    }
    if (!setups.contains(0)) {
        UTSetup setup = UltrasonicManager::channelSetup(0);
        setup.setPrf(o.setup.prf());
        setups[0] = setup;
    }
    return setups;
}

MotionAxisList ScanPathCylinderOptions::elementAxisList() const
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

MotionPose ScanPathCylinderOptions::elementMotionPose(const QVector3D &vector) const
{
    EndPoint *endPoint = MotionManager::endPoint(endPointId);
    if (endPoint == nullptr) {
        return MotionPose();
    }
    return endPoint->motionPose(vector);
}

void ScanPathCylinderOptions::addPatch(QList<MotionElement> &elements, const QVector3D &startOffset, const QVector3D &endOffset, const MotionElement &element) const
{
    MotionElement startElement = element;
    startElement.pos << elementMotionPose(posList.first() + startOffset);
    elements << startElement;

    MotionElement endElement = element;
    endElement.pos << elementMotionPose(posList.first() + endOffset);
    endElement.interPolicy = MotionElement::InterpolateNone;
    elements << endElement;
}

QList<MotionElement> ScanPathCylinderOptions::motionElements(const MotionElement &element) const
{
    if (posList.isEmpty()) {
        return {};
    }
    const QVector3D direction = posList.last() - posList.first();
    Qt::Axis mainStepAxis = ScanPathUtilities::maximumAxis(direction);
    const qreal mainStep = qAbs(direction[mainStepAxis]);
    const int stepDirection = arsSign(mainStep / qAbs(mainStep));
    QList<MotionElement> elements;
    if (primaryChannelIdList().count() > 1) {
        // patchSpan : 一个扫描块能够覆盖的步进距离
        // patchCount : 扫描需要的完整块数
        // patchLast : 完整块之后, 最后一个扫描块的尺寸
        const qreal patchSpan = probeInterval * primaryChannelIdList().count();
        const int patchCount = qFloor(mainStep / patchSpan);
        const qreal patchLast = mainStep - patchCount * patchSpan;
        QVector3D unit = ScanPathUtilities::unitVector(mainStepAxis);
        if (patchCount == 0) {
            QVector3D endOffset = (qAbs(patchLast) > probeInterval ? probeInterval : patchLast) * unit;
            addPatch(elements, QVector3D(), endOffset, element);
        } else {
            for (int patchIndex = 0; patchIndex < patchCount; ++patchIndex) {
                QVector3D startOffset = patchIndex * patchSpan * unit;
                QVector3D endOffset = (patchIndex * patchSpan + probeInterval * stepDirection) * unit ;
                addPatch(elements, startOffset, endOffset, element);
            }
            if (qFuzzyIsNull(patchLast)) {

            } else if (qAbs(patchLast) <= probeInterval) {
                elements.last().addOffset(patchLast, mainStepAxis);
            } else if (qAbs(patchLast) > probeInterval) {
                QVector3D startOffset = ((patchCount - 1) * patchSpan + patchLast) * unit;
                QVector3D endOffset = ((patchCount - 1) * patchSpan + patchLast + probeInterval * stepDirection) * unit;
                addPatch(elements, startOffset, endOffset, element);
            }
        }
    } else {
        addPatch(elements, QVector3D(), direction, element);
    }
    return elements;
}

MotionPath ScanPathCylinder::motionPath(const QVariant &options) const
{
    const ScanPathCylinderOptions o = options.value<ScanPathCylinderOptions>();

    MotionPath path;
    path.channelOffset = o.probeInterval;
    path.multiChannelEnable = (o.channelSelect == ScanPathCylinderOptions::Parallel);
    path.multiChannels << o.channels;
    path.rotations << o.pathRotation();
    path.interval = o.pathRotation().nativeSpeed(o.scanInterval);
    path.prerequisites << o.requisityElement();

    MotionElement element;
    element.type = MotionElement::MoveLine;
    element.conditions << o.elementCondition();
    element.mainAxis = MotionManager::axisIndex(o.scanAxis);
    element.stepAxis = o.stepAxis();
    element.axis = o.elementAxisList();
    element.speed = o.effectiveRotatorAngularSpeed();
    RotationInfo stepRotation = RotationInfo::byAxisIndex(path.rotations, element.stepAxis);
    element.stepInterval = stepRotation.nativeSpeed(o.stepInterval);
    element.stepSpeed = stepRotation.nativeSpeed(element.speed * stepSpeedScale);
    element.interPolicy = MotionElement::InterpolateUnidirectional;
    path.elements = o.motionElements(element);
    return path;
}

QList<ImageOptions> ScanPathCylinder::imageOptions(const QVariant &options) const
{
    const ScanPathCylinderOptions o = options.value<ScanPathCylinderOptions>();
    if (o.posList.isEmpty()) {
        return {};
    }
    QSizeF scale = o.unitScale();
    QSizeF grid = {o.scanInterval * scale.width(), o.stepInterval};     //管棒扫描在扫描方向上单位应该是degree
    QRectF sourceRect = o.adjustedSourceRect();
    qreal minMargin = qMin(grid.width(), grid.height());
    sourceRect = sourceRect.marginsAdded(QMarginsF(10 * minMargin, 10 * minMargin, 10 * minMargin, 10 * minMargin));

    ImageOptions mainChannelImageOptions(sourceRect, grid);
    mainChannelImageOptions.setCoordinateType(ImageOptions::CoordNormal);
    mainChannelImageOptions.setCyclularCenter(0);
    Axis *axis = MotionManager::axis(o.scanAxis);
    if (axis == nullptr) {
        return {};
    }
    mainChannelImageOptions.setAxisX(axis->index());
    mainChannelImageOptions.setAxisY(o.stepAxis());
    mainChannelImageOptions.setBacklash(QPointF(axis->config().backlash, 0));
    mainChannelImageOptions.setChannels(MotionAxisList(o.primaryChannelIdList()));
    mainChannelImageOptions.setScale(scale);

    QVector<QVector<float>> channelOffsets;
    auto offset = o.channelOffset();
    for (int key: offset.keys()) {
        if (key >= channelOffsets.size()) {
            channelOffsets.resize(key + 1);
        }
        channelOffsets.replace(key, offset.value(key));
    }
    mainChannelImageOptions.setChannelOffset(channelOffsets);

    if (o.alterSourceEnable) {
        ImageOptions subChannelImageOptions = mainChannelImageOptions;
        subChannelImageOptions.setChannels(MotionAxisList(o.secondaryChannelIdList()));
        subChannelImageOptions.setChannelOffset(channelOffsets);
        return QList<ImageOptions> ({mainChannelImageOptions, subChannelImageOptions});
    }
    return QList<ImageOptions> ({mainChannelImageOptions});
}

QPair<qreal, qreal> ScanPathCylinder::boundingRange(const QVariant &options, int axis) const
{
    if (MotionManager::axis(axis) == nullptr) {
        return qMakePair(0, 10);
    }
    const ScanPathCylinderOptions o = options.value<ScanPathCylinderOptions>();
    qreal min = o.pathRotation().nativeDegrees(0);
    qreal max = o.pathRotation().nativeDegrees(360);
    if (!(qIsFinite(min) && qIsFinite(max))) {
        return qMakePair(min, max);
    }
    return qMakePair(min, max);
}

ScanPathFactory::CheckResult ScanPathCylinder::checkOptions(const QVariant &options) const
{
    CheckResult result;
    auto o = options.value<ScanPathCylinderOptions>();

    Axis *axis = MotionManager::axis(o.scanAxis);
    if (axis == nullptr) {
        result.addError(tr("步进轴为空"));
        return result;
    }
    if (axis->axisType() != AxisConfig::Rotation) {
        result.addError(tr("%1 不是无限旋转轴").arg(axis->name()));
    }
    if (qFuzzyIsNull(o.stepInterval)) {
        result.addError(tr("无效的步进间隔"));
    }
    if (qFuzzyIsNull(o.effectiveLinearSpeed())) {
        result.addError(tr("扫描线速度无效"));
    }
    if (axis->GetSensorMap().isEmpty()) {
        result.addError(tr("传感器未设置"));
    }
//    if  (o.axisIndex(o.scanAxis) < 0 ) {
//        result.addError(tr("机械臂坐标系非笛卡尔坐标系"));
//    }
    if (o.posList.isEmpty()) {
        result.addError(tr("坐标列表为空"));
    }
    if ((!o.posList.isEmpty()) && (o.posList.first() == o.posList.last())) {
        result.addError(tr("起始点相同"));
    }
    if (o.effectiveLinearSpeed() > o.advicedLinearSpeed()) {
        result.addWarning(tr("超过推荐扫描线速度: %1").arg(o.advicedLinearSpeed()));
    }
    // TODO 添加超过推荐角速度warning
    return result;
}

QWidget *ScanPathCylinder::widget(const QVariant &options) const
{
    auto *edit = new ScanPathCylinderEdit;
    edit->setOptions(options.value<ScanPathCylinderOptions>());
    return edit;
}

QVariant ScanPathCylinder::widgetOptions(QWidget *widget) const
{
    auto *edit = qobject_cast<ScanPathCylinderEdit *>(widget);
    if (edit != nullptr) {
        return QVariant::fromValue(edit->options());
    }
    return QVariant();
}

MotionCondition ScanPathCylinderOptions::elementCondition() const
{
    MotionCondition condition;
    Axis *axis = MotionManager::axis(scanAxis);
    if (axis == nullptr) {
        return {};
    }
    QMap<QString, int> map = axis->GetSensorMap();   // map is Empty then add error
    if (map.isEmpty()) {
        return {};
    }
    condition.arg = map.first();
    condition.axis = MotionManager::axisIndex(scanAxis);
    condition.type = MotionCondition::ConditionIoFalling;
    return condition;
}

MotionElement ScanPathCylinderOptions::requisityElement() const
{
    MotionElement requisity;
    requisity.type = MotionElement::Continuous;
    requisity.mainAxis = MotionManager::axisIndex(scanAxis);
    requisity.axis.setAxis(MotionManager::axisIndex(scanAxis));
    requisity.speed = effectiveRotatorAngularSpeed();
    return requisity;
}

RotationInfo ScanPathCylinderOptions::pathRotation() const
{
    RotationInfo r;
    r.speedType = RotationInfo::Uniform;
    Axis *axis = MotionManager::axis(scanAxis);
    if (axis != nullptr) {
        r.axisDiameter = axis->config().axleDiameter;
        r.type = qFuzzyIsNull(axis->config().axleDiameter) ? RotationInfo::AxisContact : RotationInfo::SurfaceContact;
    }
    r.partDiameter = cylinderDiameter;
    r.axis = MotionManager::axisIndex(scanAxis);
    return r;
}

int ScanPathCylinderOptions::stepAxis() const
{
    if (posList.isEmpty()) {
        return -1;
    }
    const QVector3D start =  posList.first();
    const QVector3D end =  posList.last();
    EndPoint *endPoint = MotionManager::endPoint(endPointId);
    if (endPoint == nullptr) {
        return -1;
    }
    Axis *axis = endPoint->axis(ScanPathUtilities::maximumAxis(start - end));
    if (axis == nullptr) {
        return -1;
    }
    return axis->index();
}

QSizeF ScanPathCylinderOptions::unitScale() const
{
    QSizeF unitScale(1, 1);
    Axis *axis = MotionManager::axis(scanAxis);
    if (axis == nullptr) {
        return unitScale;
    }
    unitScale.setWidth(360 / (M_PI * axleDiameter()));
    return unitScale;
}

QRectF ScanPathCylinderOptions::adjustedSourceRect() const
{
    Axis * axis = MotionManager::axis(scanAxis);
    if (axis == nullptr) {
        return QRect();
    }
    if (posList.isEmpty()) {
        return QRect();
    }
    int direction = ScanPathUtilities::maximumAxis(posList.last() - posList.first());
    qreal width = qFuzzyCompare(axis->config().axleDiameter, 0) ? 360 : 360 * (cylinderDiameter / axis->config().axleDiameter);
    qreal height = qAbs((posList.last() - posList.first())[direction]);
    qreal stepDirection = (posList.last()[direction] - posList.first()[direction])  > 0 ? posList.first()[direction] : posList.last()[direction];
    return QRect(0, stepDirection, width, height);
}

QHash<int, QVector<float> > ScanPathCylinderOptions::channelOffset() const
{
    QHash<int, QVector<float> > offsets;                                        /*!< <channel, <axisOffset> > */
    int axisCount = MotionManager::axisCount();
    int channelListCount = primaryChannelIdList().count();
    if (alterSourceEnable) {
        channelListCount = secondaryChannelIdList().count();
    }
    for (int index = 0; index < channelListCount; ++index) {
        QVector<float> &offset = offsets[primaryChannelIdList()[index]];
        offset.resize(axisCount);
        offset.replace(stepAxis(), probeInterval * index);
        offsets.insert(index, offset);
    }
    return offsets;
}

int ScanPathCylinderOptions::axisIndex(const QUuid &axisId) const
{
    EndPoint *endPoint = MotionManager::endPoint(endPointId);
    if (endPoint == nullptr) {
        return -1;
    }
//    if (endPoint->type() != EndPoint::Cartesian) {
//        return -1;
//    }
    Axis *axis = endPoint->axis(axisId);
    if (axis == nullptr) {
        return -1;
    }
    return axis->index();
}

QList<int> ScanPathCylinderOptions::primaryChannelIdList() const
{
    if (channelSelect == Single) {
        return QList<int>({channel});
    }
    return channels;
}

QList<int> ScanPathCylinderOptions::secondaryChannelIdList() const
{
    return Utils::transform(primaryChannelIdList(), [] (int channelId) {
        UTChannel *channel = UltrasonicManager::channel(channelId);
        if (channel == nullptr) {
            return -1;
        }
        UTInstrument *instrument = channel->instrument();
        if (instrument == nullptr) {
            return -1;
        }
        UTChannel *secondaryChannel = instrument->channel(1);
        if (secondaryChannel == nullptr) {
            return -1;
        }
        return secondaryChannel->channelId();
    });
}

qreal ScanPathCylinderOptions::advicedLinearSpeed() const
{
    int prf = setup.prf();
    int sampleAverage = setup.sampleAverage();;
    if (!setupEnable) {
        const auto channels = UltrasonicManager::channels();
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
    return speed;
}

qreal ScanPathCylinderOptions::advicedRotatorAngularSpeed() const
{
    qreal speed = qRadiansToDegrees(effectiveLinearSpeed() / (axleDiameter() / 2));
    if (mode == SpecifyRotatorAngularSpeed) {
        speed = rotatorAngularSpeed;
    }
    Axis *axis = MotionManager::axis(scanAxis);
    if (axis == nullptr) {
        return speed;
    }
    return qMin(speed, axis->config().speedMax);
}

qreal ScanPathCylinderOptions::advicedCylinderAngularSpeed() const
{
    if (qFuzzyCompare(cylinderDiameter, 0)) {
        return 0;
    }
    return effectiveRotatorAngularSpeed() * axleDiameter() / cylinderDiameter;
}

qreal ScanPathCylinderOptions::axleDiameter() const
{
    Axis *axis = MotionManager::axis(scanAxis);
    if (axis == nullptr) {
        return qInf();
    }
    // 若选择的是卡盘，则返回工件直径
    if (qFuzzyIsNull(axis->config().axleDiameter)) {
        return cylinderDiameter;
    }
    return axis->config().axleDiameter;
}

qreal ScanPathCylinderOptions::effectiveLinearSpeed() const
{
    if (mode == SpecifyLinearSpeed) {
        return linearSpeed;
    }
    return advicedLinearSpeed();
}

qreal ScanPathCylinderOptions::effectiveRotatorAngularSpeed() const
{
    return advicedRotatorAngularSpeed();
}

qreal ScanPathCylinderOptions::effectiveCylinderAngularSpeed() const
{
    if (mode == SpecifyCylinderAngularSpeed) {
        return cylinderAngularSpeed;
    }
    return advicedCylinderAngularSpeed();
}
