#include "DialogMotionCondition.h"
#include "ui_DialogMotionCondition.h"
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <QtMath>
#include <allrising/Util.h>

DialogMotionCondition::DialogMotionCondition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMotionCondition)
{
    ui->setupUi(this);
    InitUI();
}

DialogMotionCondition::~DialogMotionCondition()
{
    delete ui;
}

void DialogMotionCondition::InitUI()
{
    ui->conditionType->clear();
    ui->conditionType->addItem(tr("无"), EnumToKey(MotionCondition::ConditionNone));
    ui->conditionType->addItem(tr("延时步进"), EnumToKey(MotionCondition::ConditionDelay));
    ui->conditionType->addItem(tr("上升沿步进"), EnumToKey(MotionCondition::ConditionIoRising));
    ui->conditionType->addItem(tr("下降沿步进"), EnumToKey(MotionCondition::ConditionIoFalling));
    ui->conditionType->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    foreach (Axis *axis, MotionManager::axisList()) {
        QMap<QString, int> map = axis->GetSensorMap();
        auto iter = map.begin();
        while (iter != map.end()) {
            QString name = tr("%1-%2(%3)").arg(axis->name(), iter.key()).arg(iter.value());
            ui->sensor->addItem(name, iter.value());
            iter++;
        }
    }
    if (ui->sensor->count() == 0) {
        ui->sensor->addItem(tr("无"), -1);
    }

    ui->delayTime->setSuffix(" ms");
    ui->delayTime->setRange(0, 99999);

    connect(ui->conditionType, &QComboBox::currentTextChanged, this, &DialogMotionCondition::OnConditionTypeChanged);
}

void DialogMotionCondition::SetCondition(const MotionCondition &condition)
{
    for (int index = 0; index < ui->conditionType->count(); ++index) {
        if (ui->conditionType->itemData(index).toString() == EnumToKey(condition.type)) {
            ui->conditionType->setCurrentIndex(index);
            break;
        }
    }
    switch (condition.type) {
    case MotionCondition::ConditionNone:
        ui->labelValue->setVisible(false);
        ui->delayTime->setVisible(false);
        ui->sensor->setVisible(false);
        break;
    case MotionCondition::ConditionDelay:
        ui->labelValue->setVisible(true);
        ui->delayTime->setVisible(true);
        ui->sensor->setVisible(false);
        ui->labelValue->setText(tr("延时时间"));
        ui->delayTime->setValue(qFloor(condition.arg));
        break;
    case MotionCondition::ConditionIoFalling:
    case MotionCondition::ConditionIoRising:
        ui->labelValue->setVisible(true);
        ui->delayTime->setVisible(false);
        ui->sensor->setVisible(true);
        ui->labelValue->setText(tr("传感器"));
        for (int index = 0; index < ui->sensor->count(); ++index)
        {
            if (ui->sensor->itemData(index).toInt() == qFloor(condition.arg))
            {
                ui->sensor->setCurrentIndex(index);
                break;
            }
        }
        break;
    default :
        break;
    }
}

MotionCondition DialogMotionCondition::GetCondition() const
{
    MotionCondition condition;
    condition.type = KeyToEnum(ui->conditionType->currentData().toString(), MotionCondition::ConditionNone);
    switch (condition.type) {
    case MotionCondition::ConditionDelay:
        condition.arg = ui->delayTime->value();
        break;
    case MotionCondition::ConditionIoFalling:
    case MotionCondition::ConditionIoRising:
        condition.arg = ui->sensor->currentData().toInt();
        break;
    default:
        condition.arg = 0;
        break;
    }
    return condition;
}

void DialogMotionCondition::OnConditionTypeChanged()
{
    MotionCondition::Type type = KeyToEnum(ui->conditionType->currentData().toString(), MotionCondition::ConditionNone);
    switch (type) {
    case MotionCondition::ConditionNone:
        ui->labelValue->setVisible(false);
        ui->delayTime->setValue(false);
        ui->sensor->setVisible(false);
        break;
    case MotionCondition::ConditionDelay:
        ui->labelValue->setVisible(true);
        ui->labelValue->setText(tr("延时时间"));
        ui->delayTime->setVisible(true);
        ui->sensor->setVisible(false);
        break;
    case MotionCondition::ConditionIoFalling:
    case MotionCondition::ConditionIoRising:
        ui->labelValue->setVisible(true);
        ui->labelValue->setText(tr("传感器"));
        ui->sensor->setVisible(true);
        break;
    default:
        break;
    }
}
