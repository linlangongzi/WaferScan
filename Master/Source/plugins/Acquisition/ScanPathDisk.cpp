#include "ScanPathDisk.h"
#include "ScanPathDiskEdit.h"
#include "ScanPathUtilities.h"
#include <Core/Platform.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <Motion/RotationInfo.h>
#include <QLoggingCategory>
#include <QtMath>
#include <Ultrasonic/UltrasonicManager.h>
#include <utils/algorithm.h>

REGISTER_JSON_CONVERTER(ScanPathDiskOptions)

static Q_LOGGING_CATEGORY(debug, "acquire.path.disk");

ScanPathDisk::ScanPathDisk(QObject *parent) :
    ScanPathFactory(parent)
{

}

QString ScanPathDisk::name() const
{
    return "Disk";
}

QString ScanPathDisk::displayName() const
{
    return tr("盘环件");
}

QIcon ScanPathDisk::icon() const
{
    return QIcon(tr(":/fatcow/16x16/cd.png"));
}

MotionPath ScanPathDisk::motionPath(const QVariant &options) const
{
    ScanPathDiskOptions o = options.value<ScanPathDiskOptions>();
    Axis *axisRotation = MotionManager::axis(o.scanAxis);
    if (!axisRotation) {
        return MotionPath();
    }

    Axis *axisLp = nullptr;
    for (Axis *axis: MotionManager::axisList()) {
        if (axis->name() == "LP") {
            axisLp = axis;
        }
    }

    QVector3D offset = axisRotation->baseOffset();
    QVector3D direction = axisRotation->baseDirection();
    if (axisLp) {
        offset.setZ(axisLp->position() * direction.z());
    }
    if (o.coordinateSystem == ScanPathDiskOptions::Workpiece) {
        o.start += offset;
        o.end += offset;
    }

    QMap<QString, int> map = axisRotation->GetSensorMap();
    if (map.isEmpty()) {
        return MotionPath();
    }

    MotionPath path;
    path.interval = o.interval;

    ChannelList channelList;
    channelList << o.channel;
    path.multiChannels = QList<ChannelList>({channelList});
    const int scanAxisIndex = MotionManager::axisIndex(o.scanAxis);

    RotationInfo rotation;
    rotation.axis = scanAxisIndex;
    rotation.type = RotationInfo::AxisContact;
    rotation.direction = RotationInfo::Forward;
    rotation.speedType = RotationInfo::Uniform;
    rotation.maxAngularSpeed = axisRotation->config().speedMax;
    path.rotations << rotation;

    MotionElement prerequisite;
    prerequisite.type = MotionElement::Continuous;
    prerequisite.mainAxis = scanAxisIndex;
    prerequisite.axis.setAxis(scanAxisIndex);
    rotation = RotationInfo::byAxisIndex(path.rotations, prerequisite.mainAxis);

    prerequisite.speed = o.angularSpeed;
    prerequisite.stepSpeed = o.angularSpeed;
    path.prerequisites << prerequisite;

    EndPoint *endPoint = MotionManager::endPoint(o.endPointId);
    if (endPoint == nullptr) {
        return {};
    }
    MotionAxisList axisList;
    foreach (Axis *axis, endPoint->axisList().mid(0, 3)) {
            axisList.setAxis(axis->index(), true);
    }
    const int rotationAxisIndex = MotionManager::axisIndex(o.rotationAxis);
    MotionPose startPos = endPoint->motionPose(o.start);
    MotionPose endPos = endPoint->motionPose(o.end);
    if (o.axisRotationEnable) {
        axisList.setAxis(rotationAxisIndex, true);
        startPos.resize(axisList.count());
        startPos[rotationAxisIndex] = o.angleStart;
        endPos.resize(axisList.count());
        endPos[rotationAxisIndex] = o.angleEnd;
    }

    MotionElement element;
    element.axis = axisList;
    element.mainAxis = scanAxisIndex;
    element.type = MotionElement::MoveLine;

    MotionCondition condition;
    const int sensorId = map.first();
    condition.type = MotionCondition::ConditionIoRising;
    condition.axis = scanAxisIndex;
    condition.arg = sensorId;
    element.conditions << condition;


    QMap<qreal, int> lengthMap;
    for (int axis = 0; axis < startPos.count(); ++axis) {
        qreal delta = startPos.distanceOnAxis(endPos, axis);
        lengthMap[delta] = axis;
    }
    auto iter = lengthMap.end();
    while (iter != lengthMap.begin()) {
        iter--;
        if (iter.value() != element.mainAxis) {
            element.stepAxis = iter.value();
            break;
        }
    }

    element.speed = o.angularSpeed;
    element.stepSpeed = o.angularSpeed;
    element.stepInterval = o.interval;
    element.interPolicy = MotionElement::InterpolateBidiretional;

    MotionElement firstElement = element;
    firstElement.pos << startPos;
    MotionElement secondElement = element;
    secondElement.pos << endPos;

    path.elements << firstElement << secondElement;

    return path;
}

QHash<int, UTSetup> ScanPathDisk::setups(const QVariant &options) const
{
    ScanPathDiskOptions o = options.value<ScanPathDiskOptions>();
    QHash<int, UTSetup> setups;
    if (o.setupEnable) {
        setups[o.channel] = o.setup;
    }
    return setups;
}

QList<ImageOptions> ScanPathDisk::imageOptions(const QVariant &options) const
{
    const ScanPathDiskOptions o = options.value<ScanPathDiskOptions>();

    const int axisX = MotionManager::axisIndex(o.scanAxis);                     // 水平轴
    int axisY = o.stepAxis();
    QSizeF scale(1, 1);
    QRectF sourceRect;
    QSizeF gridSize(o.interval, o.interval);
    QPointF backlash(MotionManager::axis(axisX)->config().backlash, 0);
    auto cyclularType = ImageOptions::CoordNormal;
    const QVector3D offset = MotionManager::axis(o.scanAxis)->baseOffset();
    const QVector2D offset2D = offset.toVector2D();
    const qreal radius = (offset2D.distanceToPoint(o.start.toVector2D()) + offset2D.distanceToPoint(o.end.toVector2D())) / 2;
    qreal center = offset.x();

    if ((axisY == 0 || axisY == 1) && !o.rectangleEnable) {
        const qreal radius = qMax(offset2D.distanceToPoint(o.start.toVector2D()), offset2D.distanceToPoint(o.end.toVector2D()));    //决定画布大小和圆心位置
        sourceRect = QRectF(offset.x() - radius, offset.y() - radius, radius + radius, radius + radius);
        cyclularType = ImageOptions::CoordThetaR;
        if (axisY == 1) {
            center = offset.y();
        }
    } else {
        const qreal radius = (offset2D.distanceToPoint(o.start.toVector2D()) + offset2D.distanceToPoint(o.end.toVector2D())) / 2;
        const qreal angleInterval = 360.0 / (2 * M_PI * radius);
        auto maxAxis = ScanPathUtilities::maximumAxis(o.end - o.start);
        const qreal startY = qMin(o.start[maxAxis] , o.end[maxAxis]);
        const qreal height = qAbs(o.end[maxAxis] - o.start[maxAxis]);
        gridSize = QSizeF(angleInterval * o.interval, o.interval);
        sourceRect = QRectF(QPointF(0, startY), QSizeF(360, height));
        scale.setWidth(angleInterval);
        center = offset.z();
    }

    qCDebug(debug) << "Source rect" << sourceRect << "Grid size" << gridSize;
    qreal minMargin = qMin(gridSize.width(), gridSize.height());
    sourceRect = sourceRect.marginsAdded(QMarginsF(10 * minMargin, 10 * minMargin, 10 * minMargin, 10 * minMargin));
    qCDebug(debug) << "Source rect after margin" << sourceRect;

    ImageOptions imageOption(sourceRect, gridSize);
    imageOption.setCoordinateType(cyclularType);
    imageOption.setCyclularCenter(center);
    imageOption.setScale(scale);
    imageOption.setAxisX(axisX);
    imageOption.setAxisY(axisY);
    imageOption.setBacklash(backlash);

    QList<int> channels;
    channels << o.channel;
    MotionAxisList channelSelected;
    channelSelected.setAxis(o.channel);
    imageOption.setChannels(channelSelected);

    QVector<float> channelOffset;
    channelOffset.fill(0, MotionManager::axisCount());
    QVector<QVector<float>> channelOffsets;
    channelOffsets.fill(channelOffset, UltrasonicManager::channels().count());
    imageOption.setChannelOffset(channelOffsets);

    QList<ImageOptions> imageOptionList;
    imageOptionList << imageOption;

    return imageOptionList;
}

QPair<qreal, qreal> ScanPathDisk::boundingRange(const QVariant &options, int axis) const
{
    using Options = ScanPathDiskOptions;
    const Options o = options.value<Options>();
    Axis *axisPointer = MotionManager::axis(axis);
    if (axisPointer == nullptr) {
        return qMakePair(Q_SNAN, Q_SNAN);
    }

    if (axisPointer->name() == "X") {
        return qMakePair(o.start.x(), o.end.x());
    } else if (axisPointer->name() == "Y") {
        return qMakePair(o.start.y(), o.end.y());
    } else if (axisPointer->name() == "Z") {
        return qMakePair(o.start.z(), o.end.z());
    }
    return qMakePair(Q_SNAN, Q_SNAN);
}

ScanPathFactory::CheckResult ScanPathDisk::checkOptions(const QVariant &options) const
{
    CheckResult result;
    auto o = options.value<ScanPathDiskOptions>();

    if (UltrasonicManager::channel(o.channel) == nullptr) {
        result.addError(tr("无效的通道 %1").arg(o.channel));
    }
    const int scanAxisIndex = MotionManager::axisIndex(o.scanAxis);
    if (scanAxisIndex < 0) {
        result.addError(tr("无效的扫描轴 %1").arg(scanAxisIndex));
    }
    if (o.angularSpeed < 1e-6) {
        result.addError(tr("无效的扫描速度 %1").arg(o.angularSpeed));
    }
    if (o.interval < 1e-6) {
        result.addError(tr("无效的步进间隔 %1").arg(o.interval));
    }

    return result;
}

QWidget *ScanPathDisk::widget(const QVariant &options) const
{
    auto *edit = new ScanPathDiskEdit;
    edit->setOptions(options.value<ScanPathDiskOptions>());
    return edit;
}

QVariant ScanPathDisk::widgetOptions(QWidget *widget) const
{
    auto *edit = qobject_cast<ScanPathDiskEdit *>(widget);
    if (edit) {
        return QVariant::fromValue(edit->options());
    }
    return QVariant();
}

void ScanPathDisk::setWidgetEnabled(QWidget *widget, bool enable) const
{
    auto *diskEdit = dynamic_cast<ScanPathDiskEdit *>(widget);
    if (!diskEdit) {
        return;
    }
    diskEdit->setSetupVisibility(enable);
}

int ScanPathDiskOptions::stepAxis() const
{
    auto *endPoint = MotionManager::endPoint(endPointId);                                                              // 步进轴
    if (endPoint == nullptr) {
        return -1;
    }
    Axis *axis = endPoint->axis(ScanPathUtilities::maximumAxis(start - end));
    if (axis == nullptr) {
        return -1;
    }
    return axis->index();
}
