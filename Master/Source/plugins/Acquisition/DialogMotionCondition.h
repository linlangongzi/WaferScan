#pragma once

#include <QDialog>
#include "Motion/MotionCondition.h"

namespace Ui {
class DialogMotionCondition;
}

class DialogMotionCondition : public QDialog
{
    Q_OBJECT
public:
    explicit DialogMotionCondition(QWidget *parent = 0);
    ~DialogMotionCondition();

public:
    void SetCondition(const MotionCondition &condition);
    MotionCondition GetCondition() const;

private slots:
    void OnConditionTypeChanged();

private:
    void InitUI();

private:
    Ui::DialogMotionCondition *ui;
};
