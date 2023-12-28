#include "DialogMotionPose.h"
#include "ui_DialogMotionPose.h"

#include <Motion/Axis.h>
#include <QLabel>
#include <QDoubleSpinBox>
#include <Motion/MotionManager.h>

DialogMotionPose::DialogMotionPose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMotionPose)
{
    ui->setupUi(this);
    InitUI();
}

DialogMotionPose::~DialogMotionPose()
{
    delete ui;
}

void DialogMotionPose::InitUI()
{
    for (Axis *axis: MotionManager::axisList()) {
        QLabel *label = new QLabel(this);
        auto *spinBox = new QDoubleSpinBox(this);
        label->setText(axis->name());
        spinBox->setSuffix(axis->axisUnit());
        spinBox->setRange(axis->config().limitReverse, axis->config().limitForward);
        ui->axesLayout->insertRow(ui->axesLayout->rowCount() - 1, label, spinBox);
    }
}

MotionPose DialogMotionPose::GetMostionPose()
{
    MotionPose pos;
    for (int index = 0; index < ui->axesLayout->rowCount(); ++index) {
        auto *spinBox = qobject_cast<QDoubleSpinBox *>(ui->axesLayout->itemAt(index, QFormLayout::FieldRole)->widget());
        if (spinBox) {
            pos << spinBox->value();
        }
    }
    return pos;
}

void DialogMotionPose::SetMotionPose(const MotionPose &pos, const MotionAxisList &axes)
{
    for (int index = 0; index < ui->axesLayout->rowCount(); ++index) {
        auto *spinBox = qobject_cast<QDoubleSpinBox *>(ui->axesLayout->itemAt(axes.toList().value(index), QFormLayout::FieldRole)->widget());
        if (spinBox) {
            spinBox->setValue(pos.value(index));
        }
    }
}
