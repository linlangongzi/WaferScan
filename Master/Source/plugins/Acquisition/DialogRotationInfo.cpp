#include "DialogRotationInfo.h"
#include "ui_DialogRotationInfo.h"
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>


DialogRotationInfo::DialogRotationInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogRotationInfo)
{
    ui->setupUi(this);
    InitUI();
}

DialogRotationInfo::~DialogRotationInfo()
{
    delete ui;
}

void DialogRotationInfo::InitUI()
{
    foreach (Axis *axis, MotionManager::axisList())
    {
        ui->axis->addItem(axis->name(), axis->axisId());
    }
    ui->type->addItem(tr("无"), QVariant::fromValue(RotationInfo::None));
    ui->type->addItem(tr("装夹"), QVariant::fromValue(RotationInfo::AxisContact));
    ui->type->addItem(tr("承载"), QVariant::fromValue(RotationInfo::SurfaceContact));
    ui->direction->addItem(tr("正向旋转"), QVariant::fromValue(RotationInfo::Forward));
    ui->direction->addItem(tr("反向旋转"), QVariant::fromValue(RotationInfo::Reverse));
    ui->axisDiameter->setRange(0, 99999.99);
    ui->partDiameter->setRange(0, 99999.99);
    ui->speedType->addItem(tr("匀速"), QVariant::fromValue(RotationInfo::Uniform));
    ui->speedType->addItem(tr("角速度限制"), QVariant::fromValue(RotationInfo::LimitAngularSpeed));
    ui->maxAngularSpeed->setRange(0, 99999.99);
}

void DialogRotationInfo::SetRotationInfo(const RotationInfo &rotation)
{
    for (int index = 0; index < ui->axis->count(); ++index)
    {
        if (ui->axis->itemData(index).toInt() == rotation.axis)
        {
            ui->axis->setCurrentIndex(index);
            break;
        }
    }
    switch (rotation.type)
    {
    case RotationInfo::None:
        ui->type->setCurrentIndex(0);
        break;
    case RotationInfo::AxisContact:
        ui->type->setCurrentIndex(1);
        break;
    case RotationInfo::SurfaceContact:
        ui->type->setCurrentIndex(2);
        break;
    default:
        break;
    }
    switch (rotation.direction)
    {
    case RotationInfo::Forward:
        ui->direction->setCurrentIndex(0);
        break;
    case RotationInfo::Reverse:
        ui->direction->setCurrentIndex(1);
        break;
    default:
        break;
    }
    ui->axisDiameter->setValue(rotation.axisDiameter);
    ui->partDiameter->setValue(rotation.partDiameter);
    switch (rotation.speedType)
    {
    case RotationInfo::Uniform:
        ui->speedType->setCurrentIndex(0);
        break;
    case RotationInfo::LimitAngularSpeed:
        ui->speedType->setCurrentIndex(1);
        break;
    default:
        break;
    }
    ui->maxAngularSpeed->setValue(rotation.maxAngularSpeed);
}

RotationInfo DialogRotationInfo::GetRotationInfo() const
{
    RotationInfo rotation;
    rotation.axis = ui->axis->currentData().toInt();
    rotation.type = ui->type->currentData().value<RotationInfo::Type>();
    rotation.direction = ui->type->currentData().value<RotationInfo::DirectionType>();
    rotation.axisDiameter = ui->axisDiameter->value();
    rotation.partDiameter = ui->partDiameter->value();
    rotation.speedType = ui->speedType->currentData().value<RotationInfo::SpeedType>();
    rotation.maxAngularSpeed = ui->maxAngularSpeed->value();
    return rotation;
}

