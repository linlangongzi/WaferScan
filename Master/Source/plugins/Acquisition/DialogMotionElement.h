#pragma once

#include <QDialog>
#include <QMenu>
#include <Motion/MotionAxisList.h>
#include "Motion/MotionElement.h"

namespace Ui {
class DialogMotionElement;
}

class QListWidgetItem;
class DialogMotionElement : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMotionElement(QWidget *parent = 0);
    ~DialogMotionElement();

public:
    void SetMotionElement(const MotionElement &elemnt, const QList<RotationInfo> &rotations);
    MotionElement GetMotionElement(const QList<RotationInfo> &rotations);

private slots:
    void OnAddPos();
    void OnEditPos();
    void OnDeletePos();
    void OnPosContextMenuRequested();
    void OnAddCondition();
    void OnEditCondition();
    void OnDeleteCondition();
    void OnConditionContextMenuRequested();

private:
    void InitUI();
    void RefreshCondition(const MotionCondition &condition, QListWidgetItem *item);

private:
    Ui::DialogMotionElement *ui;
    QMenu posMenu;
    QMenu conditionMenu;
    MotionElement motionElement;
};
