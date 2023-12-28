#include "ScanPathAdvancedEdit.h"
#include "ui_ScanPathAdvancedEdit.h"
#include "ScanPlan.h"
#include <Acquisition/DialogMotionCondition.h>
#include <Acquisition/DialogMotionElement.h>
#include <Acquisition/DialogMotionPose.h>
#include <Acquisition/DialogRotationInfo.h>
#include <Core/LicenseManager.h>
#include <Core/Platform.h>
#include <Imaging/DialogImageOptions.h>
#include <Imaging/UTDocument.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <QDebug>
#include <QHeaderView>
#include <QInputDialog>
#include <QListWidget>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QtMath>
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <Waveform/DialogUTSetup.h>
#include <Waveform/PanelUTSetup.h>
#include <allrising/GeneralDelegate.h>
#include <allrising/Util.h>
#include <utils/algorithm.h>

static Q_LOGGING_CATEGORY(debug, "acquire.adv.edit");

enum {
    DelegateRole    = Qt::UserRole + 1000,
    ValueTypeRole   = Qt::UserRole + 1001,
    ValueDataRole   = Qt::UserRole + 1002,
    SuffixRole      = Qt::UserRole + 1003
};

enum {
    ElementWidgetItemType = QTreeWidgetItem::UserType + 1,
    RotationWidgetItemType = QTreeWidgetItem::UserType + 2,
    RotationPropertyWidgetItemType = QTreeWidgetItem::UserType + 3,
    PropertyWidgetItemType = QTreeWidgetItem::UserType + 4,
    PoseWidgetItemType = QTreeWidgetItem::UserType + 5,
    AxesWidgetItemType = QTreeWidgetItem::UserType + 6,
    ConditionWidgetItemType = QTreeWidgetItem::UserType + 7
};

static QMap<QVariant, QVariant> elementPropertyMap;
static QMap<QVariant, QVariant> elementTypeMap;
static QMap<QVariant, QVariant> interpolateTypeMap;
static QMap<QVariant, QVariant> axisListMap;
static QMap<QVariant, QVariant> conditionTypeMap;
static QMap<QVariant, QVariant> rotationPropertyMap;
static QMap<QVariant, QVariant> directionMap;
static QMap<QVariant, QVariant> rotationTypeMap;
static QMap<QVariant, QVariant> speedTypeMap;

QWidget *ChannelDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        auto *box = new QComboBox(parent);
        box->addItem(tr("无"), -1);
        for (auto *channel: UltrasonicManager::channels()) {
            box->addItem(channel->name(), channel->channelId());
        }
        return box;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void ChannelDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto *box = qobject_cast<QComboBox *>(editor);
    if (box) {
        int channelIndex = index.data(Qt::UserRole).toInt();
        box->setCurrentIndex(box->findData(channelIndex));
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void ChannelDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto *box = qobject_cast<QComboBox *>(editor);
    if (box) {
        model->setData(index, box->currentData(), Qt::UserRole);                /* setData(UserRole) 会导致 setEditorData 被调用, 所以需要先于 DisplayRole 调用 */
        model->setData(index, box->currentText(), Qt::DisplayRole);
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void ChannelDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}

WidgetItemObject::WidgetItemObject(QTreeWidgetItem *parent, int type) :
    QTreeWidgetItem(parent, type)
{
    setFlags(flags() | Qt::ItemIsEditable);
}

void WidgetItemObject::setData(int column, int role, const QVariant &value)
{
    QVariant oldValue = data(column, role);
    if (oldValue != value) {
        QTreeWidgetItem::setData(column, role, value);
        if (role == ValueDataRole) {
            emit DataChanged(value, oldValue);
        }
    }
}

class PropertyWidgetItem : public WidgetItemObject
{
    Q_DECLARE_TR_FUNCTIONS(PropertyWidgetItem)
public:
    PropertyWidgetItem(const QString &property, const QVariant &value, QTreeWidgetItem *parent, int type) :
        WidgetItemObject(parent, type),
        propertyName(property)
    {
        setFlags(flags() | Qt::ItemIsEditable);
        if (type == PropertyWidgetItemType) {
            RefreshElementProperty(property, value);
        } else if (type == RotationPropertyWidgetItemType) {
            RefreshRotationProperty(property, value);
        }
    }

    QString GetPropertyName() const {return propertyName;}

    void SetProperty(MotionElement *elemnt, const QMetaProperty &property, const QList<RotationInfo> &rotations)
    {
        QString propertyName = property.name();
        if (propertyName == "type") {
            property.writeOnGadget(elemnt, data(1, ValueDataRole).value<MotionElement::Type>());
        } else if (propertyName == "interPolicy") {
            property.writeOnGadget(elemnt, data(1, ValueDataRole).value<MotionElement::InterpolatePolicy>());
        } else if (propertyName == "mainAxis" || propertyName == "stepAxis") {
            property.writeOnGadget(elemnt, data(1, ValueDataRole).toInt());
        } else if (propertyName == "stepInterval" || propertyName == "stepSpeed") {
            RotationInfo rotation = RotationInfo::byAxisIndex(rotations, elemnt->stepAxis);
            property.writeOnGadget(elemnt, rotation.nativeSpeed(data(1, ValueDataRole).toReal()));
        } else if (propertyName == "speed") {
            RotationInfo rotation = RotationInfo::byAxisIndex(rotations, elemnt->mainAxis);
            property.writeOnGadget(elemnt, rotation.nativeSpeed(data(1, ValueDataRole).toReal()));
        } else {
            property.writeOnGadget(elemnt, data(1, ValueDataRole).toReal());
        }
    }

    void SetProperty(RotationInfo *rotation, const QMetaProperty &property)
    {
         QString propertyName = property.name();
         if (propertyName == "axis") {
            property.writeOnGadget(rotation, data(1, ValueDataRole).toInt());
         } else if (propertyName == "direction") {
            property.writeOnGadget(rotation, data(1, ValueDataRole).value<RotationInfo::DirectionType>());
         } else if (propertyName == "type") {
            property.writeOnGadget(rotation, data(1, ValueDataRole).value<RotationInfo::Type>());
         } else if (propertyName == "speedType") {
            property.writeOnGadget(rotation, data(1, ValueDataRole).value<RotationInfo::SpeedType>());
         } else if (propertyName == "axisDiameter" || propertyName == "partDiameter" || propertyName == "maxAngularSpeed" || propertyName == "minAngularSpeed") {
             property.writeOnGadget(rotation, data(1, ValueDataRole).toReal());
         }
    }

private:
    void RefreshElementProperty(const QString &propertyName, const QVariant &value)
    {
        if (elementPropertyMap.isEmpty()) {
            elementPropertyMap["type"] = tr("运动方式");
            elementPropertyMap["interPolicy"] = tr("插值方式");
            elementPropertyMap["mainAxis"] = tr("主扫描轴");
            elementPropertyMap["stepAxis"] = tr("步进轴");
            elementPropertyMap["stepInterval"] = tr("步进间隔");
            elementPropertyMap["speed"] = tr("扫描速度");
            elementPropertyMap["stepSpeed"] = tr("步进速度");
        }
        setText(0, elementPropertyMap.value(propertyName).toString());
        if (propertyName == "type") {
            setText(1, elementTypeMap[value].toString());
            setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::ElementType));
            setData(1, DelegateRole, ScanPathAdvancedEdit::Combobox);
        } else if (propertyName == "interPolicy") {
            setText(1, interpolateTypeMap[value].toString());
            setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::InterpolateType));
            setData(1, DelegateRole, ScanPathAdvancedEdit::Combobox);
        } else if (propertyName == "mainAxis" || propertyName == "stepAxis") {
            if (axisListMap.contains(value.toInt())) {
                setText(1, axisListMap.key(value).toString());
            } else {
                setText(1, tr("无"));
            }
            setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::AxisListType));
            setData(1, DelegateRole, ScanPathAdvancedEdit::Combobox);
        } else {
            setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::DoubleValueType));
            setData(1, DelegateRole, ScanPathAdvancedEdit::DoubleSpinBox);
            if (propertyName == "stepInterval") {
                setText(1, tr("%1 mm").arg(value.toReal()));
                setData(1, SuffixRole, tr(" mm"));
            } else if (propertyName == "speed" || propertyName == "stepSpeed") {
                setText(1, tr("%1 mm/s").arg(value.toReal()));
                setData(1, SuffixRole, tr(" mm/s"));
            }
        }
        setData(1, ValueDataRole, value);
    }

    void RefreshRotationProperty(const QString &propertyName, const QVariant &value)
    {
        if (rotationPropertyMap.isEmpty()) {
            rotationPropertyMap.insert("axis", tr("轴选择"));
            rotationPropertyMap.insert("type", tr("承载类型"));
            rotationPropertyMap.insert("direction", tr("旋转方向"));
            rotationPropertyMap.insert("axisDiameter", tr("轴直径"));
            rotationPropertyMap.insert("partDiameter", tr("工件直径"));
            rotationPropertyMap.insert("speedType", tr("速度选择"));
            rotationPropertyMap.insert("maxAngularSpeed", tr("最大角速度"));
            rotationPropertyMap.insert("minAngularSpeed", tr("最小角速度"));
        }
        setText(0, rotationPropertyMap.value(propertyName).toString());
        if (propertyName == "axis") {
            if (axisListMap.contains(value.toInt())) {
                setText(1, axisListMap.key(value).toString());
            } else {
                setText(1, tr("无"));
            }
            setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::AxisListType));
            setData(1, DelegateRole, ScanPathAdvancedEdit::Combobox);
        } else if (propertyName == "direction") {
            setText(1, directionMap.value(value).toString());
            setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::RotationDirectionType));
            setData(1, DelegateRole, ScanPathAdvancedEdit::Combobox);
        } else if (propertyName == "type") {
            setText(1, rotationTypeMap.value(value).toString());
            setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::RotationType));
            setData(1, DelegateRole, ScanPathAdvancedEdit::Combobox);
        } else if (propertyName == "axisDiameter" || propertyName == "partDiameter") {
            setText(1, QString("%1 mm").arg(value.toReal()));
            setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::DoubleValueType));
            setData(1, DelegateRole, ScanPathAdvancedEdit::DoubleSpinBox);
            setData(1, SuffixRole, tr(" mm"));
        } else if (propertyName == "speedType") {
            setText(1, speedTypeMap.value(value).toString());
            setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::SpeedType));
            setData(1, DelegateRole, ScanPathAdvancedEdit::Combobox);
        } else if (propertyName == "maxAngularSpeed" || propertyName == "minAngularSpeed") {
            setText(1, QString("%1 °/s").arg(value.toReal()));
            setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::DoubleValueType));
            setData(1, DelegateRole, ScanPathAdvancedEdit::DoubleSpinBox);
            setData(1, SuffixRole, tr(" °/s"));
        }
        setData(1, ValueDataRole, value);
    }

private:
    QString propertyName;
};

class PoseWidgetItem : public QTreeWidgetItem
{
    Q_DECLARE_TR_FUNCTIONS(PoseWidgetItem)
public:
    PoseWidgetItem(const MotionPose &pose, const MotionAxisList &axes, QTreeWidgetItem *parent = nullptr) :
        QTreeWidgetItem(parent, PoseWidgetItemType),
        axis(axes)
    {
        SetPose(pose, axes);
    }

    void SetPose(const MotionPose &pose, const MotionAxisList &axes)
    {
        qDeleteAll(takeChildren());
        bool isPoseEmpty = pose.count() <= 0;
        int count = isPoseEmpty ? MotionManager::axisCount() : qMin(MotionManager::axisCount(), pose.count());
        for (int index = 0; index < count; ++index) {
            Axis *axis = MotionManager::axis(axes.toList().value(index));
            if (axis) {
                auto *item = new QTreeWidgetItem(this);
                item->setFlags(flags() | Qt::ItemIsEditable);
                item->setText(0, axis->name());
                item->setText(1, tr("%1").arg(pose.value(index)) + tr("%1").arg(axis->axisUnit()));
                item->setData(1, DelegateRole, ScanPathAdvancedEdit::DoubleSpinBox);
                item->setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::DoubleValueType));
                item->setData(1, ValueDataRole, pose.value(index));
                item->setData(1, SuffixRole, axis->axisUnit());
            }
        }
        setText(0, tr("位置"));
        setExpanded(true);
    }

    MotionPose GetPose() const
    {
        MotionPose pos;
        for (int index = 0; index < childCount(); ++index) {
            pos << child(index)->data(1, ValueDataRole).toReal();
        }
        return pos;
    }

    MotionAxisList GetAxes() const
    {
        return axis;
    }

private:
    MotionAxisList axis;
};

class AxesWidgetItem : public QTreeWidgetItem
{
    Q_DECLARE_TR_FUNCTIONS(AxesWidgetItem)
public:
    AxesWidgetItem(const MotionAxisList &axes, QTreeWidgetItem *parent) :
        QTreeWidgetItem(parent, AxesWidgetItemType)
    {
        setText(0, tr("轴选择"));
        for (int index = 0; index < MotionManager::axisCount(); ++index) {
            Axis *axis = MotionManager::axis(index);
            if (!axis) {
                continue;
            }
            auto *item = new QTreeWidgetItem(this);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setText(0, axis->name());
            item->setData(0, Qt::UserRole, axis->config().axisId);
            Qt::CheckState state = axes.hasAxis(index) ? Qt::Checked : Qt::Unchecked;
            item->setCheckState(0, state);
        }
        setExpanded(true);
    }

    MotionAxisList GetMotionAxisList() const
    {
        MotionAxisList axes;
        for (int index = 0; index < childCount(); ++index) {
            QTreeWidgetItem *item = child(index);
            if (!item) {
                continue;
            }
            int axisIndex = item->data(0, Qt::UserRole).toInt();
            axes.setAxis(axisIndex, item->checkState(0) == Qt::Checked);
        }
        return axes;
    }
};

ConditionWidgetItem::ConditionWidgetItem(const MotionCondition &condition, QTreeWidgetItem *parent) :
    QTreeWidgetItem(parent, ConditionWidgetItemType)
{
    SetCondition(condition);
}

MotionCondition ConditionWidgetItem::GetMotionCondition()
{
    MotionCondition condition;
    for (int row = 0; row < childCount(); ++row) {
        switch (row) {
        case 0:
            condition.type = child(0)->data(1, ValueDataRole).value<MotionCondition::Type>();
            break;
        case 1:
            if (!child(1)->isHidden()) {
                condition.arg = child(1)->data(1, ValueDataRole).toReal();
            }
            break;
        case 2:
            if (!child(2)->isHidden()) {
                condition.arg = child(2)->data(1, ValueDataRole).toReal();
            }
            break;
        case 3:
            condition.axis = child(3)->data(1, ValueDataRole).toInt();
            break;
        case 4:
            condition.repCount = child(4)->data(1, ValueDataRole).toInt();
            break;
        default:
            break;
        }
    }
    return condition;
}

void ConditionWidgetItem::SetCondition(const MotionCondition &condition)
{
    qDeleteAll(takeChildren());
    setText(0, tr("步进条件"));
    auto *item = new WidgetItemObject(this, ConditionWidgetItemType);
    item->setText(0, tr("触发类型"));
    item->setText(1, conditionTypeMap[QVariant::fromValue(condition.type)].toString());
    item->setData(1, DelegateRole, ScanPathAdvancedEdit::Combobox);
    item->setData(1, ValueTypeRole, EnumToKey( ScanPathAdvancedEdit::ConditionType ));
    item->setData(1, ValueDataRole, QVariant::fromValue( condition.type ));
    connect(item, &WidgetItemObject::DataChanged, this, &ConditionWidgetItem::OnConditionTypeChanged);

    auto *sensorItem = new QTreeWidgetItem(this);
    sensorItem->setFlags(flags() | Qt::ItemIsEditable);
    sensorItem->setText(0, tr("传感器"));
    QString sensorText = tr("无");
    foreach (Axis *axis, MotionManager::axisList()) {
        QMap<QString, int> map = axis->GetSensorMap();
        auto sensorId = int(condition.arg);
        QList<QString> nameList = map.keys(sensorId);
        if (!nameList.isEmpty()) {
            QString name = nameList.value(0);
            sensorText = tr("%1-%2(%3)").arg(axis->name(), name).arg(sensorId);
            break;
        }
    }
    sensorItem->setText(1, sensorText);
    sensorItem->setData(1, DelegateRole, ScanPathAdvancedEdit::Combobox);
    sensorItem->setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::SensorType));
    sensorItem->setData(1, ValueDataRole, QVariant(int(condition.arg)));

    auto *delayItem = new QTreeWidgetItem(this);
    delayItem->setFlags(flags() | Qt::ItemIsEditable);
    delayItem->setText(0, tr("延时时间"));
    delayItem->setText(1, tr("%1 ms").arg(condition.arg));
    delayItem->setData(1, DelegateRole, ScanPathAdvancedEdit::DoubleSpinBox);
    delayItem->setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::DoubleValueType));
    delayItem->setData(1, ValueDataRole, condition.arg);
    delayItem->setData(1, SuffixRole, tr(" ms"));

    auto *axisItem = new QTreeWidgetItem(this);
    axisItem->setFlags(flags() | Qt::ItemIsEditable);
    axisItem->setText(0, tr("轴选择"));
    if (axisListMap.contains(condition.axis)) {
        axisItem->setText(1, axisListMap.key(condition.axis).toString());
    } else {
        axisItem->setText(1, tr("无"));
    }
    axisItem->setData(1, ValueDataRole, condition.axis);
    axisItem->setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::AxisListType));
    axisItem->setData(1, DelegateRole, ScanPathAdvancedEdit::Combobox);

    auto *repCountItem = new QTreeWidgetItem(this);
    repCountItem->setText(0, tr("重复次数"));
    repCountItem->setFlags(flags() | Qt::ItemIsEditable);
    repCountItem->setText(1, QString::number(condition.repCount));
    repCountItem->setData(1, ValueDataRole, condition.repCount);
    repCountItem->setData(1, ValueTypeRole, EnumToKey(ScanPathAdvancedEdit::IntValueType));
    repCountItem->setData(1, DelegateRole, ScanPathAdvancedEdit::SpinBox);

    RefreshCondition(condition);
    setExpanded(true);
}

void ConditionWidgetItem::OnConditionTypeChanged(const QVariant &newType, const QVariant &oldType)
{
    auto newCondtionType = newType.value<MotionCondition::Type>();
    auto oldConditionType = oldType.value<MotionCondition::Type>();
    if ((newCondtionType == MotionCondition::ConditionIoFalling || newCondtionType == MotionCondition::ConditionIoFalling) &&
            (oldConditionType == MotionCondition::ConditionIoFalling || oldConditionType == MotionCondition::ConditionIoFalling)) {
        return;
    }
    MotionCondition condition;
    condition.type = newCondtionType;
    QTreeWidgetItem *valueItem = child(1);
    if (valueItem) {
        condition.arg = valueItem->data(1, ValueDataRole).toReal();
    }
    RefreshCondition(condition);
}

void ConditionWidgetItem::RefreshCondition(const MotionCondition &condition)
{
    if (condition.type == MotionCondition::ConditionIoFalling || condition.type == MotionCondition::ConditionIoRising) {
        child(1)->setHidden(false);
        child(2)->setHidden(true);
    } else if (condition.type == MotionCondition::ConditionDelay) {
        child(1)->setHidden(true);
        child(2)->setHidden(false);
    } else {
        child(1)->setHidden(false);
        child(2)->setHidden(false);
    }
}


ElementWidgetItem::ElementWidgetItem(const MotionElement &element, const QList<RotationInfo> &rotations, QTreeWidgetItem *parent) :
    QTreeWidgetItem(parent, ElementWidgetItemType)
{
    SetElement(element, rotations);
}

void ElementWidgetItem::SetElement(const MotionElement &element, const QList<RotationInfo> &rotations)
{
    qDeleteAll(takeChildren());
    setText(0, element.description());
    MetaType::forEachProperty(element, [&, this] (QString name, QVariant value) {
        if (name == "stepInterval" || name == "stepSpeed") {
            RotationInfo rotation = RotationInfo::byAxisIndex(rotations, element.stepAxis);
            value = rotation.surfaceSpeed(value.toReal());
        } else if (name == "speed") {
            RotationInfo rotation = RotationInfo::byAxisIndex(rotations, element.mainAxis);
            value = rotation.surfaceSpeed(value.toReal());
        }
        auto *item = new PropertyWidgetItem(name, value, this, PropertyWidgetItemType);
        connect(item, &PropertyWidgetItem::DataChanged, this, &ElementWidgetItem::OnPropertyChanged);
    });
    new AxesWidgetItem(element.axis, this);
    foreach (const MotionPose &pos, element.pos) {
        new PoseWidgetItem(pos, element.axis, this);
    }
    foreach (const MotionCondition &condition, element.conditions) {
        new ConditionWidgetItem(condition, this);
    }
    setExpanded(true);
}

MotionElement ElementWidgetItem::GetElement(const QList<RotationInfo> &rotations) const
{
    MotionElement element;
    QMetaObject meta = element.staticMetaObject;
    for (int index = 0; index < childCount(); ++index) {
        QTreeWidgetItem *item = child(index);
        if (item->type() == PropertyWidgetItemType) {
            auto *propertyItem = static_cast<PropertyWidgetItem *>(item);
            if (propertyItem) {
                propertyItem->SetProperty(&element, meta.property(index), rotations);
            }
        } else if (item->type() == AxesWidgetItemType) {
            auto *axisItem = static_cast<AxesWidgetItem *>(item);
            if (axisItem) {
                element.axis = axisItem->GetMotionAxisList();
            }
        } else if (item->type() == PoseWidgetItemType) {
            auto *poseItem = static_cast<PoseWidgetItem *>(item);
            if (poseItem) {
                element.pos << poseItem->GetPose();
            }
        } else if (item->type() == ConditionWidgetItemType) {
            auto *conditionItem = static_cast<ConditionWidgetItem *>(item);
            if (conditionItem) {
                element.conditions << conditionItem->GetMotionCondition();
            }
        }
    }
    return element;
}

void ElementWidgetItem::OnPropertyChanged(const QVariant &newValue, const QVariant &oldValue)
{
    Q_UNUSED(newValue);
    Q_UNUSED(oldValue);
    auto *item = static_cast<PropertyWidgetItem *>(sender());
    if (item->GetPropertyName() == "type") {

    }
}

PropertyWidgetItem *ElementWidgetItem::GetPropertyItemByName(const QString &name)
{
    for (int index = 0; index < childCount(); ++index) {
        QTreeWidgetItem *item = child(index);
        if (item->type() != PropertyWidgetItemType) {
            continue;
        }
        auto *propertyItem = static_cast<PropertyWidgetItem *>(item);
        if (propertyItem->GetPropertyName() == name) {
            return propertyItem;
        }
    }
    return nullptr;
}

class RotationWidgetItem : public QTreeWidgetItem
{
public:
    RotationWidgetItem(const RotationInfo &rotation, QTreeWidgetItem *parent) :
        QTreeWidgetItem(parent, RotationWidgetItemType)
    {
        setRotationInfo(rotation);
    }

    void setRotationInfo(const RotationInfo &rotation)
    {
        qDeleteAll(takeChildren());
        setText(0, rotation.description());
        MetaType::forEachProperty(rotation, [this] (const QString &name, const QVariant &value) {
            new PropertyWidgetItem(name, value, this, RotationPropertyWidgetItemType);
        });
        setExpanded(true);
    }

    RotationInfo rotationInfo() const
    {
        RotationInfo rotation;
        QMetaObject meta = rotation.staticMetaObject;
        for (int index = 0; index < childCount(); ++index) {
            QTreeWidgetItem *item = child(index);
            if (item->type() == RotationPropertyWidgetItemType) {
                auto *propertyItem = static_cast<PropertyWidgetItem *>(item);
                if (propertyItem) {
                    propertyItem->SetProperty(&rotation, meta.property(index));
                }
            }
        }
        return rotation;
    }
};

class ElementDelegate : public QStyledItemDelegate
{
public:
    ElementDelegate(QObject *obj) : QStyledItemDelegate(obj)
    {}

    void InsertValueMap(const QString &type, const QMap<QVariant, QVariant> &text)
    {
        valueMap.insert(type, text);
    }

private:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        bool ok = false;
        int type = index.data(DelegateRole).toInt(&ok);
        if (!ok) {
            return QStyledItemDelegate::createEditor(parent, option, index);
        }
        if (type == ScanPathAdvancedEdit::DoubleSpinBox) {
            auto *spinBox = new QDoubleSpinBox(parent);
            spinBox->setRange(0, 99999.99);
            return spinBox;
        } else if (type == ScanPathAdvancedEdit::Combobox) {
            return new QComboBox(parent);
        } else if (type == ScanPathAdvancedEdit::SpinBox) {
            auto *spinBox = new QSpinBox(parent);
            spinBox->setRange(1, 99999);
            return spinBox;
        }
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        bool ok = false;
        int type = index.data(DelegateRole).toInt(&ok);
        if (!ok) {
            return;
        }
        if (type == ScanPathAdvancedEdit::DoubleSpinBox) {
            auto *spinBox = qobject_cast<QDoubleSpinBox *>(editor);
            spinBox->setValue(index.data(ValueDataRole).toReal());
            spinBox->setSuffix(index.data(SuffixRole).toString());
        } else if (type == ScanPathAdvancedEdit::Combobox) {
            auto *combox = qobject_cast<QComboBox *>(editor);
            QString valueType = index.data(ValueTypeRole).toString();
            if (valueType == EnumToKey(ScanPathAdvancedEdit::SensorType)) {
                foreach (Axis *axis, MotionManager::axisList()) {
                    QMap<QString, int> map = axis->GetSensorMap();
                    auto iter = map.begin();
                    while (iter != map.end()) {
                        QString name = tr("%1-%2(%3)").arg(axis->name(), iter.key()).arg(iter.value());
                        combox->addItem(name, iter.value());
                        iter++;
                    }
                }
            } else {
                QMap<QVariant, QVariant> items = valueMap.value(valueType);
                auto iter = items.begin();
                while (iter != items.end()) {
                    if (valueType == EnumToKey(ScanPathAdvancedEdit::AxisListType)) {
                        combox->addItem(iter.key().toString(), iter.value());
                    } else if (valueType == EnumToKey(ScanPathAdvancedEdit::ElementType)) {
                        combox->addItem(elementTypeMap[iter.key()].toString(), iter.key());
                    } else if (valueType == EnumToKey(ScanPathAdvancedEdit::InterpolateType)) {
                        combox->addItem(interpolateTypeMap[iter.key()].toString(), iter.key());
                    } else if (valueType == EnumToKey(ScanPathAdvancedEdit::ConditionType)) {
                        combox->addItem(conditionTypeMap[iter.key()].toString(), iter.key());
                    } else if (valueType == EnumToKey(ScanPathAdvancedEdit::RotationDirectionType)) {
                        combox->addItem(directionMap[iter.key()].toString(), iter.key());
                    } else if (valueType == EnumToKey(ScanPathAdvancedEdit::RotationType)) {
                        combox->addItem(rotationTypeMap[iter.key()].toString(), iter.key());
                    } else if (valueType == EnumToKey(ScanPathAdvancedEdit::SpeedType)) {
                        combox->addItem(speedTypeMap[iter.key()].toString(), iter.key());
                    }
                    iter++;
                }
            }
            if (combox->count() <= 0) {
                valueType == EnumToKey(ScanPathAdvancedEdit::AxisListType) ? combox->addItem(tr("无"), -1) : combox->addItem(tr("无"), "");
            }
            QString currenttext = index.data(Qt::DisplayRole).toString();
            combox->setCurrentText(currenttext);
        } else if (type == ScanPathAdvancedEdit::SpinBox) {
            auto *spinBox = qobject_cast<QDoubleSpinBox *>(editor);
            spinBox->setValue(index.data(ValueDataRole).toInt());
            spinBox->setSuffix(index.data(SuffixRole).toString());
        } else {
            QStyledItemDelegate::setEditorData(editor, index);
        }
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override
    {
        bool ok = false;
        int type = index.data(DelegateRole).toInt(&ok);
        if (!ok) {
            return;
        }
        if (type == ScanPathAdvancedEdit::DoubleSpinBox) {
            auto *spinBox = qobject_cast<QDoubleSpinBox *>(editor);
            double value = spinBox->value();
            model->setData(index, tr("%1").arg(value) + tr("%1").arg(spinBox->suffix()), Qt::DisplayRole);
            model->setData(index, value, ValueDataRole);
        } else if (type == ScanPathAdvancedEdit::Combobox) {
            auto *comBox = qobject_cast<QComboBox *>(editor);
            QString currentText = comBox->currentText();
            QVariant currentValue = comBox->currentData();
            model->setData(index, currentText, Qt::DisplayRole);
            model->setData(index, currentValue, ValueDataRole);
        } else if (type == ScanPathAdvancedEdit::SpinBox) {
            auto *spinBox = qobject_cast<QSpinBox *>(editor);
            int value = spinBox->value();
            model->setData(index, tr("%1").arg(value) + tr("%1").arg(spinBox->suffix()), Qt::DisplayRole);
            model->setData(index, value, ValueDataRole);
        } else {
            QStyledItemDelegate::setModelData(editor, model, index);
        }
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        Q_UNUSED(index);
        editor->setGeometry(option.rect);
    }

private:
    QMap<QString, QMap<QVariant, QVariant>> valueMap;
};

ScanPathAdvancedEdit::ScanPathAdvancedEdit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanPathAdvancedEdit)
{
    ui->setupUi(this);

    prerequisitesRootItem = new QTreeWidgetItem(ui->path);
    prerequisitesRootItem->setText(0, tr("扫描前提"));
    rotationRootItem = new QTreeWidgetItem(ui->path);
    rotationRootItem->setText(0, tr("旋转属性"));
    elementRootItem = new QTreeWidgetItem(ui->path);
    elementRootItem->setText(0, tr("扫描路线"));

    ui->rotationSensor->setEmptyText(tr("无"));

    /*! 使用motionPath的新接口 */
    ui->rectangle->installEventFilter(this);
    ui->advice->installEventFilter(this);
    ui->cylinderPrismatic->installEventFilter(this);
    ui->cylinderRotation->installEventFilter(this);
    ui->diskPrismatic->installEventFilter(this);
    ui->diskRotation->installEventFilter(this);

    ui->path->setColumnCount(2);
    ui->path->setContextMenuPolicy(Qt::CustomContextMenu);
    auto *delegate = new ElementDelegate(this);
    ui->path->setItemDelegateForColumn(1, delegate);
    if (elementTypeMap.isEmpty()) {
        elementTypeMap[QVariant::fromValue(MotionElement::None)] = tr("无");
        elementTypeMap[QVariant::fromValue(MotionElement::Stop)] = tr("停止");
        elementTypeMap[QVariant::fromValue(MotionElement::Home)] = tr("复位");
        elementTypeMap[QVariant::fromValue(MotionElement::Locate)] = tr("定位");
        elementTypeMap[QVariant::fromValue(MotionElement::MoveLine)] = tr("直线平移");
        elementTypeMap[QVariant::fromValue(MotionElement::MoveRel)] = tr("相对移动");
        elementTypeMap[QVariant::fromValue(MotionElement::MoveCircle)] = tr("环形移动");
        elementTypeMap[QVariant::fromValue(MotionElement::Continuous)] = tr("旋转");
        elementTypeMap[QVariant::fromValue(MotionElement::Cam)] = tr("自由曲线");
    }
    if (interpolateTypeMap.isEmpty()) {
        interpolateTypeMap[QVariant::fromValue(MotionElement::InterpolateNone)] = tr("无");
        interpolateTypeMap[QVariant::fromValue(MotionElement::InterpolateUnidirectional)] = tr("单向插值");
        interpolateTypeMap[QVariant::fromValue(MotionElement::InterpolateBidiretional)] = tr("双向插值");
    }
    if (axisListMap.isEmpty()) {
        axisListMap[tr("无")] = -1;
        foreach (Axis *axis, MotionManager::axisList()) {
            axisListMap[axis->name()] = axis->config().axisId;
        }
    }
    if (conditionTypeMap.isEmpty()) {
        conditionTypeMap[QVariant::fromValue(MotionCondition::ConditionNone)] = tr("无");
        conditionTypeMap[QVariant::fromValue(MotionCondition::ConditionIoFalling)] = tr("下降沿触发");
        conditionTypeMap[QVariant::fromValue(MotionCondition::ConditionIoRising)] = tr("上升沿触发");
        conditionTypeMap[QVariant::fromValue(MotionCondition::ConditionDelay)] = tr("延时触发");
    }
    if (directionMap.isEmpty()) {
        directionMap[QVariant::fromValue(RotationInfo::Forward)] = tr("正向旋转");
        directionMap[QVariant::fromValue(RotationInfo::Reverse)] = tr("反向旋转");
    }
    if (rotationTypeMap.isEmpty()) {
        rotationTypeMap[QVariant::fromValue(RotationInfo::None)] = tr("无");
        rotationTypeMap[QVariant::fromValue(RotationInfo::AxisContact)] = tr("装夹");
        rotationTypeMap[QVariant::fromValue(RotationInfo::SurfaceContact)] = tr("承载");
    }
    if (speedTypeMap.isEmpty()) {
        speedTypeMap[QVariant::fromValue(RotationInfo::Uniform)] = tr("匀速");
        speedTypeMap[QVariant::fromValue(RotationInfo::LimitAngularSpeed)] = tr("角速度限制");
    }
    delegate->InsertValueMap(EnumToKey(ElementType), elementTypeMap);
    delegate->InsertValueMap(EnumToKey(InterpolateType), interpolateTypeMap);
    delegate->InsertValueMap(EnumToKey(AxisListType), axisListMap);
    delegate->InsertValueMap(EnumToKey(ConditionType), conditionTypeMap);
    delegate->InsertValueMap(EnumToKey(RotationDirectionType), directionMap);
    delegate->InsertValueMap(EnumToKey(RotationType), rotationTypeMap);
    delegate->InsertValueMap(EnumToKey(SpeedType), speedTypeMap);

    ui->channelList->setItemDelegate(new ChannelDelegate(this));
    ui->channelList_advice->setItemDelegate(new ChannelDelegate(this));

    ui->path->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->channelList->setEditTriggers(QAbstractItemView::DoubleClicked);
    connect(ui->path, &QTreeWidget::customContextMenuRequested, this, &ScanPathAdvancedEdit::onPathConetextMenuRequested);
    connect(ui->path, &QTreeWidget::itemDoubleClicked, this, &ScanPathAdvancedEdit::onPathDoubleClicked);
    connect(ui->path, &QTreeWidget::doubleClicked, this, [this](const QModelIndex &index) {
        if (index.column() == 1) {
            ui->path->edit(index);
        }
    });

    QList<QDoubleSpinBox *> spinBoxes({
        ui->interval,
        ui->linearSpeed,
        ui->stepLinearSpeed,
        ui->maxAngularSpeed,
        ui->productDiameter,
        ui->wheelDiameter
    });
    for (QDoubleSpinBox *box: spinBoxes) {
        connect(box, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ScanPathAdvancedEdit::refreshSpeed);
    }
    refreshSpeed();

    LicenseManager::registerLicenseUpdate(this, &ScanPathAdvancedEdit::onLicenseUpdated);
}

ScanPathAdvancedEdit::~ScanPathAdvancedEdit()
{
    delete ui;
}

ScanPathAdvancedOptions ScanPathAdvancedEdit::options() const
{
    ScanPathAdvancedOptions options;
    options.motionPath = motionPath();
    options.motionType = EnumToKey(m_motionType);
    options.imageOptions = imageOptions();
    options.setupEnable = ui->setupEnable->isChecked();
    UTSetup setup = m_setup;
    if (options.setupEnable) {
        QHash<int, UTSetup> setups;
        QList<ChannelList> channelsList = multiChannels(ui->channelList);
        for (const auto &channels: channelsList) {
            for (int channelIndex: channels) {
                setups.insert(channelIndex, setup);
            }
        }
        if (setups.isEmpty()) {
            setups[0] = setup;
        }
        options.m_setups = setups;
    } else {
        options.m_setups = QHash<int, UTSetup>();
    }

    return options;
}

void ScanPathAdvancedEdit::setSetupVisibility(bool enabled)
{
    for (QRadioButton *button: findChildren<QRadioButton *>()) {                // 扫描计划选择器
        button->setEnabled(enabled);
    }

    ui->adviceInfo->setEnabled(enabled);                                        // 高级计划
    for (QGroupBox *box: ui->baseInfo->findChildren<QGroupBox *>()) {           // 基本扫描计划
        box->setEnabled(enabled);
    }

    ui->imageOptionsGroup->setEnabled(enabled);                                 // 成像参数组
    ui->setupGroup->setEnabled(true);
    ui->setupEnable->setEnabled(enabled);
    ui->setup->setEnabled(true);
}

void ScanPathAdvancedEdit::setOptions(const ScanPathAdvancedOptions &options)
{
    m_motionType = KeyToEnum(options.motionType, ScanPlan::CAM);
    m_motionPath = options.motionPath;
    bool readOnly = false;

    ui->container->setEnabled(!readOnly);
    ui->adviceInfo->setEnabled(!readOnly);
    ui->imageOptionsGroup->setEnabled(!readOnly);

    ui->imageOptions->clear();
    for (const ImageOptions &option: options.imageOptions) {
        addImageOptions(option);
    }

    ui->setupEnable->setChecked(options.setupEnable);
    if (!options.m_setups.isEmpty()) {
        m_setup = options.m_setups.begin().value();
    } else {
        m_setup = UTSetup();
    }

    const QList<UTChannel *> channels = UltrasonicManager::channels();
    if (channels.isEmpty()) {
        return;
    }
    UTSetup setup = channels.first()->setup();
    m_setup = setup;

    bool isRectangle = m_motionType == ScanPlan::RECTANGLE;
    bool isCylinder = m_motionType == ScanPlan::CYLINDER;
    bool isDisk = m_motionType == ScanPlan::DISK;
    bool isCylinderRotation = m_motionType == ScanPlan::CYLINDERROTATION;
    bool isDiskRotation = m_motionType == ScanPlan::DISKROTATION;

    ui->rectangle->setEnabled(isRectangle);
    ui->rectangle->setChecked(isRectangle);
    ui->cylinderPrismatic->setEnabled(isCylinder);
    ui->cylinderPrismatic->setChecked(isCylinder);
    ui->cylinderRotation->setEnabled(isCylinderRotation);
    ui->cylinderRotation->setChecked(isCylinderRotation);
    ui->diskPrismatic->setEnabled(isDisk);
    ui->diskPrismatic->setChecked(isDisk);
    ui->diskRotation->setEnabled(isDiskRotation);
    ui->diskRotation->setChecked(isDiskRotation);

    if (isRectangle) {
        ui->path->takeTopLevelItem(ui->path->indexOfTopLevelItem(prerequisitesRootItem));
        ui->path->takeTopLevelItem(ui->path->indexOfTopLevelItem(rotationRootItem));
    } else {
        ui->path->insertTopLevelItem(0, prerequisitesRootItem);
        ui->path->insertTopLevelItem(1, rotationRootItem);
        ui->rotationSensor->clear();
        for (Axis *axis: MotionManager::axisList()) {
            QMap<QString, int> map = axis->GetSensorMap();
            for (auto iter = map.begin(); iter != map.end(); ++iter) {
                ui->rotationSensor->addItem(tr("%1-%2(%3)").arg(axis->name(), iter.key()).arg(iter.value()), iter.value());
            }
        }
    }

    QWidgetList cylinderHideList = {
        ui->maxAngularSpeed, ui->maxAngularSpeedTitle
    };
    QWidgetList cylinderRotationHideList = {
        ui->angularSpeed, ui->angularSpeedTitle,
        ui->productAngularSpeed, ui->productAngularSpeedTitle,
        ui->maxAngularSpeed, ui->maxAngularSpeedTitle,
    };
    QWidgetList diskHideList = {
        ui->angularSpeed, ui->angularSpeedTitle,
        ui->productAngularSpeed, ui->productAngularSpeedTitle,
        ui->productDiameter, ui->productDiameterTitle,
        ui->groupBearing
    };
    QWidgetList rectangleHideList = {
        ui->angularSpeed, ui->angularSpeedTitle,
        ui->productAngularSpeed, ui->productAngularSpeedTitle,
        ui->productDiameter, ui->productDiameterTitle,
        ui->maxAngularSpeed, ui->maxAngularSpeedTitle,
        ui->groupBearing,
        ui->groupOther
    };

    foreach (QWidget *widget, rectangleHideList) {
        widget->setVisible(true);
    }
    foreach (QWidget *widget, diskHideList) {
        widget->setVisible(true);
    }
    foreach (QWidget *widget, cylinderHideList) {
        widget->setVisible(true);
    }
    foreach (QWidget *widget, cylinderRotationHideList) {
        widget->setVisible(true);
    }

    if (isRectangle) {
        ui->scanAxisTitle->setText(tr("扫描轴"));
        foreach (QWidget *widget, rectangleHideList) {
            widget->setVisible(false);
        }
    } else if (isCylinder) {
        ui->scanAxisTitle->setText(tr("扫描轴"));
        foreach (QWidget *widget, cylinderHideList) {
            widget->setVisible(false);
        }
    } else if (isDisk || isDiskRotation) {
        ui->scanAxisTitle->setText(isDisk ? tr("扫描轴") : tr("步进轴"));
        for (QWidget *widget: diskHideList) {
            widget->setVisible(false);
        }
    } else if (isCylinderRotation) {
        ui->scanAxisTitle->setText(tr("步进轴"));
        for (QWidget *widget: cylinderRotationHideList) {
            widget->setVisible(false);
        }
    }

    if (m_motionType == ScanPlan::CAM) {
        ui->scanInfo->setCurrentWidget(ui->adviceInfo);
    } else {
        ui->scanInfo->setCurrentWidget(ui->baseInfo);
    }

    if (ui->scanInfo->currentWidget() == ui->adviceInfo) {
        refreshAdvanced(m_motionPath);
    } else if (ui->scanInfo->currentWidget() == ui->baseInfo) {
        refreshBase(m_motionPath);
    }
    resize(ui->verticalLayout_3->minimumSize());
}

/*! 获取编辑界面完成的motionPath */
MotionPath ScanPathAdvancedEdit::motionPath() const
{
    if (ui->scanInfo->currentWidget() == ui->adviceInfo) {
        return motionPathFromAdvanced();
    } else if (ui->scanInfo->currentWidget() == ui->baseInfo) {
        return motionPathFromBase();
    }
    return MotionPath();
}

/*! 获取成像参数 */
QList<ImageOptions> ScanPathAdvancedEdit::imageOptions() const
{
    QList<ImageOptions> list;
    for (int index = 0; index < ui->imageOptions->count(); ++index) {
        QListWidgetItem *item = ui->imageOptions->item(index);
        list.append(item->data(Qt::UserRole).value<ImageOptions>());
    }
    return list;
}

/*! 使用motionPath的新接口 */
bool ScanPathAdvancedEdit::eventFilter(QObject *watch, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        auto *radio = static_cast<QRadioButton *>(watch);
        if (!radio || !radio->isEnabled()) {
            return QWidget::eventFilter(watch, event);
        }
        if (radio->isChecked()) {
            return true;
        }
        radio->setChecked(true);
        if (watch == ui->advice && ui->scanInfo->currentWidget() == ui->baseInfo) {
            // 对当前基础页面参数检查，是一个合格的MotionPath之后，使用组成的MotionPath更新高级页面并显示
            MotionPath motionPath = motionPathFromBase();
            refreshAdvanced(motionPath);
            ui->scanInfo->setCurrentWidget(ui->adviceInfo);
            m_motionType = ScanPlan::CAM;
        } else if ((watch == ui->rectangle || watch == ui->cylinderPrismatic || watch == ui->cylinderRotation
                 || watch == ui->diskPrismatic || watch == ui->diskRotation) && ui->scanInfo->currentWidget() == ui->adviceInfo) {
            // 需要检查路径是否有改动，如果有改动，需要给与提示
            MotionPath motionPath = motionPathFromAdvanced();
            refreshBase(motionPath);
            ui->scanInfo->setCurrentWidget(ui->baseInfo);
            if (watch == ui->rectangle) {
                m_motionType = ScanPlan::RECTANGLE;
            } else if (watch == ui->cylinderPrismatic) {
                m_motionType = ScanPlan::CYLINDER;
            } else if (watch == ui->cylinderRotation) {
                m_motionType = ScanPlan::CYLINDERROTATION;
            } else if (watch == ui->diskPrismatic) {
                m_motionType = ScanPlan::DISK;
            } else if (watch == ui->diskRotation) {
                m_motionType = ScanPlan::DISKROTATION;
            }
        }
    } else if (event->type() == QEvent::MouseButtonRelease) {
        if (watch == ui->rectangle || watch == ui->cylinderPrismatic || watch == ui->cylinderRotation
                         || watch == ui->diskPrismatic || watch == ui->diskRotation || watch == ui->advice) {
            return true;
        }
    }
    return QWidget::eventFilter(watch, event);
}

void ScanPathAdvancedEdit::onLicenseUpdated()
{
    bool develop = LicenseManager::instance()->hasDevelopLicense();
    ui->expand->setVisible(develop);
    ui->imageOptionsGroup->setVisible(develop);
}

/*! 使用motionPath的新接口 */
void ScanPathAdvancedEdit::refreshAdvanced(const MotionPath &path)
{
    if (m_motionType == ScanPlan::CYLINDER || m_motionType == ScanPlan::DISK) {
        ui->intervalAdvice->setSuffix(tr(" °"));
    } else {
        ui->intervalAdvice->setSuffix(tr(" mm"));
    }
    ui->intervalAdvice->setValue(path.interval);
    ui->mulitChannels->setChecked(path.multiChannelEnable);
    ui->channelOffset->setValue(path.channelOffset);
    ui->channelList_advice->clear();

    refreshChannels(path, ui->channelList_advice);

    refreshRotations(path.rotations, rotationRootItem);
    refreshElements(path.prerequisites, path.rotations, prerequisitesRootItem);
    refreshElements(path.elements, path.rotations, elementRootItem);
}

/*! 使用motionPath的新接口 */
void ScanPathAdvancedEdit::refreshBase(const MotionPath &path)
{
    MotionElement firstElement = path.elements.value(0);

    RotationInfo stepRotation = RotationInfo::byAxisIndex(path.rotations, firstElement.stepAxis);
    RotationInfo scanRotation = RotationInfo::byAxisIndex(path.rotations, firstElement.mainAxis);
    if (m_motionType == ScanPlan::CYLINDER || m_motionType == ScanPlan::DISK) {
        ui->interval->setValue(scanRotation.surfaceSpeed(path.interval));
    } else {
        ui->interval->setValue(path.interval);
    }

    ui->stepInterval->setValue(stepRotation.surfaceSpeed(firstElement.stepInterval));
    ui->rotationSensor->deselectAll();

    MotionElement secondElement = path.elements.value(1);
    MotionPose startPos = firstElement.pos.value(0);
    MotionPose endPos;
    if (m_motionType == ScanPlan::RECTANGLE) {
        ui->linearSpeed->setValue(firstElement.speed);
        ui->stepLinearSpeed->setValue(firstElement.stepSpeed);
        ui->unidirection->setChecked(firstElement.interPolicy == MotionElement::InterpolateUnidirectional);
        endPos = secondElement.pos.value(1);

//        filter["AxisType"] = EnumToKey(Axis::Prismatic);
//        ui->scanAxis->SetFilter(filter);
        ui->scanAxis->setCurrentIndex(firstElement.mainAxis);
    } else if (m_motionType == ScanPlan::CYLINDER || m_motionType == ScanPlan::DISK || m_motionType == ScanPlan::CYLINDERROTATION || m_motionType == ScanPlan::DISKROTATION) {
//        filter["AxisType"] = EnumToKey(Axis::Rotation);
//        filter["Hidden"] = false;
//        ui->scanAxis->SetFilter(filter);
        if (m_motionType == ScanPlan::CYLINDER || m_motionType == ScanPlan::DISK) {
            endPos = secondElement.pos.value(0);
            bool hasFound = false;
            for (const MotionElement &element: path.prerequisites) {
                if (hasFound) {
                    break;
                }
                for (int axisIndex: element.axis.toList()) {
                    Axis *axis = MotionManager::axis(axisIndex);
                    if (axis && axis->isRotation()) {
                        ui->scanAxis->setCurrentIndex(axis->config().axisId);
                        ui->linearSpeed->setValue(scanRotation.surfaceSpeed(element.speed));
                        hasFound = true;
                        break;
                    }
                }
            }
        } else {
            endPos = secondElement.pos.value(1);
            ui->scanAxis->setCurrentIndex(firstElement.stepAxis);
            ui->linearSpeed->setValue(scanRotation.surfaceSpeed(firstElement.speed));
        }
        ui->stepLinearSpeed->setValue(stepRotation.surfaceSpeed(firstElement.stepSpeed));

        RotationInfo rotation;
        for (const RotationInfo &info: path.rotations) {
            if (info.axis == ui->scanAxis->currentAxisIndex()) {
                rotation = info;
                break;
            }
        }
        ui->rotationReversed->setChecked(rotation.direction == RotationInfo::Reverse);
        if (m_motionType == ScanPlan::DISK || m_motionType == ScanPlan::DISKROTATION) {
            ui->maxAngularSpeed->setValue(rotation.maxAngularSpeed);
        }

        for (const MotionCondition &condition: firstElement.conditions) {
            if (condition.type == MotionCondition::ConditionIoFalling || condition.type == MotionCondition::ConditionIoRising) {
                ui->rotationSensor->selectItem(qRound(condition.arg));
                ui->rotationRisingEdge->setChecked(condition.type == MotionCondition::ConditionIoRising);
            }
        }
        if (m_motionType == ScanPlan::CYLINDER || m_motionType == ScanPlan::CYLINDERROTATION) {
            ui->bearingScan->setChecked(rotation.type == RotationInfo::SurfaceContact);
            ui->coaxialScan->setChecked(rotation.type == RotationInfo::AxisContact);
            ui->wheelDiameter->setValue(rotation.axisDiameter);
            ui->productDiameter->setValue(rotation.partDiameter);
        }
    }
    ui->startPoint->setDisplayPos(startPos.value(0), startPos.value(1), startPos.value(2));
    ui->endPoint->setDisplayPos(endPos.value(0), endPos.value(1), endPos.value(2));

    ui->groupParallel->setChecked(path.multiChannelEnable);
    ui->probeInterval->setValue(path.channelOffset);

    refreshChannels(path, ui->channelList);
}

void ScanPathAdvancedEdit::refreshChannels(const MotionPath &path, QTreeWidget *tree)
{
    tree->clear();
    tree->setColumnCount(2);
    int rowCount = path.multiChannels.count();
    for (int row = 0; row < rowCount; ++row) {
        auto *item = new QTreeWidgetItem(tree);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ChannelList channels = path.multiChannels.value(row);
        for (int column = 0; column < tree->columnCount(); ++column) {
            int channelIndex = channels.value(column);
            UTChannel *channel = UltrasonicManager::channel(channelIndex);
            if (channel) {
                item->setData(column, Qt::DisplayRole, channel->name());
                item->setData(column, Qt::UserRole, channelIndex);
            } else {
                item->setData(column, Qt::DisplayRole, tr("无"));
                item->setData(column, Qt::UserRole, -1);
            }
        }
    }
}

/*! 通过树形列表获取多通道信息 */
QList<ChannelList> ScanPathAdvancedEdit::multiChannels(QTreeWidget *tree) const
{
    QList<ChannelList> multiChannels;
    for (int row = 0; row < tree->topLevelItemCount(); ++row) {
        ChannelList channels;
        for (int column = 0; column < tree->columnCount(); ++column) {
            int channelIndex = tree->topLevelItem(row)->data(column, Qt::UserRole).toInt();
            channels << channelIndex;
        }
        multiChannels << channels;
    }
    return multiChannels;
}

/*! 使用motionPath的新接口 */
void ScanPathAdvancedEdit::refreshElements(const QList<MotionElement> &elements, const QList<RotationInfo> &rotations, QTreeWidgetItem *treeItem)
{
    qDeleteAll(treeItem->takeChildren());
    for (const MotionElement &element: elements) {
        new ElementWidgetItem(element, rotations, treeItem);
    }
    treeItem->setExpanded(true);
    ui->path->resizeColumnToContents(0);
}

/*! 使用motionPath的新接口 */
void ScanPathAdvancedEdit::refreshRotations(const QList<RotationInfo> &rotations, QTreeWidgetItem *treeItem)
{
    qDeleteAll(treeItem->takeChildren());
    for (const RotationInfo &info: rotations) {
        new RotationWidgetItem(info, treeItem);
    }
    treeItem->setExpanded(true);
    ui->path->resizeColumnToContents(0);
}

/*! 使用motionPath的新接口 */
void ScanPathAdvancedEdit::onPathConetextMenuRequested()
{
    QList<QTreeWidgetItem *> items = ui->path->selectedItems();
    if (items.isEmpty()) {
        return;
    }

    ui->actionAddElement->setEnabled(false);
    ui->actionAddRotation->setEnabled(false);
    QTreeWidgetItem *item = items.value(0);
    if (ui->path->indexOfTopLevelItem(item) >= 0) {                             // 根节点
        if (item == rotationRootItem) {
            ui->actionAddRotation->setEnabled(true);
        } else {
            ui->actionAddElement->setEnabled(true);
        }
    }

    QMenu menu;
    if (item->type() == ElementWidgetItemType) {
        menu.addAction(ui->actionAddElement);
        menu.addAction(ui->actionEditElement);
        menu.addAction(ui->actionDeleteElement);
        menu.addAction(ui->actionAddPos);
        menu.addAction(ui->actionAddCondition);
    } else if (item->type() == PoseWidgetItemType) {
        menu.addAction(ui->actionAddPos);
        menu.addAction(ui->actionEditPos);
        menu.addAction(ui->actionDeletePos);
    } else if (item->type() == ConditionWidgetItemType) {
        menu.addAction(ui->actionAddCondition);
        menu.addAction(ui->actionEditCondition);
        menu.addAction(ui->actionDeleteCondition);
    } else if (item->type() == RotationWidgetItemType) {
        menu.addAction(ui->actionAddRotation);
        menu.addAction(ui->actionEditRotation);
        menu.addAction(ui->actionDeleteRotation);
    }
    menu.exec(QCursor::pos());
}

/*! 使用motionPath的新接口 */
void ScanPathAdvancedEdit::onPathDoubleClicked(QTreeWidgetItem *item)
{
    switch (item->type()) {
    case ElementWidgetItemType:
        on_actionEditElement_triggered();
        break;
    case PoseWidgetItemType:
        on_actionEditPos_triggered();
        break;
    case RotationWidgetItemType:
        on_actionEditRotation_triggered();
        break;
    default:
        break;
    }
}

/*! 从高级界面获取MotionPath */
MotionPath ScanPathAdvancedEdit::motionPathFromAdvanced() const
{
    MotionPath motionPath;
    motionPath.interval = ui->intervalAdvice->value();
    motionPath.multiChannelEnable = ui->mulitChannels->isChecked();
    motionPath.channelOffset = ui->channelOffset->value();
    motionPath.multiChannels = multiChannels(ui->channelList_advice);

    for (int index = 0; index < rotationRootItem->childCount(); ++index) {
        QTreeWidgetItem *item = rotationRootItem->child(index);
        auto *rotationItem = static_cast<RotationWidgetItem *>(item);
        if (rotationItem) {
            motionPath.addRotation(rotationItem->rotationInfo());
        }
    }
    for (int index = 0; index < prerequisitesRootItem->childCount(); ++index) {
        QTreeWidgetItem *item = prerequisitesRootItem->child(index);
        auto *elementItem = static_cast<ElementWidgetItem *>(item);
        if (elementItem) {
            motionPath.addPrerequisite(elementItem->GetElement(motionPath.rotations));
        }
    }
    for (int index = 0; index < elementRootItem->childCount(); ++index) {
        QTreeWidgetItem *item = elementRootItem->child(index);
        auto *elementItem = static_cast<ElementWidgetItem *>(item);
        if (elementItem) {
            motionPath.addElement(elementItem->GetElement(motionPath.rotations));
        }
    }
    return motionPath;
}

/*! 从基本页面获取MotionPath */
MotionPath ScanPathAdvancedEdit::motionPathFromBase() const
{
    MotionPath motionPath;
    motionPath.interval = ui->interval->value();
    motionPath.multiChannelEnable = ui->groupParallel->isChecked();
    motionPath.channelOffset = ui->probeInterval->value();

    motionPath.multiChannels = multiChannels(ui->channelList);

    MotionElement element;
    // 组织rotation/condition
    if (m_motionType == ScanPlan::CYLINDER
            || m_motionType == ScanPlan::DISK
            || m_motionType == ScanPlan::CYLINDERROTATION
            || m_motionType == ScanPlan::DISKROTATION) {
        RotationInfo rotation;
        RotationInfo::Type type = RotationInfo::None;
        if (m_motionType == ScanPlan::CYLINDER || m_motionType == ScanPlan::CYLINDERROTATION) {
            if (ui->bearingScan->isChecked()) {
                type = RotationInfo::SurfaceContact;
            } else if (ui->coaxialScan->isChecked()) {
                type = RotationInfo::AxisContact;
            }
            rotation.speedType = RotationInfo::Uniform;
            rotation.axisDiameter = ui->wheelDiameter->value();
            rotation.partDiameter = ui->productDiameter->value();
        } else {
            type = RotationInfo::AxisContact;
            rotation.speedType = RotationInfo::LimitAngularSpeed;
            if (m_motionType == ScanPlan::DISK || m_motionType == ScanPlan::DISKROTATION) {
                rotation.maxAngularSpeed = ui->maxAngularSpeed->value();
            }
        }
        rotation.type = type;
        rotation.axis = ui->scanAxis->currentAxisIndex();
        rotation.direction = ui->rotationReversed->isChecked() ? RotationInfo::Reverse : RotationInfo::Forward;
        motionPath.rotations << rotation;

        for (const QVariant &sensor: ui->rotationSensor->selectedItemData()) {
            MotionCondition condition;
            condition.type = ui->rotationRisingEdge->isChecked() ? MotionCondition::ConditionIoRising : MotionCondition::ConditionIoFalling;
            condition.axis = ui->scanAxis->currentAxisIndex();
            condition.arg = sensor.toInt();
            condition.repCount = ui->rotationCount->value();
            element.conditions << condition;
        }

        if (m_motionType == ScanPlan::CYLINDER || m_motionType == ScanPlan::DISK) {
            MotionElement requisityElement;
            requisityElement.type = MotionElement::Continuous;
            requisityElement.mainAxis = ui->scanAxis->currentAxisIndex();
            requisityElement.axis.setAxis(ui->scanAxis->currentAxisIndex());
            RotationInfo rotation = RotationInfo::byAxisIndex(motionPath.rotations, requisityElement.mainAxis);
            requisityElement.speed = rotation.nativeSpeed(ui->linearSpeed->value());
            motionPath.prerequisites << requisityElement;

            motionPath.interval = rotation.nativeSpeed(ui->interval->value());
        }
    }

    MotionAxisList axisList;
    axisList.setAxis(0, true);
    axisList.setAxis(1, true);
    axisList.setAxis(2, true);
//    axisList.setAxis(ui->scanAxis->currentIndex(), true);
    element.axis = axisList;
    if (m_motionType == ScanPlan::RECTANGLE || m_motionType == ScanPlan::CYLINDER || m_motionType == ScanPlan::DISK) {
        element.mainAxis = ui->scanAxis->currentAxisIndex();
    } else if (m_motionType == ScanPlan::CYLINDERROTATION || m_motionType == ScanPlan::DISKROTATION) {
        element.stepAxis = ui->scanAxis->currentAxisIndex();
        element.axis.setAxis(ui->scanAxis->currentAxisIndex(), true);
    }
    element.type = MotionElement::MoveLine;
    QVector3D startPos = ui->startPoint->nativePos();
    QVector3D endPos = ui->endPoint->nativePos();
    QMap<qreal, int> lengthMap;
    lengthMap.insert(qAbs(endPos.x() - startPos.x()), 0);
    lengthMap.insert(qAbs(endPos.y() - startPos.y()), 1);
    lengthMap.insert(qAbs(endPos.z() - startPos.z()), 2);
    auto iter = lengthMap.end();
    while (iter != lengthMap.begin()) {
        iter--;
        if (iter.value() != element.mainAxis) {
            if (m_motionType == ScanPlan::RECTANGLE || m_motionType == ScanPlan::CYLINDER || m_motionType == ScanPlan::DISK) {
                element.stepAxis = iter.value();
            } else if (m_motionType == ScanPlan::CYLINDERROTATION || m_motionType == ScanPlan::DISKROTATION) {
                element.mainAxis = iter.value();
            }
            break;
        }
    }

    RotationInfo stepRotation = RotationInfo::byAxisIndex(motionPath.rotations, element.stepAxis);
    RotationInfo scanRotation = RotationInfo::byAxisIndex(motionPath.rotations, element.mainAxis);
    element.speed = scanRotation.nativeSpeed(ui->linearSpeed->value());
    element.stepInterval = stepRotation.nativeSpeed(ui->stepInterval->value());
    element.stepSpeed = stepRotation.nativeSpeed(ui->stepLinearSpeed->value());
    MotionPose topLeft;
    topLeft << startPos.x() << startPos.y() << startPos.z();
    MotionPose bottomRight;
    bottomRight << endPos.x() << endPos.y() << endPos.z();

    element.interPolicy = ui->unidirection->isChecked() ? MotionElement::InterpolateUnidirectional : MotionElement::InterpolateBidiretional;
    MotionElement firstElement = element;
    MotionElement secondElement = element;
    // 组织pos
    if (m_motionType == ScanPlan::RECTANGLE) {
        MotionPose topRight;
        MotionPose bottomLeft;
        switch (element.mainAxis) {
        case 0:
            topRight << endPos.x() << startPos.y() << startPos.z();
            bottomLeft << startPos.x() << endPos.y() << endPos.z();
            break;
        case 1:
            topRight << startPos.x() << endPos.y() << startPos.z();
            bottomLeft << endPos.x() << startPos.y() << endPos.z();
            break;
        case 2:
            topRight << startPos.x() << startPos.y() << endPos.z();
            bottomLeft << endPos.x() << endPos.y() << startPos.z();
            break;
        default:
            break;
        }
        firstElement.pos << topLeft << topRight;
        secondElement.pos << bottomLeft << bottomRight;
    } else if (m_motionType == ScanPlan::CYLINDER || m_motionType == ScanPlan::DISK) {
        firstElement.pos << topLeft;
        secondElement.pos << bottomRight;
    } else if (m_motionType == ScanPlan::CYLINDERROTATION || m_motionType == ScanPlan::DISKROTATION) {
        RotationInfo rotation = RotationInfo::byAxisIndex(motionPath.rotations, ui->scanAxis->currentAxisIndex());
        qreal minDegree = rotation.nativeDegrees(0);
        qreal maxDegree = rotation.nativeDegrees(360);
        MotionPose firstStart = topLeft;
        firstStart.append(minDegree);
        MotionPose secondStart = topLeft;
        secondStart.append(maxDegree);
        MotionPose firstEnd = bottomRight;
        firstEnd.append(minDegree);
        MotionPose secondEnd = bottomRight;
        secondEnd.append(maxDegree);
        firstElement.pos << firstStart << firstEnd;
        secondElement.pos << secondStart << secondEnd;
    }
    motionPath.elements << firstElement << secondElement;
    return motionPath;
}

/*! 添加Element */
void ScanPathAdvancedEdit::on_actionAddElement_triggered()
{
    QList<QTreeWidgetItem *> items = ui->path->selectedItems();
    if (items.isEmpty()) {
        return;
    }
    ElementWidgetItem *elementItem = nullptr;
    QTreeWidgetItem *parent = nullptr;
    QTreeWidgetItem *item = items.value(0);
    if (item->type() == ElementWidgetItemType) {
        elementItem = static_cast<ElementWidgetItem *>(item);
        parent = item->parent();
    } else {
        QTreeWidgetItem *lastChild = item->child(item->childCount() - 1);
        if (lastChild) {
            elementItem = static_cast<ElementWidgetItem *>(lastChild);
        }
        parent = item;
    }
    QList<RotationInfo> rotations = currentRotations();
    ElementWidgetItem * newItem = new ElementWidgetItem(elementItem ? elementItem->GetElement(rotations) : MotionElement(), rotations, parent);
    elementRootItem->setExpanded(true);
    newItem->setExpanded(true);
}

/*! 编辑Elment */
void ScanPathAdvancedEdit::on_actionEditElement_triggered()
{
    QList<QTreeWidgetItem *> items = ui->path->selectedItems();
    if (items.isEmpty()) {
        return;
    }
    QTreeWidgetItem *item = items.value(0);
    auto *elementItem = static_cast<ElementWidgetItem *>(item);
    if (!elementItem) {
        return;
    }
    QList<RotationInfo> rotations = currentRotations();
    MotionElement element = elementItem->GetElement(rotations);
    DialogMotionElement dialog;
    dialog.SetMotionElement(element, rotations);
    if (dialog.exec() == QDialog::Accepted) {
        elementItem->SetElement(dialog.GetMotionElement(rotations), rotations);
    }
}

/*! 添加pose点 */
void ScanPathAdvancedEdit::on_actionAddPos_triggered()
{
    QList<QTreeWidgetItem *> items = ui->path->selectedItems();
    if (items.isEmpty()) {
        return;
    }
    QTreeWidgetItem *item = items.value(0);
    ElementWidgetItem *elementItem = nullptr;
    if (item->type() == ElementWidgetItemType) {
        elementItem = static_cast<ElementWidgetItem *>(item);
    } else if (item->type() == PoseWidgetItemType) {
        elementItem = static_cast<ElementWidgetItem *>(item->parent());
    }
    if (!elementItem) {
        return;
    }
    int index = -2;
    for (int row = 0; row < elementItem->childCount(); ++row) {
        if (elementItem->child(row)->type() == PoseWidgetItemType) {
            index = row;
        }
    }
    MotionElement element = elementItem->GetElement(currentRotations());
    MotionPose pos = element.pos.value(element.pos.count() - 1);
    index < 0 ? index = elementItem->childCount() : index + 1;
    elementItem->insertChild(index, new PoseWidgetItem(pos, element.axis));
}

void ScanPathAdvancedEdit::on_actionEditPos_triggered()
{
    QList<QTreeWidgetItem *> items = ui->path->selectedItems();
    if (items.isEmpty()) {
        return;
    }
    QTreeWidgetItem *item = items.value(0);
    auto *poseItem = static_cast<PoseWidgetItem *>(item);
    if (poseItem) {
        DialogMotionPose dialog;
        dialog.SetMotionPose(poseItem->GetPose(), poseItem->GetAxes());
        if (dialog.exec() == QDialog::Accepted) {
            poseItem->SetPose(dialog.GetMostionPose(), poseItem->GetAxes());
        }
    }
}

void ScanPathAdvancedEdit::on_actionDeleteElement_triggered()
{
    QList<QTreeWidgetItem *> items = ui->path->selectedItems();
    if (items.isEmpty()) {
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定要删除当前选中的扫描路线"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }
    for (QTreeWidgetItem *item: items) {
        if (item->type() == ElementWidgetItemType) {
            delete item;
        }
    }
}

void ScanPathAdvancedEdit::on_actionDeletePos_triggered()
{
    QList<QTreeWidgetItem *> items = ui->path->selectedItems();
    if (items.isEmpty()) {
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定要删除当前选中的扫描位置"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        foreach (QTreeWidgetItem *item, items) {
            if (item->type() == PoseWidgetItemType) {
                delete item;
            }
        }
    }
}

void ScanPathAdvancedEdit::on_actionAddCondition_triggered()
{
    QList<QTreeWidgetItem *> items = ui->path->selectedItems();
    if (items.isEmpty()) {
        return;
    }
    QTreeWidgetItem *item = items.value(0);
    ElementWidgetItem *elementItem = nullptr;
    if (item->type() == ElementWidgetItemType) {
        elementItem = static_cast<ElementWidgetItem *>(item);
    } else if (item->type() == ConditionWidgetItemType) {
        elementItem = static_cast<ElementWidgetItem *>(item->parent());
    }
    if (!elementItem) {
        return;
    }
    int index = -2;
    for (int row = 0; row < elementItem->childCount(); ++row) {
        if (elementItem->child(row)->type() == ConditionWidgetItemType) {
            index = row;
        }
    }
    MotionElement element = elementItem->GetElement(currentRotations());
    MotionCondition condition = element.conditions.value(element.conditions.count() - 1);
    index < 0 ? index = elementItem->childCount() : index + 1;
    elementItem->insertChild(index, new ConditionWidgetItem(condition));
}

void ScanPathAdvancedEdit::on_actionEditCondition_triggered()
{
    QList<QTreeWidgetItem *> items = ui->path->selectedItems();
    if (items.isEmpty()) {
        return;
    }
    QTreeWidgetItem *item = items.value(0);
    auto *conditionItem = static_cast<ConditionWidgetItem *>(item);
    if (conditionItem) {
        DialogMotionCondition dialog;
        dialog.SetCondition(conditionItem->GetMotionCondition());
        if (dialog.exec() == QDialog::Accepted) {
            conditionItem->SetCondition(dialog.GetCondition());
        }
    }
}

void ScanPathAdvancedEdit::on_actionDeleteCondition_triggered()
{
    QList<QTreeWidgetItem *> items = ui->path->selectedItems();
    if (items.isEmpty()) {
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定要删除当前选中的扫描位置"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        foreach (QTreeWidgetItem *item, items) {
            if (item->type() == ConditionWidgetItemType) {
                delete item;
            }
        }
    }
}

void ScanPathAdvancedEdit::on_actionAddRotation_triggered()
{
    RotationInfo rotation;
    auto *item = static_cast<RotationWidgetItem *>(rotationRootItem->child(rotationRootItem->childCount() - 1));
    if (item) {
        rotation = item->rotationInfo();
    }
    new RotationWidgetItem(rotation, rotationRootItem);
    rotationRootItem->setExpanded(true);
}

void ScanPathAdvancedEdit::on_actionEditRotation_triggered()
{
    QList<QTreeWidgetItem *> items = ui->path->selectedItems();
    if (items.isEmpty()) {
        return;
    }
    QTreeWidgetItem *item = items.value(0);
    auto *rotationItem = static_cast<RotationWidgetItem *>(item);
    if (rotationItem) {
        DialogRotationInfo dialog;
        dialog.SetRotationInfo(rotationItem->rotationInfo());
        if (dialog.exec() == QDialog::Accepted) {
            rotationItem->setRotationInfo(dialog.GetRotationInfo());
        }
    }
}

void ScanPathAdvancedEdit::on_actionDeleteRotation_triggered()
{
    QList<QTreeWidgetItem *> items = ui->path->selectedItems();
    if (items.isEmpty()) {
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定要删除当前选中的旋转属性"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        foreach (QTreeWidgetItem *item, items) {
            if (item->type() == RotationWidgetItemType) {
                delete item;
            }
        }
    }
}

void ScanPathAdvancedEdit::on_imageOptions_itemDoubleClicked(QListWidgetItem *item)
{
    DialogImageOptions editor(this);
    ImageOptions defaultOptions = item->data(Qt::UserRole).value<ImageOptions>();
    editor.SetDefaultOptions(defaultOptions);
    editor.SetOptions(item->data(Qt::UserRole).value<ImageOptions>());
    if (editor.exec() != QDialog::Accepted) {
        return;
    }
    ImageOptions options = editor.GetOptions();
    item->setText(imageOptionsDescription(options));
    item->setData(Qt::UserRole, QVariant::fromValue(options));
}

void ScanPathAdvancedEdit::on_removeAdviceImage_clicked()
{
    qDeleteAll(ui->imageOptions->selectedItems());
}

void ScanPathAdvancedEdit::on_addAdviceImage_clicked()
{
    DialogImageOptions editor(this);
    ImageOptions defaultOptions = defaultImageOptions();
    editor.SetDefaultOptions(defaultOptions);
    editor.SetOptions(defaultOptions);
    if (editor.exec() != QDialog::Accepted) {
        return;
    }
    ImageOptions options = editor.GetOptions();
    addImageOptions(options);
}

ImageOptions ScanPathAdvancedEdit::defaultImageOptions() const
{
    return options().adviceImageOptions(0);
}

QString ScanPathAdvancedEdit::imageOptionsDescription(const ImageOptions &options)
{
    QStringList channelList;
    const auto &channels = options.channels();
    for (int index = 0; index < channels.count(); ++index) {
        if (channels.hasAxis(index)) {
            channelList << QString::number(index + 1);
        }
    }
    Axis *axisX = MotionManager::axis(options.axisX());
    Axis *axisY = MotionManager::axis(options.axisY());
    QString axisNameX = axisX != nullptr ? axisX->name() : "?";
    QString axisNameY = axisY != nullptr ? axisY->name() : "?";
    QString channelNames = channelList.isEmpty() ? tr("无") : channelList.join("/");
    return tr("%1 - %2 通道(%3)").arg(axisNameX, axisNameY, channelNames);
}

void ScanPathAdvancedEdit::addImageOptions(const ImageOptions &options)
{
    QListWidgetItem *item = new QListWidgetItem(imageOptionsDescription(options), ui->imageOptions);
    item->setData(Qt::UserRole, QVariant::fromValue(options));
}

/*! 限制线速度 */
double ScanPathAdvancedEdit::speedLimit(double speed)
{
    Axis *scanAxis = ui->scanAxis->currentAxis();
    if (scanAxis == nullptr) {
        return 50;
    }
    double speedLimit = scanAxis->config().speedMax;
    if (!scanAxis->isRotation()) {
        return speedLimit < speed ? speedLimit : speed;
    } else {
        if (m_motionType == ScanPlan::CYLINDER || m_motionType == ScanPlan::CYLINDERROTATION) {
            double realSpeed = 0;
            if (ui->bearingScan->isChecked()) {
                realSpeed = qDegreesToRadians(speedLimit) * ui->wheelDiameter->value();
            } else if (ui->coaxialScan->isChecked()) {
                realSpeed = qDegreesToRadians(speedLimit) * ui->productDiameter->value();
            }
            return realSpeed < speed ? realSpeed : speed;
        } else if (m_motionType == ScanPlan::DISK || m_motionType == ScanPlan::DISKROTATION) {
            return speed;
        }
    }
    return 50;
}

/*! 根据扫描间隔，计算最大扫描速度 */
double ScanPathAdvancedEdit::intervalToSpeed(double interval)
{
    double coefficient = platform()->Get("ScanSpeedCoefficient").toDouble(0.9);
    UTChannel *firstChannel = UltrasonicManager::channel(0);
    if (firstChannel == nullptr) {
        return 0;
    }
    UTSetup setup = firstChannel->setup();
    double prf = 500;
    int sampleAverage = 1;
    prf = setup.prf();
    sampleAverage = 1 << setup.sampleAverage();
    return speedLimit(coefficient * interval * prf / sampleAverage);
}

QList<RotationInfo> ScanPathAdvancedEdit::currentRotations() const
{
    QList<RotationInfo> rotations;
    for (int index = 0 ; index < rotationRootItem->childCount(); ++index) {
        QTreeWidgetItem *item = rotationRootItem->child(index);
        auto *rotationItem = static_cast<RotationWidgetItem *>(item);
        if (rotationItem) {
            rotations << rotationItem->rotationInfo();
        }
    }
    return rotations;
}

/*! 使用推荐速度 */
void ScanPathAdvancedEdit::on_applyAdviceSpeed_clicked()
{
    ui->linearSpeed->setValue(intervalToSpeed(ui->interval->value()));
}

void ScanPathAdvancedEdit::on_addChannel_advice_clicked()
{
    addChannel(ui->channelList_advice);
}

void ScanPathAdvancedEdit::on_removeChannel_advice_clicked()
{
    removeChannel(ui->channelList_advice);
}

void ScanPathAdvancedEdit::on_addChannel_clicked()
{
    addChannel(ui->channelList);
}

void ScanPathAdvancedEdit::on_removeChannel_clicked()
{
    removeChannel(ui->channelList);
}

void ScanPathAdvancedEdit::addChannel(QTreeWidget *tree)
{
    auto *item = new QTreeWidgetItem(tree);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    for (int column = 0; column < tree->columnCount(); ++column) {
        item->setData(column, Qt::DisplayRole, tr("无"));
        item->setData(column, Qt::UserRole, -1);
    }
}

void ScanPathAdvancedEdit::removeChannel(QTreeWidget *tree)
{
    delete tree->takeTopLevelItem(tree->currentIndex().row());
}

void ScanPathAdvancedEdit::on_expand_clicked()
{
    MotionPath path = motionPathFromAdvanced();
    path = path.expandMultiChannel();
    refreshAdvanced(path);
}

void ScanPathAdvancedEdit::on_coaxialScan_toggled(bool checked)
{
    ui->diameterTitle->setVisible(!checked);
    ui->wheelDiameter->setVisible(!checked);
    ui->angularSpeed->setVisible(!checked);
    ui->angularSpeedTitle->setVisible(!checked);
}

void ScanPathAdvancedEdit::refreshSpeed()
{
    qreal linearSpeed = ui->linearSpeed->value();
    qreal adviceSpeed = intervalToSpeed(ui->interval->value());
    ui->adviceSpeed->setText(tr("最大扫描线速度: %1 mm/s").arg(adviceSpeed));

    if (m_motionType == ScanPlan::CYLINDER || m_motionType == ScanPlan::CYLINDERROTATION) {
        qreal wheelRadius = ui->wheelDiameter->value() / 2;
        qreal productRadius = ui->productDiameter->value() / 2;
        if (ui->bearingScan->isChecked()) {
            qreal angularSpeed = linearSpeed / wheelRadius;                                         // 弧度
            if (angularSpeed > 0) {
                ui->angularSpeed->setValue(qRadiansToDegrees(angularSpeed));
                if (productRadius > 0) {
                    ui->productAngularSpeed->setValue(qRadiansToDegrees(angularSpeed) * wheelRadius / productRadius);
                }
            }
        } else if (ui->coaxialScan->isChecked()) {
            if (productRadius > 0) {
                qreal angularSpeed = qRadiansToDegrees(linearSpeed / productRadius);
                ui->angularSpeed->setValue(angularSpeed);
                ui->productAngularSpeed->setValue(angularSpeed);
            }
        }
    }

    static const QString warningStyle("color: #FF0000");
    ui->linearSpeed->setStyleSheet(ui->linearSpeed->value() - adviceSpeed > 1e-6 ? warningStyle : QString());
    Axis *scanAxis = ui->scanAxis->currentAxis();
    if (!scanAxis || !scanAxis->isRotation()) {
        return;
    }
    qreal speedLimit = scanAxis->config().speedMax;
    ui->angularSpeed->setStyleSheet(ui->angularSpeed->value() > speedLimit ? warningStyle : QString());
}

void ScanPathAdvancedEdit::on_setup_clicked()
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
