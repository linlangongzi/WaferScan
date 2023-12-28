#include "ScanPathDiskEdit.h"
#include "ui_ScanPathDiskEdit.h"
#include "AxisSelector.h"
#include "PointXYZ.h"
#include <Core/Platform.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QtMath>
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <Waveform/DialogUTSetup.h>

static Q_LOGGING_CATEGORY(debug, "scan.disk");

ScanPathDiskEdit::ScanPathDiskEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanPathDiskEdit)
{
    ui->setupUi(this);

    ui->channel->clear();
    for (UTChannel *channel: UltrasonicManager::channels()) {
        ui->channel->addItem(channel->name(), channel->channelId());
    }

    ui->coordinateSystem->clear();
    ui->coordinateSystem->addItem(tr("机械坐标系"), ScanPathDiskOptions::Equipment);
//    ui->coordinateSystem->addItem(tr("工件坐标系"), ScanPathDiskOptions::Workpiece);

    ui->scanAxis->setFilter([](Axis *axis){
        return (axis->axisType() == AxisConfig::Rotation);
    });
    ui->axesRotationLimited->setFilter([](Axis *axis){
        return (axis->axisType() == AxisConfig::RotationLimited);
    });

    connect(ui->scanAxis, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ScanPathDiskEdit::refreshOffset);
    connect(ui->scanAxis, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ScanPathDiskEdit::refreshSpeed);
    connect(ui->startPoint, &PointXYZ::pointChanged, this, &ScanPathDiskEdit::refreshSpeed);
    connect(ui->endPoint, &PointXYZ::pointChanged, this, &ScanPathDiskEdit::refreshSpeed);

    for (UTChannel *channel: UltrasonicManager::channels()) {
        connect(channel, &UTChannel::setupChanged, this, &ScanPathDiskEdit::refreshSpeed);
    }
}

ScanPathDiskEdit::~ScanPathDiskEdit()
{
    delete ui;
}

void ScanPathDiskEdit::setOptions(const ScanPathDiskOptions &options)
{
    ui->endPointUid->setCurrentUuid(options.endPointId);
    ui->channel->setCurrentIndex(ui->channel->findData(options.channel));
    ui->interval->setValue(options.interval);
    ui->coordinateSystem->setCurrentIndex(ui->coordinateSystem->findData(options.coordinateSystem));
    ui->scanAxis->setCurrentUuid(options.scanAxis);
    ui->startPoint->setDisplayPos(options.start);
    ui->endPoint->setDisplayPos(options.end);
    ui->angularSpeed->setValue(options.angularSpeed);
    ui->axisRotationEnable->setChecked(options.axisRotationEnable);
    ui->axesRotationLimited->setCurrentUuid(options.rotationAxis);
    ui->startPoint->setAngle(options.angleStart);
    ui->endPoint->setAngle(options.angleEnd);
    ui->setupEnable->setChecked(options.setupEnable);
    ui->rectangleEnable->setChecked(options.rectangleEnable);
    m_setup = options.setup;

    refreshSpeed();
}

ScanPathDiskOptions ScanPathDiskEdit::options() const
{
    ScanPathDiskOptions options;
    options.endPointId = ui->endPointUid->currentId();
    options.channel = ui->channel->currentData(Qt::UserRole).toInt();
    options.scanAxis = ui->scanAxis->currentAxisUuid();
    options.angularSpeed = ui->angularSpeed->value();
    options.interval = ui->interval->value();
    options.start = ui->startPoint->displayPos();
    options.end = ui->endPoint->displayPos();
    options.coordinateSystem = ScanPathDiskOptions::CoordinateSystem(ui->coordinateSystem->currentData(Qt::UserRole).toInt());
    options.axisRotationEnable = ui->axisRotationEnable->isChecked();
    options.rotationAxis = ui->axesRotationLimited->currentAxisUuid();
    options.angleStart = ui->startPoint->angle();
    options.angleEnd = ui->endPoint->angle();
    options.setupEnable = ui->setupEnable->isChecked();
    options.rectangleEnable = ui->rectangleEnable->isChecked();
    options.setup = m_setup;
    if (!options.setupEnable) {
        options.setup = {};
    }

    return options;
}

void ScanPathDiskEdit::setSetupVisibility(bool enable)
{
    for (auto object: ui->parameterContainer->children()) {
        auto *widget = dynamic_cast<QWidget*>(object);
        if (widget) {
            widget->setEnabled(enable);
        }
    }
    ui->groupStart->setEnabled(enable);
    ui->groupEnd->setEnabled(enable);
    ui->setup->setEnabled(true);
}

void ScanPathDiskEdit::on_setup_clicked()
{
    UTSetup setup;
    setup = m_setup;

    DialogUTSetup dialog(this);
    dialog.setSetup(setup);
    if (dialog.exec() == QDialog::Accepted) {
        m_setup = setup;
        refreshSpeed();
    }
}

void ScanPathDiskEdit::refreshSpeed()
{
    ui->suggestedSpeed->setText(tr("最大角速度：%1 °/s").arg(suggestedSpeed(), 0, 'f', 2));
    on_angularSpeed_valueChanged(ui->angularSpeed->value());
}

void ScanPathDiskEdit::refreshOffset()
{
    const QVector3D offset = calculateOffset();
    ui->startPoint->setOffset(offset);
    ui->endPoint->setOffset(offset);
    ui->origin->setText(tr("(%1, %2, %3)").arg(offset.x()).arg(offset.y()).arg(offset.z()));
}

Axis *ScanPathDiskEdit::liftingAxis() const
{
    for (Axis *axis: MotionManager::axisList()) {
        if (axis->name() == "LP") {
            return axis;
        }
    }
    return nullptr;
}

QVector3D ScanPathDiskEdit::calculateOffset() const
{
    Axis *axisLp = liftingAxis();
    Axis *axisRotation = ui->scanAxis->currentAxis();

    if (!axisRotation || !axisLp) {
        return {};
    }
    QVector3D offset(0, 0, 0);
    if (coordinateSystem() == ScanPathDiskOptions::Workpiece) {
        const QVector3D direction = axisRotation->baseDirection();
        offset = axisRotation->baseOffset();
        offset.setZ(offset.z() - axisLp->position() * direction.z());
    }
    return offset;
}

ScanPathDiskOptions::CoordinateSystem ScanPathDiskEdit::coordinateSystem() const
{
    return ScanPathDiskOptions::CoordinateSystem(ui->coordinateSystem->currentData(Qt::UserRole).toInt());
}

void ScanPathDiskEdit::on_coordinateSystem_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    const QVector3D offset = calculateOffset();
    ui->startPoint->setOffsetAndRefresh(offset);
    ui->endPoint->setOffsetAndRefresh(offset);
    ui->origin->setText(tr("(%1, %2, %3)").arg(offset.x()).arg(offset.y()).arg(offset.z()));
}

void ScanPathDiskEdit::on_scanAxis_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    for (Axis *axis: MotionManager::axisList()) {
        disconnect(axis, &Axis::statusChanged, this, &ScanPathDiskEdit::refreshOffset);
    }

    Axis *axisLp = liftingAxis();
    Axis *scanAxis = ui->scanAxis->currentAxis();
    if (scanAxis && axisLp) {
        connect(axisLp, &Axis::statusChanged, this, &ScanPathDiskEdit::refreshOffset);
        connect(scanAxis, &Axis::statusChanged, this, &ScanPathDiskEdit::refreshOffset);
    }
}

void ScanPathDiskEdit::on_useSuggestedSpeed_clicked()
{
    ui->angularSpeed->setValue(suggestedSpeed());
}

qreal ScanPathDiskEdit::suggestedSpeed() const
{
    Axis *axis = ui->scanAxis->currentAxis();
    if (!axis) {
        return 0;
    }
    const QList<UTChannel *> channels = UltrasonicManager::channels();
    if (channels.isEmpty()) {
        return 0;
    }
    UTSetup channelSetup = channels.first()->setup();
    const qreal prf = channelSetup.prf();
    qreal average = channelSetup.sampleAverage();

    int currentIndex = ui->channel->currentIndex();
    if (currentIndex != 0) {                                     //采样平均次数与当前通道有关
        UTSetup channelSetupSecond = channels.value(currentIndex)->setup();
        average = channelSetupSecond.sampleAverage();
    }

    const QVector3D offset = axis->baseOffset();
    const QVector3D direction = axis->baseDirection();
    const QVector3D start = ui->startPoint->nativePos();
    const QVector3D end = ui->endPoint->nativePos();
    const qreal radius = qMax(offset.distanceToLine(start, direction), offset.distanceToLine(end, direction));
    const qreal interval = ui->interval->value();
    const qreal scale = platform()->Get("AngularSpeedScale").toDouble(0.9);
    const qreal speedMax = axis->config().speedMax;
    ui->angularSpeed->setMaximum(speedMax);

    qreal angularSpeed = 360 * (prf * interval * scale / average) / (2 * M_PI * radius);
    angularSpeed = qMin(angularSpeed, axis->config().speedMax);
    if (!qIsFinite(angularSpeed) || qFuzzyIsNull(angularSpeed) || radius < 0) {
        return axis->setup().speedLocate;
    }

    return angularSpeed;
}

void ScanPathDiskEdit::on_axisRotationEnable_toggled(bool checked)
{
    ui->startPoint->setAxisRotationEnable(checked);
    ui->endPoint->setAxisRotationEnable(checked);
}

void ScanPathDiskEdit::on_axesRotationLimited_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QUuid axisId = ui->axesRotationLimited->currentAxisUuid();
    ui->startPoint->setAxisRotationId(axisId);
    ui->endPoint->setAxisRotationId(axisId);
}

void ScanPathDiskEdit::on_angularSpeed_valueChanged(double arg1)
{
    const qreal speed = suggestedSpeed();
    ui->angularSpeed->setStyleSheet(arg1 - speed > 1e-2 ? "color: #FF0000" : QString());
}
