#pragma once

#include "ScanPathAdvanced.h"
#include "ScanPlan.h"
#include <Motion/Axis.h>
#include <Ultrasonic/UTChannel.h>
#include "Motion/MotionPath.h"
#include <QComboBox>
#include <QDialog>
#include <QJsonObject>
#include <QMenu>
#include <QStyledItemDelegate>
#include <QTreeWidget>
#include <Ultrasonic/UTSetup.h>

class QListWidget;

class ChannelDelegate : public QStyledItemDelegate
{
public:
    ChannelDelegate(QObject *obj) : QStyledItemDelegate(obj) {}

private:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

namespace Ui {
class ScanPathAdvancedEdit;
}
class QListWidgetItem;
class PropertyWidgetItem;
class WidgetItemObject : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
signals:
    void DataChanged(const QVariant &newValue, const QVariant &oldValue);

public:
    WidgetItemObject(QTreeWidgetItem *parent, int type = Type);
    virtual void setData(int column, int role, const QVariant &value);
};

class ConditionWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    ConditionWidgetItem(const MotionCondition &condition, QTreeWidgetItem *parent = nullptr);

public:
    MotionCondition GetMotionCondition();
    void SetCondition(const MotionCondition &condition);

private slots:
    void OnConditionTypeChanged(const QVariant &newType, const QVariant &oldType);

private:
    void RefreshCondition(const MotionCondition &condition);
};

class ElementWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    ElementWidgetItem(const MotionElement &element, const QList<RotationInfo> &rotations, QTreeWidgetItem *parent);
    void SetElement(const MotionElement &element, const QList<RotationInfo> &rotations);
    MotionElement GetElement(const QList<RotationInfo> &rotations) const;

private:
    void OnPropertyChanged(const QVariant &newValue, const QVariant &oldValue);
    PropertyWidgetItem *GetPropertyItemByName(const QString &name);
};

class ScanPathAdvancedEdit : public QWidget
{
    Q_OBJECT

public:
    enum DelegateType {
        Combobox,
        DoubleSpinBox,
        SpinBox
    };
    Q_ENUM(DelegateType)

    enum ValueType {
        ElementType,
        InterpolateType,
        AxisListType,
        ConditionType,
        SensorType,
        DoubleValueType,
        IntValueType,
        RotationDirectionType,
        RotationType,
        SpeedType
    };
    Q_ENUM(ValueType)

    explicit ScanPathAdvancedEdit(QWidget *parent = 0);
    ~ScanPathAdvancedEdit();

    ScanPathAdvancedOptions options() const;
    void setSetupVisibility(bool enabled);

public slots:
    void setOptions(const ScanPathAdvancedOptions &options);

    MotionPath motionPath() const;
    QList<ImageOptions> imageOptions() const;
    ScanPlan::MotionType motionType() {return m_motionType;}

protected:
    virtual bool eventFilter(QObject *watch, QEvent *event);

private slots:
    void onLicenseUpdated();
    /*! 使用motionPath的新接口 */
    void onPathConetextMenuRequested();
    void onPathDoubleClicked(QTreeWidgetItem *item);
    void on_actionAddElement_triggered();
    void on_actionEditElement_triggered();
    void on_actionAddPos_triggered();
    void on_actionEditPos_triggered();
    void on_actionDeleteElement_triggered();
    void on_actionDeletePos_triggered();
    void on_actionAddRotation_triggered();
    void on_actionEditRotation_triggered();
    void on_actionDeleteRotation_triggered();
    void on_actionAddCondition_triggered();
    void on_actionEditCondition_triggered();
    void on_actionDeleteCondition_triggered();
    void on_applyAdviceSpeed_clicked();
    void on_imageOptions_itemDoubleClicked(QListWidgetItem *item);
    void on_removeAdviceImage_clicked();
    void on_addAdviceImage_clicked();
    void on_addChannel_advice_clicked();
    void on_removeChannel_advice_clicked();
    void on_addChannel_clicked();
    void on_removeChannel_clicked();
    void on_expand_clicked();
    void on_coaxialScan_toggled(bool checked);
    void refreshSpeed();
    void on_setup_clicked();

private:
    /*! 使用motionPath的新接口 */
    void refreshAdvanced(const MotionPath &path);
    void refreshBase(const MotionPath &path);
    void refreshChannels(const MotionPath &path, QTreeWidget *tree);
    QList<ChannelList> multiChannels(QTreeWidget *tree) const;
    void refreshElements(const QList<MotionElement> &elements, const QList<RotationInfo> &rotations, QTreeWidgetItem *treeItem);
    void refreshRotations(const QList<RotationInfo> &rotations, QTreeWidgetItem *treeItem);
    MotionPath motionPathFromAdvanced() const;
    MotionPath motionPathFromBase() const;
    double speedLimit(double speed);
    double intervalToSpeed(double interval);
    QList<RotationInfo> currentRotations() const;
    QString imageOptionsDescription(const ImageOptions &options);
    void addImageOptions(const ImageOptions &options);
    ImageOptions defaultImageOptions() const;
    void addChannel(QTreeWidget *tree);
    void removeChannel(QTreeWidget *tree);

private:
    Ui::ScanPathAdvancedEdit *ui;
    ScanPlan::MotionType m_motionType = ScanPlan::NONE;
    MotionPath m_motionPath;
    QTreeWidgetItem *prerequisitesRootItem = nullptr;
    QTreeWidgetItem *elementRootItem = nullptr;
    QTreeWidgetItem *rotationRootItem = nullptr;
    UTSetup m_setup;
};
