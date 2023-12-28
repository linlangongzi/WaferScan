#include "DialogMotionElement.h"
#include "ui_DialogMotionElement.h"
#include <Motion/Axis.h>

#include <allrising/Util.h>
#include "DialogMotionPose.h"
#include "DialogMotionCondition.h"
#include <QCheckBox>
#include <QMessageBox>
#include <QtMath>
#include <Motion/MotionManager.h>

DialogMotionElement::DialogMotionElement(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMotionElement)
{
    ui->setupUi(this);
    InitUI();
}

DialogMotionElement::~DialogMotionElement()
{
    delete ui;
}

void DialogMotionElement::InitUI()
{
    connect(ui->addPos, &QToolButton::clicked, this, &DialogMotionElement::OnAddPos);
    connect(ui->actionAddPos, &QAction::triggered, this, &DialogMotionElement::OnAddPos);
    connect(ui->editPos, &QToolButton::clicked, this, &DialogMotionElement::OnEditPos);
    connect(ui->actionEditPos, &QAction::triggered, this, &DialogMotionElement::OnEditPos);
    connect(ui->deletePos, &QToolButton::clicked, this, &DialogMotionElement::OnDeletePos);
    connect(ui->actionDeletePos, &QAction::triggered, this, &DialogMotionElement::OnDeletePos);
    connect(ui->addCondition, &QToolButton::clicked, this, &DialogMotionElement::OnAddCondition);
    connect(ui->actionAddCondition, &QAction::triggered, this, &DialogMotionElement::OnAddCondition);
    connect(ui->editCondition, &QToolButton::clicked, this, &DialogMotionElement::OnEditCondition);
    connect(ui->actionEditCondition, &QAction::triggered, this, &DialogMotionElement::OnEditCondition);
    connect(ui->deleteCondition, &QToolButton::triggered, this, &DialogMotionElement::OnDeleteCondition);
    connect(ui->actionDeleteCondition, &QAction::triggered, this, &DialogMotionElement::OnDeleteCondition);
    ui->posList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->posList, &QListWidget::customContextMenuRequested, this, &DialogMotionElement::OnPosContextMenuRequested);
    ui->conditionList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->conditionList, &QListWidget::customContextMenuRequested, this, &DialogMotionElement::OnConditionContextMenuRequested);
    QList<QAction *> actions;
    actions << ui->actionAddPos << ui->actionEditPos << ui->actionDeletePos;
    posMenu.addActions(actions);
    actions.clear();
    actions << ui->actionAddCondition << ui->actionEditCondition << ui->actionDeleteCondition;
    conditionMenu.addActions(actions);
    // 加载轴列表
    for (int index = 0; index < MotionManager::axisCount(); ++index) {
        Axis *axis = MotionManager::axis(index);
        if (axis) {
            int row = index / 2;
            int column = index % 2;
            auto *box = new QCheckBox(this);
            box->setText(axis->name());
            ui->axesLayout->addWidget(box, row, column);
        }
    }
    // 初始化运动类型
    ui->motionType->clear();
    ui->motionType->addItem(tr("无"), EnumToKey(MotionElement::None));
    ui->motionType->addItem(tr("停止"), EnumToKey(MotionElement::Stop));
    ui->motionType->addItem(tr("复位"), EnumToKey(MotionElement::Home));
    ui->motionType->addItem(tr("定位"), EnumToKey(MotionElement::Locate));
    ui->motionType->addItem(tr("直线平移"), EnumToKey(MotionElement::MoveLine));
    ui->motionType->addItem(tr("相对移动"), EnumToKey(MotionElement::MoveRel));
    ui->motionType->addItem(tr("旋转"), EnumToKey(MotionElement::Continuous));
    ui->motionType->addItem(tr("自由曲线"), EnumToKey(MotionElement::Cam));
    // 初始化插值方式
    ui->Interpolate->clear();
    ui->Interpolate->addItem(tr("无"), EnumToKey(MotionElement::InterpolateNone));
    ui->Interpolate->addItem(tr("单向插值"), EnumToKey(MotionElement::InterpolateUnidirectional));
    ui->Interpolate->addItem(tr("双向插值"), EnumToKey(MotionElement::InterpolateBidiretional));
}

/*! 设置MotionElement更新界面 */
void DialogMotionElement::SetMotionElement(const MotionElement &element, const QList<RotationInfo> &rotations)
{
    motionElement = element;
    int typeIndex = ui->motionType->findData(QVariant(EnumToKey(element.type)), Qt::UserRole);
    ui->motionType->setCurrentIndex(typeIndex);
    QList<int> axes = element.axis.toList();
    for (int index = 0; index < ui->axesLayout->count(); ++index) {
        auto *box = static_cast<QCheckBox *>(ui->axesLayout->itemAt(index)->widget());
        if (box) {
            box->setChecked(axes.contains(index));
        }
    }
//    Axis *axis = MotionManager::axis(element.stepAxis);
    RotationInfo stepRotation = RotationInfo::byAxisIndex(rotations, element.stepAxis);
    RotationInfo scanRotation = RotationInfo::byAxisIndex(rotations, element.mainAxis);
    ui->stepAxis->setCurrentIndex(element.stepAxis);
    ui->stepInterval->setValue( stepRotation.surfaceSpeed( element.stepInterval ));
    ui->scanAxis->setCurrentIndex(element.mainAxis);
    ui->speed->setValue( scanRotation.surfaceSpeed( element.speed ));
    int interPolicyIndex = ui->Interpolate->findData(QVariant(EnumToKey(element.interPolicy)), Qt::UserRole);
    ui->Interpolate->setCurrentIndex(interPolicyIndex);
    ui->posList->clear();
    foreach (const MotionPose &pose, element.pos) {
        auto *item = new QListWidgetItem(ui->posList);
        item->setText(pose.toString());
        item->setData(Qt::UserRole, QVariant::fromValue(pose));
    }
    ui->conditionList->clear();
    foreach (const MotionCondition &condition, element.conditions) {
        RefreshCondition(condition, new QListWidgetItem(ui->conditionList));
    }
}

/*! 由界面获取MotionElement */
MotionElement DialogMotionElement::GetMotionElement(const QList<RotationInfo> &rotations)
{
    MotionElement element;
    // 运动类型
    element.type = KeyToEnum(ui->motionType->currentData().toString(), MotionElement::None);
    // 获取轴列表
    MotionAxisList axisList;
    for (int index = 0; index < ui->axesLayout->count(); ++index) {
        auto *box = static_cast<QCheckBox *>(ui->axesLayout->itemAt(index)->widget());
        if (box) {
            axisList.setAxis(index, box->isChecked());
        }
    }
    element.axis = axisList;
    RotationInfo stepRotation = RotationInfo::byAxisIndex(rotations, ui->stepAxis->currentAxisIndex());
    RotationInfo scanRotation = RotationInfo::byAxisIndex(rotations, ui->scanAxis->currentAxisIndex());
    // 获取
    element.stepAxis = ui->stepAxis->currentAxisIndex();
    element.stepInterval = stepRotation.nativeSpeed(ui->stepInterval->value());
    element.mainAxis = ui->scanAxis->currentAxisIndex();
    element.speed = scanRotation.nativeSpeed(ui->speed->value());
    element.interPolicy = KeyToEnum(ui->Interpolate->currentData().toString(), MotionElement::InterpolateNone);
    // 特征点
    QList<MotionPose> motionPoseList;
    for (int index = 0; index < ui->posList->count(); ++index) {
        motionPoseList.append(ui->posList->item(index)->data(Qt::UserRole).value<MotionPose>());
    }
    element.pos = motionPoseList;
    // condition;
    for (int index = 0; index < ui->conditionList->count(); ++index) {
        element.conditions.append(ui->conditionList->item(index)->data(Qt::UserRole).value<MotionCondition>());
    }
    return element;
}

/*! 添加MotionPos */
void DialogMotionElement::OnAddPos()
{
    DialogMotionPose dialogPos;
    if (dialogPos.exec() == QDialog::Accepted) {
        MotionPose pos = dialogPos.GetMostionPose();
        auto *item = new QListWidgetItem(ui->posList);
        item->setText(pos.toString());
        item->setData(Qt::UserRole, QVariant::fromValue(pos));
    }
}

/*! 编辑MotionPos */
void DialogMotionElement::OnEditPos()
{
    QListWidgetItem *item = ui->posList->selectedItems().value(0);
    if (!item) {
        return;
    }
    MotionPose pos = item->data(Qt::UserRole).value<MotionPose>();
    DialogMotionPose dialogPos;
    dialogPos.SetMotionPose(pos, motionElement.axis);
    if (dialogPos.exec() == QDialog::Accepted) {
        MotionPose newPos = dialogPos.GetMostionPose();
        item->setText(newPos.toString());
        item->setData(Qt::UserRole, QVariant::fromValue(newPos));
    }
}

/*! 删除MotionPos */
void DialogMotionElement::OnDeletePos()
{
    QList<QListWidgetItem *> itemList = ui->posList->selectedItems();
    if (itemList.count() <= 0) {
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定删除当前选中点吗"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        qDeleteAll(itemList);
    }
}

/*! 列表的右键菜单请求 */
void DialogMotionElement::OnPosContextMenuRequested()
{
    QList<QListWidgetItem *> itemList = ui->posList->selectedItems();
    ui->actionEditPos->setEnabled(!itemList.isEmpty());
    ui->actionDeletePos->setEnabled(!itemList.isEmpty());
    posMenu.exec(QCursor::pos());
}

void DialogMotionElement::RefreshCondition(const MotionCondition &condition, QListWidgetItem *item)
{
    if (!item) {
        return;
    }
    QString name;
    switch (condition.type) {
    case MotionCondition::ConditionNone:
        name = tr("无");
        break;
    case MotionCondition::ConditionDelay:
        name = tr("延时步进(%1)").arg(qFloor(condition.arg));
        break;
    case MotionCondition::ConditionIoFalling:
        name = tr("下降沿触发(%1)").arg(qFloor(condition.arg));
        break;
    case MotionCondition::ConditionIoRising:
        name = tr("上升沿触发(%1)").arg(qFloor(condition.arg));
        break;
    default:
        break;
    }
    item->setText(name);
    item->setData(Qt::UserRole, QVariant::fromValue(condition));
}

void DialogMotionElement::OnAddCondition()
{
    DialogMotionCondition dialog;
    dialog.SetCondition(MotionCondition());
    if (dialog.exec() == QDialog::Accepted) {
        RefreshCondition(dialog.GetCondition(), new QListWidgetItem(ui->conditionList));
    }
}

void DialogMotionElement::OnEditCondition()
{
    QListWidgetItem *item = ui->conditionList->selectedItems().value(0);
    if (!item) {
        return;
    }
    MotionCondition condition = item->data(Qt::UserRole).value<MotionCondition>();
    DialogMotionCondition dialog;
    dialog.SetCondition(condition);
    if (dialog.exec() == QDialog::Accepted) {
        RefreshCondition(dialog.GetCondition(), item);
    }
}

void DialogMotionElement::OnDeleteCondition()
{
    QList<QListWidgetItem *> itemList = ui->conditionList->selectedItems();
    if (itemList.count() <= 0) {
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定删除当前选中控制条件吗"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        qDeleteAll(itemList);
    }
}

void DialogMotionElement::OnConditionContextMenuRequested()
{
    QList<QListWidgetItem *> itemList = ui->conditionList->selectedItems();
    ui->actionEditCondition->setEnabled(!itemList.isEmpty());
    ui->actionDeleteCondition->setEnabled(!itemList.isEmpty());
    conditionMenu.exec(QCursor::pos());
}
