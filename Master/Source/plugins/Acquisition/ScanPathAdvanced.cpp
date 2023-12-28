#include "ScanPathAdvanced.h"
#include "ScanPathAdvancedEdit.h"
#include <Core/Platform.h>
#include <Imaging/UTDocument.h>
#include <Motion/MotionManager.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/MetaType/MetaType.h>

REGISTER_JSON_CONVERTER(ScanPathAdvancedOptions)

QPair<qreal, qreal> ScanPathAdvancedOptions::boundingRange(int axis) const
{
    if (!MotionManager::axis(axis)) {
        return qMakePair(0, 10);
    }
    qreal min = qInf();
    qreal max = -qInf();
    RotationInfo rotation = RotationInfo::byAxisIndex(motionPath.rotations, axis);
    auto type = KeyToEnum(motionType, ScanPlan::NONE);
    if ((type == ScanPlan::CYLINDER || type == ScanPlan::DISK) && rotation.type != RotationInfo::None) {
        min = rotation.nativeDegrees(0);
        max = rotation.nativeDegrees(360);
    } else {
        for (const MotionElement &element: motionPath.elements) {
            int axisIndex = element.axis.toList().indexOf(axis);
            for (const MotionPose &pose: element.pos) {
                min = qMin<qreal>(pose.value(axisIndex), min);
                max = qMax<qreal>(pose.value(axisIndex), max);
            }
        }
    }
    if (!(qIsFinite(min) && qIsFinite(max))) {
        return qMakePair(min, max);
    }
    return qMakePair(min, max);
}

QRectF ScanPathAdvancedOptions::boundingRect(int axisX, int axisY) const
{
    auto rangeX = boundingRange(axisX);
    auto rangeY = boundingRange(axisY);
    return Range::toRect(rangeX, rangeY);
}

QSizeF ScanPathAdvancedOptions::unitScale(int axisIndexX, int axisIndexY) const
{
    QSizeF unitScale(1, 1);
    auto type = KeyToEnum(motionType, ScanPlan::NONE);
    if (type == ScanPlan::CYLINDER || type == ScanPlan::CYLINDERROTATION) {
        RotationInfo rotationX = RotationInfo::byAxisIndex(motionPath.rotations, axisIndexX);
        RotationInfo rotationY = RotationInfo::byAxisIndex(motionPath.rotations, axisIndexY);
        if (rotationX.type != RotationInfo::None) {
            if (rotationX.type == RotationInfo::SurfaceContact) {
                unitScale.setWidth(360 / (M_PI * rotationX.axisDiameter));
            } else {
                unitScale.setWidth(360 / (M_PI * rotationX.partDiameter));
            }
        }
        if (rotationY.type != RotationInfo::None) {
            if (rotationY.type == RotationInfo::SurfaceContact) {
                unitScale.setHeight(360 / (M_PI * rotationY.axisDiameter));
            } else {
                unitScale.setHeight(360 / (M_PI * rotationY.partDiameter));
            }
        }
    }
    Q_ASSERT(qIsFinite(unitScale.height()));
    Q_ASSERT(qIsFinite(unitScale.width()));
    return unitScale;
}

QSizeF ScanPathAdvancedOptions::adviceGridSize(int axisX, int axisY) const
{
    qreal step = 0.01;
    qreal interval = motionPath.interval;
    for (const MotionElement &element: motionPath.elements) {
        step = qMax(step, element.stepInterval);
    }

    QSizeF scale = unitScale(axisX, axisY);
    return QSizeF(interval * scale.width(), step * scale.height());
}

QVector<QVector<float> > ScanPathAdvancedOptions::channelOffset(int imageIndex) const
{
    const auto elements = motionPath.elements;
    if (elements.isEmpty()) {
        return QVector<QVector<float>>();
    }

    QList<int> channels;
    for (const ChannelList &channelList: motionPath.multiChannels) {
        if (imageIndex >= channelList.count()) {
            break;
        }
        channels << channelList[imageIndex];
    }
    if (channels.isEmpty()) {
        channels << 0;
    }

    QVector<QVector<float>> offsets;
    const int globalChannelCount = UltrasonicManager::channelCount();
    offsets.resize(globalChannelCount);
    for (int index = 0; index < channels.count(); ++index) {
        QVector<float> offset;
        offset.resize(MotionManager::axisCount());
        offset.replace(elements.first().stepAxis, motionPath.channelOffset * index);
        const int channelIndex = channels[index];
        if (uint(channelIndex) >= uint(globalChannelCount)) {
            continue;
        }
        offsets.replace(channelIndex, offset);
    }

    return offsets;
}

MotionAxisList ScanPathAdvancedOptions::channelsSelected(int index) const
{
    MotionAxisList axisList;
    if (motionPath.multiChannelEnable) { // 多通道开启
        for (const ChannelList &channelList: motionPath.multiChannels) {
            if (channelList.count() < index + 1) {
                break;
            }
            axisList.setAxis(channelList.value(index));
        }
    }
    foreach (const ImageOptions &options, imageOptions) {
        if (!options.channels().isEmpty()) {
            axisList = options.channels();
        }
    }
    if (axisList.isEmpty()) {
        axisList.setAxis(0);
    }
    return axisList;
}

QPointF ScanPathAdvancedOptions::adviceBacklash(int axisIndexX, int axisIndexY) const
{
    Axis *axisX = MotionManager::axis(axisIndexX);
    Axis *axisY = MotionManager::axis(axisIndexY);
    if (axisX && axisY) {
        if (motionPath.elements.count() > 0) {
            MotionElement element = motionPath.elements.value(0);
            if (element.mainAxis == axisIndexX) {
                return QPointF(axisX->config().backlash, 0);
            } else if (element.mainAxis == axisIndexY) {
                return QPointF(0, axisY->config().backlash);
            }
        }
    }
    return QPointF(0, 0);
}

ImageOptions ScanPathAdvancedOptions::adviceImageOptions(int index) const
{
    int axisX = 0;
    int axisY = 1;

    auto type = KeyToEnum(motionType, ScanPlan::NONE);
    if (type == ScanPlan::NONE) {                                              // 如果是文件夹形式，成像轴根据第一个有效扫描计划决定
        return {};
    }
    if (motionPath.elements.size() > 0) {
        const auto &first = motionPath.elements.first();
        axisX = first.mainAxis;
        axisY = first.stepAxis;
    }

    QRectF sourceRect;
    QSizeF gridSize;
    auto cyclularType = ImageOptions::CoordNormal;
    QSizeF scale = unitScale(axisX, axisY);
    double stepCenter = 0;
    auto path = motionPath;
    int scanAxisIndex = path.elements.value(0).mainAxis;
    if ((type == ScanPlan::DISK || type == ScanPlan::DISKROTATION) && (scanAxisIndex == axisX || scanAxisIndex == axisY)) {
        Axis *scanAxis = MotionManager::axis(scanAxisIndex);
        Q_ASSERT(scanAxis);
        QVector3D direction = scanAxis->baseDirection();
        QVector3D offset = scanAxis->baseOffset();
        qreal radius = 0;
        qreal step = 1;
        for (const MotionElement &element: path.elements) {
            step = element.stepInterval;
            for (const MotionPose &pos: element.pos) {
                QVector3D point(pos.value(0), pos.value(1), pos.value(2));
                radius = qMax<qreal>(radius, point.distanceToLine(offset, direction));
            }
        }
        sourceRect = QRectF(offset.x() - radius, offset.y() - radius, radius + radius, radius + radius);
        cyclularType = ImageOptions::CoordThetaR;
        stepCenter = axisY == 0 ? offset.x() : offset.y();
        gridSize = QSizeF(step, step);
    } else {
        sourceRect = boundingRect(axisX, axisY);
        gridSize = adviceGridSize(axisX, axisY);
    }

    gridSize = QSizeF(gridSize.width() / scale.width(), gridSize.height() / scale.height());
    qreal minMargin = qMin(gridSize.width(), gridSize.height());
    sourceRect = sourceRect.marginsAdded( QMarginsF(10 *minMargin, 10 * minMargin, 10 * minMargin, 10 * minMargin) );

    ImageOptions transform(sourceRect, gridSize);
    transform.setCoordinateType(cyclularType);
    transform.setCyclularCenter(stepCenter);
    transform.setScale(scale);
    transform.setAxisX(axisX);
    transform.setAxisY(axisY);
    transform.setBacklash(adviceBacklash(axisX, axisY));
    transform.setChannelOffset(channelOffset(index));
    transform.setChannels(channelsSelected(index));

    for (const MotionElement &element: path.elements) {
        if (element.interPolicy == MotionElement::InterpolateUnidirectional) {
            transform.setDirectionFilter(ImageOptions::DirectionForward);
        }
    }

    return transform;
}

ScanPathAdvanced::ScanPathAdvanced(QObject *parent) :
    ScanPathFactory(parent)
{
}

QString ScanPathAdvanced::name() const
{
    return "Advanced";
}

QString ScanPathAdvanced::displayName() const
{
    return tr("高级");
}

QIcon ScanPathAdvanced::icon() const
{
    return QIcon(tr(":/fatcow/16x16/draw_spiral.png"));
}

MotionPath ScanPathAdvanced::motionPath(const QVariant &options) const
{
    auto o = options.value<ScanPathAdvancedOptions>();
    return o.motionPath;
}

QHash<int, UTSetup> ScanPathAdvanced::setups(const QVariant &options) const
{
    auto o = options.value<ScanPathAdvancedOptions>();
    return o.m_setups;
}

QList<ImageOptions> ScanPathAdvanced::imageOptions(const QVariant &options) const
{
    QList<ImageOptions> imageOptionsList;
    auto o = options.value<ScanPathAdvancedOptions>();
    for (UTLayer *layer: document()->GetLayerMap()) {
        imageOptionsList << o.adviceImageOptions(layer->layerIndex());
    }
    return imageOptionsList;
}

QPair<qreal, qreal> ScanPathAdvanced::boundingRange(const QVariant &options, int axis) const
{
    auto o = options.value<ScanPathAdvancedOptions>();

    return o.boundingRange(axis);
}

ScanPathFactory::CheckResult ScanPathAdvanced::checkOptions(const QVariant &options) const
{
    CheckResult result;
    auto o = options.value<ScanPathAdvancedOptions>();
    auto type = KeyToEnum(o.motionType, ScanPlan::NONE);
    if (type == ScanPlan::NONE) {
        result.addError(tr("无效的路径类型"));
    }
    for (const auto &element: o.motionPath.elements) {
        if (element.speed < 1e-6) {
            result.addError(tr("扫描速度无效"));
        }
        if (element.stepSpeed < 1e-6) {
            result.addError(tr("步进速度无效"));
        }
        for (const auto &condition: element.conditions) {
            if (condition.arg < 0 && (condition.type == MotionCondition::ConditionIoRising || condition.type == MotionCondition::ConditionIoRising)) {
                result.addError(tr("步进传感器设置无效"));
            }
            if (condition.arg < 0 && condition.type == MotionCondition::ConditionDelay) {
                result.addError(tr("步进延迟设置无效"));
            }
        }
    }
    return result;
}

//QList<RecordConfig> ScanPathAdvanced::recordOptions(const QVariant &options) const
//{
//    auto o = options.value<ScanPathAdvancedOptions>();
//    return o.records;
//}

//SafetyBarrierOptions ScanPathAdvanced::safetyBarrier(const QVariant &options) const
//{
//    auto o = options.value<ScanPathAdvancedOptions>();
//    return o.safetyBarrier;
//}

QWidget *ScanPathAdvanced::widget(const QVariant &options) const
{
    auto *edit = new ScanPathAdvancedEdit;
    edit->setOptions(options.value<ScanPathAdvancedOptions>());
    return edit;
}

QVariant ScanPathAdvanced::widgetOptions(QWidget *widget) const
{
    auto *edit = qobject_cast<ScanPathAdvancedEdit *>(widget);
    if (edit) {
        return QVariant::fromValue(edit->options());
    }
    return QVariant();
}

void ScanPathAdvanced::setWidgetEnabled(QWidget *widget, bool enable) const
{
    auto *advanceEdit = dynamic_cast<ScanPathAdvancedEdit *>(widget);
    if (!advanceEdit) {
        return;
    }
    advanceEdit->setSetupVisibility(enable);
}
