#include "PointXYZ.h"
#include "ui_PointXYZ.h"
#include <Core/Platform.h>
#include <Motion/Axis.h>
#include <Motion/DialogNamedPositionList.h>
#include <Motion/EndPoint.h>
#include <QFormLayout>
#include <QMessageBox>
#include <QVector3D>
#include <Ultrasonic/UTPose.h>

PointXYZ::PointXYZ(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointXYZ)
{
    ui->setupUi(this);
    ui->endPointFromTable->setAutoDefault(false);
    ui->toEndpoint->setAutoDefault(false);
    ui->getEndpoint->setAutoDefault(false);

    QList<QLabel *> axes = QList<QLabel *>() << ui->titleX << ui->titleY << ui->titleZ;
    for (int index = 0; index < 3; ++index) {
        Axis *axis = MotionManager::axis(index);
        QLabel *label = axes.value(index);
        if (axis && label) {
            label->setText(axis->name());
        }
    }

    setAxisRotationEnable(false);
}

PointXYZ::~PointXYZ()
{
    delete ui;
}

qreal PointXYZ::angle() const
{
    return ui->posRotation->value();
}

void PointXYZ::setAngle(qreal angle)
{
    ui->posRotation->setValue(angle);
}

QUuid PointXYZ::axisRotationId() const
{
    if (m_axisRotationEnable) {
        return m_axisRotationId;
    }
    return QUuid();
}

void PointXYZ::setAxisRotationId(const QUuid &id)
{
    if (m_axisRotationId == id) {
        return;
    }
    m_axisRotationId = id;
    ui->axisRotationTitle->setText(MotionManager::axis(id)->name());
}

/*!
 * \brief PointXYZ::nativePos
 * \return 控件中显示的位置 + 偏移量
 */
QVector3D PointXYZ::nativePos() const
{
    return (displayPos() + m_offset);
}

void PointXYZ::setDisplayPos(const QVector3D &pos)
{
    QVector3D prevNativePos(displayPos());
    QSignalBlocker blocker(this);
    ui->posX->setValue(pos.x());
    ui->posY->setValue(pos.y());
    ui->posZ->setValue(pos.z());
    blocker.unblock();
    if (!qFuzzyCompare(prevNativePos, pos)) {
        emit pointChanged(pos);
    }
}

void PointXYZ::setDisplayPos(double x, double y, double z)
{
    QVector3D prevNativePos(displayPos());
    QSignalBlocker blocker(this);
    ui->posX->setValue(x);
    ui->posY->setValue(y);
    ui->posZ->setValue(z);
    blocker.unblock();
    if (!qFuzzyCompare(prevNativePos, QVector3D(x, y, z))) {
        emit pointChanged(QVector3D(x, y, z));
    }
}

void PointXYZ::setNativePos(const QVector3D &pos)
{
    QVector3D display = pos - m_offset;
    QVector3D prevNativePos(nativePos());
    QSignalBlocker blocker(this);
    ui->posX->setValue(display.x());
    ui->posY->setValue(display.y());
    ui->posZ->setValue(display.z());
    blocker.unblock();
    if (!qFuzzyCompare(pos, prevNativePos)) {
        emit pointChanged(pos);
    }
}

/*!
 * \brief PointXYZ::pos
 * \return 控件中显示的位置
 */
QVector3D PointXYZ::displayPos() const
{
    return QVector3D(ui->posX->value(), ui->posY->value(), ui->posZ->value());
}

void PointXYZ::setOffset(const QVector3D offset)
{
    if (m_offset == offset) {
        return;
    }

    m_offset = offset;
}

void PointXYZ::setOffsetAndRefresh(const QVector3D offset)
{
    if (m_offset == offset) {
        return;
    }

    const QVector3D native = nativePos();
    m_offset = offset;
    setNativePos(native);
}

void PointXYZ::setAxisRotationEnable(bool enable)
{
    m_axisRotationEnable = enable;
    ui->posRotation->setVisible(enable);
    ui->axisRotationTitle->setVisible(enable);
    ui->posRotation->setEnabled(enable);
    ui->axisRotationTitle->setEnabled(enable);
}

void PointXYZ::on_endPointFromTable_clicked()
{
    DialogNamedPositionList dialog;
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QVector3D native(dialog.coordinate(0), dialog.coordinate(1), dialog.coordinate(2));
    setNativePos(native);
}

void PointXYZ::on_getEndpoint_clicked()
{
    EndPoint *endpoint = MotionManager::endPoint(0);
    if (endpoint) {
        MotionPose pose = endpoint->currentPos();
        QVector3D native(pose.value(0), pose.value(1), pose.value(2));
        setNativePos(native);

        const int axisIndex = MotionManager::axisIndex(m_axisRotationId);
        if (axisIndex < 0) {
            return;
        }
        ui->posRotation->setValue(pose.value(axisIndex));
    }
}

void PointXYZ::on_toEndpoint_clicked()
{
    EndPoint *endpoint = MotionManager::endPoint(0);
    if (!endpoint) {
        return;
    }

    QVector<Axis *> axes({MotionManager::axis(0), MotionManager::axis(1), MotionManager::axis(2)});
    Axis *axisRotation = MotionManager::axis(m_axisRotationId);
    if (m_axisRotationEnable) {
        axes << axisRotation;
    }

    for (int index = 0; index < axes.count(); ++index) {
        Axis *axis = axes[index];
        if (!axis) {
            QMessageBox::warning(this, QApplication::applicationName(), tr("含有无效轴 %1").arg(index));
            return;
        }
        if (axis->setup().disabled) {
            QMessageBox::warning(this, QApplication::applicationName(), tr("含有被禁用的轴 %1").arg(index));
            return;
        }
    }

    QHash<QUuid, qreal> pose;
    const QVector3D pos = nativePos();
    pose[axes[0]->uuid()] = pos.x();
    pose[axes[1]->uuid()] = pos.y();
    pose[axes[2]->uuid()] = pos.z();

    if (m_axisRotationEnable) {
        pose[m_axisRotationId] = ui->posRotation->value();
    }

    endpoint->Locate(pose);
}

void PointXYZ::onValueChanged()
{
    emit pointChanged(nativePos());
}
