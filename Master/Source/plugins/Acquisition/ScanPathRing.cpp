#include "ScanPathRing.h"
#include "ScanPathRingEdit.h"
#include "AcquisitionConstants.h"
#include <Core/Platform.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <QLoggingCategory>
#include <QtMath>
#include <Ultrasonic/UltrasonicManager.h>

static const qreal angularSpeedMax = 50;
static const qreal stepSpeedScale = 0.5;
REGISTER_JSON_CONVERTER(ScanPathRingOptions);

static Q_LOGGING_CATEGORY(debug, "acquire.ring");

using namespace Acquisition;

ScanPathRing::ScanPathRing(QObject *parent) :
    ScanPathFactory(parent)
{
}

QString ScanPathRing::name() const
{
    return "Ring";
}

QString ScanPathRing::displayName() const
{
    return tr("圆环");
}

QIcon ScanPathRing::icon() const
{
    return QIcon(":/fatcow/16x16/dish.png");
}

MotionPath ScanPathRing::motionPath(const QVariant &options) const
{
    using Options = ScanPathRingOptions;
    Options option = options.value<ScanPathRingOptions>();

    const int channelCount = option.multiChannels.count();
    if (channelCount == 0 || qFuzzyCompare(0, option.stepInterval)) {
        return MotionPath();
    }
    qCDebug(debug) << "Annulus motion path";

//    QSettings *settings = Core::ICore::settings();
//    settings->beginGroup(K_GROUP_RING);
    const qreal safetyGap = platform()->Get("AnnulusPressingDistance").toDouble(0);
    const qreal probeBoxThickness = platform()->Get("AnnulusProbeBoxThickness").toDouble();
    const QPointF placementOrigin = QPointF(platform()->Get("AnnulusDatumPointX").toDouble(), platform()->Get("AnnulusDatumPointY").toDouble());
    const QPointF placementDirection = QPointF(platform()->Get("AnnulusDirectionX").toDouble(), platform()->Get("AnnulusDirectionY").toDouble());
//    settings->endGroup();

    const qreal offset = option.probeInterval;

    MotionPath path;
    path.interval = option.stepInterval;
    path.multiChannelEnable = false;
    path.channelOffset = offset;
    path.multiChannels = option.multiChannels;

    MotionAxisList axes;
    for (auto axis: MotionManager::axisList()) {
        int axisId = axis->config().axisId;
        QString axisName = axis->name();
        if ((axisName == "X" && axisId == 0)
         || (axisName == "Y" && axisId == 1)
         || (axisName == "Z" && axisId == 2)
         || (axisId == option.scanAxis)) {                                      // ANNULUS 工件类型约定：X轴 id为0; Y轴 id为1; Z轴 id为2
            axes.setAxis(axisId, true);
        }
    }
    if (axes.count() < 4) {                                                     // X、Y、Z轴配置错误返回空的MotionPath
        return MotionPath();
    }

    MotionElement element;
    element.type = MotionElement::MoveCircle;
    element.axis = axes;
    element.mainAxis = option.scanAxis;

    if (option.angleStart >= option.angleEnd) {
        qSwap(option.angleStart, option.angleEnd);
    }

    qreal angleOffset = 0;
    qreal radius = option.innerRadius;
    qreal safeRadius = radius;
    qreal mainStep = option.height;
    qreal depth = option.depth;
    qreal startBase = depth;
    qreal pressingAngle = option.pressingAngle;
    if (option.direction == Options::Outside) {
        radius = option.radius + probeBoxThickness;
        safeRadius = radius + safetyGap;
        angleOffset = MotionPath::isInbounds(-180, option.angleStart, 360) ? -180 : 180;
        option.angleStart += angleOffset;
        option.angleEnd += angleOffset;
        pressingAngle += angleOffset;
    } else if (option.direction == Options::Inside) {
        radius = option.innerRadius - probeBoxThickness;
        safeRadius = radius - safetyGap;
    } else if (option.direction == Options::Above) {
        startBase = option.innerRadius;
        mainStep = qAbs(option.radius - option.innerRadius);
    }
    if (radius < 0 || depth < 0) {
        return MotionPath();
    }
    qCDebug(debug) << "Total step:" << mainStep << EnumToKey(option.direction) << "direction";

    element.speed = qMin(360 * option.linearSpeed / (2 * M_PI * radius), angularSpeedMax);
    element.stepInterval = option.stepInterval;
    element.stepSpeed = option.linearSpeed * stepSpeedScale;

    QPointF center = option.center;
    if (option.centerType == Options::Calculate) {
        center = placementOrigin + placementDirection * option.radius;
    }

    MotionPose annulus;
    annulus << center.x() << center.y() << radius << depth << option.height;    // 圆心X、圆心Y、起始半径、表面深度、工件高度

    MotionPose angle;
    /*! 此处反着添加时因为使用时先互换0-1再使用 */
    angle << option.angleEnd << option.angleStart << angleOffset;               // 起始角度、目标角度、偏移角度
    element.pos << annulus << angle;

    /*! 创建起始 locate */
    if (option.direction == Options::Above) {
        const qreal startRadian = qDegreesToRadians(option.angleStart);

        MotionPose posAboveStart;                                               /*! 上表面 检测起点上方 */
        posAboveStart << center.x() + radius * qCos(startRadian) << center.y() + radius * qSin(startRadian) << 0;
        MotionPose posStart;                                                    /*! 上表面 检测起点位置 */
        posStart << center.x() + radius * qCos(startRadian) << center.y() + radius * qSin(startRadian) << option.depth;

        MotionElement aboveOverStart;                                           /*! 上表面 定位到指定的检测起点上空 */
        aboveOverStart.type = MotionElement::None;
        aboveOverStart.axis = axes;
        aboveOverStart.pos << posAboveStart;
        path.addElement(aboveOverStart);

        MotionElement aboveRotate;                                              /*! 上表面 C轴旋转到检测起始角度 */
        aboveRotate.type = MotionElement::Locate;
        aboveRotate.pos << MotionPose();
        aboveRotate.pos.first() << option.angleStart;
        aboveRotate.axis.setAxis(option.scanAxis);
        path.addElement(aboveRotate);

        MotionElement aboveStart;                                               /*! 上表面 下降到工件上表面 */
        aboveStart.type = MotionElement::Locate;
        aboveStart.pos << posStart;
        aboveStart.axis = axes;
        path.addElement(aboveStart);
    } else if (option.direction == Options::Inside || option.direction == Options::Outside) {
        if (!MotionPath::isInbounds(option.angleStart, pressingAngle, option.angleEnd)) {
            qCWarning(debug) << "Dangerous pressing angle" << pressingAngle - angleOffset;
            return MotionPath();                                                /*! 在工件检测角度范围外下压后移动有危险 */
        }

        const qreal pressingRadian = qDegreesToRadians(pressingAngle - angleOffset);

        MotionPose safePose;                                                    /*! 侧面 指定安全下落角度的上空 */
        safePose << center.x() + safeRadius * qCos(pressingRadian) << center.y() + safeRadius * qSin(pressingRadian) << 0;
        MotionPose safePoseDown;                                                /*! 侧面 指定下落位置 */
        safePoseDown << center.x() + safeRadius * qCos(pressingRadian) << center.y() + safeRadius * qSin(pressingRadian) << option.depth;
        MotionPose pressingPosDown;                                             /*! 侧面 指定探头盒下压后的位置 */
        pressingPosDown << center.x() + radius * qCos(pressingRadian) << center.y() + radius * qSin(pressingRadian) << option.depth;

        MotionElement sideSafe;                                                 /*! 侧面 定位到安全的下降角度 */
        sideSafe.type = MotionElement::None;
        sideSafe.axis = axes;
        sideSafe.pos << safePose;
        path.addElement(sideSafe);

        MotionElement sideRotate;                                               /*! 侧面 C轴旋转到正对工件检测面的角度 */
        sideRotate.type = MotionElement::Locate;
        sideRotate.pos << MotionPose();
        sideRotate.pos.first() << pressingAngle;
        sideRotate.axis.setAxis(option.scanAxis);
        path.addElement(sideRotate);

        MotionElement sideSafeDown;                                             /*! 侧面 下降到安全半径的指定深度 */
        sideSafeDown.type = MotionElement::Locate;
        sideSafeDown.pos << safePoseDown;
        sideSafeDown.axis = axes;
        path.addElement(sideSafeDown);

        MotionElement sidePressingDown;                                         /*! 侧面 将探头盒压到工件上 */
        sidePressingDown.type = MotionElement::Locate;
        sidePressingDown.pos << pressingPosDown;
        sidePressingDown.axis = axes;
        path.addElement(sidePressingDown);

        MotionElement sideCircleToStart = element;                              /*! 侧面 将探头盒转动到指定开始检测的角度 */
        sideCircleToStart.pos[1][0] = pressingAngle;
        sideCircleToStart.pos[1][1] = option.angleStart;
        path.addElement(sideCircleToStart);
    }

    if (channelCount > 1) {
        const qreal patchSpan = offset * channelCount;                          /*!< 一个扫描块能够覆盖的步进距离 */
        qCDebug(debug) << "Patch span:" << patchSpan;

        int patchCount = 0;
        if (!qFuzzyCompare(0, patchSpan)) {
            patchCount = qFloor(mainStep / patchSpan);                          /*!< 扫描需要的完整块数 */
        }
        qCDebug(debug) << "Patch count:" << patchCount;

        const qreal patchLast = mainStep - patchCount * patchSpan;              /*!< 完整块之后, 最后一个扫描块的尺寸 */
        qCDebug(debug) << "Last patch:" << patchLast;

        if (patchCount == 0) {
            const qreal realStep = qMin(patchLast, offset);
            const qreal start = startBase;
            const qreal end = start + realStep;
            path.elements << expandByDirection(element, option, start, end);
            qCDebug(debug) << "No full patch. Single step size" << realStep;
        } else {
            for (int patchIndex = 0; patchIndex < patchCount; ++patchIndex) {
                const qreal start = startBase + patchIndex * patchSpan;
                const qreal end = start + offset;
                path.elements << expandByDirection(element, option, start, end);
            }
            qCDebug(debug) << "Processing " << patchCount << "full patches, and last patch length" << patchLast;

            if (qFuzzyIsNull(patchLast)) {
                /* 不存在不完整的块, 跳过 */
            } else if (patchLast <= offset) {                           /* 剩余的部分不足1块, 将最后一个完整路径延长 */
                const qreal start = startBase + (patchCount - 1) * patchSpan + offset;
                const qreal end = start + patchLast;
                path.elements << expandByDirection(element, option, start, end);
                qCDebug(debug) << "Adding final patch: extending last patch by" << patchLast;
            } else if (patchLast > offset) {                            /* 剩余的部分超过一块, 但不足n块, 令最后一个探头指向结尾 */
                const qreal start = startBase + (patchCount - 1) * patchSpan + patchLast;
                const qreal end = start + offset;
                path.elements << expandByDirection(element, option, start, end);
                qCDebug(debug) << "Adding final patch with offset" << start << "~" << start + offset;
            }
        }
    } else {
        const qreal start = startBase;
        const qreal end = start + mainStep + option.stepInterval;
        path.elements << expandByDirection(element, option, start, end);
        qCDebug(debug) << "Single channel full step size" << mainStep;
    }

    /*! 创建结尾 locate */
    MotionElement lastElement = path.elements.last();
    if (lastElement.type == MotionElement::MoveCircle) {
        if (option.direction != Options::Above) {
            MotionElement circleEnd = lastElement;                                  /*! 将探头盒转动到可以安全抬起的角度 */
            circleEnd.pos[1][0] = lastElement.pos[1][1];
            circleEnd.pos[1][1] = pressingAngle;
            path.addElement(circleEnd);

            MotionPose pullUpPos;
            const qreal putUpRadian = qDegreesToRadians(pressingAngle - angleOffset);
            pullUpPos << center.x() + safeRadius * qCos(putUpRadian) << center.y() + safeRadius * qSin(putUpRadian);

            MotionElement pullUp;                                                   /*! 移动探头盒远离工件 */
            pullUp.type = MotionElement::Locate;
            pullUp.axis = QList<int>({0, 1});
            pullUp.pos << pullUpPos;
            path.addElement(pullUp);
        }

        MotionPose poseEndUp;
        poseEndUp << 0;

        MotionElement locateEndUp;                                              /*! 升起Z轴到最高点,整个检测流程结束 */
        locateEndUp.type = MotionElement::Locate;
        locateEndUp.axis.setAxis(2);
        locateEndUp.pos << poseEndUp;
        path.addElement(locateEndUp);
    }

    return path;
}

QHash<int, UTSetup> ScanPathRing::setups(const QVariant &options) const
{
    Q_UNUSED(options);
    return {};
}

QList<ImageOptions> ScanPathRing::imageOptions(const QVariant &options) const
{
    const ScanPathRingOptions o = options.value<ScanPathRingOptions>();

    int axisX = 0;
    int axisY = 1;
    if (o.direction == ScanPathRingOptions::Inside || o.direction == ScanPathRingOptions::Outside) {
        axisX = o.scanAxis;
        axisY = 2;
    }
    return QList<ImageOptions>{imageOption(o, 0, axisX, axisY)};
}

QPair<qreal, qreal> ScanPathRing::boundingRange(const QVariant &options, int axis) const
{
    const ScanPathRingOptions o = options.value<ScanPathRingOptions>();

    if (o.direction == ScanPathRingOptions::Above) {
        if (axis == 0) {
            return qMakePair(o.center.x() - o.radius, o.center.x() + o.radius);
        } else if (axis == 1) {
            return qMakePair(o.center.y() - o.radius, o.center.y() + o.radius);
        } else if (axis == 2) {
            return qMakePair(o.depth, o.depth);
        } else if (axis == 3) {
            return qMakePair(o.angleStart, o.angleEnd);
        }
    } else if (o.direction == ScanPathRingOptions::Inside) {
        if (axis == 0) {
            return qMakePair(o.center.x() - o.innerRadius, o.center.x() + o.innerRadius);
        } else if (axis == 1) {
            return qMakePair(o.center.y() - o.innerRadius, o.center.y() + o.innerRadius);
        } else if (axis == 2) {
            return qMakePair(o.depth, o.depth + o.height);
        } else if (axis == 3) {
            return qMakePair(o.angleStart, o.angleEnd);
        }
    } else if (o.direction == ScanPathRingOptions::Outside) {
        if (axis == 0) {
            return qMakePair(o.center.x() - o.radius, o.center.x() + o.radius);
        } else if (axis == 1) {
            return qMakePair(o.center.y() - o.radius, o.center.y() + o.radius);
        } else if (axis == 2) {
            return qMakePair(o.depth, o.depth + o.height);
        } else if (axis == 3) {
            return qMakePair(o.angleStart, o.angleEnd);
        }
    }
    return qMakePair(Q_SNAN, Q_SNAN);
}

//QList<RecordConfig> ScanPathRing::recordOptions(const QVariant &options) const
//{
//    const ScanPathRingOptions option = options.value<ScanPathRingOptions>();

//    QList<RecordConfig> records;
//    auto axes = imageAxes(option);

//    for (int index = 0; index < option.multiChannels.count(); ++index) {
//        RecordConfig record;
//        record.imageOptions = imageOption(option, index, axes.first, axes.second);

//        records << record;
//    }
//    return records;
//}

QWidget *ScanPathRing::widget(const QVariant &options) const
{
    auto *widget = new ScanPathRingEdit;
    widget->setOptions(options.value<ScanPathRingOptions>());
    return widget;
}

QVariant ScanPathRing::widgetOptions(QWidget *widget) const
{
    auto *edit = qobject_cast<ScanPathRingEdit *>(widget);
    if (!edit) {
        return QVariant();
    }
    return QVariant::fromValue(edit->options());
}

ImageOptions ScanPathRing::imageOption(const ScanPathRingOptions &option, int index, int axisX, int axisY) const
{
    const qreal radius = (option.direction == ScanPathRingOptions::Inside) ? option.innerRadius : option.radius;
    const qreal diameter = radius * 2;
    QPointF center(option.center.x(), option.center.y());
    QSizeF gridSize = QSizeF(option.stepInterval, option.stepInterval);
    QRectF sourceRect = QRectF(QPointF(center.x() - radius, center.y() - radius), QSize(diameter, diameter));

    QSizeF unitScale(1, 1);
    if (option.direction != ScanPathRingOptions::Above) {
        qreal angleInterval = 360.0 / (2 * M_PI * radius);
        sourceRect = QRectF(QPointF(option.angleStart, option.depth), QSize(qAbs(option.angleStart - option.angleEnd), option.height));
        gridSize = QSizeF(angleInterval * option.stepInterval, option.stepInterval);
        unitScale.setWidth(angleInterval);
    }

    const qreal minMargin = qMin(gridSize.width(), gridSize.height());
    sourceRect = sourceRect.marginsAdded( QMarginsF(10 * minMargin, 10 * minMargin, 10 * minMargin, 10 * minMargin) );

    ImageOptions transform(sourceRect, gridSize);
    transform.setFeatureIndex(3);
    transform.setCoordinateType(ImageOptions::CoordNormal);
    transform.setScale(unitScale);
    transform.setAxisX(axisX);
    transform.setAxisY(axisY);

    QList<int> channels;
    MotionAxisList channelSelected;
    for (const ChannelList &channelList: option.multiChannels) {
        if (index >= channelList.count()) {
            break;
        }
        channels << channelList[index];
        channelSelected.setAxis(channelList[index]);
    }
    if (channelSelected.isEmpty()) {
        channels << 0;
        channelSelected.setAxis(0);
    }
    transform.setChannels(channelSelected);

    QHash<int, QVector<float>> offsets;
    const int channelCount = UltrasonicManager::channels().count();
    const int axisCount = MotionManager::axisCount();
    const int stepAxisIndex = option.direction == (ScanPathRingOptions::Above) ? 0 : 2;
    for (int index = 0; index < channels.count(); ++index) {
        QVector<float> offset;
        offset.resize(axisCount);
        offset.replace(stepAxisIndex, option.probeInterval * index);
        const int channelIndex = channels[index];
        if (uint(channelIndex) >= uint(channelCount)) {
            continue;
        }
        offsets[channelIndex] = offset;
    }
    transform.setChannelOffset(offsets.values().toVector());

//    qCDebug(debug) << "Annulus image option" << transform;
    return transform;
}

QPair<int, int> ScanPathRing::imageAxes(const ScanPathRingOptions &option) const
{
    if (option.direction == ScanPathRingOptions::Inside
            || option.direction == ScanPathRingOptions::Outside) {
        return qMakePair(option.scanAxis, 2);
    }

    return qMakePair(0, 1);
}

QList<MotionElement> ScanPathRing::expandByDirection(MotionElement &base, const ScanPathRingOptions &option, qreal start, qreal end) const
{
    QList<MotionElement> elementList;

    if (option.direction == ScanPathRingOptions::Above) {
        for (qreal radius = start; radius < end; radius += base.stepInterval) {
            base.speed = qMin(360 * option.linearSpeed / (2 * M_PI * radius), angularSpeedMax);
            elementList << base.modifyRadius(radius);
        }
    } else if (option.direction == ScanPathRingOptions::Inside
               || option.direction == ScanPathRingOptions::Outside) {
        for (qreal depth = start; depth < end; depth += base.stepInterval) {
            elementList << base.modifyDepth(depth);
        }
    } else {
        elementList << base;
    }

    return elementList;
}
