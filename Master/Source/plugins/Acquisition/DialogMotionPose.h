#pragma once

#include <QDialog>
#include "Motion/MotionPose.h"

namespace Ui {
class DialogMotionPose;
}

class DialogMotionPose : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMotionPose(QWidget *parent = 0);
    ~DialogMotionPose();

public:
    MotionPose GetMostionPose();
    void SetMotionPose(const MotionPose &pos, const MotionAxisList &axes);

private:
    void InitUI();

private:
    Ui::DialogMotionPose *ui;
};
